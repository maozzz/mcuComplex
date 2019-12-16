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
 *  File ETHIO.h: Ethernet configuration definitions.
 *
 *===========================================================================*/

#ifndef __ETHIO_H
#define __ETHIO_H

#include "../../bastypes.h"

//ETH block number
#define ETH_N1          0 //ETH #1
#define ETH_N2          1 //ETH #2
#define ETH_N3          2 //ETH #3
#define ETH_N4          3 //ETH #4
#define ETH_N5          4 //ETH #5
#define ETH_N6          5 //ETH #6
#define ETH_N7          6 //ETH #7
#define ETH_N8          7 //ETH #8

//Ethernet controller options
#define ETH_OPTS_10M                 0x00 //10 Mbit/s
#define ETH_OPTS_100M                0x01 //100 Mbit/s
#define ETH_OPTS_FD                  0x00 //Full duplex
#define ETH_OPTS_HD                  0x02 //Half duplex
#define ETH_OPTS_FX                  0x04 //FX mode
#define ETH_OPTS_AUTONEG             0x08 //Auto negotiation
#define ETH_OPTS_HUB                 0x10 //Hub mode
#define ETH_OPTS_LOW_CONSUMP         0x20 //Low consumption
#define ETH_OPTS_FULL_AUTONEG        0x40 //Full auto negotiation

typedef u8                              ETHBLOCK;

typedef void (*ETHFrameHandler)(void);
typedef ETHFrameHandler                 LPETHFrameHandler;

typedef void (*SETETHMACADDRESS_ROUTINE)(ETHBLOCK eth, u8 *mac);
typedef SETETHMACADDRESS_ROUTINE        LPSETETHMACADDRESS_ROUTINE;

typedef u32  (*ISETHLINKED_ROUTINE)(ETHBLOCK eth);
typedef ISETHLINKED_ROUTINE             LPISETHLINKED_ROUTINE;

typedef void (*TURNETH_ROUTINE)(ETHBLOCK eth, u8 turn);
typedef TURNETH_ROUTINE                 LPTURNETH_ROUTINE;

typedef s32 (*GETOUTSPACE_ROUTINE)(ETHBLOCK eth);
typedef GETOUTSPACE_ROUTINE             LPGETOUTSPACE_ROUTINE;

typedef s32  (*ETHREAD_ROUTINE)(ETHBLOCK eth, u8 *buffer, s32 length);
typedef ETHREAD_ROUTINE                 LPETHREAD_ROUTINE;

typedef s32  (*ETHWRITE_ROUTINE)(ETHBLOCK eth, u8 *buffer, s32 length);
typedef ETHWRITE_ROUTINE                LPETHWRITE_ROUTINE;

/* Represents ethernet controller configuration */
typedef struct tag_ETHConfig
{
        //ADC block (see ETH_Nx)
        ETHBLOCK eth;

        //Options (see ETH_OPTS_x)
        u32 opts;

        //Occurs when frame received
        LPETHFrameHandler               FrameReceived;

        //Set MAC address
        LPSETETHMACADDRESS_ROUTINE      setMAC;

        //Indicates that cable is linked
        LPISETHLINKED_ROUTINE           isLinked;

        //Turn ON/OFF ethernet controller
        LPTURNETH_ROUTINE               turn;

        //Get free space of output buffer
        LPGETOUTSPACE_ROUTINE           getOutSpace;

        //Read data from ethernet controller
        LPETHREAD_ROUTINE               read;

        //Write data to ethernet controller
        LPETHWRITE_ROUTINE              write;

} ETHConfig;

#endif /* __ETHIO_H */

/*===========================================================================
 * End of file ETHIO.h
 *===========================================================================*/

