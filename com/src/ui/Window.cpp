/* @(#)Z 1.50 com/src/ui/Window.cpp, odui, od96os2, odos29646d 96/11/15 15:28:28 (96/11/05 06:37:29) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odui
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

/********************************************************************/
/*  Licensed Materials - Property of IBM                            */
/*                                                                  */
/*                                                                  */
/* Copyright (C) International Business Machines Corp., 1994.       */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/*  US Government Users Restricted Rights -                         */
/*  Use, duplication, or disclosure restricted                      */
/*  by GSA ADP Schedule Contract with IBM Corp.                     */
/*                                                                  */
/*  IBM Change History (most recent first):                         */
/*   21285 04/19/96 brr Removed #include CmdDefs.xh                 */
/*          2/20/96  jrb Complete Apple DR4 Integration             */
/*  143654 11/13/95  ced Apple DR4 IDL changes; SetShouldDispose    */
/*                       method removed, souldDispose param added   */
/*                       to InitWindow and InitWindowForFrame.      */
/*  140083  11/6/95  pfe InitWindow THROWing too late on null rootpart */
/*  131165  9/29/95  st  changes to window title/window list entry  */
/*  133968  8/24/95  pfe Port DR3 Apple code                        */
/*  133596  8/18/95  SS  Comment ifdef MULTITHREADING to enable     */
/*                       docshell multithreading                    */
/*  133504  8/18/95  aht Build break in new build tree              */
/*  129922  07/19/95 SS  Change ODWindowUpdate to take an ODEventData */
/*                       parameter; Update gets updateregion from   */
/*                       event data.                                */
/*  117197  06/20/95 BLG Store properties correctly when save file  */
/*                       and restore it correctly when opening it   */
/*  123223  05/24/95 dcp Added exception in InitWindow for          */
/*                       null rootPart.                             */
/*  119388  4/29/95 st   changes for menu code - already done       */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*  119874  04/21/95 ced Fix GetFacetUnderPoint for non-clipped     */
/*                       overlapping sibling facets.                */
/*  <OS2>   8/16/94 MAP  Port A8C3                                  */
/*                                                                  */
/********************************************************************/
/*
        File:           Window.cpp

        Contains:       Implementation of class ODWindow

        Owned by:       Richard Rodseth

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <80>    10/18/95        JBS             1288922 in ::Update, move DrawActiveBorder
                                                                        to after EndUpdate
                <79>    10/17/95        jpa             &RCR: 1292659: Catch/ignore part exceptions
                                                                        returned from handling activate/suspend
                                                                        events. 1289153: Don't crash if opening
                                                                        window fails.
                <78>    10/16/95        RR              #1293067 Pass shouldDispose to
                                                                        RegisterWindow
                <77>    10/10/95        DM              1277216: GM:API: Dont return ODPoint from
                                                                        GetWindowPoint()
                <76>     10/8/95        TJ              Fixes Recomended by Refball
                <75>     10/3/95        TJ              Changes done by RefBall Team
                <74>     9/21/95        JBS             1284283 set _fRootFrame to NULL after
                                                                        Close-ing or Remove-ing it
                <73>     9/13/95        DM              1277216 GM:API return no ODPoints nor
                                                                        ODPolygons
                <72>     9/12/95        RR              #1282498 Removed CanvasChanged call in
                                                                        Close() and CloseAndRemove(). #1280629
                                                                        Catch outofmemory error in ODDrawGrowIcon
                <71>      9/8/95        jpa             Renumbered private err codes [1281354]
                <70>      9/8/95        T‚              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
                <69>      9/6/95        RR              #1236387 Don't draw active border in
                                                                        background
                <68>      9/1/95        RR              # 1279100/1280338 Added fIsActive. Rewrote
                                                                        IsActive and Select
                <67>     8/29/95        RR              #1267251 Pass synthesized activate events
                                                                        to Dispatcher  #1278308 Remove THROW in
                                                                        SetStorageUnit
                <66>     8/26/95        T‚              1274606 FB2: Patching Remarks
                <65>     8/21/95        RR               #1246478 Disabled fDIrty optimization.
                                                                        #1243157 Changed GetFacetUnderPoint
                                                                        iteration order. #1273596 Set _fStorageUnit
                                                                        in InitWindowForFrame
                <64>     8/15/95        RR              # 1264461/1262903 Added ODDrawGrowIcon.
                                                                        1267238 Fixed ASSERT in InitWindow. 1263706
                                                                        Indentation.
                <63>     8/12/95        T‚              1276807 Opt./Bug: use StdTypIO routines for
                                                                        portable streaming & smaller footprint,
                                                                        1276806 Optimization: use kODFalse instead
                                                                        of kODTrue in comparisons
                <62>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <61>     7/26/95        DM              #1270320: Memory leak fixes: delete
                                                                        rootCanvas in Close and CloseAndRemove
                <60>     6/30/95        RR              1242642 BB Ref counting fixes. Hide calls
                                                                        HideODWindow only if shown.
                <59>     6/28/95        RR              1242642 BB Mostly ref counting. Acquire and
                                                                        Release fDraft
                <58>     6/25/95        T‚              1242642 BB: Turn on ODDebug warning if
                                                                        refcount is wrong in
                                                                        ODRefCntObjectsomUninit.
                <57>     6/23/95        JBS             1261323 Draft::CreateFrame: add isSubframe
                                                                        param, remove isRoot
                <56>     6/15/95        RR              #1241680 Don't call somFree in InitWindow
                <55>      6/8/95        RR              #1257260 Collapse base classes. #1214898
                                                                        StdTypes.r -> ODTypes.r
                <54>     5/31/95        RR              #1251403 Release after AcquirePart
                <53>     5/30/95        CG              #1252280 BB: PStrToIntl should not  be used
                                                                        by OpenDoc.
                <52>     5/26/95        RR              #1251403: Multithreading naming support
                <51>     5/25/95        jpa             Fixed usage of ODDebug. [1253321]
                <50>     5/19/95        RR              #1249981 Added (Set)ShouldDIspose #1251128
                                                                        WARN if shouldSave is set and root frame is
                                                                        non-persistent
                <49>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
                                                                        refcount. #1248624 Error updating window
                                                                        causes endless alerts
                <48>     5/10/95        JBS             1224727 Facet hit-testing calls should take
                                                                        point param in frame coords;1227547
                                                                        Coordinate system for ODWindow::Update and
                                                                        ODFacet::Update
                <47>      5/2/95        RR              # 1244133 Fixed SetDefaultWindowTitles to
                                                                        avoid extra bogus characters. Also
                                                                        CloseAndRemove calls frame->Close if draft
                                                                        is readonly
                <46>     4/28/95        RR              1211085 Remove 5$ comments
                <45>     4/14/95        T‚              #1235279 BB: InfoUtil & StdTypIO functions
                                                                        should take Environment* and SU* when
                                                                        possible
                <44>     4/13/95        RR              # 1216618 Added ODVolatile, use
                                                                        ODDeleteObject
                <43>      4/7/95        RR              #1216618 Added SOM_TRY etc.
                <42>      4/6/95        RR              #1211085 Remove 5$ comments
                <41>      4/4/95        RR              # 1220228 Use Factory methods, #1211085
                                                                        SetSourceFrame ref counting
                <40>     3/31/95        T‚              1223505 BB: Opening and creating documents
                                                                        on read only media.  Fixed Creation of
                                                                        Non-persistent frames on readonly drafts.
                <39>     3/22/95        RR              #1225420, 1227993 Added private
                                                                        InitWindowForFrame
                                                                        Removed InitWindowFromStorage
                                                                        Modified Externalize
                <38>     3/10/95        RR              # 1220199 Externalize source frame
                                                                        properly. 1226379 Call ODNewRgn. Dispose
                                                                        growRgn.
                <37>      3/7/95        RR              # 1220929  Added fSelectionFocus #1212490
                                                                        Added fInBackground
                <36>      3/1/95        RR              #1205622 Support re-opening an open draft
                                                                        by making ::Open a no-op if the window is
                                                                        already open.
                <35>     2/28/95        VL              1194656: Commented out extraenous AddValue.
                <34>     2/24/95        jpa             Use ODNewRgn. [1220810]
                <33>     2/22/95        RR              # 1220228 Use factory methods
                <32>     2/20/95        T‚              #1221861 BB: Incomplete AOCE support needs
                                                                        to be removed from OpenDoc
                <31>     2/15/95        JBS             1198600 don't release rootFrame after
                                                                        close, private field fShouldAdjustOnOpen
                <30>     2/10/95        RR              # 1218467 Refcount source frame. Remove
                                                                        root part field, and add root part
                                                                        parameter to CreateRootFrame
                <29>     1/26/95        eeh             1214080: OpenDoc really uses kODMacIText,
                                                                        not kODIntlText.
                <28>     1/26/95        VL              #???: Use updated Storage Unit Ref API.
                <27>     1/13/95        RR              Pass eventInfo to HandleEvent
                <26>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                <25>     11/1/94        RR              #1196759 Dispose of window pointer, unless
                                                                        it's the mailer (check was wrong)
                <24>     9/30/94        RR              #1183610 Add and use fIsDirty
                <23>     9/29/94        eeh             #1154961: remove local def of
                                                                        kODPropWindowMailerIsExpanded
                <22>     9/29/94        RA              1189812: Mods for 68K build.
                <21>     9/22/94        JBS             1188214: coordinate bias implementation
                <20>     9/22/94        eeh             #1154961 'ternalize whether mailer was
                                                                        expanded; use this info in creating root
                                                                        facet.
                <19>      9/1/94        RR              #1181184 Externalize bounds as ODRect and
                                                                        title as IText, and use StdTypIO routines,
                                                                        #1167957 Fixed GetProcID so that all window
                                                                        types are correctly ternalized, #115685
                                                                        Don't  invalidate content region when
                                                                        resizing. Don't Erase when Updating.
                <18>     8/31/94        T‚              #1183129, #1183116, #1183119, #1183111:
                                                                        Lots of ErrorCode cleanup.
                <17>     8/29/94        RR              #1156187,1171772,1178673,1183062,1171771
                <16>     8/26/94        VL              1183174: Use updated cloning APIs.
                <15>     8/19/94        T‚              #1180922 Need to Stop using obsolete types
                                                                        (kOD ID)
                <14>     8/15/94        JBS             1181138: add frameType to CreateFrame();
                                                                        1181156: UI API Cleanup
                <13>     8/15/94        T‚              #1180922 Removed most obsolete types from
                                                                        StdTypes.idl
                <12>     8/15/94        VL              #???: Added frameType (kODNULL) parameter
                                                                        to CreateFrame.
                <11>     8/12/94        JBS             1179919: add biasCanvas param to
                                                                        Draft::CreateFrame()
                <10>     8/12/94        JBS             1179920: move fWindow from Facet to Frame
                 <9>     8/10/94        JBS             1179919: coordinate system bias changes
                 <8>      8/4/94        eeh             bug 1179054: in ODWindowIsActive check for
                                                                        null frontwindow
                 <7>      8/3/94        VL              1153123: Storage to ODStor.
                 <6>     7/27/94        eeh             Changes necessitated by WinStat.idl changes
                 <5>     7/26/94        eeh             ODQDPoint->Point, no more lying to SOM
                 <4>     7/21/94        eeh             Rect param to FixUpMailerWindow -> Rect*
                 <3>      7/5/94        VL              Fixed parameters to CreateFrame.
                 <2>     6/28/94        RR              First conversion
                 <1>     6/28/94        RR              first checked in
                 <0>     6/27/94        SV              SOMverted
                <11>      4/6/94        JBS             1155477, 1155480
                <10>     3/28/94        CG              1153547: Renamed XMPSessn.h to XMPSessM.h
                 <9>     3/25/94        JA              Added missing #include (1147588)
                 <8>     3/18/94        CC              Include ConstDef.h to access kXMPAppleTEXT.
                                                                        (1151636)
                 <7>     3/16/94        eeh             bug #1149649: InitWindowFromStorage now
                                                                        calls MakeWindowIntoReply
                 <6>      3/9/94        eeh             bug #1146749: assign TRUE to isMailerAware
                                                                        by default for root windows
                 <5>     2/22/94        RR              #1145944 Call Release() as the last thing
                                                                        in CloseAndRemove().
                 <4>     2/22/94        VL              Release Window after fRootFrame is
                                                                        released.
                 <3>     2/17/94        JBS             #114???? fix inconsistent facet coords
                 <2>     2/16/94        JA              Include new AltPoint.h.
                <91>      2/7/94        NP              Tiger Team doings.
                <90>      2/7/94        NP              Tiger Team doings.
                <89>      2/7/94        JA              Utility.h --> XMPUtils.h
                <88>      2/7/94        NP              Tiger Team doings.
                <87>      2/3/94        CG              Session reorg.
                <86>      2/2/94        RR              Removed calls to SetRoot
                <85>      2/2/94        JBS             new AddDisplayFrame recipie
                <84>      2/1/94        JA              Removed use of obsolete typedef in
                                                                        CreateRootFrame.
                <83>      2/1/94        CC              Added ShouldShowLinks(),
                                                                        SetShouldShowLinks(), and code to read &
                                                                        write the setting.
                <82>     1/28/94        RR              macro for ASLM qd globals
                <81>     1/27/94        RR              setwtitle->SetWTitle
                <80>     1/26/94        RR              Call Dispatcher::InvalidateFacetUnderMouse
                <79>     1/25/94        RR              Removed redundant fWindowWasVisible. Moved
                                                                        Release from WinState::RemoveWindow to
                                                                        ::Close. Open no longer calls Show.
                <78>     1/24/94        SS              Update for new InitCanvas method
                <77>     1/24/94        eeh             moved AOCEObj init to CommonInitWindow
                <76>     1/21/94        RR              Added shouldSave to InitWindow
                <75>     1/19/94        RR              Removed redundant methods for drawing
                                                                        active border
                <74>     1/18/94        RR              Remove const from Drag. Use IsShown rather
                                                                        than AcquireWindowVisible
                <73>     1/15/94        VL              Release fRootFrame after closing it.
                <72>     1/15/94        RR              WinState.h -> WinStat.h, Dispatch.h ->
                                                                        Disptch.h
                <71>     1/14/94        RR              Added utility GetWindowPoint (Private to UI
                                                                        subsystem)
                <70>     1/13/94        eeh             integrated AOCEObj
                <69>     1/12/94        RR              Added CloseAndRemove
                <68>     1/11/94        eeh             made mailer menu heierarchical
                <67>     1/11/94        T‚              Init... changes
                <66>     1/10/94        RR              Added SetWindowTitle
                <65>    12/22/93        RR              Added Get/SetID
                <64>    12/21/93        VL              Changed ODStorageUnit::GetValueSize to
                                                                        ODStorageUnit::GetSize.
                <63>    12/21/93        JBS             InitFacet changes
                <62>    12/21/93        JBS             fix constructor and CreateRootFrame to
                                                                        CreateFrame with viewType & presentation
                <61>    12/20/93        VL              Made ODWindow a non-persistent object.
                <60>    12/17/93        VL              Fixed warning on ULong to UChar.
                <59>    12/16/93        RR              Respect IsSelected as well as IsFrozen.
                                                                        Call DisposeDialog if it's a dialog.
                <58>    12/15/93        JBS             back pointer in facet not frame
                <57>    12/14/93        EL              Use text instead of Str255.
                <56>    12/14/93        T‚              InitWindow changes.  Internalize,
                                                                        Initialize went away.
                <55>     12/8/93        VL              Used new WindowState names.
                <54>     12/6/93        RR              Added SuspendResume
                <53>     12/2/93        RR              Fixed bug in AcquireFrameUnderPoint
                <52>     12/1/93        VL              Added floating window support. Changed code
                                                                        for Show, Hide, IsActive. Added Select and
                                                                        Drag. Made window invisible when it is
                                                                        first created.
                <51>    11/24/93        RR              Moved CreateRootFrame to
                                                                        InitializePersistent
                <50>    11/23/93        JA              Changed <MenuBar.h> to <MenuBar.xh>.
                <49>    11/18/93        JBS             use activeFrame->AcquireWindow(ev), ensure
                                                                        fRootFrame-SetWindow(this)
                <48>    11/18/93        eeh             added SetTempMailerWindow call to
                                                                        RemoveAECEMailer
                <47>    11/18/93        T‚              call inherited::InitializePersistent()
                                                                        ::Externalize()
                <46>    11/18/93        eeh             changed Internalize, Resized and
                                                                        CreateRootFacet to open mailer docs
                                                                        correctly; added GetMailerOffset
                <45>    11/17/93        RR              Added accessors for shouldSave
                <44>    11/15/93        eeh             various tweaks to support saving and
                                                                        opening docs with mailer attached
                <43>    11/12/93        JBS             use Facets
                <42>     11/1/93        VL              Used Strong References.
                <41>    10/29/93        RR              EnDisableCommand -> EnableCommand
                <40>    10/29/93        T‚              changed FixUpMailerWindow to use a Str255
                                                                        not a Str255* to pacify CFront
                <39>    10/27/93        RR              AcquireFrameUnderPoint takes point in window
                                                                        coordinates
                <38>    10/27/93        eeh             added kODPropWindowHasMailer property to
                                                                        in/externalize, changed AOCEMailerIsActive
                                                                        to AOCEMailerIsPresent, and added call to
                                                                        FixUpMailerWindow to allow opening mailer
                                                                        documents.
                <37>    10/25/93        RR              Use kODStr255 for window title
                <36>    10/22/93        RR              GetProcID returns a short, not a long.
                                                                        Removed unused variable
                <35>    10/21/93        RR              Added floating, resizable and rootWindow
                                                                        properties and their accessors. Implemented
                                                                        Show and Hide. Externalize new properties.
                                                                        Hide window in Close. Create window
                                                                        invisible in Internalize. etc. etc.
                <34>    10/20/93        eeh             fixed problems with menus on non-AOCE
                                                                        machines
                <33>    10/20/93        eeh             Changes in UpdateMailerMenus() and
                                                                        elsewhere required by new menu stuff.
                <32>    10/14/93        RR              Add source frame parameter to
                                                                        AddDisplayFrame call
                <31>    10/14/93        JA              ODShape fu: No longer returns copies from
                                                                        GetPlatformShape. Deal w/it.
                <30>    10/12/93        eeh             Made mailer property persistent, and other
                                                                        mailer changes.
                <29>     10/8/93        VL              Added Release().
                <28>     10/7/93        RR              Added fRootPart and added rootPart to
                                                                        constructor. Renamed CreateRootFrame
                                                                        ->CreateLayout
                <27>     10/7/93        eeh             Added methods for AOCE mailer support.
                <26>     10/6/93        JA              Use new fixed-point XMPPoint.
                <25>     10/5/93        T‚              added #ifdef SSREMOVEWORKAROUND until
                                                                        su-> Remove(ev,ev) is fixed
                <24>     10/4/93        RR              Define persistent properties in
                                                                        InitializePersistent, and ASSERT their
                                                                        presence in Internalize/Externalize
                <23>     10/4/93        RR              Internalize/Externalize all window
                                                                        properties
                <22>     9/30/93        T‚              check to make sure fLayout­NIL before
                                                                        trying to Close() and Release() it
                <21>     9/29/93        T‚              with RCR, changed Close() to Close Layout
                <20>     9/29/93        VL              Added InitializePersistent.
                <18>     9/23/93        T‚              fixed a minor bug, was deleting fLayout
                                                                        which is now a PstObj and responsibility of Draft
                <17>     9/22/93        RR              Added Open() method
                <16>     9/21/93        PH              Remove fDragInfo GetDragInfo
                                                                        De/RegisterDroppableFrame
                <15>     9/17/93        JBS             change Frame::Draw() call - no canvas param
                <14>     9/16/93        JA              Updated shape/transform calls.
                <13>     9/10/93        RR              Copy prototype code over
                <12>     9/10/93        JBS             remove fCanvas, GetCanvas(), ChangeCanvas()
                <11>     8/24/93        RR              Got rid of some CFront warnings
                <10>     8/19/93        RR              Temporary hacks for "Real" shell
                 <9>     8/11/93        T‚              move type & property constants to StdProps
                                                                        & StdTypes
                 <8>      8/4/93        SS              Field name change for new drag mgr code
                 <6>     7/9/93     Added some minimal implementation
                 <5>     7/9/93     RCR         Rename -> ODWindow
                 <4>     6/23/93        RCR             Deregister -> Unregister
                 <3>     4/29/93        RCR             Rename includes
                 <2>     4/26/93        RCR             Added missing Initialize/Purge
                 <1>     4/9/93         RCR             Initial skeleton

        To Do:

        In Progress:

*/

#ifdef DEBUG
// By defining this macro, SOM doesn't define it and I am able to use the
// MyMethodDebug as a break point for IPMD debugging
#define ODWindowMethodDebug(c,m) MyMethodDebug(c,m)
static void MyMethodDebug(char *c, char *m);
#endif

#define ODWindow_Class_Source

#ifndef _ALTPOINT_
#include "AltPoint.h"                   // Use C++ savvy XMPPoint and XMPRect
#endif

#ifdef _PLATFORM_OS2_
#include <ODos2.h>
#endif

#ifdef _PLATFORM_WIN32_
#ifndef _WINDOWS_
#include <windows.h>
#endif
#endif

#ifdef _PLATFORM_UNIX_
#include <Xm/XmAll.h>
#endif

#ifndef SOM_ODWindow_xih
#define VARIABLE_MACROS
#include <Window.xih>
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODDispatcher_xh
#include <Disptch.xh>
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif


#ifndef SOM_ODFacetIterator_xh
#include <FacetItr.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODCanvas_xh
#include <Canvas.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#if 0
#ifdef _PLATFORM_WIN32_
#ifndef _ODUTILW_
#include <ODUtilW.h>
#endif
#endif
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __RESOURCES__
#include <Resources.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>    // SysBeep etc.
#endif
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#ifndef _WINUTILM_
#include "WinUtilM.h"
#endif

#ifndef _WINUTILS_
#include "WinUtils.h"
#endif

// Includes for NLS support for window title
#include <ODMessag.h> // Header file for catalog IDs
#include <stdio.h>    // Used for sprintf for window title string

#ifndef _ODMATH_
#include <ODMath.h>
#endif
#endif

#ifdef _PLATFORM_OS2_
#ifndef _OS2WCANV_
#include <OS2WCanv.xh>
#endif
#endif

#ifdef _PLATFORM_WIN32_
#ifndef _WINWCANV_
#include <WinWCanv.xh>
#endif
#ifndef _OLEMGRW_               //      for OLE wrapper
#include "OleMgrW.h"            //      for OLE wrapper
#endif                          //      for OLE wrapper
#ifndef SOM_DocumentManager_xh  //      for OLE wrapper
#include "DocMgr.xh"            //      for OLE wrapper
#endif                          //      for OLE wrapper
#ifndef _OLECTL_H_              //      for OLE wrapper
#include "olectl.h"             //      for OLE wrapper
#endif                          //      for OLE wrapper
#ifndef _OLEWRAP_H_             //      for OLE wrapper
#include "olewrap.h"            //      for OLE wrapper
#endif                          //      for OLE wrapper
#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>           //      for SetupDropTarget (until OLE Wrapper is ready)
#endif
#endif

#ifdef _PLATFORM_UNIX_
#ifndef _AIXWCANV_
#include <AIXWCanv.xh>
#endif
#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>           // for RegisterDropSite
#endif
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __OCESTANDARDMAIL__
#include <OCEStandardMail.h>
#endif

#ifndef __OCEERRORS__
#include <OCEErrors.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef SOM_ODMenuBar_xh
#include <MenuBar.xh>
#endif

//#ifndef SOM_Module_OpenDoc_Commands_defined
//#include <CmdDefs.xh>
//#endif

#ifdef _PLATFORM_OS2_
#ifndef _ODTYPESP_
#include "ODTypesp.h"
#endif
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _CONSTDEF_
#include "ConstDef.h"
#endif

#ifndef __GESTALTEQU__
#include <GestaltEqu.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _DOCUTILS_
#include <DocUtils.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _DSPUTILM_
#include "DspUtilM.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODWindow
#endif

#include "WindowB.cpp"  // Platform-independent methods, if any

#define ODDebugActivates 0

#ifdef DEBUG
// This method is only used in the debug case as a break point for IPMD
static void MyMethodDebug(char *c, char *m)
{
  SOMMethodDebug(c,m);
}
#endif

//==============================================================================
// Constants
//==============================================================================

// Check ErrorDef.idl before adding any new codes here, to make sure they're unique.
const ODError kODErrInvalidPlatformWindow       = -29828;
const ODError kODErrInvalidWindow                       = -29827;

// T‚: This error code was only being used in this file, and then only
// for checking the _fPlatformWindow field of a window at the beginning of
// methods.  It didn't seem to have anything to do with the api.
// That is why I made it private to this file.
// perhaps the error that should be thrown is kODErrObjectNotInitialized?
// kODErrInvalidWindow falls into the same category.


//==============================================================================
// Types
//==============================================================================

//=====================================================================================
// ODWindow Methods
//=====================================================================================


//-------------------------------------------------------------------------------------
// ODWindow: CommonInitWindow
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowCommonInitWindow(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","CommonInitWindow");

    SOM_TRY

                _fSession = (ODSession*) _fDraft->GetDocument(ev)->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev);
                _fArbitrator = _fSession->GetArbitrator(ev);
                _fSelectionFocus = _fSession->Tokenize(ev,kODSelectionFocus);

#ifdef _PLATFORM_UNIX_
                // AIXtodo - contingent on IsFloating attribute?
                // I don't think so, since we need all windows to be
                // able to be iconified separately.  This means we have
                // to use the TopLevelShell for each.

                // Cache the TopLevelShellWidget, the MainWindowWidget and the
                // DrawingAreaWidget

                // Convert the platformwindow to its toplevel shell widget
                XSync(_fSession->GetWindowState(ev)->GetDisplay(ev), False);
                _fTopLevelShellWidget =
                    XtWindowToWidget(_fSession->GetWindowState(ev)->GetDisplay(ev),
                                     _fPlatformWindow);

                // If converted to a toplevelshell widget
                if ((_fTopLevelShellWidget) && (XtIsTopLevelShell(_fTopLevelShellWidget)))
                {
                    // Get the toplevelshell's children
                    Cardinal count = 0;
                    WidgetList list;
                    XtVaGetValues(_fTopLevelShellWidget,
                                  XmNnumChildren, &count,
                                  XmNchildren, &list,
                                  NULL);

                    // If 1 item returned of type mainwindow widget
                    if ((count == 1) &&
                        (_fMainWindowWidget = list[0]) &&
                        (XmIsMainWindow(_fMainWindowWidget)))
                    {
                        // Get the drawing area widget
                        XtVaGetValues(_fMainWindowWidget,
                                      XmNworkWindow, &_fDrawingAreaWidget,
                                      NULL);

                        // If widget not of type drawing area
                        if (!XmIsDrawingArea(_fDrawingAreaWidget))
                        {
                           THROW(kODErrCannotCreateWindow);
                        }
                    }
                    else
                    {
                        THROW(kODErrCannotCreateWindow);
                    }
                }
                else
                {
                    THROW(kODErrCannotCreateWindow);
                }

                // perform additional DragAndDrop initialization
                ODDragAndDrop* dnd = _fSession->GetDragAndDrop(ev);
                if (dnd)
                    dnd->RegisterDropSite(ev, _fDrawingAreaWidget);
#endif

        SOM_CATCH_ALL
        SOM_ENDTRY
}


//-------------------------------------------------------------------------------------
// ODWindow: InitWindow
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowInitWindow (ODWindow *somSelf, Environment *ev,
                ODPlatformWindow        platformWindow,
                ODType                          frameType,
                ODBoolean                       isRootWindow,
                ODBoolean                       isResizable,
                ODBoolean                       isFloating,
                ODBoolean                       shouldSave,
                ODBoolean                       shouldDispose,
                ODPart*                         rootPart,
                ODTypeToken                     viewType,
                ODTypeToken                     presentation,
                ODFrame*                        sourceFrame)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","InitWindow");

        SOM_TRY

                /* Moved from somInit. SOM itself sets fields to zero
                _fPlatformWindow = kODNULL;
                _fRootFacet = kODNULL;
                _fSourceFrame = kODNULL;
                _fSession = kODNULL;
                _fIsRootWindow = kODTrue;
                _fShouldSave = kODTrue;
                _fIsResizable = kODFalse;
                _fIsFloating = kODFalse;
                _fWasVisible = kODFalse;
                _fShouldShowLinks = kODFalse;
                _fArbitrator = kODNULL;

                _fStorageUnit = kODNULL;
                _fIsDirty = kODFalse;
                _fDraft = kODNULL;

                _fShouldAdjustOnOpen = kODFalse;
                _fInBackground = kODFalse;
                _fIsActive = kODFalse;
                _fSelectionFocus = 0;
                */
                _fShouldDispose = shouldDispose;

                somSelf->InitRefCntObject(ev);

#if ODDebug
                if (shouldSave && ODISOStrEqual(frameType, kODNonPersistentFrameObject))
                        WARNMSG_DEBUG(WARN_INDEX(-1), "InitWindow: shouldSave is TRUE, but root frame is nonpersistent ");

#endif

//moved the following check up in the method (before someone tries to use it!)
//[140083] pfe
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_) // $$$$ dcp # 123223
        // If the rootPart parameter is null
        if ( rootPart == kODNULL )
        {
           // Throw an error
           THROW(kODErrIllegalNullPartInput);
        }
#endif

                _fPlatformWindow = platformWindow;
                _fIsRootWindow  = isRootWindow;
                _fIsResizable   = isResizable;
                _fIsFloating    = isFloating;
                if (sourceFrame)
                        sourceFrame->Acquire(ev);
                _fSourceFrame   = sourceFrame;
                _fDraft                 = rootPart->GetStorageUnit(ev)->GetDraft(ev);
                _fDraft->Acquire(ev);
                if (!HAS_WRITE_ACCESS(_fDraft->GetPermissions(ev)))
                {
                        shouldSave = kODFalse;
                        frameType = kODNonPersistentFrameObject;
                }

                _fShouldSave    = shouldSave;

                somSelf->CommonInitWindow(ev);

                somSelf->CreateRootFrame(ev, frameType, viewType, presentation, rootPart);

#if defined(_PLATFORM_OS2_) || (_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        // Initialize window title
        somSelf->InitWindowTitle(ev);
#endif

                _fIsDirty = kODTrue;
                _fShouldAdjustOnOpen = kODTrue;

#ifdef _PLATFORM_WIN32_
#ifdef OLE_WRAPPER_READY
        // register the window as a valid drop target for OLE drag-and-drop
        OleWrapper *pOleWrapper = ODOLEManager::GetShell()->GetActiveDocumentManager(ev)->GetOleWrapper(ev);
        if (pOleWrapper)
                pOleWrapper->SetupDropTarget(_fPlatformWindow);
#else
        // use the drag and drop manager's SetupDropTarget method
        ODDragAndDrop *pDragAndDrop = _fSession->GetDragAndDrop(ev);
        if (pDragAndDrop)
                pDragAndDrop->SetupDropTarget(ev, _fPlatformWindow);
#endif // OLE_WRAPPER_READY
#endif // _PLATFORM_WIN32_

        SOM_CATCH_ALL
        SOM_ENDTRY
}
//-------------------------------------------------------------------------------------
// ODWindow: InitWindowForFrame
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowInitWindowForFrame (ODWindow *somSelf, Environment *ev,
                ODPlatformWindow        platformWindow,
                ODFrame*                        frame,
                ODBoolean                       isRootWindow,
                ODBoolean                       isResizable,
                ODBoolean                       isFloating,
                ODBoolean                       shouldSave,
                ODBoolean                       shouldDispose,
                ODFrame*                        sourceFrame)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","InitWindow");

    SOM_TRY

                ASSERT(frame != kODNULL, kODErrInvalidFrame);

                /* Moved from somInit. SOM itself sets fields to zero
                _fPlatformWindow = kODNULL;
                _fRootFacet = kODNULL;
                _fSourceFrame = kODNULL;
                _fSession = kODNULL;
                _fIsRootWindow = kODTrue;
                _fShouldSave = kODTrue;
                _fIsResizable = kODFalse;
                _fIsFloating = kODFalse;
                _fWasVisible = kODFalse;
                _fShouldShowLinks = kODFalse;
                _fArbitrator = kODNULL;

                _fStorageUnit = kODNULL;
                _fIsDirty = kODFalse;
                _fDraft = kODNULL;

                _fShouldAdjustOnOpen = kODFalse;
                _fInBackground = kODFalse;
                _fSelectionFocus = 0;
                */
                _fShouldDispose = shouldDispose;

                somSelf->InitRefCntObject(ev);

#if ODDebug
                if (shouldSave && (frame->GetStorageUnit(ev) == kODNULL))
                        WARNMSG_DEBUG(WARN_INDEX(-1), "InitWindowForFrame: shouldSave is TRUE, but root frame is nonpersistent ");

#endif

                _fPlatformWindow = platformWindow;
                _fIsRootWindow  = isRootWindow;
                _fIsResizable   = isResizable;
                _fIsFloating    = isFloating;
                if (sourceFrame)
                        sourceFrame->Acquire(ev);
                _fSourceFrame   = sourceFrame;
                {
                        TempODPart part = frame->AcquirePart(ev);
                        _fDraft                 = part->GetStorageUnit(ev)->GetDraft(ev);
                        _fDraft->Acquire(ev);
                }

                if (!HAS_WRITE_ACCESS(_fDraft->GetPermissions(ev)))
                        shouldSave = kODFalse;

                _fShouldSave    = shouldSave;

                somSelf->CommonInitWindow(ev);

                if (frame)
                {
                        frame->Acquire(ev);
                        frame->SetWindow(ev, somSelf);
                }
                _fRootFrame = frame;

#if defined(_PLATFORM_OS2_) || (_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        // Initialize window title
        somSelf->InitWindowTitle(ev);
#endif

                _fIsDirty = kODFalse;
                _fShouldAdjustOnOpen = kODFalse;

                ODStorageUnit* frameSU = frame->GetStorageUnit(ev);
                if ((frameSU != kODNULL) && (_fDraft != kODNULL))
                {
                        ODID windowPropsID = ODGetStrongSURefProp(ev, frameSU, kODPropWindowProperties, kODStrongStorageUnitRef);
                        if (windowPropsID != kODNULLID)
                        {
                                _fStorageUnit = _fDraft->AcquireStorageUnit(ev, windowPropsID);
                        }
                }

#ifdef _PLATFORM_WIN32_                 //TDF: quick hack for beta!
#ifdef OLE_WRAPPER_READY
        // register the window as a valid drop target for OLE drag-and-drop
        OleWrapper *pOleWrapper = ODOLEManager::GetShell()->GetActiveDocumentManager(ev)->GetOleWrapper(ev);
        if (pOleWrapper)
                pOleWrapper->SetupDropTarget(_fPlatformWindow);
#else
        // use the drag and drop manager's SetupDropTarget method
        ODDragAndDrop *pDragAndDrop = _fSession->GetDragAndDrop(ev);
        if (pDragAndDrop)
                pDragAndDrop->SetupDropTarget(ev, _fPlatformWindow);
#endif // OLE_WRAPPER_READY
#endif // _PLATFORM_WIN32_

        SOM_CATCH_ALL
        SOM_ENDTRY
}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
//-------------------------------------------------------------------------------------
// ODWindow: InitWindowTitle
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowInitWindowTitle (ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","InitWindowTitle");

    PlatformFile* file = kODNULL; ODVolatile(file);
    char *windowName = NULL;
    char *parthandlername = NULL;

    SOM_TRY

        // If this is not a root window - root window titles are set via
        // SetDefaultWindowTitles
        if (!_fIsRootWindow)
        {
           // Acquire the part wrapper associated with the source frame
           ODPart *part = _fSourceFrame->AcquirePart(ev);

           // Get the real part from the part wrapper
           ODPart *realPart = part->GetRealPart(ev);

           // Get the realPart's metaclass
           M_ODPart* clsPart = (M_ODPart*) realPart->somGetClass();

           // Release the real part
           part->ReleaseRealPart(ev);

           // If the class is a part metaclass.  It won't be if this
           // is NoPart.  NoPart open into window doesn't need a title
           if (clsPart->somIsA(_M_ODPart))
           {
               // Get the part handler name
               char *parthandlername = clsPart->clsGetODPartHandlerDisplayName(ev);

               // If data returned
               if (parthandlername)
               {
                   // Get the draft's document's container
                   ODContainer* container = _fDraft->GetDocument(ev)->GetContainer(ev);

                   file = GetPlatformFileFromContainer(ev, container);

                   ODULong draftNum = 0;

                   TRY
                           draftNum = GetDraftNumFromDraft(ev, _fDraft);
                   CATCH_ALL
                   ENDTRY

                   // Maximum file name if we need it
                   char fileName[kODMaxFileNameSize + 1];

                   // Get the appropriate message from the catalog
                   char *ret_string;
                   if (draftNum != 0)
                   {
#ifdef __IBMCPP__
                       ret_string =
                         catgets(ODcat_handle,
                                 WINSTAT_SET,
                                 WINSTAT_40,
                                 "%s - %s Draft %d");
#else
                       ret_string = "%s - %s Draft %d";
#endif
                   }
                   else
                   {
#ifdef __IBMCPP__
                       ret_string =
                         catgets(ODcat_handle,
                                 WINSTAT_SET,
                                 WINSTAT_30,
                                 "%s - %s");
#else
                       ret_string = "%s - %s";
#endif
                   }

                   // Determine where the second token comes from
                   char *token2;

                   // If there is a long document name
                   char *longname = _fSession->GetWindowState(ev)->GetLongName(ev);
                   if ((longname) && (longname[0] != '\0'))
                   {
                       // Note that for an in-memory container,
                       // this is the only valid path.  An
                       // additional test for this case would
                       // probably be approriate in the future.

                       // Use the long document name
                       token2 = longname;
                   }
                   else
                   {
                       // Get the filename of the document
                       file->GetAsciiName(fileName,
                                          kODMaxFileNameSize);

                       // Use the filename
                       token2 = fileName;
                   }


                   // Determine length of window title
                   // Space for message, first token, second token
                   // and draft number if we need it
                   unsigned int length = strlen(ret_string) +
                                         strlen(parthandlername)     +
                                         strlen(token2)     +
                                         32;

                   // Allocate storage for window name
                   windowName = new char[length];

                   // If storage allocated
                   if (windowName)
                   {
                       // If there was a draft number
                       if (draftNum)
                       {
                           // Create the window title
                           sprintf(windowName,
                                   ret_string,
                                   parthandlername,
                                   token2,
                                   draftNum);
                       }
                       else
                       {
                           // Create the window title
                           sprintf(windowName,
                                   ret_string,
                                   parthandlername,
                                   token2);
                       }

                       somSelf->SetWindowTitle(ev, windowName);
                       delete windowName;
                   }

                   ODDeleteObject(file);

#if 0
                   // Free the part handler display name
                   SOMFree(parthandlername);
#endif
               }
           }
        }

    SOM_CATCH_ALL
        if (parthandlername)
        {
#if 0
            SOMFree(parthandlername);
#endif
            ODDeleteObject(file);
            delete windowName;
        }
    SOM_ENDTRY
}
#endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_ || _PLATFORM_UNIX_

//-------------------------------------------------------------------------------------
// ODWindow: Purge
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODSize         SOMLINK ODWindowPurge(ODWindow *somSelf, Environment *ev,
                ODSize size)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Purge");

    ODSize purgeSize = 0;

    SOM_TRY

        purgeSize = parent_Purge(somSelf, ev, size);

    SOM_CATCH_ALL
    SOM_ENDTRY

        return purgeSize;
}

//-------------------------------------------------------------------------------------
// ODWindow: Release
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void   SOMLINK ODWindowRelease(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Release");

    SOM_TRY

            parent_Release(somSelf, ev);
                if (somSelf->GetRefCount(ev) == 0)
                        _fSession->GetWindowState(ev)->ReleaseWindow(ev,somSelf);

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: ~ODWindow
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowsomUninit (ODWindow *somSelf)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","somUninit");

        Environment* ev = somGetGlobalEnvironment();
        //      ¥ Get rid of our window
        if (_fPlatformWindow && _fShouldDispose)
        {
#ifdef _PLATFORM_MACINTOSH_
                if (((WindowRecord*)_fPlatformWindow)->windowKind == dialogKind)
                        DisposeDialog(_fPlatformWindow);
                else
                        DisposeWindow(_fPlatformWindow);
#endif

#ifdef _PLATFORM_OS2_ // CED
        // Destroy the window, but first remove any menus
        HWND menu = WinWindowFromID(_fPlatformWindow, FID_MENU);
        if (menu) {
            WinSetParent(menu, HWND_OBJECT, FALSE);
            WinSetOwner(menu,  HWND_OBJECT);
        } /* endif */
        WinDestroyWindow(_fPlatformWindow);

        // If dummy menu bar exists
        if (_fDummyMenu)
        {
            // Destroy the dummy menu bar
            WinDestroyWindow(_fDummyMenu);
        }
#endif

#ifdef _PLATFORM_WIN32_
        // Ensure the window does not destroy a menu as well.  The menu may be
        // shared among multiple windows.
        SetMenu(_fPlatformWindow, NULL);

        // Destroy the window
        DestroyWindow(_fPlatformWindow);
#endif
#ifdef _PLATFORM_UNIX_
        // Destroy the window
        XtDestroyWidget(_fTopLevelShellWidget);
        XSync(_fSession->GetWindowState(ev)->GetDisplay(ev), False);
        _fTopLevelShellWidget = 0;
        _fMainWindowWidget = 0;
        _fDrawingAreaWidget = 0;
#endif
                _fPlatformWindow = kODNULL;
        }

        // Storage Unit must be released before draft
        ODSafeReleaseObject(_fStorageUnit);
        ODSafeReleaseObject(_fDraft);
        ODSafeReleaseObject(_fRootFrame);
        ODSafeReleaseObject(_fSourceFrame);
}       //       SOMLINK ODWindowsomUninit

//-------------------------------------------------------------------------------------
// ODWindow: GetPlatformWindow
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODPlatformWindow       SOMLINK ODWindowGetPlatformWindow(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetPlatformWindow");

        return _fPlatformWindow;
}

//-------------------------------------------------------------------------------------
// ODWindow: AcquireSourceFrame
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODFrame*       SOMLINK ODWindowAcquireSourceFrame(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","AcquireSourceFrame");

        SOM_TRY

    if (_fSourceFrame)
        _fSourceFrame->Acquire(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY
        return _fSourceFrame;
}

//-------------------------------------------------------------------------------------
// ODWindow: SetSourceFrame
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void   SOMLINK ODWindowSetSourceFrame(ODWindow *somSelf, Environment *ev,
                                                                                                ODFrame* frame)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","AcquireSourceFrame");

    SOM_TRY

            if (frame)
                frame->Acquire(ev);
            if (_fSourceFrame)
                _fSourceFrame->Release(ev);

                _fSourceFrame = frame;

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: GetRootFrame
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODFrame*  SOMLINK ODWindowGetRootFrame(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetRootFrame");

        return _fRootFrame;
}

//-------------------------------------------------------------------------------------
// ODWindow: GetRootFacet
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODFacet*  SOMLINK ODWindowGetRootFacet(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetRootFacet");

        return _fRootFacet;
}

//-------------------------------------------------------------------------------------
// ODWindow: CreateRootFrame
// PRIVATE
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowCreateRootFrame(ODWindow *somSelf, Environment *ev,
                ODType          frameType,
                ODTypeToken     viewType,
                ODTypeToken     presentation,
                ODPart* rootPart)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","CreateRootFrame");

        ODShape* newShape = kODNULL;ODVolatile(newShape);

        SOM_TRY

                if (_fPlatformWindow)
                {
          #ifdef _PLATFORM_MACINTOSH_
                        Rect frameShape = _fPlatformWindow->portRect;
          #endif

                        TRY

                                newShape = new ODShape; // Can't Use factory method because no frame is available yet
                                THROW_IF_NULL(newShape);
                                newShape->InitShape(ev);

                        CATCH_ALL
                                ODDeleteObject(newShape); newShape = kODNULL; // must delete before InitShape returns
                                RERAISE;
                        ENDTRY

                        TempODShape tempShape = newShape;       // ensures it's released

          #ifdef _PLATFORM_MACINTOSH_
                        ODRect r = _fPlatformWindow->portRect;
          #endif
          #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
            ODPoint scale;
            ODPoint bounds;

          #ifdef _PLATFORM_OS2_
            RECTL rclClient;

            // Get the docshell window rectangle
            WinQueryWindowRect(_fPlatformWindow, &rclClient);

            // Calculate the client area of the docshell
            WinCalcFrameRect(_fPlatformWindow, &rclClient, TRUE);

            // Convert the client dimensions to a Fixed ODPoint.  Note that
            // OS/2 has point (0,0) as the lower left
            bounds.Set(ODIntToFixed(rclClient.xRight - rclClient.xLeft),
                       ODIntToFixed(rclClient.yTop - rclClient.yBottom));
          #endif

          #ifdef _PLATFORM_WIN32_
            RECT rclClient;

            // Get the client area of the docshell window
            GetClientRect(_fPlatformWindow, &rclClient);

            // Convert the client dimensions to a Fixed ODPoint.  Note that
            // Windows has point (0,0) as the upper left
            bounds.Set(ODIntToFixed(rclClient.right - rclClient.left),
                       ODIntToFixed(rclClient.bottom - rclClient.top));
          #endif

          #ifdef _PLATFORM_UNIX_
            // Get the size of the DrawingAreaWidget
            Dimension width, height;
            XtVaGetValues(_fDrawingAreaWidget,
                          XmNwidth, &width,
                          XmNheight, &height,
                          NULL);

            // Convert the drawingarea dimensions to a Fixed ODPoint.
            bounds.Set(ODIntToFixed(width),
                       ODIntToFixed(height));
          #endif

            // Instantiate a new transform and initialize to an identity
            // transform
            ODTransform* devTransform = new ODTransform;
            THROW_IF_NULL(devTransform);
            devTransform->InitTransform(ev);
            devTransform->Reset(ev);

            // Get the 72dpi scale for this platform
            Get72DPIToScreenScale(_fSession, &scale.x, &scale.y);

            // Scale the transform by the 72dpi value
            devTransform->ScaleBy(ev, &scale);

            // Invert the window bounds into 72dpi
            devTransform->InvertPoint(ev, &bounds);

            // Release the transform
            ODReleaseObject(ev, devTransform);

            // Instantiate an ODRect with 72dpi using transformed window corners
            ODRect r(ODPoint(0,0), bounds);
          #endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_ || _PLATFORM_UNIX_

                        newShape->SetRectangle(ev,&r);
                        _fRootFrame = _fDraft->CreateFrame(ev, frameType, (ODFrame*)kODNULL,
                                                                                newShape, (ODCanvas*)kODNULL,
                                                                                rootPart, viewType, presentation,
                                                                                kODFalse,       // isSubframe
                                                                                kODFalse);      // isOverlaid
                        _fRootFrame->SetWindow(ev, somSelf);

                        if (rootPart && _fSourceFrame)
                                rootPart->AttachSourceFrame(ev, _fRootFrame, _fSourceFrame);
                }
                _fIsDirty = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY
}       // CreateRootFrame()

//-------------------------------------------------------------------------------------
// ODWindow: CreateRootFacet
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowCreateRootFacet(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","CreateRootFacet");

        ODShape* frameShape = kODNULL;
        ODShape* clipShape = kODNULL;

        SOM_TRY

                frameShape = _fRootFrame->AcquireFrameShape(ev, kODNULL);
                clipShape = frameShape->Copy(ev);
                ODReleaseObject(ev, frameShape);

                ODTransform* externalTransform = _fRootFrame->CreateTransform(ev);

      #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        ODPoint scale;

        // Initialize transform to the identity transform
        externalTransform->Reset(ev);

        // Get the 72dpi scale for this platform
        Get72DPIToScreenScale(_fSession, &scale.x, &scale.y);

        // Scale the transform by the 72dpi value
        externalTransform->ScaleBy(ev, &scale);
      #endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_ || _PLATFORM_UNIX_

                ODWindowState* windowState = _fSession->GetWindowState(ev);
      #ifdef _PLATFORM_MACINTOSH_
                ODCanvas* rootCanvas = windowState->CreateCanvas(ev, kODQuickDraw,
                                                                (ODPlatformCanvas) _fPlatformWindow,
      #endif
      #ifdef _PLATFORM_OS2_
        // Instantiate an OS2 specific window canvas and pass to Window
        // State to create a canvas
        ODOS2WindowCanvas* platformCanvas = new ODOS2WindowCanvas;
        THROW_IF_NULL(platformCanvas);
                ODCanvas* rootCanvas = windowState->CreateCanvas(ev, kODPM,
                                                                platformCanvas,
      #endif
      #ifdef _PLATFORM_WIN32_
        // Instantiate a Windows specific window canvas and pass to Window
        // State to create a canvas
        ODWin32WindowCanvas* platformCanvas = new ODWin32WindowCanvas;
        THROW_IF_NULL(platformCanvas);
                ODCanvas* rootCanvas = windowState->CreateCanvas(ev, kODWin32,
                                                                platformCanvas,
      #endif
      #ifdef _PLATFORM_UNIX_
        // Instantiate an AIX specific window canvas and pass to Window
        // State to create a canvas
        ODAIXWindowCanvas* platformCanvas = new ODAIXWindowCanvas;
        THROW_IF_NULL(platformCanvas);
                ODCanvas* rootCanvas = windowState->CreateCanvas(ev, kODAIX,
                                                                platformCanvas,
      #endif
                                                                kODTrue,                // isDynamic
                                                                kODFalse);              // isOffscreen

                _fRootFacet = windowState->CreateFacet(ev, _fRootFrame, clipShape, externalTransform, rootCanvas, kODNULL);
                ODReleaseObject(ev, clipShape);
                ODReleaseObject(ev, externalTransform);

                _fRootFrame->FacetAdded(ev,_fRootFacet);
                _fIsDirty = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: AdjustWindowShape
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowAdjustWindowShape (ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","AdjustWindowShape");

    SOM_TRY

                if (_fPlatformWindow)
                {
#ifdef _PLATFORM_MACINTOSH_
                        Rect contentRect = _fPlatformWindow->portRect;
                        Rect growRect = contentRect;
                        growRect.left = growRect.right - 15;
                        growRect.top = growRect.bottom - 15;

                        RgnHandle contentRgn = ODNewRgn();
                        RgnHandle growRgn = ODNewRgn();
                        RectRgn(contentRgn,&contentRect);
                        RectRgn(growRgn,&growRect);

                        if (_fIsResizable)
                        DiffRgn(contentRgn,growRgn,contentRgn);
                        ODDisposeHandle((Handle)growRgn);

                        SetPort(_fPlatformWindow);
                        // InvalRgn(contentRgn);

                        ODShape* windowShape =_fRootFrame->CreateShape(ev);
                        windowShape->SetPlatformShape(ev,kODQuickDraw, (ODPlatformShape)contentRgn);
        #ifdef TO_BE_DELETED
                        ODTransform* xform = _fRootFacet->AcquireExternalTransform(ev);
                        windowShape->InverseTransform(ev, xform);       // JBS - must transform frameShape to facet coords
                        ODReleaseObject(ev, xform);
        #endif // TO_BE_DELETED
#endif // _PLATFORM_MACINTOSH

#ifdef _PLATFORM_OS2_
            // Get the document window's rectangle
            RECTL contentRect;
            WinQueryWindowRect(_fPlatformWindow, &contentRect);

            // Get the docshell's client rectangle and ensure it's lower
            // left is (0,0)
            WinCalcFrameRect(_fPlatformWindow, &contentRect, TRUE);
            contentRect.xRight -= contentRect.xLeft;
            contentRect.yTop -= contentRect.yBottom;
            contentRect.xLeft = contentRect.yBottom = 0;

            // Get the root facet's HWND
            HWND hwndRootFacet = _fRootFacet->GetFacetHWND(ev);

            // Set the size of the root facet to match the new size of the
            // docshell's client area.  Note that the window procedure of
            // the root facet will cause all child facets to be sized
            // identically
            WinSetWindowPos(hwndRootFacet, 0, 0, 0, contentRect.xRight, contentRect.yTop, SWP_SIZE);

            // Convert docshell's client area to fixed ODRect object
            ODCoordinate left   = ODIntToFixed(contentRect.xLeft);
            ODCoordinate right  = ODIntToFixed(contentRect.xRight);
            ODCoordinate top    = ODIntToFixed(contentRect.yTop);
            ODCoordinate bottom = ODIntToFixed(contentRect.yBottom);
            ODRect rect;
            rect.Set(left, top, right, bottom);

            // Instantiate a new window shape, initialize it and set the
            // rectangle from the ODRect above
            ODShape* windowShape = new ODShape;
            THROW_IF_NULL(windowShape);
            windowShape->InitShape(ev);
            windowShape->SetRectangle(ev, &rect);

            // Inverse transform the new window shape to be in 72dpi
            {
              TempODTransform exttrans = _fRootFacet->AcquireExternalTransform(ev, kODNULL);
              windowShape->InverseTransform(ev, exttrans);
            }
#endif // _PLATFORM_OS2_
#ifdef _PLATFORM_WIN32_
            // Invalidate the entire docshell window's client area
            InvalidateRect(_fPlatformWindow, NULL, FALSE);

            // Get the docshell's client rectangle
            RECT contentRect;
                        GetClientRect(_fPlatformWindow,&contentRect);

            // Get the root facet's HWND
            HWND hwndRootFacet = _fRootFacet->GetFacetHWND(ev);

            // Set the size of the root facet to match the new size of the
            // docshell's client area.
            SetWindowPos(hwndRootFacet, 0, 0, 0,
                         contentRect.right, contentRect.bottom,
                                         SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

            // Convert docshell's client area to fixed ODRect object
            ODCoordinate left   = ODIntToFixed(contentRect.left);
            ODCoordinate right  = ODIntToFixed(contentRect.right);
            ODCoordinate top    = ODIntToFixed(contentRect.top);
            ODCoordinate bottom = ODIntToFixed(contentRect.bottom);
            ODRect rect;
            rect.Set(left, top, right, bottom);

            // Instantiate a new window shape, initialize it and set the
            // rectangle from the ODRect above
            ODShape* windowShape = new ODShape;
            THROW_IF_NULL(windowShape);
            windowShape->InitShape(ev);
            windowShape->SetRectangle(ev, &rect);

            // Inverse transform the new window shape to be in 72dpi
            {
              TempODTransform exttrans = _fRootFacet->AcquireExternalTransform(ev, kODNULL);
              windowShape->InverseTransform(ev, exttrans);
            }
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
            // Get the size of the DrawingAreaWidget
            Position x, y;
            Dimension width, height;
            XtVaGetValues(_fDrawingAreaWidget,
                          XmNwidth, &width,
                          XmNheight, &height,
                          NULL);

            // Convert drawingarea widget's client area to fixed ODRect object
            ODCoordinate left   = ODIntToFixed(0);
            ODCoordinate right  = ODIntToFixed(width);
            ODCoordinate top    = ODIntToFixed(0);
            ODCoordinate bottom = ODIntToFixed(height);
            ODRect rect;
            rect.Set(left, top, right, bottom);

            // Instantiate a new window shape, initialize it and set the
            // rectangle from the ODRect above
            ODShape* windowShape = new ODShape;
            THROW_IF_NULL(windowShape);
            windowShape->InitShape(ev);
            windowShape->SetRectangle(ev, &rect);

            // Inverse transform the new window shape to be in 72dpi
            {
              TempODTransform exttrans = _fRootFacet->AcquireExternalTransform(ev, kODNULL);
              windowShape->InverseTransform(ev, exttrans);
            }
#endif

                        ODShape* tempShape = kODNULL;
                        tempShape = windowShape->Copy(ev);
                        _fRootFacet->GetFrame(ev)->ChangeFrameShape(ev, windowShape, kODNULL);
                        _fRootFacet->ChangeGeometry(ev, tempShape, kODNULL, kODNULL);
                        ODReleaseObject(ev, windowShape);
                        ODReleaseObject(ev, tempShape);

#ifdef _PLATFORM_UNIX_
        // I put this here since the facet widget may have changed size or
        // position
        XSync(_fSession->GetWindowState(ev)->GetDisplay(ev), False);
#endif
                }
                _fIsDirty = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY
}       // AdjustWindowShape


//-------------------------------------------------------------------------------------
// ODWindow: Open
//
// Description
//-------------------------------------------------------------------------------------


SOM_Scope void  SOMLINK ODWindowOpen (ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Open");

    SOM_TRY

                if (_fRootFacet == kODNULL)
                {
                        somSelf->CreateRootFacet(ev);
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
// On OS/2, NT and AIX, we need to always perform the AdjustWindowShape in case
// the resolution of the display has changed.  AdjustWindowShape typically
// causes the draft to get marked dirty.  However, on OS/2, NT and AIX, we
// only mark the draft dirty if the shape has really changed.  Initially, the
// change to frame.cpp was required because NT sends the WM_SIZE message late.
#else
                        if ( _fShouldAdjustOnOpen )
#endif
                                somSelf->AdjustWindowShape(ev);
                        _fIsDirty = kODTrue;
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: Close
//
// Description
//-------------------------------------------------------------------------------------


SOM_Scope void  SOMLINK ODWindowClose(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Close");

    SOM_TRY

                somSelf->Hide(ev);
                _fSession->GetWindowState(ev)->RemoveWindow(ev,somSelf);

                if ( _fRootFrame )
                {
                        if ( _fRootFacet )
                        {
                                _fRootFrame->FacetRemoved(ev,_fRootFacet);
#ifdef _PLATFORM_MACINTOSH_
                                _fSession->GetDispatcher(ev)->InvalidateFacetUnderMouse(ev);
#endif

#ifdef _PLATFORM_MACINTOSH_
                                ODCanvas* rootCanvas = _fRootFacet->GetCanvas(ev); // DMc - new

                                ODDeleteObject(rootCanvas); // DMc - new
#endif
                                ODDeleteObject(_fRootFacet);
                        }

                        _fRootFrame->Close(ev);
                        _fRootFrame = kODNULL;
                }

                ODReleaseObject(ev, _fSourceFrame);
                _fIsDirty = kODTrue;
                somSelf->Release(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY

}       // Close()

//-------------------------------------------------------------------------------------
// ODWindow: CloseAndRemove
//
// Description
//-------------------------------------------------------------------------------------


SOM_Scope void  SOMLINK ODWindowCloseAndRemove(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","CloseAndRemove");

    SOM_TRY

                if (_fPlatformWindow)
                {
                        somSelf->Hide(ev);
                        _fSession->GetWindowState(ev)->RemoveWindow(ev,somSelf);

                        if ( _fRootFrame )
                        {
                                if ( _fRootFacet )
                                {
                                        _fRootFrame->FacetRemoved(ev,_fRootFacet);
#ifdef _PLATFORM_MACINTOSH_
                                        _fSession->GetDispatcher(ev)->InvalidateFacetUnderMouse(ev);
#endif

#ifdef _PLATFORM_MACINTOSH_
                                        ODCanvas* rootCanvas = _fRootFacet->GetCanvas(ev); // DMc - new
                                        ODDeleteObject(rootCanvas); // DMc - new
#endif
                                        ODDeleteObject(_fRootFacet);
                                }

                                if (!HAS_WRITE_ACCESS(_fDraft->GetPermissions(ev)))
                                {
                                        _fRootFrame->Close(ev);
                                        _fRootFrame = kODNULL;
                                }
                                else
                                {
                                        _fRootFrame->Remove(ev);
                                        _fRootFrame = kODNULL;
                                }
                        }

                        ODReleaseObject(ev, _fSourceFrame);
                        somSelf->Release(ev);
                        _fIsDirty = kODTrue;
                }

        SOM_CATCH_ALL
        SOM_ENDTRY

}



//-------------------------------------------------------------------------------------
// ODWindow: HandleActivateEvent
//
// Description
//-------------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
static void ODDrawGrowIcon(ODPlatformWindow window)
{
        GrafPtr savePort;

        GetPort(&savePort);
        SetPort(window);
        Rect r = window->portRect;
        r.left = r.right  - 15;
        r.top  = r.bottom - 15;
        RgnHandle       oldClip = kODNULL; ODVolatile(oldClip);

        TRY
                oldClip = ODNewRgn();

                GetClip(oldClip);
                ClipRect(&r);
                DrawGrowIcon(window);
                SetClip(oldClip);

        CATCH_ALL
                // Don't Reraise. Since this gets called when putting up
                // the low memory alert calls DeactivateFrontWindows #1280629
        ENDTRY

        ODDisposeHandle((Handle)oldClip);
        SetPort(savePort);
}
#endif // _PLATFORM_MACINTOSH_

SOM_Scope void  SOMLINK ODWindowHandleActivateEvent (ODWindow *somSelf, Environment *ev,
                ODEventData* theEvent
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                ,ODEventInfo *eventInfo
#endif
                )
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","HandleActivateEvent");

#ifdef _PLATFORM_UNIX_
        // If this is a FocusIn event
        if (eventInfo->originalType == FocusIn)
        {
            // Set status field in this window signifying we have received
            // a FocusIn event
            _fFocusInReceived = kODTrue;

            // Ensure this window is saved for later AcquireActiveWindow
            // calls to WinState.
            _fSession->GetWindowState(ev)->SaveActiveWindow(ev, somSelf);
        }
        // else - it has to be a FocusOut.
        else
        {
            // If we haven't yet received a FocusIn event
            if (!_fFocusInReceived)
            {
                // Ignore this event.  It's possible in AIX to get a FocusOut
                // event prior to a FocusIn event when activating a window
                // upon mouse entry.  This messes up Part needsactivating
                // logic.  So, we will prevent this from happening.
                return;
            }

            // Reset flag.  This AIX abnormality can happen every time a
            // mouse enters a window.
            _fFocusInReceived = kODFalse;
        }
#endif

        ODFrame* frame = kODNULL;
        ODFacetIterator* t = kODNULL; ODVolatile(t);

        SOM_TRY

#ifdef _PLATFORM_MACINTOSH_
                _fIsActive = (theEvent->modifiers & activeFlag == 1);
#endif

                t = _fRootFacet->CreateFacetIterator(ev,
                                                                kODBottomUp, kODBackToFront);

                for (ODFacet* facet = t->First(ev); t->IsNotComplete(ev); facet = t->Next(ev))
                {
                        if (facet)
                        {
                                TRY{
                                        frame = facet->GetFrame(ev);
                                        TempODPart part = frame->AcquirePart(ev); // -- T‚ tempobj'd
                                        if ((ODPart*)part)
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                                                part->HandleEvent(ev,theEvent, frame, facet, eventInfo);
#else
                                                part->HandleEvent(ev,theEvent, frame, facet, kODNULL);
#endif
                                }CATCH_ALL{
                                        // ignore exception
                                }ENDTRY
                        }
                }
                ODDeleteObject(t);
#ifdef _PLATFORM_MACINTOSH_
                if (somSelf->IsResizable(ev))
                        ODDrawGrowIcon(_fPlatformWindow);
#endif

#ifdef _PLATFORM_OS2_
               // If deactivating window
               if (SHORT1FROMMP(theEvent->mp1) == FALSE)
               {
                   // If the window has a menubar
                   HWND oldMenu = WinWindowFromID(_fPlatformWindow, FID_MENU);
                   if (oldMenu)
                   {
                       // Set the parent and owner of the old menu bar
                       WinSetParent(oldMenu, HWND_OBJECT, False);
                       WinSetOwner(oldMenu, HWND_OBJECT);

                       // If dummy menu bar already exists
                       if (_fDummyMenu)
                       {
                           // Destroy the old dummy menu bar
                           WinDestroyWindow(_fDummyMenu);
                       }

                       // Create dummy menu bar
                       _fDummyMenu = WinCreateMenu(HWND_OBJECT, NULL);

                       // copy font name from old menu
                       // ------------------------------------
                       const ULONG nMaxPath = 288;
                       char  szFontName[nMaxPath];

                       WinQueryPresParam(oldMenu, PP_FONTNAMESIZE, 0L, NULL,
                                         nMaxPath, (PPVOID)szFontName, 0);
                       WinSetPresParam(_fDummyMenu, PP_FONTNAMESIZE,
                                         strlen(szFontName)+1, (PVOID)szFontName);

                       // Find out how many items are in the existing menu bar
                       SHORT count = SHORT1FROMMR(WinSendMsg(oldMenu,
                                                             MM_QUERYITEMCOUNT,
                                                             0,
                                                             0));

                       // For all the items in the existing menu bar
                       for (SHORT index = 0; index < count; index++)
                       {
                           // Get the item id for the menu item at this position
                           SHORT id = SHORT1FROMMR(WinSendMsg(oldMenu,
                                                              MM_ITEMIDFROMPOSITION,
                                                              MPFROMSHORT(index),
                                                              0));

                           // Get the menuitem structure for the item
                           MENUITEM mi;
                           SHORT1FROMMR(WinSendMsg(oldMenu,
                                                   MM_QUERYITEM,
                                                   MPFROM2SHORT(id, FALSE),
                                                   MPFROMP(&mi)));

                           // Clear out the submenu flags and fields
                           mi.afStyle &= ~MIS_SUBMENU;
                           mi.hwndSubMenu = 0;

                           // If this item has text
                           char *text = NULL;
                           ODBoolean isText = (mi.afStyle & MIS_TEXT) ? kODTrue : kODFalse;
                           if (isText)
                           {
                               // Get the text length of the menu item
                               SHORT len = SHORT1FROMMR(WinSendMsg(oldMenu,
                                                                   MM_QUERYITEMTEXTLENGTH,
                                                                   MPFROMSHORT(id),
                                                                   0));

                               // allocate storage for the menu text length
                               text = new char[len + 1];

                               // If allocated
                               if (text)
                               {
                                   // Get the text
                                   WinSendMsg(oldMenu,
                                              MM_QUERYITEMTEXT,
                                              MPFROM2SHORT(id, len + 1),
                                              MPFROMP(text));
                               }
                           }

                           // Add the synthetic menu item to the new menubar
                           WinSendMsg(_fDummyMenu,
                                      MM_INSERTITEM,
                                      MPFROMP(&mi),
                                      MPFROMP(isText ? text : NULL));

                           // If text
                           if (isText)
                           {
                               // delete the text
                               delete text;
                           }
                       }

                       // Set the parent and owner of the dummy menu bar to the PlatformWindow.
                       WinSetParent(_fDummyMenu, _fPlatformWindow, FALSE);
                       WinSetOwner(_fDummyMenu, _fPlatformWindow);

                       // We have to do this message or else the window won't get the new menu
                       MRESULT mres = WinSendMsg(_fPlatformWindow, WM_UPDATEFRAME,
                                                 MPFROMLONG(FCF_MENU), NULL);

                   }
               }
#endif
        SOM_CATCH_ALL

                ODDeleteObject(t);

        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: HandleUpdateEvent
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowHandleUpdateEvent (ODWindow *somSelf, Environment *ev,
                ODEventData* theEvent)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","HandleUpdateEvent");

#ifdef _PLATFORM_MACINTOSH_
        ODUnused(theEvent);
#endif

        SOM_TRY
#ifdef _PLATFORM_MACINTOSH_
                somSelf->Update(ev);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        // Call the update method passing in theEvent as a parameter
                somSelf->Update(ev, theEvent);
#endif
        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: Update
//
// Description
//   Note that on OS/2 and Win32 there is an additional parameter.  This
//   parameter contains the invalid region in the mp2/lParam parameter of the
//   event structure.
//-------------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
SOM_Scope void  SOMLINK ODWindowUpdate(ODWindow *somSelf, Environment *ev)
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
SOM_Scope void  SOMLINK ODWindowUpdate(ODWindow *somSelf,  Environment *ev,
                                       ODEventData* theEvent)
#endif
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Update");

    SOM_TRY

                if (_fPlatformWindow)
                {
                                // #1248624 Failure handling should be further improved, but this should prevent
                                // the infinite dialog problem
#ifdef _PLATFORM_UNIX_
                    // The UNIX Expose event is relative to each widget, not to the
                    // TopLevelShellWidget.  As a result, the logic for UNIX
                    // is different than the Macintosh, OS/2 and Windows platforms.
                    // I also combine all Expose events for a particular window
                    // into a region prior to calling the Facet's Update method.

                    // The dispatcher has already found the applicable ODWindow for us.
                    // We now have to find the applicable facet which needs updating.
                    // The dispatcher has also ensured that the window in the EsposeEvent
                    // is for a widget.

                    // Copy the input event into our local Event
                    XEvent localEvent;
                    memcpy(&localEvent, theEvent, sizeof(XEvent));

                    // Get the event's widget
                    Widget facetWidget = XtWindowToWidget(localEvent.xany.display,
                                                          localEvent.xany.window);

                    // Facet found for widget?
                    ODBoolean FacetFound = kODFalse;

                    // Instantiate and initialize a facet iterator for the rootfacet
                    ODFacetIterator* iter = _fRootFacet->CreateFacetIterator(ev,
                                                                             kODTopDown,
                                                                             kODBackToFront);

                    // For all the facets of this window
                    for (ODFacet *facet = iter->First(ev);
                         iter->IsNotComplete(ev);
                         facet = iter->Next(ev))
                    {
                        // If this facet's widget is the same as the widget in the
                        // expose event
                        if (facetWidget == facet->GetFacetWidget(ev))
                        {
                            // Signify the facet has been found
                            FacetFound = kODTrue;

                            // We need to create a region from the
                            // set of contiguous Expose events

                            // Create an empty region
                            Region Rgn = XCreateRegion();

                            do
                            {
                                // Combine this event's rectangle with
                                // the existing region
                                XRectangle rect;
                                rect.x      = localEvent.xexpose.x;
                                rect.y      = localEvent.xexpose.y;
                                rect.width  = localEvent.xexpose.width;
                                rect.height = localEvent.xexpose.height;
                                XUnionRectWithRegion(&rect,
                                                     Rgn,
                                                     Rgn);

                                // Dispatch the expose event.
                                XtDispatchEvent(&localEvent);
                            }
                            // while there is another expose event for this
                            // facet widget's window.
                            while (XCheckWindowEvent(localEvent.xany.display,
                                                     localEvent.xany.window,
                                                     ExposureMask,
                                                     &localEvent));

                            // Create aix shape from region
                            TempODShape exposeShape = facet->CreateShape(ev);
                            exposeShape->SetRegion(ev, Rgn);

                            // Acquire the facet's windowframetransform
                            TempODTransform xform = facet->AcquireWindowFrameTransform(ev, kODNULL);

                            // Transform the facet's origin into window frame coordinates
                            ODPoint Point(0, 0);
                            xform->TransformPoint(ev, &Point);

                            // Create a new transform to move over the shape and convert
                            // to the facet's frame coordinates.  Note that the rectangles
                            // in the ExposeEvents are described in terms of widget coordinates.
                            // This is equivalent to Window coordinates without the offseting
                            // origin.
                            TempODTransform xform2 = facet->CreateTransform(ev);
                            xform2->SetOffset(ev, &Point);
                            TempODTransform xform3 = xform->Copy(ev)->Invert(ev);
                            xform2->PostCompose(ev, xform3);

                            // Transform the shape to be in the facet's frame coordinates
                            xform2->TransformShape(ev, exposeShape);

                            // Update the found facet
                            facet->Update(ev, exposeShape, kODNULL);

                            // Note that the region does not need to be
                            // destroyed.  It will be destroyed when the
                            // exposeShape goes out of scope.
                            // Leaving this call here will cause unknown memory
                            // to get freed which will cause a later error.
                            // XDestroyRegion(Rgn);

                            // Get the currently selected frame
                            TempODFrame activeFrame = _fSession->GetArbitrator(ev)->
                                                      AcquireFocusOwner(ev,_fSelectionFocus);

                            // Redraw the active border - do we need to do this on AIX?
                            somSelf->DrawActiveBorder(ev, activeFrame);

                            // Break out of the for loop
                            break;
                        }
                    }

                    // Delete the facet iterator
                    ODDeleteObject(iter);

                    // If a facet was not found for the Expose event's widget
                    if (!FacetFound)
                    {
                        // Give the event to Xt.
                        XtDispatchEvent(&localEvent);
                    }
#else
#ifdef _PLATFORM_MACINTOSH_
                        TRY
                                GrafPtr savePort;

                                GetPort(&savePort);
                                somSelf->PrepareCanvas(ev);
#endif
#ifdef _PLATFORM_OS2_
                // Get hGUpdateRegion from rlshell.cpp
                HRGN updateRgn = (HRGN) theEvent->mp2;
#endif
#ifdef _PLATFORM_WIN32_
                // Get hGUpdateRegion from rlshell.cpp
                HRGN updateRgn = (HRGN) theEvent->lParam;
#endif

                                TempODFrame activeFrame
                                        = _fSession->GetArbitrator(ev)->AcquireFocusOwner(ev,_fSelectionFocus);
#ifdef _PLATFORM_MACINTOSH_
                                BeginUpdate(_fPlatformWindow);                                  // this sets up the visRgn

                                if (!EmptyRgn(_fPlatformWindow->visRgn))                        // draw if updating needs to be done
                                {
                                        // Let the part decide
                                        // EraseRgn (_fPlatformWindow->visRgn);

                                        {
#endif
                                                ODFacet* rootFacet = _fRootFacet;

#ifdef _PLATFORM_MACINTOSH_
                                                RgnHandle updateRgn = ODNewRgn();
                                                CopyRgn(_fPlatformWindow->visRgn, updateRgn);
#endif
                                                TempODShape updateShape = rootFacet->CreateShape(ev);
#ifdef _PLATFORM_MACINTOSH_
                                                updateShape->SetPlatformShape(ev,kODQuickDraw, updateRgn);
#endif
#ifdef _PLATFORM_OS2_
                        // Set the updateShape's region
                        updateShape->SetPlatformShape(ev, kODPM, updateRgn);
#endif
#ifdef _PLATFORM_WIN32_
                        // Set the updateShape's region
                        updateShape->SetPlatformShape(ev, kODWin32, updateRgn);
#endif
                                                TempODTransform xform = rootFacet->AcquireWindowFrameTransform(ev, kODNULL);
                                                updateShape->InverseTransform(ev, xform);
                                                rootFacet->Update(ev, updateShape, kODNULL);
#ifdef _PLATFORM_MACINTOSH_
                                        }
                                        // ¥ Focus back on the window so that everything gets reset
                                        somSelf->PrepareCanvas(ev);

                                        // ¥ After everything is back in shape we need to get the grow icon drawn
                                        if (somSelf->IsResizable(ev))
                                                ODDrawGrowIcon(_fPlatformWindow);
                                }
                                EndUpdate(_fPlatformWindow);
#endif
                                somSelf->DrawActiveBorder(ev,activeFrame);
#ifdef _PLATFORM_MACINTOSH_
                                SetPort(savePort);

                        CATCH_ALL
                                EndUpdate(_fPlatformWindow);
                        ENDTRY
#endif
#endif
                }
                _fIsDirty = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY
}       //  SOMLINK ODWindowUpdate

//-------------------------------------------------------------------------------------
// ODWindow: DrawActiveBorder
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowDrawActiveBorder(ODWindow *somSelf, Environment *ev, ODFrame* frame)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","DrawActiveBorder");

// !!! needs to draw border for all facets of active frame
// Don't draw in background. Also need to invalidate

        SOM_TRY

#ifdef _PLATFORM_MACINTOSH_
                if ( !_fInBackground && frame && (frame != _fRootFrame) )
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        // If a frame parameter was passed in and the frame is not the
        // root frame.
                if ( frame && (frame != _fRootFrame) )
#endif
                {
                        frame->DrawActiveBorder(ev);
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

#ifdef _PLATFORM_MACINTOSH_
//-------------------------------------------------------------------------------------
// ODWindow: PrepareCanvas
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowPrepareCanvas(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","PrepareCanvas");

    SOM_TRY

                if (_fPlatformWindow)
                {
                        SetPort (_fPlatformWindow);
                        SetOrigin(0,0);
                        ClipRect (&_fPlatformWindow->portRect);
                }
                else
                        ClipRect (&ODQDGlobals.thePort->portRect);

        SOM_CATCH_ALL
        SOM_ENDTRY
}
#endif // _PLATFORM_MACINTOSH_

//-------------------------------------------------------------------------------------
// ODWindow: IsResizable
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowIsResizable(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","IsResizable");

        return _fIsResizable;
}

//-------------------------------------------------------------------------------------
// ODWindow: IsFloating
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowIsFloating(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","IsFloating");

        return _fIsFloating;
}

//-------------------------------------------------------------------------------------
// ODWindow: IsRootWindow
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowIsRootWindow(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","IsRootWindow");

        return _fIsRootWindow;
}

//-------------------------------------------------------------------------------------
// ODWindow: ShouldSave
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowShouldSave(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","ShouldSave");

        return _fShouldSave;
}

//-------------------------------------------------------------------------------------
// ODWindow: SetShouldSave
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSetShouldSave(ODWindow *somSelf, Environment *ev,
                ODBoolean shouldSave)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetShouldSave");

        _fShouldSave = shouldSave;
        _fIsDirty = kODTrue;
}

//-------------------------------------------------------------------------------------
// ODWindow: ShouldShowLinks
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowShouldShowLinks(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","ShouldShowLinks");

        return _fShouldShowLinks;
}

//-------------------------------------------------------------------------------------
// ODWindow: SetShouldShowLinks
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSetShouldShowLinks(ODWindow *somSelf, Environment *ev,
                ODBoolean shouldShowLinks)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetShouldShowLinks");

        _fShouldShowLinks = shouldShowLinks;
        _fIsDirty = kODTrue;
}

//-------------------------------------------------------------------------------------
// ODWindow: ShouldDispose
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowShouldDispose(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","ShouldDispose");

        return _fShouldDispose;
}

//-------------------------------------------------------------------------------------
// ODWindow: GetFacetUnderPoint
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODFacet*  SOMLINK ODWindowGetFacetUnderPoint(ODWindow *somSelf, Environment *ev,
                ODPoint* windowPoint)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetFacetUnderPoint");

        ODFacet* foundFacet = kODNULL;
        ODFacetIterator* t = kODNULL;           ODVolatile(t);
        ODPoint framePoint;

        SOM_TRY
                //!!! Should use frameshape containment to prune search
                t = _fRootFacet->CreateFacetIterator(ev,
                                kODTopDown, kODBackToFront);

                for ( ODFacet* facet = t->First(ev);
                                t->IsNotComplete(ev);
                                facet = t->Next(ev) )
                {
                        // get windowPoint in frame coords for hit-testing
                        { TempODTransform winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
                          framePoint = *windowPoint;
                          winToFrame->InvertPoint(ev, &framePoint);
                        }

                        if (facet->ContainsPoint(ev, &framePoint, kODNULL))
                        {
                                foundFacet = facet;
                                if (facet->GetFrame(ev)->IsFrozen(ev) || facet->IsSelected(ev))
                                        t->SkipChildren(ev);
                        }
                        else
                                t->SkipChildren(ev);
                }
                delete t;

        SOM_CATCH_ALL

                ODDeleteObject(t);

        SOM_ENDTRY

        return foundFacet;
}

//-------------------------------------------------------------------------------------
// ODWindow: IsActive
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowIsActive(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","IsActive");

#ifdef _PLATFORM_MACINTOSH_
    ODBoolean isActive = _fIsActive && !_fInBackground;
#endif
#ifdef _PLATFORM_OS2_
    // Get PM's active window and see if it the same as the docshell
    // window
    HWND hwnd = _fPlatformWindow;
    HWND hwndActive = WinQueryActiveWindow(WinQueryWindow(hwnd, QW_PARENT));
    ODBoolean isActive = (hwnd == hwndActive);
#endif
#ifdef _PLATFORM_WIN32_
    // Get Window's active window and see if it the same as the docshell
    // window
    HWND hwnd = _fPlatformWindow;
    HWND hwndActive;

    // If running as an OLE Server

    if (_fSession->GetOleManager(ev)->IsOleServer())
        hwndActive = _fSession->GetWindowState(ev)->GetFrontNonFloatingPlatformWindow(ev);
    else
        hwndActive = GetActiveWindow();
    ODBoolean isActive = (hwnd == hwndActive);
#endif
#ifdef _PLATFORM_UNIX_
    Window activeWindow;
    int revert;
    // Get the window which has the input focus
    XSync(_fSession->GetWindowState(ev)->GetDisplay(ev), False);
    XGetInputFocus(_fSession->GetWindowState(ev)->GetDisplay(ev),
                   &activeWindow,
                   &revert);

    // If the windows are the same, this window is active
    ODBoolean isActive = (_fPlatformWindow == activeWindow);
#endif

        return isActive;
}

//-------------------------------------------------------------------------------------
// ODWindow: Show
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowShow(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Show");

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

                if (!somSelf->IsShown(ev))
                        _fSession->GetWindowState(ev)->ShowODWindow(ev,somSelf);
                _fIsDirty = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: Hide
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowHide(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Hide");

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

                if (somSelf->IsShown(ev))
                {
                        _fSession->GetWindowState(ev)->HideODWindow(ev,somSelf);
                        _fIsDirty = kODTrue;
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

#ifdef _PLATFORM_MACINTOSH_
//-------------------------------------------------------------------------------------
// ODWindow: SuspendResume
//
// Send event to each visible window
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSuspendResume(ODWindow *somSelf, Environment *ev,
                ODEventData* event)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SuspendResume");


        const short kResumeMask = 0x01; // High byte suspend/resume event
        ODFrame* frame = kODNULL;
        ODFacetIterator* t = kODNULL; ODVolatile(t);

        _fInBackground = (event->message & kResumeMask) == 0;

    SOM_TRY

                somSelf->PrepareCanvas(ev);
                if (somSelf->IsResizable(ev))
                        ODDrawGrowIcon(_fPlatformWindow);

                t = _fRootFacet->CreateFacetIterator(ev,
                                                                kODTopDown, kODFrontToBack);

                for (ODFacet* facet = t->First(ev); t->IsNotComplete(ev); facet = t->Next(ev))
                {
                        if (facet)
                        {
                                TRY{
                                        frame = facet->GetFrame(ev);
                                        TempODPart part = frame->AcquirePart(ev); // -- T‚ tempobj'd
                                        if ((ODPart*)part)
                                                part->HandleEvent(ev,event, frame, facet, kODNULL);
                                }CATCH_ALL{
                                        // ignore exception
                                }ENDTRY
                        }
                }
                ODDeleteObject(t);


                TempODFrame activeFrame = _fSession->GetArbitrator(ev)->AcquireFocusOwner(ev,_fSelectionFocus);
                if (activeFrame)
                {
                        if (_fInBackground)
                                activeFrame->InvalidateActiveBorder(ev);
                        else    // Limitation of API: Can't call Invalidate twice, because shape gets released
                                activeFrame->DrawActiveBorder(ev);
                }



        SOM_CATCH_ALL

                ODDeleteObject(t);

        SOM_ENDTRY
}
#endif // _PLATFORM_MACINTOSH_

//-------------------------------------------------------------------------------------
// ODWindow: Activate
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowActivate(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Activate");

#ifdef _PLATFORM_UNIX_
        // This code isn't used yet.  It's called in
        // WindowState::HighlightAndActivateWindow and isn't needed for UNIX
#else
        SOM_TRY

                // Floating window implementation relies on suppressing activate events
                // Part::HandleEvent expects them, so they're synthesize here.
                // They must be routed through the dispatcher, so they can be monitored/patched
                // The modifiers field is hacked to alert the dispatcher that this is
                // an OpenDoc-generated activate event.

#ifdef _PLATFORM_MACINTOSH_
                EventRecord event;
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        // The Mac OdEventData is of type EventRecord.  EventRecord, however,
        // is not native to OS/2, Windows or AIX, so I'm using the actual
        // type expected by both SetActivateEvent and Dispatch
                ODEventData event;
#endif

                SetActivateEvent(event, _fPlatformWindow, kODTrue);

                _fSession->GetDispatcher(ev)->Dispatch(ev, &event);

        SOM_CATCH_ALL
        SOM_ENDTRY
#endif
}

//-------------------------------------------------------------------------------------
// ODWindow: Deactivate
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowDeactivate(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Deactivate");

#ifdef _PLATFORM_UNIX_
        // This code isn't used yet.  It's called in
        // WindowState::HighlightAndActivateWindow and isn't needed for UNIX
#else
        SOM_TRY

                // Floating window implementation relies on suppressing activate events
                // Part::HandleEvent expects them, so they're synthesize here.
                // They must be routed through the dispatcher, so they can be monitored/patched
                // Bit 1 (starting from 0) of the modifiers field is set to alert the dispatcher
                // that this is an OpenDoc-generated activate event.

#ifdef _PLATFORM_MACINTOSH_
                EventRecord event;
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        // The Mac OdEventData is of type EventRecord.  EventRecord, however,
        // is not native to OS/2, Windows or AIX, so I'm using the actual
        // type expected by both SetActivateEvent and Dispatch
                ODEventData event;
#endif

                SetActivateEvent(event, _fPlatformWindow, kODFalse);

                _fSession->GetDispatcher(ev)->Dispatch(ev, &event);

        SOM_CATCH_ALL
        SOM_ENDTRY
#endif
}

#ifdef _PLATFORM_MACINTOSH_
//-------------------------------------------------------------------------------------
// ODWindow: Drag
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowDrag(ODWindow *somSelf, Environment *ev,
                Point* startPoint,
                Rect* draggingBounds)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Drag");

        SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

                _fSession->GetWindowState(ev)->DragODWindow(ev,somSelf, startPoint, draggingBounds);

        SOM_CATCH_ALL
        SOM_ENDTRY
}
#endif // _PLATFORM_MACINTOSH_

//-------------------------------------------------------------------------------------
// ODWindow: Select
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSelect(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Select");

        SOM_TRY

#ifdef _PLATFORM_MACINTOSH_
                if (_fInBackground)
                {
                        ProcessSerialNumber current;
                        OSErr   err = GetCurrentProcess(&current);
                        if (err == noErr)
                                SetFrontProcess(&current);
                }
#endif
                _fSession->GetWindowState(ev)->SelectODWindow(ev,somSelf);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: IsShown
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowIsShown(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","IsShown");

    ODBoolean isShown = kODFalse;

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

#ifdef _PLATFORM_MACINTOSH_
                isShown = ((WindowRecord*)_fPlatformWindow)->visible;
#endif
#ifdef _PLATFORM_OS2_
        // Determine whether the document window is visible
        isShown = WinIsWindowVisible(_fPlatformWindow);
#endif
#ifdef _PLATFORM_WIN32_
        // Determine whether the document window is visible
                isShown = IsWindowVisible(_fPlatformWindow);
#endif
#ifdef _PLATFORM_UNIX_
        // Get the window's attributes
        XSync(_fSession->GetWindowState(ev)->GetDisplay(ev), False);
        XWindowAttributes attributes;
        XGetWindowAttributes(_fSession->GetWindowState(ev)->GetDisplay(ev),
                             _fPlatformWindow,
                             &attributes);

        // Determine whether the document window is visible
        isShown = (attributes.map_state == IsViewable ? kODTrue : kODFalse);
#endif

        SOM_CATCH_ALL
        SOM_ENDTRY

        return isShown;
}

#ifdef _PLATFORM_MACINTOSH_
//-------------------------------------------------------------------------------------
// Protected methods
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowHasCloseBox(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","HasCloseBox");

    ODBoolean hasCloseBox = kODFalse;

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL,kODErrInvalidPlatformWindow);
                hasCloseBox =   ((WindowRecord*)_fPlatformWindow)->goAwayFlag;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return hasCloseBox;
}

SOM_Scope ODBoolean  SOMLINK ODWindowHasZoomBox(ODWindow *somSelf, Environment *ev)

{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","HasZoomBox");

    ODBoolean hasZoomBox = kODFalse;

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);
                hasZoomBox = ((WindowRecord*)_fPlatformWindow)->spareFlag;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return hasZoomBox;
}

SOM_Scope ODBoolean  SOMLINK ODWindowHasGrowBox(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","HasGrowBox");

    ODBoolean hasGrowBox = kODFalse;

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);
                hasGrowBox = somSelf->IsResizable(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return hasGrowBox;
}

SOM_Scope ODSShort  SOMLINK ODWindowGetProcID(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetProcID");

    short theID = 0;
        ResType theType;
        Str255 theName;

        SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

                GetResInfo(((WindowPeek)_fPlatformWindow)->windowDefProc, &theID, &theType, theName);
                short theVariant = GetWVariant(_fPlatformWindow);
                theID = theID*16 + theVariant;
                // return GetWVariant(_fPlatformWindow);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return theID;

}
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_OS2_
SOM_Scope ODULong  SOMLINK ODWindowGetCreateFlags(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetCreateFlags");

    ODULong flCreateFlags = 0;

  SOM_TRY

    ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

    // need to gather create flags the hard way   117197
    if (WinWindowFromID ( _fPlatformWindow, FID_SYSMENU) )
       flCreateFlags |= FCF_SYSMENU;
    if (WinWindowFromID ( _fPlatformWindow, FID_TITLEBAR) )
       flCreateFlags |= FCF_TITLEBAR;
    if (WinWindowFromID ( _fPlatformWindow, FID_MINMAX)   )
       flCreateFlags |= FCF_MINMAX;
    if (WinWindowFromID ( _fPlatformWindow, FID_MENU)      )
       flCreateFlags |= FCF_MENU;
    if (WinWindowFromID ( _fPlatformWindow, FID_VERTSCROLL) )
       flCreateFlags |= FCF_VERTSCROLL;
    if (WinWindowFromID ( _fPlatformWindow, FID_HORZSCROLL)  )
       flCreateFlags |= FCF_HORZSCROLL;
    ULONG style = WinQueryWindowULong( _fPlatformWindow, QWL_STYLE);
    if (style & FS_TASKLIST)
       flCreateFlags |= FCF_TASKLIST;
    if (style & FS_NOBYTEALIGN)
       flCreateFlags |= FCF_NOBYTEALIGN;
    if (style & FS_NOMOVEWITHOWNER)
       flCreateFlags |= FCF_NOMOVEWITHOWNER;
    if (style & FS_SYSMODAL)
       flCreateFlags |= FCF_SYSMODAL;
    if (style & FS_DLGBORDER)
       flCreateFlags |= FCF_DLGBORDER;
    if (style & FS_BORDER)
       flCreateFlags |= FCF_BORDER;
    if (style & FS_SCREENALIGN)
       flCreateFlags |= FCF_SCREENALIGN;
    if (style & FS_MOUSEALIGN)
       flCreateFlags |= FCF_MOUSEALIGN;
    if (style & FS_SIZEBORDER)
       flCreateFlags |= FCF_SIZEBORDER;
    if (style & FS_AUTOICON)
       flCreateFlags |= FCF_AUTOICON;
#ifdef INCL_NLS
    if (style & FS_DBE_APPSTAT)
       flCreateFlags |= FCF_DBE_APPSTAT;
#endif
    if (style & FS_ICON)
       flCreateFlags |= FCF_ICON;
    if (style & FS_ACCELTABLE)
       flCreateFlags |= FCF_ACCELTABLE;
    if (style & FS_SHELLPOSITION)
       flCreateFlags |= FCF_SHELLPOSITION;
                                                // 117197 end
// WM_QUERYWINDOWPARAMS does not work below code always fails
//  FRAMECDATA cData;  // sizeof(cData)};
//  FRAMECDATA *pData = &cData;
//  cData.cb = sizeof(cData);
//  WNDPARAMS  wndParams = {WPM_CTLDATA | WPM_CBCTLDATA, 0, 0, 0, 0, sizeof(cData), pData};
//  if ((WinSendMsg(_fPlatformWindow, WM_QUERYWINDOWPARAMS, MPFROMP(&wndParams), 0)))
//    flCreateFlags = cData.flCreateFlags;
                                                // 117197 end

  SOM_CATCH_ALL
  SOM_ENDTRY

    return flCreateFlags;
}
#endif

#ifdef _PLATFORM_WIN32_
SOM_Scope ODULong  SOMLINK ODWindowGetCreateFlags(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetCreateFlags");

    ODULong flCreateFlags   = 0;
    ODULong flCreateFlagsEx = 0;

  SOM_TRY

  ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

    flCreateFlags   = GetWindowLong(_fPlatformWindow, GWL_STYLE);
    flCreateFlagsEx = GetWindowLong(_fPlatformWindow, GWL_EXSTYLE);

  SOM_CATCH_ALL
  SOM_ENDTRY

    return flCreateFlags;
}
#endif

#ifdef _PLATFORM_UNIX_
SOM_Scope ODULong  SOMLINK ODWindowGetCreateFlags(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetCreateFlags");

    // AIXtodo - I don't know that we will give the user any options
    return 0;
}
#endif

SOM_Scope void  SOMLINK ODWindowGetWindowBounds(ODWindow *somSelf, Environment *ev,
#ifdef _PLATFORM_MACINTOSH_
                Rect* bounds)
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                ODRect* bounds)
#endif
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetWindowBounds");

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);

#ifdef _PLATFORM_MACINTOSH_
                ODRgnHandle contRgn;

                contRgn = ((WindowRecord*)_fPlatformWindow)->contRgn;
                *bounds = (**contRgn).rgnBBox;
#endif
#ifdef _PLATFORM_OS2_  // CED
        // Get the docshell window's rectangle
        SWP swp;
        WinQueryWindowPos(_fPlatformWindow, &swp);

        // Set the ODRect object's values
        ODCoordinate left   = ODIntToFixed(swp.x);
        ODCoordinate right  = ODIntToFixed(swp.x + swp.cx);
        ODCoordinate top    = ODIntToFixed(swp.y + swp.cy);
        ODCoordinate bottom = ODIntToFixed(swp.y);

        bounds->Set(left, top, right, bottom);
#endif
#ifdef _PLATFORM_WIN32_
                // get the bounds for the docshell window
        RECT winrect;
                GetWindowRect (_fPlatformWindow, &winrect);

        // Set the ODRect object's bounding vertices.
        ODCoordinate left   = ODIntToFixed(winrect.left);
        ODCoordinate right  = ODIntToFixed(winrect.right);
        ODCoordinate top    = ODIntToFixed(winrect.top);
        ODCoordinate bottom = ODIntToFixed(winrect.bottom);

        bounds->Set(left, top, right, bottom);
#endif
#ifdef _PLATFORM_UNIX_
        // Get the bounds of the toplevelshellwidget
        Position x, y;
        Dimension width, height;
        XtVaGetValues(_fTopLevelShellWidget,
                      XmNx, &x,
                      XmNy, &y,
                      XmNwidth, &width,
                      XmNheight, &height,
                      NULL);

        // Set the ODRect object's bounding vertices.
        ODCoordinate left   = ODIntToFixed(x);
        ODCoordinate right  = ODIntToFixed(x + width);
        ODCoordinate top    = ODIntToFixed(y);
        ODCoordinate bottom = ODIntToFixed(y + height);

        bounds->Set(left, top, right, bottom);
#endif

        SOM_CATCH_ALL
        SOM_ENDTRY
}

#ifdef _PLATFORM_MACINTOSH_
SOM_Scope ODSLong  SOMLINK ODWindowGetRefCon(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetRefCon");

    ODSLong refCon = 0;

    SOM_TRY

                ASSERT(_fPlatformWindow != kODNULL, kODErrInvalidPlatformWindow);
                refCon = ((WindowRecord*)_fPlatformWindow)->refCon;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return refCon;
}
#endif

//-------------------------------------------------------------------------------------
// ODWindow: SetWindowTitle
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSetWindowTitle(ODWindow *somSelf, Environment *ev,
                char* title)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetWindowTitle");

#ifdef _PLATFORM_MACINTOSH_
    Str255 titleCopy;

    strcpy( (char*) &(titleCopy[1]), title);
    titleCopy[0] = strlen(title);
#endif

    SOM_TRY

#ifdef _PLATFORM_MACINTOSH_
                SetWTitle(_fPlatformWindow, titleCopy);
#endif
#ifdef _PLATFORM_OS2_
        // Get the window handle of docshell window's title bar
        HWND hwndTitleBar = WinWindowFromID(_fPlatformWindow, FID_TITLEBAR);

        // Set title bar text of the docshell window
        WinSetWindowText(hwndTitleBar, (PSZ)title);

        // Get the PM window handle
        SWCNTRL swctl;

        // Get the current task-list switch entry for this window
        HSWITCH  hswitch = WinQuerySwitchHandle(_fPlatformWindow,0);
        WinQuerySwitchEntry(hswitch, &swctl);

        // Copy over the new window title
        strncpy(swctl.szSwtitle, title, MAXNAMEL);

        // Change the task entry title
        WinChangeSwitchEntry (hswitch, &swctl);
#endif
#ifdef _PLATFORM_WIN32_
        // Set title bar text of the docshell window
        SetWindowText(_fPlatformWindow, title);
#endif
#ifdef _PLATFORM_UNIX_
        // Set title bar of the docshell window
        XtVaSetValues(_fTopLevelShellWidget,
                      XmNtitle, title,
                      XmNiconName, title,
                      NULL);
        XSync(_fSession->GetWindowState(ev)->GetDisplay(ev), False);
#endif

                _fIsDirty = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: SetWasVisible
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSetWasVisible(ODWindow *somSelf, Environment *ev,
                ODBoolean wasVisible)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetWasVisible");

        _fWasVisible = wasVisible;
        _fIsDirty = kODTrue;
}

//-------------------------------------------------------------------------------------
// ODWindow: GetWasVisible
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODWindowGetWasVisible(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetWasVisible");

        return _fWasVisible;
}

//-------------------------------------------------------------------------------------
// ODWindow: Externalize
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowExternalize(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","Externalize");

        // #1246478 Removing fIsDirty optimization, because clients can call GetPlatformWindow
        // and change things.
        //if ( !_fIsDirty )
        //      return;

        SOM_TRY

        // Should Title be an ODIText?

                ODStorageUnit* su = _fStorageUnit;

                // No need to do these ASSERTs because the StdTypIO routines make sure
                // the appropriate property and value exist. -T‚
                /*
                ASSERT(su->Exists(ev,kODPropWindowRect, kODRect, 0), kODErrInvalidWindow);
#ifdef _PLATFORM_MACINTOSH_
                ASSERT(su->Exists(ev,kODPropWindowTitle, kODMacIText, 0), kODErrInvalidWindow);
#endif
#ifdef _PLATFORM_OS2_
                ASSERT(su->Exists(ev,kODPropWindowTitle, kODOS2IText, 0), kODErrInvalidWindow);
#endif
#ifdef _PLATFORM_WIN32_
                ASSERT(su->Exists(ev,kODPropWindowTitle, kODWindowsIText, 0), kODErrInvalidWindow);
#endif
#ifdef _PLATFORM_UNIX
                ASSERT(su->Exists(ev,kODPropWindowTitle, kODAIXIText, 0), kODErrInvalidWindow);

#endif
                ASSERT(su->Exists(ev,kODPropWindowProcID, kODSShort, 0), kODErrInvalidWindow);
                ASSERT(su->Exists(ev,kODPropWindowIsVisible, kODBoolean, 0), kODErrInvalidWindow);
#ifdef _PLATFORM_MACINTOSH_
                ASSERT(su->Exists(ev,kODPropWindowHasCloseBox, kODBoolean, 0), kODErrInvalidWindow);
                ASSERT(su->Exists(ev,kODPropWindowHasZoomBox, kODBoolean, 0), kODErrInvalidWindow);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        ASSERT(su->Exists(ev,kODPropWindowCreateFlags, kODULong, 0), kODErrInvalidWindow);
        ASSERT(su->Exists(ev,kODPropWindowSwpFlags, kODULong, 0), kODErrInvalidWindow); // 117197
#endif
                ASSERT(su->Exists(ev,kODPropWindowIsResizable, kODBoolean, 0), kODErrInvalidWindow);
                ASSERT(su->Exists(ev,kODPropWindowIsRootWindow, kODBoolean, 0), kODErrInvalidWindow);
                ASSERT(su->Exists(ev,kODPropWindowIsFloating, kODBoolean, 0), kODErrInvalidWindow);
#ifdef _PLATFORM_MACINTOSH_
                ASSERT(su->Exists(ev,kODPropWindowRefCon, kODSLong, 0), kODErrInvalidWindow);
#endif
                ASSERT(su->Exists(ev,kODPropRootFrame, kODStrongStorageUnitRef, 0), kODErrInvalidWindow);
                //ASSERT(su->Exists(ev,kODPropSourceFrame, kODStrongStorageUnitRef, 0), kODErrInvalidWindow);
                ASSERT(su->Exists(ev,kODPropShouldShowLinks, kODBoolean, 0), kODErrInvalidWindow);
                */

                // Externalize Window Manager properties

                // Externalize the bounds as a standard ODRect

#ifdef _PLATFORM_MACINTOSH_
                Rect r;
#endif

#ifdef _PLATFORM_OS2_  // BLG if window maximized save restore properties
// 117197  - store properties for window

        // Query the window position
        ODRect r;
        SWP swp;
        WinQueryWindowPos( _fPlatformWindow, (PSWP)&swp );   // 117197

        // If the window is minimized or maximized, we need to save the
        // restored size
        if (swp.fl & (SWP_MAXIMIZE |SWP_MINIMIZE))
        {
           // Get the restored size of the document shell window and convert
           // to fixed point
           ODCoordinate left   = ODIntToFixed(WinQueryWindowUShort(_fPlatformWindow, QWS_XRESTORE));
           ODCoordinate right  = ODIntToFixed(WinQueryWindowUShort(_fPlatformWindow, QWS_XRESTORE) +
                                              WinQueryWindowUShort(_fPlatformWindow, QWS_CXRESTORE));
           ODCoordinate top    = ODIntToFixed(WinQueryWindowUShort(_fPlatformWindow, QWS_YRESTORE) +
                                              WinQueryWindowUShort(_fPlatformWindow, QWS_CYRESTORE));
           ODCoordinate bottom = ODIntToFixed(WinQueryWindowUShort(_fPlatformWindow, QWS_YRESTORE));

           // Set the ODRect rectangle to hold the points
           r.Set(left, top, right, bottom);
        }
        // else - we will use the current window size
        else
#endif
#ifdef _PLATFORM_WIN32_  // if window maximized save restore properties
        ODRect r;
        WINDOWPLACEMENT wp;

        // Query the window position
        memset(&wp, 0, sizeof(wp));
        wp.length = sizeof(wp);
        GetWindowPlacement(_fPlatformWindow, &wp);

        // If the window is minimized or maximized, we need to save the
        // restored size
        if ((IsIconic(_fPlatformWindow)) || (IsZoomed(_fPlatformWindow)))
        {
           // Get the restored size of the document shell window and convert
           // to fixed point
           ODCoordinate left   = ODIntToFixed(wp.rcNormalPosition.left);
           ODCoordinate right  = ODIntToFixed(wp.rcNormalPosition.right);
           ODCoordinate top    = ODIntToFixed(wp.rcNormalPosition.top);
           ODCoordinate bottom = ODIntToFixed(wp.rcNormalPosition.bottom);

           // Set the ODRect rectangle to hold the points
           r.Set(left, top, right, bottom);
        }
        // else - we will use the current window size
        else
#endif
#ifdef _PLATFORM_UNIX_
        ODRect r;
        if (0)
        {
            // AIXtodo; - How do I get if iconified or zoomed and if so then
                  // then get the restored size?
        }
        else
#endif
                somSelf->GetWindowBounds(ev,&r);
        ODRect odRect(r);

#ifdef _PLATFORM_OS2_
        // Convert rectangle origin from lower-left to upper-left for cross platform
        // compatibility

        // Get screen height in pixels
        LONG screenHeight;
        HPS hps = WinGetScreenPS( HWND_DESKTOP );
        HDC hdc = GpiQueryDevice( hps );
        DevQueryCaps(hdc, CAPS_HEIGHT, 1, &screenHeight);
        WinReleasePS( hps );

        // Determine window height
        ODFixed height = odRect.top - odRect.bottom;

        // Flip window rectangle
        odRect.top = ODIntToFixed(screenHeight) - odRect.top;
        odRect.bottom = odRect.top + height;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        // Convert odRect to 72DPI
        ODFixed xscale, yscale;
        GetScreenTo72DPIScale(_fSession, &xscale, &yscale);

        odRect.left = ODFixedMultiply(xscale, odRect.left);
        odRect.right = ODFixedMultiply(xscale, odRect.right);
        odRect.top = ODFixedMultiply(yscale, odRect.top);
        odRect.bottom = ODFixedMultiply(yscale, odRect.bottom);
#endif

        ODSetRectProp(ev, su, kODPropWindowRect, kODRect, &odRect);

        // Externalize title as an IText

        Str255 title;
#ifdef _PLATFORM_MACINTOSH_
        GetWTitle(_fPlatformWindow, title);

        ODScriptCode script = FontToScript(GetAppFont());
        ODLangCode lang = GetScriptVariable(script, smScriptLang);

        ODIText* iText = CreateITextPString(script, lang, &title[0]);
        ODSetITextProp(ev, su, kODPropWindowTitle, kODMacIText, iText);
#endif
#ifdef _PLATFORM_OS2_
        // Get the current window title
        WinQueryWindowText(WinWindowFromID(_fPlatformWindow, FID_TITLEBAR),
                           sizeof(title) - 1,  (PCH)&title[0]);

        // Convert the window title and save in the storage unit
        ODIText* iText = CreateITextFromCharPtr((char *) title);
                ODSetITextProp(ev, su, kODPropWindowTitle, kODOS2IText, iText);
#endif
#ifdef _PLATFORM_WIN32_
        // Get the current window title
        GetWindowText(_fPlatformWindow,
                      (PCH)&title[0], sizeof(title) - 1);

        // Convert the window title and save in the storage unit
        ODIText* iText = CreateITextFromCharPtr((char *) title);
                ODSetITextProp(ev, su, kODPropWindowTitle, kODWindowsIText, iText);
#endif
#ifdef _PLATFORM_UNIX_
        // Get the current window title
        char *tmpTitle;
        XtVaGetValues(_fTopLevelShellWidget,
                      XmNtitle, &tmpTitle,
                      NULL);
        strncpy((char *) title, tmpTitle, sizeof(title) - 1);

        // Convert the window title and save in the storage unit
        ODIText* iText = CreateITextFromCharPtr((char *) title);
                ODSetITextProp(ev, su, kODPropWindowTitle, kODAIXIText, iText);
#endif
                DisposeIText(iText);

#ifdef _PLATFORM_MACINTOSH_
                ODSetSShortProp(ev, su, kODPropWindowProcID, kODSShort, somSelf->GetProcID(ev));
#endif
                ODSetBooleanProp(ev, su, kODPropWindowIsVisible, kODBoolean, somSelf->IsShown(ev));
#ifdef _PLATFORM_MACINTOSH_
                ODSetBooleanProp(ev, su, kODPropWindowHasCloseBox, kODBoolean,  somSelf->HasCloseBox(ev));
                ODSetBooleanProp(ev, su, kODPropWindowHasZoomBox, kODBoolean,  somSelf->HasZoomBox(ev));
                ODSetULongProp(ev, su, kODPropWindowRefCon, kODSLong, somSelf->GetRefCon(ev));
#endif
#ifdef _PLATFORM_OS2_
        // Save the window create flags
        ODSetULongProp(ev, su, kODPropWindowCreateFlags, kODULong, somSelf->GetCreateFlags(ev));

        // Save the window's minimized and maximized flags
        ODSetULongProp(ev, su, kODPropWindowSwpFlags, kODULong, swp.fl);  // 117197
#endif
#ifdef _PLATFORM_WIN32_
        // Save the window create flags
        ODSetULongProp(ev, su, kODPropWindowCreateFlags, kODULong, somSelf->GetCreateFlags(ev));

        // Save the window's minimized and maximized flags
        ODSetULongProp(ev, su, kODPropWindowSwpFlags, kODULong, wp.showCmd);
#endif
#ifdef _PLATFORM_UNIX_
        // AIXtodo - What UNIX info do I want to save?
#endif

                // Other properties

                ODSetBooleanProp(ev, su, kODPropWindowIsResizable, kODBoolean,   somSelf->IsResizable(ev));
                ODSetBooleanProp(ev, su, kODPropWindowIsFloating, kODBoolean, somSelf->IsFloating(ev));
                ODSetBooleanProp(ev, su, kODPropWindowIsRootWindow, kODBoolean, somSelf->IsRootWindow(ev));

                ODSetBooleanProp(ev, su, kODPropShouldShowLinks, kODBoolean, somSelf->ShouldShowLinks(ev));

                // root frame

                ODSetStrongSURefProp(ev, su, kODPropRootFrame, kODStrongStorageUnitRef, _fRootFrame->GetStorageUnit(ev)->GetID(ev));

                // Source Frame (this property is only written if the value is non-NULL)

                if (_fSourceFrame)
                {
                        ODSetStrongSURefProp(ev, su, kODPropSourceFrame, kODStrongStorageUnitRef,
                                                        _fSourceFrame->GetStorageUnit(ev)->GetID(ev));
                }
                else if (ODSUExistsThenFocus(ev, su ,kODPropSourceFrame, kODStrongStorageUnitRef))
                {
                        su->Remove(ev);
                }

                // Annotate the root frame with the window properties
                ODStorageUnit* frameSU = _fRootFrame->GetStorageUnit(ev);
                ODSetStrongSURefProp(ev, frameSU, kODPropWindowProperties, kODStrongStorageUnitRef,
                                                su->GetID(ev));

                _fIsDirty = kODFalse;

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: GetID
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODID  SOMLINK ODWindowGetID(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetID");

        return _fID;
}

//-------------------------------------------------------------------------------------
// ODWindow: SetID
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSetID(ODWindow *somSelf, Environment *ev,
                ODID windowID)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetID");

        _fID = windowID;
}

//-------------------------------------------------------------------------------------
// ODWindow: AcquireStorageUnit
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODStorageUnit*  SOMLINK ODWindowGetStorageUnit(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetStorageUnit");

    SOM_TRY

                if (_fStorageUnit != kODNULL)
                        _fStorageUnit->Internalize(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return _fStorageUnit;
}

//-------------------------------------------------------------------------------------
// ODWindow: SetStorageUnit
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODWindowSetStorageUnit(ODWindow *somSelf, Environment *ev, ODStorageUnit* su)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetStorageUnit");

    SOM_TRY

                _fStorageUnit = su;
                su->Acquire(ev);

                // These below calls are unnecessary because the StdTypIO routines make sure
                // the appropriate property and value exist. -T‚
                /*
                ODSUAddPropValue(ev, kODPropWindowRect, kODRect);
                ODSUAddPropValue(ev, kODPropWindowTitle, kODMacIText);
                ODSUAddPropValue(ev, kODPropWindowProcID, kODSShort);
                ODSUAddPropValue(ev, kODPropWindowIsVisible, kODBoolean);
#ifdef _PLATFORM_MACINTOSH_
                ODSUAddPropValue(ev, kODPropWindowHasCloseBox, kODBoolean);
                ODSUAddPropValue(ev, kODPropWindowHasZoomBox, kODBoolean);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        // Add properties for window create flags and window minimize/maximize
        // state
        ODSUAddPropValue(ev,kODPropWindowCreateFlags, kODULong);
        ODSUAddPropValue(ev,kODPropWindowSwpFlags, kODULong);                 // 117197
#endif
                ODSUAddPropValue(ev, kODPropWindowIsResizable, kODBoolean);
                ODSUAddPropValue(ev, kODPropWindowIsRootWindow, kODBoolean);
                ODSUAddPropValue(ev, kODPropWindowIsFloating, kODBoolean);
#ifdef _PLATFORM_MACINTOSH_
                ODSUAddPropValue(ev, kODPropWindowRefCon, kODSLong);
#endif
                ODSUAddPropValue(ev, kODPropRootFrame, kODStrongStorageUnitRef);
                ODSUAddPropValue(ev, kODPropShouldShowLinks, kODBoolean);
                */

                _fIsDirty = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// ODWindow: GetDraft
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK ODWindowGetDraft(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetDraft");

        return _fDraft;
}

#ifdef _PLATFORM_MACINTOSH_
//-------------------------------------------------------------------------------------
// ODWindow: GetWindowPoint
//
// Description
//-------------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODWindowGetWindowPoint(ODWindow *somSelf, Environment *ev,
                Point* globalPoint, ODPoint* windowPoint)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetWindowPoint");

    Point localCopy = *globalPoint;

        GrafPtr curPort;
        GetPort(&curPort);
        SetPort(_fPlatformWindow);
        SetOrigin(0,0);
        GlobalToLocal(&localCopy);
        SetPort(curPort);
        *windowPoint = (ODPoint)localCopy;
}
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_UNIX_
//-------------------------------------------------------------------------------------
// ODWindow: GetTopLevelShellWidget
//
// Description
//-------------------------------------------------------------------------------------
SOM_Scope Widget SOMLINK ODWindowGetTopLevelShellWidget(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetTopLevelShellWidget");

    return _fTopLevelShellWidget;
}

//-------------------------------------------------------------------------------------
// ODWindow: GetMainWindowWidget
//
// Description
//-------------------------------------------------------------------------------------
SOM_Scope Widget SOMLINK ODWindowGetMainWindowWidget(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetMainWindowWidget");

    return _fMainWindowWidget;
}

//-------------------------------------------------------------------------------------
// ODWindow: GetDrawingAreaWidget
//
// Description
//-------------------------------------------------------------------------------------
SOM_Scope Widget SOMLINK ODWindowGetDrawingAreaWidget(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","GetDrawingAreaWidget");

    return _fDrawingAreaWidget;
}

//-------------------------------------------------------------------------------------
// ODWindow: SetRootChildWindow
//
// Description
//-------------------------------------------------------------------------------------
SOM_Scope void SOMLINK ODWindowSetRootChildWindow(ODWindow *somSelf, Environment *ev,
                                                  ODPlatformWindow rootchild)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetRootChildWindow");

    _fRootChild = rootchild;
}

//-------------------------------------------------------------------------------------
// ODWindow: GetRootChildWindow
//
// Description
//-------------------------------------------------------------------------------------
SOM_Scope ODPlatformWindow SOMLINK ODWindowGetRootChildWindow(ODWindow *somSelf, Environment *ev)
{
    ODWindowData *somThis = ODWindowGetData(somSelf);
    ODWindowMethodDebug("ODWindow","SetRootChildWindow");

    return _fRootChild;
}
#endif

