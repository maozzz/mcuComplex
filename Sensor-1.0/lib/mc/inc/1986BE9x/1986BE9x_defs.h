/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE9x_defs.h: Hardware definitions of MCU 1986BE9x
 *
 *  MCU 1986BE9x address mapping:
 *  Code section       (512MB)
 *  [0x0000 0000..0x0000 07FF] - Boot ROM            (2KB)
 *  [0x0000 0800..0x07FF FFFF] - Unmapped            (128MB)
 *  [0x0800 0000..0x0807 FFFF] - EEPROM program      (512KB, MCU has 128KB)
 *  [0x0808 0000..0x0FFF FFFF] - Unmapped            (127.5MB)
 *  [0x1000 0000..0x1FFF FFFF] - External code       (256MB)
 *  Data section       (512MB)
 *  [0x2000 0000..0x2000 FFFF] - Internal RAM        (64KB, MCU has 32KB)
 *  [0x2001 0000..0x21FF FFFF] - Unmapped            (31.94MB)
 *  [0x2200 0000..0x23FF FFFF] - RAM bit-band        (32MB)
 *  [0x2400 0000..0x2FFF FFFF] - Unmapped            (192MB)
 *  [0x3000 0000..0x3FFF FFFF] - External RAM        (256MB)
 *  Peripheral section (512MB)
 *  [0x4000 0000..0x400F FFFF] - Peripheral device   (1MB)
 *  [0x4010 0000..0x41FF FFFF] - Unmapped            (31MB)
 *  [0x4200 0000..0x43FF FFFF] - Peripheral bit-band (32MB)
 *  [0x4400 0000..0x4FFF FFFF] - Unmapped            (192MB)
 *  [0x5000 0000..0x5FFF FFFF] - External RAM        (256MB)
 *  External bus       (2GB)
 *  [0x6000 0000..0x9FFF FFFF] - External RAM        (1GB)
 *  [0xA000 0000..0xDFFF FFFF] - External device     (1GB)
 *  System             (512MB)
 *  [0xE000 0000..0xE00F FFFF] - Core registers      (1MB)
 *  [0xE010 0000..0xFFFF FFFF] - Reserved            (511MB)
 *
 *===========================================================================*/

#ifndef __1986BE9x_DEFS_H
#define __1986BE9x_DEFS_H

#include "../../../bastypes.h"
#include "1986be91.h"


#define BASE_RAM_N1				(0x20000000)
#define SIZE_RAM_N1				(32 * 1024)

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
#define MAX_HCLK                108000000 //80000000 //Maximum core speed, Hz
#define MAX_VALID_HCLK          110000000 //82000000 //Maximum valid core speed, Hz

//Internal generators
#define FREQ_HSI                8000000  //8MHz HSI
#define FREQ_LSI                40000    //40KHz LSI

//Count of ports
#define PORT_COUNT              6

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
#define PA_In(mask)             PORTA->OE &= ~(mask)
#define PA_Out(mask)            PORTA->OE |= (mask)
//PORT B
#define PB_In(mask)             PORTB->OE &= ~(mask)
#define PB_Out(mask)            PORTB->OE |= (mask)
//PORT C
#define PC_In(mask)             PORTC->OE &= ~(mask)
#define PC_Out(mask)            PORTC->OE |= (mask)
//PORT D
#define PD_In(mask)             PORTD->OE &= ~(mask)
#define PD_Out(mask)            PORTD->OE |= (mask)
//PORT E
#define PE_In(mask)             PORTE->OE &= ~(mask)
#define PE_Out(mask)            PORTE->OE |= (mask)
//PORT F
#define PF_In(mask)             PORTF->OE &= ~(mask)
#define PF_Out(mask)            PORTF->OE |= (mask)

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

#define IRQ_CAN1        0
#define IRQ_CAN2        1
#define IRQ_USB         2
#define IRQ_DMA         5
#define IRQ_UART1       6
#define IRQ_UART2       7
#define IRQ_SSP1        8
#define IRQ_I2C1        10
#define IRQ_POWER       11
#define IRQ_WWDT        12
#define IRQ_TIMER1      14
#define IRQ_TIMER2      15
#define IRQ_TIMER3      16
#define IRQ_ADC         17
#define IRQ_COMP        19
#define IRQ_SSP2        20
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
#define CLK_I2C1      10
#define CLK_POWER     11
#define CLK_WWDT      12
#define CLK_IWDT      13
#define CLK_TIMER1    14
#define CLK_TIMER2    15
#define CLK_TIMER3    16
#define CLK_ADC       17
#define CLK_DAC       18
#define CLK_COMP      19
#define CLK_SSP2      20
#define CLK_PORTA     21
#define CLK_PORTB     22
#define CLK_PORTC     23
#define CLK_PORTD     24
#define CLK_PORTE     25
#define CLK_BKP       27
#define CLK_RTC       CLK_BKP
#define CLK_PORTF     29
#define CLK_EXTBUS    30


/* MCU software reset */
__STATIC_INLINE void MCUSoftReset()
{
    //1986BE9x: don't use AIRCR register (0xE000ED0C),
    //RESET signal is not cleared
    //0x05FA - key; SYSRESETREQ[2] = 1 - request reset
    //*((u32*)(0xE000ED0C)) = (0x05FA0000 | PN2);

    //Use watch timer to reset: when T > W - RESET generated
    //Enable WWDG clock
    ClkEnable(CLK_WWDT);

    //EWI[9]     = 0 - Disable interrupt
    //WGTB[8..7] = 0 - TMCLK = PCLK / 4096 / 1
    //W[6..0]    = 0 - window is 0
    WWDG->CFR = 0x00;

    //WDGA[7] = 1 - timer ON
    //T[6..0] = 0x7F - timer counter
    WWDG->CR = 0xFF;
    WWDG->CR = 0xFF;
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
#define DMA_CHNL_SOFT1			8
#define DMA_CHNL_SOFT2			9
#define DMA_CHNL_TIM1			10
#define DMA_CHNL_TIM2			11
#define DMA_CHNL_TIM3			12
#define DMA_CHNL_SOFT3			13
#define DMA_CHNL_SOFT4			14
#define DMA_CHNL_SOFT5			15
#define DMA_CHNL_SOFT6			16
#define DMA_CHNL_SOFT7			17
#define DMA_CHNL_SOFT8			18
#define DMA_CHNL_SOFT9			19
#define DMA_CHNL_SOFT10			20
#define DMA_CHNL_SOFT11			21
#define DMA_CHNL_SOFT12			22
#define DMA_CHNL_SOFT13			23
#define DMA_CHNL_SOFT14			24
#define DMA_CHNL_SOFT15			25
#define DMA_CHNL_SOFT16			26
#define DMA_CHNL_SOFT17			27
#define DMA_CHNL_SOFT18			28
#define DMA_CHNL_SOFT19			29
#define DMA_CHNL_SOFT20			30
#define DMA_CHNL_SOFT21			31

/*===========================================================================
 * EEPROM definitions
 * Remarks:
 * 1. EEPROM CODE: 128K = 32 pages x 4K
 * 2. EEPROM DATA:   4K =  1 page  x 4K
 *===========================================================================*/

//Start address of code eeprom
#define EEPROM_CODE_BASE	    0x08000000U

//Start address of data eeprom: same as BASE_EEPROM_CODE,
//data eeprom selected by set IFREN=1 (EEPROM->CMD register bit 9)
#define EEPROM_DATA_BASE	    0x08000000U

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

/*===========================================================================
 * ADC definitions
 * Remarks:
 * 1. MC has 2 ADC
 * 2. Valid clock sources:
 *      ADCConfig::clkSrc = CLKSRC_{HCLK|LSI|LSE|HSI}
 *===========================================================================*/

//Count of ADCs
#define ADC_COUNT               2           //Count of ADCs
#define ADC_CHNL_COUNT          16          //Count of ADC channels

#define CLKSRC_HCLK             15          //ADC clock from hclk

#define ADC_MAX_CLOCK           24000000    //Maximum ADC clock: 24MHz
#define ADC_MIN_CLOCK           40000       //Minimum ADC clock: 40KHz

#define ADC_MAXVALUE            0x0FFF      //ADC result 12bit
#define ADC_HALFVALUE           0x0800      //ADC half of 12 dig value
#define ADC_VALMASK             0x0FFF      //ADC value mask
#define ADC_VALSIZE             12          //ADC value size, bits
#define ADC_REF_VOLTAGE         3.25        //DON'T CHANGE! Internal Vref, V.

/*===========================================================================
 * DAC definitions
 * Remarks:
 * 1. MC DAC has 2 channels
 *===========================================================================*/

#define DAC_COUNT               1         //Count of DACs
#define DAC_CHNL_COUNT          2         //Count of DAC channels

#define DAC_MAXVALUE            0x0FFF    //DAC is 12-bit value
#define DAC_HALFVALUE           0x0800    //DAC half of 12-bit value
#define DAC_VALMASK             0x0FFF    //DAC value mask
#define DAC_VALSIZE             12        //DAC value size, bits

/*===========================================================================
 * COMP definitions
 * Remarks:
 * 1. MC has 1 comparator
 *===========================================================================*/

#define COMP_COUNT              1

//Comparator inputs selector (field COMPConfig::inSel)
//Input "+"
#define COMP_INP_IN1            0x00
#define COMP_INP_CVREF          0x01

//Input "-"
#define COMP_INM_IN2            0x00
#define COMP_INM_IN1            0x01
#define COMP_INM_IN3            0x02
#define COMP_INM_IVREF          0x03

//Comparator inputs mapping
#define COMP_MAP_IN_E2E8        1 //[In1,In2,Vref+,Vref-,In3] ->
                                  //[E2..E5, E8] (MODE = analog)

//Comparator output mapping
#define COMP_MAP_OUT_NONE       0 //No out
#define COMP_MAP_OUT_B8         1 //Out -> B8  (MODE = 2 - alter)
#define COMP_MAP_OUT_B11        2 //Out -> B11 (MODE = 3 - remap)

#define COMP_IVREF              1.25F //DON'T CHANGE! Internal Vref, V

/*===========================================================================
 * SSP definitions
 * Remarks:
 * 1. MC has 2 SSPs
 *===========================================================================*/

//Count of SSP blocks
#define SSP_COUNT               2

//Maximum SSP speed, bit/s
#define SSP_MAX_SPEED           25000000

//FIFO length
#define SSP_FIFO_RX_LENGTH      8 //Input FIFO length
#define SSP_FIFO_TX_LENGTH      8 //Output FIFO length

//SSP1 control mapping
#define SSP1_MAP_CTRL_B12B13    1 //SSP1 FSS/CLK -> B12/B13 (MODE = 2 - alter)
#define SSP1_MAP_CTRL_F2F1      2 //SSP1 FSS/CLK -> F2/F1   (MODE = 2 - alter)
#define SSP1_MAP_CTRL_D9D10     3 //SSP1 FSS/CLK -> D9/D10  (MODE = 3 - remap)

//SSP2 control mapping
#define SSP2_MAP_CTRL_D3D5      1 //SSP2 FSS/CLK -> D3/D5   (MODE = 2 - alter)
#define SSP2_MAP_CTRL_B12B13    2 //SSP2 FSS/CLK -> B12/B13 (MODE = 3 - remap)
#define SSP2_MAP_CTRL_C0C1      3 //SSP2 FSS/CLK -> C0/C1   (MODE = 3 - remap)
#define SSP2_MAP_CTRL_F12F13    4 //SSP2 FSS/CLK -> F12/F13 (MODE = 3 - remap)

//SSP1 Rx/Tx mapping
#define SSP1_MAP_RXTX_B14B15    1 //SSP1 Rx/Tx -> B14/B15 (MODE = 2 - alter)
#define SSP1_MAP_RXTX_F3F0      2 //SSP1 Rx/Tx -> F3/F0   (MODE = 2 - alter)
#define SSP1_MAP_RXTX_D11D12    3 //SSP2 Rx/Tx -> D11/D12 (MODE = 3 - remap)

//SSP2 Rx/Tx mapping
#define SSP2_MAP_RXTX_D2D6      1 //SSP2 Rx/Tx -> D2/D6   (MODE = 2 - alter)
#define SSP2_MAP_RXTX_B14B15    2 //SSP2 Rx/Tx -> B14/B15 (MODE = 3 - remap)
#define SSP2_MAP_RXTX_C2C3      3 //SSP2 Rx/Tx -> C2/C3   (MODE = 3 - remap)
#define SSP2_MAP_RXTX_F14F15    4 //SSP2 Rx/Tx -> F14/F15 (MODE = 3 - remap)

/*===========================================================================
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
#define UART1_MAP_CTRL_A10A15    1 //UART1 control i/o pins
                                   //[nDTR,nRTS,nRI,nDCD,nDSR,nCTS]->[A10..A15]

//UART1 Rx/Tx pins mapping
#define UART1_MAP_RXTX_B6B5      1 //UART1 Rx/Tx -> B6/B5 (MODE = 2 - alter)
#define UART1_MAP_RXTX_A6A7      2 //UART1 Rx/Tx -> A6/A7 (MODE = 3 - remap)
#define UART1_MAP_RXTX_D7D8      3 //UART1 Rx/Tx -> D7/D8 (MODE = 3 - remap)
#define UART1_MAP_RXTX_E12E13    4 //UART1 Rx/Tx -> E12/E13 (MODE = 3 - remap)
#define UART1_MAP_RXTX_D7E13     5 //UART1 Rx/Tx -> D7/E13  (MODE = 3 - remap)

//UART2 Rx/Tx pins mapping
#define UART2_MAP_RXTX_D0D1      1 //UART2 Rx/Tx -> D0/D1 (MODE = 2 - alter)
#define UART2_MAP_RXTX_F0F1      2 //UART2 Rx/Tx -> F0/F1 (MODE = 3 - remap)

//Minimum valid uart clock, Hz
#define UART_MINCLK              1000000

/*===========================================================================
 * CAN definitions
 * Remarks:
 * 1. MC has 2 CANs
 *===========================================================================*/

#define CAN_COUNT                2
//#define CAN_FIFO_RX_LENGTH      16      //Input FIFO length
//#define CAN_FIFO_TX_LENGTH      16      //Output FIFO length

//CAN1 Rx/Tx pins mapping
#define CAN1_MAP_RXTX_C9C8      1 //CAN1 Rx/Tx -> C9/C8   (MODE = 1 - basic)
#define CAN1_MAP_RXTX_A7A6      2 //CAN1 Rx/Tx -> A7/A6   (MODE = 2 - alter)
#define CAN1_MAP_RXTX_B3B2      3 //CAN1 Rx/Tx -> B3/B2   (MODE = 3 - remap)
#define CAN1_MAP_RXTX_D14D13    4 //CAN1 Rx/Tx -> D14/D13 (MODE = 3 - remap)
#define CAN1_MAP_RXTX_E0E1      5 //CAN1 Rx/Tx -> E0/E1   (MODE = 3 - remap)

//CAN2 Rx/Tx pins mapping
#define CAN2_MAP_RXTX_D15D9     1 //CAN2 Rx/Tx -> D15/D9  (MODE = 1 - basic)
#define CAN2_MAP_RXTX_E6E7      2 //CAN2 Rx/Tx -> E6/E7   (MODE = 2 - alter)
#define CAN2_MAP_RXTX_C14C15    3 //CAN2 Rx/Tx -> C14/C15 (MODE = 3 - remap)
#define CAN2_MAP_RXTX_F2F3      4 //CAN2 Rx/Tx -> F2/F3   (MODE = 3 - remap)

/*===========================================================================
 * I2C definitions
 * Remarks:
 *
 *===========================================================================*/

#define I2C_COUNT               1

//I2C pins mapping
#define I2C1_MAP_C0C1           0 //I2C1 SCK/SDA -> C0/C1   (alter)
#define I2C1_MAP_E14E15         1 //I2C1 SCK/SDA -> E14/E15 (remap)

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

/*===========================================================================
 * System timer definitions
 * Remarks:
 * 1. Valid clock sources:
 *      SysTimerConfig::clkSrc = CLKSRC_{HCLK|LSI}
 *===========================================================================*/

#define SYSTIM_COUNT            1

/*===========================================================================
 * TimerX definitions
 * Remarks:
 * 1. MC has 3 timers
 *===========================================================================*/

#define TIM_COUNT               3

/*===========================================================================
 * BKP and RTC definitions
 * Remarks:
 * 1. MC has 1 32-bit RTC
 * 2. Valid clock sources
 *      RTCConfig::clkSrc = CLKSRC_{LSI|LSE|HSI|HSE}
 *===========================================================================*/

#define RTC_COUNT               1

typedef u32                     RTCVALUE;

#endif /* __1986BE9x_DEFS_H */

/*===========================================================================
 * End of file 1986BE9x_defs.h
 *===========================================================================*/

