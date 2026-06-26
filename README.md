[![Build status](https://ci.appveyor.com/api/projects/status/08uk7dxqw3vn3usi/branch/main?svg=true)](https://ci.appveyor.com/project/HaniAmmar/qentem-engine/branch/main)

<p align="center">
  <img src="./icon.png" width="335" height="336"/>
</p>

# Qentem Engine

-   [Introduction](#introduction)
-   [Features](#features)
-   [Requirements](#requirements)
-   [Embedded / Microcontroller Support](#embedded--microcontroller-support)
-   [Memory Model](#memory-model)
-   [Documentation](#documentation)
-   [Live Testing](#live-testing)
-   [Template Example](#template-example)
-   [Modules](#modules)
-   [JSON Example](#json-example)
-   [Tests](#tests)
-   [Deprecation Notice (Windows)](#deprecation-notice-windows)
-   [License](#license)

## Introduction

Qentem Engine is a lightweight, high-performance C++ library built around custom memory management, containers, Unicode string processing, numeric utilities, template rendering, and JSON parsing. It is STL-free, dependency-free, and designed for use across native backends, embedded systems, and frontend environments through WebAssembly.

## Features

### Core Runtime

* Header-only, cross-platform, and fully self-contained. No STL, no external dependencies.
* Custom memory reservation system (`Reserver`) for predictable, low-overhead dynamic allocation.
* Exception-free design with deterministic control flow.
* No global mutable state; safe for isolated multi-threaded use with separate objects.
* Supports both little-endian and big-endian architectures.
* Optional libc-free Linux execution path through direct system call abstractions.
* Low memory footprint.

### Numeric System

* High-performance integer and floating-point parsing (FPU-free).
* Fast floating-point to string conversion.
* Arbitrary-precision integer support (BigInt).
* Efficient numeric formatting utilities.

### Containers

* Custom hash table implementation designed for low allocation overhead.
* Lightweight arrays, lists, and deque structures.
* STL-free container ecosystem.
* Cache-friendly data layouts for performance-sensitive workloads.

### Strings & Unicode

* Native UTF-8, UTF-16, and UTF-32 support.
* String views, mutable strings, and stream builders.
* Fast Unicode-aware string manipulation.
* Efficient encoding conversion utilities.

### JSON

* Fast JSON parsing and serialization.
* Value merging, sorting, and grouping.
* Optional C-style comment support (`//`, `/* */`).

### Template Rendering

* Ultra-fast template rendering engine.
* Safe expression evaluation with automatic HTML escaping.
* Raw output support when escaping is not desired.
* Nested loops with sorting and grouping support.
* Conditional and inline expression evaluation.
* Built-in arithmetic expression engine.

## Requirements

A C++17 or later compiler.

## Embedded / Microcontroller Support

Qentem Engine’s modular, STL-free design makes it well-suited for embedded systems and microcontrollers with limited memory and no operating system. Core components, including template rendering, JSON parsing, custom containers, and the internal memory system, have been validated on platforms such as the ESP32 and can operate in bare-metal or Arduino-style C++ environments.

The engine is built without exceptions, threading primitives, or OS-specific dependencies, making it suitable for single-threaded, memory-constrained targets where deterministic behavior and low overhead are essential.

Memory usage can be tuned at compile time through `QENTEM_RESERVER_BLOCK_SIZE`, which defines the block size used by the internal allocator (`Reserver`). This allows applications to adapt the engine to tight memory budgets while preserving predictable allocation behavior.

> _Note_: Qentem Engine does not provide file or stream I/O abstractions for embedded targets. Platform-specific I/O integration is expected to be supplied by the host application.

## Memory Model

Qentem uses a custom memory reservation system (`Reserver`) designed for predictable, low-overhead allocation and long-term memory reuse.

### Key characteristics:

* Block-based reservation model with no allocation metadata stored inside user memory.
* First-fit region selection with bitfield tracking for fast reuse.
* Fixed-size preallocated blocks to reduce fragmentation.
* Optional memory diagnostics via `MemoryRecord`.

### Threading model:

Reserver is intentionally designed for core-pinned, single-threaded workloads and does not currently provide internal synchronization. This model enables predictable allocation behavior and naturally extends to per-core allocator designs.

## Documentation

Usage and examples @ [Documentation](https://github.com/HaniAmmar/Qentem-Engine/tree/main/Documentation).

## Live Testing

Test templates directly in your browser: [JQen Tool Live Demo](https://haniammar.github.io/JQen-Tool).

## Template Example

```cpp
#include "Qentem/JSON.hpp"
#include "Qentem/Template.hpp"

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
#include "Qentem/JSON.hpp"

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

    std::cout << v_arr << '\n';
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

    std::cout << v_obj << '\n';
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

    std::cout << v_arr << '\n';
    // Output: [1,2,3,4,5,6,7]

    v_arr.Sort(false); // Descending

    std::cout << v_arr << '\n';
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
        std::cout << v_arr2 << '\n';
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


## Deprecation Notice (Windows)

Windows support is deprecated and will be discontinued in a future release. Future development will target POSIX-compliant systems.

## License

MIT License. See the [LICENSE](LICENSE) file for details.
