#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template01.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;

    value["name"]       = "Qentem";
    value["version"]    = 3.0;
    const char *content = R"({var:name}, {var:version})";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            Qentem, 3
    */
}
