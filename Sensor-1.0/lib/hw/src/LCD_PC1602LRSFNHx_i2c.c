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
 *  File LCD_PC1602LRSFNHx_i2c.c: LCD PC1602x, PC2004x functions.
 *                                I2C interface, PCF8574x converter.
 *                                Vdd=5V.
 *
 *===========================================================================*/

#include "../inc/LCD.h"
#include "../../mc/inc/SysTimer.h"
#include "../../mc/inc/I2C.h"
#include <string.h>

//LCD_INV_CLKCYC = 1 / (clkCyc * 1000)
#ifndef LCD_INV_CLKCYC
#define LCD_INV_CLKCYC      0.000125F
#endif /* !LCD_INV_CLKCYC */

//P7 P6 P5 P4 P3 P2 P1 P0   PCF8574x 8 bit port
//D7 D6 D5 D4 -  E  RW RS   LCD inputs

//PCF8574x eval.board (top view)
//- SCL
//- SDA        [VRES]      [PCF8574]                     -
//- Vdd                                               led-
//- GND
//      GND Vdd Vo P0 P1 P2 x  x  x  x  P4 P5 P6 P7 A  K
//        |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |

//      GND Vdd Vo RS RW E  D0 D1 D2 D3 D4 D5 D6 D7 A  K
//                          [LCD]


#define LCDInRS     (1 << 0)
#define LCDInRW     (1 << 1)
#define LCDInE      (1 << 2)

//Backlight ON (define 0 to disable)
#define LCDInLED    (1 << 3)

//Port IO counters
static u32 LCD_Pause    = 0;

//Count of HCLK clocks per usec
static u32 hclk_usec;

//I2C address of PCF8574x converter
//PCF8574 : 0100xxx = 0x20
//PCF8574A: 0111xxx = 0x38
//Address by default: 0x20 + [A2,A1,A0]=[1,1,1]
static u8 PCF8574x_i2caddr = (0x20 | 0x07);

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
    hclk_usec = (u32)(hclk / 1000000);

    //Common counters calculation
    LCD_Pause = LCDGetCounter(LCD_Pause_ns, hclk_usec);
}



//#define WaitReady()     WaitInterval(LCD_Pause)
__STATIC_INLINE void WaitReady()
{
	u32 cycCnt;
	WaitInterval(LCD_Pause);	
}



/* Write LCD cell/command */
__STATIC_INLINE void LCDWriteCell0(u32 addr, u32 value)
{    
    u8 txdata[8];
    u8 lcdrs;

    //P7 P6 P5 P4 P3 P2 P1 P0   PCF8574x 8 bit port
    //D7 D6 D5 D4 L  E  RW RS   LCD inputs

    //RS: 1/0 by address
    lcdrs = addr ? LCDInRS : 0;

    //Address + RW=0 (write)
    txdata[0] = (PCF8574x_i2caddr << 1);

    //High 4 bits and E=1
    txdata[1] = (u8)(value & 0xF0) | LCDInE | LCDInLED | lcdrs;

    //High 4 bits and E=0
    txdata[2] = (u8)(value & 0xF0) | LCDInLED | lcdrs;

    //Low 4 bits and E=1
    txdata[3] = (u8)((value << 4) & 0xF0) | LCDInE | LCDInLED | lcdrs;

    //Low 4 bits and E=0
    txdata[4] = (u8)((value << 4) & 0xF0) | LCDInLED | lcdrs;

    //Tx data within I2C and wait command execution time
    if (I2CWrite(I2C_N1, txdata, 5) == 5)
    {        
    }

    WaitReady();
}



/* Write LCD cell/command */
__STATIC_INLINE void WriteAddr(u32 addr, u32 value)
{    
    u8 txdata[8];
    u8 lcdrs;

    //P7 P6 P5 P4 P3 P2 P1 P0   PCF8574x 8 bit port
    //D7 D6 D5 D4 L  E  RW RS   LCD inputs

    //RS: 1/0 by address
    lcdrs = addr ? LCDInRS : 0;

    //Address + RW=0 (write)
    txdata[0] = (PCF8574x_i2caddr << 1);

    //High 4 bits and E=1
    txdata[1] = (u8)(value & 0xF0) | LCDInE | LCDInLED | lcdrs;

    //High 4 bits and E=0
    txdata[2] = (u8)(value & 0xF0) | LCDInLED | lcdrs;

    I2CWrite(I2C_N1, txdata, 3);
}



/* LCD initialization */
void InitializeLCD(u32 hclk)
{
    u8 txdata[2];

    //This function must be defined at "targetlcd.h"
    InitializeLCDPorts();

    LCDSetHCLK(hclk);


    //Set PCF8574 port to zeros
    txdata[0] = (PCF8574x_i2caddr << 1);
    txdata[1] = 0;

    I2CWrite(I2C_N1, txdata, 2);

    WaitReady();
}



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
        //defines this initialization sequence (4-bit interface)

        //Wait >15ms after Vdd rise to 4.5V:
        //assumed that completed when MCU initializing

        //DL=1: 8bit data + pause >4.1 ms
        WriteAddr(LCD_CMD1, LCD_CMD_FUNCTION_SET|LCD_CMD_DL);
        WaitSysTimer(0, 6000 * hclk_usec);

        //DL=1: 8bit data + pause >100usec
        WriteAddr(LCD_CMD1, LCD_CMD_FUNCTION_SET|LCD_CMD_DL);
        WaitSysTimer(0, 500 * hclk_usec);

        //DL=0: TOGGLE TO 4bit data interface
        WriteAddr(LCD_CMD1, LCD_CMD_FUNCTION_SET);
        WaitReady();

        //DL=0: 4bit data; N=1: 2 lines (4 lines PC2004); F=0: font 5x8
        LCDWriteCell(LCD_CMD1, LCD_CMD_FUNCTION_SET|LCD_CMD_N);

        //D=0: display OFF
        LCDWriteCell(LCD_CMD1, LCD_CMD_DISPLAY_CONTROL);

        //Clear display + pause >1.64ms
        LCDWriteCell(LCD_CMD1, LCD_CMD_DISPLAY_CLEAR);
        WaitSysTimer(0, 3000 * hclk_usec);

        //ID=1: increment address; SH=0: no shift screen
        LCDWriteCell(LCD_CMD1, LCD_CMD_MODESET|LCD_CMD_ID);

        //D=1: display ON; C=0: no cursor; B=0: no blink
        LCDWriteCell(LCD_CMD1, LCD_CMD_DISPLAY_CONTROL|LCD_CMD_D);
    }
    else
    {
        //D=0: display OFF; C=0: no cursor; B=0: no blink
        LCDWriteCell(LCD_CMD1, LCD_CMD_DISPLAY_CONTROL);
    }
}



/* Select I2C address */
void LCDSelectI2CAddress(u8 i2caddr)
{    
    PCF8574x_i2caddr = i2caddr;
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



/* Write LCD cell/command */
s32 LCDWriteCell(u32 addr, u32 value)
{                    
    LCDWriteCell0(addr, value);
    return 1;
}



/* Read LCD cell/status */
u32 LCDReadCell(u32 addr)
{
    return 0;
}



/* */
s32 LCDWrite(u32 addr, u8 *data, s32 length)
{
    s32 i;    

    //Set DDRAM address
    LCDWriteCell0(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | addr);

    //Clear line with spaces
    for(i = 0; i < (s32)(LCD_WIDTH / 6) && i < length; i++)
    {
        LCDWriteCell0(LCD_DATA1, data[i]);
    }

    return i;
}



/* */
s32 LCDRead(u32 addr, u8 *data, s32 length)
{
    return 0;
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
        LCDWriteCell0(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | ymap[y]);

        //Send char codes to LCD
        for(x = 0; x < xMax; x++)
        {
            LCDWriteCell0(LCD_DATA1, 0x20);
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

        //Set DDRAM address: start of current line
        LCDWriteCell0(LCD_CMD1, LCD_CMD_SET_DDRAM_ADDR | ymap[y]);

        //Send char codes to LCD
        for(x = 0; x < xMax; x++)
        {
            //Convert encoding
            if (scp->cp)
                bch = GetLetterIndex(scp->cp, *pchar);
            else
                bch = *pchar;

            LCDWriteCell0(LCD_DATA1, bch);
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

/*===========================================================================
 * End of file LCD_PC1602LRSFNHx_i2c.c
 *===========================================================================*/
