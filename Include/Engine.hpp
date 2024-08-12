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
namespace Engine {

template <typename WordsList_T, typename Char_T, typename Number_T>
struct Finder {
    Finder()                          = delete;
    Finder(Finder &&)                 = delete;
    Finder(const Finder &)            = delete;
    Finder &operator=(Finder &&)      = delete;
    Finder &operator=(const Finder &) = delete;
    ~Finder()                         = default;

    static constexpr SizeT32 Bits   = 4U; // Number of bits per ID
    static constexpr SizeT32 Blocks = ((sizeof(SystemIntType) * 8U) / Bits);

    Finder(const Char_T *content, Number_T length) noexcept : content_{content}, length_{length} {
    }

    inline void Next() noexcept {
        match_ = 0U;

        while (offset_ < length_) {
            SizeT32 id = WordsList_T::GetFirstCharID(content_[offset_]);

            if (id < WordsList_T::FirstCharsCount) {
                ++offset_;

                const Number_T start_offset = offset_;
                const SizeT32  group_count  = WordsList_T::GetGroupedByFirstCount(id);
                const SizeT32 *group_list   = WordsList_T::GetGroupedByFirstChar(id);
                id                          = 0U; // Reuse for 'word index'.

                do {
                    const SizeT32  word_id         = group_list[id];
                    const SizeT32  word_length     = WordsList_T::GetWordLength(word_id);
                    const Char_T  *word            = WordsList_T::GetWord(word_id);
                    const Number_T word_end_offset = Number_T(offset_ + word_length);

                    // Match the last char of the word.
                    if ((word_end_offset < length_) && (content_[word_end_offset] == word[word_length])) {
                        Number_T word_offset{0};

                        // Match chars between the first and the last char of the word.
                        while ((offset_ < word_end_offset) && (content_[offset_] == word[word_offset])) {
                            ++word_offset;
                            ++offset_;
                        }

                        if (offset_ == word_end_offset) {
                            match_ = (word_id + SizeT32{1});
                            ++offset_;
                            return;
                        }

                        offset_ = start_offset;
                    }
                } while (++id < group_count);

                continue;
            }

            // if (WordsList_T::SingleCharsCount == 1U) {
            // Match single chars.
            if (content_[offset_] == WordsList_T::SingleChar) {
                match_ = SizeT32{1};
                ++offset_;
                return;
            }
            // } else {
            //     SizeT32 char_index{0U};

            //     while (char_index < WordsList_T::SingleCharsCount) {
            //         const SizeT32 word_id = WordsList_T::GetSingleCharGroup()[char_index];

            //         if (content_[offset_] == WordsList_T::GetWord(word_id)[0]) {
            //             match_ = (word_id + SizeT32{1});
            //             ++offset_;
            //             return;
            //         }

            //         ++char_index;
            //     }
            // }

            ++offset_;
        }
    }

    inline void SetOffset(Number_T offset) noexcept {
        offset_ = offset;
    }

    inline Number_T GetOffset() const noexcept {
        return offset_;
    }

    inline Number_T GetLength() const noexcept {
        return length_;
    }

    inline SizeT32 GetMatch() const noexcept {
        // Call 'Next()' before calling 'GetMatch()'.
        return match_;
    }

  private:
    const Char_T  *content_;
    const Number_T length_;
    Number_T       offset_{0};
    SizeT32        match_{0};
};

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

} // namespace Engine
} // namespace Qentem

#endif
