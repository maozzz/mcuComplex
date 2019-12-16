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
 *  File MILSTD1553.h: MILSTD-1553B (GOST R 52070-2003) definitions
 *                     and function prototypes.
 *
 *===========================================================================*/

#ifndef __MILSTD1553_H
#define __MILSTD1553_H

#include "types.h"
#include "CPU.h"

//MILSTD number
#define MILSTD_N1       0
#define MILSTD_N2       1

//MILSTD modes
//Master (Bus Controller, BC)
#define MILSTD_MODE_MASTER                 0
//Slave device
#define MILSTD_MODE_SLAVE                  1
//Unaddressed monitor
#define MILSTD_MODE_MONITOR                2

//Channel (primary/secondary)
#define MILSTD_MODE_PRIMARY                (0 << 2)
#define MILSTD_MODE_SECONDARY              (1 << 2)

//Additional options
//Disable auto-tuning of the middle of the bit interval
#define MILSTD_MODE_NOAUTOTUNE             (1 << 15)
//Filter NRZ stream
#define MILSTD_MODE_ENFILTER               (1 << 14)
//Invert PRD_PRMx signals
#define MILSTD_MODE_INVTR                  (1 << 13)
//Keeps error bits (clears by reset MILSTD controller),
//otherwise error bits cleared when controller going to idle
#define MILSTD_MODE_KEEPERR                (1 << 12)

//MILSTD commands
//No data words (K=1)
#define MILSTD_CMD_BUSCTRL          0x00 //Dynamic bus control
#define MILSTD_CMD_SYNC             0x01 //Synchronize (no data word)
#define MILSTD_CMD_GETSTATUS        0x02 //Transmit status word
#define MILSTD_CMD_SELFTEST         0x03 //Initiate self-test
#define MILSTD_CMD_TRMOFF           0x04 //Transmitter shutdown
#define MILSTD_CMD_TRMON            0x05 //Override transmitter shutdown
#define MILSTD_CMD_TERMBITOFF       0x06 //Inhibit terminal flag bit
#define MILSTD_CMD_TERMBITON        0x07 //Override inhibit terminal flag bit
#define MILSTD_CMD_RESET            0x08 //Reset remote terminal
//With data words
#define MILSTD_CMD_GETVECWORD       0x10 //Transmit (request) vector word K=1 word<-slave
#define MILSTD_CMD_TXVECWORD        0x11 //Synchronize (with data word)   K=0 word->slave
#define MILSTD_CMD_GETLASTCMD       0x12 //Transmit last command          K=1 word<-slave
#define MILSTD_CMD_GETBITWORD       0x13 //Transmit bit word              K=1 word<-slave
#define MILSTD_CMD_SELTRMOFF        0x14 //Selected transmitter shutdown          K=0
#define MILSTD_CMD_SELTRMON         0x15 //Override selected transmitter shutdown K=0

#define MILSTD_CMD_DATA_RECEIVED    0x80 //Data received (from master or slave)
#define MILSTD_CMD_DATA_TXCOMPLETED 0x81 //Data transmission completed (to master or slave)
#define MILSTD_CMD_FAIL             0xFF //Error during transaction


//MILSTD status word flags
#define MILSTD_STS_MSG_ERROR        (1 << 10) //Message error
#define MILSTD_STS_TRAN_RSPWORD     (1 <<  9) //Transmit response word
#define MILSTD_STS_REQ_MNTANCE      (1 <<  8) //Request to maintenance
#define MILSTD_STS_RECV_GRPCMD      (1 <<  4) //Received group command
#define MILSTD_STS_ABON_BUSY        (1 <<  3) //Abonent busy
#define MILSTD_STS_ABON_FAIL        (1 <<  2) //Abonent fail
#define MILSTD_STS_ACPT_BUSCTRL     (1 <<  1) //Accepted bus control
#define MILSTD_STS_SLAVE_FAIL       (1 <<  0) //Slave device fail


//MILSTD error flags
#define MILSTD_ERR_INVWORD          (1 << 6) //Invalid word
#define MILSTD_ERR_BREAK            (1 << 5) //Message breaked
#define MILSTD_ERR_NOGAP            (1 << 4) //No 4us pause between command-response
#define MILSTD_ERR_CSYC             (1 << 3) //Rx data sync instead command sync
#define MILSTD_ERR_DSYC             (1 << 2) //Rx command sync instead data sync
#define MILSTD_ERR_PARITY           (1 << 1) //Parity error or invalid count of rx bits
#define MILSTD_ERR_NORECV           (1 << 0) //No response in 14us interval


//Maximum count of data words at message
//(as MILSTD-1553 defined: 32 words = 64 bytes)
#define MILSTD_MSG_DATA_MAXLENGTH   64

typedef u8                          MILSTDBLOCK;

/* Represents MILSTD1553B configuration */
typedef struct tag_MILSTD1553BConfig
{
    //MILSTD block (see MILSTD_Nx)
    MILSTDBLOCK mil;

    //Slave address [0..30] (31 - group address)
    u8 saddr;

    //Mode (see MILSTD_MODE_x)
    u16 mode;

    //Mapping Rx/Tx (see MILSTDx_MAP_RXTX_x)
    u32 mapRxTx;

    //Event handler
    //"saddr" - slave address
    //"cmd"   - command (see MILSTD_CMD_x)
    void (*evhandler)(s32 saddr, s32 cmd);
} MILSTD1553BConfig;


/* Initialize MILSTD1553B controller */
MCRESULT InitializeMILSTD1553B(const MILSTD1553BConfig *cfg);

/* Set mode (see MILSTD_MODE_x) */
void MILSTD1553BSetMode(MILSTDBLOCK mil, u32 mode);

/* Select channel (primary/secondary, master only) */
void MILSTD1553BSelectChannel(MILSTDBLOCK mil, u32 chindex);

/* Turn MILSTD1553B */
void TurnMILSTD1553B(MILSTDBLOCK mil, u8 on);

/* Reset MILSTD1553B */
void ResetMILSTD1553B(MILSTDBLOCK mil);

/* Indicates that transaction in progress (master only) */
s32 IsMILSTDTranInProgress(MILSTDBLOCK mil);


/* Send command to specified slave device */
s32 MILSTD1553BSendCommand(MILSTDBLOCK mil, s32 daddr, s32 command, s32 dataword);

/* Request data from slave */
s32 MILSTD1553BRequestData(MILSTDBLOCK mil, s32 daddr, s32 count);


/* Read data from specified subaddress */
s32 MILSTD1553BRead(MILSTDBLOCK mil, s32 suba, void *dest, s32 count);

/* Write data to internal buffer */
s32 MILSTD1553BWrite(MILSTDBLOCK mil, s32 daddr, const void *src, s32 count);


/* Sets vector word */
void MILSTD1553BSetVecWord(MILSTDBLOCK mil, u32 vword);

/* Gets received vector word */
u32 MILSTD1553BGetVecWord(MILSTDBLOCK mil);


/* Sets status word.
   sword - MILSTD_STS_x flags */
void MILSTD1553BSetStatusWord(MILSTDBLOCK mil, u32 sword);

/* Gets received status word */
u32 MILSTD1553BGetStatusWord(MILSTDBLOCK mil, u32 swordN);


/* Gets last error.
   Returns MILSTD_ERR_x flags */
u32 MILSTD1553BGetError(MILSTDBLOCK mil);

#endif /* !__MILSTD1553_H */

/*===========================================================================
 * End of file MILSTD1553.h
 *===========================================================================*/
