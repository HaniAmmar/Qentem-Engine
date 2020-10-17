/*
 * Copyright (c) 2020 Hani Ammar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Platform.hpp"
#include "StringUtils.hpp"

#ifndef QENTEM_ENGINE_H_
#define QENTEM_ENGINE_H_

namespace Qentem {

/*
 * A fast search class for nest/sub-matching.
 */
class Engine {
  public:
    Engine()               = delete;
    Engine(Engine &&)      = delete;
    Engine(const Engine &) = delete;
    Engine &operator=(Engine &&) = delete;
    Engine &operator=(const Engine &) = delete;
    ~Engine()                         = delete;
#ifdef QENTEM_SIMD_ENABLED
    /*
     * Returns an the index of a character + 1.
     */

    template <typename Number_T_>
    static Number_T_ FindOne(const char one_char, const char *content,
                             Number_T_ offset, Number_T_ end_before) noexcept {
        // TODO: Add 16 and 32-bit char
        if (offset < end_before) {
            content += offset;
            const QMM_VAR_ m_pattern = QMM_SETONE_8_(one_char);

            do {
                const QMM_VAR_ m_content =
                    QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(content));

                const QMM_Number_T bits =
                    QMM_COMPARE_8_MASK_(m_pattern, m_content);

                if (bits != 0) {
                    const Number_T_ index = (Platform::CTZ(bits) + offset + 1);

                    if (index > end_before) {
                        return 0;
                    }

                    return index;
                }

                offset += QMM_SIZE_;
                content += QMM_SIZE_;
            } while (offset < end_before);
        }

        return 0;
    }

    /*
     * Returns an the index of a pattern + the length of it.
     */
    template <typename Number_T_>
    static Number_T_ Find(const char *pattern, SizeT pattern_length,
                          const char *content, Number_T_ offset,
                          Number_T_ end_before) noexcept {
        if (pattern_length == 1) {
            return FindOne(*pattern, content, offset, end_before);
        }

        if (offset < end_before) {
            content += offset;

            const QMM_VAR_ m_pattern_first = QMM_SETONE_8_(*pattern);
            const SizeT    len_less_one    = (pattern_length - 1);
            const QMM_VAR_ m_pattern_last =
                QMM_SETONE_8_(pattern[len_less_one]);

            do {
                QMM_VAR_ m_content =
                    QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(content));
                QMM_Number_T bits =
                    QMM_COMPARE_8_MASK_(m_content, m_pattern_first);

                m_content = QMM_LOAD_(
                    reinterpret_cast<const QMM_VAR_ *>(content + len_less_one));
                bits &= QMM_COMPARE_8_MASK_(m_content, m_pattern_last);

                while (bits != 0) {
                    const Number_T_ index = Platform::CTZ(bits);
                    const Number_T_ pattern_index =
                        (index + offset + pattern_length);

                    if (pattern_index > end_before) {
                        return 0;
                    }

                    if ((len_less_one == 1) ||
                        StringUtils::IsEqual(pattern, (content + index),
                                             len_less_one)) {
                        return pattern_index;
                    }

                    bits ^= (QMM_Number_T{1} << index);
                }

                offset += QMM_SIZE_;
                content += QMM_SIZE_;
            } while (offset < end_before);
        }

        return 0;
    }
#else
    /*
     * Returns an the index of a character + 1.
     */
    template <typename Char_T_, typename Number_T_>
    static Number_T_ FindOne(const Char_T_ one_char, const Char_T_ *content,
                             Number_T_ offset, Number_T_ end_before) noexcept {
        while (offset < end_before) {
            if (one_char == content[offset]) {
                return (offset + 1);
            }

            ++offset;
        }

        return 0;
    }

    /*
     * Returns an the index of a pattern + the length of it.
     */
    template <typename Char_T_, typename Number_T_>
    QENTEM_NOINLINE static Number_T_
    Find(const Char_T_ *pattern, SizeT pattern_length, const Char_T_ *content,
         Number_T_ offset, Number_T_ end_before) noexcept {
        if (pattern_length == 1) {
            return FindOne(*pattern, content, offset, end_before);
        }

        --pattern_length;

        if ((offset + pattern_length) >= end_before) {
            return 0;
        }

        end_before -= pattern_length;

        while (offset < end_before) {
            if (*pattern == content[offset]) {
                Number_T_ tmp_offset = (offset + pattern_length);

                if (pattern[pattern_length] == content[tmp_offset]) {
                    const Char_T_ *tmp_content = (content + offset);
                    tmp_offset                 = 1;

                    while ((tmp_offset != pattern_length) &&
                           pattern[tmp_offset] == tmp_content[tmp_offset]) {
                        ++tmp_offset;
                    }

                    if (tmp_offset == pattern_length) {
                        return (offset + tmp_offset + 1);
                    }
                }
            }

            ++offset;
        }

        return 0;
    }
#endif

    /*
     * Finds the head and the tail of a match and search aging inside the match
     * to see if it's Nested by itself or a custom expression.
     */
    template <typename Char_T_, typename Number_T_, typename Class_T_>
    static Number_T_ FindNest(const Char_T_ *content, Number_T_ offset,
                              Number_T_ end_before, Number_T_ max_end_before,
                              Class_T_ *caller) {
        Number_T_ current_offset = offset;

        while (current_offset < end_before) {
            /*
             * Number_T_ FindH(const Class_T_ *content, Number_T_ offset,
             *                 Number_T_ end_before)
             *
             * For finding the head
             */
            current_offset = caller->FindH(content, current_offset, end_before);

            if (current_offset == 0) {
                // The head hasn't been found
                break;
            }

            // Save the current offset for Found().
            const Number_T_ start_offset = current_offset;
            // Makes sub-matching easy.
            Number_T_ sub_offset = current_offset;

            /*
             * bool HasTail();
             *
             * This is invoked to check if the match has a second part/tail.
             * Like { for the head and the tail is } or () or "" or < > ...
             */
            if (caller->HasTail()) {
                do {
                    /*
                     * Number_T_ FindT(const Class_T_ *content, Number_T_
                     *                 offset, Number_T_ end_before)
                     *
                     * For finding the tail
                     */
                    current_offset =
                        caller->FindT(content, current_offset, max_end_before);

                    if (current_offset == 0) {
                        /*
                         * void Failed()
                         *
                         * If the head of the match is found but not the tail,
                         * this function will be invoked.
                         */
                        caller->Failed();
                        return sub_offset;
                    }

                    /*
                     * Number_T_ Nest(const Class_T_ *content, Number_T_ offset,
                     *                Number_T_ end_before,
                     *                Number_T_ max_end_before)
                     *
                     * This function is invoked everytime a match found, and
                     * should look like the first line that started the search.
                     * with the exception of setting different values; like
                     * passing a sub-container.
                     */
                    sub_offset = caller->Nest(content, sub_offset,
                                              current_offset, max_end_before);

                    if (current_offset > sub_offset) {
                        /*
                         * No sub-matches or none of them breaking this match,
                         * So stop and call this a match.
                         */
                        break;
                    }

                    /*
                     * If nest() returns a bigger offset than current_offset,
                     * then this match is not complete. Adjust the offset and
                     * keep looking.
                     */
                    current_offset = sub_offset;
                } while (current_offset != max_end_before);
            }

            /*
             * void Found(const Class_T_ *content, Number_T_ offset, Number_T_
             *            end_before, Number_T_ start_offset, Number_T_
             * &current_offset)
             *
             * Once a full match is found (head & tail), the function will be
             * invoked. "current_offset" is referenced: it can be adjusted to
             * set the starting offset of the next search.
             */
            caller->Found(content, offset, end_before, start_offset,
                          current_offset);
        }

        // Return the location of the last match.
        return current_offset;
    }

    /*
     * If a search is about { ... }, and nest itself like:
     * {.{..{...}..}.}; then this function can be invoked from nest() to skip
     * inner brackets:
     *
     * return SkipInnerPatterns("{", 1, "}", 1,
     *                          content, offset, end_before, max_end_before);
     *
     * See Template::nest(...)
     */
    template <typename Char_T_, typename Number_T_>
    static Number_T_ SkipInnerPatterns(const Char_T_ *start, SizeT start_length,
                                       const Char_T_ *end, SizeT end_length,
                                       const Char_T_ *content, Number_T_ offset,
                                       Number_T_ max_end_before) noexcept {
        Number_T_ end_before = offset;

        do {
            end_before =
                Find(end, end_length, content, end_before, max_end_before);

            if (end_before != 0) {
                offset = Find(start, start_length, content, offset, end_before);

                if (offset != 0) {
                    continue;
                }
            }

            break;
        } while (true);

        return end_before;
    }

    template <typename Char_T_, typename Number_T_>
    static Number_T_ SkipInnerPatterns(const Char_T_ start, const Char_T_ end,
                                       const Char_T_ *content, Number_T_ offset,
                                       Number_T_ max_end_before) noexcept {
        Number_T_ end_before = offset;

        do {
            end_before = FindOne(end, content, end_before, max_end_before);

            if (end_before != 0) {
                offset = FindOne(start, content, offset, end_before);

                if (offset != 0) {
                    continue;
                }
            }

            break;
        } while (true);

        return end_before;
    }
};

} // namespace Qentem

#endif
