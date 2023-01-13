#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
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

    std::cout << Template::Render(content, value) << '\n';

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
