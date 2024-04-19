/* @(#)Z 1.7 com/src/core/ODObject.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:47 (96/10/29 09:20:59) */
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
	File:		ODObject.cp

	Contains:	Definition of class ODObject

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<12>	 8/26/95	T‚		1273191 Lets eliminate
									ODObject::IsInitialized
		<11>	 5/26/95	RR		#1251403: Multithreading naming support
		<10>	  5/4/95	eeh		1243497: make ReleaseExtension raise
									exception
		 <9>	 4/26/95	CG		1211082 BB: 5$ Bugs need to be evaluated
									and removed from Core
		 <8>	 9/14/94	jpa		ODValidatePtr --> MMValidatePtr [1186692]
		 <7>	 8/31/94	T‚		#1183129, #1183116, #1183119, #1183111:
									Lots of ErrorCode cleanup.
		 <6>	 8/17/94	jpa		Tore out THROW on SubClassResponsibility:
									we cannot throw out of SOM methods!!!
		 <5>	 8/17/94	jpa		Warn when deleting problematic object
									(block not marked as SOM object) [1181510]
		 <4>	 8/12/94	VL		Added IsEqualTo.
		 <3>	 7/21/94	VL		SubClassResponsibility should THROW instead
									of WARN.
		 <2>	  7/8/94	VL		Added warn to SubClassResponsibility.
		 <1>	 6/22/94	CG		first checked in
		 <0>	 6/21/94	SV		SOMverted
		 <9>	 5/27/94	MB		#1162181: Fixed MMM integration bug
		 <8>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <7>	 3/25/94	MB		Symantec ASLM fixes. #1150864
		 <6>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <5>	 2/25/94	T‚		comment/ifdef the validate object stuff
		 <4>	 2/22/94	CC		Moved GetFirstObject here for ASLM.
		 <3>	 2/22/94	VL		Added include for ODMemory.
		 <2>	 2/22/94	VL		Added support for object list.
		<11>	  2/8/94	T‚		Throw -> THROW & some code clean up
		<10>	 1/11/94	T‚		Init... changes
		 <9>	12/15/93	T‚		InitObject changes
		 <8>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		 <7>	11/11/93	T‚		Throw(kODErrUnsupportedExtension) in
									GetExtension
		 <6>	  8/9/93	PH		Segmentation
		 <5>	 6/16/93	VL		Moved ODRefCntObject and
									ODPersistentObject to their own files.
		 <4>	 6/10/93	NP		Changed some parameter types to ODType.
		 <3>	  5/6/93	NP		Added Initialize method.
		 <2>	 4/29/93	T‚		change names of includefiles
		 <1>	 4/26/93	RCR		first checked in

	To Do:
*/

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#define ODObject_Class_Source
#define VARIABLE_MACROS
#include <ODObject.xih>

#ifndef _EXCEPT_
#include "Except.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#pragma segment ODObject

SOM_Scope void  SOMLINK ODObjectInitObject(ODObject *somSelf, Environment *ev)
{
    //ODObjectData *somThis = ODObjectGetData(somSelf);
    ODObjectMethodDebug("ODObject","InitObject");
}


SOM_Scope ODBoolean	 SOMLINK ODObjectHasExtension
		(ODObject *somSelf, Environment *ev,
		ODType extensionName)
{
//	ODObjectData *somThis = ODObjectGetData(somSelf);
    ODObjectMethodDebug("ODObject","HasExtension");

ODUnused(extensionName);
	return kODFalse;
}


SOM_Scope ODExtension*	 SOMLINK ODObjectAcquireExtension
		(ODObject *somSelf, Environment *ev,
		ODType extensionName)
{
//	ODObjectData *somThis = ODObjectGetData(somSelf);
    ODObjectMethodDebug("ODObject","AcquireExtension");

ODUnused(extensionName);
	ODSetSOMException(ev,kODErrUnsupportedExtension);
	return kODNULL;
}

SOM_Scope void	 SOMLINK ODObjectReleaseExtension
		(ODObject *somSelf, Environment *ev,
		ODExtension* extension)
{
//  ODObjectData *somThis = ODObjectGetData(somSelf);
    ODObjectMethodDebug("ODObject","ReleaseExtension");

ODUnused(extension);
	ODSetSOMException(ev, kODErrUnsupportedExtension );
}

SOM_Scope ODSize  SOMLINK ODObjectPurge
		(ODObject *somSelf, Environment *ev,
		ODSize size)
{
//	ODObjectData *somThis = ODObjectGetData(somSelf);
    ODObjectMethodDebug("ODObject","Purge");

	return 0;
}

SOM_Scope ODBoolean  SOMLINK ODObjectIsEqualTo
		(ODObject *somSelf, Environment *ev,
		ODObject* object)
{
//	ODObjectData *somThis = ODObjectGetData(somSelf);
    ODObjectMethodDebug("ODObject","IsEqualTo");

	return (somSelf == object ? kODTrue : kODFalse);
}

SOM_Scope void  SOMLINK ODObjectSubClassResponsibility
		(ODObject *somSelf, Environment *ev)
{
//	ODObjectData *somThis = ODObjectGetData(somSelf);
    ODObjectMethodDebug("ODObject","SubClassResponsibility");

	WARNMSG(WARN_INDEX(AMSG_740),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODObjectsomInit(ODObject *somSelf)
{
	ODObjectMethodDebug("ODObject","somInit");

    ODObject_parent_SOMObject_somInit(somSelf);
    
#ifdef _PLATFORM_MACINTOSH_
    ODBlockIsObject(somSelf,kODTrue);	// Inform memory mgr this is an object
#endif
}


SOM_Scope void  SOMLINK ODObjectsomUninit(ODObject *somSelf)
{
    ODObjectMethodDebug("ODObject","somUninit");

#ifdef _PLATFORM_MACINTOSH_
#if ODDebug
	if( !ODIsBlockAnObject(somSelf) ) {
		// It's not marked in the heap as a SOM object. What's wrong?
		if( !MMValidatePtr(somSelf,kODTrue,"somUninit") )
			return;
		else if( !somIsObj(somSelf) ) {
			WARN("somUninit called on non-SOM-object %p",somSelf);
			return;
		} else
			WARN("Block %p mistakenly not marked as SOM object",somSelf);
	}
#endif // ODDebug

	ODBlockIsObject(somSelf,kODFalse);	// This is no longer an object
#endif // _PLATFORM_MACINTOSH_

    ODObject_parent_SOMObject_somUninit(somSelf);
}

SOM_Scope SOMObject*  SOMLINK somNew(M_ODObject *somSelf)
{
	// 1242632 Why do we need this method? 
	// It doesn't do anything but call its parent!
	
    /* M_ODObjectData *somThis = M_ODObjectGetData(somSelf); */
    M_ODObjectMethodDebug("M_ODObject","somNew");

    return (M_ODObject_parent_SOMClass_somNew(somSelf));
}
