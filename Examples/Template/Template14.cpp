#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value += 4;
    value += 5;
    value += 1;
    value += 6;
    value += 2;
    value += 7;
    value += 3;

    const char *content = R"(
<loop value="_val1" sort="ascend">{var:_val1} </loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
    /*
        Output: 1 2 3 4 5 6 7
    */

    const char *content2 = R"(
<loop value="_val1" sort="descend">{var:_val1} </loop>
    )";

    stream.Clear();

    Template::Render(content2, value, stream);
    std::cout << stream << '\n';
    /*
        Output: 7 6 5 4 3 2 1
    */
}
