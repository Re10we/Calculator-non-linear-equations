#ifndef MNEWTONA_H
#define MNEWTONA_H

#include "Methods.h"
#include "../../Derivative/expression.h"
#include "../../Derivative/parser.h"


class MNewtona: public Methods{
public:
    double* solve() override;
    MNewtona(std::vector<std::string> EquationArray, double e, double* startValue): Methods(EquationArray,e,startValue) {
        StrResult = "";
          for(uint8_t i=0;i<Equation.size();i++)   parser.SetVariableID("x"+std::to_string(i+1), i);
    };
private:
    std::string** buildJacobianMatrix();
    int determinate(double** matrix, int n);//детерминант матрици
    double maxElement(double* arr, int n);
    double** inverseMatrix(double** matrix, int n);//инвертировая матрица
    double** transpose(double** cofactorMatrix, double** matrix, int n);//транспортирование матрици
    double*  multiplicationMatrix(double** mainMatrix, double* matrixF, int n);
    double** unionMatrixAndMatrixF(double** matrix, double* matrixF, int n);//обьединение двух матриц
    double* sumMatrix(double* matrix, double* secondMatrix, int n);
    //Метод Жордана-Гауса для решения СЛАУ
    int performOperation(double** matrix, int n);//Функция для приведения матрицы к уменьшенной форме эшелона строк
    int checkConsistency(double** matrix, int n, int flag);//Проверка существуют ли бесконечные решения или решения не существует
    double* resultJordanGausse(double** matrix, int n, int flag);//Расчёт всех неизвестных
    //////////////////////////////////////
    template <typename Type>
    Type** createMatrix(int n);//создание матрици
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

#endif // MNEWTONA_H
