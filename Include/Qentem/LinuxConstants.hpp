/**
 * @file LinuxConstants.hpp
 * @brief Linux userspace ABI constants used by QenNet.
 *
 * Provides a minimal set of Linux constants required by the library
 * without depending on libc headers such as <fcntl.h> or <unistd.h>.
 *
 * Values match the Linux kernel userspace ABI.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT
 */

#ifndef QENTEM_LINUX_CONSTANTS_HPP
#define QENTEM_LINUX_CONSTANTS_HPP

#if defined(__linux__)

#include "Qentem/QCommon.hpp"

namespace Qentem {

/**
 * @name fcntl() Commands
 * @{
 */

/**
 * @brief Get file status flags.
 */
static constexpr SystemLongI Q_F_GETFL = 3;

/**
 * @brief Set file status flags.
 */
static constexpr SystemLongI Q_F_SETFL = 4;

/** @} */

/**
 * @name *at() Special Values and Flags
 * @{
 */

/**
 * @brief Use the current working directory as the base directory.
 */
static constexpr SystemLongI Q_AT_FDCWD = -100;

/**
 * @brief Operate on the file descriptor itself when the pathname is empty.
 */
static constexpr SystemLongI Q_AT_EMPTY_PATH = 0x1000;

/**
 * @brief Do not follow symbolic links.
 */
static constexpr SystemLongI Q_AT_SYMLINK_NOFOLLOW = 0x100;

/**
 * @brief Remove a directory rather than a regular file.
 */
static constexpr SystemLongI Q_AT_REMOVEDIR = 0x200;

/** @} */

/**
 * @name open() Flags
 * @{
 */

/**
 * @brief Open for read-only access.
 */
static constexpr SystemLongI Q_RDONLY = 00000000;

/**
 * @brief Open for write-only access.
 */
static constexpr SystemLongI Q_WRONLY = 00000001;

/**
 * @brief Open for read and write access.
 */
static constexpr SystemLongI Q_RDWR = 00000002;

/**
 * @brief Create the file if it does not exist.
 */
static constexpr SystemLongI Q_CREAT = 00000100;

/**
 * @brief Fail if the file already exists.
 */
static constexpr SystemLongI Q_EXCL = 00000200;

/**
 * @brief Truncate the file on open.
 */
static constexpr SystemLongI Q_TRUNC = 00001000;

/**
 * @brief Append writes to the end of the file.
 */
static constexpr SystemLongI Q_APPEND = 00002000;

/**
 * @brief Enable non-blocking I/O.
 */
static constexpr SystemLongI Q_NONBLOCK = 00004000;

/**
 * @brief Set close-on-exec for the file descriptor.
 */
static constexpr SystemLongI Q_CLOEXEC = 02000000;

/**
 * @brief Open only a path reference without access permissions.
 */
static constexpr SystemLongI Q_PATH = 010000000;

/**
 * @brief Require the target to be a directory.
 */
static constexpr SystemLongI Q_DIRECTORY = 00200000;

/**
 * @brief Do not follow symbolic links.
 */
static constexpr SystemLongI Q_NOFOLLOW = 00400000;

/** @} */

} // namespace Qentem

#endif
#endif
