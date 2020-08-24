# Arithmetic & Logic Evaluator

## Introduction

ALE made to evaluate `if case="..."` for [Template](https://github.com/HaniAmmar/Qentem-Engine/blob/master/Documentation/Template.md). It returns 1 `true` or 0 `false` if it's a logic evaluation, or the result of arithmetic equation.

-   Logic operations:
    -   &&  `And`
    -   ||  `Or`
    -   ==  `Equal`
    -   !=  `Not equal`
    -   \>  `Bigger than`
    -   \>= `Bigger than or equal`
    -   <   `Less than`
    -   <=  `Less than or equal`
-   Arithmetic operations:
    -   ^   `Exponent`
    -   %   `Remainder`
    -   \*  `Multiplication`
    -   /   `Division`
    -   \+  `Addition`
    -   \-  `Subtraction`

Also: Parentheses `( )` and brackets `{ }`. Parentheses can have any operation, and brackets are used for variables when implementing `ALEHelper`.

## Order
-   Parentheses.
-   Exponent. Remainder.
-   Multiplication. Division.
-   Addition. Subtraction.
-   Equal. Not Equal. Less than. Bigger than.
-   And. Or.

## Example 1

```cpp
#include "ALE.hpp"

#include <iostream>

using Qentem::ALE;
using Qentem::Array;
using Qentem::String;
using Qentem::UInt;
using Qentem::ULong;

int main() {
    Array<const char *> equations;

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

    for (ULong i = 0; i < equations.Size(); i++) {
        std::cout << ALE::Evaluate(equations[i]) << '\n';
    }
}
```

## Example 2

```cpp
#include "ALE.hpp"

#include <iostream>

using Qentem::ALE;

int main() {
    double result;

    if (ALE::Evaluate(result, "8^-2")) {
        std::cout << result << '\n';
    }

    if (!ALE::Evaluate(result, "abc")) {
        std::cout << "invalid" << '\n';
    }

    if (!ALE::Evaluate(result, "a+2")) {
        std::cout << "invalid" << '\n';
    }
}
```

## Example 3

```cpp
#include "ALE.hpp"

#include <iostream>

using Qentem::ALE;

int main() {
    std::cout << ALE::Evaluate(" A ==   A  ") << '\n'; // 1
    std::cout << ALE::Evaluate("A==A") << '\n';        // 1
    std::cout << ALE::Evaluate("A!=a") << '\n';        // 1
    std::cout << ALE::Evaluate("A==a") << '\n';        // 0
    std::cout << ALE::Evaluate("ABCD!=BCD") << '\n';   // 1
    std::cout << ALE::Evaluate("ABCD==abcd") << '\n';  // 0
}
```

## Example 4 (Brackets)

```cpp
#include "ALE.hpp"
#include "JSON.hpp"

#include <iostream>

using Qentem::ALE;
using Qentem::ALEHelper;
using Qentem::String;
using Qentem::UInt;
using Qentem::ULong;
using Qentem::JSON::Value;

struct aleHelper : ALEHelper {
    explicit aleHelper(const Value *value) : value_(value) {
    }

    bool ALESetNumber(double &number, const char *content,
                      UInt length) const override {
        const Value *val = value_->GetValue(content, length);

        if (val != nullptr) {
            return val->GetNumber(number);
        }

        return false;
    }

    bool ALEIsEqual(bool &result, const char *left, UInt left_length,
                    const char *right, UInt right_length) const override {
        const Value *val;
        const char * str_left;
        const char * str_right;
        ULong        str_left_length;
        ULong        str_right_length;

        if (left[0] == '{') {
            val = value_->GetValue(left, left_length);

            if (val != nullptr) {
                str_left        = val->Char();
                str_left_length = val->Length();
            } else {
                return false;
            }
        } else {
            str_left        = left;
            str_left_length = left_length;
        }

        if (right[0] == '{') {
            val = value_->GetValue(right, right_length);

            if (val != nullptr) {
                str_right        = val->Char();
                str_right_length = val->Length();
            } else {
                return false;
            }
        } else {
            str_right        = right;
            str_right_length = right_length;
        }

        result = ((str_left_length == str_right_length) &&
                  String::Compare(str_right, str_left, str_right_length));

        return true;
    }

  private:
    const Value *value_;
};

int main() {
    Value     value;
    aleHelper ale(&value);

    value["{one}"]  = 1;
    value["{two}"]  = 2;
    value["{ten}"]  = 10;
    value["{bool}"] = true;
    value["{name}"] = "Qentem";

    // std::cout << ALE::Evaluate("{two}", &ale) << '\n';
    std::cout << ALE::Evaluate("4^{two}+{one}", &ale) << '\n';       // 17
    std::cout << ALE::Evaluate("({ten}+{one}) == 11", &ale) << '\n'; // 1
    std::cout << ALE::Evaluate("({bool}) == 1", &ale) << '\n';       // 1
    std::cout << ALE::Evaluate("{name} == Qentem", &ale) << '\n';    // 1
}
```
