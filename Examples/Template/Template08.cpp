#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    for (int i = 0; i < 10; i++) {
        value["list"] += i;
    }

    value["size"] = 5;

    const char *content =
        R"(<loop set="list" repeat="{var:size}" index="5" value="loop1-value">loop1-value</loop>)";

    std::cout << Template::Render(content, &value).GetString() << '\n';

    /*
        Output:
            56789
    */

    value["size"]     = 7;
    value["start_at"] = 3;

    content =
        R"(<loop set="list" repeat="{var:size}" index="{var:start_at}" value="loop1-value">loop1-value</loop>)";

    std::cout << Template::Render(content, &value).GetString() << '\n';

    /*
        Output:
            3456789
    */
}
