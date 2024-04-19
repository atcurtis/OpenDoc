/* @(#)Z 1.4 com/src/storage/DgItmIt.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:25:49 (96/10/29 09:30:40) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: oddataxfer
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
	File:		DgItmIt.cpp

	Contains:	Implementation of ODDragItemIterator

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <5>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <4>	 5/26/95	VL		1251403: Multithreading naming support.
		 <3>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <2>	 7/21/94	VL		Added implementation.

	To Do:
*/

#define ODDragItemIterator_Class_Source
#define VARIABLE_MACROS
#include <DgItmIt.xih>

#ifndef _DRAGPRIV_
#include <DragPriv.h>
#endif

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

#include "DgItmItB.cpp"	// Platform-independent methods, if any

SOM_Scope void  SOMLINK ODDragItemIteratorInitDragItemIterator(ODDragItemIterator *somSelf, Environment *ev,
		LinkedListIterator* dragItemListIter)
{
    ODDragItemIteratorData *somThis = ODDragItemIteratorGetData(somSelf);
    ODDragItemIteratorMethodDebug("ODDragItemIterator","ODDragItemIteratorInitDragItemIterator");

	_fDragItemListIter = dragItemListIter;
}

SOM_Scope void  SOMLINK ODDragItemIteratorsomUninit(ODDragItemIterator *somSelf)
{
    ODDragItemIteratorData *somThis = ODDragItemIteratorGetData(somSelf);
    ODDragItemIteratorMethodDebug("ODDragItemIterator","ODDragItemIteratorsomUninit");

	delete _fDragItemListIter;
	
    ODDragItemIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODStorageUnit*  SOMLINK ODDragItemIteratorFirst(ODDragItemIterator *somSelf, Environment *ev)
{
    ODDragItemIteratorData *somThis = ODDragItemIteratorGetData(somSelf);
    ODDragItemIteratorMethodDebug("ODDragItemIterator","ODDragItemIteratorFirst");

	ODDragLink*	theLink = (ODDragLink*) _fDragItemListIter->First();
	return(theLink ? (ODStorageUnit*) theLink->fItem->fSU : (ODStorageUnit*) kODNULL);
}

SOM_Scope ODStorageUnit*  SOMLINK ODDragItemIteratorNext(ODDragItemIterator *somSelf, Environment *ev)
{
    ODDragItemIteratorData *somThis = ODDragItemIteratorGetData(somSelf);
    ODDragItemIteratorMethodDebug("ODDragItemIterator","ODDragItemIteratorNext");

	ODDragLink *theLink = (ODDragLink*)_fDragItemListIter->Next();
	return(theLink ? (ODStorageUnit*) theLink->fItem->fSU : (ODStorageUnit*) kODNULL);
}

SOM_Scope ODBoolean  SOMLINK ODDragItemIteratorIsNotComplete(ODDragItemIterator *somSelf, Environment *ev)
{
    ODDragItemIteratorData *somThis = ODDragItemIteratorGetData(somSelf);
    ODDragItemIteratorMethodDebug("ODDragItemIterator","ODDragItemIteratorIsNotComplete");

	return (_fDragItemListIter->IsNotComplete());
}
