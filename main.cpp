#include <iostream>
#include "BigDecimal/BigDecimal.h"

using namespace std;

int main() {
    BigDecimal y(1000);
    y += 69;
    y.print();
    BigDecimal z(104);
    z += 420;
    z.print();
    // TODO: Stackoverflow
    BigDecimal x = y / z;
    x.print();

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
//
//    cout << endl << "Finished";
//
//    return 0;
}

