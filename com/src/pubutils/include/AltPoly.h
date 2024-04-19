/* @(#)Z 1.20 com/src/pubutils/include/AltPoly.h, odpubutils, od96os2, odos29646d 96/11/15 15:29:36 (96/10/29 09:28:26) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odpubutils
 *
 *   CLASSES:   ODPolygon
 *		ODTempPolygon
 *		ODTempPolygonPtr
 *		PolyEdgeIterator
 *		TempGXShape
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
	File:		AltPoly.h

	Contains:	OpenDoc polygon: optional C++ savvy classes

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	
	
	Theory of Operation:
	
		This is an alternate definition of ODPolygon and ODContour. The data format is
		identical, but the structs defined here have a lot of useful methods including
		constructors, accessors and conversion operators.
		
		To use these instead of the regular structs defined in Polygon.h, just include
		this header file _before_ Polygon.h. An easy way to do this is to include it
		first.
		
		QuickDraw GX users take note:
		ODContour is identical in data format to a gxPolygon.
		ODPolygonData (the data stored inside an ODPolygon) is identical in data
			format to a gxPolygons <sic>.
		See <GXTypes.h>.
	
		** The way things are done has changed since A6. SOM wants all variable-sized
		structures to adhere to a common data forma, where a small struct points to
		the data and stores its size. This is so DSOM can tell how to copy the data
		across an address-space boundary. The ODPolygon structure has most of the same
		methods as before, but you can now create them directly. However, to get the
		actual polygon data you'll need to call the GetData method. **
	
	In Progress:
		
*/


#ifndef _ALTPOLY_
#define _ALTPOLY_

#ifdef SOM_Module_OpenDoc_Polygon_defined
	#error "Must include AltPoly.h *before* Polygon.xh!"
#else
	/* Make sure Polygon.xh does NOT get included later! */
	#define SOM_Module_OpenDoc_Polygon_defined 2
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODEXCEPT_
#include <ODExcept.h>					/* For Destructo, used by ODTempPolygon */
#endif

#include <stddef.h>					/* for size_t */

#if _PLATFORM_MACINTOSH_
	#ifndef __QUICKDRAW__
	#include <QuickDraw.h>				/* for Region and Polygon types */
	#endif
	#ifndef __FIXMATH__
	#include <FixMath.h>				/* Must include before GX headers... */
	#endif
	#ifndef __GXTYPES__
	#include <GXTypes.h>				/* for gxShape type */
	#endif
#endif

#if _PLATFORM_OS2_
#ifndef _ODOS2_
#include "ODos2.h"
#endif
typedef struct _POLYGON POLYGON;    // defined in pmgpi.h
#endif // _PLATFORM_OS2_

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


//==============================================================================
// Classes used in this interface
//==============================================================================

struct ODRect;
class ODStorageUnit;
class ODTransform;

//==============================================================================
// ODContour
//==============================================================================

struct ODContour
{
	public:
	
	ODSLong	nVertices;
	ODPoint	vertex[1];		// Array size is actually nVertices
#ifdef OD_BUG // 21439
	_DLLIMPORTEXPORT_ ODContour* NextContourEx(const ODSLong& );
#endif
	ODContour* NextContour( )				const	{return (ODContour*)&vertex[nVertices];}
	_DLLIMPORTEXPORT_ ODBoolean	IsRectangular( )			const;
	_DLLIMPORTEXPORT_ ODBoolean	AsRectangle( ODRect* )		const;
	_DLLIMPORTEXPORT_ ODBoolean	HasExactRegion( )			const;
#if _PLATFORM_MACINTOSH_
	PolyHandle	AsQDPolygon( )				const;
#endif
#if _PLATFORM_WIN32_
	_DLLIMPORTEXPORT_ Point *AsWinPolygon ( )  const;
#endif
#if _PLATFORM_OS2_
        void AsPOLYGON(POLYGON&) const;
#endif    
#ifdef  _PLATFORM_AIX_
	Point *AsXPolygon ( )  const;
#endif
	
	_DLLIMPORTEXPORT_ ODBoolean	operator== ( const ODContour& )			const;
	ODBoolean	operator!= ( const ODContour &c )		const	{return !(*this==c);}
};

//==============================================================================
// ODPolygonData
//==============================================================================

struct ODPolygonData {
	ODSLong	nContours;						// Number of contours
	ODContour	firstContour;				// Rest of contours follow after first
};

//==============================================================================
// ODPolygon
//==============================================================================

class ODPolygon
{
	public:
	
					_DLLIMPORTEXPORT_ ODPolygon( );
#ifdef _PLATFORM_OS2_
// note: the orig os/2 port had a note about the ODPolygon dtor.  They
// actually have it commented out. (Rob)
#endif
#if ODDebug
				   ~ODPolygon( );		// Delete myself, but not data
#endif
	
	_DLLIMPORTEXPORT_ void			Delete( );			// Delete myself & my data
	_DLLIMPORTEXPORT_ void			Clear( );			// Just deletes my data
	
	// ACCESSORS:
	
	ODBoolean		HasData( )						const	{return _length!=0;}
	ODULong			GetDataSize( )					const	{return _length;}
	ODPolygonData*	GetData( )						const	{return _buf;}
	
	_DLLIMPORTEXPORT_ void			SetData( const ODPolygonData* );	// Does not copy the data!
	
	_DLLIMPORTEXPORT_ ODSLong			GetNContours( )					const;
	_DLLIMPORTEXPORT_ ODContour*		FirstContour( );
	_DLLIMPORTEXPORT_ const ODContour*FirstContour( )					const;
	
	// GEOMETRY:
	
	_DLLIMPORTEXPORT_ void		ComputeBoundingBox( ODRect* )		const;
	_DLLIMPORTEXPORT_ ODBoolean	IsRectangular( )					const;
	_DLLIMPORTEXPORT_ void		Transform( Environment*, ODTransform* );
	
	_DLLIMPORTEXPORT_ ODBoolean	operator== ( ODPolygon& )			const;
	ODBoolean	operator!= ( ODPolygon& p )			const	{return !(*this==p);}
	
	_DLLIMPORTEXPORT_ ODSLong	Contains( ODPoint )						const;
	_DLLIMPORTEXPORT_ ODBoolean	IsEmpty( )							const;
	
	// CONVERSIONS:
	
	_DLLIMPORTEXPORT_ ODBoolean	AsRectangle( ODRect* )				const;	// False if nonrectangular
	_DLLIMPORTEXPORT_ ODBoolean	HasExactRegion( )					const;
#if _PLATFORM_MACINTOSH_
	RgnHandle	AsQDRegion( )						const;
	gxShape		AsGXShape( )						const;
#endif
#if _PLATFORM_WIN32_
	_DLLIMPORTEXPORT_ ODRgnHandle	AsWinRegion( )						const;
#endif
#if _PLATFORM_OS2_
        HRGN AsRegion(HPS hps) const;
#endif
#ifdef _PLATFORM_AIX_
        Region AsXRegion( ) const;
#endif
#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) && defined(DEBUG)
        _DLLIMPORTEXPORT_ void            SelfTest();  // test myself! 
        _DLLIMPORTEXPORT_ void    PrintIt(char *msg = NULL); 
#endif // (defined (_PLATFORM_WIN32_) || _OS2_ || _AIX_) && defined(DEBUG)
	
	// ALLOCATION:
	
	_DLLIMPORTEXPORT_ ODPolygon*	SetNVertices( ODSLong nVertices );
	_DLLIMPORTEXPORT_ ODPolygon*	SetVertices( ODSLong nVertices, const ODPoint *vertices );
	_DLLIMPORTEXPORT_ ODPolygon*	SetContours( ODSLong nContours, const ODSLong *contourVertices );
	_DLLIMPORTEXPORT_ ODPolygon*	SetRect( const ODRect& );

	_DLLIMPORTEXPORT_ ODPolygon*	Copy( )								const;
	_DLLIMPORTEXPORT_ ODPolygon*	CopyFrom( const ODPolygon& );
#if _PLATFORM_MACINTOSH_
	ODPolygon*	CopyFrom( gxShape );		// Accepts rect, polygon(s), path(s)
#endif
	_DLLIMPORTEXPORT_ ODPolygon*	MoveFrom( ODPolygon& );		// Justs adjusts pointers, no copying
	
	// INPUT/OUTPUT:
	
	_DLLIMPORTEXPORT_ ODPolygon*	ReadFrom( Environment*, ODStorageUnit* );
	_DLLIMPORTEXPORT_ ODPolygon*	WriteTo( Environment*, ODStorageUnit* )		const;
	
	private:
	
	void		Realloc( ODULong dataSize );
	
	// DATA MEMBERS:
	
    unsigned long _maximum;						// Exact same data as an ODByteArray
    unsigned long _length;
    ODPolygonData *_buf;
};


//==============================================================================
// ODTempPolygon
//==============================================================================

/*	ODTempPolygon is a polygon whose destructor disposes of its data.
	This is useful if you have a local variable that's a temporary polygon
	and you want to make sure the data gets disposed.
	This _is_ exception-safe: inheriting from Destructo guarantees that the
	data will be cleaned up even if an exception is thrown.
*/

#if defined (_PLATFORM_WIN32_) ||  defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
// There is a slight problem here inheriting from Destructo since
// we are compiling with _NATIVE_EXCEPTIONS_ defined.  The     
// Destructo class is defined differently in this case (struct). (rob)
class ODTempPolygon :public ODPolygon
#else
class ODTempPolygon :public ODPolygon, Destructo
#endif // defined (_PLATFORM_WIN32_) ||  defined(_PLATFORM_OS2_)
{
public:
	_DLLIMPORTEXPORT_ ODTempPolygon( );
   _DLLIMPORTEXPORT_ ~ODTempPolygon( );
};


/*	ODTempPolygonPtr is a _pointer_ to a polygon, whose destructor deletes
	the polygon structure (and its data.) Yes, it is a class, but due to the
	magic of operator overloading it can be used just as a pointer. See the
	implementation of ODPolygon::Copy in AltPoly.cpp for an example. */

#if defined (_PLATFORM_WIN32_) ||  defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
// There is a slight problem here inheriting from Destructo since
// we are compiling with _NATIVE_EXCEPTIONS_ defined.  The     
// Destructo class is defined differently in this case (struct). (rob)
class ODTempPolygonPtr
#else
class ODTempPolygonPtr :Destructo
#endif //  _PLATFORM_WIN32_
{
public:
	_DLLIMPORTEXPORT_ ODTempPolygonPtr( );
	_DLLIMPORTEXPORT_ ODTempPolygonPtr( ODPolygon* );
	_DLLIMPORTEXPORT_ ~ODTempPolygonPtr( );
	operator ODPolygon* ( )					{return fPoly;}
	ODPolygon* operator-> ( )				{return fPoly;}
	ODPolygon* operator= ( ODPolygon *p )	{return (fPoly=p);}
	ODPolygon* DontDelete( )				{ODPolygon* temp=fPoly; fPoly=kODNULL; return temp;}
	
private:
	ODPolygon *fPoly;
};


/*	TempGXShape is a temporary reference to a gxShape. It will be released when
	the reference goes out of scope. For an example, see the implementation of
	ODPolygon::CopyFrom( gxShape ). */

#if !defined (_PLATFORM_WIN32_) &&  !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_AIX_)
class TempGXShape :Destructo
{
public:
	TempGXShape( );
	TempGXShape( gxShape );
	~TempGXShape( );
	operator gxShape ( )				{return fShape;}
	gxShape operator= ( gxShape s )		{return (fShape=s);}
	gxShape DontRelease( )				{gxShape temp=fShape; fShape=kODNULL; return temp;}
	
private:
	gxShape fShape;
};
#endif // _PLATFORM_WIN32_


//==============================================================================
// Polygon Edge Iterator
//==============================================================================

class PolyEdgeIterator {
	public:

	_DLLIMPORTEXPORT_ PolyEdgeIterator( const ODPolygon* );
	
	_DLLIMPORTEXPORT_ void		CurrentEdge( const ODPoint* &v1, const ODPoint* &v2 );
	const ODContour* CurrentContour( )			{return fCurContour;}
	long		CurrentContourIndex( )			{return fCurContourIndex;}
	long		CurrentEdgeIndex( )				{return fCurVertex;}
	
	_DLLIMPORTEXPORT_ ODBoolean	Next( );
	_DLLIMPORTEXPORT_ ODBoolean	IsNotComplete( );
	
	private:
	const ODPolygon*	const fPoly;
	const ODContour*		  fCurContour;
	long					  fCurContourIndex;
	long					  fCurVertex;
};


#endif //_ALTPOLY_
