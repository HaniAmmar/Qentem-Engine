[![Build Status](https://travis-ci.org/HaniAmmar/Qentem-Engine.svg?branch=master)](https://travis-ci.org/HaniAmmar/Qentem-Engine)
[![Build status](https://ci.appveyor.com/api/projects/status/08uk7dxqw3vn3usi/branch/master?svg=true)](https://ci.appveyor.com/project/HaniAmmar/qentem-engine/branch/master)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4f7b5b192150491b8a1062465d15de51)](https://www.codacy.com/manual/HaniAmmar/Qentem-Engine?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=HaniAmmar/Qentem-Engine&amp;utm_campaign=Badge_Grade)
[![codecov](https://codecov.io/gh/HaniAmmar/Qentem-Engine/branch/master/graph/badge.svg)](https://codecov.io/gh/HaniAmmar/Qentem-Engine)

# Qentem Engine

-   [Introduction](#introduction)
-   [Features](#features)
-   [Requirements](#requirements)
-   [Documentation](#documentation)
-   [Components](#requirements)
    -   [JSON](#json)
    -   [Template](#template)
        -   [PHP Module](#php-module)
        -   [JavaScript Module](#javascript-module)
    -   [ALE](#ale)
-   [Tests](#tests)
-   [License](#license)

## Introduction

Qentem Engine is an independent and cross-platform library that uses a fast algorithm for nest-matching. It has a JSON parser, template rendering engine, and just-in-time arithmetic and logic evaluation.

## Features

-   Header only and self-contained; no external libraries like the STL is needed.
-   Uses SSE2, AVX2, and AVX512BW when available.
-   Supports UTF-16 and UTF-32 surrogates.
-   Customizable template patterns with (variable replacement, nesting loops, nesting if conditions and Inline if)

## Requirements

C++ 11+ compiler.

## Documentation

Usage and examples @ [Documentation](https://github.com/HaniAmmar/Qentem-Engine/tree/master/Documentation).

## Components

### JSON

Value is a first-class type in Qentem. It's being used by the template engine for variables' replacement. It has an operator that can access values by its index and/or by its key value in case of objects. The parser is fast and very strict; a single mistake in the JSON string will give an empty value. Stringify gives a UTF-8 c-string.

#### JSON Example

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

    v_arr[5] = abc;                           // Add sub-array
    v_arr += JSON::Parse(R"([100,200,300])"); // Addition

    std::cout << v_arr.Stringify().Storage() << '\n';
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
    v_obj += JSON::Parse(
        R"({"key0": "text", "key4": true, "key5": 500, "key7": [1,2,3,4], "key8": null})");

    std::cout << v_obj.Stringify().Storage() << '\n';
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
}
```

### Template

Rendering templates is the main reason for making Qentem, and it uses every part of the library. It is fast and HTML friendly. It has loops/nesting loops, if/nesting-if with else and else-if, inline if, variable replacement, and math evaluation.

#### Template Example

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

#### Template Examples

More examples are inside [Examples/Template](https://github.com/HaniAmmar/Qentem-Engine/tree/master/Examples/Template).

#### PHP Module

Templating module for PHP: [BQen](https://github.com/HaniAmmar/BQen)

#### JavaScript Module

Templating module for JavaScript: [JQen](https://github.com/HaniAmmar/JQen)

### ALE

Arithmetic and logic evaluator for evaluating if conditions and calculation math values for the template engine. It has support for nesting parenthesis `(..)`, and made to be pluggable for numbers' replacement when curly brackets `{..}` are used.

#### ALE Example

```cpp
#include "ALE.hpp"

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

    for (Qentem::ULong i = 0; i < math.Size(); i++) {
        std::cout << Qentem::ALE::Evaluate(math[i]) << '\n';
    }
}
```

#### ALE Examples

More examples are inside [Examples/ALE](https://github.com/HaniAmmar/Qentem-Engine/tree/master/Examples/ALE).

## Tests

The tests are ~13k lines of code; ~4 times the size of the library. To run the tests:

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
        cmake --config "Debug" ..
        cmake --build .
        ctest -C Debug
        ```

-   gcc/clang
    ```shell
    mkdir Build
    c++ -std=c++11 -I ./Include ./Tests/Test.cpp -o ./Build/QTest.bin
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
