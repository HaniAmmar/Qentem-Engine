#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template08.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;

    for (int i = 0; i < 10; i++) {
        value["list"] += i;
    }

    const char *content = R"(<loop set="list" value="loop1-value">{var:loop1-value}</loop>)";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            0123456789
    */
}
