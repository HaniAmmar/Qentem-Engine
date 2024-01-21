#include "Template.hpp"
#include "JSON.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template18.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    Value<char> value;

    value["phrase"] = "Welcome {0} to {1}. You have {2} points. "
                      "Your points will become {3} next month. "
                      "Your points will be here when you login to {1} every {4}.";

    value["username"]    = "X";
    value["site_name"]   = "Y";
    value["points"]      = 10U;
    value["points_html"] = R"(<span id="pid">10</span>)";
    value["time"]        = "week";

    const char *content =
        "{svar:phrase, {var:username}, {var:site_name}, {raw:points_html}, {math: {var:points} * 2}, {var:time}}";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
    /*
        Output: Welcome X to Y. You have <span id="pid">10</span> points. Your points will become 20 next month. Your
                points will be here when you login to Y every week.
    */

    //////////////////////////////////////
    value.Reset();
    stream.Clear();

    const char *json_data = R"(
        {
            "phrase": "user {0} logged in {1}.\n",
            "list": [
                {
                    "name": "X",
                    "last_seen": "today"
                },
                {
                    "name": "Y",
                    "last_seen": "yesterday"
                },
                {
                    "name": "Z",
                    "last_seen": "last week"
                }
            ]
        }
    )";

    value = JSON::Parse(json_data);

    content = R"(<loop set="list" value="val">{svar:phrase, {var:val[name]}, {var:val[last_seen]}}</loop>)";

    Template::Render(content, value, stream);
    std::cout << '\n' << stream << '\n';

    /*
    Output:
        user User1 logged in today.
        user User2 logged in yesterday.
        user User3 logged in last week.
    */
}
