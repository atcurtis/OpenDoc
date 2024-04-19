/* @(#)Z 1.7 com/src/ui/DspUtilM.h, odui, od96os2, odos29646d 96/11/15 15:29:30 (96/07/15 18:26:23) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES: none
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */
/*
	File:           DspUtilM.h

	Contains:       Dispatcher utilities.

	Owned by:       Richard Rodseth

	Copyright:      © 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>     8/29/95        RR              #1267251 Utilites for synthsized activate
									events
		 <3>     5/25/95        jpa             Fixed usage of ODDebug. [1253321]
		 <2>     5/10/95        RR              # 1239139 Added LogEvent, and
									GetActiveFacetWithBorderUnderPoint

	To Do:
	In Progress:
*/

#ifndef _DSPUTILM_
#define _DSPUTILM_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

const kODGeneratedActivate = 0x0002;

class ODFacet;
class ODFrame;
class ODWindow;

void WindowToLocal(Environment* ev, ODPoint& point, ODFacet* facet);

ODFacet* GetActiveFacetWithBorderUnderPoint(Environment* ev, ODFrame* frame, ODWindow* window, ODPoint& windowPoint);

inline long Min(long a, long b)
{
	return a < b ? a : b;
}

#ifdef _PLATFORM_MACINTOSH_
inline void SetEvent(ODEventData& event, short what)
{
	event.what = what;
	event.message = 0;
	event.when = 0;
	event.where.h = 0; event.where.v = 0;
	event.modifiers = 0;
}

inline void SetEvent(ODEventData& event, short what, Point globalWhere)
{
	event.what = what;
	event.message = 0;
	event.when = 0;
	event.where.h = globalWhere.h; event.where.v = globalWhere.v;
	event.modifiers = 0;
}
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_OS2_
inline void SetEvent(ODEventData& event, HWND hwnd, ULONG msg, MPARAM mp1 = 0, MPARAM mp2 = 0)
{
  event.hwnd = hwnd;
  event.msg = msg;
  event.mp1 = mp1;
  event.mp2 = mp2;
  event.result = 0;
}
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
inline void SetEvent(ODEventData& event, unsigned long what)
{
	event.hwnd = NULL;
	event.message = what;
	event.wParam = 0;
	event.lParam = 0;
	event.time = 0;
	event.pt.x = 0; event.pt.y = 0;
}

inline void SetEvent(ODEventData& event, unsigned long what, Point globalWhere)
{
	event.hwnd = NULL;
	event.message = what;
	event.wParam = 0;
	event.lParam = 0;
	event.time = 0;
	event.pt.x = globalWhere.x; event.pt.y = globalWhere.y;
}
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_MACINTOSH_
inline void SetActivateEvent(ODEventData& event, WindowPtr window, ODBoolean activate)
{       
	event.what = activateEvt;
	event.message = (long) window;
	event.when = 0;
	event.where.h = 0; event.where.v = 0;
	if (activate)
		event.modifiers = 1 | kODGeneratedActivate; 
	else
		event.modifiers = 0 | kODGeneratedActivate; 
}

inline ODBoolean IsODActivateEvent(ODEventData& event)
{       
	return (event.modifiers & kODGeneratedActivate);
}
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_WIN32_
inline void SetActivateEvent(ODEventData& event, WindowPtr window, ODBoolean activate)
{       
	event.hwnd    = (HWND)window;
	event.message = WM_ACTIVATE;

	if (activate)
	    event.wParam = MAKEWPARAM(WA_ACTIVE,   0);
	else
	    event.wParam = MAKEWPARAM(WA_INACTIVE, 0);

	event.lParam         = NULL;

	event.time = 0;
	event.pt.x = 0; 
	event.pt.y = 0;
}

inline ODBoolean IsODActivateEvent(ODEventData& event)
{       
    return (LOWORD(event.wParam) == WA_ACTIVE);

}
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
inline void SetActivateEvent(ODEventData& event, WindowPtr window, ODBoolean activate)
{       
	event.hwnd = (HWND)window;
	event.msg  = WM_ACTIVATE;

	if (activate)
	    event.mp1 = MPFROMSHORT(TRUE);
	else
	    event.mp1 = MPFROMSHORT(FALSE);

	event.mp2     = MPFROMHWND(window);

	event.result  = MRFROMSHORT(0);
}

inline ODBoolean IsODActivateEvent(ODEventData& event)
{       
    return SHORT1FROMMP(event.mp1);
}
#endif // _PLATFORM_OS2_

#if defined(_PLATFORM_MACINTOSH) || defined(_PLATFORM_WIN32_)
inline void SetEventInfo(ODEventInfo& info, ODPoint where)
{
	info.embeddedFrame = kODNULL;
	info.embeddedFacet = kODNULL;
	info.where = where;
	info.propagated = kODFalse;
}
#endif // _PLATFORM_MACINTOSH, _PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
inline void SetEventInfo(ODEventInfo& info, ODPoint where, ODFrame* embeddedFrame = kODNULL,
			 ODFacet* embeddedFacet = kODNULL, ODULong flags = 0)
{
  info.embeddedFrame = embeddedFrame;
  info.embeddedFacet = embeddedFacet;
  info.where = where;
  info.flags = flags;
}
#endif // _PLATFORM_OS2_


#if ODDebug
void LogEvent(ODEventData* event, ODEventInfo* eventInfo, ODFrame* frame, ODFacet* facet);
#endif

#endif // _DSPUTILM_
