/* @(#)Z 1.7 com/src/storage/StorageU.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:05 (96/10/29 09:31:54) */
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
	File:		StorageU.cpp

	Contains:	Implementation of ODStorageUnit

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<11>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<10>	 5/26/95	VL		1251403: Multithreading naming support.
		 <9>	 5/19/95	VL		1249626: in length and out byteArray for
									GetValue and GetPromiseValue.
		 <8>	 3/27/95	VL		1231412: Removed CopyTo.
		 <7>	 3/10/95	VL		1226116: Added SetStorageUnitRef.
		 <6>	 1/26/95	VL		#???: SURef is not a 4-byte array.
		 <5>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <4>	 8/26/94	VL		1183174: Removed CloneTo.
		 <3>	 8/11/94	VL		Added CreateCursor, modified CreateView and
									changed GetStorageUnitRefIterator to
									CreateStorageUnitRefIterator.
		 <2>	  7/5/94	VL		Added SubClassResponsibility.
		 <1>	 6/28/94	VL		first checked in

	To Do:
*/

#define ODStorageUnit_Class_Source
#include <StorageU.xih>

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

SOM_Scope void  SOMLINK ODStorageUnitInitStorageUnit(ODStorageUnit *somSelf, Environment *ev,
		ODDraft* draft,
		ODStorageUnitID suid)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitInitStorageUnit");
}

SOM_Scope ODDraft*  SOMLINK ODStorageUnitGetDraft(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetDraft");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope ODSession*  SOMLINK ODStorageUnitGetSession(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetSession");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitExists(ODStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName,
		ODValueType valueType,
		ODValueIndex valueIndex)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitExists");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitExistsWithCursor(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitCursor* cursor)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitExistsWithCursor");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODFalse;
}

SOM_Scope ODULong  SOMLINK ODStorageUnitCountProperties(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitCountProperties");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODULong  SOMLINK ODStorageUnitCountValues(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitCountValues");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitFocus(ODStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName,
		ODPositionCode propertyPosCode,
		ODValueType valueType,
		ODValueIndex valueIndex,
		ODPositionCode valuePosCode)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitFocus");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitFocusWithCursor(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitCursor* cursor)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitFocusWithCursor");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitExternalize(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitExternalize");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitInternalize(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitInternalize");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope ODID  SOMLINK ODStorageUnitGetID(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetID");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODStorageUnitName  SOMLINK ODStorageUnitGetName(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetName");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope void  SOMLINK ODStorageUnitSetName(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitName name)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitSetName");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitAddProperty(ODStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitAddProperty");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitAddValue(ODStorageUnit *somSelf, Environment *ev,
		ODValueType type)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitAddValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitRemove(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitRemove");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope void  SOMLINK ODStorageUnitCloneInto(ODStorageUnit *somSelf, Environment *ev,
		ODDraftKey key,
		ODStorageUnit* destStorageUnit,
		ODID initiatingFrame)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitCloneInto");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODStorageUnitView*  SOMLINK ODStorageUnitCreateView(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitCreateView");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope ODStorageUnitCursor*  SOMLINK ODStorageUnitCreateCursor(ODStorageUnit *somSelf, Environment *ev,
		ODPropertyName propertyName,
		ODValueType valueType,
		ODValueIndex valueIndex)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitCreateCursor");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope ODStorageUnitCursor*  SOMLINK ODStorageUnitCreateCursorWithFocus(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitCreateCursorWithFocus");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope ODPropertyName  SOMLINK ODStorageUnitGetProperty(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetProperty");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope ODValueType  SOMLINK ODStorageUnitGetType(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetType");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope void  SOMLINK ODStorageUnitSetType(ODStorageUnit *somSelf, Environment *ev,
		ODValueType valueType)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitSetType");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODStorageUnitSetOffset(ODStorageUnit *somSelf, Environment *ev,
		ODULong offset)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitSetOffset");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODULong  SOMLINK ODStorageUnitGetOffset(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetOffset");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODULong  SOMLINK ODStorageUnitGetValue(ODStorageUnit *somSelf, Environment *ev,
	ODULong length,
	ODByteArray* value)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope void  SOMLINK ODStorageUnitSetValue(ODStorageUnit *somSelf, Environment *ev,
	ODByteArray* value)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitSetValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODStorageUnitInsertValue(ODStorageUnit *somSelf, Environment *ev,
		ODByteArray* value)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitInsertValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODStorageUnitDeleteValue(ODStorageUnit *somSelf, Environment *ev,
		ODULong length)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitDeleteValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODULong  SOMLINK ODStorageUnitGetSize(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetSize");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitIsValidStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef aRef)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitIsValidStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODFalse;
}

SOM_Scope   void SOMLINK ODStorageUnitGetStrongStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID,
		ODStorageUnitRef ref)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetStrongStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODStorageUnitGetWeakStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID,
		ODStorageUnitRef ref)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetWeakStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitIsStrongStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitIsStrongStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODFalse;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitIsWeakStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitIsWeakStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODFalse;
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitRemoveStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef aRef)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitRemoveStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return somSelf;
}

SOM_Scope ODStorageUnitID  SOMLINK ODStorageUnitGetIDFromStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitRef aRef)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetIDFromStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODStorageUnitRefIterator*  SOMLINK ODStorageUnitCreateStorageUnitRefIterator(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitCreateStorageUnitRefIterator");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODNULL;
}

SOM_Scope void  SOMLINK ODStorageUnitSetStorageUnitRef(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID, ODStorageUnitRef ref)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitSetStorageUnitRef");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODULong  SOMLINK ODStorageUnitGetGenerationNumber(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetGenerationNumber");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODULong  SOMLINK ODStorageUnitIncrementGenerationNumber(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitIncrementGenerationNumber");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitIsPromiseValue(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitIsPromiseValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return kODFalse;
}

SOM_Scope void  SOMLINK ODStorageUnitSetPromiseValue(ODStorageUnit *somSelf, Environment *ev,
		ODValueType valueType,
		ODULong offset,
		ODByteArray* value,
		ODPart* sourcePart)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitSetPromiseValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODULong  SOMLINK ODStorageUnitGetPromiseValue(ODStorageUnit *somSelf, Environment *ev,
		ODValueType valueType,
		ODULong offset,
		ODULong length,
		ODByteArray* value,
		ODPart** sourcePart)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitGetPromiseValue");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope void  SOMLINK ODStorageUnitClearAllPromises(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitClearAllPromises");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODStorageUnitResolveAllPromises(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitResolveAllPromises");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODStorageUnitKey  SOMLINK ODStorageUnitLock(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitKey key)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitLock");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	return 0;
}

SOM_Scope void  SOMLINK ODStorageUnitUnlock(ODStorageUnit *somSelf, Environment *ev,
		ODStorageUnitKey key)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitUnlock");

	WARNMSG(WARN_INDEX(AMSG_710),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODStorageUnitsomUninit(ODStorageUnit *somSelf)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitsomUninit");

    ODStorageUnit_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope void  SOMLINK ODStorageUnitAcquire(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitAcquire");

    ODStorageUnit_parent_ODRefCntObject_Acquire(somSelf,ev);
}

SOM_Scope void  SOMLINK ODStorageUnitRelease(ODStorageUnit *somSelf, Environment *ev)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitRelease");

    ODStorageUnit_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope ODSize  SOMLINK ODStorageUnitPurge(ODStorageUnit *somSelf, Environment *ev,
		ODSize size)
{
    /* ODStorageUnitData *somThis = ODStorageUnitGetData(somSelf); */
    ODStorageUnitMethodDebug("ODStorageUnit","ODStorageUnitPurge");

    return (ODStorageUnit_parent_ODRefCntObject_Purge(somSelf,ev,size));
}
