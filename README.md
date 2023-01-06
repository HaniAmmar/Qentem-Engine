[![Build Status](https://app.travis-ci.com/HaniAmmar/Qentem-Engine.svg?branch=main)](https://app.travis-ci.com/HaniAmmar/Qentem-Engine)
[![Build status](https://ci.appveyor.com/api/projects/status/08uk7dxqw3vn3usi/branch/main?svg=true)](https://ci.appveyor.com/project/HaniAmmar/qentem-engine/branch/main)
[![codecov](https://codecov.io/gh/HaniAmmar/Qentem-Engine/branch/main/graph/badge.svg)](https://codecov.io/gh/HaniAmmar/Qentem-Engine)

# Qentem Engine

-   [Introduction](#introduction)
-   [Features](#features)
-   [Requirements](#requirements)
-   [Documentation](#documentation)
-   [Live Testing](#live-testing)
-   [Template Example](#template-example)
    -   [JavaScript Module](#javascript-module)
    -   [Python Module](#python-module)
    -   [PHP Module](#php-module)
-   [JSON Example](#json-example)
-   [ALE Example](#ale-example)
-   [Tests](#tests)
-   [License](#license)

## Introduction

Qentem Engine is a self-contained library for rendering HTML templates, designed to work in the back-end or the font-end, and because it has no specific requirement and easy to use, it can be includes in any C++ or C project, or even JAVA. There are a few modules made for PHP, Python and Javascript, in case rendering HTML in C++ is not the norm. The library has a built-in fast JSON parser, and has its own logic and arithmetic evaluator which is responsable for evaluating `if`, or calculating long algorithms. In addition to all of that, it can be compiled to `WASM`, and tested directly in the browser, with all of its features like: nested loops, nested if conditions, inline if and variable replacement with auto-escape for HTML special characters.

## Features

-   General

    -   Cross-platform, header-only and self-contained; no external libraries are needed.
    -   Supports UTF-8, UTF-16 and UTF-32.
    -   Supports 32-bit and 64-bit architecture, little and big-endian.
    -   Low memory usage.
    -   No exceptions (try, catch and throw).
    -   Short string optimization.

-   JSON

    -   Fast parser.
    -   Fast stringify.
    -   Data grouping.
    -   Data sorting.
    -   `Value` size: 16 bytes on 64-bit, and 12 on 32-bit.

-   Template engine
    -   Fast template rendering.
    -   Safe evaluation.
    -   Variable replacement with HTML auto-escape (Optional: on by default).
    -   Raw variable replacement **without** HTML auto-escape.
    -   Nested loops (with data grouping and sorting).
    -   Nested if conditions.
    -   Inline if.
    -   Math tag.

## Requirements

C++ 11+ compiler.

## Documentation

Usage and examples @ [Documentation](https://github.com/HaniAmmar/Qentem-Engine/tree/main/Documentation).

## Live Testing

Templates can be tested live @ [JQen Tool](https://haniammar.github.io/JQen-Tool)

## Template Example

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

### JavaScript Module

JavaScript module: [JQen](https://github.com/HaniAmmar/JQen)

### Python Module

Python module: [PiQen](https://github.com/HaniAmmar/PiQen)

### PHP Module

PHP module: [BQen](https://github.com/HaniAmmar/BQen)

## JSON Example

```cpp
#include "JSON.hpp"

#include <iostream>

using Value    = Qentem::Value<char>;
namespace JSON = Qentem::JSON;

int main() {
 Value abc = JSON::Parse(R"(["A","B","C"])");

    Value v_arr; // To be used as an array.

    v_arr += 0;
    v_arr[1] = 10;
    v_arr += nullptr;
    v_arr += true;
    v_arr += false;

    v_arr[0] = "text"; // Overwrite

    v_arr += abc; // Add sub-array
    v_arr.Merge(JSON::Parse(R"([100,200,300])"));

    std::cout << v_arr.Stringify() << '\n';
    /* Output:
        [
            "text",
            10,
            null,
            true,
            false,
            [
                "A",
                "B",
                "C"
            ],
            100,
            200,
            300
        ]
    */

    ///////////////////////////////////////////

    Value v_obj; // To be used as an object.

    v_obj["key0"] = 0;
    v_obj["key1"] = 10;
    v_obj["key2"] = nullptr;
    v_obj["key3"] = true;
    v_obj["key4"] = false;
    v_obj["key5"] = nullptr;

    v_obj["key6"] = abc; // Add sub-array

    // Add the value if the key does not exist, or replace it otherwise.
    v_obj.Merge(JSON::Parse(
        R"({"key0": "text", "key4": true, "key5": 500, "key7": [1,2,3,4], "key8": null})"));

    std::cout << v_obj.Stringify() << '\n';
    /* Output:
       {
            "key0": "text",
            "key1": 10,
            "key2": null,
            "key3": true,
            "key4": true,
            "key5": 500,
            "key6": [
                "A",
                "B",
                "C"
            ],
            "key7": [
                1,
                2,
                3,
                4
            ],
            "key8": null
        }
    */

    ///////////////////////////////////////////

    // Sorting

    v_arr.Reset();

    v_arr += 4;
    v_arr += 1;
    v_arr += 3;
    v_arr += 5;
    v_arr += 2;
    v_arr += 7;
    v_arr += 6;

    v_arr.Sort(); // Ascending

    std::cout << v_arr.Stringify() << '\n';
    // Output: [1,2,3,4,5,6,7]

    v_arr.Sort(false); // Descending

    std::cout << v_arr.Stringify() << '\n';
    // Output: [7,6,5,4,3,2,1]

    ///////////////////////////////////////////

    // Grouping

    v_arr = JSON::Parse(
        R"([{"year":2019,"month":4},{"year":2020,"month":5},{"year":2017,"month":1},{"year":2020,"month":6},{"year":2018,"month":2},{"year":2020,"month":7},{"year":2018,"month":3}])");

    Value v_arr2;
    if (v_arr.GroupBy(v_arr2, "year")) {
        std::cout << v_arr2.Stringify() << '\n';
    }

    /* Output:
     {
         "2019": [
             {
                 "month": 4
             }
         ],
         "2020": [
             {
                 "month": 5
             },
             {
                 "month": 6
             },
             {
                 "month": 7
             }
         ],
         "2017": [
             {
                 "month": 1
             }
         ],
         "2018": [
             {
                 "month": 2
             },
             {
                 "month": 3
             }
         ]
     }
 */
}
```

## ALE Example

```cpp
#include "ALE.hpp"
#include "Array.hpp"

#include <iostream>

int main() {
    Qentem::Array<const char *> equations;

    equations += "+1+1";                         // 2
    equations += "---1";                         // -1
    equations += "-1+3";                         // 2
    equations += "-1*8";                         // -8
    equations += "--1*5";                        // 5
    equations += "1--1";                         // 2
    equations += "11==11";                       // 1
    equations += "11!=11";                       // 0
    equations += "11>2";                         // 1
    equations += "11>=2";                        // 1
    equations += "1<2";                          // 1
    equations += "2^8";                          // 256
    equations += "11<=11";                       // 1
    equations += "11&&2";                        // 1
    equations += "4*2+6/3";                      // 10
    equations += "(2+3)*5";                      // 25
    equations += "(5)+3";                        // 8
    equations += "((1+2)^(1+2))/2";              // 13.5
    equations += "((5/5+1)*2+1)+3*3";            // 14
    equations += "5+2*4-8/2==9 && 1";            // 1
    equations += "((5/5+1)*2+1)+3*3 != 12 && 1"; // 1
    equations +=
        R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) *
        (8 / 4 + 1) - 1 - -1 + 2 == ((5/5+1)*2+1)+3*3)"; // 1
    ///////////////////////////////////////////////////

    for (const auto &eq : equations) { {
        std::cout << Qentem::ALE::Evaluate(eq) << '\n';
    }
}
```

## Tests

There are over 15k lines of code for testing; 5 times the size of the library, and some tests can be used as examples. To run the tests:

-   cmake

    -   Linux
        ```shell
        mkdir Build
        cd Build
        cmake ..
        cmake --build .
        ctest
        ```
    -   Windows
        ```shell
        mkdir Build
        cd Build
        cmake -D CMAKE_BUILD_TYPE=Debug ..
        cmake --build .
        ctest -C Debug
        ```

-   gcc/clang
    ```shell
    mkdir Build
    c++ -I ./Include ./Tests/Test.cpp -o ./Build/QTest.bin
    ./Build/QTest.bin
    ```

## License

> MIT License
>
> Copyright (c) 2020 Hani Ammar
>
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.
