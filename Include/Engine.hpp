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

#include "Memory.hpp"

#ifndef QENTEM_ENGINE_H_
#define QENTEM_ENGINE_H_

namespace Qentem {

/*
 * A customizable lexer for nest/sub-matching.
 */
class Engine {
  public:
    /*
     * This function resturn an the offset of the match + the length of the
     * keyword, or zero.
     */
#ifdef QENTEM_SIMD_ENABLED_
    static ULong Find(const char *keyword, UInt keyword_length,
                      const char *content, ULong offset, ULong end_before) {
        const QMM_VAR_ m_keyword_first = QMM_SETONE_8_(keyword[0]);
        QMM_VAR_       m_keyword_last;

        if (keyword_length != 1U) {
            m_keyword_last = QMM_SETONE_8_(keyword[keyword_length - 1U]);
        }

        while (offset < end_before) {
            const QMM_VAR_ m_content_0 =
                QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(content + offset));

            QMM_Number_ bits =
                QMM_COMPARE_8_MASK_(m_content_0, m_keyword_first);

            if (keyword_length != 1U) {
                const QMM_VAR_ m_content_1 =
                    QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(
                        content + offset + (keyword_length - 1U)));
                bits &= QMM_COMPARE_8_MASK_(m_content_1, m_keyword_last);
            }

            while (bits != 0) {
                const ULong index = (Q_CTZL(bits) + offset);

                if ((index + keyword_length) > end_before) {
                    return 0;
                }

                if ((keyword_length < 3U) ||
                    Memory::Compare(keyword, (content + index),
                                    keyword_length)) {
                    return (index + keyword_length);
                }

                bits &= (bits - 1U); // Remove the leading one
            }

            offset += QMM_SIZE_;
        }

        // No match.
        return 0;
    }
#else
    // Old school
    static ULong Find(const char *keyword, UInt keyword_length,
                      const char *content, ULong offset,
                      ULong end_before) noexcept {
        ULong last_offset;

        while (offset < end_before) {
            if (keyword[0] == content[offset]) {
                last_offset = offset;

                if ((offset + keyword_length) > end_before) {
                    return 0;
                }

                ++offset;
                UInt keyword_offset = 1U;

                while ((keyword_offset < keyword_length) &&
                       keyword[keyword_offset] == content[offset]) {
                    ++offset;
                    ++keyword_offset;
                }

                if (keyword_offset == keyword_length) {
                    return offset;
                }

                offset = last_offset;
            }

            ++offset;
        }

        // No match.
        return 0;
    }
#endif

    /*
     * This function will try to find the head and the tail of a match and
     * search aging inside the match to see if it's Nested by itself or a
     * custom expression.
     */
    ULong FindNest(const char *content, ULong offset, ULong end_before,
                   ULong max_end_before) {
        ULong current_offset = offset;

        while (current_offset < end_before) {
            current_offset = find(content, current_offset, end_before);

            if (current_offset == 0) {
                // The head hasn't been found
                break;
            }

            // Save the starting point to be passed to Found().
            ULong start_offset = current_offset;
            // Makes sub-matching possible.
            ULong sub_offset = current_offset;

            if (hasTail()) {
                do {
                    current_offset =
                        find2(content, current_offset, max_end_before);

                    if (current_offset == 0) {
                        // The tail hasn't been found.
                        failed();
                        return ((sub_offset > start_offset) ? sub_offset
                                                            : start_offset);
                    }

                    // Look for sub-matches.
                    sub_offset = nest(content, sub_offset, current_offset,
                                      max_end_before);

                    if (current_offset > sub_offset) {
                        /*
                         * No sub-matches or none of them breaking this match,
                         * So stop and call this a match.
                         */
                        break;
                    }

                    /*
                     * If nest() returns a bigger offset or equal to what the
                     * tail has, then this match is not complete. Adjust the
                     * offset and keep looking.
                     */
                    current_offset = sub_offset;
                } while (current_offset < max_end_before);

                if (current_offset == sub_offset) {
                    return current_offset;
                }
            }

            // A complete/full match is found.
            Found(content, offset, end_before, start_offset, current_offset);
        }

        // Return the location of the last match.
        return current_offset;
    }

    /*
     * Assume that match is about { ... }, but can nest itself like:
     * {.{..{...}..}.}; this function can then be called from nest(), to skip
     * inner brackets:
     *
     * return SkipInnerKeywords("{", 1, "}", 1,
     *                          content, offset, end_before, max_end_before);
     *
     * See Template::nest(...)
     */
    static ULong SkipInnerKeywords(const char *start, UInt start_length,
                                   const char *end, UInt end_length,
                                   const char *content, ULong offset,
                                   ULong end_before, ULong max_end_before) {
        UInt times = 0;

        do {
            // TODO Use Count
            offset = Find(start, start_length, content, offset, end_before);

            if (offset == 0) {
                break;
            }

            ++times;
        } while (true);

        if (times != 0) {
            ULong last_offset = end_before;

            while (--times != 0) {
                end_before =
                    Find(end, end_length, content, last_offset, max_end_before);

                if (end_before == 0) {
                    break;
                }

                last_offset = end_before;
            }

            return last_offset;
        }

        return 0;
    }

    /*
     * Once a full match is found (head & tail), the function will be called.
     * "current_offset" is referenced: it can be adjusted to set
     * the starting offset of the next search.
     */
    virtual void Found(const char *content, ULong offset, ULong end_before,
                       ULong start_offset, ULong &current_offset) {
        // OVERRIDE IS NEEDED WHEN USING FindNest().

        (void)content;
        (void)offset;
        (void)end_before;
        (void)start_offset;
        (void)current_offset;
    }

    //////////// Private ////////////

  private:
    virtual ULong find(const char *content, ULong offset, ULong end_before) = 0;

    // For finding the tail
    virtual ULong find2(const char *content, ULong offset, ULong end_before) {
        // OVERRIDE IS NEEDED WHEN USING FindNest().

        (void)content;
        (void)offset;
        (void)end_before;

        return 0;
    }

    /*
     * This function is called everytime a match found, and should look like the
     * first line that started the search. with the exception of setting
     * different values; like passing a sub-container.
     *
     * FindNest() should be conditioned to check if the
     * remaining length could hold another match between "offset" and
     * "end_before".
     */
    virtual ULong nest(const char *content, ULong offset, ULong end_before,
                       ULong max_end_before) {
        // OVERRIDE IS NEEDED WHEN USING FindNest().

        (void)content;
        (void)offset;
        (void)end_before;
        (void)max_end_before;

        return 0;
    }

    /*
     * If the head of the match is found but not the tail, then this
     * function will be called.
     */
    virtual void failed() {
    }

    /*
     * This is called to check if the match has a second part; a tail.
     * Like head: { and tail: } or () or "" or < > ...etc.
     */
    inline virtual bool hasTail() const noexcept {
        return false;
    }
};

} // namespace Qentem

#endif
