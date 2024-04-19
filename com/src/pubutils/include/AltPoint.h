/* @(#)Z 1.20 com/src/pubutils/include/AltPoint.h, odpubutils, od96os2, odos29646d 96/11/15 15:29:11 (96/10/30 09:23:22) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odpubutils
 *
 *   CLASSES: none
 *
 *   ORIGINS: 82,27,94
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
	File:		AltPoint.h

	Contains:	C++ savvy points and rects (alternate ODPoint, ODRect)

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.
	
	Notes:
	
	These are alternate definitions of the ODPoint and ODRect structs.
	These definitions have the same size and data format and can be used
	interchangeably with the basic definitions; but they're much more C++
	savvy, with constructors, operators, conversions, and utility methods.
	
	To use these classes instead of the defaults, just include "AltPoint.h"
	as the first thing in your source file. It has to be included first so
	it can override the default struct definitions in PlfmType.h.
	
	This API and implementation are **NOT** an official part of the OpenDoc
	API, just handy utilities for C++ programmers.
	
*/


#ifndef _ALTPOINT_
#define _ALTPOINT_

// Make sure that built-in structs do not get defined by PlfmType.h.
#if !defined(SOM_Module_OpenDoc_GeoTypes_defined)
	#define SOM_Module_OpenDoc_GeoTypes_defined
#else
	#error "Must include AltPoint.h *before* ODTypes.h!"
#endif


#ifndef _ODTYPESF_
#include "ODTypesF.h"		// Must include before ODTypesM.xh
#endif

#ifndef SOM_Module_OpenDoc_Global_TypesB_defined
#include "ODTypesB.xh"		
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __TYPES__
#include <Types.h>
#endif
#endif

#ifdef _PLATFORM_OS2_
#ifndef __OS2DEF__
#include <os2def.h>
#endif
#endif // _PLATFORM_OS2_

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


//==============================================================================
// ODCoordinate
//==============================================================================

typedef ODFixed ODCoordinate;

//==============================================================================
// ODPoint
//==============================================================================

struct ODPoint {
	public:
	
	// CONTENTS:
	
	ODCoordinate x, y;
	
	// CONSTRUCTORS:
	
	ODPoint( ) { }
	
	ODPoint( ODCoordinate xx, ODCoordinate yy )
					{x=xx; y=yy;}
	
	_DLLIMPORTEXPORT_ ODPoint( const ODPoint& );				// Copy constructor
	
	// ASSIGNMENT:
	
	_DLLIMPORTEXPORT_ ODPoint& operator= ( const ODPoint& );	// Copy from another pt
	
	// MODIFICATION:
	
	inline void	Clear( )
					{x=y=0;}
	inline void	Set( ODCoordinate xx, ODCoordinate yy )
					{x=xx; y=yy;}
	_DLLIMPORTEXPORT_ void	Offset( ODCoordinate x, ODCoordinate y );
	_DLLIMPORTEXPORT_ void	operator+=( const ODPoint& );
	_DLLIMPORTEXPORT_ void	operator-=( const ODPoint& );
	
	// ACCESSORS:

	_DLLIMPORTEXPORT_ ODSShort	IntX( )		const;		// Returns X-coord as (16bit) integer
	_DLLIMPORTEXPORT_ ODSShort	IntY( )		const;		// Returns Y-coord as (16bit) integer
	
	// COMPARISON:
	
	_DLLIMPORTEXPORT_ ODBoolean	operator==( const ODPoint& )	const;
	_DLLIMPORTEXPORT_ ODBoolean	operator!=( const ODPoint& )	const;
	_DLLIMPORTEXPORT_ ODBoolean	ApproxEquals( const ODPoint& )	const;		// to within roundoff error
	
	// MAC TOOLBOX CONVENIENCES:
	
#ifdef _PLATFORM_MACINTOSH_
	ODPoint( Point );							// Construct from QD point
	ODPoint& operator= ( const Point& );		// Copy from a QD Point
	Point	AsQDPoint( )					const;	// Convert to integer (QD) point
	void	operator+=( const    Point& );			// Add/subtract QD point
	void	operator-=( const    Point& );
#endif

#ifdef _PLATFORM_WIN32_
        _DLLCTIMPORTEXPORT_ ODPoint( Point );
	_DLLIMPORTEXPORT_ ODPoint& operator= ( const Point& );
	_DLLIMPORTEXPORT_ Point	AsWinPoint( ) const;
	_DLLIMPORTEXPORT_ void	operator+=( const    Point& );
	_DLLIMPORTEXPORT_ void	operator-=( const    Point& );
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_OS2_
       ODPoint( POINTL ptl);                       // Construct from GPI POINTL
       ODPoint& operator= ( const POINTL& ptl);    // Copy from a GPI POINTL
       POINTL  AsPOINTL( )          const;         // Convert to integer POINTL
       void  operator+=( const    POINTL& ptl);    // Add/subtract POINTL
       void  operator-=( const    POINTL& ptl);
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_AIX_
        ODPoint( Point );
	ODPoint& operator= ( const Point& );
	Point	AsXPoint( ) const;
	void	operator+=( const    Point& );
	void	operator-=( const    Point& );
#endif // _PLATFORM_AIX_

#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) && defined(DEBUG)
        _DLLIMPORTEXPORT_ void    SelfTest(HWND testWin = NULL);  // a quick unit test
        // PrintIt will print the x,y points and a text msg if you
        // want to send one.
        _DLLIMPORTEXPORT_ void    PrintIt(char *msg = NULL); 
#endif //((_PLATFORM_WIN32_) || (_PLATFORM_OS2_) || (_PLATFORM_AIX_)) && (DEBUG)
};


//==============================================================================
// ODRect
//==============================================================================

struct ODRect {
	public:
	
	// CONTENTS:
	
	ODCoordinate left, top, right, bottom;
		
	// CONSTRUCTORS:
	
	ODRect( )		{ }
	ODRect( ODCoordinate l, ODCoordinate t,
				  ODCoordinate r, ODCoordinate b )
			{left=l; top=t; right=r; bottom=b; }
	_DLLIMPORTEXPORT_ ODRect( const ODPoint&, const ODPoint& );	// Any 2 opposite pts
#ifdef _PLATFORM_OS2_
	ODRect( const ODPoint &bottomLeft, ODCoordinate width, ODCoordinate height );
#else
	_DLLIMPORTEXPORT_ ODRect( const ODPoint &topLeft, ODCoordinate width, ODCoordinate height );
#endif
#if ((defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_WIN32_)) \
      || defined(_PLATFORM_AIX_) )
	_DLLIMPORTEXPORT_ ODRect( const Rect& );
#endif
#ifdef _PLATFORM_OS2_
	_DLLIMPORTEXPORT_ ODRect( const RECTL& );
#endif
	
	// ASSIGNMENT:
	
#if ( (defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_WIN32_)) \
      || defined(_PLATFORM_AIX_) )
	_DLLIMPORTEXPORT_ ODRect& operator= ( const Rect& );
#endif

#ifdef _PLATFORM_OS2_
	ODRect& operator= ( const RECTL& );
#endif

	
	// MODIFICATION:
	
	_DLLIMPORTEXPORT_ void	Clear( );
	_DLLIMPORTEXPORT_ void	Set( ODCoordinate l, ODCoordinate t, ODCoordinate r, ODCoordinate b );
	_DLLIMPORTEXPORT_ void	Set( const ODPoint&, ODCoordinate width, ODCoordinate height );
	_DLLIMPORTEXPORT_ void	Set( const ODPoint&, const ODPoint& );	// Any 2 opposite pts
	_DLLIMPORTEXPORT_ void	SetInt( short l, short t, short r, short b );
	_DLLIMPORTEXPORT_ void	Offset( ODCoordinate x, ODCoordinate y );
	_DLLIMPORTEXPORT_ void	Offset( const ODPoint& );
	_DLLIMPORTEXPORT_ void	Inset( ODCoordinate x, ODCoordinate y );
	
	_DLLIMPORTEXPORT_ void	operator&= ( const ODRect& );	// Intersect with rectangle
	_DLLIMPORTEXPORT_ void	operator|= ( const ODRect& );	// Union with rectangle
	_DLLIMPORTEXPORT_ void	operator|= ( const ODPoint& );		// Expand to fit point
		
	// ACCESSORS
	
	const ODPoint& TopLeft( )										const
								{return *(ODPoint*)&left;}
	ODPoint&		TopLeft( )
								{return *(ODPoint*)&left;}
	const ODPoint& BotRight( )										const
								{return *(ODPoint*)&right;}
	ODPoint&		BotRight( )
								{return *(ODPoint*)&right;}
	ODCoordinate	Width( )										const
								{return right-left;}
	ODCoordinate	Height( )										const
#ifdef _PLATFORM_OS2_
								{return top-bottom;}
#else
								{return bottom-top;}
#endif // _PLATFORM_OS2_
#ifdef _PLATFORM_MACINTOSH_
	void			AsQDRect( Rect& )								const;
#endif
#ifdef _PLATFORM_WIN32_
	_DLLIMPORTEXPORT_ void			AsWinRect( Rect& )								const;
#endif
#ifdef _PLATFORM_OS2_
	void			AsRECTL( RECTL& )								const;
#endif
#ifdef _PLATFORM_AIX_
	void			AsXRect( Rect& )								const;
#endif
	
	// TESTING

	_DLLIMPORTEXPORT_ ODBoolean	operator==( const ODRect& )							const;
	ODBoolean	operator!=( const ODRect &r )						const
								{return !(*this==r);}
	_DLLIMPORTEXPORT_ ODBoolean	ApproxEquals( const ODRect &r )						const;
	
	_DLLIMPORTEXPORT_ ODBoolean	IsEmpty( )											const;
	_DLLIMPORTEXPORT_ ODBoolean	Contains( const ODPoint& )							const;
	_DLLIMPORTEXPORT_ ODBoolean	Contains( const ODRect& )							const;
	_DLLIMPORTEXPORT_ ODBoolean	ApproxContains( const ODRect& )						const;
	_DLLIMPORTEXPORT_ ODBoolean	Intersects( const ODRect& )							const;

#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) && defined(DEBUG)
        _DLLIMPORTEXPORT_ void    SelfTest(HWND testWin = NULL);  // a quick unit test
        // PrintIt will print the rect points and a text msg if you
        // want to send one.
        _DLLIMPORTEXPORT_ void    PrintIt(char *msg = NULL); 
#endif //((_PLATFORM_WIN32_) || (_PLATFORM_OS2_) || (_PLATFORM_AIX_)) && (DEBUG)
};

#if ( (defined (_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)) \
      || defined (_PLATFORM_AIX_) ) 
struct ODToolSpaceRect {
public:
       ODCoordinate    left;
       ODCoordinate    top;
       ODCoordinate    right;
       ODCoordinate    bottom;
       ODRect          floatRect;

       ODToolSpaceRect( )              { }

       _DLLIMPORTEXPORT_ ODToolSpaceRect(ODCoordinate l, ODCoordinate t,
                       ODCoordinate r, ODCoordinate b, ODRect* aFloatRect);

       _DLLIMPORTEXPORT_ ODBoolean IsEmpty() const;

       _DLLIMPORTEXPORT_ void Set(ODCoordinate l, ODCoordinate t,
                ODCoordinate r, ODCoordinate b, ODRect* aFloatRect);

       _DLLIMPORTEXPORT_ void Clear(void);
};
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_


#endif //_ALTPOINT_
