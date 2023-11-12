#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["some_set"]["a"] = 10;
    value["some_set"]["b"] = 20;
    value["some_set"]["c"] = 30;

    const char *content = R"(
<loop set="some_set" value="loop1-value">
{var:loop1-value}</loop>
    )";

    std::cout << Template::Render<StringStream<char>>(content, value) << '\n';

    /*
        Output:
            10
            20
            30
    */
}
