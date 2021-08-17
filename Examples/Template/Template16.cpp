#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;
    value += 3;
    value += 7;
    value += 16;
    value += 18;

    const char *content = R"(
{if case="{var:0} + {var:1} == 10" true="{var:2}" false="{var:3}"}
{if case="{var:0} + {var:1} != 10" true="{var:2}" false="{var:3}"}
{if case="{var:0} + {var:1} == 10" true="{var:2}"}
{if case="{var:0} + {var:1} != 10" false="{var:3}"}
    )";

    std::cout << Template::Render(content, &value).GetString() << '\n';
    /*
        Output:
            16
            18
            16
            18
    */
}
