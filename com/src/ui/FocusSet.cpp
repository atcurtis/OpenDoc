/* @(#)Z 1.5 com/src/ui/FocusSet.cpp, odui, od96os2, odos29646d 96/11/15 15:28:05 (96/07/15 18:26:45) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odui
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
	File:		FocusSet.cpp

	Contains:	ODFocusSet implementation

	Owned by:	Richard Rodseth

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<11>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<10>	 7/25/95	VL		1270320: DeleteAllLinks on _fImplementation
									in somUninit.
		 <9>	 5/26/95	RR		#1251403: Multithreading naming support
		 <8>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <7>	 4/28/95	RR		1211085 Remove 5$ comments
		 <6>	 4/13/95	RR		# 1216618 Added ODVolatile, ODDeleteObject
		 <5>	  4/7/95	RR		#1216618 Added SOM_TRY etc.
		 <4>	 2/22/95	RR		#1209427 Changed private api between
									iterator and iteratee
		 <3>	 9/29/94	RA		1189812: Mods for 68K build.
		 <2>	 9/26/94	RR		Include ArbUtils
		 <1>	 9/26/94	RR		first checked in

	To Do:
	In Progress:
*/


#define ODFocusSet_Class_Source
#define VARIABLE_MACROS
#include <FocusSet.xih>

#ifndef SOM_ODFocusSetIterator_xh
#include "FocusItr.xh"
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include "Foci.xh"	// For kODNULLFocus. Returned by iterator, but ignored
#endif

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

#ifndef _ARBUTILS_
#include "ArbUtils.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"	// Adkins -- added
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)  
#include "ODUtils.h"
#endif

#if !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_WIN32_)
#pragma segment ODFocusSet
#endif


//======================================================================================
// Method Implementations for ODFocusSet
//======================================================================================


SOM_Scope void  SOMLINK ODFocusSetInitFocusSet(ODFocusSet *somSelf, Environment *ev)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetInitFocusSet");
    
	/* Moved from somInit. SOM itself sets fields to zero
	_fImplementation = kODNULL;
	*/

	if (_fImplementation == kODNULL)
	{
    	LinkedList* linkedList = kODNULL; ODVolatile(linkedList);

	    SOM_TRY
		
			
			somSelf->InitObject(ev);
	
			linkedList = new LinkedList;
			_fImplementation = linkedList;
			
		SOM_CATCH_ALL
			
			ODDeleteObject(linkedList);
			
		SOM_ENDTRY
	}
}

SOM_Scope void  SOMLINK ODFocusSetAdd(ODFocusSet *somSelf, Environment *ev,
		ODTypeToken focus)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetAdd");

	FocusLink* focusLink = kODNULL; ODVolatile(focusLink);
		
	SOM_TRY
	
		ASSERT(_fImplementation != kODNULL,kODErrUndefined); // Note: Undefined error code
	
		if (!somSelf->Contains(ev,focus))
		{
			focusLink = new FocusLink(focus);		
			_fImplementation->AddLast(focusLink);
		}

	SOM_CATCH_ALL
	
		ODDeleteObject(focusLink);
		
	SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODFocusSetRemove(ODFocusSet *somSelf, Environment *ev,
		ODTypeToken focus)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetRemove");

	SOM_TRY
	
		ASSERT(_fImplementation != kODNULL,kODErrUndefined);
	
		LinkedListIterator iter(_fImplementation);
		ODTypeToken foundFocus;
		
		FocusLink* link = (FocusLink*) iter.First();
		while (link != kODNULL)
		{
			foundFocus = link->fFocus;
			if (foundFocus == focus) 
			{
				_fImplementation->Remove(*link);
				delete link;
				link = kODNULL;
			}
			else
				link = (FocusLink*) iter.Next();
		}	

	SOM_CATCH_ALL
	SOM_ENDTRY

}

SOM_Scope ODBoolean  SOMLINK ODFocusSetContains(ODFocusSet *somSelf, Environment *ev,
		ODTypeToken focus)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetContains");
    
    ODBoolean containsFocus = kODFalse;

	SOM_TRY
	
		ASSERT(_fImplementation != kODNULL,kODErrUndefined);
	
		LinkedListIterator iter(_fImplementation);
		ODTypeToken foundFocus;
	
		FocusLink* link = (FocusLink*) iter.First();
		while (link != kODNULL)
		{
			foundFocus = link->fFocus;
	
			if (foundFocus == focus) 
			{
				containsFocus = kODTrue;
				break;	
			}
			else
				link = (FocusLink*) iter.Next();
		}	

	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return containsFocus;
}

SOM_Scope ODFocusSetIterator*  SOMLINK ODFocusSetCreateIterator(ODFocusSet *somSelf, Environment *ev)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetCreateIterator");

	ODFocusSetIterator* iterator = kODNULL; ODVolatile(iterator);
	
	SOM_TRY
	
		iterator = new ODFocusSetIterator;
		THROW_IF_NULL(iterator);
		iterator->InitFocusSetIterator(ev,somSelf);
		
	SOM_CATCH_ALL
	
		ODDeleteObject(iterator);
		
	SOM_ENDTRY
	
	return iterator;
}

SOM_Scope ODULong  SOMLINK ODFocusSetAddIterator(ODFocusSet *somSelf, Environment *ev,
		ODFocusSetIterator* iterator)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetAddIterator");

	LinkedListIterator* iter = kODNULL; ODVolatile(iter);

	SOM_TRY
	
		iter = new LinkedListIterator(_fImplementation);
		
	SOM_CATCH_ALL
	
		ODDeleteObject(iter);
		
	SOM_ENDTRY

	return (ODULong) iter;
}

SOM_Scope ODTypeToken  SOMLINK ODFocusSetFirst(ODFocusSet *somSelf, Environment *ev,
		ODULong iteratorID)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetFirst");

	ODTypeToken focus = kODNullFocus;

	SOM_TRY
	
		LinkedListIterator* iterator = (LinkedListIterator*) iteratorID; 
		FocusLink* link = (FocusLink*) iterator->First();
		if (link)
			focus = link->fFocus;

	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return focus;
}

SOM_Scope ODTypeToken  SOMLINK ODFocusSetNext(ODFocusSet *somSelf, Environment *ev,
		ODULong iteratorID)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetNext");

	ODTypeToken focus = kODNullFocus;

	SOM_TRY

		LinkedListIterator* iterator = (LinkedListIterator*) iteratorID; 
		FocusLink* link = (FocusLink*) iterator->Next();
		if (link)
			focus = link->fFocus;
			
	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return focus;
}

SOM_Scope ODBoolean  SOMLINK ODFocusSetIsNotComplete(ODFocusSet *somSelf, Environment *ev,
		ODULong iteratorID)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetIsNotComplete");
    
    ODBoolean isNotComplete = kODFalse;

	SOM_TRY

		LinkedListIterator* iterator = (LinkedListIterator*) iteratorID; 
		isNotComplete = iterator->IsNotComplete();
	
	SOM_CATCH_ALL
	SOM_ENDTRY

	return isNotComplete;
}

SOM_Scope void  SOMLINK ODFocusSetRemoveIterator(ODFocusSet *somSelf, Environment *ev,
		ODULong iteratorID)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetRemoveIterator");
    
	LinkedListIterator* iterator = (LinkedListIterator*) iteratorID; 
    ODDeleteObject(iterator);
}

SOM_Scope void  SOMLINK ODFocusSetsomUninit(ODFocusSet *somSelf)
{
    ODFocusSetData *somThis = ODFocusSetGetData(somSelf);
    ODFocusSetMethodDebug("ODFocusSet","ODFocusSetsomUninit");

	_fImplementation->DeleteAllLinks();
	ODDeleteObject(_fImplementation);
    parent_somUninit(somSelf);
}
