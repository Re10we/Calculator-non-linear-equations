#ifndef METHODS_H
#define METHODS_H

#include <vector>
#include <string>
#include <math.h>
#include "Parser/SimpleParser.h"
#include "Methods/HFiles/InstrumentalClass.h"
#include <algorithm>
#include <float.h>
#include <QVector>


class Methods{
protected:
    std::vector<std::string> Equation;
    double Approximation;

    double* StartValue;
    //std::string IntervalStartValue;
public:
    //Для начальный значений ввиде начальных значений x0,y0,z0
    Methods(std::vector<std::string> EquationArray, double e, double* startValue): Equation(EquationArray), Approximation(e), StartValue(startValue) {
        StrResult = "";
    }
    //Для начальных значений ввиде интервала [a;b]
    //Methods(std::vector<std::string> EquationArray, double e, std::string intervalStartValue): Equation(EquationArray), Approximation(e), IntervalStartValue(intervalStartValue) {StrResult = "";}
    virtual double* solve() = 0;
    std::string StrResult;
    QVector<double> X ;
    QVector<double> Y ;
};

#endif // METHODS_H
