/* @(#)Z 1.12 com/src/core/PstObj.cpp, odcore, od96os2, odos29646d 96/11/15 15:29:16 (96/10/29 09:21:27) */
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
	File:		PstObj.cpp

	Contains:	Implementation of ODPersistentObject.

	Owned by:	Vincent Lo, Tantek ‚elik

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<19>	 10/8/95	TJ		Fixes Recomended by Refball
		<18>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<17>	 4/16/95	VL		1230084: Moved so-called standard
									properties to POUtils.
		<16>	 4/14/95	T‚		#1235279 BB: InfoUtil & StdTypIO functions
									should take Environment* and SU* when
									possible
		<15>	  4/7/95	EL		Use ODGetTime_TProp/ODSetTime_TProp for
									document exchange.
		<14>	 2/28/95	VL		1194656: Check for existence before adding
									property or value.
		<13>	  2/7/95	CC		1190870: CloneInto() also copies creation
									date property.
		<12>	 1/26/95	eeh		1214080: OpenDoc really uses kODMacIText,
									not kODIntlText.
		<11>	 1/19/95	VL		1190305: Make sure that all standard
									properties are present.
		<10>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <9>	 9/26/94	JBS		#1188981 - support nonpersistent frames
		 <8>	  9/9/94	CG		#1183531: Commented out NULL storage unit
									reference warning in destructor.
		 <7>	 8/31/94	VL		1183174: Changed implementation of
									CloneInto to not rely on SU::CloneInto.
		 <6>	 8/29/94	T‚		#1183567 StdTypIO routine implementation &
									fixup
		 <5>	 8/26/94	VL		1183174: Added implementation to CloneInto.
		 <4>	 8/16/94	VL		1143605: Added CloneInto.
		 <3>	 8/12/94	NP		1180762-commented out calls to GetUserName
									(they were unused and the interface has
									changed.).
		 <2>	 7/18/94	T‚		moved code over for somUninit method
		 <1>	 6/23/94	CG		first checked in
		 <0>	 6/22/94	SV		SOMverted
		 <6>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <5>	 3/28/94	CG		1153547: Use new method
									ODSession::GetUserName to set up user
									name in Info dlg.
		 <4>	 3/18/94	CC		Value type of user property changed from
									kXMPAppleTEXT to kODASCIIText (but should
									be international text!).  (1151636)
		 <3>	  3/1/94	JA		Include ODMemory.h.
		 <2>	 2/28/94	CG		(no bug #) Changed calls to HLock and
									HUnlock to XMPLockHandle and
									XMPUnlockHandle.
		<13>	 1/28/94	CC		Moved release of fSU from ReleaseAll() into
									destructor.
		<12>	 1/27/94	VL		Added ReleaseAll.
		<11>	 1/11/94	T‚		Init... changes
		<10>	12/15/93	T‚		InitObject changes
		 <9>	12/14/93	EL		Use text instead of Str255.
		 <8>	12/14/93	T‚		more InitPersistentObject changes.
									Internalize, Initialize went away.
		 <7>	12/13/93	T‚		InitPersistentObject changes
		 <6>	12/11/93	T‚		fixed modified by user code in
									InitializePersistent and Externalize
		 <5>	 12/8/93	T‚		added code to update ModUser property
		 <4>	11/18/93	T‚		add functionality to InitializePersistent &
									Externalize to do Creation, Mod dates and
									ModUser
		 <3>	  8/9/93	PH		Segments
		 <2>	 6/28/93	PH		Initialize fID in constructor
		 <1>	 6/16/93	VL		first checked in

	To Do:
*/

#define ODPersistentObject_Class_Source
#define VARIABLE_MACROS
#include <PstObj.xih>

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif	

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif	

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif	

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif	

#ifndef _EXCEPT_
#include <Except.h>
#endif	

#ifndef SOM_Module_OpenDoc_Errors_defined
#include <ErrorDef.xh>
#endif	

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef __TIME_H__
#include <time.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __RESOURCES__
#include <Resources.h>
#endif
#endif // _PLATFORM_MACINTOSH_


#pragma segment ODPersistentObject

SOM_Scope void	 SOMLINK ODPersistentObjectInitPersistentObject
		(ODPersistentObject *somSelf, Environment *ev,
		ODStorageUnit* storageUnit)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","InitPersistentObject");

	/* Moved from somInit. SOM itself sets fields to zero
    _fSU = kODNULL;
    _fID = 0;
	*/
	somSelf->InitRefCntObject(ev);

	if ( storageUnit != kODNULL )
	{
		_fSU = storageUnit;
		_fID = _fSU->GetID(ev);
	}
}

SOM_Scope void	 SOMLINK ODPersistentObjectInitPersistentObjectFromStorage
		(ODPersistentObject *somSelf, Environment *ev,
		ODStorageUnit* storageUnit)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","InitPersistentObjectFromStorage");

	/* Moved from somInit. SOM itself sets fields to zero
    _fSU = kODNULL;
    _fID = 0;
	*/
	somSelf->InitRefCntObject(ev);

	// defect 22563, better check to make sure su is valid before 
 	// calling one of its methods!
	if (storageUnit) {
		_fSU = storageUnit;
		_fID = _fSU->GetID(ev);
	}
}

SOM_Scope void	 SOMLINK ODPersistentObjectReleaseAll
		(ODPersistentObject *somSelf, Environment *ev)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","ReleaseAll");
}

SOM_Scope void	 SOMLINK ODPersistentObjectExternalize
		(ODPersistentObject *somSelf, Environment *ev)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","Externalize");
}

SOM_Scope ODStorageUnit*  SOMLINK ODPersistentObjectGetStorageUnit
		(ODPersistentObject *somSelf, Environment *ev)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","GetStorageUnit");

	return _fSU;
}


SOM_Scope ODID 	 SOMLINK ODPersistentObjectGetID
		(ODPersistentObject *somSelf, Environment *ev)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","GetID");

	return _fID;
}

SOM_Scope void  SOMLINK ODPersistentObjectCloneInto
		(ODPersistentObject *somSelf, Environment *ev,
		ODDraftKey key,
		ODStorageUnit* toSU,
		ODFrame* scopeFrame)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","CloneInto");
}

SOM_Scope void  SOMLINK ODPersistentObjectsomUninit
		(ODPersistentObject *somSelf)
{
    ODPersistentObjectData *somThis = ODPersistentObjectGetData(somSelf);
    ODPersistentObjectMethodDebug("ODPersistentObject","somUninit");

	Environment     *ev = somGetGlobalEnvironment();
	if (_fSU)
		_fSU->Release(ev);
}
