/*===========================================================================
 *
 *  Hardware library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File evbrd.h: EVALUATION BOARD COMMON DEFINITIONS.
 *                MCU 1986BE1T evaluation board is used.
 *                It's not recommended change any definitions at this file.
 *
 *===========================================================================*/

#ifndef __EVBRD_H
#define __EVBRD_H

#include <CPU.h>
#include <ADC.h>
#include <DAC.h>
#include <SSP.h>
#include <UART.h>
#include "../evbrds.h"

//Target evaluation board
#define TARGET_EVBRD		EVBRD_1986BE1T

//Evaluation board buttons
#define PORT_BUTTON		PORT_E
#define BUTTON_SELECT           PN9
#define BUTTON_UP               PN8
#define BUTTON_DOWN             PN15
#define BUTTON_LEFT             PN10
#define BUTTON_RIGHT            PN5
#define BUTTON_BACK		PN11
#define BUTTON_NONE		0
#define BUTTON_MASK             (PN9|PN8|PN15|PN10|PN5|PN11)
#define BUTTON_COUNT		6
#define ReadButton()		(~(PE_Read()) & BUTTON_MASK)

//Evaluation board leds
#define PORT_LED			PORT_D
#define LED_0				PN7
#define LED_1				PN8
#define LED_2				PN9
#define LED_3				PN10
#define LED_4				PN11
#define LED_5				PN12
#define LED_6				PN13
#define LED_7				PN14
#define LED_MASK			(PN7|PN8|PN9|PN10|PN11|PN12|PN13|PN14)
#define LED_COUNT			8
#define ResetLed(mask)			PD_Reset(mask)
#define SetLed(mask)			PD_Set(mask)

//SD card initial SSP speed, bit/s
#define SDCARD_SSP_SPEED		3000000

//SD card port on evaluation board
#define SDCARD_SSP_NN			SSP_N1
#define SDCARD_SSP_MAP_CTRL		SSP1_MAP_CTRL_C8C7
#define SDCARD_SSP_MAP_RXTX		SSP1_MAP_RXTX_C6C5

//DB9 port on evaluation board
//VARIANT 1.
#define DB9_UART_NN			UART_N1
#define DB9_UART_MAP_CTRL		UARTx_MAP_CTRL_NONE
#define DB9_UART_MAP_RXTX		UART1_MAP_RXTX_C4C3

//VARIANT 2.
//#define DB9_UART_NN			UART_N2
//#define DB9_UART_MAP_CTRL		UARTx_MAP_CTRL_NONE
//#define DB9_UART_MAP_RXTX		UART2_MAP_RXTX_D14D13

#define AMP_DAC_NN			DAC_N1 //Amplifier

#define RTRIM_ADC_NN			ADC_N1
#define RTRIM_ADC_CHNL_MASK		PN2    //Rtrim ADC channel mask
#define RTRIM_ADC_CHNL_NUM		2      //Rtrim ADC channel number

#define InitializeEvBrdButtons() InitializePORTEx(PORT_BUTTON, BUTTON_MASK, 0,\
						PN_PORTIO   | PN_INPUT      |\
						PN_NOPULLUP | PN_NOPULLDOWN |\
						PN_NOSHMIT  | PN_CTRLDRIVER |\
						PN_NOINFILTER);

#define InitializeEvBrdLeds() InitializePORTEx(PORT_LED, LED_MASK, 0,\
						PN_PORTIO   | PN_PWR_FAST   |\
						PN_NOPULLUP | PN_NOPULLDOWN |\
						PN_NOSHMIT  | PN_CTRLDRIVER |\
						PN_NOINFILTER)

#endif /* !__EVBRD_H */

/*===========================================================================
 * End of file evbrd.h
 *===========================================================================*/
