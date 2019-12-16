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
 *  File stdcf.c: Analogs for standard C functions.
 *
 *===========================================================================*/

#include "../../inc/stdcf.h"
#include "../../../bastypes.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* */
static cpcstr_t argTypeSpecifiers    = "sciudpxXofFeEgGaA\0";
static cpcstr_t argDoubleSpecifiers  = "fFeEgGaA\0";
static cpcstr_t argTypeModifiers     = "lhLjt\0";
static cpcstr_t argHexCharsLC        = "0123456789abcdef\0";
static cpcstr_t argHexCharsUC        = "0123456789ABCDEF\0";

#define STR_ARG_IS_INF				"fni\0"
#define STR_ARG_IS_NAN				"nan\0"
#define STR_ARG_IS_NULL				"(null)\0"
#define CHAR_ARGUMENT				'%'
#define CHAR_UNDEFPADDING			'?'

#define STR_MAX_LENGTH				(512 * 1024)
#define STR_TEMP_BUFFER_SIZE		64

#define ARG_INTFRAC_WIDTH			64
#define ARG_DEFAULT_PRECISION		6

//Argument format flags
#define ARGFL_FLAGS					0x01
#define ARGFL_PRECISION				0x02
#define ARGFL_MODIFIER				0x04
#define ARGFL_RIGHTPADDING			0x08
#define ARGFL_ADDSIGN				0x10
#define ARGFL_ALTERNATE				0x20
#define ARGFL_NEGATIVE				0x40

/* Standard IEEE754:
	Normalized/denormalized numbers:
	F = (-1)^S * 2^(E-2^(b-1)+1) * (1 + M/(2^n)),
	F = (-1)^S * 2^(E-2^(b-1)+2) * M/(2^n),
	where: b - exponent bit width, n - mantissa bit width

	Double-precision 64-bit (double):
	[63]      sign (S): 1/0 -> -/+
	[62..52]  exponent (E) shifted by +1023
	[51.. 0]  mantissa (M)

	Normailzed/denormailzed numbers:
	F = (-1)^S * 2^(E-1023) * (1 + M/(2^52))
	F = (-1)^S * 2^(E-1022) * M/(2^52)

	0xFFFFFFFF FFFFFFFF -
	...................  | -nan
	0xFFF00000 00000001 -
	0xFFF00000 00000000    -inf
	0xFFEFFFFF FFFFFFFF    -1,797693134862315708e+308 -
	...................                                | normalized
	0x80100000 00000000    -2,225073858507201383e-308 -
	0x800FFFFF FFFFFFFF    -2,225073858507200889e-308 -
	...................                                | denormalized
	0x80000000 00000001    -4,940656458412465441e-324 -
	0x80000000 00000000    -0
	-----------------------------------------------------------------
	0x00000000 00000000    +0
	0x00000000 00000001    +4,940656458412465441e-324 -
	...................                                | denormalized
	0x000FFFFF FFFFFFFF    +2,225073858507200889e-308 -
	0x00100000 00000000    +2,225073858507201383e-308 -
	...................                                | normalized
	0x7FEFFFFF FFFFFFFF    +1,797693134862315708e+308 -
	0x7FF00000 00000000    +inf
	0x7FF00000 00000001 -
	...................  | +nan
	0x7FFFFFFF FFFFFFFF -


	Single-precision 32-bit (float):
	[31]      sign (S): 1/0 -> -/+
	[30..23]  exponent (E) shifted by +127
	[22.. 0]  mantissa (M)

	Normailzed/denormailzed numbers:
	F = (-1)^S * 2^(E-127) * (1 + M/(2^23))
	F = (-1)^S * 2^(E-126) * M/(2^23)

	0xFFFFFFFF -
	..........  |   -nan
	0xFF800001 -
	0xFF800000      -inf
	0xFF7FFFFF      -3,40282347e+38 -
	..........                       | normalized
	0x80800000      -1,17549435e-38 -
	0x807FFFFF      -1,17549421e-38 -
	..........                       | denormalized
	0x80000001      -1,40129846e-45 -
	0x80000000      -0
	-----------------------------------------------
	0x00000000      +0
	0x00000001      +1,40129846e-45 -
	..........                       | denormalized
	0x007FFFFF      +1,17549421e-38 -
	0x00800000      +1,17549435e-38 -
	..........                       | normalized
	0x7F7FFFFF      +3,40282347e+38 -
	0x7F800000      +inf
	0x7F800001 -
	..........  |   +nan
	0x7FFFFFFF -                                     */
//Double-precision definitions
#define INF_DOUBLE			0x7FF0000000000000ULL
#define S_DOUBLE_MASK		0x8000000000000000ULL
#define E_DOUBLE_MASK		0x7FF0000000000000ULL
#define M_DOUBLE_MASK		0x000FFFFFFFFFFFFFULL

#define E_DOUBLE_WIDTH		11
#define M_DOUBLE_WIDTH		52
#define E_DOUBLE_OFFSET		-1023

#define is_double_zero(dw)	(((dw) & (~S_DOUBLE_MASK)) == 0x0000000000000000ULL)
#define is_double_inf(dw)	(((dw) & (~S_DOUBLE_MASK)) == INF_DOUBLE)
#define is_double_nan(dw)	(((dw) & (~S_DOUBLE_MASK)) > INF_DOUBLE)

#define FRAC_DECDIG_COUNT			12
#define FRAC_HEXDIG_COUNT			13
#define FRAC_NUMBERS_COUNT			40
#define FRAC_MAX_VALUE				999999999999ULL

static const u64 tabFrac64[FRAC_NUMBERS_COUNT]=
{
	500000000000ULL,
	250000000000ULL,
	125000000000ULL,
	 62500000000ULL,
	 31250000000ULL,
	 15625000000ULL,
	  7812500000ULL,
	  3906250000ULL,
	  1953125000ULL,
	   976562500ULL,
	   488281250ULL,
	   244140625ULL,
	   122070313ULL,
		61035156ULL,
		30517578ULL,
		15258789ULL,
		 7629395ULL,
		 3814697ULL,
		 1907349ULL,
		  953674ULL,
		  476837ULL,
		  238419ULL,
		  119209ULL,
		   59605ULL,
		   29802ULL,
		   14901ULL,
			7451ULL,
			3725ULL,
			1863ULL,
			 931ULL,
			 466ULL,
			 233ULL,
			 116ULL,
			  58ULL,
			  29ULL,
			  15ULL,
			   7ULL,
			   4ULL,
			   2ULL,
			   1ULL
};

/* */
//#define wrdestbuf(c)			*pDest++ = (char)(c)

/* */
#define add_padding(argStrLength)	\
	{ int i = argWidth - (int)argStrLength;\
	  while(i-- > 0) { writeChar(pc, chPadding); }}

/* */
#define add_left_padding(argStrLength)	if (!(fmtFlags & ARGFL_RIGHTPADDING)) add_padding(argStrLength)

/* */
#define add_right_padding(argStrLength)	if (fmtFlags & ARGFL_RIGHTPADDING)  add_padding(argStrLength)

/* */
#define is_digit(c)				((c) >= 0x30 && (c) <= 0x39)



/* */
static int is_argtype(char c)
{
	int i = 0;
	char cs;
	while((cs = argTypeSpecifiers[i++]) != 0 && cs != c);
	return (int)cs;
}



/* */
static int is_argdouble(char c)
{
	int i = 0;
	char cs;
	while((cs = argDoubleSpecifiers[i++]) != 0 && cs != c);
	return (int)cs;
}



/* */
static int is_modifier(char c)
{
	int i = 0;
	char cs;
	while((cs = argTypeModifiers[i++]) != 0 && cs != c);
	return (int)cs;
}



/* */
static int int_tostring(char *dest, const u64 *value)
{
	u64 tmp = *value;
	int i = 0;
	do
	{
		dest[i++] = (char)((tmp % 10ULL) + 0x30);
		tmp = (u64)(tmp / 10ULL);
	} while(tmp && i < STR_TEMP_BUFFER_SIZE);

	return i;
}



/* */
static int frac_tostring(char *dest, int digCount, const u64 *value)
{
	u64 tmp = *value;
	int i = 0;
	while(digCount--)
	{
		i++;
		dest[FRAC_DECDIG_COUNT - i] = (char)((tmp % 10ULL) + 0x30);
		tmp = (u64)(tmp / 10ULL);
	}

	return i;
}



/* */
int subst_vsprintf0(LpWriteCharRoutine writeChar, PrintContext *pc, const char *format, va_list argv)
{
	int fmtIndex0, fmtIndex, fmtFlags;
	int strIndex1, strIndex2, argLength;
	int chModifierIndex, argWidth, argPrec;
	char chFormat, chPadding, chSign;
	char chModifier[4];
	char strValue1[STR_TEMP_BUFFER_SIZE];
	char strValue2[STR_TEMP_BUFFER_SIZE];

	//Set indexes at format and destination strings top zero	
	fmtIndex  = 0;

	//Format string is null: write '\0', return 0 (zero length)
	if (format == 0)
	{		
		writeChar(pc, 0);
		return 0;
	}	

	while((chFormat = format[fmtIndex++]) != 0)
	{
		//None format char copy to destination buffer
		if (chFormat != CHAR_ARGUMENT)
		{
			writeChar(pc, chFormat);
			continue;
		}

		//Format started
		//%[#-+0]*[1-9]*[.{1-9}+]*[hh|hljt]*{uicsxXdlf}
		chPadding      = CHAR_UNDEFPADDING;
		chModifier[0]  = 0;

		strValue1[0]   = 0;
		strValue2[0]   = 0;

		chModifierIndex = 0;
		strIndex1       = 0;
		strIndex2       = 0;

		fmtIndex0      = fmtIndex;
		fmtFlags	   = ARGFL_FLAGS;

		argPrec        = 0;

		//Parse format
		while((chFormat = format[fmtIndex]) != 0
			&& chFormat != CHAR_ARGUMENT
			&& chFormat != ' ')
		{
			fmtIndex++;
			if (is_argtype(chFormat))
				break;			
			else if (is_modifier(chFormat))
			{
				fmtFlags &= ~(ARGFL_FLAGS|ARGFL_PRECISION);
				fmtFlags |= ARGFL_MODIFIER;
				if (chModifierIndex < 2)
					chModifier[chModifierIndex++] = chFormat;
			}
			else if (fmtFlags & ARGFL_MODIFIER)
			{
				//no actions
				continue;
			}
			else if (chFormat == '.')
			{
				fmtFlags &= ~ARGFL_FLAGS;
				fmtFlags |= ARGFL_PRECISION;
			}
			else if (is_digit(chFormat))
			{
				if ((fmtFlags & ARGFL_FLAGS) && chFormat == '0')
				{
					if (chPadding == CHAR_UNDEFPADDING)
						chPadding = '0';
				}
				else
				{
					fmtFlags &= ~ARGFL_FLAGS;

					if (fmtFlags & ARGFL_PRECISION)
						strValue2[strIndex2++] = chFormat;
					else
						strValue1[strIndex1++] = chFormat;
				}
			}
			else if (fmtFlags & ARGFL_FLAGS)
			{
				switch(chFormat)
				{
					case '#': fmtFlags |= ARGFL_ALTERNATE; break;

					case '+': fmtFlags |= ARGFL_ADDSIGN;   break;

					case '-':
					{
						fmtFlags |= ARGFL_RIGHTPADDING;
						chPadding = ' ';
						break;
					}				
				}
			}
		} //while format


		if (chFormat == 0)
			break;		
		else if (chFormat == CHAR_ARGUMENT || chFormat == ' ')
		{
			int cpCount = fmtIndex - fmtIndex0;
			const char *ps;

			if (cpCount > 0)
			{
				//Unknown format: copy
				writeChar(pc, '%');
				ps = &format[fmtIndex0];
				while(cpCount--)
					writeChar(pc, *ps++);
			}
			else
			{
				if (chFormat == CHAR_ARGUMENT)
				{
					writeChar(pc, chFormat);
				}

				fmtIndex++;
			}

			continue;
		}

		chModifier[chModifierIndex] = 0;

		strValue1[strIndex1] = 0;
		strValue2[strIndex2] = 0;

		argWidth = (strValue1[0] != 0) ?
					atoi(strValue1): 0;

		if (chPadding == CHAR_UNDEFPADDING)
			chPadding = ' ';

		//Replace format
		if (chFormat == 'p')
		{
			//Convert pointer to hexadecimal format: 0xnnnnnnnn
			if (sizeof(void*) == 8)
			{
				chModifier[0] = 'l';
				chModifier[1] = 'l';
				chModifierIndex = 2;
			}

			chFormat = 'x';
			fmtFlags |= ARGFL_ALTERNATE;
		}	


		//char buffer or single char
		if (chFormat == 's' ||  chFormat ==  'c')
		{
			const char *argStr;
			char argChar = 0;

			if (chFormat == 's')
			{
				argStr = (char*)va_arg(argv, char*);

				//Default value whether pointer to string is null
				if (!argStr)
					argStr = STR_ARG_IS_NULL;

				//Count of visible chars
				argPrec = ((fmtFlags & ARGFL_PRECISION) && (strValue2[0] != 0)) ?
							atoi(strValue2): STR_MAX_LENGTH;

				argLength = 0;
				while(argStr[argLength] != 0 && argPrec--)
					argLength++;
			}
			else
			{
				argChar = (char)va_arg(argv, int);
				chPadding = ' ';
				argLength = 1;
			}

			add_left_padding(argLength);

			if (chFormat == 's')
			{
				//Copy chars value to output
				argPrec = argLength;
				while(argPrec--)
					writeChar(pc, *argStr++);
			}
			else
			{
				writeChar(pc, argChar);
			}

			add_right_padding(argLength);
		} //if 's' || 'c'
		//number: decimal, hexadecimal, octal, double
		else
		{
			s64 argInt = 0LL;
			int expValue;
			u8 isUnsigned = 0;
			u8 isLongLong = 0;
			u8 isConvertible = 0;
			u8 isDouble;
			u8 isExpForm;
			u8 isgGForm;

			argLength = 0;

			isDouble = (u8)is_argdouble(chFormat);

			if (isDouble)
			{
				//Double argument
				isgGForm  = (chFormat == 'g' || chFormat == 'G') ? 1 : 0;
				isExpForm = (chFormat == 'e' || chFormat == 'E' || isgGForm) ? 1 : 0;
			}
			else
			{
				//Integer argument
				isgGForm  = 0;
				isExpForm = 0;

				isUnsigned = (chFormat == 'i' || chFormat == 'd') ? 0 : 1;

				isLongLong = (chModifierIndex > 1 &&
							  chModifier[0] == 'l' &&	chModifier[1] == 'l') ? 1 : 0;

				if (isLongLong)
				{
					argInt = (s64)((long long int)va_arg(argv, long long int));
				}
				else if (chModifierIndex > 0 && chModifier[0] == 'l')
				{
					argInt = (s64)((long int)va_arg(argv, long int));

					if (sizeof(long int) == 8)
						isLongLong = 1;
				}
				else
				{
					argInt = (s64)((int)va_arg(argv, int));
				}

				if (chModifierIndex > 1 &&
					chModifier[0] == 'h' &&	chModifier[1] == 'h')
				{
					argInt = isUnsigned ?
							(s64)((unsigned char)argInt):
							(s64)((char)argInt);
				}
				else if (chModifierIndex > 0 && chModifier[0] == 'h')
				{
					argInt = isUnsigned ?
							(s64)((unsigned short)argInt):
							(s64)((short)argInt);
				}
			}

			//Decimal integers
			if (chFormat == 'i' || chFormat == 'u' || chFormat == 'd')
			{
				u64 argUInt;

				fmtFlags &= ~ARGFL_ALTERNATE;

				//Decimal value
				//Check sign
				if (chFormat != 'u' && argInt < 0)
				{
					fmtFlags |= ARGFL_NEGATIVE;
					argInt = -argInt;
					argWidth--;
				}
				else if (fmtFlags & ARGFL_ADDSIGN)
					argWidth--;

				argUInt = (isLongLong) ? (u64)argInt : (u64)((u32)argInt);

				//Convert integer value to string
				strIndex1 = int_tostring(strValue1, &argUInt);

				argLength = strIndex1;
			} //'iud'
			//Octal/hexadecimal
			else if (chFormat == 'x' || chFormat == 'X' || chFormat == 'o')
			{
				const char *pHexChars;
				int digCount;
				u32 bitShift, digMask, chi;
				u64 argUInt;

				if (chFormat == 'o')
				{
					bitShift = 3;
					digMask  = 0x07;
					digCount = isLongLong ? 22 : 11;
				}
				else
				{
					bitShift = 4;
					digMask  = 0x0F;
					digCount = isLongLong ? 16 : 8;
				}

				//Select lower/upper case letters
				pHexChars = (chFormat == 'x') ? argHexCharsLC : argHexCharsUC;

				//Reset flags
				fmtFlags &= ~(ARGFL_ADDSIGN|ARGFL_NEGATIVE);

				argUInt = (u64)argInt;

				//Convert integer value to octal/hexadecimal string
				strIndex1 = 0;
				do
				{
					chi = ((u32)argUInt) & digMask;
					strValue1[strIndex1++] = pHexChars[chi];
					argUInt >>= bitShift;
				} while(argUInt != ((u64)0) && strIndex1 < digCount);

				//Add padding and '0'/'0x' prefix
				if (fmtFlags & ARGFL_ALTERNATE)
				{
					//pad zeros between '0'/'0x' and octal/hex digits
					if (chPadding == '0')
					{
						int i = argWidth - strIndex1;

						i = (chFormat == 'o') ? (i - 1) : (i - 2);

						while(i-- > 0)
							strValue1[strIndex1++] = '0';

						chPadding = ' ';
					}

					//Add prefix: '0'/'0x'
					if (chFormat != 'o')
						strValue1[strIndex1++] = chFormat; //'x'/'X'

					strValue1[strIndex1++] = '0';
				}

				//Save string length
				argLength = strIndex1;
			} //'xXo'
			//Double-precision  argument (double)
			else if (isDouble)
			{
				union { double fl; u64 dw; } valDouble;
				int i, shift, E, sign;
				const char *pnstr;
				const char *pHexChars;
				u64 addOne;
				u64 argIntPart;
				u64 argFracPartBits;
				u64 argFracPart;

				argIntPart    = 0ULL;
				isConvertible = 0;
				addOne        = 1ULL;
				pnstr         = "\0";

				if (fmtFlags & ARGFL_PRECISION)
				{
					argPrec = (strValue2[0] == 0) ? 0 : atoi(strValue2);

					if (isgGForm && argPrec == 0)
							argPrec = 1;					

					if (!argPrec)
						fmtFlags &= ~ARGFL_PRECISION;
				}
				else
				{
					if (chFormat == 'a' || chFormat == 'A')
						argPrec = FRAC_HEXDIG_COUNT;
					else
					{
						argPrec = ARG_DEFAULT_PRECISION;
						fmtFlags |= ARGFL_PRECISION;
					}
				}

				//Get double value
				valDouble.fl = (double)va_arg(argv, double);
				sign = ((int)(valDouble.dw >> 63)) & 1;

				if (is_double_nan(valDouble.dw))
					pnstr = STR_ARG_IS_NAN;
				else if (is_double_inf(valDouble.dw))
					pnstr = STR_ARG_IS_INF;
				else
				{
					isConvertible = 1;

					//Get exponent value
					E = (int)((valDouble.dw >> M_DOUBLE_WIDTH) & 0x7FF);

					if (E == 0)
					{
						//Denormalized number
						E = (E_DOUBLE_OFFSET + 1);
						addOne = 0ULL;
					}
					else
					{
						//Normalized number
						E = E + E_DOUBLE_OFFSET;
					}

					//Get mantissa value
					argFracPartBits = (u64)(valDouble.dw & M_DOUBLE_MASK);

					if (chFormat == 'a' || chFormat == 'A')
					{
						//Double at hexadecimal format.
						//Sample: 0X1.B807A26A11858P+6
						u64 tmp;
						int mCharCounter;
						char eSign;

						if (argFracPartBits == 0ULL)
						{
							E = 0;
							if (!(fmtFlags & ARGFL_PRECISION))
								argPrec = 0;
						}

						if (E < 0)
						{
							tmp = (u64)(-E);
							eSign = '-';
						}
						else
						{
							tmp = (u64)E;
							eSign = '+';
						}

						pHexChars = (chFormat == 'a') ? argHexCharsLC : argHexCharsUC;

						//Exponent value
						strIndex1 = int_tostring(strValue1, &tmp);

						//Exponent sign
						strValue1[strIndex1++] = eSign;
						strValue1[strIndex1++] = (chFormat == 'a') ? 'p' : 'P';
						mCharCounter = 0;

						if (argPrec < FRAC_HEXDIG_COUNT)
						{
							int bitShift = 4 * (FRAC_HEXDIG_COUNT - argPrec);

							//Round
							if (((argFracPartBits >> (bitShift - 4)) & 0x0FULL) > 0x07ULL)
							{
								if (argPrec)
									argFracPartBits += (1ULL << bitShift);
								else
									addOne += 1ULL;
							}

							argFracPartBits >>= bitShift;
						}
						else
						{
							//Add trailing zeros
							int k = argPrec - FRAC_HEXDIG_COUNT;
							while (k--)
							{
								strValue1[strIndex1++] = '0';
							}
						}

							//Convert mantissa to hexadecimal string
						while(mCharCounter < FRAC_HEXDIG_COUNT &&
							  mCharCounter < argPrec)
						{
							strValue1[strIndex1++] = pHexChars[(int)(argFracPartBits & 0x0FULL)];
							argFracPartBits >>= 4;
							mCharCounter++;
						}

						//'0x1.' or '0x0.'
						if (argPrec || (fmtFlags & ARGFL_ALTERNATE))
							strValue1[strIndex1++] = '.';

						strValue1[strIndex1++] = 0x30 + (int)addOne;
						strValue1[strIndex1++] = (chFormat == 'a') ? 'x' : 'X';
						strValue1[strIndex1++] = '0';

						argLength = strIndex1;

						//End convert double to hexadecimal string
						goto label_enddouble;
					}

					//Shift to high bits
					argFracPartBits <<= (ARG_INTFRAC_WIDTH - M_DOUBLE_WIDTH);

					if (E == 0)
						argIntPart = addOne;
					else if (E < 0)
					{
						E = -E;
						argIntPart = 0;
						argFracPartBits >>= E;
						argFracPartBits |= (addOne << (ARG_INTFRAC_WIDTH - 1)) >> (E - 1);
					}
					else
					{
						shift = (ARG_INTFRAC_WIDTH - 1) - E;

						argIntPart = (addOne << (ARG_INTFRAC_WIDTH - 1)) | (argFracPartBits >> 1);

						if (shift < 0)
						{
							pnstr = STR_ARG_IS_INF;
							isConvertible = 0;
						}
						else
							argIntPart >>= shift;

						argFracPartBits <<= E;
					}

					//is convertible value
				}

				//Check sign. Sign added for "inf" and "nan"
				if (sign)
				{
					fmtFlags |= ARGFL_NEGATIVE;
					argWidth--;
				}
				else if (fmtFlags & ARGFL_ADDSIGN)
					argWidth--;

				//Convert to string
				if (isConvertible)
				{
					int argPrecE = argPrec;

					if (isExpForm && argIntPart != 0ULL)
					{
						//Select rounded digit index for integer
						u64 tmpInt = argIntPart;
						while(tmpInt)
						{
							argPrecE--;
							tmpInt = (u64)(tmpInt / 10ULL);
						}

						argPrecE++;
					}

					//Convert fraction part to string
					if (argFracPartBits != 0ULL)
					{
						i = 0;
						//Convert fraction to decimal 64-bit integer number
						argFracPart = 0;
						while(i < FRAC_NUMBERS_COUNT)
						{
							if (argFracPartBits & (1ULL << 63))
								argFracPart += tabFrac64[i];

							argFracPartBits <<= 1;
							i++;
						}

						//Fraction to string
						strIndex2 = frac_tostring(strValue2, FRAC_DECDIG_COUNT, &argFracPart);

						if (isExpForm && argIntPart == 0ULL)
						{
							//Select rounded digit index for fraction
							int k = 0;

							argPrecE++;
							while(k < strIndex2 && strValue2[k++] == '0')
								argPrecE++;
						}

						//Round fraction
						if (argPrecE >= 0 && argPrecE < FRAC_DECDIG_COUNT &&
							strValue2[argPrecE] >= '5')
						{
							u64 oneRound = 1ULL;
							int k = FRAC_DECDIG_COUNT - argPrecE;

							while(k--)
								oneRound *= 10ULL;

							argFracPart += oneRound;
							if (argFracPart > FRAC_MAX_VALUE)
							{
								argFracPart = 0ULL;
								argIntPart = argIntPart + 1ULL;
							}

							//Convert rounded fraction to string
							strIndex2 = frac_tostring(strValue2, FRAC_DECDIG_COUNT, &argFracPart);
						}
						//argFracPartBits != 0
					}
					else
					{
						//argFracPartBits == 0: fill string with '0' chars
						strIndex2 = 0;
						while(strIndex2 < FRAC_DECDIG_COUNT)
							strValue2[strIndex2++] = '0';
					}

					//Convert integer part to string (must performed after fraction)
					strIndex1 = int_tostring(strValue1, &argIntPart);

					//Round integer
					if (argPrecE < 0 &&	strValue1[-argPrecE - 1] >= '5')
					{
						u64 oneRound = 1ULL;
						argPrecE = -argPrecE;

						while(argPrecE--)
							oneRound *= 10ULL;

						//Convert rounded integer to string
						argIntPart += oneRound;
						strIndex1 = int_tostring(strValue1, &argIntPart);
					}

					if (isExpForm)
					{
						//Reverser chars for integer part
						char *p1 = &strValue1[0];
						char *p2 = &strValue1[strIndex1 - 1];
						char tmp;
						while(p1 < p2)
						{
							tmp = *p1;
							*p1 = *p2;
							*p2 = tmp;
							p1++;
							p2--;
						}

						//1,e,+,nn = 5 chars
						argLength = 5 + argPrec;
					}
					else
					{
						//Full float length [int]'.'[frac]
						argLength = strIndex1 + argPrec;
					}

					//ARGFL_ALTERNATE: always add '.'
					if (argPrec || (fmtFlags & ARGFL_ALTERNATE))
						argLength++;

					if (isgGForm)
					{
						int k;

						//Remove trailing zeros (fraction)
						while(strIndex2 > 0 && strValue2[strIndex2 - 1] == '0')
								strIndex2--;

						if (strIndex2 == 0)
						{
							//Remove trailing zeros (integer)
							while(strIndex1 > 0 && strValue1[strIndex1 - 1] == '0')
									strIndex1--;
						}

						k = 0;
						expValue = strIndex1 - 1;

						//Solve decimal exponent value
						if (strIndex1 == 1 && strValue1[0] == '0')
						{
							expValue--;
							while(k < strIndex2 && strValue2[k] == '0')
							{
								expValue--;
								k++;
							}
						}

						//Dform
						if (!(expValue < -4 || expValue >= argPrec))
							argLength = strIndex2 + strIndex1 + 1;
						else //Eform .,e,+,nn = 4 chars
							argLength = strIndex2 + strIndex1 + 5;
					}//'gG'
				} //isConvertible (double number)
				else
				{
					//Not a double number (nan, inf)
					strcpy(strValue1, pnstr);
					strIndex1 = strlen(pnstr);
					strIndex2 = 0;
					argPrec   = 0;
					argLength = strIndex1;
					fmtFlags &= ~(ARGFL_ALTERNATE|ARGFL_PRECISION);
				}
			} //'fFeEaAgG'
			else
			{
				//Unknown format, continue
				continue;
			}

label_enddouble:

			//Add argument value at string representation
			//Add signs '-/+'
			if (fmtFlags & ARGFL_NEGATIVE)
				chSign = '-';
			else if (fmtFlags & ARGFL_ADDSIGN)
				chSign = '+';
			else
				chSign = 0;

			if (chSign && chPadding == '0')
			{
				writeChar(pc, chSign);
				chSign = 0;
			}

			add_left_padding(argLength);

			if (chSign)
			{
				writeChar(pc, chSign);
				chSign = 0;
			}

			if (isConvertible && isExpForm)
			{
				//Print at exponencial form d.dddde{+-}nn
				int k;
				char dChar;
				char eChar;
				char *p1;

				expValue = strIndex1 - 1;
				k = 0;

				//Solve decimal exponent value
				if (strIndex1 == 1 && strValue1[0] == '0')
				{
					expValue--;

					while(k < strIndex2 && strValue2[k] == '0')
					{
						expValue--;
						k++;
					}

					if (k < strIndex2)
						dChar = strValue2[k];
					else
					{
						dChar = '0';
						expValue = 0;
					}

					k++;					
				}
				else
					dChar = strValue1[0];				

				if (isgGForm &&	expValue >= -4 && expValue < argPrec)
				{
					//Decimal form
					//Integer part
					k = 0;
					while(k < strIndex1)
					{
						writeChar(pc, strValue1[k++]);
					}

					//Point+Fraction part
					if (strIndex2 > 0 || (fmtFlags & ARGFL_ALTERNATE))
					{
						writeChar(pc, '.');

						k = 0;
						while(k < strIndex2 && k < FRAC_DECDIG_COUNT)
						{
							writeChar(pc, strValue2[k++]);
						}
					}
				}//decimal form
				else
				{
					//Exponencial form
					//One digit before point
					strIndex1--;
					writeChar(pc, dChar);

					//Point
					if (argPrec > 0 || (fmtFlags & ARGFL_ALTERNATE))
						writeChar(pc, '.');

					//Other digits
					p1 = &strValue1[1];
					while(strIndex1-- > 0 && argPrec-- > 0)
					{
						writeChar(pc, *p1++);
					}

					strIndex2 = k;
					while(argPrec-- > 0)
					{
						eChar = (strIndex2 < FRAC_DECDIG_COUNT) ?
									strValue2[strIndex2++] : '0';

						writeChar(pc, eChar);
					}

					//Convert exponent value to string
					//Select char: 'e','E'
					chFormat = (chFormat == 'g' || chFormat == 'e') ?
							'e' : 'E';

					//'-'/'+'
					if (expValue < 0)
					{
						expValue = -expValue;
						eChar = '-';
					}
					else
						eChar = '+';

					writeChar(pc, chFormat);
					writeChar(pc, eChar);

					if (expValue < 10)
					{
						writeChar(pc, '0');
					}

					strIndex2 = 0;
					do
					{
						strValue2[strIndex2++] = (char)((expValue % 10) + 0x30);
						expValue = (int)(expValue / 10);						
					} while(expValue);

					while(strIndex2--)
					{
						writeChar(pc, strValue2[strIndex2]);
					}
				}//exponencial form
			} //'eEgG'
			else
			{
				//Print at decimal form: "integer.fraction"
				//Now string value chars at reverse order, reverse it
				//Integer part (iudxXaA)
				while(strIndex1--)
				{
					writeChar(pc, strValue1[strIndex1]);
				}

				//Point+Fraction part ('fF')
				if ((chFormat == 'f' || chFormat == 'F') &&
					(fmtFlags & (ARGFL_ALTERNATE|ARGFL_PRECISION)))
				{
					writeChar(pc, '.');

					strIndex2 = 0;
					while(argPrec--)
					{
						chFormat = (strIndex2 < FRAC_DECDIG_COUNT) ?
									strValue2[strIndex2++] : '0';

						writeChar(pc, chFormat);
					}
				}
			} //iudxXaA

			add_right_padding(argLength);
		} //is number (i.e. except 's', 'c')
	} //while

	//Write terminate '\0'
	writeChar(pc, 0);

	return 0;
}



/* Put char to string buffer (sprintf, vsprintf functions) */
static void buffer_printchar(PrintContext *pc, char c)
{
	*pc->dest++ = c;
}



/* Stub function. No actions */
static void stub_printchar(PrintContext *pc, char c)
{
}



/* To "stdcf.c". Pointer to function that print chars to standard output */
LpWriteCharRoutine std_pfprintchar = &stub_printchar;



/* */
int subst_sprintf(char *dest, const char *format, ...)
{
	PrintContext pc;
	va_list argv;

	pc.dest = dest;

	if (format == 0)
	{
		buffer_printchar(&pc, 0);
		return 0;
	}	

	va_start(argv, format);

	subst_vsprintf0(&buffer_printchar, &pc, format, argv);

	va_end(argv);

	//Count of chars without '\0'
	return ((int)pc.dest - (int)dest - 1);
}



/* */
int subst_vsprintf(char *dest, const char *format, va_list argv)
{
	PrintContext pc;
	pc.dest = dest;

	if (format == 0)
	{
		buffer_printchar(&pc, 0);
		return 0;
	}

	subst_vsprintf0(&buffer_printchar, &pc, format, argv);

	//Count of chars without '\0'
	return ((int)pc.dest - (int)dest - 1);
}



/* Print to standard output */
int std_printf(const char *format, ...)
{
	va_list argv;

	if (!std_pfprintchar)
		return -1;
	else if (format == 0)
	{
		std_pfprintchar(0, 0);
		return 0;
	}

	va_start(argv, format);

	subst_vsprintf0(std_pfprintchar, 0, format, argv);

	va_end(argv);

	return 0;
}



/* */
int std_vprintf(const char *format, va_list argv)
{
	if (!std_pfprintchar)
		return -1;
	else if (format == 0)
	{
		std_pfprintchar(0, 0);
		return 0;
	}

	subst_vsprintf0(std_pfprintchar, 0, format, argv);

	return 0;
}



/* */
const char *std_strstr(const char *src, const char *find)
{
    size_t lsrc  = strlen(src);
    size_t lfind = strlen(find);
    size_t i;

    if (lfind == 0 || lsrc == 0 || lfind > lsrc)
        return 0;

    lsrc = lsrc - lfind + 1;

    //Compare string and substring
    i = 0;
    for(i = 0; i < lsrc; i++)
    {
        if (memcmp(&src[i], find, lfind) == 0)
            return &src[i];
    }

    return 0;
}

/*===========================================================================
 * End of file stdcf.c
 *===========================================================================*/

