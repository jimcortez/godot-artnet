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
#elif defined(__linux__) || defined(__ANDROID__)
// Linux: provide byte order functions via arpa/inet.h
// The library uses htons/ntohs which are in arpa/inet.h
#include <arpa/inet.h>
// htons, ntohs, htonl, ntohl are in arpa/inet.h
#endif

#endif // SYS_ENDIAN_H

