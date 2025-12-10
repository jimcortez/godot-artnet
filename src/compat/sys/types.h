// Compatibility header for sys/types.h
#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#ifdef _WIN32
#include <cstddef>
#include <cstdint>

// Windows doesn't have ssize_t, but it's commonly used in POSIX code
// ssize_t is a signed size type, typically the same as long or int
// On Windows, we'll use ptrdiff_t (signed version of size_t) or int
// Since Windows socket functions return int, we'll use int for compatibility
typedef int ssize_t;

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
