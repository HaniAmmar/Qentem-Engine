#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value;

    value["d"]["a"] = 10;
    value["d"]["b"] = 20;
    value["d"]["c"] = 30;

    String content(R"(
<loop set="d" key="loopID1" value="loopVAL1">
loopID1: loopVAL1</loop>
    )");

    std::cout << Template<>::Render(content, &value).Storage() << '\n';

    /*
        Output:
            a: 10
            b: 20
            c: 30
    */
}
