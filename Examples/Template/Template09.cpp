#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value;

    value["size"] = 10;

    String content(
        R"(<loop times="{var:size}" index="5" key="loop1-id">loop1-id</loop>)");

    std::cout << Template<>::Render(content, &value).Char() << '\n';

    /*
        Output:
            56789
    */

    value["start-at"] = 3;

    content =
        R"(<loop times="{var:size}" index="{var:start-at}" key="loop1-id">loop1-id</loop>)";

    std::cout << Template<>::Render(content, &value).Char() << '\n';

    /*
        Output:
            3456789
    */
}
