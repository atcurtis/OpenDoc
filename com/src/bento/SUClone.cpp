/* @(#)Z 1.8 com/src/bento/SUClone.cpp, odstorage, od96os2, odos29646d 96/11/15 15:29:29 (96/10/29 09:13:25) */
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
	File:		SUClone.cpp

	Contains:	Implementation SUCloneHelper class.

	Owned by:	Vincent Lo

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 5/26/95	VL		1251403: Multithreading naming support.
		 <2>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <1>	 1/26/95	VL		first checked in
	
	To Do:
	In Progress:
		
*/

#ifndef _SUCLONE_
#include "SUClone.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

//==============================================================================
// Local classes
//==============================================================================

class ScopeIDLink : public Link {

public:
	
	ScopeIDLink(ODID id) {fID = id;}
	~ScopeIDLink() {};
	
	ODID GetID() {return fID;};

private:
	
	ODID	fID;
};

class ScopeIDList
{
public:

	ScopeIDList() {fHeap = ODRecoverHeapID(this);}
	
	~ScopeIDList() {fLinkedList.DeleteAllLinks();}
	
	void Reset() {fLinkedList.DeleteAllLinks();}
	
	void Add(ODID id);
	
	ODBoolean Exists(ODID id);
	
private:

	LinkedList fLinkedList;
	ODMemoryHeapID	fHeap;
	
	ODMemoryHeapID	GetHeap() {return fHeap;}
};


//==============================================================================
// SUCloneHelper
//==============================================================================

//------------------------------------------------------------------------------
// SUCloneHelper::SUCloneHelper
//------------------------------------------------------------------------------

SUCloneHelper::SUCloneHelper()
{
	fHeap = ODRecoverHeapID(this);
#ifdef _PLATFORM_MACINTOSH_
	fScopeIDList = new(GetHeap()) ScopeIDList;
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	fScopeIDList = new ScopeIDList;
#endif
	
}	

//------------------------------------------------------------------------------
// SUCloneHelper::SUCloneHelper
//------------------------------------------------------------------------------

SUCloneHelper::~SUCloneHelper()
{
	delete fScopeIDList;
}	

//------------------------------------------------------------------------------
// SUCloneHelper::ShouldClone
//------------------------------------------------------------------------------

ODBoolean SUCloneHelper::ShouldClone(ODDraftKey key, ODID scopeID)
{
	ODBoolean shouldClone = kODFalse;
       
	if (fKey != key) {
		fScopeIDList->Reset();
		fScopeIDList->Add(scopeID);
		fKey = key;
		shouldClone = kODTrue;
	}
	else if (fScopeIDList->Exists(scopeID) == kODFalse) {
		fScopeIDList->Add(scopeID);
		shouldClone = kODTrue;
	}
	return shouldClone;
}

//==============================================================================
// ScopeIDList
//==============================================================================

//------------------------------------------------------------------------------
// ScopeIDList::Add
//------------------------------------------------------------------------------

void ScopeIDList::Add(ODID id)
{
        ScopeIDLink* link = kODNULL;
    try
    {
#ifdef _PLATFORM_MACINTOSH_
	link = new(GetHeap()) ScopeIDLink(id);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	link = new ScopeIDLink(id);
#endif
	fLinkedList.AddFirst(link);
    }
    catch (ODException _exception)
    {
          if(link){
             delete(link);
             link = kODNULL;
          }
          throw;
    }
}

//------------------------------------------------------------------------------
// ScopeIDList::Exists
//------------------------------------------------------------------------------

ODBoolean ScopeIDList::Exists(ODID id)
{
	ODBoolean	exists = kODFalse;
        Environment* ev = somGetGlobalEnvironment();
	
	LinkedListIterator	iter(&fLinkedList);
	
	ScopeIDLink* link = (ScopeIDLink*) iter.First();
	while (iter.IsNotComplete() && (exists == kODFalse)) {
		if (link->GetID() == id)
			exists = kODTrue;
		link = (ScopeIDLink*) iter.Next();
	}

	return exists;
}

