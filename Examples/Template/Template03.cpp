#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
{
    "a": 1,
    "b": 2,
    "c": 3
}
    )");

    const char *content = R"(
<loop key="loopID1" value="loopVAL1">
loopID1: loopVAL1</loop>
    )";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            a: 1
            b: 2
            c: 3
    */
}
