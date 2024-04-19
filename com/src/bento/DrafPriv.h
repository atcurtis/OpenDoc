/* @(#)Z 1.4 com/src/bento/DrafPriv.h, odstorage, od96os2, odos29646d 96/11/15 15:29:28 (96/10/29 09:12:07) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   IDList
 *		PreserveFocus
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
	File:		DrafPriv.h

	Contains:	Private class used by CMDraft.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <8>	 6/16/95	VL		1244940: Removed fEV from PreserveFocus
									class.
		 <7>	 6/13/95	VL		1241352: Moved PreserveFocus from
									CMDraft.cpp.
		 <6>	 4/25/95	DM		1172791: Replace AEHashTable with
									OpenHashTable
		 <5>	 8/11/94	CC		Added #define _DRAFPRIV_.
		 <4>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <3>	 6/15/94	RR		ODHeap -> ODMemoryHeap
		 <2>	 5/27/94	VL		Changed ODAEHashTable to AEHashTable.
		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

#ifndef _DRAFPRIV_
#define _DRAFPRIV_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

class OpenHashTable;
class ODStorageUnit;
class ODStorageUnitCursor;

class IDList {

public:

	IDList();
	ODVMethod	~IDList();
	
	ODMethod void Initialize();
	
	ODMethod void	Add(ODID id, void* object);
	ODMethod ODID Add(void* object);
	ODMethod void 	Remove(ODID id);

	ODMethod void*	Get(ODID id);
	ODMethod ODID	GetID(void* object);
	
	ODMethod ODBoolean	Exists(ODID id);
	ODMethod ODBoolean	ObjectExists(void* object);

private:

	ODMemoryHeapID			GetHeap();
	
	OpenHashTable*	fIDToObj;
	OpenHashTable*	fObjToID;
	ODID			fCurrentID;
	ODMemoryHeapID			fHeap;
};

class PreserveFocus
{

public:

	PreserveFocus(Environment* ev, ODStorageUnit* su);
	~PreserveFocus();
		
private:

	ODStorageUnit* fSU;
	ODStorageUnitCursor* fSUCursor;
};

#endif	// _DRAFPRIV_
