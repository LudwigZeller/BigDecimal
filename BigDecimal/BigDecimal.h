//
// Created by Ludwig Zeller on 09.11.2021.
//

// TODO: Clean up and rework
// TODO: THE NEXT BIG STEP: DYNAMIC ARRAYS (000123.123000 -> 123.321)
// TODO: MULTITHREADING
// TODO: SPEED UP VIA FOURIER TRANSFORMATION

#ifndef BIGDECIMAL_BIGDECIMAL_H
#define BIGDECIMAL_BIGDECIMAL_H
#include <iostream>

class BigDecimal {
private:
    bool sign;
    short *aoz;
    short *auz;
    int poz;
    int puz;

    void relocate();


public:

    BigDecimal(int precision_over_zero, int precision_under_zero);

    BigDecimal(int precision_over_zero, int precision_under_zero, int dot_index, int sign, int array_size,
               const short init_array[]);
    BigDecimal(const BigDecimal&);
    BigDecimal& operator=(const BigDecimal&);

    // Utility
    static BigDecimal invert(const BigDecimal&, int converging_limit = 100);
    static BigDecimal sqrt(const BigDecimal&, int converging_limit = 100);
    void resize(bool under_zero, int precision);
    void print() const;


    // Calculation Functions
    // Add
    BigDecimal add(short addend) const;
    BigDecimal add(const BigDecimal& addend) const;
    BigDecimal operator+(short addend) const;
    BigDecimal operator+(const BigDecimal& addend) const;
    void operator+=(short addend);
    void operator+=(const BigDecimal& addend);

    // Subtract
    BigDecimal subtract(short subtrahend) const;
    BigDecimal subtract(const BigDecimal& subtrahend) const;
    BigDecimal operator-(short) const;
    BigDecimal operator-(const BigDecimal&) const;
    void operator-=(short);
    void operator-=(const BigDecimal&);

    // Multiply
    BigDecimal multiply(short factor) const;
    BigDecimal multiply(const BigDecimal& factor) const;
    BigDecimal operator*(short) const;
    BigDecimal operator*(const BigDecimal&) const;
    void operator*=(short);
    void operator*=(const BigDecimal&);

    // Divide
    BigDecimal divide(short divisor) const;
    BigDecimal divide(const BigDecimal& divisor) const;
    BigDecimal operator/(short) const;
    BigDecimal operator/(const BigDecimal&) const;
    void operator/=(short);
    void operator/=(const BigDecimal&);

    // Compare Functions
    // Greater Than
    bool greaterThan(const BigDecimal& compare) const;
    bool operator>(const BigDecimal&) const;

    // Smaller Than
    bool smallerThan(const BigDecimal& compare) const;
    bool operator<(const BigDecimal&) const;

    // Equals
    bool equals(const BigDecimal& compare) const;
    bool operator==(const BigDecimal&) const;
    bool isZero() const;

    // Unequal
    bool unequals(const BigDecimal& compare) const;
    bool operator!=(const BigDecimal& compare) const;
};


#endif //BIGDECIMAL_BIGDECIMAL_H
