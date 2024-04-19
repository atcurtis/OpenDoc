//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.5 com/src/utils/OrdColl.cpp, odutils, od96os2, odos29646d 7/15/96 17:59:12 [ 11/15/96 15:28:51 ]
/*
	File:		OrdColl.cpp

	Contains:	Implementation of OrderedCollection and OrderedCollectionIterator

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/


#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#include <ODPagtun.h>
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#pragma segment ODCollections

//======================================================================================
// Class ValueLink - Implementation
//======================================================================================

ValueLink::ValueLink(ElementType value)
{
	fValue = value;
}

ValueLink::~ValueLink()
{
}

//======================================================================================
// Class OrderedCollection
//======================================================================================

//------------------------------------------------------------------------------
// OrderedCollection::OrderedCollection
//------------------------------------------------------------------------------

OrderedCollection::OrderedCollection()
{
	fHeap = kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollection::OrderedCollection
//------------------------------------------------------------------------------

OrderedCollection::OrderedCollection(ODMemoryHeapID where)
{
	fHeap = where;
}

// OrderedCollection::~OrderedCollection
//------------------------------------------------------------------------------

OrderedCollection::~OrderedCollection()
{
	this->RemoveAll();
}

//------------------------------------------------------------------------------
// OrderedCollection::AddFirst
//------------------------------------------------------------------------------

void OrderedCollection::AddFirst(ElementType element)
{
	ValueLink* newLink = this->CreateNewLink(element);
	fImplementation.AddFirst(newLink);
}

//------------------------------------------------------------------------------
// OrderedCollection::AddLast
//------------------------------------------------------------------------------

void OrderedCollection::AddLast(ElementType element)
{
	ValueLink* newLink = CreateNewLink(element);
	fImplementation.AddLast(newLink);
}


//------------------------------------------------------------------------------
// OrderedCollection::AddBefore
//------------------------------------------------------------------------------

void OrderedCollection::AddBefore(ElementType existing, ElementType tobeadded)
{
	LinkedList* impl = (LinkedList*) &fImplementation; // cast away const
	LinkedListIterator iter(impl);
	ValueLink* aLink = (ValueLink*) iter.First();
	while (aLink != kODNULL)
	{
		ElementType v = ((ValueLink*) aLink)->GetValue();

		if (this->ElementsMatch(v,existing))	
		{
			ValueLink* newLink = CreateNewLink(tobeadded);
			fImplementation.AddBefore(*aLink, newLink);
			aLink = kODNULL;
		}
		else
			aLink = (ValueLink*) iter.Next();
	}
}

//------------------------------------------------------------------------------
// OrderedCollection::AddAfter
//------------------------------------------------------------------------------

void OrderedCollection::AddAfter(ElementType existing, ElementType tobeadded)
{
	LinkedList* impl = (LinkedList*) &fImplementation; // cast away const
	LinkedListIterator iter(impl);
	ValueLink* aLink = (ValueLink*) iter.First();
	while (aLink != kODNULL)
	{
		ElementType v = ((ValueLink*) aLink)->GetValue();

		if (this->ElementsMatch(v,existing))	
		{
			ValueLink* newLink = CreateNewLink(tobeadded);
			fImplementation.AddAfter(*aLink, newLink);
			aLink = kODNULL;
		}
		else
			aLink = (ValueLink*) iter.Next();
	}
}

//------------------------------------------------------------------------------
// OrderedCollection::After
//------------------------------------------------------------------------------

ElementType OrderedCollection::After(ElementType existing) const
{
	ValueLink* linkAfter = kODNULL;

	LinkedList* impl = (LinkedList*) &fImplementation; // cast away const
	LinkedListIterator iter(impl);

	for (ValueLink* link = (ValueLink*) iter.First(); iter.IsNotComplete(); link = (ValueLink*) iter.Next())
	{
		ElementType v = ((ValueLink*) link)->GetValue();

		if (this->ElementsMatch(v,existing))	
		{
			linkAfter = (ValueLink*) fImplementation.After(*link);
			break;
		}
	}
	
	return linkAfter ? linkAfter->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollection::Before
//------------------------------------------------------------------------------

ElementType OrderedCollection::Before(ElementType existing) const
{
	ValueLink* linkBefore = kODNULL;

	LinkedList* impl = (LinkedList*) &fImplementation; // cast away const
	LinkedListIterator iter(impl);

	for (ValueLink* link = (ValueLink*) iter.First(); iter.IsNotComplete(); link = (ValueLink*) iter.Next())
	{
		ElementType v = ((ValueLink*) link)->GetValue();

		if (this->ElementsMatch(v,existing))	
		{
			linkBefore = (ValueLink*) fImplementation.Before(*link);
			break;
		}
	}
	
	return linkBefore ? linkBefore->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollection::First
//------------------------------------------------------------------------------

ElementType OrderedCollection::First() const
{
	ValueLink* firstLink = (ValueLink*) fImplementation.First();
	return firstLink ? firstLink->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollection::Last
//------------------------------------------------------------------------------

ElementType OrderedCollection::Last() const
{
	ValueLink* lastLink = (ValueLink*) fImplementation.Last();
	return lastLink ? lastLink->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollection::RemoveFirst
//------------------------------------------------------------------------------

ElementType	OrderedCollection::RemoveFirst()
{
	ValueLink* aLink = (ValueLink*) fImplementation.RemoveFirst();
	ElementType value = aLink ? aLink->GetValue() : kODNULL;
	delete aLink;
	return value;
}

//------------------------------------------------------------------------------
// OrderedCollection::RemoveLast
//------------------------------------------------------------------------------

ElementType	OrderedCollection::RemoveLast()
{
	ValueLink* aLink = (ValueLink*) fImplementation.RemoveLast();
	ElementType value = aLink ? aLink->GetValue() : kODNULL;
	delete aLink;
	return value;
}

//------------------------------------------------------------------------------
// OrderedCollection::Remove
//------------------------------------------------------------------------------

ODBoolean OrderedCollection::Remove(ElementType existing)
{
	LinkedList* impl = (LinkedList*) &fImplementation; // cast away const
	LinkedListIterator iter(impl);
	ValueLink* aLink = (ValueLink*) iter.First();
	ODBoolean removed = kODFalse;
	while (aLink != kODNULL)
	{
		ElementType v = ((ValueLink*) aLink)->GetValue();

		if (this->ElementsMatch(v,existing))
		{
			fImplementation.Remove(*aLink);
			delete aLink;
			removed = kODTrue;
			aLink = kODNULL;	
		}
		else
			aLink = (ValueLink*) iter.Next();
	}
	return removed;
}

//------------------------------------------------------------------------------
// OrderedCollection::RemoveAll
//------------------------------------------------------------------------------

void OrderedCollection::RemoveAll()
{
	Link* link = fImplementation.RemoveFirst();
	while (link != kODNULL)
	{
		delete link;
		link = fImplementation.RemoveFirst();
	}
}

//------------------------------------------------------------------------------
// OrderedCollection::DeleteAll
//------------------------------------------------------------------------------

void OrderedCollection::DeleteAll()
{
	Link* link = fImplementation.RemoveFirst();
	while (link != kODNULL)
	{
		ElementType value = ((ValueLink*) link)->GetValue();
		delete value;
		delete link;
		link = fImplementation.RemoveFirst();
	}
}

//------------------------------------------------------------------------------
// OrderedCollection::Contains
//------------------------------------------------------------------------------

ODBoolean	OrderedCollection::Contains(ElementType existing) const
{
	LinkedList* impl = (LinkedList*) &fImplementation; // cast away const
	LinkedListIterator iter(impl);
	ValueLink* aLink = (ValueLink*) iter.First();
	while (aLink != kODNULL)
	{
		ElementType v = ((ValueLink*) aLink)->GetValue();

		if (this->ElementsMatch(v,existing))
		{
			return kODTrue;	
		}
		else
			aLink = (ValueLink*) iter.Next();
	}	
	return kODFalse;
}

//------------------------------------------------------------------------------
// OrderedCollection::CreateIterator
//------------------------------------------------------------------------------

OrderedCollectionIterator* OrderedCollection::CreateIterator()
{
	return new(fHeap) OrderedCollectionIterator(this);
}


//------------------------------------------------------------------------------
// OrderedCollection::CreateNewLink
//------------------------------------------------------------------------------

ValueLink*	OrderedCollection::CreateNewLink(ElementType value) const
{
	return new (fHeap) ValueLink(value);
}

//------------------------------------------------------------------------------
// OrderedCollection::ElementsMatch
//------------------------------------------------------------------------------

ODBoolean	OrderedCollection::ElementsMatch(ElementType v1,ElementType v2) const
{
	return (v1 == v2);
}

//======================================================================================
// OrderedCollectionIterator
//======================================================================================

//------------------------------------------------------------------------------
// OrderedCollectionIterator::OrderedCollectionIterator
//------------------------------------------------------------------------------

OrderedCollectionIterator::OrderedCollectionIterator(OrderedCollection* collection)	
	: fImplementation(collection ? &(collection->fImplementation) : (LinkedList*)kODNULL)
{
	fCollection =  collection;
}

//------------------------------------------------------------------------------
// OrderedCollectionIterator::~OrderedCollectionIterator
//------------------------------------------------------------------------------

OrderedCollectionIterator::~OrderedCollectionIterator()						
{
}

//------------------------------------------------------------------------------
// OrderedCollectionIterator::First
//------------------------------------------------------------------------------

ElementType	OrderedCollectionIterator::First()
{
	ValueLink* link = (ValueLink*) fImplementation.First();
	
	return link ? link->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollectionIterator::Next
//------------------------------------------------------------------------------

ElementType	OrderedCollectionIterator::Next()
{		
	ValueLink* link = (ValueLink*) fImplementation.Next();
	
	return link ? link->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollectionIterator::Last
//------------------------------------------------------------------------------

ElementType	OrderedCollectionIterator::Last()
{
	ValueLink* link = (ValueLink*) fImplementation.Last();
	
	return link ? link->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollectionIterator::Previous
//------------------------------------------------------------------------------

ElementType	OrderedCollectionIterator::Previous()
{
	ValueLink* link = (ValueLink*) fImplementation.Previous();
	
	return link ? link->GetValue() : (ElementType)kODNULL;
}

//------------------------------------------------------------------------------
// OrderedCollectionIterator::IsNotComplete
//------------------------------------------------------------------------------

ODBoolean OrderedCollectionIterator::IsNotComplete()
{
	return fImplementation.IsNotComplete();
}


//------------------------------------------------------------------------------
// OrderedCollectionIterator::RemoveCurrent
//------------------------------------------------------------------------------

void OrderedCollectionIterator::RemoveCurrent()
{
	fImplementation.RemoveCurrent();
}







