/* @(#)Z 1.9 com/src/imaging/Shape.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:04 (96/10/29 09:25:24) */
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
/*          3/18/96 jab  Merge with Apple DR4 code.  Add platform   */
/*                       specific code for Windows.                 */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
	File:		Shape.cpp

	Contains:	ODShape class (Macintosh subclass of ODBaseShape)

	Owned by:	Jens Alfke

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <8>	 8/26/95	T‚		1257260 Remove  somInit methods. Don't call
									IsInitialized
		 <7>	 3/20/95	jpa		SOM_CATCH --> SOM_TRY [1224355]
		 <6>	 12/5/94	jpa		Call Simplify after SetPolygon. Removed
									some test code.
		 <5>	11/28/94	SS		Merged MacTech branch changes (1198242)
		 <4>	 8/17/94	jpa		Function prefixes. [1181512]
		 <3>	 7/26/94	jpa		Added somInit so it could initialize gGX
									flag.
		 <2>	 6/20/94	jpa		Filled in method implementations.
		 <1>	 6/15/94	jpa		first checked in

	To Do:
	In Progress:
*/


#ifdef _PLATFORM_OS2_
#define INCL_WIN
#define INCL_GPI
#endif

#define ODShape_Class_Source
#include <Shape.xih>

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#ifndef _PLATSHAPE_
#include "PlatShpe.h"
#endif
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

//------------------------------------------------------------------------------
//	InitShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODShapeInitShape( ODShape *somSelf, Environment *ev )
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","InitShape");

	SOM_TRY
	somSelf->InitBaseShape(ev);

	SOM_CATCH_ALL
	SOM_ENDTRY
}


//
// IBM Platforms
//
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)

SOM_Scope ODRgnHandle  SOMLINK ODShapeGetRegion( ODShape *somSelf, Environment *ev )
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","GetRegion");

  SOM_TRY;
#ifdef _PLATFORM_OS2_
    return (ODRgnHandle) somSelf->GetPlatformShape( ev, kODPM );
#endif 

#ifdef _PLATFORM_WIN32_
    return (ODRgnHandle) somSelf->GetPlatformShape( ev, kODWin32 );
#endif 

#ifdef _PLATFORM_AIX_
    return (ODRgnHandle) somSelf->GetPlatformShape( ev, kODAIX );
#endif 


  SOM_CATCH_ALL
  SOM_ENDTRY
  return kODNULL;
}


SOM_Scope void  SOMLINK ODShapeSetRegion( ODShape *somSelf, Environment *ev,
                                            ODRgnHandle rgn )
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","SetRegion");

  SOM_TRY
#ifdef _PLATFORM_OS2_
     somSelf->SetPlatformShape( ev, kODPM, (ODPlatformShape)rgn );
#endif

#ifdef _PLATFORM_WIN32_
     somSelf->SetPlatformShape( ev, kODWin32, (ODPlatformShape)rgn );
#endif

#ifdef _PLATFORM_AIX_
     somSelf->SetPlatformShape( ev, kODAIX, (ODPlatformShape)rgn );
#endif


  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODRgnHandle  SOMLINK ODShapeCopyRegion( ODShape *somSelf, Environment *ev )
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","CopyRegion");

  SOM_TRY
#ifdef _PLATFORM_OS2_
     ODHandle rgn = (ODHandle) somSelf->GetPlatformShape( ev,kODPM );
#endif 

#ifdef _PLATFORM_WIN32_
     ODHandle rgn = (ODHandle) somSelf->GetPlatformShape( ev, kODWin32 );
#endif 

#ifdef _PLATFORM_AIX_
     ODHandle rgn = (ODHandle) somSelf->GetPlatformShape( ev, kODAIX );
#endif 

     return (ODRgnHandle) ODCopyHandle(rgn);

  SOM_CATCH_ALL
  SOM_ENDTRY
  return kODNULL;
}

#endif  // IBM Platforms


//
// Macintosh Specific code
//
#ifdef _PLATFORM_MACINTOSH

//------------------------------------------------------------------------------
//	GetQDRegion
//------------------------------------------------------------------------------

SOM_Scope ODRgnHandle  SOMLINK ODShape__GetQDRegion(ODShape *somSelf, Environment *ev)
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","GetQDRegion");

	SOM_TRY
    return (RgnHandle) somSelf->GetPlatformShape(ev,kODQuickDraw);

	SOM_CATCH_ALL
	SOM_ENDTRY
	return kODNULL;
}

//------------------------------------------------------------------------------
//	SetQDRegion
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODShape__SetQDRegion(ODShape *somSelf, Environment *ev,
		ODRgnHandle rgn)
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","SetQDRegion");

	SOM_TRY
	somSelf->SetPlatformShape(ev,kODQuickDraw,(ODPlatformShape)rgn);

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	GetGXShape
//------------------------------------------------------------------------------

SOM_Scope ODgxShape  SOMLINK ODShape__GetGXShape(ODShape *somSelf, Environment *ev)
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","GetGXShape");

	SOM_TRY
    return (gxShape) somSelf->GetPlatformShape(ev,kODQuickDrawGX);

	SOM_CATCH_ALL
	SOM_ENDTRY
	return kODNULL;
}

//------------------------------------------------------------------------------
//	SetGXShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODShape__SetGXShape(ODShape *somSelf, Environment *ev,
		ODgxShape s)
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","SetGXShape");

	SOM_TRY
	somSelf->SetPlatformShape(ev,kODQuickDrawGX,(ODPlatformShape)s);

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
//	CopyQDRegion
//------------------------------------------------------------------------------

SOM_Scope ODRgnHandle  SOMLINK ODShape__CopyQDRegion(ODShape *somSelf, Environment *ev)
{
    /* ODShapeData *somThis = ODShapeGetData(somSelf); */
    ODShapeMethodDebug("ODShape","CopyQDRegion");
	
	SOM_TRY

	ODHandle rgn = (ODHandle) somSelf->GetPlatformShape(ev,kODQuickDraw);
	return (RgnHandle) ODCopyHandle(rgn);

  SOM_CATCH_ALL
  SOM_ENDTRY
  return kODNULL;
} 

#endif  // _PLATFORM_MACINTOSH_



