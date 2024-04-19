/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES:   OS2DispatchModule
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */
/* @(#) 1.9 com/src/ui/StdDispO.h, odui, od96os2, odos29646d 10/31/96 12:45:51 [11/15/96 15:49:42] */

/********************************************************************/
/*                                                                  */
/*                                                                  */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/*  IBM Change History (most recent first):                         */
/*  138899 11/12/95 ced Add support for embedded PM controls        */
/*                      handling drag/drop events.                  */
/*  133968 08/23/95 aml Port DR3                                    */
/*         08/16/94 Ced Port from Apple code (StdDispM.h)           */
/*                                                                  */
/********************************************************************/
/*
  File:    StdDispO.h

  Contains:  Helper object for standard dispatch module for OS2

  Written by:  Chuck Dumont

  Copyright:  © 1993 - 1995 by IBM, all rights reserved.

  Change History (most recent first):

     <8>   6/15/95  RR    #1254828/1255536 Adding Redispatching
     <7>   5/10/95  RR    # 1242893, 1245692 Added fields to support
                  Mouse up dispatching
     <6>    5/2/95  RR    # 1226050 Added fLastBorderFrame.
     <5>   1/13/95  RR    Added eventInfo to event handling methods
     <4>  10/27/94  RR    Added tokenized focus fields
     <3>   9/30/94  RR    #1171182 Treat icons as bundled. Added
                  fViewAsFrameToken
     <2>    9/6/94  RR    Added utility methods for event propagation
     <1>   6/24/94  RR    first checked in
    <14>   3/15/94  MB    Changes to support SCpp/ASLM builds,
                  #1150864.
    <13>   2/16/94  JA    Stop including Point.h.
    <12>    2/9/94  NP    Tiger Team cleanup.
    <11>   1/21/94  RR    Restored Dispatch() signature
    <10>   1/18/94  RR    Add fScrollingFocus. Added DispatchTheEvent
                  to propagate to container where appropriate
     <9>   1/15/94  RR    WinState.h->WinStat.h,
                  Dispatch.h->Disptch.h
     <8>   1/11/94  T‚    Init... changes
     <7>  12/20/93  RR    New init strategy
     <6>  12/16/93  RR    DispatchToFacetUnderPoint became a member
                  function
     <5>   12/2/93  RR    Use new eventType definition
     <4>  11/19/93  RR    Added DispatchWindowEvent
     <3>   8/13/93  CG    Added ASLM stuff.
     <2>   8/11/93  RR    Added fields to cache handy references to
                  session globals
     <1>   8/10/93  RCR    first checked in

  To Do:
*/

#ifndef _STDDISPO_
#define _STDDISPO_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class OS2DispatchModule;

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class ODWindow;
class ODWindowState;
class ODArbitrator;
class ODDispatcher;
class ODSession;
class ODFrame;
class ODFacet;

//=====================================================================================
// Global Variables
//=====================================================================================

//=====================================================================================
// OS2DispatchModule
//=====================================================================================

class OS2DispatchModule
{
public:

  OS2DispatchModule();

  ~OS2DispatchModule();

  ODNVMethod void InitOS2DispatchModule(Environment* ev, ODSession* session);

  ODNVMethod ODBoolean Dispatch(Environment* ev, ODEventData* event,
				ODEventInfo* eventInfo);

protected:

  ODNVMethod ODBoolean HandleResizeEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo);
  ODNVMethod ODBoolean HandleMoveEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo);
  ODNVMethod ODBoolean DispatchMenuEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo);
  ODNVMethod ODBoolean DispatchMouseEvent(Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo);
  ODNVMethod ODBoolean DispatchKeyEvent(Environment* ev, ODEventData* event, ODEventInfo* eventInfo);

  ODNVMethod ODBoolean DispatchUpdateEvent(Environment* ev, ODEventData* event);
  ODNVMethod ODBoolean DispatchActivateEvent(Environment* ev, ODEventData* event, ODEventInfo* eventInfo);

  ODNVMethod ODBoolean DispatchToFacetUnderPoint(Environment* ev, ODWindow* window, ODPoint& windowPt,
                                                 ODEventData* theEvent, ODEventInfo* eventInfo);

  ODNVMethod ODBoolean DispatchClipboardEvent(Environment*ev, ODEventData* event);

  ODNVMethod ODBoolean DispatchDragDropEvent(Environment*ev, ODEventData* event);

  ODNVMethod ODBoolean DispatchTheEvent(Environment* ev,
              ODEventData* theEvent,
              ODFrame* targetFrame,
              ODFacet* targetFacet,
              ODEventInfo* eventInfo);

  ODNVMethod ODBoolean PropagateTheEvent(Environment* ev,
              ODEventData* theEvent,
              ODFrame* initialFrame,
              ODFacet* initialFacet,
              ODEventInfo* eventInfo);

protected:

  // Caches for frequently accessed items
  ODSession*     fSession;
  ODArbitrator*   fArbitrator;
  ODWindowState* fWindowState;
  ODDispatcher*   fDispatcher;

  ODTypeToken fKeyFocusToken;
  ODTypeToken fMenuFocusToken;
  ODTypeToken fSelectionFocusToken;
  ODTypeToken fModalFocusToken;
  ODTypeToken fMouseFocusToken;
  ODTypeToken fMouseUpOutsideFocusToken;
  ODTypeToken fScrollingFocusToken;
  ODTypeToken fLargeIconViewToken;
  ODTypeToken fSmallIconViewToken;
  ODTypeToken fThumbnailViewToken;

  ODFacet* fMouseDownFacet; // The facet to which the last mouse down in border was sent. Used for the mouse up
  ODFacet* fEmbeddedFacet;  // The embedded facet fpr a kODEvtMouseDownEmbedded. Used for the mouse up
  ODBoolean fInvalidClick;  // Mouse down or up outside active border

  HWND fPrevDragTarget;       // [138899] HWND of window last dragover message was sent to, -1 if last dragover
                              // message was sent to drag manager, 0 if no prev outstanding dragover message.
  ODBoolean fNeverDropTarget; // [138899] set if fPrevDragTarget window return DOR_NEVERDROP.
};
#endif // _STDDISPO_
