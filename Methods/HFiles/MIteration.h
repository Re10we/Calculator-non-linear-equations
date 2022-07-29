#ifndef MITERATION_H
#define MITERATION_H


#include <string>
#include "Methods/HFiles/Methods.h"


class MIteration: public Methods{
public:
    double* solve() override;
    MIteration(std::vector<std::string> EquationArray, double e, double* startValue): Methods(EquationArray,e,startValue){StrResult = "";};

/*
protected:
    std::vector<std::string> Equation;
    double Approximation;
    std::string StrResult;
    std::vector<double> StartValue;
*/ 

};

#endif // MITERATION_H
