/* @(#)Z 1.13 com/src/utils/StorUtil.cpp, odstorage, od96os2, odos29646d 96/11/15 15:28:55 (96/09/25 14:06:11) */
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
	File:		StorUtil.cpp

	Contains:	Utilities for working with ODStorageUnits.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#define _OD_DONT_IMPORT_CPP_

#define __PRIVATE__

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include "StdTypes.xh"
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifdef __PRIVATE__
#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif
#endif // __PRIVATE__

#ifdef __PRIVATE__
#ifndef _EXCEPT_
#include <Except.h>
#endif
#else
#ifndef _ODEXCEPT_
#include <ODExcept.h>
#endif
#endif // _PRIVATE__


#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifdef __PRIVATE__
#ifdef _PLATFORM_MACINTOSH_
#ifndef __TEXTEDIT__
#include <TextEdit.h>
#endif
#else // !defined(_PLATFORM_MACINTOSH_)
#ifndef __ODPAGTUN__
#include <ODPagTun.h>
#endif
#endif // defined(_PLATFORM_MACINTOSH_)
#endif // __PRIVATE__

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

//==============================================================================
// ODSUAddPropValue
//==============================================================================
#ifdef _PLATFORM_MACINTOSH_
void	ODSUAddPropValue(Environment* ev, ODStorageUnit* su, 
                       ODPropertyName prop, ODValueType val)
#else
SOMEXTERN WIN32_DLLEXPORT void	SOMLINK  ODSUAddPropValue(Environment* ev, ODStorageUnit* su, 
                       ODPropertyName prop, ODValueType val)
#endif // _PLATFORM_MACINTOSH_

{
	su->AddProperty(ev, prop)->AddValue(ev, val);
}

//==============================================================================
// ODSUForceFocus
//==============================================================================
#ifdef _PLATFORM_MACINTOSH_
void	ODSUForceFocus(Environment* ev, ODStorageUnit* su,
                                ODPropertyName prop, ODValueType val)
#else
SOMEXTERN WIN32_DLLEXPORT void	SOMLINK ODSUForceFocus(Environment* ev, ODStorageUnit* su,
                                ODPropertyName prop, ODValueType val)
#endif // _PLATFORM_MACINTOSH_

{
	if (prop != kODNULL) {
		if (su->Exists(ev, prop, kODNULL, 0) == kODFalse)
			su->AddProperty(ev, prop);
		else
			su->Focus(ev, prop, kODPosUndefined, kODNULL, 0, kODPosUndefined);
	}
	if (val != kODNULL) { 
		if (su->Exists(ev, prop, val, 0) == kODFalse)
			su->AddValue(ev, val);
		else
			su->Focus(ev, prop, kODPosSame, val, 0, kODPosUndefined);
	}
	// else; // Presumably the caller has already focussed the su somewhere,
			 // perhaps midstream. -TC
}

//==============================================================================
// ODSUExistsThenFocus
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
ODBoolean  ODSUExistsThenFocus(Environment* ev, ODStorageUnit* su,
                                  ODPropertyName prop, ODValueType val)
#else
SOMEXTERN WIN32_DLLEXPORT ODBoolean  SOMLINK  ODSUExistsThenFocus(Environment* ev, ODStorageUnit* su,
                                  ODPropertyName prop, ODValueType val)
#endif // _PLATFORM_MACINTOSH_

{
	if (prop == kODNULL && val == kODNULL)
		return kODTrue;
	// else; // Presumably the caller has already focussed the su somewhere,
			 // perhaps midstream. Property:kODNULL && ValueType:kODNULL always 'exist'. -TC
			 
	if (su->Exists(ev, prop, val, 0))
	{
		if ( val )
			su->Focus(ev,  prop, kODPosSame, val, 0, kODPosSame);
		else
			su->Focus(ev,  prop, kODPosSame, kODNULL, 0, kODPosAll);
		return kODTrue;	
	}
	else
	{
		return kODFalse;
	}
}

//==============================================================================
// ODSURemoveProperty
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
void ODSURemoveProperty(Environment* ev, ODStorageUnit* su, ODPropertyName prop)
#else
SOMEXTERN WIN32_DLLEXPORT void SOMLINK ODSURemoveProperty(Environment* ev, ODStorageUnit* su, ODPropertyName prop)
#endif // _PLATFORM_MACINTOSH_
{
	if ( ODSUExistsThenFocus(ev, su, prop, kODNULL) )
		su->Remove(ev);
}

#ifdef __PRIVATE__
//----------------------------------------------------------------------------------------
// GetPlatformFileFromContainer
//----------------------------------------------------------------------------------------
PlatformFile*	GetPlatformFileFromContainer(Environment* ev, ODContainer* container)
{
	ODByteArray	ba = container->GetID(ev);
	PlatformFile*	file = new PlatformFile();
	file->Specify((ODFileSpec*) ba._buffer);
	ODDisposePtr(ba._buffer);
	
	return file;
}

//----------------------------------------------------------------------------------------
// GetODFileSpecFromContainer
//----------------------------------------------------------------------------------------
ODFileSpec GetODFileSpecFromContainer(Environment* ev, ODContainer* container)
{
	ODByteArray	ba = container->GetID(ev);
	ODFileSpec documentSpec = *((ODFileSpec*) ba._buffer);
	ODDisposePtr(ba._buffer); // DMc dispose when done

	return documentSpec;
}

//----------------------------------------------------------------------------------------
// CreateFileContainer
//----------------------------------------------------------------------------------------
ODContainer*	CreateFileContainer(Environment* ev, ODSession* session, ODFileSpec* fsSpec)
{
#ifdef _PLATFORM_MACINTOSH_
	ODByteArray*	ba = CreateByteArray(fsSpec, sizeof(short) + sizeof(long) + fsSpec->name[0] + 1);
#else
	ODByteArray*	ba = CreateByteArray(fsSpec, strlen((char *) (&(fsSpec->name[0]))) + 1);
#endif
	ODContainer*	newContainer = session->GetStorageSystem(ev)->
		CreateContainer(ev,kODDefaultFileContainer /*file->GetContainerType()*/,ba);
	DisposeByteArray(ba);
	
	return newContainer;
}

//----------------------------------------------------------------------------------------
// GetFileContainer
//----------------------------------------------------------------------------------------
ODContainer*	GetFileContainer(Environment* ev, ODSession* session, ODFileSpec* fsSpec)
{
#ifdef _PLATFORM_MACINTOSH_
	ODByteArray*	ba = CreateByteArray(fsSpec, sizeof(short) + sizeof(long) + fsSpec->name[0] + 1);
#else
	ODByteArray*	ba = CreateByteArray(fsSpec, strlen((char *) (&(fsSpec->name[0]))) + 1);
#endif
	ODContainer*	newContainer = session->GetStorageSystem(ev)->
		AcquireContainer(ev,kODDefaultFileContainer /*file->GetContainerType()*/,ba);
	DisposeByteArray(ba);
	
	return newContainer;
}
	
//----------------------------------------------------------------------------------------
// CreateMemoryContainer
//----------------------------------------------------------------------------------------
ODContainer* CreateMemoryContainer(Environment* ev,
				ODSession* session,
				ODHandle handle,
				ODContainerType containerType)
{
	ODByteArray* ba;
	TRY
		ODLockHandle(handle);
		ba = CreateByteArray(&handle, sizeof(ODHandle));
		ODUnlockHandle(handle);
	CATCH_ALL
		ODUnlockHandle(handle);
		RERAISE;
	ENDTRY

	ODContainer* newContainer = session->GetStorageSystem(ev)->
		CreateContainer(ev, containerType, ba);
	DisposeByteArray(ba);
	
	return newContainer;
}

//----------------------------------------------------------------------------------------
// GetMemoryContainer
//----------------------------------------------------------------------------------------
ODContainer* GetMemoryContainer(Environment* ev, 
				ODSession* session,
				ODHandle handle,
				ODContainerType containerType)
{
	ODByteArray* ba;
	TRY
		ODLockHandle(handle);
		ba = CreateByteArray(&handle, sizeof(ODHandle));
		ODUnlockHandle(handle);
	CATCH_ALL
		ODUnlockHandle(handle);
		RERAISE;
	ENDTRY

	ODContainer* newContainer = session->GetStorageSystem(ev)->
		AcquireContainer(ev, containerType, ba);
	DisposeByteArray(ba);
	
	return newContainer;
}
#endif // __PRIVATE__

//----------------------------------------------------------------------------------------
// StorageUnitGetValue
//----------------------------------------------------------------------------------------
#ifdef _PLATFORM_MACINTOSH_
ODULong	StorageUnitGetValue(ODStorageUnit* su, Environment* ev,
					ODULong	size, ODPtr buffer) 
#else
SOMEXTERN WIN32_DLLEXPORT ODULong  SOMLINK  StorageUnitGetValue(ODStorageUnit* su, Environment* ev,
					ODULong	size, ODPtr buffer) 
#endif // _PLATFORM_MACINTOSH_
{
	ODByteArray	ba;	
	ODULong bytesRead = su->GetValue(ev, size, &ba);

	if (bytesRead > 0)
	{
	    ODBlockMove(ba._buffer, buffer, bytesRead);
	    ODDisposePtr(ba._buffer);
	}

	return bytesRead;
}

//----------------------------------------------------------------------------------------
// StorageUnitViewGetValue
//----------------------------------------------------------------------------------------
#ifdef _PLATFORM_MACINTOSH_
ODULong	StorageUnitViewGetValue(ODStorageUnitView* suv,
							Environment* ev,
							ODULong	size,
							ODPtr buffer)
#else
SOMEXTERN WIN32_DLLEXPORT ODULong  SOMLINK  StorageUnitViewGetValue(ODStorageUnitView* suv,
							Environment* ev,
							ODULong	size,
							ODPtr buffer)
#endif // _PLATFORM_MACINTOSH_
{
	ODByteArray	ba;
	
	ODULong bytesRead = suv->GetValue(ev, size, &ba);

	if (bytesRead > 0)
	{
	    ODBlockMove(ba._buffer, buffer, ba._length);
	    ODDisposePtr(ba._buffer);
	}
	
	return bytesRead;
}


//----------------------------------------------------------------------------------------
// StorageUnitSetValue
//----------------------------------------------------------------------------------------
#ifdef _PLATFORM_MACINTOSH_
void	StorageUnitSetValue(ODStorageUnit* su,
							Environment* ev,
							ODULong	size,
							const void *buffer)
#else
SOMEXTERN WIN32_DLLEXPORT void	SOMLINK StorageUnitSetValue(ODStorageUnit* su,
							Environment* ev,
							ODULong	size,
							const void *buffer)
#endif // _PLATFORM_MACINTOSH_
{
	ODByteArray ba;
	ba._length = size;
	ba._maximum = size;
	ba._buffer = (octet*) buffer;
	su->SetValue(ev, &ba);
}

//----------------------------------------------------------------------------------------
// StorageUnitViewSetValue
//----------------------------------------------------------------------------------------
#ifdef _PLATFORM_MACINTOSH_
void	StorageUnitViewSetValue(ODStorageUnitView* suv,
							Environment* ev,
							ODULong	size,
							const void *buffer)
#else
SOMEXTERN WIN32_DLLEXPORT void  SOMLINK StorageUnitViewSetValue(ODStorageUnitView* suv,
							Environment* ev,
							ODULong	size,
							const void *buffer)
#endif // _PLATFORM_MACINTOSH_
{
	ODByteArray ba;
	ba._length = size;
	ba._maximum = size;
	ba._buffer = (octet*) buffer;
	suv->SetValue(ev, &ba);
}

//----------------------------------------------------------------------------------------
// StorageUnitSetPromiseValue
//----------------------------------------------------------------------------------------
#ifdef _PLATFORM_MACINTOSH_
void	StorageUnitSetPromiseValue(ODStorageUnit* su,
							Environment* ev,
							ODValueType valueType,
							ODULong offset,
							ODULong	size,
							const void *buffer,
							ODPart *sourcePart)
#else
SOMEXTERN WIN32_DLLEXPORT void  SOMLINK  StorageUnitSetPromiseValue(ODStorageUnit* su,
							Environment* ev,
							ODValueType valueType,
							ODULong offset,
							ODULong	size,
							const void *buffer,
							ODPart *sourcePart)
#endif // _PLATFORM_MACINTOSH_
{
	ODByteArray ba;
	ba._length = size;
	ba._maximum = size;
	ba._buffer = (octet*) buffer;
	su->SetPromiseValue(ev, valueType, offset, &ba, sourcePart);
}

//----------------------------------------------------------------------------------------
// StorageUnitGetStylFromStyledText
//----------------------------------------------------------------------------------------
#ifdef _PLATFORM_MACINTOSH_
ODBoolean StorageUnitGetStylFromStyledText(ODStorageUnit* su,
							Environment* ev,
							ODULong* size,
							ODPtr* styl)
{
	ODBoolean	result = kODFalse;
	ODULong		stxtSize;
	ODULong		stylSize;
	ODUShort	scrpNStyles;
	ODType		applestxt = kODNULL;

#if defined(_PLATFORM_WIN32_) && defined(PORTING_HACK)
	// Note:  This cast should not be necessary.  On DR4, ODPlatform eventually
	//        ends up as an unsigned long.  For some reason, it's a void* on
	//        WIN32.  Talk to Leigh Williamson about this.
	const ODPlatformType kODScrapType_stxt = (ODPlatformType) 0x73747874;	// 'stxt'
#else // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	const ODPlatformType kODScrapType_stxt = 0x73747874;	// 'stxt'
#endif // defined(_PLATFORM_WIN32_) && defined(PORTING_HACK)

	ODVolatile(applestxt);
	ODVolatile(result);
	
	SOM_TRY

		ODTranslation* translation = su->GetSession(ev)->GetTranslation(ev);
		applestxt = translation->GetISOTypeFromPlatformType(ev, kODScrapType_stxt, kODPlatformDataType);
			
		if (ODSUExistsThenFocus(ev, su, kODPropContents, applestxt) )
		{
			stxtSize = su->GetSize(ev);
			if ( stxtSize >= sizeof(ODUShort) )
			{
				StorageUnitGetValue(su, ev, sizeof(ODUShort), &scrpNStyles);
				stylSize = (scrpNStyles * sizeof(ScrpSTElement)) + sizeof(ODUShort);
				// Mac-specific format.  Apprently the data is stored in Clipbd.cpp,
				// so this code needs to be sync'ed up with Clipbd.cpp if ever
				// used.
				if ( stxtSize >= stylSize )
				{
					*styl = ODNewPtr(stylSize);
					*size = stylSize;
					su->SetOffset(ev, 0);
					StorageUnitGetValue(su, ev, stylSize, *styl);
					result = kODTrue;
				}
			}
		}
		
		delete applestxt;
	
	SOM_CATCH_ALL
			if ( applestxt )
				delete applestxt;
			result = kODFalse;
	SOM_ENDTRY

	return result;
}
#endif // _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
// GetOriginalCloneKind
//------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
ODCloneKind GetOriginalCloneKind(Environment* ev, ODDraft* draft)
#else
SOMEXTERN WIN32_DLLEXPORT ODCloneKind SOMLINK GetOriginalCloneKind(Environment* ev, ODDraft* draft)
#endif //_PLATFORM_MACINTOSH_
{
	// If content was put on the clipboard without cloning, there will be no
	// kODPropOriginalCloneKind property in the draft's preferences storage
	// unit.  Assume the original operation was a copy in this case. Since
	// no links cannot be placed on the clipboard directly, there won't be
	// links needing fixup on a paste.

	TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);
	ODCloneKind cloneKind = ODGetULongProp(ev, draftProperties, kODPropOriginalCloneKind, kODULong);
	
	if ((ODULong)(cloneKind) != 0)
		return cloneKind;
	else
		return kODCloneCopy;
}

//------------------------------------------------------------------------------
// RemoveDataInterchangeProperties
//------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
void RemoveDataInterchangeProperties (Environment* ev,
		ODStorageUnit*	su,
		ODBoolean  clonedToFileContainer)
#else
SOMEXTERN WIN32_DLLEXPORT  void  SOMLINK  RemoveDataInterchangeProperties (Environment* ev,
		ODStorageUnit*	su,
		ODBoolean  clonedToFileContainer)
#endif // _PLATFORM_MACINTOSH_
{
	ODSURemoveProperty(ev, su, kODPropLinkSpec);
	ODSURemoveProperty(ev, su, kODPropMouseDownOffset);
	ODSURemoveProperty(ev, su, kODPropCloneKindUsed);
	if ( !clonedToFileContainer )
	{
		ODSURemoveProperty(ev, su, kODPropProxyContents);
		ODSURemoveProperty(ev, su, kODPropContentFrame);
		ODSURemoveProperty(ev, su, kODPropSuggestedFrameShape);
	}
}

