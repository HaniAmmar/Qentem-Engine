[![Build status](https://ci.appveyor.com/api/projects/status/08uk7dxqw3vn3usi/branch/main?svg=true)](https://ci.appveyor.com/project/HaniAmmar/qentem-engine/branch/main)

# Qentem Engine

-   [Introduction](#introduction)
-   [Features](#features)
-   [Requirements](#requirements)
-   [Documentation](#documentation)
-   [Live Testing](#live-testing)
-   [Template Example](#template-example)
-   [Modules](#modules)
-   [JSON Example](#json-example)
-   [Tests](#tests)
-   [License](#license)

## Introduction
Qentem Engine is a C++ library offering high-performance template rendering and a fast, built-in JSON parser. It is lightweight, STL-free, dependency-free, and supports deployment on native backends as well as frontend environments such as web browsers via WebAssembly.

## Features

### General

* Header-only, cross-platform, and fully self-contained — STL-free and with no external dependencies.
* High-performance numeric parsing and formatting without relying on floating-point hardware (FPU-free).
* Supports both 32-bit and 64-bit systems, with little-endian and big-endian architectures.
* Exception-free design — no `try`, `catch`, or `throw`.
* Full Unicode support: UTF-8, UTF-16, and UTF-32.
* Low memory footprint — ideal for resource-constrained environments.

### Template Rendering

* Ultra-fast and lightweight template rendering.
* Safe expression evaluation with automatic HTML escaping (enabled by default).
* Raw variable output (no escaping) when needed.
* Nested loops with support for data sorting and grouping.
* Conditional expressions with support for nesting and inline evaluation.
* Fast and secure arithmetic evaluation.

### JSON

* Fast and efficient parsing and serialization.
* Supports merging, sorting, and grouping of values.

## Requirements

A C++11 or later compiler.

_Note_: `iostream` is currently used in tests.

## Documentation

Usage and examples @ [Documentation](https://github.com/HaniAmmar/Qentem-Engine/tree/main/Documentation).

## Live Testing

Test templates directly in your browser: [JQen Tool Live Demo](https://haniammar.github.io/JQen-Tool).

## Template Example

```cpp
#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;
using Qentem::JSON;

int main() {
    Value<char> value = JSON::Parse(R"(
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
<html>

<body>
    <div>
        <h2>Students' list:</h2>
        <loop value="department_val">
            <h3>Major: {var:department_val[major]}</h3>
            <ul>
            <loop set="department_val[students]" value="student_val">
                <li>
                    <span>Name: {var:student_val[Name]}</span>
                    <span>
                        GPA: {var:student_val[GPA]}
                        <if case="{var:student_val[GPA]} < 2.5"> (Inform adviser!)
                        <else if case="{var:student_val[GPA]} >= 3.5"> (President's List!)
                        <else if case="{var:student_val[GPA]} >= 3.0"> (Dean's List!)
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

_Note_: the template does not have to be HTML.

### Modules

-   JavaScript: [JQen](https://github.com/HaniAmmar/JQen)
-   Python: [PiQen](https://github.com/HaniAmmar/PiQen)
-   PHP: [BQen](https://github.com/HaniAmmar/BQen)

## JSON Example

```cpp
#include "JSON.hpp"

#include <iostream>

using Value = Qentem::Value<char>;
using Qentem::JSON;

int main() {
    Value abc = JSON::Parse(R"(["A","B","C"])");

    Value v_arr; // To be used as an array.

    v_arr += 0;
    v_arr[1] = 10;
    v_arr += nullptr;
    v_arr += true;
    v_arr += false;

    v_arr[0] = "text"; // Overwrite

    v_arr += abc; // Add sub-array as it is.

    // Unpack all the values and merge them into v_arr.
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

    // Replace any value with the same key, or add it if it does not exist.
    v_obj += JSON::Parse(R"({"key0": "text", "key4": true, "key5": 500, "key7": [1,2,3,4], "key8": null})");

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
        R"([
            {"year":2019,"month":4},
            {"year":2020,"month":5},
            {"year":2017,"month":1},
            {"year":2020,"month":6},
            {"year":2018,"month":2},
            {"year":2020,"month":7},
            {"year":2018,"month":3}])");

    Value v_arr2;
    if (v_arr.GroupBy(v_arr2, "year")) {
        v_arr2.Sort();
        std::cout << v_arr2.Stringify() << '\n';
    }

    /* Output:
    {
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
        ],
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
        ]
    }
    */
}
```

## Tests

The test suite includes over 23k lines of code, more than five times the size of the library itself, and completes in less than 0.01 seconds.

-   gcc/clang

    ```shell
    mkdir Build
    c++ -I ./Include ./Tests/Test.cpp -o ./Build/QTest.bin
    ./Build/QTest.bin
    ```

-   make
    ```shell
    make test
    ```

-   cmake
    ```shell
    mkdir Build
    cd Build
    cmake ..
    cmake --build .
    ctest -C Debug
    ```

## License

> MIT License
>
> Copyright (c) 2025 Hani Ammar
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
