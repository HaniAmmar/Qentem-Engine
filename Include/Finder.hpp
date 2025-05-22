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

#ifndef QENTEM_FINDER_H
#define QENTEM_FINDER_H

#include "QCommon.hpp"

namespace Qentem {

template <typename WordsList_T, typename Char_T, typename Number_T>
struct Finder {
    Finder()                          = delete;
    Finder(Finder &&)                 = delete;
    Finder(const Finder &)            = delete;
    Finder &operator=(Finder &&)      = delete;
    Finder &operator=(const Finder &) = delete;
    ~Finder()                         = default;

    Finder(const Char_T *content, Number_T length) noexcept : content_{content}, length_{length} {
    }

    inline void NextSegment() noexcept {
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

    inline SizeT32 CurrentMatch() const noexcept {
        // Call 'NextSegment()' before calling 'GetMatch()'.
        return match_;
    }

  private:
    const Char_T  *content_;
    const Number_T length_;
    Number_T       offset_{0};
    SizeT32        match_{0};
};

} // namespace Qentem

#endif
