/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File stdcf.h: Analogs for standard C functions.
 *
 *===========================================================================*/

#ifndef __STDCF_H
#define __STDCF_H

#include <stdarg.h>

/* */
typedef struct tag_PrintContext
{
	char *dest;
	int i;
	void *s;
} PrintContext;

/* */
typedef void (*pWriteCharRoutine)(PrintContext *pc, char c);
typedef pWriteCharRoutine LpWriteCharRoutine;

/* */
extern LpWriteCharRoutine std_pfprintchar;

/* */
int subst_vsprintf0(LpWriteCharRoutine writeChar,
		    PrintContext *pc, const char *format, va_list argv);

#if defined(__GNUC__)
#define _STDCF_ATTR(attrs) __attribute__ (attrs)
#else
#define _STDCF_ATTR(attrs)
#endif

//#if (!defined(__NO_STDCF__) && defined(__GNUC__))
#if (!defined(__NO_STDCF__))
/* Select substitutions */
/* Substitution for "sprintf". */
_STDCF_ATTR((__format__(__printf__, 2, 3)))
int subst_sprintf(char *dest, const char *format, ...);

/* Substitution for "vsprintf". */
int subst_vsprintf(char *dest, const char *format, va_list argv);

#define std_sprintf		subst_sprintf
#define std_vsprintf	subst_vsprintf

#else

#include <stdio.h>

/* Select libc functions */
#define std_sprintf		sprintf
#define std_vsprintf	vsprintf

#endif /* !__GNUC__*/

/* */
_STDCF_ATTR((__format__(__printf__, 1, 2)))
int std_printf(const char *format, ...);

/* */
int std_vprintf(const char *format, va_list argv);

/* */
const char *std_strstr(const char *src, const char *find);

#endif /* !__STDCF_H */

/*===========================================================================
 * End of file stdcf.h
 *===========================================================================*/

