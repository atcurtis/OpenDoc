/* @(#)Z 1.10 com/src/imaging/realshps/PolyShpe.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:09 (96/10/29 09:26:56) */
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
/*          3/19/96 jab  Modifications for cross-platform support.  */
/*  122004  5/11/95 aml  Performance enhancements                   */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    PolyShpe.cpp

  Contains:  Polygon shape class, private to ODShape.

  Owned by: Jens Alfke

  Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

    <12>   8/18/95  NP	  1274946: Remove kODErrInvalidParameter
    <11>   8/16/95  NP	  1274946: ErrorDef.idl problems. Add include
			     file.
    <10>   5/25/95  jpa   Use new GX headers [1241078, 1253324]
     <9>   3/20/95  jpa   No need to wrap TRY around use of
                  ODTempPolygon. [1215160
     <8>  12/20/94  jpa    Removed bogus call to Simplify in
                  InitQDRegion. [1168281]
     <7>   12/5/94  jpa    Removed fSimple; assume client will call
                  Simplify after SetPolygon. [1191192,
                  1196018]. Also code-review cleanup
                  [1203923]
     <6>  10/24/94  jpa    Implemented Outset. [1186719, 1190423].
                  Added fSimple flag, and simplified before
                  converting to Region. [1191192]
     <5>   9/29/94  RA    1189812: Mods for 68K build.
     <4>    9/9/94  jpa    Partial implementation of Outset; right now
                  punts to RgnShape since poly outsetter is
                  unimplemented. [1178690]
     <3>    8/8/94  jpa    Added Outset method [1178690]
     <2>    8/2/94  jpa    Added no-op case to ::Combine.
     <1>   6/15/94  jpa    first checked in

  In Progress:
*/

#ifdef _PLATFORM_OS2_
#define INCL_GPI
#define INCL_WIN
#endif   // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
#include <wtypes.h>
#include <wingdi.h>
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif
#endif    // IBM Platforms

#ifndef _ALTPOINT_
#include "AltPoint.h"      /* Use C++ savvy ODPoint and ODRect*/
#endif


#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifndef _POLYSHPE_
#include "PolyShpe.h"
#endif

#ifndef _RECTSHPE_
#include "RectShpe.h"
#endif

#ifndef _RGNSHPE_
#include "RgnShpe.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef _RGN2POLY_
#include "Rgn2PlyM.h"
#endif

#ifndef _POLYCLIP_
#include "PolyClip.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif


#ifdef _PLATFORM_MACINTOSH_ 
#ifndef __GXGRAPHICS__
#include <GXGraphics.h>
#endif
#endif


PolygonShape::PolygonShape( ODGeometryMode mode )
  :RealShape( mode ),
   fPolygon(),
   fBoundsValid( kODFalse )
{
#if ODDebug
   fType = 2;
#ifdef _PLATFORM_MACINTOSH_ 
   WASSERTM(!gGX,"PolygonShape is not worthy: GX installed");
#endif // !_PLATFORM_MACINTOSH_
#endif // ODDebug
}


PolygonShape::~PolygonShape( )
{
   fPolygon.Clear();
}


ODSize
PolygonShape::Purge( ODSize bytes )
{
   if( bytes==0 )
      fBoundsValid = kODFalse;    // Means just flush cache
   return RealShape::Purge( bytes );
}


void
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_) // CED
PolygonShape::GetBoundingBox( Environment*, ODRect *bounds )
#else
PolygonShape::GetBoundingBox( ODRect *bounds )
#endif // IBM Platforms
{
   if( !fBoundsValid )
      fPolygon.ComputeBoundingBox( &fBounds );
   if( bounds )
     *bounds = fBounds;
}


RealShape*
PolygonShape::SetRectangle( const ODRect *r )
{
   RealShape *rect = new RectShape( fMode,*r );
   delete this;
   return rect;
}


void
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
PolygonShape::CopyPolygon( Environment*, ODPolygon &poly )
#else
PolygonShape::CopyPolygon( ODPolygon &poly )
#endif // IBM Platforms
{
   poly.CopyFrom(fPolygon);
}


RealShape*
PolygonShape::SetPolygon( const ODPolygon &poly )
{
   fPolygon.CopyFrom( poly );
   this->Purge(0);
   return this;
}


void
PolygonShape::Simplify( )
{
  // Client is responsible for calling this method after SetPolygon
  // if the polygon is not known to be simple. PolygonShape assumes
  // its fPolygon is always simple.

   PolySimplify( fPolygon, fPolygon );
}


void
#if defined(_PLATFORM_OS2_) || (_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
PolygonShape::InitQDRegion( Environment* ev)
#else
PolygonShape::InitQDRegion( )
#endif  // IBM Platforms
{
#ifdef _PLATFORM_OS2_
   HPS hps = GetPresSpace();
   RgnHandle rgn = fPolygon.AsRegion(hps);
   if (fQDRegion)
//     GpiDestroyRegion(hps, fQDRegion);
      fQDRegion.DisposeRgn();
   ReleasePresSpace(hps);
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   RgnHandle rgn = fPolygon.AsWinRegion();
   if (fQDRegion)
      fQDRegion.DisposeRgn();
#endif  // _PLATFORM_WIN32_
  
#ifdef _PLATFORM_UNIX_
   RgnHandle rgn = fPolygon.AsXRegion();
   if (fQDRegion)
      fQDRegion.DisposeRgn();
#endif  // _PLATFORM_UNIX_
  
#ifdef _PLATFORM_MACINTOSH_
   RgnHandle rgn = fPolygon.AsQDRegion();
   ODDisposeHandle((ODHandle)fQDRegion);
#endif // _PLATFORM_MACINTOSH__

   fQDRegion = rgn;

}


#ifdef _PLATFORM_MACINTOSH_ // CED
gxShape
PolygonShape::CopyGXShape( )
{
   ASSERT( gGX>0, kODErrAssertionFailed );
   return fPolygon.AsGXShape();
}
#endif // _PLATFORM_MACINTOSH_


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)   || defined(_PLATFORM_UNIX_)
PolygonShape::IsSameAs( Environment* ev, RealShape *shape )
{
   if( shape == this )
     return kODTrue;
   else if( this->IsEmpty(ev) )
     return shape->IsEmpty(ev);

   else if( shape->IsEmpty(ev) )
     return kODFalse;

   else if( shape->IsRectangular(ev) )
      if( this->IsRectangular(ev) ) {
         ODRect itsBox;
         this->GetBoundingBox( ev, NULL );
         shape->GetBoundingBox( ev, &itsBox );
         return fBounds==itsBox;

      } 
      else
         return kODFalse;

   else if( this->IsRectangular(ev) )
      return kODFalse;

   else if( shape->HasGeometry() ) {
      ODTempPolygon poly;
      shape->CopyPolygon(ev, poly);
      ODBoolean same;
      same = (fPolygon == poly);
      return same;

   } 
   else
     return kODFalse;
}
#endif  // IBM Platforms


#ifdef _PLATFORM_MACINTOSH_
PolygonShape::IsSameAs( RealShape *shape )
{
  if( shape == this )
    return kODTrue;

  else if( this->IsEmpty() )
    return shape->IsEmpty();

  else if( shape->IsEmpty() )
    return kODFalse;

  else if( shape->IsRectangular() )
    if( this->IsRectangular() ) {
      ODRect itsBox;
      this->GetBoundingBox(NULL);
      shape->GetBoundingBox(&itsBox);
      return fBounds==itsBox;

    } else
      return kODFalse;

  else if( this->IsRectangular() )
    return kODFalse;

  else if( shape->HasGeometry() ) {
    ODTempPolygon poly;
    shape->CopyPolygon(poly);
    ODBoolean same;
    same = (fPolygon == poly);
    return same;

  } else
    return kODFalse;
}
#endif // _PLATFORM_MACINTOSH_


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
PolygonShape::IsEmpty( Environment* )
#else
PolygonShape::IsEmpty( )
#endif
{
   return fPolygon.IsEmpty();
}


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
PolygonShape::ContainsPoint( Environment* ev, ODPoint point )
#else
PolygonShape::ContainsPoint( ODPoint point )
#endif // IBM Platforms 
{
   if( fPolygon.HasData() ) {
      ODRect bbox;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
      this->GetBoundingBox(ev, NULL);
#else
      this->GetBoundingBox(NULL);
#endif // IBM Platforms

      if( !fBounds.Contains(point) )
         return kODFalse;          // Not in bounding-box
      return fPolygon.Contains(point) != 0;
   }  
   else
      return kODFalse;
}


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
PolygonShape::IsRectangular(Environment* )
#else
PolygonShape::IsRectangular( )
#endif // IBM Platforms
{
   return fPolygon.IsRectangular();
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)   || defined(_PLATFORM_UNIX_)
PolygonShape::Copy( Environment* )
#else
PolygonShape::Copy( )
#endif // IBM Platforms
{
   PolygonShape *s = new PolygonShape( fMode );
   TRY{
      s->SetPolygon( fPolygon );
   }CATCH_ALL{
      delete s;
      RERAISE;
   }ENDTRY
   return s;
}


RealShape*
PolygonShape::Transform( Environment *ev, ODTransform *xform )
{
  fPolygon.Transform( ev,xform );
  this->Purge( 0 );
  return this;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
PolygonShape::Outset( Environment* ev, ODCoordinate distance )
#else
PolygonShape::Outset( ODCoordinate distance )
#endif  // IBM Platforms
{
   if( distance!=0 && fPolygon.HasData() ) {
      if( fMode==kODLoseGeometry ) {

         // Convert to QD region:
#ifdef _PLATFORM_OS2_
         RgnHandle rgn = (RgnHandle) ODCopyRgnHandle( (RgnHandle)this->GetPlatformShape( ev, kODQuickDraw ) );
#endif 

#ifdef _PLATFORM_WIN32_
         RgnHandle rgn = (RgnHandle) ODCopyRgnHandle( (RgnHandle)this->GetPlatformShape( ev, kODWin32 ) );
#endif 

#ifdef _PLATFORM_AIX_
         RgnHandle rgn = (RgnHandle) ODCopyRgnHandle( (RgnHandle)this->GetPlatformShape( ev, kODAIX ) );
#endif 

#ifdef _PLATFORM_MACINTOSH_
         RgnHandle rgn = (RgnHandle) ODCopyHandle( (ODHandle)this->GetPlatformShape( kODQuickDraw ) );
#endif 

         RealShape *s;
         TRY{
            s = new RgnShape( fMode );
         }CATCH_ALL{
            DisposeRgn( rgn );
            RERAISE;
         }ENDTRY

         TRY{

#ifdef _PLATFORM_OS2_
            s->SetPlatformShape( kODQuickDraw, rgn );
            s = s->Outset( ev, distance );
#endif 

#ifdef _PLATFORM_WIN32_
            s->SetPlatformShape( kODWin32, rgn );
            s = s->Outset (ev, distance );
#endif 

#ifdef _PLATFORM_AIX_
            s->SetPlatformShape( kODAIX, rgn );
            s = s->Outset (ev, distance );
#endif 

#ifdef _PLATFORM_MACINTOSH_
            s->SetPlatformShape( kODQuickDraw, rgn );
            s = s->Outset( distance );
#endif

         }CATCH_ALL{
            delete s;
            RERAISE;
         }ENDTRY
         delete this;
         return s;

      } else {
         if( PolyOutset( fPolygon, distance ) )
             PolySimplify( fPolygon, fPolygon );
      }
   }
   return this;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
PolygonShape::Combine( Environment* ev, ODShapeOp op, RealShape *shape )
#else
PolygonShape::Combine( ODShapeOp op, RealShape *shape )
#endif // IBM Platforms
{
   if( op == kShapeNoOp )
      return this;
   else if( op == kShapeOutset )
      ASSERT( shape != kODNULL, kODErrInvalidParameter );

   ODPolygon result;
   {
      ODTempPolygon shapePoly;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
      shape->CopyPolygon( ev, shapePoly );
#else
      shape->CopyPolygon( shapePoly );
#endif //  IBM Platforms

      ODPolygon* poly[2];
      poly[0] = &fPolygon;
      poly[1] = &shapePoly;
      PolyClip( 2, (const ODPolygon**)poly, op, result );
   }

   if( result.HasData() ) {
      fPolygon.MoveFrom( result );
      this->Purge( 0 );
      return this;
   } else
      return this->Clear();
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
PolygonShape::Subtract( Environment* ev, RealShape *shape )
#else
PolygonShape::Subtract( RealShape *shape )
#endif // IBM Platforms
{
   if( shape == this )
      return this->Clear();

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   else if( shape->IsObviousEmpty(ev) || this->IsEmpty(ev) )
#else
   else if( shape->IsEmpty() || this->IsEmpty() )
#endif // IBM Platforms

      return this;

   // Check bounding boxes:
   ODRect itsBox;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   this->GetBoundingBox(ev, 0);
   shape->GetPesimisticBoundingBox( ev, &itsBox );
#else
   this->GetBoundingBox(NULL);
   shape->GetBoundingBox( &itsBox );
#endif // IBM Platforms


   if( !fBounds.Intersects( itsBox ) )
      return this;            // BBoxes do not intersect: no-op

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   else if( itsBox.Contains( fBounds ) && shape->IsRectangular(ev) )
#else
   else if( itsBox.Contains( fBounds ) && shape->IsRectangular() )
#endif // IBM Platforms

      return this->Clear();        // shape is a rectangle containing me: clear

   if( fMode!=kODLoseGeometry && shape->HasGeometry() )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
      return this->Combine( ev, kShapeDifference, shape );
   else
      return this->Promote( ev, kShapeDifference, shape );
#else
      return this->Combine( kShapeDifference, shape );
   else
      return this->Promote( kShapeDifference, shape );
#endif //  IBM Platforms
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
PolygonShape::Intersect( Environment* ev, RealShape *shape )
#else
PolygonShape::Intersect( RealShape *shape )
#endif // IBM Platforms
{
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

   if( shape == this || this->IsEmpty(ev) )
      return this;
   else if( shape->IsObviousEmpty(ev) )
      return this->Clear();

#else

   if( shape == this || this->IsEmpty() )
      return this;
   else if( shape->IsEmpty() )
      return this->Clear();

#endif // IBM Platforms

   // Check bounding boxes:
   ODRect itsBox;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   this->GetBoundingBox( ev, 0 );
   shape->GetPesimisticBoundingBox( ev, &itsBox );
#else
   this->GetBoundingBox(0);
   shape->GetBoundingBox( &itsBox );
#endif // IBM Platforms

   if( !fBounds.Intersects( itsBox ) )
      return this->Clear();        // BBoxes do not intersect: clear

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   else if( itsBox.Contains( fBounds ) && shape->IsRectangular(ev) )
#else
   else if( itsBox.Contains( fBounds ) && shape->IsRectangular() )
#endif // IBM Platforms

    return this;            // shape is a rectangle containing me: no-op


   // OK, must intersect:
   if( fMode!=kODLoseGeometry && shape->HasGeometry() )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
      return this->Combine( ev, kShapeIntersection, shape );
   else
      return this->Promote( ev, kShapeIntersection, shape );
#else
      return this->Combine( kShapeIntersection, shape );
   else
      return this->Promote( kShapeIntersection, shape );
#endif // IBM Platforms
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
PolygonShape::Union( Environment* ev, RealShape *shape )
#else
PolygonShape::Union( RealShape *shape )
#endif // IBM Platforms
{
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

   if( shape == this || shape->IsObviousEmpty(ev) )
      return this;
   else if( this->IsEmpty(ev) )
      return this->ReplaceWith( ev, shape );

#else

   if( shape == this || shape->IsEmpty() )
      return this;
   else if( this->IsEmpty() )
      return this->ReplaceWith( shape );

#endif // IBM Platforms


  // Check bounding boxes:
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

   if( shape->IsObviousRectangular(ev) ) {
      ODRect itsBox;
      this->GetBoundingBox( ev, 0 );
      shape->GetPesimisticBoundingBox( ev, &itsBox );
      if( itsBox.Contains( fBounds ) )
         return this->ReplaceWith( ev, shape );

#else

   if( shape->IsRectangular() ) {
      ODRect itsBox;
      this->GetBoundingBox(0);
      shape->GetBoundingBox(&itsBox);
      if( itsBox.Contains(fBounds) )
         return this->ReplaceWith(shape);

#endif // IBM Platforms
   }

   if( fMode!=kODLoseGeometry && shape->HasGeometry() )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

      return this->Combine(ev, kShapeUnion,shape);
   else
      return this->Promote(ev, kShapeUnion,shape);

#else

      return this->Combine(kShapeUnion,shape);
   else
      return this->Promote(kShapeUnion,shape);

#endif // IBM Platforms

}




