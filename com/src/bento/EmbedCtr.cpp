/* @(#)Z 1.14 com/src/bento/EmbedCtr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:31 (96/10/29 09:12:12) */
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
	File:		EmbedCtr.cpp

	Contains:	Implementation for ODEmbeddedContainer.

	Owned by:	Vincent Lo

	Copyright:	© 1993-1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<27>	10/24/95	jpa		1293441: DM/VL: Bento memory reserve &
									fatal container err.
		<26>	 10/8/95	TJ		Fixes Recomended by Refball
		<25>	 8/21/95	VL		1278330, 1278315: Error code cleanup.
		<24>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<23>	 7/21/95	VL		1270320: Dispose ba._buffer from
									container->GetID. Dispose prevObjectName
									when done. Delete _fHandlers in somUninit.
		<22>	 5/26/95	VL		1251403: Multithreading naming support.
		<21>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<20>	 4/25/95	VL		1210982: Removed 5$.
		<19>	 3/31/95	EL		1234685: Get merge target during open
									rather than close, this would ensure
									getting the right target and not some
									corrrupted embedded container.
		<18>	 3/24/95	EL		1182275: We don't need to try to merge if
									it is read only.
		<17>	  3/6/95	EL		1182275: We don't need
									DraftPropertiesObject.
		<16>	 2/21/95	EL		1182275: Add SetDraftPropertiesObject and
									call CMKeepObject with it before closing.
		<15>	 2/17/95	EL		1182275: Actually merge embedded
									containers.
		<14>	 2/15/95	EL		1182275: Add private method SetMergeFlag.
		<13>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		<12>	 11/1/94	VL		1151339: Preparation for MergeContainer.
		<11>	10/19/94	VL		1155857: Commented out check for
									cmContainer permission in Close. This
									should fix "Cannot create  more than 10
									drafts" problem.
		<10>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets. This affects EmbeddedContainers
									also.
		 <9>	 8/26/94	EL		#1182275 Include code (not yet active) for
									merging of container.
		 <8>	 8/10/94	NP		Added QDFixM.h.
		 <7>	  8/5/94	VL		1171603: Use somSelf->GetCMSession instead
									of depending on ODStorageSystem.
		 <6>	  8/3/94	VL		1153123: Storage to ODStor.
		 <5>	 7/11/94	VL		Fixed a bug in uninit. Close should not be
									called on an Embedded Container. (Bug was
									introduced during SOM conversion).
		 <4>	  7/7/94	VL		Fixed the reversal of conditional in Abort
									(this bug was introduced during the SOM
									conversion).
		 <3>	  7/5/94	VL		Added GetUseMode.
		 <2>	  7/1/94	jpa		Fixed implicit casts from void*.
		 <1>	 6/30/94	CC		first checked in
		 <0>	 6/28/94	SV		SOMverted
		 <2>	 4/13/94	VL		1157028: StorageM.h does not include
									CMAPI.h anymore, so GetCMSession returns
									ODULong.
		 <1>	 3/24/94	VL		first checked in
		 <3>	  3/1/94	JA		Moved string const here from .h file.
									(1147295)
		 <2>	 2/17/94	JA		Removed bogus quote in include statement.
		<15>	  2/8/94	VL		Correct capitalization and <> for includes.
		<14>	  2/8/94	VL		Use new exception macros.
		<13>	 1/19/94	EL		Use kCMNextGeneration when opening new
									container for update.
		<12>	 1/12/94	VL		Init changes.
		<11>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		<10>	11/23/93	VL		Made EmbeddedContainer pool-aware.
		 <9>	  8/9/93	VL		Moved error code to ODError.h.
		 <8>	 7/30/93	VL		ODContainerName* should be
									ODContainerName.
		 <7>	  7/6/93	VL		ODContainerName should be
									ODContainerName*.
		 <6>	 6/30/93	VL		Changed ODName to ODType.
		 <5>	 6/30/93	VL		Removed ODDocument::Reopen in Abort() &
									Close(). Open() checks for fContainer
									instead of fUseMode.
		 <4>	 6/22/93	VL		Used RefCount from ODRefCntObject. Added
									Abort.
		 <3>	 6/15/93	VL		Reopen drafts after Close.
		 <2>	  6/1/93	VL		Changed open mode on CMOpenContainer in
									Open.
		 <1>	 5/27/93	VL		first checked in

	To Do:
		Eliminate shouldMerge in containerID.
	In Progress:
		
*/

#ifdef _PLATFORM_MACINTOSH_
#ifndef _QDFIXM_
#include "QDFixM.h"
#endif
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

#define ODEmbeddedContainer_Class_Source
#define VARIABLE_MACROS
#include <EmbedCtr.xih>

#ifndef SOM_ODRefCntObject_xh
#include <RefCtObj.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"		// for sessionRoutinesMetahandler
#endif

#ifndef _EMBEDHDR_
#include "EmbedHdr.h"
#endif

#ifndef _TARGTHDR_
#include "TargtHdr.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _INDHDR_
#include "IndHdr.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef __STRING__
#include <string.h>		// for strlen
#endif



//==============================================================================
// Constants
//==============================================================================
const ODType kODEmbeddedContainerTypeName = "EmbedHdr";

#if ODDebug
// #define ODDebug_EmbedCtr 1
#endif

#pragma segment EmbedCtr

//==============================================================================
// ODEmbeddedContainer
//==============================================================================

//------------------------------------------------------------------------------
// ODEmbeddedContainer: GetName
//------------------------------------------------------------------------------

SOM_Scope ODContainerName  SOMLINK ODEmbeddedContainerGetName(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","GetName");

	WARNMSG(WARN_INDEX(AMSG_680),"EmbeddedContainer does not have a name.");
	
	// The following two lines are used to pacify the compiler.
	// They should never be executed.
	ODIText	dummy;
	return 	dummy;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: SetName
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODEmbeddedContainerSetName(ODEmbeddedContainer *somSelf, Environment *ev,
		ODContainerName* name)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","SetName");

	// This function is an no-op.
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: AcquireDocument
//------------------------------------------------------------------------------

SOM_Scope ODDocument*  SOMLINK ODEmbeddedContainerAcquireDocument(ODEmbeddedContainer *somSelf, Environment *ev,
		ODDocumentID id)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","AcquireDocument");

ODUnused(id);

	return kODNULL;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODEmbeddedContainerRelease(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","Release");

	ODEmbeddedContainer_parent_ODBentoContainer_Release(somSelf, ev);
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: ~ODEmbeddedContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODEmbeddedContainersomUninit(ODEmbeddedContainer *somSelf)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","somUninit");

#ifdef DebugRefCount
	printf("~ODEmbeddedContainer %x CMContainer %x\n", somSelf, _fContainer);
	fflush(stdout);
#endif

	Environment* ev = somGetGlobalEnvironment ();
    try
    {

//	if (_fCMContainer != kODNULL)
//		somSelf->Close(ev);

	if (_fHandlers != kODNULL)
		delete _fHandlers;

//	Theoretically, we should not delete the ID because they should be able to 
//	be reused.
//
//	ODContainerID id = somSelf->GetID(ev);
//	if (id != kXMPNULL)
//		delete id;

    ODEmbeddedContainer_parent_ODBentoContainer_somUninit(somSelf);
    }
    catch(ODException _exception)
    {
          ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: InitContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODEmbeddedContainerInitContainer(ODEmbeddedContainer *somSelf, Environment *ev,
		ODStorageSystem* storage,
		ODContainerID* id)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","InitContainer");
 
        ODByteArray ba; 
    try
    {

	/* Moved from somInit. SOM itself sets fields to zero
	_fCMContainer = kODNULL;
	_fHandlers = kODNULL;
	_fTargetValue = kODNULL;
	*/
	_fUseMode = kCMReading;
	_fShouldMerge = kODTrue;

	ODEmbeddedContainer_parent_ODBentoContainer_InitContainer(somSelf, ev, storage, id);

	
	ba = somSelf->GetID(ev);
	ODEmbeddedContainerID	containerID = *((ODEmbeddedContainerID*) ba._buffer);
//	_fShouldMerge = containerID.shouldMerge;
#ifdef _PLATFORM_MACINTOSH_
	_fHandlers = new(somSelf->GetHeap(ev)) ODEmbeddedHandlers(somSelf->GetCMSession(ev),
																containerID.cmValue);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	_fHandlers = new ODEmbeddedHandlers(somSelf->GetCMSession(ev),
					containerID.cmValue);
#endif
	_fHandlers->Initialize();

	ODDisposePtr(ba._buffer);

     }
     catch (ODException _exception)
     {
       if(ba._buffer)
 	 ODDisposePtr(ba._buffer);
         ODSetSOMException(ev, _exception);
     }
 
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: Create
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODEmbeddedContainerCreate(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","Create");

    try
    {

	if (_fCMContainer == kODNULL) {

		CMSession cmSession = somSelf->GetCMSession(ev);
		if (cmSession == kODNULL)
			THROW(kODErrCannotCreateContainer);

//		ODType	prevObjectName = _fID->GetPrevObjectName();

		ODByteArray	ba = somSelf->GetID(ev);
		CMValue cmValue = *((CMValue*) ba._buffer);
		ODType prevObjectName = (ODType) CMGetValueRefCon(cmValue);
		ODDisposePtr(ba._buffer);

		// Just to be safe, we are calling this even though we may have called this
		// already for some other containers.
		CMSetMetaHandler(cmSession,
						(CMGlobalName) CMTargetHandlersTypeName,
						targetContainerMetahandler);

		CMSetMetaHandler(cmSession, 
							kODIndirectValueGlobalName, 
							IndirectDynamicValueMetahandler);

		if (prevObjectName != kODNULL) {
#if MergeContainersAvailable
			CMValue 			embeddedValue;
			CMContainer			fileContainer;
			CMObject			embeddedObject;
			CMProperty			embeddedCntProp, targetObjectProp;
			CMType				embeddedCntType;
			CMObject			prevObject;
			
			_fCMContainer = CMOpenNewContainer(cmSession,
										_fHandlers,
										kODEmbeddedContainerTypeName,
										(CMContainerUseMode) (kCMUpdateTarget+kCMMerging+kCMReuseFreeSpace),
										kCMNextGeneration, kCMDefaultEndian,
										prevObjectName);
			if (_fCMContainer == kODNULL)
				THROW(kODErrCannotCreateContainer);
			ODVolatile(embeddedObject);
			ODVolatile(embeddedCntProp);
			ODVolatile(embeddedCntType);
			try
                        {		
				ODSessionMustHaveCMAllocReserve(_fCMContainer);
				embeddedValue = _fHandlers->ReturnParentValueHandler();
				CMGetValueInfo(embeddedValue, &fileContainer, &embeddedObject, 
								&embeddedCntProp, &embeddedCntType, kODNULL);
				targetObjectProp = CMRegisterProperty(fileContainer, prevObjectName);
				CMGetValueInfo(embeddedValue, &fileContainer, &embeddedObject, 
								&embeddedCntProp, &embeddedCntType, NULL);
				prevObject = CMGetPrevObjectWithProperty(fileContainer, embeddedObject, embeddedCntProp);
				embeddedObject = CMGetPrevObjectWithProperty(fileContainer, embeddedObject, targetObjectProp);
				if (prevObject == embeddedObject)
					CMReleaseObject(prevObject);
				else { 	/* we suspect this is garbage, check if we should delete it 	*/
					try
                                        { /* if anything fails, garbage object is not deleted, big deal	*/
						embeddedValue = CMUseValue(prevObject, embeddedCntProp, embeddedCntType);
						CMReleaseValue(embeddedValue);
						if (!CMContainsValidLabel(embeddedValue)) {
							CMReleaseObject(prevObject);
							CMDeleteObject(prevObject);
						}
						else
							CMReleaseObject(prevObject);
                                        }
					catch (ODException _exception)
                                        {
                                            ODSetSOMException(ev, _exception);
                                        }
				}
				embeddedValue = CMUseValue(embeddedObject, embeddedCntProp, embeddedCntType);
				ODSessionRestoreCMAllocReserve(_fCMContainer);
                        }
			catch (ODException _exception)
                        {
				embeddedValue = kODNULL;
                                ODSetSOMException(ev, _exception);
                        }
			_fTargetValue = embeddedValue;
#else
			_fCMContainer = CMOpenNewContainer(cmSession,
										_fHandlers,
										kODEmbeddedContainerTypeName,
										(CMContainerUseMode) (kCMUpdateTarget),
										kCMNextGeneration, kCMDefaultEndian,
										prevObjectName);
#endif
			_fUseMode = kCMUpdateTarget;
			ODDisposePtr(prevObjectName);
			CMSetValueRefCon(cmValue, kODNULL);
		}
		else {
			_fCMContainer = CMOpenNewContainer(cmSession,
										_fHandlers,
										kODEmbeddedContainerTypeName,
										(CMContainerUseMode) (kCMWriting),
										kCMNextGeneration, kCMDefaultEndian);
			_fUseMode = kCMWriting;
		}
		
		if (_fCMContainer == kODNULL)
			THROW(kODErrCannotCreateContainer);
			
	}


     }
     catch (ODException _exception)
     {
          ODSetSOMException(ev, _exception);
     }
	return somSelf;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: Open
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODEmbeddedContainerOpen(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","Open");

   try
   {

	if (_fCMContainer == kODNULL) {

		CMSession cmSession = somSelf->GetCMSession(ev);
		if (cmSession == kODNULL)
			THROW(kODErrCannotOpenContainer);

		// Just to be safe, we are calling this even though we may have called this
		// already for some other containers.
		CMSetMetaHandler(cmSession,
						(CMGlobalName) CMTargetHandlersTypeName,
						targetContainerMetahandler);

		CMSetMetaHandler(cmSession, 
							kODIndirectValueGlobalName, 
							IndirectDynamicValueMetahandler);

		_fCMContainer = CMOpenContainer(cmSession,
										_fHandlers,
										kODEmbeddedContainerTypeName,
										(CMContainerUseMode)(kCMReading));

		ODSessionRestoreCMAllocReserve(_fCMContainer);
		
		if (_fCMContainer == kODNULL)
			THROW(kODErrCannotOpenContainer);

		_fUseMode = kCMReading;
	}
		
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
       
	return somSelf;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: Close
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODEmbeddedContainerClose(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","Close");
	
   try
   {

	CMContainer cmContainer = somSelf->GetCMContainer(ev);
	if (_fCMContainer != kODNULL) {
			ODSessionMustHaveCMAllocReserve(_fCMContainer);
//		if (_fUseMode != kCMReading) {
#if MergeContainersAvailable
			if (_fShouldMerge && (_fUseMode != kCMReading)) {
#ifdef ODDebug_EmbedCtr
	somPrintf("Closing EmbedCtr: Merge\n");
#endif
				CMMergeContainer(_fCMContainer, _fTargetValue);
			}
			else {
#ifdef ODDebug_EmbedCtr
	somPrintf("Closing EmbedCtr: Close\n");
#endif
				CMCloseContainer(_fCMContainer);
			}
			if (_fTargetValue) {
				CMReleaseValue(_fTargetValue);
				_fTargetValue = kODNULL;
			}

#else
			CMCloseContainer(_fCMContainer);
#endif
			_fCMContainer = kODNULL;
//		}
	}
	
        }
	catch (ODException _exception)
        {
            ODSetSOMException(ev, _exception);
        }
	return somSelf;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODEmbeddedContainerPurge(ODEmbeddedContainer *somSelf, Environment *ev,
		ODSize size)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","Purge");

	return ODEmbeddedContainer_parent_ODBentoContainer_Purge(somSelf, ev, size);
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: ReleaseDocument
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODEmbeddedContainerReleaseDocument(ODEmbeddedContainer *somSelf, Environment *ev,
		ODDocument* document)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","ReleaseDocument");

ODUnused(document);

	return somSelf;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: Abort
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODEmbeddedContainerAbort(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","Abort");


	CMSize valueSize;
        ODByteArray ba;

	if (_fCMContainer != kODNULL) {
		if (_fUseMode != kCMReading) {
			try
                        {
				ODSessionMustHaveCMAllocReserve(_fCMContainer);
				CMAbortContainer(_fCMContainer);
				
				// Trunc the value as Bento does not do it for us.
				ba = somSelf->GetID(ev);
				CMValue value = *((CMValue*) ba._buffer);
				ODDisposePtr(ba._buffer);
				valueSize = CMGetValueSize(value);
				CMDeleteValueData(value, 0, valueSize);
                        }
			catch (ODException _exception)
                        {
                          if(ba._buffer)
		             ODDisposePtr(ba._buffer);
 			     ODSetSOMException(ev, _exception);
                        }
			_fCMContainer = kODNULL;
		}
	}
	
	return somSelf;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: GetCMContainer
//------------------------------------------------------------------------------

SOM_Scope CMContainer  SOMLINK ODEmbeddedContainerGetCMContainer(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","GetCMContainer");

	return _fCMContainer;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: GetHandlers
//------------------------------------------------------------------------------

SOM_Scope ODBentoHandlers*  SOMLINK ODEmbeddedContainerGetHandlers(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","GetHandlers");

	return _fHandlers;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: GetUseMode
//------------------------------------------------------------------------------

SOM_Scope CMContainerUseMode  SOMLINK ODEmbeddedContainerGetUseMode(ODEmbeddedContainer *somSelf, Environment *ev)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","GetHandlers");

	return _fUseMode;
}

//------------------------------------------------------------------------------
// ODEmbeddedContainer: SetMergeFlag
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODEmbeddedContainerSetMergeFlag(ODEmbeddedContainer *somSelf, Environment *ev,
		ODBoolean flag)
{
    ODEmbeddedContainerData *somThis = ODEmbeddedContainerGetData(somSelf);
    ODEmbeddedContainerMethodDebug("ODEmbeddedContainer","SetMergeFlag");

	_fShouldMerge = flag;
}

