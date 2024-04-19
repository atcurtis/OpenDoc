/* @(#)Z 1.8 com/src/storage/PfTLItr.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:26:04 (96/10/28 13:39:55) */

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
        File:           PfTLItr.cpp

        Contains:       Implementation of class ODPlatformTypeListIterator.

        Owned by:       Craig Carper

        Copyright:      © 1994, 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                 <6>     6/25/96        DJ              23458: Enable compile on AIX platform
                 <5>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                 <4>     7/20/95        CC              1254451: IsNotComplete fails if First or
                                                                        Next has not yet been called.
                 <3>     5/26/95        VL              1251403: Multithreading naming support.
                 <2>     4/11/95        CC              1236452: InitODPlatformTypeListIterator:
                                                                        Added SOM_CATCH and throw if underlying
                                                                        interator can't be created.
                                                                        Changed parent method calls from verbose to
                                                                        simple form (parent_method).
                 <1>     6/23/94        CC              first checked in
                 <0>     6/23/94        SV              SOMverted
                 <2>      4/7/94        JA              Added missing #include (1147588)
                 <1>      4/4/94        CC              first checked in
                 <0>      4/5/94        CC              Code extracted from TranslaM.cpp (1153046)

        To Do:
*/

#define VARIABLE_MACROS

#define ODPlatformTypeListIterator_Class_Source
#include <PfTLItr.xih>

#ifndef SOM_ODPlatformTypeList_xh
#include <PfTypLs.xh>
#endif

#ifndef SOM_ODObject_xh
#include <ODObject.xh>
#endif

#ifndef _ORDCOLL_
#include <OrdColl.h>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _PLATFORM_OS2_
#pragma segment ODPlatformTypeList
#endif

//==============================================================================
// Class ODPlatformTypeListIterator
//==============================================================================

//------------------------------------------------------------------------------
// ODPlatformTypeListIterator: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPlatformTypeListIteratorsomUninit(ODPlatformTypeListIterator *somSelf)
{
    ODPlatformTypeListIteratorData *somThis = ODPlatformTypeListIteratorGetData(somSelf);
    ODPlatformTypeListIteratorMethodDebug("ODPlatformTypeListIterator","somUninit");

        delete _fIterator;

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODPlatformTypeListIterator: InitPlatformTypeListIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPlatformTypeListIteratorInitPlatformTypeListIterator(ODPlatformTypeListIterator *somSelf, Environment *ev,
                ODPlatformTypeList* typeList)
{
    ODPlatformTypeListIteratorData *somThis = ODPlatformTypeListIteratorGetData(somSelf);
    ODPlatformTypeListIteratorMethodDebug("ODPlatformTypeListIterator","ODPlatformTypeListIteratorInitPlatformTypeListIterator");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        /* Moved from somInit. SOM itself sets fields to zero
        _fPlatformTypeList = kODNULL;
        _fIterator = kODNULL;
        _fFirstCalled = kODFalse;
        */
        somSelf->InitObject(ev);

        _fPlatformTypeList = typeList;
        OrderedCollection* ordColl = _fPlatformTypeList->GetImplementation(ev);
        _fIterator = ordColl->CreateIterator();

        THROW_IF_NULL(_fIterator);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODPlatformTypeListIterator: IsNotComplete
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODPlatformTypeListIteratorIsNotComplete(ODPlatformTypeListIterator *somSelf, Environment *ev)
{
    ODPlatformTypeListIteratorData *somThis = ODPlatformTypeListIteratorGetData(somSelf);
    ODPlatformTypeListIteratorMethodDebug("ODPlatformTypeListIterator","IsNotComplete");

        ODBoolean isNotComplete = kODFalse;

        SOM_TRY

                if ( !_fFirstCalled )
                        THROW(kODErrIteratorNotInitialized);

                isNotComplete = _fIterator->IsNotComplete();

        SOM_CATCH_ALL
        SOM_ENDTRY

        return isNotComplete;
}

//------------------------------------------------------------------------------
// ODPlatformTypeListIterator: First
//------------------------------------------------------------------------------

SOM_Scope ODPlatformType  SOMLINK ODPlatformTypeListIteratorFirst(ODPlatformTypeListIterator *somSelf, Environment *ev)
{
    ODPlatformTypeListIteratorData *somThis = ODPlatformTypeListIteratorGetData(somSelf);
    ODPlatformTypeListIteratorMethodDebug("ODPlatformTypeListIterator","First");

        ODPlatformType platformType = (ODPlatformType) 0;

        SOM_TRY

                _fFirstCalled = kODTrue;
                platformType = (ODPlatformType) _fIterator->First();

        SOM_CATCH_ALL
        SOM_ENDTRY

        return platformType;
}

//------------------------------------------------------------------------------
// ODPlatformTypeListIterator: Next
//------------------------------------------------------------------------------

SOM_Scope ODPlatformType  SOMLINK ODPlatformTypeListIteratorNext(ODPlatformTypeListIterator *somSelf, Environment *ev)
{
    ODPlatformTypeListIteratorData *somThis = ODPlatformTypeListIteratorGetData(somSelf);
    ODPlatformTypeListIteratorMethodDebug("ODPlatformTypeListIterator","Next");

        ODPlatformType platformType = (ODPlatformType) 0;

        SOM_TRY

                _fFirstCalled = kODTrue;
                platformType = (ODPlatformType) _fIterator->Next();

        SOM_CATCH_ALL
        SOM_ENDTRY

        return platformType;
}
