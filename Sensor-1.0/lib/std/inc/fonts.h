/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File fonts.h: Fonts and codepages definitions.
 *
 *===========================================================================*/

#ifndef __FONTS_H
#define __FONTS_H

#include "../../bastypes.h"

//Font IDs
#define FONTID_6x8              0x00 //Font 6x8
#define FONTID_7x10             0x01 //Font 7x10
#define FONTID_7x10b            0x02 //Font 7x10 bold
#define FONTID_12x16            0x03 //Font 12x16
#define FONTID_18x24            0x04 //Font 18x24
#define FONTID_24x32            0x05 //Font 24x32

//Codepage IDs
#define CPID_IBM866             0x0E //IBM866 (CP866)
#define CPID_WIN1251            0x32 //Windows-1251
#define CPID_KOI8_R             0x33 //KOI8-R
#define CPID_KOI7_0             0x34 //KOI7-0
#define CPID_KOI7_1             0x35 //KOI7-1
#define CPID_MTK_2              0x36 //MTK-2
#define CPID_ISO8859_5          0x1D //ISO-8859-5 (cyrillic)
#define CPID_MAC				0x37 //Macintosh
#define CPID_IBM855				0x06 //IBM855 (CP855)
#define CPID_PC1602LRSx     	0x81 //LCD PC1602LRSx
#define CPID_PC1602LRSFNHx    	0x82 //LCD PC1602LRS-FNHx

/* Represents font parameters */
typedef struct tag_FONT
{
        u16 width;              //Symbol width, points
        u16 height;             //Symbol height, points

        u16 byteRowCount;       //Symbol row count
        u16 bytesPerSymbol;     //Bytes per symbol

        const u8 *data;         //Table font data address
        u32 dataLength;         //Font data length
} FONT;



/* Represents codepage parameters */
typedef struct tag_CODEPAGE
{
        u32 cpid;
        const u8 *encoding;
        u32 offset;
        u32 length;
} CODEPAGE;

extern FONT font6x8_win1251;   // Font 6õ8  points
extern FONT font7x10_win1251;  // Font 7õ10 points
extern FONT font7x10b_win1251; // Font 7õ10 points bold
extern FONT font12x16_win1251; // Font 12õ16 points
extern FONT font18x24_win1251; // Font 18õ24 points
extern FONT font24x32_win1251; // Font 24x32 points

extern CODEPAGE cp1251;          //Windows-1251
extern CODEPAGE cp866;           //CP866 (IBM866)
extern CODEPAGE cpKOI8_R;        //KOI8-R
extern CODEPAGE cpKOI7_0;        //KOI7-0 (7bit latin capital/small letters)
extern CODEPAGE cpKOI7_1;        //KOI7-1 (7bit latin/russian capital letters)
extern CODEPAGE cpMTK_2;         //MTK-2  (5bit telegraph encoding)
extern CODEPAGE cpISO8859_5;     //ISO-8859-5 (cyrillic)
extern CODEPAGE cpMAC;		     //Macintosh
extern CODEPAGE cp855;		     //CP855 (IBM855)
extern CODEPAGE cpPC1602LRSx;    //LCD PC1602LRSx
extern CODEPAGE cpPC1602LRSFNHx; //LCD PC1602LRS-FNHx

#endif /* __FONTS_H */

/*===========================================================================
 * End of file fonts.h
 *===========================================================================*/

