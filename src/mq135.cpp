#include "mq135.h"

// Процедура общей настройки АЦП
void ADCInit() {
    // Включение тактирования АЦП
    RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
    // Объявление структур для общей настройки АЦП
    ADC_InitTypeDef ADC;
    // Загрузка значений по умолчанию в структуру ADC
    ADC_StructInit(&ADC);
    // Инициализация АЦП объявленной структурой
    ADC_Init(&ADC);
}

// Процедура настройки АЦП1
void ADC1Init() {
    // Объявление структур для общей настройки АЦП1
    ADCx_InitTypeDef ADC1;
    // Загрузка значений по умолчанию в структуру ADC1
    ADCx_StructInit(&ADC1);
    // Установка номера канала АЦП,
    // подключенного к резистору R1 платы
    ADC1.ADC_ChannelNumber = ADC_CH_ADC6;
    // Инициализация первого АЦП объявленной структурой
    ADC1.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
    ADC1.ADC_LowLevel = 0;
    ADC1.ADC_HighLevel = 0x07FF;
    ADC1.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
    ADC1.ADC_Prescaler = ADC_CLK_div_512;
    ADC1_Init(&ADC1);
    // Включение первого АЦП
    ADC1_Cmd(ENABLE);
}

void initAdc() {
    PORT_InitTypeDef PortInitStructure;
    PORT_StructInit(&PortInitStructure);
    PortInitStructure.PORT_Pin = PORT_Pin_6;
    PortInitStructure.PORT_MODE = PORT_MODE_ANALOG;
    PORT_Init(MDR_PORTD, &PortInitStructure);

    ADCInit();
    ADC1Init();
}

void mq135Task(void *arg) {
    initAdc();
    float ppm;
    uint32_t raw_adc;
    char str[9];
    while (1) {
        ADC1_Start();
        while (ADC1_GetFlagStatus(ADC1_FLAG_END_OF_CONVERSION) == 0) {
            portYIELD()
        }
        raw_adc = ADC1_GetResult() & 0x00000FFF;
        ppm = ((10000.0 / 4096.0) * raw_adc) - 200;
        sprintf(str, "CO2: %2.2f ppm\n", ppm);
        Uart::uart->send(str);
        vTaskDelay(500);
    }
}