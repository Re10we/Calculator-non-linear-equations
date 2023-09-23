#include "../HFiles/InstrumentalClass.h"
#include <climits>

std::string InstrumentalClass::replaceUknownToValue(std::string MainStr, int countUknown, double* value){
	for(uint8_t i=0;i<countUknown;i++){
        std::string Uknown = "x"+std::to_string(i+1);
		while(MainStr.find(Uknown)!=std::string::npos){
			MainStr.replace(MainStr.find(Uknown),Uknown.size(),std::to_string(value[i]));
		}
	}
	return MainStr;
}



double InstrumentalClass::eval(const Expression& e) {
    switch (e.args.size()) {
    case 2: {
        auto a = eval(e.args[0]);
        auto b = eval(e.args[1]);
        if (e.token == "+") return a + b;
        if (e.token == "-") return a - b;
        if (e.token == "*") return a * b;
        if (e.token == "/") return a / b;
        if (e.token == "^") return pow(abs(a), b);
        if (e.token == "mod") return (int)a % (int)b;
        throw std::runtime_error("Unknown binary operator");
    }

    case 1: {
        auto a = eval(e.args[0]);
        if (e.token == "+") return +a;
        if (e.token == "-") return -a;
        if (e.token == "abs") return abs(a);
        if (e.token == "sin") return sin(a);
        if (e.token == "cos") return cos(a);
        throw std::runtime_error("Unknown unary operator");
    }

    case 0:
        return strtod(e.token.c_str(), nullptr);
    }

    throw std::runtime_error("Unknown expression type");
}

void InstrumentalClass::SeparateUknown(std::string& StrEquation, std::string Uknown){
    //x1+x2-x1*x2+x1^3 - 12
    bool Minus = false;
    bool firstWithStr = false;
    if(StrEquation.find(Uknown+"^")!=std::string::npos){
        std::vector<uint8_t> posArray;
        //Ищу все подстроки с оператором "^"
        for(uint8_t pos=0;StrEquation.find(Uknown+"^",pos)!=std::string::npos;pos+=Uknown.size()+2){
            pos = StrEquation.find(Uknown+"^",pos);
            posArray.push_back(StrEquation.find(Uknown+"^",pos));
        }
        if(posArray.size()==1){
            if(posArray[0]==0 || posArray[0]==1) firstWithStr = true;
            std::string power = "";
            std::string number = "", reverseNumber="";
            int startErasePos = posArray[0];
            uint8_t len=1+Uknown.size();
            for(uint8_t pos = posArray[0]+1+Uknown.size();isdigit(StrEquation[pos]);pos++){
                power+=StrEquation[pos];
                len++;
            }
            for(int pos = posArray[0];StrEquation[pos]!='-' && StrEquation[pos]!='+' && pos>=0 ;pos--){
                if(StrEquation[pos]=='+')Minus = true;
                if(isdigit(StrEquation[pos]) || StrEquation[pos]=='.') number+=StrEquation[pos];
                startErasePos--;
                len++;
            }
            if(startErasePos==-1){
                startErasePos++;
                firstWithStr = true;
            }
            StrEquation = StrEquation.erase(startErasePos,len);
            if(firstWithStr) StrEquation = StrEquation.erase(0,1);
            if(number.size()!=0){
                for(uint8_t i=number.size()-1;i>0;i--) reverseNumber+=number[i];
                reverseNumber+=number[0];
                StrEquation = "("+StrEquation+")"+"/"+reverseNumber;
            }
            StrEquation = "("+StrEquation+")"+"^(1/"+power+")";
        }
        else{
            std::vector<int> powerArr;
            int indexPower=0;
            std::string number = "", reverseNumber="";
            int startErasePos;
            uint8_t len=1+Uknown.size();
            for(uint8_t i=0;i<posArray.size();i++){
                std::string power ="";
                for(uint8_t pos = posArray[i]+1+Uknown.size();isdigit(StrEquation[pos]);pos++){
                    power+=StrEquation[pos];
                }
                powerArr.push_back(std::stoi(power));
            }
            indexPower = indexMaxElement(powerArr);

            len += std::to_string(powerArr[indexPower]).size();
            startErasePos = posArray[indexPower];
            for(int pos = posArray[indexPower];StrEquation[pos]!='-' && StrEquation[pos]!='+' && pos>=0 ;pos--){
                if(StrEquation[pos]=='+')Minus = true;
                if(isdigit(StrEquation[pos]) || StrEquation[pos]=='.') number+=StrEquation[pos];
                startErasePos--;
                len++;
            }
            if(startErasePos==-1){
                startErasePos++;
                firstWithStr = true;
            }
            StrEquation = StrEquation.erase(startErasePos,len);
            if(firstWithStr) StrEquation = StrEquation.erase(0,1);
            if(number.size()!=0){
                for(uint8_t i=number.size()-1;i>0;i--) reverseNumber+=number[i];
                reverseNumber+=number[0];
                StrEquation = "("+StrEquation+")"+"/"+reverseNumber;
            }
            StrEquation = "("+StrEquation+")"+"^(1/"+std::to_string(powerArr[indexPower])+")";
        }
    }
    else{
        uint8_t indexUknown = StrEquation.find(Uknown);
        std::string number = "", reverseNumber="";
        uint8_t len = Uknown.size()-1;
        int startErasePos = indexUknown;
        for(int i=indexUknown;i>=0 && StrEquation[i]!='-' && StrEquation[i]!='+';i--){
            if(StrEquation[i]=='+') Minus = true;
            if(isdigit(StrEquation[i]) || StrEquation[i]=='.') number+=StrEquation[i];
            startErasePos--;
            len++;
        }
        if(startErasePos==-1){
            startErasePos++;
            firstWithStr = true;
        }
        StrEquation = StrEquation.erase(startErasePos,len);
        if(firstWithStr) StrEquation = StrEquation.erase(0,1);
        if(number.size()!=0){
            for(uint8_t i=number.size()-1;i>0;i--) reverseNumber+=number[i];
            reverseNumber+=number[0];
            StrEquation = "("+StrEquation+")"+"/"+reverseNumber;
        }
    }

    if(Minus || firstWithStr) StrEquation.insert(0,"-");
}


int InstrumentalClass::indexMaxElement(std::vector<int> Arr){
    int index=0;
    int maxValue = INT_MAX;
    for(uint16_t i=0;i<Arr.size();i++){
        if(Arr[i]>maxValue){
            index = i;
            maxValue = Arr[i];
        }
    }
    return index;
}

