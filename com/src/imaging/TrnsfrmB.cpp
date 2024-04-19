/* @(#)Z 1.14 com/src/imaging/TrnsfrmB.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:05 (96/10/29 09:25:39) */
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
/*          3/15/96 jab  Merged DR4 with OS/2 os2_feb13 code base.  */
/*  129073  7/11/95 ced  Use SOM_RAISE instead of ODSetSOMException */
/*                       to get error logging, etc.                 */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
	File:		TrnsfrmB.cp

	Contains:	Implementation of ODBaseTransform

	Owned by:	Jens Alfke

	Copyright:	© 1993-94 Apple Computer, Inc. All rights reserved.

	Change History (most recent first):
	
		<18>	 10/8/95	TJ		Fixes Recomended by Refball
		<17>	 9/13/95	DM		1277216 GM:API return no ODPoints nor
									ODPolygons
		<16>	  9/8/95	jpa		Use SET_SOM_EXCEPTION [1281031]
		<15>	 8/18/95	NP		1274946:
									kODErrInvalidParameter->kODErrIllegalNullIn
									put
		<14>	 8/16/95	jpa		Use ODGet/SetMatrixProp in ReadFrom/WriteTo
									[1276814]
		<13>	 8/12/95	T‚		1276808 Call ODReleaseObject instead of
									ODDeleteObject on shapes & transforms
		<12>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<11>	 3/20/95	jpa		SOM_CATCH --> SOM_TRY [1224355]
		<10>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <9>	12/20/94	jpa		Added composite xform support [1205794]
		 <8>	 12/5/94	jpa		Code-review fixes, error handling & such.
									[1203923]
		 <7>	 9/29/94	RA		1189812: Mods for 68K build.
		 <6>	  9/9/94	jpa		Added Release method, and verify refcnt in
									somUninit. [1185660]
		 <5>	 8/17/94	jpa		Function prefixes. [1181512]
		 <4>	  8/8/94	jpa		Added NewTransform [1179574]
		 <3>	 7/15/94	jpa		CopyFrom now calls InitBaseTransform in
									case caller had just created the object.
		 <2>	 6/30/94	jpa		Ported implementation to SOM.
		 <1>	 6/15/94	jpa		first checked in (stubs)
		 ---------------------------Moved to ODSOM project.
		 <6>	 5/27/94	jpa		Fixed bug in ScaleBy, noticed while copying
									code over to SOM.
		 <5>	  5/9/94	jpa		Use new built-in matrix code. [1162092]
		 <4>	 3/25/94	JA		Rewrote & changed API to support scaling
									for QD parts. (1153438)
		 <3>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <2>	 2/16/94	JA		Include new AltPoint.h.
		<15>	  2/7/94	JA		Further tigerings.
		<14>	  2/3/94	JA		Tiger Team Makeover!
		<13>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		<12>	11/23/93	JA		Minor tweaks.
		<11>	10/14/93	JA		No more copying in get/setters.
		 <9>	 10/6/93	JA		Use new fixed-point ODPoint.
		 <8>	 9/23/93	JA		Restored MoveBy. Added Mac-specific
									convenience methods.
		 <7>	 9/22/93	JA		Oops! Removed leftover line of test code.
		 <6>	 9/22/93	JA		Updated ASSERTs for new macro.
		 <5>	 9/20/93	JA		Post-brownies: Okay, REALLY fixed
									ReadPlatformTransform.
		 <4>	 9/17/93	T‚		fixed ReadPlatformTransform
		 <3>	 9/16/93	JA		Added Read/WritePlatformTransform. Removed
									constructor w/params. Use copy-in, copy-out
									semantics.
		 <2>	 9/07/93	jpa		Fleshed out class, cleverly letting GX do the matrix grunge.
		 <1>	 8/18/93	T‚		first checked in
		 <0>	 8/18/93	JBS		created
	
	Theory Of Operation:
	
		An ODBaseTransform is mostly a holder for an ODMatrix. It also caches an
		inverse of its matrix for use in InvertPoint and Invert operations.
		Since it is possible for subclasses of ODBaseTransform to exist that do not
		use matrices (nonlinear transforms like morphs or spherizers), it must be
		able to coexist with these. They can be detected by sending them a
		HasMatrix message. The major problem arises when I'm asked to compose
		myself with one of these; matrix multiplication is right out, so I have
		to keep around a copy of the other transform and do manual composition. In
		fact I have to keep around two potential transforms, one for pre-composi-
		tion and one for post-composition.
		It is illegal to externalize a composite transform, since this would be
		difficult to implement. In practice the only transforms that get extern-
		alized are frames' internal transforms, which are probably not composite.
		
		Anyone wanting to implement nonlinear transforms needs to subclass
		ODBaseTransform and override at least the following methods:
			GetMatrix	[need to throw ODErrTransformErr]
			HasMatrix	[need to return false]
			Reset
			Copy
			CopyFrom
			Invert
			PreCompose
			PostCompose
			TransformPoint
			InvertPoint
			TransformShape
			InvertShape
			WriteTo
			ReadFrom

	To Do:
	In Progress:
		
*/

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ALTPOINT_
#include "AltPoint.h"			/* Use C++ savvy ODPoint and ODRect*/
#endif


#define ODBaseTransform_Class_Source
#define VARIABLE_MACROS
#include <TrnsfrmB.xih>


#ifndef SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef _MATRIX_
#include "Matrix.h"
#endif

#ifndef _ODMATH_
#include "ODMath.h"
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _LINEOPS_
#include "LineOps.h"					/* For kFixedEpsilon */
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif


#define BAIL_IF_NULL(PARAM,VAL) \
	if( (PARAM)!=kODNULL ) ; else {	\
	WARNMSG_DEBUG(WARN_INDEX(-1),"Invalid NULL parameter " #PARAM); \
	ODSetSOMException(ev, kODErrIllegalNullInput); \
	return VAL; \
	}

#define VOID /**/


#ifdef _PLATFORM_MACINTOSH__
#pragma segment ODTransform
#endif


//==============================================================================
// HOUSEKEEPING
//==============================================================================


//------------------------------------------------------------------------------
//	somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseTransformsomUninit(ODBaseTransform *somSelf)
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","somUninit");
   
	ODDeleteObject(_fInverseMatrix);
	ODSafeReleaseObject(_fPreTransform);
	ODSafeReleaseObject(_fPostTransform);
	
	ODBaseTransform_parents_somUninit(somSelf);

}

//------------------------------------------------------------------------------
//	InitBaseTransform
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseTransformInitBaseTransform(ODBaseTransform *somSelf, Environment *ev)
{
    ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","ODBaseTransformInitBaseTransform");

	SOM_TRY
	
	/* Moved from somInit. SOM itself sets fields to zero
	_fInverseMatrix = kODNULL;
	_fPreTransform = kODNULL;
	_fPostTransform = kODNULL;
	*/
	_fType = kODIdentityXform;
	_fMatrix = kODIdentityMatrix;
	
	somSelf->InitRefCntObject(ev);
	
	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseTransformRelease(ODBaseTransform *somSelf, Environment *ev)
{
//	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseShape","Release");

	SOM_TRY

#ifdef _PLATFORM_OS2_
        parent_Release(somSelf, ev);
#else
	ODBaseTransform_parents_Release(somSelf,ev);
#endif // _PLATFORM_OS2_
	
	// Delete myself when my refCount goes to zero:
	if( somSelf->GetRefCount(ev) < 1 )
		delete somSelf;

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODBaseTransformPurge(ODBaseTransform *somSelf, Environment *ev,
		ODSize size)
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","Purge");

	ODUnused(size);
	
	if( _fInverseMatrix ) {
		ODDeleteObject(_fInverseMatrix);
		return sizeof(ODMatrix);
	} else
		return 0;
}


//==============================================================================
// GETTERS
//==============================================================================


//------------------------------------------------------------------------------
//	GetType
//------------------------------------------------------------------------------

SOM_Scope ODTransformType SOMLINK ODBaseTransformGetType(ODBaseTransform *somSelf, Environment *ev)
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","GetType");

	if( _fPreTransform || _fPostTransform )
		return kODUnknownXform;
	else
		return _fType;
}

//------------------------------------------------------------------------------
//	GetMatrix
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformGetMatrix(ODBaseTransform *somSelf, Environment *ev,
		 ODMatrix *m )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","GetMatrix");

	BAIL_IF_NULL(m,VOID);
	*m = _fMatrix;
	if( _fPreTransform || _fPostTransform )
		ODSetSOMException(ev,kODErrTransformErr);
}

//------------------------------------------------------------------------------
//	HasMatrix
//------------------------------------------------------------------------------

SOM_Scope ODBoolean SOMLINK ODBaseTransformHasMatrix(ODBaseTransform *somSelf, Environment *ev)
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","HasMatrix");

	return _fPreTransform==kODNULL && _fPostTransform==kODNULL;
}

//------------------------------------------------------------------------------
//	GetOffset
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformGetOffset(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *p )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","GetOffset");

	BAIL_IF_NULL(p,VOID);
	p->x = ODFractDivide(_fMatrix.m[2][0], _fMatrix.m[2][2]);
	p->y = ODFractDivide(_fMatrix.m[2][1], _fMatrix.m[2][2]);
}

//------------------------------------------------------------------------------
//	GetScale
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformGetScale(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *scale )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","GetScale");

	BAIL_IF_NULL(scale,VOID);
	scale->x = ODFractDivide(_fMatrix.m[0][0], _fMatrix.m[2][2]);
	scale->y = ODFractDivide(_fMatrix.m[1][1], _fMatrix.m[2][2]);
}

//------------------------------------------------------------------------------
//	GetPreScaleOffset
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformGetPreScaleOffset(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *offset )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","GetPreScaleOffset");

	// This returns the offset to use if you are going to apply the offset
	// _before_ scaling. Just divide the translation elements by the scales.
	
	BAIL_IF_NULL(offset,VOID);
	offset->x = ODFixedDivide(_fMatrix.m[2][0],_fMatrix.m[0][0]);
	offset->y = ODFixedDivide(_fMatrix.m[2][1],_fMatrix.m[1][1]);
	
	if( offset->x==kODFixedInfinity || offset->x==kODFixedMinusInfinity
	  ||offset->y==kODFixedInfinity || offset->y==kODFixedMinusInfinity )
			ODSetSOMException(ev,kODErrTransformErr);
}

//------------------------------------------------------------------------------
//	IsSameAs
//------------------------------------------------------------------------------

static ODBoolean Neq( ODFixed a, ODFixed b )
{
	return (a-b > kFixedEpsilon) || (a-b < -kFixedEpsilon);
}

SOM_Scope ODBoolean SOMLINK ODBaseTransformIsSameAs(ODBaseTransform *somSelf, Environment *ev,
		 ODTransform *xform )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","IsSameAs");

	SOM_TRY
	
	BAIL_IF_NULL(xform,kODFalse);
	
	if( _fPreTransform || _fPostTransform )
		return kODFalse;					// ????? What else can I do??

	if( (ODBaseTransform*)xform != somSelf) {
		ODMatrix m0, m1, *mp;
		xform->GetMatrix(ev,&m1);
		
		if(_fMatrix.m[2][2]!=kODFract1 ) {
			m0 = _fMatrix;
			MxNormalize(&m0);				// Normalize my matrix
			mp = &m0;
		} else
			mp = &_fMatrix;
		if(m1.m[2][2]!=kODFract1 )
			MxNormalize(&m1);				// Normalize other matrix
			
		for( long i=8; i>=0; i-- )
			if( Neq(mp->m[0][i],m1.m[0][i]) )	// Compare matrices
				return kODFalse;
	}
	return kODTrue;

	SOM_CATCH_ALL
	SOM_ENDTRY
	return kODFalse;
}


//==============================================================================
// SETTERS
//==============================================================================


//------------------------------------------------------------------------------
//	SetMatrix
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformSetMatrix(ODBaseTransform *somSelf, Environment *ev,
		 ODMatrix *m )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","SetMatrix");

	SOM_TRY
	
	if( m == kODNULL )
		somSelf->Reset(ev);
	else {
		ODTransformType type = MxType(m);
		if( type==kODUnknownXform )
			THROW(kODErrTransformErr);
		_fMatrix = *m;
		_fType = type;
		ODDeleteObject(_fInverseMatrix);
		ODReleaseObject(ev,_fPreTransform);
		ODReleaseObject(ev,_fPostTransform);
	}

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	Reset
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformReset(ODBaseTransform *somSelf, Environment *ev)
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","Reset");

	_fMatrix = kODIdentityMatrix;
	_fType = kODIdentityXform;
	ODDeleteObject(_fInverseMatrix);
	ODSafeReleaseObject(_fPreTransform);	_fPreTransform = kODNULL;
	ODSafeReleaseObject(_fPostTransform);_fPostTransform= kODNULL;
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	NewTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODBaseTransformNewTransform(ODBaseTransform *somSelf, Environment *ev)
{
//	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","NewTransform");
	
	ODTransform *x = new ODTransform;
	if( !x )
		ODSetSOMException(ev,kODErrOutOfMemory);
	else {
		TRY{
			x->InitTransform(ev);
		}CATCH_ALL{
			ODSafeReleaseObject(x);
			SET_SOM_EXCEPTION(ev);
		}ENDTRY
	}
	return x;
}

//------------------------------------------------------------------------------
//	Copy
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformCopy(ODBaseTransform *somSelf, Environment *ev )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","Copy");

	ODTransform *x = kODNULL; ODVolatile(x);
	
	SOM_TRY
	
		x = new ODTransform;
		THROW_IF_NULL(x);
		x->InitTransform(ev);
		x->SetMatrix(ev,&_fMatrix);
		if( _fPreTransform ) {
			ODTransform *t = _fPreTransform->Copy(ev);
			x->PreCompose(ev,t);
			t->Release(ev);
		}
		if( _fPostTransform ) {
			ODTransform *t = _fPostTransform->Copy(ev);
			x->PostCompose(ev,t);
			t->Release(ev);
		}
	
	SOM_CATCH_ALL
		ODSafeReleaseObject(x);
		x = kODNULL;
	SOM_ENDTRY
	return x;
}

//------------------------------------------------------------------------------
//	CopyFrom
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformCopyFrom(ODBaseTransform *somSelf, Environment *ev,
		 ODTransform *xform )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","CopyFrom");

	SOM_TRY
	
	BAIL_IF_NULL(xform,kODNULL);

	if( xform->HasMatrix(ev) ) {
		xform->GetMatrix(ev,&_fMatrix);
		_fType = xform->GetType(ev);
		ODDeleteObject(_fInverseMatrix);
	} else {
		// If xform is some weird nonmatrix thing, I'll just have to make myself
		// an identity and make it my pre-transform:
		somSelf->Reset(ev);
		_fPreTransform = xform->Copy(ev);
	}

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}


//==============================================================================
// MATRIX TRANSFORMATIONS
//==============================================================================


//------------------------------------------------------------------------------
//	SetOffset
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformSetOffset(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *offset )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","SetOffset");

	SOM_TRY
	
	BAIL_IF_NULL(offset, kODNULL);

	somSelf->Reset(ev);
	_fMatrix.m[2][0] = offset->x;
	_fMatrix.m[2][1] = offset->y;
	_fType = kODTranslateXform;

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	MoveBy
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformMoveBy(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *offset )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","MoveBy");

	SOM_TRY
	
	BAIL_IF_NULL(offset, kODNULL );

	ODMatrix m = _fMatrix;
	if( MxMove(&m,_fType,offset->x,offset->y) )
		THROW(kODErrTransformErr);
	somSelf->SetMatrix(ev, &m);

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	ScaleBy
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformScaleBy(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *scale )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","ScaleBy");

	SOM_TRY
	
	BAIL_IF_NULL(scale, kODNULL );

	ODMatrix m = _fMatrix;
	if( MxScale(&m,scale->x,scale->y) )
		THROW(kODErrTransformErr);
	somSelf->SetMatrix(ev, &m);

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	ScaleDownBy
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformScaleDownBy(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *scale )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","ScaleDownBy");

	SOM_TRY
	
	BAIL_IF_NULL( scale, kODNULL );

	if( scale->x==0 || scale->y==0 )
		THROW(kODErrTransformErr);
	ODPoint s;
	s.x = ODFixedDivide(kODFixed1,scale->x);
	s.y = ODFixedDivide(kODFixed1,scale->y);
	somSelf->ScaleBy(ev, &s);

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	BuildInverse
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformBuildInverse(ODBaseTransform *somSelf, Environment *ev)
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","BuildInverse");

	SOM_TRY
	
	WASSERT(!_fInverseMatrix);
	_fInverseMatrix = new ODMatrix;
	if( MxInverse(&_fMatrix,_fType, _fInverseMatrix) ) {
		ODDeleteObject(_fInverseMatrix);
		THROW(kODErrTransformErr);
	}
	
	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	Invert
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformInvert(ODBaseTransform *somSelf, Environment *ev )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","Invert");
	
	SOM_TRY

	if( !_fInverseMatrix )  {
		somSelf->BuildInverse(ev);
	
           // BuildInverse can fail if the current matrix is not invertible.
           // If not invertible a SOM Exception is set and _fInverseMatrix is
           // set to NULL.  Without the following check this condition goes 
           // undetected and crashes in MxMul because _fInverseMatrix is NULL. 
           //    jab - 4/30/96
           if( !_fInverseMatrix ) 
	       return kODNULL;
        }

	if( _fPreTransform || _fPostTransform ) {
		// When inverting, switch pre with post and invert each one.
		ODBaseTransform *pre = _fPreTransform;
		_fPreTransform = _fPostTransform ?_fPostTransform->Invert(ev) :kODNULL;
		_fPostTransform = pre ?pre->Invert(ev) :kODNULL;
	}
	
	// Swap matrix with inverse:
	ODFixed *a = &_fMatrix.m[0][0];
	ODFixed *b = &_fInverseMatrix->m[0][0];
	for( ODSLong i=9; i>0; i-- ) {
		register ODFixed temp = *a;
		*(a++) = *b;
		*(b++) = temp;
	}
	
	// No need to change _fType, since matrix and inverse have same type.
	
	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	PreCompose
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformPreCompose(ODBaseTransform *somSelf, Environment *ev,
		 ODTransform *t )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","PreCompose");

	SOM_TRY
	BAIL_IF_NULL(t,kODNULL);
	
	if( _fPreTransform )
		_fPreTransform->PreCompose(ev,t);
	else if( !t->HasMatrix(ev) )
		_fPreTransform = t->Copy(ev);
	else {
		ODMatrix m;
		t->GetMatrix(ev,&m);
		if( MxConcat( &_fMatrix,_fType, &m,t->GetType(ev) ) )	// m = m * _fMatrix
			THROW(kODErrTransformErr);
		somSelf->SetMatrix(ev, &m);
	}

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	PostCompose
//------------------------------------------------------------------------------

SOM_Scope ODTransform* SOMLINK ODBaseTransformPostCompose(ODBaseTransform *somSelf, Environment *ev,
		 ODTransform *t )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","PostCompose");

	SOM_TRY
	BAIL_IF_NULL(t,kODNULL);
	
	if( _fPostTransform )
		_fPostTransform->PostCompose(ev,t);
	else if( !t->HasMatrix(ev) )
		_fPostTransform = t->Copy(ev);
	else {
		ODMatrix m;
		ODMatrix me = _fMatrix;
		t->GetMatrix(ev,&m);
		if( MxConcat( &m,t->GetType(ev), &me,_fType) )				// me = me * m
			THROW(kODErrTransformErr);
		somSelf->SetMatrix(ev, &me);
	}

	SOM_CATCH_ALL
	SOM_ENDTRY
	return (ODTransform*) somSelf;
}

//------------------------------------------------------------------------------
//	TransformPoint
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformTransformPoint(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *pt )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","TransformPoint");
	
	SOM_TRY

	BAIL_IF_NULL(pt, VOID);

	ODPoint xp = *pt;
	
	if( _fPreTransform )
		_fPreTransform->TransformPoint(ev,&xp);
	if( MxMul(&_fMatrix,_fType, &xp,1) )
		THROW(kODErrTransformErr);
	if( _fPostTransform )
		_fPostTransform->TransformPoint(ev,&xp);

	*pt = xp;
	
	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	InvertPoint
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformInvertPoint(ODBaseTransform *somSelf, Environment *ev,
		 ODPoint *pt )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","InvertPoint");


	SOM_TRY

	BAIL_IF_NULL(pt, VOID);

	ODPoint inv = *pt;

	if( !_fInverseMatrix )  {
		somSelf->BuildInverse(ev);
	
           // BuildInverse can fail if the current matrix is not invertible.
           // If not invertible a SOM Exception is set and _fInverseMatrix is
           // set to NULL.  Without the following check this condition goes 
           // undetected and crashes in MxMul because _fInverseMatrix is NULL. 
           //    jab - 4/30/96
           if( !_fInverseMatrix ) 
	       return;
        }
 
	if( _fPostTransform )
		_fPostTransform->InvertPoint(ev,&inv);
	if( MxMul(_fInverseMatrix,_fType, &inv,1) )
		THROW(kODErrTransformErr);
	if( _fPreTransform )
		_fPreTransform->InvertPoint(ev,&inv);
		
	*pt = inv;

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	TransformShape
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformTransformShape(ODBaseTransform *somSelf, Environment *ev,
		 ODShape *shape )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","TransformShape");

	SOM_TRY
	
	BAIL_IF_NULL(shape,VOID);
	if( _fPreTransform )
		_fPreTransform->TransformShape(ev,shape);
	shape->Transform(ev, (ODTransform*)somSelf);
	if( _fPostTransform )
		_fPostTransform->TransformShape(ev,shape);
	
	/*	This call is (a) a convenience for clients and (b) provided for
		implementations of non-affine transformations, i.e. transformations
		that do not map straight lines onto straight lines. ODShapes trans-
		form themselves by running all their control points through the
		transform. For a non-affine transformation this is not sufficient
		and the transform itself will have to do the work. In fact, if
		the transform has no matrix (HasMatrix()==false), then the body of
		this method must _not_ call ODShape::Transform(somSelf) or an endless
		recursion will result! (Look at ODShape::Transform to see why.)
	*/

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	InvertShape
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformInvertShape(ODBaseTransform *somSelf, Environment *ev,
		 ODShape *shape )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","InvertShape");

	SOM_TRY
	
	BAIL_IF_NULL(shape,VOID);
	if( _fPostTransform )
		_fPostTransform->TransformShape(ev,shape);
	shape->InverseTransform(ev, (ODTransform*)somSelf);
	if( _fPreTransform )
		_fPreTransform->TransformShape(ev,shape);
	
	// See above comment in TransformShape.

	SOM_CATCH_ALL
	SOM_ENDTRY
}


//==============================================================================
// INPUT / OUTPUT
//==============================================================================


//------------------------------------------------------------------------------
// WriteTo
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformWriteTo(ODBaseTransform *somSelf, Environment *ev,
		 ODStorageUnit *su )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","WriteTo");
	
	SOM_TRY
	BAIL_IF_NULL(su,VOID);
	
	if( _fPreTransform || _fPostTransform) {
		WARNMSG(WARN_INDEX(AMSG_790),"Tried to externalize composite transform %p",somSelf);
	}
	
	ODSUForceFocus(ev,su,kODNULL,kODTransform);
	ODULong size = su->GetSize(ev);
	if( size!=0 )
		su->DeleteValue(ev,size);		// Nuke previous contents
	ODSetMatrixProp(ev, su, kODNULL,kODNULL, &_fMatrix);

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ReadFrom
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBaseTransformReadFrom(ODBaseTransform *somSelf, Environment *ev,
		 ODStorageUnit *su )
{
	ODBaseTransformData *somThis = ODBaseTransformGetData(somSelf);
	ODBaseTransformMethodDebug("ODBaseTransform","ReadFrom");

	SOM_TRY
	
	ODReleaseObject(ev,_fPreTransform);
	ODReleaseObject(ev,_fPostTransform);
	
	BAIL_IF_NULL(su,VOID);
	ODMatrix m;
	ODGetMatrixProp(ev, su, kODNULL, kODTransform, &m);
	somSelf->SetMatrix(ev, &m);

	SOM_CATCH_ALL
	SOM_ENDTRY
}
