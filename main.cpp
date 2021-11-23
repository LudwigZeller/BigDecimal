#include <iostream>
#include "BigDecimal/BigDecimal.h"

using namespace std;

int main() {
    BigDecimal x(100);
    x += 28;
    BigDecimal::invert(x).print();
    x.print();

    BigDecimal y(100);
    y += 28;
    y.resize(true, 110);
    y = BigDecimal::invert(y);
    y.print();

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

