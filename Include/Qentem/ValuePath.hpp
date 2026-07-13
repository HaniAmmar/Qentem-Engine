/**
 * @file ValuePath.hpp
 * @brief Provides path-based access helpers for Value objects.
 *
 * ValuePath contains utility functions for traversing nested Value
 * structures and retrieving values through hierarchical paths. It also
 * provides convenience functions for safely extracting strings and numeric
 * values while handling missing nodes gracefully.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef WEBSITE_VALUE_PATH_H
#define WEBSITE_VALUE_PATH_H

#include "Qentem/Value.hpp"

namespace Qentem {

struct ValuePath {
    /**
     * @brief Retrieves a nested value using a two-level path.
     *
     * Searches for a child value identified by @p name1 within @p container,
     * then searches for a child value identified by @p name2 within the
     * resulting value.
     *
     * @tparam Value_T Value type.
     * @param container Root value to search.
     * @param name1 First path component.
     * @param name2 Second path component.
     *
     * @return Pointer to the matching value, or nullptr if any path component
     *         cannot be resolved.
     */
    template <typename Value_T>
    QENTEM_NOINLINE static const Value_T *GetValue(const Value_T &container, const typename Value_T::StringViewT &name1,
                                                   const typename Value_T::StringViewT &name2) noexcept {
        const Value_T *val = container.GetValue(name1);

        if (val != nullptr) {
            val = val->GetValue(name2);
        }

        return val;
    }

    template <typename Value_T>
    QENTEM_NOINLINE static const Value_T *GetValue(const Value_T &container, const typename Value_T::StringViewT &name1,
                                                   const typename Value_T::StringViewT &name2,
                                                   const typename Value_T::StringViewT &name3) noexcept {
        const Value_T *val = container.GetValue(name1);

        if (val != nullptr) {
            val = val->GetValue(name2);

            if (val != nullptr) {
                val = val->GetValue(name3);
            }
        }

        return val;
    }

    template <typename Value_T>
    QENTEM_NOINLINE static const Value_T *GetValue(const Value_T &container, const typename Value_T::StringViewT &name1,
                                                   const typename Value_T::StringViewT &name2,
                                                   const typename Value_T::StringViewT &name3,
                                                   const typename Value_T::StringViewT &name4) noexcept {
        const Value_T *val = container.GetValue(name1);

        if (val != nullptr) {
            val = val->GetValue(name2);

            if (val != nullptr) {
                val = val->GetValue(name3);

                if (val != nullptr) {
                    val = val->GetValue(name4);
                }
            }
        }

        return val;
    }

    template <typename Value_T>
    QENTEM_NOINLINE static const typename Value_T::StringViewT
    GetStringView(const Value_T &container, const typename Value_T::StringViewT &name) noexcept {
        const Value_T *val = container.GetValue(name);

        if (val != nullptr) {
            return val->GetStringView();
        }

        return typename Value_T::StringViewT{};
    }

    template <typename Value_T>
    QENTEM_NOINLINE static const typename Value_T::StringViewT
    GetStringView(const Value_T &container, const typename Value_T::StringViewT &name1,
                  const typename Value_T::StringViewT &name2) noexcept {
        const Value_T *val = container.GetValue(name1);

        if (val != nullptr) {
            val = val->GetValue(name2);

            if (val != nullptr) {
                return val->GetStringView();
            }
        }

        return typename Value_T::StringViewT{};
    }

    template <typename Value_T>
    QENTEM_NOINLINE static const typename Value_T::StringViewT
    GetStringView(const Value_T &container, const typename Value_T::StringViewT &name1,
                  const typename Value_T::StringViewT &name2, const typename Value_T::StringViewT &name3) noexcept {
        const Value_T *val = container.GetValue(name1);

        if (val != nullptr) {
            val = val->GetValue(name2);

            if (val != nullptr) {
                val = val->GetValue(name3);

                if (val != nullptr) {
                    return val->GetStringView();
                }
            }
        }

        return typename Value_T::StringViewT{};
    }

    template <typename Value_T>
    QENTEM_NOINLINE static const typename Value_T::StringViewT
    GetStringView(const Value_T &container, const typename Value_T::StringViewT &name1,
                  const typename Value_T::StringViewT &name2, const typename Value_T::StringViewT &name3,
                  const typename Value_T::StringViewT &name4) noexcept {
        const Value_T *val = container.GetValue(name1);

        if (val != nullptr) {
            val = val->GetValue(name2);

            if (val != nullptr) {
                val = val->GetValue(name3);

                if (val != nullptr) {
                    val = val->GetValue(name4);

                    if (val != nullptr) {
                        return val->GetStringView();
                    }
                }
            }
        }

        return typename Value_T::StringViewT{};
    }

    template <typename Value_T>
    QENTEM_NOINLINE static SizeT64 GetUInt64(const Value_T                       &container,
                                             const typename Value_T::StringViewT &name) noexcept {
        const Value_T *val = container.GetValue(name);

        if (val != nullptr) {
            return val->GetUInt64();
        }

        return 0;
    }

    template <typename Value_T>
    QENTEM_NOINLINE static SizeT64I GetInt64(const Value_T                       &container,
                                             const typename Value_T::StringViewT &name) noexcept {
        const Value_T *val = container.GetValue(name);

        if (val != nullptr) {
            return val->GetInt64();
        }

        return 0;
    }

    template <typename Value_T>
    QENTEM_NOINLINE static const typename Value_T::StringViewT GetStringView(const Value_T &container,
                                                                             SizeT32        index) noexcept {
        const Value_T *val = container.GetValueAt(index);

        if (val != nullptr) {
            return val->GetStringView();
        }

        return typename Value_T::StringViewT{};
    }

    template <typename Value_T>
    QENTEM_NOINLINE static SizeT64 GetUInt64(const Value_T &container, SizeT32 index) noexcept {
        const Value_T *val = container.GetValueAt(index);

        if (val != nullptr) {
            return val->GetUInt64();
        }

        return 0;
    }

    template <typename Value_T>
    QENTEM_NOINLINE static SizeT64I GetInt64(const Value_T &container, SizeT32 index) noexcept {
        const Value_T *val = container.GetValueAt(index);

        if (val != nullptr) {
            return val->GetInt64();
        }

        return 0;
    }
};

} // namespace Qentem

#endif
