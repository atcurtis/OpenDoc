/* @(#)Z 1.6 com/src/core/EmbFrItr.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:45 (96/10/29 09:20:30) */
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
	File:		EmbFrItr.cpp

	Contains:	EmbeddedFrameIterator class skeleton implementation

	Owned by:	Joshua Susser

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

 <3>	  8/3/95	RR	#1257260: Collapse B classes. Remove
				somInit methods. Don't call IsInitialized
				or SubclassResponsibility
 <2>	 1/20/95	JBS	1195017, 1205669: part API changes
 <1>	 7/20/94	JBS	first checked in
 <0>	 7/19/94	JBS	Added SubClassResponsibility.
	To Do:
*/

#define ODEmbeddedFramesIterator_Class_Source
#include <EmbFrItr.xih>

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

SOM_Scope void  SOMLINK ODEmbeddedFramesIteratorInitEmbeddedFramesIterator
		(ODEmbeddedFramesIterator *somSelf, Environment *ev,
		ODPart* part)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator",
		"InitEmbeddedFramesIterator");

	WARNMSG(WARN_INDEX(AMSG_730),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, 
		"SubClass Responsibility");
}

SOM_Scope ODFrame*  SOMLINK ODEmbeddedFramesIteratorFirst
		(ODEmbeddedFramesIterator *somSelf, Environment *ev)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator","First");

	WARNMSG(WARN_INDEX(AMSG_730),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, 
		"SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODFrame*  SOMLINK ODEmbeddedFramesIteratorNext
		(ODEmbeddedFramesIterator *somSelf, Environment *ev)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator","Next");

	WARNMSG(WARN_INDEX(AMSG_730),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, 
		"SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODBoolean  SOMLINK ODEmbeddedFramesIteratorIsNotComplete
		(ODEmbeddedFramesIterator *somSelf, Environment *ev)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator",
		"IsNotComplete");

	WARNMSG(WARN_INDEX(AMSG_730),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, 
		"SubClass Responsibility");
	return kODFalse;
}

SOM_Scope void  SOMLINK ODEmbeddedFramesIteratorPartRemoved
		(ODEmbeddedFramesIterator *somSelf, Environment *ev)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator",
		"PartRemoved");

	WARNMSG(WARN_INDEX(AMSG_730),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, 
		"SubClass Responsibility");
}

SOM_Scope ODBoolean  SOMLINK ODEmbeddedFramesIteratorIsValid
		(ODEmbeddedFramesIterator *somSelf, Environment *ev)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator","IsValid");

	WARNMSG(WARN_INDEX(AMSG_730),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, 
		"SubClass Responsibility");
	return kODFalse;
}

SOM_Scope void  SOMLINK ODEmbeddedFramesIteratorCheckValid
		(ODEmbeddedFramesIterator *somSelf, Environment *ev)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator",
		"CheckValid");

	WARNMSG(WARN_INDEX(AMSG_730),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, 
		"SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODEmbeddedFramesIteratorsomUninit
		(ODEmbeddedFramesIterator *somSelf)
{
    /* ODEmbeddedFramesIteratorData *somThis = 
		ODEmbeddedFramesIteratorGetData(somSelf); */
    ODEmbeddedFramesIteratorMethodDebug("ODEmbeddedFramesIterator","somUninit");

    ODEmbeddedFramesIterator_parent_ODObject_somUninit(somSelf);
}
