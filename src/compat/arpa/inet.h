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
// On Android/Linux, ensure netinet/in.h is included first as htons/ntohs may depend on it
// Include netinet/in.h first to ensure proper definitions
#if defined(__ANDROID__) || defined(__linux__)
// Use include_next to get the real netinet/in.h (skip our compatibility header)
#include_next <netinet/in.h>
#endif
#include_next <arpa/inet.h>
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
