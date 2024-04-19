//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.10 com/src/utils/include/LineOps.h, odutils, od96os2, odos29646d 8/19/96 12:01:38 [ 11/15/96 15:29:32 ]
/*
	File:		LineOps.h

	Contains:	Geometric operations on lines in 2-space.

	Owned by:	Jens Alfke
				IntersectLines by Ken Turkowski, from Apple Technical Memorandum KT14.

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Theory of Operation:
	
	Endpoints of lines or ranges need not be given in any particular order.
	
	Ranges include both endpoints, unline rectangles which don't include right and bottom.
	(This is necessary to make segments that share an endpoint intersect, and to make e.g.
	the horizontal range of a vertical line be non-empty.)
	
*/


#ifndef _LINEOPS_
#define _LINEOPS_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


enum{
	kIntersection,				// Segments intersect
	kOutsideIntersection,		// Lines intersect, but past ends of segments
	kNoIntersection				// Lines are parallel or degenerate
};
typedef short IntersectionStatus;


const ODCoordinate kFixedEpsilon = 7;		// This is about 0.0001 pixels


extern "C" {


inline ODCoordinate Min( ODCoordinate a, ODCoordinate b )	{return a<b ?a :b;}
inline ODCoordinate Max( ODCoordinate a, ODCoordinate b )	{return a>b ?a :b;}

inline ODCoordinate Abs( ODCoordinate n )					{return n>=0 ?n :-n;}

_DLLIMPORTEXPORT_ ODBoolean WithinEpsilon( ODCoordinate a, ODCoordinate b );

_DLLIMPORTEXPORT_ ODCoordinate	Distance( const ODPoint &p0, const ODPoint &p1 );

_DLLIMPORTEXPORT_ void		GetLineShift( ODPoint p0, ODPoint p1, ODCoordinate dist,
									ODPoint &delta );

_DLLIMPORTEXPORT_ IntersectionStatus	IntersectLines( const ODPoint &p0, const ODPoint &p1,
								    const ODPoint &q0, const ODPoint &q1,
								    ODPoint *sect );
			    
_DLLIMPORTEXPORT_ ODBoolean	IntersectSegments( const ODPoint &p0, const ODPoint &p1,
									   const ODPoint &q0, const ODPoint &q1,
									   ODPoint *sect );

_DLLIMPORTEXPORT_ ODCoordinate	GetLineXAtY( const ODPoint &p0, const ODPoint &p1,
									   ODCoordinate y );

#ifdef _PLATFORM_MACINTOSH_
Fract				GetIntersectionT( const ODPoint &p0, const ODPoint &p1,
#else
_DLLIMPORTEXPORT_ ODFract	GetIntersectionT( const ODPoint &p0, const ODPoint &p1,
#endif
									  const ODPoint &sect );

_DLLIMPORTEXPORT_ ODBoolean	InRange( ODCoordinate n,  ODCoordinate r0, ODCoordinate r1 );

_DLLIMPORTEXPORT_ ODBoolean	RangesDisjoint( ODCoordinate a0, ODCoordinate a1,
									ODCoordinate b0, ODCoordinate b1 );

}


#endif /*_LINEOPS_*/
