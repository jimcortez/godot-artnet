// Compatibility header for sys/socket.h
#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

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

// Windows recvfrom expects char* but code uses uint8_t*
// Provide a compatibility wrapper to handle the type mismatch
#include <cstdint>

// Provide an overloaded recvfrom that accepts uint8_t*
// This allows the library code to work without modification
inline int recvfrom(SOCKET s, uint8_t *buf, int len, int flags, struct sockaddr *from, int *fromlen) {
    return ::recvfrom(s, reinterpret_cast<char*>(buf), len, flags, from, fromlen);
}

// Keep the original recvfrom for char* buffers
// (The original is already declared in winsock2.h)

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

