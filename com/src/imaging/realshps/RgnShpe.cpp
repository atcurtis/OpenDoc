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
/* @(#)Z 1.27 com/src/imaging/realshps/RgnShpe.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:10 (96/10/31 13:33:13) */
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
/*          3/19/96 jab  Changes for cross platform support         */
/*  144539 11/20/95 ced  Fix bug in Union method                    */
/*  122017  5/15/95 aml  Performance enhancements                   */
/*  121968  5/12/95 ced  Fix bug in Outset method                   */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    RgnShpe.cpp

  Contains:  RgnShape class, private to ODShape.

  Written by:  Jens Alfke

  Copyright:  ­ 1993-94 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

    <10>   8/29/95  jpa    Throw kODErrInvalidPlatformShape if NULL
 				region passed in. [12788284]
     <9>   8/16/95  NP     1274946: ErrorDef.idl problems. Add include
				file.
     <8>    8/8/95  jpa    Got rid of CATCH(): not native-exceptions
				compatible. [1268187]
     <7>   12/5/94  jpa    Implemented CopyGXShape. [1195654]
     <6>  10/24/94  jpa    Use RealShape::NewGeometricShape to
                  promote. [1195070]
     <5>   9/29/94  RA    1189812: Mods for 68K build.
     <4>   9/14/94  jpa    Outset converts distance to short before
                  calling InsetRgn. [1183857]
     <3>    8/8/94  jpa    Added Outset method [1178690]
     <2>   6/18/94  MB    Update memory includes
     <1>   6/15/94  jpa    first checked in

  In Progress:
*/
#ifdef _PLATFORM_OS2_
#define INCL_GPI
#define INCL_WIN
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
#include <wtypes.h>
#include <wingdi.h>
#endif


#ifndef _ALTPOINT_
#include "AltPoint.h"      /* Use C++ savvy ODPoint and ODRect*/
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif
#endif

#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifndef _RGNSHPE_
#include "RgnShpe.h"
#endif

#ifndef _POLYSHPE_
#include "PolyShpe.h"
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

#ifndef _ODMATH_
#include <ODMath.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

#ifndef _BARRAY_
#include "BArray.h"
#endif

extern void TransformRectangle(Environment* ev, ODTransform *xform, ODRect *r);


#ifdef RGNDEBUG

#include "qprintf.h"

#ifdef _PLATFORM_OS2_
void OS2DumpRegion(HPS hps, HRGN hrgn);
int OS2GetRegionRects(HPS hps, HRGN hrgn, PRECTL *pprcl);
#endif

#ifdef _PLATFORM_WIN32_
void DumpRegion(HRGN hrgn);
int GetRegionRects(HRGN hrgn, PRECTL *pprcl);
#endif

#endif  // RGNDEBUG

#endif   // IBM Platforms


#include <TempObj.h>


RgnShape::RgnShape( ODGeometryMode mode )
   :RealShape(mode),
    fPolygon()
{
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
   fTransform = 0;
   bbox.Clear();
   bboxValid = FALSE;
#endif   // IBM Platforms

#if ODDebug
   fType = 1;
#endif
}



RgnShape::~RgnShape( )
{
   this->Purge(0);
   if ( fTransform ) {
      // Use ODSafeReleaseObject here because we don't have the SOM
      // Environment in this method.  ODSafeReleaseObject uses the
      // SOM Global Environment in it's call to ODTransform::Release.
      ODSafeReleaseObject( fTransform );
   }
}



ODSize
RgnShape::Purge( ODSize reqsize )
{
  // Don't dispose the region like my parent class!

   // Purge gets rid of the polygon, transform and bounding box private
   // data if the polygon (fPolygon) actually has data.  The region
   // (fQDRegion) definition is left intact.
   if( fPolygon.HasData() ) {
      ODSize size = fPolygon.GetDataSize();
      fPolygon.Clear();

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
      reqsize -= size;
      if ( fTransform ) {
         Environment* ev = somGetGlobalEnvironment();
         size += fTransform->Purge( ev, reqsize );
         // Use ODSafeReleaseObject here because we don't have the SOM
         // Environment in this method.  ODSafeReleaseObject uses the
         // SOM Global Environment in it's call to ODTransform::Release.
         ODSafeReleaseObject( fTransform );
         fTransform = kODNULL;
      }
      bbox.Clear();
      bboxValid = FALSE;
#endif   // IBM Platforms

      return size;
   } else
      return 0;
}



void
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::GetBoundingBox( Environment* ev, ODRect *bounds )
#else
RgnShape::GetBoundingBox( ODRect *bounds )
#endif   // IBM Platforms
{
   if( fQDRegion ) {

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
      if( bboxValid ) {
         *bounds = bbox;
         return;
      }

#ifdef _PLATFORM_OS2_

      RECTL box;
      HPS hps = GetPresSpace();
      GpiQueryRegionBox(hps, fQDRegion, &box);

#endif

#ifdef _PLATFORM_WIN32_

      Rect box;
      LONG lComplexity = GetRgnBox( fQDRegion, &box );

#endif   // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_

      Rect box;
      XClipBox(fQDRegion, &box);
#endif   // _PLATFORM_UNIX_

      bbox = box;

      // fTransform is a cached transform set in the Transform method.
      // It is not applied to any of the private data until we are ready
      // to access the transformed, private data.  Well, this is one
      // of those times.  If we have a cached xform, apply it to the
      // bounding box before returning it.
      if (fTransform) {
         ODTransformType Xtype = fTransform->GetType(ev);
         if( Xtype == kODIdentityXform )
            ;
         else if( Xtype <= kODScaleTranslateXform ) {  // transform bounding box
            TransformRectangle( ev, fTransform, &bbox );
         }
         else {  // transform polygon and get its bounding box
            if( !fPolygon.HasData() ) {

#ifdef _PLATFORM_OS2_
               Rgn2Poly( hps, fQDRegion, fPolygon );
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
               Rgn2Poly( fQDRegion, fPolygon );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
               Rgn2Poly( fQDRegion, fPolygon );
#endif  // _PLATFORM_UNIX_

               fPolygon.Transform( ev, fTransform );
            }

#ifdef _PLATFORM_OS2_
            fQDRegion = fPolygon.AsRegion( hps );
#endif

#ifdef _PLATFORM_WIN32_
            fQDRegion = fPolygon.AsWinRegion( );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
            fQDRegion = fPolygon.AsXRegion( );
#endif  // _PLATFORM_UNIX_

            ODReleaseObject( ev, fTransform );
            fPolygon.ComputeBoundingBox( &bbox );
         }
      }

      *bounds = bbox;
#ifdef _PLATFORM_OS2_
      ReleasePresSpace(hps);
#endif


#else  // else part of 1st IBM Platforms

      Rect bbox = (**fQDRegion).rgnBBox;
      *bounds = bbox;

#endif    // IBM Platforms


   } else {            // else from if(fQDRegion)

      bounds->Clear();

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

      bbox.Clear();
   }
   bboxValid = TRUE;

#else

   }

#endif

}



// AML: GetPesimisticBoundingBox
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
void
RgnShape::GetPesimisticBoundingBox( Environment* ev, ODRect *bounds )
{
   // this method gets an approximate bounding box which can be much
   // larger than a bounding box calculated by converting to a polygon,
   // transforming the polygon then getting the bounding box from the
   // transformed polygon.

   if( bboxValid ) {
      *bounds = bbox;
      return;
   }
   bboxValid = TRUE;
   if( fQDRegion ) {

#ifdef _PLATFORM_OS2_
      RECTL box;
      HPS hps = GetPresSpace();
      GpiQueryRegionBox( hps, fQDRegion, &box );
      ReleasePresSpace( hps );
#endif

#ifdef _PLATFORM_WIN32_

      Rect box;
      LONG lComplexity = GetRgnBox( fQDRegion, &box );

#endif   // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_

      Rect box;
      XClipBox(fQDRegion, &box);
#endif   // _PLATFORM_UNIX_

#ifdef _PLATFORM_MACINTOSH_
#endif // _PLATFORM_MACINTOSH_

      bbox = box;
      if ( fTransform ) {
         ODTransformType Xtype = fTransform->GetType(ev);
         if( Xtype == kODIdentityXform )
            ;
         else if( Xtype <= kODScaleTranslateXform ) {
            TransformRectangle( ev, fTransform, &bbox );
         }
         else if( fPolygon.HasData() ) {  // I have the polygon
            fPolygon.ComputeBoundingBox( &bbox );
         }
         else  { // get the bounding box of the transformed bbox

            // This is the case where the polygon has no data and the
            // xform is not a simple scale and/or translate matrix.
            // The bbox computed by this method can be much larger than
            // the bounding box computed by more normal methods. bboxValid
            // is marked FALSE in this case.
            ODPolygon poly;
            poly.SetRect( bbox );
            poly.Transform( ev, fTransform );
            poly.ComputeBoundingBox( &bbox );
            bboxValid = FALSE;
         }
      }
   } else
     bbox.Clear();
  *bounds = bbox;
}
#endif   //  IBM Platforms




RealShape*
RgnShape::SetRectangle( const ODRect *r )
{
   if( !fQDRegion ) {
      fQDRegion = ODNewRgn();
// The OS/2 compiler doesn't like the next line. jab 05/20/96
//      THROW_IF_NULL( fQDRegion );
        ASSERT_NOT_NULL( fQDRegion );
   }

   Rect qdr;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

 #ifdef _PLATFORM_OS2_

   r->AsQDRect( qdr );
   fQDRegion.Unlink();
   RectRgn( fQDRegion, &qdr );

 #endif  // _PLATFORM_OS2_

 #ifdef _PLATFORM_WIN32_

   r->AsWinRect( qdr );
   fQDRegion.Unlink();
   fQDRegion = CreateRectRgn( qdr.left, qdr.top, qdr.right, qdr.bottom );

 #endif  // _PLATFORM_WIN32_

 #ifdef _PLATFORM_UNIX_

   r->AsXRect( qdr );
   fQDRegion.Unlink();
   Region newRgn = XCreateRegion();
   XUnionRectWithRegion(&qdr, fQDRegion, newRgn);
   fQDRegion = newRgn;

 #endif  // _PLATFORM_UNIX_

   this->Purge(0);

   if ( fTransform ) {
      // Use ODSafeReleaseObject here because we don't have the SOM
      // Environment in this method.  ODSafeReleaseObject uses the
      // SOM Global Environment in it's call to ODTransform::Release.
      ODSafeReleaseObject( fTransform );
      fTransform = kODNULL;
   }

   if( bboxValid ) {
      bboxValid = FALSE;
      bbox.Clear();
   }

#endif   // IBM PLATFORMS


#ifdef _PLATFORM_MACINTOSH_

   r->AsQDRect( qdr );
   RectRgn( fQDRegion, &qdr );
   this->Purge(0);

#endif // _PLATFORM_MACINTOSH_

   return this;
}


RealShape*
RgnShape::Clear( )
{
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

   fQDRegion.Unlink();
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
   SetRectRgn( fQDRegion, 0, 0, 0, 0 );
#endif

#if defined(_PLATFORM_UNIX_)
   SetRectRgn( fQDRegion, 0, 0, 0, 0 );
#endif

   this->Purge(0);

   if ( fTransform ) {
      // Use ODSafeReleaseObject here because we don't have the SOM
      // Environment in this method.  ODSafeReleaseObject uses the
      // SOM Global Environment in it's call to ODTransform::Release.
      ODSafeReleaseObject( fTransform );
      fTransform = kODNULL;
   }

   if( bboxValid ) {
      bboxValid = FALSE;
      bbox.Clear();
   }

#else

   SetRectRgn( fQDRegion, 0, 0, 0, 0 );
   this->Purge(0);

#endif   // IBM Platforms

   return this;
}


void
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
RgnShape::CopyPolygon( Environment* ev, ODPolygon &poly )
#else
RgnShape::CopyPolygon( ODPolygon &poly )
#endif   // IBM Platforms
{
   if( !fPolygon.HasData() ) {

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

 #ifdef _PLATFORM_OS2_

      HPS hps = GetPresSpace();
      Rgn2Poly( hps, fQDRegion,fPolygon );
      ReleasePresSpace( hps );

 #endif  //  _PLATFORM_OS2_

 #ifdef _PLATFORM_WIN32_

      Rgn2Poly( fQDRegion, fPolygon );

 #endif  // _PLATFORM_WIN32_

 #ifdef _PLATFORM_UNIX_

      Rgn2Poly( fQDRegion, fPolygon );

 #endif  // _PLATFORM_UNIX_

   }

   // We want to leave fPolygon untransformed since our working state
   // is that we either have the regions up to date (all xforms have
   // already been applied and fTransform is NULL) or that we have
   // a cached xform and the regions, bounding box & polygon need to
   // be xformed before working with them. (fTransform is not NULL)

   // Copy fPolygon and, if necessary, apply the cached xform to
   // the copy.
   poly.CopyFrom( fPolygon );

   if ( fTransform && fTransform->GetType(ev) != kODIdentityXform)  {
      poly.Transform( ev, fTransform );
   }

#endif  // _PLATFORM_WIN32_


#ifdef _PLATFORM_MACINTOSH_

      Rgn2Poly( fQDRegion,fPolygon );

   }

   poly.CopyFrom( fPolygon );

#endif  // _PLATFORM_MACINTOSH_


}



RealShape*
RgnShape::SetPolygon( const ODPolygon &poly )
{

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

 #ifdef _PLATFORM_OS2_

   HPS hps = GetPresSpace();
   ODRgnHandle rgn = poly.AsRegion( hps );
   ReleasePresSpace( hps );

 #endif  // _PLATFORM_OS2_

 #ifdef _PLATFORM_WIN32_

   ODRgnHandle rgn = poly.AsWinRegion();

 #endif  // _PLATFORM_WIN32_

 #ifdef _PLATFORM_UNIX_

   ODRgnHandle rgn = poly.AsXRegion();

 #endif  // _PLATFORM_UNIX_

   fQDRegion.Unlink();
   this->Purge(0);

   if ( fTransform ) {
      // Use ODSafeReleaseObject here because we don't have the SOM
      // Environment in this method.  ODSafeReleaseObject uses the
      // SOM Global Environment in it's call to ODTransform::Release.
      ODSafeReleaseObject( fTransform );
      fTransform = kODNULL;
   }

   if( bboxValid ) {
      bboxValid = FALSE;
      bbox.Clear();
   }

#endif  //  IBM PLATFORMS


#ifdef _PLATFORM_MACINTOSH_

   RgnHandle rgn = poly.AsQDRegion();
   this->Purge(0);

#endif  // _PLATFORM_MACINTOSH_


   fQDRegion = rgn;

   TRY{
      fPolygon.CopyFrom( poly );

   }CATCH_ALL{
      if ( ErrorCode() == kODErrOutOfMemory )
         fPolygon.Clear();
      else
        RERAISE;

   }ENDTRY

   return this;
}



void
RgnShape::InitQDRegion( Environment* )
{
}



#ifdef _PLATFORM_MACINTOSH_ // CED
gxShape

RgnShape::CopyGXShape( )
{
   if( !fPolygon.HasData() )
      Rgn2Poly( fQDRegion,fPolygon );
   return fPolygon.AsGXShape();
}
#endif // _PLATFORM_MACINTOSH_



void
RgnShape::SetPlatformShape( ODGraphicsSystem system, ODPlatformShape shape )
{

#ifdef _PLATFORM_MACINTOSH_

   ASSERT( system == kODQuickDraw, kODErrAssertionFailed );
   if( shape==kODNULL )
      THROW(kODErrInvalidPlatformShape);

   RealShape::Purge(0);
   fQDRegion = (RgnHandle)shape;

#endif   // _PLATFORM_MACINTOSH_


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

 #ifdef _PLATFORM_OS2_

   if( system == kODQuickDraw ) {

 #endif   // _PLATFORM_OS2_

 #ifdef _PLATFORM_WIN32_

   if( ( system == kODWin32 ) || ( system == kODWinNT ) || ( system == kODWin95 ) ) {

 #endif   // _PLATFORM_WIN32_

 #ifdef _PLATFORM_AIX_

   if( system == kODAIX ) {

 #endif   // _PLATFORM_AIX_

      fQDRegion.Unlink();
      this->Purge(0);

      if( bboxValid ) {
         bboxValid = FALSE;
         bbox.Clear();
      }

      if ( fTransform ) {
         // Use ODSafeReleaseObject here because we don't have the SOM
         // Environment in this method.  ODSafeReleaseObject uses the
         // SOM Global Environment in it's call to ODTransform::Release.
         ODSafeReleaseObject( fTransform );
         fTransform = kODNULL;
      }

      fQDRegion = (ODRgnHandle)shape;

   } else
     THROW( kODErrInvalidGraphicsSystem );

#endif   //  IBM PLATFORMS

}



#ifdef _PLATFORM_OS2_ // CED

ODPlatformShape RgnShape::GetPlatformShape( Environment*ev, ODGraphicsSystem system )
{

   if( system == kODQuickDraw ) {
      ASSERT_NOT_NULL( fQDRegion );
      ODTransformType Xtype;

      ODPoint scale;
      if( fTransform ) {
         fTransform->GetScale( ev, &scale ); 
      }
   
      if (fTransform && ( Xtype = fTransform->GetType( ev ) ) != kODIdentityXform ) {
         fQDRegion.Unlink();
         HPS hps = GetPresSpace();

         if ( Xtype == kODTranslateXform ) {

            // The xform is a simple translate.  Find the offset and
            // and translate the region.
            ODPoint offset;
            fTransform->GetOffset( ev, &offset );
            POINTL ptlOffset = offset.AsPOINTL();

            BOOL result = GpiOffsetRegion( hps, fQDRegion, &ptlOffset );

            if( result == 0 ) {
               WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::GetPlatformShape - GpiOffsetRgn failed."); 
            }

            // Purge the polygon data since it is no longer current after
            // transforming the region data.
            this->Purge( 0 );

         }
         else if ( ( Xtype <= kODLinearXform ) && 
                    ( scale.x >= 0.0 ) && ( scale.y >= 0.0 ) )  {

            // Transform is scale or scale + offset *and* the scale factors
            // do not give us a situation where the transformed rectangles have
            // a lower left corner with coordinate values greater than the
            // the upper right corner (rotations of 180 degrees can produce
            // such rectangles).  GpiSetRegion expects the input rectangles
            // to have lower left < upper right and will fail if otherwise. 


            // The first call to GpiQueryRegionRects is used to figure out how many 
            // rectangles define this region.  We'll malloc memory based on
            // the crcReturned field.  The 2nd call actually gets the data.
            // NOTE: There are occasions where crcReturned from the 1st call
            // is larger than crcReturned from the 2nd call.  This looks like
            // a GPI bug.  Fortunately crcReturned from the 2nd call has
            // always been smaller so we don't overrun our allocated memory.
            RGNRECT rgnrcControl = {1, 0, 0, RECTDIR_LFRT_BOTTOP};
            int nRects;
            GpiQueryRegionRects( hps, fQDRegion, NULL, &rgnrcControl, NULL );

#ifdef RGNDEBUG
            qprintf("\n  RgnShape::GetPlatformShape - region before xform");
            OS2DumpRegion(hps, fQDRegion);
#endif
            nRects = rgnrcControl.crcReturned;

            if ( nRects > 0 ) {
               PRECTL arcl = (PRECTL) malloc( nRects * sizeof(RECTL) );
               ODRect *aRect = (ODRect*)malloc( nRects * sizeof(ODRect) );

               rgnrcControl.crcReturned = 0;
               rgnrcControl.crc = nRects;

               GpiQueryRegionRects( hps, fQDRegion, NULL, &rgnrcControl, arcl );


               if( rgnrcControl.crcReturned > nRects ) {
                  WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::GetPlatformShape - GpiQueryRegionRects returned too many rects."); 
               }
               else if( rgnrcControl.crcReturned == 0 ) {
                  WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::GetPlatformShape - GpiQueryRegionRects returned 0 rects."); 
               }
               else {
                  // Get the actual number of rectangles from the 2nd call
                  // and convert RECTLs to ODRects.
                  nRects = rgnrcControl.crcReturned;

                  for ( register int i = 0; i < nRects; i++ )
                     aRect[i] = arcl[i];

                  // Transform the points and convert back to RECTLs
                  ODByteArray points;
                  UseByteArray( &points, aRect, nRects*2 );
                  fTransform->TransformPoints( ev, &points, nRects*2 );

                  for ( i = 0; i < nRects; i++ )
                     aRect[i].AsRECTL(arcl[i]);

                  // Redefine the region using the transformed rectangles.
                  BOOL result = GpiSetRegion( hps, fQDRegion, nRects, arcl );

                  if( result == 0 ) {
                      WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::GetPlatformShape - GpiSetRegion failed."); 
                  }
               }
#ifdef RGNDEBUG
               qprintf("\n  RgnShape::GetPlatformShape - region after xform");
               OS2DumpRegion(hps, fQDRegion);
#endif

               free( arcl );
               free( aRect );
            }

            // Purge the polygon data since it is no longer current after
            // transforming the region data.
            this->Purge( 0 );

         } else {

            // The xform is more complicated than a simple scale and/or
            // translate. We have no choice now but to convert to polygon
            // and transform the polygon.  We can't rotate the series of
            // rectangles which comprise the region.
            //
            if( !fPolygon.HasData() )
               Rgn2Poly( hps, fQDRegion, fPolygon );

            if( fTransform )
               fPolygon.Transform( ev, fTransform );

            // Now convert it back to regions.
            fQDRegion = fPolygon.AsRegion( hps );
         }
         ReleasePresSpace( hps );

         if( fTransform ) {
            ODReleaseObject( ev, fTransform );
            fTransform = kODNULL;
         }

         if( bboxValid ) {
	    bboxValid = FALSE;
            bbox.Clear();
         }
      }
      return fQDRegion;
   }
   else {
      THROW( kODErrInvalidGraphicsSystem );
      return kODNULL;
   }

}
#endif // _PLATFORM_OS2_



#ifdef _PLATFORM_WIN32_
ODPlatformShape RgnShape::GetPlatformShape( Environment*ev, ODGraphicsSystem system )
{

   if( ( system == kODWin32 ) || ( system == kODWinNT ) ||( system == kODWin95 ) ) {
      ASSERT_NOT_NULL( fQDRegion );
      ODTransformType Xtype;

      if ( fTransform && ( Xtype = fTransform->GetType(ev)) != kODIdentityXform ) {

         if ( Xtype == kODTranslateXform ) {

            // The xform is a simple translate.  Find the offset and
            // and translate the region.
            ODPoint offset;
            fTransform->GetOffset( ev, &offset );
            Point ptlOffset = offset.AsWinPoint();

            fQDRegion.Unlink();
            int result = OffsetRgn( fQDRegion, ptlOffset.x, ptlOffset.y );

            if( result == ERROR ) {
               WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::GetPlatformShape - OffsetRgn failed."); 
            }

            // Purge the polygon data since it is no longer current after
            // transforming the region data.
            this->Purge( 0 );

         }
         else if ( Xtype <= kODLinearXform ) {

            // Transform is scale or scale + offset, so transform the
            // region rectangles.

            // Get the number of rectangles which make up this region
            LPRGNDATA lpRgn;

            // If dwSize = 0 then an error occurred while getting the
            // size of memory needed to hold the region data. Otherwise,
            // dwSize = 1 if successful, and equal to the size, in bytes, 
            // of the required buffer.

            DWORD dwSize = GetRegionData( fQDRegion, 0, NULL );
            if( dwSize < 1 ) {
               WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::GetPlatformShape - GetRegionData failed.");
               return fQDRegion;
            }

            lpRgn = (LPRGNDATA) malloc( dwSize );
            dwSize = GetRegionData( fQDRegion, dwSize, lpRgn );


#ifdef RGNDEBUG
            RECT brect;
            DWORD nRects = lpRgn->rdh.nCount;
            LPRECT ptRect = (LPRECT)lpRgn->Buffer;
            GetRgnBox(fQDRegion, &brect);
            qprintf("RgnShpe::GetPlatformShape before cached xform applied \n");
            qprintf("   pointer to object=%x nrects=%i \n  \
                  brect(left,right,top,bot)=(%i,%i,%i,%i) \n \
                  rect[0]=(%i,%i,%i,%i)\t rect[nRects]=(%i,%i,%i,%i)\n\n",
               this, nRects, brect.left, brect.right, brect.top, brect.bottom,
               ptRect[0].left, ptRect[0].right, ptRect[0].top, ptRect[0].bottom,
               ptRect[nRects-1].left, ptRect[nRects-1].right,
               ptRect[nRects-1].top, ptRect[nRects-1].bottom);

// Less detailed dump
//            qprintf("\n  RgnShape::GetPlatformShape - region before xform");
//            DumpRegion(fQDRegion);

#endif  // RGNDEBUG


            ODMatrix matrix;
            fTransform->GetMatrix( ev, &matrix );
            const XFORM plat_xform = { ((double)matrix.m[0][0])/65536.0,
                                       0.0,
                                       0.0,
                                       ((double)matrix.m[1][1])/65536.0,
                                       ((double)matrix.m[2][0])/65536.0,
                                       ((double)matrix.m[2][1])/65536.0 };

            HRGN tmpRgn = ExtCreateRegion( &plat_xform, dwSize, lpRgn);

            if( !tmpRgn ) {
               WARNMSG_DEBUG( WARN_INDEX(-1), " RgnShape::GetPlatformShape - ExtCreateRegion failed ");
               free( lpRgn );
               return fQDRegion;
            }

            fQDRegion.Unlink();
            int result = CombineRgn( fQDRegion, tmpRgn, 0, RGN_COPY );

            free( lpRgn );
            DeleteObject( tmpRgn );

            if ( result == ERROR ) { 
               WARNMSG_DEBUG( WARN_INDEX(-1), " RgnShape::GetPlatformShape - CombineRgn failed, i = 0 ");

               return fQDRegion;
            }

            // Everything successful so far.  Purge the polygon data since 
            // it is no longer current after transforming the region data.
            this->Purge( 0 );


#ifdef RGNDEBUG

// Less detailed dump
//            qprintf("\n  RgnShape::GetPlatformShape - region after xform");
//            DumpRegion(fQDRegion);


            qprintf( " transform matrix (float)= |%f %f %f|\n"
                        "                           |%f %f %f|\n"
                        "                           |%f %f %x|\n\n",
                        plat_xform.eM11, plat_xform.eM12, 0,
                        plat_xform.eM21, plat_xform.eM22, 0, plat_xform.eDx,
                        plat_xform.eDy, matrix.m[2][2] );

            dwSize = GetRegionData( fQDRegion, 0, NULL );
            if( dwSize > 1 ) {
               LPRGNDATA tmplpRgn;
               tmplpRgn = (LPRGNDATA) malloc( dwSize );
               dwSize = GetRegionData( fQDRegion, dwSize, tmplpRgn );
               nRects = tmplpRgn->rdh.nCount;
               ptRect = (LPRECT)tmplpRgn->Buffer;
               GetRgnBox( fQDRegion, &brect );
               qprintf("RgnShpe::GetPlatformShape after cached xform applied \n");
               qprintf("type of region = %i pointer to object = %x nrects = %i \n \
                        bbox(left,right,top,bot) = (%i,%i,%i,%i) \n \
                        rect[0] = (%i,%i,%i,%i)\t rect[nRects] = (%i,%i,%i,%i)\n\n",
                        result, this, nRects, brect.left, brect.right,
                        brect.top, brect.bottom, ptRect[0].left, 
                        ptRect[0].right, ptRect[0].top, ptRect[0].bottom,
                        ptRect[nRects-1].left, ptRect[nRects-1].right,
                        ptRect[nRects-1].top, ptRect[nRects-1].bottom);
               free( tmplpRgn );
            }
            else {
               WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::GetPlatformShape - GetRegionData failed.");
            }
#endif  //RGNDEBUG


         } else {

            // The transform is not a simple scale and/or translate.  We have
            // no choice now but to convert to polygon and transform the
            // polygon.  We can't rotate the series of rectangles which
            // comprise the region.
            if( !fPolygon.HasData() )
               Rgn2Poly( fQDRegion, fPolygon );

            if( fTransform )
               fPolygon.Transform( ev, fTransform );

            // Convert the polygon back into a region.
            fQDRegion.Unlink();
            fQDRegion = fPolygon.AsWinRegion();

         }

         if( fTransform ) {
            ODReleaseObject( ev, fTransform );
            fTransform = kODNULL;
         }

         if( bboxValid ) {
	    bboxValid = FALSE;
            bbox.Clear();
         }
      }
      return fQDRegion;
   }
   else {
      THROW( kODErrInvalidGraphicsSystem );
      return kODNULL;
   }

}
#endif // _PLATFORM_WIN32_



#ifdef _PLATFORM_AIX_

ODPlatformShape RgnShape::GetPlatformShape( Environment*ev, ODGraphicsSystem system )
{

   if( system == kODAIX ) {
      ASSERT_NOT_NULL( fQDRegion );
      ODTransformType Xtype;

      if ( fTransform && ( Xtype = fTransform->GetType(ev)) != kODIdentityXform ) {
         fQDRegion.Unlink();

         if ( Xtype == kODTranslateXform ) {

            // The xform is a simple translate.  Find the offset and
            // and translate the region.
            ODPoint offset;
            fTransform->GetOffset( ev, &offset );
            Point ptlOffset = offset.AsXPoint();

            OffsetRgn( fQDRegion, ptlOffset.x, ptlOffset.y );

            // Purge the polygon data since it is no longer current after
            // transforming the region data.
            this->Purge( 0 );

         }
         else if ( Xtype <= kODLinearXform ) {

            // Transform is scale or scale + offset, so transform the
            // region rectangles.
            XRectangle* rectangles;

            int rect_count = GetRegionRects( fQDRegion, &rectangles );

            Region new_plat_shape = XCreateRegion();

            ODPoint point1, point2;

            for (long index = 0; index < rect_count; index++) {
              point1.x = IntToFixed(rectangles[index].x);
              point1.y = IntToFixed(rectangles[index].y);
              point2.x = point1.x + IntToFixed(rectangles[index].width);
              point2.y = point1.y + IntToFixed(rectangles[index].height);
              fTransform->TransformPoint( ev, &point1 );
              fTransform->TransformPoint( ev, &point2 );
              rectangles[index].width = FixedToInt(point2.x - point1.x);
              rectangles[index].height = FixedToInt(point2.y - point1.y);
              rectangles[index].x = FixedToInt(point1.x);
              rectangles[index].y = FixedToInt(point1.y);
              
              XRectangle* rect = &(rectangles[index]);
              XUnionRectWithRegion(rect, new_plat_shape, new_plat_shape);
            }

            XFree(rectangles);

            fQDRegion = new_plat_shape;

            // Purge the polygon data since it is no longer current after
            // transforming the region data.
            this->Purge( 0 );

         } else {

            // The transform is not a simple scale and/or translate.  We have
            // no choice now but to convert to polygon and transform the
            // polygon.  We can't rotate the series of rectangles which
            // comprise the region.
            if( !fPolygon.HasData() )
               Rgn2Poly( fQDRegion, fPolygon );

            if( fTransform )
               fPolygon.Transform( ev, fTransform );

            // Convert the polygon back into a region.
            fQDRegion = fPolygon.AsXRegion();

         }

         if( fTransform ) {
            ODReleaseObject( ev, fTransform );
            fTransform = kODNULL;
         }

         if( bboxValid ) {
	    bboxValid = FALSE;
            bbox.Clear();
         }

      }
      return fQDRegion;
   }
   else {
      THROW( kODErrInvalidGraphicsSystem );
      return kODNULL;
   }

}
#endif // _PLATFORM_AIX_




ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::IsSameAs( Environment* ev, RealShape *shape )
#else
RgnShape::IsSameAs( RealShape *shape )
#endif    // IBM Platforms
{
   
   ASSERT_NOT_NULL( fQDRegion );
   if( shape==this )
      return kODTrue;


#ifdef _PLATFORM_OS2_

   ODBoolean empty = this->IsObviousEmpty( ev );
   ODBoolean shapeEmpty = shape->IsObviousEmpty( ev );

   if( empty || shapeEmpty )
      return empty==shapeEmpty;

   ODBoolean equal;
   if ( fTransform && fTransform->GetType( ev ) != kODIdentityXform ) {
      ODTransform* xform = fTransform->Copy( ev )->Invert( ev );
      RealShape* testShape = shape->Copy( ev )->Transform( ev, xform );
      equal = EqualRgn( fQDRegion, (ODRgnHandle)testShape->GetPlatformShape( ev, kODPM ) );
      delete testShape;
      // Use ODSafeReleaseObject here because we don't have the SOM
      // Environment in this method.  ODSafeReleaseObject uses the
      // SOM Global Environment in it's call to ODTransform::Release.
      ODSafeReleaseObject( xform );
   }
   else
      equal = EqualRgn(fQDRegion, (ODRgnHandle)shape->GetPlatformShape(ev, kODPM));
   return equal;

#endif   // _PLATFORM_OS2_


#ifdef _PLATFORM_WIN32_

   ODBoolean empty = this->IsObviousEmpty( ev );
   ODBoolean shapeEmpty = shape->IsObviousEmpty( ev );

   if( empty || shapeEmpty )
      return empty==shapeEmpty;

   ODBoolean equal;
   if ( fTransform && fTransform->GetType( ev ) != kODIdentityXform ) {
      ODTransform* xform = fTransform->Copy( ev )->Invert( ev );
      RealShape* testShape = shape->Copy( ev )->Transform( ev, xform );
      equal = EqualRgn( fQDRegion, (ODRgnHandle)testShape->GetPlatformShape( ev, kODWin32 ) );
      delete testShape;
      // Use ODSafeReleaseObject here because we don't have the SOM
      // Environment in this method.  ODSafeReleaseObject uses the
      // SOM Global Environment in it's call to ODTransform::Release.
      ODSafeReleaseObject( xform );
   }
   else
      equal = EqualRgn( fQDRegion, (ODRgnHandle)shape->GetPlatformShape( ev, kODWin32 ) );
   return equal;

#endif   // _PLATFORM_WIN32_


#ifdef _PLATFORM_AIX_

   ODBoolean empty = this->IsObviousEmpty( ev );
   ODBoolean shapeEmpty = shape->IsObviousEmpty( ev );

   if( empty || shapeEmpty )
      return empty==shapeEmpty;

   ODBoolean equal;
   if ( fTransform && fTransform->GetType( ev ) != kODIdentityXform ) {
      ODTransform* xform = fTransform->Copy( ev )->Invert( ev );
      RealShape* testShape = shape->Copy( ev )->Transform( ev, xform );
      equal = EqualRgn( fQDRegion, (ODRgnHandle)testShape->GetPlatformShape( ev, kODAIX ) );
      delete testShape;
      // Use ODSafeReleaseObject here because we don't have the SOM
      // Environment in this method.  ODSafeReleaseObject uses the
      // SOM Global Environment in it's call to ODTransform::Release.
      ODSafeReleaseObject( xform );
   }
   else
      equal = EqualRgn( fQDRegion, (ODRgnHandle)shape->GetPlatformShape( ev, kODAIX ) );
   return equal;

#endif   // _PLATFORM_AIX_


#ifdef _PLATFORM_MACINTOSH_

   ODBoolean empty = this->IsEmpty();
   ODBoolean shapeEmpty = shape->IsEmpty();

   if( empty || shapeEmpty )
      return empty==shapeEmpty;

   return EqualRgn( fQDRegion, (RgnHandle)shape->GetPlatformShape( kODQuickDraw ) );

#endif   // _PLATFORM_MACINTOSH_


}



ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::IsEmpty( Environment* ev )
#else
RgnShape::IsEmpty( )
#endif  // IBM Platforms
{
   ASSERT_NOT_NULL( fQDRegion );
   return EmptyRgn( fQDRegion );
}


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::ContainsPoint( Environment* ev, ODPoint point )
#else
RgnShape::ContainsPoint( ODPoint point )
#endif   // IBM Platforms
{
   ASSERT_NOT_NULL( fQDRegion );

#ifdef _PLATFORM_OS2_

   if ( fTransform && fTransform->GetType(ev) != kODIdentityXform )
      fTransform->InvertPoint( ev, &point );  // [137664]

   return PtInRgn( point.AsQDPoint(), fQDRegion );

#endif // _PLATFORM_OS2_


#ifdef _PLATFORM_WIN32_

   if ( fTransform && fTransform->GetType(ev) != kODIdentityXform )
      fTransform->InvertPoint( ev, &point );  // [137664]

   return PtInRgn( point.AsWinPoint(), fQDRegion );

#endif   // _PLATFORM_WIN32_


#ifdef _PLATFORM_UNIX_

   if ( fTransform && fTransform->GetType(ev) != kODIdentityXform )
      fTransform->InvertPoint( ev, &point );  // [137664]

   return PtInRgn( point.AsXPoint(), fQDRegion );

#endif   // _PLATFORM_UNIX_


#ifdef _PLATFORM_MACINTOSH_

   return PtInRgn( point.AsQDPoint(), fQDRegion );

#endif  // _PLATFORM_MACINTOSH_


}


ODBoolean
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::IsRectangular( Environment* ev )
#else
RgnShape::IsRectangular( )
#endif   //  IBM Platforms
{


#ifdef _PLATFORM_OS2_ // JLC

   RECTL box;  // dummy
   HPS hps = GetPresSpace();
   LONG  IsRectangle = GpiQueryRegionBox( hps, fQDRegion, &box );
   ReleasePresSpace( hps );
//  return ( IsRectangle == RGN_RECT );
   if( IsRectangle == RGN_RECT && ( !fTransform || fTransform->GetType(ev) <= kODScaleTranslateXform ) )
       return TRUE;
   return FALSE;

#endif // _PLATFORM_OS2_


#ifdef _PLATFORM_WIN32_

   RECT box;  // dummy
   int  IsRectangle = GetRgnBox( fQDRegion, &box );

   if( IsRectangle == SIMPLEREGION && ( !fTransform || fTransform->GetType( ev ) <= kODScaleTranslateXform ) )
       return kODTrue;
   return kODFalse;

#endif // _PLATFORM_OS2_


#ifdef _PLATFORM_UNIX_ 

   XRectangle *rectangles;
   int NumRectangles = GetRegionRects( fQDRegion, &rectangles );

   XFree(rectangles);

   if( NumRectangles == 1 && ( !fTransform || fTransform->GetType( ev ) <= kODScaleTranslateXform ) )
       return kODTrue;
   return kODFalse;

#endif // _PLATFORM_UNIX_


#ifdef _PLATFORM_MACTINOSH

   ASSERT_NOT_NULL( fQDRegion );
   return GetHandleSize( (Handle)fQDRegion == sizeof( Region ) );
   // (Empty region has no extra data past the end of its struct.)

#endif // _PLATFORM_MACINTOSH_
}


ODBoolean
RgnShape::HasGeometry( )
{
   return kODTrue /*this->IsRectangular()*/;
}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::Copy( Environment* ev )
#else
RgnShape::Copy( )
#endif   // IBM Platforms
{

   ASSERT_NOT_NULL( fQDRegion );

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

   RgnShape* s;
   s = new RgnShape( fMode );
   s->fQDRegion = ODCopyRgnHandle( fQDRegion );

   if ( fTransform && fTransform->GetType( ev ) != kODIdentityXform )
      s = (RgnShape*)s->Transform( ev, fTransform );

   if( bboxValid ) {
      s->bbox = bbox;
      s->bboxValid = TRUE;
   }

#else

   ODRgnHandle r = (ODRgnHandle) ODCopyHandle( (ODHandle)fQDRegion );
   RgnShape* s;
   TRY{
      s = new RgnShape( fMode );
   }CATCH_ALL{
      DisposeRgn( r );
      RERAISE;
   }ENDTRY
   s->SetPlatformShape( kODQuickDraw, r );

#endif

  return s;

}


RealShape*
RgnShape::Transform( Environment *ev, ODTransform *xform )
{

   ASSERT_NOT_NULL( fQDRegion );

   if( !EmptyRgn( fQDRegion ) )
   {
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

      if ( !fTransform )
         fTransform = xform->Copy( ev );
      else
         fTransform->PostCompose( ev, xform );

#ifdef RGNDEBUG
// Force the shape to be transformed
      qprintf("\n  RgnShape::Transform ");

#ifdef _PLATFORM_OS2_
      this->GetPlatformShape(ev, kODPM );
#endif

#ifdef _PLATFORM_WIN32_
      this->GetPlatformShape(ev, kODWin32 );
#endif
#endif

   }


   fPolygon.Clear();    // maybe is better to apply the transform !?
   if( bboxValid && xform->GetType(ev) <= kODScaleTranslateXform ) {
               TransformRectangle( ev, xform, &bbox );
   }
   else {
      bboxValid = FALSE;
      bbox.Clear();
   }

#else

     if( xform->IsQDOffset( ev ) ) {
        Point offset = xform->GetQDOffset( ev );
        OffsetRgn( fQDRegion, offset.h, offset.v );    // QD offset: just offset region

        if( fPolygon.HasData() )
           fPolygon.Transform( ev, xform );      // ...and cached polygon (if any)

      } else {

      // Must promote myself to a polygon and try again.
         if( !fPolygon.HasData() )
            Rgn2Poly( fQDRegion, fPolygon );

         RealShape *polyShape = RealShape::NewGeometricShape( fMode );

         TRY{
            polyShape->SetPolygon( fPolygon );
            polyShape->Transform( ev, xform );
         }CATCH_ALL{
            delete polyShape;
            RERAISE;
         }ENDTRY
         delete this;
         return polyShape;
      }

#endif   // IBM Platforms

   return this;

}



RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::Outset( Environment* ev, ODCoordinate distance )
#else
RgnShape::Outset( ODCoordinate distance )
#endif // PLATFORM_OS2_
{

   ASSERT_NOT_NULL( fQDRegion );

   if( !EmptyRgn( fQDRegion ) ) {

#ifdef RGNDEBUG
      qprintf("\n  RgnShape::Outset - fDQRegion not empty");
#endif

#ifdef _PLATFORM_OS2_

      short intDist = ODFixedRound( distance );     // [121968] - ced
      this->GetPlatformShape( ev, kODQuickDraw ); // need to apply the transform first

      HPS hps = GetPresSpace();
      HRGN origRgn = GpiCreateRegion( hps, 0, 0 );
      HRGN tmpRgn = GpiCreateRegion( hps, 0, 0 );
      int result = GpiCombineRegion( hps, origRgn, fQDRegion, 0, CRGN_COPY );

      const POINTL ptlOffset[] = { intDist, intDist,
                                   intDist, 0,
                                   intDist, -intDist,
                                         0, -intDist,
                                  -intDist, -intDist,
                                  -intDist, 0,
                                  -intDist, intDist,
                                         0, intDist };

      fQDRegion.Unlink();

      for( int i = 0; i < 8; i++ ) {

         result = GpiCombineRegion( hps, tmpRgn, origRgn, 0, CRGN_COPY );
         if( result == RGN_ERROR ) {
            WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::Outset - CombineRgn failed."); 
         }

         result = GpiOffsetRegion( hps, tmpRgn,  (PPOINTL)(&ptlOffset[i]) );
         if( result == RGN_ERROR ) {
            WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::Outset - OffsetRgn failed."); 
         }

         if( intDist < 0 ) {
            result = GpiCombineRegion( hps, fQDRegion, fQDRegion, tmpRgn, CRGN_AND );
         } else {
            result = GpiCombineRegion( hps, fQDRegion, fQDRegion, tmpRgn, CRGN_OR );
         }
         if( result == RGN_ERROR ) {
            WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::Outset - OffsetRgn failed."); 
         }
      }

      GpiDestroyRegion( hps, tmpRgn );
      GpiDestroyRegion( hps, origRgn );


      if(bboxValid) 
         bbox.Inset( -distance, -distance );

      ReleasePresSpace(hps);


#endif   // _PLATFORM_OS2_


#ifdef _PLATFORM_WIN32_

      short intDist = ODFixedRound( distance );

      this->GetPlatformShape( ev, kODWin32 ); // need to apply the transform first

      int nRects;
      LPRGNDATA lpRgn;

      HRGN origRgn = CreateRectRgn( 0, 0, 0, 0 );
      HRGN tmpRgn = CreateRectRgn( 0, 0, 0, 0 );
      int result = CombineRgn( origRgn, fQDRegion, 0, RGN_COPY );

      const Point ptlOffset[] = { intDist, intDist,
                                intDist, 0,
                                intDist, -intDist,
                                      0, -intDist,
                               -intDist, -intDist,
                               -intDist, 0,
                               -intDist, intDist,
                                      0, intDist };

      fQDRegion.Unlink();
      for( int i = 0; i < 8; i++ ) {

         result = CombineRgn( tmpRgn, origRgn, 0, RGN_COPY );
         if( result == ERROR ) {
            WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::Outset - CombineRgn failed."); 
         }

         result = OffsetRgn( tmpRgn, ptlOffset[i].x, ptlOffset[i].y );
         if( result == ERROR ) {
            WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::Outset - OffsetRgn failed."); 
         }

         if( intDist < 0 ) {
            result = CombineRgn( fQDRegion, fQDRegion, tmpRgn, RGN_AND );
         } else {
            result = CombineRgn( fQDRegion, fQDRegion, tmpRgn, RGN_OR );
         }
         if( result == ERROR ) {
            WARNMSG_DEBUG( WARN_INDEX(-1), "RgnShape::Outset - OffsetRgn failed."); 
         }
      }

      DeleteObject( tmpRgn );
      DeleteObject( origRgn );


      if(bboxValid) 
         bbox.Inset( -distance, -distance );


   #ifdef RGNDEBUG

      GetRgnBox(fQDRegion, &boundingrect);
      qprintf("RgnShpe::Outset - region data after outset\n");
      dwSize = GetRegionData( fQDRegion, 0, NULL );
      lpRgn = (LPRGNDATA) malloc( dwSize );
      dwSize = GetRegionData( fQDRegion, dwSize, lpRgn );
      nRects = lpRgn->rdh.nCount;
      LPRECT ptRect = (LPRECT)lpRgn->Buffer;

      RECT brect;
      GetRgnBox(fQDRegion, &brect);

      qprintf("   type of region = %i pointer to object = %x nrects = %i \n \
                  bbox(left,right,top,bot) = (%i,%i,%i,%i) \n \
                  rect[0] = (%i,%i,%i,%i)\t rect[nRects] = (%i,%i,%i,%i)\n\n",
               result, this, nRects, brect.left, brect.right, brect.top, 
               brect.bottom, ptRect[0].left, ptRect[0].right, ptRect[0].top,
               ptRect[0].bottom, ptRect[nRects-1].left, ptRect[nRects-1].right,
               ptRect[nRects-1].top, ptRect[nRects-1].bottom);

      free(lpRgn);
   #endif  //RGNDEBUG


#endif   // _PLATFORM_WIN32_



#ifdef _PLATFORM_AIX_

      short intDist = ODFixedRound( distance );
      this->GetPlatformShape( ev, kODAIX ); // need to apply the transform first

      XShrinkRegion(fQDRegion, -intDist, -intDist);

      if(bboxValid) bbox.Inset( -distance, -distance );

#endif   // _PLATFORM_AIX_


#ifdef _PLATFORM_MACINTOSH_

      short intDist = ODFixedRound( -distance );
      InsetRgn( fQDRegion, intDist, intDist );

#endif   // _PLATFORM_MACINTOSH_


      this->Purge(0);


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

      if( fTransform ) {
         ODReleaseObject( ev, fTransform );
         fTransform = kODNULL;
      }

#endif   //  IBM Platforms

   }

   return this;

}


RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::Subtract( Environment* ev, RealShape *shape )
#else
RgnShape::Subtract( RealShape *shape )
#endif   // IBM Platforms
{

   ASSERT_NOT_NULL( fQDRegion );

#ifdef RGNDEBUG
   qprintf("\n  RgnShape::Subtract");
#endif

   if( shape == this )
      return this->Clear();

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

   else {                 // if( !this->IsEmpty(ev) && !shape->IsObviousEmpty(ev) )  {
      ODRect a, b;

      shape->GetPesimisticBoundingBox( ev, &a );
      if( a.IsEmpty() )
         return this;

      this->GetPesimisticBoundingBox( ev, &b );
      if(b.IsEmpty())
         return this;

      // Quick test to see if bounding boxes intersect
      a &= b;
      if(a.IsEmpty())    // bounding boxes don't intersect
         return this;


   // _DO_POLYCLIP_ is the magic which turns on and off the use of
   // the CompositeShape class and the associated shape B-Tree.
   // With the CompositeShape class, the Promote will create a copy of
   // the shape and link it into the B-tree.  In the DR4 code, we actually
   // want to take the difference between the 2 regions.

 #ifndef _DO_POLYCLIP_

      return this->Promote( ev, kShapeDifference, shape );

 #else

      // GetPlatformShape will apply the cached xform to the regions.
      // We need to do this before subtracting the regions.

  #ifdef _PLATFORM_WIN32_

      this->GetPlatformShape( ev, kODWin32 );

      DiffRgn( fQDRegion, shape->GetPlatformShape( ev, kODWin32 ), fQDRegion );

   #ifdef RGNDEBUG

      qprintf("RgnShpe::Subtract - region data after subtraction\n");
      DWORD dwSize = GetRegionData( fQDRegion, 0, NULL );
      LPRGNDATA lpRgn = (LPRGNDATA) malloc( dwSize );
      dwSize = GetRegionData( fQDRegion, dwSize, lpRgn );
      DWORD nRects = lpRgn->rdh.nCount;
      LPRECT ptRect = (LPRECT)lpRgn->Buffer;

      RECT brect;
      GetRgnBox(fQDRegion,&brect);

      qprintf("   pointer to object = %x nrects = %i \n \
                  bbox(left,right,top,bot) = (%i,%i,%i,%i) \n \
                  rect[0] = (%i,%i,%i,%i)\t rect[nRects] = (%i,%i,%i,%i)\n\n",
               this, nRects, brect.left, brect.right, brect.top, brect.bottom,
               ptRect[0].left, ptRect[0].right, ptRect[0].top, ptRect[0].bottom,
               ptRect[nRects-1].left, ptRect[nRects-1].right,
               ptRect[nRects-1].top, ptRect[nRects-1].bottom);

      free(lpRgn);
   #endif  //RGNDEBUG

  #endif

  #ifdef _PLATFORM_AIX_

      this->GetPlatformShape( ev, kODAIX );

      DiffRgn( fQDRegion, shape->GetPlatformShape( ev, kODAIX ), fQDRegion );

  #endif

  #ifdef _PLATFORM_OS2_

      this->GetPlatformShape( ev, kODPM );

      DiffRgn( fQDRegion, shape->GetPlatformShape( ev, kODPM ), fQDRegion );

  #endif

      // We need this additional call to Purge since there is one case
      // in GetPlatformShape where the region is converted to a polygon.
      // Once we call DiffRgn, that polygon will be invalid.

      this->Purge(0);


 #endif  // _DO_POLYCLIP_


#else  // IBM_PLATFORMS

   else if( !this->IsEmpty() && !shape->IsEmpty() )  {
      ODRect bounds;
      Rect b, sect;
      shape->GetBoundingBox(&bounds);
      bounds.AsQDRect(b);
      if( SectRect(&b,&(**fQDRegion).rgnBBox,&sect) )
      {
         DiffRgn(fQDRegion, shape->GetQDRegion(), fQDRegion);
         this->Purge(0);
      }
#endif   // IBM Platforms

   }
   return this;

}




RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::Intersect( Environment* ev, RealShape *shape )
#else
RgnShape::Intersect( RealShape *shape )
#endif   // IBM Platforms
{

   ASSERT_NOT_NULL( fQDRegion );

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

#ifdef RGNDEBUG
   qprintf("\n  RgnShape::Intersect");
#endif

   if( shape!=this && !this->IsEmpty( ev ) )  {
      ODRect a, b ;
      shape->GetPesimisticBoundingBox( ev, &a );

      if( !a.IsEmpty() ) {
         this->GetPesimisticBoundingBox( ev, &b );
         a &= b;
      }

      if( !a.IsEmpty() ) {

         // _DO_POLYCLIP_ is the magic which turns on and off the use of
         // the CompositeShape class and the associated shape B-Tree.
         // With the CompositeShape class, the Promote will create a copy of
         // the shape and link it into the B-tree.  In the DR4 code, we actually
         // want to find the intersection between the 2 regions.

 #ifndef _DO_POLYCLIP_

         return this->Promote( ev, kShapeIntersection, shape );

 #else


         // GetPlatformShape will apply the cached xform to the regions.
         // We need to do this before taking the intersection of the
         // 2 shapes.


  #ifdef _PLATFORM_WIN32_

         this->GetPlatformShape( ev, kODWin32 );

         SectRgn( fQDRegion, shape->GetPlatformShape( ev, kODWin32 ), fQDRegion );
   #ifdef RGNDEBUG

         qprintf("RgnShpe::Intersect - region data after intersection \n");
         DWORD dwSize = GetRegionData( fQDRegion, 0, NULL );
         LPRGNDATA lpRgn = (LPRGNDATA) malloc( dwSize );
         dwSize = GetRegionData( fQDRegion, dwSize, lpRgn );
         DWORD nRects = lpRgn->rdh.nCount;
         LPRECT ptRect = (LPRECT)lpRgn->Buffer;

         RECT brect;
         GetRgnBox(fQDRegion,&brect);

         qprintf("   pointer to object = %x nrects = %i \n \
                  bbox(left,right,top,bot) = (%i,%i,%i,%i) \n \
                  rect[0] = (%i,%i,%i,%i)\t rect[nRects] = (%i,%i,%i,%i)\n\n",
               this, nRects, brect.left, brect.right, brect.top, brect.bottom,
               ptRect[0].left, ptRect[0].right, ptRect[0].top, ptRect[0].bottom,
               ptRect[nRects-1].left, ptRect[nRects-1].right,
               ptRect[nRects-1].top, ptRect[nRects-1].bottom);

         free(lpRgn);
   #endif  //RGNDEBUG


  #endif

  #ifdef _PLATFORM_AIX_

         this->GetPlatformShape( ev, kODAIX );

         SectRgn( fQDRegion, shape->GetPlatformShape( ev, kODAIX ), fQDRegion );

  #endif

  #ifdef _PLATFORM_OS2_

         this->GetPlatformShape( ev, kODPM );

         SectRgn( fQDRegion, shape->GetPlatformShape( ev, kODPM ), fQDRegion );

  #endif

 #endif  // _DO_POLYCLIP_

      }

#else  // IBM_PLATFORMS

   if( shape!=this && !this->IsEmpty() )  {
      ODRect bounds;
      Rect b, sect;
      shape->GetBoundingBox( &bounds );
      bounds.AsQDRect( b );
      if( !bounds.IsEmpty() && SectRect( &b, &(**fQDRegion).rgnBBox, &sect ) )
         SectRgn( fQDRegion, shape->GetQDRegion(), fQDRegion );

#endif   // IBM Platforms


      else {


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
         fQDRegion.Unlink();
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
         SetRectRgn( fQDRegion, 0, 0, 0, 0 );
#endif
#if defined(_PLATFORM_UNIX_)
         SetRectRgn( fQDRegion, 0, 0, 0, 0 );
#endif
      }

      // We need this additional call to Purge since there is one case
      // in GetPlatformShape where the region is converted to a polygon.
      // Once we call DiffRgn, that polygon will be invalid.

      this->Purge(0);

   }

   return this;
}



RealShape*
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
RgnShape::Union( Environment* ev, RealShape *shape )
#else
RgnShape::Union( RealShape *shape )
#endif   // IBM Platforms
{

   ASSERT_NOT_NULL( fQDRegion );

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

#ifdef RGNDEBUG
   qprintf("\n  RgnShape::Union");
#endif

   if( shape == this || shape->IsObviousEmpty( ev ) )
      return this;

   if( this->IsEmpty( ev ) ) {
      return this->ReplaceWith( ev, shape );   // [144539] - ced
   }

   // _DO_POLYCLIP_ is the magic which turns on and off the use of
   // the CompositeShape class and the associated shape B-Tree.
   // With the CompositeShape class, the Promote will create a copy of
   // the shape and link it into the B-tree.  In the DR4 code, we actually
   // want to take the union of the 2 regions.

 #ifndef _DO_POLYCLIP_

   return this->Promote( ev, kShapeUnion, shape );

 #else


   // GetPlatformShape will apply the cached xform to the regions.
   // We need to do this before taking the union of the 2 shapes.

  #ifdef _PLATFORM_WIN32_

   this->GetPlatformShape( ev, kODWin32 );

   UnionRgn( fQDRegion, shape->GetPlatformShape( ev, kODWin32 ), fQDRegion );

   #ifdef RGNDEBUG

      qprintf("RgnShpe::Union - region data after union \n");
      DWORD dwSize = GetRegionData( fQDRegion, 0, NULL );
      LPRGNDATA lpRgn = (LPRGNDATA) malloc( dwSize );
      dwSize = GetRegionData( fQDRegion, dwSize, lpRgn );
      DWORD nRects = lpRgn->rdh.nCount;
      LPRECT ptRect = (LPRECT)lpRgn->Buffer;

      RECT brect;
      GetRgnBox(fQDRegion,&brect);

      qprintf("   pointer to object = %x nrects = %i \n \
                  bbox(left,right,top,bot) = (%i,%i,%i,%i) \n \
                  rect[0] = (%i,%i,%i,%i)\t rect[nRects] = (%i,%i,%i,%i)\n\n",
               this, nRects, brect.left, brect.right, brect.top, brect.bottom,
               ptRect[0].left, ptRect[0].right, ptRect[0].top, ptRect[0].bottom,
               ptRect[nRects-1].left, ptRect[nRects-1].right,
               ptRect[nRects-1].top, ptRect[nRects-1].bottom);

      free(lpRgn);
   #endif  //RGNDEBUG

  #endif

  #ifdef _PLATFORM_AIX_

   this->GetPlatformShape( ev, kODAIX );

   UnionRgn( fQDRegion, shape->GetPlatformShape( ev, kODAIX ), fQDRegion );

  #endif

  #ifdef _PLATFORM_OS2_

   this->GetPlatformShape( ev, kODPM );

   UnionRgn( fQDRegion, shape->GetPlatformShape( ev, kODPM ), fQDRegion );

  #endif

   // We need this additional call to Purge since there is one case
   // in GetPlatformShape where the region is converted to a polygon.
   // Once we call DiffRgn, that polygon will be invalid.

   this->Purge(0);

 #endif  // _DO_POLYCLIP_



#else  // IBM_PLATFORMS

   if( shape!=this && !shape->IsEmpty() )  {
      UnionRgn( fQDRegion, shape->GetQDRegion(), fQDRegion );
      this->Purge(0);
   }

#endif   // IBM Platforms

   return this;


}


#ifdef RGNDEBUG

#ifdef _PLATFORM_OS2_

void OS2DumpRegion(HPS hps, HRGN hrgn)
{
  PRECTL prcl, prclOrig;
  int nRects = OS2GetRegionRects(hps, hrgn, &prcl);

   qprintf("\n    DumpRegion: region contains %d rectangles\n",  nRects);

   prclOrig = prcl;
   for (int i = 0; i < nRects; i++ ) {
//      cout << "Rect " << i << " - l, b - " << prcl[i].xLeft << ", " <<  prcl[i].yBottom << "    r, t - " << prcl[i].xRight << ", " << prcl[i].yTop << endl;
      qprintf("      Rect %d - l, b - %d, %d     r, t - %d, %d \n",  i, prcl->xLeft, prcl->yBottom, prcl->xRight, prcl->yTop);
      if (!((i+1) % 4)) {
         qprintf("\n");
      }
      prcl++;
   }
   qprintf("\n");
   free(prclOrig);
}


int OS2GetRegionRects(HPS hps, HRGN hrgn, PRECTL *pprcl)
{
   RGNRECT rgnrcControl;
   PRECTL prcl;
   BOOL fResult;
   int i, nRects, nBytes;

   /*--------------------------------------------------+
   | Determine the number of rectangles in the region. |
   +--------------------------------------------------*/

   rgnrcControl.crc = 100;
   rgnrcControl.ulDirection = RECTDIR_LFRT_TOPBOT;
   rgnrcControl.ircStart = 1;
   nBytes = rgnrcControl.crc * sizeof(RECTL);
   if (!(prcl = (PRECTL)malloc(nBytes)))
      qprintf("\n***** malloc failed in GetRegionRects.\n");

   while (0 != (fResult = GpiQueryRegionRects(hps, hrgn, NULL,
                                 &rgnrcControl,
                                 prcl + rgnrcControl.ircStart - 1)) &&
          rgnrcControl.crcReturned == rgnrcControl.crc)
   {
      nBytes += (rgnrcControl.crc * sizeof(RECTL));
      if (!(prcl = (PRECTL)realloc(prcl, nBytes)))
         qprintf("\n***** malloc failed in GetRegionRects.\n");
      rgnrcControl.ircStart += rgnrcControl.crc;
   };
   nRects = rgnrcControl.ircStart + rgnrcControl.crcReturned - 1;
   /*-----------------------------------------------+
   | If there was an error then raise an exception. |
   +-----------------------------------------------*/
   if (!fResult) {
      if (prcl) free(prcl);
      *pprcl = NULL;
      return 0;
   }
   *pprcl = prcl;
   return nRects;
}

#endif // _PLATFORM_OS2_






#ifdef _PLATFORM_WIN32_
void DumpRegion( HRGN hrgn )
{
   LPRGNDATA lpRgn;
   LPRECT prcl;
   DWORD nRects, dwSize;

   if( !hrgn ) {
      qprintf("DumpRegion:hrgn is NULL, returning.");
      return;
   }

   dwSize = GetRegionData( hrgn, 0, NULL );

   if( dwSize > 1 ) {
      if( !( lpRgn = (LPRGNDATA) malloc( dwSize ) ) )
         qprintf("malloc failed in DumpRegion.");
      dwSize = GetRegionData( hrgn, dwSize, lpRgn );

      nRects = lpRgn->rdh.nCount;
   }
   else {
      /*-----------------------------------------------+
      | If there was an error then raise an exception. |
      +-----------------------------------------------*/
         qprintf("DumpRegion:No region data for HRGN, dwSize <= 1.");
         return;
   }


   prcl = (LPRECT)lpRgn->Buffer;

   qprintf("DumpRegion: region contains %d rectangles.", nRects );

   qprintf("\n");
   for (int i = 0; i < nRects; i++ ) {
      qprintf("Rect %d - l, b - %d, %d     r, t - %d, %d \n",  i, prcl->left, prcl->bottom, prcl->right, prcl->top);
      if (!((i+1) % 4)) {
         qprintf("\n");
      }
      prcl++;
   }
   qprintf("\n");
   free( lpRgn );
}



int GetRegionRectangles( HRGN hrgn, LPRGNDATA *pprcl )
{
   LPRECT prcl;
   BOOL fResult;
   int i, nRects, nBytes;

   /*--------------------------------------------------+
   | Determine the number of rectangles in the region. |
   +--------------------------------------------------*/

   LPRGNDATA lpRgn;

   // TODO: Should do some error checking here.  If dwSize = 0
   //       then an error occurred, dwSize = 1 if successful,
   //       and otherwise equal to the size, in bytes, of the
   //       required buffer.

   DWORD dwSize = GetRegionData( hrgn, 0, lpRgn );


   if( dwSize > 1 ) {
      if( !( lpRgn = (LPRGNDATA) malloc( dwSize ) ) )
         THROW( kODErrOutOfMemory );
      DWORD dwSize = GetRegionData( hrgn, dwSize, lpRgn );

      DWORD nRects = lpRgn->rdh.nCount;
   }
   else {
      /*-----------------------------------------------+
      | If there was an error then raise an exception. |
      +-----------------------------------------------*/
         return 0;
   }
   *pprcl = lpRgn;
   return nRects;
}

#endif  // _PLATFORM_WIN32_


#endif // RGNDEBUG



