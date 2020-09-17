#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value[0] = 5;

    const char *content =
        R"(<loop times="{var:0}" index="5" key="loop1-id">loop1-id</loop>)";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            56789
    */

    value[1] = 3;

    content =
        R"(<loop times="{var:0}" index="{var:1}" key="loop1-id">loop1-id</loop>)";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            3456789
    */
}
