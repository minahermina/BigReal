#include "BigReal.h"

BigReal::BigReal(double realNumber): BigReal(to_string(realNumber)) {}

BigReal::BigReal(string realNumber) {

    if(realNumber == "1."){
        integerDigits = BigDecimalInt("1");
        fractionDigits = BigDecimalInt("0");
    }else if(realNumber == ".1"){
        integerDigits = BigDecimalInt("0");
        fractionDigits = BigDecimalInt("1");
    }else{
        regex validNumber("^[-+]?([0-9]*\\.[0-9]+|[0-9]+)$");
        if(!regex_match(realNumber, validNumber))
            throw "Invalid Real Number";

        // Setting the Sign
        if(realNumber[0] == '+' || isdigit(realNumber[0])){
            Sign = '+';
        }else if(realNumber[0] == '-' ){
            Sign = '-';
        }

        bool signExist = (!isdigit(realNumber[0]));
        if(signExist)
            realNumber.erase(0, 1);

        if(realNumber.find('.') == string::npos){
            integerDigits = BigDecimalInt(realNumber);
            fractionDigits = BigDecimalInt("0");
        }else{
            // Get the position of decimal point
            int pos = find(realNumber.begin(), realNumber.end(), '.') - realNumber.begin();
            // Getting the integer digits and fraction digits.
            string tempInt = realNumber.substr(0, pos);
            string tempFrac = realNumber.substr(pos + 1, realNumber.size() - pos - 1);
            integerDigits = BigDecimalInt(tempInt);
            fractionDigits = BigDecimalInt(tempFrac);
        }
    }

}

BigReal::BigReal(BigDecimalInt bigInteger) {
    Sign = (bigInteger.isPositive() ? '+': '-');
    integerDigits = bigInteger;
    fractionDigits = BigDecimalInt("0");
}

BigReal::BigReal(BigDecimalInt integer, BigDecimalInt fraction, char sign = '+'):
        integerDigits(integer),
        fractionDigits(fraction),
        Sign(sign) {}

int BigReal::size() const{
    return (this->integerDigits.size() + this->fractionDigits.size());
}

char BigReal::sign() const{
    return (this->Sign);
}

// Assignment Operators
BigReal &BigReal::operator=(const string & str) {
    *this = BigReal(str);
    return *this;
}

BigReal& BigReal::operator=(const BigDecimalInt& num){
    *this = BigReal(num);
    return *this;
}
BigReal &BigReal::operator=(const int & num) {
    *this = BigReal(double(num));
    return *this;
}

BigReal &BigReal::operator=(const long long int & num) {
    *this = BigReal(double(num));
    return *this;
}

bool BigReal::operator<(const BigReal& rhs){
    // Signs are not the same
    if(this->Sign != rhs.Sign){
        if(this->Sign == '+')
            return false;
        else
            return true;
    }
    // Counting zeros on the left of the fraction, to compare between of them
    int zerosCntLeft = 0, zerosCntRight = 0;
    string left = this->fractionDigits.toString();
    string right = rhs.fractionDigits.toString();
    for (char digit : left) {
        if(digit != '0')
            break;
        zerosCntLeft++;
    }
    for (char digit : right) {
        if(digit != '0')
            break;
        zerosCntRight++;
    }
//     Signs are the same, then check integerDigits & fractionDigits
    if(this->integerDigits == rhs.integerDigits){
        if(zerosCntRight == zerosCntLeft){
            return this->fractionDigits > rhs.fractionDigits;
        }
        return zerosCntLeft > zerosCntRight;
    }
    return integerDigits < rhs.integerDigits;
}

bool BigReal::operator>(const BigReal& anotherReal){
    if(*this == anotherReal)
        return false;
    return !(*this < anotherReal);
}

bool BigReal::operator==(const BigReal& anotherReal){
    // Check sign, integerDigits and fractionDigits
    return (Sign == anotherReal.Sign && integerDigits == anotherReal.integerDigits && fractionDigits == anotherReal.fractionDigits);
}

void BigReal::formatFractionsZeros(BigDecimalInt &lhs, BigDecimalInt &rhs) {
    int sizeDifference = (lhs.size() - rhs.size());
    BigDecimalInt result;
    if(sizeDifference > 0){
        string temp = rhs.toString();
        temp.insert(temp.size(), sizeDifference, '0');
        rhs = temp;
    }else{
        string temp = lhs.toString();
        temp.insert(temp.size(), abs(sizeDifference), '0');
        lhs = temp;
    }
}

void BigReal::handleBorrow (BigReal& real, int size){
//    cout << real.fractionDigits <<endl;
    if(real.fractionDigits.isNegative() ){
        string max(size, '9');
        BigDecimalInt one("1"), maxValueForFraction(max);
        // if fraction part is negative, then borrow from integer part and add to fraction.
        real.integerDigits = real.integerDigits - one;
        real.fractionDigits = real.fractionDigits + maxValueForFraction + one;
    }
}
void BigReal::handleCarry (BigReal& real, int size){
    string max(size, '9');
    BigDecimalInt one("1"), maxValueForFraction(max);
    // check if there is a carry
    if(real.fractionDigits.size() > maxValueForFraction.size()) {
        real.integerDigits = real.integerDigits + one;
        string finalFractionResult = real.fractionDigits.toString();
        finalFractionResult.erase(0, 1);
        real.fractionDigits = BigDecimalInt(finalFractionResult);
    }
}

BigReal BigReal::subtract (BigReal lhs, BigReal rhs, int biggerSize){
    BigDecimalInt intResult = lhs.integerDigits - rhs.integerDigits;
    formatFractionsZeros(lhs.fractionDigits, rhs.fractionDigits);
    BigDecimalInt fractionResult = lhs.fractionDigits - rhs.fractionDigits;

    // Handling missing left zeros.
    // e.g. (2.5454) + (-0.54) = 2.0054 NOT 2.54
    int diffSize = biggerSize - fractionResult.size();
    string fractionAfterAddingZeros = fractionResult.toString();
    fractionAfterAddingZeros.insert(0, diffSize, '0');

    if(fractionResult.isNegative())
        fractionAfterAddingZeros.insert(0, "-");

    fractionResult = BigDecimalInt(fractionAfterAddingZeros );

    BigReal answer(intResult, fractionResult);
    handleBorrow(answer, biggerSize);
    return answer ;
}

BigReal BigReal::operator+ (BigReal &rhs){
    BigDecimalInt integerResult, fractionResult;
    BigDecimalInt fraction1 = this->fractionDigits, fraction2 = rhs.fractionDigits;
    BigDecimalInt int1 = this->integerDigits, int2 = rhs.integerDigits;
    BigReal finalResult;
    // Formatting zeros of fraction parts.
    formatFractionsZeros(fraction1, fraction2);

    int biggerSize = max(fraction1.size(), fraction2.size());
    // Signs are the same
    if (this->sign() == rhs.sign()){
        finalResult = BigReal(int1 + int2, fraction1 + fraction2);
        handleCarry(finalResult, biggerSize);//----//
        // e.g. (-a + (-b)) -> -( a + b)
        if(this->sign() == '-')
            finalResult.Sign = '-';
        return finalResult;
    }
    // Signs Differs
    if(this->Sign != rhs.Sign){
        if(this->sign() == '+'){
            // e.g. (5.5) + (-5.5)
            if(int1 == int2 && fraction1 == fraction2)
                return BigReal("0.0");

            bool isLhsBigger = int1 > int2;
            // e.g. 5.8 + (-5.5)
            if(this->integerDigits == rhs.integerDigits)
                isLhsBigger = fraction1 > fraction2;

            // e.g. 5.5 + (-2.8)
            if(isLhsBigger){
                finalResult = subtract(*this, rhs, biggerSize);
            }
                // e.g. 5.5 +(-5.8) = -(5.8 - 5.5)
                // e.g. 5.2 +(-8.8) = -(8.8 - 5.2)
            else{
                finalResult = subtract(rhs, *this, biggerSize);
                finalResult.Sign = '-';
            }
            return finalResult;
        }else {
            return (rhs + (*this));
        }
    }
    return BigReal(integerResult, fractionResult);
}

BigReal BigReal::operator-(BigReal&& rhs){
    return ((*this) - rhs);
}
BigReal BigReal::operator+(BigReal&& rhs){
    return ((*this) + rhs);
}

BigReal BigReal::operator-(BigReal &rhs){
    BigReal result, realNumber1 = *this, realNumber2 = rhs;
    // Signs are the same
    // (-5 -(-5)) = (-5 + 5)
    if(this->Sign == rhs.Sign){
        realNumber2.Sign = (realNumber2.Sign == '+' ? '-' : '+');
        result = (realNumber1 + realNumber2);
    }else{
        realNumber1.Sign = realNumber2.Sign = '+';
        result = (realNumber1 + realNumber2);
        if(this->Sign == '-')
            result.Sign = '-';
    }
    return result;
}

ostream& operator<< (ostream& out, BigReal& num){
    if(num.Sign == '-')
        out << num.Sign;
    out << num.integerDigits << '.' << num.fractionDigits;
    return out;
}

ostream &operator<<(ostream &out, BigReal&& num){
    out << num;
    return out;
}

istream& operator>>(istream& in, BigReal& num){
    string tempNumber;
    in >> tempNumber;
    num = BigReal(tempNumber);
    return in;
}

