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
// On Android, include headers in the correct order
// Some Android NDK versions require specific include order or have functions in different headers
#if defined(__GNUC__) || defined(__clang__)
// Try endian.h first (some Android NDK versions have htons/ntohs here)
#ifdef __has_include
#if __has_include(<endian.h>)
#include_next <endian.h>
#endif
#endif
// Include netinet/in.h for socket types
#include_next <netinet/in.h>
// Include arpa/inet.h for htons/ntohs (most common location)
#include_next <arpa/inet.h>
#else
#include <endian.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#else
// On Linux, try to include the real sys/_endian.h if it exists
#if defined(__GNUC__) || defined(__clang__)
#ifdef __has_include
#if __has_include(<sys/_endian.h>)
#include_next <sys/_endian.h>
#endif
#endif
#endif
// Include arpa/inet.h for htons/ntohs
#if defined(__GNUC__) || defined(__clang__)
#include_next <arpa/inet.h>
#else
#include <arpa/inet.h>
#endif
#endif
// htons, ntohs, htonl, ntohl are in arpa/inet.h
#endif

#endif // SYS_ENDIAN_H

