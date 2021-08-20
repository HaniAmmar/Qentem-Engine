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
        content += offset;
        const QENTEM_SIMD_VAR m_pattern = QENTEM_SIMD_SET_TO_ONE_8(one_char);

        while (offset < end_before) {
            const QENTEM_SIMD_VAR m_content = QENTEM_SIMD_LOAD(
                reinterpret_cast<const QENTEM_SIMD_VAR *>(content));

            const QENTEM_SIMD_NUMBER_T bits =
                QENTEM_SIMD_COMPARE_8_MASK(m_pattern, m_content);

            if (bits != 0) {
                const Number_T_ index = (Platform::CTZ(bits) + offset + 1);

                if (index > end_before) {
                    return 0;
                }

                return index;
            }

            offset += QENTEM_SIMD_SIZE;
            content += QENTEM_SIMD_SIZE;
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

        content += offset;

        const SizeT           len_less_one = (pattern_length - 1);
        const QENTEM_SIMD_VAR m_pattern_first =
            QENTEM_SIMD_SET_TO_ONE_8(*pattern);
        const QENTEM_SIMD_VAR m_pattern_last =
            QENTEM_SIMD_SET_TO_ONE_8(pattern[len_less_one]);

        while (offset < end_before) {
            QENTEM_SIMD_VAR m_content = QENTEM_SIMD_LOAD(
                reinterpret_cast<const QENTEM_SIMD_VAR *>(content));
            QENTEM_SIMD_NUMBER_T bits =
                QENTEM_SIMD_COMPARE_8_MASK(m_content, m_pattern_first);

            m_content =
                QENTEM_SIMD_LOAD(reinterpret_cast<const QENTEM_SIMD_VAR *>(
                    content + len_less_one));
            bits &= QENTEM_SIMD_COMPARE_8_MASK(m_content, m_pattern_last);

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

                bits ^= (QENTEM_SIMD_NUMBER_T{1} << index);
            }

            offset += QENTEM_SIMD_SIZE;
            content += QENTEM_SIMD_SIZE;
        }

        return 0;
    }
#else

    /*
     * Returns an the index of a character + 1.
     */
    template <typename Char_T_, typename Number_T_>
    static Number_T_ FindOne(Char_T_ one_char, const Char_T_ *content,
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
                if (pattern[pattern_length] ==
                    content[offset + pattern_length]) {
                    const Char_T_ *tmp_content = (content + offset);
                    Number_T_      tmp_offset  = 1;

                    while ((tmp_offset != pattern_length) &&
                           (pattern[tmp_offset] == tmp_content[tmp_offset])) {
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
    static Number_T_
    SkipInnerPatterns(const Char_T_ *prefix, SizeT prefix_length,
                      const Char_T_ *suffix, SizeT suffix_length,
                      const Char_T_ *content, Number_T_ offset,
                      Number_T_ max_end_before) noexcept {
        Number_T_ offset2 = offset;

        do {
            offset2 =
                Find(suffix, suffix_length, content, offset2, max_end_before);
            offset = Find(prefix, prefix_length, content, offset, offset2);

            if (offset == 0) {
                return offset2;
            }
        } while (true);

        return 0;
    }

    template <typename Char_T_, typename Number_T_>
    static Number_T_ SkipInnerPatterns(const Char_T_  prefix,
                                       const Char_T_  suffix,
                                       const Char_T_ *content, Number_T_ offset,
                                       Number_T_ max_end_before) noexcept {
        Number_T_ offset2 = offset;

        do {
            offset2 = FindOne(suffix, content, offset2, max_end_before);
            offset  = FindOne(prefix, content, offset, offset2);

            if (offset == 0) {
                return offset2;
            }
        } while (true);

        return 0;
    }
};

} // namespace Qentem

#endif
