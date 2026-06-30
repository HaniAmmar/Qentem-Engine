/**
 * @file VariableTag.hpp
 * @brief Defines the VariableTag struct used for variable offsets in Qentem templates.
 *
 * This header exists independently to avoid dependency cycles between Tags.hpp and Template.hpp.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_VARIABLE_TAG_H
#define QENTEM_VARIABLE_TAG_H

#include "Qentem/Reserver.hpp"

namespace Qentem {
namespace Tags {

struct VariableInfo {
    SizeT Offset{0}; ///< Offset of the variable within the template.
    SizeT Hash{0};
};

/**
 * @brief Represents the metadata for a template variable.
 */
struct VariableTag {
    QENTEM_INLINE VariableTag() {
        if constexpr (sizeof(void *) >= sizeof(VariableInfo)) {
            List = nullptr;
        } else {
            Info.Offset = 0;
            Info.Hash   = 0;
        }
    }

    QENTEM_INLINE VariableTag(VariableTag &&src) noexcept
        : Count{src.Count}, Length{src.Length}, IDLength{src.IDLength}, Level{src.Level} {
        if constexpr (sizeof(void *) >= sizeof(VariableInfo)) {
            List     = src.List;
            src.List = nullptr;
        } else {
            Info.Offset = src.Info.Offset;
            Info.Hash   = src.Info.Hash;
        }

        src.Count = 0;
    }

    QENTEM_INLINE VariableTag(const VariableTag &src)
        : Count{src.Count}, Length{src.Length}, IDLength{src.IDLength}, Level{src.Level} {
        if constexpr (sizeof(void *) >= sizeof(VariableInfo)) {
            List = src.List;
        } else {
            Info.Offset = src.Info.Offset;
            Info.Hash   = src.Info.Hash;
        }

        if (src.Count > 1) {
            List = Reserver::Reserve<VariableInfo>(static_cast<SystemLong>(src.Count));

            SizeT index = 0;

            while (index < src.Count) {
                List[index].Offset = src.List[index].Offset;
                List[index].Hash   = src.List[index].Hash;
                ++index;
            }
        }
    }

    ~VariableTag() {
        if (Count > 1) {
            Reserver::Release(List, static_cast<SystemLong>(Count));
        }
    }

    VariableTag &operator=(VariableTag &&src) noexcept {
        if (this != &src) {
            if (Count > 1) {
                Reserver::Release(List, static_cast<SystemLong>(Count));
            }

            if constexpr (sizeof(void *) >= sizeof(VariableInfo)) {
                List     = src.List;
                src.List = nullptr;
            } else {
                Info.Offset = src.Info.Offset;
                Info.Hash   = src.Info.Hash;
            }

            Count    = src.Count;
            Length   = src.Length;
            IDLength = src.IDLength;
            Level    = src.Level;

            src.Count = 0;
        }

        return *this;
    }

    VariableTag &operator=(const VariableTag &src) {
        if (this != &src) {
            if (Count > 1) {
                Reserver::Release(List, static_cast<SystemLong>(Count));
            }

            if constexpr (sizeof(void *) >= sizeof(VariableInfo)) {
                List = src.List;
            } else {
                Info.Offset = src.Info.Offset;
                Info.Hash   = src.Info.Hash;
            }

            if (src.Count > 1) {
                List = Reserver::Reserve<VariableInfo>(static_cast<SystemLong>(src.Count));

                SizeT index = 0;

                while (index < src.Count) {
                    List[index].Offset = src.List[index].Offset;
                    List[index].Hash   = src.List[index].Hash;
                    ++index;
                }
            }

            Count    = src.Count;
            Length   = src.Length;
            IDLength = src.IDLength;
            Level    = src.Level;
        }

        return *this;
    }

    union {
        VariableInfo *List;
        VariableInfo  Info{};
    };

    SizeT8 Count{0};    ///< Number of segments.
    SizeT8 Length{0};   ///< Length of the entire tag variable identifier.
    SizeT8 IDLength{0}; ///< Length of the loop tag variable identifier.
    SizeT8 Level{0};    ///< Nesting level of the variable (for scopes).
};

} // namespace Tags
} // namespace Qentem

#endif
