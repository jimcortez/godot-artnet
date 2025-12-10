// Force-include header to fix missing includes in lib-artnet-4-cpp
// This ensures cstring is available for std::memset, std::memcpy, std::strlen
// Also ensures sys/time.h is available for struct timeval on web/wasm
// On Windows, prevents ERROR macro conflicts with LogLevel::ERROR

#ifndef ARNET_FORCE_INCLUDE_H
#define ARNET_FORCE_INCLUDE_H

#include <cstring>

// For web/wasm builds, sys/time.h may not be automatically included
// Include it here to ensure timeval is available
#ifdef __EMSCRIPTEN__
#include <sys/time.h>
#endif

// On Windows, prevent ERROR macro from windows.h from conflicting with LogLevel::ERROR
// We need to undefine it before any Windows headers are included
// and ensure it stays undefined for the artnet library code
#ifdef _WIN32
// Save ERROR if it exists, then undefine it
// This must happen before windows.h is included via compatibility headers
#ifndef ARNET_ERROR_SAVED
#define ARNET_ERROR_SAVED
#ifdef ERROR
#define ARNET_SAVED_ERROR ERROR
#undef ERROR
#endif
#endif
#endif

#endif // ARNET_FORCE_INCLUDE_H



