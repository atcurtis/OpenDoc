/* @(#)Z 1.14 com/src/imaging/ShapeB.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:04 (96/10/29 09:25:29) */
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
/*          3/22/96 jab  Merge with DR4. Changes for cross          */
/*                       platform support.                          */
/*  142710 11/17/95 ced  put back fix for [125719]                  */
/*  122017  5/12/95 aml  Added the extra Environment* parameter to  */
/*                       some RealShape method calls                */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
	File:		ShapeB.cpp

	Contains:	Implementation of ODBaseShape class.

	Owned by:	Jens Alfke

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<20>	10/17/95	jpa		1293487: ReadShape doesn't delete & return
									NULL if shape is empty.
		<19>	 10/8/95	TJ		Fixes Recomended by Refball
		<18>	 9/13/95	DM		1277216 GM:API return no ODPoints nor
									ODPolygons
		<17>	 9/12/95	JBS		1283120 Memory leak in ::ReadShape
		<16>	 8/12/95	T‚		1276808 Call ODReleaseObject instead of
									ODDeleteObject on shapes & transforms
		<15>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<14>	  5/4/95	jpa		IsSameAs checks for NULL target realshape.
									[1238838]
		<13>	 4/15/95	jpa		Added exception handling for Purge method
									[1236970]. Intersect, Subtract and Union
									check for NULL target realshape. [1238838]
		<12>	 3/20/95	jpa		SOM_CATCH --> SOM_TRY [1224355]
		<11>	 2/24/95	jpa		Fixed bug in CopyFrom if src shape was
									newly created (no RealShape). [1220690]
		<10>	 1/12/95	jpa		Removed five-dollar comment [1209502]
		 <9>	 12/5/94	jpa		Call Simplify after SetPolygon. Other code
									review fixes. [1203923]
		 <8>	10/24/94	jpa		Use NewGeometricShape to instantiate
									polygon/gx shapes. [1195070]
		 <7>	 9/29/94	RA		1189812: Mods for 68K build.
		 <6>	  9/9/94	jpa		Added Release method, and verify refcnt in
									somUninit. [1185660]
		 <5>	 8/17/94	jpa		Function prefixes. [1181512]
		 <4>	  8/8/94	jpa		Added Outset, NewShape, Reset [1178690,
									1179574]
		 <3>	 7/15/94	jpa		CopyFrom now calls InitBaseShape in case
									caller had not yet initialized the object.
		 <2>	 7/13/94	jpa		Call InitObject in InitBaseShape.
		 <1>	 6/15/94	jpa		first checked in

	To Do:
	In Progress:
		
*/

#ifdef _PLATFORM_OS2_
#define INCL_WIN
#define INCL_GPI
#endif // _PLATFORM_OS2_

#include "AltPoint.h"
#include "AltPoly.h"

#define ODBaseShape_Class_Source
#define VARIABLE_MACROS
#include <ShapeB.xih>

#include <Shape.xh>
#include <Trnsform.xh>
#include <StorageU.xh>

#include <Except.h>
#include <ODUtils.h>

#include "RectShpe.h"
#include "RgnShpe.h"
#include "PolyShpe.h"

#ifdef _PLATFORM_MACINTOSH_
#include "GXShpe.h"
#endif  // _PLATFORM_MACINTOSH_

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif


//------------------------------------------------------------------------------
//	somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapesomUninit( ODBaseShape *somSelf )
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","somUninit");
	
	ODDeleteObject( _fShape );

	parent_somUninit( somSelf );
}

//------------------------------------------------------------------------------
//	InitBaseShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeInitBaseShape( ODBaseShape *somSelf, Environment *ev )
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","InitBaseShape");

	SOM_TRY

	   /* Moved from somInit. SOM itself sets fields to zero
	   _fShape = kODNULL;
	   */
	   _fMode = kODPreserveGeometry;
		
	   somSelf->InitRefCntObject(ev);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

           if( !_fShape ) {   // [125710], [125715]
              ODRect initRect;
              initRect.Clear();
              _fShape = new RectShape( _fMode, initRect );
           }

#endif   // IBM Platforms

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODBaseShapePurge(ODBaseShape *somSelf, Environment *ev,
											ODSize bytes)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Purge");
	
	ODSize volatile purged = 0; ODVolatile( purged );
	
	SOM_TRY

	purged = parent_Purge(somSelf,ev,bytes);
	bytes -= purged;
	
	if( _fShape && bytes>0 )
		purged += _fShape->Purge( bytes );
	
	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return purged;
}

//------------------------------------------------------------------------------
//	Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeRelease(ODBaseShape *somSelf, Environment *ev)
{
//	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Release");

	parent_Release( somSelf, ev );
	
	// Delete myself when my refCount goes to zero:
	if( somSelf->GetRefCount(ev) == 0 )
		delete somSelf;
}

//------------------------------------------------------------------------------
//	NewShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeNewShape(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","NewShape");
	
	ODShape *shape = kODNULL;		ODVolatile( shape );
	
	SOM_TRY
	
	shape = new ODShape;
	THROW_IF_NULL( shape );
	shape->InitShape( ev );
	if( _fMode != kODPreserveGeometry )
		shape->SetGeometryMode( ev, _fMode );

	SOM_CATCH_ALL
		ODSafeReleaseObject( shape ); 
                shape = kODNULL;
	SOM_ENDTRY
	
	return shape;
}

//------------------------------------------------------------------------------
//	Copy
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeCopy(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Copy");
	
	ODShape *copy = kODNULL;		ODVolatile( copy );
	
	SOM_TRY

	copy = new ODShape;
	THROW_IF_NULL( copy );
	copy->InitShape( ev );
	copy->CopyFrom( ev, (ODShape*)somSelf );

	SOM_CATCH_ALL
		ODSafeReleaseObject( copy );
		copy = kODNULL;
	SOM_ENDTRY
	
	return copy;
}

//------------------------------------------------------------------------------
//	CopyFrom
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeCopyFrom(ODBaseShape *somSelf, Environment *ev,
		ODShape* sourceShape)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","CopyFrom");
	
	SOM_TRY
	
	ASSERT_NOT_NULL( sourceShape );
	RealShape *newShape = sourceShape->GetRealShape( ev );
	if( newShape )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined (_PLATFORM_UNIX_)
                newShape = newShape->Copy( ev );
#else
                newShape = newShape->Copy();
#endif   // IBM Platforms

	delete _fShape;
	_fShape = newShape;
	_fMode = sourceShape->GetGeometryMode( ev );

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	GetRealShape
//------------------------------------------------------------------------------

SOM_Scope RealShape*  SOMLINK ODBaseShapeGetRealShape(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","GetRealShape");

	return _fShape;
}

//------------------------------------------------------------------------------
//	GetGeometryMode
//------------------------------------------------------------------------------

SOM_Scope ODGeometryMode  SOMLINK ODBaseShapeGetGeometryMode(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","GetGeometryMode");

	if( _fShape )
		return _fShape->GetGeometryMode();
	else
		return _fMode;
}

//------------------------------------------------------------------------------
//	SetGeometryMode
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeSetGeometryMode(ODBaseShape *somSelf, Environment *ev,
		ODGeometryMode mode)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","SetGeometryMode");

	SOM_TRY
	
	if( _fShape )
		_fShape->SetGeometryMode( mode );
	_fMode = mode;

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	GetBoundingBox
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeGetBoundingBox(ODBaseShape *somSelf, Environment *ev,
		ODRect* bounds)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","GetBoundingBox");
	
	SOM_TRY
	
	ASSERT_NOT_NULL( bounds );
	if( _fShape )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                _fShape->GetBoundingBox( ev, bounds) ;
#else
		_fShape->GetBoundingBox( bounds );
#endif   // IBM Platforms

	else
		bounds->Clear();

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	SetRectangle
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeSetRectangle(ODBaseShape *somSelf, Environment *ev,
		ODRect* rect)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","SetRectangle");

	SOM_TRY
	
	ASSERT_NOT_NULL( rect );
	if( !_fShape )
		_fShape = new RectShape( _fMode, *rect );
	else
		_fShape= _fShape->SetRectangle( rect );

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	CopyPolygon
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeCopyPolygon(ODBaseShape *somSelf, Environment *ev,
		ODPolygon* polygon)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","CopyPolygon");
   
	SOM_TRY
	
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
	if( _fShape ) {
		_fShape->CopyPolygon( ev, *polygon );
	}		
	else
	{
		ODPolygon empty;
		polygon->CopyFrom( empty );
	}
#else
	if( _fShape )

		_fShape->CopyPolygon( *polygon );
	else
	{
		ODPolygon empty;
		polygon->CopyFrom( empty );

	}
#endif   // IBM Platforms

	SOM_CATCH_ALL
	SOM_ENDTRY
}


//------------------------------------------------------------------------------
//	SetPolygon
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeSetPolygon(ODBaseShape *somSelf, Environment *ev,
		ODPolygon* polygon)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","SetPolygon");
	
	SOM_TRY
	
	ASSERT_NOT_NULL( polygon );
	if( !_fShape ) {
		ODRect r;
		if( polygon->AsRectangle( &r ) )
			_fShape = new RectShape( _fMode, r );
		else if( _fMode == kODLoseGeometry )
			_fShape = new RgnShape( _fMode );
		else
			_fShape = RealShape::NewGeometricShape( _fMode );
			
		TRY{
			_fShape = _fShape->SetPolygon( *polygon );
			_fShape->Simplify();		// PolygonShape needs to simplify polygon
		}CATCH_ALL{
			delete _fShape;
			_fShape = kODNULL;
			RERAISE;
		}ENDTRY
		
	} else {
		_fShape = _fShape->SetPolygon( *polygon );
                _fShape->Simplify(); // as per John B. Sounds like a good idea to me!
        }

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	GetPlatformShape
//------------------------------------------------------------------------------

SOM_Scope ODPlatformShape  SOMLINK ODBaseShapeGetPlatformShape(ODBaseShape *somSelf, Environment *ev,
		ODGraphicsSystem graphicsSystem)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","GetPlatformShape");

	SOM_TRY

	if( !_fShape ) {
		ODRect r;
		r.Clear();
		somSelf->SetRectangle( ev, &r );
	}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        return _fShape->GetPlatformShape( ev, graphicsSystem );
#else
	return _fShape->GetPlatformShape( graphicsSystem );
#endif   // IBM Platforms

	SOM_CATCH_ALL
	SOM_ENDTRY
	return kODNULL;
}

//------------------------------------------------------------------------------
//	SetPlatformShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeSetPlatformShape(ODBaseShape *somSelf, Environment *ev,
		ODGraphicsSystem graphicsSystem,
		ODPlatformShape platformShape)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","SetPlatformShape");

	SOM_TRY
		
	ASSERT_NOT_NULL( platformShape );
	
	RealShape *s;
	
#ifdef _PLATFORM_OS2_ // CED

        if( graphicsSystem == kODPM )
           s = new RgnShape( _fMode );
	else
	   THROW( kODErrInvalidGraphicsSystem );

#endif  // _PLATFORM_OS2_


#ifdef _PLATFORM_WIN32_

        if( (graphicsSystem == kODWinNT) || (graphicsSystem == kODWin32) || (graphicsSystem == kODWin95) )
           s = new RgnShape( _fMode );
	else
	   THROW( kODErrInvalidGraphicsSystem );

#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_AIX_

        if( graphicsSystem == kODAIX )
           s = new RgnShape( _fMode );
	else
	   THROW( kODErrInvalidGraphicsSystem );

#endif  // _PLATFORM_AIX_


#ifdef _PLATFORM_MACINTOSH_

	if( graphicsSystem == kODQuickDraw )
		s = new RgnShape( _fMode );
	else if( graphicsSystem == kODQuickDrawGX && gGX )
		s = new QDGXShape( _fMode );
	else
		THROW( kODErrInvalidGraphicsSystem );

#endif  // _PLATFORM_MACINTOSH_

	TRY{
		s->SetPlatformShape( graphicsSystem, platformShape );
	}CATCH_ALL{
		delete s;
		RERAISE;
	}ENDTRY
	
	delete _fShape;
	_fShape = s;

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	Reset
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeReset(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Reset");
	
	SOM_TRY

	if( _fShape )
		_fShape = _fShape->Clear();

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	WriteShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseShapeWriteShape(ODBaseShape *somSelf, Environment *ev,
		ODStorageUnit* su)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","WriteShape");
	
	SOM_TRY


        if (su == kODNULL)
           THROW(kODErrIllegalNullStorageUnitInput);
	
	ODPolygon poly;
	if( _fShape )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
           _fShape->CopyPolygon( ev, poly );
#else
	   _fShape->CopyPolygon( poly );
#endif  // IBM Platforms

	TRY{
		poly.WriteTo( ev, su );
	}CATCH_ALL{
		poly.Clear();
		RERAISE;
	}ENDTRY
	poly.Clear();

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	ReadShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeReadShape(ODBaseShape *somSelf, Environment *ev,
		ODStorageUnit* su)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","ReadShape");


    SOM_TRY

    if (su == kODNULL) {
       THROW(kODErrIllegalNullStorageUnitInput);
       return kODNULL;
    }
	
    ODTempPolygon poly;
    poly.ReadFrom( ev, su );

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)

    if (poly.IsRectangular()) {
        ODRect rect;
        poly.AsRectangle( &rect );
        somSelf->SetRectangle( ev, &rect );
    } else

#endif  // IBM Platforms

        somSelf->SetPolygon( ev, &poly );
	
    SOM_CATCH_ALL
    SOM_ENDTRY
	
    return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	IsSameAs
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseShapeIsSameAs(ODBaseShape *somSelf, Environment *ev,
		ODShape* compareShape)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","IsSameAs");

	SOM_TRY
	
	ASSERT_NOT_NULL( compareShape );
	if( _fShape ) {
  	   RealShape *itsShape = compareShape->GetRealShape( ev );
	   if( itsShape )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)

              return _fShape->IsSameAs( ev, itsShape );
           else
              return _fShape->IsEmpty( ev );

#else

              return _fShape->IsSameAs( itsShape );
           else
              return _fShape->IsEmpty();

#endif    // IBM Platforms

	} else
	   return compareShape->IsEmpty( ev );

	SOM_CATCH_ALL
	SOM_ENDTRY
	return kODFalse;
}

//------------------------------------------------------------------------------
//	IsEmpty
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseShapeIsEmpty(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","IsEmpty");

  SOM_TRY

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
    return _fShape ?_fShape->IsEmpty( ev ) :kODTrue;
#else
    return _fShape ?_fShape->IsEmpty() :kODTrue;
#endif   // IBM Platforms

    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODFalse;
}

//------------------------------------------------------------------------------
//	ContainsPoint
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseShapeContainsPoint(ODBaseShape *somSelf, Environment *ev,
		ODPoint* point)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","ContainsPoint");

    SOM_TRY

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
    ASSERT_NOT_NULL( point );
    return _fShape ?_fShape->ContainsPoint( ev, *point ) :kODFalse;
#else
    return _fShape ?_fShape->ContainsPoint( *point ) :kODFalse;
#endif   // IBM Platforms

    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODFalse;
}

//------------------------------------------------------------------------------
//	IsRectangular
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseShapeIsRectangular(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","IsRectangular");

    SOM_TRY

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
    return _fShape ?_fShape->IsRectangular( ev ) :kODTrue;
#else
    return _fShape ?_fShape->IsRectangular() :kODTrue;
#endif   // IBM Platforms

    SOM_CATCH_ALL
    SOM_ENDTRY
    return kODFalse;
}

//------------------------------------------------------------------------------
//	HasGeometry
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseShapeHasGeometry(ODBaseShape *somSelf, Environment *ev)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","HasGeometry");

	SOM_TRY
	
	return _fShape ?_fShape->HasGeometry() :kODTrue;

	SOM_CATCH_ALL
	SOM_ENDTRY
	return kODFalse;
}

//------------------------------------------------------------------------------
//	Transform
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeTransform(ODBaseShape *somSelf, Environment *ev,
		ODTransform* xform)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Transform");

	SOM_TRY
	
	ASSERT_NOT_NULL( xform );
	if( _fShape ) {
		if( xform->HasMatrix( ev ) )
			_fShape = _fShape->Transform( ev, xform );
		else
			xform->TransformShape( ev, (ODShape*)somSelf );
	}

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	InverseTransform
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeInverseTransform(ODBaseShape *somSelf, Environment *ev,
		ODTransform* xform)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","InverseTransform");

	SOM_TRY
	
	/* ***** Temporarily inverts xform, which may cause problems with threads.
			 The alternative would be to copy xform and invert the copy. */
	
	// In the implementation of ODTransform, Invert is a fully reversible operation
	// with no round-off error. (The transform caches an inverse matrix, and
	// inverting the transform again is just a matter of swapping matrices.)
	
	ASSERT_NOT_NULL( xform );
	
	if( _fShape ) {
		if( xform->HasMatrix( ev ) ) {
			xform->Invert( ev );
			TRY{
				_fShape = _fShape->Transform( ev, xform );
			}CATCH_ALL{
				xform->Invert( ev );
				RERAISE;
			}ENDTRY
			xform->Invert( ev );
		
		} else
			xform->InvertShape( ev, (ODShape*)somSelf );
	}

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	Subtract
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeSubtract(ODBaseShape *somSelf, Environment *ev,
		ODShape* diffShape)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Subtract");

	SOM_TRY
	
	ASSERT_NOT_NULL( diffShape );
	if( _fShape ) {
	   RealShape *itsShape = diffShape->GetRealShape( ev );
	   if( itsShape )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
	      _fShape = _fShape->Subtract( ev, itsShape );
#else
	      _fShape = _fShape->Subtract( itsShape );
#endif    //  IBM Platforms 

	}
	
	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	Intersect
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeIntersect(ODBaseShape *somSelf, Environment *ev,
		ODShape* sectShape)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Intersect");

	SOM_TRY
	
	ASSERT_NOT_NULL( sectShape );

	if( _fShape ) {
	   RealShape *itsShape = sectShape->GetRealShape( ev );
	   if( itsShape )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
	      _fShape = _fShape->Intersect( ev, itsShape );
#else
	      _fShape = _fShape->Intersect( itsShape );
#endif  //  IBM Platforms 

	   else
	      somSelf->Reset( ev );
	}

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	Union
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeUnion(ODBaseShape *somSelf, Environment *ev,
		ODShape* unionShape)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Union");

	SOM_TRY
	
	ASSERT_NOT_NULL( unionShape );
	if( _fShape ) {
	   RealShape *itsShape = unionShape->GetRealShape( ev );
	   if( itsShape )

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
	      _fShape = _fShape->Union( ev, itsShape );
#else
	      _fShape = _fShape->Union( itsShape );
#endif   //  IBM Platforms 

	} else
		somSelf->CopyFrom( ev, unionShape );

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODShape*) somSelf;
}

//------------------------------------------------------------------------------
//	Outset
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODBaseShapeOutset(ODBaseShape *somSelf, Environment *ev,
		ODCoordinate distance)
{
	ODBaseShapeData *somThis = ODBaseShapeGetData(somSelf);
	ODBaseShapeMethodDebug("ODBaseShape","Outset");
	
	if( distance != 0 && _fShape ) {
	   SOM_TRY

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  || defined(_PLATFORM_UNIX_)
	      _fShape = _fShape->Outset( ev, distance );
#else
	      _fShape = _fShape->Outset( distance );
#endif   // IBM Platforms  

	   SOM_CATCH_ALL
	   SOM_ENDTRY
	}
	return (ODShape*) somSelf;
}
