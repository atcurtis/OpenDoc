/* @(#)Z 1.14 com/src/storage/DragPriv.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:25:50 (96/10/29 09:31:05) */
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
	File:		DragPriv.cpp

	Contains:	Implementation of Private classes for ODDragAndDrop.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<27>	10/13/95	EL		1287340: Use standard ISO prefix
		<26>	 10/4/95	jpa		Added missing ODVolatiles [1285799]
		<25>	 10/4/95	VL		1287585: Check to see whether we are
									dragging a folder before getting the file
									info.
		<24>	 9/13/95	VL		1283523: Stop copying names for stationery
									files.
		<23>	  9/8/95	VL		1274572: Preserved the curResFile in
									GetIconFamilyFromFile.
		<22>	  9/8/95	T‚		1281096 FB2:Many constants in ODTypesB
									without kOD prefix!
		<21>	  9/6/95	VL		1274572: Added GetFinderInfo and
									GetIconFamilyFromFile.
		<20>	 8/12/95	T‚		1276812 Need to use TempObjs and TempRefs
									for exception safety and to avoid TRY
									blocks, 1276807 Opt./Bug: use StdTypIO
									routines for portable streaming & smaller
									footprint
		<19>	 7/26/95	VL		1270320: ClearAllPromises when deleting
									memory container.
		<18>	 5/26/95	VL		1251403: Multithreading naming support.
		<17>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<16>	 4/25/95	VL		1240737: Make sure that ID for file
									container is set to the right _length.
		<15>	 4/15/95	VL		1240014: turned off debug statements.
		<14>	 4/14/95	VL		Temporarily turned on debug printf.
		<13>	 3/10/95	VL		1227218: Get rid of system heap.
		<12>	 2/27/95	VL		1198037: Removed debugstr from code.
		<11>	 2/10/95	VL		1205627: Used kODBentoUpdateContainer.
		<10>	 1/26/95	VL		#???: Use updated Storage Unit Ref API.
		 <9>	 1/19/95	CC		1212419 Pass kODBentoMemoryContainer to
									CreateMemoryContainer & GetMemoryContainer.
		 <8>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <7>	12/13/94	VL		1203627,1200603,1203451,1198037,1194537,119
									4755,1186815: Bug fixes.
		 <6>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets.
		 <5>	 8/26/94	VL		1183174: Use updated cloning APIs
									(specifically GetStorageUnitRef takes ID
									now).
		 <4>	 8/15/94	T‚		#1180922 Removed most obsolete types from
									StdTypes.idl
		 <3>	  8/3/94	VL		1153123: Storage to ODStor.
		 <2>	 7/26/94	VL		Got rid of dependency on Bento.
		 <1>	 7/21/94	VL		first checked in

	To Do:
	In Progress:
*/

#ifndef _DRAGPRIV_
#include <DragPriv.h>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef SOM_ODDraft_xh
#include <ODDraft.xh>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#if (defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_))
#ifndef _ODUTILS_
#include <ODUtils.h>				//  for ODReleaseObject
#endif
#ifdef DAD_DEBUG
#include "qprintf.h"
#endif // DAD_DEBUG
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX__

#ifdef _PLATFORM_OS2_
#include <stdio.h>					// for sprintf
#endif // _PLATFORM_OS2_

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _INFOUTIL_
#include <InfoUtil.h>
#endif

#ifndef _ICONS_
#include <Icons.h>
#endif

#ifndef __FINDER__
#include <Finder.h>
#endif

#if ODDebug
#ifndef __STRINGS__
#include <Strings.h>
#endif
#endif // ODDebug
#endif // _PLATFORM_MACINTOSH_

#ifndef _USERSRCM_
#include <UseRsrcM.h>
#endif

#if ODDebug
// #define ODDebug_DragPriv	1
#endif

const ODContainerType kODBentoUpdateContainer = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:Update";

//==============================================================================
// Function Prototype
//==============================================================================

ODContainerType	GetContainerSuiteType(FSSpec* fsSpec);

//==============================================================================
// ODDragItem
//==============================================================================
//------------------------------------------------------------------------------
// ODDragItem::ODDragItem
//------------------------------------------------------------------------------
ODDragItem::ODDragItem(ODStorageSystem *storage, ODBoolean IsForeignDataSU)
{
	fStorageSystem = storage;
	fContainer = kODNULL;
	fDocument = kODNULL;
	fDraft = kODNULL;
	fSU = kODNULL;
}

void ODDragItem::Open(Environment* ev)
{
	WARNMSG_DEBUG(WARN_INDEX(-1),"ODDragItem::Open should not be called.");
}


void ODDragItem::Close(Environment* ev)
{
	WARNMSG_DEBUG(WARN_INDEX(-1),"ODDragItem::Close should not be called.");
}

//==============================================================================
// ODMemDragItem
//==============================================================================
//------------------------------------------------------------------------------
// ODMemDragItem::~ODMemDragItem
//------------------------------------------------------------------------------
ODMemDragItem::~ODMemDragItem()
{
	Environment* ev = somGetGlobalEnvironment();

#ifdef DAD_DEBUG
	qprintf("Destructing ODMemDragItem %08p, fContainerHandle = %08p:\n", this, fContainerHandle);
	qprintf("\tfContainer = %08p, ref count = %d\n",
			fContainer, fContainer->GetRefCount(ev));
	qprintf("\tfDocument = %08p, ref count = %d\n",
			fDocument, fDocument->GetRefCount(ev));
	qprintf("\tfDraft = %08p, ref count = %d\n",
			fDraft, fDraft->GetRefCount(ev));
	qprintf("\tfSU = %08p, ref count = %d\n",
			fSU, fSU->GetRefCount(ev));
#endif  // DAD_DEBUG
	if (fSU != kODNULL)
	{
		fSU->ClearAllPromises(ev);
		fSU->Release(ev);
	}
	if (fDraft != kODNULL)
		fDraft->Release(ev);
	if (fDocument != kODNULL)
		fDocument->Release(ev);
	if (fContainer != kODNULL)
		fContainer->Release(ev);
	ODDisposeHandle(fContainerHandle);
}

//------------------------------------------------------------------------------
// ODMemDragItem::Initialize
//------------------------------------------------------------------------------
void ODMemDragItem::Initialize(Environment* ev)
{
	ODDragItem::Initialize(ev);
	
	fContainerHandle = ODNewHandle(0);
	ASSERTMSG_DEBUG(MemError() == noErr, MemError(), "Error allocating D&D container handle");

	fContainer = CreateMemoryContainer(ev, fStorageSystem->GetSession(ev), fContainerHandle, kODBentoMemoryContainer);
	fDocument = fContainer->AcquireDocument(ev, kODDefaultDocument);		
	fDraft = fDocument->AcquireBaseDraft(ev, kODDPExclusiveWrite);
	fSU = fDraft->CreateStorageUnit(ev);
	TempODStorageUnit draftProperties = fDraft->AcquireDraftProperties(ev);
	ODSetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef, fSU->GetID(ev));
#ifdef DAD_DEBUG
	qprintf("Initializing ODMemDragItem %08p, fContainerHandle = %08p:\n", this, fContainerHandle);
	qprintf("\tfContainer = %08p, ref count = %d\n",
			fContainer, fContainer->GetRefCount(ev));
	qprintf("\tfDocument = %08p, ref count = %d\n",
			fDocument, fDocument->GetRefCount(ev));
	qprintf("\tfDraft = %08p, ref count = %d\n",
			fDraft, fDraft->GetRefCount(ev));
	qprintf("\tfSU = %08p, ref count = %d\n",
			fSU, fSU->GetRefCount(ev));
#endif  // DAD_DEBUG
}

//------------------------------------------------------------------------------
// ODMemDragItem::Open
//------------------------------------------------------------------------------
void ODMemDragItem::Open(Environment* ev)
{
	fContainer = GetMemoryContainer(ev, fStorageSystem->GetSession(ev), fContainerHandle, kODBentoMemoryContainer);
	fDocument = fContainer->AcquireDocument(ev, kODDefaultDocument);
	fDraft = fDocument->AcquireBaseDraft(ev, kODDPExclusiveWrite);
	TempODStorageUnit	draftProperties = fDraft->AcquireDraftProperties(ev);
	fSU = fDraft->AcquireStorageUnit(ev, ODGetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef));

#ifdef DAD_DEBUG
	qprintf("Opening ODMemDragItem %08p, fSU %08p\n", this, fSU);
#endif  // DAD_DEBUG
}

//------------------------------------------------------------------------------
// ODMemDragItem::Close
//------------------------------------------------------------------------------
void ODMemDragItem::Close(Environment* ev)
{
#ifdef DAD_DEBUG
	qprintf("Closing ODMemDragItem %08p, fSU %08p\n", this, fSU);
#endif  // DAD_DEBUG

	if (fSU != kODNULL) {
		fSU->Release(ev);
	}
	if (fDraft != kODNULL) {
		fDraft->Externalize(ev);
		fDraft->Release(ev);
	}
	if (fDocument != kODNULL)
		fDocument->Release(ev);
	if (fContainer != kODNULL)
		fContainer->Release(ev);
}

#ifdef _PLATFORM_AIX_
//------------------------------------------------------------------------------
// ODMemDragItem::Restore
//------------------------------------------------------------------------------
void ODMemDragItem::Restore(Environment* ev, void *containerData, int containerSize)
{
	// initialize the drag item, then close it so we can set its data
	Initialize(ev);
	Close(ev);

	// copy data into memory container
	fContainerHandle = ODNewHandle((ODULong)containerSize);
	ASSERTMSG_DEBUG(MemError() == noErr, MemError(), "Error allocating D&D container handle");
	memcpy(*((void **)fContainerHandle), containerData,
		(size_t)containerSize);

	// now open the new drag item (this causes the drag item to 'commit'
	// its new data to a real live container object)
	Open(ev);
}
#endif

//==============================================================================
// ODFileDragItem
//==============================================================================
//------------------------------------------------------------------------------
// ODFileDragItem::~ODFileDragItem
//------------------------------------------------------------------------------
ODFileDragItem::~ODFileDragItem()
{
#if ODDebug_DragPriv
	somPrintf("~ODFileDragItem: %x %x %x %x\n", fSU, fDraft, fDocument, fContainer);
#endif

	Environment* ev = somGetGlobalEnvironment();
	
	if (fSU != kODNULL)
		fSU->Release(ev);
	if (fDraft != kODNULL)
		fDraft->Release(ev);
	if (fDocument != kODNULL)
		fDocument->Release(ev);
	if (fContainer != kODNULL)
		fContainer->Release(ev);
}

//------------------------------------------------------------------------------
// ODFileDragItem::Initialize
//------------------------------------------------------------------------------
void ODFileDragItem::Initialize(Environment* ev, ODContainerID* containerID)
{
	ODDragItem::Initialize(ev);
	
	ODStorageUnitID	suid;
	ODContainerType containerType = kODNULL;	ODVolatile(containerType);
		
	TRY
		containerType = GetContainerSuiteType((FSSpec*) containerID->_buffer);
		containerID->_length = strlen((char *)&(((FSSpec*) containerID->_buffer)->name[0])) + 1;
		fContainer = fStorageSystem->CreateContainer(ev, containerType, containerID);
		fDocument = fContainer->AcquireDocument(ev, kODDefaultDocument);		
		fDraft = fDocument->AcquireBaseDraft(ev, kODDPExclusiveWrite);
		TempODStorageUnit	draftProperties = fDraft->AcquireDraftProperties(ev);
		suid = ODGetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef);
		fSU = fDraft->AcquireStorageUnit(ev, suid);
		ODDisposePtr(containerType);
#if ODDebug_DragPriv
	somPrintf("ODFileDragItem::Initialize %x %x %x %x\n", fSU, fDraft, fDocument, fContainer);
#endif

	CATCH_ALL
#ifdef ODDebug_DragPriv
		ODError err = ErrorCode();
		WARNMSG_DEBUG(WARN_INDEX(-1),"Error in ODFileDragItem::Initialize");
		char	errString[100];
		numtostring(err, errString);
		WARNMSG_DEBUG(WARN_INDEX(-1),errString);
#endif
		ODReleaseObject(ev,fSU);
		ODReleaseObject(ev,fDraft);
		ODReleaseObject(ev,fDocument);
		ODReleaseObject(ev,fContainer);
		ODDisposePtr(containerType);
		RERAISE;
	ENDTRY
}

//==============================================================================
// ODDragLink
//==============================================================================
//------------------------------------------------------------------------------
// ODDragLink::ODDragLink
//------------------------------------------------------------------------------
ODDragLink::ODDragLink(ODDragItem *theItem, ODBoolean cleanup)
{
	fItem = theItem;
	fCleanup = cleanup;
}

//------------------------------------------------------------------------------
// ODDragLink::~ODDragLink
//------------------------------------------------------------------------------
ODDragLink::~ODDragLink()
{
	if (fCleanup)
		delete fItem;
}

#ifdef _PLATFORM_OS2_
//==============================================================================
// ODDragRMF
//==============================================================================
//------------------------------------------------------------------------------
// ODDragRMF::ODDragRMF
//------------------------------------------------------------------------------
ODDragRMF::ODDragRMF(PSZ pszRMF)
{
	fRMF = strdup(pszRMF);
}

//------------------------------------------------------------------------------
// ODDragRMF::~ODDragRMF
//------------------------------------------------------------------------------
ODDragRMF::~ODDragRMF()
{
	free(fRMF);
}

struct StdFmtKind
{
	PSZ kindName;
	PSZ pszFormat;
};

static const StdFmtKind StdFmtKinds[] =
{
	kODKindOS2Bitmap,        "DRF_BITMAP",
	kODKindOS2DspBitmap,     "DRF_DSPBITMAP",
	kODKindOS2Metafile,      "DRF_METAFILE",
	kODKindOS2Text,          "DRF_TEXT",
	kODKindOS2DIB,           "DRF_DIB",
	kODKindOS2DIF,           "DRF_DIF",
	kODKindOS2OEMText,       "DRF_OEMTEXT",
	kODKindOS2OwnerDisplay,  "DRF_OWNERDISPLAY",
	kODKindOS2PtrPict,       "DRF_PTRPICT",
	kODKindOS2RTF,           "DRF_RTF",
	kODKindOS2SYLK,          "DRF_SYLK",
	kODKindOS2TIFF,          "DRF_TIFF",
	kODKindOS2Unknown,       "DRF_UNKNOWN"
};

static const StdFmtKind StdTypeKinds[] =
{
	kODKindOS2Bitmap,        "DRT_BITMAP",
	kODKindOS2Metafile,      "DRT_METAFILE",
	kODKindOS2Text,          "DRT_TEXT",
	kODKindOS2Unknown,       "DRF_UNKNOWN"
};

#define NumStdFormats  (sizeof(StdFmtKinds)/sizeof(StdFmtKind))

PSZ GetKindForFormat(PSZ fmt)
{
	for(int i=0; i<NumStdFormats; i++)
    	if(!strcmp(fmt,StdFmtKinds[i].pszFormat))
			return StdFmtKinds[i].kindName;
	return kODNULL;
}

ODBoolean GetRMFForKind(PSZ kind, PSZ pszRMF)
{
	for(int i=0; i<NumStdFormats; i++)
    	if(!strcmp(kind,StdFmtKinds[i].kindName))
		{
        	sprintf(pszRMF,"<DRM_OS2FILE,%s>",StdFmtKinds[i].pszFormat);
			return kODTrue;
		}
	return kODFalse;
}
#endif // _PLATFORM_OS2_

//------------------------------------------------------------------------------
// GetContainerSuiteType
//------------------------------------------------------------------------------
ODContainerType	GetContainerSuiteType(FSSpec* fsSpec)
{
#if USE_GETCONTAINER_SUITE_TYPE
	PlatformFile	file;
	file.Specify(fsSpec);
	
	ODContainerType containerType = file.AcquireContainerType();
	if (ODISOStrEqual(containerType, kODBentoFileContainer) || 
		ODISOStrEqual(containerType, ""))
	{
		containerType = kODBentoUpdateContainer;
	}
#else
	ODULong len = strlen(kODBentoUpdateContainer);
	ODContainerType containerType = (ODContainerType) ODNewPtr(len + 1);
	strcpy(containerType, kODBentoUpdateContainer);
	return containerType;
#endif // USE_GETCONTAINER_SUITE_TYPE
}

#ifdef _PLATFORM_MACINTOSH
//------------------------------------------------------------------------------
// GetFinderInfo
//------------------------------------------------------------------------------
void GetFinderInfo(Environment* ev, ODStorageUnit* su, PlatformFile* file)
{
	if (!file->IsDirectory())
	{
		ODTime creationDate = file->GetFileCreationDate();
		ODSetCreationDate(ev, su, creationDate);
		ODTime modDate = file->GetFileModDate();
		ODSetModificationDate(ev, su, modDate);
	
		if (!file->IsStationery())
		{	
			ODName* name = file->GetName();
			ODSetPONameUsingSU(ev, su, name);
			DisposeIText(name);
		}
	
		// Stationery bit is purposedly left out.
	}
}

//------------------------------------------------------------------------------
// GetIconFamilyFromFile
//------------------------------------------------------------------------------
void GetIconFamilyFromFile(Environment* ev, ODStorageUnit* su, PlatformFile* file)
{
	if (!file->IsDirectory())
	{
		Handle iconFamily;
		
		short refNum = 0;
		ODBoolean hasCustomIcon;
		OSErr err = noErr;
		ODVolatile(err);
		TRY
			hasCustomIcon = file->HasCustomIcon();
			if (hasCustomIcon)
			{
				file->SetPermission(fsRdPerm);
				refNum = CurResFile();
				file->OpenResFile();
			}
		CATCH_ALL
			UseResFile(refNum);
		ENDTRY
		
		if (hasCustomIcon && (err == noErr))
		{		
			err = GetIconSuite(&iconFamily, kCustomIconResource, svAllAvailableData);
			if (err == noErr)
			{
				ODSetIconFamilyProp(ev, su, kODPropCustomIcon, kODIconFamily, iconFamily, kODFalse);
				DisposeIconSuite(iconFamily, true);
			}
#ifdef ODDEBUG
			else
				WARN("Problem getting icon family: %d ", err);
#endif
			file->CloseResFile();
		}
	}
}
#endif // _PLATFORM_MACINTOSH_
