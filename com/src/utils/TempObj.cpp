//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.7 com/src/utils/TempObj.cpp, odutils, od96os2, odos29646d 10/2/96 12:58:56 [ 11/15/96 15:28:57 ]
/*
	File:		TempObj.cpp

	Contains:	Template utilities for exception-safe temporary object references

	Owned by:	Jens Alfke

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#define __PRIVATE__

#ifndef SOM_ODRefCntObject_xh
#include <RefCtObj.xh>
#endif

#ifndef SOM_ODExtension_xh
#include <Extensn.xh>
#endif

#ifndef SOM_ODSettingsExtension_xh
#include <Settings.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODPart_xh
#include "Part.xh"
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifdef __PRIVATE__
#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif
#endif // __PRIVATE__

#ifndef SOM_ODLink_xh
#include <Link.xh>
#endif

#ifndef SOM_ODLinkSource_xh
#include <LinkSrc.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODMenuBar_xh
#include <MenuBar.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifdef __PRIVATE__
#ifndef _PLFMFILE_
#include "PlfmFile.h"
#endif
#endif // __PRIVATE__

#ifndef _UTILERRS_
#include <UtilErrs.h>
#endif

#ifndef _USE_TEMPLATES_

	#ifdef __MWERKS__
	#pragma push
	#pragma once off
	#endif

	// Define the non-inline methods of the various template classes:

	#define _TMPL_IMPL_
	
	#define _T_		ODObject
	#define _C_		TempODObject
	#include "TempObj.th"
	#define _T_		ODSession
	#define _C_		TempODSession
	#include "TempObj.th"
	#define _T_		ODTypeList
	#define _C_		TempODTypeList
	#include "TempObj.th"
	#define _T_		ODStorageUnitView
	#define _C_		TempODStorageUnitView
	#include "TempObj.th"
	
	#define _T_		ODRefCntObject
	#define _C_		TempODRefCntObject
	#include "TempRef.th"
	#define _T_		ODWindow
	#define _C_		TempODWindow
	#include "TempRef.th"
	#define _T_		ODFrame
	#define _C_		TempODFrame
	#include "TempRef.th"
	#define _T_		ODPart
	#define _C_		TempODPart
	#include "TempRef.th"
	#define _T_		ODShape
	#define _C_		TempODShape
	#include "TempRef.th"
	#define _T_		ODTransform
	#define _C_		TempODTransform
	#include "TempRef.th"
	#define _T_		ODStorageUnit
	#define _C_		TempODStorageUnit
	#include "TempRef.th"

	#define _T_		ODDraft
	#define _C_		TempODDraft
	#include "TempRef.th"
	#define _T_		ODDocument
	#define _C_		TempODDocument
	#include "TempRef.th"
	#define _T_		ODContainer
	#define _C_		TempODContainer
	#include "TempRef.th"
	#define _T_		ODLink
	#define _C_		TempODLink
	#include "TempRef.th"
	#define _T_		ODLinkSource
	#define _C_		TempODLinkSource
	#include "TempRef.th"
	#define _T_		ODMenuBar
	#define _C_		TempODMenuBar
	#include "TempRef.th"

/*	#define _T_		ODExtension
	#define _C_		TempODExtension
	#include "TempRef.th"
	#define _T_		ODSettingsExtension
	#define _C_		TempODSettingsExtension
	#include "TempRef.th"
*/

	#undef _TMPL_IMPL_

	#ifdef __MWERKS__
	#pragma pop
	#endif

#endif /*_USE_TEMPLATES_*/

WIN32_DLLEXPORT TempODPtr::TempODPtr( ) : fBlock(kODNULL)
{
}

WIN32_DLLEXPORT TempODPtr::TempODPtr( void *block ) : fBlock(block)
{
}

WIN32_DLLEXPORT TempODPtr::~TempODPtr( )
{
	delete fBlock;
	fBlock = kODNULL;
}

BaseTempObj::BaseTempObj( )
{
	fObj = kODNULL;
}

WIN32_DLLEXPORT BaseTempObj::~BaseTempObj( )
{
	delete fObj;
	fObj = kODNULL;
}

BaseTempRef::BaseTempRef( )
{
	fObj = kODNULL;
}

WIN32_DLLEXPORT void
BaseTempRef::Release( )
{
	if( fObj ) {
		ASSERT_IS_A(fObj,ODRefCntObject);
		fObj->Release(somGetGlobalEnvironment());
		fObj = kODNULL;
	}
}

WIN32_DLLEXPORT BaseTempRef::~BaseTempRef( )
{
	if( fObj ) {

		// In case the destructor is being called because a C++
		// exception was thrown - clear any SOM exceptions errors
		// that exist in the ev, release the object, then reset
		// the SOM errors.
		Environment *ev = somGetGlobalEnvironment();
		exception_type   nonOpenDocSOMExceptionType;
		string           nonOpenDocSOMExceptionID;
		somToken         nonOpenDocSOMExceptionValue;
		ODError savedExistingError = ODGetSOMException(ev);

		// If there is a non-OpenDoc SOM exception, save the data.
		if (savedExistingError == kODErrSOMException) {
			nonOpenDocSOMExceptionType = ev->_major;
			nonOpenDocSOMExceptionID = somExceptionId(ev);
			nonOpenDocSOMExceptionValue = somExceptionValue(ev);

			// now clear the non-OpenDoc SOM exception
			ev->_major = NO_EXCEPTION;
		}
		// else, if there is an OpenDoc SOM exception, clear it.
		else if (savedExistingError != kODNoError)
			ODSetSOMException(ev, kODNoError);

		//  If a previous error occurred, don't let a new C++
		//  error prevent us from re-setting the SOM exception
		try {

		    ASSERT_IS_A(fObj,ODRefCntObject);
		    fObj->Release(ev);
		    fObj = kODNULL;

		} catch (...) {

		    // If there was an original SOM Exception, return
		    // that instead of the new C++ exception.
		    if (savedExistingError == kODNoError)
		    	throw;
		}

		// If we originally had a non-OpenDoc SOM exception,
		// restore it.
		if (savedExistingError == kODErrSOMException)
			somSetException(ev,
					nonOpenDocSOMExceptionType,
					nonOpenDocSOMExceptionID,
					nonOpenDocSOMExceptionValue);

		// else, if we originally had an OpenDoc SOM exception,
		// restore it. 
		else if (savedExistingError != kODNoError)
			ODSetSOMException(ev, savedExistingError);
		
	}
}

//===========================================================================
//	Temp strings.
//===========================================================================

WIN32_DLLEXPORT TempODString::TempODString( char* s ) : fStr(s)
{
}

WIN32_DLLEXPORT TempODString::~TempODString( )
{
	char* s = fStr;
	fStr = kODNULL;
	delete s;
}
#ifdef __PRIVATE__
//===========================================================================
//	Temp handles.
//===========================================================================


TempODHandle::TempODHandle( ODHandle h ) : fHandle(h)
{
}

TempODHandle::~TempODHandle( )
{
	ODHandle h = fHandle;
	fHandle = kODNULL;
	if ( h )
		ODDisposeHandle( h );
}

TempODHandleLock::TempODHandleLock( ODHandle h ) : fHandle(h)
{
	if ( h )
		ODLockHandle( h );
}

TempODHandleLock::~TempODHandleLock( )
{
	ODHandle h = fHandle;
	fHandle = (ODHandle) kODNULL;
	if ( h )
		ODUnlockHandle( h );
}

//===========================================================================
//	Temp platform files.
//===========================================================================


TempPlatformFile::TempPlatformFile( PlatformFile* f ) : fFile(f)
{
}

TempPlatformFile::~TempPlatformFile( )
{
	PlatformFile* f = fFile;
	fFile = kODNULL;
	if ( f )
		delete f;
}
#endif // __PRIVATE__

//===========================================================================
//	Temp ODPasteAsResult.
//===========================================================================


WIN32_DLLEXPORT TempODPasteAsResult::TempODPasteAsResult( ODPasteAsResult* r ) : fResult(r)
{
}

WIN32_DLLEXPORT TempODPasteAsResult::~TempODPasteAsResult( )
{
	ODPasteAsResult* r = fResult;
	fResult = kODNULL;
	if ( r )
	{
		if  ( r->selectedKind ) 
		{
			delete r->selectedKind;
			r->selectedKind = kODNULL;
		}
		if  ( r->translateKind ) 
		{
			delete r->translateKind;
			r->translateKind = kODNULL;
		}
		if  ( r->editor ) 
		{
			delete r->editor;
			r->editor = kODNULL;
		}
	}
}

#ifdef _PLATFORM_MACINTOSH_
//===========================================================================
//	Temp AEDescs.
//===========================================================================


TempAEDesc::TempAEDesc( AEDesc* d ) : fDesc(d)
{
}

TempAEDesc::~TempAEDesc( )
{
	AEDesc* d = fDesc;
	fDesc = kODNULL;
	if ( d )
		AEDisposeDesc(d);
}
#endif

//===========================================================================
//	Temp ODIText.
//===========================================================================

WIN32_DLLEXPORT TempODIText::TempODIText( ODIText* t ) : fText(t)
{
}

WIN32_DLLEXPORT TempODIText::~TempODIText( )
{
	ODIText* t = fText;
	fText = kODNULL;
	if ( t )
		DisposeIText(t);
}

//===========================================================================
//	Temp byte arrays.
//===========================================================================


WIN32_DLLEXPORT TempODByteArray::TempODByteArray( ODByteArray* ba ) : fBA(ba)
{
}

WIN32_DLLEXPORT TempODByteArray::~TempODByteArray( )
{
	ODByteArray* ba = fBA;
	fBA = kODNULL;
	if ( ba )
	{
		if ( ba->_buffer )
			ODDisposePtr(ba->_buffer);
		ODDisposePtr(ba);
	}
}

WIN32_DLLEXPORT TempODByteArrayStruct::TempODByteArrayStruct( )
{
	fBA._buffer = kODNULL;
}

WIN32_DLLEXPORT TempODByteArrayStruct::TempODByteArrayStruct( ODByteArray ba ) : fBA(ba)
{
}

WIN32_DLLEXPORT TempODByteArrayStruct::~TempODByteArrayStruct( )
{
	ODDisposePtr(fBA._buffer);
}
