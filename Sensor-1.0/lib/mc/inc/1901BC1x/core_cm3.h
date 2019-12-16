/**************************************************************************//**
 * @file     core_cm3.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 * @version  V3.00
 * @date     11. April 2016
 *
 * @note
 * Copyright (C) 2009-2016 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/
#if defined ( __ICCARM__ )
 #pragma system_include  /* treat file as system include file for MISRA check */
#endif

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef __CORE_CM3_H_GENERIC
#define __CORE_CM3_H_GENERIC

#define __CORTEX_M		  (0x03)				   /*!< Cortex-M Core			 */


#if   defined ( __CC_ARM )
  #define __ASM 	   __asm				      /*!< asm keyword for ARM Compiler 	 */
  #define __INLINE	   __inline				      /*!< inline keyword for ARM Compiler	 */
  #define __STATIC_INLINE  static __inline

#elif defined ( __ICCARM__ )
  #define __ASM 	   __asm				      /*!< asm keyword for IAR Compiler 	 */
  #define __INLINE	   inline				      /*!< inline keyword for IAR Compiler. Only available in High optimization mode! */
  #define __STATIC_INLINE  static inline

#elif defined ( __GNUC__ )
  #define __ASM 	   __asm				      /*!< asm keyword for GNU Compiler 	 */
  #define __INLINE	   inline				      /*!< inline keyword for GNU Compiler	 */
  #define __STATIC_INLINE  static inline

#elif defined ( __TASKING__ )
  #define __ASM 	   __asm				      /*!< asm keyword for TASKING Compiler	 */
  #define __INLINE	   inline				      /*!< inline keyword for TASKING Compiler	 */
  #define __STATIC_INLINE  static inline

#elif defined ( __CMCARM__ )
  #define __ASM 	   #pragma asm				  /*!< #pragma asm for Phyton CMC-ARM Compiler	*/
  #define __INLINE	   __inline				      /*!< inline keyword for Phyton CMC-ARM Compiler */
  #define __STATIC_INLINE  static __inline

#endif

/** __FPU_USED indicates whether an FPU is used or not. This core does not support an FPU at all */
#define __FPU_USED	 0

#if defined ( __CC_ARM )
  #if defined __TARGET_FPU_VFP
	#warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
  #endif

#elif defined ( __ICCARM__ )
  #if defined __ARMVFP__
	#warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
  #endif

#elif defined ( __GNUC__ )
  #if defined (__VFP_FP__) && !defined(__SOFTFP__)
	#warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
  #endif

#elif defined ( __TASKING__ )
	/* add preprocessor checks */

#elif defined ( __CMCARM__ )
	/* add preprocessor checks */

#endif

#include <stdint.h>			     /* standard types definitions			    */
#include "core_cmInstr.h"		 /* Core Instruction Access			    */
#include "core_cmFunc.h"		 /* Core Function Access			    */

#endif /* __CORE_CM3_H_GENERIC */

#ifdef __cplusplus
}
#endif
