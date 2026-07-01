#include "Qentem/JSON.hpp"
#include "Qentem/Template.hpp"
#include "Qentem/QConsole.hpp"

using Qentem::JSON;
using Qentem::QConsole;
using Qentem::StringStream;
using Qentem::Template;

/*
mkdir Build
c++ -g ./Examples/Template/Template11.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    auto value = JSON::Parse(R"([0,1,2,3])");

    const char *content = R"(
<if case="{var:0} == 0">
Zero!<if case="{var:1} == 1"> and one<if case="{var:2} == 2"> and two!</if></if>
</if>

<if case="({var:3} + 5) == 0">
Zero!
<else />
<if case="{var:3} == 3">{var:3}+5 is 8 not {var:3}</if>
</if>
)";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    QConsole::Print(stream, '\n');

    /*
        Output:
        Zero! and one and two!

        3+5 is 8 not 3
    */
}
