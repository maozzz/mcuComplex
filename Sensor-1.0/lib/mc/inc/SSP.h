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
 *  File SSP.h: SSP interface definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SSP_H
#define __SSP_H

#include "types.h"
#include "CPU.h"

#ifndef __SSP_MODULE_VERSION
#define __SSP_MODULE_VERSION            1
#endif

//SSP blocks
#define SSP_N1                          0 //SSP1
#define SSP_N2                          1 //SSP2
#define SSP_N3                          2 //SSP3
#define SSP_N4                          3 //SSP4

//SSP protocols
#define SSP_PROTO_SPI                   0 //SPI (Motorola)
#define SSP_PROTO_SSI                   1 //SSI (Texas Instruments)
#define SSP_PROTO_MICROWAVE             2 //Microwave (National Semiconductor)

//SSP mode (master/slave)
#define SSP_MODE_MASTER                 0
#define SSP_MODE_SLAVE                  1

//SSP basic options
#define SSP_OPTS_LOOPBACK               0x40000000 //Enable loopback (test SSP)

//SSP protocol specific options
//SPI protocol
//Clock line state at standby
#define SSP_OPTS_SPI_POLHIGH            0x00000001 //high level
#define SSP_OPTS_SPI_POLLOW             0x00000000 //low level
//Data register
#define SSP_OPTS_SPI_PHASE2             0x00000002 //by second edge
#define SSP_OPTS_SPI_PHASE1             0x00000000 //by first edge

//Options           Register data by
//POLLOW  + PHASE1: front edge
//POLLOW  + PHASE2: back edge
//POLHIGH + PHASE1: back edge
//POLHIGH + PHASE2: front edge

//SSP option identifiers
#define SSP_OID_TURN					0x01
#define SSP_OID_PROTO					0x02
#define SSP_OID_MODE					0x03
#define SSP_OID_SPEED					0x04
#define SSP_OID_DATABITS				0x05
#define SSP_OID_POLARITY				0x06
#define SSP_OID_PHASE					0x07
#define SSP_OID_LOOPBACK				0x08


typedef u8                              SSPBLOCK;

/* Represents SSP configuration */
typedef struct tag_SSPConfig
{
        //SSP block (see SSP_Nx)
        SSPBLOCK ssp;

        //SSP protocol (see SSP_PROTO_x)
        u8 proto;

        //Mode: (see SSP_MODE_x)
        u8 mode;

        //Data bits: [4..16]
        u8 dataBits;

        //Bit/s (see SSP_MAX_SPEED)
        u32 speed;

        //SSP options (see SSP_OPTS_x)
        u32 opts;

        //Rx data handler
        void (*received)(void);

        #if (__SSP_MODULE_VERSION == 2)
        //Tx data requested
        void (*transmitted)(void);
        #endif

        //Mapping control (see SSPx_MAP_CTRL_x)
        u16 mapCtrl;

        //Mapping Rx/Tx (see SSPx_MAP_RXTX_x)
        u16 mapRxTx;
} SSPConfig;

/* Initialize SSP controller */
MCRESULT InitializeSSP(const SSPConfig *cfg);

/* Set SSP speed */
MCRESULT SSPSetSpeed(SSPBLOCK ssp, u32 speed);

/* Sets option using specified option id and value */
s32 SSPSetOption(SSPBLOCK port, u32 oid, const void *ival);

/* Gets option using specified option identifier */
s32 SSPGetOption(SSPBLOCK port, u32 oid, void *oval);

/* Get SSP clock speed */
u32 GetSSPClk(SSPBLOCK ssp);

/* Get SSP bit speed */
u32 GetSSPSpeed(SSPBLOCK ssp);

/* Turn ON/OFF selected SSP */
void TurnSSP(SSPBLOCK ssp, u8 turn);

/* Turn ON/OFF transmitter of selected SSP */
void TurnSSPTx(SSPBLOCK ssp, u8 turn);

/* Turn SSP loopback */
void TurnSSPLB(SSPBLOCK ssp, u8 on);

/* Wait while SSP is busy */
void SSPWaitNotBusy(SSPBLOCK ssp);

/* Indicating that input fifo is empty */
u8 IsSSPInFifoEmpty(SSPBLOCK ssp);

/* Indicating that input fifo is full */
u8 IsSSPInFifoFull(SSPBLOCK ssp);

/* Indicating that output fifo is empty */
u8 IsSSPOutFifoEmpty(SSPBLOCK ssp);

/* Indicating that output fifo is full */
u8 IsSSPOutFifoFull(SSPBLOCK ssp);

/* Indicates that SSP busy: tx/rx data or tx FIFO not empty */
u8 IsSSPBusy(SSPBLOCK ssp);

/* Fill output fifo cells with specified value */
void SSPInitOutFifo(SSPBLOCK ssp, u32 v);

/* Flush output data */
void SSPFlush(SSPBLOCK ssp);

/* Read one element from SSP fifo */
s32 SSPReadFifo(SSPBLOCK ssp, u16 *data);

/* Write one element to SSP fifo */
s32 SSPWriteFifo(SSPBLOCK ssp, u16 data);

/* Read data from SSP */
s32 SSPRead(SSPBLOCK ssp, void *dest, s32 count);

/* Write data to SSP */
s32 SSPWrite(SSPBLOCK ssp, const void *src, s32 count);

#endif /* __SSP_H */

/*===========================================================================
 * End of file SSP.h
 *===========================================================================*/



