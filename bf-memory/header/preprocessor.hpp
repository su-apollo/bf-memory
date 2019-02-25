#pragma once

// compiler
#if defined(_MSC_VER)
#if _MSC_VER >= 1800
#define BF_VC 12
#elif _MSC_VER >= 1700
#define BF_VC 11
#elif _MSC_VER >= 1600
#define BF_VC 10
#elif _MSC_VER >= 1500
#define BF_VC 9
#else
#error "Unknown VC version"
#endif
#elif defined(__clang__)
#define BF_CLANG 1
#elif defined(__GNUC__)
#define BF_GCC 1
#else
#error "Unknown compiler"
#endif

// os
#if defined(_WIN64)
#define BF_WIN64 1
#elif defined(_WIN32)
#define BF_WIN32 1
#elif defined(__ANDROID__)
#define BF_ANDROID 1
#elif defined(__linux__)
#define BF_LINUX 1
#elif defined(__APPLE__) && (defined(__arm__) || defined(__arm64__))
#define BF_IOS 1
#elif defined(__APPLE__)
#define BF_OSX 1
#else
#error "Unknown operating system"
#endif

// architecture
#if defined(__x86_64__) || defined(_M_X64)
#define BF_X64 1
#elif defined(__i386__) || defined(_M_IX86)
#define BF_X86 1
#elif defined(__arm64__) || defined(__aarch64__)
#define BF_A64 1
#elif defined(__arm__) || defined(_M_ARM)
#define BF_ARM 1
#else
#error "Unknown architecture"
#endif

// SIMD
#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
#define BF_SSE2 1
#endif
#if defined(_M_ARM) || defined(__ARM_NEON__)
#define BF_NEON 1
#endif


#ifndef BF_VC
#define BF_VC 0
#endif
#ifndef BF_CLANG
#define BF_CLANG 0
#endif
#ifndef BF_GCC
#define BF_GCC 0
#endif
#ifndef BF_WIN64
#define BF_WIN64 0
#endif
#ifndef BF_WIN32
#define BF_WIN32 0
#endif
#ifndef BF_ANDROID
#define BF_ANDROID 0
#endif
#ifndef BF_LINUX
#define BF_LINUX 0
#endif
#ifndef BF_IOS
#define BF_IOS 0
#endif
#ifndef BF_OSX
#define BF_OSX 0
#endif
#ifndef BF_X64
#define BF_X64 0
#endif
#ifndef BF_X86
#define BF_X86 0
#endif
#ifndef BF_A64
#define BF_A64 0
#endif
#ifndef BF_ARM
#define BF_ARM 0
#endif
#ifndef BF_SSE2
#define BF_SSE2 0
#endif
#ifndef BF_NEON
#define BF_NEON 0
#endif

#define BF_GCC_FAMILY (BF_CLANG || BF_GCC)

#define BF_WINDOWS_FAMILY (BF_WIN32 || BF_WIN64)
#define BF_LINUX_FAMILY (BF_LINUX || BF_ANDROID)
#define BF_APPLE_FAMILY (BF_IOS || BF_OSX)
#define BF_UNIX_FAMILY (BF_LINUX_FAMILY || BF_APPLE_FAMILY)

#define BF_INTEL_FAMILY (BF_X64 || BF_X86)
#define BF_ARM_FAMILY (BF_ARM || BF_A64)
#define BF_P64_FAMILY (BF_X64 || BF_A64)


#ifndef BF_ALIGN
#if BF_WINDOWS_FAMILY
#define BF_ALIGN(alignment, decl) __declspec(align(alignment)) decl
#define BF_ALIGN_PREFIX(alignment) __declspec(align(alignment))
#define BF_ALIGN_SUFFIX(alignment)
#elif BF_GCC_FAMILY
#define BF_ALIGN(alignment, decl) decl __attribute__((aligned(alignment)))
#define BF_ALIGN_PREFIX(alignment)
#define BF_ALIGN_SUFFIX(alignment) __attribute__((aligned(alignment)))
#else
#define BF_ALIGN(alignment, decl)
#define BF_ALIGN_PREFIX(alignment)
#define BF_ALIGN_SUFFIX(alignment)
#endif
#endif


// on win32 we only have 8-byte alignment guaranteed, but the CRT provides special aligned allocation
#define BF_MEMORY_ALLOCATION_ALIGNMENT 16