#include <iostream>
#include "BigDecimal/BigDecimal.h"

using namespace std;

int main() {

    cout << endl;

    short arr[] = {1, 1, 1, 2, 3, 3, 6, 1, 1, 5, 3, 2, 1, 2, 3};
    short arr2[] = {3, 5, 5, 1, 2, 8, 7, 4, 3, 5, 6, 3, 5, 6, 9};

    BigDecimal num(3, 12, 3, false, (sizeof arr / sizeof arr[0]), arr);
    BigDecimal num2(2, 13, 2, true, (sizeof arr2 / sizeof arr2[0]), arr2);
    num.print();
    cout << "/" << endl;
    num2.print();
    cout << "=" << endl;
    (num / num2).print();

    cout << endl<<"Problems with newtons method:" << endl << "1/2 =" << endl;

    BigDecimal b(1, 1000);
    b += 2;
    BigDecimal::invert(b).print();

    cout << endl << "sqrt(2) =" << endl;

    BigDecimal a(4, 1000);
    a += 2;
    BigDecimal::sqrt(a).print();

    cout << endl << "Finished";
    return 0;
}

