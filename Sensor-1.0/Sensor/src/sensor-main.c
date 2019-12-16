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
 *  File sensor-main.c. Read sensors and print to LCD.
 *                      I2C and 1-Wire buses used.  
 *
 *===========================================================================*/

#include "sensor-config.h"

/*===========================================================================
 * INTERRUPT HANDLERS
 *===========================================================================*/

//Test variables
u32 timCounter  = 0;
u32 mainSpeed = 0;
u32 mainCounter = 0;

volatile u32 buttonDelayCounter = 0;
static u32 ledBlinkCounter;
static u32 systimBlinkCounter = 0;

static void UARTRxDataHandler(const u8 *rxdata, s32 n);

/* */
void SysTimer_ZeroEventHandler()
{
    systimBlinkCounter++;
    if (systimBlinkCounter & 1)
    {
        SetLed(LED_2);
    }
    else
    {
        ResetLed(LED_2);
    }
}



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
        mainSpeed = mainCounter;        
        mainCounter = 0;
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

    UARTRxDataHandler(buf, n);
}



/* PVD event handler */
void PVD_EventHandler()
{
    if (IsPVDPowerEvent(PVD_N1))
    {
        std_printf("Ucc>Uth\n");
    }

    if (IsPVDBatteryEvent(PVD_N1))
    {
        std_printf("BUcc>BUth\n");
    }
}

/*===========================================================================
 * KEYBOARD
 *===========================================================================*/

//d2019.01.01.16.59.00
static char dtimestr[32] = { 0 };

/* */
static void UARTRxDataHandler(const u8 *rxdata, s32 n)
{
    s32 i, len;
    char ch;

    //Setup date/time using data received within UART
    for(i = 0; i < n; i++)
    {
        ch = (char)rxdata[i];

        if (ch == 'd' && dtimestr[0] == 0)
        {
            dtimestr[0] = ch;
            dtimestr[1] = 0;
        }
        else if ((ch >= '0' && ch <= '9') || ch == '.')
        {
            len = strlen(dtimestr);

            if (len > 0)
            {
                dtimestr[len]     = ch;
                dtimestr[len + 1] = 0;
            }
        }
        else
        {
            dtimestr[0] = 0;
        }

        if (strlen(dtimestr) >= 20)
        {
            if (dtimestr[0]  == 'd' &&
                dtimestr[5]  == '.' && dtimestr[8]  == '.' &&
                dtimestr[11] == '.' && dtimestr[14] == '.' &&
                dtimestr[17] == '.')
            {
                //Set date time
                u32 val;
                DateTime dt;

                dt.year  = (u32)atoi(&dtimestr[1]);
                dt.month = (u32)atoi(&dtimestr[6]);
                dt.day   = (u32)atoi(&dtimestr[9]);
                dt.hour  = (u32)atoi(&dtimestr[12]);
                dt.min   = (u32)atoi(&dtimestr[15]);
                dt.sec   = (u32)atoi(&dtimestr[18]);

                if (dt.year >= 2019 &&
                    (dt.month >= 1 && dt.month <= 12) &&
                    (dt.day   >= 1 && dt.day   <= 31) &&
                     dt.hour <= 23 && dt.min <= 59 && dt.sec <= 59)
                {
                    RTCWrite(DateTimeToBinary(&dt, &val));
                }
            }

            dtimestr[0] = 0;
        }
    }
}



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
                       "Main: %u cyc/s\n\n",
                        GetHCLK(),             //Current CPU clock, Hz
                        GetUARTSpeed(UART_NN), //Current UART speed, bit/s                        
                        GetI2CSpeed(I2C_N1),   //Current I2C speed, bit/s                        
                        mainSpeed);            //Count of main cycles per second

            ledcnt++;
            if (ledcnt & 1) { SetLed(LED_2);   }
            else            { ResetLed(LED_2); }

            break;
        }

        case BUTTON_UP:
        {           
            //std_printf("Up\n");
            ScrollTextBoxV(&textBox, -2);

            PrintSensorsData(0);
            break;
        }

        case BUTTON_DOWN:
        {            
            //std_printf("Down\n");

            ScrollTextBoxV(&textBox, 2);

            PrintSensorsData(0);            
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

            /*static u32 timMs = 0;
            u32 tmval = 0xFFFFFFFEU;
            u32 d = timMs - tmval;
            if (d > 10) std_printf("Y %u\n", d);
            else        std_printf("N %u\n", d);
            timMs++;*/
            break;
        }

        case BUTTON_RIGHT:
        {
            s32 r;
            u8 regs[32];


            std_printf("Right\n");            
            BMP280ResetLeveling(pBMP280Sensor);

            memset(regs, 0, 32);

            r = I2CReadRegister(I2C_N1, 0x68, 0x3A, regs, 1);

            std_printf("RD %i: "
                       " %02X%02X; %02X%02X; %02X%02X;" //X,Y,Z                       
                       "\n",
                       r,
                       regs[0],regs[1], regs[2],regs[3], regs[4],regs[5]);
            break;
        }        

        #if (defined(TARGET_EVBRD) && TARGET_EVBRD == EVBRD_1986BE1T)
        case BUTTON_BACK:
        {
            //std_printf("Back\n");
            s32 i, count;
            SensorConfig cfgs[SENSOR_MAXCOUNT];
            SensorConfig *p;

            count = AutoconfigureSensors(cfgs, SENSOR_MAXCOUNT);

            std_printf("AUTOCONF (%i):\n", count);
            for(i = 0; i < count; i++)
            {
                p = &cfgs[i];
                std_printf("%s %02X:%02X%02X%02X%02X%02X%02X:%02X %s\n",
                           p->name,
                           p->addr[0], p->addr[1], p->addr[2], p->addr[3],
                           p->addr[4], p->addr[5], p->addr[6], p->addr[7],
                           SensorTypeToString(p->type)
                           );
            }

            break;
        }

        #endif /* EVBRD_1986BE1T */
    } //switch(buttonCurrent)
}

/*===========================================================================
 * INITIALIZATION AND MAIN FUNCTION
 *===========================================================================*/

/* Main function */
void main(void)
{    
    InitializeDevice();

    while(1)
    {
        KeyboardHandler();

        UpdateSensorsState();

        mainCounter++;
    }
}

/*===========================================================================
 * End of file sensor-main.c
 *===========================================================================*/
