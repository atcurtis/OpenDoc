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
// @(#) 1.4 com/src/utils/RefCtCol.cpp, odutils, od96os2, odos29646d 7/15/96 17:59:29 [ 11/15/96 15:28:51 ]
/*
	File:		RefCtCol.cpp

	Contains:	ODRefCntCollection of ODRefCntObject

	Owned by:	David McCusker

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	 

*/

#ifndef _REFCTCOL_
#include "RefCtCol.h"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#pragma segment ODCollections

//------------------------------------------------------------------------------
// ODObjectOrdColl::^ODObjectOrdColl
//------------------------------------------------------------------------------

ODObjectOrdColl::~ODObjectOrdColl()
{
}

//------------------------------------------------------------------------------
// ODObjectOrdColl::ElementsMatch (OVERRIDE)
//------------------------------------------------------------------------------

ODBoolean ODObjectOrdColl::ElementsMatch(ElementType v1,ElementType v2) const
{
	Environment* ev = somGetGlobalEnvironment();
	return ODObjectsAreEqual(ev, (ODObject*) v1, (ODObject*) v2);
}

//======================================================================================
// Class ODRefCntCollection
//======================================================================================

//------------------------------------------------------------------------------
// ODRefCntCollection::ODRefCntCollection
//------------------------------------------------------------------------------

ODRefCntCollection::ODRefCntCollection(Environment* ev)
	: fCol( ), fEv( ev )
{
}

//------------------------------------------------------------------------------
// ODRefCntCollection::ODRefCntCollection
//------------------------------------------------------------------------------


ODRefCntCollection::ODRefCntCollection(Environment* ev, ODMemoryHeapID where)
	: fCol( where ), fEv( ev )
{
}

// ODRefCntCollection::~ODRefCntCollection
//------------------------------------------------------------------------------

ODRefCntCollection::~ODRefCntCollection()
{
	this->RemoveAndReleaseAll();
}

//------------------------------------------------------------------------------
// ODRefCntCollection::AddFirst
//------------------------------------------------------------------------------

void ODRefCntCollection::AddFirstAndAcquire(ODRefCntObject* element)
{
	fCol.OrderedCollection::AddLast(element);
	ODAcquireObject(fEv, element); // function, not macro
}

//------------------------------------------------------------------------------
// ODRefCntCollection::AddLast
//------------------------------------------------------------------------------

void ODRefCntCollection::AddLastAndAcquire(ODRefCntObject* element)
{
	fCol.OrderedCollection::AddLast(element);
	ODAcquireObject(fEv, element); // function, not macro
}

//------------------------------------------------------------------------------
// ODRefCntCollection::AddBefore
//------------------------------------------------------------------------------

void ODRefCntCollection::AddBeforeAndAcquire(ODRefCntObject* existing, ODRefCntObject* tobeadded)
{
	fCol.OrderedCollection::AddBefore(existing, tobeadded);
	ODAcquireObject(fEv, tobeadded); // function, not macro
}

//------------------------------------------------------------------------------
// ODRefCntCollection::AddAfter
//------------------------------------------------------------------------------

void ODRefCntCollection::AddAfterAndAcquire(ODRefCntObject* existing, ODRefCntObject* tobeadded)
{
	fCol.OrderedCollection::AddAfter(existing, tobeadded);
	ODAcquireObject(fEv, tobeadded); // function, not macro
}

//------------------------------------------------------------------------------
// ODRefCntCollection::RemoveAndRelease
//------------------------------------------------------------------------------

ODBoolean ODRefCntCollection::RemoveAndRelease(ODRefCntObject* existing)
{
	ODBoolean removed = fCol.OrderedCollection::Remove(existing);
	if ( removed )
		ODRelease(fEv, existing); // function, not macro
	return removed;
}

//------------------------------------------------------------------------------
// ODRefCntCollection::RemoveAndReleaseAll
//------------------------------------------------------------------------------

void ODRefCntCollection::RemoveAndReleaseAll()
{
	OrderedCollectionIterator iter(&fCol);
	
	try {
		for (ODRefCntObject* rco = (ODRefCntObject*) iter.First();
				iter.IsNotComplete();
				rco = (ODRefCntObject*) iter.Next() )
		{
			ODRelease(fEv, rco); // function, not macro
		}
	} catch(ODException _exception) {
	}

	// delete iter;

	fCol.RemoveAll();
}

//------------------------------------------------------------------------------
// ODRefCntCollection::CreateIterator
//------------------------------------------------------------------------------

// ODRefCntCollectionIterator* ODRefCntCollection::CreateIterator()
// {
// 	return new(fCol.GetHeap()) ODRefCntCollectionIterator(this);
// }

//======================================================================================
// ODRefCntCollectionIterator
//======================================================================================

//------------------------------------------------------------------------------
// ODRefCntCollectionIterator::ODRefCntCollectionIterator
//------------------------------------------------------------------------------

ODRefCntCollectionIterator::ODRefCntCollectionIterator(ODRefCntCollection* collection)	
	: fIter( &collection->fCol )
{
}

//------------------------------------------------------------------------------
// ODRefCntCollectionIterator::~ODRefCntCollectionIterator
//------------------------------------------------------------------------------

ODRefCntCollectionIterator::~ODRefCntCollectionIterator()						
{
}



