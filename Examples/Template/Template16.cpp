#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template16.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;
    value += 3;
    value += 7;
    value += 16;
    value += 18;

    const char *content = R"(
{if case="{var:0} + {var:1} == 10" true="{var:2}" false="{var:3}"}
{if case="{var:0} + {var:1} != 10" true="{var:2}" false="{var:3}"}
{if case="{var:0} + {var:1} == 10" true="{var:2}"}
{if case="{var:0} + {var:1} != 10" false="{var:3}"}
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
    /*
        Output:
            16
            18
            16
            18
    */
}
