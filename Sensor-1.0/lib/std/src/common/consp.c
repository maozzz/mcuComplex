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
 *  File consp.c: Console output functions (within UART)
 *
 *===========================================================================*/

#include "../../inc/consp.h"

#if (PRINT_OUTPUT == PRINT_UART)

/* PRINT_OUTPUT == PRINT_UART */
#elif (PRINT_OUTPUT == PRINT_CONSOLE)

/* PRINT_OUTPUT == PRINT_CONSOLE */
#elif (PRINT_OUTPUT == PRINT_TOFILE)

//Output file descriptor
FILE *hFileConPrint;
//Temporary buffer
char consPrintBuf[CONSPBUF_LENGTH] = { 0 };

/* PRINT_OUTPUT == PRINT_TOFILE */
#else

/* No output: nothing */

/* PRINT_OUTPUT == PRINT_NULL */
#endif 


#if (PRINT_OUTPUT != PRINT_NULL)

#include "../../bastypes.h"
#include "../../inc/stdcf.h"
#include <string.h>

//Count of hexadecimal values per line. Power of 2: [8, 16, 32]
#define HEX_LINE_SIZE		32

/* Print buffer as hex to console/uart/file */
void ConPrintBuf(const void *buf, s32 length, u32 opts)
{
	u8 *pbuf = (u8*)buf;
	s32 i = 0;
	char *phex, *pchars;
	//Temporary buffers
	char  offset[12];
	char   hex[(HEX_LINE_SIZE * 3 + 4)];
	char chars[(HEX_LINE_SIZE + 4)];

	if (opts & 4)
	{
		ConPrintf("const u8 data_%08X[%i] =\n{\n\t", (u32)buf, length);

		//Print buffer as C-array
		while(i++ < length)
		{
			//Byte to hexadecimal string
			ConPrintf("0x%02X, ", *pbuf);

			//Increment pointer to current byte
			pbuf++;

			//End line: print LF char
			if (!(i & 0x0F))
			{
				ConPrintf("\n\t");
			}			
		}

		ConPrintf("\n};\n");

		return;
	}

	//Reset pointers to start
	phex   = hex;
	pchars = chars;

	while(i++ < length)
	{
		//Byte to hexadecimal string
		std_sprintf(phex, "%02X ", *pbuf);
		phex += 3;

		//Byte to char
		if (opts & 1)
		{
			if (*pbuf >= 0x20 && *pbuf <= 0x7E)
				*pchars++ = (char)*pbuf;
			else
				*pchars++ = '.';
		}

		//Increment pointer to current byte
		pbuf++;

		//Print line of 16 bytes
		if (!(i & (HEX_LINE_SIZE - 1)))
		{
			//Terminate string with 0
			*phex   = 0x00;
			*pchars = 0x00;

			//Offset string value
			if (opts & 2)
				std_sprintf(offset, "%08X", i - HEX_LINE_SIZE);

			//<offset> <hex values> <chars>
			ConPrintf("%s  %s %s\n",
				(opts & 2) ? offset : "\0",
				hex,
				(opts & 1) ? chars : "\0");

			//Reset pointers to start
			phex   = hex;
			pchars = chars;
		}
	}

	//Print line of HEX_LINE_SIZE bytes
	//(input length is not aligned by HEX_LINE_SIZE)
	if (length & (HEX_LINE_SIZE - 1))
	{
		s32 cnt = HEX_LINE_SIZE - (length & (HEX_LINE_SIZE - 1));
		memset(phex,   0x20, 3 * cnt);
		memset(pchars, 0x20, cnt);

		hex[HEX_LINE_SIZE * 3] = 0;
		chars[HEX_LINE_SIZE]   = 0;

		if (opts & 2)
			std_sprintf(offset, "%08X", length & ~(HEX_LINE_SIZE - 1));

		ConPrintf("%s  %s %s\n",
			(opts & 2) ? offset : "\0",
			hex,
			(opts & 1) ? chars : "\0");
	}
}

#endif /* PRINT_OUTPUT != PRINT_NULL */

/*===========================================================================
 * End of file consp.c
 *===========================================================================*/

