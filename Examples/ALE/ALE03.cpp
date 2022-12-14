#include "ALE.hpp"
#include "Value.hpp"

#include <iostream>

using Qentem::ALE;
using Qentem::SizeT;
using Qentem::StringUtils;
using Value = Qentem::Value<char>;
using Digit = Qentem::Digit<char>;

struct ALEHelper {
    explicit ALEHelper(const Value *value) : value_(value) {}

    bool ALESetNumber(double &number, const Value *value) const noexcept {
        if (value->SetNumber(number)) {
            return true;
        }

        const char *str;
        SizeT       len;

        if (value->SetCharAndLength(str, len)) {
            return ALE::Evaluate(number, str, len, this);
        }

        return false;
    }

    bool ALESetNumber(double &number, const char *content, SizeT length) const noexcept {
        const Value *value = nullptr;

        if (length > 2) {
            ++content;
            length -= 2; // 2 = {...}
            value = value_->GetValue(content, length);
        }

        return ((value != nullptr) && ALESetNumber(number, value));
    }

    bool ALEIsEqual(bool &result, const char *content, ALE::Number left, ALE::Number right, bool left_evaluated,
                    bool right_evaluated) const noexcept {
        const Value *value_left  = nullptr;
        const Value *value_right = nullptr;

        const char *left_content  = (content + left.Content.Offset);
        const char *right_content = (content + right.Content.Offset);
        SizeT       left_length   = left.Content.Length;
        SizeT       right_length  = right.Content.Length;

        bool is_number = (left_evaluated || right_evaluated);

        // If the left side is a variable
        if (!left_evaluated) {
            if ((*left_content == '{') && (left_length > 2)) {
                value_left = value_->GetValue((++left_content),
                                              (left_length - 2)); // {var:x}

                if (value_left == nullptr) {
                    return false;
                }

                if (value_left->IsNumber()) {
                    is_number = true;
                }
            } else {
                // 2 = (...)
                if (*left_content != '(') {
                    left_evaluated = Digit::StringToNumber(left.Number, left_content, left_length);
                } else {
                    left_evaluated = ALE::Evaluate(left.Number, (++left_content), (left_length -= 2), this);
                }

                if (!left_evaluated && is_number) {
                    return false;
                }

                is_number = left_evaluated;
            }
        }

        if (!right_evaluated) {
            // If the right side is a variable
            if ((*right_content == '{') && (right_length > 2)) {
                value_right = value_->GetValue((++right_content),
                                               (right_length - 2)); // {var:x}

                if (value_right == nullptr) {
                    return false;
                }

                if (value_right->IsNumber()) {
                    is_number = true;
                }
            } else if (is_number) {
                if (*right_content != '(') {
                    if (!(Digit::StringToNumber(right.Number, right_content, right_length))) {
                        return false;
                    }
                } else if (!(ALE::Evaluate(right.Number, (++right_content), (right_length - 2), this))) {
                    return false;
                }
            }
        }

        if (is_number) {
            if (!left_evaluated && ((value_left == nullptr) || !(ALESetNumber(left.Number, value_left)))) {
                return false;
            }

            if ((value_right != nullptr) && !(ALESetNumber(right.Number, value_right))) {
                return false;
            }
        } else {
            if ((value_left != nullptr) && !(value_left->SetCharAndLength(left_content, left_length))) {
                return false;
            }

            if ((value_right != nullptr) && !(value_right->SetCharAndLength(right_content, right_length))) {
                return false;
            }
        }

        if (is_number) {
            result = (left.Number == right.Number);
        } else {
            result = ((left_length == right_length) && StringUtils::IsEqual(left_content, right_content, right_length));
        }

        return true;
    }

  private:
    const Value *value_{nullptr};
};

int main() {
    Value     value;
    ALEHelper ale(&value);

    value["one"]  = 1;
    value["two"]  = 2;
    value["ten"]  = 10;
    value["bool"] = true;
    value["name"] = "Qentem";
    value["eq"]   = "(8+1+{one})";

    std::cout << ALE::Evaluate("{two}", &ale) << '\n';               // 2
    std::cout << ALE::Evaluate("4^{two}+{one}", &ale) << '\n';       // 17
    std::cout << ALE::Evaluate("({ten}+{one}) == 11", &ale) << '\n'; // 1
    std::cout << ALE::Evaluate("({bool}) == 1", &ale) << '\n';       // 1
    std::cout << ALE::Evaluate("{name} == Qentem", &ale) << '\n';    // 1
    std::cout << ALE::Evaluate("{eq}", &ale) << '\n';                // 10
    std::cout << ALE::Evaluate("10==(8+1+{one})", &ale) << '\n';     // 1
    std::cout << ALE::Evaluate("{eq}==9+1", &ale) << '\n';           // 1
    std::cout << ALE::Evaluate("(5*2)=={eq}", &ale) << '\n';         // 1
}
