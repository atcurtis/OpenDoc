/* @(#)Z 1.10 com/src/core/ValueNS.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:56 (96/10/29 09:21:59) */
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
	File:		ValueNS.cpp

	Contains:	Implementation of ODValueNameSpace class

	Owned by:	David McCusker

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<18>	 9/11/95	CG		#1282719 FB2: Core error code mods.
		<17>	 8/29/95	DM		1279569: check nil for new SOM instances
		<16>	 8/18/95	NP		1274946: Added include.
		<15>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<14>	 7/27/95	DM		#1270320 - mem leaks: delete GetName
									returns in Read and Write
		<13>	 7/24/95	CG		#1270320: Memory leak fixes.
		<12>	 5/11/95	DM		#1246523: GetEntry() must alloc space for
									the out value. #1246913: Register() memory
									leak (destroy old value). #1181622: Updated
									file ownership.
		<11>	  5/2/95	DM		1244864: SOM_TRY blocks needed
		<10>	  5/1/95	CG		#1241627	BB: SOM_TRY blocks needed in Core
									SOM methods
		 <9>	 3/24/95	CG		#1217023 API: Read and Write now use
									ODByteArray instead of Platformfile.
		 <8>	  2/2/95	CG		#1195019:  ValueNameSpace entries are now
									copied in and out instead of pointers being
									passed around.
		 <7>	 1/25/95	jpa		Fixed no-prefs crashes [1214750]
		 <6>	 1/23/95	CG		#1195019: ValueNameSpace methods now take
									ODByteArray parameter instead of ODPtr.
		 <5>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <4>	 8/31/94	T‚		#1183129, #1183116, #1183119, #1183111:
									Lots of ErrorCode cleanup.
		 <3>	 8/30/94	CG		#1152929: Changed kODErrInvalidFileContents
									back to kODErrInvalidName.
		 <2>	 8/17/94	CG		#1181440: Use kDefaultHeapID instead of 0
									in NewPtr calls.
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
		
*/

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#define ODValueNameSpace_Class_Source
#include <ValueNS.xih>

#ifndef SOM_ODNameSpace_xh
#include "NamSpac.xh"
#endif

#ifndef _STRHSHTB_
#include "StrHshTb.h"
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

#ifndef _ODTYPESP_
#include <ODTypesP.h>
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

#ifndef SOM_ODValueIterator_xh
#include <ValueItr.xh>
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifndef _PLFMFILE_
#include "PLFMFile.h"
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _UTILERRS_
#include <UtilErrs.h>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include "NmSpcMg.xh"
#endif

#pragma segment ODValueNameSpace

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

//const ODUShort		kOSTypeKeySize = sizeof(ODOSType);
//const ODUShort		kSLongKeySize = sizeof(ODSLong);
//const ODUShort		kODPtrSize = sizeof(ODPtr);
//const ODBoolean	kNotInSystemHeap = false;

//==============================================================================
// Function Prototype
//==============================================================================

//==============================================================================
// Local Functions
//==============================================================================

//==============================================================================
// ODValueNameSpace
//==============================================================================

//------------------------------------------------------------------------------
// ODValueNameSpace: InitValueNameSpace
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceInitValueNameSpace(ODValueNameSpace *somSelf, Environment *ev,
		ODNameSpaceManager*		mgr,
		ODNameSpace*			parent,
		ODULong					numExpectedEntries,
		ODISOStr				name)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","InitValueNameSpace");

	try {

	somSelf->SetType( ev, kODNSDataTypeODValue );
	somSelf->InitNameSpace(ev, mgr, parent, numExpectedEntries, name);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: ~ODValueNameSpace
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpacesomUninit(ODValueNameSpace *somSelf)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","somUninit");

	Environment* ev = somGetGlobalEnvironment();
	
	try {

	StringHashTable* hashTable = somSelf->GetHashTable(ev);
	StringHashTableIterator* hashTableItr = new StringHashTableIterator(hashTable);

	if (hashTableItr)
	{
		ODULong	len;
		ODUByte*	string;
		ODPtr buffer = kODNULL;
		ODULong size = 0;
		for(hashTableItr->First(&string, &len ,&buffer, &size);
			hashTableItr->IsNotComplete();
			hashTableItr->Next(&string, &len ,&buffer, &size))
		{
			ODDisposePtr( buffer );
		}
		
		delete ( hashTableItr );
	}
	
    ODValueNameSpace_parent_ODNameSpace_somUninit(somSelf);

        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODValueNameSpacePurge(ODValueNameSpace *somSelf, Environment *ev,
		ODSize size)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","Purge");

	ODUnused(size);
	return 0;
}

//------------------------------------------------------------------------------
// ODValueNameSpace: Register
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceRegister(ODValueNameSpace *somSelf, Environment *ev,
		ODISOStr key, ODByteArray* value)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","Register");

	ODPtr buffer = kODNULL;
	ODULong size = 0;
	
	try {
	
	ODULong len = ODISOStrLength(key);
	StringHashTable* hashTable = somSelf->GetHashTable(ev);
	
	if (hashTable->Find((ODUByte*)key, len, &buffer, &size))
	{
		// destroy pre-existing old value to prevent a memory leak
		ODDisposePtr( buffer );
		hashTable->Remove((ODUByte*)key, len);
	}
	
	ODULong valueLength = value->_length;
	buffer = ODNewPtrClear(valueLength, kDefaultHeapID);
	ODBlockMove( value->_buffer, buffer, valueLength );
	hashTable->Insert((ODUByte*)key,len, buffer, valueLength);

	if (somSelf->GetNameSpaceManager(ev)->
					   GetMaxISOStrLength(ev) < valueLength)
		somSelf->GetNameSpaceManager(ev)->
						SetMaxISOStrLength(ev, valueLength);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: Unregister
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceUnregister(ODValueNameSpace *somSelf, Environment *ev,
		ODISOStr key)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","ODValueNameSpaceUnregister");

	ODPtr buffer = kODNULL;
	ODULong size = 0;

	try {

	size_t len = strlen(key);
	somSelf->GetHashTable(ev)->Find((ODUByte*)key, len, &buffer, &size);
	ODDisposePtr( buffer );
	somSelf->GetHashTable(ev)->Remove((ODUByte*)key, len);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: GetEntry
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODValueNameSpaceGetEntry(ODValueNameSpace *somSelf, Environment *ev,
		ODISOStr key, ODByteArray* value)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","GetEntry");

	ODBoolean	result = kODFalse;
	value->_buffer = kODNULL;
	value->_maximum = value->_length = 0;

	try {

	ODPtr buffer = kODNULL;
	ODULong size = 0;

	if (somSelf->GetType(ev) == kODNSDataTypeODValue)
	{
		size_t len = strlen(key);
		result = somSelf->GetHashTable(ev)->Find((ODUByte*)key, len, 
				&buffer, &size);
		if ( result == kODFalse )
		{
			ODValueNameSpace *parent =(ODValueNameSpace*)somSelf->GetParent(ev);
			if (parent)
			{
				if (parent->GetType(ev) == kODNSDataTypeODValue)
					result = parent->GetEntry(ev, key, value);
			}
		}
		else
		{
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

	return result;
}

//------------------------------------------------------------------------------
// ODValueNameSpace: CreateIterator
//------------------------------------------------------------------------------

SOM_Scope ODValueIterator*  SOMLINK ODValueNameSpaceCreateIterator(ODValueNameSpace *somSelf, Environment *ev)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","CreateIterator");

	ODValueIterator* theValueIterator = kODNULL;
	
	try {
	
	theValueIterator = new ODValueIterator;
	THROW_IF_NULL(theValueIterator); // check all SOM new's for nil
	theValueIterator->InitValueIterator(ev, somSelf);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }

	return theValueIterator;
}

//------------------------------------------------------------------------------
// WriteULong
//------------------------------------------------------------------------------

static void WriteULong(ODFileRefNum file, ODStorageUnitView* view,
							ODULong value)
{
	ODSLong	length ;
	ODSLong	savedLength;
	Environment* ev = somGetGlobalEnvironment ();

	length = sizeof(value);
	if (view) {
		StorageUnitViewSetValue(view, ev, length, &value);
	}
	else
	{
		savedLength = length;

#ifdef _PLATFORM_MACINTOSH_
		THROW_IF_ERROR(FSWrite(file, &length, (long*)&value));
#endif
		if (savedLength != length)
			ODSetSOMException(ev, kODErrWriteErr);
	}
}

//------------------------------------------------------------------------------
// WriteSizeAndBytes
//------------------------------------------------------------------------------

static void WriteSizeAndBytes(ODFileRefNum file, ODStorageUnitView* view,
								ODPtr bytes, ODULong length)
{
	ODSLong	savedLength;
	Environment* ev = somGetGlobalEnvironment ();

	// WRITE LENGTH OF STREAM
	WriteULong(file, view, length);

	// WRITE STREAM
	if (view)
		StorageUnitViewSetValue(view, ev, length, bytes);
	else
	{
		savedLength = length;
#ifdef _PLATFORM_MACINTOSH_
		THROW_IF_ERROR(FSWrite(file, (long *)&length, (long *)bytes));
#endif
		if (savedLength != length)
			ODSetSOMException(ev,kODErrWriteErr);
	}
}

//------------------------------------------------------------------------------
// ODValueNameSpace: Write
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceWrite(ODValueNameSpace *somSelf, Environment *ev,
		PlatformFile* file, ODStorageUnitView* view)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","Write");

	try {
	
	// WRITE NAME
	{ TempODISOStr selfName(somSelf->GetName(ev));
	  WriteSizeAndBytes(file->GetFileRefNum(), view, 
		  (ODISOStr) selfName, ODISOStrLength( (ODISOStr) selfName ));
	}
	
	// WRITE NUMENTRIES
	ODULong numEntries = somSelf->GetNumEntries(ev);
	WriteULong(file->GetFileRefNum(), view, numEntries);

	if (numEntries)
	{
		StringHashTableIterator	iter(somSelf->GetHashTable(ev));
		ODUByte*					key;
		ODULong					keyLength;
		ODPtr						value;
		ODULong					valueLength;
		
		for (iter.First(&key, &keyLength, &value, &valueLength)
				;iter.IsNotComplete()
				;iter.Next(&key, &keyLength, &value, &valueLength))
		{
			WriteSizeAndBytes(file->GetFileRefNum(), view, key, keyLength);
			WriteSizeAndBytes(file->GetFileRefNum(), view, value, valueLength);
		}
	}
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: WriteToFile
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceWriteToFile(ODValueNameSpace *somSelf, Environment *ev,
		ODByteArray* file)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","WriteToFile");

	try {

	PlatformFile*	platFile = ((PlatformFile*) file->_buffer);
        somSelf->Write(ev, platFile, (ODStorageUnitView*)kODNULL);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: WriteToStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceWriteToStorage(ODValueNameSpace *somSelf, Environment *ev,
		ODStorageUnitView* view)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","WriteToStorage");

	try {

	somSelf->Write(ev,0, view);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: Read
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceRead(ODValueNameSpace *somSelf, Environment *ev,
		PlatformFile* file, ODStorageUnitView* view)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","Read");

	try {

	{   // READ NAME-VERIFY THAT IT'S THE SAME
		ODULong nameLength = 1; 
	        ReadSize(file->GetFileRefNum(), view);

		TempODISOStr name = (ODISOStr)ODNewPtr(nameLength + 1, kDefaultHeapID);

		ReadBytes(file->GetFileRefNum(), view, name, &nameLength);
		((ODISOStr) name)[nameLength] = 0;
	
		TempODISOStr selfName(somSelf->GetName(ev)); // make sure it's deleted
		
		if ( ! ODISOStrEqual(name, selfName))
		ODSetSOMException(ev,kODErrInvalidNSName);
	}

	// READ NUM ENTRIES
	ODULong numEntries = 1; 
	ReadSize(file->GetFileRefNum(), view);

	// MORE THAN ONE ENTRY?
	if (numEntries)
	{
		ODISOStr	key = kODNULL;
		ODPtr		value = kODNULL;
		ODULong	keyLength;
		ODULong	valueLength;
		ODULong	bufferSize = UCHAR_MAX;
		ODPtr		buffer = ODNewPtr(bufferSize, kDefaultHeapID); // FOR FASTER KEY READING
		ODBoolean	allocatedKey = kODFalse;
		ODVolatile(allocatedKey);

		try {
			for (ODULong i = 0; i < numEntries; i++)
			{
				keyLength = 1; 
				ReadSize(file->GetFileRefNum(), view);
				if (keyLength > bufferSize)
				{
					key = (ODISOStr)ODNewPtr(keyLength, kDefaultHeapID);
					allocatedKey = kODTrue;
				}
				else
					key = (ODISOStr)buffer;

				ReadBytes(file->GetFileRefNum(), view, key, &keyLength);

				try {
					valueLength = 1; 
					ReadSize(file->GetFileRefNum(), view);
					value = ODNewPtr(valueLength, kDefaultHeapID);
					try {
					ReadBytes(file->GetFileRefNum(), view, value, &valueLength);
						somSelf->GetHashTable(ev)->Insert((unsigned char*)key,
						  keyLength, value, valueLength);

						if (somSelf->GetNameSpaceManager(ev)->
						    GetMaxISOStrLength(ev) < valueLength)
							somSelf->GetNameSpaceManager(ev)->
							SetMaxISOStrLength(ev, valueLength);
					
                                        }       
                                        catch (ODException _exception) {
			        		ODDisposePtr(value);
						RERAISE;
                                                SetErrorCode(kODNoError);
                                        }
                                }       
                                catch (ODException _exception) {
					if (allocatedKey)
						ODDisposePtr(key);
					RERAISE;
                                        SetErrorCode(kODNoError);
                                }

				if (allocatedKey)
					ODDisposePtr(key);
			}
			
			ODDisposePtr(buffer);
                }       
                catch (ODException _exception) {
			ODDisposePtr(buffer);
			RERAISE;
                        SetErrorCode(kODNoError);
                }
	}
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: ReadFromFile
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceReadFromFile(ODValueNameSpace *somSelf, Environment *ev,
		ODByteArray* file)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","ReadFromFile");
	
	try {
	
	PlatformFile*	platFile = ((PlatformFile*) file->_buffer);
	somSelf->Read(ev, platFile, (ODStorageUnitView*)kODNULL);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODValueNameSpace: ReadFromStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODValueNameSpaceReadFromStorage(ODValueNameSpace *somSelf, Environment *ev,
		ODStorageUnitView* view)
{
    /* ODValueNameSpaceData *somThis = ODValueNameSpaceGetData(somSelf); */
    ODValueNameSpaceMethodDebug("ODValueNameSpace","ReadFromStorage");

	try {

	somSelf->Read(ev,0, view);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}
