#include <iostream>
#include "BigDecimal/BigDecimal.h"

using namespace std;

int main() {
    BigDecimal y(1000);
    y += 69;
    y.print();
    BigDecimal z(1000);
    z += 420;
    z /= 3;
    z.print();
    // TODO: Division looses precision
    // TODO: Why does this produce a stackoverflow with GCC (Somehow caused by debugging with breakpoints)

    BigDecimal x = z * y;
    x.print();
    //BigDecimal x = y / z;


//    cout << endl<<"Problems with newtons method:" << endl << "1/2 =" << endl;
//
//    BigDecimal b(1000);
//    b += 2;
//    BigDecimal::invert(b).print();
//
//    cout << endl << "sqrt(2) =" << endl;
//
//    BigDecimal a(1000);
//    a += 2;
//    BigDecimal::sqrt(a).print();

    cout << endl << "Finished";

    return 0;
}

