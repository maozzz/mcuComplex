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
 *  File CPU.h: MCU core speed setup, ports initialization.
 *
 *===========================================================================*/

#ifndef __CPU_H
#define __CPU_H

#include "types.h"

/*===========================================================================
 * Port configuration definitions
 *===========================================================================*/

//Bit masks
#define PN0                      (1U << 0)
#define PN1                      (1U << 1)
#define PN2                      (1U << 2)
#define PN3                      (1U << 3)
#define PN4                      (1U << 4)
#define PN5                      (1U << 5)
#define PN6                      (1U << 6)
#define PN7                      (1U << 7)
#define PN8                      (1U << 8)
#define PN9                      (1U << 9)
#define PN10                     (1U << 10)
#define PN11                     (1U << 11)
#define PN12                     (1U << 12)
#define PN13                     (1U << 13)
#define PN14                     (1U << 14)
#define PN15                     (1U << 15)
#define PN16                     (1U << 16)
#define PN17                     (1U << 17)
#define PN18                     (1U << 18)
#define PN19                     (1U << 19)
#define PN20                     (1U << 20)
#define PN21                     (1U << 21)
#define PN22                     (1U << 22)
#define PN23                     (1U << 23)
#define PN24                     (1U << 24)
#define PN25                     (1U << 25)
#define PN26                     (1U << 26)
#define PN27                     (1U << 27)
#define PN28                     (1U << 28)
#define PN29                     (1U << 29)
#define PN30                     (1U << 30)
#define PN31                     (1U << 31)
#define PNALL                    ((u32)~0)

//Port enumeration
#define PORT_A                  0
#define PORT_B                  1
#define PORT_C                  2
#define PORT_D                  3
#define PORT_E                  4
#define PORT_F                  5

//Function InitializePortEx, parameter opts:
//Port functions [3..0]
#define PN_PORTIO               (0x00) //Digital port I/O
#define PN_BASIC                (0x01) //Basic function
#define PN_ALTER                (0x02) //Alternative function
#define PN_REMAP                (0x03) //Remap function
#define PN_ANALOG               (0x04) //Analog I/O

//Output power [7..4]
#define PN_INPUT                (0x00 <<  4) //Input
#define PN_PWR_SLOW             (0x01 <<  4) //Slow front
#define PN_PWR_FAST             (0x02 <<  4) //Fast front
#define PN_PWR_FASTEST          (0x03 <<  4) //Very fast front

//Pull up ON/OFF [8]
#define PN_PULLUP               (0x01 <<  8)
#define PN_NOPULLUP             (0x00 <<  8)

//Pull down ON/OFF [9]
#define PN_PULLDOWN             (0x01 <<  9)
#define PN_NOPULLDOWN           (0x00 <<  9)

//Shmit trigger ON/OFF [10]
#define PN_SHMIT                (0x01 << 10)
#define PN_NOSHMIT              (0x00 << 10)

//Open drain / control by driver [12..11]
#define PN_OPENDRAIN            (0x01 << 11)
#define PN_CTRLDRIVER           (0x00 << 11)

//Input filter ON/OFF [13]
#define PN_INFILTER             (0x01 << 13)
#define PN_NOINFILTER           (0x00 << 13)

#ifndef PORT_OPTS_DEFAULT
#define PORT_OPTS_DEFAULT       (PN_PORTIO     | PN_PWR_FAST   |\
                                 PN_NOPULLUP   | PN_NOPULLDOWN |\
                                 PN_NOSHMIT    | PN_CTRLDRIVER |\
                                 PN_NOINFILTER)
#endif /* !PORT_OPTS_DEFAULT */

//Clock sources
#define CLKSRC_HSI              0 //HSI
#define CLKSRC_HSE              1 //HSE

#define CLKSRC_LSE              2 //LSE
#define CLKSRC_LSI              3 //LSI
#define CLKSRC_HSI_DIRECT       4 //HSI directly without PLL and Div blocks


//CPU option identifiers. (CPUSetOption/CPUGetOption functions)
//Gets/sets CPU speed
#define CPU_OID_SPEED			0x01
//Gets CPU ID
#define CPU_OID_CPUID			0x02

//Port option identifiers. (PORTSetOption/PORTGetOption functions)
/* Set/Get port function.
   In  value: function (PN_ANALOG, PN_PORTIO, e.t.c)
   Out value: function for all pins at port */
#define PORT_OID_FUNC			0x01

/* Set/Get port power.
   In  value: power value (see PN_PWR_x)
   Out value: power for all pins at port */
#define PORT_OID_PWR			0x02

//Next options for set/reset flags
//In value : [0, 48] - reset flag, otherwise flag will set on
//Out value: flag for all pins at selected port
//Set/reset levels on selected pins
#define PORT_OID_RXTX			0x03
//Turn on/off port block
#define PORT_OID_TURN			0x04
//Select output/input mode.
#define PORT_OID_OUTPUT			0x05
//Select digital/analog mode.
#define PORT_OID_DIGITAL		0x06
//Turn on/off pull up
#define PORT_OID_PULLUP			0x07
//Turn on/off pull down
#define PORT_OID_PULLDOWN		0x08
//Turn on/off pull Shmit trigger
#define PORT_OID_SHMIT			0x09
//Turn on/off open drain
#define PORT_OID_OPENDRAIN		0x0A
//Turn on/off input filter
#define PORT_OID_INFILTER		0x0B

/* */
typedef u8                      PORTBLOCK;

/* */
typedef struct tag_PORTOPT
{
	u32 mask;
	u8 value;
} PORTOPT;

/*===========================================================================
 * CPU functions
 *===========================================================================*/

/* Setup CPU core speed */
MCRESULT SetHCLK(u32 hclk, u32 clkSrc);

/* Gets current CPU core speed, Hz */
u32 GetHCLK(void);

/* Gets CPU identifier */
u32 GetCPUID();

/* Indicating that specified clock source is ready */
u8 IsClkSrcReady(u32 clkSrc);

/* Turn ON/OFF specified clock source */
u8 TurnClkSrc(u32 clkSrc, u8 turn);

/* Tune HSI clock source with specified increment value */
s32 TuneClkSrcHSIInc(s32 trimInc);

/* Tune HSI clock source with specified trim value */
s32 TuneClkSrcHSIVal(s32 trimVal);

/* Sets option using specified pin mask, option id and value */
s32 CPUSetOption(u8 cpu, u32 oid, const void *ival);

/* Gets option using specified option identifier */
s32 CPUGetOption(u8 cpu, u32 oid, void *oval);

/*===========================================================================
 * Port functions
 *===========================================================================*/

/* Initialize specified pins at port */
MCRESULT InitializePORTEx(PORTBLOCK port, u32 pinmask, u32 pinval, u32 opts);

/* Turn port */
void TurnPORT(PORTBLOCK port, u8 turn);

/* Sets option using specified pin mask, option id and value.
   Parameter ival is pointer to PORTOPT structure */
s32 PORTSetOption(PORTBLOCK port, u32 oid, const void *ival);

/* Sets option using specified pin mask, option id and value.
   Parameter ival is pointer to integer variable. */
s32 PORTSetOptionE(PORTBLOCK port, PINMASK pins, u32 oid, const void *ival);

/* Gets option using specified option identifier.
   Parameter oval is pointer to integer variable.*/
s32 PORTGetOption(PORTBLOCK port, u32 oid, void *oval);

/* Turn pull up/down for specified port pins */
s32 PORTSetPull(PORTBLOCK port, PINMASK pins, u8 up, u8 down);

/* Read data from port */
s32 PORTRead(PORTBLOCK port, u8 *buffer, s32 count);

/* Write data to port */
s32 PORTWrite(PORTBLOCK port, const u8 *buffer, s32 count);

/*===========================================================================
 * External bus functions
 *===========================================================================*/

//Currently selected memory
#define EXTBUS_ROM        0
#define EXTBUS_RAM        1
#define EXTBUS_NAND       2

typedef u32               EBusConfig;


/* Calculate clock count using specified time interval and core speed */
#define CalcClocks(tm_ns, clkMHz)\
(u32)((float)(tm_ns * clkMHz) * 0.001F + 0.500F)

/* */
MCRESULT InitializeEBus(EBusConfig *cfg);

/* */
void EBusSetClock(u32 ws);

/* */
void EBusSetTime(u32 ws_ns);

/* */
void EBusSetClockEx(u32 addr, u32 wsSetup, u32 wsActive, u32 wsHold);

/* */
void EBusSetTimeEx(u32 addr, u32 wsSetup_ns, u32 wsActive_ns, u32 wsHold_ns);

/* */
void EBusSelectMemory(u32 selmem);

#endif /* __CPU_H */

/*===========================================================================
 * End of file CPU.h
 *===========================================================================*/

