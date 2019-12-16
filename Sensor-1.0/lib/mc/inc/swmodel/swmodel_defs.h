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
 *  File swmodel_defs.h: Definitions of MCU software model.
 *
 *===========================================================================*/

#ifndef __SWMODEL_DEFS_H
#define __SWMODEL_DEFS_H

typedef u16 PINMASK;
typedef u16 PINVALUE;

#define __IO			volatile
typedef volatile unsigned int	__io_reg;

//Internal generators
#define FREQ_HSI                8000000  //8MHz HSI
#define FREQ_LSI                40000    //40KHz LSI



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
#define IRQ_I2C         10
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
#define IRQ_UART3       31

#define IRQ_SSP4         1
#define IRQ_DSP_ACODEC	21
#define IRQ_DSP_CORE    24

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
#define CLK_I2C       10
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
#define CLK_UART3     31

#define CLK_DSP_ACODEC	28

/* _dsp_irq structure */
typedef struct
{
    __io_reg IRQ; 		/* AIR: bits [31..16]; DIR: bits [15..0] */
} _dsp_irq;

/* _port structure */
typedef struct {
	__io_reg RXTX;
	__io_reg OE;
	__io_reg FUNC;
	__io_reg ANALOG;
	__io_reg PULL;
	__io_reg PD;
	__io_reg PWR;
	__io_reg GFEN;
} _port;

/* _ssp structure */
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

/* _uart structure */
typedef struct
{
	__io_reg DR;
	__io_reg RSR_ECR;
	__io_reg __dummy0[4];
	__io_reg FR;
	__io_reg __dummy1;
	__io_reg ILPR;
	__io_reg IBRD;
	__io_reg FBRD;
	__io_reg LCR_H;
	__io_reg CR;
	__io_reg IFLS;
	__io_reg IMSC;
	__io_reg RIS;
	__io_reg MIS;
	__io_reg ICR;
	__io_reg DMACR;
} _uart;

/* _rst_clk structure */
typedef struct {
	__io_reg	CLOCK_STATUS;           /*!<Clock Status Register */
	__io_reg	PLL_CONTROL;            /*!<PLL Control Register */
	__io_reg	HS_CONTROL;             /*!<HS Control Register */
	__io_reg	CPU_CLOCK;              /*!<CPU Clock Register */
	__io_reg	USB_CLOCK;              /*!<USB Clock Register */
	__io_reg	ADC_MCO_CLOCK;          /*!<ADC Clock Register */
	__io_reg	RTC_CLOCK;              /*!<RTC Clock Register */
	__io_reg	PER_CLOCK;              /*!<Peripheral Clock Enable Register */
	__io_reg	RESERVED0;
	__io_reg	TIM_CLOCK;              /*!<Timer Clock Register */
	__io_reg	UART_CLOCK;             /*!<UART Clock Register */
	__io_reg	SSP_CLOCK;              /*!<SSP1-SSP3 Clock Register */
	__io_reg	DSP_CLOCK;              /*!<DSP Clock Register */
	__io_reg	SSP_CLOCK2;             /*!<SSP4 Clock Register */
	__io_reg	DSP_CONTROL_STATUS;     /*!<DSP Control Status Register */
} _rst_clk;

/* _dsp_acodec structure */
typedef struct {
	__io_reg POWCTL;                 /*!<AUDIO control register */
	__io_reg ADCCTL;                 /*!<ADC control register */
	__io_reg DACCTL;                 /*!<DAC control register */
	__io_reg MASKCTL;                /*!<IRQ mask register */
	__io_reg IRQFLAG;                /*!<IRQ flag register */
	__io_reg ADCREG;                 /*!<ADC FIFO register */
	__io_reg DACREG;                 /*!<DAC FIFO register */
} _dsp_acodec;

/* _dsp_clk structure */
typedef struct
{
	__io_reg CLKMD;
} _dsp_clk;


typedef struct {
    __io_reg REG_00;
    __io_reg REG_01;
    __io_reg REG_02;
    __io_reg REG_03;
    __io_reg REG_04;
    __io_reg REG_05;
    __io_reg REG_06;
    __io_reg REG_07;
    __io_reg REG_08;
    __io_reg REG_09;
    __io_reg REG_0A;
    __io_reg REG_0B;
    __io_reg REG_0C;
    __io_reg REG_0D;
    __io_reg REG_0E;
    __io_reg REG_0F;
    __io_reg RTC_CNT;
    __io_reg RTC_DIV;
    __io_reg RTC_PRL;
    __io_reg RTC_ALRM;
    __io_reg RTC_CS;
} _bkp;

extern _dsp_acodec  *DSP_ACODEC;

extern _port	    *PORTA;
extern _port	    *PORTB;
extern _port	    *PORTC;
extern _port	    *PORTD;
extern _port	    *PORTE;
extern _port	    *PORTF;

extern _ssp	    *SSP1;
extern _ssp	    *SSP2;
extern _ssp	    *SSP3;
extern _ssp	    *SSP4;

extern _uart	    *UART1;
extern _uart	    *UART2;
extern _uart	    *UART3;

extern _rst_clk	    *RST_CLK;
extern _dsp_irq	    *DSP_IRQ;
extern _dsp_clk     *DSP_CLK;


extern _bkp *BKP;

//Count of ports
#define PORT_COUNT              6

//Write/Read, Set/Reset bit macros
//PORT A
#define PA_Write(value)          PORTA->RXTX = (value)
#define PA_Read()                PORTA->RXTX
#define PA_Set(mask)             PORTA->RXTX |= (mask)
#define PA_Reset(mask)           PORTA->RXTX &= ~(mask)
//PORT B
#define PB_Write(value)          PORTB->RXTX = (value)
#define PB_Read()                PORTB->RXTX
#define PB_Set(mask)             PORTB->RXTX |= (mask)
#define PB_Reset(mask)           PORTB->RXTX &= ~(mask)
//PORT C
#define PC_Write(value)          PORTC->RXTX = (value)
#define PC_Read()                PORTC->RXTX
#define PC_Set(mask)             PORTC->RXTX |= (mask)
#define PC_Reset(mask)           PORTC->RXTX &= ~(mask)
//PORT D
#define PD_Write(value)          PORTD->RXTX = (value)
#define PD_Read()                PORTD->RXTX
#define PD_Set(mask)             PORTD->RXTX |= (mask)
#define PD_Reset(mask)           PORTD->RXTX &= ~(mask)
//PORT E
#define PE_Write(value)          PORTE->RXTX = (value)
#define PE_Read()                PORTE->RXTX
#define PE_Set(mask)             PORTE->RXTX |= (mask)
#define PE_Reset(mask)           PORTE->RXTX &= ~(mask)
//PORT F
#define PF_Write(value)          PORTF->RXTX = (value)
#define PF_Read()                PORTF->RXTX
#define PF_Set(mask)             PORTF->RXTX |= (mask)
#define PF_Reset(mask)           PORTF->RXTX &= ~(mask)

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


#define MAX_HCLK            205000000
#define MAX_VALID_HCLK		205000000


/* Indicates that clock of specified peripheral block is enabled */
#define IsClkEnable(clknum)	(1)

/* Enable clock of specified peripheral block */
#define ClkEnable(clknum)

/* Disable clock of specified peripheral block */
#define ClkDisable(clknum)

/* Enable interrupts from specified peripheral block */
#define IRQEnable(irq)

/* Disable interrupts from specified peripheral block */
#define IRQDisable(irq)

//
#define __IRQH_FATTR

/*===========================================================================
 * EEPROM definitions
 * Remarks:
 * 1. EEPROM CODE: 128K = 32 pages x 4K
 * 2. EEPROM DATA:   4K =  1 page  x 4K
 *===========================================================================*/

//EEPROM CODE BUFFER
extern u8 eeprom_code_buffer[];

//EEPROM DATA BUFFER
extern u8 eeprom_data_buffer[];

//Start address of code eeprom
#define EEPROM_CODE_BASE	    ((u32)eeprom_code_buffer)

//Start address of data eeprom
#define EEPROM_DATA_BASE	    ((u32)eeprom_data_buffer)

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
//No attributes for software model
#if defined(__CC_ARM)
#define __EEPROM_FATTR

#elif defined(__ICCARM__)
#define __EEPROM_FATTR
//#define __EEPROM_FDECL

#elif defined (__CMCARM__)
#define __EEPROM_FATTR
//#define __EEPROM_FATTR

#elif defined(__GNUC__)
#define __EEPROM_FATTR

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
 * SSP
 *===========================================================================*/

#define SSP_FIFO_RX_LENGTH		8
#define SSP_FIFO_TX_LENGTH		8

#define SSP2_MAP_CTRL_D12D14		2
#define SSP2_MAP_RXTX_D13D15		2

#define SSP4_MAP_CTRL_E0E1		3
#define SSP4_MAP_RXTX_E3E2		3

#define SSP1_MAP_CTRL_E15E14    3 //SSP1 FSS/CLK -> E15/E14 (MODE = 2 - alter)
#define SSP1_MAP_RXTX_E12E13    3 //SSP1 Rx/Tx -> E12/E13 (MODE = 2 - alter)

#define SSP2_MAP_CTRL_F12F13    4
#define SSP2_MAP_RXTX_F14F15    4

#define SSP1_MAP_CTRL_B12B13    5 //Control pins mapping (FSS, CLK)
#define SSP1_MAP_RXTX_B14B15    5 //Rx/Tx pins

/*===========================================================================
 * UART
 *===========================================================================*/

#define UART_FIFO_RX_LENGTH      16 //Input FIFO length
#define UART_FIFO_TX_LENGTH      16 //Output FIFO length

//UARTx control pins mapping
#define UARTx_MAP_CTRL_NONE		0 //Not use control i/o
#define UART3_MAP_RXTX_F0F1		2

#define UART1_MAP_RXTX_A6A7     4

//Minimum valid uart clock, Hz
#define UART_MINCLK              1000000


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
 * Audio Codec
 *===========================================================================*/

#define ACODEC_DAC_HALFVALUE		0

//Input/output FIFO length
#define ACODEC_FIFO_RX_LENGTH      	16
#define ACODEC_FIFO_TX_LENGTH      	16

//ADC/DAC gain range, dB [-42..20]
#define ACODEC_GAIN_MIN_DB	    ((s16)-42)
#define ACODEC_GAIN_MAX_DB	    ((s16)20)

/*===========================================================================
 * RTC
 *===========================================================================*/

typedef u32 RTCVALUE;

/*===========================================================================
 * ADC
 *===========================================================================*/

//Count of ADCs
#define ADC_COUNT               1           //Count of ADCs
#define ADC_CHNL_COUNT          1           //Count of ADC channels

#define CLKSRC_HCLK             15          //ADC clock from hclk

#define ADC_MAX_CLOCK           24000000    //Maximum ADC clock: 24MHz
#define ADC_MIN_CLOCK           40000       //Minimum ADC clock: 40KHz

#define ADC_MAXVALUE            0x0FFF      //ADC result 12bit
#define ADC_HALFVALUE           0x0800      //ADC half of 12 dig value
#define ADC_VALMASK             0x0FFF      //ADC value mask
#define ADC_VALSIZE             12          //ADC value size, bits
#define ADC_REF_VOLTAGE         3.3F        //DON'T CHANGE! Internal Vref, V.

extern u16 swmodel_adcch0_value;
void ADC_IRQHandler(void);

/*===========================================================================
 * DAC
 *===========================================================================*/

#define DAC_COUNT               1         //Count of DACs
#define DAC_CHNL_COUNT          2         //Count of DAC channels

#define DAC_MAXVALUE            0x0FFF    //DAC is 12-bit value
#define DAC_HALFVALUE           0x0800    //DAC half of 12-bit value
#define DAC_VALMASK             0x0FFF    //DAC value mask
#define DAC_VALSIZE             12        //DAC value size, bits

extern u16 swmodel_dac0_value;
extern u16 swmodel_dac1_value;

/*===========================================================================
 * Timer
 *===========================================================================*/

void Timer1_IRQHandler(void);

/*===========================================================================
 * Functions
 *===========================================================================*/

void MCUSoftReset();

void __NOP();

#endif /* !__SWMODEL_DEFS_h */

/*===========================================================================
 * End of file swmodel_defs.h
 *===========================================================================*/
