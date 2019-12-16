/*===========================================================================
 *
 *  Test software
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
 *  File kt0803-main.c. FM radio transmitter KT0803 usage sample
 *                      No LCD, tx information within UART.
 *
 *===========================================================================*/

#include "sensor.h"
#include <CPU.h>
#include <Timer.h>
#include <SysTimer.h>
#include <UART.h>
#include <I2C.h>
#include <IWire.h>
#include <LCD.h>
#include <evbrds.h>
#include <evbrd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <errno.h>
#include "sensor.h"

//Timer speed, Hz (must 1000Hz)
#define TIMER_SPEED                (1000)
#define time2ticks(msec)           ((TIMER_SPEED * (msec)) / 1000)

#define ticks2time(tick)           (((tick) * 1000) / TIMER_SPEED)

//Requested CPU speed, Hz (80MHz recommended)
#define CPU_SPEED                   80000000

//Requested I2C speed, bit/s
#define I2C_SPEED                   30000

//DB9 port on evbrd
#define UART_NN                     DB9_UART_NN
#define UART_MAP_CTRL               DB9_UART_MAP_CTRL
#define UART_MAP_RXTX               DB9_UART_MAP_RXTX

//Button delay, timer clocks
#define BUTTON_DELAY_INTERVAL       time2ticks(200)


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
    0,                         //No event handler
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


//I2C configuration (master mode only)
static const I2CConfig i2cCfg =
{
    I2C_N1,            //I2C block number (see I2C_Nx)
    I2C_SPEED,         //Requested speed, bit/s
    I2C_OPTS_MASTER,   //Options (see I2C_OPTS_x)
    0,                 //Tx/Rx data completed
    I2C1_MAP_NN        //mapping
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

/*===========================================================================
 * INTERRUPT HANDLERS
 *===========================================================================*/

//Test variables
volatile u32 timCounter  = 0;
u32 mainSpeed = 0;
u32 sensorRdSpeed = 0;

u32 mainCounter = 0;
u32 sensorRdCounter = 0;

volatile u32 buttonDelayCounter = 0;
static u32 ledBlinkCounter;


/* */
void Timer_ZeroEventHandler()
{
    //Keyboard timers
    if (buttonDelayCounter > 0)
        buttonDelayCounter--;

    if (senWaitTimeCounter > 0)
        senWaitTimeCounter--;

    timCounter++;

    UpdateI2CTimers();

    if (timCounter == TIMER_SPEED)
    {
        mainSpeed     = mainCounter;
        sensorRdSpeed = sensorRdCounter;

        mainCounter     = 0;
        sensorRdCounter = 0;

        timCounter = 0;        

        ledBlinkCounter++;
        if (ledBlinkCounter & 1)
        {
            SetLed(LED_1);
        }
        else
        {
            ResetLed(LED_1);
        }
    }
}



/* UART received */
void UART_Received()
{
    s32 n;
    u8 buf[UART_FIFO_RX_LENGTH];

    n = UARTRead(UART_NN, buf, UART_FIFO_RX_LENGTH);

    if (n) {}
}

/*===========================================================================
 * KT0803x TOP LEVEL FUNCTIONS
 *===========================================================================*/

//PGA at dB
static const s32 kt0803_pga_dB[8] =
{
    0, -4, -8, -12,
    0,  4,  8,  12
};


//RFGAIN at dBuV
static const float kt0803_rfgain_dBuV[24] =
{
    95.5F, //RFGAIN=0
    96.5F, //RFGAIN=1
    97.5F, //...
    98.2F,
    98.9F,
    100.0F,
    101.5F,
    102.8F, //RFGAIN=7

    //When Reg0E.PA_BIAS=0
    105.1F, //RFGAIN=8
    105.6F, //...
    106.2F,
    106.5F,
    107.0F,
    107.4F,
    107.7F,
    108.0F, //RFGAIN=15

    //When Reg0E.PA_BIAS=1
    107.2F, //RFGAIN=8
    108.0F, //...
    108.7F,
    109.5F,
    110.3F,
    111.0F,
    111.7F,
    112.5F  //RFGAIN=15
};


//Registers that defined at KT0803L datasheet
#define kt0803l_regaddr_count   18
static const u8 kt0803l_regaddr[kt0803l_regaddr_count] =
{
     0x00, 0x01, 0x02, 0x04, 0x0B, 0x0C, 0x0E, 0x0F,
     0x10, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x1E,
     0x26, 0x27
};


//KT0803 configuration (constant)
static const SensorConfig kt0803Cfg =
{
    SENSOR_TYPE_KT0803,                        //type
    0,                                         //profile:
    { KT0803_CADDR0,   0, 0, 0, 0, 0, 0, 0 },  //address
    "RT\0"                                     //name
};



//KT0803 control structure (runtime state)
static SensorCtrl kt0803Ctrl;

//Requested frequency, kHz.
//Must: KT0803_MINFREQ_KHZ <= freq <= KT0803_MAXFREQ_KHZ
static s32 kt0803_reqfreq = 89000;

/* */
static s32 IsKT0803LValidRegAddr(u8 regaddr)
{
    s32 i;

    for(i = 0; i < kt0803l_regaddr_count; i++)
    {
        if (kt0803l_regaddr[i] == regaddr)
            return 1;
    }

    return 0;
}



/* Initialize KT0803 transmitter */
s32 InitializeRTKT0803()
{
    SensorCtrl *sen = &kt0803Ctrl;

    //Initialize control structure
    InitializeKT0803(sen, &kt0803Cfg);

    //Wait while reg0F.PW_OK != 1;
    //Turn KT0803 ON (reset standby bit);
    if (KT0803WaitPowerFlag(sen) == 0 &&
        KT0803Turn(sen, 1) == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}



/* Tune KT0803 */
s32 RTKT0803Configure(SensorCtrl *sen, s32 fkHz)
{
    std_printf("KT0803 %i kHz...", fkHz);

    if (KT0803Tune(sen,
                   fkHz,
                   KT0803_DEFAULT_RFGAIN,
                   KT0803_DEFAULT_PGA,
                   KT0803_DEFAULT_PHTCNST) == 0)
    {
        std_printf("OK\n");
        return 0;
    }
    else
    {
        std_printf("FAIL\n");
        return -1;
    }
}



/* */
void RTKT0803FreqStep(SensorCtrl *sen, s32 fstepkHz)
{
    //New frequency
    s32 fkHz = kt0803_reqfreq;

    fkHz += fstepkHz;

    if (fkHz < KT0803_MINFREQ_KHZ)
        fkHz = KT0803_MAXFREQ_KHZ;
    else if (fkHz > KT0803_MAXFREQ_KHZ)
        fkHz = KT0803_MINFREQ_KHZ;

    if (RTKT0803Configure(sen, fkHz) == 0)
    {
        //Success: save frequency
        kt0803_reqfreq = fkHz;
    }
}



/* Read and print KT0803x registers/configuration */
void RTKT0803PrintRegisters(SensorCtrl *sen)
{
    #define KT0803x_REGCOUNT        0x28
    s32 i, fkHz;
    u8 rfgain, pga, pht;
    u8 regs[KT0803x_REGCOUNT];

    //---------------------------------------------
    std_printf("KT0803 REGISTERS: \n");

    if (I2CReadRegister(I2C_N1, KT0803_CADDR0,
                        0x00, regs, KT0803x_REGCOUNT) == KT0803x_REGCOUNT)
    {
        for(i = 0; i < KT0803x_REGCOUNT; i++)
        {
            if (IsKT0803LValidRegAddr(i))
                std_printf("[%02X] %02X\n", i, regs[i]);
        }
    }
    else
    {
        std_printf("KT0803 READ FAIL\n");
    }

    #undef KT0803x_REGCOUNT

    //---------------------------------------------
    i = KT0803GetStatus(sen, &fkHz, &rfgain, &pga, &pht);

    std_printf("KT0803 CONFIG (%s):\n"
               "FREQ    %i kHz\n"
               "RFGAIN  %.1f dBuV (%i)\n"
               "PGA     %i dB\n"
               "PHTCNST %i us\n",
               (i == 0) ? "OK" : "FAIL",
               fkHz,
               kt0803_rfgain_dBuV[rfgain], rfgain,
               kt0803_pga_dB[pga],
               (pht == 0) ? 75: 50 //75/50 -> USA/Europe
                );
}

/*===========================================================================
 * KEYBOARD
 *===========================================================================*/

//Current button
static u32 prevEvbrdKey = BUTTON_NONE;


/* Simple evaluation board keyboard handler */
void KeyboardHandler()
{
    u32 evbrdKey;

    evbrdKey = ReadButton();

    if (evbrdKey == BUTTON_NONE)
    {
        prevEvbrdKey = BUTTON_NONE;
        return;
    }
    else if (prevEvbrdKey == evbrdKey && buttonDelayCounter > 0)
        return;

    buttonDelayCounter = BUTTON_DELAY_INTERVAL;
    prevEvbrdKey       = evbrdKey;

    //"BUTTON_x" defintions at file "evbrd.h"
    switch(evbrdKey)
    {
        case BUTTON_SELECT:
        {            
            //std_printf function transmit formatted strings within UART (DB9 port on eval.board)
            //(see stdout_printchar function at "main.c")
            std_printf("Device info:\n"
                       "CPU : %u Hz\n"
                       "UART: %u bps\n"
                       "I2C : %u bps\n"                       
                       "Main: %u cyc/s\n\n",
                        GetHCLK(),             //Current CPU clock, Hz
                        GetUARTSpeed(UART_NN), //Current UART speed, bit/s                        
                        GetI2CSpeed(I2C_N1),   //Current I2C speed, bit/s                                                
                        mainSpeed);            //Count of main cycles per second

            RTKT0803PrintRegisters(&kt0803Ctrl);
            break;
        }

        case BUTTON_UP:
        {
            //KT0803: increase frequency
            RTKT0803FreqStep(&kt0803Ctrl, 1000);
            break;
        }

        case BUTTON_DOWN:
        {
            //KT0803: decrease frequency
            RTKT0803FreqStep(&kt0803Ctrl, -1000);
            break;
        }

        case BUTTON_LEFT:
        {
            //std_printf("Left\n");            
            char dest[400];

            ScanI2CBus(dest, 0);

            std_printf("Scan I2C bus...\n"
                       "%s"
                       "Scan completed\n\n", dest);
            break;
        }

        case BUTTON_RIGHT:
        {
            std_printf("Right\n");
            break;
        }        

        #if (defined(TARGET_EVBRD) && TARGET_EVBRD == EVBRD_1986BE1T)
        case BUTTON_BACK:
        {
            std_printf("Back\n");
            break;
        }

        #endif /* EVBRD_1986BE1T */
    } //switch(buttonCurrent)
}

/*===========================================================================
 * INITIALIZATION AND MAIN FUNCTION
 *===========================================================================*/

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

    //Debug messages enabled
    std_pfprintchar = &stdout_printchar;

    InitializeUART(&uartCfg);

    TurnUART(uartCfg.uart, 1);

    InitializeI2C(&i2cCfg);

    InitializeIWire(&iwireCfg);

    //3. Turn blocks
    TurnSysTimer(systimCfg.tim, 1);

    TurnI2C(i2cCfg.i2c, 1);

    TurnIWire(iwireCfg.iwire, 1);


    //Initialize and configure KT0803
    InitializeRTKT0803();
    RTKT0803Configure(&kt0803Ctrl, kt0803_reqfreq);

    //Ready indication
    SetLed(LED_0);

    return MCR_OK;
}



/* Main function */
void main(void)
{    
    InitializeDevice();

    while(1)
    {
        KeyboardHandler();        

        mainCounter++;
    }
}

/*===========================================================================
 * End of file kt0803-main.c
 *===========================================================================*/
