// Compatibility header for arpa/inet.h
#ifndef ARPA_INET_H
#define ARPA_INET_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

// htons, ntohs, htonl, ntohl are already defined in winsock2.h
// inet_addr, inet_ntoa are in winsock2.h
// inet_pton, inet_ntop are in ws2tcpip.h
#else
// On Unix/Linux/Android, include the real arpa/inet.h using include_next to skip this header
// #include_next is supported by GCC, Clang, and Intel ICC
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
// On Android, htons/ntohs may be in endian.h or require specific include order
#if defined(__ANDROID__)
// On Android, try to include endian.h first if available, as it may define htons/ntohs
#ifdef __has_include
#if __has_include(<endian.h>)
#include_next <endian.h>
#endif
#endif
// Also include netinet/in.h as it may be needed
#include_next <netinet/in.h>
// Include arpa/inet.h which should provide htons/ntohs/htonl/ntohl
#include_next <arpa/inet.h>

// If the functions still aren't declared (e.g., #include_next didn't work), declare them ourselves
// This is a fallback for Android NDK versions where #include_next doesn't work correctly
#include <cstdint>

// Undefine any macros first (in case system headers defined them as macros)
#ifdef htons
#undef htons
#endif
#ifdef ntohs
#undef ntohs
#endif
#ifdef htonl
#undef htonl
#endif
#ifdef ntohl
#undef ntohl
#endif

// Provide our own implementations as fallback
// These will be used if system headers didn't provide the functions
// Note: We assume little-endian (which Android is), so we need to swap bytes
inline uint16_t htons(uint16_t hostshort) {
    return ((hostshort & 0xFF00) >> 8) | ((hostshort & 0x00FF) << 8);
}

inline uint16_t ntohs(uint16_t netshort) {
    return htons(netshort);
}

inline uint32_t htonl(uint32_t hostlong) {
    return ((hostlong & 0xFF000000) >> 24) | ((hostlong & 0x00FF0000) >> 8) |
           ((hostlong & 0x0000FF00) << 8) | ((hostlong & 0x000000FF) << 24);
}

inline uint32_t ntohl(uint32_t netlong) {
    return htonl(netlong);
}
#elif defined(__linux__)
// On Linux, ensure netinet/in.h is included first as htons/ntohs may depend on it
#include_next <netinet/in.h>
#include_next <arpa/inet.h>
#endif
#else
// For compilers without #include_next support:
// Check if we're being included recursively (system header guard already defined)
// Most system headers define _ARPA_INET_H or similar
#if !defined(_ARPA_INET_H) && !defined(_NETINET_IN_H)
// Not recursively including - this shouldn't happen with proper compilers
// but if it does, error out to prevent infinite recursion
#error "arpa/inet.h compatibility header requires GCC, Clang, or Intel ICC compiler with #include_next support"
#endif
#endif
#endif // _WIN32
#endif // ARPA_INET_H
