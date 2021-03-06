#ifndef TYPES_H
#define TYPES_H

typedef char      s8;  //!< Signed 8 bit value
typedef short     s16; //!< Signed 16 bit value
typedef int       s32; //!< Signed 32 bit value
typedef long long s64; //!< Signed 64 bit value

typedef unsigned char      u8;  //!< Unsigned 8 bit value
typedef unsigned short     u16; //!< Unsigned 16 bit value
typedef unsigned int       u32; //!< Unsigned 32 bit value
typedef unsigned long long u64; //!< Unsigned 64 bit value

typedef unsigned int ptr_t;

#define NULL (void *)0

#define __hot         __attribute__((hot))
#define __packed      __attribute__((packed))
#define __noreturn    __attribute__((noreturn))
#define __aligned(A)  __attribute__((aligned(A)))

#if defined(__clang__)
#define __nooptimize  __attribute__((optnone))
#elif defined(__GNUC__)
#define __nooptimize  __attribute__((optimize(0)))
#endif


#endif

