/* @(#)Z 1.10 com/src/imaging/realshps/RectShpe.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:10 (96/10/29 09:27:14) */
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

/*
  File:    RectShpe.h

  Contains:  RealShape class, private to ODShape.

  Owned by:  Jens Alfke

  Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):
	
     <7>    9/13/95 DM	   1277216 GM:API return no ODPoints nor
					ODPolygons
     <6>    8/16/95 NP	   1274946: ErrorDef.idl problems. Add include
					file.
     <5>   5/25/95  jpa    Use new GX headers [1241078, 1253324]
     <4>   12/5/94  jpa    gxSolidFill -> gxWindingFill. [1191192]
     <3>    8/8/94  jpa    Added Outset method [1178690]
     <2>    8/2/94  jpa    Use AsPolygonShape, not Promote, in
                  Transform method.
     <1>   6/15/94  jpa    first checked in

  In Progress:
*/

#ifdef _PLATFORM_OS2_
#define INCL_WIN
#define INCL_GPI
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

#ifndef _RECTSHPE_
#include "RectShpe.h"
#endif

#ifndef _PolySHPE_
#include "PolyShpe.h"
#endif

#ifndef SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif


#ifdef _PLATFORM_MACINTOSH_ 

#ifndef __GXGRAPHICS__
#include <GXGraphics.h>
#endif

#pragma segment ODShape
#endif // _PLATFORM_MACINTOSH_


RectShape::RectShape( ODGeometryMode mode, const ODRect &rect )
   :RealShape(mode),
    fRect(rect)
{
#if ODDebug
   fType = 0;
#endif
}


void
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::GetBoundingBox( Environment*, ODRect *bounds )
#else
RectShape::GetBoundingBox( ODRect *bounds )
#endif // IBM Platforms
{
   *bounds = fRect;
}


RealShape*
RectShape::SetRectangle( const ODRect *r )
{
   fRect = *r;
   this->Purge(0);
   return this;
}


void
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::CopyPolygon( Environment*, ODPolygon &poly )
#else
RectShape::CopyPolygon( ODPolygon &poly )
#endif    // IBM Platforms
{
   poly.SetRect(fRect);
}


void
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::InitQDRegion( Environment* ev )
#else
RectShape::InitQDRegion( )
#endif   // IBM Platforms
{

   Rect r;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_MACINTOSH_)
   fRect.AsQDRect( r );
   RectRgn( fQDRegion, &r );
#endif   

#ifdef _PLATFORM_WIN32_
   fRect.AsWinRect( r );
   fQDRegion = CreateRectRgn( r.left, r.top, r.right, r.bottom );
#endif   

#ifdef _PLATFORM_UNIX_
   fRect.AsXRect( r );
   Region newRgn = XCreateRegion();
   XUnionRectWithRegion(&r, fQDRegion, newRgn);
   fQDRegion = newRgn;
#endif   

}


#ifdef _PLATFORM_MACINTOSH_ // CED
gxShape
RectShape::CopyGXShape( )
{
   ASSERT(gGX>0,kODErrAssertionFailed);
   gxShape r = GXNewRectangle( (gxRectangle*) &fRect );
   GXSetShapeFill(r,gxWindingFill);
   ThrowIfGXError();
   return r;
}
#endif // _PLATFORM_MACINTOSH_


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::IsSameAs( Environment* ev, RealShape *shape )
#else
RectShape::IsSameAs( RealShape *shape )
#endif // IBM Platforms
{

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
   if( !shape->IsRectangular(ev) )
#else
   if( !shape->IsRectangular() )
#endif   // IBM Platforms
     return kODFalse;

   ODRect r;
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
   shape->GetBoundingBox( ev, &r );
#else
   shape->GetBoundingBox(&r);
#endif   // IBM Platforms

   return r == fRect;
}


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::IsEmpty( Environment* ev )
#else
RectShape::IsEmpty( )
#endif   // IBM Platforms
{
   return fRect.IsEmpty();
}


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::ContainsPoint( Environment*, ODPoint point )
#else
RectShape::ContainsPoint( ODPoint point )
#endif   // IBM Platforms
{
   return fRect.Contains( point );
}


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::IsRectangular( Environment* )
#else
RectShape::IsRectangular( )
#endif   // IBM Platforms
{
   return kODTrue;    // Duhhhhhh...
}


RealShape*
RectShape::Clear( )
{
  fRect.Clear();
  this->Purge(0);
  return this;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::Copy( Environment* )
#else
RectShape::Copy( )
#endif   // IBM Platforms  
{
   RectShape *r = new RectShape( fMode, fRect );
   return r->SetRectangle( &fRect );
}


RealShape*
RectShape::Transform( Environment *ev, ODTransform *xform )
{
   if( fRect.IsEmpty() )
    ;                  // I'm empty: do nothing

   else if( xform->GetType(ev) <= kODScaleTranslateXform ) {

//#ifdef _PLATFORM_OS2_ // CED
// This was ported directly from OS/2.  The cross-platform version
// of OpenDoc still maintains TopLeft and BottomRight as member
// functions of ODRect.  We'll keep this in the code just as a reference
// until we're done with testing.  Ya never know when we might find this
// useful.  jab - 05/20/96
//    ODPoint botLeft = fRect.BotLeft();
//    ODPoint topRight = fRect.TopRight();
//    xform->TransformPoint(ev,&botLeft);  // [137664]
//    xform->TransformPoint(ev,&topRight);
//    fRect = ODRect(botLeft,topRight);
//
//#else

      ODPoint topLeft = fRect.TopLeft();
      ODPoint botRight = fRect.BotRight();
      xform->TransformPoint( ev, &topLeft ); 
      xform->TransformPoint( ev, &botRight );
      fRect = ODRect( topLeft, botRight );

//#endif    // _PLATFORM_OS2_

      this->Purge(0);

   } else {

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
      RealShape* s = this->AsPolygonShape(ev);
#else
      RealShape* s = this->AsPolygonShape();  // General case: Promote myself
#endif    // IBM Platforms

      TRY{
         s= s->Transform(ev,xform);  // ...and ask the promoted shape to do the job.
      }CATCH_ALL{
         delete s;
         RERAISE;
      }ENDTRY

      delete this;
      return s;
   }

   return this;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::Outset( Environment* ev, ODCoordinate distance )
#else
RectShape::Outset( ODCoordinate distance )
#endif   // IBM Platforms
{
   if( ! fRect.IsEmpty() ) {
      fRect.Inset( -distance, -distance );
      this->Purge(0);
   }
   return this;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::Subtract( Environment* ev, RealShape *shape )
#else
RectShape::Subtract( RealShape *shape )
#endif // IBM Platforms
{

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
   if( shape->IsObviousEmpty(ev) || this->IsEmpty(ev) )        // I (heart) no-ops
#else
   if( shape->IsEmpty() || this->IsEmpty() )        // I (heart) no-ops
#endif   // IBM Platforms
      return this;

   else if( shape == this )                  // S - S = 0
      return this->Clear();

   else {

      ODRect itsBounds;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
      shape->GetPesimisticBoundingBox( ev, &itsBounds );
#else
      shape->GetBoundingBox( &itsBounds );
#endif   // IBM Platforms

      if( !fRect.Intersects( itsBounds ) )
         return this;
      else
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
         return this->Promote( ev, kShapeDifference, shape );
#else
         return this->Promote( kShapeDifference, shape );
#endif   // IBM Platforms
   }
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::Intersect( Environment* ev, RealShape *shape )
#else
RectShape::Intersect( RealShape *shape )
#endif   // IBM Platforms
{
   ODRect r;
 
   if( shape==this )
      return this;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
   shape->GetPesimisticBoundingBox( ev, &r );
   if( shape->IsObviousRectangular( ev ) ) {
      fRect &= r;              // Intersection of two rects is a rect
      this->Purge(0);
   } else if( this->IsEmpty(ev) || r.IsEmpty() || !fRect.Intersects(r) ) {
      fRect.Clear();            // One of us is empty, or we don't intersect
      this->Purge(0);            // so make myself empty

   } else if( fRect.Contains(r) )      // I contain the shape, so make myself a copy of it
      return this->ReplaceWith( ev, shape );
   else
      return this->Promote( ev, kShapeIntersection, shape );
   return this;

#else

   shape->GetBoundingBox(&r);
   if( shape->IsRectangular() ) {
       fRect &= r;              // Intersection of two rects is a rect
       this->Purge(0);
   } else if( this->IsEmpty() || shape->IsEmpty() || !fRect.Intersects(r) ) {
       fRect.Clear();            // One of us is empty, or we don't intersect
       this->Purge(0);            // so make myself empty

   } else if( fRect.Contains(r) )      // I contain the shape, so make myself a copy of it
       return this->ReplaceWith( shape );
   else
      return this->Promote( kShapeIntersection, shape );

   return this;

#endif   // IBM Platforms 
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RectShape::Union( Environment* ev, RealShape *shape )
#else
RectShape::Union( RealShape *shape )
#endif   //  IBM Platforms
{

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)

   if( shape==this || shape->IsObviousEmpty(ev) )  // Shape is me or is empty: no-op
      return this;
   if( this->IsEmpty(ev) )          // I am empty: change to a copy of the other shape
      return this->ReplaceWith( ev, shape );

   ODRect bounds;
   shape->GetPesimisticBoundingBox( ev, &bounds );

   if( fRect.Contains( bounds ) )
      ;                  // Shape is contained in me: no-op

   else if( shape->IsObviousRectangular( ev ) && bounds.Contains( fRect ) ) {
      fRect = bounds;
      this->Purge(0);            // I am contained in another rectangle:
                      // just grow my bounds to its
   } else
      return this->Promote( ev, kShapeUnion, shape );

   return this;


#else

   if( shape==this || shape->IsEmpty() )  // Shape is me or is empty: no-op
      return this;
   if( this->IsEmpty() )          // I am empty: change to a copy of the other shape
      return this->ReplaceWith( shape );

   ODRect bounds;
   shape->GetBoundingBox( &bounds );

   if( fRect.Contains( bounds ) )
      ;                  // Shape is contained in me: no-op

   else if( shape->IsRectangular() && bounds.Contains( fRect ) ) {
      fRect = bounds;
      this->Purge(0);            // I am contained in another rectangle:
                      // just grow my bounds to its
   } else
      return this->Promote( kShapeUnion, shape );

   return this;

#endif   //  IBM Platforms

}




