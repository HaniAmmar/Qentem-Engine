#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template03.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

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
