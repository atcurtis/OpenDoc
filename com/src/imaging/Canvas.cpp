/* @(#)Z 1.7 com/src/imaging/Canvas.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:03 (96/10/29 09:24:55) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odimaging
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

//********************************************************************/
/*  Licensed Materials - Property of IBM                            */
/*                                                                  */
/*                                                                  */
/* Copyright (C) International Business Machines Corp., 1994.       */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/*  US Government Users Restricted Rights -                         */
/*  Use, duplication, or disclosure restricted                      */
/*  by GSA ADP Schedule Contract with IBM Corp.                     */
/*                                                                  */
/*  IBM Change History (most recent first):                         */
/*          5/15/96 jab  Corrections for OS/2 port.                 */
/*          3/11/96 jab  Merge with Apple DR4.                      */
/*                       Changes for cross-platform support.        */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*  <OS2>   8/27/94 MAP  Port A8C3                                  */
/*                                                                  */
/********************************************************************/
/*
    File:        Canvas.cpp

    Contains:    ODCanvas, a Macintosh drawing environment.

    Owned by:    Jens Alfke
    Owned by:    Jens Alfke, Joshua Susser

    Copyright:    © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

    Change History (most recent first):

         <9>     9/18/95    JBS        1285042 don't use QD grafport calls for
                                    in/validation when printing
         <8>      8/7/95    JBS        1275083 added Invalidate & Validate (moved
                                    from ODFacet)
         <7>     5/25/95    jpa        Use new GX headers [1241078, 1253324]
         <6>     3/20/95    jpa        Create gxViewPort when asked for it
                                    [1218787]. SOM_CATCH --> SOM_TRY [1224355].
         <5>     12/5/94    jpa        Fixes from code review. [1203923]
         <4>     9/29/94    RA        1189812: Mods for 68K build.
         <3>     8/17/94    jpa        Function prefixes. [1181512]
         <2>      8/8/94    jpa        API changes for multiformat canvases & bias
                                    transforms [1179683, 1178688]
         <1>     6/15/94    jpa        first checked in

    To Do:
    In Progress:
*/


#define ODCanvas_Class_Source
#define VARIABLE_MACROS
#include <Canvas.xih>

#include <PlatCanv.xih>

#ifdef _PLATFORM_OS2_
#define INCL_WINWINDOWMGR
#include <os2.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _REALSHPE_
#include "RealShpe.h"    /* For gGX */
#endif
#endif  // _PLATFORM_MACINTOSH_

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __GXENVIRONMENT__
#include <GXEnvironment.h>
#endif
#endif  // _PLATFORM_MACINTOSH_



//------------------------------------------------------------------------------
// CreatePlatformCanvas
//------------------------------------------------------------------------------

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
SOM_Scope ODPlatformCanvas* SOMLINK ODCanvasCreatePlatformCanvas(ODCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g,
	ODBoolean isDynamic,
	ODBoolean isOffScreen )
{
  ODCanvasData *somThis = ODCanvasGetData(somSelf);
  ODCanvasMethodDebug("ODCanvas","CreatePlatformCanvas");

  ODPlatformCanvas* newPC;
  ODPlatformCanvas *c = new ODPlatformCanvas;

  SOM_TRY

    newPC = c->CreateConcreteCanvas( ev, g, isDynamic, isOffScreen );
    delete c;
    return newPC;

  SOM_CATCH_ALL
    delete c;
    return kODNULL;
  SOM_ENDTRY

}

#endif  // _PLATFORM_WIN32_


//------------------------------------------------------------------------------
// InitCanvas
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODCanvasInitCanvas(ODCanvas *somSelf, Environment *ev,
        ODGraphicsSystem graphicsSystem,
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        ODPlatformCanvas *platformCanvas,
#else
        ODPlatformCanvas platformCanvas,
#endif
        ODBoolean isDynamic,
        ODBoolean isOffscreen)
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","InitCanvas");
    
    somSelf->InitBaseCanvas( ev, isDynamic, isOffscreen );

    // 
    // OS/2 Specific code
    //
#ifdef _PLATFORM_OS2_

    // OS/2 Specific code
    if ( graphicsSystem == kODPM )
       _fPlatformCanvas = platformCanvas;
    else
       ODSetSOMException( ev, kODErrInvalidGraphicsSystem );

#endif  // _PLATFORM_OS2_

 
    // 
    // Windows Specific code
    //
#ifdef _PLATFORM_WIN32_

    if ( (graphicsSystem == kODWin32) || (graphicsSystem == kODWin95) || (graphicsSystem == kODWinNT) ) 
       _fPlatformCanvas = platformCanvas;
    else
       ODSetSOMException( ev, kODErrInvalidGraphicsSystem );

#endif  // _PLATFORM_WIN32_
 
    // 
    // AIX Specific code
    //
#ifdef _PLATFORM_AIX_

    if ( graphicsSystem == kODAIX )
       _fPlatformCanvas = platformCanvas;
    else
       ODSetSOMException( ev, kODErrInvalidGraphicsSystem );

#endif  // _PLATFORM_AIX_
 
    // 
    // Macintosh Specific code
    //
#ifdef _PLATFORM_MACINTOSH_

    if( graphicsSystem == kODQuickDraw )
        _fQDCanvas= platformCanvas;
    else if( graphicsSystem == kODQuickDrawGX )
        _fGXCanvas = platformCanvas;
    else
        ODSetSOMException(ev, kODErrInvalidGraphicsSystem);

#endif  // _PLATFORM_MACINTOSH_

}


//------------------------------------------------------------------------------
// HasPlatformCanvas
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODCanvasHasPlatformCanvas(ODCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g)
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","HasPlatformCanvas");


    // 
    // OS/2 Specific code
    //
#ifdef _PLATFORM_OS2_

    if( g == kODPM )
       return ( _fPlatformCanvas != kODNULL );
    else
       return kODFalse;

#endif  // _PLATFORM_OS2_


    // 
    // Windows Specific code
    //
#ifdef _PLATFORM_WIN32_

    if ( (g == kODWin32) || (g == kODWin95) || (g == kODWinNT) ) 
       return ( _fPlatformCanvas != kODNULL );
    else
       return kODFalse;

#endif  // _PLATFORM_WIN32


    // 
    // AIX Specific code
    //
#ifdef _PLATFORM_AIX_

    if (g == kODAIX)
       return ( _fPlatformCanvas != kODNULL );
    else
       return kODFalse;

#endif  // _PLATFORM_AIX


    // 
    // Macintosh Specific code
    //
#ifdef _PLATFORM_MACINTOSH_

    if( g == kODQuickDraw )
        return _fQDCanvas != kODNULL;
    else if( g == kODQuickDrawGX )
        return _fGXCanvas != kODNULL || (gGX && _fQDCanvas != kODNULL);
    else
        return kODFalse;

#endif  // _PLATFORM_MACINTOSH_

}

//------------------------------------------------------------------------------
// GetPlatformCanvas
//------------------------------------------------------------------------------

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
SOM_Scope ODPlatformCanvas* SOMLINK ODCanvasGetPlatformCanvas(ODCanvas *somSelf, Environment *ev,
#else
SOM_Scope ODPlatformCanvas  SOMLINK ODCanvasGetPlatformCanvas(ODCanvas *somSelf, Environment *ev,
#endif
        ODGraphicsSystem g)
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","GetPlatformCanvas");


    // 
    // OS/2 Specific code
    //
#ifdef _PLATFORM_OS2_

    ODPlatformCanvas* c;
    if( g == kODPM )  {
       c = _fPlatformCanvas;
       return c;
    }
    else
       c = kODNULL;

#endif // _PLATFORM_OS2_



    // 
    // Windows Specific code
    //
#ifdef _PLATFORM_WIN32_

    ODPlatformCanvas* c;
    if ( (g == kODWin32) || (g == kODWin95) || (g == kODWinNT) )  {
       c = _fPlatformCanvas;
       return c;
    }
    else
       c = kODNULL;

#endif // _PLATFORM_WIN32_



    // 
    // AIX Specific code
    //
#ifdef _PLATFORM_AIX_

    ODPlatformCanvas* c;
    if ( g == kODAIX ) {
       c = _fPlatformCanvas;
       return c;
    }
    else
       c = kODNULL;

#endif // _PLATFORM_AIX_



    // 
    // Macintosh Specific code
    //
#ifdef _PLATFORM_MACINTOSH_

    ODPlatformCanvas c;
    if( g == kODQuickDraw )
        c= _fQDCanvas;
    else if( g == kODQuickDrawGX ) {
        if( !_fGXCanvas ) {
            (void) ODCanvas__GetGXViewport(somSelf,ev);
            if( ev->_major )
                return kODNULL;
        }
        c = _fGXCanvas;
    } else
        c = kODNULL;

#endif  // _PLATFORM_MACINTOSH_

    
    if( c == kODNULL )
        ODSetSOMException( ev, kODErrInvalidGraphicsSystem );
    return c;

}


//------------------------------------------------------------------------------
// SetPlatformCanvas
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODCanvasSetPlatformCanvas(ODCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g,
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        ODPlatformCanvas* c)
#else
        ODPlatformCanvas c)
#endif
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","SetPlatformCanvas");



    // 
    // OS/2 Specific code
    //
#ifdef _PLATFORM_OS2_

    if ( g == kODPM )
      _fPlatformCanvas = c;
    else
      ODSetSOMException( ev, kODErrInvalidGraphicsSystem );

#endif // _PLATFORM_OS2_


    // 
    // Windows Specific code
    //
#ifdef _PLATFORM_WIN32_

    if ( (g == kODWin32) || (g == kODWin95) || (g == kODWinNT) ) 
      _fPlatformCanvas = c;
    else
      ODSetSOMException( ev, kODErrInvalidGraphicsSystem );

#endif // _PLATFORM_WIN32_



    // 
    // AIX Specific code
    //
#ifdef _PLATFORM_AIX_

    if ( g == kODAIX )
      _fPlatformCanvas = c;
    else
      ODSetSOMException( ev, kODErrInvalidGraphicsSystem );

#endif // _PLATFORM_AIX_


    // 
    // Macintosh Specific code
    //
#ifdef _PLATFORM_MACINTOSH_

    if( g == kODQuickDraw )
        _fQDCanvas= c;
    else if( g == kODQuickDrawGX )
        _fGXCanvas = c;
    else
        ODSetSOMException(ev, kODErrInvalidGraphicsSystem);

#endif  // _PLATFORM_MACINTOSH_

}


// 
// Macintosh Specific methods
//
#ifdef _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
// GetQDPort
//------------------------------------------------------------------------------

SOM_Scope GrafPtr  SOMLINK ODCanvas__GetQDPort(ODCanvas *somSelf, Environment *ev)
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","GetQDPort");

    if( !_fQDCanvas )
        ODSetSOMException(ev, kODErrInvalidGraphicsSystem);
    return (GrafPtr)_fQDCanvas;
}


//------------------------------------------------------------------------------
// GetGXViewport
//------------------------------------------------------------------------------

SOM_Scope ODPlatformCanvas  SOMLINK ODCanvas__GetGXViewport(ODCanvas *somSelf, Environment *ev)
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","GetGXViewport");

    if( !_fGXCanvas ) {
        SOM_TRY
            if( !gGX || !_fQDCanvas )
                THROW(kODErrInvalidGraphicsSystem);
            else if( somSelf->IsOffscreen(ev) ) {
                THROW(kODErrNotImplemented);    // $$$$$
            } else {
                ClearGXError();
                gxViewPort v = GXGetWindowViewPort((WindowPtr)_fQDCanvas);
                if( !v )
                    v = GXNewWindowViewPort((WindowPtr)_fQDCanvas);
                ThrowIfFirstGXError();
                _fGXCanvas = (ODPlatformCanvas) v;
            }
        SOM_CATCH_ALL
        SOM_ENDTRY
    }
    return _fGXCanvas;
}


//
// jab 03/13/96
//
// In the Apple DR4 drop, the ODCanvas overrode the Invalidate and 
// Validate methods though it's not very clear as to why.  The OS/2 
// implementation of ODCanvas from 02/13/96 did not override these methods.
// At this time, it appears as though the overridding of these methods
// in ODCanvas is necessary in the Mac implementation, but may not be 
// necessary in other implementations.  We can always override these
// methods, if necessary, on a platform by platform basis.  We'll have to
// change the IDL for ODCanvas to not override these methods and simply
// use the parent's implementation.
//


//------------------------------------------------------------------------------
// Invalidate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODCanvas__Invalidate(ODCanvas *somSelf, Environment *ev,
        ODShape* shape)
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","Invalidate");

    SOM_TRY

    if ( somSelf->IsOffscreen(ev) )
        parent_Invalidate(somSelf, ev, shape);
    else if ( _fQDCanvas != kODNULL
            && !somSelf->HasPlatformPrintJob(ev, kODQuickDraw) )
    {
        GrafPtr savedPort;
        GrafPtr windowPort = (GrafPtr)_fQDCanvas;
        GetPort(&savedPort);
        SetPort(windowPort);
        short oldLeft = windowPort->portRect.left;
        short oldTop = windowPort->portRect.top;
        SetOrigin(0, 0);
        InvalRgn(shape->GetQDRegion(ev));
        SetOrigin(oldLeft, oldTop);
        SetPort(savedPort);
    }
        
    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// Validate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODCanvas__Validate(ODCanvas *somSelf, Environment *ev,
        ODShape* shape)
{
    ODCanvasData *somThis = ODCanvasGetData(somSelf);
    ODCanvasMethodDebug("ODCanvas","Validate");

    SOM_TRY

    if ( somSelf->IsOffscreen(ev) )
        parent_Validate(somSelf, ev, shape);
    else if ( _fQDCanvas != kODNULL
            && !somSelf->HasPlatformPrintJob(ev, kODQuickDraw) )
    {
        GrafPtr savedPort;
        GrafPtr windowPort = (GrafPtr)_fQDCanvas;
        GetPort(&savedPort);
        SetPort(windowPort);
        short oldLeft = windowPort->portRect.left;
        short oldTop = windowPort->portRect.top;
        SetOrigin(0, 0);
        ValidRgn(shape->GetQDRegion(ev));
        SetOrigin(oldLeft, oldTop);
        SetPort(savedPort);
    }
        
    SOM_CATCH_ALL
    SOM_ENDTRY
}

#endif  // _PLATFORM_MACINTOSH

