#include "Methods/HFiles/MNewtona.h"

double* MNewtona::solve(){
    std::string** MatrixJacoby = buildJacobianMatrix();//построил матрицу Якоби
    std::string*  MatrixReplacmentUknowns = new std::string[Equation.size()];//F(x)
    double** MatrixJacobyReplacmentUknowns = createMatrix<double>(Equation.size());
    double*  matrixF = new double[Equation.size()];
    double* DeltaX;
    double  MaxDeltaX = DBL_MAX;
    X.push_back(StartValue[0]);
    Y.push_back(StartValue[1]);
    if(Equation.size()==2){
        while(MaxDeltaX>Approximation){
                StrResult +="Вирішана матриця Якобі: \nW(x)(";
                for(uint8_t i=0;i<Equation.size();i++){
                    StrResult+="(";
                    for(uint8_t j=0;j<Equation.size();j++){
                        std::string tempStr  = InstrumentalClass::replaceUknownToValue(MatrixJacoby[i][j],Equation.size(),StartValue);
                        Parser pars(tempStr.c_str());
                        MatrixJacobyReplacmentUknowns[i][j] = InstrumentalClass::eval(pars.parse());
                        StrResult+= std::to_string(MatrixJacobyReplacmentUknowns[i][j])+", ";
                    }
                StrResult.pop_back();
                StrResult.pop_back();
                StrResult+=")\n";
                }
            StrResult.pop_back();
            StrResult+=")\n";
            for(uint8_t i=0;i<Equation.size();i++){
                MatrixReplacmentUknowns[i] =  InstrumentalClass::replaceUknownToValue(Equation[i],Equation.size(),StartValue);
            }

            //вычисляю F(x)
            StrResult +="F(x):(";
            for(uint8_t i=0;i<Equation.size();i++){
                try {
                    Parser pars(MatrixReplacmentUknowns[i].c_str());
                    matrixF[i] = InstrumentalClass::eval(pars.parse());
                    StrResult+=std::to_string(matrixF[i])+",";
                } catch (...) {
                    return NULL;
                }
            }
            StrResult.pop_back();
            StrResult.pop_back();
            StrResult += ")\n";
            double**  unionMatrix = unionMatrixAndMatrixF(MatrixJacobyReplacmentUknowns,matrixF,Equation.size());
            int flag = performOperation(unionMatrix, Equation.size());
            if (flag == 1)
                flag = checkConsistency(unionMatrix, Equation.size(), flag);
            DeltaX = resultJordanGausse(unionMatrix,Equation.size(),flag);
            if(DeltaX==NULL) return NULL;
            StartValue = sumMatrix(StartValue,DeltaX,Equation.size());
            MaxDeltaX = maxElement(DeltaX,Equation.size());
            X.push_back(StartValue[0]);
            Y.push_back(StartValue[1]);
        }
        return StartValue;
    }
    else{
        for(uint8_t i=0;i<Equation.size();i++){
            MatrixReplacmentUknowns[i] =  InstrumentalClass::replaceUknownToValue(Equation[i],Equation.size(),StartValue);
        }
        //создаю переменную которая будет хранить уже вычисленные значения матрици Якоби
        double** IMatrix;//W^-1(x)
        StrResult +="Вирішана матриця Якобі:\nW(x)(";
        for(uint8_t i=0;i<Equation.size();i++){
            StrResult+="(";
            for(uint8_t j=0;j<Equation.size();j++){
                //Заменяю в строке неизвестную на значение
                //передаю эту строку в парсер получаю значение
                //это значение передаёться в нужную матрицу
                std::string tempStr  = InstrumentalClass::replaceUknownToValue(MatrixJacoby[i][j],Equation.size(),StartValue);
                Parser pars(tempStr.c_str());
                MatrixJacobyReplacmentUknowns[i][j] = InstrumentalClass::eval(pars.parse());
                StrResult+= std::to_string(MatrixJacobyReplacmentUknowns[i][j])+", ";
            }
            StrResult.pop_back();
            StrResult+=")\n";
        }
        StrResult+=")\n";
        //вычисляю F(x)
        StrResult +="F(x):(";
        for(uint8_t i=0;i<Equation.size();i++){

            Parser pars(MatrixReplacmentUknowns[i].c_str());
            try {
                matrixF[i] = InstrumentalClass::eval(pars.parse());
                StrResult+=std::to_string(matrixF[i])+",";
            } catch (...) {
                return NULL;
            }
        }
        StrResult.pop_back();
        StrResult += ")\n";
        /////
        IMatrix = inverseMatrix(MatrixJacobyReplacmentUknowns,Equation.size());
        DeltaX =  multiplicationMatrix(IMatrix,matrixF,Equation.size());
        StartValue = sumMatrix(StartValue,DeltaX,Equation.size());
        MaxDeltaX = maxElement(DeltaX,Equation.size());
        if(MaxDeltaX<=Approximation){
            return StartValue;
        }
        else{
            while(MaxDeltaX>Approximation){
                    StrResult +="Вирішана матриця Якобі: W(x)(";
                    for(uint8_t i=0;i<Equation.size();i++){
                        StrResult+="(";
                        for(uint8_t j=0;j<Equation.size();j++){
                            std::string tempStr  = InstrumentalClass::replaceUknownToValue(MatrixJacoby[i][j],Equation.size(),StartValue);
                            Parser pars(tempStr.c_str());
                            MatrixJacobyReplacmentUknowns[i][j] = InstrumentalClass::eval(pars.parse());
                            StrResult+= std::to_string(MatrixJacobyReplacmentUknowns[i][j])+", ";
                        }
                    StrResult.pop_back();
                    StrResult+=")\n";
                    }
                StrResult+=")\n";
                for(uint8_t i=0;i<Equation.size();i++){
                    MatrixReplacmentUknowns[i] =  InstrumentalClass::replaceUknownToValue(Equation[i],Equation.size(),StartValue);
                }

                //вычисляю F(x)
                StrResult +="F(x):(";
                for(uint8_t i=0;i<Equation.size();i++){
                    Parser pars(MatrixReplacmentUknowns[i].c_str());
                    matrixF[i] = InstrumentalClass::eval(pars.parse());
                    StrResult+=std::to_string(matrixF[i])+",";
                }
                StrResult.pop_back();
                StrResult += ")\n";
                double**  unionMatrix = unionMatrixAndMatrixF(MatrixJacobyReplacmentUknowns,matrixF,Equation.size());
                int flag = performOperation(unionMatrix, Equation.size());
                if (flag == 1)
                    flag = checkConsistency(unionMatrix, Equation.size(), flag);
                DeltaX = resultJordanGausse(unionMatrix,Equation.size(),flag);
                if(DeltaX==NULL) return NULL;
                StartValue = sumMatrix(StartValue,DeltaX,Equation.size());
                MaxDeltaX = maxElement(DeltaX,Equation.size());
            }
            return StartValue;
        }
    }


}

std::string** MNewtona::buildJacobianMatrix(){
	std::string** resultMatrix = createMatrix<std::string>(Equation.size());
    int nerr=0;
	StrResult += "Матриця Якобі має вигляд:\nW(x):\n(";
	for(uint8_t i=0;i<Equation.size();i++){
        StrResult +="(";
		for(uint8_t j=0;j<Equation.size();j++){
            Ev3::Expression expr = parser.Parse(Equation[i].c_str(), nerr);
            Ev3::Expression derivative = Ev3::Diff(expr, j);
            resultMatrix[i][j] = derivative->ToString();
			StrResult+=resultMatrix[i][j]+", ";
		}
        StrResult.pop_back();
        StrResult.pop_back();
        StrResult +=")\n";
	}
    StrResult.pop_back();
	StrResult+=")\n";
	return resultMatrix;
}


template <typename Type>
Type** MNewtona::createMatrix(int n){
     Type** matrix = new Type*[n];
     for(uint8_t i=0;i<n;i++) matrix[i] = new Type[n];
   	 return matrix;
}

int MNewtona::determinate(double** matrix, int n){
   int det = 0;
    if (n == 2)
   		return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
   else {
   	   double** submatrix = createMatrix<double>(n);
       for (int x = 0; x < n; x++) {
         int subi = 0;
         for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
               if (j == x)
               continue;
               submatrix[subi][subj] = matrix[i][j];
               subj++;
            }
            subi++;
         }
         det = det + (pow(-1, x) * matrix[0][x] * determinate( submatrix, n - 1 ));
      }
   }
   return det;
}

double** MNewtona::inverseMatrix(double** matrix, int n){
	double** minorMatrix = createMatrix<double>(n);
	double** cofactorMatrix = createMatrix<double>(n);
		int col3, row3, row2, col2, row, col;
		for (row3=0; row3 < n; row3++){
			for (col3=0; col3 < n; col3++){
				row2 =0;
				col2 = 0;
				for (row=0; row < n; row++){
					for (col=0; col < n; col++){
						if (row != row3 && col != col3){
							minorMatrix[row2][col2] = matrix[row][col];
							if (col2 < (n - 2)){
								col2++;
							}
							else {
								col2 = 0;
								row2++;
							}
						}
					}
				}
				cofactorMatrix[row3][col3] = pow(-1, (row3 + col3)) * determinate(minorMatrix, (n - 1));
			}
		}
        return transpose(cofactorMatrix,matrix,n);	// function
		//return;
}


double** MNewtona::transpose(double** cofactorMatrix,double** matrix, int n){
		int row, col;
		double** resultMatrix = createMatrix<double>(n);
		for (row=0; row < n; row++){
			for (col=0; col < n; col++){
				//transposeMatrix[row][col] = cofactorMatrix[col][row];//transpose matrix
				resultMatrix[row][col] = cofactorMatrix[col][row]/determinate(matrix,n); // adjoint method
			}
		}
		return resultMatrix;
}

//умножение двох матриц matrix[n][n]*matrix2[n]
double* MNewtona::multiplicationMatrix(double** mainMatrix, double* matrixF, int n){
	double* resultMatrix = new double[n];
	for (int i = 0; i < n; i++){            
        resultMatrix[i] = 0;
        for (int j = 0; j < n; j++){
            resultMatrix[i] += (0 -mainMatrix[i][j]) * matrixF[j];
        }
    }
    return resultMatrix;
}

double* MNewtona::sumMatrix(double* matrix, double* secondMatrix, int n){
    double* resultSumMatrix = new double[n];
	for(uint8_t i=0;i<n;i++){
        resultSumMatrix[i] = matrix[i]+secondMatrix[i];
	}
	return resultSumMatrix;
}


// Функция для приведения матрицы к уменьшенной форме эшелона строк.
int MNewtona::performOperation(double** matrix, int n)
{
    int i, j, k = 0, c, flag = 0;
    // Выполнение элементарных операций
    for (i = 0; i < n; i++)
    {
        if (matrix[i][i] == 0)
        {
            c = 1;
            while ((i + c) < n && matrix[i + c][i] == 0)
                c++;           
            if ((i + c) == n) {
                flag = 1;
                break;
            }
            for (j = i, k = 0; k <= n; k++)
                std::swap(matrix[j][k], matrix[j+c][k]);
        }
 
        for (j = 0; j < n; j++) {
             
            // Исключая все i == j
            if (i != j) {
                 
                // Преобразование матрицы в уменьшенную форму эшелона строк (диагональная матрица)
                double pro = matrix[j][i] / matrix[i][i];
 
                for (k = 0; k <= n; k++)                
                    matrix[j][k] = matrix[j][k] - (matrix[i][k]) * pro;               
            }
        }
    }
    return flag;
}
 
double* MNewtona::resultJordanGausse(double** matrix, int n, int flag)
{
    if (flag == 2 || flag==3)    return NULL;     
    // Вычисление решения путем деления констант на их соответствующие диагональные элементы
    else {
    	double* resultMatrix = new double[n];
        for (int i = 0; i < n; i++)        
            resultMatrix[i] =  0 - (matrix[i][n] / matrix[i][i]);
        return resultMatrix;       
    }

}
 
//Проверка существуют ли бесконечные решения или решения не существует
int MNewtona::checkConsistency(double** matrix, int n, int flag)
{
    int i, j;
    float sum;
     
    // flag == 2 бесконечные решения
    // flag == 3 нету решения
    flag = 3;
    for (i = 0; i < n; i++)
    {
        sum = 0;
        for (j = 0; j < n; j++)       
            sum = sum + matrix[i][j];
        if (sum == matrix[i][j])
            flag = 2;       
    }
    return flag;
}


//обьединение двух матриц
double** MNewtona::unionMatrixAndMatrixF(double** matrix, double* matrixF, int n){
    double** UnionMatrix = createMatrix<double>(n);
	for(int i=0;i<n;i++){
		for(int j=0;j<=n;j++){
			if(j<n) UnionMatrix[i][j] = matrix[i][j];
			else UnionMatrix[i][j] = matrixF[i];
		}
	}

	return UnionMatrix;
}


double MNewtona::maxElement(double* arr, int n){
	double maxElem = DBL_MIN; 
	for(uint8_t i=0;i<n;i++){
		if(maxElem<std::abs(arr[i])){
			maxElem = std::abs(arr[i]);
		}
	}
    return maxElem;
}
