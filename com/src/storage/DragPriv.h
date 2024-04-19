/* @(#)Z 1.6 com/src/storage/DragPriv.h, oddataxfer, od96os2, odos29646d 96/11/15 15:25:50 (96/10/29 09:31:11) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: oddataxfer
 *
 *   CLASSES:   ODDragItem
 *		ODDragLink
 *		ODDragRMF
 *		ODFileDragItem
 *		ODMemDragItem
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
	File:		DragPriv.cpp

	Contains:	Definition of Private classes for ODDragAndDrop.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>	  9/6/95	VL		1274572: Added GetFinderInfo and
									GetIconFamilyFromFile.
		 <6>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <5>	12/13/94	VL		1203627,1200603,1203451,1198037,1194537,119
									4755,1186815: Bug fixes.
		 <4>	 9/29/94	RA		1189812: Mods for 68K build.
		 <3>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets.
		 <2>	 7/26/94	VL		Got rid of dependency on Bento Container
									Suite.
		 <1>	 7/21/94	VL		first checked in

	To Do:
	In Progress:
*/

#ifndef _DRAGPRIV_
#define _DRAGPRIV_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef _PLFMDEF_
#include <PlfmDef.h>
#endif

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

//==============================================================================
// Forward declaration
//==============================================================================

class	ODStorageSystem;
class	ODContainer;
class	ODDocument;
class	ODDraft;
class	PlatformFile;
class	ODStorageUnit;

//==============================================================================
// Function prototype
//==============================================================================
void GetFinderInfo(Environment* ev, ODStorageUnit* su, PlatformFile* file);
void GetIconFamilyFromFile(Environment* ev, ODStorageUnit* su, PlatformFile* file);

//==============================================================================
// ODDragItem
//==============================================================================
class ODDragItem
{
public:

	ODDragItem(ODStorageSystem *storage, ODBoolean IsForeignDataSU);
	ODVMethod	~ODDragItem() {}
	ODNVMethod	void Initialize(Environment* ev) {}
	ODVMethod	void Open(Environment* ev);
	ODVMethod	void Close(Environment* ev);
	
	ODStorageUnit*	fSU;

protected:

	ODStorageSystem*		fStorageSystem;
	ODContainer*			fContainer;
	ODDocument*				fDocument;
	ODDraft*				fDraft;
};

class ODMemDragItem : public ODDragItem
{
public:

	ODMemDragItem(ODStorageSystem *storage, ODBoolean IsForeignDataSU)
		: ODDragItem(storage, IsForeignDataSU) {};
	~ODMemDragItem();
		void Initialize(Environment* ev);
		void Open(Environment* ev);
		void Close(Environment* ev);
#ifdef _PLATFORM_AIX_
		void Restore(Environment* ev, void* containerData, int containerSize);
#endif

	ODHandle                  fContainerHandle;
};

class ODFileDragItem : public ODDragItem
{
public:

		ODFileDragItem(ODStorageSystem *storage, ODBoolean IsForeignDataSU)
			: ODDragItem(storage, IsForeignDataSU) {};
	   	~ODFileDragItem();
		void Initialize(Environment* ev, ODContainerID* file);
};


//==============================================================================
// ODDragLink
//==============================================================================
class ODDragLink : public Link
{
	public:
		ODDragLink(ODDragItem *theItem, ODBoolean cleanup);
		~ODDragLink();
	   
		ODDragItem *fItem;
		ODBoolean   fCleanup;
};

#ifdef _PLATFORM_OS2_
//==============================================================================
// ODDragRMF
//==============================================================================
class ODDragRMF : public Link
{
   public:
    ODDragRMF(PSZ pszRMF);
    ~ODDragRMF();

    PSZ fRMF;
};
#endif // _PLATFORM_OS2_

#endif	// _DRAGPRIV_
