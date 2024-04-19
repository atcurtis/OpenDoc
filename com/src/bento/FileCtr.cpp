/* @(#)Z 1.15 com/src/bento/FileCtr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:33 (96/10/29 09:12:41) */
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
	File:		FileCtr.cpp

	Contains:	Implementation of ODFileContainer class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<23>	10/24/95	jpa		1293441: DM/VL: Bento memory reserve &
									fatal container err.
		<22>	 10/8/95	TJ		Fixes Recomended by Refball
		<21>	 8/24/95	EL		1276818: move Bento container suite
									property to BentoDef.h
		<20>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		<19>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<18>	 7/21/95	VL		1270320: Create ba on stack in GetID.
		<17>	 5/26/95	VL		1251403: Multithreading naming support.
		<16>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<15>	 4/25/95	VL		1240737: Make sure that the returned ID
									from GetID is set to the right _length.
		<14>	 4/15/95	VL		1240014: Modifiy id->_length in
									InitContainer to reflect the real length.
		<13>	  4/7/95	EL		1225905: Do SetModDate here rather than in
									handler.
		<12>	 3/24/95	EL		1209355: open container r/w instead of
									update because CMTakeSnapShot does not work
									with update yet.
		<11>	 3/23/95	VL		1230357: Implemented Purge.
		<10>	  3/6/95	EL		1225905: Call RestoreModDate if file
									container to be aborted.
		 <9>	 2/21/95	EL		1182275: Garbage collect embedded values
									with zero length.
		 <8>	 2/10/95	VL		1205627: Check to see if a file is locked.
									If so, open it read-only.
		 <7>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <6>	 11/1/94	VL		1185688: Added SOM_CATCH.
		 <5>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets. 1184166: Implemented GetName and
									SetName.
		 <4>	 8/26/94	EL		#1182275 Include code (not yet active) for
									merging of container.
		 <3>	  8/5/94	VL		1171603: Use somSelf->GetCMSession instead
									of depending on ODStorageSystem.
		 <2>	  8/3/94	VL		1153123: Storage to ODStor.
		 <1>	 6/30/94	CC		first checked in
		 <0>	 6/28/94	SV		SOMverted
		 <2>	 4/13/94	VL		1157028: StorageM.h does not include
									CMAPI.h anymore, so GetCMSession returns
									ODULong.
		 <1>	 3/24/94	VL		first checked in
		<15>	  2/8/94	VL		Use new exception macros.
		<14>	 1/21/94	CG		Added include for BentoCtr.h.
		<13>	 1/19/94	EL		Use kCMNextGeneration when opening new
									container for update.
		<12>	 1/12/94	VL		Init changes.
		<11>	  1/6/94	EL		Reuse free space in the file.
		<10>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		 <9>	11/23/93	VL		Made FileContainer pool-aware.
		 <8>	  9/9/93	VL		Moved Session Global code to ODDraft.
		 <7>	  8/9/93	VL		Moved error code to ODError.h.
		 <6>	  8/6/93	VL		fContainer is now fCMContainer and is
									protected.
		 <5>	 6/30/93	VL		Typecasted ODType to CMGlobalName for CM
									calls.
		 <4>	 6/22/93	VL		Used RefCount from ODRefCntObject.
		 <3>	 6/15/93	VL		This class is now a subclass of
									ODBentoContainer. Stripped some code and
									put it in ODBentoContainer.
		 <2>	  6/1/93	VL		Delete fID (PlatformFile object) in the
									destructor. Open should SetMetaHandlers to
									target and use CMOpenNewContainer instead
									of CMOpenContainer.
		 <1>	 5/27/93	VL		first checked in

	To Do:
		1) Get rid of fContainer (public by convention). Maybe we can make it
			private and use friends to access it.
	In Progress:
		
*/

#define ODFileContainer_Class_Source

#define VARIABLE_MACROS

#include <FileCtr.xih>

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef _FSHDR_
#include "FSHdr.h"
#endif

#ifndef _TARGTHDR_
#include "TargtHdr.h"
#endif

#ifndef _INDHDR_
#include "IndHdr.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_Module_OpenDoc_Errors_defined
#include "ErrorDef.xh"
#endif

#ifndef _ODNEW_
#include <ODNew.h>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifdef _PLATFORM_MACINTOSH
#ifndef __ALIASES__
#include <Aliases.h>
#endif
#endif

#ifdef DebugRefCount
#include "Stdio.h"
#endif

#if ODDebug
// #define ODDebug_ODFileContainer 1
#endif

#pragma segment ODFileContainer

ODBoolean IsReadOnlyFile(PlatformFile *platformFile)
{
        ODFileAttrFlags fileAttrs;

        // Get file attributes and return true if file is a readonly file
        platformFile->GetFileAttributes(&fileAttrs);
        return (fileAttrs & kODFileAttrReadOnly);
}


//==============================================================================
// ODFileContainer
//==============================================================================

//------------------------------------------------------------------------------
// ODFileContainer: ~ODFileContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFileContainersomUninit(ODFileContainer *somSelf)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","somUninit");

#ifdef DebugRefCount
	printf("~ODFileContainer %x CMContainer %x RefCount %d\n", somSelf, _fCMContainer, _fRefCount);
	fflush(stdout);
#endif

		Environment *ev = somGetGlobalEnvironment ();
	try
        {
	
		if (_fCMContainer != kODNULL)
			somSelf->Close(ev);
	
		if (_fHandlers != kODNULL)
			delete _fHandlers;
	
		if (_fPlatformFile != kODNULL)
			delete _fPlatformFile;
			
#ifdef _PLATFORM_MACINTOSH_
		if (_fAlias != kODNULL)
			ODDisposeHandle((ODHandle) _fAlias);
#endif
	}
        catch (ODException _exception)
        {
		// Ignore exceptions
              SetErrorCode(kODNoError);
	}
}

//------------------------------------------------------------------------------
// ODFileContainer: InitContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFileContainerInitContainer(ODFileContainer *somSelf, Environment *ev,
		ODStorageSystem* storage, ODContainerID* id)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","InitContainer");
	
	_fPlatformFile = kODNULL;
	_fHandlers = kODNULL;
     try
     {

	/* Moved from somInit. SOM itself sets fields to zero
	_fCMContainer = kODNULL;
	_fHandlers = kODNULL;
#ifdef _PLATFORM_MACINTOSH_
	_fAlias = kODNULL;
#endif
	_fPlatformFile = kODNULL;
	*/
	
#ifdef _PLATFORM_MACINTOSH_
	OSErr err = NewAlias(kODNULL, (FSSpec*) id->_buffer, &_fAlias);
	if (err != noErr)
		THROW(kODErrCannotCreateContainer);
#endif

	// Change id to exclude any garbage before FSSpec.name.
	// struct FSSpec {
	// 	short							vRefNum;
	// 	long							parID;
	// 	Str63							name;
	// };
#ifdef _PLATFORM_MACINTOSH_
	id->_length = sizeof(short) + sizeof(long) + ((FSSpec*) id->_buffer)->name[0] + 1;
#endif
	
	// Call parent with the updated id
	parent_InitContainer(somSelf, ev, storage, id);
	
	_fPlatformFile = new PlatformFile();
	_fPlatformFile->Specify((ODFileSpec*) id->_buffer);

        if (IsReadOnlyFile(_fPlatformFile))
           _fPlatformFile->SetPermission(kODFileShareRead);

		
#ifdef _PLATFORM_MACINTOSH_
	_fHandlers = new(somSelf->GetHeap(ev)) ODFSBentoHandlers(somSelf->GetCMSession(ev),
															_fPlatformFile);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	_fHandlers = new ODFSBentoHandlers(somSelf->GetCMSession(ev),
						_fPlatformFile);
#endif
	_fHandlers->Initialize();

    }
    catch (ODException _exception)
    {
	if (_fHandlers != kODNULL)
		delete _fHandlers;
	
	if (_fPlatformFile != kODNULL)
		delete _fPlatformFile;
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// ODFileContainer: GetID
//------------------------------------------------------------------------------

SOM_Scope ODContainerID  SOMLINK ODFileContainerGetID(ODFileContainer *somSelf, Environment *ev)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","GetID");
	
	ODByteArray ba;

     try
     {
	
	ODBoolean	dummyWasChanged;
	ODFileSpec*	fsSpec = (ODFileSpec*) ODNewPtrClear(sizeof(ODFileSpec));
	
#ifdef _PLATFORM_MACINTOSH_
	OSErr err = ResolveAlias(kODNULL, _fAlias, fsSpec, &dummyWasChanged);
	if (err != noErr) {
		*fsSpec = _fPlatformFile->GetFileSpec();
	}
#else
	*fsSpec = _fPlatformFile->GetFileSpec();
	
#endif
	ba._buffer = (octet*) fsSpec;
	ba._maximum = sizeof(ODFileSpec);
#ifdef _PLATFORM_MACINTOSH_
	ba._length = sizeof(short) + sizeof(long) + ((FSSpec*) ba._buffer)->name[0] + 1;
#else
        ba._length = strlen((char *) (&(fsSpec->name[0]))) + 1;
#endif
	
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
	ba._buffer = kODNULL;
	ba._maximum = ba._length = 0;
    }
	
	return ba;
}

//------------------------------------------------------------------------------
// ODFileContainer: GetName
//------------------------------------------------------------------------------

SOM_Scope ODContainerName  SOMLINK ODFileContainerGetName(ODFileContainer *somSelf, Environment *ev)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","GetName");

	ODFileSpec			fsSpec;
	ODContainerName*	name = kODNULL;
	ODBoolean			dummyWasChanged;
	
    try
    {
	
#ifdef _PLATFORM_MACINTOSH_
	OSErr err = ResolveAlias(kODNULL, _fAlias, &fsSpec, &dummyWasChanged);
	if (err != noErr) {
		name = _fPlatformFile->GetName();
	}
	else {
		name = CreateIText(0, 0, (StringPtr) &fsSpec.name);
	}
#else
	fsSpec = _fPlatformFile->GetFileSpec();
	name = CreateITextFromCharPtr((char *)&fsSpec.name);
#endif
	
    }
    catch (ODException _exception)
    {
         name = kODNULL;
         ODSetSOMException(ev, _exception);
    }
	
	return *name;	
}

//------------------------------------------------------------------------------
// ODFileContainer: SetName
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFileContainerSetName(ODFileContainer *somSelf, Environment *ev,
		ODContainerName* name)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","SetName");

   try
   {
	
        StringPtr fileName = GetPStringFromIText(name);
       _fPlatformFile->Rename(fileName);

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }

}

//------------------------------------------------------------------------------
// ODFileContainer: Create
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODFileContainerCreate(ODFileContainer *somSelf, Environment *ev)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","Create");

   try
   {

	if (_fCMContainer == kODNULL) {
	
    	somSelf->SetModDate(ev, _fPlatformFile->GetFileModDate());

		CMSession cmSession = somSelf->GetCMSession(ev);

		CMSetMetaHandler(cmSession,
						(CMGlobalName)CMTargetHandlersTypeName,
						targetContainerMetahandler);

		_fCMContainer = CMOpenNewContainer(cmSession,
										_fHandlers,
										(CMGlobalName) kODBentoFileTypeName,
										(CMContainerUseMode) (kCMWriting),
										1, kCMDefaultEndian);
		if (_fCMContainer == kODNULL)
			THROW(kODErrCannotCreateContainer);
	}


   }
   catch (ODException _exception)
   {
        _fCMContainer = kODNULL;
        ODSetSOMException(ev, _exception);
   }
	return somSelf;
}

//------------------------------------------------------------------------------
// ODFileContainer: Open
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODFileContainerOpen(ODFileContainer *somSelf, Environment *ev)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","Open");

    try
    {

	if (_fCMContainer == kODNULL) {
	
    	somSelf->SetModDate(ev, _fPlatformFile->GetFileModDate());

		CMSession cmSession = somSelf->GetCMSession(ev);
		
		CMSetMetaHandler(cmSession,
				(CMGlobalName)CMTargetHandlersTypeName,
				targetContainerMetahandler);
						
                if ((_fPlatformFile->GetPermission() == kODFileRead) || 
                    (_fPlatformFile->GetPermission() == kODFileShareRead))
                        _fCMContainer = CMOpenContainer(cmSession,
                                        _fHandlers,
                                        (CMGlobalName) kODBentoFileTypeName,
                                        (CMContainerUseMode) kCMReading);
                else

		_fCMContainer = CMOpenContainer(cmSession,
				_fHandlers,
				(CMGlobalName) kODBentoFileTypeName,
				(CMContainerUseMode) kCMReuseFreeSpace);

		if (_fCMContainer == kODNULL)
			THROW(kODErrCannotOpenContainer);
	}


    }
    catch (ODException _exception)
    {  
       _fCMContainer = kODNULL;
       ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

//------------------------------------------------------------------------------
// ODFileContainer: Close
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK ODFileContainerClose(ODFileContainer *somSelf, Environment *ev)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","Close");

    try
    {

	if (_fCMContainer != kODNULL) {
		try
                {
			if (somSelf->GetDirtyFlag(ev) != kODFalse) {
				CMObject	currObj, nextObj;
				CMProperty	embedProp;
				CMType		embedType;
				CMValue		currValue;
				CMSize		valueSize;
	
				embedProp = CMRegisterProperty(_fCMContainer, kODPropVersionList);
				currObj = CMGetNextObjectWithProperty(_fCMContainer, kODNULL, embedProp);
				CMKeepObject(currObj);
				embedProp = CMRegisterProperty(_fCMContainer, kODEmbeddedContainerProperty); // "OpenDoc:EmbeddedContainer"
				embedType = CMRegisterType(_fCMContainer, kODEmbeddedContainerType); // "OpenDoc:EmbeddedContainerType"
				currObj = CMGetNextObjectWithProperty(_fCMContainer, kODNULL, embedProp);
				while (currObj) {
					nextObj = CMGetNextObjectWithProperty(_fCMContainer, currObj, embedProp);
					currValue = CMUseValue(currObj, embedProp, embedType);
					if (currValue) {
						valueSize =  CMGetValueSize(currValue);
						CMReleaseValue(currValue);
						if (valueSize)	/* no need to keep it unless length is not zero */
							CMKeepObject(currObj);
					} 
					currObj = nextObj;
				};
				CMCloseContainer(_fCMContainer);
#if ODDebug_ODFileContainer
somPrintf("FileContainer Close: Close\n");
#endif
			}
			else {
				CMAbortContainer(_fCMContainer);
#if ODDebug_ODFileContainer
somPrintf("FileContainer Close: Abort\n");
#endif
			}
                        if ((_fPlatformFile->GetPermission() != kODFileRead) && 
                            (_fPlatformFile->GetPermission() != kODFileShareRead))
		        	_fPlatformFile->SetFileModDate(somSelf->GetModDate(ev));
                }
		catch(ODException _exception)
                {
			if (ErrorCode() == kODErrBentoErr)
				SetErrorCode(kODErrFatalContainerError);
                        ODSetSOMException(ev, _exception);
		}
		_fCMContainer = kODNULL;
	}
	

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
	return ODFileContainer_parent_ODBentoContainer_Close(somSelf, ev);
}

//------------------------------------------------------------------------------
// ODFileContainer: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODFileContainerPurge(ODFileContainer *somSelf, Environment *ev,
		ODSize size)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","Purge");

        ODSize odsize = 0;
	
    try
    {
	odsize = parent_Purge(somSelf, ev, size);

    }
    catch (ODException _exception)
    {
        odsize = 0;
        ODSetSOMException(ev, _exception);
    }
        return odsize;
}

//------------------------------------------------------------------------------
// ODFileContainer: GetCMContainer
//------------------------------------------------------------------------------

SOM_Scope CMContainer  SOMLINK ODFileContainerGetCMContainer(ODFileContainer *somSelf, Environment *ev)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","GetCMContainer");

	return _fCMContainer;
}

//------------------------------------------------------------------------------
// ODFileContainer: GetHandlers
//------------------------------------------------------------------------------

SOM_Scope ODBentoHandlers*  SOMLINK ODFileContainerGetHandlers(ODFileContainer *somSelf, Environment *ev)
{
    ODFileContainerData *somThis = ODFileContainerGetData(somSelf);
    ODFileContainerMethodDebug("ODFileContainer","GetHandlers");

	return _fHandlers;
}

