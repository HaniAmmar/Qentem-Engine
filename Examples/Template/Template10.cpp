#include "Qentem/Template.hpp"
#include "Qentem/Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template10.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;

    value += 0;
    value += 1;
    value += 2;
    value += 3;

    const char *content = R"(
<if case="{var:0} == 0">
Zero!
</if>

<if case="{var:1} == 0">
Zero!
<else />
Not {var:0} but {var:1}.
</if>

<if case="{var:2} == 0">
Zero!
<else if case="{var:2} == 2">
Two!
<else>
Not zero or one.
</if>

<if case="{var:2} == 0">
Zero!
<elseif case="{var:2} == 5" />
Two!
<elseif case="{var:3} == 3" />
{var:3}
<else />
Not zero or one or two.
</if>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            Zero!

            Not 0 but 1.

            Two!

            3
    */
}
