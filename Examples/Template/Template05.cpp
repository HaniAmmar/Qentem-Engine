#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value[0] = 40;
    value[1] = 60;
    value[2] = 80;

    const char *content = R"(
<loop value="loop1-value">
loop1-value</loop>
    )";

    std::cout << Template::Render(content, &value) << '\n';

    /*
        Output:
        40
        60
        80
    */
}
