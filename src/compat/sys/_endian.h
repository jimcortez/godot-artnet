// Compatibility header for sys/_endian.h (BSD/macOS)
// This provides compatibility for Linux and Windows
#ifndef SYS_ENDIAN_H
#define SYS_ENDIAN_H

#ifdef _WIN32
// Windows uses winsock2.h for byte order functions
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
// htons, ntohs, htonl, ntohl are in winsock2.h
#elif defined(__APPLE__) || defined(__MACH__)
// macOS/BSD: include the real sys/_endian.h and arpa/inet.h for byte order functions
#ifdef __GNUC__
#include_next <sys/_endian.h>
#else
#include <sys/_endian.h>
#endif
// htons, ntohs, htonl, ntohl are in arpa/inet.h on macOS
#include <arpa/inet.h>
#elif defined(__linux__) || defined(__ANDROID__)
// Linux/Android: provide byte order functions via arpa/inet.h
// On Android, sys/_endian.h may not exist, so we need to ensure htons/ntohs are available
#if defined(__ANDROID__)
// On Android, arpa/inet.h is already included by the library and provides fallback functions
// We just need to ensure the headers are included in the right order
// Since arpa/inet.h already has the fallback implementations, we don't redefine them here
#if defined(__GNUC__) || defined(__clang__)
// Include netinet/in.h first (required for socket types)
#include_next <netinet/in.h>
// Include arpa/inet.h which provides htons/ntohs/htonl/ntohl (with fallbacks if needed)
// Note: arpa/inet.h compatibility header already provides fallback functions on Android
#include_next <arpa/inet.h>
#else
// For non-GCC/Clang compilers, include directly
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
// Functions are now available from arpa/inet.h (either from system or our fallback)
#else
// On Linux, sys/_endian.h doesn't exist (it's BSD/macOS only)
// Just include arpa/inet.h which provides htons/ntohs/htonl/ntohl
#if defined(__GNUC__) || defined(__clang__)
#include_next <arpa/inet.h>
#else
#include <arpa/inet.h>
#endif
#endif
// htons, ntohs, htonl, ntohl are in arpa/inet.h
#endif

#endif // SYS_ENDIAN_H

