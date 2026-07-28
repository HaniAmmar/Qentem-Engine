/**
 * @file StringStream.hpp
 * @brief Reserver-backed character stream.
 *
 * StringStream provides a dynamically expandable character stream built on
 * StringStreamBase and backed by Reserver-managed storage. It supports efficient
 * sequential writes, string construction, and conversion to Qentem::String
 * while benefiting from allocator-specific growth, shrinking, and expansion
 * capabilities.
 *
 * Unlike LiteStream, which allocates directly from SystemMemory using
 * page-backed storage, StringStream uses Reserver to minimize allocation
 * overhead and support allocator-aware memory management operations.
 *
 * Typical use cases include string building, text formatting, template
 * generation, serialization, and other workloads requiring efficient
 * in-memory character accumulation.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_STRING_STREAM_H
#define QENTEM_STRING_STREAM_H

#include "Qentem/StringStreamBase.hpp"
#include "Qentem/String.hpp"

namespace Qentem {
// Reserver-backed memory provider for StringStreamBase.
struct StringStreamReserverBackend {
    template <typename Char_T>
    QENTEM_INLINE static Char_T *Reserve(SizeT &capacity) {
        capacity = static_cast<SizeT>(Reserver::RoundUpBytes<Char_T>(capacity) / sizeof(Char_T));
        return Reserver::Reserve<Char_T>(capacity);
    }

    template <typename Char_T>
    QENTEM_INLINE static void Release(Char_T *storage, SizeT capacity) {
        Reserver::Release(storage, capacity);
    }

    template <typename Char_T>
    QENTEM_INLINE static bool Shrink(Char_T *storage, SizeT from_size, SizeT to_size) noexcept {
        to_size = static_cast<SizeT>(Reserver::RoundUpBytes<Char_T>(to_size) / sizeof(Char_T));
        return Reserver::Shrink<Char_T>(storage, from_size, to_size);
    }

    template <typename Char_T>
    QENTEM_INLINE static bool TryExpand(Char_T *storage, SizeT from_size, SizeT to_size) noexcept {
        to_size = static_cast<SizeT>(Reserver::RoundUpBytes<Char_T>(to_size) / sizeof(Char_T));
        return Reserver::TryExpand(storage, from_size, to_size);
    }
};

template <typename Char_T, typename MemoryProvider_T = StringStreamReserverBackend>
struct StringStream : public StringStreamBase<Char_T, MemoryProvider_T> {
    using BaseT = StringStreamBase<Char_T, MemoryProvider_T>;
    using BaseT::BaseT;

    using BaseT::Capacity;
    using BaseT::Clear;
    using BaseT::Detach;
    using BaseT::First;
    using BaseT::InsertNull;
    using BaseT::Length;
    using BaseT::shrink;
    using BaseT::Storage;
    using BaseT::Write;
    using BaseT::operator=;
    using BaseT::operator+=;
    using BaseT::operator!=;
    using BaseT::operator==;
    using BaseT::operator<<;

    StringStream &operator=(const String<Char_T> &string) {
        Clear();
        Write(string.First(), string.Length());
        return *this;
    }

    QENTEM_INLINE void operator+=(const String<Char_T> &string) {
        Write(string.First(), string.Length());
    }

    QENTEM_INLINE StringStream &operator<<(const String<Char_T> &string) {
        Write(string.First(), string.Length());
        return *this;
    }

    QENTEM_INLINE bool operator==(const String<Char_T> &string) const noexcept {
        return ((Length() == string.Length()) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    QENTEM_INLINE bool operator!=(const String<Char_T> &string) const noexcept {
        return (!(*this == string));
    }

    String<Char_T> GetString() {
        String<Char_T> new_str{};
        const SizeT    length = Length(); // Detach() resets Length.

        if (Capacity() > Length()) {
            SizeT new_capacity = (Length() + SizeT{1});
            shrink(Storage(), Capacity(), new_capacity);
        }

        InsertNull();
        new_str.Adopt(Detach(), length, length);

        return new_str;
    }
};

} // namespace Qentem

#endif
