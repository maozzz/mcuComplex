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
 *  File UART.h: UART interface definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __UART_H
#define __UART_H

#include "types.h"
#include "CPU.h"

//UART block number
#define UART_N1                 0        //UART1
#define UART_N2                 1        //UART2
#define UART_N3                 2        //UART3

//Uart speed bit/s
#define UART_SPEED_110          110
#define UART_SPEED_300          300
#define UART_SPEED_1200         1200
#define UART_SPEED_2400         2400
#define UART_SPEED_4800         4800
#define UART_SPEED_9600         9600
#define UART_SPEED_19200        19200
#define UART_SPEED_38400        38400
#define UART_SPEED_57600        57600
#define UART_SPEED_115200       115200
#define UART_SPEED_230400       230400
#define UART_SPEED_460800       460800
#define UART_SPEED_921600       921600

//Data bits
#define UART_DATABITS_8         8 //8bits
#define UART_DATABITS_7         7 //7bits
#define UART_DATABITS_6         6 //6bits
#define UART_DATABITS_5         5 //5bits

//Option: parity
#define UART_PARITY_MASK        0x00000007 //Option mask
#define UART_PARITY_NONE        0x00000000 //No parity control
#define UART_PARITY_EVEN        0x00000001 //Parity control
#define UART_PARITY_ODD         0x00000002 //Odd control
#define UART_PARITY_MARK        0x00000003 //Marker (1)
#define UART_PARITY_SPACE       0x00000004 //Space (0)

//Option: stop bits
#define UART_STOPBITS_MASK      0x00000018 //Option mask
#define UART_STOPBITS_1         0x00000000 //1 stop bit
#define UART_STOPBITS_2         0x00000008 //2 stop bits

//Option: stream control
#define UART_CTRL_MASK          0x000000E0  //Option mask
#define UART_NOCTRL             0x00000000  //No control
#define UART_RTS                0x00000020  //RTS enable
#define UART_CTS                0x00000040  //CTS enable

//Other UART options
#define UART_LOOPBACK           0x00000100 //Enable loopback
#define UART_FIFO_DISABLE       0x00000200 //Disable UART FIFO
#define UART_BREAKLINE          0x00000400 //Break line

//Default UART options
#define UART_OPTS_DEFAULT       (UART_PARITY_NONE | UART_STOPBITS_1 | UART_NOCTRL)

//UART option identifiers
#define UART_OID_CLOCK			0x01
#define UART_OID_TURN			0x02
#define UART_OID_TURNTX			0x03
#define UART_OID_TURNRX			0x04
#define UART_OID_SPEED			0x05
#define UART_OID_DATABITS		0x06
#define UART_OID_PARITY			0x07
#define UART_OID_STOPBITS		0x08
#define UART_OID_CTRL			0x09
#define UART_OID_LOOPBACK		0x0A

//Flow control chars
//XON char
#ifndef UART_CHAR_XON
#define UART_CHAR_XON			0x11
#endif /* !UART_CHAR_XON */

//XOFF char
#ifndef UART_CHAR_XOFF
#define UART_CHAR_XOFF			0x13
#endif /* !UART_CHAR_XOFF */


typedef u8                      UARTBLOCK;

/* Represents UART configuration */
typedef struct tag_UARTConfig
{
        //UART block (see UART_Nx)
        UARTBLOCK uart;

        //Data bits count (see UART_DATABITS_x)
        u8 dataBits;

        //Speed (see UART_SPEED_x)
        u32 speed;

        //UART options
        //(see UART_PARITY_x, UART_STOPBITS_x, UART_{RTS|CTS|NOCTRL})
        u32 opts;

        //Pointer to received data handler
        void (*received)(void);

        //Mapping control (see UARTx_MAP_CTRL_x)
        u16 mapCtrl;

        //Mapping Rx/Tx (see UARTx_MAP_RXTX_x)
        u16 mapRxTx;
} UARTConfig;


/* Initialize UART controller */
MCRESULT InitializeUART(const UARTConfig *cfg);

/* Sets option using specified option id and value */
s32 UARTSetOption(UARTBLOCK uart, u32 oid, const void *ival);

/* Gets option using specified option identifier */
s32 UARTGetOption(UARTBLOCK uart, u32 oid, void *oval);

/* Get UART bit speed */
u32 GetUARTSpeed(UARTBLOCK uart);

/* Wait while UART send data */
void UARTWaitNotBusy(UARTBLOCK uart);

/* Set UART speed */
MCRESULT UARTSetSpeed(UARTBLOCK uart, u32 speed);

/* Set UART character framing */
MCRESULT UARTSetCharFraming(UARTBLOCK uart, u8 dataBits, u32 opts);

/* Set UART speed and character framing */
MCRESULT UARTSetSpeedCharFraming(UARTBLOCK uart, u32 speed, u8 dataBits, u32 opts);

/* Turn UART */
void TurnUART(UARTBLOCK uart, u8 turn);

/* Turn UART transmitter */
void TurnUARTTx(UARTBLOCK uart, u8 turn);

/* Turn UART receiver */
void TurnUARTRx(UARTBLOCK uart, u8 turn);

/* Turn UART loopback */
void TurnUARTLB(UARTBLOCK uart, u8 on);

/* Indicating that UART is turned ON */
u8 IsUARTTurn(UARTBLOCK uart);

/* Indicating that input fifo is empty */
u8 IsUARTInFifoEmpty(UARTBLOCK uart);

/* Indicating that input fifo is full */
u8 IsUARTInFifoFull(UARTBLOCK uart);

/* Indicating that output fifo is empty */
u8 IsUARTOutFifoEmpty(UARTBLOCK uart);

/* Indicating that output fifo is full */
u8 IsUARTOutFifoFull(UARTBLOCK uart);

/* Flush output data */
void UARTFlush(UARTBLOCK uart);

/* Read data from UART */
s32 UARTRead(UARTBLOCK uart, void *dest, s32 count);

/* Write data to UART */
s32 UARTWrite(UARTBLOCK uart, const void *src, s32 count);

/* Send XON char */
void UARTSendXON(UARTBLOCK uart);

/* Send XOFF char */
void UARTSendXOFF(UARTBLOCK uart);

#endif /* __UART_H */

/*===========================================================================
 * End of file UART.h
 *===========================================================================*/

