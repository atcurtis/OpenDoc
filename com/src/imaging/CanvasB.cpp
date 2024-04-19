/* @(#)Z 1.12 com/src/imaging/CanvasB.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:03 (96/10/29 09:25:00) */
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

/********************************************************************/
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
/*          3/11/96 jab  Changes for cross-platform support.        */
/*                       Eliminated use of TempODShape.             */
/*  120445  5/16/95 ced  Check for NULL _fUpdateShape in            */
/*                       AcquireUpdateShape.                        */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*  119447  4/18/95 ced  Add ODPlatformPrintJob type.               */
/*  <OS2>   8/27/94 MAP  Port A8C3                                  */
/*                                                                  */
/********************************************************************/
/*
    File:        CanvasB.cpp

    Contains:    ODBaseCanvas, an abstract drawing environment.

    Owned by:    Jens Alfke, Joshua Susser

    Copyright:    © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

    Change History (most recent first):

        <17>     8/29/95    jpa        Use TempODShape to avoid checking result of
                                    'new ODShape'. [1279066]
        <16>      8/3/95    RR        #1257260: Collapse B classes. Remove
                                    somInit methods. Don't call IsInitialized
                                    or SubclassResponsibility
        <15>      6/1/95    jpa        Removed CheckUpdateShape. [1232306]
        <14>     5/26/95    RR        #1251403: Multithreading naming support
        <13>      5/4/95    jpa        Fixed bad TRY block in GetOwner. [1246060]
        <12>     3/31/95    jpa        Fixed crash in GetUpdateShape for onscreen
                                    canvases. [1223422]
        <11>     3/22/95    jpa        Use new ODPlatformPrintJob type. [1230236]
        <10>     3/20/95    jpa        SOM_CATCH --> SOM_TRY [1224355]
         <9>     1/13/95    RR        Check for null in GetBiasTransform
         <8>    12/20/94    jpa        GetUpdateShape increments shape's refcount
                                    [1207402]
         <7>     12/5/94    jpa        Fixes from code review. [1203923]
         <6>    10/24/94    jpa        Include Trnsform.xh. Initialize
                                    fUpdateShape. [1153437, 1165562]
         <5>     9/29/94    RA        1189812: Mods for 68K build.
         <4>     8/17/94    jpa        Function prefixes. [1181512]
         <3>      8/8/94    jpa        API changes for multiformat canvases & bias
                                    transforms [1179683, 1178688]
         <2>     7/13/94    jpa        Call InitObject in InitBaseCanvas.
         <1>     6/15/94    jpa        first checked in

    To Do:
    In Progress:
        
*/

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif   // _PAGETUNING_OFF_
#endif

#define ODBaseCanvas_Class_Source
#define VARIABLE_MACROS
#include <CanvasB.xih>

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

// Note: This was commented out in the OS/2 Feb. 13 drop.  It affects code
// in ResetUpdateShape.  In looking at the code in ResetUpdateShape a little
// closer, we (Karalee and I) don't believe the DR4 code which creates a
// TempODShape really works anyway.  Since it's causing warnings to be thrown
// in the odutils test cases and it looks incorrect, I'm taking it out completely by
// wrapping it with _PLATFORM_MACINTOSH_.  jab 05/28/96
#ifdef _PLATFORM_MACINTOSH_
#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif
#endif

//------------------------------------------------------------------------------
// somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvassomUninit(ODBaseCanvas *somSelf)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","somUninit");
    
    ODSafeReleaseObject( _fOwner );
    ODSafeReleaseObject( _fUpdateShape );
    ODSafeReleaseObject( _fBiasTransform );
    
    ODBaseCanvas_parents_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// InitBaseCanvas
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasInitBaseCanvas(ODBaseCanvas *somSelf, Environment *ev,
        ODBoolean isDynamic,
        ODBoolean isOffscreen)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","InitBaseCanvas");
    
    SOM_TRY

    /* Moved from somInit. SOM itself sets fields to zero
    _fOwner = kODNULL;
    _fUpdateShape = kODNULL;
    _fFacet = kODNULL;
    _fIsDynamic = kODTrue;
    _fIsOffscreen = kODFalse;
    _fBiasTransform = kODNULL;
    _fPlatformPrintJob = kODNULL;
    */
    _fPrintJobGraphicsSystem = kODNoGraphicsSystem;

    somSelf->InitObject(ev);

    _fIsDynamic = isDynamic;
    _fIsOffscreen = isOffscreen;

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// GetOwner
//------------------------------------------------------------------------------

SOM_Scope ODPart*  SOMLINK ODBaseCanvasAcquireOwner(ODBaseCanvas *somSelf, Environment *ev)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","AcquireOwner");

    if( _fOwner ) {
        SOM_TRY
            _fOwner->Acquire(ev);
        SOM_CATCH_ALL
        SOM_ENDTRY
    }
    return _fOwner;
}

//------------------------------------------------------------------------------
// SetOwner
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasSetOwner(ODBaseCanvas *somSelf, Environment *ev,
        ODPart* owner)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","SetOwner");
    
    if( owner != _fOwner ) {
        SOM_TRY
            ODTransferReference(ev,_fOwner,owner);
            _fOwner = owner;
        SOM_CATCH_ALL
        SOM_ENDTRY
    }
}

//------------------------------------------------------------------------------
// GetFacet
//------------------------------------------------------------------------------

SOM_Scope ODFacet*  SOMLINK ODBaseCanvasGetFacet(ODBaseCanvas *somSelf, Environment *ev)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","GetFacet");

    return _fFacet;
}

//------------------------------------------------------------------------------
// SetFacet
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasSetFacet(ODBaseCanvas *somSelf, Environment *ev,
        ODFacet* facet)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","SetFacet");

    _fFacet = facet;
}

//------------------------------------------------------------------------------
// IsDynamic
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseCanvasIsDynamic(ODBaseCanvas *somSelf, Environment *ev)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","IsDynamic");

    return _fIsDynamic;
}

//------------------------------------------------------------------------------
// IsOffscreen
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseCanvasIsOffscreen(ODBaseCanvas *somSelf, Environment *ev)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","IsOffscreen");

    return _fIsOffscreen;
}

//------------------------------------------------------------------------------
// HasPlatformCanvas
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseCanvasHasPlatformCanvas(ODBaseCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g)
{
    WARNMSG(WARN_INDEX(AMSG_540),"ODCanvas: A subclass should have overridden this method!");
    ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
    return kODFalse;
}


//------------------------------------------------------------------------------
// GetPlatformCanvas
//------------------------------------------------------------------------------

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
SOM_Scope ODPlatformCanvas*  SOMLINK ODBaseCanvasGetPlatformCanvas(ODBaseCanvas *somSelf, Environment *ev,
#else
SOM_Scope ODPlatformCanvas  SOMLINK ODBaseCanvasGetPlatformCanvas(ODBaseCanvas *somSelf, Environment *ev,
#endif
        ODGraphicsSystem g)
{
    WARNMSG(WARN_INDEX(AMSG_540),"ODCanvas: A subclass should have overridden this method!");
    ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
    return kODNULL;
}

//------------------------------------------------------------------------------
// SetPlatformCanvas
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasSetPlatformCanvas(ODBaseCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g,
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
      ODPlatformCanvas* c)
#else
        ODPlatformCanvas c)
#endif
{
    WARNMSG(WARN_INDEX(AMSG_540),"ODCanvas: A subclass should have overridden this method!");
    ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// HasPlatformPrintJob
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseCanvasHasPlatformPrintJob(ODBaseCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","HasPlatformPrintJob");
    
    // This base implementation can only keep track of one graphics system's print job at
    // a time; it seems unlikely that a print job would be run by more than one graphics
    // system at once. If you really need to store more than one (as is done with platform
    // canvases) then just override the 3 PlatformPrintJob methods in your platform subclass.

    return _fPlatformPrintJob!=kODNULL && g==_fPrintJobGraphicsSystem;
}

//------------------------------------------------------------------------------
// GetPlatformPrintJob
//------------------------------------------------------------------------------

SOM_Scope ODPlatformPrintJob  SOMLINK ODBaseCanvasGetPlatformPrintJob(ODBaseCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","GetPlatformPrintJob");

    if( _fPlatformPrintJob!=kODNULL && g==_fPrintJobGraphicsSystem )
        return _fPlatformPrintJob;
    else {
        ODSetSOMException(ev, kODErrInvalidGraphicsSystem);
        return kODNULL;
    }
}

//------------------------------------------------------------------------------
// SetPlatformPrintJob
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasSetPlatformPrintJob(ODBaseCanvas *somSelf, Environment *ev,
        ODGraphicsSystem g,
        ODPlatformPrintJob j)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","SetPlatformPrintJob");

    _fPrintJobGraphicsSystem = g;
    _fPlatformPrintJob = j;
}

//------------------------------------------------------------------------------
// GetUpdateShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseCanvasAcquireUpdateShape(ODBaseCanvas *somSelf, Environment *ev)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","AcquireUpdateShape");
    
    SOM_TRY

    if( !_fUpdateShape )
        somSelf->ResetUpdateShape(ev);
    if( _fUpdateShape )
        _fUpdateShape->Acquire(ev);
    return _fUpdateShape;

    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// ResetUpdateShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasResetUpdateShape(ODBaseCanvas *somSelf, Environment *ev)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","ResetUpdateShape");
    
    if( !_fIsOffscreen )
        return;
    
    SOM_TRY

    if( !_fUpdateShape ) {
        // Create a new, empty update shape. It would be nice to use a factory,
        // but from here there's no factory object we can reach...

// 5/28/96  jab  Creating a TempODShape and not doing an Acquire is causing
//               warnings all over in some of the odutils test cases.  Since this
//               code looks suspicious, I'm wrapping it with _PLATFORM_MACINTOSH_
//               to remove it from the build.
#ifdef _PLATFORM_MACINTOSH_          
        TempODShape s = new ODShape;
        THROW_IF_NULL(s);
        s->InitShape(ev);
        _fUpdateShape = s.DontRelease();
#endif

      // This section of code replaces the above section wrapped in _PLATFORM_MACINTOSH_.
      // It was pulled from the OS/2 Feb. 13 drop.
      ODShape *s = new ODShape;
      THROW_IF_NULL(s);
      s->InitShape(ev);
      _fUpdateShape = s;
            
    } else {
        // Just clear the update shape:
        ODRect empty(0,0,0,0);
        _fUpdateShape->SetRectangle(ev,&empty);
    }

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ComputeBiasTransform
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasComputeBiasTransform(
                                            ODBaseCanvas *somSelf, Environment *ev)
{
//    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","ComputeBiasTransform");

    // Do nothing by default.
}

//------------------------------------------------------------------------------
// GetBiasTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODBaseCanvasAcquireBiasTransform(ODBaseCanvas *somSelf, Environment *ev)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","GetBiasTransform");

    SOM_TRY
    if (_fBiasTransform)
        _fBiasTransform->Acquire(ev);
    return _fBiasTransform;

    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODNULL;
}

//------------------------------------------------------------------------------
// SetBiasTransform
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasSetBiasTransform(ODBaseCanvas *somSelf, Environment *ev,
        ODTransform* x)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","SetBiasTransform");

    if( x != _fBiasTransform ) {
        SOM_TRY
    
            ODTransferReference(ev,_fBiasTransform,x);
            _fBiasTransform = x;

        SOM_CATCH_ALL
        SOM_ENDTRY
    }
}

//------------------------------------------------------------------------------
// Invalidate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasInvalidate(ODBaseCanvas *somSelf, Environment *ev,
        ODShape* shape)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","Invalidate");


    if ( !_fIsOffscreen )
        return;
        
    SOM_TRY

    ASSERT_NOT_NULL( shape );

    if( !_fUpdateShape )
        somSelf->ResetUpdateShape(ev);
    _fUpdateShape->Union(ev,shape);

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
// Validate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseCanvasValidate(ODBaseCanvas *somSelf, Environment *ev,
        ODShape* shape)
{
    ODBaseCanvasData *somThis = ODBaseCanvasGetData(somSelf);
    ODBaseCanvasMethodDebug("ODBaseCanvas","Validate");

    if ( !_fIsOffscreen )
        return;

    SOM_TRY

    if( !_fUpdateShape )
        somSelf->ResetUpdateShape(ev);
    _fUpdateShape->Subtract(ev,shape);

    SOM_CATCH_ALL
    SOM_ENDTRY
}

