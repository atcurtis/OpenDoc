/* @(#)Z 1.5 com/src/bento/CMSURefI.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:27 (96/10/29 09:11:43) */
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
	File:		CMSURefI.cpp

	Contains:	Implementation for CMStorageUnitRefIterator class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	10/24/95	jpa		1293441: DM/VL: Bento memory reserve &
									fatal container err.
		 <5>	 10/8/95	TJ		Fixes Recomended by Refball
		 <4>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <3>	 5/26/95	VL		1251403: Multithreading naming support.
		 <2>	 1/26/95	VL		#???: SURef is now a 4-byte array.
		 <1>	  7/5/94	VL		first checked in

	To Do:
	In Progress:
		
*/

#define CMStorageUnitRefIterator_Class_Source
#define VARIABLE_MACROS
#include <CMSURefI.xih>

#ifndef SOM_CMStorageUnit_xh
#include <CMSU.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif


//==============================================================================
// Function Prototype
//==============================================================================

inline void BREAK_AND_THROW(ODError err)
{
	THROW(err);
}

//==============================================================================
// CMStorageUnitRefIterator
//==============================================================================

//------------------------------------------------------------------------------
// CMStorageUnitRefIterator::~CMStorageUnitRefIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitRefIteratorsomUninit(CMStorageUnitRefIterator *somSelf)
{
    CMStorageUnitRefIteratorData *somThis = CMStorageUnitRefIteratorGetData(somSelf);
    CMStorageUnitRefIteratorMethodDebug("CMStorageUnitRefIterator","CMStorageUnitRefIteratorsomUninit");

    CMStorageUnitRefIterator_parent_ODStorageUnitRefIterator_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// CMStorageUnitRefIterator::InitStorageUnitRefIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitRefIteratorInitStorageUnitRefIterator(CMStorageUnitRefIterator *somSelf, Environment *ev,
		ODStorageUnit* storageUnit)
{
    CMStorageUnitRefIteratorData *somThis = CMStorageUnitRefIteratorGetData(somSelf);
    CMStorageUnitRefIteratorMethodDebug("CMStorageUnitRefIterator","CMStorageUnitRefIteratorInitStorageUnitRefIterator");

    try
    {
	
	/* Moved from somInit. SOM itself sets fields to zero
	_fValue = kODNULL;
	*((ODULong*) _fReference) = 0; 
	*/
	_fNotComplete = kODTrue;
	
    CMStorageUnitRefIterator_parent_ODStorageUnitRefIterator_InitStorageUnitRefIterator(somSelf,ev,storageUnit);

	CMValue value = ((CMStorageUnit*) storageUnit)->GetCurValue(ev);
	
	if (value == kODNULL)
		BREAK_AND_THROW(kODErrUnfocusedStorageUnit);
	_fValue = value;
	
     }
     catch (ODException _exception)
     {
         ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMStorageUnitRefIterator::First
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitRefIteratorFirst(CMStorageUnitRefIterator *somSelf, Environment *ev,
	ODStorageUnitRef ref)
{
    CMStorageUnitRefIteratorData *somThis = CMStorageUnitRefIteratorGetData(somSelf);
    CMStorageUnitRefIteratorMethodDebug("CMStorageUnitRefIterator","CMStorageUnitRefIteratorFirst");

    try
    {

	CMContainer	container = (_fValue)? CMGetValueContainer(_fValue) : (CMContainer) kODNULL;
	if (container)
		ODSessionMustHaveCMAllocReserve(container);
		
	*((ODULong*) _fReference) = 0; 
	if (CMGetNextReference(_fValue, _fReference) != kODNULL) {	
		ODBlockMove(_fReference, ref, kODStorageUnitRefSize);
		_fNotComplete = kODTrue;
	}
	else
		_fNotComplete = kODFalse;

	if (container)
		ODSessionRestoreCMAllocReserve(container);
	
    }
    catch (ODException _exception)
    {
          ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMStorageUnitRefIterator::Next
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMStorageUnitRefIteratorNext(CMStorageUnitRefIterator *somSelf, Environment *ev,
	ODStorageUnitRef ref)
{
    CMStorageUnitRefIteratorData *somThis = CMStorageUnitRefIteratorGetData(somSelf);
    CMStorageUnitRefIteratorMethodDebug("CMStorageUnitRefIterator","CMStorageUnitRefIteratorNext");

     try
     {

	CMContainer	container = (_fValue)? CMGetValueContainer(_fValue) : (CMContainer) kODNULL;
	if (container)
		ODSessionMustHaveCMAllocReserve(container);
	
	if (CMGetNextReference(_fValue, _fReference) != kODNULL) {
		ODBlockMove(_fReference, ref, kODStorageUnitRefSize);
		_fNotComplete = kODTrue;
	}
	else
		_fNotComplete = kODFalse;
	
	if (container)
		ODSessionRestoreCMAllocReserve(container);
	
     }
     catch (ODException _exception)
     {
            ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMStorageUnitRefIterator::IsNotComplete
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMStorageUnitRefIteratorIsNotComplete(CMStorageUnitRefIterator *somSelf, Environment *ev)
{
    CMStorageUnitRefIteratorData *somThis = CMStorageUnitRefIteratorGetData(somSelf);
    CMStorageUnitRefIteratorMethodDebug("CMStorageUnitRefIterator","CMStorageUnitRefIteratorIsNotComplete");

	return _fNotComplete;
}
