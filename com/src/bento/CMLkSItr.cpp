/* @(#)Z 1.8 com/src/bento/CMLkSItr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:21 (96/10/29 09:11:27) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
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
        File:           CMLkSItr.cpp

        Contains:       Implementation of class CMLinkSourceIterator

        Owned by:       Craig Carper

        Copyright:      © 1994, 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                 <8>    10/24/95        jpa             1293441: DM/VL: Bento memory reserve &
                                                                        fatal container err.
                 <7>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                 <6>     5/26/95        VL              1251403: Multithreading naming support.
                 <5>     5/18/95        CC              1249197: Renamed InitODLinkSourceIterator
                                                                        to InitLinkSourceIterator.
                 <4>     4/11/95        CC              1236452: InitCMLinkSourceIterator: Added
                                                                        SOM_CATCH and throw if underlying interator
                                                                        can't be created.
                                                                        Changed parent method calls from verbose to
                                                                        simple form (parent_method).
                 <3>     10/4/94        CC              1190857 - Don't call CMReleaseObject() on
                                                                        _fCurrent since its added to the draft's
                                                                        list of objects.
                 <2>     9/23/94        VL              1182227: Includes BentoDef.h for Bento
                                                                        Types.
                 <1>     8/11/94        CC              first checked in

        To Do:
        In Progress:

*/

#define VARIABLE_MACROS

#define CMLinkSourceIterator_Class_Source

#ifdef _PLATFORM_OS2_
#include <ODTypesB.xh>

typedef struct CMItrNode
{
    ODStorageUnitID linkSourceSUID;
    CMItrNode*    next;
} CMItrNode;

#endif /*_PLATFORM_OS2_*/

#include <CMLkSItr.xih>

#ifndef __CM_API__
#include <CMAPI.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_ODObject_xh
#include <ODObject.xh>
#endif

#ifndef SOM_CMCtr_xh
#include <CMCtr.xh>
#endif

#ifndef SOM_CMDraft_xh
#include <CMDraft.xh>
#endif

#ifndef _DRAFPRIV_
#include <DrafPriv.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifndef _BENTODEF_
#include <BentoDef.h>
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#pragma segment CMLinkSourceIterator

//==============================================================================
// CMLinkSourceIterator
//==============================================================================

//------------------------------------------------------------------------------
// CMLinkSourceIterator: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMLinkSourceIteratorsomUninit(CMLinkSourceIterator *somSelf)
{
    CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf);
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","somUninit");

#ifdef _PLATFORM_OS2_
    while(_fCurrentNode = _fCMItrList)
    {
      _fCMItrList = _fCurrentNode->next;
      _fCurrentNode->next = (CMItrNode*)kODNULL;
      delete _fCurrentNode;
    }
#else /* _PLATFORM_OS2_*/

        if (_fBuffer != kODNULL)
                ODDisposePtr(_fBuffer);

    parent_somUninit(somSelf);
#endif /* _PLATFORM_OS2_*/
}

//------------------------------------------------------------------------------
// CMLinkSourceIterator: InitCMLinkSourceIterator
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMLinkSourceIteratorInitCMLinkSourceIterator(CMLinkSourceIterator *somSelf, Environment *ev,
                CMDraft* draft)
{
    CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf);
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","InitCMLinkSourceIterator");

#ifdef _NATIVE_EXCEPTIONS_
//      Use C++ native exception
        SOM_TRY
#else
        SOM_CATCH return;
#endif

        /* Moved from somInit. SOM itself sets fields to zero
        _fDraft = kODNULL;
        _fCurrent = kODNULL;
        _fContainer = kODNULL;
        _fProperty = kODNULL;
        _fType = kODNULL;
        _fBuffer = kODNULL;
        _fSize = 0;
        */

        somSelf->InitLinkSourceIterator(ev);

        _fDraft = draft;
        _fContainer = draft->GetCMContainer(ev);

        ODSessionMustHaveCMAllocReserve(_fContainer);

        _fProperty = CMRegisterProperty(_fContainer, kODPropStorageUnitType);
        _fType = CMRegisterType(_fContainer, kODISOStr);

        ODSessionRestoreCMAllocReserve(_fContainer);

        _fSize = ODISOStrLength(kODLinkSource) + 1;
        _fBuffer = ODNewPtr(_fSize, kDefaultHeapID);

        THROW_IF_NULL(_fBuffer);

#ifdef _NATIVE_EXCEPTIONS_
//      Use C++ native exception
        SOM_CATCH_ALL
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// CMLinkSourceIterator: IsNotComplete
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMLinkSourceIteratorIsNotComplete(CMLinkSourceIterator *somSelf, Environment *ev)
{
    CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf);
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","IsNotComplete");

#ifdef _PLATFORM_OS2_
   if (_fCurrentNode != (CMItrNode*)kODNULL)
   {
      return kODTrue;
   }
   else
   {
      return kODFalse;
   }
#else /*_PLATFORM_OS2_*/
        return ( _fLinkSource != kODNULL );
#endif /* _PLATFORM_OS2_ */
}

//------------------------------------------------------------------------------
// CMLinkSourceIterator: First
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK CMLinkSourceIteratorFirst(CMLinkSourceIterator *somSelf, Environment *ev)
{
    /* CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf); */
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","First");

#ifdef _NATIVE_EXCEPTIONS_
//      Use C++ native exception
        ODLinkSource *retLinkSource = kODNULL;

        SOM_TRY
#ifdef _PLATFORM_OS2_
        retLinkSource = somSelf->AcquireFirst(ev);
#else /*_PLATFORM_OS2_*/
        retLinkSource = somSelf->Next(ev);
#endif /* _PLATFORM_OS2_ */

        SOM_CATCH_ALL
        SOM_ENDTRY

        return retLinkSource;
#else
        SOM_CATCH return kODNULL;
        return somSelf->Next(ev);
#endif

}

//------------------------------------------------------------------------------
// CMLinkSourceIterator: Next
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK CMLinkSourceIteratorNext(CMLinkSourceIterator *somSelf, Environment *ev)
{
    CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf);
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","Next");

#ifdef _PLATFORM_OS2_
        return somSelf->AcquireNext(ev);
#else /*_PLATFORM_OS2_*/
        CMObject        object = _fCurrent;
        CMObject        oldObject = object;

#ifdef _NATIVE_EXCEPTIONS_
//      Use C++ native exception
        ODLinkSource    *retLinkSource = kODNULL;

        SOM_TRY
#else
        SOM_CATCH return kODNULL;
#endif

        _fLinkSource = kODNULL;
        do {
                ODSessionMustHaveCMAllocReserve(_fContainer); // safest once per loop

                object = CMGetNextObjectWithProperty(_fContainer, object, _fProperty);
                if (object != kODNULL) {
                        CMValue value = CMUseValue(object, _fProperty, _fType);
                        if (value != kODNULL) {
                                if ((CMReadValueData(value, _fBuffer, 0, _fSize) == _fSize) &&
                                        (ODISOStrCompare((const ODISOStr) _fBuffer, (const ODISOStr) kODLinkSource) == 0)) {
                                        _fCurrent = object;
                                        ODStorageUnitID id = _fDraft->GetIDList(ev)->Add(_fCurrent);
                                        _fLinkSource = _fDraft->AcquireLinkSource(ev, id);
                                }
                                CMReleaseValue(value);
                        }
                        if (object != _fCurrent)
                                CMReleaseObject(object);        /* we did not use it */
                }
        } while ((object != kODNULL) && (_fCurrent == oldObject));

        ODSessionRestoreCMAllocReserve(_fContainer);

#ifdef _NATIVE_EXCEPTIONS_
//      Use C++ native exception
        retLinkSource = _fLinkSource;

        SOM_CATCH_ALL
        SOM_ENDTRY
        return retLinkSource;
#else
        return _fLinkSource;
#endif
#endif /* _PLATFORM_OS2_ */
}
#ifdef _PLATFORM_OS2_
/*
 *=============================================================================
// LSVCLinkSourceIterator: AcquireFirst
 *=============================================================================
 */
SOM_Scope ODLinkSource*
SOMLINK CMLinkSourceIteratorAcquireFirst(CMLinkSourceIterator *somSelf,
                                         Environment *ev)
{
    CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf);
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","First");

    if ((_fCurrentNode = _fCMItrList) != (CMItrNode*)kODNULL)
    {
      ODLinkSource* linkSource =
                  _fDraft->AcquireLinkSource(ev, _fCurrentNode->linkSourceSUID);
      return linkSource;
    }
    else
    {
      return kODNULL;
    }
}

/*
 *=============================================================================
// LSVCLinkSourceIterator: AcquireNext
 *=============================================================================
 */

SOM_Scope ODLinkSource*
SOMLINK CMLinkSourceIteratorAcquireNext(CMLinkSourceIterator *somSelf,
                                        Environment *ev)
{
    CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf);
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","Next");

    if (_fCurrentNode != (CMItrNode*)kODNULL)
    {
      _fCurrentNode = _fCurrentNode->next;
      if (_fCurrentNode != (CMItrNode*)kODNULL)
      {
        ODLinkSource* linkSource =
                    _fDraft->AcquireLinkSource(ev, _fCurrentNode->linkSourceSUID);
        return linkSource;
      }
    }
    return kODNULL;
}

/*
 *=============================================================================
// LSVCLinkSourceIterator: Add
 *=============================================================================
 */

SOM_Scope void
SOMLINK CMLinkSourceIteratorAdd(CMLinkSourceIterator *somSelf,
                                Environment *ev,
                                ODStorageUnitID linkSourceSUID)
{
    CMLinkSourceIteratorData *somThis = CMLinkSourceIteratorGetData(somSelf);
    CMLinkSourceIteratorMethodDebug("CMLinkSourceIterator","Add");

    CMItrNode* entry = new CMItrNode;
    THROW_IF_NULL(entry);

    entry->linkSourceSUID = linkSourceSUID;
    if (_fCMItrList != (CMItrNode*)kODNULL)
    {
      entry->next = _fCMItrList;
    }
    else
    {
      entry->next = (CMItrNode*)kODNULL;
    }
    _fCMItrList = entry;
}

#endif /* _PLATFORM_OS2_*/
