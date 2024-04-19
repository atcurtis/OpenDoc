/* @(#)Z 1.11 com/src/storage/StorPriv.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:05 (96/10/29 09:31:49) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
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
	File:		StorPriv.cpp

	Contains:	Implementation for ODStorageSystem class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<14>	 8/22/95	VL		1255362: Removed XMP from header.
		<13>	 8/21/95	VL		1278330, 1278315: Error code cleanup.
		<12>	 7/21/95	VL		1270320: Dispose ba._buffer from
									container->GetID.
		<11>	 5/26/95	VL		1251403: Multithreading naming support.
		<10>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <9>	 4/25/95	VL		1210982: Removed 5$.
		 <8>	 4/15/95	VL		1240014: ContainerList::Add gets the real
									ID from container instead of through a
									parameter.
		 <7>	10/19/94	VL		Turned off DEBUG_STORPRIV to avoid
									generating unnecessary debug strs.
		 <6>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets.
		 <5>	  7/7/94	VL		Commented out use of ODRecoverHeapID.
		 <4>	 6/28/94	VL		Use ODRecoverHeapID.
		 <3>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <2>	 6/15/94	CC		ODHeap -> ODMemoryHeap.
		 <1>	  6/1/94	VL		first checked in

	To Do:
	In Progress:
*/

#ifndef _STORPRIV_
#include "StorPriv.h"
#endif

#ifndef SOM_ODContainer_xh
#include "ODCtr.xh"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _ODNEW_
#include "ODNew.h"
#endif
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef som_xh
#include "som.xh"
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment Storage
#endif

//==============================================================================
// Constants
//==============================================================================
#define kODErrContainerExistsWithDifferentID 923

#if ODDebug
// #define DEBUG_STORPRIV 1
#endif

//==============================================================================
// ContainerList
//==============================================================================

//------------------------------------------------------------------------------
// ContainerList::ContainerList
//------------------------------------------------------------------------------

ContainerList::ContainerList()
{
	fHeap = kODNULL;
}

//------------------------------------------------------------------------------
// ContainerList::~ContainerList
//------------------------------------------------------------------------------

ContainerList::~ContainerList()
{
	delete fLinkedList;
}

//------------------------------------------------------------------------------
// ContainerList::Initialize
//------------------------------------------------------------------------------

void ContainerList::Initialize()
{
	// Using default heap because the Storage System also resides in Default heap.
	fHeap = kDefaultHeapID;
	
#ifdef _PLATFORM_MACINTOSH_
        fLinkedList = new(GetHeap()) LinkedList;
#endif
#if ((defined _PLATFORM_WIN32_)||(defined _PLATFORM_OS2_)||(defined _PLATFORM_UNIX_))
        fLinkedList = new LinkedList;
#endif
}

//------------------------------------------------------------------------------
// ContainerList::Get
//------------------------------------------------------------------------------

ODContainer* ContainerList::Get(ODContainerID* containerID)
{	
	ODContainer*	container = kODNULL;
	ContainerLink* link = this->GetLink(containerID);
	if (link != kODNULL)
		container = link->fContainer;
		
#ifdef DEBUG_STORPRIV
	somPrintf("ContainerList:Get containerID %x length %x container %x\n", containerID, containerID->_length, container);
	for (ODULong i = 0; i < containerID->_length; i++) 
		somPrintf("%x ", (containerID->_buffer)[i]);
	somPrintf("\n");
#endif
	return container;
}

//------------------------------------------------------------------------------
// ContainerList::Add
//------------------------------------------------------------------------------

void ContainerList::Add(ODContainer* container)
{	
	ODContainer* existingContainer;
	ContainerLink* link  = kODNULL;

#ifdef DEBUG_STORPRIV
	somPrintf("ContainerList:Add containerID %x length %x container %x\n", containerID, containerID->_length, container);
	for (ODULong i = 0; i < containerID->_length; i++) 
		somPrintf("%x ", (containerID->_buffer)[i]);
	somPrintf("\n");
#endif
	Environment* ev = somGetGlobalEnvironment();
        ODContainerID containerID;
	
	containerID = container->GetID(ev);
     try
     {
	if ((existingContainer = this->Get(&containerID)) == kODNULL) {
#ifdef _PLATFORM_MACINTOSH_
                link = new(GetHeap()) ContainerLink(container);
#endif
#if ((defined _PLATFORM_WIN32_)||(defined _PLATFORM_OS2_)||(defined _PLATFORM_UNIX_))
                link = new ContainerLink(container);
#endif
		fLinkedList->AddLast(link);
	}
	ODDisposePtr(containerID._buffer);
     }
     catch (ODException _exception)
     {
        if(link)
            delete(link);
        link = kODNULL;
        ODDisposePtr(containerID._buffer);
        throw;
     }
}
		
//------------------------------------------------------------------------------
// ContainerList::Remove
//------------------------------------------------------------------------------

void ContainerList::Remove(ODContainerID* containerID)
{
 
        ContainerLink* link  = kODNULL;
	link = this->GetLink(containerID);

#ifdef DEBUG_STORPRIV
	somPrintf("ContainerList:Remove containerID %x length %x\n", containerID, containerID->_length, *((ODULong*) containerID->_buffer));
	for (ODULong i = 0; i < containerID->_length; i++) 
		somPrintf("%x ", (containerID->_buffer)[i]);
	somPrintf("\n");
#endif
	
	if (link == kODNULL) {
		THROW(kODErrContainerDoesNotExist);
	}
	else {
		fLinkedList->Remove(*link);
		delete link;
	}
}

//------------------------------------------------------------------------------
// ContainerList::GetLink
//------------------------------------------------------------------------------

ContainerLink* ContainerList::GetLink(ODContainerID* containerID)
{
	Environment*	ev = somGetGlobalEnvironment();
	LinkedListIterator	iter(fLinkedList);
	ODBoolean	found = kODFalse;
        ContainerLink*  link = kODNULL;
        ODContainerID id;
	
	link = (ContainerLink*) iter.Last();
	while ((link != kODNULL) && (found == kODFalse))
	{
		id = link->fContainer->GetID(ev);
           	try
           	{
			if (AreByteArraysEqual(&id, containerID))
				found = kODTrue;
			else
				link = (ContainerLink*) iter.Previous();
                
			ODDisposePtr(id._buffer);
                }
		catch (ODException _exception)
		{
			ODDisposePtr(id._buffer);
			throw;
		}
	}
	return link;
}

//------------------------------------------------------------------------------
// ContainerList::GetHeap
//------------------------------------------------------------------------------

ODMemoryHeapID ContainerList::GetHeap()
{
	return fHeap;
}

//------------------------------------------------------------------------------
// ContainerList::GetLinkedList
//------------------------------------------------------------------------------

LinkedList* ContainerList::GetLinkedList()
{
	return fLinkedList;
}

//==============================================================================
// ContainerListIterator
//==============================================================================

//------------------------------------------------------------------------------
// ContainerListIterator::ContainerListIterator
//------------------------------------------------------------------------------

ContainerListIterator::ContainerListIterator(ContainerList* containerList)
{
	fContainerList = containerList;
	fIterator = kODNULL;
}
	
//------------------------------------------------------------------------------
// ContainerListIterator::~ContainerListIterator
//------------------------------------------------------------------------------

ContainerListIterator::~ContainerListIterator()
{
	delete fIterator;
}

//------------------------------------------------------------------------------
// ContainerListIterator::Initialize
//------------------------------------------------------------------------------

void ContainerListIterator::Initialize()
{
#ifdef _PLATFORM_MACINTOSH_
	fIterator = new(fContainerList->GetHeap()) LinkedListIterator(fContainerList->GetLinkedList());
#endif
#if ((defined _PLATFORM_WIN32_)||(defined _PLATFORM_OS2_)||(defined _PLATFORM_UNIX_))
	fIterator = new LinkedListIterator(fContainerList->GetLinkedList());
#endif
}

//------------------------------------------------------------------------------
// ContainerListIterator::Last
//------------------------------------------------------------------------------

ODContainer* ContainerListIterator::Last()
{
	ContainerLink* link = (ContainerLink*) fIterator->Last();
	if (link == kODNULL)
		return kODNULL;
	return link->fContainer;
}

//------------------------------------------------------------------------------
// ContainerListIterator::Previous
//------------------------------------------------------------------------------

ODContainer* ContainerListIterator::Previous()
{
	ContainerLink* link = (ContainerLink*) fIterator->Previous();
	if (link == kODNULL)
		return kODNULL;
	return link->fContainer;
}
		
//------------------------------------------------------------------------------
// ContainerListIterator::IsNotComplete
//------------------------------------------------------------------------------

ODBoolean ContainerListIterator::IsNotComplete()
{
	return fIterator->IsNotComplete();
}

