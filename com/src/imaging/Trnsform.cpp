/* @(#)Z 1.9 com/src/imaging/Trnsform.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:05 (96/10/29 09:25:34) */
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

/*
	File:		Trnsform.cpp

	Contains:	Macintosh implementation of ODTransform

	Owned by:	Jens Alfke

	Copyright:	© 1994 Apple Computer, Inc. All rights reserved.

	Change History (most recent first):

		 <8>	 10/8/95	TJ		Fixes Recomended by Refball
		 <7>	 12/5/94	jpa		Fixes from code review. [1203923]
		 <6>	 8/17/94	jpa		Function prefixes. [1181512]
		 <5>	  8/8/94	jpa		Updated after base class API change
									[1179574]
		 <4>	 7/26/94	eeh		ODQDPoint -> Point
		 <3>	 7/15/94	jpa		SetQDOffset now takes a Point*.
		 <2>	 6/30/94	jpa		Filled in methods.
		 <1>	 6/15/94	jpa		first checked in

	To Do:
*/


#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif 

#define ODTransform_Class_Source
#include <Trnsform.xih>


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

#ifndef _MATRIX_
#include "Matrix.h"
#endif

#ifndef _ODMATH_
#include "ODMath.h"
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#endif   // IBM Platforms 


#ifndef _LINEOPS_
#include <LineOps.h>      /* For kFixedEpsilon */
#endif


static ODBoolean AlmostInteger( ODFixed n )
{
	ODSShort f = (ODSShort)n;				// f is fractional part of n
	return f<=(ODSShort)kFixedEpsilon && f>=-(ODSShort)kFixedEpsilon;
}


SOM_Scope void  SOMLINK ODTransformInitTransform(ODTransform *somSelf, Environment *ev)
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","InitTransform");

	SOM_TRY
	somSelf->InitBaseTransform(ev);
	SOM_CATCH_ALL
	SOM_ENDTRY
}


#if ( defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_) )

SOM_Scope ODBoolean  SOMLINK ODTransformIsIntegerOffset(ODTransform *somSelf, Environment *ev)
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","IsIntegerOffset");

#else

SOM_Scope ODBoolean  SOMLINK ODTransform__IsQDOffset(ODTransform *somSelf, Environment *ev)
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","IsQDOffset");

#endif  // IBM Platforms

	SOM_TRY
	
	if( somSelf->GetType(ev) <= kODTranslateXform ) {
		ODPoint offset;
		somSelf->GetOffset(ev, &offset);
		return AlmostInteger(offset.x) &&
			   AlmostInteger(offset.y);
	} else
		return kODFalse;
	
	SOM_CATCH_ALL
	SOM_ENDTRY
	return kODFalse;
}



//
// OS/2 Specific methods
//
#ifdef _PLATFORM_OS2_
SOM_Scope void  SOMLINK ODTransformGetMATRIXLF(ODTransform *somSelf,  Environment *ev,
                                               MATRIXLF* mtx)
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","GetMATRIXLF");


    ASSERT_NOT_NULL(mtx);

    ODMatrix m;
    somSelf->GetMatrix(ev, &m);

    mtx->fxM11 = m.m[0][0];
    mtx->fxM12 = m.m[0][1];
    mtx->lM13  = 0;
    mtx->fxM21 = m.m[1][0];
    mtx->fxM22 = m.m[1][1];
    mtx->lM23  = 0;
    mtx->lM31  = ODFixedRound(m.m[2][0]);
    mtx->lM32  = ODFixedRound(m.m[2][1]);
    mtx->lM33  = 1;
}

SOM_Scope void  SOMLINK ODTransformSetMATRIXLF(ODTransform *somSelf,  Environment *ev,
                                               MATRIXLF* mtx)
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","SetMATRIXLF");

    ASSERT_NOT_NULL(mtx);

    ODMatrix m;

    /*
     * TODO: We should check that lM31 and lM32 are not greater than 64k and
     *       throw an exception if they are.
     */

    m.m[0][0] = mtx->fxM11;
    m.m[0][1] = mtx->fxM12;
    m.m[0][2] = 0;
    m.m[1][0] = mtx->fxM21;
    m.m[1][1] = mtx->fxM22;
    m.m[1][2] = 0;
    m.m[2][0] = ODIntToFixed(mtx->lM31);
    m.m[2][1] = ODIntToFixed(mtx->lM32);
    m.m[2][2] = kODFract1;

    somSelf->SetMatrix(ev, &m);
}
#endif  // _PLATFORM_OS2_


// 122004 :
//------------------------------------------------------------------------------
//  TransformPoints
//------------------------------------------------------------------------------

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)

SOM_Scope void SOMLINK ODTransformTransformPoints(ODTransform *somSelf, Environment *ev,
     ODByteArray* Points, ODULong nPoints )
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","TransformPoints");

  ODMatrix m;
  somSelf->GetMatrix(ev, &m);
  if( MxMul(&m,somSelf->GetType(ev), (ODPoint*)Points->_buffer, nPoints) )
    THROW(kODErrTransformErr);
}
// end 122004
#endif  // IBM Platforms


//
// Macintosh specific code
//
#ifdef _PLATFORM_MACINTOSH_

SOM_Scope Point  SOMLINK ODTransform__GetQDOffset(ODTransform *somSelf, Environment *ev)
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","GetQDOffset");

	ODPoint pt;

	SOM_TRY
	somSelf->GetOffset(ev,&pt);
	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return pt.AsQDPoint();
}



SOM_Scope ODTransform*  SOMLINK ODTransform__SetQDOffset(ODTransform *somSelf, Environment *ev,
		Point* point)
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","SetQDOffset");

	SOM_TRY
	
	ODPoint offset( *point );
	somSelf->SetOffset(ev,&offset);
	
	SOM_CATCH_ALL
	SOM_ENDTRY
	return somSelf;
}
#endif  // _PLATFORM_MACINTOSH_




//
// Windows specific code
//
#ifdef _PLATFORM_WIN32_

SOM_Scope Point  SOMLINK ODTransformGetOffsetAsPoint( ODTransform *somSelf, 
                                                        Environment *ev )
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","GetOffsetAsPoint");

	ODPoint pt;

	SOM_TRY
	somSelf->GetOffset( ev, &pt );
	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return pt.AsWinPoint();
}



SOM_Scope ODTransform*  SOMLINK ODTransformSetOffsetUsingPoint( ODTransform *somSelf, 
                                                                Environment *ev,
		                                                Point* point )
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","SetOffsetWithPoint");

	SOM_TRY
	
	ODPoint offset( *point );
	somSelf->SetOffset( ev, &offset );
	
	SOM_CATCH_ALL
	SOM_ENDTRY
	return somSelf;
}
#endif  // _PLATFORM_WIN32_

//
// UNIX specific code
//
#ifdef _PLATFORM_UNIX_

SOM_Scope Point  SOMLINK ODTransformGetOffsetAsPoint( ODTransform *somSelf, 
                                                        Environment *ev )
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","GetOffsetAsPoint");

	ODPoint pt;

	SOM_TRY
	somSelf->GetOffset( ev, &pt );
	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return pt.AsXPoint();
}



SOM_Scope ODTransform*  SOMLINK ODTransformSetOffsetUsingPoint( ODTransform *somSelf, 
                                                                Environment *ev,
		                                                Point* point )
{
    /* ODTransformData *somThis = ODTransformGetData(somSelf); */
    ODTransformMethodDebug("ODTransform","SetOffsetWithPoint");

	SOM_TRY
	
	ODPoint offset( *point );
	somSelf->SetOffset( ev, &offset );
	
	SOM_CATCH_ALL
	SOM_ENDTRY
	return somSelf;
}
#endif  // _PLATFORM_UNIX_





