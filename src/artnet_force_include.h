// Force-include header to fix missing includes in lib-artnet-4-cpp
// This ensures cstring is available for std::memset, std::memcpy, std::strlen
// Also ensures sys/time.h is available for struct timeval on web/wasm

#ifndef ARNET_FORCE_INCLUDE_H
#define ARNET_FORCE_INCLUDE_H

#include <cstring>

// For web/wasm builds, sys/time.h may not be automatically included
// Include it here to ensure timeval is available
#ifdef __EMSCRIPTEN__
#include <sys/time.h>
#endif

#endif // ARNET_FORCE_INCLUDE_H



