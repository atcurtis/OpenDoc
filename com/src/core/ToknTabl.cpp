/* @(#)Z 1.4 com/src/core/ToknTabl.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:55 (96/10/29 09:21:45) */
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
	File:		ToknTabl.cpp

	Contains:	Implementation of the TokenTable class

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<12>	 7/26/95	VL		1270320: Dispose of key and value allocated
									by ValueIteratorNext in GetType.
		<11>	 5/11/95	DM		#1246524, #1246529: ValueIteratorFirst(),
									ValueIteratorNext(),
									ValueNameSpaceGetEntry(),
									must pass through the value allocated.
		<10>	 3/24/95	CG		#1225887: Memory leak in
									TokenTable::GetType fixed.
		 <9>	  2/2/95	CG		#1195019:  ValueNameSpace entries are now
									copied in and out instead of pointers being
									passed around.
		 <8>	 1/23/95	CG		#1195019: ValueNameSpace methods now take
									ODByteArray parameter instead of ODPtr.
		 <7>	 8/19/94	NP		1181622: Ownership fix.
		 <6>	 8/17/94	CG		1181440: No more magic nums, delete
									iterators.
		 <5>	 7/29/94	CG		NameSpace API changes.
		 <4>	 6/27/94	CG		Removed fEv - not thread safe.
		 <3>	 6/18/94	MB		Add memory component
		 <2>	 6/14/94	CG		Include MemUtil.h instead of ODMem.h
		 <1>	  6/7/94	CG		first checked in
		 <9>	 1/29/94	NP		Fixed parameters to CreateNameSpace.
		 <8>	 1/20/94	SS		Fixed ::GetType
		 <7>	 1/18/94	CG		Changes for ODNameSpace::GetValue();
		 <6>	 1/14/94	CG		Added includes for NmSpcMgr.h and
									NmSpcItr.h
		 <5>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		 <4>	11/15/93	NP		Changed ODSymbols to ODNameSpaceManager.
		 <3>	 11/2/93	NP		Cast kODNULL to proper type.
		 <2>	10/28/93	NP		GetType returns a boolean instead of
									throwing an exception.
		 <1>	10/25/93	NP		first checked in
		 <6>	10/22/93	NP		Adjust to new ODStringHashTable protocol.
		 <5>	 9/21/93	CG		Updated to work with StrHashTbl changes.
		 <4>	 8/25/93	NP		Changed Fail to Throw.
		 <3>	 7/21/93	NP		Replace #ifdefs for volatile with
									ODVolatile.
		 <2>	 6/23/93	NP		Fixed bug that did not return the same
									ODTypeToken for the same ODType.
		 <1>	 6/22/93	NP		first checked in

	To Do:
*/

#ifndef _TOKNTABL_
#include "ToknTabl.h"
#endif

#ifndef SOM_ODValueNameSpace_xh
#include "ValueNS.xh"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include "NmSpcMg.xh"
#endif

#ifndef SOM_ODValueIterator_xh
#include <ValueItr.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif


#pragma segment TokenTable

//==============================================================================
// Constants
//==============================================================================

const ODULong kNumTableSlots = 50;

//==============================================================================
// TokenTable
//==============================================================================

//------------------------------------------------------------------------------
// TokenTable::TokenTable
//------------------------------------------------------------------------------

TokenTable::TokenTable(ODNameSpaceManager* mgr)
{
	fNameSpaceManager = mgr;
	fNameSpace = kODNULL;
	fNextUniqueID = 1;
}

//------------------------------------------------------------------------------
// TokenTable::Initialize
//------------------------------------------------------------------------------

void TokenTable::Initialize()
{
	Environment* ev = somGetGlobalEnvironment ();
	fNameSpace = (ODValueNameSpace*)fNameSpaceManager->CreateNameSpace(ev, 
												"TokenTable",
												(ODNameSpace*)kODNULL,
												kNumTableSlots, 
												kODNSDataTypeODValue);
}

//------------------------------------------------------------------------------
// TokenTable::~TokenTable
//------------------------------------------------------------------------------

TokenTable::~TokenTable()
{
	Environment* ev = somGetGlobalEnvironment ();
	if (fNameSpace && fNameSpaceManager)
		fNameSpaceManager->DeleteNameSpace(ev, fNameSpace);
}

//------------------------------------------------------------------------------
// TokenTable::Tokenize
//------------------------------------------------------------------------------

ODTypeToken TokenTable::Tokenize(ODType type)
{
	ODTypeToken     token;
	ODTypeToken*    tokenPtr;
	ODULong			valueLen;
	Environment* 	ev = somGetGlobalEnvironment ();

	if (ValueNameSpaceGetEntry(fNameSpace, ev, type, (ODPtr*) &tokenPtr, &valueLen))
	{
		token = *tokenPtr;
		ODDeleteObject( tokenPtr );
	}
	else
	{
		ValueNameSpaceRegister(fNameSpace, ev, type, (ODPtr) &fNextUniqueID, 
														sizeof(fNextUniqueID));
		token = fNextUniqueID++;
	}
	return token;
}

//------------------------------------------------------------------------------
// TokenTable::RemoveEntry
//------------------------------------------------------------------------------

void TokenTable::RemoveEntry(ODType type)
{
	Environment* ev = somGetGlobalEnvironment ();
	fNameSpace->Unregister(ev, type);
}

//------------------------------------------------------------------------------
// TokenTable::GetType
//
//	I need some functions to isolate ODISOStr creation, deletion and other
//	manipulation.
//------------------------------------------------------------------------------

ODBoolean TokenTable::GetType(ODTypeToken token, ODType* type)
{
	Environment* ev = somGetGlobalEnvironment ();
	ODValueIterator* iter = fNameSpace->CreateIterator(ev);
	ODTypeToken*	value = kODNULL;
	ODULong			len;
	ODULong			valueLength;
	ODBoolean		retVal = kODFalse;
	ODType			key = kODNULL;
	
	for (ValueIteratorFirst(iter, ev, &key,(ODPtr*) &value,  &valueLength);
		iter->IsNotComplete(ev) && !retVal;
		ValueIteratorNext(iter, ev, &key, (ODPtr*) &value, &valueLength)
		)
	{
		if (token == *value)
		{
			len = strlen(key);
			*type = (ODType) ODNewPtrClear(len + 1, kDefaultHeapID);
			ODBlockMove(key, *type, len);
			retVal = kODTrue;
		}
		ODDeleteObject( key );
		ODDeleteObject( value );
	}
	ODDeleteObject( key );
	ODDeleteObject( value );
	
	ODDeleteObject( iter);
	return retVal;
}

