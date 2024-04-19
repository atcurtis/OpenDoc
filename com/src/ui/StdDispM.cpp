/* @(#)Z 1.5 com/src/ui/StdDispM.cpp, odui, od96os2, odos29646d 96/11/15 15:28:13 (96/07/16 17:32:51) */
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
#ifdef _PLATFORM_MACINTOSH_
/*
        File:           StdDispM.cpp

        Contains:       Implementation of standard dispatch module for Macintosh events

        Owned by:       Richard Rodseth

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <49>    04/19/96        brr               21285: Removed #include CmdDefs.xh
                <48>    10/10/95        DM              1277216: GM:API: Dont return ODPoint from
                                                                        GetWindowPoint()
                <47>    10/10/95        RR              1289924 Now get kODEvtMouseUpEmbedded with
                                                                        bundled embedded frames
                <46>     9/21/95        RR              #1286167 Suppress mouse up after bg
                                                                        mousedown. Renamed fInvalidClick to
                                                                        fSuppressMouseUp
                <45>     9/13/95        DM              1277216 GM:API return no ODPoints nor
                                                                        ODPolygons
                <44>     9/12/95        DM              1280020 FB1 part trans dlg mem leaks
                <43>     9/12/95        JBS             1283337 use TempOD<Object>s to make
                                                                        refcounting exception safe
                <42>     9/12/95        RR              Changed some somPrintfs
                <41>     8/29/95        DM              1221778, 1276165: [ShowLinkUpdateAlert]
                                                                        call LinkMgr SuspendResume()
                <40>     8/29/95        RR              #1267251 handle synthesized activate events
                <39>     7/26/95        DM              #1270320: Memory leak fixes.
                <38>     6/28/95        RR              #1242642. Ref counting fixes
                <37>     6/28/95        JBS             1262900 if click in icon view root frame,
                                                                        pass to shell instead of Redispatch
                <36>     6/26/95        T‚              1242642 BB:Fix refcounting bugs
                <35>     6/25/95        T‚              1242642 BB: Turn on ODDebug warning if
                                                                        refcount is wrong in
                                                                        ODRefCntObjectsomUninit.
                <34>     6/20/95        JBS             1257315 change Facet::AcquireWindow to
                                                                        GetWindow
                <33>     6/15/95        RR              #1256879 Get->AcquireCurrentMenuBar
                <32>     5/31/95        RR              #1251403 Release after AcquirePart
                <31>     5/26/95        RR              #1251403: Multithreading naming support
                <30>     5/25/95        jpa             Fixed usage of ODDebug. [1253321]
                <29>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
                                                                        refcount
                <28>     5/15/95        RR              # 1249142 Added checks for NULL to
                                                                        PropagateTheEvent
                <27>     5/10/95        RR              # 1242893, 1245692 Fixed Mouse up
                                                                        dispatching
                <26>     5/10/95        JBS             1241913 ODPart::AdjustBorderShape is not
                                                                        called with null shape; 1224727 Facet
                                                                        hit-testing calls should take point param
                                                                        in frame coords
                <25>      5/2/95        RR              # 1226050 Added fLastBorderFrame.
                <24>     4/28/95        RR              1211085 Remove 5$ comments
                <23>      4/6/95        RR              #1220104 Use of ODObjectsAreEqual, #1211085
                                                                        Remove 5$ comments
                <22>      4/4/95        RR              1220104 Use ODObjectsAreEqual, 1227750
                                                                        local coords for mouseUp
                <21>     3/13/95        NP              1221979: Remove incorrect comment.
                <20>     3/10/95        RR              # 1199122 Dispatch Null Menu Events
                <19>      3/7/95        RR              # 122649 WindowToLocal on eventInfo->where
                <18>      3/1/95        RR              # 1224776 Disabled dispatching of activate
                                                                        events, since it interferes with floating
                                                                        window strategy.
                <17>     2/28/95        jpa             Removed a bit o' test code accidentally
                                                                        left in [1220704]
                <16>     2/24/95        jpa             Dispatch kODEvtBGMouseDown &
                                                                        kODEvtBGMouseDownEmbedded [1220704]
                <15>     2/22/95        RR              # 1213850 Dispatch menu events to root part
                                                                        of active window if not handled by menu
                                                                        focus
                <14>     2/20/95        T‚              #1221861 BB: Incomplete AOCE support needs
                                                                        to be removed from OpenDoc
                <13>     1/31/95        RR              # 1162080 delete facet iterator in
                                                                        GetActiveFacetWithBorderUnderPoint
                <12>     1/13/95        RR              Added eventInfo to event handling methods.
                                                                        Remove calls to HandleEventInEmbedded
                <11>     11/1/94        RR              #1196756 Don't call DialogSelect for
                                                                        updates and activates
                <10>    10/27/94        eeh             #1191434 for RR: handle dlogs with the
                                                                        modal focus correctly
                 <9>    10/27/94        RR              # 1191434 Check for ViewAsIcon etc in
                                                                        DispatchToFacetUnderPoint. No longer handle
                                                                        dialog updates and activates
                 <8>    10/21/94        NP              1187682: TSM support.
                 <7>    10/18/94        RR              Check for null containing facet in
                                                                        DispatchToFacetUnderPoint
                 <6>     9/30/94        RR              #1188888, #1171182 For mouse clicks,
                                                                        respect icon views, selected and dragging
                                                                        states.
                 <5>     9/20/94        RR              #1167854. Rewrote DispatchKeyDown so that
                                                                        unmatched command keys are passed to part
                                                                        as a keystroke
                 <4>      9/6/94        RR              #1183392. Call HandleEventInEmbedded where
                                                                        appropriate
                 <3>     8/10/94        JBS             1179919: coordinate system bias changes
                 <2>     7/26/94        eeh             ODQDPoint->Point
                 <1>     6/24/94        RR              first checked in
                 <7>      5/4/94        SS              #1160449: fixes for ModalFocus dialogs
                 <6>      4/8/94        RR              #1144767
                 <5>      4/1/94        T‚              #1154941: Get OpenDoc source to actually
                                                                        compile for PowerPC
                 <4>     3/28/94        CG              1153547: Renamed XMPSessn.h to XMPSessM.h
                 <3>     3/16/94        RR              #1151144. Made global functions static
                 <2>     2/16/94        JA              Include new AltPoint.h.
                <38>      2/7/94        NP              Tiger Team doings.
                <37>      2/7/94        NP              Tiger Team doings.
                <36>     1/21/94        RR              Restored Dispatch() signature
                <35>     1/20/94        RR              Handle AOCE Mailer events
                <34>     1/18/94        RR              Use scrolling focus for pageUp etc., add
                                                                        propagation
                <33>     1/15/94        RR              WinState.h->WinStat.h,
                                                                        Dispatch.h->Disptch.h
                <32>     1/15/94        RR              Updated DispatchMenuEvent (command no. is
                                                                        no longer in event)
                <31>     1/14/94        RR              Shift-click in active border beeps
                <30>     1/11/94        T‚              Init... changes
                <29>     1/10/94        RR              Added HiliteMenu calls
                <28>    12/20/93        RR              New init strategy
                <27>    12/16/93        RR              Various methods respect modal focus. Update
                                                                        and activate events in dialogs are handled.
                                                                        DispatchToFacetUnderPoint became a member
                                                                        function.
                <26>    12/15/93        JBS             activeShape & window now in facet not frame
                <25>    12/13/93        RR              Remove warning.
                <24>     12/6/93        RR              Call WindowState->SuspendResume
                <23>     12/3/93        CC              SuspendResume: Moved call to
                                                                        ExportClipboard() into the shell; no longer
                                                                        modifies its event parameter so shell can
                                                                        post-process the event.
                <22>     12/2/93        CC              SuspendResume: Catch errors thrown by
                                                                        ExportClipboard().
                <21>     12/2/93        RR              Fixed some bugs in handling of mouse events
                <20>    11/24/93        CC              Call ExportClipboard() on suspend events
                <19>    11/19/93        RR              Correctly set "handled"
                <18>    11/19/93        RR              Use correct event codes. Handle command
                                                                        keys. Dispatch mouse downs in title bar to
                                                                        root part.
                <17>    11/18/93        JBS             use activeFrame->AcquireWindow()
                <16>    11/16/93        RR              Central Casting for CFront
                <15>    11/16/93        RR              
                <14>    11/16/93        RR              Shell now constructs menu events. Also
                                                                        update for cursor-tracking and idling
                <13>    11/12/93        JBS             use Facets
                <12>    10/27/93        RR              Use Window::AcquireFrameUnderPoint instead of
                                                                        local routine.
                <11>    10/12/93        PH              Do some cleanup for VodooMonkey
                <10>     10/8/93        RR              Use IsXMPWindow
                 <9>     10/6/93        JA              Use new fixed-point XMPPoint.
                 <8>     10/1/93        RR              Ignore 0 result of MenuSelect()
                 <7>     9/16/93        RR              Menu event uses command number
                 <6>     9/15/93        RR              Use command number in menu event (currently
                                                                        commented out)
                 <5>      9/3/93        JBS             updated to use new Layout class
                 <4>     8/24/93        T‚              commented out ifdef PROTOTYPEBUILD so
                                                                        implementation could use hit detection code
                                                                        from prototype
                 <3>     8/24/93        RR              Got rid of some CFront warnings
                 <2>     8/11/93        RR              Included hit detection code from prototype,
                                                                        for now
                 <1>     8/10/93        RCR             first checked in

        To Do:
        In Progress:
*/

#ifndef _ALTPOINT_
#include "AltPoint.h"                   // Use C++ savvy ODPoint and ODRect
#endif


#ifndef _STDDISPM_
#include "StdDispM.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _ODSESSN_
#include "ODSessn.xh"
#endif

#ifndef SOM_ODDispatcher_xh
#include "Disptch.xh"
#endif

#ifndef SOM_ODWindow_xh
#include "Window.xh"
#endif

#ifndef SOM_ODFacet_xh
#include "Facet.xh"
#endif

#ifndef SOM_ODWindowState_xh
#include "WinStat.xh"
#endif

#ifndef SOM_ODMenuBar_xh
#include "MenuBar.xh"
#endif

//#ifndef SOM_Module_OpenDoc_Commands_defined
//#include "CmdDefs.xh"
//#endif

#ifndef SOM_LinkMgr_xh
#include <LinkMgr.xh>
#endif

#ifndef SOM_ODArbitrator_xh
#include "Arbitrat.xh"
#endif

#ifndef SOM_ODPart_xh
#include "Part.xh"
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include "Foci.xh"
#endif

#ifndef SOM_ODFrame_xh
#include "Frame.xh"
#endif

#ifndef SOM_ODFrameFacetIterator_xh
#include "FrFaItr.xh"
#endif

#ifndef SOM_ODFacetIterator_xh
#include "FacetItr.xh"
#endif

#ifndef _DSPUTILM_
#include "DspUtilM.h"
#endif

#ifndef __EPPC__
#include <EPPC.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __TEXTSERVICES_
#include "TextServices.h"
#endif

#pragma segment ODStdDispatchModule

#define ODDebugMenuEvents 0
#define ODDebugLayerEvents 0
#define ODDebugTitleBarEvents 0
#define ODLogEvents 0

//=====================================================================================
// CONSTANTS
//=====================================================================================

const short     kSuspendResumeMessage = 0x01;   // High byte suspend/resume event
const short     kClipConvertMask      = 0x02;   // Bit of message field clip conversion
const short     kResumeMask           = 0x01;   // Resume vs. suspend mask
const short     kMouseMovedMessage    = 0xFA;   // High byte mouse-moved event message

//----------------------------------------------------------------------------------------
// Enumeration for ASCII Character Constants
//----------------------------------------------------------------------------------------

enum EAsciiControlCode {
        chBackspace = 8,                                        // ASCII code for Backspace character
        chClear = 27,                                           // ASCII code for Clear key (aka ESC)
        chDown = 31,                                            // ASCII code for down arrow
        chEnd = 4,                                                      // ASCII code for the End key
        chEnter = 3,                                            // ASCII code for Enter character
        chEscape = 27,                                          // ASCII code for Escape (aka Clear) key
        chFunction = 16,                                        // ASCII code for any function key
        chFwdDelete = 127,                                      // ASCII code for forward delete
        chHelp = 5,                                                     // ASCII code for Help key
        chHome = 1,                                                     // ASCII code for the Home key
        chLeft = 28,                                            // ASCII code for left arrow
        chPageDown = 12,                                        // ASCII code for Page Down key
        chPageUp = 11,                                          // ASCII code for Page Up key
        chReturn = 13,                                          // ASCII code for Return character
        chRight = 29,                                           // ASCII code for right arrow
        chSpace = 32,                                           // ASCII code for Space character
        chTab = 9,                                                      // ASCII code for Tab character
        chUp = 30                                                       // ASCII code for up arrow
};


//----------------------------------------------------------------------------------------
// Enumeration for Virtual Key Code Constants
//----------------------------------------------------------------------------------------

enum EVirtualKeyCode {
        kClearVirtualCode = 71,                         // Clear key virtual code
        kEscapeVirtualCode = 53,                        // Escape key virtual code
        kF10VirtualCode = 109,                          // F10 virtual key code
        kF11VirtualCode = 103,                          // F11 virtual key code
        kF12VirtualCode = 111,                          // F12 virtual key code
        kF13VirtualCode = 105,                          // F13 virtual key code
        kF14VirtualCode = 107,                          // F14 virtual key code
        kF15VirtualCode = 113,                          // F15 virtual key code
        kF1VirtualCode = 122,                           // F1 virtual key code
        kF2VirtualCode = 120,                           // F2 virtual key code
        kF3VirtualCode = 99,                            // F3 virtual key code
        kF4VirtualCode = 118,                           // F4 virtual key code
        kF5VirtualCode = 96,                            // F5 virtual key code
        kF6VirtualCode = 97,                            // F6 virtual key code
        kF7VirtualCode = 98,                            // F7 virtual key code
        kF8VirtualCode = 100,                           // F8 virtual key code
        kF9VirtualCode = 101,                           // F9 virtual key code
        kFwdDelVirtualCode = 117                        // Forward Delete virtual code
};

//=====================================================================================
// Class MacDispatchModule
//=====================================================================================

//-------------------------------------------------------------------------------------
// MacDispatchModule::MacDispatchModule
//
// Description
//-------------------------------------------------------------------------------------

MacDispatchModule::MacDispatchModule()
{
        fArbitrator = kODNULL;
        fWindowState = kODNULL;

        fMouseDownFacet = kODNULL;
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::~MacDispatchModule
//
// Description
//-------------------------------------------------------------------------------------

MacDispatchModule::~MacDispatchModule()
{
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::InitStandardDispatchModule
//
// Description
//-------------------------------------------------------------------------------------

void MacDispatchModule::InitMacDispatchModule(Environment* ev, ODSession* session)
{
        fSession = session;
        // Cache tokenized focus names
        fKeyFocusToken = fSession->Tokenize(ev, kODKeyFocus);
        fMenuFocusToken = fSession->Tokenize(ev, kODMenuFocus);
        fSelectionFocusToken = fSession->Tokenize(ev, kODSelectionFocus);
        fModalFocusToken = fSession->Tokenize(ev, kODModalFocus);
        fMouseFocusToken = fSession->Tokenize(ev, kODMouseFocus);
        fScrollingFocusToken = fSession->Tokenize(ev, kODScrollingFocus);

        fLargeIconViewToken = fSession->Tokenize(ev, kODViewAsLargeIcon);
        fSmallIconViewToken = fSession->Tokenize(ev, kODViewAsSmallIcon);
        fThumbnailViewToken = fSession->Tokenize(ev, kODViewAsThumbnail);

        // Cache session globals for easy access

        fArbitrator = fSession->GetArbitrator(ev);
        fWindowState = fSession->GetWindowState(ev);
        fDispatcher = fSession->GetDispatcher(ev);
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::Dispatch
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::Dispatch(Environment* ev, ODEventData* event, ODEventInfo* eventInfo)
{
        ODEventType eventType = event->what;

        switch (eventType)
        {
                case nullEvent:
                        break;          // Currently handled in ODDispatcher

                case mouseDown:
                case kODEvtBGMouseDown:
                        return this->DispatchMouseDownEvent(ev, event, eventInfo);

                case kODEvtMouseDownBorder:     // via Redispatch
                        return this->DispatchMouseDownInBorder(ev, event, eventInfo);

                case kODEvtMouseDownEmbedded:
                case kODEvtBGMouseDownEmbedded:
                        return this->DispatchMouseDownEmbedded(ev, event, eventInfo);

                case kODEvtMouseUpEmbedded:
                        return this->DispatchMouseUpEmbedded(ev, event, eventInfo);

                case mouseUp:
                        return this->DispatchMouseUpEvent (ev, event, eventInfo);

                case keyDown:
                        return this->DispatchKeyDownEvent(ev, event, eventInfo);

                case autoKey:
                        return this->DispatchKeyDownEvent(ev, event, eventInfo);

                case keyUp:
                        return this->DispatchKeyUpEvent(ev, event, eventInfo);

                case updateEvt:
                        return this->DispatchUpdateEvent(ev, event);

                case activateEvt:
                        return this->DispatchActivateEvent(ev, event);

                case osEvt:
                        this->DispatchOSEvent(ev, event);
                        return kODTrue;

                case kHighLevelEvent:
                        break;  // Handled by shell

                case kODEvtMenu:
                        return this->DispatchMenuEvent(ev, event, eventInfo);

                default:
                        return kODFalse;
        }
        return kODFalse;
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchOSEvent
//
// Description
//-------------------------------------------------------------------------------------

void MacDispatchModule::DispatchOSEvent(Environment* ev, ODEventData* theEvent)
{
        unsigned char   typeOSEvent;

        // ¥ Is it a multifinder event?
        typeOSEvent = (unsigned char) ( theEvent->message >> 24 ) & 0x00FF;

        // ¥ Switch on the type of OSEvent that occurred, high byte of message is event type
        switch ( typeOSEvent )
        {
                case kMouseMovedMessage:
                        this->MouseMoved(ev, theEvent);
                        break;

                case kSuspendResumeMessage:
                        this->SuspendResume(ev, theEvent);
                        break;
        }
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::MouseMoved
//
// Description
//-------------------------------------------------------------------------------------

void MacDispatchModule::MouseMoved(Environment* ev, ODEventData* theEvent)
{
        ODUnused(theEvent);

        fDispatcher->InvalidateMouseRegion(ev);
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::SuspendResume
//
// Description
//-------------------------------------------------------------------------------------

void MacDispatchModule::SuspendResume(Environment* ev, ODEventData* theEvent)
{
        fWindowState->SuspendResume(ev, theEvent);

        ODLinkManager* linkMgr = fSession->GetLinkManager(ev);
        linkMgr->SuspendResume(ev, theEvent);
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchMenuEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchMenuEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;

        long menuResult = theEvent->message;
        short menu = HiWord(menuResult);
        short item = LoWord(menuResult);

        //if (menu != 0) // #
        {
                TempODFrame targetFrame = fArbitrator->AcquireFocusOwner(ev, fMenuFocusToken);

#if ODDebug && ODDebugMenuEvents
        somPrintf("Menu Event %d, %d for frame %x\n",menu, item, (ODFrame*)targetFrame);
#endif

                if ( targetFrame )
                {
                        handled = this->DispatchTheEvent(ev, theEvent, targetFrame, kODNULL, eventInfo);
                }
                // Note: To support root menu items like Print. We could add a new focus for this
                if (!handled)
                {
                        TempODWindow window = fWindowState->AcquireActiveWindow(ev);
                        ODFrame* rootFrame = window ? window->GetRootFrame(ev) : kODNULL;
                        if (rootFrame && (rootFrame != targetFrame))
                        {
                                TempODPart rootPart = rootFrame->AcquirePart(ev);
#if ODDebug && ODLogEvents
        LogEvent(theEvent, eventInfo, rootFrame, kODNULL);
#endif
                                handled = rootPart->HandleEvent(ev, theEvent, rootFrame, kODNULL, eventInfo);
                        }
                }
        }
        //else
        //      handled = kODTrue;

        //if (handled)
        HiliteMenu(0);

        return handled;
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchMouseDownInContent
//
// Description
//-------------------------------------------------------------------------------------

        static ODBoolean IsContained(Environment* ev, ODFrame* frame, ODFrame* container)
        {
                if ( (frame == kODNULL) || (container == kODNULL))
                        return kODFalse;
                if (ODObjectsAreEqual(ev, frame, container))
                        return kODTrue;

                ODFrame* tempFrame = frame;
                ODFrame* tempContainer = kODNULL;
                tempFrame->Acquire(ev); // -- T‚ added
                while (tempFrame != kODNULL)
                {
                        tempContainer = tempFrame->AcquireContainingFrame(ev);
                        tempFrame->Release(ev); // -- T‚ added
                        if (ODObjectsAreEqual(ev, tempContainer, container))
                        {
                                tempContainer->Release(ev); // -- T‚ added
                                return kODTrue;
                        }
                        tempFrame = tempContainer;
                }
                return kODFalse;
        }

        static ODFacet* GetActiveFacet(Environment* ev, ODFrame* frame, ODWindow* window, ODPoint& windowPoint)
        // Could be a frame method
        // Return the facet of "frame" which contains the point, or kODNULL
        {
                ODFacet* foundFacet = kODNULL;
                ODPoint framePoint;

                if ( frame && window )
                {
                        ODFrameFacetIterator* iter = frame->CreateFacetIterator(ev);

                        for (ODFacet* facet = iter->First(ev);
                                        iter->IsNotComplete(ev);
                                        facet = iter->Next(ev))
                        {
                                {
                                        // get windowPoint in frame coords for hit-testing
                                        TempODTransform winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
                                        framePoint = windowPoint;
                                        winToFrame->InvertPoint(ev, &framePoint);
                                }
                                if (ODObjectsAreEqual(ev, facet->GetWindow(ev), window)
                                        && facet->ContainsPoint(ev, &framePoint, kODNULL) )
                                {
                                        foundFacet = facet;
                                        break;
                                }
                        }

                        delete iter;
                }
                return foundFacet;
        }


        static ODFacet* GetFirstFacetUnderPoint(Environment* ev, ODWindow* window, ODFacet* root, ODPoint& windowPoint)
        {
        // returns NULL, root or an embedded facet of root

                ODFacet* foundFacet = kODNULL;
                ODPoint framePoint;

                if (window && root && (ODObjectsAreEqual(ev, root->GetWindow(ev),window)))
                {
                        ODFacetIterator* t = root->CreateFacetIterator(ev, kODTopDown, kODFrontToBack);
                        TempODObject tempIterator(t); // DMc - ensure deletion despite return/exceptions

                        ODFacet* facet = t->First(ev);

                        // get windowPoint in frame coords for hit-testing
                        {
                                TempODTransform winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
                                framePoint = windowPoint;
                                winToFrame->InvertPoint(ev, &framePoint);
                        }

                        if ( !facet->ContainsPoint(ev, &framePoint, kODNULL) )
                                return kODNULL;

                        for ( facet = t->Next(ev);
                                        t->IsNotComplete(ev);
                                        facet = t->Next(ev) )
                        {
                                // get windowPoint in frame coords for hit-testing
                                {
                                        TempODTransform winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
                                        framePoint = windowPoint;
                                        winToFrame->InvertPoint(ev, &framePoint);
                                }
                                if (facet->ContainsPoint(ev, &framePoint, kODNULL))
                                {
                                        foundFacet = facet;
                                        break;
                                }
                                else
                                        t->SkipChildren(ev);
                        }
                        // delete t; // now deleted by TempODObject
                }
                if ( foundFacet )
                        return foundFacet;
                else
                        return root;
        }

ODBoolean MacDispatchModule::DispatchToFacetUnderPoint(Environment* ev, ODWindow* window, ODPoint& windowPt,
                                                                                                        ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;
        ODFacet* facet = window->GetFacetUnderPoint(ev, &windowPt);
        ODFrame* frame = facet ? facet->GetFrame(ev) : kODNULL;

        if (facet && frame)
        {
                TempODFrame modalFocus = fArbitrator->AcquireFocusOwner(ev, fModalFocusToken);
                if ( modalFocus && !IsContained(ev, frame, modalFocus))
                {
                        TempODPart part = modalFocus->AcquirePart(ev); // -- T‚ tempobj'd
                        if (part)
                        {
#if ODDebug && ODLogEvents
        LogEvent(theEvent, eventInfo, modalFocus, kODNULL);
#endif
                                handled = part->HandleEvent(ev, theEvent, modalFocus, kODNULL, eventInfo);
                        }
                }
                else
                {
                        ODTypeToken frameView = frame->GetViewType(ev);
                        ODBoolean isFrozen = frame->IsFrozen(ev);
                        ODBoolean isSelected = facet->IsSelected(ev);
                        ODBoolean isIcon = (frameView == fLargeIconViewToken ||
                                                                frameView == fSmallIconViewToken ||
                                                                frameView == fThumbnailViewToken);
                        ODBoolean isDragging = frame->IsDragging(ev);
                        ODBoolean isMouseDown = (theEvent->what != mouseUp);

                        if ( isFrozen
                                || isSelected && isMouseDown
                                || isIcon
                                || isDragging && !isMouseDown)
                        {
                                //PRINT("Mouse Down or Up to container\n");
                                switch( theEvent->what )
                                {
                                        case mouseDown:                 theEvent->what = kODEvtMouseDownEmbedded; break;
                                        case kODEvtBGMouseDown: theEvent->what = kODEvtBGMouseDownEmbedded; break;
                                        default:                                theEvent->what = kODEvtMouseUpEmbedded; break;
                                }
                                eventInfo->embeddedFrame = frame;
                                eventInfo->embeddedFacet = facet;

                                ODFacet* containingFacet = kODNULL;
                                containingFacet = facet->GetContainingFacet(ev);


                                if ( containingFacet == kODNULL )
                                        return kODFalse;  // pass to shell/containerApp if this is a root frame
                                else
                                {
                                        // Cache the mouse down facet for use in mouse up dispatching, except
                                        // if the facet is selected, in which case the mouse up should go to the facet
                                        // under the mouse, provided the container doesn't do a drag.
                                        if (isMouseDown && (isFrozen || isIcon))
                                        {
                                                fMouseDownFacet = containingFacet;
                                                fEmbeddedFacet = facet;
                                        }
                                        handled = fDispatcher->Redispatch(ev, theEvent, eventInfo);
                                }
                        }
                        else
                        {
                                fMouseDownFacet = facet;
                                handled = this->DispatchTheEvent(ev, theEvent, frame, facet, eventInfo);
                        }
                }
        }
        return handled;
}

ODBoolean MacDispatchModule::DispatchMouseDownInContent (Environment* ev, ODWindow* theWindow,
                                                                                                                                ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;
        ODFacet* activeFacet = kODNULL;

        ODPoint windowPt;
        theWindow->GetWindowPoint(ev, &theEvent->where, &windowPt);

        ODBoolean modified = ((theEvent->modifiers & cmdKey) || (theEvent->modifiers & shiftKey)) != 0;
        ODBoolean tooModified = ((theEvent->modifiers & optionKey) || (theEvent->modifiers & controlKey) ) != 0;
        modified = modified && !tooModified;
        eventInfo->where = windowPt;

        TempODFrame activeFrame = fArbitrator->AcquireFocusOwner(ev, fSelectionFocusToken); // -- T‚ tempobj'd
        if ((ODFrame*)activeFrame )
                activeFacet = GetActiveFacetWithBorderUnderPoint(ev, activeFrame, theWindow, ODPoint(windowPt));

        if ( activeFacet ) // Mouse down in border
        {
                if( theEvent->what==kODEvtBGMouseDown )
                        handled = kODFalse;
                else
                {
                        theEvent->what = kODEvtMouseDownBorder;
                        eventInfo->embeddedFrame = activeFrame;
                        eventInfo->embeddedFacet = activeFacet;
                        handled = fDispatcher->Redispatch(ev, theEvent, eventInfo);
                }
        }
        else // Click not in border
        {
                if (modified)
                {
                        ODFacet* rootFacet = activeFrame ? GetActiveFacet(ev, activeFrame, theWindow, windowPt)
                                : (ODFacet*) kODNULL;
                        ODFacet* facet = GetFirstFacetUnderPoint(ev, theWindow, rootFacet, windowPt);
                        // returns NULL, rootFacet or embeddedFrame

                        if (facet)
                        {
                                if (ODObjectsAreEqual(ev, facet, rootFacet))
                                {
                                        fMouseDownFacet = rootFacet;
                                        handled = this->DispatchTheEvent(ev, theEvent, activeFrame, rootFacet, eventInfo);
                                }
                                else // frame is embedded in active frame
                                {
                                        theEvent->what = (theEvent->what==kODEvtBGMouseDown) ? kODEvtBGMouseDownEmbedded
                                                                                                                                                 : kODEvtMouseDownEmbedded;
                                        eventInfo->embeddedFrame = facet->GetFrame(ev);
                                        eventInfo->embeddedFacet = facet;
                                        fMouseDownFacet = rootFacet;
                                        fEmbeddedFacet = facet;
                                        //handled = this->DispatchTheEvent(ev, theEvent, activeFrame, rootFacet, eventInfo);
                                        handled = fDispatcher->Redispatch(ev, theEvent, eventInfo);
                                }
                        }
                        else // Modified click outside active frame
                        {
                                SysBeep(2);
                                fSuppressMouseUp = kODTrue;
                        }
                }
                else // No modifiers
                {
                        handled = DispatchToFacetUnderPoint(ev, theWindow, windowPt, theEvent, eventInfo);
                }
        }
        return handled;
} // MouseDownInContent

ODBoolean MacDispatchModule::DispatchMouseDownInBorder(Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;
        ODBoolean modified = ((theEvent->modifiers & cmdKey) || (theEvent->modifiers & shiftKey) ||
                                                  (theEvent->modifiers & optionKey) || (theEvent->modifiers & controlKey) ) != 0;

        if (modified)
        {
                SysBeep(2); // Shift-click or Command-click in border
                fSuppressMouseUp = kODTrue;
                handled = kODTrue;
        }
        else
        {
                ODFacet* containingFacet = eventInfo->embeddedFacet->GetContainingFacet(ev);
                fMouseDownFacet = containingFacet;

                handled = this->DispatchTheEvent(ev, theEvent, containingFacet->GetFrame(ev), containingFacet, eventInfo);
        }
        return handled;
}

ODBoolean MacDispatchModule::DispatchMouseDownEmbedded(Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;

        ODFacet* containingFacet = eventInfo->embeddedFacet->GetContainingFacet(ev);

        // We don't cache mouse down facet here, because the mouse up goes to the embedded part in some cases

        handled = this->DispatchTheEvent(ev, theEvent, containingFacet->GetFrame(ev), containingFacet, eventInfo);

        return handled;
}

ODBoolean MacDispatchModule::DispatchMouseUpEmbedded(Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;

        if (fSuppressMouseUp)
                return kODTrue;

        if (fMouseDownFacet && fEmbeddedFacet)
                handled = this->DispatchTheEvent(ev, theEvent, fMouseDownFacet->GetFrame(ev), fMouseDownFacet, eventInfo);
        else
                handled = kODTrue;

        return handled;
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchMouseDownEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchMouseDownEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{

        ODBoolean handled = kODFalse;

        TempODFrame mouseFocusFrame = fArbitrator->AcquireFocusOwner(ev, fMouseFocusToken);
        if (mouseFocusFrame)
        {
                TempODPart part = mouseFocusFrame->AcquirePart(ev);
                if ((ODPart*)part)
                {
#if ODDebug && ODLogEvents
        LogEvent(theEvent, eventInfo, mouseFocusFrame, fMouseDownFacet);
#endif
                        handled = part->HandleEvent(ev, theEvent, mouseFocusFrame, fMouseDownFacet, eventInfo);
                }
                return handled;
        }

        WindowPtr platformWindow = kODNULL;
        TempODWindow aWindow = kODNULL;
        short           partCode;

        fMouseDownFacet = kODNULL;
        fEmbeddedFacet = kODNULL;
        fSuppressMouseUp = kODFalse;

        partCode = FindWindow ( theEvent->where, &platformWindow );


        if (platformWindow && fWindowState->IsODWindow(ev, platformWindow))
                aWindow = fWindowState->AcquireODWindow(ev, platformWindow);

        // Handle only clicks in OD content if this is a bg click:
        if( theEvent->what==kODEvtBGMouseDown )
        {
                fSuppressMouseUp = kODTrue;
                if( aWindow==kODNULL || partCode!=inContent )
                        return kODFalse;
        }
        if (partCode == inMenuBar)
        {
                long menuResult = MenuSelect(theEvent->where);

                if (!TSMMenuSelect(menuResult))
                {
                        theEvent->what = kODEvtMenu;
                        theEvent->message = menuResult;
                        handled = fDispatcher->Redispatch(ev, theEvent, eventInfo);
                }
        }
        else if (aWindow)
        {
                switch (partCode)
                {
                        case inContent:
                                handled = this->DispatchMouseDownInContent(ev, aWindow, theEvent, eventInfo);
                                break;

                        case inGoAway:
                                if (TrackGoAway (platformWindow, theEvent->where))
                                        handled = this->DispatchWindowEvent(ev, aWindow, partCode, theEvent, eventInfo);
                                else
                                        handled = kODTrue;
                                break;

                        case inDrag:
                                handled = this->DispatchWindowEvent(ev, aWindow, partCode, theEvent, eventInfo);
                                break;

                        case inGrow:
                                handled = this->DispatchWindowEvent(ev, aWindow, partCode, theEvent, eventInfo);
                                break;

                        case inZoomIn:
                        case inZoomOut:
                                if (TrackBox(platformWindow, theEvent->where, partCode))
                                        handled = this->DispatchWindowEvent(ev, aWindow, partCode, theEvent, eventInfo);
                                else
                                        handled = kODTrue;
                                break;

                        default:
                                break;
                }
        }

        return handled;

}       //      MacDispatchModule::DispatchMouseDownEvent

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchWindowEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchWindowEvent(Environment* ev, ODWindow* window, short partCode,
                                                                                                        ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;

        TempODFrame modalFocus = fArbitrator->AcquireFocusOwner(ev, fModalFocusToken);
        if (modalFocus && !IsContained(ev, window->GetRootFrame(ev), modalFocus))
        {
                TempODPart part = modalFocus->AcquirePart(ev); // -- T‚ tempobj'd
                if ((ODPart*)part)
                {
#if ODDebug && ODLogEvents
        LogEvent(theEvent, eventInfo, modalFocus, kODNULL);
#endif
                        handled = part->HandleEvent(ev, theEvent, modalFocus, kODNULL, eventInfo);
                }
        }
        else
        {
                ODFacet* rootFacet = window->GetRootFacet(ev);
                ODFrame* rootFrame = window->GetRootFrame(ev);

                theEvent->what = kODEvtWindow;
                theEvent->message = partCode;
#if ODDebug && ODDebugTitleBarEvents
                somPrintf("Title Bar Event for root frame %x\n", rootFrame);
#endif
                handled = this->DispatchTheEvent(ev, theEvent, rootFrame, rootFacet, eventInfo);
        }
        return handled;
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchMouseUpEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchMouseUpEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;
        WindowPtr platformWindow = kODNULL;
        TempODWindow aWindow = kODNULL;
        short           partCode;

        if (fSuppressMouseUp)
                return kODTrue;

        TempODFrame mouseFocusFrame = fArbitrator->AcquireFocusOwner(ev, fMouseFocusToken);
        if (mouseFocusFrame)
        {
                TempODPart part = mouseFocusFrame->AcquirePart(ev);
                if ((ODPart*)part)
                {
#if ODDebug && ODLogEvents
        LogEvent(theEvent, eventInfo, mouseFocusFrame, fMouseDownFacet);
#endif
                        handled = part->HandleEvent(ev, theEvent, mouseFocusFrame, fMouseDownFacet, eventInfo);
                }
                return handled;
        }

#ifdef ODDebug
//somPrintf("Mouse Up\n");
#endif

        partCode = FindWindow ( theEvent->where, &platformWindow );
        ODBoolean modified = ((theEvent->modifiers & cmdKey) || (theEvent->modifiers & shiftKey)) != 0;
        ODBoolean tooModified = ((theEvent->modifiers & optionKey) || (theEvent->modifiers & controlKey) ) != 0;
        modified = modified && !tooModified;


        if (platformWindow)
                aWindow = fWindowState->AcquireODWindow(ev, platformWindow);

        if (aWindow)
        {
                ODPoint windowPt;
                aWindow->GetWindowPoint(ev, &theEvent->where, &windowPt);

                eventInfo->where = windowPt;

                switch ( partCode )
                {
                                case inContent:
                                        // if (!modified)       // #1242893: What if it is?
                                        {
                                                if (fMouseDownFacet)
                                                {
                                                        if (fEmbeddedFacet)
                                                        {
                                                                theEvent->what = kODEvtMouseUpEmbedded;
                                                                eventInfo->embeddedFacet = fEmbeddedFacet;
                                                                eventInfo->embeddedFrame = fEmbeddedFacet->GetFrame(ev);
                                                                handled = fDispatcher->Redispatch(ev, theEvent, eventInfo);
                                                        }
                                                        else
                                                                handled = this->DispatchTheEvent(ev, theEvent, fMouseDownFacet->GetFrame(ev), fMouseDownFacet, eventInfo);
                                                }
                                                else
                                                        handled = DispatchToFacetUnderPoint(ev, aWindow, windowPt, theEvent, eventInfo);
                                        }
                                        break;

                                default:
                                        break;
                }
        }
        return handled;
}       //      MacDispatchModule::DispatchMouseUpEvent

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchKeyDownEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchKeyDownEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;
        const short kNotAllowed =  shiftKey | optionKey | controlKey;
        char key = (char) (theEvent->message & charCodeMask) ;

        if ( (theEvent->modifiers & cmdKey) && !(theEvent->modifiers & kNotAllowed) )
        {
                long menuResult = MenuKey(key);
                short menu = HiWord(menuResult);
                short item = LoWord(menuResult);

                if (menu == 0)  // Unmatched command keys, eg. Command-Period or Disabled Item
                                                // Sent to part as keystroke
                {
                        TempODFrame keyFrame = fArbitrator->AcquireFocusOwner(ev, fKeyFocusToken);
                        handled = this->DispatchTheEvent(ev, theEvent, keyFrame, kODNULL, eventInfo);
                }
                else    // Turn it into a menu event
                {
                        theEvent->what = kODEvtMenu;
                        theEvent->message = menuResult;
                        handled = fDispatcher->Redispatch(ev, theEvent, eventInfo);

                        if (!handled)   // Deal with Close as a special case
                        {
                                ODMenuBar* menuBar = fWindowState->AcquireCurrentMenuBar(ev);
                                ODCommandID command = menuBar ? menuBar->GetCommand(ev, menu, item) : 0;
                                menuBar->Release(ev);

                                if (command == kODCommandClose)
                                {
                                        TempODWindow activeWindow = fWindowState->AcquireActiveWindow(ev);
                                        ODFrame* rootFrame = activeWindow ? activeWindow->GetRootFrame(ev) : kODNULL;
                                        TempODPart rootPart = rootFrame ? rootFrame->AcquirePart(ev) : kODNULL;  // -- T‚ tempobj'd

                                        if (rootPart)
                                        {
#if ODDebug && ODLogEvents
        LogEvent(theEvent, eventInfo, rootFrame, kODNULL);
#endif
                                                handled = rootPart->HandleEvent(ev, theEvent,rootFrame, kODNULL, eventInfo);
                                        }
                                }
                        }
                }
        }
        else    // Regular unmodified keystrokes
        {
                TempODFrame scrollingFrame = fArbitrator->AcquireFocusOwner(ev, fScrollingFocusToken);
                if (scrollingFrame &&
                        ((key == chPageUp) || (key == chPageDown) || (key == chHome) || (key == chEnd)))
                {
                        handled = this->DispatchTheEvent(ev, theEvent, scrollingFrame, kODNULL, eventInfo);
                }

                if (!handled)
                {
                        TempODFrame keyFrame = fArbitrator->AcquireFocusOwner(ev, fKeyFocusToken);
                        handled = this->DispatchTheEvent(ev, theEvent, keyFrame, kODNULL, eventInfo);
                }
        }
        return handled;
}       //      MacDispatchModule::DispatchKeyDownEvent

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchKeyUpEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchKeyUpEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;
        {
                TempODFrame targetFrame = fArbitrator->AcquireFocusOwner(ev, fKeyFocusToken);
                handled = this->DispatchTheEvent(ev, theEvent, targetFrame, kODNULL, eventInfo);
        }
        return handled;
}

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchUpdateEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchUpdateEvent (Environment* ev, ODEventData* theEvent)
{
        ODBoolean handled = kODFalse;

        WindowPtr platformWindow = (WindowPtr) theEvent->message;
        TempODWindow aWindow = kODNULL;

        if (platformWindow && fWindowState->IsODWindow(ev, platformWindow))
                aWindow = fWindowState->AcquireODWindow(ev, platformWindow);

        if ( aWindow )
        {
                aWindow->HandleUpdateEvent(ev, theEvent);
                handled = kODTrue;
        }
        return handled;
}       //      MacDispatchModule::DispatchUpdateEvent

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchActivateEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchActivateEvent      (Environment* ev, ODEventData* theEvent)
{
        ODBoolean handled = kODFalse;

        WindowPtr platformWindow = (WindowPtr) theEvent->message;
        TempODWindow aWindow = kODNULL;

        if (platformWindow)
                aWindow = fWindowState->AcquireODWindow(ev, platformWindow);

        if (aWindow )
        {
#if ODDebug && ODDebugLayerEvents
        //if ( ((WindowPeek)platformWindow)->windowKind != dialogKind)
        //{
                if ((theEvent->modifiers & activeFlag) != 0)
                        somPrintf("Ignoring Activate Event from event loop for root frame %x\n", aWindow->GetRootFrame(ev));
                else
                        somPrintf("Ignoring Deactivate Event from event loop for root frame %x\n", aWindow->GetRootFrame(ev));
        //}
#endif
                // Note: Ignoring activate events solves the following problem:
                // When using the Drafts dialog to select an existing draft,
                // the dialog would generate an activate event for the window below it
                // when closed. The shell then opens the selected draft which brings its
                // window to the front using ODWindow::Select (i.e. no events).
                // But then the activate event  comes in for the window which is no
                // longer frontmost.

                // Bit 1 (starting from 0) of the modifiers field is set in ODWindow::Activate/Deactivate
                // to alert the dispatcher that this is an OpenDoc-generated activate event. Toolbox-generated
                // activates are ignored.

                if (IsODActivateEvent(*theEvent))
                {
#if ODDebug && ODDebugLayerEvents
                if ((theEvent->modifiers & activeFlag) != 0)
                        somPrintf("OD Activate Event for root frame %x\n", aWindow->GetRootFrame(ev));
                else
                        somPrintf("OD Deactivate Event for root frame %x\n", aWindow->GetRootFrame(ev));
#endif
                        aWindow->HandleActivateEvent(ev, theEvent);
                }
                else
                {
#if ODDebug && ODDebugLayerEvents
                if ((theEvent->modifiers & activeFlag) != 0)
                        somPrintf("MacOS Activate Event for root frame %x\n", aWindow->GetRootFrame(ev));
                else
                        somPrintf("MacOS Deactivate Event for root frame %x\n", aWindow->GetRootFrame(ev));
#endif
                }

                handled = kODTrue;
        }
        return handled;
}       //      MacDispatchModule::DispatchActivateEvent

//-------------------------------------------------------------------------------------
// MacDispatchModule::DispatchTheEvent
//
// Dispatches the event to the initial target, or to a containing frame/facet,
// if the initial target doesn't handle it, and the containing part has
// set the "DoesPropagateEvents" flag of the embedded frame
//-------------------------------------------------------------------------------------

ODBoolean MacDispatchModule::DispatchTheEvent(Environment* ev,
                                                        ODEventData* theEvent,
                                                        ODFrame* targetFrame,
                                                        ODFacet* targetFacet,
                                                        ODEventInfo* eventInfo)
{
        ODBoolean handled = kODFalse;
        TempODPart targetPart = targetFrame ? targetFrame->AcquirePart(ev) : kODNULL; // -- T‚ tempobj'd

        if ((ODPart*)targetPart)
        {
                if (eventInfo && targetFacet)
                        WindowToLocal(ev, eventInfo->where, targetFacet);
#if ODDebug && ODLogEvents
        LogEvent(theEvent, eventInfo, targetFrame, targetFacet);
#endif
                if (eventInfo && eventInfo->embeddedFrame)
                {
                        handled = targetPart->HandleEvent(ev, theEvent, targetFrame, targetFacet, eventInfo);
                }
                else
                        handled = targetPart->HandleEvent(ev, theEvent, targetFrame, targetFacet, eventInfo);
                if (!handled)
                {
                        handled = PropagateTheEvent(ev, theEvent,targetFrame, targetFacet, eventInfo);
                }
        }
        return handled;
}

ODBoolean MacDispatchModule::PropagateTheEvent(Environment* ev,
                                                        ODEventData* theEvent,
                                                        ODFrame* initialFrame,
                                                        ODFacet* initialFacet,
                                                        ODEventInfo* eventInfo)
{
        ODFacet* targetFacet = initialFacet;
        TempODFrame targetFrame = initialFacet ? initialFacet->GetFrame(ev) : initialFrame; // DMc refcount - temp
        // ODPart* targetPart = initialFrame ? initialFrame->AcquirePart(ev) : kODNULL; // Appears to be obsolete -T‚
        ODBoolean handled = kODFalse;

        targetFrame->Acquire(ev);       // -- T‚ added
        while (targetFrame)
        {
                if (handled  || (!targetFrame->DoesPropagateEvents(ev)))
                        break;

                if (targetFacet)
                {
                        targetFacet = targetFacet->GetContainingFacet(ev);
                        ODReleaseObject(ev, targetFrame); // DMc: about to reassign
                        targetFrame = targetFacet ? targetFacet->GetFrame(ev) : kODNULL;
                }
                else
                {
                        ODFrame* contFrame = targetFrame->AcquireContainingFrame(ev);   // -- T‚ added
                        ODReleaseObject(ev, targetFrame);                                                               // -- T‚ added
                        targetFrame = contFrame;
                }

                {
                        TempODPart targetPart = targetFrame ? targetFrame->AcquirePart(ev) : kODNULL; // -- T‚ tempobj'd
                        if ((ODPart*)targetPart)
                        {
                                eventInfo->propagated = kODTrue;
                                handled = targetPart->HandleEvent(ev, theEvent, targetFrame, targetFacet, eventInfo);
                        }
                }
        }

        return handled;
}

#endif // _PLATFORM_MACINTOSH_
