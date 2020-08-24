#include "ALE.hpp"

#include <iostream>

using Qentem::ALE;
using Qentem::Array;
using Qentem::String;
using Qentem::UInt;
using Qentem::ULong;

int main() {
    Array<const char *> equations;

    equations += "+1+1";                         // 2
    equations += "---1";                         // -1
    equations += "-1+3";                         // 2
    equations += "-1*8";                         // -8
    equations += "--1*5";                        // 5
    equations += "1--1";                         // 2
    equations += "11==11";                       // 1
    equations += "11!=11";                       // 0
    equations += "11>2";                         // 1
    equations += "11>=2";                        // 1
    equations += "1<2";                          // 1
    equations += "2^8";                          // 256
    equations += "11<=11";                       // 1
    equations += "11&&2";                        // 1
    equations += "4*2+6/3";                      // 10
    equations += "(2+3)*5";                      // 25
    equations += "(5)+3";                        // 8
    equations += "((1+2)^(1+2))/2";              // 13.5
    equations += "((5/5+1)*2+1)+3*3";            // 14
    equations += "5+2*4-8/2==9 && 1";            // 1
    equations += "((5/5+1)*2+1)+3*3 != 12 && 1"; // 1
    equations +=
        R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) *
        (8 / 4 + 1) - 1 - -1 + 2 == ((5/5+1)*2+1)+3*3)"; // 1
    ///////////////////////////////////////////////////

    for (ULong i = 0; i < equations.Size(); i++) {
        std::cout << ALE::Evaluate(equations[i]) << '\n';
    }
}
