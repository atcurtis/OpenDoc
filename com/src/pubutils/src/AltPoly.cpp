/* @(#)Z 1.34 com/src/pubutils/src/AltPoly.cpp, odpubutils, od96os2, odos29646d 96/11/15 15:29:05 (96/10/29 09:28:46) */
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
	File:		AltPoly.cpp

	Contains:	OpenDoc polygon: optional C++ savvy classes

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	To Do:
				  
		Improve the equality tests for contours and polygons. See comments
		in the two "operator==" methods for details.
	
	In Progress:
		
*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ALTPOINT_
#include "AltPoint.h"			// Use C++ savvy ODPoint and ODRect
#endif

#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifndef SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef _LINEOPS_
#include "LineOps.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef SOM_ODStorageUnit_xh
#include "StorageU.xh"
#endif

#ifndef _STDTYPES_
#include "StdTypes.xh"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
	#ifndef __GXERRORS__
	#include "GXErrors.h"
	#endif
	#ifndef __GXGRAPHICS__
	#include "GXGraphics.h"
	#endif
#endif

#include <stddef.h>					// Defines offsetof() macro

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifdef _PLATFORM_AIX_
#ifndef _ODMATH_
#include "ODMath.h"
#endif
Region DrawToXRegion(ODContour* temp_contour);
ODBoolean IsPositive(ODContour* target_contour);
#endif  // _PLATFORM_AIX_

#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) && defined(DEBUG)
#include <stdio.h>

#define CHECKRES(x) \
  if( ( x ) ) \
      printf("PASS\n"); \
  else \
      printf("FAIL\n");

  struct MyContour   // Used in ODPolygon::SelfTest
  {             // Microsoft Visual C++ won't allow this in ODPolygon::SelfTest
    ODSLong nVertices;
    ODPoint vertex [4];
  };

  struct MyPolygonData  // Used in ODPolygon::SelfTest 
  {             // Microsoft Visual C++ won't allow this in ODPolygon::SelfTest
    ODSLong nContours;
    MyContour firstContour;
  };
#endif // defined (_PLATFORM_WIN32_ || OS2 || AIX ) && defined(DEBUG)

const ODSLong kMaxLong	= 0x7FFFFFFF;

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODShape
#endif  // _PLATFORM_MACINTOSH_

//==============================================================================
// Destructos
//==============================================================================


WIN32_DLLEXPORT ODTempPolygon::ODTempPolygon( )
{
	// This constructor doesn't do anything special, but if not declared it will
	// be inlined at the call site, resulting in lots of extra code due to the
	// multiple inheritance.
}


WIN32_DLLEXPORT ODTempPolygon::~ODTempPolygon( )
{
	this->Clear();
}


WIN32_DLLEXPORT ODTempPolygonPtr::ODTempPolygonPtr( )
	:fPoly(kODNULL)
{
}


WIN32_DLLEXPORT ODTempPolygonPtr::ODTempPolygonPtr( ODPolygon *p )
	:fPoly(p)
{
}


WIN32_DLLEXPORT ODTempPolygonPtr::~ODTempPolygonPtr( )
{
	delete fPoly;
	fPoly = kODNULL;
}


#ifdef _PLATFORM_MACINTOSH_
TempGXShape::TempGXShape( )
	:fShape(kODNULL)
{
}


TempGXShape::TempGXShape( gxShape s )
	:fShape(s)
{
}


TempGXShape::~TempGXShape( )
{
	if( fShape ) {
		GXDisposeShape(fShape);
		fShape = kODNULL;
	}
}

#endif // _PLATFORM_MACINTOSH_

//==============================================================================
// QuickDraw GX Utilities
//==============================================================================


#ifdef _PLATFORM_MACINTOSH_
#pragma segment QDGXShape


static void
ClearGXError( )
{
	GXGetGraphicsError(kODNULL);
	// GX error status is cleared after asking for errors.
}


static void
ThrowIfGXError( )
{
	gxGraphicsError err = GXGetGraphicsError(kODNULL);	// Get latest graphics error
	if( err )
		THROW(err,"QuickDraw GX error");
}


static void
ThrowIfFirstGXError( )
{
	gxGraphicsError err;
	(void) GXGetGraphicsError(&err);			// Get first error, not last
	if( err )
		THROW(err,"QuickDraw GX error");
}

#endif // _PLATFORM_MACINTOSH_

/******************************************************************************/
//**	ALLOCATION
/******************************************************************************/


WIN32_DLLEXPORT ODPolygon::ODPolygon( )
	:_maximum(0),
	 _length(0),
	 _buf(kODNULL)
{
}


#if ODDebug
ODPolygon::~ODPolygon( )
{
	// To help catch double-deletes of ODPolygon structures!
	_buf = (ODPolygonData*)0xDDDDDDDD;
	_length = _maximum = 0xDDDDDDDD;
}
#endif


WIN32_DLLEXPORT void
ODPolygon::Delete( )
{
	ODDisposePtr(_buf);
	delete this;
}


WIN32_DLLEXPORT void
ODPolygon::Clear( )
{
	ODDisposePtr(_buf);
	_buf = kODNULL;
	_length = _maximum = 0;
}


static ODULong
CalcDataSize( ODSLong nVertices )
{
	if( nVertices==0 )
		return 0;
	else
#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
		return sizeof(ODPolygonData)+(nVertices-1)*sizeof(ODPoint);
#else
		return offsetof(ODPolygonData,firstContour.vertex[nVertices]);
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
}


void
ODPolygon::Realloc( ODULong dataSize )
{
	if( _buf!=kODNULL && dataSize>=_length && dataSize<=_maximum )
		_length = dataSize;
	else {
		ODPtr newData;
		if( dataSize!=0 )
			newData = ODNewPtr(dataSize);
		ODDisposePtr(_buf);
		if( dataSize!=0 )
			_buf = (ODPolygonData*)newData;
		else
			_buf = kODNULL;
		_length = _maximum = dataSize;
	}
}


WIN32_DLLEXPORT void
ODPolygon::SetData( const ODPolygonData *data )
{
	_length = sizeof(ODULong) * (1+data->nContours);
	const ODContour *c = &data->firstContour;
	for( ODULong i=data->nContours; i!=0; i-- ) {
		_length += c->nVertices * sizeof(ODPoint);
		c = c->NextContour();
	}
	
	ODDisposePtr(_buf);
	_buf = (ODPolygonData*)data;
	_maximum = _length;
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::SetNVertices( ODSLong nVertices )
{
	ASSERT(nVertices>=0,kODErrValueOutOfRange);
	
	this->Realloc(CalcDataSize(nVertices));
	if( nVertices>0 ) {
		_buf->nContours = 1;
		_buf->firstContour.nVertices = nVertices;
	}
	return this;
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::SetVertices( ODSLong nVertices, const ODPoint *vertices )
{
	ASSERT(nVertices>=0,kODErrValueOutOfRange);
	ASSERT(vertices!=kODNULL,kODErrIllegalNullInput);
	
	this->SetNVertices(nVertices);
	if( nVertices>0 )
		ODBlockMove( (void *) vertices, _buf->firstContour.vertex, nVertices*sizeof(ODPoint) );
	return this;
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::SetContours( ODSLong nContours, const ODSLong *contourVertices )
{
	ASSERT(nContours>=0,kODErrValueOutOfRange);
	if( nContours==0 )
		return this->SetNVertices(0);
	else {
		ASSERT(contourVertices!=kODNULL,kODErrIllegalNullInput);
		ODULong totalVertices = 0;
		ODSLong i;
		for( i=nContours-1; i>=0; i-- )
			totalVertices += contourVertices[i];
		this->Realloc( offsetof(ODPolygonData,firstContour)
					 + offsetof(ODContour,vertex[0]) * nContours
					 + sizeof(ODPoint)*totalVertices );
		_buf->nContours = nContours;
		ODContour *cont = this->FirstContour();
		for( i=0; i<nContours; i++ ) {
			cont->nVertices = contourVertices[i];
			cont = cont->NextContour();
		}
		return this;
	}
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::SetRect( const ODRect &r )
{
	if( r.IsEmpty() )
		return this->SetNVertices(0);
	else {
		this->SetNVertices(4);
#ifdef _PLATFORM_OS2_ 
		_buf->firstContour.vertex[0].Set(r.left,r.bottom); 
		_buf->firstContour.vertex[1] = r.TopLeft(); 
		_buf->firstContour.vertex[2].Set(r.right,r.top); 
		_buf->firstContour.vertex[3] = r.BotRight(); 
#else 
		_buf->firstContour.vertex[0] = r.TopLeft();
		_buf->firstContour.vertex[1].Set(r.right,r.top);
		_buf->firstContour.vertex[2] = r.BotRight();
		_buf->firstContour.vertex[3].Set(r.left,r.bottom);
#endif
	}
	return this;
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::CopyFrom( const ODPolygon &poly )
{
	if( poly._buf != _buf ) {
		ODULong size = poly.GetDataSize();
		this->Realloc(size);
		ODBlockMove(poly.GetData(),_buf,size);
	}
	return this;
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::MoveFrom( ODPolygon &poly )
{
	if( poly._buf != _buf ) {
		ODDisposePtr(_buf);
		_buf = poly._buf;
		_length = poly._length;
		_maximum = poly._maximum;
	}
	if( &poly._buf != &_buf ) {			// Don't clear poly if it's myself!
		poly._buf = kODNULL;
		poly._length = poly._maximum = 0;
	}
	return this;
}



#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODGXShape

ODPolygon*
ODPolygon::CopyFrom( gxShape shape )
{
	ClearGXError();
	TempGXShape copiedShape = GXCopyToShape(kODNULL,shape);
	GXPrimitiveShape(copiedShape);
	GXSimplifyShape(copiedShape);
	GXSetShapeType(copiedShape,gxPolygonType);
	ThrowIfFirstGXError();
	
	ODULong size = GXGetPolygonParts(copiedShape, 1,gxSelectToEnd, kODNULL);
	ThrowIfGXError();
	this->Realloc(size);
	GXGetPolygonParts(copiedShape, 1,gxSelectToEnd, (gxPolygons*)_buf);
	ThrowIfGXError();
	
	return this;
}
#pragma segment ODShape
#endif


WIN32_DLLEXPORT ODPolygon*
ODPolygon::ReadFrom( Environment *ev, ODStorageUnit *su )
{

	if( !su->Exists(ev,kODNULL,kODPolygon,kODPosUndefined) ) {
		this->Clear();
	} else {
		ODPropertyName propName = su->GetProperty(ev);
		ODGetPolygonProp(ev, su, propName, kODPolygon, this);
		ODDisposePtr((ODPtr) propName);
	}

	return this;
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::WriteTo( Environment *ev, ODStorageUnit *su )  const
{
	ODPropertyName propName = su->GetProperty(ev);
	ODSetPolygonProp(ev, su, propName, kODPolygon, this);
	ODDisposePtr((ODPtr) propName);
	
	return (ODPolygon*)this;
}


/******************************************************************************/
//**	POLYGON STUFF
/******************************************************************************/


WIN32_DLLEXPORT ODSLong
ODPolygon::GetNContours( )  const
{
	return _length>0 ?_buf->nContours :0;
}


WIN32_DLLEXPORT const ODContour*
ODPolygon::FirstContour( )  const
{
	return _length>0 ?&_buf->firstContour :kODNULL;
}


WIN32_DLLEXPORT ODContour*
ODPolygon::FirstContour( )
{
	return _length>0 ?&_buf->firstContour :kODNULL;
}


WIN32_DLLEXPORT ODPolygon*
ODPolygon::Copy( ) const
{
	ODTempPolygonPtr poly = new ODPolygon;
	poly->CopyFrom(*this);
	return poly.DontDelete();
}


WIN32_DLLEXPORT void
ODPolygon::ComputeBoundingBox( ODRect *bbox ) const
{
	ASSERT(bbox!=kODNULL,kODErrIllegalNullInput);
	
	if( _buf==kODNULL || _buf->nContours <= 0 ) {
		bbox->Clear();
		return;
	}
	
	// Start bbox out as maximally empty:
#ifdef _PLATFORM_OS2_ 
        bbox->left = bbox->bottom = kMaxLong; 
        bbox->right = bbox->top   = -kMaxLong; 
#else 
	bbox->left	= bbox->top		=  kMaxLong;
	bbox->right = bbox->bottom	= -kMaxLong;
#endif
	
	const ODContour *c = this->FirstContour();
	for( ODSLong i=this->GetNContours(); i>0; i-- ) {
		ODPoint *pt = (ODPoint*)c->vertex;
		for( ODSLong v=c->nVertices; v>0; v--,pt++ ) {
			if( pt->x < bbox->left )	bbox->left	= pt->x;
			if( pt->x > bbox->right )	bbox->right	= pt->x;
#ifdef _PLATFORM_OS2_ 
                        if( pt->y > bbox->top )  bbox->top = pt->y; 
                        if( pt->y < bbox->bottom ) bbox->bottom = pt->y; 
#else 
			if( pt->y < bbox->top )		bbox->top	= pt->y;
			if( pt->y > bbox->bottom )	bbox->bottom= pt->y;
#endif
		}
		if( i>1 )
			c = c->NextContour();
	}
}


WIN32_DLLEXPORT ODBoolean
ODContour::operator== ( const ODContour &cont ) const
{
	/*	This test is complicated by the fact that the two contours might have the
		same points, but out of phase. Therefore we have to compare the points
		in sequence, once per possible phase difference.  */
	
	ODSLong nv = this->nVertices;
	if( nv != cont.nVertices )
		return kODFalse;
	
	for( ODSLong phase=0; phase<nv; phase++ ) {
		const ODPoint *p0 = &this->vertex[0];
		const ODPoint *p1 = &cont.vertex[phase];
		ODSLong i;
		for( i=nVertices; i>0; i-- ) {
			if( i==phase )
				p1 = &cont.vertex[0];
			if( ! (p0++)->ApproxEquals(*p1++) )		// Coords may differ very slightly
				break;
		}
		if( i==0 )
			return kODTrue;
	}
	return kODFalse;
}


#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif //  _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
ODPolygon::operator== ( ODPolygon &poly ) const
{
	/*	This test is complicated by the fact that the two polygons may not have their
		contours in the same order. Our approach is to step through my contours in
		order, trying to match each to a unique contour in the target. To ensure
		uniqueness, the sign of the nVertices field in a target contour is flipped
		after it's matched.  */
	
	if( this->GetNContours() != poly.GetNContours() )
		return kODFalse;
	if( &poly == this )
		return kODTrue;
	
	ODBoolean result = kODTrue;
	const ODContour * c = this->FirstContour();
	ODContour *pc;
	
	ODSLong i;
	for( i=this->GetNContours(); i>0; i-- ) {
		pc = poly.FirstContour();
		ODSLong j;
		for( j=poly.GetNContours(); j>0; j-- ) {
			if( pc->nVertices>0 && *c==*pc ) {		// Compare contours!
				pc->nVertices = -pc->nVertices;		// Use sign bit as a flag (yech)
				break;
			}
			if( j>1 )
#ifdef OD_BUG // 21439
				pc = pc->NextContourEx( (abs(pc->nVertices) ) );
#else
				pc = pc->NextContour();
#endif // OD_BUG
		}
		if( j<=0 ) {
			result = kODFalse;					// No match for contour
			break;
		}

		if( i>1 )
			c = c->NextContour();
	}

	// Now that we know, clear all the sign bits:
	pc = poly.FirstContour();
	for( i=poly.GetNContours(); i>0; i-- ) {
		if( pc->nVertices<0 )
			pc->nVertices = -pc->nVertices;
		pc = pc->NextContour();
	}
	return result;
}


#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_

ODPolygon::IsEmpty( ) const
{
	// FIX: This is not very smart. It will probably be necessary to compute the area
	// of each contour...
	
	return _buf==kODNULL || _buf->nContours==0;
}

#ifdef OD_BUG  // 21439
WIN32_DLLEXPORT ODContour*
ODContour::NextContourEx(const ODSLong& specifiedVertices )
{
// NextContourEx(plicit)
// This is a workaround to a OD_BUG.  Basically, the 
// ODPolygon::operator== sets the sign bit on the contours that
// it has determined to be equal.  The problem is, when 
// NextContour is called, it takes the vertex and moves up by
// that amount.  Since the sign bit has been set to negative,
// this moves us backwards (i.e., the wrong way) in our contour
// table.  This version of NextContour takes a paramater that 
// allows you to specify the increment.  For us, we basically 
// pass in the same vertex, but we also take the absolute value
// of it as well.
             return   (ODContour*)&vertex[specifiedVertices];
}
#endif // OD_BUG

#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
ODPolygon::IsRectangular( ) const
{
	return _buf==kODNULL || _buf->nContours==0 ||
			(_buf->nContours==1  && _buf->firstContour.IsRectangular());
}


#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
ODPolygon::AsRectangle( ODRect *r ) const
{
	if( _buf==kODNULL || _buf->nContours==0 ) {
		r->Clear();
		return kODTrue;
	} else if( _buf->nContours==1 )
		return _buf->firstContour.AsRectangle(r);
	else
		return kODFalse;
}


#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
ODContour::IsRectangular( ) const
{
	if( nVertices != 4 )
		return kODFalse;
	else if( vertex[0].x == vertex[1].x )			// 1st edge is vertical
		return vertex[1].y==vertex[2].y
		    && vertex[2].x==vertex[3].x
		    && vertex[3].y==vertex[0].y;
	else if( vertex[0].y == vertex[1].y )			// 1st edge is horizontal
		return vertex[1].x==vertex[2].x
		    && vertex[2].y==vertex[3].y
		    && vertex[3].x==vertex[0].x;
	else
		return kODFalse;
}


#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
ODContour::AsRectangle( ODRect *r ) const
{
	ASSERT(r!=kODNULL,kODErrIllegalNullInput);
	
	if( this->IsRectangular() ) {
		ODRect r2(vertex[0],vertex[2]);			// C'tor properly orders the coords
		*r = r2;
		return kODTrue;
	} else
		return kODFalse;
}


/******************************************************************************/
//**	REGION CONVERSION
/******************************************************************************/


WIN32_DLLEXPORT ODBoolean
ODContour::HasExactRegion( ) const
{
	const ODPoint *b = &vertex[0];
#ifndef _PLATFORM_MACINTOSH_    // OD_BUG
	for( ODSLong i=nVertices; i>0; i-- ) {
		const ODPoint *a = &vertex[i-1];
#else  // The original code uses vertex[i] which is beyond vertex array.
	for( ODSLong i=nVertices; i>=0; i-- ) {
		const ODPoint *a = &vertex[i];
#endif  // OD_BUG
		if( (a->x & 0xFFFF) || (a->y & 0xFFFF) )
			return kODFalse;						// Non-integer coordinates
		if( a->x!=b->x && a->y!=b->y )
			return kODFalse;						// Diagonal line
		b = a;
	}
	return kODTrue;
}


WIN32_DLLEXPORT ODBoolean
ODPolygon::HasExactRegion( ) const
{
	const ODContour *c = this->FirstContour();
	for( long i=this->GetNContours(); i>0; i-- ) {
		if( !c->HasExactRegion() )
			return kODFalse;
		if( i>1 )
			c = c->NextContour();
	}
	return kODTrue;
}


#ifdef _PLATFORM_MACINTOSH_
PolyHandle
ODContour::AsQDPolygon( ) const
{
	ODSLong size = (sizeof(short)+sizeof(Rect)+sizeof(Point)) + nVertices*sizeof(Point);
	if( size > 32767 )
		THROW(kODErrShapeTooComplex);
	PolyHandle p = (PolyHandle) ODNewHandle(size);
	(**p).polySize = (short)size;
	Rect &bbox = (**p).polyBBox;
	SetRect(&bbox,32767,32767,-32767,-32767);
	
	const ODPoint *src = &vertex[0];
	Point *dst = &(**p).polyPoints[0];
	for( ODSLong i=nVertices; i>0; i-- ) {
		*dst = src->AsQDPoint();
		if( dst->h < bbox.left  )	bbox.left  = dst->h;
		if( dst->v < bbox.top   )	bbox.top   = dst->v;
		if( dst->h > bbox.right )	bbox.right = dst->h;
		if( dst->v > bbox.bottom)	bbox.bottom= dst->v;
		src++;
		dst++;
	}
	*dst = vertex[0].AsQDPoint();	// QD makes us repeat the 1st pt at the end
	
	return p;
}
#endif

#ifdef _PLATFORM_WIN32_
WIN32_DLLEXPORT Point *
ODContour::AsWinPolygon( ) const
{
	ODSLong size = 10 + nVertices*sizeof(Point);
	if( size > 32767 )
		THROW(kODErrShapeTooComplex);
	Point *p = (Point *) ODNewPtr(size);
	ODSLong* pl = (ODSLong*)p;
	*pl = nVertices;
	RECT bbox;
	SetRect(&bbox,32767,32767,-32767,-32767);
	
	const ODPoint *src = &vertex[0];
	Point *dst = &p[1];
	for( ODSLong i=nVertices; i>0; i-- ) {
		*dst = src->AsWinPoint();
		if( dst->x < bbox.left  )	bbox.left  = dst->x;
		if( dst->y < bbox.top   )	bbox.top   = dst->y;
		if( dst->x > bbox.right )	bbox.right = dst->x;
		if( dst->y > bbox.bottom)	bbox.bottom= dst->y;
		src++;
		dst++;
	}
	return p;
}

#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_AIX_
Point *
ODContour::AsXPolygon( ) const
{
	ODSLong size = 10 + nVertices*sizeof(Point);
	Point *p = (Point *) ODNewPtr(size);
	ODSLong* pl = (ODSLong*)p;
	*pl = nVertices;
	
	const ODPoint *src = &vertex[0];
	Point *dst = &p[1];
	for( ODSLong i=nVertices; i>0; i-- ) {
		*dst = src->AsXPoint();
		src++;
		dst++;
	}
	return p;
}
#endif // _PLATFORM_AIX_

#ifdef _PLATFORM_OS2_
void ODContour::AsPOLYGON( POLYGON& p) const
{

  p.aPointl = (PPOINTL)SOMMalloc(nVertices * sizeof(POINTL));

  const ODPoint *src = &vertex[0];
  POINTL *dst = p.aPointl;
  for( ODSLong i=nVertices; i>0; i-- ) {
    *dst = (src++)->AsPOINTL();
    (dst++)->y -= 1;              // OS/2 polygons are bottom-right exclusive
                                  // while OpenDoc polygons are top-right
                                  // exclusive.
  }
  p.ulPoints = nVertices;
}


// The canvas (HPS) is assumed to be associated with the target device
// for the which the region will be used, and already set up with the
// desired transforms.

HRGN
ODPolygon::AsRegion(HPS hps) const
{
  ODRgnHandle rgn;
  POLYGON poly;

  if( !this->HasData() )
    return GpiCreateRegion(hps, 0, 0);

  GpiSavePS(hps);
  GpiBeginPath(hps, 1);
  try {
    const ODContour *cont = this->FirstContour();
    for( ODSLong i=_buf->nContours; i>0; i-- ) {
      POLYGON poly;
      cont->AsPOLYGON( poly );
      GpiSetCurrentPosition(hps, &poly.aPointl[poly.ulPoints-1]);
      GpiPolyLine(hps, poly.ulPoints, poly.aPointl);
      //delete poly.aPointl;
      poly.aPointl = NULL;
      cont = cont->NextContour();
    }
  } catch (ODException _exception) {
    delete[] poly.aPointl;
    GpiEndPath(hps);
    GpiRestorePS(hps, -1);
    throw;
  }

  GpiEndPath(hps);
  rgn = GpiPathToRegion(hps, 1, FPATH_WINDING | FPATH_EXCL);
  GpiRestorePS(hps, -1);
  THROW_IF_NULL((void*)rgn);
  return rgn;
}
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_
RgnHandle
ODPolygon::AsQDRegion( ) const
{
	// NOTE: This method will not work properly for self-intersecting polygons!
	// QuickDraw uses even-odd filling, while OpenDoc uses winding-number.
	
	if( !this->HasData() )
		return ODNewRgn();

	GrafPtr port, myPort;

	// Make sure we have a real port to work with:
	GetPort(&port);
	if( FrontWindow() ) {
		SetPort(FrontWindow());
		myPort = kODNULL;
	} else {
		myPort = new GrafPort;
		OpenPort(myPort);
	}
	
	RgnHandle rgn = ODNewRgn();
	
	OpenRgn();
	try {
		PolyHandle poly;
		const ODContour *cont = this->FirstContour();
		for( ODSLong i=_buf->nContours; i>0; i-- ) {
			poly = cont->AsQDPolygon();
			FramePoly(poly);
			KillPoly(poly);
			cont = cont->NextContour();
		}
	} catch(ODException _exception) {
		CloseRgn(rgn);
		DisposeRgn(rgn);
		SetPort(port);
		if( myPort ) {
			ClosePort(myPort);
			delete myPort;
		}
		throw;
	}
	
	CloseRgn(rgn);
	
	SetPort(port);
	if( myPort ) {
		ClosePort(myPort);
		delete myPort;
	}
	return rgn;
}
#endif


#ifdef _PLATFORM_MACINTOSH_
gxShape
ODPolygon::AsGXShape( ) const
{
	gxShape shape;
	if( this->HasData() ) {
		shape = GXNewPolygons( (gxPolygons*)this->GetData() );
		GXSetShapeFill(shape,gxWindingFill);
	} else
		shape = GXNewShape(gxEmptyType);
	
	ThrowIfFirstGXError();
	return shape;
}
#endif

#ifdef _PLATFORM_WIN32_
WIN32_DLLEXPORT ODRgnHandle
ODPolygon::AsWinRegion( ) const
{
	
	ODRgnHandle rgn;
	HDC hdc = CreateCompatibleDC (NULL);
	
	try {
		Point *poly;
		const ODContour *cont = this->FirstContour();
		BeginPath(hdc);
		for( ODSLong i=_buf->nContours; i>0; i-- ) {
			poly = cont->AsWinPolygon();
			Polyline (hdc, &poly[1], *((ODSLong*)poly));
			CloseFigure(hdc);
			ODDisposePtr (poly);
			cont = cont->NextContour();
		}
		EndPath(hdc);
		rgn = PathToRegion(hdc);
//ShowRgn (rgn);
	} catch (ODException _exception) {
		DeleteObject(rgn);
		DeleteDC (hdc);
		throw;
	}

	DeleteDC (hdc);
	
	return rgn;
}
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_AIX_
Region DrawToXRegion(ODContour* target_contour) 
{
	unsigned long v_count = target_contour->nVertices;
	XPoint* xpoints = new XPoint[v_count];
	ODPoint* temp_vertex = kODNULL;

	for (unsigned long v = 0; v < v_count; v++)
	{
        	temp_vertex = &target_contour->vertex[v];
		if (!temp_vertex) break;

		xpoints[v].x = ODFixedRound(temp_vertex->x);
		xpoints[v].y = ODFixedRound(temp_vertex->y);
	}

	Region new_region = XPolygonRegion(xpoints, v_count, WindingRule);

	delete xpoints;

	return new_region;

}

WIN32_DLLEXPORT ODBoolean IsPositive(ODContour* target_contour) 
{
// Note:
// If the IsPositive function and the DrawToXRegion function are made into
// ODContour methods for AIX and
// if the data members fIsPositive and fSignKnown are added to the
// ODContour structure for AIX, we'd have to add a default c'tor that inits
// these fields (if a default c'tor can be added to a struct).  We'd also have
// to make sure that ODContours exist when the ODContour methods are 
// called.  If a user creates a struct myContour containing only nVertices and
// vertex[x] and NOT fIsPositive and NOT fSignKnown and then the user simply 
// creates a pointer to a ODContour and inits it to the myContour struct to 
// use the ODContour methods, the fIsPositive and fSignKnown fields would not
// be initialized causing us problems.  Assuming we can avoid these problems,
// saving _fIsPositive and _fSignKnown would be a performance enhancement.
 
// If this is changed to a ODContour method and the data fields are added:
// if (_SignKnown)
//	{
//	return (_fIsPositive);
//	}

	ODBoolean fIsPositive = kODTrue;  // Should this default to true????

	if (! target_contour->nVertices)
	{
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODContour::IsPositive - contour is empty\n");
		return 0;
	}
	
	long area_x_minus2 = 0;
	long v_count = target_contour->nVertices;

	ODPoint* v_i        = kODNULL;
	ODPoint* v_last_i   = kODNULL;

	for (ODULong i = 0; i < v_count; i++)
	{
	if (i == 0)
		v_last_i = &target_contour->vertex[v_count-1];
	else
		v_last_i = v_i;

	v_i = &target_contour->vertex[i];

	if ((! v_last_i) || (! v_i))
	{
		area_x_minus2 = 0;
		break;
	}

	// convert from Fixed to long before trying to do
	// the cross product, to avoid overflow.
	// ----------------------------------------
	long x_last_i   = ODFixedRound(v_last_i->x);
	long y_last_i   = ODFixedRound(v_last_i->y);
	long x_i        = ODFixedRound(v_i->x);
	long y_i        = ODFixedRound(v_i->y);

	area_x_minus2 += (x_last_i * y_i) - (y_last_i * x_i);
	}

	if (area_x_minus2 == 0)
	{
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODContour::IsPositive - contour has undefined area!\n");
		return kODFalse;
	}
	else
	{
// If _fIsPositive and _fSignKnown are added to ODContour for AIX:
//		_fIsPositive = (area_x_minus2 > 0);
		fIsPositive = (area_x_minus2 > 0);
	}

// If _fIsPositive and _fSignKnown are added to ODContour for AIX:
//	_fSignKnown = kODTrue;

	return fIsPositive;

}

WIN32_DLLEXPORT Region
ODPolygon::AsXRegion( ) const
{
	
	Region fRegion = XCreateRegion();

	// calculate Region if polygon data is available
	// ---------------------------------------------
	if ( (_buf) && 
	     ((_buf->nContours > 0) && (_buf->firstContour.nVertices > 0)) ) 
	{
		ODContour* contour =  &_buf->firstContour;

		for (ODULong c = 0 ; c < _buf->nContours ; c++)
		{
			if (!contour) return kODNULL;

			Region subregion = DrawToXRegion(contour);

			if (IsPositive(contour)) 
				XUnionRegion(fRegion, subregion, fRegion);
			else
				XSubtractRegion(fRegion, subregion, fRegion);

			XDestroyRegion(subregion);
			contour =  contour->NextContour();
		}
	}

	return (Region) fRegion;

}
#endif // _PLATFORM_AIX_

WIN32_DLLEXPORT void
ODPolygon::Transform( Environment *ev, ODTransform *xform )
{
	if( this->HasData() ) {
		ODContour *c = &_buf->firstContour;
		for( ODSLong i=_buf->nContours; i>0; i-- ) {
			ODPoint *pt = c->vertex;
			for( ODSLong v=c->nVertices; v>0; v--, pt++ )
				xform->TransformPoint(ev,pt);
			if( i>1 )
				c = c->NextContour();
		}
	}
}


/******************************************************************************/
//**	CONTAINMENT TEST
/******************************************************************************/


//------------------------------------------------------------------------------
// ODPolygon::Contains
//
// Does a polygon contain a point?
// We determine this by drawing a ray from the point to the right towards
// infinity, and finding the polygon edges that intersect this ray. For each
// such edge, count it as 1 if its y value is increasing, -1 if decreasing.
// The sum of these values is 0 if the point is outside the polygon.
//------------------------------------------------------------------------------

#ifndef _PLATFORM_WIN32_
WIN32_DLLEXPORT ODSLong
ODPolygon::Contains( ODPoint point ) const
{
	if( !this->HasData() )
		return kODFalse;

	ODSLong count = 0;
	const ODPoint *pp1, *pp2;
	ODPoint p1, p2;
	ODPoint ray = point;
	
	for( PolyEdgeIterator polyIter (this); polyIter.IsNotComplete(); polyIter.Next() ) {
		polyIter.CurrentEdge(pp1,pp2);
		p1 = *pp1;
		p2 = *pp2;
		
		if( p1.y==p2.y ) {											// Horizontal line: ignore
			if( p1.y==point.y && InRange(point.x, p1.x,p2.x) ) {	// unless point is on it
				return 0;
			}
		} else {
			ray.x = Max(p1.x,p2.x);
			ODPoint sect;
			if( ray.x >= point.x )
				if( IntersectSegments(p1,p2, point,ray, &sect) ) {
					if( WithinEpsilon(point.x,sect.x) && WithinEpsilon(point.y,sect.y) ) {
						return 0;
					}
					if( p2.y > p1.y )
						count++;
					else
						count--;
				}
		}
	}
	
	return count;
}
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_WIN32_

WIN32_DLLEXPORT ODSLong
ODPolygon::Contains( ODPoint point ) const
{
	ODRgnHandle rgn = this->AsWinRegion();
	ODBoolean b = kODFalse;

	if (rgn != kODNULL) 
	{
		Point pt = point.AsWinPoint();

		b = PtInRegion (rgn, pt.x, pt.y);

		DeleteObject (rgn);
	}

	return b;
}
#endif // _PLATFORM_WIN32_

/******************************************************************************/
//**	POLYGON EDGE ITERATOR
/******************************************************************************/


WIN32_DLLEXPORT PolyEdgeIterator::PolyEdgeIterator( const ODPolygon *poly )
	:fPoly (poly)
{
	fCurContour = poly->FirstContour();
	fCurContourIndex = 0;
	fCurVertex = 0;
}


WIN32_DLLEXPORT void
PolyEdgeIterator::CurrentEdge( const ODPoint* &v1, const ODPoint* &v2 )
{
	v1 = &fCurContour->vertex[fCurVertex];
	if( fCurVertex+1 < fCurContour->nVertices )
		v2 = v1+1;
	else
		v2 = &fCurContour->vertex[0];
}


#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
PolyEdgeIterator::Next( )
{
	if( !fCurContour )									// Was already finished
#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
		return kODFalse;
#else
		return false;
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
	if( ++fCurVertex >= fCurContour->nVertices )		// Next vertex; if past contour:
		if( ++fCurContourIndex >= fPoly->GetNContours() ) {		// Next contour; if past end:
			fCurContour = kODNULL;
#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
			return kODFalse;		//...we're done.
#else
			return false;										//...we're done.
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
		} else {
			fCurContour = fCurContour->NextContour();			// Else go to start of contour
			fCurVertex = 0;
		}
	return kODTrue;
}


#if ( (defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))  \
       || defined(_PLATFORM_AIX_) )
WIN32_DLLEXPORT ODBoolean
#else
Boolean
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_
PolyEdgeIterator::IsNotComplete( )
{
	return fCurContour!=kODNULL;
}


#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) && defined(DEBUG)
// test programs begin here


WIN32_DLLEXPORT void ODPolygon::SelfTest()
{

  // let's first set up some Contour and PolygonData info

  ODCoordinate a1 = IntToFixed(0);
  ODCoordinate b1 = IntToFixed(20);
  ODCoordinate p1 = IntToFixed(10);

  ODCoordinate myLeft = IntToFixed(20);
  ODCoordinate myTop = IntToFixed(20);
  ODCoordinate myRight = IntToFixed(100);
  ODCoordinate myBottom = IntToFixed(100);

  ODPoint point1(a1,a1);
  ODPoint point2(b1,a1);
  ODPoint point3(b1,b1);
  ODPoint point4(a1,b1);

  ODPoint point5(p1,p1);
  ODRect rect2(myLeft, myTop, myRight, myBottom);

  MyContour *contour1 = new MyContour;

  contour1->nVertices = (ODSLong)4;
  contour1->vertex[0] = point1;
  contour1->vertex[1] = point2;
  contour1->vertex[2] = point3;
  contour1->vertex[3] = point4;

  MyPolygonData *polyData = new MyPolygonData;
  MyPolygonData *polyData2 = new MyPolygonData;
  MyPolygonData *polyData3 = new MyPolygonData;
  MyPolygonData *polyData4 = new MyPolygonData;

  polyData->nContours = (ODSLong)1;
  polyData->firstContour = *contour1;

  polyData2->nContours = (ODSLong)1;
  polyData2->firstContour = *contour1;

  polyData3->nContours = (ODSLong)1;
  polyData3->firstContour = *contour1;

  polyData4->nContours = (ODSLong)1;
  polyData4->firstContour = *contour1;

  // let's test!
  printf ("\n\nBEGIN ODPolygon::SelfTest\n\n");


  // testing ctor
  ODPolygon polygon1;
  ODPolygon polygon2;
  ODPolygon polygon3;
  ODPolygon *polygon4 = new ODPolygon;

  // test ::SetData
  // test ::HasData
  polygon1.SetData((ODPolygonData*)polyData);
  polygon1.PrintIt("polygon1: testing ::SetData() and ::HasData()");
  CHECKRES(polygon1.HasData());

  // test ::IsRectangular  
  polygon1.PrintIt("polygon1: testing ::IsRectangular()");
  CHECKRES(polygon1.IsRectangular());

  // test ::GetDataSize()
  ODULong dataSize = polygon1.GetDataSize();
  printf("polygon1 data size:[%ld]\n", dataSize);
  polygon1.PrintIt("testing ::GetDataSize()");

  // test ::GetData()
  ODPolygonData *myPolygonData = polygon1.GetData();
  polygon1.PrintIt("testing ::GetData()");

  // test  ::GetNContours()
  ODSLong numContours = polygon1.GetNContours();
  polygon1.PrintIt("testing ::GetNContours()");
  printf("polygon1 number contours:[%ld]\n", numContours);

  // test ::FirstContour() 
  ODContour* firstContour = polygon1.FirstContour(); 
  polygon1.PrintIt("testing ::FirstContour()");

  // test ::ComputeBoundingBox()
  ODRect* boundingBox = new ODRect;
  polygon1.ComputeBoundingBox(boundingBox);
  polygon1.PrintIt("testing ::ComputeBoundingBox(ODRect)");
  
  // test ::SetData()
  polygon3.SetData((ODPolygonData*)polyData3);
  polygon3.PrintIt("polygon3: after ::SetData()");

  // test ::Transform
  polygon3.PrintIt("about to run polygon3::Transform test!");
  // Create locale SOM environment to pass into
  // SOM class calls used in this method.
  Environment* ev = SOM_CreateLocalEnvironment();
  ODULong rc = FALSE;
  ODTransform* xform = kODNULL;
  ODPoint pt(IntToFixed(2), IntToFixed(2));

  try
    {
        xform = new ODTransform;
        xform->InitTransform(ev);
        xform->Reset(ev);
        xform->ScaleBy(ev, &pt);

        polygon3.Transform(ev, xform);
        polygon3.PrintIt("after running polygon3::Transform scale*2 test!");
        ODContour *transContour = polygon3.FirstContour();
        ODPoint *transPoint = transContour->vertex;
        for( ODSLong v=transContour->nVertices; 
             ((v>0) && (rc==FALSE)); v--, transPoint++ )
        {
         if (v==4) {
          if ((transPoint->x != IntToFixed(0 * 2)) ||
              (transPoint->y != IntToFixed(0 * 2)))
                rc = TRUE;
          }
         if (v==3) {
          if ((transPoint->x != IntToFixed(20 * 2)) ||
              (transPoint->y != IntToFixed(0 * 2)))
                rc = TRUE;
          }
         if (v==2) {
          if ((transPoint->x != IntToFixed(20 * 2)) ||
              (transPoint->y != IntToFixed(20 * 2)))
                rc = TRUE;
          }
         if (v==1) {
          if ((transPoint->x != IntToFixed(0 * 2)) ||
              (transPoint->y != IntToFixed(20 * 2)))
                rc = TRUE;
          }
        }
        if (rc == FALSE)
            printf("PASS\n");
        else
           {
                printf("FAIL\n");
                printf("Failing point: v== %d \n",(4-v));
                printf("transPoint->x == %d \n",transPoint->x );
                printf("transPoint->y == %d \n",transPoint->y );
           }
      }
      catch(ODException _exception)
      {
         printf("ODException occurred while testing ODPolygon::Transform \n");
         printf("FAIL\n");
         // Clear SOM Exception 
         SetErrorCode(kODNoError);
       } 
       // Destroy local SOM environment we created.
       SOM_DestroyLocalEnvironment(ev);

  printf("End of ODPolygon::Transform test.\n");
  
  // test ::operator==
  polygon2.SetData((ODPolygonData*)polyData2);
  polygon2.PrintIt("polygon2: after ::SetData()");
  polygon1.PrintIt("polygon1: testing ::operator==");
  CHECKRES(polygon1 == polygon2);

  // test ::operator!=
  polygon1.PrintIt("polygon1: testing ::operator!=");
  CHECKRES(!(polygon1 != polygon2));

  // test ::Contains()
  polygon1.PrintIt("polygon1: testing ::Contains(ODPoint)");
  CHECKRES(polygon1.Contains(point5));

  // test ::IsEmpty()
  polygon1.PrintIt("polygon1: testing ::IsEmpty()");
  CHECKRES(!(polygon1.IsEmpty()));

  // test ::AsRectangle()
  ODRect *rect1 = new ODRect;
  polygon1.PrintIt("polygon1: testing ::AsRect(ODRect*)");
  CHECKRES(polygon1.AsRectangle(rect1) );

  // test ::HasExactRegion()
  polygon1.PrintIt("polygon1: testing ::HasExactRegion");
  CHECKRES(polygon1.HasExactRegion() );

#ifdef _PLATFORM_WIN32_
  // test ::AsWinRegion()
  polygon1.PrintIt("polygon1: testing ::AsWinRegion");
  ODRgnHandle myRegion = polygon1.AsWinRegion();
#elif defined(_PLATFORM_OS2_)
  // test ::AsRegion()
  polygon1.PrintIt("polygon1: testing ::AsRegion");
  HPS hps;
  hps = WinGetPS(HWND_DESKTOP);

  ODRgnHandle myRegion = polygon1.AsRegion(hps);
#elif defined(_PLATFORM_AIX_)
  // test ::AsXRegion()
  polygon1.PrintIt("polygon1: testing ::AsXRegion");

  ODRgnHandle myRegion = polygon1.AsXRegion();
  polygon1.PrintIt("polygon1: after ::AsXRegion");
#endif
  CHECKRES(myRegion);

  // test ::SetNVertices()
  ODPolygon *copyPolygon1 = polygon1.Copy();
  copyPolygon1->PrintIt("copyPolygon1: after polygon1.Copy()");
  copyPolygon1->SetNVertices((ODSLong) 5);
  copyPolygon1->PrintIt("copyPolygon1: after ::SetNVertices");
  polygon1.PrintIt("polygon1: after ::copyPolygon1->SetNVertices");

  // test ::SetVertices()
  ODPoint *point6 = new ODPoint;
  point6->Set(b1,b1);
  polygon2.PrintIt("polygon2: before ::SetVertices");
  polygon2.SetVertices((ODSLong) 1 , point6);
  polygon2.PrintIt("polygon2: after  ::SetVertices");

  // test ::SetContours()
  ODSLong pointArray[4] = {(ODSLong)20,(ODSLong)20,(ODSLong)100,(ODSLong)100};
  polygon2.PrintIt("polygon2: before ::SetContours");
  polygon2.SetContours((ODSLong)1, pointArray);
  polygon2.PrintIt("polygon2: after  ::SetContours");

  polygon1.PrintIt("polygon1: before the Copy call!!");
  ODPolygon *copyPolygon = polygon1.Copy();
  copyPolygon->PrintIt("copyPolygon: after polygon1.Copy()");
  CHECKRES(polygon1 == *copyPolygon);

  // test ::SetRect()
  polygon2.SetRect(rect2);
  polygon2.PrintIt("polygon2: after ::SetRect(rect2)");

  // test ::MoveFrom()
  polygon1.MoveFrom(polygon2);
  polygon1.PrintIt("polygon1: after ::MoveFrom(polygon2)");
  CHECKRES( (polygon2.IsEmpty()) );

  // test ::SetData
  polygon4->SetData((ODPolygonData*)polyData4);
  polygon4->PrintIt("polygon4: testing ::SetData()");

  // Delete the MyPolygonData.  
  // Note: Can't call polygon1.Delete() since polygon1,2, & 3 are on the stack
  printf ("\n Deleting polyData and polygon4 \n");
  ODDisposePtr(polyData);
  ODDisposePtr(polyData2);
  ODDisposePtr(polyData3);
  polygon4->Delete();

  printf ("\n\nEND ODPolygon::SelfTest\n\n");

 
// STILL need to add test cases for these:
  // test ::ReadFrom()
  // test ::WriteTo()
 

}

WIN32_DLLEXPORT void ODPolygon::PrintIt(char *msg)
{
  printf ("-----------------------------------------------------\n");
  if(msg != NULL)
    printf ("%s\n",msg);
    if( this->HasData() ) {
      ODContour *c = &_buf->firstContour;
      for( ODSLong i=_buf->nContours; i>0; i-- ) {
        ODPoint *pt = c->vertex;
        for( ODSLong v=c->nVertices; v>0; v--, pt++ )
          printf("Point[%d]: %d,%d\n",(c->nVertices - v), pt->x, pt->y);
          if( i>1 )
            c = c->NextContour();
      } // for
    } // if
}
#endif // defined (_PLATFORM_WIN32_ || OS2 || AIX ) && defined(DEBUG)
