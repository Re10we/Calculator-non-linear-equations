#ifndef MBROYDEN_H
#define MBROYDEN_H


#include <string>
#include "Methods/HFiles/Methods.h"
#include "Derivative/expression.h"
#include "Derivative/parser.h"


class MBroyden: public Methods{
public:
   double* solve() override;
    MBroyden(std::vector<std::string> EquationArray, double e, double* startValue): Methods(EquationArray,e,startValue){
        for(uint8_t i=0;i<Equation.size();i++)   parser.SetVariableID("x"+std::to_string(i+1), i);
        StrResult = "";
    };
private:
    std::string** buildJacobianMatrix();
    void getCofactor(double** MainMatrix, double** tempMatrix , int p, int q, int n);
    int  determinant(double** matrix, int n);
    void adjoint(double** matrix,double** adj, int n);
    bool inverse(double** matrix, double** inverseMatrix, int n);
    double multiplicationSMatrix(double* matrixF, int n);
    double* multiplicationMatrix(double** mainMatrix, double* matrixF, int n);
    double* sumMatrix(double* matrix, double* secondMatrix, int n);
    double** sumMatrix(double** matrix, double** secondMatrix, int n);
    double* subMatrix(double* matrix, double* secondMatrix, int n);
    double maxElement(double* arr, int n);
    double** multiplicationSimpleMatrixAndTransposeMatrix(double* simpleMatrix, double* transposeMatrix, int n);
    template <typename Type>
    Type** createMatrix(int n);


    //
    Ev3::ExpressionParser parser;
/*
protected:
    std::vector<std::string> Equation;
    double Approximation;
    std::string StrResult;
    std::vector<double> StartValue;
*/
};

#endif // MBROYDEN_H
