/* @(#)Z 1.3 com/src/storage/StorPriv.h, odstorage, od96os2, odos29646d 96/11/15 15:29:27 (96/07/15 18:23:39) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   ContainerLink
 *		ContainerList
 *		ContainerListIterator
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */
/*
	File:		StorPriv.h

	Contains:	Private Definition for XMPStorageSystem

	Owned by:	Vincent Lo

	Copyright:	© 1992 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):
	
		 <6>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <5>	 4/15/95	VL		1240014: ContainerList::Add gets the real
									ID from container instead of through a
									parameter.
		 <4>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets.
		 <3>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <2>	 6/15/94	CC		ODHeap -> ODMemoryHeap.
		 <1>	  6/1/94	VL		first checked in

	To Do:
	In Progress:
*/

#ifndef _STORPRIV_
#define _STORPRIV_

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef SOM_ODContainer_xh
#include "ODCtr.xh"
#endif

//==============================================================================
// Classes defined in this interface
//==============================================================================

class ContainerLink;
class ContainerList;
class ContainerListIterator;

//==============================================================================
// ContainerLink
//==============================================================================

class ContainerLink : public Link {

public:
	
	ContainerLink(ODContainer* container)
	{
		fContainer = container;
	}
	~ContainerLink() {};
	
	ODContainer*	fContainer;
};

//==============================================================================
// ContainerList
//==============================================================================

class ContainerList
{	

public:

	ContainerList();
	
	ODVMethod	~ContainerList();
	
	ODMethod	void Initialize();
	
	ODMethod void Add(ODContainer* container);
	
	ODMethod void Remove(ODContainerID* containerID);
	
	ODMethod ODContainer* Get(ODContainerID* containerID);

private:

	ODMethod ContainerLink*	GetLink(ODContainerID* containerID);

	LinkedList*		fLinkedList;
	ODMemoryHeapID	fHeap;
	
public:	// private by convention
	
	ODMethod	ODMemoryHeapID	GetHeap();
	ODMethod	LinkedList*		GetLinkedList();
};

//==============================================================================
// ContainerListIterator
//==============================================================================

class ContainerListIterator
{

public:

	ContainerListIterator(ContainerList* containerList);
	
	ODVMethod	~ContainerListIterator();
	
	ODMethod	void			Initialize();
	ODMethod	ODContainer*	Last();
	ODMethod	ODContainer*	Previous();
	ODMethod	ODBoolean		IsNotComplete();
	
private:

	LinkedListIterator* fIterator;
	ContainerList*		fContainerList;
};
	
#endif	// _STORPRIV_
