/*===========================================================================
 *
 *  Sensor controller
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File sensor-config.c. Read sensors and print to LCD.
 *                        Configuration.
 *
 *===========================================================================*/

#include "sensor-config.h"
#include <CRC.h>
#include <fonts.h>
#include <screen.h>
#include <controls.h>

#if (TARGET_MCU == MCU_1986BE9x)
//MCU: 1986BE9x
#define I2C1_MAP_NN     I2C1_MAP_C0C1   //PC0/PC1   -> X33.2 #10/#11
//#define I2C1_MAP_NN   I2C1_MAP_E14E15 //PE14/PE15 -> X32.1 #27/#28

#define IWIRE_MAP_NN   ((PORT_A << 8) | 9) //PA9 -> X33.1 #25

#elif (TARGET_MCU == MCU_1901BC1x)
//MCU: 1901BC1x
#define I2C1_MAP_NN     I2C1_MAP_D0D1   //PD0/PD1   -> X33.2 #17/#22 (or JTAGB #7/#9)
//#define I2C1_MAP_NN   I2C1_MAP_C14C15 //PC14/PC15 -> X33.2 #28/#24

#define IWIRE_MAP_NN   ((PORT_A << 8) | 9) //PA9 -> X33.1 #25

#elif (TARGET_MCU == MCU_1986BE1x)
//MCU: 1986BE1x
#define I2C1_MAP_NN     I2C1_MAP_C11C12 //PC11/PC12 -> XP48 #7/#8
//#define I2C1_MAP_NN     I2C1_MAP_D0D1 //PD0/PD1   -> XP48 #22/#24

#define IWIRE_MAP_NN   ((PORT_A << 8) | 9) //PA9 -> XP47 #25

#else
#error "This source code does not work with specified MCU"

#endif /* TARGET_MCU */

void UART_Received();
void Timer_ZeroEventHandler();
void SysTimer_ZeroEventHandler();
void PVD_EventHandler();

//RTC configuration
static const RTCConfig rtcCfg =
{
    RTC_N1,      //RTC #
    0,           //Calibrating
    1,           //Speed, Hz (must 1)
    CLKSRC_LSE,  //Source
    0            //Handler
};


//Timer configuration
static const TimerConfig timCfg =
{
    (TIMBLOCK)TIM_N1,       //Timer #
    (u32)TIMER_SPEED,       //Speed, Hz
    &Timer_ZeroEventHandler //Tick handler
};


//System timer configuration
static const SysTimerConfig systimCfg =
{
    SYSTIM_N1,                 //Systimer #
    10000,                     //Speed, Hz
    CLKSRC_HCLK,               //Clock source {CLKSRC_HCLK|CLKSRC_LSE}
    0,//&SysTimer_ZeroEventHandler //Timer event handler
};


//UART configuration
static const UARTConfig uartCfg =
{
    (UARTBLOCK)UART_NN,
    (u8)UART_DATABITS_8,    //8, 7, 6, 5
    (u32)UART_SPEED_115200, //57600
    (u32)(UART_PARITY_NONE|UART_STOPBITS_1|UART_NOCTRL),
    &UART_Received,         //Incoming data handler
    (u16)UART_MAP_CTRL,
    (u16)UART_MAP_RXTX
};


//PVD configuration
static const PVDConfig pvdCfg =
{
    PVD_N1,          //PVD #
    0,               //Options (see PVD_OPTS_x)
    30,               //Source power threshold, BCD format
    26,               //Battery power threshold, BCD format
    &PVD_EventHandler //User-defined handler
};


//1-Wire configuration.
//MCUs 1986x 1901x does not have 1-Wire controller,
//software implementation used
static const IWireConfig iwireCfg =
{
    IWIRE_N1,          //1-Wire #
    IWIRE_OPTS_MASTER, //Options (see IWIRE_OPTS_x)
    IWIRE_MAP_NN,      //pin mapping
    0                  //handler
};


//I2C configuration (master mode only)
static const I2CConfig i2cCfg =
{
    I2C_N1,            //I2C block number (see I2C_Nx)
    I2C_SPEED,         //Requested speed, bit/s
    I2C_OPTS_MASTER,   //Options (see I2C_OPTS_x)
    0,                 //Tx/Rx data completed
    I2C1_MAP_NN        //mapping
};


//Selected MPU60X0 profile: 4g, 250deg/s
#define MPU6000_PROFILE_4g250d  (MPU6000_PROFILE_4g|MPU6000_PROFILE_250dps)

//Sensor configuration: { type, profile, address[8], name }
const SensorConfig sensorConfig[SENSOR_MAXCOUNT] =
{    
    { SENSOR_TYPE_DS18B20, 0, { 0x28, 0xFF, 0x64, 0x18, 0x9F, 0x46, 0xD7, 0xF0 }, "T1\0" },
    { SENSOR_TYPE_DS18B20, 0, { 0x28, 0xFF, 0x64, 0x18, 0x9F, 0x41, 0xE3, 0x41 }, "T2\0" },
    { SENSOR_TYPE_DS18B20, 0, { 0x28, 0xFF, 0x64, 0x18, 0x9F, 0x5D, 0x1E, 0x69 }, "T3\0" },

    { SENSOR_TYPE_BH1750,  0, { BH1750_CADDR0,   0, 0, 0, 0, 0, 0, 0 }, "L1\0" },

    { SENSOR_TYPE_INA219,  0, { INA219_CADDR00,  0, 0, 0, 0, 0, 0, 0 }, "V1\0" },

    { SENSOR_TYPE_PCF8591, 0, { PCF8591_CADDR|0, 0, 0, 0, 0, 0, 0, 0 }, "A1\0" },

    { SENSOR_TYPE_LM75A,   0, { LM75A_CADDR|1,   0, 0, 0, 0, 0, 0, 0 }, "T4\0" },

    { SENSOR_TYPE_DHT12,   0, { DHT12_CADDR0,    0, 0, 0, 0, 0, 0, 0 }, "T5\0" },

    { SENSOR_TYPE_BMP280,  BMP280_PROFILE_ELEVATOR, { BMP280_CADDR0,  0, 0, 0, 0, 0, 0, 0 }, "P1\0" },

    { SENSOR_TYPE_MPU6000, MPU6000_PROFILE_4g250d,  { MPU6000_CADDR0, 0, 0, 0, 0, 0, 0, 0 }, "G1\0" }
};

/*===========================================================================
 * SCREEN AND CONTROLS
 *===========================================================================*/

#if (TARGET_LCD == LCD_PC1602LRSFNHx || TARGET_LCD == LCD_PC2004A)

#if (TARGET_LCD == LCD_PC1602LRSFNHx)
//LCD with cyrillic letters
#define LCD_CODEPAGE &cpPC1602LRSFNHx
#else
//LCD without cyrillic letters
#define LCD_CODEPAGE &cpPC1602LRSx
#endif

//Char display
//LCD PC1602x: 16 chars x 2 lines
//LCD PC2004x: 20 chars x 4 lines
//Screens dimentions, chars (assume font 6x8)
#define SCF6x8_WIDTH          (LCD_WIDTH  / 6)
#define SCF6x8_HEIGHT         (LCD_HEIGHT / 8)

//Char buffer
static char charBuffer[(SCF6x8_WIDTH * SCF6x8_HEIGHT + 4)];

//Text screen configuration
ScreenParams screenPCWWHH =
{
    0,                    //no font: char display used
    LCD_CODEPAGE,         //codepage
    0,                    //no color table
    0,                    //no backcolor buffer
    0,                    //no forecolor buffer
    0,
    charBuffer,           //char buffer
    SCF6x8_WIDTH,         //width, chars
    SCF6x8_HEIGHT,        //height, chars
    SCREEN_OPTS_COLOR_1b, //1-bit color
    { 0, 0, 0 }           //cursor
};

ScreenParams *pscreen = &screenPCWWHH;

#else

//Graphic display
//LCD MT-12864: 128x64 pixels
//Screens dimentions, chars
//Font 6x8
#define SCF6x8_WIDTH          (LCD_WIDTH  / 6)
#define SCF6x8_HEIGHT         (LCD_HEIGHT / 8)

//Background, foreground and char buffers
#define CHCOLORS_BUFSIZE       SCREEN_GRBUFSIZE_1b(SCF6x8_WIDTH, SCF6x8_HEIGHT)
static u8 bColorsBuffer[CHCOLORS_BUFSIZE];
static u8 fColorsBuffer[CHCOLORS_BUFSIZE];
static char charBuffer[(SCF6x8_WIDTH * SCF6x8_HEIGHT + 4)];

//Text screen configuration
//Using LCD 128x64 pixels
//Font 6x8:   64/8  x (int)(128/6)  => 8 lines x 21 chars
ScreenParams screenF6x8 =
{
    &font6x8_win1251,     //font: 6x8 pixels
    0,                    //codepage (0 - not used)
    0,                    //color table (0 - not used)
    bColorsBuffer,        //background colors buffer
    fColorsBuffer,        //foreground colors buffer
    0,
    charBuffer,           //char buffer
    SCF6x8_WIDTH,         //width, chars
    SCF6x8_HEIGHT,        //height, chars
    SCREEN_OPTS_COLOR_1b, //1-bit color
    { 0, 0, 0 }           //cursor
};

ScreenParams *pscreen = &screenF6x8;

#endif /* TARGET_LCD */


#define TEXTBOX_MAXLENGTH       (SCF6x8_WIDTH * 50)
static char textBox_buffer[TEXTBOX_MAXLENGTH];
TextBox textBox;

/* Initialize controls */
static void InitializeControls()
{
    InitializeControlParameters(&textBox.control, CTRL_TYPE_TEXTBOX, 0,
                                0, 0, SCF6x8_WIDTH, SCF6x8_HEIGHT,
                                COL_WHITE, COL_BLACK,
                                CTRL_OPTS_VISIBLE|CTRL_OPTS_ENABLED|CTRL_OPTS_WORDWRAP|CTRL_OPTS_NOCURSOR,
                                0);

    textBox.text = textBox_buffer;
    textBox.textMaxLength = TEXTBOX_MAXLENGTH;
}



/* Standard output within UART */
void stdout_printchar(PrintContext *pc, char c)
{
    if (c)
        UARTWrite(UART_NN, (u8*)&c, 1);
}



/* Initialize device */
MCRESULT InitializeDevice()
{
    u32 tm;

    //1. Set CPU core speed, Hz
    SetHCLK(CPU_SPEED, CLKSRC_HSE);

    //2. Initialize peripherals
    #ifdef LED_0
    InitializeEvBrdButtons();

    InitializeEvBrdLeds();
    #endif /* LED_0 */

    InitializeSysTimer(&systimCfg);

    InitializeTimerX(&timCfg);

    TurnTimerX(timCfg.tim, 1);

    //Wait >15ms after Vdd rise to 4.5V (char displays PC1602, PC2004)
    tm = i2cTimerMs;
    while((i2cTimerMs - tm) < 50) __NOP();

    InitializeEBus(0);

    InitializeRTC(&rtcCfg);

    InitializeLCD(GetHCLK());

    //Debug messages enabled
    std_pfprintchar = &stdout_printchar;

    InitializeUART(&uartCfg);

    TurnUART(uartCfg.uart, 1);

    InitializeI2C(&i2cCfg);

    InitializeIWire(&iwireCfg);

    InitializePVD(&pvdCfg);

    //3. Turn blocks
    TurnSysTimer(systimCfg.tim, 1);

    TurnPVD(pvdCfg.pvd, 1);

    TurnI2C(i2cCfg.i2c, 1);

    TurnIWire(iwireCfg.iwire, 1);

    TurnRTC(rtcCfg.rtc, 1);

    TurnLCD(1);

    LCDClear(LCD_BASEADDR);

    //Initialize sensors
    InitializeSensors(&sensorConfig[0], SENSOR_POLL_INTERVAL);

    //Initialize screen controls
    InitializeControls();

    //First otuput to LCD
    PrintSensorsData(1);

    //Ready indication
    SetLed(LED_0);

    return MCR_OK;
}

/*===========================================================================
 * End of file sensor-config.c
 *===========================================================================*/
