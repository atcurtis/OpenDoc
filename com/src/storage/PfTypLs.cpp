/* @(#)Z 1.8 com/src/storage/PfTypLs.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:26:04 (96/10/28 13:39:59) */

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
        File:           PfTypLs.cpp

        Contains:       Implementation of class ODPlatformTypeList.

        Owned by:       Craig Carper

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                 <7>     6/25/96        DJ              23458: Enable compile on AIX platform
                 <6>      9/6/95        VL              1279074: Replaced THROW_IF_NULL with
                                                                        ODSetSOMException.
                 <5>     8/30/95        EL              1279832: Storage THROW_IF_NULL problems.
                 <4>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                 <3>     5/26/95        VL              1251403: Multithreading naming support.
                 <2>     4/25/95        CC              1242555: Removed obsolete $5 comment.
                                                                        1242620: InitPlatformTypeList: Added
                                                                        ODVolatile().  Use plain parent macros.
                 <1>     6/23/94        CC              first checked in
                 <0>     6/23/94        SV              SOMverted
                 <2>      4/7/94        JA              Added missing #include (1147588)
                 <1>      4/4/94        CC              first checked in
                 <0>      4/5/94        CC              Code extracted from TranslaM.cpp (1153046)

        To Do:
*/

#define VARIABLE_MACROS

#define ODPlatformTypeList_Class_Source
#include <PfTypLs.xih>

#ifndef SOM_ODPlatformTypeListIterator_xh
#include <PfTLItr.xh>
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

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include <ODUtils.h>
#endif
#endif

#ifndef _PLATFORM_OS2_
#pragma segment ODPlatformTypeList
#endif

#ifdef _PLATFORM_OS2_
#define true  kODTrue
#define false kODFalse
#endif

//==============================================================================
// Class ODPlatformTypeList
//==============================================================================

//------------------------------------------------------------------------------
// ODPlatformTypeList: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPlatformTypeListsomUninit(ODPlatformTypeList *somSelf)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","somUninit");

        delete _fList;

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODPlatformTypeList: InitPlatformTypeList
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPlatformTypeListInitPlatformTypeList(ODPlatformTypeList *somSelf, Environment *ev,
                ODPlatformTypeList* typeList)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","InitPlatformTypeList");

        ODPlatformTypeListIterator* iter = kODNULL;
        ODVolatile(iter);

        SOM_TRY

                /* Moved from somInit. SOM itself sets fields to zero
                _fList = kODNULL;
                */
                somSelf->InitObject(ev);

                _fList = new OrderedCollection;

                if ( typeList != (ODPlatformTypeList*) kODNULL )
                {
                        iter = typeList->CreatePlatformTypeListIterator(ev);

                        for (ODPlatformType type = iter->First(ev);
                                 iter->IsNotComplete(ev);
                                 type = iter->Next(ev))
                        {
                                somSelf->AddLast(ev,type);
                        }
                }

        SOM_CATCH_ALL

                ODDeleteObject(_fList);

        SOM_ENDTRY

        ODDeleteObject(iter);
}

//------------------------------------------------------------------------------
// ODPlatformTypeList: AddLast
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPlatformTypeListAddLast(ODPlatformTypeList *somSelf, Environment *ev,
                ODPlatformType type)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","AddLast");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        if ( somSelf->Contains(ev,type) == kODFalse )
                _fList->AddLast((ElementType)type);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODPlatformTypeList: Remove
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPlatformTypeListRemove(ODPlatformTypeList *somSelf, Environment *ev,
                ODPlatformType type)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","Remove");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        if ( somSelf->Contains(ev,type) )
                _fList->Remove((ElementType)type);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODPlatformTypeList: Contains
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODPlatformTypeListContains(ODPlatformTypeList *somSelf, Environment *ev,
                ODPlatformType type)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","Contains");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        return _fList->Contains((ElementType)type);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return kODFalse;
#endif
}

//------------------------------------------------------------------------------
// ODPlatformTypeList: Count
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODPlatformTypeListCount(ODPlatformTypeList *somSelf, Environment *ev)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","Count");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return 0;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        return _fList->Count();

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return 0;
#endif
}

//------------------------------------------------------------------------------
// ODPlatformTypeList: CreatePlatformTypeListIterator
//------------------------------------------------------------------------------

SOM_Scope ODPlatformTypeListIterator*  SOMLINK ODPlatformTypeListCreatePlatformTypeListIterator(ODPlatformTypeList *somSelf, Environment *ev)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","CreatePlatformTypeListIterator");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return (ODPlatformTypeListIterator*) kODNULL;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        ODPlatformTypeListIterator* iter = new ODPlatformTypeListIterator;
        if (iter == kODNULL)
                ODSetSOMException(ev, kODErrOutOfMemory);
        else
                iter->InitPlatformTypeListIterator(ev, somSelf);

        return iter;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return (ODPlatformTypeListIterator*) kODNULL;
#endif
}

//------------------------------------------------------------------------------
// ODPlatformTypeList: GetImplementation
//------------------------------------------------------------------------------

SOM_Scope OrderedCollection*  SOMLINK ODPlatformTypeListGetImplementation(ODPlatformTypeList *somSelf, Environment *ev)
{
    ODPlatformTypeListData *somThis = ODPlatformTypeListGetData(somSelf);
    ODPlatformTypeListMethodDebug("ODPlatformTypeList","GetImplementation");

    return _fList;
}
