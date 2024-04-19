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
// @(#) 1.7 com/src/utils/PriortyQ.cpp, odutils, od96os2, odos29646d 7/15/96 17:59:25 [ 11/15/96 15:28:51 ]
/*
	File:		PriortyQ.cpp

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
	
	An abstract mix-in class "Sortable" is provided, which provides a single
	method "ComesBefore" that allows its instances to be sorted.
	Objects in the priority queue must inherit from Sortable.
*/

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#include <ODPagtun.h>
#endif

#ifndef _PRIORTYQ_
#include "PriortyQ.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif


#define PARENT(N)		((N)>>1)		// Parent of a node
#define LEFTCHILD(N)	((N)<<1)		// Left child of a node
#define SIBLING(N)		((N)+1)			// Right sibling of left child


//=============================================================================
// Housekeeping
//=============================================================================


PriorityQueue::PriorityQueue( ODULong suggestedSize /*=0*/ )
	:fHeap(kODNULL),
	 fLast(0)				// Item 0 is not used so start fLast at 0
{
	if( suggestedSize==0 )
		fSize = 32;
	else if( suggestedSize >= 0x10000000 ) {
		WARNMSG_DEBUG(WARN_INDEX(-1),"Ridiculous suggested size %ld",suggestedSize);
		fSize = 128;
	} else
		fSize = suggestedSize;
}


PriorityQueue::~PriorityQueue( )
{
	ODDeleteObject(fHeap);
}


//=============================================================================
// Insertion
//=============================================================================


void
PriorityQueue::Add( const Sortable *s )
{
	ASSERT(s!=kODNULL, kODErrIllegalNullInput);
	
	// Create or grow the heap if necessary:
	if( fHeap==kODNULL || fLast+1>=fSize ) {
		ODULong newSize = fSize;
		if( fHeap )
			newSize <<= 1;			// Grow by a factor of 2
		const Sortable* *newHeap = new const Sortable* [newSize];
		newHeap[0] = kODNULL;
		ODBlockMove(&fHeap[1],&newHeap[1], fLast*sizeof(Sortable*));
		if( fHeap ) delete fHeap;
		fHeap = newHeap;
		fSize = newSize;
	}
	
	// Standard heap insertion: (Sedgewick p.150)
	ODULong k = ++fLast;								// 1st item is at 1!
	while( k>1 && s->ComesBefore(fHeap[PARENT(k)]) ) {
		fHeap[k] = fHeap[PARENT(k)];
		k = PARENT(k);
	}
	fHeap[k] = s;
}


//=============================================================================
// Accessing
//=============================================================================


Sortable*
PriorityQueue::GetFirst( ) const
{
	if( fLast==0 )
		return kODNULL;
	else
		return (Sortable*)fHeap[1];	// Cast to non-const so client can modify!
}


//=============================================================================
// Deletion
//=============================================================================


Sortable*
PriorityQueue::RemoveFirst( )
{
	if( fLast==0 )
		return kODNULL;
	else {
		const Sortable *first = fHeap[1];
		
		// Now remove the first element by moving the last atop it and sorting:
		const Sortable *v = fHeap[fLast--];
		ODULong k = 1;
		ODULong j;
		while( k <= (fLast>>1) ) {
			j = LEFTCHILD(k);
			if( j<fLast && fHeap[SIBLING(j)]->ComesBefore(fHeap[j]) )
				j = SIBLING(j);
			if( v->ComesBefore(fHeap[j]) )
				break;
			fHeap[k] = fHeap[j];
			k = j;
		}
		fHeap[k] = v;
		
		return (Sortable*)first;	// Cast to non-const so client can modify!
	}
}


void
PriorityQueue::DeleteAll( )
{
	// This function does violate the constness of the contained objects
	// (by deleting them) but this is often useful to do...
	
	for( ODULong i=1; i<=fLast; i++ )
		delete (Sortable*) fHeap[i];
	fLast = 0;
}
