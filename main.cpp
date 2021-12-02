#include <iostream>
#include "BigDecimal/BigDecimal.h"

using namespace std;

int main() {
    cout << endl<<"Problems with newtons method:" << endl << "1/2 =" << endl;

    BigDecimal b(1000);
    b += 2;
    BigDecimal::invert(b).print();

    cout << endl << "sqrt(2) =" << endl;

    BigDecimal a(500);
    a += 2;
    BigDecimal::sqrt(a).print();

    cout << endl << "The Problem still is that the bigger the number the bigger the runtime and the bigger the precision loss!" << endl;

    cout << endl << "Finished";
    return 0;
}

