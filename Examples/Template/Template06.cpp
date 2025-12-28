#include "Qentem/JSON.hpp"
#include "Qentem/Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;

/*
mkdir Build
c++ -g ./Examples/Template/Template06.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    auto value = JSON::Parse(R"(
{
    "Group-1": [["A","B","C"],["DD","EE","FF"],["GGG","HHH","Qentem"]],
    "Group-2": [[1,2,3],[10,20,30],[100,200,300]]
}
    )");

    const char *content = R"(
<loop value="loop1-val">
    <loop set="loop1-val" value="loop2-val">
        <loop set="loop2-val" value="loop3-val">{var:loop3-val}
        </loop>
    </loop>
</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            A
            B
            C

            DD
            EE
            FF

            GGG
            HHH
            Qentem

            1
            2
            3

            10
            20
            30

            100
            200
            300
    */
}
