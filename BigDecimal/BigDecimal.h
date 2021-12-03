//
// Created by Ludwig Zeller on 09.11.2021.
//

// KISS - Keep it simple, stupid!
// Also: If it was difficult to write it should be difficult to read!
// On a serious node I am sorry for anyone trying to read my code

// TODO: The bigger the number, the higher the chance of not converging and the higher the runtime
// TODO: Think about solving and receiving a periodic number in division BSP: 60 / 3
// TODO: Rework Equals functions with any_of, none_of, memcmp
// TODO: All relocate calls should first be evaluated if they are necessary
// TODO: More comparison compatibilities with int (also look if comparison with zero calls isZero())
// TODO: MULTITHREADING
// TODO: SPEED UP VIA FOURIER TRANSFORMATION

#ifndef BIGDECIMAL_BIGDECIMAL_H
#define BIGDECIMAL_BIGDECIMAL_H

#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm> // Used for fast compare methods
#include <stdexcept>
#include <string>

class BigDecimal {
private:
/* To be private */
    bool sign;
    short *aoz;
    short *auz;
    int poz;
    int puz;
    struct RANGE {
        // Saves the highest index and lowest index
        // Why did I use a struct? Because I wanted to!
        // It doesn't have much point here, but I have never actually used one, so I wanted to try it
        int oz = 0;
        int uz = 0;
    } range;

    BigDecimal(int precision_over_zero, int precision_under_zero);
    void relocate();
    void update_range();
    void debug_print() const;
    /*               */

public:

    // Constructor (Copy, Assignment, etc.)
    void resize(bool under_zero, int precision);
    explicit BigDecimal(int precision);
    BigDecimal(const BigDecimal &);
    BigDecimal &operator=(const BigDecimal &);


    // Utility
    static BigDecimal invert(const BigDecimal &, int converging_limit = 100);
    static BigDecimal sqrt(const BigDecimal &, int converging_limit = 100);
    void print() const;
    std::string toString() const;


    // Calculation Functions
    // Add
    BigDecimal add(int addend) const;
    BigDecimal add(const BigDecimal &addend) const;
    BigDecimal operator+(int addend) const;
    BigDecimal operator+(const BigDecimal &addend) const;
    void operator+=(int addend);
    void operator+=(const BigDecimal &addend);

    // Subtract
    BigDecimal subtract(int subtrahend) const;
    BigDecimal subtract(const BigDecimal &subtrahend) const;
    BigDecimal operator-(int) const;
    BigDecimal operator-(const BigDecimal &) const;
    void operator-=(int);
    void operator-=(const BigDecimal &);

    // Multiply
    BigDecimal multiply(int factor) const;
    BigDecimal multiply(const BigDecimal &factor) const;
    BigDecimal operator*(int) const;
    BigDecimal operator*(const BigDecimal &) const;
    void operator*=(int);
    void operator*=(const BigDecimal &);

    // Divide
    BigDecimal divide(int divisor) const;
    BigDecimal divide(const BigDecimal &divisor) const;
    BigDecimal operator/(int) const;
    BigDecimal operator/(const BigDecimal &) const;
    void operator/=(int);
    void operator/=(const BigDecimal &);

    // Compare Functions
    // Greater Than
    bool greaterThan(const BigDecimal &compare) const;
    bool operator>(const BigDecimal &) const;

    // Smaller Than
    bool smallerThan(const BigDecimal &compare) const;
    bool operator<(const BigDecimal &) const;

    // Equals
    bool equals(const BigDecimal &compare) const;
    bool operator==(const BigDecimal &) const;
    bool isZero() const;

    // Unequal
    bool unequals(const BigDecimal &compare) const;
    bool operator!=(const BigDecimal &compare) const;
};


#endif //BIGDECIMAL_BIGDECIMAL_H
