/* @(#)Z 1.11 com/src/imaging/polyclip/PGEvent.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:08 (96/10/29 09:26:29) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odimaging
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
/*          3/21/96 jab  Merge with DR4.  Changes for cross         */
/*                       platform support.                          */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    PGEvent.cpp

  Contains:  Geometric event structure for the clipper.

	Owned by:	Jens Alfke (based on algorithm by A. C. Kilgour)

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):
  
     <8>   8/16/95  NP	   1274946: ErrorDef.idl problems. Add include file.
     <7>   5/25/95  jpa    Fixed 'for' loop for ANSI compliance   [1253324]
     <6>   3/22/95  jpa	   Fixed MergeWith to merge upper edge ranges correctly [1230776]
     <5>  12/20/94  jpa    Removed some excess code from Reggie
                  [1186959]
     <4>   12/5/94  jpa    Minor code cleanup from code review
                  [1203923]
     <3>   9/29/94  RA    1189812: Mods for 68K build.
     <2>   6/17/94  jpa    Include AltPoint and AltPoly.
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <1>    5/9/94  jpa    first checked in

  Theory of Operation:
  "An event is defined to be any point in 2D space which lies on more
  than one edge of the input polygon or polygons, with the exception
  of non-vertex points shared by coincident or partially coincident
  edges. Every event is either a vertex or an intersection between
  edges (or both)." --Kilgour, p.381
  
  To Do:
	In Progress:
		
*/


#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODPolygonClip
#endif


#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)  
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif
#endif
  
#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifndef _PGEVENT_
#include "PGEvent.h"
#endif

#ifndef _PGEDGE_
#include "PGEdge.h"
#endif

#ifndef _PGPOLY_
#include "PGPoly.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

//
// Uncomment the next 2 lines to turn on logging in this file.
//
//#undef LOGGING
//#define LOGGING 1



//=============================================================================
// PGEvent Initialization
//=============================================================================


PGEvent::PGEvent( )
  :ODPoint(),
   fFirstUpperEdge(kODNULL),
   fLastUpperEdge(kODNULL),
   fLowerEdges()
{
}


PGEvent*
PGEvent::InitLocalMinEvent( const PGVertex *v )
{
  *(ODPoint*)this = *v;
  PGEdge *e1 = new PGEdge(v->GetPrevious(),v);
  PGEdge *e2 = new PGEdge(v,v->GetNext());
  if( e1->TestPoint(e2->fLowVert->AsPoint()) != kLeft ) {  // Edges point _down_, remember
    PGEdge *temp = e1;
    e1 = e2; e2 = temp;
  }
  fLowerEdges.AddFirst(e1);
  fLowerEdges.AddLast (e2);
  
  BEGINLOG; LOG("e Added local min event at "); LOG(*v); LOG("\n"); ENDLOG; 
  
  return this;
}


PGEvent*
PGEvent::InitLowEvent( const PGEdge *edge )
{
  // Create a new event for the lower vertex of this edge. The next lower edge
  // in the input contour will become the lower edge of the new event.
  
  const PGVertex *tail = edge->fLowVert;
  *(ODPoint*)this = *tail;

  // Now find head/tail of next edge in sequence:
  const PGVertex *head;
  if( edge->fSense > 0 )
    head = tail->GetNext();
  else {
    head = tail;
    tail = tail->GetPrevious();
  }
  if( Compare(head,tail) == edge->fSense ) {        // Not at a local max:
    PGEdge *e = new PGEdge(tail,head);
    fLowerEdges.AddFirst(e);
  }
  
  fFirstUpperEdge = fLastUpperEdge = (PGEdge*)edge;

  BEGINLOG; LOG("e Added lower-vertex event at ");
        LOG(edge->fLowVert->AsPoint()); LOG("\n"); ENDLOG; 
  
  return this;
}


PGEvent*
PGEvent::InitIntersection( const PGEdge *e1, const PGEdge *e2, const ODPoint &sect )
{
  // e1 must be before e2 in the aet!!
  
  *(ODPoint*)this = sect;
  fFirstUpperEdge = (PGEdge*)e1;
  fLastUpperEdge = (PGEdge*)e2;

  BEGINLOG; LOG("e Added intersection event at "); LOG(sect); LOG("\n"); ENDLOG; 
  
  return this;
}


//=============================================================================
// PGEvent Operations
//=============================================================================

#ifndef _EVENT_TEMPLATES_
ODBoolean
PGEvent::ComesBefore ( const Sortable *s ) const
{
  const PGEvent *ev = (const PGEvent*)s;
  if( this->y < ev->y )
    return kODTrue;                  // I am above ev
  else
    return (this->y == ev->y && this->x < ev->x);  // I am directly left of ev
}
#endif   // _EVENT_TEMPLATES_


#if 0
ODBoolean
PGEvent::operator> ( const PGEvent &ev ) const
{
  if( this->y > ev.y )
    return kODTrue;                  // I am below ev
  else
    return (this->y == ev.y && this->x > ev.x);    // I am directly right of ev
}
#endif


void
PGEvent::MergeWith( PGEvent *ev, PGEdgeTable &aet )
{
  if( ev->fFirstUpperEdge )
    if( fFirstUpperEdge ) {
      // Merge together the upper edge lists. Just see if the other event's
      // range of upper edges extends beyond mine:

			PGEdge *e;
			PGEdge *e2 = ev->fFirstUpperEdge;
			if( e2 != fFirstUpperEdge )
				for(e=fFirstUpperEdge; ; ) {		// scan leftwards from 1st edge
					e=(PGEdge*)aet.Before(*e);
					if( !e )
						break;
					else if( e==e2 ) {
						fFirstUpperEdge = e2;
						break;
					}
				}
			e2 = ev->fLastUpperEdge;
			if( e2 != fLastUpperEdge )
				for(e=fLastUpperEdge; ; ) {			// scan rightwards from last edge
					e=(PGEdge*)aet.After(*e);
					if( !e )
						break;
					else if( e==e2 ) {
						fLastUpperEdge = e2;
						break;
					}
				}
    } else {
      fFirstUpperEdge = ev->fFirstUpperEdge;
      fLastUpperEdge = ev->fLastUpperEdge;
    }
  
  if( ! ev->fLowerEdges.IsEmpty() )
    if( ! fLowerEdges.IsEmpty() ) {
      // Merge together the lower edge lists by inserting each of the other
      // event's lower edges into my list:
      
      PGEdge *e, *enext;
      for( e=(PGEdge*)ev->fLowerEdges.First(); e; e=enext ) {
        enext = (PGEdge*)ev->fLowerEdges.After(*e);
        e->Remove();
        fLowerEdges.Add(e);        // Add in sorted order
      }
      
    } else {
      fLowerEdges.InsertListBefore(ev->fLowerEdges,kODNULL);
    }
  
  delete ev;
}


//=============================================================================
// PGEvent Processing
//=============================================================================


void
PGEvent::Process( PGEdgeTable &aet, PGEventQueue &eventQueue )
{
  // See Kilgour pp.386-387
  
  PGEdge *leftNeighbor, *rightNeighbor;
  
  // Determine the left and right neighbors of my upper edges:
  if( fFirstUpperEdge ) {
    leftNeighbor = (PGEdge*)aet.Before(*fFirstUpperEdge);
    rightNeighbor = (PGEdge*)aet.After(*fLastUpperEdge);
    
  } else {
    // Search for first active edge to the right of me:
    PGSide side;
    PGEdge *e;
    for( e=(PGEdge*)aet.First(); e; e=(PGEdge*)aet.After(*e) ) {
      side = e->TestPoint(*this);
      if( side != kLeft )
        break;          // break when e is not to my left
    }
    if( e ) {
      leftNeighbor = (PGEdge*)aet.Before(*e);  // In aet somewhere
      if( side==kOnTop ) {
        rightNeighbor = (PGEdge*)aet.After(*e);      // On top of an active edge
        fFirstUpperEdge = fLastUpperEdge = e;      // Make this my upper edge
      } else
        rightNeighbor = e;
    } else {
      leftNeighbor = (PGEdge*)aet.Last();      // Past end of aet
      rightNeighbor = kODNULL;
    }
  }
  
  PGEdgeTable upperEdges;
  
  // Now process upper edges:
  if( fFirstUpperEdge ) {
    // Extend range of upper edges to include entire bundles:
    while( fFirstUpperEdge->fLeftBundle ) {
      PGASSERT(leftNeighbor!=NULL);
      fFirstUpperEdge = leftNeighbor;
      leftNeighbor = (PGEdge*)aet.Before(*leftNeighbor);
    }
    while( fLastUpperEdge->fRightBundle ) {
      PGASSERT(rightNeighbor!=NULL);
      fLastUpperEdge = rightNeighbor;
      rightNeighbor = (PGEdge*)aet.After(*rightNeighbor);
    }
    
    // Remove the list of upper edges from the aet:
    aet.RemoveRange(fFirstUpperEdge,fLastUpperEdge);
    upperEdges.AddRange(fFirstUpperEdge,fLastUpperEdge, NULL);
    
    // Copy edges that extend below/right of this point, to the lower edge list:
    for( PGEdge *e=(PGEdge*)upperEdges.First(); e; e=(PGEdge*)upperEdges.After(*e) ) {
      if( Compare(e->fLowVert,this) == kPositive ) {
        // We have to keep a _copy_ of the edge on the upper list, but move the
        // actual edge down below:
        PGEdge *ecopy = e->ReplaceWithCopy();
        fLowerEdges.Add(e);        // Add to lower-edge list in sorted order
        e = ecopy;
      }
    }
  }
  
#if ODDebug
  {
    // Number of edges in aet must remain even.
    ODSLong nUpper = upperEdges.Count();
    ODSLong nLower = fLowerEdges.Count();
    LOG("m Matching %d upper edges and %d lower edges...\n",nUpper,nLower);
    if( (nUpper-nLower)&1 ) {
      LOG("!!!Parity violation; %d upper, %d lower!\n", nUpper,nLower);
      WARNMSG_DEBUG(WARN_INDEX(-1), "Parity violation; %d upper, %d lower!", nUpper,nLower);
    }
  }
#endif
  
  if( fLowerEdges.IsEmpty() ) {
    // No lower edges, all we need to do is see if the two neighbors intersect below:
    eventQueue.AddIntersectionIfBelow(leftNeighbor,rightNeighbor, this);

  } else {
    // Record wrap numbers and edge-bottom events for lower edges:
    short thiswrap;
    if( leftNeighbor ) {
      thiswrap = leftNeighbor->fWrapNo;
      if( leftNeighbor->fSense>0 )
        thiswrap--;
    } else
      thiswrap = 0;
      
    for( PGEdge *e=(PGEdge*)fLowerEdges.First(); e; e=(PGEdge*)fLowerEdges.After(*e) ) {
      if( e->fSense<0 )          // Would be ">0", for CCW paths as in Kilgour
        e->fWrapNo = ++thiswrap;
      else
        e->fWrapNo = thiswrap--;
        
      if( !e->fLowEvent ) {
        PGEvent *ev = (new PGEvent)->InitLowEvent(e);
        eventQueue.Add(ev);
        e->fLowEvent = kODTrue;
      }
    }
#if ODDebug
    if( rightNeighbor ) {
      if( rightNeighbor->fSense<0 )    // Would be ">0", for CCW paths as in Kilgour
        thiswrap++;
      PGASSERT(thiswrap==rightNeighbor->fWrapNo);
    } else
      PGASSERT(thiswrap==0);
#endif
    
    // Check for intersections between lower edges & neighbors in aet:
    eventQueue.AddIntersectionIfBelow(leftNeighbor,(PGEdge*)fLowerEdges.First(), this);
    eventQueue.AddIntersectionIfBelow((PGEdge*)fLowerEdges.Last(),rightNeighbor, this);
  }
    
  //Extend/merge paths of upper & lower edges:
  this->ConnectEdgePaths(upperEdges);
  
  // Link lower edges into AET:
  aet.InsertListBefore(fLowerEdges,rightNeighbor);
    
  // Dispose of upper edge list:
  upperEdges.DeleteAllLinks();
}


//=============================================================================
// PGEvent Path Connection
// See Kilgour, pp.388-391.
//=============================================================================


void
PGEvent::ConnectEdgePaths( PGEdgeTable &upperEdges )
{
  // Main part of merging.
  // All the signs (kPositive/kNegative) have been reversed from Kilgour's
  // paper, since he uses counterclockwise polygons and we use clockwise!
  
  upperEdges.Unmatch();
  fLowerEdges.Unmatch();
  
  this->ConnectAdjacentEdgePaths(upperEdges,kNegative);
  this->ConnectAdjacentEdgePaths(fLowerEdges,kNegative);
  
  PGEdge *up, *lo;
  up = upperEdges.FirstUnmatched();
  if( up && up->fSense==kPositive ) {
    lo = fLowerEdges.FirstUnmatched();
    while( lo && lo->fSense==kPositive ) {
      LOG("      +É/+É\n");
      up->ExtendPath(lo,this);
      up = upperEdges.UnmatchedAfter(up);
      if( up && up->fSense==kPositive )
        lo = fLowerEdges.UnmatchedAfter(lo);
      else
        break;
    }
  }
  
  up = upperEdges.LastUnmatched();
  if( up && up->fSense==kNegative ) {
    lo = fLowerEdges.LastUnmatched();
    while( lo && lo->fSense==kNegative ) {
      LOG("      É-/É-\n");
      up->ExtendPath(lo,this);
      up = upperEdges.UnmatchedBefore(up);
      if( up && up->fSense==kNegative )
        lo = fLowerEdges.UnmatchedBefore(lo);
      else
        break;
    }
  }
  
  ConnectAdjacentEdgePaths(upperEdges,kPositive);
  ConnectAdjacentEdgePaths(fLowerEdges,kPositive);
  
  PGASSERT( upperEdges.FirstUnmatched()==kODNULL);
  PGASSERT(fLowerEdges.FirstUnmatched()==kODNULL);
}


void
PGEvent::ConnectAdjacentEdgePaths( PGEdgeTable &edges, PGSense firstSense )
{
  // Subroutine called by ConnectEdgePaths.
  // Connect nested adjacent opposite-sign edge pairs in upper or lower list:
  
  PGEdge *e1, *e2;
  
  e1 = edges.FirstUnmatched();
  if( !e1 )
    return;
  e2 = edges.UnmatchedAfter(e1);
  while( e2 ) {
    if( e1->fSense==firstSense && e2->fSense==-firstSense ) {
      
      // Found some edges to connect!
      if( LOGGING ) {
        BEGINLOG;
        LOG("      ");
        if( &edges==&fLowerEdges )
          LOG("/");
        if( firstSense==kPositive )
          LOG("+-");
        else
          LOG("-+");
        if( &edges!=&fLowerEdges )
          LOG("/");
        LOG("\n");
        ENDLOG;
      }
      if( &edges==&fLowerEdges )
        e1->StartPath(e2,this);
      else
        e1->MergePaths(e2,this);
        
      e1 =  edges.UnmatchedBefore(e1);
      if( !e1 ) {
        e1 = edges.UnmatchedAfter(e2);
        if( !e1 )
          return;
        e2 = e1;
      }
    } else
      e1 = e2;
    e2 = edges.UnmatchedAfter(e2);
  }
}


//=============================================================================
// PGEventQueue
//=============================================================================


void
PGEventQueue::AddIntersectionIfBelow( PGEdge *e1, PGEdge *e2, const PGEvent *curEv )
{
  // e1 must be before e2 in the aet!!
  
  ODPoint sect;
  if( e1 && e2 && e1->IntersectWith(e2, sect) )
    if( Compare(&sect,curEv) == kPositive ) {
      PGEvent *ev = new PGEvent;
      ev->InitIntersection(e1,e2,sect);
      this->Add(ev);
    }
}



