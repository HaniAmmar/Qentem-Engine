#include "Qentem/Template.hpp"
#include "Qentem/Value.hpp"
#include "Qentem/QConsole.hpp"

using Qentem::QConsole;
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

    QConsole::Print(stream, '\n');

    /*
        Output:
            Qentem, 3
    */
}
