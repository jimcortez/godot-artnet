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
// Linux/Android: provide byte order functions via arpa/inet.h or endian.h
// On Android, htons/ntohs may be in endian.h, so include it if available
#if defined(__ANDROID__)
#ifdef __has_include
#if __has_include(<endian.h>)
#include_next <endian.h>
#endif
#endif
// Also include netinet/in.h as it may be needed
#if defined(__GNUC__) || defined(__clang__)
#include_next <netinet/in.h>
#endif
#endif
// Include arpa/inet.h for htons/ntohs (works on both Linux and Android)
#if defined(__GNUC__) || defined(__clang__)
#include_next <arpa/inet.h>
#else
#include <arpa/inet.h>
#endif
// htons, ntohs, htonl, ntohl are in arpa/inet.h or endian.h
#endif

#endif // SYS_ENDIAN_H

