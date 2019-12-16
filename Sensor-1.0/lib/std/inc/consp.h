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
 *  File consp.h: Console output definitions.
 *
 *===========================================================================*/

#ifndef __CONSP_H
#define __CONSP_H

#include <globaldef.h>
#include "../../bastypes.h"

#define PRINT_NULL              0
#define PRINT_UART              1
#define PRINT_CONSOLE           2
#define PRINT_TOFILE            3

#ifndef PRINT_OUTPUT
#define PRINT_OUTPUT            PRINT_NULL
#endif /* !PRINT_OUTPUT */


#if (PRINT_OUTPUT == PRINT_UART)

#include "stdcf.h"

#define OpenConout(fname)
#define CloseConout()

#define ConReadBuf(a, b)        0
#define ConWriteBuf(a, b)		std_printf("%s", a)
#define ConPrintf(...)			std_printf(__VA_ARGS__)

/* Print buffer as hex within UART */
void ConPrintBuf(const void *buf, s32 length, u32 opts);

/* PRINT_OUTPUT == PRINT_UART */
#elif (PRINT_OUTPUT == PRINT_CONSOLE)

#include <stdio.h>
#include "../../bastypes.h"

#define OpenConout(fname)
#define CloseConout()

#define ConReadBuf(a, b)        0
#define ConWriteBuf(a, b)       printf(a)
#define ConPrintf(...)          printf(__VA_ARGS__)

/* Print buffer as hex to console */
void ConPrintBuf(const void *buf, s32 length, u32 opts);

/* PRINT_OUTPUT == PRINT_CONSOLE */
#elif (PRINT_OUTPUT == PRINT_TOFILE)

#include <stdio.h>
#include "stdcf.h"
#include "../../bastypes.h"

#define CONSPBUF_LENGTH   1024
extern FILE *hFileConPrint;
extern char consPrintBuf[];

#define OpenConout(fname) hFileConPrint = fopen(fname, "wb");
#define CloseConout()     fclose(hFileConPrint);

#define ConReadBuf(a, b)        0

#define ConWriteBuf(a, b)\
        fwrite(a, 1, strlen(b), hFileConPrint);

#define ConPrintf(...)\
        fwrite(consPrintBuf, 1, std_sprintf(consPrintBuf, __VA_ARGS__), \
                        hFileConPrint);

/* Print buffer as hex to file */
void ConPrintBuf(const void *buf, s32 length, u32 opts);

/* PRINT_OUTPUT == PRINT_TOFILE */
#else

#define OpenConout(fname)
#define CloseConout()

#define ConReadBuf(a, b)        0
#define ConWriteBuf(a, b)
#define ConPrintf(...)

#define ConPrintBuf(a, b, c)

#endif /* PRINT_OUTPUT == PRINT_NULL */

#endif /* __CONSP_H */

/*===========================================================================
 * End of file consp.h
 *===========================================================================*/

