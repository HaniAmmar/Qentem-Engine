# Template.hpp

-   [Variable](#variable)
-   [Raw Variable](#raw-variable)
-   [Math](#math)
-   [Super Variable](#super-variable)
-   [Inline If](#inline-if)
-   [If Condition](#if-condition)
-   [Loop](#loop)

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

In the case of ordered arrays, use a numeric id.

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

### Variable Example

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["name"]       = "Qentem";
    value["version"]    = 3.0;
    const char *content = R"({var:name}, {var:version})";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            Qentem, 3
    */
}
```

### Variable Tag Note

Spaces after `{var:` or before `}` are considered part of the variable's name, and any string value will be made safe to be printed to the browser; by escaping HTML special chars (>, <, ", ' , &).

## Raw Variable
```txt
{raw:name}
{raw:version}
```
Raw variable tag is the same as Variable tag, except it does not escape HTML special chars (>, <, ", ' , &).

### Raw Variable Example

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["name"]       = "<Qentem>";
    value["version"]    = 3.0;
    const char *content = R"({raw:name}, {raw:version})";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            <Qentem>, 3
    */
}

## Math

```txt
{math:...}
```

-   Logic operations:

    -   && `And`
    -   || `Or`
    -   == `Equal`
    -   != `Not equal`
    -   \> `Greater than`
    -   \>= `Greater than or equal`
    -   < `Less than`
    -   <= `Less than or equal`

-   Arithmetic operations:

    -   ^ `Exponent`
    -   % `Remainder`
    -   \* `Multiplication`
    -   / `Division`
    -   \+ `Addition`
    -   \- `Subtraction`

### Math Example

```cpp
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

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            0.1+0.2 is: 0.3
            1+4*2+1 = 10; (1+8+1)
            6^2 = 36
            1+3 = 4
            9 % 5 = 4
    */
}
```

## Super Variable

```txt
{svar:var, sub_var_0, sub_var_1, sub_var_2, ... sub_var_9}
```

Super variable tag is like a tiny template engine engineered for basic text replacement inside a phrase; it can accept a variable tag, a raw variable tag, and a math tag. The maximum number of sub-variables is 10, starting at 0 and ending at 9.

```txt
svar_value: when {0} met with {1}, he saw {2}, {3}, {4}, {5}, {6}, {7} and {8} but not {9}.
```

The order of values does not matter, and every value can be reused at any time.

```txt
svar_value: when {5} met with {3}, he saw {2}, {1}, {4}, {0}, {6}, {7} and {8} but not {9}.
{3} and {5} were at the market when they met.
```

### Super Variable Example

```cpp
#include "Template.hpp"
#include "JSON.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

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
```

## Inline If

-   Logic operations:

    -   && `And`
    -   || `Or`
    -   == `Equal`
    -   != `Not equal`
    -   \> `Greater than`
    -   \>= `Greater than or equal`
    -   < `Less than`
    -   <= `Less than or equal`

-   Arithmetic operations:

    -   ^ `Exponent`
    -   % `Remainder`
    -   \* `Multiplication`
    -   / `Division`
    -   \+ `Addition`
    -   \- `Subtraction`

```txt
{if case="..." true="..." false="..."}
```

Inline if condition uses the `case` value for evaluation, and if the returned value is a positive number, it will print the value inside `true`. Otherwise, `false`'s value is used.

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

## If Condition

-   Logic operations:

    -   && `And`
    -   || `Or`
    -   == `Equal`
    -   != `Not equal`
    -   \> `Greater than`
    -   \>= `Greater than or equal`
    -   < `Less than`
    -   <= `Less than or equal`

-   Arithmetic operations:

    -   ^ `Exponent`
    -   % `Remainder`
    -   \* `Multiplication`
    -   / `Division`
    -   \+ `Addition`
    -   \- `Subtraction`

```txt
<if case="...">...</if>
<if case="...">...<else />...</if>
<if case="...">...<elseif case="..." />...</if>
<if case="...">...<elseif case="..." />...<else />...</if>
```

Similar to `{if case="..." ...}`, but capable of branching and nesting.

### If Example 1

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
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

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

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

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;
using Qentem::JSON;

int main() {
    auto value = JSON::Parse(R"([0,1,2,3])");

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

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
        Zero! and one and two!

        3+5 is 8 not 3
    */
}
```

### Evaluation Note

Evaluate uses 1 for `true`, 0 for `false`. if the operation is equal `==`, Template will try the following:

-   If one of the variables is a number, it will try to convert the other one to a number: ({var:one} == 1)
-   If none of the variables are numbers, it will try to convert them to strings: ({var:bool} == true)
-   However, it's better to use bool alone as it will be converted to number. `{var:bool}` is the same as `({var:bool}) == 1` and `({var:bool} == true)` but `{var:bool}` alone is faster and less writing: `{if case="{var:bool1}" true="bool1 is true" false="bool1 is false"}`.
-   If `case` contains a variable only and the variable is string with length bigger than zero, the value inside `true` will be printed, otherwise, `false` content will be printed.

```txt
{if case="{var:string1}" true="string1 is not empty" false="null, empty or not a string"}
```

To force converting the variables to numbers, use parentheses: {math:({var:bool}) == 1}

### Evaluation Order

-   Parentheses.
-   Exponent. Remainder.
-   Multiplication. Division.
-   Addition. Subtraction.
-   Equal. Not Equal. Less than. Greater than.
-   And. Or.

## Loop

```txt
<loop set="..." value="..." group="..." sort="...">...</loop>
<loop value="...">...</loop>
<loop set="..." value="...">...</loop>
<loop set="..." value="..." group="...">...</loop>
<loop set="..." value="..." sort="...">...</loop>
<loop set="..." value="..." group="..." sort="...">...</loop>
```

Loops over a set and replaces the values with the string inside `value`. the `set` works like `{var:...}` and can be used to point to a sub-array. `value` accepts only strings for matching and replacing the values inside the loop. The `group` option groups the given array by a sub-value of an object. `sort` will sort the set in ascending or descending order.

### Loop Example 1 (unordered set)

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;
using Qentem::JSON;

int main() {
    auto value = JSON::Parse(R"(
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
{var:loop1-value[name]}: {var:loop1-value[value]}</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

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

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value["some_set"]["a"] = 10;
    value["some_set"]["b"] = 20;
    value["some_set"]["c"] = 30;

    const char *content = R"(
<loop set="some_set" value="loop1-value">
{var:loop1-value}</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

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

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value[0] = 40;
    value[1] = 60;
    value[2] = 80;

    const char *content = R"(
<loop value="loop1-value">
{var:loop1-value}</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

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

using Qentem::StringStream;
using Qentem::Template;
using Qentem::JSON;
using Qentem::Value;

int main() {
    auto value = JSON::Parse(R"(
{
    "Group-1": [["A","B","C"],["DD","EE","FF"],["GGG","HHH","Qentem"]],
    "Group-2": [[1,2,3],[10,20,30],[100,200,300]]
}
    )");

    const char *content = R"(
<loop value="loop1-val">
    <loop set="loop1-val" value="loop2-val">
        <loop set="loop2-val" value="loop3-val">{var:loop3-val}
        </loop>
    </loop>
</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

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

### Loop Example 5

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    value += 0;
    value += 1;

    const char *content =
        R"(<loop value="loop1-value"><loop value="loop2-value"><loop value="loop3-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}) </loop></loop></loop>)";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
                (0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1)
    */
}
```

### Loop Example 6

```cpp
#include "Template.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    Value<char> value;

    for (int i = 0; i < 10; i++) {
        value["list"] += i;
    }

    const char *content = R"(<loop set="list" value="loop1-value">{var:loop1-value}</loop>)";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

    /*
        Output:
            0123456789
    */
}
```

### Loop Example 7 (sub value)

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;
using Qentem::JSON;

int main() {
    auto value = JSON::Parse(R"(
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
{var:item[var1]} {var:item[var2]} {var:item[var3]} {var:item[var4]}</loop>

<loop set="array" value="item">
{var:item[0]} {var:item[1]} {var:item[2]} {var:item[3]}</loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';

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

using Qentem::StringStream;
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
<loop value="val_1" sort="ascend">{var:val_1} </loop>
    )";

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
    /*
        Output: 1 2 3 4 5 6 7
    */

    const char *content2 = R"( = R"(
<loop value="val_1" sort="descend">{var:val_1} </loop>
    )";

    stream.Clear();

    Template::Render(content2, value, stream);
    std::cout << stream << '\n';
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

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;
using Qentem::JSON;

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

    std::cout << Template::Render<StringStream<char>>(content2, value) << '\n';
}
```

### Template Example 1

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;
using Qentem::JSON;

int main() {
    auto value = JSON::Parse(R"(
[
    {
        "major": "Computer Science",
        "students": [
            { "Name": "Student1", "GPA": 3.2 },
            { "Name": "Student2", "GPA": 3.8 },
            { "Name": "Student3", "GPA": 2.8 }
        ]
    },
    {
        "major": "Math",
        "students": [
            { "Name": "Student4", "GPA": 3.0 },
            { "Name": "Student5", "GPA": 2.5 },
            { "Name": "Student6", "GPA": 2.4 }
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

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
}
```

### Template Example 2

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;
using Qentem::JSON;

int main() {
    auto value = JSON::Parse(R"data(
{
    "dir": "ltr",
    "title": "Wishful News",
    "dark_mode": 1,
    "body": {
        "bc": "rgb(17, 66, 61)",
        "color": "black",
        "dark_mode_color": "white",
        "H1": "Latest News"
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
    )data");

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
            color: {if case="{var:dark_mode}" true="{var:body[dark_mode_color]}" false="{var:body[color]}"}
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

    StringStream<char> stream;

    Template::Render(content, value, stream);
    std::cout << stream << '\n';
}
```
