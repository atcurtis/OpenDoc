/* @(#)Z 1.4 com/src/bento/DocPriv.h, odstorage, od96os2, odos29646d 96/11/15 15:29:27 (96/10/29 09:11:55) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   DraftLink
 *		DraftList
 *		DraftListIterator
 *		VersionLink
 *		VersionList
 *		VersionListIterator
 *
 *   ORIGINS: 82,27
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
	File:		DocPriv.h

	Contains:	Private Definition for CMDocument

	Owned by:	Vincent Lo

	Copyright:	© 1992-1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):
	
		<13>	 6/10/95	VL		1252808: ODVersionID is moved from
									ODTypesB.idl.
		<12>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		<11>	 3/31/95	EL		1234685: ExportTo has extra parameter so we
									can skip top draft. Dirty flag no longer
									needed because version list only
									externalized on close or flush.
		<10>	 3/24/95	EL		1209355: dirty flag for version list.
									#define TestFlushContainer before making
									this change permanent.
		 <9>	 3/23/95	VL		1228003: Added Print.
		 <8>	  3/9/95	VL		1226089: Introduced fHighestDraftID to
									prevent DraftID from being reused in a
									session.
		 <7>	 2/15/95	EL		1182275: Add GetSameVersionDraftID to get
									the lowest draft that has the same version
									as the latest draft.
		 <6>	 1/11/95	VL		1185688: Made storage more robust in terms
									of error handling. Also did some code
									review cleanup.
		 <5>	 8/26/94	VL		1150214: Added Count.
		 <4>	 7/11/94	VL		Added Exists calls.
		 <3>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <2>	 6/15/94	RR		ODHeap -> ODMemoryHeap
		 <1>	 5/27/94	VL		first checked in

	To Do:
	In Progress:
*/


#ifndef _DOCPRIV_
#define _DOCPRIV_

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

//==============================================================================
// Types
//==============================================================================
typedef	ODULong	ODVersionID;

//==============================================================================
// Constants
//==============================================================================
const	ODVersionID	kODTombstonedVersion = 0;
#define TestFlushContainer 1

//==============================================================================
// Classes defined in this interface
//==============================================================================
class DraftLink;
class DraftList;
class VersionList;
class DraftListIterator;
class VersionListIterator;
class CMDraft;

//==============================================================================
// Classes used by this interface
//==============================================================================

//==============================================================================
// DraftLink
//==============================================================================

class DraftLink : public Link {

public:
	
	DraftLink(ODDraftID id, CMDraft* draft)
	{
		fDraftID = id;
		fDraft = draft;
	}
	ODVMethod ~DraftLink() {;};
	
	ODDraftID GetDraftID() {return fDraftID;};
	CMDraft*	GetDraft() {return fDraft;};

private:
	
	ODDraftID				fDraftID;
	CMDraft*				fDraft;
};

//==============================================================================
// DraftList
//==============================================================================

class DraftList
{	
public:

	DraftList();
	
	ODVMethod	~DraftList();
	
	ODMethod void Initialize();
	
	ODMethod void Add(ODDraftID draftID, CMDraft* draft);
	
	ODMethod void Remove(ODDraftID draftID);
	
	ODMethod CMDraft* Get(ODDraftID draftID);
	
	ODMethod ODULong Count();
	
private:

	ODMethod DraftLink* GetLink(ODDraftID draftID);
	

	LinkedList* fLinkedList;
	
	ODMemoryHeapID		fHeap;
	
public:	// private by convention

	ODMethod	ODMemoryHeapID		GetHeap();
	ODMethod	LinkedList* GetLinkedList();
};

//==============================================================================
// DraftListIterator
//==============================================================================

class DraftListIterator
{

public:

	DraftListIterator(DraftList* DraftList);
	
	~DraftListIterator();
	
	ODMethod	void		Initialize();
	
	ODMethod	CMDraft*	Last();
	ODMethod	CMDraft*	Previous();
	ODMethod	ODBoolean	IsNotComplete();
	
private:

	DraftList*			fDraftList;
	LinkedListIterator* fIterator;
};

//==============================================================================
// VersionLink
//==============================================================================

class VersionLink : public Link {

public:
	
	VersionLink(ODDraftID draftID, ODVersionID versionID);
	ODVMethod ~VersionLink() {;};
	
	ODDraftID		GetDraftID() {return fDraftID;};
	ODVersionID	GetVersionID() {return fVersionID;};
	void			SetVersionID(ODVersionID versionID) {fVersionID = versionID;};

private:

	ODDraftID				fDraftID;
	ODVersionID			fVersionID;
};

	
//==============================================================================
// VersionList
//==============================================================================

class VersionList {
	
public:

	VersionList();
	ODVMethod	~VersionList();
	
	ODMethod	void Initialize();
	ODMethod	void Initialize(ODPtr buffer, ODULong size);
	ODMethod	void ExportTo(ODPtr* buffer, ODULong* size, ODBoolean ignoreTopDraft);
	ODMethod	void Reinitialize(ODPtr buffer, ODULong size);
	
	ODMethod 	ODDraftID CreateDraft();
	ODMethod	void RemoveDraft(ODDraftID draftID);
	
	ODMethod	ODVersionID	GetDraft(ODDraftID draftID);
	
	ODMethod	ODVersionID	CreateVersion(ODDraftID draftID);
	ODMethod	ODVersionID	GetCurrentVersion(ODDraftID draftID);
	
	ODMethod	ODBoolean Exists(ODDraftID draftID);
	
	ODMethod	void 			CollapseDrafts(ODDraftID fromDraftID, ODDraftID toDraftID);
	ODMethod	void 			SaveToAPrevDraft(ODDraftID fromDraftID, ODDraftID toDraftID);
	ODMethod	ODVersionID	RemoveChanges(ODDraftID draftID);
	ODMethod	ODBoolean		ChangedFromPrev(ODDraftID draftID);

	ODMethod	ODDraftID GetBaseDraftID();
	ODMethod	ODDraftID GetLatestDraftID();
	ODMethod	ODDraftID GetPreviousDraftID(ODDraftID draftID);
	ODMethod	ODDraftID GetNextDraftID(ODDraftID draftID);
	ODMethod	ODDraftID GetSameVersionDraftID(ODDraftID draftID);

	ODMethod	ODBoolean PreviousDraftExists(ODDraftID draftID);
	ODMethod	ODBoolean NextDraftExists(ODDraftID draftID);
	
	ODMethod	ODBoolean	IsAbove(ODDraftID draftID1, ODDraftID draftID2);
	ODMethod	ODBoolean	IsBelow(ODDraftID draftID1, ODDraftID draftID2);

	ODMethod	void Print(char* string);
		
private:

	ODMethod	VersionLink* GetLink(ODDraftID draftID);
	ODMethod	void Clear();
	
	LinkedList*		fLinkedList;
	ODMemoryHeapID	fHeap;
	ODVersionID		fLatestVersionID;
	ODDraftID		fHighestDraftID;
	
public: // private by convention

	ODMethod	ODMemoryHeapID		GetHeap();
	ODMethod	LinkedList*	GetLinkedList();
};


//==============================================================================
// VersionListIterator
//==============================================================================

class VersionListIterator
{

public:

	VersionListIterator(VersionList* versionList);
	
	ODVMethod	~VersionListIterator();
	
	ODMethod	void	Initialize();
	
	ODMethod	VersionLink*	Last();
	ODMethod	VersionLink*	Previous();
	ODMethod	ODBoolean		IsNotComplete();
	
private:

	VersionList*		fVersionList;
	LinkedListIterator* fIterator;
};

#endif	// _DOCPRIV_
