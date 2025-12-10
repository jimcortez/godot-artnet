// Compatibility header for pthread.h
#ifndef PTHREAD_H_COMPAT
#define PTHREAD_H_COMPAT

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <process.h>

// Undefine ERROR macro to prevent conflicts with LogLevel::ERROR in logging.h
#ifdef ERROR
#undef ERROR
#endif

// Minimal pthread compatibility for Windows
// Note: This is a minimal stub - full pthread implementation would require more
typedef HANDLE pthread_t;
typedef CRITICAL_SECTION pthread_mutex_t;

// Provide pthread_self() for Windows
inline pthread_t pthread_self() {
    return GetCurrentThread();
}

// For setThreadPriority in utils.h, we'll use Windows API directly
// So we don't need full pthread implementation here
#else
// On Unix/Linux, include the real pthread.h using include_next to skip this header
// #include_next is supported by GCC, Clang, and Intel ICC
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <pthread.h>
#else
// For compilers without #include_next support:
// Check if we're being included recursively (system header guard already defined)
// Most system headers define _PTHREAD_H or similar
#if !defined(_PTHREAD_H) && !defined(__PTHREAD_H)
// Not recursively including - this shouldn't happen with proper compilers
// but if it does, error out to prevent infinite recursion
#error "pthread.h compatibility header requires GCC, Clang, or Intel ICC compiler with #include_next support"
#endif
#endif
#endif // _WIN32
#endif // PTHREAD_H_COMPAT


