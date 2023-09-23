#include "../HFiles/MSeidel.h"

double* MSeidel::solve(){
    double* oldValue = new double[Equation.size()];
    std::vector<std::string> tempEquation = Equation;
    bool EndIteration = false;

    for(uint8_t i =0;i<Equation.size();i++) InstrumentalClass::SeparateUknown(tempEquation[i],"x"+std::to_string(i+1));
    try{
        do
        {
            X.push_back(StartValue[0]);
            Y.push_back(StartValue[1]);
            for(uint8_t i=0;i<Equation.size();i++) oldValue[i] = StartValue[i];
            for(uint8_t i =0;i<tempEquation.size();i++){
                bool Minus =false;
                std::string str = InstrumentalClass::replaceUknownToValue(tempEquation[i],tempEquation.size(),StartValue);
                if(str[0]=='-'){
                    str = str.erase(0,1);
                    Minus = true;
                }
                Parser pars(str.c_str());
                if(Minus){
                    StartValue[i] = (0 -InstrumentalClass::eval(pars.parse()));
                }
                else{
                    StartValue[i] = InstrumentalClass::eval(pars.parse());
                }

            }

            for(uint8_t i=0;i<Equation.size();i++){
                if(std::fabs(oldValue[i]-StartValue[i])<Approximation) {
                    EndIteration=true;
                    break;
                }
            }

        }while(!EndIteration);

        return StartValue;
    }
    catch(...){
        return NULL;
    }
}
