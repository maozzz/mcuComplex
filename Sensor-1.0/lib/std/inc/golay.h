/*===========================================================================
 *
 *  Math library
 *
 *  Copyright (C) 2000 - 2001
 *  Brain Charles,  "chbrain@dircon.co.uk",
 *  Toivanen Ilkka, "pile@aimo.kareltek.fi"
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *---------------------------------------------------------------------------
 *
 *  File golay.h: Golay encoder/decoder.
 *
 *===========================================================================*/

#ifndef __GOLAY_H
#define __GOLAY_H

#include "../../bastypes.h"

/* */
u32 EncodeGolay(u32 data);

/* */
u32 DecodeGolay(u32 code, s32 *errors);

#endif /* !__GOLAY_H */

/*===========================================================================
 * End of file golay.h
 *===========================================================================*/
