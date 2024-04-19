/* @(#)Z 1.22 com/src/ui/StdDispO.cpp, odui, od96os2, odos29646d 96/11/15 15:49:42 (96/11/01 10:36:50) */
//#====START_GENERATED_PROLOG======================================
//#
//#
//#   COMPONENT_NAME: odui
//#
//#   CLASSES: none
//#
//#   ORIGINS: 82,27
//#
//#
//#   (C) COPYRIGHT International Business Machines Corp. 1995,1996
//#   All Rights Reserved
//#   Licensed Materials - Property of IBM
//#   US Government Users Restricted Rights - Use, duplication or
//#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//#   	
//#   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
//#   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//#   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
//#   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
//#   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
//#   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
//#   OR PERFORMANCE OF THIS SOFTWARE.
//#
//#====END_GENERATED_PROLOG========================================
//#
/*
  File:    StdDispO.cpp

  Contains:  Implementation of standard dispatch module for Macintosh events

  Written by:  Chuck Dumont

  Copyright:  © 1993 - 1995 by IBM, all rights reserved.

  Change History (most recent first):

     138899 11/12/95 ced Support embedded PM controls handling drag/drop
     137664 9/21/95 aml  API change for ODTransform::InverPoint
     137091 9/15/95 st   change WM_xxDBLCLK event handling
     133968 8/25/95 pfe  Port of DR3 Apple code
     133565 8/18/95 ced  Remove CS_PARENTCLIP style from facet windows.
     129403 7/28/95 aml  Added support for scrolling events; send them to the root part
     119934 7/28/95 aml  handle font & color palette drop on embedded PM controls
     128947 7/25/95 st   route WM_xxDBLCLK events to part under mouse
     127662 7/25/95 BLG  OD_HELP processed as Key event vs old as menu event.
     129147 7/19/95 aml  Added support for WM_REALIZEPALETTE and WM_PRESPARAMCHANGED
     129003 7/11/95 ced  Fix keyboard focus bug in DispatchMouseEvent
     125700 6/14/95 ced  Change InitOS2DispatchModule to use const defined in
                         facet.xh for number of reserved window words when
                         registering facet window class.
     123149 5/23/95 ced  Pass WM_BUTTON1CLICK to selected embedded frames
                         instead of WM_BUTTON1DOWN
     <7> defect 120683 - Improve mouse over active border code
     <6>  05/03/95   BLG  119564 Number of messages not dispatched correctly.
     119388  4/29/95  st  changes for menu code
     120604  4/28/95 ced  Merge with B1C16 Apple code.
     <5>  04/25/95  BLG    118998 - WM_CONTEXTMENU not dispatched correctly
     <4>  04/25/95  BLG    110736 - change pointer when over active part border
     <3>   4/11/95  BLG   added support for WM_MENUSELECT
     <2>   1/13/95  CED   Added eventInfo to event handling methods.
                          Remove calls to HandleEventInEmbedded
     <1>   8/16/94  CED    Port from Apple code

  To Do:
*/
#ifdef _PLATFORM_OS2_
#include <ODos2.h>
#include <ODRes.h>
#endif

#ifndef _ALTPOINT_
#include "AltPoint.h"      // Use C++ savvy ODPoint and ODRect
#endif

#include "ODMath.h"

#ifndef _ODUTILS_
#include "odutils.h"
#endif

#ifdef _PLATFORM_OS2_
#ifndef _STDDISPO_
#include "StdDispO.h"
#endif
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

#ifndef SOM_Module_OpenDoc_Commands_defined
#include "CmdDefs.xh"
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

#ifndef _SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef SOM_ODFrameFacetIterator_xh
#include "FrFaItr.xh"
#endif

#ifndef _DSPUTILM_
#include "DspUtilM.h"
#endif

#ifndef SOM_ODFacetIterator_xh
#include "FacetItr.xh"
#endif

#ifndef SOM_ODDragAndDrop_xh
#include "DragDrp.xh"
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_Module_ODTypesM_OpenDoc_Global_Types_defined
#include <ODTypesM.xh>
#endif
#ifndef SOM_ODClipboard_xh
#include <clipbd.xh>
#endif

//=====================================================================================
// Class OS2DispatchModule
//=====================================================================================

ODEventType ChangeEventTypeToEmbedded(ODEventType evtType);

//-------------------------------------------------------------------------------------
// OS2DispatchModule::OS2DispatchModule
//
// Description
//-------------------------------------------------------------------------------------

OS2DispatchModule::OS2DispatchModule()
{
  fArbitrator = kODNULL;
  fWindowState = kODNULL;
  fPrevDragTarget = 0;             // [138899] - ced
  fNeverDropTarget = kODFalse;     // [138899] - ced
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::~OS2DispatchModule
//
// Description
//-------------------------------------------------------------------------------------

OS2DispatchModule::~OS2DispatchModule()
{
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::InitStandardDispatchModule
//
// Description
//-------------------------------------------------------------------------------------

void OS2DispatchModule::InitOS2DispatchModule(Environment* ev, ODSession* session)
{
  fSession = session;
  // Cache tokenized focus names
  fKeyFocusToken = fSession->Tokenize(ev, kODKeyFocus);
  fMenuFocusToken = fSession->Tokenize(ev, kODMenuFocus);
  fSelectionFocusToken = fSession->Tokenize(ev, kODSelectionFocus);
  fModalFocusToken = fSession->Tokenize(ev, kODModalFocus);
  fMouseFocusToken = fSession->Tokenize(ev, kODMouseFocus);
  fScrollingFocusToken = fSession->Tokenize(ev, kODScrollingFocus);
  fMouseUpOutsideFocusToken = fSession->Tokenize(ev, kODMouseUpOutsideFocus);

  fLargeIconViewToken = fSession->Tokenize(ev, kODViewAsLargeIcon);
  fSmallIconViewToken = fSession->Tokenize(ev, kODViewAsSmallIcon);
  fThumbnailViewToken = fSession->Tokenize(ev, kODViewAsThumbnail);

  // Cache session globals for easy access

  fArbitrator = fSession->GetArbitrator(ev);
  fWindowState = fSession->GetWindowState(ev);
  fDispatcher = fSession->GetDispatcher(ev);

}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::Dispatch
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::Dispatch(Environment* ev, ODEventData* event,
					ODEventInfo* eventInfo)
{
    ODFacet* facet;
    ODFrame* targetFrame;
    HWND platformWindow;
    // dr3 ODWindow* theWindow;

    POINTL pt;
    ODPoint windowPt;
    ODPart* part;
    HWND hwndFacet;

    switch (event->msg)
    {
      case WM_BUTTON1DOWN       :
      case WM_BUTTON2DOWN       :
      case WM_BUTTON3DOWN       :
      case WM_BUTTON1CLICK      :
      case WM_BUTTON2CLICK      :
      case WM_BUTTON3CLICK      :
      case WM_BUTTON1DBLCLK     :          //137019 st
      case WM_BUTTON2DBLCLK     :
      case WM_BUTTON3DBLCLK     :
      case WM_BUTTON1MOTIONSTART:
      case WM_BUTTON1MOTIONEND  :
      case WM_BUTTON2MOTIONSTART:
      case WM_BUTTON2MOTIONEND  :
      case WM_BUTTON3MOTIONSTART:
      case WM_BUTTON3MOTIONEND  :
      case WM_BUTTON1UP         :
      case WM_BUTTON2UP         :
      case WM_BUTTON3UP         :
      case WM_MOUSEMOVE         :
      case WM_CHORD             :
        return this->DispatchMouseEvent(ev, event, eventInfo );
       // defect 119564 - will now route messages right BLG
      case WM_BEGINDRAG         :
      case WM_CONTEXTMENU       :
      case WM_ENDDRAG           :
      case WM_CONTEXTHELP       :  // I don't have documentation for this one.
      case WM_SINGLESELECT      :
      case WM_BEGINSELECT       :
      case WM_ENDSELECT         :
      case WM_OPEN              :
      case WM_TEXTEDIT          :
        if ((SHORT2FROMMP(event->mp2))&INP_KBD) { // if message resulted from keyboard
           return this->DispatchKeyEvent(ev, event, eventInfo);
        } else {                   // message resulted from mouse event
           return this->DispatchMouseEvent(ev, event, eventInfo);
        }

      case WM_CHAR:
      case kODEvtKeyUp:
      case kODEvtKeyDown:
        return this->DispatchKeyEvent(ev, event, eventInfo);

      case WM_PAINT:
        return this->DispatchUpdateEvent(ev, event);

      case WM_ACTIVATE:
        return this->DispatchActivateEvent(ev, event, eventInfo);

      case WM_INITMENU:
        return this->DispatchMenuEvent(ev, event, eventInfo);
      case OD_HELP:
        return this->DispatchKeyEvent(ev, event, eventInfo);// 127662 the F1 key pressed
   //        return this->DispatchMenuEvent(ev, event, eventInfo);
      case WM_COMMAND:
        if (SHORT1FROMMP(event->mp2) & CMDSRC_MENU)
           return this->DispatchMenuEvent(ev, event, eventInfo);
        else if (SHORT1FROMMP(event->mp2) & CMDSRC_ACCELERATOR)
           return this->DispatchKeyEvent(ev, event, eventInfo);
        break;

      case WM_RENDERFMT:
        return this->DispatchClipboardEvent(ev, event);

      case WM_SIZE:
        return this->HandleResizeEvent(ev, event, eventInfo);

      case WM_MOVE:
        return this->HandleMoveEvent(ev, event, eventInfo);

      case DM_RENDER:
      case DM_DRAGOVER:
      case DM_DRAGLEAVE:
      case DM_DROP:
      case DM_DROPHELP:                     // [138899] - ced
      case DM_RENDERCOMPLETE:
      case DM_ENDCONVERSATION:
        return this->DispatchDragDropEvent(ev, event);
      //  have part recieve drawitem messages
      case WM_DRAWITEM:
      case WM_MENUSELECT:
      case WM_MEASUREITEM:
           return this->DispatchMenuEvent(ev, event, eventInfo);
      case WM_QUERYCONVERTPOS:               // [124535]
      case WM_REALIZEPALETTE:                // [129147] - Sent to the active part
           targetFrame = fArbitrator->AcquireFocusOwner(ev, fSelectionFocusToken);
           if (targetFrame)
           {
              ODBoolean result = this->DispatchTheEvent(ev, event, targetFrame, kODNULL, eventInfo);
              ODReleaseObject(ev, targetFrame);
              return result;
           }
           break;

//    case WM_BUTTON1DBLCLK     :               //128947 ST
//    case WM_BUTTON2DBLCLK     :
//    case WM_BUTTON3DBLCLK     :
//         {
//         platformWindow = WinQueryWindow(event->hwnd, QW_PARENT);
//         if(!platformWindow) break;
//         WinQueryPointerPos(HWND_DESKTOP,&pt);
//         TempODWindow theWindow = fWindowState->AcquireODWindow(ev, platformWindow);
//         if(!theWindow) break;
//         WinMapWindowPoints(HWND_DESKTOP,platformWindow,&pt,1);
//         windowPt = pt;
//         facet = theWindow->GetFacetUnderPoint(ev, &windowPt);
//         if(!facet) break;
//         targetFrame = facet->GetFrame(ev);
//         part = targetFrame->AcquirePart(ev);
//         return part->HandleEvent(ev, event, targetFrame, facet, eventInfo);
//         }
//      break;

      case WM_PRESPARAMCHANGED:              // [129147] - Sent to the facet under the mouse
      {
           platformWindow = WinQueryWindow(event->hwnd, QW_PARENT);
           if(!platformWindow) break;
           WinQueryPointerPos(HWND_DESKTOP,&pt);
           TempODWindow theWindow = fWindowState->AcquireODWindow(ev, platformWindow);
           if(!theWindow) break;
           WinMapWindowPoints(HWND_DESKTOP,platformWindow,&pt,1);
           windowPt = pt;
           facet = theWindow->GetFacetUnderPoint(ev, &windowPt);
           if(!facet) break;
           // start [119934] : handle palette drop on embedded PM controls
           hwndFacet = facet->GetFacetHWND(ev);
           if (hwndFacet && !(facet->GetHandleMouseEvents(ev) & kODPartHandlesMouseEvents))  // [138899] - ced
           {
               POINTL ptl = pt;
               WinMapWindowPoints(platformWindow,hwndFacet,&ptl,1);
               WinSetWindowULong(hwndFacet, QWL_HITTESTFACET, TRUE); // Tell facet window not
                                                 //  to be transparent.
               HWND hwndTarget = WinWindowFromPoint(hwndFacet, &ptl, TRUE);
               WinSetWindowULong(hwndFacet, QWL_HITTESTFACET, FALSE);
               if (hwndTarget && hwndTarget != hwndFacet)
               {
                   char parambuf[CCHMAXPATH];
                   USHORT len;
                   ULONG presId = (ULONG)event->mp1;
                   len = WinQueryPresParam(event->hwnd,
                                           presId,
                                           0,
                                           NULL,
                                           sizeof(parambuf),
                                           (PVOID)parambuf,
                                           0);
                   if(len) {
                        WinSetPresParam(hwndTarget, presId, len, (PVOID)parambuf);
                   }
                   return kODTrue;
               }
           }
           // end [119934]
           targetFrame = facet->GetFrame(ev);
           part = targetFrame->AcquirePart(ev);
           return part->HandleEvent(ev, event, targetFrame, facet, eventInfo);
      }
      case WM_HSCROLL:              // [129403] - Sent to the root part
      case WM_VSCROLL:
      {
           platformWindow = WinQueryWindow(event->hwnd, QW_PARENT);
           if(!platformWindow) break;
           TempODWindow theWindow = fWindowState->AcquireODWindow(ev, platformWindow);
           if(!theWindow) break;
           facet = theWindow->GetRootFacet(ev);
           targetFrame = facet->GetFrame(ev);
           part = targetFrame->AcquirePart(ev);
           return part->HandleEvent(ev, event, targetFrame, facet, eventInfo);
      }
      default:
        return kODFalse;
    }
  return kODFalse;
}
//-------------------------------------------------------------------------------------
// OS2DispatchModule::HandleResizeEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::HandleResizeEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
  ODBoolean handled = kODFalse;
  ODPlatformWindow platformWindow = WinQueryWindow(theEvent->hwnd, QW_PARENT);
  // ODWindow *aWindow = kODNULL;

  ODWindowState* fWindowState = fSession->GetWindowState(ev);
  if (platformWindow && fWindowState->IsODWindow(ev, platformWindow)) {
     TempODWindow aWindow = fWindowState->AcquireODWindow(ev, platformWindow);
     aWindow->AdjustWindowShape(ev);
     handled = kODTrue;
  }
  return handled;
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::HandleMoveEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::HandleMoveEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
  ODBoolean handled = kODFalse;
  ODPlatformWindow platformWindow = WinQueryWindow(theEvent->hwnd, QW_PARENT);

  ODWindowState* fWindowState = fSession->GetWindowState(ev);
  if (platformWindow && fWindowState->IsODWindow(ev, platformWindow))
  {
     TempODWindow aWindow = fWindowState->AcquireODWindow(ev, platformWindow);
     if (aWindow->GetRootFacet(ev))
     {
         ODDraft *draft = aWindow->GetDraft(ev);
         if (draft->GetPermissions(ev) >= kODDPSharedWrite)
         {
             draft->SetChangedFromPrev(ev);
         }
         handled = kODTrue;
     }
  }
  return handled;
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchClipboardEvent
//
// Description
//      created to help support delayed rendering (WM_RENDERFMT)
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::DispatchClipboardEvent (Environment* ev, ODEventData* theEvent)
{
  ODBoolean handled = kODFalse;
  switch (theEvent->msg) {
     case WM_RENDERFMT:
        fSession->GetClipboard(ev)->DispatchHandler( ev, theEvent );
        handled = kODTrue;
        break;
  }
  return handled;
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchDragDropEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::DispatchDragDropEvent (Environment* ev, ODEventData* theEvent)
{
#if 0   //crs-11/21/94
  ODBoolean handled = kODFalse;
  switch (theEvent->msg) {
  case DM_ENDCONVERSATION:
  case DM_RENDER:
     fSession->GetDragAndDrop(ev)->SendDataHandler( ev, theEvent );
     handled = kODTrue;  // handled
     break;

  case DM_DRAGOVER:
  case DM_DRAGLEAVE:
     //if (theEvent->msg == DM_DRAGOVER) {
     //   theEvent->where.x = SHORT1FROMMP(theEvent->mp2);
     //   theEvent->where.y = SHORT2FROMMP(theEvent->mp2);
     //} /* endif */
     fSession->GetDragAndDrop(ev)->DragTrackingHandler( ev, theEvent );
     handled = kODTrue;  // handled
     break;

  case DM_RENDERCOMPLETE:
  case DM_DROP:
     //  need to uncomment this and get MyReceiveDropHandler
     //    to accept a reasonable set of parameters.
     BOOL      s;
     //PDRAGINFO pDInfo;

     //pDInfo = (PDRAGINFO)(theEvent->mp1);
     //theEvent->where.x = pDInfo->xDrop;
     //theEvent->where.y = pDInfo->yDrop;
     fSession->GetDragAndDrop(ev)->ReceiveDropHandler( ev, theEvent );
     theEvent->result = 0;  // reserved value
     handled = kODTrue;          // handled
     break;
  }
  return handled;
#else
  ODBoolean handled = kODFalse;
// [138899] begin - ced
//switch (theEvent->msg) {
//case DM_ENDCONVERSATION:
//case DM_RENDER:
//case DM_DRAGOVER:
//case DM_DRAGLEAVE:
//case DM_RENDERCOMPLETE:
//case DM_DROP:
  ODFacet* targetfacet = kODNULL;
  ODPoint      localMouse;
  POINTL       mouse;

  switch (theEvent->msg) {
  case DM_DRAGOVER:
  {
     HWND         hwndTarget = -1;
     mouse.x = SHORT1FROMMP(theEvent->mp2);
     mouse.y = SHORT2FROMMP(theEvent->mp2);
     targetfacet = fSession->GetDragAndDrop(ev)->FindTargetFacet(ev,
                                     WinQueryWindow(theEvent->hwnd, QW_PARENT),
                                     &mouse, &localMouse);

     if (targetfacet)
     {
        HWND hwndFacet = targetfacet->GetFacetHWND(ev);
        if (hwndFacet && !(targetfacet->GetHandleMouseEvents(ev) & kODPartHandlesDragDropEvents))
        {
          WinMapWindowPoints(HWND_DESKTOP, hwndFacet, &mouse, 1);
          WinSetWindowULong(hwndFacet, QWL_HITTESTFACET, TRUE); // Tell facet window not
                                                 //  to be transparent.
          hwndTarget = WinWindowFromPoint(hwndFacet, &mouse, TRUE);
          WinSetWindowULong(hwndFacet, QWL_HITTESTFACET, FALSE);

          if (hwndTarget == hwndFacet)
             hwndTarget = -1;    // dragging over part
        }
     }

     if (fPrevDragTarget && fPrevDragTarget != hwndTarget)
     {
        if (fPrevDragTarget != -1)
        {
           WinSendMsg(fPrevDragTarget, DM_DRAGLEAVE, theEvent->mp1, 0);
           fNeverDropTarget = kODFalse;
        }
        else
        {
           ODEventData event;
           event.msg = DM_DRAGLEAVE;
           event.mp1 = theEvent->mp1;
           event.mp2 = 0;
           event.hwnd = theEvent->hwnd;
           event.result = 0;
           fSession->GetDragAndDrop(ev)->DispatchHandler( ev, &event, kODNULL );
        }
     }

     if (hwndTarget && hwndTarget != -1)
     {
        if (!fNeverDropTarget) {
           theEvent->result = WinSendMsg(hwndTarget, theEvent->msg,
                                         theEvent->mp1, theEvent->mp2);
           if (SHORT1FROMMP(theEvent->result) == DOR_NEVERDROP) {
              theEvent->result = (MRESULT)DOR_NODROP;
              fNeverDropTarget = kODTrue;
           }
        } else {
           theEvent->result = (MRESULT)DOR_NODROP;
        }
     }
     else
     {
        fSession->GetDragAndDrop(ev)->DispatchHandler( ev, theEvent, targetfacet );
     }

     fPrevDragTarget = hwndTarget;
     handled = kODTrue;          // handled
     break;
  }
  case DM_DRAGLEAVE:
  case DM_DROP:
  case DM_DROPHELP:
     if (fPrevDragTarget != -1)
     {
        theEvent->result = WinSendMsg(fPrevDragTarget, theEvent->msg,
                                      theEvent->mp1, theEvent->mp2);
     }
     else
     {
        fSession->GetDragAndDrop(ev)->DispatchHandler( ev, theEvent, kODNULL );
     }
     if (theEvent->msg == DM_DRAGLEAVE || theEvent->msg == DM_DROP)
     {
        fPrevDragTarget = 0;
        fNeverDropTarget = kODFalse;
     }
     handled = kODTrue;          // handled
     break;


  case DM_ENDCONVERSATION:
  case DM_RENDER:
  case DM_RENDERCOMPLETE:
     fSession->GetDragAndDrop(ev)->DispatchHandler( ev, theEvent, kODNULL );
     handled = kODTrue;          // handled
     break;
  }
// [138899] end - ced
  return handled;
#endif
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchMenuEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::DispatchMenuEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
  ODBoolean handled = kODFalse;

//changed for new menu stuff @ST 119388

    ODMenuBar * mb = kODNULL;
    ODWindowState* fWindowState = fSession->GetWindowState(ev);

       mb = fWindowState->AcquireCurrentMenuBar(ev );
       TempODFrame targetFrame = fArbitrator->AcquireFocusOwner(ev, fMenuFocusToken);

       if (targetFrame)
       {
         handled = this->DispatchTheEvent(ev, theEvent, targetFrame, kODNULL, eventInfo);
       }

  return handled;

}


  static ODBoolean IsContained(Environment* ev, ODFrame* frame, ODFrame* container)
  {
    if ( (frame == kODNULL) || (container == kODNULL))
      return kODFalse;
    if (ODObjectsAreEqual(ev, frame, container))
      return kODTrue;

    ODFrame* tempFrame = frame;
    ODFrame* tempContainer = kODNULL;
    tempFrame->Acquire(ev);  // -- T‚ added
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
    ODTransform* winToFrame = kODNULL;
    ODPoint framePoint;

    if ( frame && window )
    {
      ODFrameFacetIterator* iter = frame->CreateFacetIterator(ev);

      for (ODFacet* facet = iter->First(ev);
          iter->IsNotComplete(ev);
          facet = iter->Next(ev))
      {
        // get windowPoint in frame coords for hit-testing
        winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
#ifdef _PLATFORM_OS2_  // [137664]
        framePoint = windowPoint;
        winToFrame->InvertPoint(ev, &framePoint);
#else
        framePoint = winToFrame->InvertPoint(ev, &windowPoint);
#endif
        ODReleaseObject(ev, winToFrame);
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
#ifndef _PLATFORM_OS2_
  static ODFacet* GetActiveFacetWithBorderUnderPoint(Environment* ev, ODFrame* frame, ODWindow* window, ODPoint& windowPoint)
  {
    ODFacet* foundFacet = kODNULL;

    if ( frame && window )
    {
      ODFrameFacetIterator* facets = frame->CreateFacetIterator(ev);
      for ( ODFacet* facet = facets->First(ev);
          facets->IsNotComplete(ev);
          facet = facets->Next(ev) )
      {
        if ( (facet->GetWindow(ev) == window) && (facet->ActiveBorderContainsPoint(ev, &windowPoint, kODNULL)) )
        {
          foundFacet = facet;
          break;
        }
      }
      delete facets;
    }
    return foundFacet;
  }
#endif

  static ODFacet* GetFirstFacetUnderPoint(Environment* ev, ODWindow* window, ODFacet* root, ODPoint& windowPoint)
  {
  // returns NULL, root or an embedded facet of root

    ODFacet* foundFacet = kODNULL;
    ODTransform* winToFrame = kODNULL;
    ODPoint framePoint;

    if (window && root && (ODObjectsAreEqual(ev, root->GetWindow(ev),window)))
    {
      ODFacetIterator* t = root->CreateFacetIterator(ev, kODTopDown, kODFrontToBack);

      ODFacet* facet = t->First(ev);
      // get windowPoint in frame coords for hit-testing
      winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
#ifdef _PLATFORM_OS2_ // [137664]
      framePoint = windowPoint;
      winToFrame->InvertPoint(ev, &framePoint);
#else
      framePoint = winToFrame->InvertPoint(ev, &windowPoint);
#endif
      ODReleaseObject(ev, winToFrame);

      if ( !facet->ContainsPoint(ev, &framePoint, kODNULL) )
        return kODNULL;
      for ( facet = t->Next(ev);
          t->IsNotComplete(ev);
          facet = t->Next(ev) )
      {
        // get windowPoint in frame coords for hit-testing
        winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
#ifdef _PLATFORM_OS2_      // [137664]
        framePoint = windowPoint;
        winToFrame->InvertPoint(ev, &framePoint);
#else
        framePoint = winToFrame->InvertPoint(ev, &windowPoint);
#endif
        ODReleaseObject(ev, winToFrame);
        if (facet->ContainsPoint(ev, &framePoint, kODNULL))
        {
          foundFacet = facet;
          break;
        }
        else
          t->SkipChildren(ev);
      }
      delete t;
    }
    if ( foundFacet )
      return foundFacet;
    else
      return root;
  }

ODBoolean OS2DispatchModule::DispatchToFacetUnderPoint(Environment* ev, ODWindow* window, ODPoint& windowPt,
                                                       ODEventData* theEvent, ODEventInfo* eventInfo)
{
  ODBoolean handled = kODFalse;
  ODFacet* containingFacet = kODNULL;
  ODFacet* facet = window->GetFacetUnderPoint(ev, &windowPt);
  ODFrame* frame = facet ? facet->GetFrame(ev) : kODNULL;

  if (facet && frame)
  {
    ODFrame* modalFocus = fArbitrator->AcquireFocusOwner(ev, fModalFocusToken);
    if (modalFocus && !IsContained(ev, frame, modalFocus))
    {
      TempODPart part = modalFocus->AcquirePart(ev); // -- T‚ tempobj'd
      if ((ODPart*)part)
        handled = part->HandleEvent(ev, theEvent, modalFocus, kODNULL, eventInfo);
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
      if ( isFrozen
        || isSelected && (theEvent->msg != WM_BUTTON1CLICK)  // [123149] ced
        || (isIcon && theEvent->msg != WM_BUTTON1DBLCLK)      //137091 st
        || isDragging)
      {
	theEvent->msg = ChangeEventTypeToEmbedded(theEvent->msg);
        containingFacet = facet->GetContainingFacet(ev);
        eventInfo->flags = kODInEmbedded;
        eventInfo->embeddedFrame = frame;
        eventInfo->embeddedFacet = facet;
        handled = this->DispatchTheEvent(ev, theEvent, containingFacet ? containingFacet->GetFrame(ev) : kODNULL,
                                         containingFacet, eventInfo);
      }
      else
      {
        // Send the event to the facet under the point, or, if there is a
        // non-facet window under the point that is a child of facet, then
        // send the event directly to that window via WinSendMsg.
        //
        // Note:WinWindowFromPoint will never return a facet window handle
        // because the facet window proc always returns HT_TRANSPARENT
        // for WM_HITTEST messages (unless, of course the wnd proc is subclassed).

        HWND hwndFacet = facet->GetFacetHWND(ev);
        if (hwndFacet && !(facet->GetHandleMouseEvents(ev) & kODPartHandlesMouseEvents))  // [138899] - ced
        {
          POINTL ptl = windowPt.AsPOINTL();
          WinMapWindowPoints(WinWindowFromID(window->GetPlatformWindow(ev), FID_CLIENT),
                             hwndFacet, &ptl, 1);
          WinSetWindowULong(hwndFacet, QWL_HITTESTFACET, TRUE); // Tell facet window not
                                                 //  to be transparent.
          HWND hwndTarget = WinWindowFromPoint(hwndFacet, &ptl, TRUE);
          WinSetWindowULong(hwndFacet, QWL_HITTESTFACET, FALSE);

          if (hwndTarget && hwndTarget != hwndFacet)
          {
            WinMapWindowPoints(hwndFacet, hwndTarget, &ptl, 1);
            theEvent->mp1 = MPFROM2SHORT(ptl.x, ptl.y);
            theEvent->result = WinSendMsg(hwndTarget, theEvent->msg,
                                          theEvent->mp1, theEvent->mp2);
            handled = kODTrue;
          }
        }
        if (!handled)
          handled = this->DispatchTheEvent(ev, theEvent, frame, facet, eventInfo);
      }
      ODReleaseObject(ev,modalFocus);
    }
  }
  return handled;
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchMouseEvent
//
// Description:
//-------------------------------------------------------------------------------------
ODBoolean OS2DispatchModule::DispatchMouseEvent(Environment* ev,
                                                ODEventData* theEvent, ODEventInfo* eventInfo)
{
  HWND platformWindow = WinQueryWindow(theEvent->hwnd, QW_PARENT);
  // ODWindow* theWindow = fWindowState->AcquireODWindow(ev, platformWindow);
  TempODWindow theWindow = fWindowState->AcquireODWindow(ev, platformWindow);
  ODFacet* containingFacet = kODNULL;
  ODFrame* activeFrame = kODNULL;
  ODFacet* activeFacet = kODNULL;
  ODBoolean modified = kODNULL;

  ODPoint windowPt;

  windowPt.x = ODIntToFixed(SHORT1FROMMP(theEvent->mp1));
  windowPt.y = ODIntToFixed(SHORT2FROMMP(theEvent->mp1));

  eventInfo->where.x = windowPt.x;
  eventInfo->where.y = windowPt.y;

  // if event msg is WM_BUTTONnMOTIONEND or event type
  // is WM_BUTTONnUP; and event hwnd != hwnd under mouse;
  // then give only to owner of kODMouseUpOutsideFocus
  // ---------------------------------------------------
  if (	(theEvent->msg == WM_BUTTON1MOTIONEND)	||
	(theEvent->msg == WM_BUTTON2MOTIONEND)	||
	(theEvent->msg == WM_BUTTON3MOTIONEND)	||
	(theEvent->msg == WM_BUTTON1UP)	||
	(theEvent->msg == WM_BUTTON2UP)	||
	(theEvent->msg == WM_BUTTON3UP)  )
  {
	POINTL mousePt;

	WinQueryPointerPos(HWND_DESKTOP, &mousePt);

	if (platformWindow != WinWindowFromPoint(HWND_DESKTOP, &mousePt, FALSE))
	{
	    TempODFrame mouseUpOutFrame = fArbitrator->AcquireFocusOwner(ev, fMouseUpOutsideFocusToken);
	    if ((ODFrame*)mouseUpOutFrame)
	    {
		eventInfo->where.x = 0;
		eventInfo->where.y = 0;

		TempODPart part = mouseUpOutFrame->AcquirePart(ev);

		if ((ODPart*)part)
		    return part->HandleEvent(ev, theEvent, mouseUpOutFrame, kODNULL, eventInfo);
	    }

	    return kODTrue;
	}
  }


  TempODFrame mouseFrame = fArbitrator->AcquireFocusOwner(ev, fMouseFocusToken); // -- T‚ tempobj'd
  if ((ODFrame*)mouseFrame)
  {
    TempODPart part = mouseFrame->AcquirePart(ev); // -- T‚ tempobj'd
    ODFacet* facet = theWindow->GetFacetUnderPoint(ev, &windowPt);

    // make sure facet goes with frame
    // -------------------------------
    if ((facet) && (facet->GetFrame(ev) != mouseFrame))
	facet = kODNULL;

    if ((ODPart*)part)
      return part->HandleEvent(ev, theEvent, mouseFrame, facet, eventInfo);
    else
      fArbitrator->RelinquishFocus(ev, fMouseFocusToken, mouseFrame);
  }

  switch (theEvent->msg)
  {
    case WM_BUTTON1CLICK:           // Mouse events that can be modified by keyboard
    case WM_BUTTON2CLICK:
    case WM_BUTTON3CLICK:
    case WM_BUTTON1DBLCLK:          //137019 st
    case WM_BUTTON2DBLCLK:
    case WM_BUTTON3DBLCLK:
    case WM_BUTTON1DOWN:
    case WM_BUTTON2DOWN:
    case WM_BUTTON3DOWN:
    case WM_BUTTON1UP:
    case WM_BUTTON2UP:
    case WM_BUTTON3UP:

      if (theEvent->msg == WM_BUTTON1DOWN || theEvent->msg == WM_BUTTON2DOWN || theEvent->msg == WM_BUTTON3DOWN){
        for (HWND hwnd = WinQueryFocus(HWND_DESKTOP); hwnd; hwnd = WinQueryWindow(hwnd, QW_PARENT))
           if (hwnd == theEvent->hwnd) break;

        if (!hwnd)
          WinSetFocus(HWND_DESKTOP, theEvent->hwnd);
      }
      modified = (SHORT2FROMMP(theEvent->mp2) | KC_NONE) ? kODTrue : kODFalse;

      activeFrame = fArbitrator->AcquireFocusOwner(ev, fSelectionFocusToken);
      if (activeFrame )
      {
        activeFacet = GetActiveFacetWithBorderUnderPoint(ev, activeFrame, theWindow, windowPt);
        if ( activeFacet )
        {
          if (modified)
          {
            // Shift-click or Command-click in border
            switch (theEvent->msg)
            {
              case WM_BUTTON1DOWN:
              case WM_BUTTON2DOWN:
              case WM_BUTTON3DOWN:
                WinAlarm(HWND_DESKTOP, WA_WARNING);
            }
            return kODTrue;
          }
          else
            containingFacet = activeFacet->GetContainingFacet(ev);
        }
      }
      break;

    default:
      break;
  }

  if (theWindow)
  {
    ODBoolean handled = kODFalse;
    if ( activeFacet && containingFacet )
    {
      eventInfo->flags = kODInBorder;
      eventInfo->embeddedFrame = activeFrame;
      eventInfo->embeddedFacet = activeFacet;

      // Don't offer mouse-move to part; it was already offered to
      // the mouse focus owner, above.

      if (theEvent->msg != WM_MOUSEMOVE)
      {
          // If not a button up event - it's a button down or click or double click.
          if ((theEvent->msg != WM_BUTTON1UP) &&
              (theEvent->msg != WM_BUTTON2UP) &&
              (theEvent->msg != WM_BUTTON3UP))
          {
              // Change event type to MouseDownBorder
              theEvent->msg = kODEvtMouseDownBorder;
          }

          handled = this->DispatchTheEvent(ev, theEvent,
					containingFacet->GetFrame(ev),
					containingFacet, eventInfo);
      }
      else
      // set the mouse pointer look -  for Over active Border defect 110736
      // defect 120683 - Improve mouse over active border code as below
      // only load Module first time - performance. Also give user chance
      // to set mouse pointer if desires if not set to default - usablity.
      {
          CHAR filler[100];
          HPOINTER hptrOverBorder;
          static HMODULE  hmodHandle;
          if (!hmodHandle) {
             APIRET rc = DosLoadModule(filler, sizeof(filler),
                "ODRes", &hmodHandle);
          } /* endif */
          hptrOverBorder = WinLoadPointer( HWND_DESKTOP,hmodHandle, ID_BORDER_PTR);
          handled =  WinSetPointer(HWND_DESKTOP,hptrOverBorder);
       } /* endif */
       // end of mouse pointer over active border
    }
    else if (theEvent->msg != WM_MOUSEMOVE)  // click, not in border
    {
      if (modified)
      {
        ODFacet* rootFacet = activeFrame ? GetActiveFacet(ev, activeFrame, theWindow, windowPt)
          : (ODFacet*) kODNULL;
        ODFacet* facet = GetFirstFacetUnderPoint(ev, theWindow, rootFacet, windowPt);
        // returns NULL, rootFacet or embeddedFrame

        if (facet)
        {
          if (facet == rootFacet)
          {
            handled = this->DispatchTheEvent(ev, theEvent, activeFrame, rootFacet, eventInfo);
          }
          else // frame is embedded in active frame
          {
	    theEvent->msg = ChangeEventTypeToEmbedded(theEvent->msg);
            eventInfo->flags = kODInEmbedded;
            eventInfo->embeddedFrame = facet->GetFrame(ev);
            eventInfo->embeddedFacet = facet;
            handled = this->DispatchTheEvent(ev, theEvent, activeFrame, rootFacet, eventInfo);
          }
        }
        else // Modified click outside active frame
        {
          WinAlarm(HWND_DESKTOP, WA_WARNING);
        }
      }
      else // No modifiers
      {
        handled = DispatchToFacetUnderPoint(ev, theWindow, windowPt, theEvent, eventInfo);
      }
    }
    if(activeFrame)
       ODReleaseObject(ev, activeFrame);
    return handled;
  }
  if(activeFrame)
     ODReleaseObject(ev, activeFrame);
  return kODFalse;
}

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchKeyEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::DispatchKeyEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)
{
  ODBoolean handled = kODFalse;


  ODFrame* keyFrame = fArbitrator->AcquireFocusOwner(ev, fKeyFocusToken);

  if (theEvent->msg == WM_CHAR)
  {
     ODFrame* scrollingFrame = fArbitrator->AcquireFocusOwner(ev, fScrollingFocusToken);
     char key = (char) (SHORT2FROMMP(theEvent->mp2));

     if (scrollingFrame &&
        ((key == VK_PAGEUP || (key == VK_PAGEDOWN) || (key == VK_HOME) || (key == VK_END))))
     {
        handled = this->DispatchTheEvent(ev, theEvent, scrollingFrame, kODNULL, eventInfo);
     }
  ODReleaseObject(ev, scrollingFrame);
  }
  if (!handled)
     handled = this->DispatchTheEvent(ev, theEvent, keyFrame, kODNULL, eventInfo);

  // if not handled as WM_CHAR, see if the part was expecting
  // a synthetic 'kODEvtKeyUp' or 'kODEvtKeyDown' event.

  if ((!handled) && (theEvent->msg == WM_CHAR))
  {
	if (CHARMSG(theEvent)->fs & KC_KEYUP)
		theEvent->msg = kODEvtKeyUp;
	else
		theEvent->msg = kODEvtKeyDown;

     handled = this->DispatchTheEvent(ev, theEvent, keyFrame, kODNULL, eventInfo);
  }

  ODReleaseObject(ev, keyFrame);

  return handled;
}  //  OS2DispatchModule::DispatchKeyEvent

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchUpdateEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::DispatchUpdateEvent (Environment* ev, ODEventData* theEvent)
{
  ODBoolean handled = kODFalse;

  WindowPtr platformWindow = WinQueryWindow(theEvent->hwnd, QW_PARENT);
//   ODWindow* aWindow = kODNULL;
  TempODWindow aWindow = kODNULL;

  if (platformWindow && fWindowState->IsODWindow(ev, platformWindow))
     aWindow = fWindowState->AcquireODWindow(ev, platformWindow);

  if ( aWindow )
  {
     HPS hps = WinGetPS(theEvent->hwnd);
    // HRGN rgn = GpiCreateRegion(hps, 0, NULL);
    // LONG complexity = WinQueryUpdateRegion(theEvent->hwnd, rgn);

    // Query the rectangle to update, rather than just the region. Grow
    // the update rectangle by 1 pixel on all sides to offset any rounding
    // which might have occurred in the calculations from INT to FIXED.
     RECTL urectl;
     WinQueryUpdateRect(theEvent->hwnd, &urectl);
     urectl.xLeft   += -1;
     urectl.yBottom += -1;
     urectl.xRight  += 1;
     urectl.yTop    += 1;
     HRGN rgn = GpiCreateRegion(hps, 1L, &urectl);

     WinReleasePS(hps);

     theEvent->mp2 = (MPARAM) rgn;
     WinEndPaint(WinBeginPaint(theEvent->hwnd, 0, 0));

     aWindow->HandleUpdateEvent(ev, theEvent);
     handled = kODTrue;

     hps = WinGetPS(theEvent->hwnd);

     // Note that the region does not need to be destroyed.  It has already
     // been destroyed in ODWindow::HandleUpdateEvent when the temporary shape
     // containing the region goes out of scope and is deleted.  Leaving this
     // call here will always return fail and is unnecessary and misleading.
     // GpiDestroyRegion(hps, rgn);

     WinReleasePS(hps);
  }
  return handled;
}  //  OS2DispatchModule::DispatchUpdateEvent

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchActivateEvent
//
// Description
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::DispatchActivateEvent  (Environment* ev,
                                                     ODEventData* theEvent,
                                                     ODEventInfo* eventInfo)
{
  ODBoolean handled = kODFalse;

  WindowPtr platformWindow = WinQueryWindow(theEvent->hwnd, QW_PARENT);
  // ODWindow* aWindow = kODNULL;
  TempODWindow aWindow = kODNULL;

  if (platformWindow)
    aWindow = fWindowState->AcquireODWindow(ev, platformWindow);

  if (aWindow )
  {
    aWindow->HandleActivateEvent(ev, theEvent, eventInfo);
    handled = kODTrue;
  }
  return handled;
}  //  OS2DispatchModule::DispatchActivateEvent

//-------------------------------------------------------------------------------------
// OS2DispatchModule::DispatchTheEvent
//
// Dispatches the event to the initial target, or to a containing frame,
// if the initial target doesn't handle it, and the containing part has
// set the "DoesPropagateEvents" flag of the embedded frame
//-------------------------------------------------------------------------------------

ODBoolean OS2DispatchModule::DispatchTheEvent(Environment* ev,
              ODEventData* theEvent,
              ODFrame* targetFrame,
              ODFacet* targetFacet,
              ODEventInfo* eventInfo)
{
  ODBoolean handled = kODFalse;
  TempODPart targetPart = targetFrame ? targetFrame->AcquirePart(ev) : kODNULL; // -- T‚ tempobj'd

  // make sure facet goes with frame
  // -------------------------------
  if ( (targetFacet) && (targetFacet->GetFrame(ev) != targetFrame) )
	targetFacet = kODNULL;

  if ((ODPart*)targetPart)
  {
#if 0
    // ifdef'd to leave eventInfo->where in global coordinates for consistency
    if (eventInfo && eventInfo->embeddedFrame)
    {
      //handled = targetPart->HandleEventInEmbedded(ev, theEvent, targetFrame, targetFacet, embeddedFrame, embeddedFacet);
      if (targetFacet)
                                WindowToLocal(ev, eventInfo->where, targetFacet);

      handled = targetPart->HandleEvent(ev, theEvent, targetFrame, targetFacet, eventInfo);
    }
    else
#endif
      handled = targetPart->HandleEvent(ev, theEvent, targetFrame, targetFacet, eventInfo);
    if (!handled)
    {
      handled = PropagateTheEvent(ev, theEvent,targetFrame, targetFacet, eventInfo);
    }
  }
  return handled;
}

ODBoolean OS2DispatchModule::PropagateTheEvent(Environment* ev,
              ODEventData* theEvent,
              ODFrame* initialFrame,
              ODFacet* initialFacet,
              ODEventInfo* eventInfo)
{
  ODFacet* targetFacet = initialFacet;
  TempODFrame targetFrame = initialFacet ? initialFacet->GetFrame(ev) : initialFrame; // DMc refcount - temp
  ODBoolean handled = kODFalse;

  targetFrame->Acquire(ev);  // -- T‚ added
  while (targetFrame)
  {
    if (handled  || (!targetFrame->DoesPropagateEvents(ev)))
      break;

    if (targetFacet)
    {
      targetFacet = targetFacet->GetContainingFacet(ev);
      ODReleaseObject(ev, targetFrame);                // -- ced added
      targetFrame = targetFacet->GetFrame(ev);
      targetFrame->Acquire(ev);                        // -- ced added
    }
    else
    {
      ODFrame* contFrame = targetFrame->AcquireContainingFrame(ev);   // -- T‚ added
      ODReleaseObject(ev, targetFrame);                // -- T‚ added
      targetFrame = contFrame;

    }

    TempODPart targetPart = targetFrame->AcquirePart(ev);
    if ((ODPart*)targetPart)
    {
      eventInfo->flags = kODPropagated;
      handled = targetPart->HandleEvent(ev, theEvent, targetFrame, targetFacet, eventInfo);
    }
  }

  return handled;
}

ODEventType ChangeEventTypeToEmbedded(ODEventType evtType)
{
   switch (evtType) {
      case WM_BUTTON1DOWN       :
      case WM_BUTTON2DOWN       :
      case WM_BUTTON3DOWN       :
      case WM_BUTTON1CLICK      :
      case WM_BUTTON2CLICK      :
      case WM_BUTTON3CLICK      :
      case WM_BUTTON1DBLCLK     :
      case WM_BUTTON2DBLCLK     :
      case WM_BUTTON3DBLCLK     :
      case WM_BUTTON1MOTIONSTART:
      case WM_BUTTON2MOTIONSTART:
      case WM_BUTTON3MOTIONSTART:
      case WM_BEGINDRAG		:
		return kODEvtMouseDownEmbedded;
		break;

      case WM_BUTTON1MOTIONEND  :
      case WM_BUTTON2MOTIONEND  :
      case WM_BUTTON3MOTIONEND  :
      case WM_BUTTON1UP         :
      case WM_BUTTON2UP         :
      case WM_BUTTON3UP         :
      case WM_ENDDRAG		:
		return kODEvtMouseUpEmbedded;
		break;

      default:  break;
   }

   return evtType;
}
