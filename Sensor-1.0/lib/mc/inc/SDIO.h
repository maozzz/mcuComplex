/*===========================================================================
 *
 *  MCU library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File SDIO.h: SDIO controller definitions and function prototypes.
 *
 *===========================================================================*/

#ifndef __SDIO_H
#define __SDIO_H

#include "types.h"
#include "CPU.h"

//SDIO number
#define SDIO_N1	0

//Options (SDIOConfig::opts)
#define SDIO_OPTS_MASTER       0x00

typedef u8 		SDIOBLOCK;

/* Represents BSP configuration */
typedef struct tag_SDIOConfig
{
    //SDIO block number (see SDIO_Nx)
	SDIOBLOCK sdio;

    //Requested clock, Hz
    u32 speed;

    //Options (see SDIO_OPTS_x)
    u32 opts;

    //(see SDIOx_MAP_x)
    u32 mappins;
} SDIOConfig;


//SDIO time counter, msecs
extern volatile u32 sdioTimerMs;


/* Update SDIO timers.
   Call this function with frequency 1000Hz */
__STATIC_INLINE void UpdateSDIOTimers()
{
    sdioTimerMs++;
}


/* Delay specified number of msec */
__STATIC_INLINE void SDIODelay(u32 msec)
{
    u32 tm = sdioTimerMs + msec;
    while( tm > sdioTimerMs) __NOP();
}


/* Initialize SDIO controller */
MCRESULT InitializeSDIO(const SDIOConfig *cfg);

/* Turn SDIO block */
void TurnSDIO(SDIOBLOCK sdio, u8 on);

/* Set SDIO speed (frequency on clock line) */
void SDIOSetSpeed(SDIOBLOCK sdio, u32 speed);

/* Get SDIO speed (frequency on clock line), Hz */
u32 GetSDIOSpeed(SDIOBLOCK sdio);

/* Tx 8 clocks, CMD line at high state */
void SDIOTxClock(SDIOBLOCK sdio);

/* Send command */
s32 SDIOWriteCommand(SDIOBLOCK sdio, u32 idcmd, u32 arg);

/* Read response */
s32 SDIOReadResponse(SDIOBLOCK sdio, u32 *dest, u32 bitlen, u32 tmout);

/* Read specified count of data */
s32 SDIORead(SDIOBLOCK sdio, void *dest, s32 count);

/* Write specified count of data */
s32 SDIOWrite(SDIOBLOCK sdio, const void *src, s32 count);

#endif /* !__SDIO_H */

/*===========================================================================
 * End of file SDIO.h
 *===========================================================================*/
