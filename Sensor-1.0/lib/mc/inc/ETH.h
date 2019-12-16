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
 *  File ETH.h: Ethernet controller definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __ETH_H
#define __ETH_H

#include "types.h"
#include "CPU.h"
#include "ETHIO.h"

/* Initialize Ethernet controller */
MCRESULT InitializeETH(const ETHConfig *cfg);

/* Indicates that frame received */
u32 IsFrameReceived(ETHBLOCK eth);

/* Indicates that cable is linked */
u32 IsETHLinked(ETHBLOCK eth);

/* Get speed */
u32 GetETHSpeed(ETHBLOCK eth);

/* Get controller mode */
u32 GetETHMode(ETHBLOCK eth);

/* Turn ON/OFF ethernet controller receiver */
void TurnETHRx(ETHBLOCK eth, u8 turn);

/* Turn ON/OFF ethernet controller transmitter */
void TurnETHTx(ETHBLOCK eth, u8 turn);

/* Turn ON/OFF ethernet controller */
void TurnETH(ETHBLOCK eth, u8 turn);

/* Set MAC address */
void SetETHMAC(ETHBLOCK eth, u8 *mac);

/* Clear input/output buffers */
void ETHClearBuffers(ETHBLOCK eth);

/* Clear input buffer */
void ETHClearInBuffer(ETHBLOCK eth);

/* Clear output buffer */
void ETHClearOutBuffer(ETHBLOCK eth);

/* Get free space of output buffer */
s32 ETHGetOutBufferSpace(ETHBLOCK eth);

/* Read data from ethernet controller */
s32 ETHRead(ETHBLOCK eth, u8 *buffer, s32 length);

/* Write data to ethernet controller */
s32 ETHWrite(ETHBLOCK eth, u8 *buffer, s32 length);

#endif /* __ETH_H */

/*===========================================================================
 * End of file ETH.h
 *===========================================================================*/

