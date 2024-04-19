/* @(#)Z 1.11 os2/src/imaging/realshps/CompShpe.cpp, odimaging, od96os2, odos29646d 96/11/15 15:49:50 (96/08/23 01:42:14) */
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
  File:    CompShpe.cpp

  Contains:  CompositeShape class, private to ODShape.

  Written by:  Chuck Dumont

*/

#ifdef _PLATFORM_OS2_
#define INCL_GPI
#define INCL_WIN
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
#include <wtypes.h>
#include <wingdi.h>
#endif

#ifndef _ALTPOINT_
#include "AltPoint.h"      // Use C++ savvy ODPoint and ODRect
#endif

#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLATSHAPE_
#include "PlatShpe.h"
#endif

#ifndef _RECTSHPE_
#include "RectShpe.h"
#endif

#ifndef _RGNSHPE_
#include "RgnShpe.h"
#endif

#ifndef _COMPSHPE_
#include "CompShpe.h"
#endif


#ifndef _POLYSHPE_
#include "PolyShpe.h"
#endif

#ifndef _POLYCLIP_
#include "PolyClip.h"
#endif

#ifndef SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef _RGN2PLYM_
#include "Rgn2PlyM.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif


void TransformRectangle(Environment* ev, ODTransform *xform, ODRect *r)
{
// This was ported directly from OS/2.  The cross-platform version
// of OpenDoc still maintains TopLeft and BottomRight as member
// functions of ODRect.  We'll keep this in the code just as a reference
// until we're done with testing.  Ya never know when we might find this
// useful.  jab - 05/20/96
//    ODPoint botLeft = r->BotLeft();
//    ODPoint topRight = r->TopRight();
//    xform->TransformPoint(ev,&botLeft);  // [137664]
//    xform->TransformPoint(ev,&topRight);
//    *r = ODRect(botLeft,topRight);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
    ODPoint topLeft = r->TopLeft();
    ODPoint botRight = r->BotRight();
    xform->TransformPoint( ev, &topLeft );  
    xform->TransformPoint( ev, &botRight );
    *r = ODRect( topLeft, botRight );
#endif  // _PLATFORM_WIN32_

}

CompositeShape::CompositeShape( RealShape* shap1, RealShape* shap2, ODShapeOp oper )
                              :  RealShape(GetMode(shap1, shap2 ) ) ,  fPolygon()
{
#if ODDebug
   fType = 4;
#endif
   shape1 = shap1;
   shape2 = shap2;
   op = oper;
   //fNumRects = 0;
   bboxValid = 0;
   bbox.Clear();
   fRgnTransform = 0;
   fPolygonTransform = 0;
}


CompositeShape::~CompositeShape( )
{
   delete shape1;
   delete shape2;
   fPolygon.Clear();
   RealShape::Purge(0);
   Environment *ev = somGetGlobalEnvironment(); // 137748 : AAA
   if ( fPolygonTransform )
      fPolygonTransform->Release( ev );
   if ( fRgnTransform )
      fRgnTransform->Release( ev );

}


ODSize
CompositeShape::Purge( ODSize requestedSize )
{
  if( fPolygon.HasData() ) {
    ODSize size = fPolygon.GetDataSize();
    fPolygon.Clear();
    if (fPolygonTransform) {
       delete fPolygonTransform;
       fPolygonTransform = 0;
    }
    size += RealShape::Purge( requestedSize );
    if ( fRgnTransform ) {
       delete fRgnTransform;
       fRgnTransform = 0;
    }
    size +=  shape1->Purge( requestedSize ) + shape2->Purge( requestedSize ) ;
    return size;
  } else
    return 0;
}


const ODSLong kMaxLong  = 0x7FFFFFFF;

#if defined(_PLATFORM_UNIX_)
#define max(a,b) (a)>(b) ? (a) : (b)
#define min(a,b) (a)<(b) ? (a) : (b)
#endif

void
CompositeShape::GetBoundingBox( Environment* ev, ODRect *bounds )
{
   ODRect b1,b2;
  if ( bboxValid ) {
     *bounds = bbox;
     return;
  }
  if ( op == kShapeUnion ) {
    shape1->GetBoundingBox( ev, &b1 );
    shape2->GetBoundingBox( ev, &b2 );
    bbox.left = min( b1.left, b2.left );
    bbox.bottom = min( b1.bottom, b2.bottom );
    bbox.right = max( b1.right, b2.right );
    bbox.top = max( b1.top, b2.top );
  } else {
    shape1->GetPesimisticBoundingBox( ev, &b1 );
    shape2->GetPesimisticBoundingBox( ev, &b2 );
    if( !b1.Intersects(b2) ) {
       if( op == kShapeIntersection ) 
          bbox.Clear();
       else if( op == kShapeDifference ) 
          shape1->GetBoundingBox( ev, &bbox );
       else { 
          WARNMSG_DEBUG( WARN_INDEX(-1), "Bogus ShapeOp in GetBoundingBox" );
          THROW( kODErrInvalidParameter );
       }
    }
    else {
            if( fMode == kODLoseGeometry ) {

#ifdef _PLATFORM_OS2_
               if( !fQDRegion ) 
                  GetPlatformShape( ev, kODQuickDraw );
               RECTL box;
               HPS hps = GetPresSpace();
               GpiQueryRegionBox(hps, fQDRegion, &box);
               ReleasePresSpace(hps);
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
               if( !fQDRegion ) 
                  GetPlatformShape( ev, kODWin32 );
               Rect box;
               LONG lComplexity = GetRgnBox( fQDRegion, &box );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_AIX_
               if( !fQDRegion ) 
                  GetPlatformShape( ev, kODAIX );
               Rect box;
               XClipBox(fQDRegion, &box);
#endif  // _PLATFORM_AIX_

               bbox = box;
            }
            else {
              this->InitQDPoly( ev );
              fPolygon.ComputeBoundingBox( &bbox );
// AL: commented out the next two lines because InitQDPoly will take care of the transform
//            if ( fPolygonTransform )
//                        fPolygonTransform->TransformRectangles( ev, &bbox, 1 );
            }
     }
  }
  *bounds = bbox;
   bboxValid = kODTrue;
}

void
CompositeShape::GetPesimisticBoundingBox( Environment* ev, ODRect *bounds )
{
   ODRect b1,b2;
  if ( bboxValid ) {
     *bounds = bbox;
     return;
  }
  if ( op == kShapeUnion ) {
    shape1->GetPesimisticBoundingBox( ev, &b1 );
    shape2->GetPesimisticBoundingBox( ev, &b2 );
        b1 |= b2;
  } else if( op == kShapeIntersection ) {
    shape1->GetPesimisticBoundingBox( ev, &b1 );
    shape2->GetPesimisticBoundingBox( ev, &b2 );
        b1 &= b2;
  }
  else if( op == kShapeDifference ) {
     shape1->GetPesimisticBoundingBox( ev,&b1 );
  }
  else {
        WARNMSG_DEBUG( WARN_INDEX(-1), "Bogus ShapeOp in GetPessimisticBoundingBox" );
        THROW( kODErrInvalidParameter );
  }
  *bounds = b1;
}


RealShape*
CompositeShape::SetRectangle( const ODRect *r )
{
  RealShape *rect = new RectShape ( fMode , *r ) ;
  delete this;
  return rect;
}


RealShape*
CompositeShape::Clear( )
{
  ODRect empty;
  empty.Clear();
  RealShape *s = new RectShape( fMode,empty );
  delete this;
  return s;
}

void
CompositeShape::InitQDPoly( Environment* ev )

{
   if( !fPolygon.HasData() ){
      ODTempPolygon shapePoly1,shapePoly2;
      shape1->CopyPolygon( ev, shapePoly1 );
      shape2->CopyPolygon( ev, shapePoly2 );
      ODPolygon* poly[2];
      poly[0] = &shapePoly1;
      poly[1] = &shapePoly2;

      TRY{
         PolyClip( 2, (const ODPolygon**)poly, op, fPolygon );
      }CATCH_ALL{
         shapePoly1.Clear();
         shapePoly2.Clear();
         RERAISE;
      }ENDTRY
   }
   else {
      if( fPolygonTransform ) fPolygon.Transform( ev, fPolygonTransform );
   }
   if ( fPolygonTransform ) {
      delete fPolygonTransform;
      fPolygonTransform = 0;
   }
}


void
CompositeShape::CopyPolygon( Environment* ev, ODPolygon &result )
{
   this->InitQDPoly(ev);
// AL: don't need the next if statement; it has been already taken care of
/*
   if ( fPolygonTransform ) {
      fPolygon.Transform( ev, fPolygonTransform );
      delete fPolygonTransform;
      fPolygonTransform = 0;
   }
*/
   result.CopyFrom( fPolygon );
}


RealShape*
CompositeShape::SetPolygon( const ODPolygon &poly)
{
   RealShape  *p;

   p   = new PolygonShape( fMode );
   p = p->SetPolygon( poly );
   delete this;
   return p;
}


void
CompositeShape::InitQDRegion( Environment* ev )
{
}



#ifdef _PLATFORM_OS2_

ODPlatformShape CompositeShape::GetPlatformShape( Environment*ev, ODGraphicsSystem system )
{
  if( system == kODQuickDraw ) {
     if (fQDRegion && (!fRgnTransform || fRgnTransform && fRgnTransform->GetType(ev) == kODIdentityXform));
     else if (fQDRegion && fRgnTransform && fRgnTransform->GetType(ev) == kODTranslateXform) {
        fQDRegion.Unlink();
        HPS hps = GetPresSpace();
        ODPoint offset;
        fRgnTransform->GetOffset(ev, &offset);
        POINTL ptlOffset = offset.AsPOINTL();
        GpiOffsetRegion(hps, fQDRegion, &ptlOffset);
        ReleasePresSpace(hps);
     }
     else {
	// Either we have no region, or the region transform is > translation.
	HPS hps = GetPresSpace();

	if (fQDRegion)
		fQDRegion.Unlink();
	else
		fQDRegion.InitRegion(GpiCreateRegion(hps, 0, 0));

	HRGN hrgn1,hrgn2;
	LONG IMode;

	hrgn1 = shape1->GetPlatformShape( ev, kODPM );
	hrgn2 = shape2->GetPlatformShape( ev, kODPM );

	switch(this->op) {
		case kShapeIntersection:
			IMode = CRGN_AND;
			break;
		case kShapeUnion:
			IMode = CRGN_OR;
			break;
		case kShapeDifference:
			IMode = CRGN_DIFF;
			break;
		default:
                        WARNMSG_DEBUG( WARN_INDEX(-1), "Bogus ShapeOp to Combine" );
			THROW( kODErrInvalidParameter );
			break;
	}
	GpiCombineRegion(hps,fQDRegion,hrgn1,hrgn2,IMode);
	ReleasePresSpace(hps);
     }  
     if(fRgnTransform) {
	fRgnTransform->Release(ev); // 137748: AAA
	fRgnTransform = 0;
     }
     return fQDRegion;
  }
  else {
     THROW(kODErrInvalidGraphicsSystem);
     return kODNULL;
  }
}

#endif  // _PLATFORM_OS2_


#ifdef _PLATFORM_WIN32_

ODPlatformShape CompositeShape::GetPlatformShape( Environment*ev, ODGraphicsSystem system )
{
  if( ( system == kODWin32 ) || ( system == kODWinNT ) || ( system == kODWin95 ) ) {
     if ( fQDRegion && ( !fRgnTransform || fRgnTransform && fRgnTransform->GetType(ev) == kODIdentityXform ) );
     else if ( fQDRegion && fRgnTransform && fRgnTransform->GetType(ev) == kODTranslateXform ) {
        fQDRegion.Unlink();
        ODPoint offset;
        fRgnTransform->GetOffset( ev, &offset );
        Point ptOffset = offset.AsWinPoint();
        OffsetRgn( fQDRegion, ptOffset.x, ptOffset.y );
     }
     else {
	// Either we have no region, or the region transform is > translation.

	if (fQDRegion)
		fQDRegion.Unlink();
	else
		fQDRegion.InitRegion( CreateRectRgn( 0, 0, 0, 0 ) );

	HRGN hrgn1,hrgn2;
	LONG IMode;

	hrgn1 = shape1->GetPlatformShape( ev, kODWin32 );
	hrgn2 = shape2->GetPlatformShape( ev, kODWin32 );

	switch(this->op) {
		case kShapeIntersection:
			IMode = RGN_AND;
			break;
		case kShapeUnion:
			IMode = RGN_OR;
			break;
		case kShapeDifference:
			IMode = RGN_DIFF;
			break;
		default:
                        WARNMSG_DEBUG( WARN_INDEX(-1), "Bogus ShapeOp to combine" );
			THROW( kODErrInvalidParameter );
			break;
	}
	CombineRgn( fQDRegion, hrgn1, hrgn2, IMode );
     }  
     if(fRgnTransform) {
	fRgnTransform->Release(ev); // 137748: AAA
	fRgnTransform = 0;
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

ODPlatformShape CompositeShape::GetPlatformShape( Environment*ev, ODGraphicsSystem system )
{
  if( system == kODAIX ) {
     if ( fQDRegion && ( !fRgnTransform || fRgnTransform && fRgnTransform->GetType(ev) == kODIdentityXform ) );
     else if ( fQDRegion && fRgnTransform && fRgnTransform->GetType(ev) == kODTranslateXform ) {
        fQDRegion.Unlink();
        ODPoint offset;
        fRgnTransform->GetOffset( ev, &offset );
        Point ptOffset = offset.AsXPoint();
        OffsetRgn( fQDRegion, ptOffset.x, ptOffset.y );
     }
     else {
	// Either we have no region, or the region transform is > translation.

	if (fQDRegion)
		fQDRegion.Unlink();
	else
		fQDRegion.InitRegion( ODNewRgn() );

	ODRgnHandle hrgn1,hrgn2;

	hrgn1 = shape1->GetPlatformShape( ev, kODAIX );
	hrgn2 = shape2->GetPlatformShape( ev, kODAIX );

	switch(this->op) {
		case kShapeIntersection:
			SectRgn(hrgn1, hrgn2, fQDRegion);
			break;
		case kShapeUnion:
                        UnionRgn(hrgn1, hrgn2, fQDRegion);
			break;
		case kShapeDifference:
                        DiffRgn(hrgn1, hrgn2, fQDRegion);
			break;
		default:
                        WARNMSG_DEBUG( WARN_INDEX(-1), "Bogus ShapeOp to combine" );
			THROW( kODErrInvalidParameter );
			break;
	}
     }  
     if(fRgnTransform) {
	fRgnTransform->Release(ev); // 137748: AAA
	fRgnTransform = 0;
     }
     return fQDRegion;
  }
  else {
     THROW( kODErrInvalidGraphicsSystem );
     return kODNULL;
  }
}

#endif  // _PLATFORM_AIX_




ODBoolean
CompositeShape::IsSameAs( Environment* ev, RealShape *shape )
{
   ODBoolean empty1,empty2;

   if( shape == this )
      return kODTrue;

   empty1 = this->IsObviousEmpty( ev );
   empty2 = shape->IsObviousEmpty( ev );

   if( empty1 && empty2 ) 
      return kODTrue;

   if( fMode == kODNeedsGeometry ) {
      if( !shape->HasGeometry() ) 
         return kODFalse;
      this->InitQDPoly( ev );
      ODPolygon poly;
      shape->CopyPolygon( ev, poly );
      return( fPolygon == poly );
   }
#ifdef _PLATFORM_OS2_
   return EqualRgn( this->GetPlatformShape( ev, kODPM ), (ODRgnHandle)shape->GetPlatformShape( ev, kODPM ) );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   return EqualRgn( this->GetPlatformShape( ev, kODWin32 ), (ODRgnHandle)shape->GetPlatformShape( ev, kODWin32 ) );
#endif  // _PLATFORM_Win32_

#ifdef _PLATFORM_AIX_
   return EqualRgn( this->GetPlatformShape( ev, kODAIX ), (ODRgnHandle)shape->GetPlatformShape( ev, kODAIX ) );
  return kODTrue;
#endif  // _PLATFORM_AIX_
}


ODBoolean
CompositeShape::IsEmpty( Environment* ev )
{
   ODRect box;

   if( this->IsObviousEmpty(ev) ) 
      return kODTrue;
   this->GetBoundingBox( ev, &box );

   return box.IsEmpty();
}


ODBoolean
CompositeShape::IsObviousEmpty( Environment* ev )
{
  ODRect box;

  this->GetPesimisticBoundingBox( ev, &box );
  return box.IsEmpty();
}


ODBoolean
CompositeShape::ContainsPoint( Environment* ev, ODPoint point )
{
   if(fQDRegion)  {
      ODPoint pt = point;
      if ( fRgnTransform && fRgnTransform->GetType(ev) != kODIdentityXform  )
           fRgnTransform->InvertPoint( ev, &pt );  // [137664]
#ifdef _PLATFORM_OS2
      return PtInRgn( pt.AsQDPoint(), fQDRegion );
#endif 

#ifdef _PLATFORM_WIN32_
      return PtInRgn( pt.AsWinPoint(), fQDRegion );
#endif

#ifdef _PLATFORM_UNIX_
      return PtInRgn( pt.AsXPoint(), fQDRegion );
#endif
   }

   switch(this->op){

   case kShapeIntersection:
       return ( shape1->ContainsPoint( ev, point ) && shape2->ContainsPoint( ev, point ) );

   case kShapeUnion:
       return ( shape1->ContainsPoint( ev, point ) || shape2->ContainsPoint( ev, point ) );

   case kShapeDifference:
       return ( shape1->ContainsPoint( ev, point ) && !shape2->ContainsPoint( ev, point ) );

   default:
       WARNMSG_DEBUG( WARN_INDEX(-1), "Bogus ShapeOp to combine" );
       THROW( kODErrInvalidParameter );
       break;
   }
   return kODFalse;
}


ODBoolean
CompositeShape::IsRectangular( Environment* ev )
{
   ODRect box;
   ODBoolean empty = kODFalse;

   this->GetBoundingBox( ev, &box );
   RealShape* tempShape = (RealShape*) new RectShape( fMode, box );
   tempShape->Subtract( ev, this) ;
   empty = tempShape->IsEmpty( ev );
   delete tempShape;
   return empty;
}


ODBoolean
CompositeShape::IsObviousRectangular( Environment* ev )
{
   ODRect box1,box2;
   ODBoolean empty = kODFalse;
   ODBoolean isrect1, isrect2;

   isrect1 = shape1->IsObviousRectangular( ev );
   isrect2 =  shape2->IsObviousRectangular( ev );

   if( !isrect1 || !isrect2 ) 
      return kODFalse;

   if( op == kShapeIntersection ) 
      return kODTrue;

   shape1->GetPesimisticBoundingBox( ev, &box1 );
   shape2->GetPesimisticBoundingBox( ev, &box2 );

   if( box1.IsEmpty() || box2.IsEmpty() ) 
      return kODTrue;

   if( box2.Contains( box1 ) ) 
      return kODTrue;

   if( box1.Contains( box2 ) ) {
      if( op == kShapeDifference ) 
         return kODFalse;
      else 
         return kODTrue;
   }

   if( box1.Intersects( box2 ) ) 
      return kODFalse;
   else if( op == kShapeDifference ) 
      return kODTrue;

   return(kODFalse);
}


ODBoolean
CompositeShape::HasGeometry( )
{
   return ( shape1->HasGeometry() && shape2->HasGeometry() );
}


RealShape*
CompositeShape::Copy( Environment* ev )
{
   ODPolygon result;
   ODRgnHandle newRgn = 0;

   RealShape* newshap1 = shape1->Copy( ev );
   RealShape* newshap2 = shape2->Copy( ev );
   CompositeShape* newShape = new CompositeShape( newshap1, newshap2, op );

   if( fQDRegion ) {
      newShape->fQDRegion = fQDRegion;
   }

   if ( fRgnTransform )
      newShape->fRgnTransform = fRgnTransform->Copy( ev );

   newShape->fPolygon.CopyFrom( fPolygon );

   if ( fPolygonTransform )
      newShape->fPolygonTransform = fPolygonTransform->Copy( ev );

   if ( bboxValid ) {
     newShape->bbox = bbox;
     newShape->bboxValid = kODTrue;
   }
   return (RealShape *)newShape;
}


ODGeometryMode  GetMode( RealShape* shap1, RealShape* shap2 )
{
/*
  if ( shap1->HasGeometry() && shap2->HasGeometry()  )
    return kODPreserveGeometry;
  else
    return kODLoseGeometry;
*/
   ODGeometryMode mode1 = shap1->GetGeometryMode();
   ODGeometryMode mode2 = shap2->GetGeometryMode();

   if( mode1 == kODLoseGeometry || mode2 == kODLoseGeometry ) {
      if( mode1 == kODNeedsGeometry )
         THROW( kODErrNoShapeGeometry );
      return kODLoseGeometry;
   }
   return mode1;
}


RealShape*
CompositeShape::Transform( Environment *ev, ODTransform *xform )
{
   if ( xform->GetType(ev) != kODIdentityXform ) {
      shape1 = shape1->Transform( ev,xform );
      shape2 = shape2->Transform( ev,xform );

      if ( fPolygon.HasData() ) {
         if ( !fPolygonTransform )
            fPolygonTransform = xform->Copy( ev );
         else
            fPolygonTransform->PostCompose( ev, xform );
      }

      if ( fQDRegion ) {
         if ( !fRgnTransform )
            fRgnTransform = xform->Copy( ev );
         else
            fRgnTransform->PostCompose( ev, xform );
      }

      if ( bboxValid ) {
// AL: I've commented out the next line and replaced it with the code that follows :
//         xform->TransformRectangles(ev, &bbox, 1);

         if( xform->GetType(ev) > kODScaleTranslateXform ) 
            bboxValid = kODFalse;                   // cannot transform bounding box, invalidate
         else 
            TransformRectangle( ev, xform, &bbox );
      }

//    RealShape::Purge(0);

   }
   return this;
}


RealShape*
CompositeShape::Outset( Environment* ev, ODCoordinate distance )
{
   shape1->Outset( ev, distance );
   if( op == kShapeDifference ) 
      shape2->Outset( ev, -distance );
   else 
      shape2->Outset( ev, distance );

   RealShape::Purge( 0 );  // flush the region
   fPolygon.Clear();     // and the polygon

   if( fPolygonTransform ) {
      delete fPolygonTransform;
      fPolygonTransform = 0;
   }
   if( fRgnTransform ) {
      delete fRgnTransform;
      fRgnTransform = 0;
   }
   if( bboxValid ) {
      if( !bbox.IsEmpty() ) 
         bbox.Inset( -distance, -distance );
   }
   return this;
}


RealShape*
CompositeShape::Subtract( Environment* ev, RealShape *shape )
{
  if( shape==this )
    return this->Clear();
  else
    return this->Promote( ev, kShapeDifference, shape );
}


RealShape*
CompositeShape::Intersect( Environment* ev, RealShape *shape )
{
  if( shape==this )
    return this;
  else
    return this->Promote( ev, kShapeIntersection, shape );
}


RealShape*
CompositeShape::Union( Environment* ev, RealShape *shape )
{
  if( shape==this )
    return this;
  else
    return this->Promote( ev, kShapeUnion,shape );
}
