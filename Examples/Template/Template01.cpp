#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["name"]       = "Qentem";
    value["version"]    = 3.0;
    const char *content = R"({var:name}, {var:version})";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            Qentem, 3
    */
}
