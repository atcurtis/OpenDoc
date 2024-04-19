/* @(#)Z 1.4 com/src/ui/StdDispM.h, odui, od96os2, odos29646d 96/11/15 15:28:19 (96/07/15 18:27:25) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES:   MacDispatchModule
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
#ifdef _PLATFORM_MACINTOSH_
/*
	File:		StdDispM.h

	Contains:	Helper object for standard dispatch module for Macintosh

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<10>	 9/21/95	RR		#1286167 Suppress mouse up after bg
									mousedown. Renamed fInvalidClick to
									fSuppressMouseUp
		 <9>	 8/29/95	RR		#1276401 Added tokenized mousr focus
		 <8>	 6/15/95	RR		#1254828/1255536 Adding Redispatching
		 <7>	 5/10/95	RR		# 1242893, 1245692 Added fields to support
									Mouse up dispatching
		 <6>	  5/2/95	RR		# 1226050 Added fLastBorderFrame.
		 <5>	 1/13/95	RR		Added eventInfo to event handling methods
		 <4>	10/27/94	RR		Added tokenized focus fields
		 <3>	 9/30/94	RR		#1171182 Treat icons as bundled. Added
									fViewAsFrameToken
		 <2>	  9/6/94	RR		Added utility methods for event propagation
		 <1>	 6/24/94	RR		first checked in
		<14>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		<13>	 2/16/94	JA		Stop including Point.h.
		<12>	  2/9/94	NP		Tiger Team cleanup.
		<11>	 1/21/94	RR		Restored Dispatch() signature
		<10>	 1/18/94	RR		Add fScrollingFocus. Added DispatchTheEvent
									to propagate to container where appropriate
		 <9>	 1/15/94	RR		WinState.h->WinStat.h,
									Dispatch.h->Disptch.h
		 <8>	 1/11/94	T‚		Init... changes
		 <7>	12/20/93	RR		New init strategy
		 <6>	12/16/93	RR		DispatchToFacetUnderPoint became a member
									function
		 <5>	 12/2/93	RR		Use new eventType definition
		 <4>	11/19/93	RR		Added DispatchWindowEvent
		 <3>	 8/13/93	CG		Added ASLM stuff.
		 <2>	 8/11/93	RR		Added fields to cache handy references to
									session globals
		 <1>	 8/10/93	RCR		first checked in

	To Do:
*/

#ifndef _STDDISPM_
#define _STDDISPM_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class MacDispatchModule;

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
// MacDispatchModule
//=====================================================================================

class MacDispatchModule 
{
public:

	MacDispatchModule();
			
	~MacDispatchModule();
	
	ODNVMethod void InitMacDispatchModule(Environment* ev, ODSession* session);

	ODNVMethod ODBoolean Dispatch(Environment* ev, ODEventData* event, ODEventInfo* eventInfo);

protected:

	ODNVMethod void DispatchOSEvent(Environment* ev, ODEventData* event);
	ODNVMethod void MouseMoved(Environment* ev, ODEventData* theEvent);
	ODNVMethod void SuspendResume(Environment* ev, ODEventData* theEvent);
	
	ODNVMethod ODBoolean DispatchMouseDownEvent(Environment* ev, ODEventData* event, ODEventInfo* eventInfo);
	ODNVMethod ODBoolean DispatchMouseUpEvent(Environment* ev, ODEventData* event, ODEventInfo* eventInfo);
	ODNVMethod ODBoolean DispatchMenuEvent (Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo);
	ODNVMethod ODBoolean DispatchMouseDownInContent (Environment* ev, ODWindow* theWindow,
												ODEventData* theEvent, ODEventInfo* eventInfo)	;
	ODNVMethod ODBoolean DispatchMouseDownInBorder(Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)	;
	ODNVMethod ODBoolean DispatchMouseDownEmbedded(Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)	;
	ODNVMethod ODBoolean DispatchMouseUpEmbedded(Environment* ev, ODEventData* theEvent, ODEventInfo* eventInfo)	;
	ODNVMethod ODBoolean DispatchKeyDownEvent(Environment* ev, ODEventData* event, ODEventInfo* eventInfo);
	ODNVMethod ODBoolean DispatchKeyUpEvent(Environment* ev, ODEventData* event, ODEventInfo* eventInfo);
	
	ODNVMethod ODBoolean DispatchUpdateEvent(Environment* ev, ODEventData* event);
	ODNVMethod ODBoolean DispatchActivateEvent(Environment* ev, ODEventData* event);

	ODNVMethod ODBoolean DispatchWindowEvent(Environment* ev, ODWindow* window, 
								short partCode, ODEventData* theEvent, ODEventInfo* eventInfo);

	ODNVMethod ODBoolean DispatchToFacetUnderPoint(Environment* ev, ODWindow* window, ODPoint& windowPt, 
													ODEventData* theEvent, ODEventInfo* eventInfo);

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
	ODSession* 		fSession;
	ODArbitrator* 	fArbitrator;
	ODWindowState* fWindowState;
	ODDispatcher* 	fDispatcher;

	ODTypeToken fKeyFocusToken;
	ODTypeToken fMenuFocusToken;
	ODTypeToken fSelectionFocusToken;
	ODTypeToken fModalFocusToken;
	ODTypeToken fMouseFocusToken;
	ODTypeToken fScrollingFocusToken;
	ODTypeToken fLargeIconViewToken;
	ODTypeToken fSmallIconViewToken;
	ODTypeToken fThumbnailViewToken;
	
	ODFacet* fMouseDownFacet; // The facet to which the last mouse down in border was sent. Used for the mouse up
	ODFacet* fEmbeddedFacet;  // The embedded facet fpr a kODEvtMouseDownEmbedded. Used for the mouse up
	ODBoolean fSuppressMouseUp;	// Mouse down outside active border; Background mouse down
};
#endif // _STDDISPM_

#endif // _PLATFORM_MACINTOSH_
