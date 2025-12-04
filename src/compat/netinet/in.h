// Windows compatibility header for netinet/in.h
#ifndef NETINET_IN_H
#define NETINET_IN_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

// sockaddr_in, socklen_t, etc. are already defined in winsock2.h
// AF_INET, SOCK_DGRAM, IPPROTO_UDP are in winsock2.h
// INADDR_ANY, INADDR_BROADCAST are in winsock2.h

#endif // _WIN32
#endif // NETINET_IN_H

