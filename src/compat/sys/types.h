// Compatibility header for sys/types.h
#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#ifdef _WIN32
// On Windows, we need to provide types that sys/stat.h and other headers expect
// Windows doesn't provide all POSIX types by default, so we define them here

// Include Windows base headers that provide fundamental types
#include <basetsd.h>
#include <stddef.h>

// Define POSIX types that Windows sys/stat.h expects but aren't provided by default
// These are needed to prevent compilation errors in sys/stat.h
#ifndef _DEV_T_DEFINED
#define _DEV_T_DEFINED
typedef unsigned int dev_t;
#endif

#ifndef _INO_T_DEFINED
#define _INO_T_DEFINED
typedef unsigned short ino_t;
#endif

#ifndef _MODE_T_DEFINED
#define _MODE_T_DEFINED
typedef unsigned short mode_t;
#endif

// Windows doesn't have ssize_t, but it's commonly used in POSIX code
// Define it here for compatibility
#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef int ssize_t;
#endif

#else
// On Unix/Linux/macOS/web, include the real sys/types.h using include_next
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <sys/types.h>
#else
// For compilers without #include_next support:
#include <sys/types.h>
#endif
#endif // _WIN32

#endif // SYS_TYPES_H
