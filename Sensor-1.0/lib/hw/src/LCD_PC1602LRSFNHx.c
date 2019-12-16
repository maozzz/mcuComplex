/*===========================================================================
 *
 *  Hardware library
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
 *  File LCD_PC1602LRSFNHx.c: LCD PC1602LRS-FNHx functions.
 *                            8-bit interface [DB7..DB0,RS,RW,E]
 *                            Vdd=5V.
 *
 *===========================================================================*/

#include "../inc/LCD.h"
#include "../../mc/inc/SysTimer.h"

#include <string.h>

//LCD wait counters
//("__LCD_USE_EXT_BUS" must be defined at "targetlcd.h")
#ifdef __LCD_USE_EXT_BUS

//LCD_INV_CLKCYC = 1 / (clkCyc * 1000)
#ifndef LCD_INV_CLKCYC
#if defined(__CMCARM__)
#define LCD_INV_CLKCYC      0.000180F
#else
#define LCD_INV_CLKCYC      0.000250F
#endif /* !__CMCARM__ */
#endif /* !LCD_INV_CLKCYC */

#else /* __LCD_USE_EXT_BUS */

//LCD_INV_CLKCYC = 1 / (clkCyc * 1000)
#ifndef LCD_INV_CLKCYC
#define LCD_INV_CLKCYC      0.000125F
#endif /* !LCD_INV_CLKCYC */

static u32 LCD_tas      = 0;
static u32 LCD_tah      = 0;
static u32 LCD_PWeh     = 0;

#endif /* !__LCD_USE_EXT_BUS */

//Port IO counters
static u32 LCD_Pause    = 0;

//Count of HCLK clocks per usec
static u32 hclk_usec;

//Wait specified interval
#define WaitInterval(cnt)   cycCnt = (cnt); while(cycCnt--);

/* */
static u32 LCDGetCounter(u32 tmNs, u32 hclkMHz)
{
        //tm = cycCnt * clkCyc / hclk
        //cycCnt = tm * hclk / clkCyc
        //cycCnt = tmNs * 10^-9 * hclkMHz * 10^6 / clkCyc;
        //cycCnt = tmNs * hclkMHz / (clkCyc * 1000);
		return (u32)(tmNs * hclkMHz * LCD_INV_CLKCYC);
}


/* */
void LCDSetHCLK(u32 hclk)
{
        //Get hclk at MHz
        u32 hclkMHz = (u32)(hclk / 1000000);

        #ifdef __LCD_USE_EXT_BUS

        u32 wsSetup, wsActive, wsHold;

        wsSetup  = CalcClocks(LCD_tas_ns,  hclkMHz);
        wsActive = CalcClocks(LCD_PWeh_ns, hclkMHz);
        wsHold   = CalcClocks(LCD_tah_ns,  hclkMHz);

        //Setup extbus clocks
        EBusSetClockEx(LCD_CMD1, wsSetup, wsActive, wsHold);
        EBusSetClockEx(LCD_DATA1, wsSetup, wsActive, wsHold);        

        //Use LCD as RAM
        EBusSelectMemory(EXTBUS_RAM);

        #else /* __LCD_USE_EXT_BUS */

        //Port I/O counters calculation
        LCD_tas      = LCDGetCounter(LCD_tas_ns, hclkMHz);
        LCD_PWeh     = LCDGetCounter(LCD_PWeh_ns, hclkMHz);
        LCD_tah      = LCDGetCounter(LCD_tah_ns, hclkMHz);

        #endif /* !__LCD_USE_EXT_BUS */

        //Common counters calculation
        LCD_Pause    = LCDGetCounter(LCD_Pause_ns, hclkMHz);

        hclk_usec = hclkMHz;
}




/* LCD initialization */
void InitializeLCD(u32 hclk)
{   
        //This function must be defined at "targetlcd.h"
        InitializeLCDPorts();

        LCDSetHCLK(hclk);

        LCD_CE_Off();
}




#ifdef __LCD_USE_EXT_BUS

/* Write address using external bus */
#define WriteAddr(address, data)            *((vu8*)(address)) = (u8)(data)

/* Read address using external bus */
#define ReadAddr(address)                   *((vu8*)(address))

#else /* __LCD_USE_EXT_BUS */

/* Write data to LCD without pause */
static void WriteAddr(u32 addr, u8 value)
{
        u32 cycCnt;
        u16 vtmp;

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Set address
        LCD_ADDR_Set(addr, vtmp);

        //RW (WE) = 0: enable write
        LCD_WE_Act();

        //Turn data port to output
        LCD_DB_Out();

        //Write data[7..0]
        LCD_DB_Write(value, vtmp);

        //Wait tas
        WaitInterval(LCD_tas);

        //E (CLOCK) = 1: active
        LCD_E_Act();

        //Wait PWeh
        WaitInterval(LCD_PWeh);

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Wait tah
        WaitInterval(LCD_tah);

        //RW (WE) = 1: disable write
        LCD_WE_Off();

        //Turn data port to input
        LCD_DB_In();
}


/* Read data from LCD without pause */
static u8 ReadAddr(u32 addr)
{
        u32 cycCnt;
        u16 vtmp;
        u16 value;

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Set address
        LCD_ADDR_Set(addr, vtmp);

        //Turn data port to input
        LCD_DB_In();

        //RW (WE) = 1: enable read
        LCD_WE_Off();

        //Wait tas
        WaitInterval(LCD_tas);

        //E (CLOCK) = 1: on
        LCD_E_Act();

        //Wait PWeh
        WaitInterval(LCD_PWeh);

        //Read data[7..0]
        LCD_DB_Read(value);

        //E (CLOCK) = 0: off
        LCD_E_Off();

        //Wait tah
        WaitInterval(LCD_tah);

        return (u8)value;
}

#endif /* !__LCD_USE_EXT_BUS */


#ifdef __LCD_USE_FLAG_BUSY

__STATIC_INLINE void WaitReady()
{
    while((ReadAddr(LCD_CMD1) & LCD_FLAG_BUSY)) __NOP;
}

#else /* __LCD_USE_FLAG_BUSY */

//#define WaitReady()     WaitInterval(LCD_Pause)
__STATIC_INLINE void WaitReady()
{
	u32 cycCnt;
	WaitInterval(LCD_Pause);	
}

#endif /* !__LCD_USE_FLAG_BUSY */


/* */
void LCDReset(u32 addr)
{
    //LCD PC1602x does not have "RESET" input:
    //clear LCD
    LCDClear(addr);
}



/* */
void TurnLCD(u8 turn)
{
    if (turn)
    {
        //Powertip Technology, Inc
        //defines this initialization sequence (8-bit interface)

        //Wait >15ms after Vdd rise to 4.5V:
        //assumed that completed when MCU initializing

        //DL=1: 8 data bits + pause >4.1ms
        WriteAddr(LCD_CMD1, LCD_CMD_FUNCTION_SET|LCD_CMD_DL);
        WaitSysTimer(0, 6000 * hclk_usec);

        //DL=1: 8 data bits + pause >100usec
        WriteAddr(LCD_CMD1, LCD_CMD_FUNCTION_SET|LCD_CMD_DL);
        WaitSysTimer(0, 500 * hclk_usec);


        //DL=1: 8 data bits; N=1: 2 lines; F=0: font 5x8 points
        WriteAddr(LCD_CMD1, LCD_CMD_FUNCTION_SET|LCD_CMD_DL|LCD_CMD_N);
        WaitReady();

        //DL=1: 8 data bits; N=1: 2 lines; F=0: font 5x8 points
        WriteAddr(LCD_CMD1, LCD_CMD_FUNCTION_SET|LCD_CMD_DL|LCD_CMD_N);
        WaitReady();

        //D=0: display OFF;
        WriteAddr(LCD_CMD1, LCD_CMD_DISPLAY_CONTROL);
        WaitReady();

        //Clear display + pause >1.64ms
        WriteAddr(LCD_CMD1, LCD_CMD_DISPLAY_CLEAR);
        WaitSysTimer(0, 3000 * hclk_usec);

        //ID=1: increment address; SH=0: no shift screen
        WriteAddr(LCD_CMD1, LCD_CMD_MODESET|LCD_CMD_ID);
        WaitReady();

        //D=1: display ON; C=0: no cursor; B=0: no blink
        WriteAddr(LCD_CMD1, LCD_CMD_DISPLAY_CONTROL|LCD_CMD_D);
        WaitReady();
    }
    else
    {
        //D=0: display OFF
        WriteAddr(LCD_CMD1, LCD_CMD_DISPLAY_CONTROL);
        WaitReady();
    }
}



/* Select I2C address */
void LCDSelectI2CAddress(u8 i2caddr)
{
    //Not I2C interface: no actions
}



/* Write page to LCD using specified starting address and length */
s32 LCDWritePage(u32 page, u32 addr, u8 *data, s32 length)
{
    //no actions
    return 0;
}



/* Read page from LCD using specified starting address and length */
s32 LCDReadPage(u32 page, u32 addr, u8 *data, s32 length)
{
    //no actions
    return 0;
}




/* Write LCD cell */
s32 LCDWriteCell(u32 addr, u32 value)
{                
    //Write cell
    WriteAddr(addr, (u8)value);
    WaitReady();
    return 1;
}



/* Read LCD cell */
u32 LCDReadCell(u32 addr)
{
    return ((u32)ReadAddr(addr));
}



/* */
s32 LCDWrite(u32 addr, u8 *data, s32 length)
{
    s32 i;

    //Set DDRAM address
    WriteAddr(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | addr);
    WaitReady();

    //Clear line with spaces
    for(i = 0; i < (s32)(LCD_WIDTH / 6) && i < length; i++)
    {
        WriteAddr(LCD_DATA1, data[i]);
        WaitReady();
    }

    return i;
}



/* */
s32 LCDRead(u32 addr, u8 *data, s32 length)
{
    s32 i;

    //Set DDRAM address
    WriteAddr(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | addr);
    WaitReady();

    //Clear line with spaces
    for(i = 0; i < (s32)(LCD_WIDTH / 6) && i < length; i++)
    {
        data[i] = ReadAddr(LCD_DATA1);
        WaitReady();
    }

    return i;
}


//LCD 1602 DDRAM mapping
static const u8 lcd1602_ymap[2] = { 0, 64 };

//LCD 2004 DDRAM mapping
static const u8 lcd2004_ymap[4] = { 0, 40,  20,  84 };


/* Clear LCD */
void LCDClear(u32 addr)
{
    u32 y, x;
    u32 yMax = (u32)(LCD_HEIGHT / 8);
    u32 xMax = (u32)(LCD_WIDTH / 6);
    const u8 *ymap;

    if (xMax == 20 && yMax == 4)
        ymap = lcd2004_ymap;
    else
        ymap = lcd1602_ymap;

    for(y = 0; y < yMax; y++)
    {
        //Set DDRAM address: start of current line
        WriteAddr(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | ymap[y]);
        WaitReady();

        //Clear LCD line with spaces (0x20)
        for(x = 0; x < xMax; x++)
        {
            WriteAddr(LCD_DATA1, 0x20);
            WaitReady();
        }
    }
}



/* Draw text on LCD using specified parameters */
void LCDDrawText(ScreenParams *scp)
{
    u32 y, x;
    u32 yMax = (u32)(LCD_HEIGHT / 8);
    u32 xMax = (u32)(LCD_WIDTH / 6);
    const u8 *ymap;
    u8 *pchar;
    u8 bch;

    if (xMax == 20 && yMax == 4)
        ymap = lcd2004_ymap;
    else
        ymap = lcd1602_ymap;

    if (yMax > (u32)scp->height)
        yMax = (u32)scp->height;

    if (xMax > (u32)scp->width)
        xMax = (u32)scp->width;

    for(y = 0; y < yMax; y++)
    {
        //Pointer to start of text line
        pchar = (u8*)&scp->buffer[(y * scp->width)];

        //Set DDRAM address: start of current line (aligned 64)
        WriteAddr(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | ymap[y]);
        WaitReady();

        //Send char codes to LCD
        for(x = 0; x < xMax; x++)
        {
            //Convert encoding
            if (scp->cp)
                bch = GetLetterIndex(scp->cp, *pchar);
            else
                bch = *pchar;

            WriteAddr(LCD_DATA1, bch);
            WaitReady();
            pchar++;
        }
    }
}



/* Draw graphic on LCD using specified parameters */
void LCDDrawGraphic(ScreenParams *scp)
{
    //LCD PC1602 is char display: can't draw graphic, clear display
    LCDClear(LCD_BASEADDR);
}



/* */
/*void LCDSetCGRAM()
{
    s32 h;

    //Set address counter to CGRAM
    WriteAddr(LCD_CMD1, LCD_CMD_SET_CGRAM_ADDR | 8);
    WaitReady();    

    //Put char image to CGRAM
    for (h = 0; h < 6; h++)
    {
        WriteAddr(LCD_DATA1, 0x05);
        WaitReady();
    }


    //Set address counter to DDRAM
    WriteAddr(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | 0);
    WaitReady();
}*/

/*===========================================================================
 * End of file LCD_PC1602LRSFNHx.c
 *===========================================================================*/
