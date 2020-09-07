#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value = JSON::Parse(R"(
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

    String content(R"(
<loop set="object" value="item">
item[var1] item[var2] item[var3] item[var4]</loop>

<loop set="array" value="item">
item[0] item[1] item[2] item[3]</loop>
    )");

    std::cout << Template<>::Render(content, &value).Storage() << '\n';

    /*
        Output:
            value1 value2 value3 value4
            value11 value22 value33 value44


            value10 value20 value30 value40
            value100 value200 value300 value400
    */
}
