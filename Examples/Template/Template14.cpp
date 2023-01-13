#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value += 4;
    value += 5;
    value += 1;
    value += 6;
    value += 2;
    value += 7;
    value += 3;

    const char *content = R"(
<loop value="val1_" sort="ascend">{var:val1_} </loop>
    )";

    std::cout << Template::Render(content, value) << '\n';
    /*
        Output: 1 2 3 4 5 6 7
    */

    content = R"(
<loop value="val1_" sort="descend">{var:val1_} </loop>
    )";

    std::cout << Template::Render(content, value) << '\n';
    /*
        Output: 7 6 5 4 3 2 1
    */
}
