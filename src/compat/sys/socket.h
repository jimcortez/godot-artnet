// Compatibility header for sys/socket.h
#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
// Include sys/types.h for ssize_t definition on Windows
#include <sys/types.h>

// Undefine ERROR macro to prevent conflicts with LogLevel::ERROR in logging.h
// Windows.h defines ERROR as a macro, which conflicts with enum values
#ifdef ERROR
#undef ERROR
#endif

// Socket functions are in winsock2.h
// AF_INET, SOCK_DGRAM, IPPROTO_UDP are in winsock2.h
// socket, bind, sendto, recvfrom, shutdown, close are in winsock2.h
// SHUT_RD, SHUT_WR, SHUT_RDWR are in winsock2.h (but use SD_RECEIVE, SD_SEND, SD_BOTH)

// Windows uses different constants for shutdown
#ifndef SHUT_RD
#define SHUT_RD SD_RECEIVE
#endif
#ifndef SHUT_WR
#define SHUT_WR SD_SEND
#endif
#ifndef SHUT_RDWR
#define SHUT_RDWR SD_BOTH
#endif

// Windows socket functions expect char* but code uses uint8_t*
// Provide compatibility wrappers to handle the type mismatch
#include <cstdint>
#include <cstddef>

// Windows sendto expects const char* but code uses const uint8_t*
// Provide overloaded sendto that accepts const uint8_t* and size_t parameters
// (Windows API uses int, but code passes size_t)
inline int sendto(SOCKET s, const uint8_t *buf, size_t len, int flags, const struct sockaddr *to, size_t tolen) {
    return ::sendto(s, reinterpret_cast<const char*>(buf), static_cast<int>(len), flags, to, static_cast<int>(tolen));
}

// Also provide overload for non-const sockaddr* (code sometimes uses this)
inline int sendto(SOCKET s, const uint8_t *buf, size_t len, int flags, struct sockaddr *to, size_t tolen) {
    return ::sendto(s, reinterpret_cast<const char*>(buf), static_cast<int>(len), flags, to, static_cast<int>(tolen));
}

// Windows recvfrom expects char* but code uses uint8_t*
// Provide an overloaded recvfrom that accepts uint8_t*
// Note: socklen_t is int on Windows, so this matches the code's usage
inline int recvfrom(SOCKET s, uint8_t *buf, int len, int flags, struct sockaddr *from, socklen_t *fromlen) {
    return ::recvfrom(s, reinterpret_cast<char*>(buf), len, flags, from, fromlen);
}

// Keep the original functions for char* buffers
// (The originals are already declared in winsock2.h)

#else
// On Unix/Linux/macOS/web, include the real sys/socket.h using include_next
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <sys/socket.h>
#else
// For compilers without #include_next support:
#if !defined(_SYS_SOCKET_H) && !defined(__SYS_SOCKET_H)
#error "sys/socket.h compatibility header requires GCC, Clang, or Intel ICC compiler with #include_next support"
#endif
#endif
#endif // _WIN32
#endif // SYS_SOCKET_H

