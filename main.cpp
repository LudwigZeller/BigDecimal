#include <iostream>
#include "BigDecimal/BigDecimal.h"

using namespace std;

int main() {
//    BigDecimal a(100);
//    a += 3;
//    a = BigDecimal::invert(a);
//    cout << a.isZero();

    BigDecimal a(100);
    BigDecimal b(101,100);
    b.aoz[100] = 1;
    for (int i = 0; i < 100; ++i) {
        a.aoz[i] = 9;
    }
    b.print();
    a.print();
    (b - a).print();

//
//    BigDecimal x(100);
//    x += 28;
//    BigDecimal::invert(x).print();
//    x.print();
//
//
//
//    BigDecimal y(100);
//    y += 28;
//    y.resize(true, 110);
//    y = BigDecimal::invert(y);
//    y.print();

    /*cout << endl<<"Problems with newtons method:" << endl << "1/2 =" << endl;

    BigDecimal b(1000);
    b += 2;
    BigDecimal::invert(b).print();

    cout << endl << "sqrt(2) =" << endl;

    BigDecimal a(1000);
    a += 2;
    BigDecimal::sqrt(a).print();

    cout << endl << "Finished";*/

    return 0;
}

