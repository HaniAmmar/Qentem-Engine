#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["d"]["a"] = 10;
    value["d"]["b"] = 20;
    value["d"]["c"] = 30;

    const char *content = R"(
<loop set="d" key="loopID1" value="loopVAL1">
loopID1: loopVAL1</loop>
    )";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            a: 10
            b: 20
            c: 30
    */
}
