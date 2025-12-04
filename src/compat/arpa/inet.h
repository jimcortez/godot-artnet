// Windows compatibility header for arpa/inet.h
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

#endif // _WIN32
#endif // ARPA_INET_H

