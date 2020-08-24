#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::JSON::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value = JSON::Parse(R"(
{
    "a": 1,
    "b": 2,
    "c": 3
}
    )");

    String content(R"(
<loop key="loopID1" value="loopVAL1">
loopID1: loopVAL1</loop>
    )");

    std::cout << Template<>::Render(content, &value).Char() << '\n';

    /*
        Output:
            a: 1
            b: 2
            c: 3
    */
}
