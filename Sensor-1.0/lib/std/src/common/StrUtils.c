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
 *  File ParseUtils.c: String utilities.
 *
 *===========================================================================*/
#define _CRT_SECURE_NO_WARNINGS

#include "../../inc/StrUtils.h"
#include "../../inc/random.h"
#include "../../inc/stdcf.h"
#include <stdio.h>

//Encoding: all 8-bit encodings
//Latin uppercase letters char codes range:
#define E8BIT_LATUPR_BEGIN		0x41
#define E8BIT_LATUPR_END		0x5A
//Latin lowercase letters char codes range:
#define E8BIT_LATLOW_BEGIN		0x61
#define E8BIT_LATLOW_END		0x7A

//Encoding: Windows-1251 (1251)
//Russian uppercase letters char codes range:
#define E1251_RUSUPR_BEGIN		0xC0
#define E1251_RUSUPR_END		0xDF
//Russian lowercase letters char codes range:
#define E1251_RUSLOW_BEGIN		0xE0
#define E1251_RUSLOW_END		0xFF
//Russian uppercase 'YO' letter char code
#define E1251_RUSYOUPR			0xA8
//Russian lowercase 'yo' letter char code
#define E1251_RUSYOLOW			0xB8

//Encoding: KOI8-R (20866)
//Russian uppercase letters char codes range:
#define E20866_RUSUPR_BEGIN		0xE0
#define E20866_RUSUPR_END		0xFF
//Russian lowercase letters char codes range:
#define E20866_RUSLOW_BEGIN		0xC0
#define E20866_RUSLOW_END		0xDF
//Russian uppercase 'YO' letter char code
#define E20866_RUSYOUPR			0xB3
//Russian lowercase 'yo' letter char code
#define E20866_RUSYOLOW			0xA3

//
#define TO_UPPERCASE		1
#define TO_LOWERCASE		0

//Current string encoding
static StringEncoding currentStringEncoding = DEFAULT_STR_ENCODING;

/*===========================================================================
 * Encoding: Windows-1251 (1251)
 *===========================================================================*/

/* Get lowercase code at Windows-1251 encoding */
u32 GetE1251Lower(u32 code)
{			
	if ((code >= E8BIT_LATUPR_BEGIN && code <= E8BIT_LATUPR_END) ||
		(code >= E1251_RUSUPR_BEGIN && code <= E1251_RUSUPR_END))
		return code + 32;	
	else if (code == E1251_RUSYOUPR)
		return E1251_RUSYOLOW;
	else
		return code;	
}



/* Get uppercase code at Windows-1251 encoding */
u32 GetE1251Upper(u32 code)
{			
	if ((code >= E8BIT_LATLOW_BEGIN && code <= E8BIT_LATLOW_END) ||
		(code >= E1251_RUSLOW_BEGIN && code <= E1251_RUSLOW_END))
		return code - 32;	
	else if (code == E1251_RUSYOLOW)
		return E1251_RUSYOUPR;
	else
		return code;	
}

/*===========================================================================
 * Encoding: KOI8-R (20866)
 *===========================================================================*/

/* Get lowercase code at KOI8-R encoding */
u32 GetE20866Lower(u32 code)
{			
	if ((code >= E8BIT_LATUPR_BEGIN  && code <= E8BIT_LATUPR_END) ||
		(code >= E20866_RUSUPR_BEGIN && code <= E20866_RUSUPR_END))
		return code - 32;	
	else if (code == E20866_RUSYOUPR)
		return E20866_RUSYOLOW;
	else
		return code;	
}



/* Get uppercase code at KOI8-R encoding */
u32 GetE20866Upper(u32 code)
{			
	if ((code >= E8BIT_LATLOW_BEGIN  && code <= E8BIT_LATLOW_END) ||
		(code >= E20866_RUSLOW_BEGIN && code <= E20866_RUSLOW_END))
		return code + 32;	
	else if (code == E20866_RUSYOLOW)
		return E20866_RUSYOUPR;
	else
		return code;	
}

/*===========================================================================
 * Select "GetExUpper", "GetExLower" functions
 *===========================================================================*/

/* Select convert function */
LPCONVCODE_ROUTINE GetUpperRoutine()
{	
	switch(currentStringEncoding)
	{
		case KOI8R: 
			return &GetE20866Upper;
		
		case Windows1251: 
		default:
			return &GetE1251Upper;		
	}	
}



/* Select convert function */
LPCONVCODE_ROUTINE GetLowerRoutine()
{	
	switch(currentStringEncoding)
	{
		case KOI8R: 
			return &GetE20866Lower;
		
		case Windows1251: 
		default:
			return &GetE1251Lower;		
	}	
}

/*===========================================================================
 * String functions
 *===========================================================================*/

/* Sets current string encoding */
void SetStrEncoding(StringEncoding se)
{	
	currentStringEncoding = se;
}



/* Gets current string encoding */
StringEncoding GetStrEncoding()
{
	return currentStringEncoding;
}



/* Determines whether the beginning of string matches the specified string */
u32 StrStartsWith(const char *str, const char *value, u32 ignoreCase)
{	
	LPCONVCODE_ROUTINE pfUpper;
	s32 i = 0, lenValue;	

	if (!str || !value)
		return 0;
	
	lenValue = strlen(value);

	//Check string length
	if ((s32)strlen(str) < lenValue)
		return 0;		

	if (ignoreCase)
	{
		pfUpper = GetUpperRoutine();
		
		while(i < lenValue)
		{						
			if (pfUpper((u32)str[i]) != pfUpper((u32)value[i]))
				return 0;

			i++;
		}
	}
	else
	{
		while(i < lenValue)
		{
			if (str[i] != value[i])
				return 0;	

			i++;
		}		
	}

	return 1;
}



/* Determines whether the end of string matches the specified string */
u32 StrEndsWith(const char *str, const char *value, u32 ignoreCase)
{
	s32 lenStr, lenValue;	

	if (!str || !value)
		return 0;
	
	lenStr   = strlen(str);
	lenValue = strlen(value);

	//Check string length
	if (lenStr < lenValue)		
		return 0;
	else
		return StrStartsWith(&str[(lenStr - lenValue)], value, ignoreCase);	
}



/* Compare 2 strings */
s32 StrCmp(const char *str1, const char *str2, u32 ignoreCase)
{		
	if (ignoreCase)
	{		
		s32 i = -1, c1, c2;
		LPCONVCODE_ROUTINE pfUpper = GetUpperRoutine();
						
		do
		{
			i++;
			c1 = pfUpper((u32)str1[i]);
			c2 = pfUpper((u32)str2[i]);						
		}
		while(c1 == c2 && str1[i] != 0 && str2[i] != 0);
				
		if (c1 > c2)
			return 1;
		else if (c1 < c2)
			return -1;
		else
			return 0;		
	}
	else
	{
		return strcmp(str1, str2);	
	}	
}



/* Returns a copy of string converted to specified case */
static char *StrConvertCase(char *dest, const char *src, u32 tocase)
{
	LPCONVCODE_ROUTINE pfConvert;	
	s32 i = 0; 
	s32 len = strlen(src);
		
	//Select uppercase/lowercase function
	if (tocase == TO_UPPERCASE)	
		pfConvert = GetUpperRoutine();	
	else
		pfConvert = GetLowerRoutine();

	while(i < len)
	{
		dest[i] = (char)pfConvert((u32)src[i]);	
		i++;
	}

	dest[i] = 0;

	return dest;
}



/* Returns a copy of string converted to uppercase */
char *StrToUpper(char *dest, const char *src)
{
	return StrConvertCase(dest, src, TO_UPPERCASE);
}



/* Returns a copy of string converted to lowercase */
char *StrToLower(char *dest, const char *src)
{
	return StrConvertCase(dest, src, TO_LOWERCASE);
}



/* Removes all leading and trailing white-space characters from the string */
char *StrTrim(char *dest, const char *src)
{
	s32 i; 
	s32 len = strlen(src);
	s32 chStart, chEnd;
			
	if (!len)
	{
		dest[0] = 0;
		return dest;
	}

	//Skip leading spaces
	i = 0;
	while(i < len &&
		  src[i] == 0x20)
		  i++;

	chStart = i;	

	//Skip trailing spaces
	i = len - 1;
	while(i >= 0 &&
		  src[i] == 0x20)
		  i--;

	chEnd = i;	

	//Copy string to destination buffer
	i = 0;
	while(chStart <= chEnd)			
		dest[i++] = src[chStart++];	

	dest[i] = 0;

	return dest;
}



/* Copies one string to another using specified length restriction */
char *StrCopy(char *dest, const char *src, s32 destMaxLength)
{	
	strncpy(dest, src, destMaxLength);
	dest[destMaxLength - 1]  = 0;
	return dest;
}



/* Merge specified string array to one string */
char *StrMerge(char *dest, const char **srcs, s32 count, s32 destMaxLength)
{
	s32 i = 0;
	s32 freeSpace = destMaxLength;

	dest[0] = 0;

	while(i < count && freeSpace > 0)
	{	
		strncpy(strend(dest), srcs[i], freeSpace);
		freeSpace -= strlen(srcs[i]);
		i++;
	}

	dest[destMaxLength - 1] = 0;

	return dest;
}



/* Convert first n chars of string at hexadecimal format
   to array of unsigned 8-bit integers */
s32 StrNToArray(u8 *dest, const char *src, s32 n)
{
	s32 i = 0;
	char c;

	if (!dest || !src)
		return 0;

	while(i < n)
	{
		c = src[i];

		//c at ['0'-'9', 'A'-'F', 'a'-'f']
		if (c >= 0x30 && c <= 0x39)
			c -= 0x30;
		else if (c >= 0x41 && c <= 0x46)
			c -= 0x37;
		else if (c >= 0x61 && c <= 0x66)
			c -= 0x57;
		else
			break;

		//Odd/even i: lower/major 4 bits
		if (i & 1)
			dest[(i >> 1)] |= (u8)c;
		else
			dest[(i >> 1)]  = (u8)(c << 4);

		i++;
	}

	return ((i + 1) >> 1);
}



/* Convert string at hexadecimal format
   to array of unsigned 8-bit integers */
s32 StrToArray(u8 *dest, const char *src)
{
	return StrNToArray(dest, src, strlen(src));
}



/* Convert string to 64-bit unsigned integer */
u64 StrToU64(const char *src, u32 isHex)
{
	s32 i = strlen(src);
	u64 value = 0, c;

	if (!src || i == 0)
		return (u64)0;

	if (isHex)
	{
		u32 shift = 0;
		while(i-- && shift < 64)
		{			
			c = (u64)src[i];

			//c at ['0'-'9', 'A'-'F', 'a'-'f']
			if (c >= 0x30 && c <= 0x39)		
				c -= 0x30;							
			else if (c >= 0x41 && c <= 0x46)		
				c -= 0x37;				
			else if (c >= 0x61 && c <= 0x66)		
				c -= 0x57;
			else
				break;
			
			value |= c << shift;
			shift += 4;			
		}		
	}
	else
	{
		u64 pow = 1;
		while(i--)
		{			
			c = (u64)src[i];
					
			//c at ['0'-'9']
			if (c >= 0x30 && c <= 0x39)		
				c -= 0x30;
			else
				break;

			value += c * pow;
			pow *= (u64)10;			
		}
	}

	return value;
}



/* Convert 64-bit unsigned integer to it string representation */
char *U64ToStr(char *dest, u64 num, u32 toHex)
{	
	if (!dest)
		return 0;

	if (toHex)
	{
		std_sprintf(dest, "%08x%08x", (u32)(num >> 32), (u32)num);
	}
	else
	{
		u64 m;
		s32 i = 31;
		char str[32];	
	
		str[i] = 0;		
		
		do
		{
			m = num % 0x0AULL;
			num = (u64)(num / 0x0AULL);
			
			i--;
			str[i] = (u32)m + 0x30;			

		} while(num > 0);

		strcpy(dest, &str[i]);
	}

	return dest;
}



/* Convert string to 64-bit unsigned integer */
u32 StrToU32(const char *src, u32 isHex)
{
	return (u32)StrToU64(src, isHex);
}



/* Convert array of unsigned 8-bit words to it string representation */
char *ArrayToString(char *dest, const u8 *src, s32 length)
{
	s32 i = 0;	

    dest[0] = 0;

	if (src)
		while(i < length)	
			std_sprintf(strend(dest), "%02x", src[i++]);

	dest[(i << 1)] = 0;

	return dest;
}



/* Split string by arguments using specified splitter chars */
int StrSplitArguments(char *src, const char *split, int narg, char *argv[])
{
	char *p, *pend;

	int arglen   = 0;
	int argc     = 0;
	int isStr    = 0;
	int isShield = 0;

	//Check parameters
	if (!src || src[0] == 0)
	{
		argv[0] = 0;
		return 0;
	}

	if (narg == 0)
		narg = 100;

	if (!split)
		split = " ";

	//Get pointers to start and end of string
	p    = &src[0];
	pend = &src[strlen(src)];

	//Skip leading spaces and tabs
	while(p < pend && (*p == ' ' || *p == 0x09)) p++;

	//Split arguments
	while(p < pend && argc < narg)
	{
		if (*p == '\\' && !isStr && !isShield)
		{
			//Shift by one char left
			char *ptmp = p;
			while(ptmp < pend)
			{
				*ptmp = *(ptmp + 1);
				ptmp++;
			}

			pend--;
			isShield = 1;
			continue;
		}
		else if (*p == '"' && !isShield)
		{
			if (isStr)
			{
				//Save pointer to argument
				if (arglen == 0)
					argv[argc] = p;

				*p = 0;
				arglen = 0;
				argc++;

				isStr = 0;
			}
			else
				isStr = 1;
		}
		else if (strchr(split, *p) != 0 && !isStr && !isShield)
		{
			if (arglen)
			{
				*p = 0;
				arglen = 0;
				argc++;
			}
		}
		else
		{
			//Save pointer to argument
			if (arglen == 0)
				argv[argc] = p;

			arglen++;
			isShield = 0;
		}

		p++;
	} //while

	//Last argument
	if (arglen)
	{
		*p = 0;
		argc++;
	}

	return argc;
}

/*===========================================================================
 * End of file StrUtils.c
 *===========================================================================*/
