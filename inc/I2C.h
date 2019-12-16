//
// Created by Alexey Matukhin on 16/12/2019.
//

#ifndef MCUCOMPLEX_I2C_H
#define MCUCOMPLEX_I2C_H

#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_i2c.h>
#include <MDR32F9Qx_rst_clk.h>


class I2C {
public:
    void init();
private:

};

extern "C" {
    void i2cTask(void*arg);
}
#endif //MCUCOMPLEX_I2C_H
