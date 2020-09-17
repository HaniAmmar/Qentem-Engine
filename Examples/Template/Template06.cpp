#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
{
    "Group-1": [["A","B","C"],["DD","EE","FF"],["GGG","HHH","Qentem"]],
    "Group-2": [[1,2,3],[10,20,30],[100,200,300]]
}
    )");

    const char *content = R"(
<loop key="loop1-key">
    <loop set="loop1-key" key="loop2-key">
loop1-key: loop2-key:
        <loop set="loop1-key[loop2-key]" key="loop3-key" value="loop3-val">
        loop3-key: loop3-val</loop>
    </loop>
</loop>
    )";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            Group-1: 0:

                    0: A
                    1: B
                    2: C

            Group-1: 1:

                    0: DD
                    1: EE
                    2: FF

            Group-1: 2:

                    0: GGG
                    1: HHH
                    2: Qentem



            Group-2: 0:

                    0: 1
                    1: 2
                    2: 3

            Group-2: 1:

                    0: 10
                    1: 20
                    2: 30

            Group-2: 2:

                    0: 100
                    1: 200
                    2: 300
    */
}
