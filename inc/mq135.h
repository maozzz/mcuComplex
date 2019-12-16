//
// Created by Alexey Matukhin on 16/12/2019.
//

#ifndef MCUCOMPLEX_MQ135_H
#define MCUCOMPLEX_MQ135_H

#include <cstdio>
#include <FreeRTOS.h>
#include <task.h>
#include <projdefs.h>
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_adc.h"
#include "MDR32F9Qx_rst_clk.h"
#include <portmacro.h>
#include "Uart.h"

void mq135Task(void *arg);


#endif //MCUCOMPLEX_MQ135_H
