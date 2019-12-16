/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2018
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File ParseUtils.h: Text parsing utilities definitions
 *                     and functions prototypes.
 *
 *===========================================================================*/

#ifndef __PARSE_UTILS_H
#define __PARSE_UTILS_H

#include "../../bastypes.h"
#include "consp.h"
#include "stdcf.h"
#include "StrUtils.h"

//Print message macros
#define PrintErrMsg(...)                        //ConPrintf(__VA_ARGS__)

//Lexeme buffer length
#define LEXEME_BUFFER_LENGTH                    128

//Parsing output codes
#define PARSLT_OK                                0
#define PARSLT_ERR_SYNTAX                       -1
#define PARSLT_ERR_INVARG                       -2
#define PARSLT_ERR_EMPTY_BUFFER                 -3
#define PARSLT_ERR_PARTIAL                      -4

typedef s32                                    PARSLT;

typedef struct tag_ParseContext
{
        //Pointer to buffer that contains data to parsed
        const char *buffer;

        //buffer length
        u32 length;

        //Current offset at buffer
        u32 i;

        //Previous lexeme
        char prevLexBuffer[LEXEME_BUFFER_LENGTH];

        //Current lexeme
        char lexBuffer[LEXEME_BUFFER_LENGTH];

        //Pointer to splitter array
        const char *splitter;

        //Count of elements at splitter array
        u32 splitLength;
} ParseContext;


/* Indicating that specifed char is digit: [0-9] */
#define IsDigit(_ch) (_ch >= 0x30 && _ch <= 0x39)

/* Indicating that specifed char is alpha: [A-Za-z_] */
#define IsAlpha(_ch) ((_ch >= 0x41 && _ch <= 0x5A)||(_ch >= 0x61 && _ch <= 0x6A)||_ch == 0x5F)

/* Indicating that current char contains at splitter array. */
u8 IsSplitter(char charCode, const char *splitters, u32 length);

/* Indicating that specifed string is number */
u8 IsNumber(const char *str);

/* Indicating that specifed string is word */
u8 IsWord(const char *str);

/* Compare current lexeme with specified string */
u32 IsString(ParseContext *cont, const char *str);

/* Get next lexeme */
s32 GetLexeme(ParseContext *cont);

/* Copy string by CRLF */
char *CopyByCRLF(ParseContext *cont, char *dest, s32 length);

/* Skip lexemes by CRLF. */
s32 SkipByCRLF(ParseContext *cont);

/* Skip lexemes by specified string. */
s32 SkipByString(ParseContext *cont, const char *str);

/* Skip leading spaces, tabulations and CRLF chars */
PARSLT SkipLeadSpaceCRLF(ParseContext *cont);

/* Parse header that contains integer value. */
PARSLT ParseNumericHeader(ParseContext *cont, u32 *value);


//Add empty line (CRLF) to the end of buffer
#define AddCRLF(str)            std_sprintf(strend(str), "\r\n")

//Indicating that current lexeme is CRLF
#define IsCRLF(__lex) (strlen((char*)__lex) == 2 && \
                                                __lex[0] == 0x0D && __lex[1] == 0x0A)

//
#define CHECK_STR(__str)        if (glr <= 0 || strcmp((char*)cont->lexBuffer, __str) != 0)\
                                {\
                                if (glr > 0)\
                                { PrintErrMsg("Error: expected '%s' instead '%s'\r\n",\
                                  __str, cont->lexBuffer);  }\
                                else\
                                { PrintErrMsg("Error: unexpected end of buffer\r\n"); }\
                                  return PARSLT_ERR_SYNTAX; }

//
#define CHECK_CRLF()            if (!IsCRLF(cont->lexBuffer))\
                                { PrintErrMsg("Error: expected 'CRLF' instead '%s'\r\n", \
                                  cont->lexBuffer);\
                                  return PARSLT_ERR_SYNTAX; }

//
#define CHECK_NUMBER()          if (!IsNumber(cont->lexBuffer))\
                                { PrintErrMsg("Error: expected number instead '%s'\r\n",\
                                  cont->lexBuffer);\
                                  return PARSLT_ERR_SYNTAX; }

//
#define TRY_GETLEXEMEEQ(__cont, __str)  if ((glr = GetLexeme(__cont)) <= 0 || \
                                             strcmp((char*)cont->lexBuffer, __str) != 0)\
                                         {  PrintErrMsg("Error: expected '%s' instead '%s'\r\n", \
                                             __str, cont->lexBuffer);\
                                            return PARSLT_ERR_SYNTAX;}

//
#define TRY_GETLEXEME(__cont, __errmsg)  if ((glr = GetLexeme(__cont)) <= 0)\
                                         {  PrintErrMsg("Error: expected %s\r\n", __errmsg);\
                                            return PARSLT_ERR_SYNTAX;}

#endif /* __PARSE_UTILS_H */

/*===========================================================================
 * End of file ParseUtils.h
 *===========================================================================*/

