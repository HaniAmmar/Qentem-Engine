#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
[
    {
        "major": "Computer Science",
        "students": [
            { "Name": "Oliver", "GPA": 3.2 },
            { "Name": "Jonah", "GPA": 3.8 },
            { "Name": "Jack", "GPA": 2.8 }
        ]
    },
    {
        "major": "Math",
        "students": [
            { "Name": "Maxim", "GPA": 3.0 },
            { "Name": "Cole", "GPA": 2.5 },
            { "Name": "Claire", "GPA": 2.4 }
        ]
    }
]
    )");

    const char *content = R"(
<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Qentem Example</title>
</head>

<body>
    <div>
        <h2>Students' list:</h2>
        <loop value="department_val">
            <h3>Major: department_val[major]</h3>
            <ul>
            <loop set="department_val[students]" value="student_val">
                <li>
                    <span>Name: student_val[Name]</span>
                    <span>
                        GPA: student_val[GPA]
                        <if case="student_val[GPA] < 2.5"> (Inform adviser!)
                        <elseif case="student_val[GPA] >= 3.5" /> (President's List!)
                        <elseif case="student_val[GPA] >= 3.0" /> (Dean's List!)
                        </if>
                    </span>
                </li>
            </loop>
            </ul>
        </loop>
    </div>
</body>

</html>
)";

    std::cout << Template::Render(content, &value) << '\n';
}
