/*===========================================================================
 *
 *  MCU library
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
 *  File 1986BE1x_defs.h: Hardware definitions of MCU 1986BE1x
 *
 *  MCU 1986BE1x address mapping:
 *  Code section       (512MB)
 *  [0x0000 0000..0x000F FFFF] - ITCM EEPROM            (1MB, MCU has 128KB)
 *  [0x0010 0000..0x0FFF FFFF] - External code          (255MB)
 *  [0x1000 0000..0x100F FFFF] - ITCM upper alias       (1MB)
 *  [0x1010 0000..0x1FFF FFFF] - External code          (255MB)
 *  Data section       (512MB)
 *  [0x2000 0000..0x200F FFFF] - Internal RAM           (1MB, MCU has 32KB)
 *  [0x2010 0000..0x201F FFFF] - AHB-Lite RAM           (1MB, MCU has 16KB)
 *  [0x2020 0000..0x2FFF FFFF] - Unmapped               (254MB)
 *  [0x3000 0000..0x3FFF FFFF] - Ethernet RAM+registers (256MB, 8KB queue)
 *  Peripheral section (512MB)
 *  [0x4000 0000..0x4FFF FFFF] - Peripheral device      (256MB)
 *  [0x5000 0000..0x5FFF FFFF] - External peripheral    (256MB)
 *  External bus       (2GB)
 *  [0x6000 0000..0x9FFF FFFF] - External RAM           (1GB)
 *  [0xA000 0000..0xDFFF FFFF] - External device        (1GB)
 *  System             (512MB)
 *  [0xE000 0000..0xE00F EFFF] - Core registers         (1020KB)
 *  [0xE00F F000..0xE00F FFFF] - ROM table              (4KB)
 *  [0xE010 0000..0xFFFF FFFF] - Reserved               (511MB)
 *
 *===========================================================================*/

#ifndef __1986BE1x_DEFS_H
#define __1986BE1x_DEFS_H

#include "1986BE1x.h"
#include "../../../bastypes.h"

#define BASE_RAM_N1				(0x20000000)
#define SIZE_RAM_N1				(32 * 1024)

#define BASE_RAM_N2				(0x20100000)
#define SIZE_RAM_N2				(16 * 1024)

/*===========================================================================
 * CPU definitions
 * Remarks:
 * 1. MC has:
 *      2 internal generators,
 *      2 external oscillators
 *      6 ports (16pin)
 * 2. Valid clock sources:
 *     clkSrc = CLKSRC_{HSI|HSE|HSI_DIRECT}
 *     (CPU.h: SetHCLK(u32 hclk, u32 clkSrc);)
 *===========================================================================*/
//Core clock restrictions
#define MAX_HCLK                144000000 //Maximum core speed, Hz
#define MAX_VALID_HCLK          145000000 //Maximum valid core speed, Hz

//Internal generators
#define FREQ_HSI                8000000   //8MHz HSI
#define FREQ_LSI                40000     //40KHz LSI

#define PORT_COUNT              6 //Count of MCU ports

//Write/Read, Set/Reset bit macros
//PORT A
#define PA_Write(value)          PORTA->RXTX =  (value)
#define PA_Read()                PORTA->RXTX
#define PA_Set(mask)             PORTA->RXTX |=  (mask)
#define PA_Reset(mask)           PORTA->RXTX &= ~(mask)
//PORT B
#define PB_Write(value)          PORTB->RXTX =  (value)
#define PB_Read()                PORTB->RXTX
#define PB_Set(mask)             PORTB->RXTX |=  (mask)
#define PB_Reset(mask)           PORTB->RXTX &= ~(mask)
//PORT C
#define PC_Write(value)          PORTC->RXTX =  (value)
#define PC_Read()                PORTC->RXTX
#define PC_Set(mask)             PORTC->RXTX |=  (mask)
#define PC_Reset(mask)           PORTC->RXTX &= ~(mask)
//PORT D
#define PD_Write(value)          PORTD->RXTX =  (value)
#define PD_Read()                PORTD->RXTX
#define PD_Set(mask)             PORTD->RXTX |=  (mask)
#define PD_Reset(mask)           PORTD->RXTX &= ~(mask)
//PORT E
#define PE_Write(value)          PORTE->RXTX =  (value)
#define PE_Read()                PORTE->RXTX
#define PE_Set(mask)             PORTE->RXTX |=  (mask)
#define PE_Reset(mask)           PORTE->RXTX &= ~(mask)
//PORT F
#define PF_Write(value)          PORTF->RXTX =  (value)
#define PF_Read()                PORTF->RXTX
#define PF_Set(mask)             PORTF->RXTX |=  (mask)
#define PF_Reset(mask)           PORTF->RXTX &= ~(mask)


//Switch output/input macroses
//PORT A
#define PA_In(mask)              PORTA->OE &= ~(mask)
#define PA_Out(mask)             PORTA->OE |= (mask)
//PORT B
#define PB_In(mask)              PORTB->OE &= ~(mask)
#define PB_Out(mask)             PORTB->OE |= (mask)
//PORT C
#define PC_In(mask)              PORTC->OE &= ~(mask)
#define PC_Out(mask)             PORTC->OE |= (mask)
//PORT D
#define PD_In(mask)              PORTD->OE &= ~(mask)
#define PD_Out(mask)             PORTD->OE |= (mask)
//PORT E
#define PE_In(mask)              PORTE->OE &= ~(mask)
#define PE_Out(mask)             PORTE->OE |= (mask)
//PORT F
#define PF_In(mask)              PORTF->OE &= ~(mask)
#define PF_Out(mask)             PORTF->OE |= (mask)

/* Indicates that clock of specified peripheral block is enabled */
__STATIC_INLINE u32 IsClkEnable(u8 clknum)
{
		return (RST_CLK->PER_CLOCK & (1 << (u32)(clknum & 0x1F))) ? 1 : 0;
}

/* Enable clock of specified peripheral block */
__STATIC_INLINE void ClkEnable(u8 clknum)
{
        RST_CLK->PER_CLOCK |= (1 << (u32)(clknum & 0x1F));
}

/* Disable clock of specified peripheral block */
__STATIC_INLINE void ClkDisable(u8 clknum)
{
        RST_CLK->PER_CLOCK &= ~(1 << (u32)(clknum & 0x1F));
}

/* Enable interrupts from specified peripheral block */
__STATIC_INLINE void IRQEnable(u8 irq)
{
        NVIC->ISER[0] = (1 << (u32)((irq) & 0x1F));
}

/* Disable interrupts from specified peripheral block */
__STATIC_INLINE void IRQDisable(u8 irq)
{
        NVIC->ICER[0] = (1 << (u32)((irq) & 0x1F));
}

/* Set pending reuquest */
__STATIC_INLINE void IRQSetPending(u8 irq)
{
	NVIC->ISPR[0] = (1 << (u32)((irq) & 0x1F));
}

/* Clear pending reuquest */
__STATIC_INLINE void IRQClearPending(u8 irq)
{
	NVIC->ICPR[0] = (1 << (u32)((irq) & 0x1F));
}

//
#define __IRQH_FATTR		__STATIC_INLINE

typedef u16                     PINMASK;
typedef u16                     PINVALUE;

/*===========================================================================
 * IRQ numbers
 *===========================================================================*/

#define IRQ_MILSTD2     0
#define IRQ_MILSTD1     1
#define IRQ_USB         2
#define IRQ_CAN1        3
#define IRQ_CAN2        4
#define IRQ_DMA         5
#define IRQ_UART1       6
#define IRQ_UART2       7
#define IRQ_SSP1        8
#define IRQ_BUSY        9
#define IRQ_ARINCR      10
#define IRQ_POWER       11
#define IRQ_WWDT        12
#define IRQ_TIMER4      13
#define IRQ_TIMER1      14
#define IRQ_TIMER2      15
#define IRQ_TIMER3      16
#define IRQ_ADC         17
#define IRQ_ETH         18
#define IRQ_SSP3        19
#define IRQ_SSP2        20
#define IRQ_ARINCT1     21
#define IRQ_ARINCT2     22
#define IRQ_ARINCT3     23
#define IRQ_ARINCT4     24
#define IRQ_BACKUP      27
#define IRQ_RTC         IRQ_BACKUP
#define IRQ_EXTINT1     28
#define IRQ_EXTINT2     29
#define IRQ_EXTINT3     30
#define IRQ_EXTINT4     31

/*===========================================================================
 * Block numbers (PER_CLOCK register)
 *===========================================================================*/

#define CLK_CAN1      0
#define CLK_CAN2      1
#define CLK_USB       2
#define CLK_EEPROM    3
#define CLK_RST_CLK   4
#define CLK_DMA       5
#define CLK_UART1     6
#define CLK_UART2     7
#define CLK_SSP1      8
#define CLK_MILSTD1   9
#define CLK_MILSTD2   10
#define CLK_POWER     11
#define CLK_WWDT      12
#define CLK_IWDT      13
#define CLK_TIMER1    14
#define CLK_TIMER2    15
#define CLK_TIMER3    16
#define CLK_ADC       17
#define CLK_DAC       18
#define CLK_TIMER4    19
#define CLK_SSP2      20
#define CLK_PORTA     21
#define CLK_PORTB     22
#define CLK_PORTC     23
#define CLK_PORTD     24
#define CLK_PORTE     25
#define CLK_ARINCR    26
#define CLK_BKP       27
#define CLK_RTC       CLK_BKP
#define CLK_ARINCT    28
#define CLK_PORTF     29
#define CLK_EXTBUS    30
#define CLK_SSP3      31


/* MCU software reset */
__STATIC_INLINE void MCUSoftReset()
{
    //1986BE1x: can use AIRCR register (0xE000ED0C)
    //0x05FA - key; SYSRESETREQ[2] = 1 - request reset
    *((u32*)(0xE000ED0C)) = (0x05FA0000 | (1 << 2));

    /*
    //1901BC1x, 1986BE9x: don't use AIRCR register,
    //RESET signal is not cleared, use watch timer to reset
    //Enable WWDG clock
    ClkEnable(CLK_WWDT);

    //EWI[9]     = 0 - Disable WWDG interrupt
    //WGTB[8..7] = 0 - TMCLK = PCLK / 4096 / 1
    //W[6..0]    = 0 - window = 0
    WWDG->CFR = 0x00;

    //WDGA[7] = 1 - timer ON
    //T[6..0] = 0x7F - timer counter
    WWDG->CR = 0xFF;
    WWDG->CR = 0xFF;

    //Now W=0 and T=0x7F: T > W - RESET singal generated
    */
}

/*==========================================================================
 * DMA definitions
 *===========================================================================*/
//Count of DMA channels
#define DMA_CHNL_COUNT			32

//DMA channels
#define DMA_CHNL_UART1_TX		0
#define DMA_CHNL_UART1_RX		1
#define DMA_CHNL_UART2_TX		2
#define DMA_CHNL_UART2_RX		3
#define DMA_CHNL_SSP1_TX		4
#define DMA_CHNL_SSP1_RX		5
#define DMA_CHNL_SSP2_TX		6
#define DMA_CHNL_SSP2_RX		7
#define DMA_CHNL_SSP3_TX		8
#define DMA_CHNL_SSP3_RX		9
#define DMA_CHNL_TIM1			10
#define DMA_CHNL_TIM2			11
#define DMA_CHNL_TIM3			12
#define DMA_CHNL_TIM4			13
#define DMA_CHNL_TIM1_CH1		14
#define DMA_CHNL_TIM1_CH2		15
#define DMA_CHNL_TIM1_CH3		16
#define DMA_CHNL_TIM1_CH4		17
#define DMA_CHNL_TIM2_CH1		18
#define DMA_CHNL_TIM2_CH2		19
#define DMA_CHNL_TIM2_CH3		20
#define DMA_CHNL_TIM2_CH4		21
#define DMA_CHNL_TIM3_CH1		22
#define DMA_CHNL_TIM3_CH2		23
#define DMA_CHNL_TIM3_CH3		24
#define DMA_CHNL_TIM3_CH4		25
#define DMA_CHNL_TIM4_CH1		26
#define DMA_CHNL_TIM4_CH2		27
#define DMA_CHNL_TIM4_CH3		28
#define DMA_CHNL_TIM4_CH4		29
#define DMA_CHNL_ADC			30
#define DMA_CHNL_SOFT1			31

/*===========================================================================
 * EEPROM definitions
 * Remarks:
 * 1. EEPROM CODE: 128K = 32 pages x 4K
 * 2. EEPROM DATA:   4K =  1 page  x 4K
 *===========================================================================*/

//Start address of code eeprom
#define EEPROM_CODE_BASE	    0x00000000U

//Start address of data eeprom: same as BASE_EEPROM_CODE,
//data eeprom selected by set IFREN=1 (EEPROM->CMD register bit 9)
#define EEPROM_DATA_BASE	    0x00000000U

//Count of pages
#define EEPROM_CODE_PAGE_COUNT	    32

//Count of pages
#define EEPROM_DATA_PAGE_COUNT	    1

//Page size: 4K x 8
#define EEPROM_CODE_PAGE_SIZE	    4096

//Page size: 4K x 8
#define EEPROM_DATA_PAGE_SIZE	    4096

//Size of code eeprom: 128K x 8
#define EEPROM_CODE_SIZE	    (EEPROM_CODE_PAGE_COUNT * EEPROM_CODE_PAGE_SIZE)

//Size of data eeprom:   4K x 8
#define EEPROM_DATA_SIZE	    (EEPROM_DATA_PAGE_COUNT * EEPROM_DATA_PAGE_SIZE)

//__EEPROM_FDECL: EEPROM functions attributes (file "EEPROM.h")
#if defined(__CC_ARM)
#define __EEPROM_FATTR      __attribute__((section("EXECUTABLE_MEMORY_SECTION"), __noinline__))

#elif defined(__ICCARM__)
#define __EEPROM_FATTR      __ramfunc
//#define __EEPROM_FDECL    IAR_SECTION("EXECUTABLE_MEMORY_SECTION")

#elif defined (__CMCARM__)
#define __EEPROM_FATTR      __ramfunc
//#define __EEPROM_FATTR    #pragma ramfunc

#elif defined(__GNUC__)
#define __EEPROM_FATTR      __attribute__((section(".ramfunc"), __noinline__))

#else
#define __EEPROM_FATTR

#endif

/*==========================================================================
 * ADC definitions
 * Remarks:
 * 1. MC has 2 ADC
 * 2. Valid clock sources:
 *      ADCConfig::clkSrc = CLKSRC_{HCLK|LSI|LSE|HSI}
 *===========================================================================*/

#define ADC_COUNT               2           //Count of ADCs
#define ADC_CHNL_COUNT          8           //Count of ADC channels

#define CLKSRC_HCLK             15          //ADC clock from hclk

#define ADC_MAX_CLOCK           24000000    //Maximum ADC clock: 24MHz
#define ADC_MIN_CLOCK           40000       //Minimum ADC clock: 40KHz

#define ADC_MAXVALUE            0x0FFF      //ADC result 12bit
#define ADC_HALFVALUE           0x0800      //ADC half of 12 dig value
#define ADC_VALMASK             0x0FFF      //ADC value mask
#define ADC_VALSIZE             12          //ADC value size, bits
#define ADC_REF_VOLTAGE         3.25F       //DON'T CHANGE! Internal Vref, V.

/*==========================================================================
 * DAC definitions
 * Remarks:
 * 1. MC DAC has 2 channels
 *===========================================================================*/

#define DAC_COUNT               1            //Count of DACs
#define DAC_CHNL_COUNT          2            //Count of DAC channels

#define DAC_MAXVALUE            0x0FFF       //DAC is 12-bit value
#define DAC_HALFVALUE           0x0800       //DAC half of 12-bit value
#define DAC_VALMASK             0x0FFF       //DAC value mask
#define DAC_VALSIZE             12           //DAC value size, bits

/*==========================================================================
 * COMP definitions
 * Remarks:
 * 1. MC doesn't have comparator
 *===========================================================================*/

#define COMP_COUNT              0

/*==========================================================================
 * SSP definitions
 * Remarks:
 * 1. MC has 3 SSPs
 *===========================================================================*/

//Count of SSP blocks
#define SSP_COUNT               3

//Maximum SSP speed, bit/s
#define SSP_MAX_SPEED           25000000

//FIFO length
#define SSP_FIFO_RX_LENGTH      8 //Input FIFO length
#define SSP_FIFO_TX_LENGTH      8 //Output FIFO length

//SSP1 control mapping
#define SSP1_MAP_CTRL_C8C7      1 //SSP1 FSS/CLK -> C8/C7   (MODE = 2 - alter)
#define SSP1_MAP_CTRL_D5D4      2 //SSP1 FSS/CLK -> D5/D4   (MODE = 2 - alter)

//SSP2 control mapping
#define SSP2_MAP_CTRL_C12C11    1 //SSP2 FSS/CLK -> C12/C11 (MODE = 1 - basic)
#define SSP2_MAP_CTRL_D10D9     2 //SSP2 FSS/CLK -> D10/D9  (MODE = 1 - basic)

//SSP3 control mapping
#define SSP3_MAP_CTRL_F13F14    1 //SSP3 FSS/CLK -> F13/F14 (MODE = 3 - remap)

//SSP1 Rx/Tx mapping
#define SSP1_MAP_RXTX_C6C5      1 //SSP1 Rx/Tx -> C6/C5   (MODE = 2 - alter)
#define SSP1_MAP_RXTX_D3D2      2 //SSP1 Rx/Tx -> D3/D2   (MODE = 2 - alter)
#define SSP1_MAP_RXTX_C5C6      3 //SSP1 Rx/Tx -> C5/C6   (MODE = 3 - remap)

//SSP2 Rx/Tx mapping
#define SSP2_MAP_RXTX_C10C9     1 //SSP2 Rx/Tx -> C10/C9  (MODE = 1 - basic)
#define SSP2_MAP_RXTX_D8D7      2 //SSP2 Rx/Tx -> D8/D7   (MODE = 1 - basic)

//SSP3 Rx/Tx mapping
#define SSP3_MAP_RXTX_D12F15    1 //SSP3 Rx/Tx -> D12/F15 (MODE = 3 - remap)
#define SSP3_MAP_RXTX_F15D12    2 //SSP3 Rx/Tx -> F15/D12 (MODE = 1 - basic)

/*==========================================================================
 * UART definitions
 * Remarks:
 * 1. MC has 2 UARTs
 * 2. UARTx Rx/Tx pins can be mapped to different MC port pins
 * 3. UART2 control pins can't be mapped to MC port pins
 * 4. Input FIFO has 16bit elements:
 *       OE[11], BE[10], PE[9], FE[8], DATA[7..0]
 *       Error flags:
 *       OE - overflow buffer
 *       BE - break line
 *       PE - parity error
 *       FE - frame error (invalid stop bit)
 *===========================================================================*/

//Count of uart blocks
#define UART_COUNT               2

//FIFO length
#define UART_FIFO_RX_LENGTH      16 //Input FIFO length
#define UART_FIFO_TX_LENGTH      16 //Output FIFO length

//UARTx control pins mapping
#define UARTx_MAP_CTRL_NONE      0 //Not use control i/o
#define UART2_MAP_CTRL_D6D11     1 //UART1 control i/o pins (MODE = 2 - alter)
                                   //[nRI,nDCD,nDTR,nDSR,nRTS,nCTS]->[D6..D11]

//UART1 Rx/Tx pins mapping
#define UART1_MAP_RXTX_C4C3      1 //UART1 Rx/Tx -> C4/C3   (MODE = 1 - basic)

//UART2 Rx/Tx pins mapping
#define UART2_MAP_RXTX_D14D13    2 //UART2 Rx/Tx -> D14/D13 (MODE = 1 - basic)
#define UART2_MAP_RXTX_C14C13    3 //UART2 Rx/Tx -> C14/C13 (MODE = 3 - remap)

//Minimum valid uart clock, Hz
#define UART_MINCLK              1000000

/*===========================================================================
 * CAN definitions
 * Remarks:
 * 1. MC has 2 CANs
 *===========================================================================*/

//Count of can blocks
#define CAN_COUNT                2

//FIFO length
//#define CAN_FIFO_RX_LENGTH      16 //Input FIFO length
//#define CAN_FIFO_TX_LENGTH      16 //Output FIFO length

//CAN1 Rx/Tx pins mapping
#define CAN1_MAP_RXTX_E11E12    1 //CAN1 Rx/Tx -> E11/E12 (MODE = 1 - basic)
#define CAN1_MAP_RXTX_C9C10     2 //CAN1 Rx/Tx -> C9/C10  (MODE = 3 - remap)

//CAN2 Rx/Tx pins mapping
#define CAN2_MAP_RXTX_E13E14    3 //CAN2 Rx/Tx -> E13/E14 (MODE = 1 - basic)
#define CAN2_MAP_RXTX_C11C12    4 //CAN1 Rx/Tx -> C11/C12 (MODE = 3 - remap)

 /*==========================================================================
 * I2C definitions
 * Remarks:
 * 1. MC doesn't have I2C interface but software implementation used,
 *    master only
 *===========================================================================*/

#define I2C_COUNT               1

//I2C pins mapping
#define I2C1_MAP_C11C12         1 //SCL/SDA -> C11/C12 (portio)
#define I2C1_MAP_D0D1           2 //SCL/SDA -> D0/D1   (portio)

/*===========================================================================
 * USB definitions
 * Remarks:
 * 1. MC has 1 USB controller
 * 2. USB controller has 4 end points
 *===========================================================================*/

//Count of USB controllers
#define USB_COUNT               1

//Count of endpoints at one controller
#define USB_EP_COUNT            4

//Endpoint tx fifo length
#define USB_EP_FIFO_TX_LENGTH   64

//Endpoint rx fifo length
#define USB_EP_FIFO_RX_LENGTH   64

 /*==========================================================================
 * System timer definitions
 * Remarks:
 * 1. Valid clock sources:
 *      SysTimerConfig::clkSrc = CLKSRC_{HCLK|LSI}
 *===========================================================================*/

#define SYSTIM_COUNT            1

 /*==========================================================================
 * TimerX definitions
 * Remarks:
 * 1. MC has 4 timers
 *===========================================================================*/

#define TIM_COUNT               4

 /*==========================================================================
 * BKP and RTC definitions
 * Remarks:
 * 1. MC has 1 32-bit RTC
 * 2. Valid clock sources
 *      RTCConfig::clkSrc = CLKSRC_{LSI|LSE|HSI|HSE}
 *===========================================================================*/

#define RTC_COUNT               1

typedef u32                     RTCVALUE;

 /*==========================================================================
 * Ethernet definitions
 * Remarks:
 * 1. MC has 1 ethernet controller
 *===========================================================================*/

//Count of Ethernet controllers
#define ETH_COUNT               1

//8K buffer: (8 * 512 * 16bit)
#define ETH_BUFFER_LENGTH       8192

//Receiver buffer length or transmitter buffer start address
//Must at [0..ETH_BUFFER_LENGTH] and 4 bytes aligned
#define ETH_DELIMITER           (ETH_BUFFER_LENGTH >> 1)

/*==========================================================================
 * ARINC429 definitions
 * Remarks:
 * 1. MC has 1 ARINC429 receiver, 8 channels
 * 2. MC has 1 ARINC429 transmitter, 4 channels
 *===========================================================================*/

//Receiver: 8 channels
#define ARINCR_COUNT              8
//Transmitter: 4 channels
#define ARINCT_COUNT              4

//Input FIFO length (for each channel), 32-bit cells
#define ARINCR1_FIFO_RX_LENGTH    256
#define ARINCR2_FIFO_RX_LENGTH    256
#define ARINCR3_FIFO_RX_LENGTH    64
#define ARINCR4_FIFO_RX_LENGTH    64
#define ARINCR5_FIFO_RX_LENGTH    64
#define ARINCR6_FIFO_RX_LENGTH    64
#define ARINCR7_FIFO_RX_LENGTH    32
#define ARINCR8_FIFO_RX_LENGTH    32

//Output FIFO length (for each channel), 32-bit cells
#define ARINCT1_FIFO_TX_LENGTH    256
#define ARINCT2_FIFO_TX_LENGTH    64
#define ARINCT3_FIFO_TX_LENGTH    64
#define ARINCT4_FIFO_TX_LENGTH    64

//LABEL FIFO length, 8-bit cells (32x8 since revision 3)
#define ARINCRx_FIFO_LABEL_LENGTH   16

//ARINC rx pins mapping
#define ARINCR1_MAP_RX_B0B1       1 //[IN1+,IN1-] => [B0,B1]
#define ARINCR2_MAP_RX_B2B3       2 //[IN2+,IN2-] => [B2,B3]
#define ARINCR3_MAP_RX_B4B5       3 //[IN3+,IN3-] => [B4,B5]
#define ARINCR4_MAP_RX_B6B7       4 //[IN4+,IN4-] => [B6,B7]
#define ARINCR5_MAP_RX_B8B9       5 //[IN5+,IN5-] => [B8,B9]
#define ARINCR6_MAP_RX_B10B11     6 //[IN6+,IN6-] => [B10,B11]
#define ARINCR7_MAP_RX_B12B13     7 //[IN7+,IN7-] => [B12,B13]
#define ARINCR8_MAP_RX_B14B16     8 //[IN8+,IN8-] => [B14,B15]

//ARINC tx pins mapping
#define ARINCT1_MAP_TX_D13D14     1 //[OUT1+,OUT1-] => [D13,D14]

#define ARINCT2_MAP_TX_F11F12     2 //[OUT2+,OUT2-] => [F11,F12]
#define ARINCT2_MAP_TX_F13F14     3 //[OUT2+,OUT2-] => [F13,F14]

#define ARINCT3_MAP_TX_D15E1      4 //[OUT3+,OUT3-] => [D15,E1]
#define ARINCT3_MAP_TX_F9F10      5 //[OUT3+,OUT3-] => [F9,F10]

#define ARINCT4_MAP_TX_E0E2       6 //[OUT4+,OUT4-] => [E0,E2]
#define ARINCT4_MAP_TX_F7F8       7 //[OUT4+,OUT4-] => [F7,F8]


/*==========================================================================
 * MILSTD-1553 definitions
 * Remarks:
 * 1. MC has 2 MILSTD controllers
 *===========================================================================*/

#define MILSTD_COUNT              2

//MILSTD1 Rx/Tx pins mapping
//Rx [PRMA+,PRMA-, PRMB+,PRMB-] => [C13,C14, C15,D0]
//Tx [PRDA+,PRDA-,PRD_PRMA, PRDB+,PRDB-,PRD_PRMB] => [D1,D2,D5, D3,D4,D6]
#define MILSTD1_MAP_RXTX_C13D6    1


//MILSTD2 Rx/Tx pins mapping
//Rx [PRMC+,PRMC-, PRMD+,PRMD-] => [F3,F4, F5,F6]
//Tx [PRDC+,PRDC-,PRD_PRMC, PRDD+,PRDD-,PRD_PRMD] => [F7,F8,F11, F9,F10,F12]
#define MILSTD2_MAP_RXTX_F3F12    1

//Rx [PRMC+,PRMC-, PRMD+,PRMD-] => [A8,A9, A10,A11]
//Tx [PRDC+,PRDC-,PRD_PRMC, PRDD+,PRDD-,PRD_PRMD] => [A12,A13,F11, A14,A15,F12]
#define MILSTD2_MAP_RXTX_A8A15    2

#endif /* __1986BE1x_DEFS_H */

/*===========================================================================
 * End of file 1986BE1x_defs.h
 *===========================================================================*/

