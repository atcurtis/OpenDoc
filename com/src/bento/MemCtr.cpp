/* @(#)Z 1.11 com/src/bento/MemCtr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:34 (96/10/29 09:12:58) */
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
	File:		MemCtr.cpp

	Contains:	Implementation of ODMemContainer class.

	Owned by:	Paul Hartenstine

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<13>	10/24/95	jpa		1293441: DM/VL: Bento memory reserve &
									fatal container err.
		<12>	 10/8/95	TJ		Fixes Recomended by Refball
		<11>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		<10>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <9>	 5/26/95	VL		1251403: Multithreading naming support.
		 <8>	 3/23/95	VL		1230357: Implemented Purge.
		 <7>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <6>	 11/1/94	VL		1194188: Implemented unimplemented
									functions.
		 <5>	 9/29/94	RA		1189812: Mods for 68K build.
		 <4>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets. 1184166: Implemented GetName and
									SetName. (still commented out).
		 <3>	  8/5/94	VL		1171603: Use somSelf->GetCMSession instead
									of depending on ODStorageSystem.
		 <2>	  8/3/94	VL		1153123: Storage to ODStor.
		 <1>	 6/30/94	CC		first checked in
		 <2>	 4/13/94	VL		1157028: StorageM.h does not include
									CMAPI.h anymore, so GetCMSession returns
									ODULong.
		 <1>	 3/24/94	VL		first checked in
		<10>	  2/8/94	VL		Use new exception macros.
		 <9>	 1/12/94	VL		Init changes.
		 <8>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		 <7>	11/23/93	VL		Made ODMemContainer pool-aware.
		 <6>	  9/9/93	VL		Moved session global code to ODDraft,39.
		 <5>	  8/9/93	VL		Moved error code to ODError.h.
		 <4>	  8/6/93	VL		fContainer should be fCMContainer.
		 <3>	  8/6/93	PH		Specify all types in calls to Bento
		 <2>	  8/3/93	PH		Fix includes
		 <1>	  8/2/93	PH		first checked in
	To Do:
	In Progress:
		
*/

#define ODMemContainer_Class_Source

#define VARIABLE_MACROS

#include <MemCtr.xih>

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef _MEMHDR_
#include "MemHdr.h"
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

#ifndef _ODNEW_
#include <ODNew.h>
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#pragma segment ODMemContainer

//==============================================================================
// ODMemContainer
//==============================================================================

//------------------------------------------------------------------------------
// ODMemContainer: ~ODMemContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODMemContainersomUninit(ODMemContainer *somSelf)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","somUninit");

#ifdef DebugRefCount
	printf("~ODMemContainer %x CMContainer %x RefCount %d\n", somSelf, _fCMContainer, _fRefCount);
	fflush(stdout);
#endif
		Environment *ev = somGetGlobalEnvironment ();

	try
        {
		if (_fCMContainer != kODNULL)
			somSelf->Close(ev);
	
		if (_fHandlers != kODNULL)
			delete _fHandlers;
	
		if (_fName != kODNULL)
			DisposeIText(_fName);
	}
        catch (ODException _exception)
        {
            SetErrorCode(kODNoError);
		// Ignore any errors
	}
}

//------------------------------------------------------------------------------
// ODMemContainer: InitContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODMemContainerInitContainer(ODMemContainer *somSelf, Environment *ev,
		ODStorageSystem* storage, ODContainerID* id)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","InitContainer");
	

    try
    {
	/* Moved from somInit. SOM itself sets fields to zero
	_fCMContainer = kODNULL;
	_fHandlers = kODNULL;
	_fName = kODNULL;
	*/
	
	ODMemContainer_parent_ODBentoContainer_InitContainer(somSelf, ev, storage, id);
	
#ifdef _PLATFORM_MACINTOSH_
	_fHandlers = new(somSelf->GetHeap(ev)) ODMemBentoHandlers(somSelf->GetCMSession(ev),
															 *((ODHandle*) id->_buffer));
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	_fHandlers = new ODMemBentoHandlers(somSelf->GetCMSession(ev),
					 *((ODHandle*) id->_buffer));
#endif
	_fHandlers->Initialize();
	
	_fName = CreateIText(0);
   }
   catch (ODException _exception)
   {
	if (_fHandlers != kODNULL)
	    delete _fHandlers;
        _fHandlers = kODNULL;
        DisposeIText(_fName);
        ODSetSOMException(ev, _exception);
   }

}

//------------------------------------------------------------------------------
// ODMemContainer: GetName
//------------------------------------------------------------------------------

SOM_Scope ODContainerName  SOMLINK ODMemContainerGetName(ODMemContainer *somSelf, Environment *ev)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","GetName");
    
	return *_fName;
}

//------------------------------------------------------------------------------
// ODMemContainer: SetName
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODMemContainerSetName(ODMemContainer *somSelf, Environment *ev,
		ODContainerName* name)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","SetName");

    try
    {
	
        if (_fName != kODNULL) {
    	    DisposeIText(_fName);
        }
        _fName = CopyIText(name);

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// ODMemContainer: Create
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODMemContainerCreate(ODMemContainer *somSelf, Environment *ev)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","Create");

    try
    {
	
	if (_fCMContainer == kODNULL) {
	
		CMSession cmSession = somSelf->GetCMSession(ev);

		CMSetMetaHandler(cmSession,
						(CMGlobalName)CMTargetHandlersTypeName,
						targetContainerMetahandler);
						
		_fCMContainer = CMOpenNewContainer(cmSession,
										_fHandlers,
										(CMGlobalName) kODBentoMemoryTypeName,
										(CMContainerUseMode)(kCMWriting),
										1, kCMDefaultEndian);

		if (_fCMContainer == kODNULL)
			THROW(kODErrCannotCreateContainer);
		ODSessionRestoreCMAllocReserve(_fCMContainer);

	}

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

//------------------------------------------------------------------------------
// ODMemContainer: Open
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODMemContainerOpen(ODMemContainer *somSelf, Environment *ev)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","Open");


    try
    {
	if (_fCMContainer == kODNULL) {
	
		CMSession cmSession = somSelf->GetCMSession(ev);

		CMSetMetaHandler(cmSession,
						(CMGlobalName)CMTargetHandlersTypeName,
						targetContainerMetahandler);
						
		_fCMContainer = CMOpenNewContainer(cmSession,
										_fHandlers,
										(CMGlobalName) kODBentoMemoryTypeName,
										(CMContainerUseMode)(kCMUpdateByAppend),
										1, kCMDefaultEndian);

		if (_fCMContainer == kODNULL)
			THROW(kODErrCannotOpenContainer);

		ODSessionRestoreCMAllocReserve(_fCMContainer);
	}

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

//------------------------------------------------------------------------------
// ODMemContainer: Close
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODMemContainerClose(ODMemContainer *somSelf, Environment *ev)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","Close");

	if (_fCMContainer != kODNULL) {
		try
                {
			ODSessionMustHaveCMAllocReserve(_fCMContainer);
			if (somSelf->GetDirtyFlag(ev) != kODFalse)
				CMCloseContainer(_fCMContainer);
			else
				CMAbortContainer(_fCMContainer);
                }
		catch (ODException _exception)
                {
			if (ErrorCode() == kODErrBentoErr)
                        {
				SetErrorCode(kODErrFatalContainerError);
                                ODSetSOMException(ev, _exception);
                        }
                }
		_fCMContainer = kODNULL;
	}
	
	return ODMemContainer_parent_ODBentoContainer_Close(somSelf, ev);
}

//------------------------------------------------------------------------------
// ODMemContainer: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODMemContainerPurge(ODMemContainer *somSelf, Environment *ev,
		ODSize size)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","Purge");

    ODSize odsize = 0;

     try
     {

	odsize =  parent_Purge(somSelf, ev, size);
    
     }
     catch (ODException _exception)
     {
	odsize =  0;
        ODSetSOMException(ev, _exception);
     }
        return odsize;
}

//------------------------------------------------------------------------------
// ODMemContainer: GetCMContainer
//------------------------------------------------------------------------------

SOM_Scope CMContainer  SOMLINK ODMemContainerGetCMContainer(ODMemContainer *somSelf, Environment *ev)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","GetCMContainer");

	return _fCMContainer;
}

//------------------------------------------------------------------------------
// ODMemContainer: GetHandlers
//------------------------------------------------------------------------------

SOM_Scope ODBentoHandlers*  SOMLINK ODMemContainerGetHandlers(ODMemContainer *somSelf, Environment *ev)
{
    ODMemContainerData *somThis = ODMemContainerGetData(somSelf);
    ODMemContainerMethodDebug("ODMemContainer","GetHandlers");

	return _fHandlers;
}
