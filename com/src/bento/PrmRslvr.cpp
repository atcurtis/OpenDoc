/* @(#)Z 1.11 com/src/bento/PrmRslvr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:35 (96/10/29 09:13:14) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
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
	File:		PrmRslvr.cpp

	Contains:	Implementation of Storage Unit for the Mac Bento Container Suite.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<12>	 8/18/95	VL		1267187: Refcount sourcepart correctly.
		<11>	 6/29/95	VL		1242642: delete suView in CATCH block of
									ResolvePromise.
		<10>	 6/16/95	VL		1259613: Fixed up ResolvePromise to handle
									SetOffset on an unfocused storage unit.
		 <9>	 5/26/95	VL		1251403: Multithreading naming support.
		 <8>	 4/10/95	VL		1236490: Made GetPromiseValue work during
									FulfillPromise.
		 <7>	 3/14/95	VL		1225208,1225201: Added IsSettingPromise to
									ensure that we don't call ResolvePromise
									while setting a promise.
		 <6>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <5>	 10/4/94	VL		1153379: Do not include SUPriv.h anymore.
		 <4>	 9/29/94	RA		1189812: Mods for 68K build.
		 <3>	 9/15/94	VL		1184871: Fixed ResolvePromise to handle
									Drag and Drop correctly.
		 <2>	 8/11/94	VL		1180299: Use modified CreateView.
		 <1>	 7/26/94	VL		first checked in

	To Do:
*/

#ifndef _PRMRSLVR_
#include "PrmRslvr.h"
#endif

#ifndef SOM_CMStorageUnit_xh
#include <CMSU.xh>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef __CM_API__
#include <CMAPI.h>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef SOM_ODStorageUnitCursor_xh
#include <SUCursor.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"	// Adkins -- added
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif

//==============================================================================
// PromiseResolver
//==============================================================================

//------------------------------------------------------------------------------
// PromiseResolver::IsPromiseValue
//------------------------------------------------------------------------------

ODBoolean PromiseResolver::IsPromiseValue(Environment* ev)
{		
	CMValue value = fSU->GetCurValue(ev);
	return (this->GetPromiseInfo(value) ? kODTrue : kODFalse);
}

//------------------------------------------------------------------------------
// PromiseResolver::SetSourcePart
//------------------------------------------------------------------------------

void PromiseResolver::SetSourcePart(Environment* ev,
									 ODPart *sourcePart)
{
	ODValueRefCon*		oldRefCon;
	ODValueRefCon*		newRefCon;
	CMValue		        value;
	ProcessSerialNumber thePSN;
	
	value = fSU->GetCurValue(ev);

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
	OSErr result = 0;
#else
	OSErr result = GetCurrentProcess(&thePSN);
	THROW_IF_ERROR(result);
#endif

	oldRefCon = (ODValueRefCon *) this->GetPromiseInfo(value);
	if (oldRefCon == kODNULL)
	{
		this->IncrementPromiseCount();
		newRefCon = (ODValueRefCon *) ODNewPtr(sizeof(ODValueRefCon),
												fSU->GetHeap(ev));
		ODAcquireObject(ev,	sourcePart);
		newRefCon->sourcePart = sourcePart;
		newRefCon->sourcePSN = thePSN;
		this->SetPromiseInfo(value, newRefCon);
	}
	else if (oldRefCon->sourcePart != sourcePart)
	{
		oldRefCon->sourcePart = sourcePart;
		oldRefCon->sourcePSN = thePSN;
	}
}

//------------------------------------------------------------------------------
// PromiseResolver::GetSourcePart
//------------------------------------------------------------------------------

ODPart* PromiseResolver::GetSourcePart(Environment* ev)
{	
	CMValue value = fSU->GetCurValue(ev);

	ODValueRefCon* theRefCon = (ODValueRefCon *) this->GetPromiseInfo(value);
	if (theRefCon == kODNULL) {
		if (fResolvingPromiseInfo)
			theRefCon = fResolvingPromiseInfo;
		else
			THROW(kODErrNotPromise);
	}
	return theRefCon->sourcePart;
}

//------------------------------------------------------------------------------
// PromiseResolver::ClearPromise
//------------------------------------------------------------------------------

void PromiseResolver::ClearPromise(Environment* ev)
{
	CMValue value = fSU->GetCurValue(ev);
	if (value != kODNULL) {
		ODPtr theRefCon = this->GetPromiseInfo(value);
		if (theRefCon) {
			ODPart* sourcePart = ((ODValueRefCon*) theRefCon)->sourcePart;
			if (sourcePart)
				ODReleaseObject(ev, sourcePart);
			ODDisposePtr(theRefCon);
			this->SetPromiseInfo(value, kODNULL);
			this->DecrementPromiseCount();
		}
	}
}

//------------------------------------------------------------------------------
// PromiseResolver::ResolvePromise
//------------------------------------------------------------------------------

void PromiseResolver::ResolvePromise(Environment* ev)
{
	CMValue value = fSU->GetCurValue(ev);
	ODValueRefCon* theRefCon = (ODValueRefCon *) this->GetPromiseInfo(value);
	if (theRefCon)
	{
		ODStorageUnitView*	destSUView = kODNULL;
		ODVolatile(destSUView);
		TRY
			fResolvingPromiseInfo = theRefCon;

			// First we must remove the refCon to stop any recursion
			this->SetPromiseInfo(value, kODNULL);
			
			// Create a view for this storage unit
			destSUView = fSU->CreateView(ev);
			
#ifdef _PLATFORM_MACINTOSH_
			ODDragAndDrop* dragAndDrop = fSU->GetSession(ev)->GetDragAndDrop(ev);
			if (dragAndDrop->GetDragReference(ev) == 0) {
				Boolean             sameProcess;
#else
				ODBoolean           sameProcess = kODTrue;
#endif
				OSErr               result;
				ProcessSerialNumber thePSN;
				
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
				result = 0;
#else
				result = GetCurrentProcess(&thePSN);
				THROW_IF_ERROR(result);
				result = SameProcess(&(theRefCon->sourcePSN), &thePSN, &sameProcess);
				THROW_IF_ERROR(result);
#endif
				
				if (sameProcess) {
					ODPart*	sourcePart = theRefCon->sourcePart;
					if (sourcePart)
						sourcePart->FulfillPromise(ev, destSUView);
				}
				else {
					WARNMSG(WARN_INDEX(AMSG_410),"Cannot fulfill promise in another process.");
				}
#ifdef _PLATFORM_MACINTOSH_
			}
			else {	
				fSU->GetSession(ev)->GetDragAndDrop(ev)->GetPromiseFromDragManager(ev,
															theRefCon->sourcePart,
															destSUView);
			}
#endif
	
			destSUView->SetOffset(ev, 0);
			delete destSUView;
			ODPart*	sourcePart = theRefCon->sourcePart;
			if (sourcePart)
			{
				ODReleaseObject(ev, sourcePart);
			}
			ODDisposePtr(theRefCon);
			this->DecrementPromiseCount();
		
		CATCH_ALL
		
			if (destSUView)
				delete destSUView;

			fResolvingPromiseInfo = kODNULL;
			if (ErrorCode() != kODErrUnfocusedStorageUnit)
				RERAISE;
			
		ENDTRY
		
		fResolvingPromiseInfo = kODNULL;
	}
}

//------------------------------------------------------------------------------
// PromiseResolver::GetPromiseInfo
//------------------------------------------------------------------------------

ODPtr PromiseResolver::GetPromiseInfo(CMValue curValue)
{
	return CMGetValueRefCon(curValue);
}

//------------------------------------------------------------------------------
// PromiseResolver::SetPromiseInfo
//------------------------------------------------------------------------------

void PromiseResolver::SetPromiseInfo(CMValue curValue, ODPtr theInfo)
{
	CMSetValueRefCon(curValue, theInfo);
}

//------------------------------------------------------------------------------
// PromiseResolver::IncrementPromiseCount
//------------------------------------------------------------------------------

void PromiseResolver::IncrementPromiseCount()
{
	fPromiseCount++;
}

//------------------------------------------------------------------------------
// PromiseResolver::DecrementPromiseCount
//------------------------------------------------------------------------------

void PromiseResolver::DecrementPromiseCount()
{
	fPromiseCount--;
}

//------------------------------------------------------------------------------
// PromiseResolver::GetPromiseCount
//------------------------------------------------------------------------------

ODULong PromiseResolver::GetPromiseCount()
{
	return fPromiseCount;
}

//------------------------------------------------------------------------------
// PromiseResolver::SettingPromise
//------------------------------------------------------------------------------

void PromiseResolver::SettingPromise()
{
	fSettingPromise = kODTrue;
}

//------------------------------------------------------------------------------
// PromiseResolver::DoneSettingPromise
//------------------------------------------------------------------------------

void PromiseResolver::DoneSettingPromise()
{
	fSettingPromise = kODFalse;
}

//------------------------------------------------------------------------------
// PromiseResolver::IsSettingPromise
//------------------------------------------------------------------------------

ODBoolean PromiseResolver::IsSettingPromise()
{
	return fSettingPromise;
}

//------------------------------------------------------------------------------
// PromiseResolver::PromiseResolver
//------------------------------------------------------------------------------
PromiseResolver::PromiseResolver()
{
	fSU = kODNULL;
	fPromiseCount = 0;
	fSettingPromise = kODFalse;
	fResolvingPromiseInfo = kODNULL;
}

//------------------------------------------------------------------------------
// PromiseResolver::~PromiseResolver
//------------------------------------------------------------------------------
PromiseResolver::~PromiseResolver()
{
}

//------------------------------------------------------------------------------
// PromiseResolver::InitPromiseResolver
//------------------------------------------------------------------------------
void PromiseResolver::InitPromiseResolver(CMStorageUnit* su)
{
	fSU = su;
}


