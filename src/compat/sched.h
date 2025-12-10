// Compatibility header for sched.h
#ifndef SCHED_H_COMPAT
#define SCHED_H_COMPAT

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "pthread.h"  // For pthread_t

// Undefine ERROR macro to prevent conflicts with LogLevel::ERROR in logging.h
#ifdef ERROR
#undef ERROR
#endif

// Minimal sched compatibility for Windows
// Provide sched_param structure and constants for utils.h
struct sched_param {
    int sched_priority;
};

// Define scheduling policies (not used on Windows, but needed for compilation)
#define SCHED_OTHER 0
#define SCHED_FIFO 1
#define SCHED_RR 2

// Provide stub functions for utils.h
inline int sched_get_priority_min(int policy) {
    (void)policy; // Unused
    return THREAD_PRIORITY_LOWEST;
}

inline int sched_get_priority_max(int policy) {
    (void)policy; // Unused
    return THREAD_PRIORITY_HIGHEST;
}

// Provide pthread functions that depend on sched
inline int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param) {
    (void)thread;
    if (policy) {
        *policy = SCHED_OTHER;
    }
    if (param) {
        param->sched_priority = 0;
    }
    return 0;
}

inline int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param) {
    (void)policy;
    if (param) {
        int winPriority = THREAD_PRIORITY_NORMAL;
        if (param->sched_priority > 0) {
            winPriority = THREAD_PRIORITY_ABOVE_NORMAL;
        } else if (param->sched_priority < 0) {
            winPriority = THREAD_PRIORITY_BELOW_NORMAL;
        }
        return SetThreadPriority(thread, winPriority) ? 0 : -1;
    }
    return 0;
}

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


