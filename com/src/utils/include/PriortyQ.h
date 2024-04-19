//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   PriorityQueue
//		Sortable
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
// @(#) 1.3 com/src/utils/include/PriortyQ.h, odutils, od96os2, odos29646d 7/15/96 18:01:52 [ 11/15/96 15:29:02 ]
/*
	File:		PriortyQ.h

	Contains:	Priority-Queue data structure.

	Owned by:	Jens Alfke

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Theory of Operation:
	
	A Priority Queue is a sorted collection that allows entries to be added to
	the queue in arbitrary order, and allows the first (in sort order) entry
	-- and only the first entry -- to be viewed and removed. Its implementation
	makes it very efficient, using a partially sorted data structure called a
	heap. Any standard algorithms textbook will describe this; see Sedgewick's
	"Algorithms in C++", ch.11.
*/


#ifndef _PRIORTYQ_
#define _PRIORTYQ_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif


//=============================================================================
// Queue-element mix-in class
//=============================================================================

// Objects you put in the queue must inherit from this abstract mix-in class.

class Sortable {
	public:
		ODVMethod ODBoolean	ComesBefore( const Sortable* ) const
			=0;
};


//=============================================================================
// PriorityQueue
//=============================================================================


class PriorityQueue {
	public:
		PriorityQueue( ODULong suggestedSize =0 );
		virtual ~PriorityQueue( );
		
		ODMethod	void		Add( const Sortable* );
		ODMethod	Sortable*	GetFirst( ) const;
		ODMethod	Sortable*	RemoveFirst( );
		inline		void		RemoveAll( )		{fLast = 0;}
		ODMethod	void		DeleteAll( );
		
		inline		ODBoolean	IsEmpty( ) const	{return fLast==0;}
		
	private:
		ODULong		fSize;
		ODULong		fLast;
		const Sortable*	*fHeap;		// Points to array of Sortable*s
};

#endif /*_PRIORTYQ_*/
