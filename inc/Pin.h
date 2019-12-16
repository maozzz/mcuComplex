//
// Created by Алексей on 03.09.2018.
//

#ifndef R1_PINOUT_H
#define R1_PINOUT_H

typedef enum {
    PIN_PULL_NONE = 0x0,
    PIN_PULL_UP = 0x1,
    PIN_PULL_DOWN = 0x2
} PIN_PULL_MODE;

#include <MDR32F9Qx_config.h>
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>

class Pin {

public:
    Pin(MDR_PORT_TypeDef *port, uint32_t pin);

    Pin(MDR_PORT_TypeDef *port, uint32_t pin, PORT_OE_TypeDef oe, PORT_SPEED_TypeDef speed, PORT_MODE_TypeDef mode,
        PORT_FUNC_TypeDef func, PIN_PULL_MODE pullMode);

    Pin *switchOn();

    Pin *switchOff();

    bool read();

    Pin *toggle();

    Pin *oe(PORT_OE_TypeDef oe);

    Pin *speed(PORT_SPEED_TypeDef speed);

    Pin *mode(PORT_MODE_TypeDef mode);

    Pin *func(PORT_FUNC_TypeDef func);

    Pin *init();

    Pin *pull(PIN_PULL_MODE mode);

private:
    PORT_InitTypeDef initStruct;
    MDR_PORT_TypeDef *port;
    uint32_t pin;
};


#endif //R1_PINOUT_H
