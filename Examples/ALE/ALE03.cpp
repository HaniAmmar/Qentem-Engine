#include "ALE.hpp"

#include <iostream>

using Qentem::ALE;

int main() {
    std::cout << ALE::Evaluate(" A ==   A  ") << '\n'; // 1
    std::cout << ALE::Evaluate("A==A") << '\n';        // 1
    std::cout << ALE::Evaluate("A!=a") << '\n';        // 1
    std::cout << ALE::Evaluate("A==a") << '\n';        // 0
    std::cout << ALE::Evaluate("ABCD!=BCD") << '\n';   // 1
    std::cout << ALE::Evaluate("ABCD==abcd") << '\n';  // 0
}
