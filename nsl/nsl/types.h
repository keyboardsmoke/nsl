#pragma once

#if defined(__x86_64__) || defined(_M_AMD64) || defined(_M_X64)
#define NSL_PLATFORM_64 1
#elif defined(i386) || defined(__i386__) || defined(__x86__) || defined(_M_IX86)
#define NSL_PLATFORM_32 1
#endif

#ifndef _STDINT
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef long long          intmax_t;
typedef unsigned long long uintmax_t;
#endif

#ifndef _SIZE_T_DEFINED
#ifdef NSL_PLATFORM_64
typedef unsigned __int64 size_t;
#elif NSL_PLATFORM_32
typedef unsigned int     size_t;
#endif
#endif

#ifndef _PTRDIFF_T_DEFINED
#ifdef NSL_PLATFORM_64
typedef __int64          ptrdiff_t;
#elif NSL_PLATFORM_32
typedef int              ptrdiff_t;
#endif
#endif

#ifndef _INTPTR_T_DEFINED
#ifdef NSL_PLATFORM_64
typedef __int64          intptr_t;
#elif NSL_PLATFORM_32
typedef int              intptr_t;
#endif
#endif

#ifndef INT8_MIN
#define INT8_MIN         (-127i8 - 1)
#endif

#ifndef INT16_MIN
#define INT16_MIN        (-32767i16 - 1)
#endif

#ifndef INT32_MIN
#define INT32_MIN        (-2147483647i32 - 1)
#endif

#ifndef INT64_MIN
#define INT64_MIN        (-9223372036854775807i64 - 1)
#endif

#ifndef INT8_MIN
#define INT8_MAX         127i8
#endif

#ifndef INT16_MAX
#define INT16_MAX        32767i16
#endif

#ifndef INT32_MAX
#define INT32_MAX        2147483647i32
#endif

#ifndef INT64_MAX
#define INT64_MAX        9223372036854775807i64
#endif

#ifndef UINT8_MAX
#define UINT8_MAX        0xffui8
#endif

#ifndef UINT16_MAX
#define UINT16_MAX       0xffffui16
#endif

#ifndef UINT32_MAX
#define UINT32_MAX       0xffffffffui32
#endif

#ifndef UINT64_MAX
#define UINT64_MAX       0xffffffffffffffffui64
#endif


// Bit helpers
/* a=target variable, b=bit number to act upon 0-n */
#ifndef BIT_SET
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#endif

#ifndef BIT_CLEAR
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#endif

#ifndef BIT_FLIP
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#endif

#ifndef BIT_CHECK
#define BIT_CHECK(a,b) ((a) & (1<<(b)))
#endif

/* x=target variable, y=mask */
#ifndef BITMASK_SET
#define BITMASK_SET(x,y) ((x) |= (y))
#endif

#ifndef BITMASK_CLEAR
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#endif

#ifndef BITMASK_FLIP
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#endif

#ifndef BITMASK_CHECK
#define BITMASK_CHECK(x,y) (((x) & (y)) == (y))
#endif