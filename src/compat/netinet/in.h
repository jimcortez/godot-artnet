// Compatibility header for netinet/in.h
#ifndef NETINET_IN_H
#define NETINET_IN_H

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

// sockaddr_in, socklen_t, etc. are already defined in winsock2.h
// AF_INET, SOCK_DGRAM, IPPROTO_UDP are in winsock2.h
// INADDR_ANY, INADDR_BROADCAST are in winsock2.h
#else
// On Unix/Linux, include the real netinet/in.h using include_next to skip this header
// #include_next is supported by GCC, Clang, and Intel ICC
#if defined(__GNUC__) || defined(__clang__) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 800)
#include_next <netinet/in.h>
#else
// For compilers without #include_next support:
// Check if we're being included recursively (system header guard already defined)
// Most system headers define _NETINET_IN_H or similar
#if !defined(_NETINET_IN_H) && !defined(__NETINET_IN_H)
// Not recursively including - this shouldn't happen with proper compilers
// but if it does, error out to prevent infinite recursion
#error "netinet/in.h compatibility header requires GCC, Clang, or Intel ICC compiler with #include_next support"
#endif
#endif
#endif // _WIN32
#endif // NETINET_IN_H


