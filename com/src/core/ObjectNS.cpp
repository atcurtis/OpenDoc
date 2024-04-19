/* @(#)Z 1.8 com/src/core/ObjectNS.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:54 (96/10/29 09:21:18) */
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
	File:		ObjectNS.cpp

	Contains:	Implementation of ODObjectNameSpace class

	Owned by:	David McCusker

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<13>	 10/8/95	TJ		Fixes Recomended by Refball
		<12>	 8/29/95	DM		1279507: make GetEntry() consistently
									return null object when it returns
									false.1279569: throw if null after newing
									SOM instances
		<11>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<10>	 7/24/95	CG		#1270320: Memory leak fixes.
		 <9>	 7/14/95	TJ		Jens added a test to check for non object
									deletions.
		 <8>	 5/11/95	DM		#1181622: Updated file ownership.
		 <7>	  5/1/95	CG		1241627	BB: SOM_TRY blocks needed in Core
									SOM methods
		 <6>	 4/26/95	CG		1211082 BB: 5$ Bugs need to be evaluated
									and removed from Core
		 <5>	12/13/94	CG		#1203295:  somUninit disposes of all
									objects.
		 <4>	 8/31/94	T‚		#1183129, #1183116, #1183119, #1183111:
									Lots of ErrorCode cleanup.
		 <3>	 8/24/94	CG		#1181622: Updated file ownership.
		 <2>	 8/18/94	jpa		Deleted obsolete I/O methods [1181839]
		 <1>	 7/29/94	CG		first checked in
		 <2>	  7/1/94	CG		Moved ReadSize and ReadBytes to
									BndNSUtl.cpp.
		 <1>	 6/24/94	CG		first checked in
		 <0>	 6/24/94	SV		SOMverted
		 <8>	 5/27/94	jpa		New exception support [1165267]
		 <7>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <6>	  4/1/94	T‚		#1154941: Get OpenDoc source to actually
									compile for PowerPC
		 <5>	 3/28/94	CG		1152929: Created new error
									kInvalidFileContents to throw when
									Read fails.
		 <4>	 3/25/94	T‚		removed erroneous comment: // $$$$ POWRPC
									NATVE
		 <3>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <2>	 2/15/94	CG		#1143680 - NameSpace now being allocated on
									specified heap.
		 <6>	  2/8/94	T‚		Throw -> THROW & some code clean up
		 <5>	  2/4/94	T‚		SysEqu -> LowMem
		 <4>	  2/3/94	CG		Added _fHeap field to store current heap.
		 <3>	 1/29/94	NP		Init changes.
		 <2>	 1/18/94	CG		Changed GetValue to take valueLen parameter.
		 <1>	 1/14/94	CG		first checked in
		<40>	 1/12/94	CG		Fixed bug in LoadNameSpaceTable().
		<39>	 1/11/94	T‚		Init... changes
		<38>	12/17/93	eeh		implement GetRegLibFileFolder
		<37>	12/15/93	T‚		more InitObject changes, remove
									Initialize() method
		<36>	12/15/93	T‚		InitObject Changes
		<35>	12/10/93	CG		Use HasNameSpace where appropriate.
		<34>	 12/6/93	CG		fixed NMAP reading bug.
		<33>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		<32>	11/18/93	CG		Changed prefs file format, mapping rez
									format.  Fixed bugs.
		<31>	11/15/93	NP		Changed XMPSymbols to ODNameSpaceManager.
									Fixed bugs.
		<30>	11/12/93	CG		The Symbols object  now uses ReadFromFile
									and WriteToFile for reading and writing to
									prefs file.
		<29>	 11/8/93	NP		Added WriteToStorage and ReadFromStorage.
		<28>	 11/8/93	NP		Tweak ReadFromFile and WriteToFile to work
									with refnum.
		<27>	 11/5/93	NP		Adding streaming functionality.
		<26>	 11/2/93	NP		Cast for CFront.
		<25>	 11/2/93	NP		Bug fixes.
		<24>	 11/2/93	CG		Fix for hash table changes.
		<23>	10/28/93	NP		Comment out obsoleted parts of API. Will
									remove later.
		<22>	10/22/93	NP		Mods for new XMPAEHashTable and
									StringHashTable protocols.
		<21>	10/18/93	CG		Inability to creat prefs file no longer
									prevents launch.
		<20>	10/14/93	JA		Changed prefs file name to a fixed "OpenDoc
									Prefs". (Using CurApName will not work
									later on when every doc is a process!)
		<18>	 10/7/93	CG		Symbols now get written to prefs file.
		<17>	 10/5/93	CG		Added Exists method to ODNameSpace.
		<16>	 9/22/93	JA		Updated ASSERTs for new macro.
		<15>	 9/21/93	CG		Added namespace table loading.
		<14>	 9/20/93	CG		Fixed ASSERT.
		<13>	 7/21/93	NP		Changed ODValueIterator::FirstNextAux.
		<12>	  7/2/93	NP		Abstract/Concrete breakout.
		<11>	 6/22/93	NP		Fixed some bugs. Synced with header.
		<10>	 6/15/93	NP		Mods to allow compilation with
									LibraryManager.h and a few bug fixes.
		 <9>	 6/14/93	NP		Implemented inheritance.
		 <8>	  6/10/93	RCR		Change to new Exception macros
		 <7>	  6/7/93	NP		Added missing cast in HasNameSpace. CFront
									complained, but Scpp did not.
		 <6>	  6/4/93	NP		Implemented everything except inheritance
									of name spaces.
		 <5>	  6/2/93	NP		Basic functionality working. Missing:
									Finding name space by name and iterator.
		 <4>	 5/18/93	NP		Updated HasNameSpace.
		 <3>	 4/28/93	NP		File name changes.
		 <2>	 4/27/93	NP		Added Purge.
		 <1>	 4/23/93	NP		first checked in

	To Do:
		Get prefs file name from a string resource.
	In Progress:
	
		Work around presence of non-objects in name space in somUninit.
*/

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif


#define ODObjectNameSpace_Class_Source
#include <ObjectNS.xih>

#ifndef _STRHSHTB_
#include "StrHshTb.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif
#endif

#ifndef __STRING__
#include <string.h>
#endif

#ifndef __LIMITS__
#include <limits.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __LOWMEM__
#include <LowMem.h>
#endif
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef SOM_ODObjectIterator_xh
#include <ObjctItr.xh>
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifndef _PLFMFILE_
#include "PLFMFile.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifdef _PLATFORM_MACINTOSH
#ifndef _MEMMGR_
#include <MemMgr.h>
#endif
#endif

#pragma segment ODObjectNameSpace

//==============================================================================
// Implementation notes
//==============================================================================

/*
Hash tables used for the implementation are lazily instantiated. They will be
created at the first request to add an entry.
*/

//==============================================================================
// Constants
//==============================================================================

// for hash tables

const ODUShort		kOSTypeKeySize = sizeof(ODOSType);
const ODUShort		kSLongKeySize = sizeof(ODSLong);
const ODUShort		kODPtrSize = sizeof(ODPtr);
const ODBoolean	kNotInSystemHeap = false;

//==============================================================================
// Function Prototype
//==============================================================================

//==============================================================================
// Local Functions
//==============================================================================

//==============================================================================
// ODObjectNameSpace
//==============================================================================

//------------------------------------------------------------------------------
// ODObjectNameSpace: InitObjectNameSpace
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODObjectNameSpaceInitObjectNameSpace(ODObjectNameSpace *somSelf, Environment *ev,
										ODNameSpaceManager*	mgr,
										ODNameSpace*		parent,
										ODULong				numExpectedEntries,
										ODISOStr			name)
{
    /* ODObjectNameSpaceData *somThis = ODObjectNameSpaceGetData(somSelf); */
    ODObjectNameSpaceMethodDebug("ODObjectNameSpace","InitObjectNameSpace");
	
	try {

		somSelf->SetType( ev, kODNSDataTypeODObject );
    		somSelf->InitNameSpace( ev, mgr, parent,numExpectedEntries, name);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODObjectNameSpace: ~ODObjectNameSpace
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODObjectNameSpacesomUninit(ODObjectNameSpace *somSelf)
{
    /* ODObjectNameSpaceData *somThis = ODObjectNameSpaceGetData(somSelf); */
    ODObjectNameSpaceMethodDebug("ODObjectNameSpace","somUninit");

	ODISOStr				key;
	ODObject*				object;
	ODULong					length;
	
	TRY{

		Environment* ev = somGetGlobalEnvironment();
		
		ODObjectIterator*	iter = somSelf->CreateIterator(ev);
		for (iter->First(ev, &key, &object, &length);
				iter->IsNotComplete(ev);
				iter->Next(ev, &key, &object, &length))
		{
			// This test is a workaround only; non-objects should not be stored here! $$$$$
#ifdef _PLATFORM_MACINTOSH_
			if( MMIsObject(object) && somIsObj(object) )
#else
			if( somIsObj(object) )
#endif
				delete object;
			else
				ODDisposePtr(object);
			ODDisposePtr( key );
		}
	
		ODDeleteObject( iter );
	}CATCH_ALL{
		// ignore exception
	}ENDTRY
}

//------------------------------------------------------------------------------
// ODObjectNameSpace: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODObjectNameSpacePurge(ODObjectNameSpace *somSelf, Environment *ev,
		ODSize size)
{
    /* ODObjectNameSpaceData *somThis = ODObjectNameSpaceGetData(somSelf); */
    ODObjectNameSpaceMethodDebug("ODObjectNameSpace","Purge");

	ODUnused(size);
	return 0;
}

//------------------------------------------------------------------------------
// ODObjectNameSpace: Register
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODObjectNameSpaceRegister(ODObjectNameSpace *somSelf, Environment *ev,
		ODISOStr key, ODObject* object)
{
    /* ODObjectNameSpaceData *somThis = ODObjectNameSpaceGetData(somSelf); */
    ODObjectNameSpaceMethodDebug("ODObjectNameSpace","Register");
	
	//1242543
	//if (somSelf->GetType(ev) != kODNSDataTypeODObject)
	//	THROW(kODErrWrongNameSpaceType);

	try {

		ODULong len = ODISOStrLength(key);
		ODULong valueLength = sizeof(ODObject);
		somSelf->GetHashTable(ev)->Insert((ODUByte*)key, len, (ODPtr)object, valueLength);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}


//------------------------------------------------------------------------------
// ODObjectNameSpace: GetEntry
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODObjectNameSpaceGetEntry(ODObjectNameSpace *somSelf, Environment *ev,
		ODISOStr key, ODObject** object )
{
    /* ODObjectNameSpaceData *somThis = ODObjectNameSpaceGetData(somSelf); */
    ODObjectNameSpaceMethodDebug("ODObjectNameSpace","GetEntry");

	ODBoolean	result = kODFalse;
	*object = kODNULL;
	
	try {

	if (somSelf->GetType(ev) == kODNSDataTypeODObject)
	{
		StringHashTable*	strHashTable = somSelf->GetHashTable(ev);
		if (strHashTable)
		{
			ODULong		objLen;
			size_t 		len = strlen(key);
			result = strHashTable->Find((ODUByte*)key, len, (ODPtr*)object, &objLen);
			if ( result == kODFalse )
			{
				ODObjectNameSpace *parent = (ODObjectNameSpace*)somSelf->GetParent(ev);
				if (parent)
				{
					if (parent->GetType(ev) == kODNSDataTypeODObject)
						result = parent->GetEntry(ev, key, object);
				}
			}
		}
		else
		{
			ODObjectNameSpace *parent = (ODObjectNameSpace*)somSelf->GetParent(ev);
			if (parent)
			{
				if (parent->GetType(ev) == kODNSDataTypeODObject)
					result = parent->GetEntry(ev, key, object);
			}
		}
	}
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }

	if ( !result )
		*object = kODNULL;

	return result;
}

//------------------------------------------------------------------------------
// ODObjectNameSpace: CreateIterator
//------------------------------------------------------------------------------

SOM_Scope ODObjectIterator*  SOMLINK ODObjectNameSpaceCreateIterator(ODObjectNameSpace *somSelf, Environment *ev)
{
    /* ODObjectNameSpaceData *somThis = ODObjectNameSpaceGetData(somSelf); */
    ODObjectNameSpaceMethodDebug("ODObjectNameSpace","CreateIterator");
	
	ODObjectIterator* theObjectIterator = kODNULL;

	try {
	
	theObjectIterator = new ODObjectIterator;
	THROW_IF_NULL(theObjectIterator);
	theObjectIterator->InitObjectIterator(ev, somSelf);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }

	return theObjectIterator;
}
