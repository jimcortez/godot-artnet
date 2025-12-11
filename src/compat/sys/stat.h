// Compatibility header for sys/stat.h
#ifndef SYS_STAT_H
#define SYS_STAT_H

#ifdef _WIN32
// On Windows, sys/stat.h requires types from sys/types.h
// Include our sys/types.h first to ensure dev_t, ino_t, mode_t, etc. are defined
#include <sys/types.h>

// Then include the real Windows sys/stat.h
#include <sys/stat.h>

#else
// On Unix/Linux/macOS/web, include the real sys/stat.h using include_next
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <sys/stat.h>
#else
// For compilers without #include_next support:
#include <sys/stat.h>
#endif
#endif // _WIN32

#endif // SYS_STAT_H
