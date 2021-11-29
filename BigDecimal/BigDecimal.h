//
// Created by Ludwig Zeller on 09.11.2021.
//

// KISS - Keep it simple, stupid!
// Also: If it was difficult to write it should be difficult to read!
// On a serious node I am sorry for anyone trying to read my code

// TODO: Clean up and rework
// TODO: Rework multiply function
// TODO: Rework Equals functions with any_of, none_of, memcmp
// TODO: All relocate calls should first be evaluated if they are necessary
// TODO: THE NEXT BIG STEP: DYNAMIC ARRAYS (000123.123000 -> 123.321)
// TODO: More comparison compatibilities with int (also look if comparison with zero calls isZero())
// TODO: All calculations taking shorts as input should take ints
// TODO: MULTITHREADING
// TODO: SPEED UP VIA FOURIER TRANSFORMATION

#ifndef BIGDECIMAL_BIGDECIMAL_H
#define BIGDECIMAL_BIGDECIMAL_H

#include <iostream>
#include <algorithm> // Used for fast compare methods
#include <stdexcept>

class BigDecimal {
private:


public:
    /* To be private */
    bool sign;
    short *aoz;
    short *auz;
    int poz;
    int puz;
    struct RANGE {
        // Saves the highest index and lowest index
        int oz = 0;
        int uz = 0;
    } range;

    BigDecimal(int precision_over_zero, int precision_under_zero);
    void relocate();
    void update_range();
    /*               */
    // Constructor (Copy, Assignment, etc.)
    void resize(bool under_zero, int precision);
    explicit BigDecimal(int precision);
    BigDecimal(const BigDecimal &);
    ~BigDecimal();
    BigDecimal &operator=(const BigDecimal &);


    // Utility
    static BigDecimal invert(const BigDecimal &, int converging_limit = 100);
    static BigDecimal sqrt(const BigDecimal &, int converging_limit = 100);
    void print() const;


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
