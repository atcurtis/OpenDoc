/* @(#) 1.16 com/src/storage/LinkSrcB.cpp, oddataxfer, od96os2, odos29646d 10/31/96 07:56:34 [11/15/96 15:25:57] */
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
        File:           LinkSrcB.cpp

        Contains:       Implementation of ODBaseLinkSource class

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <52>     8/19/96        DJ              24768: Fix helps on Link Source Info Dlg
                <51>     6/27/96        DJ              23569: Enable compile on AIX platform
                <50>     10/8/95        TJ              Fixes Recomended by Refball
                <49>     10/5/95        CC              1241300: Clear removes data interchange
                                                                        properties.
                <48>     10/3/95        TJ              Added Includes so it compiles
                <47>     10/3/95        TJ              Changes done by RefBall Team
                <46>      9/8/95        T‚              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
                <45>      9/1/95        CC              1280757: Throw kODErrBrokenLink instead of
                                                                        kODErrBrokenLinkSource.
                <44>     8/30/95        EL              1279582: Storage ODVolatile problems.
                <43>     8/29/95        DM              1221778, 1276165: add LinkUpdateAlert-
                                                                        Completed, fUpdatedAfterSuspended,
                                                                        fSuspended, and change Unlock to let
                                                                        LinkMgr show update alerts.
                <42>     8/21/95        VL              1277291: Added #include <StorUtil.h>.
                <41>     8/12/95        T‚              1276807 Opt./Bug: use StdTypIO routines for
                                                                        portable streaming & smaller footprint
                <40>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <39>     7/28/95        CC              1183569: Externalize only when dirty.
                <38>     7/28/95        CC              1242625: ODDeleteObject sets argument to
                                                                        null.
                <37>     7/27/95        CC              1266053: Unlock only fails if the key is
                                                                        invalid.  Errors caused by notify ing
                                                                        destinations are suppressed.
                                                                        1272086: Unlock returns kODFalse if
                                                                        returned key is null.
                                                                        1266119: InitBaseLinkSource: Set fUpdateID
                                                                        to a valid change ID, in case the initial
                                                                        update of the link fails.
                <36>     7/24/95        CC              1154714: Don't pass update id to
                                                                        UpdateDependents.
                                                                        Unlock: Set _fKindsUsed to kODNULL after
                                                                        deleting.
                <35>     7/20/95        CC              1270145: Clear: Fail immediately if draft
                                                                        cannot be changed.
                <34>     6/21/95        CC              1260937: BB: ODTypeListIterator must return
                                                                        copies of ODType values.
                <33>     6/19/95        CC              1260182 Lock: Return kODFalse if lock not
                                                                        granted.
                <32>     6/17/95        CC              1259902: Clear, ContentUpdated: Fail if id
                                                                        is kODUnknownUpdate.
                                                                        1252700: Clear: Inhibit notification if
                                                                        cleared with existing update ID.
                <31>     6/12/95        CC              1255414: Externalize, CloneInto: Write list
                                                                        of types used if last update failed.
                                                                        Lock: Delete iterator.
                                                                        Added static function
                                                                        RemovePromisedContent; called by
                                                                        Externalize and CloneInto.
                <30>      6/7/95        CC              1252870: Replaced kODKeyNotGranted with
                                                                        local kODNullKey.
                <29>      6/2/95        CC              1255474: GetLink: AcquireLink restored to
                                                                        GetLink.
                <28>     5/26/95        VL              1251403: Multithreading naming support.
                <27>     5/18/95        CC              1250280: ShowLinkSourceInfo throws on
                                                                        invalid null arguments.
                <26>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
                                                                        refcount
                <25>     5/16/95        CC              1246729: Clear removes the kODPropContents
                                                                        property; promises for values in use
                                                                        fulfilled in Unlock.
                <24>     4/27/95        CC              1242550: Moved circular link detection from
                                                                        ContentUpdated to Unlock.
                                                                        Always set fChangeTime, even if change is
                                                                        not propogated.
                                                                        Removed private method IsChanged.
                <23>     4/25/95        CC              1242555: Lock: $5 entered as bug 1242561.
                                                                        1242555: Clear: Removed $5 comment.
                <22>     4/16/95        VL              1230084: Use POUtils in Init,
                                                                        InitFromStorage and CloneInto.
                <21>     4/14/95        T‚              #1235279 BB: InfoUtil & StdTypIO functions
                                                                        should take Environment* and SU* when
                                                                        possible
                <20>     4/11/95        CC              1235953: Lock link's storage unit, not its
                                                                        content storage unit, to guard access to
                                                                        link content.
                                                                        1236452: Added SOM_CATCH to
                                                                        InitBaseLinkSourceFromStorage.
                <19>     3/22/95        CC              1230650: Clear() looks for content in wrong
                                                                        storage unit.
                                                                        1229648: Clear() and Externalize() remove
                                                                        unfulfilled promises.
                <18>      3/9/95        CC              1221471: Added changesAllowed parameter to
                                                                        ShowLinkSourceInfo().
                <17>     2/15/95        CC              1216124 ShowLinkSourceInfo: Aquire modal
                                                                        focus & deactivate front window.
                <16>     1/19/95        CC              1193551 Removed GetLinkStatus().
                                                                        1212356 ShowSourceContent: Catch errors
                                                                        returned by
                                                                        ODLinkSource::ShowSourceContent().
                <15>    12/22/94        CC              1207986 Moved ODSURemoveProperty to
                                                                        StdTypIO.
                <14>    12/16/94        CC              1203516 Removed ODLink parameter from
                                                                        InitBaseLinkSource(); added SetLink().
                <13>    12/15/94        CC              1205605 SetSourcePart() - Removed call to
                                                                        SetChangedFromPrev().
                <12>     10/7/94        CC              1160232 - Alert user of possible link cycle
                                                                        when updating.
                <11>     10/4/94        CC              1190307 - Robustify for non-existent link
                                                                        object.
                <10>     9/29/94        RA              1189812: Mods for 68K build.
                 <9>     9/15/94        CC               1186669 - Replace CloneTo() with
                                                                        CloneInto().
                 <8>      9/8/94        CC              #1185565 - Fix Lock();
                                                                        #1185113 - Use StdTypeIO to read/write
                                                                        property values.
                 <7>      9/7/94        CC              #1185344 - Implement Lock() and Unlock()
                                                                        using storage unit locks.
                 <6>     8/26/94        VL              1183174: Use updated cloning APIs.
                 <5>     8/16/94        T‚              #1180922  Remove more obsolete types from
                                                                        StdTypes.idl.  Localized kODLinkContent to
                                                                        this file, changed usage of kODUpdateID to
                                                                        kODULong which is what the format of an
                                                                        ODUpdateID actually is.
                 <4>     8/12/94        CC              1180598 - Added ODFacet parameter to
                                                                        ShowLinkSourceInfo().
                 <3>     8/10/94        CC              1179943 - Moved in platform-independent
                                                                        implementation from LinkSrc.cpp.
                 <2>      8/5/94        CC              #1179160 - ContentUpdated() takes ODLinkKey
                                                                        parameter.
                 <1>     6/24/94        CC              first checked in
        To Do:
*/

#define VARIABLE_MACROS

#define ODBaseLinkSource_Class_Source
#include <LinkSrcB.xih>

#include <LinkSrc.xh>

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


#ifndef SOM_ODLink_xh
#include <Link.xh>
#endif

#ifndef _LINKDLGS_
#include <LinkDlgs.h>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif
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

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
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

/* Adkins -- added to provide prototype for ODUnused and LMGetBufPtr */
/* Adkins -- LMGetBufPtr is in LowMem.h, which PlfmDef.h conditionaly includes */
#ifndef _PLFMDEF_
#include <PlfmDef.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifndef _DOCUTILS_
#include <DocUtils.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef _POUTILS_
#include <POUtils.h>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_LinkMgr_xh
#include <LinkMgr.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#pragma segment ODBaseLinkSource

//==============================================================================
// Constants
//==============================================================================
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
const   ODType  kODLinkContent          = "OpenDoc:LinkContent";
#endif // OS2 or AIX
const   ODULong kODNullKey              = 0;

// dirty masks
const ODULong kNotDirty                                 = 0x0000UL;
const ODULong kLinkDirty                                = 0x0001UL;
const ODULong kSourcePartDirty                  = 0x0002UL;
const ODULong kAutoUpdateDirty                  = 0x0004UL;
const ODULong kUpdateIDDirty                    = 0x0008UL;
const ODULong kChangeTimeDirty                  = 0x0010UL;
const ODULong kKindsUsedDirty                   = 0x0020UL;
const ODULong kContentSUDirty                   = 0x0040UL;
const ODULong kChangeLimitDirty                 = 0x0080UL;
const ODULong kAllDirty                                 = 0xFFFFUL;

//==============================================================================
// Local functions
//==============================================================================

static void RemovePromisedContent(Environment *ev, ODStorageUnit* su);

//==============================================================================
// ODBaseLinkSource
//==============================================================================

//------------------------------------------------------------------------------
// ODBaseLinkSource: somInit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourcesomInit(ODBaseLinkSource *somSelf)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","somInit");

    parent_somInit(somSelf);

        _fContentSU = (ODStorageUnit*) kODNULL;
        _fLink = (ODLink*) kODNULL;
        _fSourcePartSU = (ODStorageUnit*) kODNULL;
        _fAutoUpdate = kODFalse;
        _fNewUpdateID = kODUnknownUpdate;
        _fUpdateID = kODUnknownUpdate;
        _fSuspended = _fUpdatedAfterSuspended = kODFalse;
        _fChangeTime = 0;
        _fChangeCount = 1;
        _fChangeLimit = 1;
        _fDirty = kNotDirty;
        _fKindsUsed = (ODTypeList*) kODNULL;
        _fCleared = kODFalse;
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourcesomUninit(ODBaseLinkSource *somSelf)
{
    /* ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf); */
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","somUninit");

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: InitBaseLinkSource
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceInitBaseLinkSource(ODBaseLinkSource *somSelf, Environment *ev,
                ODStorageUnit* storageUnit,
                ODPart* sourcePart)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","InitBaseLinkSource");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        somSelf->InitPersistentObject(ev, storageUnit);
        InitDateInfo(ev, storageUnit);
#ifdef _PLATFORM_OS2_
#else
        _fUpdateID = storageUnit->GetSession(ev)->UniqueUpdateID(ev);

        _fSuspended = _fUpdatedAfterSuspended = kODFalse;

#endif
        if ( sourcePart != (ODPart*) kODNULL )
        {
                _fSourcePartSU = sourcePart->GetStorageUnit(ev);
                _fSourcePartSU->Acquire(ev);
                _fAutoUpdate = kODTrue;
        }

        ODStorageUnit* su = somSelf->GetStorageUnit(ev);

        _fContentSU = su->GetDraft(ev)->CreateStorageUnit(ev);
        ODSetISOStrProp(ev, _fContentSU, kODPropStorageUnitType, kODISOStr, kODLinkContent);

        ODSUAddPropValue(ev, su, kODPropUpdateID, kODULong);
        ODSUAddPropValue(ev, su, kODPropChangeLimit, kODULong);
        ODSUAddPropValue(ev, su, kODPropChangeTime, kODTime_T);
        ODSUAddPropValue(ev, su, kODPropAutoUpdate, kODBoolean);
        ODSUAddPropValue(ev, su, kODPropLinkContentSU, kODStrongStorageUnitRef);
        ODSUAddPropValue(ev, su, kODPropLink, kODWeakStorageUnitRef);

        if ( _fSourcePartSU != (ODStorageUnit*) kODNULL )
                ODSUAddPropValue(ev, su, kODPropSourcePart, kODWeakStorageUnitRef);

#ifdef _PLATFORM_OS2_
#else
        somSelf->SetBaseLinkSourceDirty(ev, kAllDirty);
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: InitBaseLinkSourceFromStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceInitBaseLinkSourceFromStorage(ODBaseLinkSource *somSelf, Environment *ev,
                ODStorageUnit* storageUnit)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","InitBaseLinkSourceFromStorage");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        somSelf->InitPersistentObjectFromStorage(ev, storageUnit);
        InitDateInfo(ev, storageUnit);

        _fContentSU = storageUnit->GetDraft(ev)->AcquireStorageUnit(ev, ODGetStrongSURefProp(ev, storageUnit, kODPropLinkContentSU, kODStrongStorageUnitRef));
        _fUpdateID = ODGetULongProp(ev, storageUnit, kODPropUpdateID, kODULong);
        _fChangeLimit = ODGetULongProp(ev, storageUnit, kODPropChangeLimit, kODULong);
        _fChangeTime = ODGetTime_TProp(ev, storageUnit, kODPropChangeTime, kODTime_T);
        _fAutoUpdate = ODGetBooleanProp(ev, storageUnit, kODPropAutoUpdate, kODBoolean);

        _fSuspended = _fUpdatedAfterSuspended = kODFalse;

        if ( storageUnit->Exists(ev, kODPropContentKindsUsed, 0, 0) )
        {
                _fKindsUsed = somSelf->GetStorageUnit(ev)->GetSession(ev)->GetStorageSystem(ev)->CreateTypeList(ev, kODNULL);
                ODGetTypeListProp(ev, storageUnit, kODPropContentKindsUsed, kODISOStrList, _fKindsUsed);
        }

        if ( storageUnit->Exists(ev, kODPropLink, 0, 0) )
        {
                ODID linkID = ODGetWeakSURefProp(ev, storageUnit, kODPropLink, kODWeakStorageUnitRef);
                if ( linkID == kODNULLID )
#ifdef _PLATFORM_OS2_
                        ;
#else
                        somSelf->SetBaseLinkSourceDirty(ev, kLinkDirty);
#endif
                else
                        _fLink = storageUnit->GetDraft(ev)->AcquireLink(ev, linkID, (ODLinkSpec*) kODNULL);
        }

        if ( storageUnit->Exists(ev, kODPropSourcePart, 0, 0) )
        {
                ODID sourcePartID = ODGetWeakSURefProp(ev, storageUnit, kODPropSourcePart, kODWeakStorageUnitRef);
                if ( sourcePartID == kODNULLID )
#ifdef _PLATFORM_OS2_
                        ;
#else
                        somSelf->SetBaseLinkSourceDirty(ev, kSourcePartDirty);
#endif
                else
                        _fSourcePartSU = storageUnit->GetDraft(ev)->AcquireStorageUnit(ev, sourcePartID);
        }


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource::Externalize (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceExternalize(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","Externalize");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif
#ifdef _PLATFORM_OS2_

    parent_Externalize(somSelf,ev);
    RemovePromisedContent(ev, _fContentSU);

  ODStorageUnit* su = somSelf->GetStorageUnit(ev);

  ODSetStrongSURefProp(ev, su, kODPropLinkContentSU, kODStrongStorageUnitRef, _fContentSU->GetID(ev));
  ODSetULongProp(ev, su, kODPropUpdateID, kODULong, _fUpdateID);
  ODSetULongProp(ev, su, kODPropChangeLimit, kODULong, _fChangeLimit);
  ODSetTime_TProp(ev, su, kODPropChangeTime, kODTime_T, _fChangeTime);
  ODSetBooleanProp(ev, su, kODPropAutoUpdate, kODBoolean, _fAutoUpdate);

  if ( _fKindsUsed == kODNULL )
    ODSURemoveProperty(ev, su, kODPropContentKindsUsed);
  else
    ODSetTypeListProp(ev, su, kODPropContentKindsUsed, kODISOStrList, _fKindsUsed);

  if ( _fLink == (ODLink*) kODNULL )
    ODSURemoveProperty(ev, su, kODPropLink);
  else
    ODSetWeakSURefProp(ev, su, kODPropLink, kODWeakStorageUnitRef, _fLink->GetStorageUnit(ev)->GetID(ev));

  if ( _fSourcePartSU == (ODStorageUnit*) kODNULL )
    ODSURemoveProperty(ev, su, kODPropSourcePart);
  else
    ODSetWeakSURefProp(ev, su, kODPropSourcePart, kODWeakStorageUnitRef, _fSourcePartSU->GetID(ev));

#else

    parent_Externalize(somSelf,ev);

    RemovePromisedContent(ev, _fContentSU);

        if ( _fDirty )
        {
                ODStorageUnit* su = somSelf->GetStorageUnit(ev);

                if ( _fDirty & kContentSUDirty )
                        ODSetStrongSURefProp(ev, su, kODPropLinkContentSU, kODStrongStorageUnitRef, _fContentSU->GetID(ev));

                if ( _fDirty & kUpdateIDDirty )
                        ODSetULongProp(ev, su, kODPropUpdateID, kODULong, _fUpdateID);

                if ( _fDirty & kChangeLimitDirty )
                        ODSetULongProp(ev, su, kODPropChangeLimit, kODULong, _fChangeLimit);

                if ( _fDirty & kChangeTimeDirty )
                        ODSetTime_TProp(ev, su, kODPropChangeTime, kODTime_T, _fChangeTime);

                if ( _fDirty & kAutoUpdateDirty )
                        ODSetBooleanProp(ev, su, kODPropAutoUpdate, kODBoolean, _fAutoUpdate);

                if ( _fDirty & kKindsUsedDirty )
                {
                        if ( _fKindsUsed == kODNULL )
                                ODSURemoveProperty(ev, su, kODPropContentKindsUsed);
                        else
                                ODSetTypeListProp(ev, su, kODPropContentKindsUsed, kODISOStrList, _fKindsUsed);
                }

                if ( _fDirty & kLinkDirty )
                {
                        if ( _fLink == (ODLink*) kODNULL )
                                ODSURemoveProperty(ev, su, kODPropLink);
                        else
                                ODSetWeakSURefProp(ev, su, kODPropLink, kODWeakStorageUnitRef, _fLink->GetStorageUnit(ev)->GetID(ev));
                }

                if ( _fDirty & kSourcePartDirty )
                {
                        if ( _fSourcePartSU == (ODStorageUnit*) kODNULL )
                                ODSURemoveProperty(ev, su, kODPropSourcePart);
                        else
                                ODSetWeakSURefProp(ev, su, kODPropSourcePart, kODWeakStorageUnitRef, _fSourcePartSU->GetID(ev));
                }

                _fDirty = kNotDirty;
        }
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: CloneInto (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceCloneInto(ODBaseLinkSource *somSelf, Environment *ev,
                ODDraftKey key,
                ODStorageUnit* toSU,
                ODFrame* scopeFrame)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","CloneInto");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        // If one of our properties already exists, this object has been cloned already
        if ( toSU->Exists(ev, kODPropLink, kODWeakStorageUnitRef, 0) )
                return;

        parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);

    RemovePromisedContent(ev, _fContentSU);

        CloneDateInfo(ev, somSelf->GetStorageUnit(ev), toSU);

        ODDraft* draft = somSelf->GetStorageUnit(ev)->GetDraft(ev);

        ODSetULongProp(ev, toSU, kODPropUpdateID, kODULong, _fUpdateID);
        ODSetULongProp(ev, toSU, kODPropChangeLimit, kODULong, 1);
        ODSetTime_TProp(ev, toSU, kODPropChangeTime, kODTime_T, _fChangeTime);
        ODSetBooleanProp(ev, toSU, kODPropAutoUpdate, kODBoolean, _fAutoUpdate);

        ODID clonedLinkContentID = draft->Clone(ev, key, _fContentSU->GetID(ev), 0, (scopeFrame ? scopeFrame->GetID(ev) : 0));
        ODSetStrongSURefProp(ev, toSU, kODPropLinkContentSU, kODStrongStorageUnitRef, clonedLinkContentID);

        if ( _fKindsUsed != kODNULL )
                ODSetTypeListProp(ev, toSU, kODPropContentKindsUsed, kODISOStrList, _fKindsUsed);

        if ( _fLink != (ODLink*) kODNULL )
        {
                ODID clonedLinkSourceID = draft->WeakClone(ev, key, _fLink->GetID(ev), 0, (scopeFrame ? scopeFrame->GetID(ev) : 0));
                ODSetWeakSURefProp(ev, toSU, kODPropLink, kODWeakStorageUnitRef, clonedLinkSourceID);
        }

        // The source part should only be kODNULL for links imported from another draft.
        if ( _fSourcePartSU != (ODStorageUnit*) kODNULL )
        {
                ODID clonedSourcePartID = draft->WeakClone(ev, key, _fSourcePartSU->GetID(ev), 0, (scopeFrame ? scopeFrame->GetID(ev) : 0));
                ODSetWeakSURefProp(ev, toSU, kODPropSourcePart, kODWeakStorageUnitRef, clonedSourcePartID);
        }


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: ReleaseAll (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceReleaseAll(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","ReleaseAll");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

    parent_ReleaseAll(somSelf,ev);

        ODReleaseObject(ev, _fContentSU);
        ODReleaseObject(ev, _fLink);
        ODReleaseObject(ev, _fSourcePartSU);


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: SetBaseLinkSourceDirty
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceSetBaseLinkSourceDirty(ODBaseLinkSource *somSelf, Environment *ev,
        ODULong dirtyProperty)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","SetBaseLinkSourceDirty");

        SOM_TRY
                _fDirty |= dirtyProperty;

                ODDraft* draft = somSelf->GetStorageUnit(ev)->GetDraft(ev);
                if ( draft->GetPermissions(ev) != kODDPReadOnly )
                        draft->SetChangedFromPrev(ev);
        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: ShowLinkSourceInfo
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseLinkSourceShowLinkSourceInfo(ODBaseLinkSource *somSelf, Environment *ev,
                ODFacet* facet,
                ODUpdateID change,
                ODBoolean changesAllowed,
                ODLinkInfoResult* infoResult)
{
   // Note: in OS/2 this method is never called - it is overriden by ODLinkSource

    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","ShowLinkSourceInfo");

        ODBoolean result = kODFalse;
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        THROW_IF_NULL(facet, kODErrNullFacetInput);
        THROW_IF_NULL(infoResult, kODErrNullLinkInfoResultInput);
#ifndef _PLATFORM_OS2_
        THROW_IF_NULL(_fLink, kODErrBrokenLink);
#endif

        ODSession* session = somSelf->GetStorageUnit(ev)->GetSession(ev);
        ODTypeToken modalFocus = session->Tokenize(ev, kODModalFocus);
        ODArbitrator* arbitrator = session->GetArbitrator(ev);
        TempODFrame currentOwner = arbitrator->AcquireFocusOwner(ev, modalFocus);

        if ( arbitrator->RequestFocus(ev, modalFocus, facet->GetFrame(ev)) )
        {
                infoResult->autoUpdate = _fAutoUpdate;

#ifdef _PLATFORM_MACINTOSH_
                session->GetWindowState(ev)->DeactivateFrontWindows(ev);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
                result = ShowLinkSourceInfo(somSelf,
                                            change,
                                            changesAllowed,
                                            somSelf->GetStorageUnit(ev)->GetSession(ev)->GetWindowSystemData(ev)->parentWindow,
                                            facet,
                                            infoResult);
#endif // _PLATFORM_WIN32_
#ifdef _PLATFORM_AIX_
                result = ShowLinkSourceInfo(somSelf, change, changesAllowed, infoResult);
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


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return kODFalse;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: Lock
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseLinkSourceLock(ODBaseLinkSource *somSelf, Environment *ev,
                ODULong wait,
                ODLinkKey* key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","Lock");

    ODUnused(wait);

#ifdef _PLATFORM_OS2_
    TRY


    if (ev->_major) ODSetSOMException(ev, kODNoError);

    *key = (ODLinkKey) _fContentSU->Lock(ev, 0);

    /* the following exception checking should be removed when FN_CATCH */
    /* works on OS2                                                     */
    if (ODGetSOMException(ev) != kODNoError || *key == 0)
    {
      return kODFalse;
    }
    else
    {
     return kODTrue;
    }
    CATCH_ALL
      return kODFalse;
   ENDTRY
#else


        if ( _fLink == (ODLink*) kODNULL )
        {
                ODSetSOMException(ev, kODErrBrokenLink);
                return kODFalse;
        }

        TRY     // Don't use SOM_TRY as we do _not_ want to return an exception!

        *key = (ODLinkKey) somSelf->GetStorageUnit(ev)->Lock(ev, 0);

        _fCleared = kODFalse;
        _fNewUpdateID = kODUnknownUpdate;

        CATCH_ALL
                *key = kODNullKey;
                // Don't reraise, just return
        ENDTRY

        return (*key != kODNullKey);
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: Unlock
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceUnlock(ODBaseLinkSource *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","Unlock");

        SOM_TRY
#ifdef _PLATFORM_OS2_
                _fUpdateContinue = kODFalse;
#endif

                // Force fulfilling of promises for content kinds used by current destinations
                if ( _fCleared && _fKindsUsed )
                {
                        ODBoolean allUsedKindsPresent = kODTrue; ODVolatile(allUsedKindsPresent);

                        ODTypeListIterator* iter = kODNULL; ODVolatile(iter);
                        TRY
                                iter = _fKindsUsed->CreateTypeListIterator(ev);

                                for (ODType kind = iter->First(ev);
                                        iter->IsNotComplete(ev);
                                        kind = iter->Next(ev))
                                {
                                        if ( ODSUExistsThenFocus(ev, _fContentSU, kODPropContents, kind) )
                                        {
                                                if ( _fContentSU->IsPromiseValue(ev) )
                                                {
                                                        // Force part to fulfill its promise
                                                        TRY
                                                                ODULong size = _fContentSU->GetSize(ev);
                                                        CATCH_ALL
                                                                allUsedKindsPresent = kODFalse;
                                                        ENDTRY
                                                }
                                        }
                                        else
                                        {
                                                allUsedKindsPresent = kODFalse;
                                        }
                                        ODDisposePtr(kind);
                                }
                        CATCH_ALL
                                allUsedKindsPresent = kODFalse;
                        ENDTRY
                        ODDeleteObject(iter);

                        if ( allUsedKindsPresent )
                        {
                                ODDeleteObject(_fKindsUsed);
                                somSelf->SetBaseLinkSourceDirty(ev, kKindsUsedDirty);
                        }
                }

                _fCleared = kODFalse;

                // Any unlocking action needs to be performed before calling UpdateDependents(),
                // since that will cause dependents to lock and unlock their link, which delegates
                // to this object.

                #if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                somSelf->GetStorageUnit(ev)->Unlock(ev, (ODStorageUnitKey) key);
                #endif

                if ( _fNewUpdateID != kODUnknownUpdate )
                {
                        ODBoolean notifyDependents = kODFalse;

                        #if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                            time(&_fChangeTime);
                        #else
                            time((time_t *)&_fChangeTime);
                        #endif
                        somSelf->SetBaseLinkSourceDirty(ev, kChangeTimeDirty);

                        if ( _fUpdateID != _fNewUpdateID )
                        {
                                // Link updated with different change ID than last time
                                _fChangeCount = 1;
                                _fUpdateID = _fNewUpdateID;
                                somSelf->SetBaseLinkSourceDirty(ev, kUpdateIDDirty);
                                notifyDependents = kODTrue;

                                if ( _fSuspended )
                                        _fUpdatedAfterSuspended = kODTrue;
                        }
                        else if ( ++_fChangeCount <= _fChangeLimit )
                        {
                                // Same change but user previously continued beyond this iteration
                                notifyDependents = kODTrue;
                        }
                        else
                        {
                                #if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
                                ODLinkManager* linkMgr =
                                        somSelf->GetStorageUnit(ev)->GetSession(ev)->GetLinkManager(ev);
                                ODLinkUpdateResult updateResult = linkMgr->ShowLinkUpdateAlert(ev, somSelf);
                                #else

                                ODLinkUpdateResult updateResult = ::ShowLinkUpdateAlert();
                                if (updateResult) {
                                   _fUpdateContinue = kODTrue;
                                }
                                _fNewUpdateID = kODUnknownUpdate;
                                return;

                                #endif

                                if ( updateResult == kODLinkUpdateContinue )
                                {
                                        // User continued propagating this update
                                        ++_fChangeLimit;
                                        somSelf->SetBaseLinkSourceDirty(ev, kChangeLimitDirty);
                                        notifyDependents = kODTrue;
                                }
                                else if ( updateResult == kODLinkUpdateStop )
                                {
                                        // User cancelled propagating this update
                                        _fChangeLimit = 1;
                                        somSelf->SetBaseLinkSourceDirty(ev, kChangeLimitDirty);
                                }
                                else // updateResult == kODLinkUpdateSuspend
                                {
                                        _fSuspended = kODTrue;
                                }
                        }
                        _fNewUpdateID = kODUnknownUpdate;

                        if ( notifyDependents && !_fSuspended && (_fLink != (ODLink*) kODNULL) )
                        {
                                // Ignore errors returned by UpdateDependents
                                TRY
#ifdef _PLATFORM_OS2_
                                        _fLink->UpdateDependents(ev, _fUpdateID);
#else
                                        _fLink->UpdateDependents(ev);
#endif
                                CATCH_ALL
                                ENDTRY
                        }
                }

        SOM_CATCH_ALL

        SOM_ENDTRY
}


//------------------------------------------------------------------------------
// ODBaseLinkSource: LinkUpdateAlertCompleted
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceLinkUpdateAlertCompleted(ODBaseLinkSource *somSelf, Environment *ev,
                ODLinkUpdateResult updateResult)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","LinkUpdateAlertCompleted");

        ODBoolean notifyDependents = kODFalse;

    SOM_TRY

        // Get and reset these booleans atomically here to avoid possible
        // confusion from setting them again while updating dependents:
        ODBoolean suspended = _fSuspended;
        ODBoolean updatedAfterSuspended = _fUpdatedAfterSuspended;
                _fSuspended = _fUpdatedAfterSuspended = kODFalse;

                if ( updateResult == kODLinkUpdateContinue )
                {
                        // User continued propagating this update
                        ++_fChangeLimit;
                        somSelf->SetBaseLinkSourceDirty(ev, kChangeLimitDirty);
                        notifyDependents = kODTrue;
                }
                else if ( updateResult == kODLinkUpdateStop )
                {
                        // User cancelled propagating this update
                        _fChangeLimit = 1;
                        somSelf->SetBaseLinkSourceDirty(ev, kChangeLimitDirty);

                        if ( updatedAfterSuspended )
                                notifyDependents = kODTrue;
                }

                if ( notifyDependents && (_fLink != (ODLink*) kODNULL) )
                {
                        // Ignore errors returned by UpdateDependents
                        TRY
#ifdef _PLATFORM_OS2_
                                        _fLink->UpdateDependents(ev, _fUpdateID);
#else
                                        _fLink->UpdateDependents(ev);
#endif
                        CATCH_ALL
                        ENDTRY
                }

        SOM_CATCH_ALL

        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: KeyValid
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceKeyValid(ODBaseLinkSource *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","KeyValid");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

#ifdef _PLATFORM_OS2_
        if (key !=1) {
          ODSetSOMException(ev,kODErrInvalidLinkKey);
        }
#else
        if (key == kODNullKey)
                ODSetSOMException(ev, kODErrInvalidLinkKey);
        else
        {
                ODStorageUnit* su = somSelf->GetStorageUnit(ev);
                ODStorageUnitKey suKey = su->Lock(ev, (ODStorageUnitKey) key);
                su->Unlock(ev, suKey);
        }
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: GetContentStorageUnit
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK ODBaseLinkSourceGetContentStorageUnit(ODBaseLinkSource *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","GetContentStorageUnit");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return (ODStorageUnit*) kODNULL;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        somSelf->KeyValid(ev, key);

        return _fContentSU;


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return (ODStorageUnit*) kODNULL;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: ContentUpdated
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceContentUpdated(ODBaseLinkSource *somSelf, Environment *ev,
                ODUpdateID id,
                ODLinkKey key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","ContentUpdated");

        SOM_TRY

                somSelf->KeyValid(ev, key);

                if ( id == kODUnknownUpdate )
                        THROW(kODErrUnknownUpdateID);

                _fNewUpdateID = id;

#ifdef _PLATFORM_OS2_
    time_t now;
    time (&now);
    _fChangeTime = now;
//  _interrupt(3);        // ShowLinkUpdateAlert() not implemented yet
#endif

        SOM_CATCH_ALL

        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: Clear
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceClear(ODBaseLinkSource *somSelf, Environment *ev,
                ODUpdateID id,
                ODLinkKey key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","Clear");

        ODValueType kind = kODNULL;     ODVolatile(kind);

        SOM_TRY

                somSelf->KeyValid(ev, key);

                // Fail immediately if this draft can't be changed
                ODDraftPermissions permissions = somSelf->GetStorageUnit(ev)->GetDraft(ev)->GetPermissions(ev);
                if (!HAS_WRITE_ACCESS(permissions))
                        THROW(kODErrInvalidPermissions);

                if ( id == kODUnknownUpdate )
                        THROW(kODErrUnknownUpdateID);

                // If id is the current id, don't notify destinations unless ContentUpdated is called.
                if ( id != _fUpdateID )
                        _fNewUpdateID = id;

                // Add each non-promise value type in the contents property to _fKindsUsed,
                // then remove the contents property.
            if (ODSUExistsThenFocus(ev, _fContentSU, kODPropContents, kODNULL) )
            {
                        ODULong numValues = _fContentSU->CountValues(ev);
                        ODULong kindIndex;

                        for (kindIndex = 0; kindIndex < numValues; kindIndex++)
                        {
                                _fContentSU->Focus(ev, kODPropContents, kODPosUndefined, kODTypeAll, 0, kODPosNextSib);
                                if ( !_fContentSU->IsPromiseValue(ev) )
                                {
                                        if ( _fKindsUsed == kODNULL )
                                                _fKindsUsed = somSelf->GetStorageUnit(ev)->GetSession(ev)->GetStorageSystem(ev)
                                                                                ->CreateTypeList(ev, kODNULL);
                                        kind = _fContentSU->GetType(ev);
                                        _fKindsUsed->AddLast(ev, kind);
                                        ODDisposePtr(kind);
                                        kind = kODNULL;
                                        somSelf->SetBaseLinkSourceDirty(ev, kKindsUsedDirty);
                                }
                        }
                        ODSURemoveProperty(ev, _fContentSU, kODPropContents);

                        RemoveDataInterchangeProperties(ev, _fContentSU, kODFalse);

                        _fCleared = kODTrue;
                }

        SOM_CATCH_ALL

                ODDisposePtr(kind);

        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: GetUpdateID
//------------------------------------------------------------------------------

SOM_Scope ODUpdateID  SOMLINK ODBaseLinkSourceGetUpdateID(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","GetUpdateID");

        return _fUpdateID;
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: SetUpdateID
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceSetUpdateID(ODBaseLinkSource *somSelf, Environment *ev,
                ODUpdateID updateID,
                ODLinkKey key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","SetUpdateID");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        somSelf->KeyValid(ev, key);

        if ( updateID != _fUpdateID )
        {
                _fUpdateID = updateID;
#ifdef _PLATFORM_OS2_
#else
                somSelf->SetBaseLinkSourceDirty(ev, kUpdateIDDirty);
#endif
        }


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: GetChangeTime
//------------------------------------------------------------------------------

SOM_Scope ODTime  SOMLINK ODBaseLinkSourceGetChangeTime(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","GetChangeTime");

        return _fChangeTime;
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: SetChangeTime
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceSetChangeTime(ODBaseLinkSource *somSelf, Environment *ev,
                ODTime changeTime,
                ODLinkKey key)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","SetChangeTime");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        somSelf->KeyValid(ev, key);

        _fChangeTime = changeTime;
#ifdef _PLATFORM_OS2_
#else
        somSelf->SetBaseLinkSourceDirty(ev, kChangeTimeDirty);
#endif


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: IsAutoUpdate
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseLinkSourceIsAutoUpdate(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","IsAutoUpdate");

        return _fAutoUpdate;
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: SetAutoUpdate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceSetAutoUpdate(ODBaseLinkSource *somSelf, Environment *ev,
                ODBoolean automatic)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","SetAutoUpdate");

        SOM_TRY
        _fAutoUpdate = automatic;
#ifdef _PLATFORM_OS2_
#else
        somSelf->SetBaseLinkSourceDirty(ev, kAutoUpdateDirty);
#endif
        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: GetLink
//------------------------------------------------------------------------------

SOM_Scope ODLink*  SOMLINK ODBaseLinkSourceGetLink(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","GetLink");

        if ( _fLink == (ODLink*) kODNULL )
        {
                ODSetSOMException(ev, kODErrBrokenLink);
                return (ODLink*) kODNULL;
        }

        return _fLink;
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: SetLink
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceSetLink(ODBaseLinkSource *somSelf, Environment *ev,
                ODLink* link)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","SetLink");

        SOM_TRY

        if ( !ODObjectsAreEqual(ev, _fLink, link) )
        {
                ODReleaseObject(ev, _fLink);

                _fLink = link;
                if ( _fLink != (ODLink*) kODNULL )
                        _fLink->Acquire(ev);

#ifdef _PLATFORM_OS2_
#else
                somSelf->SetBaseLinkSourceDirty(ev, kLinkDirty);
#endif
        }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: AcquireSourcePart
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK ODBaseLinkSourceAcquireSourcePart(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","AcquireSourcePart");

        if ( _fSourcePartSU != (ODStorageUnit*) kODNULL ) {
                SOM_TRY
                _fSourcePartSU->Acquire(ev);
                SOM_CATCH_ALL
                SOM_ENDTRY
        }
        return _fSourcePartSU;
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: SetSourcePart
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceSetSourcePart(ODBaseLinkSource *somSelf, Environment *ev,
                ODStorageUnit* sourcePartSU)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","SetSourcePart");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        if ( !ODObjectsAreEqual(ev, _fSourcePartSU, sourcePartSU) )
        {
                ODReleaseObject(ev, _fSourcePartSU);

                _fSourcePartSU = sourcePartSU;
                if ( _fSourcePartSU != (ODStorageUnit*) kODNULL )
                        _fSourcePartSU->Acquire(ev);
#ifdef _PLATFORM_OS2_
#else
                somSelf->SetBaseLinkSourceDirty(ev, kSourcePartDirty);
#endif
        }


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSource: ShowSourceContent
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSourceShowSourceContent(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","ShowSourceContent");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        if ( _fSourcePartSU == (ODStorageUnit*) kODNULL )
                THROW(kODErrBrokenLinkSource);

        TempODPart part = somSelf->GetStorageUnit(ev)->GetDraft(ev)->AcquirePart(ev, _fSourcePartSU->GetID(ev));
        part->RevealLink(ev, (ODLinkSource*) somSelf);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          return;
        SOM_ENDTRY
#endif


}

//------------------------------------------------------------------------------
// RemovePromisedContent
//------------------------------------------------------------------------------

static void RemovePromisedContent(Environment *ev, ODStorageUnit* su)
{
    // Remove value types that still contain promises
    if ( su && ODSUExistsThenFocus(ev, su, kODPropContents, kODNULL) )
    {
                ODULong numValues = su->CountValues(ev);
                ODULong kindIndex;

                for (kindIndex = 0; kindIndex < numValues; kindIndex++)
                {
                        su->Focus(ev, kODPropContents, kODPosUndefined, kODTypeAll, 0, kODPosNextSib);
                        if ( su->IsPromiseValue(ev) )
                                su->Remove(ev);
                }
        }
}

#if defined(_PLATFORM_OS2_)
//------------------------------------------------------------------------------
// ODBaseLinkSource: GetUpdateContinue
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseLinkSourceGetUpdateContinue(ODBaseLinkSource *somSelf, Environment *ev)
{
    ODBaseLinkSourceData *somThis = ODBaseLinkSourceGetData(somSelf);
    ODBaseLinkSourceMethodDebug("ODBaseLinkSource","GetUpdateContinue");

    return _fUpdateContinue;
}
#endif

