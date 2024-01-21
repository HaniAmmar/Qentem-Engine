#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template09.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    auto value = JSON::Parse(R"(
{
    "object": [
        {
            "var1": "value1",
            "var2": "value2",
            "var3": "value3",
            "var4": "value4"
        },        {
            "var1": "value11",
            "var2": "value22",
            "var3": "value33",
            "var4": "value44"
        }
    ],
    "array": [
        [
            "value10",
            "value20",
            "value30",
            "value40"
        ],
        [
            "value100",
            "value200",
            "value300",
            "value400"
        ]
    ]
}
    )");

    const char *content = R"(
<loop set="object" value="item">
{var:item[var1]} {var:item[var2]} {var:item[var3]} {var:item[var4]}</loop>

<loop set="array" value="item">
{var:item[0]} {var:item[1]} {var:item[2]} {var:item[3]}</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            value1 value2 value3 value4
            value11 value22 value33 value44


            value10 value20 value30 value40
            value100 value200 value300 value400
    */
}
