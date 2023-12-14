#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    const Value<char> value = JSON::Parse(
        R"([{"year":2019,"month":4},{"year":2020,"month":1},{"year":2017,"month":1},{"year":2020,"month":5},{"year":2018,"month":2},{"year":2020,"month":7},{"year":2018,"month":3}])");

    const char *content = R"(
<loop value="_val1" group="year" sort="ascend">Year({var:_val1}):
    <loop set="_val1" value="_val2">Month({var:_val2[month]})
    </loop>
</loop>
    )";

    std::cout << Template::Render<StringStream<char>>(content, value) << '\n';
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
<loop value="_val1" group="year" sort="ascend">Year({var:_val1}):
    <loop set="_val1" value="_val2">Month(<loop set="_val2" value="_val3">{var:_val3}</loop>)
    </loop>
</loop>
    )";

    std::cout << Template::Render<StringStream<char>>(content2, value) << '\n';
}
