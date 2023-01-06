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
    Engine()                          = delete;
    Engine(Engine &&)                 = delete;
    Engine(const Engine &)            = delete;
    Engine &operator=(Engine &&)      = delete;
    Engine &operator=(const Engine &) = delete;
    ~Engine()                         = delete;

    /*
     * Returns the (index+1) of a given character.
     */
    template <typename Char_T_, typename Number_T_>
    static Number_T_ FindOne(const Char_T_ char_1, const Char_T_ *content, Number_T_ offset,
                             const Number_T_ end_offset) noexcept {
#ifdef QENTEM_SIMD_ENABLED
        if (offset < end_offset) {
            SizeT m_size = ((end_offset - offset) >> QENTEM_SIMD_SHIFT_SIZE);

            if (m_size != 0) {
                const QENTEM_SIMD_VAR m_char_1 = Platform::SMIDSetToOne(char_1);

                do {
                    const QENTEM_SIMD_VAR m_content =
                        QENTEM_SIMD_LOAD(reinterpret_cast<const QENTEM_SIMD_VAR *>(content + offset));
                    const QENTEM_SIMD_NUMBER_T bits = Platform::SMIDCompare<Char_T_>(m_char_1, m_content);

                    if (bits != 0) {
                        return (Platform::CTZ(bits) + offset);
                    }

                    offset += Platform::SMIDNextOffset<Char_T_, Number_T_>();
                    --m_size;
                } while (m_size != 0);
            }
        }
#endif

        while (offset < end_offset) {
            if (char_1 == content[offset]) {
                return (offset + 1);
            }

            ++offset;
        }

        return 0;
    }

    /*
     * Returns the (index+length) of a given pattern.
     * pattern_length should be bigger than 1;
     */
    template <typename Char_T_, typename Number_T_>
    static Number_T_ Find(const Char_T_ *pattern, const SizeT pattern_length, const Char_T_ *content, Number_T_ offset,
                          Number_T_ end_offset) noexcept {
        if ((offset < end_offset) && ((offset + pattern_length) <= end_offset)) {
            const SizeT len_one_less = (pattern_length - 1);
            end_offset -= len_one_less;

#ifdef QENTEM_SIMD_ENABLED
            SizeT m_size = ((end_offset - offset) >> QENTEM_SIMD_SHIFT_SIZE);

            if (m_size != 0) {
                const Char_T_        *content_ofs     = (content + offset);
                const QENTEM_SIMD_VAR m_pattern_first = Platform::SMIDSetToOne(*pattern);
                const QENTEM_SIMD_VAR m_pattern_last  = Platform::SMIDSetToOne(pattern[len_one_less]);

                do {
                    QENTEM_SIMD_VAR m_content =
                        QENTEM_SIMD_LOAD(reinterpret_cast<const QENTEM_SIMD_VAR *>(content_ofs));
                    QENTEM_SIMD_NUMBER_T bits = Platform::SMIDCompare<Char_T_>(m_content, m_pattern_first);

                    m_content = QENTEM_SIMD_LOAD(reinterpret_cast<const QENTEM_SIMD_VAR *>(content_ofs + len_one_less));
                    bits &= Platform::SMIDCompare<Char_T_>(m_content, m_pattern_last);

                    while (bits != 0) {
                        const Number_T_ index         = Platform::CTZ(bits) - 1;
                        const Number_T_ pattern_index = (index + offset + pattern_length);

                        if ((len_one_less == 1) || StringUtils::IsEqual(pattern, (content_ofs + index), len_one_less)) {
                            return pattern_index;
                        }

                        bits ^= (QENTEM_SIMD_NUMBER_T{1} << index);
                    }

                    offset += Platform::SMIDNextOffset<Char_T_, Number_T_>();
                    content_ofs += Platform::SMIDNextOffset<Char_T_, Number_T_>();
                    --m_size;
                } while (m_size != 0);
            }
#endif

            while (offset < end_offset) {
                if ((*pattern == content[offset]) && (pattern[len_one_less] == content[offset + len_one_less])) {
                    Number_T_ tmp_offset = 1;

                    while ((tmp_offset < len_one_less) && (pattern[tmp_offset] == content[tmp_offset + offset])) {
                        ++tmp_offset;
                    }

                    if (tmp_offset == len_one_less) {
                        return (offset + tmp_offset + 1);
                    }
                }

                ++offset;
            }
        }

        return 0;
    }

    /*
     * If a search is about { ... }, and nest itself like:
     * {.{..{...}..}.}; then this function can skip inner brackets:
     *
     * return SkipInnerPatterns("{", 1, "}", 1,
     *                          content, offset, end_offset, max_end_before);
     *
     */
    template <typename Char_T_, typename Number_T_>
    static Number_T_ SkipInnerPatterns(const Char_T_ *prefix, SizeT prefix_length, const Char_T_ *suffix,
                                       SizeT suffix_length, const Char_T_ *content, Number_T_ offset,
                                       const Number_T_ max_end_before) noexcept {
        Number_T_ offset2 = offset;

        while (true) {
            offset2 = Find(suffix, suffix_length, content, offset2, max_end_before);
            offset  = Find(prefix, prefix_length, content, offset, offset2);

            if (offset == 0) {
                return offset2;
            }
        }

        return 0;
    }

    template <typename Char_T_, typename Number_T_>
    static Number_T_ SkipInnerPatterns(const Char_T_ prefix, const Char_T_ suffix, const Char_T_ *content,
                                       Number_T_ offset, const Number_T_ max_end_before) noexcept {
        Number_T_ offset2 = offset;

        while (true) {
            offset2 = FindOne(suffix, content, offset2, max_end_before);
            offset  = FindOne(prefix, content, offset, offset2);

            if (offset == 0) {
                return offset2;
            }
        }

        return 0;
    }
};

} // namespace Qentem

#endif
