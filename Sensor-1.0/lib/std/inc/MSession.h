/*===========================================================================
 *
 *  Audio library
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
 *  File MSession.h: Media session definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __MSESSION_H
#define __MSESSION_H

#include "RTP.h"
#include "SIP.h"
#include "IPv4.h"

#define MSES_OPTS_RECVEN         0x01
#define MSES_OPTS_TRANEN         0x02
#define MSES_OPTS_DEFAULT        (MSES_OPTS_RECVEN | MSES_OPTS_TRANEN)

#define IsFreeSession(s)        (s->remEp.addr == IPADDR_ANY)
#define IsTranEnable(s)         (s->opts & MSES_OPTS_TRANEN)
#define IsRecvEnable(s)         (s->opts & MSES_OPTS_RECVEN)

#define GetPTMapping(pt, ptmap) (((pt) < RTP_PT_DYNSTART || !ptmap) ? \
                                 (pt) : ptmap[(pt) - RTP_PT_DYNSTART])

/* */
typedef struct tag_MSession
{                
        //Local end point
        NetEndPoint locEp;

        //Remote end point
        NetEndPoint remEp;

		//Elapsed time, s
		u32 elTime;

		u8 rtpMapOut[RTP_PT_DYNCOUNT];
        //u8 rtpMapIn[RTP_PT_DYNCOUNT];

		//Input codec (see RTP_PT_x)
		u8 codecIn;

        //Output codec (see RTP_PT_x)
        u8 codec;

        //Frame duration, ms
        u8 frameDuration;

        //Session opts (see SES_OPTS_x)
        u8 opts;
} MSession;

#endif /* __MSESSION_H */

/*===========================================================================
 * End of file MSession.h
 *===========================================================================*/

