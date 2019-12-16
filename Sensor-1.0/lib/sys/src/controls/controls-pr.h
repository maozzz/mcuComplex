/*===========================================================================
 *
 *  User interface library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File controls-pr.h: Private definitions.
 *
 *===========================================================================*/

#ifndef __CONTROLS_PR_H
#define __CONTROLS_PR_H

#include "../../../bastypes.h"
#include "../../inc/controls/Dialog.h"

#ifndef __TEXTBOX_BUFFER_UNALIGNED_ACCESS_ENABLED
//0 - when buffer memory has 16-bit cells: can't use unaligned access
#define __TEXTBOX_BUFFER_UNALIGNED_ACCESS_ENABLED       0
#endif /* __TEXTBOX_BUFFER_UNALIGNED_ACCESS_ENABLED */


/* Indicates that taskbar is visible */
#define IsTaskBarVisible() (guiGlobalState.taskBar.control.opts & CTRL_OPTS_VISIBLE)

/* Get control width restricted with parent size */
__STATIC_INLINE s16 GetRestrictedWidth(CtrlParams *ctrl)
{
    if (ctrl->parent && (ctrl->x + ctrl->width > ctrl->parent->width))
        return (ctrl->parent->width - ctrl->x);
    else
        return (ctrl->width);
}



/* Get control height restricted with parent size */
__STATIC_INLINE s16 GetRestrictedHeight(CtrlParams *ctrl)
{
    if (ctrl->parent && (ctrl->y + ctrl->height > ctrl->parent->height))
        return (ctrl->parent->height - ctrl->y);
    else
        return (ctrl->height);
}

#endif /* __CONTROLS_PR_H */

/*===========================================================================
 * End of file controls-pr.h
 *===========================================================================*/
