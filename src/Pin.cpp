//
// Created by Алексей on 03.09.2018.
//

#include "main.h"


Pin::Pin(MDR_PORT_TypeDef *port, uint32_t pin, PORT_OE_TypeDef _oe, PORT_SPEED_TypeDef _speed, PORT_MODE_TypeDef _mode,
         PORT_FUNC_TypeDef _func, PIN_PULL_MODE pullMode) {
    assert_param(IS_PORT_PIN(pin));
    this->port = port;
    this->pin = pin;

    RST_CLK_PCLKcmd(PCLK_BIT(port), ENABLE);
    PORT_StructInit(&initStruct);
    func(_func);
    oe(_oe);
    speed(_speed);
    mode(_mode);
    pull(pullMode);
    init();
}

Pin::Pin(MDR_PORT_TypeDef *port, uint32_t pin) {
    assert_param(IS_PORT_PIN(pin));
    this->port = port;
    this->pin = pin;

    RST_CLK_PCLKcmd(PCLK_BIT(port), ENABLE);
    PORT_StructInit(&initStruct);
}

Pin *Pin::oe(PORT_OE_TypeDef oe) {
    oe == PORT_OE_OUT ? port->OE |= pin : port->OE &= ~pin;
    return this;
}

Pin *Pin::speed(PORT_SPEED_TypeDef speed) {
    uint32_t tempPwr = port->PWR;

    vTaskDelay(10);
    tempPwr &= ~(3 * pin * pin);
    if (speed & 0x1) tempPwr |= pin * pin;
    if (speed & 0x2) tempPwr |= pin * pin * 2;
    port->PWR = tempPwr;
    return this;
}

Pin *Pin::func(PORT_FUNC_TypeDef func) {
    uint32_t tempFunc = port->FUNC;

    tempFunc &= ~(3 * pin * pin);
    if (func & 0x1) tempFunc |= pin * pin;
    if (func & 0x2) tempFunc |= pin * pin * 2;
    port->FUNC = tempFunc;
    return this;
}

Pin *Pin::mode(PORT_MODE_TypeDef mode) {
    if (mode) port->ANALOG |= pin;
    return this;
}

Pin *Pin::pull(PIN_PULL_MODE mode) {
    if (mode == PIN_PULL_DOWN) {
        port->PULL &= ~(pin << 16);
        port->PULL |= pin;
    } else if (mode == PIN_PULL_UP) {
        port->PULL &= ~pin;
        port->PULL |= pin << 16;
    } else {
        port->PULL &= ~pin;
        port->PULL &= ~(pin << 16);
    }
}

bool Pin::read() {
    return (port->RXTX & pin) > 0;
}

Pin *Pin::init() {
    return this;
}

Pin *Pin::switchOn() {
    port->RXTX |= pin;
    return this;
}

Pin *Pin::switchOff() {
    port->RXTX &= ~pin;
    return this;
}

Pin *Pin::toggle() {
    if (PORT_ReadInputDataBit(port, pin) == 0) {
        switchOn();
    } else {
        switchOff();
    }
    return this;
}
