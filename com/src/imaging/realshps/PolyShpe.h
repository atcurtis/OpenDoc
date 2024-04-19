/* @(#)Z 1.6 com/src/imaging/realshps/PolyShpe.h, odimaging, od96os2, odos29646d 96/11/15 15:29:17 (96/10/29 09:27:00) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
 *
 *   CLASSES:   PolygonShape
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

/*
	File:		PolyShpe.h

	Contains:	PolygonShape class, private to ODShape.

	Written by:	Jens Alfke

	Copyright:	1993-94 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>	 12/5/94	jpa		Code review cleanup [1203923]. Also
									simplify before clipping [1196018]
		 <3>	10/24/94	jpa		Added fSimple flag [1191192]
		 <2>	  8/8/94	jpa		Added Outset method [1178690]
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


#ifndef _POLYSHPE_
#define _POLYSHPE_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _REALSHPE_
#include "RealShpe.h"		/* Base class*/
#endif


//==============================================================================
// PolyShape
//==============================================================================


class PolygonShape :public RealShape {
  public:
    PolygonShape( ODGeometryMode );
    virtual ~PolygonShape( );

    ODVMethod RealShape*  SetRectangle( const ODRect *rect );
    ODVMethod RealShape*  SetPolygon( const ODPolygon& );
    ODVMethod void        Simplify( );      // Call after SetPolygon if poly isn't simple
    ODVMethod RealShape*  Transform( Environment*, ODTransform* );
    ODVMethod ODSize      Purge( ODSize );

//
// IBM Platforms
//
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

    ODVMethod void        GetBoundingBox( Environment*,  ODRect *bounds );
    ODVMethod void        CopyPolygon( Environment*, ODPolygon& );
    ODVMethod ODBoolean   IsSameAs(  Environment*, RealShape * compareShape );
    ODVMethod ODBoolean   IsEmpty( Environment* );
    ODVMethod ODBoolean   ContainsPoint( Environment*, ODPoint point );
    ODVMethod ODBoolean   IsRectangular( Environment* );
    ODVMethod RealShape*  Copy( Environment* );              // Really returns an ODPolygonShape*
    ODVMethod RealShape*  Outset(Environment*, ODCoordinate distance );
    ODVMethod RealShape*  Subtract( Environment*, RealShape * diffShape );
    ODVMethod RealShape*  Intersect( Environment*, RealShape * sectShape );
    ODVMethod RealShape*  Union( Environment*, RealShape * sectShape );
    ODVMethod RealShape*  Combine( Environment*,  ODShapeOp, RealShape * );

#else

    ODVMethod void        GetBoundingBox( ODRect *bounds );
    ODVMethod void        CopyPolygon( ODPolygon& );
    ODVMethod ODBoolean   IsSameAs( RealShape* compareShape );
    ODVMethod ODBoolean   IsEmpty( );
    ODVMethod ODBoolean   ContainsPoint(ODPoint point);
    ODVMethod ODBoolean   IsRectangular( );
    ODVMethod RealShape*  Copy( );  // Really returns an ODPolygonShape*
    ODVMethod RealShape*  Outset(ODCoordinate distance );
    ODVMethod RealShape*  Subtract(RealShape* diffShape );
    ODVMethod RealShape*  Intersect(RealShape* sectShape );
    ODVMethod RealShape*  Union(RealShape* sectShape );
    ODVMethod RealShape*  Combine( ODShapeOp, RealShape* );

#endif  // IBM Platforms


  protected:

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

    ODVMethod void      InitQDRegion( Environment* );

#else

    ODVMethod void      InitQDRegion( );
    ODVMethod gxShape    CopyGXShape( );

#endif  // IBM Platforms

  private:

  ODPolygon  fPolygon;
  ODRect    fBounds;
  ODBoolean  fBoundsValid;
};


#endif /*_POLYSHPE_*/















