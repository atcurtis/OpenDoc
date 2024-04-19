/* @(#)Z 1.76 com/src/ui/WinStat.cpp, odui, od96os2, odos29646d 96/11/15 15:28:21 (96/10/29 09:33:52) */
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
/********************************************************************/
/*  IBM Change History (most recent first):                         */
/*                                                                  */
/*   21996 05/15/96 brr Added API's for Popup.                      */
/*                        CreatePopupMenu                           */
/*                        CreatePopupMenu                           */
/*                        AcquireBasePopupMenu                      */
/*                      Renamed GetCurrentPopupMenu to              */
/*                        AcquireCurrentPopupMenu                   */
/*   21498 04/29/96 brr Created CreateMenuBarEx                     */
/*   21467 04/26/96 brr Changed InitMenuBar to InitMenuBarEx        */
/*   21316 04/22/96 brr Changed new param to InitMenuBar to be an   */
/*                       address to an array of submenu ID's rather */
/*                       than the menuID we had tried to do with    */
/*                       defect #21263                              */
/*   21263 04/19/96 brr Changed call to InitMenuBar to pass another */
/*                       parameter that is the menuID.              */
/*   21285 04/19/96 brr Removed #include CmdDefs.xh                 */
/*          2/21/96 jrb Complete Apple DR4 Integration              */
/*  145958 12/06/95 pfe Added Set and Get longname methods          */
/*  144010 11/27/95 st  AdjustPartMenus: allow adjustmenus to go to root part */
/*  142229 11/25/95 st  change window list to CUA '89 style         */
/*  143654 11/13/95 ced Apple DR4 IDL changes.                      */
/*  136667  11/6/95 pfe THROW on null platwindow in RegisterWindow  */
/*  142504  11/2/95 st  move adjust menu items to rlshell           */
/*  131165  9/29/95 st  changes to window title/window list entry   */
/*  135340  9/13/95 BLG Add prompt line text support for popup menus*/
/*          9/08/95 mjk Add Unicode Support                         */
/*  133968  8/24/95 pfe Port of Apple DR3 code                      */
/*  129227  8/15/95 pfe infinite loop in AcquireFrontFloatingWindow */
/*  130696  8/07/95 pfe throw in Internalize if no Externalize      */
/*  128450  7/17/95 faa window title set to file name w/o path      */
/*  124340  6/03/95 st  disable VIEW-OPENAS menu item               */
/*  122761  6/01/95 BLG returning null instead of valid handle      */
/*  123687  6/01/95 BLG OdwindowState::SetDefaultWindowTitles       */
/*                      traped if draft parameter Null put in test  */
/*  107897  5/12/95 st  exception handling in methods for null draft*/
/*                      Also put in 2 methods for popup menus       */
/*  121993  5/11/95 ced Put back in code in SetDefaultWindowTitles  */
/*                      accidently taken out during B1C16 merge for */
/*                      switch list entry and fix buffer overflow   */
/*                      bug with long titles.                       */
/*  119388  4/29/95 st  changes for new menus - need currentmenubar */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
        File:           WinStat.cpp

        Contains:       Definition of ODWindowState class

        Owned by:       Richard Rodseth

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <82>     11/2/95        RR              #1298525, 1298642 Internalize now uses a
                                                                        SUView on the draftsProperties, and checks
                                                                        for reference validity
                <81>    10/26/95        eeh             1296308: deal with non-persistant frames in
                                                                        Internalize
                <80>    10/24/95        RR              #1295596 OpenWindows no longer calls
                                                                        select() during normal document open.
                <79>    10/18/95        RR              1289153: Recover from exceptions creating /
                                                                                                                                                internalizing / registering
                                                                        windows. Added TRY/CATCH in CATCH block of
                                                                        ::INnternalize
                <78>    10/17/95        jpa             1289153: Recover from exceptions creating /
                                                                        internalizing / registering windows.
                <77>    10/16/95        RR              #1293067 Pass shouldDispose to
                                                                        RegisterWindow
                <76>     10/8/95        TJ              Fixes Recomended by Refball
                <75>     10/3/95        eeh             1287095: use ReleaseObject in
                                                                        RegisterWindow
                <74>     10/3/95        TJ              Changes done by RefBall Team
                <73>     9/21/95        RR              # 1285189 AcquireFrontWindow calls
                                                                        FrontWindow() rather than
                                                                        GetFrontNonFloatingWindow
                <72>     9/13/95        TÇ              1282067 FB3:  UI temp var before SOM_TRY
                <71>     9/12/95        RR              #1274439 Don't activate/deactivate windows
                                                                        when in background
                <70>      9/6/95        RR              #(1236387) Deactivate before suspend.
                                                                        Reactivate after Resume
                <69>      9/1/95        RR              # 1279100/1280338 Added IsActive test to
                                                                        SelectODWindow
                <68>     8/26/95        TÇ              1274606 FB2: Patching Remarks
                <67>     8/25/95        JBS             1263078 FB: fix part editor swapping
                <66>     8/15/95        RR              # 1233767 Use window iterator in
                                                                        SuspendResume
                <65>     8/12/95        TÇ              1276812 Need to use TempObjs and TempRefs
                                                                        for exception safety and to avoid TRY
                                                                        blocks, 1276807 Opt./Bug: use StdTypIO
                                                                        routines for portable streaming & smaller
                                                                        footprint, 1276806 Optimization: use
                                                                        kODFalse instead of kODTrue in comparisons
                <64>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <63>      8/2/95        VL              1270320: Correct refcounting in
                                                                        SetCurrentMenuBar and SetBaseMenuBar.
                <62>     6/30/95        RR              1242642 BB Ref counting fixes in
                                                                        HideODWindow.
                <61>     6/28/95        RR              1242642 BB Mostly ref counting. AddWindow
                                                                        and RemoveWindow adjust ref counts. Window
                                                                        iterator skips items marked for lazy
                                                                        deletion.
                <60>     6/26/95        TÇ              1242642 BB:Fix refcounting bugs
                <59>     6/25/95        TÇ              1242642 BB: Turn on ODDebug warning if
                                                                        refcount is wrong in
                                                                        ODRefCntObjectsomUninit.
                <58>     6/22/95        RR              #1245283 Undoable frame deletion
                                                                        #1209427 Changed private api between
                                                                        iterator and iteratee. Allow deletion while
                                                                        iterating
                <57>     6/19/95        jpa             Added hook to check for path pop-up
                                                                        (cmd-click title bar) [1259398]
                <56>     6/15/95        RR              #1256879 Get->AcquireCurrentMenuBar
                <55>      6/8/95        RR              #1257260 Collapse base classes. #1214898
                                                                        StdTypes.r -> ODTypes.r
                <54>     5/31/95        RR              #1251403 Release after AcquirePart #1251980
                                                                        Missing release in GetFrontRootWindow
                <53>     5/26/95        RR              #1251403: Multithreading naming support
                <52>     5/25/95        jpa             List.h --> LinkList.h [1253324]
                <51>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
                                                                        refcount
                <50>     5/10/95        RR              # 1234319. Check for rootFrameList before
                                                                        internalizing
                <49>      5/4/95        eeh             1242889: get drafts string from resource
                <48>      5/2/95        RR              # 1244133 Fixed SetDefaultWindowTitles to
                                                                        avoid extra bogus characters
                <47>     4/28/95        RR              1211085 Remove 5$ comments
                <46>     4/14/95        TÇ              With RR & CG: #1194507 DR/BB:title bar of a
                                                                        draft window doesn't reveal it is a draft
                                                                        or which draft it is
                <45>     4/13/95        RR              #1216618 Added ODVolatile
                <44>      4/7/95        RR              #1216618 Added SOM_TRY etc.
                <43>      4/6/95        RR              # 1236361 Call Deactivate in HideODWindow.
                                                                        Remove redundant code to prevent
                                                                        deadstripping of PlatformFile
                <42>      4/4/95        RR              # 1220104 Use ODObjectsAreEqual
                <41>     3/22/95        RR              #1225420, 1227993 Parts internalize
                                                                        windows. Modified OpenWindow, Externalize,
                                                                        Internalize. Added RegisterWindowForFrame
                <40>     3/10/95        RR              # 1225861 Send Suspend/resume to invisible
                                                                        windows
                <39>      3/7/95        RR              # 1151165 Cleaned up AdjustPartMenus #
                                                                        1220929 Added fMenuFocus
                <38>      3/3/95        CC              # 1205622: Ensure activate event is
                                                                        delivered after modal dialog is dismissed.
                <37>      3/1/95        RR              # 1205622 ::OpenWindows now supports
                                                                        re-opening an open draft by bringing its
                                                                        windows to the front.
                <36>     2/24/95        jpa             Use ODNewRgn. [1220810]
                <35>     2/22/95        RR              # 1213850 Call AdjustMenus for root part of
                                                                        active window before calling it for menu
                                                                        focus
                <34>     2/20/95        TÇ              #1221861 BB: Incomplete AOCE support needs
                                                                        to be removed from OpenDoc
                <33>     1/31/95        RR              # 1209165 Rewrote HandleAOCEEvent. #1209552
                                                                        Call SendBehind in HideODWindow
                <31>     1/26/95        VL              #???: Use updated Storage Unit Ref API.
                <30>     1/25/95        RR              #1211853 Removed session parameter from
                                                                        CreateMenuBar. Pass _fSession to
                                                                        InitMenuBar
                <29>     1/23/95        RR              # 1211853 Added CreateMenuBar
                <28>      1/4/95        eeh             1209165: added test to
                                                                        HandleAOCEMailerEvent
                <27>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                <26>    12/19/94        eeh             1192626: change ::Internalize not to
                                                                        duplicate windows already open
                <25>    11/28/94        RR              Check for AOCE presence
                <24>     11/1/94        RR              #1196761 Don't exit HandleAOCEMailerEvent
                                                                        without handling updates in inactive mailer
                                                                        windows
                <23>    10/18/94        RR              Used AcquireWindow(id) to validate windows in
                                                                        CLoseWindows, in case part closed
                                                                        subsidiary windows
                <22>     9/29/94        RA              1189812: Mods for 68K build.
                <21>     9/23/94        VL              1155579, 1184272: Use StorUtil to
                                                                        create/get container and its file.
                <20>     9/22/94        eeh             #1154961 AddAOCEMailer takes additional
                                                                        param.
                <19>     9/19/94        eeh             #1164891: check for userCanceledErr after
                                                                        SMPMailerEvent call.
                <18>      9/1/94        RR              #1176805 Release old base menu bar
                <17>      9/1/94        CC              RADAR #1181971 - missing factory methods
                                                                        (for JBS)
                <16>     8/29/94        RR              #1171772 DOn't call SelectWindow
                <15>     8/26/94        TÇ              #1181761 rename obsolete kOD IDs to correct
                                                                        kODStrong/WeakStorageUnitRefs
                <14>     8/26/94        VL              1183174: Use updated cloning APIs.
                <13>     8/25/94        RR              Added private AcquireBaseMenuBar. Set
                                                                        generation of menubar in SetBaseMenuBar
                <12>     8/19/94        TÇ              #1180922 Need to Stop using obsolete types
                                                                        (kOD ID)
                <11>     8/18/94        jpa             Filled in CreateCanvas [1180387]
                <10>     8/16/94        JBS             1180387: add CreateCanvas()
                 <9>     8/15/94        JBS             1181138: add frameType to CreateFrame();
                                                                        1181156: UI API Cleanup
                 <8>      8/3/94        VL              1153123: Storage to ODStor.
                 <7>     7/27/94        eeh             fix use of Point and Rect (honesty to SOM…)
                 <6>     7/21/94        eeh             fix FixUpMailerWindow
                 <5>     7/15/94        TÇ              make sure PlatformFile code is not
                                                                        deadstripped
                 <4>      7/8/94        RR              Converted HandleAOCEMailerEvent
                 <3>     6/27/94        eeh             add Environment* parameters to
                                                                        AOCEHelperObj method calls
                 <2>     9/27/94        RR              Call InitBaseWindowState
                 <1>     9/26/94        RR              first checked in

        To Do:
        In Progress:

*/

#ifdef DEBUG
// By defining this macro, SOM doesn't define it and I am able to use the
// MyMethodDebug as a break point for IPMD debugging
#define ODWindowStateMethodDebug(c,m) MyMethodDebug(c,m)
static void MyMethodDebug(char *c, char *m);
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

#define ODWindowState_Class_Source
#define VARIABLE_MACROS
#include <WinStat.xih>

#ifndef SOM_ODWindowIterator_xh
#include <WinIter.xh>
#endif

#ifndef _WINUTILM_
#include "WinUtilM.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _UIDEFS_
#include "UIDefs.h"
#endif
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODMenuBar_xh
#include <MenuBar.xh>
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#ifndef SOM_ODPopup_xh
#include <Popup.xh>
#endif

// Includes for NLS support for window title
#include <ODMessag.h> // Header file for catalog IDs
#include <stdio.h>    // Used for sprintf for window title string
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#include <odres.h>
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_Module_Apple_defined
#include <Part.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODCanvas_xh
#include <Canvas.xh>
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

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

//#ifndef SOM_Module_OpenDoc_Commands_defined
//#include <CmdDefs.xh>
//#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __GESTALTEQU__
#include <GestaltEqu.h>
#endif

#ifndef _DLOGUTIL_
#include <DlogUtil.h>
#endif

#ifndef _USERSRCM_
#include <UseRsrcM.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _STORUTIL
#include <StorUtil.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __LOWMEM__
#include <LowMem.h> // For WindowList global
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _ODDEBUG_
#include "ODDebug.h"    // Adkins -- added
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _WINPOPM_
#include "WinPopM.h"
#endif


// New Window API uses a new "WindowRef" type as an opaque window type. So that we can compile
// with or without the new Windows.h, define WindowRef if it's not already defined:      --jpa
#ifndef STRICT_WINDOWS
#define WindowRef WindowPeek
#endif
#endif // _PLATFORM_MACINTOSH_

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

#ifndef SOM_ODSession_xh
#include "odsessn.xh"
#endif

#ifndef SOM_DocumentManager_xh
#include "docmgr.xh"
#endif
#endif

#define ODDebugActivates 0

#ifdef DEBUG
// This method is only used in the debug case as a break point for IPMD
static void MyMethodDebug(char *c, char *m)
{
  SOMMethodDebug(c,m);
}
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODWindowState
#endif

#include "WinStatB.cpp" // Platform-independent methods, if any

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
// Define the constant string for the docshell window class
const char *kODDocumentWindowClass = "ODDocumentWindowClass";
#endif

#ifdef _PLATFORM_OS2_ // CED

#ifdef _UNICODE_
char szMsg1[Str255];
#endif

//----------------------------------------
// Mac style window manipulation functions

inline ODBoolean SendBehind(ODPlatformWindow aWindow, ODPlatformWindow behindWindow)
{
   return WinSetWindowPos(aWindow, behindWindow, 0, 0, 0, 0, SWP_ZORDER);
}

inline ODBoolean BringToFront(ODPlatformWindow aWindow)
{
   return WinSetWindowPos(aWindow, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER);
}

inline void SelectWindow(ODPlatformWindow aWindow)
{
   WinFocusChange(HWND_DESKTOP, aWindow, FC_NOBRINGTOTOP | FC_NOSETFOCUS);

   // If the current focus window is still the window being passed in.  It won't
   // be if ExcFocus has accomplished its WinSetFocus due to a mouse click or
   // a part has changed focus to some other window (embedded PM control perhaps).
   if (WinQueryFocus(HWND_DESKTOP) == aWindow)
   {
       // This means previous attempts to set the focus to the client window
       // most likely failed in ExcFocus.cpp.  Set focus to the client area
       // so keyboard entry will work.
       WinSetFocus(HWND_DESKTOP, WinWindowFromID(aWindow, FID_CLIENT));
   }
}

inline ODBoolean ShowHide(ODPlatformWindow windowToShow, ODBoolean visibility)
{
   return WinShowWindow( windowToShow, visibility );
}

inline ODBoolean HiliteWindow(ODPlatformWindow aWindow, ODBoolean activate)
{
   if (activate)
      return WinSetActiveWindow(HWND_DESKTOP, aWindow);
   else
      return WinSetActiveWindow(HWND_DESKTOP, HWND_DESKTOP);
}
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_

//----------------------------------------
// Mac style window manipulation functions

inline ODBoolean SendBehind(ODPlatformWindow aWindow, ODPlatformWindow behindWindow)
{
   return SetWindowPos(aWindow, behindWindow, 0, 0, 0, 0,
                       SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOMOVE);
}

inline ODBoolean BringToFront(ODPlatformWindow aWindow)
{
   return SetWindowPos(aWindow, HWND_TOP, 0, 0, 0, 0,
                       SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOMOVE);

// Old Novell code.  However, BringWindowToTop also activates the window and
// we don't want this.
// return BringWindowToTop(aWindow);
}

inline void SelectWindow(ODPlatformWindow aWindow)
{
   SetActiveWindow(aWindow);
}

inline ODBoolean ShowHide(ODPlatformWindow windowToShow, ODBoolean visibility)
{
   return ShowWindow( windowToShow, visibility ? SW_SHOWNA : SW_HIDE);
}

inline ODBoolean HiliteWindow(ODPlatformWindow aWindow, ODBoolean activate)
{
   if (activate)
      return SetActiveWindow(aWindow) ? kODTrue : kODFalse;
   else
      return SetActiveWindow(HWND_DESKTOP) ? kODTrue : kODFalse;
}
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
//----------------------------------------
// MapNotify eventhandler and atom definitions
extern "C" void MapNotifyEventHandler(Widget widget, XtPointer client_data, XEvent *event, Boolean *cont);
const String ODWinStatMapNotifyPropertyAtom = "OpenDoc:WinStat:MyMapNotifyProperty:String";
Atom MyMapNotifyProperty = 0;

//----------------------------------------
// Mac style window manipulation functions

// The windows passed in must be children of the root window
inline void SendBehind(Display *display,
                       ODPlatformWindow aWindow,
                       ODPlatformWindow behindWindow)
{
   Window windows[2];
   windows[0] = behindWindow;
   windows[1] = aWindow;
   XRestackWindows(display,
                   windows,
                   2);
   XSync(display, False);
}
#endif // _PLATFORM_UNIX_

#ifdef _PLATFORM_MACINTOSH_
static ODBoolean IsFrontProcess()
{
        ProcessSerialNumber     currentPSN;
        ProcessSerialNumber     frontPSN;
        OSErr                           getFrontProcessResult;
        OSErr                           getCurrentProcessResult;
        ODBoolean                               isSameProcess = kODFalse;

        // Compare this process and the front process
        getFrontProcessResult = GetFrontProcess(&frontPSN);
        getCurrentProcessResult = GetCurrentProcess(&currentPSN);

        if ((getFrontProcessResult == noErr) && (getCurrentProcessResult == noErr))
                SameProcess(&frontPSN, &currentPSN, &isSameProcess);

        return isSameProcess;
}


SOM_Scope void  SOMLINK ODWindowStateDeactivateFrontWindows(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateDeactivateFrontWindows");

    SOM_TRY

                WindowPtr                       firstDocWindow;
                WindowPtr                       secondDocWindow;
                WindowPtr                       window;
                WindowListIterator      iter;

                firstDocWindow = somSelf->GetFrontNonFloatingPlatformWindow(ev);
                if (firstDocWindow != kODNULL)
                        secondDocWindow = GetNextWindow(firstDocWindow);

                window = iter.First();
                while (iter.IsNotComplete() && (window != secondDocWindow))
                {
                        if (GetWindowVisible(window) != kODFalse)
                        {
                                somSelf->DeactivateWindow(ev,window);
                        }
                        window = iter.Next();
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODWindowStateActivateFrontWindows(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateActivateFrontWindows");

        SOM_TRY

                WindowPtr                       firstDocWindow;
                WindowPtr                       secondDocWindow;
                WindowPtr                       window;
                WindowListIterator      iter;

                firstDocWindow = somSelf->GetFrontNonFloatingPlatformWindow(ev);
                if (firstDocWindow != kODNULL)
                        secondDocWindow = GetNextWindow(firstDocWindow);

                window = iter.First();
                while (iter.IsNotComplete() && (window != secondDocWindow))
                {
                        if (GetWindowVisible(window) != kODFalse)
                        {
                                somSelf->ActivateWindow(ev,window);
                        }
                        window = iter.Next();
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
//@ST - popup menus
SOM_Scope ODPopup*  SOMLINK ODWindowStateCopyBasePopup(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCopyBasePopup");

    // If there is a popup, return a pointer to the copy of the popup,
    // otherwise return null
    return _fBasePopup ? (ODPopup*) _fBasePopup->Copy(ev) : (ODPopup*) kODNULL;
}

SOM_Scope void  SOMLINK ODWindowStateSetBasePopup(ODWindowState *somSelf, Environment *ev,
    ODPopup* thePopup)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSetBasePopup");

    SOM_TRY

        // If the current popup is different than the one passed in
        if (!ODObjectsAreEqual(ev, _fBasePopup, thePopup))
        {
            // Release the currently held popup
            ODReleaseObject(ev, _fBasePopup);

            // If the new popup is non-null
            if (thePopup)
            {
                // Acquire the new popup
                thePopup->Acquire(ev);
            }

            // Save away the pointer to the new popup
            _fBasePopup = thePopup;
        }

    SOM_CATCH_ALL
        SOM_ENDTRY
}
#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_ || _PLATFORM_UNIX_


SOM_Scope ODWindow*  SOMLINK ODWindowStateAcquireFrontWindow(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireFrontWindow");

        ODWindow*       odWindow = kODNULL;

        SOM_TRY

#ifdef _PLATFORM_UNIX_
                Window *windows = kODNULL;
                unsigned int numWindows;
                XSync(_fDisplay, False);
                WindowPtr window = FrontWindow(_fScreen,
                                               &windows,
                                               &numWindows);

                if (window != kODNULL)
                {
                        somSelf->ResetRootChildren(ev, kODTrue);
                        if (somSelf->IsODWindowRootChild(ev,window))
                                odWindow = somSelf->AcquireODWindowRootChild(ev,window);
                        somSelf->ResetRootChildren(ev, kODFalse);
                }

                XFree(windows);
#else
                WindowPtr window = FrontWindow();

                if (window != kODNULL)
                {
                        if (somSelf->IsODWindow(ev,window))
                                odWindow = somSelf->AcquireODWindow(ev,window);
                }
#endif

        SOM_CATCH_ALL
        SOM_ENDTRY

        // AcquireODWindow increments ref count
        //if (odWindow)
        //      odWindow->Acquire(ev);
        return odWindow;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateAcquireFrontFloatingWindow(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireFrontFloatingWindow");

        ODWindow* frontFloatingWindow = kODNULL;

        SOM_TRY

                WindowListIterator      iter;

#ifdef _PLATFORM_UNIX_
                somSelf->ResetRootChildren(ev, kODTrue);
                WindowPtr       platformWindow = iter.First(_fScreen);
#else
                WindowPtr       platformWindow = iter.First();
#endif
                while (iter.IsNotComplete() && (frontFloatingWindow == kODNULL))
                {
#ifdef _PLATFORM_UNIX_
                        ODWindow*       odWindow = somSelf->AcquireODWindowRootChild(ev,platformWindow);
#else
                        ODWindow*       odWindow = somSelf->AcquireODWindow(ev,platformWindow);
#endif
                        if (odWindow != kODNULL)
                        {
                                if (odWindow->IsFloating(ev) != kODFalse)
                                {
                                        frontFloatingWindow = odWindow;
                                }
                                else
                                {
                                        ODReleaseObject(ev, odWindow);  // -- TÇ: released only if we don't want it.
                                }
                        }
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                       // I believe Apple will want this fix to, since without it
                       // this is a infinite loop :) [pfe] [129227]
                       // (Unless there is only one window)
            platformWindow = iter.Next();
#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_ || _PLATFORM_UNIX_
                }
#ifdef _PLATFORM_UNIX_
                somSelf->ResetRootChildren(ev, kODFalse);
#endif

        SOM_CATCH_ALL

                frontFloatingWindow = kODNULL;

        SOM_ENDTRY

        // AcquireODWindow increments ref count
        //if (frontFloatingWindow)
        //      frontFloatingWindow->Acquire(ev);
        return frontFloatingWindow;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateAcquireFrontRootWindow(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireFrontRootWindow");

        ODWindow* odWindow = kODNULL;

        SOM_TRY


#ifdef _PLATFORM_WIN32_
        // If running as an OLE Server

        if (_fSession->GetOleManager(ev)->IsOleServer()) {

            // Get the shell's active document manager

            DocumentManager *docMgr;
            docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);

            // If the shell has an active document manager

            if (docMgr) {

                // Get the OLE wrapper
                OleWrapper *wrapper;
                wrapper = docMgr->GetOleWrapper(ev);

                // If there is an OLE wrapper
                if (wrapper) {

                    // Return the root document window from the OLE wrapper.  This would have been
                    // previously set either in SelectODWindow or ShowODWindow
                    WindowPtr platformWindow;

                    platformWindow = wrapper->GetRootDocumentWindow();
                    odWindow = somSelf->AcquireODWindow(ev,platformWindow);
                    return odWindow;
                }
            }
        }
#endif


                WindowListIterator      iter;

#ifdef _PLATFORM_UNIX_
                somSelf->ResetRootChildren(ev, kODTrue);
                for (WindowPtr platformWindow = iter.First(_fScreen);
#else
                for (WindowPtr platformWindow = iter.First();
#endif
                        iter.IsNotComplete();
                        platformWindow = iter.Next())
                {
#ifdef _PLATFORM_UNIX_
                        odWindow = somSelf->AcquireODWindowRootChild(ev,platformWindow);
#else
                        odWindow = somSelf->AcquireODWindow(ev,platformWindow);
#endif
                        if (odWindow && odWindow->IsRootWindow(ev))
                                break;
                        else
                                ODReleaseObject(ev, odWindow);
                }

#ifdef _PLATFORM_UNIX_
                somSelf->ResetRootChildren(ev, kODFalse);
#endif

        SOM_CATCH_ALL

                odWindow = kODNULL;

        SOM_ENDTRY

        // AcquireODWindow increments ref count
        //if (odWindow)
        //      odWindow->Acquire(ev);
        return odWindow;        // -- TÇ: was acquired in the for loop above.
}

SOM_Scope void  SOMLINK ODWindowStateInitWindowState(ODWindowState *somSelf, Environment *ev,
                ODSession* session)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateInitWindowState");

        LinkedList* linkedList = kODNULL; ODVolatile(linkedList);

        SOM_TRY

                /* Moved from somInit. SOM itself sets fields to zero
                _fWindowList = kODNULL;
                _fSession = kODNULL;
                _fBaseMenuBar = kODNULL;
                _fCurrentMenuBar = kODNULL;
                _fNextID = 0;
                _fMenuFocus = 0;
                _fIteratorCount = 0;
                */

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                /* Moved from somInit. SOM itself sets fields to zero
        _fCurrentPopupMenu = kODNULL;
        _fBasePopup = kODNULL;
        */

        // Allocate storage for the long document name and initialize it to
        // an empty string.
        _fDocLongName = new char[1]; //145958
        _fDocLongName[0] = '\0';     //145958
#endif

                somSelf->InitObject(ev);
                _fSession = session;
                _fMenuFocus = _fSession->Tokenize(ev,kODMenuFocus);
                linkedList = new LinkedList;
                _fWindowList = linkedList;

#ifdef _PLATFORM_OS2_
      // If the document shell window class is not already registered
      CLASSINFO classinfo;
      if (!WinQueryClassInfo(_fSession->GetWindowSystemData(ev)->hab,
                             kODDocumentWindowClass,
                             &classinfo))
      {
          // Register the document shell window class
          if(!WinRegisterClass(_fSession->GetWindowSystemData(ev)->hab,
                               kODDocumentWindowClass,
                               _fSession->GetWindowSystemData(ev)->windowProc,
                               CS_SIZEREDRAW | CS_MOVENOTIFY,
                               12L))
          {
              // Throw an error
              THROW(kODErrUndefined);
          }
      }

#endif

#ifdef _PLATFORM_WIN32_
      // If the document shell window class is not already registered
      WNDCLASS existingWC;
      if (!GetClassInfo(_fSession->GetWindowSystemData(ev)->instance,
                        kODDocumentWindowClass,
                        &existingWC))
      {
          // Register the document shell window class
          WNDCLASS wc;
          wc.style         = CS_DBLCLKS;
          wc.lpfnWndProc   = _fSession->GetWindowSystemData(ev)->windowProc;
          wc.cbClsExtra    = 0;
          wc.cbWndExtra    = 12;
          wc.hInstance     = _fSession->GetWindowSystemData(ev)->instance;
          wc.hIcon         = LoadIcon(_fSession->GetWindowSystemData(ev)->resInstance,
                                      MAKEINTRESOURCE(IDR_PRODUCTINFO));
          wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
          wc.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1);
          wc.lpszMenuName  = NULL;
          wc.lpszClassName = kODDocumentWindowClass;

          if(!RegisterClass(&wc))
          {
              // Throw an error
              THROW(kODErrUndefined);
          }
      }
#endif

#ifdef _PLATFORM_UNIX_
      // Cache the display, screen and appcontext
      _fDisplay    = _fSession->GetWindowSystemData(ev)->display;
      _fScreen     = _fSession->GetWindowSystemData(ev)->screen;
      _fAppContext = _fSession->GetWindowSystemData(ev)->appContext;
      _fApplicationShell = _fSession->GetWindowSystemData(ev)->appShell;

      // Create MapNotifyProperty global atom
      MyMapNotifyProperty = XmInternAtom(_fDisplay,
                                         ODWinStatMapNotifyPropertyAtom,
                                         False);
#endif

        SOM_CATCH_ALL

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
      delete _fDocLongName;
      _fDocLongName = NULL;
#endif

                ODDeleteObject(linkedList);

        SOM_ENDTRY
}

#define OLE_EXIT_MAX_NAME  256

SOM_Scope void  SOMLINK ODWindowStateSetCurrentMenuBar(ODWindowState *somSelf, Environment *ev,
                ODMenuBar* theMenuBar)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSetCurrentMenuBar");

        SOM_TRY

                if (!ODObjectsAreEqual(ev, _fCurrentMenuBar, theMenuBar))
                {
                        ODReleaseObject(ev, _fCurrentMenuBar);

#ifdef _PLATFORM_WIN32_
                        // If we're running as an OLE server
                        if (_fSession->GetOleManager(ev)->IsOleServer()) {

                            // Get OLE wrapper if it exists
                            OleWrapper *wrapper;
                            wrapper = ODOLEManager::GetShell()->GetActiveDocumentManager(ev)->GetOleWrapper(ev);

                            // If the OLE wrapper exists
                            if (wrapper)
                            {
                                // Call the wrapper to destroy the current in-place menu
                                if ((wrapper->fWFlg & WFLG_UIACTIVATED) &&
                                    (wrapper->fActiveDocumentWindow == wrapper->fRootDocumentWindow))
                                    wrapper->InPlaceMenuDestroy();
                            }
                        }
#endif

                        if (theMenuBar)
                        {
                                theMenuBar->Acquire(ev);
#ifdef _PLATFORM_WIN32_
                                // If we're running as an OLE server
                                if (_fSession->GetOleManager(ev)->IsOleServer()) {

                                    // Get OLE wrapper if it exists
                                    OleWrapper *wrapper;
                                    wrapper = ODOLEManager::GetShell()->GetActiveDocumentManager(ev)->GetOleWrapper(ev);

                                    // If the OLE wrapper exists
                                    if (wrapper)
                                    {
                                        // Create a new in-place menu from the current
                                        // menu bar, and set the new in-place menu bar
                                        if ((wrapper->fWFlg & WFLG_UIACTIVATED) &&
                                            (wrapper->fActiveDocumentWindow == wrapper->fRootDocumentWindow)) {
                                            wrapper->InPlaceMenuCreate(theMenuBar->GetSystemMenus(ev));
                                            wrapper->InPlaceMenuSet();
                                        }

                                        if (wrapper->fpszObjName) {
                                            // Change the "Exit" menu item on the Document
                                            // menu to "Exit & Return to <compound document>"
                                            char  resourceString[OLE_EXIT_MAX_NAME], szTemp[OLE_EXIT_MAX_NAME];

                                            HINSTANCE  hInstance = _fSession->GetWindowSystemData(ev)->resInstance;
                                            LoadString( hInstance, IDS_EXITANDRETURN, resourceString, OLE_EXIT_MAX_NAME );
                                            wsprintf( szTemp, resourceString, wrapper->fpszObjName );
                                            ModifyMenu( theMenuBar->GetSystemMenus(ev), DOC_EXIT, MF_STRING, DOC_EXIT, szTemp);
                                        }
                                    }
                                }
#endif
                        }

                        _fCurrentMenuBar = theMenuBar;

                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
SOM_Scope ODPopup*  SOMLINK ODWindowStateAcquireCurrentPopupMenu(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireCurrentPopupMenu");

        SOM_TRY

        if (_fCurrentPopupMenu)
                _fCurrentPopupMenu->Acquire(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY
        return _fCurrentPopupMenu;

}

SOM_Scope ODPopup*  SOMLINK ODWindowStateAcquireBasePopupMenu(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireBasePopupMenu");

        SOM_TRY

        if (_fBasePopup)
                _fBasePopup->Acquire(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return _fBasePopup;
}
#endif

SOM_Scope ODMenuBar*  SOMLINK ODWindowStateAcquireCurrentMenuBar(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetCurrentMenuBar");

        SOM_TRY

        if (_fCurrentMenuBar)
                _fCurrentMenuBar->Acquire(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY
        return _fCurrentMenuBar;
}

SOM_Scope ODMenuBar*  SOMLINK ODWindowStateAcquireBaseMenuBar(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireBaseMenuBar");

        SOM_TRY

        if (_fBaseMenuBar)
                _fBaseMenuBar->Acquire(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return _fBaseMenuBar;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateAddWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* window)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAddWindow");

        SOM_TRY

                if (window)
                {
                        ODBoolean found = kODFalse;
                        WindowLink* foundLink = kODNULL;

                        LinkedListIterator iter(_fWindowList);

                        for ( WindowLink* link = (WindowLink*) iter.First();
                                        iter.IsNotComplete();
                                        link = (WindowLink*) iter.Next())
                        {
                                if (!link->ShouldRemove() && ODObjectsAreEqual(ev, link->fWindow, window))
                                {
                                        found = kODTrue;
                                        foundLink = link;
                                        break;
                                }
                        }
                        if (!found)
                        {
                                // There is a problem here. We assume that the window is always going to be
                                // in front. We may need to change the Window API to handle the case when
                                // someone actually wants a window at the back.

#ifdef _PLATFORM_UNIX_
                                // In the AIX Case, placement behind another window doesn't
                                // work with widgets, so I have to use the widget's
                                // ancestor which is a child of the root window.  This is
                                // because the window manager has graciously reparented
                                // my widget window.  However, BringToFront will work on a
                                // widget's window.

                                // Order of functionality is also important here.  The
                                // GetLastFloatingPlatformWindow must come before
                                // adding the new window using AddLast so the new window
                                // won't be considered.  The AddLast must come before
                                // the ResetRootChildren so the root child window for the
                                // new ODWindow will be determined prior to the SendBehind
                                // call.

                                WindowPtr       lastFloatingWindow = somSelf->GetLastFloatingPlatformWindow(ev);

                                // Acquire and add the window to the list
                                window->Acquire(ev);
                                _fNextID++;
                                WindowLink* link = new WindowLink(_fNextID, window);
                                window->SetID(ev,_fNextID);
                                THROW_IF_NULL(link);
                                _fWindowList->AddLast(link);

                                // If there was already a floating window
                                if (lastFloatingWindow != kODNULL)
                                {
                                        // Determine rootchild windows
                                        somSelf->ResetRootChildren(ev, kODTrue);

                                        // Move the new window behind the old
                                        // floating platform window
                                        SendBehind(_fDisplay,
                                                   window->GetRootChildWindow(ev),
                                                   lastFloatingWindow);

                                        somSelf->ResetRootChildren(ev, kODFalse);
                                }
                                else
                                {
                                        // Raise the new window to the top of the
                                        // stacking order
                                        XRaiseWindow(_fDisplay,
                                                     window->GetPlatformWindow(ev));
                                        XSync(_fDisplay, False);
                                }
#else
                                WindowPtr       platformWindow = window->GetPlatformWindow(ev);
                                WindowPtr       lastFloatingWindow = somSelf->GetLastFloatingPlatformWindow(ev);
                                if (lastFloatingWindow != kODNULL) {
                                        SendBehind(platformWindow, lastFloatingWindow);
                                }
                                else {
                                        BringToFront(platformWindow);
                                }

                                window->Acquire(ev);
                                _fNextID++;
                                WindowLink* link = new WindowLink(_fNextID, window);
                                window->SetID(ev,_fNextID);
                                THROW_IF_NULL(link);
                                _fWindowList->AddLast(link);
#endif
                        }
                }

        SOM_CATCH_ALL

                window = kODNULL;

        SOM_ENDTRY

        return window;
}

SOM_Scope void  SOMLINK ODWindowStateRemoveWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* oldWindow)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateRemoveWindow");

        SOM_TRY

                ODBoolean found = kODFalse;
                WindowLink* foundLink = kODNULL;

                LinkedListIterator iter(_fWindowList);

                for ( WindowLink* link = (WindowLink*) iter.First();
                                iter.IsNotComplete();
                                link = (WindowLink*) iter.Next())
                {
                        if (!link->ShouldRemove() && ODObjectsAreEqual(ev, link->fWindow, oldWindow))
                        {
                                found = kODTrue;
                                foundLink = link;
                                break;
                        }
                }
                if (found)
                {
                        ODWindow* window = foundLink->fWindow;
                        ODReleaseObject(ev, window);
                        if (_fIteratorCount > 0)
                        {
                                foundLink->fRemove = kODTrue;
                        }
                        else
                        {
                                _fWindowList->Remove(*foundLink);
                                ODDeleteObject(foundLink);
                        }
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

#ifdef _PLATFORM_MACINTOSH_
SOM_Scope ODWindow*  SOMLINK ODWindowStateFindODWindow(ODWindowState *somSelf, Environment *ev,
                Point* globalPoint)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateFindODWindow");

        ODWindow* odWindow = kODNULL;

        SOM_TRY

                WindowPtr window;
                FindWindow(*globalPoint, &window);
                if (window)
                        odWindow = somSelf->AcquireODWindow(ev,window);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return odWindow;        // -- TÇ: ODWindowStateFindODWindow is an 'acquire' function
}
#endif // _PLATFORM_MACINTOSH_

SOM_Scope void  SOMLINK ODWindowStateReleaseWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* window)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateReleaseWindow");

        SOM_TRY

#ifdef _PLATFORM_UNIX_
                if (window == _fActiveWindow)
                {
                    // Ensure this field is NULL so we don't have a bad pointer.
                    _fActiveWindow = kODNULL;
                }
#endif

                ODDeleteObject(window);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

#ifdef _PLATFORM_MACINTOSH_
SOM_Scope void  SOMLINK ODWindowStateSuspendResume(ODWindowState *somSelf, Environment *ev,
                ODEventData* event)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSuspendResume");

        SOM_TRY

                const short kResumeMask = 0x01; // High byte suspend/resume event

                ODBoolean       goingToBackground = (event->message & kResumeMask) == 0;
                // WindowPtr    frontWindow = somSelf->GetFrontNonFloatingPlatformWindow(ev);
                //ODWindow* odWindow = kODNULL;

        #if ODDebug && ODDebugActivates
                somPrintf("WindowState Suspend/Resume %d\n", !goingToBackground);
        #endif

                ODWindowIterator* iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev); window = iter->Next(ev))
                {
                        window->SuspendResume(ev,event);
                }
                ODDeleteObject(iter);

                // Deactivate the front doc window and any floaters that haven't been hidden.
                // The system only unhilites the frontmost window

                if (goingToBackground)
                {
        #if ODDebug && ODDebugActivates
                somPrintf("WindowState Suspend Event - deactivate front windows\n");
        #endif
                        somSelf->DeactivateFrontWindows(ev);
                }
                else
                {
        #if ODDebug && ODDebugActivates
                somPrintf("WindowState Resume Event - activate front windows\n");
        #endif
                        somSelf->ActivateFrontWindows(ev);
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}
#endif // _PLATFORM_MACINTOSH_

SOM_Scope void  SOMLINK ODWindowStateSelectODWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* window)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSelectODWindow");

        SOM_TRY

                WindowPtr                       currentFrontWindow;
                WindowPtr                       lastFloatingWindow;
                Boolean                         isFloatingWindow;
                Boolean                         isFrontProcess;
                WindowPtr                       windowToSelect = window->GetPlatformWindow(ev);

#ifdef _PLATFORM_MACINTOSH_
                isFrontProcess = IsFrontProcess();
#endif

#ifdef _PLATFORM_UNIX_
                somSelf->ResetRootChildren(ev, kODTrue);
                Window *windows = kODNULL;
                unsigned int numWindows;
#endif

                if (window->IsFloating(ev) != kODFalse)
                {
                        isFloatingWindow = kODTrue;
#ifdef _PLATFORM_UNIX_
                        currentFrontWindow = (WindowPtr) FrontWindow(_fScreen,
                                                                     &windows,
                                                                     &numWindows);
#else
                        currentFrontWindow = (WindowPtr) FrontWindow();
#endif
                }
                else
                {
                        isFloatingWindow = kODFalse;
                        currentFrontWindow = somSelf->GetFrontNonFloatingPlatformWindow(ev);
                        lastFloatingWindow = somSelf->GetLastFloatingPlatformWindow(ev);
                }

#ifdef _PLATFORM_MACINTOSH_
                // Be fast (and lazy) and do nothing if we don’t have to.
                // The !IsActive test was added because this method might get called when
                // in the background during a Drop.

                if ((currentFrontWindow != windowToSelect) || (!window->IsActive(ev)))
                {

                // Selecting floating windows are easy, since they’re always active

                        if (isFloatingWindow)
                        {
                                BringToFront(windowToSelect);
                        }
                        else
                        {
                                // This replaces the code below.
                                // In order to facilitate a predictable flow of events,
                                // we must use Macintosh Toolbox calls that don't generate activate
                                // events themselves (ie. BringToFront, SendBehind).

                                if (isFrontProcess)
                                        somSelf->DeactivateWindow(ev, currentFrontWindow);

                                if (lastFloatingWindow == kODNULL)
                                        BringToFront(windowToSelect);
                                else
                                        SendBehind(windowToSelect, lastFloatingWindow);

                                //if (isFrontProcess) // Must allow activates for Drop in inactive window
                                        somSelf->ActivateWindow(ev,windowToSelect);

                        }
                }
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        if (isFloatingWindow || lastFloatingWindow == kODNULL) {
          BringToFront(windowToSelect);
        }
        else if (lastFloatingWindow != kODNULL) {
          SendBehind(windowToSelect, lastFloatingWindow);
        }
        SelectWindow(windowToSelect);
#endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_ ||

#ifdef _PLATFORM_UNIX_
        // Again, UNIX is interesting as I can raise and "select"
        // a toplevelshell widget's window.  However, I can
        // only set the stacking order of children of the root
        if (isFloatingWindow || lastFloatingWindow == kODNULL)
        {
          XRaiseWindow(_fDisplay, windowToSelect);
        }
        else if (lastFloatingWindow != kODNULL)
        {
          SendBehind(_fDisplay,
                     window->GetRootChildWindow(ev), lastFloatingWindow);
        }

        // Ensure the window is showing or the SetInputFocus can fail
        somSelf->ShowODWindow(ev, window);

        // See if the window is visible yet.
        XWindowAttributes attributes;
        XGetWindowAttributes(_fDisplay,
                             windowToSelect,
                             &attributes);

        // If the window is already viewable
        if (attributes.map_state == IsViewable)
        {
            // Set input focus to the window
            XSetInputFocus(_fDisplay,
                           windowToSelect,
                           RevertToPointerRoot,
                           CurrentTime);
            XSync(_fDisplay, False);
        }
        else
        {
            // Add a property to the window so XSetInputFocus can
            // be called later when the window
            // is mapped.  XSetInputFocus will cause a
            // protocol error if the window is not yet
            // viewable.
            XChangeProperty(_fDisplay,
                            windowToSelect,
                            MyMapNotifyProperty,
                            XA_STRING,
                            8,
                            PropModeReplace,
                            NULL,
                            0);
        }

        somSelf->ResetRootChildren(ev, kODFalse);

        // Update active window
        somSelf->SaveActiveWindow(ev, window);

        if (windows)
        {
            XFree(windows);
        }
#endif // _PLATFORM_UNIX_

#ifdef _PLATFORM_WIN32_
        // If we're running as an OLE server
        if (_fSession->GetOleManager(ev)->IsOleServer())
        {
            // Get the OLE wrapper
            OleWrapper *wrapper;
            wrapper = ODOLEManager::GetShell()->GetActiveDocumentManager(ev)->GetOleWrapper(ev);

            // If the wrapper exists
            if (wrapper)
            {
                // Set the active document window.  This is later used by
                // GetFrontNonFloatingPlatformWindow
                wrapper->SetActiveDocumentWindow(windowToSelect);

                // Set the root document window, if the window is a root window.
                // This is later used by GetFrontRootWindow
                if (window->IsRootWindow(ev))
                    wrapper->SetRootDocumentWindow(windowToSelect);
            }
        }
#endif

        SOM_CATCH_ALL
        SOM_ENDTRY
} // SelectODWindow


#ifdef _PLATFORM_MACINTOSH_
SOM_Scope void  SOMLINK ODWindowStateDragODWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* window,
                Point* startPoint,
                Rect* draggingBounds)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateDragODWindow");

        SOM_TRY

                // Check for command-click on window title:
                if( TrackWindowPathPopUp(ev,window,*startPoint) )
                        return;

                WindowPtr       windowToDrag = window->GetPlatformWindow(ev);
                Rect            dragRect;
                KeyMap          keyMap;
                GrafPtr         savePort;
                GrafPtr         windowManagerPort;
                RgnHandle       dragRegion;
                RgnHandle       windowContentRegion;
                long            dragResult;
                short           topLimit;
                short           newHorizontalWindowPosition;
                short           newVerticalWindowPosition;
                short           horizontalOffset;
                short           verticalOffset;
                Boolean         commandKeyDown = kODFalse;

                if (WaitMouseUp()) {

                        // Adjust the top of the dragging rectangle so that it’s below the menu bar

                        topLimit = LMGetMBarHeight() + 4;
                        dragRect = *draggingBounds;
                        if (dragRect.top < topLimit)
                                dragRect.top = topLimit;

                // Set up the Window Manager port.

                        GetPort(&savePort);
                        GetWMgrPort(&windowManagerPort);
                        SetPort(windowManagerPort);
                        SetClip(GetGrayRgn());

                        // Check to see if the command key is down.  If it is, don’t bring the window to the
                        // front after the move.  Trying to do Pascal stuff in C is so much fun.  GetKeys()
                        // is a total pain to try to use properly from C, so I’m going to hard code where the
                        // command key is in the KeyMap array.

                        GetKeys(keyMap);
                        if (keyMap[1] & 0x8000)
                                commandKeyDown = kODTrue;

                        if ((commandKeyDown != kODFalse) || (window->IsFloating(ev) == kODFalse)) {

                                if (commandKeyDown == kODFalse)

                                // If there are floating windows, clip the dragging outline to draw behind the floaters.

                                        /* Adkins -- changed WindowPeek to WindowRef */
                                        ClipAbove((WindowRef) somSelf->GetFrontNonFloatingPlatformWindow(ev));
                                else

                                // If the command key was down, clip the outline to draw behind any windows above
                                // the window being dragged.

                                        ClipAbove((WindowRef) windowToDrag); /* Adkins -- changed WindowPeek to WindowRef */

                        }

                        // Create a region to drag

                        dragRegion = ODNewRgn();
                        CopyRgn(GetStructureRegion(windowToDrag), dragRegion);

                        // Drag the window around

                        dragResult = DragGrayRgn(dragRegion, *startPoint, &dragRect, &dragRect, noConstraint, nil);

                        // Restore the port for coordinate conversion.

                        SetPort(savePort);

                        if (dragResult != 0) {
                                horizontalOffset = (ODSShort) (dragResult & 0xFFFF);
                                verticalOffset = (ODSShort) (dragResult >> 16);

                                // Only move it if it stayed inside the dragging box.

                                if (verticalOffset != -32768) {
                                        windowContentRegion = GetContentRegion(windowToDrag);
                                        newHorizontalWindowPosition = (**windowContentRegion).rgnBBox.left + horizontalOffset;
                                        newVerticalWindowPosition = (**windowContentRegion).rgnBBox.top + verticalOffset;

                                        MoveWindow((WindowPtr) windowToDrag, newHorizontalWindowPosition, newVerticalWindowPosition, kODFalse);

                                }
                        }

                        // Bring the window forward if the command key wasn’t down

                        if (commandKeyDown == kODFalse)
                                somSelf->SelectODWindow(ev, window);

                        // Get rid of the dragging region

                        DisposeRgn(dragRegion);
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}
#endif // _PLATFORM_MACINTOSH_

SOM_Scope void  SOMLINK ODWindowStateShowODWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* window)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateShowODWindow");

        SOM_TRY

#ifdef _PLATFORM_MACINTOSH_
                WindowPtr                       windowToShow = window->GetPlatformWindow(ev);
                WindowPtr                       windowBehind;
                ODBoolean                       windowIsInFront = kODFalse;

                if (GetWindowVisible(windowToShow) == kODFalse)
                {

                        // If the window behind the window to show is currently the frontmost document window,
                        // unhighlight it, and highlight the new front window.

                        if (window->IsFloating(ev) == kODFalse)
                        {
                                WindowPtr       frontPlatformWindow = somSelf->GetFrontNonFloatingPlatformWindow(ev);
                                if (frontPlatformWindow == kODNULL)
                                        windowIsInFront = kODTrue;
                                else {
                                        windowBehind = GetNextWindow(windowToShow);
                                        if (windowBehind == frontPlatformWindow)
                                        {
                                                if (windowBehind != kODNULL)
                                                        somSelf->DeactivateWindow(ev,windowBehind);
                                                windowIsInFront = kODTrue;
                                        }
                                }
                        }
                        else
                        {

                                // A floating window is being shown.  Should check to see if a modal window is up before
                                // trying to highlight it.

                                windowIsInFront = kODTrue;
                        }

                        if (windowIsInFront)
                                SetWindowHilite(windowToShow, kODTrue);
                        else
                                SetWindowHilite(windowToShow, kODFalse);

                        // Show the window

                        ShowHide(windowToShow, kODTrue);

                        // If this is the new frontmost document window or a floating window, send it an activate event

                        if (windowIsInFront &&  IsFrontProcess())
                        {
        #if ODDebug && ODDebugActivates
                                        somPrintf("Activate in ShowODWindow for root frame %x\n", window->GetRootFrame(ev));
        #endif
                                window->Activate(ev);
                        }
                }
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_OS2_ // CED
        // Show the window
        WinShowWindow(window->GetPlatformWindow(ev), TRUE);
#endif

#ifdef _PLATFORM_WIN32_
        // If we're running as an OLE server
        if (_fSession->GetOleManager(ev)->IsOleServer())
        {
            // Get the OLE wrapper
            OleWrapper *wrapper;
            wrapper = ODOLEManager::GetShell()->GetActiveDocumentManager(ev)->GetOleWrapper(ev);

            // If the wrapper exists
            if (wrapper)
            {
                // Set the active document window.  This is later used by
                // GetFrontNonFloatingPlatformWindow
                wrapper->SetActiveDocumentWindow(window->GetPlatformWindow(ev));

                // Set the root document window, if the window is a root window.
                // This is later used by GetFrontRootWindow
                if (window->IsRootWindow(ev))
                    wrapper->SetRootDocumentWindow(window->GetPlatformWindow(ev));

                else {    // Edit->OpenSelection case
                    // If we're not being prevented from showing the window.  We will
                    // be prevented from showing in the case where this document is
                    // only being opened so a link target can get updated.
                    if (_fSession->GetWindowSystemData(ev)->showToScreen)
                        ShowWindow(window->GetPlatformWindow(ev), SW_SHOWNA);
                }
            }
        }
        else
        {
            // If we're not being prevented from showing the window.  We will
            // be prevented from showing in the case where this document is
            // only being opened so a link target can get updated.
            if (_fSession->GetWindowSystemData(ev)->showToScreen)
            {
                // Show the window
                ShowWindow(window->GetPlatformWindow(ev), SW_SHOWNA);
            }
        }
#endif

#ifdef _PLATFORM_UNIX_
        // Show the window
        XtMapWidget(window->GetTopLevelShellWidget(ev));
        XSync(_fDisplay, False);
#endif

        SOM_CATCH_ALL
        SOM_ENDTRY
}

// ref-count-neutral static function used in workaround in HideODWindow below
static ODBoolean IsFloating(Environment* ev, ODWindowState* windowState, ODPlatformWindow win)
{
        ODBoolean isFloating = kODFalse;
#ifdef _PLATFORM_UNIX_
        // As this is only called from HideODWindow, I can be assured that
        // ResetRootChildren has already been called and therefore don't
        // also do it here.
        ODWindow* window = windowState->AcquireODWindowRootChild(ev,win);
#else
        ODWindow* window = windowState->AcquireODWindow(ev,win);
#endif
        if (window)
                isFloating = window->IsFloating(ev);
        ODReleaseObject(ev, window);
        return isFloating;
}

SOM_Scope void  SOMLINK ODWindowStateHideODWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* window)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateHideODWindow");

        SOM_TRY

                WindowPtr                       windowToHide = window->GetPlatformWindow(ev);
                WindowPtr                       frontFloater;
                WindowPtr                       frontPlatformWindow;
                WindowPtr                       windowBehind;


#ifdef _PLATFORM_UNIX_
                XSync(_fDisplay, False);
                if (GetWindowVisible(_fDisplay, windowToHide) != kODFalse)
                {
                        // Get the first visible floating window, if any.
                        Window *windows;
                        unsigned int numWindows;
                        frontFloater = (WindowPtr) FrontWindow(_fScreen,
                                                               &windows,
                                                               &numWindows);

                        somSelf->ResetRootChildren(ev, kODTrue);

                        TempODWindow ODFrontFloater = somSelf->AcquireODWindowRootChild(ev,frontFloater);
#else
                if (GetWindowVisible(windowToHide) != kODFalse)
                {
                        // Get the first visible floating window, if any.
                        frontFloater = (WindowPtr) FrontWindow();

                        TempODWindow ODFrontFloater = somSelf->AcquireODWindow(ev,frontFloater);
#endif
                        if ((ODFrontFloater != kODNULL) && (ODFrontFloater->IsFloating(ev) == kODFalse))
                                frontFloater = nil;

                        // Get the first visible document window, if any.

                        frontPlatformWindow = somSelf->GetFrontNonFloatingPlatformWindow(ev);

                        // Deactivate and Hide the window.
                        somSelf->DeactivateWindow(ev,windowToHide);

#ifdef _PLATFORM_UNIX_
                        // Need to hide after SendBehind call or we
                        // will get a protocol error.

                        // Replace windowToHide with the ODWindow's root child window
                        windowToHide = window->GetRootChildWindow(ev);
#else
                        ShowHide(windowToHide, kODFalse);
#endif

                        // If the frontmost floating window is being hidden, move it behind the floating window
                        // behind it, if there is one.

                        if (windowToHide == frontFloater)
                        {
#ifdef _PLATFORM_MACINTOSH_
                                windowBehind = GetNextWindow(windowToHide);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
                // Get window behind window to hide
                                windowBehind = NextWindow(windowToHide);
#endif
#ifdef _PLATFORM_UNIX_
                // Get window behind window to hide
                                windowBehind = NextWindow(windows,
                                                          numWindows,
                                                          windowToHide);
#endif

                        // Only do the rearrangement if there’s another floating window.

        //                      if ((windowBehind != kODNULL) &&
        //                              (((ODWindowBehind = somSelf->AcquireODWindow(ev,windowBehind)) != kODNULL) &&
        //                                      (ODWindowBehind->IsFloating() != kODFalse))) {
        //                              SetNextWindow(windowToHide, GetNextWindow(windowBehind));
        //                              SetNextWindow(windowBehind, windowToHide);
        //                              SetWindowList(windowBehind);
        //                      }
                        }
                        else
                        {

                        // If the frontmost document window is behind hidden, send it behind the window
                        // behind it.
                                TempODWindow            ODWindowBehind = kODNULL;

                                if (windowToHide == frontPlatformWindow)
                                {
                                        windowBehind = windowToHide;
#if 0
// This is commented out in the DR4 code.  I ifdef it to make it more obvious
                                        /* $$$$$ Need to find out why this doesn't work, ref count wise
                                        do
                                        {
                                                windowBehind = GetNextWindow(windowBehind);

                                                if (somSelf->IsODWindow(ev,windowBehind))
                                                {
                                                        ODReleaseObject(ev, ODWindowBehind); // Swapping references inside loop
                                                        ODWindowBehind = somSelf->AcquireODWindow(ev,windowBehind);
                                                }
                                                else
                                                        ODWindowBehind = kODNULL;
                                        }
                                        while (windowBehind &&
                                                        (((ODWindowBehind == kODNULL) || (ODWindowBehind->IsFloating(ev))) ||
                                                          (GetWindowVisible(windowBehind) == kODFalse)));
                                        */
#endif

                                        // Workaround for above loop, to avoid refcounted variable in loop
                                        // Is Floating is an rc-neutral static function above this method
                                        ODBoolean isODWindow = kODFalse;
                                        do
                                        {
#ifdef _PLATFORM_MACINTOSH_
                                                windowBehind = GetNextWindow(windowBehind);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
                        // Get window behind current window
                                                windowBehind = NextWindow(windowBehind);
#endif
#ifdef _PLATFORM_UNIX_
                        // Get window behind current window
                                                windowBehind = NextWindow(windows,
                                                                          numWindows,
                                                                          windowBehind);
                                                isODWindow = somSelf->IsODWindowRootChild(ev,windowBehind);
#else
                                                isODWindow = somSelf->IsODWindow(ev,windowBehind);
#endif
                                        }
                                        while (windowBehind &&
                                                        (((isODWindow == kODFalse) || (IsFloating(ev, somSelf, windowBehind))) ||
#ifdef _PLATFORM_UNIX_
                                                          (GetWindowVisible(_fDisplay,
                                                                            windowBehind) == kODFalse)));
#else
                                                          (GetWindowVisible(windowBehind) == kODFalse)));
#endif
                                        // End workaround
                                        if (windowBehind != kODNULL)
                                        {
                                                // Note: Experiment to fix #1209552. Using GetNextVisible instead of GetNext
                                                // might be better

#ifdef _PLATFORM_UNIX_
                                                SendBehind(_fDisplay,
                                                           windowToHide, windowBehind);
#else
                                                SendBehind(windowToHide, windowBehind);
#endif

                                                // The window behind it is now the front document window.  Highlight it and send it
                                                // and activate event.

#ifdef _PLATFORM_MACINTOSH_
                                                if (IsFrontProcess())
#endif
#ifdef _PLATFORM_UNIX_
                                                // If the last window was an ODWindow
                                                if (isODWindow)
                                                {
                                                    // Acquire the window
                                                    TempODWindow tempwin = somSelf->AcquireODWindowRootChild(ev, windowBehind);

                                                    // Replace the windowBehind with the ODWindow's platformwindow
                                                    windowBehind = tempwin->GetPlatformWindow(ev);
                                                }

#endif
                                                        somSelf->ActivateWindow(ev,windowBehind);
                                        }
                                }

                        }
#ifdef _PLATFORM_UNIX_
                        // Hide the window here.  Has to be after the
                        // SendBehind call or we will get protocol
                        // error from SendBehind.
                        XtUnmapWidget(window->GetTopLevelShellWidget(ev));
                        XSync(_fDisplay, False);

                        XFree(windows);
                        somSelf->ResetRootChildren(ev, kODFalse);
#endif
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

/*
 *  for ODWindowIterator
 */

SOM_Scope ODULong  SOMLINK ODWindowStateAddIterator(ODWindowState *somSelf, Environment *ev,
                ODWindowIterator* iterator)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAddIterator");

        LinkedListIterator* iter = kODNULL; ODVolatile(iter);

        SOM_TRY

                iter = new LinkedListIterator(_fWindowList);
                _fIteratorCount++;

        SOM_CATCH_ALL

                ODDeleteObject(iter);

        SOM_ENDTRY

        return (ODULong) iter;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateFirst(ODWindowState *somSelf, Environment *ev,
                ODULong iteratorID)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateFirst");

        ODWindow* window = kODNULL;

        SOM_TRY

                LinkedListIterator* iterator = (LinkedListIterator*) iteratorID;
                WindowLink* link = (WindowLink*) iterator->First();

                while (link && link->ShouldRemove())
                        link = (WindowLink*) iterator->Next();

                if (link)
                        window = link->fWindow;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return window;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateNext(ODWindowState *somSelf, Environment *ev,
                ODULong iteratorID)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateNext");

        ODWindow* window = kODNULL;

        SOM_TRY

                LinkedListIterator* iterator = (LinkedListIterator*) iteratorID;
                WindowLink* link = (WindowLink*) iterator->Next();

                while (link && link->ShouldRemove())
                        link = (WindowLink*) iterator->Next();

                if (link)
                        window = link->fWindow;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return window;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateLast(ODWindowState *somSelf, Environment *ev,
                ODULong iteratorID)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateLast");

        ODWindow* window = kODNULL;

        SOM_TRY

                LinkedListIterator* iterator = (LinkedListIterator*) iteratorID;
                WindowLink* link = (WindowLink*) iterator->Last();

                while (link && link->ShouldRemove())
                        link = (WindowLink*) iterator->Previous();

                if (link)
                        window = link->fWindow;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return window;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStatePrevious(ODWindowState *somSelf, Environment *ev,
                ODULong iteratorID)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStatePrevious");

        ODWindow* window = kODNULL;

        SOM_TRY

                LinkedListIterator* iterator = (LinkedListIterator*) iteratorID;
                WindowLink* link = (WindowLink*) iterator->Previous();

                while (link && link->ShouldRemove())
                        link = (WindowLink*) iterator->Previous();

                if (link)
                        window = link->fWindow;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return window;
}

SOM_Scope ODBoolean  SOMLINK ODWindowStateIsNotComplete(ODWindowState *somSelf, Environment *ev,
                ODULong iteratorID)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateIsNotComplete");

    ODBoolean isNotComplete = kODFalse;

        SOM_TRY

                LinkedListIterator* iterator = (LinkedListIterator*) iteratorID;
                isNotComplete = iterator->IsNotComplete();

        SOM_CATCH_ALL
        SOM_ENDTRY

        return isNotComplete;
}

SOM_Scope void  SOMLINK ODWindowStateRemoveIterator(ODWindowState *somSelf, Environment *ev,
                ODULong iteratorID)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateRemoveIterator");

        SOM_TRY

                LinkedListIterator* iterator = (LinkedListIterator*) iteratorID;
            ODDeleteObject(iterator);
                _fIteratorCount--;

                if (_fIteratorCount == 0) // Clear items marked for deletion during iteration
                {
                        LinkedListIterator iter(_fWindowList);
                        for ( WindowLink* link = (WindowLink*) iter.First();
                                        iter.IsNotComplete();
                                        link = (WindowLink*) iter.Next())
                        {
                                if (link->ShouldRemove())
                                {
                                        iter.RemoveCurrent();
                                        delete link;
                                }
                        }
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}


SOM_Scope void  SOMLINK ODWindowStateActivateWindow(ODWindowState *somSelf, Environment *ev,
                ODPlatformWindow platformWindow)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateActivateWindow");

        SOM_TRY

                somSelf->HighlightAndActivateWindow(ev, platformWindow, kODTrue);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODWindowStateDeactivateWindow(ODWindowState *somSelf, Environment *ev,
                ODPlatformWindow platformWindow)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateDeactivateWindow");

        SOM_TRY

                somSelf->HighlightAndActivateWindow(ev, platformWindow, kODFalse);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODWindowStateHighlightAndActivateWindow(ODWindowState *somSelf, Environment *ev,
                ODPlatformWindow platformWindow,
                ODBoolean activate)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateHighlightAndActivateWindow");

        SOM_TRY

#if ODDebug && ODDebugActivates
        somPrintf("Activate/Deactivate message from OpenDoc\n");
#endif

                // Note: This optimization doesn't work if we don't dispatch
                // activate events from the Toolbox. eg. if you dismiss a modal dialog
                // the activate calls below don't get made because the Toolbox must have already
                // tweaked the highlighting.

                //if (AcquireWindowHilite(platformWindow) != activate)
                {
#ifdef _PLATFORM_UNIX_
                        // if activating
                        if (activate)
                        {
                            // Set the input focus
                            XSync(_fDisplay, False);
                            XSetInputFocus(_fDisplay,
                                           platformWindow,
                                           RevertToPointerRoot,
                                           CurrentTime);
                        }
                        else
                        {
                            // Set the input focus to the root window
                            XSetInputFocus(_fDisplay,
                                           PointerRoot,
                                           RevertToPointerRoot,
                                           CurrentTime);
                        }
                        XSync(_fDisplay, False);
#else
                        HiliteWindow(platformWindow, activate);

                        if (somSelf->IsODWindow(ev,platformWindow) != kODFalse)
                        {

                                TempODWindow odWindow = somSelf->AcquireODWindow(ev,platformWindow);
                                if (activate == kODFalse)
                                {
        #if ODDebug && ODDebugActivates
                                        somPrintf("Deactivate message from OpenDoc for root frame %x\n", odWindow->GetRootFrame(ev));
        #endif
                                        odWindow->Deactivate(ev);
                                }
                                else
                                {
        #if ODDebug && ODDebugActivates
                                        somPrintf("Activate message from OpenDoc for root frame %x\n", odWindow->GetRootFrame(ev));
        #endif
                                        odWindow->Activate(ev);
                                }
                        }
#endif
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope ODPlatformWindow  SOMLINK ODWindowStateGetLastFloatingPlatformWindow(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetLastFloatingPlatformWindow");

        WindowPtr       lastFloatingWindow = kODNULL;

        SOM_TRY

                WindowPtr       theWindow;
                ODWindow*       odWindow;

#ifdef _PLATFORM_UNIX_
        somSelf->ResetRootChildren(ev, kODTrue);

        // Instantiate an iterator over the windows
        WindowListIterator   iter;

        // Get the first window from the iterator
        theWindow = iter.First(_fScreen);

        lastFloatingWindow = kODNULL;

        // We have to search the entire window list because we don’t know what the windowKind
        // of other windows in the list might be, and we have account for the fact that a modal
        // dialog is up.

        // While more windows to iterate over
        while (iter.IsNotComplete())
        {
            if (somSelf->IsODWindowRootChild(ev,theWindow) != kODFalse)
            {
                odWindow = somSelf->AcquireODWindowRootChild(ev,theWindow);

                if (odWindow->IsFloating(ev) != kODFalse)
                {
                    lastFloatingWindow = theWindow;
                }
                ODReleaseObject(ev, odWindow);
            }

            // Get the next window from the iterator
            theWindow = iter.Next();
        }
        somSelf->ResetRootChildren(ev, kODFalse);
#else

#ifdef _PLATFORM_MACINTOSH_
                theWindow = ::GetWindowList();
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        // Instantiate an iterator over the windows
        WindowListIterator   iter;

        // Get the first window from the iterator
        theWindow = iter.First();
#endif

                lastFloatingWindow = kODNULL;

                // We have to search the entire window list because we don’t know what the windowKind
                // of other windows in the list might be, and we have account for the fact that a modal
                // dialog is up.

#ifdef _PLATFORM_MACINTOSH_
                while (theWindow != kODNULL)
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        // While more windows to iterate over
        while (iter.IsNotComplete())
#endif
                {
                        if (somSelf->IsODWindow(ev,theWindow) != kODFalse)
                        {
                                odWindow = somSelf->AcquireODWindow(ev,theWindow);
                                if (odWindow->IsFloating(ev) != kODFalse)
                                        lastFloatingWindow = theWindow;
                                ODReleaseObject(ev, odWindow);
                        }
#ifdef _PLATFORM_MACINTOSH_
                        theWindow = GetNextWindow(theWindow);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
            // Get the next window from the iterator
            theWindow = iter.Next();
#endif
                }
#endif


        SOM_CATCH_ALL

                lastFloatingWindow = kODNULL;

        SOM_ENDTRY

        return lastFloatingWindow;
}

SOM_Scope ODPlatformWindow  SOMLINK ODWindowStateGetFrontNonFloatingPlatformWindow(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetFrontNonFloatingPlatformWindow");


        WindowPtr       theWindow = kODNULL;
        ODVolatile(theWindow);

        SOM_TRY

#ifdef _PLATFORM_WIN32_
        // If running as an OLE Server
        if (_fSession->GetOleManager(ev)->IsOleServer())
        {
            // Get the shell's active document manager
            DocumentManager *docMgr;
            docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);

            // If the shell has an active document manager
            if (docMgr)
            {
                // Get the OLE wrapper
                OleWrapper *wrapper;
                wrapper = docMgr->GetOleWrapper(ev);

                // If there is an OLE wrapper
                if (wrapper)
                {
                    // Return the active document window from the OLE wrapper.  This would have been
                    // previously set either in SelectODWindow or ShowODWindow
                    return(wrapper->GetActiveDocumentWindow());
                }
            }
        }
#endif

                TempODWindow    odWindow = kODNULL;

                // Get the first visible window in the window list.
#ifdef _PLATFORM_UNIX_
        somSelf->ResetRootChildren(ev, kODTrue);
        Window *windows;
        unsigned int numWindows;

        theWindow = FrontWindow(_fScreen,
                                &windows,
                                &numWindows);

        // while valid hwnd
        while (theWindow != kODNULL)
        {
           // -- TÇ: to balance the above acquire
           ODReleaseObject(ev, odWindow);

           // opendoc window?
           odWindow = somSelf->AcquireODWindowRootChild(ev,theWindow);
           if  (odWindow)
           {
              // I get protocol errors on occasion if the GetWindowVisible
              // function is called against all windows, so I'll only use
              // against ODWindows
              if ((odWindow->IsFloating(ev) == kODFalse) &&
                  (GetWindowVisible(_fDisplay,
                                    theWindow) == kODTrue))
              {
                 break;
              } /* endif */
           } /* endif */

           // Get the next window in the Z-order
           theWindow = NextWindow(windows, numWindows, theWindow);
        } /* endwhile */

        XFree(windows);
        somSelf->ResetRootChildren(ev, kODFalse);
#else
                theWindow = FrontWindow();

                // Keep searching until a visible window whose windowKind is not
                // kApplicationFloaterKind is found, or the end of the window list is reached.

#ifdef _PLATFORM_MACINTOSH_
                while ((theWindow != kODNULL) &&
                                (((odWindow = somSelf->AcquireODWindow(ev,theWindow)) != kODNULL) &&  /* -- TÇ:released down below */
                                         (odWindow->IsFloating(ev) != kODFalse)))
                {
                        ODReleaseObject(ev, odWindow); // -- TÇ: to balance the above acquire
                        do
                        {
                                theWindow = GetNextWindow(theWindow);
                        }
                        while ((theWindow != kODNULL) && (GetWindowVisible(theWindow) == kODFalse));
                }
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        // defect 122761 - returning null instead of valid handle
        // while loop was a mess

        // while valid hwnd
        while (theWindow != kODNULL)
        {
           // -- TÇ: to balance the above acquire
           ODReleaseObject(ev, odWindow);

           // opendoc window?
           odWindow = somSelf->AcquireODWindow(ev,theWindow);
           if  (odWindow)
           {
              if (odWindow->IsFloating(ev)==kODFalse )
              {
                 break;
              } /* endif */
           } /* endif */

           do {
              theWindow = NextWindow(theWindow);
           } while ((theWindow != kODNULL) && (GetWindowVisible(theWindow) == kODFalse));

        } /* endwhile */
#endif // _PLATFORM_OS2 || _PLATFORM_WIN32_
#endif


        SOM_CATCH_ALL

                theWindow = kODNULL;

        SOM_ENDTRY

        // odWindow is released here on destruction of the TempODWindow, if not in the loop above
        return theWindow;

}

SOM_Scope void  SOMLINK ODWindowStatesomUninit(ODWindowState *somSelf)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStatesomUninit");

        Environment* ev = somGetGlobalEnvironment();

        ODDeleteObject(_fWindowList);
        ODSafeReleaseObject(_fBaseMenuBar);             _fBaseMenuBar = kODNULL;
        ODSafeReleaseObject(_fCurrentMenuBar);  _fCurrentMenuBar = kODNULL;

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
    // Release the popup menu
    ODSafeReleaseObject(_fBasePopup); _fBasePopup = kODNULL;

    // If there was a document long name allocated
    if (_fDocLongName)
    {
       // Free the storage for the document long name
       delete _fDocLongName;
       _fDocLongName = kODNULL;
    }
#endif
}

SOM_Scope ODSize  SOMLINK ODWindowStatePurge(ODWindowState *somSelf, Environment *ev,
                ODSize size)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStatePurge");

    return 0;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateRegisterWindow(ODWindowState *somSelf, Environment *ev,
                ODPlatformWindow newWindow,
                ODType frameType,
                ODBoolean isRootWindow,
                ODBoolean isResizable,
                ODBoolean isFloating,
                ODBoolean shouldSave,
                ODBoolean shouldDispose,
                ODPart* rootPart,
                ODTypeToken viewType,
                ODTypeToken presentation,
                ODFrame* sourceFrame)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateRegisterWindow");

        ODWindow* window = kODNULL; ODVolatile(window);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
    // If the newWindow was passed in as a null
    if (newWindow == kODNULL)
    {
       // Throw an error
       THROW(kODErrIllegalNullInput);
    } /* endif */
#endif

        SOM_TRY

                window = new ODWindow();
                if (window == kODNULL)
                        THROW(kODErrCannotCreateWindow); // Note: Should we just use kODErrOutOfMemory?

                window->InitWindow(ev, newWindow, frameType,
                                                        isRootWindow, isResizable, isFloating, shouldSave, shouldDispose,
                                                        rootPart, viewType, presentation, sourceFrame);
                somSelf->AddWindow(ev,window);

        SOM_CATCH_ALL

                if (window)
                {
                        TRY
                                window->CloseAndRemove(ev);
                        CATCH_ALL
                        ENDTRY
                        window = kODNULL;
                }

        SOM_ENDTRY

        return window;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateRegisterWindowForFrame(ODWindowState *somSelf, Environment *ev,
                ODPlatformWindow newWindow,
                ODFrame* frame,
                ODBoolean isRootWindow,
                ODBoolean isResizable,
                ODBoolean isFloating,
                ODBoolean shouldSave,
                ODBoolean shouldDispose,
                ODFrame* sourceFrame)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateRegisterWindowForFrame");

        ODWindow* window = kODNULL; ODVolatile(window);

        SOM_TRY

                window = new ODWindow();
                if (window == kODNULL)
                        THROW(kODErrCannotCreateWindow); // Note: Should we just use kODErrOutOfMemory?

                window->InitWindowForFrame(ev, newWindow, frame,
                                                        isRootWindow, isResizable, isFloating, shouldSave,  shouldDispose, sourceFrame);
                somSelf->AddWindow(ev,window);

        SOM_CATCH_ALL

                if (window)
                {
                        TRY
                                window->Close(ev);
                        CATCH_ALL
                        ENDTRY
                        window = kODNULL;
                }

        SOM_ENDTRY

        return window;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateAcquireWindow(ODWindowState *somSelf, Environment *ev,
                ODID id)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireWindow");

        ODWindow* window = kODNULL;

        SOM_TRY

/*              LinkedListIterator iter(_fWindowList);

                for ( WindowLink* link = (WindowLink*) iter.First();
                                iter.IsNotComplete();
                                link = (WindowLink*) iter.Next())
                {
                        if (!link->ShouldRemove() && (link->fID == id))
                        {
                                window = link->fWindow;
                                break;
                        }
                }
*/
                        ODWindowIterator* iter = kODNULL;

                        iter = somSelf->CreateWindowIterator(ev);

                        for (ODWindow* win = iter->First(ev); iter->IsNotComplete(ev);
                                        win = iter->Next(ev))
                        {
                                if (win->GetID(ev) == id)
                                {
                                        window = win;
                                        break;
                                }
                        }
                        ODDeleteObject(iter);

                        if (window)
                                window->Acquire(ev);

        SOM_CATCH_ALL

                window = kODNULL;

        SOM_ENDTRY

        return window;
}

SOM_Scope void  SOMLINK ODWindowStateInternalize(ODWindowState *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateInternalize");

        OrderedCollection* wsuIDCollection = kODNULL; ODVolatile(wsuIDCollection);

        SOM_TRY

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
        // If the draft parameter was passed in as a null - 107897 ST
        if (draft == kODNULL)
        {
           // Throw an error
           THROW(kODErrIllegalNullDraftInput);
        } /* endif */
#endif

                // To avoid calling Select in OpenWindows, we use this piece of cross-method state
                // A better solution might be to change ODOpenDraft to distinguish between the
                // opendraft and activateopendraft (from the dialog) cases. See also #1295739

                _fActivateOnOpen = (somSelf->GetRootWindowCount(ev, draft) > 0);

                ODULong offset, offsetLimit;
                ODStorageUnitRef suRef;

                TempODStorageUnit draftProps = draft->AcquireDraftProperties(ev); // -- TÇ tempobj'd

                if (ODSUExistsThenFocus(ev, draftProps,kODPropRootFrameList, kODStrongStorageUnitRefs))
                {
                        offsetLimit = draftProps->GetSize(ev);

                        // Get collection of windowstorageunit ids belonging to the draft here.  We'll
                        // use it below to ensure that we don't reopen open windows.

                        wsuIDCollection = new OrderedCollection;
#ifdef _PLATFORM_MACINTOSH_
                        WindowPtr theWindow = ::GetWindowList();
                        while (theWindow != kODNULL)
#endif
#ifdef _PLATFORM_UNIX_
            somSelf->ResetRootChildren(ev, kODTrue);

            // Instantiate an iterator over the windows
            WindowListIterator   iter;

            // Get the first window from the iteration
            WindowPtr theWindow = iter.First(_fScreen);

            // While iteration over the windows is not complete
            while (iter.IsNotComplete())
            {
                if (somSelf->IsODWindowRootChild(ev,theWindow) != kODFalse)
                {
                    TempODWindow odWindow = somSelf->AcquireODWindowRootChild(ev,theWindow);
                    if ( ODObjectsAreEqual(ev, odWindow->GetDraft(ev) , draft) )
                    {
                        ODStorageUnit* storageU = odWindow->GetRootFrame(ev)->GetStorageUnit(ev);
                        if ( storageU )
                        {
                            wsuIDCollection->AddLast( (ElementType)(storageU->GetID(ev)) );
                        }
                        else
                        {
                            // Throw an error
                            THROW(kODErrDocNotSaved); // [pfe] 130696
                        }
                    }
                }
                // Get the next window from the iterator
                theWindow = iter.Next();
            }
            somSelf->ResetRootChildren(ev, kODFalse);
#else

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
            // Instantiate an iterator over the windows
            WindowListIterator   iter;

            // Get the first window from the iteration
            WindowPtr theWindow = iter.First();

            // While iteration over the windows is not complete
            while (iter.IsNotComplete())
#endif
                        {
                                if (somSelf->IsODWindow(ev,theWindow) != kODFalse)
                                {
                                        TempODWindow odWindow = somSelf->AcquireODWindow(ev,theWindow);
                                        if ( ODObjectsAreEqual(ev, odWindow->GetDraft(ev) , draft) )
                                        {
                                                ODStorageUnit* storageU = odWindow->GetRootFrame(ev)->GetStorageUnit(ev);
                                                if ( storageU )
                                                        wsuIDCollection->AddLast( (ElementType)(storageU->GetID(ev)) );
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
                        else
                        {
                            // Throw an error
                            THROW(kODErrDocNotSaved); // [pfe] 130696
                        }
#endif
                                        }
                                }
#ifdef _PLATFORM_MACINTOSH_
                                theWindow = GetNextWindow(theWindow);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
                // Get the next window from the iterator
                theWindow = iter.Next();
#endif
                        }
#endif

                        TempODStorageUnitView draftPropsView = draftProps->CreateView(ev);
                        for (offset = 0; offset < offsetLimit; offset += sizeof(ODStorageUnitRef))
                        {
                                draftPropsView->SetOffset(ev,offset);
                                StorageUnitViewGetValue(draftPropsView,ev,sizeof(ODStorageUnitRef), (ODValue)&suRef);
                                if (draftPropsView->IsValidStorageUnitRef(ev, suRef))
                                {
                                        ODStorageUnitID wsuID = draftPropsView->GetIDFromStorageUnitRef(ev,suRef);
                                        if ( !wsuIDCollection->Contains((ElementType)wsuID) )
                                        {
                                                ODFrame *rootFrame = draft->AcquireFrame(ev, wsuID);
                                                if (rootFrame)
                                                {
                                                        TRY{
                                                                TempODPart rootPart = rootFrame->AcquirePart(ev);
                                                                rootPart->Open(ev, rootFrame);
                                                        }CATCH_ALL{
                                                                        TRY
                                                                                rootFrame->Close(ev);
                                                                        CATCH_ALL
                                                                        ENDTRY
                                                                RERAISE;
                                                        }ENDTRY
                                                        rootFrame->Release(ev);
                                                }
                                        }
                                }
                        }
                        ODDeleteObject(wsuIDCollection);

                        somSelf->SetDefaultWindowTitles(ev,draft);
                }

        SOM_CATCH_ALL

                ODDeleteObject(wsuIDCollection);

        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODWindowStateExternalize(ODWindowState *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateExternalize");

        SOM_TRY

                somSelf->SetDefaultWindowTitles(ev,draft);

                ODULong offset, offsetLimit;
                ODFrame* rootFrame = kODNULL;

                TempODStorageUnit draftProps = draft->AcquireDraftProperties(ev);

                ODSUForceFocus(ev, draftProps, kODPropRootFrameList, kODStrongStorageUnitRefs);

                offset = 0;
                offsetLimit = draftProps->GetSize(ev);

                // Use the Window Manager list, since it's ordered
                // Note: we write the windows out back to front, since we create them in internalize
                // with the "behind" pointer -1. This only works because ShowODWindow does not bring
                // the newly shown window in front of the other invisible ones, as does ShowWindow.

                WindowListIterator iter;

#ifdef _PLATFORM_UNIX_
                somSelf->ResetRootChildren(ev, kODTrue);
                for (WindowPtr window = iter.Last(_fScreen);
                     iter.IsNotComplete();
                     window = iter.Previous())
                {
                        if (somSelf->IsODWindowRootChild(ev,window))
                        {
                                TempODWindow odWindow = somSelf->AcquireODWindowRootChild(ev,window); // DMc refcount - make temp
#else
                for (WindowPtr window = iter.Last(); iter.IsNotComplete(); window = iter.Previous())
                {
                        if (somSelf->IsODWindow(ev,window))
                        {
                                TempODWindow odWindow = somSelf->AcquireODWindow(ev,window); // DMc refcount - make temp
#endif
                                if (odWindow
                                        && (odWindow->ShouldSave(ev))
                                        && (ODObjectsAreEqual(ev, odWindow->GetDraft(ev), draft)))
                                {
                                        if (odWindow->GetStorageUnit(ev) == kODNULL)
                                        {
                                                TempODStorageUnit       su = draft->CreateStorageUnit(ev);
                                                odWindow->SetStorageUnit(ev, su);
                                        }
                                        odWindow->Externalize(ev);

                                        rootFrame = odWindow->GetRootFrame(ev);
                                        draftProps->SetOffset(ev,offset);       // $opt: Won't the offset be correct automatically? -TC
                                        ODSetStrongSURefProp(ev, draftProps, kODNULL, kODNULL, rootFrame->GetStorageUnit(ev)->GetID(ev));
                                        offset += sizeof(ODStorageUnitRef); // $opt: Won't the offset be correct automatically? -TC
                                }
                        }
                }

#ifdef _PLATFORM_WIN32_
                if (_fSession->GetOleManager(ev)->IsOleServer())
                {
                    // If a window is embedded in an OLE container, it won't appear as a child
                    // of the desktop.  So, we have to make a second pass of the ODWindows
                    // and externalize any windows which are not children of the desktop

                    // Instantiate an iterator over the ODWindow's
                    ODWindowIterator* iter2 = somSelf->CreateWindowIterator(ev);

                    for (ODWindow* odWindow = iter2->First(ev);
                         iter2->IsNotComplete(ev);
                         odWindow = iter2->Next(ev))
                    {
                            if (!odWindow->IsRootWindow(ev))
                                continue;

                            // Get the platform window
                            ODPlatformWindow platwin = odWindow->GetPlatformWindow(ev);

                            // Get the platform window's parent
                            HWND platwinParent = GetParent(platwin);

                            // If the parent of the platform window is not the desktop - this window
                            // hasn't been externalize yet.
                            if ((platwinParent != HWND_DESKTOP) &&
                                (platwinParent != GetDesktopWindow()))
                            {
                                    if ((odWindow->ShouldSave(ev)) &&
                                        (ODObjectsAreEqual(ev, odWindow->GetDraft(ev), draft)))
                                    {
                                            if (odWindow->GetStorageUnit(ev) == kODNULL)
                                            {
                                                    TempODStorageUnit       su = draft->CreateStorageUnit(ev);
                                                    odWindow->SetStorageUnit(ev, su);
                                            }
                                            odWindow->Externalize(ev);

                                            rootFrame = odWindow->GetRootFrame(ev);
                                            draftProps->SetOffset(ev,offset);       // $opt: Won't the offset be correct automatically? -TC
                                            ODSetStrongSURefProp(ev, draftProps, kODNULL, kODNULL, rootFrame->GetStorageUnit(ev)->GetID(ev));
                                            offset += sizeof(ODStorageUnitRef); // $opt: Won't the offset be correct automatically? -TC
                                    }
                            }
                    }

                    ODDeleteObject(iter2);
                }

#endif

#ifdef _PLATFORM_UNIX_
                somSelf->ResetRootChildren(ev, kODFalse);
#endif

                if (offset < offsetLimit)
                        draftProps->DeleteValue(ev,offsetLimit - offset);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODWindowStateSetDefaultWindowTitles(ODWindowState *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSetDefaultWindowTitles");

#if defined(_PLATFORM_WIN32_)
        // If we're running as an OLE server - OLE wrapper sets titlebars
        if (_fSession->GetOleManager(ev)->IsOleServer())
            return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
    // If the draft parameter was passed in as a null - 123687 BLG
    if (draft == kODNULL)
    {
       // Throw an error
       THROW(kODErrIllegalNullDraftInput);
    } /* endif */

    char *windowName = NULL;
#endif

        //!!! Needs work. Should add counter
        // Sets default window titles for all root windows of the given draft,
        // based on the file name.

        ODWindowIterator* iter = kODNULL; ODVolatile(iter);
        PlatformFile* file = kODNULL; ODVolatile(file);

        SOM_TRY

                ODContainer* container = draft->GetDocument(ev)->GetContainer(ev);

                file = GetPlatformFileFromContainer(ev, container);

#ifdef _PLATFORM_MACINTOSH_
                char    windowName[256];
#endif

                iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                window = iter->Next(ev))
                {
                        if ( (ODObjectsAreEqual(ev, window->GetDraft(ev), draft)) && window->IsRootWindow(ev) )
                        {
#ifdef _PLATFORM_MACINTOSH_
                                file->GetAsciiName(windowName,235);
#endif

                                ODULong draftNum = 0;

                                TRY
                                        draftNum = GetDraftNumFromDraft(ev, draft);
                                CATCH_ALL
                                ENDTRY

#ifdef _PLATFORM_MACINTOSH_
                                if (draftNum != 0)
                                {
                                        CToPascalString(windowName);

                                        const ODSShort kMaxNumberSuffixLength = 10;
                                        char theNum[kMaxNumberSuffixLength];
                                        NumToString(draftNum, (StringPtr)theNum); // Macintosh Specific

                                        ODSLong savedRefNum;
                                        BeginUsingLibraryResources(savedRefNum);
                                        ReplaceIntoString( kODDraftTextResID, (StringPtr)windowName,
                                                        (StringPtr)theNum, (StringPtr)windowName );
                                        EndUsingLibraryResources(savedRefNum);
                                        PascalToCString((StringPtr)windowName);
                                }
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
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
                                              WINSTAT_20,
                                              "OpenDoc - %s Draft %d");
#else
                                    ret_string = "OpenDoc - %s Draft %d";
#endif
                                }
                                else
                                {
#ifdef __IBMCPP__
                                    ret_string =
                                      catgets(ODcat_handle,
                                              WINSTAT_SET,
                                              WINSTAT_10,
                                              "OpenDoc - %s");
#else
                                    ret_string = "OpenDoc - %s";
#endif
                                }

                                // Determine where the first token comes from
                                char *token1;

                                // If there is a long document name
                                if (_fDocLongName[0] != '\0')
                                {
                                    // Note that for an in-memory container,
                                    // this is the only valid path.  An
                                    // additional test for this case would
                                    // probably be approriate in the future.

                                    // Use the long document name
                                    token1 = _fDocLongName;
                                }
                                else
                                {
                                    // Get the filename of the document
                                    file->GetAsciiName(fileName,
                                                       kODMaxFileNameSize);

                                    // Use the filename
                                    token1 = fileName;
                                }


                                // Determine length of window title
                                // Space for message, first token
                                // and draft number if we need it
                                unsigned int length = strlen(ret_string) +
                                                      strlen(token1)     +
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
                                                token1,
                                                draftNum);
                                    }
                                    else
                                    {
                                        // Create the window title
                                        sprintf(windowName,
                                                ret_string,
                                                token1);
                                    }
#endif

                                    window->SetWindowTitle(ev, windowName);

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                                    delete windowName;
                                    windowName = NULL;
#endif

                                }
                        }
                }
                ODDeleteObject(iter);
                ODDeleteObject(file);

        SOM_CATCH_ALL

                ODDeleteObject(iter);
                ODDeleteObject(file);
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                delete windowName;
#endif

        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODWindowStateOpenWindows(ODWindowState *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateOpenWindows");

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
    // If the draft parameter is null - 107897 ST
    if (draft == kODNULL)
    {
       // Throw an error
       THROW(kODErrIllegalNullDraftInput);
    } /* endif */
#endif

        ODWindowIterator* iter = kODNULL; ODVolatile(iter);

        SOM_TRY

                if (draft)
                {
                        iter = somSelf->CreateWindowIterator(ev);

                        // Windows were externalize/internalized back to front
                        for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                        window = iter->Next(ev))
                        {
                                if (ODObjectsAreEqual(ev, window->GetDraft(ev) , draft))
                                {
                                        window->Open(ev); // A no-op if its already open
#ifdef _PLATFORM_MACINTOSH_
                                        if (_fActivateOnOpen && window->IsShown(ev)) // The draft is already open
                                                window->Select(ev);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                                        if (window->IsShown(ev)) // The draft is already open
                                        {
                                           // Show and select the window
                                           window->Show(ev);
                                           window->Select(ev);
                                        }
#endif
                                }
                        }
                        ODDeleteObject(iter);
                }
                _fActivateOnOpen = kODFalse;

        SOM_CATCH_ALL

                _fActivateOnOpen = kODFalse;
                ODDeleteObject(iter);

        SOM_ENDTRY

}

SOM_Scope void  SOMLINK ODWindowStateCloseWindows(ODWindowState *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCloseWindows");

        SOM_TRY

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
    // If the draft parameter is null - 107897 ST
    if (draft == kODNULL)
    {
       // Throw an error
       THROW(kODErrIllegalNullDraftInput);
    } /* endif */
#endif

                // For now, since we can't delete while iterating, we make a copy of the list. Yuck.

/*              LinkedList windowListCopy;

                {
                        LinkedListIterator iter(_fWindowList);

                        for ( WindowLink* link = (WindowLink*) iter.First();
                                        iter.IsNotComplete();
                                        link = (WindowLink*) iter.Next())
                        {
                                WindowLink* linkCopy = new WindowLink(link->fID, link->fWindow);
                                THROW_IF_NULL(linkCopy);
                                windowListCopy.AddLast(linkCopy);
                        }
                }

                LinkedListIterator citer(&windowListCopy);
                for ( WindowLink* clink = (WindowLink*) citer.First();
                                citer.IsNotComplete();
                                clink = (WindowLink*) citer.Next())
                {
                        // Note: Use ID check in case Part closed a subsidiary window
                        // This works now because this is a copy of the list.
                        // Should probably do a deferred close scheme instead

                        ODWindow* window = somSelf->AcquireWindow(ev, clink->fID);
                        // ODWindow* window = link->fWindow;
                        if (window && (ODObjectsAreEqual(ev, window->GetDraft(ev), draft )))
                        {
                                window->Close(ev);
                        }
                        ODReleaseObject(ev, window);
                }
                windowListCopy.DeleteAllLinks();
*/

                ODWindowIterator* iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev); window = iter->Next(ev))
                {
                        if (ODObjectsAreEqual(ev, window->GetDraft(ev), draft ))
                        {
                                window->Acquire(ev); // Close Releases
                                window->Close(ev);
                        }
                }
                ODDeleteObject(iter);


        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope ODUShort  SOMLINK ODWindowStateGetWindowCount(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetWindowCount");

        ODUShort count = 0;

        SOM_TRY

                if (_fWindowList)
                        count = _fWindowList->Count();

        SOM_CATCH_ALL
        SOM_ENDTRY

        return count;
}

SOM_Scope ODUShort  SOMLINK ODWindowStateGetRootWindowCount(ODWindowState *somSelf, Environment *ev,
                ODDraft* draft)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetRootWindowCount");

        ODUShort count = 0;
        ODWindowIterator* iter = kODNULL; ODVolatile(iter);

        SOM_TRY

                if (draft)
                {
                        iter = somSelf->CreateWindowIterator(ev);

                        for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                        window = iter->Next(ev))
                        {
                                if (window->IsRootWindow(ev) && (ODObjectsAreEqual(ev, window->GetDraft(ev), draft)))
                                {
                                        count++;
                                }
                        }
                        ODDeleteObject(iter);
                }

        SOM_CATCH_ALL

                        ODDeleteObject(iter);

        SOM_ENDTRY

        return count;
}

SOM_Scope ODUShort  SOMLINK ODWindowStateGetTotalRootWindowCount(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetTotalRootWindowCount");

        //!!! Should get count directly from collection

        ODUShort count = 0;
        ODWindowIterator* iter = kODNULL; ODVolatile(iter);

        SOM_TRY

                iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                window = iter->Next(ev))
                {
                        if (window->IsRootWindow(ev))
                                count++;
                }
                ODDeleteObject(iter);

        SOM_CATCH_ALL

                ODDeleteObject(iter);

        SOM_ENDTRY

        return count;
}

SOM_Scope ODBoolean  SOMLINK ODWindowStateIsODWindow(ODWindowState *somSelf, Environment *ev,
                ODPlatformWindow aWindow)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateIsODWindow");

        ODWindowIterator* iter = kODNULL; ODVolatile(iter);
        ODBoolean isODWindow = kODFalse;

        SOM_TRY

                iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                window = iter->Next(ev))
                {
                        if (window && (aWindow == window->GetPlatformWindow(ev)))
                        {
                                isODWindow = kODTrue;
                                break;
                        }
                }
                ODDeleteObject(iter);


        SOM_CATCH_ALL

                ODDeleteObject(iter);

        SOM_ENDTRY

        return isODWindow;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateAcquireODWindow(ODWindowState *somSelf, Environment *ev,
                ODPlatformWindow aWindow)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireODWindow");

        ODWindowIterator* iter = kODNULL; ODVolatile(iter);
        ODWindow* odWindow = kODNULL;

        SOM_TRY

                iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                window = iter->Next(ev))
                {
                        if (window && (aWindow == window->GetPlatformWindow(ev)))
                        {
                                odWindow = window;
                                break;
                        }
                }
                ODDeleteObject(iter);

                if (odWindow)
                        odWindow->Acquire(ev);

        SOM_CATCH_ALL

                ODDeleteObject(iter);
                odWindow = kODNULL;

        SOM_ENDTRY

        return odWindow;
}

SOM_Scope ODWindowIterator*  SOMLINK ODWindowStateCreateWindowIterator(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCreateWindowIterator");

        ODWindowIterator* iter = kODNULL; ODVolatile(iter);

        SOM_TRY

                iter = new ODWindowIterator;
                THROW_IF_NULL(iter); // "new" does not throw for SOM objects
                iter->InitWindowIterator(ev, somSelf);

        SOM_CATCH_ALL

                ODDeleteObject(iter);

        SOM_ENDTRY

        return iter;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateAcquireActiveWindow(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireActiveWindow");

        ODWindow* aWindow = kODNULL;

        SOM_TRY

#ifdef _PLATFORM_UNIX_
                // If a window was previously saved away as the active window.
                // This is necessary on AIX since the currently-active window
                // is not necessarily in the front.  The AIX desktop supports
                // activation without bringing a window to the front.
                aWindow = _fActiveWindow;
                if (aWindow)
                {
                    // Acquire the window
                    aWindow->Acquire(ev);
                }
                // else - use the same old logic
                else
                {
#endif
                    WindowPtr platformWindow = kODNULL;

                    //      • First we need to find out the frontmost window
                    platformWindow = somSelf->GetFrontNonFloatingPlatformWindow(ev);

                    if (platformWindow)
#ifdef _PLATFORM_UNIX_
                    {
                        somSelf->ResetRootChildren(ev, kODTrue);
                        aWindow = somSelf->AcquireODWindowRootChild(ev,platformWindow);
                        somSelf->ResetRootChildren(ev, kODFalse);
                    }
#else
                        aWindow = somSelf->AcquireODWindow(ev,platformWindow);
#endif

#ifdef _PLATFORM_UNIX_
                }
#endif

        SOM_CATCH_ALL
        SOM_ENDTRY

        // Not necessary to increment ref count, because AcquireODWindow has done it
        //if (aWindow)
        //      aWindow->Acquire(ev);
        return aWindow;
}

SOM_Scope void  SOMLINK ODWindowStateSetBaseMenuBar(ODWindowState *somSelf, Environment *ev,
                ODMenuBar* theMenuBar)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSetBaseMenuBar");

        SOM_TRY

                // $$$$$ Change this error code to be more explicit. -VL
                ASSERT(theMenuBar, kODErrIllegalNullInput);

                if (!ODObjectsAreEqual(ev, theMenuBar, _fBaseMenuBar))
                {
#ifdef _PLATFORM_MACINTOSH_
                    ODULong generation;
                        if (_fBaseMenuBar)
                                generation = _fBaseMenuBar->GetGeneration(ev) + 1;
                        else
                                generation = 1;
#endif
                        ODReleaseObject(ev, _fBaseMenuBar);
                        theMenuBar->Acquire(ev);
                        _fBaseMenuBar = theMenuBar;
#ifdef _PLATFORM_MACINTOSH_
                        _fBaseMenuBar->SetGeneration(ev,generation);
#endif
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope ODMenuBar*  SOMLINK ODWindowStateCopyBaseMenuBar(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCopyBaseMenuBar");

    ODMenuBar* menuBar = kODNULL;

    SOM_TRY

                if (_fBaseMenuBar)
                        menuBar = _fBaseMenuBar->Copy(ev);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return menuBar; // -- TÇ: CopyBaseMenuBar is an 'acquire' function.
}

SOM_Scope void  SOMLINK ODWindowStateAdjustPartMenus(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAdjustPartMenus");

        SOM_TRY

                TempODFrame targetFrame
                        = _fSession->GetArbitrator(ev)->AcquireFocusOwner(ev,_fMenuFocus);

                // To support root menu items like Print.
                // Other platforms may choose to add a new focus for this

                ODFrame* rootFrame;
                { TempODWindow window = somSelf->AcquireActiveWindow(ev) ;
                  rootFrame = window ? window->GetRootFrame(ev) : kODNULL;
                }

                if (rootFrame)
                {
                        TempODPart targetPart = rootFrame->AcquirePart(ev); // -- TÇ tempobj'd
                        targetPart->AdjustMenus(ev,rootFrame);
                }

                if (targetFrame && (targetFrame != rootFrame))
                {
                        TempODPart targetPart = targetFrame->AcquirePart(ev); // -- TÇ tempobj'd
                        targetPart->AdjustMenus(ev,targetFrame);
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope ODMenuBar*  SOMLINK ODWindowStateCreateMenuBar(ODWindowState *somSelf, Environment *ev,
                ODPlatformMenuBar menuBar)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCreateMenuBar");

    ODMenuBar* mb = kODNULL;  ODVolatile(mb);

        SOM_TRY

            mb = new ODMenuBar;
            THROW_IF_NULL(mb);  // "new" does not THROW for SOM objects
            mb->InitMenuBar(ev, _fSession, menuBar);

        SOM_CATCH_ALL

        ODDeleteObject(mb);

        SOM_ENDTRY

        return mb;
}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
SOM_Scope ODPopup*  SOMLINK ODWindowStateCreatePopupMenu(ODWindowState *somSelf, Environment *ev,
                ODPlatformMenuBar popupMenu)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCreatePopupMenu");

    ODPopup* mb = kODNULL;  ODVolatile(mb);

        SOM_TRY

            mb = new ODPopup;
            THROW_IF_NULL(mb);  // "new" does not THROW for SOM objects
            mb->InitPopup(ev, _fSession, popupMenu);

        SOM_CATCH_ALL

        ODDeleteObject(mb);

        SOM_ENDTRY

        return mb;
}
#endif // PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

#ifdef _PLATFORM_WIN32_
SOM_Scope ODMenuBar*  SOMLINK ODWindowStateCreateMenuBarEx(ODWindowState *somSelf, Environment *ev,
                ODPlatformMenuBar menuBar, ODUShort subMenuIDcount, ODMenuIDInfo *subMenuIDinfo)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCreateMenuBarEx");

    ODMenuBar* mb = kODNULL;  ODVolatile(mb);

        SOM_TRY

            mb = new ODMenuBar;
            THROW_IF_NULL(mb);  // "new" does not THROW for SOM objects
            mb->InitMenuBarEx(ev, _fSession, menuBar, subMenuIDcount, subMenuIDinfo);

        SOM_CATCH_ALL

        ODDeleteObject(mb);

        SOM_ENDTRY

        return mb;
}
#endif // PLATFORM_WIN32

#ifdef _PLATFORM_WIN32_
SOM_Scope ODPopup*  SOMLINK ODWindowStateCreatePopupMenuEx(ODWindowState *somSelf, Environment *ev,
                ODPlatformMenuBar popupMenu, ODUShort subMenuIDcount, ODMenuIDInfo *subMenuIDinfo)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCreatePopupMenuEx");

    ODPopup* mb = kODNULL;  ODVolatile(mb);

        SOM_TRY

            mb = new ODPopup;
            THROW_IF_NULL(mb);  // "new" does not THROW for SOM objects
            mb->InitPopupEx(ev, _fSession, popupMenu, subMenuIDcount, subMenuIDinfo);

        SOM_CATCH_ALL

        ODDeleteObject(mb);

        SOM_ENDTRY

        return mb;
}
#endif // PLATFORM_WIN32

SOM_Scope ODCanvas*  SOMLINK ODWindowStateCreateCanvas(ODWindowState *somSelf, Environment *ev,
                ODGraphicsSystem graphicsSystem,
#ifdef _PLATFORM_MACINTOSH_
                ODPlatformCanvas platformCanvas,
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined (_PLATFORM_UNIX_)
        ODPlatformCanvas* platformCanvas,
#endif
                ODBoolean isDynamic,
                ODBoolean isOffscreen)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","CreateCanvas");

    ODCanvas* canvas = kODNULL;  ODVolatile(canvas);

        SOM_TRY

            canvas = new ODCanvas;
            THROW_IF_NULL(canvas);      // "new" does not THROW for SOM objects
            canvas->InitCanvas(ev, graphicsSystem,platformCanvas,isDynamic,isOffscreen);

        SOM_CATCH_ALL

        ODDeleteObject(canvas);

        SOM_ENDTRY

        return canvas;
}

SOM_Scope ODFacet*  SOMLINK ODWindowStateCreateFacet(ODWindowState *somSelf, Environment *ev,
                ODFrame* frame,
                ODShape* clipShape,
                ODTransform* externalTransform,
                ODCanvas* canvas,
                ODCanvas* biasCanvas)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","CreateFacet");

    ODFacet* facet = kODNULL;  ODVolatile(facet);

        SOM_TRY

            facet = new ODFacet;
            THROW_IF_NULL(facet);       // "new" does not THROW for SOM objects
            facet->InitFacet(ev, frame, clipShape, externalTransform, canvas, biasCanvas);

        SOM_CATCH_ALL

        ODDeleteObject(facet);

        SOM_ENDTRY

    return facet;
}

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
// adding support for current popup menu prompt line text link list  135340
SOM_Scope void  SOMLINK ODWindowStateSetCurrentPopUpMenu(ODWindowState *somSelf, Environment *ev,
    ODPopup* thePopup)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSetCurrentPopUpMenu");

    SOM_TRY

        // If the current base popup is different than the one passed in
        if (!ODObjectsAreEqual(ev, _fCurrentPopupMenu, thePopup))
        {
            // Release the currently held popup
            ODReleaseObject(ev, _fCurrentPopupMenu);

            // If the new popup is non-null
            if (thePopup)
            {
                // Acquire the new popup
                thePopup->Acquire(ev);
            }

            // Save away the pointer to the new popup
            _fCurrentPopupMenu = thePopup;
        }

    SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODWindowStateSetLongName(ODWindowState *somSelf, Environment *ev,
                                                 string longname)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSetLongName");

    // If there is currently a long name associated with the window state
    if (_fDocLongName)
    {
        // Delete the current storage for long name
        delete _fDocLongName;
    }

    // Allocate storage for the passed in long name */
    _fDocLongName = new char[strlen(longname)+1];
    THROW_IF_NULL(_fDocLongName);

    // Copy over the input long name
    strcpy(_fDocLongName, longname);

    //later on we will fix other things..like setting the switch
    //list when this is called after initialization.
    //for now..we will just set the string, so it can be gotten
    //by SetDefaultWindowTitles for setting switch list, etc.
}

SOM_Scope string  SOMLINK ODWindowStateGetLongName(ODWindowState *somSelf, Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetLongName");

    // Return the document long name
    return _fDocLongName;
}

SOM_Scope ODPlatformWindow  SOMLINK ODWindowStateCreatePlatformWindow
                                          (ODWindowState *somSelf,
                                           Environment *ev,
                                           ODBoolean isFloating)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateCreatePlatformWindow");

#ifdef _PLATFORM_OS2_
    // Set the create flags
    unsigned long CreateFlags =  FCF_STANDARD &
                                ~FCF_ACCELTABLE &
                                ~FCF_SHELLPOSITION;

    // If this is a floating window
    if (isFloating)
    {
        // Turn off the window's menu creation flag
        CreateFlags &= ~FCF_MENU;
    }

    // Create a standard window.  If we want to later add a status line as
    // in the OS/2 code, we'll have to create the frame window and client
    // windows separately as in OS/2's rlshell.cpp.  We'll also have to have
    // two window procs and two registered window classes.  But, for now,
    // let's get a window showing without the fluff
    HWND clientHWND;
    ODPlatformWindow platformWindow =
                     WinCreateStdWindow(HWND_DESKTOP,
                                        0,
                                        &CreateFlags,
                                        kODDocumentWindowClass,
                                        "OpenDoc",
                                        0,
                                        _fSession->GetWindowSystemData(ev)
                                                 ->resInstance,
                                        ID_BASEMENUBAR,
                                        &clientHWND);

    // If the window was created
    if (platformWindow)
    {
        // Get the system's recommended window position and size
        SWP swp;
        WinQueryTaskSizePos(WinQueryAnchorBlock(HWND_DESKTOP),
                            0, &swp);

        // Set the window position and size from the recommended values
        WinSetWindowPos(platformWindow, HWND_TOP,
                        swp.x, swp.y, swp.cx, swp.cy,
                        SWP_SIZE | SWP_MOVE);
    }

    // Return the window handle to the caller
    return platformWindow;
#endif


#ifdef _PLATFORM_WIN32_

    ODPlatformWindow  platformWindow;
    ODULong            ulFlags = 0;

    if (_fSession->GetOleManager(ev)->IsOleServer()) {

        DocumentManager  *docMgr = NULL;
        OleWrapper       *wrapper = NULL;

        docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);
        if (docMgr) {
            wrapper = docMgr->GetOleWrapper(ev);
            if (wrapper) {
                int XScreen = GetSystemMetrics(SM_CXSCREEN);
                int YScreen = GetSystemMetrics(SM_CYSCREEN);

                if (!wrapper->fRootDocumentWindow) {    /* Root Document Window creation */

                    // Current, NT3.51 has a problem converting WS_OVERLAPPED windows
                    // to the WS_CHILD style (and back again). This shows up especially
                    // when processing the WM_NCPAINT message. Current OLE inproc server
                    // design dictates that we create a child window of a desktop window
                    // (WS_OVERLAPPED) and reparent that child window to the OLE container.
                    // This ensures that the window being reparented within/without the
                    // container remains a WS_CHILD window at all times.
                    //
                    // Thus, if the wrapper does not have a root document window, then
                    // this is the creation of that root (part) window. Create the
                    // (parent) desktop window (for WM_NCPAINT problem) and create as a
                    // child of that window the platformWindow that is returned to OpenDoc.
                    //
                    // Note that this fix may cause other problems walking the desktop's
                    // Z-order.  Problems are not yet known.

                    ODPlatformWindow  parentWindow =
                              CreateWindowEx(0L,
                                             kODDocumentWindowClass,
                                             "OpenDoc",
                                             WS_OVERLAPPEDWINDOW,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             XScreen / 2,
                                             YScreen * 2 / 3,
                                             _fSession->GetWindowSystemData(ev)->parentWindow,
                                             _fSession->GetWindowSystemData(ev)->menu,
                                             _fSession->GetWindowSystemData(ev)->instance,
                                             NULL);

                    platformWindow = CreateWindowEx(0L,
                                                    kODDocumentWindowClass,
                                                    "OpenDoc",
                                                    WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                                    CW_USEDEFAULT,
                                                    CW_USEDEFAULT,
                                                    XScreen / 2,
                                                    YScreen * 2 / 3,
                                                    parentWindow,
                                                    NULL,
                                                    _fSession->GetWindowSystemData(ev)->instance,
                                                    NULL);
                }
                else {
                    // The wrapper already has a root document window created, so we are
                    // being called as part of an OpenDoc operation (i.e. Open Selection)
                    // to create a transient window. Current OLE inproc server design
                    // dictates that we create these windows as children of the container's
                    // frame window (for an inplace-active part) or as children of the server's
                    // window (for non-in place active part). In either case, these transient
                    // windows will not have an associated menubar (WS_CHILD) and will be
                    // graphically confined to the boundaries of the parent's window.

                    ulFlags = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                              WS_THICKFRAME | WS_BORDER | WS_SYSMENU | WS_DLGFRAME;


                    platformWindow = CreateWindowEx(0L,
                                                    kODDocumentWindowClass,
                                                    "OpenDoc",
                                                    ulFlags,
                                                    CW_USEDEFAULT,
                                                    CW_USEDEFAULT,
                                                    (wrapper->fObjRect.right - wrapper->fObjRect.left) / 2,
                                                    (wrapper->fObjRect.bottom - wrapper->fObjRect.top) * 2 / 3,
                                                    wrapper->fRootDocumentWindow,
                                                    NULL,
                                                    _fSession->GetWindowSystemData(ev)->instance,
                                                    NULL);
                }
            }   /* if (wrapper) */
        }   /* if (docMgr) */
    }
    else {   /* Regular OpenDoc */

        HMENU menu = NULL;

        // If the window is to be floating
        if (isFloating)
        {
            // Set the window style to be a popup window with a system menu
            ulFlags = WS_POPUPWINDOW | WS_CAPTION |
                      WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        }
        else
        {
            // Set the window style as a standard window
            ulFlags = WS_OVERLAPPEDWINDOW;

            // Load a copy of the menu so DestroyWindow doesn't destroy the original
            menu = LoadMenu(_fSession->GetWindowSystemData(ev)->resInstance,
                            MAKEINTRESOURCE(ID_BASEMENUBAR));
        }

        // Get screen size
        int XScreen = GetSystemMetrics(SM_CXSCREEN);
        int YScreen = GetSystemMetrics(SM_CYSCREEN);

        // Create the window and return the window handle
        platformWindow = CreateWindowEx(0L,
                                        kODDocumentWindowClass,
                                        "OpenDoc",
                                        ulFlags,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        XScreen / 2,
                                        YScreen * 2 / 3,
                                        _fSession->GetWindowSystemData(ev)->parentWindow,
                                        menu,
                                        _fSession->GetWindowSystemData(ev)->instance,
                                        NULL);
    }

    return platformWindow;

#endif


#ifdef _PLATFORM_UNIX_
    // AIXtodo - floating vs non-floating differences?  I think we
    // need to continue to use the TopLevelShell for floating windows
    // as well so they can continue to iconify separately from other
    // windows

    // AIXtodo - icon - Lance is working on for me

    // Get dimensions of screen
    Dimension height = HeightOfScreen(_fScreen);
    height = height*2/3;
    Dimension width = WidthOfScreen(_fScreen);
    width = width/2;

    // Create toplevelshell widget
    Widget topLevelShell = XtVaAppCreateShell("OpenDoc",
                                              "OpenDoc",
                                              topLevelShellWidgetClass,
                                              _fDisplay,
                                              NULL);

    // right here is where the callback for the Close message
    // from the Window Manager needs to be set to use the function
    // passed in from the DocShell in the WindowSystemData.

    // Set widget size.  Also ensure that widget does not get automatically
    // mapped when it is realized or destroyed when the system menu's close
    // is used.
    XtVaSetValues(topLevelShell,
                  XmNwidth, width,
                  XmNheight, height,
                  XmNtitle, "OpenDoc",
                  XmNiconName, "OpenDoc",
                  XmNmappedWhenManaged, FALSE,
                  XmNdeleteResponse, XmDO_NOTHING,
                  NULL);

    // right here is where the callback for the Close message
    // from the Window Manager needs to be set to use the function
    // passed in from the DocShell in the WindowSystemData.
    Atom XaWmDestroyWindow = XmInternAtom(_fDisplay,
                                          "WM_DELETE_WINDOW",
                                          False);
    XmAddWMProtocols(topLevelShell,
                     &XaWmDestroyWindow,
                     1);
    XmAddWMProtocolCallback(topLevelShell,
                            XaWmDestroyWindow,
                            _fSession->GetWindowSystemData(ev)->closeCallBack,
                            (XtPointer) NULL);

    // right here is where the callback for the session close message
    // from the Window Manager needs to be set to use the function
    // passed in from the DocShell in the WindowSystemData.
    Atom XaWmSaveYourself = XmInternAtom(_fDisplay,
                                          "WM_SAVE_YOURSELF",
                                          False);
    XmAddWMProtocols(topLevelShell,
                     &XaWmSaveYourself,
                     1);
    XmAddWMProtocolCallback(topLevelShell,
                     XaWmSaveYourself,
                     _fSession->GetWindowSystemData(ev)->saveYourselfCallBack,
                     (XtPointer) NULL);

    // Register MapNotify event handler - this is necessary in order to
    // get XSetInputFocus to work in all cases.
    XtAddEventHandler(topLevelShell,
                      StructureNotifyMask,
                      False,
                      MapNotifyEventHandler,
                      NULL);

    // Create mainwindow widget
    Widget mainwindow = XtVaCreateManagedWidget("mainWindow",
                                                xmMainWindowWidgetClass,
                                                topLevelShell,
                                                NULL);

    // If not floating window
    Widget menubar;
    if (isFloating)
    {
        // There is no menubar
        menubar = NULL;
    }
    else
    {
        // Create a dummy MenuBar
        XmString doc = XmStringCreateSimple("Document");
        menubar = XmVaCreateSimpleMenuBar(mainwindow,
                                          "menubar",
                                          XmVaCASCADEBUTTON, doc, 'D',
                                          NULL);
        XmStringFree(doc);

        // add a dummy close button
        XmString closeapp = XmStringCreateSimple("Close");
        Widget menu = XmVaCreateSimplePulldownMenu(menubar, "file_menu", 0,
                                                   (XtCallbackProc)exit,
                                                   XmVaPUSHBUTTON, closeapp, 'C', NULL, NULL,
                                                   NULL);
        XmStringFree(closeapp);

        // Manage the menubar via its parent
        XtManageChild(menubar);
    }

    // create a MainWindow workarea Widget - ensure the margin sizes are 0
    Widget drawing_a = XtVaCreateManagedWidget("drawingarea",
                                               xmDrawingAreaWidgetClass,
                                               mainwindow,
                                               XmNmarginWidth, 0,
                                               XmNmarginHeight, 0,
                                               NULL);

    // set the MainWindow areas.
    XmMainWindowSetAreas(mainwindow,
                         menubar,
                         NULL,
                         NULL,
                         NULL,
                         drawing_a);

    // Realize the widgets
    XtRealizeWidget(topLevelShell);

    XSync(_fDisplay, False);

    // Return the window corresponding to the topLevelShell widget
    return XtWindow(topLevelShell);
#endif
}

#endif

#ifdef _PLATFORM_UNIX_
SOM_Scope Display* SOMLINK ODWindowStateGetDisplay(ODWindowState *somSelf,
                                                  Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetDisplay");

    return _fDisplay;
}

SOM_Scope Screen* SOMLINK ODWindowStateGetScreen(ODWindowState *somSelf,
                                                Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetScreen");

    return _fScreen;
}

SOM_Scope XtAppContext SOMLINK ODWindowStateGetAppContext(ODWindowState *somSelf,
                                                          Environment *ev)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateGetAppContext");

    return _fAppContext;
}

SOM_Scope void SOMLINK ODWindowStateResetRootChildren(ODWindowState *somSelf,
                                                      Environment *ev,
                                                      ODBoolean Entry)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateResetRootChildren");

    ODWindowIterator* iter = kODNULL; ODVolatile(iter);

    SOM_TRY

        // If entering
        if (Entry)
        {
            // If not yet reset
            if (_fResetRootEntryCount == 0)
            {
                XSync(_fDisplay, False);

                // Create an ODWindowIterator
                iter = somSelf->CreateWindowIterator(ev);

                // For all the windows in the list
                for (ODWindow* window = iter->First(ev);
                     iter->IsNotComplete(ev);
                     window = iter->Next(ev))
                {
                    // Get the platform window
                    Window topwin = window->GetPlatformWindow(ev);

                    // Follow the parent window chain until we get to the root

                    // Loop for toplevel window until the parent
                    // is the root window.  The last topwin
                    // will be the one we want
                    Window parent = topwin, root;
                    do
                    {
                        // Assign topwin to the previous parent
                        topwin = parent;

                        Window *children;
                        unsigned int num_children;
                        Status rc;

                        // Get the parent of the current parent window
                        rc = XQueryTree(_fDisplay,
                                        topwin,
                                        &root,
                                        &parent,
                                        &children,
                                        &num_children);
                        XFree(children);
                    }
                    while (parent != root);

                    // Save away the root child window in the window
                    window->SetRootChildWindow(ev, topwin);
                }

                // Delete the iterator
                ODDeleteObject(iter);
            }

            _fResetRootEntryCount++;
        }
        else
        {
            _fResetRootEntryCount--;
        }

    SOM_CATCH_ALL

        ODDeleteObject(iter);

    SOM_ENDTRY

}

SOM_Scope ODBoolean  SOMLINK ODWindowStateIsODWindowRootChild(ODWindowState *somSelf,
                                                              Environment *ev,
                                                              ODPlatformWindow aWindow)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateIsODWindowRootChild");

        ODWindowIterator* iter = kODNULL; ODVolatile(iter);
        ODBoolean isODWindow = kODFalse;

        SOM_TRY

                iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                window = iter->Next(ev))
                {
                        if (window && (aWindow == window->GetRootChildWindow(ev)))
                        {
                                isODWindow = kODTrue;
                                break;
                        }
                }
                ODDeleteObject(iter);


        SOM_CATCH_ALL

                ODDeleteObject(iter);

        SOM_ENDTRY

        return isODWindow;
}

SOM_Scope ODWindow*  SOMLINK ODWindowStateAcquireODWindowRootChild(ODWindowState *somSelf,
                                                                   Environment *ev,
                                                                   ODPlatformWindow aWindow)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateAcquireODWindowRootChild");

        ODWindowIterator* iter = kODNULL; ODVolatile(iter);
        ODWindow* odWindow = kODNULL;

        SOM_TRY

                iter = somSelf->CreateWindowIterator(ev);

                for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
                                window = iter->Next(ev))
                {
                        if (window && (aWindow == window->GetRootChildWindow(ev)))
                        {
                                odWindow = window;
                                break;
                        }
                }
                ODDeleteObject(iter);

                if (odWindow)
                        odWindow->Acquire(ev);

        SOM_CATCH_ALL

                ODDeleteObject(iter);
                odWindow = kODNULL;

        SOM_ENDTRY

        return odWindow;
}

SOM_Scope void  SOMLINK ODWindowStateSaveActiveWindow(ODWindowState *somSelf, Environment *ev,
                ODWindow* window)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);
    ODWindowStateMethodDebug("ODWindowState","ODWindowStateSaveActiveWindow");

        SOM_TRY

                // If not floating window
                if (!window->IsFloating(ev))
                {
                    // Save this as the currently active window
                    _fActiveWindow = window;
                }

        SOM_CATCH_ALL
        SOM_ENDTRY

} // SaveActiveWindow

extern "C" void MapNotifyEventHandler(Widget widget,
                                      XtPointer client_data,
                                      XEvent *event,
                                      Boolean *cont)
{
    // If MapNotify event
    if (event->xany.type == MapNotify)
    {
        Atom type;
        int format;
        unsigned long items;
        unsigned long bytes_after;
        unsigned char *data;

        // If the window has my property and the type is correct
        if ((XGetWindowProperty(event->xany.display,
                                event->xany.window,
                                MyMapNotifyProperty,
                                0,
                                0,
                                True,
                                XA_STRING,
                                &type,
                                &format,
                                &items,
                                &bytes_after,
                                &data) == Success) &&
            (type == XA_STRING))
        {
            // We were unable to set input focus on the window earlier
            // since the window was not yet viewable.  Now it is.

            // Invoke XSetInputFocus on the window
            XSetInputFocus(event->xany.display,
                           event->xany.window,
                           RevertToPointerRoot,
                           CurrentTime);
        }
    }
}
#endif

