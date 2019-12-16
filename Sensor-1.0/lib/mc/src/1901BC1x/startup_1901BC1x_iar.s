;/**
; *****************************************************************************
; * @file	startup_MDR1901VC1T.s
; * @author	Milandr Application Team
; * @brief	CMSIS Cortex-CM3 Peripheral Access Layer Header File for
; * 		MDR1901VC1T from Milandr
; * @version	1.0 (IAR compiler)
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

	
        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
                 DCD     sfe(CSTACK)                  ; Top of Stack
                 DCD     Reset_Handler                ; Reset Handler
                 DCD     NMI_Handler       			  ; NMI Handler
                 DCD     HardFault_Handler            ; Hard Fault Handler
                 DCD     MemManage_Handler     		  ; Memory Management Fault Handler
                 DCD     BusFault_Handler             ; Bus Fault Handler
                 DCD     UsageFault_Handler           ; Usage Fault Handler
                 DCD     0                            ; Reserved
                 DCD     0                            ; Reserved
                 DCD     0                            ; Reserved
                 DCD     0                            ; Reserved
                 DCD     SVCall_Handler               ; SVCall Handler
                 DCD     0                            ; Reserved
                 DCD     0                            ; Reserved
                 DCD     PendSV_Handler               ; Pend SV Handler
                 DCD     SysTick_Handler              ; SysTick Handler

                 ; External Interrupts
                 DCD     SSP3_IRQHandler              ; SSP3 Handler
                 DCD     SSP4_IRQHandler              ; SSP4  Handler
                 DCD     USB_IRQHandler               ; USB Host  Handler
                 DCD     DSP_BSP1_IRQHandler          ; DSP BSP1  Handler
                 DCD     DSP_BSP2_IRQHandler          ; DSP BSP2  Handler
                 DCD     DMA_IRQHandler               ; DMA  Handler
                 DCD     UART1_IRQHandler             ; UART1  Handler
                 DCD     UART2_IRQHandler             ; UART2  Handler
                 DCD     SSP1_IRQHandler              ; SSP1  Handler
                 DCD     DSP_BSP3_IRQHandler          ; DSP BSP3  Handler
                 DCD     I2C_IRQHandler               ; I2C  Handler
                 DCD     POWER_IRQHandler             ; POWER Detecor  Handler
                 DCD     WWDG_IRQHandler              ; Window Watchdog  Handler
                 DCD     DSP_DMA_IRQHandler           ; DSP DMA Handler
                 DCD     Timer1_IRQHandler            ; Timer1  Handler
                 DCD     Timer2_IRQHandler            ; Timer2  Handler
                 DCD     Timer3_IRQHandler            ; Timer3  Handler
                 DCD     ADC_IRQHandler               ; ADC  Handler
                 DCD     SDIO_IRQHandler              ; SDIO  Handler
                 DCD     COMPARATOR_IRQHandler        ; COMPARATOR  Handler
                 DCD     SSP2_IRQHandler              ; SSP2  Handler
                 DCD     DSP_ACODEC_IRQHandler        ; DSP ACODEC Handler
                 DCD     DSP_CRYPTO_IRQHandler        ; DSP CRYPTO Handler
                 DCD     DSP_TIMER_IRQHandler         ; DSP TIMER  Handler
                 DCD     DSP_CORE_IRQHandler          ; DSP CORE   Handler
                 DCD     DSP_STATE_IRQHandler         ; DSP STATE  Handler
                 DCD     UART3_IRQHandler             ; UART3  Handler
                 DCD     BACKUP_IRQHandler            ; BACKUP  Handler
                 DCD     EXT_INT1_IRQHandler          ; EXT_INT1  Handler
                 DCD     EXT_INT2_IRQHandler          ; EXT_INT2  Handler
                 DCD     EXT_INT3_IRQHandler          ; EXT_INT3  Handler
                 DCD     EXT_INT4_IRQHandler          ; EXT_INT4  Handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

; Reset Handler
		PUBWEAK Reset_Handler
		SECTION .text:CODE:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0


; Dummy Exception Handlers (infinite loops which can be modified)
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER(1)
NMI_Handler
		   BL NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER(1)
HardFault_Handler
		   BL HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER(1)
MemManage_Handler
		   BL MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER(1)
BusFault_Handler
		   BL BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER(1)
UsageFault_Handler
		   BL UsageFault_Handler

        PUBWEAK SVCall_Handler
        SECTION .text:CODE:REORDER(1)
SVCall_Handler
		   BL SVCall_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER(1)
PendSV_Handler
		   BL PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER(1)
SysTick_Handler
		   BL SysTick_Handler


; External Interrupts

        PUBWEAK SSP3_IRQHandler
        SECTION .text:CODE:REORDER(1)
SSP3_IRQHandler
		   BL SSP3_IRQHandler

        PUBWEAK SSP4_IRQHandler
        SECTION .text:CODE:REORDER(1)
SSP4_IRQHandler
		   BL SSP4_IRQHandler

        PUBWEAK USB_IRQHandler
        SECTION .text:CODE:REORDER(1)
USB_IRQHandler
		   BL USB_IRQHandler

        PUBWEAK DSP_BSP1_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_BSP1_IRQHandler
		   BL DSP_BSP1_IRQHandler

        PUBWEAK DSP_BSP2_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_BSP2_IRQHandler
		   BL DSP_BSP2_IRQHandler

        PUBWEAK DMA_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA_IRQHandler
		   BL DMA_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER(1)
UART1_IRQHandler
		   BL UART1_IRQHandler

        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:REORDER(1)
UART2_IRQHandler
		   BL UART2_IRQHandler

        PUBWEAK SSP1_IRQHandler
        SECTION .text:CODE:REORDER(1)
SSP1_IRQHandler
		   BL SSP1_IRQHandler

        PUBWEAK DSP_BSP3_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_BSP3_IRQHandler
		   BL DSP_BSP3_IRQHandler

        PUBWEAK I2C_IRQHandler
        SECTION .text:CODE:REORDER(1)
I2C_IRQHandler
		   BL I2C_IRQHandler

        PUBWEAK POWER_IRQHandler
        SECTION .text:CODE:REORDER(1)
POWER_IRQHandler
		   BL POWER_IRQHandler

        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:REORDER(1)
WWDG_IRQHandler
		   BL WWDG_IRQHandler

        PUBWEAK DSP_DMA_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_DMA_IRQHandler
		   BL DSP_DMA_IRQHandler

        PUBWEAK Timer1_IRQHandler
        SECTION .text:CODE:REORDER(1)
Timer1_IRQHandler
		   BL Timer1_IRQHandler

        PUBWEAK Timer2_IRQHandler
        SECTION .text:CODE:REORDER(1)
Timer2_IRQHandler
		   BL Timer2_IRQHandler

        PUBWEAK Timer3_IRQHandler
        SECTION .text:CODE:REORDER(1)
Timer3_IRQHandler
		   BL Timer3_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER(1)
ADC_IRQHandler
		   BL ADC_IRQHandler

        PUBWEAK SDIO_IRQHandler
        SECTION .text:CODE:REORDER(1)
SDIO_IRQHandler
		   BL SDIO_IRQHandler

        PUBWEAK COMPARATOR_IRQHandler
        SECTION .text:CODE:REORDER(1)
COMPARATOR_IRQHandler
		   BL COMPARATOR_IRQHandler

        PUBWEAK SSP2_IRQHandler
        SECTION .text:CODE:REORDER(1)
SSP2_IRQHandler
		   BL SSP2_IRQHandler

        PUBWEAK DSP_ACODEC_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_ACODEC_IRQHandler
		   BL DSP_ACODEC_IRQHandler

        PUBWEAK DSP_CRYPTO_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_CRYPTO_IRQHandler
		   BL DSP_CRYPTO_IRQHandler

        PUBWEAK DSP_TIMER_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_TIMER_IRQHandler
		   BL DSP_TIMER_IRQHandler

        PUBWEAK DSP_CORE_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_CORE_IRQHandler
		   BL DSP_CORE_IRQHandler

        PUBWEAK DSP_STATE_IRQHandler
        SECTION .text:CODE:REORDER(1)
DSP_STATE_IRQHandler
		   BL DSP_STATE_IRQHandler

        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:REORDER(1)
UART3_IRQHandler
		   BL UART3_IRQHandler

        PUBWEAK BACKUP_IRQHandler
        SECTION .text:CODE:REORDER(1)
BACKUP_IRQHandler
		   BL BACKUP_IRQHandler

        PUBWEAK EXT_INT1_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXT_INT1_IRQHandler
		   BL EXT_INT1_IRQHandler

        PUBWEAK EXT_INT2_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXT_INT2_IRQHandler
		   BL EXT_INT2_IRQHandler

        PUBWEAK EXT_INT3_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXT_INT3_IRQHandler
		   BL EXT_INT3_IRQHandler

        PUBWEAK EXT_INT4_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXT_INT4_IRQHandler
		   BL EXT_INT4_IRQHandler

	END
