//
// Created by Ludwig Zeller on 09.11.2021.
//

#include "BigDecimal.h"

BigDecimal::BigDecimal(int precision) {
    sign = true;
    poz = 100;
    aoz = new short[poz];
    memset(aoz, 0, poz * sizeof(*aoz));
    puz = precision;
    auz = new short[puz];
    memset(auz, 0, puz * sizeof(*auz));
}

BigDecimal::BigDecimal(int precision_over_zero, int precision_under_zero) {
    sign = true;
    poz = precision_over_zero;
    aoz = new short[precision_over_zero];
    memset(aoz, 0, sizeof(*aoz) * poz);
    puz = precision_under_zero;
    auz = new short[precision_under_zero];
    memset(auz, 0, sizeof(*auz) * puz);
}

BigDecimal::BigDecimal(const BigDecimal &to_copy) {
    sign = to_copy.sign;
    poz = to_copy.poz;
    puz = to_copy.puz;
    aoz = new short[poz];
    auz = new short[puz];
    memcpy(aoz, to_copy.aoz, sizeof(*aoz) * poz);
    memcpy(auz, to_copy.auz, sizeof(*auz) * puz);
    range = to_copy.range;
}

BigDecimal &BigDecimal::operator=(const BigDecimal &to_copy) {
    if (this == &to_copy) {
        return *this;
    }
    sign = to_copy.sign;
    poz = to_copy.poz;
    puz = to_copy.puz;
    aoz = new short[poz];
    auz = new short[puz];
    memcpy(aoz, to_copy.aoz, sizeof(*to_copy.aoz) * to_copy.poz);
    memcpy(auz, to_copy.auz, sizeof(*to_copy.auz) * to_copy.puz);
    range = to_copy.range;
    return *this;
}

// Utility Functions
// Relocate
void BigDecimal::relocate() {
    short transfer = 0;
    //Relocate under Zero
    for (int i = puz - 1; i >= 0; --i) {
        auz[i] = short(auz[i] + transfer);
        transfer = 0;
        if (auz[i] > 9) {
            transfer = short(auz[i] / 10);
            auz[i] = short(auz[i] - transfer * 10);
        }
        if (auz[i] < 0) {
            transfer = short((auz[i] / 10) - 1);
            auz[i] = short(auz[i] + 10 * -transfer);
        }
    }
    //Relocate over Zero
    for (int i = 0; i < poz; ++i) {
        aoz[i] = short(aoz[i] + transfer);
        transfer = 0;
        if (aoz[i] > 9) {
            transfer = short(aoz[i] / 10);
            aoz[i] = short(aoz[i] - transfer * 10);
        }
        if (aoz[i] < 0) {
            transfer = short((aoz[i] / 11) - 1);
            aoz[i] = short(aoz[i] + 10 * -transfer);
        }
    }
    // Handle transfer problems
    if (transfer) {
        if (transfer > 0) {
            // TODO: Test if this works
            // If number outgrew the array raise the precision over zero
            memcpy(aoz, aoz, sizeof(*aoz) * (poz + 100));
            memset(aoz + poz, 0, sizeof(*aoz) * 100);
            aoz[poz] = transfer;
            poz += 100;
            relocate();
//            auto *temp = new short[poz];
//            *temp = *aoz;
//            aoz = new short[poz + 100];
//            *aoz = *temp;
//            for (int i = 0; i < 100; ++i) {
//                aoz[poz + i] = 0;
//            }
//            aoz[poz] = transfer;
        } else {
            // If number went under Zero flip it around and change sign
            // TODO: Maybe look if this can be simplified
            BigDecimal subtract_value(poz + 1, puz);
            subtract_value.aoz[poz] = 1;
            bool old_sign = sign;
            sign = true;
            subtract_value -= *this;
            sign = old_sign;
            subtract_value.resize(false, poz);
            *this = subtract_value;
            sign = !old_sign;
        }
    }

    // Update Range
    range.oz = 0;
    range.uz = 0;
    for (int i = poz  - 1; i >= 0; --i) {
        if(aoz[i]){
            range.oz = i;
            break;
        }
    }
    for (int i = puz  - 1; i >= 0; --i) {
        if(auz[i]){
            range.uz = i;
            break;
        }
    }
}

// Invert
BigDecimal BigDecimal::invert(const BigDecimal &to_invert, int converging_limit) {
    BigDecimal one(1, 1);
    one += 1;
    if (to_invert == one) {
        return to_invert;
    }

    BigDecimal to_invert_copy = to_invert; // To change to a positive sign
    to_invert_copy.sign = true;
    int highest_index = to_invert.poz; // Unreachable value
    for (int i = to_invert.poz - 1; i >= 0; --i) {
        if (to_invert.aoz[i]) {
            highest_index = i;
            break;
        }
    }

    if (highest_index == to_invert.poz) {
        for (int i = 0; i < to_invert.puz; ++i) {
            if (to_invert.auz[i]) {
                highest_index = -1 - i;
                break;
            }
        }
    }
    if (highest_index == to_invert.poz) {
        std::cout << "Tried inverse of zero" << std::endl;
        return to_invert;
    }

    int safety_poz = (abs(highest_index) > to_invert.poz ? abs(highest_index) : to_invert.poz) + 10 +
                     int((abs(highest_index) > to_invert.poz ? abs(highest_index) : to_invert.poz) * 0.1);
    int safety_puz = (abs(highest_index) > to_invert.puz ? abs(highest_index) : to_invert.puz) + 10 +
                     int((abs(highest_index) > to_invert.puz ? abs(highest_index) : to_invert.puz) * 0.1);

    BigDecimal approx(safety_poz, safety_puz);

    if (highest_index >= 0) {
        approx.auz[highest_index] = 1;
    } else {
        approx.aoz[abs(highest_index) - 1] = 1;
    }

    BigDecimal check1(1, 1);
    check1 += 1;
    BigDecimal check2(1, 1);

    // Newton's Approximation
    // 1/y = x(n+1) = 2*x(n) - x(n)^2*y
    // x(0) = Approximately the inverse

    for (int i = 0; i < converging_limit; ++i) {
        if (check1 == check2)
            break;
        check1 = approx;
        approx = (approx * 2) - ((approx * approx) * to_invert_copy);
        check2 = approx;
        check1.resize(true, to_invert.puz);
        check2.resize(true, to_invert.puz);
    }
    if (check1 != check2 && converging_limit == 100)
        std::cout << "Inverse hasn't converged" << std::endl;
//    while (check1 != check2) {
//        check1 = approx;
//        approx = (approx * 2) - ((approx * approx) * to_invert_copy);
//        check2 = approx;
//        check1.resize(true, to_invert.puz);
//        check2.resize(true, to_invert.puz);
//    }

    approx.resize(true, to_invert.puz);
    approx.resize(false, to_invert.poz);
    approx.sign = to_invert.sign;
    return approx;
}

// Square root
BigDecimal BigDecimal::sqrt(const BigDecimal &base, int converging_limit) {
    BigDecimal base_copy = base; // To change to a positive sign
    base_copy.sign = true;
    int highest_index = base.poz; // Unreachable value

    for (int i = base.poz - 1; i >= 0; --i) {
        if (base.aoz[i]) {
            highest_index = i;
            break;
        }
    }

    if (highest_index == base.poz) {
        for (int i = 0; i < base.puz; ++i) {
            if (base.auz[i]) {
                highest_index = -1 - i;
                break;
            }
        }
    }


    BigDecimal approx(base.poz,
                      (abs(highest_index) + 10) > (base.puz + 10) ? (abs(highest_index) + 10) : (base.puz + 10));

    if (highest_index >= 0) {
        approx.aoz[int(highest_index / 2)] = 1;
    } else {
        approx.auz[int((abs(highest_index) - 1) / 2)] = 1;
    }

    BigDecimal check1(1, 1);
    check1 += 1;
    BigDecimal check2(1, 1);

    BigDecimal one_half(1, 1);
    one_half.auz[0] = 5;

    // Newton's Approximation
    // sqrt(y) = x(n+1) = (x(n) + y / x(n)) / 2
    // x(0) = Approximately the square root

    for (int i = 0; i < converging_limit; ++i) {
        if (check1 == check2)
            break;
        check1 = approx;
        approx = ((base_copy * invert(approx, 15)) + approx) * one_half;
        check2 = approx;
        check1.resize(true, base.puz);
        check2.resize(true, base.puz);
    }

    if (check1 != check2 && converging_limit == 100) {
        std::cout << "Square Root hasn't converged" << std::endl;
    }
//    while (check1 != check2) {
//        check1 = approx;
//        approx = ((base_copy * invert(approx, 15)) + approx) * one_half;
//        check2 = approx;
//        check1.resize(true, base.puz);
//        check2.resize(true, base.puz);
//    }

    approx.resize(true, base.puz);
    approx.resize(false, base.poz);
    approx.sign = base.sign;
    return approx;
}

//Resize
void BigDecimal::resize(bool under_zero, int precision) {
    if (under_zero) {
        memcpy(auz, auz, sizeof(*auz) * precision);
        if (precision > puz) {
            memset((auz + puz), 0, sizeof(*auz) * (precision - puz));
        }
        puz = precision;
    } else {
        memcpy(aoz, aoz, sizeof(*aoz) * precision);
        if (precision > poz) {
            memset((aoz + poz), 0, sizeof(*aoz) * (precision - poz));
        }
        poz = precision;
    }
}

// Print
void BigDecimal::print() const {
    std::cout << (sign ? "+" : "-");

    for (int i = range.oz; i >= 0; i--) {
        std::cout << aoz[i];
    }
    std::cout << ".";
    for (int i = 0; i <= range.uz; i++) {
        std::cout << auz[i];
    }
    std::cout << std::endl;
}

// Calculation Functions
// Add
BigDecimal BigDecimal::add(short addend) const {
    if (!sign) {
        return subtract(addend);
    }
    BigDecimal return_value = *this;
    return_value.aoz[0] = short(return_value.aoz[0] + addend);
    return_value.relocate();
    if (return_value.isZero())
        return_value.sign = true;
    return return_value;
}

BigDecimal BigDecimal::add(const BigDecimal &addend) const {
    // Sign related cases
    if (!addend.sign && sign) {
        BigDecimal temp = addend;
        temp.sign = true;
        return subtract(temp);
    }
    if (!sign && addend.sign) {
        BigDecimal temp = *this;
        temp.sign = true;
        temp = temp.subtract(addend);
        temp.sign = !temp.sign;
        return temp;
    }

    BigDecimal return_val((poz > addend.poz) ? poz : addend.poz,
                          (puz > addend.puz) ? puz : addend.puz);
    for (int i = 0; i < (puz > addend.puz ? addend.puz : puz); ++i) {
        return_val.auz[i] = short(auz[i] + addend.auz[i]);
    }
    for (int i = 0; i < (poz > addend.poz ? addend.poz : poz); ++i) {
        return_val.aoz[i] = short(aoz[i] + addend.aoz[i]);
    }

    if (poz != addend.poz) {
        if (poz > addend.poz) {
            for (int i = addend.poz; i < poz; ++i) {
                return_val.aoz[i] = aoz[i];
            }
        } else {
            for (int i = poz; i < addend.poz; ++i) {
                return_val.aoz[i] = addend.aoz[i];
            }
        }
    }
    if (puz != addend.puz) {
        if (puz > addend.puz) {
            for (int i = addend.puz; i < puz; ++i) {
                return_val.auz[i] = auz[i];
            }
        } else {
            for (int i = puz; i < addend.puz; ++i) {
                return_val.auz[i] = addend.auz[i];
            }
        }
    }

    return_val.relocate();
    return return_val;
}

BigDecimal BigDecimal::operator+(short addend) const {
    return add(addend);
}

BigDecimal BigDecimal::operator+(const BigDecimal &addend) const {
    return add(addend);
}

void BigDecimal::operator+=(short addend) {
    *this = add(addend);
}

void BigDecimal::operator+=(const BigDecimal &addend) {
    *this = add(addend);
}

// Subtract
BigDecimal BigDecimal::subtract(short subtrahend) const {
    BigDecimal return_value = *this;
    return_value.aoz[0] = short(return_value.aoz[0] - subtrahend);
    return_value.relocate();
    if (return_value.isZero())
        return_value.sign = true;
    return return_value;
}

BigDecimal BigDecimal::subtract(const BigDecimal &subtrahend) const {
    // Sign related cases
    if (!sign && subtrahend.sign) {
        BigDecimal temp = *this;
        temp.sign = true;
        temp += subtrahend;
        temp.sign = false;
        return temp;
    }
    if (!subtrahend.sign && sign) {
        BigDecimal temp = subtrahend;
        temp.sign = true;
        return add(temp);
    }

    BigDecimal return_val((poz > subtrahend.poz ? poz : subtrahend.poz),
                          (puz > subtrahend.puz ? puz : subtrahend.puz));
    for (int i = 0; i < (poz < subtrahend.poz ? poz : subtrahend.poz); ++i) {
        return_val.aoz[i] = short(aoz[i] - subtrahend.aoz[i]);
    }
    for (int i = 0; i < (puz < subtrahend.puz ? puz : subtrahend.puz); ++i) {
        return_val.auz[i] = short(auz[i] - subtrahend.auz[i]);
    }

    if (poz != subtrahend.poz) {
        if (poz > subtrahend.poz) {
            for (int i = subtrahend.poz; i < poz; ++i) {
                return_val.aoz[i] = short(aoz[i]);
            }
        } else {
            for (int i = poz; i < subtrahend.poz; ++i) {
                return_val.aoz[i] = short(-subtrahend.aoz[i]);
            }
        }
    }

    if (puz != subtrahend.puz) {
        if (puz > subtrahend.puz) {
            for (int i = subtrahend.puz; i < puz; ++i) {
                return_val.auz[i] = short(-auz[i]);
            }
        } else {
            for (int i = puz; i < subtrahend.puz; ++i) {
                return_val.auz[i] = short(-subtrahend.auz[i]);
            }
        }
    }

    return_val.relocate();
    if (return_val.isZero())
        return_val.sign = true;
    return return_val;
}

BigDecimal BigDecimal::operator-(short subtrahend) const {
    return subtract(subtrahend);
}

BigDecimal BigDecimal::operator-(const BigDecimal &subtrahend) const {
    return subtract(subtrahend);
}

void BigDecimal::operator-=(short subtrahend) {
    *this = subtract(subtrahend);
}

void BigDecimal::operator-=(const BigDecimal &subtrahend) {
    *this = subtract(subtrahend);
}

// Multiply
BigDecimal BigDecimal::multiply(short factor) const {
    if (factor == 0)
        return {poz, puz};

    BigDecimal return_value = *this;
    if (factor < 0) {
        return_value.sign = !return_value.sign;
        if (factor == -1) {
            return return_value;
        }
        factor = short(-factor);
    } else if (factor == 1) {
        return return_value;
    }
    for (int i = 0; i < puz; ++i) {
        return_value.auz[i] = short(return_value.auz[i] * factor);
    }
    for (int i = 0; i < poz; ++i) {
        return_value.aoz[i] = short(return_value.aoz[i] * factor);
    }
    return_value.relocate();
    return return_value;
}

BigDecimal BigDecimal::multiply(const BigDecimal &factor) const {
    if (factor.isZero() || isZero())
        return {100, 100};


    int here_highest_index = 0;
    int here_lowest_index = 0;
    for (int i = poz - 1; i >= 0; --i) {
        if (aoz[i]) {
            here_highest_index = i;
            break;
        }
    }
    for (int i = puz - 1; i >= 0; --i) {
        if (auz[i]) {
            here_lowest_index = i;
            break;
        }
    }

    int there_highest_index = 0;
    int there_lowest_index = 0;
    for (int i = factor.poz - 1; i >= 0; --i) {
        if (factor.aoz[i]) {
            there_highest_index = i + 3;
            break;
        }
    }
    for (int i = factor.puz - 1; i >= 0; --i) {
        if (factor.auz[i]) {
            there_lowest_index = i + 3;
            break;
        }
    }

    int min_poz = 5 + 2 * (here_highest_index > there_highest_index ? here_highest_index : there_highest_index);
    int return_poz = min_poz > (poz > factor.poz ? poz : factor.poz) ? min_poz : (poz > factor.poz ? poz : factor.poz);
    int min_puz = 5 + 2 * (here_lowest_index > there_lowest_index ? here_lowest_index : there_lowest_index);
    int return_puz = min_puz > (puz > factor.puz ? puz : factor.puz) ? min_puz : (puz > factor.puz ? puz : factor.puz);
    BigDecimal return_value(return_poz, return_puz);

    if (!sign && factor.sign || !factor.sign && sign) {
        return_value.sign = false;
    }

    short temp;
    // Multiplication over Zero
    for (int i = 0; i < poz; ++i) {
        for (int ii = 0; ii < factor.poz; ++ii) {
            temp = short(aoz[i] * factor.aoz[ii]);
            if (i + ii <= return_value.poz)
                return_value.aoz[i + ii] = short(return_value.aoz[i + ii] + temp);
        }
    }

    // Multiplication under Zero
    for (int i = 0; i < puz; ++i) {
        for (int ii = 0; ii < factor.puz; ++ii) {
            temp = short(auz[i] * factor.auz[ii]);
            if (i + ii + 2 <= return_value.puz)
                return_value.auz[i + ii + 1] = short(return_value.auz[i + ii + 1] + temp);
        }
    }

    // Multiplication over with under Zero
    for (int i = 0; i < poz; ++i) {
        for (int ii = 0; ii < factor.puz; ++ii) {
            temp = short(aoz[i] * factor.auz[ii]);
            if (i - (ii + 1) >= 0) {
                if (return_value.poz >= i - (ii + 1))
                    return_value.aoz[i - (ii + 1)] = short(return_value.aoz[i - (ii + 1)] + temp);
            } else {
                if (return_value.puz >= -(i - (ii + 1)))
                    return_value.auz[-(i - (ii + 1)) - 1] = short(return_value.auz[-(i - (ii + 1)) - 1] + temp);
            }
        }
    }


    // Multiplication under with over Zero
    for (int i = 0; i < puz; ++i) {
        for (int ii = 0; ii < factor.poz; ++ii) {
            temp = short(auz[i] * factor.aoz[ii]);
            if (ii - (i + 1) >= 0) {
                if (return_value.poz >= i - (ii + 1))
                    return_value.aoz[ii - (i + 1)] = short(return_value.aoz[ii - (i + 1)] + temp);
            } else {
                if (return_value.puz >= -(i - (ii + 1)))
                    return_value.auz[-(ii - (i + 1)) - 1] = short(return_value.auz[-(ii - (i + 1)) - 1] + temp);
            }
        }
    }

    return_value.relocate();
    return_value.resize(false, (poz > factor.poz ? poz : factor.poz));
    return_value.resize(true, (puz > factor.puz ? puz : factor.puz));
    return return_value;
}

BigDecimal BigDecimal::operator*(short factor) const {
    return multiply(factor);
}

BigDecimal BigDecimal::operator*(const BigDecimal &factor) const {
    return multiply(factor);
}

void BigDecimal::operator*=(short factor) {
    *this = multiply(factor);
}

void BigDecimal::operator*=(const BigDecimal &factor) {
    *this = multiply(factor);
}

// Divide
BigDecimal BigDecimal::divide(short divisor) const {
    BigDecimal _divisor(5, 5);
    _divisor += divisor;
    return multiply(invert(_divisor));
}

BigDecimal BigDecimal::divide(const BigDecimal &divisor) const {
    BigDecimal divisor_copy = divisor;
    divisor_copy.resize(false, divisor.poz + 10);
    divisor_copy.resize(true, divisor.puz + 10);
    BigDecimal return_value = multiply(invert(divisor_copy));
    return_value.resize(false, (poz > divisor.poz ? poz : divisor.poz));
    return_value.resize(true, (puz > divisor.puz ? puz : divisor.puz));
    return return_value;
}

BigDecimal BigDecimal::operator/(short divisor) const {
    return divide(divisor);
}

BigDecimal BigDecimal::operator/(const BigDecimal &divisor) const {
    return divide(divisor);
}

void BigDecimal::operator/=(short divisor) {
    *this = divide(divisor);
}

void BigDecimal::operator/=(const BigDecimal &divisor) {
    *this = divide(divisor);
}

// Compare Functions
// Greater Than
bool BigDecimal::greaterThan(const BigDecimal &compare) const {
    // Returns True if this BigDecimal is bigger
    // In all other cases it returns false




    int here_index = poz; // Value that cannot be reached
    int compare_index = compare.poz;

    if (sign != compare.sign) {
        if (sign)
            return false;
        else
            return true;
    }

    for (int i = poz - 1; i >= 0; --i) {
        if (aoz[i]) {
            here_index = i;
            break;
        }
    }
    if (here_index == poz) {
        for (int i = 0; i < puz; ++i) {
            if (auz[i]) {
                here_index = -1 - i;
                break;
            }
        }
    }

    for (int i = poz - 1; i >= 0; --i) {
        if (aoz[i] != 0) {
            compare_index = i;
        }
    }
    if (compare_index == compare.poz) {
        for (int i = 0; i < compare.puz; ++i) {
            if (compare.auz[i]) {
                here_index = -1 - i;
                break;
            }
        }
    }

    if (here_index == compare_index) {
        // I don't know why, but if I try to insert the statement into the for loop it doesn't work anymore
        int undervalue = -1 - (compare.puz < puz ? compare.puz : puz);
        for (int i = here_index; i > undervalue; --i) {
            if (i > 0) {
                if (aoz[i] != compare.aoz[i]) {
                    return aoz[i] > compare.aoz[i];
                }
            } else {
                int j = abs(i) - 1;
                if (auz[j] != compare.auz[j]) {
                    return auz[j] > compare.auz[j];
                }
            }
        }
        if (puz > compare.puz) {
            for (int i = compare.puz; i < puz; ++i) {
                if (auz[i]) {
                    return true;
                }
            }
        } else if (compare.puz > puz) {
            for (int i = puz; i < compare.puz; ++i) {
                if (compare.auz[i]) {
                    return false;
                }
            }
        }
    }

    return here_index > compare_index;
}

bool BigDecimal::operator>(const BigDecimal &compare) const {
    return greaterThan(compare);
}

// Smaller Than
bool BigDecimal::smallerThan(const BigDecimal &compare) const {
    // Returns True if this BigDecimal is smaller
    // In all other cases it returns false
    int here_index = poz; // Value that cannot be reached
    int compare_index = compare.poz;

    if (sign != compare.sign) {
        if (sign)
            return true;
        else
            return false;
    }

    for (int i = poz - 1; i >= 0; --i) {
        if (aoz[i]) {
            here_index = i;
            break;
        }
    }
    if (here_index == poz) {
        for (int i = 0; i < puz; ++i) {
            if (auz[i]) {
                here_index = -1 - i;
                break;
            }
        }
    }

    for (int i = poz - 1; i >= 0; --i) {
        if (aoz[i] != 0) {
            compare_index = i;
        }
    }
    if (compare_index == compare.poz) {
        for (int i = 0; i < compare.puz; ++i) {
            if (compare.auz[i]) {
                here_index = -1 - i;
                break;
            }
        }
    }

    if (here_index == compare_index) {
        int loop_limit = -1 - (compare.puz < puz ? compare.puz : puz);
        for (int i = here_index; i > loop_limit; ++i) {
            if (i > 0) {
                if (aoz[i] != compare.aoz[i]) {
                    return aoz[i] < compare.aoz[i];
                }
            } else {
                int j = abs(i) - 1;
                if (auz[j] != compare.auz[j]) {
                    return auz[j] < compare.auz[j];
                }
            }
        }
        if (puz > compare.puz) {
            for (int i = compare.puz; i < puz; ++i) {
                if (auz[i]) {
                    return false;
                }
            }
        } else if (compare.puz > puz) {
            for (int i = puz; i < compare.puz; ++i) {
                if (compare.auz[i]) {
                    return true;
                }
            }
        }
    }

    return here_index < compare_index;
}

bool BigDecimal::operator<(const BigDecimal &compare) const {
    return smallerThan(compare);
}

// Equals
bool BigDecimal::equals(const BigDecimal &compare) const {
    // Returns True if one BigDecimal is the same number as the on to compare to
    // In all other cases it returns false

    if (sign != compare.sign)
        return false;

    for (int i = 0; i < (poz < compare.poz ? poz : compare.poz); ++i) {
        if (aoz[i] != compare.aoz[i])
            return false;
    }
    for (int i = 0; i < (puz < compare.puz ? puz : compare.puz); ++i) {
        if (auz[i] != compare.auz[i])
            return false;
    }

    if (poz > compare.poz) {
        for (unsigned int i = compare.poz; i < poz; ++i) {
            if (aoz[i])
                return false;
        }
    } else if (poz < compare.poz) {
        for (unsigned int i = poz; i < compare.poz; ++i) {
            if (compare.aoz[i])
                return false;
        }
    }

    if (puz > compare.puz) {
        for (unsigned int i = compare.puz; i < puz; ++i) {
            if (auz[i])
                return false;
        }
    } else if (puz < compare.puz) {
        for (unsigned int i = puz; i < compare.puz; ++i) {
            if (compare.auz[i])
                return false;
        }
    }

    return true;
}

bool BigDecimal::operator==(const BigDecimal &compare) const {
    return equals(compare);
}

bool BigDecimal::isZero() const {
    if (std::any_of(aoz, aoz + poz, [](int i) { return i > 0; }))
        return false;
    if (std::any_of(aoz, aoz + poz, [](int i) { return i > 0; }))
        return false;
    return true;
//    for (int i = 0; i < puz; ++i) {
//        if (auz[i])
//            return false;
//    }
//    for (int i = 0; i < poz; ++i) {
//        if (aoz[i])
//            return false;
//    }
//    return true;
}

bool BigDecimal::unequals(const BigDecimal &compare) const {
    return !equals(compare);
}

bool BigDecimal::operator!=(const BigDecimal &compare) const {
    return unequals(compare);
}

