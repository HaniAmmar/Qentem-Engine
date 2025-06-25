#include "JSON.hpp"

#include <iostream>

using namespace Qentem;

/*
mkdir Build
c++ -g ./Examples/JSON/JSON02.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

int main() {
    StringStream<char> ss;

    // Test the first polluted JSON
    ss = R"jsonc(
// Leading comment
/* Block comment before array */
[
    /* Block before first value */
    1, // Inline after value
    2, /* Block after value */
    // Inline before value
    3,
    4 /* Unterminated block,
    // Inline inside block
    still block */ , 5, // After comma
    /* Multiline
       block
       comment */
    "value", // String value
    "//not a comment", /* Block after string */
    "/*not a comment*/",
    "Escaped \\\"// still string", // This is still a string
    "//",
    /* Comment with // and " inside */ 6,
    "C-style /* inside string // with //slashes*/ 7", // confusing!
    8 // Last value
] // Trailing comment
/* Block at end */
)jsonc";

    StringUtils::StripComments(ss.Storage(), ss.Length());
    Value<char> val1 = JSON::Parse(ss.First(), ss.Length());

    std::cout << "Parsed polluted_json_1:\n" << val1.Stringify() << "\n\n";

    /* Output:
        [
            1,
            2,
            3,
            4,
            5,
            "value",
            "\/\/not a comment",
            "\/*not a comment*\/",
            "Escaped \\\"\/\/ still string",
            "\/\/",
            6,
            "C-style \/* inside string \/\/ with \/\/slashes*\/ 7",
            8
        ]
    */

    ////////////////////////////

    // Test the second polluted JSON
    ss = R"jsonc(
// Start
[
    // Nested start [
    1, /* block */ 2, // Inline after block
    "string // not a comment", // Real comment
    "weird /* string // with // multiple slashes*/", /* after string */
    3,//inline
    4, //inline
    /* block
    // inline in block
    */5,
    6,/*block*//*block again*/7,
    "escaped quote: \\\" /* not a comment */", // test escape
    "backslash: \\\\", // ends with \
    "mix: // /* \" \\\" */" // crazy!
]
// end //
/* Block at EOF */
)jsonc";

    StringUtils::StripComments(ss.Storage(), ss.Length());
    Value<char> val2 = JSON::Parse(ss.First(), ss.Length());

    std::cout << "Parsed polluted_json_2:\n" << val2.Stringify() << "\n\n";

    /*
    [ Output:
        1,
        2,
        "string \/\/ not a comment",
        "weird \/* string \/\/ with \/\/ multiple slashes*\/",
        3,
        4,
        5,
        6,
        7,
        "escaped quote: \\\" \/* not a comment *\/",
        "backslash: \\\\",
        "mix: \/\/ \/* \" \\\" *\/"
    ]
    */

    return 0;
}
