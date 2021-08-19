/* date = May 10th 2021 10:21 pm */

#ifndef GS_PLATFORM_H
#define GS_PLATFORM_H

#define COMPILER_MSVC 0
#define COMPILER_CLANG 0

#define PLATFORM_WINDOWS 0
#define PLATFORM_OSX 0
#define PLATFORM_LINUX 0
#define PLATFORM_ANDROID 0
#define PLATFORM_IOS 0
#define PLATFORM_XBOX 0
#define PLATFORM_PS 0
#define PLAYFORM_SWITCH 0

#if defined(_MSC_VER)
# undef COMPILER_MSVC
# define COMPILER_MSVC 1
#endif

#if defined(__clang__)
# undef COMPILER_CLANG
# define COMPILER_CLANG 1
#endif

#if defined(_WIN32) || defined(_WIN64)
# undef PLATFORM_WINDOWS
# define PLATFORM_WINDOWS 1
#endif

#if defined(__APPLE__) && defined(__MACH__)
# undef PLATFORM_OSX
# define PLATFORM_OSX 1
#endif

#if defined(__linux__)
# undef PLATFORM_LINUX
# define PLATFORM_LINUX 1
#endif

#if defined(__ANDROID__)
# undef PLATFORM_ANDROID
# define PLATFORM_ANDROID 1
#endif

// TODO(PS): ios, xbox, playstation, switch
#endif //GS_PLATFORM_H
