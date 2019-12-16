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
 *  File CAN.h: CAN interface definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __CAN_H
#define __CAN_H

#include "types.h"
#include "CPU.h"

//CAN block number
#define CAN_N1                 0        //CAN1
#define CAN_N2                 1        //CAN2

//CAN protocols
#define CAN_PROTO_CAN20_A      0
#define CAN_PROTO_CAN20_B      1

//CAN mode
#define CAN_MODE_NORMAL        0        //Receive/transmit
#define CAN_MODE_RECEIVE       1        //Receive only
#define CAN_MODE_SELFTEST      2        //Self test

#define CAN_MAX_SPEED          1000000  //Maximum speed, bit/s


typedef u8                     CANBLOCK;

/* Represents CAN configuration */
typedef struct tag_CANConfig
{
        //CAN block (see UART_Nx)
        CANBLOCK can;

        //CAN proto
        u8 proto;

        //CAN mode
        u8 mode;

        //Data bits count (see CAN_DATABITS_x)
        u8 dataBits;

        //Speed (see CAN_SPEED_x)
        u32 speed;

        //UART options
        //(see CAN_OPTS_x)
        u32 opts;

        //Pointer received data handler
        void (*received)(void);

        //Mapping Rx/Tx (see CANx_MAP_RXTX_x)
        u16 mapRxTx;
} CANConfig;


//Initialize CAN
MCRESULT InitializeCAN(CANConfig *cfg);

//Turn CAN
void TurnCAN(CANBLOCK uart, u8 turn);

//Turn CAN transmitter
void TurnCANTx(CANBLOCK uart, u8 turn);


//Read data from input fifo
u32 CANRead(CANBLOCK can, canrx_t *buffer, u32 count);

//Write data to output fifo
u32 CANWrite(CANBLOCK can, cantx_t *buffer, u32 count);

#endif /* __CAN_H */

/*===========================================================================
 * End of file CAN.h
 *===========================================================================*/

