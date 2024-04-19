/* @(#)Z 1.21 com/src/bento/CMSU.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:21 (96/10/29 09:11:34) */
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
	File:		CMSU.cpp

	Contains:	Implementation CMStorageUnit class.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<53>	10/24/95	jpa		1293441: DM/VL: Bento memory reserve &
									fatal container err & don't throw in
									Release.
		<52>	10/19/95	EL		1292685: After purging we may get a
									different storage unit for the same
									CMObject.
		<51>	 10/8/95	TJ		Fixes Recomended by Refball
		<50>	 10/3/95	TJ		Changes done by RefBall Team
		<49>	 9/29/95	TJ		Made Changes for MAC SOM
		<48>	 8/22/95	VL		1276990: Removed bogus THROW in
									SetPromiseValue.
		<47>	 8/21/95	VL		1278330, 1278315: Error code cleanup.
		<46>	 8/17/95	VL		1266572: Added SOM_TRY and CATCH in
									CountValues and fixed THROW in GetDraft.
		<45>	 8/16/95	VL		1266572: Added SOM_TRY and CATCH in
									SetOffset and GetOffset.
		<44>	 8/12/95	T‚		1276812 Need to use TempObjs and TempRefs
									for exception safety and to avoid TRY
									blocks, 1276807 Opt./Bug: use StdTypIO
									routines for portable streaming & smaller
									footprint, 1276806 Optimization: use
									kODFalse instead of kODTrue in comparisons
		<43>	  8/8/95	EL		#1265282: GetStorageUnitRef does not check
									for null. #1266000: Focus cannot be called
									twice on the same value.
		<42>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<41>	 6/20/95	VL		Turned off debug code (accidentally left on
									in the previous checkin.
		<40>	 6/16/95	VL		1259613: Changed CreateCursorXXX to allow
									creation of cursor even when the su is
									unfocused. Used PreserveFocus.
		<39>	 6/13/95	VL		1241352: Added modification date support
									(mainly in SetChangedFromPrevFlag and
									Externalize).
		<38>	 5/26/95	VL		1251403: Multithreading naming support.
		<37>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<36>	 5/22/95	VL		1246940: Added more debug code.
		<35>	 5/19/95	VL		1249626: in length and out byteArray for
									GetValue and GetPromiseValue.
		<34>	 5/11/95	EL		1242515: value not released in
									GetObjectSize.
		<33>	  5/2/95	VL		1245113: RefCount check should not be used
									for invariants.
		<32>	 4/25/95	VL		1210982: Removed 5$.
		<31>	 4/24/95	VL		1241729: Fixed backward iteration of SU
									values.
		<30>	 4/11/95	VL		1236483: Fixed infinite cloning.
		<29>	 4/10/95	VL		1235948: Fixed crash for NULL input for
									GetStorageUnitRef. 1236490: Made
									GetPromiseValue work during FulfillPromise.
		<28>	 3/31/95	VL		1232945: Fixed leaking in CloneInto.
		<27>	 3/27/95	VL		1231412: Removed CopyTo.
		<26>	 3/19/95	VL		1228275: ASSERT if ODByteArray  is kODNULL.
		<25>	 3/14/95	VL		1225208,1225201: Added IsSettingPromise to
									ensure that we don't call ResolvePromise
									while setting a promise.
		<24>	 3/10/95	VL		1226116: No need to typecast for
									SetStorageUnitRef.
		<23>	  3/9/95	VL		1225493: CloneInto should preserve the
									focus. 1226082: Removed WASSERTM.
		<22>	 2/28/95	VL		1194656: Added checks for existing property
									and value for AddProperty and AddValue.
		<21>	 2/21/95	EL		1182275: Do not throw if object cannot be
									internalized (it probably was disposed in
									garbage collection).
		<20>	 1/31/95	EL		1213321: Speed up focus and exist by using
									CMUseValue.
		<19>	 1/26/95	VL		#???: SURef is now a 4-byte array.
		<18>	  1/9/95	VL		1183661: Use new SUCursor getters.
		<17>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		<16>	 11/1/94	CC		1190911 - CloneInto(): Updated comment.
		<15>	 11/1/94	VL		1179951: Implemented Remove for all
									Properties. 1194188: Implemented
									unimplemented functions.
		<14>	 10/6/94	VL		1185112: Lock should not throw if the
									storage unit is already locked.
		<13>	 9/29/94	RA		1189812: Mods for 68K build.
		<12>	 9/15/94	VL		1184871: In order to get cross-doc dad to
									work, I need to fix Focus to correctly
									focus to the value type first.
		<11>	  9/5/94	VL		1184871: Used Renew to remove dependency on
									default heap.
		<10>	  9/1/94	VL		1183174: Removed Clone and GetClonedIDs.
									1183054: Changed kODErrInvalidName to
									kODErrInvalidProperty.
		 <9>	 8/31/94	T‚		#1183129, #1183116, #1183119, #1183111:
									Lots of ErrorCode cleanup.
		 <8>	 8/31/94	VL		1183174, 1106013: #ifdef ODDebug somPrintf
									statements. Also, fixed GetName to return
									kODNULL if there is no name.
		 <7>	 8/26/94	VL		1183174: Implemented CloneInto and removed
									CloneTo.
		 <6>	 8/11/94	VL		1180299: Added CreateCursor and modified
									CreateView.
		 <5>	  8/3/94	VL		1153123: Storage to ODStor.
		 <4>	 7/26/94	VL		Got rid of helper methods for promise
									resolution. Instead, use PromiseResolver.
		 <3>	 7/21/94	VL		Added WASSERTM at various places to check
									for illegal sizes.
		 <2>	  7/7/94	VL		Fixed up local variables in Focus.
		 <1>	  7/5/94	VL		first checked in

	To Do:
	In Progress:
		
*/

#define CMStorageUnit_Class_Source
#define VARIABLE_MACROS
#include <CMSU.xih>

#ifndef SOM_CMStorageUnitRefIterator_xh
#include <CMSURefI.xh>
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef SOM_ODStorageUnitCursor_xh
#include <SUCursor.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef SOM_CMDraft_xh
#include <CMDraft.xh>
#endif

#ifndef SOM_CMDocument_xh
#include <CMDoc.xh>
#endif

#ifndef SOM_CMCtr_xh
#include <CMCtr.xh>
#endif

#ifndef _DRAFPRIV_
#include "DrafPriv.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _AEHSHTBL_
#include "AEHshTbl.h"
#endif
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef _PRMRSLVR_
#include "PrmRslvr.h"
#endif

#ifndef _SUREF_
#include "SURef.h"
#endif

#ifndef _SUCLONE_
#include "SUClone.h"
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef __STDIO__
#include <stdio.h>			// For sprintf
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef _TIME
#include <time.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif

#pragma segment CMStorageUnit

#define USE_CLONEHELPER 1

//==============================================================================
// Constants
//==============================================================================
#define kInitialHashTableEntries 8
#define kHighBitMask	0x80000000
#define kLowBitsMask	0x7FFFFFFF

#if ODDebug
// #define ODDebug_ODStorageUnit 1
#define ODDebug_AddProperty	1
#define ODDebug_AddValue	1
// #define ODDebug_SUCloneHelper	1
#endif

//==============================================================================
// Function Prototype
//==============================================================================

static ODStorageUnitView* NewODStorageUnitView(ODMemoryHeapID heapID);
static ODStorageUnitCursor* NewODStorageUnitCursor(ODMemoryHeapID heapID);
static CMStorageUnitRefIterator*	NewCMStorageUnitRefIterator(ODMemoryHeapID heapID);

#define FailIfInvalidRefCount() ASSERTMSG(somSelf->GetRefCount(ev)!=0, kODErrZeroRefCount, "Invalid refCount", 0)
#define FailIfIllegalByteArray(value) ASSERT(value != kODNULL, kODErrIllegalNullInput)

inline void BREAK_AND_THROW(ODError err)
{
//	Debugger();
	THROW(err);
}

#define ODEnterCriticalSection()
#define ODExitCriticalSection()

// Callers must call ODSessionMustHaveCMAllocReserve(container);
static ODULong GetPropertySize(CMObject object, CMProperty property);
static ODULong GetObjectSize(CMObject object);

//==============================================================================
// CMStorageUnit
//==============================================================================

//------------------------------------------------------------------------------
// CMStorageUnit: AcquireDraft
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMStorageUnitGetDraft(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetDraft");
	
	return _fDraft;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Acquire
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitAcquire(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Acquire");

    try
    {

	parent_Acquire(somSelf, ev);
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }

}
	
//------------------------------------------------------------------------------
// CMStorageUnit: Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitRelease(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Release");

    try
    {

	CMStorageUnit_parent_ODStorageUnit_Release(somSelf, ev);
	if (somSelf->GetRefCount(ev) == 0) {
		if (_fCurValue != kODNULL) {
			CMContainer cmContainer = _fDraft->GetCMContainer(ev);
			ODSessionMustHaveCMAllocReserve(cmContainer);
			
			CMReleaseValue(_fCurValue);
			
			_fCurValue = kODNULL;
		}
		_fCurProperty = kODNULL;
		_fDraft->ReleaseStorageUnit(ev, _fID);
	}
		
    }
    catch (ODException _exception)
    {
	
		ODError err = ErrorCode();

		WARNMSG(WARN_INDEX(AMSG_670),
			"Error occurred in ODStorageUnit::Release: %d %s", 
			err, ErrorMessage());

		if (err == kODErrBentoErr){
			SetErrorCode(kODErrFatalContainerError);
                        ODSetSOMException(ev, _exception);
                }
		else{
                     if (err != kODErrFatalContainerError)
			SetErrorCode(kODNoError);
                     else
                        ODSetSOMException(ev, _exception);
                }
			
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize			 SOMLINK CMStorageUnitPurge(CMStorageUnit *somSelf, Environment *ev,
		ODSize size)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Purge");
     
        ODULong runningTotal = 0; ODVolatile( runningTotal ); // DR5 code added

    try
    {
	
	ODBoolean hasReserve = kODFalse;
	
	if (_fCurValue != kODNULL) {
		CMContainer cmContainer = _fDraft->GetCMContainer(ev);
		ODSessionMustHaveCMAllocReserve(cmContainer);
		hasReserve = kODTrue;
		
		CMReleaseValue(_fCurValue);
		_fCurValue = kODNULL;
	}
	
	_fCurProperty = kODNULL;
	
	if (_fObject != kODNULL) {
	
                CMDraft* tempDraft = (CMDraft*) somSelf->GetDraft(ev);
		IDList*	idList = kODNULL;
                if(tempDraft)
                    idList = tempDraft->GetIDList(ev);
		ASSERT(idList != kODNULL, kODErrInvalidIDList);
		
		if (idList->Exists(_fID) == kODFalse) {
			if ( !hasReserve )
			{
				CMContainer cmContainer = _fDraft->GetCMContainer(ev);
				ODSessionMustHaveCMAllocReserve(cmContainer);
				hasReserve = kODTrue;
			}
			CMReleaseObject(_fObject);
			_fObject = kODNULL;
		}
	}
        runningTotal += parent_Purge(somSelf, ev, size);  // DR5 code added
	
    }
    catch (ODException _exception)
    {
         WARNMSG(WARN_INDEX(AMSG_906),"Error %ld trying to purge in CMStorageUnitPurge",ErrorCode()); // DR5 code added.
             // dh - Eat the exception; Purge should not 
             // propagate it because clients function 
             // fine whether memory was purged or not.
         SetErrorCode(kODNoError);   // DR5 code added
         runningTotal = 0;   // DR5 code added
    }
	return runningTotal;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Exists
//------------------------------------------------------------------------------

SOM_Scope ODBoolean	 SOMLINK CMStorageUnitExists(CMStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName,
									ODValueType valueType, 
									ODValueIndex valueIndex)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Exists");

	ODBoolean		valueFound = kODFalse;

    try
    {
	CMContainer		container = somSelf->GetCMContainer(ev);
	ODBoolean		propertyFound = kODFalse;
	CMProperty		targetProperty;
	CMProperty		property;
	CMType			targetType;
	CMValue			value;
	CMValue			tmpValue;
	ODValueIndex	index;

	FailIfInvalidRefCount();
	somSelf->Internalize(ev);
		
	if (_fObject == kODNULL)
		valueFound =  kODFalse;

	ODSessionMustHaveCMAllocReserve(container);

	if (propertyName != kODNULL) {
		targetProperty = CMRegisterProperty(container, propertyName);
		if (targetProperty == kODNULL)
			BREAK_AND_THROW(kODErrIllegalPropertyName);
	}
	else
		targetProperty = _fCurProperty;
	
	if (valueType != kODNULL) {
		targetType = CMRegisterType(container, valueType);
		if (targetType == kODNULL)
			BREAK_AND_THROW(kODErrInvalidValueType);
			
		value = CMUseValue(_fObject, targetProperty, targetType);
		if (value != kODNULL) {
			CMReleaseValue(value);
			valueFound = kODTrue;
		}
	}
	else if (valueIndex > 0) {
		index = 1;
		value = CMGetNextValue(_fObject, targetProperty, kODNULL);
		while ((value != kODNULL) && (index < valueIndex)) {
			index++;
			tmpValue = value;
			value = CMGetNextValue(_fObject, targetProperty, tmpValue);
			CMReleaseValue(tmpValue);
		}
		if (value != kODNULL) {
			CMReleaseValue(value);
			if (index == valueIndex)
				valueFound = kODTrue;
		}
	}
	else {
		if (propertyName != kODNULL) {
			property = CMGetNextObjectProperty(_fObject, kODNULL);
			while ((property != kODNULL) && (valueFound == kODFalse)) {
				if (targetProperty == property)
					valueFound = kODTrue;
				property = CMGetNextObjectProperty(_fObject, property);
			}
		}
		else
			valueFound = kODTrue;
	}
	ODSessionRestoreCMAllocReserve(container);


    }
    catch (ODException _exception)
    {
           valueFound  = kODFalse;
           ODSetSOMException(ev, _exception);
    }
	return	valueFound;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Exists
//------------------------------------------------------------------------------

SOM_Scope ODBoolean	 SOMLINK CMStorageUnitExistsWithCursor(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitCursor* cursor)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","ExistsWithCursor");

	ODBoolean		exists = kODFalse;
	ODPropertyName propertyName = kODNULL;
	ODValueType	valueType = kODNULL;

    try
    {
	ODValueIndex	valueIndex;

	FailIfInvalidRefCount();
	somSelf->Internalize(ev);
	
	cursor->GetProperty(ev, &propertyName);
	cursor->GetValueType(ev, &valueType);
	cursor->GetValueIndex(ev, &valueIndex);

	exists = somSelf->Exists(ev, propertyName, valueType, valueIndex);
	
	ODDisposePtr(propertyName);
	ODDisposePtr(valueType);


    }
    catch (ODException _exception)
    {
      if(propertyName)
        ODDisposePtr(propertyName);
      if(valueType)
        ODDisposePtr(valueType);
	exists  = kODFalse;
        ODSetSOMException(ev, _exception);
    }
	return exists;
}

//------------------------------------------------------------------------------
// CMStorageUnit: CountProperties
//------------------------------------------------------------------------------

SOM_Scope ODULong	 SOMLINK CMStorageUnitCountProperties(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CountProperties");
          
        ODULong CountProps = 0;
	
    try
    {
	FailIfInvalidRefCount();
	somSelf->Internalize(ev);

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	CountProps =  CMCountProperties(_fObject, kODNULL);

    }
    catch(ODException _exception)
    {
           CountProps =  0;
           ODSetSOMException(ev, _exception);
    }
        return CountProps;
}

//------------------------------------------------------------------------------
// CMStorageUnit: CountValues
//------------------------------------------------------------------------------

SOM_Scope ODULong	 SOMLINK CMStorageUnitCountValues(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CountValues");
	
	ODULong numValues = 0;
	
	try
        {
	
		FailIfInvalidRefCount();
		somSelf->Internalize(ev);
		
		if (_fCurProperty == kODNULL)
			THROW(kODErrUnfocusedStorageUnit);
	
		CMContainer container = _fDraft->GetCMContainer(ev);
		ODSessionMustHaveCMAllocReserve(container);
		
		numValues = CMCountValues(_fObject, _fCurProperty, kODNULL);
		
        }
        catch (ODException _exception)
        {
            numValues = 0;
            ODSetSOMException(ev, _exception);
        }
	
	return numValues;
}
		
//------------------------------------------------------------------------------
// CMStorageUnit: Focus
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*	 SOMLINK CMStorageUnitFocus(CMStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName,
									ODPositionCode propertyPosCode,
									ODValueType valueType,
									ODValueIndex valueIndex,
									ODPositionCode	valuePosCode)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Focus");

	
    try
    {
	FailIfInvalidRefCount();
	somSelf->Internalize(ev);
	
	CMProperty	property = _fCurProperty;
	CMValue		value = _fCurValue;

                                                                    // FEB-13
#ifdef _PLATFORM_OS2_                                               //116316
        ODBoolean       fReleaseValue = kODTrue;                    //116316
#endif                                                              //116316
	
	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	if (propertyName != kODNULL) {
		property = CMRegisterProperty(container, propertyName);
		if (property == kODNULL) {
#ifdef ODDebug_ODStorageUnit
	somPrintf("Error in Focus: ID %x PID %x propertyName %s\n", somSelf->GetID(ev), _fObjectID, propertyName);
#endif
			somSelf->CleanupAndFail(ev, kODErrIllegalPropertyName);
		}
	}
	else {
		_fHasPropertyLooped = propertyPosCode & kODPosMWrap;
		switch (propertyPosCode) {
			case kODPosUndefined:
				somSelf->CleanupAndFail(ev, kODErrUnsupportedPosCode);
				break;
			case kODPosSame:
                                                                    // FEB-13
#ifdef _PLATFORM_OS2_                                               //116316
                                fReleaseValue = kODFalse;           //116316
#endif                                                              //116316
				break;
			case kODPosAll:
				property = kODNULL;
				break;
			case kODPosFirstSib:
				property = CMGetNextObjectProperty(_fObject, kODNULL);
				if (property == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrPropertyDoesNotExist);
				break;
			case kODPosLastSib:
				property = CMGetPrevObjectProperty(_fObject, kODNULL);
				if (property == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrPropertyDoesNotExist);
				break;
			case kODPosNextSib:
				property = CMGetNextObjectProperty(_fObject, property);
				if (property == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrPropertyDoesNotExist);
				break;
			case kODPosPrevSib:
				property = CMGetPrevObjectProperty(_fObject, property);
				if (property == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrPropertyDoesNotExist);
				break;
			case kODPosFirstBelow:
			case kODPosLastBelow:
			case kODPosFirstAbove:
			case kODPosLastAbove:
			default:
				somSelf->CleanupAndFail(ev, kODErrUnsupportedPosCode);
				break;
		}
	}
	
	ODBoolean needReleaseValue = kODTrue;

	if (valueType != kODNULL) {
		CMType type = kODNULL;
		
		CMType targetType = CMRegisterType(container, valueType);
		
		value = CMUseValue(_fObject, property, targetType);
		if (value == kODNULL)
			somSelf->CleanupAndFail(ev, kODErrSUValueDoesNotExist);
			
	} else if (valueIndex > 0) {
		ODULong		i = 1;
		CMValue		tmpValue;

		value = CMGetNextValue(_fObject, property, kODNULL);
		while ((value != kODNULL) && (i < valueIndex)) {
			tmpValue = value;
			value = CMGetNextValue(_fObject, property, tmpValue);
			CMReleaseValue(tmpValue);
			i++;
		}		
		if (value == kODNULL)
			somSelf->CleanupAndFail(ev, kODErrValueIndexOutOfRange);
	}
	else {
		_fHasValueLooped = valuePosCode & kODPosMWrap;
		switch (valuePosCode) {
			case kODPosUndefined:
				value = kODNULL;
			break;
			case kODPosAll:
				value = kODNULL;
				break;
			case kODPosFirstSib:
				value = CMGetNextValue(_fObject, property, kODNULL);
				if (value == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrSUValueDoesNotExist);
				break;
			case kODPosNextSib:
				value = CMGetNextValue(_fObject, property, value);
				if (value == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrSUValueDoesNotExist);
				break;
			case kODPosSame:
				needReleaseValue = kODFalse;
				break;
			case kODPosLastSib:
				value = CMGetPrevValue(_fObject, property, kODNULL);
				if (value == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrSUValueDoesNotExist);
				break;
			case kODPosPrevSib:
				value = CMGetPrevValue(_fObject, property, value);
				if (value == kODNULL)
					somSelf->CleanupAndFail(ev, kODErrSUValueDoesNotExist);
				break;
			case kODPosFirstBelow:
			case kODPosLastBelow:
			case kODPosFirstAbove:
			case kODPosLastAbove:
			default:
				somSelf->CleanupAndFail(ev, kODErrUnsupportedPosCode);
				break;
		}
	}
	
                                                                    // FEB-13
#ifdef _PLATFORM_OS2_                                               //116316
      if (fReleaseValue == kODTrue )                                //116316
#else
	if (needReleaseValue != kODFalse) 
#endif
		if (_fCurValue != kODNULL)
			CMReleaseValue(_fCurValue);
	
	_fOffset = 0;
	_fCurValue = value;
	_fCurProperty = property;

	ODSessionRestoreCMAllocReserve(container);
	

    }
    catch (ODException _exception)
    {
       ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Focus
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*	 SOMLINK CMStorageUnitFocusWithCursor(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitCursor* suCursor)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","FocusWithCursor");
	
	ODPropertyName	propertyName = kODNULL;
	ODValueType	valueType    = kODNULL;
	
    try
    {
	ODValueIndex	valueIndex;
	
	FailIfInvalidRefCount();
	somSelf->Internalize(ev);

	if (suCursor == kODNULL)
		BREAK_AND_THROW(kODErrIllegalNullSUCursorInput);

	suCursor->GetProperty(ev, &propertyName);
	suCursor->GetValueType(ev, &valueType);
	suCursor->GetValueIndex(ev, &valueIndex);
	
	somSelf->Focus(ev, propertyName,
				kODPosAll,
				valueType,
				valueIndex,
				kODPosAll);
				
	if (propertyName != kODNULL)
		ODDisposePtr(propertyName);
		
	if (valueType != kODNULL)
		ODDisposePtr(valueType);
	

    }
    catch (ODException _exception)
    {
      if (propertyName != kODNULL)
	ODDisposePtr(propertyName);

      if (valueType != kODNULL)
	ODDisposePtr(valueType);

        ODSetSOMException(ev, _exception);
    }
   
	return somSelf;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Externalize
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*	 SOMLINK CMStorageUnitExternalize(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Externalize");


   try
   {
	PreserveFocus	originalFocus(ev, somSelf);

	if (_fDirty) {	
		somSelf->ResolveAllPromises(ev);
	
		if (_fSURefKeeper != kODNULL)
			_fSURefKeeper->Externalize();
		
		if (somSelf->Exists(ev, kODPropModDate, kODTime_T, 0)) {
			ODSetTime_TProp(ev, somSelf, kODPropModDate, kODTime_T, (ODTime) _fModificationDate);
		}
		
		_fDirty = kODFalse;
	}

     }
     catch (ODException _exception)
     {
         ODSetSOMException(ev, _exception);
     }
	return somSelf;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Internalize
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*	 SOMLINK CMStorageUnitInternalize(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Internalize");

		
    try
    {
	if (_fObject == kODNULL) {

                CMDraft* tempDraft = (CMDraft*) somSelf->GetDraft(ev);
		IDList*	idList = kODNULL;
                if(tempDraft)
                     idList =  tempDraft->GetIDList(ev);
		ASSERT(idList != kODNULL, kODErrInvalidIDList);
				
		if (idList->Exists(_fID) != kODFalse) {			
			_fObject = (CMObject) idList->Get(_fID);
		}
		else if (_fObjectID != kODNULL) {
			CMContainer container = _fDraft->GetCMContainer(ev);
			ODSessionMustHaveCMAllocReserve(container);
			
			_fObject = CMGetObject(container, _fObjectID);

			ODSessionRestoreCMAllocReserve(container);
			
//			if (_fObject == kODNULL)
//				BREAK_AND_THROW(kODErrBentoCannotNewObject);
			if (_fObject != kODNULL)
				idList->Add(_fID, _fObject);
		}
		else
			BREAK_AND_THROW(kODErrInvalidStorageUnit);
	}
		

     }
     catch( ODException _exception)
     {
          ODSetSOMException(ev, _exception);
     }

	return somSelf;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetID
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitID  SOMLINK CMStorageUnitGetID(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetID");

	// Do not call fail if RefCount is zero because GetID can be called
	// from Remove where RefCount MUST be zero -pjh
	// FailIfInvalidRefCount();
	
	return _fID;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetName
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitName  SOMLINK CMStorageUnitGetName(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetName");
	
        ODStorageUnitName ODSUName = kODNULL;
    try
    {
	
	FailIfInvalidRefCount();
	ODSUName = ODGetISOStrProp(ev, somSelf, kODPropStorageUnitName, kODISOStr, kODNULL, kODNULL);

   }
   catch (ODException _exception)
   {
	ODSUName = kODNULL;
        ODSetSOMException(ev, _exception);
   }
        return ODSUName;
}
	
//------------------------------------------------------------------------------
// CMStorageUnit: SetName
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetName(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitName name)
{
    // CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","SetName");

			
   try
   {
	FailIfInvalidRefCount();
		
	ODSetISOStrProp(ev, somSelf, kODPropStorageUnitName, kODISOStr, name);

   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMStorageUnit: AddProperty
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*	 SOMLINK CMStorageUnitAddProperty(CMStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName)	
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","AddProperty");

   try
   {

#ifdef ODDebug_ODStorageUnit
	somPrintf("AddProperty %x %x %s\n", _fID, _fObjectID, propertyName);
#endif
	
	CMContainer container = somSelf->GetCMContainer(ev);	
	CMProperty	property = kODNULL;
	
	FailIfInvalidRefCount();
	
	somSelf->Internalize(ev);
	
	if (propertyName == kODNULL)
		BREAK_AND_THROW(kODErrIllegalNullPropertyInput);
	if (propertyName != kODNULL) {

		ODSessionMustHaveCMAllocReserve(container);
		
		property = CMRegisterProperty(container,propertyName);
		if (property == kODNULL) {
			somSelf->CleanupAndFail(ev, kODErrCannotAddProperty);
		}
		else {
		
#if ODDebug_AddProperty
			CMProperty tmpProp = kODNULL;
			do {
				ODSessionMustHaveCMAllocReserve(container); // safest once per loop
				tmpProp = CMGetNextObjectProperty(_fObject, tmpProp);
			} while ((tmpProp != kODNULL) && (tmpProp != property));
			
			if (tmpProp == property) {
				WARNMSG(WARN_INDEX(AMSG_210),"Property exists already.");
				somPrintf("propertyName %s exists already.\n", propertyName);
			}
#endif

			if (_fCurValue != kODNULL) {
				CMReleaseValue(_fCurValue);
				_fCurValue = kODIDAll;
			}
			_fCurProperty = property;
			somSelf->SetChangedFromPrevFlag(ev, kODTrue);
		}
		ODSessionRestoreCMAllocReserve(container);
	}

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

	
//------------------------------------------------------------------------------
// CMStorageUnit: AddValue
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK CMStorageUnitAddValue(CMStorageUnit *somSelf, Environment *ev,
		ODValueType type)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","AddValue");


    try
    {
	CMContainer	container = somSelf->GetCMContainer(ev);
	char*		dummyBuffer = "";
	CMValue		value;
	
	FailIfInvalidRefCount();
	somSelf->SetChangedFromPrevFlag(ev, kODTrue);
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	if (type == kODNULL)
		BREAK_AND_THROW(kODErrIllegalNullValueTypeInput);
	else {
		ODSessionMustHaveCMAllocReserve(container);

		_fCurType = CMRegisterType(container, type);
		if (_fCurType == kODNULL)
			BREAK_AND_THROW(kODErrInvalidValueType);
		value = CMUseValue(_fObject, _fCurProperty, _fCurType);
		if (value == kODNULL) {
			value = CMNewValue(_fObject, _fCurProperty, _fCurType);
			if (value == kODNULL)
				somSelf->CleanupAndFail(ev, kODErrBentoCannotNewValue);
			CMWriteValueData(value, dummyBuffer, 0, 0);
		}
#if ODDebug_AddValue
		else {
			WARNMSG(WARN_INDEX(AMSG_220),"Value exists already.");
			CMGlobalName propName = CMGetGlobalName(_fCurProperty);
			somPrintf("Property %s Value %s exists already.\n", propName, type);
		}
#endif
		if (_fCurValue != kODNULL)
			CMReleaseValue(_fCurValue);
		_fCurValue = value;
		_fOffset = 0;

		ODSessionRestoreCMAllocReserve(container);
	}

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Remove
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*	 SOMLINK CMStorageUnitRemove(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Remove");


   try
   {
	FailIfInvalidRefCount();
	somSelf->SetChangedFromPrevFlag(ev, kODTrue);

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);

	if (_fCurValue != kODNULL) {
		if ((_fPromiseResolver != kODNULL) && (_fPromiseResolver->IsSettingPromise() == kODFalse))
			_fPromiseResolver->ClearPromise(ev);
		CMDeleteValue(_fCurValue);
		_fCurValue = kODNULL;
	}
	else if (_fCurProperty != kODNULL) {
		CMDeleteObjectProperty(_fObject, _fCurProperty);
		_fCurProperty = kODNULL;
	}
	else {
		CMProperty curProperty = CMGetNextObjectProperty(_fObject, kODNULL);
		while (curProperty != kODNULL) {
			CMGlobalName name = CMGetGlobalName(curProperty);
			CMProperty property = curProperty;
			curProperty = CMGetNextObjectProperty(_fObject, curProperty);
			if (ODISOStrNCompare(name, kODBentoPrefix, ODISOStrLength(kODBentoPrefix)) != 0) {
				CMDeleteObjectProperty(_fObject, property);
			}
		}
	}
	ODSessionRestoreCMAllocReserve(container);
	

    }
    catch (ODException _exception)
    {
       ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

//------------------------------------------------------------------------------
// CMStorageUnit: CloneInto
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitCloneInto(CMStorageUnit *somSelf, Environment *ev,
		ODDraftKey key, ODStorageUnit* toSU, ODID scopeID)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CloneInto");
	ODPropertyName	propertyName;
	CMGlobalName	typeName;

    try
    {

#ifdef USE_CLONEHELPER
	if (_fSUCloneHelper->ShouldClone(key, scopeID) != kODFalse) {
#endif
#ifdef ODDebug_SUCloneHelper
		somPrintf("Cloning %d in scope %d\n", somSelf->GetID(ev), scopeID);
#endif

#ifdef ODDebug_ODStorageUnit
	somPrintf("CMSU CloneInto: from %x %x to %x %x scope %x\n",
		_fID, _fObjectID, toSU->GetID(ev), ((CMStorageUnit*) toSU)->GetObjectID(ev), scopeID);
#endif
	
	if (((CMDraft*) somSelf->GetDraft(ev))->IsValidDraftKey(ev, key) == kODFalse)
		THROW(kODErrInvalidDraftKey);

	FailIfInvalidRefCount();
		
	ODULong			numProperties;
	ODULong			numValues;
	ODULong			i;
	ODULong			j;
	ODULong			tmpSize;
	ODULong			size;
	ODPtr			buffer;
	ODStorageUnitCursor*	originalFocus = kODNULL;
	
	somSelf->Internalize(ev);

	if (_fCurProperty != kODNULL)
		originalFocus = somSelf->CreateCursorWithFocus(ev);
		
	somSelf->Focus(ev, (ODPropertyName) kODNULL, 
							kODPosAll,
							kODTypeAll,
							0,
							kODPosUndefined);
							
	numProperties = somSelf->CountProperties(ev);
	
	for (i = 0; i < numProperties; i++) {
	
		somSelf->Focus(ev, (ODPropertyName) kODNULL,
						kODPosNextSib,
						kODTypeAll,
						0,
						kODPosUndefined);
	
		propertyName = somSelf->GetProperty(ev);
		
#if ODDebug_AddProperty
		if (toSU->Exists(ev, propertyName, kODNULL, 0) != kODFalse) {
			toSU->Focus(ev, propertyName, kODPosUndefined, kODNULL, 0, kODPosUndefined);
		}
		else
			toSU->AddProperty(ev, propertyName);
#else
		toSU->AddProperty(ev, propertyName);
#endif
	
		numValues = somSelf->CountValues(ev);	
		
		for (j = 0; j < numValues; j++) {

			somSelf->Focus(ev, (ODPropertyName) kODNULL,
							kODPosSame,
							kODNULL,
							0,
							kODPosNextSib);

			typeName = somSelf->GetType(ev);
			
			// Do no overrite existing values except for the storage unit type property.
			// New storage units are created with a generic storage unit type which should
			// be overwritten.  The link iterators currently use the storage unit type
			// property to identify link and linkSource storage units.
			if ((ODISOStrCompare(propertyName, (const ODISOStr) kODPropStorageUnitType) == 0)
				|| (toSU->Exists(ev, propertyName, typeName, 0) == kODFalse)) { 

#ifdef ODDebug_ODStorageUnit			
				somPrintf("Copying from %x %x %s %s to %x %x\n", 
					_fID, _fObjectID, propertyName, typeName, toSU->GetID(ev), ((CMStorageUnit*) toSU)->GetObjectID(ev));
#endif

#if ODDebug_AddValue
				if (toSU->Exists(ev, propertyName, typeName, 0) != kODFalse) {
					toSU->Focus(ev, propertyName, kODPosUndefined, typeName, 0, kODPosUndefined);
				}
				else
					toSU->AddValue(ev, typeName);
#else				
				toSU->AddValue(ev, typeName);
#endif
				
				tmpSize = toSU->GetSize(ev);
				toSU->DeleteValue(ev, tmpSize);
				
				PreserveFocus* focus = new PreserveFocus(ev, somSelf);
				size = somSelf->GetSize(ev);
				delete focus;
				numValues = somSelf->CountValues(ev);
				numProperties = somSelf->CountProperties(ev);
				
#ifdef _PLATFORM_MACINTOSH_
				buffer = ODNewPtr(size, somSelf->GetHeap(ev));
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//				The use of heap is not implemented in Windows
//				platform
				buffer = ODNewPtr(size);
#endif
				StorageUnitGetValue(somSelf, ev, size, (ODValue) buffer);
				StorageUnitSetValue(toSU, ev, size, (ODValue) buffer);
				ODDisposePtr(buffer);
			}
			else {
				toSU->Focus(ev, propertyName,
							kODPosUndefined,
							typeName,
							0,
							kODPosUndefined);
			}
			
			ODStorageUnitRefIterator*	iter;
			ODStorageUnitRef			ref;

			iter = somSelf->CreateStorageUnitRefIterator(ev);				
				for(iter->First(ev, ref); iter->IsNotComplete(ev) != kODFalse; iter->Next(ev, ref)) {
                                ODBoolean ValidRefFound = kODFalse;
                                try{
                                  ValidRefFound = somSelf->IsValidStorageUnitRef(ev, ref);
                                }
                                catch(ODException _exception){
                                  somExceptionFree(ev);  SOM_InitEnvironment(ev);
                                  SetErrorCode(kODNoError);
                                }
                                
				if (ValidRefFound) {				
					ODStorageUnitID			toEmbeddedID = 0;
					ODStorageUnitID			containingFrameID = 0;
					ODBoolean				strongClone = somSelf->IsStrongStorageUnitRef(ev, ref);
					ODStorageUnitID			fromEmbeddedID = somSelf->GetIDFromStorageUnitRef(ev, ref);
	
// Do *not* change the value of scopeID, because this will
// interfere with the cloning of subsequent values and
// properties. (1371928)
                                               ODID subScopeID = scopeID; // possible new scope
                                               if (subScopeID != 0) {

#ifdef _NO_TMP_OBJS_
						ODStorageUnit *fromEmbeddedSU = _fDraft->AcquireStorageUnit(ev, fromEmbeddedID);
#else
						TempODStorageUnit fromEmbeddedSU = _fDraft->AcquireStorageUnit(ev, fromEmbeddedID);
#endif
						if ((containingFrameID = ODGetWeakSURefProp(ev, fromEmbeddedSU, kODPropContainingFrame, kODWeakStorageUnitRef))
							!= kODNULLID)
						{								
							if (containingFrameID == subScopeID)
								subScopeID = fromEmbeddedID;
							else
								strongClone = kODFalse;
						}
#ifdef _NO_TMP_OBJS_
						fromEmbeddedSU->Release(ev);
#endif
					}
					
					if (strongClone != kODFalse) {
#ifdef ODDebug_ODStorageUnit
						somPrintf("{{{ Strong Clone %x Begins\n", fromEmbeddedID);
#endif
							toEmbeddedID = _fDraft->Clone(ev, key, fromEmbeddedID, 0, subScopeID);
#ifdef ODDebug_ODStorageUnit
						somPrintf("}}} Strong Clone %x Ends %x\n", fromEmbeddedID, toEmbeddedID);
#endif
					}
					else {
#ifdef ODDebug_ODStorageUnit
						somPrintf("Begin Weak Clone from %x\n", fromEmbeddedID);
#endif
							toEmbeddedID = _fDraft->WeakClone(ev, key, fromEmbeddedID, 0, subScopeID);
#ifdef ODDebug_ODStorageUnit
						somPrintf("End Weak Clone from %x %x to %x %x\n", fromEmbeddedID, toEmbeddedID);
#endif
					}
					if (toEmbeddedID != 0) {
	
#ifdef ODDebug_ODStorageUnit
	ODPropertyName	tmpPropertyName = somSelf->GetProperty(ev);
	ODValueType	tmpTypeName = somSelf->GetType(ev);
	ODPropertyName	tmpToPropertyName = toSU->GetProperty(ev);
	ODValueType	tmpToTypeName = toSU->GetType(ev);
	somPrintf("SetStorageUnitRef: FromID %x %s %s to toID %x %s %s\n",
												fromEmbeddedID,
												tmpPropertyName,
												tmpTypeName,
												toEmbeddedID,
												tmpToPropertyName,
												tmpToTypeName);
	ODDisposePtr(tmpPropertyName);
	ODDisposePtr(tmpTypeName);
	ODDisposePtr(tmpToPropertyName);
	ODDisposePtr(tmpToTypeName);
#endif
	
						toSU->SetStorageUnitRef(ev, toEmbeddedID, ref);
					}
				}
			}
			delete iter;

			ODDisposePtr(typeName);
		}
		ODDisposePtr(propertyName);
	}
	
	if (originalFocus == kODNULL)
		somSelf->Focus(ev, kODNULL, kODPosAll, kODNULL, 0, kODPosAll);
	else {
		somSelf->FocusWithCursor(ev, originalFocus);
		delete originalFocus;
	}
#ifdef USE_CLONEHELPER
	}
#endif
#ifdef USE_CLONEHELPER
#ifdef ODDebug_SUCloneHelper
	else
		somPrintf("Not cloning %d in scope %d\n", somSelf->GetID(ev), scopeID);
#endif
#endif

    }
    catch (ODException _exception)
    {
      if(propertyName)
         ODDisposePtr(propertyName);
      if(typeName)
         ODDisposePtr(typeName);   
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: CreateView
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitView*	 SOMLINK CMStorageUnitCreateView(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CreateView");
    
	ODStorageUnitView* suView = kODNULL;

   try
   {

	FailIfInvalidRefCount();
	
	ODStorageUnitCursor* cursor = somSelf->CreateCursorWithFocus(ev);
	
	suView = NewODStorageUnitView(somSelf->GetHeap(ev));
	suView->InitStorageUnitView(ev, somSelf, cursor);


    }
    catch (ODException _exception)
    {
	suView = kODNULL;
        ODSetSOMException(ev, _exception);
    }
	return suView;
}

//------------------------------------------------------------------------------
// CMStorageUnit: CreateCursor
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitCursor*  SOMLINK CMStorageUnitCreateCursor(CMStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName,
		ODValueType valueType,
		ODValueIndex valueIndex)
{
	/*	CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf); */
    CMStorageUnitMethodDebug("CMStorageUnit","CMStorageUnitCreateCursor");
	
        ODStorageUnitCursor* suCursor = kODNULL;
	
   try
   {
	suCursor = NewODStorageUnitCursor(somSelf->GetHeap(ev));
	suCursor->InitStorageUnitCursor(ev, propertyName, valueType, valueIndex);
	

    }
    catch (ODException _exception)
    {
	suCursor =  kODNULL;
        ODSetSOMException(ev, _exception);
    }
	return suCursor;
}

//------------------------------------------------------------------------------
// CMStorageUnit: CreateCursorWithFocus
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitCursor*  SOMLINK CMStorageUnitCreateCursorWithFocus(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CreateCursorWithFocus");

	ODStorageUnitCursor* 	suCursor = kODNULL;

   try
   {
	ODPropertyName			propertyName = kODNULL;
	ODValueType			valueType = kODNULL;

	FailIfInvalidRefCount();
	somSelf->Internalize(ev);

	if (_fCurProperty != kODNULL)
		propertyName = somSelf->GetProperty(ev);
	
	if (_fCurValue != kODNULL)
		valueType = somSelf->GetType(ev);

	suCursor = NewODStorageUnitCursor(somSelf->GetHeap(ev));
	suCursor->InitStorageUnitCursor(ev, propertyName, valueType, _fCurValueIndex);
	
	ODDisposePtr(propertyName);
	ODDisposePtr(valueType);
						

    }
    catch (ODException _exception)
    {
	suCursor = kODNULL;
        ODSetSOMException(ev, _exception);
    }
	return suCursor;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetProperty
//------------------------------------------------------------------------------

SOM_Scope ODPropertyName  SOMLINK CMStorageUnitGetProperty(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetProperty");

	ODPropertyName	propertyName = kODNULL;
	
    try
    {
	CMGlobalName	tmp;

	FailIfInvalidRefCount();

	if (_fCurProperty == kODNULL)
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	
	// CMContainer container = _fDraft->GetCMContainer(ev);
	// ODSessionMustHaveCMAllocReserve(container);
	// CMIsProperty() and CMGetGlobalName() do not allocate memory.

	if (CMIsProperty(_fCurProperty)) {
		tmp = CMGetGlobalName(_fCurProperty);
		propertyName = ODISOStrFromCStr(tmp);
	}
	// ODSessionRestoreCMAllocReserve(container);
	

    }
    catch (ODException _exception)
    {
	propertyName = kODNULL;
        ODSetSOMException(ev, _exception);
    }
	return (propertyName);
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetType
//------------------------------------------------------------------------------

SOM_Scope ODValueType  SOMLINK CMStorageUnitGetType(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetType");

	ODValueType	typeName = kODNULL;

    try
    {
	CMType			type;
	CMGlobalName	tmp;

	FailIfInvalidRefCount();

	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	
	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);

	CMGetValueInfo(_fCurValue, kODNULL, kODNULL, kODNULL, &type, kODNULL);
	
	if (CMIsType(type)) {
		tmp = CMGetGlobalName(type);
		typeName = ODISOStrFromCStr(tmp);
	}
	ODSessionRestoreCMAllocReserve(container);
	

    }
    catch (ODException _exception)
    {
	typeName =  kODNULL;
        ODSetSOMException(ev, _exception);
    }
	return (typeName);
}

//------------------------------------------------------------------------------
// CMStorageUnit: SetType
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetType(CMStorageUnit *somSelf, Environment *ev,
		ODValueType typeName)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","SetType");


    try
    {
        if(typeName == kODNULL)                // #25523 KP
             BREAK_AND_THROW(kODErrInvalidValueType);

	CMContainer container = _fDraft->GetCMContainer(ev);
	CMType		type;

	FailIfInvalidRefCount();
	somSelf->SetChangedFromPrevFlag(ev, kODTrue);
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
		
	// CMContainer container = somSelf->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	type = CMRegisterType(container, (CMGlobalName) typeName);
	if (type != kODNULL)
		CMSetValueType(_fCurValue, type);
	else
		BREAK_AND_THROW(kODErrInvalidValueType);

	ODSessionRestoreCMAllocReserve(container);

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: SetOffset
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetOffset(CMStorageUnit *somSelf, Environment *ev,
		ODULong offset)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","SetOffset");

      try
      {
		FailIfInvalidRefCount();
		
		if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
			THROW(kODErrUnfocusedStorageUnit);
	
		_fOffset = offset;
	
      }
      catch (ODException _exception)
      {
          ODSetSOMException(ev, _exception);
      }
}

SOM_Scope ODULong  SOMLINK CMStorageUnitGetOffset(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetOffset");

	ODULong offset = 0;
	
       try
       {
		FailIfInvalidRefCount();
		
		if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
			THROW(kODErrUnfocusedStorageUnit);
			
		offset = _fOffset;
       }
       catch (ODException _exception)
       {
           ODSetSOMException(ev, _exception);
       }		
	
	return offset;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetValue
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK CMStorageUnitGetValue(CMStorageUnit *somSelf, Environment *ev,
		ODULong length,
		ODByteArray* value)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetValue");

   try
   {
	
	FailIfIllegalByteArray(value);
	FailIfInvalidRefCount();
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	
	if ((_fPromiseResolver != kODNULL) && (_fPromiseResolver->IsSettingPromise() == kODFalse))
		_fPromiseResolver->ResolvePromise(ev);

	value->_buffer = (octet*) ODNewPtr(length);
	value->_maximum = length;
	
	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);

	value->_length = CMReadValueData(_fCurValue, (CMPtr) value->_buffer, _fOffset, length);

	ODSessionRestoreCMAllocReserve(container);

	_fOffset += value->_length;


     }
     catch (ODException _exception)
     {
	value->_length =  0;
        ODSetSOMException(ev, _exception);
     }
	return value->_length;
}

//------------------------------------------------------------------------------
// CMStorageUnit: SetValue
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetValue(CMStorageUnit *somSelf, Environment *ev,
		ODByteArray* value)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","SetValue");

    try
    {
	
	FailIfIllegalByteArray(value);
	FailIfInvalidRefCount();
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);

	if ((_fPromiseResolver != kODNULL) && (_fPromiseResolver->IsSettingPromise() == kODFalse))
		_fPromiseResolver->ResolvePromise(ev);

	somSelf->SetChangedFromPrevFlag(ev, kODTrue);
	
	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);

	CMWriteValueData(_fCurValue, (CMPtr) value->_buffer, _fOffset, value->_length);

	ODSessionRestoreCMAllocReserve(container);

	_fOffset += value->_length;

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: InsertValue
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitInsertValue(CMStorageUnit *somSelf, Environment *ev,
		ODByteArray* value)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","InsertValue");

   try
   {
	FailIfIllegalByteArray(value);
	FailIfInvalidRefCount();
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);

	if ((_fPromiseResolver != kODNULL) && (_fPromiseResolver->IsSettingPromise() == kODFalse))
		_fPromiseResolver->ResolvePromise(ev);

	somSelf->SetChangedFromPrevFlag(ev, kODTrue);

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	CMInsertValueData(_fCurValue,(CMPtr)  value->_buffer, _fOffset, value->_length);

	ODSessionRestoreCMAllocReserve(container);

	_fOffset += value->_length;

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: DeleteValue
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitDeleteValue(CMStorageUnit *somSelf, Environment *ev,
		ODULong length)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","DeleteValue");

   try
   {
	FailIfInvalidRefCount();
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);

	if ((_fPromiseResolver != kODNULL) && (_fPromiseResolver->IsSettingPromise() == kODFalse))
		_fPromiseResolver->ResolvePromise(ev);

	somSelf->SetChangedFromPrevFlag(ev, kODTrue);

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	CMDeleteValueData(_fCurValue, _fOffset, length);

	ODSessionRestoreCMAllocReserve(container);

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetSize
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK CMStorageUnitGetSize(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetSize");

	ODULong	size = 0;
	
   try
   {
	FailIfInvalidRefCount();
	somSelf->Internalize(ev);

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	// GetPropertySize() and GetObjectSize() call CM methods
		
	if (_fCurValue != kODNULL) {
		if ((_fPromiseResolver != kODNULL) && (_fPromiseResolver->IsSettingPromise() == kODFalse))
			_fPromiseResolver->ResolvePromise(ev);
		
		size = CMGetValueSize(_fCurValue);
	}
	else if (_fCurProperty != kODNULL)
		size = GetPropertySize(_fObject, _fCurProperty);
	else
		size = GetObjectSize(_fObject);

	ODSessionRestoreCMAllocReserve(container);
		

     }
     catch (ODException _exception)
     {
	size =  0; 
        ODSetSOMException(ev, _exception);
     }
	return size;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetStrongStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitGetStrongStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID, ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetStrongStorageUnitRef");

    try
    {
	
	somSelf->GetStorageUnitRef(ev, embeddedSUID, kODTrue, ref);

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// CMStorageUnit: GetWeakStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitGetWeakStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID, ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetWeakStorageUnitRef");

    try
    {
	
	somSelf->GetStorageUnitRef(ev, embeddedSUID, kODFalse, ref);

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: IsValidStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMStorageUnitIsValidStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","IsValidStorageUnitRef");

	ODBoolean validSU = kODFalse;

   try
   {
	FailIfInvalidRefCount();
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	
	CMObject	object = kODNULL;
	ODVolatile(object);
#ifdef PLATFORM_MACINTOSH
	TRY
#endif // PLATFORM_MACINTOSH
		CMContainer container = _fDraft->GetCMContainer(ev);
		ODSessionMustHaveCMAllocReserve(container);
		
		object = CMGetReferencedObject(_fCurValue, ref);
		if (object != kODNULL)
			CMReleaseObject(object);
			
		ODSessionRestoreCMAllocReserve(container);

#ifdef PLATFORM_MACINTOSH
	CATCH_ALL
		object = kODNULL;
	ENDTRY
#endif // PLATFORM_MACINTOSH
	
	if(object != kODNULL)
             validSU = kODTrue;

    }
    catch (ODException _exception)
    {
         validSU = kODFalse;	
         ODSetSOMException(ev, _exception);
    }
        return validSU;
}

//------------------------------------------------------------------------------
// CMStorageUnit: IsStrongStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMStorageUnitIsStrongStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","IsStrongStorageUnitRef");

	ODBoolean isStrongSUref = kODFalse;

   try
   {

	isStrongSUref =  _fSURefKeeper->IsStrongSURef(ref);
   }
   catch (ODException _exception)
   {
         isStrongSUref = kODFalse;
         ODSetSOMException(ev, _exception);
   }
   return isStrongSUref;
}

//------------------------------------------------------------------------------
// CMStorageUnit: IsWeakStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMStorageUnitIsWeakStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","IsWeakStorageUnitRef");

	ODBoolean isWeakSUref = kODFalse;

   try
   {

	isWeakSUref =  _fSURefKeeper->IsWeakSURef(ref);
   }
   catch (ODException _exception)
   {
         isWeakSUref = kODFalse;
         ODSetSOMException(ev, _exception);
   }
   return isWeakSUref;
}

//------------------------------------------------------------------------------
// CMStorageUnit: RemoveStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK CMStorageUnitRemoveStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","RemoveStorageUnitRef");

    try
    {
	FailIfInvalidRefCount();

	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);

	somSelf->SetChangedFromPrevFlag(ev, kODTrue);

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
		
	CMDeleteReference(_fCurValue, ref);
					
	ODSessionRestoreCMAllocReserve(container);
	

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	return somSelf;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetIDFromStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitID  SOMLINK CMStorageUnitGetIDFromStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetIDFromStorageUnitRef");

	ODID	id = 0;
   try
   {
	FailIfInvalidRefCount();
	
	CMObject	object;
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	try
        {
		object = CMGetReferencedObject(_fCurValue, ref);
        }
	catch (ODException _exception)
        {
		BREAK_AND_THROW(kODErrInvalidStorageUnitRef);
	}
	
	if (object == kODNULL)
		BREAK_AND_THROW(kODErrInvalidStorageUnitRef);
		
        CMDraft* tempDraft = (CMDraft*) somSelf->GetDraft(ev);
	IDList*	idList = kODNULL;
        if(tempDraft)
            idList = tempDraft->GetIDList(ev);
	ASSERT(idList != kODNULL, kODErrInvalidIDList);

	if (idList->ObjectExists(object) != kODFalse) {
		id = idList->GetID(object);
		CMReleaseObject(object);
	}
	else {
		id = idList->Add(object);
	}
	ODSessionRestoreCMAllocReserve(container);
	

    }
    catch (ODException _exception)
    {
        id = 0;
        ODSetSOMException(ev, _exception);
    } 
	return id;
}

//------------------------------------------------------------------------------
// CMStorageUnit: CreateStorageUnitRefIterator
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitRefIterator*  SOMLINK CMStorageUnitCreateStorageUnitRefIterator(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CrateStorageUnitRefIterator");

        CMStorageUnitRefIterator* iter = kODNULL;
   try
   {
	FailIfInvalidRefCount();

	iter = NewCMStorageUnitRefIterator(somSelf->GetHeap(ev));
	iter->InitStorageUnitRefIterator(ev, somSelf);


   }
   catch (ODException _exception)
   {
	iter =  kODNULL;
        ODSetSOMException(ev, _exception);
   }
	return iter;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetGenerationNumber
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK CMStorageUnitGetGenerationNumber(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetGenerationNumber");

	ODULong	generation = 0;
   try
   {
	FailIfInvalidRefCount();
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
		

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);

	CMGetValueInfo(_fCurValue, kODNULL, kODNULL, kODNULL, kODNULL, &generation);
	
	ODSessionRestoreCMAllocReserve(container);


    }
    catch (ODException _exception)
    {
	generation =  0;
        ODSetSOMException(ev, _exception);
    }
	return generation;
}

//------------------------------------------------------------------------------
// CMStorageUnit: IncrementGenerationNumber
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK CMStorageUnitIncrementGenerationNumber(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","IncrementGenerationNumber");

        ODULong generation = 0;
    try
    {
	FailIfInvalidRefCount();
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);

	somSelf->SetChangedFromPrevFlag(ev, kODTrue);

	generation = somSelf->GetGenerationNumber(ev);
	generation++;

	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);

	CMSetValueGeneration(_fCurValue, generation);

	ODSessionRestoreCMAllocReserve(container);
	

    }
    catch (ODException _exception)
    {
	generation =  0;
        ODSetSOMException(ev, _exception);
    }
	return generation;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetSession
//------------------------------------------------------------------------------

SOM_Scope ODSession*	 SOMLINK CMStorageUnitGetSession(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetSession");

        ODSession* session = kODNULL;

    try
    {
	session =  (ODSession*) _fDraft->GetDocument(ev)->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev);

    }
    catch (ODException _exception)
    {
	session = kODNULL;
        ODSetSOMException(ev, _exception);
    }
        return session;
}

//------------------------------------------------------------------------------
// CMStorageUnit: InitStorageUnit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitInitStorageUnit(CMStorageUnit *somSelf, Environment *ev,
		ODDraft* draft,ODStorageUnitID suid)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","InitStorageUnit");
	
   try
   {
			
	/* Moved from somInit. SOM itself sets fields to zero
	_fDraft = kODNULL;
	_fID = 0;
	_fObjectID = kODNULL;
	_fObject = kODNULL;
	_fCurProperty = kODIDAll;
	_fCurValueIndex = 0;
	_fCurValue = kODIDAll;
	_fCurValueType = 0;
	_fCurType = kODNULL;
	_fOffset = 0;
	_fHasPropertyLooped = kODFalse;
	_fHasValueLooped = kODFalse;
	
	_fCurrentKey = 0;
	_fLockCount = 0;
	
	_fSURefKeeper = kODNULL;
	
#ifdef USE_CLONEHELPER
	_fSUCloneHelper = kODNULL;
#endif
	
	_fHeap = kDefaultHeapID;
	
	_fModificationDate = 0;
	_fDirty = kODFalse;
	*/

#ifdef _PLATFORM_OS2_ //<crs> - implement mutex sem under os/2                               //D133590
  //<crs> - Changing the meaning of _fCurrentKey. It is now going to be                      //D133590
  //        an HMTX (it's a ODUlong so there's no problem there). We will                    //D133590
  //        no longer maintain _fLockCount.                                                  //D133590
  // Create an unowned, unnamed shared mutex semaphore                                       //D133590
  APIRET rc = DosCreateMutexSem( 0, &(_fCurrentKey), DC_SEM_SHARED, 0 );                     //D133590
#endif //<crs> - end                                                                         //D133590

	_fCurProperty = kODIDAll;
	_fCurValue = kODIDAll;
	_fHeap = kDefaultHeapID;
	
	somSelf->InitRefCntObject(ev);
	
	
	if (draft != kODNULL) {
	
		_fDraft = (CMDraft*) draft;
		_fID = suid;
	
		IDList*	idList = kODNULL;
			
		idList = _fDraft->GetIDList(ev);
		ASSERT(idList != kODNULL, kODErrInvalidIDList);
	
		_fObject = (CMObject) idList->Get(_fID);
		if (_fObject == kODNULL)
			BREAK_AND_THROW(kODErrBentoInvalidObject);
		
		_fCMContainer = _fDraft->GetCMContainer(ev);
			
		_fObjectID = CMGetObjectID(_fObject);
		
		_fHeap = _fDraft->GetHeap(ev);

#ifdef _PLATFORM_MACINTOSH_
		_fPromiseResolver = new(_fHeap) PromiseResolver;
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		_fPromiseResolver = new PromiseResolver;
#endif
		_fPromiseResolver->InitPromiseResolver(somSelf);
		
#ifdef _PLATFORM_MACINTOSH_
		_fSURefKeeper = new(_fHeap) SURefKeeper(somSelf);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		_fSURefKeeper = new SURefKeeper(somSelf);
#endif


#ifdef USE_CLONEHELPER		
#ifdef _PLATFORM_MACINTOSH_
		_fSUCloneHelper = new(_fHeap) SUCloneHelper;
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		_fSUCloneHelper = new SUCloneHelper;
#endif
#endif
	}
	else
		THROW(kODErrIllegalNullDraftInput);

    }
    catch (ODException _exception)
    {
         ODDeleteObject(_fSUCloneHelper);
         ODDeleteObject(_fPromiseResolver);
         ODDeleteObject(_fSURefKeeper);
         ODSetSOMException(ev, _exception);
    }
}


//------------------------------------------------------------------------------
// CMStorageUnit: ~CMStorageUnit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitsomUninit(CMStorageUnit *somSelf)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","somUninit");

        Environment* ev = somGetGlobalEnvironment();
    try
    {
	delete _fPromiseResolver;
	delete _fSURefKeeper;
	
#ifdef USE_CLONEHELPER		
	delete _fSUCloneHelper;
#endif	

#ifdef _PLATFORM_OS2_ //<crs> - implement mutex sem under os/2                                  //D133590
    APIRET rc = DosCloseMutexSem( _fCurrentKey );                                               //D133590
#endif //<crs> - end                                                                            //D133590

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	
    CMStorageUnit_parents_somUninit(somSelf);

}

//------------------------------------------------------------------------------
// CMStorageUnit: CleanupAndFail
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitCleanupAndFail(CMStorageUnit *somSelf, Environment *ev,
		ODError err)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CleanupAndFail");

    try
    {
	
#ifdef ODDebug_ODStorageUnit
	somPrintf("Cleanup and fail: ID %x PID %x\n", somSelf->GetID(ev), _fObjectID);
#endif

	if (_fCurValue != kODNULL) {
		CMContainer container = _fDraft->GetCMContainer(ev);
		ODSessionMustHaveCMAllocReserve(container);
		
		CMReleaseValue(_fCurValue);
		
		ODSessionRestoreCMAllocReserve(container);
		
		_fCurValue = kODNULL;
	}
	if (err) {
		BREAK_AND_THROW(err);
	}

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: Lock
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnitKey  SOMLINK CMStorageUnitLock(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitKey key)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Lock");
#ifdef _PLATFORM_OS2_

  // Note: under os/2 we wouldn't want to call DosEnterCritSec() here as            //D133590
  //       we will be making api calls...                                           //D133590
                                                                                    //D133590
  TRY                                                                               //D133590
                                                                                    //D133590
     if ( key != 0 && key != _fCurrentKey )                                         //D133590
        {                                                                           //D133590
        THROW(kODErrInvalidStorageUnitKey);                                         //D133590
        }                                                                           //D133590
                                                                                    //D133590
     APIRET rc = DosOpenMutexSem( 0, &(_fCurrentKey) );                             //D133590
     if ( rc == NO_ERROR )                                                          //D133590
        rc = DosRequestMutexSem( _fCurrentKey, SEM_INDEFINITE_WAIT );               //D133590
                                                                                    //D133590
     key = ( rc == NO_ERROR ) ? _fCurrentKey : 0;                                   //D133590
                                                                                    //D133590
 CATCH_ALL                                                                          //D133590
                                                                                    //D133590
    RERAISE;                                                                        //D133590
                                                                                    //D133590
  ENDTRY                                                                            //D133590
#else
	
	ODEnterCriticalSection();
	
	if (key == 0) {
		if (_fLockCount == 0) {
			_fLockCount = 1;
			_fCurrentKey++;
			key = _fCurrentKey;
		}
	}
	else {
		if ((_fCurrentKey != key) || (_fLockCount == 0))
			ODSetSOMException(ev,kODErrInvalidStorageUnitKey);
		else
			_fLockCount++;
	}
		
	ODExitCriticalSection();
	
#endif

	return key;
}

//------------------------------------------------------------------------------
// CMStorageUnit: Unlock
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitUnlock(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitKey key)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","Unlock");

#ifdef _PLATFORM_OS2_

  PTIB  pptib;                                                                      //D133590
  PPIB  pppib;                                                                      //D133590
  PID   pid;                                                                        //D133590
  TID   tid;                                                                        //D133590
  ULONG ulCount;                                                                    //D133590
                                                                                    //D133590
  TRY                                                                               //D133590
                                                                                    //D133590
     if ( key != _fCurrentKey )                                                     //D133590
        {                                                                           //D133590
        THROW(kODErrInvalidStorageUnitKey);                                         //D133590
        }                                                                           //D133590
                                                                                    //D133590
     APIRET rc = DosQueryMutexSem( _fCurrentKey, &pid, &tid, &ulCount );            //D133590
     if ( rc != NO_ERROR )                                                          //D133590
        {                                                                           //D133590
        THROW(kODErrInvalidStorageUnitKey);                                         //D133590
        }                                                                           //D133590
                                                                                    //D133590
     DosGetInfoBlocks( &pptib, &pppib );                                            //D133590
     if ( (pid != pppib->pib_ulpid) ||                                              //D133590
          (tid != pptib->tib_ptib2->tib2_ultid) )                                   //D133590
        {                                                                           //D133590
        THROW(kODErrInvalidStorageUnitKey);                                         //D133590
        }                                                                           //D133590
                                                                                    //D133590
     if ( ulCount == 0 )                                                            //D133590
        {                                                                           //D133590
        THROW(kODErrStorageUnitNotLocked);                                          //D133590
        }                                                                           //D133590
                                                                                    //D133590
     rc = DosReleaseMutexSem( _fCurrentKey );                                       //D133590
     if ( rc == NO_ERROR )                                                          //D133590
        rc = DosCloseMutexSem( _fCurrentKey );                                      //D133590
                                                                                    //D133590
                                                                                    //D133590
  CATCH_ALL                                                                         //D133590
                                                                                    //D133590
    RERAISE;                                                                        //D133590
                                                                                    //D133590
  ENDTRY                                                                            //D133590
#else
	
	ODEnterCriticalSection();
	
	if (_fCurrentKey != key)
		ODSetSOMException(ev,kODErrInvalidStorageUnitKey);
	
	else if (_fLockCount == 0)
		ODSetSOMException(ev,kODErrStorageUnitNotLocked);
	
	else
		--_fLockCount;
		
	ODExitCriticalSection();

#endif
}


//------------------------------------------------------------------------------
// CMStorageUnit: GetStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitGetStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID, ODBoolean strong, ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetStorageUnitRef");

    try
    {
	FailIfInvalidRefCount();

	if (embeddedSUID == kODNULLID)
		THROW(kODErrIllegalNullStorageUnitInput);
		
	// Clear incoming reference
	_fSURefKeeper->InvalidateSURef(ref);

	CMReference			theReferenceData;	
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
		
	CMStorageUnit* embeddedSU = (CMStorageUnit*) somSelf->GetDraft(ev)->AcquireStorageUnit(ev, embeddedSUID);
#ifdef _NO_TMP_OBJS_
	ODStorageUnit *tempSU = embeddedSU; // ensure it's released
#else
	TempODStorageUnit tempSU = embeddedSU; // ensure it's released
#endif
	
	if (embeddedSU) {
		CMObject object = embeddedSU->GetObject(ev);
		if (object) {	
			CMContainer container = _fDraft->GetCMContainer(ev);
			ODSessionMustHaveCMAllocReserve(container);
			
			if (CMGetReferenceForObject(_fCurValue, object, theReferenceData) != kODNULL) {
				// we already have this object in the reference, reuse the id			
				ODBlockMove(theReferenceData, ref, sizeof(CMReference));
				// if strong, value must be strong, else value must be weak
				if ((strong != kODFalse) ? somSelf->IsWeakStorageUnitRef(ev, ref) : somSelf->IsStrongStorageUnitRef(ev, ref))
					_fSURefKeeper->InvalidateSURef(ref);
			}
			
			if (somSelf->IsValidStorageUnitRef(ev, ref) == kODFalse) {
				_fSURefKeeper->GetNextSURef(ref, strong);
//				ODBlockMove(ref, theReferenceData, sizeof(CMReference));
			}
			
//			CMSetReference(_fCurValue, embeddedSU->GetObject(ev), theReferenceData);
			CMSetReference(_fCurValue, object, ref);

			ODSessionRestoreCMAllocReserve(container);
		}
	}
#ifdef _NO_TMP_OBJS_
	tempSU->Release(ev);
#endif

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   } 

}

//------------------------------------------------------------------------------
// CMStorageUnit: SetStorageUnitRef
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetStorageUnitRef(CMStorageUnit *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID, ODStorageUnitRef ref)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","SetStorageUnitRef");

   try
   {
	FailIfInvalidRefCount();

	CMReference			theReferenceData;
	
	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL) || (_fCurValue == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	
	if (embeddedSUID == 0)
		THROW(kODErrIllegalNullIDInput);
		
	CMStorageUnit* embeddedSU = (CMStorageUnit*) somSelf->GetDraft(ev)->AcquireStorageUnit(ev, embeddedSUID);
#ifdef _NO_TMP_OBJS_
	ODStorageUnit *tempSU = embeddedSU;
#else
	TempODStorageUnit tempSU = embeddedSU;
#endif
	
	somSelf->SetChangedFromPrevFlag(ev, kODTrue);
	
	ODBlockMove(ref, theReferenceData, sizeof(CMReference));
	
	if (_fSURefKeeper != kODNULL) {
		_fSURefKeeper->Reset(ref);
	}
	
	CMContainer container = _fDraft->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	CMSetReference(_fCurValue, embeddedSU->GetObject(ev), theReferenceData);

	ODSessionRestoreCMAllocReserve(container);
#ifdef _NO_TMP_OBJS_
	tempSU->Release(ev);
#endif

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetHeap
//------------------------------------------------------------------------------

SOM_Scope ODMemoryHeapID  SOMLINK CMStorageUnitGetHeap(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetHeap");

	return _fHeap;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetCMContainer
//------------------------------------------------------------------------------

SOM_Scope CMContainer  SOMLINK CMStorageUnitGetCMContainer(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetCMContainer");

        CMContainer container = kODNULL;
   try
   {

      if(_fDraft)
	container =  _fDraft->GetCMContainer(ev);

   }
   catch (ODException _exception)
   {
	container =  kODNULL;
        ODSetSOMException(ev, _exception);
   }
        return container;
}

//------------------------------------------------------------------------------
// CMStorageUnit: SetChangedFromPrevFlag
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetChangedFromPrevFlag(CMStorageUnit *somSelf, Environment *ev,
		ODBoolean changed)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","SetChangedFromPrevFlag");

   try
   {

	if (_fDraft != kODNULL)
		_fDraft->SetChangedFromPrevFlag(ev, changed);

	time((time_t *)(&_fModificationDate));
	
	_fDirty = kODTrue;

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetObjectID
//------------------------------------------------------------------------------

SOM_Scope ODID  SOMLINK CMStorageUnitGetObjectID(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetObjectID");

	return (ODID) _fObjectID;
}

//------------------------------------------------------------------------------
// GetPropertySize
//------------------------------------------------------------------------------

// Callers must call ODSessionMustHaveCMAllocReserve(container);

static ODULong GetPropertySize(CMObject object, CMProperty property)
{
	ODULong	size = 0;
        Environment* ev = somGetGlobalEnvironment();
   try
   {

	ODULong	numValues = CMCountValues(object, property, kODNULL);
	CMValue		curValue = kODNULL;
	
	for (ODULong i = 0; i < numValues; i++) {
		curValue = CMGetNextValue(object, property, curValue);
		if (curValue != kODNULL) {
			size += CMGetValueSize(curValue);
			CMReleaseValue(curValue);
		}
	}
    }
    catch (ODException _exception)
    {
         size = 0;
         ODSetSOMException(ev, _exception);
    }
	return size;
}

//------------------------------------------------------------------------------
// GetObjectSize
//------------------------------------------------------------------------------

// Callers must call ODSessionMustHaveCMAllocReserve(container);

static ODULong GetObjectSize(CMObject object)
{
	ODULong	size = 0;
        Environment* ev = somGetGlobalEnvironment();
   try
   {
	ODULong	numProperties = CMCountProperties(object, kODNULL);
	CMProperty	curProperty = kODNULL;
	
	for (ODULong i = 0; i < numProperties; i++) {
		curProperty = CMGetNextObjectProperty(object, curProperty);
		size += GetPropertySize(object, curProperty);
	}
    }
    catch (ODException _exception)
    {
         size = 0;
         ODSetSOMException(ev, _exception);
    }
	return size;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetCurValue
//------------------------------------------------------------------------------

SOM_Scope CMValue  SOMLINK CMStorageUnitGetCurValue(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetCurValue");

	return _fCurValue;
}

//------------------------------------------------------------------------------
// CMStorageUnit: SetCurValue
//------------------------------------------------------------------------------
//
//SOM_Scope void  SOMLINK CMStorageUnitSetCurValue(CMStorageUnit *somSelf, Environment *ev, CMValue curValue)
//{
//    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
//    CMStorageUnitMethodDebug("CMStorageUnit","SetCurValue");
//
//	_fCurValue = curValue;
//}

//------------------------------------------------------------------------------
// CMStorageUnit: GetObject
//------------------------------------------------------------------------------

SOM_Scope CMObject  SOMLINK CMStorageUnitGetObject(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetCurValue");

	return _fObject;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetCurProperty
//------------------------------------------------------------------------------

SOM_Scope CMObject  SOMLINK CMStorageUnitGetCurProperty(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetCurProperty");

	return _fCurProperty;
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetCurType
//------------------------------------------------------------------------------

SOM_Scope CMType  SOMLINK CMStorageUnitGetCurType(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","GetCurType");

	return _fCurType;
}

//------------------------------------------------------------------------------
// CMStorageUnit: SetCurType
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetCurType(CMStorageUnit *somSelf, Environment *ev, CMType curType)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","SetCurType");

	_fCurType = curType;
}

//------------------------------------------------------------------------------
// CMStorageUnit: IsPromiseValue
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMStorageUnitIsPromiseValue(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CMStorageUnitIsPromiseValue");

        ODBoolean valueFound = kODFalse;

   try
   {
	valueFound =  _fPromiseResolver->IsPromiseValue(ev);

   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
	valueFound =  kODFalse;
   }
        return valueFound;
}

//------------------------------------------------------------------------------
// CMStorageUnit: SetPromiseValue
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitSetPromiseValue(CMStorageUnit *somSelf, Environment *ev,
		ODValueType valueType,
		ODULong offset,
		ODByteArray* value,
		ODPart* sourcePart)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CMStorageUnitSetPromiseValue");

   try
   {
	FailIfIllegalByteArray(value);
	FailIfInvalidRefCount();

	if ((_fObject == kODNULL) || (_fCurProperty == kODNULL))
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	
	if (_fPromiseResolver != kODNULL) {
		_fPromiseResolver->SettingPromise();

		try
                {
#if ODDebug_AddValue
			ODPropertyName propertyName = somSelf->GetProperty(ev);
			if (somSelf->Exists(ev, propertyName, valueType, 0) != kODFalse) {
				somSelf->Focus(ev, propertyName, kODPosUndefined, valueType, 0, kODPosUndefined);
			}
			else
				somSelf->AddValue(ev, valueType);
			ODDisposePtr(propertyName);
#else				
			somSelf->AddValue(ev, valueType);
#endif
			somSelf->SetChangedFromPrevFlag(ev, kODTrue);
			
			somSelf->SetOffset(ev, offset);
			somSelf->SetValue(ev, value);
			
			_fPromiseResolver->SetSourcePart(ev, sourcePart);
			
			_fPromiseResolver->DoneSettingPromise();
                }
		catch (ODException _exception)
                {
			_fPromiseResolver->DoneSettingPromise();
			throw;
		}
	}

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: GetPromiseValue
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK CMStorageUnitGetPromiseValue(CMStorageUnit *somSelf, Environment *ev,
		ODValueType valueType,
		ODULong offset,
		ODULong length,
		ODByteArray* value,
		ODPart** sourcePart)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CMStorageUnitGetPromiseValue");

	 value->_length = 0;

   try
   {
	FailIfIllegalByteArray(value);
	FailIfInvalidRefCount();

	somSelf->Focus(ev, kODNULL, kODPosSame, valueType, 0, kODPosSame);
	
	value->_buffer = (octet*) ODNewPtr(length);
	value->_maximum = length;
	value->_length = CMReadValueData(_fCurValue, (CMPtr) value->_buffer, offset, length);
	
	if ((_fPromiseResolver != kODNULL) && (sourcePart != kODNULL))
		*sourcePart = _fPromiseResolver->GetSourcePart(ev);
	

     }
     catch (ODException _exception)
     {
       if(value->_buffer)
         ODDisposePtr(value->_buffer);
	 value->_length = 0;
         ODSetSOMException(ev, _exception);
     }    
	return value->_length;
}

//------------------------------------------------------------------------------
// CMStorageUnit: ClearAllPromises
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitClearAllPromises(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CMStorageUnitClearAllPromises");

	ODBoolean	ret = kODFalse;
   try
   {
	FailIfInvalidRefCount();
	
	if (_fPromiseResolver->GetPromiseCount())
	{
		somSelf->Focus(ev,
						(ODPropertyName) kODNULL, 
						kODPosAll,
						kODTypeAll,
						0,
						kODPosUndefined);
		ODULong numProperties = somSelf->CountProperties(ev);
		for (ODULong i = 0; i < numProperties; i++)
		{
			somSelf->Focus(ev,
						(ODPropertyName) kODNULL,
						kODPosNextSib,
						kODTypeAll,
						0,
						kODPosUndefined);
			ODULong numValues = somSelf->CountValues(ev);	
			for (ODULong j = 0; j < numValues; j++)
			{
				somSelf->Focus(ev,
							(ODPropertyName)kODNULL,
							kODPosSame,
							kODTypeAll,
							0,
							kODPosNextSib);
				_fPromiseResolver->ClearPromise(ev);
				if (_fPromiseResolver->GetPromiseCount() == 0)
				{
					ret = kODTrue;
					break;
				}
			}
			if (ret)
				break;
		}
	}

    }
    catch (ODException _exception)
    {
	ret = kODFalse;
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnit: ResolveAllPromises
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitResolveAllPromises(CMStorageUnit *somSelf, Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
    CMStorageUnitMethodDebug("CMStorageUnit","CMStorageUnitResolveAllPromises");

	ODBoolean	ret = kODFalse;
    try
    {

	if (_fPromiseResolver && _fPromiseResolver->GetPromiseCount())
	{
		somSelf->Focus(ev, (ODPropertyName)kODNULL, 
								kODPosAll,
								kODTypeAll,
								0,
								kODPosUndefined);
		ODULong numProperties = somSelf->CountProperties(ev);
		for (ODULong i = 0; i < numProperties; i++)
		{
			somSelf->Focus(ev, (ODPropertyName)kODNULL,
						kODPosNextSib,
						kODTypeAll,
						0,
						kODPosUndefined);
			ODULong numValues = somSelf->CountValues(ev);	
			for (ODULong j = 0; j < numValues; j++)
			{
				somSelf->Focus(ev, (ODPropertyName)kODNULL,
							kODPosSame,
							kODTypeAll,
							0,
							kODPosNextSib);
				PreserveFocus* focus = new PreserveFocus(ev, somSelf);
				_fPromiseResolver->ResolvePromise(ev);
				delete focus;
				numValues = somSelf->CountValues(ev);
				numProperties = somSelf->CountProperties(ev);
				if (_fPromiseResolver->GetPromiseCount() == 0)
				{
					ret = kODTrue;
					break;
				}
			}
			if (ret)
				break;
		}
	}

    }
    catch (ODException _exception)
    {
	ret = kODFalse;
        ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// NewODStorageUnitView
//------------------------------------------------------------------------------

static ODStorageUnitView* NewODStorageUnitView(ODMemoryHeapID heapID)
{
        Environment* ev = somGetGlobalEnvironment();
	ODStorageUnitView*	suView = kODNULL;
   try
   {
#ifdef _PLATFORM_MACINTOSH_
	SOMClass*	suViewClass = somNewClassReference(ODStorageUnitView);
	ODULong		size = suViewClass->somGetInstanceSize();
	ODPtr		buffer = ODNewPtr(size, heapID);
	suView = (ODStorageUnitView*) suViewClass->somRenew(buffer);
	somReleaseClassReference ( suViewClass );
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
                  suView  = new ODStorageUnitView;
#endif
   }
   catch (ODException _exception)
   {
        ODDeleteObject(suView);
        ODSetSOMException(ev, _exception);
   }
	
	return suView;
}

//------------------------------------------------------------------------------
// NewODStorageUnitCursor
//------------------------------------------------------------------------------

static ODStorageUnitCursor* NewODStorageUnitCursor(ODMemoryHeapID heapID)
{
        Environment* ev = somGetGlobalEnvironment();
	ODStorageUnitCursor*	suCursor = kODNULL;
   try
   {
#ifdef _PLATFORM_MACINTOSH_
	SOMClass*	suCursorClass = somNewClassReference(ODStorageUnitCursor);
	ODULong		size = suCursorClass->somGetInstanceSize();
	ODPtr		buffer = ODNewPtr(size, heapID);
	suCursor = (ODStorageUnitCursor*) suCursorClass->somRenew(buffer);
	somReleaseClassReference ( suCursorClass );
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
                   suCursor  = new ODStorageUnitCursor;
#endif
   }
   catch (ODException _exception)
   {
        ODDeleteObject(suCursor);
        ODSetSOMException(ev, _exception);
   }
	
	return suCursor;
}

//------------------------------------------------------------------------------
// NewCMStorageUnitRefIterator
//------------------------------------------------------------------------------

static CMStorageUnitRefIterator* NewCMStorageUnitRefIterator(ODMemoryHeapID heapID)
{
        Environment* ev = somGetGlobalEnvironment();
	CMStorageUnitRefIterator*	refIter = kODNULL;
   try
   {
#ifdef _PLATFORM_MACINTOSH_
	SOMClass*	refIterClass = somNewClassReference(CMStorageUnitRefIterator);
	ODULong		size = refIterClass->somGetInstanceSize();
	ODPtr		buffer = ODNewPtr(size, heapID);
	refIter = (CMStorageUnitRefIterator*) refIterClass->somRenew(buffer);
	somReleaseClassReference ( refIterClass );
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
               refIter = new CMStorageUnitRefIterator;
#endif
   }
   catch (ODException _exception)
   {
        ODDeleteObject(refIter);
        ODSetSOMException(ev, _exception);
   }
	
	return refIter;
}


