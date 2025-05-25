/**
 * @file VariableTag.hpp
 * @brief Defines the VariableTag struct used for variable offsets in Qentem templates.
 *
 * This header exists independently to avoid dependency cycles between Tags.hpp and Template.hpp.
 * It is a foundational data structure used throughout the Qentem Engine library.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_VARIABLE_TAG_H
#define QENTEM_VARIABLE_TAG_H

#include "QCommon.hpp"

namespace Qentem {
namespace Tags {

/**
 * @brief Represents the metadata for a template variable.
 *
 * Yes, this is an entire header file just for one tiny struct. Why?
 * Because if you dare put it in Tags.hpp or Template.hpp, C++ compilers
 * (especially GCC) will remind you what an "incomplete type" is, and
 * you'll be debugging include cycles until next week.
 *
 * VariableTag bravely lives alone to keep Tags and Template happy and
 * your code building everywhere—even on compilers that woke up grumpy.
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
