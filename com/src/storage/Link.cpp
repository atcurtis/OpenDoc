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
// @(#) 1.9 com/src/storage/Link.cpp, oddataxfer, od96os2, odos29646d 9/3/96 17:21:19 [ 11/15/96 15:25:51 ]
/*
        File:           Link.cpp

        Contains:       Implementation of ODLink class

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <43>     10/8/95        TJ              Fixes Recomended by Refball
                <42>     10/4/95        jpa             Added missing ODVolatiles [1285799]
                <41>      9/8/95        TÇ              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
                <40>     8/12/95        TÇ              1276807 Opt./Bug: use StdTypIO routines for
                                                                        portable streaming & smaller footprint
                <39>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <38>     7/28/95        CC              1183569: Externalize only when dirty.
                <37>     7/27/95        CC              1259986: WriteLinkEdition: Close edition
                                                                        file on failure.
                                                                        CreateLinkEdition: Delete edition file on
                                                                        error writing file.
                <36>     7/24/95        CC              1154714: Removed ODUpdateID parameter from
                                                                        UpdateDependents.
                <35>     7/21/95        CC              1153788: Write 'TEXT' & 'PICT' formats to
                                                                        Edition file.
                <34>     6/30/95        CC              1242642: DeleteLinkEdition: Release after
                                                                        unregistering.
                <33>     5/26/95        VL              1251403: Multithreading naming support.
                <32>     5/22/95        CC              1248361: CloneToMemoryContainer:
                                                                        Externalize content SU to  resolve promises
                                                                        before cloning to memory container.
                <31>     5/18/95        CC              1238898: Add destFrame argument to
                                                                        BeginClone call.
                <30>     5/16/95        CC              1248090: CloneToMemoryContainer: Clone
                                                                        content frame if present.
                <29>     4/25/95        CC              1242555:  Removed obsolete $5 comment.
                <28>     4/14/95        TÇ              #1235279 BB: InfoUtil & StdTypIO functions
                                                                        should take Environment* and SU* when
                                                                        possible
                <27>     4/11/95        CC              1236452:  Replaced SOM_CATCH with SOM_TRY
                                                                        in WriteLinkEdition.
                                                                        Added parenthesis to TopDraft (probably not
                                                                        necessary).
                <26>      4/3/95        CC              1229068: IsTopDraft: Use ODDocument::Exists
                                                                        to test for top draft.
                <25>      3/9/95        CC              1221471: Added changesAllowed parameter to
                                                                        ShowLinkDestinationInfo().
                <24>      3/3/95        CC              1223861, 1153949 Override
                                                                        ShowLinkDestinationInfo().
                <23>      2/7/95        CC              1185113 Externalize: Lock handles during
                                                                        writing.
                                                                        1193560 InitLinkFromStorage: Use ptr
                                                                        returned by ODLockHandle;
                                                                        WASSERTS --> if…THROW
                <22>      2/1/95        CC              1153347 Added static function IsTopDraft().
                                                                        Register sections if draft is topmost.
                                                                        Use IsRegisteredSection for robustness.
                <21>     1/19/95        CC              1212831, 1193556, 1194158 Use creator of
                                                                        document file when creating edition file.
                <20>    12/22/94        CC              1153926 UpdateDependents() - Update
                                                                        cross-document links immediately if source
                                                                        is manual.
                <19>    12/22/94        CC              1153759 CreateLinkEdition - call ODDRaft::
                                                                        SetChangedFromPrev().
                                                                        1207987 Added DeleteLinkEdition().
                <18>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                <17>    12/16/94        CC              1203516 Removed ODLinkSource parameter from
                                                                        InitLink().
                <16>     12/8/94        CC              1154711 InitLinkFromStorage(),
                                                                        ReleaseAll(): Only register & unregister
                                                                        with the Edition manager if draft has write
                                                                        permission.
                <15>    11/30/94        RR              1188078 Use ODTime, not time_t
                <14>     10/7/94        CC              1191089 - Use change time for mod date of
                                                                        edition file.
                <13>     10/4/94        CC              1190862 - Added Release().
                <12>     9/29/94        RA              1189812: Mods for 68K build.
                <11>     9/23/94        VL              1184272: ContainerID is now a sequence of
                                                                        octets.
                <10>     9/15/94        CC              1186669 - Implemented CloneInto();
                                                                        CloneToMemoryContainer use draftKey for
                                                                        Clone() call.
                 <9>      9/7/94        CC              #1185114 - Catch throws from KeyValid().
                 <8>     8/26/94        VL              1183174: Use updated cloning APIs.
                 <7>     8/15/94        TÇ              #1180922 Removed most obsolete types from
                                                                        StdTypes.idl
                 <6>     8/10/94        CC              1179943 - Moved platform-independent
                                                                        implementation into LinkB.cpp.
                 <5>      8/3/94        VL              1153123: Storage to ODStor.
                 <4>     6/24/94        CC              Clean up some easy $5 comments.
                 <3>     6/24/94        CC              Added call to ShowLinkDestinationInfo().
                 <2>     6/24/94        CC              Changed parameters to InitBaseLink().
                 <1>     6/21/94        CC              first checked in

                ------------------- CC ---- Converted to SOM

                 <5>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <4>     4/12/94        CC              Clients may call UnregisterDependent()
                                                                        after this object's ReleaseAll() method has
                                                                        run. (1155973)
                 <3>     3/28/94        CG              1153547: ODSessn.h renamed to ODSessM.h
                 <2>     3/27/94        TÇ              #1153523.  Adjust name of
                                                                        kODPropRootPartSU to more clearly indicate
                                                                        what it is supposed to be used for.
                 <1>     3/25/94        CC              first checked in

                ------------------- CC ---- Moved from Core to Storage subsystem

                 <7>     3/24/94        CC              Implemented CloneTo() (1153038) and
                                                                        GetChangeTime() (115340).
                 <6>     3/15/94        MB              Changes to support SCpp/ASLM builds,
                                                                        #1150864.
                 <5>      3/2/94        CC              ReleaseAll: Check fLinkSource for NIL
                                                                        before releasing it! (1144936)
                 <4>      3/1/94        JA              Include ODMemory.h. (1147588)
                 <3>      3/1/94        CC              Replaced calls to HLock, HUnlock, and
                                                                        GetHandleSize with OD equivalents.
                 <2>     2/17/94        CC              CloneToMemoryContainer: Re-instate fix lost
                                                                        after 2/2/94    checkin.
                <12>      2/8/94        TÇ              Throw -> THROW & some code clean up
                <11>      2/7/94        TÇ              more fixes to compile with PPC Headers
                <10>      2/4/94        TÇ              fixes to compile with PPC Universal headers
                 <9>      2/3/94        CG              System Session break out.
                 <8>      2/2/94        CC              CloneToMemoryContainer:  Moved code to set
                                                                        kODPropRootSU property after the rootSU
                                                                        has been determined.
                 <7>      2/1/94        CC              Pass ODCloneKind argument to BeginClone.
                 <6>     1/28/94        CC              Added ReleaseAll(); increment and decrement
                                                                        ref count of part in ODPartList::AddPart
                                                                        and ::RemovePart; release parts and delete
                                                                        list entries in ODPartList destructor.
                 <5>     1/27/94        VL              Used ODStorageUnit::CloneInto.
                 <4>     1/25/94        CC              CloneToMemoryContainer: use distinct
                                                                        identifiers for clipboard and draft keys;
                                                                        RegisterDependent: call LinkUpdated only if
                                                                        change id is different; GetTextValue: add
                                                                        Volatile stmt.; UpdateDependents: remove
                                                                        ODUnused().
                 <3>     1/24/94        CC              Added ODUpdateID parameter to call to
                                                                        LinkUpdated().
                 <2>     1/22/94        CC              Don't call release() in destructor.
                 <1>     1/21/94        CC              first checked in
                <17>     1/18/94        CG              Added include for StorgDef.h
                <16>     1/18/94        CC              Use kODScrapTypePart; release draft
                                                                        properties on failure in
                                                                        CloneToMemoryContainer and
                                                                        CloneFromMemoryContainer
                <15>     1/14/94        CC              Added Lock(), Unlock(), GetTimeStamp(),
                                                                        GetStatus(), SetAutoExport(),
                                                                        IsAutoExport(), Export(),
                                                                        ShowSourceContent(), type ODLinkKey; added
                                                                        key parameter to various routines; renamed
                                                                        Edition Manager support routines.
                <14>     1/11/94        TÇ              Init... changes
                <13>    12/22/93        CC              Maintain persistent fields fContentSU and
                                                                        fChangedDate.  Read/write link content
                                                                        from/to edition file by cloning fContentSU
                                                                        into a memory container.  Increment link
                                                                        ref count for registered sections.  Write
                                                                        edition files only on Externalize().
                <12>    12/21/93        VL              Changed ODStorageUnit::GetValueSize to
                                                                        StorageUnit::GetSize.
                <11>    12/16/93        CC              Added methods to create, write, and read
                                                                        Edition files.  Save Edition Manager
                                                                        section records and aliases persistently.
                <10>    12/14/93        TÇ              InitLink changes.  Internalize, Initialize
                                                                        went away.
                 <9>     12/8/93        CC              ODValueLink -> ODPartListLink to avoid
                                                                        name conflict.  Removed multiple
                                                                        constructors.  Added InitPartListLink
                 <8>    11/18/93        TÇ              call inherited::InitializePersistent()
                                                                        ::Externalize()
                 <7>    11/17/93        CC              RegisterDependent calls the LinkUpdated
                                                                        method of its part argument.
                 <6>     9/29/93        VL              Added InitializePersistent.
                 <5>      7/1/93        NP              Abstract/Concrete breakout.
                 <4>     6/10/93        NP              Added comments.
                 <3>     4/28/93        NP              File name changes.
                 <2>     4/27/93        NP              Now compilable.
                 <1>     4/21/93        NP              first checked in

        To Do:
        In Progress:
*/

#define VARIABLE_MACROS

#ifdef _PLATFORM_OS2_
 #ifndef _ITEXT_
  #include <IText.h>
 #endif
#endif

#define ODLink_Class_Source
#include <Link.xih>

#ifndef _LINKDLGS_
#include <LinkDlgs.h>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifdef _PLATFORM_OS2_
#include <ODPagtun.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
 #ifndef _CONSTDEF_
  #include <ConstDef.h>
 #endif
#endif

#ifndef SOM_ODLinkSource_xh
#include <LinkSrc.xh>
#endif

#ifndef SOM_ODLinkManager_xh
#include <LinkMgr.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
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

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
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

#ifdef _PLATFORM_OS2_ // CED
 #include <builtin.h>
#else
 #ifndef __ERRORS__
  #include <Errors.h>
 #endif
#endif // _PLATFORM_OS2_

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#if ODDebug
#define ODDebugLink 1
#else
#define ODDebugLink 0
#endif

#ifdef _PLATFORM_MACINTOSH_
 #pragma segment ODLink
#endif

//==============================================================================
// Local Functions
//==============================================================================

static ODBoolean IsTopDraft(Environment* ev, ODDraft* draft);

//------------------------------------------------------------------------------
// IsTopDraft
//------------------------------------------------------------------------------

static ODBoolean IsTopDraft(Environment* ev, ODDraft* draft)
{
#if ODDebugLink
        somPrintf("IsTopDraft: draft refcount is %d\n", draft->GetRefCount(ev));
#endif

        return !(draft->GetDocument(ev)->Exists(ev, kODNULLID, draft, kODPosFirstAbove));
}

//==============================================================================
// ODLink
//==============================================================================

//------------------------------------------------------------------------------
// ODLink: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinksomUninit(ODLink *somSelf)
{
    /* ODLinkData *somThis = ODLinkGetData(somSelf); */
    ODLinkMethodDebug("ODLink","somUninit");

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODLink: InitLink
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkInitLink(ODLink *somSelf, Environment *ev,
                ODStorageUnit* storageUnit)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","InitLink");


#ifdef _PLATFORM_OS2_ // CED
  _interrupt(3);
#else
        SOM_CATCH return;

        /* Moved from somInit. SOM itself sets fields to zero
        _fSection = (SectionHandle) kODNULL;
        _fDirty = kODFalse;
        */
        somSelf->InitBaseLink(ev, storageUnit);

        somSelf->SetLinkDirty(ev);
#endif

}

//------------------------------------------------------------------------------
// ODLink: InitLinkFromStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkInitLinkFromStorage(ODLink *somSelf, Environment *ev,
                ODStorageUnit* storageUnit)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","InitLinkFromStorage");


#ifdef _PLATFORM_OS2_ // CED
  _interrupt(3);
#else

        SOM_CATCH return;

        /* Moved from somInit. SOM itself sets fields to zero
        _fSection = (SectionHandle) kODNULL;
        _fDirty = kODFalse;
        */
        somSelf->InitBaseLinkFromStorage(ev, storageUnit);

        if (ODSUExistsThenFocus(ev, storageUnit, kODPropLinkSection, kODApplesect))
        {
                const int sectionRecordSize = sizeof(SectionRecord);

                if ( storageUnit->GetSize(ev) != sectionRecordSize )
                        THROW(kODErrCorruptLink);

                AliasHandle alias = kODNULL;
                ODVolatile(alias);

                TRY
                        _fSection = (SectionHandle) ODNewHandle(sectionRecordSize);
                        ODValue theSectionPtr = ODLockHandle((ODHandle) _fSection);
                        StorageUnitGetValue(storageUnit, ev, sectionRecordSize, theSectionPtr);
                        ODUnlockHandle((ODHandle) _fSection);

                        if ( (**_fSection).kind != stPublisher )
                                THROW(kODErrCorruptLink);

                        storageUnit->Focus(ev, kODPropEditionAlias, kODPosUndefined, kODApplealis, (ODValueIndex)0, kODPosUndefined);
                        ODULong size = storageUnit->GetSize(ev);

                        if ( size < sizeof(AliasRecord) )
                                THROW(kODErrCorruptLink);

                        alias = (AliasHandle) ODNewHandle(size);
                        ODValue theAliasPtr = ODLockHandle((ODHandle) alias);
                        StorageUnitGetValue(storageUnit, ev, size, theAliasPtr);
                        ODUnlockHandle((ODHandle) alias);
                CATCH_ALL
                        ODDisposeHandle((ODHandle) _fSection);
                        _fSection = (SectionHandle) kODNULL;
                        ODDisposeHandle((ODHandle) alias);
                        RERAISE;
                ENDTRY

                (**_fSection).alias = alias;
                (**_fSection).refCon = (long) somSelf;

                if ( IsTopDraft(ev, storageUnit->GetDraft(ev)) )
                {
                        if ( IsRegisteredSection(_fSection) != noErr )
                        {
                                ODBoolean aliasWasUpdated;

                                ODContainer* container = storageUnit->GetDraft(ev)->GetDocument(ev)->GetContainer(ev);;
                                ODFileSpec documentSpec = GetODFileSpecFromContainer(ev, container);

                                OSErr error = RegisterSection(&documentSpec, _fSection, &aliasWasUpdated);

                                // For now, ignore any error returned by RegisterSection.  The most likely errors are:
                                //      multiplePublisherWrn = -460,            A Publisher is already registered for that container
                                //      containerNotFoundWrn = -461,            Could not find editionContainer at this time
                                //      containerAlreadyOpenWrn = -462,         Container already opened by this section
                                //      notThePublisherWrn = -463,                      Not the first registered publisher for that container

                                if ( IsRegisteredSection(_fSection) == noErr )
                                        somSelf->Acquire(ev);   // Link must not be deleted untill its section is unregistered!
                        }
                }
        }
#endif
}

//------------------------------------------------------------------------------
// ODLink: Release (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkRelease(ODLink *somSelf, Environment *ev)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","Release");


#ifdef _PLATFORM_OS2_
        SOM_TRY
#else
        SOM_CATCH return;
#endif

        parent_Release(somSelf, ev);

#if ODDebugLink
        somPrintf("ODLink %d::Release: Ref count is %d\n", somSelf->GetID(ev), somSelf->GetRefCount(ev));
#endif

        if (somSelf->GetRefCount(ev) == 0)
                somSelf->GetStorageUnit(ev)->GetDraft(ev)->ReleaseLink(ev, somSelf);

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
        return;
#endif

}

//------------------------------------------------------------------------------
// ODLink: Externalize (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkExternalize(ODLink *somSelf, Environment *ev)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","Externalize");

#ifdef _PLATFORM_OS2_

    parent_Externalize(somSelf,ev);

#else

        SOM_CATCH return;

    parent_Externalize(somSelf,ev);

        ODStorageUnit* su = somSelf->GetStorageUnit(ev);

        if ( _fDirty )
        {
                if ( _fSection == (SectionHandle) kODNULL )
                {
                        ODSURemoveProperty(ev, su, kODPropLinkSection);
                        ODSURemoveProperty(ev, su, kODPropEditionAlias);
                }
                else
                {
                        SectionPtr theSectionPtr = (SectionPtr) ODLockHandle((ODHandle) _fSection);

                        ODSUForceFocus(ev, su, kODPropLinkSection, kODApplesect);
                        StorageUnitSetValue(su, ev, ODGetHandleSize((ODHandle) _fSection), (ODValue) theSectionPtr);

                        ODValue theAliasPtr = ODLockHandle((ODHandle) theSectionPtr->alias);
                        ODSUForceFocus(ev, su, kODPropEditionAlias, kODApplealis);
                        StorageUnitSetValue(su, ev, ODGetHandleSize((ODHandle) theSectionPtr->alias), theAliasPtr);

                        ODUnlockHandle((ODHandle) theSectionPtr->alias);
                        ODUnlockHandle((ODHandle) _fSection);
                }
                _fDirty = kODFalse;
        }
#endif // _PLATFORM_OS2_
}

//------------------------------------------------------------------------------
// ODLink: CloneInto (OVERRIDE)
//------------------------------------------------------------------------------
// We don't clone the cross-document link info, so a second publisher can't be created.
// This depends on this method being called rather than ODStorageUnit::CloneInto().
// Currently, all link objects are internalized when a draft is opened, so this is true.

SOM_Scope void  SOMLINK ODLinkCloneInto(ODLink *somSelf, Environment *ev,
                ODDraftKey key,
                ODStorageUnit* toSU,
                ODFrame* scopeFrame)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","CloneInto");

#ifdef _PLATFORM_OS2_

    parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);

#else
        SOM_CATCH return;

        // If one of our properties already exists, this object has been cloned already
        if ( toSU->Exists(ev, kODPropLinkSection, kODApplesect, 0) )
                return;

        parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);

#endif // _PLATFORM_OS2_
}

//------------------------------------------------------------------------------
// ODLink: ReleaseAll (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkReleaseAll(ODLink *somSelf, Environment *ev)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","ReleaseAll");

#ifdef _PLATFORM_OS2_

    parent_ReleaseAll(somSelf,ev);

#else
        SOM_CATCH return;

    parent_ReleaseAll(somSelf,ev);

        if ( _fSection != (SectionHandle) kODNULL )
        {
                if ( IsTopDraft(ev, somSelf->GetStorageUnit(ev)->GetDraft(ev)) )
                {
                        if ( IsRegisteredSection(_fSection) == noErr )
                        {
                                OSErr error = UnRegisterSection(_fSection);
                                somSelf->Release(ev);   // RefCount incremented when section was registered
                        }
                }

                ODDisposeHandle((ODHandle) (**_fSection).alias);
                ODDisposeHandle((ODHandle) _fSection);
                _fSection = (SectionHandle) kODNULL;
        }

#endif // _PLATFORM_OS2_
}

//------------------------------------------------------------------------------
// ODBaseLink: UpdateDependents (OVERRIDE)
//------------------------------------------------------------------------------
#ifdef _PLATFORM_OS2_
SOM_Scope void
SOMLINK ODLinkUpdateDependents(ODLink *somSelf,
                               Environment *ev,
                               ODUpdateID id)
#else
SOM_Scope void  SOMLINK ODLinkUpdateDependents(ODLink *somSelf, Environment *ev)
#endif
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","UpdateDependents");

#ifdef _PLATFORM_OS2_

    parent_UpdateDependents(somSelf, ev, id);

#else

        SOM_CATCH return;

    parent_UpdateDependents(somSelf, ev);

        if ( (_fSection != (SectionHandle) kODNULL) )
        {
                ODBoolean autoUpdate = somSelf->GetLinkSource(ev)->IsAutoUpdate(ev);
                if ( autoUpdate )
                {
                        ODLinkManager*  linkManager = somSelf->GetStorageUnit(ev)->GetSession(ev)->GetLinkManager(ev);
                        linkManager->ExportOnSave(ev, somSelf, kODTrue);
                }
                else
                {
                        ODLinkKey key;
                        ODVolatile(key);
                        if ( somSelf->Lock(ev, 0, &key) )
                        {
                                TRY
                                        somSelf->UpdateLinkEdition(ev, key);
                                CATCH_ALL
                                        somSelf->Unlock(ev, key);
                                        RERAISE;
                                ENDTRY
                                somSelf->Unlock(ev, key);
                        }
                }
        }

#endif // _PLATFORM_OS2_
}

#ifdef _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
// ODLink::ShowLinkDestinationInfo (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkShowLinkDestinationInfo(ODLink *somSelf, Environment *ev,
                ODFacet* facet,
                ODLinkInfo* info,
                ODBoolean changesAllowed,
                ODLinkInfoResult* infoResult)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","ShowLinkDestinationInfo");

        SOM_CATCH return kODFalse;

        ODLinkSource* linkSource = somSelf->GetLinkSource(ev);

        ODError error = linkSource->GetStatus(ev);

        if ( error )
                ShowLinkStatusAlert(error);

        return parent_ShowLinkDestinationInfo(somSelf, ev, facet, info, changesAllowed, infoResult);
}
#endif // _PLATFORM_MACINTOSH_


//------------------------------------------------------------------------------
// ODLink: SetLinkDirty
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSetLinkDirty(ODLink *somSelf, Environment *ev)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","SetLinkDirty");

        SOM_TRY
                _fDirty = kODTrue;

                ODDraft* draft = somSelf->GetStorageUnit(ev)->GetDraft(ev);
                if ( draft->GetPermissions(ev) != kODDPReadOnly )
                        draft->SetChangedFromPrev(ev);
        SOM_CATCH_ALL
        SOM_ENDTRY
}

#ifdef _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
// ODLink: CreateLinkEdition
//------------------------------------------------------------------------------

SOM_Scope AliasHandle  SOMLINK ODLinkCreateLinkEdition(ODLink *somSelf, Environment *ev,
                EditionContainerSpec editionContainer,
                ODFileSpec* documentSpec,
                ODULong sectionID,
                ODLinkKey key)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","CreateLinkEdition");

        AliasHandle result = (AliasHandle) kODNULL;

        OSErr error;
        OSErr createError = mFulErr;    // Anything but noErr
        SectionHandle tempSection = kODNULL;

        ODVolatile(createError);
        ODVolatile(tempSection);
        ODVolatile(editionContainer);

        SOM_TRY

                somSelf->KeyValid(ev, key);

                // Should throw kODErrCannotCreateLink if linkSource imports

                if ( _fSection != kODNULL)
                        THROW(kODErrLinkAlreadyExported);       // Internal error

                FInfo documentFInfo;
                error = FSpGetFInfo((FSSpecPtr) documentSpec, &documentFInfo);
                THROW_IF_ERROR(error);

                createError = CreateEditionContainerFile(&editionContainer.theFile,
                                        documentFInfo.fdCreator, editionContainer.theFileScript);
                THROW_IF_ERROR(createError);

                error = NewSection(&editionContainer, (FSSpecPtr) documentSpec, stPublisher, sectionID, pumOnSave, &tempSection);
                if ( (error != noErr) &&
                         (error != multiplePublisherWrn) &&
                         (error != notThePublisherWrn) )
                {
                        THROW(error);
                }

                (**tempSection).mdDate = (TimeStamp) somSelf->GetChangeTime(ev);
                (**tempSection).refCon = (long) somSelf;

                somSelf->WriteLinkEdition(ev, tempSection, (FSSpecPtr) documentSpec, key);

                // Edition file was successfully created and written, so add section to persistent state.
                // This Link must not be deleted until its section is unregistered!
                _fSection = tempSection;
                somSelf->Acquire(ev);

                somSelf->SetLinkDirty(ev);

                result = (**_fSection).alias;

        SOM_CATCH_ALL

                if ( createError == noErr )
                        DeleteEditionContainerFile(&editionContainer.theFile);

                if ( tempSection )
                {
                        UnRegisterSection(tempSection);
                        ODDisposeHandle((ODHandle) (**tempSection).alias);
                        ODDisposeHandle((ODHandle) tempSection);
                }

        SOM_ENDTRY

        return result;
}

//------------------------------------------------------------------------------
// ODLink: GetLinkEdition
//------------------------------------------------------------------------------

SOM_Scope AliasHandle  SOMLINK ODLinkGetLinkEdition(ODLink *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","GetLinkEdition");

        AliasHandle editionAlias = (AliasHandle) kODNULL;

        SOM_CATCH return (AliasHandle) kODNULL;

        somSelf->KeyValid(ev, key);

        if (_fSection != (SectionHandle) kODNULL)
                editionAlias = (**_fSection).alias;

        return editionAlias;
}

//------------------------------------------------------------------------------
// ODLink: DeleteLinkEdition
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkDeleteLinkEdition(ODLink *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","DeleteLinkEdition");

        SOM_CATCH return;

        somSelf->KeyValid(ev, key);

        if ( _fSection != (SectionHandle) kODNULL )
        {
                if ( IsTopDraft(ev, somSelf->GetStorageUnit(ev)->GetDraft(ev)) )
                {
                        FSSpec editionFile;
                        ODBoolean wasChanged;

                        OSErr error = ResolveAlias(nil, (**_fSection).alias, &editionFile, &wasChanged);
                        THROW_IF_ERROR(error);

                        if ( IsRegisteredSection(_fSection) == noErr )
                        {
                                error = UnRegisterSection(_fSection);
                                if ( error == noErr )
                                        somSelf->Release(ev);   // RefCount incremented when section was registered
                        }

                        error = DeleteEditionContainerFile(&editionFile);
                        THROW_IF_ERROR(error);

                        ODDisposeHandle((ODHandle) (**_fSection).alias);
                        ODDisposeHandle((ODHandle) _fSection);
                        _fSection = (SectionHandle) kODNULL;

                        somSelf->SetLinkDirty(ev);
                }
        }
}

//------------------------------------------------------------------------------
// ODLink: UpdateLinkEdition
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkUpdateLinkEdition(ODLink *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","UpdateLinkEdition");

        SOM_CATCH return;

        if (_fSection != (SectionHandle) kODNULL)
        {
                ODTime changeTime = somSelf->GetChangeTime(ev);
                if ((TimeStamp) changeTime != (**_fSection).mdDate)
                {
                        ODStorageUnit* storageUnit = somSelf->GetStorageUnit(ev);
                        ODContainer* container = storageUnit->GetDraft(ev)->GetDocument(ev)->GetContainer(ev);;
                        ODFileSpec documentSpec = GetODFileSpecFromContainer(ev, container);

                        (**_fSection).mdDate = (TimeStamp) changeTime;
                        somSelf->WriteLinkEdition(ev, _fSection, &documentSpec, key);
                }
        }
}

//------------------------------------------------------------------------------
// ODLink: GetPlatformValue
//------------------------------------------------------------------------------

SOM_Scope ODHandle  SOMLINK ODLinkGetPlatformValue(ODLink *somSelf, Environment *ev,
                ODPlatformType platformType,
                ODLinkKey key)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","GetPlatformValue");

        ODHandle                dataHandle = (ODHandle) kODNULL;

        SOM_TRY

        ODULong                 dataSize;
        ODStorageUnit*  contentSU = somSelf->GetContentStorageUnit(ev, key);
        ODValueType             isoType = kODNULL;

        ODVolatile(isoType);
        ODVolatile(dataHandle);
        TRY
                ODTranslation* translation = somSelf->GetStorageUnit(ev)->GetSession(ev)->GetTranslation(ev);
                isoType = translation->GetISOTypeFromPlatformType(ev, platformType, kODPlatformDataType);

                if ( ODSUExistsThenFocus(ev, contentSU, kODPropContents, isoType) )
                {
                        dataSize = contentSU->GetSize(ev);
                        dataHandle = ODNewHandle(dataSize);
                        ODValue dataPtr = ODLockHandle(dataHandle);
                        StorageUnitGetValue(contentSU, ev, dataSize, dataPtr);
                        ODUnlockHandle(dataHandle);
                }
        CATCH_ALL
                ODDisposeHandle(dataHandle);
                dataHandle = (ODHandle) kODNULL;
        ENDTRY

        ODDisposePtr(isoType);

        SOM_CATCH_ALL
                dataHandle = kODNULL;
        SOM_ENDTRY

        return dataHandle;
}

#endif // _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
// ODLink: CloneToMemoryContainer
//------------------------------------------------------------------------------

SOM_Scope ODHandle  SOMLINK ODLinkCloneToMemoryContainer(ODLink *somSelf, Environment *ev,
                ODLinkKey key)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","CloneToMemoryContainer");


#ifdef _PLATFORM_OS2_ // CED
  _interrupt(3);
  return (ODHandle) kODNULL;
#else
        SOM_CATCH return (ODHandle) kODNULL;


        ODHandle                        containerHandle;
        ODContainer*            container = (ODContainer*) kODNULL;
        ODDocument*                     document = (ODDocument*) kODNULL;
        ODDraft*                        draft = (ODDraft*) kODNULL;
        ODStorageUnit*          draftProperties = (ODStorageUnit*) kODNULL;
        ODStorageUnit*          contentSU = (ODStorageUnit*) kODNULL;
        ODDraft*                        myDraft = somSelf->GetStorageUnit(ev)->GetDraft(ev);
        ODSession*                      session = somSelf->GetStorageUnit(ev)->GetSession(ev);
        ODStorageUnitID         rootSUID = 0;

        ODVolatile(draft);
        ODVolatile(draftProperties);
        ODVolatile(document);
        ODVolatile(container);
        ODVolatile(containerHandle);
        ODVolatile(myDraft);

        contentSU = somSelf->GetContentStorageUnit(ev, key);

        // Externalize contentSU to force resolution of promises before the
        // clone into the memory container is begun.
        contentSU->Externalize(ev);

        containerHandle = ODNewHandle(0);
        TRY
                container = CreateMemoryContainer(ev, session, containerHandle, kODBentoMemoryContainer);

                document = container->AcquireDocument(ev, kODDefaultDocument);

                draft = document->AcquireBaseDraft(ev, kODDPExclusiveWrite);

                ODDraftKey draftKey = myDraft->BeginClone(ev, draft, kODNULL, kODCloneAll);
                ODVolatile(draftKey);
                TRY
                        rootSUID = myDraft->Clone(ev, draftKey, contentSU->GetID(ev), kODNULLID, 0);

                        if ( ODSUExistsThenFocus(ev, contentSU, kODPropContentFrame, kODWeakStorageUnitRef) )
                        {
                                ODID contentFrameID = ODGetWeakSURefProp(ev, contentSU, kODPropContentFrame, kODWeakStorageUnitRef);
                                myDraft->Clone(ev, draftKey, contentFrameID, kODNULLID, 0);
                        }
                CATCH_ALL
                        myDraft->AbortClone(ev, draftKey);
                        RERAISE;
                ENDTRY

                myDraft->EndClone(ev, draftKey);

                draftProperties = draft->AcquireDraftProperties(ev);
                ODSetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef, rootSUID);
                draftProperties->Release(ev);
                draftProperties = kODNULL;

                draft->Externalize(ev);
                draft->Release(ev);
                document->Release(ev);
                container->Release(ev);

        CATCH_ALL
                ODReleaseObject(ev, draftProperties);
                ODReleaseObject(ev, draft);
                ODReleaseObject(ev, document);
                ODReleaseObject(ev, container);

                ODDisposeHandle(containerHandle);
                RERAISE;
        ENDTRY

        return containerHandle;

#endif // _PLATFORM_OS2_
}

#ifdef _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
// ODLink: WriteLinkEdition
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkWriteLinkEdition(ODLink *somSelf, Environment *ev,
                SectionHandle section,
                ODFileSpec* documentSpec,
                ODLinkKey key)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","WriteLinkEdition");

        OSErr                   infoError;
        OSErr                   openError;
        OSErr                   writeError = noErr;
        EditionRefNum   edRefNum = 0;
        ODHandle                dataHandle = (ODHandle) kODNULL;
        ODBoolean               successful = kODTrue;

        ODVolatile(dataHandle);
        ODVolatile(successful);

        SOM_TRY

                if ( section == (SectionHandle) kODNULL )
                        THROW(kODErrNotExportedLink);

                FInfo documentFInfo;
                infoError = FSpGetFInfo((FSSpecPtr) documentSpec, &documentFInfo);
                THROW_IF_ERROR(infoError);

                openError = OpenNewEdition(section, documentFInfo.fdCreator, documentSpec, &edRefNum);
                THROW_IF_ERROR(openError);

                dataHandle = somSelf->CloneToMemoryContainer(ev, key);
                if ( dataHandle != (ODHandle) kODNULL )
                {
                        ODPtr dataPtr = ODLockHandle(dataHandle);
                        writeError = WriteEdition(edRefNum, kODScrapTypeODBentoContainer, dataPtr, ODGetHandleSize(dataHandle));
                        ODDisposeHandle(dataHandle);
                        dataHandle = kODNULL;
                }
                THROW_IF_ERROR(writeError);

                // Also write out any text content property (unnecessary for OpenDoc, but allows
                // subscribing by non-OpenDoc applications, and allows the Edition Manager to display
                // content in the GetInfo dialog.
                dataHandle = somSelf->GetPlatformValue(ev, 'TEXT', key);
                if ( dataHandle != (ODHandle) kODNULL )
                {
                        ODPtr dataPtr = ODLockHandle(dataHandle);
                        writeError = WriteEdition(edRefNum, 'TEXT', dataPtr, ODGetHandleSize(dataHandle));
                        ODUnlockHandle(dataHandle);
                        ODDisposeHandle(dataHandle);
                        dataHandle = kODNULL;
                }
                THROW_IF_ERROR(writeError);

                dataHandle = somSelf->GetPlatformValue(ev, 'PICT', key);
                if ( dataHandle != (ODHandle) kODNULL )
                {
                        ODPtr dataPtr = ODLockHandle(dataHandle);
                        writeError = WriteEdition(edRefNum, 'PICT', dataPtr, ODGetHandleSize(dataHandle));
                        ODUnlockHandle(dataHandle);
                        ODDisposeHandle(dataHandle);
                        dataHandle = kODNULL;
                }
                THROW_IF_ERROR(writeError);


        SOM_CATCH_ALL

                ODDisposeHandle(dataHandle);
                successful = kODFalse;

        SOM_ENDTRY;

        if ( edRefNum != 0 )
                CloseEdition(edRefNum, successful);
}
#endif // _PLATFORM_MACINTOSH_


#ifdef _PLATFORM_OS2_
//------------------------------------------------------------------------------
// ODLink: GetLinkSourceConnectionData
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODLinkGetLinkSourceConnectionData(ODLink *somSelf, Environment *ev,
                ODLinkConnectionData** srcConnData)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","GetLinkSourceConnectionData");
    somSelf->SubClassResponsibility(ev);
}

//------------------------------------------------------------------------------
// ODLink: SetDescription
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODLinkSetDescription(ODLink *somSelf,
                                              Environment *ev,
                                             ODLinkDescription* Desc)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","SetDescription");
    somSelf->SubClassResponsibility(ev);
}

//------------------------------------------------------------------------------
// ODLink: GetConnectionData
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODLinkGetConnectionData(ODLink *somSelf, Environment *ev,
                ODLinkConnectionData** ConnData)
{
    ODLinkData *somThis = ODLinkGetData(somSelf);
    ODLinkMethodDebug("ODLink","GetConnectionData");
    somSelf->SubClassResponsibility(ev);
}

//--------------------------------------------------------------------------------
//ODLink:GetDescription
//---------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkGetDescription(ODLink *somSelf,
                                              Environment *ev,
                                             ODLinkDescription* Desc)
{
    /* ODLinkData *somThis = ODLinkGetData(somSelf); */
    ODLinkMethodDebug("ODLink","ODLinkGetDescription");
    somSelf->SubClassResponsibility(ev);

}

//--------------------------------------------------------------------------------
//ODLink:IsRegistered
//---------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkIsRegistered(ODLink *somSelf,
                                                 Environment *ev)
{
    /* ODLinkData *somThis = ODLinkGetData(somSelf); */
    ODLinkMethodDebug("ODLink","ODLinkIsRegistered");
    somSelf->SubClassResponsibility(ev);
}

#endif /*_PLATFORM_OS2_*/

