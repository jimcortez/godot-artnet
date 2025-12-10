// Compatibility header for sys/time.h
#ifndef SYS_TIME_H
#define SYS_TIME_H

#ifdef _WIN32
// Windows doesn't have sys/time.h, but timeval is in winsock2.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
// timeval is defined in winsock2.h
#else
// On Unix/Linux/macOS/web, include the real sys/time.h using include_next
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <sys/time.h>
#else
// For compilers without #include_next support:
// Check if we're being included recursively
#if !defined(_SYS_TIME_H) && !defined(__SYS_TIME_H)
#error "sys/time.h compatibility header requires GCC, Clang, or Intel ICC compiler with #include_next support"
#endif
#endif
#endif // _WIN32
#endif // SYS_TIME_H

