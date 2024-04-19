/* @(#)Z 1.6 com/src/core/ObjctItr.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:53 (96/10/29 09:21:14) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odcore
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
	File:		ObjctItr.cp

	Contains:	Implementation of ODValueIterator class

	Owned by:	Caia Grisar

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <8>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <7>	  5/3/95	DM		IsNotComplete return after try blocks
		 <6>	  5/2/95	DM		1244864: SOM_TRY blocks needed
		 <5>	 8/31/94	T‚		#1183129, #1183116, #1183119, #1183111:
									Lots of ErrorCode cleanup.
		 <4>	 8/24/94	CG		#1181622: Updated file ownership.
		 <3>	 8/17/94	CG		#1181440: delete -> ODDeleteObject, no more
									throwing, no more magic no.s.
		 <2>	  8/8/94	jpa		Fixed mistaken "1" value for heap to
									ODNewPtr
		 <1>	 7/29/94	CG		first checked in
		 <1>	 6/24/94	CG		first checked in
		 <0>	 6/24/94	SV		SOMverted
		 <3>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <2>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <2>	  2/8/94	T‚		Throw -> THROW & some code clean up
		 <1>	 1/14/94	CG		first checked in
	To Do:
	In Progress:
*/


#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif


#define ODObjectIterator_Class_Source
#define VARIABLE_MACROS
#include <ObjctItr.xih>

#ifndef SOM_ODObjectNameSpace_xh
#include <ObjectNS.xh>
#endif

#ifndef _STRHSHTB_
#include "StrHshTb.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#pragma segment ODObjectIterator

//==============================================================================
// ODObjectIterator
//
//	For all GetNext...
//	kODTrue is returned if an entry was found, kODFalse otherwise.
//	kODErrWrongNameSpaceType is thrown if the user attempts to use a function
//	that does not match the type that the ODNameSpace was initialized
//	with.
//==============================================================================

//------------------------------------------------------------------------------
// ODObjectIterator: ~ODObjectIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODObjectIteratorsomUninit(ODObjectIterator *somSelf)
{
    ODObjectIteratorData *somThis = ODObjectIteratorGetData(somSelf);
    ODObjectIteratorMethodDebug("ODObjectIterator","somUninit");

	if (_fStrHashIterator)
		ODDeleteObject( _fStrHashIterator );

    ODObjectIterator_parent_ODObject_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODObjectIterator: InitObjectIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODObjectIteratorInitObjectIterator(ODObjectIterator *somSelf, Environment *ev,
		ODObjectNameSpace* nameSpace)
{
    ODObjectIteratorData *somThis = ODObjectIteratorGetData(somSelf);
    ODObjectIteratorMethodDebug("ODObjectIterator","InitObjectIterator");

	try {
		somSelf->InitObject(ev);
		_fNameSpace = nameSpace;
		_fStrHashIterator = kODNULL;
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODObjectIterator: First
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODObjectIteratorFirst(ODObjectIterator *somSelf, Environment *ev,
		ODISOStr* key, ODObject** object, ODULong* objectLength)
{
    ODObjectIteratorData *somThis = ODObjectIteratorGetData(somSelf);
    ODObjectIteratorMethodDebug("ODObjectIterator","First");

	try {

	if (!_fStrHashIterator)
		_fStrHashIterator = new StringHashTableIterator
								(_fNameSpace->GetHashTable(ev));
	//THROW_IF_NULL(_fStrHashIterator);

	if (_fStrHashIterator)
	{
		ODULong	len;
		ODUByte*	string;
		_fStrHashIterator->First(&string, &len, (ODPtr*)object, objectLength);
	
		if (_fStrHashIterator->IsNotComplete())
		{
			*key = (ODISOStr)ODNewPtr(len + 1, kDefaultHeapID);
			ODBlockMove((ODPtr)string, *key, len);
			(*key)[len] = 0;
		}
	}
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODObjectIterator: Next
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODObjectIteratorNext(ODObjectIterator *somSelf, Environment *ev,
		ODISOStr* key, ODObject** object, ODULong* objectLength)
{
    ODObjectIteratorData *somThis = ODObjectIteratorGetData(somSelf);
    ODObjectIteratorMethodDebug("ODObjectIterator","Next");

	try {

	ODUByte* string;
	ODULong	len;

	_fStrHashIterator->Next(&string, &len, (ODPtr*)object, objectLength);

	if (_fStrHashIterator->IsNotComplete())
	{
		*key = (ODISOStr)ODNewPtr(len + 1, kDefaultHeapID);
		ODBlockMove((ODPtr)string, *key, len);
		(*key)[len] = 0;
	}
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}


//------------------------------------------------------------------------------
// ODObjectIterator: IsNotComplete
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODObjectIteratorIsNotComplete(ODObjectIterator *somSelf, Environment *ev)
{
    ODObjectIteratorData *somThis = ODObjectIteratorGetData(somSelf);
    ODObjectIteratorMethodDebug("ODObjectIterator","IsNotComplete");

	ODBoolean notComplete = kODFalse;

	try {

	if (_fStrHashIterator)
		notComplete = _fStrHashIterator->IsNotComplete();
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	
	return notComplete;
}



