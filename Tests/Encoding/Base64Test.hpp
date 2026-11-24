/*
 * Copyright (c) 2026 Hani Ammar
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

#ifndef QENTEM_ENCODING_BASE64_TESTS_H
#define QENTEM_ENCODING_BASE64_TESTS_H

#include "Qentem/QTest.hpp"
#include "Qentem/Encoding/Base64.hpp"
#include "Qentem/StringStream.hpp"

namespace Qentem {
namespace Test {
namespace Encoding {
using namespace Qentem::Encoding;

static void TestBase64EncodedSize(QTest &test) {
    test.IsEqual(Base64::EncodedSize(0), 0, __LINE__);
    test.IsEqual(Base64::EncodedSize(1), 4, __LINE__);
    test.IsEqual(Base64::EncodedSize(2), 4, __LINE__);
    test.IsEqual(Base64::EncodedSize(3), 4, __LINE__);
    test.IsEqual(Base64::EncodedSize(4), 8, __LINE__);
    test.IsEqual(Base64::EncodedSize(5), 8, __LINE__);
    test.IsEqual(Base64::EncodedSize(6), 8, __LINE__);
    test.IsEqual(Base64::EncodedSize(7), 12, __LINE__);
    test.IsEqual(Base64::EncodedSize(8), 12, __LINE__);
    test.IsEqual(Base64::EncodedSize(9), 12, __LINE__);
    test.IsEqual(Base64::EncodedSize(10), 16, __LINE__);
    test.IsEqual(Base64::EncodedSize(11), 16, __LINE__);
    test.IsEqual(Base64::EncodedSize(12), 16, __LINE__);
}

static void TestBase64Encode(QTest &test, StringStream<char> &stream) {
    StringView<char> in{""};
    StringView<char> out{};
    Base64::Record   record{};

    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream.Length(), SizeT{0}, __LINE__);

    in  = "a";
    out = "YQ==";
    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "ao";
    out = "YW8=";
    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "aox";
    out = "YW94";
    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "Woob";
    out = "V29vYg==";
    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "~!@#$";
    out = "fiFAIyQ=";
    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "%^&*()";
    out = "JV4mKigp";
    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "abcdefghigklmnobqrs123456789";
    out = "YWJjZGVmZ2hpZ2tsbW5vYnFyczEyMzQ1Njc4OQ==";
    Base64::Encode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    const SizeT8 zero_byte[] = {0x00};
    out                      = "AA==";
    Base64::Encode(stream, zero_byte, sizeof(zero_byte));
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    const SizeT8 two_bytes[] = {0x00, 0x01};
    out                      = "AAE=";
    Base64::Encode(stream, two_bytes, sizeof(two_bytes));
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    const SizeT8 data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
    out                 = "AAECAwQFBgcICQoLDA==";
    Base64::Encode(stream, data, sizeof(data));
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    SizeT8 all_bytes[256];

    for (SizeT i = 0; i < SizeT{256}; ++i) {
        all_bytes[i] = static_cast<SizeT8>(i);
    }

    out = "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4"
          "OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3Bx"
          "cnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmq"
          "q6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj"
          "5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==";

    Base64::Encode(stream, all_bytes, sizeof(all_bytes));
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();
    /////////////////////////////////////////////////
    for (SizeT i = 0; i < SizeT{256}; ++i) {
        SizeT8 c = static_cast<SizeT8>(i);
        Base64::EncodeStream(record, stream, &c, 1U);
    }

    Base64::FinalizeEncode(record, stream);
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();
    /////////////////////////////////////////////////

    out = "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4"
          "OTo7PD0-P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3Bx"
          "cnN0dXZ3eHl6e3x9fn-AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmq"
          "q6ytrq-wsbKztLW2t7i5uru8vb6_wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t_g4eLj"
          "5OXm5-jp6uvs7e7v8PHy8_T19vf4-fr7_P3-_w==";

    Base64::EncodeURL(stream, all_bytes, sizeof(all_bytes));
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    /////////////////////////////////////////////////
    for (SizeT i = 0; i < SizeT{256}; ++i) {
        SizeT8 c = static_cast<SizeT8>(i);
        Base64::EncodeURLStream(record, stream, &c, 1U);
    }

    Base64::FinalizeEncodeURL(record, stream);
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();
    /////////////////////////////////////////////////
}

static void TestBase64Decode(QTest &test, StringStream<char> &stream) {
    StringView<char>     in{""};
    StringView<char>     out{};
    Base64::Record       record{};
    Base64::DecodeStatus status{};

    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream.Length(), SizeT{0}, __LINE__);

    in = "A";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "A=";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "YQ=";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "AAA==";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "AB==";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "AAB==";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "YQ=a";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "YQ==a";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "Zm8==";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in = "AA==AAAA";
    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    stream.Clear();

    in  = "YQ==";
    out = "a";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    ///////////////////////////////////////////////
    for (SizeT i = 0; i < in.Length(); ++i) {
        status = Base64::DecodeStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsTrue(status == Base64::DecodeStatus::Complete, __LINE__);
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    for (SizeT i = 0; i < in.Length(); ++i) {
        status = Base64::DecodeStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsTrue(status == Base64::DecodeStatus::Complete, __LINE__);
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();
    ///////////////////////////////////////////////

    status = Base64::DecodeStream(record, stream, "Zg=", 3);
    test.IsTrue(status == Base64::DecodeStatus::Partial, __LINE__);
    status = Base64::DecodeStream(record, stream, "A", 1);
    test.IsTrue(status == Base64::DecodeStatus::Error, __LINE__);
    stream.Clear();

    out    = "f";
    status = Base64::DecodeStream(record, stream, "Zg=", SizeT{3});
    test.IsTrue(status == Base64::DecodeStatus::Partial, __LINE__);
    status = Base64::DecodeStream(record, stream, "=", SizeT{1});
    test.IsTrue(status == Base64::DecodeStatus::Complete, __LINE__);
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    status = Base64::DecodeStream(record, stream, "Zg=", 3);
    test.IsTrue(status == Base64::DecodeStatus::Partial, __LINE__);
    status = Base64::DecodeStream(record, stream, "==", 2);
    test.IsTrue(status == Base64::DecodeStatus::Error, __LINE__);
    stream.Clear();

    status = Base64::DecodeStream(record, stream, "A===", 4);
    test.IsTrue(status == Base64::DecodeStatus::Error, __LINE__);
    stream.Clear();

    // One call:
    status = Base64::DecodeStream(record, stream, "Zg==AAAA", 8);
    test.IsTrue(status == Base64::DecodeStatus::Error, __LINE__);
    stream.Clear();

    out = "faox";
    // Two calls:
    status = Base64::DecodeStream(record, stream, "Zg==", 4);
    test.IsTrue(status == Base64::DecodeStatus::Complete, __LINE__);
    status = Base64::DecodeStream(record, stream, "YW94", 4);
    test.IsTrue(status == Base64::DecodeStatus::Complete, __LINE__);
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "YW8=";
    out = "ao";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "YW94";
    out = "aox";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "V29vYg==";
    out = "Woob";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "fiFAIyQ=";
    out = "~!@#$";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "JV4mKigp";
    out = "%^&*()";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    in  = "YWJjZGVmZ2hpZ2tsbW5vYnFyczEyMzQ1Njc4OQ==";
    out = "abcdefghigklmnobqrs123456789";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    ///////////////////////////////////////////////
    for (SizeT i = 0; i < in.Length(); ++i) {
        Base64::DecodeStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsEqual(stream, out, __LINE__);
    stream.Clear();

    for (SizeT i = 0; i < in.Length(); ++i) {
        Base64::DecodeAnyStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsEqual(stream, out, __LINE__);
    stream.Clear();
    ///////////////////////////////////////////////

    const char zero_byte[] = {0x00};
    in                     = "AA==";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(zero_byte[0]), SizeT{1}}, __LINE__);
    stream.Clear();

    const char two_bytes[] = {0x00, 0x01};
    in                     = "AAE=";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(two_bytes[0]), SizeT{2}}, __LINE__);
    stream.Clear();

    const char data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
    in                = "AAECAwQFBgcICQoLDA==";
    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream.Length(), SizeT{13}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(data[0]), SizeT{13}}, __LINE__);
    stream.Clear();

    char all_bytes[256];

    for (SizeT i = 0; i < SizeT{256}; ++i) {
        all_bytes[i] = static_cast<char>(i);
    }

    in = "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4"
         "OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3Bx"
         "cnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmq"
         "q6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj"
         "5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==";

    Base64::Decode(stream, in.First(), in.Length());
    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();

    ///////////////////////////////////////////////
    for (SizeT i = 0; i < in.Length(); ++i) {
        Base64::DecodeStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();
    ///////////////////////////////////////////////
    for (SizeT i = 0; i < in.Length(); ++i) {
        Base64::DecodeAnyStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();
    ///////////////////////////////////////////////

    Base64::DecodeAny(stream, in.First(), in.Length());
    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();

    in = "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4"
         "OTo7PD0-P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3Bx"
         "cnN0dXZ3eHl6e3x9fn-AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmq"
         "q6ytrq-wsbKztLW2t7i5uru8vb6_wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t_g4eLj"
         "5OXm5-jp6uvs7e7v8PHy8_T19vf4-fr7_P3-_w==";

    test.IsFalse(Base64::Decode(stream, in.First(), in.Length()), __LINE__);
    test.IsNotEqual(stream.Length(), SizeT{256}, __LINE__);
    stream.Clear();

    test.IsTrue(Base64::DecodeURL(stream, in.First(), in.Length()), __LINE__);
    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();

    test.IsTrue(Base64::DecodeAny(stream, in.First(), in.Length()), __LINE__);
    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();

    ///////////////////////////////////////////////
    for (SizeT i = 0; i < in.Length(); ++i) {
        Base64::DecodeURLStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();
    ///////////////////////////////////////////////
    for (SizeT i = 0; i < in.Length(); ++i) {
        Base64::DecodeAnyStream(record, stream, (in.First() + i), SizeT{1});
    }

    test.IsEqual(stream.Length(), SizeT{256}, __LINE__);
    test.IsEqual(stream, StringView<char>{&(all_bytes[0]), SizeT{256}}, __LINE__);
    stream.Clear();
    //////////////////////////////////////////////////////////////////////////////////////////////
}

static int RunBase64Tests() {
    QTest test{"Encoding/Base64.hpp", __FILE__};

    test.PrintGroupName();

    StringStream<char> stream{512};

    test.Test("Base64 EncodedSize Test", TestBase64EncodedSize, false);
    test.Test("Base64 Encode Test", TestBase64Encode, false, stream);
    test.Test("Base64 Decode Test", TestBase64Decode, false, stream);

    return test.EndTests();
}

} // namespace Encoding
} // namespace Test
} // namespace Qentem

#endif
