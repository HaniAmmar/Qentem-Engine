#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value;

    value["name"]    = "Qentem";
    value["version"] = 3.0;

    String content(R"({var:name}, {var:version})");
    std::cout << Template<>::Render(content, &value).Char() << '\n';

    // Or
    // const char *temp = R"({var:name}, {var:version})";
    // std::cout << Template<>::Render(temp, &value).Char() << '\n';
}
