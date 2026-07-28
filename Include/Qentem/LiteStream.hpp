/**
 * @file LiteStream.hpp
 * @brief Page-backed character stream.
 *
 * LiteStream provides the StringStream interface using a page-based memory
 * backend. Storage is allocated directly from SystemMemory and expanded in
 * page-sized increments, making it suitable for sequential output workloads
 * where predictable memory usage and minimal allocator overhead are desired.
 *
 * Unlike StringStream, which uses Reserver for allocation and supports
 * allocator-specific optimizations, LiteStream relies exclusively on
 * page-backed storage and does not support in-place shrinking or expansion.
 *
 * Typical use cases include logging, diagnostics, temporary text generation,
 * and other write-oriented workloads where direct system memory allocation
 * is preferred.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_LITE_STREAM_H
#define QENTEM_LITE_STREAM_H

#include "Qentem/StringStreamBase.hpp"
#include "Qentem/SystemMemory.hpp"
#include "Qentem/Platform.hpp"

namespace Qentem {
// Page-backed memory provider for StringStreamBase.
struct StringStreamPageBackend {
    template <typename Char_T>
    QENTEM_INLINE static Char_T *Reserve(SizeT &capacity) {
        SizeT capacity_bytes = (capacity * sizeof(Char_T));

#ifndef QENTEM_SYSTEM_MEMORY_FALLBACK
        if (capacity_bytes > SystemMemory::GetPageSize()) {
            capacity_bytes = SystemMemory::AlignToPageSize(capacity_bytes);
        } else {
            capacity_bytes = static_cast<SizeT>(SystemMemory::GetPageSize());
        }

        capacity = (capacity_bytes / sizeof(Char_T));
#endif

        return static_cast<Char_T *>(SystemMemory::Reserve(capacity_bytes));
    }

    template <typename Char_T>
    QENTEM_INLINE static void Release(Char_T *storage, SizeT capacity) {
        SystemMemory::Release(storage, (capacity * sizeof(Char_T)));
    }

    template <typename Char_T>
    QENTEM_INLINE constexpr static bool Shrink(Char_T *, SizeT, SizeT) noexcept {
        return false;
    }

    template <typename Char_T>
    QENTEM_INLINE constexpr static bool TryExpand(Char_T *, SizeT, SizeT) noexcept {
        return false;
    }
};

struct LiteStream : public StringStreamBase<char, StringStreamPageBackend> {
    using BaseT = StringStreamBase<char, StringStreamPageBackend>;
    using BaseT::BaseT;
};

} // namespace Qentem

#endif
