# Arithmetic & Logic Evaluator

## Introduction

ALE is for evaluating `{math:...` and `if case="..."` for [Template](https://github.com/HaniAmmar/Qentem-Engine/blob/master/Documentation/Template.md).

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
        std::cout << "Invalid" << '\n';
    }

    if (!ALE::Evaluate(result, "a+2")) {
        std::cout << "Invalid" << '\n';
    }
}
```

## Example 3

```cpp
#include "ALE.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::ALE;
using Qentem::ALEHelper;
using Value = Qentem::Value<char>;

struct aleHelper {
    explicit aleHelper(const Value *value) : value_(value) {}

    bool ALESetNumber(double &number, const char *content,
                      unsigned int length) const {
        const Value *val = value_->GetValue(content, length);

        if (val != nullptr) {
            return val->SetNumber(number);
        }

        return false;
    }

    bool ALEIsEqual(bool &result, const char *left, unsigned int left_length,
                    const char *right, unsigned int right_length) const {
        const Value *val;
        const char * str_left;
        const char * str_right;
        unsigned int str_left_length;
        unsigned int str_right_length;

        if (left[0] == '{') {
            val = value_->GetValue(left, left_length);

            if (val != nullptr) {
                str_left        = val->StringStorage();
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
                str_right        = val->StringStorage();
                str_right_length = val->Length();
            } else {
                return false;
            }
        } else {
            str_right        = right;
            str_right_length = right_length;
        }

        result = ((str_left_length == str_right_length) &&
                  Qentem::StringUtils::IsEqual(str_right, str_left,
                                               str_right_length));

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

    std::cout << ALE::Evaluate("{two}", &ale) << '\n';               // 2
    std::cout << ALE::Evaluate("4^{two}+{one}", &ale) << '\n';       // 17
    std::cout << ALE::Evaluate("({ten}+{one}) == 11", &ale) << '\n'; // 1
    std::cout << ALE::Evaluate("({bool}) == 1", &ale) << '\n';       // 1
    std::cout << ALE::Evaluate("{name} == Qentem", &ale) << '\n';    // 1
}
```
