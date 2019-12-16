/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File bastypes.h: Base types definitions.
 *
 *===========================================================================*/

#ifndef __BASTYPES_H
#define __BASTYPES_H

typedef signed long long int        s64;
typedef signed int					s32;
typedef signed short int            s16;
typedef signed char                 s8;

typedef unsigned long long int		u64;
typedef unsigned int				u32;
typedef unsigned short int          u16;
typedef unsigned char               u8;

typedef volatile s64				vs64;
typedef volatile s32				vs32;
typedef volatile s16				vs16;
typedef volatile s8					vs8;

typedef volatile u64				vu64;
typedef volatile u32				vu32;
typedef volatile u16				vu16;
typedef volatile u8					vu8;

typedef s64							Word64;
typedef s32							Word32;
typedef s16							Word16;
typedef s8            				Word8;

typedef u64							UWord64;
typedef u32							UWord32;
typedef u16							UWord16;
typedef u8							UWord8;

typedef signed long					Long;
typedef unsigned long				ULong;

typedef short int					Flag;

//Constant pointer to constant string type definition
typedef const char * const      	cpcstr_t;

//Color type
typedef u32                     	color_t;


//Unaligned memory access enabled
//#define __UNALIGNED_ACCESS	1


#if   defined ( __CC_ARM )  //ARM Compiler
  #define __ASM            __asm
  #define __INLINE         __inline
  #define __STATIC         static
  #define __STATIC_INLINE  static __inline

#elif defined ( __ICCARM__ ) //IAR Compiler
  #define __ASM            __asm
  #define __INLINE         inline
  #define __STATIC         static
  #define __STATIC_INLINE  static inline

#elif defined ( __GNUC__ )  //GNU Compiler
  #define __ASM            __asm
  #define __INLINE         inline
  #define __STATIC         static
  #define __STATIC_INLINE  static inline

#elif defined ( __TASKING__ ) //TASKING Compiler
  #define __ASM            __asm
  #define __INLINE         inline
  #define __STATIC         static
  #define __STATIC_INLINE  static inline

#elif defined ( __CMCARM__ )  //CMC-ARM Compiler
  #define __ASM            #pragma asm
  #define __INLINE         __inline
  #define __STATIC         static
  #define __STATIC_INLINE  static __inline
#else
  #define __ASM            __asm
  #define __INLINE         __inline
  #define __STATIC         static
  #define __STATIC_INLINE  static __inline
#endif

#endif /* __BASTYPES_H */

/*===========================================================================
 * End of file bastypes.h
 *===========================================================================*/

