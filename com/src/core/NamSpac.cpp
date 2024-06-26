/* @(#)Z 1.8 com/src/core/NamSpac.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:46 (96/10/29 09:20:44) */
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
	File:		NamSpac.cpp

	Contains:	Implementation of ODNameSpace class

	Owned by:	David McCusker

	Copyright:	� 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<19>	 10/8/95	TJ		Fixes Recomended by Refball
		<18>	 9/11/95	CG		#1282719 FB2: Core error code mods.
		<17>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<16>	 7/27/95	DM		#1270320: Mem leaks: GetName() needed to
									allocate the name returned
		<15>	 7/24/95	CG		#1270320: Memory leak fixes.
		<14>	 5/25/95	jpa		Return of <12>: Added SetSynchedToEditors
									and IsSynchedToEditors.  [1240150]
		<13>	 5/17/95	TJ		Backed out changes from previous checkin.
		<11>	 5/11/95	DM		#1181622: Updated file ownership.
		<10>	  5/2/95	DM		1242453: InitNameSpace handles errors
		 <9>	  5/1/95	CG		#1241627	BB: SOM_TRY blocks needed in Core
									SOM methods
		 <8>	 4/26/95	CG		1211082 BB: 5$ Bugs need to be evaluated
									and removed from Core
		 <7>	 3/24/95	CG		#1217023 API: Write and Read use
									ODByteArray params instead of Platformfile.
		 <6>	 1/23/95	CG		#1195019: Added private getter for
									fNameSpaceManager.
		 <5>	 9/29/94	RA		1189812: Mods for 68K build.
		 <4>	 8/24/94	CG		#1181622: Updated file ownership.
		 <3>	 8/17/94	CG		#1181493: Exists no longer depends on what
									kind of name space it is.
		 <2>	  8/5/94	VL		Implemented Exists.
		 <1>	 7/29/94	CG		first checked in
		 <1>	 6/24/94	CG		first checked in
	To Do:
	In Progress:
*/

/*
 *  This file was generated by the SOM Compiler.
 *  Generated using: 
 *      SOM Emitter emitxtm.dll: somc/smmain.c
 */

#define ODNameSpace_Class_Source
#define VARIABLE_MACROS
#include <NamSpac.xih>

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _STRHSHTB_
#include "StrHshTb.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"	// Adkins -- added
#endif


SOM_Scope ODISOStr  SOMLINK ODNameSpaceGetName
		(ODNameSpace *somSelf, Environment *ev)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceGetName");
	
	try {

	return ODISOStrFromCStr(_fName); // allocate a copy
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	return kODNULL;
}

SOM_Scope ODNSTypeSpec  SOMLINK ODNameSpaceGetType
		(ODNameSpace *somSelf, Environment *ev)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceGetType");

	return _fType;
}

SOM_Scope ODNameSpace*  SOMLINK ODNameSpaceGetParent
		(ODNameSpace *somSelf, Environment *ev)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceGetParent");

	return _fParent;
}

SOM_Scope void  SOMLINK ODNameSpaceSetType
		(ODNameSpace *somSelf, Environment *ev,
	ODNSTypeSpec type)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceSetType");

	_fType = type;
}

//------------------------------------------------------------------------------
// ODNameSpace: Unregister
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceUnregister
		(ODNameSpace *somSelf, Environment *ev,
		ODISOStr key)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceUnregister");

	try {
	
        	if (_fStringHashTable)
	        {
		    size_t len = strlen(key);
		    _fStringHashTable->Remove((ODUByte*)key, len);
	        }
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }

}

//------------------------------------------------------------------------------
// ODNameSpace: Exists
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODNameSpaceExists
		(ODNameSpace *somSelf, Environment *ev,
		ODISOStr key)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceExists");
	ODBoolean	exists = kODFalse;
	
	
	try {

        	if (_fStringHashTable)
	        {
		        size_t len = strlen(key);
		        exists = _fStringHashTable->Exists( (ODUByte*)key, len );
	        }
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	
	return exists;
}

//------------------------------------------------------------------------------
// ODNameSpace: WriteToFile
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceWriteToFile
		(ODNameSpace *somSelf, Environment *ev,
		ODByteArray* file)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceWriteToFile");

}

//------------------------------------------------------------------------------
// ODNameSpace: ReadFromFile
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceReadFromFile
		(ODNameSpace *somSelf, Environment *ev,
		ODByteArray* file)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceReadFromFile");

}

//------------------------------------------------------------------------------
// ODNameSpace: WriteToStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceWriteToStorage
		(ODNameSpace *somSelf, Environment *ev,
		ODStorageUnitView* view)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceWriteToStorage");

}

//------------------------------------------------------------------------------
// ODNameSpace: ReadFromStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceReadFromStorage
		(ODNameSpace *somSelf, Environment *ev,
		ODStorageUnitView* view)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceReadFromStorage");

}

//------------------------------------------------------------------------------
// ODNameSpace: InitNameSpace
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceInitNameSpace
		(ODNameSpace *somSelf, Environment *ev,
		ODNameSpaceManager* mgr,
		ODNameSpace* parent,
		ODULong numExpectedEntries,
		ODISOStr name)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpaceInitNameSpace");

	try {
	
		/* Moved from somInit. SOM itself sets fields to zero
		_fName = kODNULL;
		_fType = kODNULL;
		_fNameSpaceManager = kODNULL;
		_fStringHashTable = kODNULL;
		_fParent = kODNULL;
		_fNumInitialEntries = 0;
		_fSynchedToEditors = kODFalse;
		*/
		somSelf->InitObject(ev);
	
		_fNameSpaceManager = mgr;
		_fParent = parent;
		_fNumInitialEntries = numExpectedEntries;

		if (name)
		{
			ODULong nameLength = ODISOStrLength(name);
			if (nameLength)
			{
				_fName = (ODISOStr)ODNewPtr(nameLength + 1, kDefaultHeapID);
				ODISOStrCopy(_fName, name);
			}
			else
				ODSetSOMException(ev, kODErrInvalidNSName);
		}
		else
			ODSetSOMException(ev, kODErrInvalidNSName);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }

}

//------------------------------------------------------------------------------
// ODNameSpace: GetHashTable
//------------------------------------------------------------------------------

SOM_Scope StringHashTable*  SOMLINK ODNameSpaceGetHashTable
		(ODNameSpace *somSelf, Environment *ev)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","GetHashTable");
	
	try {
		if (!_fStringHashTable)
		{
			_fStringHashTable = new StringHashTable(_fNumInitialEntries);
			_fStringHashTable->Initialize(0);
		}
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	
	return _fStringHashTable;
}

//------------------------------------------------------------------------------
// ODNameSpace: GetNumEntries
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODNameSpaceGetNumEntries
		(ODNameSpace *somSelf, Environment *ev)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","GetNumEntries");

	ODULong	numEntries = 0;
	
	try {
		if (_fStringHashTable)
			numEntries = _fStringHashTable->GetNumEntries();
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
	
	return numEntries;
}

//------------------------------------------------------------------------------
// ODNameSpace: GetNameSpaceManager
//------------------------------------------------------------------------------

SOM_Scope ODNameSpaceManager*  SOMLINK ODNameSpaceGetNameSpaceManager
		(ODNameSpace *somSelf, Environment *ev)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","GetNameSpaceManager");

	return _fNameSpaceManager;
}

//------------------------------------------------------------------------------
// ODNameSpace: NotifyDying
//
//	ODNameSpaceManager lets every ODNameSpace know when one of its kind
//	is dying.
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceNotifyDying
		(ODNameSpace *somSelf, Environment *ev,
		ODNameSpace* space)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","NotifyDying");

	WASSERTMSG_DEBUG(space!=somSelf, "Being notified of self dying");
	if (_fParent == space)
		_fParent = kODNULL;
}


//------------------------------------------------------------------------------
// ODNameSpace: ~ODNameSpace
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODNameSpacesomUninit(ODNameSpace *somSelf)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","ODNameSpacesomUninit");

	if (_fStringHashTable)
		ODDeleteObject( _fStringHashTable );
	
	ODDisposePtr( _fName );
	
    ODNameSpace_parent_ODObject_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODNameSpace: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODNameSpacePurge
		(ODNameSpace *somSelf, Environment *ev,
		ODSize size)
{
    /* ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf); */
    ODNameSpaceMethodDebug("ODNameSpace","Purge");

	ODUnused(size);
	return 0;
}

//------------------------------------------------------------------------------
// ODNameSpace: SetSynchedToEditors
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODNameSpaceSetSynchedToEditors
		(ODNameSpace *somSelf, Environment *ev,
		ODBoolean synched)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","SetSynchedToEditors");

	_fSynchedToEditors = synched;
}

//------------------------------------------------------------------------------
// ODNameSpace: IsSynchedToEditors
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODNameSpaceIsSynchedToEditors
		(ODNameSpace *somSelf, Environment *ev)
{
    ODNameSpaceData *somThis = ODNameSpaceGetData(somSelf);
    ODNameSpaceMethodDebug("ODNameSpace","IsSynchedToEditors");

	return _fSynchedToEditors;
}

