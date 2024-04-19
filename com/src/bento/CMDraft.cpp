/* @(#)Z 1.41 com/src/bento/CMDraft.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:14 (96/10/29 11:36:07) */
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
        File:           CMDraft.cpp

        Contains:       Implementation for CMDraft class.

        Owned by:       Vincent Lo

        Copyright:      © 1993-1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

           <125>        10/24/95        TJ              Changed string to corbastring
           <124>        10/24/95        jpa             1293441: DM/VL: Bento memory reserve &
                                                                        fatal container err & don't throw in
                                                                        Release.
           <123>        10/20/95        VL              1293256: Changed
                                                                        kODErrBentoInvalidVersionList to
                                                                        kODErrDraftDoesNotExist.
           <122>        10/19/95        EL              1292685: After purging we may get a
                                                                        different storage unit for the same
                                                                        CMObject.
           <121>        10/17/95        jpa             1289153: CreatePart, AcquirePart,
                                                                        AcquireFrame handle failure gracefully.
           <120>        10/13/95        EL              1287340: Use standard ISO prefix
           <119>         10/8/95        TJ              Fixes Recomended by Refball
           <118>         10/5/95        CC              1286180: Added CloneCompanionObject method.
                                                                        1286180: Clone: Call CloneCompanionObject
                                                                        if clone kind is kODCloneAll.
           <117>         10/3/95        TJ              Changes done by RefBall Team
           <116>         9/29/95        TJ              Made Changes for MAC SOM
           <115>         9/14/95        VL              1283566: Fixed Clone so that CloneFromLink
                                                                        would also skip links (i.e., return 0 as
                                                                        the id).
           <114>         9/11/95        VL              1283326: CreateFrame now takes ODObjectType
                                                                        instead of ODType. 1283157: Fixed memory
                                                                        Leak in CMDraft::ConstructRealPart.
           <113>          9/8/95        VL              1276955: Make sure draft is not read-only
                                                                        before writing out preferred kind.
           <112>          9/8/95        T‚              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
           <111>          9/1/95        CC              1273646, 1279220:  Support kODCloneToFile.
           <110>         8/29/95        VL              1279830, 1279074: THROW_IF_NULL after new
                                                                        on SOM objects. 1279505: Catches exceptions
                                                                        returned from ReleaseAll.
           <109>         8/26/95        T‚              1270499Two editors which support same kind
                                                                        binding problem
           <108>         8/25/95        JBS             1263078 FB: fix part editor swapping
           <107>         8/24/95        CC              1251959: BeginClone must clean up before
                                                                        returning an error.
           <106>         8/24/95        EL              1274602: cmObject not released resulting in
                                                                        wrong internal Bento ref count.
           <105>         8/22/95        VL              1277720, 1278310: Cleaned up error code in
                                                                        Release/RemoveStorageUnit.
           <104>         8/21/95        VL              1278330, 1278315: Error code cleanup.
           <103>         8/21/95        VL              1277291: Use GetOriginalCloneKind from
                                                                        StorUtil. 1258660: Rewrote exception
                                                                        handling for CreateFrame to ensure that the
                                                                        correct exception are propogated.
           <102>         8/17/95        CC              1277781: CheckPartAction: Check size of
                                                                        storage unit type before comparing strings;
                                                                        check for invalid part ID returned by
                                                                        ODGetStrongSURefProp.
           <101>         8/16/95        EL              Disable ODDebug_CloningAnnotations
           <100>         8/16/95        NP              1274946: ErrorDef.idl problems. Add include
                                                                        file. 1277286: CMDraft: ConstructRealPart
                                                                        gropes ev->_major when it doesn't mean to.
                <99>     8/16/95        VL              1266007: IsValidDraftKey returns kODFalse
                                                                        for kODNULLKey.
                <98>     8/15/95        CC              1274794, 1273863: CopyProperties: Focus to
                                                                        property (don't assume already focused).
                                                                        CopyDraftAnnotations: Always copy the
                                                                        storage unit type property.
                <97>     8/12/95        T‚              1276812 Need to use TempObjs and TempRefs
                                                                        for exception safety and to avoid TRY
                                                                        blocks, 1276807 Opt./Bug: use StdTypIO
                                                                        routines for portable streaming & smaller
                                                                        footprint, 1276806 Optimization: use
                                                                        kODFalse instead of kODTrue in comparisons
                <96>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <95>     7/26/95        VL              1270320: Dispose of valueName in
                                                                        CopyProperty.
                <94>     7/21/95        VL              1270320: Freeing internal field. Disposed
                                                                        ba._buffer of ID.
                <93>     7/20/95        CC              1268790: Clone: Make sure original ID is
                                                                        still valid before substituting!
                <92>      7/3/95        VL              1255736: Every instantiated persistent
                                                                        object has the object type correctly set.
                <91>     6/29/95        VL              1242642: Release DraftProperties in
                                                                        DeleteCollections. Release SU before
                                                                        removing it.
                <90>     6/23/95        JBS             1261323 Draft::CreateFrame: add isSubframe
                                                                        param, remove isRoot
                <89>     6/19/95        VL              1170098: Added code to count part instances
                                                                        (PartInstantiated, PartDeleted,
                                                                        PurgeClasses). Code for the actual
                                                                        unloading of the libraries is commented out
                                                                        now. Added DeleteRealPart.
                <88>     6/16/95        VL              1244940: Made Externalize which triggers
                                                                        promise resolution work.
                <87>     6/14/95        VL              1251227: Delete the collections and their
                                                                        objects in somUninit.
                <86>     6/13/95        VL              1241352: Moved PreserveFocus to DrafPriv.
                <85>     6/12/95        CC              1252059: BeginClone: Allow clone to link
                                                                        when clone from link is in progress.
                <84>     6/10/95        VL              1255300: Updated CopyDraftAnnotations for
                                                                        cloning annotations and metadata.
                <83>      6/7/95        CC              1255762: EndClone() now uses destFrame
                                                                        passed to BeginClone().
                                                                        CheckClonedObject, ContainingPartInClone,
                                                                        CheckPartAction: Changed kODFrame to
                                                                        kODFrameObject.
                                                                        CheckClonedObject: Changed kODPart to
                                                                        kODPartObject.
                                                                        ContainingPartInClone: Fixed refcount
                                                                        problem.
                <82>      6/2/95        CC              1255476: CreateVersion, et. al. : release
                                                                        document acquired for debug string.
                                                                        1255476: ContainingPartInClone: Release
                                                                        acquired parts and frames.
                                                                        1255474: AcquireDocument restored to
                                                                        GetDocument.
                                                                        1255474: GetPersistentObject restored to
                                                                        AcquirePersistentObject.
                                                                        1255474: AcquireStorageUnitType restored to
                                                                        GetStorageUnitType.
                <81>      6/1/95        jpa             Handle errs thrown from ODNewObject.
                                                                        [1242839]
                <80>     5/26/95        VL              1251403: Multithreading naming support.
                <79>     5/25/95        jpa             Fixed usage of ODDebug. [1253321]
                <78>     5/22/95        CC              1242879: Added IsValidID.
                <77>     5/22/95        VL              1246940: Preserve focus during Clone.
                <76>     5/19/95        VL              1250574: Added check for kODNULLID in Get
                                                                        methods.
                <75>     5/18/95        CC              1238898: Add destFrame argument to
                                                                        BeginClone call; added fDestFrame field.
                <74>     5/17/95        VL              1170098: Added ConstructRealPart and
                                                                        ReleaseRealPart.
                <73>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
                                                                        refcount
                <72>      5/3/95        CC              Split file into two segments to avoid
                                                                        16-bit PC-relative offset problems on 68K.
                <71>      5/2/95        CC              1207493: Detect embedding part within
                                                                        itself.
                                                                        New methods: ContainingPartInClone and
                                                                        CheckClonedObject.
                                                                        New fields: fAnyFrameCloned and
                                                                        fRootPartCloned.
                                                                        New static variables: sIntermediateDraft,
                                                                        sSUTypeBuffer, and sRootPartIDToIgnore.
                                                                        New static functions: CheckPartAction,
                                                                        CopyDraftAnnotations, CopyProperty,
                                                                        GetStorageUnitType, RootPartID.
                                                                        1245718: StrongClone: Call
                                                                        CopyDraftAnnotations.
                                                                        1245706: CreatePart creates
                                                                        kODPropPreferredKind property, not
                                                                        kODPropPart.
                <70>      5/2/95        VL              1211972: RemoveFrame does not dirty draft
                                                                        if frame is non-persistent. 1243122: Use
                                                                        kODErrValueIndexOutOfRange. 1221370:
                                                                        AcquirePersistentObject works on read-only
                                                                        draft.
                <69>      5/2/95        EL              1234685: Container manager does not like
                                                                        value created but not written to, so always
                                                                        write something after CMNewValue.
                <68>     4/25/95        DM              1172791: Replace AEHashTable with
                                                                        OpenHashTable
                <67>     4/25/95        VL              1210982: Removed 5$.
                <66>     4/25/95        CC              1242555: CreateLinkSource, CreateLink,
                                                                        Clone, AbortClone: Removed $5 from comment.
                <65>     4/10/95        VL              1230350: Release Part before trying to
                                                                        remove it.
                <64>      4/7/95        EL              1226127: make sure code works when
                                                                        TestFlushContainer is false.
                <63>     3/31/95        T‚              1223505 BB: Opening and creating documents
                                                                        on read only media.  Fixed Creation of
                                                                        Non-persistent frames on readonly drafts.
                <62>     3/24/95        EL              1209355: Abort would remove changes in
                                                                        version list rather than reinitialize it.
                                                                        Cut down on version list externalization.
                <61>     3/23/95        VL              1228003: Added debug code for versionlist.
                                                                        1230371: Check before calling
                                                                        IDList::Remove.
                <60>     3/17/95        CC              1194656: SetOriginalDraft must check for
                                                                        existence of property and value before
                                                                        adding.
                <59>     3/13/95        VL              1226094: Added error handling in CreateSU
                                                                        to handle the case when any invalid ID is
                                                                        passed in.
                <58>     3/10/95        VL              1222416: Write out 0 at the end of
                                                                        persistent object ISOString.
                <57>      3/9/95        VL              1225504: Use GetTargetDocument to prevent
                                                                        ref-counting problem.
                <56>      3/7/95        CC              1224121: ODPart::CreateLink()  and
                                                                        ODLinkManager::CreateLink() return an
                                                                        ODLinkSource object.
                <55>      3/6/95        EL              1182275: Replace SetDraftPropertyObject by
                                                                        CMKeepObject.
                <54>     2/28/95        VL              1194656: Check for existence before adding
                                                                        property or value.
                <53>     2/21/95        EL              1182275: Do not externalize object if it
                                                                        was garbage collected. Tell embedded
                                                                        container about the draft property object.
                <52>     2/21/95        eeh             1222416: add 1 to len in
                                                                        GetPersistentObjectID
                <51>     2/17/95        VL              1220231: CreateFrame now calls
                                                                        InitFrameNonPersistent.
                <50>     2/15/95        EL              1182275: Set embedded container to no merge
                                                                        if draft is created. 1158620: Lazy open
                                                                        would avoid reopening of embedded container
                                                                        when document is closed.
                <49>     2/10/95        VL              1205627: Use TargetContainer in
                                                                        CreateVersion.
                <48>      2/6/95        TJ              Sorry, the last comment should be touched
                                                                        copy right date.
                <47>      2/6/95        TJ              Updated Check in Date.
                <46>      2/3/95        VL              1216535: Implemented storage support for
                                                                        non-persistent frames.
                <45>     1/18/95        VL              1193559: Added new 2
                                                                        methods(AcquirePersistentObject and
                                                                        GetPersistentObjectID). Renamed
                                                                        AcquirePersistentObject to
                                                                        RetrievePersistentObject. Removed
                                                                        GetPersistentLinkID and AcquireLinkID.
                <44>     1/11/95        VL              1185688: Reverted one of the changes in
                                                                        ExternalizeCollections to fix a refcounting
                                                                        problem.
                <43>     1/11/95        VL              1185688: Made storage more robust in terms
                                                                        of error handling. Also did some code
                                                                        review cleanup.
                <42>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                <41>    12/16/94        CC              1203516 Modified CreateLinkSource to use
                                                                        SetLink() and SetLinkSource() methods,
                                                                        modified CreateLink().
                <40>    12/15/94        CC              1193569 CreateLinkSpec() takes ODByteArray
                                                                        parameter.
                                                                        1197221 AcquireLink() - added SOM_CATCH;
                                                                        changed call to ODPart::CreateLink() to
                                                                        pass byte array.
                <39>     11/1/94        CC              1190911, 1192030, 1196908 - Clone reuses
                                                                        same objects if moved within same draft;
                                                                        Clone while fulfilling promises won't
                                                                        recopy already cloned objects.
                <38>     11/1/94        VL              1151339: Preparation for MergeContainer.
                <37>    10/19/94        VL              1155857: Added ODDebug_Drafts to debug
                                                                        "Cannot create more than 10 drafts"
                                                                        problem.
                <36>    10/12/94        VL              (JPA) Added debugstrs for frames.
                <35>     10/4/94        CC              1190858 - Added ReleaseLinkSource().
                <34>     9/29/94        RA              1189812: Mods for 68K build.
                <33>     9/23/94        VL              1184272: ContainerID is now a sequence of
                                                                        octets. This affects EmbeddedContainers
                                                                        also.
                <32>     9/23/94        CC              1187509 - Added somPrintfs for debugging
                                                                        clone operations.
                <31>     9/16/94        CC              1186957 - Fix to clone newly created links
                                                                        correctly (added AcquireLinkSourceFromLink and
                                                                        AcquireLinkFromLinkSource).
                <30>     9/15/94        CC              1186776 - Enforce correct link behavior
                                                                        during clone operations.
                                                                        1160117 - Strip links when cloning into a
                                                                        link.
                                                                        1186657 - Correct ref count on object
                                                                        returned by CreateLinkSource().
                                                                        1153940 - Clone must support new clone kind
                                                                        constants.
                <29>      9/9/94        CG              #1183531: ConstructPart instantiates a
                                                                        PartWrapper which in turn creates a part.
                <28>      9/6/94        VL              1184177: Removed GetName and SetName.
                <27>      9/5/94        VL              1184871: Used Renew to remove dependency on
                                                                        default heap.
                <26>     8/31/94        T‚              #1183129, #1183116, #1183119, #1183111:
                                                                        Lots of ErrorCode cleanup.
                <25>     8/31/94        VL              1106013: Fixed GetName to return kODNULL if
                                                                        the draft does not have a name.
                <24>     8/26/94        VL              1183174: Added destDraft to BeginClone and
                                                                        added toID to Clone.
                <23>     8/19/94        T‚              #1181622 Fix Written to Owned & name &
                                                                        reference to ASLM Build
                <22>     8/17/94        CG              #1181487: Instantiating parts by name
                                                                        again!
                <21>     8/16/94        T‚              #1180922  Remove more obsolete types from
                                                                        StdTypes.idl.  Localized kODStorageUnit to
                                                                        this file.
                <20>     8/16/94        VL              1143605: Added CloneInto.
                <19>     8/15/94        VL              #???: Added frameType parameter to
                                                                        CreateFrame.
                <18>     8/13/94        T‚              #1180814 Added support for ODPartWrapper
                                                                        API
                <17>     8/12/94        JBS             1179919: add biasCanvas param to
                                                                        Draft::CreateFrame()
                <16>     8/11/94        CC              Added implementation of
                                                                        CreateLinkIterator() and
                                                                        CreateLinkSourceIterator() methods.
                <15>     8/10/94        JBS             1179919: coordinate system bias changes
                <14>      8/4/94        eeh             bug 1179054: fix kODPartPartsBin
                <13>      8/3/94        VL              1153123: Storage to ODStor.
                <12>     7/28/94        T‚              Create Clock statically
                <11>     7/27/94        CC              Removed static functions IsMyProcess() and
                                                                        IsRunningProcess(); changed AcquireLink()
                                                                        method to use new ODLinkSpec methods.
                <10>     7/26/94        VL              Got rid of MacStorageUnit and
                                                                        MacDragAndDropStorageUnit.
                 <9>     7/26/94        eeh             
                 <8>     7/21/94        VL              Removed ASLM BUILD.
                 <7>     7/19/94        CC              Create DragText statically.
                 <6>     7/18/94        T‚              moved refcounting fixes to PstObj.cpp
                 <5>     7/18/94        T‚              fixed another refcount problem
                 <4>     7/17/94        T‚              fixed ReleasePersistentObject to release
                                                                        the storageUnit of a persistentObject
                 <3>     7/14/94        eeh             Apple module name -> AppleTestDraw
                 <2>     7/12/94        VL              Added code to Create DrawPart statically.
                 <1>      7/5/94        VL              first checked in
                -----------------------------------------------------------------------
                <10>      6/1/94        CG              Changed to use public binding object.
                 <9>     5/27/94        jpa             Use ODNewObject to avoid ASLM dependency,
                                                                        to support new exception scheme [1165267]
                 <8>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <7>      4/6/94        JBS             1155480
                 <6>     3/28/94        T‚              #1145834.  Remove PrtType.h & .cpp from the
                                                                        project.  Noted a few places where there is
                                                                        code which is only needed by the static
                                                                        build.
                 <5>     3/28/94        CG              1153547: Changed XMPSessn.h to XMPSessM.h
                 <4>     3/27/94        T‚              localized use of kXMPPropRootSU
                 <3>     3/25/94        CC              Change name of included file to
                                                                        "LinkMgrM.h". (1153348)
                 <2>     3/25/94        MB              Symantec ASLM fixes. #1150864
                 <1>     3/24/94        VL              first checked in
                 <7>     3/18/94        CC              CMRegisterType(): Changed kXMPAppleTEXT
                                                                        argument to kXMPISOStr. (1151636)
                 <6>     3/15/94        MB              Changes to support SCpp/ASLM builds,
                                                                        #1150864.
                 <5>     3/14/94        CC              Added internal methods FixReusedLinks() and
                                                                        ReferencedStorageUnitCloned(); Revised
                                                                        EndClone(), PrepareLinksForClone().
                                                                        (1149492)
                 <4>     2/22/94        VL              THROW -> THROW_IF_ERROR.
                 <3>     2/18/94        VL              #1142972, #1144024: Added
                                                                        GetDragDropStorageUnit to enable importing
                                                                        data from a foreign source (e.g. the
                                                                        Finder).
                 <2>     2/17/94        CC              Bug #1142584 - GetOriginalCloneKind returns
                                                                        kXMPCloneCopy by default in case content
                                                                        was put directly onto the clipboard without
                                                                        cloning.
           <106>          2/8/94        VL              Corrected capitalization of include files
                                                                        and used <> for Toolbox headers.
           <105>          2/8/94        VL              Use new exception macros.
           <104>          2/4/94        VL              Session.h -> XMPSessn.h.
           <103>          2/4/94        VL              More code cleanup.
           <102>          2/4/94        VL              Moved to PPC Header and began code cleanup.
                                                                        (Added itoa to replace NumToString. Got rid
                                                                        of some warnings.)
           <101>          2/3/94        CG              System Session break out.
           <100>          2/2/94        VL              Made Hash Table use system heap when
                                                                        appropriate.
                <99>      2/2/94        CC              Revised PrepareLinksForClone() and
                                                                        AnnotateLinksForNextClone().
                <98>      2/2/94        RR              Added isRoot to CreateFrame
                <97>      2/1/94        CC              Added PrepareLinksForClone()
                                                                        AnnotateLinksForNextClone(), and non-member
                                                                        support functions; not yet called by
                                                                        EndClone().  Added XMPCloneType argument to
                                                                        BeginClone.
                <96>     1/28/94        CC              Removed UniqueUpdateID(),
                                                                        ShowLinkBorders(), and SetLinkBorders().
                                                                        Implemented Purge() (VL).  Abort() now
                                                                        releases draft properties (VL).  Fixed
                                                                        sequence of deletions in
                                                                        RemovePersistentObject() (VL).
                                                                        DeleteCollections() - call ReleaseAll() on
                                                                        all objects, then delete all objects.
                <95>     1/27/94        VL              Used ReleaseAll. Fixed up
                                                                        ReleasePersistentObject. Added
                                                                        IsValidDraftKey.
                <94>     1/25/94        CC              Use fShowLinkBorders field.
                <93>     1/22/94        CC              AcquireLink(), AcquireLinkSource(): Avoid infinite
                                                                        recursion by adding persistent object to
                                                                        the draft before calling
                                                                        InitFromStorageUnit().
                <92>     1/21/94        CC              Added CreateLinkSource(), AcquireLinkSource(),
                                                                        RemoveLinkSource(), and
                                                                        AcquireLinkSourceIterator().  Moved calls to
                                                                        XMPLinkManager::DraftOpened,
                                                                        ::DraftClosing, and ::DraftSaved into
                                                                        document shell.
                <91>     1/21/94        CG              Renamed kXMPEmbeddedContainer to
                                                                        kXMPBentoEmbeddedContainer.  Changed calls
                                                                        to NewObject to take current heap.
                <90>     1/18/94        CG              Added include for BentoDef.h for
                                                                        kXMPEmbeddedContainer.
                <89>     1/18/94        CC              Added stub implementations of
                                                                        ShowLinkBorders() and SetLinkBorders();
                                                                        changed DraftClosed() to DraftClosing().
                <88>     1/15/94        VL              Made sure that the Part Destructor is
                                                                        called.
                <87>     1/14/94        CC              Externalize(): Call
                                                                        XMPLinkManager::DraftSaved();
                                                                        RemoveChanges(): Call
                                                                        XMPLinkManager::DraftClosed().
                <86>     1/12/94        CG              Changed CreatePart and AcquirePart to use the
                                                                        new Binding object in ASLM build.
                <85>     1/12/94        VL              Init... Changes.
                <84>    12/22/93        CC              AcquireLink() - Better error handling and
                                                                        clean-up.
                <83>    12/21/93        VL              Added support for Persistent Link ID.
                <82>    12/21/93        JBS             CreateFrame with viewType & presentation
                <81>    12/20/93        SS              Added PartsBin to Get/CreatePart
                <80>    12/20/93        VL              Removed window-related stuff (XMPWindow is
                                                                        no longer a persistent object).
                <79>    12/16/93        CC              AcquireLink: Instantiate link objects, moved
                                                                        code into LinkMgr.cp.  CreateLink: object
                                                                        type is kXMPLink not kXMPPart!
                <78>    12/16/93        T‚              CreatePart now takes an optional Editor
                                                                        parameter
                <77>    12/14/93        VL              Added private methods to GetCMContainer and
                                                                        used AcquireDraft instead of fDraft.
                <76>    12/14/93        T‚              InitFrame, InitLink, InitWindow changes.
                                                                        PstObj::Initialize, PstObj:Internalize,
                                                                        PstObj::InitializePersistent all went away.
                <75>    12/13/93        T‚              InitPart changes
                <74>     12/8/93        CC              Changes to CreateLinkSpec
                                                                        AcquireLink sends apple event to resolve
                                                                        linkSpec.
                <73>     12/3/93        T‚              Stop including XMPError.h, it is included
                                                                        as ErrorDef.h inside Except.h
                <72>    11/30/93        VL              Changed frameGroup from XMPUShort to
                                                                        XMPULong.
                <71>    11/24/93        VL              Added document check in Initialize.
                <70>    11/23/93        VL              Made XMPDraft pool-aware.
                <69>    11/18/93        PH              Call parent method in
                                                                        RemovePersistentObject
                <68>    11/18/93        PH              Fixed RemovePersistentObject to call
                                                                        Release before calling delete
                <67>    11/17/93        CC              Fixed CreateLink to compile with CFront.
                <66>    11/16/93        EL              Use OpenDoc instead of Amber.
                <65>    11/16/93        SS              Reactivated DragPart
                <64>    11/16/93        EL              Part name is ISOStr and should be null
                                                                        terminated.
                <63>    11/15/93        CC              Implemented AcquireLink for LinkSpec parameter
                                                                        Implemented CreateLink and CreateLinkSpec
                                                                        Added IsMyProcess
                <62>    11/12/93        JBS             remove Layout methods
                <61>     11/9/93        PH              Added CreateDragDropStorageUnit
                <60>     11/4/93        SS              Added #ifdef QUALITY_PARTS too
                                                                        Get/CreatePart for quality team
                <59>     11/1/93        VL              Added BeginClone, EndClone and AbortClone.
                                                                        Collapsed fStorageUnits and
                                                                        fReleasedStorageUnits into one hash table.
                                                                        Collapsed fPersistentObjects and
                                                                        fReleasedPersistentObjects into one hash
                                                                        table.
                <58>    10/29/93        PH              Fix AcquirePart and add check to
                                                                        ReleaseStorageU
                <57>    10/21/93        RR              CreateWindow takes new window flags (at
                                                                        least until we have new Initialize strategy
                                                                        for SOM)
                <56>    10/12/93        VL              Uncommented WASSERTs. Release
                                                                        fDraftProperties in ::RemoveChanges.
                <55>     10/8/93        VL              Temporarily removed WASSERT.
                <54>     10/7/93        RR              Added rootPart to CreateWindow call
                <53>     10/7/93        VL              Moved error codes to XMPError.h.
                <52>     10/6/93        T‚              remove obsolete parts (StylPart)
                <51>     9/30/93        VL              Removed SetFinished.
                <50>     9/30/93        VL              Removed unnecessary STORAGE_TEST ifdefs.
                <49>     9/29/93        VL              Added code for smart internalization.
                <48>     9/24/93        PH              Use new AppleTEXT
                <47>     9/24/93        VL              Removed unnecessary Throws.
                <46>     9/24/93        JA              Minor syntactic tweaks for THINK C++.
                <45>     9/23/93        VL              Added code to handle StorageUnitType
                                                                        property. Fixed SetName to use Exists
                                                                        instead of exception. Moved methods around
                                                                        to conform to .h file.
                <44>     9/22/93        JA              Updated ASSERTs for new macro.
                <43>     9/21/93        VL              Avoid creating unnecessary versions.
                <42>     9/16/93        JA              Fixed polarity of ASSERTs.
                <40>     9/14/93        VL              Made Draft thread-safe.
                <39>      9/9/93        VL              Set up the Session global for
                                                                        EmbeddedContainers. Fixed casting problem
                                                                        caused in <38>.
                <38>      9/9/93        VL              Fixed RemovePersistentObject to
                                                                        CMDeleteObject the real object, not ID.
                <37>      9/8/93        VL              Draft can handle flushing now.
                <36>      9/7/93        PH              Add code to create a DragPart
                <35>      9/3/93        JBS             added support for XMPLayout class
                <34>     8/26/93        VL              Used new VersionList calls.
                <33>     8/23/93        VL              Added FailIfNotExclusiveWrite check. Used
                                                                        fExternalized flag to show the state of the
                                                                        draft. Implemented NeedExternalizing.
                <32>     8/20/93        T‚              Commented out #ifdef PROTOTYPEBUILD stuff
                                                                        so that the implementation shell could use
                                                                        the full functionality of Draft
                <31>     8/20/93        VL              Cast XMPPtr to XMPName.
                <30>     8/20/93        VL              Used ODMemory.h
                <29>     8/19/93        VL              Put in CMDeleteObject again.
                <28>     8/18/93        VL              ReleaseStorageUnit takes an ID now.
                <27>     8/18/93        PH              Don't call CMDeleteObject in
                                                                        RemoveStorageUnit
                <26>     8/18/93        VL              Fixed ReleaseSTorageUnit.
                <25>     8/17/93        PH              Call internalize in AcquirePart and don't
                                                                        increment the ref count of the SU in
                                                                        GetPstObj if the object exists
                <24>     8/13/93        VL              Used VersionList::GetLatestVersionID.
                                                                        Commented out some code in Externalize.
                <23>     8/12/93        VL              Made GetName raise an exception instead of
                                                                        returning kXMPNULL. Used kXMP props and
                                                                        types.
                <22>     8/11/93        VL              ifdef out implementation code for
                                                                        CreateFrame because the XMPFrame class is
                                                                        not there yet.
                <21>     8/10/93        VL              Put some real code in CreateFrame.
                <20>     8/10/93        PH              Call release on su in ReleasePstObj
                <19>     8/10/93        PH              Really fix ReleasePstObj
                <18>      8/9/93        PH              Fix ReleasePersistentObject to do ref
                                                                        counting properly
                <17>      8/9/93        VL              Moved error code to XMPError.h.
                <16>      8/6/93        PH              Change kPartType to kXMPPart
                <15>      8/6/93        VL              Used CreateCollections and
                                                                        DeleteCollections.
                <14>      8/6/93        PH              Storage integration
                <13>      8/6/93        VL              Added support for
                                                                        creating/getting/releasing/removing
                                                                        persistent objects.
                <12>     7/29/93        VL              Removed getting persistent objects by name.
                                                                        Implemented GetName and SetName.
                <11>     7/23/93        VL              Removed root SU creation code in
                                                                        AcquireDraftPropertiesObject.
                <10>     7/14/93        VL              Implemented basic storage unit functions.
                 <9>      7/6/93        VL              Implemented RemoveFromDocument.
                 <8>      7/6/93        VL              XMPStorageUnitName should be
                                                                        XMPStorageUnitName* in AcquireStorageUnit.
                 <7>      7/2/93        T‚              CreateSU should take XMPType, not XMPName.
                 <6>     6/30/93        VL              Changed XMPName to XMPType.
                 <5>     6/30/93        VL              Added Reinitialize, Open, Close, Abort.
                                                                        Also added utility calls
                                                                        Open/Close/AbortVersion.
                 <4>     6/22/93        VL              Used RefCount from RefCtObj. Added
                                                                        SetChangedFromPrev() and
                                                                        AcquireDraftPropertiesObject. Initialize() gets
                                                                        SU for Draft Properties.
                 <3>     6/15/93        VL              Put it some test code.
                 <1>      6/2/93        VL              first checked in

        To Do:
        In Progress:

*/

#define CMDraft_Class_Source
#define VARIABLE_MACROS
#include <CMDraft.xih>

#ifdef _PLATFORM_WIN32_
#include <process.h>
#endif

#ifdef _PLATFORM_OS2_

// headers for DSOM linking

#include <process.h>
#include <somd.xh>

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#include <tpllitr.xh>
#include <tpll.xh>

#ifndef _IODDEFS_
#include <IODDefs.xh>
#endif

// #include <link.xh>
#include <ICIDItr.xh>
#include <ILinkSrc.xh>
#include <ILnkSrvc.xh>
#include <ILnkTrgt.xh>

#endif  // PLATFORM_OS2_

#ifndef _DRAFPRIV_
#include "DrafPriv.h"
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_CMDocument_xh
#include <CMDoc.xh>
#endif

#ifndef SOM_CMStorageUnit_xh
#include <CMSU.xh>
#endif

#ifndef _DOCPRIV_
#include "DocPriv.h"
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODStorageUnitCursor_xh
#include <SUCursor.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
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

#ifndef SOM_ODLinkManager_xh
#include <LinkMgr.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_ODBentoContainer_xh
#include "CMCtr.xh"
#endif

#ifndef _CMAPI_
#include "CMAPI.h"
#endif

#ifndef __CM_API_TYPE_
#include "CMAPITyp.h"
#endif

#ifndef SOM_ODEmbeddedContainer_xh
#include <EmbedCtr.xh>
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_CMLinkIterator_xh
#include <CMLkItr.xh>
#endif

#ifndef SOM_CMLinkSourceIterator_xh
#include <CMLkSItr.xh>
#endif

#ifdef _PLATFORM_OS2_
#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif
#endif

#ifndef _INDHDR_
#include "IndHdr.h"                     // for some const ODName
#endif

#ifndef _OPENHASH_
#include "OpenHash.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef __STRING__
#include <string.h>                     // For strlen, strcpy....
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"    // Adkins -- added
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef SOM_ODBinding_xh
#include <ODBindng.xh>
#endif

#ifndef SOM_ODPartWrapper_xh
#include <PartWrap.xh>
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifdef _PLATFORM_OS2_
#ifndef _ODUTILS_
#include <ODUtils.h>
#endif
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>
#endif
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef _NO_TMP_OBJS_
#ifndef _ODNEWOBJ_
#include <ODNewObj.h>
#endif
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _NO_TMP_OBJS_
#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef SOM_SOMClassMgr_xh
#include <somcm.xh>
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_NoPart_xh
#include <NoPart.xh>
#endif

#ifdef _PLATFORM_WIN32_
#ifndef _OLEWRAP_H_
#include "olewrap.h"
#endif

#ifndef _OLEMGRW_H_
#include "olemgrw.h"
#endif

#ifndef SOM_OpenDocShell_xh
#include "odshell.xh"
#endif

#ifndef SOM_DocumentManager_xh
#include "docmgr.xh"
#endif
#endif

#ifndef _BENTOERR_
#include "BentoErr.h"
#endif


#ifdef _PLATFORM_OS2_

#include <ILnkSrvc.xh>
#include <ILinkSrc.xh>

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef LMGRDEFS_H
#include "lmgrdefs.h"
#endif
  
#endif  // PLATFORM_OS2_

#pragma segment CMDraft

//==============================================================================
// Constants
//==============================================================================

const ODType    kODVersionNamePrefix = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Property:Bento Version Name";
const ODPropertyName kODPropRootSU  = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Property:DraftRootStorageUnit";
const ODType    kODStorageUnitType      = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Type:StorageUnit";
const ODType    kODStorageUnit          = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:ObjectType:StorageUnit";
const ODULong   kODInitialNumEntries = 8;

const corbastring kFrameClassName = "ODFrame";
                // $$$$$ This seems like a bad hardcoded dependency on a classname.

#define kODPropOriginalPID "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Metadata:OriginalPID"
#define kMaxStringSize  32
#define kInitialHashTableEntries 8

#define lazyOpen 1

// For debugging

#if ODDebug
// #define ODDebug_Drafts       1
// #define ODDebug_DebugRefCount 1
// #define ODDebug_CMDraft 1
// #define DebugClone 1
// #define ODDebug_VersionList  1
// #define ODDebug_CloningAnnotations 1
// #define ODDebug_Unloading_Classes 1
#endif


//==============================================================================
// Local Classes
//==============================================================================

class SULink : public Link {

public:

        SULink(ODStorageUnitID fromID, ODStorageUnitID toID)
        {
                fFromID = fromID;
                fToID = toID;
        }
        ~  SULink() {;};

        ODStorageUnitID GetFromID() {return fFromID;};
        ODStorageUnitID GetToID() {return fToID;};

private:

        ODStorageUnitID                         fFromID;
        ODStorageUnitID                         fToID;
};

//==============================================================================
// Function Prototypes
//==============================================================================

static CMStorageUnit* NewCMStorageUnit(ODMemoryHeapID heapID);

static CMObject AcquireDraftPropertiesObject(CMContainer container);
static ODType GetVersionNameFromVersionID(ODVersionID id, ODMemoryHeapID heapID);

static void SetOriginalDraft(Environment* ev, ODDraft* targetDraft, ODDraft* originalDraft);
static ODDraft* GetOriginalDraft(Environment* ev, ODDraft* draft);
static ODBoolean OriginalCloneKindExists(Environment* ev, ODDraft* draft);
static void SetOriginalCloneKind(Environment* ev, ODDraft* targetDraft, ODCloneKind cloneKind);

static ODBoolean IsLinkObject(Environment* ev, ODDraft* draft, ODID objectID);
static ODBoolean IsLinkSourceObject(Environment* ev, ODDraft* draft, ODID objectID);
static ODBoolean IsNeitherLinkObject(Environment* ev, ODDraft* draft, ODID objectID);

static void CM_itoa(ODULong number, ODSByte* cstring);   // KP

static ODULong PurgeAllStorageUnits(Environment* ev, OpenHashTable* storageUnits, IDList* idList);

static void SetupForUpdatingDraft(Environment* ev,
                                                                                CMDocument* localDoc,
                                                                                ODVersionID prevVersionID,
                                                                                CMValue version);

static ODBoolean CheckPartAction(void* k, void* v, ODULong s, void* r);

static void CopyProperty(Environment *ev, ODStorageUnit* fromSU, ODStorageUnit* toSU, ODPropertyName prop);
static void CopyDraftAnnotations(Environment *ev, ODStorageUnit* fromSU, ODStorageUnit* toSU);
static void SetStorageUnitType(Environment* ev, ODDraftPermissions perms, ODStorageUnit* su, ODType suType);
static ODISOStr GetStorageUnitType(Environment* ev, ODDraft* draft, ODID objectID);
static ODID RootPartID(Environment* ev, ODDraft* draft);

static void PurgeClasses(Environment* ev, OpenHashTable* partClasses);

// For debugging

#ifdef DebugStorage

#define MyDebugStr(s) do {somPrintf(s);} while (0)
#define MyDebug2Str(f,p1,p2) do {somPrintf(f, p1, p2);} while (0)

#else

#define MyDebugStr(s)
#define MyDebug2Str(f,p1,p2)

#endif

//==============================================================================
// Static variables
//==============================================================================

// Used by CheckPartAction
static CMDraft* sInterchangeDraft = kODNULL;
static ODISOStr sSUTypeBuffer;
static ODID             sRootPartIDToIgnore;

//------------------------------------------------------------------------------
// ReadClonedObjectTable
//------------------------------------------------------------------------------

// These are private property and value for Bento Container Suite.
// They are used for data interchange. They enable multiple clones to the clipboard
// and d&d container.
const ODPropertyName    kODPropClonedObjectTable  = "OpenDoc:Property:Draft:ClonedObjectTable";
const ODValueType               kODTypeClonedObjectTable  = "OpenDoc:Type:Draft:ClonedObjectTable";

#ifdef _PLATFORM_OS2_
const ODPropertyName   kODPropCloneOfALink = "OpenDoc:Property:Link:Cloned";
#endif


ODStatic void ReadClonedObjectTable(Environment* ev, OpenHashTable* clonedSUIDs, ODDraft* destDraft)
{
    try
    {
#ifdef DebugClone
        somPrintf("CMDraft: ReadClonedObjectTable\n");
#endif

	if (!destDraft)
		THROW(kODErrIllegalNullDraftInput);

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *su = destDraft->AcquireDraftProperties(ev);
#else
        TempODStorageUnit su = destDraft->AcquireDraftProperties(ev);
#endif
        if (ODSUExistsThenFocus(ev, su, kODPropClonedObjectTable, kODTypeClonedObjectTable) )
        {
                su->SetOffset(ev, 0);

                ODULong size = su->GetSize(ev);
                ODID    fromID;
                ODID    toID;

                while ( su->GetOffset(ev) < size )
                {
                        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &fromID);
                        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &toID);
#ifdef DebugClone
                        somPrintf("     fromID %d, toID %d\n", fromID, toID);
#endif
                        clonedSUIDs->ReplaceEntry(&fromID, &toID);
                }
        }
#ifdef _NO_TMP_OBJS_
        su->Release(ev);
#endif
   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// WriteClonedObjectTable
//------------------------------------------------------------------------------

ODStatic void WriteClonedObjectTable(Environment* ev, OpenHashTable* clonedSUIDs, ODDraft* destDraft)
{
    try
    {
#ifdef DebugClone
        somPrintf("CMDraft: WriteClonedObjectTable\n");
#endif

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *su = destDraft->AcquireDraftProperties(ev);
#else
        TempODStorageUnit su = destDraft->AcquireDraftProperties(ev);
#endif

        ODSUForceFocus(ev, su, kODPropClonedObjectTable, kODTypeClonedObjectTable);

        su->SetOffset(ev, 0);

        ODULong oldSize = su->GetSize(ev);

        ODID            fromID;
        ODID            toID;

        OpenHashTableIterator iter(clonedSUIDs);
        for (iter.First(&fromID, &toID); iter.IsNotComplete(); iter.Next(&fromID, &toID))
        {
#ifdef DebugClone
                somPrintf("     fromID %d, toID %d\n", fromID, toID);
#endif
                StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &fromID);
                StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &toID);
        }

        ODULong newSize = su->GetOffset(ev);
        if ( oldSize > newSize )
                su->DeleteValue(ev, oldSize - newSize);
#ifdef _NO_TMP_OBJS_
        su->Release(ev);
#endif
   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
}

//==============================================================================
// CMDraft
//==============================================================================

//------------------------------------------------------------------------------
// CMDraft: FailIfNotExclusiveWrite
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftFailIfNotExclusiveWrite(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","FailIfNotExclusiveWrite");

        if (_fPermissions != kODDPExclusiveWrite)
                ODSetSOMException(ev,kODErrInvalidPermissions);
}

//------------------------------------------------------------------------------
// CMDraft: GetDocument
//------------------------------------------------------------------------------

SOM_Scope ODDocument*  SOMLINK CMDraftGetDocument(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetDocument");

        return _fDocument;
}

//------------------------------------------------------------------------------
// CMDraft: GetID
//------------------------------------------------------------------------------

SOM_Scope ODDraftID  SOMLINK CMDraftGetID(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetID");

        return _fID;
}

//------------------------------------------------------------------------------
// CMDraft: AcquireDraftProperties
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK CMDraftAcquireDraftProperties(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AcquireDraftProperties");

        ODStorageUnit *retStorageUnit = kODNULL;
        CMStorageUnit*          draftProperties;

    try
    {
        CMObject                        draftPropertiesObject;
        ODStorageUnitID id;

        if (_fDraftProperties != kODNULL) {
                _fDraftProperties->Internalize(ev);
        }
        else {

                CMContainer cmContainer = somSelf->GetCMContainer(ev);
                ODSessionMustHaveCMAllocReserve(cmContainer);
                // AcquireDraftPropertiesObject() makes CM calls:

                draftPropertiesObject = AcquireDraftPropertiesObject(somSelf->GetCMContainer(ev));
                if (draftPropertiesObject == kODNULL)
                        THROW(kODErrNoDraftProperties);

                id = _fIDList->Add(draftPropertiesObject);
                draftProperties = NewCMStorageUnit(somSelf->GetHeap(ev));
                draftProperties->InitStorageUnit(ev, somSelf, id);
                _fStorageUnits->ReplaceEntry(&id, &draftProperties);
                _fDraftProperties = draftProperties;

                ODSessionRestoreCMAllocReserve(cmContainer);
        }

        _fDraftProperties->Acquire(ev);
        retStorageUnit = _fDraftProperties;

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
       
    }
        return _fDraftProperties;
}

//------------------------------------------------------------------------------
// CMDraft: Acquire
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftAcquire(CMDraft *somSelf, Environment *ev)
{
//  CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Acquire");

     try
     {
                parent_Acquire(somSelf, ev);
     }
     catch (ODException _exception)
     {
          ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMDraft: Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRelease(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Release");


    try
    {
        parent_Release(somSelf, ev);
        if (somSelf->GetRefCount(ev) == 0) {
                if (_fDraftProperties != kODNULL) {
                        _fDraftProperties->Release(ev);
                        _fDraftProperties = kODNULL;
                }
//              WASSERTM((somSelf->AreEmptyCollections(ev) != kODFalse), "OutstandingObjects in CMDraft: Release.");
                _fDocument->ReleaseDraft(ev, somSelf);
        }


    }
    catch (ODException _exception)
    {
                ODError err = ErrorCode();

                WARNMSG(WARN_INDEX(AMSG_610),"Error occurred in ODDraft::Release: %d %s",
                        err, ErrorMessage() ?ErrorMessage() :"");

                if (err == kODErrBentoErr){
                        SetErrorCode(kODErrFatalContainerError);
                        ODSetSOMException(ev, _exception);
                }
                else{
                     if (err != kODErrFatalContainerError)
                        SetErrorCode(kODNoError);
                     else
                        ODSetSOMException(ev, _exception);
                }

    }
}

//------------------------------------------------------------------------------
// CMDraft: GetPermissions
//------------------------------------------------------------------------------

SOM_Scope ODDraftPermissions  SOMLINK CMDraftGetPermissions(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetPermissions");

        return _fPermissions;
}

//------------------------------------------------------------------------------
// CMDraft: CreateStorageUnit
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK CMDraftCreateStorageUnit(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateStorageUnit");

        ODStorageUnit*  su = kODNULL; ODVolatile(su);

    try
    {

        somSelf->FailIfNotExclusiveWrite(ev);

        su = somSelf->CreateSU(ev, kODNULL, kODStorageUnit);
        SetStorageUnitType(ev, _fPermissions, su, kODStorageUnit);

        somSelf->SetChangedFromPrevFlag(ev, kODTrue);

     }
     catch (ODException _exception)
     {
                ODError error = ErrorCode();
                SaveEv();
                if (su != kODNULL) {
                        try
                        {
                                somSelf->ReleaseStorageUnit(ev, su->GetID(ev));
                        }
                        catch (ODException _exception)
                        {
                             SetErrorCode(kODNoError);   // ignore exception
                        }
                        su = kODNULL;
                }
                RestoreEv();
                ODSetSOMException(ev, error);
      }
        return su;
}

//------------------------------------------------------------------------------
// CMDraft: IsValidID
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftIsValidID(CMDraft *somSelf, Environment *ev,
                ODID id)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","IsValidID");

    ODBoolean  isValidID = kODFalse;
    try
    {
        if ( id != kODNULLID )
                isValidID =  _fIDList->Exists(id);

    }
    catch (ODException _exception)
    {
          ODSetSOMException(ev, _exception);
    }
        return isValidID;
}

//------------------------------------------------------------------------------
// CMDraft: AcquireStorageUnit
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK CMDraftAcquireStorageUnit(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AcquireStorageUnit");

        ODStorageUnit*  su = kODNULL;


    try
    {
        if (id == kODNULLID)
                THROW(kODErrIllegalNullIDInput);


        if (_fStorageUnits->GetValue(&id, &su)) {
                su->Acquire(ev);
        }
        else if ((_fIDList->Exists(id) == kODFalse) ||
                         (_fIDList->Get(id) != kODNULL)) {
                su = somSelf->CreateSU(ev, id, kODStorageUnit);
        }

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
        return su;
}

//------------------------------------------------------------------------------
// CMDraft: RemoveStorageUnit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRemoveStorageUnit(CMDraft *somSelf, Environment *ev,
                ODStorageUnit* su)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemoveStorageUnit");

   try
   {

        CMContainer cmContainer = somSelf->GetCMContainer(ev);
        ODSessionMustHaveCMAllocReserve(cmContainer);

        ODID                    id;
        CMObject                object;
        ODStorageUnit*  draftProperties;

        somSelf->FailIfNotExclusiveWrite(ev);

        if (!su)
                THROW(kODErrIllegalNullStorageUnitInput);

        draftProperties = somSelf->AcquireDraftProperties(ev);
        if (su == draftProperties) {
                draftProperties->Release(ev);
                THROW(kODErrIllegalOperationOnSU);
        }
        else
                draftProperties->Release(ev);

        // Get Storage Unit ID

        id = su->GetID(ev);
        if (id == kODNULL)
                THROW(kODErrInvalidStorageUnit);

        // Release the storage unit
        su->Release(ev);

        // Remove Storage Unit from outstanding SU collection

        _fStorageUnits->RemoveEntry(&id);

        // Get and delete the CMObject associated with su.

        object = (CMObject) _fIDList->Get(id);

        // Remove the entry in the ID-object collection.

        _fIDList->Remove(id);

        // Delete the object

        if (object != kODNULL)
                CMDeleteObject(object);

        // Destroy su

        delete su;

        // Mark this draft dirty

        somSelf->SetChangedFromPrevFlag(ev, kODTrue);

        ODSessionRestoreCMAllocReserve(cmContainer);

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: ChangedFromPrev
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftChangedFromPrev(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ChangedFromPrev");

        ODBoolean               changedFromPrev = kODFalse;

    try
    {

        VersionList*    versionList = kODNULL;

        versionList = _fDocument->TestAndGetVersionList(ev);
        ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

        try
        {
                changedFromPrev = somSelf->IsChangedFromPrev(ev, versionList);

        }
        catch (ODException _exception)
        {
            ODError error = ErrorCode();
            SaveEv();
            try
            {
                _fDocument->ReleaseVersionList(ev);
            }
            catch(ODException _exception)
            {
                SetErrorCode(kODNoError);
            }
            RestoreEv();
            ODSetSOMException(ev, error);
            throw;

        }

        _fDocument->ReleaseVersionList(ev);

    }
    catch (ODException _exception)
    {
          changedFromPrev = kODFalse;
          ODSetSOMException(ev, _exception);
    }
        return changedFromPrev;
}

//------------------------------------------------------------------------------
// CMDraft: SetChangedFromPrev
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftSetChangedFromPrev(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","SetChangedFromPrev");


    try
    {

#ifdef _PLATFORM_WIN32_
        DocumentManager  *docMgr = NULL;
        OleWrapper       *wrapper = NULL;

        docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);
        if (docMgr) {
            wrapper = docMgr->GetOleWrapper(ev);
            if (wrapper)
                wrapper->OnDataChange();
        }
#endif

        somSelf->FailIfNotExclusiveWrite(ev);

        somSelf->SetChangedFromPrevFlag(ev, kODTrue);

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: RemoveFromDocument
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRemoveFromDocument(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemoveFromDocument");


    try
    {
        _fDocument->CollapseDrafts(ev, somSelf, kODNULL);

    }
    catch (ODException _exception)
    {
       ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: RemoveChanges
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMDraftRemoveChanges(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemoveChanges");

   try
   {

        VersionList*    versionList = kODNULL;

        somSelf->FailIfNotExclusiveWrite(ev);

        if (_fDraftProperties != kODNULL) {
                _fDraftProperties->Release(ev);
                _fDraftProperties = kODNULL;
        }

//      WASSERTM((somSelf->AreEmptyCollections(ev) != kODFalse), "OutstandingObjects in CMDraft: RemoveChanges.");

        somSelf->DestroyVersion(ev);

        versionList = _fDocument->TestAndGetVersionList(ev);
        ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

#ifdef ODDebug_VersionList
        _fDocument->GetVersionList(ev)->Print(">>> Entering RemoveChanges");
#endif

        try
        {
                if (versionList->Exists(_fID) != kODFalse)
                        versionList->RemoveChanges(_fID);

        }
        catch (ODException _exception)
        {
            ODError error = ErrorCode();
            SaveEv();
            try
            {
                _fDocument->ReleaseVersionList(ev);
            }
            catch (ODException _exception)
            {
                SetErrorCode(kODNoError);
            }
            RestoreEv();
            ODSetSOMException(ev, error);
            throw;

        }

#if !TestFlushContainer
        _fDocument->ExternalizeVersionList(ev, kODFalse);
#endif
        _fDocument->ReleaseVersionList(ev);

        somSelf->Open(ev);

        somSelf->SetChangedFromPrevFlag(ev, kODFalse);

#ifdef ODDebug_VersionList
        _fDocument->GetVersionList(ev)->Print(">>> Exiting RemoveChanges");
#endif

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
        return somSelf;
}

//------------------------------------------------------------------------------
// CMDraft: Externalize
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMDraftExternalize(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Externalize");

    try
    {

#ifdef _PLATFORM_OS2_                          // $$$$ dcp # 120702  05/23/95
        if ( _fPermissions == kODDPReadOnly ) THROW(kODErrInvalidPermissions);

        // check to see if linking is on or off (131199)

        ODSession* session = (ODSession*) _fDocument->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev);
        char flag = session->GetLinkONOFFflag(ev);
        if(flag)
        {
           // Call LinkService's externalize method so that
           // tables not yet stored to DraftProperties can be saved
           if (_fPermissions == kODDPExclusiveWrite) _fLinkService->DraftSaving(ev, somSelf);
        }

#endif

        somSelf->ExternalizeCollections(ev);

        _fExternalized = kODTrue;

        // DO THE ACTUAL EXTERNALIZATION HERE

        somSelf->FlushVersion(ev);

   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
        return somSelf;
}

//------------------------------------------------------------------------------
// CMDraft: SaveToAPrevious
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMDraftSaveToAPrevious(CMDraft *somSelf, Environment *ev,
                ODDraft* to)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","SaveToAPrevious");


   try
   {
        _fDocument->SaveToAPrevDraft(ev, somSelf, to);

   }
   catch( ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
        return somSelf;
}

//------------------------------------------------------------------------------
// CMDraft: CreateFrame
//------------------------------------------------------------------------------

SOM_Scope ODFrame*  SOMLINK CMDraftCreateFrame(CMDraft *somSelf, Environment *ev,
                ODObjectType            frameType,
                ODFrame*        containingFrame,
                ODShape*        frameShape,
                ODCanvas*       biasCanvas,
                ODPart*                 part,
                ODTypeToken     viewType,
                ODTypeToken     presentation,
                ODBoolean               isSubframe,
                ODBoolean               isOverlaid)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateFrame");

        ODStorageUnit*          su = kODNULL;
        ODFrame*                        frame = kODNULL;
        ODStorageUnitID         id = kODNULLID;

        ODVolatile(su);
        ODVolatile(frame);
        ODVolatile(id);


     try
     {
                if (ODISOStrEqual(frameType, kODFrameObject) != kODFalse)
                        somSelf->FailIfNotExclusiveWrite(ev);

                frame = new ODFrame();
                if (frame == kODNULL)
                        THROW(kODErrCannotCreateFrame);

                 if (ODISOStrEqual(frameType, kODFrameObject) != kODFalse) {
                        su = somSelf->CreateSU(ev, kODNULL, kODFrameObject);
                        SetStorageUnitType(ev, _fPermissions, su, kODFrameObject);
                        id = su->GetID(ev);
                        _fPersistentObjects->ReplaceEntry(&id, &frame);
                        frame->InitFrame(ev, su, containingFrame, frameShape, biasCanvas, part,
                                                         viewType, presentation, isSubframe, isOverlaid);
                        somSelf->SetChangedFromPrevFlag(ev, kODTrue);
                }
                else if (ODISOStrEqual(frameType, kODNonPersistentFrameObject) != kODFalse) {
                        id = _fIDList->Add(kODNULL);
                        _fPersistentObjects->ReplaceEntry(&id, &frame);
                        frame->InitFrameNonPersistent(ev, somSelf, id, containingFrame, frameShape,
                                                biasCanvas, part, viewType, presentation, isSubframe, isOverlaid);
                }
                else
                        THROW(kODErrInvalidObjectType);


     }
     catch (ODException _exception)
     {
                ODError error = ErrorCode();
                SaveEv();
                if (frame != kODNULL) {
                        try{
                                if (id != kODNULLID)
                                {
                                        if (su == kODNULL)
                                        {
                                                // This is just for non-persistent frame.
                                                // Every other real persistent object should use RemovePersistentObject.
                                                ODReleaseObject(ev, frame);
                                                _fPersistentObjects->RemoveEntry(&id);
                                        }
                                        else
                                                somSelf->RemovePersistentObject(ev, frame);
                                }
                                else {
                                        delete frame;
                                        if (su != kODNULL)
                                                somSelf->RemoveStorageUnit(ev, su);
                                }
                        }catch (ODException _exception){
                           SetErrorCode(kODNoError);     // ignore exception
                        }
                        frame = kODNULL;
                }
                RestoreEv();
                ODSetSOMException(ev, error);
        }

        return frame;
}

//------------------------------------------------------------------------------
// CMDraft: AcquireFrame
//------------------------------------------------------------------------------

SOM_Scope ODFrame*  SOMLINK CMDraftAcquireFrame(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AcquireFrame");

        ODStorageUnit*  su = kODNULL; ODVolatile(su);
        ODFrame*                frame = kODNULL; ODVolatile(frame);

    try
    {
        if (id == kODNULLID)
                THROW(kODErrIllegalNullIDInput);

        frame = (ODFrame*) somSelf->RetrievePersistentObject(ev, id);

        if (frame == kODNULL) {

                su = somSelf->AcquireStorageUnit(ev, id);
                frame = new ODFrame();

                if (frame == kODNULL)
                        THROW(kODErrCannotAcquireFrame);

                // A better way to do this is to let ODFrame::InitFrameFromStorage or
                // ODPersistentObject::InitPersistentObjectFromStorage to increment the
                // refcount. In that way, we can always release su here (or even better
                // make su into a temp obj. - VL
                _fPersistentObjects->ReplaceEntry(&id, &frame);
                frame->InitFrameFromStorage(ev, su);

                ODStorageUnit *su2 = su;
                su = kODNULL;                                                                   // frame owns reference now

                SetStorageUnitType(ev, _fPermissions, su2, kODFrameObject);
        }

     }
     catch(ODException _exception)
     {
                ODError error = ErrorCode();
                if( frame ) WASSERT(frame->GetRefCount(ev)==1);
                SaveAndRestoreEv1(frame);
                ODSetSOMException(ev, error);
     }

        return frame;
}

//------------------------------------------------------------------------------
// CMDraft: ReleaseFrame
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftReleaseFrame(CMDraft *somSelf, Environment *ev,
                ODFrame* frame)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ReleaseFrame");


    try
    {
        somSelf->ReleasePersistentObject(ev, frame);

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: RemoveFrame
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRemoveFrame(CMDraft *somSelf, Environment *ev,
                ODFrame* frame)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemoveFrame");

   try
   {
        somSelf->FailIfNotExclusiveWrite(ev);

        if (!frame)
                THROW(kODErrIllegalNullFrameInput);

        if (frame->GetStorageUnit(ev) == kODNULL)
                frame->Release(ev);
        else
                somSelf->RemovePersistentObject(ev, frame);
   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }

}

//------------------------------------------------------------------------------
// CMDraft: ConstructRealPart
//------------------------------------------------------------------------------

SOM_Scope ODPart*  SOMLINK CMDraftConstructRealPart(CMDraft *somSelf, Environment *ev,
                ODStorageUnit* su, ODBoolean isInitPartFromStorage,
                ODPartWrapper* partWrapper,
                ODType partType, ODEditor theEditor)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ConstructRealPart");

        ODPart* part = kODNULL; ODVolatile(part);
        ODEditor editorForPart = kODNULL;
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOMClass*       partClass;
        somId           partsSomId;
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_


      try
      {
                ODSession* session = (ODSession*) _fDocument->
                        GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev);
                ODBinding* binding = session->GetBinding(ev);

                if (theEditor == kODNULL)
                        editorForPart = binding->
                                ChooseEditorForPart(ev, su, partType);
                else
                {
                        editorForPart = theEditor;
                        if (_fPermissions != kODDPReadOnly)
                                ODSetISOStrProp(ev, su, kODPropPreferredEditor,
                                                 kODEditor, editorForPart);
                }

#ifdef _PLATFORM_MACINTOSH_
                if (ODISOStrEqual(editorForPart,
                                  kODBlackBoxHandlerOfLastResort))
                        part = binding->ConstructNoPart(ev);
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
                if ( (ODISOStrEqual(editorForPart, "NoPart")) ||
                     (ODISOStrEqual(editorForPart, "")) )
                        part = new NoPart;

#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_

                else

#ifdef _PLATFORM_MACINTOSH_
                        part = (ODPart*) ODNewObject(editorForPart);
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
                        {
                        partsSomId = somIdFromString((string)editorForPart);
                        partClass = SOMClassMgrObject->somFindClass(partsSomId, 0, 0);
                        if (!partClass) {
                                WARNMSG_DEBUG(WARN_INDEX(-1),
                                   "The Part Handler %s failed to load!",
                                   editorForPart);
                                part = new NoPart;
                           }
                        else {
                                part = (ODPart *)partClass->somNew();
                           }

                        }

#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_AIX_

                if (part == kODNULL)
                        THROW(kODErrCannotCreatePart);

                partWrapper->SetRealPart(ev, part, editorForPart);

                ODID id = su->GetID(ev);
                _fPersistentObjects->ReplaceEntry(&id, &partWrapper);

                su->Acquire(ev);                                                                 //This new reference belongs to the part
                if (isInitPartFromStorage)
                        partWrapper->InitPartFromStorage(ev, su, partWrapper);
                else
                        partWrapper->InitPart(ev, su, partWrapper);

                if (ODISOStrEqual(editorForPart,
                        kODBlackBoxHandlerOfLastResort) == kODFalse)
                        somSelf->PartInstantiated(ev, part);
      }
      catch (ODException _exception)
      {
                part = kODNULL;

                // Map to more specific error
                if( ErrorCode() == kODErrCantLoadSOMClass )
                        SetErrorCode(kODErrCannotCreatePart);
                ODSetSOMException(ev, _exception);

      }

#ifdef _PLATFORM_MACINTOSH_
        if (editorForPart != theEditor)
                ODDisposePtr(editorForPart);
#endif // _PLATFORM_MACINTOSH_

        return part;
}

//------------------------------------------------------------------------------
// CMDraft: ReleaseRealPart
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftReleaseRealPart(CMDraft *somSelf, Environment *ev,
                                                                                                ODPart* realPart)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ReleaseRealPart");


   try
   {
        if (realPart == kODNULL)
                THROW(kODErrIllegalNullPartInput);

        if (realPart->GetRefCount(ev) != 0)
                THROW(kODErrRefCountGreaterThanZero);

        realPart->ReleaseAll(ev);

        somSelf->PartDeleted(ev, realPart);

        delete realPart;

   }
   catch (ODException _exception)
   {
      ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: DeleteRealPart
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftDeleteRealPart(CMDraft *somSelf, Environment *ev,
                                                                                                ODPart* realPart)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","DeleteRealPart");


   try
   {
        if (realPart == kODNULL)
                THROW(kODErrIllegalNullPartInput);

        somSelf->PartDeleted(ev, realPart);

        delete realPart;

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: CreatePart
//------------------------------------------------------------------------------

SOM_Scope ODPart*  SOMLINK CMDraftCreatePart(CMDraft *somSelf, Environment *ev,
                ODType partType, ODEditor optionalEditor)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreatePart");


        ODPartWrapper* partWrapper = kODNULL;

        ODVolatile(partWrapper);

    try
    {
                somSelf->FailIfNotExclusiveWrite(ev);

#ifdef _NO_TMP_OBJS_
                ODStorageUnit *su = somSelf->CreateSU(ev, kODNULL, kODPartObject);
#else
                TempODStorageUnit su = somSelf->CreateSU(ev, kODNULL, kODPartObject);
#endif

                SetStorageUnitType(ev, _fPermissions, su, kODPartObject);

                if ( partType != kODNULL )
                {
                        ODSetISOStrProp(ev, su, kODPropPreferredKind, kODISOStr, partType);
                }

                partWrapper = new ODPartWrapper;
                THROW_IF_NULL(partWrapper, kODErrOutOfMemory);
                partWrapper->InitPartWrapper(ev);

                somSelf->ConstructRealPart(ev, su, kODFalse, partWrapper, partType, optionalEditor);

                somSelf->SetChangedFromPrevFlag(ev, kODTrue);
#ifdef _NO_TMP_OBJS_
                su->Release(ev);
#endif


      }
      catch (ODException _exception)
      {
                ODError error = ErrorCode();
                SaveAndRestoreEv1(partWrapper);
                partWrapper = kODNULL;
                ODSetSOMException(ev, error );

      }

        return partWrapper;
}

//------------------------------------------------------------------------------
// CMDraft: AcquirePart
//------------------------------------------------------------------------------

SOM_Scope ODPart*  SOMLINK CMDraftAcquirePart(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AcquirePart");

        ODPart* part = kODNULL;
        ODPartWrapper* partWrapper = kODNULL;

        ODVolatile(partWrapper);

      try
      {
                if (id == kODNULLID)
                        THROW(kODErrIllegalNullIDInput);

                part = (ODPart*) somSelf->RetrievePersistentObject(ev, id);
                if (part == kODNULL) {
                        ODStorageUnit*  su = kODNULL;

                        if (_fStorageUnits->GetValue(&id, &su) != kODFalse)
                                su->Acquire(ev);
                        else
                                su = somSelf->CreateSU(ev, id, kODPartObject);
#ifdef _NO_TMP_OBJS_
                        ODStorageUnit *tempSU = su; // ensure it's released
#else
                        TempODStorageUnit tempSU = su; // ensure it's released
#endif

                        partWrapper = new ODPartWrapper;
                        THROW_IF_NULL(partWrapper, kODErrOutOfMemory);
                        partWrapper->InitPartWrapper(ev);

                        somSelf->ConstructRealPart(ev, su, kODTrue, partWrapper, kODNULL,  kODNULL);

                        part = partWrapper;

                        SetStorageUnitType(ev, _fPermissions, su, kODPartObject);
#ifdef _NO_TMP_OBJS_
                        tempSU->Release(ev);
#endif
                }


      }
      catch (ODException _exception)
      {
                ODError  error = ErrorCode();
                SaveAndRestoreEv1(partWrapper);
                part = kODNULL;
                ODSetSOMException(ev, error);

      } 

        return part;
}

//------------------------------------------------------------------------------
// CMDraft: ReleasePart
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftReleasePart(CMDraft *somSelf, Environment *ev,
                ODPart* part)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ReleasePart");

   try
   {

        somSelf->ReleasePersistentObject(ev, part);

   }
   catch (ODException _exception)
   {
      ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: RemovePart
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRemovePart(CMDraft *somSelf, Environment *ev,
                ODPart* part)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemovePart");

   try
   {

        if (!part)
                THROW(kODErrIllegalNullPartInput);

        somSelf->RemovePersistentObject(ev, part);

   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: CreateLinkSpec
//------------------------------------------------------------------------------

SOM_Scope ODLinkSpec*  SOMLINK CMDraftCreateLinkSpec (CMDraft *somSelf, Environment *ev,
                ODPart* part,
                ODByteArray* data)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateLinkSpec");

        ODLinkSpec* ls = kODNULL;

        ODVolatile(ls);
#ifdef _PLATFORM_OS2_

   try
   {
    ls = new ODLinkSpec;

    // check to see if linking is on or off (131199)

    ODSession* session = (ODSession*) _fDocument->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev);
    char flag = session->GetLinkONOFFflag(ev);
    if(flag)
    {
      if (_fLinkServiceXID == kODNULL)
      {
        session->StartServer(ev, (char*)kODNULL);
        SOMDObject* objRef = SOMD_ServerObject->
                               somdRefFromSOMObj(ev, _fLinkService);
        _fLinkServiceXID = SOMD_ORBObject->object_to_string(ev, objRef);
      }

      //XXXXX ls->InitLinkSpec(ev, _fLinkServiceXID, part->GetID(ev), data);
      ls->InitLinkSpec(ev, _fLinkServiceXID, part, data);
    }
   }
   catch (ODException _exception)
   {
      ODError error = ErrorCode();
      SaveEv();
      try
      {
          ODDeleteObject(ls);
      }
      catch(ODException _exception)
      {
          SetErrorCode(kODNoError);
      }
      RestoreEv();
      ODSetSOMException(ev, error);
   }
  
#else

    try
    {
                ls = new ODLinkSpec;
                THROW_IF_NULL(ls, kODErrOutOfMemory);
// #ifdef _PLATFORM_OS2_
//                ls->InitLinkSpec(ev, kODNULL /* 22470 */ , part, data);
// #endif
#if defined (_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
                ls->InitLinkSpec(ev, part, data);
#endif

     }
     catch (ODException _exception)
     {
      ODError error = ErrorCode();
       SaveEv();
      try
      {
          ODDeleteObject(ls);
      }
      catch(ODException _exception)
      {
          SetErrorCode(kODNoError);
      }
      RestoreEv();
      ODSetSOMException(ev, error);
     }
#endif // _PLATFORM_OS2_

        return ls;
}

//------------------------------------------------------------------------------
// CMDraft: CreateLinkSource
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK CMDraftCreateLinkSource(CMDraft *somSelf, Environment *ev,
                ODPart* part)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateLinkSource");


#ifdef _PLATFORM_OS2_

    IODLinkSource*  linkSource = kODNULL;
  try
  {
    char            docName[256];
    char            hostName[256];
    ODStorageUnitID id;
    ODStorageUnit*  su;
    ODISOStr        ASName;

    if (part==kODNULL)
      THROW(kODErrCannotCreateLink);

    somSelf->FailIfNotExclusiveWrite(ev);


    su = somSelf->CreateSU(ev, kODNULL, kODLinkSource);

    // $$$$$ Links no longer depend on this property
    // $$$$$  (but LinkSourceIterator does!)
    su->AddProperty(ev, kODPropStorageUnitType);
    su->AddValue(ev, kODISOStr);
    StorageUnitSetValue(su,
                        ev,
                        ODISOStrLength(kODLinkSource)+1,
                        (ODValue)kODLinkSource);

    linkSource = new IODLinkSource();
    if (linkSource == kODNULL)
    {
      delete su;
      THROW(kODErrCannotCreateLink);
    }

    id = su->GetID(ev);
    _fPersistentObjects->ReplaceEntry(&id, &linkSource);

     if(getenv("HOSTNAME") == NULL)                   //@SK [123273]
       {                                              //@SK [123273]

#ifdef ODDebug
       somPrintf("\nError: Incomplete DSOM environment setup: Missing HOSTNAME\n");//@SK [123273]
#endif
       DosExit(EXIT_PROCESS, 1);                      //@SK [123273]
       }
    else
       {

         linkSource->InitILinkSource(ev,
                                     su,
                                     part,
                                     getenv("HOSTNAME"),
                                     _fLinkService->GetDocName(ev));

         ODLinkID linkID =
           _fLinkService->RegisterLinkSource(ev, somSelf, id, linkSource, part, &ASName);

         linkSource->CompleteInitILinkSource(ev, su, linkID, (IODAuxiliaryState*)kODNULL, kODFalse, ASName);
         ODDisposePtr(ASName);
       somSelf->SetChangedFromPrevFlag(ev, kODTrue);
       return (ODLinkSource*)linkSource;
      }
   }
   catch (ODException _exception)
   {
       linkSource = kODNULL;
       ODSetSOMException(ev, _exception);
   }
       return (ODLinkSource*)linkSource;


#else
        ODLinkSource*   linkSource = kODNULL;

    try
    {
        ODStorageUnitID id;

        somSelf->FailIfNotExclusiveWrite(ev);

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *su = somSelf->CreateSU(ev, kODNULL, kODLinkSource);
#else
        TempODStorageUnit su = somSelf->CreateSU(ev, kODNULL, kODLinkSource);
#endif

        // The implementation of CMLinkSourceIterator depends on this property being present
        // in link source storage units and nowhere else.

        SetStorageUnitType(ev, _fPermissions, su, kODLinkSource);

        linkSource = new ODLinkSource();
        THROW_IF_NULL(linkSource,kODErrCannotCreateLink);

#ifdef _NO_TMP_OBJS_
        ODLink *link = kODNULL;
#else
        TempODLink link = kODNULL;
#endif
        try
        {
                link = somSelf->CreateLink(ev);
        }
        catch (ODException _exception)
        {
            ODError error = ErrorCode();
            SaveEv();
            try
            {
                linkSource->ReleaseAll(ev);
                ODDeleteObject(linkSource);
            }
            catch(ODException _exception)
            {
                 SetErrorCode(kODNoError);
            }
            RestoreEv();
            ODSetSOMException(ev, error);
            throw;
        }

        id = su->GetID(ev);

        _fPersistentObjects->ReplaceEntry(&id, &linkSource);

#ifdef _NO_TMP_OBJS_
        linkSource->InitLinkSource(ev, su, part);
#else
        linkSource->InitLinkSource(ev, su.DontRelease(), part);
#endif
        // ("DontDelete" prevents su from being auto-released by destructor)

        link->SetLinkSource(ev, linkSource);
        linkSource->SetLink(ev, link);

        somSelf->SetChangedFromPrevFlag(ev, kODTrue);

#ifdef _NO_TMP_OBJS_
        su->Release(ev);
        link->Release(ev);
#endif


        }
        catch (ODException _exception)
        {
	     linkSource = kODNULL;
             ODSetSOMException(ev, _exception);
        }
        return linkSource;
#endif  // _PLATFORM_OS2_
}

//------------------------------------------------------------------------------
// CMDraft: AcquireLinkSource
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource*  SOMLINK CMDraftAcquireLinkSource(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AcquireLinkSource");



#ifdef _PLATFORM_OS2_


    // If the id already corresponds to a live linkSource, use it.
    // If not, create a new one.
    // Update the cache so that next time this new linkSource will be found.
    // Get the SU.
    // If this is a read-only draft, create the linkSource with a null auxState
    // Otherwise, call LinkService::OpenLinkSource to get auxState/lock
    //    and use this to internalize the linksource.

    IODLinkSource* linkSource = (IODLinkSource*)kODNULL;
  try
  { 
        if (id == kODNULLID)
                THROW(kODErrIllegalNullIDInput);

    linkSource = ((IODLinkSource*)somSelf->RetrievePersistentObject(ev, id));

    if (linkSource == (IODLinkSource*)kODNULL)
    {
      linkSource = new IODLinkSource();
      if (linkSource == (IODLinkSource*)kODNULL)
      {
        THROW(kODErrCannotAcquireLink);
      }

      ODStorageUnit* su = (ODStorageUnit*)kODNULL;
      ODVolatile(su);
      ODVolatile(id);

      try
      {

        su = somSelf->AcquireStorageUnit(ev, id);
        _fPersistentObjects->ReplaceEntry(&id, &linkSource);
        if (_fPermissions != kODDPExclusiveWrite)
        {
          ODLinkID linkid = linkSource->InitILinkSourceFromStorage(ev, su);
          linkSource->CompleteInitILinkSource(ev, su, linkid, kODNULL, kODFalse, "");
        }
        else
        {
          IODAuxiliaryState* auxState;
          ODBoolean isLocked;
          ODISOStr        ASName;

          ODLinkID originalLinkID = linkSource->InitILinkSourceFromStorage(ev, su);
          ODLinkID finalLinkID = _fLinkService->OpenLinkSource(ev,
                                        somSelf,
                                        linkSource->GetID(ev),
                                        linkSource,
                                        originalLinkID,
                                        &auxState,
                                        &isLocked,
                                        &ASName);

          linkSource->CompleteInitILinkSource(ev,
                                              su,
                                              finalLinkID,
                                              auxState,
                                              isLocked,
                                              ASName);
          if (auxState != kODNULL)
          {
            SOMFree(auxState->remoteLinkInfos._buffer);
            SOMFree(auxState);
          }
          ODDisposePtr(ASName);
        }

      }
      catch (ODException _exception)
      {

       SaveEv();
       try
       {
        if (su != (ODStorageUnit*) kODNULL)
        {
          _fPersistentObjects->RemoveEntry(&id);
          su->Release(ev);
        }

        linkSource->ReleaseAll(ev);
        delete linkSource;
       }
       catch(ODException _exception)
       {
            SetErrorCode(kODNoError);
       }
       ODError error = ErrorCode();
       ODSetSOMException(ev, error);
       throw;

      }
    }
   }
   catch (ODException _exception)
   {
        linkSource = kODNULL;
        ODSetSOMException(ev, _exception);
   }

    return (ODLinkSource*)linkSource;

#else
        ODLinkSource* linkSource = (ODLinkSource*) kODNULL;
   try
   {
        if (id == kODNULLID)
                THROW(kODErrIllegalNullIDInput);

        linkSource = ((ODLinkSource*) somSelf->RetrievePersistentObject(ev, id));

        if (linkSource == (ODLinkSource*) kODNULL) {
                linkSource = new ODLinkSource();
                if (linkSource == (ODLinkSource*) kODNULL)
                                THROW(kODErrCannotAcquireLink);

                ODStorageUnit*  su = (ODStorageUnit*) kODNULL;
                ODVolatile(su);
                ODVolatile(id);
                try
                {
                        su = somSelf->AcquireStorageUnit(ev, id);
                        _fPersistentObjects->ReplaceEntry(&id, &linkSource);
                        linkSource->InitLinkSourceFromStorage(ev, su);
                        SetStorageUnitType(ev, _fPermissions, su, kODLinkSource);
                }
                catch (ODException _exception)
                {
                    ODError error = ErrorCode();
                    SaveEv();
      		    try
      		    {

                        if (su != (ODStorageUnit*) kODNULL) {
                                _fPersistentObjects->RemoveEntry(&id);
                                su->Release(ev);
                                }
                        linkSource->ReleaseAll(ev);
                        delete linkSource;
                    }
                    catch(ODException _exception)
      		    {
                            SetErrorCode(kODNoError);
                    }
      	            RestoreEv()
                    ODSetSOMException(ev, error);

                        throw;
                }
                }

     }
     catch (ODException  _exception)
     {
          linkSource = kODNULL;
          ODSetSOMException(ev, _exception);
     }
        return linkSource;
#endif  // _PLATFORM_OS2_

}

//------------------------------------------------------------------------------
// CMDraft: RemoveLinkSource
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRemoveLinkSource(CMDraft *somSelf, Environment *ev,
                ODLinkSource* linkSource)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemoveLinkSource");


   try
   {
        somSelf->FailIfNotExclusiveWrite(ev);

        somSelf->RemovePersistentObject(ev, linkSource);

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: CreateLink
//------------------------------------------------------------------------------

SOM_Scope ODLink*  SOMLINK CMDraftCreateLink(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateLink");


#ifdef _PLATFORM_OS2_

    IODLinkTarget*  link = kODNULL;
   try
   {

      ODStorageUnitID id;
      ODStorageUnit*  su;

      su = somSelf->CreateSU(ev, kODNULL, kODLink);

      // $$$$$ Links no longer depend on this property
      // $$$$$ (but LinkSourceIterator does!)
      su->AddProperty(ev, kODPropStorageUnitType);
      su->AddValue(ev, kODISOStr);
      StorageUnitSetValue(su,
                          ev,
                          ODISOStrLength(kODLink)+1,
                          (ODValue)kODLink);

      link = new IODLinkTarget();
      if (link == kODNULL)
      {
        delete su;
        THROW(kODErrCannotCreateLink);
      }

      id = su->GetID(ev);
      _fPersistentObjects->ReplaceEntry(&id, &link);

      link->InitLinkTarget(ev, su, _fLinkService->GetDocName(ev));

      somSelf->SetChangedFromPrevFlag(ev, kODTrue);
   }
   catch (ODException _exception)
   {
       link = kODNULL;
       ODSetSOMException(ev, _exception);
   }

      return (ODLink*)link;

#else
        ODLink*         link = kODNULL;

   try
   {
        ODStorageUnitID id;
        ODStorageUnit*  su;

        su = somSelf->CreateSU(ev, kODNULL, kODLink);

        // The implementation of CMLinkIterator depends on this property being present
        // in link storage units and nowhere else.
        SetStorageUnitType(ev, _fPermissions, su, kODLink);

        link = new ODLink();
        if (link == kODNULL)
        {
                delete su;
                THROW(kODErrCannotCreateLink);
        }

        id = su->GetID(ev);
        _fPersistentObjects->ReplaceEntry(&id, &link);

        link->InitLink(ev, su);

        somSelf->SetChangedFromPrevFlag(ev, kODTrue);

    }
    catch (ODException _exception)
    {
         link = kODNULL;
         ODSetSOMException(ev, _exception);
    }
        return link;
#endif // _PLATFORM_OS2_
}

//------------------------------------------------------------------------------
// CMDraft: AcquireLink
//------------------------------------------------------------------------------

SOM_Scope ODLink*  SOMLINK CMDraftAcquireLink(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id,ODLinkSpec* linkSpec)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AcquireLink");

#ifdef _PLATFORM_OS2_
        IODLinkTarget* link = (IODLinkTarget*)kODNULL;
#else
        ODLink* link = (ODLink*) kODNULL;
#endif

   try
   {

#ifdef _PLATFORM_OS2_

    if (id != (ODStorageUnitID)kODNULL)
    {
      ODPersistentObject*  object;
      object = somSelf->RetrievePersistentObject(ev, id);

      if (IsLinkObject(ev, somSelf, id))
      {
       link = (IODLinkTarget*)object;
      }
      else
      {
       THROW(kODErrInvalidID);
      }
      if (link == (IODLinkTarget*)kODNULL)
      {
        link = new IODLinkTarget();
        if (link == (IODLinkTarget*)kODNULL)
        {
          THROW(kODErrCannotAcquireLink);
        }

        ODStorageUnit* su = (ODStorageUnit*)kODNULL;
        ODVolatile(su);
        ODVolatile(id);

        try
        {

          su = somSelf->AcquireStorageUnit(ev, id);
          _fPersistentObjects->ReplaceEntry(&id, &link);
          link->InitLinkTargetFromStorage(ev, su, _fLinkService->GetDocName(ev));

        }
        catch (ODException _exception)
        {

          ODError error = ErrorCode();
          SaveEv();
          try
          {
              if (su != (ODStorageUnit*) kODNULL)
              {
                 _fPersistentObjects->RemoveEntry(&id);
                 su->Release(ev);
              }
              link->ReleaseAll(ev);
              delete link;
          }
          catch(ODException _exception)
          {
                SetErrorCode(kODNoError);
          }
          RestoreEv();
          ODSetSOMException(ev, error);

          throw;

        }
      }
    }
    else if (linkSpec == (ODLinkSpec*)kODNULL)
    {
       THROW(kODErrInvalidID);
    }
    else
    {
       link = (IODLinkTarget*)somSelf->CreateLink(ev);
       link->EstablishLink(ev, linkSpec, _fLinkService);
    };


#else


        if (id != (ODStorageUnitID) kODNULL) {
                link = ((ODLink*) somSelf->RetrievePersistentObject(ev, id));
                if (link == (ODLink*) kODNULL) {
                        link = new ODLink();
                        if (link == kODNULL)
                                THROW(kODErrCannotAcquireLink);

                        ODStorageUnit*  su = (ODStorageUnit*) kODNULL;
                        ODVolatile(su);
                        ODVolatile(id);
                        try
                        {
                                su = somSelf->AcquireStorageUnit(ev, id);
                                _fPersistentObjects->ReplaceEntry(&id, &link);
                                link->InitLinkFromStorage(ev, su);
                                SetStorageUnitType(ev, _fPermissions, su, kODLink);
                        }
                        catch(ODException _exception)
                        {
                            ODError error = ErrorCode();
                            SaveEv();
      			    try
      			    {

                                if (su != (ODStorageUnit*) kODNULL) {
                                        _fPersistentObjects->RemoveEntry(&id);
                                        su->Release(ev);
                                        }
                                link->ReleaseAll(ev);
                                delete link;
                            }
      			    catch(ODException _exception)
      			    {
                                  SetErrorCode(kODNoError);
                            }
                            RestoreEv();
                            ODSetSOMException(ev, error);

                                throw;
                        }
                        }
                }
        else if (linkSpec == (ODLinkSpec*) kODNULL) {
                THROW(kODErrInsufficientInfoInParams);
                }
        else if (linkSpec->FromThisDraft(ev)) {
                ODPart* part = linkSpec->GetPart(ev);
                ODByteArray partData = linkSpec->GetPartData(ev);
                ODVolatile(partData);
                ODLinkSource* linkSource = kODNULL;
                ODVolatile(linkSource);
                try
                {
                        linkSource = part->CreateLink(ev, &partData);
                        if ( linkSource != (ODLinkSource*) kODNULL ) {
                                link = linkSource->GetLink(ev);
                                if ( link != (ODLink*) kODNULL )
                                        link->Acquire(ev);
                                }
                }
                catch (ODException _exception)
                {
                        ODError error = ErrorCode();
                        DisposeByteArrayStruct(partData);
                        SaveAndRestoreEv1(linkSource);
                        ODSetSOMException(ev, error);
                        throw;
                }
                DisposeByteArrayStruct(partData);
                ODReleaseObject(ev, linkSource);
                }
        else {
                // This link spec originated in another document; forward the
                // AcquireLink call to the originating draft.
                ODSession* session = _fDocument->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev);
                if (session != kODNULL) {
                        ODLinkSource* linkSource = kODNULL;
                        ODVolatile(linkSource);
                        try
                        {
                                linkSource = session->GetLinkManager(ev)->CreateLink(ev, somSelf, linkSpec);
                                if ( linkSource != (ODLinkSource*) kODNULL ) {
                                        link = linkSource->GetLink(ev);
                                        if ( link != (ODLink*) kODNULL )
                                                link->Acquire(ev);
                                        }
                        }
                        catch (ODException _exception)
                        {
                                ODError error = ErrorCode();
                                SaveAndRestoreEv1(linkSource);
                                ODSetSOMException(ev, error);
                                throw;
                        }
                        ODReleaseObject(ev, linkSource);
                        }
                }
#endif // _PLATFORM_OS2_

        }
        catch (ODException _exception)
        {
                link = kODNULL;
                ODSetSOMException(ev, _exception);
        }
        return (ODLink*)link;
}

//------------------------------------------------------------------------------
// CMDraft: ReleaseLink
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftReleaseLink(CMDraft *somSelf, Environment *ev,
                ODLink* link)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ReleaseLink");

   try
   {

        somSelf->ReleasePersistentObject(ev, link);

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: ReleaseLinkSource
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftReleaseLinkSource(CMDraft *somSelf, Environment *ev,
                ODLinkSource* linkSource)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ReleaseLinkSource");


   try
   {
        somSelf->ReleasePersistentObject(ev, linkSource);

   }
   catch (ODException _exception)
   {
      ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: RemoveLink
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRemoveLink(CMDraft *somSelf, Environment *ev,
                ODLink* link)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemoveLink");

   try
   {

        somSelf->FailIfNotExclusiveWrite(ev);

        somSelf->RemovePersistentObject(ev, link);

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: GetPersistentObjectID
//------------------------------------------------------------------------------

SOM_Scope ODPersistentObjectID  SOMLINK CMDraftGetPersistentObjectID(CMDraft *somSelf, Environment *ev,
                ODPersistentObject* object,
                ODObjectType    objectType)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetPersistentObjectID");

        ODPersistentObjectID retID = kODNULLID;

   try
   {
        ASSERT(ODISOStrEqual(objectType, kODPartObject) || ODISOStrEqual(objectType, kODFrameObject),
                        kODErrInvalidObjectType);

        CMStorageUnit* su = (CMStorageUnit*) object->GetStorageUnit(ev);

        if (su == kODNULL)
                THROW(kODErrInvalidPersistentObject);

        retID = su->GetObjectID(ev);
    }
    catch (ODException _exception)
    {
         ODSetSOMException (ev, _exception);
    }
        return retID;
}

//------------------------------------------------------------------------------
// CMDraft: AcquirePersistentObject
//------------------------------------------------------------------------------

SOM_Scope ODPersistentObject* SOMLINK CMDraftAcquirePersistentObject(CMDraft *somSelf, Environment *ev,
                ODPersistentObjectID objectID,
                ODObjectType*   objectType)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AcquirePersistentObject");

        ODPersistentObject*     object = kODNULL;

    try
    {

        *objectType = kODNULL;
        ODStorageUnitID id = kODNULLID;

        if (objectID == 0)
                THROW(kODErrIllegalNullIDInput);

        CMContainer cmContainer = somSelf->GetCMContainer(ev);
        ODSessionMustHaveCMAllocReserve(cmContainer);

        CMObject cmObject = CMGetObject(cmContainer, objectID);
        if (cmObject == kODNULL)
                THROW(kODErrInvalidPersistentObjectID);

        if (_fIDList->ObjectExists(cmObject) != kODFalse) {
                id = _fIDList->GetID(cmObject);
                CMReleaseObject(cmObject);
        }
        else
                id = _fIDList->Add(cmObject);

        ODSessionRestoreCMAllocReserve(cmContainer);

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *su = somSelf->AcquireStorageUnit(ev, id);
#else
        TempODStorageUnit su = somSelf->AcquireStorageUnit(ev, id);
#endif
        ODPtr buffer;
        if ((buffer = ODGetISOStrProp(ev, su, kODPropStorageUnitType, kODISOStr, kODNULL, kODNULL)) != kODNULL)
        {
                if (ODISOStrEqual((ODISOStr) buffer, kODPartObject) != kODFalse)
                        object = somSelf->AcquirePart(ev, id);
                else if (ODISOStrEqual((ODISOStr) buffer, kODFrameObject) != kODFalse)
                        object = somSelf->AcquireFrame(ev, id);
                else
                        THROW(kODErrInvalidPersistentObjectID);

                if (object != kODNULL)
                        *objectType = (ODType) buffer;
                else
                        ODDisposePtr(buffer);
        }

#ifdef _NO_TMP_OBJS_
        su->Release(ev);
#endif

     }
     catch (ODException _exception)
     {
           object = kODNULL;
           ODSetSOMException(ev, _exception);
     }
        return object;
}

//------------------------------------------------------------------------------
// CMDraft: ReleaseStorageUnit
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMDraftReleaseStorageUnit(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ReleaseStorageUnit");


    try
    {
        ODStorageUnit*  su = kODNULL;

        if (! _fStorageUnits->GetValue(&id, &su))
                THROW(kODErrInvalidStorageUnit);

        if (su->GetRefCount(ev) != 0)
                THROW(kODErrRefCountGreaterThanZero);

        if (_fDraftProperties != kODNULL) {
                if (id == _fDraftProperties->GetID(ev))
                        _fDraftProperties = kODNULL;
        }

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
        return somSelf;
}

//------------------------------------------------------------------------------
// CMDraft: ~CMDraft
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftsomUninit(CMDraft *somSelf)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","somUninit");

                Environment*    ev = somGetGlobalEnvironment();
     try
     {

#if ODDebug_DebugRefCount
                if (somSelf->GetRefCount(ev) != 0)
                        DebugStr("\pRefCount of Draft is not 0 at uninit.");
                somPrintf("~CMDraft %x RefCount %d EmbeddedCtr %x CMCtr %x\n",
                                        somSelf,
                                        somSelf->GetRefCount(ev),
                                        somSelf->GetEmbeddedContainer(ev),
                                        somSelf->GetCMContainer(ev));
#endif


#ifdef _PLATFORM_OS2_
       //XXXXX Added to force LinkTarget to unregister
       if (_fPermissions == kODDPExclusiveWrite)
       //XXXXX
       somSelf->DeleteCollections(ev);
       parent_somUninit(somSelf);
#else
                delete _fPartClasses;

                somSelf->DeleteCollections(ev);

#endif

/*
#ifdef _PLATFORM_OS2_

    if (_fLinkService != kODNULL)
    (
      delete _fLinkService;
      delete _fLinkServiceXID;
    }

#endif
*/

        }
        catch (ODException _exception)
        {
            SetErrorCode(kODNoError); // Ignore exception
        }
}

//------------------------------------------------------------------------------
// CMDraft: InitDraft
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftInitDraft(CMDraft *somSelf, Environment *ev,
                ODDocument* document, ODDraftID id, ODDraftPermissions perms)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","InitDraft");

   try
   {

        /* Moved from somInit. SOM itself sets fields to zero
        _fChangedFromPrev = kODFalse;

        _fDocument = kODNULL;
        _fID = 0;
        _fPermissions = kODDPNone;
        _fEmbeddedContainer = kODNULL;
        _fVersionID = kODTombstonedVersion;
        _fIsNewDraft = kODFalse;

        _fExternalized = kODFalse;
        _fRemoveChangeOnAbort = kODFalse;

        _fStorageUnits = kODNULL;
        _fPersistentObjects = kODNULL;

        _fIDList = kODNULL;
        _fDraftProperties = kODNULL;

        _fDestDraft = kODNULL;
        _fDestFrame = kODNULL;
        _fClonedSUIDs = kODNULL;
        _fWeakClonedSUIDs = kODNULL;
        _fSavedWeakClonedSUIDs = kODNULL;
        _fCurrentKey = kODNULL;
        _fLockCount = 0;

        _fAnyFrameCloned = kODFalse;
        _fRootPartReused = kODFalse;

        _fOrigTopVersionDraftID = 0;

        _fHeap = kDefaultHeapID;
        */
        _fVersionID = kODTombstonedVersion;

        somSelf->InitRefCntObject(ev);

#ifdef ODDebug_VersionList
        ((CMDocument*) document)->GetVersionList(ev)->Print(">>>Entering InitDraft");
#endif

        _fDocument = (CMDocument*) document;
        _fID = id;
        _fPermissions = perms;

        VersionList*    versionList = kODNULL;

        if (_fDocument == kODNULL)
                THROW(kODErrIllegalNullDocumentInput);

        if (_fID == kODNULL) {
                versionList = _fDocument->GetVersionList(ev);
                ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

                _fID = versionList->CreateDraft();
                _fIsNewDraft = kODTrue;
                _fRemoveChangeOnAbort = kODTrue;

#if !TestFlushContainer
                _fDocument->ExternalizeVersionList(ev, kODFalse);
#endif
        }

        somSelf->CreateCollections(ev);

#if lazyOpen
        _fExternalized = kODFalse;
#else
        if (_fPermissions == kODDPReadOnly)
                somSelf->OpenVersion(ev);
        else
                somSelf->CreateVersion(ev);
#endif

        _fHeap = _fDocument->GetHeap(ev);

#ifdef _PLATFORM_OS2_

    _fLinkService = _fDocument->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev)->GetLinkService(ev);

#endif


#ifdef ODDebug_Unloading_Classes
        SetOutputMode(kWriteToFile);
#endif

#ifdef _PLATFORM_MACINTOSH_
        _fPartClasses = new(_fHeap)
                OpenHashTable(OpenHashTable::StdEqual,
                              OpenHashTable::StdHash, _fHeap);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//      The use of heap is not implemented in Windows platform
        _fPartClasses = new OpenHashTable(OpenHashTable::StdEqual,
                              OpenHashTable::StdHash, 0);
#endif
        _fPartClasses->Initialize(kODInitialNumEntries,
                                        sizeof(SOMClass*),
                                        sizeof(ODULong));

#ifdef ODDebug_VersionList
        _fDocument->GetVersionList(ev)->Print(">>>Exiting InitDraft");
#endif

#ifdef _PLATFORM_WIN32_
        DocumentManager  *docMgr = NULL;
        OleWrapper       *wrapper = NULL;

        docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);
        if (docMgr) {
            wrapper = docMgr->GetOleWrapper(ev);
            if (wrapper)
                wrapper->OnDataChange();
        }
#endif

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK CMDraftPurge(CMDraft *somSelf, Environment *ev,
                ODSize size)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Purge");

        ODULong         runningTotal = 0;

   try
   {

        OpenHashTableIterator   i(_fPersistentObjects);
        ODStorageUnitID         id;
        ODPersistentObject*     object;

        for (i.First(&id, &object); i.IsNotComplete(); i.Next(&id, &object)) {
                runningTotal += object->Purge(ev, size);
                if (object->GetRefCount(ev) == 0) {
                        object->ReleaseAll(ev);
                }
        }
        for (i.First(&id, &object); i.IsNotComplete(); i.Next(&id, &object)) {
                if (object->GetRefCount(ev) == 0) {
                        i.RemoveCurrent();
                        delete object;
                }
        }
        _fPersistentObjects->ShrinkToFit(/*extraSlots*/ 0);

        // purge all storage units, but don't release CMObjects
        runningTotal += PurgeAllStorageUnits(ev, _fStorageUnits, kODNULL);

        PurgeClasses(ev, _fPartClasses);

    }
    catch (ODException _exception)
    {
         runningTotal = 0;
         ODSetSOMException(ev, _exception);
    }
        return runningTotal;
}

//------------------------------------------------------------------------------
// CMDraft: CreateSU
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK CMDraftCreateSU(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id, ODType suType)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateSU");

        ODStorageUnit*  su = kODNULL;

    try
    {
        CMObject                object = kODNULL;
        CMContainer             cmContainer = somSelf->GetCMContainer(ev);
        ODSessionMustHaveCMAllocReserve(cmContainer);

        ODVolatile(su);
        ODVolatile(object);
        try
        {

                // Create CMObject if necessary

                if (id == kODNULL) {
                        if ((object = CMNewObject(cmContainer)) == kODNULL)
                                THROW(kODErrBentoCannotNewObject);

                        id = _fIDList->Add(object);
                }

                // Create the Storage Unit
                su = NewCMStorageUnit(somSelf->GetHeap(ev));
                su->InitStorageUnit(ev, somSelf, id);

        }
        catch (ODException _exception)
        {

                if (object != kODNULL)
                        CMReleaseObject(object);

                if (su != kODNULL)
                        delete su;

                if (ErrorCode() == kODErrBentoInvalidObject)
                        THROW(kODErrInvalidID);
                else
                        throw;

        }
        ODSessionRestoreCMAllocReserve(cmContainer);

        // Add Storage Unit to outstanding SU collection

        _fStorageUnits->ReplaceEntry(&id, &su);

        }
        catch (ODException _exception)
        {
            su = kODNULL;
            ODSetSOMException(ev, _exception);
        }

        return su;
}

//------------------------------------------------------------------------------
// CMDraft: SetChangedFromPrevFlag
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftSetChangedFromPrevFlag(CMDraft *somSelf, Environment *ev,
                ODBoolean changed)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","SetChangedFromPrevFlag");

        if (changed != kODFalse){
                try
                {
                        somSelf->FailIfNotExclusiveWrite(ev);
                }
                catch(ODException _exception)
                {
                      ODSetSOMException(ev, _exception);
                }
        }
        _fChangedFromPrev = changed;
}

static void SetupForUpdatingDraft(Environment* ev,
                                                                                CMDocument* localDoc,
                                                                                ODVersionID prevVersionID,
                                                                                CMValue version)
{
   try
   {
        ODBentoContainer*       localContainer = (ODBentoContainer*) localDoc->GetContainer(ev);
        ODBentoContainer*       container = kODNULL;

        // check to see whether we need to do a update
        container = localContainer->GetTargetContainer(ev);

        if (container != kODNULL) {

                // get the target document
                CMDocument*             targetDoc = localContainer->GetTargetDocument(ev);

                // find its version list
                VersionList*    versionList = targetDoc->GetVersionList(ev);
                ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

                // get the latest draft
                ODDraftID       latestDraftID = versionList->GetLatestDraftID();

                // get the version id of the latest draft
                prevVersionID = versionList->GetDraft(latestDraftID);
        }
        else {
                container = localContainer;
        }
        // Note: getting/setting refcon's does not allocate memory in Bento:

        // get the name for indirection
        ODType prevVersionName = GetVersionNameFromVersionID(prevVersionID, localDoc->GetHeap(ev));

        // store name so that it can be passed to handler
        CMSetValueRefCon(version, prevVersionName);

        // get cmContainer so that we can assess CMSession
        CMContainer cmContainer = container->GetCMContainer(ev);

        // Save the outermost container for embedded container creation.
        ODSessionRefCon* sessionRefCon = (ODSessionRefCon*) CMGetSessionRefCon(cmContainer);
        sessionRefCon->container = container;
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: CreateVersion
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftCreateVersion(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateVersion");


   try
   {
        CMContainer                             cmContainer = ((ODBentoContainer*) _fDocument->GetContainer(ev))->GetCMContainer(ev);
        CMObject                                versionObject = kODNULL;
        CMType                                  versionNameType = kODNULL;
        ODType                                  versionName = kODNULL;
        CMProperty                              versionNameProperty = kODNULL;
        CMValue                                 versionNameValue = kODNULL;
        CMType                                  versionDataType = kODNULL;
        CMProperty                              versionDataProperty = kODNULL;
        CMValue                                 version = kODNULL;

        VersionList*                    versionList = kODNULL;

        versionList = _fDocument->GetVersionList(ev);
        ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

        _fOrigTopVersionDraftID = versionList->GetSameVersionDraftID(_fID);
        _fPrevVersionID = versionList->GetCurrentVersion(_fID);
        _fVersionID = versionList->CreateVersion(_fID);

        ODSessionMustHaveCMAllocReserve(cmContainer);

        versionObject = CMNewObject(cmContainer);
        versionNameType = CMRegisterType(cmContainer, kODISOStr);

        versionName = GetVersionNameFromVersionID(_fVersionID, somSelf->GetHeap(ev));
        versionNameProperty = CMRegisterProperty(cmContainer, versionName);

        versionNameValue = CMNewValue(versionObject, versionNameProperty, versionNameType);
        CMWriteValueData(versionNameValue, "", 0, 1);

        versionDataType = CMRegisterType(cmContainer, kODEmbeddedContainerType);
        versionDataProperty = CMRegisterProperty(cmContainer, kODEmbeddedContainerProperty);
        version = CMNewValue(versionObject, versionDataProperty, versionDataType);
        if (version == kODNULL)
                THROW(kODErrCannotCreateDraftVersion);
        CMWriteValueData(version, "", 0, 0);                            /* Make the container manager happy */

        ODSessionRestoreCMAllocReserve(cmContainer);

        SetupForUpdatingDraft(ev, _fDocument, _fPrevVersionID, version);

#ifdef ODDebug_CMDraft
        ODDocument* tempDoc = somSelf->GetDocument(ev);
        somPrintf("CreateVersion: CMDocument %x DraftID %d IsNewDraft %d versionID %d prevVersionID %d\n", tempDoc, somSelf->GetID(ev), somSelf->IsNewDraft(ev), _fVersionID, _fPrevVersionID);
#endif

        ODEmbeddedContainerID   containerID;
        containerID.cmValue = version;
        containerID.shouldMerge = (somSelf->IsNewDraft(ev) ? kODFalse : kODTrue);
        ODByteArray*    ba = CreateByteArray(&containerID, sizeof(ODEmbeddedContainerID));
        _fEmbeddedContainer = (ODEmbeddedContainer*)
                        _fDocument->GetContainer(ev)->GetStorageSystem(ev)->CreateContainer(ev, kODBentoEmbeddedContainer, ba);
        DisposeByteArray(ba);

        if (versionName != kODNULL)
                ODDisposePtr(versionName);

        _fExternalized = kODFalse;

#ifdef _PLATFORM_WIN32_
        DocumentManager  *docMgr = NULL;
        OleWrapper       *wrapper = NULL;

        docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);
        if (docMgr) {
            wrapper = docMgr->GetOleWrapper(ev);
            if (wrapper)
                wrapper->OnDataChange();
        }
#endif

        somSelf->OpenCollections(ev);

   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: OpenVersion
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftOpenVersion(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","OpenVersion");


    try
    {
        CMContainer                             cmContainer = ((ODBentoContainer*) _fDocument->GetContainer(ev))->GetCMContainer(ev);
        ODType                                  versionName = kODNULL;
        ODType                                  oldVersionName = kODNULL;
        CMType                                  versionDataType;
        CMProperty                              versionDataProperty;
        CMProperty                              versionNameProperty;
        CMObject                                versionObject;
        CMValue                                 version;

        VersionList*                    versionList;

        versionList = _fDocument->GetVersionList(ev);
        ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

        _fVersionID = versionList->GetCurrentVersion(_fID);
        _fPrevVersionID = _fVersionID;
        if (_fVersionID == kODTombstonedVersion)
                THROW(kODErrDraftHasBeenDeleted);

        if (_fPermissions == kODDPReadOnly) {
                ODSessionMustHaveCMAllocReserve(cmContainer);

                MyDebugStr("**** OpenVersion: kODDPReadOnly.\n");
                versionDataType = CMRegisterType(cmContainer, kODEmbeddedContainerType);
                versionDataProperty = CMRegisterProperty(cmContainer, kODEmbeddedContainerProperty);

                versionName = GetVersionNameFromVersionID(_fVersionID, somSelf->GetHeap(ev));
                versionNameProperty = CMRegisterProperty(cmContainer, versionName);

                versionObject = CMGetNextObjectWithProperty(cmContainer, kODNULL, versionNameProperty);
                version = CMUseValue(versionObject, versionDataProperty, versionDataType);

                CMSetValueRefCon(version, kODNULL);

                // Save the outermost container for embedded container creation.
                ODSessionRefCon* sessionRefCon = (ODSessionRefCon*) CMGetSessionRefCon(cmContainer);
                sessionRefCon->container = _fDocument->GetContainer(ev);

#ifdef ODDebug_CMDraft
        ODDocument* tempDoc = somSelf->GetDocument(ev);
        somPrintf("OpenVersion: CMDocument %x DraftID %d IsNewDraft %d versionID %d prevVersionID %d\n", tempDoc, somSelf->GetID(ev), somSelf->IsNewDraft(ev), _fVersionID, _fPrevVersionID);
#endif
                ODEmbeddedContainerID   containerID;
                containerID.cmValue = version;
                containerID.shouldMerge = kODFalse;
                ODByteArray*    ba = CreateByteArray(&containerID, sizeof(ODEmbeddedContainerID));
                _fEmbeddedContainer = (ODEmbeddedContainer*)
                        _fDocument->GetContainer(ev)->GetStorageSystem(ev)->AcquireContainer(ev, kODBentoEmbeddedContainer, ba);
                DisposeByteArray(ba);

                if (versionName != kODNULL)
                        ODDisposePtr(versionName);
                if (oldVersionName != kODNULL)
                        ODDisposePtr(oldVersionName);

                ODSessionRestoreCMAllocReserve(cmContainer);

                somSelf->OpenCollections(ev);
        }
        else
                THROW(kODErrInvalidPermissions);

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: CloseVersion
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftCloseVersion(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CloseVersion");

   try
   {

#ifdef ODDebug_CMDraft
        ODDocument* tempDoc = somSelf->GetDocument(ev);
        somPrintf("CloseVersion: CMDocument %x DraftID %d versionID %d Permission %d\n", tempDoc, somSelf->GetID(ev), _fVersionID, _fPermissions);
#endif


        if (_fPermissions == kODDPExclusiveWrite) {
                VersionList*    versionList = kODNULL;

                versionList = _fDocument->TestAndGetVersionList(ev);
                ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

                if (versionList->IsBelow(_fOrigTopVersionDraftID, versionList->GetSameVersionDraftID(_fID)))
                        _fEmbeddedContainer->SetMergeFlag(ev, kODFalse);
                somSelf->CloseCollections(ev);
        }
        else {
                somSelf->DeleteCollections(ev);
                somSelf->CreateCollections(ev);
        }

        if (_fEmbeddedContainer != kODNULL) {
                _fEmbeddedContainer->Release(ev);
                _fEmbeddedContainer = kODNULL;
        }

        _fIsNewDraft = kODFalse;

   } 
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: DestroyVersion
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftDestroyVersion(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","DestroyVersion");

#ifdef ODDebug_CMDraft
        ODDocument* tempDoc = somSelf->GetDocument(ev);
        somPrintf("Destroy Version: CMDocument %x DraftID %d versionID %d Permission %d\n", tempDoc, somSelf->GetID(ev), _fVersionID, _fPermissions);
#endif

        if (_fPermissions == kODDPExclusiveWrite) {

           try
           {
                somSelf->DeleteCollections(ev);
                somSelf->CreateCollections(ev);

                if (_fEmbeddedContainer != kODNULL) {
                        _fEmbeddedContainer->Abort(ev);
                        CMObject                parentObject = kODNULL;
                        ODByteArray     ba = _fEmbeddedContainer->GetID(ev);
                        CMValue parentValue = *((CMValue*) ba._buffer);
                        ODDisposePtr(ba._buffer);

                        CMContainer cmContainer = somSelf->GetCMContainer(ev);
                        ODSessionMustHaveCMAllocReserve(cmContainer);

                        CMGetValueInfo(parentValue, kODNULL, &parentObject,
                                                        kODNULL, kODNULL, kODNULL);
                        CMDeleteValue(parentValue);
                        CMDeleteObject(parentObject);

                        ODSessionRestoreCMAllocReserve(cmContainer);

                        _fEmbeddedContainer->Release(ev);
                        _fEmbeddedContainer = kODNULL;
                }
            } 
            catch (ODException _exception)
            {
                ODSetSOMException(ev, _exception);
            }
           

        }
}

//------------------------------------------------------------------------------
// CMDraft: FlushVersion
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftFlushVersion(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","FlushVersion");

}

//------------------------------------------------------------------------------
// CMDraft: Reinitialize
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftReinitialize(CMDraft *somSelf, Environment *ev,
                ODDraftPermissions perms)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Reinitialize");


   try
   {
        if ((perms == kODDPExclusiveWrite) &&
                (_fEmbeddedContainer != kODNULL) &&
                (_fEmbeddedContainer->GetUseMode(ev) & kCMReading)) {
                MyDebugStr("Reinitialize.\n");
                THROW(kODErrInvalidPermissions);
        }

        // Close this version first

        // somSelf->CloseVersion(ev);

        // Set the permissions

        _fPermissions = perms;

        // Open or create a new version

        if (_fEmbeddedContainer == kODNULL) {
#if lazyOpen
                _fExternalized = kODFalse;
#else
                if (_fPermissions == kODDPReadOnly)
                        somSelf->OpenVersion(ev);
                else
                        somSelf->CreateVersion(ev);
#endif
        }

        //_fDocument->Reopen(ev);

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: Open
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftOpen(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Open");

     try
     {

        if (_fEmbeddedContainer == kODNULL) {
#if lazyOpen
                _fExternalized = kODFalse;
#else
                if (_fPermissions == kODDPReadOnly)
                        somSelf->OpenVersion(ev);
                else
                        somSelf->CreateVersion(ev);
#endif
        }

     }
     catch (ODException _exception)
     {
         ODSetSOMException(ev, _exception);
     }
}

#pragma segment CMDraft2

//------------------------------------------------------------------------------
// CMDraft: Close
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftClose(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Close");


    try
    {
#if ODDebug_Drafts
        somPrintf("CMDraftClose: %d\n", somSelf->GetID(ev));
#endif

#if lazyOpen
        if (_fEmbeddedContainer != kODNULL) {

                CMContainer cmContainer = somSelf->GetCMContainer(ev);
                ODSessionMustHaveCMAllocReserve(cmContainer);
                // AcquireDraftPropertiesObject() makes CM calls:

                CMObject draftPropertiesObject = AcquireDraftPropertiesObject(somSelf->GetCMContainer(ev));
                if (draftPropertiesObject == kODNULL)
                        THROW(kODErrNoDraftProperties);

                CMKeepObject(draftPropertiesObject);

                ODSessionRestoreCMAllocReserve(cmContainer);

                somSelf->CloseVersion(ev);
        }
#else
        somSelf->CloseVersion(ev);
#endif

     }
     catch (ODException _exception)
     {
         ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMDraft: Abort
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftAbort(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Abort");


     try
     {
        if (_fPermissions == kODDPReadOnly)
                THROW(kODErrInvalidPermissions);

        if (_fDraftProperties != kODNULL) {
                _fDraftProperties->Release(ev);
                _fDraftProperties = kODNULL;
        }

#if TestFlushContainer
        if (_fRemoveChangeOnAbort) {
                VersionList*    versionList = kODNULL;

                versionList = _fDocument->TestAndGetVersionList(ev);
                ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);

                try
                {
                        if (versionList->Exists(_fID) != kODFalse)
                                versionList->RemoveChanges(_fID);

                }
                catch (ODException _exception)
                {
     		    ODError error = ErrorCode();
                    SaveEv();
      		    try
                    {
                        _fDocument->ReleaseVersionList(ev);
                    }
                    catch(ODException _exception)
                    {
                         SetErrorCode(kODNoError);
                    }
      		    RestoreEv();
      		    ODSetSOMException(ev, error);

                        throw;

                }
        }
#else
        _fDocument->InternalizeVersionList(ev);
#endif
        somSelf->DestroyVersion(ev);
        //      _fDocument->Reopen(ev);

     }
     catch (ODException _exception)
     {
          ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMDraft: Flush
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftFlush(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Flush");

        if (_fPermissions == kODDPExclusiveWrite) {
                try
                {
                        somSelf->FlushVersion(ev);
                }
                catch (ODException _exception)
                {
                    ODSetSOMException(ev, _exception);
                }
        }
}


//------------------------------------------------------------------------------
// CMDraft: IsNewDraft
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftIsNewDraft(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","IsNewDraft");

        return _fIsNewDraft;
}

//------------------------------------------------------------------------------
// CMDraft: GetEmbeddedContainer
//------------------------------------------------------------------------------

SOM_Scope ODEmbeddedContainer*  SOMLINK CMDraftGetEmbeddedContainer(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetEmbeddedContainer");

#if lazyOpen
      try
      {
        if (_fEmbeddedContainer == kODNULL) {
                if (_fPermissions == kODDPReadOnly)
                        somSelf->OpenVersion(ev);
                else
                        somSelf->CreateVersion(ev);
        }
      }
      catch (ODException _exception)
      {
          ODSetSOMException(ev, _exception);
      }
#endif

        return _fEmbeddedContainer;
}

//------------------------------------------------------------------------------
// CMDraft: GetCMContainer
//------------------------------------------------------------------------------

SOM_Scope CMContainer  SOMLINK CMDraftGetCMContainer(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetCMContainer");

          CMContainer container = kODNULL;
     try
     {

#if lazyOpen
        if (_fEmbeddedContainer == kODNULL) {
                if (_fPermissions == kODDPReadOnly)
                        somSelf->OpenVersion(ev);
                else
                        somSelf->CreateVersion(ev);
        }
#endif

        if (_fEmbeddedContainer == kODNULL)
                THROW(kODErrCannotGetDraftVersion);
        container =  _fEmbeddedContainer->GetCMContainer(ev);

     }
     catch (ODException _exception)
     {
         ODSetSOMException(ev, _exception);
     }
        return container;
}

//------------------------------------------------------------------------------
// AcquireDraftPropertiesObject
//------------------------------------------------------------------------------

// Callers must ODSessionMustHaveCMAllocReserve() first:

static CMObject AcquireDraftPropertiesObject(CMContainer container)
{
        CMObject                draftPropertiesObject = kODNULL;
        Environment* ev = somGetGlobalEnvironment();
   try
   {
        CMType                  rootSUType;
        CMProperty              rootSUProp;
        CMValue                 rootSU;

        CMContainerModeFlags    openMode;

        if ((rootSUType = CMRegisterType(container, kODStorageUnitType)) == kODNULL)
                THROW(kODErrBentoInvalidType);
        if ((rootSUProp = CMRegisterProperty(container, kODPropRootSU)) == kODNULL)
                THROW(kODErrBentoInvalidProperty);
        draftPropertiesObject = CMGetNextObjectWithProperty(container, kODNULL, rootSUProp);

        if (draftPropertiesObject == kODNULL) {

                CMGetContainerInfo(container, kODNULL, kODNULL, kODNULL, kODNULL, &openMode);
                if (openMode == kCMReading)
                        return kODNULL;

                draftPropertiesObject = CMNewObject(container);

                if ((rootSU = CMNewValue(draftPropertiesObject, rootSUProp, rootSUType)) == kODNULL)
                        THROW(kODErrBentoCannotNewValue);
                CMWriteValueData(rootSU, "", 0, 0);
        }
    }
    catch (ODException _exception)
    {
       draftPropertiesObject = kODNULL;
       ODSetSOMException(ev, _exception);
    }

        return draftPropertiesObject;
}

//------------------------------------------------------------------------------
// GetVersionNameFromVersionID
//------------------------------------------------------------------------------

static ODType GetVersionNameFromVersionID(ODVersionID id, ODMemoryHeapID heapID)
{
        ODSByte*        versionName = kODNULL;
        Environment* ev = somGetGlobalEnvironment();
   try
   {
        ODSByte cString[kMaxStringSize];

        if (id != 0) {
                CM_itoa(id, cString);   // KP
                versionName = (ODSByte*) ODNewPtr(strlen(kODVersionNamePrefix) + strlen(cString) + 1, heapID);
                strcpy(versionName, kODVersionNamePrefix);
                strcat(versionName, cString);
        }
   }
   catch (ODException _exception)
   {
       versionName = kODNULL;
       ODSetSOMException(ev, _exception);
   }    
        return versionName;
}

//------------------------------------------------------------------------------
// CMDraft: RetrievePersistentObject
//------------------------------------------------------------------------------

SOM_Scope ODPersistentObject*  SOMLINK CMDraftRetrievePersistentObject(CMDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RetrievePersistentObject");

        ODPersistentObject*     object = kODNULL;


    try
    {
        if (_fPersistentObjects->GetValue(&id, &object))
                object->Acquire(ev);

    }
    catch (ODException _exception)
    {
        object = kODNULL;
        ODSetSOMException(ev, _exception);
    }
        return object;
}

//------------------------------------------------------------------------------
// CMDraft: ReleasePersistentObject
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftReleasePersistentObject(CMDraft *somSelf, Environment *ev,
                ODPersistentObject* object)
{
//  CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ReleasePersistentObject");

   try
   {
        if (object->GetRefCount(ev) != 0)
                THROW(kODErrRefCountGreaterThanZero);
   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }

}

//------------------------------------------------------------------------------
// CMDraft: RemovePersistentObject
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftRemovePersistentObject(CMDraft *somSelf, Environment *ev,
                ODPersistentObject* object)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","RemovePersistentObject");

    try
    {
        if (!object)
                THROW(kODErrInvalidPersistentObject);

        ODStorageUnit*          su = object->GetStorageUnit(ev);
        ODStorageUnitID         id = object->GetID(ev);
        CMObject                        cmObject = kODNULL;

        if (id == kODNULLID)
                THROW(kODErrInvalidPersistentObjectID);

        object->Release(ev);

        if (object->GetRefCount(ev) != 0)
                THROW(kODErrRefCountGreaterThanZero);

        if ((su != kODNULL) && (su->GetRefCount(ev) != 1))
                THROW(kODErrRefCountNotEqualOne);

        object->ReleaseAll(ev);

        _fPersistentObjects->RemoveEntry(&id);
        delete object;

        _fStorageUnits->RemoveEntry(&id);
        delete su;

        if (_fIDList->Exists(id) != kODFalse) {
                cmObject = (CMObject) _fIDList->Get(id);
                _fIDList->Remove(id);

                CMContainer cmContainer = somSelf->GetCMContainer(ev);
                ODSessionMustHaveCMAllocReserve(cmContainer);

                if (cmObject != kODNULL)
                        CMDeleteObject(cmObject);

                ODSessionRestoreCMAllocReserve(cmContainer);
        }

        somSelf->SetChangedFromPrevFlag(ev, kODTrue);

     }
     catch (ODException _exception)
     {
         ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMDraft: CreateCollections
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftCreateCollections(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateCollections");

    try
    {
        ODMemoryHeapID heap = somSelf->GetHeap(ev);

#ifdef _PLATFORM_MACINTOSH_
        _fPersistentObjects = new(heap)
                OpenHashTable(OpenHashTable::StdEqual,
                              OpenHashTable::StdHash, heap);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//      The use of heap is not implemented in Windows platform
        _fPersistentObjects = new OpenHashTable(OpenHashTable::StdEqual,
                              OpenHashTable::StdHash, 0);
#endif
        _fPersistentObjects->Initialize(kODInitialNumEntries,
                                        sizeof(ODStorageUnitID),
                                        sizeof(ODPersistentObject*));

#ifdef _PLATFORM_MACINTOSH_
        _fStorageUnits = new(heap)
                OpenHashTable(OpenHashTable::StdEqual,
                              OpenHashTable::StdHash, heap);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//      The use of heap is not implemented in Windows platform
        _fStorageUnits = new OpenHashTable(OpenHashTable::StdEqual,
                              OpenHashTable::StdHash, 0);
#endif
        _fStorageUnits->Initialize(kODInitialNumEntries,
                                   sizeof(ODStorageUnitID),
                                   sizeof(ODStorageUnit*));

#ifdef _PLATFORM_MACINTOSH_
        _fIDList = new(somSelf->GetHeap(ev)) IDList;
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//      The use of heap is not implemented in Windows platform
        _fIDList = new IDList;
#endif
        _fIDList->Initialize();

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: DeleteCollections
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftDeleteCollections(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","DeleteCollections");

   try
   {
        if (_fDraftProperties != kODNULL)
        {
                _fDraftProperties->Release(ev);
                _fDraftProperties = kODNULL;
        }
        if (_fPersistentObjects != kODNULL) {
                ODStorageUnitID         id;
                ODPersistentObject*     object;
                OpenHashTableIterator   iter(_fPersistentObjects);

                for (iter.First(&id, &object); iter.IsNotComplete(); iter.Next(&id, &object)) {
                        if (object != kODNULL) {
                                try
                                {
                                        object->ReleaseAll(ev);
                                }
                                catch (ODException _exception)
                                {
                                        WARNMSG(WARN_INDEX(AMSG_620),
                                        "Exception thrown by object %x ID %x error %d\n",
                                        object, object->GetID(ev), ErrorCode());
                                }
                        }
                }

                for (iter.First(&id, &object); iter.IsNotComplete(); iter.Next(&id, &object)) {
                        if (object != kODNULL) {
                                delete object;
                        }
                }
                delete _fPersistentObjects;
                _fPersistentObjects = kODNULL;
        }

        if (_fStorageUnits != kODNULL) {
                ODStorageUnitID         id;
                ODStorageUnit*                  su;
                OpenHashTableIterator   iter(_fStorageUnits);
                for (iter.First(&id, &su); iter.IsNotComplete(); iter.Next(&id, &su)) {
                        if (su != kODNULL) {
                                delete su;
                        }
                }
                delete _fStorageUnits;
                _fStorageUnits = kODNULL;
                _fDraftProperties = kODNULL;
        }

        ODDeleteObject(_fIDList);

   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: ExternalizeCollections
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftExternalizeCollections(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ExternalizeCollections");
         
           ODBoolean acquired = kODFalse;
    try
    {
        ODID                                    id;
        ODStorageUnit*                  su;

        OpenHashTableIterator   persistentObjects(_fPersistentObjects);
        ODPersistentObject*     object;

        for (persistentObjects.First(&id, &object);
                        persistentObjects.IsNotComplete();
                        persistentObjects.Next(&id, &object)) {
           try
           {
                // Temporarily bump the refcount to make sure that the object is
                // in a valid state.
                object->Acquire(ev);
                acquired = kODTrue;
                su = object->GetStorageUnit(ev);
                if ((su != kODNULL) && (su->Exists(ev, kODNULL, kODNULL, 0) != kODFalse))
                object->Externalize(ev);
                object->Release(ev);
            }
            catch (ODException _exception)
            {
              ODError error = ErrorCode();
              SaveEv();
              try
              {
                object->Release(ev);
              }
              catch(ODException _exception)
              {
                  SetErrorCode(kODNoError);
              }
              RestoreEv();
              ODSetSOMException(ev, error);
              throw;
            }
        }

        OpenHashTable                   suCollection(*_fStorageUnits);
        suCollection.InitAndCopyFrom(*_fStorageUnits);
        OpenHashTableIterator   storageUnits(&suCollection);

        for (storageUnits.First(&id, &su);
                        storageUnits.IsNotComplete();
                        storageUnits.Next(&id, &su)) {
                // Temporarily bump the refcount to make sure that the object is
                // in a valid state.
                su->Acquire(ev);
#ifdef _NO_TMP_OBJS_
                ODStorageUnit *tempSU = su; // ensure it's released
#else
                TempODStorageUnit tempSU = su; // ensure it's released
#endif
                if (su->Exists(ev, kODNULL, kODNULL, 0) != kODFalse)
                        su->Externalize(ev);
#ifdef _NO_TMP_OBJS_
                tempSU->Release(ev);
#endif
        }

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: CloseCollections
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftCloseCollections(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CloseCollections");


    try
    {
        somSelf->FailIfNotExclusiveWrite(ev);

        somSelf->Purge(ev, 0);
        PurgeAllStorageUnits(ev, _fStorageUnits, _fIDList); // purge SU, relase CMObjects

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    } 
 
}

//------------------------------------------------------------------------------
// CMDraft: OpenCollections
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftOpenCollections(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","OpenCollections");


   try
   {
        OpenHashTableIterator   storageUnits(_fStorageUnits);

        ODID                                    id;
        ODStorageUnit*                  su;

        for (storageUnits.First(&id, &su);
                        storageUnits.IsNotComplete();
                        storageUnits.Next(&id, &su)) {
                su->Internalize(ev);
        }
   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }

}

//------------------------------------------------------------------------------
// CMDraft: AreEmptyCollections
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftAreEmptyCollections(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AreEmptyCollections");

           ODBoolean areEmptyCollections = kODTrue;
   try
   {
        OpenHashTableIterator   storageUnits(_fStorageUnits);
        OpenHashTableIterator   persistentObjects(_fPersistentObjects);
        ODID                                    id;
        ODPersistentObject*     object;
        ODStorageUnit*                  su;

        for (persistentObjects.First(&id, &object);
                        persistentObjects.IsNotComplete();
                        persistentObjects.Next(&id, &object)) {
                if (object->GetRefCount(ev) > 0)
                        return kODFalse;
        }

        for (storageUnits.First(&id, &su); storageUnits.IsNotComplete(); storageUnits.Next(&id, &su)) {
                if (su->GetRefCount(ev) > 0)
                        return kODFalse;
        }

    }
    catch (ODException _exception)
    {
         areEmptyCollections = kODFalse;
         ODSetSOMException(ev, _exception);
    }

        return areEmptyCollections;
}

//------------------------------------------------------------------------------
// CMDraft: NeedExternalizing
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftNeedExternalizing(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","NeedExternalizing");

        return _fExternalized;
}


//------------------------------------------------------------------------------
// CMDraft: GetIDList
//------------------------------------------------------------------------------

SOM_Scope IDList*  SOMLINK CMDraftGetIDList(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetIDList");

        return _fIDList;
}

//------------------------------------------------------------------------------
// CMDraft: IsChangedFromPrev
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftIsChangedFromPrev(CMDraft *somSelf, Environment *ev,
                VersionList* versionList)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","IsChangedFromPrev");

        ODBoolean               changedFromPrev = kODFalse;
        ODVersionID     prevVersionID;
        ODDraftID               prevDraftID;

        // If this draft has been modified, return kODTrue.

        if (_fChangedFromPrev != kODFalse)
                return kODTrue;

        // versionList->ChangedFromPrev is accurate only when the draft is
        // opened read-only. Therefore, additional check is needed to ensure that
        // we don't return kODTrue even when the draft has not be changed.

        try
        {
                changedFromPrev = versionList->ChangedFromPrev(_fID);

                if ((changedFromPrev != kODFalse) && (_fPermissions == kODDPExclusiveWrite)) {
                        prevDraftID = versionList->GetPreviousDraftID(_fID);
                        prevVersionID = versionList->GetDraft(prevDraftID);
                        if (prevVersionID == _fPrevVersionID)
                                changedFromPrev = kODFalse;
                }
        }
        catch (ODException _exception)
        {
            changedFromPrev = kODFalse;
            ODSetSOMException(ev, _exception);
        }

        return changedFromPrev;
}

//------------------------------------------------------------------------------
// CMDraft: BeginClone
//------------------------------------------------------------------------------

SOM_Scope ODDraftKey  SOMLINK CMDraftBeginClone(CMDraft *somSelf, Environment *ev,
                ODDraft* destDraft,
                ODFrame* destFrame,
                ODCloneKind kind)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","BeginClone");

        OpenHashTable* clonedSUIDs = kODNULL;           ODVolatile(clonedSUIDs);
        OpenHashTable* weakClonedSUIDs = kODNULL;       ODVolatile(weakClonedSUIDs);

        ODDraftKey result = 0;

        try
        {

#ifdef _PLATFORM_OS2_          // $$$ dcp # 120111 05/09/95
        if(destDraft==kODNULL)
           THROW(kODErrInvalidParameter);
#endif

                // Allow beginning a new clone into a link if a clone from a link
                // is in progress; common if the link contains promises
                if ( (kind == kODCloneToLink) && (_fCloneKind == kODCloneFromLink) )
                {
                        if ( _fLockCount > 1 )
                                THROW(kODErrCloningInProgress);
                }
                else if (_fLockCount > 0)
                        THROW(kODErrCloningInProgress);

#ifdef DebugClone
                somPrintf("\nCMDraft::BeginClone - ");
                switch (kind)
                {
                case kODCloneCut:               somPrintf("Cut\n"); break;
                case kODCloneCopy:              somPrintf("Copy\n"); break;
                case kODClonePaste:             somPrintf("Paste\n"); break;
                case kODCloneDropCopy:  somPrintf("Drop Copy\n"); break;
                case kODCloneDropMove:  somPrintf("Drop Move\n"); break;
                case kODCloneToLink:    somPrintf("To Link\n"); break;
                case kODCloneFromLink:  somPrintf("From Link\n"); break;
                case kODCloneAll:               somPrintf("All\n"); break;
                case kODCloneToFile:    somPrintf("To File\n"); break;
                default:                                somPrintf("Invalid clone kind!\n"); break;
                }
#endif

                // Validate the clone kind parameter
                switch (kind)
                {
                case kODClonePaste:
                case kODCloneDropCopy:
                case kODCloneDropMove:
                case kODCloneToFile:
                        {
                        ODCloneKind origCloneKind = GetOriginalCloneKind(ev, somSelf);
                        if ( (origCloneKind != kODCloneCut) && (origCloneKind != kODCloneCopy) )
                                THROW(kODErrInconsistentCloneKind);
                        }
                        if ( somSelf == destDraft )
                                THROW(kODErrInvalidDestinationDraft);
                        break;
                case kODCloneCut:
                case kODCloneCopy:
                #ifdef _PLATFORM_OS2_
                #else
                        if ( somSelf == destDraft )
                                THROW(kODErrInvalidDestinationDraft);
                #endif
                        break;

                #ifdef _PLATFORM_OS2_
                case kODCloneToLink:
                  if ( somSelf != destDraft )
                     THROW(kODErrInvalidDestinationDraft);
                  break;

                case kODCloneAll:
                case kODCloneFromLink:
                break;
                #else

                case kODCloneToLink:
                case kODCloneFromLink:
                        if ( somSelf != destDraft )
                                THROW(kODErrInvalidDestinationDraft);
                        break;

                case kODCloneAll:
                        break;
                #endif

                default:
                        THROW(kODErrInvalidCloneKind);
                        break;
                }

                ODMemoryHeapID heap = somSelf->GetHeap(ev);

                // Ensure both hash tables can be allocated
                if ( _fLockCount == 0 )
                {
#ifdef _PLATFORM_MACINTOSH_
                        clonedSUIDs = new(heap)
                                OpenHashTable(OpenHashTable::StdEqual,
                                              OpenHashTable::StdHash, heap);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//                      The use of heap is not implemented in Windows platform
                        clonedSUIDs = new OpenHashTable(OpenHashTable::StdEqual,
                                              OpenHashTable::StdHash, 0);
#endif
                        clonedSUIDs->Initialize(kInitialHashTableEntries,
                                                  sizeof(ODStorageUnitID),
                                                  sizeof(ODStorageUnitID));

                        if ( (kind == kODCloneCut) || (kind == kODCloneCopy) )
                                ReadClonedObjectTable(ev, clonedSUIDs, destDraft);
                }

#ifdef _PLATFORM_MACINTOSH_
                weakClonedSUIDs = new(heap)
                        OpenHashTable(OpenHashTable::StdEqual,
                                      OpenHashTable::StdHash, heap);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//              The use of heap is not implemented in Windows platform
                weakClonedSUIDs = new OpenHashTable(OpenHashTable::StdEqual,
                                      OpenHashTable::StdHash, 0);
#endif
                weakClonedSUIDs->Initialize(kInitialHashTableEntries,
                                              sizeof(ODStorageUnitID),
                                              sizeof(ODStorageUnitID));

                // BeginClone must not fail beyond this point

                _fLockCount++;

                if ( _fLockCount == 1 )
                {
                        _fCurrentKey++;
                        _fCloneKind = kind;
                        _fDestDraft = destDraft;
                        _fDestFrame = destFrame;

                        _fAnyFrameCloned = kODFalse;
                        _fRootPartReused = kODFalse;

                        _fClonedSUIDs = clonedSUIDs;
                }

                _fSavedWeakClonedSUIDs = _fWeakClonedSUIDs;
                _fWeakClonedSUIDs = weakClonedSUIDs;

                result = _fCurrentKey;

        }
        catch (ODException _exception)
        {

             result = 0;
             ODSetSOMException(ev, _exception);
             ODDeleteObject(clonedSUIDs);
             ODDeleteObject(weakClonedSUIDs);

        }

        return result;
}

//------------------------------------------------------------------------------
// CMDraft: GetClonedSUIDs
//------------------------------------------------------------------------------

SOM_Scope OpenHashTable*  SOMLINK CMDraftGetClonedSUIDs(CMDraft *somSelf, Environment *ev,
                ODDraft* destDraft)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetClonedSUIDs");
    try
    {

        if ((_fDestDraft != kODNULL) && (_fDestDraft != destDraft)) {
                ODDeleteObject(_fClonedSUIDs);
                ODDeleteObject(_fWeakClonedSUIDs);
                THROW(kODErrInvalidDestinationDraft);
        }
        _fDestDraft = destDraft;
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
        return kODNULL;
    }

        return _fClonedSUIDs;
}

//------------------------------------------------------------------------------
// CMDraft: EndClone
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftEndClone(CMDraft *somSelf, Environment *ev,
                ODDraftKey key)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","EndClone");

        ODStorageUnitID         fromID;
        ODStorageUnitID         toID;

    try
    {

        if ((key != _fCurrentKey) || (_fLockCount == 0))
                THROW(kODErrInvalidDraftKey);

        OpenHashTableIterator iter(_fWeakClonedSUIDs);
        for (iter.First(&fromID, &toID); iter.IsNotComplete(); iter.Next(&fromID, &toID)) {
                if (toID != 0) {
                        ODStorageUnit* toSU = _fDestDraft->AcquireStorageUnit(ev, toID);
#ifdef DebugClone
                        somPrintf("Removing %d %d\n", toID, ((CMStorageUnit*) toSU)->GetObjectID(ev));
#endif
                        _fDestDraft->RemoveStorageUnit(ev, toSU);
                }
        }

        switch (_fCloneKind)
        {
        case kODCloneCopy:
        case kODCloneCut:
                SetOriginalDraft(ev, _fDestDraft, somSelf);
                if ( !OriginalCloneKindExists(ev, _fDestDraft) )
                        SetOriginalCloneKind(ev, _fDestDraft, _fCloneKind);
                WriteClonedObjectTable(ev, _fClonedSUIDs, _fDestDraft);
                break;

        case kODClonePaste:
        case kODCloneDropCopy:
        case kODCloneDropMove:
        case kODCloneToFile:
                if ( (_fCloneKind == kODClonePaste) && (GetOriginalCloneKind(ev, somSelf) == kODCloneCut) )
                        SetOriginalCloneKind(ev, somSelf, kODCloneCopy);
                break;

        default:
                break;
        }

        if ( somSelf->GetCloneKind(ev) == kODCloneDropMove )
        {
                if ( GetOriginalDraft(ev, somSelf) == _fDestDraft )
                {
                        if ( _fAnyFrameCloned )
                        {
                                if ( somSelf->ContainingPartInClone(ev, _fDestFrame) )
                                        THROW(kODErrMoveIntoSelf);
                        }
                }
        }

        --_fLockCount;

        if ( _fLockCount == 0 )
        {
                _fDestDraft = kODNULL;
                _fDestFrame = kODNULL;

                ODDeleteObject(_fClonedSUIDs);
        }

        ODDeleteObject(_fWeakClonedSUIDs);
        _fWeakClonedSUIDs = _fSavedWeakClonedSUIDs;

#ifdef DebugClone
        somPrintf("CMDraft::EndClone - Clone completed\n\n");
#endif

    }
    catch (ODException _exception)
    {
       ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: AbortClone
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftAbortClone(CMDraft *somSelf, Environment *ev,
                ODDraftKey key)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","AbortClone");

   try
   {

#if ODDebug_Drafts
        somPrintf("CMDraftAbort: %d\n", somSelf->GetID(ev));
#endif

        if ((key != _fCurrentKey) || (_fLockCount == 0))
                THROW(kODErrInvalidDraftKey);

        --_fLockCount;

        if ( _fLockCount == 0 )
        {
                _fDestDraft = kODNULL;
                _fDestFrame = kODNULL;

                ODDeleteObject(_fClonedSUIDs);
        }

        ODDeleteObject(_fWeakClonedSUIDs);
        _fWeakClonedSUIDs = _fSavedWeakClonedSUIDs;

   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDraft: SetOriginalID
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftSetOriginalID(CMDraft *somSelf, Environment* ev,
                ODStorageUnitID destID,
                ODStorageUnitID originalID)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","SetOriginalID");

    try
    {

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *destSU = _fDestDraft->AcquireStorageUnit(ev, destID);
#else
        TempODStorageUnit destSU = _fDestDraft->AcquireStorageUnit(ev, destID);
#endif
        ODSetULongProp(ev, destSU, kODPropOriginalID, kODULong, originalID);
#ifdef _NO_TMP_OBJS_
        destSU->Release(ev);
#endif

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: GetOriginalID
//------------------------------------------------------------------------------
// An original ID may not be present in the root storage unit of the draft,
// since this SU is often constructed in place rather than cloned.
// An null object ID is returned if the property is not present.

SOM_Scope ODStorageUnitID  SOMLINK CMDraftGetOriginalID(CMDraft *somSelf, Environment* ev,
                ODStorageUnitID fromID)
{
    //CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetOriginalID");

        ODStorageUnitID retSID = 0;

    try
    {
#ifdef _NO_TMP_OBJS_
        ODStorageUnit *fromSU = somSelf->AcquireStorageUnit(ev, fromID);
#else
        TempODStorageUnit fromSU = somSelf->AcquireStorageUnit(ev, fromID);
#endif

        retSID = (ODStorageUnitID)ODGetULongProp(ev, fromSU, kODPropOriginalID, kODULong);
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
        return retSID;
}

//------------------------------------------------------------------------------
// CheckPartAction
//------------------------------------------------------------------------------

static ODBoolean CheckPartAction(void* k, void* v, ODULong s, void* r)
{
        Environment*    ev = somGetGlobalEnvironment();
        ODBoolean               result = kODFalse;
   try
   {
        ODID                    originalID =  * (ODID *) k;
        ODID                    interupdateID = * (ODID *) v;
        OpenHashTable*  targetParts = (OpenHashTable*) r;

        // Note that storage units in the original document draft may not be up to date
        // if changes have not been externalized.  In particular, a frame's storage
        // unit may not have a valid reference to its part's storage unit.  For this
        // reason, use the storage units in the data interchange draft instead.

#ifdef DebugClone
        somPrintf("interupdateID %d, originalID %d\n", interupdateID, originalID);
#endif

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *interchangeSU = ::sInterchangeDraft->AcquireStorageUnit(ev, interupdateID);
#else
        TempODStorageUnit interchangeSU = ::sInterchangeDraft->AcquireStorageUnit(ev, interupdateID);
#endif
        if ( ODSUExistsThenFocus(ev, interchangeSU, kODPropStorageUnitType, kODISOStr) )
        {
                if ( interchangeSU->GetSize(ev) == ODISOStrLength(kODFrameObject)+1 )
                {
                        ODULong size = ODISOStrLength(kODFrameObject)+1;
                        // Only read the same number of characters as in kODFrameObject, to prevent overwriting the
                        // terminating zero byte of the buffer.
                        if (ODGetISOStrProp(ev, interchangeSU, kODPropStorageUnitType, kODISOStr, (ODISOStr) ::sSUTypeBuffer, &size)
                                != kODNULL)
                        {
                                if (ODISOStrEqual(kODFrameObject, ::sSUTypeBuffer))
                                {
                                        ODID interchangePartID = ODGetStrongSURefProp(ev, interchangeSU, kODPropPart, kODStrongStorageUnitRef);
                                        if ( interchangePartID == kODNULLID )
                                        {
                                                // Note: If the reference isn't valid, ODGetStrongSURefProp will return
                                                //   kODNULLID so it isn't necessary to call IsValidID instead.
#ifdef DebugClone
                                                somPrintf("Invalid part reference from frame id %d\n", interupdateID);
#endif
                                                result = kODTrue;       // Better safe than sorry!
                                        }
                                        else if ( interchangePartID != ::sRootPartIDToIgnore )
                                        {
                                                ODID targetPartID = ::sInterchangeDraft->GetOriginalID(ev, interchangePartID);
#ifdef DebugClone
                                                somPrintf("Checking part id %d from frame id %d\n", targetPartID, originalID);
#endif
                                                result = targetParts->Exists(&targetPartID);
                                        }
#ifdef DebugClone
                                        else
                                        {
                                                somPrintf("Skipping root part id %d because it was not cloned\n", interchangePartID);
                                        }
#endif
                                }
                        }
                }
        }

#ifdef _NO_TMP_OBJS_
        interchangeSU->Release(ev);
#endif
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
        return result;
}

//------------------------------------------------------------------------------
// CMDraft: ContainingPartInClone
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftContainingPartInClone(CMDraft *somSelf, Environment* ev,
                ODFrame* targetFrame)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ContainingPartInClone");

        ODBoolean               result = kODFalse;
        ODFrame*                frame;
        ODFrame*                nextFrame;
        ODID                    partID;

        OpenHashTable* targetParts = kODNULL;
        OpenHashTable* clonedSUIDs = kODNULL;

        try
        {

                ODMemoryHeapID  heap = somSelf->GetHeap(ev);

#ifdef DebugClone
                somPrintf("ContainingPartInClone called with target frame id %d <%x>\n", targetFrame->GetID(ev), targetFrame);
#endif

#ifdef _PLATFORM_MACINTOSH_
                OpenHashTable* targetParts = new(heap) OpenHashTable(OpenHashTable::StdEqual,
                                                                                                        OpenHashTable::StdHash,
                                                                                                        heap);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//              The use of heap is not implemented in Windows platform
                OpenHashTable* targetParts = new OpenHashTable(OpenHashTable::StdEqual,
                                                        OpenHashTable::StdHash,
                                                        0);
#endif
                targetParts->Initialize(kInitialHashTableEntries, sizeof(ODID), 0, kODTrue);

                // Build a table of parts that own or embed the target frame
                frame = targetFrame;
                if ( frame )
                        frame->Acquire(ev);
                while ( frame != kODNULL )
                {
#ifdef _NO_TMP_OBJS_
                        ODFrame *tempFrame = frame; // ensure it's released
#else
                        TempODFrame tempFrame = frame; // ensure it's released
#endif
                        ODPart* part = frame->AcquirePart(ev);
                        partID = part->GetID(ev);
                        part->Release(ev);
#ifdef DebugClone
                        somPrintf("Adding target part id %d from frame id %d\n", partID, frame->GetID(ev));
#endif
                        targetParts->ReplaceEntry(&partID, kODNULL);

                        nextFrame = frame->AcquireContainingFrame(ev);
                        if ( nextFrame == kODNULL )
                        {
#ifdef _NO_TMP_OBJS_
                                { ODWindow *window = frame->AcquireWindow(ev);
#else
                                { TempODWindow window = frame->AcquireWindow(ev);
#endif
                                  nextFrame = window->AcquireSourceFrame(ev);
#ifdef _NO_TMP_OBJS_
                                window->Release(ev);
#endif
                                }

                                // The source frame always displays the same part, so skip to its containing frame.
                                // Note that it must have a containing frame.
                                if ( nextFrame != kODNULL )
                                {
#ifdef _NO_TMP_OBJS_
                                        ODFrame *sourceFrame = nextFrame;
#else
                                        TempODFrame sourceFrame = nextFrame;
#endif
                                        nextFrame = sourceFrame->AcquireContainingFrame(ev);
#ifdef _NO_TMP_OBJS_
                                        sourceFrame->Release(ev);
#endif
                                }
                        }
                        frame = nextFrame;
#ifdef _NO_TMP_OBJS_
                tempFrame->Release(ev);
#endif
                }

#ifdef _PLATFORM_MACINTOSH_
                clonedSUIDs = new(heap) OpenHashTable(OpenHashTable::StdEqual,
                                                                                OpenHashTable::StdHash,
                                                                                heap);
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//              The use of heap is not implemented in Windows platform
                clonedSUIDs = new OpenHashTable(OpenHashTable::StdEqual,
                                                OpenHashTable::StdHash,
                                                0);
#endif
                clonedSUIDs->Initialize(kInitialHashTableEntries,
                                          sizeof(ODStorageUnitID),
                                          sizeof(ODStorageUnitID));

                // Get the table of objects cloned into the data interchange draft
                ReadClonedObjectTable(ev, clonedSUIDs, somSelf);

                // See if any frame of any target part is part of the clone
                ::sInterchangeDraft = somSelf;
                ::sSUTypeBuffer = (ODISOStr) ODNewPtrClear(ODISOStrLength(kODFrameObject) + 1);
                ::sRootPartIDToIgnore = ( _fRootPartReused ? kODNULLID : RootPartID(ev, somSelf) );

//                This line of code is commented out because the object IDs 
//                stored in kODPropClonedObjectTable is not valid any more if
//                the drag and drop storage unit is closed and reopened. IDs
//                are not persistent objects and cannot be depended on. Since
//                the main purpose of function CheckPartAction is to prevent
//                drag and drop to the same frame and part and drag and drop
//                codes have receipe to prevent this from happen, comment out
//                this code should not create any ill problem.
//
//                result = clonedSUIDs->Walk(CheckPartAction, targetParts);
                ODDisposePtr(::sSUTypeBuffer);

     }
     catch (ODException _exception)
     {
         result = kODFalse;
         ODSetSOMException(ev, _exception);
     }


        ODDeleteObject(targetParts);
        ODDeleteObject(clonedSUIDs);

#ifdef DebugClone
        somPrintf("ContainingPartInClone returns %d\n", result);
#endif

        return result;
}

//------------------------------------------------------------------------------
// CMDraft: ValidWeakReferenceProperty
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftValidWeakReferenceProperty(CMDraft *somSelf, Environment *ev,
        ODID                    objectID,
        ODPropertyName  propertyName)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","ValidWeakReferenceProperty");

        ODBoolean result = kODFalse;

        try
        {

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *objectSU = somSelf->AcquireStorageUnit(ev, objectID);
#else
        TempODStorageUnit objectSU = somSelf->AcquireStorageUnit(ev, objectID);
#endif
        if (objectSU != kODNULL)
        {
                result = (ODGetWeakSURefProp(ev, objectSU, propertyName, kODWeakStorageUnitRef) != 0);
//#ifdef DebugClone
//                      somPrintf("ValidWeakReferenceProperty: %s property does not exist.\n", propertyName);
//#endif
        }

#ifdef _NO_TMP_OBJS_
        objectSU->Release(ev);
#endif

        }
        catch (ODException _exception)
        {
#ifdef DebugClone
                somPrintf("ValidWeakReferenceProperty: returning error.\n");
#endif
                ODSetSOMException(ev, _exception);
                result =  kODFalse;
        }
        return result;

}

//------------------------------------------------------------------------------
//  CMDraft: CloneCompanionObject
//------------------------------------------------------------------------------
// If fromID is a link or link source object, clone its companion.
// Note that kODNULLID is always used as the scopeID, since scope is irrelevant
// for link and link source objects.

SOM_Scope ODID  SOMLINK CMDraftCloneCompanionObject(CMDraft *somSelf, Environment *ev,
                ODDraftKey key,
                ODID fromID)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CloneCompanionObject");

        ODID toID = kODNULLID;  ODVolatile(toID);

        try
        {

                ODPropertyName companionProperty = kODNULL;

                if ( IsLinkObject(ev, somSelf, fromID) )
                        companionProperty = kODPropLinkSource;
                else if ( IsLinkSourceObject(ev, somSelf, fromID) )
                        companionProperty = kODPropLink;

                if ( companionProperty != kODNULL )
                {
#ifdef _NO_TMP_OBJS_
                        ODStorageUnit *objectSU = somSelf->AcquireStorageUnit(ev, fromID);
#else
                        TempODStorageUnit objectSU = somSelf->AcquireStorageUnit(ev, fromID);
#endif
                        if ( objectSU != kODNULL )
                        {
                                ODStorageUnitID companionID = ODGetWeakSURefProp(ev, objectSU, companionProperty, kODWeakStorageUnitRef);
                                if ( companionID != kODNULLID )
                                {
                                        // Optimization: If the companion object has already been cloned,
                                        // there is no need to clone it again since scope is irrelevant for
                                        // these objects. [cc 9/28/95]
                                        _fClonedSUIDs->GetValue(&companionID, &toID);
                                        if ( toID == kODNULLID )
                                                toID = somSelf->StrongClone(ev, key, companionID, kODNULLID, kODNULLID);
                                }
                        }
#ifdef _NO_TMP_OBJS_
                        objectSU->Release(ev);
#endif
                }

        }
        catch (ODException _exception)
        {
             toID = kODNULLID;
             ODSetSOMException(ev, _exception);
        }

        return toID;
}

//------------------------------------------------------------------------------
//  CMDraft: CompanionWasCloned
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftCompanionWasCloned(CMDraft *somSelf, Environment* ev,
                ODStorageUnitID fromID,
                ODPropertyName prop)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CompanionWasCloned");

        ODBoolean result = kODFalse;

        try
        {
                result= somSelf->ValidWeakReferenceProperty(ev, fromID, prop);
        }
        catch (ODException _exception)
        {
            ODSetSOMException(ev, _exception);
        }
        return result;
}

//------------------------------------------------------------------------------
//  CMDraft: GetCloneKind
//------------------------------------------------------------------------------

SOM_Scope ODCloneKind  SOMLINK CMDraftGetCloneKind(CMDraft *somSelf, Environment* ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetCloneKind");

        ODCloneKind cloneKind = _fCloneKind;

     try
     {

        if ( cloneKind == kODClonePaste )
        {
                if ( (GetOriginalCloneKind(ev, somSelf) == kODCloneCut) && (GetOriginalDraft(ev, somSelf) == _fDestDraft) )
                        cloneKind = kODCloneDropMove;
                else
                        cloneKind = kODCloneDropCopy;
        }
        else if ( cloneKind == kODCloneToFile )
        {
                cloneKind = kODCloneDropCopy;
        }

     }
     catch (ODException _exception)
     {
           ODSetSOMException(ev, _exception);
     }
        return cloneKind;
}

//------------------------------------------------------------------------------
//  CMDraft: CopiedLinkSubstitution
//------------------------------------------------------------------------------
//
// If this clone is a copy, or a move to another draft, substitute the original
// object ID or the null object id, and return true.  Returns false if the object
// should be cloned, and does not assign to the toID parameter.
// The toObjectID parameter is just for validity checking.

SOM_Scope ODBoolean  SOMLINK CMDraftCopiedLinkSubstitution(CMDraft *somSelf, Environment* ev,
                ODDraftKey      key,
                ODID            fromID,
                ODID            toObjectID,
                ODID*           toID)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CopiedLinkSubstitution");

        ODUnused(key);

        ODBoolean substitute = kODFalse;

    try
    {

        ODBoolean isCopy = (somSelf->GetCloneKind(ev) == kODCloneDropCopy);
        ODBoolean isCrossDraftMove =
                (somSelf->GetCloneKind(ev) == kODCloneDropMove) && (GetOriginalDraft(ev, somSelf) != _fDestDraft);

        if ( isCopy || isCrossDraftMove )
        {
                if ( IsLinkObject(ev, somSelf, fromID) )
                {
                        if ( !somSelf->CompanionWasCloned(ev, fromID, kODPropLinkSource) )
                        {
                                if ( GetOriginalDraft(ev, somSelf) == _fDestDraft )
                                {
                                        ASSERT(toObjectID == 0, kODErrInvalidID);
                                        *toID = somSelf->GetOriginalID(ev, fromID);
                                        ASSERT(*toID != 0, kODErrInvalidID);
                                }
                                else
                                        *toID = 0;
                                substitute = kODTrue;
                        }
                }
                else if ( IsLinkSourceObject(ev, somSelf, fromID) )
                {
                        if ( !somSelf->CompanionWasCloned(ev, fromID, kODPropLink) )
                        {
                                *toID = 0;
                                substitute = kODTrue;
                        }
                }
        }

#ifdef DebugClone
        if ( substitute )
        {
                if ( !IsLinkObject(ev, somSelf, fromID) )
                        IsLinkSourceObject(ev, somSelf, fromID);
                somPrintf("Reusing existing object id = %d for cloned object %d\n", toID, fromID);
        }
#endif

    }
    catch (ODException _exception)
    {
        substitute = kODFalse;
        ODSetSOMException(ev, _exception);
    }
        return substitute;
}

//------------------------------------------------------------------------------
// CMDraft: Clone
//------------------------------------------------------------------------------
//
// Why is the switch statement in this method so hard to understand?  It should
// really be replaced by a table-driven algorithm.  There are only three possible
// actions when cloning a link or link source object:
//              (1) Go ahead and clone the link,
//              (2) Don't clone the link,
//              (3) Substitute the original link rather than clone in a duplicate
// However, there are four factors that determine which action should be taken.
// There are 8x3x2x2 = 96 distinct combinations of these factors; however,
// most combinations are impossible or illegal.
// The switch statement is an embodyment in code of the resulting sparse matrix.
// Several cases are sometimes handled at once.  Only by going back to the full
// case analysis represented by a table can one understand this code.
//
// Note that its possible that a link object's companion doesn't exist; in this
// case the link is broken and won't be copied.
//

SOM_Scope ODID  SOMLINK CMDraftClone(CMDraft *somSelf, Environment *ev,
                ODDraftKey key,
                ODID fromID,
                ODID toObjectID,
                ODID scopeID)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Clone");

        ODStorageUnitID toID = 0;

    try
    {

        if ((key != _fCurrentKey) || (_fLockCount == 0))
                THROW(kODErrInvalidDraftKey);


        ODStorageUnitID companionID = 0;

#ifdef _PLATFORM_OS2_

        switch ( somSelf->GetCloneKind(ev) )
        {
        case kODCloneAll:
        case kODCloneFromLink:
                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                break;

        case kODCloneToLink:
                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                break;

        case kODCloneCopy:
        case kODCloneCut:
                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                somSelf->SetOriginalID(ev, toID, fromID);
                break;

        case kODCloneDropCopy:
                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                break;
        case kODCloneDropMove:
                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                break;

        default:
                break;
        }


// Notes on cloning in OS2 implementation:
// In OS2 implementation, we need to distinguish SUs which are clones of links
// from SUs which are the original links for the following reasons:
// 1. A stored link which is not a clone comes back up without issuing
//    AddLinkTarget, whereas a clone must issue AddLinkTarget (unless its
//    source was also copied)
// 2. A cloned link which has a valid ref to a link source (meaning that
//    its link source was also copied) has an invalid linkid for the source,
//    since new linkids are created lazily.
//    Such a link cannot be further cloned from a document (copy, cut, copytolink)
//    for fear that it will be cloned without its linksource and the
//    correct linksource reference lost.

//    So whenever we clone a link (target) object, we give it the CloneOfALink property
//    Whenever the link comes up again, that property is removed.

#ifdef _NO_TMP_OBJS_
        ODStorageUnit* destSU  = _fDestDraft->AcquireStorageUnit(ev, toID);
#else
        TempODStorageUnit destSU  = _fDestDraft->AcquireStorageUnit(ev, toID);
#endif

        if (IsLinkObject(ev,_fDestDraft,toID) &&
            !destSU->Exists(ev, kODPropCloneOfALink, kODNULL, 0))
              {
                ODSUAddPropValue(ev, destSU, kODPropCloneOfALink, kODBoolean);
                ODBoolean True = kODTrue ;
                StorageUnitSetValue(destSU, ev, sizeof(ODBoolean), (ODValue)&(True));
              }
#ifdef _NO_TMP_OBJS_
        destSU->Release(ev);
#endif

#else
        switch ( somSelf->GetCloneKind(ev) )
        {
        case kODCloneAll:
                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                somSelf->CloneCompanionObject(ev, key, fromID);
                break;

        case kODCloneFromLink:
        case kODCloneToLink:
                if ( IsNeitherLinkObject(ev, somSelf, fromID) )
                        toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                break;

        case kODCloneCopy:
        case kODCloneCut:
                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                somSelf->SetOriginalID(ev, toID, fromID);
                break;

        case kODCloneDropCopy:
                if ( !somSelf->CopiedLinkSubstitution(ev, key, fromID, toObjectID, &toID) )
                        toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                break;

        case kODCloneDropMove:
                // When moved to another draft, links should behave as if copied
                if ( !somSelf->CopiedLinkSubstitution(ev, key, fromID, toObjectID, &toID) )
                {
                        if ( GetOriginalDraft(ev, somSelf) == _fDestDraft )
                        {
                                // toObjectID will be non-zero if the object has been weakly cloned.
                                // If the weak clone substituted the original object, set toID so
                                // the strong clone is avoided.  If the weak clone was into a
                                // storage unit other than the original, toID is not set, so the
                                // strong clone will be performed.
                                ODID originalID = somSelf->GetOriginalID(ev, fromID);
                                if ( _fDestDraft->IsValidID(ev, originalID)
                                     &&
                                     ((toObjectID == 0) || (toObjectID == originalID)) )
                                        toID = originalID;
                                somSelf->CheckClonedObject(ev, fromID, toID, originalID);
                        }
                        if ( toID == 0 )
                        {
                                toID = somSelf->StrongClone(ev, key, fromID, toObjectID, scopeID);
                        }
#ifdef DebugClone
                        else
                        {
                                if ( !IsLinkObject(ev, somSelf, fromID) )
                                        IsLinkSourceObject(ev, somSelf, fromID);
                                somPrintf("Reusing existing object id = %d for cloned object %d\n", toID, fromID);
                        }
#endif
                }
                break;

        default:
                break;
        }
#endif // _PLATFORM_OS2_

    }
    catch (ODException _exception)
    {
         toID = 0;
         ODSetSOMException(ev, _exception);
    }
        return toID;
}

//------------------------------------------------------------------------------
// CMDraft: StrongClone
//------------------------------------------------------------------------------

SOM_Scope ODID  SOMLINK CMDraftStrongClone(CMDraft *somSelf, Environment *ev,
                ODDraftKey key,
                ODID fromID,
                ODID toObjectID,
                ODID scopeID)
{

    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","StrongClone");

        ODStorageUnitID         toID = 0;

   try
   {

#ifdef DebugClone
        somPrintf("CMDraftStrongClone %d %d %d\n", fromID, toObjectID, scopeID);
#endif

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *toSU = kODNULL;
#else
        TempODStorageUnit toSU = kODNULL;
#endif

        _fClonedSUIDs->GetValue(&fromID, &toID);
        if (toID == 0) {
                _fWeakClonedSUIDs->GetValue(&fromID, &toID);
                if (toID == 0) {
                        if (toObjectID != 0)
                                toSU = _fDestDraft->AcquireStorageUnit(ev, toObjectID);
                        else
                                toSU = _fDestDraft->CreateStorageUnit(ev);
                        toID = toSU->GetID(ev);
                        _fClonedSUIDs->ReplaceEntry(&fromID, &toID);
                }
                else {
                        if ((toObjectID != 0) && (toObjectID != toID))
                                THROW(kODErrInvalidID);
                        _fWeakClonedSUIDs->RemoveEntry(&fromID);
                        _fClonedSUIDs->ReplaceEntry(&fromID, &toID);
                        toSU = _fDestDraft->AcquireStorageUnit(ev, toID);
                }
        }
        else {
                if ((toObjectID != 0) && (toObjectID != toID))
                        THROW(kODErrInvalidID);
                toSU = _fDestDraft->AcquireStorageUnit(ev, toID);
        }

        ODPersistentObject *fromObject = somSelf->RetrievePersistentObject(ev, fromID);
        if ( fromObject ) {
#ifdef _NO_TMP_OBJS_
                ODRefCntObject *tempFromObject = fromObject; // so it gets released
                ODFrame *scopeFrame = kODNULL;
#else
                TempODRefCntObject tempFromObject = fromObject; // so it gets released
                TempODFrame scopeFrame = kODNULL;
#endif
                if (scopeID != 0)
                        scopeFrame = somSelf->AcquireFrame(ev, scopeID);
                fromObject->CloneInto(ev, key, toSU, scopeFrame);
                ODStorageUnit* fromSU = fromObject->GetStorageUnit(ev);
                if (fromSU != kODNULL)
                        CopyDraftAnnotations(ev, fromSU, toSU);
#ifdef _NO_TMP_OBJS_
                tempFromObject->Release(ev);
                scopeFrame->Release(ev);
#endif
        }
        else {
#ifdef _NO_TMP_OBJS_
                ODStorageUnit *fromSU = somSelf->AcquireStorageUnit(ev, fromID);
#else
                TempODStorageUnit fromSU = somSelf->AcquireStorageUnit(ev, fromID);
#endif
                if (fromSU != kODNULL) {
#ifdef _PLATFORM_OS2_
                    // If the following conditions are all met:
                    // o This SU has the CloneOfALinkProperty
                    // o This SU has a valid LinkTargetToLinkSource reference
                    //   (implying the link was cloned together with its linksource)
                    // o Origin is a live document (clonekind is Copy, Cut, or CopytoLink)
                    // then we must convert the SU into an object before further cloning.
                    // The reason is that the linkid information in the SU
                    // is invalid, as a result of our decision not to register
                    // copied linksources with the AS until they have been opened.
                    if (fromSU->Exists(ev, kODPropCloneOfALink, kODNULL, 0) &&
                        somSelf->CompanionWasCloned(ev, fromID, kODPropLinkTargetToLinkSource) &&
                        (_fCloneKind == kODCloneCopy || _fCloneKind == kODCloneCut || _fCloneKind == kODCloneToLink) ) {

                            fromObject = (ODPersistentObject*) somSelf->AcquireLink(ev, fromID, (ODLinkSpec*) kODNULL);
                            ODFrame* scopeFrame = kODNULL;
                            if (scopeID != 0)
                                    scopeFrame = somSelf->AcquireFrame(ev, scopeID);
                            fromObject->CloneInto(ev, key, toSU, scopeFrame);
                            fromObject->Release(ev);
                            if (scopeFrame != kODNULL)
                                    scopeFrame->Release(ev);
                    } else {
                            fromSU->CloneInto(ev, key, toSU, scopeID);
                    }
//                    fromSU->Release(ev);
#else
                        fromSU->CloneInto(ev, key, toSU, scopeID);
			ODBoolean keepProxyProperties = (_fCloneKind == kODCloneToFile)
			  || (_fCloneKind == kODCloneAll);
                        RemoveDataInterchangeProperties(ev, toSU, keepProxyProperties);
//                      CopyDraftAnnotations(ev, fromSU, toSU);
#endif // _PLATFORM_OS2_
                }
#ifdef _NO_TMP_OBJS_
                fromSU->Release(ev);
#endif

        }

#ifdef DebugClone
        somPrintf("Done CMDraftStrongClone %d\n", toID);
#endif

#ifdef _NO_TMP_OBJS_
        toSU->Release(ev);
#endif

    }
    catch (ODException _exception)
    {
         toID = 0;
         ODSetSOMException(ev, _exception);
    }
        return toID;
}

//------------------------------------------------------------------------------
// CMDraft: WeakClone
//------------------------------------------------------------------------------

SOM_Scope ODID  SOMLINK CMDraftWeakClone(CMDraft *somSelf, Environment *ev,
                ODDraftKey key,
                ODID fromID,
                ODID toObjectID,
                ODID scopeID)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","Clone");

        ODStorageUnitID toID = 0;
   try
   {

        _fClonedSUIDs->GetValue(&fromID, &toID);
        if (toID == 0)
        {
                _fWeakClonedSUIDs->GetValue(&fromID, &toID);
                if (toID == 0)
                {
                        toID = toObjectID;
                        if ( toID == 0 )
                        {
                                // Note that this may substitute the null object id, which
                                // will be returned instead of a valid object id!
#ifdef _PLATFORM_OS2_
                                ODBoolean substitute = kODFalse;
#else
                                ODBoolean substitute = somSelf->CopiedLinkSubstitution(ev, key, fromID, toObjectID, &toID);
#endif
                                if ( !substitute )
//                                if ( !somSelf->CopiedLinkSubstitution(ev, key, fromID, toObjectID, &toID) )
                                {
                                        if ( somSelf->GetCloneKind(ev) == kODCloneDropMove )
                                        {
                                                // If this clone is a move back into the original draft,
                                                // substitute the original object, making this equivalent to
                                                // a strong clone.
                                                if ( GetOriginalDraft(ev, somSelf) == _fDestDraft )
                                                        toID = somSelf->GetOriginalID(ev, fromID);
                                        }
                                        if ( toID == 0 )
                                        {
#ifdef _NO_TMP_OBJS_
                                                ODStorageUnit *toSU = _fDestDraft->CreateStorageUnit(ev);
#else
                                                TempODStorageUnit toSU = _fDestDraft->CreateStorageUnit(ev);
#endif
                                                toID = toSU->GetID(ev);
                                                _fWeakClonedSUIDs->ReplaceEntry(&fromID, &toID);
#ifdef _NO_TMP_OBJS_
                                                toSU->Release(ev);
#endif
                                        }
                                }
                        }
                }
        }
        if ((toObjectID != 0) && (toObjectID != toID))
                THROW(kODErrInvalidID);

    }
    catch (ODException _exception)
    {
         toID = 0;
         ODSetSOMException(ev, _exception);
    }
        return toID;
}

//------------------------------------------------------------------------------
// CMDraft: IsValidDraftKey
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDraftIsValidDraftKey(CMDraft *somSelf, Environment *ev,
                ODDraftKey key)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","IsValidDraftKey");

        return ((key != kODNULLKey) && (_fCurrentKey == key) ? kODTrue : kODFalse);
}

//------------------------------------------------------------------------------
// CopyProperty
//------------------------------------------------------------------------------

static void CopyProperty(Environment *ev, ODStorageUnit* fromSU, ODStorageUnit* toSU, ODPropertyName prop)
{
        // Assuming fromSU is focused to prop.
        // Copy all values into toSU, overwriting any existing values there.

   try
   {
        PreserveFocus fromFocus(ev, fromSU);
        PreserveFocus toFocus(ev, toSU);

        ODULong                 numValues;
        ODULong                 j;
        ODValueType             valueName;

        fromSU->Focus(ev, (ODPropertyName) prop, kODPosUndefined, kODNULL, 0, kODPosAll);

        numValues = fromSU->CountValues(ev);

        if ( numValues > 0)
                ODSUForceFocus(ev, toSU, prop, kODNULL);

        for (j = 0; j < numValues; j++)
        {
                fromSU->Focus(ev, (ODPropertyName) kODNULL, kODPosSame, kODNULL, 0, kODPosNextSib);

                valueName = fromSU->GetType(ev);

                ODSUForceFocus(ev, toSU, prop, valueName);
                ODULong toSize = toSU->GetSize(ev);
                toSU->DeleteValue(ev, toSize);

                ODDisposePtr(valueName);

                ODULong size = fromSU->GetSize(ev);
                ODPtr buffer = ODNewPtr(size);
                StorageUnitGetValue(fromSU, ev, size, (ODValue) buffer);
                StorageUnitSetValue(toSU, ev, size, (ODValue) buffer);
                ODDisposePtr(buffer);
        }
   }
   catch (ODException _exception)
   {
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CopyDraftAnnotations
//------------------------------------------------------------------------------

static void CopyDraftAnnotations(Environment *ev, ODStorageUnit* fromSU, ODStorageUnit* toSU)
{
   try
   {
        ODULong annotationPrefixLength = ODISOStrLength(kODPropPreAnnotation);
        ODULong metaDataPrefixLength = ODISOStrLength(kODPropPreODMetaData);

        fromSU->Focus(ev, (ODPropertyName) kODNULL,
                                                        kODPosAll,
                                                        kODTypeAll,
                                                        0,
                                                        kODPosUndefined);
        ODULong numProperties = fromSU->CountProperties(ev);
        for (ODULong i = 1; i <= numProperties; i++) {
                fromSU->Focus(ev, (ODPropertyName) kODNULL,
                                                kODPosNextSib,
                                                kODTypeAll,
                                                0,
                                                kODPosUndefined);
                ODPropertyName propertyName = fromSU->GetProperty(ev);
                if (ODISOStrNCompare(propertyName, kODPropPreAnnotation, annotationPrefixLength) == 0)
                        CopyProperty(ev, fromSU, toSU, propertyName);
                else if ((ODISOStrNCompare(propertyName, kODPropPreODMetaData, metaDataPrefixLength) == 0) &&
                        (toSU->Exists(ev, propertyName, kODNULL, 0) == kODFalse)) {
#ifdef ODDebug_CloningAnnotations
                        SetOutputMode(kWriteToFile);
                        PRINT("metadata property %s does not exist in destination su.\n", propertyName);
#endif
                        CopyProperty(ev, fromSU, toSU, propertyName);
                }
                ODDisposePtr(propertyName);
        }

        // Force copying of storage unit type property, which always exists so is
        // not copied in the iteration above.
        CopyProperty(ev, fromSU, toSU, kODPropStorageUnitType);
    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// IsLinkObject
//------------------------------------------------------------------------------

static ODBoolean IsLinkObject(Environment* ev, ODDraft* draft, ODID objectID)
{
        // Only link objects contain a kODPropLinkSource property
        //  in OS2, a kODPropAvailabilityServerName property

          ODBoolean isLink = kODFalse;
    try
    {

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *su = draft->AcquireStorageUnit(ev, objectID);
#else
        TempODStorageUnit su = draft->AcquireStorageUnit(ev, objectID);
#endif
#ifdef _PLATFORM_OS2_
        isLink = su->Exists(ev, kODPropAvailabilityServerName, kODNULL, 0);
#else
        isLink = su->Exists(ev, kODPropLinkSource, kODNULL, 0);
#endif

#ifdef DebugClone
        somPrintf("IsLinkObject returns %u\n", isLink);
#endif

#ifdef _NO_TMP_OBJS_
        su->Release(ev);
#endif

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
        return isLink;
    
}

//------------------------------------------------------------------------------
// IsLinkSourceObject
//------------------------------------------------------------------------------

static ODBoolean IsLinkSourceObject(Environment* ev, ODDraft* draft, ODID objectID)
{
        // Only link source objects contain a kODPropLink property
        // in OS2, a kODPropLinkSourceState property

             ODBoolean isLink = kODFalse;

    try
    {

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *su = draft->AcquireStorageUnit(ev, objectID);
#else
        TempODStorageUnit su = draft->AcquireStorageUnit(ev, objectID);
#endif
#ifdef _PLATFORM_OS2_
        isLink = su->Exists(ev, kODPropLinkSourceState, kODNULL, 0);
#else
        isLink = su->Exists(ev, kODPropLink, kODNULL, 0);
#endif

#ifdef DebugClone
        somPrintf("IsLinkSourceObject returns %u\n", isLink);
#endif

#ifdef _NO_TMP_OBJS_
        su->Release(ev);
#endif

    }
    catch (ODException _exception)
    {
        isLink =  kODFalse;
        ODSetSOMException(ev, _exception);
    }
        return isLink;
}

//------------------------------------------------------------------------------
// IsNeitherLinkObject
//------------------------------------------------------------------------------

static ODBoolean IsNeitherLinkObject(Environment* ev, ODDraft* draft, ODID objectID)
{
        if ( IsLinkObject(ev, draft, objectID) )
                return kODFalse;
        else
                return !IsLinkSourceObject(ev, draft, objectID);
}

//------------------------------------------------------------------------------
// SetStorageUnitType
//------------------------------------------------------------------------------

static void SetStorageUnitType(Environment* ev, ODDraftPermissions perms, ODStorageUnit* su, ODType suType)
{
    try
    {
        if (perms == kODDPExclusiveWrite)
        {
#ifdef _NO_TMP_OBJS_
                char *curType = ODGetISOStrProp(ev, su, kODPropStorageUnitType, kODISOStr, kODNULL, kODNULL);
#else
                TempODType curType = ODGetISOStrProp(ev, su, kODPropStorageUnitType, kODISOStr, kODNULL, kODNULL);
#endif
                if (!curType || !ODISOStrEqual(curType, suType))
                        ODSetISOStrProp(ev, su, kODPropStorageUnitType, kODISOStr, suType);
        }
    }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// GetStorageUnitType
//------------------------------------------------------------------------------

static ODISOStr GetStorageUnitType(Environment* ev, ODDraft* draft, ODID objectID)
{
            ODISOStr Storageunittype = kODNULL;      
    try
    {

#ifdef _NO_TMP_OBJS_
        ODStorageUnit *su = draft->AcquireStorageUnit(ev, objectID);
#else
        TempODStorageUnit su = draft->AcquireStorageUnit(ev, objectID);
#endif
        PreserveFocus fromFocus(ev, su);

        Storageunittype =  ODGetISOStrProp(ev, su, kODPropStorageUnitType, kODISOStr, kODNULL, kODNULL);

    }
    catch (ODException _exception)
    {
       ODSetSOMException(ev, _exception);
    }
         return Storageunittype;
}

//------------------------------------------------------------------------------
// RootPartID
//------------------------------------------------------------------------------

static ODID RootPartID(Environment* ev, ODDraft* draft)
{
         ODID RootpartID = kODNULLID;

     try
     {
#ifdef _NO_TMP_OBJS_
        ODStorageUnit *draftProperties = draft->AcquireDraftProperties(ev);
#else
        TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);
#endif
        RootpartID =  ODGetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef);
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
        return RootpartID;
}

//------------------------------------------------------------------------------
// CheckClonedObject
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftCheckClonedObject(CMDraft *somSelf, Environment *ev,
                ODID fromID,
                ODID toID,
                ODID originalID)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CheckClonedObject");

    ODISOStr storageUnitType = kODNULL; ODVolatile(storageUnitType);

        try
        {

    storageUnitType = GetStorageUnitType(ev, somSelf, fromID);

#ifdef DebugClone
    somPrintf("CheckClonedObject: Type is %s\n", storageUnitType);
#endif

        if ( ODISOStrEqual(kODFrameObject, storageUnitType) )
        {
                _fAnyFrameCloned = kODTrue;
        }
        else if ( ODISOStrEqual(kODPartObject, storageUnitType) )
        {
                if ( (toID == originalID) && (RootPartID(ev, somSelf) == fromID) )
                {
                        _fRootPartReused = kODTrue;
#ifdef DebugClone
                        somPrintf("CheckClonedObject: Root part reused\n");
#endif
                }
        }

        }
        catch (ODException _exception)
        {
            ODSetSOMException(ev, _exception);
        }

        ODDisposePtr(storageUnitType);
}

//------------------------------------------------------------------------------
// SetOriginalDraft
//------------------------------------------------------------------------------

static void SetOriginalDraft(Environment* ev, ODDraft* targetDraft, ODDraft* originalDraft)
{
    try
    {
#ifdef _NO_TMP_OBJS_
        ODStorageUnit *draftProperties = targetDraft->AcquireDraftProperties(ev);
#else
        TempODStorageUnit draftProperties = targetDraft->AcquireDraftProperties(ev);
#endif
        ODSetULongProp(ev, draftProperties, kODPropOriginalDraft, kODULong, (ODULong) originalDraft);

	// Save the process ID to be used by the drop/paste site. 
        // If the process ID in the drag/cut site is the same as the process 
        // ID in the drop site, then drag-n-drop/cut-n-paste happens within 
        // the same document.
        int pid = getpid();
        ODSetULongProp(ev, draftProperties, kODPropOriginalPID, kODULong, (ODULong) pid);

#ifdef _NO_TMP_OBJS_
        draftProperties->Release(ev);
#endif
   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// GetOriginalDraft
//------------------------------------------------------------------------------
// Returns kODNULL if the original draft is unknown.  This is the case when content
// was placed in the draft without cloning.

static ODDraft* GetOriginalDraft(Environment* ev, ODDraft* draft)
{
     ODDraft* retDraft = kODNULL;
    try
    {
#ifdef _NO_TMP_OBJS_
        ODStorageUnit   *draftProperties = draft->AcquireDraftProperties(ev);
#else
        TempODStorageUnit       draftProperties = draft->AcquireDraftProperties(ev);
#endif
        // Retrieve the process ID belonging to the drag/cut site
        int originalPID = ODGetULongProp(ev, draftProperties, kODPropOriginalPID, kODULong);
      
        // If the process ID of the drag/cut site is the same as the process ID
        // of the drop/paste site, then drag-n-drop/cut-n-paste is within the 
        // same document.
        // If this is within the same document, then return the original draft. 
        // Otherwise, if it is across documents, then return kODNULL as
        // the original draft.
        if (originalPID == getpid())
           retDraft =  (ODDraft*)ODGetULongProp(ev, draftProperties, kODPropOriginalDraft, kODULong);
        else
           retDraft = kODNULL;
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
        return retDraft;
}

//------------------------------------------------------------------------------
// OriginalCloneKindExists
//------------------------------------------------------------------------------

static ODBoolean OriginalCloneKindExists(Environment* ev, ODDraft* draft)
{
        ODBoolean  OrgCloneKindExists = kODFalse;
   try
   {
#ifdef _NO_TMP_OBJS_
        ODStorageUnit *draftProperties = draft->AcquireDraftProperties(ev);
#else
        TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);
#endif
        OrgCloneKindExists =  draftProperties->Exists(ev, kODPropOriginalCloneKind, kODULong, 0);
   }
   catch (ODException _exception)
   {
      ODSetSOMException(ev, _exception);
   }
     return OrgCloneKindExists;
}

//------------------------------------------------------------------------------
// SetOriginalCloneKind
//------------------------------------------------------------------------------

static void SetOriginalCloneKind(Environment* ev, ODDraft* targetDraft, ODCloneKind cloneKind)
{
   try
   {
#ifdef _NO_TMP_OBJS_
        ODStorageUnit *draftProperties = targetDraft->AcquireDraftProperties(ev);
#else
        TempODStorageUnit draftProperties = targetDraft->AcquireDraftProperties(ev);
#endif
        ODSetULongProp(ev, draftProperties, kODPropOriginalCloneKind, kODULong, (ODULong) cloneKind);
#ifdef _NO_TMP_OBJS_
        draftProperties->Release(ev);
#endif
   }
   catch (ODException _exception)
   {
       ODSetSOMException(ev, _exception);
   }
    
}

//------------------------------------------------------------------------------
// CMDraft: GetHeap
//------------------------------------------------------------------------------

SOM_Scope ODMemoryHeapID  SOMLINK CMDraftGetHeap(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetHeap");

        return _fHeap;
}

//------------------------------------------------------------------------------
// CMDraft: CreateLinkIterator
//------------------------------------------------------------------------------

SOM_Scope ODLinkIterator*  SOMLINK CMDraftCreateLinkIterator(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateLinkIterator");

        CMLinkIterator* iter = kODNULL;
        ODVolatile(iter);
        try
        {
                iter = new CMLinkIterator();
                THROW_IF_NULL(iter, kODErrOutOfMemory);
                iter->InitCMLinkIterator(ev, somSelf);

        }
        catch (ODException _exception)
        {
           ODSetSOMException(ev, _exception);
           ODDeleteObject(iter);
         }


        return (ODLinkIterator*) iter;
}

//------------------------------------------------------------------------------
// CMDraft: CreateLinkSourceIterator
//------------------------------------------------------------------------------

SOM_Scope ODLinkSourceIterator*  SOMLINK CMDraftCreateLinkSourceIterator(CMDraft *somSelf, Environment *ev)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreateLinkSourceIterator");

        CMLinkSourceIterator* iter  = kODNULL;
        ODVolatile(iter);
        try
        {
                iter = new CMLinkSourceIterator();
                THROW_IF_NULL(iter, kODErrOutOfMemory);
                iter->InitCMLinkSourceIterator(ev, somSelf);

        }
        catch (ODException _exception)
        {
           ODSetSOMException(ev, _exception);
           ODDeleteObject(iter);

        }

        return (ODLinkSourceIterator*) iter;
}

//------------------------------------------------------------------------------
// CM_itoa   KP
//------------------------------------------------------------------------------
static void CM_itoa(ODULong number, ODSByte* cstring)
{
        ODULong i = 0;

        do {
                cstring[i++] = (ODSByte) (number % 10 + '0');
        } while ((number /= 10) > 0);
        cstring[i] = '\0';

        ODSByte c;

        ODULong j;

        for (i = 0, j = strlen(cstring)-1; i < j;i++, j--) {
                c = cstring[i];
                cstring[i] = cstring[j];
                cstring[j] = c;
        }
}

//------------------------------------------------------------------------------
// NewCMStorageUnit
//------------------------------------------------------------------------------

static CMStorageUnit* NewCMStorageUnit(ODMemoryHeapID heapID)
{
        CMStorageUnit*  cmStorageUnit = kODNULL;
        Environment* ev = somGetGlobalEnvironment();
    try
    {
#ifdef _PLATFORM_MACINTOSH_
        SOMClass*       cmStorageUnitClass = somNewClassReference(CMStorageUnit);
        ODULong         size = cmStorageUnitClass->somGetInstanceSize();
        ODPtr           buffer = ODNewPtr(size, heapID);
        cmStorageUnit = (CMStorageUnit*) cmStorageUnitClass->somRenew(buffer);
        somReleaseClassReference ( cmStorageUnitClass );
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
//      The use of heap is not implemented in Windows platform
        cmStorageUnit  = new CMStorageUnit;
#endif
    }
    catch (ODException _exception)
    {
      ODError error = ErrorCode();
      SaveEv();
      try
      {
        ODDeleteObject(cmStorageUnit);
      }
      catch(ODException _exception)
      {
          SetErrorCode(kODNoError);
      }
      RestoreEv();
      ODSetSOMException(ev, error);
    }

        return cmStorageUnit;
}

//------------------------------------------------------------------------------
// PurgeAllStorageUnits
//------------------------------------------------------------------------------

static ODULong PurgeAllStorageUnits(Environment* ev, OpenHashTable* storageUnits, IDList* idList)
{
        ODULong                                 runningTotal = 0;
   try
   {
        ODID                                    id;
        ODStorageUnit*                  su;

// idList is used to indicate whether we need to remove linkage between id and cmObject

        OpenHashTableIterator   suIter(storageUnits);
        for (suIter.First(&id, &su); suIter.IsNotComplete(); suIter.Next(&id, &su)) {
                if (su->GetRefCount(ev) != 0) {
                        if (idList) // purge should release CMObject, signal that by removing from idList
                                idList->Remove(id);
                        runningTotal += su->Purge(ev, 0);
                }
        }
/*
        for (suIter.First(&id, &su); suIter.IsNotComplete(); suIter.Next(&id, &su)) {
                if (su->GetRefCount(ev) == 0) {
                        suIter.RemoveCurrent();
                        delete su;
                }
        }
*/
        storageUnits->ShrinkToFit(/*extraSlots*/ 0);
    }
    catch (ODException _exception)
    {
         runningTotal = 0;
         ODSetSOMException(ev, _exception);
    }

        return runningTotal;
}

//------------------------------------------------------------------------------
// PurgeClasses
//------------------------------------------------------------------------------

static void PurgeClasses(Environment* ev, OpenHashTable* partClasses)
{
    try
    {
        OpenHashTableIterator   classes(partClasses);
        SOMClass*                               partClass;
        ODULong                                 count;

#ifdef ODDebug_Unloading_Classes
        PRINT("**** Purging classes.....\n");
#endif

        for (classes.First(&partClass, &count); classes.IsNotComplete(); classes.Next(&partClass, &count)) {
#ifdef ODDebug_Unloading_Classes
                if (somIsObj(partClass)) {
                        corbastring className = partClass->somGetName();
                        PRINT("Class %x %s count %d\n", partClass, className, count);
                }
                else {
                        PRINT("%x is not a valid som object.\n", partClass);
                }
#endif
                if (count == 0) {
//                      SOMClassMgrObject->somUnregisterClass(partClass);
                        classes.RemoveCurrent();
                }
        }
    }
    catch (ODException _exception)
    {
       ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDraft: PartInstantiated
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftPartInstantiated(CMDraft *somSelf, Environment *ev,
                ODPart* realPart)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","PartInstantiated");

        try
        {

        SOMClass* partClass = realPart->somGetClass();

#ifdef ODDebug_Unloading_Classes
                        corbastring className = partClass->somGetName();
                        PRINT(">>>> Instantiating %s\n", className);
#endif

        ODULong count = 1;
        if (_fPartClasses->GetValue(&partClass, &count)) {
                ++count;
                _fPartClasses->ReplaceEntry(&partClass, &count);
        }
        else {
                _fPartClasses->ReplaceEntry(&partClass, &count);
        }

        }
        catch (ODException _exception)
        {
            ODSetSOMException(ev, _exception);
        }
}

//------------------------------------------------------------------------------
// CMDraft: PartDeleted
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftPartDeleted(CMDraft *somSelf, Environment *ev,
                ODPart* realPart)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","PartDeleted");

        try
        {

        SOMClass* partClass = realPart->somGetClass();

#ifdef ODDebug_Unloading_Classes
                        corbastring className = partClass->somGetName();
                        PRINT("<<<< Deleting %s\n", className);
#endif

        ODULong count = 0;
        if (_fPartClasses->GetValue(&partClass, &count))
        {
                if (count > 0) {
                        --count;
                        _fPartClasses->ReplaceEntry(&partClass, &count);
                }
                else
                        WARNMSG_DEBUG(WARN_INDEX(-1),
                        "Instances of class %s have been deleted. But %x is found.",
                        partClass, realPart);
        }

        }
        catch (ODException _exception)
        {
            ODSetSOMException(ev, _exception);
        }
}

//------------------------------------------------------------------------------
// CMDraft: SwapPart
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDraftSwapPart(CMDraft *somSelf, Environment *ev,
                ODPart* part)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","SwapPart");

        try
        {
                OpenHashTableIterator   i(_fPersistentObjects);
                ODStorageUnitID                 id;
                ODPersistentObject*             object;
                ODFrame*                                frame;

                somSelf->Externalize(ev);

                for (i.First(&id, &object); i.IsNotComplete(); i.Next(&id, &object)) {
                        if ( strcmp(object->somGetClassName(), kFrameClassName)==0 ) {
                                frame = (ODFrame*)object;
                                if ( frame->IsSubframe(ev) == kODFalse )
                                        frame->PrepareToSwap(ev, part);
                        }
                }
        }
        catch (ODException _exception)
        {
            ODSetSOMException(ev, _exception);
        }
}


#ifdef _PLATFORM_OS2_
//------------------------------------------------------------------------------
// CMDraft: CreatePartLinkSourceIterator
//------------------------------------------------------------------------------

SOM_Scope ODLinkSourceIterator*  SOMLINK CMDraftCreatePartLinkSourceIterator(CMDraft *somSelf, Environment *ev, ODPart* part)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreatePartLinkSourceIterator");

    CMLinkSourceIterator* linkSourceIter = kODNULL;

  try
  {
    linkSourceIter  = new CMLinkSourceIterator();
    linkSourceIter->InitCMLinkSourceIterator(ev, somSelf);

    _fLinkService->GetLinkSourcesForPart(ev,part,linkSourceIter);
  }
  catch (ODException _exception)
  {
      ODSetSOMException(ev, _exception);
      ODDeleteObject(linkSourceIter);
  }

    return (ODLinkSourceIterator*)linkSourceIter;
}

//------------------------------------------------------------------------------
// CMDraft: CreatePartLinkIterator
//------------------------------------------------------------------------------

SOM_Scope ODLinkIterator*  SOMLINK CMDraftCreatePartLinkIterator(CMDraft *somSelf, Environment *ev, ODPart* part)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","CreatePartLinkIterator");

    CMLinkIterator* linkTargetIter = kODNULL; 
 
  try
  {
    linkTargetIter = new CMLinkIterator();
    linkTargetIter->InitCMLinkIterator(ev, somSelf);

    _fLinkService->GetLinkTargetsForPart(ev,part,linkTargetIter);
  }
  catch (ODException _exception)
  {
      ODSetSOMException(ev, _exception);
      ODDeleteObject(linkTargetIter);
  }

    return (ODLinkIterator*)linkTargetIter;
}

// *=============================================================================
// * CMDraft: GetLinkSrcForAvlSvr
// *=============================================================================

SOM_Scope ODLinkSource*
SOMLINK CMDraftGetLinkSrcForAvlSvr(CMDraft *somSelf,
                                       Environment *ev,
                                       ODStorageUnitID id,
                                       IODAuxiliaryState *auxState)
{
    // We are assuming that this is only called on an id
    // of a linksource which has not yet been internalized.
    // This call is only made on surrogate drafts.
    // The availserver is responsible for releasing the usecount.
    // This is like AcquireLinkSource but no call is made to the LinkService.
    // It is impossible for the linkid to change since the
    // availability server only knows about links after registration.
    CMDraftData *somThis = CMDraftGetData(somSelf);
    CMDraftMethodDebug("CMDraft","GetLinkSrcForAvlSvr");

    IODLinkSource* linkSource = (IODLinkSource*)kODNULL;

    try
    {


    linkSource = ((IODLinkSource*)somSelf->RetrievePersistentObject(ev, id));

    if (linkSource == (IODLinkSource*)kODNULL)
    {
      linkSource = new IODLinkSource();
      if (linkSource == (IODLinkSource*)kODNULL)
      {
        THROW(kODErrCannotAcquireLink);
      }

      ODStorageUnit* su = (ODStorageUnit*)kODNULL;
      ODVolatile(su);
      ODVolatile(id);

      try
      {

        su = somSelf->AcquireStorageUnit(ev, id);
        _fPersistentObjects->ReplaceEntry(&id, &linkSource);

          linkSource->InitILinkSourceFromStorage(ev, su);
          linkSource->CompleteInitILinkSource(ev, su, 0, auxState, kODFalse, (ODISOStr)"");
      }
      catch (ODException _exception)
      {

         ODError error = ErrorCode();
         SaveEv();
         try
         {

            if (su != (ODStorageUnit*) kODNULL)
            {
               _fPersistentObjects->RemoveEntry(&id);
               su->Release(ev);
            }

            linkSource->ReleaseAll(ev);
            delete linkSource;
         }
         catch(ODException _exception)
         {
               SetErrorCode(kODNoError);
         }
         RestoreEv();
         ODSetSOMException(ev, error);
         throw;

      }
    }

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
        linkSource =  kODNULL;
    }
    return (ODLinkSource*)linkSource;
}
#endif /* _PLATFORM_OS2_ */
