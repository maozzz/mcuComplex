;/**
; *****************************************************************************
; * @file	startup_MDR1901VC1T.s
; * @author	Milandr Application Team
; * @brief	CMSIS Cortex-CM3 Peripheral Access Layer Header File for
; * 		MDR1901VC1T from Milandr
; * @version	1.0 (CC ARM compiler)
; * @date	2014-02-06
; *****************************************************************************
; * <br><br>
; *
; * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
; * TIME. AS A RESULT, PHYTON SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
; * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
; * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
; * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
; *
;
; * <h2><center>&copy; COPYRIGHT 2014 MDR1901VC1T </center></h2>
; *****************************************************************************
; * FILE startup_MDR1901VC1T.s
; */

; <<< Use Configuration Wizard in Context Menu >>>

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00001000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
	               EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
__Vectors       DCD     __initial_sp             ; Top of Stack
                 DCD     Reset_Handler           ; Reset Handler
                 DCD     NMI_Handler  			 ; NMI Handler
                 DCD     HardFault_Handler       ; Hard Fault Handeler
                 DCD     MemManage_Handler		 ; Memory Management Fault Handler
                 DCD     BusFault_Handler        ; Bus Fault Handler
                 DCD     UsageFault_Handler      ; Usage Fault Handler
                 DCD     0                       ; Reserved
                 DCD     0                       ; Reserved
                 DCD     0                       ; Reserved
                 DCD     0                       ; Reserved
                 DCD     SVCall_Handler          ; SV Call Handler
                 DCD     0                       ; Reserved
                 DCD     0                       ; Reserved
                 DCD     PendSV_Handler          ; Pend SV Handler
                 DCD     SysTick_Handler         ; SysTick Handeler

                 ; External Interrupts
                 DCD     SSP3_IRQHandler         ; IRQ0
                 DCD     SSP4_IRQHandler         ; IRQ1
                 DCD     USB_IRQHandler          ; IRQ2
                 DCD     DSP_BSP1_IRQHandler     ; IRQ3
                 DCD     DSP_BSP2_IRQHandler     ; IRQ4
                 DCD     DMA_IRQHandler          ; IRQ5
                 DCD     UART1_IRQHandler        ; IRQ6
                 DCD     UART2_IRQHandler        ; IRQ7
                 DCD     SSP1_IRQHandler         ; IRQ8
                 DCD     DSP_BSP3_IRQHandler     ; IRQ9
                 DCD     I2C_IRQHandler          ; IRQ10
                 DCD     POWER_IRQHandler        ; IRQ11
                 DCD     WWDG_IRQHandler         ; IRQ12
                 DCD     DSP_DMA_IRQHandler      ; IRQ13
                 DCD     Timer1_IRQHandler       ; IRQ14
                 DCD     Timer2_IRQHandler       ; IRQ15
                 DCD     Timer3_IRQHandler       ; IRQ16
                 DCD     ADC_IRQHandler          ; IRQ17
                 DCD     SDIO_IRQHandler         ; IRQ18
                 DCD     COMPARATOR_IRQHandler   ; IRQ19
                 DCD     SSP2_IRQHandler         ; IRQ20
                 DCD     DSP_ACODEC_IRQHandler   ; IRQ21
                 DCD     DSP_CRYPTO_IRQHandler   ; IRQ22
                 DCD     DSP_TIMER_IRQHandler    ; IRQ23
                 DCD     DSP_CORE_IRQHandler     ; IRQ24
                 DCD     DSP_STATE_IRQHandler    ; IRQ25
                 DCD     UART3_IRQHandler        ; IRQ26
                 DCD     BACKUP_IRQHandler       ; IRQ27
                 DCD     EXT_INT1_IRQHandler     ; IRQ28
                 DCD     EXT_INT2_IRQHandler     ; IRQ29
                 DCD     EXT_INT3_IRQHandler     ; IRQ30
                 DCD     EXT_INT4_IRQHandler     ; IRQ31
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main
                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)
NMI_Handler     PROC
                EXPORT  NMI_Handler        			 [WEAK]
                B       .
		                ENDP

HardFault_Handler PROC
                EXPORT  HardFault_Handler             [WEAK]
                B       .
		                ENDP

MemManage_Handler PROC
                EXPORT  MemManage_Handler      		  [WEAK]
                B       .
		                ENDP

BusFault_Handler PROC
                EXPORT  BusFault_Handler              [WEAK]
                B       .
		                ENDP

UsageFault_Handler PROC
                EXPORT  UsageFault_Handler            [WEAK]
                B       .
		                ENDP

SVCall_Handler PROC
                EXPORT  SVCall_Handler                [WEAK]
                B       .
		                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler                [WEAK]
                B       .
		                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler               [WEAK]
                B       .
		                ENDP


; External Interrupts

SSP3_IRQHandler PROC
                EXPORT  SSP3_IRQHandler               [WEAK]
                B       .
                     ENDP

SSP4_IRQHandler   PROC
                EXPORT  SSP4_IRQHandler               [WEAK]
                B       .
                     ENDP

USB_IRQHandler    PROC
                EXPORT  USB_IRQHandler                [WEAK]
                B       .
                     ENDP

DSP_BSP1_IRQHandler PROC
                EXPORT  DSP_BSP1_IRQHandler             [WEAK]
                B       .
                     ENDP

DSP_BSP2_IRQHandler PROC
                EXPORT  DSP_BSP2_IRQHandler             [WEAK]
                B       .
                     ENDP

DMA_IRQHandler   PROC
                EXPORT  DMA_IRQHandler                [WEAK]
                B       .
                     ENDP

UART1_IRQHandler PROC
                EXPORT  UART1_IRQHandler              [WEAK]
                B       .
                     ENDP

UART2_IRQHandler PROC
                EXPORT  UART2_IRQHandler              [WEAK]
                B       .
                     ENDP

SSP1_IRQHandler  PROC
                EXPORT  SSP1_IRQHandler               [WEAK]
                B       .
                     ENDP

DSP_BSP3_IRQHandler PROC
                EXPORT  DSP_BSP3_IRQHandler             [WEAK]
                B       .
                     ENDP

I2C_IRQHandler PROC
                EXPORT  I2C_IRQHandler                [WEAK]
                B       .
                     ENDP

POWER_IRQHandler PROC
                EXPORT  POWER_IRQHandler              [WEAK]
                B       .
                     ENDP

WWDG_IRQHandler PROC
                EXPORT  WWDG_IRQHandler               [WEAK]
                B       .
                     ENDP

DSP_DMA_IRQHandler PROC
                EXPORT  DSP_DMA_IRQHandler            [WEAK]
                B       .
                     ENDP

Timer1_IRQHandler PROC
                EXPORT  Timer1_IRQHandler             [WEAK]
                B       .
                     ENDP

Timer2_IRQHandler PROC
                EXPORT  Timer2_IRQHandler             [WEAK]
                B       .
                     ENDP

Timer3_IRQHandler PROC
                EXPORT  Timer3_IRQHandler             [WEAK]
                B       .
                     ENDP

ADC_IRQHandler PROC
                EXPORT  ADC_IRQHandler                [WEAK]
                B       .
                     ENDP

SDIO_IRQHandler PROC
                EXPORT  SDIO_IRQHandler               [WEAK]
                B       .
                     ENDP

COMPARATOR_IRQHandler   PROC
                EXPORT  COMPARATOR_IRQHandler         [WEAK]
                B       .
                     ENDP

SSP2_IRQHandler PROC
                EXPORT  SSP2_IRQHandler               [WEAK]
                B       .
                     ENDP

DSP_ACODEC_IRQHandler PROC
                EXPORT  DSP_ACODEC_IRQHandler           [WEAK]
                B       .
                     ENDP

DSP_CRYPTO_IRQHandler PROC
                EXPORT  DSP_CRYPTO_IRQHandler           [WEAK]
                B       .
                     ENDP

DSP_TIMER_IRQHandler PROC
                EXPORT  DSP_TIMER_IRQHandler          [WEAK]
                B       .
                     ENDP

DSP_CORE_IRQHandler PROC
                EXPORT  DSP_CORE_IRQHandler           [WEAK]
                B       .
                     ENDP

DSP_STATE_IRQHandler PROC
                EXPORT  DSP_STATE_IRQHandler          [WEAK]
                B       .
                     ENDP

UART3_IRQHandler PROC
                EXPORT  UART3_IRQHandler              [WEAK]
                B       .
                     ENDP

BACKUP_IRQHandler PROC
                EXPORT  BACKUP_IRQHandler             [WEAK]
                B       .
                     ENDP

EXT_INT1_IRQHandler PROC
                EXPORT  EXT_INT1_IRQHandler           [WEAK]
                B       .
                     ENDP

EXT_INT2_IRQHandler PROC
                EXPORT  EXT_INT2_IRQHandler           [WEAK]
                B       .
                     ENDP

EXT_INT3_IRQHandler PROC
                EXPORT  EXT_INT3_IRQHandler           [WEAK]
                B       .
                     ENDP

EXT_INT4_IRQHandler PROC
                EXPORT  EXT_INT4_IRQHandler           [WEAK]
                B       .
                     ENDP


                 ALIGN



; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
