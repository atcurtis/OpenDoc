/* @(#)Z 1.25 com/src/core/ODSessnB.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:53 (96/10/29 09:21:09) */

//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odcore
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
	File:		ODSessnB.cpp

	Contains:	Base Session class implementation

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<19>	 10/8/95	TJ		Fixes Recomended by Refball
		<18>	 9/27/95	T‚		1287399 GM: Cyberdog needs chance to
									cleanup before ODSession dies
		<17>	 8/26/95	T‚		1274606 FB2: Patching Remarks
		<16>	 8/16/95	NP		1273600: FB: ShellSI is not ref-counted
									correctly.
		<15>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<14>	 7/31/95	CG		#1270320: Delete _fInfo in somUnint.
		<13>	 5/26/95	DM		1251403: Multithreading naming support.
		<12>	 5/21/95	NP		1250142: AcquireShellSemtInterface should do
									ref counting.
		<11>	 5/18/95	DM		#1250474: RemoveEntry(in ODType type)
									inout->in
		<10>	 5/11/95	NP		1242421: Fix exception handling, 1248917:
									Remove unused method.
		 <9>	  1/9/95	NP		1196615: ODSession::Get/SetShellSemtIntf
									changes.
		 <8>	11/15/94	NP		1200193-The Shell should instantiate its
									Semantic Interface.
		 <7>	 10/7/94	CC		1191489 - Moved UniqueUpdateID() to
									ODSession.
		 <6>	 8/22/94	T‚		#1182448 System Process quits when no
									OpenDoc documents are running
		 <5>	 8/19/94	NP		1181622: Ownership fix.
		 <4>	 8/17/94	jpa		Added tons of includes so somUninit will
									delete the objects properly. [1181515]
		 <3>	 8/13/94	NP		1180819-Added support for private
									ScritingObject.
		 <2>	 8/12/94	NP		1180762-added setters for getters. moved
									stuff to non-B file. Removed
									GetSemtInterface.
		 <1>	 6/23/94	CG		first checked in
		------------------------------------------------------------
		 <3>	  6/1/94	CG		Removed include for Binding.h
		 <2>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <1>	  2/3/94	CG		first checked in
	
	To Do:
	In Progress:
		- Not including SOM class headers so the 'delete' calls were
not properly deleting the objects.
*/

#ifndef _ALTPOINT_
#include "AltPoint.h"                   // Use C++ savvy ODPoint and ODRect
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _DOCUTILS_
#include <DocUtils.h>
#endif

#ifndef _TOKNTABL_
#include "ToknTabl.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef SOM_ODSemanticInterface_xh
#include "SemtIntB.xh"
#endif
#endif

// We have to include the proper headers for the classes of all the
// instance variables, so the proper destructors get called when they're deleted.
#include <WinStat.xh>
#include <Disptch.xh>
#include <Arbitrat.xh>
#include <Clipbd.xh>
#include <DragDrp.xh>
#include <LinkMgr.xh>
#ifdef _PLATFORM_MACINTOSH_  // do not include OSA classes for WIN32, OS2, AIX
#include <MssgIntf.xh>
#include <NamRslvr.xh>
#endif // _PLATFORM_MACINTOSH_
#include <Translt.xh>
#include <Undo.xh>
#ifdef 		_TOOLSPACE_
#include <ODTlSpc.xh>
#endif 		// _TOOLSPACE_
#include "ToknTabl.h"
#include <ODBindng.xh>
#include <NmSpcMg.xh>
#include <ODStor.xh>
#include <Info.xh>

#define ODBaseSession_Class_Source
#define VARIABLE_MACROS
#include <ODSessnB.xih>

#pragma segment ODBaseSession

//==============================================================================
// ODBaseSession
//==============================================================================

//------------------------------------------------------------------------------
// ODBaseSession: InitBaseSession
//------------------------------------------------------------------------------

SOM_Scope void	 SOMLINK ODBaseSessionInitBaseSession
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","InitBaseSession");

	try {
	
		/* Moved from somInit. SOM itself sets fields to zero
		_fWindowState = kODNULL;
		_fDispatcher = kODNULL;
		_fArbitrator = kODNULL;
		_fClipboard = kODNULL;
		_fDragAndDrop = kODNULL;
		_fLinkManager = kODNULL;
		_fMessageInterface = kODNULL;
		_fNameResolver = kODNULL;
		_fTranslation = kODNULL;
		_fUndo = kODNULL;
		_fBinding = kODNULL;
		_fNameSpaceManager = kODNULL;
		_fStorage = kODNULL;
		_fShellSemtInterface = kODNULL;
		_fInfo = kODNULL;
	
		_fTokenTable = kODNULL;
		_fScriptingObject = kODNULL;
		*/
		somSelf->InitObject(ev);
		
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

#if 0
//------------------------------------------------------------------------------
// ODBaseSession: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODBaseSessionPurge(ODBaseSession *somSelf, Environment *ev,
		ODSize size)
{
    //ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","Purge");

	ODSize	retVal = 0;

	SOM_TRY
		retVal = ODBaseSession_parent_ODObject_Purge(somSelf, ev, size);
	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return retVal;
}
#endif /* 0 */

//------------------------------------------------------------------------------
// ODBaseSession: somUninit
//------------------------------------------------------------------------------

static void	DispatchExitEvent(Environment* ev, 
#ifdef _PLATFORM_UNIX_
                                  ODBaseSession *somSelf, 
#endif
                                  ODDispatcher* dispatcher)
{
// Copied from RealShell::FakePrintMenuEvent
	ODEventData event;

#ifdef _PLATFORM_MACINTOSH_
	event.message = 0L;
	event.what = kODEvtExit;
	// zero the rest of the fields
	WASSERT( sizeof(Point) == sizeof(long) );
	*(long*)&event.where = 0L;
	event.when = 0L;
	event.modifiers = 0;
#endif

#ifdef _PLATFORM_WIN32_
	event.message = kODEvtExit;
#endif
#ifdef _PLATFORM_OS2_
        event.msg = kODEvtExit;
#endif
#ifdef _PLATFORM_UNIX_
        event.type = kODEvtExit;
        event.display = somSelf->GetWindowSystemData(ev)->display;
#endif

	if (dispatcher) dispatcher->Dispatch(ev, &event);
}


SOM_Scope void  SOMLINK ODBaseSessionsomUninit(ODBaseSession *somSelf)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","somUninit");
	Environment *ev = somGetGlobalEnvironment();
	
	TRY
		DispatchExitEvent(ev, 
#ifdef _PLATFORM_UNIX_
                                  somSelf, 
#endif
                                  _fDispatcher);
	//	close outstanding documents appropriately
		ODDocument* document = kODNULL;
		while ((document = ODGetFirstOpenDocument(ev, (ODSession*)somSelf)) 
					!= kODNULL)
			 ODCloseDocument(ev, (ODSession*)somSelf, document);
	CATCH_ALL
		WARNMSG(WARN_INDEX(AMSG_750),
			"Exit event or closing outstanding documents failed.");
	ENDTRY

	ODDeleteObject(_fWindowState);
	ODDeleteObject(_fDispatcher);
	ODDeleteObject(_fArbitrator);
	ODDeleteObject(_fClipboard);
	ODDeleteObject(_fLinkManager);
	ODDeleteObject(_fDragAndDrop);
#ifdef _PLATFORM_MACINTOSH_  // do not include OSA classes for WIN32, OS2, AIX
	ODDeleteObject(_fMessageInterface);
	ODDeleteObject(_fNameResolver);
#endif
	ODDeleteObject(_fTranslation);
	ODDeleteObject(_fUndo);
#ifdef _TOOLSPACE_ 
	ODDeleteObject(_fToolSpaceManager);
#endif
	ODDeleteObject(_fTokenTable);
	ODDeleteObject(_fBinding);
	ODDeleteObject(_fNameSpaceManager);
	ODDeleteObject(_fStorage);
	ODDeleteObject(_fInfo);
	
#ifdef _PLATFORM_MACINTOSH_  // do not include OSA classes for WIN32, OS2, AIX
    ODSafeReleaseObject(_fShellSemtInterface); _fShellSemtInterface = kODNULL;
#endif
}

//------------------------------------------------------------------------------
// "Getters" / "Setters"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ODBaseSession: GetWindowState
//------------------------------------------------------------------------------

SOM_Scope ODWindowState*  SOMLINK ODBaseSessionGetWindowState
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetWindowState");

	return _fWindowState;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetWindowState
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetWindowState
		(ODBaseSession *somSelf, Environment *ev,
		ODWindowState* windowState)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetWindowState");

	_fWindowState = windowState;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetDispatcher
//------------------------------------------------------------------------------

SOM_Scope ODDispatcher*  SOMLINK ODBaseSessionGetDispatcher
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetDispatcher");

	return _fDispatcher;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetDispatcher
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetDispatcher
		(ODBaseSession *somSelf, Environment *ev,
		ODDispatcher* dispatcher)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetDispatcher");

	_fDispatcher = dispatcher;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetArbitrator
//------------------------------------------------------------------------------

SOM_Scope ODArbitrator*  SOMLINK ODBaseSessionGetArbitrator
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetArbitrator");

	return _fArbitrator;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetArbitrator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetArbitrator
		(ODBaseSession *somSelf, Environment *ev,
		ODArbitrator* arbitrator)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetArbitrator");

	_fArbitrator = arbitrator;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetClipboard
//------------------------------------------------------------------------------

SOM_Scope ODClipboard*  SOMLINK ODBaseSessionGetClipboard
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetClipboard");

	return _fClipboard;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetClipboard
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetClipboard
		(ODBaseSession *somSelf, Environment *ev,
		ODClipboard* clipboard)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetClipboard");

	_fClipboard = clipboard;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetDragAndDrop
//------------------------------------------------------------------------------

SOM_Scope ODDragAndDrop*	 SOMLINK ODBaseSessionGetDragAndDrop
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetDragAndDrop");

	return _fDragAndDrop;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetDragAndDrop
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetDragAndDrop
		(ODBaseSession *somSelf, Environment *ev,
		ODDragAndDrop* dragAndDrop)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetDragAndDrop");

	_fDragAndDrop = dragAndDrop;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetInfo
//------------------------------------------------------------------------------

SOM_Scope ODInfo*	 SOMLINK ODBaseSessionGetInfo
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetInfo");

	return _fInfo;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetInfo
//------------------------------------------------------------------------------

SOM_Scope void	 SOMLINK ODBaseSessionSetInfo
		(ODBaseSession *somSelf, Environment *ev, ODInfo* info)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","SetInfo");

	_fInfo = info;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetLinkManager
//------------------------------------------------------------------------------

SOM_Scope ODLinkManager*	 SOMLINK ODBaseSessionGetLinkManager
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetLinkManager");

	return _fLinkManager;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetLinkManager
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetLinkManager
		(ODBaseSession *somSelf, Environment *ev,
		ODLinkManager* linkManager)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetLinkManager");

	_fLinkManager = linkManager;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetMessageInterface
//------------------------------------------------------------------------------

SOM_Scope ODMessageInterface*  SOMLINK ODBaseSessionGetMessageInterface
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetMessageInterface");

	return _fMessageInterface;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetMessageInterface
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetMessageInterface
		(ODBaseSession *somSelf, Environment *ev,
		ODMessageInterface* messageInterface)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetMessageInterface");

	_fMessageInterface = messageInterface;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetNameResolver
//------------------------------------------------------------------------------

SOM_Scope ODNameResolver*  SOMLINK ODBaseSessionGetNameResolver
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetNameResolver");

	return _fNameResolver;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetNameResolver
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetNameResolver
		(ODBaseSession *somSelf, Environment *ev,
		ODNameResolver* nameResolver)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetNameResolver");

	_fNameResolver = nameResolver;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetTranslation
//------------------------------------------------------------------------------

SOM_Scope ODTranslation*  SOMLINK ODBaseSessionGetTranslation
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetTranslation");

	return _fTranslation;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetTranslation
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetTranslation
		(ODBaseSession *somSelf, Environment *ev,
		ODTranslation* translation)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetTranslation");

	_fTranslation = translation;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetUndo
//------------------------------------------------------------------------------

SOM_Scope ODUndo*  SOMLINK ODBaseSessionGetUndo
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetUndo");

	return _fUndo;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetUndo
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetUndo
		(ODBaseSession *somSelf, Environment *ev,
		ODUndo* undo)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetUndo");

	_fUndo = undo;
}

#ifdef _TOOLSPACE_ 
// ToolSpaceManager only implemented on Win32 and OS/2
//------------------------------------------------------------------------------
// ODBaseSession: GetToolSpaceManager
//------------------------------------------------------------------------------

SOM_Scope ODToolSpaceManager*  SOMLINK ODBaseSessionGetToolSpaceManager
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetToolSpaceManager");

	return _fToolSpaceManager;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetToolSpaceManager
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetToolSpaceManager
		(ODBaseSession *somSelf, Environment *ev,
		ODToolSpaceManager* toolSpaceManager)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetToolSpaceManager");

	_fToolSpaceManager = toolSpaceManager;
}
#endif  // _TOOLSPACE_ 

//------------------------------------------------------------------------------
// ODBaseSession: GetStorageSystem
//------------------------------------------------------------------------------

SOM_Scope ODStorageSystem*  SOMLINK ODBaseSessionGetStorageSystem
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetStorageSystem");

	return _fStorage;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetStorageSystem
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetStorageSystem
		(ODBaseSession *somSelf, Environment *ev,
		ODStorageSystem* storageSystem)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetStorageSystem");

	_fStorage = storageSystem;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetNameSpaceManager
//------------------------------------------------------------------------------

SOM_Scope ODNameSpaceManager*  SOMLINK ODBaseSessionGetNameSpaceManager
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetNameSpaceManager");

	return _fNameSpaceManager;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetNameSpaceManager
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetNameSpaceManager
		(ODBaseSession *somSelf, Environment *ev,
		ODNameSpaceManager* nameSpaceManager)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetNameSpaceManager");

	_fNameSpaceManager = nameSpaceManager;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetBinding
//------------------------------------------------------------------------------

SOM_Scope ODBinding*  SOMLINK ODBaseSessionGetBinding
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetBinding");

	return _fBinding;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetBinding
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetBinding
		(ODBaseSession *somSelf, Environment *ev,
		ODBinding* binding)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetBinding");

	_fBinding = binding;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetRegistry
//------------------------------------------------------------------------------

SOM_Scope ODPartHandlerRegistryInt*  SOMLINK ODBaseSessionGetRegistry
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetRegistry");

	return _fODRegistry;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetRegistry
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetRegistry
		(ODBaseSession *somSelf, Environment *ev,
		ODPartHandlerRegistryInt* registry)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetRegistry");

	_fODRegistry = registry;
}


//------------------------------------------------------------------------------
// ODBaseSession: AcquireShellSemtInterface
//------------------------------------------------------------------------------

SOM_Scope ODSemanticInterface*  SOMLINK ODBaseSessionAcquireShellSemtInterface
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","AcquireShellSemtInterface");

#ifdef _PLATFORM_MACINTOSH_  // do not include OSA classes for WIN32, OS2, AIX
	if (_fShellSemtInterface)
		_fShellSemtInterface->Acquire(ev);
#endif
	return _fShellSemtInterface;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetShellSemtInterface
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetShellSemtInterface
		(ODBaseSession *somSelf, Environment *ev,
		ODSemanticInterface* shellSemanticInterface)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetShellSemtInterface");

#ifdef _PLATFORM_MACINTOSH_  // do not include OSA classes for WIN32, OS2, AIX
	SOM_TRY
		ODReleaseObject(ev, _fShellSemtInterface);
		if (shellSemanticInterface)
			shellSemanticInterface->Acquire(ev);
		_fShellSemtInterface = shellSemanticInterface;
	SOM_CATCH_ALL
	SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseSession: Tokenize
//------------------------------------------------------------------------------

SOM_Scope ODTypeToken  SOMLINK ODBaseSessionTokenize
		(ODBaseSession *somSelf, Environment *ev,
		ODType type)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","Tokenize");

	ODTypeToken	retVal = kODNullTypeToken;

	try {
		retVal = _fTokenTable->Tokenize(type);
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	
	return retVal;
}

//------------------------------------------------------------------------------
// ODBaseSession: RemoveEntry
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionRemoveEntry
		(ODBaseSession *somSelf, Environment *ev,
		ODType type)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","RemoveEntry");

	try {
		_fTokenTable->RemoveEntry(type);
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODBaseSession: GetType
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseSessionGetType
		(ODBaseSession *somSelf, Environment *ev,
		ODTypeToken token, ODType* type)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","GetType");

	ODBoolean	retVal = kODFalse;

	try {
		retVal = _fTokenTable->GetType(token, type);
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	
	return retVal;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetTokenTable
//------------------------------------------------------------------------------

SOM_Scope TokenTable*  SOMLINK ODBaseSessionGetTokenTable
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionGetTokenTable");

    return _fTokenTable;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetTokenTable
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetTokenTable
		(ODBaseSession *somSelf, Environment *ev,
		TokenTable* table)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionSetTokenTable");

	_fTokenTable = table;
}

//------------------------------------------------------------------------------
// ODBaseSession: GetWindowSystemData
//------------------------------------------------------------------------------

SOM_Scope WindowSystemData*  SOMLINK ODBaseSessionGetWindowSystemData(
					ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession",
					"ODBaseSessionGetWindowSystemData");

    return &_fWindowSystemData;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetWindowSystemData
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetWindowSystemData
		(ODBaseSession *somSelf,
				Environment *ev,
				WindowSystemData* windowSystemData)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession",
					"ODBaseSessionSetWindowSystemData");

    memcpy(&_fWindowSystemData, windowSystemData, 
					sizeof(WindowSystemData));
}

#ifdef _PLATFORM_WIN32_
//------------------------------------------------------------------------------
// ODBaseSession: GetOleManager
//------------------------------------------------------------------------------

SOM_Scope ODOLEManager*  SOMLINK ODBaseSessionGetOleManager(
					ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession",
					"ODBaseSessionGetOleManager");

    return _fOLEManager;
}

//------------------------------------------------------------------------------
// ODBaseSession: SetOleManager
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseSessionSetOleManager( ODBaseSession *somSelf, 
					Environment *ev,
					ODOLEManager *OLEManager)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession",
					"ODBaseSessionSetOleManager");

    _fOLEManager = OLEManager;
}
#endif

#if 0
//------------------------------------------------------------------------------
// ODBaseSession: GetScriptingbject
//------------------------------------------------------------------------------

SOM_Scope ScriptingObject*  SOMLINK ODBaseSessionGetScriptingObject
		(ODBaseSession *somSelf, Environment *ev)
{
    ODBaseSessionData *somThis = ODBaseSessionGetData(somSelf);
    ODBaseSessionMethodDebug("ODBaseSession","ODBaseSessionGetScriptingObject");

//	if (!_fScriptingObject)
//		_fScriptingObject = new ScriptingObject;
//	return _fScriptingObject;
	return 0;
}
#endif /* 0 */
