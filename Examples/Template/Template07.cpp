#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    const char *content = R"(<loop times="2" key="loop1-id"><loop
        times="2" key="loop2-id"><loop
        times="2" key="loop3-id">(loop1-id: loop2-id: loop3-id) </loop></loop></loop>)";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
(0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1)
    */
}
