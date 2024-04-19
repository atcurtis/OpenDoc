/* @(#)Z 1.7 com/src/layout/FacetItr.cpp, odlayout, od96os2, odos29646d 96/11/15 15:25:23 (96/07/15 18:32:09) */
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
/*  IBM Change History (most recent first):                         */
/*   20470 03/19/96 tmw/SA Apple DR4 and common Windows/OS2         */
/********************************************************************/
/*
   File:       FacetM.cpp

   Contains:   Implementation of class ODFacetIterator

   Owned by:   Joshua Susser

   Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <5>      8/3/95    RR      #1257260: Collapse B classes. Remove
                                   somInit methods. Don't call IsInitialized
                                   or SubclassResponsibility
        <4>     5/26/95    RR      #1251403: Multithreading naming support
        <3>     5/25/95    jpa     List.h --> LinkList.h [1253324]
        <2>      5/2/95    JBS     1244569 Layout iterators need SOM exception
                                   handling; 1243361 FrameFacetIterator should
                                   throw kODErrIteratorNotInitialized
        <1>     6/29/94    JBS     first checked in
        <0>     6/29/94    SV      SOMverted

   In Progress:

*/

#define ODFacetIterator_Class_Source

#ifndef SOM_ODFacetIterator_xih
#define VARIABLE_MACROS
#include <FacetItr.xih>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef _FACETNOD_
#include "FacetNod.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _NODE_
#include "Node.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODFacet
#endif

#ifdef _FACET_DEBUG_
#include <qprintf.h>
#endif

SOM_Scope void  SOMLINK ODFacetIteratorsomUninit(ODFacetIterator *somSelf)
{
    ODFacetIteratorData *somThis = ODFacetIteratorGetData(somSelf);
    ODFacetIteratorMethodDebug("ODFacetIterator","somUninit");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacetIterator::somUninit()\n");
#endif

   delete _fNodeTraverser;
    ODFacetIterator_parents_somUninit(somSelf);
}

SOM_Scope void  SOMLINK ODFacetIteratorInitFacetIterator(ODFacetIterator *somSelf, Environment *ev,
       ODFacet* facet,
       ODTraversalType traversalType,
       ODSiblingOrder siblingOrder)
{
    ODFacetIteratorData *somThis = ODFacetIteratorGetData(somSelf);
    ODFacetIteratorMethodDebug("ODFacetIterator","ODFacetIteratorInitFacetIterator");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacetIterator::InitFacetIterator()\n");
#endif
   SOM_TRY

       /* Moved from somInit. SOM itself sets fields to zero
       _fFacet = kODNULL;
       _fNodeTraverser = kODNULL;
       */
       somSelf->InitObject(ev);

       _fFacet = facet;
       _fTraversalType = traversalType;
       _fSiblingOrder = siblingOrder;
   SOM_CATCH_ALL
   SOM_ENDTRY
}

SOM_Scope ODFacet*  SOMLINK ODFacetIteratorFirst(ODFacetIterator *somSelf, Environment *ev)
{
    ODFacetIteratorData *somThis = ODFacetIteratorGetData(somSelf);
    ODFacetIteratorMethodDebug("ODFacetIterator","First");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacetIterator::First()\n");
#endif

   SOM_TRY
       if (!_fNodeTraverser)
       {
           _fNodeTraverser = new NodeTraverser(_fFacet->GetNode(ev),
                                               _fTraversalType, _fSiblingOrder);
           THROW_IF_NULL(_fNodeTraverser);
       }

       FacetNode* node = (FacetNode*) _fNodeTraverser->First();
       return node ? node->GetFacet() : (ODFacet*)kODNULL;
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODNULL;
}

SOM_Scope ODFacet*  SOMLINK ODFacetIteratorNext(ODFacetIterator *somSelf, Environment *ev)
{
    ODFacetIteratorData *somThis = ODFacetIteratorGetData(somSelf);
    ODFacetIteratorMethodDebug("ODFacetIterator","Next");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacetIterator::Next()\n");
#endif
   SOM_TRY
       if ( !_fNodeTraverser )
           THROW(kODErrIteratorNotInitialized);

       FacetNode* node = (FacetNode*) _fNodeTraverser->Next();
       return node ? node->GetFacet() : (ODFacet*)kODNULL;
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODNULL;
}

SOM_Scope void  SOMLINK ODFacetIteratorSkipChildren(ODFacetIterator *somSelf, Environment *ev)
{
    ODFacetIteratorData *somThis = ODFacetIteratorGetData(somSelf);
    ODFacetIteratorMethodDebug("ODFacetIterator","SkipChildren");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacetIterator::SkipChildren()\n");
#endif
   SOM_TRY
       if ( !_fNodeTraverser )
           THROW(kODErrIteratorNotInitialized);

       _fNodeTraverser->SkipChildren();
   SOM_CATCH_ALL
   SOM_ENDTRY
}

SOM_Scope ODBoolean  SOMLINK ODFacetIteratorIsNotComplete(ODFacetIterator *somSelf, Environment *ev)
{
    ODFacetIteratorData *somThis = ODFacetIteratorGetData(somSelf);
    ODFacetIteratorMethodDebug("ODFacetIterator","IsNotComplete");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFacetIterator::IsNotComplete()\n");
#endif
   SOM_TRY
       if ( !_fNodeTraverser )
           THROW(kODErrIteratorNotInitialized);

       return _fNodeTraverser->IsNotComplete();
   SOM_CATCH_ALL
   SOM_ENDTRY
   return kODNULL;
}
