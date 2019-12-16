//
// Created by Alexey Matukhin on 16/12/2019.
//

#include "I2C.h"


void I2C::init() {
    RST_CLK_PCLKcmd(RST_CLK_PCLK_I2C, ENABLE);
    MDR_PORTC->ANALOG = 0xFFFFFFFF;   // PortC is digital
    MDR_PORTC->FUNC = (0x02 << 0) | (0x02 << 2);// PC0 & PC1 are for I2C
    MDR_PORTC->PWR = 0xFFFFFFFF;
    MDR_I2C->PRL = 0x20;// 80 000 000 Hz / 100 000 Hz = 800 ( 0x320)
    MDR_I2C->PRH = 0x3;// ........................
    MDR_I2C->CTR = (1 << 7);//I2C block enable
    while (1) {
        // I2C WRITE
        MDR_I2C->CMD = (1 << 7) | (1 << 4);//START bit SET,WR bit set
        MDR_I2C->CMD = (0 << 7) | (1 << 4);//START bit RESET
        MDR_I2C->TXD = 0b10010000; //CONTROL BYTE ( as an example)
        MDR_I2C->CMD = (1 << 4);//WR bit set
        while (((MDR_I2C->STA) & 0x02) != 0) {}//wait for T.PROG
        MDR_I2C->TXD = 0b00000000; // address of high byte( as an example)
        MDR_I2C->CMD = (1 << 4);//WR bit set
        while (((MDR_I2C->STA) & 0x80) == 0) {}//wait for ACK


        // I2C READ
        MDR_I2C->CMD = (1 << 7) | (1 << 5);//START bit SET
        MDR_I2C->CMD = (0 << 7) | (1 << 4);//START bit RESET
        MDR_I2C->TXD = 0b10010000; //CONTROL BYTE ( as an example)
        MDR_I2C->CMD = (1 << 5);//WR bit set
        while (((MDR_I2C->STA) & 0x02) != 0) {}//wait for T.PROG
        volatile uint32_t i = MDR_I2C->RXD;
        while (((MDR_I2C->STA) & 0x02) != 0) {}//wait for T.PROG
        volatile uint32_t d = MDR_I2C->RXD;
        MDR_I2C->CMD = 1 << 6;
        __NOP();
        asm("nop");
    }
}

extern "C" void i2cTask(void *arg) {
    I2C *i2c = new I2C();
    i2c->init();

    while (1) portYIELD();
}