/* @(#)Z 1.11 com/src/core/RefCtObj.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:55 (96/10/29 09:21:32) */
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
	File:		RefCtObj.cpp

	Contains:	Implementation of ODRefCntObject.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<12>	10/24/95	jpa		1293441: Don't throw kODErrZeroRefCount,
									only warn. (Also added more ref-count
									debugging support.)
		<11>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<10>	 6/30/95	JP		Enabled assert in SomUninit
		 <9>	 6/28/95	RR		1242642 BB Mostly ref counting. Added class
									name to output
		 <8>	 6/22/95	jpa		Added extra warnings for releasing objects
									with zero ref-counts and for deleting
									objects with nonzero ref-counts. [1242642]
		 <7>	 5/26/95	RR		#1251403: Multithreading naming support
		 <6>	 5/25/95	jpa		Added debugging tools. [1253335]
		 <5>	 4/26/95	CG		1211082 BB: 5$ Bugs need to be evaluated
									and removed from Core
		 <4>	 3/31/95	VL		1233725: Changed THROW to SetSOMException
									in Release.
		 <3>	 1/20/95	JBS		1195017, 1205669: part API changes
		 <2>	 9/22/94	JBS		1188221: initialize fRefCount to 0
		 <1>	 6/23/94	CG		first checked in
		<11>	  2/8/94	T‚		Throw -> THROW & some code clean up
		<10>	 1/11/94	T‚		Init... changes
		 <9>	12/15/93	T‚		InitObject changes
		 <8>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		 <7>	11/24/93	VL		Commented out changes in <6>.
		 <6>	11/11/93	T‚		In destructor:	if (fRefCount>0) do warning
									assert.  Actual Throw code in comment next
									to the WASSERTM.
		 <5>	 8/13/93	VL		Check for fRefCount == 0 in Release.
		 <4>	  8/9/93	PH		Segments
		 <3>	  8/6/93	VL		Implemented GetRefCount.
		 <2>	 6/30/93	VL		RefCount should be 1 when an object is
									created.
		 <1>	 6/16/93	VL		first checked in

	To Do:
	In Progress:
		
*/

#define ODRefCntObject_Class_Source
#define VARIABLE_MACROS
#include <RefCtObj.xih>

#ifndef _EXCEPT_
#include "Except.h"
#endif	

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

#pragma segment ODRefCntObject


#ifdef DEBUG
/* Debugging feature: If you set gTrackObj (presumably using a debugger) 
   to point to an object you're interested in, info will be written via 
   somPrintf whenever that object's reference count changes. This includes 
   the current reference count.
   If you enter a class name in gTrackClass, the ref-counts of all objects 
   of that class will be tracked & logged in the same way. */
	
// Set to true (1) to see creation/deletion of all ODRefCntObject
// --------------------------------------------------------
static ODBoolean gTrackAll = kODFalse;	
// --------------------------------------------------------

// Edit at runtime to track an individual ODRefCntObject
// --------------------------------------------------------
static ODRefCntObject *gTrackObj = kODNULL; 
// --------------------------------------------------------

// Edit at runtime to track all objs of a ODRefCntObject subclass
// --------------------------------------------------------
char gTrackClass[64] = ""; 
// --------------------------------------------------------

static void LogRefCount( ODRefCntObject *obj, ODBoolean increment, 
					ODULong refCount )
{
	somPrintf("%s %s %p: RefCount now %d\n",
		increment ? "+++ IncrementRefCount of" :"--- Release",
		obj->somGetClassName(),
		obj,
		refCount );
}
#endif // DEBUG


//----------------------------------------------------------------------
//	InitRefCntObject
//----------------------------------------------------------------------
SOM_Scope void  SOMLINK ODRefCntObjectInitRefCntObject
		(ODRefCntObject *somSelf, Environment *ev)
{
    ODRefCntObjectData *somThis = ODRefCntObjectGetData(somSelf);
    ODRefCntObjectMethodDebug("ODRefCntObject","InitRefCntObject");

	/* Moved from somInit. SOM itself sets fields to zero
	_fRefCount = 0;
	*/
	somSelf->InitObject(ev);
	
	_fRefCount = 1;

#ifdef DEBUG

	char* thisClass = somSelf->somGetClassName();
	if ( 	gTrackAll || 
		somSelf == gTrackObj || 
		(gTrackClass[0] && strcmp(thisClass,gTrackClass)==0)
	)
		LogRefCount(somSelf, kODTrue, _fRefCount);

#endif // DEBUG
}

//----------------------------------------------------------------------
//	Acquire
//----------------------------------------------------------------------
SOM_Scope void  SOMLINK ODRefCntObjectAcquire
		(ODRefCntObject *somSelf, Environment *ev)
{
    ODRefCntObjectData *somThis = ODRefCntObjectGetData(somSelf);
    ODRefCntObjectMethodDebug("ODRefCntObject","Acquire");

	_fRefCount++;

#ifdef DEBUG

	char* thisClass = somSelf->somGetClassName();
	if ( somSelf == gTrackObj || 
	     (gTrackClass[0] && strcmp(thisClass,gTrackClass)==0)
	)
		LogRefCount(somSelf, kODTrue, _fRefCount);

#endif // DEBUG
}

//----------------------------------------------------------------------
//	Release
//----------------------------------------------------------------------
SOM_Scope void  SOMLINK ODRefCntObjectRelease
		(ODRefCntObject *somSelf, Environment *ev)
{
    ODRefCntObjectData *somThis = ODRefCntObjectGetData(somSelf);
    ODRefCntObjectMethodDebug("ODRefCntObject","Release");

	if (_fRefCount == 0) {
	   char* refwarn = getenv("ODREFCOUNTWARNINGS");
	   if ( (refwarn != NULL) && (*refwarn != 0) ) {
		WARNMSG(WARN_INDEX(AMSG_770),"%s %p released one too many times", 
					somSelf->somGetClassName(),somSelf);
		ODSetSOMException(ev, kODErrZeroRefCount);
	   }
	} else
		--_fRefCount;

#ifdef DEBUG

	char* thisClass = somSelf->somGetClassName();
	if ( somSelf == gTrackObj || 
	     (gTrackClass[0] && strcmp(thisClass,gTrackClass)==0)
	)
		LogRefCount(somSelf, kODFalse, _fRefCount);

#endif // DEBUG
}

//----------------------------------------------------------------------
//	GetRefCount
//----------------------------------------------------------------------
SOM_Scope ODULong  SOMLINK ODRefCntObjectGetRefCount
		(ODRefCntObject *somSelf, Environment *ev)
{
    ODRefCntObjectData *somThis = ODRefCntObjectGetData(somSelf);
    ODRefCntObjectMethodDebug("ODRefCntObject","GetRefCount");

	return _fRefCount;
}

//----------------------------------------------------------------------
//	somUninit
//----------------------------------------------------------------------
SOM_Scope void  SOMLINK ODRefCntObjectsomUninit
		(ODRefCntObject *somSelf)
{
    ODRefCntObjectData *somThis = ODRefCntObjectGetData(somSelf);
    ODRefCntObjectMethodDebug("ODRefCntObject","somUninit");

#ifdef DEBUG

	char* thisClass = somSelf->somGetClassName();
	if ( _fRefCount!=0 || 
		gTrackAll || 
		somSelf == gTrackObj || 
			(gTrackClass[0] && strcmp(thisClass,gTrackClass)==0)
	) {
		somPrintf("XXX Deleted %s %p (with ref-count of %d)\n",
			somSelf->somGetClassName(), somSelf, _fRefCount);

		// cleanup individual object reference (its no longer valid)
		if ( somSelf == gTrackObj ) gTrackObj = 0;

		if( _fRefCount != 0 ) {
	   	  char* refwarn = getenv("ODREFCOUNTWARNINGS");
	   	  if ( (refwarn != NULL) && (*refwarn != 0) ) {
			WARNMSG_DEBUG(WARN_INDEX(-1),
				"%s at %p deleted with refcount == %d", 
				somSelf->somGetClassName(), 
				somSelf, 
				_fRefCount);
	   	  }
		}
	  }

#endif // DEBUG
}
