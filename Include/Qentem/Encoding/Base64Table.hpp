/**
 * @file Encoding/Base64Table.hpp
 * @brief Defines the character alphabets and padding symbol used by Base64 encoding.
 *
 * @details
 * Provides the standard Base64 alphabet and its URL-safe variant as
 * defined in RFC 4648. These lookup tables are shared by the Base64
 * encoding and decoding implementations.
 *
 * @see RFC 4648
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_ENCODING_BASE64_TABLE_H
#define QENTEM_ENCODING_BASE64_TABLE_H

namespace Qentem {
namespace Encoding {

struct Base64Table {
    inline static const unsigned char Pad = '=';

    /**
     * @brief Standard Base64 encoding alphabet defined by RFC 4648, Table 1.
     *
     * @details
     * Maps 6-bit values in the range [0, 63] to their corresponding
     * Base64 characters. The alphabet consists of uppercase letters,
     * lowercase letters, decimal digits, '+' and '/'.
     */
    inline static const unsigned char Table1[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

    /**
     * @brief URL-safe Base64 alphabet defined by RFC 4648, Table 2.
     *
     * @details
     * Maps 6-bit values from 0 to 63 to the URL-safe Base64 characters.
     * The final two entries are '-' and '_', replacing '+' and '/'
     * from the standard alphabet.
     */
    inline static const unsigned char Table2[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'};
};

} // namespace Encoding
} // namespace Qentem

#endif
