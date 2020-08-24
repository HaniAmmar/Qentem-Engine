#include "ALE.hpp"

#include <iostream>

using Qentem::ALE;

int main() {
    double result;

    if (ALE::Evaluate(result, "8^-2")) {
        std::cout << result << '\n';
    }

    if (!ALE::Evaluate(result, "abc")) {
        std::cout << "invalid" << '\n';
    }

    if (!ALE::Evaluate(result, "a+2")) {
        std::cout << "invalid" << '\n';
    }
}
