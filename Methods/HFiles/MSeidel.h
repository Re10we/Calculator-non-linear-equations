#ifndef MSEIDEL_H
#define MSEIDEL_H


#include "Methods/HFiles/Methods.h"

class MSeidel: public Methods{
public:
	double* solve() override;
    MSeidel(std::vector<std::string> EquationArray, double e, double* startValue): Methods(EquationArray,e,startValue){StrResult = "";};

/*
protected:
    std::vector<std::string> Equation;
    double Approximation;
    std::string StrResult;
    std::vector<double> StartValue;
*/ 


};

#endif // MSEIDEL_H
