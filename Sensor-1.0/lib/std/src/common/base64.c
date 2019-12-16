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
 *  File base64.h: Base-64 scheme encoder/decoder (RFC 1421).
 *
 *===========================================================================*/

#include "../../inc/base64.h"

#define BASE64_SKIPCHAR		0x3D //'='

#ifdef USE_BASE64_CODE_TABLES

//Base64 codes ([0x2B..0x7A] => [62,63,52-61,0-25,26-51])
static const u8 decTableB64[80] = 
{
  //'+'             '/'
	62,  0,  0,  0, 63,
  //'0' '1'                               '9'
	52, 53, 54, 55, 56, 57, 58, 59,   60, 61,  0,  0,  0,  0,  0,  0,
  //    'A' 'B' 'C'                                               'O'
	 0,  0,  1,  2,  3,  4,  5,  6,    7,  8,  9, 10, 11, 12, 13, 14,
  //'P'                                       'Z'
	15, 16, 17, 18, 19, 20, 21, 22,   23, 24, 25,  0,  0,  0,  0,  0,
  //    'a' 'b' 'c'                                               'o'
	 0, 26, 27, 28, 29, 30, 31, 32,   33, 34, 35, 36, 37, 38, 39, 40,
  //'p'                                       'z'
    41, 42, 43, 44, 45, 46, 47, 48,   49, 50, 51  
};

//Base64 chars ([0..63] => ['A'-'Z','a'-'z','0'-'9','+','/'])
static const char encTableB64[] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/\0";

#else

static char GetEncCharB64(s32 index)
{
	index &= 0x3F;

	if (index <= 25)       //[0..25] => ['A'..'Z'] is [0x41..0x5A]
		return index + 65; //65 = 0x41
	else if (index <= 51)  //[26..51] => ['a'..'z'] is [0x61..0x7A]
		return index + 71; //71 = 0x61 - 26
	else if (index <= 61)  //[52..61] => ['0'..'9'] is [0x30..0x39]
		return index - 4;  //-4 = 0x30 - 52
	else if (index == 62)  //62 => '+' is 0x2B
		return 0x2B;       
	else                   //63 => '/' is 0x2F	
		return 0x2F;       
}

#endif



/* */
s32 EncodeBase64(const u8 *buf, s32 length, u8 *b64buf)
{
	s32 chi = 0, enci = 0, k = 0, tmp = 0;		

	while(chi < length)
	{	
		tmp = (tmp << 8) | buf[chi];
		chi++;
		k++;

		if (k == 3)
		{				
			#ifdef USE_BASE64_CODE_TABLES

			b64buf[enci]     = encTableB64[((tmp >> 18) & 0x3F)];
			b64buf[enci + 1] = encTableB64[((tmp >> 12) & 0x3F)];
			b64buf[enci + 2] = encTableB64[((tmp >> 6) & 0x3F)];
			b64buf[enci + 3] = encTableB64[(tmp & 0x3F)];
			
			#else

			b64buf[enci]     = GetEncCharB64(tmp >> 18);
			b64buf[enci + 1] = GetEncCharB64(tmp >> 12);
			b64buf[enci + 2] = GetEncCharB64(tmp >> 6);
			b64buf[enci + 3] = GetEncCharB64(tmp);

			#endif

			enci += 4;
			k = 0;
			tmp = 0;
		}
	}
		

	//Encode last bytes
	if (k != 0)
	{								
		tmp <<= (k == 1) ? 16 : 8;			

		#ifdef USE_BASE64_CODE_TABLES

		b64buf[enci]     = encTableB64[((tmp >> 18) & 0x3F)];
		b64buf[enci + 1] = encTableB64[((tmp >> 12) & 0x3F)];						
		
		b64buf[enci + 2] = (k == 1) ? 
							BASE64_SKIPCHAR :
							encTableB64[((tmp >> 6) & 0x3F)]; 			
		
		#else

		b64buf[enci]     = GetEncCharB64(tmp >> 18);
		b64buf[enci + 1] = GetEncCharB64(tmp >> 12);

		b64buf[enci + 2] = (k == 1) ? 
							BASE64_SKIPCHAR :
							GetEncCharB64(tmp >> 6);

		#endif

		b64buf[enci + 3] = BASE64_SKIPCHAR; 
			
		enci += 4;
	}

	return enci;	
}


/* */
s32 DecodeBase64(const u8 *b64buf, s32 length, u8 *buf)
{
	s32 chi = 0, enci = 0, k = 0, tmp = 0, skipCnt = 0;		
	u8 code;

	if (length < 4)
	{
		buf[0] = 0;
		return 0;
	}	

	while(enci < length)
	{
		code = b64buf[enci++];
								
		tmp <<= 6;

		if (code == BASE64_SKIPCHAR)			
			skipCnt++;		
		else
		{
			#ifdef USE_BASE64_CODE_TABLES

			if (code < 0x2B || code > 0x7A)
				break;

			tmp |= decTableB64[code - 0x2B];		
			
			#else

			if (code == 0x2B)      //'+' => 62
				tmp |= 0x3E;
			else if (code == 0x2F) //'/' => 63
				tmp |= 0x3F;
			else if (code <= 0x39) //['0'..'9'] is [0x30..0x39] => [52..61]
				tmp |= code + 0x04;//0x04 = 52 - 0x30 
			else if (code <= 0x5A) //['A'..'Z'] is [0x41..0x5A] => [0..25]
				tmp |= code - 0x41;
			else                   //['a'..'z'] is [0x61..0x7A] => [26..51]
				tmp |= code - 0x47;//-0x47 = 26 - 0x61

			#endif
		}		
			
		k++;

		if (k == 4)
		{
			buf[chi]     = (tmp >> 16) & 0xFF;
			buf[chi + 1] = (tmp >> 8) & 0xFF;
			buf[chi + 2] = tmp & 0xFF;

			chi += 3;
			k = 0;
			tmp = 0;
		}
	}

	return chi - skipCnt;	
}

/*===========================================================================
 * End of file base64.c
 *===========================================================================*/