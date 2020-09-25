#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
[
    {
        "name": "a",
        "value": 1
    },
    {
        "name": "b",
        "value": 2
    },
    {
        "name": "c",
        "value": 3
    }
]
    )");

    const char *content = R"(
<loop value="loop1-value">
loop1-value[name]: loop1-value[value]</loop>
    )";

    std::cout << Template::Render(content, &value).GetString().First() << '\n';

    /*
        Output:
            a: 1
            b: 2
            c: 3
    */
}
