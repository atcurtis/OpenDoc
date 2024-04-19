/* @(#)Z 1.19 com/src/imaging/realshps/RealShpe.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:10 (96/10/29 09:27:05) */
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
/********************************************************************/
/*  IBM Change History (most recent first):                         */
/*  23332   6/24/96 chb  Added SetDisplay/fDisplay for X Windows    */
/*          3/19/96 jab  Changes for cross platform support.        */
/*  122017  5/11/95 aml  Performance enhancements                   */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    RealShpe.cpp

  Contains:  RealShape class, private to ODShape.

  Owned by:  Jens Alfke

  Copyright:  © 1993-1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

    <10>   8/29/95  jpa   Don't use kODErrInvalidValue. [1278284]
     <9>   5/25/95  jpa    Use new GX headers [1241078, 1253324]
     <8>   2/24/95  jpa   Removed private ODNewRgn. [1220810]
     <7>   12/5/94  jpa    Code review cleanup [1203923]
     <6>  10/24/94  jpa    Added NewGeometricShape [1195070]. Added
                  ODNewRgn: allocate rgns from temp-mem.
                  [1151710]
     <5>   9/29/94  RA    1189812: Mods for 68K build.
     <4>    8/2/94  jpa    Added AsPolygonShape method.
     <3>   7/26/94  jpa    Initialize gGX to 99, not -1 [it's
                  unsigned]. Actually any value >1 will do.
     <2>   6/18/94  MB    Update memory includes
     <1>   6/15/94  jpa    first checked in
              --------Moved to SOM project and rearranged-----
     <9>   5/10/94  jpa    Replaced 'inherited::' to appease %$#@
                  cfront.
     <8>    5/9/94  MB    #1162181: Changes necessary to install MMM.
     <7>    5/9/94  jpa    Enable polygon clipper and GX support
                  [1162090]
     <6>   3/25/94  JA    Use new XMPTransform API. (1153438)
     <5>   3/15/94  MB    Changes to support SCpp/ASLM builds,
                  #1150864.
     <4>   2/17/94  JA    Include new AltPoly.h.
     <3>   2/16/94  JA    Stop using pointers to member fns. Include
                  new AltPoint.h.
     <2>    2/9/94  JA    ShapePrv.h --> ShapePvM.h
    <10>    2/7/94  JA    Capitalized THROW.
     <9>    2/7/94  JA    Further tigerings.
     <8>    2/3/94  JA    Tiger Team Makeover!
     <7>   1/31/94  JA    Mucho fixes to polygon shapes & region
                  shapes. Other bugs squashed. Etc.
     <6>   12/3/93  T‚    Stop including XMPError.h, it is included
                  as ErrorDef.h inside Except.h
     <5>  11/29/93  JA    Fixed XMPRectShape::Union.
     <4>  11/24/93  JA    cfront fixes.
     <3>  11/24/93  VL    Rolled back changes as ASLM build breaks
                  the THINK build.
     <2>  11/23/93  VL    Made this work with ASLM.
     <1>  11/23/93  JA    first checked in

  In Progress:
*/

//#define _DO_POLYCLIP_

#ifdef _PLATFORM_OS2_
#define INCL_GPI
#define INCL_WIN
#endif

#ifdef _PLATFORM_WIN32_
#include <wtypes.h>
#include <wingdi.h>
#endif

#ifndef _ALTPOINT_
#include "AltPoint.h"      /* Use C++ savvy XMPPoint and XMPRect*/
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif
#endif   // IBM Platforms

#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifdef _PLATFORM_OS2_
#include <builtin.h>
#endif

#ifndef _REALSHPE_
#include "RealShpe.h"
#endif

#ifdef _PLATFORM_MACINTOSH_ 
#ifndef _GXSHPE_
#include "GXShpe.h"
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _POLYSHPE_
#include "PolyShpe.h"
#endif

#ifndef _RECTSHPE_
#include "RectShpe.h"
#endif

#ifndef _RGNSHPE_
#include "RgnShpe.h"
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#ifndef _COMPSHPE_
#include "CompShpe.h"
#endif
#endif

#ifndef _SHAPE_
#include "Shape.xh"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __GXGRAPHICS__
#include <GXGraphics.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifdef _PLATFORM_UNIX_
#ifndef _SHAPE_H_
#include "X11/extensions/shape.h"
#endif
#endif

#ifdef _PLATFORM_UNIX_
	    static Display *fDisplay = (Display *)NULL;
	    static ODBoolean fShapeExtensionOK = kODFalse;
	    static ODBoolean fShapeExtensionOK_Known = kODFalse;
#endif // _PLATFORM_UNIX_

#ifdef RGNDEBUG
#include "qprintf.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
//==============================================================================
// QuickDraw GX Utilities
//==============================================================================


ODBoolean gGX = 99;    // Is GX installed? Initialized by ODShape constructor


#pragma segment QDGXShape


void
ClearGXError( )
{
  GXGetGraphicsError(kODNULL);
  // GX error status is cleared after asking for errors.
}


void
ThrowIfGXError( )
{
  gxGraphicsError err = GXGetGraphicsError(kODNULL);  // Get latest graphics error
  if( err )
    THROW(err,"QuickDraw GX error");
}


void
ThrowIfFirstGXError( )
{
  gxGraphicsError err;
  (void) GXGetGraphicsError(&err);      // Get first error, not last
  if( err )
    THROW(err,"QuickDraw GX error");
}
#endif // _PLATFORM_MACINTOSH_


//==============================================================================
// RealShape
//==============================================================================


#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODShape
#endif // _PLATFORM_MACINTOSH_


RealShape::RealShape( ODGeometryMode mode )
   :fQDRegion(kODNULL),
    fMode(mode)
{
}


RealShape::~RealShape( )
{
   this->Purge(0);
}


ODSize
RealShape::Purge( ODSize )
{
   if( fQDRegion ) {

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

      ODSize size = 0;
      fQDRegion.DisposeRgn();

#else

      ODSize size = GetHandleSize( (Handle)fQDRegion );
      DisposeRgn( fQDRegion );

#endif // IBM Platforms

      fQDRegion = kODNULL;
      return size;
   } else
      return 0;
}


RealShape*
RealShape::NewGeometricShape( ODGeometryMode mode ) //static method
{
#ifdef _PLATFORM_MACINTOSH_
   if( gGX )
      return new QDGXShape( mode );
   else
#endif // _PLATFORM_MACINTOSH_
      return new PolygonShape( mode );
}


void
RealShape::SetGeometryMode( ODGeometryMode mode )
{
   if( mode==kODNeedsGeometry && !this->HasGeometry() )
      THROW( kODErrNoShapeGeometry );

   fMode = mode;
}


ODGeometryMode
RealShape::GetGeometryMode( )
{
   return fMode;
}


RealShape*
RealShape::SetPolygon( const ODPolygon &poly )
{
   ODRect r;
   if( poly.AsRectangle( &r ) )
      return this->SetRectangle( &r );

   else {
      RealShape *s;
      s = RealShape::NewGeometricShape( fMode );
      s = s->SetPolygon( poly );
      delete this;
    return s;
  }
}


void
RealShape::Simplify( )
{
  // Only PolygonShape does anything in this method.
}


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_MACINTOSH_)
ODPlatformShape
#ifdef _PLATFORM_OS2_ // CED
RealShape::GetPlatformShape( Environment* ev, ODGraphicsSystem system )
#else
RealShape::GetPlatformShape( ODGraphicsSystem system )
#endif // IBM Platforms
{
   if( system==kODQuickDraw ) {
      if( !fQDRegion ) {

#ifdef _PLATFORM_OS2_
         fQDRegion.InitRegion( NewRgn() );
#else
         fQDRegion = ODNewRgn();
#endif // _PLATFORM_OS2_

         TRY{

#ifdef _PLATFORM_OS2_
            this->InitQDRegion(ev);
#else
            this->InitQDRegion();
#endif

         }CATCH_ALL{

#ifdef _PLATFORM_OS2_
            fQDRegion.DisposeRgn();
#else
            DisposeRgn(fQDRegion);
#endif

            fQDRegion = kODNULL;
            RERAISE;
         }ENDTRY
      }
      return fQDRegion;

#ifdef _PLATFORM_OS2_ // CED
   }
#else
   } else if( system==kODQuickDrawGX && gGX )
      return this->CopyGXShape();
#endif // _PLATFORM_OS2_

   else {
      THROW(kODErrInvalidGraphicsSystem);
      return kODNULL;
   }
}
#endif  // _PLATFORM_OS2 || _PLATFORM_MACINTOSH_



#ifdef _PLATFORM_WIN32_
ODPlatformShape
RealShape::GetPlatformShape( Environment* ev, ODGraphicsSystem system )
{
   if( ( system == kODWin32 ) || ( system == kODWinNT ) || ( system == kODWin95 ) ) {
      if( !fQDRegion ) {
         fQDRegion.InitRegion( NewRgn() );
         TRY{
            this->InitQDRegion(ev);
         }CATCH_ALL{
            fQDRegion.DisposeRgn();
            fQDRegion = kODNULL;
            RERAISE;
         }ENDTRY
      }
      return fQDRegion;
   }
   else {
      THROW( kODErrInvalidGraphicsSystem );
      return kODNULL;
   }
}
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_AIX_
ODPlatformShape
RealShape::GetPlatformShape( Environment* ev, ODGraphicsSystem system )
{
   if( system == kODAIX ) {
      if( !fQDRegion ) {
         fQDRegion.InitRegion( NewRgn() );
         TRY{
            this->InitQDRegion(ev);
         }CATCH_ALL{
            fQDRegion.DisposeRgn();
            fQDRegion = kODNULL;
            RERAISE;
         }ENDTRY
      }
      return fQDRegion;
   }
   else {
      THROW( kODErrInvalidGraphicsSystem );
      return kODNULL;
   }
}
#endif  // _PLATFORM_AIX_



void
RealShape::SetPlatformShape( ODGraphicsSystem, ODPlatformShape )
{
   WARNMSG(WARN_INDEX(AMSG_550),"RealShape: A subclass should have been overridden this method!" );
   THROW( kODErrInvalidGraphicsSystem );
}


ODBoolean
RealShape::HasGeometry( )
{
   return kODTrue;  // ODRgnShape overrides this to return false if non-rectangular
}


RealShape*
RealShape::Clear( )
{
   // Delete me and return a new empty rect-shape:

   ODRect empty( 0,0,0,0 );
   RealShape *s = new RectShape( fMode,empty );
   delete this;
   return s;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RealShape::ReplaceWith( Environment* ev, RealShape *shape )
#else
RealShape::ReplaceWith( RealShape *shape )
#endif    // IBM Platforms
{

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   RealShape *s = shape->Copy(ev);
#else
   RealShape *s = shape->Copy();
#endif

   delete this;
   return s;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RealShape::Combine( Environment* ev, ODShapeOp op, RealShape *shape )
#else
RealShape::Combine( ODShapeOp op, RealShape *shape )
#endif   // IBM Platforms
{
   switch( op ) {

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

     case kShapeIntersection:
        return this->Intersect( ev, shape );
     case kShapeUnion:
        return this->Union( ev, shape );
     case kShapeDifference:
        return this->Subtract( ev, shape );

#else

     case kShapeIntersection:
        return this->Intersect( shape );
     case kShapeUnion:
        return this->Union( shape );
     case kShapeDifference:
        return this->Subtract( shape );

#endif // IBM Platforms

     case kShapeNoOp:
        return this;
     default:
        WARNMSG(WARN_INDEX(AMSG_590),"Illegal Shape Operation" );
        THROW( kODErrInvalidParameter );
        break;
   }
   return kODNULL;
}

RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RealShape::AsPolygonShape( Environment* ev )
#else
RealShape::AsPolygonShape( )
#endif    // IBM Platforms
{
   RealShape *s = kODNULL; ODVolatile( s );
   ODPolygon poly;      ODVolatile( poly );

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   this->CopyPolygon( ev, poly );
#else
   this->CopyPolygon( poly );
#endif   // IBM Platforms

   TRY{
      s = RealShape::NewGeometricShape( fMode );
      s->SetPolygon( poly );
   }CATCH_ALL{
      delete s;
      poly.Clear();
      RERAISE;
   }ENDTRY
   poly.Clear();

   return s;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RealShape::Promote( Environment* ev, ODShapeOp op, RealShape *shape )
{

   // _DO_POLYCLIP_ is the magic which turns on and off the use of
   // the CompositeShape class and the associated shape B-Tree.
   // The CompositeShape class overrides all of the methods of RealShape
   // and defers the calculation of the actual shape until much later.

#ifndef _DO_POLYCLIP_

   if( fMode == kODNeedsGeometry )
      THROW( kODErrNoShapeGeometry );

   return  new CompositeShape( this, shape->Copy(ev), op );

#else

   // This section of the _DO_POLYCLIP_ implements the default 
   // method of dealing with shapes as implemented by Apple including
   // the polygon clipping code.  Note that the call to Combine is what
   // the OS/2 shape B-tree algorithm defers until much later.
 
   RealShape *s; ODVolatile(s);

   if( fMode != kODLoseGeometry && ( shape == kODNULL || shape->HasGeometry() ) ) {
      s = this->AsPolygonShape( ev );
      TRY{
         s = s->Combine( ev, op, shape );
      }CATCH_ALL{
         delete s;
         RERAISE;
      }ENDTRY

   } else {
      if( fMode == kODNeedsGeometry )
         THROW( kODErrNoShapeGeometry );

#ifdef _PLATFORM_OS2_
      RgnHandle rgn = (RgnHandle) ODCopyRgnHandle(
                    (RgnHandle)this->GetPlatformShape( ev, kODQuickDraw ) );
#endif 
#ifdef _PLATFORM_WIN32_
      RgnHandle rgn = (RgnHandle) ODCopyRgnHandle(
                    (RgnHandle)this->GetPlatformShape( ev, kODWin32 ) );
#endif
#ifdef _PLATFORM_AIX_
      RgnHandle rgn = (RgnHandle) ODCopyRgnHandle(
                    (RgnHandle)this->GetPlatformShape( ev, kODAIX ) );
#endif


      TRY{
         s = new RgnShape( fMode );
      }CATCH_ALL{
         DisposeRgn( rgn );
         RERAISE;
      }ENDTRY

      TRY{
#ifdef _PLATFORM_OS2_
         s->SetPlatformShape( kODQuickDraw, rgn );
#endif
#ifdef _PLATFORM_WIN32_
         s->SetPlatformShape( kODWin32, rgn );
#endif
#ifdef _PLATFORM_AIX_
         s->SetPlatformShape( kODAIX, rgn );
#endif
         s = s->Combine( ev, op, shape );
      }CATCH_ALL{
         delete s;
         RERAISE;
      }ENDTRY
   }

   delete this;
   return s;

#endif // !_DO_POLYCLIP_

}


#else     // defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)


RealShape::Promote( ODShapeOp op, RealShape *shape )
{
   // Delete me and return an equivalent region- or polygon-shape.
   // Optionally perform a binary operation on the promoted shape.

   RealShape *s; ODVolatile(s);

   if( fMode!=kODLoseGeometry && ( shape==kODNULL || shape->HasGeometry() ) ) {
      s = this->AsPolygonShape();
      TRY{
         s = s->Combine( op,shape );
      }CATCH_ALL{
         delete s;
         RERAISE;
      }ENDTRY

   } else {
      if( fMode == kODNeedsGeometry )
         THROW( kODErrNoShapeGeometry );

      RgnHandle rgn = (RgnHandle) ODCopyHandle(
                    (ODHandle)this->GetPlatformShape( kODQuickDraw ) );
      TRY{
         s = new RgnShape( fMode );
      }CATCH_ALL{
         DisposeRgn( rgn );
         RERAISE;
      }ENDTRY

      TRY{
         s->SetPlatformShape( kODQuickDraw, rgn );
         s = s->Combine( op, shape );
      }CATCH_ALL{
         delete s;
         RERAISE;
      }ENDTRY
   }

   delete this;
   return s;
}
#endif // defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
void RealShape::GetPesimisticBoundingBox( Environment* ev, ODRect *bounds )
{
   GetBoundingBox( ev, bounds );
}


ODBoolean RealShape::IsObviousEmpty( Environment* ev )
{
   return IsEmpty( ev );
}


ODBoolean RealShape::IsObviousRectangular( Environment* ev )
{
   return IsRectangular( ev );
}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
Region_rep::Region_rep( HRGN RegionHandle ) {
   hrgn = RegionHandle;
   refs = 1;
}
#endif

#if defined(_PLATFORM_UNIX_)
Region_rep::Region_rep( Region RegionHandle ) {
   hrgn = RegionHandle;
   refs = 1;
}
#endif


Region_rep::~Region_rep() {

   if ( hrgn ) {
#ifdef _PLATFORM_OS2_
      HPS hps = GetPresSpace();
      BOOL success = GpiDestroyRegion( hps, hrgn );
  #ifdef RGNDEBUG
      if( !success )
         qprintf("Region_rep::~Region_rep - DeleteObject( hrgn ) failed.");
  #endif
      ReleasePresSpace( hps );
#endif

#ifdef _PLATFORM_WIN32_
      BOOL success = DeleteObject( hrgn );
  #ifdef RGNDEBUG
      if( !success )
         qprintf("Region_rep::~Region_rep - DeleteObject( hrgn ) failed.");
  #endif
#endif 

#ifdef _PLATFORM_UNIX_
      XDestroyRegion( hrgn );
#endif
   }
}

RegionRef::RegionRef( RegionRef& ref ) {
   rgn = ref.rgn;
   if( rgn ) 
      rgn->refs++;
}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
RegionRef::RegionRef( HRGN RegionHandle ) {
   if( RegionHandle )
      rgn = new Region_rep( RegionHandle );
   else 
      rgn = 0;
}
#endif

#if defined(_PLATFORM_UNIX_)
RegionRef::RegionRef( Region RegionHandle ) {
   if( RegionHandle )
      rgn = new Region_rep( RegionHandle );
   else 
      rgn = 0;
}
#endif

void RegionRef::DisposeRgn()
{
   if ( rgn && !--rgn->refs )
      delete rgn;
   rgn = 0;
}

RegionRef::~RegionRef() {
   DisposeRgn();
}

RegionRef& RegionRef::operator =( const RegionRef& ref ) {
   if ( rgn && !--rgn->refs )
      delete rgn;
   rgn = ref.rgn;
   if( rgn ) 
      rgn->refs++;
   return *this;
}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
RegionRef& RegionRef::operator =( HRGN RegionHandle ) {
   if ( rgn && !--rgn->refs )
      delete rgn;
   if(RegionHandle) 
      rgn = new Region_rep( RegionHandle );
   else 
      rgn = 0;
   return *this;
}
#endif

#if defined(_PLATFORM_UNIX_)
RegionRef& RegionRef::operator =( Region RegionHandle ) {
   if ( rgn && !--rgn->refs )
      delete rgn;
   if(RegionHandle) 
      rgn = new Region_rep( RegionHandle );
   else 
      rgn = 0;
   return *this;
}
#endif


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
RegionRef::operator HRGN() {
   if( rgn ) 
      return rgn->hrgn;
   return(0);
}
#endif

#if defined(_PLATFORM_UNIX_)
RegionRef::operator Region() {
   if( rgn ) 
      return rgn->hrgn;
   return(0);
}
#endif

void RegionRef::Unlink() {
   if( !rgn ) return;
   if ( rgn->refs > 1 ) {
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
      HRGN newRgn = 0;
#endif
#if defined(_PLATFORM_UNIX_)
      Region newRgn = 0;
#endif
      if ( rgn->hrgn ) {

#ifdef _PLATFORM_OS2_
         HPS hps = GetPresSpace();
         newRgn = GpiCreateRegion( hps, 0, 0 );
         GpiCombineRegion( hps, newRgn, rgn->hrgn, 0, CRGN_COPY );
         ReleasePresSpace(hps);
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
        newRgn = CreateRectRgn( 0, 0, 0, 0 );
        CombineRgn( newRgn, rgn->hrgn, 0, RGN_COPY );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
        newRgn = XCreateRegion();
        Region newRgn2 = XCreateRegion();
        XUnionRegion(newRgn2, rgn->hrgn, newRgn);
        XDestroyRegion(newRgn2);
#endif  // _PLATFORM_UNIX_

      }
      --rgn->refs;
      rgn = new Region_rep( newRgn );
   }
}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
void RegionRef::InitRegion( HRGN initRgn ) {
#if ODDebug
// jab 03/19/96 Not sure what _interrupt does...
//   if (rgn && rgn->hrgn)
//      _interrupt(3);
#endif
   if( !rgn ) {
      if( initRgn ) 
        rgn = new Region_rep( initRgn );
      else 
        rgn = 0;
   }
   else 
      rgn->hrgn = initRgn;
}
#endif

#if defined(_PLATFORM_UNIX_)
void RegionRef::InitRegion( Region initRgn ) {
#if ODDebug
// jab 03/19/96 Not sure what _interrupt does...
//   if (rgn && rgn->hrgn)
//      _interrupt(3);
#endif
   if( !rgn ) {
      if( initRgn ) 
        rgn = new Region_rep( initRgn );
      else 
        rgn = 0;
   }
   else 
      rgn->hrgn = initRgn;
}
#endif

#endif // defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

#if _PLATFORM_UNIX_
void RealShape::SetDisplay( Environment* ev, Display *display )
{
  fDisplay = display;
}

int RealShape::GetRegionRects( ODRgnHandle rgn, XRectangle** rectangles )
{
//  rectangles = (XRectangle **)NULL;
  
  if (!fShapeExtensionOK_Known) {
    int unused1, unused2;
    fShapeExtensionOK = XShapeQueryExtension(fDisplay, &unused1, &unused2);
    fShapeExtensionOK_Known = kODTrue;
  }

  if (!fShapeExtensionOK) {
    WARNMSG(WARN_INDEX(AMSG_800),"GetRegionRects - X Server does not support the X11\n"
                          "    Nonrectangular Window Shape Extension.\n");
    return 0;
  }

  Window root_win = DefaultRootWindow(fDisplay);
  int unused_x, unused_y;
  unsigned int win_width, win_height, unused_bw, win_depth;
  XWindowAttributes root_win_attr;

  if (!XGetWindowAttributes(fDisplay, DefaultRootWindow(fDisplay), &root_win_attr)) {
    WARNMSG(WARN_INDEX(AMSG_810),"GetRegionRects - can't read attributes\n"
                          "                 of root X window.\n");
    return 0;
  }

  XSetWindowAttributes unused_attr;

  Window temp_win = XCreateWindow(fDisplay,
                                  root_win,
                                  0, 0, 1024, 1024, 0,
                                  root_win_attr.depth,
                                  InputOutput, root_win_attr.visual,
                                  (unsigned long) 0, &unused_attr);
  if (!temp_win) {
    WARNMSG(WARN_INDEX(AMSG_820),"GetRegionRects - can't create temporary X window!\n");
    return 0;
  }

//  XGCValues unused_gc_vals;

//  GC tmp_gc = XCreateGC(fDisplay, temp_win, (unsigned long) 0,
//                        &unused_gc_vals);

  XShapeCombineRegion(fDisplay, temp_win, ShapeClip, 0, 0, rgn, ShapeIntersect);

  // Now get a list of rectangles back from the shaped temp. X window

  int rect_count = 0;
  int rect_ordering;

  *rectangles = XShapeGetRectangles(fDisplay, temp_win,
                                   ShapeClip, &rect_count, &rect_ordering);

  XDestroyWindow(fDisplay, temp_win);
//  XFreeGC(fDisplay, tmp_gc);

  return rect_count;
}

#endif // _PLATFORM_UNIX_
