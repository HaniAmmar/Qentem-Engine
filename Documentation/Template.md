# Template.hpp

-   [Variable](#variable)
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
            "subItem1": 10,
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
[
    "a",
    "b",
    1,
    [
        "x",
        [
            true
        ]
    ]
]
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
            "name": "Qentem",
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

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            Qentem, 3
    */
}
```

### Variable Note

Spaces after `{var:` or before `}` are considered part of the variable's name.

## Math

```txt
{math:...}
```

This tag passes its value to [ALE](https://github.com/HaniAmmar/Qentem-Engine/blob/master/Documentation/ALE.md) for arithmetic and logic evaluation.

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;
    value["Equation"] = "1+4*2+1";
    value["one"]      = "1";
    value["three"]    = "3";

    const char *content = R"(
0.1+0.2 is: {math: 0.1  +   0.2 }
{var:Equation} = {math:{var:Equation}}; (1+8+1)
6^2 = {math:6^2}
--1 = {math:--1}
{var:one}+{var:three} = {math:{var:one}+{var:three}}
9 % 5 = {math:9 % 5}
)";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

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

Inline condition. [ALE](https://github.com/HaniAmmar/Qentem-Engine/blob/master/Documentation/ALE.md) takes the `case` value to evaluate it, and if the returned value is a positive number, it will print the value inside `true`. Otherwise, `false''s value is used.

```txt
{if case="1" true="one" false="not one"} // Fine.
{if case="2!=1" false="not one"} // Fine.
{if case="1&&1" true="one"} // Fine.

{if true="one" false="not one"} // Wrong.
{if case="1"} // Wrong.
{if true="one"} // Wrong.
{if false="not one"} // Wrong.
{if true="one" case="1"} // Wrong, 'case' has to be before "true" or "false".
```

With variables:

```json
[
    "Correct!",
    "WRONG!",
    4,
    5,
    9,
]
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

### ALE Note
ALE treats anything inside parentheses `( )`; true is one, false and null is 0. if the operation is equal, Template will try the following:
-   If one of the variables is a number, it will convert the other part to a number: ({var:one} == 1)
-   If none of the variables is a number, it will convert the other part to a string: ({var:bool} == true)

To force ALE to treat the variables as numbers, use parentheses: (({var:bool}) == 1)

## Loop

```txt
<loop set="..." key="..." value="..." index="...">...</loop>
<loop set="..." key="..." value="...">...</loop>
<loop set="..." value="...">...</loop>
<loop set="..." key="...">...</loop>

<loop value="...">...</loop>
<loop key="..." times="...">...</loop>
<loop key="...">...</loop>
```

Loops over a set and replaces the values with the string inside `value` and keys with `key`. The size can be set using `times` option. and the index can start from the value of `index`. If the array an ordered array, numeric keys will be places as keys. The options `times` and `index` can be numbers or a variable tag. the `set` works like `{var:...}` and can be used to point to a sub-array. `key` and `value` accept only strings; for matching and replacing of the template inside the loop.

### Loop Example 1 (unordered set)

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = Qentem::JSON::Parse(R"(
{
    "a": 1,
    "b": 2,
    "c": 3
}
    )");

    const char *content = R"(
<loop key="loopID1" value="loopVAL1">
loopID1: loopVAL1</loop>
    )";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

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

    value["d"]["a"] = 10;
    value["d"]["b"] = 20;
    value["d"]["c"] = 30;

    const char *content = R"(
<loop set="d" key="loopID1" value="loopVAL1">
loopID1: loopVAL1</loop>
    )";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            a: 10
            b: 20
            c: 30
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
<loop key="loop1-id" value="loop1-value">
[loop1-id]: loop1-value</loop>
    )";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
        [0]: 40
        [1]: 60
        [2]: 80
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
```

### Loop Example 5 (times)

```cpp
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

    value[0] = 5;

    const char *content =
        R"(<loop times="{var:0}" index="5" key="loop1-id">loop1-id</loop>)";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            56789
    */

    value[1] = 3;

    content =
        R"(<loop times="{var:0}" index="{var:1}" key="loop1-id">loop1-id</loop>)";

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

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

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

    /*
        Output:
            value1 value2 value3 value4
            value11 value22 value33 value44


            value10 value20 value30 value40
            value100 value200 value300 value400
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

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

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

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';

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
{
    "major": {
        "Computer Science": [
            { "Name": "Oliver", "GPA": 3.2 },
            { "Name": "Jonah", "GPA": 3.8 },
            { "Name": "Ava", "GPA": 2.8 }
        ],
        "Math": [
            { "Name": "Maxim", "GPA": 3.0 },
            { "Name": "Cole", "GPA": 2.5 },
            { "Name": "Claire", "GPA": 2.4 }
        ]
    }
}
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
        <loop set="major" key="loop-major-key">
            <h3>Major: loop-major-key</h3>
            <ul>
            <loop set="major[loop-major-key]" value="student_val">
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

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';
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
    "news": {
        "Tech": [
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
        ],
        "Health": [
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
        ],
        "Energy": [
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

    <loop set="news" key="NewsID">
    <h2>NewsID</h2>
    <div>
        <ul>
            <loop set="news[NewsID]" value="news_val">
            <li><span news_val[options]>news_val[text]</span> -
            <span>
                <if case="news_val[date] == 1">
                New
                <elseif case="news_val[date] == 2" />
                Yesterday
                <elseif case="news_val[date] == 3" />
                Two days ago
                <else />
                news_val[date]
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

    std::cout << Template::Render(content, &value).GetString().Storage()
              << '\n';
}
```
