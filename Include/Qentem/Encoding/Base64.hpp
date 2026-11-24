
/**
 * @file Encoding/Base64.hpp
 * @brief Provides Base64 and Base64URL encoding and decoding for binary data.
 *
 * @details
 * Implements the Base64 encoding schemes defined by RFC 4648, supporting
 * both the standard Base64 alphabet and the URL-safe Base64 alphabet.
 *
 * Base64 converts arbitrary binary data into an ASCII-compatible textual
 * representation. Each group of three input bytes (24 bits) is divided
 * into four 6-bit values, which are mapped to characters in a 64-character
 * encoding alphabet.
 *
 * When the final input group contains fewer than three bytes, the output
 * is completed using the '=' padding character. Consequently, the encoded
 * output length is always a multiple of four characters.
 *
 * @par Supported encoding schemes
 *
 * - Standard Base64:
 *   Uses the alphabet defined in RFC 4648, Section 4. The characters at
 *   indices 62 and 63 are '+' and '/', respectively.
 *
 * - Base64URL:
 *   Uses the URL-safe alphabet defined in RFC 4648, Section 5. The
 *   characters at indices 62 and 63 are '-' and '_', respectively.
 *
 *   The URL-safe encoder retains '=' padding.
 *
 * @par Encoding interface
 *
 * Provides two encoding approaches:
 *
 * - One-shot encoding:
 *   Encode() and EncodeURL() process the complete input and finalize
 *   the resulting output using the selected alphabet.
 *
 * - Streaming encoding:
 *   EncodeStream() and EncodeURLStream() accept successive input chunks
 *   while retaining the incomplete encoding group in a caller-owned Record.
 *   Finalize() or FinalizeURL() must be called after the final input chunk
 *   to emit any remaining characters and the required padding.
 *
 * Streaming encoding does not require the complete input to be available
 * at once. The Record retains only the state required to complete the
 * current three-byte input group, avoiding an input-sized intermediate
 * buffer.
 *
 * A Record belongs to a single active encoding operation. The selected
 * alphabet must remain consistent throughout that operation.
 *
 * @par Decoding interface
 *
 * Provides three decoding modes:
 *
 * - Decode() and DecodeStream():
 *   Accept the standard Base64 alphabet only.
 *
 * - DecodeURL() and DecodeURLStream():
 *   Accept the Base64URL alphabet only.
 *
 * - DecodeAny() and DecodeAnyStream():
 *   Accept both standard and URL-safe alphabet characters.
 *   Both alphabets may be used within the same input. The characters
 *   '+' and '-' represent value 62, while '/' and '_' represent value 63.
 *
 * All decoding modes validate the input alphabet, padding placement,
 * and unused trailing bits. Characters outside the accepted alphabet,
 * including whitespace, are not silently ignored.
 *
 * @par Streaming decoding
 *
 * DecodeStream(), DecodeURLStream(), and DecodeAnyStream() retain
 * incomplete decoding state in a caller-owned Record, allowing encoded
 * input to be supplied in successive chunks of arbitrary lengths.
 *
 * Streaming decoding returns a DecodeStatus describing the processing
 * result:
 *
 * - DecodeStatus::Complete:
 *   The supplied input has been processed and the decoder has completed
 *   the current decoding group.
 *
 * - DecodeStatus::Partial:
 *   Additional input is required to complete the current decoding group
 *   or padding sequence.
 *
 * - DecodeStatus::Error:
 *   The input contains an invalid character, malformed padding, or an
 *   invalid encoding sequence.
 *
 * A completed padded group terminates the corresponding Base64 value.
 * The caller is responsible for respecting the encoded message boundary
 * and must not supply additional characters belonging to that value after
 * its padding has been completed.
 *
 * An incomplete final group is reported as DecodeStatus::Partial.
 * In particular, final groups containing fewer than three decoded bytes
 * require the appropriate '=' padding.
 *
 * @par Output handling
 *
 * Decoded bytes and encoded characters are written directly to the
 * caller-provided stream. Completed groups may be written before the
 * entire input has been validated.
 *
 * Consequently, an error encountered later in the input does not
 * automatically roll back output already written to the stream.
 *
 * @par Input requirements
 *
 * Input is treated as a sequence of bytes, not as a null-terminated string.
 * The supplied length specifies the exact number of bytes to process.
 *
 * A valid input buffer must remain readable for the duration of the
 * operation. Binary input, including zero bytes and bytes with the
 * highest bit set, is supported.
 *
 * @par Memory and resource usage
 *
 * The implementation uses fixed-size internal state and does not require
 * an intermediate buffer proportional to the input length.
 *
 * Streaming operations use caller-owned Record objects, allowing multiple
 * independent encoding or decoding operations to maintain separate state.
 * A Record must not be shared between concurrent operations.
 *
 * @note The standard and URL-safe alphabets differ only at indices 62
 *       and 63. Their encoding and decoding operations share the same
 *       underlying processing logic.
 *
 * @note Whitespace and other application-specific separators are not
 *       ignored automatically. Callers that need to decode formatted
 *       Base64 input may manually skip such characters and pass each
 *       contiguous sequence of Base64 characters to DecodeStream(),
 *       DecodeURLStream(), or DecodeAnyStream(), using the same Record
 *       and destination stream throughout the operation.
 *
 *       This allows formatted input, including line-wrapped Base64 data,
 *       to be decoded without modifying or copying the original input.
 *       The '=' padding character must not be skipped, as it is part
 *       of the Base64 encoding and is validated by the decoder.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_ENCODING_BASE64_H
#define QENTEM_ENCODING_BASE64_H

#include "Qentem/Encoding/Base64Table.hpp"
#include "Qentem/StringUtils.hpp"

namespace Qentem {
namespace Encoding {

struct Base64 {
    /**
     * @enum DecodeStatus
     * @brief Describes the result of an incremental Base64 decoding operation.
     *
     * @details
     * Used by the streaming decoding interfaces to indicate whether the
     * supplied input was successfully processed, whether additional input
     * is required, or whether an invalid encoding sequence was encountered.
     *
     * @note Complete indicates that the supplied input has been processed
     *       and the decoder has reached a complete decoding boundary. It
     *       does not prevent subsequent input from being supplied.
     */
    enum struct DecodeStatus : SizeT8 {
        /**
         * @brief The supplied input contains an invalid Base64 sequence.
         *
         * Returned when an unsupported character, invalid padding,
         * or malformed encoding sequence is encountered.
         */
        Error = 0,

        /**
         * @brief The current input is incomplete and requires additional data.
         *
         * The decoder retains its intermediate state in the associated
         * Record, allowing decoding to continue when another input chunk
         * is supplied.
         *
         * This status may indicate an incomplete encoding group or an
         * unfinished padding sequence.
         */
        Partial,

        /**
         * @brief The supplied input has been successfully processed.
         *
         * All complete decoding groups have been processed, and no
         * incomplete group or pending padding sequence remains.
         *
         * Additional input may be supplied through subsequent streaming
         * calls to continue decoding another encoding group.
         */
        Complete
    };

    /**
     * @struct Record
     * @brief Maintains the intermediate state of a streaming Base64 operation.
     *
     * @details
     * Provides fixed-size storage for retaining the intermediate state
     * required by incremental Base64 encoding and decoding.
     *
     * The same structure is used by both operations, with its contents
     * interpreted according to the active operation:
     *
     * - Encoding:
     *   Retains the encoded characters generated from an incomplete
     *   three-byte input group, together with the pending bits required
     *   to complete the next Base64 character.
     *
     * - Decoding:
     *   Retains the decoded bytes being assembled from an incomplete
     *   four-character Base64 group, together with the decoder's
     *   intermediate state.
     *
     * The record is caller-owned and requires no dynamic allocation.
     * It allows successive input chunks to be processed without retaining
     * or buffering the original input data.
     *
     * A record must belong to a single active operation. The caller must
     * consistently use the same operation type and, where applicable,
     * the same Base64 alphabet throughout that operation.
     *
     * @note Records are initialized to an empty state upon default
     *       construction.
     *
     * @warning Do not modify the record's internal fields directly.
     *
     * @see DecodeStatus
     */
    struct Record {
        /**
         * @brief Internal working storage for the current encoding or decoding group.
         *
         * @details
         * Used to retain intermediate Base64 characters, pending bits,
         * decoded bytes, and decoding state, depending on the active operation.
         *
         * The storage is limited to four bytes, corresponding to the
         * maximum size of a Base64 encoding group.
         */
        SizeT8 Output[4]{0};

        /**
         * @brief Current position within the active Base64 group.
         *
         * @details
         * Tracks the progress of the current encoding or decoding group.
         * Its interpretation depends on the active operation:
         *
         * - Encoding: Number of input bytes accumulated in the incomplete
         *   three-byte group.
         *
         * - Decoding: Number of decoded bytes already assembled in the
         *   incomplete four-character group.
         *
         * The value is normally in the range [0, 2] between calls.
         */
        SizeT32 Index{0};

        /**
         * @brief Resets the record to its initial empty state.
         *
         * @details
         * Clears all intermediate storage and resets the group position.
         * Any pending encoding or decoding state is discarded.
         *
         * This operation does not flush pending output to the destination
         * stream.
         *
         * @note Call Reset() when abandoning an incomplete operation or
         *       before reusing a record for an independent operation.
         */
        void Reset() noexcept {
            Index     = 0;
            Output[0] = 0;
            Output[1] = 0;
            Output[2] = 0;
            Output[3] = 0;
        }
    };

    /**
     * @brief Calculates the output length of padded Base64 encoding.
     *
     * @details
     * Calculates the number of characters required to encode an input
     * containing the specified number of bytes using standard padded
     * Base64 encoding.
     *
     * Base64 processes input in groups of three bytes, producing four
     * output characters per group. An incomplete final group is padded
     * to produce a complete four-character output group.
     *
     * The calculation is:
     *
     * @code
     * output_size = (size / 3) * 4 + (size % 3 != 0 ? 4 : 0);
     * @endcode
     *
     * The returned size excludes any null terminator.
     *
     * The result is identical for standard Base64 and Base64URL because
     * both encoding interfaces retain padding.
     *
     * @tparam Number_T Integral type used to represent the input and output sizes.
     *
     * @param size Number of input bytes.
     *
     * @return Number of characters required for the padded Base64 representation.
     *
     * @warning The input size must be non-negative, and the resulting
     *          encoded size must be representable by Number_T.
     */
    template <typename Number_T>
    static constexpr Number_T EncodedSize(Number_T size) noexcept {
        Number_T length = (size / Number_T{3});
        Number_T rem    = (size % Number_T{3});

        return (length * Number_T{4}) + ((rem == 0) ? Number_T{0} : Number_T{4});
    }

    /**
     * @brief Encodes binary data using the standard Base64 alphabet.
     *
     * @details
     * Encodes the entire input using the standard Base64 alphabet defined
     * in RFC 4648, Section 4.
     *
     * The resulting output uses the '+' and '/' characters and includes
     * the required '=' padding for an incomplete final input group.
     *
     * This is a one-shot operation. Intermediate encoding state is
     * managed internally and does not need to be supplied by the caller.
     *
     * @tparam Stream_T Destination stream type.
     *                   Must provide a CharType definition and a compatible
     *                   Write() operation.
     *
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param stream Destination stream receiving the encoded characters.
     * @param data Pointer to the input byte sequence.
     * @param length Number of input bytes to encode.
     *
     * @note The input is treated as binary data. Null bytes and bytes with
     *       their highest bit set are supported.
     *
     * @note This operation is noexcept. The destination stream's Write()
     *       operation must not throw exceptions.
     *
     * @see EncodeStream()
     * @see FinalizeEncode()
     * @see EncodedSize()
     */
    template <typename Stream_T, typename Number_T>
    static void Encode(Stream_T &stream, const void *data, Number_T length) noexcept {
        encode(stream, data, length, &(Base64Table::Table1[0]));
    }

    /**
     * @brief Encodes an input chunk using the standard Base64 alphabet.
     *
     * @details
     * Performs incremental Base64 encoding while retaining the intermediate
     * state in a caller-provided Record.
     *
     * Input is processed as it is received. Complete three-byte groups
     * are encoded and written to the destination stream immediately.
     * An incomplete final group is retained in the record until additional
     * input is supplied or FinalizeEncode() is called.
     *
     * This interface allows arbitrarily large inputs to be encoded in
     * successive chunks without requiring the complete input to be
     * available in memory.
     *
     * The same Record must be supplied for every chunk belonging to
     * the same encoding operation.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param record Caller-owned encoding state.
     * @param stream Destination stream receiving the encoded characters.
     * @param data Pointer to the current input chunk.
     * @param length Number of bytes contained in the current input chunk.
     *
     * @note A zero-length input chunk does not modify the record or stream.
     *
     * @note Call FinalizeEncode() after supplying the final input chunk
     *       to emit the remaining characters and required padding.
     *
     * @warning Do not use the same record for another encoding or decoding
     *          operation while this operation is active.
     *
     * @see Record
     * @see FinalizeEncode()
     * @see Encode()
     */
    template <typename Stream_T, typename Number_T>
    static void EncodeStream(Record &record, Stream_T &stream, const void *data, Number_T length) noexcept {
        if (length != 0) {
            encode(record, stream, data, length, &(Base64Table::Table1[0]));
        }
    }

    /**
     * @brief Completes a standard Base64 streaming encoding operation.
     *
     * @details
     * Processes any remaining input bytes retained in the record and
     * writes the final Base64 characters to the destination stream.
     *
     * If the final input group contains one or two bytes, the output is
     * completed using the required '=' padding characters.
     *
     * If no incomplete input group remains, no additional characters
     * are written.
     *
     * @tparam Stream_T Destination stream type.
     *
     * @param record Encoding state associated with the active streaming operation.
     * @param stream Destination stream receiving the remaining encoded characters.
     *
     * @note This function must be called after the final EncodeStream()
     *       invocation to complete a padded Base64 representation.
     *
     * @warning Use the same record and destination stream associated
     *          with the corresponding EncodeStream() calls.
     *
     * @see EncodeStream()
     * @see Record
     */
    template <typename Stream_T>
    static void FinalizeEncode(Record &record, Stream_T &stream) noexcept {
        finalizeEncode(record, stream, &(Base64Table::Table1[0]));
    }

    /**
     * @brief Decodes binary data from the standard Base64 alphabet.
     *
     * @details
     * Decodes a complete Base64 input using the standard alphabet defined
     * in RFC 4648, Section 4.
     *
     * Only characters belonging to the standard Base64 alphabet and
     * correctly positioned padding characters are accepted.
     * Whitespace and other non-alphabet characters are not ignored.
     *
     * The decoded bytes are written directly to the destination stream.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param stream Destination stream receiving the decoded bytes.
     * @param data Pointer to the encoded input sequence.
     * @param length Number of encoded characters to process.
     *
     * @return true if the supplied input is accepted by the decoder;
     *         false if decoding fails.
     *
     * @note The destination stream may already contain decoded output
     *       if an error is encountered later in the input.
     *
     * @see DecodeStream()
     * @see DecodeURL()
     * @see DecodeAny()
     */
    template <typename Stream_T, typename Number_T>
    static bool Decode(Stream_T &stream, const void *data, Number_T length) noexcept {
        return decode<DecodeType::Standard>(stream, data, length);
    }

    /**
     * @brief Decodes an input chunk using the standard Base64 alphabet.
     *
     * @details
     * Performs incremental Base64 decoding while retaining intermediate
     * decoding state in a caller-provided Record.
     *
     * Complete four-character groups are decoded and written to the
     * destination stream as they are processed. An incomplete group or
     * unfinished padding sequence is retained in the record for subsequent
     * decoding calls.
     *
     * The returned DecodeStatus describes the result of processing the
     * supplied input chunk.
     *
     * The same record must be used for all chunks belonging to the same
     * decoding operation.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param record Caller-owned decoding state.
     * @param stream Destination stream receiving the decoded bytes.
     * @param data Pointer to the current encoded input chunk.
     * @param length Number of encoded characters contained in the chunk.
     *
     * @return DecodeStatus::Complete if the supplied input has been
     *         successfully processed at a complete decoding boundary;
     *         DecodeStatus::Partial if additional input is required;
     *         DecodeStatus::Error if an invalid sequence is encountered.
     *
     * @note DecodeStatus::Complete does not indicate that no additional
     *       input may be supplied. It indicates completion of the current
     *       decoding group.
     *
     * @note Decoded output is written incrementally. Output produced
     *       before an error is not automatically rolled back.
     *
     * @see Record
     * @see DecodeStatus
     * @see Decode()
     */
    template <typename Stream_T, typename Number_T>
    static DecodeStatus DecodeStream(Record &record, Stream_T &stream, const void *data, Number_T length) noexcept {
        return decode<DecodeType::Standard>(record, stream, data, length);
    }

    /**
     * @brief Encodes binary data using the URL-safe Base64 alphabet.
     *
     * @details
     * Encodes the entire input using the URL-safe alphabet defined
     * in RFC 4648, Section 5.
     *
     * The URL-safe alphabet replaces '+' with '-' and '/' with '_',
     * making the resulting representation suitable for use in URL
     * and filename components.
     *
     * Required '=' padding is retained.
     *
     * This is a one-shot operation. Intermediate encoding state is
     * managed internally.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param stream Destination stream receiving the encoded characters.
     * @param data Pointer to the input byte sequence.
     * @param length Number of input bytes to encode.
     *
     * @note Supports arbitrary binary input, including null bytes.
     *
     * @see EncodeURLStream()
     * @see FinalizeEncodeURL()
     * @see Encode()
     */
    template <typename Stream_T, typename Number_T>
    static void EncodeURL(Stream_T &stream, const void *data, Number_T length) noexcept {
        encode(stream, data, length, &(Base64Table::Table2[0]));
    }

    /**
     * @brief Encodes an input chunk using the URL-safe Base64 alphabet.
     *
     * @details
     * Performs incremental Base64URL encoding while retaining intermediate
     * encoding state in a caller-provided Record.
     *
     * Complete three-byte groups are encoded and written immediately.
     * An incomplete final group is retained until additional input is
     * supplied or FinalizeEncodeURL() is called.
     *
     * The same record must be used for every chunk belonging to the
     * same URL-safe encoding operation.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param record Caller-owned encoding state.
     * @param stream Destination stream receiving the encoded characters.
     * @param data Pointer to the current input chunk.
     * @param length Number of bytes contained in the current input chunk.
     *
     * @note A zero-length input chunk does not modify the record or stream.
     *
     * @note The standard and URL-safe alphabets must not be mixed within
     *       a single encoding operation.
     *
     * @see Record
     * @see FinalizeEncodeURL()
     * @see EncodeURL()
     */
    template <typename Stream_T, typename Number_T>
    static void EncodeURLStream(Record &record, Stream_T &stream, const void *data, Number_T length) noexcept {
        if (length != 0) {
            encode(record, stream, data, length, &(Base64Table::Table2[0]));
        }
    }

    /**
     * @brief Completes a URL-safe Base64 streaming encoding operation.
     *
     * @details
     * Encodes any remaining input bytes retained in the record and writes
     * the final URL-safe Base64 characters to the destination stream.
     *
     * An incomplete final group is completed using the required '='
     * padding characters.
     *
     * @tparam Stream_T Destination stream type.
     *
     * @param record Encoding state associated with the active streaming operation.
     * @param stream Destination stream receiving the remaining encoded characters.
     *
     * @see EncodeURLStream()
     * @see Record
     */
    template <typename Stream_T>
    static void FinalizeEncodeURL(Record &record, Stream_T &stream) noexcept {
        finalizeEncode(record, stream, &(Base64Table::Table2[0]));
    }

    /**
     * @brief Decodes binary data from the URL-safe Base64 alphabet.
     *
     * @details
     * Decodes a complete Base64URL input using the alphabet defined
     * in RFC 4648, Section 5.
     *
     * The URL-safe characters '-' and '_' are used in place of '+'
     * and '/' respectively.
     *
     * Only characters belonging to the URL-safe alphabet and correctly
     * positioned padding characters are accepted. Standard Base64
     * special characters are rejected.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param stream Destination stream receiving the decoded bytes.
     * @param data Pointer to the encoded input sequence.
     * @param length Number of encoded characters to process.
     *
     * @return true if the supplied input is accepted by the decoder;
     *         false if decoding fails.
     *
     * @see DecodeURLStream()
     * @see Decode()
     * @see DecodeAny()
     */
    template <typename Stream_T, typename Number_T>
    static bool DecodeURL(Stream_T &stream, const void *data, Number_T length) noexcept {
        return decode<DecodeType::URL>(stream, data, length);
    }

    /**
     * @brief Decodes an input chunk using the URL-safe Base64 alphabet.
     *
     * @details
     * Performs incremental Base64URL decoding while retaining intermediate
     * decoding state in a caller-provided Record.
     *
     * Complete four-character groups are decoded immediately. Incomplete
     * groups and unfinished padding sequences are retained in the record
     * for subsequent calls.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param record Caller-owned decoding state.
     * @param stream Destination stream receiving the decoded bytes.
     * @param data Pointer to the current encoded input chunk.
     * @param length Number of encoded characters contained in the chunk.
     *
     * @return DecodeStatus describing the result of processing the input chunk.
     *
     * @note The same record and decoding mode must be maintained throughout
     *       a streaming operation.
     *
     * @see Record
     * @see DecodeStatus
     * @see DecodeURL()
     */
    template <typename Stream_T, typename Number_T>
    static DecodeStatus DecodeURLStream(Record &record, Stream_T &stream, const void *data, Number_T length) noexcept {
        return decode<DecodeType::URL>(record, stream, data, length);
    }

    /**
     * @brief Decodes Base64 input using either supported Base64 alphabet.
     *
     * @details
     * Accepts characters from both the standard Base64 alphabet and
     * the URL-safe Base64 alphabet.
     *
     * The following character pairs are treated as equivalent:
     *
     * - '+' and '-' represent the value 62.
     * - '/' and '_' represent the value 63.
     *
     * Characters from the two alphabets may be mixed within the same input.
     *
     * All other decoding rules, including character validation, padding
     * placement, and unused trailing bit validation, remain enforced.
     * Whitespace and unrelated characters are not ignored.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param stream Destination stream receiving the decoded bytes.
     * @param data Pointer to the encoded input sequence.
     * @param length Number of encoded characters to process.
     *
     * @return true if the supplied input is accepted by the decoder;
     *         false if decoding fails.
     *
     * @note This mode is intentionally more permissive about the alphabet,
     *       but does not disable Base64 syntax validation.
     *
     * @see Decode()
     * @see DecodeURL()
     * @see DecodeAnyStream()
     */
    template <typename Stream_T, typename Number_T>
    static bool DecodeAny(Stream_T &stream, const void *data, Number_T length) noexcept {
        return decode<DecodeType::Any>(stream, data, length);
    }

    /**
     * @brief Decodes an input chunk using both supported Base64 alphabets.
     *
     * @details
     * Performs incremental decoding while accepting characters from either
     * the standard or URL-safe Base64 alphabet.
     *
     * Standard and URL-safe special characters may appear together in
     * the same input. Both alphabets use identical bit representation,
     * allowing the same decoding state and processing logic to be used.
     *
     * Complete decoding groups are written to the destination stream
     * immediately. Incomplete groups and unfinished padding sequences
     * are retained in the caller-provided Record.
     *
     * @tparam Stream_T Destination stream type.
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param record Caller-owned decoding state.
     * @param stream Destination stream receiving the decoded bytes.
     * @param data Pointer to the current encoded input chunk.
     * @param length Number of encoded characters contained in the chunk.
     *
     * @return DecodeStatus describing the result of processing the input chunk.
     *
     * @note Invalid characters, malformed padding, and noncanonical
     *       trailing bits are still rejected.
     *
     * @see Record
     * @see DecodeStatus
     * @see DecodeAny()
     */
    template <typename Stream_T, typename Number_T>
    static DecodeStatus DecodeAnyStream(Record &record, Stream_T &stream, const void *data, Number_T length) noexcept {
        return decode<DecodeType::Any>(record, stream, data, length);
    }

  private:
    /**
     * @enum DecodeType
     * @brief Identifies the Base64 alphabet accepted by the decoder.
     *
     * @details
     * Selects the character alphabet used during Base64 decoding.
     * The selection is resolved at compile time through template
     * specialization, allowing the decoding implementation to avoid
     * runtime alphabet selection.
     *
     * @var DecodeType::Standard
     * Uses the standard Base64 alphabet defined by RFC 4648, Section 4.
     * The characters '+' and '/' represent the values 62 and 63.
     *
     * @var DecodeType::URL
     * Uses the URL-safe Base64 alphabet defined by RFC 4648, Section 5.
     * The characters '-' and '_' represent the values 62 and 63.
     *
     * @var DecodeType::Any
     * Accepts both standard and URL-safe alphabet characters.
     * The characters '+', '-' represent the value 62, while '/',
     * '_' represent the value 63. Characters from both alphabets
     * may appear within the same input.
     */
    enum struct DecodeType : SizeT8 { Standard = 0, URL, Any };

    /**
     * @brief Encodes an input chunk using the specified Base64 alphabet.
     *
     * @details
     * Performs incremental Base64 encoding using a caller-provided Record
     * to preserve the intermediate state between successive input chunks.
     *
     * Input bytes are processed individually and converted into 6-bit
     * values using bit shifts and masks. Each resulting value is mapped
     * to its corresponding character through the supplied alphabet table.
     *
     * Whenever three input bytes have been accumulated, the resulting
     * four Base64 characters are written together to the destination stream.
     *
     * If the input ends with an incomplete three-byte group, its encoded
     * characters and remaining bits are retained in the Record for
     * subsequent calls or finalization.
     *
     * The Record stores the partially generated output characters in
     * Output[0] through Output[2]. Output[3] serves as temporary storage
     * for the remaining bits required to generate the next Base64 character.
     *
     * This function processes only the supplied input bytes and does not
     * finalize an incomplete group. Padding and the remaining output
     * characters are handled by finalizeEncode().
     *
     * @tparam Stream_T Destination stream type. Must provide a CharType
     *                  definition and a compatible Write() operation.
     *
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param record Caller-owned encoding state. Must be initialized or
     *               contain valid state from a previous encoding call.
     *
     * @param stream Destination stream receiving the encoded characters.
     *
     * @param data Pointer to the input byte sequence.
     *
     * @param length Number of input bytes to encode. Must be greater than zero.
     *
     * @param table Pointer to the 64-character Base64 alphabet used for
     *              converting 6-bit values into encoded characters.
     *              Defaults to the standard Base64 alphabet.
     *
     * @pre The Record must belong to an active encoding operation using
     *      the same alphabet table.
     *
     * @pre The input pointer must reference at least @p length readable bytes.
     *
     * @pre The supplied alphabet table must contain at least 64 entries.
     *
     * @post Every completed three-byte input group is written to the
     *       destination stream as four encoded characters.
     *
     * @post Any incomplete input group remains stored in the Record.
     *
     * @note This function does not append padding or finalize the encoding.
     *       The corresponding finalizeEncode() function must be called
     *       after the final input chunk.
     *
     * @note The implementation uses fixed-size internal state and does
     *       not allocate an intermediate buffer proportional to the input.
     *
     * @note This function assumes that the stream's Write() operation
     *       does not throw exceptions.
     *
     * @warning This function assumes a nonzero input length. Callers must
     *          check for empty input before invoking it.
     *
     * @see Record
     * @see EncodeStream()
     * @see FinalizeEncode()
     * @see encode(Stream_T&, const void*, Number_T, const SizeT8*)
     */
    template <typename Stream_T, typename Number_T>
    static void encode(Record &record, Stream_T &stream, const void *data, Number_T length,
                       const SizeT8 *table = &(Base64Table::Table1[0])) noexcept {
        using Char_T = typename Stream_T::CharType;

        constexpr SizeT32 shift_right[]{2U, 4U, 6U};
        constexpr SizeT32 shift_left[]{4U, 2U, 0};
        constexpr SizeT8  mask[]{0x03, 0x0F, 0x3F};

        const SizeT8 *buffer = (static_cast<const SizeT8 *>(data));

        do {
            const SizeT8 c = *buffer;

            SizeT8 value = c;
            value >>= shift_right[record.Index];
            value |= record.Output[3U];

            record.Output[record.Index] = table[value];

            record.Output[3U] = (c & mask[record.Index]);
            record.Output[3U] <<= shift_left[record.Index];

            --length;
            ++buffer;
            ++record.Index;

            if (record.Index == 3U) {
                record.Output[3U] = table[record.Output[3U]];

                stream.Write(reinterpret_cast<const Char_T *>(&(record.Output[0])), SizeT{4});
                record.Index      = 0;
                record.Output[3U] = 0;
            }
        } while (length != 0);
    }

    /**
     * @brief Encodes binary data using the specified Base64 alphabet.
     *
     * @details
     * Provides a one-shot encoding operation by creating a temporary
     * Record, processing the complete input through the streaming encoder,
     * and finalizing the resulting output.
     *
     * The input is processed in groups of three bytes, with each complete
     * group producing four Base64 characters. If the input contains a
     * remaining group of one or two bytes, the final output is completed
     * using the required '=' padding characters.
     *
     * The supplied alphabet table determines the character representation
     * of each 6-bit value. By selecting the appropriate table, the same
     * implementation supports both standard Base64 and Base64URL encoding.
     *
     * Unlike the streaming interface, this function manages its intermediate
     * state internally. The caller does not need to create or maintain
     * a Record.
     *
     * @tparam Stream_T Destination stream type. Must provide a CharType
     *                  definition and a compatible Write() operation.
     *
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param stream Destination stream receiving the encoded characters.
     *
     * @param data Pointer to the input byte sequence.
     *
     * @param length Number of input bytes to encode. A zero length
     *               produces no output.
     *
     * @param table Pointer to the 64-character Base64 alphabet used to
     *              convert 6-bit values into their corresponding characters.
     *              Defaults to the standard Base64 alphabet.
     *
     * @pre The input pointer must reference at least @p length readable bytes
     *      whenever @p length is nonzero.
     *
     * @pre The supplied alphabet table must contain at least 64 entries.
     *
     * @post The complete encoded representation, including any required
     *       padding, is written to the destination stream.
     *
     * @note The encoded output length is always a multiple of four.
     *
     * @note The output length can be calculated using EncodedSize().
     *
     * @note No intermediate buffer proportional to the input length
     *       is allocated. Only a fixed-size Record is used.
     *
     * @note This function is noexcept. The destination stream's Write()
     *       operation must not throw exceptions.
     *
     * @see Record
     * @see encode(Record&, Stream_T&, const void*, Number_T, const SizeT8*)
     * @see finalizeEncode()
     * @see EncodedSize()
     */
    template <typename Stream_T, typename Number_T>
    static void encode(Stream_T &stream, const void *data, Number_T length,
                       const SizeT8 *table = &(Base64Table::Table1[0])) noexcept {
        if (length != 0) {
            Record record{};
            encode(record, stream, data, length, table);
            finalizeEncode(record, stream, table);
        }
    }

    /**
     * @brief Finalizes an incremental Base64 encoding operation.
     *
     * @details
     * Completes any remaining input group retained in the supplied Record
     * and writes the resulting Base64 characters to the destination stream.
     *
     * A complete input group of three bytes has already been encoded and
     * written by the streaming encoder. Therefore, this function only
     * processes an incomplete final group containing one or two bytes.
     *
     * The remaining bits are converted into the final Base64 character,
     * and the output is completed with the required '=' padding characters:
     *
     * - One remaining input byte produces two Base64 characters followed
     *   by two padding characters.
     *
     * - Two remaining input bytes produce three Base64 characters followed
     *   by one padding character.
     *
     * If the record contains no incomplete group, no additional output
     * is written.
     *
     * After finalization, the Record is reset to its initial empty state,
     * making it available for another independent encoding operation.
     *
     * @tparam Stream_T Destination stream type. Must provide a CharType
     *                  definition and a compatible Write() operation.
     *
     * @param record Encoding state associated with the streaming operation.
     *
     * @param stream Destination stream receiving the remaining encoded characters.
     *
     * @param table Pointer to the same 64-character Base64 alphabet used
     *              during the corresponding encoding operation.
     *              Defaults to the standard Base64 alphabet.
     *
     * @pre The record must contain valid intermediate state produced by
     *      the streaming encoder.
     *
     * @pre The supplied alphabet table must match the alphabet used
     *      throughout the encoding operation.
     *
     * @post Any remaining encoded characters and required padding
     *       are written to the destination stream.
     *
     * @post The record is reset, regardless of whether an incomplete
     *       input group was present.
     *
     * @note Calling this function on an empty record produces no output
     *       and leaves the record in its initial state.
     *
     * @note Calling this function again after successful finalization
     *       produces no additional output.
     *
     * @note The destination stream's Write() operation must not throw
     *       exceptions.
     *
     * @see Record
     * @see encode(Record&, Stream_T&, const void*, Number_T, const SizeT8*)
     * @see EncodeStream()
     * @see FinalizeEncode()
     */
    template <typename Stream_T>
    static void finalizeEncode(Record &record, Stream_T &stream,
                               const SizeT8 *table = &(Base64Table::Table1[0])) noexcept {
        using Char_T = typename Stream_T::CharType;

        if (record.Index != 0) {
            SizeT pad = static_cast<SizeT>(3U - record.Index);

            record.Output[record.Index] = table[record.Output[3U]];

            ++record.Index;

            while (pad != 0) {
                record.Output[record.Index] = Base64Table::Pad;
                ++record.Index;
                --pad;
            }

            stream.Write(reinterpret_cast<const Char_T *>(&(record.Output[0])), record.Index);
        }

        record.Reset();
    }

    /**
     * @brief Decodes an input chunk using the selected Base64 alphabet.
     *
     * @details
     * Performs incremental Base64 decoding using the alphabet selected by
     * the DecodeType template parameter.
     *
     * The decoder converts each accepted Base64 character into its
     * corresponding 6-bit value and progressively combines these values
     * to reconstruct the original input bytes.
     *
     * Each group of four Base64 characters represents three decoded bytes.
     * Complete groups are written to the destination stream immediately,
     * while incomplete groups are retained in the caller-provided Record
     * for processing by subsequent calls.
     *
     * @par Alphabet selection
     *
     * The DecodeType template parameter determines which Base64 alphabet
     * is accepted:
     *
     * - DecodeType::Standard:
     *   Accepts the standard Base64 alphabet defined in RFC 4648, Section 4.
     *   The characters '+' and '/' represent the values 62 and 63.
     *
     * - DecodeType::URL:
     *   Accepts the URL-safe Base64 alphabet defined in RFC 4648, Section 5.
     *   The characters '-' and '_' represent the values 62 and 63.
     *
     * - DecodeType::Any:
     *   Accepts both standard and URL-safe alphabet characters.
     *   The characters '+', '-' represent the value 62, while '/',
     *   '_' represent the value 63. Characters from both alphabets
     *   may appear within the same input.
     *
     * Alphabet selection is resolved at compile time, avoiding runtime
     * selection between the supported decoding modes.
     *
     * @par Decoding state
     *
     * The Record maintains the partially reconstructed output and the
     * state of the current Base64 group:
     *
     * - Output[0] through Output[2] hold the decoded bytes being assembled.
     *
     * - Output[3] maintains the intermediate decoding state:
     *   - 0 indicates that no incomplete group is currently being processed.
     *   - 1 indicates that a decoding group has started.
     *   - 2 indicates that the first required padding character has been
     *     consumed and an additional padding character is expected.
     *
     * - Index tracks the number of decoded bytes assembled in the current
     *   group and not yet written to the destination stream.
     *
     * The record is updated as input is consumed, allowing decoding to
     * continue across successive input chunks without retaining the
     * original encoded characters.
     *
     * @par Padding validation
     *
     * The decoder validates the number and position of padding characters.
     * Padding is accepted only when the current group contains enough
     * Base64 characters to represent at least one decoded byte.
     *
     * - Two Base64 characters require two padding characters.
     * - Three Base64 characters require one padding character.
     * - Four Base64 characters form a complete group and require no padding.
     *
     * Incomplete padding sequences are retained in the Record and reported
     * as DecodeStatus::Partial until the required padding is received.
     *
     * The decoder also validates unused trailing bits to enforce the
     * canonical Base64 representation defined by RFC 4648:
     *
     * - For a final group containing two Base64 characters, the lower four
     *   bits of the second 6-bit value must be zero.
     *
     * - For a final group containing three Base64 characters, the lower
     *   two bits of the third 6-bit value must be zero.
     *
     * @par Input validation
     *
     * Characters outside the selected alphabet are rejected, including
     * whitespace and other non-alphabet characters.
     *
     * Malformed padding, invalid trailing bits, and invalid character
     * sequences result in DecodeStatus::Error.
     *
     * When an error is encountered, the Record is reset to its initial
     * state. Previously written output is not rolled back.
     *
     * @par Return status
     *
     * Returns one of the following DecodeStatus values:
     *
     * - DecodeStatus::Complete:
     *   All characters supplied in the current call have been processed
     *   successfully, and no incomplete decoding group or pending padding
     *   sequence remains in the Record.
     *
     * - DecodeStatus::Partial:
     *   The supplied input has been processed, but additional characters
     *   are required to complete the current decoding group or padding
     *   sequence. The intermediate state is retained in the Record.
     *
     * - DecodeStatus::Error:
     *   An invalid character, malformed padding, or noncanonical encoding
     *   sequence has been encountered. The Record is reset.
     *
     * @note DecodeStatus::Complete describes completion of the supplied
     *       input at a decoding boundary. It does not represent a permanent
     *       end-of-input state.
     *
     * @note An incomplete final group requiring padding is reported as
     *       DecodeStatus::Partial. This function does not finalize an
     *       unpadded input group.
     *
     * @note Complete three-byte groups are written immediately. If an error
     *       is encountered later, previously written bytes remain in the
     *       destination stream.
     *
     * @note This function does not allocate an intermediate buffer
     *       proportional to the input length.
     *
     * @note The input is treated as a sequence of encoded bytes, not as a
     *       null-terminated string. The supplied length determines exactly
     *       how many characters are processed.
     *
     * @tparam Type_T Compile-time decoding alphabet selection.
     *                Must be one of the DecodeType enumerators.
     *
     * @tparam Stream_T Destination stream type. Must provide a CharType
     *                  definition and a compatible Write() operation.
     *
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param record Caller-owned decoding state. Must be initialized or
     *               contain valid state from a previous decoding call.
     *
     * @param stream Destination stream receiving the decoded bytes.
     *
     * @param data Pointer to the encoded input sequence.
     *
     * @param length Number of encoded bytes to process. Must be non-negative.
     *
     * @return DecodeStatus indicating whether decoding completed, requires
     *         additional input, or encountered an error.
     *
     * @pre The Record must belong to an active decoding operation.
     *
     * @pre The input pointer must reference at least @p length readable
     *      bytes whenever @p length is nonzero.
     *
     * @pre The same decoding mode must be used throughout a streaming
     *      operation.
     *
     * @pre The destination stream's Write() operation must not throw
     *      exceptions.
     *
     * @see DecodeType
     * @see DecodeStatus
     * @see Record
     * @see DecodeStream()
     * @see DecodeURLStream()
     * @see DecodeAnyStream()
     * @see decodeChar()
     */
    template <DecodeType Type_T, typename Stream_T, typename Number_T>
    static DecodeStatus decode(Record &record, Stream_T &stream, const void *data, Number_T length) noexcept {
        using Char_T = typename Stream_T::CharType;

        constexpr SizeT32 shift_left[]{2U, 4U, 6U};
        constexpr SizeT32 shift_right[]{4U, 2U, 0U};
        constexpr SizeT8  mask[]{0x3F, 0x0F, 0x03};

        const SizeT8 *buffer = (static_cast<const SizeT8 *>(data));

        while (length != 0) {
            SizeT8 value;

            if (record.Output[3] == 0) {
                record.Output[3] = SizeT8{1};

                value = decodeChar<Type_T>(*buffer);

                if (value == SizeT8{0xFF}) {
                    record.Reset();
                    return DecodeStatus::Error;
                }

                record.Output[0] = value;
                record.Output[0] <<= shift_left[record.Index];

                --length;
                ++buffer;
            }

            while (length != 0) {
                value = decodeChar<Type_T>(*buffer);

                if ((value == SizeT8{0xFF}) || (record.Output[3] == SizeT8{2})) {
                    if (record.Index != 0) {
                        if (record.Index == 1U) {
                            // The lower four bits must be zero.
                            if (record.Output[1] != 0) {
                                record.Reset();
                                return DecodeStatus::Error;
                            }
                        } else if ((record.Index == 2U) && (record.Output[2] != 0)) {
                            // The lower two bits must be zero.
                            record.Reset();

                            return DecodeStatus::Error;
                        }

                        SizeT pad =
                            ((record.Output[3] == SizeT8{1}) ? static_cast<SizeT>(3U - record.Index) : SizeT{1});

                        while ((length != 0) && (pad != 0) && (*buffer == Base64Table::Pad)) {
                            --length;
                            --pad;
                            ++buffer;
                        }

                        if (length == 0) {
                            if (pad == 0) {
                                stream.Write(reinterpret_cast<const Char_T *>(&(record.Output[0])), record.Index);
                                record.Reset();

                                return DecodeStatus::Complete;
                            }

                            if (record.Output[3] == SizeT8{1}) {
                                record.Output[3] += SizeT8{1};

                                return DecodeStatus::Partial;
                            }

                            if (record.Output[3] == SizeT8{2}) {
                                stream.Write(reinterpret_cast<const Char_T *>(&(record.Output[0])), record.Index);
                                record.Reset();

                                return DecodeStatus::Complete;
                            }
                        }
                    }

                    record.Reset();
                    return DecodeStatus::Error;
                }

                record.Output[record.Index] |= (value >> shift_right[record.Index]);

                --length;
                ++record.Index;
                ++buffer;

                if (record.Index == 3U) {
                    stream.Write(reinterpret_cast<const Char_T *>(&(record.Output[0])), record.Index);
                    record.Index     = 0;
                    record.Output[3] = 0;

                    break;
                }

                record.Output[record.Index] = (value & mask[record.Index]);
                record.Output[record.Index] <<= shift_left[record.Index];
            }
        }

        if ((length == 0) && (record.Output[3] == 0)) {
            record.Reset();
            return DecodeStatus::Complete;
        }

        return DecodeStatus::Partial;
    }

    /**
     * @brief Decodes a complete Base64 input using the selected alphabet.
     *
     * @details
     * Provides a one-shot decoding operation by creating a temporary
     * Record and processing the complete encoded input through the
     * incremental decoding implementation.
     *
     * The DecodeType template parameter selects the accepted Base64
     * alphabet:
     *
     * - DecodeType::Standard accepts the standard Base64 alphabet.
     * - DecodeType::URL accepts the URL-safe Base64 alphabet.
     * - DecodeType::Any accepts both alphabets, including mixed input.
     *
     * The decoded bytes are written directly to the destination stream.
     * No intermediate buffer proportional to the input length is allocated.
     *
     * Unlike the streaming interface, this function manages its decoding
     * state internally and requires the complete encoded input to be
     * supplied in a single call.
     *
     * @par Return value
     *
     * Returns true only when the decoding operation completes successfully.
     * Returns false when the input contains an invalid sequence or when
     * additional input would be required to complete the encoding.
     *
     * Consequently, incomplete final groups requiring padding are not
     * accepted by this one-shot interface.
     *
     * @tparam Type_T Compile-time selection of the accepted Base64 alphabet.
     *
     * @tparam Stream_T Destination stream type. Must provide a CharType
     *                  definition and a compatible Write() operation.
     *
     * @tparam Number_T Integral type used to represent the input length.
     *
     * @param stream Destination stream receiving the decoded bytes.
     *
     * @param data Pointer to the encoded input sequence.
     *
     * @param length Number of encoded bytes to process. Must be non-negative.
     *
     * @return true if the complete input is successfully decoded;
     *         false if decoding fails or requires additional input.
     *
     * @pre The input pointer must reference at least @p length readable
     *      bytes whenever @p length is nonzero.
     *
     * @pre The destination stream's Write() operation must not throw
     *      exceptions.
     *
     * @note An empty input is considered successfully decoded.
     *
     * @note Complete decoding groups are written immediately. If an error
     *       is encountered later, previously written output is not rolled back.
     *
     * @note The temporary Record is local to this operation and is discarded
     *       after decoding.
     *
     * @see Decode()
     * @see DecodeURL()
     * @see DecodeAny()
     * @see DecodeStream()
     * @see Record
     * @see DecodeStatus
     */
    template <DecodeType Type_T, typename Stream_T, typename Number_T>
    static bool decode(Stream_T &stream, const void *data, Number_T length) noexcept {
        Record record{};

        return (decode<Type_T>(record, stream, data, length) == DecodeStatus::Complete);
    }

    /**
     * @brief Converts a Base64 character into its corresponding 6-bit value.
     *
     * @details
     * Converts an ASCII Base64 character into its numerical value in the
     * range [0, 63] using character-range comparisons and arithmetic.
     *
     * The conversion does not require a reverse lookup table.
     *
     * The alphanumeric characters are converted using their ASCII values:
     *
     * - 'A' through 'Z' map to values 0 through 25.
     * - 'a' through 'z' map to values 26 through 51.
     * - '0' through '9' map to values 52 through 61.
     *
     * The remaining values depend on the selected DecodeType:
     *
     * - DecodeType::Standard:
     *   '+' maps to 62 and '/' maps to 63.
     *
     * - DecodeType::URL:
     *   '-' maps to 62 and '_' maps to 63.
     *
     * - DecodeType::Any:
     *   Both '+' and '-' map to 62, while both '/' and '_' map to 63.
     *
     * Alphabet selection is resolved at compile time through the DecodeType
     * template parameter.
     *
     * Characters that do not belong to the selected alphabet are rejected.
     *
     * @par Invalid character representation
     *
     * Returns 0xFF when the supplied character is not valid for the selected
     * alphabet.
     *
     * The sentinel value is outside the valid Base64 range and can therefore
     * be used by the decoding implementation to detect invalid input.
     *
     * @note The padding character '=' is intentionally not handled here.
     *       Padding is processed separately by the decoding state machine,
     *       which validates its position and quantity.
     *
     * @note This conversion assumes ASCII-compatible character values.
     *
     * @tparam Type_T Compile-time selection of the accepted Base64 alphabet.
     *
     * @param c Unsigned byte containing the Base64 character to convert.
     *
     * @return The corresponding 6-bit Base64 value in the range [0, 63],
     *         or 0xFF if the character is not part of the selected alphabet.
     *
     * @see DecodeType
     * @see decode()
     */
    template <DecodeType Type_T>
    static constexpr SizeT8 decodeChar(SizeT8 c) noexcept {
        if ((c >= SizeT8{'A'}) && (c <= SizeT8{'Z'})) {
            return (c - SizeT8{'A'});
        }

        if ((c >= SizeT8{'a'}) && (c <= SizeT8{'z'})) {
            return (c - SizeT8{'G'});
        }

        if ((c >= SizeT8{'0'}) && (c <= SizeT8{'9'})) {
            return (c + SizeT8{4});
        }

        if constexpr (Type_T == DecodeType::Standard) {
            if (c == SizeT8{'+'}) {
                return SizeT8{62};
            }

            if (c == SizeT8{'/'}) {
                return SizeT8{63};
            }
        } else if constexpr (Type_T == DecodeType::URL) {
            if (c == SizeT8{'-'}) {
                return SizeT8{62};
            }

            if (c == SizeT8{'_'}) {
                return SizeT8{63};
            }
        } else {
            if ((c == SizeT8{'+'}) || (c == SizeT8{'-'})) {
                return SizeT8{62};
            }

            if ((c == SizeT8{'/'}) || (c == SizeT8{'_'})) {
                return SizeT8{63};
            }
        }

        return SizeT8{0xFF};
    }
};

} // namespace Encoding
} // namespace Qentem

#endif
