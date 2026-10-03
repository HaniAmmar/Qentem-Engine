/**
 * @file LiteStream.hpp
 * @brief Page-backed character stream.
 *
 * LiteStream provides the StringStream interface using a page-based
 * SystemMemory backend. Storage is allocated directly from SystemMemory and
 * sized according to the system page size, providing predictable memory
 * behavior with minimal allocation overhead.
 *
 * On supported platforms, unused pages can be released when the stream
 * shrinks, while Linux can attempt to expand the existing mapping in place.
 * When an in-place resize is unavailable or fails, normal reallocation can
 * be used instead.
 *
 * Typical use cases include logging, diagnostics, temporary text generation,
 * and other write-oriented workloads where direct system memory allocation
 * is preferred.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
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
        if (storage != nullptr) {
            const SizeT size_bytes = SystemMemory::AlignToPageSize<SizeT>(capacity * sizeof(Char_T));

            SystemMemory::Release(storage, static_cast<SystemLong>(size_bytes));
        }
    }

    template <typename Char_T>
    QENTEM_INLINE static bool Shrink(Char_T *storage, SizeT from_size, SizeT &to_size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK) && !defined(_WIN32)
        if (storage != nullptr) {
            const SizeT from_size_bytes = SystemMemory::AlignToPageSize<SizeT>(from_size * sizeof(Char_T));
            const SizeT to_size_bytes   = SystemMemory::AlignToPageSize<SizeT>(to_size * sizeof(Char_T));

            if ((to_size_bytes < from_size_bytes) &&
                SystemMemory::ReleasePages((reinterpret_cast<char *>(storage) + to_size_bytes),
                                           static_cast<SystemLong>(from_size_bytes - to_size_bytes))) {
                to_size = (to_size_bytes / sizeof(Char_T));

                return true;
            }
        }
#else
        (void)storage;
        (void)from_size;
        (void)to_size;
#endif
        return false;
    }

    template <typename Char_T>
    QENTEM_INLINE static bool TryExpand(Char_T *storage, SizeT from_size, SizeT &to_size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK) && defined(__linux__)
        if (storage != nullptr) {
            const SizeT from_size_bytes = SystemMemory::AlignToPageSize<SizeT>(from_size * sizeof(Char_T));
            const SizeT to_size_bytes   = SystemMemory::AlignToPageSize<SizeT>(to_size * sizeof(Char_T));

            if ((to_size_bytes > from_size_bytes) &&
                SystemMemory::ExpandPages(storage, from_size_bytes, to_size_bytes)) {
                to_size = (to_size_bytes / sizeof(Char_T));

                return true;
            }
        }
#else
        (void)storage;
        (void)from_size;
        (void)to_size;
#endif
        return false;
    }
};

struct LiteStream : public StringStreamBase<char, StringStreamPageBackend> {
    using BaseT = StringStreamBase<char, StringStreamPageBackend>;
    using BaseT::BaseT;
};

} // namespace Qentem

#endif
