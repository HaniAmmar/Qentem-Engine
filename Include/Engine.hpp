/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_ENGINE_H
#define QENTEM_ENGINE_H

#include "Memory.hpp"
#include "Platform.hpp"
#include "StringUtils.hpp"

namespace Qentem {
/*
 * A fast search for nest/sub-matching.
 */
struct Engine {
    Engine()                          = delete;
    Engine(Engine &&)                 = delete;
    Engine(const Engine &)            = delete;
    Engine &operator=(Engine &&)      = delete;
    Engine &operator=(const Engine &) = delete;
    ~Engine()                         = delete;

    /*
     * Every word's ID is stored in 4 bits,
     * 64bit system can store 16 IDs. 16*4 = 64 bits
     * 32bit system can store 8 IDs. 8*4 = 32 bits
     * bit_count = sizeof(SystemIntType) * 8 = 32 or 64
     * blocks = bit_count / 4 = 8 or 16
     */

    static constexpr SizeT32 Bits   = 4U; // Number of bits per ID
    static constexpr SizeT32 Blocks = ((sizeof(SystemIntType) * 8U) / Bits);

    template <typename WordsList_T, typename Char_T, typename Number_T>
    static void Find(const WordsList_T &list, SystemIntType *storage, const Char_T *content, Number_T offset,
                     const Number_T end_offset) {
        SizeT32 id;
        SizeT32 group_count;

        do {
            group_count = 0;

            while (offset < end_offset) {
                id = 0;

                while ((id < list.FirstCharsCount) && (content[offset] != list.GetFirstChar(id))) {
                    ++id;
                }

                if (id < list.FirstCharsCount) {
                    group_count = list.GetGroupedByFirstCount(id);

                    break;
                }

                // Match single chars.
                SizeT32 char_index{0U};

                while (char_index < list.SingleCharGroupCount) {
                    const SizeT32 word_id = list.GetSingleCharGroup()[char_index];

                    if (content[offset] == list.GetWord(word_id)[0]) {
                        const SizeT   storage_index = SizeT(offset / Blocks);
                        const SizeT32 item_index    = SizeT32(offset - (storage_index * Blocks));

                        storage[storage_index] |= (SystemIntType(word_id + SizeT32{1}) << (item_index * Bits));

                        break;
                    }

                    ++char_index;
                }

                ++offset;
            }

            ++offset;

            if ((group_count != SizeT32{0}) && (offset < end_offset)) {
                const Number_T start_offset = offset;
                const SizeT32 *group_list   = list.GetGroupedByFirstChar(id);
                id                          = 0; // Reuse for 'word index'.

                while (id < group_count) {
                    const SizeT32  word_id         = group_list[id];
                    const SizeT32  word_length     = list.GetWordLength(word_id);
                    const Number_T word_end_offset = Number_T(offset + word_length);
                    const Char_T  *word            = list.GetWord(word_id);

                    // Match the last char of the word.
                    if ((word_end_offset < end_offset) && (content[word_end_offset] == word[word_length])) {
                        Number_T word_offset{0};

                        // Match chars between the first and the last char of the word.
                        while ((offset < word_end_offset) && (content[offset] == word[word_offset])) {
                            ++word_offset;
                            ++offset;
                        }

                        if (offset == word_end_offset) {
                            word_offset                 = Number_T(start_offset - Number_T{1});
                            const SizeT   storage_index = SizeT(word_offset / Blocks);
                            const SizeT32 item_index    = SizeT32(word_offset - (storage_index * Blocks));

                            storage[storage_index] |= (SystemIntType(word_id + SizeT32{1}) << (item_index * Bits));
                            ++offset;

                            break;
                        }

                        offset = start_offset;
                    }

                    ++id;
                }
            }
        } while (offset < end_offset);
    }

    /*
     * Returns the (index+1) of a given character.
     */
    template <typename Char_T, typename Number_T>
    static Number_T FindOne(const Char_T char_1, const Char_T *content, Number_T offset, const Number_T end_offset,
                            Number_T full_length = 0) noexcept {
#if defined(QENTEM_SIMD_ENABLED) && (QENTEM_SIMD_ENABLED == 1)
        if (offset < end_offset) {
            Number_T m_size =
                (Number_T(((full_length > end_offset) ? full_length : end_offset) - offset) >> Platform::SIMD::Shift);

            if (m_size != Number_T{0}) {
                const Platform::SIMD::VAR_T m_char_1 = Platform::SMIDSetToOne(char_1);

                do {
                    const Platform::SIMD::VAR_T m_content =
                        Platform::SIMD::Load(Memory::ChangePointer<const Platform::SIMD::VAR_T>(content + offset));
                    const Platform::SIMD::Number_T bits = Platform::SMIDCompare<Char_T>(m_char_1, m_content);

                    if (bits != Platform::SIMD::Number_T{0}) {
                        const Number_T simd_offset = Number_T(Platform::FindFirstBit(bits) + offset + Number_T{1});
                        return ((simd_offset <= end_offset) ? simd_offset : Number_T{0});
                    }

                    offset += Platform::SMIDNextOffset<Char_T, Number_T>();
                    --m_size;
                } while ((m_size != Number_T{0}) && (offset < end_offset));
            }
        }
#else
        (void)full_length;
#endif

        while ((offset < end_offset) && (char_1 != content[offset])) {
            ++offset;
        }

        if (offset < end_offset) {
            ++offset;
            return offset;
        }

        return Number_T{0};
    }

    /*
     * Returns the (index+length) of a given pattern.
     * 'pattern_length' should be greater than 1;
     */
    template <typename Char_T, typename Number_T>
    static Number_T Find(const Char_T *pattern, const Number_T pattern_length, const Char_T *content, Number_T offset,
                         Number_T end_offset, Number_T full_length = 0) noexcept {
        if ((offset < end_offset) && ((offset + pattern_length) <= end_offset)) {
            const Number_T len_one_less = (pattern_length - Number_T{1});
            const Char_T   pattern_last = pattern[len_one_less];
            end_offset -= len_one_less;

#if defined(QENTEM_SIMD_ENABLED) && (QENTEM_SIMD_ENABLED == 1)
            Number_T m_size =
                (Number_T(((full_length > end_offset) ? (full_length - len_one_less) : end_offset) - offset) >>
                 Platform::SIMD::Shift);

            if (m_size != Number_T{0}) {
                const Char_T               *content_ofs     = (content + offset);
                const Platform::SIMD::VAR_T m_pattern_first = Platform::SMIDSetToOne(*pattern);
                const Platform::SIMD::VAR_T m_pattern_last  = Platform::SMIDSetToOne(pattern_last);

                do {
                    Platform::SIMD::VAR_T m_content =
                        Platform::SIMD::Load(Memory::ChangePointer<const Platform::SIMD::VAR_T>(content_ofs));
                    Platform::SIMD::Number_T bits = Platform::SMIDCompare<Char_T>(m_content, m_pattern_first);

                    m_content = Platform::SIMD::Load(
                        Memory::ChangePointer<const Platform::SIMD::VAR_T>(content_ofs + len_one_less));
                    bits &= Platform::SMIDCompare<Char_T>(m_content, m_pattern_last);

                    while (bits != Platform::SIMD::Number_T{0}) {
                        const Number_T index         = Number_T(Platform::FindFirstBit(bits));
                        const Number_T pattern_index = (index + offset);

                        if ((index + offset) > end_offset) {
                            return Number_T{0};
                        }

                        if (StringUtils::IsEqual(pattern, (content_ofs + index), len_one_less)) {
                            return (pattern_index + pattern_length);
                        }

                        bits ^= (Platform::SIMD::Number_T{1} << index);
                    }

                    offset += Platform::SMIDNextOffset<Char_T, Number_T>();
                    content_ofs += Platform::SMIDNextOffset<Char_T, Number_T>();
                    --m_size;
                } while ((m_size != Number_T{0}) && (offset < end_offset));
            }
#else
            (void)full_length;
#endif

            while (offset < end_offset) {
                if ((*pattern == content[offset]) && (pattern_last == content[offset + len_one_less])) {
                    Number_T tmp_offset{1};

                    while ((tmp_offset < len_one_less) && (pattern[tmp_offset] == content[tmp_offset + offset])) {
                        ++tmp_offset;
                    }

                    if (tmp_offset == len_one_less) {
                        offset += tmp_offset;
                        ++offset;
                        return offset;
                    }
                }

                ++offset;
            }
        }

        return Number_T{0};
    }

    /*
     *
     * SkipInnerPatterns("{{", 2, "}}", 2, content, offset, end_offset, full_length);
     *
     * 'prefix_length' and 'suffix_length' should be greater than 1;
     */
    template <typename Char_T, typename Number_T>
    static Number_T SkipInnerPatterns(const Char_T *prefix, Number_T prefix_length, const Char_T *suffix,
                                      SizeT suffix_length, const Char_T *content, Number_T offset,
                                      const Number_T end_offset, Number_T full_length = 0) noexcept {
        Number_T offset2 = offset;

        do {
            offset2 = Find(suffix, suffix_length, content, offset2, end_offset, full_length);
            offset  = Find(prefix, prefix_length, content, offset, offset2, full_length);
        } while (offset != Number_T{0});

        return offset2;
    }

    /*
     * If a search is about { ... }, and nest itself like:
     * {.{..{...}..}.}; then this function can skip inner brackets:
     *
     * SkipInnerPatterns("{", "}", content, offset, end_offset, full_length);
     *
     */

    template <typename Char_T, typename Number_T>
    static Number_T SkipInnerPatterns(const Char_T prefix, const Char_T suffix, const Char_T *content, Number_T offset,
                                      const Number_T end_offset, Number_T full_length = 0) noexcept {
        Number_T offset2 = offset;

        do {
            offset2 = FindOne(suffix, content, offset2, end_offset, full_length);
            offset  = FindOne(prefix, content, offset, offset2, full_length);
        } while (offset != Number_T{0});

        return offset2;
    }
};

} // namespace Qentem

#endif
