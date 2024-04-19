//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.5 com/src/utils/TempIter.cpp, odutils, od96os2, odos29646d 8/19/96 11:50:41 [ 11/15/96 15:28:56 ]
/*
	File:		TempIter.cpp

	Contains:	Template utilities for OpenDoc iterators

	Owned by:	Jens Alfke

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

*/

#define _OD_DONT_IMPORT_CPP_

#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>
#endif

#ifndef _TEMPITER_
#include "TempIter.h"
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODFacetIterator_xh
#include <FacetItr.xh>
#endif

#ifndef SOM_ODEmbeddedFramesIterator_xh
#include <EmbFrItr.xh>
#endif

#ifndef SOM_ODFrameFacetIterator_xh
#include <FrFaItr.xh>
#endif

#ifdef _PLATFORM_WIN32_
#ifndef SOM_ODPlatformCanvas_xh
#include <PlatCanv.xh>
#endif
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODWindowIterator_xh
#include <WinIter.xh>
#endif

#ifdef _OD_IMPL_
#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>
#endif

#ifndef SOM_ODDragItemIterator_xh
#include <DgItmIt.xh>
#endif
#endif

#ifndef SOM_ODFocusSet_xh
#include <FocusSet.xh>
#endif

#ifndef SOM_ODFocusSetIterator_xh
#include <FocusItr.xh>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

//=====================================================================================
// TempODFacetIterator
//=====================================================================================

WIN32_DLLEXPORT TempODFacetIterator::TempODFacetIterator( Environment *ev, ODFacet *fa,
										  ODTraversalType tt, ODSiblingOrder so )
	:BaseTempObj(),
	 fEv(ev)
{
	fObj = fa->CreateFacetIterator(ev,tt,so);
	this->First();
}


WIN32_DLLEXPORT ODFacet*
TempODFacetIterator::First( )
{
	return fCurrent = ((ODFacetIterator*)fObj)->First(fEv);
}


WIN32_DLLEXPORT ODFacet*
TempODFacetIterator::Next( )
{
	return fCurrent = ((ODFacetIterator*)fObj)->Next(fEv);
}


WIN32_DLLEXPORT ODBoolean
TempODFacetIterator::IsNotComplete( )
{
	return ((ODFacetIterator*)fObj)->IsNotComplete(fEv);
}


//=====================================================================================
// TempODFrameFacetIterator
//=====================================================================================

WIN32_DLLEXPORT TempODFrameFacetIterator::TempODFrameFacetIterator( Environment *ev, ODFrame *fr )
	:BaseTempObj(),
	 fEv(ev)
{
	fObj = fr->CreateFacetIterator(ev);
	this->First();
}


WIN32_DLLEXPORT ODFacet*
TempODFrameFacetIterator::First( )
{
	return fCurrent = ((ODFrameFacetIterator*)fObj)->First(fEv);
}


WIN32_DLLEXPORT ODFacet*
TempODFrameFacetIterator::Next( )
{
	return fCurrent = ((ODFrameFacetIterator*)fObj)->Next(fEv);
}


WIN32_DLLEXPORT ODBoolean
TempODFrameFacetIterator::IsNotComplete( )
{
	return ((ODFrameFacetIterator*)fObj)->IsNotComplete(fEv);
}


//=====================================================================================
// TempODEmbeddedFramesIterator
//=====================================================================================

WIN32_DLLEXPORT TempODEmbeddedFramesIterator::TempODEmbeddedFramesIterator( Environment *ev,
															ODPart *part, ODFrame *fr )
	:BaseTempObj(),
	 fEv(ev)
{
	fObj = part->CreateEmbeddedFramesIterator(ev,fr);
	this->First();
}


WIN32_DLLEXPORT ODFrame*
TempODEmbeddedFramesIterator::First( )
{
	return fCurrent = ((ODEmbeddedFramesIterator*)fObj)->First(fEv);
}


WIN32_DLLEXPORT ODFrame*
TempODEmbeddedFramesIterator::Next( )
{
	return fCurrent = ((ODEmbeddedFramesIterator*)fObj)->Next(fEv);
}


WIN32_DLLEXPORT ODBoolean
TempODEmbeddedFramesIterator::IsNotComplete( )
{
	return ((ODEmbeddedFramesIterator*)fObj)->IsNotComplete(fEv);
}


//=====================================================================================
// TempODWindowIterator
//=====================================================================================

WIN32_DLLEXPORT TempODWindowIterator::TempODWindowIterator( Environment *ev, ODWindowState *ws )
	:BaseTempObj(),
	 fEv(ev)
{
	fObj = ws->CreateWindowIterator(ev);
	this->First();
}


WIN32_DLLEXPORT ODWindow*
TempODWindowIterator::First( )
{
	return fCurrent = ((ODWindowIterator*)fObj)->First(fEv);
}


WIN32_DLLEXPORT ODWindow*
TempODWindowIterator::Next( )
{
	return fCurrent = ((ODWindowIterator*)fObj)->Next(fEv);
}


WIN32_DLLEXPORT ODBoolean
TempODWindowIterator::IsNotComplete( )
{
	return ((ODWindowIterator*)fObj)->IsNotComplete(fEv);
}


//=====================================================================================
// TempODDragItemIterator
//=====================================================================================

#ifdef _OD_IMPL_

TempODDragItemIterator::TempODDragItemIterator( Environment *ev, ODDragAndDrop *d )
	:BaseTempObj(),
	 fEv(ev)
{
	fObj = d->GetDragItemIterator(ev);
	this->First();
}


ODStorageUnit*
TempODDragItemIterator::First( )
{
	return fCurrent = ((ODDragItemIterator*)fObj)->First(fEv);
}


ODStorageUnit*
TempODDragItemIterator::Next( )
{
	return fCurrent = ((ODDragItemIterator*)fObj)->Next(fEv);
}


ODBoolean
TempODDragItemIterator::IsNotComplete( )
{
	return ((ODDragItemIterator*)fObj)->IsNotComplete(fEv);
}

#endif /*_OD_IMPL_*/


//=====================================================================================
// TempODFocusSetIterator
//=====================================================================================

WIN32_DLLEXPORT TempODFocusSetIterator::TempODFocusSetIterator( Environment *ev, ODFocusSet *fs )
	:BaseTempObj(),
	 fEv(ev)
{
	fObj = fs->CreateIterator(ev);
	this->First();
}


WIN32_DLLEXPORT ODTypeToken
TempODFocusSetIterator::First( )
{
	return fCurrent = ((ODFocusSetIterator*)fObj)->First(fEv);
}


WIN32_DLLEXPORT ODTypeToken
TempODFocusSetIterator::Next( )
{
	return fCurrent = ((ODFocusSetIterator*)fObj)->Next(fEv);
}


WIN32_DLLEXPORT ODBoolean
TempODFocusSetIterator::IsNotComplete( )
{
	return ((ODFocusSetIterator*)fObj)->IsNotComplete(fEv);
}


//=====================================================================================
// TempODTypeListIterator
// 		ODTypeListIterator returns copies of the data, which must be disposed.
//=====================================================================================

WIN32_DLLEXPORT TempODTypeListIterator::TempODTypeListIterator( Environment *ev, ODTypeList *t )
	:BaseTempObj(),
	 fEv(ev),
	 fCurrent(kODNULL)
{
	fObj = t->CreateTypeListIterator(ev);
	this->First();
}


WIN32_DLLEXPORT TempODTypeListIterator::~TempODTypeListIterator( )
{
	ODDisposePtr(fCurrent); fCurrent = kODNULL;
}


WIN32_DLLEXPORT ODType
TempODTypeListIterator::First( )
{
	ODDisposePtr(fCurrent); fCurrent = kODNULL;
	return fCurrent = ((ODTypeListIterator*)fObj)->First(fEv);
}


WIN32_DLLEXPORT ODType
TempODTypeListIterator::Next( )
{
	ODDisposePtr(fCurrent); fCurrent = kODNULL;
	return fCurrent = ((ODTypeListIterator*)fObj)->Next(fEv);
}


WIN32_DLLEXPORT ODBoolean
TempODTypeListIterator::IsNotComplete( )
{
	return ((ODTypeListIterator*)fObj)->IsNotComplete(fEv);
}
