#include "Qentem/Template.hpp"
#include "Qentem/Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template05.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;

    value[0] = 40;
    value[1] = 60;
    value[2] = 80;

    const char *content = R"(
<loop value="loop1-value">
{var:loop1-value}</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
        40
        60
        80
    */
}
