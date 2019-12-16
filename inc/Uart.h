//
// Created by Alexey Matukhin on 14/12/2019.
//
#ifndef FREERTOSTEMPLATE_UART_H
#define FREERTOSTEMPLATE_UART_H

#define MAX_MESSAGE_LEN 255

#include <MDR32F9Qx_uart.h>
#include <MDR32F9Qx_rst_clk.h>
#include <MDR32F9Qx_port.h>
#include <FreeRTOS.h>
#include <queue.h>

class Uart {
public:
    static Uart *uart;
    void init(MDR_PORT_TypeDef *tPort, uint16_t tPin, MDR_PORT_TypeDef *rPort, uint16_t rPin, MDR_UART_TypeDef *uartx);
    static void task(void *arg);
    void send(char ch);
    void send(char *str);
    void read();
    static void interrupt();
private:
    uint8_t uid = 0;
    uint8_t counter = 0;
    MDR_UART_TypeDef *uartx;
};

extern "C" {
void UART2_IRQHandler(void);
void uartTask(void *arg);
}

#endif //FREERTOSTEMPLATE_UART_H
