
#ifndef STM32_TYPES_H
#define STM32_TYPES_H

/* Includes ------------------------------------------------------------------- */
#include <stdint.h>

/**
 * @brief Boolean Type definition
 */
typedef enum {FALSE = 0, TRUE = !FALSE} Bool;

/* NULL pointer */
#ifndef NULL
#define NULL ((void*) 0)
#endif

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/** SMA type for character type */
typedef char CHAR;

/** SMA type for 8 bit unsigned value */
typedef uint8_t UNS_8;

/** SMA type for 8 bit signed value */
typedef int8_t INT_8;

/** SMA type for 16 bit unsigned value */
typedef	uint16_t UNS_16;

/** SMA type for 16 bit signed value */
typedef	int16_t INT_16;

/** SMA type for 32 bit unsigned value */
typedef	uint32_t UNS_32;

/** SMA type for 32 bit signed value */
typedef	int32_t INT_32;

/** SMA type for 64 bit signed value */
typedef int64_t INT_64;

/** SMA type for 64 bit unsigned value */
typedef uint64_t UNS_64;

/** 32 bit boolean type */
typedef Bool BOOL_32;

/** 16 bit boolean type */
typedef Bool BOOL_16;

/** 8 bit boolean type */
typedef Bool BOOL_8;

/**
 * @}
 */
#ifndef __FALSE
#define __FALSE   (0)
#endif

#ifndef __TRUE
#define __TRUE    (1)
#endif

typedef signed char        s8;
typedef unsigned char      u8;
typedef short              s16;
typedef unsigned short     u16;
typedef int                s32;
typedef unsigned int       u32;
typedef long long          s64;
typedef unsigned long long u64;

typedef signed char        S8;
typedef unsigned char      U8;
typedef short              S16;
typedef unsigned short     U16;
typedef int                S32;
typedef unsigned int       U32;
typedef long long          S64;
typedef unsigned long long U64;
typedef unsigned char      BIT;
typedef unsigned int       BOOL;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

#endif /* STM32_TYPES_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
