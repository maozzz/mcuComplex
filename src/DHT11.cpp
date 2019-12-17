//
// Created by Alexey Matukhin on 17/12/2019.
//

#include <Uart.h>
#include <cstdio>
#include "DHT11.h"

DHT11 *DHT11::dht11 = new DHT11();

void DHT11::init() {
    pin = new Pin(MDR_PORTB, PORT_Pin_10, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT,
                  PIN_PULL_UP);
}

void DHT11::request() {
    pin->oe(PORT_OE_OUT);
    pin->switchOff();
    vTaskDelay(18);

    vPortEnterCritical();
    pin->switchOn();
    pin->oe(PORT_OE_IN);
    while (pin->read());
    while (!pin->read()); // Ждем пока поднимется.

    result[0] = result[1] = result[2] = result[3] = result[4] = 0;
    for (int i = 0; i < 40; i++) {
        result[i / 8] |= readBit() << (7 - i % 8);
    }
    vPortExitCritical();
}

bool DHT11::readBit() {
    pin->oe(PORT_OE_IN);
    if (pin->read()) {
        while (pin->read());
    }
    while (!pin->read());
    return countLen() > 40;
}

uint32_t DHT11::countLen() {
    counter = 0;
    while (pin->read()) counter++;
    return counter;
}