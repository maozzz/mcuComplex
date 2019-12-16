/*===========================================================================
 *
 *  MCU library
 *
 *  (C) 2010, Phyton
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File 1986BE91.h: Special Function Register (SFR) declarations
 *                   for Milandr 1986BE91x.
 *
 *===========================================================================*/

#ifndef __1986BE91_SFR_H
#define __1986BE91_SFR_H

#include "1986be91_can.h"
#include "1986be91_usb.h"
#include "1986be91_eeprom.h"
#include "1986be91_rst_clk.h"
#include "1986be91_dma.h"
#include "1986be91_uart.h"
#include "1986be91_ssp.h"
#include "1986be91_i2c.h"
#include "1986be91_power.h"
#include "1986be91_wwdg.h"
#include "1986be91_iwdg.h"
#include "1986be91_timer.h"
#include "1986be91_adc.h"
#include "1986be91_dac.h"
#include "1986be91_comp.h"
#include "1986be91_port.h"
#include "1986be91_bkp.h"
#include "1986be91_extbus.h"
#include "1986be91_sys.h"

#include "core_cm3.h"

typedef struct
{
    __io_reg RESERVED0[20];
    __io_reg NAND_CYCLES;
    __io_reg CONTROL;

    #ifdef TARGET_MCUM
    #if (TARGET_MCUM == MCUM_1986BE94x)
    __io_reg RAM_Cycles1;
    __io_reg RAM_Cycles2;
    __io_reg RAM_Cycles3;
    __io_reg RAM_Cycles4;
    #endif /* TARGET_MCUM == MCUM_1986BE94x */
    #endif /* TARGET_MCUM */
} _extbus;

#define BASE_CAN1			  (0x40000000)
#define BASE_CAN2			  (0x40008000)
#define BASE_USB			  (0x40010000)
#define BASE_EEPROM			  (0x40018000)
#define BASE_RST_CLK			  (0x40020000)
#define BASE_DMA			  (0x40028000)
#define BASE_UART1			  (0x40030000)
#define BASE_UART2			  (0x40038000)
#define BASE_SSP1			  (0x40040000)
#define BASE_I2C			  (0x40050000)
#define BASE_POWER			  (0x40058000)
#define BASE_WWDG			  (0x40060000)
#define BASE_IWDG			  (0x40068000)
#define BASE_TIMER1			  (0x40070000)
#define BASE_TIMER2			  (0x40078000)
#define BASE_TIMER3			  (0x40080000)
#define BASE_ADC			  (0x40088000)
#define BASE_DAC			  (0x40090000)
#define BASE_COMP			  (0x40098000)
#define BASE_SSP2			  (0x400A0000)
#define BASE_PORTA			  (0x400A8000)
#define BASE_PORTB			  (0x400B0000)
#define BASE_PORTC			  (0x400B8000)
#define BASE_PORTD			  (0x400C0000)
#define BASE_PORTE			  (0x400C8000)
#define BASE_BKP			  (0x400D8000)
#define BASE_PORTF			  (0x400E8000)
#define BASE_EXTBUS			  (0x400F0000)
#define BASE_SYSCTRL			  (0xE000E000)
#define BASE_SYSTICK			  (0xE000E010)
#define BASE_NVIC			  (0xE000E100)
#define BASE_SYS			  (0xE000ED00)
#define BASE_MPU			  (0xE000ED90)

#define CAN1				  ((_can    *) BASE_CAN1)
#define CAN2				  ((_can    *) BASE_CAN2)
#define USB 				  ((_usb    *) BASE_USB)
#define EEPROM				  ((_eeprom *) BASE_EEPROM)
#define RST_CLK 			  ((_rst_clk*) BASE_RST_CLK)
#define DMA 				  ((_dma    *) BASE_DMA)
#define UART1				  ((_uart   *) BASE_UART1)
#define UART2				  ((_uart   *) BASE_UART2)
#define SSP1				  ((_ssp    *) BASE_SSP1)
#define I2C 				  ((_i2c    *) BASE_I2C)
#define POWER				  ((_power  *) BASE_POWER)
#define WWDG				  ((_wwdg   *) BASE_WWDG)
#define IWDG				  ((_iwdg   *) BASE_IWDG)
#define TIMER1				  ((_timer  *) BASE_TIMER1)
#define TIMER2				  ((_timer  *) BASE_TIMER2)
#define TIMER3				  ((_timer  *) BASE_TIMER3)
#define ADC 				  ((_adc    *) BASE_ADC)
#define DAC 				  ((_dac    *) BASE_DAC)
#define COMP				  ((_comp   *) BASE_COMP)
#define SSP2				  ((_ssp    *) BASE_SSP2)
#define PORTA				  ((_port   *) BASE_PORTA)
#define PORTB				  ((_port   *) BASE_PORTB)
#define PORTC				  ((_port   *) BASE_PORTC)
#define PORTD				  ((_port   *) BASE_PORTD)
#define PORTE				  ((_port   *) BASE_PORTE)
#define BKP 				  ((_bkp    *) BASE_BKP)
#define PORTF				  ((_port   *) BASE_PORTF)
#define EXTBUS				  ((_extbus *) BASE_EXTBUS)
#define SYSCTRL				  ((_sysctrl*) BASE_SYSCTRL)
#define SYSTICK				  ((_systick*) BASE_SYSTICK)
#define NVIC				  ((_nvic   *) BASE_NVIC)
#define SYS		        	  ((_sys    *) BASE_SYS)
#define MPU		        	  ((_mpu    *) BASE_MPU)

#endif /* __1986BE91_SFR_H */

/*===========================================================================
 * End of file 1986BE91.h
 *===========================================================================*/
