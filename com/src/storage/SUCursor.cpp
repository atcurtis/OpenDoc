/* @(#)Z 1.6 com/src/storage/SUCursor.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:04 (96/10/29 09:31:35) */
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
	File:		SUCursor.cpp

	Contains:	xxx put contents here xxx

	Owned by:	xxx put writers here xxx

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>	 10/8/95	TJ		Fixes Recomended by Refball
		 <6>	 8/21/95	VL		1278330, 1278315: Error code cleanup.
		 <5>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <4>	 6/16/95	VL		1259613: SUCursor can be created for
									unfocused SU.
		 <3>	 5/26/95	VL		1251403: Multithreading naming support.
		 <2>	  1/9/95	VL		1183661: Added setters and getters.
		 <1>	 6/28/94	VL		first checked in

	To Do:
*/

#define ODStorageUnitCursor_Class_Source
#define VARIABLE_MACROS
#include <SUCursor.xih>

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment SUCursor
#endif

SOM_Scope void  SOMLINK ODStorageUnitCursorInitStorageUnitCursor(ODStorageUnitCursor *somSelf, Environment *ev,
		ODPropertyName propertyName,
		ODValueType valueType,
		ODValueIndex valueIndex)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorInitStorageUnitCursor");
	
      try
      {

	/* Moved from somInit. SOM itself sets fields to zero
	_fCurProperty = kODNULL;
	_fCurValueType = kODNULL;
	_fCurValue = 0;
	*/

	if (propertyName != kODNULL) {	
		_fCurProperty = (ODPropertyName) ODNewPtr(ODISOStrLength(propertyName) + 1);
		ODISOStrCopy(_fCurProperty, propertyName);
	}
	else if ((valueType != kODNULL) || (valueIndex != 0))
		THROW(kODErrInsufficientInfoInParams);

	if (valueType != kODNULL) {
		_fCurValueType = (ODValueType) ODNewPtr(ODISOStrLength(valueType) + 1, ODGetDefaultHeap());
		ODISOStrCopy(_fCurValueType, valueType);
	}
	
	_fCurValue = valueIndex;
      }
      catch (ODException _exception)
      {
	_fCurProperty = kODNULL;
	_fCurValueType = kODNULL;
	_fCurValue = 0;
         ODSetSOMException(ev, _exception);
      }
}

SOM_Scope void  SOMLINK ODStorageUnitCursorSetProperty(ODStorageUnitCursor *somSelf, Environment *ev,
		ODPropertyName propertyName)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorSetCursor");

	ODDisposePtr(_fCurProperty);
		
	if (propertyName == kODNULL)
		_fCurProperty = kODNULL;
	else {
	       try
               {
	           _fCurProperty = (ODPropertyName) ODNewPtr(ODISOStrLength(propertyName) + 1, ODGetDefaultHeap());
		    ODISOStrCopy(_fCurProperty, propertyName);
               }
               catch (ODException _exception)
               {
                    _fCurProperty = kODNULL;
                    ODSetSOMException(ev, _exception);
               }
	}
}

SOM_Scope void  SOMLINK ODStorageUnitCursorSetValueType(ODStorageUnitCursor *somSelf, Environment *ev,
		ODValueType valueType)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorSetValueType");

	ODDisposePtr(_fCurValueType);

	if (valueType != kODNULL) {
              try
              {
		_fCurValueType = (ODValueType) ODNewPtr(ODISOStrLength(valueType) + 1, ODGetDefaultHeap());
		ODISOStrCopy(_fCurValueType, valueType);
              }
              catch (ODException _exception)
              {
                    _fCurValueType = kODNULL; 
                    ODSetSOMException(ev, _exception);
              }
	}
	else {
		_fCurValueType = kODNULL;
	}
}

SOM_Scope void  SOMLINK ODStorageUnitCursorSetValueIndex(ODStorageUnitCursor *somSelf, Environment *ev,
		ODValueIndex valueIndex)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorSetValueIndex");

	_fCurValue = valueIndex;
}

SOM_Scope void  SOMLINK ODStorageUnitCursorGetProperty(ODStorageUnitCursor *somSelf, Environment *ev,
		ODPropertyName* propertyName)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorGetProperty");

	if (_fCurProperty == kODNULL) {
		*propertyName = kODNULL;
	}
	else {
	     try
             {
		*propertyName = (ODPropertyName) ODNewPtr(ODISOStrLength(_fCurProperty) + 1, ODGetDefaultHeap());
		ODISOStrCopy(*propertyName, _fCurProperty);
             }
             catch (ODException _exception)
	     {
                *propertyName = kODNULL;
                ODSetSOMException(ev, _exception);
             }
	}
}

SOM_Scope void  SOMLINK ODStorageUnitCursorGetValueType(ODStorageUnitCursor *somSelf, Environment *ev,
		ODValueType* valueType)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorGetValueType");

	if (_fCurValueType == kODNULL)
		*valueType = kODNULL;
	else {
	     try
             {   
		*valueType = (ODValueType) ODNewPtr(ODISOStrLength(_fCurValueType) + 1, ODGetDefaultHeap());
		ODISOStrCopy(*valueType, _fCurValueType);
             }
	     catch (ODException _exception)
	     {
                 *valueType = kODNULL;
                 ODSetSOMException(ev, _exception);
             }
	}
}

SOM_Scope void  SOMLINK ODStorageUnitCursorGetValueIndex(ODStorageUnitCursor *somSelf, Environment *ev,
		ODValueIndex* valueIndex)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorGetValueIndex");

	*valueIndex = _fCurValue;
}

SOM_Scope void  SOMLINK ODStorageUnitCursorsomUninit(ODStorageUnitCursor *somSelf)
{
    ODStorageUnitCursorData *somThis = ODStorageUnitCursorGetData(somSelf);
    ODStorageUnitCursorMethodDebug("ODStorageUnitCursor","ODStorageUnitCursorsomUninit");

        Environment* ev = somGetGlobalEnvironment();
   try
   {
	if (_fCurProperty != kODNULL)
		ODDisposePtr((ODPtr) _fCurProperty);
	if (_fCurValueType != kODNULL)
		ODDisposePtr((ODPtr) _fCurValueType);
   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }

    ODStorageUnitCursor_parent_ODObject_somUninit(somSelf);
}
