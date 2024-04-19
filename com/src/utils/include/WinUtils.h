//====START_GENERATED_PROLOG====================================== 
// 
// 
//   COMPONENT_NAME: odutils 
// 
//   CLASSES: none 
// 
//   ORIGINS: 82,27 
// 
// 
//   (C) COPYRIGHT International Business Machines Corp. 1995,1996 
//   All Rights Reserved 
//   Licensed Materials - Property of IBM 
//   US Government Users Restricted Rights - Use, duplication or 
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp. 
//   	 
//   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
//   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
//   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
//   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
//   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
//   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
//   OR PERFORMANCE OF THIS SOFTWARE. 
// 
//====END_GENERATED_PROLOG======================================== 
// 
// @(#) 1.9 com/src/utils/include/WinUtils.h, odutils, od96os2, odos29646d 9/12/96 15:15:23 [ 11/15/96 15:29:04 ] 
/* 
	File:		WinUtils.h 
 
	Contains:	Utility functions for getting persistent window properties from a frame 
 
	Owned by:	Richard Rodseth 
 
	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved. 
 
*/ 
 
#ifndef _WINUTILS_ 
#define _WINUTILS_ 
 
 
#ifndef SOM_ODSession_xh 
#include <ODSessn.xh> 
#endif 
 
#ifndef _ODTYPES_ 
#include <ODTypes.h> 
#endif 
 
#ifndef _ODMVCLNK_ 
#include <ODMvcLnk.h> 
#endif 
 
 
 
//============================================================================== 
// Theory of operation 
//============================================================================== 
 
/* 
 
These functions allow a part editor to obtain the properties necessary to create 
a window, from the storage annotation on a root frame. BeginGetWindowProperties 
returns kODTrue if the annotation exists, and fills in the "properties" struct. 
EndGetWindProperties releases the sourceFrame field of the struct. 
 
Usage: 
 
	WindowProperties props; 
	ODWindow* window = kODNULL; 
	 
	if (BeginGetWindowProperties(ev, frame, &props)) 
	{ 
 
		ODPlatformWindow platformWindow = NewCWindow(kODNULL, &(props.boundsRect), props.title, 
					kODFalse, props.procID, (WindowPtr)-1L, props.hasCloseBox, props.refCon); 
		 
		window =  fSession->GetWindowState(ev)->RegisterWindowForFrame(ev, platformWindow, 
														frame, 
														 props.isRootWindow,	// Keeps draft open 
														 kODTrue,	// Is resizable 
														 kODFalse,	// Is floating 
														 kODTrue,	// shouldSave 
														 props.sourceFrame); 
		EndGetWindowProperties(ev, &props); // Release source frame 
	} 
 
*/ 
 
//============================================================================== 
// Classes used by this interface 
//============================================================================== 
 
struct	ODFrame; 
 
//============================================================================== 
// Function prototypes 
//============================================================================== 
 
struct WindowProperties 
{ 
#ifdef _PLATFORM_OS2_ 
        RECTL boundsRect; 
#else 
	Rect boundsRect; 
#endif // _PLATFORM_OS2_ 
	Str255 title; 
#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_) 
        ODULong   createFlags; 
        ODULong   swpFlags; 
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_ 
	ODSShort  procID; 
	ODBoolean hasCloseBox; 
	ODSLong   refCon; 
	ODBoolean wasVisible; 
	ODBoolean isResizable; 
	ODBoolean isFloating; 
	ODBoolean isRootWindow; 
	ODBoolean shouldShowLinks; 
	ODFrame*  sourceFrame; 
}; 
 
#ifdef __cplusplus 
extern "C" { 
#endif 
 
_DLLIMPORTEXPORT_ ODBoolean BeginGetWindowProperties(Environment* ev, ODFrame* frame, WindowProperties* properties); 
 
        // Obtains window properties from the storage unit referred to in the frame's 
        // kODPropWindowProperties. 
 
_DLLIMPORTEXPORT_ void EndGetWindowProperties(Environment* ev, WindowProperties* properties); 
 
       // Releases the source frame 
 
 
_DLLIMPORTEXPORT_ void Get72DPIToScreenScale( ODSession *session, ODFixed *xScale, ODFixed *yScale ); 
 
       // Returns the scale factors from OpenDoc 72DPI space to device space. 
 
 
_DLLIMPORTEXPORT_ void GetScreenTo72DPIScale( ODSession *session, ODFixed *xScale, ODFixed *yScale ); 
 
       // Returns the scale factors from device space to OpenDoc 72DPI space. 
 
 
#ifdef __cplusplus 
} /* extern "C" */ 
#endif 
 
 
#endif // _WINUTILS_ 
