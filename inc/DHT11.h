//
// Created by Alexey Matukhin on 17/12/2019.
//

#ifndef MCUCOMPLEX_DHT11_H
#define MCUCOMPLEX_DHT11_H

#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include "Pin.h"

class DHT11 {
public:
    static DHT11 *dht11;
    void init();
    void request();
    uint8_t result[5];
private:
    Pin *pin;
    uint32_t counter = 0;
    bool readBit();
    uint32_t countLen();
};

#endif //MCUCOMPLEX_DHT11_H
