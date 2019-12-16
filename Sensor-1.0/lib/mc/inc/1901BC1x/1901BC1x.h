/*===========================================================================
 *
 *  MCU library
 *
 *  (C) 2014, Phyton
 *  (C) 2016, Marusenkov S.A. "ostfriesland@mail.ru"
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1901BC1x.h: Special Function Register (SFR) declarations
 *                   for Milandr 1901BC1x.
 *
 *===========================================================================*/

#ifndef __1901BC1X_SFR_H
#define __1901BC1X_SFR_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined ( __CC_ARM   )
	#pragma anon_unions
#endif

/* 1901BC1T Interrupt Number Definition */
typedef enum IQRn{
/*-- Cortex-CM3 Processor Exceptions Numbers ----------------------------------*/
	NonMaskableInt_IRQn                   = -14, /*!< 2 Non Maskable Interrupt */
	HardFault_IRQn                        = -13, /*!< 3 Hard Fault Interrupt */
	MemoryManagement_IRQn                 = -12, /*!< 4 Memory Management Interrupt */
	BusFault_IRQn                         = -11, /*!< 5 Bus Fault Interrupt */
	UsageFault_IRQn                       = -10, /*!< 6 Usage Fault Interrupt */
	SVCall_IRQn                           = -5, /*!< 11 SV Call Interrupt */
	PendSV_IRQn                           = -2, /*!< 14 Pend SV Interrupt */
	SysTick_IRQn                          = -1, /*!< 15 System Tick Timer Interrupt */
/*-- MDR1901VC1T specific Interrupt Numbers --------------------------------------*/
	SSP3_IRQn                             = 0, /*!< SSP3 Interrupt */
	SSP4_IRQn                             = 1, /*!< SSP4  Interrupt */
	USB_IRQn                              = 2, /*!< USB Host  Interrupt */
	DSP_BSP1_IRQn                         = 3, /*!< DSP BSP1  Interrupt */
	DSP_BSP2_IRQn                         = 4, /*!< DSP BSP2  Interrupt */
	DMA_IRQn                              = 5, /*!< DMA  Interrupt */
	UART1_IRQn                            = 6, /*!< UART1  Interrupt */
	UART2_IRQn                            = 7, /*!< UART2  Interrupt */
	SSP1_IRQn                             = 8, /*!< SSP1  Interrupt */
	DSP_BSP3_IRQn                         = 9, /*!< DSP BSP3  Interrupt */
	I2C_IRQn                              = 10, /*!< I2C  Interrupt */
	POWER_IRQn                            = 11, /*!< POWER Detecor  Interrupt */
	WWDG_IRQn                             = 12, /*!< Window Watchdog  Interrupt */
	DSP_DMA_IRQn                          = 13, /*!< DSP DMA Interrupt */
	TIMER1_IRQn                           = 14, /*!< Timer1  Interrupt */
	TIMER2_IRQn                           = 15, /*!< Timer2  Interrupt */
	TIMER3_IRQn                           = 16, /*!< Timer3  Interrupt */
	ADC_IRQn                              = 17, /*!< ADC  Interrupt */
	SDIO_IRQn                             = 18, /*!< SDIO  Interrupt */
	COMPARATOR_IRQn                       = 19, /*!< COMPARATOR  Interrupt */
	SSP2_IRQn                             = 20, /*!< SSP2  Interrupt */
	DSP_ACODEC_IRQn                       = 21, /*!< DSP ACODEC Interrupt */
	DSP_CRYPTO_IRQn                       = 22, /*!< DSP CRYPTO Interrupt */
	DSP_TIMER_IRQn                        = 23, /*!< DSP TIMER  Interrupt */
	DSP_CORE_IRQn                         = 24, /*!< DSP CORE   Interrupt */
	DSP_STATE_IRQn                        = 25, /*!< DSP STATE  Interrupt */
	UART3_IRQn                            = 26, /*!< UART3  Interrupt */
	BACKUP_IRQn                           = 27, /*!< BACKUP  Interrupt */
	EXT_INT1_IRQn                         = 28, /*!< EXT_INT1  Interrupt */
	EXT_INT2_IRQn                         = 29, /*!< EXT_INT2  Interrupt */
	EXT_INT3_IRQn                         = 30, /*!< EXT_INT3  Interrupt */
	EXT_INT4_IRQn                         = 31  /*!< EXT_INT4  Interrupt */
} IRQn_Type;


/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV              0x0102   /*!< Cortex-M3 Core Revision                      */
#define __MPU_PRESENT             1     /*!< MPU present or not                           */
#define __NVIC_PRIO_BITS          3     /*!< Number of Bits used for Priority Levels      */
#define __Vendor_SysTickConfig    0     /*!< Set to 1 if different SysTick Config is used */

/* Includes ------------------------------------------------------------------*/
#include "cm3_macro.h"
#include "core_cm3.h"

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
#define IS_FLAG_STATUS(FLAG) (((FLAG) == RESET) || ((FLAG) == SET))
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
/** @} */ /* End of group __Exported_types */

/* SSP structure */
typedef struct
{
  __io_reg CR0;
  __io_reg CR1;
  __io_reg DR;
  __io_reg SR;
  __io_reg CPSR;
  __io_reg IMSC;
  __io_reg RIS;
  __io_reg MIS;
  __io_reg ICR;
  __io_reg DMACR;
} _ssp;


/*-- SSP_CR0:SSP CR0 Register -----*/
/* Bit field positions: */
#define SSP_CR0_DSS_Pos                                    0
#define SSP_CR0_FRF_Pos                                    4
#define SSP_CR0_SPO_Pos                                    6
#define SSP_CR0_SPH_Pos                                    7
#define SSP_CR0_SCR_Pos                                    8
/* Bit field masks: */
#define SSP_CR0_DSS_Msk                                    ((uint32_t)0x0000000F)
#define SSP_CR0_FRF_Msk                                    ((uint32_t)0x00000030)
#define SSP_CR0_SPO                                        ((uint32_t)0x00000040)
#define SSP_CR0_SPH                                        ((uint32_t)0x00000080)
#define SSP_CR0_SCR_Msk                                    ((uint32_t)0x0000FF00)

/*-- SSP_CR1:SSP CR1 Register -----*/
/* Bit field positions: */
#define SSP_CR1_LBM_Pos                                    0
#define SSP_CR1_SSE_Pos                                    1
#define SSP_CR1_MS_Pos                                     2
#define SSP_CR1_SOD_Pos                                    3
/* Bit field masks: */
#define SSP_CR1_LBM                                        ((uint32_t)0x00000001)
#define SSP_CR1_SSE                                        ((uint32_t)0x00000002)
#define SSP_CR1_MS                                         ((uint32_t)0x00000004)
#define SSP_CR1_SOD                                        ((uint32_t)0x00000008)

/*-- SSP_DR:SSP DR Register -----*/
/* Bit field positions: */
#define SSP_DR_DATA_Pos                                    0
/* Bit field masks: */
#define SSP_DR_DATA_Msk                                    ((uint32_t)0x0000FFFF)

/*-- SSP_SR:SSP SR Register -----*/
/* Bit field positions: */
#define SSP_SR_TFE_Pos                                     0
#define SSP_SR_TNF_Pos                                     1
#define SSP_SR_RNE_Pos                                     2
#define SSP_SR_RFF_Pos                                     3
#define SSP_SR_BSY_Pos                                     4
/* Bit field masks: */
#define SSP_SR_TFE                                         ((uint32_t)0x00000001)
#define SSP_SR_TNF                                         ((uint32_t)0x00000002)
#define SSP_SR_RNE                                         ((uint32_t)0x00000004)
#define SSP_SR_RFF                                         ((uint32_t)0x00000008)
#define SSP_SR_BSY                                         ((uint32_t)0x00000010)

/*-- SSP_CPSR:SSP CPSR Register -----*/
/* Bit field positions: */
#define SSP_CPSR_CPSDVSR_Pos                               0
/* Bit field masks: */
#define SSP_CPSR_CPSDVSR_Msk                               ((uint32_t)0x000000FF)

/*-- SSP_IMSC:SSP IMSC Register -----*/
/* Bit field positions: */
#define SSP_IMSC_RORIM_Pos                                 0
#define SSP_IMSC_RTIM_Pos                                  1
#define SSP_IMSC_RXIM_Pos                                  2
#define SSP_IMSC_TXIM_Pos                                  3
/* Bit field masks: */
#define SSP_IMSC_RORIM                                     ((uint32_t)0x00000001)
#define SSP_IMSC_RTIM                                      ((uint32_t)0x00000002)
#define SSP_IMSC_RXIM                                      ((uint32_t)0x00000004)
#define SSP_IMSC_TXIM                                      ((uint32_t)0x00000008)

/*-- SSP_RIS:SSP RIS Register -----*/
/* Bit field positions: */
#define SSP_RIS_RORRIS_Pos                                 0
#define SSP_RIS_RTRIS_Pos                                  1
#define SSP_RIS_RXRIS_Pos                                  2
#define SSP_RIS_TXRIS_Pos                                  3
/* Bit field masks: */
#define SSP_RIS_RORRIS                                     ((uint32_t)0x00000001)
#define SSP_RIS_RTRIS                                      ((uint32_t)0x00000002)
#define SSP_RIS_RXRIS                                      ((uint32_t)0x00000004)
#define SSP_RIS_TXRIS                                      ((uint32_t)0x00000008)

/*-- SSP_MIS:SSP MIS Register -----*/
/* Bit field positions: */
#define SSP_MIS_RORMIS_Pos                                 0
#define SSP_MIS_RTMIS_Pos                                  1
#define SSP_MIS_RXMIS_Pos                                  2
#define SSP_MIS_TXMIS_Pos                                  3
/* Bit field masks: */
#define SSP_MIS_RORMIS                                     ((uint32_t)0x00000001)
#define SSP_MIS_RTMIS                                      ((uint32_t)0x00000002)
#define SSP_MIS_RXMIS                                      ((uint32_t)0x00000004)
#define SSP_MIS_TXMIS                                      ((uint32_t)0x00000008)

/*-- SSP_ICR:SSP ICR Register -----*/
/* Bit field positions: */
#define SSP_ICR_RORIC_Pos                                  0
#define SSP_ICR_RTIC_Pos                                   1
/* Bit field masks: */
#define SSP_ICR_RORIC                                      ((uint32_t)0x00000001)
#define SSP_ICR_RTIC                                       ((uint32_t)0x00000002)

/*-- SSP_DMACR:SSP DMACR Register -----*/
/* Bit field positions: */
#define SSP_DMACR_RXDMAE_Pos                               0
#define SSP_DMACR_TXDMAE_Pos                               1
/* Bit field masks: */
#define SSP_DMACR_RXDMAE                                   ((uint32_t)0x00000001)
#define SSP_DMACR_TXDMAE                                   ((uint32_t)0x00000002)


#ifdef __USB_NEW_STRUCT_DECLARATION
/* _usb_sep structure */
typedef struct {
	__IO uint32_t CTRL;
	__IO uint32_t STS;
	__IO uint32_t TS;
	__IO uint32_t NTS;
} _usb_sep;

/* _usb_sep_fifo structure */
typedef struct {
	__IO uint32_t RXFD;
	__I  uint32_t RESERVED0;
	__IO uint32_t RXFDC_L;
	__IO uint32_t RXFDC_H;
	__IO uint32_t RXFC;
	__I  uint32_t RESERVED1[11];
	__IO uint32_t TXFD;
	__I  uint32_t RESERVED2[3];
	__IO uint32_t TXFDC;
	__I  uint32_t RESERVED3[10];
	__I  uint32_t RESERVED4;
} _usb_sep_fifo;


/* _usb structure */
typedef struct {
	__IO uint32_t	HTXC;                   /*!<USB HTXC Register */
	__IO uint32_t	HTXT;                   /*!<USB HTXT Register */
	__IO uint32_t	HTXLC;                  /*!<USB HTXLC Register */
	__IO uint32_t	HTXSE;                  /*!< */
	__IO uint32_t	HTXA;                   /*!< */
	__IO uint32_t	HTXE;                   /*!< */
	__IO uint32_t	HFN_L;                  /*!< */
	__IO uint32_t	HFN_H;                  /*!< */
	__IO uint32_t	HIS;                    /*!<USB_HIS Register */
	__IO uint32_t	HIM;                    /*!<USB_HIM Register */
	__IO uint32_t	HRXS;                   /*!<USB_HRXS Register */
	__IO uint32_t	HRXP;                   /*!< */
	__IO uint32_t	HRXA;                   /*!< */
	__IO uint32_t	HRXE;                   /*!< */
	__IO uint32_t	HRXCS;                  /*!< */
	__IO uint32_t	HSTM;                   /*!< */
	__I  uint32_t	RESERVED0[16];
	__IO uint32_t	HRXFD;                  /*!< */
	__I  uint32_t	RESERVED1;
	__IO uint32_t	HRXFDC_L;               /*!< */
	__IO uint32_t	HRXFDC_H;               /*!< */
	__IO uint32_t	HRXFC;                  /*!< */
	__I  uint32_t	RESERVED2[11];
	__IO uint32_t	HTXFD;                  /*!< */
	__I  uint32_t	RESERVED3[3];
	__IO uint32_t	HTXFC;                  /*!< */
	__I  uint32_t	RESERVED4[11];
	_usb_sep 		USB_SEP[4];
	__IO uint32_t	SC;                     /*!<USB_SC Register */
	__IO uint32_t	SLS;                    /*!< */
	__IO uint32_t	SIS;                    /*!<USB_SIS Register */
	__IO uint32_t	SIM;                    /*!<USB_SIM Register */
	__IO uint32_t	SA;                     /*!< */
	__IO uint32_t	SFN_L;                  /*!< */
	__IO uint32_t	SFN_H;                  /*!< */
	__I  uint32_t	RESERVED5[9];
	_usb_sep_fifo	USB_SEP_FIFO[4];
	__IO uint32_t	HSCR;                   /*!<USB_HSCR Register */
	__IO uint32_t	HSVR;                   /*!<USB_HSVR Register */
} _usb;
#endif


typedef struct {
    //USB host registers
    __IO uint32_t HTXC;
    __IO uint32_t HTXT;
    __IO uint32_t HTXLC;
    __IO uint32_t HTXSE;
    __IO uint32_t HTXA;
    __IO uint32_t HTXE;
    __IO uint32_t HFN_L;
    __IO uint32_t HFN_H;
    __IO uint32_t HIS;
    __IO uint32_t HIM;
    __IO uint32_t HRXS;
    __IO uint32_t HRXP;
    __IO uint32_t HRXA;
    __IO uint32_t HRXE;
    __IO uint32_t HRXCS;
    __IO uint32_t HSTM;
    __IO uint32_t __dummy0[16];
    __IO uint32_t HRXFD;
    __IO uint32_t __dummy1;
    __IO uint32_t HRXFDC_L;
    __IO uint32_t HRXFDC_H;
    __IO uint32_t HRXFC;
    __IO uint32_t __dummy2[11];
    __IO uint32_t HTXFD;
    __IO uint32_t __dummy3[3];
    __IO uint32_t HTXFDC;
    __IO uint32_t __dummy4[11];

    //USB device registers
    //SEP0 control
    __IO uint32_t SEP0_CTRL;
    __IO uint32_t SEP0_STS;
    __IO uint32_t SEP0_TS;
    __IO uint32_t SEP0_NTS;

    //SEP1 control
    __IO uint32_t SEP1_CTRL;
    __IO uint32_t SEP1_STS;
    __IO uint32_t SEP1_TS;
    __IO uint32_t SEP1_NTS;

    //SEP2 control
    __IO uint32_t SEP2_CTRL;
    __IO uint32_t SEP2_STS;
    __IO uint32_t SEP2_TS;
    __IO uint32_t SEP2_NTS;

    //SEP3 control
    __IO uint32_t SEP3_CTRL;
    __IO uint32_t SEP3_STS;
    __IO uint32_t SEP3_TS;
    __IO uint32_t SEP3_NTS;

    __IO uint32_t SC;
    __IO uint32_t SLS;
    __IO uint32_t SIS;
    __IO uint32_t SIM;
    __IO uint32_t SA;
    __IO uint32_t SFN_L;
    __IO uint32_t SFN_H;

    __IO uint32_t __dummy5[9];

    //SEP0 FIFO control
    __IO uint32_t SEP0_RXFD;
    __IO uint32_t __dummy6;
    __IO uint32_t SEP0_RXFDC_L;
    __IO uint32_t SEP0_RXFDC_H;
    __IO uint32_t SEP0_RXFC;
    __IO uint32_t __dummy7[11];
    __IO uint32_t SEP0_TXFD;
    __IO uint32_t __dummy8[3];
    __IO uint32_t SEP0_TXFDC;

    __IO uint32_t __dummy9[11];

    //SEP1 FIFO control
    __IO uint32_t SEP1_RXFD;
    __IO uint32_t __dummy10;
    __IO uint32_t SEP1_RXFDC_L;
    __IO uint32_t SEP1_RXFDC_H;
    __IO uint32_t SEP1_RXFC;
    __IO uint32_t __dummy11[11];
    __IO uint32_t SEP1_TXFD;
    __IO uint32_t __dummy12[3];
    __IO uint32_t SEP1_TXFDC;

    __IO uint32_t __dummy13[11];

    //SEP2 FIFO control
    __IO uint32_t SEP2_RXFD;
    __IO uint32_t __dummy14;
    __IO uint32_t SEP2_RXFDC_L;
    __IO uint32_t SEP2_RXFDC_H;
    __IO uint32_t SEP2_RXFC;
    __IO uint32_t __dummy15[11];
    __IO uint32_t SEP2_TXFD;
    __IO uint32_t __dummy16[3];
    __IO uint32_t SEP2_TXFDC;

    __IO uint32_t __dummy17[11];

    //SEP3 FIFO control
    __IO uint32_t SEP3_RXFD;
    __IO uint32_t __dummy18;
    __IO uint32_t SEP3_RXFDC_L;
    __IO uint32_t SEP3_RXFDC_H;
    __IO uint32_t SEP3_RXFC;
    __IO uint32_t __dummy19[11];
    __IO uint32_t SEP3_TXFD;
    __IO uint32_t __dummy20[3];
    __IO uint32_t SEP3_TXFDC;

    __IO uint32_t __dummy21[11];

    __IO uint32_t HSCR;
    __IO uint32_t HSVR;
} _usb;


/*-- USB_HTXC:USB HTXC Register -----*/
/* Bit field positions: */
#define USB_HTXC_TREQ_Pos                                  0
#define USB_HTXC_SOFS_Pos                                  1
#define USB_HTXC_PREEN_Pos                                 2
#define USB_HTXC_ISOEN_Pos                                 3
/* Bit field masks: */
#define USB_HTXC_TREQ                                      ((uint32_t)0x00000001)
#define USB_HTXC_SOFS                                      ((uint32_t)0x00000002)
#define USB_HTXC_PREEN                                     ((uint32_t)0x00000004)
#define USB_HTXC_ISOEN                                     ((uint32_t)0x00000008)

/*-- USB_HTXT:USB HTXT Register -----*/
/* Bit field positions: */
#define USB_HTXT_TTYPE_Pos                                 0
/* Bit field masks: */
#define USB_HTXT_TTYPE_Msk                                 ((uint32_t)0x00000003)

/*-- USB_HTXLC:USB HTXLC Register -----*/
/* Bit field positions: */
#define USB_HTXLC_Pos                                      0
#define USB_HTXLC_DC_Pos                                   2
#define USB_HTXLC_FSPL_Pos                                 3
#define USB_HTXLC_FSLR_Pos                                 4
/* Bit field masks: */
#define USB_HTXLC_Msk                                	   ((uint32_t)0x00000003)
#define USB_HTXLC_DC                                       ((uint32_t)0x00000004)
#define USB_HTXLC_FSPL                                     ((uint32_t)0x00000008)
#define USB_HTXLC_FSLR                                     ((uint32_t)0x00000010)

/*-- USB_HTXSE:USB HTXSE Register -----*/
/* Bit field positions: */
#define USB_HTXSE_SOFEN_Pos                                0
/* Bit field masks: */
#define USB_HTXSE_SOFEN                                    ((uint32_t)0x00000001)

/*-- USB_HTXA:USB HTXA Register -----*/
/* Bit field positions: */
#define USB_HTXA_DEVADDR_Pos                               0
/* Bit field masks: */
#define USB_HTXA_DEVADDR_Msk                               ((uint32_t)0x0000007F)

/*-- USB_HTXE:USB HTXE Register -----*/
/* Bit field positions: */
#define USB_HTXE_EPADDR_Pos                                0
/* Bit field masks: */
#define USB_HTXE_EPADDR_Msk                                ((uint32_t)0x0000000F)

/*-- USB_HFN_L:USB HFN_L Register -----*/
/* Bit field positions: */
#define USB_HFN_L_FNUM_Pos                                 0
/* Bit field masks: */
#define USB_HFN_L_FNUM_Msk                                 ((uint32_t)0x000000FF)

/*-- USB_HFN_H:USB HFN_H Register -----*/
/* Bit field positions: */
#define USB_HFN_H_FNUM_Pos                                 0
/* Bit field masks: */
#define USB_HFN_H_FNUM_Msk                                 ((uint32_t)0x00000007)

/*-- USB_HIS:USB HIS Register -----*/
/* Bit field positions: */
#define USB_HIS_TDONE_Pos                                  0
#define USB_HIS_RESUME_Pos                                 1
#define USB_HIS_CONEV_Pos                                  2
#define USB_HIS_SOFS_Pos                                   3
/* Bit field masks: */
#define USB_HIS_TDONE                                      ((uint32_t)0x00000001)
#define USB_HIS_RESUME                                     ((uint32_t)0x00000002)
#define USB_HIS_CONEV                                      ((uint32_t)0x00000004)
#define USB_HIS_SOFS                                       ((uint32_t)0x00000008)

/*-- USB_HIM:USB HIM Register -----*/
/* Bit field positions: */
#define USB_HIM_TDONEIE_Pos                                0
#define USB_HIM_RESUMEIE_Pos                               1
#define USB_HIM_CONEVIE_Pos                                2
#define USB_HIM_SOFIE_Pos                                  3
/* Bit field masks: */
#define USB_HIM_TDONEIE                                    ((uint32_t)0x00000001)
#define USB_HIM_RESUMEIE                                   ((uint32_t)0x00000002)
#define USB_HIM_CONEVIE                                    ((uint32_t)0x00000004)
#define USB_HIM_SOFIE                                      ((uint32_t)0x00000008)

/*-- USB_HRXS:USB HRXS Register -----*/
/* Bit field positions: */
#define USB_HRXS_CRCERR_Pos                                0
#define USB_HRXS_BSERR_Pos                                 1
#define USB_HRXS_RXOF_Pos                                  2
#define USB_HRXS_RXTO_Pos                                  3
#define USB_HRXS_NAKRXED_Pos                               4
#define USB_HRXS_STALLRXED_Pos                             5
#define USB_HRXS_ACKRXED_Pos                               6
#define USB_HRXS_DATASEQ_Pos                               7

/* Bit field masks: */
#define USB_HRXS_CRCERR                                    ((uint32_t)0x00000001)
#define USB_HRXS_BSERR                                     ((uint32_t)0x00000002)
#define USB_HRXS_RXOF                                      ((uint32_t)0x00000004)
#define USB_HRXS_RXTO                                      ((uint32_t)0x00000008)
#define USB_HRXS_NAKRXED                                   ((uint32_t)0x00000010)
#define USB_HRXS_STALLRXED                                 ((uint32_t)0x00000020)
#define USB_HRXS_ACKRXED                                   ((uint32_t)0x00000040)
#define USB_HRXS_DATASEQ                                   ((uint32_t)0x00000080)

/*-- USB_HRXP:USB HRXP Register -----*/
/* Bit field positions: */
#define USB_HRXP_RPID_Pos                                  0
/* Bit field masks: */
#define USB_HRXP_RPID_Msk                                  ((uint32_t)0x0000000F)

/*-- USB_HRXA:USB HRXA Register -----*/
/* Bit field positions: */
#define USB_HRXA_RADDR_Pos                                 0
/* Bit field masks: */
#define USB_HRXA_RADDR_Msk                                 ((uint32_t)0x0000007F)

/*-- USB_HRXE:USB HRXE Register -----*/
/* Bit field positions: */
#define USB_HRXE_RENDP_Pos                                 0
/* Bit field masks: */
#define USB_HRXE_RENDP_Msk                                 ((uint32_t)0x0000000F)

/*-- USB_HRXCS:USB HRXCS Register -----*/
/* Bit field positions: */
#define USB_HRXCS_RXLS_Pos                                 0
/* Bit field masks: */
#define USB_HRXCS_RXLS_Msk                                 ((uint32_t)0x00000003)

/*-- USB_HSTM:USB HSTM Register -----*/
/* Bit field positions: */
#define USB_HSTM_HSTM_Pos                                  0
/* Bit field masks: */
#define USB_HSTM_HSTM_Msk                                  ((uint32_t)0x000000FF)

/*-- USB_HRXFD:USB HRXFD Register -----*/
/* Bit field positions: */
#define USB_HRXFD_RXFIFODATA_Pos                           0
/* Bit field masks: */
#define USB_HRXFD_RXFIFODATA_Msk                           ((uint32_t)0x000000FF)

/*-- USB_HRXFDC_L:USB HRXFDC_L Register -----*/
/* Bit field positions: */
#define USB_HRXFDC_L_FIFODATACOUNT_Pos                     0
/* Bit field masks: */
#define USB_HRXFDC_L_FIFODATACOUNT_Msk                     ((uint32_t)0x000000FF)

/*-- USB_HRXFDC_H:USB HRXFDC_H Register -----*/
/* Bit field positions: */
#define USB_HRXFDC_H_FIFODATACOUNT_Pos                     0
/* Bit field masks: */
#define USB_HRXFDC_H_FIFODATACOUNT_Msk                     ((uint32_t)0x000000FF)

/*-- USB_HRXFC:USB HRXFC Register -----*/
/* Bit field positions: */
#define USB_HRXFC_FIFOFORCEEMPTY_Pos                       0
/* Bit field masks: */
#define USB_HRXFC_FIFOFORCEEMPTY                           ((uint32_t)0x00000001)

/*-- USB_HTXFD:USB HTXFD Register -----*/
/* Bit field positions: */
#define USB_HTXFD_TXFIFODATA_Pos                           0
/* Bit field masks: */
#define USB_HTXFD_TXFIFODATA_Msk                           ((uint32_t)0x000000FF)

/*-- USB_HTXFC:USB HTXFC Register -----*/
/* Bit field positions: */
#define USB_HTXFC_FIFOFORCEEMPTY_Pos                       0
/* Bit field masks: */
#define USB_HTXFC_FIFOFORCEEMPTY                           ((uint32_t)0x00000001)

/*-- USB_SEP_CTRL:USB SEP_CTRL Register -----*/
/* Bit field positions: */
#define USB_SEP_CTRL_EPEN_Pos                              0
#define USB_SEP_CTRL_EPRDY_Pos                             1
#define USB_SEP_CTRL_EPDATASEQ_Pos                         2
#define USB_SEP_CTRL_EPSSTALL_Pos                          3
#define USB_SEP_CTRL_EPISOEN_Pos                           4
/* Bit field masks: */
#define USB_SEP_CTRL_EPEN                                  ((uint32_t)0x00000001)
#define USB_SEP_CTRL_EPRDY                                 ((uint32_t)0x00000002)
#define USB_SEP_CTRL_EPDATASEQ                             ((uint32_t)0x00000004)
#define USB_SEP_CTRL_EPSSTALL                              ((uint32_t)0x00000008)
#define USB_SEP_CTRL_EPISOEN                               ((uint32_t)0x00000010)

/*-- USB_SEP_STS:USB SEP_STS Register -----*/
/* Bit field positions: */
#define USB_SEP_STS_SCCRCERR_Pos                           0
#define USB_SEP_STS_SCBSERR_Pos                            1
#define USB_SEP_STS_SCRXOF_Pos                             2
#define USB_SEP_STS_SCRXTO_Pos                             3
#define USB_SEP_STS_SCNAKSENT_Pos                          4
#define USB_SEP_STS_SCSTALLSENT_Pos                        5
#define USB_SEP_STS_SCACKRXED_Pos                          6
#define USB_SEP_STS_SCDATASEQ_Pos                          7
/* Bit field masks: */
#define USB_SEP_STS_SCCRCERR                               ((uint32_t)0x00000001)
#define USB_SEP_STS_SCBSERR                                ((uint32_t)0x00000002)
#define USB_SEP_STS_SCRXOF                                 ((uint32_t)0x00000004)
#define USB_SEP_STS_SCRXTO                                 ((uint32_t)0x00000008)
#define USB_SEP_STS_SCNAKSENT                              ((uint32_t)0x00000010)
#define USB_SEP_STS_SCSTALLSENT                            ((uint32_t)0x00000020)
#define USB_SEP_STS_SCACKRXED                              ((uint32_t)0x00000040)
#define USB_SEP_STS_SCDATASEQ                              ((uint32_t)0x00000080)

/*-- USB_SEP_TS:USB SEP_TS Register -----*/
/* Bit field positions: */
#define USB_SEP_TS_SCTTYPE_Pos                             0
/* Bit field masks: */
#define USB_SEP_TS_SCTTYPE_Msk                             ((uint32_t)0x00000003)

/*-- USB_SEP_NTS:USB SEP_NTS Register -----*/
/* Bit field positions: */
#define USB_SEP_NTS_NTTYPE_Pos                             0
/* Bit field masks: */
#define USB_SEP_NTS_NTTYPE_Msk                             ((uint32_t)0x00000003)

/*-- USB_SC:USB SC Register -----*/
/* Bit field positions: */
#define USB_SCGEN_Pos                                   0
#define USB_SCTXLS_Pos                                  1
#define USB_SCDC_Pos                                    3
#define USB_SCFSP_Pos                                   4
#define USB_SCFSR_Pos                                   5
/* Bit field masks: */
#define USB_SCGEN                                       ((uint32_t)0x00000001)
#define USB_SCTXLS_Msk                                  ((uint32_t)0x00000006)
#define USB_SCDC                                        ((uint32_t)0x00000008)
#define USB_SCFSP                                       ((uint32_t)0x00000010)
#define USB_SCFSR                                       ((uint32_t)0x00000020)

/*-- USB_SLS:USB SLS Register -----*/
/* Bit field positions: */
#define USB_SLS_SCRXLS_Pos                                 0
/* Bit field masks: */
#define USB_SLS_SCRXLS_Msk                                 ((uint32_t)0x00000003)

/*-- USB_SIS:USB SIS Register -----*/
/* Bit field positions: */
#define USB_SIS_SCTDONE_Pos                                0
#define USB_SIS_SCRESUME_Pos                               1
#define USB_SIS_SCRESETEV_Pos                              2
#define USB_SIS_SCSOFREC_Pos                               3
#define USB_SIS_SCNAKSENT_Pos                              4
/* Bit field masks: */
#define USB_SIS_SCTDONE                                    ((uint32_t)0x00000001)
#define USB_SIS_SCRESUME                                   ((uint32_t)0x00000002)
#define USB_SIS_SCRESETEV                                  ((uint32_t)0x00000004)
#define USB_SIS_SCSOFREC                                   ((uint32_t)0x00000008)
#define USB_SIS_SCNAKSENT                                  ((uint32_t)0x00000010)

/*-- USB_SIM:USB SIM Register -----*/
/* Bit field positions: */
#define USB_SIM_SCTDONEIE_Pos                              0
#define USB_SIM_SCRESUMEIE_Pos                             1
#define USB_SIM_SCRESETEVIE_Pos                            2
#define USB_SIM_SCSOFRECIE_Pos                             3
#define USB_SIM_SCNAKSENTIE_Pos                            4
/* Bit field masks: */
#define USB_SIM_SCTDONEIE                                  ((uint32_t)0x00000001)
#define USB_SIM_SCRESUMEIE                                 ((uint32_t)0x00000002)
#define USB_SIM_SCRESETEVIE                                ((uint32_t)0x00000004)
#define USB_SIM_SCSOFRECIE                                 ((uint32_t)0x00000008)
#define USB_SIM_SCNAKSENTIE                                ((uint32_t)0x00000010)

/*-- USB_SA:USB SA Register -----*/
/* Bit field positions: */
#define USB_SA_SCDEVADDR_Pos                               0
/* Bit field masks: */
#define USB_SA_SCDEVADDR_Msk                               ((uint32_t)0x0000007F)

/*-- USB_SFN_L:USB SFN_L Register -----*/
/* Bit field positions: */
#define USB_SFN_L_FRAMENUM_Pos                             0
/* Bit field masks: */
#define USB_SFN_L_FRAMENUM_Msk                             ((uint32_t)0x000000FF)

/*-- USB_SFN_H:USB SFN_H Register -----*/
/* Bit field positions: */
#define USB_SFN_H_FRAMENUM_Pos                             0
/* Bit field masks: */
#define USB_SFN_H_FRAMENUM_Msk                             ((uint32_t)0x00000007)

/*-- USB_SEP_FIFO_RXFD:USB SEP_FIFO_RXFD Register -----*/
/* Bit field positions: */
#define USB_SEP_FIFO_RXFD_RXFIFODATA_Pos                   0
/* Bit field masks: */
#define USB_SEP_FIFO_RXFD_RXFIFODATA_Msk                   ((uint32_t)0x000000FF)

/*-- USB_SEP_FIFO_RXFDC_L:USB SEP_FIFO_RXFDC_L Register -----*/
/* Bit field positions: */
#define USB_SEP_FIFO_RXFDC_L_FIFODATACOUNT_Pos             0
/* Bit field masks: */
#define USB_SEP_FIFO_RXFDC_L_FIFODATACOUNT_Msk             ((uint32_t)0x000000FF)

/*-- USB_SEP_FIFO_RXFDC_H:USB SEP_FIFO_RXFDC_H Register -----*/
/* Bit field positions: */
#define USB_SEP_FIFO_RXFDC_H_FIFODATACOUNT_Pos             0
/* Bit field masks: */
#define USB_SEP_FIFO_RXFDC_H_FIFODATACOUNT_Msk             ((uint32_t)0x000000FF)

/*-- USB_SEP_FIFO_RXFC:USB SEP_FIFO_RXFC Register -----*/
/* Bit field positions: */
#define USB_SEP_FIFO_RXFC_FIFOFORCEEMPTY_Pos               0
/* Bit field masks: */
#define USB_SEP_FIFO_RXFC_FIFOFORCEEMPTY                   ((uint32_t)0x00000001)

/*-- USB_SEP_FIFO_TXFD:USB SEP_FIFO_TXFD Register -----*/
/* Bit field positions: */
#define USB_SEP_FIFO_TXFD_TXFIFODATA_Pos                   0
/* Bit field masks: */
#define USB_SEP_FIFO_TXFD_TXFIFODATA_Msk                   ((uint32_t)0x000000FF)

/*-- USB_SEP_FIFO_TXFDC:USB SEP_FIFO_TXFDC Register -----*/
/* Bit field positions: */
#define USB_SEP_FIFO_TXFDC_FIFOFORCEEMPTY_Pos              0
/* Bit field masks: */
#define USB_SEP_FIFO_TXFDC_FIFOFORCEEMPTY                  ((uint32_t)0x00000001)

/*-- USB_HSCR:USB HSCR Register -----*/
/* Bit field positions: */
#define USB_HSCR_HOST_MODE_Pos                             0
#define USB_HSCR_RESET_CORE_Pos                            1
#define USB_HSCR_EN_TX_Pos                                 2
#define USB_HSCR_EN_RX_Pos                                 3
#define USB_HSCR_DP_PULLUP_Pos                             4
#define USB_HSCR_DP_PULLDOWN_Pos                           5
#define USB_HSCR_DM_PULLUP_Pos                             6
#define USB_HSCR_DM_PULLDOWN_Pos                           7
/* Bit field masks: */
#define USB_HSCR_HOST_MODE                                 ((uint32_t)0x00000001)
#define USB_HSCR_RESET_CORE                                ((uint32_t)0x00000002)
#define USB_HSCR_EN_TX                                     ((uint32_t)0x00000004)
#define USB_HSCR_EN_RX                                     ((uint32_t)0x00000008)
#define USB_HSCR_DP_PULLUP                                 ((uint32_t)0x00000010)
#define USB_HSCR_DP_PULLDOWN                               ((uint32_t)0x00000020)
#define USB_HSCR_DM_PULLUP                                 ((uint32_t)0x00000040)
#define USB_HSCR_DM_PULLDOWN                               ((uint32_t)0x00000080)

/*-- USB_HSVR:USB HSVR Register -----*/
/* Bit field positions: */
#define USB_HSVR_VERSION_Pos                               0
#define USB_HSVR_REVISION_Pos                              4
/* Bit field masks: */
#define USB_HSVR_VERSION_Msk                               ((uint32_t)0x0000000F)
#define USB_HSVR_REVISION_Msk                              ((uint32_t)0x000000F0)

/* _eeprom structure */
typedef struct {
	__IO uint32_t	CMD;                    /*!<EEPROM Command Register */
	__IO uint32_t	ADR;                    /*!<EEPROM Address Register */
	__IO uint32_t	DI;                     /*!<EEPROM Read Data Register */
	__IO uint32_t	DO;                     /*!<EEPROM Write Data Register */
	__IO uint32_t	KEY;                    /*!<EEPROM Key Register */
	__IO uint32_t	CTRL;                   /*!< */
} _eeprom;

/*-- EEPROM_CMD:EEPROM CMD Register -----*/
/* Bit field positions: */
#define EEPROM_CMD_CON_Pos                                 0
#define EEPROM_CMD_WR_Pos                                  1
#define EEPROM_CMD_RD_Pos                                  2
#define EEPROM_CMD_DELAY_Pos                               3
#define EEPROM_CMD_XE_Pos                                  6
#define EEPROM_CMD_YE_Pos                                  7
#define EEPROM_CMD_SE_Pos                                  8
#define EEPROM_CMD_IFREN_Pos                               9
#define EEPROM_CMD_ERASE_Pos                               10
#define EEPROM_CMD_MAS1_Pos                                11
#define EEPROM_CMD_PROG_Pos                                12
#define EEPROM_CMD_NVSTR_Pos                               13
/* Bit field masks: */
#define EEPROM_CMD_CON                                     ((uint32_t)0x00000001)
#define EEPROM_CMD_WR                                      ((uint32_t)0x00000002)
#define EEPROM_CMD_RD                                      ((uint32_t)0x00000004)
#define EEPROM_CMD_DELAY_Msk                               ((uint32_t)0x00000038)
#define EEPROM_CMD_XE                                      ((uint32_t)0x00000040)
#define EEPROM_CMD_YE                                      ((uint32_t)0x00000080)
#define EEPROM_CMD_SE                                      ((uint32_t)0x00000100)
#define EEPROM_CMD_IFREN                                   ((uint32_t)0x00000200)
#define EEPROM_CMD_ERASE                                   ((uint32_t)0x00000400)
#define EEPROM_CMD_MAS1                                    ((uint32_t)0x00000800)
#define EEPROM_CMD_PROG                                    ((uint32_t)0x00001000)
#define EEPROM_CMD_NVSTR                                   ((uint32_t)0x00002000)

/*-- EEPROM_ADR:EEPROM ADR Register -----*/
/* Bit field positions: */
#define EEPROM_ADR_ADR_Pos                                 0
/* Bit field masks: */
#define EEPROM_ADR_ADR_Msk                                 ((uint32_t)0xFFFFFFFF)

/*-- EEPROM_DI:EEPROM DI Register -----*/
/* Bit field positions: */
#define EEPROM_DI_DATA_Pos                                 0
/* Bit field masks: */
#define EEPROM_DI_DATA_Msk                                 ((uint32_t)0xFFFFFFFF)

/*-- EEPROM_DO:EEPROM DO Register -----*/
/* Bit field positions: */
#define EEPROM_DO_DATA_Pos                                 0
/* Bit field masks: */
#define EEPROM_DO_DATA_Msk                                 ((uint32_t)0xFFFFFFFF)

/*-- EEPROM_KEY:EEPROM KEY Register -----*/
/* Bit field positions: */
#define EEPROM_KEY_KEY_Pos                                 0
/* Bit field masks: */
#define EEPROM_KEY_KEY_Msk                                 ((uint32_t)0xFFFFFFFF)

/*-- EEPROM_CTRL:EEPROM CTRL Register -----*/
/* Bit field positions: */
#define EEPROM_CTRL_ICEN_Pos                               0
#define EEPROM_CTRL_DCEN_Pos                               1
/* Bit field masks: */
#define EEPROM_CTRL_ICEN                                   ((uint32_t)0x00000001)
#define EEPROM_CTRL_DCEN                                   ((uint32_t)0x00000002)

/* _rst_clk structure */
typedef struct {
	__IO uint32_t	CLOCK_STATUS;           /*!<Clock Status Register */
	__IO uint32_t	PLL_CONTROL;            /*!<PLL Control Register */
	__IO uint32_t	HS_CONTROL;             /*!<HS Control Register */
	__IO uint32_t	CPU_CLOCK;              /*!<CPU Clock Register */
	__IO uint32_t	USB_CLOCK;              /*!<USB Clock Register */
	__IO uint32_t	ADC_MCO_CLOCK;          /*!<ADC Clock Register */
	__IO uint32_t	RTC_CLOCK;              /*!<RTC Clock Register */
	__IO uint32_t	PER_CLOCK;              /*!<Peripheral Clock Enable Register */
	__I  uint32_t	RESERVED0;
	__IO uint32_t	TIM_CLOCK;              /*!<Timer Clock Register */
	__IO uint32_t	UART_CLOCK;             /*!<UART Clock Register */
	__IO uint32_t	SSP_CLOCK;              /*!<SSP1-SSP3 Clock Register */
	__IO uint32_t	DSP_CLOCK;              /*!<DSP Clock Register */
	__IO uint32_t	SSP_CLOCK2;             /*!<SSP4 Clock Register */
	__IO uint32_t	DSP_CONTROL_STATUS;     /*!<DSP Control Status Register */
} _rst_clk;

/*-- RST_CLK_CLOCK_STATUS:RST_CLK CLOCK_STATUS Register -----*/
/* Bit field positions: */
#define RST_CLK_CLOCK_STATUS_PLL_USB_RDY_Pos               0
#define RST_CLK_CLOCK_STATUS_PLL_CPU_RDY_Pos               1
#define RST_CLK_CLOCK_STATUS_HSE_RDY_Pos                   2
#define RST_CLK_CLOCK_STATUS_PLL_DSP_RDY_Pos               3
/* Bit field masks: */
#define RST_CLK_CLOCK_STATUS_PLL_USB_RDY                   ((uint32_t)0x00000001)
#define RST_CLK_CLOCK_STATUS_PLL_CPU_RDY                   ((uint32_t)0x00000002)
#define RST_CLK_CLOCK_STATUS_HSE_RDY                       ((uint32_t)0x00000004)
#define RST_CLK_CLOCK_STATUS_PLL_DSP_RDY                   ((uint32_t)0x00000008)

/*-- RST_CLK_PLL_CONTROL:RST_CLK PLL_CONTROL Register -----*/
/* Bit field positions: */
#define RST_CLK_PLL_CONTROL_PLL_USB_ON_Pos                 0
#define RST_CLK_PLL_CONTROL_PLL_USB_RLD_Pos                1
#define RST_CLK_PLL_CONTROL_PLL_CPU_ON_Pos                 2
#define RST_CLK_PLL_CONTROL_PLL_CPU_PLD_Pos                3
#define RST_CLK_PLL_CONTROL_PLL_USB_MUL_Pos                4
#define RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos                8
#define RST_CLK_PLL_CONTROL_PLL_DSP_ON_Pos                 16
#define RST_CLK_PLL_CONTROL_PLL_DSP_PLD_Pos                17
#define RST_CLK_PLL_CONTROL_PLL_DSP_MUL_Pos                20
/* Bit field masks: */
#define RST_CLK_PLL_CONTROL_PLL_USB_ON                     ((uint32_t)0x00000001)
#define RST_CLK_PLL_CONTROL_PLL_USB_RLD                    ((uint32_t)0x00000002)
#define RST_CLK_PLL_CONTROL_PLL_CPU_ON                     ((uint32_t)0x00000004)
#define RST_CLK_PLL_CONTROL_PLL_CPU_PLD                    ((uint32_t)0x00000008)
#define RST_CLK_PLL_CONTROL_PLL_USB_MUL_Msk                ((uint32_t)0x000000F0)
#define RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Msk                ((uint32_t)0x00000F00)
#define RST_CLK_PLL_CONTROL_PLL_DSP_ON                     ((uint32_t)0x00010000)
#define RST_CLK_PLL_CONTROL_PLL_DSP_PLD                    ((uint32_t)0x00020000)
#define RST_CLK_PLL_CONTROL_PLL_DSP_MUL_Msk                ((uint32_t)0x00F00000)

/*-- RST_CLK_HS_CONTROL:RST_CLK HS_CONTROL Register -----*/
/* Bit field positions: */
#define RST_CLK_HS_CONTROL_HSE_ON_Pos                      0
#define RST_CLK_HS_CONTROL_HSE_BYP_Pos                     1
/* Bit field masks: */
#define RST_CLK_HS_CONTROL_HSE_ON                          ((uint32_t)0x00000001)
#define RST_CLK_HS_CONTROL_HSE_BYP                         ((uint32_t)0x00000002)

/*-- RST_CLK_CPU_CLOCK:RST_CLK CPU_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos                   0
#define RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos                   2
#define RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos                   4
#define RST_CLK_CPU_CLOCK_HCLK_SEL_Pos                     8
/* Bit field masks: */
#define RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk                   ((uint32_t)0x00000003)
#define RST_CLK_CPU_CLOCK_CPU_C2_SEL                       ((uint32_t)0x00000004)
#define RST_CLK_CPU_CLOCK_CPU_C3_SEL_Msk                   ((uint32_t)0x000000F0)
#define RST_CLK_CPU_CLOCK_HCLK_SEL_Msk                     ((uint32_t)0x00000300)

/*-- RST_CLK_USB_CLOCK:RST_CLK USB_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_USB_CLOCK_USB_C1_SEL_Pos                   0
#define RST_CLK_USB_CLOCK_USB_C2_SEL_Pos                   2
#define RST_CLK_USB_CLOCK_USB_C3_SEL_Pos                   4
#define RST_CLK_USB_CLOCK_USB_CLK_EN_Pos                   8
/* Bit field masks: */
#define RST_CLK_USB_CLOCK_USB_C1_SEL_Msk                   ((uint32_t)0x00000003)
#define RST_CLK_USB_CLOCK_USB_C2_SEL                       ((uint32_t)0x00000004)
#define RST_CLK_USB_CLOCK_USB_C3_SEL                       ((uint32_t)0x00000010)
#define RST_CLK_USB_CLOCK_USB_CLK_EN                       ((uint32_t)0x00000100)

/*-- RST_CLK_ADC_MCO_CLOCK:RST_CLK ADC_MCO_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_ADC_MCO_CLOCK_ADC_C1_SEL_Pos               0
#define RST_CLK_ADC_MCO_CLOCK_ADC_C2_SEL_Pos               4
#define RST_CLK_ADC_MCO_CLOCK_ADC_C3_SEL_Pos               8
#define RST_CLK_ADC_MCO_CLOCK_MCO_EN_Pos                   12
#define RST_CLK_ADC_MCO_CLOCK_ADC_CLK_EN_Pos               13
#define RST_CLK_ADC_MCO_CLOCK_AUC_C1_SEL_Pos               16
#define RST_CLK_ADC_MCO_CLOCK_AUC_C2_SEL_Pos               20
#define RST_CLK_ADC_MCO_CLOCK_AUC_C3_SEL_Pos               24
#define RST_CLK_ADC_MCO_CLOCK_AUC_CLK_EN_Pos               31
/* Bit field masks: */
#define RST_CLK_ADC_MCO_CLOCK_ADC_C1_SEL_Msk               ((uint32_t)0x00000003)
#define RST_CLK_ADC_MCO_CLOCK_ADC_C2_SEL_Msk               ((uint32_t)0x00000030)
#define RST_CLK_ADC_MCO_CLOCK_ADC_C3_SEL_Msk               ((uint32_t)0x00000F00)
#define RST_CLK_ADC_MCO_CLOCK_MCO_EN                       ((uint32_t)0x00001000)
#define RST_CLK_ADC_MCO_CLOCK_ADC_CLK_EN                   ((uint32_t)0x00002000)
#define RST_CLK_ADC_MCO_CLOCK_AUC_C1_SEL_Msk               ((uint32_t)0x00030000)
#define RST_CLK_ADC_MCO_CLOCK_AUC_C2_SEL_Msk               ((uint32_t)0x00300000)
#define RST_CLK_ADC_MCO_CLOCK_AUC_C3_SEL_Msk               ((uint32_t)0x0F000000)
#define RST_CLK_ADC_MCO_CLOCK_AUC_CLK_EN                   ((uint32_t)0x80000000)

/*-- RST_CLK_RTC_CLOCK:RST_CLK RTC_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_RTC_CLOCK_HSE_SEL_Pos                      0
#define RST_CLK_RTC_CLOCK_HSI_SEL_Pos                      4
#define RST_CLK_RTC_CLOCK_HSE_RTC_EN_Pos                   8
#define RST_CLK_RTC_CLOCK_HSI_RTC_EN_Pos                   9
/* Bit field masks: */
#define RST_CLK_RTC_CLOCK_HSE_SEL_Msk                      ((uint32_t)0x0000000F)
#define RST_CLK_RTC_CLOCK_HSI_SEL_Msk                      ((uint32_t)0x000000F0)
#define RST_CLK_RTC_CLOCK_HSE_RTC_EN                       ((uint32_t)0x00000100)
#define RST_CLK_RTC_CLOCK_HSI_RTC_EN                       ((uint32_t)0x00000200)

/*-- RST_CLK_PER_CLOCK:RST_CLK PER_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_PER_CLOCK_PCLK_EN_Pos                      0
/* Bit field masks: */
#define RST_CLK_PER_CLOCK_PCLK_EN_Msk                      ((uint32_t)0xFFFFFFFF)

/*-- RST_CLK_TIM_CLOCK:RST_CLK TIM_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_TIM_CLOCK_TIM1_BRG_Pos                     0
#define RST_CLK_TIM_CLOCK_TIM2_BRG_Pos                     8
#define RST_CLK_TIM_CLOCK_TIM3_BRG_Pos                     16
#define RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos                  24
#define RST_CLK_TIM_CLOCK_TIM2_CLK_EN_Pos                  25
#define RST_CLK_TIM_CLOCK_TIM3_CLK_EN_Pos                  26
/* Bit field masks: */
#define RST_CLK_TIM_CLOCK_TIM1_BRG_Msk                     ((uint32_t)0x000000FF)
#define RST_CLK_TIM_CLOCK_TIM2_BRG_Msk                     ((uint32_t)0x0000FF00)
#define RST_CLK_TIM_CLOCK_TIM3_BRG_Msk                     ((uint32_t)0x00FF0000)
#define RST_CLK_TIM_CLOCK_TIM1_CLK_EN                      ((uint32_t)0x01000000)
#define RST_CLK_TIM_CLOCK_TIM2_CLK_EN                      ((uint32_t)0x02000000)
#define RST_CLK_TIM_CLOCK_TIM3_CLK_EN                      ((uint32_t)0x04000000)

/*-- RST_CLK_UART_CLOCK:RST_CLK UART_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_UART_CLOCK_UART1_BRG_Pos                   0
#define RST_CLK_UART_CLOCK_UART2_BRG_Pos                   8
#define RST_CLK_UART_CLOCK_UART3_BRG_Pos                   16
#define RST_CLK_UART_CLOCK_UART1_CLK_EN_Pos                24
#define RST_CLK_UART_CLOCK_UART2_CLK_EN_Pos                25
#define RST_CLK_UART_CLOCK_UART3_CLK_EN_Pos                26
/* Bit field masks: */
#define RST_CLK_UART_CLOCK_UART1_BRG_Msk                   ((uint32_t)0x000000FF)
#define RST_CLK_UART_CLOCK_UART2_BRG_Msk                   ((uint32_t)0x0000FF00)
#define RST_CLK_UART_CLOCK_UART3_BRG_Msk                   ((uint32_t)0x00FF0000)
#define RST_CLK_UART_CLOCK_UART1_CLK_EN                    ((uint32_t)0x01000000)
#define RST_CLK_UART_CLOCK_UART2_CLK_EN                    ((uint32_t)0x02000000)
#define RST_CLK_UART_CLOCK_UART3_CLK_EN                    ((uint32_t)0x04000000)

/*-- RST_CLK_SSP_CLOCK:RST_CLK SSP_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_SSP_CLOCK_SSP1_BRG_Pos                     0
#define RST_CLK_SSP_CLOCK_SSP2_BRG_Pos                     8
#define RST_CLK_SSP_CLOCK_SSP3_BRG_Pos                     16
#define RST_CLK_SSP_CLOCK_SSP1_CLK_EN_Pos                  24
#define RST_CLK_SSP_CLOCK_SSP2_CLK_EN_Pos                  25
#define RST_CLK_SSP_CLOCK_SSP3_CLK_EN_Pos                  26
/* Bit field masks: */
#define RST_CLK_SSP_CLOCK_SSP1_BRG_Msk                     ((uint32_t)0x000000FF)
#define RST_CLK_SSP_CLOCK_SSP2_BRG_Msk                     ((uint32_t)0x0000FF00)
#define RST_CLK_SSP_CLOCK_SSP3_BRG_Msk                     ((uint32_t)0x00FF0000)
#define RST_CLK_SSP_CLOCK_SSP1_CLK_EN                      ((uint32_t)0x01000000)
#define RST_CLK_SSP_CLOCK_SSP2_CLK_EN                      ((uint32_t)0x02000000)
#define RST_CLK_SSP_CLOCK_SSP3_CLK_EN                      ((uint32_t)0x04000000)

/*-- RST_CLK_DSP_CLOCK:RST_CLK DSP_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_DSP_CLOCK_DSP_C1_SEL_Pos                   0
#define RST_CLK_DSP_CLOCK_DSP_C2_SEL_Pos                   2
#define RST_CLK_DSP_CLOCK_DSP_C3_SEL_Pos                   4
#define RST_CLK_DSP_CLOCK_DSP_CLK_EN_Pos                   8
/* Bit field masks: */
#define RST_CLK_DSP_CLOCK_DSP_C1_SEL_Msk                   ((uint32_t)0x00000003)
#define RST_CLK_DSP_CLOCK_DSP_C2_SEL                       ((uint32_t)0x00000004)
#define RST_CLK_DSP_CLOCK_DSP_C3_SEL                       ((uint32_t)0x00000010)
#define RST_CLK_DSP_CLOCK_DSP_CLK_EN                       ((uint32_t)0x00000100)

/*-- RST_CLK_SPP2_CLOCK:RST_CLK SPP2_CLOCK Register -----*/
/* Bit field positions: */
#define RST_CLK_SPP2_CLOCK_SSP4_BRG_Pos                    0
#define RST_CLK_SPP2_CLOCK_SSP4_CLK_EN_Pos                 24
/* Bit field masks: */
#define RST_CLK_SPP2_CLOCK_SSP4_BRG_Msk                    ((uint32_t)0x000000FF)
#define RST_CLK_SPP2_CLOCK_SSP4_CLK_EN                     ((uint32_t)0x01000000)

/*-- RST_CLK_DSP_CONTROL_STATUS:RST_CLK DSP_CONTROL_STATUS Register -----*/
/* Bit field positions: */
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP_Pos             0
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP_CPU_Pos         1
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP_MEM_Pos         2
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP_PER_Pos         3
#define RST_CLK_DSP_CONTROL_STATUS_HOLD_Pos                4
#define RST_CLK_DSP_CONTROL_STATUS_BIO_Pos                 5
#define RST_CLK_DSP_CONTROL_STATUS_XF_Pos                  8
#define RST_CLK_DSP_CONTROL_STATUS_HOLDA_Pos               9
#define RST_CLK_DSP_CONTROL_STATUS_IDLE_Pos                10
#define RST_CLK_DSP_CONTROL_STATUS_BRTRD_Pos               14
#define RST_CLK_DSP_CONTROL_STATUS_RD_BUFF_EN_Pos          15
/* Bit field masks: */
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP                 ((uint32_t)0x00000001)
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP_CPU             ((uint32_t)0x00000002)
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP_MEM             ((uint32_t)0x00000004)
#define RST_CLK_DSP_CONTROL_STATUS_RST_DSP_PER             ((uint32_t)0x00000008)
#define RST_CLK_DSP_CONTROL_STATUS_HOLD                    ((uint32_t)0x00000010)
#define RST_CLK_DSP_CONTROL_STATUS_BIO                     ((uint32_t)0x00000020)
#define RST_CLK_DSP_CONTROL_STATUS_XF                      ((uint32_t)0x00000100)
#define RST_CLK_DSP_CONTROL_STATUS_HOLDA                   ((uint32_t)0x00000200)
#define RST_CLK_DSP_CONTROL_STATUS_IDLE_Msk                ((uint32_t)0x00000C00)
#define RST_CLK_DSP_CONTROL_STATUS_BRTRD                   ((uint32_t)0x00004000)
#define RST_CLK_DSP_CONTROL_STATUS_RD_BUFF_EN              ((uint32_t)0x00008000)

/* _dma structure (ARM core) */
typedef struct {
	__IO uint32_t	STATUS;                 /*!<DMA Status Register */
	__IO uint32_t	CFG;                    /*!<DMA Configuration Register */
	__IO uint32_t	CTRL_BASE_PTR;          /*!< */
	__IO uint32_t	ALT_CTRL_BASE_PTR;      /*!< */
	__IO uint32_t	WAITONREQ_STATUS;       /*!< */
	__IO uint32_t	CHNL_SW_REQUEST;        /*!< */
	__IO uint32_t	CHNL_USEBURST_SET;      /*!< */
	__IO uint32_t	CHNL_USEBURST_CLR;      /*!< */
	__IO uint32_t	CHNL_REQ_MASK_SET;      /*!< */
	__IO uint32_t	CHNL_REQ_MASK_CLR;      /*!< */
	__IO uint32_t	CHNL_ENABLE_SET;        /*!< */
	__IO uint32_t	CHNL_ENABLE_CLR;        /*!< */
	__IO uint32_t	CHNL_PRI_ALT_SET;       /*!< */
	__IO uint32_t	CHNL_PRI_ALT_CLR;       /*!< */
	__IO uint32_t	CHNL_PRIORITY_SET;      /*!< */
	__IO uint32_t	CHNL_PRIORITY_CLR;      /*!< */
	__I  uint32_t	RESERVED0[3];
	__IO uint32_t	ERR_CLR;                /*!< */
} _dma;

/*-- DMA_STATUS:DMA STATUS Register -----*/
/* Bit field positions: */
#define DMA_STATUS_MASTER_ENABLE_Pos                       0
#define DMA_STATUS_STATE_Pos                               4
#define DMA_STATUS_CHNLS_MINUS1_Pos                        16
#define DMA_STATUS_TEST_STATUS_Pos                         28
/* Bit field masks: */
#define DMA_STATUS_MASTER_ENABLE                           ((uint32_t)0x00000001)
#define DMA_STATUS_STATE_Msk                               ((uint32_t)0x000000F0)
#define DMA_STATUS_CHNLS_MINUS1_Msk                        ((uint32_t)0x001F0000)
#define DMA_STATUS_TEST_STATUS_Msk                         ((uint32_t)0xF0000000)

/*-- DMA_CFG:DMA CFG Register -----*/
/* Bit field positions: */
#define DMA_CFG_MASTER_ENABLE_Pos                          0
#define DMA_CFG_CHNL_PROT_CTRL_Pos                         5
/* Bit field masks: */
#define DMA_CFG_MASTER_ENABLE                              ((uint32_t)0x00000001)
#define DMA_CFG_CHNL_PROT_CTRL_Msk                         ((uint32_t)0x000000E0)

/*-- DMA_CTRL_BASE_PTR:DMA CTRL_BASE_PTR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_ALT_CTRL_BASE_PTR:DMA ALT_CTRL_BASE_PTR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_WAITONREQ_STATUS:DMA WAITONREQ_STATUS Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_SW_REQUEST:DMA CHNL_SW_REQUEST Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_USEBURST_SET:DMA CHNL_USEBURST_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_USEBURST_CLR:DMA CHNL_USEBURST_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_REQ_MASK_SET:DMA CHNL_REQ_MASK_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_REQ_MASK_CLR:DMA CHNL_REQ_MASK_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_ENABLE_SET:DMA CHNL_ENABLE_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_ENABLE_CLR:DMA CHNL_ENABLE_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_PRI_ALT_SET:DMA CHNL_PRI_ALT_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_PRI_ALT_CLR:DMA CHNL_PRI_ALT_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_PRIORITY_SET:DMA CHNL_PRIORITY_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_CHNL_PRIORITY_CLR:DMA CHNL_PRIORITY_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DMA_ERR_CLR:DMA ERR_CLR Register -----*/
/* Bit field positions: */
#define DMA_ERR_CLR_ERR_CLR_Pos                            0
/* Bit field masks: */
#define DMA_ERR_CLR_ERR_CLR                                ((uint32_t)0x00000001)

/* _uart structure */
typedef struct {
	__IO uint32_t	DR;                     /*!<UART Data Register */
	__IO uint32_t	RSR_ECR;                /*!<UART RSR Register */
	__I  uint32_t	RESERVED0[4];
	__IO uint32_t	FR;                     /*!<UART Flag Register */
	__I  uint32_t	RESERVED1;
	__IO uint32_t	ILPR;                   /*!< */
	__IO uint32_t	IBRD;                   /*!< */
	__IO uint32_t	FBRD;                   /*!< */
	__IO uint32_t	LCR_H;                  /*!<UART LCR_H Register */
	__IO uint32_t	CR;                     /*!<UART Command Register */
	__IO uint32_t	IFLS;                   /*!<UART IFLS Register */
	__IO uint32_t	IMSC;                   /*!<UART Interrupt Mask Register */
	__IO uint32_t	RIS;                    /*!<UART Interrupt Pending Register */
	__IO uint32_t	MIS;                    /*!<UART Masked Interrupt Pending Register */
	__IO uint32_t	ICR;                    /*!<UART Interrupt Clear Register */
	__IO uint32_t	DMACR;                  /*!<UART DMA Control Register */
} _uart;

/*-- UART_DR:UART DR Register -----*/
/* Bit field positions: */
#define UART_DR_DATA_Pos                                   0
#define UART_DR_FE_Pos                                     8
#define UART_DR_PE_Pos                                     9
#define UART_DR_BE_Pos                                     10
#define UART_DR_OE_Pos                                     11
/* Bit field masks: */
#define UART_DR_DATA_Msk                                   ((uint32_t)0x000000FF)
#define UART_DR_FE                                         ((uint32_t)0x00000100)
#define UART_DR_PE                                         ((uint32_t)0x00000200)
#define UART_DR_BE                                         ((uint32_t)0x00000400)
#define UART_DR_OE                                         ((uint32_t)0x00000800)

/*-- UART_RSR_ECR:UART RSR_ECR Register -----*/
/* Bit field positions: */
#define UART_RSR_ECR_FE_Pos                                0
#define UART_RSR_ECR_PE_Pos                                1
#define UART_RSR_ECR_BE_Pos                                2
#define UART_RSR_ECR_OE_Pos                                3
/* Bit field masks: */
#define UART_RSR_ECR_FE                                    ((uint32_t)0x00000001)
#define UART_RSR_ECR_PE                                    ((uint32_t)0x00000002)
#define UART_RSR_ECR_BE                                    ((uint32_t)0x00000004)
#define UART_RSR_ECR_OE                                    ((uint32_t)0x00000008)

/*-- UART_FR:UART FR Register -----*/
/* Bit field positions: */
#define UART_FR_CTS_Pos                                    0
#define UART_FR_DSR_Pos                                    1
#define UART_FR_DCD_Pos                                    2
#define UART_FR_BUSY_Pos                                   3
#define UART_FR_RXFE_Pos                                   4
#define UART_FR_TXFF_Pos                                   5
#define UART_FR_RXFF_Pos                                   6
#define UART_FR_TXFE_Pos                                   7
#define UART_FR_RI_Pos                                     8
/* Bit field masks: */
#define UART_FR_CTS                                        ((uint32_t)0x00000001)
#define UART_FR_DSR                                        ((uint32_t)0x00000002)
#define UART_FR_DCD                                        ((uint32_t)0x00000004)
#define UART_FR_BUSY                                       ((uint32_t)0x00000008)
#define UART_FR_RXFE                                       ((uint32_t)0x00000010)
#define UART_FR_TXFF                                       ((uint32_t)0x00000020)
#define UART_FR_RXFF                                       ((uint32_t)0x00000040)
#define UART_FR_TXFE                                       ((uint32_t)0x00000080)
#define UART_FR_RI                                         ((uint32_t)0x00000100)

/*-- UART_ILPR:UART ILPR Register -----*/
/* Bit field positions: */
#define UART_ILPR_ILPDVSR_Pos                              0
/* Bit field masks: */
#define UART_ILPR_ILPDVSR_Msk                              ((uint32_t)0x000000FF)

/*-- UART_IBRD:UART IBRD Register -----*/
/* Bit field positions: */
#define UART_IBRD_BAUD_DIVINT_Pos                          0
/* Bit field masks: */
#define UART_IBRD_BAUD_DIVINT_Msk                          ((uint32_t)0x0000FFFF)

/*-- UART_FBRD:UART FBRD Register -----*/
/* Bit field positions: */
#define UART_FBRD_BAUD_DIVFRAC_Pos                         0
/* Bit field masks: */
#define UART_FBRD_BAUD_DIVFRAC_Msk                         ((uint32_t)0x0000003F)

/*-- UART_LCR_H:UART LCR_H Register -----*/
/* Bit field positions: */
#define UART_LCR_H_BRK_Pos                                 0
#define UART_LCR_H_PEN_Pos                                 1
#define UART_LCR_H_EPS_Pos                                 2
#define UART_LCR_H_STP2_Pos                                3
#define UART_LCR_H_FEN_Pos                                 4
#define UART_LCR_H_WLEN_Pos                                5
#define UART_LCR_H_SPS_Pos                                 7
/* Bit field masks: */
#define UART_LCR_H_BRK                                     ((uint32_t)0x00000001)
#define UART_LCR_H_PEN                                     ((uint32_t)0x00000002)
#define UART_LCR_H_EPS                                     ((uint32_t)0x00000004)
#define UART_LCR_H_STP2                                    ((uint32_t)0x00000008)
#define UART_LCR_H_FEN                                     ((uint32_t)0x00000010)
#define UART_LCR_H_WLEN_Msk                                ((uint32_t)0x00000060)
#define UART_LCR_H_SPS                                     ((uint32_t)0x00000080)

/*-- UART_CR:UART CR Register -----*/
/* Bit field positions: */
#define UART_CR_UARTEN_Pos                                 0
#define UART_CR_SIREN_Pos                                  1
#define UART_CR_SIRLP_Pos                                  2
#define UART_CR_LBE_Pos                                    7
#define UART_CR_TXE_Pos                                    8
#define UART_CR_RXE_Pos                                    9
#define UART_CR_DTR_Pos                                    10
#define UART_CR_RTS_Pos                                    11
#define UART_CR_OUT1_Pos                                   12
#define UART_CR_OUT2_Pos                                   13
#define UART_CR_RTSEN_Pos                                  14
#define UART_CR_CTSEN_Pos                                  15
/* Bit field masks: */
#define UART_CR_UARTEN                                     ((uint32_t)0x00000001)
#define UART_CR_SIREN                                      ((uint32_t)0x00000002)
#define UART_CR_SIRLP                                      ((uint32_t)0x00000004)
#define UART_CR_LBE                                        ((uint32_t)0x00000080)
#define UART_CR_TXE                                        ((uint32_t)0x00000100)
#define UART_CR_RXE                                        ((uint32_t)0x00000200)
#define UART_CR_DTR                                        ((uint32_t)0x00000400)
#define UART_CR_RTS                                        ((uint32_t)0x00000800)
#define UART_CR_OUT1                                       ((uint32_t)0x00001000)
#define UART_CR_OUT2                                       ((uint32_t)0x00002000)
#define UART_CR_RTSEN                                      ((uint32_t)0x00004000)
#define UART_CR_CTSEN                                      ((uint32_t)0x00008000)

/*-- UART_IFLS:UART IFLS Register -----*/
/* Bit field positions: */
#define UART_IFLS_TXIFLSEL_Pos                             0
#define UART_IFLS_RXIFLSEL_Pos                             3
/* Bit field masks: */
#define UART_IFLS_TXIFLSEL_Msk                             ((uint32_t)0x00000007)
#define UART_IFLS_RXIFLSEL_Msk                             ((uint32_t)0x00000038)

/*-- UART_IMSC:UART IMSC Register -----*/
/* Bit field positions: */
#define UART_IMSC_RIMIM_Pos                                0
#define UART_IMSC_CTSMIM_Pos                               1
#define UART_IMSC_DCDMIM_Pos                               2
#define UART_IMSC_DSRMIM_Pos                               3
#define UART_IMSC_RXIM_Pos                                 4
#define UART_IMSC_TXIM_Pos                                 5
#define UART_IMSC_RTIM_Pos                                 6
#define UART_IMSC_FEIM_Pos                                 7
#define UART_IMSC_PEIM_Pos                                 8
#define UART_IMSC_BEIM_Pos                                 9
#define UART_IMSC_OEIM_Pos                                 10
/* Bit field masks: */
#define UART_IMSC_RIMIM                                    ((uint32_t)0x00000001)
#define UART_IMSC_CTSMIM                                   ((uint32_t)0x00000002)
#define UART_IMSC_DCDMIM                                   ((uint32_t)0x00000004)
#define UART_IMSC_DSRMIM                                   ((uint32_t)0x00000008)
#define UART_IMSC_RXIM                                     ((uint32_t)0x00000010)
#define UART_IMSC_TXIM                                     ((uint32_t)0x00000020)
#define UART_IMSC_RTIM                                     ((uint32_t)0x00000040)
#define UART_IMSC_FEIM                                     ((uint32_t)0x00000080)
#define UART_IMSC_PEIM                                     ((uint32_t)0x00000100)
#define UART_IMSC_BEIM                                     ((uint32_t)0x00000200)
#define UART_IMSC_OEIM                                     ((uint32_t)0x00000400)

/*-- UART_RIS:UART RIS Register -----*/
/* Bit field positions: */
#define UART_RIS_RIRMIS_Pos                                0
#define UART_RIS_CTSRMIS_Pos                               1
#define UART_RIS_DCDRMIS_Pos                               2
#define UART_RIS_DSRRMIS_Pos                               3
#define UART_RIS_RXRIS_Pos                                 4
#define UART_RIS_TXRIS_Pos                                 5
#define UART_RIS_RTRIS_Pos                                 6
#define UART_RIS_FERIS_Pos                                 7
#define UART_RIS_PERIS_Pos                                 8
#define UART_RIS_BERIS_Pos                                 9
#define UART_RIS_OERIS_Pos                                 10
/* Bit field masks: */
#define UART_RIS_RIRMIS                                    ((uint32_t)0x00000001)
#define UART_RIS_CTSRMIS                                   ((uint32_t)0x00000002)
#define UART_RIS_DCDRMIS                                   ((uint32_t)0x00000004)
#define UART_RIS_DSRRMIS                                   ((uint32_t)0x00000008)
#define UART_RIS_RXRIS                                     ((uint32_t)0x00000010)
#define UART_RIS_TXRIS                                     ((uint32_t)0x00000020)
#define UART_RIS_RTRIS                                     ((uint32_t)0x00000040)
#define UART_RIS_FERIS                                     ((uint32_t)0x00000080)
#define UART_RIS_PERIS                                     ((uint32_t)0x00000100)
#define UART_RIS_BERIS                                     ((uint32_t)0x00000200)
#define UART_RIS_OERIS                                     ((uint32_t)0x00000400)

/*-- UART_MIS:UART MIS Register -----*/
/* Bit field positions: */
#define UART_MIS_RIMMIS_Pos                                0
#define UART_MIS_CTSMMIS_Pos                               1
#define UART_MIS_DCDMMIS_Pos                               2
#define UART_MIS_DSRMMIS_Pos                               3
#define UART_MIS_RXMIS_Pos                                 4
#define UART_MIS_TXMIS_Pos                                 5
#define UART_MIS_RTMIS_Pos                                 6
#define UART_MIS_FEMIS_Pos                                 7
#define UART_MIS_PEMIS_Pos                                 8
#define UART_MIS_BEMIS_Pos                                 9
#define UART_MIS_OEMIS_Pos                                 10
/* Bit field masks: */
#define UART_MIS_RIMMIS                                    ((uint32_t)0x00000001)
#define UART_MIS_CTSMMIS                                   ((uint32_t)0x00000002)
#define UART_MIS_DCDMMIS                                   ((uint32_t)0x00000004)
#define UART_MIS_DSRMMIS                                   ((uint32_t)0x00000008)
#define UART_MIS_RXMIS                                     ((uint32_t)0x00000010)
#define UART_MIS_TXMIS                                     ((uint32_t)0x00000020)
#define UART_MIS_RTMIS                                     ((uint32_t)0x00000040)
#define UART_MIS_FEMIS                                     ((uint32_t)0x00000080)
#define UART_MIS_PEMIS                                     ((uint32_t)0x00000100)
#define UART_MIS_BEMIS                                     ((uint32_t)0x00000200)
#define UART_MIS_OEMIS                                     ((uint32_t)0x00000400)

/*-- UART_ICR:UART ICR Register -----*/
/* Bit field positions: */
#define UART_ICR_RIMIC_Pos                                 0
#define UART_ICR_CTSMIC_Pos                                1
#define UART_ICR_DCDMIC_Pos                                2
#define UART_ICR_DSRMIC_Pos                                3
#define UART_ICR_RXIC_Pos                                  4
#define UART_ICR_TXIC_Pos                                  5
#define UART_ICR_RTIC_Pos                                  6
#define UART_ICR_FEIC_Pos                                  7
#define UART_ICR_PEIC_Pos                                  8
#define UART_ICR_BEIC_Pos                                  9
#define UART_ICR_OEIC_Pos                                  10
/* Bit field masks: */
#define UART_ICR_RIMIC                                     ((uint32_t)0x00000001)
#define UART_ICR_CTSMIC                                    ((uint32_t)0x00000002)
#define UART_ICR_DCDMIC                                    ((uint32_t)0x00000004)
#define UART_ICR_DSRMIC                                    ((uint32_t)0x00000008)
#define UART_ICR_RXIC                                      ((uint32_t)0x00000010)
#define UART_ICR_TXIC                                      ((uint32_t)0x00000020)
#define UART_ICR_RTIC                                      ((uint32_t)0x00000040)
#define UART_ICR_FEIC                                      ((uint32_t)0x00000080)
#define UART_ICR_PEIC                                      ((uint32_t)0x00000100)
#define UART_ICR_BEIC                                      ((uint32_t)0x00000200)
#define UART_ICR_OEIC                                      ((uint32_t)0x00000400)

/*-- UART_DMACR:UART DMACR Register -----*/
/* Bit field positions: */
#define UART_DMACR_RXDMAE_Pos                              0
#define UART_DMACR_TXDMAE_Pos                              1
#define UART_DMACR_DMAONERR_Pos                            2
/* Bit field masks: */
#define UART_DMACR_RXDMAE                                  ((uint32_t)0x00000001)
#define UART_DMACR_TXDMAE                                  ((uint32_t)0x00000002)
#define UART_DMACR_DMAONERR                                ((uint32_t)0x00000004)

/* _sdio structure */
typedef struct {
	__IO uint32_t	CR;                     /*!< */
	__IO uint32_t	SR;                     /*!< */
	__IO uint32_t	CMDDR;                  /*!< */
	__IO uint32_t	DATDR;                  /*!< */
	__IO uint32_t	CMDCRC;                 /*!< */
	__IO uint32_t	DAT0CRC;                /*!< */
	__IO uint32_t	DAT1CRC;                /*!< */
	__IO uint32_t	DAT2CRC;                /*!< */
	__IO uint32_t	DAT3CRC;                /*!< */
	__IO uint32_t	CMD_TRANSFER;           /*!< */
	__IO uint32_t	DAT_TRANSFER;           /*!< */
} _sdio;

/*-- SDIO_CR:SDIO CR Register -----*/
/* Bit field positions: */
#define SDIO_CR_SDE_Pos                                    0
#define SDIO_CR_DIRCMD_Pos                                 1
#define SDIO_CR_DIRDATA_Pos                                2
#define SDIO_CR_WORK1_Pos                                  3
#define SDIO_CR_SBITCMD_Pos                                4
#define SDIO_CR_SBITDAT_Pos                                5
#define SDIO_CR_BR_Pos                                     6
#define SDIO_CR_CRC_EN_DATA_Pos                            9
#define SDIO_CR_CRC_EN_CMD_Pos                             10
#define SDIO_CR_TXEIE_DAT_Pos                              11
#define SDIO_CR_RXNEIE_DAT_Pos                             12
#define SDIO_CR_RXFIE_DAT_Pos                              13
#define SDIO_CR_TXEIE_CMD_Pos                              14
#define SDIO_CR_RXNEIE_CMD_Pos                             15
#define SDIO_CR_RXFEIE_CMD_Pos                             16
#define SDIO_CR_WIDTHDAT_Pos                               17
#define SDIO_CR_WRITECMD_Pos                               18
#define SDIO_CR_ENDBUSY_Pos                                19
#define SDIO_CR_WORK2_Pos                                  20
#define SDIO_CR_CLKOE_Pos                                  21
/* Bit field masks: */
#define SDIO_CR_SDE                                        ((uint32_t)0x00000001)
#define SDIO_CR_DIRCMD                                     ((uint32_t)0x00000002)
#define SDIO_CR_DIRDATA                                    ((uint32_t)0x00000004)
#define SDIO_CR_WORK1                                      ((uint32_t)0x00000008)
#define SDIO_CR_SBITCMD                                    ((uint32_t)0x00000010)
#define SDIO_CR_SBITDAT                                    ((uint32_t)0x00000020)
#define SDIO_CR_BR_Msk                                     ((uint32_t)0x000001C0)
#define SDIO_CR_CRC_EN_DATA                                ((uint32_t)0x00000200)
#define SDIO_CR_CRC_EN_CMD                                 ((uint32_t)0x00000400)
#define SDIO_CR_TXEIE_DAT                                  ((uint32_t)0x00000800)
#define SDIO_CR_RXNEIE_DAT                                 ((uint32_t)0x00001000)
#define SDIO_CR_RXFIE_DAT                                  ((uint32_t)0x00002000)
#define SDIO_CR_TXEIE_CMD                                  ((uint32_t)0x00004000)
#define SDIO_CR_RXNEIE_CMD                                 ((uint32_t)0x00008000)
#define SDIO_CR_RXFEIE_CMD                                 ((uint32_t)0x00010000)
#define SDIO_CR_WIDTHDAT                                   ((uint32_t)0x00020000)
#define SDIO_CR_WRITECMD                                   ((uint32_t)0x00040000)
#define SDIO_CR_ENDBUSY                                    ((uint32_t)0x00080000)
#define SDIO_CR_WORK2                                      ((uint32_t)0x00100000)
#define SDIO_CR_CLKOE                                      ((uint32_t)0x00200000)

/*-- SDIO_SR:SDIO SR Register -----*/
/* Bit field positions: */
#define SDIO_SR_FIFOCMD_EMPTY_Pos                          0
#define SDIO_SR_FIFODAT_EMPTY_Pos                          1
#define SDIO_SR_FIFOCMD_FULL_Pos                           2
#define SDIO_SR_FIFODAT_FULL_Pos                           3
/* Bit field masks: */
#define SDIO_SR_FIFOCMD_EMPTY                              ((uint32_t)0x00000001)
#define SDIO_SR_FIFODAT_EMPTY                              ((uint32_t)0x00000002)
#define SDIO_SR_FIFOCMD_FULL                               ((uint32_t)0x00000004)
#define SDIO_SR_FIFODAT_FULL                               ((uint32_t)0x00000008)

/*-- SDIO_CMDDR:SDIO CMDDR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- SDIO_DATDR:SDIO DATDR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- SDIO_CMDCRC:SDIO CMDCRC Register -----*/
/* Bit field positions: */
#define SDIO_CMDCRC_CMDCRC_Pos                             0
/* Bit field masks: */
#define SDIO_CMDCRC_CMDCRC_Msk                             ((uint32_t)0x0000007F)

/*-- SDIO_DAT0CRC:SDIO DAT0CRC Register -----*/
/* Bit field positions: */
#define SDIO_DAT0CRC_DAT0CRC_Pos                           0
/* Bit field masks: */
#define SDIO_DAT0CRC_DAT0CRC_Msk                           ((uint32_t)0x0000FFFF)

/*-- SDIO_DAT1CRC:SDIO DAT1CRC Register -----*/
/* Bit field positions: */
#define SDIO_DAT1CRC_DAT1CRC_Pos                           0
/* Bit field masks: */
#define SDIO_DAT1CRC_DAT1CRC_Msk                           ((uint32_t)0x0000FFFF)

/*-- SDIO_DAT2CRC:SDIO DAT2CRC Register -----*/
/* Bit field positions: */
#define SDIO_DAT2CRC_DAT2CRC_Pos                           0
/* Bit field masks: */
#define SDIO_DAT2CRC_DAT2CRC_Msk                           ((uint32_t)0x0000FFFF)

/*-- SDIO_DAT3CRC:SDIO DAT3CRC Register -----*/
/* Bit field positions: */
#define SDIO_DAT3CRC_DAT3CRC_Pos                           0
/* Bit field masks: */
#define SDIO_DAT3CRC_DAT3CRC_Msk                           ((uint32_t)0x0000FFFF)

/*-- SDIO_CMD_TRANSFER:SDIO CMD_TRANSFER Register -----*/
/* Bit field positions: */
#define SDIO_CMD_TRANSFER_CMD_TRANSFER_Pos                 0
/* Bit field masks: */
#define SDIO_CMD_TRANSFER_CMD_TRANSFER_Msk                 ((uint32_t)0x0000FFFF)

/*-- SDIO_DAT_TRANSFER:SDIO DAT_TRANSFER Register -----*/
/* Bit field positions: */
#define SDIO_DAT_TRANSFER_DAT_TRANSFER_Pos                 0
/* Bit field masks: */
#define SDIO_DAT_TRANSFER_DAT_TRANSFER_Msk                 ((uint32_t)0x0000FFFF)

/* _i2c structure */
typedef struct {
	__IO uint32_t	PRL;                    /*!<I2C Prescaler (low byte) Register */
	__IO uint32_t	PRH;                    /*!<I2C Prescaler (high byte) Register */
	__IO uint32_t	CTR;                    /*!<I2C Control Register */
	__IO uint32_t	RXD;                    /*!<I2C Received Data Register */
	__IO uint32_t	STA;                    /*!<I2C Status Register */
	__IO uint32_t	TXD;                    /*!<I2C Transmitted Data Register */
	__IO uint32_t	CMD;                    /*!<I2C Command Register */
} _i2c;

/*-- I2C_PRL:I2C PRL Register -----*/
/* Bit field positions: */
#define I2C_PRL_PR_Pos                                     0
/* Bit field masks: */
#define I2C_PRL_PR_Msk                                     ((uint32_t)0x000000FF)

/*-- I2C_PRH:I2C PRH Register -----*/
/* Bit field positions: */
#define I2C_PRH_PR_Pos                                     0
/* Bit field masks: */
#define I2C_PRH_PR_Msk                                     ((uint32_t)0x000000FF)

/*-- I2C_CTR:I2C CTR Register -----*/
/* Bit field positions: */
#define I2C_CTR_S_I2C_Pos                                  5
#define I2C_CTR_EN_INT_Pos                                 6
#define I2C_CTR_EN_I2C_Pos                                 7
/* Bit field masks: */
#define I2C_CTR_S_I2C                                      ((uint32_t)0x00000020)
#define I2C_CTR_EN_INT                                     ((uint32_t)0x00000040)
#define I2C_CTR_EN_I2C                                     ((uint32_t)0x00000080)

/*-- I2C_RXD:I2C RXD Register -----*/
/* Bit field positions: */
#define I2C_RXD_RXD_Pos                                    0
/* Bit field masks: */
#define I2C_RXD_RXD_Msk                                    ((uint32_t)0x000000FF)

/*-- I2C_STA:I2C STA Register -----*/
/* Bit field positions: */
#define I2C_STA_INT_Pos                                    0
#define I2C_STA_TR_PROG_Pos                                1
#define I2C_STA_LOST_ARB_Pos                               5
#define I2C_STA_BUSY_Pos                                   6
#define I2C_STA_RX_ACK_Pos                                 7
/* Bit field masks: */
#define I2C_STA_INT                                        ((uint32_t)0x00000001)
#define I2C_STA_TR_PROG                                    ((uint32_t)0x00000002)
#define I2C_STA_LOST_ARB                                   ((uint32_t)0x00000020)
#define I2C_STA_BUSY                                       ((uint32_t)0x00000040)
#define I2C_STA_RX_ACK                                     ((uint32_t)0x00000080)

/*-- I2C_TXD:I2C TXD Register -----*/
/* Bit field positions: */
#define I2C_TXD_TXD_Pos                                    0
/* Bit field masks: */
#define I2C_TXD_TXD_Msk                                    ((uint32_t)0x000000FF)

/*-- I2C_CMD:I2C CMD Register -----*/
/* Bit field positions: */
#define I2C_CMD_CLRINT_Pos                                 0
#define I2C_CMD_ACK_Pos                                    3
#define I2C_CMD_WR_Pos                                     4
#define I2C_CMD_RD_Pos                                     5
#define I2C_CMD_STOP_Pos                                   6
#define I2C_CMD_START_Pos                                  7
/* Bit field masks: */
#define I2C_CMD_CLRINT                                     ((uint32_t)0x00000001)
#define I2C_CMD_ACK                                        ((uint32_t)0x00000008)
#define I2C_CMD_WR                                         ((uint32_t)0x00000010)
#define I2C_CMD_RD                                         ((uint32_t)0x00000020)
#define I2C_CMD_STOP                                       ((uint32_t)0x00000040)
#define I2C_CMD_START                                      ((uint32_t)0x00000080)

/* _power structure */
typedef struct {
	__IO uint32_t	PVDCS;                  /*!<POWER Power Detector Control/Status Register */
} _power;

/*-- POWER_PVDCS:POWER PVDCS Register -----*/
/* Bit field positions: */
#define POWER_PVDCS_PVDEN_Pos                              0
#define POWER_PVDCS_PBLS_Pos                               1
#define POWER_PVDCS_PLS_Pos                                3
#define POWER_PVDCS_PVBD_Pos                               6
#define POWER_PVDCS_PVD_Pos                                7
#define POWER_PVDCS_IEPVBD_Pos                             8
#define POWER_PVDCS_IEPVD_Pos                              9
#define POWER_PVDCS_INVB_Pos                               10
#define POWER_PVDCS_INV_Pos                                11
/* Bit field masks: */
#define POWER_PVDCS_PVDEN                                  ((uint32_t)0x00000001)
#define POWER_PVDCS_PBLS_Msk                               ((uint32_t)0x00000006)
#define POWER_PVDCS_PLS_Msk                                ((uint32_t)0x00000038)
#define POWER_PVDCS_PVBD                                   ((uint32_t)0x00000040)
#define POWER_PVDCS_PVD                                    ((uint32_t)0x00000080)
#define POWER_PVDCS_IEPVBD                                 ((uint32_t)0x00000100)
#define POWER_PVDCS_IEPVD                                  ((uint32_t)0x00000200)
#define POWER_PVDCS_INVB                                   ((uint32_t)0x00000400)
#define POWER_PVDCS_INV                                    ((uint32_t)0x00000800)

/* _wwdg structure */
typedef struct {
	__IO uint32_t	CR;                     /*!<WWDG Command Register */
	__IO uint32_t	CFR;                    /*!<WWDG Configuration Register */
	__IO uint32_t	SR;                     /*!<WWDG Status Register */
} _wwdg;

/*-- WWDG_CR:WWDG CR Register -----*/
/* Bit field positions: */
#define WWDG_CR_T_Pos                                      0
#define WWDG_CR_WDGA_Pos                                   7
/* Bit field masks: */
#define WWDG_CR_T_Msk                                      ((uint32_t)0x0000007F)
#define WWDG_CR_WDGA                                       ((uint32_t)0x00000080)

/*-- WWDG_CFR:WWDG CFR Register -----*/
/* Bit field positions: */
#define WWDG_CFR_W_Pos                                     0
#define WWDG_CFR_WGTB_Pos                                  7
#define WWDG_CFR_EWI_Pos                                   9
/* Bit field masks: */
#define WWDG_CFR_W_Msk                                     ((uint32_t)0x0000007F)
#define WWDG_CFR_WGTB_Msk                                  ((uint32_t)0x00000180)
#define WWDG_CFR_EWI                                       ((uint32_t)0x00000200)

/*-- WWDG_SR:WWDG SR Register -----*/
/* Bit field positions: */
#define WWDG_SR_EWIF_Pos                                   0
/* Bit field masks: */
#define WWDG_SR_EWIF                                       ((uint32_t)0x00000001)

/* _iwdg structure */
typedef struct {
	__IO uint32_t	KR;                     /*!<IWDG Key Register */
	__IO uint32_t	PR;                     /*!<IWDG Clock Prescaler Register */
	__IO uint32_t	RLR;                    /*!<IWDG Reload Register */
	__IO uint32_t	SR;                     /*!<IWDG Status Register */
} _iwdg;

/*-- IWDG_KR:IWDG KR Register -----*/
/* Bit field positions: */
#define IWDG_KR_KEY_Pos                                    0
/* Bit field masks: */
#define IWDG_KR_KEY_Msk                                    ((uint32_t)0x0000FFFF)

/*-- IWDG_PR:IWDG PR Register -----*/
/* Bit field positions: */
#define IWDG_PR_PR_Pos                                     0
/* Bit field masks: */
#define IWDG_PR_PR_Msk                                     ((uint32_t)0x00000007)

/*-- IWDG_RLR:IWDG RLR Register -----*/
/* Bit field positions: */
#define IWDG_RLR_RLR_Pos                                   0
/* Bit field masks: */
#define IWDG_RLR_RLR_Msk                                   ((uint32_t)0x00000FFF)

/*-- IWDG_SR:IWDG SR Register -----*/
/* Bit field positions: */
#define IWDG_SR_PVU_Pos                                    0
#define IWDG_SR_RVU_Pos                                    1
/* Bit field masks: */
#define IWDG_SR_PVU                                        ((uint32_t)0x00000001)
#define IWDG_SR_RVU                                        ((uint32_t)0x00000002)

/* _timer structure */
typedef struct {
	__IO uint32_t	CNT;                    /*!<Timer Counter Register */
	__IO uint32_t	PSG;                    /*!<Timer Clock Prescaler Register */
	__IO uint32_t	ARR;                    /*!<Timer Auto-Reload Register */
	__IO uint32_t	CNTRL;                  /*!<Timer Control Register */
	__IO uint32_t	CCR1;                   /*!<Timer Capture/Compare Register 1 */
	__IO uint32_t	CCR2;                   /*!<Timer Capture/Compare Register 2 */
	__IO uint32_t	CCR3;                   /*!<Timer Capture/Compare Register 3 */
	__IO uint32_t	CCR4;                   /*!<Timer Capture/Compare Register 4 */
	__IO uint32_t	CH1_CNTRL;              /*!<Timer Channel Control Register 1 */
	__IO uint32_t	CH2_CNTRL;              /*!<Timer Channel Control Register 2 */
	__IO uint32_t	CH3_CNTRL;              /*!<Timer Channel Control Register 3 */
	__IO uint32_t	CH4_CNTRL;              /*!<Timer Channel Control Register 4 */
	__IO uint32_t	CH1_CNTRL1;             /*!<Timer Channel Control1 Register 1 */
	__IO uint32_t	CH2_CNTRL1;             /*!<Timer Channel Control1 Register 2 */
	__IO uint32_t	CH3_CNTRL1;             /*!<Timer Channel Control1 Register 3 */
	__IO uint32_t	CH4_CNTRL1;             /*!<Timer Channel Control1 Register 4 */
	__IO uint32_t	CH1_DTG;                /*!<Timer Channel DTG Register 1 */
	__IO uint32_t	CH2_DTG;                /*!<Timer Channel DTG Register 2 */
	__IO uint32_t	CH3_DTG;                /*!<Timer Channel DTG Register 3 */
	__IO uint32_t	CH4_DTG;                /*!<Timer Channel DTG Register 4 */
	__IO uint32_t	BRKETR_CNTRL;           /*!<Timer BRK/ETR Control Register */
	__IO uint32_t	STATUS;                 /*!<Timer Status Register */
	__IO uint32_t	IE;                     /*!<Timer Interrupt Enable Register */
	__IO uint32_t	DMA_RE;                 /*!<Timer DMA Request Enable Register */
	__IO uint32_t	CH1_CNTRL2;             /*!<Timer Channel Control2 Register 1 */
	__IO uint32_t	CH2_CNTRL2;             /*!<Timer Channel Control2 Register 2 */
	__IO uint32_t	CH3_CNTRL2;             /*!<Timer Channel Control2 Register 3 */
	__IO uint32_t	CH4_CNTRL2;             /*!<Timer Channel Control2 Register 4 */
	__IO uint32_t	CCR11;                  /*!<Timer Capture/Compare1 Register 1 */
	__IO uint32_t	CCR21;                  /*!<Timer Capture/Compare1 Register 2 */
	__IO uint32_t	CCR31;                  /*!<Timer Capture/Compare1 Register 3 */
	__IO uint32_t	CCR41;                  /*!<Timer Capture/Compare1 Register 4 */
} _timer;

/*-- TIMER_CNT:TIMER CNT Register -----*/
/* Bit field positions: */
#define TIMER_CNT_CNT_Pos                                  0
/* Bit field masks: */
#define TIMER_CNT_CNT_Msk                                  ((uint32_t)0x0000FFFF)

/*-- TIMER_PSG:TIMER PSG Register -----*/
/* Bit field positions: */
#define TIMER_PSG_PSG_Pos                                  0
/* Bit field masks: */
#define TIMER_PSG_PSG_Msk                                  ((uint32_t)0x0000FFFF)

/*-- TIMER_ARR:TIMER ARR Register -----*/
/* Bit field positions: */
#define TIMER_ARR_ARR_Pos                                  0
/* Bit field masks: */
#define TIMER_ARR_ARR_Msk                                  ((uint32_t)0x0000FFFF)

/*-- TIMER_CNTRL:TIMER CNTRL Register -----*/
/* Bit field positions: */
#define TIMER_CNTRL_CNT_EN_Pos                             0
#define TIMER_CNTRL_ARRB_EN_Pos                            1
#define TIMER_CNTRL_WR_CMPL_Pos                            2
#define TIMER_CNTRL_DIR_Pos                                3
#define TIMER_CNTRL_FDTS_Pos                               4
#define TIMER_CNTRL_CNT_MODE_Pos                           6
#define TIMER_CNTRL_EVENT_SEL_Pos                          8
/* Bit field masks: */
#define TIMER_CNTRL_CNT_EN                                 ((uint32_t)0x00000001)
#define TIMER_CNTRL_ARRB_EN                                ((uint32_t)0x00000002)
#define TIMER_CNTRL_WR_CMPL                                ((uint32_t)0x00000004)
#define TIMER_CNTRL_DIR                                    ((uint32_t)0x00000008)
#define TIMER_CNTRL_FDTS_Msk                               ((uint32_t)0x00000030)
#define TIMER_CNTRL_CNT_MODE_Msk                           ((uint32_t)0x000000C0)
#define TIMER_CNTRL_EVENT_SEL_Msk                          ((uint32_t)0x00000F00)

/*-- TIMER_CCR:TIMER CCR Register -----*/
/* Bit field positions: */
#define TIMER_CCR_CCR_Pos                                  0
/* Bit field masks: */
#define TIMER_CCR_CCR_Msk                                  ((uint32_t)0x0000FFFF)

/*-- TIMER_CH_CNTRL:TIMER CH_CNTRL Register -----*/
/* Bit field positions: */
#define TIMER_CH_CNTRL_CHFLTR_Pos                          0
#define TIMER_CH_CNTRL_CHSEL_Pos                           4
#define TIMER_CH_CNTRL_CHPSC_Pos                           6
#define TIMER_CH_CNTRL_OCCE_Pos                            8
#define TIMER_CH_CNTRL_OCCM_Pos                            9
#define TIMER_CH_CNTRL_BRKEN_Pos                           12
#define TIMER_CH_CNTRL_ETREN_Pos                           13
#define TIMER_CH_CNTRL_WR_CMPL_Pos                         14
#define TIMER_CH_CNTRL_CAP_NPWM_Pos                        15
/* Bit field masks: */
#define TIMER_CH_CNTRL_CHFLTR_Msk                          ((uint32_t)0x0000000F)
#define TIMER_CH_CNTRL_CHSEL_Msk                           ((uint32_t)0x00000030)
#define TIMER_CH_CNTRL_CHPSC_Msk                           ((uint32_t)0x000000C0)
#define TIMER_CH_CNTRL_OCCE                                ((uint32_t)0x00000100)
#define TIMER_CH_CNTRL_OCCM_Msk                            ((uint32_t)0x00000E00)
#define TIMER_CH_CNTRL_BRKEN                               ((uint32_t)0x00001000)
#define TIMER_CH_CNTRL_ETREN                               ((uint32_t)0x00002000)
#define TIMER_CH_CNTRL_WR_CMPL                             ((uint32_t)0x00004000)
#define TIMER_CH_CNTRL_CAP_NPWM                            ((uint32_t)0x00008000)

/*-- TIMER_CH_CNTRL1:TIMER CH_CNTRL1 Register -----*/
/* Bit field positions: */
#define TIMER_CH_CNTRL1_SELOE_Pos                          0
#define TIMER_CH_CNTRL1_SELO_Pos                           2
#define TIMER_CH_CNTRL1_INV_Pos                            4
#define TIMER_CH_CNTRL1_NSELOE_Pos                         8
#define TIMER_CH_CNTRL1_NSELO_Pos                          10
#define TIMER_CH_CNTRL1_NINV_Pos                           12
/* Bit field masks: */
#define TIMER_CH_CNTRL1_SELOE_Msk                          ((uint32_t)0x00000003)
#define TIMER_CH_CNTRL1_SELO_Msk                           ((uint32_t)0x0000000C)
#define TIMER_CH_CNTRL1_INV                                ((uint32_t)0x00000010)
#define TIMER_CH_CNTRL1_NSELOE_Msk                         ((uint32_t)0x00000300)
#define TIMER_CH_CNTRL1_NSELO_Msk                          ((uint32_t)0x00000C00)
#define TIMER_CH_CNTRL1_NINV                               ((uint32_t)0x00001000)

/*-- TIMER_CH_DTG:TIMER CH_DTG Register -----*/
/* Bit field positions: */
#define TIMER_CH_DTGX_Pos                              0
#define TIMER_CH_DTG_EDTS_Pos                          4
#define TIMER_CH_DTG_Pos                               8
/* Bit field masks: */
#define TIMER_CH_DTGX_Msk                              ((uint32_t)0x0000000F)
#define TIMER_CH_DTG_EDTS                              ((uint32_t)0x00000010)
#define TIMER_CH_DTG_Msk                               ((uint32_t)0x0000FF00)

/*-- TIMER_BRKETR_CNTRL:TIMER BRKETR_CNTRL Register -----*/
/* Bit field positions: */
#define TIMER_BRKETR_CNTRL_BRK_INV_Pos                     0
#define TIMER_BRKETR_CNTRL_ETR_INV_Pos                     1
#define TIMER_BRKETR_CNTRL_ETR_PSC_Pos                     2
#define TIMER_BRKETR_CNTRL_ETR_FILTER_Pos                  4
/* Bit field masks: */
#define TIMER_BRKETR_CNTRL_BRK_INV                         ((uint32_t)0x00000001)
#define TIMER_BRKETR_CNTRL_ETR_INV                         ((uint32_t)0x00000002)
#define TIMER_BRKETR_CNTRL_ETR_PSC_Msk                     ((uint32_t)0x0000000C)
#define TIMER_BRKETR_CNTRL_ETR_FILTER_Msk                  ((uint32_t)0x000000F0)

/*-- TIMER_STATUS:TIMER STATUS Register -----*/
/* Bit field positions: */
#define TIMER_STATUS_CNT_ZERO_EVENT_Pos                    0
#define TIMER_STATUS_CNT_ARR_EVENT_Pos                     1
#define TIMER_STATUS_ETR_RE_EVENT_Pos                      2
#define TIMER_STATUS_ETR_FE_EVENT_Pos                      3
#define TIMER_STATUS_BRK_EVENT_Pos                         4
#define TIMER_STATUS_CCR_CAP_EVENT_Pos                     5
#define TIMER_STATUS_CCR_REF_EVENT_Pos                     9
#define TIMER_STATUS_CCR1_CAP_EVENT_Pos                    13
/* Bit field masks: */
#define TIMER_STATUS_CNT_ZERO_EVENT                        ((uint32_t)0x00000001)
#define TIMER_STATUS_CNT_ARR_EVENT                         ((uint32_t)0x00000002)
#define TIMER_STATUS_ETR_RE_EVENT                          ((uint32_t)0x00000004)
#define TIMER_STATUS_ETR_FE_EVENT                          ((uint32_t)0x00000008)
#define TIMER_STATUS_BRK_EVENT                             ((uint32_t)0x00000010)
#define TIMER_STATUS_CCR_CAP_EVENT_Msk                     ((uint32_t)0x000001E0)
#define TIMER_STATUS_CCR_REF_EVENT_Msk                     ((uint32_t)0x00001E00)
#define TIMER_STATUS_CCR1_CAP_EVENT_Msk                    ((uint32_t)0x0001E000)

/*-- TIMER_IE:TIMER IE Register -----*/
/* Bit field positions: */
#define TIMER_IE_CNT_ZERO_EVENT_IE_Pos                     0
#define TIMER_IE_CNT_ARR_EVENT_IE_Pos                      1
#define TIMER_IE_ETR_RE_EVENT_IE_Pos                       2
#define TIMER_IE_ETR_FE_EVENT_IE_Pos                       3
#define TIMER_IE_BRK_EVENT_IE_Pos                          4
#define TIMER_IE_CCR_CAP_EVENT_IE_Pos                      5
#define TIMER_IE_CCR_REF_EVENT_IE_Pos                      9
#define TIMER_IE_CCR1_CAP_EVENT_IE_Pos                     13
/* Bit field masks: */
#define TIMER_IE_CNT_ZERO_EVENT_IE                         ((uint32_t)0x00000001)
#define TIMER_IE_CNT_ARR_EVENT_IE                          ((uint32_t)0x00000002)
#define TIMER_IE_ETR_RE_EVENT_IE                           ((uint32_t)0x00000004)
#define TIMER_IE_ETR_FE_EVENT_IE                           ((uint32_t)0x00000008)
#define TIMER_IE_BRK_EVENT_IE                              ((uint32_t)0x00000010)
#define TIMER_IE_CCR_CAP_EVENT_IE_Msk                      ((uint32_t)0x000001E0)
#define TIMER_IE_CCR_REF_EVENT_IE_Msk                      ((uint32_t)0x00001E00)
#define TIMER_IE_CCR1_CAP_EVENT_IE_Msk                     ((uint32_t)0x0001E000)

/*-- TIMER_DMA_RE:TIMER DMA_RE Register -----*/
/* Bit field positions: */
#define TIMER_DMA_RE_CNT_ZERO_EVENT_RE_Pos                 0
#define TIMER_DMA_RE_CNT_ARR_EVENT_RE_Pos                  1
#define TIMER_DMA_RE_ETR_RE_EVENT_RE_Pos                   2
#define TIMER_DMA_RE_ETR_FE_EVENT_RE_Pos                   3
#define TIMER_DMA_RE_BRK_EVENT_RE_Pos                      4
#define TIMER_DMA_RE_CCR_CAP_EVENT_RE_Pos                  5
#define TIMER_DMA_RE_CCR_REF_EVENT_RE_Pos                  9
#define TIMER_DMA_RE_CCR1_CAP_EVENT_RE_Pos                 13
/* Bit field masks: */
#define TIMER_DMA_RE_CNT_ZERO_EVENT_RE                     ((uint32_t)0x00000001)
#define TIMER_DMA_RE_CNT_ARR_EVENT_RE                      ((uint32_t)0x00000002)
#define TIMER_DMA_RE_ETR_RE_EVENT_RE                       ((uint32_t)0x00000004)
#define TIMER_DMA_RE_ETR_FE_EVENT_RE                       ((uint32_t)0x00000008)
#define TIMER_DMA_RE_BRK_EVENT_RE                          ((uint32_t)0x00000010)
#define TIMER_DMA_RE_CCR_CAP_EVENT_RE_Msk                  ((uint32_t)0x000001E0)
#define TIMER_DMA_RE_CCR_REF_EVENT_RE_Msk                  ((uint32_t)0x00001E00)
#define TIMER_DMA_RE_CCR1_CAP_EVENT_RE_Msk                 ((uint32_t)0x0001E000)

/*-- TIMER_CH_CNTRL2:TIMER CH_CNTRL2 Register -----*/
/* Bit field positions: */
#define TIMER_CH_CNTRL2_CHSEL1_Pos                         0
#define TIMER_CH_CNTRL2_CCR1_EN_Pos                        2
#define TIMER_CH_CNTRL2_CCRRLD_Pos                         3
/* Bit field masks: */
#define TIMER_CH_CNTRL2_CHSEL1_Msk                         ((uint32_t)0x00000003)
#define TIMER_CH_CNTRL2_CCR1_EN                            ((uint32_t)0x00000004)
#define TIMER_CH_CNTRL2_CCRRLD                             ((uint32_t)0x00000008)

/*-- TIMER_CCR1:TIMER CCR1 Register -----*/
/* Bit field positions: */
#define TIMER_CCR1_CCR_Pos                                 0
/* Bit field masks: */
#define TIMER_CCR1_CCR_Msk                                 ((uint32_t)0x0000FFFF)

/* _adc structure */
typedef struct {
	__IO uint32_t	ADC1_CFG;               /*!<ADC1 Configuration Register */
	__IO uint32_t	ADC2_CFG;               /*!<ADC2 Configuration Register */
	__IO uint32_t	ADC1_H_LEVEL;           /*!<ADC1 High Level Register */
	__IO uint32_t	ADC2_H_LEVEL;           /*!<ADC2 High Level Register */
	__IO uint32_t	ADC1_L_LEVEL;           /*!<ADC1 Low Level Register */
	__IO uint32_t	ADC2_L_LEVEL;           /*!<ADC2 Low Level Register */
	__IO uint32_t	ADC1_RESULT;            /*!<ADC1 Result Register */
	__IO uint32_t	ADC2_RESULT;            /*!<ADC2 Result Register */
	__IO uint32_t	ADC1_STATUS;            /*!<ADC1 Status Register */
	__IO uint32_t	ADC2_STATUS;            /*!<ADC2 Status Register */
	__IO uint32_t	ADC1_CHSEL;             /*!<ADC1 Channel Selector Register */
	__IO uint32_t	ADC2_CHSEL;             /*!<ADC2 Channel Selector Register */
} _adc;

/*-- ADC1_CFG: ADC1_CFG Register -----*/
/* Bit field positions: */
#define ADC1_CFG_REG_ADON_Pos                              0
#define ADC1_CFG_REG_GO_Pos                                1
#define ADC1_CFG_REG_CLKS_Pos                              2
#define ADC1_CFG_REG_SAMPLE_Pos                            3
#define ADC1_CFG_REG_CHS_Pos                               4
#define ADC1_CFG_REG_CHCH_Pos                              9
#define ADC1_CFG_REG_RNGC_Pos                              10
#define ADC1_CFG_M_REF_Pos                                 11
#define ADC1_CFG_REG_DIVCLK_Pos                            12
#define ADC1_CFG_SYNC_CONVER_Pos                           16
#define ADC1_CFG_TS_EN_Pos                                 17
#define ADC1_CFG_TS_BUF_EN_Pos                             18
#define ADC1_CFG_SEL_TS_Pos                                19
#define ADC1_CFG_SEL_VREF_Pos                              20
#define ADC1_CFG_TR_Pos                                    21
#define ADC1_CFG_DELAY_GO_Pos                              25
#define ADC1_CFG_DELAY_ADC_Pos                             28
/* Bit field masks: */
#define ADC1_CFG_REG_ADON                                  ((uint32_t)0x00000001)
#define ADC1_CFG_REG_GO                                    ((uint32_t)0x00000002)
#define ADC1_CFG_REG_CLKS                                  ((uint32_t)0x00000004)
#define ADC1_CFG_REG_SAMPLE                                ((uint32_t)0x00000008)
#define ADC1_CFG_REG_CHS_Msk                               ((uint32_t)0x000001F0)
#define ADC1_CFG_REG_CHCH                                  ((uint32_t)0x00000200)
#define ADC1_CFG_REG_RNGC                                  ((uint32_t)0x00000400)
#define ADC1_CFG_M_REF                                     ((uint32_t)0x00000800)
#define ADC1_CFG_REG_DIVCLK_Msk                            ((uint32_t)0x0000F000)
#define ADC1_CFG_SYNC_CONVER                               ((uint32_t)0x00010000)
#define ADC1_CFG_TS_EN                                     ((uint32_t)0x00020000)
#define ADC1_CFG_TS_BUF_EN                                 ((uint32_t)0x00040000)
#define ADC1_CFG_SEL_TS                                    ((uint32_t)0x00080000)
#define ADC1_CFG_SEL_VREF                                  ((uint32_t)0x00100000)
#define ADC1_CFG_TR_Msk                                    ((uint32_t)0x01E00000)
#define ADC1_CFG_DELAY_GO_Msk                              ((uint32_t)0x0E000000)
#define ADC1_CFG_DELAY_ADC_Msk                             ((uint32_t)0xF0000000)

/*-- ADC2_CFG: ADC2_CFG Register -----*/
/* Bit field positions: */
#define ADC2_CFG_REG_ADON_Pos                              0
#define ADC2_CFG_REG_GO_Pos                                1
#define ADC2_CFG_REG_CLKS_Pos                              2
#define ADC2_CFG_REG_SAMPLE_Pos                            3
#define ADC2_CFG_REG_CHS_Pos                               4
#define ADC2_CFG_REG_CHCH_Pos                              9
#define ADC2_CFG_REG_RNGC_Pos                              10
#define ADC2_CFG_M_REF_Pos                                 11
#define ADC2_CFG_REG_DIVCLK_Pos                            12
#define ADC2_CFG_ADC1_OP_Pos                               17
#define ADC2_CFG_ADC2_OP_Pos                               18
#define ADC2_CFG_DELAY_GO_Pos                              25
/* Bit field masks: */
#define ADC2_CFG_REG_ADON                                  ((uint32_t)0x00000001)
#define ADC2_CFG_REG_GO                                    ((uint32_t)0x00000002)
#define ADC2_CFG_REG_CLKS                                  ((uint32_t)0x00000004)
#define ADC2_CFG_REG_SAMPLE                                ((uint32_t)0x00000008)
#define ADC2_CFG_REG_CHS_Msk                               ((uint32_t)0x000001F0)
#define ADC2_CFG_REG_CHCH                                  ((uint32_t)0x00000200)
#define ADC2_CFG_REG_RNGC                                  ((uint32_t)0x00000400)
#define ADC2_CFG_M_REF                                     ((uint32_t)0x00000800)
#define ADC2_CFG_REG_DIVCLK_Msk                            ((uint32_t)0x0000F000)
#define ADC2_CFG_ADC1_OP                                   ((uint32_t)0x00020000)
#define ADC2_CFG_ADC2_OP                                   ((uint32_t)0x00040000)
#define ADC2_CFG_DELAY_GO_Msk                              ((uint32_t)0x0E000000)

/*-- ADC1_H_LEVEL: ADC1_H_LEVEL Register -----*/
/* Bit field positions: */
#define ADC1_H_LEVEL_REG_H_LEVEL_Pos                       0
/* Bit field masks: */
#define ADC1_H_LEVEL_REG_H_LEVEL_Msk                       ((uint32_t)0x00000FFF)

/*-- ADC2_H_LEVEL: ADC2_H_LEVEL Register -----*/
/* Bit field positions: */
#define ADC2_H_LEVEL_REG_H_LEVEL_Pos                       0
/* Bit field masks: */
#define ADC2_H_LEVEL_REG_H_LEVEL_Msk                       ((uint32_t)0x00000FFF)

/*-- ADC1_L_LEVEL: ADC1_L_LEVEL Register -----*/
/* Bit field positions: */
#define ADC1_L_LEVEL_REG_L_LEVEL_Pos                       0
/* Bit field masks: */
#define ADC1_L_LEVEL_REG_L_LEVEL_Msk                       ((uint32_t)0x00000FFF)

/*-- ADC2_L_LEVEL: ADC2_L_LEVEL Register -----*/
/* Bit field positions: */
#define ADC2_L_LEVEL_REG_L_LEVEL_Pos                       0
/* Bit field masks: */
#define ADC2_L_LEVEL_REG_L_LEVEL_Msk                       ((uint32_t)0x00000FFF)

/*-- ADC1_RESULT: ADC1_RESULT Register -----*/
/* Bit field positions: */
#define ADC1_RESULT_RESULT_Pos                             0
#define ADC1_RESULT_CHANNEL_Pos                            16
/* Bit field masks: */
#define ADC1_RESULT_RESULT_Msk                             ((uint32_t)0x00000FFF)
#define ADC1_RESULT_CHANNEL_Msk                            ((uint32_t)0x0FFF0000)

/*-- ADC2_RESULT: ADC2_RESULT Register -----*/
/* Bit field positions: */
#define ADC2_RESULT_RESULT_Pos                             0
#define ADC2_RESULT_CHANNEL_Pos                            16
/* Bit field masks: */
#define ADC2_RESULT_RESULT_Msk                             ((uint32_t)0x00000FFF)
#define ADC2_RESULT_CHANNEL_Msk                            ((uint32_t)0x0FFF0000)

/*-- ADC1_STATUS: ADC1_STATUS Register -----*/
/* Bit field positions: */
#define ADC_STATUS_FLG_REG_OVERWRITE_Pos                   0
#define ADC_STATUS_FLG_REG_AWOIFEN_Pos                     1
#define ADC_STATUS_FLG_REG_EOCIF_Pos                       2
#define ADC_STATUS_AWOIF_IE_Pos                            3
#define ADC_STATUS_ECOIF_IE_Pos                            4
/* Bit field masks: */
#define ADC_STATUS_FLG_REG_OVERWRITE                       ((uint32_t)0x00000001)
#define ADC_STATUS_FLG_REG_AWOIFEN                         ((uint32_t)0x00000002)
#define ADC_STATUS_FLG_REG_EOCIF                           ((uint32_t)0x00000004)
#define ADC_STATUS_AWOIF_IE                                ((uint32_t)0x00000008)
#define ADC_STATUS_ECOIF_IE                                ((uint32_t)0x00000010)

/*-- ADC1_CHSEL: ADC1_CHSEL Register -----*/
/* Bit field positions: */
#define ADC1_CHSEL_SL_CH_CH_REF0_Pos                       0
#define ADC1_CHSEL_SL_CH_CH_REF1_Pos                       1
#define ADC1_CHSEL_SL_CH_CH_REF2_Pos                       2
#define ADC1_CHSEL_SL_CH_CH_REF3_Pos                       3
#define ADC1_CHSEL_SL_CH_CH_REF4_Pos                       4
#define ADC1_CHSEL_SL_CH_CH_REF5_Pos                       5
#define ADC1_CHSEL_SL_CH_CH_REF6_Pos                       6
#define ADC1_CHSEL_SL_CH_CH_REF7_Pos                       7
#define ADC1_CHSEL_SL_CH_CH_REF8_Pos                       8
#define ADC1_CHSEL_SL_CH_CH_REF9_Pos                       9
#define ADC1_CHSEL_SL_CH_CH_REF10_Pos                      10
#define ADC1_CHSEL_SL_CH_CH_REF11_Pos                      11
#define ADC1_CHSEL_SL_CH_CH_REF12_Pos                      12
#define ADC1_CHSEL_SL_CH_CH_REF13_Pos                      13
#define ADC1_CHSEL_SL_CH_CH_REF14_Pos                      14
#define ADC1_CHSEL_SL_CH_CH_REF15_Pos                      15
#define ADC1_CHSEL_SL_CH_CH_REF16_Pos                      16
#define ADC1_CHSEL_SL_CH_CH_REF17_Pos                      17
#define ADC1_CHSEL_SL_CH_CH_REF18_Pos                      18
#define ADC1_CHSEL_SL_CH_CH_REF19_Pos                      19
#define ADC1_CHSEL_SL_CH_CH_REF20_Pos                      20
#define ADC1_CHSEL_SL_CH_CH_REF21_Pos                      21
#define ADC1_CHSEL_SL_CH_CH_REF22_Pos                      22
#define ADC1_CHSEL_SL_CH_CH_REF23_Pos                      23
#define ADC1_CHSEL_SL_CH_CH_REF24_Pos                      24
#define ADC1_CHSEL_SL_CH_CH_REF25_Pos                      25
#define ADC1_CHSEL_SL_CH_CH_REF26_Pos                      26
#define ADC1_CHSEL_SL_CH_CH_REF27_Pos                      27
#define ADC1_CHSEL_SL_CH_CH_REF28_Pos                      28
#define ADC1_CHSEL_SL_CH_CH_REF29_Pos                      29
#define ADC1_CHSEL_SL_CH_CH_REF30_Pos                      30
#define ADC1_CHSEL_SL_CH_CH_REF31_Pos                      31
/* Bit field masks: */
#define ADC1_CHSEL_SL_CH_CH_REF0                           ((uint32_t)0x00000001)
#define ADC1_CHSEL_SL_CH_CH_REF1                           ((uint32_t)0x00000002)
#define ADC1_CHSEL_SL_CH_CH_REF2                           ((uint32_t)0x00000004)
#define ADC1_CHSEL_SL_CH_CH_REF3                           ((uint32_t)0x00000008)
#define ADC1_CHSEL_SL_CH_CH_REF4                           ((uint32_t)0x00000010)
#define ADC1_CHSEL_SL_CH_CH_REF5                           ((uint32_t)0x00000020)
#define ADC1_CHSEL_SL_CH_CH_REF6                           ((uint32_t)0x00000040)
#define ADC1_CHSEL_SL_CH_CH_REF7                           ((uint32_t)0x00000080)
#define ADC1_CHSEL_SL_CH_CH_REF8                           ((uint32_t)0x00000100)
#define ADC1_CHSEL_SL_CH_CH_REF9                           ((uint32_t)0x00000200)
#define ADC1_CHSEL_SL_CH_CH_REF10                          ((uint32_t)0x00000400)
#define ADC1_CHSEL_SL_CH_CH_REF11                          ((uint32_t)0x00000800)
#define ADC1_CHSEL_SL_CH_CH_REF12                          ((uint32_t)0x00001000)
#define ADC1_CHSEL_SL_CH_CH_REF13                          ((uint32_t)0x00002000)
#define ADC1_CHSEL_SL_CH_CH_REF14                          ((uint32_t)0x00004000)
#define ADC1_CHSEL_SL_CH_CH_REF15                          ((uint32_t)0x00008000)
#define ADC1_CHSEL_SL_CH_CH_REF16                          ((uint32_t)0x00010000)
#define ADC1_CHSEL_SL_CH_CH_REF17                          ((uint32_t)0x00020000)
#define ADC1_CHSEL_SL_CH_CH_REF18                          ((uint32_t)0x00040000)
#define ADC1_CHSEL_SL_CH_CH_REF19                          ((uint32_t)0x00080000)
#define ADC1_CHSEL_SL_CH_CH_REF20                          ((uint32_t)0x00100000)
#define ADC1_CHSEL_SL_CH_CH_REF21                          ((uint32_t)0x00200000)
#define ADC1_CHSEL_SL_CH_CH_REF22                          ((uint32_t)0x00400000)
#define ADC1_CHSEL_SL_CH_CH_REF23                          ((uint32_t)0x00800000)
#define ADC1_CHSEL_SL_CH_CH_REF24                          ((uint32_t)0x01000000)
#define ADC1_CHSEL_SL_CH_CH_REF25                          ((uint32_t)0x02000000)
#define ADC1_CHSEL_SL_CH_CH_REF26                          ((uint32_t)0x04000000)
#define ADC1_CHSEL_SL_CH_CH_REF27                          ((uint32_t)0x08000000)
#define ADC1_CHSEL_SL_CH_CH_REF28                          ((uint32_t)0x10000000)
#define ADC1_CHSEL_SL_CH_CH_REF29                          ((uint32_t)0x20000000)
#define ADC1_CHSEL_SL_CH_CH_REF30                          ((uint32_t)0x40000000)
#define ADC1_CHSEL_SL_CH_CH_REF31                          ((uint32_t)0x80000000)

/*-- ADC2_CHSEL: ADC2_CHSEL Register -----*/
/* Bit field positions: */
#define ADC2_CHSEL_SL_CH_CH_REF0_Pos                       0
#define ADC2_CHSEL_SL_CH_CH_REF1_Pos                       1
#define ADC2_CHSEL_SL_CH_CH_REF2_Pos                       2
#define ADC2_CHSEL_SL_CH_CH_REF3_Pos                       3
#define ADC2_CHSEL_SL_CH_CH_REF4_Pos                       4
#define ADC2_CHSEL_SL_CH_CH_REF5_Pos                       5
#define ADC2_CHSEL_SL_CH_CH_REF6_Pos                       6
#define ADC2_CHSEL_SL_CH_CH_REF7_Pos                       7
#define ADC2_CHSEL_SL_CH_CH_REF8_Pos                       8
#define ADC2_CHSEL_SL_CH_CH_REF9_Pos                       9
#define ADC2_CHSEL_SL_CH_CH_REF10_Pos                      10
#define ADC2_CHSEL_SL_CH_CH_REF11_Pos                      11
#define ADC2_CHSEL_SL_CH_CH_REF12_Pos                      12
#define ADC2_CHSEL_SL_CH_CH_REF13_Pos                      13
#define ADC2_CHSEL_SL_CH_CH_REF14_Pos                      14
#define ADC2_CHSEL_SL_CH_CH_REF15_Pos                      15
#define ADC2_CHSEL_SL_CH_CH_REF16_Pos                      16
#define ADC2_CHSEL_SL_CH_CH_REF17_Pos                      17
#define ADC2_CHSEL_SL_CH_CH_REF18_Pos                      18
#define ADC2_CHSEL_SL_CH_CH_REF19_Pos                      19
#define ADC2_CHSEL_SL_CH_CH_REF20_Pos                      20
#define ADC2_CHSEL_SL_CH_CH_REF21_Pos                      21
#define ADC2_CHSEL_SL_CH_CH_REF22_Pos                      22
#define ADC2_CHSEL_SL_CH_CH_REF23_Pos                      23
#define ADC2_CHSEL_SL_CH_CH_REF24_Pos                      24
#define ADC2_CHSEL_SL_CH_CH_REF25_Pos                      25
#define ADC2_CHSEL_SL_CH_CH_REF26_Pos                      26
#define ADC2_CHSEL_SL_CH_CH_REF27_Pos                      27
#define ADC2_CHSEL_SL_CH_CH_REF28_Pos                      28
#define ADC2_CHSEL_SL_CH_CH_REF29_Pos                      29
#define ADC2_CHSEL_SL_CH_CH_REF30_Pos                      30
#define ADC2_CHSEL_SL_CH_CH_REF31_Pos                      31
/* Bit field masks: */
#define ADC2_CHSEL_SL_CH_CH_REF0                           ((uint32_t)0x00000001)
#define ADC2_CHSEL_SL_CH_CH_REF1                           ((uint32_t)0x00000002)
#define ADC2_CHSEL_SL_CH_CH_REF2                           ((uint32_t)0x00000004)
#define ADC2_CHSEL_SL_CH_CH_REF3                           ((uint32_t)0x00000008)
#define ADC2_CHSEL_SL_CH_CH_REF4                           ((uint32_t)0x00000010)
#define ADC2_CHSEL_SL_CH_CH_REF5                           ((uint32_t)0x00000020)
#define ADC2_CHSEL_SL_CH_CH_REF6                           ((uint32_t)0x00000040)
#define ADC2_CHSEL_SL_CH_CH_REF7                           ((uint32_t)0x00000080)
#define ADC2_CHSEL_SL_CH_CH_REF8                           ((uint32_t)0x00000100)
#define ADC2_CHSEL_SL_CH_CH_REF9                           ((uint32_t)0x00000200)
#define ADC2_CHSEL_SL_CH_CH_REF10                          ((uint32_t)0x00000400)
#define ADC2_CHSEL_SL_CH_CH_REF11                          ((uint32_t)0x00000800)
#define ADC2_CHSEL_SL_CH_CH_REF12                          ((uint32_t)0x00001000)
#define ADC2_CHSEL_SL_CH_CH_REF13                          ((uint32_t)0x00002000)
#define ADC2_CHSEL_SL_CH_CH_REF14                          ((uint32_t)0x00004000)
#define ADC2_CHSEL_SL_CH_CH_REF15                          ((uint32_t)0x00008000)
#define ADC2_CHSEL_SL_CH_CH_REF16                          ((uint32_t)0x00010000)
#define ADC2_CHSEL_SL_CH_CH_REF17                          ((uint32_t)0x00020000)
#define ADC2_CHSEL_SL_CH_CH_REF18                          ((uint32_t)0x00040000)
#define ADC2_CHSEL_SL_CH_CH_REF19                          ((uint32_t)0x00080000)
#define ADC2_CHSEL_SL_CH_CH_REF20                          ((uint32_t)0x00100000)
#define ADC2_CHSEL_SL_CH_CH_REF21                          ((uint32_t)0x00200000)
#define ADC2_CHSEL_SL_CH_CH_REF22                          ((uint32_t)0x00400000)
#define ADC2_CHSEL_SL_CH_CH_REF23                          ((uint32_t)0x00800000)
#define ADC2_CHSEL_SL_CH_CH_REF24                          ((uint32_t)0x01000000)
#define ADC2_CHSEL_SL_CH_CH_REF25                          ((uint32_t)0x02000000)
#define ADC2_CHSEL_SL_CH_CH_REF26                          ((uint32_t)0x04000000)
#define ADC2_CHSEL_SL_CH_CH_REF27                          ((uint32_t)0x08000000)
#define ADC2_CHSEL_SL_CH_CH_REF28                          ((uint32_t)0x10000000)
#define ADC2_CHSEL_SL_CH_CH_REF29                          ((uint32_t)0x20000000)
#define ADC2_CHSEL_SL_CH_CH_REF30                          ((uint32_t)0x40000000)
#define ADC2_CHSEL_SL_CH_CH_REF31                          ((uint32_t)0x80000000)

/* _dac structure */
typedef struct {
	__IO uint32_t	CFG;                    /*!<DAC Configuration Register */
	__IO uint32_t	DAC1_DATA;              /*!<DAC1 Data Register */
	__IO uint32_t	DAC2_DATA;              /*!<DAC2 Data Register */
} _dac;

/*-- DAC_CFG: DAC_CFG Register -----*/
/* Bit field positions: */
#define DAC_CFG_M_REF0_Pos                                 0
#define DAC_CFG_M_REF1_Pos                                 1
#define DAC_CFG_ON_DAC0_Pos                                2
#define DAC_CFG_ON_DAC1_Pos                                3
#define DAC_CFG_SYNC_A_Pos                                 4
/* Bit field masks: */
#define DAC_CFG_M_REF0                                     ((uint32_t)0x00000001)
#define DAC_CFG_M_REF1                                     ((uint32_t)0x00000002)
#define DAC_CFG_ON_DAC0                                    ((uint32_t)0x00000004)
#define DAC_CFG_ON_DAC1                                    ((uint32_t)0x00000008)
#define DAC_CFG_SYNC_A                                     ((uint32_t)0x00000010)

/*-- DAC1_DATA: DAC1_DATA Register -----*/
/* Bit field positions: */
#define DAC1_DATA_DAC0DATA_Pos                             0
#define DAC1_DATA_DAC1DATA_Pos                             16
/* Bit field masks: */
#define DAC1_DATA_DAC0DATA_Msk                             ((uint32_t)0x00000FFF)
#define DAC1_DATA_DAC1DATA_Msk                             ((uint32_t)0x0FFF0000)

/*-- DAC2_DATA: DAC2_DATA Register -----*/
/* Bit field positions: */
#define DAC2_DATA_DAC1DATA_Pos                             0
#define DAC2_DATA_DAC0DATA_Pos                             16
/* Bit field masks: */
#define DAC2_DATA_DAC1DATA_Msk                             ((uint32_t)0x00000FFF)
#define DAC2_DATA_DAC0DATA_Msk                             ((uint32_t)0x0FFF0000)

/* _comp structure */
typedef struct {
	__IO uint32_t	CFG;                    /*!<COMP Configuration Register */
	__IO uint32_t	RESULT;                 /*!<COMP Result Register */
	__IO uint32_t	RESULT_LATCH;           /*!<COMP Result Latch Register */
} _comp;

/*-- COMP_CFG:COMP CFG Register -----*/
/* Bit field positions: */
#define COMP_CFG_ON_Pos                                    0
#define COMP_CFG_CVRR_Pos                                  1
#define COMP_CFG_CVRSS_Pos                                 2
#define COMP_CFG_CVREN_Pos                                 3
#define COMP_CFG_CVR_Pos                                   4
#define COMP_CFG_CREF_Pos                                  8
#define COMP_CFG_CCH_Pos                                   9
#define COMP_CFG_INV_Pos                                   11
#define COMP_CFG_READY_Pos                                 12
#define COMP_CFG_CMPIE_Pos                                 13
/* Bit field masks: */
#define COMP_CFG_ON                                        ((uint32_t)0x00000001)
#define COMP_CFG_CVRR                                      ((uint32_t)0x00000002)
#define COMP_CFG_CVRSS                                     ((uint32_t)0x00000004)
#define COMP_CFG_CVREN                                     ((uint32_t)0x00000008)
#define COMP_CFG_CVR_Msk                                   ((uint32_t)0x000000F0)
#define COMP_CFG_CREF                                      ((uint32_t)0x00000100)
#define COMP_CFG_CCH_Msk                                   ((uint32_t)0x00000600)
#define COMP_CFG_INV                                       ((uint32_t)0x00000800)
#define COMP_CFG_READY                                     ((uint32_t)0x00001000)
#define COMP_CFG_CMPIE                                     ((uint32_t)0x00002000)

/*-- COMP_RESULT:COMP RESULT Register -----*/
/* Bit field positions: */
#define COMP_RESULT_RSLT_SY_Pos                            0
#define COMP_RESULT_RSLT_AS_Pos                            1
#define COMP_RESULT_RST_LCH_Pos                            2

/* Bit field masks: */
#define COMP_RESULT_RSLT_SY                                ((uint32_t)0x00000001)
#define COMP_RESULT_RSLT_AS                                ((uint32_t)0x00000002)
#define COMP_RESULT_RST_LCH                                ((uint32_t)0x00000004)

/*-- COMP_RESULT_LATCH:COMP RESULT_LATCH Register -----*/
/* Bit field positions: */
#define COMP_RESULT_LATCH_RST_LCH_Pos                      0
/* Bit field masks: */
#define COMP_RESULT_LATCH_RST_LCH                          ((uint32_t)0x00000001)

/* _port structure */
typedef struct {
	__IO uint32_t	RXTX;                   /*!<PORT Data Register */
	__IO uint32_t	OE;                     /*!<PORT Output Enable Register */
	__IO uint32_t	FUNC;                   /*!<PORT Function Register */
	__IO uint32_t	ANALOG;                 /*!<PORT Analog Function Register */
	__IO uint32_t	PULL;                   /*!<PORT Pull Up/Down Register */
	__IO uint32_t	PD;                     /*!<PORT Driver Mode Register */
	__IO uint32_t	PWR;                    /*!<PORT Power Register */
	__IO uint32_t	GFEN;                   /*!<PORT Filter Configuration Register */
	__IO uint32_t   SET;					/*!<PORT Set Selected Bits Register */
	__IO uint32_t   CLR;					/*!<PORT Clear Selected Bits Register */
	__IO uint32_t   RDTX;					/*!< */
} _port;

/*-- PORT_RXTX:PORT RXTX Register -----*/
/* Bit field positions: */
#define PORT_RXTX_RXTX_Pos                                 0
/* Bit field masks: */
#define PORT_RXTX_RXTX_Msk                                 ((uint32_t)0x0000FFFF)

/*-- PORT_OE:PORT OE Register -----*/
/* Bit field positions: */
#define PORT_OE_OE_Pos                                     0
/* Bit field masks: */
#define PORT_OE_OE_Msk                                     ((uint32_t)0x0000FFFF)

/*-- PORT_FUNC:PORT FUNC Register -----*/
/* Bit field positions: */
#define PORT_FUNC_MODE0_Pos                                0
#define PORT_FUNC_MODE1_Pos                                2
#define PORT_FUNC_MODE2_Pos                                4
#define PORT_FUNC_MODE3_Pos                                6
#define PORT_FUNC_MODE4_Pos                                8
#define PORT_FUNC_MODE5_Pos                                10
#define PORT_FUNC_MODE6_Pos                                12
#define PORT_FUNC_MODE7_Pos                                14
#define PORT_FUNC_MODE8_Pos                                16
#define PORT_FUNC_MODE9_Pos                                18
#define PORT_FUNC_MODE10_Pos                               20
#define PORT_FUNC_MODE11_Pos                               22
#define PORT_FUNC_MODE12_Pos                               24
#define PORT_FUNC_MODE13_Pos                               26
#define PORT_FUNC_MODE14_Pos                               28
#define PORT_FUNC_MODE15_Pos                               30
/* Bit field masks: */
#define PORT_FUNC_MODE0_Msk                                ((uint32_t)0x00000003)
#define PORT_FUNC_MODE1_Msk                                ((uint32_t)0x0000000C)
#define PORT_FUNC_MODE2_Msk                                ((uint32_t)0x00000030)
#define PORT_FUNC_MODE3_Msk                                ((uint32_t)0x000000C0)
#define PORT_FUNC_MODE4_Msk                                ((uint32_t)0x00000300)
#define PORT_FUNC_MODE5_Msk                                ((uint32_t)0x00000C00)
#define PORT_FUNC_MODE6_Msk                                ((uint32_t)0x00003000)
#define PORT_FUNC_MODE7_Msk                                ((uint32_t)0x0000C000)
#define PORT_FUNC_MODE8_Msk                                ((uint32_t)0x00030000)
#define PORT_FUNC_MODE9_Msk                                ((uint32_t)0x000C0000)
#define PORT_FUNC_MODE10_Msk                               ((uint32_t)0x00300000)
#define PORT_FUNC_MODE11_Msk                               ((uint32_t)0x00C00000)
#define PORT_FUNC_MODE12_Msk                               ((uint32_t)0x03000000)
#define PORT_FUNC_MODE13_Msk                               ((uint32_t)0x0C000000)
#define PORT_FUNC_MODE14_Msk                               ((uint32_t)0x30000000)
#define PORT_FUNC_MODE15_Msk                               ((uint32_t)0xC0000000)

/*-- PORT_ANALOG:PORT ANALOG Register -----*/
/* Bit field positions: */
#define PORT_ANALOG_ANALOG_EN_Pos                          0
/* Bit field masks: */
#define PORT_ANALOG_ANALOG_EN_Msk                          ((uint32_t)0x0000FFFF)

/*-- PORT_PULL:PORT PULL Register -----*/
/* Bit field positions: */
#define PORT_PULL_PULL_DOWN_Pos                            0
#define PORT_PULL_PULL_UP_Pos                              16
/* Bit field masks: */
#define PORT_PULL_PULL_DOWN_Msk                            ((uint32_t)0x0000FFFF)
#define PORT_PULL_PULL_UP_Msk                              ((uint32_t)0xFFFF0000)

/*-- PORT_PD:PORT PD Register -----*/
/* Bit field positions: */
#define PORT_PD_PD_Pos                                     0
#define PORT_PD_SHM_Pos                                    16
/* Bit field masks: */
#define PORT_PD_PD_Msk                                     ((uint32_t)0x0000FFFF)
#define PORT_PD_SHM_Msk                                    ((uint32_t)0xFFFF0000)

/*-- PORT_PWR:PORT PWR Register -----*/
/* Bit field positions: */
#define PORT_PWR_PWR0_Pos                                  0
#define PORT_PWR_PWR1_Pos                                  2
#define PORT_PWR_PWR2_Pos                                  4
#define PORT_PWR_PWR3_Pos                                  6
#define PORT_PWR_PWR4_Pos                                  8
#define PORT_PWR_PWR5_Pos                                  10
#define PORT_PWR_PWR6_Pos                                  12
#define PORT_PWR_PWR7_Pos                                  14
#define PORT_PWR_PWR8_Pos                                  16
#define PORT_PWR_PWR9_Pos                                  18
#define PORT_PWR_PWR10_Pos                                 20
#define PORT_PWR_PWR11_Pos                                 22
#define PORT_PWR_PWR12_Pos                                 24
#define PORT_PWR_PWR13_Pos                                 26
#define PORT_PWR_PWR14_Pos                                 28
#define PORT_PWR_PWR15_Pos                                 30
/* Bit field masks: */
#define PORT_PWR_PWR0_Msk                                  ((uint32_t)0x00000003)
#define PORT_PWR_PWR1_Msk                                  ((uint32_t)0x0000000C)
#define PORT_PWR_PWR2_Msk                                  ((uint32_t)0x00000030)
#define PORT_PWR_PWR3_Msk                                  ((uint32_t)0x000000C0)
#define PORT_PWR_PWR4_Msk                                  ((uint32_t)0x00000300)
#define PORT_PWR_PWR5_Msk                                  ((uint32_t)0x00000C00)
#define PORT_PWR_PWR6_Msk                                  ((uint32_t)0x00003000)
#define PORT_PWR_PWR7_Msk                                  ((uint32_t)0x0000C000)
#define PORT_PWR_PWR8_Msk                                  ((uint32_t)0x00030000)
#define PORT_PWR_PWR9_Msk                                  ((uint32_t)0x000C0000)
#define PORT_PWR_PWR10_Msk                                 ((uint32_t)0x00300000)
#define PORT_PWR_PWR11_Msk                                 ((uint32_t)0x00C00000)
#define PORT_PWR_PWR12_Msk                                 ((uint32_t)0x03000000)
#define PORT_PWR_PWR13_Msk                                 ((uint32_t)0x0C000000)
#define PORT_PWR_PWR14_Msk                                 ((uint32_t)0x30000000)
#define PORT_PWR_PWR15_Msk                                 ((uint32_t)0xC0000000)

/*-- PORT_GFEN:PORT GFEN Register -----*/
/* Bit field positions: */
#define PORT_GFEN_GFEN_Pos                                 0
/* Bit field masks: */
#define PORT_GFEN_GFEN_Msk                                 ((uint32_t)0x0000FFFF)

/* _bkp structure */
typedef struct {
	__IO uint32_t	REG_00;                 /*!<Backup Register 0 */
	__IO uint32_t	REG_01;                 /*!<Backup Register 1 */
	__IO uint32_t	REG_02;                 /*!<Backup Register 2 */
	__IO uint32_t	REG_03;                 /*!<Backup Register 3 */
	__IO uint32_t	REG_04;                 /*!<Backup Register 4 */
	__IO uint32_t	REG_05;                 /*!<Backup Register 5 */
	__IO uint32_t	REG_06;                 /*!<Backup Register 6 */
	__IO uint32_t	REG_07;                 /*!<Backup Register 7 */
	__IO uint32_t	REG_08;                 /*!<Backup Register 8 */
	__IO uint32_t	REG_09;                 /*!<Backup Register 9 */
	__IO uint32_t	REG_0A;                 /*!<Backup Register 10 */
	__IO uint32_t	REG_0B;                 /*!<Backup Register 11 */
	__IO uint32_t	REG_0C;                 /*!<Backup Register 12 */
	__IO uint32_t	REG_0D;                 /*!<Backup Register 13 */
	__IO uint32_t	REG_0E;                 /*!<Backup Register 14 */
	__IO uint32_t	REG_0F;                 /*!<Backup Register 15 */
	__IO uint32_t	RTC_CNT;                /*!<Realtime Clock Counter Register */
	__IO uint32_t	RTC_DIV;                /*!<Realtime Prescaler Counter Register */
	__IO uint32_t	RTC_PRL;                /*!<Realtime Prescaler Auto-Reload Value Register */
	__IO uint32_t	RTC_ALRM;               /*!<Realtime Alarm Register */
	__IO uint32_t	RTC_CS;                 /*!<Backup Realtime clock Register */
} _bkp;

/*-- BKP_REG_00:BKP REG_00 Register -----*/
/* Bit field positions: */
#define BKP_REG_00_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_00_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_01:BKP REG_01 Register -----*/
/* Bit field positions: */
#define BKP_REG_01_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_01_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_02:BKP REG_02 Register -----*/
/* Bit field positions: */
#define BKP_REG_02_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_02_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_03:BKP REG_03 Register -----*/
/* Bit field positions: */
#define BKP_REG_03_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_03_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_04:BKP REG_04 Register -----*/
/* Bit field positions: */
#define BKP_REG_04_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_04_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_05:BKP REG_05 Register -----*/
/* Bit field positions: */
#define BKP_REG_05_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_05_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_06:BKP REG_06 Register -----*/
/* Bit field positions: */
#define BKP_REG_06_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_06_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_07:BKP REG_07 Register -----*/
/* Bit field positions: */
#define BKP_REG_07_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_07_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_08:BKP REG_08 Register -----*/
/* Bit field positions: */
#define BKP_REG_08_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_08_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_09:BKP REG_09 Register -----*/
/* Bit field positions: */
#define BKP_REG_09_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_09_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_0A:BKP REG_0A Register -----*/
/* Bit field positions: */
#define BKP_REG_0A_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_0A_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_0B:BKP REG_0B Register -----*/
/* Bit field positions: */
#define BKP_REG_0B_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_0B_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_0C:BKP REG_0C Register -----*/
/* Bit field positions: */
#define BKP_REG_0C_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_0C_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_0D:BKP REG_0D Register -----*/
/* Bit field positions: */
#define BKP_REG_0D_BKP_REG_Pos                             0
/* Bit field masks: */
#define BKP_REG_0D_BKP_REG_Msk                             ((uint32_t)0xFFFFFFFF)

/*-- BKP_REG_0E:BKP REG_0E Register -----*/
/* Bit field positions: */
#define BKP_REG_0E_LOW_Pos                                 0
#define BKP_REG_0E_SELECTRI_Pos                            3
#define BKP_REG_0E_JTAGA_Pos                               6
#define BKP_REG_0E_JTAGB_Pos                               7
#define BKP_REG_0E_TRIM_Pos                                8
#define BKP_REG_0E_FPOR_Pos                                11
#define BKP_REG_0E_MODE_Pos                                12
/* Bit field masks: */
#define BKP_REG_0E_LOW_Msk                                 ((uint32_t)0x00000007)
#define BKP_REG_0E_SELECTRI_Msk                            ((uint32_t)0x00000038)
#define BKP_REG_0E_JTAGA                                   ((uint32_t)0x00000040)
#define BKP_REG_0E_JTAGB                                   ((uint32_t)0x00000080)
#define BKP_REG_0E_TRIM_Msk                                ((uint32_t)0x00000700)
#define BKP_REG_0E_FPOR                                    ((uint32_t)0x00000800)
#define BKP_REG_0E_MODE_Msk                                ((uint32_t)0x00007000)

/*-- BKP_REG_0F:BKP REG_0F Register -----*/
/* Bit field positions: */
#define BKP_REG_0F_LSE_ON_Pos                              0
#define BKP_REG_0F_LSE_BYP_Pos                             1
#define BKP_REG_0F_RTC_SEL_Pos                             2
#define BKP_REG_0F_RTC_EN_Pos                              4
#define BKP_REG_0F_CAL_Pos                                 5
#define BKP_REG_0F_LSE_RDY_Pos                             13
#define BKP_REG_0F_LSI_ON_Pos                              15
#define BKP_REG_0F_LSI_TRIM_Pos                            16
#define BKP_REG_0F_LSI_RDY_Pos                             21
#define BKP_REG_0F_HSI_ON_Pos                              22
#define BKP_REG_0F_HSI_RDY_Pos                             23
#define BKP_REG_0F_HSI_TRIM_Pos                            24
#define BKP_REG_0F_STANDBY_Pos                             30
#define BKP_REG_0F_RTC_RESET_Pos                           31
/* Bit field masks: */
#define BKP_REG_0F_LSE_ON                                  ((uint32_t)0x00000001)
#define BKP_REG_0F_LSE_BYP                                 ((uint32_t)0x00000002)
#define BKP_REG_0F_RTC_SEL_Msk                             ((uint32_t)0x0000000C)
#define BKP_REG_0F_RTC_EN                                  ((uint32_t)0x00000010)
#define BKP_REG_0F_CAL_Msk                                 ((uint32_t)0x00001FE0)
#define BKP_REG_0F_LSE_RDY                                 ((uint32_t)0x00002000)
#define BKP_REG_0F_LSI_ON                                  ((uint32_t)0x00008000)
#define BKP_REG_0F_LSI_TRIM_Msk                            ((uint32_t)0x001F0000)
#define BKP_REG_0F_LSI_RDY                                 ((uint32_t)0x00200000)
#define BKP_REG_0F_HSI_ON                                  ((uint32_t)0x00400000)
#define BKP_REG_0F_HSI_RDY                                 ((uint32_t)0x00800000)
#define BKP_REG_0F_HSI_TRIM_Msk                            ((uint32_t)0x3F000000)
#define BKP_REG_0F_STANDBY                                 ((uint32_t)0x40000000)
#define BKP_REG_0F_RTC_RESET                               ((uint32_t)0x80000000)

/*-- BKP_RTC_CNT:BKP RTC_CNT Register -----*/
/* Bit field positions: */
#define BKP_RTC_CNT_RTC_CNT_Pos                            0
/* Bit field masks: */
#define BKP_RTC_CNT_RTC_CNT_Msk                            ((uint32_t)0xFFFFFFFF)

/*-- BKP_RTC_DIV:BKP RTC_DIV Register -----*/
/* Bit field positions: */
#define BKP_RTC_DIV_RTC_DIV_Pos                            0
/* Bit field masks: */
#define BKP_RTC_DIV_RTC_DIV_Msk                            ((uint32_t)0x000FFFFF)

/*-- BKP_RTC_PRL:BKP RTC_PRL Register -----*/
/* Bit field positions: */
#define BKP_RTC_PRL_RTC_PRL_Pos                            0
/* Bit field masks: */
#define BKP_RTC_PRL_RTC_PRL_Msk                            ((uint32_t)0x000FFFFF)

/*-- BKP_RTC_ALRM:BKP RTC_ALRM Register -----*/
/* Bit field positions: */
#define BKP_RTC_ALRM_RTC_ALRM_Pos                          0
/* Bit field masks: */
#define BKP_RTC_ALRM_RTC_ALRM_Msk                          ((uint32_t)0xFFFFFFFF)

/*-- BKP_RTC_CS:BKP RTC_CS Register -----*/
/* Bit field positions: */
#define BKP_RTC_CS_OWF_Pos                                 0
#define BKP_RTC_CS_SECF_Pos                                1
#define BKP_RTC_CS_ALRF_Pos                                2
#define BKP_RTC_CS_OWF_IE_Pos                              3
#define BKP_RTC_CS_SECF_IE_Pos                             4
#define BKP_RTC_CS_ALRF_IE_Pos                             5
#define BKP_RTC_CS_WEC_Pos                                 6
/* Bit field masks: */
#define BKP_RTC_CS_OWF                                     ((uint32_t)0x00000001)
#define BKP_RTC_CS_SECF                                    ((uint32_t)0x00000002)
#define BKP_RTC_CS_ALRF                                    ((uint32_t)0x00000004)
#define BKP_RTC_CS_OWF_IE                                  ((uint32_t)0x00000008)
#define BKP_RTC_CS_SECF_IE                                 ((uint32_t)0x00000010)
#define BKP_RTC_CS_ALRF_IE                                 ((uint32_t)0x00000020)
#define BKP_RTC_CS_WEC                                     ((uint32_t)0x00000040)

/* _extbus structure */
typedef struct
{
		__io_reg RESERVED0[20];
		__io_reg NAND_CYCLES;

		//WAIT_STATE[15..12], BUSY[7], CPOL[3], NAND[2], RAM[1], ROM[0]
		__io_reg CONTROL;		
} _extbus;

/*-- EBC_NAND_CYCLES:EBC NAND_CYCLES Register -----*/
/* Bit field positions: */
#define EBC_NAND_CYCLES_TRC_Pos                            0
#define EBC_NAND_CYCLES_TWC_Pos                            4
#define EBC_NAND_CYCLES_TREA_Pos                           8
#define EBC_NAND_CYCLES_TWP_Pos                            12
#define EBC_NAND_CYCLES_TWHR_Pos                           16
#define EBC_NAND_CYCLES_TALEA_Pos                          20
#define EBC_NAND_CYCLES_TRR_Pos                            24
/* Bit field masks: */
#define EBC_NAND_CYCLES_TRC_Msk                            ((uint32_t)0x0000000F)
#define EBC_NAND_CYCLES_TWC_Msk                            ((uint32_t)0x000000F0)
#define EBC_NAND_CYCLES_TREA_Msk                           ((uint32_t)0x00000F00)
#define EBC_NAND_CYCLES_TWP_Msk                            ((uint32_t)0x0000F000)
#define EBC_NAND_CYCLES_TWHR_Msk                           ((uint32_t)0x000F0000)
#define EBC_NAND_CYCLES_TALEA_Msk                          ((uint32_t)0x00F00000)
#define EBC_NAND_CYCLES_TRR_Msk                            ((uint32_t)0x0F000000)

/*-- EBC_CONTROL:EBC CONTROL Register -----*/
/* Bit field positions: */
#define EBC_CONTROL_ROM_Pos                                0
#define EBC_CONTROL_RAM_Pos                                1
#define EBC_CONTROL_NAND_Pos                               2
#define EBC_CONTROL_CPOL_Pos                               3
#define EBC_CONTROL_BUSY_Pos                               7
#define EBC_CONTROL_WAIT_STATE_Pos                         12
/* Bit field masks: */
#define EBC_CONTROL_ROM                                    ((uint32_t)0x00000001)
#define EBC_CONTROL_RAM                                    ((uint32_t)0x00000002)
#define EBC_CONTROL_NAND                                   ((uint32_t)0x00000004)
#define EBC_CONTROL_CPOL                                   ((uint32_t)0x00000008)
#define EBC_CONTROL_BUSY                                   ((uint32_t)0x00000080)
#define EBC_CONTROL_WAIT_STATE_Msk                         ((uint32_t)0x0000F000)

/*===========================================================================
 * DSP core definitions: BSP, TIMER, DIR, AIR, CRYPTO, ACODEC, CLKMD, DMA
 *===========================================================================*/

/* _dsp_bsp structure */
typedef struct {
	__IO uint32_t	DRR;                    /*!< */
	__IO uint32_t	DXR;                    /*!< */
	__IO uint32_t	SPSA;                   /*!< */
	__IO uint32_t	SPCR;                   /*!< */
	__IO uint32_t	RCR;                    /*!< */
	__IO uint32_t	XCR;                    /*!< */
	__IO uint32_t	SRGR;                   /*!< */
	__IO uint32_t	MCR;                    /*!< */
	__IO uint32_t	XCER;                   /*!< */
	__IO uint32_t	RCER;                   /*!< */
	__IO uint32_t	PCR_SPSR;               /*!< */
} _dsp_bsp;

/*-- DSP_BSP_DRR:DSP_BSP DRR Register -----*/
/* Bit field positions: */
#define DSP_BSP_DRR_DATA_RX_Pos                              0
/* Bit field masks: */
#define DSP_BSP_DRR_DATA_RX_Msk                              ((uint32_t)0xFFFFFFFF)

/*-- DSP_BSP_DXR:DSP_BSP DXR Register -----*/
/* Bit field positions: */
#define DSP_BSP_DXR_DATA_TX_Pos                              0
/* Bit field masks: */
#define DSP_BSP_DXR_DATA_TX_Msk                              ((uint32_t)0xFFFFFFFF)

/*-- DSP_BSP_SPSA:DSP_BSP SPSA Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_BSP_SPCR:DSP_BSP SPCR Register -----*/
/* Bit field positions: */
#define DSP_BSP_SPCR_NCGRST_Pos                              0
#define DSP_BSP_SPCR_NFGRST_Pos                              1
#define DSP_BSP_SPCR_NRRST_Pos                               2
#define DSP_BSP_SPCR_NXRST_Pos                               3
#define DSP_BSP_SPCR_CLKSTP_Pos                              4
#define DSP_BSP_SPCR_ABIS_Pos                                6
#define DSP_BSP_SPCR_DLB_Pos                                 7
#define DSP_BSP_SPCR_RJUST_Pos                               8
#define DSP_BSP_SPCR_XJUST_Pos                               10
#define DSP_BSP_SPCR_JBOUND_Pos                              12
#define DSP_BSP_SPCR_LW_ACC_Pos                              14
#define DSP_BSP_SPCR_DXENA_Pos                               15
#define DSP_BSP_SPCR_REVNT0_Pos                              16
#define DSP_BSP_SPCR_REVNT1_Pos                              18
#define DSP_BSP_SPCR_RINTM_Pos                               20
#define DSP_BSP_SPCR_SOFT_Pos                                23
#define DSP_BSP_SPCR_XEVENT0_Pos                             24
#define DSP_BSP_SPCR_XEVNT1_Pos                              26
#define DSP_BSP_SPCR_XEVENT1_Pos                             27
#define DSP_BSP_SPCR_XINTM_Pos                               28
#define DSP_BSP_SPCR_FREE_Pos                                31
/* Bit field masks: */
#define DSP_BSP_SPCR_NCGRST                                  ((uint32_t)0x00000001)
#define DSP_BSP_SPCR_NFGRST                                  ((uint32_t)0x00000002)
#define DSP_BSP_SPCR_NRRST                                   ((uint32_t)0x00000004)
#define DSP_BSP_SPCR_NXRST                                   ((uint32_t)0x00000008)
#define DSP_BSP_SPCR_CLKSTP_Msk                              ((uint32_t)0x00000030)
#define DSP_BSP_SPCR_ABIS                                    ((uint32_t)0x00000040)
#define DSP_BSP_SPCR_DLB                                     ((uint32_t)0x00000080)
#define DSP_BSP_SPCR_RJUST_Msk                               ((uint32_t)0x00000300)
#define DSP_BSP_SPCR_XJUST_Msk                               ((uint32_t)0x00000C00)
#define DSP_BSP_SPCR_JBOUND_Msk                              ((uint32_t)0x00003000)
#define DSP_BSP_SPCR_LW_ACC                                  ((uint32_t)0x00004000)
#define DSP_BSP_SPCR_DXENA                                   ((uint32_t)0x00008000)
#define DSP_BSP_SPCR_REVNT0_Msk                              ((uint32_t)0x00030000)
#define DSP_BSP_SPCR_REVNT1_Msk                              ((uint32_t)0x000C0000)
#define DSP_BSP_SPCR_RINTM_Msk                               ((uint32_t)0x00700000)
#define DSP_BSP_SPCR_SOFT                                    ((uint32_t)0x00800000)
#define DSP_BSP_SPCR_XEVENT0_Msk                             ((uint32_t)0x03000000)
#define DSP_BSP_SPCR_XEVNT1                                  ((uint32_t)0x04000000)
#define DSP_BSP_SPCR_XEVENT1                                 ((uint32_t)0x08000000)
#define DSP_BSP_SPCR_XINTM_Msk                               ((uint32_t)0x70000000)
#define DSP_BSP_SPCR_FREE                                    ((uint32_t)0x80000000)

/*-- DSP_BSP_RCR:DSP_BSP RCR Register -----*/
/* Bit field positions: */
#define DSP_BSP_RCR_WLEN_P0_Pos                              0
#define DSP_BSP_RCR_DELAY_Pos                                5
#define DSP_BSP_RCR_MSB1ST_Pos                               7
#define DSP_BSP_RCR_FLEN_P0_Pos                              8
#define DSP_BSP_RCR_FIG_Pos                                  15
#define DSP_BSP_RCR_WLEN_P1_Pos                              16
#define DSP_BSP_RCR_CODEC_Pos                                21
#define DSP_BSP_RCR_FLEN_P1_Pos                              24
#define DSP_BSP_RCR_MPHASE_Pos                               31
/* Bit field masks: */
#define DSP_BSP_RCR_WLEN_P0_Msk                              ((uint32_t)0x0000001F)
#define DSP_BSP_RCR_DELAY_Msk                                ((uint32_t)0x00000060)
#define DSP_BSP_RCR_MSB1ST                                   ((uint32_t)0x00000080)
#define DSP_BSP_RCR_FLEN_P0_Msk                              ((uint32_t)0x00007F00)
#define DSP_BSP_RCR_FIG                                      ((uint32_t)0x00008000)
#define DSP_BSP_RCR_WLEN_P1_Msk                              ((uint32_t)0x001F0000)
#define DSP_BSP_RCR_CODEC_Msk                                ((uint32_t)0x00E00000)
#define DSP_BSP_RCR_FLEN_P1_Msk                              ((uint32_t)0x7F000000)
#define DSP_BSP_RCR_MPHASE                                   ((uint32_t)0x80000000)

/*-- DSP_BSP_XCR:DSP_BSP XCR Register -----*/
/* Bit field positions: */
#define DSP_BSP_XCR_WLEN_P0_Pos                              0
#define DSP_BSP_XCR_DELAY_Pos                                5
#define DSP_BSP_XCR_MSB1ST_Pos                               7
#define DSP_BSP_XCR_FLEN_P0_Pos                              8
#define DSP_BSP_XCR_FIG_Pos                                  15
#define DSP_BSP_XCR_WLEN_P1_Pos                              16
#define DSP_BSP_XCR_CODEC_Pos                                21
#define DSP_BSP_XCR_FLEN_P1_Pos                              24
#define DSP_BSP_XCR_MPHASE_Pos                               31
/* Bit field masks: */
#define DSP_BSP_XCR_WLEN_P0_Msk                              ((uint32_t)0x0000001F)
#define DSP_BSP_XCR_DELAY_Msk                                ((uint32_t)0x00000060)
#define DSP_BSP_XCR_MSB1ST                                   ((uint32_t)0x00000080)
#define DSP_BSP_XCR_FLEN_P0_Msk                              ((uint32_t)0x00007F00)
#define DSP_BSP_XCR_FIG                                      ((uint32_t)0x00008000)
#define DSP_BSP_XCR_WLEN_P1_Msk                              ((uint32_t)0x001F0000)
#define DSP_BSP_XCR_CODEC_Msk                                ((uint32_t)0x00E00000)
#define DSP_BSP_XCR_FLEN_P1_Msk                              ((uint32_t)0x7F000000)
#define DSP_BSP_XCR_MPHASE                                   ((uint32_t)0x80000000)

/*-- DSP_BSP_SRGR:DSP_BSP SRGR Register -----*/
/* Bit field positions: */
#define DSP_BSP_SRGR_CLKGDV_Pos                              0
#define DSP_BSP_SRGR_FWID_Pos                                8
#define DSP_BSP_SRGR_FPER_Pos                                16
#define DSP_BSP_SRGR_CLKSP_Pos                               28
#define DSP_BSP_SRGR_CLKSM_Pos                               29
#define DSP_BSP_SRGR_FSGM_Pos                                30
#define DSP_BSP_SRGR_GSYNC_Pos                               31
/* Bit field masks: */
#define DSP_BSP_SRGR_CLKGDV_Msk                              ((uint32_t)0x000000FF)
#define DSP_BSP_SRGR_FWID_Msk                                ((uint32_t)0x0000FF00)
#define DSP_BSP_SRGR_FPER_Msk                                ((uint32_t)0x0FFF0000)
#define DSP_BSP_SRGR_CLKSP                                   ((uint32_t)0x10000000)
#define DSP_BSP_SRGR_CLKSM                                   ((uint32_t)0x20000000)
#define DSP_BSP_SRGR_FSGM                                    ((uint32_t)0x40000000)
#define DSP_BSP_SRGR_GSYNC                                   ((uint32_t)0x80000000)

/*-- DSP_BSP_MCR:DSP_BSP MCR Register -----*/
/* Bit field positions: */
#define DSP_BSP_MCR_MCM_Pos                                  0
#define DSP_BSP_MCR_CUR_BLK_Pos                              5
#define DSP_BSP_MCR_BLK0_EN_Pos                              8
#define DSP_BSP_MCR_BLK1_EN_Pos                              9
#define DSP_BSP_MCR_BLK2_EN_Pos                              10
#define DSP_BSP_MCR_BLK3_EN_Pos                              11
#define DSP_BSP_MCR_BLK4_EN_Pos                              12
#define DSP_BSP_MCR_BLK5_EN_Pos                              13
#define DSP_BSP_MCR_BLK6_EN_Pos                              14
#define DSP_BSP_MCR_BLK7_EN_Pos                              15
/* Bit field masks: */
#define DSP_BSP_MCR_MCM_Msk                                  ((uint32_t)0x00000003)
#define DSP_BSP_MCR_CUR_BLK_Msk                              ((uint32_t)0x000000E0)
#define DSP_BSP_MCR_BLK0_EN                                  ((uint32_t)0x00000100)
#define DSP_BSP_MCR_BLK1_EN                                  ((uint32_t)0x00000200)
#define DSP_BSP_MCR_BLK2_EN                                  ((uint32_t)0x00000400)
#define DSP_BSP_MCR_BLK3_EN                                  ((uint32_t)0x00000800)
#define DSP_BSP_MCR_BLK4_EN                                  ((uint32_t)0x00001000)
#define DSP_BSP_MCR_BLK5_EN                                  ((uint32_t)0x00002000)
#define DSP_BSP_MCR_BLK6_EN                                  ((uint32_t)0x00004000)
#define DSP_BSP_MCR_BLK7_EN                                  ((uint32_t)0x00008000)

/*-- DSP_BSP_XCER:DSP_BSP XCER Register -----*/
/* Bit field positions: */
#define DSP_BSP_XCER_CH0_EN_Pos                              0
#define DSP_BSP_XCER_CH1_EN_Pos                              1
#define DSP_BSP_XCER_CH2_EN_Pos                              2
#define DSP_BSP_XCER_CH3_EN_Pos                              3
#define DSP_BSP_XCER_CH4_EN_Pos                              4
#define DSP_BSP_XCER_CH5_EN_Pos                              5
#define DSP_BSP_XCER_CH6_EN_Pos                              6
#define DSP_BSP_XCER_CH7_EN_Pos                              7
#define DSP_BSP_XCER_CH8_EN_Pos                              8
#define DSP_BSP_XCER_CH9_EN_Pos                              9
#define DSP_BSP_XCER_CH10_EN_Pos                             10
#define DSP_BSP_XCER_CH11_EN_Pos                             11
#define DSP_BSP_XCER_CH12_EN_Pos                             12
#define DSP_BSP_XCER_CH13_EN_Pos                             13
#define DSP_BSP_XCER_CH14_EN_Pos                             14
#define DSP_BSP_XCER_CH15_EN_Pos                             15
#define DSP_BSP_XCER_CH16_EN_Pos                             16
#define DSP_BSP_XCER_CH17_EN_Pos                             17
#define DSP_BSP_XCER_CH18_EN_Pos                             18
#define DSP_BSP_XCER_CH19_EN_Pos                             19
#define DSP_BSP_XCER_CH20_EN_Pos                             20
#define DSP_BSP_XCER_CH21_EN_Pos                             21
#define DSP_BSP_XCER_CH22_EN_Pos                             22
#define DSP_BSP_XCER_CH23_EN_Pos                             23
#define DSP_BSP_XCER_CH24_EN_Pos                             24
#define DSP_BSP_XCER_CH25_EN_Pos                             25
#define DSP_BSP_XCER_CH26_EN_Pos                             26
#define DSP_BSP_XCER_CH27_EN_Pos                             27
#define DSP_BSP_XCER_CH28_EN_Pos                             28
#define DSP_BSP_XCER_CH29_EN_Pos                             29
#define DSP_BSP_XCER_CH30_EN_Pos                             30
#define DSP_BSP_XCER_CH31_EN_Pos                             31
/* Bit field masks: */
#define DSP_BSP_XCER_CH0_EN                                  ((uint32_t)0x00000001)
#define DSP_BSP_XCER_CH1_EN                                  ((uint32_t)0x00000002)
#define DSP_BSP_XCER_CH2_EN                                  ((uint32_t)0x00000004)
#define DSP_BSP_XCER_CH3_EN                                  ((uint32_t)0x00000008)
#define DSP_BSP_XCER_CH4_EN                                  ((uint32_t)0x00000010)
#define DSP_BSP_XCER_CH5_EN                                  ((uint32_t)0x00000020)
#define DSP_BSP_XCER_CH6_EN                                  ((uint32_t)0x00000040)
#define DSP_BSP_XCER_CH7_EN                                  ((uint32_t)0x00000080)
#define DSP_BSP_XCER_CH8_EN                                  ((uint32_t)0x00000100)
#define DSP_BSP_XCER_CH9_EN                                  ((uint32_t)0x00000200)
#define DSP_BSP_XCER_CH10_EN                                 ((uint32_t)0x00000400)
#define DSP_BSP_XCER_CH11_EN                                 ((uint32_t)0x00000800)
#define DSP_BSP_XCER_CH12_EN                                 ((uint32_t)0x00001000)
#define DSP_BSP_XCER_CH13_EN                                 ((uint32_t)0x00002000)
#define DSP_BSP_XCER_CH14_EN                                 ((uint32_t)0x00004000)
#define DSP_BSP_XCER_CH15_EN                                 ((uint32_t)0x00008000)
#define DSP_BSP_XCER_CH16_EN                                 ((uint32_t)0x00010000)
#define DSP_BSP_XCER_CH17_EN                                 ((uint32_t)0x00020000)
#define DSP_BSP_XCER_CH18_EN                                 ((uint32_t)0x00040000)
#define DSP_BSP_XCER_CH19_EN                                 ((uint32_t)0x00080000)
#define DSP_BSP_XCER_CH20_EN                                 ((uint32_t)0x00100000)
#define DSP_BSP_XCER_CH21_EN                                 ((uint32_t)0x00200000)
#define DSP_BSP_XCER_CH22_EN                                 ((uint32_t)0x00400000)
#define DSP_BSP_XCER_CH23_EN                                 ((uint32_t)0x00800000)
#define DSP_BSP_XCER_CH24_EN                                 ((uint32_t)0x01000000)
#define DSP_BSP_XCER_CH25_EN                                 ((uint32_t)0x02000000)
#define DSP_BSP_XCER_CH26_EN                                 ((uint32_t)0x04000000)
#define DSP_BSP_XCER_CH27_EN                                 ((uint32_t)0x08000000)
#define DSP_BSP_XCER_CH28_EN                                 ((uint32_t)0x10000000)
#define DSP_BSP_XCER_CH29_EN                                 ((uint32_t)0x20000000)
#define DSP_BSP_XCER_CH30_EN                                 ((uint32_t)0x40000000)
#define DSP_BSP_XCER_CH31_EN                                 ((uint32_t)0x80000000)

/*-- DSP_BSP_RCER:DSP_BSP RCER Register -----*/
/* Bit field positions: */
#define DSP_BSP_RCER_CH0_EN_Pos                              0
#define DSP_BSP_RCER_CH1_EN_Pos                              1
#define DSP_BSP_RCER_CH2_EN_Pos                              2
#define DSP_BSP_RCER_CH3_EN_Pos                              3
#define DSP_BSP_RCER_CH4_EN_Pos                              4
#define DSP_BSP_RCER_CH5_EN_Pos                              5
#define DSP_BSP_RCER_CH6_EN_Pos                              6
#define DSP_BSP_RCER_CH7_EN_Pos                              7
#define DSP_BSP_RCER_CH8_EN_Pos                              8
#define DSP_BSP_RCER_CH9_EN_Pos                              9
#define DSP_BSP_RCER_CH10_EN_Pos                             10
#define DSP_BSP_RCER_CH11_EN_Pos                             11
#define DSP_BSP_RCER_CH12_EN_Pos                             12
#define DSP_BSP_RCER_CH13_EN_Pos                             13
#define DSP_BSP_RCER_CH14_EN_Pos                             14
#define DSP_BSP_RCER_CH15_EN_Pos                             15
#define DSP_BSP_RCER_CH16_EN_Pos                             16
#define DSP_BSP_RCER_CH17_EN_Pos                             17
#define DSP_BSP_RCER_CH18_EN_Pos                             18
#define DSP_BSP_RCER_CH19_EN_Pos                             19
#define DSP_BSP_RCER_CH20_EN_Pos                             20
#define DSP_BSP_RCER_CH21_EN_Pos                             21
#define DSP_BSP_RCER_CH22_EN_Pos                             22
#define DSP_BSP_RCER_CH23_EN_Pos                             23
#define DSP_BSP_RCER_CH24_EN_Pos                             24
#define DSP_BSP_RCER_CH25_EN_Pos                             25
#define DSP_BSP_RCER_CH26_EN_Pos                             26
#define DSP_BSP_RCER_CH27_EN_Pos                             27
#define DSP_BSP_RCER_CH28_EN_Pos                             28
#define DSP_BSP_RCER_CH29_EN_Pos                             29
#define DSP_BSP_RCER_CH30_EN_Pos                             30
#define DSP_BSP_RCER_CH31_EN_Pos                             31
/* Bit field masks: */
#define DSP_BSP_RCER_CH0_EN                                  ((uint32_t)0x00000001)
#define DSP_BSP_RCER_CH1_EN                                  ((uint32_t)0x00000002)
#define DSP_BSP_RCER_CH2_EN                                  ((uint32_t)0x00000004)
#define DSP_BSP_RCER_CH3_EN                                  ((uint32_t)0x00000008)
#define DSP_BSP_RCER_CH4_EN                                  ((uint32_t)0x00000010)
#define DSP_BSP_RCER_CH5_EN                                  ((uint32_t)0x00000020)
#define DSP_BSP_RCER_CH6_EN                                  ((uint32_t)0x00000040)
#define DSP_BSP_RCER_CH7_EN                                  ((uint32_t)0x00000080)
#define DSP_BSP_RCER_CH8_EN                                  ((uint32_t)0x00000100)
#define DSP_BSP_RCER_CH9_EN                                  ((uint32_t)0x00000200)
#define DSP_BSP_RCER_CH10_EN                                 ((uint32_t)0x00000400)
#define DSP_BSP_RCER_CH11_EN                                 ((uint32_t)0x00000800)
#define DSP_BSP_RCER_CH12_EN                                 ((uint32_t)0x00001000)
#define DSP_BSP_RCER_CH13_EN                                 ((uint32_t)0x00002000)
#define DSP_BSP_RCER_CH14_EN                                 ((uint32_t)0x00004000)
#define DSP_BSP_RCER_CH15_EN                                 ((uint32_t)0x00008000)
#define DSP_BSP_RCER_CH16_EN                                 ((uint32_t)0x00010000)
#define DSP_BSP_RCER_CH17_EN                                 ((uint32_t)0x00020000)
#define DSP_BSP_RCER_CH18_EN                                 ((uint32_t)0x00040000)
#define DSP_BSP_RCER_CH19_EN                                 ((uint32_t)0x00080000)
#define DSP_BSP_RCER_CH20_EN                                 ((uint32_t)0x00100000)
#define DSP_BSP_RCER_CH21_EN                                 ((uint32_t)0x00200000)
#define DSP_BSP_RCER_CH22_EN                                 ((uint32_t)0x00400000)
#define DSP_BSP_RCER_CH23_EN                                 ((uint32_t)0x00800000)
#define DSP_BSP_RCER_CH24_EN                                 ((uint32_t)0x01000000)
#define DSP_BSP_RCER_CH25_EN                                 ((uint32_t)0x02000000)
#define DSP_BSP_RCER_CH26_EN                                 ((uint32_t)0x04000000)
#define DSP_BSP_RCER_CH27_EN                                 ((uint32_t)0x08000000)
#define DSP_BSP_RCER_CH28_EN                                 ((uint32_t)0x10000000)
#define DSP_BSP_RCER_CH29_EN                                 ((uint32_t)0x20000000)
#define DSP_BSP_RCER_CH30_EN                                 ((uint32_t)0x40000000)
#define DSP_BSP_RCER_CH31_EN                                 ((uint32_t)0x80000000)

/*-- DSP_BSP_PCR_SPSR:DSP_BSP PCR_SPSR Register -----*/
/* Bit field positions: */
#define DSP_BSP_PCR_SPSR_CLKRP_Pos                           0
#define DSP_BSP_PCR_SPSR_CLKXP_Pos                           1
#define DSP_BSP_PCR_SPSR_FSRP_Pos                            2
#define DSP_BSP_PCR_SPSR_FSXP_Pos                            3
#define DSP_BSP_PCR_SPSR_DR_ST_Pos                           4
#define DSP_BSP_PCR_SPSR_DX_ST_Pos                           5
#define DSP_BSP_PCR_SPSR_CLKS_ST_Pos                         6
#define DSP_BSP_PCR_SPSR_CLKRM_Pos                           8
#define DSP_BSP_PCR_SPSR_CLKXM_Pos                           9
#define DSP_BSP_PCR_SPSR_FSRM_Pos                            10
#define DSP_BSP_PCR_SPSR_FSXM_Pos                            11
#define DSP_BSP_PCR_SPSR_RIOEN_Pos                           12
#define DSP_BSP_PCR_SPSR_XIOEN_Pos                           13
#define DSP_BSP_PCR_SPSR_RRDY_Pos                            16
#define DSP_BSP_PCR_SPSR_FFULL_Pos                           17
#define DSP_BSP_PCR_SPSR_RSER_Pos                            18
#define DSP_BSP_PCR_SPSR_RLOST_Pos                           19
#define DSP_BSP_PCR_SPSR_RFIFO_E_Pos                         20
#define DSP_BSP_PCR_SPSR_RFIFO_H_Pos                         21
#define DSP_BSP_PCR_SPSR_RFIFO_F_Pos                         22
#define DSP_BSP_PCR_SPSR_XRDY_Pos                            24
#define DSP_BSP_PCR_SPSR_XEMPTY_Pos                          25
#define DSP_BSP_PCR_SPSR_XSERR_Pos                           26
#define DSP_BSP_PCR_SPSR_XLOST_Pos                           27
#define DSP_BSP_PCR_SPSR_XFIFO_E_Pos                         28
#define DSP_BSP_PCR_SPSR_ZFIFO_H_Pos                         29
#define DSP_BSP_PCR_SPSR_XFIFO_F_Pos                         30
/* Bit field masks: */
#define DSP_BSP_PCR_SPSR_CLKRP                               ((uint32_t)0x00000001)
#define DSP_BSP_PCR_SPSR_CLKXP                               ((uint32_t)0x00000002)
#define DSP_BSP_PCR_SPSR_FSRP                                ((uint32_t)0x00000004)
#define DSP_BSP_PCR_SPSR_FSXP                                ((uint32_t)0x00000008)
#define DSP_BSP_PCR_SPSR_DR_ST                               ((uint32_t)0x00000010)
#define DSP_BSP_PCR_SPSR_DX_ST                               ((uint32_t)0x00000020)
#define DSP_BSP_PCR_SPSR_CLKS_ST                             ((uint32_t)0x00000040)
#define DSP_BSP_PCR_SPSR_CLKRM                               ((uint32_t)0x00000100)
#define DSP_BSP_PCR_SPSR_CLKXM                               ((uint32_t)0x00000200)
#define DSP_BSP_PCR_SPSR_FSRM                                ((uint32_t)0x00000400)
#define DSP_BSP_PCR_SPSR_FSXM                                ((uint32_t)0x00000800)
#define DSP_BSP_PCR_SPSR_RIOEN                               ((uint32_t)0x00001000)
#define DSP_BSP_PCR_SPSR_XIOEN                               ((uint32_t)0x00002000)
#define DSP_BSP_PCR_SPSR_RRDY                                ((uint32_t)0x00010000)
#define DSP_BSP_PCR_SPSR_FFULL                               ((uint32_t)0x00020000)
#define DSP_BSP_PCR_SPSR_RSER                                ((uint32_t)0x00040000)
#define DSP_BSP_PCR_SPSR_RLOST                               ((uint32_t)0x00080000)
#define DSP_BSP_PCR_SPSR_RFIFO_E                             ((uint32_t)0x00100000)
#define DSP_BSP_PCR_SPSR_RFIFO_H                             ((uint32_t)0x00200000)
#define DSP_BSP_PCR_SPSR_RFIFO_F                             ((uint32_t)0x00400000)
#define DSP_BSP_PCR_SPSR_XRDY                                ((uint32_t)0x01000000)
#define DSP_BSP_PCR_SPSR_XEMPTY                              ((uint32_t)0x02000000)
#define DSP_BSP_PCR_SPSR_XSERR                               ((uint32_t)0x04000000)
#define DSP_BSP_PCR_SPSR_XLOST                               ((uint32_t)0x08000000)
#define DSP_BSP_PCR_SPSR_XFIFO_E                             ((uint32_t)0x10000000)
#define DSP_BSP_PCR_SPSR_ZFIFO_H                             ((uint32_t)0x20000000)
#define DSP_BSP_PCR_SPSR_XFIFO_F                             ((uint32_t)0x40000000)

/* _dsp_timer structure */
typedef struct {
	__IO uint32_t	PDR_TIM;                /*!< */
	__IO uint32_t	TCR;                    /*!< */
} _dsp_timer;

/*-- DSP_TIMER_PDR_TIM:DSP_TIMER PDR_TIM Register -----*/
/* Bit field positions: */
#define DSP_TIMER_PDR_TIM_CNT_Pos                          0
#define DSP_TIMER_PDR_TIM_PR_Pos                           16
/* Bit field masks: */
#define DSP_TIMER_PDR_TIM_CNT_Msk                          ((uint32_t)0x0000FFFF)
#define DSP_TIMER_PDR_TIM_PR_Msk                           ((uint32_t)0xFFFF0000)

/*-- DSP_TIMER_TCR:DSP_TIMER TCR Register -----*/
/* Bit field positions: */
#define DSP_TIMER_TCR_TDDR_Pos                             0
#define DSP_TIMER_TCR_TSS_Pos                              4
#define DSP_TIMER_TCR_TRB_Pos                              5
#define DSP_TIMER_TCR_PSC_Pos                              6
#define DSP_TIMER_TCR_FREE_Pos                             8
#define DSP_TIMER_TCR_SOFT_Pos                             9
/* Bit field masks: */
#define DSP_TIMER_TCR_TDDR_Msk                             ((uint32_t)0x0000000F)
#define DSP_TIMER_TCR_TSS                                  ((uint32_t)0x00000010)
#define DSP_TIMER_TCR_TRB                                  ((uint32_t)0x00000020)
#define DSP_TIMER_TCR_PSC_Msk                              ((uint32_t)0x000000C0)
#define DSP_TIMER_TCR_FREE                                 ((uint32_t)0x00000100)
#define DSP_TIMER_TCR_SOFT                                 ((uint32_t)0x00000200)

/* _dsp_irq structure */
typedef struct
{
	__io_reg IRQ; 		/*!< AIR: bits [31..16]; DIR: bits [15..0] */	
} _dsp_irq;

/* Bit field positions (DIR register: bits [15..0], bit 14 reserved): */
#define DSP_IRQ_DIRQ0_Pos                       0
#define DSP_IRQ_DIRQ1_Pos                       1
#define DSP_IRQ_DIRQ2_Pos                       2
#define DSP_IRQ_DIRQ3_Pos                       3
#define DSP_IRQ_TIRQ_Pos                        4
#define DSP_IRQ_RIRQ1_Pos                       5
#define DSP_IRQ_XIRQ1_Pos                       6
#define DSP_IRQ_RIRQ2_Pos                       7
#define DSP_IRQ_XIRQ2_Pos                       8
#define DSP_IRQ_RIRQ3_Pos                       9
#define DSP_IRQ_XIRQ3_Pos                       10
#define DSP_IRQ_CDIRQ_Pos                       11
#define DSP_IRQ_CRIRQ_Pos                       12
#define DSP_IRQ_DMAIRQ_Pos						13                                               
#define DSP_IRQ_DSNR_Pos                       	15
/* Bit field positions (AIR register: bits [31..16], bit 21 reserved): */
#define DSP_IRQ_AIRQ0_Pos                       16
#define DSP_IRQ_AIRQ1_Pos                       17
#define DSP_IRQ_AIRQ2_Pos                       18
#define DSP_IRQ_AIRQ3_Pos                       19
#define DSP_IRQ_ANMI_Pos                        20                                               
#define DSP_IRQ_ADMARQ0_Pos						22
#define DSP_IRQ_ADMARQ1_Pos						23
#define DSP_IRQ_ADMARQ2_Pos						24
#define DSP_IRQ_ADMARQ3_Pos						25
#define DSP_IRQ_ADMADONE0_Pos					26
#define DSP_IRQ_ADMADONE1_Pos					27
#define DSP_IRQ_ADMADONE2_Pos					28
#define DSP_IRQ_ADMADONE3_Pos					29
#define DSP_IRQ_ASID_Pos                        30
#define DSP_IRQ_ASNR_Pos                        31
/* Bit field masks (DIR register: bits [15..0], bit 14 reserved (mask 0x00004000)): */
#define DSP_IRQ_DIRQ0                           ((uint32_t)0x00000001)
#define DSP_IRQ_DIRQ1                           ((uint32_t)0x00000002)
#define DSP_IRQ_DIRQ2                           ((uint32_t)0x00000004)
#define DSP_IRQ_DIRQ3                           ((uint32_t)0x00000008)
#define DSP_IRQ_TIRQ                            ((uint32_t)0x00000010)
#define DSP_IRQ_RIRQ1                           ((uint32_t)0x00000020)
#define DSP_IRQ_XIRQ1                           ((uint32_t)0x00000040)
#define DSP_IRQ_RIRQ2                           ((uint32_t)0x00000080)
#define DSP_IRQ_XIRQ2                           ((uint32_t)0x00000100)
#define DSP_IRQ_RIRQ3                           ((uint32_t)0x00000200)
#define DSP_IRQ_XIRQ3                           ((uint32_t)0x00000400)
#define DSP_IRQ_CDIRQ                           ((uint32_t)0x00000800)
#define DSP_IRQ_CRIRQ                           ((uint32_t)0x00001000)
#define DSP_IRQ_DMAIRQ							((uint32_t)0x00002000)
#define DSP_IRQ_DSNR                            ((uint32_t)0x00008000)
/* Bit field masks (AIR register: bits [31..16], bit 21 reserved (mask 0x00200000)): */
#define DSP_IRQ_AIRQ0                           ((uint32_t)0x00010000)
#define DSP_IRQ_AIRQ1                           ((uint32_t)0x00020000)
#define DSP_IRQ_AIRQ2                           ((uint32_t)0x00040000)
#define DSP_IRQ_AIRQ3                           ((uint32_t)0x00080000)
#define DSP_IRQ_ANMI                            ((uint32_t)0x00100000)													
#define DSP_IRQ_ADMARQ0							((uint32_t)0x00400000)
#define DSP_IRQ_ADMARQ1							((uint32_t)0x00800000)
#define DSP_IRQ_ADMARQ2							((uint32_t)0x01000000)
#define DSP_IRQ_ADMARQ3							((uint32_t)0x02000000)
#define DSP_IRQ_ADMADONE0						((uint32_t)0x04000000)
#define DSP_IRQ_ADMADONE1						((uint32_t)0x08000000)
#define DSP_IRQ_ADMADONE2						((uint32_t)0x10000000)
#define DSP_IRQ_ADMADONE3						((uint32_t)0x20000000)
#define DSP_IRQ_ASID                    		((uint32_t)0x40000000)
#define DSP_IRQ_ASNR                            ((uint32_t)0x80000000)

/* _dsp_clk structure */
typedef struct
{
	__io_reg CLKMD;
} _dsp_clk;

/* Bit field positions (bits [2..0] reserved): */
#define DSP_CLK_CLKMD_CRP_Pos                   3
#define DSP_CLK_CLKMD_CDC_Pos                   4
#define DSP_CLK_CLKMD_TMR_Pos                   5
#define DSP_CLK_CLKMD_DMA_Pos                   6
#define DSP_CLK_CLKMD_MC3_Pos                   7
#define DSP_CLK_CLKMD_PC3_Pos                   8
#define DSP_CLK_CLKMD_MC2_Pos                   9
#define DSP_CLK_CLKMD_PC2_Pos                   10
#define DSP_CLK_CLKMD_MC1_Pos                   11
#define DSP_CLK_CLKMD_PC1_Pos                   12
#define DSP_CLK_CLKMD_CDM_Pos                   13
#define DSP_CLK_CLKMD_CPM_Pos                   14
#define DSP_CLK_CLKMD_CPU_Pos                   15
/* Bit field masks: */
#define DSP_CLK_CLKMD_CRP                       ((uint32_t)0x00000008)
#define DSP_CLK_CLKMD_CDC                       ((uint32_t)0x00000010)
#define DSP_CLK_CLKMD_TMR                       ((uint32_t)0x00000020)
#define DSP_CLK_CLKMD_DMA                       ((uint32_t)0x00000040)
#define DSP_CLK_CLKMD_MC3                       ((uint32_t)0x00000080)
#define DSP_CLK_CLKMD_PC3                       ((uint32_t)0x00000100)
#define DSP_CLK_CLKMD_MC2                       ((uint32_t)0x00000200)
#define DSP_CLK_CLKMD_PC2                       ((uint32_t)0x00000400)
#define DSP_CLK_CLKMD_MC1                       ((uint32_t)0x00000800)
#define DSP_CLK_CLKMD_PC1                       ((uint32_t)0x00001000)
#define DSP_CLK_CLKMD_CDM                       ((uint32_t)0x00002000)
#define DSP_CLK_CLKMD_CPM                       ((uint32_t)0x00004000)
#define DSP_CLK_CLKMD_CPU                       ((uint32_t)0x00008000)

/* _dsp_core structure (IRQ + CLKMD, duplicate structures _dsp_irq and _dsp_clk) */
typedef struct {	
	__io_reg IRQ;           /*!< 0x30000078: AIR: bits [31..16]; DIR: bits [15..0] */	
	__io_reg RESERVED0[16]; /*!< 0x3000007C: skip 2+16+14=32 16-bit words (2 + CRYPTO + ACODEC registers) */
	__io_reg CLKMD;         /*!< 0x300000BC: CLKMD */
} _dsp_core;

/*-- DSP_CORE_IRQ: DIR Register (bits [15..0])  -----*/
/*-- DSP_CORE_IRQ: AIR Register (bits [31..16]) -----*/
/* Bit field positions (DIR register: bits [15..0], bit 14 reserved): */
#define DSP_CORE_IRQ_DIRQ0_Pos                             0
#define DSP_CORE_IRQ_DIRQ1_Pos                             1
#define DSP_CORE_IRQ_DIRQ2_Pos                             2
#define DSP_CORE_IRQ_DIRQ3_Pos                             3
#define DSP_CORE_IRQ_TIRQ_Pos                              4
#define DSP_CORE_IRQ_RIRQ1_Pos                             5
#define DSP_CORE_IRQ_XIRQ1_Pos                             6
#define DSP_CORE_IRQ_RIRQ2_Pos                             7
#define DSP_CORE_IRQ_XIRQ2_Pos                             8
#define DSP_CORE_IRQ_RIRQ3_Pos                             9
#define DSP_CORE_IRQ_XIRQ3_Pos                             10
#define DSP_CORE_IRQ_CDIRQ_Pos                             11
#define DSP_CORE_IRQ_CRIRQ_Pos                             12
#define DSP_CORE_IRQ_DMAIRQ_Pos                            13
#define DSP_CORE_IRQ_DSNR_Pos                              15
/* Bit field positions (AIR register: bits [31..16], bit 21 reserved): */
#define DSP_CORE_IRQ_AIRQ0_Pos                             16
#define DSP_CORE_IRQ_AIRQ1_Pos                             17
#define DSP_CORE_IRQ_AIRQ2_Pos                             18
#define DSP_CORE_IRQ_AIRQ3_Pos                             19
#define DSP_CORE_IRQ_ANMI_Pos                              20
#define DSP_CORE_IRQ_ADMARQ0_Pos                           22
#define DSP_CORE_IRQ_ADMARQ1_Pos                           23
#define DSP_CORE_IRQ_ADMARQ2_Pos                           24
#define DSP_CORE_IRQ_ADMARQ3_Pos                           25
#define DSP_CORE_IRQ_ADMADONE0_Pos                         26
#define DSP_CORE_IRQ_ADMADONE1_Pos                         27
#define DSP_CORE_IRQ_ADMADONE2_Pos                         28
#define DSP_CORE_IRQ_ADMADONE3_Pos                         29
#define DSP_CORE_IRQ_ASID_Pos                              30
#define DSP_CORE_IRQ_ASNR_Pos                              31
/* Bit field masks (DIR register: bits [15..0], bit 14 reserved): */
#define DSP_CORE_IRQ_DIRQ0                                 ((uint32_t)0x00000001)
#define DSP_CORE_IRQ_DIRQ1                                 ((uint32_t)0x00000002)
#define DSP_CORE_IRQ_DIRQ2                                 ((uint32_t)0x00000004)
#define DSP_CORE_IRQ_DIRQ3                                 ((uint32_t)0x00000008)
#define DSP_CORE_IRQ_TIRQ                                  ((uint32_t)0x00000010)
#define DSP_CORE_IRQ_RIRQ1                                 ((uint32_t)0x00000020)
#define DSP_CORE_IRQ_XIRQ1                                 ((uint32_t)0x00000040)
#define DSP_CORE_IRQ_RIRQ2                                 ((uint32_t)0x00000080)
#define DSP_CORE_IRQ_XIRQ2                                 ((uint32_t)0x00000100)
#define DSP_CORE_IRQ_RIRQ3                                 ((uint32_t)0x00000200)
#define DSP_CORE_IRQ_XIRQ3                                 ((uint32_t)0x00000400)
#define DSP_CORE_IRQ_CDIRQ                                 ((uint32_t)0x00000800)
#define DSP_CORE_IRQ_CRIRQ                                 ((uint32_t)0x00001000)
#define DSP_CORE_IRQ_DMAIRQ                                ((uint32_t)0x00002000)
#define DSP_CORE_IRQ_DSNR                                  ((uint32_t)0x00008000)
/* Bit field masks (AIR register: bits [31..16], bit 21 reserved): */
#define DSP_CORE_IRQ_AIRQ0                                 ((uint32_t)0x00010000)
#define DSP_CORE_IRQ_AIRQ1                                 ((uint32_t)0x00020000)
#define DSP_CORE_IRQ_AIRQ2                                 ((uint32_t)0x00040000)
#define DSP_CORE_IRQ_AIRQ3                                 ((uint32_t)0x00080000)
#define DSP_CORE_IRQ_ANMI                                  ((uint32_t)0x00100000)
#define DSP_CORE_IRQ_ADMARQ0                               ((uint32_t)0x00400000)
#define DSP_CORE_IRQ_ADMARQ1                               ((uint32_t)0x00800000)
#define DSP_CORE_IRQ_ADMARQ2                               ((uint32_t)0x01000000)
#define DSP_CORE_IRQ_ADMARQ3                               ((uint32_t)0x02000000)
#define DSP_CORE_IRQ_ADMADONE0                             ((uint32_t)0x04000000)
#define DSP_CORE_IRQ_ADMADONE1                             ((uint32_t)0x08000000)
#define DSP_CORE_IRQ_ADMADONE2                             ((uint32_t)0x10000000)
#define DSP_CORE_IRQ_ADMADONE3                             ((uint32_t)0x20000000)
#define DSP_CORE_IRQ_ASID                                  ((uint32_t)0x40000000)
#define DSP_CORE_IRQ_ASNR                                  ((uint32_t)0x80000000)

/*-- DSP_CORE_CLKMD: DSP_CORE CLKMD Register -----*/
/* Bit field positions (bits [2..0] reserved): */
#define DSP_CORE_CLKMD_CRP_Pos                            3
#define DSP_CORE_CLKMD_CDC_Pos                            4
#define DSP_CORE_CLKMD_TMR_Pos                            5
#define DSP_CORE_CLKMD_DMA_Pos                            6
#define DSP_CORE_CLKMD_MC3_Pos                            7
#define DSP_CORE_CLKMD_PC3_Pos                            8
#define DSP_CORE_CLKMD_MC2_Pos                            9
#define DSP_CORE_CLKMD_PC2_Pos                            10
#define DSP_CORE_CLKMD_MC1_Pos                            11
#define DSP_CORE_CLKMD_PC1_Pos                            12
#define DSP_CORE_CLKMD_CDM_Pos                            13
#define DSP_CORE_CLKMD_CPM_Pos                            14
#define DSP_CORE_CLKMD_CPU_Pos                            15
/* Bit field masks: */
#define DSP_CORE_CLKMD_CRP                                ((uint32_t)0x00000008)
#define DSP_CORE_CLKMD_CDC                                ((uint32_t)0x00000010)
#define DSP_CORE_CLKMD_TMR                                ((uint32_t)0x00000020)
#define DSP_CORE_CLKMD_DMA                                ((uint32_t)0x00000040)
#define DSP_CORE_CLKMD_MC3                                ((uint32_t)0x00000080)
#define DSP_CORE_CLKMD_PC3                                ((uint32_t)0x00000100)
#define DSP_CORE_CLKMD_MC2                                ((uint32_t)0x00000200)
#define DSP_CORE_CLKMD_PC2                                ((uint32_t)0x00000400)
#define DSP_CORE_CLKMD_MC1                                ((uint32_t)0x00000800)
#define DSP_CORE_CLKMD_PC1                                ((uint32_t)0x00001000)
#define DSP_CORE_CLKMD_CDM                                ((uint32_t)0x00002000)
#define DSP_CORE_CLKMD_CPM                                ((uint32_t)0x00004000)
#define DSP_CORE_CLKMD_CPU                                ((uint32_t)0x00008000)

/* _dsp_crypto structure */
typedef struct {
	__IO uint32_t	CWR;                    /*!< */
	__IO uint32_t	SR;                     /*!< */
	__IO uint32_t	DATA;                   /*!< */
	__IO uint32_t	KR;                     /*!< */
	__IO uint32_t	SYNR;                   /*!< */
	__IO uint32_t	CR;                     /*!< */
	__IO uint32_t	IMIT;                   /*!< */
	__IO uint32_t	ITER;                   /*!< */
} _dsp_crypto;

/*-- DSP_CRYPTO_CWR:DSP_CRYPTO CWR Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_CWR_MODE_Pos                              0
#define DSP_CRYPTO_CWR_IM_Pos                                2
#define DSP_CRYPTO_CWR_START_Pos                             3
#define DSP_CRYPTO_CWR_DIR_Pos                               4
#define DSP_CRYPTO_CWR_IE_CRPT_Pos                           5
#define DSP_CRYPTO_CWR_RST_Pos                               6
#define DSP_CRYPTO_CWR_BIST_Pos                              7
/* Bit field masks: */
#define DSP_CRYPTO_CWR_MODE_Msk                              ((uint32_t)0x00000003)
#define DSP_CRYPTO_CWR_IM                                    ((uint32_t)0x00000004)
#define DSP_CRYPTO_CWR_START                                 ((uint32_t)0x00000008)
#define DSP_CRYPTO_CWR_DIR                                   ((uint32_t)0x00000010)
#define DSP_CRYPTO_CWR_IE_CRPT                               ((uint32_t)0x00000020)
#define DSP_CRYPTO_CWR_RST                                   ((uint32_t)0x00000040)
#define DSP_CRYPTO_CWR_BIST                                  ((uint32_t)0x00000080)

/*-- DSP_CRYPTO_SR:DSP_CRYPTO SR Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_SR_READY_Pos                              0
#define DSP_CRYPTO_SR_ERROR_Pos                              1
#define DSP_CRYPTO_SR_KF_Pos                                 2
#define DSP_CRYPTO_SR_DW_Pos                                 3
#define DSP_CRYPTO_SR_DR_Pos                                 4
#define DSP_CRYPTO_SR_SW_Pos                                 5
#define DSP_CRYPTO_SR_KW_Pos                                 6
/* Bit field masks: */
#define DSP_CRYPTO_SR_READY                                  ((uint32_t)0x00000001)
#define DSP_CRYPTO_SR_ERROR                                  ((uint32_t)0x00000002)
#define DSP_CRYPTO_SR_KF                                     ((uint32_t)0x00000004)
#define DSP_CRYPTO_SR_DW                                     ((uint32_t)0x00000008)
#define DSP_CRYPTO_SR_DR                                     ((uint32_t)0x00000010)
#define DSP_CRYPTO_SR_SW                                     ((uint32_t)0x00000020)
#define DSP_CRYPTO_SR_KW                                     ((uint32_t)0x00000040)

/*-- DSP_CRYPTO_DATA:DSP_CRYPTO DATA Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_DATA_DATA_Pos                             0
/* Bit field masks: */
#define DSP_CRYPTO_DATA_DATA_Msk                             ((uint32_t)0x0000FFFF)

/*-- DSP_CRYPTO_KR:DSP_CRYPTO KR Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_KR_KEY_Pos                                0
/* Bit field masks: */
#define DSP_CRYPTO_KR_KEY_Msk                                ((uint32_t)0x0000FFFF)

/*-- DSP_CRYPTO_SYNR:DSP_CRYPTO SYNR Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_SYNR_KEY_Pos                              0
#define DSP_CRYPTO_SYNR_SYNCH_Pos                            1
/* Bit field masks: */
#define DSP_CRYPTO_SYNR_KEY                                  ((uint32_t)0x00000001)
#define DSP_CRYPTO_SYNR_SYNCH_Msk                            ((uint32_t)0x0000FFFE)

/*-- DSP_CRYPTO_CR:DSP_CRYPTO CR Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_CR_CONST_Pos                              0
/* Bit field masks: */
#define DSP_CRYPTO_CR_CONST_Msk                              ((uint32_t)0x0000FFFF)

/*-- DSP_CRYPTO_IMIT:DSP_CRYPTO IMIT Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_IMIT_IMIT_Pos                             0
/* Bit field masks: */
#define DSP_CRYPTO_IMIT_IMIT_Msk                             ((uint32_t)0x0000FFFF)

/*-- DSP_CRYPTO_ITER:DSP_CRYPTO ITER Register -----*/
/* Bit field positions: */
#define DSP_CRYPTO_ITER_ITER0_Pos                            0
#define DSP_CRYPTO_ITER_ITER1_Pos                            1
#define DSP_CRYPTO_ITER_ITER2_Pos                            2
#define DSP_CRYPTO_ITER_ITER3_Pos                            3
#define DSP_CRYPTO_ITER_ITER4_Pos                            4
#define DSP_CRYPTO_ITER_ITER5_Pos                            5
#define DSP_CRYPTO_ITER_EN_CRPT_Pos                          6
/* Bit field masks: */
#define DSP_CRYPTO_ITER_ITER0                                ((uint32_t)0x00000001)
#define DSP_CRYPTO_ITER_ITER1                                ((uint32_t)0x00000002)
#define DSP_CRYPTO_ITER_ITER2                                ((uint32_t)0x00000004)
#define DSP_CRYPTO_ITER_ITER3                                ((uint32_t)0x00000008)
#define DSP_CRYPTO_ITER_ITER4                                ((uint32_t)0x00000010)
#define DSP_CRYPTO_ITER_ITER5                                ((uint32_t)0x00000020)
#define DSP_CRYPTO_ITER_EN_CRPT                              ((uint32_t)0x00000040)

/* _dsp_acodec structure */
typedef struct {
	__IO uint32_t	POWCTL;                 /*!<AUDIO control register */
	__IO uint32_t	ADCCTL;                 /*!<ADC control register */
	__IO uint32_t	DACCTL;                 /*!<DAC control register */
	__IO uint32_t	MASKCTL;                /*!<IRQ mask register */
	__IO uint32_t	IRQFLAG;                /*!<IRQ flag register */
	__IO uint32_t	ADCREG;                 /*!<ADC FIFO register */
	__IO uint32_t	DACREG;                 /*!<DAC FIFO register */
} _dsp_acodec;

/*-- DSP_ACODEC_POWCTL:DSP_ACODEC POWCTL Register -----*/
/* Bit field positions: */
#define DSP_ACODEC_POWCTL_ADCEN_Pos                          0
#define DSP_ACODEC_POWCTL_DACEN_Pos                          1
#define DSP_ACODEC_POWCTL_IIREN_Pos                          2
#define DSP_ACODEC_POWCTL_ALB_Pos                            3
#define DSP_ACODEC_POWCTL_DLB_Pos                            4
/* Bit field masks: */
#define DSP_ACODEC_POWCTL_ADCEN                              ((uint32_t)0x00000001)
#define DSP_ACODEC_POWCTL_DACEN                              ((uint32_t)0x00000002)
#define DSP_ACODEC_POWCTL_IIREN                              ((uint32_t)0x00000004)
#define DSP_ACODEC_POWCTL_ALB                                ((uint32_t)0x00000008)
#define DSP_ACODEC_POWCTL_DLB                                ((uint32_t)0x00000010)

/*-- DSP_ACODEC_ADCCTL:DSP_ACODEC ADCCTL Register -----*/
/* Bit field positions: */
#define DSP_ACODEC_ADCCTL_ADGAIN_Pos                         0
#define DSP_ACODEC_ADCCTL_INBG_Pos                           6
#define DSP_ACODEC_ADCCTL_AINSEL_Pos                         8
#define DSP_ACODEC_ADCCTL_ICONT_Pos                          10
/* Bit field masks: */
#define DSP_ACODEC_ADCCTL_ADGAIN_Msk                         ((uint32_t)0x0000003F)
#define DSP_ACODEC_ADCCTL_INBG_Msk                           ((uint32_t)0x000000C0)
#define DSP_ACODEC_ADCCTL_AINSEL_Msk                         ((uint32_t)0x00000300)
#define DSP_ACODEC_ADCCTL_ICONT                              ((uint32_t)0x00000400)

/*-- DSP_ACODEC_DACCTL:DSP_ACODEC DACCTL Register -----*/
/* Bit field positions: */
#define DSP_ACODEC_DACCTL_DAGAIN_Pos                         0
#define DSP_ACODEC_DACCTL_MUTE1_Pos                          6
#define DSP_ACODEC_DACCTL_ODAMP_Pos                          7
#define DSP_ACODEC_DACCTL_ODBIAS_Pos                         8
#define DSP_ACODEC_DACCTL_ODDAC_Pos                          9
#define DSP_ACODEC_DACCTL_OVECBA_Pos                         10
#define DSP_ACODEC_DACCTL_OVECBS_Pos                         11
#define DSP_ACODEC_DACCTL_SIDETONE_Pos                       12
#define DSP_ACODEC_DACCTL_DACRES_Pos                         15
/* Bit field masks: */
#define DSP_ACODEC_DACCTL_DAGAIN_Msk                         ((uint32_t)0x0000003F)
#define DSP_ACODEC_DACCTL_MUTE1                              ((uint32_t)0x00000040)
#define DSP_ACODEC_DACCTL_ODAMP                              ((uint32_t)0x00000080)
#define DSP_ACODEC_DACCTL_ODBIAS                             ((uint32_t)0x00000100)
#define DSP_ACODEC_DACCTL_ODDAC                              ((uint32_t)0x00000200)
#define DSP_ACODEC_DACCTL_OVECBA                             ((uint32_t)0x00000400)
#define DSP_ACODEC_DACCTL_OVECBS                             ((uint32_t)0x00000800)
#define DSP_ACODEC_DACCTL_SIDETONE_Msk                       ((uint32_t)0x00007000)
#define DSP_ACODEC_DACCTL_DACRES                             ((uint32_t)0x00008000)

/*-- DSP_ACODEC_MASKCTL:DSP_ACODEC MASKCTL Register -----*/
/* Bit field positions: */
#define DSP_ACODEC_MASKCTL_DAOVFM_Pos                        0
#define DSP_ACODEC_MASKCTL_ADCVFM_Pos                        1
#define DSP_ACODEC_MASKCTL_ADCNSM_Pos                        2
#define DSP_ACODEC_MASKCTL_DACNSM_Pos                        3
#define DSP_ACODEC_MASKCTL_OVERCURM_Pos                      4
/* Bit field masks: */
#define DSP_ACODEC_MASKCTL_DAOVFM                            ((uint32_t)0x00000001)
#define DSP_ACODEC_MASKCTL_ADCVFM                            ((uint32_t)0x00000002)
#define DSP_ACODEC_MASKCTL_ADCNSM                            ((uint32_t)0x00000004)
#define DSP_ACODEC_MASKCTL_DACNSM                            ((uint32_t)0x00000008)
#define DSP_ACODEC_MASKCTL_OVERCURM                          ((uint32_t)0x00000010)

/*-- DSP_ACODEC_IRQFLAG:DSP_ACODEC IRQFLAG Register -----*/
/* Bit field positions: */
#define DSP_ACODEC_IRQFLAG_DAOVF_Pos                         0
#define DSP_ACODEC_IRQFLAG_ADCVF_Pos                         1
#define DSP_ACODEC_IRQFLAG_ADCNS_Pos                         2
#define DSP_ACODEC_IRQFLAG_DACNS_Pos                         3
#define DSP_ACODEC_IRQFLAG_OVERVCUR_Pos                      4
/* Bit field masks: */
#define DSP_ACODEC_IRQFLAG_DAOVF                             ((uint32_t)0x00000001)
#define DSP_ACODEC_IRQFLAG_ADCVF                             ((uint32_t)0x00000002)
#define DSP_ACODEC_IRQFLAG_ADCNS                             ((uint32_t)0x00000004)
#define DSP_ACODEC_IRQFLAG_DACNS                             ((uint32_t)0x00000008)
#define DSP_ACODEC_IRQFLAG_OVERVCUR                          ((uint32_t)0x00000010)

/*-- DSP_ACODEC_ADCREG:DSP_ACODEC ADCREG Register -----*/
/* Bit field positions: */
#define DSP_ACODEC_ADCREG_ADCSMP_Pos                         0
/* Bit field masks: */
#define DSP_ACODEC_ADCREG_ADCSMP_Msk                         ((uint32_t)0x0000FFFF)

/*-- DSP_ACODEC_DACREG:DSP_ACODEC DACREG Register -----*/
/* Bit field positions: */
#define DSP_ACODEC_DACREG_DACSMP_Pos                         0
/* Bit field masks: */
#define DSP_ACODEC_DACREG_DACSMP_Msk                         ((uint32_t)0x0000FFFF)

/* _dsp_dma structure */
typedef struct {
	__IO uint32_t	STATUS;                 /*!< */
	__IO uint32_t	CFG;                    /*!< */
	__IO uint32_t	CTRL_BASE_PTR;          /*!< */
	__IO uint32_t	ALT_CTRL_BASE_PTR;      /*!< */
	__IO uint32_t	WAITONREG_STATUS;       /*!< */
	__IO uint32_t	CHNL_SW_REQUEST;        /*!< */
	__IO uint32_t	CHNL_USEBURST_SET;      /*!< */
	__IO uint32_t	CHNL_USEBURST_CLR;      /*!< */
	__IO uint32_t	CHNL_REQ_MASK_SET;      /*!< */
	__IO uint32_t	CHNL_REQ_MASK_CLR;      /*!< */
	__IO uint32_t	CHNL_ENABLE_SET;        /*!< */
	__IO uint32_t	CHNL_ENABLE_CLR;        /*!< */
	__IO uint32_t	CHNL_PRI_ALT_SET;       /*!< */
	__IO uint32_t	CHNL_PRI_ALT_CLR;       /*!< */
	__IO uint32_t	CHNL_PRIORITY_SET;      /*!< */
	__IO uint32_t	CHNL_PRIORITY_CLR;      /*!< */	
} _dsp_dma;
  
/*-- DSP_DMA_DMA_STATUS:DSP_DMA DMA_STATUS Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_DMA_CONFIG:DSP_DMA DMA_CONFIG Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CTRL_BASE_PTR:DSP_DMA CTRL_BASE_PTR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_ALT_CTRL_BASE_PTR:DSP_DMA ALT_CTRL_BASE_PTR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_DMA_WAITONREG_STATUS:DSP_DMA DMA_WAITONREG_STATUS Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_SW_REQUEST:DSP_DMA CHNL_SW_REQUEST Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_USEBURST_SET:DSP_DMA CHNL_USEBURST_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_USEBURST_CLR:DSP_DMA CHNL_USEBURST_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_REQ_MASK_SET:DSP_DMA CHNL_REQ_MASK_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_REQ_MASK_CLR:DSP_DMA CHNL_REQ_MASK_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_ENABLE_SET:DSP_DMA CHNL_ENABLE_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_ENABLE_CLR:DSP_DMA CHNL_ENABLE_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_PRI_ALT_SET:DSP_DMA CHNL_PRI_ALT_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_PRI_ALT_CLR:DSP_DMA CHNL_PRI_ALT_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_PRIORITY_SET:DSP_DMA CHNL_PRIORITY_SET Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_CHNL_PRIORITY_CLR:DSP_DMA CHNL_PRIORITY_CLR Register -----*/
/* Bit field positions: */
/* Bit field masks: */

/*-- DSP_DMA_ERR_CLR:DSP_DMA ERR_CLR Register -----*/
/* Bit field positions: */
#define DSP_DMA_ERR_CLR_CHNL_PRIORITY_SET_Pos              0
/* Bit field masks: */
#define DSP_DMA_ERR_CLR_CHNL_PRIORITY_SET                  ((uint32_t)0x00000001)

/* _sysctrl structure */
typedef struct 
{
    __io_reg __dummy0;
    __io_reg __dummy1;
    __io_reg ACTLR;
    __io_reg __dummy2;
} _sysctrl;

/* _systick structure */
typedef struct
{
  __io_reg CTRL;
  __io_reg LOAD;
  __io_reg VAL;
  __io_reg CALIB;
} _systick;

/* _nvic structure */
typedef struct
{
	__io_reg ISER[1];       //0x0000
	__io_reg __dummy0[31];
    __io_reg ICER[1];       //0x0080
	__io_reg __dummy1[31];
    __io_reg ISPR[1];       //0x0100
	__io_reg __dummy2[31];
    __io_reg ICPR[1];       //0x0180
	__io_reg __dummy3[31];
    __io_reg IABR[1];       //0x0200
	__io_reg __dummy4[31];    
    __io_reg __dummy5[32];  //0x0280
    __io_reg IP[8];         //0x0300
} _nvic;

/* _sys structure */
typedef struct
{
	__io_reg CPUID;
    __io_reg ICSR;
    __io_reg VTOR;
    __io_reg AIRCR;
    __io_reg SCR;
    __io_reg CCR;
    __io_reg SHPR[3];    
    __io_reg SHCSR;
    __io_reg CFSR;
    __io_reg HFSR;
    __io_reg __dummy0;
    __io_reg MMFAR;
    __io_reg BFAR;
} _sys;

/* _mpu structure */
typedef struct 
{
    __io_reg TYPE;    
    __io_reg CTRL;
    __io_reg RNR;
    __io_reg RBAR;
    __io_reg RASR; 
} _mpu;

#define BASE_SSP3                  (0x40000000)
#define BASE_SSP4                  (0x40008000)
#define BASE_USB                   (0x40010000)
#define BASE_EEPROM                (0x40018000)
#define BASE_RST_CLK               (0x40020000)
#define BASE_DMA                   (0x40028000)
#define BASE_UART1                 (0x40030000)
#define BASE_UART2                 (0x40038000)
#define BASE_SSP1                  (0x40040000)
#define BASE_SDIO                  (0x40048000)
#define BASE_I2C                   (0x40050000)
#define BASE_POWER                 (0x40058000)
#define BASE_WWDG                  (0x40060000)
#define BASE_IWDG                  (0x40068000)
#define BASE_TIMER1                (0x40070000)
#define BASE_TIMER2                (0x40078000)
#define BASE_TIMER3                (0x40080000)
#define BASE_ADC                   (0x40088000)
#define BASE_DAC                   (0x40090000)
#define BASE_COMP                  (0x40098000)
#define BASE_SSP2                  (0x400A0000)
#define BASE_PORTA                 (0x400A8000)
#define BASE_PORTB                 (0x400B0000)
#define BASE_PORTC                 (0x400B8000)
#define BASE_PORTD                 (0x400C0000)
#define BASE_PORTE                 (0x400C8000)
#define BASE_UART3                 (0x400D0000)
#define BASE_BKP                   (0x400D8000)
#define BASE_PORTF                 (0x400E8000)
#define BASE_EXTBUS                (0x400F0000)
#define BASE_DSP_BSP1              (0x30000040)
#define BASE_DSP_BSP2              (0x30000050)
#define BASE_DSP_BSP3              (0x30000060)
#define BASE_DSP_TIMER             (0x30000070)
#define BASE_DSP_CORE              (0x30000078)
#define BASE_DSP_IRQ		       (0x30000078)
#define BASE_DSP_CRYPTO            (0x30000080)
#define BASE_DSP_ACODEC            (0x300000A0)
#define BASE_DSP_CLK			   (0x300000BC)
#define BASE_DSP_DMA               (0x300000C0)
#define BASE_SYSCTRL    	  	   (0xE000E000)
#define BASE_SYSTICK		  	   (0xE000E010)
#define BASE_NVIC	    	  	   (0xE000E100)
#define BASE_SYS	    	  	   (0xE000ED00)
#define BASE_MPU	    	  	   (0xE000ED90)

#define SSP3                       ((_ssp 	  *) BASE_SSP3)
#define SSP4                       ((_ssp 	  *) BASE_SSP4)
#define USB                        ((_usb 	  *) BASE_USB)
#define EEPROM                     ((_eeprom  *) BASE_EEPROM)
#define RST_CLK                    ((_rst_clk *) BASE_RST_CLK)
#define DMA                        ((_dma 	  *) BASE_DMA)
#define UART1                      ((_uart 	  *) BASE_UART1)
#define UART2                      ((_uart 	  *) BASE_UART2)
#define SSP1                       ((_ssp 	  *) BASE_SSP1)
#define SDIO                       ((_sdio 	  *) BASE_SDIO)
#define I2C                        ((_i2c  	  *) BASE_I2C)
#define POWER                      ((_power   *) BASE_POWER)
#define WWDG                       ((_wwdg 	  *) BASE_WWDG)
#define IWDG                       ((_iwdg    *) BASE_IWDG)
#define TIMER1                     ((_timer   *) BASE_TIMER1)
#define TIMER2                     ((_timer   *) BASE_TIMER2)
#define TIMER3                     ((_timer   *) BASE_TIMER3)
#define ADC                        ((_adc     *) BASE_ADC)
#define DAC                        ((_dac     *) BASE_DAC)
#define COMP                       ((_comp    *) BASE_COMP)
#define SSP2                       ((_ssp     *) BASE_SSP2)
#define PORTA                      ((_port    *) BASE_PORTA)
#define PORTB                      ((_port    *) BASE_PORTB)
#define PORTC                      ((_port    *) BASE_PORTC)
#define PORTD                      ((_port    *) BASE_PORTD)
#define PORTE                      ((_port    *) BASE_PORTE)
#define UART3                      ((_uart    *) BASE_UART3)
#define BKP                        ((_bkp     *) BASE_BKP)
#define PORTF                      ((_port    *) BASE_PORTF)
#define EXTBUS					   ((_extbus  *) BASE_EXTBUS)
#define DSP_BSP1				   ((_dsp_bsp    *) BASE_DSP_BSP1)
#define DSP_BSP2        	       ((_dsp_bsp    *) BASE_DSP_BSP2)
#define DSP_BSP3                   ((_dsp_bsp    *) BASE_DSP_BSP3)
#define DSP_TIMER                  ((_dsp_timer  *) BASE_DSP_TIMER)
#define DSP_CORE                   ((_dsp_core   *) BASE_DSP_CORE)
#define DSP_IRQ					   ((_dsp_irq    *) BASE_DSP_IRQ)
#define DSP_CRYPTO                 ((_dsp_crypto *) BASE_DSP_CRYPTO)
#define DSP_ACODEC                 ((_dsp_acodec *) BASE_DSP_ACODEC)
#define DSP_CLK				   	   ((_dsp_clk    *) BASE_DSP_CLK)
#define DSP_DMA                    ((_dsp_dma    *) BASE_DSP_DMA)
#define SYSCTRL 	    	  	   ((_sysctrl *) BASE_SYSCTRL)
#define SYSTICK 	    	  	   ((_systick *) BASE_SYSTICK)
#define NVIC		    	  	   ((_nvic    *) BASE_NVIC)
#define SYS		        	  	   ((_sys	  *) BASE_SYS)
#define MPU		        	  	   ((_mpu	  *) BASE_MPU)

#ifdef __cplusplus
}
#endif

#endif /* __1901BC1X_SFR_H */

/*===========================================================================
 * End of file 1901BC1x.h
 *===========================================================================*/
