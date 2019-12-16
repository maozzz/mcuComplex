/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File ARINC429.h: ARINC429 (GOST 18977-79) definitions
 *                   and functions prototypes.
 *
 *===========================================================================*/

#ifndef __ARINC429_H
#define __ARINC429_H

#include "types.h"
#include "CPU.h"

//ARINC429 (receiver/transmitter) channel number
#define ARINC_N1	0 //Channel #1
#define ARINC_N2	1
#define ARINC_N3	2
#define ARINC_N4	3
#define ARINC_N5	4
#define ARINC_N6	5
#define ARINC_N7	6
#define ARINC_N8	7 //Channel #8

//ARINC429 options (ARINC429Config::opts)
//SDI2,SDI1 filter (ARINC429 receiver only)
#define ARINC_SDI_NONE        (0) //Disable SDI detection
#define ARINC_SDI_0           (4) //SDI2=0, SDI1=0
#define ARINC_SDI_1           (5) //SDI2=0, SDI1=1
#define ARINC_SDI_2           (6) //SDI2=1, SDI1=0
#define ARINC_SDI_3           (7) //SDI2=1, SDI1=1
#define ARINC_SDI_MASK        (7) //option mask

//Parity control
#define ARINC_PARITY_NONE     (0 << 4) //No parity control
#define ARINC_PARITY_ODD      (1 << 4) //Odd parity
#define ARINC_PARITY_EVEN     (2 << 4) //Even parity
#define ARINC_PARITY_MASK     (3 << 4) //option mask

//Speed
#define ARINC_SPEED_100000    (0 << 6) //100kbit/s
#define ARINC_SPEED_12500     (1 << 6) //12.5kbit/s
#define ARINC_SPEED_MASK      (3 << 6) //option mask


typedef u8 		ARINCBLOCK;

/* Represents ARINC429 configuration (transmitter/receiver) */
typedef struct tag_ARINC429Config
{
    //ARINC block (see ARINC_Nx)
    ARINCBLOCK arinc;    

    //Mapping Rx or Tx (see ARINCRx_MAP_RX_x, ARINCTx_MAP_TX_x)
    u8 mapRxTx;

    //Options (see ARINC_SDI_x, ARINC_PARITY_x, ARINC_SPEED_x)
    u16 opts;

    //Pointer to data handler
    void (*evhandler)(void);
} ARINC429Config;


/* Initialize ARINC429 transmitter */
MCRESULT InitializeARINC429T(const ARINC429Config *cfg);

/* Initialize ARINC429 receiver */
MCRESULT InitializeARINC429R(const ARINC429Config *cfg);

/* Turn ARINC429 transmitter */
void TurnARINC429T(ARINCBLOCK arn, u8 on);

/* Turn ARINC429 receiver */
void TurnARINC429R(ARINCBLOCK arn, u8 on);

/* Set transmitter speed */
s32 ARINC429TSetSpeed(ARINCBLOCK arn, u32 speed);

/* Set receiver speed */
s32 ARINC429RSetSpeed(ARINCBLOCK arn, u32 speed);

/* Set accepted SDI2,SDI1 (receiver only)
   sdi: one of "ARINC_SDI_x" values */
s32 ARINC429SetSDI(ARINCBLOCK arn, u32 sdi);

/* Set accepted labels (receiver only) */
s32 ARINC429SetLabels(ARINCBLOCK arn, const u8 *labels, s32 count);

/* Indicating that input fifo of specified channel is empty */
u8 IsARINC429InFifoEmpty(ARINCBLOCK arn);

/* Indicating that input fifo of specified channel is full */
u8 IsARINC429InFifoFull(ARINCBLOCK arn);

/* Indicating that output fifo of specified channel is empty */
u8 IsARINC429OutFifoEmpty(ARINCBLOCK arn);

/* Indicating that output fifo of specified channel is full */
u8 IsARINC429OutFifoFull(ARINCBLOCK arn);

/* Read data */
s32 ARINC429Read(ARINCBLOCK arn, void *dest, s32 count);

/* Write data */
s32 ARINC429Write(ARINCBLOCK arn, const void *src, s32 count);

/* Read data word */
s32 ARINC429ReadWord(ARINCBLOCK arn, u32 *word);

/* Write data word */
s32 ARINC429WriteWord(ARINCBLOCK arn, u32 word);

#endif /* __ARINC429_H */

/*===========================================================================
 * End of file ARINC429.h
 *===========================================================================*/
