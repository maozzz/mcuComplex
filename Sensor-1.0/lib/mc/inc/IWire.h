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
 *  File IWire.h: 1-Wire interface definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __IWIRE_H
#define __IWIRE_H

#include "types.h"
#include "CPU.h"

//1-Wire blocks
#define IWIRE_N1                        0

//1-Wire options
#define IWIRE_OPTS_MASTER               0
#define IWIRE_OPTS_SLAVE                1

typedef u8 IWIREBLOCK;

//1-Wire common commands
#define IWIRE_CMD_SEARCHROM             0xF0
#define IWIRE_CMD_READROM               0x33
#define IWIRE_CMD_SKIPROM               0xCC
#define IWIRE_CMD_MATCHROM              0x55

//Return codes
//Presence signal detected
#define IWIRE_RESULT_OK                  0
//No presence signal (no devices on wire)
#define IWIRE_RESULT_NOPRESENCE         -1
//Short curcuit detected
#define IWIRE_RESULT_SHORTCURCUIT       -2
//CRC error
#define IWIRE_RESULT_CRCERROR           -3

/* */
typedef struct tag_IWireConfig
{
    //Block #
    IWIREBLOCK iwire;

    //Options (see IWIRE_OPTS_x)
    u8 opts;

    //Pin mapping
    u16 mapping;

    //Event handler
    void (*evhandler)(void);
} IWireConfig;


/* Initialize 1-Wire */
MCRESULT InitializeIWire(const IWireConfig *cfg);

/* Turn 1-Wire ON/OFF */
void TurnIWire(IWIREBLOCK iwire, u8 on);

/* Transmit "reset" and try detect "presence" singal.
   Returns: one of "IWIRE_RESULT_x" values */
s32 IWireResetPresence(IWIREBLOCK iwire);

/* Transmit one byte */
s32 IWireWriteByte(IWIREBLOCK iwire, u8 val);

/* Read one byte */
u8 IWireReadByte(IWIREBLOCK iwire);

/* Transmit specified count of bytes */
s32 IWireWrite(IWIREBLOCK iwire, const void *src, s32 n);

/* Read specified count of bytes */
s32 IWireRead(IWIREBLOCK iwire, void *dest, s32 n);

/* Scan 1-Wire bus (search ROMs) */
s32 IWireScanBus(IWIREBLOCK iwire, u8 *roms, s32 maxcount);

#endif /* __IWIRE_H */

/*===========================================================================
 * End of file IWire.h
 *===========================================================================*/
