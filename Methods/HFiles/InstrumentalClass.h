#ifndef INSTRUMENTALCLASS_H
#define INSTRUMENTALCLASS_H


#include <string>
#include "Parser/SimpleParser.h"

class InstrumentalClass{
public:
    //Замена неизвестных значениями
    static std::string replaceUknownToValue(std::string MainStr, int countUknown, double* value);
    static double eval(const Expression& e);
    static void SeparateUknown(std::string& StrEquation, std::string Uknown);
    static int indexMaxElement(std::vector<int> Arr);
};

#endif // INSTRUMENTALCLASS_H
