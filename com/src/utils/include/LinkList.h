//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   Link
//		LinkedList
//		LinkedListIterator
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
// @(#) 1.3 com/src/utils/include/LinkList.h, odutils, od96os2, odos29646d 7/15/96 18:00:37 [ 11/15/96 15:29:32 ]
/*
	File:		LinkList.h

	Contains:	Primitive linked list class

	Owned by:	Richard Rodseth and Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _LINKLIST_
#define _LINKLIST_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


//=====================================================================================
// Theory of Operation
//=====================================================================================

/*

 Note: These classes are private to the implementation. They are not available
 to part handlers.

 Note: These are primitive classes for implementing higher-level collections
 For example, to create a FrameList class, subclass Link to add a field to
 store the frame. The FrameList class would use a LinkedList in its implementation
 and would manufacture the Link objects internally.
 
*/

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

//==============================================================================
// Classes defined in this interface
//==============================================================================

class Link;
class LinkedList;
class LinkedListIterator;

//==============================================================================
// Classes used by this interface
//==============================================================================


//==============================================================================
// Link
//==============================================================================

class  Link {
	public:
	
		Link();
		
		Link(Link* next, Link* previous);
		
		Link( const Link& );
		
		virtual ~Link();
		
		Link* GetNext() const					{return fNext;}
		
		Link* GetPrevious() const				{return fPrevious;}
	
	// The following operations are provided for efficiency, but DO NOT USE THEM
	// if there are any iterators active on a list. These operations don't bump
	// the list's fSeed and the iterators will not be able to detect that they
	// are out of sync!
		
		void  Remove( );
		
		void  AddBefore( Link *aLink );
		
		void  AddAfter( Link *aLink );
	
  //private-by-convention:
  		
		void  SetNext(Link* aLink)				{fNext = aLink;}
		
		void  SetPrevious(Link* aLink)			{fPrevious = aLink;}

	private:
	
		Link*		fNext;
		Link*		fPrevious;
};


//==============================================================================
// LinkedList
//==============================================================================


class LinkedList {

	public:
	
	  	LinkedList();
	  	
	  	~LinkedList();
	  	
	  	ODBoolean		IsEmpty( )										const;
	  	
	  	ODULong			Count()											const;
	  	
	  	ODBoolean		Includes( const Link* )							const;
	  	
	  	void			Remove(Link&);
	  	
	  	void			RemoveAll();

	  	void			DeleteAllLinks();
	  	
	  	Link*			RemoveFirst();
	  	
	  	Link*			RemoveLast();
	  	
	  	void			AddBefore(Link& existing, Link* link);
	  	
	  	void			AddAfter(Link& existing, Link* link);
	  	
	  	void			AddFirst(Link* link);
	  	
	  	void			AddLast(Link* link);
	  	
	  	void			AddLast( LinkedList &list );
	  	
	  	void			AddLastUnique( LinkedList &list );
	  	
	  	Link*			After(const Link& link)							const;
	  	
	  	Link* 			Before(const Link& link)						const;
	  	
	  	Link*			First()											const;
	  	
	  	Link* 			Last()											const;

protected:

		Link			fSentinel;	// Marks the head & tail
		ODULong			fSeed;		// Used to detect out-of-sync iterators
		
		Link*			GetSentinel( )
												{return &fSentinel;}
		const Link*		GetSentinel( )									const
												{return &fSentinel;}
		ODBoolean		IsSentinel( const Link* link )					const
												{return link==this->GetSentinel();}
		ODBoolean		NotSentinel( const Link* link )					const
												{return link!=this->GetSentinel();}

private:			  	
		friend class LinkedListIterator;
};


//=====================================================================================
// LinkedListIterator
//=====================================================================================

class LinkedListIterator {

	public:
	
		LinkedListIterator(LinkedList*	list);
		
		~LinkedListIterator();
		
		Link*			First();
		
		Link*			Next();
		
		Link*			Last();
		
		Link*			Previous();

		Link*			Current();

		ODBoolean 		IsNotComplete();
		
		void			RemoveCurrent();
		
	private:
	
		LinkedList*		fList;
		Link*			fCurrent;
		Link*			fNext;		// Used only when deleting while iterating
		Link* 			fPrevious;	// Used only when deleting while iterating
		Link*			fSentinel;
		ODULong			fSeed;		// Used to detect out-of-sync iterators
		
};


#endif // _LINKLIST_
