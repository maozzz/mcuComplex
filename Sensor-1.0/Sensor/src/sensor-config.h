/*===========================================================================
 *
 *  Sensor controller
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
 *  File sensor-config.h. Definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __SENSOR_CONFIG_H
#define __SENSOR_CONFIG_H

#include <CPU.h>
#include <Timer.h>
#include <SysTimer.h>
#include <UART.h>
#include <PVD.h>
#include <I2C.h>
#include <IWire.h>
#include <RTC.h>
#include <LCD.h>
#include <evbrds.h>
#include <evbrd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include "sensor.h"

/*===========================================================================
 * HARDWARE DEFINITONS
 *===========================================================================*/

//Timer speed, Hz (must 1000Hz)
#define TIMER_SPEED                (1000)
#define time2ticks(msec)           ((TIMER_SPEED * (msec)) / 1000)

//Requested CPU speed, Hz
#define CPU_SPEED                   80000000

//Requested I2C speed, bit/s
#define I2C_SPEED                   30000

//DB9 port on evbrd
#define UART_NN                     DB9_UART_NN
#define UART_MAP_CTRL               DB9_UART_MAP_CTRL
#define UART_MAP_RXTX               DB9_UART_MAP_RXTX

//Button delay, timer clocks
#define BUTTON_DELAY_INTERVAL       time2ticks(200)

//Sensor configuration
extern const SensorConfig sensorConfig[SENSOR_MAXCOUNT];

/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/

/* Initialize device */
MCRESULT InitializeDevice();

#endif /* __SENSOR_CONFIG_H */

/*===========================================================================
 * End of file sensor-config.h
 *===========================================================================*/
