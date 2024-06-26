/* @(#) 1.10 com/src/storage/LkSrcItr.cpp, oddataxfer, od96os2, odos29646d 10/31/96 07:56:38 [11/15/96 15:26:03] */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: oddataxfer
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
        File:           LkSrcItr.cpp

        Contains:       Implementation of class ODLinkSourceIterator


        Owned by:       Craig Carper

        Copyright:      � 1994, 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                 <6>     6/25/96        DJ              23458: Enable compile on AIX platform
                 <5>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                 <4>     5/26/95        VL              1251403: Multithreading naming support.
                 <3>     5/18/95        CC              1249197: Renamed InitODLinkSourceIterator
                                                                        to InitLinkSourceIterator.
                 <2>     4/11/95        CC              1236452: Added SOM_CATCH to
                                                                        InitODLinkSourceIterator.
                                                                        Changed parent method calls from verbose to
                                                                        simple form (parent_method).
                 <1>     8/11/94        CC              first checked in

        To Do:
*/

#define VARIABLE_MACROS

#define ODLinkSourceIterator_Class_Source
#include <LkSrcItr.xih>

#ifndef SOM_ODObject_xh
#include <ODObject.xh>
#endif


#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_WIN32_)
#pragma segment ODLinkSourceIterator
#endif

//==============================================================================
// ODLinkSourceIterator
//==============================================================================

//------------------------------------------------------------------------------
// ODLinkSourceIterator: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceIteratorsomUninit(ODLinkSourceIterator *somSelf)
{
    /* ODLinkSourceIteratorData *somThis = ODLinkSourceIteratorGetData(somSelf); */
    ODLinkSourceIteratorMethodDebug("ODLinkSourceIterator","somUninit");

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODLinkSourceIterator: InitLinkSourceIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceIteratorInitLinkSourceIterator(ODLinkSourceIterator *somSelf, Environment *ev)
{
    /* ODLinkSourceIteratorData *somThis = ODLinkSourceIteratorGetData(somSelf); */
    ODLinkSourceIteratorMethodDebug("ODLinkSourceIterator","InitLinkSourceIterator");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        somSelf->InitObject(ev);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODLinkSourceIterator: IsNotComplete
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkSourceIteratorIsNotComplete(ODLinkSourceIterator *somSelf, Environment *ev)
{
    /* ODLinkSourceIteratorData *somThis = ODLinkSourceIteratorGetData(somSelf); */
    ODLinkSourceIteratorMethodDebug("ODLinkSourceIterator","IsNotComplete");

        WARNMSG_DEBUG(WARN_INDEX(-1),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODFalse;
}

//------------------------------------------------------------------------------
// ODLinkSourceIterator: First
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK ODLinkSourceIteratorFirst(ODLinkSourceIterator *somSelf, Environment *ev)
{
    /* ODLinkSourceIteratorData *somThis = ODLinkSourceIteratorGetData(somSelf); */
    ODLinkSourceIteratorMethodDebug("ODLinkSourceIterator","First");

        WARNMSG_DEBUG(WARN_INDEX(-1),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

//------------------------------------------------------------------------------
// ODLinkSourceIterator: Next
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK ODLinkSourceIteratorNext(ODLinkSourceIterator *somSelf, Environment *ev)
{
    /* ODLinkSourceIteratorData *somThis = ODLinkSourceIteratorGetData(somSelf); */
    ODLinkSourceIteratorMethodDebug("ODLinkSourceIterator","Next");

        WARNMSG_DEBUG(WARN_INDEX(-1),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}
