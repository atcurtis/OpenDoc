/* @(#)Z 1.15 os2/src/pubutils/src/FocusLib.cpp, odpubutils, od96os2, odos29646d 96/11/15 15:49:46 (96/10/22 13:56:05) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odpubutils
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

//
//

#ifdef _PLATFORM_OS2_
// We have a platform specific version here.

#pragma import ( DrgGetClipPS,"DrgGetClipPS","PMCtls",108)

//#ifndef _ODUTILS_
//#include <ODUtils.h>
//#endif

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#ifndef _FOCUSLIB_
#include <FocusLib.h>
#endif

#ifndef SOM_ODShape_xh
#include "Shape.xh"
#endif

#ifndef SOM_ODPlatformCanvas_xh
#include "PlatCanv.xh"
#endif

#ifndef SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef SOM_ODFacet_xh
#include "Facet.xh"
#endif

#ifndef SOM_ODFrame_xh
#include "Frame.xh"
#endif

#ifndef SOM_ODWindow_xh
#include "Window.xh"
#endif

#ifndef SOM_OS2PCanv_xh
#include "OS2PCanv.xh"
#endif

#ifndef SOM_OS2WCanv_xh
#include "OS2WCanv.xh"
#endif

#ifndef SOM_ODCanvas_xh
#include "Canvas.xh"
#endif

HPS APIENTRY DrgGetClipPS(HWND hwnd,HWND hwndClip,ULONG fl);  // Not yet in PMWIN.H


//------------------------------------------------------------------------------
// FocusState::BeginFocus
//------------------------------------------------------------------------------
void
FocusState::BeginFocus(ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
                    ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS)
{

  // Remember toWindow
  fToWindow = toWindow;
  MATRIXLF mtx;

  // Remember the facet
  fFacet          = facet;
  fclipShape      = NULL;
  Environment *ev = somGetGlobalEnvironment();
  fEv         = ev;
  ODCanvas* canvas = facet->GetCanvas(ev);

  // Get the HDC
  if (toWindow)
  {
    fHWND = WinWindowFromID((HWND)( facet->GetWindow(ev)->GetPlatformWindow(ev) ), FID_CLIENT);
    ULONG ulClipFlags = 0, ulStyle;
    ulStyle = WinQueryWindowULong(fHWND, QWL_STYLE);
    if (ulStyle & WS_CLIPCHILDREN) ulClipFlags |= PSF_CLIPCHILDREN;
    if (ulStyle & WS_CLIPSIBLINGS) ulClipFlags |= PSF_CLIPSIBLINGS;
    if (ulStyle & WS_PARENTCLIP)   ulClipFlags |= PSF_PARENTCLIP;
    if (!fDragPS)
    {
      if (lockedWindowUpdate)     ulClipFlags |= PSF_LOCKWINDOWUPDATE;
      fHPS = WinGetClipPS(fHWND, 0, ulClipFlags);
    }
    else
    {
      ulClipFlags |= PSF_LOCKWINDOWUPDATE;
      fHPS = DrgGetClipPS(fHWND, 0, ulClipFlags);
    }
  }
  else
  {
    ODPlatformCanvas *tmpCanvas = facet->GetCanvas(ev)->GetPlatformCanvas(ev, kODPM);
    fHPS = ( (ODOS2Canvas*)tmpCanvas )->GetPS(ev);
  }


  // Find out if we're a Metafile DC
  ODBoolean isMetafileDC = 0;

  // Are we Printing?
  ODBoolean isPrinting = !(facet->GetCanvas(ev)->IsDynamic(ev));

  // Clip to it (don't clip if to a Window)
  // Don't clip to Metafiles because it's basically useless
  if (!toWindow && !isMetafileDC)
  {
    fclipShape = facet->CreateShape(ev);
    if (fclipShape)
    {
      // When drawing to a window, the clip shape is the invalid shape
      // which was passed in. 
      if (!isPrinting) 
      {
        if (invalShape)
        {
          fclipShape->CopyFrom(ev, invalShape);
        }
        else
        {
          ODShape *cShape = facet->AcquireClipShape(ev, kODNULL);
          fclipShape->CopyFrom( ev, cShape);
          cShape->Release( ev );
        }
      }
      else 
      {
        // In the printing case, the shape we clip to is the determined by
        // the aggregate clip shape intersected with the frame's used
        // shape, then intersected with the invalid shape which was passed
        // in.
        ODShape *aggshape = facet->AcquireAggregateClipShape(ev, kODNULL);
        ODShape *usedshape = facet->GetFrame(ev)->AcquireUsedShape(ev, kODNULL);
        fclipShape->CopyFrom( ev, aggshape );
        fclipShape->Intersect( ev, usedshape );
	if (invalShape)
	        fclipShape->Intersect(ev, invalShape);

        aggshape->Release( ev );
        usedshape->Release( ev );
      }

      // When drawing to a window, transform using the facet's window 
      // frame transform.  This should convert the shape to window 
      // coordinates for the docshell window.
      ODTransform *frameTransform;
      if( !isPrinting ) 
      {
        frameTransform = facet->AcquireWindowFrameTransform(ev, kODNULL);
        fclipShape->Transform(ev, frameTransform);
      }
      else 
      {
        // For printing, transform using the facet's frame transform. The
        // facet's window frame transform wouldn't work in this case. 
        frameTransform = facet->AcquireFrameTransform(ev, kODNULL);
        fclipShape->Transform(ev, frameTransform);
      }
	
      HRGN clip;
      
      // The following lines were added so parts are clipped properly when 
      // rotated. The GpiOffsetRegion() call below was also added and some 
      // lines were deleted.  If you use GetOffset(), as was previously used, 
      // it takes into account post-scale but pre-rotation.
      // GetBoundingBox() is taking into account post-scale & post-rotation.
      if( !isPrinting ) 
      {
        ODShape *frameShape = facet->GetFrame(ev)->AcquireFrameShape(ev, kODNULL);
        ODShape *frameShape2 = frameShape->Copy(ev);
        frameShape->Release(ev);
        frameShape2->Transform(ev, frameTransform);
        ODRect RectBounds;
        frameShape2->GetBoundingBox(ev, &RectBounds);
        frameShape2->Release(ev);
        RECTL rl;
        RectBounds.AsRECTL(rl);
        POINTL pt;
        pt.x = -rl.xLeft;
        pt.y = -rl.yBottom;

        // Get the region from the clipshape
        clip = fclipShape->GetRegion(ev);
        GpiOffsetRegion(fHPS, clip, &pt);
      }
      else 
      {
        clip = fclipShape->GetRegion(ev);
      }

      ODTransform* xform1 = facet->AcquireContentTransform(ev, canvas);
      xform1->GetMATRIXLF(ev, &mtx);
      xform1->Release(ev);

      GpiSavePS(fHPS);
      GpiResetPS(fHPS, GRES_ATTRS);
      GpiSetDefaultViewMatrix(fHPS, 9, &mtx, TRANSFORM_REPLACE);

      // If a region was retrieved
      if (clip)
      {
        if (!isPrinting) 
        { 
          HRGN mainRgn = facet->CopyAggregateClipRegion(ev, fHPS);  // Fast PM Clipping
          GpiCombineRegion(fHPS, clip, clip, mainRgn, CRGN_AND );
          GpiDestroyRegion(fHPS, mainRgn);
        }

        HRGN hrgnOld;
        GpiSetClipRegion(fHPS, clip, &hrgnOld);
        GpiDestroyRegion(fHPS, hrgnOld);
      }

      frameTransform->Release(ev);
    }
  }
}

//------------------------------------------------------------------------------
// FocusState::EndFocus
//------------------------------------------------------------------------------

void
FocusState::EndFocus( )
{
  // Only got fclipShape if (!toWindow && !isMetafileDC && !isPrinting)
  if (fclipShape)
  {
    fclipShape->Release(fEv);
  }

  GpiRestorePS(fHPS, -1);

  // Release the PS
  if (fToWindow)
  {
    if (fDragPS)
      DrgReleasePS(fHPS);
    else
      WinReleasePS(fHPS);
  } 
  else
  {
    ODPlatformCanvas *theCanv = fFacet->GetCanvas(fEv)->GetPlatformCanvas(fEv, kODPM);
    ( (ODOS2Canvas*)theCanv )->ReleasePS(fEv);
  }
}

//------------------------------------------------------------------------------
// CFocus
//------------------------------------------------------------------------------

CFocus::CFocus(ODFacet *facet, ODShape* invalShape, HPS *theHPS )
{
  f.BeginFocus(facet, invalShape, kODTrue, kODFalse, kODFalse, kODFalse);
  *theHPS = f.fHPS;
}

CFocus::CFocus(ODFacet *facet, ODShape* invalShape, HPS *theHPS, HRGN *theClipRgn)
{
  f.BeginFocus(facet, invalShape, kODFalse, kODFalse, kODFalse, kODFalse);
  *theHPS = f.fHPS;
  *theClipRgn = f.fClipRgn;
}

CFocus::~CFocus( )
{
  try
  {
    f.EndFocus();
  }
  catch (ODException _exception)
  {
  }
}

CFocusWindow::CFocusWindow(ODFacet *facet, ODShape* invalShape,
                            HPS *theHPS, HWND *theHWND, TypePS typePS)
{
  f.BeginFocus(facet, invalShape, kODTrue, kODTrue,
                        (typePS == LockedWindowUpdatePS), (typePS == DragPS));
  *theHPS = f.fHPS;
  if (theHWND)
    *theHWND = f.fHWND;
}

CFocusWindow::CFocusWindow(ODFacet *facet, ODShape* invalShape,
                            HPS *theHPS, HWND *theHWND, TypePS typePS, HRGN *theClipRgn)
{
  f.BeginFocus(facet, invalShape, kODFalse, kODTrue,
                        (typePS == LockedWindowUpdatePS), (typePS == DragPS));
  *theHPS = f.fHPS;
  if (theHWND)
    *theHWND = f.fHWND;
  *theClipRgn = f.fClipRgn;
}

CFocusWindow::~CFocusWindow( )
{
  try
  {
    f.EndFocus();
  }
  catch (ODException _exception)
  {
  }
}

//------------------------------------------------------------------------------
// BeginFocus and EndFocus
//------------------------------------------------------------------------------
void
BeginFocus(FocusState* foc, ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
            ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS)
{
  foc->BeginFocus(facet, invalShape, setClipRgn,
                   toWindow, lockedWindowUpdate, dragPS);
}

void
EndFocus( FocusState* foc)
{
  foc->EndFocus();
}


#endif // _PLATFORM_OS2_
