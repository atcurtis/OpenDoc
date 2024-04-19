/* @(#)Z 1.6 com/src/layout/FrFaItr.cpp, odlayout, od96os2, odos29646d 96/11/15 15:25:24 (96/07/15 18:32:20) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odlayout
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
   File:       FrFaItr.cpp

   Contains:   Implementation of class ODFrameFacetIterator

   Owned by:   Joshua Susser

   Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <6>      8/7/95    JBS     1260631added ASSERT for frame in Init
        <5>      8/3/95    RR      #1257260: Collapse B classes. Remove
                                   somInit methods. Don't call IsInitialized
                                   or SubclassResponsibility
        <4>     5/26/95    RR      #1251403: Multithreading naming support
        <3>      5/2/95    JBS     1244569 Layout iterators need SOM exception
                                   handling; 1243361 FrameFacetIterator should
                                   throw kODErrIteratorNotInitialized
        <2>     4/27/95    JBS     1209506 $5 comments eliminated
        <1>     6/29/94    JBS     first checked in
        <0>     6/28/94    SV      SOMverted
*/

#define ODFrameFacetIterator_Class_Source

#ifndef SOM_ODFrameFacetIterator_xih
#define VARIABLE_MACROS
#include <FrFaItr.xih>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ORDCOLL_
#include <OrdColl.h>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODFrFaItr
#endif

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifdef _FACET_DEBUG_
#include <qprintf.h>
#endif

SOM_Scope void  SOMLINK ODFrameFacetIteratorsomUninit(ODFrameFacetIterator *somSelf)
{
    ODFrameFacetIteratorData *somThis = ODFrameFacetIteratorGetData(somSelf);
    ODFrameFacetIteratorMethodDebug("ODFrameFacetIterator","somUninit");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrameFacetIterator::somUninit()\n");
#endif
   delete _fIter;

    ODFrameFacetIterator_parents_somUninit(somSelf);

}

SOM_Scope void  SOMLINK ODFrameFacetIteratorInitFrameFacetIterator(ODFrameFacetIterator *somSelf, Environment *ev,
       ODFrame* frame)
{
    ODFrameFacetIteratorData *somThis = ODFrameFacetIteratorGetData(somSelf);
    ODFrameFacetIteratorMethodDebug("ODFrameFacetIterator","InitFrameFacetIterator");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrameFacetIterator::InitFrameFacetIterator()\n");
#endif
   SOM_TRY
       ASSERT(frame, kODErrIllegalNullFrameInput);

       /* Moved from somInit. SOM itself sets fields to zero
       _fFrame = kODNULL;
       _fIter = kODNULL;
       */
       somSelf->InitObject(ev);

       _fFrame = frame;
   SOM_CATCH_ALL
   SOM_ENDTRY
}

SOM_Scope ODFacet*  SOMLINK ODFrameFacetIteratorFirst(ODFrameFacetIterator *somSelf, Environment *ev)
{
    ODFrameFacetIteratorData *somThis = ODFrameFacetIteratorGetData(somSelf);
    ODFrameFacetIteratorMethodDebug("ODFrameFacetIterator","First");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrameFacetIterator::First()\n");
#endif
   SOM_TRY
       if ( !_fIter )
           _fIter = new OrderedCollectionIterator(_fFrame->GetFacets(ev));
       THROW_IF_NULL(_fIter);

       return (ODFacet*) _fIter->First();
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODNULL;
}

SOM_Scope ODFacet*  SOMLINK ODFrameFacetIteratorNext(ODFrameFacetIterator *somSelf, Environment *ev)
{
    ODFrameFacetIteratorData *somThis = ODFrameFacetIteratorGetData(somSelf);
    ODFrameFacetIteratorMethodDebug("ODFrameFacetIterator","Next");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrameFacetIterator::Next()\n");
#endif
   SOM_TRY
       if ( !_fIter )
           THROW(kODErrIteratorNotInitialized);

       return (ODFacet*) _fIter->Next();
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODNULL;
}

SOM_Scope ODBoolean  SOMLINK ODFrameFacetIteratorIsNotComplete(ODFrameFacetIterator *somSelf, Environment *ev)
{
    ODFrameFacetIteratorData *somThis = ODFrameFacetIteratorGetData(somSelf);
    ODFrameFacetIteratorMethodDebug("ODFrameFacetIterator","IsNotComplete");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrameFacetIterator::IsNotComplete()\n");
#endif
   SOM_TRY
       if ( !_fIter )
           THROW(kODErrIteratorNotInitialized);

       return _fIter->IsNotComplete();
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODNULL;
}
