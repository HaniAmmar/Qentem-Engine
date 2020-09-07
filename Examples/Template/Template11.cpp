#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value = JSON::Parse(R"([0,1,2,3])");

    String content(R"(
<if case="{var:0} == 0">
Zero!<if case="{var:1} == 1"> and one<if case="{var:2} == 2"> and two!</if></if>
</if>

<if case="({var:3} + 5) == 0">
Zero!
<else />
<if case="{var:3} == 3">{var:3}+5 is 8 not {var:3}</if>
</if>
)");

    std::cout << Template<>::Render(content, &value).Storage() << '\n';

    /*
        Output:
        Zero! and one and two!

        3+5 is 8 not 3
    */
}
