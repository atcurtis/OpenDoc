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
// @(#) 1.5 com/src/storage/Translt.cpp, oddataxfer, od96os2, odos29646d 7/23/96 18:30:16 [ 11/15/96 15:26:06 ]
/*
	File:		Translt.cpp

	Contains:	Implementation of ODTranslation class.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<33>	10/24/95	VL		1294279: Checks existence of CFM library in
									addition to Gestalt.
		<32>	10/18/95	CC		GM 1293725: ShowTranslateDialog:
									initialEditor parameter may be null.
		<31>	10/16/95	VL		1291692: Added SOM_TRY to
									GetISOTypeFromPlatformType and
									GetPlatformTypeFromISOType.
		<30>	 10/9/95	VL		1288571: Moved
									kODErrNoSysTranslationFacility to
									ErrorDef.idl.
		<29>	 10/8/95	TJ		Fixes Recomended by Refball
		<28>	 9/12/95	DM		1280020 FB1 part trans dlg mem leaks
		<27>	 8/26/95	T‚		1274606 FB2: Patching Remarks
		<26>	 8/15/95	CC		1276520: ShowTranslateDialog: Changed
									"inout" parameter to one "in" and one
									"out".
		<25>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<24>	 7/28/95	VL		1270320: Use TempODTypeList for
									GetTranslationOf and CommonTranslate.
		<23>	 7/14/95	TJ		Turned On TransLation for 68K.
		<22>	 6/21/95	CC		1260937: BB: ODTypeListIterator must return
									copies of ODType values.
		<21>	  6/7/95	CC		1237791: LocalGetPlatformTypeFromISOType:
									Initialize result to 0.
		<20>	  6/2/95	CC		1236618: Use CreateTypeList factory method.
		<19>	 5/26/95	VL		1251403: Multithreading naming support.
		<18>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<17>	 5/19/95	VL		1249626: Allocate memory for out
									ODByteArray in Translate.
		<16>	 4/25/95	VL		1210982: Removed 5$.
		<15>	 3/28/95	CC		1229640: Added forPaste parameter to
									ShowTranslateDialog call.
		<14>	 3/28/95	CC		1233117: Removed unused ODStorageUnit
									parameter from  ShowPartTranslateDialog.
		<13>	 3/27/95	CC		1229640: Renamed ShowTranslateDialog() to
									ShowPartTranslateDialog().
									Added ShowPartTranslateDialog().
		<12>	 3/23/95	VL		1228275: ASSERT for null ODByteArray.
		<11>	 2/10/95	CG		#1177475: Added include for StdDefs.xh.
		<10>	 1/10/95	CC		1209483 TranslateView() fixed to write into
									destination view.
		 <9>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <8>	12/13/94	CG		#1187128: Moved Translation dialog code to
									Storage subsystem.
		 <7>	 10/4/94	VL		1162335: Moved consts to StorDef.h.
		 <6>	 9/29/94	RA		1189812: Mods for 68K build.
		 <5>	 9/20/94	VL		Fixes for new MEO headers.
		 <4>	 8/28/94	VL		1183500: Added implementation based on MEO
									1.1.
		 <3>	 8/16/94	VL		1180054: TypeSet should be TypeList.
		 <2>	 7/26/94	VL		Changed ODType to ODValueType.
		 <1>	 6/28/94	VL		first checked in
		 <1>	 6/28/94	VL		first checked in

	To Do: 
	In Progress:
*/

#define ODTranslation_Class_Source
#define VARIABLE_MACROS
#include <Translt.xih>

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif


#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _STORDEF_
#include "StorDef.h"
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __TRANSLATION__
#include <Translation.h>
#endif

#ifndef __GESTALTEQU__
#include <GestaltEqu.h>	// For Gestalt
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>	// For BitTst
#endif

#ifndef __EDITIONS__
#include <Editions.h>	// For FormatType
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef _TRANSDLG_
#include <TransDlg.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#pragma segment ODTranslation

#include "TransltB.cpp"	// Platform-independent methods, if any

#if ODDebug
// #define ODDebug_Translation 1
#endif
#ifdef _PLATFORM_OS2_
#define PLFM_TEXT						0x54584554L	/* "TEXT" */		
#define PLFM_DSPTEXT						0x44545854L	/* "TXTD" */
#define PLFM_BITMAP						0x504D5442L	/* "BTMP" */	
#define PLFM_DSPBITMAP						0x44504D42L	/* "BMPD" */
#define PLFM_METAFILE				                0x4654454DL	/* "METF" */
#define PLFM_DSPMETAFILE				        0x4454454DL	/* "METD" */	
#define PLFM_PALETTE					        0x544C4150L	/* "PALT" */	
#endif
#ifdef _PLATFORM_MACINTOSH_
//==============================================================================
// Scalar Types
//==============================================================================

typedef struct {
	ODOSType	destType;
} TranslationHints;

typedef struct {
	FormatType	theType;
	long		theLength;
} FormatAvailable;

#endif // _PLATFORM_MACINTOSH_
//==============================================================================
// Local Classes
//==============================================================================

class TranslationRefCon {

public:

	TranslationRefCon() {fTypeList = kODNULL;
							fDataPtr = kODNULL;
							fDataType = kODNULL;
							fDataSize = 0;}
	~TranslationRefCon() {}
	
	void InitTranslationRefCon(ODTypeList* typeList) {fTypeList = typeList;}
	void InitTranslationRefCon(ODTypeList* 	typeList,
								ODPtr			dataPtr, 
								ODPlatformType	dataType,
								ODULong		dataSize) {fTypeList = typeList;
															fDataPtr = dataPtr;
															fDataType = dataType;
															fDataSize = dataSize;}

	ODTypeList*	fTypeList;
	ODPtr			fDataPtr;
	ODPlatformType	fDataType;
	ODULong		fDataSize;
};

//==============================================================================
// Function Prototype
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
static OSErr CommonTranslate(ODTranslation* translation,
						Environment*	ev,
						ODValueType		fromType,
						ODByteArray*	fromData,
						ODValueType		toType,
						ODHandle*		destHandle);

static pascal OSErr GetScrapDataProc(ScrapType requestedFormat,
							Handle dataH,
							void* srcDataGetterRefCon);
#endif // _PLATFORM_MACINTOSH_

static ODType LocalGetISOTypeFromPlatformType(ODPlatformType platformType,
			ODPlatformTypeSpace typeSpace);

static ODPlatformType LocalGetPlatformTypeFromISOType(ODType isoType);

#ifdef _PLATFORM_MACINTOSH_
//==============================================================================
// Globals
//==============================================================================

static RoutineDescriptor	gGetScrapDataProcRD = BUILD_ROUTINE_DESCRIPTOR(uppGetScrapDataProcInfo, GetScrapDataProc);

#endif // _PLATFORM_MACINTOSH_

//==============================================================================
// Class ODTranslation
//==============================================================================

//------------------------------------------------------------------------------
// ODTranslation: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODTranslationPurge(ODTranslation *somSelf, Environment *ev,
		ODSize size)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationPurge");

	return 0;
}

//------------------------------------------------------------------------------
// ODTranslation: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTranslationsomUninit(ODTranslation *somSelf)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationsomUninit");

#ifdef _PLATFORM_MACINTOSH_
	ODDeleteObject(_fTranslationDialog);
#endif // _PLATFORM_MACINTOSH_
    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODTranslation: InitTranslation
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTranslationInitTranslation(ODTranslation *somSelf, Environment *ev,
		ODSession* session)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationInitTranslation");

	SOM_TRY

	/* Moved from somInit. SOM itself sets fields to zero
	_fMEOExists = kODFalse;
	_fTranslationDialog = kODNULL;
	*/
    somSelf->InitObject(ev);

	_fSession = session;

#ifdef _PLATFORM_MACINTOSH_
	ODSLong result;
	
	_fMEOExists = (Gestalt(gestaltTranslationAttr, &result) == noErr) 
					&& (BitTst(&result, 31 - gestaltTranslationMgrExists))
					&& (&TranslateScrap != (void*)kUnresolvedCFragSymbolAddress);
	
	_fTranslationDialog = new TranslationDialog();
	if (_fTranslationDialog)
		_fTranslationDialog->InitTranslationDialog(session);
#endif // _PLATFORM_MACINTOSH_

	SOM_CATCH_ALL
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODTranslation: CanTranslate
//------------------------------------------------------------------------------

SOM_Scope ODTranslateResult  SOMLINK ODTranslationCanTranslate(ODTranslation *somSelf, Environment *ev,
		ODValueType fromType)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationCanTranslate");
	
	ODTranslateResult	result;

	SOM_TRY

	ODTypeList* typeList = somSelf->GetTranslationOf(ev, fromType);
	
	if (typeList->Count(ev) > 0)
		result = kODCanTranslate;
	else
		result = kODCannotTranslate;
		
	delete typeList;
	
	SOM_CATCH_ALL
		result = kODCannotTranslate;
	SOM_ENDTRY
		
	return result;
}

//------------------------------------------------------------------------------
// ODTranslation: ShowPartTranslateDialog
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODTranslationShowPartTranslateDialog(ODTranslation *somSelf, Environment *ev,
	ODPartWrapper* partWrapper)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf);
    ODTranslationMethodDebug("ODTranslation","ODTranslationShowPartTranslateDialog");

#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return kODFalse;

	return _fTranslationDialog->ShowPartTranslateDialog(partWrapper);
#endif // _PLATFORM_MACINTOSH_
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        return kODFalse;
#endif // _PLATFORM_OS2_ or _PLATFORM_AIX_
}

//------------------------------------------------------------------------------
// ODTranslation: ShowTranslateDialog
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODTranslationShowTranslateDialog(ODTranslation *somSelf, Environment *ev,
				ODTypeList*	translateToList,
				ODUShort	initialKind,
				ODUShort*	selectedKind,
				ODEditor	initialEditor,
				ODBoolean	canChangeEditor,
				ODEditor*	selectedEditor)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf);
    ODTranslationMethodDebug("ODTranslation","ODTranslationShowTranslateDialog");

	ODBoolean result = kODFalse;	ODVolatile(result);
#ifdef _PLATFORM_MACINTOSH_
	ODUShort kind = initialKind;	ODVolatile(kind);
	ODEditor editor = kODNULL;		ODVolatile(editor);

	*selectedKind = initialKind;
	*selectedEditor = kODNULL;

	SOM_TRY
	
		// Convert from initial kind and editor
		if ( initialEditor != kODNULL )
			editor = ODISOStrFromCStr((char *) initialEditor);
	
		result = _fTranslationDialog->ShowTranslateDialog(
													translateToList,
													&kind,
													&editor,
													canChangeEditor,
													kODTrue);
	
		// Convert to selected kind and editor
		if ( result )
		{
			*selectedKind = kind;
			*selectedEditor = editor;
			editor = kODNULL;
		}

	SOM_CATCH_ALL
	
	SOM_ENDTRY

	ODDisposePtr((ODPtr) editor);
#endif // _PLATFORM_MACINTOSH_

	return result;
}

//------------------------------------------------------------------------------
// ODTranslation: GetTranslationOf
//------------------------------------------------------------------------------
// Return the set of kinds the argument may be translated into.  Internally, this
// routine converts to and from platform types to use the translation services of
// the host platform.

SOM_Scope ODTypeList*  SOMLINK ODTranslationGetTranslationOf(ODTranslation *somSelf, Environment *ev,
		ODValueType fromType)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationGetTranslationOf");

#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return kODNULL;
#endif // _PLATFORM_MACINTOSH_
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif // _PLATFORM_OS2_ or _PLATFORM_AIX_
	
#ifdef _PLATFORM_MACINTOSH_
	ODHandle destinationTypes;
	
	if (_fMEOExists == kODFalse)
		THROW(kODErrNoSysTranslationFacility);

	// Initialize the parameters for TranslateScrap
	// Set up ODTypeList as refCon
	
	TempODTypeList fromTypeList = _fSession->GetStorageSystem(ev)->CreateTypeList(ev, kODNULL);
	fromTypeList->AddLast(ev, fromType);
	
	TranslationRefCon refCon;
	refCon.InitTranslationRefCon(fromTypeList);

	destinationTypes = ODNewHandle(0);
	
	// This TranslateScrap is only a query
	

	OSErr err = noErr;

//#ifdef __MWERKS__
	// The following call is ifdef out because we don't have a 68K
	// CFM library for Translation Manager yet.
	
	TranslateScrap((GetScrapDataUPP) &gGetScrapDataProcRD,
					&refCon,
					kFormatListFormat,
					(Handle) destinationTypes,
					kTranslationScrapProgressDialogID);
					
//#endif

	THROW_IF_ERROR(err);
	
	// Put destinationTypes into a ODTypeList
	
	ODULong		numDestTypes = ODGetHandleSize(destinationTypes) / sizeof(FormatAvailable);
#ifdef ODDebug_Translation
	somPrintf("GetTranslationOf: numDestTypes %d\n", numDestTypes);
#endif
	FormatAvailable*	osTypes = (FormatAvailable*) ODLockHandle(destinationTypes);
	ODTypeList*	toTypeList = _fSession->GetStorageSystem(ev)->CreateTypeList(ev, kODNULL);
	
	OSType	fromOSType = LocalGetPlatformTypeFromISOType(fromType);
	
	for (ODULong i = 0; i < numDestTypes; i++) {
		if (osTypes[i].theType != fromOSType) {
			TempODType isoType = // DMc: make temp to ensure deletion
				somSelf->GetISOTypeFromPlatformType(ev, osTypes[i].theType, kODPlatformDataType);
			toTypeList->AddLast(ev, isoType);
		}
	}
	
	ODUnlockHandle(destinationTypes);
	ODDisposeHandle(destinationTypes);
	
	// Return the created ODTypeList
	
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
	ODTypeList* toTypeList = _fSession->GetStorageSystem(ev)->CreateTypeList(ev, kODNULL);
#endif // _PLATFORM_OS2_ or _PLATFORM_AIX_

	return toTypeList;
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
                return kODNULL;
        SOM_ENDTRY
#endif // _PLATFORM_OS2_ or _PLATFORM_AIX_
}

//------------------------------------------------------------------------------
// ODTranslation: TranslateView
//------------------------------------------------------------------------------

SOM_Scope ODTranslateResult  SOMLINK ODTranslationTranslateView(ODTranslation *somSelf, Environment *ev,
		ODStorageUnitView* fromView,
		ODStorageUnitView* toView)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationTranslateView");
	
	ODTranslateResult result = kODCannotTranslate;

#ifdef _PLATFORM_MACINTOSH_
	SOM_TRY

	if (_fMEOExists == kODFalse)
		THROW(kODErrNoSysTranslationFacility);

	ODHandle	toDataHandle = kODNULL;
	
	// Get the source data	
	TempODValueType fromType = fromView->GetType(ev); // DMc: ensure it's deleted
	
	ODULong size = fromView->GetSize(ev);
	ODByteArray	fromDataBA;
	fromView->GetValue(ev, size, &fromDataBA);

	// Get the to type
	TempODValueType	toType = toView->GetType(ev); // DMc: ensure it's deleted

	// Do the translation
	if (CommonTranslate(somSelf, ev, fromType, &fromDataBA, toType, &toDataHandle) == noErr) {
	
		// Create the toData
		ODPtr toData = ODLockHandle(toDataHandle);
		
		// Set the value in the toView
		StorageUnitViewSetValue(toView, ev, ODGetHandleSize(toDataHandle), toData);
		
		// Get rid of toData Byte Array and Handle

		ODUnlockHandle(toDataHandle);
		ODDisposeHandle(toDataHandle);

		// Set up result
		result = kODCanTranslate;
	}
	
	ODDisposePtr(fromDataBA._buffer);
	
	SOM_CATCH_ALL
		result = kODCannotTranslate;
	SOM_ENDTRY

	return result;

#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        THROW(kODErrNoSysTranslationFacility);
	return result;
#endif // _PLATFORM_OS2_ or _PLATFORM_AIX_
}

//------------------------------------------------------------------------------
// ODTranslation: Translate
//------------------------------------------------------------------------------

SOM_Scope ODTranslateResult  SOMLINK ODTranslationTranslate(ODTranslation *somSelf, Environment *ev,
		ODValueType		fromType,
		ODByteArray*	fromDataBA,
		ODValueType		toType,
		ODByteArray*	toDataBA)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationTranslate");
	
	ODTranslateResult	result = kODCannotTranslate;
#ifdef _PLATFORM_MACINTOSH_
	ODHandle			toDataHandle = kODNULL;
	
	SOM_TRY

	// Check for MEO
	
	if (_fMEOExists == kODFalse)
		THROW(kODErrNoSysTranslationFacility);
	
	ASSERT(fromDataBA != kODNULL, kODErrIllegalNullInput);
	ASSERT(toDataBA != kODNULL, kODErrIllegalNullInput);
	
	// Translate
	
	if (CommonTranslate(somSelf, ev, fromType, fromDataBA, toType, &toDataHandle) == noErr) {
		ODULong toDataSize = ODGetHandleSize(toDataHandle);
#ifdef ODDebug_Translation
	somPrintf("toDataHandle %x toDataSize %d\n", toDataHandle, toDataSize);
#endif
		ODPtr	toData = ODLockHandle(toDataHandle);
		
		*toDataBA = CreateByteArrayStruct(toData, toDataSize);
		
		// Get rid of handle
		ODUnlockHandle(toDataHandle);
		ODDisposeHandle(toDataHandle);
		
		// Set up return code
		result = kODCanTranslate;
	}
	
	SOM_CATCH_ALL
		result = kODCannotTranslate;
	SOM_ENDTRY
	
	return result;
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        THROW(kODErrNoSysTranslationFacility);
	return result;
#endif // _PLATFORM_OS2_ or _PLATFORM_AIX_
}

//------------------------------------------------------------------------------
// ODTranslation: GetISOTypeFromPlatformType
//------------------------------------------------------------------------------

SOM_Scope ODValueType  SOMLINK ODTranslationGetISOTypeFromPlatformType(ODTranslation *somSelf, Environment *ev,
		ODPlatformType platformType,
		ODPlatformTypeSpace typeSpace)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationGetISOTypeFromPlatformType");

	ODValueType type = kODNULL; ODVolatile(type);
	
	SOM_TRY
	
		type = LocalGetISOTypeFromPlatformType(platformType, typeSpace);
		
	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return type;
}

//------------------------------------------------------------------------------
// ODTranslation: GetPlatformTypeFromISOType
//------------------------------------------------------------------------------

SOM_Scope ODPlatformType  SOMLINK ODTranslationGetPlatformTypeFromISOType(ODTranslation *somSelf, Environment *ev,
		ODValueType type)
{
    ODTranslationData *somThis = ODTranslationGetData(somSelf); 
    ODTranslationMethodDebug("ODTranslation","ODTranslationGetPlatformTypeFromISOType");
	
	SOM_TRY
		return LocalGetPlatformTypeFromISOType(type);
	SOM_CATCH_ALL
	SOM_ENDTRY
	
#ifdef _PLATFORM_MACINTOSH_
	return '    ';	// This is just used to pacify the compiler. This return result
					// should never be used by the caller.
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
	return (ODPlatformType) kODNULL;
#endif // _PLATFORM_OS2_ or _PLATFORM_AIX_
}

#ifdef _PLATFORM_MACINTOSH_
//=====================================================================================
// CommonTranslate
//=====================================================================================

OSErr CommonTranslate(ODTranslation* translation,
						Environment*	ev,
						ODValueType		fromType,
						ODByteArray*	fromDataBA,
						ODValueType		toType,
						ODHandle*		destHandle)
{
	// Set up params for Translation
	
	*destHandle = kODNULL;
	TranslationRefCon	refCon;
	
	TempODTypeList	fromTypeList = 
		ODTranslationGetData(translation)->fSession->GetStorageSystem(ev)->CreateTypeList(ev, kODNULL);
	fromTypeList->AddLast(ev, fromType);
	
	refCon.InitTranslationRefCon(fromTypeList,
									fromDataBA->_buffer,
									translation->GetPlatformTypeFromISOType(ev, fromType),
									(ODULong) fromDataBA->_length);
	
	OSType	destType = translation->GetPlatformTypeFromISOType(ev, toType);

	// Do the translation

//#ifdef __MWERKS__
	*destHandle = ODNewHandle(0);
	OSErr err = TranslateScrap((GetScrapDataUPP) &gGetScrapDataProcRD,
					&refCon,
					destType,
					(Handle) *destHandle,
					kTranslationScrapProgressDialogID);
	if (err != noErr) {
		ODDisposeHandle(*destHandle);
		*destHandle = kODNULL;
	}
	
	return err;

//#else

//	return noErr;

//#endif

}

#endif // _PLATFORM_MACINTOSH_
//=====================================================================================
// LocalGetISOTypeFromPlatformType
//=====================================================================================

ODType LocalGetISOTypeFromPlatformType(ODPlatformType platformType,
			ODPlatformTypeSpace typeSpace)
{
	ODType		isoType = kODNULL;
	ODULong	prefixSize;
#ifdef _PLATFORM_OS2_
	ODBoolean needPrefix = kODTrue;
#endif

	switch (typeSpace)
	{			 
		case kODPlatformDataType:
		
			prefixSize = ODISOStrLength(kODAppleScrapTypePrefix);
			isoType = (ODValueType) ODNewPtr(prefixSize + sizeof(ODPlatformType) + 1,
										ODGetDefaultHeap());
			if (isoType != kODNULL) {
#ifdef _PLATFORM_OS2_

				/* Figure out the four character suffix for this "Apple:XXXX" 		
				 * ISO type name, if this is one of the standard formats
				 */
				switch ( (UINT)platformType )
				{
					case CF_TEXT:
						platformType	= (ODPlatformType) PLFM_TEXT;
						break;					
					case CF_BITMAP:
						platformType	= (ODPlatformType) PLFM_BITMAP;
						break;					
					case CF_DSPTEXT:
						platformType	= (ODPlatformType) PLFM_DSPTEXT;
						break;					
					case CF_DSPBITMAP:
						platformType	= (ODPlatformType) PLFM_DSPBITMAP;
						break;					
					case CF_DSPMETAFILE:
						platformType	= (ODPlatformType) PLFM_DSPMETAFILE;
						break;					
					case CF_METAFILE:
						platformType	= (ODPlatformType) PLFM_METAFILE;
						break;					
					case CF_PALETTE:
						platformType	= (ODPlatformType) PLFM_PALETTE;
						break;					
                                        default: 
                                                HATOMTBL hAtomTable = WinQuerySystemAtomTable();
                                                ULONG theSize = WinQueryAtomLength(hAtomTable, platformType);
                                                if (theSize) {
                                                   needPrefix = kODFalse;
                                                   char* theAtomName = (char*) SOMMalloc(theSize + 1);
                                                   ULONG result = WinQueryAtomName(hAtomTable, platformType, theAtomName, theSize+1);
                                                   if(result)
                                                      ODISOStrCopy(isoType, theAtomName);
                                                   SOMFree((somToken) theAtomName);
                                                } /* endif */

				}
                                if(needPrefix) {
				  ODISOStrCopy(isoType, kODAppleScrapTypePrefix);
				  ODISOStrNConcat(isoType, (ODISOStr) &platformType, sizeof(ODPlatformType));
				  isoType[prefixSize + sizeof(ODPlatformType)] = 0;
                                }
#else
				ODISOStrCopy(isoType, kODAppleScrapTypePrefix);
				ODISOStrNConcat(isoType, (ODISOStr) &platformType, sizeof(ODPlatformType));
				isoType[prefixSize + sizeof(ODPlatformType)] = 0;
#endif // _PLATFORM_OS2_
			}
			break;
			
		case kODPlatformFileType:
			prefixSize = ODISOStrLength(kODAppleFileTypePrefix);
			isoType = (ODValueType) ODNewPtr(prefixSize + sizeof(ODPlatformType) + 1,
										ODGetDefaultHeap());
			if (isoType != kODNULL) {
				ODISOStrCopy(isoType, kODAppleFileTypePrefix);
				ODISOStrNConcat(isoType, (ODISOStr) &platformType, sizeof(ODPlatformType));
				isoType[prefixSize + sizeof(ODPlatformType)] = 0;
			}
			break;
						
		default:
			break;
	}
	
	return isoType;
}

//=====================================================================================
// LocalGetPlatformTypeFromISOType
//=====================================================================================

ODPlatformType LocalGetPlatformTypeFromISOType(ODType type)
{

	ODPlatformType	platformType = (ODPlatformType) 0L;
	ODULong			prefixSize;
	
	prefixSize = ODISOStrLength(kODAppleScrapTypePrefix);
	if (ODISOStrNCompare(kODAppleScrapTypePrefix, (ODISOStr) type, prefixSize)) 
	{
		prefixSize = ODISOStrLength(kODAppleFileTypePrefix);
		if (ODISOStrNCompare(kODAppleFileTypePrefix, (ODISOStr) type, prefixSize)) 
		{
			prefixSize = 0;
			platformType = kODNULL;
		}
	}
	if (prefixSize > 0)
	{
		ODISOStrNCopy((ODISOStr) &platformType,
						(ODISOStr) ((ODSByte*) type + prefixSize), 
						sizeof(ODPlatformType));
#ifdef _PLATFORM_OS2_
		if (platformType == (ODPlatformType) PLFM_TEXT)
			platformType	= (ODPlatformType) CF_TEXT;
		else if (platformType == (ODPlatformType) PLFM_DSPTEXT)
			platformType	= (ODPlatformType) CF_DSPTEXT;
		else if(platformType == (ODPlatformType) PLFM_BITMAP)
			platformType	= (ODPlatformType) CF_BITMAP;
		else if(platformType == (ODPlatformType) PLFM_DSPBITMAP)
			platformType	= (ODPlatformType) CF_DSPBITMAP;
		else if(platformType == (ODPlatformType) PLFM_METAFILE)
			platformType	= (ODPlatformType) CF_METAFILE;
		else if(platformType == (ODPlatformType) PLFM_DSPMETAFILE)
			platformType	= (ODPlatformType) CF_DSPMETAFILE;
		else if(platformType == (ODPlatformType) PLFM_PALETTE)
			platformType	= (ODPlatformType) CF_PALETTE;
		else {
                        HATOMTBL hAtomTable = WinQuerySystemAtomTable();
                        ATOM theAtom;
                        if(!(theAtom = WinFindAtom(hAtomTable, type)))
                           theAtom = WinAddAtom(hAtomTable, type);
                        platformType = theAtom;
                     }
        }
	else {
                HATOMTBL hAtomTable = WinQuerySystemAtomTable();
                ATOM theAtom;
                if(!(theAtom = WinFindAtom(hAtomTable, type)))
                   theAtom = WinAddAtom(hAtomTable, type);
                platformType = theAtom;
             }
#else
	}
#endif // _PLATFORM_OS2_
	return platformType;
}

#ifdef _PLATFORM_MACINTOSH_
//=====================================================================================
// GetScrapDataProc
//=====================================================================================
pascal OSErr GetScrapDataProc(ScrapType requestedFormat,
							Handle data,
							void* srcDataGetterRefCon)
{
#ifdef ODDebug_Translation
	somPrintf("Entering GetScrapDataProc.\n");
#endif

	Environment* 	ev = somGetGlobalEnvironment();
	
	ODHandle	dataHandle = (ODHandle) data;
	
	// Get the refCon and return the data for the requested type
	TranslationRefCon* refCon = (TranslationRefCon*) srcDataGetterRefCon;
	
	if (requestedFormat == kFormatListFormat) {
		ODTypeList*	typeList = refCon->fTypeList;
		ODULong		numOfTypes = typeList->Count(ev);

#ifdef ODDebug_Translation
	somPrintf("numOfTypes: %d\n", numOfTypes);
#endif

		ODSetHandleSize(dataHandle, numOfTypes * sizeof(FormatAvailable));
		
		FormatAvailable* dataPtr = (FormatAvailable*) ODLockHandle(dataHandle);
		ODTypeListIterator* iter = typeList->CreateTypeListIterator(ev);
		ODType isoType = iter->First(ev);
		for (ODULong i = 0; i < numOfTypes; i++) {
			OSType osType = LocalGetPlatformTypeFromISOType(isoType);
#ifdef ODDebug_Translation
	somPrintf("OSType %x ISOType %s\n", osType, isoType);
#endif
			ODBlockMove(&osType, &(dataPtr[i].theType), sizeof(OSType));
			dataPtr[i].theLength = 0;
			ODDisposePtr(isoType);
			isoType = iter->Next(ev);
		}
		ODDeleteObject(iter);
		ODUnlockHandle(dataHandle);
		
#ifdef ODDebug_Translation
		ODULong	hSize = GetHandleSize((Handle) dataHandle);
		ODULong	j;
		ODPtr		hDataPtr = ODLockHandle(dataHandle);
		somPrintf("DataHandle: ");
		for (j = 0; j < hSize; j++)
			somPrintf("%x ", ((char*) hDataPtr)[j]);
		somPrintf("\n");
		ODUnlockHandle(dataHandle);
#endif
	}
	else {
#ifdef ODDebug_Translation
		somPrintf("%x %s %d\n", refCon->fDataType, refCon->fDataPtr, refCon->fDataSize);
#endif
		ODSetHandleSize(dataHandle, refCon->fDataSize);
		ODPtr dataPtr = ODLockHandle(dataHandle);
		ODBlockMove(refCon->fDataPtr, dataPtr, refCon->fDataSize);
		ODUnlockHandle(dataHandle);
	}
	
#ifdef ODDebug_Translation
	somPrintf("Exiting GetScrapDataProc.\n");
#endif
	return noErr;
}

#endif // _PLATFORM_MACINTOSH_
