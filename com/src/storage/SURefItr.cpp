/* @(#)Z 1.7 com/src/storage/SURefItr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:05 (96/10/29 09:31:39) */
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
	File:		SURefItr.cpp

	Contains:	Implementation of ODStorageUnitRefIterator

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <5>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <4>	 5/26/95	VL		1251403: Multithreading naming support.
		 <3>	 1/26/95	VL		#???: SURef is not a 4-byte array.
		 <2>	 9/26/94	VL		1185419: Added SubClassResponsibility to
									methods.
		 <1>	 6/28/94	VL		first checked in

	To Do:
*/

#define ODStorageUnitRefIterator_Class_Source
#include <SURefItr.xih>

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

SOM_Scope void  SOMLINK ODStorageUnitRefIteratorInitStorageUnitRefIterator(ODStorageUnitRefIterator *somSelf, Environment *ev,
		ODStorageUnit* storageUnit)
{
    /* ODStorageUnitRefIteratorData *somThis = ODStorageUnitRefIteratorGetData(somSelf); */
    ODStorageUnitRefIteratorMethodDebug("ODStorageUnitRefIterator","ODStorageUnitRefIteratorInitStorageUnitRefIterator");

}

SOM_Scope void  SOMLINK ODStorageUnitRefIteratorFirst(ODStorageUnitRefIterator *somSelf, Environment *ev,
	ODStorageUnitRef ref)
{
    /* ODStorageUnitRefIteratorData *somThis = ODStorageUnitRefIteratorGetData(somSelf); */
    ODStorageUnitRefIteratorMethodDebug("ODStorageUnitRefIterator","ODStorageUnitRefIteratorFirst");

	WARNMSG(WARN_INDEX(AMSG_720),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODStorageUnitRefIteratorNext(ODStorageUnitRefIterator *somSelf, Environment *ev,
	ODStorageUnitRef ref)
{
    /* ODStorageUnitRefIteratorData *somThis = ODStorageUnitRefIteratorGetData(somSelf); */
    ODStorageUnitRefIteratorMethodDebug("ODStorageUnitRefIterator","ODStorageUnitRefIteratorNext");

	WARNMSG(WARN_INDEX(AMSG_720),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODBoolean  SOMLINK ODStorageUnitRefIteratorIsNotComplete(ODStorageUnitRefIterator *somSelf, Environment *ev)
{
    /* ODStorageUnitRefIteratorData *somThis = ODStorageUnitRefIteratorGetData(somSelf); */
    ODStorageUnitRefIteratorMethodDebug("ODStorageUnitRefIterator","ODStorageUnitRefIteratorIsNotComplete");

	WARNMSG(WARN_INDEX(AMSG_720),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODTrue;
}

SOM_Scope void  SOMLINK ODStorageUnitRefIteratorsomUninit(ODStorageUnitRefIterator *somSelf)
{
    /* ODStorageUnitRefIteratorData *somThis = ODStorageUnitRefIteratorGetData(somSelf); */
    ODStorageUnitRefIteratorMethodDebug("ODStorageUnitRefIterator","ODStorageUnitRefIteratorsomUninit");

    ODStorageUnitRefIterator_parent_ODObject_somUninit(somSelf);
}
