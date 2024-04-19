/* @(#)Z 1.14 com/src/bento/UpdatCtr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:39 (96/10/29 09:14:07) */
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
	File:		UpdatCtr.cpp

	Contains:	Implementation of ODMemContainer class.

	Owned by:	Vincent Lo

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 10/8/95	TJ		Fixes Recomended by Refball
		 <5>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <4>	 7/21/95	VL		1270320: Create PlatformFile on stack.
		 <3>	 5/26/95	VL		1251403: Multithreading naming support.
		 <2>	  3/9/95	VL		1225504: Fixed Document ref-counting
									problem.
		 <1>	 2/10/95	VL		first checked in
To Do:
*/

#define ODUpdateContainer_Class_Source

#define VARIABLE_MACROS

#include <UpdatCtr.xih>

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_CMDocument_xh
#include <CMDoc.xh>
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
#include <Except.h>
#endif

#ifndef _ODNEW_
#include <ODNew.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif
 
#ifndef _BENTOERR_
#include "BentoErr.h"
#endif

#pragma segment ODUpdateContainer

//==============================================================================
// ODUpdateContainer
//==============================================================================

//------------------------------------------------------------------------------
// ODUpdateContainer: ~ODUpdateContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUpdateContainersomUninit(ODUpdateContainer *somSelf)
{
    ODUpdateContainerData *somThis = ODUpdateContainerGetData(somSelf);
    ODUpdateContainerMethodDebug("ODUpdateContainer","somUninit");
	
          Environment* ev = somGetGlobalEnvironment();
    try
    {
	ODSafeReleaseObject(_fTargetDocument);
	ODSafeReleaseObject(_fTargetContainer);

	ODDisposeHandle(_fHandle);
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// ODUpdateContainer: InitContainer
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODUpdateContainerInitContainer(ODUpdateContainer *somSelf, Environment *ev,
		ODStorageSystem* storage, ODContainerID* id)
{
    ODUpdateContainerData *somThis = ODUpdateContainerGetData(somSelf);
    ODUpdateContainerMethodDebug("ODUpdateContainer","InitContainer");
	
	ODBoolean		fileLocked = kODTrue;
	_fTargetContainer = kODNULL;
	_fTargetDocument = kODNULL;
	_fHandle = kODNULL;
	PlatformFile	file;
	ODByteArray* ba;
    try
    {

	/* Moved from somInit. SOM itself sets fields to zero
		_fTargetContainer = kODNULL;
		_fTargetDocument = kODNULL;
		_fHandle = kODNULL;
	*/
	
	
	ODVolatile(fileLocked);

#ifdef _PLATFORM_MACINTOSH_
		file.Specify((FSSpec *) id->_buffer);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
		file.Specify((ODFileSpec *) id->_buffer);
#endif
		fileLocked = file.IsLocked();
		if (fileLocked == kODFalse)
			file.Lock();
	
		_fTargetContainer = (ODBentoContainer*) GetFileContainer(ev,
						storage->GetSession(ev), 	
						(FSSpec*) id->_buffer);
		_fTargetDocument = (CMDocument*) _fTargetContainer->AcquireDocument(ev, kODDefaultDocument);
		
			

	_fHandle = ODNewHandle(0);
	
	ba = CreateByteArray(&_fHandle, sizeof(ODHandle));	
	parent_InitContainer(somSelf, ev, storage, ba);
	DisposeByteArray(ba);
    }
    catch (ODException _exception)
    {
        ODError error = ErrorCode();
	SaveAndRestoreEv2(_fTargetDocument, _fTargetContainer);
	ODDisposeHandle(_fHandle);
	DisposeByteArray(ba);
        ODSetSOMException(ev, error);
    }
	if (fileLocked == kODFalse)
		file.Unlock();

}

//------------------------------------------------------------------------------
// ODUpdateContainer: GetTargetFileContainer
//------------------------------------------------------------------------------

SOM_Scope ODBentoContainer*  SOMLINK ODUpdateContainerGetTargetContainer(ODUpdateContainer *somSelf, Environment *ev)
{
	ODUpdateContainerData *somThis = ODUpdateContainerGetData(somSelf);
    ODUpdateContainerMethodDebug("ODUpdateContainer","GetTargetContainer");

	return _fTargetContainer;
}

//------------------------------------------------------------------------------
// ODUpdateContainer: GetTargetDocument
//------------------------------------------------------------------------------

SOM_Scope CMDocument*  SOMLINK ODUpdateContainerGetTargetDocument(ODUpdateContainer *somSelf, Environment *ev)
{
	ODUpdateContainerData *somThis = ODUpdateContainerGetData(somSelf);
    ODUpdateContainerMethodDebug("ODUpdateContainer","GetTargetDocument");

	return _fTargetDocument;
}
