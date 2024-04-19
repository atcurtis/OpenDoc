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
// @(#) 1.21 com/src/utils/WinUtils.cpp, odutils, od96os2, odos29646d 9/27/96 09:57:00 [ 11/15/96 15:28:57 ]
/* 
        File:           WinUtils.cpp 
 
        Contains:       Implementation of window utilities 
 
        Owned by:       Richard Rodseth 
 
        Copyright:      © 1995 by Apple Computer, Inc., all rights reserved. 
 
 
*/ 
 
#define _OD_DONT_IMPORT_CPP_ 
 
#ifndef _WINUTILS_ 
#include "WinUtils.h" 
#endif 
 
#ifndef SOM_ODDraft_xh 
#include "Draft.xh" 
#endif 
 
#ifndef SOM_ODFrame_xh 
#include "Frame.xh" 
#endif 
 
#ifdef _PLATFORM_MACINTOSH_ 
#ifndef _PASCLSTR_ 
#include "PasclStr.h" 
#endif 
#endif // _PLATFORM_MACINTOSH_ 
 
#ifndef _ODEXCEPT_ 
#include <ODExcept.h> 
#endif 
 
#ifndef _ODDEBUG_ 
#include "ODDebug.h" 
#endif 
 
#ifdef _PLATFORM_MACINTOSH_ 
#ifndef _ODMEMORY_ 
#include <ODMemory.h> 
#endif 
#endif // _PLATFORM_MACINTOSH_ 
 
#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODStorageUnit_xh 
#include <StorageU.xh> 
#endif 
 
#ifndef SOM_ODStorageUnitView_xh 
#include <SUView.xh> 
#endif 
 
#ifndef _STDTYPIO_ 
#include "StdTypIO.h" 
#endif 
 
#ifndef _TEMPOBJ_ 
#include <TempObj.h> 
#endif 
 
#ifndef _STORUTIL_ 
#include <StorUtil.h> 
#endif 
 
#ifndef SOM_Module_OpenDoc_StdTypes_defined 
#include <StdTypes.xh> 
#endif 
 
#ifndef SOM_Module_OpenDoc_StdProps_defined 
#include <StdProps.xh> 
#endif 
 
#ifndef _ODMATH_ 
#include "ODMath.h" 
#endif 
 
 
 
 
WIN32_DLLEXPORT ODBoolean BeginGetWindowProperties(Environment* ev, ODFrame* frame, WindowProperties* properties) 
{ 
        ASSERT(frame != kODNULL, kODErrInvalidFrame); 
 
        ODBoolean propsExist = kODFalse; 
 
        ODStorageUnit* frameSU = frame->GetStorageUnit(ev); 
        ODStorageUnit* windowPropsSU = kODNULL; 
 
        ODID id = ODGetStrongSURefProp(ev, frameSU, kODPropWindowProperties, kODStrongStorageUnitRef); 
 
        if (id != 0) 
        { 
                try { 
                TempODStorageUnit windowPropsSU = frameSU->GetDraft(ev)->AcquireStorageUnit(ev,id); 
                if (windowPropsSU) 
                { 
                        propsExist = kODTrue; 
 
                        ODRect odRect; 
                        ODGetRectProp(ev, windowPropsSU, kODPropWindowRect, kODRect, &odRect); 
 
                        // Scale the window rectangle from 72dpi to our resolution 
                        ODSession *session = frameSU->GetSession( ev ); 
 
                        ODFixed xscale, yscale; 
                        Get72DPIToScreenScale( session, &xscale, &yscale ); 
 
                        odRect.left = ODFixedMultiply( xscale, odRect.left ); 
                        odRect.right = ODFixedMultiply( xscale, odRect.right ); 
                        odRect.top = ODFixedMultiply( yscale, odRect.top ); 
                        odRect.bottom = ODFixedMultiply( yscale, odRect.bottom ); 
 
#ifdef _PLATFORM_WIN32_ 
                        SetRect(&properties->boundsRect, 
                             FixedToInt(odRect.left), 
                             FixedToInt(odRect.top), 
                             FixedToInt(odRect.right) - FixedToInt(odRect.left), 
                             FixedToInt(odRect.bottom) - FixedToInt(odRect.top)); 
#elif _PLATFORM_OS2_ 
                        // Get screen height in pixels 
                        LONG screenHeight; 
                        HPS hps = WinGetScreenPS( HWND_DESKTOP ); 
                        HDC hdc = GpiQueryDevice( hps ); 
                        DevQueryCaps(hdc, CAPS_HEIGHT, 1, &screenHeight); 
                        WinReleasePS( hps ); 
 
                        // Determine window height 
                        ODFixed height = odRect.bottom - odRect.top; 
 
                        // Flip window rectangle 
                        odRect.top = IntToFixed(screenHeight) - odRect.top; 
                        odRect.bottom = odRect.top - height; 
 
                        properties->boundsRect.xLeft = FixedToInt(odRect.left); 
                        properties->boundsRect.xRight = FixedToInt(odRect.right) - FixedToInt(odRect.left); 
                        properties->boundsRect.yTop = FixedToInt(odRect.top) - FixedToInt(odRect.bottom); 
                        properties->boundsRect.yBottom = FixedToInt(odRect.bottom); 
#elif _PLATFORM_AIX_ 
                        properties->boundsRect.x = FixedToInt(odRect.left); 
                        properties->boundsRect.width = 
                                FixedToInt(odRect.right) - FixedToInt(odRect.left); 
                        properties->boundsRect.y = FixedToInt(odRect.top); 
                        properties->boundsRect.height = 
                                FixedToInt(odRect.bottom) - FixedToInt(odRect.top); 
#else 
                        SetRect(&properties->boundsRect, 
                             FixedToInt(odRect.left), 
                             FixedToInt(odRect.top), 
                             FixedToInt(odRect.right), 
                             FixedToInt(odRect.bottom)); 
#endif // _PLATFORM_WIN32_ 
 
 
                        properties->title[0] = 0; 
#ifdef _PLATFORM_MACINTOSH_ 
                        ODIText* iText = ODGetITextProp(ev, windowPropsSU, kODPropWindowTitle, kODMacIText, kODNULL); 
                        if (iText) 
                                IntlToPStr(iText, &(properties->title[0])); 
                                                                        //  No error checking is done for 
                                                                        //  strings longer than 255 chars. 
                        DisposeIText(iText); 
#endif // _PLATFORM_MACINTOSH_ 
 
#ifdef _PLATFORM_MACINTOSH_ 
                        properties->procID = ODGetSShortProp(ev, 
                                windowPropsSU, kODPropWindowProcID, kODSShort); 
#endif 
 
                        properties->wasVisible = ODGetBooleanProp(ev, 
                                windowPropsSU, kODPropWindowIsVisible, kODBoolean); 
#ifdef _PLATFORM_MACINTOSH_ 
                        properties->hasCloseBox = ODGetBooleanProp(ev, 
                                windowPropsSU, kODPropWindowHasCloseBox, kODBoolean); 
#endif 
                        properties->shouldShowLinks = ODGetBooleanProp(ev, 
                                windowPropsSU, kODPropShouldShowLinks, kODBoolean); 
 
#ifdef _PLATFORM_MACINTOSH_ 
                        properties->refCon = ODGetSLongProp(ev, 
                                windowPropsSU, kODPropWindowRefCon, kODSLong); 
#elif defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
    properties->createFlags = ODGetULongProp(ev, windowPropsSU, kODPropWindowCreateFlags, kODULong); 
    properties->swpFlags = ODGetULongProp(ev, windowPropsSU, kODPropWindowSwpFlags, kODULong); 
#endif 
                        properties->isResizable = ODGetBooleanProp(ev, 
                                windowPropsSU, kODPropWindowIsResizable, kODBoolean); 
                        properties->isFloating = ODGetBooleanProp(ev, 
                                windowPropsSU, kODPropWindowIsFloating, kODBoolean); 
                        properties->isRootWindow = ODGetBooleanProp(ev, 
                                windowPropsSU, kODPropWindowIsRootWindow, kODBoolean); 
 
                        properties->sourceFrame = kODNULL; 
                        ODID id = ODGetStrongSURefProp(ev, 
                                windowPropsSU, kODPropSourceFrame, kODStrongStorageUnitRef); 
                        if (id != 0) 
                        { 
                                properties->sourceFrame = windowPropsSU->GetDraft(ev)->AcquireFrame(ev,id); 
                        } 
                } 
                } catch (ODException _exception) { 
                        WARNMSG(WARN_INDEX(AMSG_450),"Frame has kODPropWindowProperties, but unable to retrieve Identifier for strong Storage Unit Reference from the Frame."); 
                } 
        } 
        return propsExist; 
} 
 
 
 
 
WIN32_DLLEXPORT void EndGetWindowProperties(Environment* ev, WindowProperties* properties) 
{ 
        if (properties && (properties->sourceFrame != kODNULL)) 
                (properties->sourceFrame)->Release(ev); 
} 
 
 
 
 
//---------------------------------------------------------------------------- 
// ;Get72DPIToScreenScale 
// 
// Description: Gets the 72 DPI to screen scale from the session 
//---------------------------------------------------------------------------- 
WIN32_DLLEXPORT void Get72DPIToScreenScale( ODSession *session, ODFixed *xScale, ODFixed *yScale ) 
{ 
 
#ifdef _PLATFORM_WIN32_ 
 
   LONG lArray[2]; 
 
   // Get the window's device context 
   HWND hDesktop = GetDesktopWindow( ); 
   HDC hdc = GetDC( hDesktop ); 
 
   // Get the device's horizontal and vertical resolution in pixels/meter 
   lArray[0] = GetDeviceCaps( hdc, HORZRES ) * 1000 / GetDeviceCaps( hdc, HORZSIZE ); 
   lArray[1] = GetDeviceCaps( hdc, VERTRES ) * 1000 / GetDeviceCaps( hdc, VERTSIZE ); 
 
   ReleaseDC( hDesktop, hdc ); 
 
#endif  // _PLATFORM_WIN32_ 
 
 
#ifdef _PLATFORM_OS2_ 
 
   LONG lArray[2]; 
 
   // Get the window's presentation space and its device context 
   HPS hps = WinGetScreenPS( HWND_DESKTOP ); 
   HDC hdc = GpiQueryDevice( hps ); 
 
   // Get the device's horizontal and vertical resolution in pels per meter 
   DevQueryCaps( hdc, CAPS_HORIZONTAL_RESOLUTION, 2, lArray ); 
 
   // Release the window's presentation space 
   WinReleasePS( hps ); 
 
#endif  // _PLATFORM_OS2_ 
 
 
#ifdef _PLATFORM_AIX_ 
 
   unsigned long lArray[2]; 
 
   Environment* ev = somGetGlobalEnvironment(); 
 
   // Get screen from windowstate 
   Screen *screen = session->GetWindowState( ev )->GetScreen( ev ); 
 
   // Get the screen's horizontal and vertical resolution in 
   // pels per meter 
   lArray[0] = WidthOfScreen( screen )  * 1000 / WidthMMOfScreen( screen ); 
   lArray[1] = HeightOfScreen( screen ) * 1000 / HeightMMOfScreen( screen ); 
 
#endif  // _PLATFORM_AIX_ 
 
 
   // Convert the pels per meter to 72 dots per inch.  The results 
   // must be stored in fixed format as the fractional part 
   // is very significant. 
   *xScale = ODFloatToFixed( 0.0254       /* meter/inch */ 
                             * lArray[0]  /* pel/meter */ 
                             / 72.0 ); 
   *yScale = ODFloatToFixed( 0.0254       /* meter/inch */ 
                             * lArray[1]  /* pel/meter */ 
                             / 72.0 ); 
 
} 
 
 
 
 
 
//---------------------------------------------------------------------------- 
// ;GetScreenTo72DPIScale 
// 
// Description: Gets the screen to 72 DPI scale from the session 
//---------------------------------------------------------------------------- 
WIN32_DLLEXPORT void GetScreenTo72DPIScale( ODSession *session, ODFixed *xScale, ODFixed *yScale ) 
{ 
 
#ifdef _PLATFORM_WIN32_ 
 
   LONG lArray[2]; 
 
   // Get the window's device context 
   HWND hDesktop = GetDesktopWindow( ); 
   HDC hdc = GetDC( hDesktop ); 
 
   // Get the device's horizontal and vertical resolution in pixels/meter 
   lArray[0] = GetDeviceCaps( hdc, HORZRES ) * 1000 / GetDeviceCaps( hdc, HORZSIZE ); 
   lArray[1] = GetDeviceCaps( hdc, VERTRES ) * 1000 / GetDeviceCaps( hdc, VERTSIZE ); 
 
   ReleaseDC( hDesktop, hdc ); 
 
#endif  // _PLATFORM_WIN32_ 
 
 
#ifdef _PLATFORM_OS2_ 
 
   LONG lArray[2]; 
 
   // Get the window's presentation space and its device context 
   HPS hps = WinGetScreenPS( HWND_DESKTOP ); 
   HDC hdc = GpiQueryDevice( hps ); 
 
   // Get the device's horizontal and vertical resolution in pels per meter 
   DevQueryCaps( hdc, CAPS_HORIZONTAL_RESOLUTION, 2, lArray ); 
 
   // Release the window's presentation space 
   WinReleasePS( hps ); 
 
#endif  // _PLATFORM_OS2_ 
 
 
#ifdef _PLATFORM_AIX_ 
 
   unsigned long lArray[2]; 
 
   Environment* ev = somGetGlobalEnvironment(); 
 
   // Get screen from windowstate 
   Screen *screen = session->GetWindowState( ev )->GetScreen( ev ); 
 
   // Get the screen's horizontal and vertical resolution in 
   // pels per meter 
   lArray[0] = WidthOfScreen( screen )  * 1000 / WidthMMOfScreen( screen ); 
   lArray[1] = HeightOfScreen( screen ) * 1000 / HeightMMOfScreen( screen ); 
 
#endif  // _PLATFORM_AIX_ 
 
 
   // Convert the pels per meter to 72 dots per inch.  The results 
   // must be stored in fixed format as the fractional part 
   // is very significant.  Divide 72.0 by the pels per meter times 
   // meter per inch. 
   *xScale = ODFloatToFixed( 72.0 / ( 0.0254 * lArray[0] ) ); 
   *yScale = ODFloatToFixed( 72.0 / ( 0.0254 * lArray[1] ) ); 
 
 
} 
 
 
 
 
