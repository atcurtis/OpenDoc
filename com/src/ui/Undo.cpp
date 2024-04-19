/* @(#)Z 1.8 com/src/ui/Undo.cpp, odui, od96os2, odos29646d 96/11/15 15:28:20 (96/08/14 17:31:01) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odui
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
	File:		Undo.cpp

	Contains:	Implementation for Undo class

	Owned by:	Nick Pilch

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<24>	 10/8/95	TJ		Fixes Recomended by Refball
		<23>	 9/11/95	NP		1272294: Messaging code cleanup.
		<22>	 8/26/95	T‚		1274606 FB2: Patching Remarks
		<21>	 8/23/95	NP		1275237, 1267015: Support for aborting
									transactions.
		<20>	 8/22/95	eeh		1276821:
									kOpenDocShellSignature->kODShellSignature
		<19>	 8/16/95	NP		1275241: IDL Review. Remove unnecessary
									overrides.
		<18>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		<17>	  8/7/95	NP		1269965: should be able to nest
									transactions, 1269789: Transaction redo
									incorrectly implemented.
		<16>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<15>	 7/26/95	DM		#1270320: Memory leak fixes: dispose descs
									in NotifyUndoOrRedoOrDispose
		<14>	 6/16/95	TJ		Uncommented the pragma export_lib on.
		<13>	 6/15/95	NP		1237542: Fix CORBA memory stuff, 1249152:
									Undo not respecting marks
		<12>	 5/26/95	RR		#1251403: Multithreading naming support
		<11>	 5/21/95	NP		1249290: Fix crash if libraries are
									preloaded.
		<10>	  5/3/95	NP		1211084: Remove 5$
		 <9>	 4/28/95	NP		1223103: Logic error in Undo. 1243568,
									1209554: use of ODGetSystemHeap.
		 <8>	 4/26/95	NP		1243568: Don't use ODSystemHeap.
		 <7>	 4/14/95	NP		1238258: Don't use OrderedCollection, use a
									collection class that has no virtual
									methods and that uses no classes with
									virtual methods.
		 <6>	 4/14/95	NP		1217965: Destroy all system objects when
									last doc is closed.
		 <5>	 1/26/95	eeh		1213354: ODActionData now an ODByteArray.
		 <4>	 1/25/95	jpa		New ODIText format [1212619]
		 <3>	12/25/94	T‚		1191189 Eliminate duplicate Def.h, Def.xh
									files
		 <2>	 9/29/94	RA		1189812: Mods for 68K build.
		 <1>	 7/27/94	NP		first checked in

	To Do:
		Remove destructors where not needed.
		
*/


#ifndef _PART_
#include "Part.xh"
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include "StdDefs.xh"
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif

#define ODUndo_Class_Source
#define VARIABLE_MACROS
#include <Undo.xih>

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#pragma segment ODUndo

/*
IMPLEMENTATION NOTES

System Heap allocations

Almost all memory allocations occur in the system heap.

I'm trying to use as little system heap memory as possible because putting
pointers into the system heap could be a bad idea (mem fragmentation).

UNDO AND REDO STACKS

The stacks are implemented with the UndoOrderedCollection class. The first item
in the collection represents the top of the stack.

MARKING

When asked to mark a stack that is empty, nothing is done. The code that removes
actions from a stack up to the last mark simply stops when it comes to the end
of a stack.

ADDCLIENT, ETC.

We need to use these functions to ensure the SystemUndo is freed when
the last OD document is closed. The memory associated with it will have gone
away.
*/

enum ODUndoMemoryAllocation
{
  kODSystemHeap
};

void*
operator new(
	     size_t size,
	     ODUndoMemoryAllocation allocation
	     );

void*
operator new(
	     size_t size,
	     ODUndoMemoryAllocation allocation
	     )
{
  void*	retVal;
  
  if (allocation == kODSystemHeap)
    retVal = ODNewPtr(size);
  else
    THROW(kODErrNotImplemented);
  
  return retVal;
}

//==============================================================================
// Temporarily include new UndoOrderedCollection class that I can allocate in the
//	system heap. (May be longer than temporary.)
//==============================================================================



/////////////// BEGIN TEMP HACK



//==============================================================================
// UndoLink
//==============================================================================

class  UndoLink {
public:
  void operator delete(void* ptr) {ODDisposePtr(ptr);}

  UndoLink();
  UndoLink(UndoLink* next, UndoLink* previous);
  UndoLink( const UndoLink& );
  ~UndoLink();
  UndoLink* GetNext() const {return fNext;}
  UndoLink* GetPrevious() const {return fPrevious;}
  
  // The following operations are provided for efficiency, but DO NOT USE THEM
  // if there are any iterators active on a list. These operations don't bump
  // the list's fSeed and the iterators will not be able to detect that they
  // are out of sync!
  void Remove( );
  void AddBefore( UndoLink *aLink );
  void AddAfter( UndoLink *aLink );
  
  //private-by-convention:
  void SetNext(UndoLink* aLink) {fNext = aLink;}
  void SetPrevious(UndoLink* aLink) {fPrevious = aLink;}
  
private:
  
  UndoLink* fNext;
  UndoLink* fPrevious;
};


//==============================================================================
// UndoLinkedList
//==============================================================================


class UndoLinkedList {
public:
  void operator delete(void* ptr) {ODDisposePtr(ptr);}
  UndoLinkedList();
  ~UndoLinkedList();
  void Remove(UndoLink&);
  void AddFirst(UndoLink* link);
  UndoLink* RemoveFirst();
  UndoLink* First() const;
protected:
  UndoLink fSentinel; // Marks the head & tail
  ODULong fSeed; // Used to detect out-of-sync iterators
  UndoLink* GetSentinel( ) {return &fSentinel;}
  const UndoLink* GetSentinel( ) const {return &fSentinel;}
  ODBoolean IsSentinel( const UndoLink* link ) const {return link==this->GetSentinel();}
  ODBoolean NotSentinel( const UndoLink* link ) const {return link!=this->GetSentinel();}
  
private:			  	
friend class UndoLinkedListIterator;
};


//=====================================================================================
// UndoLinkedListIterator
//=====================================================================================

class UndoLinkedListIterator {
public:
  UndoLinkedListIterator(UndoLinkedList* list);
  ~UndoLinkedListIterator();
  UndoLink* First();
  UndoLink* Next();
  ODBoolean IsNotComplete();
private:
  UndoLinkedList* fList;
  UndoLink* fCurrent;
  UndoLink* fNext;      // Used only when deleting while iterating
  UndoLink* fPrevious;  // Used only when deleting while iterating
  UndoLink* fSentinel;
  ODULong fSeed;        // Used to detect out-of-sync iterators
};

//==============================================================================
// Scalar Types
//==============================================================================

typedef void* ElementType;

//=====================================================================================
// Class UndoValueLink - Definition
//=====================================================================================

class UndoValueLink : public UndoLink {
public:
  UndoValueLink(ElementType value);		
  ~UndoValueLink();
  ElementType GetValue() { return fValue;}
  void SetValue(ElementType v) { fValue = v;}
  
private:
  ElementType fValue;
};

//=====================================================================================
// Class UndoOrderedCollection
//=====================================================================================

class UndoOrderedCollection
{
  
public:
  void operator delete(void* ptr) {ODDisposePtr(ptr);}
  UndoOrderedCollection();
  UndoOrderedCollection(ODUndoMemoryAllocation where);
  ~UndoOrderedCollection();
  void	AddFirst(ElementType element);
  ElementType	First();

  // Returns kODNULL if there is no first element.
  ElementType	RemoveFirst();

  // Don't call if there are no elements. Crash will result.
  void	Remove(ElementType existing);
  
  // Called from the destructor. Removes all elements, deleting the links
  // Does not delete the elements themselves
  void	RemoveAll();
  
protected:
  UndoValueLink* CreateNewLink(ElementType value) const;

  // Does a pointer comparison by default 
  ODBoolean ElementsMatch(ElementType v1,ElementType v2) const;
  
private:
  UndoLinkedList fImplementation;
  ODUndoMemoryAllocation fHeap;  // if kODNULL, use default heap.
  
friend class UndoOrderedCollectionIterator;
friend class ListIterator;
};

//=====================================================================================
// Class UndoOrderedCollectionIterator
//=====================================================================================

class UndoOrderedCollectionIterator {
public:
  UndoOrderedCollectionIterator(UndoOrderedCollection* collection);
  ~UndoOrderedCollectionIterator();
  ElementType First();
  ElementType Next();
  ODBoolean IsNotComplete();
  
private:
  UndoOrderedCollection* fCollection;
  UndoLinkedListIterator fImplementation;
};

//==============================================================================
// UndoLink
//==============================================================================



// Many of the simple link methods are inlines; see List.h for implementations.


//------------------------------------------------------------------------------
// UndoLink::UndoLink
//
// Constructor for UndoLink
//------------------------------------------------------------------------------

UndoLink::UndoLink()							
{ 
  fNext = kODNULL; 
  fPrevious = kODNULL;
}

//------------------------------------------------------------------------------
// UndoLink::UndoLink
//
// Constructor for UndoLink
//------------------------------------------------------------------------------

UndoLink::UndoLink(UndoLink* next, UndoLink* previous)							
{ 
  fNext = next; 
  fPrevious = previous;
}

//------------------------------------------------------------------------------
// UndoLink::UndoLink
//
// Copy constructor for UndoLink
//------------------------------------------------------------------------------

UndoLink::UndoLink( const UndoLink &link )							
{ 
  fNext = link.fNext; 
  fPrevious = link.fPrevious;
}

//------------------------------------------------------------------------------
// UndoLink::UndoLink
//
// Destructor for UndoLink
//------------------------------------------------------------------------------

UndoLink::~UndoLink()											
{
}

//------------------------------------------------------------------------------
// UndoLink::Remove
//
// Remove a link from its list (if any). DO NOT call this directly if there are
// any iterators active on the list; use UndoLinkedList::Remove instead.
//------------------------------------------------------------------------------

void	UndoLink::Remove( )
{
  if( fPrevious )
    fPrevious->SetNext(fNext);
  if( fNext )
    fNext->SetPrevious(fPrevious);
  fNext = kODNULL;
  fPrevious = kODNULL;
}

//------------------------------------------------------------------------------
// UndoLink::AddBefore
//
// Add a link to a list before another link. It must not already be on any list.
// DO NOT call this directly if there are any iterators active on the list;
// use UndoLinkedList::Remove instead.
//------------------------------------------------------------------------------

void	UndoLink::AddBefore( UndoLink *link )
{
  ASSERT(link!=kODNULL,paramErr);
  WASSERT(fNext==kODNULL);
  WASSERT(fPrevious==kODNULL);
  fNext = link;
  fPrevious = link->GetPrevious();
  fPrevious->SetNext(this);
  fNext->SetPrevious(this);
}

//------------------------------------------------------------------------------
// UndoLink::AddAfter
//
// Add a link to a list after another link. It must not already be on any list.
// DO NOT call this directly if there are any iterators active on the list;
// use UndoLinkedList::Remove instead.
//------------------------------------------------------------------------------

void	UndoLink::AddAfter( UndoLink *link )
{
  ASSERT(link!=kODNULL,paramErr);
  WASSERT(fNext==kODNULL);
  WASSERT(fPrevious==kODNULL);
  fPrevious = link;
  fNext = link->GetNext();
  fPrevious->SetNext(this);
  fNext->SetPrevious(this);
}

//======================================================================================
// Class UndoLinkedList
//======================================================================================

//------------------------------------------------------------------------------
// UndoLinkedList::UndoLinkedList
//
// Constructor for UndoLinkedList
//------------------------------------------------------------------------------

UndoLinkedList::UndoLinkedList()
:fSentinel(&fSentinel,&fSentinel),
 fSeed(0)
{
}

//------------------------------------------------------------------------------
// UndoLinkedList::~UndoLinkedList
//
// Destructor for UndoLinkedList
//------------------------------------------------------------------------------

UndoLinkedList::~UndoLinkedList()
{
  // The list does NOT delete all its links!
}

//------------------------------------------------------------------------------
// UndoLinkedList::Remove
//
// Description
//------------------------------------------------------------------------------

void	UndoLinkedList::Remove(UndoLink& aLink)
{
  fSeed++;
  aLink.Remove();
}

//------------------------------------------------------------------------------
// UndoLinkedList::RemoveFirst
//
// Description
//------------------------------------------------------------------------------

UndoLink* UndoLinkedList::RemoveFirst()
{
  UndoLink* old = fSentinel.GetNext();
  if (this->NotSentinel(old))
    {
      fSeed++;
      old->Remove();
      return old;
    }
  else
    {
      return kODNULL;
    }
}

//------------------------------------------------------------------------------
// UndoLinkedList::AddFirst
//
// Description
//------------------------------------------------------------------------------

void	UndoLinkedList::AddFirst(UndoLink* link)
{
  link->AddAfter(this->GetSentinel());
  fSeed++;
}

//------------------------------------------------------------------------------
// UndoLinkedList::First
//
// Description
//------------------------------------------------------------------------------

UndoLink*	UndoLinkedList::First()  const
{
  return this->NotSentinel(fSentinel.GetNext()) ? fSentinel.GetNext() : (UndoLink*) kODNULL;
}

//======================================================================================
// Class UndoLinkedListIterator
//======================================================================================

//------------------------------------------------------------------------------
// UndoLinkedListIterator::UndoLinkedListIterator
//
// Constructor for UndoLinkedListIterator
//------------------------------------------------------------------------------

UndoLinkedListIterator::UndoLinkedListIterator(UndoLinkedList* list)
{
  fList = list;
  fCurrent = kODNULL;
  fNext = kODNULL;
  fPrevious = kODNULL;
  fSentinel = &list->fSentinel;
  fSeed = fList->fSeed;	
}

//------------------------------------------------------------------------------
// UndoLinkedListIterator::~UndoLinkedListIterator
//
// Destructor for UndoLinkedListIterator
//------------------------------------------------------------------------------

UndoLinkedListIterator::~UndoLinkedListIterator()
{
}

//------------------------------------------------------------------------------
// UndoLinkedListIterator::First
//
// Description
//------------------------------------------------------------------------------

UndoLink* UndoLinkedListIterator::First()
{
  if (fList == kODNULL)
    return kODNULL;
  
  if (fSeed != fList->fSeed)
    THROW(kODErrIteratorOutOfSync);
  
  fCurrent = fList->First();
  if (fCurrent == fSentinel)
    fCurrent = kODNULL;
  return fCurrent;
}

//------------------------------------------------------------------------------
// UndoLinkedListIterator::Next
//
// Description
//------------------------------------------------------------------------------

UndoLink* UndoLinkedListIterator::Next()
{
  if (fList == kODNULL)
    return kODNULL;
  
  if (fSeed != fList->fSeed)
    THROW(kODErrIteratorOutOfSync);
  
  if (fCurrent == kODNULL)
    {
      if ((fNext == kODNULL) && (fPrevious == kODNULL))	// Just starting out
	{
	  return this->First();
	}
      else	// Just deleted
	{
	  fCurrent = fNext;
	  fPrevious = kODNULL;
	  fNext = kODNULL;
	}
    }
  else
    fCurrent = fCurrent->GetNext();
  
  if (fCurrent == fSentinel)
    fCurrent = kODNULL;
  return fCurrent;
}

//------------------------------------------------------------------------------
// UndoLinkedListIterator::IsNotComplete
//
// Description
//------------------------------------------------------------------------------

ODBoolean UndoLinkedListIterator::IsNotComplete()
{
  return (fCurrent != kODNULL);
}

//======================================================================================
// Class UndoValueLink - Implementation
//======================================================================================

UndoValueLink::UndoValueLink(ElementType value)
{
  fValue = value;
}

UndoValueLink::~UndoValueLink()
{
}

//======================================================================================
// Class UndoOrderedCollection
//======================================================================================

//------------------------------------------------------------------------------
// UndoOrderedCollection::UndoOrderedCollection
//------------------------------------------------------------------------------

UndoOrderedCollection::UndoOrderedCollection()
{
  fHeap = kODSystemHeap;
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::UndoOrderedCollection
//------------------------------------------------------------------------------

UndoOrderedCollection::UndoOrderedCollection(ODUndoMemoryAllocation where)
{
  fHeap = where;
}

// UndoOrderedCollection::~UndoOrderedCollection
//------------------------------------------------------------------------------

UndoOrderedCollection::~UndoOrderedCollection()
{
  this->RemoveAll();
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::AddFirst
//------------------------------------------------------------------------------

void UndoOrderedCollection::AddFirst(ElementType element)
{
  UndoValueLink* newLink = this->CreateNewLink(element);
  fImplementation.AddFirst(newLink);
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::First
//------------------------------------------------------------------------------

ElementType UndoOrderedCollection::First()
{
  UndoValueLink* firstLink = (UndoValueLink*) fImplementation.First();
  return firstLink ? firstLink->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::RemoveFirst
//------------------------------------------------------------------------------

ElementType	UndoOrderedCollection::RemoveFirst()
{
  UndoValueLink* aLink = (UndoValueLink*) fImplementation.RemoveFirst();
  ElementType value = aLink ? aLink->GetValue() : kODNULL;
  delete aLink;
  return value;
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::Remove
//------------------------------------------------------------------------------

void UndoOrderedCollection::Remove(ElementType existing)
{
  UndoLinkedListIterator iter(&fImplementation);
  UndoValueLink* aLink = (UndoValueLink*) iter.First();
  while (aLink != kODNULL)
    {
      ElementType v = ((UndoValueLink*) aLink)->GetValue();
      
      if (this->ElementsMatch(v,existing))
	{
	  fImplementation.Remove(*aLink);
	  delete aLink;
	  aLink = kODNULL;	
	}
      else
	aLink = (UndoValueLink*) iter.Next();
    }	
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::RemoveAll
//------------------------------------------------------------------------------

void UndoOrderedCollection::RemoveAll()
{
  UndoLink* link = fImplementation.RemoveFirst();
  while (link != kODNULL)
    {
      delete link;
      link = fImplementation.RemoveFirst();
    }
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::CreateNewLink
//------------------------------------------------------------------------------

UndoValueLink*	UndoOrderedCollection::CreateNewLink(ElementType value) const
{
  return new (fHeap) UndoValueLink(value);
}

//------------------------------------------------------------------------------
// UndoOrderedCollection::ElementsMatch
//------------------------------------------------------------------------------

ODBoolean	UndoOrderedCollection::ElementsMatch(ElementType v1,ElementType v2) const
{
  return (v1 == v2);
}

//======================================================================================
// UndoOrderedCollectionIterator
//======================================================================================

//------------------------------------------------------------------------------
// UndoOrderedCollectionIterator::UndoOrderedCollectionIterator
//------------------------------------------------------------------------------

UndoOrderedCollectionIterator::UndoOrderedCollectionIterator(UndoOrderedCollection* collection)	
: fImplementation(collection ? &(collection->fImplementation) : (UndoLinkedList*)kODNULL)
{
  fCollection =  collection;
}

//------------------------------------------------------------------------------
// UndoOrderedCollectionIterator::~UndoOrderedCollectionIterator
//------------------------------------------------------------------------------

UndoOrderedCollectionIterator::~UndoOrderedCollectionIterator()						
{
}

//------------------------------------------------------------------------------
// UndoOrderedCollectionIterator::First
//------------------------------------------------------------------------------

ElementType	UndoOrderedCollectionIterator::First()
{
  UndoValueLink* link = (UndoValueLink*) fImplementation.First();
  
  return link ? link->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// UndoOrderedCollectionIterator::Next
//------------------------------------------------------------------------------

ElementType	UndoOrderedCollectionIterator::Next()
{		
  UndoValueLink* link = (UndoValueLink*) fImplementation.Next();
  
  return link ? link->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// UndoOrderedCollectionIterator::IsNotComplete
//------------------------------------------------------------------------------

ODBoolean	UndoOrderedCollectionIterator::IsNotComplete()
{
  return fImplementation.IsNotComplete();
}



/////////////// END TEMP HACK



//==============================================================================
// Constants
//==============================================================================

#if defined(_PLATFORM_MACINTOSH_)
const OSType kUndoNotifyID = 'undo';
const OSType kRedoNotifyID = 'redo';
const OSType kDisposeActionNotifyID = 'del ';

const AEKeyword kDataPtrKeyword = 'data';
const AEKeyword kPartPtrKeyword = 'part';
const AEKeyword kDoneStateKeyword = 'done';
#endif

// A number of functions can share code. They pass along this enum to know
// which course of action to take.
enum ODUndoRedoType
{
  kUndo,
  kRedo,
  kDispose
};

//==============================================================================
// Classes used in this file
//==============================================================================

class UndoAction;
class SystemUndo;

//==============================================================================
// Function Prototypes
//==============================================================================

#if defined(_PLATFORM_MACINTOSH_)
static OSErr NotifyUndoOrRedoOrDispose(UndoAction* action,
				       ODUndoRedoType which);
static OSErr NotifyUndo(UndoAction* action);
static OSErr NotifyRedo(UndoAction* action);
static OSErr NotifyDispose(UndoAction* action);
static SystemUndo *GetSystemUndo();
static ODBoolean SetSystemUndo(SystemUndo *systemUndo);
#else

// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// These following functions:
//   NotifyUndo
//   NotifyRedo
//   NotifyDispose
// replace the Apple functions above. Use them until
// Scripting is reimplemented. -Rab

static ODError NotifyUndo(UndoAction* action);
static ODError NotifyRedo(UndoAction* action);
static ODError NotifyDispose(UndoAction* action);

// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
#endif

//==============================================================================
// Functions
//==============================================================================

// ¥ Using ASLM we registered the system undo object with ASLM using the
// arbitration mechanism provided by ASLM. To eliminate our dependency on ASLM
// we are storing the system undo object in a global which is accessed by the
// following two functions. This is NOT a thread safe method for accessing the
// system undo!

//------------------------------------------------------------------------------
// GetSystemUndo
//------------------------------------------------------------------------------

#if defined(_PLATFORM_MACINTOSH_)
#pragma lib_export on
#endif

static SystemUndo* gSystemUndo;

#if defined(_PLATFORM_MACINTOSH_)
#pragma lib_export off
#endif

static SystemUndo *GetSystemUndo()
{
  return gSystemUndo;
}

//------------------------------------------------------------------------------
// SetSystemUndo
//------------------------------------------------------------------------------

static ODBoolean SetSystemUndo(SystemUndo *systemUndo)
{
  gSystemUndo = systemUndo;
  return kODTrue;
}

//==============================================================================
// Local Classes
//==============================================================================

//------------------------------------------------------------------------------

class UndoAction
{
public:
  void operator delete(void* ptr) {ODDisposePtr(ptr);}
  UndoAction(
	     ODPart* whichPart,
	     ODActionData* actionData,
	     ODActionType actionType,
	     ODName* undoActionLabel,
	     ODName* redoActionLabel
	     );
  ~UndoAction();
  
  ODPart* fPart;
  ODActionData* fActionData;
  ODActionType fActionType;
  ODName* fUndoActionLabel;
  ODName* fRedoActionLabel;
  ODBoolean fMark;
  ODDoneState fDoneState;
};

//------------------------------------------------------------------------------
// UndoAction::UndoAction
//
//	This constructor may throw an error, but it's OK because there is nothing
//	to deallocate in the destructor.
//------------------------------------------------------------------------------

UndoAction::UndoAction(
		       ODPart* whichPart, 
		       ODActionData* actionData,
		       ODActionType actionType,
		       ODName* undoActionLabel,
		       ODName* redoActionLabel
		       )
{
  fPart = whichPart;
#if defined(_PLATFORM_MACINTOSH_)
  THROW_IF_ERROR(GetCurrentProcess(&fProcessNum));
#endif
  fActionData = actionData;
  fActionType = actionType;
  fUndoActionLabel = undoActionLabel;
  fRedoActionLabel = redoActionLabel;
  fMark = kODFalse;
  fDoneState = kODDone;
}

//------------------------------------------------------------------------------
// UndoAction::~UndoAction
//
//	Notify owning part owning fActionData to dipose of any associated memory.
//------------------------------------------------------------------------------

UndoAction::~UndoAction()
{
#if defined(_PLATFORM_MACINTOSH_)
  NotifyDispose(this);
#endif
  
  DisposeIText(fUndoActionLabel);
  DisposeIText(fRedoActionLabel);
  DisposeByteArray(fActionData);
}

//------------------------------------------------------------------------------

class SystemUndo
{
public:
  void operator delete(void* ptr) {ODDisposePtr(ptr);}
  SystemUndo();
  ~SystemUndo();
  void	Initialize();
  ODSize Purge(ODSize size);
  ODBoolean CheckActionOK(UndoAction* action);
  void AddActionToHistory(
			  ODPart* whichPart, 
			  ODActionData* actionData,
			  ODActionType actionType,
			  ODName* undoActionLabel,
			  ODName* redoActionLabel
			  );
  void Undo();
  void Redo();
  void MarkActionHistory();
  void ClearActionHistory(ODRespectMarksChoices respectMarks);
  void ClearRedoHistory();
  ODBoolean PeekUndoHistory(
			    ODPart** part,
			    ODActionData* actionData,
			    ODActionType* actionType,
			    ODName* actionLabel
			    );
  ODBoolean PeekRedoHistory(
			    ODPart** part,
			    ODActionData* actionData,
			    ODActionType* actionType,
			    ODName* actionLabel
			    );
  void RemoveEntriesForThisProcess();
  void AddClient(ODUndo* docUndoObject);
  void RemoveClient(ODUndo* docUndoObject);
  ODULong GetNumClients();
  void AbortCurrentTransaction();
private:
  void TransactionUndo(UndoAction* firstAction);
  void TransactionRedo(UndoAction* firstAction);
  void MarkActionOrSelf(UndoAction* action, ODBoolean* mark);
  void ClearActionsToMark(UndoOrderedCollection* stack);
  void ClearStack(UndoOrderedCollection* stack);
  ODBoolean PeekHistory(
			ODPart** part,
			ODActionData* actionData,
			ODActionType* actionType,
			ODName* actionLabel,
			ODUndoRedoType which);
  void MoveUndoToRedo();
  void MoveRedoToUndo();

  UndoOrderedCollection* fUndoStack;
  UndoOrderedCollection* fRedoStack;
  ODULong fInTransaction;
  ODBoolean fCurrentlyUndoingOrRedoing;
  ODULong fNumUsers;
};

//------------------------------------------------------------------------------
// SystemUndo::SystemUndo
//------------------------------------------------------------------------------

SystemUndo::SystemUndo()
{
  fUndoStack = kODNULL;
  fRedoStack = kODNULL;
  fInTransaction = 0;
  fCurrentlyUndoingOrRedoing = kODFalse;
  fNumUsers = 0;
}

//------------------------------------------------------------------------------
// SystemUndo::Initialize				
//------------------------------------------------------------------------------

void SystemUndo::Initialize ()
{
  ODUndoMemoryAllocation	heap = kODSystemHeap;
  
  fUndoStack = new (heap) UndoOrderedCollection(heap);
  fRedoStack = new (heap) UndoOrderedCollection(heap);
  
}

//------------------------------------------------------------------------------
// SystemUndo::~SystemUndo
//------------------------------------------------------------------------------

SystemUndo::~SystemUndo()
{
//	WASSERTM(0, "System Undo object being destroyed!");
  delete fUndoStack;
  delete fRedoStack;
}

//------------------------------------------------------------------------------
// SystemUndo::Purge
//------------------------------------------------------------------------------

ODSize	SystemUndo::Purge(ODSize size)
{
  ODUnused(size);
  return 0;
}

//------------------------------------------------------------------------------
// SystemUndo::CheckActionOK
//------------------------------------------------------------------------------

ODBoolean SystemUndo::CheckActionOK(UndoAction* action)
{
  if (action && !(action->fMark))
    return kODTrue;
  return kODFalse;
}

//------------------------------------------------------------------------------
// SystemUndo::AddClient
//------------------------------------------------------------------------------

void SystemUndo::AddClient(ODUndo* docUndoObject)
{
  ODUnused(docUndoObject);
  ++fNumUsers;
}

//------------------------------------------------------------------------------
// SystemUndo::RemoveClient
//------------------------------------------------------------------------------

void SystemUndo::RemoveClient(ODUndo* docUndoObject)
{
  ODUnused(docUndoObject);
  --fNumUsers;
}

//------------------------------------------------------------------------------
// SystemUndo::GetNumClients
//------------------------------------------------------------------------------

ODULong SystemUndo::GetNumClients()
{
  return fNumUsers;
}

//------------------------------------------------------------------------------
// SystemUndo::AddActionToHistory
//------------------------------------------------------------------------------

void SystemUndo::AddActionToHistory(
				    ODPart* whichPart, 
				    ODActionData* actionData,
				    ODActionType actionType,
				    ODName* undoActionLabel,
				    ODName* redoActionLabel
				    )
{
  if (fCurrentlyUndoingOrRedoing)
    THROW(kODErrCannotAddAction);
  
  if (fInTransaction && actionType == kODBeginAction)
    THROW(kODErrCannotAddAction);
  
  if (actionType == kODBeginAction)
    ++fInTransaction;
  if (actionType == kODEndAction)
    --fInTransaction;
  
  ODIText* undoName = CopyIText(undoActionLabel);
  ODIText* redoName = CopyIText(redoActionLabel);
  
  ODActionData* copiedData = (ODActionData*)ODNewPtr(sizeof(ODActionData));
  copiedData->_buffer = (octet*)ODNewPtr(actionData->_length);
  ODBlockMove(actionData->_buffer, copiedData->_buffer, actionData->_length);
  copiedData->_length = actionData->_length;
  copiedData->_maximum = actionData->_length;
  
  UndoAction* action =
    new (kODSystemHeap) UndoAction(whichPart, copiedData,
				   actionType, undoName,
				   redoName);
  fUndoStack->AddFirst(action);
  this->ClearStack(fRedoStack);
}

//------------------------------------------------------------------------------
// SystemUndo::MoveUndoToRedo
//
//	Utility to move item from top of one stack to the top of another.
//------------------------------------------------------------------------------

void SystemUndo::MoveUndoToRedo()
{
  ElementType item = fUndoStack->RemoveFirst();
  ((UndoAction*)item)->fDoneState = kODUndone;
  fRedoStack->AddFirst(item);
}

//------------------------------------------------------------------------------
// SystemUndo::MoveRedoToUndo
//
//	Utility to move item from top of one stack to the top of another.
//------------------------------------------------------------------------------

void SystemUndo::MoveRedoToUndo()
{
  ElementType item = fRedoStack->RemoveFirst();
  ((UndoAction*)item)->fDoneState = kODRedone;
  fUndoStack->AddFirst(item);
}

//------------------------------------------------------------------------------
// SystemUndo::Undo
//
//	If there's an error while undoing anything, just clear the stacks and get
//	out.
//------------------------------------------------------------------------------

void SystemUndo::Undo()
{
  UndoAction* action = (UndoAction*)fUndoStack->First();
  if (!this->CheckActionOK(action))
    THROW(kODErrEmptyStack);
  
  fCurrentlyUndoingOrRedoing = kODTrue;
  
  TRY
    if (action->fActionType == kODEndAction) // End is at top for an Undo
      this->TransactionUndo(action);
    else
      {
	THROW_IF_ERROR(NotifyUndo(action));
	this->MoveUndoToRedo();
      }
  CATCH_ALL
    this->ClearActionHistory(kODDontRespectMarks);
  fCurrentlyUndoingOrRedoing = kODFalse;
  RERAISE;
  ENDTRY
    fCurrentlyUndoingOrRedoing = kODFalse;
}

//------------------------------------------------------------------------------
// SystemUndo::TransactionUndo
//------------------------------------------------------------------------------

void SystemUndo::TransactionUndo(UndoAction* firstAction)
{
  UndoAction*	action;
  ODULong		nestedTransactionCounter = 1;
  
  THROW_IF_ERROR(NotifyUndo(firstAction));
  this->MoveUndoToRedo();
  
  do
    {
      action = (UndoAction*)fUndoStack->First();
      if (action == (UndoAction*)kODNULL)
				THROW(kODErrNoBeginAction);
      THROW_IF_ERROR(NotifyUndo(action));
      this->MoveUndoToRedo();
      if (action->fActionType == kODBeginAction)
	--nestedTransactionCounter;
      else if (action->fActionType == kODEndAction)
	++nestedTransactionCounter;
    }
  while (nestedTransactionCounter);
}

//------------------------------------------------------------------------------
// SystemUndo::Redo
//------------------------------------------------------------------------------

void SystemUndo::Redo()
{
  UndoAction* action = (UndoAction*)fRedoStack->First();
  if (!this->CheckActionOK(action))
    THROW(kODErrEmptyStack);
  
  fCurrentlyUndoingOrRedoing = kODTrue;
  
  TRY
    if (action->fActionType == kODBeginAction)//Start is at top for an Redo
      this->TransactionRedo(action);
    else
      {
	THROW_IF_ERROR(NotifyRedo(action));
	this->MoveRedoToUndo();
      }
  CATCH_ALL
    this->ClearActionHistory(kODDontRespectMarks);
  fCurrentlyUndoingOrRedoing = kODFalse;
  RERAISE;
  ENDTRY
    fCurrentlyUndoingOrRedoing = kODFalse;
}

//------------------------------------------------------------------------------
// SystemUndo::TransactionRedo
//------------------------------------------------------------------------------

void SystemUndo::TransactionRedo(UndoAction* firstAction)
{
  UndoAction*	action;
  ODULong		nestedTransactionCounter = 1;
  
  THROW_IF_ERROR(NotifyRedo(firstAction));
  this->MoveRedoToUndo();
  
  do
    {
      action = (UndoAction*)fRedoStack->First();
//		if (action == (UndoAction*)kODNULL)
//			THROW(kODErrNoBeginAction);
#if defined(_PLATFORM_MACINTOSH_)
      THROW_IF_ERROR(NotifyRedo(action));
#endif
      this->MoveRedoToUndo();
      if (action->fActionType == kODBeginAction)
				++nestedTransactionCounter;
      else if (action->fActionType == kODEndAction)
				--nestedTransactionCounter;
    }
  while (nestedTransactionCounter);
}

//------------------------------------------------------------------------------
// SystemUndo::MarkActionHistory
//------------------------------------------------------------------------------

void SystemUndo::MarkActionHistory()
{
  UndoAction*	action;
  
  action = (UndoAction*)fUndoStack->First();
  if (action)
    action->fMark = kODTrue;
  action = (UndoAction*)fRedoStack->First();
  if (action)
    action->fMark = kODTrue;
}

//------------------------------------------------------------------------------
// SystemUndo::ClearStack
//------------------------------------------------------------------------------

void SystemUndo::ClearStack(UndoOrderedCollection* stack)
{
  UndoOrderedCollectionIterator	iter(stack);
  UndoAction*			action;
  
  for (action = (UndoAction*)iter.First();
       iter.IsNotComplete();
       action = (UndoAction*)iter.Next())
    {
      delete action;
    }
  
  stack->RemoveAll();
}

//------------------------------------------------------------------------------
// SystemUndo::ClearActionsToMark
//
//	To make more efficient, should just start removing items from the front of
//	the list til I hit the mark.
//------------------------------------------------------------------------------

void SystemUndo::ClearActionsToMark(UndoOrderedCollection* stack)
{
  UndoOrderedCollectionIterator	collectionIter(stack);
  UndoAction*					action;
  UndoOrderedCollection			elementsToDelete;
  
  // ADD ELEMENTS TO REMOVE TO A LIST
  for (action = (UndoAction*)collectionIter.First();
       collectionIter.IsNotComplete();
       action = (UndoAction*)collectionIter.Next())
    {
      if (action->fMark != kODFalse)
	{
	  action->fMark = kODFalse;
	  break;
	}
      else
	elementsToDelete.AddFirst(action);
    }
  
  // REMOVE ELEMENTS
  UndoOrderedCollectionIterator	deleteIter(&elementsToDelete);
  for (action = (UndoAction*)deleteIter.First();
       deleteIter.IsNotComplete();
       action = (UndoAction*)deleteIter.Next())
    {
      stack->Remove(action);
      delete action;
    }
}

//------------------------------------------------------------------------------
// SystemUndo::ClearActionHistory
//------------------------------------------------------------------------------

void SystemUndo::ClearActionHistory(ODRespectMarksChoices respectMarks)
{
  if (respectMarks == kODRespectMarks)
    {
      this->ClearActionsToMark(fUndoStack);
      this->ClearActionsToMark(fRedoStack);
    }
  else
    {
      this->ClearStack(fUndoStack);
      this->ClearStack(fRedoStack);
    }
}

//------------------------------------------------------------------------------
// SystemUndo::ClearRedoHistory
//------------------------------------------------------------------------------

void SystemUndo::ClearRedoHistory()
{
  this->ClearActionsToMark(fRedoStack);
}

//------------------------------------------------------------------------------
// SystemUndo::AbortCurrentTransaction
//------------------------------------------------------------------------------

void SystemUndo::AbortCurrentTransaction()
{
  ODULong			nestingLevel = 0;
  ODActionType	actionType;
  
  if (fCurrentlyUndoingOrRedoing)
    return;
  if (!fInTransaction)
    return;
  
  UndoAction* action = (UndoAction*)fUndoStack->First();
  
  do
    {
      if (action->fActionType == kODEndAction)
	++nestingLevel;
      fUndoStack->RemoveFirst();
      actionType = action->fActionType;
      delete action;
      if (actionType == kODBeginAction && nestingLevel == 0)
	break;
      action = (UndoAction*)fUndoStack->First();//should never be NULL, right?
    }
  while (kODTrue);
}

//------------------------------------------------------------------------------
// SystemUndo::PeekUndoHistory
//------------------------------------------------------------------------------

ODBoolean SystemUndo::PeekUndoHistory(
				      ODPart** part,
				      ODActionData* actionData,
				      ODActionType* actionType,
				      ODName* actionLabel
				      )
{
  return PeekHistory(part, actionData, actionType, actionLabel, kUndo);
}

//------------------------------------------------------------------------------
// SystemUndo::PeekRedoHistory
//------------------------------------------------------------------------------

ODBoolean SystemUndo::PeekRedoHistory(
				      ODPart** part,
				      ODActionData* actionData,
				      ODActionType* actionType,
				      ODName* actionLabel
				      )
{
  return PeekHistory(part, actionData, actionType, actionLabel, kRedo);
}

//------------------------------------------------------------------------------
// SystemUndo::PeekHistory
//------------------------------------------------------------------------------

ODBoolean SystemUndo::PeekHistory(
				  ODPart** part,
				  ODActionData* actionData,
				  ODActionType* actionType,
				  ODName* actionLabel,
				  ODUndoRedoType which
				  )
{
  UndoOrderedCollection* stack;
  ODIText* iText;
  
  if ((!actionData) || ( !actionLabel) || (!part)   ) {	
    THROW(kODErrInvalidParameter);	
  }

  if (which == kUndo)
    stack = fUndoStack;
  else if (which == kRedo)
    stack = fRedoStack;
  
  UndoAction* action = (UndoAction*)stack->First();
  
  if (!this->CheckActionOK(action))
    return kODFalse;
  else
    {
      if (action->fMark != kODFalse)
	return kODFalse;
      else
	{
	  *part = action->fPart;
	  *actionType = action->fActionType;
	  
	  *actionData = CopyByteArrayStruct(action->fActionData);
	  
	  if (which == kUndo)
	    iText = action->fUndoActionLabel;
	  else if (which == kRedo)
	    iText = action->fRedoActionLabel;
	  
	  *actionLabel = CopyITextStruct(iText);
	  
	  return kODTrue;
	}
    }
}


// This code must be included back into the base for WIN32 and OS2 as soon as
// OSA scripting becomes available. The Apple event mechinism musted be changed
// to use OSA instead. -Rab
#if defined(_PLATFORM_MACINTOSH_)
//------------------------------------------------------------------------------
// NotifyUndoOrRedoOrDispose
//
//	Notify part to Undo or Redo or Dispose its data.
//	Could fine-tune to only pack parameters needed according to "which"
//	parameter
//------------------------------------------------------------------------------

static OSErr NotifyUndoOrRedoOrDispose(
				       UndoAction* action,
				       ODUndoRedoType which
				       )
{
  AEAddressDesc address;
  AppleEvent message;
  AppleEvent reply;
  OSErr error = noErr;
  ProcessSerialNumber psn;
  Boolean isSameProcess;
  DescType eventID;
  
  TRY
  // SOME CODE TO HANDLE THE SEND TO SELF CASE WELL. IF WE HAVE A STANDARD
  //	IDLE PROC TO USE, WE PROBABLY DON'T NEED THIS. (NP-WHAT DID I MEAN
  //	HERE?)
  THROW_IF_ERROR(GetCurrentProcess(&psn));
  THROW_IF_ERROR(SameProcess(&action->fProcessNum, &psn,
			     &isSameProcess));
  if (isSameProcess)
    {
      psn.lowLongOfPSN = kCurrentProcess;
      psn.highLongOfPSN = 0;
    }
  else
    psn = action->fProcessNum;

  THROW_IF_ERROR(AECreateDesc(typeProcessSerialNumber,
			      (Ptr)&psn, sizeof(psn), &address));
  
  if (which == kUndo)
    eventID = kUndoNotifyID;
  else if (which == kRedo)
    eventID = kRedoNotifyID;
  else if (which == kDispose)
    eventID = kDisposeActionNotifyID;
  
  THROW_IF_ERROR(AECreateAppleEvent(kODShellSignature, eventID,
				    &address, kAutoGenerateReturnID, kAnyTransactionID,
				    &message));
  TempAEDesc tempMessage(&message); // DMc make sure it is disposed
  
  THROW_IF_ERROR(AEPutParamPtr(&message, kDataPtrKeyword, typeChar,
			       action->fActionData->_buffer,
			       action->fActionData->_length));
  
  THROW_IF_ERROR(AEPutParamPtr(&message, kPartPtrKeyword, typeInteger,
			       (Ptr)&action->fPart,
			       sizeof(action->fPart)));
  
  THROW_IF_ERROR(AEPutParamPtr(&message, kDoneStateKeyword, typeInteger,
			       (Ptr)&action->fDoneState,
			       sizeof(action->fDoneState)));
  
  THROW_IF_ERROR(AESend(&message, &reply,
			kAEWaitReply + kAECanInteract + kAECanSwitchLayer
			+ kAEDontRecord,
			kAENormalPriority,
			kAEDefaultTimeout, (AEIdleUPP)kODNULL,
			(AEFilterUPP)kODNULL));
  AEDisposeDesc(&reply); // DMc
  CATCH_ALL
    error = ErrorCode();
  ENDTRY
    
    return error;
}

//------------------------------------------------------------------------------
// NotifyUndo
//
//	Call ODPart::Undo.
//------------------------------------------------------------------------------

static OSErr NotifyUndo(UndoAction* action)
{
  return NotifyUndoOrRedoOrDispose(action, kUndo);
}

//------------------------------------------------------------------------------
// NotifyRedo
//
//	Call ODPart::Redo.
//------------------------------------------------------------------------------

static OSErr NotifyRedo(UndoAction* action)
{
  return NotifyUndoOrRedoOrDispose(action, kRedo);
}

//------------------------------------------------------------------------------
// NotifyDispose
//
//	Call ODPart::DiposeActionState.
//------------------------------------------------------------------------------

static OSErr NotifyDispose(UndoAction* action)
{
  return NotifyUndoOrRedoOrDispose(action, kDispose);
}


//==============================================================================
// ODUndo
//==============================================================================

//------------------------------------------------------------------------------
// HandleUndoOrRedoOrDisposeNotify
//
//	Could fine-tune to only unpack parameters needed according to "which"
//	parameter
//------------------------------------------------------------------------------

static OSErr HandleUndoOrRedoOrDisposeNotify(AppleEvent* message,
					     AppleEvent* reply,
					     long refCon,
					     ODUndoRedoType which)
{
  ODUnused(reply);
  ODUnused(refCon);
  
  OSErr			error = noErr;
  DescType		gotType;
  ODActionData*	data;
  ODPart*			part;
  ODDoneState		doneState;
  Size			actualSize;
  Size			maximumDataSize;
  Size			maximumPartSize = sizeof(part);
  Size			maximumDoneStateSize = sizeof(doneState);
  
  TRY
    gotType = typeChar;
  THROW_IF_ERROR(AESizeOfParam(message, kDataPtrKeyword, &gotType,
			       &actualSize));
  if (gotType != typeChar)
    THROW(errAECorruptData);
  maximumDataSize = actualSize;
  data = CreateEmptyByteArray(actualSize);
  THROW_IF_ERROR(AEGetParamPtr(message, kDataPtrKeyword, typeChar,
			       &gotType, data->_buffer, maximumDataSize,
			       &actualSize));
  if (actualSize != maximumDataSize)
    THROW(errAECorruptData);
  
  THROW_IF_ERROR(AEGetParamPtr(message, kPartPtrKeyword, typeInteger,
			       &gotType, (Ptr)&part, maximumPartSize,
			       &actualSize));
  if (gotType != typeInteger || actualSize != maximumPartSize)
    THROW(errAECorruptData);
  
  THROW_IF_ERROR(AEGetParamPtr(message, kDoneStateKeyword, typeInteger,
			       &gotType, (Ptr)&doneState,
			       maximumDoneStateSize,
			       &actualSize));
  if (gotType != typeInteger || actualSize != maximumDoneStateSize)
    THROW(errAECorruptData);
  
  Environment*	ev = somGetGlobalEnvironment();
  if (which == kUndo)
    {
      part->UndoAction(ev, data);
      error = ODGetSOMException(ev);
    }
  else if (which == kRedo)
    {
      part->RedoAction(ev, data);
      error = ODGetSOMException(ev);
    }
  else if (which == kDispose)
    {
      part->DisposeActionState(ev, data, doneState);
      error = ODGetSOMException(ev);
    }
  
  DisposeByteArray(data);
  
  CATCH_ALL
    error = ErrorCode();
  ENDTRY
    
    return error;
}

//------------------------------------------------------------------------------
// HandleUndoNotify				
//------------------------------------------------------------------------------

static pascal OSErr HandleUndoNotify(AppleEvent* message, AppleEvent* reply,
				     long refCon)
{
  return HandleUndoOrRedoOrDisposeNotify(message, reply, refCon, kUndo);
}

//------------------------------------------------------------------------------
// HandleRedoNotify				
//------------------------------------------------------------------------------

static pascal OSErr HandleRedoNotify(AppleEvent* message, AppleEvent* reply,
				     long refCon)
{
  return HandleUndoOrRedoOrDisposeNotify(message, reply, refCon, kRedo);
}

//------------------------------------------------------------------------------
// HandleDisposeNotify				
//------------------------------------------------------------------------------

static pascal OSErr HandleDisposeNotify(AppleEvent* message, AppleEvent* reply,
					long refCon)
{
  return HandleUndoOrRedoOrDisposeNotify(message, reply, refCon, kDispose);
}
#else

// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// These following functions:
//   NotifyUndo
//   NotifyRedo
//   NotifyDispose
// replace the Apple functions above. Use them until
// Scripting is reimplemented. -Rab
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!! Notice !!!!!!!!!!!!!!!!

//------------------------------------------------------------------------------
// NotifyUndo
//
//	Call ODPart::Undo.
//------------------------------------------------------------------------------

static ODError NotifyUndo(UndoAction* action)
{
  Environment*	ev = somGetGlobalEnvironment();
		action->fPart->UndoAction(ev, action->fActionData);
  return ODGetSOMException(ev);
}

static ODError NotifyRedo(UndoAction* action)
{
  Environment*	ev = somGetGlobalEnvironment();
		action->fPart->RedoAction(ev, action->fActionData);
  return ODGetSOMException(ev);
}

static ODError NotifyDispose(UndoAction* action)
{
  Environment*	ev = somGetGlobalEnvironment();
		action->fPart->DisposeActionState(ev, action->fActionData, action->fDoneState);
  return ODGetSOMException(ev);
}

#endif // 

//------------------------------------------------------------------------------
// ODUndo::InitUndo
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoInitUndo(ODUndo *somSelf, Environment *ev)
{
  ODUndoData *somThis = ODUndoGetData(somSelf);
  ODUndoMethodDebug("ODUndo","ODUndoInitUndo");
  
  /* Moved from somInit. SOM itself sets fields to zero
     _fSystemUndo = kODNULL;
     */
  
  TRY
    somSelf->InitObject(ev);
  
  _fSystemUndo = GetSystemUndo();
  if (!_fSystemUndo)
    {
      _fSystemUndo = new (kODSystemHeap) SystemUndo();
      if (_fSystemUndo)
	{
	  _fSystemUndo->Initialize();
	  SetSystemUndo(_fSystemUndo);
	}
    }

#if defined(_PLATFORM_MACINTOSH_)
  THROW_IF_ERROR(AEInstallEventHandler(kODShellSignature,
				       kUndoNotifyID,
				       NewAEEventHandlerProc(HandleUndoNotify),
				       (long)somSelf, !kIsSysHandler));
  THROW_IF_ERROR(AEInstallEventHandler(kODShellSignature,
				       kRedoNotifyID,
				       NewAEEventHandlerProc(HandleRedoNotify),
				       (long)somSelf, !kIsSysHandler));
  THROW_IF_ERROR(AEInstallEventHandler(kODShellSignature,
				       kDisposeActionNotifyID,
				       NewAEEventHandlerProc(HandleDisposeNotify),
				       (long)somSelf, !kIsSysHandler));
#endif

  _fSystemUndo->AddClient(somSelf);
  CATCH_ALL
    ODDeleteObject(_fSystemUndo);
  ODSetSOMException(ev, ErrorCode());
  ENDTRY
}

//------------------------------------------------------------------------------
// ODUndo::~ODUndo
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndosomUninit(ODUndo *somSelf)
{
  ODUndoData *somThis = ODUndoGetData(somSelf);
  ODUndoMethodDebug("ODUndo","ODUndosomUninit");
  
  if (_fSystemUndo)
    {
      _fSystemUndo->RemoveClient(somSelf);
      
      if (_fSystemUndo->GetNumClients() == 0) // reclaim space.
	{
	  //		WARN("Deleting system undo object.");
	  ODDeleteObject(_fSystemUndo);
	  SetSystemUndo(kODNULL);
	}
    }
  ODUndo_parents_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODUndo::Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODUndoPurge(ODUndo *somSelf, Environment *ev,
																			ODSize size)
{
	ODUndoData *somThis = ODUndoGetData(somSelf);
	ODUndoMethodDebug("ODUndo","ODUndoPurge");
	
  if (_fSystemUndo)
    return _fSystemUndo->Purge(size);
  else
    return 0;
}

//------------------------------------------------------------------------------
// ODUndo::AddActionToHistory
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoAddActionToHistory(ODUndo *somSelf, Environment *ev,
						 ODPart* whichPart,
						 ODActionData* actionData,
						 ODActionType actionType,
						 ODName* undoActionLabel,
						 ODName* redoActionLabel)
{
  ODUndoData *somThis = ODUndoGetData(somSelf);
  ODUndoMethodDebug("ODUndo","ODUndoAddActionToHistory");
  
  SOM_TRY
  
    if ((!actionData)
	|| (!whichPart)
	|| (!undoActionLabel)
	|| (!redoActionLabel)
	) {
      THROW(kODErrInvalidParameter);
    }
    if ((undoActionLabel->text._length > undoActionLabel->text._maximum)
	||(redoActionLabel->text._length > redoActionLabel->text._maximum)
	) {
      THROW(kODErrInvalidParameter); // 124782
    }

    if (_fSystemUndo)
      _fSystemUndo->AddActionToHistory(whichPart, actionData, actionType,
				     undoActionLabel, redoActionLabel);
    else
      THROW(kODErrOutOfMemory);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return;
}

//------------------------------------------------------------------------------
// ODUndo::Undo
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoUndo(ODUndo *somSelf, Environment *ev)
{
  ODUndoData *somThis = ODUndoGetData(somSelf);
  ODUndoMethodDebug("ODUndo","ODUndoUndo");
  
  SOM_TRY

    if (_fSystemUndo)
      _fSystemUndo->Undo();
    else
      THROW(kODErrEmptyStack);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return;
}

//------------------------------------------------------------------------------
// ODUndo::Redo
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoRedo(ODUndo *somSelf, Environment *ev)
{
  ODUndoData *somThis = ODUndoGetData(somSelf);
  ODUndoMethodDebug("ODUndo","ODUndoRedo");
  
  SOM_TRY

    if (_fSystemUndo)
      _fSystemUndo->Redo();
    else
      THROW(kODErrEmptyStack);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return;
}

//------------------------------------------------------------------------------
// ODUndo::MarkActionHistory
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoMarkActionHistory(ODUndo *somSelf, Environment *ev)
{
  ODUndoData *somThis = ODUndoGetData(somSelf);
  ODUndoMethodDebug("ODUndo","ODUndoMarkActionHistory");
  
  SOM_TRY

    if (_fSystemUndo)
      _fSystemUndo->MarkActionHistory();
    else
      THROW(kODErrCannotMarkAction);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return;
}

//------------------------------------------------------------------------------
// ODUndo::ClearActionHistory
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoClearActionHistory(ODUndo *somSelf, Environment *ev,
						 ODRespectMarksChoices respectMarks)
{
  ODUndoMethodDebug("ODUndo","ODUndoClearActionHistory");
  ODUndoData *somThis = ODUndoGetData(somSelf);
  
  SOM_TRY

    if (_fSystemUndo)
      _fSystemUndo->ClearActionHistory(respectMarks);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return;
}

//------------------------------------------------------------------------------
// ODUndo::ClearRedoHistory
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoClearRedoHistory(ODUndo *somSelf, Environment *ev)
{
  ODUndoMethodDebug("ODUndo","ODUndoClearRedoHistory");
  ODUndoData *somThis = ODUndoGetData(somSelf);
  
  SOM_TRY

    if (_fSystemUndo)
      _fSystemUndo->ClearRedoHistory();

  SOM_CATCH_ALL
  SOM_ENDTRY

  return;
}

//------------------------------------------------------------------------------
// ODUndo::PeekUndoHistory
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODUndoPeekUndoHistory(ODUndo *somSelf, Environment *ev,
						   ODPart** part,
						   ODActionData* actionData,
						   ODActionType* actionType,
						   ODName* actionLabel)
{
  ODUndoMethodDebug("ODUndo","ODUndoPeekUndoHistory");
  ODUndoData *somThis = ODUndoGetData(somSelf);
  
  SOM_TRY

    if (_fSystemUndo)
      return _fSystemUndo->PeekUndoHistory(part, actionData, actionType, actionLabel);
    else
      return kODFalse;

  SOM_CATCH_ALL
  SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODUndo::PeekRedoHistory
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODUndoPeekRedoHistory(ODUndo *somSelf, Environment *ev,
						   ODPart** part,
						   ODActionData* actionData,
						   ODActionType* actionType,
						   ODName* actionLabel)
{
  ODUndoMethodDebug("ODUndo","ODUndoPeekRedoHistory");
  ODUndoData *somThis = ODUndoGetData(somSelf);
  
  SOM_TRY

    if (_fSystemUndo)
      return _fSystemUndo->PeekRedoHistory(part, actionData, actionType, actionLabel);
    else
      return kODFalse;

  SOM_CATCH_ALL
  SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODUndo::AbortCurrentTransaction
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUndoAbortCurrentTransaction(ODUndo *somSelf, Environment *ev)
{
  ODUndoMethodDebug("ODUndo","ODUndoAbortCurrentTransaction");
  ODUndoData *somThis = ODUndoGetData(somSelf);
  
  SOM_TRY

    if (_fSystemUndo)
      _fSystemUndo->AbortCurrentTransaction();

  SOM_CATCH_ALL
  SOM_ENDTRY

  return;
}
