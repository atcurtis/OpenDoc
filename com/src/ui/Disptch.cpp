/* @(#)Z 1.25 com/src/ui/Disptch.cpp, odui, od96os2, odos29646d 96/11/15 15:28:02 (96/10/29 09:33:36) */
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
/*  133968  8/23/95 pfe  DR3 port of Apple code                     */
/*  129403  7/28/95 aml  Added WM_HSCROLL and WM_VSCROLL to the     */
/*                       standard dispatch event table              */
/*  125850  7/18/95 aml  Changed kODErrDispatchModuleAlreadyExists  */
/*                       to kODErrExistingDispatchModule and        */
/*                       kODErrDispatchModuleNotInitialized to      */
/*                       kODErrInvalidDispatchModule, as per class  */
/*                       reference                                  */
/*  107613  5/25/95 aml  Throw if adding an uninitialized dispatch  */
/*                       module (AddDispatchModule & AddMonitor)    */
/*  107617  5/25/95 aml  Throw if adding an existing event/dispatch */
/*                       module (AddDispatchModule )                */
/*  107819  5/25/95 aml  Added RemoveDispatchModuleEntries method   */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*  04/11/95  BLG   Added support for WM_MENUSELECT for status line */
/*                  display                                         */
/*  04/07/95  BLG   added support for Opendoc message OD_HELP       */
/*                                                                  */
/********************************************************************/
/*
        File:           Disptch.cpp

        Contains:       Implementation of class ODDispatcher

        Owned by:       Richard Rodseth

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <50>    10/17/95        jpa             1292659: Ignore exceptions sending null
                                                                        events to parts
                <49>    10/10/95        DM              1277216: GM:API: Dont return ODPoint from
                                                                        GetWindowPoint()
                <48>     10/3/95        TJ              Changes done by RefBall Team
                <47>      9/6/95        RR              #1282075 ADded ODVolatile
                <46>     8/29/95        RR              # 1276401 Respect Mouse Focus
                <45>     8/26/95        T‚              1274606 FB2: Patching Remarks
                <44>     8/23/95        NP              1264142: Don't Release null object.
                <43>     8/15/95        RR              #1222870 Moved NULL handling to end of
                                                                        method to allow patching/monitors
                <42>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <41>     6/29/95        NP              1263893: Fix TSM support.
                <40>     6/25/95        T‚              1242642 BB: Turn on ODDebug warning if
                                                                        refcount is wrong in
                                                                        ODRefCntObjectsomUninit.
                <39>     6/23/95        CC              1256943: MouseRegionIsInvalid: Replaced
                                                                        StillDown with Button.
                <38>     6/22/95        NP              1260250: Set up correctly for call to
                                                                        CallEventHandler.
                <37>     6/20/95        JP              1251250: ChangedReleaseExtension into
                                                                        Release
                <36>     6/15/95        RR              #1254828/1255536 Added Redispatch
                <35>      6/8/95        RR              #1257260 Collapse base classes. #1214898
                                                                        StdTypes.r -> ODTypes.r
                <34>     5/31/95        RR              #1251403 Release after AcquirePart
                <33>     5/26/95        RR              #1251403: Multithreading naming support
                <32>     5/25/95        jpa             Fixed usage of ODDebug. [1253321]
                <31>     5/21/95        NP              1251212: Wrap procs with UPPs.
                <30>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
                                                                        refcount
                <29>     5/10/95        RR              #1239139 MouseEnter etc.  over active
                                                                                                                                                border go to container. Don't send
                                                                        until mouse up.
                <28>     5/10/95        RR               #1239139 MouseEnter etc.  over active
                                                                        border go to container. Don't send until mouse up.
                <27>     4/27/95        eeh             1230798: call part::ReleaseExtension
                <26>      4/7/95        RR              #1216618 Added SOM_TRY etc.
                <25>      4/4/95        RR              # 1211085 Removed fThreadsAvailable,
                                                                        #1220104 Use ODObjectsAreEqual, #1228161
                                                                        RegisterIdle does not update frequency if
                                                                        called twice
                <24>     3/20/95        jpa             Cooperative thread support [1167943]. Moved
                                                                        bg click handler installation to private
                                                                        impl file [1220704].
                <23>     3/10/95        RR              # 1187137 Set Cursor to arrow in
                                                                        InvalidateFacetUnderMouse
                <22>     2/24/95        jpa             Install BG-mouse-down handler [1220704].
                                                                        Use ODNewRgn. [1220810]
                <21>     2/20/95        T‚              #1221861 BB: Incomplete AOCE support needs
                                                                        to be removed from OpenDoc
                <20>     1/31/95        RR              # 1206909 Pass ev to
                                                                        RemoveUnregisteredIdlers
                <19>     1/25/95        jpa             Include StdExts.xh [1206907]
                <18>     1/13/95        RR              Replace calls to MouseEnter etc with calls
                                                                        to HandleEvent
                <17>     1/12/95        jpa             Don't use obsolete Toolbox names [1211211].
                                                                        Change qd --> ODQDGlobals [1210936]
                <16>      1/9/95        NP              1194880: SemtIntf name changes.
                <15>    11/28/94        RR              Prepare for thread support
                <14>     9/30/94        RR              #1167950 Allow unregistering while idling.
                                                                        Call RemoveUnregisteredIdlers in
                                                                        HandleNULLEvent
                <13>     9/29/94        RA              1189812: Mods for 68K build.
                <12>     9/20/94        NP              1187682: Initial TSM support.
                <11>     9/20/94        RR              #1154046 Moved ref counting into IdleList
                                                                        methods so that an unregistered part is not
                                                                        released. #1156594 Call
                                                                        WindowState->HandleAOCEMailerEvent instead
                                                                        of calling SMPMailerEvent directly.
                <10>      9/2/94        eeh             #?: added SMPMailerEvent call to enable use
                                                                        of mailer until Richard and I agree on a
                                                                        cleaner fix to a bug in his hamper.
                 <9>     8/31/94        T‚              #1183129, #1183116, #1183119, #1183111:
                                                                        Lots of ErrorCode cleanup.
                 <8>     8/15/94        JBS             1181156: UI API Cleanup
                 <7>     8/10/94        JBS             1179919: coordinate system bias changes
                 <6>      8/4/94        eeh             bug 1177981: add
                                                                        ODDispatcherGetDispatchModule
                 <5>     7/27/94        eeh             Changes necessitated by WinStat.idl changes
                 <4>     7/26/94        eeh             ODQDPoint->Point
                 <3>     6/30/94        CG              Moved InitStandardDispatchModule to
                                                                        InitDispatch from InitSession.
                 <2>     9/27/94        RR              Call InitBaseDispatcher
                 <1>     9/26/94        RR              first checked in

        To Do:
                * Get Gestalt bit for gestaltHasBGMouseDown.
                * Call SetBgndMouseHandler via PrivateInterfaceLib if it's exported therefrom.
                * Check whether bg mousedown handler calls are exported from PrivateInterfaceLib
                        and use those if they are (for Copland compatibility)

        In Progress:

*/


/*
 *  This file was generated by the SOM Compiler.
 *  Generated using:
 *      SOM Emitter emitxtm.dll: somc/smmain.c
 */

// ------------------------
#ifdef _PLATFORM_OS2_
#include <ODOs2.h>
#endif // _PLATFORM_OS2_
// ------------------------

#define ODDispatcher_Class_Source
#define VARIABLE_MACROS
#include <Disptch.xih>

// Platform-independent include files

#ifndef SOM_ODDispatchModule_xh
#include "DispMod.xh"
#endif

#ifndef SOM_ODWindowState_xh
#include "WinStat.xh"
#endif

#ifndef SOM_ODWindow_xh
#include "Window.xh"
#endif

#ifndef SOM_ODSession_xh
#include "ODSessn.xh"
#endif

#ifndef _DISPTABL_
#include "DispTabl.h"
#endif

#ifndef SOM_ODFacet_xh
#include "Facet.xh"
#endif

#ifndef SOM_ODFrame_xh
#include "Frame.xh"
#endif

#ifndef SOM_ODPart_xh
#include "Part.xh"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_ODStandardDispatchModule_xh
#include <StdDisp.xh>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef SOM_Module_OpenDoc_StandardExtensions_defined
#include <StdExts.xh>
#endif

#ifndef _DSPUTILM_
#include "DspUtilM.h"
#endif

#ifndef _ODUtils_
#include <ODUtils.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
// -------------------------------------
// Mac-only include files & declarations
// -------------------------------------

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _IDLELIST_
#include "IdleList.h"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef SOM_ODSemanticInterface_xh
#include <SemtIntB.xh>
#endif

#ifndef SOM_ODAppleEvent_xh
#include <ODAplEvt.xh>
#endif

#ifndef _BARRAY_
#include "BArray.h"
#endif

#ifndef _ODDESUTL_
#include "ODDesUtl.h"
#endif

#ifndef _BGCLICKM_
#include "BGClickM.h"
#endif

#ifndef _ODTHREDM_
#include "ODThredM.h"
#endif

#ifndef __TEXTSERVICES_
#include "TextServices.h"
#endif

#ifndef __GESTALTEQU__
#include <GestaltEqu.h>
#endif

#define ODLogEvents 0

#pragma segment ODDispatcher

/* Define event types handled by Standard Mac Dispatch Module */

ODEventType StdMacEventTypes[] = {
        mouseDown,
        mouseUp,
        keyDown,
        keyUp,
        autoKey,
        updateEvt,
        diskEvt,
        activateEvt,
        osEvt,
        kHighLevelEvent,
        kODEvtMenu,
        kODEvtBGMouseDown,
        kODEvtMouseDownBorder,
        kODEvtMouseDownEmbedded,
        kODEvtBGMouseDownEmbedded,
        kODEvtMouseUpEmbedded
};

ODULong NumStdMacEventTypes = sizeof(StdMacEventTypes) / sizeof(ODEventType);

#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_OS2_ //CED
// --------------------------------------
// OS/2-only include files & declarations
// --------------------------------------

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ERRORDEF_
#include "ErrorDef.xh"
#endif

#ifndef _DSPLITEM_
#include "dsplitem.h"
#endif

/* Define event types handled by Standard OS/2 Dispatch Module */

ODEventType StdOS2EventTypes[] = {
      WM_BUTTON1DOWN
     ,WM_BUTTON2DOWN
     ,WM_BUTTON3DOWN
     ,WM_BUTTON1UP
     ,WM_BUTTON2UP
     ,WM_BUTTON3UP
     ,WM_MOUSEMOVE
     ,WM_CHORD
     ,WM_BUTTON1MOTIONSTART
     ,WM_BUTTON1MOTIONEND
     ,WM_BUTTON2MOTIONSTART
     ,WM_BUTTON2MOTIONEND
     ,WM_BUTTON3MOTIONSTART
     ,WM_BUTTON3MOTIONEND
     ,WM_CONTEXTHELP
     ,WM_BUTTON1CLICK
     ,WM_BUTTON2CLICK
     ,WM_BUTTON3CLICK
     ,WM_BUTTON1DBLCLK
     ,WM_BUTTON2DBLCLK
     ,WM_BUTTON3DBLCLK
     ,WM_BEGINDRAG
     ,WM_ENDDRAG
     ,WM_SINGLESELECT
     ,WM_CONTEXTMENU
     ,WM_BEGINSELECT
     ,WM_ENDSELECT
     ,WM_OPEN
     ,WM_TEXTEDIT
     ,WM_CHAR
     ,kODEvtKeyUp
     ,kODEvtKeyDown
     ,WM_PAINT
     ,WM_ACTIVATE
     ,WM_COMMAND
     ,WM_INITMENU
     ,WM_TIMER
     ,WM_SIZE
     ,WM_MOVE
     ,WM_RENDERFMT
     ,DM_DRAGOVER
     ,DM_DRAGLEAVE
     ,DM_DROP
     ,DM_RENDER
     ,DM_RENDERCOMPLETE
     ,DM_ENDCONVERSATION
     ,WM_MENUSELECT
     ,WM_DRAWITEM
     ,WM_MEASUREITEM
     ,OD_HELP
     ,WM_QUERYCONVERTPOS  // [124535]
     ,WM_REALIZEPALETTE   // [129147]
     ,WM_PRESPARAMCHANGED // [129147]
     ,WM_HSCROLL          // [129403]
     ,WM_VSCROLL          // [129403]
};

ODULong NumStdOS2EventTypes = sizeof(StdOS2EventTypes) / sizeof(ODEventType);

#endif // _PLATFORM_OS2_ //CED

#ifdef _PLATFORM_WIN32_
// ---------------------------------------
// WIN32-only include files & declarations
// ---------------------------------------

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _STDDISPWIN_
#include <StdDispW.xh>
#endif

/* Define event types handled by Standard Win32 Dispatch Module */

ODEventType StdWinEventTypes[] = {
        WM_RBUTTONDOWN,
        WM_MBUTTONDOWN,
        WM_LBUTTONDOWN,
        WM_RBUTTONUP,
        WM_MBUTTONUP,
        WM_LBUTTONUP,
        WM_RBUTTONDBLCLK,
        WM_MBUTTONDBLCLK,
        WM_LBUTTONDBLCLK,
        WM_KEYDOWN,
        WM_KEYUP,
        WM_CHAR,
        WM_MOUSEMOVE,
        WM_VSCROLL,
        WM_HSCROLL,
        WM_PAINT,
        WM_ACTIVATE,
        WM_COMMAND,
        WM_ERASEBKGND,
        WM_NCCREATE,
        WM_SETFOCUS,
        WM_SIZE,
        WM_WINDOWPOSCHANGED,
        kODEvtMouseDownEmbedded,
        kODEvtMouseDownBorder,
        kODEvtMouseUpEmbedded,
	OD_HELP
};

ODULong NumStdWinEventTypes = sizeof(StdWinEventTypes) / sizeof(ODEventType);

#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
// ---------------------------------------
// UNIX-only include files & declarations
// ---------------------------------------

#ifndef X_H
#include "X.h"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _STDDISPX11_
#include <StdDispX.xh>
#endif

/* Define event types handled by Standard X11 Dispatch Module */

ODEventType StdX11EventTypes[] = {
        ButtonPress,
        ButtonRelease,
        KeyPress,
        KeyRelease,
        Expose,
        ConfigureNotify,
        FocusIn,
        FocusOut,
        MotionNotify,
        kODEvtMouseDownEmbedded,
        kODEvtMouseUpEmbedded,
        kODEvtMouseDownBorder,
        kODEvtMenu
};

ODULong NumStdX11EventTypes = sizeof(StdX11EventTypes) / sizeof(ODEventType);

#endif // _PLATFORM_UNIX_

//------------------------------------------------------------------------------
//      ODDispatcher
//------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
//      RegisterIdle (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherRegisterIdle(ODDispatcher *somSelf, Environment *ev,
                ODPart* part,
                ODFrame* frame,
                ODIdleFrequency frequency)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherRegisterIdle");

        SOM_TRY

                if (part)
                        _fIdleList->AddIdle(ev, part, frame, frequency);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      UnregisterIdle (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherUnregisterIdle(ODDispatcher *somSelf, Environment *ev,
                ODPart* part,
                ODFrame* frame)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherUnregisterIdle");

        SOM_TRY

                if (part)
                        _fIdleList->RemoveIdle(ev, part, frame);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      SetIdleFrequency (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherSetIdleFrequency(ODDispatcher *somSelf, Environment *ev,
                ODPart* part,
                ODFrame* frame,
                ODIdleFrequency frequency)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherSetIdleFrequency");

        SOM_TRY

                if (part)
                        _fIdleList->SetIdleFrequency(ev, part, frame, frequency);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      Yield (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherYield(ODDispatcher *somSelf, Environment *ev,
                ODFrame* frame)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherYield");

        SOM_TRY

        /*
                Will Call WaitNextEvent(), masking out all but null and update events.
                Idle events are sent to all but the part that yielded
        */
                const   ODSLong         kODSleepTicks                   =       20L;//  Number sleep ticks

                EventRecord             eventRec;
                WaitNextEvent ( updateMask, &eventRec, somSelf->GetSleepTime(ev), somSelf->GetMouseRegion(ev) ) ;
                if (eventRec.what == nullEvent)
                        somSelf->HandleNullEvent(ev, (ODEventData*) &eventRec, frame);  // Note Cast
                else
                        somSelf->Dispatch(ev, (ODEventData*) &eventRec);


        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      SetMouseRegion (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherSetMouseRegion(ODDispatcher *somSelf, Environment *ev,
                ODRgnHandle area)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherSetMouseRegion");

        if (area && _fPartMouseRegion)
        {
                CopyRgn(area, _fPartMouseRegion);
        }

}

//------------------------------------------------------------------------------
//      GetMouseRegion (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope ODRgnHandle  SOMLINK ODDispatcherGetMouseRegion(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherGetMouseRegion");

        SOM_TRY

                if (somSelf->MouseRegionIsInvalid(ev))
                        somSelf->ComputeMouseRegion(ev);


        /*

                        CGrafPort desktopPort;                                  // Used for showing cursor region
                        long ticks;                                                             // Used for showing cursor region
                        GrafPtr savedPort;                                              // Used for showing cursor region

                        // Cursor region is in global coords. Need to create desktop port
                        GetPort(&savedPort);
                        OpenCPort(&desktopPort);
                        CopyRgn(GetGrayRgn(), desktopPort.visRgn);
                        desktopPort.portRect = (*(desktopPort.visRgn))->rgnBBox;

                        PenNormal();
                        PenMode(patXor);

                        PaintRgn(_fPartMouseRegion);
                        Delay(30, &ticks);
                        PaintRgn(_fMouseRegion);

                        CloseCPort(&desktopPort);
                        SetPort(savedPort);


        */

        return _fMouseRegion;

        SOM_CATCH_ALL

                return kODNULL;

        SOM_ENDTRY

        return kODNULL;
}

//------------------------------------------------------------------------------
//      InvalidateFacetUnderMouse (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherInvalidateFacetUnderMouse(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherInvalidateFacetUnderMouse");

        SOM_TRY

                _fFacetUnderMouse = kODNULL;
                somSelf->InvalidateMouseRegion(ev);
                SetCursor(&ODQDGlobals.arrow);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      GetSleepTime (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope ODSLong  SOMLINK ODDispatcherGetSleepTime(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherGetSleepTime");

        SOM_TRY

                // Don't sleep at all if another thread is ready to run:
                if( ODAnyThreadsReady() )
                        return 0;

                const ODSLong kMaxLong = 2147483647;
                const ODTicks kMaxIdleTime = kMaxLong;


                ODTicks waitTicks = kMaxIdleTime;

                IdleListIterator iter(_fIdleList);

                for (IdleInfo* idleInfo = iter.First(); iter.IsNotComplete(); idleInfo = iter.Next())
                {
                        waitTicks = Min(waitTicks, idleInfo->NextIdle(TickCount()));
                }
                return waitTicks;

        SOM_CATCH_ALL

                return 0;

        SOM_ENDTRY

        return 0;
}

//------------------------------------------------------------------------------
//      HandleTSMEvents (Mac-only) (non-SOM)
//------------------------------------------------------------------------------
static OSErr pascal HandleTSMEvents(const AppleEvent *theAppleEvent,
                                                                                AppleEvent *reply,
                                                                                long handlerRefcon)
{
        ODSession*              session = (ODSession*)handlerRefcon;
        Environment*    ev = somGetGlobalEnvironment();
//      DescType                eventID;
//      DescType                actualType;
        Size                    maxSize = sizeof(DescType);
//      Size                    actualSize;
        OSErr                   result = noErr;
        ODSemanticInterface* semtIntf = kODNULL; ODVolatile(semtIntf);
        ODPart* keyPart = kODNULL; ODVolatile(keyPart);

        TRY
                TempODFrame keyFrame
                        = session->GetArbitrator(ev)->AcquireFocusOwner(ev,
                                                                                                session->Tokenize(ev,
                                                                                                                                        kODKeyFocus));
                        // -- T‚ tempobj'd
                if ((ODFrame*)keyFrame != kODNULL)
                {
                        keyPart = keyFrame->AcquirePart(ev);
                        if (keyPart->HasExtension(ev, kODExtSemanticInterface))
                        {
                                semtIntf = (ODSemanticInterface*)keyPart->AcquireExtension(ev,
                                                                                                                kODExtSemanticInterface);
                                // WHAT WAS I GOING TO DO WITH THIS INFO? - NP 6/21/95
//                              result = AEGetAttributePtr(theAppleEvent, keyEventIDAttr,
//                                                                                                      typeType, &actualType,
//                                                                                                      (Ptr)&eventID, maxSize,
//                                                                                                      &actualSize);
//                              if (result || actualType != typeType || actualSize != maxSize)
//                                      THROW(errAEEventNotHandled);

                                // CREATE NEW ODAPPLEEVENTS FOR THE PARAMETERS
                                ODAppleEvent*   odEvent = new ODAppleEvent;
                                THROW_IF_NULL(odEvent);
                                odEvent->InitODAppleEvent(ev);
                                AEDescToODDesc((AEDesc*)theAppleEvent, odEvent);
                                ODAppleEvent*   odReply = new ODAppleEvent;
                                THROW_IF_NULL(odReply);
                                odReply->InitODAppleEvent(ev);
                                AEDescToODDesc((AEDesc*)reply, odReply);

                                // PASS THE EVENT TO THE PART
                                TRY
                                        semtIntf->CallEventHandler(ev, keyPart, odEvent, odReply);
                                CATCH_ALL
                                        result = ErrorCode();
                                ENDTRY

                                // CLEAN UP. NOTE, WE MUST UPDATE THE REPLY DATAHANDLE SO THAT
                                //      THE APPLE EVENTS MANAGER SEES THE CHANGE (IF ANY).
                                delete odEvent;
                                if (reply->descriptorType != typeNull
                                                && reply->dataHandle != kODNULL
                                                && !result)
                                {
                                        ODByteArray data = odReply->GetRawData(ev);
                                        SetHandleSize(reply->dataHandle, data._length);
                                        THROW_IF_ERROR(MemError());
                                        ODBlockMove(data._buffer, *(reply->dataHandle),
                                                                        data._length);
                                        DisposeByteArrayStruct(data);
                                }
                                delete odReply;
                        }
                        else
                                result = errAEEventNotHandled;
                }
                else
                        result = errAEEventNotHandled;
        CATCH_ALL
                result = ErrorCode();
        ENDTRY

        if ( semtIntf != kODNULL )
                semtIntf->Release(ev);
        if ( keyPart != kODNULL )
                keyPart->Release(ev);

        return result;
}

#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_UNIX_
//------------------------------------------------------------------------------
//      SetXtDispatchMode (UNIX-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherSetXtDispatchMode(ODDispatcher *somSelf,
						      Environment *ev,
						      ODBoolean mode)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherSetXtDispatchMode");

    _fXtDispatchMode = mode;
}

//------------------------------------------------------------------------------
//      GetXtDispatchMode (UNIX-only)
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODDispatcherGetXtDispatchMode(ODDispatcher *somSelf,
						      Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherGetXtDispatchMode");

    return _fXtDispatchMode;
}
#endif // PLATFORM_UNIX

//------------------------------------------------------------------------------
//      InitDispatcher
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherInitDispatcher(ODDispatcher *somSelf, Environment *ev,
                ODSession* session)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherInitDispatcher");

    const ODBoolean     kIsSysHandler = kODTrue;

    SOM_TRY

        /* Moved from somInit. SOM itself sets fields to zero
        // _fExit = kODFalse;

        // _fDispatchModules = kODNULL;
        // _fMonitors = kODNULL;
        // _fDispatchTable = kODNULL;

        // _fSession = kODNULL;
        // _fIdleList = kODNULL;

        // _fMouseRegion = kODNULL;
        // _fPartMouseRegion = kODNULL;
        // _fFacetUnderMouse = kODNULL;
        */

        somSelf->InitObject(ev);

        _fSession = session;

        /* Create dispatch table */

        _fDispatchTable = new DispatchTable;
        THROW_IF_NULL(_fDispatchTable);

        /* Create idle list and mouse regions (Mac only) */

#ifdef _PLATFORM_MACINTOSH_
        _fIdleList = new IdleList;
        THROW_IF_NULL(_fIdleList);

        _fMouseRegion = ODNewRgn();
        _fPartMouseRegion = ODNewRgn();
#endif // _PLATFORM_MACINTOSH_

        /* Create platform-specific standard dispatch modules */

#ifdef _PLATFORM_MACINTOSH_
        ODStandardDispatchModule* stdMacModule = new ODStandardDispatchModule();
        THROW_IF_NULL(stdMacModule);
        stdMacModule->InitStandardDispatchModule(ev,session);

        for (int i = 0; i < NumStdMacEventTypes; i++)
        {
            somSelf->AddDispatchModule(ev, StdMacEventTypes[i], stdMacModule);
        }

        THROW_IF_ERROR(AEInstallEventHandler(kTextServiceClass,
        kOffset2Pos, proc, (long)_fSession, !kIsSysHandler));

        THROW_IF_ERROR(AEInstallEventHandler(kTextServiceClass,
        kShowHideInputWindow, proc, (long)_fSession, !kIsSysHandler));

        // Background mouse-down support:
        ODInstallBGMouseDownHandler(session);

        // Thread support:
        ODInitThreads();
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_OS2_
        ODStandardDispatchModule* stdOS2Module = new ODStandardDispatchModule();
        THROW_IF_NULL(stdOS2Module);
        stdOS2Module->InitStandardDispatchModule(ev,session);

        for (int i = 0; i < NumStdOS2EventTypes; i++)
        {
            somSelf->AddDispatchModule(ev, StdOS2EventTypes[i], stdOS2Module);
        }
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
        ODStdWinDispatchModule* stdWinModule = new ODStdWinDispatchModule();
        THROW_IF_NULL(stdWinModule);
        stdWinModule->InitStdWinDispatchModule(ev, session, somSelf);

        for (int i = 0; i < NumStdWinEventTypes; i++)
        {
            somSelf->AddDispatchModule(ev, StdWinEventTypes[i], stdWinModule);
        }
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
        ODStdX11DispatchModule* stdX11Module = new ODStdX11DispatchModule();
        THROW_IF_NULL(stdX11Module);
        stdX11Module->InitStdX11DispatchModule(ev, session, somSelf);

        for (int i = 0; i < NumStdX11EventTypes; i++)
        {
            somSelf->AddDispatchModule(ev, StdX11EventTypes[i], stdX11Module);
        }

        // cache a pointer to the Xt display
        WindowSystemData* winSysData = kODNULL;

        if (_fSession)
            winSysData = _fSession->GetWindowSystemData(ev);

        if (winSysData)
            _fXtDisplay = winSysData->display;

        _fXtDispatchMode = kODFalse;

#endif // _PLATFORM_UNIX_

    SOM_CATCH_ALL
    SOM_ENDTRY
}

#ifdef _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
//      InvalidateMouseRegion (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherInvalidateMouseRegion(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherInvalidateMouseRegion");

        SOM_TRY

                ASSERT(_fPartMouseRegion != kODNULL, kODErrDispatcherNotInitialized);
                SetEmptyRgn(_fPartMouseRegion);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      GetFacetUnderMouse (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope ODFacet*  SOMLINK ODDispatcherGetFacetUnderMouse(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherGetFacetUnderMouse");

        return _fFacetUnderMouse;
}

//------------------------------------------------------------------------------
//      SetFacetUnderMouse (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherSetFacetUnderMouse(ODDispatcher *somSelf, Environment *ev,
                ODFacet* facet)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherSetFacetUnderMouse");


        _fFacetUnderMouse = facet;

}

//------------------------------------------------------------------------------
//      HandleNullEvent (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODDispatcherHandleNullEvent(ODDispatcher *somSelf, Environment *ev,
                ODEventData* event,
                ODFrame* skipFrame)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherHandleNullEvent");

        ODBoolean handled = kODFalse;

        SOM_TRY

                // Give threads time to run:
                ODYield();

                IdleListIterator iter(_fIdleList);
                ODFrame* frame = kODNULL;

                for (IdleInfo* idleInfo = iter.First(); iter.IsNotComplete(); idleInfo = iter.Next())
                {
                        if ( (idleInfo->NeedsIdle(TickCount())))
                        {
                                frame = idleInfo->GetFrame();
                                if ( (frame == kODNULL) || !ODObjectsAreEqual(ev, frame,skipFrame) )
                                {
                                        TRY{
                                                idleInfo->GetPart()->HandleEvent(ev,event, frame, kODNULL, kODNULL);
                                        }CATCH_ALL{
                                                // ignore exception
                                        }ENDTRY
                                        idleInfo->SetLastIdle(TickCount());
                                }
                        }
                }
                _fIdleList->RemoveUnregisteredIdlers(ev);
                handled = kODTrue;

        SOM_CATCH_ALL
        SOM_ENDTRY

        return handled;
}

//------------------------------------------------------------------------------
//      MouseRegionIsInvalid (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODDispatcherMouseRegionIsInvalid(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherMouseRegionIsInvalid");

        ODBoolean isInvalid = kODTrue;

        if (Button())
                return kODFalse;

        SOM_TRY

                ASSERT(_fPartMouseRegion != kODNULL, kODErrDispatcherNotInitialized);
                isInvalid = EmptyRgn(_fPartMouseRegion);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return isInvalid;
}

//------------------------------------------------------------------------------
//      ComputeMouseRegion (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherComputeMouseRegion(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherComputeMouseRegion");

        SOM_TRY

                somSelf->CheckFacetUnderMouse(ev); // A Part may call SetMouseRegion
                /*
                        if no frame is under the mouse, compute an appropriate big region
                        either way, add the current mouse location?
                        And clip region to frame shape?
                */

                // Add the current mouse location to whatever region is computed by the part
                Point globalMouse;
                GetMouse(&globalMouse);
                LocalToGlobal(&globalMouse);

                SetRectRgn(_fMouseRegion, globalMouse.h, globalMouse.v, globalMouse.h + 1, globalMouse.v + 1);
                UnionRgn(_fPartMouseRegion, _fMouseRegion, _fPartMouseRegion);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      CheckFacetUnderMouse (Mac-only)
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherCheckFacetUnderMouse(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherCheckFacetUnderMouse");

        SOM_TRY

                Point globalPoint;
                ODPoint windowPoint;

                GetMouse(&globalPoint);
                LocalToGlobal(&globalPoint);

                if (!SetTSMCursor(globalPoint)) // NP: temporary. dispatcher really needs
                                                                                //      to set a 1-pixel mouse region so that
                                                                                //      SetTSMCursor can be called every time
                                                                                //      the mouse moves.
                {
                        ODWindow* odWindow = kODNULL;
                        ODFacet* foundFacet = kODNULL;

                        ODWindowState* _fWindowState = _fSession->GetWindowState(ev); // temp

                        TempODFrame mouseFocusFrame = kODNULL;
                        mouseFocusFrame = _fSession->GetArbitrator(ev)->AcquireFocusOwner(ev, _fSession->Tokenize(ev,kODMouseFocus));

                        if (mouseFocusFrame)
                        {

                                        if (_fFacetUnderMouse && ODObjectsAreEqual(ev, mouseFocusFrame, _fFacetUnderMouse->GetFrame(ev)))
                                        {
                                                odWindow = _fFacetUnderMouse->GetWindow(ev);
                                                odWindow->GetWindowPoint(ev, &globalPoint, &windowPoint);

                                                ODEventData event;
                                                SetEvent(event,kODEvtMouseWithin, globalPoint);
                                                ODEventInfo info;
                                                SetEventInfo(info, windowPoint);
                                                WindowToLocal(ev, info.where, _fFacetUnderMouse);

                                                TempODPart part = mouseFocusFrame->AcquirePart(ev);
#if ODDebug && ODLogEvents
        LogEvent(&event, &info, mouseFocusFrame, _fFacetUnderMouse);
#endif
                                                part->HandleEvent(ev, &event, mouseFocusFrame, _fFacetUnderMouse, &info);
                                        }
                                        return;
                        }

                        odWindow = _fWindowState->FindODWindow(ev, &globalPoint);

                        if (odWindow)
                        {
                                TempODWindow tempWindow = odWindow; // ensure it's released
                                odWindow->GetWindowPoint(ev, &globalPoint, &windowPoint);
                                TempODFrame activeFrame = _fSession->GetArbitrator(ev)->AcquireFocusOwner(ev, _fSession->Tokenize(ev,kODSelectionFocus));
                                ODFacet* activeFacet = activeFrame ? GetActiveFacetWithBorderUnderPoint(ev, activeFrame, odWindow, windowPoint) : kODNULL;

                                if (activeFacet)
                                        foundFacet = activeFacet->GetContainingFacet(ev);
                                else
                                {
                                        foundFacet = odWindow->GetFacetUnderPoint(ev, &windowPoint);
                                        if (foundFacet && (foundFacet->GetFrame(ev)->IsFrozen(ev) || foundFacet->IsSelected(ev)))
                                                foundFacet = foundFacet->GetContainingFacet(ev);
                                }
                        }


                        if (ODObjectsAreEqual(ev, foundFacet, _fFacetUnderMouse))
                        {
                                if (foundFacet)
                                {
                                        ODEventData event;
                                        SetEvent(event,kODEvtMouseWithin, globalPoint);
                                        ODEventInfo info;
                                        SetEventInfo(info, windowPoint);
                                        WindowToLocal(ev, info.where, _fFacetUnderMouse);

                                        ODFrame* frame = _fFacetUnderMouse->GetFrame(ev);
#if ODDebug && ODLogEvents
        LogEvent(&event, &info, frame, _fFacetUnderMouse);
#endif
                                        TempODPart part = frame->AcquirePart(ev);
                                        part->HandleEvent(ev, &event, frame, _fFacetUnderMouse, &info);
                                }
                        }
                        else
                        {
                                if (_fFacetUnderMouse)
                                {
                                        ODEventData event;
                                        SetEvent(event,kODEvtMouseLeave, globalPoint);
                                        ODEventInfo info;
                                        SetEventInfo(info, windowPoint);
                                        WindowToLocal(ev, info.where, _fFacetUnderMouse);

                                        ODFrame* frame = _fFacetUnderMouse->GetFrame(ev);
#if ODDebug && ODLogEvents
        LogEvent(&event, &info, frame, _fFacetUnderMouse);
#endif
                                        TempODPart part = frame->AcquirePart(ev);
                                        part->HandleEvent(ev, &event, frame, _fFacetUnderMouse, &info);
                                }
                                if (foundFacet)
                                {
                                        ODEventData event;
                                        SetEvent(event,kODEvtMouseEnter, globalPoint);
                                        ODEventInfo info;
                                        SetEventInfo(info, windowPoint);
                                        WindowToLocal(ev, info.where, foundFacet);

                                        ODFrame* frame = foundFacet->GetFrame(ev);
#if ODDebug && ODLogEvents
        LogEvent(&event, &info, frame, foundFacet);
#endif
                                        TempODPart part = frame->AcquirePart(ev);
                                        part->HandleEvent(ev, &event,frame,foundFacet, &info);
                                }
                                _fFacetUnderMouse = foundFacet;
                        }
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}
#endif // _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
//      somUninit
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatchersomUninit(ODDispatcher *somSelf)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatchersomUninit");

    /* free dispatch table (or OS/2 dispatcher lists) */

    ODDeleteObject(_fDispatchTable);

    /* free Mac-only instance variables  */

#ifdef _PLATFORM_MACINTOSH_
        ODDeleteObject(_fIdleList);
        ODDisposeHandle((Handle)_fMouseRegion);
        ODDisposeHandle((Handle)_fPartMouseRegion);
#endif // _PLATFORM_MACINTOSH_

        parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
//      Purge
//------------------------------------------------------------------------------
SOM_Scope ODSize  SOMLINK ODDispatcherPurge(ODDispatcher *somSelf, Environment *ev,
                ODSize size)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherPurge");

        return 0;
}

//------------------------------------------------------------------------------
//      AddDispatchModule
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherAddDispatchModule(ODDispatcher *somSelf, Environment *ev,
                ODEventType eventType,
                ODDispatchModule* dispatchModule)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherAddDispatchModule");

        SOM_TRY

        ASSERT(_fDispatchTable != kODNULL,kODErrDispatcherNotInitialized);

        if (!dispatchModule)
            THROW(kODErrIllegalNullDispatchModuleInput);

        _fDispatchTable->AddDispatchModule(eventType,dispatchModule);

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      GetDispatchModule
//------------------------------------------------------------------------------
SOM_Scope ODDispatchModule*  SOMLINK ODDispatcherGetDispatchModule(ODDispatcher *somSelf, Environment *ev,
                ODEventType eventType)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherGetDispatchModule");

    ODDispatchModule* m = kODNULL;

    SOM_TRY

        ASSERT(_fDispatchTable != kODNULL,kODErrDispatcherNotInitialized);

        m = _fDispatchTable->GetDispatchModule( eventType );

    SOM_CATCH_ALL
    SOM_ENDTRY

    return m;
}

//------------------------------------------------------------------------------
//      RemoveDispatchModule
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherRemoveDispatchModule(ODDispatcher *somSelf, Environment *ev,
                ODEventType eventType)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherRemoveDispatchModule");

    SOM_TRY

        ASSERT(_fDispatchTable != kODNULL,kODErrDispatcherNotInitialized);

        _fDispatchTable->RemoveDispatchModule(eventType);

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      AddMonitor
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherAddMonitor(ODDispatcher *somSelf, Environment *ev,
                ODEventType eventType,
                ODDispatchModule* dispatchModule)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherAddMonitor");

    SOM_TRY

        ASSERT(_fDispatchTable != kODNULL,kODErrDispatcherNotInitialized);

    if (!dispatchModule)
        THROW(kODErrIllegalNullDispatchModuleInput);

    _fDispatchTable->AddMonitor(eventType,dispatchModule);

    SOM_CATCH_ALL
    SOM_ENDTRY
}

//------------------------------------------------------------------------------
//      RemoveMonitor
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherRemoveMonitor(ODDispatcher *somSelf, Environment *ev,
                ODEventType eventType,
                ODDispatchModule* dispatchModule)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherRemoveMonitor");

    SOM_TRY

        ASSERT(_fDispatchTable != kODNULL,kODErrDispatcherNotInitialized);

        _fDispatchTable->RemoveMonitor(eventType, dispatchModule);

    SOM_CATCH_ALL
    SOM_ENDTRY
}


#ifdef _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
//      IntlTSMEvent (Mac-only) (non-SOM)
//------------------------------------------------------------------------------
static Boolean IntlTSMEvent(EventRecord *event) // NP: from TE 27.
{
        short oldFont;
        ScriptCode keyboardScript;

        // make sure we have a port and it's not the Window Manager port
        if (ODQDGlobals.thePort != nil && FrontWindow() != nil)
        {
                oldFont = ODQDGlobals.thePort->txFont;
                keyboardScript = GetScriptManagerVariable(smKeyScript);
                if (FontToScript(oldFont) != keyboardScript)
                        TextFont(GetScriptVariable(keyboardScript, smScriptAppFond));
        };
        return TSMEvent(event);
}
#endif // _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
//      Dispatch
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODDispatcherDispatch(ODDispatcher *somSelf, Environment *ev,
                ODEventData* eventData)
{
        ODBoolean handled = kODFalse;

        SOM_TRY

                ODEventInfo eventInfo;  // Note: Need some utility functions
                eventInfo.embeddedFrame = kODNULL;
                eventInfo.embeddedFacet = kODNULL;
                eventInfo.where.x = 0;
                eventInfo.where.y = 0;

        #ifdef _PLATFORM_OS2_
                eventInfo.flags = 0;
        #else
                eventInfo.propagated = kODFalse;
        #endif

        #ifdef _PLATFORM_OS2_
                eventInfo.originalType = eventData->msg;
        #endif
        #ifdef _PLATFORM_WIN32_
                eventInfo.originalType = eventData->message;
        #endif
        #ifdef _PLATFORM_UNIX_
                eventInfo.originalType = eventData->type;
        #endif

        #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
                // Save away the original type in a local variable so we can restore it
                // after the Redispatch call
                ODEventType originalType = eventInfo.originalType;
        #endif

                handled =somSelf->Redispatch(ev, eventData, &eventInfo);

        #ifdef _PLATFORM_OS2_
                // Restore the original type
                eventData->msg = originalType;
        #endif
        #ifdef _PLATFORM_WIN32_
                eventData->message = originalType;
        #endif
        #ifdef _PLATFORM_UNIX_
                eventData->type = originalType;
        #endif

        SOM_CATCH_ALL
        SOM_ENDTRY

        return handled;
}

//------------------------------------------------------------------------------
//      Redispatch
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODDispatcherRedispatch(ODDispatcher *somSelf, Environment *ev,
                ODEventData* eventData,
                ODEventInfo* eventInfo)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherDispatch");

    ODBoolean handled = kODFalse;
    ODEventType eventType;

#ifdef _PLATFORM_MACINTOSH_
    eventType = eventData->what;
#endif

#ifdef _PLATFORM_OS2_
    eventType = eventData->msg;
#endif

#ifdef _PLATFORM_WIN32_
    eventType = eventData->message;
#endif

#if defined(_PLATFORM_UNIX_)
    eventType = eventData->type;
#endif

    SOM_TRY

        ASSERT(_fDispatchTable != kODNULL,kODErrDispatcherNotInitialized);

        ODBoolean isMacNullEvent = kODFalse;

        #ifdef _PLATFORM_MACINTOSH_
        isMacNullEvent = (eventType == nullEvent);
        #endif // _PLATFORM_MACINTOSH_

        if (isMacNullEvent)
        {
        #ifdef _PLATFORM_MACINTOSH_
            somSelf->HandleNullEvent(ev,eventData, kODNULL);
        #endif // _PLATFORM_MACINTOSH_
            handled = kODTrue;
        }
        else
        {
            OrderedCollection* monitors = kODNULL;
            ODDispatchModule* module = kODNULL;

            monitors = _fDispatchTable->GetMonitors(eventType);
            if (monitors)
            {
                OrderedCollectionIterator iter(monitors);

                for (module = (ODDispatchModule*) iter.First();
                        iter.IsNotComplete();
                        module = (ODDispatchModule*) iter.Next())
                {
                    module->Dispatch(ev, eventData, eventInfo);
                }
            }

            ODBoolean isMacTSMevent = kODFalse; // we hope

        #ifdef _PLATFORM_MACINTOSH_
            isMacTSMevent = (IntlTSMEvent((EventRecord*)eventData));
        #endif // _PLATFORM_MACINTOSH_

            if (! isMacTSMevent)
            {

            #ifdef _PLATFORM_UNIX_
                // offer modal X11 mouse/kb events (for menus, popup dialogs, etc.)
		// to the Xt event dispatcher first
		// ----------------------------------------------------------------
		if ((_fXtDispatchMode)	&&
		    (eventData)		&&
		    (eventData->display == _fXtDisplay))
		{
		    switch (eventData->type)
		    {
			case MotionNotify:
			case ButtonPress:
			case ButtonRelease:
			case KeyPress:
			case KeyRelease:
				    XtDispatchEvent((XEvent*)eventData);
				    handled = kODTrue;
				    break;

			// to do: what about synthetic border/embedded mouse events?

			default:    break;
		    }
		}
            #endif // _PLATFORM_UNIX_

                // offer events to the appropriate dispatch module
		// ------------------------------------------------
                if (!handled)
		    module = _fDispatchTable->GetDispatchModule(eventType);

                if (module)
                    handled = module->Dispatch(ev, eventData, eventInfo);

            #ifdef _PLATFORM_UNIX_
                // offer leftover X11 events to the Xt dispatcher
		// ----------------------------------------------

                if ((!handled)		 &&
		    (eventData)		 &&
                    (eventData->display == _fXtDisplay))
                {
                    XtDispatchEvent((XEvent *)eventData);
                    handled = kODTrue;
                }
            #endif // _PLATFORM_UNIX_

            }
        }

        // Moved to the end so that dispatch modules can monitor or patch
        // null event handling
        if ((isMacNullEvent) && (!handled))
        {
        #ifdef _PLATFORM_MACINTOSH_
            somSelf->HandleNullEvent(ev,eventData, kODNULL);
        #endif // _PLATFORM_MACINTOSH_
            handled = kODTrue;
        }

    SOM_CATCH_ALL
    SOM_ENDTRY

    return handled;
}

//------------------------------------------------------------------------------
//      Exit
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODDispatcherExit(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherExit");

        _fExit = kODTrue;
}

//------------------------------------------------------------------------------
//      ShouldExit
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODDispatcherShouldExit(ODDispatcher *somSelf, Environment *ev)
{
    ODDispatcherData *somThis = ODDispatcherGetData(somSelf);
    ODDispatcherMethodDebug("ODDispatcher","ODDispatcherShouldExit");

        return _fExit;
}
