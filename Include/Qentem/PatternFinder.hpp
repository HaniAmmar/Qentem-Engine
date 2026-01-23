/**
 * @file PatternFinder.hpp
 * @brief Implements a high-performance, multi-pattern string finder for Qentem Engine.
 *
 * This header defines the PatternFinder utility, which enables efficient searching for multiple
 * patterns or delimiters within a string in a single pass. PatternFinder is optimized for
 * scenarios requiring simultaneous lookup of several substrings or tokens, making it
 * ideal for template parsing and fast text processing tasks.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_FINDER_H
#define QENTEM_FINDER_H

#include "Qentem/QCommon.hpp"

namespace Qentem {

template <typename WordsList_T, typename Char_T, typename Number_T>
struct PatternFinder {
    PatternFinder()                                 = delete;
    PatternFinder(PatternFinder &&)                 = delete;
    PatternFinder(const PatternFinder &)            = delete;
    PatternFinder &operator=(PatternFinder &&)      = delete;
    PatternFinder &operator=(const PatternFinder &) = delete;
    ~PatternFinder()                                = default;

    PatternFinder(const Char_T *content, Number_T length) noexcept : content_{content}, length_{length} {
    }

    void NextSegment() noexcept {
        match_ = 0U;

        while (offset_ < length_) {
            SizeT32 id = WordsList_T::GetFirstCharID(content_[offset_]);

            if (id < WordsList_T::FirstCharsCount) {
                ++offset_;

                const Number_T start_offset = offset_;
                const SizeT32  group_count  = WordsList_T::GroupedByFirstCount[id];
                const SizeT32 *group_list   = WordsList_T::GroupedByFirstChar[id];
                id                          = 0U; // Reuse for 'word index'.

                do {
                    const SizeT32  word_id         = group_list[id];
                    const SizeT32  word_length     = WordsList_T::WordLength[word_id];
                    const Char_T  *word            = WordsList_T::Word[word_id];
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
            //         const SizeT32 word_id = WordsList_T::SingleCharGroup[char_index];

            //         if (content_[offset_] == WordsList_T::Word[word_id][0]) {
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

    QENTEM_INLINE void SetOffset(Number_T offset) noexcept {
        offset_ = offset;
    }

    QENTEM_INLINE Number_T GetOffset() const noexcept {
        return offset_;
    }

    QENTEM_INLINE Number_T GetLength() const noexcept {
        return length_;
    }

    QENTEM_INLINE SizeT32 CurrentMatch() const noexcept {
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
