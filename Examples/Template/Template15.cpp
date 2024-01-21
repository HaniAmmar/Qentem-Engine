#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

/*
mkdir Build
c++ -g ./Examples/Template/Template15.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    const Value<char> value = JSON::Parse(
        R"([{"year":2019,"month":4},{"year":2020,"month":1},{"year":2017,"month":1},{"year":2020,"month":5},{"year":2018,"month":2},{"year":2020,"month":7},{"year":2018,"month":3}])");

    const char *content = R"(
<loop value="val_1" group="year" sort="ascend">Year({var:val_1}):
    <loop set="val_1" value="val_2">Month({var:val_2[month]})
    </loop>
</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
    /*
        Output:
            Year(2017):
                Month(1)

            Year(2018):
                Month(2)
                Month(3)

            Year(2019):
                Month(4)

            Year(2020):
                Month(1)
                Month(5)
                Month(7)
    */

    // Without values names.
    const char *content2 = R"(
<loop value="val_1" group="year" sort="ascend">Year({var:val_1}):
    <loop set="val_1" value="val_2">Month(<loop set="val_2" value="val_3">{var:val_3}</loop>)
    </loop>
</loop>
    )";

    stream.Clear();
    Template::Render(content2, value, stream);
    std::cout << stream << '\n';
}
