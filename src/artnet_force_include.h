// Force-include header to fix missing includes in lib-artnet-4-cpp
// This ensures cstring is available for std::memset, std::memcpy, std::strlen

#ifndef ARNET_FORCE_INCLUDE_H
#define ARNET_FORCE_INCLUDE_H

#include <cstring>

#endif // ARNET_FORCE_INCLUDE_H


