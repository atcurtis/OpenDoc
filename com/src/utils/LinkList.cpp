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
// @(#) 1.5 com/src/utils/LinkList.cpp, odutils, od96os2, odos29646d 7/15/96 17:58:27 [ 11/15/96 15:28:45 ]
/*
	File:		LinkList.cpp

	Contains:	Primitive linked list class

	Owned by:	Richard Rodseth, Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#pragma segment ODLinkedList

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

//==============================================================================
// Local Classes
//==============================================================================

//==============================================================================
// Global Variables
//==============================================================================

//==============================================================================
// Function Prototype
//==============================================================================


//==============================================================================
// Link
//==============================================================================



// Many of the simple link methods are inlines; see List.h for implementations.


//------------------------------------------------------------------------------
// Link::Link
//
// Constructor for Link
//------------------------------------------------------------------------------

Link::Link()							
{ 
	fNext = kODNULL; 
	fPrevious = kODNULL;
}

//------------------------------------------------------------------------------
// Link::Link
//
// Constructor for Link
//------------------------------------------------------------------------------

Link::Link(Link* next, Link* previous)							
{ 
	fNext = next; 
	fPrevious = previous;
}

//------------------------------------------------------------------------------
// Link::Link
//
// Copy constructor for Link
//------------------------------------------------------------------------------

Link::Link( const Link &link )							
{ 
	fNext = link.fNext; 
	fPrevious = link.fPrevious;
}

//------------------------------------------------------------------------------
// Link::Link
//
// Destructor for Link
//------------------------------------------------------------------------------

Link::~Link()											
{
}

//------------------------------------------------------------------------------
// Link::Remove
//
// Remove a link from its list (if any). DO NOT call this directly if there are
// any iterators active on the list; use LinkedList::Remove instead.
//------------------------------------------------------------------------------

void	Link::Remove( )
{
	if( fPrevious )
		fPrevious->SetNext(fNext);
	if( fNext )
		fNext->SetPrevious(fPrevious);
	fNext = kODNULL;
	fPrevious = kODNULL;
}

//------------------------------------------------------------------------------
// Link::AddBefore
//
// Add a link to a list before another link. It must not already be on any list.
// DO NOT call this directly if there are any iterators active on the list;
// use LinkedList::Remove instead.
//------------------------------------------------------------------------------

void	Link::AddBefore( Link *link )
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
// Link::AddAfter
//
// Add a link to a list after another link. It must not already be on any list.
// DO NOT call this directly if there are any iterators active on the list;
// use LinkedList::Remove instead.
//------------------------------------------------------------------------------

void	Link::AddAfter( Link *link )
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
// Class LinkedList
//======================================================================================

//------------------------------------------------------------------------------
// LinkedList::LinkedList
//
// Constructor for LinkedList
//------------------------------------------------------------------------------

LinkedList::LinkedList()
	:fSentinel(&fSentinel,&fSentinel),
	 fSeed(0)
{
}

//------------------------------------------------------------------------------
// LinkedList::~LinkedList
//
// Destructor for LinkedList
//------------------------------------------------------------------------------

LinkedList::~LinkedList()
{
	// The list does NOT delete all its links!
}

//------------------------------------------------------------------------------
// LinkedList::IsEmpty
//
// Description
//------------------------------------------------------------------------------

ODBoolean LinkedList::IsEmpty() const
{
	return this->IsSentinel( fSentinel.GetNext() );
}

//------------------------------------------------------------------------------
// LinkedList::Count
//
// Description
//------------------------------------------------------------------------------

ODULong LinkedList::Count() const
{
	Link* l = fSentinel.GetNext();
	ODULong count = 0;
	while (this->NotSentinel(l))
	{
		count++;
		l = l->GetNext();
		ASSERT(l!=kODNULL,kODErrAssertionFailed);
	}
	return count;
}

//------------------------------------------------------------------------------
// LinkedList::Includes
//
// Does the list include a particular link?
//------------------------------------------------------------------------------

ODBoolean LinkedList::Includes( const Link *link ) const
{
	Link* l = fSentinel.GetNext();
	while (this->NotSentinel(l))
	{
		if( l == link )
			return kODTrue;
		else
			l = l->GetNext();
	}
	return kODFalse;
}

//------------------------------------------------------------------------------
// LinkedList::DeleteAllLinks
//
// Description
//------------------------------------------------------------------------------

void	LinkedList::DeleteAllLinks()
{

	Link* l = fSentinel.GetNext();
	Link* n = l;
	while (this->NotSentinel(n))
	{
		n = l->GetNext();
		delete l;	
		l = n;
	}
	fSentinel.SetNext(&fSentinel);
	fSentinel.SetPrevious(&fSentinel);
	fSeed++;
}

//------------------------------------------------------------------------------
// LinkedList::RemoveAll
//
// Description
//------------------------------------------------------------------------------

void	LinkedList::RemoveAll()
{

	Link* l = fSentinel.GetNext();
	Link* n = l;
	while (this->NotSentinel(l))
	{
		n = l->GetNext();
		l->SetNext(kODNULL);
		l->SetPrevious(kODNULL);
		l = n;
	}
	fSentinel.SetNext(&fSentinel);
	fSentinel.SetPrevious(&fSentinel);
}

//------------------------------------------------------------------------------
// LinkedList::Remove
//
// Description
//------------------------------------------------------------------------------

void	LinkedList::Remove(Link& aLink)
{
	fSeed++;
	aLink.Remove();
}

//------------------------------------------------------------------------------
// LinkedList::RemoveFirst
//
// Description
//------------------------------------------------------------------------------

Link* LinkedList::RemoveFirst()
{
	Link* old = fSentinel.GetNext();
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
// LinkedList::RemoveLast
//
// Description
//------------------------------------------------------------------------------

Link* LinkedList::RemoveLast()
{
	Link* old = fSentinel.GetPrevious();
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
// LinkedList::AddFirst
//
// Description
//------------------------------------------------------------------------------

void	LinkedList::AddFirst(Link* link)
{
	link->AddAfter(this->GetSentinel());
	fSeed++;
}

//------------------------------------------------------------------------------
// LinkedList::AddLast( Link )
//
// Description
//------------------------------------------------------------------------------

void	LinkedList::AddLast(Link* link)
{
	link->AddBefore(this->GetSentinel());
	fSeed++;
}

//------------------------------------------------------------------------------
// LinkedList::AddLast( LinkedList )
//
// Append contents of another list to my end. (Other list becomes empty.)
//------------------------------------------------------------------------------

void	LinkedList::AddLast( LinkedList &list )
{
	if( !list.IsEmpty() ) {
		Link *myLast = fSentinel.GetPrevious();
		Link *itsFirst = list.First();
		myLast->SetNext(itsFirst);
		itsFirst->SetPrevious(myLast);
		
		Link *itsLast = list.Last();
		itsLast->SetNext(this->GetSentinel());
		fSentinel.SetPrevious(itsLast);
		
		list.fSentinel.SetNext(this->GetSentinel());
		list.fSentinel.SetPrevious(this->GetSentinel());

		fSeed++;
		list.fSeed++;
	}
}

//------------------------------------------------------------------------------
// LinkedList::AddLastUnique( LinkedList )
//
// Append contents of another list (w/o duplication) to my end.
//------------------------------------------------------------------------------

void	LinkedList::AddLastUnique( LinkedList &list )
{
	for( Link *link = fSentinel.GetNext(); this->NotSentinel(link); link=link->GetNext() )
		if( list.Includes(link) )
			list.Remove(*link);
	this->AddLast(list);
}

//------------------------------------------------------------------------------
// LinkedList::AddBefore
//
// Description
//------------------------------------------------------------------------------

void LinkedList::AddBefore(Link& existing, Link* link)
{
	link->AddBefore(&existing);
	fSeed++;
}

//------------------------------------------------------------------------------
// LinkedList::AddAfter
//
// Description
//------------------------------------------------------------------------------

void LinkedList::AddAfter(Link& existing, Link* link)
{
	link->AddAfter(&existing);
	fSeed++;
}

//------------------------------------------------------------------------------
// LinkedList::After
//
// Description
//------------------------------------------------------------------------------

Link* LinkedList::After(const Link& link) const
{
	Link *next = link.GetNext();
	if( this->IsSentinel(next) )
		return kODNULL;
	else
		return next;
}

//------------------------------------------------------------------------------
// LinkedList::Before
//
// Description
//------------------------------------------------------------------------------

Link* LinkedList::Before(const Link& link) const
{
	Link *prev = link.GetPrevious();
	if( this->IsSentinel(prev) )
		return kODNULL;
	else
		return prev;
}

//------------------------------------------------------------------------------
// LinkedList::First
//
// Description
//------------------------------------------------------------------------------

Link*	LinkedList::First()  const
{
	return this->NotSentinel(fSentinel.GetNext()) ? fSentinel.GetNext() : (Link*) kODNULL;
}

//------------------------------------------------------------------------------
// LinkedList::Last
//
// Description
//------------------------------------------------------------------------------

Link* LinkedList::Last() const
{
	return this->NotSentinel(fSentinel.GetPrevious()) ? fSentinel.GetPrevious() : (Link*) kODNULL;
}


//======================================================================================
// Class LinkedListIterator
//======================================================================================

//------------------------------------------------------------------------------
// LinkedListIterator::LinkedListIterator
//
// Constructor for LinkedListIterator
//------------------------------------------------------------------------------

LinkedListIterator::LinkedListIterator(LinkedList* list)
{
	fList = list;
	fCurrent = kODNULL;
	fNext = kODNULL;
	fPrevious = kODNULL;
	fSentinel = &list->fSentinel;
	fSeed = fList->fSeed;	
}

//------------------------------------------------------------------------------
// LinkedListIterator::~LinkedListIterator
//
// Destructor for LinkedListIterator
//------------------------------------------------------------------------------

LinkedListIterator::~LinkedListIterator()
{
}

//------------------------------------------------------------------------------
// LinkedListIterator::First
//
// Description
//------------------------------------------------------------------------------

Link* LinkedListIterator::First()
{
	if (fList == kODNULL)
		return kODNULL;
	
#ifdef _PLATFORM_MACINTOSH_	
	if (fSeed != fList->fSeed)
		THROW(kODErrIteratorOutOfSync);
#else
	// re-initialize for a new run
	fNext = kODNULL;
	fPrevious = kODNULL;
	fSentinel = &fList->fSentinel;
	fSeed = fList->fSeed;
#endif	
		
	fCurrent = fList->First();
	if (fCurrent == fSentinel)
		fCurrent = kODNULL;
	return fCurrent;
}

//------------------------------------------------------------------------------
// LinkedListIterator::Next
//
// Description
//------------------------------------------------------------------------------

Link* LinkedListIterator::Next()
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
// LinkedListIterator::Last
//
// Description
//------------------------------------------------------------------------------

Link* LinkedListIterator::Last()
{
	if (fList == kODNULL)
		return kODNULL;

#ifdef _PLATFORM_MACINTOSH_	
	if (fSeed != fList->fSeed)
		THROW(kODErrIteratorOutOfSync);
#else
	// re-initialize for a new run
	fNext = kODNULL;
	fPrevious = kODNULL;
	fSentinel = &fList->fSentinel;
	fSeed = fList->fSeed;
#endif	

	fCurrent = fList->Last();
	if (fCurrent == fSentinel)
		fCurrent = kODNULL;
	return fCurrent;
}

//------------------------------------------------------------------------------
// LinkedListIterator::Previous
//
// Description
//------------------------------------------------------------------------------

Link* LinkedListIterator::Previous()
{
	if (fList == kODNULL)
		return kODNULL;

	if (fSeed != fList->fSeed)
		THROW(kODErrIteratorOutOfSync);

	if (fCurrent == kODNULL)
	{
		if ((fNext == kODNULL) && (fPrevious == kODNULL))	// Just starting out
		{
			return this->Last();
		}
		else	// Just deleted
		{
			fCurrent = fPrevious;
			fPrevious = kODNULL;
			fNext = kODNULL;
		}
	}
	else
		fCurrent = fCurrent->GetPrevious();

	if (fCurrent == fSentinel)
		fCurrent = kODNULL;
	return fCurrent;
}

//------------------------------------------------------------------------------
// LinkedListIterator::Current
//
// Description
//------------------------------------------------------------------------------

Link* LinkedListIterator::Current()
{
	return fCurrent;
}

//------------------------------------------------------------------------------
// LinkedListIterator::IsNotComplete
//
// Description
//------------------------------------------------------------------------------

ODBoolean LinkedListIterator::IsNotComplete()
{
	return (fCurrent != kODNULL);
}

//------------------------------------------------------------------------------
// LinkedListIterator::RemoveCurrent
//
// Description
//------------------------------------------------------------------------------

void  LinkedListIterator::RemoveCurrent()
{
	if (fList == kODNULL)
		return;
		
	if (fSeed != fList->fSeed)
		THROW(kODErrIteratorOutOfSync);
	 	
	if (fCurrent != kODNULL)
	{
		fNext = fCurrent->GetNext();
		fPrevious = fCurrent->GetPrevious();
			
		fList->Remove(*fCurrent);
		fCurrent = kODNULL;
		fSeed = fList->fSeed;
	}
}
