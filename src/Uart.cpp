//
// Created by Alexey Matukhin on 14/12/2019.
//

#include <string>
#include "Uart.h"

Uart *Uart::uart = new Uart();
extern "C" void Uart::task(void *arg) {
    Uart::uart->init(MDR_PORTF, PORT_Pin_1, MDR_PORTF, PORT_Pin_0, MDR_UART2);
    // Бесконечный цикл
    while (1) {
        Uart::uart->send(Uart::uart->uid / 100 + '0');
        Uart::uart->send((Uart::uart->uid % 100) / 10 + '0');
        Uart::uart->send(Uart::uart->uid % 10 + '0');
        Uart::uart->uid++;
        Uart::uart->send(": Maoz's MCU over UART!\n");
        vTaskDelay(10000);// Задержка
    }
}


void Uart::send(char ch) {
    this->uartx->DR = ch;
    while (MDR_UART2->FR & UART_FLAG_TXFF) {
        portYIELD()
    }
}

void Uart::send(char *str) {
    for (counter = 0; counter < 255; counter++) {
        if (str[counter] == '\0') break;
        send(str[counter]);
    }
}

void Uart::read() {
    while (!(MDR_UART2->FR & UART_FR_RXFE)) {
        Uart::uart->send(MDR_UART2->DR);
    }
}

void Uart::interrupt() {
    if (UART_GetITStatusMasked(MDR_UART2, UART_IT_TX) == SET) {
        UART_ClearITPendingBit(MDR_UART2, UART_IT_TX);
        MDR_UART2->ICR |= UART_ICR_TXIC;
    }
    if (UART_GetITStatusMasked(MDR_UART2, UART_IT_RX) == SET) {
        Uart::uart->read();
    }
}

/**
 * Инициализация UART
 * @param Rx
 * @param Tx
 */
void
Uart::init(MDR_PORT_TypeDef *tPort, uint16_t tPin, MDR_PORT_TypeDef *rPort, uint16_t rPin, MDR_UART_TypeDef *uartx) {
    this->uartx = uartx;
    // Процедура инициализации порта
    // Включение тактирования порта F
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
    // Объявление структуры для инициализации порта
    PORT_InitTypeDef Nastroyka;
    // Инициализация порта B для функции UART
    // Настройка порта по умолчанию
    PORT_StructInit(&Nastroyka);
    // Переопределение функции порта
    Nastroyka.PORT_FUNC = PORT_FUNC_OVERRID;
    // Установка короткого фронта
    Nastroyka.PORT_SPEED = PORT_SPEED_MAXFAST;
    // Цифровой режим работы вывода
    Nastroyka.PORT_MODE = PORT_MODE_DIGITAL;
    // Инициализация вывода PF1 как UART_TX (передача)
    Nastroyka.PORT_Pin = tPin;
    Nastroyka.PORT_OE = PORT_OE_OUT;
    PORT_Init(tPort, &Nastroyka);
    // Инициализация вывода PF0 как UART_RX (прием)
    Nastroyka.PORT_Pin = rPin;
    Nastroyka.PORT_OE = PORT_OE_IN;
    PORT_Init(rPort, &Nastroyka);


    // Включение тактирования UART2
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);
    // Объявление структуры для инициализации контроллера UART
    UART_InitTypeDef UART_InitStructure;
    // Делитель тактовой частоты UART = 1
    UART_BRGInit(this->uartx, UART_HCLKdiv1);
    // Конфигурация UART
    // Скорость передачи данных – 115200 бод
    UART_InitStructure.UART_BaudRate = 115200;
    // Количество бит в посылке – 8
    UART_InitStructure.UART_WordLength = UART_WordLength8b;
    // Один стоп-бит
    UART_InitStructure.UART_StopBits = UART_StopBits1;
    // Без проверки четности
    UART_InitStructure.UART_Parity = UART_Parity_No;
    // Выключить работу буфера FIFO приемника и передатчика,
    // т.е. передача осуществляется по одному байту
    UART_InitStructure.UART_FIFOMode = UART_FIFO_ON;
    // Разрешить прием и передачу данных
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    // Инициализация UART2 с заданными параметрами
    UART_Init(this->uartx, &UART_InitStructure);
    // Включить сконфигурированный UART
    UART_Cmd(this->uartx, ENABLE);

    NVIC_EnableIRQ(UART2_IRQn);
//    UART_ITConfig(this->uartx, UART_IT_TX, ENABLE);
    UART_ITConfig(this->uartx, UART_IT_RX, ENABLE);

    QueueHandle_t sendQueue = xQueueCreate(3, sizeof(MAX_MESSAGE_LEN));
    QueueHandle_t recieveQueue = xQueueCreate(3, sizeof(MAX_MESSAGE_LEN));
}


#ifdef __cplusplus
extern "C" {
#endif

void UART2_IRQHandler(void) {
    Uart::interrupt();
}
#ifdef __cplusplus
} // extern "C" block end
#endif