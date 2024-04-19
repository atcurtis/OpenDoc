/* @(#)Z 1.19 com/src/storage/ODStor.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:03 (96/10/29 09:31:24) */
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
	File:		ODStor.cpp

	Contains:	Implementation of ODStorageSystem

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<28>	10/24/95	jpa		1293441: VL: Better error handling closing
									containers.
		<27>	 10/8/95	TJ		Fixes Recomended by Refball
		<26>	  9/8/95	VL		1282012: Replaced kODErrContainerNotExists
									with kODErrContainerDoesNotExist.
		<25>	  9/6/95	VL		1279074: Replaced THROW_IF_NULL with
									ODSetSOMException.
		<24>	 8/30/95	EL		1279832: Storage THROW_IF_NULL problems.
		<23>	 8/26/95	T‚		1274606 FB2: Patching Remarks
		<22>	 8/16/95	NP		1274946: ErrorDef.idl problems. Add include
									file.
		<21>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		<20>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<19>	 7/21/95	VL		1270320: Dispose ba._buffer from
									container->GetID.
		<18>	 6/14/95	EL		#1258050: Throw if container suite is
									missing.
		<17>	  6/1/95	jpa		Handle errs thrown from ODNewObject.
									[1242839]
		<16>	 5/26/95	VL		1251403: Multithreading naming support.
		<15>	 4/15/95	VL		1240014: Used new ContainerList::Add which
									does not take a ContainerID.
		<14>	 3/23/95	VL		1230357: implemented NeedSpace.
		<13>	  2/2/95	CG		#1195019:  ValueNameSpace entries are now
									copied in and out instead of pointers being
									passed around.
		<12>	 1/23/95	CG		#1195019: ValueNameSpace methods now take
									ODByteArray parameter instead of ODPtr.
		<11>	11/17/94	CG		#1181489: Turned off STATICBENTO, Bento now
									dynamically linked.
		<10>	11/14/94	VL		1188257: Stopped using Bento errors.
		 <9>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets.
		 <8>	  9/9/94	jpa		Added sufficient SOM_CATCHes to let OpenDoc
									run normally. Many more are needed; I'll
									leave that to Vincent. [1185656]
		 <7>	  9/5/94	VL		1184871: Used Renew to remove dependency on
									default heap.
		 <6>	 8/19/94	T‚		#1181622 Fix Written to Owned & name &
									reference to ASLM Build
		 <5>	 8/17/94	CG		#1181489: Made ready for Container Suite
									instantiation by name.  ASLM BUILD ->
									STATICBENTO.
		 <4>	 8/11/94	VL		1180299: Added CreateTypeList and
									CreatePlatformTypeList.
		 <3>	  8/5/94	VL		Changed ODBaseSession to ODSession.
		 <2>	  8/5/94	VL		1171603: Removed Get/SetCMSession.
		 <1>	  8/3/94	VL		first checked in
		 <4>	 7/29/94	CG		NameSpace API changes.
		 <3>	  7/7/94	VL		Commented out ODRecoverHeapID.
		 <2>	 6/28/94	VL		Use ODGetMemoryHeapID.
		 <1>	 6/28/94	VL		first checked in

	To Do:
		**** ERROR HANDLING!!!! ****
	In Progress:
		
*/

#define ODStorageSystem_Class_Source
#define VARIABLE_MACROS
#include <ODStor.xih>

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _STORPRIV_
#include "StorPriv.h"
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef SOM_ODBinding_xh
#include <ODBindng.xh>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODPlatformTypeList_xh
#include <PfTypLs.xh>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _ODNEW_
#include <ODNew.h>
#endif
#endif

#ifndef _ODMemory_
#include <ODMemory.h>
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
#define STATICBENTO
#endif
//#define STATICBENTO
#ifndef STATICBENTO

	#ifndef _ODNEWOBJ_
	#include "ODNewObj.h"
	#endif
	
	#ifndef __STRING__
	#include <string.h>		// Only for comparing ContainerType. Should be removed when we use DLL.
	#endif

#else

	#ifndef SOM_ODFileContainer_xh
	#include <FileCtr.xh>
	#endif

	#ifndef SOM_ODEmbeddedContainer_xh
	#include <EmbedCtr.xh>
	#endif
	
	#ifndef SOM_ODMemContainer_xh
	#include <MemCtr.xh>
	#endif
	
#endif

#ifdef _PLATFORM_WIN32_
#ifndef SOM_ODIStreamContainer_xh
#include <IStrCtr.xh>
#endif
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
#include <ODUtils.h>
#endif

//==============================================================================
// Constants
//==============================================================================


//==============================================================================
// Function Prototype
//==============================================================================

#ifdef STATICBENTO

static ODFileContainer* NewODFileContainer(ODMemoryHeapID heapID);
static ODMemContainer* NewODMemContainer(ODMemoryHeapID heapID);
static ODEmbeddedContainer* NewODEmbeddedContainer(ODMemoryHeapID heapID);
#ifdef _PLATFORM_WIN32_
static ODIStreamContainer* NewODIStreamContainer(ODMemoryHeapID heapID);
#endif

#endif


#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODStorageSystem
#endif

#include "ODStorB.cpp"	// Platform-independent methods, if any

SOM_Scope ODMemoryHeapID  SOMLINK ODStorageSystemGetHeap(ODStorageSystem *somSelf, Environment *ev)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","GetHeap");

	return _fHeap;
}

SOM_Scope void  SOMLINK ODStorageSystemsomUninit(ODStorageSystem *somSelf)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemsomUninit");

	if (_fContainers)
	{
			Environment* ev = somGetGlobalEnvironment();
 			ContainerListIterator* iter = kODNULL;
		try
                {
			
#ifdef _PLATFORM_MACINTOSH_
			iter = new(_fHeap) ContainerListIterator(_fContainers);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
			iter = new ContainerListIterator(_fContainers);
#endif
			iter->Initialize();
		
			ODContainer*	container = iter->Last();
			while (container != kODNULL) {
				container->Close(ev);
				delete container;
				container = iter->Previous();
			}
			delete iter;
			
			delete _fContainers;
		}
                catch (ODException _exception)
                {
                   ODSetSOMException(ev, _exception);
                   ODDeleteObject(iter);
		}
	}
}

SOM_Scope ODSize  SOMLINK ODStorageSystemPurge(ODStorageSystem *somSelf, Environment *ev,
		ODSize size)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","Purge");

	ODULong sizePurged = 0;
        ContainerListIterator* iter = kODNULL;
	
      try
      {
	
#ifdef _PLATFORM_MACINTOSH_
	iter = new(somSelf->GetHeap(ev)) ContainerListIterator(_fContainers);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
	iter = new ContainerListIterator(_fContainers);
#endif
	iter->Initialize();

	ODContainer*	container = iter->Last();
	while (container != kODNULL) {
		sizePurged += container->Purge(ev, size-sizePurged);
		container = iter->Previous();
	}
	delete iter;
      }

      catch (ODException _exception)
      {
            ODSetSOMException(ev, _exception);
            ODDeleteObject(iter);
	    sizePurged = 0;
      }
	return sizePurged;
}

SOM_Scope void  SOMLINK ODStorageSystemInitStorageSystem(ODStorageSystem *somSelf, Environment *ev,
		ODSession* session)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","InitStorageSystem");

     try
     {
	
	/* Moved from somInit. SOM itself sets fields to zero
	_fSession = kODNULL;
	_fHeap = kDefaultHeapID;
	_fContainers = kODNULL;
	*/
	_fSession = session;
	_fHeap = ODRecoverHeapID(somSelf);
	
#ifdef _PLATFORM_MACINTOSH_
	_fContainers = new(somSelf->GetHeap(ev)) ContainerList;
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
	_fContainers = new ContainerList;
#endif
	_fContainers->Initialize();
	
     }
     catch (ODException _exception)
     {
         _fSession = kODNULL;
         _fContainers = kODNULL;
         ODSetSOMException(ev, _exception);
     }
}

SOM_Scope ODSession*  SOMLINK ODStorageSystemGetSession(ODStorageSystem *somSelf, Environment *ev)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemGetSession");

	return _fSession;
}

SOM_Scope ODContainer*  SOMLINK ODStorageSystemAcquireContainer(ODStorageSystem *somSelf, Environment *ev,
		ODContainerType containerType,
		ODContainerID* id)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemAcquireContainer");
    
	ODContainer*	container = kODNULL;

    try 
    {
	
	if (id == kODNULL)
		THROW(kODErrIllegalNullIDInput);

	if ((container = _fContainers->Get(id)) != kODNULL) {
		container->Acquire(ev);
	}
	else {
#ifndef STATICBENTO

		ODContainerSuite theContainerSuite = 
			_fSession->GetBinding(ev)->GetContainerSuite(ev,containerType);
		
		if ( theContainerSuite )
		{
			try
                        {
				container = (ODContainer*) ODNewObject(theContainerSuite);
                        }
			catch (ODException _exception)
                        {
				ODDisposePtr( theContainerSuite );
				THROW( ErrorCode()==kODErrCantLoadSOMClass ?kODErrCannotCreateContainer
														   :ErrorCode() );
			}
			ODDisposePtr( theContainerSuite );
		}
		else
			THROW( kODErrCannotCreateContainer );
			
#else
		if (containerType == kODNULL)
			THROW(kODErrIllegalNullInput);

		// Hardwire for ODContainer for now. We should do the dynamic loading
		//	of the class using containerType in the future.

		if (strcmp(kODDefaultFileContainer, containerType) == 0)
			container = NewODFileContainer(somSelf->GetHeap(ev));
		else if (strcmp(kODBentoEmbeddedContainer, containerType) == 0)
			container = NewODEmbeddedContainer(somSelf->GetHeap(ev));
		else if (strcmp(kODDefaultMemoryContainer, containerType) == 0)
			container = NewODMemContainer(somSelf->GetHeap(ev));
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
		else if (strcmp(kODBentoMemoryContainer, containerType) == 0)
			container = NewODMemContainer(somSelf->GetHeap(ev));
#if defined(_PLATFORM_WIN32_)
		else if (strcmp(kODBentoIStreamContainer, containerType) == 0)
			container = NewODIStreamContainer(somSelf->GetHeap(ev));
#endif // defined(_PLATFORM_WIN32_)
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_
#if OD_BUG
		else THROW(kODErrCannotCreateContainer);
#endif
			
#endif			
		container->InitContainer(ev, somSelf, id);
		
		_fContainers->Add(container);
		
		container->Open(ev);
	}
     }

     catch (ODException _exception)
     {
        container = kODNULL;
        ODSetSOMException(ev, _exception);
     }
	return container;
}

SOM_Scope ODContainer*  SOMLINK ODStorageSystemCreateContainer(ODStorageSystem *somSelf, Environment *ev,
		ODContainerType containerType,
		ODContainerID* id)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemCreateContainer");


	ODContainer*	container = kODNULL;

     try
     {
	
	if (id == kODNULL)
		THROW(kODErrIllegalNullIDInput);
	if ((container = _fContainers->Get(id)) != kODNULL)
		THROW(kODErrContainerExists);
	if (containerType == kODNULL)
		THROW(kODErrIllegalNullInput);
	
#ifndef STATICBENTO

	ODContainerSuite theContainerSuite = 
		_fSession->GetBinding(ev)->GetContainerSuite(ev,containerType);
	
	if ( theContainerSuite )
	{
		try
                {
			container = (ODContainer*) ODNewObject(theContainerSuite);
		catch (ODException _exception)
                {
			ODDisposePtr( theContainerSuite );
			THROW( ErrorCode()==kODErrCantLoadSOMClass ?kODErrCannotCreateContainer
													   :ErrorCode() );
		}
		ODDisposePtr( theContainerSuite );
	}
	else
		THROW( kODErrCannotCreateContainer );

#else
	// Hardwire for the two Bento Containers for now. We should do the dynamic loading
	//	of the class using containerType in the future.

	if (strcmp(kODDefaultFileContainer, containerType) == 0)
		container = NewODFileContainer(somSelf->GetHeap(ev));
	else if (strcmp(kODBentoEmbeddedContainer, containerType) == 0)
			container = NewODEmbeddedContainer(somSelf->GetHeap(ev));
	else if (strcmp(kODDefaultMemoryContainer, containerType) == 0)
			container = NewODMemContainer(somSelf->GetHeap(ev));
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
	else if (strcmp(kODBentoMemoryContainer, containerType) == 0)
		container = NewODMemContainer(somSelf->GetHeap(ev));
#if defined(_PLATFORM_WIN32_)
	else if (strcmp(kODBentoIStreamContainer, containerType) == 0)
		container = NewODIStreamContainer(somSelf->GetHeap(ev));
#endif // defined(_PLATFORM_WIN32_)
#endif
#if OD_BUG
	else THROW(kODErrCannotCreateContainer);
#endif
			
#endif

	container->InitContainer(ev, somSelf, id);
	
	_fContainers->Add(container);
	
	container->Create(ev);
	container->Open(ev);

     }
     catch (ODException _exception)
     {
	container = kODNULL;
        ODSetSOMException(ev, _exception);
     }
	return container;
}

SOM_Scope void  SOMLINK ODStorageSystemNeedSpace(ODStorageSystem *somSelf, Environment *ev,
		ODSize memSize,
		ODBoolean doPurge)
{
    //ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemNeedSpace");

	if (doPurge != kODFalse) {
		try
                {
		   somSelf->Purge(ev, memSize);
                }
		catch (ODException _exception)
                {
                   ODSetSOMException(ev, _exception);
		}
	}
}

SOM_Scope ODTypeList*  SOMLINK ODStorageSystemCreateTypeList(ODStorageSystem *somSelf, Environment *ev,
		ODTypeList* typeList)
{
    //ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemCreateTypeList");

	ODTypeList* tmp; ODVolatile(tmp);
	
	try
        {
	   tmp = new ODTypeList;
           if(tmp == kODNULL)
	      THROW(kODErrOutOfMemory);
           else
	       tmp->InitTypeList(ev, typeList);
	}
	catch (ODException  _exception)
        {
	    ODDeleteObject(tmp);
            ODSetSOMException(ev, _exception);
	}
	return tmp;
}

SOM_Scope ODPlatformTypeList*  SOMLINK ODStorageSystemCreatePlatformTypeList(ODStorageSystem *somSelf, Environment *ev,
		ODPlatformTypeList* typeList)
{
    //ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemNeedSpace");
	
	ODPlatformTypeList* tmp  = kODNULL;
	
     try
     {

	tmp = new ODPlatformTypeList;
	if (tmp == kODNULL)
		THROW(kODErrOutOfMemory);
	else
		tmp->InitPlatformTypeList(ev, typeList);
		
     }
     catch (ODException _exception)
     {
	ODDeleteObject(tmp);
        ODSetSOMException(ev, _exception);
     }

	
	return tmp;
}


SOM_Scope void  SOMLINK ODStorageSystemGrowHeap(ODStorageSystem *somSelf, Environment *ev,
		ODSize memSize)
{
    //ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemGrowHeap");

ODUnused(memSize);
}

SOM_Scope ODStorageSystem*  SOMLINK ODStorageSystemReleaseContainer(ODStorageSystem *somSelf, Environment *ev,
		ODContainer* container)
{
    ODStorageSystemData *somThis = ODStorageSystemGetData(somSelf);
    ODStorageSystemMethodDebug("ODStorageSystem","ODStorageSystemReleaseContainer");

     try
     {

	ODContainerID containerID = container->GetID(ev);
		
	if ((container = _fContainers->Get(&containerID)) != kODNULL) {
		_fContainers->Remove(&containerID);
		ODDisposePtr(containerID._buffer);
		try
                {
			container->Close(ev);
			delete container;
                }
		catch (ODException _exception)
                {
			SetErrorCode(kODNoError);
		}
	}
	else
	{
		ODDisposePtr(containerID._buffer);
		THROW(kODErrContainerDoesNotExist);
	}
		
     }
     catch (ODException _exception)
     {
         ODSetSOMException(ev, _exception);
         return kODNULL;
     }
	return somSelf;
}

#ifdef STATICBENTO

//------------------------------------------------------------------------------
// NewODFileContainer
//------------------------------------------------------------------------------

static ODFileContainer* NewODFileContainer(ODMemoryHeapID heapID)
{
	Environment* ev = somGetGlobalEnvironment();
        ODFileContainer* fileCtr = kODNULL;
     try
     {
#ifdef _PLATFORM_MACINTOSH_
	SOMClass*	fileCtrClass = ODFileContainerNewClass(ODFileContainer_MajorVersion,
														ODFileContainer_MinorVersion);
	ODULong		size = fileCtrClass->somGetInstanceSize();
	ODPtr		buffer = ODNewPtr(size, heapID);
	fileCtr = (ODFileContainer*) fileCtrClass->somRenew(buffer);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)

	fileCtr = new ODFileContainer;
#endif
     }
     catch (ODException _exception)
     {
           ODSetSOMException(ev, _exception);
           ODDeleteObject(fileCtr);
     }

	return fileCtr;
}

//------------------------------------------------------------------------------
// NewODMemContainer
//------------------------------------------------------------------------------

static ODMemContainer* NewODMemContainer(ODMemoryHeapID heapID)
{
	Environment* ev = somGetGlobalEnvironment();
        ODMemContainer* memCtr = kODNULL;
     try
     {
#ifdef _PLATFORM_MACINTOSH_
	SOMClass*	memCtrClass = ODMemContainerNewClass(ODMemContainer_MajorVersion,
														ODMemContainer_MinorVersion);
	ODULong		size = memCtrClass->somGetInstanceSize();
	ODPtr		buffer = ODNewPtr(size, heapID);
	memCtr = (ODMemContainer*) memCtrClass->somRenew(buffer);
#endif
	
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
	memCtr = new ODMemContainer;
#endif
     }
     catch (ODException _exception)
     {
           ODSetSOMException(ev, _exception);
           ODDeleteObject(memCtr);
     }
	return memCtr;
}

//------------------------------------------------------------------------------
// NewODEmbeddedContainer
//------------------------------------------------------------------------------

static ODEmbeddedContainer* NewODEmbeddedContainer(ODMemoryHeapID heapID)
{
	Environment* ev = somGetGlobalEnvironment();
        ODEmbeddedContainer* embeddedCtr = kODNULL;
     try
     {
#ifdef _PLATFORM_MACINTOSH_
	SOMClass*	embeddedCtrClass = ODEmbeddedContainerNewClass(ODEmbeddedContainer_MajorVersion,
																ODEmbeddedContainer_MinorVersion);
	ODULong		size = embeddedCtrClass->somGetInstanceSize();
	ODPtr		buffer = ODNewPtr(size, heapID);
	embeddedCtr = (ODEmbeddedContainer*) embeddedCtrClass->somRenew(buffer);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
	embeddedCtr = new ODEmbeddedContainer;
#endif
     }
     catch (ODException _exception)
     {
           ODSetSOMException(ev, _exception);
           ODDeleteObject(embeddedCtr);
     }
	return embeddedCtr;
}

//------------------------------------------------------------------------------
// NewODIStreamContainer
//------------------------------------------------------------------------------

#if defined(_PLATFORM_WIN32_)
static ODIStreamContainer* NewODIStreamContainer(ODMemoryHeapID heapID)
{
	Environment* ev = somGetGlobalEnvironment();
	ODIStreamContainer* istreamCtr  = kODNULL;
     try
     {
	istreamCtr = new ODIStreamContainer;
     }
     catch (ODException _exception)
     {
           ODSetSOMException(ev, _exception);
           ODDeleteObject(istreamCtr);
     }

	return istreamCtr;
}
#endif

#endif



