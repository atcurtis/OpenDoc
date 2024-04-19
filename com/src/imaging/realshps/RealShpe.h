/* @(#)Z 1.11 com/src/imaging/realshps/RealShpe.h, odimaging, od96os2, odos29646d 96/11/15 15:29:18 (96/10/29 09:27:09) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
 *
 *   CLASSES:   RealShape
 *		RegionRef
 *		Region_rep
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */

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
/*  23332    6/24/96 chb  Added SetDisplay/fDisplay for X Windows   */
/*  133968  08/22/95 aml  Port DR3                                  */
/*  122017   5/11/95 aml  Performance enhancements                  */
/*  120604   4/28/95 ced  Merge with B1C16 Apple code.              */
/*                                                                  */
/********************************************************************/
/*
	File:		RealShpe.h

	Contains:	RealShape class, private to ODShape.

	Owned by:	Jens Alfke

	Copyright:	1993-94 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>	 5/25/95	jpa		Use new GX headers [1241078, 1253324]
		 <6>	 2/24/95	jpa		Removed local ODNewRgn. [1220810]
		 <5>	 12/5/94	jpa		Code review cleanup [1203923]
		 <4>	10/24/94	jpa		Added ODNewRgn. [1151710]
		 <3>	  8/8/94	jpa		Added Outset method [1178690]
		 <2>	  8/2/94	jpa		Added AsPolygonShape.
		 <1>	 6/15/94	jpa		first checked in
							--------Moved to SOM project and rearranged-----
		 <7>	  5/9/94	jpa		Enable polygon clipper and GX support
									[1162090]
		 <6>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <5>	 2/17/94	JA		Declare XMPPolygon as struct, not class.
		 <4>	 2/16/94	JA		Include new AltPoint.h.
		 <3>	 2/10/94	JA		Stop using ptrs to member fns.
		 <2>	  2/9/94	JA		Renamed from ShapePrv.h --> ShapePvM.h
		 <6>	  2/7/94	JA		Further tigerings.
		 <5>	  2/3/94	JA		Tiger Team Makeover!
		 <4>	 1/31/94	JA		XMPRealShape no longer derives from
									XMPAbsShape. Other API improvements.
		 <3>	11/24/93	VL		Rolled back changes as ASLM build breaks
									the THINK build.
		 <2>	11/23/93	VL		Made this work with ASLM.
		 <1>	11/23/93	JA		first checked in
		 
	In Progress:
		
*/


#ifndef _REALSHPE_
#define _REALSHPE_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __GXTYPES__
#include <GXTypes.h>				/* for gxShape type */
#endif
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#ifndef _PLATSHAPE_
#include "PlatShpe.h"
#endif
#endif // IBM Platforms

#ifndef _POLYCLIP_
#include "PolyClip.h"			/* for ODShapeOp enum*/
#endif


#ifdef _PLATFORM_MACINTOSH_

#ifndef __QUICKDRAW__
#include <QuickDraw.h>			/* for Region type*/
#endif

#else

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
class Region_rep {
   HRGN hrgn;
   int refs;
   Region_rep( HRGN );
   ~Region_rep();
   friend class RegionRef;
};

class RegionRef {
   Region_rep *rgn;
public:
   RegionRef &operator =( const RegionRef& );
   RegionRef &operator =( HRGN );
   operator HRGN ();
   RegionRef( RegionRef& );
   RegionRef( HRGN = 0 );
   ~RegionRef();
   void InitRegion( HRGN );
   void Unlink();
   void DisposeRgn();
};
#endif

#if defined(_PLATFORM_UNIX_)
class Region_rep {
   Region hrgn;
   int refs;
   Region_rep( Region );
   ~Region_rep();
   friend class RegionRef;
};

class RegionRef {
   Region_rep *rgn;
public:
   RegionRef &operator =( const RegionRef& );
   RegionRef &operator =( Region );
   operator Region ();
   RegionRef( RegionRef& );
   RegionRef( Region = 0 );
   ~RegionRef();
   void InitRegion( Region );
   void Unlink();
   void DisposeRgn();
};
#endif // _PLATFORM_UNIX_

#endif



//==============================================================================
// Classes used in this interface
//==============================================================================

class ODShape;

class PolygonShape;

//==============================================================================
// QuickDraw Classic/GX Utilities
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
void ClearGXError( );
void ThrowIfGXError( );
void ThrowIfFirstGXError( );
#endif


//==============================================================================
// RealShape
//==============================================================================

class RealShape {
  public:

    RealShape( ODGeometryMode );
    virtual ~RealShape( );

    ODVMethod ODSize     Purge(ODSize size);

    ODMethod void      SetGeometryMode( ODGeometryMode );
    ODMethod ODGeometryMode  GetGeometryMode( );

    ODVMethod RealShape*  SetPolygon( const ODPolygon& );
    ODVMethod void      Simplify( );                       // Implemented only by PolygonShape

    ODVMethod void       SetPlatformShape( ODGraphicsSystem, ODPlatformShape );

//
// IBM Platforms
//
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

    ODMethod ODPlatformShape GetPlatformShape( Environment*, ODGraphicsSystem );

    inline RgnHandle    GetQDRegion( Environment* ev )
            {return (RgnHandle)this->GetPlatformShape( ev, kODQuickDraw);}

#else

    ODMethod ODPlatformShape GetPlatformShape( ODGraphicsSystem );

    inline RgnHandle    GetQDRegion( )
            {return (RgnHandle)this->GetPlatformShape(kODQuickDraw);}

#endif //  IBM Platforms 


    ODVMethod ODBoolean    HasGeometry( );

    ODVMethod RealShape*  Clear( );

    static RealShape*    NewGeometricShape( ODGeometryMode mode );



  // Abstract stubs (these basically follow the ODShape interface:)

// 
// IBM Platforms
//
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)


    ODVMethod void       GetBoundingBox( Environment* ev, ODRect *bounds ) = 0;
    ODVMethod void GetPesimisticBoundingBox(Environment *ev, ODRect *bounds);  // AL
    ODVMethod RealShape*   SetRectangle( const ODRect *rect ) = 0;
    ODVMethod void       CopyPolygon( Environment*, ODPolygon& ) = 0;
    ODVMethod ODBoolean    IsSameAs( Environment*, RealShape * compareShape) = 0;
    ODVMethod ODBoolean    IsEmpty( Environment* ) = 0;
    ODVMethod ODBoolean    IsObviousEmpty(Environment*);  // AL
    ODVMethod ODBoolean    ContainsPoint( Environment*, ODPoint point ) = 0;
    ODVMethod ODBoolean    IsRectangular( Environment* ) = 0;
    ODVMethod ODBoolean    IsObviousRectangular(Environment*);  // AL
    ODVMethod RealShape*  Transform(Environment*, ODTransform* transform) = 0;
    ODVMethod RealShape*  Copy( Environment* ) = 0;
    ODVMethod RealShape*  Outset(Environment*, ODCoordinate distance) = 0;
    ODVMethod RealShape*  Subtract( Environment*, RealShape * diffShape ) = 0;
    ODVMethod RealShape*  Intersect(Environment*, RealShape * sectShape) = 0;
    ODVMethod RealShape*  Union( Environment*, RealShape * unionShape ) = 0;

    ODVMethod RealShape*  Combine( Environment*, ODShapeOp, RealShape * );

#else

    ODVMethod void       GetBoundingBox( ODRect *bounds ) = 0;
    ODVMethod RealShape*   SetRectangle( const ODRect *rect ) = 0;
    ODVMethod void       CopyPolygon( ODPolygon& ) = 0;
    ODVMethod ODBoolean    IsSameAs( RealShape* compareShape) = 0;
    ODVMethod ODBoolean    IsEmpty( ) = 0;
    ODVMethod ODBoolean    ContainsPoint( ODPoint point ) = 0;
    ODVMethod ODBoolean    IsRectangular( ) = 0;
    ODVMethod RealShape*  Copy( ) = 0;
    ODVMethod RealShape*  Transform(Environment*, ODTransform* transform) = 0;
    ODVMethod RealShape*  Outset(ODCoordinate distance) = 0;
    ODVMethod RealShape*  Subtract( RealShape* diffShape ) = 0;
    ODVMethod RealShape*  Intersect(RealShape* sectShape) = 0;
    ODVMethod RealShape*  Union( RealShape* unionShape ) = 0;

    ODVMethod RealShape*  Combine( ODShapeOp, RealShape* );

#endif // IBM Platforms


#if ODDebug
	char					GetType( ) const		{return fType;}
#endif

#if _PLATFORM_UNIX_
    static void SetDisplay(Environment*, Display *display);
    static int  GetRegionRects( ODRgnHandle rgn, XRectangle** rectangles );
#endif // _PLATFORM_UNIX_

  protected:

// 
// IBM Platforms
//
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

    ODVMethod RealShape*  ReplaceWith( Environment*, RealShape* );   // Delete me & return copy of shape
    ODMethod RealShape*    Promote( Environment*, 
                                    ODShapeOp =kShapeNoOp,
                                    RealShape*  = NULL );            // Convert & perform op
    ODVMethod RealShape*  AsPolygonShape( Environment* );            // Return equivalent polygon/GX shape
    ODVMethod void      InitQDRegion( Environment* ) = 0;            // Called when fQDRegion needs to be set

#else

    ODVMethod RealShape*  ReplaceWith( RealShape* );                 // Delete me & return copy of shape
    ODMethod RealShape*    Promote( ODShapeOp =kShapeNoOp,
                                    RealShape*  = NULL);             // Convert & perform op
    ODVMethod RealShape*  AsPolygonShape( );                         // Return equivalent polygon/GX shape
    ODVMethod gxShape    CopyGXShape( ) = 0;                         // Return a GX shape
    ODVMethod void      InitQDRegion( ) = 0;                         // Called when fQDRegion needs to be set

#endif // IBM Platforms 


#if ODDebug
    char          fType;          // Number identifying class type:
                                  /* 0=Rect, 1=Rgn, 2=Poly, 3=GX*/
#endif

    ODGeometryMode      fMode;          // Geometry mode

// 
// IBM Platforms
//
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

    RegionRef        fQDRegion;

#ifdef _PLATFORM_UNIX_
	//    static Display *fDisplay;
	//    static ODBoolean fShapeExtensionOK;
	//    static ODBoolean fShapeExtensionOK_Known;
#endif // _PLATFORM_UNIX_

#else

    RgnHandle        fQDRegion;        // Cached QuickDraw region

#endif  // IBM Platforms 

};


#ifdef _PLATFORM_MACINTOSH_
extern ODBoolean gGX;		// Is GX installed?
#endif  // _PLATFORM_MACINTOSH_

#endif /*_REALSHPE_*/








