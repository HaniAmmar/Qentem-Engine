#include "Qentem/Template.hpp"
#include "Qentem/Value.hpp"
#include "Qentem/QConsole.hpp"

using Qentem::QConsole;
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
    QConsole::Print(stream, '\n');

    /*
        Output:
            0123456789
    */
}
