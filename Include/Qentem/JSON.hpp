/**
 * @file JSON.hpp
 * @brief Provides an iterative, non-recursive JSON parser for Qentem Engine.
 *
 * This header defines the core types and algorithms for parsing, representing,
 * and manipulating JSON documents. Parsing is performed using an explicit
 * state machine rather than recursive descent, enabling safe handling of
 * deeply nested structures. Optional support for C-style line and block
 * comments is provided as a preprocessing step.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_JSON_H
#define QENTEM_JSON_H

#include "Qentem/Value.hpp"

namespace Qentem {

struct JSON {
    template <typename Stream_T, typename Char_T, typename Number_T>
    QENTEM_INLINE static Value<Char_T> Parse(Stream_T &stream, const Char_T *content, Number_T length) {
        return parse(stream, content, SizeT(length));
    }

    template <typename Char_T, typename Number_T>
    QENTEM_INLINE static Value<Char_T> Parse(const Char_T *content, Number_T length) {
        StringStream<Char_T> stream;
        return Parse(stream, content, SizeT(length));
    }

    template <typename Char_T>
    QENTEM_INLINE static Value<Char_T> Parse(const Char_T *content) {
        return Parse(content, StringUtils::Count(content));
    }

    template <typename Stream_T, typename Char_T, typename Number_T>
    QENTEM_INLINE static Value<Char_T> ParseWithComments(Stream_T &stream, const Char_T *content, Number_T length) {
        return parse<Stream_T, Char_T, true>(stream, content, SizeT(length));
    }

    template <typename Char_T, typename Number_T>
    QENTEM_INLINE static Value<Char_T> ParseWithComments(const Char_T *content, Number_T length) {
        StringStream<Char_T> stream;
        return ParseWithComments(stream, content, SizeT(length));
    }

    template <typename Char_T>
    QENTEM_INLINE static Value<Char_T> ParseWithComments(const Char_T *content) {
        return ParseWithComments(content, StringUtils::Count(content));
    }

  private:
    template <typename Stream_T, typename Char_T, bool WithComments_T = false>
    static Value<Char_T> parse(Stream_T &stream, const Char_T *content, SizeT length) {
        using WhiteSpaceChars = StringUtils::WhiteSpaceChars_T<Char_T>;
        Value<Char_T> value{};

        if (length != 0) {
            SizeT offset = 0;
            parse<Value<Char_T>, Stream_T, Char_T, WithComments_T>(value, stream, content, offset, length);

            if constexpr (WithComments_T) {
                while (true) {
                    while ((offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                                 (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                                 (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                                 (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
                        ++offset;
                    }

                    // Comments at the end
                    if ((offset < length) && (content[offset] == '/')) {
                        ++offset;

                        if (offset < length) {
                            if (content[offset] == '/') {
                                while ((offset < length) && (content[offset] != WhiteSpaceChars::LineControlChar)) {
                                    ++offset;
                                }
                            } else if (content[offset] == '*') {
                                while ((++offset < length) && (content[offset] != '*')) {
                                }

                                if ((++offset < length) && (content[offset] == '/')) {
                                    ++offset;
                                } else {
                                    value.Reset();
                                }
                            } else {
                                value.Reset();
                            }
                        }
                    } else {
                        break;
                    }
                }
            }

            while ((offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                         (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                         (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                         (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
                ++offset;
            }

            if (offset != length) {
                value.Reset();
            }
        }

        return value;
    }

    template <typename ValueT, typename Stream_T, typename Char_T, bool WithComments_T = false>
    static void parse(ValueT &value, Stream_T &stream, const Char_T *content, SizeT &offset, const SizeT length) {
        using WhiteSpaceChars   = StringUtils::WhiteSpaceChars_T<Char_T>;
        using NotationConstants = JSONUtils::NotationConstants_T<Char_T>;
        using ObjectT           = typename ValueT::ObjectT;
        using ArrayT            = typename ValueT::ArrayT;

        while ((offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                     (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                     (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                     (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
            ++offset;
        }

        if (offset < length) {
            Array<ValueT *> tree{SizeT{8}};
            ValueT         *parent{&value};
            ObjectT        *obj{nullptr};
            ArrayT         *arr{nullptr};
            ValueT         *obj_value{nullptr};
            bool            expecting_value{false};

            // Comments at the begining
            if constexpr (WithComments_T) {
                while (true) {
                    while ((offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                                 (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                                 (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                                 (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
                        ++offset;
                    }

                    if ((offset < length) && (content[offset] == '/')) {
                        ++offset;

                        if (offset < length) {
                            if (content[offset] == '/') {
                                while ((offset < length) && (content[offset] != WhiteSpaceChars::LineControlChar)) {
                                    ++offset;
                                }
                            } else if (content[offset] == '*') {
                                while ((++offset < length) && (content[offset] != '*')) {
                                }

                                if ((++offset < length) && (content[offset] == '/')) {
                                    ++offset;
                                } else {
                                    return;
                                }
                            } else {
                                return;
                            }
                        }
                    } else {
                        break;
                    }
                }

                while ((offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                             (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                             (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                             (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
                    ++offset;
                }
            }

            if (content[offset] == NotationConstants::SSquareChar) {
                value = {ValueType::Array};
                arr   = parent->GetArray();
            } else {
                value = {ValueType::Object};
                obj   = parent->GetObject();
            }

            while ((++offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                           (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                           (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                           (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
            }

            while (offset < length) {
                switch (content[offset]) {
                    case NotationConstants::QuoteChar: {
                        ++offset;

                        if (offset < length) {
                            const Char_T *str = (content + offset);
                            SizeT         len = JSONUtils::UnEscape(str, (length - offset), stream);

                            if (len != 0) {
                                offset += len;
                                --len;

                                if (stream.IsNotEmpty()) {
                                    str = stream.First();
                                    len = stream.Length();
                                    stream.Clear();
                                }

                                if (obj != nullptr) {
                                    if (obj_value == nullptr) {
                                        // Name
                                        obj_value = &((*obj)[String<Char_T>{str, len}]);
                                    } else {
                                        *obj_value = ValueT{String<Char_T>{str, len}};
                                        obj_value  = nullptr;
                                    }
                                } else {
                                    *arr += ValueT{String<Char_T>{str, len}};
                                }

                                expecting_value = false;
                            }
                        }

                        break;
                    }

                    case NotationConstants::F_Char: {
                        const Char_T *false_string = (NotationConstants::FalseString + 1U);

                        while ((++offset < length) && (content[offset] == *false_string)) {
                            ++false_string;
                        }

                        if (*false_string == Char_T{0}) {
                            if (obj_value != nullptr) {
                                *obj_value = ValueT{ValueType::False};
                                obj_value  = nullptr;
                            } else if (arr != nullptr) {
                                *arr += ValueT{ValueType::False};
                            } else {
                                value.Reset();
                                return;
                            }

                            expecting_value = false;
                            break;
                        }

                        value.Reset();
                        return;
                    }

                    case NotationConstants::N_Char: {
                        const Char_T *null_string = (NotationConstants::NullString + 1U);

                        while ((++offset < length) && (content[offset] == *null_string)) {
                            ++null_string;
                        }

                        if (*null_string == Char_T{0}) {
                            if (obj_value != nullptr) {
                                *obj_value = ValueT{ValueType::Null};
                                obj_value  = nullptr;
                            } else if (arr != nullptr) {
                                *arr += ValueT{ValueType::Null};
                            } else {
                                value.Reset();
                                return;
                            }

                            expecting_value = false;
                            break;
                        }

                        value.Reset();
                        return;
                    }

                    case NotationConstants::T_Char: {
                        const Char_T *true_string = (NotationConstants::TrueString + 1U);

                        while ((++offset < length) && (content[offset] == *true_string)) {
                            ++true_string;
                        }

                        if (*true_string == Char_T{0}) {
                            if (obj_value != nullptr) {
                                *obj_value = ValueT{ValueType::True};
                                obj_value  = nullptr;
                            } else if (arr != nullptr) {
                                *arr += ValueT{ValueType::True};
                            } else {
                                value.Reset();
                                return;
                            }

                            expecting_value = false;
                            break;
                        }

                        value.Reset();
                        return;
                    }

                    case NotationConstants::SSquareChar:
                    case NotationConstants::SCurlyChar: {
                        tree += parent;

                        const bool is_object = (content[offset] == NotationConstants::SCurlyChar);

                        if (obj_value != nullptr) {
                            *obj_value = ValueT{is_object ? ValueType::Object : ValueType::Array};
                            parent     = obj_value;
                            obj_value  = nullptr;
                        } else if (arr != nullptr) {
                            parent = &(arr->Insert(ValueT{is_object ? ValueType::Object : ValueType::Array}));
                        } else {
                            value.Reset();
                            return;
                        }

                        if (is_object) {
                            obj = parent->GetObject();
                            arr = nullptr;
                        } else {
                            obj = nullptr;
                            arr = parent->GetArray();
                        }

                        expecting_value = false;
                        ++offset;
                        break;
                    }

                    case NotationConstants::ESquareChar: {
                        if ((arr != nullptr) && !expecting_value) {
                            arr->Compress();
                            ++offset;

                            ValueT **last = tree.Last();

                            if (last != nullptr) {
                                parent = *(last);
                                tree.Drop(SizeT{1});

                                if (parent->IsObject()) {
                                    obj = parent->GetObject();
                                    arr = nullptr;
                                } else {
                                    obj = nullptr;
                                    arr = parent->GetArray();
                                }

                                break;
                            }

                            return;
                        }

                        value.Reset();
                        return;
                    }

                    case NotationConstants::ECurlyChar: {
                        if ((obj != nullptr) && !expecting_value) {
#if QENTEM_VALUE_EXPANSION_MULTIPLIER > 2U
                            obj->RemoveExcessStorage();
#endif
                            ++offset;

                            ValueT **last = tree.Last();

                            if (last != nullptr) {
                                parent = *(last);
                                tree.Drop(SizeT{1});

                                if (parent->IsObject()) {
                                    obj = parent->GetObject();
                                    arr = nullptr;
                                } else {
                                    obj = nullptr;
                                    arr = parent->GetArray();
                                }

                                break;
                            }

                            return;
                        }

                        value.Reset();
                        return;
                    }

                    default: {
                        if constexpr (WithComments_T) {
                            if (content[offset] == '/') {
                                ++offset;

                                if (offset < length) {
                                    if (content[offset] == '/') {
                                        while ((offset < length) &&
                                               (content[offset] != WhiteSpaceChars::LineControlChar)) {
                                            ++offset;
                                        }
                                    } else if (content[offset] == '*') {
                                        while ((++offset < length) && (content[offset] != '*')) {
                                        }

                                        if ((++offset < length) && (content[offset] == '/')) {
                                            ++offset;
                                        } else {
                                            break;
                                        }
                                    } else {
                                        break;
                                    }

                                    if (offset < length) {
                                        while ((offset < length) &&
                                               ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                                (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                                (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                                (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
                                            ++offset;
                                        }
                                    }

                                    if ((offset < length) && (content[offset] != NotationConstants::CommaChar) &&
                                        content[offset] != NotationConstants::ColonChar) {
                                        continue;
                                    }
                                }

                                break;
                            }
                        }

                        QNumber64 number;

                        switch (Digit::StringToNumber(number, content, offset, length)) {
                            case QNumberType::NotANumber: {
                                value.Reset();
                                return;
                            }

                            case QNumberType::Natural: {
                                if (obj_value != nullptr) {
                                    *obj_value = ValueT{number.Natural};
                                    obj_value  = nullptr;
                                } else if (arr != nullptr) {
                                    *arr += ValueT{number.Natural};
                                } else {
                                    value.Reset();
                                    return;
                                }

                                expecting_value = false;
                                break;
                            }

                            case QNumberType::Integer: {
                                if (obj_value != nullptr) {
                                    *obj_value = ValueT{number.Integer};
                                    obj_value  = nullptr;
                                } else if (arr != nullptr) {
                                    *arr += ValueT{number.Integer};
                                } else {
                                    value.Reset();
                                    return;
                                }

                                expecting_value = false;
                                break;
                            }

                            case QNumberType::Real: {
                                if (obj_value != nullptr) {
                                    *obj_value = ValueT{number.Real};
                                    obj_value  = nullptr;
                                } else if (arr != nullptr) {
                                    *arr += ValueT{number.Real};
                                } else {
                                    value.Reset();
                                    return;
                                }

                                expecting_value = false;
                                break;
                            }
                        };
                    }
                }

                while ((offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                             (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                             (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                             (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
                    ++offset;
                }

                if (offset < length) {
                    if (obj_value != nullptr) {
                        if (content[offset] == NotationConstants::ColonChar) {
                            expecting_value = true;
                            ++offset;
                        } else {
                            if constexpr (WithComments_T) {
                                if (content[offset] == NotationConstants::SlashChar) {
                                    continue;
                                }
                            }

                            break;
                        }
                    } else if (!expecting_value &&
                               (((arr != nullptr) && arr->IsNotEmpty()) || ((obj != nullptr) && obj->IsNotEmpty()))) {
                        if (content[offset] == NotationConstants::CommaChar) {
                            expecting_value = true;
                            ++offset;
                        } else {
                            if ((content[offset] == NotationConstants::ESquareChar) ||
                                (content[offset] == NotationConstants::ECurlyChar)) {
                                continue;
                            }

                            if constexpr (WithComments_T) {
                                if (content[offset] == NotationConstants::SlashChar) {
                                    continue;
                                }
                            }

                            break;
                        }
                    }
                }

                while ((offset < length) && ((content[offset] == WhiteSpaceChars::SpaceChar) ||
                                             (content[offset] == WhiteSpaceChars::LineControlChar) ||
                                             (content[offset] == WhiteSpaceChars::TabControlChar) ||
                                             (content[offset] == WhiteSpaceChars::CarriageControlChar))) {
                    ++offset;
                }
            }
        }

        value.Reset();
        return;
    }
};

} // namespace Qentem

#endif
