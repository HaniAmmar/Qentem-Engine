/**
 * @file VariableTag.hpp
 * @brief Defines the VariableTag struct used for variable offsets in Qentem templates.
 *
 * This header exists independently to avoid dependency cycles between Tags.hpp and Template.hpp.
 * It is a foundational data structure used throughout the Qentem Engine library.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_VARIABLE_TAG_H
#define QENTEM_VARIABLE_TAG_H

#include "Qentem/QCommon.hpp"

namespace Qentem {
namespace Tags {
// TODO: Add QNumber for hash value or string-to-number value

/**
 * @brief Represents the metadata for a template variable.
 */
struct VariableTag {
    SizeT   Offset{0};   ///< Offset of the variable within the template.
    SizeT16 Length{0};   ///< Length of the variable name.
    SizeT8  IDLength{0}; ///< Length of the variable identifier.
    SizeT8  Level{0};    ///< Nesting level of the variable (for scopes).
};

} // namespace Tags
} // namespace Qentem

#endif // QENTEM_VARIABLE_TAG_H
