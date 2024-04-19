/* @(#)Z 1.3 com/src/ui/DspUtilM.cpp, odui, od96os2, odos29646d 96/11/15 15:28:03 (96/07/15 18:26:20) */
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
/*
	File:           DspUtilM.cpp

	Contains:       xxx put contents here xxx

	Owned by:       Richard Rodseth

	Copyright:      © 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>     9/13/95        DM              1277216 GM:API return no ODPoints nor
									ODPolygons
		 <6>     8/29/95        RR              #1267251 Remove ODLogEvents #define
		 <5>     6/20/95        JBS             1257315 change Facet::AcquireWindow to
									GetWindow
		 <4>     5/26/95        RR              #1251403: Multithreading naming support
		 <3>     5/25/95        jpa             Fixed usage of ODDebug. [1253321]
		 <2>     5/10/95        RR              # 1239139 Added LogEvent, and
									GetActiveFacetWithBorderUnderPoint

	To Do:
	In Progress:
*/

#ifndef _DSPUTILM_
#include "DspUtilM.h" 
#endif

#ifndef SOM_ODFacet_xh
#include "Facet.xh"
#endif

#ifndef SOM_ODFrame_xh
#include "Frame.xh"
#endif

#ifndef SOM_ODFrameFacetIterator_xh
#include "FrFaItr.xh"
#endif

#ifndef SOM_ODPart_xh
#include "Part.xh"
#endif

#ifndef SOM_ODTransform_xh
#include "Trnsform.xh"
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif

void WindowToLocal(Environment* ev, ODPoint& point, ODFacet* facet)
{
	TempODTransform xform = facet->AcquireWindowFrameTransform(ev, facet->GetCanvas(ev));
	xform->InvertPoint(ev, &point);
}

ODFacet* GetActiveFacetWithBorderUnderPoint(Environment* ev, ODFrame* frame, ODWindow* window, ODPoint& windowPoint)
{
	ODFacet* foundFacet = kODNULL;
	ODPoint framePoint;

	if ( frame && window )
	{
		ODFrameFacetIterator* facets = frame->CreateFacetIterator(ev);
		for ( ODFacet* facet = facets->First(ev);
				facets->IsNotComplete(ev);
				facet = facets->Next(ev) )
		{
			// get windowPoint in frame coords for hit-testing
			{ TempODTransform winToFrame = facet->AcquireWindowFrameTransform(ev, kODNULL);
			  framePoint = windowPoint;
			  winToFrame->InvertPoint(ev, &framePoint);
			}
			
			if (ODObjectsAreEqual(ev, facet->GetWindow(ev), window)
				&& (facet->ActiveBorderContainsPoint(ev, &framePoint, kODNULL)) )
			{
				foundFacet = facet;
				break;
			}
		}
		delete facets;
	}
	return foundFacet;
}

#if defined(ODDebug) && defined(_PLATFORM_MACINTOSH_)

void LogEvent(ODEventData* event, ODEventInfo* eventInfo, ODFrame* frame, ODFacet* facet)
{
	char eventType[30];
	switch (event->what)
	{
		case kODEvtNull:                 
			strcpy(eventType,"NULL");
			break;
		case kODEvtMouseDown: 
			strcpy(eventType,"kODEvtMouseDown");
			break;
		case kODEvtMouseUp:      
			strcpy(eventType,"kODEvtMouseUp");
			break;
		case kODEvtKeyDown: 
			strcpy(eventType,"kODEvtKeyDown");
			break;
		case kODEvtKeyUp: 
			strcpy(eventType,"kODEvtKeyUp");
			break;
		case kODEvtAutoKey: 
			strcpy(eventType,"kODEvtAutoKey");
			break;
		case kODEvtUpdate: 
			strcpy(eventType,"kODEvtUpdate");
			break;
		case kODEvtDisk: 
			strcpy(eventType,"kODEvtDisk");
			break;
		case kODEvtActivate: 
			strcpy(eventType,"kODEvtActivate");
			break;
		case kODEvtOS: 
			strcpy(eventType,"kODEvtOS");
			break;
		case kODEvtBGMouseDown: 
			strcpy(eventType,"kODEvtBGMouseDown");
			break;
		case kODEvtMenu: strcpy(eventType,"kODEvtMenu");
			break;
		case kODEvtMouseDownEmbedded: 
			strcpy(eventType,"kODEvtMouseDownEmbedded");    
			break;
		case kODEvtMouseUpEmbedded: 
			strcpy(eventType,"kODEvtMouseUpEmbedded");
			break;
		case kODEvtMouseDownBorder: 
			strcpy(eventType,"kODEvtMouseDownBorder");
			break;
		case kODEvtMouseUpBorder: 
			strcpy(eventType,"kODEvtMouseUpBorder");
			break;
		case kODEvtWindow: 
			strcpy(eventType,"kODEvtWindow");
			break;
		case kODEvtMouseEnter: 
			strcpy(eventType,"kODEvtMouseEnter");
			break;
		case kODEvtMouseWithin: 
			strcpy(eventType,"kODEvtMouseWithin");
			break;
		case kODEvtMouseLeave: 
			strcpy(eventType,"kODEvtMouseLeave");
			break;
		case kODEvtBGMouseDownEmbedded: 
			strcpy(eventType,"kODEvtBGMouseDownEmbedded");
			break;
		default:         
			strcpy(eventType,"UNKNOWN");
			break;
	}
	somPrintf("Event: %3d %-25s Facet: %x, Frame: %x", event->what, eventType, facet, frame);
	somPrintf(" Coords: %3d,%3d",eventInfo->where.IntX(),eventInfo->where.IntY());
	if ((event->modifiers & shiftKey) != 0)
		somPrintf(" Shift");
	if ((event->modifiers & cmdKey) != 0)
		somPrintf(" Cmd");
	if ((event->modifiers & optionKey) != 0)
		somPrintf(" Opt");
	if ((event->modifiers & controlKey) != 0)
		somPrintf(" Ctl");
	if (eventInfo->propagated)
		somPrintf(" PROPAGATED");
	somPrintf("\n");
	if ((event->what == kODEvtMouseDownEmbedded) ||
		(event->what == kODEvtMouseUpEmbedded) ||
		(event->what == kODEvtBGMouseDownEmbedded) ||
		(event->what == kODEvtMouseDownBorder))
	{
		somPrintf("		Embedded Facet: %x Embedded Frame: %x\n", eventInfo->embeddedFacet, eventInfo->embeddedFrame);
	}
}

#endif // ODDebug & _PLATFORM_MACHINTOSH_
