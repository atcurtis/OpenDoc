//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   ODObjectOrdColl
//		ODRefCntCollection
//		ODRefCntCollectionIterator
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
// @(#) 1.3 com/src/utils/include/RefCtCol.h, odutils, od96os2, odos29646d 7/15/96 18:01:58 [ 11/15/96 15:29:34 ]
/*
	File:		RefCtCol.h

	Contains:	OrderedCollection of ODRefCntObject

	Owned by:	David McCusker

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _REFCTCOL_
#define _REFCTCOL_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

// ODRefCntCollection is an ordered collection of ODRefCntObject.  Adding to
// the collection always performs an Acquire(), but removing generally does not
// (presumably you sometimes wish to perform actions on removed elements).
// Duplicates are allowed.
//
// ODRefCntCollection is not intended to be subclassable.

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class ODRefCntCollection;	// An ordered (not sorted) collection of ODRefCntObject
class ODRefCntCollectionIterator;

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class ODObjectOrdColl : public OrderedCollection
{
	public:
		ODObjectOrdColl() : OrderedCollection()	{}
		ODObjectOrdColl(ODMemoryHeapID where) : OrderedCollection(where) {}
		~ODObjectOrdColl();

		ODMethod ODBoolean ElementsMatch(ElementType v1,ElementType v2) const;
		// match using ODObjectsAreEqual()
};

//=====================================================================================
// Global Variables
//=====================================================================================

//=====================================================================================
// Class OrderedCollection
//=====================================================================================

class ODRefCntCollection
{
public:

	ODRefCntCollection(Environment* ev);
	ODRefCntCollection(Environment* ev, ODMemoryHeapID where);
		// The environment is needed by the destructor, so we just pass
		// it in to the constructor rather than to every method that needs
		// to acquire or release.
		
	~ODRefCntCollection();

	ODULong          Count() const;
	
	void             AddFirstAndAcquire(ODRefCntObject* element);
	void             AddLastAndAcquire(ODRefCntObject* element);  
		// AddFirst() and AddLast acquire element.
	
	void             AddBeforeAndAcquire(
	                    ODRefCntObject* existing, 
	                    ODRefCntObject* tobeadded);
	void             AddAfterAndAcquire(
	                    ODRefCntObject* existing, 
	                    ODRefCntObject* tobeadded);
		// AddBefore() and AddAfter() acquire tobeadded.

	ODRefCntObject*	 After(ODRefCntObject* existing) const;
	ODRefCntObject*	 Before(ODRefCntObject* existing) const;

	ODRefCntObject*	 First() const;
	ODRefCntObject*	 Last() const;
		// Returns kODNULL if there is no first element.

	ODRefCntObject*	 RemoveFirst();
	ODRefCntObject*	 RemoveLast();  
		// Returns kODNULL if there is no remaming elements.
		// Does not release returned element.
		// You must release what RemoveFirst() or RemoveLast() returns.
		
	void             RemoveAndReleaseAll();
		// Called by the destructor: removes and releases all elements.
		
	ODBoolean        Remove(ODRefCntObject* existing); 
		// Returns true if existing was actually removed.
		// Does not release existing. You must eventually release 
		// existing object if it was actually removed.
	
	ODBoolean        RemoveAndRelease(ODRefCntObject* existing);
		// Returns true if existing was actually removed.
		// If actually removed, existing is also released.
		// (So it is unnecessay to call Contains() to decide
		// whether to release.)
	
	ODBoolean        Contains(ODRefCntObject* existing) const;
	
	// ODRefCntCollectionIterator* CreateIterator();
		// Most of the time you will be better off creating an instance
		// of ODRefCntCollectionIterator on the stack, e.g.:
		// ODRefCntCollectionIterator iter(collection);
	
	ODMemoryHeapID GetHeap() const;

private:
	ODObjectOrdColl  fCol;
	Environment*     fEv; 

	friend class ODRefCntCollectionIterator;
};


inline ODMemoryHeapID ODRefCntCollection::GetHeap() const 
{
	return fCol.OrderedCollection::GetHeap( );
}

//------------------------------------------------------------------------------
// ODRefCntCollection::Count
//------------------------------------------------------------------------------

inline ODULong ODRefCntCollection::Count() const
{
	return fCol.OrderedCollection::Count( );
}

//------------------------------------------------------------------------------
// ODRefCntCollection::After
//------------------------------------------------------------------------------

inline ODRefCntObject* ODRefCntCollection::After(ODRefCntObject* existing) const
{
	return (ODRefCntObject*) fCol.OrderedCollection::Before(existing);
}

//------------------------------------------------------------------------------
// ODRefCntCollection::Before
//------------------------------------------------------------------------------

inline ODRefCntObject* ODRefCntCollection::Before(ODRefCntObject* existing) const
{
	return (ODRefCntObject*) fCol.OrderedCollection::Before(existing);
}

//------------------------------------------------------------------------------
// ODRefCntCollection::First
//------------------------------------------------------------------------------

inline ODRefCntObject* ODRefCntCollection::First() const
{
	return (ODRefCntObject*) fCol.OrderedCollection::First();
}

//------------------------------------------------------------------------------
// ODRefCntCollection::Last
//------------------------------------------------------------------------------

inline ODRefCntObject* ODRefCntCollection::Last() const
{
	return (ODRefCntObject*) fCol.OrderedCollection::Last();
}

//------------------------------------------------------------------------------
// ODRefCntCollection::RemoveFirst
//------------------------------------------------------------------------------

inline ODRefCntObject*	ODRefCntCollection::RemoveFirst()
{
	return (ODRefCntObject*) fCol.OrderedCollection::RemoveFirst();
}

//------------------------------------------------------------------------------
// ODRefCntCollection::RemoveLast
//------------------------------------------------------------------------------

inline ODRefCntObject*	ODRefCntCollection::RemoveLast()
{
	return (ODRefCntObject*) fCol.OrderedCollection::RemoveLast();
}

//------------------------------------------------------------------------------
// ODRefCntCollection::Remove
//------------------------------------------------------------------------------

inline ODBoolean ODRefCntCollection::Remove(ODRefCntObject* existing)
	// Returns true if existing was actually removed.
	// Does not release existing. You must eventually release 
	// existing object if it was actually removed.
{
	return fCol.OrderedCollection::Remove(existing);
}

//------------------------------------------------------------------------------
// ODRefCntCollection::Contains
//------------------------------------------------------------------------------

inline ODBoolean ODRefCntCollection::Contains(ODRefCntObject* existing) const
{
	return fCol.OrderedCollection::Contains(existing);
}

//=====================================================================================
// Class OrderedCollectionIterator
//=====================================================================================

class ODRefCntCollectionIterator {
public:
	ODRefCntCollectionIterator(ODRefCntCollection* collection);
	~ODRefCntCollectionIterator();
	ODRefCntObject* First();
	ODRefCntObject* Next();
	ODRefCntObject* Last();
	ODRefCntObject* Previous();
	ODBoolean   IsNotComplete();
	void        RemoveCurrent();
	
private:
  	OrderedCollectionIterator fIter;
};

inline ODRefCntObject* ODRefCntCollectionIterator::First()
{
	return (ODRefCntObject*) fIter.First();
}

inline ODRefCntObject* ODRefCntCollectionIterator::Next()
{
	return (ODRefCntObject*) fIter.Next();
}

inline ODRefCntObject* ODRefCntCollectionIterator::Last()
{
	return (ODRefCntObject*) fIter.Last();
}

inline ODRefCntObject* ODRefCntCollectionIterator::Previous()
{
	return (ODRefCntObject*) fIter.Previous();
}

inline ODBoolean ODRefCntCollectionIterator::IsNotComplete()
{
	return fIter.IsNotComplete();
}

inline void ODRefCntCollectionIterator::RemoveCurrent()
{
	fIter.RemoveCurrent();
}

#endif // _REFCTCOL_
