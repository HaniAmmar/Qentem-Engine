#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    for (int i = 0; i < 10; i++) {
        value["list"] += i;
    }

    value["size"] = 5;

    const char *content = R"(<loop set="list" value="loop1-value">{var:loop1-value}</loop>)";

    std::cout << Template::Render<StringStream<char>>(content, value) << '\n';

    /*
        Output:
            0123456789
    */
}
