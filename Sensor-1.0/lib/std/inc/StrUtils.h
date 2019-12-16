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
 *  File StrUtils.h: String utilities definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __STR_UTILS_H
#define __STR_UTILS_H

#include "../../bastypes.h"
#include <string.h>

/* Available string encodings */
typedef enum tag_StringEncoding
{
	Windows1251 = 1251,
	KOI8R       = 20866
} StringEncoding;

//
typedef u32 (*PCONVCODE_ROUTINE)(u32 code);
typedef PCONVCODE_ROUTINE LPCONVCODE_ROUTINE;

//Default string encoding (see StringEncoding enum)
#define DEFAULT_STR_ENCODING	Windows1251

/* Gets pointer to end of string */
#define strend(str)				(char*)&str[(strlen((char*)str))]

/* Sets current string encoding */
void SetStrEncoding(StringEncoding se);

/* Gets current string encoding */
StringEncoding GetStrEncoding();

/* Select convert function */
LPCONVCODE_ROUTINE GetUpperRoutine();

/* Select convert function */
LPCONVCODE_ROUTINE GetLowerRoutine();

/* Determines whether the beginning of string matches the specified string */
u32 StrStartsWith(const char *str, const char *value, u32 ignoreCase);

/* Determines whether the end of string matches the specified string */
u32 StrEndsWith(const char *str, const char *value, u32 ignoreCase);

/* Compare specified strings */
s32 StrCmp(const char *str1, const char *str2, u32 ignoreCase);

/* Returns a copy of string converted to uppercase */
char *StrToUpper(char *dest, const char *src);

/* Returns a copy of string converted to lowercase */
char *StrToLower(char *dest, const char *src);

/* Removes all leading and trailing white-space characters from the string */
char *StrTrim(char *dest, const char *src);

/* Copies one string to another using specified length restriction */
char *StrCopy(char *dest, const char *src, s32 destMaxLength);

/* Merge specified string array to one string */
char *StrMerge(char *dest, const char **srcs, s32 count, s32 destMaxLength);

/* Convert string at hexadecimal format
   to array of unsigned 8-bit integers */
s32 StrToArray(u8 *dest, const char *src);

/* Convert first n chars of string at hexadecimal format
   to array of unsigned 8-bit integers */
s32 StrNToArray(u8 *dest, const char *src, s32 n);

/* Convert string to 64-bit unsigned integer */
u64 StrToU64(const char *src, u32 isHex);

/* Convert string to 32-bit unsigned integer */
u32 StrToU32(const char *src, u32 isHex);

/* Convert 64-bit unsigned integer to it string representation */
char *U64ToStr(char *dest, u64 num, u32 toHex);

/* Convert array of unsigned 8-bit words to it string representation */
char *ArrayToString(char *dest, const u8 *src, s32 length);

/* Split string by arguments using specified splitter chars */
int StrSplitArguments(char *src, const char *split,	int narg, char *argv[]);

#endif /* !__STR_UTILS_H */

/*===========================================================================
 * End of file StrUtils.h
 *===========================================================================*/
