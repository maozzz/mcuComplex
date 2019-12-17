
#include "main.h"

void initClk();

int main() {
    initClk();

    xTaskCreate(Uart::task, "uart", 32, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(mq135Task, "mq135", 200, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(DHT11Task, "dht11", 200, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(glcdTask, "glcd", 300, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Запускаем диспетчер и понеслась.
    vTaskStartScheduler();

    while (true) {
        asm("nop");
    }
}



void initClk(void) {
    // HSE ON
    RST_CLK_DeInit();
    SystemInit();

    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while (RST_CLK_HSEstatus() != SUCCESS);

    // FLASH MEMORY
    MDR_EEPROM->CMD |= 0b111000;

    // PLL ON
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
    RST_CLK_CPU_PLLcmd(ENABLE);
    while (RST_CLK_CPU_PLLstatus() != SUCCESS);

    RST_CLK_CPU_PLLuse(ENABLE);
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    SystemCoreClockUpdate();
}

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} // extern "C" block end
#endif