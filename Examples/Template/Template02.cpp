#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;
    value["one"]   = "1";
    value["two"]   = "2";
    value["three"] = "3";
    value["four"]  = "4";

    const char *content = R"(
0.1+0.2 is: {math: 0.1  +   0.2 }
{var:one}+{var:four}*{var:two}+{var:one} = {math:{var:one}+{var:four}*{var:two}+{var:one}}; (1+8+1)
6^2 = {math:6^2}
{var:one}+{var:three} = {math:{var:one}+{var:three}}
9 % 5 = {math:9 % 5}
)";

    std::cout << Template::Render<StringStream<char>>(content, value) << '\n';

    /*
        Output:
            0.1+0.2 is: 0.3
            1+4*2+1 = 10; (1+8+1)
            6^2 = 36
            1+3 = 4
            9 % 5 = 4
    */
}
