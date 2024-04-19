/* @(#)Z 1.6 com/src/core/ValueItr.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:56 (96/10/29 09:21:54) */
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
	File:		ValueItr.cp

	Contains:	Implementation of ODValueIterator class

	Owned by:	David McCusker

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<12>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<11>	 5/11/95	DM		#1246524: First() and Next() inout->out
									requires allocation for value
									#1181622: Updated file ownership.
		<10>	  5/3/95	DM		IsNotComplete return after try blocks
		 <9>	  5/2/95	DM		1244864: SOM_TRY blocks needed
		 <8>	  2/2/95	CG		#1195019:  ValueNameSpace entries are now
									copied in and out instead of pointers being
									passed around.
		 <7>	 1/23/95	CG		#1195019: ValueIterator methods now take
									ODByteArray parameter instead of ODPtr.
		 <6>	 9/14/94	jpa		Don't include MemDebg.h [1186692]
		 <5>	 8/31/94	T‚		#1183129, #1183116, #1183119, #1183111:
									Lots of ErrorCode cleanup.
		 <4>	 8/24/94	CG		#1181622: Updated file ownership.
		 <3>	 8/17/94	CG		#1181440: No more magic nums, no throwing,
									delete -> ODDeleteObject.
		 <2>	  8/4/94	eeh		bug 1179044: pass null heap id in
									ODValueIteratorNext
		 <1>	 7/29/94	CG		first checked in
		 <1>	 6/24/94	CG		first checked in
		 <0>	 6/24/94	SV		SOMverted
		 <3>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <2>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <2>	  2/8/94	T‚		Throw -> THROW & some code clean up
		 <1>	 1/14/94	CG		first checked in
	To Do:
*/


#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODTYPESP_
#include <ODTypesP.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

#define ODValueIterator_Class_Source
#define VARIABLE_MACROS
#include <ValueItr.xih>

#ifndef SOM_ODValueNameSpace_xh
#include <ValueNS.xh>
#endif

//#ifndef _AEHSHTBL_
//#include "AEHshTbl.h"
//#endif

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

#ifndef _BARRAY_
#include <BArray.h>
#endif

#pragma segment ODValueIterator

//==============================================================================
// ODValueIterator
//
//	For all GetNext...
//	kODTrue is returned if an entry was found, kODFalse otherwise.
//	kODErrWrongNameSpaceType is thrown if the user attempts to use a function
//	that does not match the type that the ODNameSpace was initialized
//	with.
//==============================================================================

//------------------------------------------------------------------------------
// ODValueIterator: ~ODValueIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueIteratorsomUninit(ODValueIterator *somSelf)
{
    ODValueIteratorData *somThis = ODValueIteratorGetData(somSelf);
    ODValueIteratorMethodDebug("ODValueIterator","somUninit");

	//if (_fAEHashTableIterator)
	//	delete _fAEHashTableIterator;
	if (_fStrHashIterator)
		ODDeleteObject( _fStrHashIterator );

    ODValueIterator_parent_ODObject_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODValueIterator: InitValueIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueIteratorInitValueIterator(ODValueIterator *somSelf, Environment *ev,
		ODValueNameSpace* nameSpace)
{
    ODValueIteratorData *somThis = ODValueIteratorGetData(somSelf);
    ODValueIteratorMethodDebug("ODValueIterator","InitValueIterator");

	try {
	
		somSelf->InitObject(ev);
		_fNameSpace = nameSpace;
		_fStrHashIterator = kODNULL;
		//_fAEHashTableIterator = kODNULL;
		
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueIterator: First
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueIteratorFirst(ODValueIterator *somSelf, Environment *ev,
		ODISOStr* key, ODByteArray* value)
{
    ODValueIteratorData *somThis = ODValueIteratorGetData(somSelf);
    ODValueIteratorMethodDebug("ODValueIterator","First");

	*key = kODNULL;
	value->_buffer = kODNULL;
	value->_maximum = value->_length = 0;
	
	try {

	if (!_fStrHashIterator)
		_fStrHashIterator = new StringHashTableIterator
								(_fNameSpace->GetHashTable(ev));
	//THROW_IF_NULL(_fStrHashIterator);

	if (_fStrHashIterator)
	{
		ODULong	len;
		ODUByte*	string;
		ODPtr buffer = kODNULL;
		ODULong size = 0;
		_fStrHashIterator->First(&string, &len ,&buffer, &size);
	
		if (_fStrHashIterator->IsNotComplete())
		{
			*key = (ODISOStr)ODNewPtr(len + 1, kDefaultHeapID);
			ODBlockMove((Ptr)string, *key, len);
			(*key)[len] = 0;
		
			value->_length = value->_maximum = size;
			if ( size > 0 )
			{
				value->_buffer = (octet*) ODNewPtr(size, kDefaultHeapID);
				ODBlockMove( buffer, value->_buffer, size);
			}
		}
	}
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueIterator: Next
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueIteratorNext(ODValueIterator *somSelf, Environment *ev,
		ODISOStr* key, ODByteArray* value)
{
    ODValueIteratorData *somThis = ODValueIteratorGetData(somSelf);
    ODValueIteratorMethodDebug("ODValueIterator","Next");

	*key = kODNULL;
	value->_buffer = kODNULL;
	value->_maximum = value->_length = 0;

	try {

//	if ( !ODValidateAllHeaps())
//		return ;

	ODUByte*	string;
	ODULong	len;
	ODPtr buffer = kODNULL;
	ODULong size = 0;

	_fStrHashIterator->Next(&string, &len, &buffer, &size);

//	if ( !ODValidateAllHeaps())
//		return ;

	if (_fStrHashIterator->IsNotComplete())
	{
//		if ( !ODValidateAllHeaps())
//			return ;

		*key = (ODISOStr)ODNewPtr(len + 1, kODNULL);
		ODBlockMove((Ptr)string, *key, len);
		(*key)[len] = 0;
		
		value->_length = value->_maximum = size;
		if ( size > 0 )
		{
			value->_buffer = (octet*) ODNewPtr(size, kDefaultHeapID);
			ODBlockMove( buffer, value->_buffer, size);
		}
	}
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueIterator: IsNotComplete
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODValueIteratorIsNotComplete(ODValueIterator *somSelf, Environment *ev)
{
    ODValueIteratorData *somThis = ODValueIteratorGetData(somSelf);
    ODValueIteratorMethodDebug("ODValueIterator","IsNotComplete");
	
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



