/*===========================================================================
 *
 *  Hardware library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File evbrd.h: EVALUATION BOARD COMMON DEFINITIONS.
 *                MCU 1986BE91T evaluation board is used.
 *                It's not recommended change any definitions at this file.
 *
 *===========================================================================*/

#ifndef __EVBRD_H
#define __EVBRD_H

#include "../../mc/inc/CPU.h"
#include "../../mc/inc/ADC.h"
#include "../../mc/inc/DAC.h"
#include "../../mc/inc/SSP.h"
#include "../../mc/inc/UART.h"
#include "../evbrds.h"

#define TARGET_EVBRD			EVBRD_1986BE91T

//Evaluation board buttons
#define PORT_BUTTON				PORT_C
#define BUTTON_SELECT           PN10
#define BUTTON_UP               PN11
#define BUTTON_DOWN             PN12
#define BUTTON_LEFT             PN13
#define BUTTON_RIGHT            PN14
#define BUTTON_NONE				0
#define BUTTON_MASK             (PN10|PN11|PN12|PN13|PN14)
#define BUTTON_COUNT			5
#define ReadButton()			(~(PC_Read()) & BUTTON_MASK)

//Evaluation board leds
#define PORT_LED				PORT_D
#define LED_0					PN10 //VD7,  X32
#define LED_1					PN11 //VD8,  X34
#define LED_2					PN12 //VD9,  X36
#define LED_3					PN13 //VD10, X38
#define LED_4					PN14 //VD11, X40
#define LED_MASK                (PN10|PN11|PN12|PN13|PN14)
#define LED_COUNT				5
#define ResetLed(mask)			PD_Reset(mask)
#define SetLed(mask)			PD_Set(mask)

//SD card initial SSP speed, bit/s
#define SDCARD_SSP_SPEED		2000000

//SD card port on evaluation board
#define SDCARD_SSP_NN			SSP_N2
#define SDCARD_SSP_MAP_CTRL		SSP2_MAP_CTRL_D3D5
#define SDCARD_SSP_MAP_RXTX		SSP2_MAP_RXTX_D2D6

//DB9 port on evaluation board
#define DB9_UART_NN				UART_N2
#define DB9_UART_MAP_CTRL		UARTx_MAP_CTRL_NONE  //Don't use control pins
#define DB9_UART_MAP_RXTX		UART2_MAP_RXTX_F0F1  //F0/F1 (DB9 on eval board)

#define AMP_DAC_NN				DAC_N1 //Amplifier

#define RTRIM_ADC_NN			ADC_N1
#define RTRIM_ADC_CHNL_MASK		PN7		//Rtrim ADC channel mask
#define RTRIM_ADC_CHNL_NUM	    7		//Rtrim ADC channel number

#define InitializeEvBrdButtons() InitializePORTEx(PORT_BUTTON, BUTTON_MASK, 0,\
                                    PN_PORTIO   | PN_INPUT       |\
                                    PN_NOPULLUP | PN_NOPULLDOWN  |\
                                    PN_NOSHMIT  | PN_CTRLDRIVER  |\
                                    PN_NOINFILTER);

#define InitializeEvBrdLeds() InitializePORTEx(PORT_LED, LED_MASK, 0,\
                                    PN_PORTIO   | PN_PWR_FAST    |\
                                    PN_NOPULLUP | PN_NOPULLDOWN  |\
                                    PN_NOSHMIT  | PN_CTRLDRIVER  |\
                                    PN_NOINFILTER)

#endif /* !__EVBRD_H */

/*===========================================================================
 * End of file evbrd.h
 *===========================================================================*/
