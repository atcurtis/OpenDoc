/* @(#)Z 1.27 com/src/pubutils/src/AltPoint.cpp, odpubutils, od96os2, odos29646d 96/11/15 15:29:05 (96/10/30 09:23:46) */
 //====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odpubutils
//
//   CLASSES: none
//
//   ORIGINS: 82,27,94
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
	File:		AltPoint.cpp

	Contains:	Alternate ODPoint, ODRect (C++ savvy)

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#ifndef SOM_Module_OpenDoc_Global_Types_defined
#include "ODTypesM.xh"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _LINEOPS_
#include "LineOps.h"		/* For kFixedEpsilon */
#endif

#include <string.h>


#ifdef _PLATFORM_MACINTOSH_
	#ifndef __GXMATH__
	#include <GXMath.h>
	#endif
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) && defined(DEBUG)
#include <stdio.h>

// define vars

char tBuf[1024];
static HWND testWindow = kODNULL;

// define macros

#ifdef _PLATFORM_WIN32_
#define CHECKRES(x) \
  if( ( x ) ) { \
    if (testWindow) { \
      sprintf(tBuf, "PASS\n"); \
      SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tBuf); \
    } \
    else { \
      printf("PASS\n"); \
    } \
  } \
  else { \
    if (testWindow) { \
      sprintf(tBuf, "FAIL\n"); \
      SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tBuf); \
    } \
    else { \
      printf("FAIL\n"); \
    } \
  }
#elif defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#define CHECKRES(x) \
  if( ( x ) ) \
      printf("PASS\n"); \
  else \
      printf("FAIL\n");
#endif

#define myLine "-----------------------------------------------------\n"
#endif // defined (_PLATFORM_WIN32_ || OS2 || AIX ) && defined(DEBUG)

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODShape
#endif  // _PLATFORM_MACINTOSH_

//==============================================================================
// ODPoint
//==============================================================================


#ifdef _PLATFORM_MACINTOSH_
ODPoint::ODPoint( Point qdpt )
{
	x = IntToFixed(qdpt.h);
	y = IntToFixed(qdpt.v);
}
#endif

#ifdef _PLATFORM_WIN32_
WIN32_DLLEXPORT ODPoint::ODPoint( Point pt )
{
	x = IntToFixed(pt.x);
	y = IntToFixed(pt.y);
}
#endif

#ifdef _PLATFORM_OS2_
ODPoint::ODPoint( POINTL ptl)
{
  x = IntToFixed(ptl.x);
  y = IntToFixed(ptl.y);
}
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_AIX_
ODPoint::ODPoint( Point pt )
{
	x = IntToFixed(pt.x);
	y = IntToFixed(pt.y);
}
#endif

WIN32_DLLEXPORT ODPoint::ODPoint( const ODPoint &pt )
{
	x = pt.x;
	y = pt.y;
}


WIN32_DLLEXPORT ODPoint& ODPoint::operator= ( const ODPoint &pt )
{
	x = pt.x;
	y = pt.y;
	return *this;
}


#ifdef _PLATFORM_MACINTOSH_
ODPoint& ODPoint::operator= ( const Point &pt )
{
	x = IntToFixed(pt.h);
	y = IntToFixed(pt.v);
	return *this;
}
#endif

#ifdef _PLATFORM_WIN32_
WIN32_DLLEXPORT ODPoint& ODPoint::operator= ( const Point &pt )
{
	x = IntToFixed(pt.x);
	y = IntToFixed(pt.y);
	return *this;
}
#endif

#ifdef _PLATFORM_OS2_
ODPoint& ODPoint::operator= ( const POINTL &ptl)
{
	x = IntToFixed(ptl.x);
	y = IntToFixed(ptl.y);
	return *this;
}
#endif

#ifdef _PLATFORM_AIX_
ODPoint& ODPoint::operator= ( const Point &pt )
{
	x = IntToFixed(pt.x);
	y = IntToFixed(pt.y);
	return *this;
}
#endif
 
WIN32_DLLEXPORT void ODPoint::Offset ( ODCoordinate xx, ODCoordinate yy )
{
	x += xx;
	y += yy;
}


WIN32_DLLEXPORT void ODPoint::operator+= ( const ODPoint &pt )
{
	x += pt.x;
	y += pt.y;
}


WIN32_DLLEXPORT void ODPoint::operator-= ( const ODPoint &pt )
{
	x -= pt.x;
	y -= pt.y;
}


#ifdef _PLATFORM_MACINTOSH_
void ODPoint::operator+= ( const Point &pt )
{
	x += IntToFixed(pt.h);
	y += IntToFixed(pt.v);
}


void ODPoint::operator-= ( const Point &pt )
{
	x -= IntToFixed(pt.h);
	y -= IntToFixed(pt.v);
}

Point
ODPoint::AsQDPoint( ) const
{
	Point pt;
	pt.h = FixedToInt(x);
	pt.v = FixedToInt(y);
	return pt;
}
#endif

#ifdef _PLATFORM_WIN32_
WIN32_DLLEXPORT void ODPoint::operator+= ( const Point &pt )
{
	x += IntToFixed(pt.x);
	y += IntToFixed(pt.y);
}


WIN32_DLLEXPORT void ODPoint::operator-= ( const Point &pt )
{
	x -= IntToFixed(pt.x);
	y -= IntToFixed(pt.y);
}

WIN32_DLLEXPORT Point
ODPoint::AsWinPoint( ) const
{
	Point pt;
	pt.x = FixedToInt(x);
	pt.y = FixedToInt(y);
	return pt;
}
#endif

#ifdef _PLATFORM_OS2_
void ODPoint::operator+= ( const POINTL &ptl )
{
	x += IntToFixed(ptl.x);
	y += IntToFixed(ptl.y);
}


void ODPoint::operator-= ( const POINTL &ptl )
{
	x -= IntToFixed(ptl.x);
	y -= IntToFixed(ptl.y);
}

POINTL
ODPoint::AsPOINTL( ) const
{
	POINTL ptl;
	ptl.x = FixedToInt(x);
	ptl.y = FixedToInt(y);
	return ptl;
}
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_AIX_
void ODPoint::operator+= ( const Point &pt )
{
	x += IntToFixed(pt.x);
	y += IntToFixed(pt.y);
}


void ODPoint::operator-= ( const Point &pt )
{
	x -= IntToFixed(pt.x);
	y -= IntToFixed(pt.y);
}

Point
ODPoint::AsXPoint( ) const
{
	Point pt;
	pt.x = FixedToInt(x);
	pt.y = FixedToInt(y);
	return pt;
}
#endif
  
WIN32_DLLEXPORT ODSShort
ODPoint::IntX( ) const
{
	return FixedToInt(x);
}


WIN32_DLLEXPORT ODSShort
ODPoint::IntY( ) const
{
	return FixedToInt(y);
}


WIN32_DLLEXPORT ODBoolean
ODPoint::operator==( const ODPoint &pt ) const
{
	return x==pt.x && y==pt.y;
}


WIN32_DLLEXPORT ODBoolean
ODPoint::operator!=( const ODPoint &pt ) const
{
	return x!=pt.x || y!=pt.y;
}


WIN32_DLLEXPORT ODBoolean
ODPoint::ApproxEquals( const ODPoint& pt ) const
{
	ODFixed delta;
	delta = x-pt.x;
	if( delta>kFixedEpsilon || delta<-kFixedEpsilon )
		return kODFalse;
	delta = y-pt.y;
	if( delta>kFixedEpsilon || delta<-kFixedEpsilon )
		return kODFalse;
	return kODTrue;
}


//==============================================================================
// ODRect
//==============================================================================


#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODShape
#endif  // _PLATFORM_MACINTOSH_


#ifdef _PLATFORM_OS2_
#define ASSERTVALID()		WASSERT(right>=left && top>=bottom)
#else
#define ASSERTVALID()		WASSERT(right>=left && bottom>=top)
#endif // _PLATFORM_OS2_ 


//------------------------------------------------------------------------------
// ::ODRect( corner1, corner2 )
//
// Construct a rectangle given two opposite corners (not necessarily topLeft,botRight)
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODRect::ODRect( const ODPoint &a, const ODPoint &b )
{
	this->Set(a,b);
}


//------------------------------------------------------------------------------
// ::ODRect( topLeft, width, height )
//
// Construct a rectangle given its origin, width and height.
//------------------------------------------------------------------------------

#ifdef _PLATFORM_OS2_
ODRect::ODRect( const ODPoint &bottomLeft, ODCoordinate width, ODCoordinate height )
{
	WASSERT(width>=0);
	WASSERT(height>=0);
	
	left = bottomLeft.x;
	right = bottomLeft.x + width;
	top  = bottomLeft.y;
        bottom= bottomLeft.y + height;
}

#else
WIN32_DLLEXPORT ODRect::ODRect( const ODPoint &topLeft, ODCoordinate width, ODCoordinate height )
{
	WASSERT(width>=0);
	WASSERT(height>=0);
	
	left = topLeft.x;
	right = topLeft.x + width;
	top  = topLeft.y;
        bottom= topLeft.y + height;
}
#endif

#if defined _PLATFORM_MACINTOSH_ || _PLATFORM_WIN32_
//------------------------------------------------------------------------------
// ::ODRect( Rect )
//
// Construct an ODRect from a QuickDraw Rect
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODRect::ODRect( const Rect &r )
{
	left = ff(r.left);		right = ff(r.right);
	top  = ff(r.top);		bottom= ff(r.bottom);
}


//------------------------------------------------------------------------------
// ::= Rect
//
// Set an ODRect from a QuickDraw Rect
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODRect&
ODRect:: operator= ( const Rect &r )
{
	left = ff(r.left);		right = ff(r.right);
	top  = ff(r.top);		bottom= ff(r.bottom);
	ASSERTVALID();
	return *this;
}
#endif

#if defined _PLATFORM_AIX_
//------------------------------------------------------------------------------
// ::ODRect( Rect )
//
// Construct an ODRect from an AIX Rect (XRectangle)
//------------------------------------------------------------------------------

ODRect::ODRect( const Rect &r )
{
	left = ff(r.x);		right = ff(r.x + r.width);
	top  = ff(r.y);		bottom= ff(r.y + r.height);
}


//------------------------------------------------------------------------------
// ::= Rect
//
// Set an ODRect from a QuickDraw Rect
//------------------------------------------------------------------------------

ODRect&
ODRect:: operator= ( const Rect &r )
{
	left = ff(r.x);		right = ff(r.x + r.width);
	top  = ff(r.y);		bottom= ff(r.y + r.height);
	ASSERTVALID();
	return *this;
}
#endif  // _PLATFORM_AIX_


#ifdef _PLATFORM_OS2_
//------------------------------------------------------------------------------
// ::ODRect( RECTL )
//
// Construct an ODRect from a GPI RECTL
//------------------------------------------------------------------------------
ODRect::ODRect( const RECTL &r )
{
  left = IntToFixed(r.xLeft);
  right = IntToFixed(r.xRight); 
  top = IntToFixed(r.yTop);
  bottom = IntToFixed(r.yBottom);
}

//------------------------------------------------------------------------------
// ::= Rect
//
// Set an ODRect from a GPI RECTL
//------------------------------------------------------------------------------
ODRect&
ODRect:: operator= ( const RECTL &r )
{
  left = IntToFixed(r.xLeft);
  right = IntToFixed(r.xRight); 
  top = IntToFixed(r.yTop);
  bottom = IntToFixed(r.yBottom);
  ASSERTVALID();
  return *this;
}
#endif // _PLATFORM_OS2_

//------------------------------------------------------------------------------
// ::Set
//
// Set all four coordinates of a rectangle. Warn if the resulting rect is bogus.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::Set( ODCoordinate l, ODCoordinate t, ODCoordinate r, ODCoordinate b )
{
	left = l;				right = r;
	top  = t;				bottom= b;
	ASSERTVALID();
}


//------------------------------------------------------------------------------
// ::Set
//
// Set all four coordinates of a rectangle. Warn if the resulting rect is bogus.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::Set( const ODPoint &origin, ODCoordinate width, ODCoordinate height )
{
	WASSERT(width>=0);
	WASSERT(height>=0);
	left = origin.x;
        right = left+width;
#ifdef _PLATFORM_OS2_ 
        bottom = origin.y; 
        top = bottom + height; 
#else 
	top  = origin.y;
        bottom= top +height;
#endif

}


//------------------------------------------------------------------------------
// ::Set
//
// Construct a rectangle given two opposite corners (not necessarily topLeft,botRight)
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::Set( const ODPoint &a, const ODPoint &b )
{
	if( a.x<b.x ) {
		left = a.x;
		right= b.x;
	} else {
		left = b.x;
		right= a.x;
	}
#ifdef _PLATFORM_OS2_ 
        if( b.y < a.y ) { 
#else 
	if( a.y<b.y ) {
#endif
		top   = a.y;
		bottom= b.y;
	} else {
		top   = b.y;
		bottom= a.y;
	}
}


//------------------------------------------------------------------------------
// ::SetInt
//
// Set coordinates of a rectangle from integers. Warn if the resulting rect is bogus.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::SetInt( short l, short t, short r, short b )
{
	left = ff(l);			right = ff(r);
	top  = ff(t);			bottom= ff(b);
	ASSERTVALID();
}


//------------------------------------------------------------------------------
// ::Offset
//
// Move a rectangle.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::Offset( ODCoordinate x, ODCoordinate y )
{
	left += x;				right += x;
	top  += y;				bottom+= y;
}

WIN32_DLLEXPORT void
ODRect::Offset( const ODPoint &pt )
{
	left += pt.x;			right += pt.x;
	top  += pt.y;			bottom+= pt.y;
}

#if 0
#ifdef _PLATFORM_WIN32_
void
ODRect::Transform( const void *xform )
{
	*((ODPoint*)&left) = ((ODTransform *)xform)->TransformPoint(*((ODPoint*)&left));
	*((ODPoint*)&right) = ((ODTransform *)xform)->TransformPoint(*((ODPoint*)&right));
}
#endif
#endif

//------------------------------------------------------------------------------
// ::Inset
//
// Inset the edges of a rectangle, without collapsing to negative dimensions.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::Inset( ODCoordinate x, ODCoordinate y )
{
	left += x;
	right = Max(left,right-x);
#ifdef _PLATFORM_OS2_ 
        bottom += y; 
        top = Max( bottom, top - y ); 
#else 
	top += y;
	bottom = Max(top,bottom-y);
#endif
}


//------------------------------------------------------------------------------
// ::Clear
//
// Zero all coordinates
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::Clear( )
{
	left = right = top = bottom = 0;
}


//------------------------------------------------------------------------------
// :: &=
//
// Intersect me with another rectangle (result stored in me)
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect:: operator&= ( const ODRect &r )
{
	left = Max(left,r.left);
        right = Min(right, r.right);
#ifdef _PLATFORM_OS2_ 
        top = Min( top, r.top ); 
        bottom = Max( bottom, r.bottom ); 
#else 
	top  = Max(top, r.top);
        bottom= Min(bottom,r.bottom);
#endif
	if( this->IsEmpty() )
		this->Clear();
}


//------------------------------------------------------------------------------
// :: |=
//
// Union me with another rectangle (result stored in me)
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect:: operator|= ( const ODRect &r )
{
	if( this->IsEmpty() )
		*this = r;
	else if( !r.IsEmpty() ) {
		left = Min(left,r.left);
		right= Max(right,r.right);
#ifdef _PLATFORM_OS2_ 
                top = Max( top, r.top ); 
                bottom = Min( bottom, r.bottom ); 
#else 
		top  = Min(top,r.top);
		bottom=Max(bottom,r.bottom);
#endif
	}
}


//------------------------------------------------------------------------------
// :: |= ODPoint
//
// Union me with a point (expand to fit point)
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect:: operator|= ( const ODPoint &pt )
{
	left = Min(left,pt.x);
	right= Max(right,pt.x);
#ifdef _PLATFORM_OS2_ 
        top = Max( top, pt.y ); 
        bottom = Min( bottom, pt.y ); 
#else 
	top  = Min(top,pt.y);
	bottom=Max(bottom,pt.y);
#endif
}


#ifdef _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
// :: AsQDRect
//
// Turn me into a QuickDraw rect
//------------------------------------------------------------------------------

void
ODRect::AsQDRect( Rect &r ) const
{
	SetRect(&r, FixedToInt(left), FixedToInt(top), FixedToInt(right), FixedToInt(bottom));
}
#endif

#ifdef  _PLATFORM_WIN32_
//------------------------------------------------------------------------------
// :: AsWinRect
//
// Turn me into a Windows rect
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void
ODRect::AsWinRect( Rect &r ) const
{
	SetRect(&r, FixedToInt(left), FixedToInt(top), FixedToInt(right), FixedToInt(bottom));
}
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
//------------------------------------------------------------------------------
// :: AsRECTL
//
// Turn me into a GPI RECTL
//------------------------------------------------------------------------------

void ODRect::AsRECTL( RECTL& r) const
{

  r.xLeft = FixedToInt(left);
  r.xRight = FixedToInt(right);
  r.yTop = FixedToInt(top);
  r.yBottom = FixedToInt(bottom);
  

}
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_AIX_
//------------------------------------------------------------------------------
// :: AsXRect
//
// Turn me into a AIX rect (XRectangle)
//------------------------------------------------------------------------------

void
ODRect::AsXRect( Rect &r ) const
{
  r.x = FixedToInt(left);
  r.width = FixedToInt(right) - FixedToInt(left);
  r.y = FixedToInt(top);
  r.height = FixedToInt(bottom) - FixedToInt(top);
}
#endif  // _PLATFORM_AIX_

//------------------------------------------------------------------------------
// :: IsEmpty
//
// Do I have no area?
//------------------------------------------------------------------------------

#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif
ODRect::IsEmpty( ) const
{
#ifdef _PLATFORM_OS2_ 
        return (ODBoolean) ( right <= left ) || ( top <= bottom ); 
#else 
	return right<=left || bottom<=top;
#endif
}


//------------------------------------------------------------------------------
// :: Contains( Point )
//
// Do I contain a point? (Remember, I don't contain my right and bottom edges, 
//                                  or right and top edges for OS/2.) 
//------------------------------------------------------------------------------

#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif
ODRect::Contains( const ODPoint &pt ) const
{
	return left<=pt.x && pt.x<right
#ifdef _PLATFORM_OS2_ 
            && bottom <= pt.y && pt.y < top; 
#else 
	    && top <=pt.y && pt.y<bottom;
#endif
}


//------------------------------------------------------------------------------
// :: Contains( Rect )
//
// Do I contain an entire rect?
//------------------------------------------------------------------------------

#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_
ODRect::Contains( const ODRect &r ) const
{
#ifdef _PLATFORM_OS2_ 
		return left<=r.left && r.right<=right 
		    && top >=r.top  && r.bottom>=bottom; 
#else 
		return left<=r.left && r.right<=right
		    && top <=r.top  && r.bottom<=bottom;
#endif
}


//------------------------------------------------------------------------------
// :: ApproxContains( Rect )
//
// Do I approximately (within epsilon) contain an entire rect?
//------------------------------------------------------------------------------

#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_
ODRect::ApproxContains( const ODRect &r ) const
{
	if( r.right<=r.left || r.bottom<=r.top )
		return kODTrue;					// Empty rect contained in anything
	else
		return left-kFixedEpsilon<=r.left && r.right<=right+kFixedEpsilon
#ifdef _PLATFORM_OS2_ 
		    && bottom-kFixedEpsilon <=r.bottom  && r.top<=top+kFixedEpsilon; 
#else 
		    && top-kFixedEpsilon <=r.top  && r.bottom<=bottom+kFixedEpsilon;
#endif
}


//------------------------------------------------------------------------------
// :: ==
//
// Am I equal to another rect?
//------------------------------------------------------------------------------

#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_

ODRect::operator==( const ODRect &r ) const
{
	return memcmp(this,&r,sizeof(ODRect)) == 0;
}


//------------------------------------------------------------------------------
// :: ApproxEquals
//
// Am I approximately equal (within epsilon) to another rect?
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODBoolean
ODRect::ApproxEquals( const ODRect &r ) const
{
	return left-r.left<=kFixedEpsilon     && left-r.left>=-kFixedEpsilon
		&& right-r.right<=kFixedEpsilon   && right-r.right>=-kFixedEpsilon
		&& top-r.top<=kFixedEpsilon       && top-r.top>=-kFixedEpsilon
		&& bottom-r.bottom<=kFixedEpsilon && bottom-r.bottom>=-kFixedEpsilon;
}


//------------------------------------------------------------------------------
// :: Intersects
//
// Do I intersect another rectangle?
// (Remember, I don't contain my right and bottom edges.)
//------------------------------------------------------------------------------

#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_
ODRect::Intersects( const ODRect &r ) const
{
	return Max(left,r.left) < Min(right,r.right)
#ifdef _PLATFORM_OS2_ 
                && Min(top, r.top) > Max(bottom, r.bottom); 
#else 
		&& Max(top,r.top) < Min(bottom,r.bottom);
#endif
}


#if ( (defined (_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)) \
      || defined (_PLATFORM_AIX_) ) 
//=======================================================
// ODToolSpaceRect
//======================================================

//-----------------------------------------------------
// ODToolSpaceRect::ODToolSpaceRect
//----------------------------------------------------

WIN32_DLLEXPORT ODToolSpaceRect::ODToolSpaceRect(ODCoordinate l, ODCoordinate t,
                                 ODCoordinate r, ODCoordinate b, ODRect* aFloatRect)
{
  left = l; top = t; right = r; bottom = b;
  if (aFloatRect) {
    floatRect = *aFloatRect;
  } else {
    floatRect.Clear();
  }
}

//----------------------------------------------------
// ODToolSpaceRect::IsEmpty
//----------------------------------------------------

WIN32_DLLEXPORT ODBoolean ODToolSpaceRect::IsEmpty() const
{
  if ((left==0) && (top==0) && (right==0) && (bottom==0) && floatRect.IsEmpty()) {
    return(kODTrue);
  } else {
    return(kODFalse);
  }
}

//----------------------------------------------------
// ODToolSpaceRect::Set
//----------------------------------------------------

WIN32_DLLEXPORT void ODToolSpaceRect::Set(ODCoordinate l, ODCoordinate t,
                          ODCoordinate r, ODCoordinate b, ODRect* aFloatRect)
{
  left=l;
  top=t;
  right=r;
  bottom=b;
  if (aFloatRect) {
    floatRect = *aFloatRect;
  } else {
    floatRect.Clear();
  }
}

//----------------------------------------------------
// ODToolSpaceRect::Clear
//----------------------------------------------------

WIN32_DLLEXPORT void ODToolSpaceRect::Clear(void)
{
  left=0;
  top=0;
  right=0;
  bottom=0;
  floatRect.Clear();
}
#endif // _PLATFORM_WIN32_


#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) && defined(DEBUG)

/////////////////////////////////////////////////////////
// Testing code below
//
// I updated the SelfTest methods to optionally take a HWND.
// I use this in my PUTest test case.  This is a Public
// Utilities part that tests focuslib and calls some of
// these SelfTest methods.  So if you aren't wanting to 
// test from a full-blown part, just don't pass in a 
// HWND and write a simple command-line test case. Rob
/////////////////////////////////////////////////////////

WIN32_DLLEXPORT void ODPoint::SelfTest(HWND testWin)
// the  HWND is ignored for OS/2
{
#ifdef _PLATFORM_WIN32_
  if(testWin) {
    testWindow = testWin;
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)"\n\nBEGIN ODPoint::SelfTest\n\n");
  }
  else {
  printf ("\n\nBEGIN ODPoint::SelfTest\n\n");
  }
#else
  printf ("\n\nBEGIN ODPoint::SelfTest\n\n");
#endif

  ODCoordinate x1 = IntToFixed(50);
  ODCoordinate y1 = IntToFixed(100);

  // test default ctor
  ODPoint point2;


  ODPoint *point1 = new ODPoint(x1, y1);  
  point1->PrintIt("point1: testing 'new' of ODPoint::ODPoint");
  CHECKRES( (point1->x == x1) && (point1->y == y1) );

  // test operator = 
  ODPoint *point3 = point1;  
  point3->PrintIt("point3: testing ODPoint::operator=");
  CHECKRES( (point3->x == x1) && (point3->y == y1) );

  // test copy ctor
  ODPoint *point4 = new ODPoint(*point1);
  point4->PrintIt("point4: testing ODPoint copy ctor");
  CHECKRES( (point4->x == x1) && (point4->y == y1) );

  // Modification tests

  // test ::Clear()
  point1->Clear();
  point1->PrintIt("point1.Clear()");
  CHECKRES( (point1->x == 0) && (point1->y == 0) );
      
  // test ::Set()
  point2.Set( x1, y1);
  point2.PrintIt("point2.Set(50,100)");
  CHECKRES( (point2.x == x1) && (point2.y == y1) );

  // test ::Offset()
  point2.Set( x1, y1);
  point2.Offset( y1, x1);
  point2.PrintIt("point2.Offset(100, 50)"); 
  CHECKRES( (point2.x == x1+y1) && (point2.y == y1+x1) );


  // test operator+=
  point2.Set( x1, y1);
  point3->Set( x1, y1);
  point2 += *point3;
  point2.PrintIt("point2 operator+= point3"); 
  CHECKRES( (point2.x == x1+x1) && (point2.y == y1+y1) );

  // test operator-=
  point2.Set( x1, y1);
  point3->Set( x1, y1);
  point2 -= *point3;
  point2.PrintIt("point2 operator-= point3 "); 
  CHECKRES( (point2.x == 0) && (point2.y == 0) );

  // test ::IntX(), ::IntY()
  printf ("-----------------------------------------------------\n");
  point2.Set( x1, y1);
  ODSShort theX = FixedToInt(x1);
  ODSShort theY = FixedToInt(y1);
  ODSShort retX = point2.IntX();
  printf("IntX = %d\n", retX);
  CHECKRES( retX == theX );

  ODSShort retY = point2.IntY();
  printf("IntY = %d\n", retY);
  CHECKRES( retY == theY );

  // test operator==
  point1->PrintIt("point1 operator== point4");
  CHECKRES (*point1 == *point4);

  // test operator!=
  point1->PrintIt("point1 operator!= point4");
  CHECKRES (!(*point1 != *point4));
 
  // test ::ApproxEquals()
  point1->PrintIt("point1->ApproxEquals(point2)");
  CHECKRES (point1->ApproxEquals(point2) );

  // test ODPoint(Point) ctor
  Point p;
  p.x = 33;
  p.y = 99;
  ODPoint point5(p);
  point5.PrintIt("point5(Point(33,99))");
  CHECKRES ( (point5.x == IntToFixed(33)) && (point5.y == IntToFixed(99)) );

#ifdef _PLATFORM_WIN32_
  // test ::AsWinPoint()
  point1->PrintIt("testing AsWinPoint");
  Point winPoint = point1->AsWinPoint();
#elif _PLATFORM_OS2_
  // test ::AsPOINTL()
  point1->PrintIt("testing AsPOINTL");
  POINTL winPoint = point1->AsPOINTL();
#elif _PLATFORM_AIX_
  // test ::AsXPoint()
  point1->PrintIt("testing AsXPoint");
  Point winPoint = point1->AsXPoint();
#endif
  printf("winPoint.x = %d, winPoint.y = %d\n", winPoint.x, winPoint.y);
  CHECKRES ( (IntToFixed(winPoint.x) == point1->x) && (IntToFixed(winPoint.y) == point1->y) );

  // test Point operator+=
  winPoint.x = 33;
  winPoint.y = 99;
  *point1 += p;
  point1->PrintIt("testing point1 operator+= winPoint");
  CHECKRES ( (point1->x == IntToFixed(83)) && (point1->y == IntToFixed(199)) );

  // test Point operator+-
  *point1 -= p;
  point1->PrintIt("testing point1 operator+- winPoint");
  CHECKRES ( (point1->x == IntToFixed(50)) && (point1->y == IntToFixed(100)) );

  // delete point4
  delete point4;

#ifdef _PLATFORM_WIN32_
  if(testWin) {
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)"\n\nEND ODPoint::SelfTest\n\n");
  }
  else {
    printf ("\n\nEND ODPoint::SelfTest\n\n");
  } 
#else
    printf ("\n\nEND ODPoint::SelfTest\n\n");
#endif
  // end of unit test
}

WIN32_DLLEXPORT void ODPoint::PrintIt(char *msg)
{
#ifdef _PLATFORM_WIN32_
  if(!testWindow) {
    printf (myLine);
    if(msg != NULL)
      printf ("%s\n",msg);
    printf ("X-Coordinate is: %d\n", x);
    printf ("Y-Coordinate is: %d\n", y);
  }
  else {
    char tmpBuff[1024];
    sprintf (tmpBuff, myLine);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    if(msg != NULL) {
      sprintf (tmpBuff, "%s\n",msg );
      SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    }
    sprintf (tmpBuff, "X-Coordinate is: %d\n", x);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    sprintf (tmpBuff, "Y-Coordinate is: %d\n", y);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
  }
#else
  printf (myLine);
  if(msg != NULL)
    printf ("%s\n",msg);
  printf ("X-Coordinate is: %d\n", x);
  printf ("Y-Coordinate is: %d\n", y);
#endif
  
}

WIN32_DLLEXPORT void ODRect::SelfTest(HWND testWin) 
{
 
#ifdef _PLATFORM_OS2_
  ODCoordinate myLeft = IntToFixed(20);
  ODCoordinate myTop = IntToFixed(100);
  ODCoordinate myRight = IntToFixed(100);
  ODCoordinate myBottom = IntToFixed(20);
#else
  ODCoordinate myLeft = IntToFixed(20);
  ODCoordinate myTop = IntToFixed(20);
  ODCoordinate myRight = IntToFixed(100);
  ODCoordinate myBottom = IntToFixed(100);
#endif  // _PLATFORM_OS2_

  ODPoint point1(IntToFixed(20), IntToFixed(20));
  ODPoint point2(IntToFixed(80), IntToFixed(80));

  Rect myRect;
#ifdef _PLATFORM_WIN32_
  myRect.left = 40;
  myRect.top = 40;
  myRect.right = 90;
  myRect.bottom = 90;
#elif _PLATFORM_OS2_
  myRect.xLeft = 40;
  myRect.yTop = 90;
  myRect.xRight = 90;
  myRect.yBottom = 40;
#elif _PLATFORM_AIX_
  myRect.x = 40;
  myRect.y = 40;
  myRect.width = 50;
  myRect.height = 50;
#endif

#ifdef _PLATFORM_WIN32_
  if(testWin) {
    testWindow = testWin;
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)"\n\nBEGIN ODRect::SelfTest\n\n");
  }
  else {
    printf ("\n\nBEGIN ODRect::SelfTest\n\n");
  }
#else
  printf ("\n\nBEGIN ODRect::SelfTest\n\n");
#endif

  // test default ctor
  ODRect rect1;

  // test ctor
  ODRect rect2(myLeft, myTop, myRight, myBottom);
  rect2.PrintIt("rect2: testing ODRect::ODRect");
  CHECKRES ( ( (rect2.left == myLeft) && (rect2.top == myTop) 
    && (rect2.right == myRight) && (rect2.bottom == myBottom) ) );

  // test ctor
  ODRect rect3(point1, point2);
  rect3.PrintIt("rect3: testing ODRect::ODRect(ODPoint, ODPoint) ctor");
#ifdef _PLATFORM_OS2_
  CHECKRES ( ( (rect3.left == point1.x) && (rect3.top == point2.y) 
    && (rect3.right == point2.x) && (rect3.bottom == point1.y) ) );
#else
  CHECKRES ( ( (rect3.left == point1.x) && (rect3.top == point1.y) 
    && (rect3.right == point2.x) && (rect3.bottom == point2.y) ) );
#endif

  // test ctor
  ODRect rect4(point1, myRight, myBottom);
  rect4.PrintIt("rect4: testing ODRect::ODRect(ODPoint, ODCoordinate, ODCoordinate) ctor");
  CHECKRES ( ( (rect4.left == point1.x) && (rect4.top == point1.y) 
    && (rect4.right == point1.x + myRight) && (rect4.bottom == point1.y + myBottom) ) );

  // test ctor
  ODRect rect5(myRect);
  rect5.PrintIt("rect5: testing ODRect::ODRect(Rect)");
#ifdef _PLATFORM_WIN32_
  CHECKRES ( ( (rect5.left == IntToFixed(myRect.left)) && (rect5.top == IntToFixed(myRect.top) ) 
    && (rect5.right == IntToFixed(myRect.right) ) && (rect5.bottom == IntToFixed(myRect.bottom )) ) );
#elif defined(_PLATFORM_OS2_)
  CHECKRES ( ( (rect5.left == IntToFixed(myRect.xLeft)) && (rect5.top == IntToFixed(myRect.yTop) ) 
    && (rect5.right == IntToFixed(myRect.xRight) ) && (rect5.bottom == IntToFixed(myRect.yBottom )) ) );
#elif defined(_PLATFORM_AIX_)
  CHECKRES ( ( (rect5.left == IntToFixed(myRect.x)) && (rect5.top == IntToFixed(myRect.y) ) 
    && (rect5.right == IntToFixed(myRect.x + myRect.width) ) && (rect5.bottom == IntToFixed(myRect.y + myRect.height )) ) );
#endif

  // test operator=
  ODRect rect6 = myRect;
  rect6.PrintIt("rect6: testing ODRect::operator= Rect");
#ifdef _PLATFORM_WIN32_
  CHECKRES ( ( (rect6.left == IntToFixed(myRect.left) ) && (rect6.top == IntToFixed(myRect.top) ) 
    && (rect6.right == IntToFixed(myRect.right) ) && (rect6.bottom == IntToFixed(myRect.bottom) ) ) );
#elif defined(_PLATFORM_OS2_)
  CHECKRES ( ( (rect6.left == IntToFixed(myRect.xLeft) ) && (rect6.top == IntToFixed(myRect.yTop) ) 
    && (rect6.right == IntToFixed(myRect.xRight) ) && (rect6.bottom == IntToFixed(myRect.yBottom) ) ) );
#elif defined(_PLATFORM_AIX_)
  CHECKRES ( (rect6.left == IntToFixed(myRect.x)) && (rect6.top == IntToFixed(myRect.y)) 
    && (rect6.right == (IntToFixed(myRect.x) + IntToFixed(myRect.width))) && (rect6.bottom == (IntToFixed(myRect.y) + IntToFixed(myRect.height))) );
#endif

  // test ::Clear()
  rect2.Clear();
  rect2.PrintIt("rect2: testing ODRect::Clear()");
  CHECKRES ( ( (rect2.left == 0) && (rect2.top == 0) 
    && (rect2.right == 0) && (rect2.bottom == 0) ) );

  // test ::Set()
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect1.PrintIt("rect1: testing ODRect::Set() using coordinates");
  CHECKRES ( ( (rect1.left == myLeft) && (rect1.top == myTop) 
    && (rect1.right == myRight) && (rect1.bottom == myBottom) ) );

  // test ::Set()
  rect1.Set(point1, myRight, myBottom); 
  rect1.PrintIt("rect1: testing ODRect::Set() using points and coordinates");
#ifdef _PLATFORM_OS2_
  CHECKRES ( ( (rect1.left == point1.x) && (rect1.top == point1.y + myBottom) 
    && (rect1.right == point1.x + myRight) && (rect1.bottom == point1.y) ) );
#else
  CHECKRES ( ( (rect1.left == point1.x) && (rect1.top == point1.y) 
    && (rect1.right == point1.x + myRight) && (rect1.bottom == point1.y + myBottom) ) );
#endif

  // test ::Set()
  rect1.Set(point1, point2);
  rect1.PrintIt("rect1: testing ODRect::Set() using points");
#ifdef _PLATFORM_OS2_
  CHECKRES ( ( (rect1.left == point1.x) && (rect1.top == point2.y) 
    && (rect1.right == point2.x) && (rect1.bottom == point1.y) ) );
#else
  CHECKRES ( ( (rect1.left == point1.x) && (rect1.top == point1.y) 
    && (rect1.right == point2.x) && (rect1.bottom == point2.y) ) );
#endif

  // test ::Set()
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect1.SetInt((short)33, (short)33, (short)99, (short)99);
  rect1.PrintIt("rect1: testing ODRect::SetInt() using shorts");
  CHECKRES ( ( (rect1.left == IntToFixed(33)) && (rect1.top == IntToFixed(33)) 
    && (rect1.right == IntToFixed(99)) && (rect1.bottom == IntToFixed(99) ) ) );

  // test ::Offset()
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect1.Offset(myRight, myBottom);
  rect1.PrintIt("rect1: testing ODRect::Offset()");
  CHECKRES ( (rect1.left == myLeft + myRight) && (rect1.top == myTop+myBottom) 
    && (rect1.right == myRight+myRight) && (rect1.bottom == myBottom+myBottom) ) ;

  // test ::Offset()
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect1.Offset(point1);
  rect1.PrintIt("rect1: testing ODRect::Offset(ODPoint)");
  rect1.PrintIt("rect1: testing ODRect::Offset()");
  CHECKRES ( (rect1.left == myLeft + point1.x) && (rect1.top == myTop+point1.y) 
    && (rect1.right == myRight+point1.x) && (rect1.bottom == myBottom+point1.y)  );

  // test ::Inset()
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect1.PrintIt("rect1: after rect1.Set(myLeft, myTop, myRight, myBottom);");
#ifdef _PLATFORM_OS2_
  rect1.Inset(myLeft, myBottom);
#else
  rect1.Inset(myLeft, myTop);
#endif  // _PLATFORM_OS2_
  rect1.PrintIt("rect1: testing ODRect::Inset()");

  // test operator&=
  rect6.Set(point1, point2);
  rect6.PrintIt("rect6: After rect6.Set(point1, point2);");
  rect1 &= rect6; 
  rect1.PrintIt("rect1: testing ODRect::operator&=() rect6");

  // test operator|=
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect1.PrintIt("rect1: after rect1.Set(myLeft, myTop, myRight, myBottom);");
  rect1 |= rect6; 
  rect1.PrintIt("rect1: testing ODRect::operator|=() rect6");

  // test operator|=
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect1.PrintIt("rect1: after rect1.Set(myLeft, myTop, myRight, myBottom);");
  rect1 |= point2; 
  rect1.PrintIt("rect1: testing ODRect::operator|=() ODPoint");

  // test ::TopLeft, ::BotRight, ::Width, ::Height
  ODPoint topLeft = rect1.TopLeft();
  ODPoint botRight = rect1.BotRight();
  printf("\nTopLeft = %d,%d\n",topLeft.x, topLeft.y);
  printf("\nBotRight = %d,%d\n",botRight.x, botRight.y);

  ODCoordinate myWidth = rect1.Width();
  ODCoordinate myHeight = rect1.Height();
  printf("\nWidth = %d\n",myWidth);
  printf("\nHeight = %d\n",myHeight);
  myWidth = FixedToInt(rect1.Width());
  myHeight = FixedToInt(rect1.Height());
  printf("\nFixedToInt Width = %d\n",myWidth);
  printf("\nFixedToInt Height = %d\n",myHeight);

  // test ::AsWinRect
  rect1.Set(myLeft, myTop, myRight, myBottom);
#ifdef _PLATFORM_WIN32_
  rect1.AsWinRect(myRect);
  rect1.PrintIt("rect1: testing ODRect::AsWinRect(Rect)");
  CHECKRES ( ( (rect1.left == IntToFixed(myRect.left)) && (rect1.top == IntToFixed(myRect.top) ) 
    && (rect1.right == IntToFixed(myRect.right)) && (rect1.bottom == IntToFixed(myRect.bottom)) ) );
#elif defined(_PLATFORM_OS2_)
  rect1.AsRECTL(myRect);
  rect1.PrintIt("rect1: testing ODRect::AsRECTL(Rect)");
  CHECKRES ( ( (rect1.left == IntToFixed(myRect.xLeft)) && (rect1.top == IntToFixed(myRect.yTop) ) 
    && (rect1.right == IntToFixed(myRect.xRight)) && (rect1.bottom == IntToFixed(myRect.yBottom)) ) );
#elif defined(_PLATFORM_AIX_)
  rect1.AsXRect(myRect);
  rect1.PrintIt("rect1: testing ODRect::AsXRect(Rect)");
  CHECKRES (  (rect1.left == IntToFixed(myRect.x)) && (rect1.top == IntToFixed(myRect.y)) 
    && (rect1.right == (IntToFixed(myRect.x) + IntToFixed(myRect.width)) ) && (rect1.bottom == (IntToFixed(myRect.y) + IntToFixed(myRect.height)))  );
#endif

  // test of operator==
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect2.Set(myLeft, myTop, myRight, myBottom);
  rect1.PrintIt("rect1: testing ODRect::operator==");
  CHECKRES (rect1 == rect2) ;


  // test of operator!=
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect2.Set(myLeft, myTop, myRight, myBottom);
  rect1.Inset(myLeft, myTop);
  rect1.PrintIt("rect1: testing ODRect::operator!=");
  CHECKRES (rect1 != rect2) ;

  // test ::ApproxEquals()
  rect1.Set(myLeft, myTop, myRight, myBottom);
  rect2.Clear();
  rect1.PrintIt("rect1.ApproxEquals(rect2)");
  CHECKRES (!(rect1.ApproxEquals(rect2)) );

  rect2.Clear();
  rect2.PrintIt("rect2.IsEmpty()");
  CHECKRES (rect2.IsEmpty() );

#ifdef _PLATFORM_OS2_
  rect1.Set(IntToFixed(20), IntToFixed(100), IntToFixed(100), IntToFixed(20));
#else
  rect1.Set(myLeft, myTop, myRight, myBottom);
#endif
  rect1.PrintIt("rect1.Contains()");
  CHECKRES (rect1.Contains(point2) );

#ifdef _PLATFORM_OS2_
  rect1.Set(IntToFixed(20), IntToFixed(100), IntToFixed(100), IntToFixed(20));
#else
  rect1.Set(myLeft, myTop, myRight, myBottom);
#endif
  rect2.Set(point1,point2);
  rect1.PrintIt("rect1.Contains()");
  CHECKRES (rect1.Contains(rect2) );

  rect1.PrintIt("rect1.ApproxContains()");
  CHECKRES (rect1.Contains(rect2) );

#ifdef _PLATFORM_OS2_
  rect2.Set(IntToFixed(20), IntToFixed(100), IntToFixed(100), IntToFixed(20));
  rect1.Set(IntToFixed(20), IntToFixed(100), IntToFixed(100), IntToFixed(20));
#else
  rect2.Set(myLeft, myTop, myRight, myBottom);
  rect1.Set(myLeft, myTop, myRight, myBottom);
#endif
  rect1.Offset(point1);
  rect1.PrintIt("rect1: testing ODRect::Intersects()");
  CHECKRES (rect1.Intersects(rect2) );

#ifdef _PLATFORM_WIN32_
  if(testWin)
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)"\n\nEND ODRect::SelfTest\n\n");
  else
    printf ("\n\nEND ODRect::SelfTest\n\n");
#else
  printf ("\n\nEND ODRect::SelfTest\n\n");
#endif

}

WIN32_DLLEXPORT void ODRect::PrintIt(char *msg)
{
#ifdef _PLATFORM_WIN32_
  if(!testWindow) {
    printf (myLine);
    if(msg != NULL)
      printf ("%s\n",msg);
    printf ("left is: %d\n", left);
    printf ("top is: %d\n", top);
    printf ("right is: %d\n", right);
    printf ("bottom is: %d\n", bottom);
  }
  else {
    char tmpBuff[1024];
    sprintf (tmpBuff, myLine);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    if(msg != NULL) {
      sprintf (tmpBuff, "%s\n",msg );
      SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    }
    sprintf (tmpBuff, "left is: %d\n", left);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    sprintf (tmpBuff, "top is: %d\n", top);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    sprintf (tmpBuff,"right is: %d\n", right);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
    sprintf (tmpBuff, "bottom is: %d\n", bottom);
    SendMessage(testWindow, LB_ADDSTRING, 0, (LONG)tmpBuff);
  }
#else
  printf (myLine);
  if(msg != NULL)
    printf ("%s\n",msg);
  printf ("left is: %d\n", left);
  printf ("top is: %d\n", top);
  printf ("right is: %d\n", right);
  printf ("bottom is: %d\n", bottom);
#endif
}

#endif // (_PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_) && DEBUG
