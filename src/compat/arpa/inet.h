// Compatibility header for arpa/inet.h
#ifndef ARPA_INET_H
#define ARPA_INET_H

#include <cstdint>
#include <cstring>
#include <cstdio>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

// htons, ntohs, htonl, ntohl are already defined in winsock2.h
// inet_addr, inet_ntoa are in winsock2.h
// inet_pton, inet_ntop are in ws2tcpip.h

#elif defined(__EMSCRIPTEN__)
// Emscripten/WebAssembly platform
// Web browsers don't have direct socket access, but we still need the definitions
// for code to compile. Provide stub/minimal implementations.
#include <netinet/in.h>

// Provide implementations for inet functions that aren't available in Emscripten
// These are needed for code compilation even though networking won't work in browser

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

inline const char* inet_ntop(int af, const void* src, char* dst, uint32_t size) {
    if (af != AF_INET || size < INET_ADDRSTRLEN) {
        return nullptr;
    }
    const unsigned char* bytes = static_cast<const unsigned char*>(src);
    snprintf(dst, size, "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);
    return dst;
}

inline int inet_pton(int af, const char* src, void* dst) {
    if (af != AF_INET) {
        return -1;
    }
    unsigned int a, b, c, d;
    if (sscanf(src, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) {
        return 0;
    }
    if (a > 255 || b > 255 || c > 255 || d > 255) {
        return 0;
    }
    unsigned char* bytes = static_cast<unsigned char*>(dst);
    bytes[0] = static_cast<unsigned char>(a);
    bytes[1] = static_cast<unsigned char>(b);
    bytes[2] = static_cast<unsigned char>(c);
    bytes[3] = static_cast<unsigned char>(d);
    return 1;
}

inline uint32_t inet_addr(const char* cp) {
    unsigned int a, b, c, d;
    if (sscanf(cp, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) {
        return INADDR_NONE;
    }
    if (a > 255 || b > 255 || c > 255 || d > 255) {
        return INADDR_NONE;
    }
    return (static_cast<uint32_t>(a)) |
           (static_cast<uint32_t>(b) << 8) |
           (static_cast<uint32_t>(c) << 16) |
           (static_cast<uint32_t>(d) << 24);
}

// inet_ntoa is deprecated and not thread-safe, but some code uses it
// We provide a simple implementation using a static buffer
inline char* inet_ntoa(struct in_addr in) {
    static char buffer[INET_ADDRSTRLEN];
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&in.s_addr);
    snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);
    return buffer;
}

#else
// On Unix/Linux/Android/macOS/iOS, include the real arpa/inet.h using include_next to skip this header
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
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
// On macOS/BSD systems, include the real headers
// These systems have inet_ntop and inet_pton in arpa/inet.h
#include_next <netinet/in.h>
#include_next <arpa/inet.h>
#else
// Unknown Unix-like platform - try to include the real headers
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
#endif // _WIN32 / __EMSCRIPTEN__ / else
#endif // ARPA_INET_H
