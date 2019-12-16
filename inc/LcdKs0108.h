//
// Created by Alexey Matukhin on 2019-04-02.
//

#ifndef R1_LCDKS0108_H
#define R1_LCDKS0108_H


#define CHIP1 ((uint8_t) 0x01) // Чипы расположены наоборот
#define CHIP2 ((uint8_t) 0x02) // первый справа, второй слева, поэтому меняем их местами
#define CHIP12 (uint8_t) 0x03 // первый справа, второй слева, поэтому меняем их местами
#define DATA 0x80

#include <cstdint>
#include "Pin.h"

class LcdKs0108 {
public:
    void putPixel(uint8_t x, uint8_t y, bool bit);

    void waitForLcdReady(uint8_t state);

    uint8_t readByte(uint8_t state);

    void writeByte(uint8_t data, uint8_t state);

    void setPage(uint8_t page);
    void clear();

    void init(Pin *_DB0, Pin *_DB1, Pin *_DB2, Pin *_DB3, Pin *_DB4, Pin *_DB5, Pin *_DB6, Pin *_DB7, Pin *_E1,
              Pin *_E2, Pin *_RES,
              Pin *_RW, Pin *_CD, Pin *_E);

    void test();
    void test2();

    static LcdKs0108 *glcd;
private:

    Pin *dataPins[8];
    Pin *E1;
    Pin *E2;
    Pin *RES;
    Pin *RW;
    Pin *A0;
    Pin *E;
};

extern "C" {

void glcdTask(void *arg);
}
#endif //R1_LCDKS0108_H