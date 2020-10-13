#include "ALE.hpp"
#include "Array.hpp"

#include <iostream>

int main() {
    Qentem::Array<const char *> math;

    math += "+1+1";                         // 2
    math += "---1";                         // -1
    math += "-1+3";                         // 2
    math += "-1*8";                         // -8
    math += "--1*5";                        // 5
    math += "1--1";                         // 2
    math += "11==11";                       // 1
    math += "11!=11";                       // 0
    math += "11>2";                         // 1
    math += "11>=2";                        // 1
    math += "1<2";                          // 1
    math += "2^8";                          // 256
    math += "11<=11";                       // 1
    math += "11&&2";                        // 1
    math += "4*2+6/3";                      // 10
    math += "(2+3)*5";                      // 25
    math += "(5)+3";                        // 8
    math += "((1+2)^(1+2))/2";              // 13.5
    math += "((5/5+1)*2+1)+3*3";            // 14
    math += "5+2*4-8/2==9 && 1";            // 1
    math += "((5/5+1)*2+1)+3*3 != 12 && 1"; // 1
    math +=
        R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) *
        (8 / 4 + 1) - 1 - -1 + 2 == ((5/5+1)*2+1)+3*3)"; // 1
    ///////////////////////////////////////////////////

    for (unsigned int i = 0; i < math.Size(); i++) {
        std::cout << Qentem::ALE::Evaluate(math[i]) << '\n';
    }
}
