/* @(#)Z 1.8 com/src/storage/TypLsItr.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:26:06 (96/10/28 13:40:04) */

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
        File:           TypeLsItr.cpp

        Contains:       Implementation of class ODTypeListIterator

        Owned by:       Craig Carper

        Copyright:      © 1994, 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                 <7>     6/25/96        DJ              23458: Enable compile on AIX platform
                 <6>      9/8/95        VL              1282310: Added exception handling.
                 <5>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                 <4>     6/21/95        CC              1260937: BB: ODTypeListIterator must return
                                                                        copies of ODType values.
                                                                        1254450: Added fFirstCalled field.
                 <3>     5/26/95        VL              1251403: Multithreading naming support.
                 <2>     4/11/95        CC              1236452: InitODTypeListIterator: Added
                                                                        SOM_CATCH and throw if underlying interator
                                                                        can't be created.
                                                                        Changed parent method calls from verbose to
                                                                        simple form (parent_method).
                 <1>     6/23/94        CC              first checked in
                 <0>     6/21/94        SV              SOMverted
                 <3>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <2>      4/7/94        JA              Added plenty of missing #includes (1147588)
                 <1>      4/4/94        CC              first checked in
                 <0>      4/5/94        CC              Code extracted from TranslaM.cpp (1153046)

        To Do:
*/

#define VARIABLE_MACROS

#define ODTypeListIterator_Class_Source
#include <TypLsItr.xih>

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODObject_xh
#include <ODObject.xh>
#endif

#ifndef SOM_Module_OpenDoc_Errors_defined
#include <ErrorDef.xh>
#endif

#ifndef _ORDCOLL_
#include <OrdColl.h>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef _ODNew_
#include <ODNew.h>
#endif

#ifndef _PLATFORM_OS2_
#pragma segment ODTypeListIterator
#endif

//==============================================================================
// ODTypeListIterator
//==============================================================================

//------------------------------------------------------------------------------
// ODTypeListIterator: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTypeListIteratorsomUninit(ODTypeListIterator *somSelf)
{
    ODTypeListIteratorData *somThis = ODTypeListIteratorGetData(somSelf);
    ODTypeListIteratorMethodDebug("ODTypeListIterator","ODTypeListIteratorsomUninit");

        delete _fIterator;

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODTypeListIterator: InitODTypeListIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTypeListIteratorInitODTypeListIterator(ODTypeListIterator *somSelf, Environment *ev,
                ODTypeList* typeList)
{
    ODTypeListIteratorData *somThis = ODTypeListIteratorGetData(somSelf);
    ODTypeListIteratorMethodDebug("ODTypeListIterator","InitODTypeListIterator");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        /* Moved from somInit. SOM itself sets fields to 0
        _fTypeList = kODNULL;
        _fIterator = kODNULL;
        _fFirstCalled = kODFalse;
        */

        somSelf->InitObject(ev);

        _fTypeList = typeList;
        OrderedCollection* ordColl = _fTypeList->GetImplementation(ev);
        _fIterator = ordColl->CreateIterator();

        THROW_IF_NULL(_fIterator);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODTypeListIterator: IsNotComplete
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODTypeListIteratorIsNotComplete(ODTypeListIterator *somSelf, Environment *ev)
{
    ODTypeListIteratorData *somThis = ODTypeListIteratorGetData(somSelf);
    ODTypeListIteratorMethodDebug("ODTypeListIterator","IsNotComplete");

        if ( !_fFirstCalled )
        {
                ODSetSOMException(ev, kODErrIteratorNotInitialized);
                return kODFalse;
        }

        return _fIterator->IsNotComplete();
}

//------------------------------------------------------------------------------
// ODTypeListIterator: First
//------------------------------------------------------------------------------

SOM_Scope ODType  SOMLINK ODTypeListIteratorFirst(ODTypeListIterator *somSelf, Environment *ev)
{
    ODTypeListIteratorData *somThis = ODTypeListIteratorGetData(somSelf);
    ODTypeListIteratorMethodDebug("ODTypeListIterator","First");

        ODType resultType = kODNULL;    ODVolatile(resultType);

        SOM_TRY

                _fFirstCalled = kODTrue;

                ODType firstType = (ODType) _fIterator->First();

                if ( firstType )
                {
                        ODULong strLength = ODISOStrLength((const ODISOStr) firstType);
                        resultType = (ODType) ODNewPtrClear(strLength+1, kDefaultHeapID);
                        ODISOStrNCopy((const ODISOStr) resultType, (ODISOStr) firstType, strLength);
                }
        SOM_CATCH_ALL

                ODDisposePtr(resultType);

        SOM_ENDTRY

        return resultType;
}

//------------------------------------------------------------------------------
// ODTypeListIterator: Next
//------------------------------------------------------------------------------

SOM_Scope ODType  SOMLINK ODTypeListIteratorNext(ODTypeListIterator *somSelf, Environment *ev)
{
    ODTypeListIteratorData *somThis = ODTypeListIteratorGetData(somSelf);
    ODTypeListIteratorMethodDebug("ODTypeListIterator","Next");

        ODType resultType = kODNULL;    ODVolatile(resultType);

        SOM_TRY

                _fFirstCalled = kODTrue;

                ODType nextType = (ODType) _fIterator->Next();

                if ( nextType )
                {
                        ODULong strLength = ODISOStrLength((const ODISOStr) nextType);
                        resultType = (ODType) ODNewPtrClear(strLength+1, kDefaultHeapID);
                        ODISOStrNCopy((const ODISOStr) resultType, (ODISOStr) nextType, strLength);
                }

        SOM_CATCH_ALL

                ODDisposePtr(resultType);

        SOM_ENDTRY

        return resultType;
}
