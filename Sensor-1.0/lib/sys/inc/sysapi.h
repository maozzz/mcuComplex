/*===========================================================================
 *
 *  Simple Operation System (SimOS)
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
 *  File sysapi.h: SimOS API.
 *
 *===========================================================================*/

#ifndef __SYSAPI_H
#define __SYSAPI_H

#include "../../bastypes.h"
#include <CPU.h>
#include <filesys.h>
#include <network.h>
#include <souser.h>

#ifndef BASE_RAM_N2
#error "MCU does not have extended RAM"
#endif /* !BASE_RAM_N2 */

//Check at "Simos-apps/src/app-1986BE1x.ld":
//ORIGIN(CODE) >= SYSAPI_BASEADDR + SYSAPI_MAXSIZE
#define SYSAPI_MAXSIZE		512
#define SYSAPI_BASEADDR		BASE_RAM_N2

#define APP_RAMSIZE			4096
#define APP_MAXSIZE			(SIZE_RAM_N2 - SYSAPI_MAXSIZE - APP_RAMSIZE)

#define APP_MINADDR			(SYSAPI_BASEADDR + SYSAPI_MAXSIZE)
#define APP_MAXADDR			(BASE_RAM_N2 + SIZE_RAM_N2)


/* */
typedef struct tag_PerformCounters
{
	u32 timCounter;
	u32 timSmpSpeed;
	u32 adcSpeed;
	u32 mainSpeed;
} PerformCounters;


/* */
typedef struct tag_SysAPI
{	
	void       (*ThreadSleepUs)(u32 usec);
	void       (*ThreadSleepMs)(u32 msec);
	void       (*GetPerformCounters)(PerformCounters *sp);
	int        (*GetDiskList)(HDiskConfig *disk[]);
	DateTime*  (*GetCurrentDateTime)(DateTime *dt);
	DateTime*  (*SetCurrentDateTime)(DateTime *dt);
	SimosUser* (*GetUser)(void);
	u64        (*StrToU64)(const char *src, u32 isHex);
	s32        (*StrToArray)(u8 *dest, const char *src);
	int        (*DoSystemEvents)();

	NetIfcConfig *(*GetNetIfcConfig)(s32 index);
	ARPEntry     *(*GetARPEntry)(s32 index);
	RouteEntry   *(*GetRouteEntry)(s32 index);

	int       (*atoi)(const char *ptr);
	double    (*atof)(const char *ptr);
	//size_t    (*strlen)(const char *s);
	//int       (*strcmp)(const char *s1, const char *s2);
	//char*     (*strcpy)(char *dest, const char *src);
	//char*     (*strncpy)(char *dest, const char *src, size_t __len);
	//void*     (*memset)(void *dest, int ch, size_t len);
	//void*     (*memcpy)(void *dest, const void *src, size_t len);
	int		  (*geterrno)(void);
	char*     (*strerror)(int errnum);

	int       (*printf)(const char *format, ...);
	int       (*sprintf)(char *dest, const char *format, ...);

	char*     (*getwd)(char *buffer);
	int       (*chdir)(const char *filename);

	FSDIR*    (*opendir)(const char *dirname, FSDIR *dirstream);
	int       (*closedir)(FSDIR *dirstream);
	void	  (*seekdir)(FSDIR *dirstream, foff_t pos);
	foff_t    (*telldir)(FSDIR *dirstream);
	struct fsdirent* (*readdir)(FSDIR *dirstream, struct fsdirent *entry);

	FSFILE*   (*fopen)(const char *filename, const char *modes, FSFILE *stream);
	int       (*fclose)(FSFILE *stream);
	int       (*fseek)(FSFILE *stream, foff_t offset, int whence);
	foff_t    (*ftell)(FSFILE *stream);
	size_t    (*fread)(void *ptr, size_t sz, size_t n, FSFILE *stream);
	size_t    (*fwrite)(const void *ptr, size_t sz, size_t n, FSFILE *stream);	
	int       (*fprintf)(FSFILE *stream, const char *format, ...);
	int       (*fflush)(FSFILE *stream);
} SysAPI;



#include <globaldef.h>

#if (defined(SOFT_MODE) && SOFT_MODE == SOFT_MODE_TESTAPP)
extern SysAPI* SAPI;

#else
#define SAPI					((SysAPI*)SYSAPI_BASEADDR)
#endif

#define GetStrError()			SAPI->strerror(SAPI->geterrno())
#define SysPrintf(...)			SAPI->printf(__VA_ARGS__)
#define SysSprintf(...)			SAPI->sprintf(__VA_ARGS__)

u32 get_stdin(void);
u32 get_stdout(void);
u32 get_stderr(void);

#define hstdin		((FSFILE*)get_stdin())
#define hstdout		((FSFILE*)get_stdout())
#define hstderr		((FSFILE*)get_stderr())

#endif /* !__SYSAPI_H */

/*===========================================================================
 * End of file sysapi.h
 *===========================================================================*/
