/* @(#)Z 1.9 com/src/core/NmSpcMg.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:46 (96/10/29 09:20:50) */
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
	File:		NmSpcMg.cpp

	Contains:	Implementation of ODNameSpaceManager class

	Owned by:	David McCusker

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<21>	 8/29/95	DM		1279569: check nil for new SOM instances
		<20>	 8/26/95	T‚		1274606 FB2: Patching Remarks
		<19>	 8/25/95	JP		1272288: Added GetAETEs
		<18>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<17>	 7/27/95	DM		#1270320: Mem leaks: temp name in
									DeleteNameSpace
		<16>	 7/24/95	CG		#1270320: Memory leak fixes.
		<15>	 5/25/95	jpa		Return of <12>: HasNameSpace will call
									SynchNSTable if the namespace is synched to
									the Editors folders. SynchNSTable now
									returns a Boolean.  [1240150]
		<14>	 5/18/95	CG		#1241621	BB: Need to implement Purge for
									Binding subsystem objects.
		<13>	 5/17/95	TJ		Backed out changes from previous checkin.
		<11>	 5/11/95	DM		#1181622: Updated file ownership.
		<10>	 4/26/95	CG		1211082 BB: 5$ Bugs need to be evaluated
									and removed from Core
		 <9>	 4/25/95	DM		1172791: Replace AEHashTable with
									OpenHashTable
		 <8>	  4/6/95	TJ		Fixed Compile Errors.
		 <7>	  4/5/95	CG		#1216169 BB, #1221135: When Editors folder
									or OpenDoc Libraries folder is missing,
									OpenDoc quits and displays error.
		 <6>	 2/14/95	TJ		Fixed copyright date.
		 <5>	 2/14/95	jpa		Preferences::LoadAnyNewEditors -->
									ScanLibsAndEditors [1205141]
		 <4>	 1/23/95	CG		#1195019: Added field for tracking maximum
									length of strings in value name spaces.
		 <3>	 8/24/94	CG		#1181622: Updated file ownership.
		 <2>	 8/17/94	CG		#1178378: Destructor now deletes all name
									spaces.
		 <1>	 7/29/94	CG		first checked in
		 <3>	 6/28/94	VL		Used Translt.xh.
		 <2>	 6/24/94	CG		SOMized.
		 <1>	 6/14/94	CG		first checked in
		--------------------------------------------------------------
		 <8>	  6/1/94	CG		Added fPreferences and associated getters.
		 <7>	 5/27/94	jpa		New exception support [1165267]
		 <6>	 5/27/94	CG		#1165186: Abstracted prefs out of binding.
		 <5>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <4>	 3/26/94	NP		1152831. Add private stuff.
		 <3>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <2>	 2/15/94	CG		#1143680 - NameSpaceManager now being
									allocated on specified heap.
		 <7>	  2/8/94	T‚		Throw -> THROW & some code clean up
		 <6>	  2/3/94	CG		System Session break out.
		 <5>	 1/29/94	NP		Init changes and changed parameters of
									CreateNameSpace.
		 <4>	 1/21/94	CG		More NMAP changes.
		 <3>	 1/18/94	CG		Changes for new NMAP and for new
									XMPNameSpace::GetValue().
		 <2>	 1/18/94	T‚		use kOpenDocShellSignature
		 <1>	 1/14/94	CG		first checked in
	To Do:
	In Progress:
*/


#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#define ODNameSpaceManager_Class_Source
#define VARIABLE_MACROS
#include <NmSpcMg.xih>

#ifndef SOM_ODNameSpace_xh
#include <NamSpac.xh>
#endif

#ifndef SOM_ODObjectNameSpace_xh
#include <ObjectNS.xh>
#endif

#ifndef SOM_ODValueNameSpace_xh
#include <ValueNS.xh>
#endif

#ifndef SOM_ODObjectIterator_xh
#include <ObjctItr.xh>
#endif

#ifndef _STRHSHTB_
#include "StrHshTb.h"
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_ODSession_xh
#include "ODSessn.xh"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _PREFS_
#include "Prefs.h"
#endif
#endif

#ifndef __STRING__
#include <string.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#pragma segment ODNameSpaceManager

#include "NmSpcMgB.cpp"	// Platform-independent methods, if any

//==============================================================================
// Constants
//==============================================================================

const ODULong kNumExpectedNameSpaces = 10;

//==============================================================================
// ODNameSpaceManager
//==============================================================================

//------------------------------------------------------------------------------
// ODNameSpaceManager: InitNameSpaceManager
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceManagerInitNameSpaceManager
		(ODNameSpaceManager *somSelf, Environment *ev,
		ODSession* session)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","InitNameSpaceManager");

	try {

	/* Moved from somInit. SOM itself sets fields to zero
	_fNameSpaceTable = kODNULL;
#ifdef _PLATFORM_MACINTOSH_
	_fPreferences = kODNULL;
#endif
	_fMaxISOStrLength = 0;
	*/
	somSelf->InitObject(ev);
		
	_fSession = session;

	_fNameSpaceTable = new ODObjectNameSpace;
	THROW_IF_NULL(_fNameSpaceTable); // check all SOM new's for nil

	{ TempODISOStr	objName = ODISOStrFromCStr("NameSpaceTable");
          _fNameSpaceTable->InitObjectNameSpace(ev, somSelf, kODNULL, 
					kNumExpectedNameSpaces, objName);
	}
	
#ifdef _PLATFORM_MACINTOSH_
	_fPreferences = new Preferences();
	_fPreferences->InitPreferences(session);
#endif

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: HasNameSpace
//------------------------------------------------------------------------------

SOM_Scope ODNameSpace*  SOMLINK ODNameSpaceManagerHasNameSpace
		(ODNameSpaceManager *somSelf, Environment *ev,
		ODISOStr spaceName)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","HasNameSpace");

	ODNameSpace* 	theNameSpace = kODNULL;

	try {
		// Look in namespace table for spacename
	_fNameSpaceTable->GetEntry (ev, spaceName, (ODObject**)&theNameSpace);
			
	// If this namespace is read from the Editors folder(s), 
	// make sure the namespaces
	// are synched to the current Editors folder(s):
	if( theNameSpace && theNameSpace->IsSynchedToEditors(ev) )
		if( somSelf->SynchNSTable(ev) ) {
			// Synching causes the namespaces to be recreated, so the pointer is
			// no longer valid. Get a new pointer:
			_fNameSpaceTable->GetEntry (ev, spaceName, (ODObject**)&theNameSpace);
		}
			
        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }

	return theNameSpace;
}



//------------------------------------------------------------------------------
// ODNameSpaceManager: ~ODNameSpaceManager
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceManagersomUninit
		(ODNameSpaceManager *somSelf)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","somUninit");

	Environment* ev = somGetGlobalEnvironment();
	
	// SOM_TRY should not be necessary around deletions and another somUninit.
	try {
	
		ODDeleteObject( _fNameSpaceTable );
	
#ifdef _PLATFORM_MACINTOSH_
		ODDeleteObject( _fPreferences );
#endif

    		ODNameSpaceManager_parent_ODObject_somUninit(somSelf);

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODNameSpaceManagerPurge
		(ODNameSpaceManager *somSelf, Environment *ev,
		ODSize size)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","Purge");

	ODSize	purgeSize = 0;
	
	try {

#ifdef _PLATFORM_MACINTOSH_
	purgeSize = _fPreferences->Purge(size);
#endif

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }

	return purgeSize;
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: CreateNameSpace
//------------------------------------------------------------------------------

SOM_Scope ODNameSpace*  SOMLINK ODNameSpaceManagerCreateNameSpace
		(ODNameSpaceManager *somSelf, Environment *ev,
		
											ODISOStr		spaceName,
											ODNameSpace*	inheritsFrom,
											ODULong			numExpectedEntries,
											ODNSTypeSpec	type)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","CreateNameSpace");

	ODNameSpace* nameSpace = kODNULL;

	try {

	nameSpace = somSelf->HasNameSpace(ev,spaceName);
	if ( nameSpace )
		//THROW(kODErrKeyAlreadyExists);
		nameSpace = (ODNameSpace*)kODNULL;
	else
	{
		switch (type)
		{
			case kODNSDataTypeODObject:
			{
				ODObjectNameSpace* objNameSpace = new ODObjectNameSpace;
				THROW_IF_NULL(objNameSpace); // check all SOM new's for nil
				objNameSpace->InitObjectNameSpace(ev,somSelf, inheritsFrom,
											numExpectedEntries, spaceName);
				_fNameSpaceTable->Register(ev, spaceName, (ODObject*)objNameSpace);
				nameSpace = (ODNameSpace*) objNameSpace;
				break;
			}

			case kODNSDataTypeODValue:
			{
				ODValueNameSpace* valNameSpace = new ODValueNameSpace;
				THROW_IF_NULL(valNameSpace); // check all SOM new's for nil
				valNameSpace->InitValueNameSpace(ev,somSelf, inheritsFrom,
											numExpectedEntries, spaceName);
				_fNameSpaceTable->Register(ev, spaceName, (ODObject*)valNameSpace);
				nameSpace =  (ODNameSpace*) valNameSpace;
				break;
			}
				
			default:
			{
				nameSpace = (ODNameSpace*)kODNULL;
				break;
			}
		}
	}

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	return nameSpace;
}


//------------------------------------------------------------------------------
// ODNameSpaceManager: DeleteNameSpace
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceManagerDeleteNameSpace
		(ODNameSpaceManager *somSelf, Environment *ev,
		ODNameSpace* nameSpace)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","DeleteNameSpace");

	//ODISOStr				nsName;
	//ODObject*				nsObject;
	//ODULong					length;

	
	try {

	{ TempODISOStr name = nameSpace->GetName(ev);
         _fNameSpaceTable->Unregister(ev, name);
	}

	//ODObjectIterator*	iter = _fNameSpaceTable->CreateIterator(ev);
	//for (iter->First(ev, &nsName, &nsObject, &length);
	//		iter->IsNotComplete(ev);
	//		iter->Next(ev, &nsName, &nsObject, &length))
	//{
	//	if (((ODNameSpace*)nsObject)->GetType(ev) == kODNSDataTypeODObject)
	//		((ODObjectNameSpace*)nsObject)->NotifyDying(ev, nameSpace);
	//	else if ( ((ODNameSpace*)nsObject)->GetType(ev) == kODNSDataTypeODValue)
	//		((ODValueNameSpace*)nsObject)->NotifyDying(ev, nameSpace);
			
	//}
	
	//ODDeleteObject( iter );
	ODDeleteObject( nameSpace );

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}


//------------------------------------------------------------------------------
// ODNameSpaceManager: GetPreferences
//------------------------------------------------------------------------------

SOM_Scope Preferences*  SOMLINK ODNameSpaceManagerGetPreferences
		(ODNameSpaceManager *somSelf, Environment *ev)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","GetPreferences");

	return _fPreferences;
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: SynchNSTable
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODNameSpaceManagerSynchNSTable
		(ODNameSpaceManager *somSelf, Environment *ev)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","SynchNSTable");

	ODBoolean result = kODFalse;
	
	try {

#ifdef _PLATFORM_MACINTOSH_
	result = _fPreferences->ScanLibsAndEditors();
#endif

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	
	return result;
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: UpdatePreferences
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceManagerUpdatePreferences
		(ODNameSpaceManager *somSelf, Environment *ev)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","UpdatePreferences");

	try {

#ifdef _PLATFORM_MACINTOSH_
	_fPreferences->UpdatePreferences();
#endif

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: GetMaxISOStrLength
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODNameSpaceManagerGetMaxISOStrLength
		(ODNameSpaceManager *somSelf, Environment *ev)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","GetMaxISOStrLength");

	return _fMaxISOStrLength;
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: SetMaxISOStrLength
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceManagerSetMaxISOStrLength
		(ODNameSpaceManager *somSelf, Environment *ev,
	ODULong newLength)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","SetMaxISOStrLength");

	_fMaxISOStrLength = newLength;
}

//------------------------------------------------------------------------------
// ODNameSpaceManager: GetAETEs
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceManagerGetAETEs
		(ODNameSpaceManager *somSelf, Environment *ev, 
		ODSLong languageCode, AEDescList* aeteList)
{
    ODNameSpaceManagerData *somThis = ODNameSpaceManagerGetData(somSelf);
    ODNameSpaceManagerMethodDebug("ODNameSpaceManager","GetAETEs");

	try {

#ifdef _PLATFORM_MACINTOSH_
	_fPreferences->GetAETEs(languageCode, aeteList);
#endif

        }
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}


