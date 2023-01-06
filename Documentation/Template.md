# Template.hpp

-   [Variable](#variable)
-   [Raw Variable](#raw-variable)
-   [Math](#math)
-   [Inline If](#inline-if)
-   [Loop](#loop)
-   [If Condition](#if-condition)

## Variable

```txt
{var:...}
```

The tag `{var:...}` is used to access a value by its id/index or by its name/key. It can be a child value, like: `ID1[ID2][ID3]...` or `key1[key2][key3]`, or both: `ID1[Key1][ID2][Key3]...`.

Unordered array:

```json
{
    "name": "Qentem",
    "version": 3.0
}
```

```txt
{var:name}
{var:version}
```

When rendered, the output will be:

```txt
Qentem
3.0
```

If it's a nested list:

```json
{
    "list1": {
        "item1": 5,
        "item2": {
            "subItem1": 10
        }
    }
}
```

Then every item can be reached using its `key`:

```txt
{var:list1[item1]}: 5
{var:list1[item2][subItem1]}: 10
```

In the case of ordered arrays, use a numeric number.

```json
["a", "b", 1, ["x", [true]]]
```

```txt
{var:0}: a
{var:1}: b
{var:2}: 1
{var:3[0]}: X
{var:3[1][0]}: true
```

Mixed of both:

```json
{
    "list1": [
        1,
        true,
        {
            "name": "Qentem"
        }
    ]
}
```

```txt
{var:list1[0]}: 1
{var:list1[1]}: true
{var:list1[2][name]}: Qentem
```

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["name"]       = "Qentem";
    value["version"]    = 3.0;
    const char *content = R"({var:name}, {var:version})";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            Qentem, 3
    */
}
```

### Variable Tag Note

Spaces after `{var:` or before `}` are considered part of the variable's name, and any string value will be made safe to be printed to the browser; by escaping HTML special chars (>, <, ", ' , &).

## Raw Variable

Raw variable tag is the same as Variable tag, except it does not escape HTML special chars (>, <, ", ' , &).

## Math

```txt
{math:...}
```

This tag passes its value to [ALE](https://github.com/HaniAmmar/Qentem-Engine/blob/main/Documentation/ALE.md) for arithmetic and logic evaluation.

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;
    value["some_math"] = "1+4*2+1";
    value["one"]       = "1";
    value["three"]     = "3";

    const char *content = R"(
0.1+0.2 is: {math: 0.1  +   0.2 }
{var:some_math} = {math:{var:some_math}}; (1+8+1)
6^2 = {math:6^2}
--1 = {math:--1}
{var:one}+{var:three} = {math:{var:one}+{var:three}}
9 % 5 = {math:9 % 5}
)";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            0.1+0.2 is: 0.3
            1+4*2+1 = 10; (1+8+1)
            6^2 = 36
            --1 = 1
            1+3 = 4
            9 % 5 = 4
    */
}
```

## Inline If

```txt
{if case="..." true="..." false="..."}
```

Inline condition. [ALE](https://github.com/HaniAmmar/Qentem-Engine/blob/main/Documentation/ALE.md) takes the `case` value to evaluate it, and if the returned value is a positive number, it will print the value inside `true`. Otherwise, `false`'s value is used.

```txt
{if case="1" true="one" false="not one"} // Fine.
{if case="2!=1" false="not one"} // Fine.
{if case="1&&1" true="one"} // Fine.
{if true="one" case="1"} // OK.

{if true="one" false="not one"} // Wrong.
{if case="1"} // Wrong.
{if true="one"} // Wrong.
{if false="not one"} // Wrong.
```

With variables:

```json
["Correct!", "WRONG!", 4, 5, 9]
```

```txt
{if case="{var:3} + {var:2} == {var:4}" true="{var:0}" false="{var:1}"}: Output "Correct!".
{if case="{var:3} + {var:2} == {var:4}" true="{var:0}"}: Same.
{if case="{var:3}" true="{var:0}"}: Same.

{if case="{var:3} + {var:2} == {var:4}" false="{var:1}"}: Output nothing.
{if case="{var:3} + {var:2} != {var:4}" true="{var:0}"}: Output nothing.
{if case="{var:3} == {var:2}" false="{var:1}"}: Output "WRONG!".


{if case="a==a" false="{var:1}" true="{var:0}"}: Output "Correct!".
{if case="a  ==  a" false="{var:1}" true="{var:0}"}: Output "Correct!".
{if case  =  "a  ==  a" false  =  "{var:1}" true   = "{var:0}"}: Same.
```

### Inline If Tag Note

`true` and `false` only accept Variable tag, Raw variable tag and Math tag.

### ALE Note

ALE uses `1` for `true`, 0 for `false` and `null`. if the operation is equal, Template will try the following:

-   If one of the variables is a number, it will convert the other part to a number: ({var:one} == 1)
-   If none of the variables is a number, it will convert the other part to a string: ({var:bool} == true)

To force ALE to treat the variables as numbers, use parentheses: (({var:bool}) == 1)

## Loop

```txt
<loop set="..." value="..." repeat="..." index="..." group="..." sort="...">...</loop>
<loop value="...">...</loop>
<loop set="..." value="...">...</loop>
<loop set="..." value="..." repeat="...">...</loop>
<loop set="..." value="..." repeat="..." index="...">...</loop>
<loop set="..." value="..." repeat="..." index="..." group="...">...</loop>

<loop repeat="...">...</loop>
<loop set="..." repeat="..." index="...">...</loop>
<loop set="..." repeat="...">...</loop>
<loop set="..." index="...">...</loop>
```

Loops over a set and replaces the values with the string inside `value`. The size can be set using `repeat` option. and the index can start from the value of `index`. The options `repeat` and `index` can be numbers or a variable tag. the `set` works like `{var:...}` and can be used to point to a sub-array. `value` accept only strings; for matching and replacing of the values inside the loop. `group` groups the giving array by sub-value of an object. `sort` will sort the set in ascendind or descending order.

### Loop Example 1 (unordered set)

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
[
    {
        "name": "a",
        "value": 1
    },
    {
        "name": "b",
        "value": 2
    },
    {
        "name": "c",
        "value": 3
    }
]
    )");

    const char *content = R"(
<loop value="loop1-value">
loop1-value[name]: loop1-value[value]</loop>
    )";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            a: 1
            b: 2
            c: 3
    */
}
```

### Loop Example 2 (unordered sub-set)

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["some_set"]["a"] = 10;
    value["some_set"]["b"] = 20;
    value["some_set"]["c"] = 30;

    const char *content = R"(
<loop set="some_set" value="loop1-value">
loop1-value</loop>
    )";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            10
            20
            30
    */
}
```

### Loop Example 3 (ordered set)

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value[0] = 40;
    value[1] = 60;
    value[2] = 80;

    const char *content = R"(
<loop value="loop1-value">
loop1-value</loop>
    )";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
        40
        60
        80
    */
}
```

### Loop Example 4 (nested sets)

```cpp
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
<loop value="loop1-val">
    <loop set="loop1-val" value="loop2-val">
        <loop set="loop2-val" value="loop3-val">
            loop3-val
        </loop>
    </loop>
</loop>
    )";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            A
            B
            C

            DD
            EE
            FF

            GGG
            HHH
            Qentem

            1
            2
            3

            10
            20
            30

            100
            200
            300
    */
}
```

### Loop Example 5 (repeat)

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value += 0;
    value += 1;

    const char *content =
        R"(<loop repeat="2" value="loop1-value"><loop repeat="2" value="loop2-value"><loop
        repeat="2" value="loop3-value">(loop1-value: loop2-value: loop3-value) </loop></loop></loop>)";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
(0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1)
    */
}
```

### Loop Example 6 (index)

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    for (int i = 0; i < 10; i++) {
        value["list"] += i;
    }

    value["size"] = 5;

    const char *content =
        R"(<loop set="list" repeat="{var:size}" index="5" value="loop1-value">loop1-value</loop>)";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            56789
    */

    value["size"]     = 7;
    value["start_at"] = 3;

    content =
        R"(<loop set="list" repeat="{var:size}" index="{var:start_at}" value="loop1-value">loop1-value</loop>)";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            3456789
    */
}
```

### Loop Example 7 (sub value)

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
{
    "object": [
        {
            "var1": "value1",
            "var2": "value2",
            "var3": "value3",
            "var4": "value4"
        },        {
            "var1": "value11",
            "var2": "value22",
            "var3": "value33",
            "var4": "value44"
        }
    ],
    "array": [
        [
            "value10",
            "value20",
            "value30",
            "value40"
        ],
        [
            "value100",
            "value200",
            "value300",
            "value400"
        ]
    ]
}
    )");

    const char *content = R"(
<loop set="object" value="item">
item[var1] item[var2] item[var3] item[var4]</loop>

<loop set="array" value="item">
item[0] item[1] item[2] item[3]</loop>
    )";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            value1 value2 value3 value4
            value11 value22 value33 value44


            value10 value20 value30 value40
            value100 value200 value300 value400
    */
}
```

### Loop Example 8 (Sorting)

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

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
<loop value="val1_" sort="ascend">val1_ </loop>
    )";

    std::cout << Template::Render(content, value) << '\n';
    /*
        Output: 1 2 3 4 5 6 7
    */

    content = R"(
<loop value="val1_" sort="descend">val1_ </loop>
    )";

    std::cout << Template::Render(content, value) << '\n';
    /*
        Output: 7 6 5 4 3 2 1
    */
}
```

### Loop Example 9 (Grouping)

```cpp
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

    std::cout << Template::Render(content, value) << '\n';
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
}
```

## If Condition

```txt
<if case="...">...</if>
<if case="...">...<else />...</if>
<if case="...">...<elseif case="..." />...</if>
<if case="...">...<elseif case="..." />...<else />...</if>
```

Similar to `{if case="..." ...}`, but capable of branching, and nesting.

### If Example 1

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value += 0;
    value += 1;
    value += 2;
    value += 3;

    const char *content = R"(
<if case="{var:0} == 0">
Zero!
</if>

<if case="{var:1} == 0">
Zero!
<else />
Not {var:0} but {var:1}.
</if>

<if case="{var:2} == 0">
Zero!
<elseif case="{var:2} == 2" />
Two!
<else />
Not zero or one.
</if>

<if case="{var:2} == 0">
Zero!
<elseif case="{var:2} == 5" />
Two!
<elseif case="{var:3} == 3" />
{var:3}
<else />
Not zero or one or two.
</if>
    )";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
            Zero!

            Not 0 but 1.

            Two!

            3
    */
}
```

### If Example 2

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"([0,1,2,3])");

    const char *content = R"(
<if case="{var:0} == 0">
Zero!<if case="{var:1} == 1"> and one<if case="{var:2} == 2"> and two!</if></if>
</if>

<if case="({var:3} + 5) == 0">
Zero!
<else />
<if case="{var:3} == 3">{var:3}+5 is 8 not {var:3}</if>
</if>
)";

    std::cout << Template::Render(content, value) << '\n';

    /*
        Output:
        Zero! and one and two!

        3+5 is 8 not 3
    */
}
```

### Template Example 1

```cpp
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

    std::cout << Template::Render(content, value) << '\n';
}
```

### Template Example 2

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"css(
{
    "dir": "ltr",
    "title": "Wishful News",
    "darkmode": 1,
    "body": {
        "bc": "rgb(17, 66, 61)",
        "color": "black",
        "darkmode_color": "white",
        "H1": "Leatest News"
    },
    "news": [
        {
            "name": "Tech",
            "list": [
                {
                    "text": "The new modern operating system CoolOp is \"Cool\".",
                    "date": "1",
                    "options": ""
                },
                {
                    "text": "The Global Internet is now free.",
                    "date": "2",
                    "options": "style=\"color: violet;\""
                },
                {
                    "text": "No more hackers after implementing NO-WAY-CRPT encryption.",
                    "date": "3",
                    "options": ""
                }
            ]
        },
        {
            "name": "Health",
            "list": [
                {
                    "text": "Free medicine are being given away.",
                    "date": "3011-1-28",
                    "options": ""
                },
                {
                    "text": "People are healthier now after using MakeMYMeds devices.",
                    "date": "3011-1-20",
                    "options": ""
                },
                {
                    "text": "TeraMedicine forced to shutdown.",
                    "date": "3011-1-16",
                    "options": "style=\"font-style: italic;\""
                }
            ]
        },
        {
            "name": "Energy",
            "list": [
                {
                    "text": "No more Uranium.",
                    "date": "3011-1-19",
                    "options": "style=\"color: red;\""
                },
                {
                    "text": "The Global Internet is now free.",
                    "date": "3011-1-19",
                    "options": ""
                },
                {
                    "text": "Gasoline cars are now banned.",
                    "date": "3011-1-18",
                    "options": ""
                }
            ]
        }
    ]
}
    )css");

    const char *content = R"HTML(
<!DOCTYPE html>
<html dir="{var:dir}">

<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>{var:title}</title>
    <style>
        body {
            background-color: {var:body[bc]};
            color: {if case="{var:darkmode}" true="{var:body[darkmode_color]}" false="{var:body[color]}"}
        }
    </style>
</head>

<body>
    <h1>{var:body[H1]}</h1>

    <loop set="news" value="value_s">
    <h2>value_s[name]</h2>
    <div>
        <ul>
            <loop set="value_s[list]" value="value">
            <li><span value[options]>value[text]</span> -
                <span>
                    <if case="value[date] == 1">
                    New
                    <elseif case="value[date] == 2" />
                    Yesterday
                    <elseif case="value[date] == 3" />
                    Two days ago
                    <else />
                    value[date]
                    </if>
                </span>
            </li>
            </loop>
        </ul>
    </div>
    </loop>
</body>

</html>
)HTML";

    std::cout << Template::Render(content, value) << '\n';
}
```
