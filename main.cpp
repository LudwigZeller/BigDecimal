#include <iostream>
#include "BigDecimal/BigDecimal.h"

using namespace std;

int main() {
    BigDecimal y(1000);
    y += 69;
    y.print();
    BigDecimal z(104);
    z += 420;
//    for (int i = 0; i < 100; ++i) {
//        z.auz[i] = i;
//    }
//    z.relocate();
    z.print();
    //BigDecimal x = y * z;
    // TODO: Climbs exponentially, so test all basic operators and their effectivity
    BigDecimal x = y / 4;
    x.print();


//    BigDecimal approx(15, 15);
//    BigDecimal check1(15, 15);
//    check1 += 1;
//    int converging_limit = 100;
//    BigDecimal to_invert(10, 10);
//    to_invert += 3;
//    if (to_invert.range.oz) {
//        approx.auz[to_invert.range.oz] = 1;
//    } else {
//        if (!to_invert.aoz[0]) {
//            for (int i = 0; i <= to_invert.range.uz; ++i) {
//                if (to_invert.auz[i]) {
//                    approx.aoz[i] = 1;
//                    break;
//                }
//            }
//        } else {
//            approx.auz[to_invert.range.oz] = 1;
//        }
//    }
//
//    cout << endl;
//
//    BigDecimal check2(15, 15);
//    for (int i = 0; i < converging_limit; ++i) {
//        if (check1 == check2)
//            break;
//        check1 = approx;
//        approx.print();
//        BigDecimal temp1 = (approx * 2);
//        temp1.print();
//        BigDecimal temp2 = approx * approx;
//        temp2.print();
//        BigDecimal temp3 = temp2 * to_invert;
//        temp3.print();
//        BigDecimal temp4 = temp1 - temp3;
//        temp4.print();
//        approx = temp4;
//        check2 = approx;
//        check1.resize(true, to_invert.puz);
//        check2.resize(true, to_invert.puz);
//        cout << endl;
//        check1.print();
//        check2.print();
//        cout << endl << endl;
//    }
//
//    cout << endl << endl;
//    check1.print();
//    check2.print();
//    cout << (check1 == check2) << endl;
//

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

