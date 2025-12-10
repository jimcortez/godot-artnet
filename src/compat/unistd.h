// Compatibility header for unistd.h
#ifndef UNISTD_H
#define UNISTD_H

#ifdef _WIN32
#include <io.h>
#include <process.h>
#include <winsock2.h>  // For closesocket()

// Windows equivalents for Unix functions
// close() is in io.h for file descriptors
// getpid() is in process.h
// read(), write() are in io.h but with different signatures

// For socket close, we need to use closesocket() from winsock2.h
// Provide a compatibility close() that handles both file descriptors and sockets
// Note: This is a minimal stub - full implementation would require more
inline int close(int fd) {
    // On Windows, sockets are SOCKET (unsigned int), not int
    // But the library uses int for sockets, so we need to handle both
    // Try closesocket first (for sockets), then _close (for file descriptors)
    if (closesocket(static_cast<SOCKET>(fd)) == 0) {
        return 0;
    }
    // If closesocket fails, try _close for file descriptors
    return _close(fd);
}

#else
// On Unix/Linux/macOS/web, include the real unistd.h using include_next
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <unistd.h>
#else
// For compilers without #include_next support:
#if !defined(_UNISTD_H) && !defined(__UNISTD_H)
#error "unistd.h compatibility header requires GCC, Clang, or Intel ICC compiler with #include_next support"
#endif
#endif
#endif // _WIN32
#endif // UNISTD_H

