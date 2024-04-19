/* @(#)Z 1.17 com/src/bento/CMCtr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:08 (96/10/29 09:10:55) */
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
	File:		CMCtr.cpp

	Contains:	Implementation of ODBentoContainer class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<26>	10/24/95	jpa		1293441: DM/VL: Bento memory reserve &
									fatal container err & don't throw in
									Release.
		<25>	 10/8/95	TJ		Fixes Recomended by Refball
		<24>	 9/29/95	TJ		Made Changes for MAC SOM
		<23>	 8/21/95	VL		1278330, 1278315: Error code cleanup.
		<22>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<21>	 7/21/95	VL		1270320: Dispose _fID. Return a copy of the
									ID in GetID.
		<20>	 5/26/95	VL		1251403: Multithreading naming support.
		<19>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<18>	 5/11/95	DM		#1246524, #1246529:
									ValueNameSpaceGetEntry() must pass through
									the value allocated.
		<17>	 5/11/95	VL		1238421: Implemented Purge.
		<16>	  4/7/95	EL		1225905: Add GetModDate and SetModDate.
		<15>	 3/23/95	VL		1230357: Implemented Purge.
		<14>	  3/9/95	VL		1225504: Fixed Document ref-counting
									problem.
		<13>	 2/10/95	VL		1205627: Added GetTargetContainer.
		<12>	  2/2/95	CG		#1195019:  ValueNameSpace entries are now
									copied in and out instead of pointers being
									passed around.
		<11>	 1/31/95	EL		1195846: allow clearing of the dirty flag.
		<10>	 1/23/95	CG		#1195019: ValueNameSpace methods now take
									ODByteArray parameter instead of ODPtr.
		 <9>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <8>	10/12/94	VL		Fixed problem introduced in the last
									checkin.
		 <7>	 9/29/94	RA		1189812: Mods for 68K build.
		 <6>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets. 1184166: ContainerName is ODIText
									now.
		 <5>	  9/5/94	VL		1184871: Used Renew to remove dependency on
									default heap.
		 <4>	  8/5/94	VL		1171603: Added GetCMSession.
		 <3>	  8/3/94	VL		1153123: Storage to ODStor.
		 <2>	  7/7/94	VL		Commented out use of ODRecoverHeapID.
		 <1>	 6/30/94	CC		first checked in
		 <0>	 6/27/94	SV		SOMverted
		 <2>	 4/13/94	VL		1157028: StorageM.h does not include
									CMAPI.h anymore, so GetCMSession returns
									ODULong.
		 <1>	 3/25/94	VL		first checked in
		 <3>	 3/24/94	VL		#1147173, #1144732: Created Bento-specific
									Container Suite classes (uses CMDocument).
		 <2>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		<15>	  2/8/94	VL		Correct capitalization and <> for includes.
		<14>	  2/8/94	VL		Use new exception macros.
		<13>	  2/4/94	VL		Moved to PPC Header and code cleanup.
		<12>	 1/12/94	VL		Init changes.
		<11>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		<10>	11/23/93	VL		Made Bento Container Pool-aware.
		 <8>	 8/20/93	VL		Implemented GetName and SetName.
		 <7>	  8/6/93	VL		fContainer should be fCMContainer.
		 <6>	 7/30/93	VL		ODContainerName* should be
									ODContainerName.
		 <5>	 7/29/93	VL		AcquireDocument should check whether a
									ODDocument is created already.
		 <4>	  7/6/93	VL		ODContainerName should be
									ODContainerName*.
		 <3>	 6/30/93	VL		Used fDocument.
		 <2>	 6/22/93	VL		Used RefCount from ODRefCntObject.
		 <1>	 6/15/93	VL		first checked in

	To Do:
	To Do:
	In Progress:
		
*/

#define ODBentoContainer_Class_Source

#define VARIABLE_MACROS

#include <CMCtr.xih>

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_CMDocument_xh
#include <CMDoc.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"		// for sessionRoutinesMetahandler
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _BENTOHDR_
#include "BentoHdr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_Module_OpenDoc_Errors_defined 
#include <ErrorDef.xh>
#endif

#ifndef _ODNEW_
#include <ODNew.h>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODValueNameSpace_xh
#include <ValueNS.xh>
#endif

#ifdef DebugRefCount
#include <stdio.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"	// Adkins -- added
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"	// Adkins -- added
#endif

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif

#pragma segment BentoCtr

//==============================================================================
// Constants
//==============================================================================

const ODISOStr	kODContainerSuitesNameSpace = "OpenDoc:Apple:Macintosh:Container Suites";
const ODISOStr	kODBentoContainerSuite = "OpenDoc:Apple:Macintosh:Container Suite:Bento";
#define kODExpectedNumOfContainerSuites 10

//==============================================================================
// ODBentoContainer
//==============================================================================

//------------------------------------------------------------------------------
// ODBentoContainer: GetStorageSystem
//------------------------------------------------------------------------------

SOM_Scope ODStorageSystem*  SOMLINK ODBentoContainerGetStorageSystem(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetStorageSystem");

	return _fStorageSystem;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetID
//------------------------------------------------------------------------------

SOM_Scope ODContainerID  SOMLINK ODBentoContainerGetID(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetID");

	ODContainerID id;
	
    try
    {
	
	id = CopyByteArrayStruct(_fID);
	
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	return id;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetName
//------------------------------------------------------------------------------

SOM_Scope ODContainerName  SOMLINK ODBentoContainerGetName(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetName");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	ODContainerName dummy;
	return dummy;
}

//------------------------------------------------------------------------------
// ODBentoContainer: SetName
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBentoContainerSetName(ODBentoContainer *somSelf, Environment *ev,
		ODContainerName* name)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","SetName");
    
	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODBentoContainer: AcquireDocument
//------------------------------------------------------------------------------

SOM_Scope ODDocument*  SOMLINK ODBentoContainerAcquireDocument(ODBentoContainer *somSelf, Environment *ev,
		ODDocumentID id)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","AcquireDocument");

	ODDocument *retDoc = kODNULL;
   try
   {
	if (_fDocument == kODNULL) {
	
#ifdef _PLATFORM_MACINTOSH_
		SOMClass*	cmDocClass = somNewClassReference(CMDocument);
	
		ODULong		size = cmDocClass->somGetInstanceSize();
		ODPtr		buffer = ODNewPtr(size, somSelf->GetHeap(ev));
		ODDocument*	document = (ODDocument*) cmDocClass->somRenew(buffer);
		somReleaseClassReference ( cmDocClass );
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		ODDocument*	document = (ODDocument*)new CMDocument;
#endif
		
		if (document != kODNULL) 
		{
			document->InitDocument(ev, somSelf, id);
			_fDocument = document;
		}
	}
	else {
		if (_fDocument->GetID(ev) != id)
			THROW(kODErrDocumentDoesNotExist);
		_fDocument->Acquire(ev);
	}			

        retDoc = _fDocument;
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	return retDoc;
}

//------------------------------------------------------------------------------
// ODBentoContainer: Acquire
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBentoContainerAcquire(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","Acquire");

   try
   {
	ODBentoContainer_parent_ODContainer_Acquire(somSelf, ev);
   }
   catch (ODException _exception)
   { 
       ODSetSOMException(ev, _exception);
   }

}

//------------------------------------------------------------------------------
// ODBentoContainer: Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBentoContainerRelease(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","Release");
	
   try
   {

	ODBentoContainer_parent_ODContainer_Release(somSelf, ev);

	if (somSelf->GetRefCount(ev) == 0) {
		somSelf->GetStorageSystem(ev)->ReleaseContainer(ev, somSelf);
	}
	
   }
   catch (ODException _exception)
   {
	
		ODError err = ErrorCode();

		WARNMSG(WARN_INDEX(AMSG_640),"Error occurred in ODContainer::Release: %d %s", err, ErrorMessage() ?ErrorMessage() :"");

//		if (err == kODErrBentoErr)
//			SetErrorCode(kODErrFatalContainerError);
//		else if (err != kODErrFatalContainerError)
			SetErrorCode(kODNoError);
   }
}

//------------------------------------------------------------------------------
// ODBentoContainer: ~ODBentoContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBentoContainersomUninit(ODBentoContainer *somSelf)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","somUninit");

#ifdef DebugRefCount
	printf("~ODBentoContainer %x RefCount %d\n", this, somSelf->GetRefCount(ev));
	fflush(stdout);
#endif

	Environment*	ev = somGetGlobalEnvironment();
   try
  {
	
	if (_fDocument != kODNULL) {
		if (_fDocument->GetRefCount(ev) != 0)
		   somPrintf("BentoContainer::somUninit - Document not ref-counted properly.\n");
                else
  		   delete _fDocument;
		_fDocument = kODNULL;
	}

	if (_fID)
	{
		ODDisposePtr(_fID->_buffer);
		ODDisposePtr(_fID);
	}
	
    ODBentoContainer_parent_ODContainer_somUninit(somSelf);
  }
  catch (ODException _exception)
  {
     ODSetSOMException(ev, _exception);
  }
}

//------------------------------------------------------------------------------
// ODBentoContainer: InitContainer
//------------------------------------------------------------------------------

#define kODcmAllocReserveSize (16 * 1204)

SOM_Scope void  SOMLINK ODBentoContainerInitContainer(ODBentoContainer *somSelf, Environment *ev,
		ODStorageSystem* storage, ODContainerID* id)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","InitContainer");

	ODSessionRefCon* sessionRefCon = kODNULL;
        ODBoolean CreateNameSpace = kODFalse;

    try
    {
	/* Moved from somInit. SOM itself sets fields to zero
	_fStorageSystem = kODNULL;
	_fID = 0;
	_fDocument = kODNULL;
	_fDirty = kODFalse;
	_fHeap = kDefaultHeapID;
	*/
	
	_fStorageSystem = storage;
	if (_fStorageSystem == kODNULL)
		THROW(kODErrIllegalNullStorageSystemInput);

    ODBentoContainer_parent_ODContainer_InitContainer(somSelf, ev, storage, id);

	_fID = CopyByteArray(id);
	_fHeap = ODRecoverHeapID(somSelf);
	
	ODSession*	session = _fStorageSystem->GetSession(ev);
	ODNameSpaceManager* nameSpaceManager = session->GetNameSpaceManager(ev);
	WASSERTMSG(nameSpaceManager!=kODNULL, "No Name Space Manager", 0);
		
	ODValueNameSpace* nameSpace = (ODValueNameSpace*) nameSpaceManager->HasNameSpace(ev, kODContainerSuitesNameSpace);
	if (nameSpace == kODNULL) {
		nameSpace = (ODValueNameSpace*) nameSpaceManager->CreateNameSpace(ev,
											kODContainerSuitesNameSpace,
											kODNULL,
											kODExpectedNumOfContainerSuites,
											kODNSDataTypeODValue);
	}
	
	CMSession  cmSession = kODNULL;
	if (nameSpace->Exists(ev, kODBentoContainerSuite) == kODFalse) {
#ifdef _PLATFORM_MACINTOSH_
		ODSessionRefCon* sessionRefCon = new(somSelf->GetHeap(ev)) ODSessionRefCon;
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		ODSessionRefCon* sessionRefCon = new ODSessionRefCon;
#endif
		
#ifdef ODDebugBentoSize
		{
			memset(sessionRefCon, 0, sizeof(ODSessionRefCon));
			SOM_TraceLevel = kODTrue;
		}
#endif		
                CreateNameSpace = kODTrue;

		sessionRefCon->container = kODNULL;
		sessionRefCon->heap = somSelf->GetHeap(ev);
		sessionRefCon->cmAllocReserveBlock = kODNULL; // init to null in case alloc fails
		sessionRefCon->cmAllocReserveSize = kODcmAllocReserveSize;
		sessionRefCon->cmAllocReserveBlock = ODNewPtr(kODcmAllocReserveSize, sessionRefCon->heap);
		
		cmSession = CMStartSession(sessionRoutinesMetahandler, sessionRefCon);
		ValueNameSpaceRegister( nameSpace, ev,
								kODBentoContainerSuite,
								(ODPtr)&cmSession,
								sizeof(CMSession));
	}
	else {
		CMSession* cmSessionPtr;
		ODULong length = 0;
		if (ValueNameSpaceGetEntry(nameSpace, ev, kODBentoContainerSuite, 
							  (ODPtr*) &cmSessionPtr, &length))
		{
			cmSession = *cmSessionPtr;
			ODDeleteObject( cmSessionPtr );
		}
		
		WASSERTMSG(length==sizeof(CMSession), "No CMSession", 0);
	}
	_fCMSession = cmSession;
   }
   catch (ODException _exception)
   {
        if(CreateNameSpace)
           ODDeleteObject(sessionRefCon);
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// ODBentoContainer: Create
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODBentoContainerCreate(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","Create");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

//------------------------------------------------------------------------------
// ODBentoContainer: Open
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODBentoContainerOpen(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","Open");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

	return somSelf;
}

//------------------------------------------------------------------------------
// ODBentoContainer: Close
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODBentoContainerClose(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","Close");


	return somSelf;
}

//------------------------------------------------------------------------------
// ODBentoContainer: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODBentoContainerPurge(ODBentoContainer *somSelf, Environment *ev,
		ODSize size)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","Purge");
	
	ODSize	purgedSize = 0;
	
    try
    {
	purgedSize = parent_Purge(somSelf, ev, size);
	if (_fDocument != kODNULL)
		purgedSize = _fDocument->Purge(ev, size-purgedSize);
	
    }
    catch (ODException _exception)
    {
	purgedSize = size;
 	ODSetSOMException(ev, _exception);
    }
	
	return (size - purgedSize);
}

//------------------------------------------------------------------------------
// ODBentoContainer: ReleaseDocument
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODBentoContainerReleaseDocument(ODBentoContainer *somSelf, Environment *ev,
		ODDocument* document)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","ReleaseDocument");

	return somSelf;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetCMContainer
//------------------------------------------------------------------------------

SOM_Scope CMContainer  SOMLINK ODBentoContainerGetCMContainer(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","ODBentoContainerGetCMContainer");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

	return (CMContainer) kODNULL;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetHandlers
//------------------------------------------------------------------------------

SOM_Scope ODBentoHandlers*  SOMLINK ODBentoContainerGetHandlers(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","ODBentoContainerGetHandlers");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

	return (ODBentoHandlers*) kODNULL;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetDirtyFlag
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBentoContainerGetDirtyFlag(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetDirtyFlag");

	return _fDirty;
}

//------------------------------------------------------------------------------
// ODBentoContainer: SetDirtyFlag
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBentoContainerSetDirtyFlag(ODBentoContainer *somSelf, Environment *ev, ODBoolean flag)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","SetDirtyFlag");

	_fDirty = flag;
}


//------------------------------------------------------------------------------
// ODBentoContainer: GetHeap
//------------------------------------------------------------------------------

SOM_Scope ODMemoryHeapID  SOMLINK ODBentoContainerGetHeap(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetHeap");

	return _fHeap;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetCMSession
//------------------------------------------------------------------------------

SOM_Scope CMSession  SOMLINK ODBentoContainerGetCMSession(ODBentoContainer *somSelf, Environment *ev)
{
    ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetHeap");

	if (_fCMSession == kODNULL)
		ODSetSOMException(ev,kODErrBentoNoSession);
		
	return _fCMSession;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetTargetContainer
//------------------------------------------------------------------------------

SOM_Scope ODBentoContainer*  SOMLINK ODBentoContainerGetTargetContainer(ODBentoContainer *somSelf, Environment *ev)
{
//   ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetTargetContainer");

	return kODNULL;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetTargetDocument
//------------------------------------------------------------------------------

SOM_Scope CMDocument*  SOMLINK ODBentoContainerGetTargetDocument(ODBentoContainer *somSelf, Environment *ev)
{
//   ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetTargetDocument");

	return kODNULL;
}

//------------------------------------------------------------------------------
// ODBentoContainer: GetModDate
//------------------------------------------------------------------------------

SOM_Scope ODTime SOMLINK ODBentoContainerGetModDate(ODBentoContainer *somSelf, Environment *ev)
{
	ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","GetModDate");

	return _fModDate;
}

//------------------------------------------------------------------------------
// ODBentoContainer: SetModDate
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODBentoContainerSetModDate(ODBentoContainer *somSelf, Environment *ev, ODTime timer)
{
	ODBentoContainerData *somThis = ODBentoContainerGetData(somSelf);
    ODBentoContainerMethodDebug("ODBentoContainer","SetModDate");

	_fModDate = timer;
}

