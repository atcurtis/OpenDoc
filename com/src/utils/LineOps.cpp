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
// @(#) 1.13 com/src/utils/LineOps.cpp, odutils, od96os2, odos29646d 10/10/96 15:59:55 [ 11/15/96 15:28:44 ]
/*
	File:		LineOps.cpp

	Contains:	Geometric operations on lines in 2-space.

	Written by:	Jens Alfke
				IntersectLines by Ken Turkowski, from Apple Technical Memorandum KT14.

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ALTPOINT_
#include "AltPoint.h"			// Use C++ savvy ODPoint and ODRect
#endif

#include "LineOps.h"

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODMATH_
#include "ODMath.h"
#endif

#include <stdlib.h>

#include <math.h>

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif


#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
enum eAnomaly {kNoAnomaly, kAnomalyVertical, kAnomalyPoint};

eAnomaly ProcessLine(const ODPoint &p0, const ODPoint &p1,
                     double *slope, double *constant);
#else // !defined(_PLATFORM_WIN32_) && defined(_PLATFORM_OS2_) && defined(_PLATFORM_UNIX_)
enum{
	A00, A01, A10, A11
};
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)


#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODShape
#endif


//------------------------------------------------------------------------------
// WithinEpsilon
//
// Returns true if the two numbers are within kEpsilon of each other.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODBoolean
WithinEpsilon( ODCoordinate a, ODCoordinate b )
{
	ODCoordinate diff = a-b;
	return diff<=kFixedEpsilon && diff>=-kFixedEpsilon;
}



//------------------------------------------------------------------------------
// InRange
//
// Determine whether n falls within the range [r0,r1].
// The r's need not be given in order. Unline rects, endpoints are included.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODBoolean
InRange( ODCoordinate n,  ODCoordinate r0, ODCoordinate r1 )
{
	if( r0<r1 )
		return n>=r0 && n<=r1;
	else
		return n>=r1 && n<=r0;
}


//------------------------------------------------------------------------------
// RangesDisjoint
//
// Determine whether two ranges do not overlap. They needn't be given in order.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODBoolean
RangesDisjoint( ODCoordinate a0, ODCoordinate a1,
				ODCoordinate b0, ODCoordinate b1 )
{
	if( a0<a1 )
		return Max(b0,b1) < a0  ||  a1 < Min(b0,b1);
	else
		return Max(b0,b1) < a1  ||  a0 < Min(b0,b1);
}


//------------------------------------------------------------------------------
// IntersectLines						by Ken Turkowski, tweaked by Jens Alfke
//
// Find the intersection of the two lines determined by two pairs of points.
// Returns:	kIntersection if the two segments intersect;
//			kOutsideIntersection if the lines intersect beyond the ends of the segments;
//			kNoIntersection if the lines are parallel, coincident or degenerate.
//						(In this last case sect will not be valid.)
//------------------------------------------------------------------------------

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
// History:
//    The original version of this function, which uses ODFixedPoint arithmetic, was
//    replaced by a version that uses floating point arithmetic for increased
//    performance.  However, the floating point version introduced excessive
//    round-off error.  (The failing scenario is two lines forming a very small angle,
//    with one line being almost horizonal.)  The version immediately below is the newer
//    floating point version.  This algorithm uses fewer multiply and divide
//    operations, so there is less round-off error.  It's even 20-35% faster than
//    the original floating point version.
WIN32_DLLEXPORT IntersectionStatus
IntersectLines(const ODPoint &p0, const ODPoint &p1,
               const ODPoint &q0, const ODPoint &q1,
               ODPoint *sect )
{
	enum eBoundsCheck {kBoundsCheckX, kBoundsCheckY};

	double slopeP, constantP, slopeQ, constantQ,
	       interX, interY;

	eAnomaly anomalyP, anomalyQ;
	eBoundsCheck pBoundsCheck = kBoundsCheckX, qBoundsCheck = kBoundsCheckX;
	ODBoolean inside;
	
	anomalyP = ProcessLine(p0, p1, &slopeP, &constantP);
	anomalyQ = ProcessLine(q0, q1, &slopeQ, &constantQ);
	
	if (anomalyP == kNoAnomaly && anomalyQ == kNoAnomaly)
	{
		double a, b;

		a = slopeP - slopeQ;
		if (a != 0.0)
		{
			b = constantQ - constantP;
			interX = b / a; 
			interY = slopeP * interX + constantP;
		}
		else
			return kNoIntersection;
	}
	else
	{
		if (anomalyP != kNoAnomaly)
		{
			switch(anomalyP)
			{
				case kAnomalyVertical:
					switch(anomalyQ)
					{
						case kNoAnomaly:
							// P is vertical, Q is normal
							pBoundsCheck = kBoundsCheckY;
							interX = ODFixedToFloat(p0.x);
							interY = interX * slopeQ + constantQ;
							break;
						case kAnomalyVertical:
						case kAnomalyPoint:
							return kNoIntersection;
							break;
					}
					break;
				case kAnomalyPoint:
					return kNoIntersection;
					break;
			}
		}
		else
		{
			// Q is an anomaly
			// P is a normal line
			switch (anomalyQ)
			{
				case kAnomalyVertical:
					// Q is vertical, P is normal
					qBoundsCheck = kBoundsCheckY;
					interX = ODFixedToFloat(q0.x);
					interY = interX * slopeP + constantP;
					break;
				case kAnomalyPoint:
					return kNoIntersection;
					break;
			}
				
		}
	}

	// Must be an intersection at this point

	sect->x = ODFloatToFixed(interX);
	sect->y = ODFloatToFixed(interY);

	if (pBoundsCheck == kBoundsCheckX)
		inside = InRange(sect->x, p0.x, p1.x);
	else
		inside = InRange(sect->y, p0.y, p1.y);

	if (inside)
	{
		if (qBoundsCheck == kBoundsCheckX)
			inside = InRange(sect->x, q0.x, q1.x);
		else
			inside = InRange(sect->y, q0.y, q1.y);
	}

	if (inside)
		return kIntersection;
	else
		return kOutsideIntersection;
}

eAnomaly ProcessLine(const ODPoint &p0, const ODPoint &p1,
                     double *slope, double *constant)
{
	double p0x = ODFixedToFloat(p0.x);
	double p0y = ODFixedToFloat(p0.y);
	double p1x = ODFixedToFloat(p1.x);
	double p1y = ODFixedToFloat(p1.y);

	if (p0x == p1x)
	{
		if (p0y == p1y)
			return kAnomalyPoint;
		else
			return kAnomalyVertical;
	}
	
	*slope = (p1y - p0y) / (p1x - p0x);
	*constant = p0y - (p0x * *slope);

	return kNoAnomaly;
}
#else // !defined(_PLATFORM_WIN32_) && defined(_PLATFORM_OS2_) && defined(_PLATFORM_UNIX_)
// Matt's conversion to "double" data type.  Getting round-off error.
WIN32_DLLEXPORT IntersectionStatus
IntersectLines( const ODPoint &p0, const ODPoint &p1,
			    const ODPoint &q0, const ODPoint &q1,
			    ODPoint *sect )
{
	double_t temp;
	double_t p0x = ODFixedToFloat(p0.x);
	double_t p0y = ODFixedToFloat(p0.y);
	double_t p1x = ODFixedToFloat(p1.x);
	double_t p1y = ODFixedToFloat(p1.y);
	double_t q0x = ODFixedToFloat(q0.x);
	double_t q0y = ODFixedToFloat(q0.y);
	double_t q1x = ODFixedToFloat(q1.x);
	double_t q1y = ODFixedToFloat(q1.y);
	
	// Construct the augmented matrix:
	
	double_t AB[2][3];
	AB[0][0] = p1y - p0y;
	AB[0][1] = p0x - p1x;
	AB[0][2] = AB[0][0]*p0x + AB[0][1]*p0y;
	
	AB[1][0] = q1y - q0y;
	AB[1][1] = q0x - q1x;
	AB[1][2] = AB[1][0]*q0x + AB[1][1]*q0y;
	
	// Find the largest element (in absolute value):

	double_t pmax = fabs(AB[0][0]);
	char pwhich = A00;
	temp = fabs(AB[0][1]);
	if( temp>pmax ) {
		pmax = temp;
		pwhich = A01;
	}

	double_t qmax = fabs(AB[1][0]);
	char qwhich = A10;
	temp = fabs(AB[1][1]);
	if( temp>qmax ) {
		qmax = temp;
		qwhich = A11;
	}
	
	double_t max;
	char which;
	if( pmax > qmax ) {
		max = pmax;
		which = pwhich;
	} else {
		max = qmax;
		which = qwhich;
	}
	
	// Give up if entire matrix is zero (we were given two degenerate line segments)
	if( max==0 )
		return kNoIntersection;
	
	// Pivot around the largest element and back-substitute. If we're about to divide
	// by zero, this means the two lines are parallel (or one segment is degenerate.)
	
	double_t sectx, secty;
	
	switch( which ) {
		case A00:										// p's Æy is the greatest
			temp = AB[1][0] / AB[0][0];
			AB[1][1] -= temp * AB[0][1];
			AB[1][2] -= temp * AB[0][2];
			if( fabs(AB[1][1]) < 0.0001 )
				return kNoIntersection;
			secty = AB[1][2] / AB[1][1];
			sectx = (AB[0][2] - AB[0][1]*secty) / AB[0][0];
			break;
		case A01:										// p's Æx is the greatest
			temp = AB[1][1] / AB[0][1];
			AB[1][0] -= temp * AB[0][0];
			AB[1][2] -= temp * AB[0][2];
			if( fabs(AB[1][0]) < 0.0001 )
				return kNoIntersection;
			sectx = AB[1][2] / AB[1][0];
			secty = (AB[0][2] - AB[0][0]*sectx) / AB[0][1];
			break;
		case A10:										// q's Æy is the greatest
			temp = AB[0][0] / AB[1][0];
			AB[0][1] -= temp * AB[1][1];
			AB[0][2] -= temp * AB[1][2];
			if( fabs(AB[0][1]) < 0.0001 )
				return kNoIntersection;
			secty =  AB[0][2] / AB[0][1];
			sectx = (AB[1][2] - AB[1][1]*secty) / AB[1][0];
			break;
		case A11:										// q's Æx is the greatest
			temp = AB[0][1] / AB[1][1];
			AB[0][0] -= temp * AB[1][0];
			AB[0][2] -= temp * AB[1][2];
			if( fabs(AB[0][0]) < 0.0001 )
				return kNoIntersection;
			sectx = AB[0][2] / AB[0][0];
			secty = (AB[1][2] - AB[1][0]*sectx) / AB[1][1];
			break;
	}
	
	if( fabs(sectx)>32767.0 || fabs(secty)>32767.0 )
		return kNoIntersection;
	sect->x = ODFloatToFixed(sectx);
	sect->y = ODFloatToFixed(secty);
	
	// Check whether the intersection is between endpoints of p and q.
	// Use max delta of p and q to test for inclusion:
	ODBoolean inside;
	if( pwhich==A00 )
		inside = InRange(sect->y, p0.y,p1.y);
	else
		inside = InRange(sect->x, p0.x,p1.x);
	if( inside )
		if( qwhich==A11 )
			inside = InRange(sect->x, q0.x,q1.x);
		else
			inside = InRange(sect->y, q0.y,q1.y);

	if( inside )
		return kIntersection;
	else
		return kOutsideIntersection;
	
}

	// ORIGINAL FIXED-POINT VERSION:
	IntersectionStatus
	IntersectLines( const ODPoint &p0, const ODPoint &p1,
				    const ODPoint &q0, const ODPoint &q1,
				    ODPoint *sect )
	{
		ODCoordinate temp;
		
		// Construct the augmented matrix:
		
		ODCoordinate AB[2][3];
		AB[0][0] = p1.y - p0.y;
		AB[0][1] = p0.x - p1.x;
		AB[0][2] = FixMul(AB[0][0],p0.x) + FixMul(AB[0][1],p0.y);
		
		AB[1][0] = q1.y - q0.y;
		AB[1][1] = q0.x - q1.x;
		AB[1][2] = FixMul(AB[1][0],q0.x) + FixMul(AB[1][1],q0.y);
		
		// Find the largest element (in absolute value):
	
		ODCoordinate pmax = Abs(AB[0][0]);
		char pwhich = A00;
		temp = Abs(AB[0][1]);
		if( temp>pmax ) {
			pmax = temp;
			pwhich = A01;
		}
	
		ODCoordinate qmax = Abs(AB[1][0]);
		char qwhich = A10;
		temp = Abs(AB[1][1]);
		if( temp>qmax ) {
			qmax = temp;
			qwhich = A11;
		}
		
		ODCoordinate max;
		char which;
		if( pmax > qmax ) {
			max = pmax;
			which = pwhich;
		} else {
			max = qmax;
			which = qwhich;
		}
		
		// Give up if entire matrix is zero (we were given two degenerate line segments)
		if( max==0 )
			return kNoIntersection;
		
		// Pivot around the largest element and back-substitute. If we're about to divide
		// by zero, this means the two lines are parallel (or one segment is degenerate.)
		
		switch( which ) {
			case A00:										// p's Æy is the greatest
				temp = ODFractDivide(AB[1][0],AB[0][0]);
				AB[1][1] -= FracMul(temp, AB[0][1]);
				AB[1][2] -= FracMul(temp, AB[0][2]);
				if( AB[1][1]==0 )
					return kNoIntersection;
				sect->y = FixDiv(AB[1][2], AB[1][1]);
				sect->x = FixDiv(AB[0][2] - FixMul(AB[0][1],sect->y), AB[0][0]);
				break;
			case A01:										// p's Æx is the greatest
				temp = ODFractDivide(AB[1][1],AB[0][1]);
				AB[1][0] -= FracMul(temp, AB[0][0]);
				AB[1][2] -= FracMul(temp, AB[0][2]);
				if( AB[1][0]==0 )
					return kNoIntersection;
				sect->x = FixDiv(AB[1][2], AB[1][0]);
				sect->y = FixDiv(AB[0][2] - FixMul(AB[0][0],sect->x), AB[0][1]);
				break;
			case A10:										// q's Æy is the greatest
				temp = ODFractDivide(AB[0][0],AB[1][0]);
				AB[0][1] -= FracMul(temp, AB[1][1]);
				AB[0][2] -= FracMul(temp, AB[1][2]);
				if( AB[0][1]==0 )
					return kNoIntersection;
				sect->y = FixDiv(AB[0][2], AB[0][1]);
				sect->x = FixDiv(AB[1][2] - FixMul(AB[1][1],sect->y), AB[1][0]);
				break;
			case A11:										// q's Æx is the greatest
				temp = ODFractDivide(AB[0][1],AB[1][1]);
				AB[0][0] -= FracMul(temp, AB[1][0]);
				AB[0][2] -= FracMul(temp, AB[1][2]);
				if( AB[0][0]==0 )
					return kNoIntersection;
				sect->x = FixDiv(AB[0][2], AB[0][0]);
				sect->y = FixDiv(AB[1][2] - FixMul(AB[1][0],sect->x), AB[1][1]);
				break;
		}
		
		// Check whether the intersection is between endpoints of p and q.
		// Use max delta of p and q to test for inclusion:
		ODBoolean inside;
		if( pwhich==A00 )
			inside = InRange(sect->x, p0.x,p1.x);
		else
			inside = InRange(sect->y, p0.y,p1.y);
		if( inside )
			if( qwhich==A11 )
				inside = InRange(sect->x, q0.x,q1.x);
			else
				inside = InRange(sect->y, q0.y,q1.y);
	
		if( inside )
			return kIntersection;
		else
			return kOutsideIntersection;
		
	}
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)


//------------------------------------------------------------------------------
// IntersectSegments
//
// Find the intersection of the two line segments determined by two pairs of points.
// This is often faster than IntersectLines in that it does not bother to compute the
// outside intersection point if the two segments do not intersect. In other words:
// ** sect will only be valid if the function returns true!
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODBoolean
IntersectSegments( const ODPoint &p0, const ODPoint &p1,
				   const ODPoint &q0, const ODPoint &q1,
				   ODPoint *sect )
{
	if( RangesDisjoint(p0.x,p1.x, q0.x,q1.x) || RangesDisjoint(p0.y,p1.y, q0.y,q1.y) )
		return kODFalse;
	else
		return IntersectLines(p0,p1, q0,q1, sect) == kIntersection;
}


//------------------------------------------------------------------------------
// GetIntersectionT
//
// Determines the "t" value (fractional distance) of a point on a segment.
// This function assumes that sect is a point on the segment (p0,p1).
// It returns 0.0 if sect=p0, 1.0 if sect=p1, and fractional values in between.
//------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
Fract
#else
WIN32_DLLEXPORT ODFract
#endif
GetIntersectionT( const ODPoint &p0, const ODPoint &p1, const ODPoint &sect )
{
	ODCoordinate dx = p1.x - p0.x;
	ODCoordinate dy = p1.y - p0.y;
	
	if( labs(dx) > labs(dy) )
		return ODFractDivide( sect.x-p0.x, dx );			// fixed / fixed = fract
	else
		return ODFractDivide( sect.y-p0.y, dy );
}


//------------------------------------------------------------------------------
// GetLineXAtY
//
// Determines the x position of a line at a given y position (i.e. the x coord
// of the intersection of the line and an infinite horizontal line at y.)
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODCoordinate
GetLineXAtY( const ODPoint &p0, const ODPoint &p1, ODCoordinate y )
{
	if( p0.x==p1.x )
		return p0.x;							// Line is vertical
	else if( y==p0.y )
		return p0.x;							// One endpoint is at y
	else if( y==p1.y )
		return p1.x;							// Other endpoint is at y
	else {										// General case:
		WASSERTMSG_DEBUG(p0.y!=p1.y,"Can't get x intercept of horiz line");
		ODPoint q0 (p0.x,y);
		ODPoint q1 (p1.x,y);
		ODPoint sect;
		IntersectLines(p0,p1,q0,q1,&sect);
		return sect.x;
	}
}


//------------------------------------------------------------------------------
// Distance
//
// Returns the distance between two points, or kODFixedInfinity on overflow.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODCoordinate
Distance( const ODPoint &p0, const ODPoint &p1 )
{
	ODCoordinate dx = p1.x-p0.x;
	ODCoordinate dy = p1.y-p0.y;
	
	// If dx or dy overflow a signed number, then so must the distance:
	if( (p1.x<p0.x) != (dx<0)  ||  (p1.y<p0.y) != (dy<0) )
		return kODFixedInfinity;
	
	if( dx==0 )
		return Abs(dy);
	else if( dy==0 )
		return Abs(dx);
	
	ODWide dx2, dy2;
	ODWideMultiply(dx,dx,&dx2);						// (16.16)^2 is 32.32
	ODWideMultiply(dy,dy,&dy2);
	ODWideAdd(&dx2,&dy2);
	
	ODCoordinate result = ODWideSquareRoot(&dx2);	// sqrt(32.32) is 16.16!
	if( result >= 0 )
		return result;
	else
		return kODFixedInfinity;					// Overflow!
}


//------------------------------------------------------------------------------
// GetLineShift
//
// Return an offset by which to shift points on a line to move them a certain
// distance to the left (in Mac coordinates) parallel to the line.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
GetLineShift( ODPoint p0, ODPoint p1, ODCoordinate dist, ODPoint &delta )
{
	if( p0.x==p1.x ) {
		if( p1.y<p0.y )
			dist = -dist;
		delta.x = dist;
		delta.y = 0;
		
	} else if( p0.y==p1.y ) {
		if( p1.x>p0.x )
			dist = -dist;
		delta.x = 0;
		delta.y = dist;
		
	} else {
		ODCoordinate len = Distance(p0,p1);
		if( len==kODFixedInfinity ) {
			// Distance overflows, so just scale things down
			p0.x >>= 2;		p0.y >>= 2;
			p1.x >>= 2;		p1.y >>= 2;
			len = Distance(p0,p1);
		}
		ODWide w;
		ODFixed rem;
		ODWideMultiply(dist,p1.y-p0.y, &w);		// delta.x =  (dist*dy)/len
		delta.x = ODWideDivide(&w,len, &rem);
		ODWideMultiply(dist,p0.x-p1.x, &w);		// delta.y = -(dist*dx)/len
		delta.y = ODWideDivide(&w,len, &rem);
	}
}
