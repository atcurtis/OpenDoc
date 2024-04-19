//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   OrderedCollection
//		OrderedCollectionIterator
//		ValueLink
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
// @(#) 1.3 com/src/utils/include/OrdColl.h, odutils, od96os2, odos29646d 7/15/96 18:01:29 [ 11/15/96 15:29:33 ]
/*
	File:		OrdColl.h

	Contains:	Definition of class OrderedCollection

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _ORDCOLL_
#define _ORDCOLL_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

// OrdereCollection is an ordered collection of elements of type void* (since
// we can't use templates)
// Duplicates are allowed.

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

typedef void* ElementType;

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class OrderedCollection;	// An ordered (not sorted) collection of ElementTypes
class OrderedCollectionIterator;

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class ValueLink; 			// A link plus a value of type ElementType.

//=====================================================================================
// Global Variables
//=====================================================================================

//=====================================================================================
// Class ValueLink - Definition
//=====================================================================================

class ValueLink : public Link {
	
public:
							ValueLink(ElementType value);		
	ODVMethod 			   ~ValueLink();
	ODNVMethod	ElementType	GetValue()						{ return fValue;}
	ODNVMethod void		SetValue(ElementType v)			{ fValue = v;}

private:
	ElementType 		fValue;
};

//=====================================================================================
// Class OrderedCollection
//=====================================================================================

class OrderedCollection
{
	
public:

	OrderedCollection();
	OrderedCollection(ODMemoryHeapID where);
	virtual ~OrderedCollection();

	ODNVMethod ODULong Count() const 			{ return fImplementation.Count(); };
	
	ODVMethod void	AddFirst(ElementType element);
	ODVMethod void	AddLast(ElementType element);
	ODVMethod void	AddBefore(ElementType existing, ElementType tobeadded);
	ODVMethod void	AddAfter(ElementType existing, ElementType tobeadded);

	ODVMethod ElementType	After(ElementType existing) const;
	ODVMethod ElementType	Before(ElementType existing) const;

	ODVMethod ElementType	First() const;
		// Returns kODNULL if there is no first element.
	ODVMethod ElementType	Last() const;

	ODVMethod ElementType	RemoveFirst();
		// Don't call if there are no elements. Crash will result.
	ODVMethod ElementType	RemoveLast();
	ODVMethod void	RemoveAll();
	
		// Called from the destructor. Removes all elements, deleting the links
		// Does not delete the elements themselves
		
	ODVMethod void	DeleteAll();
	
		// Removes and deletes all elements
		
	ODVMethod ODBoolean	Remove(ElementType existing);
		// Returns true if existing was actually removed.
		
	ODVMethod ODBoolean	Contains(ElementType existing) const;
	
	ODVMethod OrderedCollectionIterator* CreateIterator();
	
	ODNVMethod ODMemoryHeapID GetHeap() const;

protected:
	 ODVMethod ValueLink* 	CreateNewLink(ElementType value) const;
	 ODVMethod ODBoolean	ElementsMatch(ElementType v1,ElementType v2) const;
	 	// Does a pointer comparison by default 

private:
	LinkedList		fImplementation;
	ODMemoryHeapID	fHeap; // if kODNULL, use default heap.

	friend class OrderedCollectionIterator;
	friend class ListIterator;
};

inline ODMemoryHeapID OrderedCollection::GetHeap() const 
{
	return fHeap;
}


//=====================================================================================
// Class OrderedCollectionIterator
//=====================================================================================

class OrderedCollectionIterator {
public:
	OrderedCollectionIterator(OrderedCollection* collection);
	~OrderedCollectionIterator();
	ElementType First();
	ElementType Next();
	ElementType Last();
	ElementType Previous();
	ODBoolean   IsNotComplete();
	void        RemoveCurrent();
	
private:
  	OrderedCollection*	fCollection;
	LinkedListIterator	fImplementation;
};

#endif // _ORDCOLL_
