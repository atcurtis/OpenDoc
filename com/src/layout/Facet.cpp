/* @(#)Z 1.98 com/src/layout/Facet.cpp, odlayout, od96os2, odos29647a 96/11/18 05:19:01 (96/11/15 10:00:24) */

//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odlayout
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
/*  IBM Change History (most recent first):                            */
/*  <1> 20537 03/25/96  SA   Apple DR4 and common Windows/OS2          */
/***********************************************************************/


#define ODFacet_Class_Source

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
#define M_ODFacet_Class_Source

#ifndef _ALTPOINT_    // AltPoint.h must be included before ODTypes.h - SA
#include "AltPoint.h"
#endif

#include <stdio.h>
#endif // _PLATFORM_OS2 || _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
#include "AltPoint.h"
#endif

#ifdef _PLATFORM_OS2_
#define INCL_WIN
#define INCL_GPI
#endif  // _PLATFORM_OS2_

#ifndef SOM_ODFacet_xih
#define VARIABLE_MACROS
#include <Facet.xih>
#endif

#ifdef _PLATFORM_MACINTOSH_    // AltPoint.h must be included before ODTypes.h - SA
#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _FACETNOD_
#include "FacetNod.h"
#endif

#ifndef SOM_ODFacetIterator_xh
#include "FacetItr.xh"
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODCanvas_xh
#include <Canvas.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODDispatcher_xh
#include <Disptch.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _NODE_
#include "Node.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODMATH_
#include "ODMath.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _FOCUSLIB_
#include "FocusLib.h"
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _BIAS_
#include "Bias.h"
#endif

#ifndef _BORDER_
#include "Border.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef som_h
#include "som.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __PRINTING__
#include <Printing.h>
#endif
#endif   // _PLATFORM_MACINTOSH_

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifdef _PLATFORM_UNIX_
#include <Xm/XmAll.h>
#include <X11/extensions/shape.h>
#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>

#ifndef SOM_ODPlatformCanvas_xh
#include "PlatCanv.xh"
#endif
#ifndef SOM_ODAIXWindowCanvas_xh
#include "AIXWCanv.xh"
#endif
#ifndef SOM_ODAIXStandardCanvas_xh
#include "AIXSCanv.xh"
#endif
#endif // _PLATFORM_UNIX_

#ifdef _PLATFORM_WIN32_

#ifndef SOM_ODPlatformCanvas_xh
#include "PlatCanv.xh"
#endif
#ifndef SOM_ODWin32WindowCanvas_xh
#include "WinWCanv.xh"
#endif
#ifndef SOM_ODWin32StandardCanvas_xh
#include "WinSCanv.xh"
#endif
#include <qprintf.h>
#endif // _PLATFORM_WIN32_

#ifdef  _FACET_TIMING_
#include <qprintf.h>
extern "C"
{
unsigned int RD_TSR();
}
#endif


#ifdef _PLATFORM_OS2_
#ifndef SOM_ODPlatformCanvas_xh
#include "PlatCanv.xh"
#endif
#ifndef SOM_ODOS2WindowCanvas_xh
#include "OS2WCanv.xh"
#endif
#ifndef SOM_ODOS2StandardCanvas_xh
#include "OS2SCanv.xh"
#endif
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_ // CED
#pragma segment ODFacet
#endif // _PLATFORM_MACINTOSH_


//=====================================================================================
// Constants
//=====================================================================================

#define kNoBias kODNULL

#ifdef _PLATFORM_UNIX_
#define MOVE_WINDOW_TO_TOP ((Window)-1)
#define MOVE_WINDOW_TO_BOTTOM ((Window)-2)
#endif // UNIX

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
//==============================================================================
// Global variables
//==============================================================================
#define HWND_MOVE_THE_WINDOW ((HWND)-5)
#define szFacetWinClassName "OpenDoc:FacetWindow"
const char *pszFacetWinClass = szFacetWinClassName;

struct FacetWndData {
  ODFacet* facet;                   // Pointer to object for this window
  ODFrame* frame;                   // Pointer to this facet's frame
  ODPart*  part;                    // Pointer to this facet's part
  HWND     hwndClient;
#ifdef _PLATFORM_OS2_
  friend MRESULT EXPENTRY fnwpFacetWindow ( HWND, ULONG, MPARAM, MPARAM);
#endif

#ifdef _PLATFORM_WIN32_
  friend LRESULT CALLBACK fnwpFacetWindow (HWND, UINT, WPARAM, LPARAM);
#endif
};


struct FacetWndCtlData {
  USHORT cb;
  ODFacet* self;
};

//#ifdef _UNICODE_
//char szMsg1[Str255];
//#endif

#endif // _PLATFORM_OS2_  || _PLATFORM_WIN32_

#if (defined _PLATFORM_WIN32_  || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
inline static void GetFacetPosition(Environment* ev, ODTransform* trm,
                        ODShape* fshape, ODPoint &offset)
{
    ODTransformType tptype = trm->GetType(ev);
    ODPoint negScale;                   // negative scale
    trm->GetScale(ev, &negScale);

    if((tptype == kODLinearTranslateXform) || (tptype == kODLinearXform) ||
        (tptype == kODPerspectiveXform) || (negScale.x < 0) || (negScale.y < 0) )
    {
        trm->TransformShape(ev, fshape);
        ODRect tbound;
        fshape->GetBoundingBox(ev, &tbound);
        offset.x = -tbound.left;
#ifdef _PLATFORM_OS2_
        offset.y = -tbound.bottom;
#endif
#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
        offset.y = -tbound.top;
#endif

    }
    else
         {
            trm->GetOffset(ev, &offset);
            offset.x = -offset.x;
            offset.y = -offset.y;

         }

}

#endif
//=====================================================================================
// ODFacet
//=====================================================================================

//------------------------------------------------------------------------------
// ODFacet: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetsomUninit(ODFacet *somSelf)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","somUninit");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::somUninit()\n");
#endif
   ODSafeReleaseObject(_fFrame);
   ODSafeReleaseObject(_fClipShape);
   ODSafeReleaseObject(_fActiveShape);
   ODSafeReleaseObject(_fExternalTransform);

   if( _fNode ) {
#ifdef OD_BUG
      if(_fNode->GetParent())
#endif
         _fNode->GetParent()->RemoveChild(*_fNode);
       delete _fNode;
   }

   ODSafeReleaseObject(_fAggregateClipShape);
   ODSafeReleaseObject(_fWindowAggregateClipShape);
   ODSafeReleaseObject(_fFrameTransform);
   ODSafeReleaseObject(_fContentTransform);
   ODSafeReleaseObject(_fWindowFrameTransform);
   ODSafeReleaseObject(_fWindowContentTransform);
   ODSafeReleaseObject(_fActiveBorderShape);

   Environment* ev = somGetGlobalEnvironment();

#ifdef _PLATFORM_OS2_
   if (_fFacetHWND)
   {
     WinDestroyWindow(_fFacetHWND);
   }
   if (_fCanvas)
   {
       ODPlatformCanvas* platCanvas = _fCanvas->GetPlatformCanvas(ev, kODPM);
       ODDeleteObject(platCanvas);                  //  delete ODOS2WindowCanvas
       ODDeleteObject(_fCanvas);                    //  delete ODCanvas
   }
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_

   if(_fUsedShapeRgn != kODNULL)
      DeleteObject(_fUsedShapeRgn);

   if(somSelf->GetFacetHWND(ev)) {
      DestroyWindow(somSelf->GetFacetHWND(ev));
   }
   if (_fCanvas)
   {
       ODPlatformCanvas* platCanvas = _fCanvas->GetPlatformCanvas(ev, kODWin32);
       ODDeleteObject(platCanvas);             //  delete ODWin32WindowCanvas
       ODDeleteObject(_fCanvas);               //  delete ODCanvas
   }
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
        if(_fCanvas)
        {
         ODPlatformCanvas* platCanvas = _fCanvas->GetPlatformCanvas(ev, kODAIX);
         if (platCanvas->IsInitialized(ev))
         {
             GC gc = ((ODAIXWindowCanvas*)platCanvas)->GetGC(ev);
             XFreeGC(XtDisplay(_fFacetWidget), gc);
         }
         ODDeleteObject(platCanvas);                  //  delete ODAIXWindowCanvas
         ODDeleteObject(_fCanvas);                    //  delete ODCanvas
        }

        if(_fActiveBorderWidget)
           XtDestroyWidget(_fActiveBorderWidget);

        if(_fFacetWidget)
           XtDestroyWidget(_fFacetWidget);

#endif // _PLATFORM_UNIX_

//    ODFacet_parents_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODFacet: InitFacet
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetInitFacet(ODFacet *somSelf, Environment *ev,
       ODFrame* frame,
       ODShape* clipShape,
       ODTransform* externalTransform,
       ODCanvas* canvas,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","InitFacet");
#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::InitFacet()\n");
#endif

   SOM_TRY

       if ( frame == kODNULL ) THROW(kODErrIllegalNullFrameInput);

       /* Moved from somInit. SOM itself sets fields to zero
       _fFrame = kODNULL;
       _fClipShape = kODNULL;
       _fActiveShape = kODNULL;
       _fExternalTransform = kODNULL;
       _fCanvas = kODNULL;
       _fPartInfo = kODNULL;
       _fIsSelected = kODFalse;

       _fNode = kODNULL;
       _fNeedsUpdate = kODFalse;
       _fIsUpdating = kODFalse;

       _fAggregateClipShape = kODNULL;
       _fWindowAggregateClipShape = kODNULL;
       _fFrameTransform = kODNULL;
       _fContentTransform = kODNULL;
       _fWindowFrameTransform = kODNULL;
       _fWindowContentTransform = kODNULL;
       _fActiveBorderShape = kODNULL;
#ifdef _PLATFORM_UNIX_
        _fActiveBorderWidget=0;
        _fFacetWidget=0;
#endif // UNIX
       */
       _fHighlight = kODNoHighlight;

       somSelf->InitObject(ev);

       _fFrame = frame;
       ODAcquireObject(ev, _fFrame);

       _fClipShape = BiasShapeSet(ev, clipShape, biasCanvas);
       ODAcquireObject(ev, _fClipShape);

       _fExternalTransform = BiasTransformSet(ev, externalTransform, biasCanvas);
       ODAcquireObject(ev, _fExternalTransform);

#ifdef _PLATFORM_WIN32_
       _fUsedShapeRgn = kODNULL;  // Used Only in Windows 95 environment
#endif

       if ( canvas )
       {
           _fCanvas = canvas;
           _fCanvas->SetFacet(ev, somSelf);
       }

       _fNode = new FacetNode(somSelf);    // must be a root facet!

#ifdef _PLATFORM_UNIX_
  if(canvas) {
     ODPlatformCanvas* platCanvas = canvas->GetPlatformCanvas(ev, kODAIX);
     if (platCanvas && platCanvas->somIsA(_ODAIXWindowCanvas) && (!platCanvas->IsInitialized(ev))) {
         ODWindow* window = somSelf->GetWindow(ev);
         if(window) {
                Widget parentWidget= window->GetDrawingAreaWidget(ev);
                if(parentWidget) {
                  _fFacetWidget = somSelf->CreateFacetWidget(ev, parentWidget, platCanvas);

                }
        }
     }
  }

#endif // _PLATFORM_UNIX_

#ifdef _PLATFORM_OS2_
  if (canvas) {
     ODPlatformCanvas* platCanvas = canvas->GetPlatformCanvas(ev, kODPM);
     if (platCanvas && platCanvas->somIsA(_ODOS2WindowCanvas) && (!platCanvas->IsInitialized(ev))) {
        ODWindow* window = somSelf->GetWindow(ev);
        if (window) {
           HWND hwndParent = WinWindowFromID(window->GetPlatformWindow(ev), FID_CLIENT);
           if ( hwndParent ) {
              // Register facet window class
                  CLASSINFO clsi;
                  HAB hab = WinQueryAnchorBlock(HWND_DESKTOP);
                  if (!WinQueryClassInfo( hab, pszFacetWinClass, &clsi))
                  {
                       WinRegisterClass( hab,
                                           pszFacetWinClass,
                                           (PFNWP)fnwpFacetWindow,
                                           CS_HITTEST,   /* | CS_PARENTCLIP,*/
                                           NUM_FACETWINDOWDATAWORDS * sizeof(ULONG));
                  }

                HWND facetHWND = somSelf->CreateFacetWindow(ev, hwndParent);
                ((ODOS2WindowCanvas*)platCanvas)->InitPlatformWindowCanvas(ev, facetHWND);
           }
        }
      }
   }
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
    if (canvas) {
      ODPlatformCanvas* platCanvas = canvas->GetPlatformCanvas(ev, kODWin32);
      if (platCanvas && platCanvas->somIsA(_ODWin32WindowCanvas) && (!platCanvas->IsInitialized(ev))) {
          ODWindow* window = somSelf->GetWindow(ev);
        if (window) {
           HWND hwndParent = window->GetPlatformWindow(ev);
           if ( hwndParent ) {

               HINSTANCE hInst = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
               WNDCLASS theWndCls;
              // check whether this class is already registered?
              if(!GetClassInfo(hInst, pszFacetWinClass, &theWndCls)) {
                WNDCLASS  wc;

                wc.style                = 0;
                wc.lpfnWndProc          = fnwpFacetWindow;
                wc.cbClsExtra           = 0;
                wc.cbWndExtra           = NUM_FACETWINDOWDATAWORDS * sizeof(ULONG);
                wc.hInstance            = hInst;
                wc.hIcon                = NULL;
                wc.hCursor              = LoadCursor(NULL,IDC_ARROW);
                wc.hbrBackground        = (HBRUSH)kODNULL;
                wc.lpszMenuName         = NULL;
                wc.lpszClassName        = pszFacetWinClass;

                if (!RegisterClass(&wc)) {
                     THROW(kODErrUndefined);
                }
               }  // GetClassInfo()

                 HWND facetHWND = somSelf->CreateFacetWindow(ev, hwndParent);
                 ((ODWin32WindowCanvas*)platCanvas)->InitPlatformWindowCanvas(ev, facetHWND);
           }  // hwndParent
        }   // window
      }   // platCanvas
   }     // canvas
#endif // _PLATFORM_WIN32_
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: InitChildFacet
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetInitChildFacet(ODFacet *somSelf, Environment *ev,
       FacetNode* node,
       ODFrame* frame,
       ODShape* clipShape,
       ODTransform* externalTransform,
       ODCanvas* canvas,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","InitChildFacet");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::InitChildFacet()\n");
#endif
   SOM_TRY

       if ( node == kODNULL ) THROW(kODErrIllegalNullFacetInput);
       if ( frame == kODNULL ) THROW(kODErrIllegalNullFrameInput);

       /* Moved from somInit. SOM itself sets fields to zero
       _fFrame = kODNULL;
       _fClipShape = kODNULL;
       _fActiveShape = kODNULL;
       _fExternalTransform = kODNULL;
       _fCanvas = kODNULL;
       _fPartInfo = kODNULL;
       _fIsSelected = kODFalse;

       _fNode = kODNULL;
       _fNeedsUpdate = kODFalse;
       _fIsUpdating = kODFalse;

       _fAggregateClipShape = kODNULL;
       _fWindowAggregateClipShape = kODNULL;
       _fFrameTransform = kODNULL;
       _fContentTransform = kODNULL;
       _fWindowFrameTransform = kODNULL;
       _fWindowContentTransform = kODNULL;
       _fActiveBorderShape = kODNULL;
       */
       _fNode = node;

       _fHighlight = kODNoHighlight;
       somSelf->InitObject(ev);

       _fFrame = frame;
       ODAcquireObject(ev, _fFrame);

       _fClipShape = BiasShapeSet(ev, clipShape, biasCanvas);
       ODAcquireObject(ev, _fClipShape);

       _fExternalTransform = BiasTransformSet(ev, externalTransform, biasCanvas);
       ODAcquireObject(ev, _fExternalTransform);

       if ( canvas )
       {
           _fCanvas = canvas;
           _fCanvas->SetFacet(ev, somSelf);
       }
#ifdef _PLATFORM_WIN32_
       _fUsedShapeRgn = kODNULL;
#endif


   SOM_CATCH_ALL
   SOM_ENDTRY
}


#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
SOM_Scope ODPlatformWindow  SOMLINK ODFacetGetFacetHWND(ODFacet *somSelf,
                                                                   Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetGetFacetHWND");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetFacetHWND()\n");
#endif

#ifdef _PLATFORM_OS2_
       return _fFacetHWND;
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
    ODPlatformWindow winHWND=kODNULL;

    if(_fCanvas && _fCanvas->GetPlatformCanvas(ev, kODWin32)->somIsA(_ODWin32WindowCanvas))
    {
       ODWin32WindowCanvas* winCanvas = (ODWin32WindowCanvas*) (_fCanvas->GetPlatformCanvas(ev, kODWin32));

       if(winCanvas->IsInitialized(ev))
          winHWND = winCanvas->GetWindow(ev);
    }

    return winHWND;
#endif // _PLATFORM_WIN32_


}
#endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_

#ifdef _PLATFORM_WIN32_
//------------------------------------------------------------------------------
// ODFacet: CreateWin32StandardCanvas
//------------------------------------------------------------------------------

SOM_Scope ODWin32StandardCanvas*  SOMLINK ODFacetCreatePlatformCanvas(ODFacet *somSelf, Environment *ev,
    HDC hdc)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CreatePlatformCanvas");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreatePlatformCanvas()\n");
#endif

    ODWin32StandardCanvas *c = kODNULL;  ODVolatile(c);

    SOM_TRY

    c = new ODWin32StandardCanvas;
    THROW_IF_NULL(c);
    c->InitPlatformCanvas(ev, hdc);

    return c;
    SOM_CATCH_ALL{
      ODDeleteObject(c);
      return kODNULL;
    }
    SOM_ENDTRY
}
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
//------------------------------------------------------------------------------
// ODFacet: CreateODAIXStandardCanvas
//------------------------------------------------------------------------------

SOM_Scope ODAIXStandardCanvas*  SOMLINK ODFacetCreatePlatformCanvas(ODFacet *somSelf, Environment *ev,
    Display *display,
    Pixmap   pixmap,
    GC       gc      )
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CreatePlatformCanvas");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreatePlatformCanvas()\n");
#endif

    ODAIXStandardCanvas *c = kODNULL;  ODVolatile(c);

    SOM_TRY

    c = new ODAIXStandardCanvas;
    THROW_IF_NULL(c);
    c->InitPlatformCanvas(ev, display, pixmap, gc);

    return c;
    SOM_CATCH_ALL{
      ODDeleteObject(c);
      return kODNULL;
    }
    SOM_ENDTRY
}

SOM_Scope Widget  SOMLINK ODFacetGetFacetWidget(ODFacet *somSelf,
                                                   Environment *ev)
{

    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetGetFacetWidget");

    return _fFacetWidget;
}

SOM_Scope void  SOMLINK ODFacetSetFacetWidgetWinPos(ODFacet *somSelf,
                                                    Environment *ev,
                                                    Window childwindow,
                                                    Window siblingwindow,
                                                    ODBoolean MoveBeforeSibling)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetSetFacetWidgetWinPos");

    if(siblingwindow == MOVE_WINDOW_TO_TOP)
       XRaiseWindow(XtDisplay(_fFacetWidget), childwindow);

    if(siblingwindow == MOVE_WINDOW_TO_BOTTOM)
       XLowerWindow(XtDisplay(_fFacetWidget), childwindow);


    if((siblingwindow != MOVE_WINDOW_TO_TOP) &&
        (siblingwindow != MOVE_WINDOW_TO_BOTTOM) && siblingwindow)
    {
                ODFacet* parentFacet=somSelf->GetContainingFacet(ev);
                if(parentFacet)
                {
                  Widget parentwidget = parentFacet->GetFacetWidget(ev);
                  Window root, parent;
                  Window *children;
                  unsigned int num_children;
                  Status rc;

                  rc = XQueryTree(XtDisplay(parentwidget),
                                  XtWindow(parentwidget),
                                  &root,
                                  &parent,
                                  &children,
                                  &num_children);
                  if(num_children > 1)
                  {

                    Window *windows = new Window[num_children];
                    Window *tpwindows = new Window[num_children-1];
                    int index = num_children-1;
                    int i=0;

                    for(; index >= 0 ; index--)
                    {
                        if(children[index] != childwindow)
                           tpwindows[i++] = children[index];
                    }

                    int j=0;
                    index = num_children - 1;
                    i = 0;

                    for(; index > 0 ; index--)
                    {
                     if(siblingwindow == tpwindows[j])
                     {
                        if(MoveBeforeSibling)
                        {
                           windows[i++] = childwindow;
                           windows[i++] = tpwindows[j++];
                        }
                        else {  // Move behind the sibling window
                              windows[i++] = tpwindows[j++];
                              windows[i++] = childwindow;
                        }

                     }
                     else
                         windows[i++] = tpwindows[j++];

                    } // for loop

                    XRestackWindows(XtDisplay(parentwidget),
                                  windows,
                                  num_children);
                    XFree(windows);
                    XFree(tpwindows);

                  }  // num_children > 1

                  XFree(children);

                } // if parent facet exists


    }  // if child window should be inserted in the existing windows

}


SOM_Scope void  SOMLINK ODFacetSetFacetWidgetSize(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetSetFacetWidgetSize");

    Dimension offsetx, offsety;
    Dimension lWidth, lHeight;
    lWidth =0; lHeight = 0;


         TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
         ODRect RectBounds;

         TempODShape bShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
         bShape->Transform(ev, xform);
         bShape->GetBoundingBox(ev, &RectBounds);

         Point childPt2 = {FixedToInt(RectBounds.left), FixedToInt(RectBounds.top)};
         lWidth  = FixedToInt(RectBounds.right) - childPt2.x;
         lHeight = FixedToInt(RectBounds.bottom) - childPt2.y;

         ODFacet* parentFacet = somSelf->GetContainingFacet(ev);
         if(parentFacet)
         {
           TempODTransform x2form = parentFacet->AcquireWindowFrameTransform(ev, kODNULL);

           ODPoint ParentPt1(0,0);
           TempODShape FrameShape = ODCopyAndRelease(ev,  parentFacet->GetFrame(ev)->AcquireFrameShape(ev, kODNULL));
           GetFacetPosition(ev, x2form, FrameShape, ParentPt1);

           Point ParentPt2 = ParentPt1.AsXPoint();

           childPt2.x += ParentPt2.x;
           childPt2.y += ParentPt2.y;

           Dimension parentWidth, parentHeight;
           XtVaGetValues(parentFacet->GetFacetWidget(ev),
                          XmNwidth, &parentWidth,
                          XmNheight, &parentHeight,
                          NULL);

           if(parentWidth < (childPt2.x + lWidth))
                lWidth = parentWidth - childPt2.x;

           if(parentHeight < (childPt2.y + lHeight))
                lHeight = parentHeight - childPt2.y;
         }

         // Move and position the window

           XtVaSetValues( _fFacetWidget,
                          XmNwidth, lWidth,
                          XmNheight, lHeight,
                          XmNx, childPt2.x,
                          XmNy, childPt2.y,
                          XmNmarginWidth, 0,
                          XmNmarginHeight, 0,
                          XmNresizePolicy, XmRESIZE_NONE,
                          NULL);

         if(_fClipShape)
         {
           // If the window has a region
           if (_fIsRegionSet)
           {
             // Current design is once a facet window region, always a facet window region.
             // A possible enhancement would be to delete the facet window
             // region if the facet's clipshape and frame's usedshape become the same as the
             // rectangular frameshape. An example is a toggle between a round analog
             // and a rectangular digital clock.
             somSelf->SetFacetWidgetRegion(ev, kODFalse);
           }
           else
           {
             // If the transformed clipshape is not rectangular or is different than the
             // transform frame shape
             TempODShape clipShape = _fClipShape->Copy(ev);
             clipShape->Transform(ev, xform);
             if( (!clipShape->IsRectangular(ev)) || (!clipShape->IsSameAs(ev, bShape)) )
             {
               // We've gone down the road where this facet window will now always have a
               // region associated with it.
               somSelf->SetFacetWidgetRegion(ev, kODFalse);
             }
           }
         }
}


SOM_Scope void  SOMLINK ODFacetSetFacetWidgetRegion(ODFacet *somSelf,
                                                    Environment *ev,
                                                    ODBoolean IsUsedShape)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetSetFacetWidgetRegion");

    TempODShape UShape = ODCopyAndRelease(ev, somSelf->GetFrame(ev)->AcquireUsedShape(ev, kNoBias));

    if(!IsUsedShape)
    {
       TempODShape clipShape = ODCopyAndRelease(ev, somSelf->AcquireClipShape(ev, kNoBias));
       UShape->Intersect(ev, clipShape);
    }

      if(UShape)
      {
       somSelf->Invalidate(ev, kODNULL, kODNULL);
       TempODTransform xform =  somSelf->AcquireWindowFrameTransform(ev, kODNULL);

       UShape->Transform(ev, xform);
       ODPoint offset(0,0);
       TempODShape FrameShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
       GetFacetPosition(ev, xform, FrameShape, offset);

       Point Pt2 = offset.AsXPoint();


       //On NT & OS2 -> ODRgnHandle UShapeRgn = UShape->GetRegion(ev);
       // get the shape's XRegion and modify XWindow to match with it
       ODPlatformShape UsedShapeRegion =  UShape->GetPlatformShape(ev, kODAIX);

       XOffsetRegion((Region)UsedShapeRegion, Pt2.x, Pt2.y);

       XShapeCombineRegion(XtDisplay(_fFacetWidget),
                           XtWindow(_fFacetWidget),
                           ShapeBounding,
                           0, 0,
                           (Region)UsedShapeRegion,
                           ShapeSet);

       _fIsRegionSet = kODTrue; // Set region flag
      }

}



SOM_Scope Widget  SOMLINK ODFacetCreateFacetWidget(ODFacet *somSelf,
                                                   Environment *ev,
                                                   Widget parentWidget,
                                                   ODPlatformCanvas* platformCanvas)
{

    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetCreateFacetWidget");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreateFacetWidget()\n");
#endif

   Dimension offsetx, offsety;
   Dimension lWidth, lHeight;
   lWidth =0; lHeight=0;
   offsetx=0; offsety=0;

          // Initialize region flag.  This can't be done in the facet's somInit.
          // This has to be done every time a facet widget is created since a
          // facet can have a widget, lose it (due to a ChangeCanvas) and get a widget again.
          _fIsRegionSet = kODFalse;

          TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
          ODRect RectBounds;

          TempODShape bShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
          bShape->Transform(ev, xform);
          bShape->GetBoundingBox(ev, &RectBounds);

          Point Pt2 = {FixedToInt(RectBounds.left), FixedToInt(RectBounds.top)};
          lWidth  = FixedToInt(RectBounds.right) - Pt2.x;
          lHeight = FixedToInt(RectBounds.bottom) - Pt2.y;

          offsetx = Pt2.x;
          offsety = Pt2.y;

         ODFacet* parentFacet = somSelf->GetContainingFacet(ev);
         if(parentFacet)
         {
           TempODTransform x2form = parentFacet->AcquireWindowFrameTransform(ev, kODNULL);

           ODPoint ParentPt1(0,0);
           TempODShape FrameShape = ODCopyAndRelease(ev,  parentFacet->GetFrame(ev)->AcquireFrameShape(ev, kODNULL));
           GetFacetPosition(ev, x2form, FrameShape, ParentPt1);
           Point ParentPt2 = ParentPt1.AsXPoint();

           offsetx += ParentPt2.x;
           offsety += ParentPt2.y;
          }


          Widget newWidget = XtVaCreateManagedWidget("FacetWidget",
                                xmDrawingAreaWidgetClass,
                                parentWidget,
                                XmNwidth, lWidth,
                                XmNheight, lHeight,
                                XmNx, offsetx,
                                XmNy, offsety,
                                XmNmarginWidth, 0,
                                XmNmarginHeight, 0,
                                XmNresizePolicy, XmRESIZE_NONE,
                                NULL);

          XtRealizeWidget(newWidget);

          Display *display = XtDisplay(newWidget);
          Window facetWindow = XtWindow(newWidget);
          GC facetGC = XCreateGC(display, facetWindow, 0, 0);


          ((ODAIXWindowCanvas*)platformCanvas)->InitPlatformWindowCanvas(ev, display, facetWindow, newWidget, facetGC);

          return newWidget;

}

SOM_Scope void  SOMLINK ODFacetCreateActiveBorderWidget(ODFacet *somSelf,
                                                        Environment *ev)
{

    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetCreateActiveBorderWidget");

    Pixel fg, bg;
    XSetWindowAttributes winatt;

    // root facet should not draw active border
    ODFrame* frame = somSelf->GetFrame(ev);

    if (!frame) return;

    ODWindow* window = somSelf->GetWindow(ev);
    if ( !(frame->IsRoot(ev)) && (!_fActiveBorderWidget) && window)
    {
        ODRect RectBounds;
        _fActiveBorderShape->GetBoundingBox(ev, &RectBounds);
        Rect XRect;
        RectBounds.AsXRect(XRect);

        Widget parentWidget= window->GetDrawingAreaWidget(ev);

        //ODFacet* parentFacet=somSelf->GetContainingFacet(ev);
        //Widget parentWidget = parentFacet->GetFacetWidget(ev);

        _fActiveBorderWidget = XtVaCreateManagedWidget(
                                "ActiveBorder",
                                xmDrawingAreaWidgetClass,
                                parentWidget,
                                XmNx, XRect.x,
                                XmNy, XRect.y,
                                XmNwidth, XRect.width,
                                XmNheight, XRect.height,
                                XmNmarginWidth, 0,
                                XmNmarginHeight, 0,
                                XmNresizePolicy, XmRESIZE_NONE,
                                XmNmappedWhenManaged, False,
                                XtVaTypedArg, XmNbackground,
                                                XmRString, "black",4,
                                XtVaTypedArg, XmNforeground,
                                                XmRString, "white",6,
                                NULL);

        // create slanted line pixmap on active border
        XtVaGetValues(_fActiveBorderWidget,
                        XmNbackground, &bg,
                        XmNforeground, &fg,
                        NULL);
        Pixmap slantedPix = XmGetPixmap(XtScreen(parentWidget),
                                        "slant_right", fg, bg);
        XtVaSetValues(_fActiveBorderWidget,
                        XmNbackgroundPixmap, slantedPix,
                        NULL);


        // set the mouse cursor when over the active border
        winatt.cursor = XCreateFontCursor(XtDisplay(parentWidget), XC_draped_box);
        XChangeWindowAttributes(XtDisplay(parentWidget), XtWindow(_fActiveBorderWidget), CWCursor, &winatt);
    }

}



SOM_Scope ODBoolean  SOMLINK ODFacetIsEventInActiveBorder(ODFacet *somSelf,
                                                          Environment *ev,
                                                          ODEventData* event)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetIsEventInActiveBorder");

        if (!event)
             return kODFalse;

        if ( (Window)event->window == XtWindow(_fActiveBorderWidget) )
              return kODTrue;
        else
              return kODFalse;
}

#endif // _PLATFORM_UNIX_


#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
SOM_Scope HWND  SOMLINK ODFacetCreateFacetWindow(ODFacet *somSelf,
                                                 Environment *ev,
                                                 HWND parentHWND)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetCreateFacetWindow");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreateFacetWindow()\n");
#endif
      FacetWndCtlData ctlData;
      ctlData.cb = sizeof(ctlData);
      ctlData.self = somSelf;
#ifdef _EVERYTHING_IS_A_WINDOW_
       // Initialize region flag.  This can't be done in the facet's somInit.
       // This has to be done every time a facet window is created since a
       // facet can have a window, lose it (due to a ChangeCanvas) and get a window again.
       _fIsRegionSet = kODFalse;

       ODFacet* parentFacet = somSelf->GetContainingFacet(ev);
       if(parentFacet)
          parentHWND = parentFacet->GetFacetHWND(ev);
#endif

#ifdef _PLATFORM_WIN32_
      int offsetx, offsety;
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
      LONG offsetx, offsety;
#endif  // _PLATFORM_OS2_

   ODULong lWidth, lHeight;
   lWidth =0; lHeight = 0;

          TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
          ODRect RectBounds;

  //        TempODShape bShape = _fClipShape->Copy(ev);
          TempODShape bShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
          bShape->Transform(ev, xform);
          bShape->GetBoundingBox(ev, &RectBounds);


#ifdef _PLATFORM_WIN32_
          Point Pt2;
          RECT RectShape;
          RectBounds.AsWinRect(RectShape);
          Pt2.x   = RectShape.left;
          Pt2.y   = RectShape.top;
          lWidth  = RectShape.right  - Pt2.x;
          lHeight = RectShape.bottom - Pt2.y;

          offsetx = Pt2.x;
          offsety = Pt2.y;
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
         POINTL Ptl ;
         RECTL RectLShape;
         RectBounds.AsRECTL(RectLShape);
         Ptl.x = RectLShape.xLeft;
         Ptl.y = RectLShape.yBottom;
         lWidth = RectLShape.xRight - Ptl.x;
         lHeight = RectLShape.yTop - Ptl.y;

         offsetx = Ptl.x;
         offsety = Ptl.y;
         PRINT("TESTING x =%d\t y=%d\n", offsetx, offsety);
#endif  // _PLATFORM_OS2_


#ifdef _PLATFORM_OS2_

    ULONG flStyle = WS_VISIBLE;

    HWND facetHWND = WinCreateWindow(
                                parentHWND,
                                pszFacetWinClass,
                                "",
                                flStyle,
                                offsetx,
                                offsety,
                                lWidth,
                                lHeight,
                                0,
                                HWND_BOTTOM,
                                0,
                                (PVOID)&ctlData,
                                NULL );
    if (!facetHWND) {
        THROW(kODErrCannotCreateWindow);
    }

    _fFacetHWND = facetHWND;
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_

#ifdef _EVERYTHING_IS_A_WINDOW_
      DWORD dwStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
#else
      DWORD dwStyle = WS_CHILDWINDOW | WS_VISIBLE;
#endif

      HINSTANCE hInstance = (HINSTANCE)GetWindowLong(parentHWND, GWL_HINSTANCE);

      HWND facetHWND = CreateWindow(pszFacetWinClass,
                                    "",
                                    dwStyle,
                                    offsetx,
                                    offsety,
                                    lWidth,
                                    lHeight,
                                    parentHWND,
                                    NULL,
                                    hInstance,
                                    (LPVOID)&ctlData);


    if (!facetHWND) {
        THROW(kODErrCannotCreateWindow);
    }


#endif // _PLATFORM_WIN32_

   return facetHWND;
}

#endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_


#ifdef _PLATFORM_OS2_
//------------------------------------------------------------------------------
// ODFacet: CreateOS2Canvas
//------------------------------------------------------------------------------

SOM_Scope ODOS2StandardCanvas*  SOMLINK ODFacetCreatePlatformCanvas(ODFacet *somSelf, Environment *ev,
    HPS hps)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CreatePlatformCanvas");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreatePlatformCanvas()\n");
#endif

    ODOS2StandardCanvas *c = kODNULL;  ODVolatile(c);

    SOM_TRY

    c = new ODOS2StandardCanvas;
    THROW_IF_NULL(c);
    c->InitPlatformCanvas(ev, hps);

    return c;
    SOM_CATCH_ALL{
      ODDeleteObject(c);
      return kODNULL;
    }
    SOM_ENDTRY
}

#endif // _PLATFORM_OS2_

//------------------------------------------------------------------------------
// ODFacet: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODFacetPurge(ODFacet *somSelf, Environment *ev,
       ODSize size)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","Purge");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::Purge()\n");
#endif
   ODSize freed = 0;   ODVolatile(freed);

   SOM_TRY
       // purge non-computed objects
       if ( _fCanvas )             freed += _fCanvas->Purge(ev, size-freed);  // Canvas didn't override Purge(), so no-op
       if ( _fClipShape )          freed += _fClipShape->Purge(ev, size-freed);
       if ( _fActiveShape )        freed += _fActiveShape->Purge(ev, size-freed);
       if ( _fExternalTransform )  freed += _fExternalTransform->Purge(ev, size-freed);
       if ( _fActiveBorderShape )  freed += _fActiveBorderShape->Purge(ev, size-freed);

       // flush cached geometry, purging first if other owners
       if ( _fAggregateClipShape )
       {
           if ( _fAggregateClipShape->GetRefCount(ev) > 1 )
               freed += _fAggregateClipShape->Purge(ev, size-freed);
           ODReleaseObject(ev, _fAggregateClipShape);
       }

       if ( _fWindowAggregateClipShape )
       {
           if ( _fWindowAggregateClipShape->GetRefCount(ev) > 1 )
               freed += _fWindowAggregateClipShape->Purge(ev, size-freed);
           ODReleaseObject(ev, _fWindowAggregateClipShape);
       }

       if ( _fFrameTransform )
       {
           if ( _fFrameTransform->GetRefCount(ev) > 1 )
               freed += _fFrameTransform->Purge(ev, size-freed);
           ODReleaseObject(ev, _fFrameTransform);
       }

       if ( _fContentTransform )
       {
           if ( _fContentTransform->GetRefCount(ev) > 1 )
               freed += _fContentTransform->Purge(ev, size-freed);
           ODReleaseObject(ev, _fContentTransform);
       }

       if ( _fWindowFrameTransform )
       {
           if ( _fWindowFrameTransform->GetRefCount(ev) > 1 )
               freed += _fWindowFrameTransform->Purge(ev, size-freed);
           ODReleaseObject(ev, _fWindowFrameTransform);
       }

       if ( _fWindowContentTransform )
       {
           if ( _fWindowContentTransform->GetRefCount(ev) > 1 )
               freed += _fWindowContentTransform->Purge(ev, size-freed);
           ODReleaseObject(ev, _fWindowContentTransform);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY

   return freed;
}

//------------------------------------------------------------------------------
// ODFacet: GetFrame
//------------------------------------------------------------------------------

SOM_Scope ODFrame*  SOMLINK ODFacetGetFrame(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","GetFrame");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetFrame()\n");
#endif
   return _fFrame;
}

//------------------------------------------------------------------------------
// ODFacet: CreateEmbeddedFacet
//------------------------------------------------------------------------------

SOM_Scope ODFacet*  SOMLINK ODFacetCreateEmbeddedFacet(ODFacet *somSelf, Environment *ev,
       ODFrame* frame,
       ODShape* clipShape,
       ODTransform* externalTransform,
       ODCanvas* canvas,
       ODCanvas* biasCanvas,
       ODFacet* siblingFacet,
       ODFramePosition position)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CreateEmbeddedFacet");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreateEmbeddedFacet()\n");
#endif
   ODFacet* newFacet  = kODNULL;       ODVolatile(newFacet);
   FacetNode* newNode = kODNULL;       ODVolatile(newNode);
   FacetNode* childNode = kODNULL;

#ifdef _PLATFORM_UNIX_
        Window siblingwindow=MOVE_WINDOW_TO_TOP;
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
    HWND siblingWindow=HWND_TOP;
#endif

   SOM_TRY

#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
       if (somSelf == siblingFacet)
           THROW(kODErrInvalidFacet);

       if ((position != kODFrameBehind) && (position != kODFrameInFront))
            THROW(kODErrUnsupportedFramePositionCode);
#endif // WIN32 or OS2 or UNIX

       if ( frame == kODNULL )
           THROW(kODErrIllegalNullFrameInput);
       if ( canvas != kODNULL )
       {
           TempODPart owner = canvas->AcquireOwner(ev);
           if ( (ODPart*)owner == kODNULL )
               THROW(kODErrCanvasHasNoOwner);
       }

       newFacet = new ODFacet();
       THROW_IF_NULL(newFacet);

       newNode = new FacetNode(newFacet);
       THROW_IF_NULL(newNode);

       if ( siblingFacet != kODNULL )
       {
#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
               if(somSelf != siblingFacet->GetContainingFacet(ev) )
                   THROW(kODErrInvalidFacet);

#endif // WIN32 or OS2 or UNIX
           childNode = siblingFacet->GetNode(ev);

#ifdef _PLATFORM_UNIX_
           siblingwindow= XtWindow(siblingFacet->GetFacetWidget(ev));
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
            siblingWindow = siblingFacet->GetFacetHWND(ev);
#endif
       }


       if ( position == kODFrameInFront )
       {
           if ( childNode != kODNULL )
               _fNode->AddChildBefore(*childNode, newNode);
           else {
                 _fNode->AddChildFirst(newNode);

                }
#ifdef _PLATFORM_UNIX_
            siblingwindow=MOVE_WINDOW_TO_TOP;
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
            siblingWindow = HWND_TOP;
#endif
       }
       else if ( position == kODFrameBehind )
       {
           if ( childNode != kODNULL )
               _fNode->AddChildAfter(*childNode, newNode);
           else {
               _fNode->AddChildLast(newNode);
                }

#ifdef _PLATFORM_UNIX_
            siblingwindow=MOVE_WINDOW_TO_BOTTOM;
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
                   siblingWindow = HWND_BOTTOM;
#endif
       }
       else
           THROW(kODErrUnsupportedFramePositionCode);

       newFacet->InitChildFacet(ev, newNode, frame, clipShape, externalTransform,
                                   canvas, biasCanvas);
       newNode = kODNULL;                          // owned by facet now

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
// Here GetCanvas() method is called in order to create facet size child window
// and also on-screen dynamic canvas to store HWND.
    if(canvas == kODNULL)                                                 // If no canvas passed in,
    {
      ODFacet* parent = newFacet->GetContainingFacet(ev);
      ODCanvas* parentCanvas = parent->GetCanvas(ev);
      if(parentCanvas && parentCanvas->IsDynamic(ev) && (!parentCanvas->IsOffscreen(ev)) )
      {
         ODCanvas* thisCanvas = newFacet->GetCanvas(ev);

#ifdef _PLATFORM_UNIX_
         Widget newfacetwidget = newFacet->GetFacetWidget(ev);
         Window newFacetwindow = XtWindow(newfacetwidget);
         if(newFacetwindow)
            newFacet->SetFacetWidgetWinPos(ev, newFacetwindow, siblingwindow, kODTrue );
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
      // Move and position the window
         HWND facetHWND = newFacet->GetFacetHWND(ev);
         newFacet->SetFacetWindowPos(ev, facetHWND, siblingWindow);
#endif
       }

    }
#endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_


       frame->FacetAdded(ev, newFacet);
   SOM_CATCH_ALL
       ODDeleteObject(newFacet);
       if( newNode ) {
           newNode->GetParent()->RemoveChild(*newNode);
           delete newNode;
       }
   SOM_ENDTRY
   return newFacet;
}

//------------------------------------------------------------------------------
// ODFacet: RemoveFacet
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetRemoveFacet(ODFacet *somSelf, Environment *ev,
       ODFacet* facet)
{
   ODFacetData *somThis = ODFacetGetData(somSelf);
   ODFacetMethodDebug("ODFacet","RemoveFacet");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::RemoveFacet()\n");
#endif
   SOM_TRY

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
        if( (facet == kODNULL) || (somSelf != facet->GetContainingFacet(ev)) )
          THROW(kODErrInvalidFacet);
#endif // OS2 or WIN32 or UNIX

#ifdef _PLATFORM_MACINTOSH_
       ASSERT( (facet->GetContainingFacet(ev) == somSelf), kODErrInvalidFacet);
       // must get StorageUnit from part as frame might be non-persistent
       TempODPart tempPart = _fFrame->AcquirePart(ev);
       tempPart->GetStorageUnit(ev)->GetSession(ev)->
           GetDispatcher(ev)->InvalidateFacetUnderMouse(ev);
#endif  // _PLATFORM_MACINTOSH_
       _fNode->RemoveChild(*(facet->GetNode(ev)));
       facet->GetFrame(ev)->FacetRemoved(ev, facet);   // tells part to remove children

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
        facet->SetPartInfo(ev, kODNULL);
#endif // OS2 or WIN32 or UNIX

   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: MoveBefore
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetMoveBefore(ODFacet *somSelf, Environment *ev,
       ODFacet* child,
       ODFacet* sibling)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","MoveBefore");
#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::MoveBefore()\n");
#endif

   SOM_TRY

       Node* childNode;
       Node* siblingNode;

       if ( child )
       {

#ifdef _PLATFORM_UNIX_
        Window siblingwindow = MOVE_WINDOW_TO_TOP;
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
          HWND siblingWindow = HWND_TOP;
#endif
           childNode = child->GetNode(ev);
           _fNode->RemoveChild(*childNode);
           if ( sibling )
           {
#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
               if(child->GetContainingFacet(ev) != sibling->GetContainingFacet(ev) )
                   THROW(kODErrInvalidFacet);
#endif
               siblingNode = sibling->GetNode(ev);
               _fNode->AddChildBefore(*siblingNode, childNode);

#ifdef _PLATFORM_UNIX_
               siblingwindow = XtWindow(sibling->GetFacetWidget(ev));
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
               siblingWindow = sibling->GetFacetHWND(ev);
#endif
           }
           else
           {
               _fNode->AddChildFirst(childNode);
           }

#ifdef _PLATFORM_UNIX_
        Window childwindow = XtWindow(child->GetFacetWidget(ev));
        if(childwindow)
           child->SetFacetWidgetWinPos(ev, childwindow, siblingwindow, kODTrue );
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
           HWND hwnd = child->GetFacetHWND(ev);
#ifdef _PLATFORM_OS2_
         WinSetWindowPos(hwnd, siblingWindow, 0, 0, 0, 0, SWP_SHOW | SWP_ZORDER | SWP_NOREDRAW);
#endif

#ifdef _PLATFORM_WIN32_
                SetWindowPos(hwnd, siblingWindow, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE);

#endif

#endif
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: MoveBehind
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetMoveBehind(ODFacet *somSelf, Environment *ev,
       ODFacet* child,
       ODFacet* sibling)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","MoveBehind");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::MoveBehind()\n");
#endif


   SOM_TRY
       Node* childNode;
       Node* siblingNode;

       if ( child )
       {

#ifdef _PLATFORM_UNIX_
        Window siblingwindow = MOVE_WINDOW_TO_BOTTOM;
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
   HWND siblingWindow=HWND_BOTTOM;
#endif
           childNode = child->GetNode(ev);
           _fNode->RemoveChild(*childNode);
           if ( sibling )
           {
#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
               if(child->GetContainingFacet(ev) != sibling->GetContainingFacet(ev) )
                   THROW(kODErrInvalidFacet);
#endif
               siblingNode = sibling->GetNode(ev);
               _fNode->AddChildAfter(*siblingNode, childNode);

#ifdef _PLATFORM_UNIX_
               siblingwindow = XtWindow(sibling->GetFacetWidget(ev));
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
               siblingWindow = sibling->GetFacetHWND(ev);
#endif
           }
           else
           {
               _fNode->AddChildLast(childNode);
           }

#ifdef _PLATFORM_UNIX_
        Window childwindow = XtWindow(child->GetFacetWidget(ev));
        if(childwindow)
           child->SetFacetWidgetWinPos(ev, childwindow, siblingwindow, kODFalse );
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
           HWND hwnd = child->GetFacetHWND(ev);
#ifdef _PLATFORM_OS2_
         WinSetWindowPos(hwnd, siblingWindow, 0, 0, 0, 0, SWP_SHOW | SWP_ZORDER | SWP_NOREDRAW);
#endif

#ifdef _PLATFORM_WIN32_
                SetWindowPos(hwnd, siblingWindow, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE);

#endif

#endif

       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)

SOM_Scope void  SOMLINK ODFacetSetFacetWindowRgn(ODFacet *somSelf, Environment *ev, ODBoolean IsUsedShape)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetSetFacetWindowRgn");
    TempODShape UShape=kODNULL;

#ifdef _EVERYTHING_IS_A_WINDOW_
       UShape = ODCopyAndRelease(ev, somSelf->GetFrame(ev)->AcquireUsedShape(ev, kNoBias));

    if(!IsUsedShape)
    {
       TempODShape clipShape = ODCopyAndRelease(ev, somSelf->AcquireClipShape(ev, kNoBias));
       UShape->Intersect(ev, clipShape);
    }

    if(UShape)
    {
     somSelf->Invalidate(ev, kODNULL, kODNULL);
     TempODTransform xform =  somSelf->AcquireWindowFrameTransform(ev, kODNULL);
     UShape->Transform(ev, xform);

     ODPoint offset(0,0);
     TempODShape FrameShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
     GetFacetPosition(ev, xform, FrameShape, offset);
     HWND hwnd = somSelf->GetFacetHWND(ev);

#ifdef _PLATFORM_WIN32_
     Point winpt = offset.AsWinPoint();
     ODRgnHandle UShapeRgn = UShape->GetRegion(ev);
     OffsetRgn(UShapeRgn, winpt.x, winpt.y);
     OSVERSIONINFO osinfo;
     osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
     GetVersionEx(&osinfo);

     if(osinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
     {
        if(_fUsedShapeRgn != kODNULL)
        {
           SetWindowRgn(hwnd, NULL, FALSE);
           DeleteObject(_fUsedShapeRgn);
        }

      //  if(!UShape->IsRectangular(ev))
          _fUsedShapeRgn = CreateRectRgn(0, 0, 0, 0);
      // else
      //    _fUsedShapeRgn = CreateEllipticRgn(0, 0, 0, 0);

        int result = CombineRgn(_fUsedShapeRgn, UShapeRgn, 0, RGN_COPY);
        SetWindowRgn(hwnd, _fUsedShapeRgn, FALSE);
     }
     else
         SetWindowRgn(hwnd, UShapeRgn, FALSE);

#endif //_PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
     ODOS2WindowCanvas *platcanv = (ODOS2WindowCanvas *) somSelf->GetCanvas(ev)->GetPlatformCanvas(ev, kODPM);
     HPS hps = platcanv->GetPS(ev);

     POINTL winpt = offset.AsPOINTL();
     ODRgnHandle usedShapeRgn = UShape->GetRegion(ev);
     GpiOffsetRegion(hps, usedShapeRgn, &winpt);

     WinSetWindowPtr(hwnd, QWL_FACETWINDOWRGN, (PVOID)1); // Set region flag, used by CopyAggregateClipRegion() method
     WinSetClipRegion(hwnd, usedShapeRgn);  // NEW PM API CALL
     platcanv->ReleasePS(ev);
#endif //_PLATFORM_OS2_

     _fIsRegionSet = kODTrue; // Set region flag
    }
#endif // EVERYTHING IS A WINDOW
}


SOM_Scope void  SOMLINK ODFacetSetFacetWindowPos(ODFacet *somSelf, Environment *ev, HWND facetHWND, HWND siblingHWND)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ODFacetSetFacetWindowPos");

#ifdef _EVERYTHING_IS_A_WINDOW_
    ODULong lWidth, lHeight;
    lWidth =0; lHeight = 0;

      TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
      ODRect RectBounds;

   // TempODShape bShape = _fClipShape->Copy(ev);
      TempODShape bShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
      bShape->Transform(ev, xform);
      bShape->GetBoundingBox(ev, &RectBounds);


#ifdef _PLATFORM_WIN32_
         Point childPt2;
         RECT RectShape;
         RectBounds.AsWinRect(RectShape);
         childPt2.x = RectShape.left;
         childPt2.y = RectShape.top;
         lWidth  = RectShape.right  - childPt2.x;
         lHeight = RectShape.bottom - childPt2.y;
#endif //_PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
         POINTL childPt2;
         RECTL RectLShape;
         RectBounds.AsRECTL(RectLShape);
         childPt2.x = RectLShape.xLeft;
         childPt2.y = RectLShape.yBottom;
         lWidth  = RectLShape.xRight  - childPt2.x;
         lHeight = RectLShape.yTop    - childPt2.y;
#endif //_PLATFORM_OS2_

         ODFacet* parentFacet = somSelf->GetContainingFacet(ev);
         if(parentFacet)
         {
           TempODTransform x2form = parentFacet->AcquireWindowFrameTransform(ev, kODNULL);
           ODPoint ParentPt1(0,0);
           TempODShape tshape = ODCopyAndRelease(ev, parentFacet->GetFrame(ev)->AcquireFrameShape(ev, kODNULL));
           GetFacetPosition(ev, x2form, tshape, ParentPt1);

#ifdef _PLATFORM_WIN32_
           Point ParentPt2 = ParentPt1.AsWinPoint();
#endif //_PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
           POINTL ParentPt2 = ParentPt1.AsPOINTL();
#endif //_PLATFORM_OS2_

           childPt2.x += ParentPt2.x;
           childPt2.y += ParentPt2.y;

         }


         // Move and position the window
#ifdef _PLATFORM_WIN32_

         if(siblingHWND == HWND_MOVE_THE_WINDOW)
         {
          MoveWindow(facetHWND, childPt2.x, childPt2.y, lWidth, lHeight, FALSE);
         }
         else {
                SetWindowPos(facetHWND, siblingHWND, childPt2.x, childPt2.y, lWidth, lHeight, SWP_NOACTIVATE | SWP_NOREDRAW);
        }
#endif //_PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
         ULONG flOptions = SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_NOREDRAW;
         if(siblingHWND)
            flOptions |= SWP_ZORDER;
         WinSetWindowPos(facetHWND, siblingHWND, childPt2.x, childPt2.y, lWidth, lHeight, flOptions);
#endif //_PLATFORM_OS2_

         if(_fClipShape)
         {
           // If the window has a region
           if (_fIsRegionSet)
           {
             // Current design is once a facet window region, always a facet window region.
             // A possible enhancement would be to delete the facet window
             // region if the facet's clipshape and frame's usedshape become the same as the
             // rectangular frameshape. An example is a toggle between a round analog
             // and a rectangular digital clock.
             somSelf->SetFacetWindowRgn(ev, kODFalse);
           }
           else
           {
             // If the transformed clipshape is not rectangular or is different than the
             // transform frame shape
             TempODShape clipShape = _fClipShape->Copy(ev);
             clipShape->Transform(ev, xform);
             if( (!clipShape->IsRectangular(ev)) || (!clipShape->IsSameAs(ev, bShape)) )
             {
               // We've gone down the road where this facet window will now always have a
               // region associated with it.
               somSelf->SetFacetWindowRgn(ev, kODFalse);
             }
           }
         }


#endif  // EVERYTHING IS A WINDOW

}

#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_

//------------------------------------------------------------------------------
// ODFacet: GetContainingFacet
//------------------------------------------------------------------------------

SOM_Scope ODFacet*  SOMLINK ODFacetGetContainingFacet(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","GetContainingFacet");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetContainingFacet()\n");
#endif
   SOM_TRY
       FacetNode* parent = (FacetNode*) _fNode->GetParent();
       if (parent)
           return parent->GetFacet();
       else
           return kODNULL;
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: CreateFacetIterator
//------------------------------------------------------------------------------

SOM_Scope ODFacetIterator*  SOMLINK ODFacetCreateFacetIterator(ODFacet *somSelf, Environment *ev,
       ODTraversalType traversalType,
       ODSiblingOrder siblingOrder)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CreateFacetIterator");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreateFacetIterator()\n");
#endif
   ODFacetIterator* iter = kODNULL; ODVolatile(iter);
   SOM_TRY
       iter = new ODFacetIterator();
       THROW_IF_NULL(iter);
       iter->InitFacetIterator(ev, somSelf, traversalType, siblingOrder);
   SOM_CATCH_ALL
       ODDeleteObject(iter);
       iter = kODNULL;
   SOM_ENDTRY
   return iter;
}

//------------------------------------------------------------------------------
// ODFacet: ChangeGeometry
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetChangeGeometry(ODFacet *somSelf, Environment *ev,
       ODShape* clipShape,
       ODTransform* transform,
       ODCanvas* biasCanvas)

{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ChangeGeometry");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::ChangeGeometry()\n");
#endif
    SOM_TRY
       ODBoolean clipShapeChanged = kODFalse;
       ODBoolean externalTransformChanged = kODFalse;

#ifdef _PLATFORM_OS2_
       somSelf->InvalidateAggregateClipShape(ev);
#endif
       if ( clipShape )
       {
           ODAcquireObject(ev, clipShape);
           ODReleaseObject(ev, _fClipShape);
           _fClipShape = BiasShapeSet(ev, clipShape, biasCanvas);
           clipShapeChanged = kODTrue;
           somSelf->InvalidateAggregateClipShape(ev);

       }

       if ( transform )
       {
           ODAcquireObject(ev, transform);
           ODReleaseObject(ev, _fExternalTransform);
           _fExternalTransform = BiasTransformSet(ev, transform, biasCanvas);
           externalTransformChanged = kODTrue;
           somSelf->InvalidateAggregateTransforms(ev);
           somSelf->InvalidateAggregateClipShape(ev);

       }

#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_)
       if (( clipShape ) || ( transform ))
#endif // _PLATFORM_WIN32_
       {
           ODFacetIterator* i = new ODFacetIterator();
           THROW_IF_NULL(i);
           i->InitFacetIterator(ev, somSelf, kODTopDown, kODFrontToBack);
           for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
           {

#ifdef _PLATFORM_UNIX_
              facet->SetFacetWidgetSize(ev);
#endif // _PLATFORM_UNIX_

#ifdef _EVERYTHING_IS_A_WINDOW_
               ODPlatformWindow hwnd;
               hwnd = facet->GetFacetHWND(ev);
               if (hwnd)
               {
                   // Move and position the window
#ifdef _PLATFORM_WIN32_
                   facet->SetFacetWindowPos(ev, hwnd, HWND_MOVE_THE_WINDOW);
#endif
#ifdef _PLATFORM_OS2_
                   facet->SetFacetWindowPos(ev, hwnd, 0);
#endif
               }
#endif   // EVERYTHING IS A WINDOW
               TempODPart tempPart = facet->GetFrame(ev)->AcquirePart(ev);
               tempPart->GeometryChanged(ev, facet, clipShapeChanged, externalTransformChanged);
           }
           ODDeleteObject(i);
       }

   SOM_CATCH_ALL
   SOM_ENDTRY
}


//------------------------------------------------------------------------------
// ODFacet: CreateShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFacetCreateShape(ODFacet *somSelf, Environment *ev)
{
   ODFacetData *somThis = ODFacetGetData(somSelf);
   ODFacetMethodDebug("ODFacet","CreateShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreateShape()\n");
#endif
   ODShape *s = kODNULL;  ODVolatile(s);
    SOM_TRY
       s = new ODShape;
       THROW_IF_NULL(s);
       s->InitShape(ev);
    SOM_CATCH_ALL
       ODDeleteObject(s);
       s = kODNULL;
    SOM_ENDTRY
    return s;
}


//------------------------------------------------------------------------------
// ODFacet: AcquireClipShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFacetAcquireClipShape(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireClipShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireClipShape()\n");
#endif
    SOM_TRY
       if ( _fClipShape )
           return BiasShapeGet(ev, _fClipShape, biasCanvas);
       else
           return ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, biasCanvas));
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: AcquireAggregateClipShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFacetAcquireAggregateClipShape(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireAggregateClipShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireAggregateClipShape()\n");
#endif

    SOM_TRY
       if ( !_fAggregateClipShape )
       {
           ODFacet* parent = somSelf->GetContainingFacet(ev);

           // Determine whether canvas is geometric or whether we can punt to regions: [jpa]
#ifdef _PLATFORM_MACINTOSH_
           ODGeometryMode mode = _fCanvas  ?GetCanvasGeometryMode(ev,_fCanvas)
                                           :kODLoseGeometry;
#endif  // _PLATFORM_MACINTOSH_

           if ( _fCanvas && _fCanvas->IsOffscreen(ev) )
           {
               _fAggregateClipShape = ODCopyAndRelease(ev, _fFrame->AcquireFrameShape(ev, kODNULL));
#ifdef _PLATFORM_MACINTOSH_
               _fAggregateClipShape->SetGeometryMode(ev,mode);
#endif  // _PLATFORM_MACINTOSH_
           }
           else
           {
               _fAggregateClipShape = ODCopyAndRelease(ev, somSelf->AcquireClipShape(ev, kODNULL));
#ifdef _PLATFORM_MACINTOSH_
               _fAggregateClipShape->SetGeometryMode(ev,mode);
#endif   // _PLATFORM_MACINTOSH_
               if ( parent )
               {
                   TempODShape parentClip = kODNULL;
                   TempODTransform xform = kODNULL;
                   parentClip = ODCopyAndRelease(ev, parent->AcquireAggregateClipShape(ev, kODNULL));
                   xform = ODCopyAndRelease(ev,
                       parent->GetFrame(ev)->AcquireInternalTransform(ev, kODNULL));
                   if ( _fExternalTransform != kODNULL )
                       xform->PreCompose(ev, _fExternalTransform);
                   parentClip->InverseTransform(ev, xform);
                   _fAggregateClipShape->Intersect(ev, parentClip);
               }
           }
       }
       return BiasShapeGet(ev, _fAggregateClipShape, biasCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

#ifdef _PLATFORM_OS2_
//------------------------------------------------------------------------------
// ODFacet: CopyAggregateClipRegion
//------------------------------------------------------------------------------

SOM_Scope ODRgnHandle SOMLINK ODFacetCopyAggregateClipRegion(ODFacet *somSelf, Environment *ev, HPS hps)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CopyAggregateClipRegion");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CopyAggregateClipRegion()\n");
#endif
    HRGN mainRgn;

    SOM_TRY
      RECTL mainRect;
      HWND mainhwnd = somSelf->GetFacetHWND(ev);
      WinQueryWindowRect(mainhwnd, &mainRect) ;
      mainRgn = GpiCreateRegion(hps, 1L, &mainRect);
      HENUM henum   = WinBeginEnumWindows(mainhwnd);                                           // enum over child windows
      HWND hwndNext;
      RECTL childrectl;
      HAB hab = WinQueryAnchorBlock(HWND_DESKTOP);
      ODRgnHandle childClipRgn;
      SWP swp;
      POINTL offsetRgn;
      BOOL boolRC;

      // Clip children

      while ((hwndNext = WinGetNextWindow(henum)) != NULLHANDLE)   // for each child window...
      {
        // First check to see if window rects intersect
        RECTL DestRect;
        WinQueryWindowRect(hwndNext, &childrectl);                                // Get child rect
        WinMapWindowPoints(hwndNext, mainhwnd, (PPOINTL)&childrectl, 2);          // map rect to parent
        WinIntersectRect(hab, &DestRect, &childrectl, &mainRect);                 // get overlap rect

        if(!WinIsRectEmpty(hab, &DestRect))                                       // if overlap
        {
          BOOL WindowIsRgn = (BOOL)WinQueryWindowPtr(hwndNext, QWL_FACETWINDOWRGN);              // Is region or rect?
          LONG lComplexity;

          if (WindowIsRgn)  // If used shape is region, not rectangular
          {
            childClipRgn = GpiCreateRegion(hps, 0, 0);
            WinQueryClipRegion(hwndNext, childClipRgn);                                          // Get child region
            WinQueryWindowPos(hwndNext, &swp);                                                   // Get its offset from me
            POINTL offsetRgn = {swp.x, swp.y};
            boolRC           = GpiOffsetRegion(hps, childClipRgn, &offsetRgn);                   // Offset child region
            LONG lComplexity = GpiCombineRegion(hps,mainRgn, mainRgn, childClipRgn, CRGN_DIFF);  // Subtract child region
            boolRC           = GpiDestroyRegion(hps, childClipRgn);
          }
          else              // If used shape is rectangular
          {
            childClipRgn     = GpiCreateRegion(hps, 1L, &childrectl);                            // region is rect
            LONG lComplexity = GpiCombineRegion(hps,mainRgn, mainRgn, childClipRgn, CRGN_DIFF);  // Subtract child rect
            boolRC           = GpiDestroyRegion(hps, childClipRgn);
          }
        }
      }
      WinEndEnumWindows(henum);

      // Clip siblings

      if(!somSelf->GetFrame(ev)->IsRoot(ev))               // Don't need to do root window
      {
        HWND  parenthwnd = WinQueryWindow(mainhwnd, QW_PARENT);                         // only parent knows my siblings
        HENUM henum      = WinBeginEnumWindows(parenthwnd);                             // enum over siblings (prime)
        WinMapWindowPoints(mainhwnd, parenthwnd, (PPOINTL)&mainRect, 2);                // map rect to parent
        SWP swpmain;
        WinQueryWindowPos(mainhwnd, &swpmain);                                          // pos to parent
        POINTL offsetRgnMain = {swpmain.x, swpmain.y};
        BOOL boolRC          = GpiOffsetRegion(hps, mainRgn, &offsetRgnMain);           // map rgn to parent

        while ((hwndNext = WinGetNextWindow(henum)) != NULLHANDLE)                      // enum over siblings
        {
          if(mainhwnd == hwndNext)                                                      // don't check self and behind
            break;

          RECTL DestRect;
          WinQueryWindowRect(hwndNext, &childrectl);                                  // sibling rect
          WinMapWindowPoints(hwndNext, parenthwnd, (PPOINTL)&childrectl, 2);          // map rect to parent
          WinIntersectRect(hab, &DestRect, &childrectl, &mainRect);                   // get overlap rect

          if(!WinIsRectEmpty(hab, &DestRect))                                         // if overlap
          {
            BOOL WindowIsRgn = (BOOL)WinQueryWindowPtr(hwndNext, QWL_FACETWINDOWRGN);     // Window ptr tells if non-rect

            if (WindowIsRgn)  // If used shape is region, not rectangular
            {
              childClipRgn = GpiCreateRegion(hps, 0, 0);
              WinQueryClipRegion(hwndNext, childClipRgn);                                 // sibling region
              WinQueryWindowPos(hwndNext, &swp);                                          // pos to parent
              POINTL offsetRgn = {swp.x, swp.y};
              boolRC           = GpiOffsetRegion(hps, childClipRgn, &offsetRgn); // map rgn to parent
              LONG lComplexity = GpiCombineRegion(hps,mainRgn, mainRgn, childClipRgn, CRGN_DIFF);
              boolRC           = GpiDestroyRegion(hps, childClipRgn);
            }
            else              // if used shape is rectangular
            {
              childClipRgn     = GpiCreateRegion(hps, 1L, &DestRect);                  // rect to region
              LONG lComplexity = GpiCombineRegion(hps,mainRgn, mainRgn, childClipRgn, CRGN_DIFF);
              boolRC           = GpiDestroyRegion(hps, childClipRgn);
            }
          }
        }
        offsetRgnMain.x = -swpmain.x;                                                   // restore back to orig offset
        offsetRgnMain.y = -swpmain.y;                                                   // restore back to orig offset
        boolRC = GpiOffsetRegion(hps, mainRgn, &offsetRgnMain);                        // map rgn back
        WinEndEnumWindows(henum);
      }
    SOM_CATCH_ALL
    SOM_ENDTRY
    return mainRgn;
}
#endif

//------------------------------------------------------------------------------
// ODFacet: AcquireWindowAggregateClipShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFacetAcquireWindowAggregateClipShape(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireWindowAggregateClipShape");
#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireWindowAggregateClipShape()\n");
#endif

    SOM_TRY
       ODFacet* parent = somSelf->GetContainingFacet(ev);

       if ( !_fWindowAggregateClipShape )
       {
           // Determine whether canvas is geometric or whether we can punt to regions: [jpa]
#ifdef _PLATFORM_MACINTOSH_
           ODGeometryMode mode = _fCanvas  ?GetCanvasGeometryMode(ev,_fCanvas)
                                           :kODLoseGeometry;
#endif   // _PLATFORM_MACINTOSH_
           _fWindowAggregateClipShape = ODCopyAndRelease(ev, somSelf->AcquireClipShape(ev, kNoBias));
#ifdef _PLATFORM_MACINTOSH_
           _fWindowAggregateClipShape->SetGeometryMode(ev,mode);
#endif   // _PLATFORM_MACINTOSH_

           if ( parent )
           {
               TempODShape parentClip = kODNULL;
               TempODTransform xform = kODNULL;
               parentClip = ODCopyAndRelease(ev, parent->AcquireWindowAggregateClipShape(ev, kNoBias));
               xform = ODCopyAndRelease(ev,
                   parent->GetFrame(ev)->AcquireInternalTransform(ev, kODNULL));
               if ( _fExternalTransform != kODNULL )
                   xform->PreCompose(ev, _fExternalTransform);
               parentClip->InverseTransform(ev, xform);
               _fWindowAggregateClipShape->Intersect(ev, parentClip);
           }
       }
       return BiasShapeGet(ev, _fWindowAggregateClipShape, biasCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: InvalidateAggregateClipShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetInvalidateAggregateClipShape(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","InvalidateAggregateClipShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::InvalidateAggregateClipShape()\n");
#endif
    SOM_TRY
       // flush aggregate clip shapes
       ODReleaseObject(ev, _fAggregateClipShape);
       ODReleaseObject(ev, _fWindowAggregateClipShape);

       // must flush cached border shape, as it is based on aggClipShape
       if ( _fActiveBorderShape != kODNULL )
           somSelf->InvalidateActiveBorder(ev);

       LinkedListIterator iter((LinkedList*)_fNode);
       for (FacetNode* node = (FacetNode*) iter.First();
               iter.IsNotComplete();
               node = (FacetNode*) iter.Next())
       {
           node->GetFacet()->InvalidateAggregateClipShape(ev);
       }

#ifdef _PLATFORM_OS2_
    somSelf->InvalidateActiveBorder(ev);
//#else
//    somSelf->DrawActiveBorder(ev);
#endif

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: AcquireActiveShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFacetAcquireActiveShape(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireActiveShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireActiveShape()\n");
#endif
    SOM_TRY
       if ( _fActiveShape )
           return BiasShapeGet(ev, _fActiveShape, biasCanvas);
       else
           return ODCopyAndRelease(ev, _fFrame->AcquireFrameShape(ev, biasCanvas));
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: ChangeActiveShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetChangeActiveShape(ODFacet *somSelf, Environment *ev,
       ODShape* activeShape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ChangeActiveShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::ChangeActiveShape()\n");
#endif
    SOM_TRY
       if ( _fActiveBorderShape != kODNULL )
       {
           somSelf->InvalidateActiveBorder(ev);
           ODFacet* contFacet = somSelf->GetContainingFacet(ev);
           if ( contFacet != kODNULL )
           {
               TempODPart part = contFacet->GetFrame(ev)->AcquirePart(ev);
               part->AdjustBorderShape(ev, somSelf, kODNULL);
           }
       }

       ODAcquireObject(ev, activeShape);
       ODReleaseObject(ev, _fActiveShape);
       _fActiveShape = BiasShapeSet(ev, activeShape, biasCanvas);


#ifdef _PLATFORM_OS2_
        somSelf->InvalidateActiveBorder(ev);
//#else
//        somSelf->DrawActiveBorder(ev);
#endif
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: CreateTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFacetCreateTransform(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CreateTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreateTransform()\n");
#endif
    ODTransform *t = kODNULL;  ODVolatile(t);

    SOM_TRY
       t = new ODTransform;
       THROW_IF_NULL(t);
       t->InitTransform(ev);
    SOM_CATCH_ALL
       ODDeleteObject(t);
       t = kODNULL;
    SOM_ENDTRY
    return t;
}

//------------------------------------------------------------------------------
// ODFacet: AcquireExternalTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFacetAcquireExternalTransform(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireExternalTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireExternalTransform()\n");
#endif
   ODTransform* xform = kODNULL;

    SOM_TRY
       if ( _fExternalTransform != kODNULL )
           xform = _fExternalTransform;
       else
           xform = somSelf->CreateTransform(ev);
       return BiasTransformGet(ev, xform, biasCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: AcquireFrameTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFacetAcquireFrameTransform(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireFrameTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireFrameTransform()\n");
#endif
    SOM_TRY
       ODFacet* parent = somSelf->GetContainingFacet(ev);

       if ( !_fFrameTransform )
       {
#ifdef _PLATFORM_MACINTOSH_
           if ( _fCanvas )
           {
               if ( _fFrame->IsRoot(ev) && (_fExternalTransform != kODNULL) )
                   _fFrameTransform = _fExternalTransform->Copy(ev);
               else
                   _fFrameTransform = somSelf->CreateTransform(ev);
           }
#endif // _PLATFORM_MACINTOSH_

#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
           if ( _fCanvas )
           {
             if (_fExternalTransform != kODNULL)
                   _fFrameTransform = _fExternalTransform->Copy(ev);
             else
                 _fFrameTransform = somSelf->CreateTransform(ev);

                if(!_fFrame->IsRoot(ev) )
                {
                   if ( parent )
                   {
                     TempODTransform xform = parent->AcquireContentTransform(ev, kODNULL);
                     _fFrameTransform->PostCompose(ev, xform);
                     ODPoint offset;

                     TempODShape tshape = ODCopyAndRelease(ev, _fFrame->AcquireFrameShape(ev, kODNULL));
                     GetFacetPosition(ev, _fFrameTransform, tshape, offset);
                     _fFrameTransform->MoveBy(ev, &offset);
                    }

                }

           }
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_

           else
           {
               if ( _fExternalTransform != kODNULL )
                   _fFrameTransform = _fExternalTransform->Copy(ev);
               else
                   _fFrameTransform = somSelf->CreateTransform(ev);
               if ( parent )
               {
                   TempODTransform xform = parent->AcquireContentTransform(ev, kODNULL);
                   _fFrameTransform->PostCompose(ev, xform);
               }
           }
       }
       return BiasTransformGet(ev, _fFrameTransform, biasCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: AcquireContentTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFacetAcquireContentTransform(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireContentTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireContentTransform()\n");
#endif
    SOM_TRY
       if (!_fContentTransform)
       {
           _fContentTransform = ODCopyAndRelease(ev, _fFrame->AcquireInternalTransform(ev, kODNULL));
           TempODTransform xform = somSelf->AcquireFrameTransform(ev, kODNULL);
           _fContentTransform->PostCompose(ev, xform);
       }
       return BiasTransformGet(ev, _fContentTransform, biasCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: AcquireWindowFrameTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFacetAcquireWindowFrameTransform(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireWindowFrameTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcuireWindowFrameTransform()\n");
#endif
    SOM_TRY
       ODFacet* parent = somSelf->GetContainingFacet(ev);

       if (!_fWindowFrameTransform)
       {
           if ( _fExternalTransform != kODNULL )
               _fWindowFrameTransform = _fExternalTransform->Copy(ev);
           else
               _fWindowFrameTransform = somSelf->CreateTransform(ev);
           if (parent)
           {
               TempODTransform xform = parent->AcquireWindowContentTransform(ev, kODNULL);
               _fWindowFrameTransform->PostCompose(ev, xform);
           }
       }
       return BiasTransformGet(ev, _fWindowFrameTransform, biasCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: AcquireWindowContentTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFacetAcquireWindowContentTransform(ODFacet *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireWindowContentTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireWindowContentTransform()\n");
#endif
    SOM_TRY
       if (!_fWindowContentTransform)
       {
           _fWindowContentTransform = ODCopyAndRelease(ev, _fFrame->AcquireInternalTransform(ev, kODNULL));
           TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
           _fWindowContentTransform->PostCompose(ev, xform);
       }
       return BiasTransformGet(ev, _fWindowContentTransform, biasCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: InternalTransformChanged
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetInternalTransformChanged(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","InternalTransformChanged");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::InternalTransformChanged()\n");
#endif
   ODFacetIterator* facets = kODNULL;  ODVolatile(facets);
   ODFacet* facet = kODNULL;

    SOM_TRY
       ODReleaseObject(ev, _fContentTransform);
       ODReleaseObject(ev, _fWindowContentTransform);

       // invalidate children
       LinkedListIterator iter((LinkedList*)_fNode);
       for (FacetNode* node = (FacetNode*) iter.First();
               iter.IsNotComplete();
               node = (FacetNode*) iter.Next())
       {
           node->GetFacet()->InvalidateAggregateClipShape(ev);
           node->GetFacet()->InvalidateAggregateTransforms(ev);
       }

       facets = somSelf->CreateFacetIterator(ev, kODTopDown, kODFrontToBack);
       for ( facet = facets->First(ev);
               facets->IsNotComplete(ev);
               facet = facets->Next(ev) )
       {

#ifdef _PLATFORM_UNIX_
              facet->SetFacetWidgetSize(ev);
#endif // _PLATFORM_UNIX_

#ifdef _EVERYTHING_IS_A_WINDOW_
               ODPlatformWindow hwnd;
               hwnd = facet->GetFacetHWND(ev);
               if (hwnd)
               {
                   // Move and position the window
#ifdef _PLATFORM_WIN32_
                   facet->SetFacetWindowPos(ev, hwnd, HWND_MOVE_THE_WINDOW);
#endif
#ifdef _PLATFORM_OS2_
                   facet->SetFacetWindowPos(ev, hwnd, 0);
#endif
               }
#endif   // EVERYTHING IS A WINDOW

           TempODPart tempPart = facet->GetFrame(ev)->AcquirePart(ev);
           tempPart->GeometryChanged(ev, facet, kODFalse, kODTrue);
       }
       ODDeleteObject(facets);

    SOM_CATCH_ALL
       ODDeleteObject(facets);
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: InvalidateAggregateTransforms
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetInvalidateAggregateTransforms(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","InvalidateAggregateTransforms");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::InvalidteAggregateTransforms()\n");
#endif
   SOM_TRY
       ODReleaseObject(ev, _fFrameTransform);
       ODReleaseObject(ev, _fContentTransform);
       ODReleaseObject(ev, _fWindowFrameTransform);
       ODReleaseObject(ev, _fWindowContentTransform);

       // invalidate children
       LinkedListIterator iter((LinkedList*)_fNode);
       for (FacetNode* node = (FacetNode*) iter.First();
               iter.IsNotComplete();
               node = (FacetNode*) iter.Next())
       {
           node->GetFacet()->InvalidateAggregateTransforms(ev);
       }
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: CreateCanvas
//------------------------------------------------------------------------------

SOM_Scope ODCanvas*  SOMLINK ODFacetCreateCanvas(ODFacet *somSelf, Environment *ev,
       ODGraphicsSystem graphicsSystem,
#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
    ODPlatformCanvas* platformCanvas,
#else
       ODPlatformCanvas platformCanvas,
#endif   // _PLATFORM_OS2_
       ODBoolean isDynamic,
       ODBoolean isOffscreen)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","CreateCanvas");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::CreateCanvas()\n");
#endif
    ODCanvas *c = kODNULL;  ODVolatile(c);

    SOM_TRY
       c = new ODCanvas;
       THROW_IF_NULL(c);
       c->InitCanvas(ev, graphicsSystem,platformCanvas,isDynamic,isOffscreen);
    SOM_CATCH_ALL
       ODDeleteObject(c);
       c = kODNULL;
    SOM_ENDTRY
    return c;
}

//------------------------------------------------------------------------------
// ODFacet: HasCanvas
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFacetHasCanvas(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","HasCanvas");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::HasCanvas()\n");
#endif
   return (_fCanvas != kODNULL);
}

//------------------------------------------------------------------------------
// ODFacet: GetCanvas
//------------------------------------------------------------------------------

SOM_Scope ODCanvas*  SOMLINK ODFacetGetCanvas(ODFacet *somSelf, Environment *ev)
{
  ODFacetData *somThis = ODFacetGetData(somSelf);
  ODFacetMethodDebug("ODFacet","GetCanvas");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetCanvas()\n");
#endif
  SOM_TRY
    if (_fCanvas)
         return _fCanvas;
    else                              // don't have a canvas, so search up thru Facet heirarchy
    {
      ODFacet* parent = somSelf->GetContainingFacet(ev);
      if (parent)                                        // If there is a parent Facet
#ifdef _PLATFORM_MACINTOSH_
      {
        return parent->GetCanvas(ev);                    // return it's Canvas (or it's parent's canvas)
      }
#endif  // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_UNIX_
      {
        ODCanvas* parentCanvas = parent->GetCanvas(ev);
        if(parentCanvas && parentCanvas->IsDynamic(ev) && !parentCanvas->IsOffscreen(ev))
        {
           Widget parentWidget = parent->GetFacetWidget(ev);
           if(parentWidget)
           {
              ODAIXWindowCanvas* newAIXCanvas = new ODAIXWindowCanvas;
              THROW_IF_NULL(newAIXCanvas);
              _fFacetWidget = somSelf->CreateFacetWidget(ev, parentWidget, newAIXCanvas);
              ODCanvas* newCanvas = somSelf->CreateCanvas(ev,
                                                          kODAIX,
                                                          newAIXCanvas,
                                                          kODTrue,    // IsDynamic?
                                                          kODFalse);  // IsOffscreen?

               _fCanvas = newCanvas;
                if ( _fCanvas )
                {
                  _fCanvas->SetFacet(ev, somSelf);                      // point back to this facet
                  TempODPart tempPart = _fFrame->AcquirePart(ev);            // Owner is new embedded Frame's part
                  _fCanvas->SetOwner(ev, tempPart);                                 // so that he can change it.
                }

            }

                return _fCanvas;

        }
        else
          return parentCanvas;
      }
#endif // _PLATFORM_UNIX_


// Don't use parent canvas if it is Dynamic Onscreen since
// a multi-threaded part would trap on sharing an hdc.  This
// only occurs if a part has removed a Dynamic Onscreen canvas thru a call to ChangeCanvas().
#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_)
      {
        ODCanvas* parentCanvas = parent->GetCanvas(ev);
        if(parentCanvas && parentCanvas->IsDynamic(ev) && !parentCanvas->IsOffscreen(ev))
        {
          ODWindow* window = somSelf->GetWindow(ev);     // Use the client hwnd as parent
          if (window)
          {
             HWND facetHWND, hwndParent;
#ifdef _PLATFORM_WIN32_
             hwndParent = window->GetPlatformWindow(ev);
#endif

#ifdef _PLATFORM_OS2_
             hwndParent = WinWindowFromID(window->GetPlatformWindow(ev), FID_CLIENT);
#endif
             if(hwndParent)
                facetHWND = somSelf->CreateFacetWindow(ev, hwndParent);

#ifdef _PLATFORM_OS2_
             ODOS2WindowCanvas* newWindowCanvas = new ODOS2WindowCanvas;
             THROW_IF_NULL(newWindowCanvas);
             newWindowCanvas->InitPlatformWindowCanvas(ev, facetHWND);
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
             ODWin32WindowCanvas* newWindowCanvas = new ODWin32WindowCanvas;
             THROW_IF_NULL(newWindowCanvas);
             newWindowCanvas->InitPlatformWindowCanvas(ev, facetHWND);
#endif // _PLATFORM_WIN32_
             ODCanvas* newCanvas = somSelf->CreateCanvas(ev,
                                                      #ifdef _PLATFORM_WIN32_
                                                              kODWin32,
                                                      #endif
                                                      #ifdef _PLATFORM_OS2_
                                                              kODPM,
                                                      #endif
                                                              newWindowCanvas,
                                                              kODTrue,    // IsDynamic?
                                                              kODFalse);  // IsOffscreen?

            _fCanvas = newCanvas;

            if ( _fCanvas )
            {
               _fCanvas->SetFacet(ev, somSelf);                      // point back to this facet
              TempODPart tempPart = _fFrame->AcquirePart(ev);            // Owner is new embedded Frame's part
              _fCanvas->SetOwner(ev, tempPart);                                 // so that he can change it.
             }  // _fCanvas

            } // window is valid
            return _fCanvas;

        } // parent canvas is On-Screen dynamic
        else
          return parentCanvas;
      }
#endif // _PLATFORM_WIN32_

      else
        THROW(kODErrCanvasNotFound);

    }
  SOM_CATCH_ALL
  SOM_ENDTRY
  return kODNULL;
}

SOM_Scope void  SOMLINK ODFacetChangeCanvas(ODFacet *somSelf, Environment *ev,
       ODCanvas* canvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ChangeCanvas");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::ChangeCanvas()\n");
#endif
   // I don't manage storage for the canvas - that's up to my part!
   ODFacetIterator* i = kODNULL;   ODVolatile(i);
    SOM_TRY

#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
     if(_fCanvas && _fCanvas->IsDynamic(ev) && (!_fCanvas->IsOffscreen(ev)) )
     {
#ifdef _PLATFORM_OS2_
       HWND hwnd = somSelf->GetFacetHWND(ev);
       if(hwnd)
          WinDestroyWindow(hwnd);
          _fFacetHWND = kODNULL;

       ODPlatformCanvas* platCanvas = _fCanvas->GetPlatformCanvas(ev, kODPM);
#endif

#ifdef _PLATFORM_WIN32_
       HWND hwnd = somSelf->GetFacetHWND(ev);
       if(hwnd)
          DestroyWindow(hwnd);

       ODPlatformCanvas* platCanvas = _fCanvas->GetPlatformCanvas(ev, kODWin32);
#endif

#ifdef _PLATFORM_UNIX_
         ODPlatformCanvas* platCanvas = _fCanvas->GetPlatformCanvas(ev, kODAIX);
         if (platCanvas->IsInitialized(ev))
         {
             GC gc = ((ODAIXWindowCanvas*)platCanvas)->GetGC(ev);
             XFreeGC(XtDisplay(_fFacetWidget), gc);
         }

// When facet gets deleted, then somUninit would destroy this widget.
//        if(_fFacetWidget)
//        {
//         XtDestroyWidget(_fFacetWidget);
//         _fFacetWidget = kODNULL;
//        }

#endif // _PLATFORM_UNIX_

         ODDeleteObject(platCanvas);             //  delete WindowCanvas
         ODDeleteObject(_fCanvas);               //  delete ODCanvas
     }

#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_

       if ( canvas != kODNULL )
       {
           TempODPart owner = canvas->AcquireOwner(ev);
           if ( (ODPart*)owner == kODNULL )
               THROW(kODErrCanvasHasNoOwner);
       }

       i = new ODFacetIterator();
       THROW_IF_NULL(i);
       i->InitFacetIterator(ev, somSelf, kODTopDown, kODBackToFront);

       if ( _fCanvas )
           _fCanvas->SetFacet(ev, kODNULL);
       _fCanvas = canvas;
       if ( _fCanvas )
       {
           _fCanvas->SetFacet(ev, somSelf);        // point back to this facet
           TempODShape tempShape = _fFrame->AcquireFrameShape(ev, kODNULL);
           _fCanvas->Invalidate(ev, tempShape); // contents are initially invalid
       }

       somSelf->InvalidateAggregateClipShape(ev);
       somSelf->InvalidateAggregateTransforms(ev);

       ODBoolean notFirst = kODFalse;
       for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
       {
#ifdef _PLATFORM_MACINTOSH_
           if ( notFirst && facet->HasCanvas(ev) )
#endif

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
               if ( facet->HasCanvas(ev) && facet->GetCanvas(ev)->IsOffscreen(ev))
#endif // OS2 || WIN32 || UNIX
               i->SkipChildren(ev);
           else
           {
               TempODPart tempPart = facet->GetFrame(ev)->AcquirePart(ev);
               tempPart->CanvasChanged(ev, somSelf);
           }
           notFirst = kODTrue;
       }
       ODDeleteObject(i);
    SOM_CATCH_ALL
       ODDeleteObject(i);
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: GetWindow
//------------------------------------------------------------------------------

SOM_Scope ODWindow*  SOMLINK ODFacetGetWindow(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","GetWindow");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetWindow()\n");
#endif
   // this method returns fFrame's window.  getting the window from fFrame
   // automatically increments the window's refcount, but because the window
   // in an invariant of the facet we don't want to increment the refcount.
   // so after getting the window from the frame, release it to restore its
   // original refcount.

   ODWindow* window = kODNULL;

    SOM_TRY
       window = _fFrame->AcquireWindow(ev);
       if (window)
           window->Release(ev); // undo ref bump from frame->AcquireWindow;
       return window;
    SOM_CATCH_ALL
    SOM_ENDTRY
   return kODNULL;
}

//------------------------------------------------------------------------------
// ODFacet: GetPartInfo
//------------------------------------------------------------------------------

SOM_Scope ODInfoType  SOMLINK ODFacetGetPartInfo(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","GetPartInfo");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetPartInfo()\n");
#endif
   return _fPartInfo;
}

SOM_Scope void  SOMLINK ODFacetSetPartInfo(ODFacet *somSelf, Environment *ev,
       ODInfoType partInfo)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","SetPartInfo");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::SetPartInfo()\n");
#endif
   _fPartInfo = partInfo;
}

//------------------------------------------------------------------------------
// ODFacet: DragEnter
//------------------------------------------------------------------------------

SOM_Scope ODDragResult  SOMLINK ODFacetDragEnter(ODFacet *somSelf, Environment *ev,
       ODPoint* point,
       ODDragItemIterator* dragInfo,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","DragEnter");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::DragEnter()\n");
#endif
    SOM_TRY
       ODPoint bPoint = BiasPointSet(ev, point, biasCanvas);
       TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       xform->InvertPoint(ev, &bPoint);
       TempODPart tempPart = _fFrame->AcquirePart(ev);
       return tempPart->DragEnter(ev, dragInfo, somSelf, &bPoint);
    SOM_CATCH_ALL
    SOM_ENDTRY
   return kODFalse;
}

//------------------------------------------------------------------------------
// ODFacet: DragWithin
//------------------------------------------------------------------------------

SOM_Scope ODDragResult  SOMLINK ODFacetDragWithin(ODFacet *somSelf, Environment *ev,
       ODPoint* point,
       ODDragItemIterator* dragInfo,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","DragWithin");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::DragWithin()\n");
#endif
    SOM_TRY
       ODPoint bPoint = BiasPointSet(ev, point, biasCanvas);
       TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       xform->InvertPoint(ev, &bPoint);
       TempODPart tempPart = _fFrame->AcquirePart(ev);
       return tempPart->DragWithin(ev, dragInfo, somSelf, &bPoint);
    SOM_CATCH_ALL
    SOM_ENDTRY
   return kODFalse;
}

//------------------------------------------------------------------------------
// ODFacet: DragLeave
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetDragLeave(ODFacet *somSelf, Environment *ev,
       ODPoint* point,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","DragLeave");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::DragLeave()\n");
#endif
    SOM_TRY
       ODPoint bPoint = BiasPointSet(ev, point, biasCanvas);
       TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       xform->InvertPoint(ev, &bPoint);
       TempODPart tempPart = _fFrame->AcquirePart(ev);
       tempPart->DragLeave(ev, somSelf, &bPoint);
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: Drop
//------------------------------------------------------------------------------

SOM_Scope ODDropResult  SOMLINK ODFacetDrop(ODFacet *somSelf, Environment *ev,
       ODPoint* point,
       ODDragItemIterator* dropInfo,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","Drop");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::Drop()\n");
#endif
    SOM_TRY
       ODPoint bPoint = BiasPointSet(ev, point, biasCanvas);
       TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       xform->InvertPoint(ev, &bPoint);
       TempODPart tempPart = _fFrame->AcquirePart(ev);
       return tempPart->Drop(ev, dropInfo, somSelf, &bPoint);
    SOM_CATCH_ALL
    SOM_ENDTRY
   return kODDropFail;
}

#ifdef _PLATFORM_OS2_
SOM_Scope ODShape* SOMLINK ODFacetAcquireUpdateShape(ODFacet *somSelf,
                                                     Environment *ev,
                                                     ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","AcquireUpdateShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::AcquireUpdateShape()\n");
#endif
    if (_fIsUpdating && _fUpdateShape)
    {
       return BiasShapeGet(ev, _fUpdateShape, biasCanvas);
    }
    else
    {
       ODFacet* containingFacet = somSelf->GetContainingFacet(ev);
       if (containingFacet)
          return containingFacet->AcquireUpdateShape(ev, biasCanvas);
    }
    return kODNULL;
}
#endif

//------------------------------------------------------------------------------
// ODFacet: Update
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetUpdate(ODFacet *somSelf, Environment *ev,
       ODShape* invalidShape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","Update");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::Update()\n");
#endif

#ifdef _FACET_TIMING_
    unsigned int t1,t2,t3,t4;
    t1=RD_TSR();
#endif
   ODFacet*        facet = kODNULL;
   ODBoolean       notFirst = kODFalse;

   SOM_TRY
       _fIsUpdating = kODTrue;

       TempODShape bShape = kODNULL;
       if ( invalidShape == kODNULL )
           bShape = ODCopyAndRelease(ev, _fFrame->AcquireFrameShape(ev, kODNULL));
       else
           bShape = BiasShapeSet(ev, invalidShape->Copy(ev), biasCanvas);

       // transform bShape into window coords for intersection with
       // shapes of embedded facets
       TempODTransform wfXform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       bShape->Transform(ev, wfXform);

       ODFacetIterator* i = new ODFacetIterator();
       THROW_IF_NULL(i);
       i->InitFacetIterator(ev, somSelf, kODTopDown, kODBackToFront);


       for (facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
       {
           TempODShape aggClipShape = facet->AcquireAggregateClipShape(ev, kODNULL);
           TempODShape testShape = aggClipShape->Copy(ev);
           TempODTransform xform = facet->AcquireWindowFrameTransform(ev, kODNULL);
           testShape->Transform(ev, xform);

           // both bShape and testShape are now in window coords
           testShape->Intersect(ev, bShape);
           if ( testShape->IsEmpty(ev) )
               i->SkipChildren(ev);
           else
           {
               facet->SetNeedsUpdate(ev, kODTrue);

               // if facet has its own canvas, and facet is not the root facet,
               // let the facet handle updating its children
#ifdef _PLATFORM_MACINTOSH_
               if ( facet->HasCanvas(ev) && notFirst )
#endif

#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_)
               if ( facet->HasCanvas(ev) && facet->GetCanvas(ev)->IsOffscreen(ev))
#endif  // WIN32 || OS2 || AIX
                   i->SkipChildren(ev);
           }
#ifdef _PLATFORM_MACINTOSH_
           notFirst = kODTrue;
#endif
       }

       for (facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
       {
           if ( facet->NeedsUpdate(ev) )
           {
               TempODShape testShape = bShape->Copy(ev);
               TempODTransform xform = facet->AcquireWindowFrameTransform(ev, kODNULL);
               testShape->InverseTransform(ev, xform);
#ifdef _PLATFORM_OS2_
               TempODShape AggClipShape = facet->AcquireAggregateClipShape(ev, kODNULL);
                testShape->Intersect(ev, AggClipShape);
#endif // OS2
#ifdef _FACET_TIMING_
    t2=RD_TSR();
#endif
               facet->Draw(ev, testShape, kODNULL);
#ifdef _FACET_TIMING_
    t3=RD_TSR();
    qprintf("cycles to Draw facet needing update=%i\n",t3-t2);
#endif
           }
           else
               i->SkipChildren(ev);
       }

       _fIsUpdating = kODFalse;

       delete i;
    SOM_CATCH_ALL
    SOM_ENDTRY
#ifdef _FACET_TIMING_
    t4=RD_TSR();
    qprintf("cycles to execute ODFacet::UPdate=%i\n",t4-t1);
#endif
}

//------------------------------------------------------------------------------
// ODFacet: Invalidate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetInvalidate(ODFacet *somSelf, Environment *ev,
       ODShape* invalidShape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","Invalidate");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::Invalidate()\n");
#endif
   //  Assumes invalidShape is in frame coordinates.

   ODCanvas* myCanvas = kODNULL;
   ODFacet* myCanvasFacet = kODNULL;
   ODFacet* myContainingFacet = kODNULL;
   ODFacet* parentCanvasFacet = kODNULL;

   SOM_TRY
       ODWindow* window = somSelf->GetWindow(ev);
       if( !window )
           return;

       TempODShape tShape = ODCopyAndRelease(ev, somSelf->AcquireAggregateClipShape(ev, kODNULL));

       if ( invalidShape )
       {
           TempODShape bShape = BiasShapeSet(ev, invalidShape->Copy(ev), biasCanvas);
           tShape->Intersect(ev, bShape);
       }

       if( !tShape->IsEmpty(ev) )
       {
           myCanvas = somSelf->GetCanvas(ev);
           TempODTransform xf1 = somSelf->AcquireFrameTransform(ev, kODNULL);
           tShape->Transform(ev, xf1);
           myCanvas->Invalidate(ev, tShape);
           tShape->InverseTransform(ev, xf1);


           if ( myCanvas->IsOffscreen(ev) )
           {
               myCanvasFacet = myCanvas->GetFacet(ev);
               myContainingFacet = myCanvasFacet->GetContainingFacet(ev);
               if ( myContainingFacet )
                   parentCanvasFacet = myContainingFacet->GetCanvas(ev)->GetFacet(ev);
               else
                   THROW(kODErrFacetNotFound);

               TempODTransform xf2 = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
               tShape->Transform(ev, xf2);
               TempODTransform xf3 = parentCanvasFacet->AcquireWindowFrameTransform(ev, kODNULL);
               tShape->InverseTransform(ev, xf3);
               parentCanvasFacet->Invalidate(ev, tShape, kODNULL);
           }
#ifdef _PLATFORM_WIN32_  // SA
           else {  // on-screen Canvas
                 TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
                 tShape->Transform(ev, xform);
                 HWND hwnd = window->GetPlatformWindow(ev);
                 InvalidateRgn(hwnd, tShape->GetRegion(ev), FALSE);
                 }
#endif // _PLATFORM_WIN32_


#ifdef _PLATFORM_OS2_
           else {
                 TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
                 tShape->Transform(ev, xform);
                 HWND hwnd = WinWindowFromID(window->GetPlatformWindow(ev), FID_CLIENT);
                 WinInvalidateRegion(hwnd, tShape->GetRegion(ev), FALSE);
                 }
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_UNIX_
           else {
                   TempODTransform xform = ODCopyAndRelease(ev, somSelf->AcquireWindowFrameTransform(ev, kODNULL));

                   if(invalidShape)
                   {
                    ODPoint offset(0,0);
                    TempODShape FrameShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
                    GetFacetPosition(ev, xform, FrameShape, offset);

                    xform->MoveBy(ev, &offset);
                   }

                   tShape->Transform(ev, xform);
                   ODRect odRect;
                   tShape->GetBoundingBox(ev, &odRect);
                   Rect xRect;
                   odRect.AsXRect(xRect);

                   if(!invalidShape)
                   {
                      xRect.x = 0;
                      xRect.y = 0;
                   }

           if ( myCanvas->IsDynamic(ev) )
                   XClearArea(XtDisplay(_fFacetWidget),
                            XtWindow(_fFacetWidget),
                            xRect.x,
                            xRect.y,
                            xRect.width,
                            xRect.height,
                            True);
                }
#endif // _PLATFORM_UNIX
       }  // Shape is NOT empty
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: Validate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetValidate(ODFacet *somSelf, Environment *ev,
       ODShape* validShape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","Validate");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::Validate()\n");
#endif
   //  Assumes validShape is in frame coordinates.

   ODCanvas* myCanvas = kODNULL;
   ODTransform* xform = kODNULL;

   SOM_TRY
       ODWindow* window = somSelf->GetWindow(ev);
       if( !window )
           return;

       TempODShape tShape = ODCopyAndRelease(ev, somSelf->AcquireAggregateClipShape(ev, kODNULL));
       if ( validShape )
       {
           TempODShape bShape = BiasShapeSet(ev, validShape->Copy(ev), biasCanvas);
           tShape->Intersect(ev, bShape);
       }

       if( !tShape->IsEmpty(ev) )
       {
           myCanvas = somSelf->GetCanvas(ev);
           TempODTransform xform = somSelf->AcquireFrameTransform(ev, kODNULL);
           tShape->Transform(ev, xform);
           myCanvas->Validate(ev, tShape);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
           tShape->InverseTransform(ev, xform);
           TempODTransform x2form = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
           tShape->Transform(ev, x2form);
#endif

#ifdef _PLATFORM_OS2_
           HWND hwnd = WinWindowFromID(window->GetPlatformWindow(ev), FID_CLIENT);
           WinValidateRegion(hwnd, tShape->GetRegion(ev), TRUE);
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
           HWND hwnd = window->GetPlatformWindow(ev);
           ValidateRgn(hwnd, tShape->GetRegion(ev));
#endif // _PLATFORM_WIN32_
       }


    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: Draw
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetDraw(ODFacet *somSelf, Environment *ev,
       ODShape* invalidShape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","Draw");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::Draw()\n");
#endif
   SOM_TRY
       TempODShape bShape = kODNULL;
       if ( invalidShape == kODNULL )
           bShape = ODCopyAndRelease(ev, _fFrame->AcquireFrameShape(ev, kODNULL));
       else
           bShape = BiasShapeSet(ev, invalidShape->Copy(ev), biasCanvas);

       if ( _fCanvas && !_fIsUpdating && _fCanvas->IsOffscreen(ev) )
       {
           // repair canvas if needs updating
           TempODShape testShape = bShape->Copy(ev);
           TempODShape updateShape = _fCanvas->AcquireUpdateShape(ev);
           testShape->Intersect(ev, updateShape);

           if ( !testShape->IsEmpty(ev) )
           {
               // fix offscreen canvas
               somSelf->Update(ev, bShape, kODNULL);
           }

           // tell canvas owner part to copy pixels
           TempODPart owner = _fCanvas->AcquireOwner(ev);
           owner->CanvasUpdated(ev, _fCanvas);
       }
       else
       {
           TempODPart tempPart = _fFrame->AcquirePart(ev);
           tempPart->Draw(ev, somSelf, bShape);
       }
       _fNeedsUpdate = kODFalse;
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: DrawChildren
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetDrawChildren(ODFacet *somSelf, Environment *ev,
       ODShape* invalidShape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","DrawChildren");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::DrawChildren()\n");
#endif
   SOM_TRY
       TempODShape bShape = kODNULL;
       if ( invalidShape == kODNULL )
           bShape = ODCopyAndRelease(ev, _fFrame->AcquireFrameShape(ev, kODNULL));
       else
           bShape = BiasShapeSet(ev, invalidShape->Copy(ev), biasCanvas);
       TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       bShape->Transform(ev, xform);

       ODFacetIterator* i = new ODFacetIterator;
       THROW_IF_NULL(i);
       i->InitFacetIterator(ev, somSelf, kODChildrenOnly, kODBackToFront);

       for ( ODFacet* child = i->First(ev); i->IsNotComplete(ev); child = i->Next(ev) )
       {
           if ( child->NeedsUpdate(ev) )
           {
               TempODTransform xf2 = child->AcquireWindowFrameTransform(ev, kODNULL);
               TempODShape tempShape = bShape->Copy(ev);
               tempShape->InverseTransform(ev, xf2);
               child->Draw(ev, tempShape, kODNULL);
               child->DrawChildren(ev, tempShape, kODNULL);
           }
       }
       ODDeleteObject(i);
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: DrawChildrenAlways
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetDrawChildrenAlways(ODFacet *somSelf, Environment *ev,
       ODShape* invalidShape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","DrawChildrenAlways");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::DrawChildrenAlways()\n");
#endif
   SOM_TRY
       TempODShape bShape = kODNULL;
       if ( invalidShape == kODNULL )
           bShape = ODCopyAndRelease(ev, _fFrame->AcquireFrameShape(ev, kODNULL));
       else
           bShape = BiasShapeSet(ev, invalidShape->Copy(ev), biasCanvas);
       TempODTransform xf1 = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       bShape->Transform(ev, xf1);

       ODFacetIterator* i = new ODFacetIterator;
       THROW_IF_NULL(i);
       i->InitFacetIterator(ev, somSelf, kODChildrenOnly, kODBackToFront);

       for ( ODFacet* child = i->First(ev); i->IsNotComplete(ev); child = i->Next(ev) )
       {
           TempODTransform xf2 = child->AcquireWindowFrameTransform(ev, kODNULL);
           TempODShape tempShape = bShape->Copy(ev);
           tempShape->InverseTransform(ev, xf2);
           child->Draw(ev, tempShape, kODNULL);
           child->DrawChildrenAlways(ev, tempShape, kODNULL);
       }
       ODDeleteObject(i);
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: InvalidateActiveBorder
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODFacetInvalidateActiveBorder(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","InvalidateActiveBorder");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::InvalidateActiveBorder()\n");
#endif
   SOM_TRY

#ifdef _PLATFORM_UNIX_
      if(_fActiveBorderWidget)
      {
        ODFacet* parentFacet = somSelf->GetContainingFacet(ev);
        Widget parentWidget = parentFacet->GetFacetWidget(ev);
        XUnmapWindow(XtDisplay(parentWidget), XtWindow(_fActiveBorderWidget) );
        XtDestroyWidget(_fActiveBorderWidget);
        _fActiveBorderWidget = kODNULL;

      }
#endif

       if ( _fActiveBorderShape != kODNULL )
       {
           ODInvalidateBorder(ev, _fActiveBorderShape, somSelf);
           ODReleaseObject(ev, _fActiveBorderShape);
       }
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: DrawActiveBorder
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODFacetDrawActiveBorder(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","DrawActiveBorder");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::DrawActiveBorder()\n");
#endif

   SOM_TRY
       ODFacet* contFacet = kODNULL;
       TempODPart contPart = kODNULL;
       TempODShape border = kODNULL;
       TempODShape scratch = kODNULL;
       TempODShape adjusted = kODNULL;
       TempODShape clip = kODNULL;
       TempODTransform xform = kODNULL;
       TempODTransform xform2 = kODNULL;
       TempODTransform xform3 = kODNULL;

       if ( _fActiveBorderShape == kODNULL )
       {
           // compute border shape
           contFacet = somSelf->GetContainingFacet(ev);
           if ( contFacet )
           {
               contPart = contFacet->GetFrame(ev)->AcquirePart(ev);
               border = ODCopyAndRelease(ev, somSelf->AcquireActiveShape(ev, kNoBias));
               xform = ODCopyAndRelease(ev, somSelf->AcquireWindowFrameTransform(ev, kNoBias));
               xform2 = contFacet->AcquireWindowFrameTransform(ev, kNoBias);
               border->Transform(ev, xform);
               border->SetGeometryMode(ev, kODLoseGeometry);
               scratch = border->Copy(ev);

               border->Outset(ev, ODIntToFixed(4));
               border->Subtract(ev, scratch);
               ODReleaseObject(ev, scratch);

               border->InverseTransform(ev, xform);
               adjusted = contPart->AdjustBorderShape(ev, somSelf, border);
               THROW_IF_NULL(adjusted);
               ODReleaseObject(ev, border);

               if ( adjusted->IsEmpty(ev) )
               {
                   // compute as inset
                   scratch = ODCopyAndRelease(ev, contFacet->GetFrame(ev)->AcquireUsedShape(ev, kNoBias));
                   scratch->Transform(ev, xform2);
                   border = ODCopyAndRelease(ev, somSelf->AcquireActiveShape(ev, kNoBias));
                   border->Transform(ev, xform);
                   border->Subtract(ev, scratch);
                   if ( !border->IsEmpty(ev) )
                   {
                       ODReleaseObject(ev, adjusted);
                       ODReleaseObject(ev, border);
                       border = scratch->Copy(ev);
                       scratch->Outset(ev, ODIntToFixed(-4));
                       border->Subtract(ev, scratch);
                       ODReleaseObject(ev, scratch);

                       border->InverseTransform(ev, xform);
                       adjusted = contPart->AdjustBorderShape(ev, somSelf, border);
                       THROW_IF_NULL(adjusted);
                       ODReleaseObject(ev, border);
                   }
                   else
                   {
                       ODReleaseObject(ev, border);
                       ODReleaseObject(ev, scratch);
                   }
               }

               // clip to containing facet's visible area
               clip = ODCopyAndRelease(ev,
                   contFacet->AcquireWindowAggregateClipShape(ev, kNoBias));


#ifdef _PLATFORM_MACINTOSH_
               xform3 = ODCopyAndRelease(ev,
                   contFacet->GetFrame(ev)->AcquireInternalTransform(ev, kNoBias));
               if ( _fExternalTransform != kODNULL )
                   xform3->PreCompose(ev, _fExternalTransform);
               clip->InverseTransform(ev, xform3);
#endif // _PLATFORM_MACINTOSH_

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
            TempODTransform xform3 = xform2->Copy(ev)->Invert(ev);
            xform3->PreCompose(ev, xform);
            clip->InverseTransform(ev, xform3);
#endif  // _PLATFORM_OS2_  || _PLATFORM_WIN32_ || _PLATFORM_UNIX_


               adjusted->Intersect(ev, clip);

#ifdef _PLATFORM_WIN32_
          ODPoint parentPt1(0,0);
          TempODShape FrameShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
          GetFacetPosition(ev, xform2, FrameShape, parentPt1);
          xform->MoveBy(ev, &parentPt1);
#endif
               adjusted->Transform(ev, xform);
               adjusted->Acquire(ev);  // because TempODShape releases on scope exit
               _fActiveBorderShape = adjusted;
           }
       }

       ODWindow* window = somSelf->GetWindow(ev);
       if ( _fActiveBorderShape && window && window->IsShown(ev) )
       {
#ifdef _PLATFORM_UNIX_
          if ( !_fActiveBorderWidget )
                  somSelf->CreateActiveBorderWidget(ev);

          XtRealizeWidget(_fActiveBorderWidget);

          ODFacet* parentFacet=somSelf->GetContainingFacet(ev);
          Widget parentWidget = parentFacet->GetFacetWidget(ev);

          // use the border "donut" shape to surround this facet
          TempODShape tmp = _fActiveBorderShape->Copy(ev);
          Region borderRegion = tmp->GetRegion(ev);

          Position x, y;
          XtVaGetValues(_fActiveBorderWidget,
                        XmNx, &x,
                        XmNy, &y,
                        NULL);

          XOffsetRegion(borderRegion, -x, -y);

          XShapeCombineRegion(XtDisplay(parentWidget),
                                  XtWindow(_fActiveBorderWidget),
                                  ShapeBounding,
                                  0, 0,
                                  borderRegion,
                                  ShapeSet);

          // make active border visible
            XtMapWidget(_fActiveBorderWidget);

#else
           ODDrawBorder(ev, _fActiveBorderShape, somSelf);
#endif
       }
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: DrawnIn
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetDrawnIn(ODFacet *somSelf, Environment *ev,
       ODShape* shape,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","DrawnIn");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::DrawnIn()\n");
#endif
   // invalidate shape on my parent canvas to ensure that changed
   // pixels get copied to it.

   ODCanvas* myCanvas = kODNULL;
   ODFacet* myCanvasFacet = kODNULL;
   ODFacet* containingFacet = kODNULL;
   ODFacet* parentCanvasFacet = kODNULL;

   SOM_TRY
       // since the shape has been drawn in, it can now be marked as valid
       somSelf->Validate(ev, shape, biasCanvas);

       myCanvas = somSelf->GetCanvas(ev);

       // don't need to worry about on-screen canvas - it's the window
       if ( !myCanvas->IsOffscreen(ev) )
           return;

       TempODShape tShape = ODCopyAndRelease(ev, somSelf->AcquireAggregateClipShape(ev, kODNULL));
       if ( shape )
       {
           TempODShape bShape = BiasShapeSet(ev, shape->Copy(ev), biasCanvas);
           tShape->Intersect(ev, bShape);
       }

       myCanvasFacet = myCanvas->GetFacet(ev);
       containingFacet = myCanvasFacet->GetContainingFacet(ev);
       if ( containingFacet )
           parentCanvasFacet = containingFacet->GetCanvas(ev)->GetFacet(ev);
       else
           THROW(kODErrFacetNotFound);

       TempODTransform xform = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
       tShape->Transform(ev, xform);
       ODReleaseObject(ev, xform);
       xform = parentCanvasFacet->AcquireWindowFrameTransform(ev, kODNULL);
       tShape->InverseTransform(ev, xform);

       // since stuff was drawn on my canvas, invalidate my parent canvas
       // so that that content will get copied there.
       parentCanvasFacet->Invalidate(ev, tShape, kODNULL);

       // notify my canvas' owner that drawing has occurred, and pixels
       // need to be copied to the parent canvas
       TempODPart owner = myCanvas->AcquireOwner(ev);
       WASSERTMSG( owner != kODNULL, "Facet::DrawnIn - no owner for canvas",0);
       owner->CanvasUpdated(ev, myCanvas);
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: ContainsPoint
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFacetContainsPoint(ODFacet *somSelf, Environment *ev,
       ODPoint* point,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ContainsPoint");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::ContainsPoint()\n");
#endif
   ODBoolean result = kODFalse;    ODVolatile(result);

   SOM_TRY
       ODPoint bPoint = BiasPointSet(ev, point, biasCanvas);
       if ( _fActiveShape || _fClipShape )
           result = ((!_fActiveShape || _fActiveShape->ContainsPoint(ev, &bPoint))
                    && (!_fClipShape || _fClipShape->ContainsPoint(ev, &bPoint)));
       else
       {
           TempODShape frameShape = _fFrame->AcquireFrameShape(ev, kNoBias);
           result = frameShape->ContainsPoint(ev, &bPoint);
       }
   SOM_CATCH_ALL
       result = kODFalse;
   SOM_ENDTRY

   return result;
}

//------------------------------------------------------------------------------
// ODFacet: ActiveBorderContainsPoint
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFacetActiveBorderContainsPoint(ODFacet *somSelf, Environment *ev,
       ODPoint* point,
       ODCanvas* biasCanvas)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ActiveBorderContainsPoint");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::ActiveBorderContainsPoint()\n");
#endif
   if ( _fActiveBorderShape == kODNULL )
       return kODFalse;

   SOM_TRY
       ODPoint bPoint = BiasPointSet(ev, point, biasCanvas);

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_UNIX_ || defined _PLATFORM_MACINTOSH_)
       // transform point to window coordinates
       TempODTransform winToFrame = somSelf->AcquireWindowFrameTransform(ev, kODNULL);
#endif // MACINTOSH or OS2

#ifdef _PLATFORM_WIN32_
       TempODTransform winToFrame = ODCopyAndRelease(ev, somSelf->AcquireWindowFrameTransform(ev, kODNULL));
       ODFacet* parentFacet = somSelf->GetContainingFacet(ev);
       TempODTransform conXform= parentFacet->AcquireWindowFrameTransform(ev, kODNULL);

       ODPoint offset(0,0);
       TempODShape FrameShape = ODCopyAndRelease(ev,  _fFrame->AcquireFrameShape(ev, kODNULL));
       GetFacetPosition(ev, conXform, FrameShape, offset);

       winToFrame->MoveBy(ev, &offset);
#endif  // WIN32

       ODPoint framePoint = bPoint;
       winToFrame->TransformPoint(ev, &framePoint);

       return _fActiveBorderShape->ContainsPoint(ev, &framePoint);
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODFalse;
}

//------------------------------------------------------------------------------
// ODFacet: IsSelected
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFacetIsSelected(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","IsSelected");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::IsSelected()\n");
#endif
   return _fIsSelected;
}

//------------------------------------------------------------------------------
// ODFacet: SetSelected
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetSetSelected(ODFacet *somSelf, Environment *ev,
       ODBoolean isSelected)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","SetSelected");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::SetSelected()\n");
#endif
   _fIsSelected = isSelected;
}

//------------------------------------------------------------------------------
// ODFacet: GetHighlight
//------------------------------------------------------------------------------

SOM_Scope ODHighlight  SOMLINK ODFacetGetHighlight(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","GetHighlight");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetHighlight()\n");
#endif
   return _fHighlight;
}

//------------------------------------------------------------------------------
// ODFacet: ChangeHighlight
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetChangeHighlight(ODFacet *somSelf, Environment *ev,
       ODHighlight highlight)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","ChangeHighlight");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::ChangeHighlight()\n");
#endif
   SOM_TRY
       _fHighlight = highlight;
       TempODPart tempPart = _fFrame->AcquirePart(ev);
       tempPart->HighlightChanged(ev, somSelf);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFacet: GetNode
//------------------------------------------------------------------------------

SOM_Scope FacetNode*  SOMLINK ODFacetGetNode(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","GetNode");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetNode()\n");
#endif
   return _fNode;
}

//------------------------------------------------------------------------------
// ODFacet: NeedsUpdate
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFacetNeedsUpdate(ODFacet *somSelf, Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","NeedsUpdate");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::NeedsUpdate()\n");
#endif
   return _fNeedsUpdate;
}

//------------------------------------------------------------------------------
// ODFacet: SetNeedsUpdate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetSetNeedsUpdate(ODFacet *somSelf, Environment *ev,
       ODBoolean needsUpdate)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","SetNeedsUpdate");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::SetNeedsUpdate()\n");
#endif
   _fNeedsUpdate = needsUpdate;
}

#ifdef _PLATFORM_OS2_

//------------------------------------------------------------------------------
// ODFacet: SetHandleMouseEvents
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFacetSetHandleMouseEvents(ODFacet *somSelf,
                                                    Environment *ev,
                                                    ODULong flags)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","SetHandleMouseEvents");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::SetHandleMouseEvents()\n");
#endif
    _fPartHandlesMouseEvents = flags;
}

//------------------------------------------------------------------------------
// ODFacet: GetHandleMouseEvents
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODFacetGetHandleMouseEvents(ODFacet *somSelf,
                                                       Environment *ev)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);
    ODFacetMethodDebug("ODFacet","GetHandleMouseEvents");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacet::GetHandleMouseEvents()\n");
#endif
    return _fPartHandlesMouseEvents;
}

SOM_Scope ODFacet*  SOMLINK M_ODFacetclsGetFacetFromHWND(M_ODFacet *somSelf, Environment *ev,
    HWND hwnd)
{
    M_ODFacetMethodDebug("M_ODFacet","clsODFacetFromHWND");

#ifdef _FACET_DEBUG_
    qprintf("Executing M_ODFacet::clsODFacetFromHWND()\n");
#endif
    CHAR  achBuffer[sizeof(szFacetWinClassName)+1];
    ODFacet* facet;

    LONG lRetLen = WinQueryClassName(hwnd, sizeof(achBuffer), (PCH)achBuffer);
    if (lRetLen == strlen(pszFacetWinClass) &&
        !strcmp(achBuffer, pszFacetWinClass)) {

      facet = (ODFacet*)WinQueryWindowPtr(hwnd, QWL_USER);
    } else {
      facet = kODNULL;
    }
    return facet;
}




//------------------------------------------------------------------------------
// Facet window procedure
//------------------------------------------------------------------------------
MRESULT EXPENTRY fnwpFacetWindow ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   ODBoolean handled = kODFalse;
   ODEventData event;
   ODEventInfo eventInfo;
   Environment* ev = somGetGlobalEnvironment();
   FacetWndData* pData;
   ULONG cx, cy;
   CHAR  achBuffer[sizeof(szFacetWinClassName)+1];
   HWND hwndChild;
   LONG lRetLen;

   MRESULT result = 0;

   switch (msg) {

   case WM_CREATE:
      {
        handled = kODTrue;
        FacetWndCtlData* pCtlData = (FacetWndCtlData*)mp1;

        // Do some sanity checking
        if (!(pCtlData->self && somIsObj(pCtlData->self) &&
              pCtlData->self->somIsA(_ODFacet)))
        {
          result = (MRESULT)kODTrue;    // Creation failed
          break;
        }
        pData = new FacetWndData;
        pData->facet = pCtlData->self;

        TRY
          pData->frame = pData->facet->GetFrame(ev);
          pData->part = pData->frame->AcquirePart(ev);
          HWND hwndFrame = pData->facet->GetWindow(ev)->GetPlatformWindow(ev);
          pData->hwndClient = WinWindowFromID(hwndFrame, FID_CLIENT);
        CATCH_ALL
          result = (MRESULT)kODTrue;    // Creation failed
          delete pData;
          break;
        ENDTRY
        WinSetWindowPtr(hwnd, QWL_USER,                   (PVOID)pData->facet); // For ref by others
        WinSetWindowPtr(hwnd, QWP_FACETWINDOWDATA,        (PVOID)pData);
        WinSetWindowPtr(hwnd, QWL_HITTESTFACET,           0);                   // For comm with std disp mod
        WinSetWindowPtr(hwnd, QWL_PARTHANDLESMOUSEEVENTS, 0);
        WinSetWindowPtr(hwnd, QWL_FACETWINDOWRGN,         0);
        break;
      }

   case WM_DESTROY:
     {
      pData = (FacetWndData*)WinQueryWindowPtr(hwnd, QWP_FACETWINDOWDATA);
      ODReleaseObject( ev, pData->part );
      delete pData;
      handled = kODTrue;
      break;
     }

   case WM_HITTEST:
     {
      if (WinQueryWindowULong(hwnd, QWL_HITTESTFACET))
         return (MRESULT)HT_NORMAL;
      else
         return (MRESULT)HT_TRANSPARENT;
     }
     break;

   case WM_PAINT:
      {
       WinEndPaint(WinBeginPaint(hwnd, 0, 0));
       handled = kODTrue;
       break;
      }

   case WM_CHAR:
     {
      pData = (FacetWndData*)WinQueryWindowPtr(hwnd, QWP_FACETWINDOWDATA);
      return WinSendMsg(pData->hwndClient, msg, mp1, mp2);
     }

   case WM_ADJUSTWINDOWPOS:
   case WM_WINDOWPOSCHANGED:
   case WM_SHOW:
   case WM_CALCVALIDRECTS:
   case WM_SIZE:
   case WM_MOUSEMOVE:
      break;

   default:
      pData = (FacetWndData*)WinQueryWindowPtr(hwnd, QWP_FACETWINDOWDATA);
      {
         event.hwnd  = hwnd;
         event.msg   = msg;
         event.mp1   = mp1;
         event.mp2   = mp2;

         TRY
            handled = pData->part->HandleEvent(ev, &event, pData->frame,
                                                           pData->facet,
                                                           kODNULL);
         CATCH_ALL
            handled = kODFalse;

//#ifdef _UNICODE_
//            WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
//                           OD_OPENDOCEXCEPTION, sizeof(szMsg1), szMsg1);
//            char fmtstring[Str255];
//            strcpy(fmtstring, szMsg1);
//            int bufsize = strlen(fmtstring) + strlen(ErrorMessage()) + sizeof(__FUNCTION__) + 11;
//#else
            static const char fmtstring[] = "OpenDoc Exception %d caught in %s\n\n%s";
            int bufsize = sizeof(fmtstring) + strlen(ErrorMessage()) + sizeof(__FUNCTION__) + 11;
//#endif  // _UNICODE
            char* buffer = new char[bufsize];
            if (buffer)
               sprintf(buffer, fmtstring, ErrorCode(), __FUNCTION__, ErrorMessage());
            WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, buffer ? buffer : "Out of memory!!!", "Error",
                          0, MB_OK | MB_ERROR | MB_APPLMODAL | MB_MOVEABLE);
            delete[] buffer;
            WinPostMsg(hwnd, WM_QUIT, 0, 0);
         ENDTRY
      }
      break;
   }
   if (handled)
      return result;
   else
      return WinDefWindowProc(hwnd, msg, mp1, mp2);
}
#endif   // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
SOM_Scope ODFacet*  SOMLINK M_ODFacetclsGetFacetFromHWND(M_ODFacet *somSelf, Environment *ev,
                                                         HWND hwnd)
{
    M_ODFacetMethodDebug("M_ODFacet","clsODFacetFromHWND");

#ifdef _FACET_DEBUG_
    qprintf("Executing M_ODFacet::clsODFacetFromHWND()\n");
#endif
    CHAR  achBuffer[sizeof(szFacetWinClassName)+1];
    ODFacet* facet;

    int lRetLen = GetClassName(hwnd, (LPSTR)achBuffer, sizeof(achBuffer));
    if (lRetLen == strlen(pszFacetWinClass) &&
        !strcmp(achBuffer, pszFacetWinClass)) {

        facet = (ODFacet*)GetWindowLong(hwnd, QWL_FACETPTR);
    } else {
      facet = kODNULL;
    }
    return facet;
}

//------------------------------------------------------------------------------
// Facet window procedure
//------------------------------------------------------------------------------
LRESULT CALLBACK fnwpFacetWindow ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   ODBoolean handled = kODFalse;
   ODEventData event;
   ODEventInfo eventInfo;
   Environment* ev = somGetGlobalEnvironment();
   FacetWndData* pData;
   ULONG cx, cy;
   CHAR  achBuffer[sizeof(szFacetWinClassName)+1];
   HWND hwndChild;
   LONG lRetLen;

   LRESULT result = 0;

   switch (msg) {

   // Don't send to part's HandleEvent() method.

   case WM_NCCREATE:                 // Before WM_CREATE
   case WM_NCCALCSIZE:               // Before WM_CREATE
   case WM_NCDESTROY:                // After WM_DESTROY
   case WM_WINDOWPOSCHANGING:
   case WM_WINDOWPOSCHANGED:
   case WM_SHOWWINDOW:
   case WM_SIZE:
   case WM_MOUSEMOVE:
        break;

   case WM_CREATE:
      {
        handled = kODTrue;
        FacetWndCtlData* pCtlData = (FacetWndCtlData*)(((LPCREATESTRUCT) lParam)->lpCreateParams);

        // Do some sanity checking
        if (!(pCtlData->self && somIsObj(pCtlData->self) &&
              pCtlData->self->somIsA(_ODFacet)))
        {
          result = kODTrue;    // Creation failed
          break;
        }
        pData = new FacetWndData;
        pData->facet = pCtlData->self;

        TRY
          pData->frame = pData->facet->GetFrame(ev);
          pData->part = pData->frame->AcquirePart(ev);
        CATCH_ALL
          result = kODTrue;    // Creation failed
          delete pData;
          break;
        ENDTRY
        SetWindowLong(hwnd, QWL_FACETPTR,               (LONG) pData->facet); // For ref by others
        SetWindowLong(hwnd, QWP_FACETWINDOWDATA,        (LONG) pData);
        SetWindowLong(hwnd, QWL_HITTESTFACET,           0L);  // For comm with std disp mod
        SetWindowLong(hwnd, QWL_PARTHANDLESMOUSEEVENTS, 0L);
        SetWindowLong(hwnd, QWL_FACETWINDOWRGN,         0L);  // not used on NT
       }
       break;
   case WM_DESTROY:
      pData = (FacetWndData*)GetWindowLong(hwnd, QWP_FACETWINDOWDATA);
      ODReleaseObject( ev, pData->part );
      delete pData;
      handled = kODTrue;
      break;


   case WM_PAINT:
      {
         RECT uRect;

         if(GetUpdateRect(hwnd, &uRect, FALSE))
         {
           HWND parenthwnd;
           pData = (FacetWndData*)GetWindowLong(hwnd, QWP_FACETWINDOWDATA);

           if(pData->frame->IsRoot(ev))
           {
             parenthwnd = GetParent(hwnd);
           }
           else
           {
             ODWindow* window = pData->frame->AcquireWindow(ev);
             if(window)
             {
               window->Release(ev);
               parenthwnd = window->GetPlatformWindow(ev);
               MapWindowPoints(hwnd, parenthwnd, (LPPOINT)&uRect, 2);
             }
           }

           InvalidateRect(parenthwnd, &uRect, FALSE);
           PAINTSTRUCT paintStruct;
           BeginPaint(hwnd, &paintStruct);
           EndPaint(hwnd, &paintStruct);

           handled = kODTrue;
         }
      }
      break;

  case WM_ERASEBKGND:
        return (LRESULT)TRUE;

  case WM_NCHITTEST:
     {
  // In Windows, even though parent window is transparent
  // controls like PUSHBUTTON still receive the events.
  //    if (GetWindowLong(hwnd, QWL_HITTESTFACET))
  //        return (LRESULT)HTCLIENT;
  //    else
            return (LRESULT)HTTRANSPARENT;
      }
      break;

   // This is done to ensure that the message goes through the Dispatcher
   // so that the part that owns the keyboard focus gets it.
   case WM_CHAR:
      pData = (FacetWndData*)GetWindowLong(hwnd, QWP_FACETWINDOWDATA);
      return SendMessage(pData->hwndClient, msg, wParam, lParam);

   default:
      // send the message to part's HandleEvent() method.
      pData = (FacetWndData*)GetWindowLong(hwnd, QWP_FACETWINDOWDATA);
      {
         event.hwnd    = hwnd;
         event.message = msg;
         event.wParam  = wParam;
         event.lParam  = lParam;
         TRY
            handled = pData->part->HandleEvent(ev, &event, pData->frame,
                                                           pData->facet,
                                                           kODNULL);
         CATCH_ALL
            handled = kODFalse;
            ASSERT(handled, ErrorCode() );

         ENDTRY
      }
      break;
   }
   if (handled)
      return result;
   else
      return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif // _PLATFORM_WIN32_
