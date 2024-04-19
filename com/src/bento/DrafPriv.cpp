/* @(#)Z 1.12 com/src/bento/DrafPriv.cpp, odstorage, od96os2, odos29646d 96/11/15 15:29:28 (96/10/29 09:12:01) */
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
	File:		DrafPriv.cpp

	Contains:	Private classes for Draft.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<18>	10/19/95	EL		1292685: Check for boolean result from hash
									manager calls.
		<17>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		<16>	 6/16/95	VL		1244940: Removed fEV from PreserveFocus
									class.
		<15>	 6/13/95	VL		1241352: Moved PreserveFocus from
									CMDraft.cpp.
		<14>	 5/26/95	VL		1251403: Multithreading naming support.
		<13>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<12>	 4/25/95	DM		1172791: Replace AEHashTable with
									OpenHashTable
		<11>	 4/25/95	VL		1210982: Removed 5$.
		<10>	 3/23/95	VL		1230371: Changed ASSERT to WARN in Remove.
		 <9>	 3/10/95	VL		1227218: Get rid of system heap.
		 <8>	  2/3/95	VL		1216535: Implemented storage support for
									non-persistent frames.
		 <7>	  9/5/94	VL		1184871: Remove dependency on default heap
									by calling ODRecoverHeapID.
		 <6>	  7/7/94	VL		Commented out use of ODRecoverHeapID.
		 <5>	 6/28/94	VL		ODRecoverHeap to ODRecoverHeapID.
		 <4>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <3>	 6/15/94	RR		ODHeap -> ODMemoryHeap
		 <2>	 5/27/94	VL		Changed ODAEHashTable to AEHashTable.
		 <1>	 5/27/94	VL		first checked in

	To Do:
	In Progress:
*/


#ifndef _DRAFPRIV_
#include  "DrafPriv.h"
#endif

#ifndef _OPENHASH_
#include "OpenHash.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDebug_
#include "ODDebug.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef SOM_ODStorageUnitCursor_xh
#include <SUCursor.xh>
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
#ifndef SOM_CMStorageUnit_xh
#include <CMSU.xh>
#endif

#ifndef _ODUTILS
#include <ODUtils.h>
#endif
#endif

#ifndef _BENTOERR_
#include "BentoErr.h"
#endif

#pragma segment DrafPriv

//==============================================================================
// Constants
//==============================================================================
const ODULong	kODInitialNumEntries = 8;


//==============================================================================
// IDList
//==============================================================================

//------------------------------------------------------------------------------
// IDList::IDList
//------------------------------------------------------------------------------

IDList::IDList()
{
	fIDToObj = kODNULL;
	fObjToID = kODNULL;
	fCurrentID = 0;
	fHeap = kODNULL;
}

//------------------------------------------------------------------------------
// IDList::~IDList
//------------------------------------------------------------------------------

IDList::~IDList()
{
	delete fIDToObj;
	delete fObjToID;
}

//------------------------------------------------------------------------------
// IDList::Initialize
//------------------------------------------------------------------------------

void IDList::Initialize()
{
	fHeap = ODRecoverHeapID(this);

	ODMemoryHeapID heap = GetHeap();
#ifdef _PLATFORM_MACINTOSH_
	fIDToObj = new(heap) 
		OpenHashTable(OpenHashTable::StdEqual,
		              OpenHashTable::StdHash, heap);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fIDToObj = new OpenHashTable(OpenHashTable::StdEqual,
		              OpenHashTable::StdHash, 0);
#endif
	fIDToObj->Initialize(kODInitialNumEntries,
	                     sizeof(ODID),
	                     sizeof(void*));
					
#ifdef _PLATFORM_MACINTOSH_
	fObjToID = new(heap) 
		OpenHashTable(OpenHashTable::StdEqual,
		              OpenHashTable::StdHash, heap);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fObjToID = new OpenHashTable(OpenHashTable::StdEqual,
		              OpenHashTable::StdHash, 0);
#endif
	fObjToID->Initialize(kODInitialNumEntries,
	                     sizeof(void*),
	                     sizeof(ODID));
}

//------------------------------------------------------------------------------
// IDList::Add
//------------------------------------------------------------------------------

void IDList::Add(ODID id, void* object)
{
	if (id > fCurrentID)
		fCurrentID = id;

	if (fIDToObj->ReplaceEntry(&id, &object) != kODFalse)
		fObjToID->ReplaceEntry(&object, &id);
}

//------------------------------------------------------------------------------
// IDList::Add
//------------------------------------------------------------------------------

ODID IDList::Add(void* object)
{
	ODID	id = 0;
	
	if ((object != kODNULL) && (this->ObjectExists(object) != kODFalse))
		id = this->GetID(object);
	else {
		fCurrentID++;
		id = fCurrentID;
		if (fIDToObj->ReplaceEntry(&id, &object) != kODFalse)
			fObjToID->ReplaceEntry(&object, &id);
	}
	return id;
}

//------------------------------------------------------------------------------
// IDList::Remove
//------------------------------------------------------------------------------

void IDList::Remove(ODID id)
{
	void*	object;
	
	if (fIDToObj->GetValue(&id, &object)) {
		fIDToObj->RemoveEntry(&id);
		fObjToID->RemoveEntry(&object);
	}
	else
	        WARNMSG_DEBUG(WARN_INDEX(-1),"Object removed already: %d", id);
}

//------------------------------------------------------------------------------
// IDList::Get
//------------------------------------------------------------------------------

void* IDList::Get(ODID id)
{
	void*	object = kODNULL;
	
	fIDToObj->GetValue(&id, &object);
	return object;
}

//------------------------------------------------------------------------------
// IDList::GetID
//------------------------------------------------------------------------------

ODID IDList::GetID(void* object)
{
	ODID	id = 0;

	fObjToID->GetValue(&object, &id);
	return id;
}

//------------------------------------------------------------------------------
// IDList::Exists
//------------------------------------------------------------------------------

ODBoolean IDList::Exists(ODID id)
{
        return (fIDToObj->Exists(&id) ? kODTrue : kODFalse);

}

//------------------------------------------------------------------------------
// IDList::ObjectExists
//------------------------------------------------------------------------------

ODBoolean IDList::ObjectExists(void* object)
{
        return (fObjToID->Exists(&object) ? kODTrue : kODFalse);
}


ODMemoryHeapID IDList::GetHeap()
{
	return fHeap;
}

//------------------------------------------------------------------------------
// PreserveFocus::PreserveFocus
//------------------------------------------------------------------------------

PreserveFocus::PreserveFocus(Environment* ev, ODStorageUnit* su)
{
	ASSERT(su, kODErrInvalidStorageUnit);
	fSUCursor = kODNULL;
	fSU = kODNULL;
	try
        {
		su->Acquire(ev);
		fSU = su;
		fSUCursor = fSU->CreateCursorWithFocus(ev);
        }
	catch (ODException _exception)
        {
		if (fSU != kODNULL)
		{
                	SaveAndRestoreEv1(fSU);
			fSU = kODNULL;
		}
		throw;
        }
}

//------------------------------------------------------------------------------
// PreserveFocus::~PreserveFocus
//------------------------------------------------------------------------------

PreserveFocus::~PreserveFocus()
{
        Environment* ev = somGetGlobalEnvironment();
	if (fSUCursor != kODNULL) {
		if (fSU->ExistsWithCursor(ev, fSUCursor))
			fSU->FocusWithCursor(ev, fSUCursor);
		delete fSUCursor;
	}
	ODReleaseObject(ev, fSU);
}
