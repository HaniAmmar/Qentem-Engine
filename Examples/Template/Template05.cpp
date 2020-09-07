#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value;

    value[0] = 40;
    value[1] = 60;
    value[2] = 80;

    String content(R"(
<loop key="loop1-id" value="loop1-value">
[loop1-id]: loop1-value</loop>
    )");

    std::cout << Template<>::Render(content, &value).Storage() << '\n';

    /*
        Output:
        [0]: 40
        [1]: 60
        [2]: 80
    */
}
