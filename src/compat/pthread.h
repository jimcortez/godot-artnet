// Compatibility header for pthread.h
#ifndef PTHREAD_H_COMPAT
#define PTHREAD_H_COMPAT

#ifdef _WIN32
#include <windows.h>
#include <process.h>

// Minimal pthread compatibility for Windows
// Note: This is a minimal stub - full pthread implementation would require more
typedef HANDLE pthread_t;
typedef CRITICAL_SECTION pthread_mutex_t;

// For setThreadPriority in utils.h, we'll use Windows API directly
// So we don't need full pthread implementation here
#else
// On Unix/Linux, include the real pthread.h using include_next to skip this header
#ifdef __GNUC__
#include_next <pthread.h>
#else
// For non-GCC compilers, try to include system header
#include <pthread.h>
#endif
#endif // _WIN32
#endif // PTHREAD_H_COMPAT


