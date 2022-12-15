#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    const Value<char> value = Qentem::JSON::Parse(
        R"([{"year":2019,"month":4},{"year":2020,"month":1},{"year":2017,"month":1},{"year":2020,"month":5},{"year":2018,"month":2},{"year":2020,"month":7},{"year":2018,"month":3}])");

    const char *content = R"(
<loop value="val1_" group="year" sort="ascend">Year(val1_):
    <loop set="val1_" value="val2_">Month(val2_[month])
    </loop>
</loop>
    )";

    std::cout << Template::Render(content, &value) << '\n';
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

    // Without the names of the values.
    const char *content2 = R"(
<loop value="val1_" group="year" sort="ascend">Year(val1_):
    <loop set="val1_" value="val2_">Month(<loop set="val2_" value="val3_">val3_</loop>)
    </loop>
</loop>
    )";

    std::cout << Template::Render(content2, &value) << '\n';
}
