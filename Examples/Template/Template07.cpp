#include "Qentem/Template.hpp"
#include "Qentem/Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template07.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;

    value += 0;
    value += 1;

    const char *content =
        R"(<loop value="loop1-value"><loop value="loop2-value"><loop value="loop3-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}) </loop></loop></loop>)";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
                (0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1)
    */
}
