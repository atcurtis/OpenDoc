/* @(#)Z 1.10 com/src/storage/SUView.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:05 (96/10/29 09:31:44) */
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
	File:		SUView.cpp

	Contains:	Implementation of ODStorageUnitView class.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<14>	 10/8/95	TJ		Fixes Recomended by Refball
		<13>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<12>	 6/16/95	VL		1259613: SetOffset should call _fSU.
		<11>	 5/26/95	VL		1251403: Multithreading naming support.
		<10>	 5/19/95	VL		1249626: in length and out byteArray for
									GetValue and GetPromiseValue.
		 <9>	 4/24/95	VL		1234914: Changed ODStorageUnitID to ODID in
									CloneInto. Changed in to out for
									GetPromiseValue.
		 <8>	 3/27/95	VL		1231412: Removed CopyTo.
		 <7>	 1/26/95	VL		#???: SURef is not a 4-byte array.
		 <6>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <5>	 8/26/94	VL		1183174: Sync up with latest SU API changes
									for cloning (CloneInto & CloneTo).
		 <4>	 8/11/94	VL		1180299: Sync up api with ODSTorageUnit for
									object factories.
		 <3>	 7/26/94	VL		Added IsValidStorageUnitRef.
		 <2>	  7/5/94	VL		Fixed sourcePart parameter of
									GetPromiseValue.
		 <1>	 6/28/94	VL		first checked in

	To Do:
*/

#define ODStorageUnitView_Class_Source
#define VARIABLE_MACROS
#include <SUView.xih>

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODStorageUnitCursor_xh
#include <SUCursor.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_UNIX_)
#include <ODUtils.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment SUView
#endif


//=====================================================================================
// SUViewLocker helper class
//=====================================================================================

#ifdef _NATIVE_EXCEPTIONS_   // using native exceptions
class SUViewLocker
#else
class SUViewLocker :public Destructo
#endif
{
	public:
		SUViewLocker( Environment *, ODStorageUnitView * );
		~SUViewLocker( );
	
	private:
		Environment *fEv;
		ODStorageUnitView *fSUV;
};

SUViewLocker::SUViewLocker( Environment *ev, ODStorageUnitView *suv )
{
	fEv = ev;
	fSUV = kODNULL;		// Don't set fSUV until after Lock succeeds
	suv->Lock(ev);		// so it won't be Unlocked if the Lock fails
	fSUV = suv;
}

SUViewLocker::~SUViewLocker( )
{
	ODStorageUnitView *suv = fSUV;
	fSUV = kODNULL;		// In case destructor is called twice
	if( suv )
		suv->Unlock(fEv);
}


//=====================================================================================
// ODStorageUnitView
//=====================================================================================

SOM_Scope void  SOMLINK ODStorageUnitViewInitStorageUnitView(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnit* storageUnit,
		ODStorageUnitCursor* cursor)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewInitStorageUnitView");

	try
        {
	
	   /* Moved from somInit. SOM itself sets fields to zero
	   _fSU = kODNULL;
	   _fCursor = kODNULL;
	   _fOffset = 0;
	   _fKey = kODNULL;
	   */
	
	   _fSU = storageUnit;
	   _fCursor = cursor;
	   _fSU->Acquire(ev);

        }
        catch (ODException _exception)
        {
	   _fSU = kODNULL;
	   _fCursor = kODNULL;
	   _fOffset = 0;
	   _fKey = kODNULL;
             
           ODSetSOMException(ev, _exception);
        }	
}

SOM_Scope ODStorageUnit*  SOMLINK ODStorageUnitViewGetStorageUnit(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetStorageUnit");

	return _fSU;
}

SOM_Scope ODStorageUnitCursor*  SOMLINK ODStorageUnitViewGetCursor(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetCursor");

	return _fCursor;
}

SOM_Scope ODStorageUnitView*  SOMLINK ODStorageUnitViewExternalize(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewExternalize");

	try
        {
		SUViewLocker lock(ev,somSelf);

		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetOffset(ev,_fOffset);
		_fSU->Externalize(ev);
		_fOffset = _fSU->GetOffset(ev);
        }
	catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
             return kODNULL;
        }
	
	return somSelf;
}

SOM_Scope ODStorageUnitView*  SOMLINK ODStorageUnitViewInternalize(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewInternalize");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetOffset(ev,_fOffset);
		_fSU->Internalize(ev);
		_fOffset = _fSU->GetOffset(ev);
        }
        catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
             return kODNULL;
         }
	
	return somSelf;
}

SOM_Scope ODID  SOMLINK ODStorageUnitViewGetID(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetID");

	ODID				id;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		id = _fSU->GetID(ev);
        }
        catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
             id = 0;
        }

	return id;
}

SOM_Scope ODStorageUnitName  SOMLINK ODStorageUnitViewGetName(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetName");

	ODStorageUnitName	name;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		name = _fSU->GetName(ev);
        }
        catch (ODException _exception)
        {
               ODSetSOMException(ev, _exception);
               name = kODNULL;
        }
	
	return name;
}

SOM_Scope void  SOMLINK ODStorageUnitViewSetName(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitName name)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewSetName");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetName(ev,name);
        }
        catch (ODException _exception)
        {
               ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODStorageUnitView*  SOMLINK ODStorageUnitViewAddProperty(ODStorageUnitView *somSelf, Environment *ev,
		ODPropertyName propertyName)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewAddProperty");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->AddProperty(ev,propertyName);
        }
        catch (ODException _exception)
        {
               ODSetSOMException(ev, _exception);
               return kODNULL;
        }
	
	return somSelf;
}

SOM_Scope ODStorageUnitView*  SOMLINK ODStorageUnitViewAddValue(ODStorageUnitView *somSelf, Environment *ev,
		ODValueType type)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewAddValue");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->AddValue(ev,type);
		_fOffset = _fSU->GetOffset(ev);
        }
        catch (ODException _exception)
        {
               ODSetSOMException(ev, _exception);
               return kODNULL;
        }
	
	return somSelf;
}

SOM_Scope ODStorageUnitView*  SOMLINK ODStorageUnitViewRemove(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewRemove");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->Remove(ev);
        }
        catch (ODException _exception)
        {
                ODSetSOMException(ev, _exception);
                return kODNULL;
        }

	return somSelf;
}

SOM_Scope void  SOMLINK ODStorageUnitViewCloneInto(ODStorageUnitView *somSelf, Environment *ev,
		ODDraftKey key,
		ODStorageUnit* destStorageUnit,
		ODID scopeID)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewCloneInto");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->CloneInto(ev,key, destStorageUnit, scopeID);
        }
        catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODPropertyName  SOMLINK ODStorageUnitViewGetProperty(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetProperty");

	ODPropertyName	propertyName;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		propertyName = _fSU->GetProperty(ev);
        }
	catch (ODException _exception)
        {
		propertyName = kODNULL;
                ODSetSOMException(ev, _exception);
        }
	
	return propertyName;
}

SOM_Scope ODValueType  SOMLINK ODStorageUnitViewGetType(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetType");

	ODValueType	valueType;
	
	try
        {
		SUViewLocker lock(ev,somSelf);

		_fSU->FocusWithCursor(ev,_fCursor);
		valueType = _fSU->GetType(ev);
        }
	catch (ODException _exception)
        {
		valueType = kODNULL;
                ODSetSOMException(ev, _exception);
        }
	
	return valueType;
}

SOM_Scope void  SOMLINK ODStorageUnitViewSetType(ODStorageUnitView *somSelf, Environment *ev,
		ODValueType valueType)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewSetType");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetType(ev,valueType);
        }
	catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
        }
}

SOM_Scope void  SOMLINK ODStorageUnitViewSetOffset(ODStorageUnitView *somSelf, Environment *ev,
		ODULong offset)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewSetOffset");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fOffset = offset;
		_fSU->SetOffset(ev, offset);
        }
        catch (ODException _exception)
        {
               ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODULong  SOMLINK ODStorageUnitViewGetOffset(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetOffset");

 	ODULong offset;
	
	try
        {
		SUViewLocker lock(ev,somSelf);

		offset = _fOffset;
		//somSelf->Unlock(ev); //Adkins
        }
        catch (ODException _exception)
        {
		offset = 0;
                ODSetSOMException(ev, _exception);
        }

	return offset;
}

SOM_Scope ODULong  SOMLINK ODStorageUnitViewGetValue(ODStorageUnitView *somSelf, Environment *ev,
		ODULong length,
		ODByteArray* value)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetValue");

	ODULong size;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetOffset(ev,_fOffset);
		size = _fSU->GetValue(ev, length, value);
		_fOffset = _fSU->GetOffset(ev);
        }
	catch (ODException _exception)
        {
		size = 0;
                ODSetSOMException(ev, _exception);
        }
                
	return size;
}

SOM_Scope void  SOMLINK ODStorageUnitViewSetValue(ODStorageUnitView *somSelf, Environment *ev,
		ODByteArray* value)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewSetValue");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetOffset(ev,_fOffset);
		_fSU->SetValue(ev,value);
		_fOffset = _fSU->GetOffset(ev);
        }
        catch (ODException _exception)
        {
            ODSetSOMException(ev, _exception);
        }
}

SOM_Scope void  SOMLINK ODStorageUnitViewInsertValue(ODStorageUnitView *somSelf, Environment *ev,
		ODByteArray* value)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewInsertValue");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetOffset(ev,_fOffset);
		_fSU->InsertValue(ev, value);
		_fOffset = _fSU->GetOffset(ev);
        }
        catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
        }
}

SOM_Scope void  SOMLINK ODStorageUnitViewDeleteValue(ODStorageUnitView *somSelf, Environment *ev,
		ODULong length)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewDeleteValue");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetOffset(ev,_fOffset);
		_fSU->DeleteValue(ev,length);
		_fOffset = _fSU->GetOffset(ev);
        }
        catch (ODException _exception)
        {
               ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODULong  SOMLINK ODStorageUnitViewGetSize(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetSize");

	ODULong	size;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		size = _fSU->GetSize(ev);
        }
	catch (ODException _exception)
        {
		size = 0;
                ODSetSOMException(ev, _exception);
        }
	
	return size;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitViewIsValidStorageUnitRef(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewIsValidStorageUnitRef");

	ODBoolean	isValid;
	
	try
        {
		SUViewLocker lock(ev,somSelf);

		_fSU->FocusWithCursor(ev,_fCursor);
		isValid = _fSU->IsValidStorageUnitRef(ev, ref);
        }
	catch (ODException _exception)
        {
                ODSetSOMException(ev, _exception);
		isValid = kODFalse;
	}
	
	return isValid;
}

SOM_Scope void SOMLINK ODStorageUnitViewGetStrongStorageUnitRef(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID,
		ODStorageUnitRef ref)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetStrongStorageUnitRef");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->GetStrongStorageUnitRef(ev,embeddedSUID, ref);
        }
	catch (ODException _exception)
        {
              ODSetSOMException(ev, _exception);
	}	
}

SOM_Scope void  SOMLINK ODStorageUnitViewGetWeakStorageUnitRef(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitID embeddedSUID,
		ODStorageUnitRef ref)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetWeakStorageUnitRef");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->GetWeakStorageUnitRef(ev,embeddedSUID, ref);
        }
        catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitViewIsStrongStorageUnitRef(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewIsStrongStorageUnitRef");

	ODBoolean	strong;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		strong = _fSU->IsStrongStorageUnitRef(ev,ref);
        }
	catch (ODException _exception)
        {
		strong = kODFalse;
                ODSetSOMException(ev, _exception);
        }
	
	return strong;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitViewIsWeakStorageUnitRef(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewIsWeakStorageUnitRef");

	ODBoolean	weak;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		weak = _fSU->IsWeakStorageUnitRef(ev,ref);
        }
	catch (ODException _exception)
        {
		weak = kODFalse;
                ODSetSOMException(ev, _exception);
	}
	
	return weak;
}

SOM_Scope ODStorageUnitView*  SOMLINK ODStorageUnitViewRemoveStorageUnitRef(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewRemoveStorageUnitRef");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->RemoveStorageUnitRef(ev,ref);
        }
	catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
             return kODNULL;
	}
	
	return somSelf;
}

SOM_Scope ODStorageUnitID  SOMLINK ODStorageUnitViewGetIDFromStorageUnitRef(ODStorageUnitView *somSelf, Environment *ev,
		ODStorageUnitRef ref)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetIDFromStorageUnitRef");
	
	ODStorageUnitID	id;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		id = _fSU->GetIDFromStorageUnitRef(ev,ref);
        }
	catch (ODException _exception)
        {    
		id = 0;
                ODSetSOMException(ev, _exception);
        }
	
	return id;
}

SOM_Scope ODStorageUnitRefIterator*  SOMLINK ODStorageUnitViewCreateStorageUnitRefIterator(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetStorageUnitRefIterator");

	ODStorageUnitRefIterator*	refIterator;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		refIterator =  _fSU->CreateStorageUnitRefIterator(ev);
        }
	catch (ODException _exception)
        {
		refIterator = kODNULL;
                ODSetSOMException(ev, _exception);
        }
	
	return refIterator;
}

SOM_Scope ODULong  SOMLINK ODStorageUnitViewGetGenerationNumber(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetGenerationNumber");

	ODULong	generationNumber;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		generationNumber = _fSU->GetGenerationNumber(ev);
        }
	catch (ODException _exception)
        {
		generationNumber = 0;
                ODSetSOMException(ev, _exception);
        }
	
	return generationNumber;
}

SOM_Scope ODULong  SOMLINK ODStorageUnitViewIncrementGenerationNumber(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewIncrementGenerationNumber");

	ODULong	generationNumber;
	
	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		generationNumber = _fSU->IncrementGenerationNumber(ev);
        }
	catch (ODException _exception)
        {
		generationNumber = 0;
                ODSetSOMException(ev, _exception);
        }

	return generationNumber;
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitViewIsPromiseValue(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewIsPromiseValue");

	ODBoolean result;

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		result = _fSU->IsPromiseValue(ev);
        }
	catch (ODException _exception)
        {
		result = kODFalse;
                ODSetSOMException(ev, _exception);
	}

	return result;
}

SOM_Scope void  SOMLINK ODStorageUnitViewSetPromiseValue(ODStorageUnitView *somSelf, Environment *ev,
		ODValueType valueType,
		ODULong offset,
		ODByteArray* value,
		ODPart* sourcePart)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewSetPromiseValue");

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		_fSU->SetPromiseValue(ev,valueType, offset, value, sourcePart);
		_fOffset = _fSU->GetOffset(ev);
        }
        catch (ODException _exception)
        {
              ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODULong  SOMLINK ODStorageUnitViewGetPromiseValue(ODStorageUnitView *somSelf, Environment *ev,
		ODValueType valueType,
		ODULong offset,
		ODULong	length,
		ODByteArray* value,
		ODPart** sourcePart)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewGetPromiseValue");

	ODULong numBytes;

	try
        {
		SUViewLocker lock(ev,somSelf);
		
		_fSU->FocusWithCursor(ev,_fCursor);
		numBytes = _fSU->GetPromiseValue(ev,valueType, offset, length, value, sourcePart);
		_fOffset = _fSU->GetOffset(ev);
        
        }
	catch (ODException _exception)
        {
		numBytes = 0;
	        ODSetSOMException(ev, _exception);
	}

	return numBytes;
}

SOM_Scope void  SOMLINK ODStorageUnitViewLock(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewLock");

	try
        {
		_fKey = _fSU->Lock(ev,_fKey);
        }
	catch (ODException _exception)
        {
             ODSetSOMException(ev, _exception);
        }
}

SOM_Scope void  SOMLINK ODStorageUnitViewUnlock(ODStorageUnitView *somSelf, Environment *ev)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewUnlock");

	try
        {
		_fSU->Unlock(ev,_fKey);
        }
	catch (ODException _exception)
	{
              ODSetSOMException(ev, _exception);
        }
	_fKey = kODNULL;
}

SOM_Scope void  SOMLINK ODStorageUnitViewsomUninit(ODStorageUnitView *somSelf)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewsomUninit");

        Environment* ev = somGetGlobalEnvironment();
        try
        {
	     ODSafeReleaseObject(_fSU);
	     delete _fCursor;
        }
        catch (ODException _exception)
        {
              ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODSize  SOMLINK ODStorageUnitViewPurge(ODStorageUnitView *somSelf, Environment *ev,
		ODSize size)
{
    ODStorageUnitViewData *somThis = ODStorageUnitViewGetData(somSelf);
    ODStorageUnitViewMethodDebug("ODStorageUnitView","ODStorageUnitViewPurge");

	return 0;
}
