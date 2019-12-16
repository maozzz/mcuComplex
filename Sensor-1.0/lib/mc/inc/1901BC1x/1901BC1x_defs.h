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
 *  File 1901BC1x_defs.h: Hardware definitions of MCU 1901BC1x.
 *
 *  MCU 1901BC1x address mapping:
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

#ifndef __1901BC1X_DEFS_H
#define __1901BC1X_DEFS_H

#include "1901BC1x.h"
#include "../../../bastypes.h"

#define BASE_RAM_N1				(0x20000000)
#define SIZE_RAM_N1				(32 * 1024)

//DSP memory: [DATA 128K] + [CODE 128K] - [DSP core/peripheral registers 256B]
#define BASE_RAM_N2				(0x30000100)
#define SIZE_RAM_N2				(256 * 1024 - 256)

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
//ARM core clock restrictions
#define MAX_HCLK                100000000 //Maximum core speed, Hz
#define MAX_VALID_HCLK          112000000 //Maximum valid core speed, Hz

//DPS core clock restrictions
#define MAX_DSPCLK              100000000 //Maximum core speed, Hz
#define MAX_VALID_DSPCLK        112000000 //Maximum valid core speed, Hz

//Internal generators
#define FREQ_HSI                8000000  //8MHz HSI
#define FREQ_LSI                40000    //40KHz LSI

//Count of ports
#define PORT_COUNT              6

//Write/Read, Set/Reset bit macros
//PORT A
#define PA_Write(value)          PORTA->RXTX = (value)
#define PA_Read()                PORTA->RXTX
#define PA_Set(mask)             PORTA->SET  = (mask)
#define PA_Reset(mask)           PORTA->CLR  = (mask)
//PORT B
#define PB_Write(value)          PORTB->RXTX = (value)
#define PB_Read()                PORTB->RXTX
#define PB_Set(mask)             PORTB->SET  = (mask)
#define PB_Reset(mask)           PORTB->CLR  = (mask)
//PORT C
#define PC_Write(value)          PORTC->RXTX = (value)
#define PC_Read()                PORTC->RXTX
#define PC_Set(mask)             PORTC->SET  = (mask)
#define PC_Reset(mask)           PORTC->CLR  = (mask)
//PORT D
#define PD_Write(value)          PORTD->RXTX = (value)
#define PD_Read()                PORTD->RXTX
#define PD_Set(mask)             PORTD->SET  = (mask)
#define PD_Reset(mask)           PORTD->CLR  = (mask)
//PORT E
#define PE_Write(value)          PORTE->RXTX = (value)
#define PE_Read()                PORTE->RXTX
#define PE_Set(mask)             PORTE->SET  = (mask)
#define PE_Reset(mask)           PORTE->CLR  = (mask)
//PORT F
#define PF_Write(value)          PORTF->RXTX = (value)
#define PF_Read()                PORTF->RXTX
#define PF_Set(mask)             PORTF->SET  = (mask)
#define PF_Reset(mask)           PORTF->CLR  = (mask)

//Switch output/input macros
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

#define IRQ_SSP3             0
#define IRQ_SSP4             1
#define IRQ_USB              2
#define IRQ_DSP_BSP1	     3
#define IRQ_DSP_BSP2	     4
#define IRQ_DMA              5
#define IRQ_UART1            6
#define IRQ_UART2            7
#define IRQ_SSP1             8
#define IRQ_DSP_BSP3         9
#define IRQ_I2C1             10
#define IRQ_POWER            11
#define IRQ_WWDG             12
#define IRQ_DSP_DMA	     13
#define IRQ_TIMER1           14
#define IRQ_TIMER2           15
#define IRQ_TIMER3           16
#define IRQ_ADC              17
#define IRQ_SDIO             18
#define IRQ_COMP	     19
#define IRQ_SSP2             20
#define IRQ_DSP_ACODEC	     21
#define IRQ_DSP_CRYPTO       22
#define IRQ_DSP_TIMER        23
#define IRQ_DSP_CORE         24
#define IRQ_DSP_STATE        25
#define IRQ_UART3            26
#define IRQ_BACKUP           27
#define IRQ_RTC		     IRQ_BACKUP
#define IRQ_EXTINT1	    28
#define IRQ_EXTINT2	    29
#define IRQ_EXTINT3	    30
#define IRQ_EXTINT4	    31

/*===========================================================================
 * Block numbers (PER_CLOCK register)
 *===========================================================================*/

#define CLK_SSP3			0
#define CLK_SSP4			1
#define CLK_USB				2
#define CLK_EEPROM			3
#define CLK_RST_CLK			4
#define CLK_DMA				5
#define CLK_UART1			6
#define CLK_UART2			7
#define CLK_SSP1			8
#define CLK_SDI0			9
#define CLK_I2C1			10
#define CLK_POWER			11
#define CLK_WWDT			12
#define CLK_IWDT			13
#define CLK_TIMER1			14
#define CLK_TIMER2			15
#define CLK_TIMER3			16
#define CLK_ADC				17
#define CLK_DAC				18
#define CLK_COMP			19
#define CLK_SSP2			20
#define CLK_PORTA			21
#define CLK_PORTB			22
#define CLK_PORTC			23
#define CLK_PORTD			24
#define CLK_PORTE			25
#define CLK_UART3			26
#define CLK_BKP				27
#define CLK_RTC				CLK_BKP
#define CLK_DSP_ACODEC			28
#define CLK_PORTF			29
#define CLK_EXTBUS			30
#define CLK_DSP_CRYPTO			31


/* MCU software reset */
__STATIC_INLINE void MCUSoftReset()
{
    //1901BC1x: don't use AIRCR register (0xE000ED0C),
    //RESET signal is not cleared
    //0x05FA - key; SYSRESETREQ[2] = 1 - request reset
    //*((u32*)(0xE000ED0C)) = (0x05FA0000 | (1 << 2));

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
#define DMA_CHNL_ADC1			8  //sreg
#define DMA_CHNL_DSP0_REG		9  //reg
#define DMA_CHNL_ADC2			9  //sreg
#define DMA_CHNL_DSP1_REG		10 //reg
#define DMA_CHNL_TIM1			10 //sreg
#define DMA_CHNL_TIM2			11 //sreg
#define DMA_CHNL_TIM3			12 //sreg
#define DMA_CHNL_SDIO			13 //sreg
#define DMA_CHNL_DSP2			14 //reg
#define DMA_CHNL_DSP0_SREG		14 //sreg
#define DMA_CHNL_DSP3			15 //reg
#define DMA_CHNL_DSP1_SREG		15 //sreg
#define DMA_CHNL_SSP3_TX		16
#define DMA_CHNL_SSP3_RX		17
#define DMA_CHNL_SSP4_TX		18
#define DMA_CHNL_SSP4_RX		19
#define DMA_CHNL_UART3_TX		20
#define DMA_CHNL_UART3_RX		21
#define DMA_CHNL_DSP_ACODEC_ADC		22 //reg
#define DMA_CHNL_DSP_ACODEC_DAC		23 //reg
#define DMA_CHNL_DSP_CRYPTO		24 //reg
#define DMA_CHNL_DSP_TIMER		25
#define DMA_CHNL_DSP_BSP1_TX		26
#define DMA_CHNL_DSP_BSP1_RX		27
#define DMA_CHNL_DSP_BSP2_TX		28
#define DMA_CHNL_DSP_BSP2_RX		29
#define DMA_CHNL_DSP_BSP3_TX		30
#define DMA_CHNL_DSP_BSP3_RX		31

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
 * DSP core definitions and functions prototypes
 *===========================================================================*/

//Start ARM address of DSP code memory
#define DSP_CODE_ARM_ADDR       0x30020000
//Start ARM address of DSP data memory (include DSP core/peripheral registers)
#define DSP_DATA_ARM_ADDR       0x30000000

//DSP code maximum size, 16-bit words (without DSP ISR table):
#define DSP_CODE_MAX_SIZE       (0x10000 - 128)
//DSP data maximum size, 16-bit words (without DSP core/peripheral registers):
#define DSP_DATA_MAX_SIZE       (0x10000 - 128)

//Convert DSP code address to ARM address
#define ARM_CODE_ADDR(dspAddr)	(DSP_CODE_ARM_ADDR + ((dspAddr) << 1))

//Convert DSP data address to ARM address
#define ARM_DATA_ADDR(dspAddr)	(DSP_DATA_ARM_ADDR + ((dspAddr) << 1))

/* Wait while operation on DSP bridge is not completed
   wait for BRTRD[14] = 1 - operations completed on ARM/DSP bridge */
#define DSPWaitOpCompleted() \
	while((RST_CLK->DSP_CONTROL_STATUS & PN14) == 0);

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

//ADC samples: 12-bit unsigned integer values
#define ADC_MAXVALUE            0x0FFF
#define ADC_HALFVALUE           0x0800
#define ADC_MINVALUE            0x0000
#define ADC_VALMASK             0x0FFF
#define ADC_VALSIZE             12    
#define ADC_REF_VOLTAGE         3.25  

/*===========================================================================
 * DAC definitions
 * Remarks:
 * 1. MC DAC has 2 channels
 *===========================================================================*/

#define DAC_COUNT               1         //Count of DACs
#define DAC_CHNL_COUNT          2         //Count of DAC channels

//DAC samples: 12-bit unsigned integer values
#define DAC_MAXVALUE            0x0FFF
#define DAC_HALFVALUE           0x0800
#define DAC_MINVALUE            0x0000
#define DAC_VALMASK             0x0FFF
#define DAC_VALSIZE             12        

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
#define COMP_MAP_OUT_C0         1 //Out -> C0  (MODE = 1 - basic)
#define COMP_IVREF              1.25F //DON'T CHANGE! Internal Vref, V

/*===========================================================================
 * SSP definitions
 * Remarks:
 * 1. MC has 4 SSPs
 *===========================================================================*/

//Count of SSP blocks
#define SSP_COUNT               4

//Maximum SSP speed, bit/s
#define SSP_MAX_SPEED           25000000

//FIFO length
#define SSP_FIFO_RX_LENGTH      8 //Input FIFO length
#define SSP_FIFO_TX_LENGTH      8 //Output FIFO length

//SSP1 control mapping
#define SSP1_MAP_CTRL_A15A14    1 //SSP1 FSS/CLK -> A15/A14 (MODE = 2 - alter)
#define SSP1_MAP_CTRL_D3D5		2 //SSP1 FSS/CLK -> D3/D5   (MODE = 2 - alter)
#define SSP1_MAP_CTRL_E15E14    3 //SSP1 FSS/CLK -> E15/E14 (MODE = 2 - alter)

//SSP2 control mapping
#define SSP2_MAP_CTRL_A9A8      1 //SSP2 FSS/CLK -> A9/A8   (MODE = 2 - alter)
#define SSP2_MAP_CTRL_B13B12    2 //SSP2 FSS/CLK -> B13/B12 (MODE = 2 - alter)
#define SSP2_MAP_CTRL_D12D14	3 //SSP2 FSS/CLK -> D12/D14 (MODE = 2 - alter)
#define SSP2_MAP_CTRL_E5E7		4 //SSP2 FSS/CLK -> E5/E7   (MODE = 2 - alter)

//SSP3 control mapping
#define SSP3_MAP_CTRL_C9C11		1 //SSP3 FSS/CLK -> C9/C11  (MODE = 2 - alter)
#define SSP3_MAP_CTRL_F13F15	2 //SSP3 FSS/CLK -> F13/F15 (MODE = 2 - alter)

//SSP4 control mapping
#define SSP4_MAP_CTRL_C0C5      1 //SSP4 FSS/CLK -> C0/C5   (MODE = 2 - alter)
#define SSP4_MAP_CTRL_C12C13	2 //SSP4 FSS/CLK -> C12/C13 (MODE = 2 - alter)
#define SSP4_MAP_CTRL_E0E1		3 //SSP4 FSS/CLK -> E0/E1   (MODE = 2 - alter)
#define SSP4_MAP_CTRL_F5F4		4 //SSP4 FSS/CLK -> F5/F4   (MODE = 2 - alter)

//SSP1 Rx/Tx mapping
#define SSP1_MAP_RXTX_A12A13    1 //SSP1 Rx/Tx -> A12/A13 (MODE = 2 - alter)
#define SSP1_MAP_RXTX_D4D2		2 //SSP1 Rx/Tx -> D4/D2   (MODE = 2 - alter)
#define SSP1_MAP_RXTX_E12E13    3 //SSP1 Rx/Tx -> E12/E13 (MODE = 2 - alter)

//SSP2 Rx/Tx mapping
#define SSP2_MAP_RXTX_A11A10	1 //SSP2 Rx/Tx -> A11/A10 (MODE = 2 - alter)
#define SSP2_MAP_RXTX_B15B14	2 //SSP2 Rx/Tx -> B15/B14 (MODE = 2 - alter)
#define SSP2_MAP_RXTX_D13D15	3 //SSP2 Rx/Tx -> D13/D15 (MODE = 2 - alter)
#define SSP2_MAP_RXTX_E6E4		4 //SSP2 Rx/Tx -> E6/E4   (MODE = 2 - alter)

//SSP3 Rx/Tx mapping
#define SSP3_MAP_RXTX_C10C8		1 //SSP3 Rx/Tx -> C10/C8  (MODE = 2 - alter)
#define SSP3_MAP_RXTX_F14F12	2 //SSP3 Rx/Tx -> F14/F12 (MODE = 2 - alter)

//SSP4 Rx/Tx mapping
#define SSP4_MAP_RXTX_C3C4		1 //SSP4 Rx/Tx -> C3/C4   (MODE = 2 - alter)
#define SSP4_MAP_RXTX_C15C14	2 //SSP3 Rx/Tx -> C15/C14 (MODE = 2 - alter)
#define SSP4_MAP_RXTX_E3E2		3 //SSP3 Rx/Tx -> E3/E2   (MODE = 2 - alter)
#define SSP4_MAP_RXTX_F2F3		4 //SSP3 Rx/Tx -> F2/F3   (MODE = 2 - alter)

/*===========================================================================
 * UART definitions
 * Remarks:
 * 1. MC has 3 UARTs
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
#define UART_COUNT               3

//FIFO length
#define UART_FIFO_RX_LENGTH      16 //Input FIFO length
#define UART_FIFO_TX_LENGTH      16 //Output FIFO length

//UARTx control pins mapping
#define UARTx_MAP_CTRL_NONE		0 //Not use control i/o

//UART1 Rx/Tx pins mapping
#define UART1_MAP_RXTX_A6A7		1 //UART1 Rx/Tx -> A6/A7 (MODE = 2 - alter)
#define UART1_MAP_RXTX_C2C1		2 //UART1 Rx/Tx -> C2/C1 (MODE = 2 - alter)
#define UART1_MAP_RXTX_E5E4		3 //UART1 Rx/Tx -> E5/E4 (MODE = 3 - remap)

//UART2 Rx/Tx pins mapping
#define UART2_MAP_RXTX_B3B0		1 //UART2 Rx/Tx -> B3/B0 (MODE = 2 - alter)
#define UART2_MAP_RXTX_C7C6		2 //UART2 Rx/Tx -> C7/C6 (MODE = 3 - remap)
#define UART2_MAP_RXTX_E11E10	3 //UART2 Rx/Tx -> E11/E10 (MODE = 2 - alter)

//UART3 Rx/Tx pins mapping
#define UART3_MAP_RXTX_D8D7     1 //UART3 Rx/Tx -> D8/D7 (MODE = 2 - alter)
#define UART3_MAP_RXTX_F0F1		2 //UART3 Rx/Tx -> F0/F1 (MODE = 2 - alter)
#define UART3_MAP_RXTX_D14D13	3 //UART3 Rx/Tx -> D14/D13 (MODE = 1 - basic)

//Minimum valid uart clock, Hz
#define UART_MINCLK              1000000

/*===========================================================================
 * Audio codec definitions
 *===========================================================================*/

#define ACODEC_COUNT				1
 
//Input/output FIFO length
#define ACODEC_FIFO_RX_LENGTH      	16
#define ACODEC_FIFO_TX_LENGTH      	16

//ACodec DAC samples: 16-bit signed integer values
#define ACODEC_DAC_MAXVALUE         0x7FFF    
#define ACODEC_DAC_HALFVALUE        0x0000    
#define ACODEC_DAC_MINVALUE         0x8000    
#define ACODEC_DAC_VALMASK          0xFFFF    
#define ACODEC_DAC_VALSIZE          16        

//ACodec ADC samples: 16-bit signed integer values
#define ACODEC_ADC_MAXVALUE         0x7FFF
#define ACODEC_ADC_HALFVALUE        0x0000
#define ACODEC_ADC_VALMASK          0x8000
#define ACODEC_ADC_VALSIZE          16  
#define ACODEC_ADC_REF_VOLTAGE      1.5 

//ADC/DAC gain range, dB [-42..20]
#define ACODEC_GAIN_MIN_DB	    ((s16)-42)
#define ACODEC_GAIN_MAX_DB	    ((s16)20)

/*===========================================================================
 * CAN definitions (no CAN)
 *===========================================================================*/
/*===========================================================================
 * I2C definitions
 * Remarks:
 *
 *===========================================================================*/

#define I2C_COUNT               1

//I2C pins mapping
#define I2C1_MAP_C14C15         0 //I2C1 SCK/SDA -> C14/C15 (basic)
#define I2C1_MAP_D0D1           1 //I2C1 SCK/SDA -> D0/D1   (alter)

/*===========================================================================
 * SDIO definitions
 * Remarks:
 *
 *===========================================================================*/

#define SDIO_COUNT               1

//SDIO pins mapping
#define SDIO1_MAP_C8C13           0 //[C8..C13] (basic)
#define SDIO1_MAP_C1C8C13         1 //[C1] (remap); [C8,C10..C13] (basic)
#define SDIO1_MAP_C0C5            2 //[C0..C5] (remap)
#define SDIO1_MAP_A0A5            3 //[A0..A5] (alter)

/*===========================================================================
 * BSP definitions
 * Remarks:
 *
 *===========================================================================*/

#define BSP_COUNT               3

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

#endif /* __1901BC1x_DEFS_H */

/*===========================================================================
 * End of file 1901BC1x_defs.h
 *===========================================================================*/
