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
// @(#) 1.7 com/src/storage/LinkMgr.cpp, oddataxfer, od96os2, odos29646d 8/22/96 15:37:17 [ 11/15/96 15:25:53 ]
/*
        File:           LinkMgr.cpp

        Contains:       Implementation for ODLinkManager class.

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <44>     10/8/95        TJ              Fixes Recomended by Refball
                <43>     10/4/95        jpa             Added missing ODVolatiles [1285799]
                <42>     10/3/95        TJ              Changes done by RefBall Team
                <41>     9/27/95        eeh             1284465: use ClipStringToBytes
                <40>      9/1/95        CC              1278590: Added RequestLinkAEHandler.
                                                                        Call SetStatus and UpdateDependents if link
                                                                        cannot be established.
                                                                        1190727: GetLinkAEHandler: Update Edition
                                                                        file if waiting for save.
                <39>     8/30/95        EL              1279582: Storage ODVolatile problems.
                                                                        1279832: Storage THROW_IF_NULL problems.
                <38>     8/29/95        DM              1221778, 1276165:  add SuspendResume(),
                                                                        ShowLinkUpdateAlert(), and _fSuspended-
                                                                        UpdateLinks to let LinkMgr do update
                                                                        alerts.
                <37>     8/26/95        T‚              1274606 FB2: Patching Remarks
                <36>     8/22/95        eeh             1276821:
                                                                        kOpenDocShellSignature->kODShellSignature
                <35>     8/15/95        CC              1275241: Moved declaration of DraftInfo
                                                                        struct into LinkMgr.cpp.
                <34>     8/12/95        T‚              1276812 Need to use TempObjs and TempRefs
                                                                        for exception safety and to avoid TRY
                                                                        blocks, 1276807 Opt./Bug: use StdTypIO
                                                                        routines for portable streaming & smaller
                                                                        footprint
                <33>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <32>     7/28/95        VL              1270320: Do not create draft info in
                                                                        DraftSaved if the draft has not been opened
                                                                        yet.
                <31>     7/25/95        CC              1212338: Created class ODObjectCollection;
                                                                        inherits from OrderedCollection but
                                                                        overrides ElementsMatch to use
                                                                        ODObjectsAreEqual macro.
                <30>     6/26/95        T‚              1242642 BB:Fix refcounting bugs
                <29>      6/7/95        CC              1242442: CreateLink: Don't add link to
                                                                        kODPropImportedLinkSources draft property.
                                                                        GetLinkAEHandler: Don' t add linkSource to
                                                                        kODPropExportedLinks draft property.
                                                                        Removed local function
                                                                        AppendStrongReferenceToDraftProperty.
                <28>      6/2/95        CC              1255474: AcquireLinkAEHandler restored to
                                                                        GetLinkAEHandler
                                                                        1255474: kODAcquireLinkEventID restored to
                                                                        kODGetLinkEventID
                <27>     5/26/95        VL              1251403: Multithreading naming support.
                <26>     5/22/95        CC              1251802: Added TruncateFileName.
                <25>     4/25/95        CC              1242555: DeleteEditionFiles: $5 entered as
                                                                        bug 1242448.
                                                                        1242555: CreateLink,AcquireLinkAEHandler: $5
                                                                        entered as bug 1242442.
                                                                        1242625: GetDraftInfo: Use ODDeleteObject
                                                                        instead of delete operator.
                <24>     4/14/95        T‚              #1235279 BB: InfoUtil & StdTypIO functions
                                                                        should take Environment* and SU* when
                                                                        possible
                <23>      3/7/95        CC              1224121 ODPart::CreateLink() and
                                                                        ODLinkManager::CreateLink() return
                                                                        ODLinkSource object.
                <22>      3/3/95        CC              1223667 ExportOnSave() and DeleteOnSave
                                                                        return previous setting as boolean result.
                <21>     2/24/95        CC              1153783: Made edition name localizable by
                                                                        using ReplaceIntoString.
                <20>     2/15/95        CC              1220327 InitLinkManager: Call
                                                                        InitLinkDialogs to set session global.
                                                                        1196831 Use PStrToText; remove static
                                                                        function NumToText.
                                                                        1220318 BuildErrorString calls
                                                                        ReplaceIntoString.
                <19>      2/1/95        CC              1153347, 1154916, 1213004:
                                                                        GetSectionFromEvent returns error rather
                                                                        than throwing.
                                                                        SectionScrollAEHandler: Alert the user via
                                                                        the notification manager.
                                                                        Added BuildErrorString() and NumToText().
                                                                        Support fNMResponseUPP and fNMRecPtr
                                                                        fields.
                <18>     1/27/95        TJ              VL: some stuff.
                <17>     1/26/95        VL              #???: Use updated Storage Unit Ref API.
                <16>     1/19/95        CC              1210935, 1207937 Add DeleteOnSave() method.
                                                                        1212315 Use shell's signature for Apple
                                                                        event class.
                                                                        1191890 - Code review changes.
                <15>    12/22/94        CC              1154916  SectionScrollAEHandler: Let part
                                                                        make process frontmost.
                                                                        1153780 Implemented ReserveSectionID; fixed
                                                                        bug in
                                                                        AppendStrongReferenceToDraftProperty.
                <14>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                <13>    12/15/94        CC              1197221 Changed call to
                                                                        ODPart::CreateLink().
                <12>     12/8/94        CC              1201906 somUninit: fixed parameters to
                                                                        AEGetEventHandler.
                <11>    11/28/94        RR              Fix for Eric - check AE errors
                <10>     10/4/94        CC              1190068 - Add ODLink publishers and
                                                                        ODLinkSource subscribers to draft
                                                                        properties.
                 <9>     9/29/94        RA              1189812: Mods for 68K build.
                 <8>     9/23/94        VL              1184272: ContainerID is now a sequence of
                                                                        octets.
                 <7>     9/19/94        CC              1187316 - somUninit() must call parent
                                                                        method last.
                 <6>     8/19/94        T‚              #1159233 Remove references to j3di
                 <5>     8/11/94        CC              Call ODDraft::CreateLinkIterator() and
                                                                        ODDraft::CreateLinkSourceIterator() rather
                                                                        than ugly cast from ODDraft to CMDraft.
                 <4>     7/31/94        CC              Changed ODDocument parameters to ODDraft
                                                                        since document properties have been
                                                                        eliminated.
                 <3>     7/27/94        CC              Added SOM_CATCH; use
                                                                        GetOriginatingProcessID() method of
                                                                        ODLinkSpec class.
                 <2>      7/6/94        CC              Temporary work-around for nil result from
                                                                        GetODLinkIterator(),
                                                                        GetODLinkSourceIterator(), and
                                                                        AcquireDocumentProperties().
                 <1>     6/21/94        CC              first checked in

                ------------------- CC ---- Converted to SOM

                 <4>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <3>      4/4/94        CC              SectionScrollAEHandler: Call link source's
                                                                        ShowSourceContent() method for registered
                                                                        sections. (1153347)
                 <2>     3/28/94        CG              1153547: XMPSessn.h renamed to XMPSessM.h
                 <1>     3/25/94        CC              first checked in

                ------------------- CC ---- Moved from Core to Storage subsystem and
                                                                        renamed from LinkMgr.cpp

                 <8>     3/25/94        eeh             bug #1153053: Changes for PPC nativity.
                 <7>     3/25/94        VL              #1147173: Draft.h to CMDraft.
                 <6>     3/17/94        CG              #1151392: Moved kOpenDocShellSignature from
                                                                        ContsDef.h to StdDefs.h
                 <5>     3/15/94        MB              Changes to support SCpp/ASLM builds,
                                                                        #1150864.
                 <4>      3/2/94        CC              CreateLink: Increment ref count of returned
                                                                        object (1144936).  Removed DebugStrs
                                                                        (1144942).
                 <3>      3/1/94        CC              Declared local functions static.
                 <2>     2/15/94        CC              Bug #1142928 - Added document parameter to
                                                                        ReserveSectionID.
                <12>      2/8/94        JA              Minor tweaks for CodeWarrior.
                <11>      2/8/94        T‚              Throw -> THROW & some code clean up
                <10>      2/7/94        T‚              more fixes to compile with PPC Headers
                 <9>      2/4/94        T‚              fixes to compile with PPC Universal headers
                 <8>     1/21/94        CC              Moved InitEditionPack() call into shell;
                                                                        ExportOnSave takes doExport parameter; made
                                                                        RemoveAllLinksInCollection and
                                                                        DeleteEditionFiles non-class functions.
                 <7>     1/18/94        CC              Added AnyLinkImported(); removed 'j3di'
                                                                        literal.
                 <6>     1/14/94        CC              Added methods UnsavedExportedLinks(),
                                                                        DraftOpened(), DraftSaved(), DraftClosed(),
                                                                        and private support routines; added
                                                                        DraftInfo type and fDraftInfos field.
                 <5>    12/22/93        CC              CreateLink - dispose targetAddress
                                                                        descriptor.
                 <4>    12/22/93        CC              Added DraftOpened(); Dispose Apple event
                                                                        descriptors in CreateLink().
                 <3>    12/22/93        CC              NextGlobalID() and callers uses XMPDocument
                                                                        instead of XMPDraft.  Handle section write
                                                                        events by calling links WriteEditionFile()
                                                                        method.  Improved error handling in several
                                                                        methods.
                 <2>    12/16/93        CC              Install handlers for Edition Manager
                                                                        events.  Moved code from
                                                                        XMPDraft::AcquireLink() into CreateLink().
                                                                        Calls link objects on receiving AcquireLink and
                                                                        Edition Manager events.
                 <1>     12/8/93        CC              first checked in
                 <2>     12/3/93        T‚              Stop including XMPError.h, it is included
                                                                        as ErrorDef.h inside Except.h
                 <1>    11/15/93        CC              first checked in
        To Do:
        In Progress:
*/

#define VARIABLE_MACROS

// Commented out for now as OSA support is yet to be defined.
/*
#ifdef _PLATFORM_OS2_
 #ifndef OSA_INCLUDED
 #define INCL_OSA
 #include <osa.h>
 #endif

 #ifndef SOM_ODOSAEvent_xh
 #include "ODOSAEvt.xh"
 #endif
#endif
*/


#define ODLinkManager_Class_Source
#include <LinkMgr.xih>

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifdef _PLATFORM_OS2_
#include <ODPagtun.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifndef _PASCLSTR_
#include <PasclStr.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
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

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODLink_xh
#include <Link.xh>
#endif

#ifndef SOM_ODLinkSource_xh
#include <LinkSrc.xh>
#endif

#ifndef SOM_ODLinkSpec_xh
#include <LinkSpec.xh>
#endif

#ifndef SOM_ODLinkIterator_xh
#include <LinkItr.xh>
#endif

#ifndef SOM_ODLinkSourceIterator_xh
#include <LkSrcItr.xh>
#endif

#ifndef _LINKDEFS_
#include <LinkDefs.h>
#endif

#ifndef _LINKDLGS_
#include <LinkDlgs.h>
#endif

#ifndef _ORDCOLL_
#include <OrdColl.h>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef SOM_Module_OpenDoc_Errors_defined
#include <ErrorDef.xh>
#endif

#ifdef _PLATFORM_OS2_ // CED
#include <builtin.h>
#else
#ifndef __ERRORS__
#include <Errors.h>
#endif
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _REFCTCOL_
#include "RefCtCol.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _USERSRCM_
#include <UseRsrcM.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
// For smSystemScript
#ifndef __SCRIPT__
#include <Script.h>
#endif

#ifndef __TEXTUTILS__
#include <TextUtils.h>
#endif

#ifndef _DLOGUTIL_
#include <DlogUtil.h>
#endif

#endif //

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __GESTALT__
#include <Gestalt.h>
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __NOTIFICATION__
#include <Notification.h>
#endif

#pragma segment ODLinkManager
#endif

#ifdef _PLATFORM_OS2_
#ifndef _ODUTILS_
#include <odutils.h>
#endif
#endif

//==============================================================================
// Constants
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
const Boolean kNotSysHandler = false;
#else
const Boolean kNotSysHandler = kODFalse;
#endif

// Apple event constants
#define kODGetLinkEventID               'gtln'
#define kODRequestLinkEventID   'rqln'
#define keyEditionAlias                 'edas'
#define kODLinkSourceKey                'lksr'

// Value type
const  ODValueType  kODULongSequence            = "OpenDoc:Type:ULongSequence";

const   ODUShort        kODStorageUnitRefLen    = 4;
const   ODUShort        kODULongSize                    = 4;

//==============================================================================
// Local Function Prototypes
//==============================================================================
ODStatic StringPtr BuildErrorString(ODSShort strResourceID, ODError error);

ODStatic EditionContainerSpec NewEditionFile(Environment* ev,
                        PlatformFile* documentFile,
                        ODDraft* draft);

ODStatic void EditionFileName(Str255 fileName, ODUShort numb);
ODStatic void TruncateFileName(Str255 fileName, Str63 numberString);

ODStatic ODULong NewEditionID(Environment *ev, ODDraft* draft);

ODStatic void RegisterPublishingLinks(Environment *ev, ODDraft* draft);
ODStatic void RegisterSubscribingLinks(Environment *ev, ODDraft* draft);
ODStatic void UpdateEditionFiles(Environment *ev, ODRefCntCollection* collection);
ODStatic void DeleteEditionFiles(Environment *ev, ODRefCntCollection* collection);
// ODStatic void RemoveAllLinksInCollection(Environment *ev, OrderedCollection* collection);
#ifdef _PLATFORM_MACINTOSH_
ODStatic OSErr GetSectionFromEvent(const AppleEvent* theAppleEvent,
        SectionHandle* section);
#else
// Commented out for now as OSA support is yet to be defined.
// ODStatic OSErr GetSectionFromEvent(const ODOSAEvent* theOSAEvent);
#endif
ODStatic void AppendULongToDraftProperty(Environment *ev,
        ODDraft* draft, ODPropertyName prop, ODULong value);
ODStatic ODBoolean SectionIDReserved(Environment *ev,
        ODDraft* draft, ODULong requestedID);
ODStatic ODULong PeekNextGlobalID(Environment *ev,
        ODDraft* draft,
        ODPropertyName globalProp);
ODStatic ODULong ReserveGlobalID(Environment *ev,
        ODDraft* draft,
        ODPropertyName globalProp);
#ifdef _PLATFORM_MACINTOSH_
ODStatic OSErr ODPascal GetLinkAEHandler(
                                                        const AppleEvent* theAppleEvent,
                                                        AppleEvent* reply,
                                                        long refCon);
ODStatic OSErr ODPascal RequestLinkAEHandler(
                                                        const AppleEvent* theAppleEvent,
                                                        AppleEvent* reply,
                                                        long refCon);
ODStatic OSErr ODPascal SectionReadAEHandler(
                                                        const AppleEvent* theAppleEvent,
                                                        const AppleEvent* reply,
                                                        long refCon);
ODStatic OSErr ODPascal SectionWriteAEHandler(
                                                        const AppleEvent* theAppleEvent,
                                                        const AppleEvent* reply,
                                                        long refCon);
ODStatic OSErr ODPascal SectionScrollAEHandler(
                                                        const AppleEvent* theAppleEvent,
                                                        const AppleEvent* reply,
                                                        long refCon);
#endif //

//==============================================================================
// struct DraftInfo
//==============================================================================

struct DraftInfo
{
        ODDraft*                draft;
        ODRefCntCollection*     linksToExport;
        ODRefCntCollection*     linksToDelete;
        ODRefCntCollection*     newExportedLinks;
        ODBoolean                       linksImported;
};

// ODObjectCollection is replaced by ODObjectOrdColl in RefCtCol.h in Utilities

//==============================================================================
// class ODObjectCollection
//==============================================================================
//
// class ODObjectCollection : public OrderedCollection
// {
//      public:
//              ODObjectCollection() : OrderedCollection()      {}
//              ODVMethod ~ODObjectCollection()                         {}
//
//              ODMethod ODBoolean ElementsMatch(ElementType v1,ElementType v2) const;
// };

//------------------------------------------------------------------------------
// ODObjectCollection::ElementsMatch (OVERRIDE)
//------------------------------------------------------------------------------

// ODBoolean ODObjectCollection::ElementsMatch(ElementType v1,ElementType v2) const
// {
//      Environment* ev = somGetGlobalEnvironment();
//      return ODObjectsAreEqual(ev, (ODObject*) v1, (ODObject*) v2);
// }

//==============================================================================
// Static functions
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
// LinkMgrNMResponse
//------------------------------------------------------------------------------

ODStatic void ODPascal LinkMgrNMResponse(NMRecPtr theNMRecPtr)
{
        OSErr error = NMRemove(theNMRecPtr);
}
#endif

//==============================================================================
// class ODLinkManager
//==============================================================================

//------------------------------------------------------------------------------
// ODLinkManager::InitLinkManager
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkManagerInitLinkManager(ODLinkManager *somSelf, Environment *ev,
                ODSession* session)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","InitLinkManager");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#else
        SOM_TRY
#endif

#ifdef _PLATFORM_MACINTOSH_ // CED
        /* Moved from somInit. SOM itself sets fields to zero
        _fSuspendedUpdateLinks = (ODRefCntCollection*) kODNULL;
        _fDraftInfos = (OrderedCollection*) kODNULL;
        _fNMResponseUPP = (NMUPP) kODNULL;
        _fNMRecPtr = (NMRecPtr) kODNULL;
        */
        somSelf->InitObject(ev);

        long  gestaltResult;
        OSErr result;

        // Check if the Edition manager is present
        result = Gestalt(gestaltEditionMgrAttr, &gestaltResult);
        THROW_IF_ERROR (result, "Error from Gestalt");

        // The mysterious 31 is the bit position as BitTst uses a convention opposite to MC68000
        if ( BitTst(&gestaltResult, (31 - gestaltEditionMgrPresent)) == false )
                THROW(kODErrNoEditionManager);

        InitLinkDialogs(session);

        _fSuspendedUpdateLinks = new ODRefCntCollection(ev);
        _fDraftInfos = new OrderedCollection;
        _fNMResponseUPP = NewNMProc(LinkMgrNMResponse);
        _fNMRecPtr = (NMRecPtr) ODNewPtr(sizeof(NMRec));
        THROW_IF_NULL(_fNMRecPtr);
        _fNMRecPtr->nmStr = kODNULL;

        AEEventHandlerUPP theHandlerUPP ;
        theHandlerUPP = NewAEEventHandlerProc( GetLinkAEHandler ) ;
        THROW_IF_ERROR(AEInstallEventHandler(kODShellSignature,
                        kODGetLinkEventID, theHandlerUPP, (long)somSelf, kNotSysHandler));

        theHandlerUPP = NewAEEventHandlerProc( RequestLinkAEHandler ) ;
        THROW_IF_ERROR(AEInstallEventHandler(kODShellSignature,
                        kODRequestLinkEventID, theHandlerUPP, (long)somSelf, kNotSysHandler));

        theHandlerUPP = NewAEEventHandlerProc( SectionReadAEHandler ) ;
        THROW_IF_ERROR(AEInstallEventHandler(sectionEventMsgClass, sectionReadMsgID,
                        theHandlerUPP, (long)somSelf, kNotSysHandler));

        theHandlerUPP = NewAEEventHandlerProc( SectionWriteAEHandler );
        THROW_IF_ERROR(AEInstallEventHandler(sectionEventMsgClass, sectionWriteMsgID,
                        theHandlerUPP, (long)somSelf, kNotSysHandler));

        theHandlerUPP = NewAEEventHandlerProc( SectionScrollAEHandler );
        THROW_IF_ERROR(AEInstallEventHandler(sectionEventMsgClass, sectionScrollMsgID,
                        theHandlerUPP, (long)somSelf, kNotSysHandler));

#endif //

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::NewSectionID
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODLinkManagerNewSectionID(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","NewSectionID");

        ODULong sectionID;

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return 0;
#else
        SOM_TRY
#endif

        do
                sectionID = ReserveGlobalID(ev, draft, kODPropSectionID);
        while
                ( SectionIDReserved(ev, draft, sectionID) );

        return sectionID;

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return 0;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::ReserveSectionID
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkManagerReserveSectionID(ODLinkManager *somSelf, Environment *ev,
                ODULong sectionID,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","ReserveSectionID");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif

        ODBoolean result = kODFalse;

        if ( sectionID >= PeekNextGlobalID(ev, draft, kODPropSectionID) )
                if ( !SectionIDReserved(ev, draft, sectionID) )
                {
                        AppendULongToDraftProperty(ev, draft, kODPropReservedSectionIDs, sectionID);
                        result = kODTrue;
                }

        return result;

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return kODFalse;
#endif
}

#ifdef _PLATFORM_OS2_
//-------------------------------------------------------------------------------------
// ODLinkManager: ShowLinkUpdateAlert
//-------------------------------------------------------------------------------------

SOM_Scope ODLinkUpdateResult  SOMLINK ODLinkManagerShowLinkUpdateAlert(ODLinkManager *somSelf, Environment *ev,
                ODBaseLinkSource* link)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","ShowLinkUpdateAlert");

    ODLinkUpdateResult result = kODLinkUpdateContinue; ODVolatile(result);

    SOM_TRY
    result = ( ::ShowLinkUpdateAlert() )?
           kODLinkUpdateContinue : kODLinkUpdateStop;

    SOM_CATCH_ALL

    SOM_ENDTRY

    return result;
}
#endif

#ifdef _PLATFORM_MACINTOSH_
//-------------------------------------------------------------------------------------
// ODLinkManager: ShowLinkUpdateAlert
//-------------------------------------------------------------------------------------

SOM_Scope ODLinkUpdateResult  SOMLINK ODLinkManagerShowLinkUpdateAlert(ODLinkManager *somSelf, Environment *ev,
                ODBaseLinkSource* link)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","ShowLinkUpdateAlert");

    ODLinkUpdateResult result = kODLinkUpdateContinue; ODVolatile(result);

    SOM_TRY

                // When we leave the background, iterate over all the link sources
                // that attempted to call ShowLinkUpdateAlert() in the background,
                // showing the alert now and calling the links back with the
                // resulting information.

                if ( _fInBackground )
                {
                        result = kODLinkUpdateStop;
                        if ( !_fSuspendedUpdateLinks->Contains(link) )
                                _fSuspendedUpdateLinks->AddLastAndAcquire(link);
                        result = kODLinkUpdateSuspend;
                }
                else
                {
                        result = ( ::ShowLinkUpdateAlert() )?
                                kODLinkUpdateContinue : kODLinkUpdateStop;
                }

        SOM_CATCH_ALL

        SOM_ENDTRY

        return result;
}
#endif

//-------------------------------------------------------------------------------------
// ODLinkManager: SuspendResume
//-------------------------------------------------------------------------------------

#if ODDebug
extern int gODButtonKeyNoEnterOkay;
#endif
#ifdef _PLATFORM_MACINTOSH_
SOM_Scope void  SOMLINK ODLinkManagerSuspendResume(ODLinkManager *somSelf, Environment *ev,
                ODEventData* event)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","SuspendResume");

        const short kResumeMask = 0x01; // High byte suspend/resume event

        ODBoolean inBackground = (event->message & kResumeMask) == 0;
        _fInBackground = inBackground;

    SOM_TRY

                // When we leave the background, show the alert and iterate over all the
                // link sources that attempted to call ShowLinkUpdateAlert() in the
                // background, calling the links back with the resulting alert information.

                if ( !inBackground )
                {
                        ODBaseLinkSource* link = (ODBaseLinkSource*) _fSuspendedUpdateLinks->RemoveFirst();
                        if (link)
                        {

                                ODLinkUpdateResult result = ( ::ShowLinkUpdateAlert() )?
                                        kODLinkUpdateContinue : kODLinkUpdateStop;

                                while ( link )
                                {
                                        TempODRefCntObject tempLink = link; // ensure it is released

                                        // Ignore errors returned by LinkUpdateAlertCompleted
                                        TRY
                                                link->LinkUpdateAlertCompleted(ev, result);
                                        CATCH_ALL
                                        ENDTRY

                                        link = (ODBaseLinkSource*) _fSuspendedUpdateLinks->RemoveFirst();
                                }
                        }
                }

        SOM_CATCH_ALL

        SOM_ENDTRY
}
#endif

//------------------------------------------------------------------------------
// ODLinkManager::UnsavedExportedLinks
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkManagerUnsavedExportedLinks(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","UnsavedExportedLinks");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif

        DraftInfo* info = somSelf->GetDraftInfo(ev, draft);

        if ( info != (DraftInfo*) kODNULL )
        {
                return info->newExportedLinks->Count() > 0;
        }
        else
                return kODFalse;

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return kODFalse;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::AnyLinkImported
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkManagerAnyLinkImported(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","AnyLinkImported");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif
        DraftInfo* info = somSelf->GetDraftInfo(ev, draft);

        if ( info != (DraftInfo*) kODNULL )
        {
                return info->linksImported;
        }
        else
                return kODFalse;

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return kODFalse;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::DraftOpened
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkManagerDraftOpened(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","DraftOpened");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#else
        SOM_TRY
#endif

        DraftInfo* info = somSelf->GetDraftInfo(ev, draft);

        if ( info == (DraftInfo*) kODNULL )
        {
                somSelf->NewDraftInfo(ev, draft);
                RegisterSubscribingLinks(ev, draft);
                RegisterPublishingLinks(ev, draft);
        }


#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::DraftSaved
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkManagerDraftSaved(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","DraftSaved");


#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#else
        SOM_TRY
#endif
        DraftInfo* info = somSelf->GetDraftInfo(ev, draft);

        if ( info != (DraftInfo*) kODNULL )
        {
                UpdateEditionFiles(ev, info->linksToExport);
                // old: RemoveAllLinksInCollection(ev, info->linksToExport);
                info->linksToExport->RemoveAndReleaseAll();

                DeleteEditionFiles(ev, info->linksToDelete);
                // old: RemoveAllLinksInCollection(ev, info->linksToDelete);
                info->linksToDelete->RemoveAndReleaseAll();

                // old: RemoveAllLinksInCollection(ev, info->newExportedLinks);
                info->newExportedLinks->RemoveAndReleaseAll();

                info->linksImported = kODFalse;
        }
        else
        {
//                WARN("Draft is not opened yet.");
        }

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::DraftClosing
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkManagerDraftClosing(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","DraftClosing");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#else
        SOM_TRY
#endif

        DraftInfo* info = somSelf->GetDraftInfo(ev, draft);

        if ( info != (DraftInfo*) kODNULL )
        {
                // old: RemoveAllLinksInCollection(ev, info->linksToExport);
                // new: info->linksToExport->RemoveAndReleaseAll();
                ODDeleteObject(info->linksToExport); // calls RemoveAndReleaseAll()

                // old: RemoveAllLinksInCollection(ev, info->linksToDelete);
                // new: info->linksToDelete->RemoveAndReleaseAll();
                ODDeleteObject(info->linksToDelete); // calls RemoveAndReleaseAll()

                DeleteEditionFiles(ev, info->newExportedLinks);
                // old: RemoveAllLinksInCollection(ev, info->newExportedLinks);
                // new: info->newExportedLinks->RemoveAndReleaseAll();
                ODDeleteObject(info->newExportedLinks); // calls RemoveAndReleaseAll()

                _fDraftInfos->Remove(info);
                delete info;
        }


#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::ExportOnSave
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkManagerExportOnSave(ODLinkManager *somSelf, Environment *ev,
                ODLink* link,
                ODBoolean doExport)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","ExportOnSave");

        ODBoolean wereToExport = kODFalse;

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif

        DraftInfo* info = somSelf->GetDraftInfo(ev, link->GetStorageUnit(ev)->GetDraft(ev));
        if ( info != (DraftInfo*) kODNULL )
        {
                wereToExport = info->linksToExport->Contains(link);
                if ( wereToExport && !doExport )
                {
                        info->linksToExport->RemoveAndRelease(link);
                        // link->Release(ev);
                }
                else if ( !wereToExport && doExport )
                {
                        info->linksToExport->AddLastAndAcquire(link);
                        // link->Acquire(ev);
                }
        }

        return wereToExport;

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return kODFalse;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::DeleteOnSave
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkManagerDeleteOnSave(ODLinkManager *somSelf, Environment *ev,
                ODLink* link,
                ODBoolean doDelete)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","DeleteOnSave");

        ODBoolean wereToDelete = kODFalse;


#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif

        DraftInfo* info = somSelf->GetDraftInfo(ev, link->GetStorageUnit(ev)->GetDraft(ev));
        if ( info != (DraftInfo*) kODNULL )
        {
                wereToDelete = info->linksToDelete->Contains(link);
                if ( wereToDelete && !doDelete )
                {
                        info->linksToDelete->RemoveAndRelease(link);
                        // link->Release(ev);
                }
                else if ( !wereToDelete && doDelete )
                {
                        info->linksToDelete->AddLastAndAcquire(link);
                        // link->Acquire(ev);
                }
        }

        return wereToDelete;

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return kODFalse;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::CreateLink
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK ODLinkManagerCreateLink(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft,
                ODLinkSpec* linkSpec)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","CreateLink");


#ifdef _PLATFORM_OS2_ // CED
  _interrupt(3);
  return 0;
#else
        ODLinkSource*                   linkSource = kODNULL;   ODVolatile(linkSource);
        AppleEvent                              theAppleEvent, replyAppleEvent;
        AEAddressDesc                   targetAddress;
        OSErr                                   error;
        ProcessSerialNumber     psn;

        SOM_TRY

                // Create a link source to return
                linkSource = draft->CreateLinkSource(ev, (ODPart*) kODNULL);

                // Send an Apple Event to the this process using GetCurrentProcess
                // so event is delivered as a high-level event
                GetCurrentProcess(&psn);
                error = AECreateDesc(typeProcessSerialNumber, &psn,
                                                        sizeof(ProcessSerialNumber), &targetAddress);
                THROW_IF_ERROR(error);
                TempAEDesc tmpTargetAddress(&targetAddress);

                error = AECreateAppleEvent(kODShellSignature, kODRequestLinkEventID,
                                                        &targetAddress, kAutoGenerateReturnID,
                                                        kAnyTransactionID, &theAppleEvent);
                THROW_IF_ERROR(error);
                TempAEDesc tmpAppleEvent(&theAppleEvent);

                // Add the link spec to the Apple event
                linkSpec->WriteToAppleEvent(ev, &theAppleEvent);

                // Add the source link to the Apple event
                error = AEPutParamPtr(&theAppleEvent, kODLinkSourceKey, typeLongInteger,
                                                        &linkSource, sizeof(linkSource));
                THROW_IF_ERROR(error);

                error = AESend(&theAppleEvent, &replyAppleEvent, kAENoReply | kAENeverInteract,
                                                        kAENormalPriority, kAEDefaultTimeout, NULL, NULL);
                THROW_IF_ERROR(error);

        SOM_CATCH_ALL

                ODSafeReleaseObject(linkSource);
                linkSource = kODNULL;

        SOM_ENDTRY

        return linkSource;

#endif
}

#ifdef _PLATFORM_MACINTOSH_ // CED
//------------------------------------------------------------------------------
// GetLinkAEHandler
//------------------------------------------------------------------------------

ODStatic OSErr ODPascal GetLinkAEHandler(
                const AppleEvent* theAppleEvent,
                AppleEvent* reply,
                long refCon)
{
        ODLinkManager*          somSelf = (ODLinkManager*)refCon;
    Environment*                ev = somGetGlobalEnvironment ();
        ODLinkSpec*                     linkSpec = (ODLinkSpec*) kODNULL; ODVolatile(linkSpec);
        ODPart*                         part;
        ODLinkKey                       key;
        AliasHandle                     editionAlias;
        OSErr                           error = noErr;
        PlatformFile*           documentFile = kODNULL; ODVolatile(documentFile);

#if ODDebug
        somPrintf("ODLinkManager: Get Link event received\n");
#endif

        TRY
                linkSpec = new ODLinkSpec;      // Can't use factory method; no draft object!
                THROW_IF_NULL(linkSpec, kODErrOutOfMemory);
                linkSpec->ReadFromAppleEvent(ev, (AppleEvent*) theAppleEvent);

                part = linkSpec->GetPart(ev);

                DraftInfo* info = somSelf->GetDraftInfo(ev, part->GetStorageUnit(ev)->GetDraft(ev));
                if ( info == (DraftInfo*) kODNULL )
                {
                        // Document has never been saved, so refuse to create a link
                        error = kODErrDocNotSaved;
                }
                else
                {
                        TempODLink link = kODNULL;
                        ODByteArray partData = linkSpec->GetPartData(ev);
                        ODVolatile(partData);
                        {
                                TempODLinkSource linkSource = kODNULL;
                                ODVolatile(linkSource);
                                TRY
                                        linkSource = part->CreateLink(ev, &partData);
                                        THROW_IF_NULL(linkSource,kODErrCannotCreateLink);
                                        link = linkSource->GetLink(ev);
                                        THROW_IF_NULL(link,kODErrCannotCreateLink);
                                        link->Acquire(ev);
                                CATCH_ALL
                                        DisposeByteArrayStruct(partData);
                                        RERAISE;
                                ENDTRY
                                DisposeByteArrayStruct(partData);
                        }

                        if ( link->Lock(ev,0,&key) )
                        {
                                TRY
                                        editionAlias = link->GetLinkEdition(ev, key);
                                        if ( editionAlias == (AliasHandle) kODNULL )
                                        {
                                                ODDraft* draft = part->GetStorageUnit(ev)->GetDraft(ev);
                                                ODContainer* container = draft->GetDocument(ev)->GetContainer(ev);;

                                                documentFile = GetPlatformFileFromContainer(ev, container);
                                                ODFileSpec documentSpec = documentFile->GetFileSpec();

                                                EditionContainerSpec editionContainer = NewEditionFile(ev, documentFile, draft);

                                                editionAlias = link->CreateLinkEdition(ev,
                                                                                                editionContainer,
                                                                                                &documentSpec,
                                                                                                somSelf->NewSectionID(ev, draft),
                                                                                                key);

                                                info->newExportedLinks->AddLastAndAcquire(link);
                                                // link->Acquire(ev);
                                        }
                                        else
                                        {
                                                // If a new cross-document destination is being created, and the edition
                                                // file is waiting for a save to update, force the edition file to update
                                                // now so the link content is the same as the content that would be pasted.
                                                // [cc; #1190727]
                                                DraftInfo* info = somSelf->GetDraftInfo(ev, link->GetStorageUnit(ev)->GetDraft(ev));
                                                if ( info && info->linksToExport->Contains(link) )
                                                {
                                                        TRY
                                                                link->UpdateLinkEdition(ev, key);
                                                                info->linksToExport->RemoveAndRelease(link);
                                                        CATCH_ALL
                                                        ENDTRY
                                                }
                                        }
                                CATCH_ALL
                                        link->Unlock(ev, key);
                                        ODDeleteObject(documentFile);
                                        RERAISE;
                                ENDTRY
                                link->Unlock(ev, key);
                                ODDeleteObject(documentFile);
                        }

                        error = AEPutParamPtr(reply, keyEditionAlias, typeAlias,
                                                                &editionAlias, sizeof(editionAlias));
                }
        CATCH_ALL

                // Must catch any exceptions thrown!
                error = ErrorCode();
        ENDTRY

        delete linkSpec;

        return error;
}

//------------------------------------------------------------------------------
// RequestLinkAEHandler
//------------------------------------------------------------------------------

ODStatic OSErr ODPascal RequestLinkAEHandler(
                const AppleEvent* requestLinkAppleEvent,
                AppleEvent* reply,
                long refCon)
{
        ODLinkManager*                  somSelf = (ODLinkManager*) refCon;
    Environment*                        ev = somGetGlobalEnvironment();
        AppleEvent                              getLinkAppleEvent;
        AppleEvent                              replyAppleEvent;
        AEAddressDesc                   targetAddress;
        ProcessSerialNumber     psn;
        DescType                                returnedType;
        Size                                    actualSize;
        AliasHandle                             editionAlias;
        EditionContainerSpec    editionContainer;
        OSErr                                   error;
        ODBoolean                               wasChanged;
        const long                              kGetLinkReplyTimeout = 600;     // Ten seconds

        ODLinkSource*   linkSource = kODNULL;   ODVolatile(linkSource);
        ODLinkSpec*             linkSpec = kODNULL;             ODVolatile(linkSpec);

        TRY

                linkSpec = new ODLinkSpec;      // Can't use factory method; no draft object!
                THROW_IF_NULL(linkSpec, kODErrOutOfMemory);

                linkSpec->ReadFromAppleEvent(ev, (AppleEvent*) requestLinkAppleEvent);

                error = AEGetParamPtr(requestLinkAppleEvent, kODLinkSourceKey,
                                                                typeLongInteger, &returnedType,
                                                                &linkSource, sizeof(linkSource), &actualSize);
                THROW_IF_ERROR(error);

                // Create the GetLink event

                linkSpec->GetOriginatingProcessID(ev, &psn);

                error = AECreateDesc(typeProcessSerialNumber, &psn,
                                                        sizeof(ProcessSerialNumber), &targetAddress);
                THROW_IF_ERROR(error);
                TempAEDesc tmpTargetAddress(&targetAddress);

                error = AECreateAppleEvent(kODShellSignature, kODGetLinkEventID,
                                                        &targetAddress, kAutoGenerateReturnID,
                                                        kAnyTransactionID, &getLinkAppleEvent);
                THROW_IF_ERROR(error);
                TempAEDesc tmpGetLinkAppleEvent(&getLinkAppleEvent);

                linkSpec->WriteToAppleEvent(ev, &getLinkAppleEvent);

                error = AESend(&getLinkAppleEvent, &replyAppleEvent,
                                                        kAEWaitReply | kAENeverInteract, kAENormalPriority,
                                                        kGetLinkReplyTimeout, NULL, NULL);
                TempAEDesc tmpReplyAppleEvent(&replyAppleEvent);
                THROW_IF_ERROR(error);

                // Process the reply

                error = AEGetParamPtr(&replyAppleEvent, keyEditionAlias, typeAlias, &returnedType,
                                                                &editionAlias, sizeof(editionAlias), &actualSize);
                THROW_IF_ERROR(error);

                error = ResolveAlias(nil, editionAlias, &editionContainer.theFile, &wasChanged);
                THROW_IF_ERROR(error);

                editionContainer.theFileScript = smSystemScript;
                editionContainer.thePart = kPartsNotUsed;

                ODDraft* draft = linkSource->GetStorageUnit(ev)->GetDraft(ev);

                ODContainer* container = draft->GetDocument(ev)->GetContainer(ev);;
                ODFileSpec documentSpec = GetODFileSpecFromContainer(ev, container);

                ODLinkKey key;  ODVolatile(key);
                if ( linkSource->Lock(ev,0,&key) )
                {
                        TRY
                                linkSource->UseLinkEdition(ev,
                                                                        editionContainer,
                                                                        &documentSpec,
                                                                        somSelf->NewSectionID(ev, draft),
                                                                        key);
                        CATCH_ALL
                                linkSource->Unlock(ev, key);
                                RERAISE;
                        ENDTRY
                        linkSource->Unlock(ev, key);
                }

        CATCH_ALL

                if ( linkSource )
                {
                        linkSource->SetStatus(ev, kODErrCannotEstablishLink);

                        // Ignore errors returned by UpdateDependents
                        TRY
                                linkSource->GetLink(ev)->UpdateDependents(ev);
                        CATCH_ALL
                        ENDTRY
                }

        ENDTRY

        ODDeleteObject(linkSpec);

        return noErr;
}

//------------------------------------------------------------------------------
// SectionReadAEHandler
//------------------------------------------------------------------------------

ODStatic OSErr ODPascal SectionReadAEHandler(
                const AppleEvent* theAppleEvent,
                const AppleEvent* reply,
                long refCon)
{
        ODUnused(reply);

        ODLinkManager*          somSelf = (ODLinkManager*)refCon;
    Environment*                ev = somGetGlobalEnvironment ();
        SectionHandle           section = kODNULL;
        OSErr                           error;

        error = GetSectionFromEvent(theAppleEvent, &section);

#if ODDebug
                somPrintf("ODLinkManager: Section Read event received; section = %x, linkSource = %x\n",
                                        section, (**section).refCon);
#endif

        if ( error == noErr )
        {
                if ( IsRegisteredSection(section) == noErr )
                {
                        ODLinkSource* linkSource = (ODLinkSource*) (*section)->refCon;
                        ODLinkKey key;

                        if ( linkSource->Lock(ev,0,&key) )
                        {
                                TRY
                                        linkSource->ReadLinkEdition(ev,key);
                                        DraftInfo* info = somSelf->GetDraftInfo(ev, linkSource->GetStorageUnit(ev)->GetDraft(ev));
                                        if ( info != (DraftInfo*) kODNULL )
                                        {
                                                info->linksImported = kODTrue;
                                        }
                                CATCH_ALL
                                        error = ErrorCode();
                                ENDTRY
                                linkSource->Unlock(ev, key);
                        }
                }
        }

        return error;
}

//------------------------------------------------------------------------------
// SectionWriteAEHandler
//------------------------------------------------------------------------------

ODStatic OSErr ODPascal SectionWriteAEHandler(
                const AppleEvent* theAppleEvent,
                const AppleEvent* reply,
                long refCon)
{
        ODUnused(reply);

        ODLinkManager*          somSelf = (ODLinkManager*)refCon;
    Environment*                ev = somGetGlobalEnvironment ();
        ODLinkKey                       key;
        SectionHandle           section = kODNULL;
        OSErr                           error;

        error = GetSectionFromEvent(theAppleEvent, &section);

#if ODDebug
                somPrintf("ODLinkManager: Section Write event received; section = %x, link = %x\n",
                                        section, (**section).refCon);
#endif

        if ( error == noErr )
        {
                if ( IsRegisteredSection(section) == noErr )
                {
                        ODLink* link = (ODLink*) (**section).refCon;

                        // Force ExportLinkEdition to write the edition file
                        (**section).mdDate = (TimeStamp) 0;

                        if ( link->Lock(ev,0,&key) )
                        {
                                TRY
                                        link->UpdateLinkEdition(ev, key);
                                CATCH_ALL
                                        error = ErrorCode();
                                ENDTRY
                                link->Unlock(ev, key);
                        }
                }
        }

        return error;
}

//------------------------------------------------------------------------------
// BuildErrorString
//------------------------------------------------------------------------------

ODStatic StringPtr BuildErrorString(ODSShort strResourceID, ODError error)
{
        Str63 numberString;
        Str255 msgStr;

        StringPtr result = kODNULL;

        ODSLong savedRefNum;
        BeginUsingLibraryResources(savedRefNum);

        NumToString(error, numberString);
        ReplaceIntoString(strResourceID, numberString, kODNULL, msgStr);

        if ( msgStr[0] > 0 )
        {
                ODULong ptrSize = msgStr[0]+1;
                result = (StringPtr) ODNewPtr(ptrSize);
                if ( result )
                        ODBlockMove((ODPtr) &msgStr, (ODPtr) result, ptrSize);
        }

        EndUsingLibraryResources(savedRefNum);

        return result;
}

//------------------------------------------------------------------------------
// SectionScrollAEHandler
//------------------------------------------------------------------------------

ODStatic OSErr ODPascal SectionScrollAEHandler(
                const AppleEvent* theAppleEvent,
                const AppleEvent* reply,
                long refCon)
{
        ODUnused(reply);

        ODLinkManager*          somSelf = (ODLinkManager*)refCon;
    ODLinkManagerData*  somThis = ODLinkManagerGetData(somSelf);
    Environment*                ev = somGetGlobalEnvironment ();
        SectionHandle           section = kODNULL;
        OSErr                           error;

        error = GetSectionFromEvent(theAppleEvent, &section);

#if ODDebug
        somPrintf("ODLinkManager: Section Scroll event received; section = %x, link = %x\n",
                                        section, (section ? (**section).refCon : 0));
#endif

        if ( error == noErr )
        {
                if ( IsRegisteredSection(section) == noErr )
                {
                        TRY
                        ODLink* link = (ODLink*) (**section).refCon;

                        if ( link != kODNULL )
                        {
                                DraftInfo* info = somSelf->GetDraftInfo(ev, link->GetStorageUnit(ev)->GetDraft(ev));
                                if ( info && info->linksToDelete && info->linksToDelete->Contains(link) )
                                        THROW(kODErrCannotRevealLink);
                                else
                                        link->ShowSourceContent(ev);
                        }
                        CATCH_ALL
                                error = ErrorCode();

                                NMRemove(_fNMRecPtr);   // ensure not already queued
                                _fNMRecPtr->qType = nmType;
                                _fNMRecPtr->nmMark = 0;
                                _fNMRecPtr->nmIcon = nil;
                                _fNMRecPtr->nmSound = nil;
                                ODDisposePtr((ODPtr) _fNMRecPtr->nmStr);
                                _fNMRecPtr->nmStr = BuildErrorString(kODLinkMgrCannotShowSourceID, error);
                                _fNMRecPtr->nmResp = _fNMResponseUPP;
                                error = NMInstall(_fNMRecPtr);
                        ENDTRY
                }
        }

        return error;
}

#endif // _PLATFORM_OS2_

//------------------------------------------------------------------------------
// NewEditionFile
//------------------------------------------------------------------------------

ODStatic EditionContainerSpec NewEditionFile(Environment* ev,
                        PlatformFile* documentFile,
                        ODDraft* draft)
{
        EditionContainerSpec    editionContainer;
        OSErr                                   error;
        Str255                                  documentName;
        ODFileSpec                              documentSpec = documentFile->GetFileSpec();

#ifdef _PLATFORM_OS2_
_interrupt( 3 );
#else
        do {
                documentFile->GetAsciiName((char *) documentName, 254);
                CToPascalString((char *) documentName);
                EditionFileName(documentName, (ODUShort) NewEditionID(ev, draft));
                error = FSMakeFSSpec(documentSpec.vRefNum,
                                                         documentSpec.parID,
                                                         documentName,
                                                         &(editionContainer.theFile));
                }
        while (error == noErr);

        // Expect a file not found error
        if ( error != fnfErr )
                THROW_IF_ERROR(error);

        editionContainer.theFileScript = smSystemScript;
        editionContainer.thePart = kPartsNotUsed;

#endif
        return editionContainer;
}

//------------------------------------------------------------------------------
// GetSectionFromEvent
//------------------------------------------------------------------------------

#ifdef _OSA_SUPPORT_

#ifdef _PLATFORM_MACINTOSH_
ODStatic OSErr GetSectionFromEvent(const AppleEvent* theAppleEvent,
                                SectionHandle* section)
#else
ODStatic OSErr GetSectionFromEvent(const ODOSAEvent* theAppleEvent,
                                SectionHandle* section)
#endif
{
        DescType                returnedType;
        Size                    actualSize;
        OSErr                   error;

#ifdef _PLATFORM_OS2_ // CED
#ifdef _OSA_SUPPORT_
  _interrupt(3);
#endif
#else


        error = AEGetParamPtr(theAppleEvent, keyDirectObject, typeSectionH, &returnedType,
                                                        section, sizeof(*section), &actualSize);
#if ODDebug
        if ( error != noErr )
                somPrintf("ODLinkManager: Error %d getting section from event\n", error);
#endif

#endif
        return error;
}
#endif

//------------------------------------------------------------------------------
// EditionFileName
//------------------------------------------------------------------------------

ODStatic void EditionFileName(Str255 fileName, ODUShort numb)
{
        Str63 numberString;

#ifdef _PLATFORM_OS2_
_interrupt( 3 );
#else
        NumToString(numb, numberString);

        TruncateFileName(fileName, numberString);

        ODSLong savedRefNum;
        BeginUsingLibraryResources(savedRefNum);

        ReplaceIntoString(kODLinkMgrEditionTemplateID, fileName, numberString, fileName);

        EndUsingLibraryResources(savedRefNum);

#endif
}

//------------------------------------------------------------------------------
// TruncateFileName
//------------------------------------------------------------------------------

ODStatic void TruncateFileName(Str255 fileName, Str63 numberString)
{
        const maxFileNameLength = 31;

#ifdef _PLATFORM_OS2_
_interrupt( 3 );
#else

        Str255 emptyString;
        emptyString[0] = (unsigned char) 0;

        ODSLong savedRefNum;
        BeginUsingLibraryResources(savedRefNum);

        ReplaceIntoString(kODLinkMgrEditionTemplateID, emptyString, numberString, emptyString);

        EndUsingLibraryResources(savedRefNum);

        if ( (StrLength(fileName)+StrLength(emptyString)) > maxFileNameLength )
                ClipStringToBytes( fileName,
                                maxFileNameLength-StrLength(emptyString), smCurrentScript );

#endif
}

//------------------------------------------------------------------------------
// NewEditionID
//------------------------------------------------------------------------------

ODStatic ODULong NewEditionID(Environment *ev, ODDraft* draft)
{
        return ReserveGlobalID(ev, draft, kODPropEditionID);
}

//------------------------------------------------------------------------------
// PeekNextGlobalID
//------------------------------------------------------------------------------

ODStatic ODULong PeekNextGlobalID(Environment *ev,
                ODDraft* draft,
                ODPropertyName globalProp)
{
        TempODStorageUnit       draftPropertiesSU = draft->AcquireDraftProperties(ev);
        ODULong                 id = 0;

        if ( ODSUExistsThenFocus(ev, draftPropertiesSU, globalProp, kODULong) )
                StorageUnitGetValue(draftPropertiesSU, ev, sizeof(ODULong), (ODValue) &id);

        return ++id;
}

//------------------------------------------------------------------------------
// ReserveGlobalID
//------------------------------------------------------------------------------

ODStatic ODULong ReserveGlobalID(Environment *ev,
                ODDraft* draft,
                ODPropertyName globalProp)
{
        TempODStorageUnit       draftPropertiesSU = draft->AcquireDraftProperties(ev);
        ODULong                 id = 0;

        ODSUForceFocus(ev, draftPropertiesSU, globalProp, kODULong);
        if ( draftPropertiesSU->GetSize(ev) > 0 )
                id = ODGetULongProp(ev, draftPropertiesSU, globalProp, kODULong);
        ++id;
        ODSetULongProp(ev, draftPropertiesSU, globalProp, kODULong, id);

        return id;
}

//------------------------------------------------------------------------------
// ODLinkManager::NewDraftInfo
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkManagerNewDraftInfo(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","NewDraftInfo");

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#else
        SOM_TRY
#endif

        // Create a new info structure for this draft
        DraftInfo* info = new DraftInfo;

        info->draft = draft;
        info->linksToExport = new ODRefCntCollection(ev);
        info->linksToDelete = new ODRefCntCollection(ev);
        info->newExportedLinks = new ODRefCntCollection(ev);
        info->linksImported = kODFalse;
        _fDraftInfos->AddLast(info);

#ifdef _PLATFORM_OS2_
        SOM_CATCH_ALL
        SOM_ENDTRY
                return;
#endif
}

//------------------------------------------------------------------------------
// ODLinkManager::GetDraftInfo
//------------------------------------------------------------------------------

SOM_Scope DraftInfo*  SOMLINK ODLinkManagerGetDraftInfo(ODLinkManager *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","GetDraftInfo");

        OrderedCollectionIterator* iter = kODNULL; ODVolatile(iter);

        DraftInfo* result = (DraftInfo*) kODNULL;

        if ( draft && _fDraftInfos )
        {
                TRY
                        iter = _fDraftInfos->CreateIterator();

                        for (DraftInfo* info = (DraftInfo*) iter->First();
                                        iter->IsNotComplete();
                                        info = (DraftInfo*) iter->Next() )
                        {
                                if ( info && draft->IsEqualTo(ev, info->draft) )
                                {
                                        result = info;
                                        break;
                                }
                        }
                CATCH_ALL
                ENDTRY

                ODDeleteObject(iter);
        }

        return result;
}

//------------------------------------------------------------------------------
// ODLinkManager::DeleteDraftInfos
//------------------------------------------------------------------------------

// SOM_Scope void  SOMLINK ODLinkManagerDeleteDraftInfos(ODLinkManager *somSelf, Environment *ev)
// {
//     ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
//     ODLinkManagerMethodDebug("ODLinkManager","DeleteDraftInfos");
//
//      SOM_CATCH return;
//
//      if (_fDraftInfos != (OrderedCollection*) kODNULL)
//      {
//              DraftInfo* info;
//              while ((info = (DraftInfo*) _fDraftInfos->First()) != (DraftInfo*) kODNULL)
//              {
//                      ODDeleteObject(info->linksToExport);
//                      ODDeleteObject(info->linksToDelete);
//                      ODDeleteObject(info->newExportedLinks);
//                      _fDraftInfos->RemoveFirst();
//                      delete info;
//              }
//              ODDeleteObject(_fDraftInfos);
//      }
// }

//------------------------------------------------------------------------------
// ODLinkManager::somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkManagersomUninit(ODLinkManager *somSelf)
{
    ODLinkManagerData *somThis = ODLinkManagerGetData(somSelf);
    ODLinkManagerMethodDebug("ODLinkManager","somUninit");

#ifdef _PLATFORM_MACINTOSH_
        AEEventHandlerUPP       toBeDisposedUPP;
        long                            toBeIgnoredRefCon;
        OSErr                           error;

        // somSelf->DeleteDraftInfos(ev);
        // It is incorrect to call methods on somSelf inside somUninit,
        // a subclass may have already been somUninited.  See OpenDoc Building Code for details.
        // Instead the implementation of DeleteDraftInfos has been copied here.

        // BEGIN copy of DeleteDraftInfos implementation
        if (_fDraftInfos != (OrderedCollection*) kODNULL )
        {
                Environment* ev = somGetGlobalEnvironment();
                DraftInfo* info;
                while ((info = (DraftInfo*) _fDraftInfos->First()) != (DraftInfo*) kODNULL)
                {
                        ODDeleteObject(info->linksToExport);
                        ODDeleteObject(info->linksToDelete);
                        ODDeleteObject(info->newExportedLinks);
                        _fDraftInfos->RemoveFirst();
                        delete info;
                }
                ODDeleteObject(_fDraftInfos);
        }
        // END copy of DeleteDraftInfos implementation

        if ( _fSuspendedUpdateLinks != (ODRefCntCollection*) kODNULL )
        {
                // _fSuspendedUpdateLinks->RemoveAndReleaseAll();
                ODDeleteObject(_fSuspendedUpdateLinks);
        }

        error = AEGetEventHandler(kODShellSignature, kODGetLinkEventID,
                        &toBeDisposedUPP, &toBeIgnoredRefCon, kNotSysHandler);
        if ( error == noErr )
        {
                DisposeRoutineDescriptor(toBeDisposedUPP);
                error = AERemoveEventHandler(kODShellSignature, kODGetLinkEventID,
                                        kODNULL,                // a UPP wouldn't work: it might not be ==
                                        kNotSysHandler);
        }

        error = AEGetEventHandler(kODShellSignature, kODRequestLinkEventID,
                        &toBeDisposedUPP, &toBeIgnoredRefCon, kNotSysHandler);
        if ( error == noErr )
        {
                DisposeRoutineDescriptor(toBeDisposedUPP);
                error = AERemoveEventHandler(kODShellSignature, kODRequestLinkEventID,
                                        kODNULL,                // a UPP wouldn't work: it might not be ==
                                        kNotSysHandler);
        }

        error = AEGetEventHandler(sectionEventMsgClass, sectionReadMsgID,
                        &toBeDisposedUPP, &toBeIgnoredRefCon, kNotSysHandler);
        if ( error == noErr )
        {
                DisposeRoutineDescriptor(toBeDisposedUPP);
                error = AERemoveEventHandler(sectionEventMsgClass, sectionReadMsgID,
                                        kODNULL,
                                        kNotSysHandler);
        }

        error = AEGetEventHandler(sectionEventMsgClass, sectionWriteMsgID,
                        &toBeDisposedUPP, &toBeIgnoredRefCon, kNotSysHandler);
        if ( error == noErr )
        {
                DisposeRoutineDescriptor(toBeDisposedUPP) ;
                error = AERemoveEventHandler(sectionEventMsgClass, sectionWriteMsgID,
                                        kODNULL,
                                        kNotSysHandler);
        }

        error = AEGetEventHandler(sectionEventMsgClass, sectionScrollMsgID,
                        &toBeDisposedUPP, &toBeIgnoredRefCon, kNotSysHandler);
        if ( error == noErr )
        {
                DisposeRoutineDescriptor(toBeDisposedUPP) ;
                error = AERemoveEventHandler(sectionEventMsgClass, sectionScrollMsgID,
                                        kODNULL,
                                        kNotSysHandler);
        }

        if ( _fNMResponseUPP != kODNULL )
                DisposeRoutineDescriptor(_fNMResponseUPP);

        if ( _fNMRecPtr )
                ODDisposePtr((ODPtr) _fNMRecPtr->nmStr);
        ODDisposePtr(_fNMRecPtr);

    parent_somUninit(somSelf);

#endif
}

//------------------------------------------------------------------------------
// RegisterSubscribingLinks
//------------------------------------------------------------------------------

ODStatic void RegisterSubscribingLinks(Environment *ev, ODDraft* draft)
{
        // Internalize all link source objects; each link subscribing to an edition
        // file will register with the edition manager.

        ODLinkSourceIterator* iter = kODNULL;

#ifdef _PLATFORM_MACINTOSH_
        FN_CATCH
        {
                ODDeleteObject(iter);
                return;
        }
#else
        TRY
#endif


        iter = draft->CreateLinkSourceIterator(ev);

        for (ODLinkSource* linkSource = iter->First(ev);
                        iter->IsNotComplete(ev);
                        linkSource = iter->Next(ev) )
        {
                linkSource->Release(ev);
        }

        delete iter;
#ifdef _PLATFORM_OS2_
        CATCH_ALL
        {
                ODDeleteObject(iter);
                return;
        }
        ENDTRY
#endif
}

//------------------------------------------------------------------------------
// RegisterPublishingLinks
//------------------------------------------------------------------------------
ODStatic void RegisterPublishingLinks(Environment *ev, ODDraft* draft)
{
        // Internalize all link objects; each link publishing an edition file will
        // register with the edition manager.

        ODLinkIterator* iter = kODNULL;

#ifdef _PLATFORM_MACINTOSH_
        FN_CATCH
        {
                ODDeleteObject(iter);
                return;
        }

#else
        TRY
#endif

        iter = draft->CreateLinkIterator(ev);

        for (ODLink* link = iter->First(ev);
                        iter->IsNotComplete(ev);
                        link = iter->Next(ev) )
        {
                link->Release(ev);
        }

        delete iter;
#ifdef _PLATFORM_OS2_
        CATCH_ALL
        {
                ODDeleteObject(iter);
                return;
        }
        ENDTRY
#endif
}

//------------------------------------------------------------------------------
// UpdateEditionFiles
//------------------------------------------------------------------------------

ODStatic void UpdateEditionFiles(Environment *ev, ODRefCntCollection* collection)
{
#ifdef _PLATFORM_OS2_ // CED
  _interrupt(3);
#else
        if ( collection != kODNULL )
        {
                ODRefCntCollectionIterator iter(collection);
                ODLinkKey key;

                for (ODLink* link = (ODLink*) iter.First();
                                iter.IsNotComplete();
                                link = (ODLink*) iter.Next() )
                {
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
}

//------------------------------------------------------------------------------
// DeleteEditionFiles
//------------------------------------------------------------------------------

ODStatic void DeleteEditionFiles(Environment *ev, ODRefCntCollection* collection)
{
#ifdef _PLATFORM_OS2_ // CED
  _interrupt(3);
#else
        if ( collection != kODNULL )
        {
                ODRefCntCollectionIterator iter(collection);
                ODLinkKey key;

                for (ODLink* link = (ODLink*) iter.First();
                                iter.IsNotComplete();
                                link = (ODLink*) iter.Next() )
                {
                        if ( link->Lock(ev,0,&key) )
                        {
                                TRY
                                        link->DeleteLinkEdition(ev, key);
                                CATCH_ALL
                                ENDTRY
                                link->Unlock(ev, key);
                        }
                }
        }
#endif
}

//------------------------------------------------------------------------------
// RemoveAllLinksInCollection
//------------------------------------------------------------------------------

// ODStatic void RemoveAllLinksInCollection(Environment *ev, ODRefCntCollection* collection)
//      // collection must contain only ODLink subclass instances
// {
//      if ( collection != kODNULL )
//      {
//              ODRefCntCollectionIterator iter(collection);
//
//              TRY
//                      for (ODLink* link = (ODLink*) iter->First();
//                                      iter->IsNotComplete();
//                                      link = (ODLink*) iter->Next() )
//                      {
//                              ODReleaseObject(ev, link);
//                      }
//              CATCH_ALL
//              ENDTRY
//
//              collection->RemoveAll();
//      }
// }

//------------------------------------------------------------------------------
// AppendULongToDraftProperty
//------------------------------------------------------------------------------

ODStatic void AppendULongToDraftProperty(Environment *ev,
        ODDraft* draft, ODPropertyName prop, ODULong value)
{
        TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);
        ODSUForceFocus(ev, draftProperties, prop, kODULongSequence);
        draftProperties->SetOffset(ev, draftProperties->GetSize(ev));
        ODSetULongProp(ev, draftProperties, kODNULL, kODNULL, value);

        draft->SetChangedFromPrev(ev);
}

//------------------------------------------------------------------------------
// SectionIDReserved
//------------------------------------------------------------------------------

ODStatic ODBoolean SectionIDReserved(Environment *ev,
        ODDraft* draft, ODULong requestedID)
{
        ODBoolean result = kODFalse;

        TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);

        if ( ODSUExistsThenFocus(ev, draftProperties, kODPropReservedSectionIDs, kODULongSequence) )
        {
                ODULong size = draftProperties->GetSize(ev);
                while ( draftProperties->GetOffset(ev) < size )
                {
                        if ( ODGetULongProp(ev, draftProperties, kODNULL, kODNULL) == requestedID )
                        {
                                result = kODTrue;
                                break;
                        }
                }
        }

        return result;
}
