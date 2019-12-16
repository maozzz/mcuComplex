/*===========================================================================
 *
 *  MCU library
 *
 *  (C) 2012, Phyton
 *  (C) 2016, Marusenkov S.A. "ostfriesland@mail.ru"
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE1x.h: Special Function Register (SFR) declarations
 *                   for Milandr 1986BE1x.
 *
 *===========================================================================*/

#ifndef __1986BE1X_SFR_H
#define __1986BE1X_SFR_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined ( __CC_ARM   )
  #pragma anon_unions
#endif

typedef volatile unsigned short __io_reg16;
typedef volatile unsigned long  __io_reg;

/* 1986BE1x Interrupt Number Definition */
typedef enum IRQn
{
/*-- Cortex-M1 Processor Exceptions Numbers ----------------------------------*/
  NonMaskableInt_IRQn     = -14,  /*!<  2 Non Maskable Interrupt              */
  HardFault_IRQn          = -13,  /*!<  3 Hard Fault Interrupt                */
  SVCall_IRQn             = -5,   /*!< 11 SV Call Interrupt                   */
  PendSV_IRQn             = -2,   /*!< 14 Pend SV Interrupt                   */
  SysTick_IRQn            = -1,   /*!< 15 System Tick Timer Interrupt         */

/*-- 1986BE1x specific Interrupt Numbers -------------------------------------*/
  MILSTD2_IRQn            =  0,   /*!< MILSTD2 Interrupt                      */
  MILSTD1_IRQn            =  1,   /*!< MILSTD1 Interrupt                      */
  USB_IRQn                =  2,   /*!< USB Host Interrupt                     */
  CAN1_IRQn               =  3,   /*!< CAN1 Interrupt                         */
  CAN2_IRQn               =  4,   /*!< CAN2 Interrupt                         */
  DMA_IRQn                =  5,   /*!< DMA Interrupt                          */
  UART1_IRQn              =  6,   /*!< UART1 Interrupt                        */
  UART2_IRQn              =  7,   /*!< UART2 Interrupt                        */
  SSP1_IRQn               =  8,   /*!< SSP1 Interrupt                         */
  BUSY_IRQn               =  9,   /*!< NAND Flash BUSY Interrupt              */
  ARINC429R_IRQn          =  10,  /*!< ARINC429R1-8 Receiver Interrupt        */
  POWER_IRQn              =  11,  /*!< POWER Detecor Interrupt                */
  WWDG_IRQn               =  12,  /*!< Window Watchdog Interrupt              */
  Timer4_IRQn             =  13,  /*!< Timer4 Interrupt                       */
  Timer1_IRQn             =  14,  /*!< Timer1 Interrupt                       */
  Timer2_IRQn             =  15,  /*!< Timer2 Interrupt                       */
  Timer3_IRQn             =  16,  /*!< Timer3 Interrupt                       */
  ADC_IRQn                =  17,  /*!< ADC Interrupt                          */
  ETH_IRQn                =  18,  /*!< Ethernet Interrupt                     */
  SSP3_IRQn               =  19,  /*!< SSP3 Interrupt                         */
  SSP2_IRQn               =  20,  /*!< SSP2 Interrupt                         */
  ARINC429T1_IRQn         =  21,  /*!< ARINC429T1 Transmitter Interrupt       */
  ARINC429T2_IRQn         =  22,  /*!< ARINC429T2 Transmitter Interrupt       */
  ARINC429T3_IRQn         =  23,  /*!< ARINC429T3 Transmitter Interrupt       */
  ARINC429T4_IRQn         =  24,  /*!< ARINC429T4 Transmitter Interrupt       */
  BACKUP_IRQn             =  27,  /*!< BKP/RTC Interrupt                      */
  EXT_INT1_IRQn           =  28,  /*!< PORTA[0] Alternative mode Interrupt    */
  EXT_INT2_IRQn           =  29,  /*!< PORTB[10] Alternative mode Interrupt   */
  EXT_INT3_IRQn           =  30,  /*!< PORTE[15] Alternative mode Interrupt   */
  EXT_INT4_IRQn           =  31   /*!< PORTC[13] Alternative mode Interrupt   */
}IRQn_Type;

/* Configuration of the Cortex-M1 Processor and Core Peripherals */
#define __CM1_REV                 0x0001  /*!< Cortex-M1 Core Revision r1p0                 */
#define __MPU_PRESENT             0       /*!< MPU present or not                           */
#define __NVIC_PRIO_BITS          2       /*!< Number of Bits used for Priority Levels      */
#define __Vendor_SysTickConfig    0       /*!< Set to 1 if different SysTick Config is used */

/* Includes ------------------------------------------------------------------*/
#include "core_cm0.h"

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/* _can_buf structure */
typedef struct
{
  __IO uint32_t ID;
  __IO uint32_t DLC;
  __IO uint32_t DATAL;
  __IO uint32_t DATAH;
} _can_buf;

/* _can_buf_filter structure */
typedef struct
{
  __IO uint32_t MASK;
  __IO uint32_t FILTER;
} _can_buf_filter;

/* MDR_CAN_TypeDef structure */
typedef struct
{
  __IO uint32_t CONTROL;
  __IO uint32_t STATUS;
  __IO uint32_t BITTMNG;
       uint32_t RESERVED0;
  __IO uint32_t INT_EN;
       uint32_t RESERVED1[2];
  __IO uint32_t OVER;
  __IO uint32_t RXID;
  __IO uint32_t RXDLC;
  __IO uint32_t RXDATAL;
  __IO uint32_t RXDATAH;
  __IO uint32_t TXID;
  __IO uint32_t TXDLC;
  __IO uint32_t DATAL;
  __IO uint32_t DATAH;
  __IO uint32_t BUF_CON[32];
  __IO uint32_t INT_RX;
  __IO uint32_t RX;
  __IO uint32_t INT_TX;
  __IO uint32_t TX;
       uint32_t RESERVED2[76];
    _can_buf CAN_BUF[32];
       uint32_t RESERVED3[64];
	_can_buf_filter CAN_BUF_FILTER[32];
} _can;

/*-- CAN_CONTROL: CAN Control Register -----------------------------*/
/* Bit field positions: */
#define CAN_CONTROL_CAN_EN_Pos                  0
#define CAN_CONTROL_ROM_Pos                     1
#define CAN_CONTROL_STM_Pos                     2
#define CAN_CONTROL_SAP_Pos                     3
#define CAN_CONTROL_ROP_Pos                     4
/* Bit field masks: */
#define CAN_CONTROL_CAN_EN                      ((uint32_t)0x00000001)
#define CAN_CONTROL_ROM                         ((uint32_t)0x00000002)
#define CAN_CONTROL_STM                         ((uint32_t)0x00000004)
#define CAN_CONTROL_SAP                         ((uint32_t)0x00000008)
#define CAN_CONTROL_ROP                         ((uint32_t)0x00000010)

/*-- CAN_STATUS: CAN Status Register -------------------------------*/
/* Bit field positions: */
#define CAN_STATUS_RX_READY_Pos                 0
#define CAN_STATUS_TX_READY_Pos                 1
#define CAN_STATUS_ERROR_OVER_Pos               2
#define CAN_STATUS_BIT_ERR_Pos                  3
#define CAN_STATUS_BIT_STUFF_ERR_Pos            4
#define CAN_STATUS_CRC_ERR_Pos                  5
#define CAN_STATUS_FRAME_ERR_Pos                6
#define CAN_STATUS_ACK_ERR_Pos                  7
#define CAN_STATUS_IDLOWER_Pos                  8
#define CAN_STATUS_ERR_STATUS_Pos               9
#define CAN_STATUS_RX_ERR_CNT8_Pos              11
#define CAN_STATUS_TX_ERR_CNT8_Pos              12
#define CAN_STATUS_RX_ERR_CNT_Pos               16
#define CAN_STATUS_TX_ERR_CNT_Pos               24
/* Bit field masks: */
#define CAN_STATUS_RX_READY                     ((uint32_t)0x00000001)
#define CAN_STATUS_TX_READY                     ((uint32_t)0x00000002)
#define CAN_STATUS_ERROR_OVER                   ((uint32_t)0x00000004)
#define CAN_STATUS_BIT_ERR                      ((uint32_t)0x00000008)
#define CAN_STATUS_BIT_STUFF_ERR                ((uint32_t)0x00000010)
#define CAN_STATUS_CRC_ERR                      ((uint32_t)0x00000020)
#define CAN_STATUS_FRAME_ERR                    ((uint32_t)0x00000040)
#define CAN_STATUS_ACK_ERR                      ((uint32_t)0x00000080)
#define CAN_STATUS_IDLOWER                      ((uint32_t)0x00000100)
#define CAN_STATUS_ERR_STATUS_Msk               ((uint32_t)0x00000600)
#define CAN_STATUS_RX_ERR_CNT8                  ((uint32_t)0x00000800)
#define CAN_STATUS_TX_ERR_CNT8                  ((uint32_t)0x00001000)
#define CAN_STATUS_RX_ERR_CNT_Msk               ((uint32_t)0x00FF0000)
#define CAN_STATUS_TX_ERR_CNT_Msk               ((uint32_t)0xFF000000)

/*-- CAN_BITTMNG: CAN Bittiming Register ---------------------------*/
/* Bit field positions: */
#define CAN_BITTMNG_BRP_Pos                     0
#define CAN_BITTMNG_PSEG_Pos                    16
#define CAN_BITTMNG_SEG1_Pos                    19
#define CAN_BITTMNG_SEG2_Pos                    22
#define CAN_BITTMNG_SJW_Pos                     25
#define CAN_BITTMNG_SB_Pos                      27
/* Bit field masks: */
#define CAN_BITTMNG_BRP_Msk                     ((uint32_t)0x0000FFFF)
#define CAN_BITTMNG_PSEG_Msk                    ((uint32_t)0x00070000)
#define CAN_BITTMNG_SEG1_Msk                    ((uint32_t)0x00380000)
#define CAN_BITTMNG_SEG2_Msk                    ((uint32_t)0x01C00000)
#define CAN_BITTMNG_SJW_Msk                     ((uint32_t)0x06000000)
#define CAN_BITTMNG_SB                          ((uint32_t)0x08000000)

/*-- CAN_INT_EN: CAN Interrupt enable Register ---------------------*/
/* Bit field positions: */
#define CAN_INT_EN_GLB_INT_EN_Pos               0
#define CAN_INT_EN_RX_INT_EN_Pos                1
#define CAN_INT_EN_TX_INT_EN_Pos                2
#define CAN_INT_EN_ERR_INT_EN_Pos               3
#define CAN_INT_EN_ERR_OVER_INT_EN_Pos          4
/* Bit field masks: */
#define CAN_INT_EN_GLB_INT_EN                   ((uint32_t)0x00000001)
#define CAN_INT_EN_RX_INT_EN                    ((uint32_t)0x00000002)
#define CAN_INT_EN_TX_INT_EN                    ((uint32_t)0x00000004)
#define CAN_INT_EN_ERR_INT_EN                   ((uint32_t)0x00000008)
#define CAN_INT_EN_ERR_OVER_INT_EN              ((uint32_t)0x00000010)

/*-- CAN_RXID:   CAN Receive ID Register ---------------------------*/
/*-- CAN_TXID:   CAN Transmit ID Register --------------------------*/
/*-- CAN_BUF_ID: CAN Buffer ID Register ----------------------------*/
/* Bit field positions: */
#define CAN_ID_EID_Pos                          0
#define CAN_ID_SID_Pos                          18
/* Bit field masks: */
#define CAN_ID_EID_Msk                          ((uint32_t)0x0003FFFF)
#define CAN_ID_SID_Msk                          ((uint32_t)0x1FFC0000)

/*-- CAN_RXDLC:   CAN Receive DLC Register -------------------------*/
/*-- CAN_TXDLC:   CAN Transmit DLC Register ------------------------*/
/*-- CAN_BUF_DLC: CAN Buffer DLC Register --------------------------*/
/* Bit field positions: */
#define CAN_DLC_Pos                             0
#define CAN_DLC_RTR_Pos                         8
#define CAN_DLC_R1_Pos                          9
#define CAN_DLC_R0_Pos                          10
#define CAN_DLC_SSR_Pos                         11
#define CAN_DLC_IDE_Pos                         12
/* Bit field masks: */
#define CAN_DLC_Msk                             ((uint32_t)0x0000000F)
#define CAN_DLC_RTR                             ((uint32_t)0x00000100)
#define CAN_DLC_R1                              ((uint32_t)0x00000200)
#define CAN_DLC_R0                              ((uint32_t)0x00000400)
#define CAN_DLC_SSR                             ((uint32_t)0x00000800)
#define CAN_DLC_IDE                             ((uint32_t)0x00001000)

/*-- CAN_RXDATAL:   CAN Receive Data low Register ------------------*/
/*-- CAN_DATAL:     CAN Transmit Data low Register -----------------*/
/*-- CAN_BUF_DATAL: CAN Buffer Data low Register -------------------*/
/* Bit field positions: */
#define CAN_DATAL_DB0_Pos                       0
#define CAN_DATAL_DB1_Pos                       8
#define CAN_DATAL_DB2_Pos                       16
#define CAN_DATAL_DB3_Pos                       24
/* Bit field masks: */
#define CAN_DATAL_DB0_Msk                       ((uint32_t)0x000000FF)
#define CAN_DATAL_DB1_Msk                       ((uint32_t)0x0000FF00)
#define CAN_DATAL_DB2_Msk                       ((uint32_t)0x00FF0000)
#define CAN_DATAL_DB3_Msk                       ((uint32_t)0xFF000000)

/*-- CAN_RXDATAH:   CAN Receive Data high Register -----------------*/
/*-- CAN_DATAH:     CAN Transmit Data high Register ----------------*/
/*-- CAN_BUF_DATAH: CAN Buffer Data high Register ------------------*/
/* Bit field positions: */
#define CAN_DATAH_DB4_Pos                       0
#define CAN_DATAH_DB5_Pos                       8
#define CAN_DATAH_DB6_Pos                       16
#define CAN_DATAH_DB7_Pos                       24
/* Bit field masks: */
#define CAN_DATAH_DB4_Msk                       ((uint32_t)0x000000FF)
#define CAN_DATAH_DB5_Msk                       ((uint32_t)0x0000FF00)
#define CAN_DATAH_DB6_Msk                       ((uint32_t)0x00FF0000)
#define CAN_DATAH_DB7_Msk                       ((uint32_t)0xFF000000)

/*-- CAN_BUF_CON: CAN Buffer Connection Register -------------------*/
/* Bit field positions: */
#define CAN_BUF_CON_EN_Pos                      0
#define CAN_BUF_CON_RX_TXN_Pos                  1
#define CAN_BUF_CON_OVER_EN_Pos                 2
#define CAN_BUF_CON_RTR_EN_Pos                  3
#define CAN_BUF_CON_PRIOR_0_Pos                 4
#define CAN_BUF_CON_TX_REQ_Pos                  5
#define CAN_BUF_CON_RX_FULL_Pos                 6
#define CAN_BUF_CON_OVER_WR_Pos                 7
/* Bit field masks: */
#define CAN_BUF_CON_EN                          ((uint32_t)0x00000001)
#define CAN_BUF_CON_RX_TXN                      ((uint32_t)0x00000002)
#define CAN_BUF_CON_OVER_EN                     ((uint32_t)0x00000004)
#define CAN_BUF_CON_RTR_EN                      ((uint32_t)0x00000008)
#define CAN_BUF_CON_PRIOR_0                     ((uint32_t)0x00000010)
#define CAN_BUF_CON_TX_REQ                      ((uint32_t)0x00000020)
#define CAN_BUF_CON_RX_FULL                     ((uint32_t)0x00000040)
#define CAN_BUF_CON_OVER_WR                     ((uint32_t)0x00000080)


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

/*-- USB_HTXC: USB HTXC Register -----------------------------------*/
/* Bit field positions: */
#define USB_HTXC_TREQ_Pos                       0
#define USB_HTXC_SOFS_Pos                       1
#define USB_HTXC_PREEN_Pos                      2
#define USB_HTXC_ISOEN_Pos                      3
/* Bit field masks: */
#define USB_HTXC_TREQ                           ((uint32_t)0x00000001)
#define USB_HTXC_SOFS                           ((uint32_t)0x00000002)
#define USB_HTXC_PREEN                          ((uint32_t)0x00000004)
#define USB_HTXC_ISOEN                          ((uint32_t)0x00000008)

/*-- USB_HTXLC: USB HTXLC Register ---------------------------------*/
/* Bit field positions: */
#define USB_HTXLC_Pos                           0
#define USB_HTXLC_DC_Pos                        2
#define USB_HTXLC_FSPL_Pos                      3
#define USB_HTXLC_FSLR_Pos                      4
/* Bit field masks: */
#define USB_HTXLC_Msk                           ((uint32_t)0x00000003)
#define USB_HTXLC_DC                            ((uint32_t)0x00000004)
#define USB_HTXLC_FSPL                          ((uint32_t)0x00000008)
#define USB_HTXLC_FSLR                          ((uint32_t)0x00000010)

/*-- USB_HIS: USB_HIS Register -------------------------------------*/
/* Bit field positions: */
#define USB_HIS_TDONE_Pos                       0
#define USB_HIS_RESUME_Pos                      1
#define USB_HIS_CONEV_Pos                       2
#define USB_HIS_SOFS_Pos                        3
/* Bit field masks: */
#define USB_HIS_TDONE                           ((uint32_t)0x00000001)
#define USB_HIS_RESUME                          ((uint32_t)0x00000002)
#define USB_HIS_CONEV                           ((uint32_t)0x00000004)
#define USB_HIS_SOFS                            ((uint32_t)0x00000008)

/*-- USB_HIM: USB_HIM Register -------------------------------------*/
/* Bit field positions: */
#define USB_HIM_TDONEIE_Pos                     0
#define USB_HIM_RESUMEIE_Pos                    1
#define USB_HIM_CONEVIE_Pos                     2
#define USB_HIM_SOFIE_Pos                       3
/* Bit field masks: */
#define USB_HIM_TDONEIE                         ((uint32_t)0x00000001)
#define USB_HIM_RESUMEIE                        ((uint32_t)0x00000002)
#define USB_HIM_CONEVIE                         ((uint32_t)0x00000004)
#define USB_HIM_SOFIE                           ((uint32_t)0x00000008)

/*-- USB_HRXS: USB_HRXS Register -----------------------------------*/
/* Bit field positions: */
#define USB_HRXS_CRCERR_Pos                     0
#define USB_HRXS_BSERR_Pos                      1
#define USB_HRXS_RXOF_Pos                       2
#define USB_HRXS_RXTO_Pos                       3
#define USB_HRXS_NAKRXED_Pos                    4
#define USB_HRXS_STALLRXED_Pos                  5
#define USB_HRXS_ACKRXED_Pos                    6
#define USB_HRXS_DATASEQ_Pos                    7
/* Bit field masks: */
#define USB_HRXS_CRCERR                         ((uint32_t)0x00000001)
#define USB_HRXS_BSERR                          ((uint32_t)0x00000002)
#define USB_HRXS_RXOF                           ((uint32_t)0x00000004)
#define USB_HRXS_RXTO                           ((uint32_t)0x00000008)
#define USB_HRXS_NAKRXED                        ((uint32_t)0x00000010)
#define USB_HRXS_STALLRXED                      ((uint32_t)0x00000020)
#define USB_HRXS_ACKRXED                        ((uint32_t)0x00000040)
#define USB_HRXS_DATASEQ                        ((uint32_t)0x00000080)

/*-- USB_SEP_CTRL: USB_SEP Control Register ------------------------*/
/* Bit field positions: */
#define USB_SEP_CTRL_EPEN_Pos                   0
#define USB_SEP_CTRL_EPRDY_Pos                  1
#define USB_SEP_CTRL_EPDATASEQ_Pos              2
#define USB_SEP_CTRL_EPSSTALL_Pos               3
#define USB_SEP_CTRL_EPISOEN_Pos                4
/* Bit field masks: */
#define USB_SEP_CTRL_EPEN                       ((uint32_t)0x00000001)
#define USB_SEP_CTRL_EPRDY                      ((uint32_t)0x00000002)
#define USB_SEP_CTRL_EPDATASEQ                  ((uint32_t)0x00000004)
#define USB_SEP_CTRL_EPSSTALL                   ((uint32_t)0x00000008)
#define USB_SEP_CTRL_EPISOEN                    ((uint32_t)0x00000010)

/*-- USB_SEP_STS: USB_SEP Status Register --------------------------*/
/* Bit field positions: */
#define USB_SEP_STS_SCCRCERR_Pos                0
#define USB_SEP_STS_SCBSERR_Pos                 1
#define USB_SEP_STS_SCRXOF_Pos                  2
#define USB_SEP_STS_SCRXTO_Pos                  3
#define USB_SEP_STS_SCNAKSENT_Pos               4
#define USB_SEP_STS_SCSTALLSENT_Pos             5
#define USB_SEP_STS_SCACKRXED_Pos               6
#define USB_SEP_STS_SCDATASEQ_Pos               7
/* Bit field masks: */
#define USB_SEP_STS_SCCRCERR                    ((uint32_t)0x00000001)
#define USB_SEP_STS_SCBSERR                     ((uint32_t)0x00000002)
#define USB_SEP_STS_SCRXOF                      ((uint32_t)0x00000004)
#define USB_SEP_STS_SCRXTO                      ((uint32_t)0x00000008)
#define USB_SEP_STS_SCNAKSENT                   ((uint32_t)0x00000010)
#define USB_SEP_STS_SCSTALLSENT                 ((uint32_t)0x00000020)
#define USB_SEP_STS_SCACKRXED                   ((uint32_t)0x00000040)
#define USB_SEP_STS_SCDATASEQ                   ((uint32_t)0x00000080)

/*-- USB_SC: USB_SC Register ---------------------------------------*/
/* Bit field positions: */
#define USB_SCGEN_Pos                           0
#define USB_SCTXLS_Pos                          1
#define USB_SCDC_Pos                            3
#define USB_SCFSP_Pos                           4
#define USB_SCFSR_Pos                           5
/* Bit field masks: */
#define USB_SCGEN                               ((uint32_t)0x00000001)
#define USB_SCTXLS_Msk                          ((uint32_t)0x00000006)
#define USB_SCDC                                ((uint32_t)0x00000008)
#define USB_SCFSP                               ((uint32_t)0x00000010)
#define USB_SCFSR                               ((uint32_t)0x00000020)

/*-- USB_SIS: USB_SIS Register -------------------------------------*/
/* Bit field positions: */
#define USB_SIS_SCTDONE_Pos                     0
#define USB_SIS_SCRESUME_Pos                    1
#define USB_SIS_SCRESETEV_Pos                   2
#define USB_SIS_SCSOFREC_Pos                    3
#define USB_SIS_SCNAKSENT_Pos                   4
/* Bit field masks: */
#define USB_SIS_SCTDONE                         ((uint32_t)0x00000001)
#define USB_SIS_SCRESUME                        ((uint32_t)0x00000002)
#define USB_SIS_SCRESETEV                       ((uint32_t)0x00000004)
#define USB_SIS_SCSOFREC                        ((uint32_t)0x00000008)
#define USB_SIS_SCNAKSENT                       ((uint32_t)0x00000010)

/*-- USB_SIM: USB_SIM Register -------------------------------------*/
/* Bit field positions: */
#define USB_SIM_SCTDONEIE_Pos                   0
#define USB_SIM_SCRESUMEIE_Pos                  1
#define USB_SIM_SCRESETEVIE_Pos                 2
#define USB_SIM_SCSOFRECIE_Pos                  3
#define USB_SIM_SCNAKSENTIE_Pos                 4
/* Bit field masks: */
#define USB_SIM_SCTDONEIE                       ((uint32_t)0x00000001)
#define USB_SIM_SCRESUMEIE                      ((uint32_t)0x00000002)
#define USB_SIM_SCRESETEVIE                     ((uint32_t)0x00000004)
#define USB_SIM_SCSOFRECIE                      ((uint32_t)0x00000008)
#define USB_SIM_SCNAKSENTIE                     ((uint32_t)0x00000010)

/*-- USB_HSCR: USB_HSCR Register -----------------------------------*/
/* Bit field positions: */
#define USB_HSCR_HOST_MODE_Pos                  0
#define USB_HSCR_RESET_CORE_Pos                 1
#define USB_HSCR_EN_TX_Pos                      2
#define USB_HSCR_EN_RX_Pos                      3
#define USB_HSCR_DP_PULLUP_Pos                  4
#define USB_HSCR_DP_PULLDOWN_Pos                5
#define USB_HSCR_DM_PULLUP_Pos                  6
#define USB_HSCR_DM_PULLDOWN_Pos                7
/* Bit field masks: */
#define USB_HSCR_HOST_MODE                      ((uint32_t)0x00000001)
#define USB_HSCR_RESET_CORE                     ((uint32_t)0x00000002)
#define USB_HSCR_EN_TX                          ((uint32_t)0x00000004)
#define USB_HSCR_EN_RX                          ((uint32_t)0x00000008)
#define USB_HSCR_DP_PULLUP                      ((uint32_t)0x00000010)
#define USB_HSCR_DP_PULLDOWN                    ((uint32_t)0x00000020)
#define USB_HSCR_DM_PULLUP                      ((uint32_t)0x00000040)
#define USB_HSCR_DM_PULLDOWN                    ((uint32_t)0x00000080)

/*-- USB_HSVR: USB_HSVR Register -----------------------------------*/
/* Bit field positions: */
#define USB_HSVR_VERSION_Pos                    0
#define USB_HSVR_REVISION_Pos                   4
/* Bit field masks: */
#define USB_HSVR_VERSION_Msk                    ((uint32_t)0x0000000F)
#define USB_HSVR_REVISION_Msk                   ((uint32_t)0x000000F0)

/* _eeprom structure */
typedef struct
{
  __IO uint32_t CMD;
  __IO uint32_t ADR;
  __IO uint32_t DI;
  __IO uint32_t DO;
  __IO uint32_t KEY;
} _eeprom;

/*-- EEPROM_CMD: EEPROM Command Register ---------------------------*/
/* Bit field positions: */
#define EEPROM_CMD_CON_Pos                      0
#define EEPROM_CMD_WR_Pos                       1
#define EEPROM_CMD_RD_Pos                       2
#define EEPROM_CMD_DELAY_Pos                    3
#define EEPROM_CMD_XE_Pos                       6
#define EEPROM_CMD_YE_Pos                       7
#define EEPROM_CMD_SE_Pos                       8
#define EEPROM_CMD_IFREN_Pos                    9
#define EEPROM_CMD_ERASE_Pos                    10
#define EEPROM_CMD_MAS1_Pos                     11
#define EEPROM_CMD_PROG_Pos                     12
#define EEPROM_CMD_NVSTR_Pos                    13
/* Bit field masks: */
#define EEPROM_CMD_CON                          ((uint32_t)0x00000001)
#define EEPROM_CMD_WR                           ((uint32_t)0x00000002)
#define EEPROM_CMD_RD                           ((uint32_t)0x00000004)
#define EEPROM_CMD_DELAY_Msk                    ((uint32_t)0x00000038)
#define EEPROM_CMD_XE                           ((uint32_t)0x00000040)
#define EEPROM_CMD_YE                           ((uint32_t)0x00000080)
#define EEPROM_CMD_SE                           ((uint32_t)0x00000100)
#define EEPROM_CMD_IFREN                        ((uint32_t)0x00000200)
#define EEPROM_CMD_ERASE                        ((uint32_t)0x00000400)
#define EEPROM_CMD_MAS1                         ((uint32_t)0x00000800)
#define EEPROM_CMD_PROG                         ((uint32_t)0x00001000)
#define EEPROM_CMD_NVSTR                        ((uint32_t)0x00002000)

/* _rst_clk structure */
typedef struct
{
  __IO uint32_t CLOCK_STATUS;
  __IO uint32_t PLL_CONTROL;
  __IO uint32_t HS_CONTROL;
  __IO uint32_t CPU_CLOCK;
  __IO uint32_t USB_CLOCK;
  __IO uint32_t ADC_MCO_CLOCK;
  __IO uint32_t RTC_CLOCK;
  __IO uint32_t PER_CLOCK;
  __IO uint32_t CAN_CLOCK;
  __IO uint32_t TIM_CLOCK;
  __IO uint32_t UART_CLOCK;
  __IO uint32_t SSP_CLOCK;
       uint32_t RESERVED0;
  __IO uint32_t ETH_CLOCK;
} _rst_clk;

/*-- RST_CLK_CLOCK_STATUS: Clock Status Register -------------------*/
/* Bit field positions: */
#define RST_CLK_CLOCK_STATUS_PLL_USB_RDY_Pos    0
#define RST_CLK_CLOCK_STATUS_PLL_CPU_RDY_Pos    1
#define RST_CLK_CLOCK_STATUS_HSE_RDY_Pos        2
/* Bit field masks: */
#define RST_CLK_CLOCK_STATUS_PLL_USB_RDY        ((uint32_t)0x00000001)
#define RST_CLK_CLOCK_STATUS_PLL_CPU_RDY        ((uint32_t)0x00000002)
#define RST_CLK_CLOCK_STATUS_HSE_RDY            ((uint32_t)0x00000004)

/*-- RST_CLK_PLL_CONTROL: PLL Control Register ---------------------*/
/* Bit field positions: */
#define RST_CLK_PLL_CONTROL_PLL_USB_ON_Pos      0
#define RST_CLK_PLL_CONTROL_PLL_USB_RLD_Pos     1
#define RST_CLK_PLL_CONTROL_PLL_CPU_ON_Pos      2
#define RST_CLK_PLL_CONTROL_PLL_CPU_PLD_Pos     3
#define RST_CLK_PLL_CONTROL_PLL_USB_MUL_Pos     4
#define RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos     8
/* Bit field masks: */
#define RST_CLK_PLL_CONTROL_PLL_USB_ON          ((uint32_t)0x00000001)
#define RST_CLK_PLL_CONTROL_PLL_USB_RLD         ((uint32_t)0x00000002)
#define RST_CLK_PLL_CONTROL_PLL_CPU_ON          ((uint32_t)0x00000004)
#define RST_CLK_PLL_CONTROL_PLL_CPU_PLD         ((uint32_t)0x00000008)
#define RST_CLK_PLL_CONTROL_PLL_USB_MUL_Msk     ((uint32_t)0x000000F0)
#define RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Msk     ((uint32_t)0x00000F00)

/*-- RST_CLK_HS_CONTROL: HS Control Register -----------------------*/
/* Bit field positions: */
#define RST_CLK_HS_CONTROL_HSE_ON_Pos           0
#define RST_CLK_HS_CONTROL_HSE_BYP_Pos          1
/* Bit field masks: */
#define RST_CLK_HS_CONTROL_HSE_ON               ((uint32_t)0x00000001)
#define RST_CLK_HS_CONTROL_HSE_BYP              ((uint32_t)0x00000002)

/*-- RST_CLK_CPU_CLOCK: CPU Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos        0
#define RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos        2
#define RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos        4
#define RST_CLK_CPU_CLOCK_HCLK_SEL_Pos          8
/* Bit field masks: */
#define RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk        ((uint32_t)0x00000003)
#define RST_CLK_CPU_CLOCK_CPU_C2_SEL            ((uint32_t)0x00000004)
#define RST_CLK_CPU_CLOCK_CPU_C3_SEL_Msk        ((uint32_t)0x000000F0)
#define RST_CLK_CPU_CLOCK_HCLK_SEL_Msk          ((uint32_t)0x00000300)

/*-- RST_CLK_USB_CLOCK: USB Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_USB_CLOCK_USB_C1_SEL_Pos        0
#define RST_CLK_USB_CLOCK_USB_C2_SEL_Pos        2
#define RST_CLK_USB_CLOCK_CPU_C3_SEL_Pos        4
#define RST_CLK_USB_CLOCK_USB_CL_KEN_Pos        8
/* Bit field masks: */
#define RST_CLK_USB_CLOCK_USB_C1_SEL_Msk        ((uint32_t)0x00000003)
#define RST_CLK_USB_CLOCK_USB_C2_SEL            ((uint32_t)0x00000004)
#define RST_CLK_USB_CLOCK_CPU_C3_SEL_Msk        ((uint32_t)0x000000F0)
#define RST_CLK_USB_CLOCK_USB_CL_KEN            ((uint32_t)0x00000100)

/*-- RST_CLK_ADC_MCO_CLOCK: ADC Clock Register ---------------------*/
/* Bit field positions: */
#define RST_CLK_ADC_MCO_CLOCK_ADC_C1_SEL_Pos    0
#define RST_CLK_ADC_MCO_CLOCK_ADC_C2_SEL_Pos    4
#define RST_CLK_ADC_MCO_CLOCK_ADC_C3_SEL_Pos    8
#define RST_CLK_ADC_MCO_CLOCK_MCO_EN_Pos        12
#define RST_CLK_ADC_MCO_CLOCK_ADC_CLK_EN_Pos    13
/* Bit field masks: */
#define RST_CLK_ADC_MCO_CLOCK_ADC_C1_SEL_Msk    ((uint32_t)0x00000003)
#define RST_CLK_ADC_MCO_CLOCK_ADC_C2_SEL_Msk    ((uint32_t)0x00000030)
#define RST_CLK_ADC_MCO_CLOCK_ADC_C3_SEL_Msk    ((uint32_t)0x00000F00)
#define RST_CLK_ADC_MCO_CLOCK_MCO_EN            ((uint32_t)0x00001000)
#define RST_CLK_ADC_MCO_CLOCK_ADC_CLK_EN        ((uint32_t)0x00002000)

/*-- RST_CLK_RTC_CLOCK: RTC Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_RTC_CLOCK_HSE_SEL_Pos           0
#define RST_CLK_RTC_CLOCK_HSI_SEL_Pos           4
#define RST_CLK_RTC_CLOCK_HSE_RTC_EN_Pos        8
#define RST_CLK_RTC_CLOCK_HSI_RTC_EN_Pos        9
/* Bit field masks: */
#define RST_CLK_RTC_CLOCK_HSE_SEL_Msk           ((uint32_t)0x0000000F)
#define RST_CLK_RTC_CLOCK_HSI_SEL_Msk           ((uint32_t)0x000000F0)
#define RST_CLK_RTC_CLOCK_HSE_RTC_EN            ((uint32_t)0x00000100)
#define RST_CLK_RTC_CLOCK_HSI_RTC_EN            ((uint32_t)0x00000200)

/*-- RST_CLK_PER_CLOCK: Peripheral Clock Enable Register -----------*/
/* Bit field positions: */
#define RST_CLK_PER_CLOCK_CAN1_Pos              0
#define RST_CLK_PER_CLOCK_CAN2_Pos              1
#define RST_CLK_PER_CLOCK_USB_Pos               2
#define RST_CLK_PER_CLOCK_EEPROM_Pos            3
#define RST_CLK_PER_CLOCK_RST_CLK_Pos           4
#define RST_CLK_PER_CLOCK_DMA_Pos               5
#define RST_CLK_PER_CLOCK_UART1_Pos             6
#define RST_CLK_PER_CLOCK_UART2_Pos             7
#define RST_CLK_PER_CLOCK_SSP1_Pos              8
#define RST_CLK_PER_CLOCK_MIL_STD_1553B1_Pos    9
#define RST_CLK_PER_CLOCK_MIL_STD_1553B2_Pos    10
#define RST_CLK_PER_CLOCK_POWER_Pos             11
#define RST_CLK_PER_CLOCK_WWDT_Pos              12
#define RST_CLK_PER_CLOCK_IWDT_Pos              13
#define RST_CLK_PER_CLOCK_TIMER1_Pos            14
#define RST_CLK_PER_CLOCK_TIMER2_Pos            15
#define RST_CLK_PER_CLOCK_TIMER3_Pos            16
#define RST_CLK_PER_CLOCK_ADC_Pos               17
#define RST_CLK_PER_CLOCK_DAC_Pos               18
#define RST_CLK_PER_CLOCK_TIMER4_Pos            19
#define RST_CLK_PER_CLOCK_SSP2_Pos              20
#define RST_CLK_PER_CLOCK_PORTA_Pos             21
#define RST_CLK_PER_CLOCK_PORTB_Pos             22
#define RST_CLK_PER_CLOCK_PORTC_Pos             23
#define RST_CLK_PER_CLOCK_PORTD_Pos             24
#define RST_CLK_PER_CLOCK_PORTE_Pos             25
#define RST_CLK_PER_CLOCK_ARINC429R_Pos         26
#define RST_CLK_PER_CLOCK_BKP_Pos               27
#define RST_CLK_PER_CLOCK_ARINC429T_Pos         28
#define RST_CLK_PER_CLOCK_PORTF_Pos             29
#define RST_CLK_PER_CLOCK_EBC_Pos               30
#define RST_CLK_PER_CLOCK_SSP3_Pos              31
/* Bit field masks: */
#define RST_CLK_PER_CLOCK_CAN1                  ((uint32_t)0x00000001)
#define RST_CLK_PER_CLOCK_CAN2                  ((uint32_t)0x00000002)
#define RST_CLK_PER_CLOCK_USB                   ((uint32_t)0x00000004)
#define RST_CLK_PER_CLOCK_EEPROM                ((uint32_t)0x00000008)
#define RST_CLK_PER_CLOCK_RST_CLK               ((uint32_t)0x00000010)
#define RST_CLK_PER_CLOCK_DMA                   ((uint32_t)0x00000020)
#define RST_CLK_PER_CLOCK_UART1                 ((uint32_t)0x00000040)
#define RST_CLK_PER_CLOCK_UART2                 ((uint32_t)0x00000080)
#define RST_CLK_PER_CLOCK_SSP1                  ((uint32_t)0x00000100)
#define RST_CLK_PER_CLOCK_MIL_STD_1553B1        ((uint32_t)0x00000200)
#define RST_CLK_PER_CLOCK_MIL_STD_1553B2        ((uint32_t)0x00000400)
#define RST_CLK_PER_CLOCK_POWER                 ((uint32_t)0x00000800)
#define RST_CLK_PER_CLOCK_WWDT                  ((uint32_t)0x00001000)
#define RST_CLK_PER_CLOCK_IWDT                  ((uint32_t)0x00002000)
#define RST_CLK_PER_CLOCK_TIMER1                ((uint32_t)0x00004000)
#define RST_CLK_PER_CLOCK_TIMER2                ((uint32_t)0x00008000)
#define RST_CLK_PER_CLOCK_TIMER3                ((uint32_t)0x00010000)
#define RST_CLK_PER_CLOCK_ADC                   ((uint32_t)0x00020000)
#define RST_CLK_PER_CLOCK_DAC                   ((uint32_t)0x00040000)
#define RST_CLK_PER_CLOCK_TIMER4                ((uint32_t)0x00080000)
#define RST_CLK_PER_CLOCK_SSP2                  ((uint32_t)0x00100000)
#define RST_CLK_PER_CLOCK_PORTA                 ((uint32_t)0x00200000)
#define RST_CLK_PER_CLOCK_PORTB                 ((uint32_t)0x00400000)
#define RST_CLK_PER_CLOCK_PORTC                 ((uint32_t)0x00800000)
#define RST_CLK_PER_CLOCK_PORTD                 ((uint32_t)0x01000000)
#define RST_CLK_PER_CLOCK_PORTE                 ((uint32_t)0x02000000)
#define RST_CLK_PER_CLOCK_ARINC429R             ((uint32_t)0x04000000)
#define RST_CLK_PER_CLOCK_BKP                   ((uint32_t)0x08000000)
#define RST_CLK_PER_CLOCK_ARINC429T             ((uint32_t)0x10000000)
#define RST_CLK_PER_CLOCK_PORTF                 ((uint32_t)0x20000000)
#define RST_CLK_PER_CLOCK_EBC                   ((uint32_t)0x40000000)
#define RST_CLK_PER_CLOCK_SSP3                  ((uint32_t)0x80000000)

/*-- RST_CLK_CAN_CLOCK: CAN Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_CAN_CLOCK_CAN1_BRG_Pos          0
#define RST_CLK_CAN_CLOCK_CAN2_BRG_Pos          8
#define RST_CLK_CAN_CLOCK_CAN1_CLK_EN_Pos       24
#define RST_CLK_CAN_CLOCK_CAN2_CLK_EN_Pos       25
/* Bit field masks: */
#define RST_CLK_CAN_CLOCK_CAN1_BRG_Msk          ((uint32_t)0x000000FF)
#define RST_CLK_CAN_CLOCK_CAN2_BRG_Msk          ((uint32_t)0x0000FF00)
#define RST_CLK_CAN_CLOCK_CAN1_CLK_EN           ((uint32_t)0x01000000)
#define RST_CLK_CAN_CLOCK_CAN2_CLK_EN           ((uint32_t)0x02000000)

/*-- RST_CLK_TIM_CLOCK: Timer Clock Register -----------------------*/
/* Bit field positions: */
#define RST_CLK_TIM_CLOCK_TIM1_BRG_Pos          0
#define RST_CLK_TIM_CLOCK_TIM2_BRG_Pos          8
#define RST_CLK_TIM_CLOCK_TIM3_BRG_Pos          16
#define RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos       24
#define RST_CLK_TIM_CLOCK_TIM2_CLK_EN_Pos       25
#define RST_CLK_TIM_CLOCK_TIM3_CLK_EN_Pos       26
/* Bit field masks: */
#define RST_CLK_TIM_CLOCK_TIM1_BRG_Msk          ((uint32_t)0x000000FF)
#define RST_CLK_TIM_CLOCK_TIM2_BRG_Msk          ((uint32_t)0x0000FF00)
#define RST_CLK_TIM_CLOCK_TIM3_BRG_Msk          ((uint32_t)0x00FF0000)
#define RST_CLK_TIM_CLOCK_TIM1_CLK_EN           ((uint32_t)0x01000000)
#define RST_CLK_TIM_CLOCK_TIM2_CLK_EN           ((uint32_t)0x02000000)
#define RST_CLK_TIM_CLOCK_TIM3_CLK_EN           ((uint32_t)0x04000000)

/*-- RST_CLK_UART_CLOCK: UART Clock Register -----------------------*/
/* Bit field positions: */
#define RST_CLK_UART_CLOCK_UART1_BRG_Pos        0
#define RST_CLK_UART_CLOCK_UART2_BRG_Pos        8
#define RST_CLK_UART_CLOCK_TIM4_BRG_Pos         16
#define RST_CLK_UART_CLOCK_UART1_CLK_EN_Pos     24
#define RST_CLK_UART_CLOCK_UART2_CLK_EN_Pos     25
#define RST_CLK_UART_CLOCK_TIM4_CLK_EN_Pos      26
/* Bit field masks: */
#define RST_CLK_UART_CLOCK_UART1_BRG_Msk        ((uint32_t)0x000000FF)
#define RST_CLK_UART_CLOCK_UART2_BRG_Msk        ((uint32_t)0x0000FF00)
#define RST_CLK_UART_CLOCK_TIM4_BRG_Msk         ((uint32_t)0x00FF0000)
#define RST_CLK_UART_CLOCK_UART1_CLK_EN         ((uint32_t)0x01000000)
#define RST_CLK_UART_CLOCK_UART2_CLK_EN         ((uint32_t)0x02000000)
#define RST_CLK_UART_CLOCK_TIM4_CLK_EN          ((uint32_t)0x04000000)

/*-- RST_CLK_SSP_CLOCK: SSP Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_SSP_CLOCK_SSP1_BRG_Pos          0
#define RST_CLK_SSP_CLOCK_SSP2_BRG_Pos          8
#define RST_CLK_SSP_CLOCK_SSP3_BRG_Pos          16
#define RST_CLK_SSP_CLOCK_SSP1_CLK_EN_Pos       24
#define RST_CLK_SSP_CLOCK_SSP2_CLK_EN_Pos       25
#define RST_CLK_SSP_CLOCK_SSP3_CLK_EN_Pos       26
/* Bit field masks: */
#define RST_CLK_SSP_CLOCK_SSP1_BRG_Msk          ((uint32_t)0x000000FF)
#define RST_CLK_SSP_CLOCK_SSP2_BRG_Msk          ((uint32_t)0x0000FF00)
#define RST_CLK_SSP_CLOCK_SSP3_BRG_Msk          ((uint32_t)0x00FF0000)
#define RST_CLK_SSP_CLOCK_SSP1_CLK_EN           ((uint32_t)0x01000000)
#define RST_CLK_SSP_CLOCK_SSP2_CLK_EN           ((uint32_t)0x02000000)
#define RST_CLK_SSP_CLOCK_SSP3_CLK_EN           ((uint32_t)0x04000000)

/*-- RST_CLK_ETH_CLOCK: ETH and MAN Clock Register -----------------*/
/* Bit field positions: */
#define RST_CLK_ETH_CLOCK_ETH_BRG_Pos           0
#define RST_CLK_ETH_CLOCK_MAN_BRG_Pos           8
#define RST_CLK_ETH_CLOCK_PHY_BRG_Pos           16
#define RST_CLK_ETH_CLOCK_ETH_CLK_EN_Pos        24
#define RST_CLK_ETH_CLOCK_MAN_CLK_EN_Pos        25
#define RST_CLK_ETH_CLOCK_SLEEP_Pos             26
#define RST_CLK_ETH_CLOCK_PHY_CLK_EN_Pos        27
#define RST_CLK_ETH_CLOCK_PHY_CLK_SEL_Pos       28
/* Bit field masks: */
#define RST_CLK_ETH_CLOCK_ETH_BRG_Msk           ((uint32_t)0x000000FF)
#define RST_CLK_ETH_CLOCK_MAN_BRG_Msk           ((uint32_t)0x0000FF00)
#define RST_CLK_ETH_CLOCK_PHY_BRG_Msk           ((uint32_t)0x00FF0000)
#define RST_CLK_ETH_CLOCK_ETH_CLK_EN            ((uint32_t)0x01000000)
#define RST_CLK_ETH_CLOCK_MAN_CLK_EN            ((uint32_t)0x02000000)
#define RST_CLK_ETH_CLOCK_SLEEP                 ((uint32_t)0x04000000)
#define RST_CLK_ETH_CLOCK_PHY_CLK_EN            ((uint32_t)0x08000000)
#define RST_CLK_ETH_CLOCK_PHY_CLK_SEL_Msk       ((uint32_t)0x30000000)

/* _dma structure */
typedef struct
{
	//RO 0x--nn0000   DMA status
	//[31...28] test_status:
	//          b0000 - MCU hasn't test scheme
	//          b0001 - MCU has test scheme
	//[20...16] chnls_minus1 (available channels minus 1):
	//          [b00000..b11111] -> [1..32] DMA channels
	//[7..4]    state:
	//			b0000 - standby;
	//			b0001 - read control data;
	//			b0010 - read end data source ptr;
	//			b0011 - read end data dest ptr;
	//			b0100 - read data from source;
	//			b0101 - write data to dest;
	//			b0110 - wait DMA request;
	//			b0111 - write control data;
	//			b1000 - stopped;
	//			b1001 - completed;
	//			b1010 - work with peripherals;
	//			[b1011..b1111] - undefined
	//[0]       master_enable:
	//			0 - disable work
	//			1 - enable work
  __IO uint32_t STATUS;

	//WO 0x--------	  DMA configuration
	//[7..5]    chnl_prot_ctrl (indication):
	//			HPROT[3] - access with cache;
	//			HPROT[2] - access with buffers;
	//			HPROT[1] - privil. access
	//[0]       master_enable:
	//			0 - disable work;
	//			1 - enable work
  __IO uint32_t CFG;

	//RW 0x00000000   Primary control data base address
	//[31..10]  ctrl_base_ptr: pointer to primary control data
  __IO uint32_t CTRL_BASE_PTR;

	//RO 0x000000nn   Alter control data base address
	//[31..0]   alt_ctrl_base_ptr: pointer to alter control data
  __IO uint32_t ALT_CTRL_BASE_PTR;

	//RO 0x00000000   Wait on request status
	//[31..0]   dma_waitonreq_status[32]: 1/0 - wait request yes/no
  __IO uint32_t WAITONREQ_STATUS;

	//WO 0x--------   Set software request
	//[31..0]   chnl_sw_request[32]: 1/0 - set channel request yes/no
  __IO uint32_t CHNL_SW_REQUEST;

	//RW 0x00000000   Set burst mode
	//[31..0]   chnl_useburst_set[32]: 1 - dma_sreq[] disable
  __IO uint32_t CHNL_USEBURST_SET;

	//WO 0x--------   Clear burst mode
	//[31..0]   chnl_useburst_clr[32]: 1 - dma_sreq[] enable
  __IO uint32_t CHNL_USEBURST_CLR;

	//RW 0x00000000   Set request mask
	//[31..0]   chnl_req_mask_set[32]: 1 - set mask dma_sreq[]/dma_req[]
  __IO uint32_t CHNL_REQ_MASK_SET;

	//WO 0x--------   Clear request mask
	//[31..0]   chnl_req_mask_clr[32]: 1 - clear mask dma_sreq[]/dma_req[]
  __IO uint32_t CHNL_REQ_MASK_CLR;

	//RW 0x00000000   Enable DMA channels
	//[31..0]   chnl_enable_set[32]: 1 - enable DMA channel
  __IO uint32_t CHNL_ENABLE_SET;

	//WO 0x--------   Disable DMA channels
	//[31..0]   chnl_enable_clr[32]: 1 - disable DMA channel
  __IO uint32_t CHNL_ENABLE_CLR;

	//RW 0x00000000   Set prim./alter. control data
	//[31..0]   chnl_pri_alt_set[32]: 1 - select alter. control data
  __IO uint32_t CHNL_PRI_ALT_SET;

	//WO 0x--------   Clear prim./alter. control data
	//[31..0]   chnl_pri_alt_clr[32]: 1 - select primary control data
  __IO uint32_t CHNL_PRI_ALT_CLR;

	//RW 0x00000000   Set channel priority
	//[31..0]   chnl_priority_set[32]: 1 - high priority
  __IO uint32_t CHNL_PRIORITY_SET;

	//WO 0x--------   Clear channel priority
	//[31..0]   chnl_priority_clr[32]: 1 - default priority
  __IO uint32_t CHNL_PRIORITY_CLR;

	//reserved
       uint32_t RESERVED0[3];

	//RW 0x00000000   Error flag/clear error flag
	//[0]       err_clr read: 1/0 error flag on/off; write: 1 - reset error flag
  __IO uint32_t ERR_CLR;
} _dma;

/*-- DMA_STATUS: DMA Status Register -------------------------------*/
/* Bit field positions: */
#define DMA_STATUS_MASTER_ENABLE_Pos            0
#define DMA_STATUS_STATE_Pos                    4
#define DMA_STATUS_CHNLS_MINUS1_Pos             16
#define DMA_STATUS_TEST_STATUS_Pos              28
/* Bit field masks: */
#define DMA_STATUS_MASTER_ENABLE                ((uint32_t)0x00000001)
#define DMA_STATUS_STATE_Msk                    ((uint32_t)0x000000F0)
#define DMA_STATUS_CHNLS_MINUS1_Msk             ((uint32_t)0x001F0000)
#define DMA_STATUS_TEST_STATUS_Msk              ((uint32_t)0xF0000000)

/*-- DMA_CFG: DMA Configuration Register ---------------------------*/
/* Bit field positions: */
#define DMA_CFG_MASTER_ENABLE_Pos               0
#define DMA_CFG_CHNL_PROT_CTRL_Pos              5
/* Bit field masks: */
#define DMA_CFG_MASTER_ENABLE                   ((uint32_t)0x00000001)
#define DMA_CFG_CHNL_PROT_CTRL_Msk              ((uint32_t)0x000000E0)

/*-- DMA_CTRL_BASE_PTR:  -------------------------------------------*/
/* Bit field positions: */
#define DMA_CTRL_BASE_PTR_CTRL_BASE_PTR_Pos     5
/* Bit field masks: */
#define DMA_CTRL_BASE_PTR_CTRL_BASE_PTR_Msk     ((uint32_t)0xFFFFFFE0)

/* _uart structure */
typedef struct
{
  __IO uint32_t DR;
  __IO uint32_t RSR_ECR;
       uint32_t RESERVED0[4];
  __IO uint32_t FR;
       uint32_t RESERVED1;
  __IO uint32_t ILPR;
  __IO uint32_t IBRD;
  __IO uint32_t FBRD;
  __IO uint32_t LCR_H;
  __IO uint32_t CR;
  __IO uint32_t IFLS;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
  __IO uint32_t DMACR;
} _uart;

/*-- UART_DR: UART Data Register -----------------------------------*/
/* Bit field positions: */
#define UART_DR_DATA_Pos                        0
#define UART_DR_FE_Pos                          8
#define UART_DR_PE_Pos                          9
#define UART_DR_BE_Pos                          10
#define UART_DR_OE_Pos                          11
/* Bit field masks: */
#define UART_DR_DATA_Msk                        ((uint32_t)0x000000FF)
#define UART_DR_FE                              ((uint32_t)0x00000100)
#define UART_DR_PE                              ((uint32_t)0x00000200)
#define UART_DR_BE                              ((uint32_t)0x00000400)
#define UART_DR_OE                              ((uint32_t)0x00000800)

/*-- UART_RSR_ECR: UART RSR Register -------------------------------*/
/* Bit field positions: */
#define UART_RSR_ECR_FE_Pos                     0
#define UART_RSR_ECR_PE_Pos                     1
#define UART_RSR_ECR_BE_Pos                     2
#define UART_RSR_ECR_OE_Pos                     3
/* Bit field masks: */
#define UART_RSR_ECR_FE                         ((uint32_t)0x00000001)
#define UART_RSR_ECR_PE                         ((uint32_t)0x00000002)
#define UART_RSR_ECR_BE                         ((uint32_t)0x00000004)
#define UART_RSR_ECR_OE                         ((uint32_t)0x00000008)

/*-- UART_FR: UART Flag Register -----------------------------------*/
/* Bit field positions: */
#define UART_FR_CTS_Pos                         0
#define UART_FR_DSR_Pos                         1
#define UART_FR_DCD_Pos                         2
#define UART_FR_BUSY_Pos                        3
#define UART_FR_RXFE_Pos                        4
#define UART_FR_TXFF_Pos                        5
#define UART_FR_RXFF_Pos                        6
#define UART_FR_TXFE_Pos                        7
#define UART_FR_RI_Pos                          8
/* Bit field masks: */
#define UART_FR_CTS                             ((uint32_t)0x00000001)
#define UART_FR_DSR                             ((uint32_t)0x00000002)
#define UART_FR_DCD                             ((uint32_t)0x00000004)
#define UART_FR_BUSY                            ((uint32_t)0x00000008)
#define UART_FR_RXFE                            ((uint32_t)0x00000010)
#define UART_FR_TXFF                            ((uint32_t)0x00000020)
#define UART_FR_RXFF                            ((uint32_t)0x00000040)
#define UART_FR_TXFE                            ((uint32_t)0x00000080)
#define UART_FR_RI                              ((uint32_t)0x00000100)

/*-- UART_LCR_H: UART LCR_H Register -------------------------------*/
/* Bit field positions: */
#define UART_LCR_H_BRK_Pos                      0
#define UART_LCR_H_PEN_Pos                      1
#define UART_LCR_H_EPS_Pos                      2
#define UART_LCR_H_STP2_Pos                     3
#define UART_LCR_H_FEN_Pos                      4
#define UART_LCR_H_WLEN_Pos                     5
#define UART_LCR_H_SPS_Pos                      7
/* Bit field masks: */
#define UART_LCR_H_BRK                          ((uint32_t)0x00000001)
#define UART_LCR_H_PEN                          ((uint32_t)0x00000002)
#define UART_LCR_H_EPS                          ((uint32_t)0x00000004)
#define UART_LCR_H_STP2                         ((uint32_t)0x00000008)
#define UART_LCR_H_FEN                          ((uint32_t)0x00000010)
#define UART_LCR_H_WLEN_Msk                     ((uint32_t)0x00000060)
#define UART_LCR_H_SPS                          ((uint32_t)0x00000080)

/*-- UART_CR: UART Command Register --------------------------------*/
/* Bit field positions: */
#define UART_CR_UARTEN_Pos                      0
#define UART_CR_SIREN_Pos                       1
#define UART_CR_SIRLP_Pos                       2
#define UART_CR_LBE_Pos                         7
#define UART_CR_TXE_Pos                         8
#define UART_CR_RXE_Pos                         9
#define UART_CR_DTR_Pos                         10
#define UART_CR_RTS_Pos                         11
#define UART_CR_OUT1_Pos                        12
#define UART_CR_OUT2_Pos                        13
#define UART_CR_RTSEN_Pos                       14
#define UART_CR_CTSEN_Pos                       15
/* Bit field masks: */
#define UART_CR_UARTEN                          ((uint32_t)0x00000001)
#define UART_CR_SIREN                           ((uint32_t)0x00000002)
#define UART_CR_SIRLP                           ((uint32_t)0x00000004)
#define UART_CR_LBE                             ((uint32_t)0x00000080)
#define UART_CR_TXE                             ((uint32_t)0x00000100)
#define UART_CR_RXE                             ((uint32_t)0x00000200)
#define UART_CR_DTR                             ((uint32_t)0x00000400)
#define UART_CR_RTS                             ((uint32_t)0x00000800)
#define UART_CR_OUT1                            ((uint32_t)0x00001000)
#define UART_CR_OUT2                            ((uint32_t)0x00002000)
#define UART_CR_RTSEN                           ((uint32_t)0x00004000)
#define UART_CR_CTSEN                           ((uint32_t)0x00008000)

/*-- UART_IFLS: UART IFLS Register ---------------------------------*/
/* Bit field positions: */
#define UART_IFLS_TXIFLSEL_Pos                  0
#define UART_IFLS_RXIFLSEL_Pos                  3
/* Bit field masks: */
#define UART_IFLS_TXIFLSEL_Msk                  ((uint32_t)0x00000007)
#define UART_IFLS_RXIFLSEL_Msk                  ((uint32_t)0x00000038)

/*-- UART_IMSC: UART Interrupt Mask Register -----------------------*/
/* Bit field positions: */
#define UART_IMSC_RIMIM_Pos                     0
#define UART_IMSC_CTSMIM_Pos                    1
#define UART_IMSC_DCDMIM_Pos                    2
#define UART_IMSC_DSRMIM_Pos                    3
#define UART_IMSC_RXIM_Pos                      4
#define UART_IMSC_TXIM_Pos                      5
#define UART_IMSC_RTIM_Pos                      6
#define UART_IMSC_FEIM_Pos                      7
#define UART_IMSC_PEIM_Pos                      8
#define UART_IMSC_BEIM_Pos                      9
#define UART_IMSC_OEIM_Pos                      10
/* Bit field masks: */
#define UART_IMSC_RIMIM                         ((uint32_t)0x00000001)
#define UART_IMSC_CTSMIM                        ((uint32_t)0x00000002)
#define UART_IMSC_DCDMIM                        ((uint32_t)0x00000004)
#define UART_IMSC_DSRMIM                        ((uint32_t)0x00000008)
#define UART_IMSC_RXIM                          ((uint32_t)0x00000010)
#define UART_IMSC_TXIM                          ((uint32_t)0x00000020)
#define UART_IMSC_RTIM                          ((uint32_t)0x00000040)
#define UART_IMSC_FEIM                          ((uint32_t)0x00000080)
#define UART_IMSC_PEIM                          ((uint32_t)0x00000100)
#define UART_IMSC_BEIM                          ((uint32_t)0x00000200)
#define UART_IMSC_OEIM                          ((uint32_t)0x00000400)

/*-- UART_RIS: UART Interrupt Pending Register ---------------------*/
/* Bit field positions: */
#define UART_RIS_RIRMIS_Pos                     0
#define UART_RIS_CTSRMIS_Pos                    1
#define UART_RIS_DCDRMIS_Pos                    2
#define UART_RIS_DSRRMIS_Pos                    3
#define UART_RIS_RXRIS_Pos                      4
#define UART_RIS_TXRIS_Pos                      5
#define UART_RIS_RTRIS_Pos                      6
#define UART_RIS_FERIS_Pos                      7
#define UART_RIS_PERIS_Pos                      8
#define UART_RIS_BERIS_Pos                      9
#define UART_RIS_OERIS_Pos                      10
/* Bit field masks: */
#define UART_RIS_RIRMIS                         ((uint32_t)0x00000001)
#define UART_RIS_CTSRMIS                        ((uint32_t)0x00000002)
#define UART_RIS_DCDRMIS                        ((uint32_t)0x00000004)
#define UART_RIS_DSRRMIS                        ((uint32_t)0x00000008)
#define UART_RIS_RXRIS                          ((uint32_t)0x00000010)
#define UART_RIS_TXRIS                          ((uint32_t)0x00000020)
#define UART_RIS_RTRIS                          ((uint32_t)0x00000040)
#define UART_RIS_FERIS                          ((uint32_t)0x00000080)
#define UART_RIS_PERIS                          ((uint32_t)0x00000100)
#define UART_RIS_BERIS                          ((uint32_t)0x00000200)
#define UART_RIS_OERIS                          ((uint32_t)0x00000400)

/*-- UART_MIS: UART Masked Interrupt Pending Register --------------*/
/* Bit field positions: */
#define UART_MIS_RIMMIS_Pos                     0
#define UART_MIS_CTSMMIS_Pos                    1
#define UART_MIS_DCDMMIS_Pos                    2
#define UART_MIS_DSRMMIS_Pos                    3
#define UART_MIS_RXMIS_Pos                      4
#define UART_MIS_TXMIS_Pos                      5
#define UART_MIS_RTMIS_Pos                      6
#define UART_MIS_FEMIS_Pos                      7
#define UART_MIS_PEMIS_Pos                      8
#define UART_MIS_BEMIS_Pos                      9
#define UART_MIS_OEMIS_Pos                      10
/* Bit field masks: */
#define UART_MIS_RIMMIS                         ((uint32_t)0x00000001)
#define UART_MIS_CTSMMIS                        ((uint32_t)0x00000002)
#define UART_MIS_DCDMMIS                        ((uint32_t)0x00000004)
#define UART_MIS_DSRMMIS                        ((uint32_t)0x00000008)
#define UART_MIS_RXMIS                          ((uint32_t)0x00000010)
#define UART_MIS_TXMIS                          ((uint32_t)0x00000020)
#define UART_MIS_RTMIS                          ((uint32_t)0x00000040)
#define UART_MIS_FEMIS                          ((uint32_t)0x00000080)
#define UART_MIS_PEMIS                          ((uint32_t)0x00000100)
#define UART_MIS_BEMIS                          ((uint32_t)0x00000200)
#define UART_MIS_OEMIS                          ((uint32_t)0x00000400)

/*-- UART_ICR: UART Interrupt Clear Register -----------------------*/
/* Bit field positions: */
#define UART_ICR_RIMIC_Pos                      0
#define UART_ICR_CTSMIC_Pos                     1
#define UART_ICR_DCDMIC_Pos                     2
#define UART_ICR_DSRMIC_Pos                     3
#define UART_ICR_RXIC_Pos                       4
#define UART_ICR_TXIC_Pos                       5
#define UART_ICR_RTIC_Pos                       6
#define UART_ICR_FEIC_Pos                       7
#define UART_ICR_PEIC_Pos                       8
#define UART_ICR_BEIC_Pos                       9
#define UART_ICR_OEIC_Pos                       10
/* Bit field masks: */
#define UART_ICR_RIMIC                          ((uint32_t)0x00000001)
#define UART_ICR_CTSMIC                         ((uint32_t)0x00000002)
#define UART_ICR_DCDMIC                         ((uint32_t)0x00000004)
#define UART_ICR_DSRMIC                         ((uint32_t)0x00000008)
#define UART_ICR_RXIC                           ((uint32_t)0x00000010)
#define UART_ICR_TXIC                           ((uint32_t)0x00000020)
#define UART_ICR_RTIC                           ((uint32_t)0x00000040)
#define UART_ICR_FEIC                           ((uint32_t)0x00000080)
#define UART_ICR_PEIC                           ((uint32_t)0x00000100)
#define UART_ICR_BEIC                           ((uint32_t)0x00000200)
#define UART_ICR_OEIC                           ((uint32_t)0x00000400)

/*-- UART_DMACR: UART DMA Control Register -------------------------*/
/* Bit field positions: */
#define UART_DMACR_RXDMAE_Pos                   0
#define UART_DMACR_TXDMAE_Pos                   1
#define UART_DMACR_DMAONERR_Pos                 2
/* Bit field masks: */
#define UART_DMACR_RXDMAE                       ((uint32_t)0x00000001)
#define UART_DMACR_TXDMAE                       ((uint32_t)0x00000002)
#define UART_DMACR_DMAONERR                     ((uint32_t)0x00000004)

/* _ssp structure */
typedef struct
{
  __IO uint32_t CR0;
  __IO uint32_t CR1;
  __IO uint32_t DR;
  __IO uint32_t SR;
  __IO uint32_t CPSR;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
  __IO uint32_t DMACR;
} _ssp;

/*-- SSP_CR0: SSP Control0 Register --------------------------------*/
/* Bit field positions: */
#define SSP_CR0_DSS_Pos                         0
#define SSP_CR0_FRF_Pos                         4
#define SSP_CR0_SPO_Pos                         6
#define SSP_CR0_SPH_Pos                         7
#define SSP_CR0_SCR_Pos                         8
/* Bit field masks: */
#define SSP_CR0_DSS_Msk                         ((uint32_t)0x0000000F)
#define SSP_CR0_FRF_Msk                         ((uint32_t)0x00000030)
#define SSP_CR0_SPO                             ((uint32_t)0x00000040)
#define SSP_CR0_SPH                             ((uint32_t)0x00000080)
#define SSP_CR0_SCR_Msk                         ((uint32_t)0x0000FF00)

/*-- SSP_CR1: SSP Control1 Register --------------------------------*/
/* Bit field positions: */
#define SSP_CR1_LBM_Pos                         0
#define SSP_CR1_SSE_Pos                         1
#define SSP_CR1_MS_Pos                          2
#define SSP_CR1_SOD_Pos                         3
/* Bit field masks: */
#define SSP_CR1_LBM                             ((uint32_t)0x00000001)
#define SSP_CR1_SSE                             ((uint32_t)0x00000002)
#define SSP_CR1_MS                              ((uint32_t)0x00000004)
#define SSP_CR1_SOD                             ((uint32_t)0x00000008)

/*-- SSP_SR: SSP Status Register -----------------------------------*/
/* Bit field positions: */
#define SSP_SR_TFE_Pos                          0
#define SSP_SR_TNF_Pos                          1
#define SSP_SR_RNE_Pos                          2
#define SSP_SR_RFF_Pos                          3
#define SSP_SR_BSY_Pos                          4
/* Bit field masks: */
#define SSP_SR_TFE                              ((uint32_t)0x00000001)
#define SSP_SR_TNF                              ((uint32_t)0x00000002)
#define SSP_SR_RNE                              ((uint32_t)0x00000004)
#define SSP_SR_RFF                              ((uint32_t)0x00000008)
#define SSP_SR_BSY                              ((uint32_t)0x00000010)

/*-- SSP_IMSC: SSP Interrupt Mask Register -------------------------*/
/* Bit field positions: */
#define SSP_IMSC_RORIM_Pos                      0
#define SSP_IMSC_RTIM_Pos                       1
#define SSP_IMSC_RXIM_Pos                       2
#define SSP_IMSC_TXIM_Pos                       3
/* Bit field masks: */
#define SSP_IMSC_RORIM                          ((uint32_t)0x00000001)
#define SSP_IMSC_RTIM                           ((uint32_t)0x00000002)
#define SSP_IMSC_RXIM                           ((uint32_t)0x00000004)
#define SSP_IMSC_TXIM                           ((uint32_t)0x00000008)

/*-- SSP_RIS: SSP Interrupt Pending Register -----------------------*/
/* Bit field positions: */
#define SSP_RIS_RORRIS_Pos                      0
#define SSP_RIS_RTRIS_Pos                       1
#define SSP_RIS_RXRIS_Pos                       2
#define SSP_RIS_TXRIS_Pos                       3
/* Bit field masks: */
#define SSP_RIS_RORRIS                          ((uint32_t)0x00000001)
#define SSP_RIS_RTRIS                           ((uint32_t)0x00000002)
#define SSP_RIS_RXRIS                           ((uint32_t)0x00000004)
#define SSP_RIS_TXRIS                           ((uint32_t)0x00000008)

/*-- SSP_MIS: SSP Masked Interrupt Pending Register ----------------*/
/* Bit field positions: */
#define SSP_MIS_RORMIS_Pos                      0
#define SSP_MIS_RTMIS_Pos                       1
#define SSP_MIS_RXMIS_Pos                       2
#define SSP_MIS_TXMIS_Pos                       3
/* Bit field masks: */
#define SSP_MIS_RORMIS                          ((uint32_t)0x00000001)
#define SSP_MIS_RTMIS                           ((uint32_t)0x00000002)
#define SSP_MIS_RXMIS                           ((uint32_t)0x00000004)
#define SSP_MIS_TXMIS                           ((uint32_t)0x00000008)

/*-- SSP_ICR: SSP Interrupt Clear Register -------------------------*/
/* Bit field positions: */
#define SSP_ICR_RORIC_Pos                       0
#define SSP_ICR_RTIC_Pos                        1
/* Bit field masks: */
#define SSP_ICR_RORIC                           ((uint32_t)0x00000001)
#define SSP_ICR_RTIC                            ((uint32_t)0x00000002)

/*-- SSP_DMACR: SSP DMA Control Register ---------------------------*/
/* Bit field positions: */
#define SSP_DMACR_RXDMAE_Pos                    0
#define SSP_DMACR_TXDMAE_Pos                    1
/* Bit field masks: */
#define SSP_DMACR_RXDMAE                        ((uint32_t)0x00000001)
#define SSP_DMACR_TXDMAE                        ((uint32_t)0x00000002)

/* _milstd structure */
typedef struct
{
  __IO uint32_t CONTROL;
  __IO uint32_t STATUS;
  __IO uint32_t ERROR;
  __IO uint32_t CommandWord1;
  __IO uint32_t CommandWord2;
  __IO uint32_t ModeData;
  __IO uint32_t StatusWord1;
  __IO uint32_t StatusWord2;
  __IO uint32_t INTEN;
  __IO uint32_t MSG;
} _milstd;

/*-- MILSTD_CONTROL:  ----------------------------------------------*/
/* Bit field positions: */
#define MILSTD_CONTROL_MR_Pos                   0
#define MILSTD_CONTROL_BCSTART_Pos              1
#define MILSTD_CONTROL_BCMODE_Pos               2
#define MILSTD_CONTROL_RTMODE_Pos               3
#define MILSTD_CONTROL_TRA_Pos                  4
#define MILSTD_CONTROL_TRB_Pos                  5
#define MILSTD_CONTROL_RTA_Pos                  6
#define MILSTD_CONTROL_DIV_Pos                  11
#define MILSTD_CONTROL_RERR_Pos                 18
/* Bit field masks: */
#define MILSTD_CONTROL_MR                       ((uint32_t)0x00000001)
#define MILSTD_CONTROL_BCSTART                  ((uint32_t)0x00000002)
#define MILSTD_CONTROL_BCMODE                   ((uint32_t)0x00000004)
#define MILSTD_CONTROL_RTMODE                   ((uint32_t)0x00000008)
#define MILSTD_CONTROL_TRA                      ((uint32_t)0x00000010)
#define MILSTD_CONTROL_TRB                      ((uint32_t)0x00000020)
#define MILSTD_CONTROL_RTA_Msk                  ((uint32_t)0x000007C0)
#define MILSTD_CONTROL_DIV_Msk                  ((uint32_t)0x0003F800)
#define MILSTD_CONTROL_RERR                     ((uint32_t)0x00040000)

/*-- MILSTD_STATUS:  -----------------------------------------------*/
/* Bit field positions: */
#define MILSTD_STATUS_IDLE_Pos                  0
#define MILSTD_STATUS_RFLAGN_Pos                1
#define MILSTD_STATUS_VALMESS_Pos               2
#define MILSTD_STATUS_ERR_Pos                   3
/* Bit field masks: */
#define MILSTD_STATUS_IDLE                      ((uint32_t)0x00000001)
#define MILSTD_STATUS_RFLAGN                    ((uint32_t)0x00000002)
#define MILSTD_STATUS_VALMESS                   ((uint32_t)0x00000004)
#define MILSTD_STATUS_ERR                       ((uint32_t)0x00000008)

/*-- MILSTD_ERROR:  ------------------------------------------------*/
/* Bit field positions: */
#define MILSTD_ERROR_NORCV_Pos                  0
#define MILSTD_ERROR_MANERR_Pos                 1
#define MILSTD_ERROR_SYNCERR_Pos                2
#define MILSTD_ERROR_SEQERR_Pos                 3
#define MILSTD_ERROR_GAPERR_Pos                 4
#define MILSTD_ERROR_CONERR_Pos                 5
#define MILSTD_ERROR_PROERR_Pos                 6
/* Bit field masks: */
#define MILSTD_ERROR_NORCV                      ((uint32_t)0x00000001)
#define MILSTD_ERROR_MANERR                     ((uint32_t)0x00000002)
#define MILSTD_ERROR_SYNCERR                    ((uint32_t)0x00000004)
#define MILSTD_ERROR_SEQERR                     ((uint32_t)0x00000008)
#define MILSTD_ERROR_GAPERR                     ((uint32_t)0x00000010)
#define MILSTD_ERROR_CONERR                     ((uint32_t)0x00000020)
#define MILSTD_ERROR_PROERR                     ((uint32_t)0x00000040)

/*-- MILSTD_INTEN:  ------------------------------------------------*/
/* Bit field positions: */
#define MILSTD_INTEN_IDLEIE_Pos                 0
#define MILSTD_INTEN_RFLAGNIE_Pos               1
#define MILSTD_INTEN_VALMESSIE_Pos              2
#define MILSTD_INTEN_ERRIE_Pos                  3
/* Bit field masks: */
#define MILSTD_INTEN_IDLEIE                     ((uint32_t)0x00000001)
#define MILSTD_INTEN_RFLAGNIE                   ((uint32_t)0x00000002)
#define MILSTD_INTEN_VALMESSIE                  ((uint32_t)0x00000004)
#define MILSTD_INTEN_ERRIE                      ((uint32_t)0x00000008)

/* _power structure */
typedef struct
{
  __IO uint32_t PVDCS;
} _power;

/*-- POWER_PVDCS: POWER Power Detector Control/Status Register -----*/
/* Bit field positions: */
#define POWER_PVDCS_PVDEN_Pos                   0
#define POWER_PVDCS_PBLS_Pos                    1
#define POWER_PVDCS_PLS_Pos                     3
#define POWER_PVDCS_PVBD_Pos                    6
#define POWER_PVDCS_PVD_Pos                     7
#define POWER_PVDCS_IEPVBD_Pos                  8
#define POWER_PVDCS_IEPVD_Pos                   9
#define POWER_PVDCS_INVB_Pos                    10
#define POWER_PVDCS_INV_Pos                     11
/* Bit field masks: */
#define POWER_PVDCS_PVDEN                       ((uint32_t)0x00000001)
#define POWER_PVDCS_PBLS_Msk                    ((uint32_t)0x00000006)
#define POWER_PVDCS_PLS_Msk                     ((uint32_t)0x00000038)
#define POWER_PVDCS_PVBD                        ((uint32_t)0x00000040)
#define POWER_PVDCS_PVD                         ((uint32_t)0x00000080)
#define POWER_PVDCS_IEPVBD                      ((uint32_t)0x00000100)
#define POWER_PVDCS_IEPVD                       ((uint32_t)0x00000200)
#define POWER_PVDCS_INVB                        ((uint32_t)0x00000400)
#define POWER_PVDCS_INV                         ((uint32_t)0x00000800)

/* _wwdg structure */
typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t CFR;
  __IO uint32_t SR;
} _wwdg;

/*-- WWDG_CR: WWDG Command Register --------------------------------*/
/* Bit field positions: */
#define WWDG_CR_T_Pos                           0
#define WWDG_CR_WDGA_Pos                        7
/* Bit field masks: */
#define WWDG_CR_T_Msk                           ((uint32_t)0x0000007F)
#define WWDG_CR_WDGA                            ((uint32_t)0x00000080)

/*-- WWDG_CFR: WWDG Configuration Register -------------------------*/
/* Bit field positions: */
#define WWDG_CFR_W_Pos                          0
#define WWDG_CFR_WGTB_Pos                       7
#define WWDG_CFR_EWI_Pos                        9
/* Bit field masks: */
#define WWDG_CFR_W_Msk                          ((uint32_t)0x0000007F)
#define WWDG_CFR_WGTB_Msk                       ((uint32_t)0x00000180)
#define WWDG_CFR_EWI                            ((uint32_t)0x00000200)

/* _iwdg structure */
typedef struct
{
  __IO uint32_t KR;
  __IO uint32_t PR;
  __IO uint32_t RLR;
  __IO uint32_t SR;
} _iwdg;

/*-- IWDG_SR: IWDG Status Register ---------------------------------*/
/* Bit field positions: */
#define IWDG_SR_PVU_Pos                         0
#define IWDG_SR_RVU_Pos                         1
/* Bit field masks: */
#define IWDG_SR_PVU                             ((uint32_t)0x00000001)
#define IWDG_SR_RVU                             ((uint32_t)0x00000002)

/* _timer structure */
typedef struct
{
  __IO uint32_t CNT;
  __IO uint32_t PSG;
  __IO uint32_t ARR;
  __IO uint32_t CNTRL;
  __IO uint32_t CCR1;
  __IO uint32_t CCR2;
  __IO uint32_t CCR3;
  __IO uint32_t CCR4;
  __IO uint32_t CH1_CNTRL;
  __IO uint32_t CH2_CNTRL;
  __IO uint32_t CH3_CNTRL;
  __IO uint32_t CH4_CNTRL;
  __IO uint32_t CH1_CNTRL1;
  __IO uint32_t CH2_CNTRL1;
  __IO uint32_t CH3_CNTRL1;
  __IO uint32_t CH4_CNTRL1;
  __IO uint32_t CH1_DTG;
  __IO uint32_t CH2_DTG;
  __IO uint32_t CH3_DTG;
  __IO uint32_t CH4_DTG;
  __IO uint32_t BRKETR_CNTRL;
  __IO uint32_t STATUS;
  __IO uint32_t IE;
  __IO uint32_t DMA_RE;
  __IO uint32_t CH1_CNTRL2;
  __IO uint32_t CH2_CNTRL2;
  __IO uint32_t CH3_CNTRL2;
  __IO uint32_t CH4_CNTRL2;
  __IO uint32_t CCR11;
  __IO uint32_t CCR21;
  __IO uint32_t CCR31;
  __IO uint32_t CCR41;
} _timer;

/*-- TIMER_CNTRL: Timer Control Register ---------------------------*/
/* Bit field positions: */
#define TIMER_CNTRL_CNT_EN_Pos                  0
#define TIMER_CNTRL_ARRB_EN_Pos                 1
#define TIMER_CNTRL_WR_CMPL_Pos                 2
#define TIMER_CNTRL_DIR_Pos                     3
#define TIMER_CNTRL_FDTS_Pos                    4
#define TIMER_CNTRL_CNT_MODE_Pos                6
#define TIMER_CNTRL_EVENT_SEL_Pos               8
/* Bit field masks: */
#define TIMER_CNTRL_CNT_EN                      ((uint32_t)0x00000001)
#define TIMER_CNTRL_ARRB_EN                     ((uint32_t)0x00000002)
#define TIMER_CNTRL_WR_CMPL                     ((uint32_t)0x00000004)
#define TIMER_CNTRL_DIR                         ((uint32_t)0x00000008)
#define TIMER_CNTRL_FDTS_Msk                    ((uint32_t)0x00000030)
#define TIMER_CNTRL_CNT_MODE_Msk                ((uint32_t)0x000000C0)
#define TIMER_CNTRL_EVENT_SEL_Msk               ((uint32_t)0x00000F00)

/*-- TIMER_CH_CNTRL: Timer Channel Control Register ----------------*/
/* Bit field positions: */
#define TIMER_CH_CNTRL_CHFLTR_Pos               0
#define TIMER_CH_CNTRL_CHSEL_Pos                4
#define TIMER_CH_CNTRL_CHPSC_Pos                6
#define TIMER_CH_CNTRL_OCCE_Pos                 8
#define TIMER_CH_CNTRL_OCCM_Pos                 9
#define TIMER_CH_CNTRL_BRKEN_Pos                12
#define TIMER_CH_CNTRL_ETREN_Pos                13
#define TIMER_CH_CNTRL_WR_CMPL_Pos              14
#define TIMER_CH_CNTRL_CAP_NPWM_Pos             15
/* Bit field masks: */
#define TIMER_CH_CNTRL_CHFLTR_Msk               ((uint32_t)0x0000000F)
#define TIMER_CH_CNTRL_CHSEL_Msk                ((uint32_t)0x00000030)
#define TIMER_CH_CNTRL_CHPSC_Msk                ((uint32_t)0x000000C0)
#define TIMER_CH_CNTRL_OCCE                     ((uint32_t)0x00000100)
#define TIMER_CH_CNTRL_OCCM_Msk                 ((uint32_t)0x00000E00)
#define TIMER_CH_CNTRL_BRKEN                    ((uint32_t)0x00001000)
#define TIMER_CH_CNTRL_ETREN                    ((uint32_t)0x00002000)
#define TIMER_CH_CNTRL_WR_CMPL                  ((uint32_t)0x00004000)
#define TIMER_CH_CNTRL_CAP_NPWM                 ((uint32_t)0x00008000)

/*-- TIMER_CH_CNTRL1: Timer Channel Control1 Register --------------*/
/* Bit field positions: */
#define TIMER_CH_CNTRL1_SELOE_Pos               0
#define TIMER_CH_CNTRL1_SELO_Pos                2
#define TIMER_CH_CNTRL1_INV_Pos                 4
#define TIMER_CH_CNTRL1_NSELOE_Pos              8
#define TIMER_CH_CNTRL1_NSELO_Pos               10
#define TIMER_CH_CNTRL1_NINV_Pos                12
/* Bit field masks: */
#define TIMER_CH_CNTRL1_SELOE_Msk               ((uint32_t)0x00000003)
#define TIMER_CH_CNTRL1_SELO_Msk                ((uint32_t)0x0000000C)
#define TIMER_CH_CNTRL1_INV                     ((uint32_t)0x00000010)
#define TIMER_CH_CNTRL1_NSELOE_Msk              ((uint32_t)0x00000300)
#define TIMER_CH_CNTRL1_NSELO_Msk               ((uint32_t)0x00000C00)
#define TIMER_CH_CNTRL1_NINV                    ((uint32_t)0x00001000)

/*-- TIMER_CH_DTG: Timer Channel DTG Register ----------------------*/
/* Bit field positions: */
#define TIMER_CH_DTGX_Pos                       0
#define TIMER_CH_DTG_EDTS_Pos                   4
#define TIMER_CH_DTG_Pos                        8
/* Bit field masks: */
#define TIMER_CH_DTGX_Msk                       ((uint32_t)0x0000000F)
#define TIMER_CH_DTG_EDTS                       ((uint32_t)0x00000010)
#define TIMER_CH_DTG_Msk                        ((uint32_t)0x0000FF00)

/*-- TIMER_BRKETR_CNTRL: Timer BRK/ETR Control Register ------------*/
/* Bit field positions: */
#define TIMER_BRKETR_CNTRL_BRK_INV_Pos          0
#define TIMER_BRKETR_CNTRL_ETR_INV_Pos          1
#define TIMER_BRKETR_CNTRL_ETR_PSC_Pos          2
#define TIMER_BRKETR_CNTRL_ETR_FILTER_Pos       4
/* Bit field masks: */
#define TIMER_BRKETR_CNTRL_BRK_INV              ((uint32_t)0x00000001)
#define TIMER_BRKETR_CNTRL_ETR_INV              ((uint32_t)0x00000002)
#define TIMER_BRKETR_CNTRL_ETR_PSC_Msk          ((uint32_t)0x0000000C)
#define TIMER_BRKETR_CNTRL_ETR_FILTER_Msk       ((uint32_t)0x000000F0)

/*-- TIMER_STATUS: Timer Status Register ---------------------------*/
/* Bit field positions: */
#define TIMER_STATUS_CNT_ZERO_EVENT_Pos         0
#define TIMER_STATUS_CNT_ARR_EVENT_Pos          1
#define TIMER_STATUS_ETR_RE_EVENT_Pos           2
#define TIMER_STATUS_ETR_FE_EVENT_Pos           3
#define TIMER_STATUS_BRK_EVENT_Pos              4
#define TIMER_STATUS_CCR_CAP_EVENT_Pos          5
#define TIMER_STATUS_CCR_REF_EVENT_Pos          9
#define TIMER_STATUS_CCR1_CAP_EVENT_Pos         13
/* Bit field masks: */
#define TIMER_STATUS_CNT_ZERO_EVENT             ((uint32_t)0x00000001)
#define TIMER_STATUS_CNT_ARR_EVENT              ((uint32_t)0x00000002)
#define TIMER_STATUS_ETR_RE_EVENT               ((uint32_t)0x00000004)
#define TIMER_STATUS_ETR_FE_EVENT               ((uint32_t)0x00000008)
#define TIMER_STATUS_BRK_EVENT                  ((uint32_t)0x00000010)
#define TIMER_STATUS_CCR_CAP_EVENT_Msk          ((uint32_t)0x000001E0)
#define TIMER_STATUS_CCR_REF_EVENT_Msk          ((uint32_t)0x00001E00)
#define TIMER_STATUS_CCR1_CAP_EVENT_Msk         ((uint32_t)0x0001E000)

/*-- TIMER_IE: Timer Interrupt Enable Register ---------------------*/
/* Bit field positions: */
#define TIMER_IE_CNT_ZERO_EVENT_IE_Pos          0
#define TIMER_IE_CNT_ARR_EVENT_IE_Pos           1
#define TIMER_IE_ETR_RE_EVENT_IE_Pos            2
#define TIMER_IE_ETR_FE_EVENT_IE_Pos            3
#define TIMER_IE_BRK_EVENT_IE_Pos               4
#define TIMER_IE_CCR_CAP_EVENT_IE_Pos           5
#define TIMER_IE_CCR_REF_EVENT_IE_Pos           9
/* Bit field masks: */
#define TIMER_IE_CNT_ZERO_EVENT_IE              ((uint32_t)0x00000001)
#define TIMER_IE_CNT_ARR_EVENT_IE               ((uint32_t)0x00000002)
#define TIMER_IE_ETR_RE_EVENT_IE                ((uint32_t)0x00000004)
#define TIMER_IE_ETR_FE_EVENT_IE                ((uint32_t)0x00000008)
#define TIMER_IE_BRK_EVENT_IE                   ((uint32_t)0x00000010)
#define TIMER_IE_CCR_CAP_EVENT_IE_Msk           ((uint32_t)0x000001E0)
#define TIMER_IE_CCR_REF_EVENT_IE_Msk           ((uint32_t)0x00001E00)

/*-- TIMER_DMA_RE: Timer DMA Request Enable Register ---------------*/
/* Bit field positions: */
#define TIMER_DMA_RE_CNT_ZERO_EVENT_RE_Pos      0
#define TIMER_DMA_RE_CNT_ARR_EVENT_RE_Pos       1
#define TIMER_DMA_RE_ETR_RE_EVENT_RE_Pos        2
#define TIMER_DMA_RE_ETR_FE_EVENT_RE_Pos        3
#define TIMER_DMA_RE_BRK_EVENT_RE_Pos           4
#define TIMER_DMA_RE_CCR_CAP_EVENT_RE_Pos       5
#define TIMER_DMA_RE_CCR_REF_EVENT_RE_Pos       9
/* Bit field masks: */
#define TIMER_DMA_RE_CNT_ZERO_EVENT_RE          ((uint32_t)0x00000001)
#define TIMER_DMA_RE_CNT_ARR_EVENT_RE           ((uint32_t)0x00000002)
#define TIMER_DMA_RE_ETR_RE_EVENT_RE            ((uint32_t)0x00000004)
#define TIMER_DMA_RE_ETR_FE_EVENT_RE            ((uint32_t)0x00000008)
#define TIMER_DMA_RE_BRK_EVENT_RE               ((uint32_t)0x00000010)
#define TIMER_DMA_RE_CCR_CAP_EVENT_RE_Msk       ((uint32_t)0x000001E0)
#define TIMER_DMA_RE_CCR_REF_EVENT_RE_Msk       ((uint32_t)0x00001E00)

/*-- TIMER_CH_CNTRL2: Timer Channel Control2 Register --------------*/
/* Bit field positions: */
#define TIMER_CH_CNTRL2_CHSEL1_Pos              0
#define TIMER_CH_CNTRL2_CCR1_EN_Pos             2
#define TIMER_CH_CNTRL2_CCRRLD_Pos              3
/* Bit field masks: */
#define TIMER_CH_CNTRL2_CHSEL1_Msk              ((uint32_t)0x00000003)
#define TIMER_CH_CNTRL2_CCR1_EN                 ((uint32_t)0x00000004)
#define TIMER_CH_CNTRL2_CCRRLD                  ((uint32_t)0x00000008)

/* _adc structure */
typedef struct
{
  __IO uint32_t ADC1_CFG;
  __IO uint32_t ADC2_CFG;
  __IO uint32_t ADC1_H_LEVEL;
  __IO uint32_t ADC2_H_LEVEL;
  __IO uint32_t ADC1_L_LEVEL;
  __IO uint32_t ADC2_L_LEVEL;
  __IO uint32_t ADC1_RESULT;
  __IO uint32_t ADC2_RESULT;
  __IO uint32_t ADC1_STATUS;
  __IO uint32_t ADC2_STATUS;
  __IO uint32_t ADC1_CHSEL;
  __IO uint32_t ADC2_CHSEL;
} _adc;

/*-- ADC1_CFG: ADC1 Configuration Register -------------------------*/
/* Bit field positions: */
#define ADC1_CFG_REG_ADON_Pos                   0
#define ADC1_CFG_REG_GO_Pos                     1
#define ADC1_CFG_REG_CLKS_Pos                   2
#define ADC1_CFG_REG_SAMPLE_Pos                 3
#define ADC1_CFG_REG_CHS_Pos                    4
#define ADC1_CFG_REG_CHCH_Pos                   9
#define ADC1_CFG_REG_RNGC_Pos                   10
#define ADC1_CFG_M_REF_Pos                      11
#define ADC1_CFG_REG_DIVCLK_Pos                 12
#define ADC1_CFG_SYNC_CONVER_Pos                16
#define ADC1_CFG_TS_EN_Pos                      17
#define ADC1_CFG_TS_BUF_EN_Pos                  18
#define ADC1_CFG_SEL_TS_Pos                     19
#define ADC1_CFG_SEL_VREF_Pos                   20
#define ADC1_CFG_TR_Pos                         21
#define ADC1_CFG_DELAY_GO_Pos                   25
#define ADC1_CFG_DELAY_ADC_Pos                  28
/* Bit field masks: */
#define ADC1_CFG_REG_ADON                       ((uint32_t)0x00000001)
#define ADC1_CFG_REG_GO                         ((uint32_t)0x00000002)
#define ADC1_CFG_REG_CLKS                       ((uint32_t)0x00000004)
#define ADC1_CFG_REG_SAMPLE                     ((uint32_t)0x00000008)
#define ADC1_CFG_REG_CHS_Msk                    ((uint32_t)0x000001F0)
#define ADC1_CFG_REG_CHCH                       ((uint32_t)0x00000200)
#define ADC1_CFG_REG_RNGC                       ((uint32_t)0x00000400)
#define ADC1_CFG_M_REF                          ((uint32_t)0x00000800)
#define ADC1_CFG_REG_DIVCLK_Msk                 ((uint32_t)0x0000F000)
#define ADC1_CFG_SYNC_CONVER                    ((uint32_t)0x00010000)
#define ADC1_CFG_TS_EN                          ((uint32_t)0x00020000)
#define ADC1_CFG_TS_BUF_EN                      ((uint32_t)0x00040000)
#define ADC1_CFG_SEL_TS                         ((uint32_t)0x00080000)
#define ADC1_CFG_SEL_VREF                       ((uint32_t)0x00100000)
#define ADC1_CFG_TR_Msk                         ((uint32_t)0x01E00000)
#define ADC1_CFG_DELAY_GO_Msk                   ((uint32_t)0x0E000000)
#define ADC1_CFG_DELAY_ADC_Msk                  ((uint32_t)0xF0000000)

/*-- ADC2_CFG: ADC2 Configuration Register -------------------------*/
/* Bit field positions: */
#define ADC2_CFG_REG_ADON_Pos                   0
#define ADC2_CFG_REG_GO_Pos                     1
#define ADC2_CFG_REG_CLKS_Pos                   2
#define ADC2_CFG_REG_SAMPLE_Pos                 3
#define ADC2_CFG_REG_CHS_Pos                    4
#define ADC2_CFG_REG_CHCH_Pos                   9
#define ADC2_CFG_REG_RNGC_Pos                   10
#define ADC2_CFG_M_REF_Pos                      11
#define ADC2_CFG_REG_DIVCLK_Pos                 12
#define ADC2_CFG_ADC1_OP_Pos                    17
#define ADC2_CFG_ADC2_OP_Pos                    18
#define ADC2_CFG_DELAY_GO_Pos                   25
/* Bit field masks: */
#define ADC2_CFG_REG_ADON                       ((uint32_t)0x00000001)
#define ADC2_CFG_REG_GO                         ((uint32_t)0x00000002)
#define ADC2_CFG_REG_CLKS                       ((uint32_t)0x00000004)
#define ADC2_CFG_REG_SAMPLE                     ((uint32_t)0x00000008)
#define ADC2_CFG_REG_CHS_Msk                    ((uint32_t)0x000001F0)
#define ADC2_CFG_REG_CHCH                       ((uint32_t)0x00000200)
#define ADC2_CFG_REG_RNGC                       ((uint32_t)0x00000400)
#define ADC2_CFG_M_REF                          ((uint32_t)0x00000800)
#define ADC2_CFG_REG_DIVCLK_Msk                 ((uint32_t)0x0000F000)
#define ADC2_CFG_ADC1_OP                        ((uint32_t)0x00020000)
#define ADC2_CFG_ADC2_OP                        ((uint32_t)0x00040000)
#define ADC2_CFG_DELAY_GO_Msk                   ((uint32_t)0x0E000000)

/*-- ADC1_RESULT: ADC1 Result Register -----------------------------*/
/*-- ADC2_RESULT: ADC2 Result Register -----------------------------*/
/* Bit field positions: */
#define ADC_RESULT_Pos                          0
#define ADC_RESULT_CHANNEL_Pos                  16
/* Bit field masks: */
#define ADC_RESULT_Msk                          ((uint32_t)0x00000FFF)
#define ADC_RESULT_CHANNEL_Msk                  ((uint32_t)0x0FFF0000)

/*-- ADC1_STATUS: ADC1 Status Register -----------------------------*/
/*-- ADC2_STATUS: ADC2 Status Register -----------------------------*/
/* Bit field positions: */
#define ADC_STATUS_FLG_REG_OVERWRITE_Pos        0
#define ADC_STATUS_FLG_REG_AWOIFEN_Pos          1
#define ADC_STATUS_FLG_REG_EOCIF_Pos            2
#define ADC_STATUS_AWOIF_IE_Pos                 3
#define ADC_STATUS_ECOIF_IE_Pos                 4
/* Bit field masks: */
#define ADC_STATUS_FLG_REG_OVERWRITE            ((uint32_t)0x00000001)
#define ADC_STATUS_FLG_REG_AWOIFEN              ((uint32_t)0x00000002)
#define ADC_STATUS_FLG_REG_EOCIF                ((uint32_t)0x00000004)
#define ADC_STATUS_AWOIF_IE                     ((uint32_t)0x00000008)
#define ADC_STATUS_ECOIF_IE                     ((uint32_t)0x00000010)

/* _dac structure */
typedef struct
{
  __IO uint32_t CFG;
  __IO uint32_t DAC1_DATA;
  __IO uint32_t DAC2_DATA;
} _dac;

/*-- DAC_CFG: DAC Configuration Register ---------------------------*/
/* Bit field positions: */
#define DAC_CFG_M_REF0_Pos                      0
#define DAC_CFG_M_REF1_Pos                      1
#define DAC_CFG_ON_DAC0_Pos                     2
#define DAC_CFG_ON_DAC1_Pos                     3
#define DAC_CFG_SYNC_A_Pos                      4
/* Bit field masks: */
#define DAC_CFG_M_REF0                          ((uint32_t)0x00000001)
#define DAC_CFG_M_REF1                          ((uint32_t)0x00000002)
#define DAC_CFG_ON_DAC0                         ((uint32_t)0x00000004)
#define DAC_CFG_ON_DAC1                         ((uint32_t)0x00000008)
#define DAC_CFG_SYNC_A                          ((uint32_t)0x00000010)

/*-- DAC1_DATA: DAC1 Data Register ---------------------------------*/
/* Bit field positions: */
#define DAC1_DATA_DAC0DATA_Pos                  0
#define DAC1_DATA_DAC1DATA_Pos                  16
/* Bit field masks: */
#define DAC1_DATA_DAC0DATA_Msk                  ((uint32_t)0x00000FFF)
#define DAC1_DATA_DAC1DATA_Msk                  ((uint32_t)0x0FFF0000)

/*-- DAC2_DATA: DAC2 Data Register ---------------------------------*/
/* Bit field positions: */
#define DAC2_DATA_DAC1DATA_Pos                  0
#define DAC2_DATA_DAC0DATA_Pos                  16
/* Bit field masks: */
#define DAC2_DATA_DAC1DATA_Msk                  ((uint32_t)0x00000FFF)
#define DAC2_DATA_DAC0DATA_Msk                  ((uint32_t)0x0FFF0000)

/* _port structure */
typedef struct
{
  __IO uint32_t RXTX;
  __IO uint32_t OE;
  __IO uint32_t FUNC;
  __IO uint32_t ANALOG;
  __IO uint32_t PULL;
  __IO uint32_t PD;
  __IO uint32_t PWR;
  __IO uint32_t GFEN;
} _port;

/*-- PORT_FUNC: PORT Function Register -----------------------------*/
/* Bit field positions: */
#define PORT_FUNC_MODE0_Pos                     0
#define PORT_FUNC_MODE1_Pos                     2
#define PORT_FUNC_MODE2_Pos                     4
#define PORT_FUNC_MODE3_Pos                     6
#define PORT_FUNC_MODE4_Pos                     8
#define PORT_FUNC_MODE5_Pos                     10
#define PORT_FUNC_MODE6_Pos                     12
#define PORT_FUNC_MODE7_Pos                     14
#define PORT_FUNC_MODE8_Pos                     16
#define PORT_FUNC_MODE9_Pos                     18
#define PORT_FUNC_MODE10_Pos                    20
#define PORT_FUNC_MODE11_Pos                    22
#define PORT_FUNC_MODE12_Pos                    24
#define PORT_FUNC_MODE13_Pos                    26
#define PORT_FUNC_MODE14_Pos                    28
#define PORT_FUNC_MODE15_Pos                    30
/* Bit field masks: */
#define PORT_FUNC_MODE0_Msk                     ((uint32_t)0x00000003)
#define PORT_FUNC_MODE1_Msk                     ((uint32_t)0x0000000C)
#define PORT_FUNC_MODE2_Msk                     ((uint32_t)0x00000030)
#define PORT_FUNC_MODE3_Msk                     ((uint32_t)0x000000C0)
#define PORT_FUNC_MODE4_Msk                     ((uint32_t)0x00000300)
#define PORT_FUNC_MODE5_Msk                     ((uint32_t)0x00000C00)
#define PORT_FUNC_MODE6_Msk                     ((uint32_t)0x00003000)
#define PORT_FUNC_MODE7_Msk                     ((uint32_t)0x0000C000)
#define PORT_FUNC_MODE8_Msk                     ((uint32_t)0x00030000)
#define PORT_FUNC_MODE9_Msk                     ((uint32_t)0x000C0000)
#define PORT_FUNC_MODE10_Msk                    ((uint32_t)0x00300000)
#define PORT_FUNC_MODE11_Msk                    ((uint32_t)0x00C00000)
#define PORT_FUNC_MODE12_Msk                    ((uint32_t)0x03000000)
#define PORT_FUNC_MODE13_Msk                    ((uint32_t)0x0C000000)
#define PORT_FUNC_MODE14_Msk                    ((uint32_t)0x30000000)
#define PORT_FUNC_MODE15_Msk                    ((uint32_t)0xC0000000)

/*-- PORT_PULL: PORT Pull Up/Down Register -------------------------*/
/* Bit field positions: */
#define PORT_PULL_DOWN_Pos                      0
#define PORT_PULL_UP_Pos                        16
/* Bit field masks: */
#define PORT_PULL_DOWN_Msk                      ((uint32_t)0x0000FFFF)
#define PORT_PULL_UP_Msk                        ((uint32_t)0xFFFF0000)

/*-- PORT_PD: PORT Driver Mode Register ----------------------------*/
/* Bit field positions: */
#define PORT_PD_Pos                             0
#define PORT_PD_SHM_Pos                         16
/* Bit field masks: */
#define PORT_PD_Msk                             ((uint32_t)0x0000FFFF)
#define PORT_PD_SHM_Msk                         ((uint32_t)0xFFFF0000)

/*-- PORT_PWR: PORT Power Register ---------------------------------*/
/* Bit field positions: */
#define PORT_PWR0_Pos                           0
#define PORT_PWR1_Pos                           2
#define PORT_PWR2_Pos                           4
#define PORT_PWR3_Pos                           6
#define PORT_PWR4_Pos                           8
#define PORT_PWR5_Pos                           10
#define PORT_PWR6_Pos                           12
#define PORT_PWR7_Pos                           14
#define PORT_PWR8_Pos                           16
#define PORT_PWR9_Pos                           18
#define PORT_PWR10_Pos                          20
#define PORT_PWR11_Pos                          22
#define PORT_PWR12_Pos                          24
#define PORT_PWR13_Pos                          26
#define PORT_PWR14_Pos                          28
#define PORT_PWR15_Pos                          30
/* Bit field masks: */
#define PORT_PWR0_Msk                           ((uint32_t)0x00000003)
#define PORT_PWR1_Msk                           ((uint32_t)0x0000000C)
#define PORT_PWR2_Msk                           ((uint32_t)0x00000030)
#define PORT_PWR3_Msk                           ((uint32_t)0x000000C0)
#define PORT_PWR4_Msk                           ((uint32_t)0x00000300)
#define PORT_PWR5_Msk                           ((uint32_t)0x00000C00)
#define PORT_PWR6_Msk                           ((uint32_t)0x00003000)
#define PORT_PWR7_Msk                           ((uint32_t)0x0000C000)
#define PORT_PWR8_Msk                           ((uint32_t)0x00030000)
#define PORT_PWR9_Msk                           ((uint32_t)0x000C0000)
#define PORT_PWR10_Msk                          ((uint32_t)0x00300000)
#define PORT_PWR11_Msk                          ((uint32_t)0x00C00000)
#define PORT_PWR12_Msk                          ((uint32_t)0x03000000)
#define PORT_PWR13_Msk                          ((uint32_t)0x0C000000)
#define PORT_PWR14_Msk                          ((uint32_t)0x30000000)
#define PORT_PWR15_Msk                          ((uint32_t)0xC0000000)

/* _arincr structure */
typedef struct
{
  __IO uint32_t CONTROL1;
  __IO uint32_t CONTROL2;
  __IO uint32_t CONTROL3;
  __IO uint32_t STATUS1;
  __IO uint32_t STATUS2;
  __IO uint32_t CONTROL4; //since rev.2
  __IO uint32_t CONTROL5; //since rev.2
  __IO uint32_t CHANNEL;
  __IO uint32_t LABEL;
  __IO uint32_t DATA_R;
  __IO uint32_t reserved1[2];
  __IO uint32_t DATA_R1;  //since rev.3
  __IO uint32_t DATA_R2;  //since rev.3
  __IO uint32_t DATA_R3;  //since rev.3
  __IO uint32_t DATA_R4;  //since rev.3
  __IO uint32_t DATA_R5;  //since rev.3
  __IO uint32_t DATA_R6;  //since rev.3
  __IO uint32_t DATA_R7;  //since rev.3
  __IO uint32_t DATA_R8;  //since rev.3
  __IO uint32_t reserved2[6];
  __IO uint32_t INTMASK;  //since rev.3
  __IO uint32_t reserved3;
  __IO uint32_t CONTROL8; //since rev.4
  __IO uint32_t CONTROL9; //since rev.4
} _arincr;

/*-- ARINCR_CONTROL1:  ---------------------------------------------*/
/* Bit field positions: */
#define ARINCR_CONTROL1_CH_EN1_Pos              0
#define ARINCR_CONTROL1_CH_EN2_Pos              1
#define ARINCR_CONTROL1_CH_EN3_Pos              2
#define ARINCR_CONTROL1_CH_EN4_Pos              3
#define ARINCR_CONTROL1_CH_EN5_Pos              4
#define ARINCR_CONTROL1_CH_EN6_Pos              5
#define ARINCR_CONTROL1_CH_EN7_Pos              6
#define ARINCR_CONTROL1_CH_EN8_Pos              7
#define ARINCR_CONTROL1_CLK1_Pos                14
#define ARINCR_CONTROL1_CLK2_Pos                15
#define ARINCR_CONTROL1_CLK3_Pos                16
#define ARINCR_CONTROL1_CLK4_Pos                17
#define ARINCR_CONTROL1_CLK5_Pos                18
#define ARINCR_CONTROL1_CLK6_Pos                19
#define ARINCR_CONTROL1_CLK7_Pos                20
#define ARINCR_CONTROL1_CLK8_Pos                21
#define ARINCR_CONTROL1_DIV_Pos                 28
/* Bit field masks: */
#define ARINCR_CONTROL1_CH_EN1                  ((uint32_t)0x00000001)
#define ARINCR_CONTROL1_CH_EN2                  ((uint32_t)0x00000002)
#define ARINCR_CONTROL1_CH_EN3                  ((uint32_t)0x00000004)
#define ARINCR_CONTROL1_CH_EN4                  ((uint32_t)0x00000008)
#define ARINCR_CONTROL1_CH_EN5                  ((uint32_t)0x00000010)
#define ARINCR_CONTROL1_CH_EN6                  ((uint32_t)0x00000020)
#define ARINCR_CONTROL1_CH_EN7                  ((uint32_t)0x00000040)
#define ARINCR_CONTROL1_CH_EN8                  ((uint32_t)0x00000080)
#define ARINCR_CONTROL1_CLK1                    ((uint32_t)0x00004000)
#define ARINCR_CONTROL1_CLK2                    ((uint32_t)0x00008000)
#define ARINCR_CONTROL1_CLK3                    ((uint32_t)0x00010000)
#define ARINCR_CONTROL1_CLK4                    ((uint32_t)0x00020000)
#define ARINCR_CONTROL1_CLK5                    ((uint32_t)0x00040000)
#define ARINCR_CONTROL1_CLK6                    ((uint32_t)0x00080000)
#define ARINCR_CONTROL1_CLK7                    ((uint32_t)0x00100000)
#define ARINCR_CONTROL1_CLK8                    ((uint32_t)0x00200000)
#define ARINCR_CONTROL1_DIV_Msk                 ((uint32_t)0xF0000000)

/*-- ARINCR_CONTROL2:  ---------------------------------------------*/
/* Bit field positions: */
#define ARINCR_CONTROL2_DIV_Pos                 0
#define ARINCR_CONTROL2_LB_EN1_Pos              3
#define ARINCR_CONTROL2_LB_EN2_Pos              4
#define ARINCR_CONTROL2_LB_EN3_Pos              5
#define ARINCR_CONTROL2_LB_EN4_Pos              6
#define ARINCR_CONTROL2_LB_EN5_Pos              7
#define ARINCR_CONTROL2_LB_EN6_Pos              8
#define ARINCR_CONTROL2_LB_EN7_Pos              9
#define ARINCR_CONTROL2_LB_EN8_Pos              10
#define ARINCR_CONTROL2_SD_EN1_Pos              17
#define ARINCR_CONTROL2_SD_EN2_Pos              18
#define ARINCR_CONTROL2_SD_EN3_Pos              19
#define ARINCR_CONTROL2_SD_EN4_Pos              20
#define ARINCR_CONTROL2_SD_EN5_Pos              21
#define ARINCR_CONTROL2_SD_EN6_Pos              22
#define ARINCR_CONTROL2_SD_EN7_Pos              23
#define ARINCR_CONTROL2_SD_EN8_Pos              24
/* Bit field masks: */
#define ARINCR_CONTROL2_DIV_Msk                 ((uint32_t)0x00000007)
#define ARINCR_CONTROL2_LB_EN1                  ((uint32_t)0x00000008)
#define ARINCR_CONTROL2_LB_EN2                  ((uint32_t)0x00000010)
#define ARINCR_CONTROL2_LB_EN3                  ((uint32_t)0x00000020)
#define ARINCR_CONTROL2_LB_EN4                  ((uint32_t)0x00000040)
#define ARINCR_CONTROL2_LB_EN5                  ((uint32_t)0x00000080)
#define ARINCR_CONTROL2_LB_EN6                  ((uint32_t)0x00000100)
#define ARINCR_CONTROL2_LB_EN7                  ((uint32_t)0x00000200)
#define ARINCR_CONTROL2_LB_EN8                  ((uint32_t)0x00000400)
#define ARINCR_CONTROL2_SD_EN1                  ((uint32_t)0x00020000)
#define ARINCR_CONTROL2_SD_EN2                  ((uint32_t)0x00040000)
#define ARINCR_CONTROL2_SD_EN3                  ((uint32_t)0x00080000)
#define ARINCR_CONTROL2_SD_EN4                  ((uint32_t)0x00100000)
#define ARINCR_CONTROL2_SD_EN5                  ((uint32_t)0x00200000)
#define ARINCR_CONTROL2_SD_EN6                  ((uint32_t)0x00400000)
#define ARINCR_CONTROL2_SD_EN7                  ((uint32_t)0x00800000)
#define ARINCR_CONTROL2_SD_EN8                  ((uint32_t)0x01000000)

/*-- ARINCR_CONTROL3:  ---------------------------------------------*/
/* Bit field positions: */
#define ARINCR_CONTROL3_SDI1_Pos                0
#define ARINCR_CONTROL3_SDI1_2_Pos              1
#define ARINCR_CONTROL3_SDI1_3_Pos              2
#define ARINCR_CONTROL3_SDI1_4_Pos              3
#define ARINCR_CONTROL3_SDI1_5_Pos              4
#define ARINCR_CONTROL3_SDI1_6_Pos              5
#define ARINCR_CONTROL3_SDI1_7_Pos              6
#define ARINCR_CONTROL3_SDI1_8_Pos              7
#define ARINCR_CONTROL3_SDI2_1_Pos              14
#define ARINCR_CONTROL3_SDI2_Pos                15
#define ARINCR_CONTROL3_SDI2_3_Pos              16
#define ARINCR_CONTROL3_SDI2_4_Pos              17
#define ARINCR_CONTROL3_SDI2_5_Pos              18
#define ARINCR_CONTROL3_SDI2_6_Pos              19
#define ARINCR_CONTROL3_SDI2_7_Pos              20
#define ARINCR_CONTROL3_SDI2_8_Pos              21
#define ARINCR_CONTROL3_INTEDR_Pos              28
#define ARINCR_CONTROL3_INTEER_Pos              29
#define ARINCR_CONTROL3_INTEFF_Pos              30
#define ARINCR_CONTROL3_INTEHF_Pos              31
/* Bit field masks: */
#define ARINCR_CONTROL3_SDI1                    ((uint32_t)0x00000001)
#define ARINCR_CONTROL3_SDI1_2                  ((uint32_t)0x00000002)
#define ARINCR_CONTROL3_SDI1_3                  ((uint32_t)0x00000004)
#define ARINCR_CONTROL3_SDI1_4                  ((uint32_t)0x00000008)
#define ARINCR_CONTROL3_SDI1_5                  ((uint32_t)0x00000010)
#define ARINCR_CONTROL3_SDI1_6                  ((uint32_t)0x00000020)
#define ARINCR_CONTROL3_SDI1_7                  ((uint32_t)0x00000040)
#define ARINCR_CONTROL3_SDI1_8                  ((uint32_t)0x00000080)
#define ARINCR_CONTROL3_SDI2_1                  ((uint32_t)0x00004000)
#define ARINCR_CONTROL3_SDI2                    ((uint32_t)0x00008000)
#define ARINCR_CONTROL3_SDI2_3                  ((uint32_t)0x00010000)
#define ARINCR_CONTROL3_SDI2_4                  ((uint32_t)0x00020000)
#define ARINCR_CONTROL3_SDI2_5                  ((uint32_t)0x00040000)
#define ARINCR_CONTROL3_SDI2_6                  ((uint32_t)0x00080000)
#define ARINCR_CONTROL3_SDI2_7                  ((uint32_t)0x00100000)
#define ARINCR_CONTROL3_SDI2_8                  ((uint32_t)0x00200000)
#define ARINCR_CONTROL3_INTEDR                  ((uint32_t)0x10000000)
#define ARINCR_CONTROL3_INTEER                  ((uint32_t)0x20000000)
#define ARINCR_CONTROL3_INTEFF                  ((uint32_t)0x40000000)
#define ARINCR_CONTROL3_INTEHF                  ((uint32_t)0x80000000)

/*-- ARINCR_STATUS1:  ----------------------------------------------*/
/* Bit field positions: */
#define ARINCR_STATUS1_DR1_Pos                  0
#define ARINCR_STATUS1_DR2_Pos                  1
#define ARINCR_STATUS1_DR3_Pos                  2
#define ARINCR_STATUS1_DR4_Pos                  3
#define ARINCR_STATUS1_DR5_Pos                  4
#define ARINCR_STATUS1_DR6_Pos                  5
#define ARINCR_STATUS1_DR7_Pos                  6
#define ARINCR_STATUS1_DR8_Pos                  7
#define ARINCR_STATUS1_ERR1_Pos                 14
#define ARINCR_STATUS1_ERR2_Pos                 15
#define ARINCR_STATUS1_ERR3_Pos                 16
#define ARINCR_STATUS1_ERR4_Pos                 17
#define ARINCR_STATUS1_ERR5_Pos                 18
#define ARINCR_STATUS1_ERR6_Pos                 19
#define ARINCR_STATUS1_ERR7_Pos                 20
#define ARINCR_STATUS1_ERR8_Pos                 21
/* Bit field masks: */
#define ARINCR_STATUS1_DR1                      ((uint32_t)0x00000001)
#define ARINCR_STATUS1_DR2                      ((uint32_t)0x00000002)
#define ARINCR_STATUS1_DR3                      ((uint32_t)0x00000004)
#define ARINCR_STATUS1_DR4                      ((uint32_t)0x00000008)
#define ARINCR_STATUS1_DR5                      ((uint32_t)0x00000010)
#define ARINCR_STATUS1_DR6                      ((uint32_t)0x00000020)
#define ARINCR_STATUS1_DR7                      ((uint32_t)0x00000040)
#define ARINCR_STATUS1_DR8                      ((uint32_t)0x00000080)
#define ARINCR_STATUS1_ERR1                     ((uint32_t)0x00004000)
#define ARINCR_STATUS1_ERR2                     ((uint32_t)0x00008000)
#define ARINCR_STATUS1_ERR3                     ((uint32_t)0x00010000)
#define ARINCR_STATUS1_ERR4                     ((uint32_t)0x00020000)
#define ARINCR_STATUS1_ERR5                     ((uint32_t)0x00040000)
#define ARINCR_STATUS1_ERR6                     ((uint32_t)0x00080000)
#define ARINCR_STATUS1_ERR7                     ((uint32_t)0x00100000)
#define ARINCR_STATUS1_ERR8                     ((uint32_t)0x00200000)

/*-- ARINCR_STATUS2:  ----------------------------------------------*/
/* Bit field positions: */
#define ARINCR_STATUS2_FF1_Pos                  0
#define ARINCR_STATUS2_FF2_Pos                  1
#define ARINCR_STATUS2_FF3_Pos                  2
#define ARINCR_STATUS2_FF4_Pos                  3
#define ARINCR_STATUS2_FF5_Pos                  4
#define ARINCR_STATUS2_FF6_Pos                  5
#define ARINCR_STATUS2_FF7_Pos                  6
#define ARINCR_STATUS2_FF8_Pos                  7
#define ARINCR_STATUS2_HF1_Pos                  14
#define ARINCR_STATUS2_HF2_Pos                  15
#define ARINCR_STATUS2_HF3_Pos                  16
#define ARINCR_STATUS2_HF4_Pos                  17
#define ARINCR_STATUS2_HF5_Pos                  18
#define ARINCR_STATUS2_HF6_Pos                  19
#define ARINCR_STATUS2_HF7_Pos                  20
#define ARINCR_STATUS2_HF8_Pos                  21
/* Bit field masks: */
#define ARINCR_STATUS2_FF1                      ((uint32_t)0x00000001)
#define ARINCR_STATUS2_FF2                      ((uint32_t)0x00000002)
#define ARINCR_STATUS2_FF3                      ((uint32_t)0x00000004)
#define ARINCR_STATUS2_FF4                      ((uint32_t)0x00000008)
#define ARINCR_STATUS2_FF5                      ((uint32_t)0x00000010)
#define ARINCR_STATUS2_FF6                      ((uint32_t)0x00000020)
#define ARINCR_STATUS2_FF7                      ((uint32_t)0x00000040)
#define ARINCR_STATUS2_FF8                      ((uint32_t)0x00000080)
#define ARINCR_STATUS2_HF1                      ((uint32_t)0x00004000)
#define ARINCR_STATUS2_HF2                      ((uint32_t)0x00008000)
#define ARINCR_STATUS2_HF3                      ((uint32_t)0x00010000)
#define ARINCR_STATUS2_HF4                      ((uint32_t)0x00020000)
#define ARINCR_STATUS2_HF5                      ((uint32_t)0x00040000)
#define ARINCR_STATUS2_HF6                      ((uint32_t)0x00080000)
#define ARINCR_STATUS2_HF7                      ((uint32_t)0x00100000)
#define ARINCR_STATUS2_HF8                      ((uint32_t)0x00200000)

/* _bkp structure */
typedef struct
{
  __IO uint32_t REG_00;
  __IO uint32_t REG_01;
  __IO uint32_t REG_02;
  __IO uint32_t REG_03;
  __IO uint32_t REG_04;
  __IO uint32_t REG_05;
  __IO uint32_t REG_06;
  __IO uint32_t REG_07;
  __IO uint32_t REG_08;
  __IO uint32_t REG_09;
  __IO uint32_t REG_0A;
  __IO uint32_t REG_0B;
  __IO uint32_t REG_0C;
  __IO uint32_t REG_0D;
  __IO uint32_t REG_0E;
  __IO uint32_t REG_0F;
  __IO uint32_t RTC_CNT;
  __IO uint32_t RTC_DIV;
  __IO uint32_t RTC_PRL;
  __IO uint32_t RTC_ALRM;
  __IO uint32_t RTC_CS;
} _bkp;

/*-- BKP_REG_0E: Backup Register 14 --------------------------------*/
/* Bit field positions: */
#define BKP_REG_0E_LOW_Pos                      0
#define BKP_REG_0E_SELECTRI_Pos                 3
#define BKP_REG_0E_STANDALONE_Pos               6
#define BKP_REG_0E_JTAGB_Pos                    7
#define BKP_REG_0E_TRIM_Pos                     8
#define BKP_REG_0E_FPOR_Pos                     11
#define BKP_REG_0E_TRIM34_Pos                   12
#define BKP_REG_0E_ILIMEN_Pos                   15
/* Bit field masks: */
#define BKP_REG_0E_LOW_Msk                      ((uint32_t)0x00000007)
#define BKP_REG_0E_SELECTRI_Msk                 ((uint32_t)0x00000038)
#define BKP_REG_0E_STANDALONE                   ((uint32_t)0x00000040)
#define BKP_REG_0E_JTAGB                        ((uint32_t)0x00000080)
#define BKP_REG_0E_TRIM_Msk                     ((uint32_t)0x00000700)
#define BKP_REG_0E_FPOR                         ((uint32_t)0x00000800)
#define BKP_REG_0E_TRIM34_Msk                   ((uint32_t)0x00003000)
#define BKP_REG_0E_ILIMEN                       ((uint32_t)0x00008000)

/*-- BKP_REG_0F: Backup Register 15 --------------------------------*/
/* Bit field positions: */
#define BKP_REG_0F_LSE_ON_Pos                   0
#define BKP_REG_0F_LSE_BYP_Pos                  1
#define BKP_REG_0F_RTC_SEL_Pos                  2
#define BKP_REG_0F_RTC_EN_Pos                   4
#define BKP_REG_0F_CAL_Pos                      5
#define BKP_REG_0F_LSE_RDY_Pos                  13
#define BKP_REG_0F_LSI_ON_Pos                   15
#define BKP_REG_0F_LSI_TRIM_Pos                 16
#define BKP_REG_0F_LSI_RDY_Pos                  21
#define BKP_REG_0F_HSI_ON_Pos                   22
#define BKP_REG_0F_HSI_RDY_Pos                  23
#define BKP_REG_0F_HSI_TRIM_Pos                 24
#define BKP_REG_0F_STANDBY_Pos                  30
#define BKP_REG_0F_RTC_RESET_Pos                31
/* Bit field masks: */
#define BKP_REG_0F_LSE_ON                       ((uint32_t)0x00000001)
#define BKP_REG_0F_LSE_BYP                      ((uint32_t)0x00000002)
#define BKP_REG_0F_RTC_SEL_Msk                  ((uint32_t)0x0000000C)
#define BKP_REG_0F_RTC_EN                       ((uint32_t)0x00000010)
#define BKP_REG_0F_CAL_Msk                      ((uint32_t)0x00001FE0)
#define BKP_REG_0F_LSE_RDY                      ((uint32_t)0x00002000)
#define BKP_REG_0F_LSI_ON                       ((uint32_t)0x00008000)
#define BKP_REG_0F_LSI_TRIM_Msk                 ((uint32_t)0x001F0000)
#define BKP_REG_0F_LSI_RDY                      ((uint32_t)0x00200000)
#define BKP_REG_0F_HSI_ON                       ((uint32_t)0x00400000)
#define BKP_REG_0F_HSI_RDY                      ((uint32_t)0x00800000)
#define BKP_REG_0F_HSI_TRIM_Msk                 ((uint32_t)0x3F000000)
#define BKP_REG_0F_STANDBY                      ((uint32_t)0x40000000)
#define BKP_REG_0F_RTC_RESET                    ((uint32_t)0x80000000)

/*-- BKP_RTC_CS: Backup Realtime clock Register --------------------*/
/* Bit field positions: */
#define BKP_RTC_CS_OWF_Pos                      0
#define BKP_RTC_CS_SECF_Pos                     1
#define BKP_RTC_CS_ALRF_Pos                     2
#define BKP_RTC_CS_OWF_IE_Pos                   3
#define BKP_RTC_CS_SECF_IE_Pos                  4
#define BKP_RTC_CS_ALRF_IE_Pos                  5
#define BKP_RTC_CS_WEC_Pos                      6
/* Bit field masks: */
#define BKP_RTC_CS_OWF                          ((uint32_t)0x00000001)
#define BKP_RTC_CS_SECF                         ((uint32_t)0x00000002)
#define BKP_RTC_CS_ALRF                         ((uint32_t)0x00000004)
#define BKP_RTC_CS_OWF_IE                       ((uint32_t)0x00000008)
#define BKP_RTC_CS_SECF_IE                      ((uint32_t)0x00000010)
#define BKP_RTC_CS_ALRF_IE                      ((uint32_t)0x00000020)
#define BKP_RTC_CS_WEC                          ((uint32_t)0x00000040)

/* _arinct structure */
typedef struct
{
  __IO uint32_t CONTROL1;
  __IO uint32_t CONTROL2;
  __IO uint32_t STATUS;
  __IO uint32_t DATA1_T;
  __IO uint32_t DATA2_T;
  __IO uint32_t DATA3_T;
  __IO uint32_t DATA4_T;
  __IO uint32_t CONTROL3; //since rev.2
  __IO uint32_t CONTROL4; //since rev.4
} _arinct;

/*-- ARINCT_CONTROL1:  ---------------------------------------------*/
/* Bit field positions: */
#define ARINCT_CONTROL1_CH_EN1_Pos              0
#define ARINCT_CONTROL1_CLK1_Pos                1
#define ARINCT_CONTROL1_EN_PAR1_Pos             2
#define ARINCT_CONTROL1_ODD1_Pos                3
#define ARINCT_CONTROL1_CH_EN2_Pos              4
#define ARINCT_CONTROL1_CLK2_Pos                5
#define ARINCT_CONTROL1_EN_PAR2_Pos             6
#define ARINCT_CONTROL1_ODD2_Pos                7
#define ARINCT_CONTROL1_DIV_Pos                 8
#define ARINCT_CONTROL1_INTE_FFT1_Pos           15
#define ARINCT_CONTROL1_INTE_TXR1_Pos           16
#define ARINCT_CONTROL1_INTE_HFT1_Pos           17
#define ARINCT_CONTROL1_INTE_FFT2_Pos           18
#define ARINCT_CONTROL1_INTE_TXR2_Pos           19
#define ARINCT_CONTROL1_INTE_HFT2_Pos           20
/* Bit field masks: */
#define ARINCT_CONTROL1_CH_EN1                  ((uint32_t)0x00000001)
#define ARINCT_CONTROL1_CLK1                    ((uint32_t)0x00000002)
#define ARINCT_CONTROL1_EN_PAR1                 ((uint32_t)0x00000004)
#define ARINCT_CONTROL1_ODD1                    ((uint32_t)0x00000008)
#define ARINCT_CONTROL1_CH_EN2                  ((uint32_t)0x00000010)
#define ARINCT_CONTROL1_CLK2                    ((uint32_t)0x00000020)
#define ARINCT_CONTROL1_EN_PAR2                 ((uint32_t)0x00000040)
#define ARINCT_CONTROL1_ODD2                    ((uint32_t)0x00000080)
#define ARINCT_CONTROL1_DIV_Msk                 ((uint32_t)0x00007F00)
#define ARINCT_CONTROL1_INTE_FFT1               ((uint32_t)0x00008000)
#define ARINCT_CONTROL1_INTE_TXR1               ((uint32_t)0x00010000)
#define ARINCT_CONTROL1_INTE_HFT1               ((uint32_t)0x00020000)
#define ARINCT_CONTROL1_INTE_FFT2               ((uint32_t)0x00040000)
#define ARINCT_CONTROL1_INTE_TXR2               ((uint32_t)0x00080000)
#define ARINCT_CONTROL1_INTE_HFT2               ((uint32_t)0x00100000)

/*-- ARINCT_CONTROL2:  ---------------------------------------------*/
/* Bit field positions: */
#define ARINCT_CONTROL2_CH_EN3_Pos              0
#define ARINCT_CONTROL2_CLK3_Pos                1
#define ARINCT_CONTROL2_EN_PAR3_Pos             2
#define ARINCT_CONTROL2_ODD3_Pos                3
#define ARINCT_CONTROL2_CH_EN4_Pos              4
#define ARINCT_CONTROL2_CLK4_Pos                5
#define ARINCT_CONTROL2_EN_PAR4_Pos             6
#define ARINCT_CONTROL2_ODD4_Pos                7
#define ARINCT_CONTROL2_INTE_FFT3_Pos           15
#define ARINCT_CONTROL2_INTE_TXR3_Pos           16
#define ARINCT_CONTROL2_INTE_HFT3_Pos           17
#define ARINCT_CONTROL2_INTE_FFT4_Pos           18
#define ARINCT_CONTROL2_INTE_TXR4_Pos           19
#define ARINCT_CONTROL2_INTE_HFT4_Pos           20
/* Bit field masks: */
#define ARINCT_CONTROL2_CH_EN3                  ((uint32_t)0x00000001)
#define ARINCT_CONTROL2_CLK3                    ((uint32_t)0x00000002)
#define ARINCT_CONTROL2_EN_PAR3                 ((uint32_t)0x00000004)
#define ARINCT_CONTROL2_ODD3                    ((uint32_t)0x00000008)
#define ARINCT_CONTROL2_CH_EN4                  ((uint32_t)0x00000010)
#define ARINCT_CONTROL2_CLK4                    ((uint32_t)0x00000020)
#define ARINCT_CONTROL2_EN_PAR4                 ((uint32_t)0x00000040)
#define ARINCT_CONTROL2_ODD4                    ((uint32_t)0x00000080)
#define ARINCT_CONTROL2_INTE_FFT3               ((uint32_t)0x00008000)
#define ARINCT_CONTROL2_INTE_TXR3               ((uint32_t)0x00010000)
#define ARINCT_CONTROL2_INTE_HFT3               ((uint32_t)0x00020000)
#define ARINCT_CONTROL2_INTE_FFT4               ((uint32_t)0x00040000)
#define ARINCT_CONTROL2_INTE_TXR4               ((uint32_t)0x00080000)
#define ARINCT_CONTROL2_INTE_HFT4               ((uint32_t)0x00100000)

/*-- ARINCT_STATUS:  -----------------------------------------------*/
/* Bit field positions: */
#define ARINCT_STATUS_TX_R1_Pos                 0
#define ARINCT_STATUS_FFT1_Pos                  1
#define ARINCT_STATUS_HFT1_Pos                  2
#define ARINCT_STATUS_TX_R2_Pos                 3
#define ARINCT_STATUS_FFT2_Pos                  4
#define ARINCT_STATUS_HFT2_Pos                  5
#define ARINCT_STATUS_TX_R3_Pos                 8
#define ARINCT_STATUS_FFT3_Pos                  9
#define ARINCT_STATUS_HFT3_Pos                  10
#define ARINCT_STATUS_TX_R4_Pos                 11
#define ARINCT_STATUS_FFT4_Pos                  12
#define ARINCT_STATUS_HFT4_Pos                  13
/* Bit field masks: */
#define ARINCT_STATUS_TX_R1                     ((uint32_t)0x00000001)
#define ARINCT_STATUS_FFT1                      ((uint32_t)0x00000002)
#define ARINCT_STATUS_HFT1                      ((uint32_t)0x00000004)
#define ARINCT_STATUS_TX_R2                     ((uint32_t)0x00000008)
#define ARINCT_STATUS_FFT2                      ((uint32_t)0x00000010)
#define ARINCT_STATUS_HFT2                      ((uint32_t)0x00000020)
#define ARINCT_STATUS_TX_R3                     ((uint32_t)0x00000100)
#define ARINCT_STATUS_FFT3                      ((uint32_t)0x00000200)
#define ARINCT_STATUS_HFT3                      ((uint32_t)0x00000400)
#define ARINCT_STATUS_TX_R4                     ((uint32_t)0x00000800)
#define ARINCT_STATUS_FFT4                      ((uint32_t)0x00001000)
#define ARINCT_STATUS_HFT4                      ((uint32_t)0x00002000)

/* _extbus structure */
typedef struct
{
       uint32_t RESERVED0[20];
  __IO uint32_t NAND_CYCLES;
  __IO uint32_t CONTROL;
  __IO uint32_t RAM_Cycles1;
  __IO uint32_t RAM_Cycles2;
  __IO uint32_t RAM_Cycles3;
  __IO uint32_t RAM_Cycles4;
} _extbus;

/*-- EBC_NAND_CYCLES: EBC NAND Timing Register ---------------------*/
/* Bit field positions: */
#define EBC_NAND_CYCLES_TRC_Pos                 0
#define EBC_NAND_CYCLES_TWC_Pos                 4
#define EBC_NAND_CYCLES_TREA_Pos                8
#define EBC_NAND_CYCLES_TWP_Pos                 12
#define EBC_NAND_CYCLES_TWHR_Pos                16
#define EBC_NAND_CYCLES_TALEA_Pos               20
#define EBC_NAND_CYCLES_TRR_Pos                 24
/* Bit field masks: */
#define EBC_NAND_CYCLES_TRC_Msk                 ((uint32_t)0x0000000F)
#define EBC_NAND_CYCLES_TWC_Msk                 ((uint32_t)0x000000F0)
#define EBC_NAND_CYCLES_TREA_Msk                ((uint32_t)0x00000F00)
#define EBC_NAND_CYCLES_TWP_Msk                 ((uint32_t)0x0000F000)
#define EBC_NAND_CYCLES_TWHR_Msk                ((uint32_t)0x000F0000)
#define EBC_NAND_CYCLES_TALEA_Msk               ((uint32_t)0x00F00000)
#define EBC_NAND_CYCLES_TRR_Msk                 ((uint32_t)0x0F000000)

/*-- EBC_CONTROL: EBC Control Register -----------------------------*/
/* Bit field positions: */
#define EBC_CONTROL_ROM_Pos                     0
#define EBC_CONTROL_RAM_Pos                     1
#define EBC_CONTROL_NAND_Pos                    2
#define EBC_CONTROL_CPOL_Pos                    3
#define EBC_CONTROL_ENDIAN_Pos                  4
#define EBC_CONTROL_LOW8_Pos                    5
#define EBC_CONTROL_LOW16_Pos                   6
#define EBC_CONTROL_BUSY_Pos                    7
#define EBC_CONTROL_WAIT_STATE_Pos              12
/* Bit field masks: */
#define EBC_CONTROL_ROM                         ((uint32_t)0x00000001)
#define EBC_CONTROL_RAM                         ((uint32_t)0x00000002)
#define EBC_CONTROL_NAND                        ((uint32_t)0x00000004)
#define EBC_CONTROL_CPOL                        ((uint32_t)0x00000008)
#define EBC_CONTROL_ENDIAN                      ((uint32_t)0x00000010)
#define EBC_CONTROL_LOW8                        ((uint32_t)0x00000020)
#define EBC_CONTROL_LOW16                       ((uint32_t)0x00000040)
#define EBC_CONTROL_BUSY                        ((uint32_t)0x00000080)
#define EBC_CONTROL_WAIT_STATE_Msk              ((uint32_t)0x0000F000)

/*-- EBC_RAM_Cycles1:  ---------------------------------------------*/
/*-- EBC_RAM_Cycles2:  ---------------------------------------------*/
/*-- EBC_RAM_Cycles3:  ---------------------------------------------*/
/*-- EBC_RAM_Cycles4:  ---------------------------------------------*/
/* Bit field positions: */
#define EBC_ENABLE_TUNE_Pos                     0
#define EBC_WS_ACTIVE_Pos                       1
#define EBC_WS_SETUP_Pos                        8
#define EBC_WS_HOLD_Pos                         11
#define EBC_USE_READY_Pos                       14
/* Bit field masks: */
#define EBC_ENABLE_TUNE                         ((uint32_t)0x00000001)
#define EBC_WS_ACTIVE_Msk                       ((uint32_t)0x000000FE)
#define EBC_WS_SETUP_Msk                        ((uint32_t)0x00000700)
#define EBC_WS_HOLD_Msk                         ((uint32_t)0x00003800)
#define EBC_USE_READY                           ((uint32_t)0x00004000)

/* MDR_ETH_TypeDef structure */
typedef struct
{
  __IO uint16_t Dilimiter;
  __IO uint16_t MAC_T;
  __IO uint16_t MAC_M;
  __IO uint16_t MAC_H;
  __IO uint16_t HASH0;
  __IO uint16_t HASH1;
  __IO uint16_t HASH2;
  __IO uint16_t HASH3;
  __IO uint16_t IPG;
  __IO uint16_t PSC;
  __IO uint16_t BAG;
  __IO uint16_t JitterWnd;
  __IO uint16_t R_CFG;
  __IO uint16_t X_CFG;
  __IO uint16_t G_CFG;
       uint16_t RESERVED0;
  __IO uint16_t IMR;
  __IO uint16_t IFR;
  __IO uint16_t MDIO_DATA;
  __IO uint16_t MDIO_CTRL;
  __IO uint16_t R_Head;
  __IO uint16_t X_Tail;
  __IO uint16_t R_Tail;
  __IO uint16_t X_Head;
  __IO uint16_t Buff_Stat;
       uint16_t RESERVED1;
  __IO uint16_t PHY_Control;
  __IO uint16_t PHY_Status;
} MDR_ETH_TypeDef;

/* _eth structure */
typedef struct
{                            //offset
  __io_reg16 Delimiter;      //00
  __io_reg16 MAC_T;          //02
  __io_reg16 MAC_M;          //04
  __io_reg16 MAC_H;          //06
  __io_reg16 HASH0;          //08
  __io_reg16 HASH1;          //0A
  __io_reg16 HASH2;          //0C
  __io_reg16 HASH3;          //0E
  __io_reg16 IPG;            //10
  __io_reg16 PSC;            //12
  __io_reg16 BAG;            //14
  __io_reg16 JitterWnd;      //16
  __io_reg16 R_CFG;          //18
  __io_reg16 X_CFG;          //1A
  __io_reg16 G_CFGl;         //1C
  __io_reg16 G_CFGh;         //1E
  __io_reg16 IMR;            //20
  __io_reg16 IFR;            //22
  __io_reg16 MDIO_CTRL;      //24
  __io_reg16 MDIO_DATA;      //26
  __io_reg16 R_Head;         //28
  __io_reg16 X_Tail;         //2A
  __io_reg16 R_Tail;         //2C
  __io_reg16 X_Head;         //2E
  __io_reg16 STAT;           //30
  __io_reg16 RESERVED1;      //32
  //PHY_Control, PHY_Status are declared as separate variables
} _eth;

/*-- ETH_R_CFG:  ---------------------------------------------------*/
/* Bit field positions: */
#define ETH_R_CFG_MCA_EN_Pos                    0
#define ETH_R_CFG_BCA_EN_Pos                    1
#define ETH_R_CFG_UCA_EN_Pos                    2
#define ETH_R_CFG_AC_EN_Pos                     3
#define ETH_R_CFG_EF_EN_Pos                     4
#define ETH_R_CFG_CF_EN_Pos                     5
#define ETH_R_CFG_LF_EN_Pos                     6
#define ETH_R_CFG_SF_EN_Pos                     7
#define ETH_R_CFG_EVNT_MODE_Pos                 8
#define ETH_R_CFG_MSB1ST_Pos                    12
#define ETH_R_CFG_BE_Pos                        13
#define ETH_R_CFG_RST_Pos                       15
/* Bit field masks: */
#define ETH_R_CFG_MCA_EN                        ((uint32_t)0x00000001)
#define ETH_R_CFG_BCA_EN                        ((uint32_t)0x00000002)
#define ETH_R_CFG_UCA_EN                        ((uint32_t)0x00000004)
#define ETH_R_CFG_AC_EN                         ((uint32_t)0x00000008)
#define ETH_R_CFG_EF_EN                         ((uint32_t)0x00000010)
#define ETH_R_CFG_CF_EN                         ((uint32_t)0x00000020)
#define ETH_R_CFG_LF_EN                         ((uint32_t)0x00000040)
#define ETH_R_CFG_SF_EN                         ((uint32_t)0x00000080)
#define ETH_R_CFG_EVNT_MODE_Msk                 ((uint32_t)0x00000700)
#define ETH_R_CFG_MSB1ST                        ((uint32_t)0x00001000)
#define ETH_R_CFG_BE                            ((uint32_t)0x00002000)
#define ETH_R_CFG_RST                           ((uint32_t)0x00008000)

/*-- ETH_X_CFG:  ---------------------------------------------------*/
/* Bit field positions: */
#define ETH_X_CFG_RTRYCNT_Pos                   0
#define ETH_X_CFG_IPG_EN_Pos                    4
#define ETH_X_CFG_CRC_EN_Pos                    5
#define ETH_X_CFG_PRE_EN_Pos                    6
#define ETH_X_CFG_PAD_EN_Pos                    7
#define ETH_X_CFG_EVNT_MODE_Pos                 8
#define ETH_X_CFG_MSB1ST_Pos                    12
#define ETH_X_CFG_BE_Pos                        13
#define ETH_X_CFG_RST_Pos                       15
/* Bit field masks: */
#define ETH_X_CFG_RTRYCNT_Msk                   ((uint32_t)0x0000000F)
#define ETH_X_CFG_IPG_EN                        ((uint32_t)0x00000010)
#define ETH_X_CFG_CRC_EN                        ((uint32_t)0x00000020)
#define ETH_X_CFG_PRE_EN                        ((uint32_t)0x00000040)
#define ETH_X_CFG_PAD_EN                        ((uint32_t)0x00000080)
#define ETH_X_CFG_EVNT_MODE_Msk                 ((uint32_t)0x00000700)
#define ETH_X_CFG_MSB1ST                        ((uint32_t)0x00001000)
#define ETH_X_CFG_BE                            ((uint32_t)0x00002000)
#define ETH_X_CFG_RST                           ((uint32_t)0x00008000)

/*-- ETH_G_CFG:  ---------------------------------------------------*/
/* Bit field positions: */
#define ETH_G_CFG_COLWND_Pos                    0
#define ETH_G_CFG_PAUSE_EN_Pos                  8
#define ETH_G_CFG_DTRM_EN_Pos                   9
#define ETH_G_CFG_HD_EN_Pos                     10
#define ETH_G_CFG_DLB_Pos                       11
#define ETH_G_CFG_AB_MODE_Pos                   12
#define ETH_G_CFG_SOFT_Pos                      14
#define ETH_G_CFG_FREE_Pos                      15
/* Bit field masks: */
#define ETH_G_CFG_COLWND_Msk                    ((uint32_t)0x000000FF)
#define ETH_G_CFG_PAUSE_EN                      ((uint32_t)0x00000100)
#define ETH_G_CFG_DTRM_EN                       ((uint32_t)0x00000200)
#define ETH_G_CFG_HD_EN                         ((uint32_t)0x00000400)
#define ETH_G_CFG_DLB                           ((uint32_t)0x00000800)
#define ETH_G_CFG_AB_MODE                       ((uint32_t)0x00001000)
#define ETH_G_CFG_SOFT                          ((uint32_t)0x00004000)
#define ETH_G_CFG_FREE                          ((uint32_t)0x00008000)

/*-- ETH_IMR:  -----------------------------------------------------*/
/*-- ETH_IFR:  -----------------------------------------------------*/
/* Bit field positions: */
#define ETH_RF_OK_Pos                           0
#define ETH_MISSED_F_Pos                        1
#define ETH_OVF_Pos                             2
#define ETH_SMB_ERR_Pos                         3
#define ETH_CRC_ERR_Pos                         4
#define ETH_CF_Pos                              5
#define ETH_LF_Pos                              6
#define ETH_SF_Pos                              7
#define ETH_XF_OK_Pos                           8
#define ETH_XF_ERR_Pos                          9
#define ETH_UNDF_Pos                            10
#define ETH_LC_Pos                              11
#define ETH_CRS_LOST_Pos                        12
#define ETH_MII_RDY_Pos                         15
/* Bit field masks: */
#define ETH_RF_OK                               ((uint32_t)0x00000001)
#define ETH_MISSED_F                            ((uint32_t)0x00000002)
#define ETH_OVF                                 ((uint32_t)0x00000004)
#define ETH_SMB_ERR                             ((uint32_t)0x00000008)
#define ETH_CRC_ERR                             ((uint32_t)0x00000010)
#define ETH_CF                                  ((uint32_t)0x00000020)
#define ETH_LF                                  ((uint32_t)0x00000040)
#define ETH_SF                                  ((uint32_t)0x00000080)
#define ETH_XF_OK                               ((uint32_t)0x00000100)
#define ETH_XF_ERR                              ((uint32_t)0x00000200)
#define ETH_UNDF                                ((uint32_t)0x00000400)
#define ETH_LC                                  ((uint32_t)0x00000800)
#define ETH_CRS_LOST                            ((uint32_t)0x00001000)
#define ETH_MII_RDY                             ((uint32_t)0x00008000)

/*-- ETH_MDIO_CTRL:  -----------------------------------------------*/
/* Bit field positions: */
#define ETH_MDIO_CTRL_RG_A_Pos                  0
#define ETH_MDIO_CTRL_DIV_Pos                   5
#define ETH_MDIO_CTRL_PHY_A_Pos                 8
#define ETH_MDIO_CTRL_OP_Pos                    13
#define ETH_MDIO_CTRL_PRE_EN_Pos                14
/* Bit field masks: */
#define ETH_MDIO_CTRL_RG_A_Msk                  ((uint32_t)0x0000001F)
#define ETH_MDIO_CTRL_DIV_Msk                   ((uint32_t)0x000000E0)
#define ETH_MDIO_CTRL_PHY_A_Msk                 ((uint32_t)0x00001F00)
#define ETH_MDIO_CTRL_OP                        ((uint32_t)0x00002000)
#define ETH_MDIO_CTRL_PRE_EN                    ((uint32_t)0x00004000)

/*-- ETH_Buff_Stat:  -----------------------------------------------*/
/* Bit field positions: */
#define ETH_BUFF_STAT_R_EMPTY_Pos               0
#define ETH_BUFF_STAT_R_AEMPTY_Pos              1
#define ETH_BUFF_STAT_R_HALF_Pos                2
#define ETH_BUFF_STAT_R_AFULL_Pos               3
#define ETH_BUFF_STAT_R_FULL_Pos                4
#define ETH_BUFF_STAT_X_EMPTY_Pos               8
#define ETH_BUFF_STAT_X_AEMPTY_Pos              9
#define ETH_BUFF_STAT_X_HALF_Pos                10
#define ETH_BUFF_STAT_X_AFULL_Pos               11
#define ETH_BUFF_STAT_X_FULL_Pos                12
/* Bit field masks: */
#define ETH_BUFF_STAT_R_EMPTY                   ((uint32_t)0x00000001)
#define ETH_BUFF_STAT_R_AEMPTY                  ((uint32_t)0x00000002)
#define ETH_BUFF_STAT_R_HALF                    ((uint32_t)0x00000004)
#define ETH_BUFF_STAT_R_AFULL                   ((uint32_t)0x00000008)
#define ETH_BUFF_STAT_R_FULL                    ((uint32_t)0x00000010)
#define ETH_BUFF_STAT_X_EMPTY                   ((uint32_t)0x00000100)
#define ETH_BUFF_STAT_X_AEMPTY                  ((uint32_t)0x00000200)
#define ETH_BUFF_STAT_X_HALF                    ((uint32_t)0x00000400)
#define ETH_BUFF_STAT_X_AFULL                   ((uint32_t)0x00000800)
#define ETH_BUFF_STAT_X_FULL                    ((uint32_t)0x00001000)


#define BASE_CAN1               (0x40000000)
#define BASE_CAN2               (0x40008000)
#define BASE_USB                (0x40010000)
#define BASE_EEPROM             (0x40018000)
#define BASE_RST_CLK            (0x40020000)
#define BASE_DMA                (0x40028000)
#define BASE_UART1              (0x40030000)
#define BASE_UART2              (0x40038000)
#define BASE_SSP1               (0x40040000)
#define BASE_MILSTD1_DATA       (0x40048000) //Rx/Tx data 1Kx16/1Kx16
#define BASE_MILSTD1            (0x40049000)
#define BASE_MILSTD2_DATA       (0x40050000) //Rx/Tx data 1Kx16/1Kx16
#define BASE_MILSTD2            (0x40051000)
#define BASE_POWER              (0x40058000)
#define BASE_WWDG               (0x40060000)
#define BASE_IWDG               (0x40068000)
#define BASE_TIMER1             (0x40070000)
#define BASE_TIMER2             (0x40078000)
#define BASE_TIMER3             (0x40080000)
#define BASE_ADC                (0x40088000)
#define BASE_DAC                (0x40090000)
#define BASE_TIMER4             (0x40098000)
#define BASE_SSP2               (0x400A0000)
#define BASE_PORTA              (0x400A8000)
#define BASE_PORTB              (0x400B0000)
#define BASE_PORTC              (0x400B8000)
#define BASE_PORTD              (0x400C0000)
#define BASE_PORTE              (0x400C8000)
#define BASE_ARINCR             (0x400D0000)
#define BASE_BKP                (0x400D8000)
#define BASE_ARINCT             (0x400E0000)
#define BASE_PORTF              (0x400E8000)
#define BASE_EXTBUS             (0x400F0000)
#define BASE_SSP3               (0x400F8000)
#define BASE_ETH                (0x30000000)
#define BASE_ETH_PHY_CTRL       (0x30000034)
#define BASE_ETH_PHY_STATUS     (0x30000036)
#define BASE_ETH_BUFFER         (0x38000000)

#define CAN1            		((_can    *) BASE_CAN1)
#define CAN2            		((_can    *) BASE_CAN2)
#define USB             		((_usb    *) BASE_USB)
#define EEPROM          		((_eeprom *) BASE_EEPROM)
#define RST_CLK         		((_rst_clk*) BASE_RST_CLK)
#define DMA             		((_dma    *) BASE_DMA)
#define UART1           		((_uart   *) BASE_UART1)
#define UART2           		((_uart   *) BASE_UART2)
#define SSP1            		((_ssp    *) BASE_SSP1)
#define MILSTD1         		((_milstd *) BASE_MILSTD1)
#define MILSTD2         		((_milstd *) BASE_MILSTD2)
#define POWER           		((_power  *) BASE_POWER)
#define WWDG            		((_wwdg   *) BASE_WWDG)
#define IWDG            		((_iwdg   *) BASE_IWDG)
#define TIMER1          		((_timer  *) BASE_TIMER1)
#define TIMER2          		((_timer  *) BASE_TIMER2)
#define TIMER3         			((_timer  *) BASE_TIMER3)
#define ADC            			((_adc    *) BASE_ADC)
#define DAC             		((_dac    *) BASE_DAC)
#define TIMER4          		((_timer  *) BASE_TIMER4)
#define SSP2            		((_ssp    *) BASE_SSP2)
#define PORTA           		((_port   *) BASE_PORTA)
#define PORTB           		((_port   *) BASE_PORTB)
#define PORTC           		((_port   *) BASE_PORTC)
#define PORTD           		((_port   *) BASE_PORTD)
#define PORTE           		((_port   *) BASE_PORTE)
#define ARINCR          		((_arincr *) BASE_ARINCR)
#define BKP             		((_bkp    *) BASE_BKP)
#define ARINCT          		((_arinct *) BASE_ARINCT)
#define PORTF          			((_port   *) BASE_PORTF)
#define EXTBUS          		((_extbus *) BASE_EXTBUS)
#define SSP3            		((_ssp    *) BASE_SSP3)
//SYSCTRL, SYSTICK, NVIC, SYS its addresses and structs defined at "core_cm0.h"
#define ETH             		((_eth    *) BASE_ETH)
extern __io_reg16       		*ETH_PHY_STATUS;
extern __io_reg16       		*ETH_PHY_CTRL;

#ifdef __cplusplus
}
#endif

#endif /* __1986BE1X_SFR_H */

/*===========================================================================
 * End of file 1986BE1x.h
 *===========================================================================*/
