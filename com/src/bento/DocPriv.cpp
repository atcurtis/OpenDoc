/* @(#)Z 1.9 com/src/bento/DocPriv.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:28 (96/10/29 09:11:49) */
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
	File:		DocPriv.cpp

	Contains:	Implementation for Private class for CMDocument.

	Owned by:	Vincent Lo

	Copyright:	© 1993-1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<21>	 8/21/95	VL		1278330, 1278315: Error code cleanup.
		<20>	 5/26/95	VL		1251403: Multithreading naming support.
		<19>	 5/25/95	jpa		List.h --> LinkList.h [1253324] Fixed
									misuse of ODDebug [1253321]
		<18>	 4/14/95	EL		1239463: Remove Change should not reduce
									version id to 0.
		<17>	 3/31/95	EL		1234685: ExportTo has extra parameter so we
									can skip top draft. Dirty flag no longer
									needed because version list only
									externalized on close or flush.
		<16>	 3/29/95	DM		make standard format little endian
		<15>	 3/24/95	EL		1209355: dirty flag for version list. Temp
									fix: remove duplicate drafts when version
									list is read.
		<14>	 3/23/95	VL		1228003: Added Print.
		<13>	  3/9/95	VL		1226089: Introduced fHighestDraftID to
									prevent DraftID from being reused in a
									session.
		<12>	 2/15/95	EL		1182275: Add GetSameVersionDraftID to get
									the lowest draft that has the same version
									as the latest draft.
		<11>	 1/31/95	EL		1213321: Do not output tomstoned versions
									during externalization.
		<10>	 1/11/95	VL		1185688: Made storage more robust in terms
									of error handling. Also did some code
									review cleanup.
		 <9>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <8>	  9/5/94	VL		1184871: Remove dependency on default heap
									by calling ODRecoverHeapID>
		 <7>	 8/26/94	VL		1183174: Use updated GetSTorageUnitRef
									which takes ID.
		 <6>	 7/11/94	VL		Added Exists calls.
		 <5>	  7/7/94	VL		Commented out use of ODRecoverHeapID.
		 <4>	 6/28/94	VL		ODRecoverHeap to ODRecoverHeapID.
		 <3>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <2>	 6/15/94	RR		ODHeap -> ODMemoryHeap
		 <1>	 5/27/94	VL		first checked in

	To Do:
		1) Get rid of tombstoned versions.
	In Progress:
*/

#ifndef _DOCPRIV_
#include "DocPriv.h"
#endif

#ifndef _FLIPEND_
#include "FlipEnd.h"
#endif

#ifndef SOM_CMDocument_xh
#include "CMDoc.xh"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef SOM_CMDraft_xh
#include "CMDraft.xh"
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif


#ifndef som_xh
#include "som.xh"
#endif

#pragma segment Document

//==============================================================================
// Scalar Types
//==============================================================================

#if ODDebug
// #define DebugStorage 1
#endif

#ifdef DebugStorage
#define MyDebug3Str(f,p1,p2,p3) somPrintf(f, p1, p2, p3)
#else
#define MyDebug3Str(f,p1,p2,p3)
#endif

typedef struct {
	ODDraftID		draftID;
	ODVersionID	versionID;
} VersionRecord;

#ifdef STORAGE_TEST
inline void BREAK_AND_THROW(ODError e)
{
	Debugger();
	THROW(e);
}
#else

#define BREAK_AND_THROW(e)	THROW(e)

#endif


//==============================================================================
// DraftList
//==============================================================================

//------------------------------------------------------------------------------
// DraftList::DraftList
//------------------------------------------------------------------------------

DraftList::DraftList()
{
	fHeap = kODNULL;
}

//------------------------------------------------------------------------------
// DraftList::~DraftList
//------------------------------------------------------------------------------

DraftList::~DraftList()
{
	Link*	link;
	
	link = fLinkedList->RemoveFirst();
	while (link != kODNULL) {
		delete link;
		link = fLinkedList->RemoveFirst();
	}
	delete fLinkedList;
}

//------------------------------------------------------------------------------
// DraftList::Initialize
//------------------------------------------------------------------------------

void DraftList::Initialize()
{
	fHeap = ODRecoverHeapID(this);
	
#ifdef _PLATFORM_MACINTOSH_
	fLinkedList = new(GetHeap()) LinkedList;
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fLinkedList = new LinkedList;
#endif
}


//------------------------------------------------------------------------------
// DraftList::Get
//------------------------------------------------------------------------------

CMDraft* DraftList::Get(ODDraftID draftID)
{	
  
   CMDraft* retDraft = kODNULL;
   DraftLink* link = this->GetLink(draftID);
	
   if (link != kODNULL)
      retDraft =  link->GetDraft();

   return retDraft;
}

//------------------------------------------------------------------------------
// DraftList::Add
//------------------------------------------------------------------------------

void DraftList::Add(ODDraftID draftID, CMDraft* draft)
{	
        Environment* ev = somGetGlobalEnvironment();
	CMDraft* existingDraft = kODNULL;
	DraftLink* link  = kODNULL;
	
  try
  {
	if ((existingDraft = this->Get(draftID)) == kODNULL) {
#ifdef _PLATFORM_MACINTOSH_
		link = new(GetHeap()) DraftLink(draftID, draft);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		link = new DraftLink(draftID, draft);
#endif
		fLinkedList->AddLast(link);
	}
	else {
		if (draftID != existingDraft->GetID(ev))
			BREAK_AND_THROW(kODErrDraftExistsWithDifferentID);
	}
  }
  catch (ODException _exception)
  {
      ODDeleteObject(link);
      throw;
  }
}
		
//------------------------------------------------------------------------------
// DraftList::Remove
//------------------------------------------------------------------------------

void DraftList::Remove(ODDraftID draftID)
{

	DraftLink* link = this->GetLink(draftID);
	
	if (link == kODNULL)
		BREAK_AND_THROW(kODErrDraftDoesNotExist);
	else {
		fLinkedList->Remove(*link);
		delete link;
	}

}

//------------------------------------------------------------------------------
// DraftList::Count
//------------------------------------------------------------------------------

ODULong DraftList::Count()
{
	return fLinkedList->Count();
}

//------------------------------------------------------------------------------
// DraftList::GetLink
//------------------------------------------------------------------------------

DraftLink* DraftList::GetLink(ODDraftID draftID)
{
	LinkedListIterator	iter(fLinkedList);
	
	DraftLink *link = (DraftLink*) iter.Last();
	while ((link != kODNULL) && (link->GetDraftID() != draftID)) {
		link = (DraftLink*) iter.Previous();
	}
	return link;
}

//------------------------------------------------------------------------------
// DraftList::GetHeap
//------------------------------------------------------------------------------

ODMemoryHeapID DraftList::GetHeap()
{
	return fHeap;
}

//------------------------------------------------------------------------------
// DraftList::GetLinkedList
//------------------------------------------------------------------------------

LinkedList* DraftList::GetLinkedList()
{
	return fLinkedList;
}

//==============================================================================
// DraftListIterator
//==============================================================================

//------------------------------------------------------------------------------
// DraftListIterator::DraftListIterator
//------------------------------------------------------------------------------

DraftListIterator::DraftListIterator(DraftList* draftList)
{
	fDraftList = draftList;
}
	
//------------------------------------------------------------------------------
// DraftListIterator::~DraftListIterator
//------------------------------------------------------------------------------

DraftListIterator::~DraftListIterator()
{
	delete fIterator;
}

//------------------------------------------------------------------------------
// DraftListIterator::Initialize
//------------------------------------------------------------------------------

void DraftListIterator::Initialize()
{
#ifdef _PLATFORM_MACINTOSH_
	fIterator = new(fDraftList->GetHeap()) LinkedListIterator(fDraftList->GetLinkedList());
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fIterator = new LinkedListIterator(fDraftList->GetLinkedList());
#endif
}
	

//------------------------------------------------------------------------------
// DraftListIterator::Last
//------------------------------------------------------------------------------

CMDraft* DraftListIterator::Last()
{
   CMDraft* retDraft = kODNULL;
   DraftLink* link  = (DraftLink*) fIterator->Last();
   if (link != kODNULL)
      retDraft =  link->GetDraft();
   return retDraft;
}

//------------------------------------------------------------------------------
// DraftListIterator::Previous
//------------------------------------------------------------------------------

CMDraft* DraftListIterator::Previous()
{
   CMDraft* retDraft = kODNULL;
   DraftLink* link = (DraftLink*) fIterator->Previous();
   if (link != kODNULL)
      retDraft =  link->GetDraft();
   return retDraft;
}
		
//------------------------------------------------------------------------------
// DraftListIterator::IsNotComplete
//------------------------------------------------------------------------------

ODBoolean DraftListIterator::IsNotComplete()
{
   return (fIterator->IsNotComplete());
   
}

//==============================================================================
// VersionLink
//==============================================================================

//------------------------------------------------------------------------------
// VersionLink::VersionLink
//------------------------------------------------------------------------------

VersionLink::VersionLink(ODDraftID draftID, ODVersionID versionID)
	: Link()
{
	fDraftID = draftID;
	fVersionID = versionID;
}

//==============================================================================
// VersionList
//==============================================================================

//------------------------------------------------------------------------------
// VersionList::VersionList
//------------------------------------------------------------------------------

VersionList::VersionList()
{
	fHighestDraftID = 0;
	fLatestVersionID = 0;
	fLinkedList = kODNULL;
	fHeap = kODNULL;
}

//------------------------------------------------------------------------------
// VersionList::~VersionList
//------------------------------------------------------------------------------

VersionList::~VersionList()
{
	this->Clear();
	delete fLinkedList;
}

//------------------------------------------------------------------------------
// VersionList::Initialize
//------------------------------------------------------------------------------

void VersionList::Initialize()
{
	fHeap = ODRecoverHeapID(this);

#ifdef _PLATFORM_MACINTOSH_
	fLinkedList = new(GetHeap()) LinkedList;
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fLinkedList = new LinkedList;
#endif
}

//------------------------------------------------------------------------------
// VersionList::Initialize
//------------------------------------------------------------------------------

void VersionList::Initialize(ODPtr buffer, ODULong size)
{
	VersionRecord*	versionList;
	ODULong		numVersionRecords;
	ODULong		i;
	VersionLink*	link  = kODNULL;

      Environment* ev = somGetGlobalEnvironment();
  try
  {
	// Note that I am assuming that VersionRecord contains just ODULong's
	// without any additional padding:
	ConvertODULongArrayFromStd((ODULong*) buffer, size / sizeof(ODULong));
	
	ASSERTMSG(size>0, kODErrIllegalNullInput, 
		"VersionList is empty.", 0);
	
	fLatestVersionID = *((ODULong*) buffer);
	
	versionList = (VersionRecord*) (((ODSByte*) buffer) + sizeof(ODULong));
	numVersionRecords = (size - sizeof(ODULong)) / sizeof(VersionRecord);
	
#ifdef _PLATFORM_MACINTOSH_
	fLinkedList = new(GetHeap()) LinkedList;
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fLinkedList = new LinkedList;
#endif
	
	ODVersionID		prevVersionID = kODTombstonedVersion;
	
	for (i = 0; i < numVersionRecords; i++) {
		MyDebug3Str("Import %x: DraftID %d VersionID %d\n", this, versionList[i].draftID, versionList[i].versionID);
#if TestFlushContainer
		if (versionList[i].versionID != prevVersionID) {
#endif
		prevVersionID = versionList[i].versionID;
#ifdef _PLATFORM_MACINTOSH_
		link = new(GetHeap()) VersionLink(versionList[i].draftID, prevVersionID);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		link = new VersionLink(versionList[i].draftID, prevVersionID);
#endif
		fLinkedList->AddLast(link);
#if TestFlushContainer
		}
#endif
	}
  }
  catch (ODException _exception)
  {
       ODDeleteObject(fLinkedList);
       ODDeleteObject(link);
       throw;
  }
}

//------------------------------------------------------------------------------
// VersionList::ExportTo
//------------------------------------------------------------------------------

void VersionList::ExportTo(ODPtr* buffer, ODULong* size, ODBoolean ignoreTopDraft)
{
      Environment* ev = somGetGlobalEnvironment();
  try
  {
	LinkedListIterator	iter(fLinkedList);
	ODULong			count = 0;
	ODULong			i = 0;
	VersionRecord*		versionList;
	ODVersionID		thisVersionID;
	
        *buffer = kODNULL;

	Link*	link = iter.First();
	while (link != kODNULL) {
		thisVersionID = ((VersionLink*) link)->GetVersionID();
		// Ignore tombstone
		if (thisVersionID != kODTombstonedVersion)
			count++;
		link = iter.Next();
	}
	
	*size = sizeof(ODULong) + count * sizeof(VersionRecord);
	*buffer = ODNewPtr(*size, GetHeap());

	if (ignoreTopDraft != kODFalse)
		*size -= sizeof(VersionRecord);

	*((ODULong*) *buffer) = ConvertODULongToStd(fLatestVersionID);
	
	versionList = (VersionRecord*) (((ODSByte*) *buffer) + sizeof(ODULong));
	
	link = iter.First();
	while (link != kODNULL) {
		thisVersionID = ((VersionLink*) link)->GetVersionID();
		// Ignore tombstone
		if (thisVersionID != kODTombstonedVersion) {
			versionList[i].draftID = ConvertODULongToStd(((VersionLink*) link)->GetDraftID());
			versionList[i].versionID = ConvertODULongToStd(thisVersionID);
			MyDebug3Str("Export %x: DraftID %d VersionID %d\n", this, versionList[i].draftID, versionList[i].versionID);
			i++;
		}
		link = iter.Next();
	}
   }
   catch (ODException _exception)
   {
        ODDisposePtr(*buffer);
        throw;
   }
}

//------------------------------------------------------------------------------
// VersionList::Reinitialize
//------------------------------------------------------------------------------

void VersionList::Reinitialize(ODPtr buffer, ODULong size)
{
	this->Clear();
	this->Initialize(buffer, size);
}

//------------------------------------------------------------------------------
// VersionList::CreateDraft
//------------------------------------------------------------------------------

ODDraftID VersionList::CreateDraft()
{
      Environment* ev = somGetGlobalEnvironment();
      VersionLink* link = kODNULL;
  try
  {

	ODVersionID	versionID;
	
	VersionLink* prevLink = (VersionLink*) fLinkedList->Last();
	if (prevLink == kODNULL) {
		fHighestDraftID = 1;
		versionID = 0;
	}
	else {
		ODDraftID draftID = prevLink->GetDraftID();
		if (draftID > fHighestDraftID)
			fHighestDraftID = draftID;
		fHighestDraftID++;
		ODDraftID prevDraftID = this->GetLatestDraftID();
		versionID = this->GetDraft(prevDraftID);
	}
	
#ifdef _PLATFORM_MACINTOSH_
	VersionLink* link = new(GetHeap()) VersionLink(fHighestDraftID, versionID);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	VersionLink* link = new VersionLink(fHighestDraftID, versionID);
#endif
	fLinkedList->AddLast(link);
   }
   catch (ODException _exception)
   {
        ODDeleteObject(link);
        throw;
   }
		
	return fHighestDraftID;
}

//------------------------------------------------------------------------------
// VersionList::RemoveDraft
//------------------------------------------------------------------------------

void VersionList::RemoveDraft(ODDraftID draftID)
{
	VersionLink* link = this->GetLink(draftID);
	
	if (link == kODNULL)
		BREAK_AND_THROW(kODErrDraftDoesNotExist);
	else {
		fLinkedList->Remove(*link);
		delete link;
	}
}

//------------------------------------------------------------------------------
// VersionList::GetDraft
//------------------------------------------------------------------------------

ODVersionID VersionList::GetDraft(ODDraftID draftID)
{
	ODVersionID verID = kODTombstonedVersion;

        VersionLink*  link   = this->GetLink(draftID);
	
        if (link)
            verID = link->GetVersionID();

        return verID;
}

//------------------------------------------------------------------------------
// VersionList::CreateVersion
//------------------------------------------------------------------------------

ODVersionID VersionList::CreateVersion(ODDraftID draftID)
{
	VersionLink*	link;
	ODDraftID		latestDraftID = this->GetLatestDraftID();
	
	if (latestDraftID != draftID)
		BREAK_AND_THROW(kODErrIllegalNonTopmostDraft);
	
	link = this->GetLink(draftID);
	if (link == kODNULL)
		BREAK_AND_THROW(kODErrDraftDoesNotExist);

	fLatestVersionID++;
	
	link->SetVersionID(fLatestVersionID);
	
	return fLatestVersionID;
}

//------------------------------------------------------------------------------
// VersionList::GetCurrentVersion
//------------------------------------------------------------------------------

ODVersionID VersionList::GetCurrentVersion(ODDraftID draftID)
{
        ODVersionID VerID = kODTombstonedVersion;
	VersionLink*	link = this->GetLink(draftID);
	
	if (link == kODNULL)
		BREAK_AND_THROW(kODErrDraftDoesNotExist);
			
	VerID =  link->GetVersionID();
        return VerID;
}

//------------------------------------------------------------------------------
// VersionList::Exists
//------------------------------------------------------------------------------

ODBoolean VersionList::Exists(ODDraftID draftID)
{
        ODBoolean exists = kODFalse; 
	VersionLink*	link = this->GetLink(draftID);
	
	if ((link == kODNULL) || (link->GetVersionID() == kODTombstonedVersion))
		exists =  kODFalse;
	else
		exists =  kODTrue;
        return exists;
}


//------------------------------------------------------------------------------
// VersionList::CollapseDrafts
//------------------------------------------------------------------------------

void VersionList::CollapseDrafts(ODDraftID fromID, ODDraftID toID)
{
      VersionListIterator*	iter  = kODNULL;
  try
  {
	ODVersionID	fromVersionID;
	VersionLink*	versionLink;
	ODVersionID	prevVersionID;

	// Check the Draft Topology
	
#ifdef _PLATFORM_MACINTOSH_
                         iter   = new(GetHeap()) VersionListIterator(this);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	                iter = new VersionListIterator(this);
#endif
	iter->Initialize();
	
	versionLink = iter->Last();
	while ((versionLink != kODNULL) && (versionLink->GetDraftID() != fromID)) {
		versionLink = iter->Previous();
	}
	fromVersionID = this->GetDraft(fromID);
	versionLink = iter->Previous();
	while ((versionLink != kODNULL) && (versionLink->GetDraftID() >= toID)) {
		prevVersionID = versionLink->GetVersionID();
		if ((prevVersionID != kODTombstonedVersion) && (fromVersionID != prevVersionID))
			BREAK_AND_THROW(kODErrNonEmptyDraft);
		versionLink = iter->Previous();
	}

	// Do the actual updating
	
	versionLink = iter->Last();
	while ((versionLink != kODNULL) && (versionLink->GetDraftID() != fromID)) {
		versionLink = iter->Previous();
	}	
	while ((versionLink != kODNULL) && (versionLink->GetDraftID() > toID)) {
		versionLink->SetVersionID(kODTombstonedVersion);
		versionLink = iter->Previous();
	}
	delete iter;
   }
   catch (ODException _exception)
   {
       ODDeleteObject(iter);
       throw;
   }
}

//------------------------------------------------------------------------------
// VersionList::SaveToAPrevDraft
//------------------------------------------------------------------------------

void VersionList::SaveToAPrevDraft(ODDraftID fromID, ODDraftID toID)
{
      VersionListIterator*	iter = kODNULL;
  try
  {
	ODVersionID	fromVersionID;
	VersionLink*	versionLink;
	
	fromVersionID = this->GetDraft(fromID);
	
	// Find the from draft
	
#ifdef _PLATFORM_MACINTOSH_
                       iter    = new(GetHeap()) VersionListIterator(this);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	        	iter = new VersionListIterator(this);
#endif
	iter->Initialize();
	versionLink = iter->Last();
	while ((versionLink != kODNULL) && (versionLink->GetDraftID() != fromID)) {
		versionLink = iter->Previous();
	}
	
	// Do the actual update of the Version List
	
	versionLink = iter->Previous();
	while ((versionLink != kODNULL) && (versionLink->GetDraftID() >= toID)) {
		if (versionLink->GetVersionID() != kODTombstonedVersion)
			versionLink->SetVersionID(fromVersionID);
		versionLink = iter->Previous();
	}
	
	delete iter;
   }
   catch (ODException _exception)
   {
       ODDeleteObject(iter);
       throw;
   }
}

//------------------------------------------------------------------------------
// VersionList::RemoveChanges
//------------------------------------------------------------------------------

ODVersionID VersionList::RemoveChanges(ODDraftID draftID)
{
        ODVersionID	prevVersionID;
	ODDraftID	prevDraftID;
	VersionLink*	link = kODNULL;
		
	link = this->GetLink(draftID);
	if (link == kODNULL)
		BREAK_AND_THROW(kODErrDraftDoesNotExist);

	if (draftID > 1) {
		prevDraftID = this->GetPreviousDraftID(draftID);
		prevVersionID = this->GetDraft(prevDraftID);
	}
	else {
#if TestFlushContainer
		prevVersionID = link->GetVersionID() - 1;
		if (prevVersionID == 0)
			prevVersionID = 1;
		else
			fLatestVersionID--;
#else
		prevVersionID = 0;
#endif
	}
	
	link->SetVersionID(prevVersionID);

	return prevVersionID;
}

//------------------------------------------------------------------------------
// VersionList::ChangedFromPrev
//------------------------------------------------------------------------------

ODBoolean VersionList::ChangedFromPrev(ODDraftID draftID)
{
        ODBoolean changedfromprev = kODFalse;
	ODDraftID		prevDraftID;
	ODVersionID	versionID;
	ODVersionID	prevVersionID;
	ODDraftID		baseDraftID = this->GetBaseDraftID();

	if (draftID == baseDraftID) 
		changedfromprev =  kODFalse;
        else
        {

	   versionID = this->GetDraft(draftID);

	   prevDraftID = this->GetPreviousDraftID(draftID);
	   prevVersionID = this->GetDraft(prevDraftID);

	   if (versionID == prevVersionID)
		changedfromprev =  kODFalse;
	   else
		changedfromprev =  kODTrue;
        }
       return changedfromprev;
}

//------------------------------------------------------------------------------
// VersionList::GetBaseDraftID
//------------------------------------------------------------------------------

ODDraftID VersionList::GetBaseDraftID()
{
        ODDraftID retDraftID = 1;
	VersionLink* link = (VersionLink*) fLinkedList->First();
	
	if (link != kODNULL)
		retDraftID =  link->GetDraftID();
        return retDraftID;
}

//------------------------------------------------------------------------------
// VersionList::GetLatestDraftID
//------------------------------------------------------------------------------

ODDraftID VersionList::GetLatestDraftID()
{
	ODDraftID  retDraftID = 0;
	VersionLink* 	versionLink;

	versionLink = (VersionLink*) fLinkedList->Last();
	
	while ((versionLink != kODNULL) && 
			(versionLink->GetVersionID() == kODTombstonedVersion)) {
		versionLink = (VersionLink*) fLinkedList->Before(*versionLink);
	}
	
	if (versionLink == kODNULL)
		retDraftID =  this->GetBaseDraftID();
	else
		retDraftID =  versionLink->GetDraftID();
        return retDraftID;
}

//------------------------------------------------------------------------------
// VersionList::GetPreviousDraftID
//------------------------------------------------------------------------------

ODDraftID VersionList::GetPreviousDraftID(ODDraftID curDraftID)
{
	VersionLink*	curVersionLink;
	ODDraftID		baseDraftID = this->GetBaseDraftID();
	VersionLink*	versionLink;
	
	if (baseDraftID == curDraftID)
		BREAK_AND_THROW(kODErrNoPreviousDraft);
	if ((curVersionLink = this->GetLink(curDraftID)) != kODNULL) {
		versionLink = (VersionLink*) fLinkedList->Before(*curVersionLink);
		while (versionLink != kODNULL) {
			if (versionLink->GetVersionID() != kODTombstonedVersion)
				return versionLink->GetDraftID();
			versionLink = (VersionLink*) fLinkedList->Before(*versionLink);
		}
	}
	BREAK_AND_THROW(kODErrDraftDoesNotExist);

        return 0;                       // To avoid unnecessary warning from the compiler.
}

//------------------------------------------------------------------------------
// VersionList::GetSameVersionDraftID
//------------------------------------------------------------------------------

ODDraftID VersionList::GetSameVersionDraftID(ODDraftID curDraftID)
{
	ODDraftID  sameVersionDraftID = 0;

	ODDraftID		baseDraftID = this->GetBaseDraftID(), draftID;
	ODVersionID 	latestVersion;

	sameVersionDraftID = curDraftID;
	latestVersion = this->GetCurrentVersion(sameVersionDraftID);
	
	if (sameVersionDraftID != baseDraftID) {
		draftID = this->GetPreviousDraftID(sameVersionDraftID);
		while (draftID >= baseDraftID) {
			if (latestVersion == this->GetCurrentVersion(draftID)) {
				sameVersionDraftID = draftID;
				if (draftID != baseDraftID)
					draftID = this->GetPreviousDraftID(draftID);
				else
					draftID = 0;
			}
			else
				draftID = 0;
		};
	}
	return sameVersionDraftID;
}

//------------------------------------------------------------------------------
// VersionList::PreviousDraftExists
//------------------------------------------------------------------------------

ODBoolean VersionList::PreviousDraftExists(ODDraftID curDraftID)
{
	ODBoolean prevDraftexists = kODFalse;
	VersionLink*	curVersionLink;
	ODDraftID		baseDraftID = this->GetBaseDraftID();
	VersionLink*	versionLink;
	
	if (baseDraftID == curDraftID)
		prevDraftexists =  kODFalse;
        else{
	  if ((curVersionLink = this->GetLink(curDraftID)) != kODNULL) {
		versionLink = (VersionLink*) fLinkedList->Before(*curVersionLink);
		while (versionLink != kODNULL) {
			if (versionLink->GetVersionID() != kODTombstonedVersion)
				return   kODTrue;
			versionLink = (VersionLink*) fLinkedList->Before(*versionLink);
		}
	  }
       } 
       return prevDraftexists;
}

//------------------------------------------------------------------------------
// VersionList::GetNextDraftID
//------------------------------------------------------------------------------

ODDraftID VersionList::GetNextDraftID(ODDraftID curDraftID)
{
	VersionLink*	curVersionLink;
	VersionLink*	versionLink;
	
	if ((curVersionLink = this->GetLink(curDraftID)) != kODNULL) {
		versionLink = (VersionLink*) fLinkedList->After(*curVersionLink);
		while (versionLink != kODNULL) {
			if (versionLink->GetVersionID() != kODTombstonedVersion)
				return  versionLink->GetDraftID();
			versionLink = (VersionLink*) fLinkedList->After(*versionLink);
		}
	}
	BREAK_AND_THROW(kODErrDraftDoesNotExist);

        return 0;                       // To avoid unnecessary warning from the compiler.
}

//------------------------------------------------------------------------------
// VersionList::NextDraftExists
//------------------------------------------------------------------------------

ODBoolean VersionList::NextDraftExists(ODDraftID curDraftID)
{
	VersionLink*	curVersionLink;
	VersionLink*	versionLink;
	
	if ((curVersionLink = this->GetLink(curDraftID)) != kODNULL) {
		versionLink = (VersionLink*) fLinkedList->After(*curVersionLink);
		while (versionLink != kODNULL) {
			if (versionLink->GetVersionID() != kODTombstonedVersion)
				return  kODTrue;
			versionLink = (VersionLink*) fLinkedList->After(*versionLink);
		}
	}	
        return kODFalse;;
}

//------------------------------------------------------------------------------
// VersionList::IsAbove
//------------------------------------------------------------------------------

ODBoolean VersionList::IsAbove(ODDraftID draftID1, ODDraftID draftID2)
{
	return (draftID1 > draftID2);
}

//------------------------------------------------------------------------------
// VersionList::IsBelow
//------------------------------------------------------------------------------

ODBoolean VersionList::IsBelow(ODDraftID draftID1, ODDraftID draftID2)
{
	return (draftID1 < draftID2);
}

//------------------------------------------------------------------------------
// VersionList::GetLink
//------------------------------------------------------------------------------

VersionLink* VersionList::GetLink(ODDraftID draftID)
{
	VersionLink* link = kODNULL;
	LinkedListIterator	iter(fLinkedList);
	
        link  = (VersionLink*) iter.Last();
	while ((link != kODNULL) && (link->GetDraftID() != draftID)) {
		link = (VersionLink*) iter.Previous();
	}
	return link;
}

//------------------------------------------------------------------------------
// VersionList::Clear
//------------------------------------------------------------------------------

void VersionList::Clear()
{
	Link*	link;
	
	link = fLinkedList->RemoveFirst();
	while (link != kODNULL) {
		delete link;
		link = fLinkedList->RemoveFirst();
	}
}

//------------------------------------------------------------------------------
// VersionList::GetHeap
//------------------------------------------------------------------------------

ODMemoryHeapID VersionList::GetHeap()
{
	return fHeap;
}

//------------------------------------------------------------------------------
// VersionList::GetLinkedList
//------------------------------------------------------------------------------

LinkedList* VersionList::GetLinkedList()
{
	return fLinkedList;
}

//------------------------------------------------------------------------------
// VersionList::Print
//------------------------------------------------------------------------------

void VersionList::Print(char* string)
{
#if ODDebug
	VersionListIterator*	versionList = new VersionListIterator(this);
	versionList->Initialize();
	for (VersionLink* link = versionList->Last(); versionList->IsNotComplete(); link = versionList->Previous()) {
		somPrintf("%s: draftID %d versionID %d\n", string, link->GetDraftID(), link->GetVersionID());
	}
	delete versionList;
#endif
}

//==============================================================================
// VersionListIterator
//==============================================================================

//------------------------------------------------------------------------------
// VersionListIterator::VersionListIterator
//------------------------------------------------------------------------------

VersionListIterator::VersionListIterator(VersionList* versionList)
{
	fVersionList = versionList;
	fIterator = kODNULL;
}
	
//------------------------------------------------------------------------------
// VersionListIterator::~VersionListIterator
//------------------------------------------------------------------------------

VersionListIterator::~VersionListIterator()
{
	delete fIterator;
}

//------------------------------------------------------------------------------
// VersionListIterator::Initialize
//------------------------------------------------------------------------------

void VersionListIterator::Initialize()
{
    
#ifdef _PLATFORM_MACINTOSH_
	fIterator = new(fVersionList->GetHeap()) LinkedListIterator(fVersionList->GetLinkedList());
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fIterator = new LinkedListIterator(fVersionList->GetLinkedList());
#endif
}

//------------------------------------------------------------------------------
// VersionListIterator::Last
//------------------------------------------------------------------------------

VersionLink* VersionListIterator::Last()
{
	return((VersionLink*) fIterator->Last());
}

//------------------------------------------------------------------------------
// VersionListIterator::Previous
//------------------------------------------------------------------------------

VersionLink* VersionListIterator::Previous()
{
	 return((VersionLink*) fIterator->Previous());
}
		
//------------------------------------------------------------------------------
// VersionListIterator::IsNotComplete
//------------------------------------------------------------------------------

ODBoolean VersionListIterator::IsNotComplete()
{
	return(fIterator->IsNotComplete());
}
