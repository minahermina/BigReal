#ifndef OOP_ASSIGNMENT_2_BIGREAL_H
#define OOP_ASSIGNMENT_2_BIGREAL_H

#include "BigDecimalInt.h"

class BigReal {
private:
    BigDecimalInt integerDigits;
    BigDecimalInt fractionDigits;
    char Sign;

    // Helper Functions.
    void formatFractionsZeros(BigDecimalInt& lhs, BigDecimalInt& rhs); // make fractions the same length
    void handleCarry (BigReal& real, int size);
    void handleBorrow (BigReal& real, int size);
    BigReal subtract (BigReal lhs, BigReal rhs, int biggerSize); // Subtract two numbers, such that lhs > rhs

public:
    // Constructors
    BigReal(double = 0.0); // Default constructor
    BigReal(string );
    BigReal(BigDecimalInt );
    BigReal(BigDecimalInt, BigDecimalInt, char);

    // Assignment Operators
    BigReal& operator=(const BigReal&) = default;
    BigReal& operator=(const string&);
    BigReal& operator=(const BigDecimalInt&);
    BigReal& operator=(const int&);
    BigReal& operator=(const long long&);

    // Increment & Decrement Operators
//    BigReal& operator++();       // pre-increment
//    BigReal& operator--();       // pre-decrement
//    BigReal operator++(int)const;     // post-increment
//    BigReal operator--(int)const;     // post-decrement

    BigReal operator+(BigReal& rhs);
    BigReal operator+(BigReal&& rhs);
    BigReal operator-(BigReal &rhs);
    BigReal operator-(BigReal&& rhs);
    bool operator<(const BigReal& rhs);
    bool operator>(const BigReal& anotherReal);
    bool operator==(const BigReal& anotherReal);
    friend ostream &operator<<(ostream &out, BigReal& num);
    friend ostream &operator<<(ostream &out, BigReal&& num); // handling rvalue reference
    friend istream &operator>>(istream &out, BigReal& num);

    // Getters
    int size() const;
    char sign() const;
};


#endif //OOP_ASSIGNMENT_2_BIGREAL_H
