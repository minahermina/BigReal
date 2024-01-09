#include "BigDecimalInt.h"

BigDecimalInt::BigDecimalInt(string decStr){

    regex validNumber("^[+-]?[0-9]+$");
    if (!regex_match(decStr, validNumber))
        throw "Invalid Number, found characters except digits and '-', '+' ";

    bool signExist = (decStr[0] == '-' || decStr[0] == '+');

    // if number is all zeroes we will start erasing from position 1 e.g. (000000)
    regex allZeroes("^[0]+$");
    if (regex_match(decStr, allZeroes))
        signExist = true;

    // Removing Trailing Zeroes e.g. (000187)
    int lengthToRemove = signExist;
    while (decStr[lengthToRemove] == '0'){
        lengthToRemove++;
    }
    decStr.erase(signExist, lengthToRemove - signExist);

    // Seperating the minusOrPlus from the digits
    if (decStr[0] == '-'){
        Sign = '-';
        digits = decStr.substr(1, decStr.size() - 1);
    }else if (decStr[0] == '+'){
        Sign = '+';
        digits = decStr.substr(1, decStr.size() - 1);
    }else{
        Sign = '+';
        digits = decStr;
    }
}

BigDecimalInt::BigDecimalInt(const int& x) :
        BigDecimalInt(to_string(x)) {}

BigDecimalInt::BigDecimalInt(const long long& x) :
        BigDecimalInt(to_string(x)) {}


int BigDecimalInt::size() const{
    return digits.size();
}

bool BigDecimalInt::isNegative() const{
    return (Sign == '-');
}

bool BigDecimalInt::isPositive() const{
    return (Sign == '+');
}

int BigDecimalInt::getDigit(int index) const{
    if (index >= 0 && index < this->digits.size())
        return this->digits[index] - '0';

    // return 0 if index not in the size of digits
    return 0;
}

BigDecimalInt BigDecimalInt::ninesComplement(int len) const{
    string result = "";
    int sizeDifference = len - this->size();
    // Subtract each digit from 9
    for (int i = len - 1; i >= 0; i--){
        if (i >= sizeDifference)
            result += char(9 - (this->digits[i - sizeDifference] - '0') + '0');
        else
            result += char(9 + '0');
    }

    reverse(result.begin(), result.end());
    return BigDecimalInt(result);
}

string BigDecimalInt::performAddition(const BigDecimalInt &num1, const BigDecimalInt &num2) const{
    string result = "";
    int sum, len, size_diff;
    // Storing the bigger size between num1 and num2
    len = (num1.size() > num2.size()) ? num1.size() : num2.size();
    // Calculate the differnce in size of num1, num2
    size_diff = abs(int(num1.size() - num2.size()));

    bool carry = false, is_num1_longer = (num1.size() > num2.size());
    for (int i = len - 1; i >= 0; i--){
        if (is_num1_longer)
            sum = num1.getDigit(i) + num2.getDigit(i - size_diff) + carry;
        else
            sum = num1.getDigit(i - size_diff) + num2.getDigit(i) + carry;

        carry = (sum > 9);
        sum %= 10;
        result += char(sum + '0');
    }
    if (carry != 0)
        result += char(carry + '0');

    reverse(result.begin(), result.end());
    return result;
}

string BigDecimalInt::performSubtraction(const BigDecimalInt & num1, const BigDecimalInt & num2) const{
    string result, gf;

    // Getting the bigger length between num1 and num2
    int lengthToLoop = (num1.size() > num2.size()) ? num1.size() : num2.size();

    // Getting 9's complement of num2 then add it to num1
    BigDecimalInt temp = num2.ninesComplement(lengthToLoop);
    result = performAddition(num1, temp);
    BigDecimalInt one("1"), l(result);
    gf = performAddition(l, one);
    // Removing the carry if exist
    if (gf.size() > lengthToLoop)
        gf.erase(0, 1);
    return gf;
}


//Assignment Operators
BigDecimalInt &BigDecimalInt::operator=(const BigDecimalInt & rhs) = default;

BigDecimalInt &BigDecimalInt::operator=(const long long & rhs) {
    *this = BigDecimalInt(rhs);
    return *this;
}

BigDecimalInt &BigDecimalInt::operator=(const string & rhs) {
    *this = BigDecimalInt(rhs);
    return *this;
}


// Increment & Decrement Operators
BigDecimalInt &BigDecimalInt::operator++() {
    *this = (*this) + BigDecimalInt(1);
    return *this;
}

BigDecimalInt &BigDecimalInt::operator--() {
    *this = (*this) - BigDecimalInt(1);
    return *this;
}

BigDecimalInt BigDecimalInt::operator++(int) const{
    BigDecimalInt temp = *this + BigDecimalInt(1);
    return temp;
}

BigDecimalInt BigDecimalInt::operator--(int) const{
    BigDecimalInt temp = *this - BigDecimalInt(1);
    return temp;
}


// Binary arithmetic Operators
BigDecimalInt BigDecimalInt::operator+(const BigDecimalInt &rhs)const{
    string s;
    BigDecimalInt temp, zero("0");
    if (this->isPositive() && rhs.isPositive())
        s = performAddition(*this, rhs);

        // both numbers are negative e.g. (-3 + (-7)) -> -(3 + 7)
    else if (this->isNegative() && rhs.isNegative()){
        s = performAddition(*this, rhs);
        s.insert(0, 1, '-');
    }
        // e.g. (-3 + 7) -> (7 - 3)
    else if (this->isNegative() && rhs.isPositive()){
        temp = zero - *this;
        if (rhs < temp)
        {
            s = performSubtraction(temp, rhs);
            s.insert(0, 1, '-');
        }
        else if (rhs > temp || temp == rhs)
        {
            s = performSubtraction(rhs, temp);
        }
    }
        // e.g. (3 + (-7)) -> (3 - 7)
    else if (this->isPositive() && rhs.isNegative()){

        temp = zero - rhs;
        if (*this > temp || *this == temp){
            s = performSubtraction(*this, temp);
        }else if (*this < temp){
            s = performSubtraction(temp, *this);
            s.insert(0, 1, '-');
        }
    }
    return BigDecimalInt(s);
}

BigDecimalInt BigDecimalInt::operator-(const BigDecimalInt &rhs)const{
    string s = "";
    // e.g. (3 -(-9)) -> 3 + 9
    if (this->isPositive() && rhs.isNegative()){
        s = performAddition(*this, rhs);
    }
        //  e.g. (-3 - (+9)) --> -(3 + 9)
    else if (this->isNegative() && rhs.isPositive()){
        s = performAddition(*this, rhs);
        s.insert(0, 1, '-');
    }
        // e.g. (9 - 3)
    else if (this->isPositive() && rhs.isPositive() && *this > rhs || *this == rhs){
        s = performSubtraction(*this, rhs);
    }
        // e.g. 15 - 18 --> -(18 - 15)
    else if (this->isPositive() && rhs.isPositive() && *this < rhs){
        s = performSubtraction(rhs, *this);
        s.insert(0, 1, '-');
    }
        // e.g. -19 -(-3) -> -(19 - 3)
    else if (this->isNegative() && rhs.isNegative() && *this < rhs){
        s = performSubtraction(*this, rhs);
        s.insert(0, 1, '-');
    }
        // e.g. -2 -(-19) -> (-2 + 19) -> (19 - 2)
    else if (this->isNegative() && rhs.isNegative() && *this > rhs || *this == rhs){
        s = performSubtraction(rhs, *this);
    }

    return BigDecimalInt(s);
}

BigDecimalInt BigDecimalInt::operator+(const BigDecimalInt &&rhs)const{
    return (*this + rhs);
}

BigDecimalInt BigDecimalInt::operator-(const BigDecimalInt &&rhs)const{
    return (*this - rhs);
}

BigDecimalInt BigDecimalInt::operator*(const BigDecimalInt& rhs) const{
    int resLength = size() + rhs.size();
    vector<int> res(resLength, 0);
    for(int i = size() - 1; i >= 0; i--){
        int carry = 0 ;
        for(int j = rhs.size() - 1; j >= 0; j--){
            BigDecimalInt product = this->getDigit(i) * rhs.getDigit(j);
            res[i + j] += carry + product.getDigit(0);

            if(res[i + j] >= 10){
                carry = 1;
                res[i + j] -= 10;
            }else{
                carry = 0;
            }

            if(product.size() > 1)
                carry += product.getDigit(1);
        }

        if(carry){
            res[i + rhs.size()] = carry;
        }

    }
//    string temp = "";
//    for(auto ii: res){
//        temp += ii + '0';
//    }
    for(auto ii: res){
        cout << ii;
    }
    cout << endl;
//    return BigDecimalInt(temp);
}

// Arithmetic-assignment Operators
BigDecimalInt &BigDecimalInt::operator+=(const BigDecimalInt & rhs) {
    *this = *this + rhs;
    return *this;
}

BigDecimalInt &BigDecimalInt::operator-=(const BigDecimalInt &rhs) {
    *this = *this - rhs;
    return *this;
}


// Comparison Operators
bool BigDecimalInt::operator<(const BigDecimalInt &rhs) const{
    return (*this != rhs) && (rhs > *this);
}

bool BigDecimalInt::operator<=(const BigDecimalInt &rhs) const{
    return (*this == rhs) || (rhs < *this);
}

bool BigDecimalInt::operator>(const BigDecimalInt &rhs) const{
    // Signs aren't the same, *this > rhs only if *this is positive
    if (this->isNegative() != rhs.isNegative())
        return (this->isPositive());

    // Digits aren't the same, then check # of digits and minusOrPlus

    // Case 1: e.g. (-13, -123)
    bool case1 = (this->size() < rhs.size()) && this->isNegative();
    // Case 2: e.g. (555, 5)
    bool case2 = (this->size() > rhs.size()) && this->isPositive();
    if (this->size() != rhs.size())
        return (case1 || case2);

    // Signs are the same, # of digits is same
    for (int i = 0; i < rhs.size(); i++){
        if (this->digits[i] < rhs.digits[i])
            return this->isNegative();
        if (this->digits[i] > rhs.digits[i])
            return this->isPositive();
    }
    return false; // *this == rhs
}

bool BigDecimalInt::operator>=(const BigDecimalInt &rhs) const{
    return (*this == rhs) || (*this > rhs);
}

bool BigDecimalInt::operator==(const BigDecimalInt &rhs) const{
    // checking if two objects has the same minusOrPlus, then checking the digits
    if (this->Sign == rhs.Sign)
        return (this->digits == rhs.digits);
    else
        return false;
}

bool BigDecimalInt::operator!=(const BigDecimalInt &rhs) const{
    return !(*this == rhs);
}

// Conversion functions:
string BigDecimalInt::toString() const {
    return digits;
}

int BigDecimalInt::toInt() const {
    if(*this > BigDecimalInt(INT_MAX))
        throw out_of_range("This Number is out of range of int");

    return stoi(digits);
}

long long BigDecimalInt::toLongLong() const {
    if(*this > BigDecimalInt(LLONG_MAX))
        throw out_of_range("This Number is out of range of int");

    return stoll(digits);
}




// I/O stream Operators:
ostream& operator<<(ostream &out, const BigDecimalInt &num){
    // print number with '-' sign or without '+' sign
    if (num.Sign == '-')
        out << num.Sign << num.digits;
    else
        out << num.digits;
    return out;
}

istream& operator>>(istream& in, BigDecimalInt& rhs){
    string num;
    in >> num;
    rhs = BigDecimalInt(num);
    return in;
}
