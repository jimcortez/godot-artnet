// Windows compatibility header for sched.h
#ifndef SCHED_H_COMPAT
#define SCHED_H_COMPAT

#ifdef _WIN32
#include <windows.h>

// Minimal sched compatibility for Windows
// For setThreadPriority in utils.h, we'll use Windows API directly
// So we don't need full sched implementation here

#endif // _WIN32
#endif // SCHED_H_COMPAT

