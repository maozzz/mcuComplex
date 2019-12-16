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
 *  File swmodel_ETH.c: Ethernet controller functions.
 *
 *===========================================================================*/

#include "../../inc/ETH.h"

#include <string.h>

//Pointer to user-defined frame handler
static void (*EthFrameReceived)(void) = 0;

/* Initialize Ethernet controller */
MCRESULT InitializeETH(const ETHConfig *cfg)
{
        return MCR_OK;
}



/* Indicates that frame received */
u32 IsFrameReceived()
{      
		return 0;
}



/* Indicates that cable is linked */
u32 IsETHLinked(ETHBLOCK eth)
{
		return 0;
}



/* Get speed */
u32 GetETHSpeed(ETHBLOCK eth)
{        
		return ETH_OPTS_100M;
}



/* Get controller mode */
u32 GetETHMode(ETHBLOCK eth)
{        
		return ETH_OPTS_HD;
}



/* Turn ON/OFF ethernet controller receiver */
void TurnETHRx(ETHBLOCK eth, u8 turn)
{       
}



/* Turn ON/OFF ethernet controller transmitter */
void TurnETHTx(ETHBLOCK eth, u8 turn)
{       
}



/* Turn ON/OFF ethernet controller */
void TurnETH(ETHBLOCK eth, u8 turn)
{
        //EN[15]: 1/0 - enable/disable
        //X_CFG - transmitter; R_CFG - receiver
        if (turn)
        {               
        }
        else
        {         
        }
}



/* Clear input/output buffers */
void ETHClearBuffers(ETHBLOCK eth)
{       
}



/* Clear input buffer */
void ETHClearInBuffer(ETHBLOCK eth)
{       
}



/* Clear output buffer */
void ETHClearOutBuffer(ETHBLOCK eth)
{      
}



/* Set MAC address */
void SetETHMAC(ETHBLOCK eth, u8 *mac)
{
        if (!mac)
                return;      
}



/* Read data from ethernet controller */
s32 ETHRead(ETHBLOCK eth, u8 *buffer, s32 length)
{       
		return 0;
}



/* Write data to ethernet controller */
s32 ETHWrite(ETHBLOCK eth, u8 *buffer, s32 length)
{
		return 0;
}



/* Interrupt handler */
void ETH_IRQHandler(void)
{        
		if (EthFrameReceived)
			EthFrameReceived();
}

/*===========================================================================
 * End of file swmodel_ETH.c
 *===========================================================================*/
