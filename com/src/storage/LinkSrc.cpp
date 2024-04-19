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
// @(#) 1.6 com/src/storage/LinkSrc.cpp, oddataxfer, od96os2, odos29646d 8/26/96 16:54:06 [ 11/15/96 15:25:54 ]
/*
        File:           LinkSrc.cpp

        Contains:       Implementation of ODLinkSource class

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <41>     10/8/95        TJ              Fixes Recomended by Refball
                <40>     10/4/95        jpa             Added missing ODVolatiles [1285799]
                <39>     10/3/95        TJ              Added Includes so it compiles
                <38>     10/3/95        TJ              Changes done by RefBall Team
                <37>      9/8/95        T‚              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
                <36>      9/1/95        CC              1278590: Added SetStatus(ODError error).
                <35>     8/12/95        T‚              1276807 Opt./Bug: use StdTypIO routines for
                                                                        portable streaming & smaller footprint
                <34>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <33>     7/28/95        CC              1183569: Externalize only when dirty.
                <32>     7/20/95        CC              1225443: ShowSourceContent: If the section
                                                                        isn't registered, register temporarily to
                                                                        call GetEditionInfo.
                <31>     6/12/95        CC              1248090: CloneFromMemoryContainer: Force
                                                                        cloning of frame referenced by
                                                                        kODPropContentFrame property.
                <30>     5/26/95        VL              1251403: Multithreading naming support.
                <29>     5/25/95        jpa             Fixed usage of ODDebug. [1253321]
                <28>     5/18/95        CC              1238898: Add destFrame argument to
                                                                        BeginClone call.
                <27>     4/27/95        CC              1242555: Removed $5 checkin comment (doh!)
                <26>     4/14/95        T‚              #1235279 BB: InfoUtil & StdTypIO functions
                                                                        should take Environment* and SU* when
                                                                        possible
                <25>     4/11/95        CC              1236452: Added SOM_CATCH to InitLinkSource
                                                                        and Release.
                <24>      3/3/95        CC              1210760 SetAutoUpdate: update edition file
                                                                        on change to manual from automatic update.
                                                                        1223861 Added GetStatus(); maintain fStatus
                                                                        field.
                <23>      2/7/95        CC              1217192: ReadLinkEdition must call
                                                                        somSelf->SetChangeTime AFTER closing the
                                                                        edition file.
                <22>     1/19/95        CC              1210953 Added override of SetSourcePart.
                                                                        1193561 Code review changes.
                <21>    12/22/94        CC              1153759 UseLinkEdition - call ODDRaft::
                                                                        SetChangedFromPrev().
                <20>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                <19>    12/16/94        CC              1203516 Removed ODLink parameter from
                                                                        InitLinkSource().
                <18>     12/8/94        CC              1154711 Use ">= kODSharedWrite" to check
                                                                        for write access.
                <17>    10/10/94        CC              1160355 - Don't register with Edition mgr.
                                                                        if draft is read only.
                <16>     10/7/94        CC              1191089 - Cross-document updates get new
                                                                        unique ID.
                <15>     10/4/94        CC              1190862 - Added override of Release().
                <14>     9/29/94        RA              1189812: Mods for 68K build.
                <13>     9/23/94        VL              1184272: ContainerID is now a sequence of
                                                                        octets.
                <12>     9/15/94        CC              1186669 - Implemented CloneInto().
                <11>      9/7/94        CC              #1185114 - Catch throws from KeyValid().
                <10>     8/26/94        VL              1183174: Use updated cloning APIs.
                 <9>     8/15/94        T‚              #1180922 Removed most obsolete types from
                                                                        StdTypes.idl
                 <8>     8/10/94        CC              1179943 - Moved platform-independent
                                                                        implementation into LinkSrcB.cpp.
                 <7>      8/5/94        CC              #1179160 - ContentUpdated() takes ODLinkKey
                                                                        parameter and sets fContentUpdated instead
                                                                        of updating dependents immediately;
                                                                                Unlock() updates dependents if contents
                                                                        have changed while locked;
                                                                                #1153789 (partial implementation) -
                                                                        Clear() iterates over values in contents
                                                                        property and deletes all data;
                 <6>      8/3/94        VL              1153123: Storage to ODStor.
                 <5>     6/24/94        CC              Clean up some easy $5.
                 <4>     6/24/94        CC              Added call to ShowLinkSourceInfo().
                 <3>     6/24/94        CC              Changed parameters to InitBaseLinkSource().
                 <2>     6/21/94        CC              Temporarily remove #include of LinkDlgs.h.
                 <1>     6/21/94        CC              first checked in

                ------------------- CC ---- Converted to SOM

                 <6>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <5>      4/7/94        CC              SetSourcePart: Allow source part to be nil;
                                                                        Externalize: Remove source part property
                                                                        source part is nil.  (1154858)
                 <4>      4/4/94        CC              CloneFromMemoryContainer: Changed
                                                                        SetValue() to GetValue() to read
                                                                        modification date. (1154449)
                 <3>     3/28/94        CG              1153547: XMPSessn.h renamed to XMPSessM.h
                 <2>     3/27/94        T‚              #1153523.  Adjust name of
                                                                        kXMPPropRootPartSU to more clearly indicate
                                                                        what it is supposed to be used for.
                 <1>     3/25/94        CC              first checked in

                ------------------- CC ---- Moved from Core to Storage subsystem

                 <5>     3/24/94        CC              Implemented CloneTo() (1153038) and
                                                                        GetChangeTime() (115340); renamed
                                                                        SetAutoExport() and IsAutoExport() to
                                                                        SetAutoUpdate() and IsAutoUpdate()
                                                                        (1142938).
                 <4>     3/14/94        CC              Added SetSourcePart(). (1149440)
                 <3>      3/1/94        JA              Include ODMemory.h (1147588)
                 <2>      3/1/94        CC              Replaced calls to HLock, HUnlock, and
                                                                        GetHandleSize with XMP equivalents.
                 <9>      2/8/94        T‚              Throw -> THROW & some code clean up
                 <8>      2/7/94        T‚              more fixes to compile with PPC Headers
                 <7>      2/4/94        T‚              fixes to compile with PPC Universal headers
                 <6>      2/3/94        CG              System Session break out.
                 <5>      2/1/94        CC              Pass ODCloneKind argument to BeginClone.
                 <4>     1/28/94        CC              Added ReleaseAll(); InitLinkSource does not
                                                                        increment ref count of argument link
                                                                        object.
                 <3>     1/27/94        VL              Used ODStorageUnit::CloneInto.
                 <2>     1/22/94        CC              Don't call release() in destructor.
                 <1>     1/21/94        CC              first checked in
                <17>     1/18/94        CG              Added include for StorgDef.h
                <16>     1/18/94        CC              Use kXMPScrapTypePart; release draft
                                                                        properties on failure in
                                                                        CloneToMemoryContainer and
                                                                        CloneFromMemoryContainer
                <15>     1/14/94        CC              Added Lock(), Unlock(), GetTimeStamp(),
                                                                        GetStatus(), SetAutoExport(),
                                                                        IsAutoExport(), Export(),
                                                                        ShowSourceContent(), type ODLinkKey; added
                                                                        key parameter to various routines; renamed
                                                                        Edition Manager support routines.
                <14>     1/11/94        T‚              Init... changes
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
                <10>    12/14/93        T‚              InitLink changes.  Internalize, Initialize
                                                                        went away.
                 <9>     12/8/93        CC              ODValueLink -> ODPartListLink to avoid
                                                                        name conflict.  Removed multiple
                                                                        constructors.  Added InitPartListLink
                 <8>    11/18/93        T‚              call inherited::InitializePersistent()
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

#define ODLinkSource_Class_Source
#include <LinkSrc.xih>

#ifndef SOM_ODLink_xh
#include <Link.xh>
#endif

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

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifdef _PLATFORM_OS2_
#include <builtin.h>
#else
#ifndef __ERRORS__
#include <Errors.h>
#endif
#endif // _PLATFORM_OS2_

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _DOCUTILS_
#include <DocUtils.h>
#endif

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

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef _PLATFORM_OS2_
#pragma segment ODLinkSource
#endif

//==============================================================================
// ODLinkSource
//==============================================================================

//------------------------------------------------------------------------------
// ODLinkSource: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourcesomUninit(ODLinkSource *somSelf)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","somUninit");

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODLinkSource: InitLinkSource
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceInitLinkSource(ODLinkSource *somSelf, Environment *ev,
                ODStorageUnit* storageUnit,
                ODPart* sourcePart)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","InitLinkSource");

        #ifdef _PLATFORM_OS2_
        SOM_TRY
        #else
        SOM_CATCH return;
        #endif

        /* Moved from somInit. SOM itself sets fields to zero
        _fSection = (SectionHandle) kODNULL;
        _fStatus = 0;
        _fDirty = kODFalse;
        */
        somSelf->InitBaseLinkSource(ev, storageUnit, sourcePart);

#ifdef _PLATFORM_OS2_
#else
        somSelf->SetLinkSourceDirty(ev);
#endif

        #ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
        return;
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: InitLinkSourceFromStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceInitLinkSourceFromStorage(ODLinkSource *somSelf, Environment *ev,
                ODStorageUnit* storageUnit)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","InitLinkSourceFromStorage");

        #ifdef _PLATFORM_OS2_
        _interrupt(3);
        #else

        SOM_CATCH return;

        /* Moved from somInit. SOM itself sets fields to zero
        _fSection = (SectionHandle) kODNULL;
        _fStatus = 0;
        _fDirty = kODFalse;
        */
        somSelf->InitBaseLinkSourceFromStorage(ev, storageUnit);

        if (ODSUExistsThenFocus(ev, storageUnit, kODPropLinkSection, kODApplesect) )
        {
                const int sectionRecordSize = sizeof(SectionRecord);

                ODValue value;

                if ( storageUnit->GetSize(ev) != sectionRecordSize )
                        THROW(kODErrCorruptLinkSource);

                _fSection = (SectionHandle) ODNewHandle(sectionRecordSize);
                value = ODLockHandle((ODHandle) _fSection);
                StorageUnitGetValue(storageUnit, ev, sectionRecordSize, value);
                ODUnlockHandle((ODHandle) _fSection);

                if ( (**_fSection).kind != stSubscriber )
                {
                        ODDisposeHandle((ODHandle) _fSection);
                        _fSection = (SectionHandle) kODNULL;
                        THROW(kODErrCorruptLinkSource);
                }

                storageUnit->Focus(ev, kODPropEditionAlias, kODPosUndefined, kODApplealis, (ODValueIndex)0, kODPosUndefined);
                ODULong size = storageUnit->GetSize(ev);

                if ( size < sizeof(AliasRecord) )
                {
                        ODDisposeHandle((ODHandle) _fSection);
                        _fSection = (SectionHandle) kODNULL;
                        THROW(kODErrCorruptLinkSource);
                }

                AliasHandle alias = (AliasHandle) ODNewHandle(size);
                value = ODLockHandle((ODHandle) alias);
                StorageUnitGetValue(storageUnit, ev, size, value);
                ODUnlockHandle((ODHandle) alias);

                (**_fSection).alias = alias;
                (**_fSection).refCon = (long) somSelf;

                ODDraftPermissions permissions = storageUnit->GetDraft(ev)->GetPermissions(ev);

                if (HAS_WRITE_ACCESS(permissions))
                {
                        ODBoolean aliasWasUpdated;

                        ODContainer* container = storageUnit->GetDraft(ev)->GetDocument(ev)->GetContainer(ev);
                        ODFileSpec documentSpec = GetODFileSpecFromContainer(ev, container);

                        _fStatus = RegisterSection(&documentSpec, _fSection, &aliasWasUpdated);

                        // Remember any error returned by RegisterSection.  The most likely errors are:
                        //      multiplePublisherWrn = -460,            A Publisher is already registered for that container
                        //      containerNotFoundWrn = -461,            Could not find editionContainer at this time
                        //      containerAlreadyOpenWrn = -462,         Container already opened by this section
                        //      notThePublisherWrn = -463,                      Not the first registered publisher for that container

                        if ( _fStatus == containerNotFoundWrn )
                                _fStatus = kODErrCannotFindLinkSourceEdition;

                        somSelf->Acquire(ev);   // Link must not be deleted untill its section is unregistered!
                }
        }
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: Release (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceRelease(ODLinkSource *somSelf, Environment *ev)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","Release");

        #ifdef _PLATFORM_OS2_
        SOM_TRY
        #else
        SOM_CATCH return;
        #endif

        parent_Release(somSelf, ev);

        #if ODDebug
        somPrintf("ODLinkSource %d::Release: Ref count is %d\n", somSelf->GetID(ev), somSelf->GetRefCount(ev));
        #endif

        if ( somSelf->GetRefCount(ev) == 0 )
                somSelf->GetStorageUnit(ev)->GetDraft(ev)->ReleaseLinkSource(ev, somSelf);

        #ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
        return;
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: Externalize (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceExternalize(ODLinkSource *somSelf, Environment *ev)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","Externalize");

        #ifdef _PLATFORM_OS2_

        parent_Externalize(somSelf,ev);

        #else
        SOM_CATCH return;

        parent_Externalize(somSelf,ev);

        if ( _fDirty )
        {
                ODStorageUnit* su = somSelf->GetStorageUnit(ev);

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
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: CloneInto (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceCloneInto(ODLinkSource *somSelf, Environment *ev,
                ODDraftKey key,
                ODStorageUnit* toSU,
                ODFrame* scopeFrame)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","CloneInto");

        #ifdef _PLATFORM_OS2_

        parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);

        #else

        SOM_CATCH return;

        // If one of our properties already exists, this object has been cloned already
        if ( toSU->Exists(ev, kODPropLinkSection, kODApplesect, 0) )
                return;

        parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);

        if ( _fSection != (SectionHandle) kODNULL )
        {
                ODSUForceFocus(ev, toSU, kODPropLinkSection, kODApplesect);
                StorageUnitSetValue(toSU, ev, ODGetHandleSize((ODHandle) _fSection), (ODValue) *_fSection);

                ODSUForceFocus(ev, toSU, kODPropEditionAlias, kODApplealis);
                StorageUnitSetValue(toSU, ev, ODGetHandleSize((ODHandle) (**_fSection).alias), (ODValue) *((**_fSection).alias));
        }
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: ReleaseAll (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceReleaseAll(ODLinkSource *somSelf, Environment *ev)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","ReleaseAll");

        #ifdef _PLATFORM_OS2_
        SOM_TRY
        #else
        SOM_CATCH return;
        #endif


        #ifdef _PLATFORM_OS2_

        parent_ReleaseAll(somSelf, ev);

        #else
        parent_ReleaseAll(somSelf,ev);

        if ( _fSection != (SectionHandle) kODNULL )
        {
                ODDraftPermissions permissions = somSelf->GetStorageUnit(ev)->GetDraft(ev)->GetPermissions(ev);

                if (HAS_WRITE_ACCESS(permissions))
                {
                        OSErr error = UnRegisterSection(_fSection);
                        somSelf->Release(ev);   // RefCount incremented when section was registered
                }

                ODDisposeHandle((ODHandle) (**_fSection).alias);
                ODDisposeHandle((ODHandle) _fSection);
                _fSection = (SectionHandle) kODNULL;
        }
        #endif

        #ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
        return;
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: SetAutoUpdate (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceSetAutoUpdate(ODLinkSource *somSelf, Environment *ev,
                ODBoolean automatic)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","SetAutoUpdate");

        #ifdef _PLATFORM_OS2_
        SOM_TRY
        #else
        SOM_CATCH return;
        #endif

        #ifdef _PLATFORM_OS2_
        parent_SetAutoUpdate(somSelf, ev, automatic);
        #else
        parent_SetAutoUpdate(somSelf, ev, automatic);

        if ( automatic == kODFalse )
        {
                ODLinkManager* linkManager = somSelf->GetStorageUnit(ev)->GetSession(ev)->GetLinkManager(ev);
                ODLink* link = kODNULL;

                TRY
                        link = somSelf->GetLink(ev);
                CATCH_ALL
                ENDTRY

                if ( link && linkManager->ExportOnSave(ev, link, kODFalse) )
                {
                        // The link source was changed from automatic to manual updating,
                        // and the last change was waiting for a save to update the edition file.
                        // Update the edition file now, so cross-document destinations are
                        // consistent with destinations in the same document.  The user will not
                        // be able to manually update the link until another change is made
                        // to the source of the link.
                        ODLinkKey key;
                        if ( link->Lock(ev,0,&key) )
                        {
                                TRY
                                        link->UpdateLinkEdition(ev, key);
                                CATCH_ALL
                                ENDTRY
                                link->Unlock(ev, key);
                        }
                }
        }
        #endif

        #ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
        return;
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: ShowSourceContent (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceShowSourceContent(ODLinkSource *somSelf, Environment *ev)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","ShowSourceContent");

#ifdef _PLATFORM_OS2_

   parent_ShowSourceContent(somSelf, ev);

#else

        SOM_CATCH
        {
#if ODDebug
                somPrintf("ODLinkSource %d: ShowSourceContent :Error %d caught\n", somSelf->GetID(ev), ErrorCode());
#endif
                return;
        }

        if ( _fSection != (SectionHandle) kODNULL )
        {
                EditionInfoRecord infoRec;
                OSErr error;

                if ( IsRegisteredSection(_fSection) == notRegisteredSectionErr )
                {
                        // Temporarily register to get Editon info
                        // $$$$$ This may cause an section Apple event to arrive
                        ODBoolean aliasWasUpdated;
                        ODContainer* container = somSelf->GetStorageUnit(ev)->GetDraft(ev)->GetDocument(ev)->GetContainer(ev);
                        ODFileSpec documentSpec = GetODFileSpecFromContainer(ev, container);
                        RegisterSection(&documentSpec, _fSection, &aliasWasUpdated);
                        error = GetEditionInfo(_fSection, &infoRec);
                        UnRegisterSection(_fSection);
                }
                else
                {
                        error = GetEditionInfo(_fSection, &infoRec);
                }
                if ( error != noErr )
                        THROW(kODErrCannotFindLinkSource);

                error = GoToPublisherSection(&infoRec.container);
                if ( error == fnfErr )
                        THROW(kODErrCannotFindLinkSourceEdition);
                else if ( error != noErr )
                        THROW(kODErrCannotFindLinkSource);
        }
        else
        {
        parent_ShowSourceContent(somSelf, ev);
        }
#endif

}

//------------------------------------------------------------------------------
// ODLinkSource: SetSourcePart (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceSetSourcePart(ODLinkSource *somSelf, Environment *ev,
                ODStorageUnit* sourcePartSU)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","SetSourcePart");


        #ifdef _PLATFORM_OS2_

        parent_SetSourcePart(somSelf, ev, sourcePartSU);

        #else
        SOM_CATCH return;

        ODLinkManager* linkManager = somSelf->GetStorageUnit(ev)->GetSession(ev)->GetLinkManager(ev);

        // If there is no source part for this link, delete the edition file when the
        // draft is saved; is there is a source part, make sure the edition file
        // is still maintained.
        linkManager->DeleteOnSave(ev, somSelf->GetLink(ev), (sourcePartSU == kODNULL));

        parent_SetSourcePart(somSelf, ev, sourcePartSU);
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: SetLinkSourceDirty
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceSetLinkSourceDirty(ODLinkSource *somSelf, Environment *ev)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","SetLinkSourceDirty");

        SOM_TRY
                _fDirty = kODTrue;

                ODDraft* draft = somSelf->GetStorageUnit(ev)->GetDraft(ev);
                if ( draft->GetPermissions(ev) != kODDPReadOnly )
                        draft->SetChangedFromPrev(ev);
        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODLinkSource: UseLinkEdition
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceUseLinkEdition(ODLinkSource *somSelf, Environment *ev,
                EditionContainerSpec editionContainer,
                #ifdef _PLATFORM_OS2_
                FSSpec* documentSpec,
                #else
                ODFileSpec* documentSpec,
                #endif
                ODULong sectionID,
                ODLinkKey key)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","UseLinkEdition");

        #ifdef _PLATFORM_OS2_
        _interrupt(3);
        #else
        SOM_CATCH return;

        somSelf->KeyValid(ev, key);

        if ( _fSection != (SectionHandle) kODNULL )
                THROW(kODErrAlreadyImportedLink);

        OSErr error = NewSection(&editionContainer, documentSpec, stSubscriber, sectionID, sumAutomatic, &_fSection);
        THROW_IF_ERROR(error);

        (**_fSection).refCon = (long) somSelf;
        somSelf->Acquire(ev);   // Link must not be deleted untill its section is unregistered!

        somSelf->SetLinkSourceDirty(ev);
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: ReadLinkEdition
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceReadLinkEdition(ODLinkSource *somSelf, Environment *ev,
                ODLinkKey key)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","ReadLinkEdition");

        #ifdef _PLATFORM_OS2_
        _interrupt(3);
        #else
        SOM_CATCH return;

        somSelf->KeyValid(ev, key);

        OSErr                   openError;
        OSErr                   readError = noErr;
        OSErr                   closeError;
        EditionRefNum   edRefNum;
        ODHandle                dataHandle = kODNULL;           ODVolatile(dataHandle);
        long                    dataSize;
        ODBoolean               successful = kODFalse;

        if ( _fSection == (SectionHandle) kODNULL )
                THROW(kODErrNotImportedLink);

        openError = OpenEdition(_fSection, &edRefNum);
        if ( openError )
        {
                _fStatus = openError;
                THROW(openError);
        }
        else if ( _fStatus == kODErrCannotFindLinkSourceEdition )
        {
                _fStatus = 0;
        }

        readError = EditionHasFormat(edRefNum, kODScrapTypeODBentoContainer, &dataSize);
        if ( readError )
                _fStatus = readError;
        if ( (readError == noErr) && (dataSize > 0) )
        {
                TRY
                        dataHandle = ODNewHandle(dataSize);
                        void* dataPtr = ODLockHandle(dataHandle);
                        readError = ReadEdition (edRefNum, kODScrapTypeODBentoContainer, dataPtr, &dataSize);
                        ODUnlockHandle(dataHandle);
                        THROW_IF_ERROR(readError);

                        // Changes originating in another document get a fresh change ID, since
                        // OpenDoc is not concerned with cycles across documents.
                        // Note that the source document does not mark the edition file with a change ID
                        // (one could be kept in the draft properties, I suppose).
                        ODUpdateID updateID = somSelf->GetStorageUnit(ev)->GetSession(ev)->UniqueUpdateID(ev);
                        somSelf->Clear(ev, updateID, key);
                        ODStorageUnit* contentSU = somSelf->GetContentStorageUnit(ev, key);

                        // Existing properties inhibit cloning in the same property, so remove all properties
                        // from the content storage unit before cloning.
                        contentSU->Focus(ev, (ODPropertyName) kODNULL, kODPosAll, kODTypeAll, 0, kODPosUndefined);
                        ODULong numProperties = contentSU->CountProperties(ev);
                        ODULong i;
                        for (i = 1; i <= numProperties; i++) {
                                contentSU->Focus(ev, (ODPropertyName) kODNULL, kODPosNextSib, kODTypeAll, 0, kODPosUndefined);
                                contentSU->Remove(ev);
                                }

                        somSelf->CloneFromMemoryContainer(ev, contentSU, dataHandle);

                        successful = kODTrue;

                        ODDisposeHandle(dataHandle);
                CATCH_ALL
                        _fStatus = ErrorCode();
                        ODDisposeHandle(dataHandle);
                        WARN("Throw from ReadEditionFile");
                ENDTRY
        }

        closeError = CloseEdition(edRefNum, successful);
        if ( successful )
                somSelf->SetChangeTime(ev, (**_fSection).mdDate, key);

        THROW_IF_ERROR(readError);
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSource: SetStatus
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceSetStatus(ODLinkSource *somSelf, Environment *ev,
                ODError error)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","SetStatus");

        _fStatus = error;
}

//------------------------------------------------------------------------------
// ODLinkSource: GetStatus
//------------------------------------------------------------------------------

SOM_Scope ODError  SOMLINK ODLinkSourceGetStatus(ODLinkSource *somSelf, Environment *ev)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","GetStatus");

        return _fStatus;
}

//------------------------------------------------------------------------------
// ODLinkSource: CloneFromMemoryContainer
//------------------------------------------------------------------------------
// This method does not call it's draft's SetChangeFromPrev() method so that the
// user is not prompted to save changes if the only change was to update a link
// from an edition file.


SOM_Scope void  SOMLINK ODLinkSourceCloneFromMemoryContainer(ODLinkSource *somSelf, Environment *ev,
                ODStorageUnit* contentSU,
                ODHandle containerHandle)
{
        ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
        ODLinkSourceMethodDebug("ODLinkSource","CloneFromMemoryContainer");

#ifdef _PLATFORM_OS2_
        SOM_TRY
#else
        SOM_CATCH return;
#endif

        ODStorageUnitID         rootID;
        ODDraft*                        myDraft = contentSU->GetDraft(ev);
        ODSession*                      session = somSelf->GetStorageUnit(ev)->GetSession(ev);

        TempODContainer container = GetMemoryContainer(ev, session, containerHandle, kODBentoMemoryContainer);

        TempODDocument document = container->AcquireDocument(ev, kODDefaultDocument);

        TempODDraft draft = document->AcquireBaseDraft(ev, kODDPExclusiveWrite);

        { TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);
          rootID = ODGetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef);
        }

        TempODStorageUnit rootSU = draft->AcquireStorageUnit(ev, rootID);

        ODDraftKey draftKey = draft->BeginClone(ev, myDraft, kODNULL, kODCloneAll);
        TRY
                draft->Clone(ev, draftKey, rootID, contentSU->GetID(ev), 0);

                if ( ODSUExistsThenFocus(ev, rootSU, kODPropContentFrame, kODWeakStorageUnitRef) )
                {
                        ODID contentFrameID = ODGetWeakSURefProp(ev, rootSU, kODPropContentFrame, kODWeakStorageUnitRef);
                        draft->Clone(ev, draftKey, contentFrameID, kODNULLID, 0);
                }
        CATCH_ALL
                draft->AbortClone(ev, draftKey);
                RERAISE;
        ENDTRY
        draft->EndClone(ev, draftKey);

        #ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
        return;
        #endif
}


#ifdef _PLATFORM_OS2_
//------------------------------------------------------------------------------
// ODLinkSource: GetLinkConnectionData
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSourceGetLinkConnectionData(ODLinkSource *somSelf,
                                                           Environment *ev,
                                                          ODLinkConnectionID connID,
                                                          ODLinkConnectionData** ConnData)
{
   ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
   ODLinkSourceMethodDebug("ODLinkSource","DescribeLinkTarget");
   somSelf->SubClassResponsibility(ev);
}

//------------------------------------------------------------------------------
// ODLinkSource: GetConnectionData
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODLinkSourceGetConnectionData(ODLinkSource *somSelf, Environment *ev,
                ODLinkConnectionData** srcConnData)
{
   ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
   ODLinkSourceMethodDebug("ODLinkSource","DescribeSelf");
   somSelf->SubClassResponsibility(ev);
}

//------------------------------------------------------------------------------
// ODLinkSource: GetLinkConnections
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODLinkSourceGetLinkConnections(ODLinkSource *somSelf,
                                                        Environment *ev,
                                                       _IDL_SEQUENCE_unsigned_long* connIDs)
{
   ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
   ODLinkSourceMethodDebug("ODLinkSource","GetLinkTargets");
   somSelf->SubClassResponsibility(ev);
}

//------------------------------------------------------------------------------
// ODLinkSource: InitCallBackMode
//------------------------------------------------------------------------------
/*
 * Called by Part after creation to initialize the mode of tracking
 *   LinkTargets.
 * By default LinkSource only tracks local or registered LinkTargets.
 *void RemoveLinkTarget (in IODConnectionID trgtID);
 */

SOM_Scope void  SOMLINK ODLinkSourceInitCallBackMode(ODLinkSource *somSelf, Environment *ev,
                ODBoolean trackTrgt)
{
   ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
   ODLinkSourceMethodDebug("ODLinkSource","InitCallBackMode");
   somSelf->SubClassResponsibility(ev);
}

//------------------------------------------------------------------------------
// ODLinkSource: BreakLinkConnection
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODLinkSourceBreakLinkConnection(ODLinkSource *somSelf,
                                                         Environment *ev,
                                                        ODLinkConnectionID connID)
{
   ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
   ODLinkSourceMethodDebug("ODLinkSource","RemoveLinkTarget");
   somSelf->SubClassResponsibility(ev);
}

//------------------------------------------------------------------------------
// ODLinkSource: SetDescription
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODLinkSourceSetDescription(ODLinkSource *somSelf,
                                                    Environment *ev,
                                                   ODLinkDescription* desc)
{
   ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf);
   ODLinkSourceMethodDebug("ODLinkSource","SetDescripton");
   somSelf->SubClassResponsibility(ev);
}

/*
 * Called by source Part to get its commentary.
 */

SOM_Scope void  SOMLINK ODLinkSourceGetDescription(ODLinkSource *somSelf,
                                                    Environment *ev,
                                                   ODLinkDescription* desc)
{
    /* ODLinkSourceData *somThis = ODLinkSourceGetData(somSelf); */
    ODLinkSourceMethodDebug("ODLinkSource","ODLinkSourceGetDescription");

}

#endif /*_PLATFORM_OS2_*/

