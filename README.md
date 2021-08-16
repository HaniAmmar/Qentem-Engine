[![Build Status](https://travis-ci.com/HaniAmmar/Qentem-Engine.svg?branch=master)](https://travis-ci.com/HaniAmmar/Qentem-Engine)
[![Build status](https://ci.appveyor.com/api/projects/status/08uk7dxqw3vn3usi/branch/master?svg=true)](https://ci.appveyor.com/project/HaniAmmar/qentem-engine/branch/master)
[![codecov](https://codecov.io/gh/HaniAmmar/Qentem-Engine/branch/master/graph/badge.svg)](https://codecov.io/gh/HaniAmmar/Qentem-Engine)

# Qentem Engine

-   [Introduction](#introduction)
-   [Features](#features)
-   [Requirements](#requirements)
-   [Documentation](#documentation)
-   [JSON Example](#json-example)
-   [Template Example](#template-example)
    -   [PHP Module](#php-module)
    -   [JavaScript Module](#javascript-module)
-   [ALE Example](#ale-example)
-   [Tests](#tests)
-   [License](#license)

## Introduction

Qentem Engine is a self-contained library for rendering HTML templates. Designed to be a server-side module or a client one `(WASM)`. It can also be used to render HTML pages in a normal Application. On PHP, it uses `z_val` for variable replacement. However, it evaluates if statements using a custom arithmetic and logic algorithm; for security and performance reasons. The client-side module – WASM has a compact `Value` container and a fast JSON parser.

## Features
-   General
    -   Cross platform, header only and self-contained; no external libraries is needed.
    -   Supports UTF-8, UTF-16 and UTF-32.
    -   Supports 32-bit and 64-bit architecture, little and big endian.
    -   Low memory usage.

-   JSON
    -   Fast parser.
    -   Fast stringify.
    -   Data Grouping.
    -   Data Sorting.
    -   `Value` size: 16 bytes on 64-bit, and 12 on 32-bit.

-   Tempate engine
    -   Fast template rendering.
    -   Safe evaluation.
    -   Variable replacement with auto-escaping.
    -   Raw variable replacement **without** auto-escaping.
    -   Nested loop (with data grouping and sorting).
    -   Nested if condition.
    -   Inline if.
    -   Math tag.

## Requirements

C++ 11+ compiler.

## Documentation

Usage and examples @ [Documentation](https://github.com/HaniAmmar/Qentem-Engine/tree/master/Documentation).

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

    std::cout << Template::Render(content, &value).GetString() << '\n';
}
```

### PHP Module

PHP module is @ [BQen](https://github.com/HaniAmmar/BQen)

### JavaScript Module

JavaScript module is @ [JQen](https://github.com/HaniAmmar/JQen)

## ALE Example

```cpp
#include "ALE.hpp"
#include "Array.hpp"

#include <iostream>

int main() {
    Qentem::Array<const char *> math;

    math += "+1+1";                         // 2
    math += "---1";                         // -1
    math += "-1+3";                         // 2
    math += "-1*8";                         // -8
    math += "--1*5";                        // 5
    math += "1--1";                         // 2
    math += "11==11";                       // 1
    math += "11!=11";                       // 0
    math += "11>2";                         // 1
    math += "11>=2";                        // 1
    math += "1<2";                          // 1
    math += "2^8";                          // 256
    math += "11<=11";                       // 1
    math += "11&&2";                        // 1
    math += "4*2+6/3";                      // 10
    math += "(2+3)*5";                      // 25
    math += "(5)+3";                        // 8
    math += "((1+2)^(1+2))/2";              // 13.5
    math += "((5/5+1)*2+1)+3*3";            // 14
    math += "5+2*4-8/2==9 && 1";            // 1
    math += "((5/5+1)*2+1)+3*3 != 12 && 1"; // 1
    math +=
        R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) *
        (8 / 4 + 1) - 1 - -1 + 2 == ((5/5+1)*2+1)+3*3)"; // 1
    ///////////////////////////////////////////////////

    for (unsigned int i = 0; i < math.Size(); i++) {
        std::cout << Qentem::ALE::Evaluate(math[i]) << '\n';
    }
}
```

## Tests

The tests are ~15k lines of code; ~5 times the size of the library. To run the tests:

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
