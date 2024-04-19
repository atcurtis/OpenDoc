/* @(#)Z 1.22 com/src/layout/Border.cpp, odlayout, od96os2, odos29646d 96/11/15 15:25:17 (96/09/13 17:02:21) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odlayout
//
//   CLASSES: none
//
//   ORIGINS: 82,27,94
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
/*  IBM Change History (most recent first):                         */
/*   20470 03/19/96 tmw/SA Apple DR4 and common Windows/OS2         */
/********************************************************************/
/*
   File:       Border.cpp

   Contains:   Implementation of active border display routines

   Owned by:   Joshua Susser

   Copyright:  © 1995 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <4>     6/20/95    JBS     1257315 change Facet::AcquireWindow to
                                   GetWindow
        <3>     5/26/95    RR      #1251403: Multithreading naming support
        <2>     5/17/95    JBS     1242496 remove SOM_CATCH
        <1>     2/28/95    JBS     first checked in
        <0>     2/28/95    JBS     1198509 created
*/

#ifdef _PLATFORM_OS2_
#define INCL_WIN
#define INCL_GPI
#include <ODos2.h>
#endif //_PLATFORM_OS2_

/*
These functions are private utilities for use by ODFacet to display and
invalidate the active border.  The functions are platform-dependent, in
that they make direct calls to the platform graphics system.
*/

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _BORDER_
#include "Border.h"
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODCanvas_xh
#include <Canvas.xh>
#endif

#ifdef _PLATFORM_WIN32_
#ifndef SOM_ODWin32WindowCanvas_xh
#include "WinWCanv.xh"
#endif
#endif //_PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
#ifndef SOM_ODOS2WindowCanvas_xh
#include "OS2WCanv.xh"
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif
#endif //_PLATFORM_OS2_


#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __QUICKDRAW__
#include <QuickDraw.h>
#endif

#pragma segment ODFacet
#endif //_PLATFORM_MACINTOSH_

//==============================================================================
// Global variables
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
extern Pattern borderPattern;
#endif //_PLATFORM_MACINTOSH_

#ifdef _FACET_DEBUG_
#include <qprintf.h>
#endif

#ifdef _PLATFORM_WIN32_

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#endif // PLATFORM_WIN32_
//==============================================================================
// Functions
//==============================================================================

//------------------------------------------------------------------------------
// ODDrawBorder
//------------------------------------------------------------------------------

void
ODDrawBorder(Environment *ev, ODShape* borderShape, ODFacet* facet)
{
#ifdef _FACET_DEBUG_
  qprintf("Executing ODDrawBorder()\n");
#endif //_FACET_DEBUG_

#ifdef _PLATFORM_WIN32_
  HDC hDC;
  ODWin32WindowCanvas *platcanv;
  LONG winStyles=0;
  LONG rStyles=0;
  HWND hwnd;


  ODWindow* window = facet->GetWindow(ev);

  if ( window != kODNULL )
  {
    ODRgnHandle borderRgn  = borderShape->GetRegion(ev);

#ifdef _FACET_DEBUG_
    ODRect boundingBox;
    borderShape->GetBoundingBox(ev, &boundingBox);
    RECT RectShape;
    boundingBox.AsWinRect(RectShape);
    qprintf("??In DrawBorder()->border shape left=%d\ttop=%d\tright=%d\tbottom=%d\n", RectShape.left, RectShape.top, RectShape.right, RectShape.bottom);
    GetRgnBox(borderRgn, &RectShape);
    qprintf("??In DrawBorder()->border region left=%d\ttop=%d\tright=%d\tbottom=%d\n", RectShape.left, RectShape.top, RectShape.right, RectShape.bottom);
#endif //_FACET_DEBUG_

    ODFacet* parentFacet = facet->GetContainingFacet(ev);

    if(parentFacet)
    {
      hwnd= parentFacet->GetFacetHWND(ev);
      HWND facethwnd = facet->GetFacetHWND(ev);
      LONG fwStyles = GetWindowLong(facethwnd, GWL_STYLE);

      if(fwStyles & WS_VISIBLE)  // OLE parts hide the window
      {
       winStyles = GetWindowLong(hwnd, GWL_STYLE);
       rStyles = (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
       winStyles &= ~(rStyles);
       SetWindowLong(hwnd, GWL_STYLE, winStyles);
      }

      platcanv = (ODWin32WindowCanvas *) parentFacet->GetCanvas(ev)->GetPlatformCanvas(ev, kODWin32);
      hDC = platcanv->GetDC(ev);

    //Hairy Hatch Border
    WORD    wHatchBmp[] = {0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88};

    HBITMAP hbm = CreateBitmap(8, 8, 1, 1, wHatchBmp);
    HBRUSH  hairBrush = CreatePatternBrush(hbm);
    HBRUSH  hbrOld = SelectObject(hDC, hairBrush);

    COLORREF cvText = SetTextColor(hDC, RGB(255, 255, 255));
    COLORREF cvBk = SetBkColor(hDC, RGB(0, 0, 0));
    FillRgn(hDC, borderRgn, hairBrush);
    SetTextColor(hDC, cvText);
    SetBkColor(hDC, cvBk);
    SelectObject(hDC, hbrOld);
    DeleteObject(hairBrush);
    DeleteObject(hbm);

      platcanv->ReleaseDC(ev);
      winStyles = GetWindowLong(hwnd, GWL_STYLE);
      winStyles |= rStyles;
      SetWindowLong(hwnd, GWL_STYLE, winStyles);
    }
  }
#endif //_PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
  ODOS2WindowCanvas *platcanv;
  HWND hwndClient = NULLHANDLE;
  ODWindow* window = facet->GetWindow(ev);
  HPS hps;
  ODFacet* parentFacet = facet->GetContainingFacet(ev);


  if ( window != kODNULL )
  {
       HWND hwndFrame = window->GetPlatformWindow(ev);  //TMW don't use GetClientWindow()
       hwndClient = WinWindowFromID(hwndFrame, FID_CLIENT);
       hps = WinGetPS(hwndClient);

    //Setup PS to draw border
    LONG lSet = GpiQueryPatternSet(hps);
    GpiResetPS(hps, GRES_ATTRS);
    GpiSetPatternSet(hps,LCID_DEFAULT);
    GpiSetPattern(hps, PATSYM_DIAG2);
    GpiPaintRegion(hps, borderShape->GetRegion(ev));
    GpiSetPatternSet(hps, lSet);

/*
     ODRect fxRect;
     RECTL upRect;
     borderShape->GetBoundingBox(ev, &fxRect);
     fxRect.AsRECTL(upRect);
     WinValidateRect(hwndClient, &upRect, TRUE);
*/

     WinReleasePS(hps);
     WinValidateRegion(hwndClient,
                       borderShape->GetRegion(ev),
                       TRUE);

  }
#endif //_PLATFORM_OS2_


#ifdef _PLATFORM_MACINTOSH_
  GrafPtr savePort;

  ODWindow* window = facet->GetWindow(ev);
  if ( window != kODNULL )
  {
    GetPort(&savePort);

    SetPort((GrafPtr)(window->GetRootFacet(ev)->
            GetCanvas(ev)->GetPlatformCanvas(ev, kODQuickDraw)));
    SetOrigin(0,0);
    ClipRect(&(ODQDGlobals.thePort->portRect));

    RgnHandle borderRgn = borderShape->GetQDRegion(ev);
    FillRgn(borderRgn, &borderPattern);

    SetPort(savePort);
  }
#endif // _PLATFORM_MACINTOSH_
}

//------------------------------------------------------------------------------
// ODInvalidateBorder
//------------------------------------------------------------------------------

void
ODInvalidateBorder(Environment *ev, ODShape* borderShape, ODFacet* facet)
{
#ifdef _FACET_DEBUG_
    qprintf("Executing ODInvalidateBorder()\n");
#endif
#ifdef _PLATFORM_WIN32_
  ODWindow* window = facet->GetWindow(ev);
  if ( window != kODNULL )
  {
    ODFacet* parentFacet = facet->GetContainingFacet(ev);
    HWND hwnd;

    if(parentFacet)
    {

      TempODShape newshape = borderShape->Copy(ev);
      TempODTransform xform = parentFacet->AcquireWindowFrameTransform(ev, kODNULL);
      ODPoint pt(0, 0);
      xform->TransformPoint(ev, &pt);
      TempODTransform xform2 = parentFacet->CreateTransform(ev);
      xform2->SetOffset(ev, &pt);
      newshape->Transform(ev, xform2);
      ODRgnHandle borderRgn  = newshape->GetRegion(ev);

      hwnd = window->GetPlatformWindow(ev);
      InvalidateRgn(hwnd,
                    borderRgn,
                    FALSE);
    }
  }
#endif //_PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_

  ODWindow* window = facet->GetWindow(ev);
  if ( window != kODNULL )
  {
    HWND   hwndFrame = window->GetPlatformWindow(ev);  //TMW don't use GetClientWindow()
    HWND hwndClient = WinWindowFromID(hwndFrame, FID_CLIENT);

    WinInvalidateRegion(hwndClient, borderShape->GetRegion(ev), TRUE);

/*
     ODRect fxRect;
     RECTL upRect;
     borderShape->GetBoundingBox(ev, &fxRect);
     fxRect.AsRECTL(upRect);
       // sometimes 1 or 2 pixel width border is NOT getting erased
       upRect.xLeft -= 2;
       upRect.yBottom -= 2;
       upRect.xRight += 2;
       upRect.yTop += 2;

       WinInvalidateRect(hwndClient, &upRect, TRUE);
*/


  }

#endif //_PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_
  GrafPtr savePort;

  ODWindow* window = facet->GetWindow(ev);
  if ( window != kODNULL )
  {
    GetPort(&savePort);

    SetPort((GrafPtr)(window->GetRootFacet(ev)->
            GetCanvas(ev)->GetPlatformCanvas(ev, kODQuickDraw)));
    SetOrigin(0,0);
    ClipRect(&(ODQDGlobals.thePort->portRect));

    RgnHandle borderRgn = borderShape->GetQDRegion(ev);
    InvalRgn(borderRgn);

    SetPort(savePort);
  }
#endif //_PLATFORM_MACINTOSH_
}

