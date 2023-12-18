#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = JSON::Parse(R"(
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
{var:loop1-value[name]}: {var:loop1-value[value]}</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            a: 1
            b: 2
            c: 3
    */
}
