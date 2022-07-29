#include "Methods/HFiles/MBroyden.h"

double* MBroyden::solve(){
  std::string** JacobyMatrix = buildJacobianMatrix();
  double* matrixF0 = new double[Equation.size()];
  double* matrixF1 = new double[Equation.size()];
  double* s0 = new double[Equation.size()];
  double* y0 = new double[Equation.size()];
  double** A0 = createMatrix<double>(Equation.size());
  double** InverseA0 = createMatrix<double>(Equation.size());
  double* oldValue = new double[Equation.size()];
  double maxElemS0;
  X.push_back(StartValue[0]);
  Y.push_back(StartValue[1]);
  try{
      for(uint8_t i=0;i<Equation.size();i++){
            for(uint8_t j=0;j<Equation.size();j++){
                std::string tempStr  = InstrumentalClass::replaceUknownToValue(JacobyMatrix[i][j],Equation.size(),StartValue);
                Parser pars(tempStr.c_str());
                A0[i][j] = InstrumentalClass::eval(pars.parse());
            }

        }
      for(uint8_t i=0;i<Equation.size();i++){
            std::string str = InstrumentalClass::replaceUknownToValue(Equation[i],Equation.size(),StartValue);
            Parser pars(str.c_str());
            matrixF0[i] = InstrumentalClass::eval(pars.parse());
      }
      inverse(A0,InverseA0,Equation.size());
      //InverseA0 = inverseMatrix(A0,Equation.size());
      s0 = multiplicationMatrix(InverseA0,matrixF0,Equation.size());
      for(uint8_t i=0;i<Equation.size();i++){
        oldValue[i] = StartValue[i];
      }
      StartValue = sumMatrix(StartValue,s0,Equation.size());
      maxElemS0 = maxElement(s0,Equation.size());
      while(maxElemS0>Approximation){
        for(uint8_t i=0;i<Equation.size();i++){
            std::string str = InstrumentalClass::replaceUknownToValue(Equation[i],Equation.size(),StartValue);
            Parser pars(str.c_str());
            matrixF1[i] = InstrumentalClass::eval(pars.parse());
        }
        y0 = subMatrix(matrixF1,matrixF0,Equation.size());

        double* Suby0 = subMatrix(y0,multiplicationMatrix(A0,s0,Equation.size()),Equation.size());//y0-A0*s0
        double** MultMatrixAndTranspose = multiplicationSimpleMatrixAndTransposeMatrix(Suby0,s0,Equation.size());
        A0 = sumMatrix(A0,MultMatrixAndTranspose,Equation.size());
        inverse(A0,InverseA0,Equation.size());
        //InverseA0 = inverseMatrix(A0,Equation.size());
        for(uint8_t i=0;i<Equation.size();i++){
            matrixF0[i] = matrixF1[i];
        }
          for(uint8_t i=0;i<Equation.size();i++){
            oldValue[i] = StartValue[i];
        }
        s0 = multiplicationMatrix(InverseA0,matrixF0,Equation.size());
        StartValue = sumMatrix(StartValue,s0,Equation.size());
        maxElemS0 = maxElement(s0,Equation.size());
        X.push_back(StartValue[0]);
        Y.push_back(StartValue[1]);
      }
      return StartValue;
  }
  catch(...){
      return NULL;
  }

}


std::string** MBroyden::buildJacobianMatrix(){
	std::string** resultMatrix = createMatrix<std::string>(Equation.size());
    int nerr=0;
	StrResult += "Матриця Якобі має вигляд:\nW(x)(";
	for(uint8_t i=0;i<Equation.size();i++){
        StrResult +="(";
		for(uint8_t j=0;j<Equation.size();j++){
            Ev3::Expression expr = parser.Parse(Equation[i].c_str(), nerr);
            Ev3::Expression derivative = Ev3::Diff(expr, j);
            resultMatrix[i][j] = derivative->ToString();
			StrResult+=resultMatrix[i][j]+", ";
		}
        StrResult.pop_back();
        StrResult +=")\n";
	}
	StrResult+=")\n";
	return resultMatrix;
}


template <typename Type>
Type** MBroyden::createMatrix(int n){
     Type** matrix = new Type*[n];
     for(uint8_t i=0;i<n;i++) matrix[i] = new Type[n];
   	 return matrix;
}

void MBroyden::getCofactor(double** MainMatrix, double** tempMatrix , int p, int q, int n)
{
    int i = 0, j = 0;
 
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                tempMatrix[i][j++] = MainMatrix[row][col];
 
                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}
 
/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
int MBroyden::determinant(double** matrix, int n)
{
    int D = 0; // Initialize result
 
    //  Base case : if matrix contains single element
    if (n == 1)
        return matrix[0][0];
 
    double** temp = createMatrix<double>(n); // To store cofactors
 
    int sign = 1;  // To store sign multiplier
 
     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(matrix, temp, 0, f, n);
        D += sign * matrix[0][f] * determinant(temp, n - 1);
 
        // terms are to be added with alternate sign
        sign = -sign;
    }
 
    return D;
}
 
// Function to get adjoint of A[N][N] in adj[N][N].
void MBroyden::adjoint(double** matrix,double** adj, int n)
{
    if (n == 1)
    {
        adj[0][0] = 1;
        return;
    }
 
    // temp is used to store cofactors of A[][]
    int sign = 1;
    double** temp = createMatrix<double>(n);
 
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(matrix, temp, i, j, n);
 
            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
 
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign)*(determinant(temp, n-1));
        }
    }
}
 
// Function to calculate and store inverse, returns false if
// matrix is singular
bool MBroyden::inverse(double** matrix, double** inverseMatrix, int n)
{
    // Find determinant of A[][]
    int det = determinant(matrix, n);
    if (det == 0)
    {
        return false;
    }
 
    // Find adjoint
    double** adj = createMatrix<double>(n);
    adjoint(matrix, adj,n);
 
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            inverseMatrix[i][j] = adj[i][j]/double(det);
 
    return true;
}

//умножение двох матриц matrix[n][n]*matrix2[n]
double* MBroyden::multiplicationMatrix(double** mainMatrix, double* matrixF, int n){
	double* resultMatrix = new double[n];
	for (int i = 0; i < n; i++){            
        resultMatrix[i] = 0;
        for (int j = 0; j < n; j++){
            resultMatrix[i] += (0 -mainMatrix[i][j]) * matrixF[j];
        }
    }
    return resultMatrix;
}

double* MBroyden::sumMatrix(double* matrix, double* secondMatrix, int n){
    double* resultSumMatrix = new double[n];
	for(uint8_t i=0;i<n;i++){
        resultSumMatrix[i] = matrix[i]+secondMatrix[i];
	}
	return resultSumMatrix;
}

double** MBroyden::sumMatrix(double** matrix, double** secondMatrix, int n){
    double** resultSumMatrix = createMatrix<double>(n);
	for(uint8_t i=0;i<n;i++){
        for(uint8_t j=0;j<n;j++){
        	resultSumMatrix[i][j] = matrix[i][j] + secondMatrix[i][j];
        }
	}
	return resultSumMatrix;
}


double* MBroyden::subMatrix(double* matrix, double* secondMatrix, int n){
    double* resultSumMatrix = new double[n];
	for(uint8_t i=0;i<n;i++){
        resultSumMatrix[i] = matrix[i]-secondMatrix[i];
	}
	return resultSumMatrix;
}



double MBroyden::maxElement(double* arr, int n){
	double maxElem = DBL_MIN; 
	for(uint8_t i=0;i<n;i++){
		if(maxElem<std::abs(arr[i])){
			maxElem = std::abs(arr[i]);
		}
	}
    return maxElem;
}


double** MBroyden::multiplicationSimpleMatrixAndTransposeMatrix(double* simpleMatrix, double* transposeMatrix, int n){
	double** result = createMatrix<double>(n);
	for(uint8_t i=0;i<n;i++){
		for(uint8_t j=0;j<n;j++){
			result[i][j] = simpleMatrix[i]*transposeMatrix[j];
		}
	}
	return result;
}
