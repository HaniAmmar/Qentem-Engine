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
