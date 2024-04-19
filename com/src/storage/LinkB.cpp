/* @(#) 1.16 com/src/storage/LinkB.cpp, oddataxfer, od96os2, odos29646d 10/31/96 07:56:20 [11/15/96 15:25:51] */
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
        File:           LinkB.cpp

        Contains:       Implementation of ODBaseLink class

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <35>     8/19/96        DJ              24768: Fix helps on Link Destination Info Dlg
                <34>     6/27/96        DJ              23569: Enable compile on AIX platform
                <33>     10/8/95        TJ              Fixes Recomended by Refball
                <32>     10/3/95        TJ              Added Includes so it compiles
                <31>     10/3/95        TJ              Changes done by RefBall Team
                <30>      9/8/95        T‚              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
                <29>      9/1/95        CC              1278590: GetContentStorageUnit: Throw if
                                                                        link source status is
                                                                        kODErrCannotEstablishLink.
                <28>     8/21/95        VL              1277291: Added #include <StorUtil.h>.
                <27>     8/12/95        T‚              1276807 Opt./Bug: use StdTypIO routines for
                                                                        portable streaming & smaller footprint
                <26>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <25>     7/28/95        CC              1183569: Externalize only when dirty.
                <24>     7/28/95        CC              1242625: ODDeleteObject sets argument to
                                                                        null.
                <23>     7/24/95        CC              1193553: Added private class
                                                                        ODPartListIterator.
                                                                        1234526: RegisterDependent: Ignore register
                                                                        if draft is read-only, so part won't be
                                                                        notified.
                <22>     7/24/95        CC              1154714:  UpdateDependents:  Iteration not
                                                                        broken by Registration & Unregistration
                                                                        during notification.
                                                                        Added AddPartList and RemovePartList to
                                                                        private class ODPartList.
                                                                        AddPart: Test for presence in list before
                                                                        adding.
                                                                        Destructor for class ODPartList removes all
                                                                        parts.
                <21>     6/17/95        CC              1259139: GetContentStorageUnit: Fail if no
                                                                        content property or value.
                <20>      6/2/95        CC              1255474: AcquireLinkSource restored to
                                                                        GetLinkSource.
                                                                        1255474: AcquireLinkedList restored to
                                                                        GetLinkedList.
                <19>     5/26/95        VL              1251403: Multithreading naming support.
                <18>     5/25/95        jpa             List.h --> LinkList.h [1253324]
                <17>     5/18/95        CC              1238898: Add destFrame argument to
                                                                        BeginClone call.
                <16>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
                                                                        refcount
                <15>     4/14/95        T‚              #1235279 BB: InfoUtil & StdTypIO functions
                                                                        should take Environment* and SU* when
                                                                        possible
                <14>     4/11/95        CC              1236452: Added SOM_CATCH to InitBaseLink,
                                                                        InitBaseLinkFromStorage,
                                                                        GetContentStorageUnit, SetLinkSource, Lock,
                                                                        and Unlock.
                                                                        Added inline override of THROW_IF_NULL.
                                                                        Removed #include of Link.xh.
                <13>      3/9/95        CC              1221471: Added changesAllowed parameter to
                                                                        ShowLinkDestinationInfo().
                <12>     2/15/95        CC              1216124 ShowLinkDestinationInfo: Aquire
                                                                        modal focus & deactivate front window.
                <11>     1/19/95        CC              1193551 Removed GetLinkStatus().
                                                                        1183569 Added fDirty flag.
                                                                        1212356 ShowSourceContent: Catch errors
                                                                        returned by
                                                                        ODLinkSource::ShowSourceContent().
                <10>    12/22/94        CC              1207986 Moved ODSURemoveProperty to
                                                                        StdTypIO.
                 <9>    12/16/94        CC              1203516 Removed ODLinkSource parameter from
                                                                        InitBaseLink(); added SetLinkSource().
                 <8>     10/4/94        CC              1190307 - Robustify for non-existent link
                                                                        object.
                 <7>     9/15/94        CC              1186669 - Replace CloneTo() with
                                                                        CloneInto().
                 <6>      9/8/94        CC              #1185113 - Use StdTypeIO to read/write
                                                                        property values.
                 <5>      9/7/94        CC              #1185114 - Catch throws from KeyValid().
                 <4>     8/26/94        VL              1183174: Use updated cloning APIs.
                 <3>     8/12/94        CC              1180598 - Added ODFacet parameter to
                                                                        ShowLinkDestinationInfo().
                 <2>     8/10/94        CC              1179943 - Moved in platform-independent
                                                                        implementation from Link.cpp.
                 <1>     6/24/94        CC              first checked in

        To Do:
        In Progress:
*/

#define VARIABLE_MACROS

#ifdef _PLATFORM_OS2_
#include <builtin.h>
#include <odos2.h>
#include <StdDefs.xh>
#endif

#define ODBaseLink_Class_Source
#include <LinkB.xih>

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _CONSTDEF_
#include <ConstDef.h>
#endif
#endif

#ifndef _LINKDLGS_
#include <LinkDlgs.h>
#endif

#ifndef SOM_ODLinkSource_xh
#include <LinkSrc.xh>
#endif

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _DOCUTILS_
#include <DocUtils.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_WIN32_)
#pragma segment ODBaseLink
#endif

//==============================================================================
// Local Classes
//==============================================================================

//------------------------------------------------------------------------------
// Class ODPartLink
//------------------------------------------------------------------------------

class ODPartLink : public Link {
        public:
                ODPartLink(ODPart* thePart) {fPart = thePart;}
                virtual ~ODPartLink()           {}
                ODPart* GetPart()                       { return fPart;}

        private:
                ODPart* fPart;
};

//------------------------------------------------------------------------------
// Class ODPartList
//------------------------------------------------------------------------------

class ODPartList
{
        friend class ODPartListIterator;

        public:

                ODPartList()                    {}
                virtual ~ODPartList()   {this->RemoveAllParts(somGetGlobalEnvironment());}

                virtual ODBoolean       ContainsPart(Environment *ev, ODPart* thePart);
                virtual ODBoolean       AddPart(Environment *ev, ODPart* thePart);
                virtual void            AddPartList(Environment *ev, ODPartList* thePartList);
                virtual void            RemovePart(Environment *ev, ODPart* thePart);
                virtual void            RemovePartList(Environment *ev, ODPartList* thePartList);
                virtual void            RemoveAllParts(Environment *ev);

        private:

                LinkedList      fImplementation;
};

//------------------------------------------------------------------------------
// Class ODPartListIterator
//------------------------------------------------------------------------------

class ODPartListIterator
{
public:

        ODPartListIterator(ODPartList* partList)
                : fIterator(&(partList->fImplementation))
                {}
        ODVMethod ~ODPartListIterator()
                {}

        ODMethod ODPart* First();
        ODMethod ODPart* Next();
        ODMethod ODBoolean IsNotComplete();

private:

        LinkedListIterator fIterator;

};

//------------------------------------------------------------------------------
// ODPartList::ContainsPart
//------------------------------------------------------------------------------

ODBoolean ODPartList::ContainsPart(Environment *ev, ODPart* thePart)
{
        ODBoolean result = kODFalse;
        ODPartListIterator iter(this);

        for (ODPart* part = iter.First(); (result == kODFalse) && iter.IsNotComplete(); part = iter.Next())
        {
                result = ODObjectsAreEqual(ev, part, thePart);
        }

        return result;
}

//------------------------------------------------------------------------------
// ODPartList::AddPart
//------------------------------------------------------------------------------

ODBoolean ODPartList::AddPart(Environment *ev, ODPart* thePart)
{
        ODBoolean notPresent = !this->ContainsPart(ev, thePart);
        if ( notPresent )
        {
                ODPartLink* aLink = new ODPartLink(thePart);
                thePart->Acquire(ev);
                fImplementation.AddLast(aLink);
        }

        return notPresent;
}

//------------------------------------------------------------------------------
// ODPartList::AddPartList
//------------------------------------------------------------------------------

void ODPartList::AddPartList(Environment *ev, ODPartList* thePartList)
{
        if ( thePartList )
        {
                ODPartListIterator iter(thePartList);

                for (ODPart* thePart = iter.First(); iter.IsNotComplete(); thePart = iter.Next())
                {
                        this->AddPart(ev, thePart);
                }
        }
}

//------------------------------------------------------------------------------
// ODPartList::RemovePart
//------------------------------------------------------------------------------

void ODPartList::RemovePart(Environment *ev, ODPart* thePart)
{
        LinkedListIterator iter(&fImplementation);

        ODPartLink* aLink = (ODPartLink*)iter.First();
        while (aLink != NULL)
        {
                ODPart* part = aLink->GetPart();
                if ( ODObjectsAreEqual(ev, part, thePart) )
                {
                        fImplementation.Remove(*aLink);
                        aLink = NULL;
                        part->Release(ev);
                }
                else
                        aLink = (ODPartLink*)iter.Next();
        }
}

//------------------------------------------------------------------------------
// ODPartList::RemovePartList
//------------------------------------------------------------------------------

void ODPartList::RemovePartList(Environment *ev, ODPartList* thePartList)
{
        if ( thePartList )
        {
                ODPartListIterator iter(thePartList);

                for (ODPart* thePart = iter.First(); iter.IsNotComplete(); thePart = iter.Next())
                {
                        this->RemovePart(ev, thePart);
                }
        }
}

//------------------------------------------------------------------------------
// ODPartList::RemoveAllParts
//------------------------------------------------------------------------------

void ODPartList::RemoveAllParts(Environment *ev)
{
        ODPartLink* aLink = (ODPartLink*) fImplementation.RemoveFirst();
        while (aLink != kODNULL)
        {
                ODPart* part = aLink->GetPart();
                part->Release(ev);
                delete aLink;
                aLink = (ODPartLink*) fImplementation.RemoveFirst();
        }
}

//------------------------------------------------------------------------------
// ODPartListIterator::First
//------------------------------------------------------------------------------

ODPart* ODPartListIterator::First()
{
        ODPart* part = kODNULL;
        ODPartLink* partLink = (ODPartLink*) fIterator.First();

        if ( partLink )
                part = partLink->GetPart();

        return part;
}

//------------------------------------------------------------------------------
// ODPartListIterator::Next
//------------------------------------------------------------------------------

ODPart* ODPartListIterator::Next()
{
        ODPart* part = kODNULL;
        ODPartLink* partLink = (ODPartLink*) fIterator.Next();

        if ( partLink )
                part = partLink->GetPart();

        return part;
}

//------------------------------------------------------------------------------
// ODPartListIterator::IsNotComplete
//------------------------------------------------------------------------------

ODBoolean ODPartListIterator::IsNotComplete()
{
        return fIterator.IsNotComplete();
}

//==============================================================================
// ODBaseLink
//==============================================================================

//------------------------------------------------------------------------------
// ODBaseLink: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinksomUninit(ODBaseLink *somSelf)
{
    /* ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf); */
    ODBaseLinkMethodDebug("ODBaseLink","somUninit");

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODBaseLink: InitBaseLink
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkInitBaseLink(ODBaseLink *somSelf, Environment *ev,
                ODStorageUnit* storageUnit)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","InitBaseLink");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        /* Moved from somInit. SOM itself sets fields to zero
        _fPartList = (ODPartList*) kODNULL;
        _fLinkSource = (ODLinkSource*) kODNULL;
        _fDirty = kODFalse;
        _fIterationInProgress = kODFalse;
        _fRestartIteration = kODFalse;
        */
        somSelf->InitPersistentObject(ev, storageUnit);

        _fPartList = new ODPartList();

        ODStorageUnit* su = somSelf->GetStorageUnit(ev);
        ODSUAddPropValue(ev, su, kODPropLinkSource, kODWeakStorageUnitRef);

#ifdef _PLATFORM_OS2_

// Save the hwnd for the client in thread 1 so that when thread 3 from dsom
// calls, the message can be routed to thread 1 for processing.
// This is to make dsom call thread safe.

    PTIB ptib;  
    PPIB ppib;  

    DosGetInfoBlocks(&ptib, &ppib);       // Get the info block
    ULONG tid = ptib->tib_ptib2->tib2_ultid;

    if ( tid == 1) {                      // Check if this is for thread 1

       ODSession     * session      = somSelf->GetStorageUnit(ev)->GetSession(ev);
       ODWindowState * winState     = session->GetWindowState(ev);

       ODWindow      * activeWindow = winState->AcquireActiveWindow(ev);
       HWND            hwndFrame    = activeWindow->GetPlatformWindow(ev);   \
       activeWindow->Release(ev);

       _fhwndClient                 = WinWindowFromID(hwndFrame, FID_CLIENT);

    } /* endif */
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        somSelf->SetBaseLinkDirty(ev);
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: InitBaseLinkFromStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkInitBaseLinkFromStorage(ODBaseLink *somSelf, Environment *ev,
                ODStorageUnit* storageUnit)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","InitBaseLinkFromStorage");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        /* Moved from somInit. SOM itself sets fields to zero
        _fPartList = (ODPartList*) kODNULL;
        _fLinkSource = (ODLinkSource*) kODNULL;
        _fDirty = kODFalse;
        _fIterationInProgress = kODFalse;
        _fRestartIteration = kODFalse;
        */
        somSelf->InitPersistentObjectFromStorage(ev, storageUnit);

        _fPartList = new ODPartList();

        if ( storageUnit->Exists(ev, kODPropLinkSource, 0, 0) )
        {
                ODID linkSourceID = ODGetWeakSURefProp(ev, storageUnit, kODPropLinkSource, kODWeakStorageUnitRef);

#if defined(_PLATFORM_OS2_)
                if ( linkSourceID != 0 )
                        _fLinkSource = storageUnit->GetDraft(ev)->AcquireLinkSource(ev, linkSourceID);
#else
                if ( linkSourceID == kODNULLID )
                        somSelf->SetBaseLinkDirty(ev);
                else
                        _fLinkSource = storageUnit->GetDraft(ev)->AcquireLinkSource(ev, linkSourceID);
#endif
        }


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: Externalize (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkExternalize(ODBaseLink *somSelf, Environment *ev)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","Externalize");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

    parent_Externalize(somSelf,ev);

#if defined(_PLATFORM_OS2_)
        ODStorageUnit* su = somSelf->GetStorageUnit(ev);

        if ( _fLinkSource == (ODLinkSource*) kODNULL )
                ODSURemoveProperty(ev, su, kODPropLinkSource);
        else
        ODSetWeakSURefProp(ev, su, kODPropLinkSource, kODWeakStorageUnitRef, _fLinkSource->GetID(ev));
#else
        if ( _fDirty )
        {
                ODStorageUnit* su = somSelf->GetStorageUnit(ev);

                if ( _fLinkSource == (ODLinkSource*) kODNULL )
                        ODSURemoveProperty(ev, su, kODPropLinkSource);
                else
                        ODSetWeakSURefProp(ev, su, kODPropLinkSource, kODWeakStorageUnitRef, _fLinkSource->GetID(ev));

                _fDirty = kODFalse;
        }
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: CloneInto (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkCloneInto(ODBaseLink *somSelf, Environment *ev,
                ODDraftKey key,
                ODStorageUnit* toSU,
                ODFrame* scopeFrame)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","CloneInto");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        // If one of our properties already exists, this object has been cloned already
        if ( toSU->Exists(ev, kODPropLinkSource, kODWeakStorageUnitRef, 0) )
                return;

        parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);

        if ( _fLinkSource != (ODLinkSource*) kODNULL )
        {
                ODDraft* draft = somSelf->GetStorageUnit(ev)->GetDraft(ev);
                ODID clonedLinkSourceID = draft->WeakClone(ev, key, _fLinkSource->GetID(ev), 0,
                                                                                                (scopeFrame ? scopeFrame->GetID(ev) : 0));

                ODSetWeakSURefProp(ev, toSU, kODPropLinkSource, kODWeakStorageUnitRef, clonedLinkSourceID);
        }


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: ReleaseAll (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkReleaseAll(ODBaseLink *somSelf, Environment *ev)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","ReleaseAll");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

    parent_ReleaseAll(somSelf,ev);

        ODReleaseObject(ev, _fLinkSource);

        // Release all part references stored in the list of registered parts
        ODDeleteObject(_fPartList);


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: SetBaseLinkDirty
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSetBaseLinkDirty(ODBaseLink *somSelf, Environment *ev)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","SetBaseLinkDirty");

        SOM_TRY
                _fDirty = kODTrue;

                ODDraft* draft = somSelf->GetStorageUnit(ev)->GetDraft(ev);
                if ( draft->GetPermissions(ev) != kODDPReadOnly )
                        draft->SetChangedFromPrev(ev);
        SOM_CATCH_ALL
        SOM_ENDTRY
}

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
//------------------------------------------------------------------------------
// ODBaseLink::ShowLinkDestinationInfo
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseLinkShowLinkDestinationInfo(ODBaseLink *somSelf, Environment *ev,
                ODFacet* facet,
                ODLinkInfo* info,
                ODBoolean changesAllowed,
                ODLinkInfoResult* infoResult)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","ShowLinkDestinationInfo");

        ODBoolean result = kODFalse;

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_

        THROW_IF_NULL(facet, kODErrNullFacetInput);
        THROW_IF_NULL(info, kODErrNullLinkInfoInput);
        THROW_IF_NULL(infoResult, kODErrNullLinkInfoResultInput);
#ifndef _PLATFORM_OS2_
        THROW_IF_NULL(_fLinkSource, kODErrBrokenLink);
#endif

        ODSession* session = somSelf->GetStorageUnit(ev)->GetSession(ev);
        ODTypeToken modalFocus = session->Tokenize(ev, kODModalFocus);
        ODArbitrator* arbitrator = session->GetArbitrator(ev);
        TempODFrame currentOwner = arbitrator->AcquireFocusOwner(ev, modalFocus);

        if ( arbitrator->RequestFocus(ev, modalFocus, facet->GetFrame(ev)) )
        {
                infoResult->autoUpdate = info->autoUpdate;
#ifdef _PLATFORM_MACINTOSH_
                session->GetWindowState(ev)->DeactivateFrontWindows(ev);
#endif

#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
                result = ShowLinkDestinationInfo(somSelf,
                                            info,
                                            changesAllowed,
                                            somSelf->GetStorageUnit(ev)->GetSession(ev)->GetWindowSystemData(ev)->parentWindow,
                                            facet,
                                            infoResult);
#else
                result = ShowLinkDestinationInfo(somSelf, info, changesAllowed, infoResult);
#endif

#ifdef _PLATFORM_MACINTOSH_
                session->GetWindowState(ev)->ActivateFrontWindows(ev);
#endif

                arbitrator->TransferFocus(ev, modalFocus, facet->GetFrame(ev), currentOwner);
        }
        else
        {
#ifdef _PLATFORM_MACINTOSH_
                SysBeep(2);
#endif
#ifdef _PLATFORM_WIN32_
                Beep(440, 250);  // play an A for 1/4 second, OK?
#endif
#ifdef _PLATFORM_OS2_
                // ?? OS/2?             SysBeep(2);
#endif
        }
        return result;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return kODFalse;
#endif
}
#endif // _PLATFORM_MACINTOSH_ or _PLATFORM_WIN32_

//------------------------------------------------------------------------------
// ODBaseLink: Lock
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseLinkLock(ODBaseLink *somSelf, Environment *ev,
                ODULong wait,
                ODLinkKey* key)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","Lock");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

#ifdef _PLATFORM_OS2_

        somSelf->SubClassResponsibility(ev);
        return kODFalse;

#else
        THROW_IF_NULL(_fLinkSource, kODErrBrokenLink);

        return _fLinkSource->Lock(ev, wait, key);
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return kODFalse;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: Unlock
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkUnlock(ODBaseLink *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","Unlock");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

#ifdef _PLATFORM_OS2_

    somSelf->SubClassResponsibility(ev);

#else

        THROW_IF_NULL(_fLinkSource, kODErrBrokenLink);

        _fLinkSource->Unlock(ev, key);

#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: KeyValid
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkKeyValid(ODBaseLink *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","KeyValid");

#ifdef _PLATFORM_OS2_

    _interrupt(3);

#else

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        THROW_IF_NULL(_fLinkSource, kODErrBrokenLink);
#ifdef _PLATFORM_MACINTOSH_
        _fLinkSource->KeyValid(ev, key);
#endif


#if  defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif

#endif //_PLATFORM_OS2_

}

//------------------------------------------------------------------------------
// ODBaseLink: GetContentStorageUnit
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK ODBaseLinkGetContentStorageUnit(ODBaseLink *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","GetContentStorageUnit");


#ifdef _PLATFORM_OS2_

    somSelf->SubClassResponsibility(ev);
    return (ODStorageUnit*) kODNULL;

#else

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return (ODStorageUnit*) kODNULL;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        THROW_IF_NULL(_fLinkSource, kODErrBrokenLink);

#ifdef _PLATFORM_MACINTOSH_
        if ( _fLinkSource->GetStatus(ev) == kODErrCannotEstablishLink )
                THROW(kODErrCannotEstablishLink);
#endif

        ODStorageUnit* su = _fLinkSource->GetContentStorageUnit(ev, key);

        if ( su && !su->Exists(ev, kODPropContents, kODNULL, 1) )
                THROW(kODErrNoLinkContent);

        return su;


#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return (ODStorageUnit*) kODNULL;
#endif

#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: RegisterDependent
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkRegisterDependent(ODBaseLink *somSelf, Environment *ev,
                ODPart* clientPart,
                ODUpdateID id)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","RegisterDependent");

#ifdef _PLATFORM_OS2_

        SOM_TRY

        if (!_fPartList->ContainsPart(ev, clientPart))
          _fPartList->AddPart(ev, clientPart);

        ODUpdateID currentID = somSelf->GetUpdateID(ev);
        if (currentID != kODUnknownUpdate)
        {
          if (id != currentID)
            clientPart->LinkUpdated(ev, (ODLink*) somSelf, currentID);
        }

#else
        // This method does nothing if clientPart is already registered or
        // if draft is read-only.

        SOM_TRY

                THROW_IF_NULL(_fLinkSource, kODErrBrokenLink);

                THROW_IF_NULL(_fPartList, kODErrCannotRegisterDependent);

                ODDraftPermissions permissions = somSelf->GetStorageUnit(ev)->GetDraft(ev)->GetPermissions(ev);

                if (HAS_WRITE_ACCESS(permissions))
                {
                        if ( _fPartList->AddPart(ev, clientPart) )
                        {
                                ODUpdateID currentID = somSelf->GetUpdateID(ev);
                                if ( (currentID != kODUnknownUpdate) && (id != currentID) )
                                        clientPart->LinkUpdated(ev, (ODLink*) somSelf, currentID);
                        }
                }

#endif
        SOM_CATCH_ALL

        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLink: UnregisterDependent
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkUnregisterDependent(ODBaseLink *somSelf, Environment *ev,
                ODPart* clientPart)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","UnregisterDependent");

        SOM_TRY

                // If fPartList is null, this object's ReleaseAll() has been called
                //   and already unregistered any dependents.
                if ( _fPartList )
                        _fPartList->RemovePart(ev, clientPart);

        SOM_CATCH_ALL

        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLink: GetUpdateID
//------------------------------------------------------------------------------

SOM_Scope ODUpdateID  SOMLINK ODBaseLinkGetUpdateID(ODBaseLink *somSelf, Environment *ev)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","GetUpdateID");

#ifdef _PLATFORM_OS2_

    somSelf->SubClassResponsibility(ev);
    return kODUnknownUpdate;

#else
        SOM_TRY

        if ( _fLinkSource == (ODLinkSource*) kODNULL )
                THROW(kODErrBrokenLink);

        return _fLinkSource->GetUpdateID(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY
        return kODUnknownUpdate;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: GetChangeTime
//------------------------------------------------------------------------------

SOM_Scope ODTime  SOMLINK ODBaseLinkGetChangeTime(ODBaseLink *somSelf, Environment *ev)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","GetChangeTime");

#ifdef _PLATFORM_OS2_

    somSelf->SubClassResponsibility(ev);
    return (ODTime) 0;

#else
        SOM_TRY

        if ( _fLinkSource == (ODLinkSource*) kODNULL )
                THROW(kODErrBrokenLink);

        return _fLinkSource->GetChangeTime(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY
        return (ODTime)0;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: SetLinkSource
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSetLinkSource(ODBaseLink *somSelf, Environment *ev,
                ODLinkSource* linkSource)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","SetLinkSource");

#ifdef _PLATFORM_OS2_

    _interrupt(3);

#else


#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        if ( !ODObjectsAreEqual(ev, _fLinkSource, linkSource) )
        {
                ODReleaseObject(ev, _fLinkSource);

                _fLinkSource = linkSource;
                if ( _fLinkSource != (ODLinkSource*) kODNULL )
                        _fLinkSource->Acquire(ev);

                somSelf->SetBaseLinkDirty(ev);
        }


#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif

#endif // _PLATFORM_OS2_

}

//------------------------------------------------------------------------------
// ODBaseLink: GetLinkSource
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK ODBaseLinkGetLinkSource(ODBaseLink *somSelf, Environment *ev)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","GetLinkSource");


#ifdef _PLATFORM_OS2_

    somSelf->SubClassResponsibility(ev);
    return (ODLinkSource*) kODNULL;

#else
        if ( _fLinkSource == (ODLinkSource*) kODNULL )
        {
                ODSetSOMException(ev, kODErrBrokenLink);
                return (ODLinkSource*) kODNULL;
        }

        return _fLinkSource;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLink: ShowSourceContent
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkShowSourceContent(ODBaseLink *somSelf, Environment *ev)
{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","ShowSourceContent");

#ifdef _PLATFORM_OS2_

    somSelf->SubClassResponsibility(ev);

#else

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        THROW_IF_NULL(_fLinkSource, kODErrBrokenLink);

        _fLinkSource->ShowSourceContent(ev);


#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif

#endif // _PLATFORM_OS2_

}

//------------------------------------------------------------------------------
// ODBaseLink: UpdateDependents
//------------------------------------------------------------------------------
#ifdef _PLATFORM_OS2_
SOM_Scope void  SOMLINK ODBaseLinkUpdateDependents(ODBaseLink *somSelf, Environment *ev,
                ODUpdateID id)
#else
SOM_Scope void  SOMLINK ODBaseLinkUpdateDependents(ODBaseLink *somSelf, Environment *ev)
#endif

{
    ODBaseLinkData *somThis = ODBaseLinkGetData(somSelf);
    ODBaseLinkMethodDebug("ODBaseLink","UpdateDependents");

  #ifdef _PLATFORM_OS2_

    PTIB ptib;  
    PPIB ppib;  

    DosGetInfoBlocks(&ptib, &ppib);       // Get the info block
    ULONG tid = ptib->tib_ptib2->tib2_ultid;

    if ( tid != 1) {                      // Check if this is for thread 1

       MPARAM mpsomSelf = MPFROMP(somSelf);
       MPARAM mpUpdateId = MPFROMLONG(id);
       ULONG  Message = kODEvtLinkUpdate;

       if (!_fhwndClient) {
          ODSession     * session      = somSelf->GetStorageUnit(ev)->GetSession(ev);
          ODWindowState * winState     = session->GetWindowState(ev);
   
          ODWindow      * activeWindow = winState->AcquireActiveWindow(ev);
          HWND            hwndFrame    = activeWindow->GetPlatformWindow(ev);   \
          activeWindow->Release(ev);
   
          _fhwndClient                 = WinWindowFromID(hwndFrame, FID_CLIENT);
       } /* endif */

       WinPostMsg(_fhwndClient,           // Post the message for thread 1
                  Message,
                  mpsomSelf,
                  mpUpdateId);

       return;
    } /* endif */
  #endif
        if ( _fIterationInProgress )
        {
                _fRestartIteration = kODTrue;
                return;
        }

        _fIterationInProgress = kODTrue;

        SOM_TRY

                do
                {
                        _fRestartIteration = kODFalse;

                        ODPartList registeredParts;
                        registeredParts.AddPartList(ev, _fPartList);

                        ODPartListIterator iter(&registeredParts);

                        for (ODPart* clientPart = iter.First();
                                 !_fRestartIteration && iter.IsNotComplete();
                                 clientPart = iter.Next())
                        {
                                // Don't call LinkUpdated if part has unregistered during iteration.
                                if ( _fPartList->ContainsPart(ev, clientPart) )
                                {
                                        // Ignore any errors returned from a part's LinkUpdated method
                                        TRY
#ifdef _PLATFORM_OS2_
                                                clientPart->LinkUpdated(ev, (ODLink*) somSelf, id);
#else
                                                clientPart->LinkUpdated(ev, (ODLink*) somSelf, somSelf->GetUpdateID(ev));
#endif
                                        CATCH_ALL
                                        ENDTRY
                                }
                        }

                } while ( _fRestartIteration );

        SOM_CATCH_ALL

        SOM_ENDTRY

        _fIterationInProgress = kODFalse;
}
