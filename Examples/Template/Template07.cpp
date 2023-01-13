#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value += 0;
    value += 1;

    const char *content =
        R"(<loop value="loop1-value"><loop value="loop2-value"><loop value="loop3-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}) </loop></loop></loop>)";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
                (0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1)
    */
}
