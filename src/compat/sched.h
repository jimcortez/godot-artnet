// Compatibility header for sched.h
#ifndef SCHED_H_COMPAT
#define SCHED_H_COMPAT

#ifdef _WIN32
#include <windows.h>

// Minimal sched compatibility for Windows
// For setThreadPriority in utils.h, we'll use Windows API directly
// So we don't need full sched implementation here
#else
// On Unix/Linux, include the real sched.h using include_next to skip this header
// #include_next is supported by GCC, Clang, and Intel ICC
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <sched.h>
#else
// For compilers without #include_next support:
// Check if we're being included recursively (system header guard already defined)
// Most system headers define _SCHED_H or similar
#if !defined(_SCHED_H) && !defined(__SCHED_H)
// Not recursively including - this shouldn't happen with proper compilers
// but if it does, error out to prevent infinite recursion
#error "sched.h compatibility header requires GCC, Clang, or Intel ICC compiler with #include_next support"
#endif
#endif
#endif // _WIN32
#endif // SCHED_H_COMPAT


