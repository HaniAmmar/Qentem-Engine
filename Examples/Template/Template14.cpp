#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template14.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;

    value += 4;
    value += 5;
    value += 1;
    value += 6;
    value += 2;
    value += 7;
    value += 3;

    const char *content = R"(
<loop value="val_1" sort="ascend">{var:val_1} </loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
    /*
        Output: 1 2 3 4 5 6 7
    */

    const char *content2 = R"(
<loop value="val_1" sort="descend">{var:val_1} </loop>
    )";

    stream.Clear();

    Template::Render(content2, value, stream);
    std::cout << stream << '\n';
    /*
        Output: 7 6 5 4 3 2 1
    */
}
