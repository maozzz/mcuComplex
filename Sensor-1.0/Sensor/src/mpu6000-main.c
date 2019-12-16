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
 *  File mpu6000-main.c. Test sensor MPU-6000 only, no LCD.
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

//Requested CPU speed, Hz
#define CPU_SPEED                   80000000

//Requested I2C speed, bit/s
#define I2C_SPEED                   100000

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
 * MPU60X0 TOP LEVEL FUNCTIONS
 *===========================================================================*/

#define MPU60X0_VARIANT_KALMAN          0
#define MPU60X0_VARIANT_GYROCORRECT     1 //recommended

//MPU60X0 configuration (constant)
static const SensorConfig mpu60X0Cfg =
{
    SENSOR_TYPE_MPU6000,                         //type
    MPU6000_PROFILE_4g|MPU6000_PROFILE_250dps,   //profile: 4g, 250 deg/s
    { MPU6000_CADDR0,   0, 0, 0, 0, 0, 0, 0 },   //address
    "G1\0"                                       //name
};


//MPU60X0 control structure (runtime state)
static SensorCtrl mpu60X0Ctrl;

//Kalman filter states
static KalmanF kalmanXF;
static KalmanF kalmanYF;

//Variants (see MPU60X0_VARIANT_x)
static u32 mpu60X0variant = MPU60X0_VARIANT_GYROCORRECT;

//Saved time
static u32 begTime = 0;

//Alpha
static float alpha = 0.95F;

//Rotate X/Y
float rotxy[3];

/*---------------------------------------------------------------------------
 * Test only
 *---------------------------------------------------------------------------*/
#define AVGBUF_BITS     3
#define AVGBUF_SIZE     (1 << AVGBUF_BITS)
static s16 avgbufX[AVGBUF_SIZE] = { 0 };
static s16 avgbufY[AVGBUF_SIZE] = { 0 };
static s16 avgbufZ[AVGBUF_SIZE] = { 0 };

static s32 Xavg = 0;
static s32 Yavg = 0;
static s32 Zavg = 0;

/* Test only */
void update_average(s16 X, s16 Y, s16 Z)
{
    s32 i;

    Xavg -= (s32)avgbufX[0];
    Yavg -= (s32)avgbufY[0];
    Zavg -= (s32)avgbufZ[0];

    for(i = 0; i < AVGBUF_SIZE - 1; i++)
    {
        avgbufX[i] = avgbufX[i + 1];
        avgbufY[i] = avgbufY[i + 1];
        avgbufZ[i] = avgbufZ[i + 1];
    }

    avgbufX[AVGBUF_SIZE - 1] = X;
    avgbufY[AVGBUF_SIZE - 1] = Y;
    avgbufZ[AVGBUF_SIZE - 1] = Z;

    Xavg = (Xavg + (s32)X);
    Yavg = (Yavg + (s32)Y);
    Zavg = (Zavg + (s32)Z);
}



/* Initialize MPU60X0 sensor */
void InitializeMPU60X0()
{    
    InitializeMPU6000(&mpu60X0Ctrl, &mpu60X0Cfg);    

    //Kalman filter
    InitializeKalmanF(&kalmanXF, 2.0F, 1.0F, 1.0F, 1.0F);
    InitializeKalmanF(&kalmanYF, 2.0F, 1.0F, 1.0F, 1.0F);
}



/* Read sensor, print data */
void UpdateMPU60X0State(SensorCtrl *sen)
{    
    if (senWaitTimeCounter)
        return;

    if (sen->flags & SENSOR_FLAG_CONFIGURED)
    {
        //Read sensor
        if (MPU6000ReadData(sen) == 0)
        {
            //Read success

            if (mpu60X0variant == MPU60X0_VARIANT_KALMAN)
            {
                //Variant 1. Apply Kalman filter
                MPU6000GetRollPitch(sen, rotxy);

                rotxy[0] = ApplyKalmanF(&kalmanXF, rotxy[0]);
                rotxy[1] = ApplyKalmanF(&kalmanYF, rotxy[1]);
            }
            else
            {
                //Variant 2. Use correction
                u32 dt;
                s16 xg, yg, zg;
                float xgf, ygf, zgf;

                //Get dt, msec
                dt = i2cTimerMs - begTime;
                begTime = i2cTimerMs;

                //encValue[4..6]: gyroscope x,y,z
                xg = (s16)sen->encValue[4] - mpu6000GyroOffset[0];
                yg = (s16)sen->encValue[5] - mpu6000GyroOffset[1];
                zg = (s16)sen->encValue[6] - mpu6000GyroOffset[2];

                xgf = (float)xg / mpu6000GyroScale;
                ygf = (float)yg / mpu6000GyroScale;
                zgf = (float)zg / mpu6000GyroScale;

                xgf = xgf * (float)dt * 0.001F + rotxy[0];
                ygf = ygf * (float)dt * 0.001F + rotxy[1];

                MPU6000GetRollPitch(sen, rotxy);

                rotxy[0] = alpha * xgf + (1.0F - alpha) * rotxy[0];
                rotxy[1] = alpha * ygf + (1.0F - alpha) * rotxy[1];
            }


            //Print data
            std_printf("P %6.1f %6.1f\n",
                       rotxy[0], rotxy[1]
                       );

            //Get offsets
            /*update_average(xg, yg, zg);
            std_printf("A %5i %5i %5i\n",
                       Xavg >> AVGBUF_BITS,
                       Yavg >> AVGBUF_BITS,
                       Zavg >> AVGBUF_BITS);*/

            //Test only
            sensorRdCounter++;
        }
        else
        {
            //Read fail
            //Reset flag
            sen->flags &= ~SENSOR_FLAG_CONFIGURED;
        }
    }
    else
    {
        //Configure sensor
        if (MPU6000Reset(sen) == 0 &&
            MPU6000Configure(sen) == 0 &&
            MPU6000ReadData(sen) == 0)
        {
            //Success
            //Get initial rotation
            MPU6000GetRollPitch(sen, rotxy);

            sen->flags |= SENSOR_FLAG_CONFIGURED;
            begTime = i2cTimerMs;
        }
        else
        {
            //Fail
            sen->flags &= ~SENSOR_FLAG_CONFIGURED;
        }
    }


    //Restart time counter
    senWaitTimeCounter = time2ticks(20);
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
            static u32 ledcnt = 0;

            //std_printf function transmit formatted strings within UART (DB9 port on eval.board)
            //(see stdout_printchar function at "main.c")
            std_printf("Device info:\n"
                       "CPU : %u Hz\n"
                       "UART: %u bps\n"
                       "I2C : %u bps\n"
                       "Sens: %u rd/s\n"
                       "Main: %u cyc/s\n\n",
                        GetHCLK(),             //Current CPU clock, Hz
                        GetUARTSpeed(UART_NN), //Current UART speed, bit/s                        
                        GetI2CSpeed(I2C_N1),   //Current I2C speed, bit/s                        
                        sensorRdSpeed,         //Count of reading/s
                        mainSpeed);            //Count of main cycles per second

            ledcnt++;
            if (ledcnt & 1) { SetLed(LED_2);   }
            else            { ResetLed(LED_2); }

            break;
        }

        case BUTTON_UP:
        {           
            std_printf("Up\n");
            break;
        }

        case BUTTON_DOWN:
        {            
            std_printf("Down\n");
            break;
        }

        case BUTTON_LEFT:
        {
            //std_printf("Left\n");
            char dest1[400];
            char dest2[400];

            ScanIWireBus(dest1, 0);
            ScanI2CBus(dest2, 0);

            std_printf("Scan 1-Wire/I2C bus...\n"
                       "%s%s"
                       "Scan completed\n\n", dest1, dest2);            
            break;
        }

        case BUTTON_RIGHT:
        {
            s16 goffs[3];
            s16 aoffs[3];

            std_printf("Get gyro offsets...\n");
            MPU6000GetGyroOffsets(&mpu60X0Ctrl, goffs);
            std_printf("GOFF %i, %i, %i\n", goffs[0], goffs[1], goffs[2]);

            std_printf("Get accel offsets...\n");
            MPU6000GetAccelOffsets(&mpu60X0Ctrl, aoffs);
            std_printf("AOFF %i, %i, %i\n", aoffs[0], aoffs[1], aoffs[2]);

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


    //Initialize sensor(s)
    InitializeMPU60X0();

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

        UpdateMPU60X0State(&mpu60X0Ctrl);

        mainCounter++;
    }
}

/*===========================================================================
 * End of file mpu6000-main.c
 *===========================================================================*/
