#include <iostream>
#include "BigDecimal/BigDecimal.h"
//using namespace std;

int main() {
    std::cout << pow(10, 3) << std::endl;
    BigDecimal y(1000);
    y += 21232;
    y.print();
    BigDecimal z(104);
    z += 31533;
    for (int i = 0; i < 10; ++i) {
        z.auz[i] = i;
    }
    z.print();
    // TODO: Climbs exponentially, so test all basic operators and their effectivity
    BigDecimal x = y * z;
    //BigDecimal x = y / 4;
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

