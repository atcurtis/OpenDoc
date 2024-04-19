/* @(#)Z 1.9 com/src/storage/TypeList.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:26:06 (96/10/28 13:40:08) */

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
        File:           TypeList.cpp

        Contains:       Implementation of class ODTypeList

        Owned by:       Craig Carper

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <14>     6/25/96        DJ              23458: Enable compile on AIX platform
                <13>    10/16/95        CC              GM 1292835: Remove: Use underlying Ord coll
                                                                        iterator instead of type list iter.
                <12>     10/8/95        TJ              Fixes Recomended by Refball
                <11>     10/4/95        jpa             Added missing ODVolatiles [1285799]
                <10>      9/6/95        VL              1279074: Replaced THROW_IF_NULL with
                                                                        ODSetSOMException.
                 <9>     8/30/95        EL              1279832: Storage THROW_IF_NULL problems.
                 <8>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                 <7>     7/28/95        CC              1242625: ODDeleteObject sets argument to
                                                                        null.
                 <6>     7/24/95        CG              #1270320: Memory leak fixes.
                 <5>     6/21/95        CC              1260937: BB: ODTypeListIterator must return
                                                                        copies of ODType values.
                 <4>     5/26/95        VL              1251403: Multithreading naming support.
                 <3>     4/14/95        CC              1236608: ODTypeList uses pointer
                                                                        equivalence not ISO string equivalence.
                                                                        1236608: Remove method fixed to remove item
                                                                        from list based on ISO string
                                                                        equality.InitTypeList: Restored call to
                                                                        ODVolatile.
                                                                        Replace verbose parent macro with suscinct
                                                                        one.
                                                                        1238442: InitTypeList iterated over itself
                                                                        instead of the argument type list!!!
                                                                        Methods return kODErrOutOfMemory if _fList
                                                                        is null.
                 <2>     9/29/94        RA              1189812: Mods for 68K build.
                 <1>     6/23/94        CC              first checked in
                 <0>     6/21/94        SV              SOMverted
                 <3>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <2>      4/7/94        JA              Added plenty of missing #includes (1147588)
                 <1>      4/4/94        CC              first checked in
                 <0>      4/5/94        CC              Code extracted from TranslaM.cpp (1153046)

        To Do:
        In Progress:

*/

#define VARIABLE_MACROS

#define ODTypeList_Class_Source
#include <TypeList.xih>

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef SOM_ODObject_xh
#include <ODObject.xh>
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"                     //jpa
#endif

#ifndef _ODTYPESP_
#include "ODTypesP.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif

#ifndef _PLATFORM_OS2_
#pragma segment ODTypeList
#endif

//==============================================================================
// ODTypeList
//==============================================================================

//------------------------------------------------------------------------------
// ODTypeList: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTypeListsomUninit(ODTypeList *somSelf)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","somUninit");

        _fList->DeleteAll();
        ODDeleteObject(_fList);

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODTypeList: InitTypeList
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTypeListInitTypeList(ODTypeList *somSelf, Environment *ev,
                ODTypeList* typeList)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","InitTypeList");

        ODType type = kODNULL;          ODVolatile(type);
        ODTypeListIterator* iter = kODNULL;     ODVolatile(iter);

        SOM_TRY

                /* Moved from somInit. SOM itself sets fields to zero
                _fList = kODNULL;
                */
                somSelf->InitObject(ev);

                _fList = new OrderedCollection;

                if (typeList != (ODTypeList*) kODNULL)
                {
                        iter = typeList->CreateTypeListIterator(ev);

                        for (type = iter->First(ev);
                                 iter->IsNotComplete(ev);
                                 type = iter->Next(ev))
                        {
                                if ( somSelf->Contains(ev,type) == false )
                                {
                                        _fList->AddLast(type);
                                        type = kODNULL;
                                }
                                else
                                        ODDisposePtr(type);
                        }
                }

        SOM_CATCH_ALL

                ODDisposePtr(type);
                ODDeleteObject(_fList);

        SOM_ENDTRY

        ODDeleteObject(iter);
}

//------------------------------------------------------------------------------
// ODTypeList: AddLast
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTypeListAddLast(ODTypeList *somSelf, Environment *ev,
                ODType type)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","AddLast");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        if ( _fList == kODNULL)
                THROW(kODErrOutOfMemory);

        // defect 117544
        #ifdef _PLATFORM_OS2_
        if (somSelf->Contains(ev,type) == kODFalse)
        #else
        if ( somSelf->Contains(ev,type) == false )
        #endif
        {
                ODULong strLength = ODISOStrLength((const ODISOStr)type);
                ElementType newType = ODNewPtrClear(strLength+1, kDefaultHeapID);
                ODISOStrNCopy((const ODISOStr)newType, (ODISOStr)type, strLength );
                _fList->AddLast(newType);
        }

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODTypeList: Remove
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODTypeListRemove(ODTypeList *somSelf, Environment *ev,
                ODType type)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","Remove");

        OrderedCollectionIterator* iterator = kODNULL;

        SOM_TRY

        if ( type != kODNULL )
        {
                iterator = somSelf->GetImplementation(ev)->CreateIterator();
                THROW_IF_NULL(iterator);

                for (ODType thisOne = (ODType) iterator->First();
                         iterator->IsNotComplete();
                         thisOne = (ODType) iterator->Next())
                {
                        if ( thisOne && ODISOStrEqual(thisOne,type) )
                        {
                                _fList->Remove((ElementType) thisOne);
                                break;
                        }
                }
        }

        SOM_CATCH_ALL

        SOM_ENDTRY

        ODDeleteObject(iterator);
}

//------------------------------------------------------------------------------
// ODTypeList: Contains
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODTypeListContains(ODTypeList *somSelf, Environment *ev,
                ODType type)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","Contains");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        ODBoolean result = kODFalse;

        ODTypeListIterator* iter = somSelf->CreateTypeListIterator(ev);

        for (ODType thisOne = iter->First(ev);
                 iter->IsNotComplete(ev);
                 thisOne = iter->Next(ev))
        {
                if ( thisOne )
                {
                        ODBoolean isThisOne = (thisOne == type) || ODISOStrEqual(thisOne,type);
                        ODDisposePtr(thisOne);
                        if ( isThisOne )
                        {
                                result = kODTrue;
                                break;
                        }
                }
        }

        delete iter;
        return result;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return kODFalse;
#endif
}

//------------------------------------------------------------------------------
// ODTypeList: Count
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODTypeListCount(ODTypeList *somSelf, Environment *ev)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","Count");

        if ( _fList == kODNULL)
        {
                ODSetSOMException(ev, kODErrOutOfMemory);
                return 0;
        }
        else
        {
                return _fList->Count();
        }
}

//------------------------------------------------------------------------------
// ODTypeList: CreateTypeListIterator
//------------------------------------------------------------------------------

SOM_Scope ODTypeListIterator*  SOMLINK ODTypeListCreateTypeListIterator(ODTypeList *somSelf, Environment *ev)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","CreateTypeListIterator");

        ODTypeListIterator* typeListIter = kODNULL; ODVolatile(typeListIter);

        SOM_TRY

        if ( _fList == kODNULL)
                ODSetSOMException(ev, kODErrOutOfMemory);
        else
        {
                typeListIter = new ODTypeListIterator;
                if (typeListIter == kODNULL)
                        ODSetSOMException(ev, kODErrOutOfMemory);
                else
                        typeListIter->InitODTypeListIterator(ev, somSelf);
        }

        SOM_CATCH_ALL
                ODDeleteObject(typeListIter);
        SOM_ENDTRY

        return typeListIter;
}

//------------------------------------------------------------------------------
// ODTypeList: GetImplementation
//------------------------------------------------------------------------------

SOM_Scope OrderedCollection*  SOMLINK ODTypeListGetImplementation(ODTypeList *somSelf, Environment *ev)
{
    ODTypeListData *somThis = ODTypeListGetData(somSelf);
    ODTypeListMethodDebug("ODTypeList","ODTypeListGetImplementation");

        if ( _fList == kODNULL)
                ODSetSOMException(ev, kODErrOutOfMemory);

    return _fList;
}
