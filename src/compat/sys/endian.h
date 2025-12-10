// Compatibility header for sys/endian.h (Linux)
// On Windows, this provides compatibility
#ifndef SYS_ENDIAN_H_LINUX
#define SYS_ENDIAN_H_LINUX

#ifdef _WIN32
// Windows uses winsock2.h for byte order functions
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
// htons, ntohs, htonl, ntohl are in winsock2.h
#else
// On Linux, include endian.h if available, otherwise use arpa/inet.h
#ifdef __has_include
#if __has_include(<endian.h>)
#include <endian.h>
#else
#include <arpa/inet.h>
#endif
#else
// Fallback: try endian.h first, but it may not exist on all systems
#include <arpa/inet.h>
#endif
#endif // _WIN32

#endif // SYS_ENDIAN_H_LINUX


