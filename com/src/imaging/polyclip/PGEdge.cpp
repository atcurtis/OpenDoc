/* @(#)Z 1.7 com/src/imaging/polyclip/PGEdge.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:07 (96/10/29 09:26:20) */
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
/*          3/21/96 jab  Merge with DR4. Changes for cross platform */
/*                       support.                                   */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    PGEdge.cpp

  Contains:  Active-edge structure for the clipper.

  Owned by: Jens Alfke (based on algorithm by A. C. Kilgour)

  Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):
  
     <6>   8/16/95  NP    1274946: ErrorDef.idl problems. Add include file.
     <5>   3/20/95  jpa   Another fix to TestPoint, from Chuck
                  Dumont. [1186959]
     <4>   12/5/94  jpa    Fixed a bug in TestPoint, thanks to Chuck
                  Dumont. [1203950]
     <3>   9/29/94  RA    1189812: Mods for 68K build.
     <2>   6/17/94  jpa    Include AltPoint and AltPoly.
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <3>   5/27/94  jpa    Fixed a possible bug that I noticed.
     <2>   5/10/94  jpa    Tweaked a comparison to appease #%$*
                  cfront.
     <1>    5/9/94  jpa    first checked in
     
  Theory of Operation:
    A. C. Kilgour, "Polygon Processing For VLSI Pattern Generation"
    In Rogers & Earnshaw, "Techniques For Computer Graphics"
    Springer-Verlag, 1987
    
    A PGEdgeTable represents the active-edge table as a LinkedList
    of PGEdges. A PGEdge points to its endpoints and to its output
    contour (if any) and has some other flags used during processing.
  
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

#ifndef _PGEDGE_
#include "PGEdge.h"
#endif

#ifndef _PGPOLY_
#include "PGPoly.h"
#endif

#ifndef _LINEOPS_
#include "LineOps.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMATH_
#include "ODMath.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#include <string.h>

//
// Uncomment the next 2 lines to turn on logging in this file.
//
//#undef LOGGING
//#define LOGGING 1

//=============================================================================
// PGEdge
//=============================================================================


PGEdge::PGEdge( const PGVertex *tail, const PGVertex *head )
  :fWrapNo(0),
   fLowEvent(kODFalse),
   fMatched(kODFalse),
   fLeftBundle(kODFalse),
   fRightBundle(kODFalse),
   fSense(Compare(head,tail)),  // Edge goes from tail to head!
   fOtherSide(kODNULL),
   fOutput(kODNULL)
{
  if( fSense == kPositive ) {
    fHighVert = tail;
    fLowVert = head;
  } else {
    fHighVert = head;
    fLowVert = tail;
  }
  fDelta =  fLowVert->AsPoint();
  fDelta-= fHighVert->AsPoint();    // fDelta.y >= 0!
}


PGEdge*
PGEdge::ReplaceWithCopy( )
{
  // When an edge that continues below an event is moved to the lower edge list,
  // a copy is left above for matching. The copy has to inherit most of the edge's
  // characteristics, like its list membership and its output contour.
  
  PGEdge *copy = new PGEdge(*this);
  
  this->GetPrevious()->SetNext(copy);
  this->GetNext()->SetPrevious(copy);
  
  this->SetPrevious(kODNULL);
  this->SetNext(kODNULL);
  
  if( fOutput ) {
    fOutput = kODNULL;
    fOtherSide->fOtherSide = copy;
    fOtherSide = kODNULL;
  }
  
  return copy;
}


PGSide
PGEdge::TestPoint( const ODPoint &pt )
{
  // Since we have to multiply two Fracts, we use the Mac Toolbox's
  // LongMul routine, which multiplies two 32-bit values producing a
  // 64-bit result. Since Fixeds are just scaled longs, we can use this
  // since all we are doing is comparing the results.
  
  // Our coord system is flipped from the normal one, so the signs here
  // are the reverse of the ones given by Kilgour.
  
  // Remember also that edges point DOWNWARD, and the left/right is
  // from the edge's perspective, so it's backwards from the normal
  // perspective!
  
  ODWide s1, s2;
  ODWideMultiply(fDelta.x, pt.y - fHighVert->y, &s1);
  ODWideMultiply(fDelta.y, pt.x - fHighVert->x, &s2);
  
#if !defined(_PLATFORM_UNIX_)
  if( s1.hi > s2.hi )
    return kRight;
  else if( s1.hi < s2.hi )
    return kLeft;
  else
    if( s1.lo==s2.lo )
      return kOnTop;
    else
      return (s1.lo>s2.lo) ? kRight : kLeft;
#else // defined(_PLATFORM_UNIX_)
  if(s1 > s2)
    return kRight;
  else if( s1 < s2)
    return kLeft;
  else
    return kOnTop;
#endif // !defined(_PLATFORM_UNIX_)
}


ODBoolean
PGEdge::IntersectWith( const PGEdge *edge, ODPoint &sect )
{
  return IntersectSegments( this->fHighVert->AsPoint(), this->fLowVert->AsPoint(),
                edge->fHighVert->AsPoint(), edge->fLowVert->AsPoint(),
                &sect );
}


//=============================================================================
// PGEdge Path Construction
//=============================================================================


void
PGEdge::StartPath( PGEdge *edge, const ODPoint *where )
{
  // Connecting two edges on the lower-edge list of an event.
  // Start a new path:
  
  PGOutputContour *out;
  if( fWrapNo>=gMinWrap && fWrapNo<=gMaxWrap ) {
    out = new PGOutputContour(this,edge);
    out->AddVertex(kRight,where);
  } else
    out = kIgnoredOutput;    // We won't be needing this contour, thanks
  

  fSide = kLeft;
  edge->fSide = kRight;
  fOtherSide = edge;
  edge->fOtherSide = this;
  fOutput = edge->fOutput = out;
  fMatched = edge->fMatched = kODTrue;

  BEGINLOG;
  LOG("^ Creating contour %p (wrap %d) at ", fOutput, fWrapNo);
  LOG(*where); LOG("\n");
  ENDLOG;
}


void
PGEdge::ExtendPath( PGEdge *edge, const ODPoint *where )
{
  // Connecting an upper with a lower edge:
  
  PGASSERT(fOutput!=kODNULL);
  PGASSERT(edge->fOutput==kODNULL);
  
  // Unless the edges are parallel, we need to insert the intersection point ('where')
  // into the output contour:
  if( fOutput!=kIgnoredOutput && *fHighVert != edge->fHighVert->AsPoint() ) {
    fOutput->AddVertex(fSide,where);
    LOG("%c Added vertex (%.2f,%.2f) to contour %p\n",
          fSide==kLeft ?'<' :'>', where->x/65536.0,where->y/65536.0, fOutput );
  }
  edge->fSide = fSide;
  edge->fOutput = fOutput;
  fOutput = kODNULL;
  edge->fOtherSide = fOtherSide;
  fOtherSide->fOtherSide = edge;
  
  fMatched = edge->fMatched = kODTrue;
}


void
PGEdge::MergePaths( PGEdge *edge, const ODPoint *where )
{
  // Connecting two upper edges:
  
  PGASSERT(this->fWrapNo==edge->fWrapNo);
  
  PGOutputContour *thisPath = fOutput;
  PGOutputContour *edgePath = edge->fOutput;
  PGASSERT(thisPath);
  PGASSERT(edgePath);
  if( ODDebug && thisPath != kIgnoredOutput )
    // Same-sense paths must connect at opposite-side edges, and vice versa:
    PGASSERT( (this->fSide==edge->fSide)
                 != (thisPath->fSense==edgePath->fSense));
  
  if( fOutput != kIgnoredOutput )
    fOutput->AddVertex(fSide,where);
  
  if( edge == fOtherSide ) {
    // Path joins with itself, i.e. is closing:
    PGASSERT(thisPath==edgePath);
    if( fOutput != kIgnoredOutput )
       fOutput->Close();
    LOG("v Closing contour %p at ", thisPath);
     
  } else {
    // Two different paths are merging together:
    PGASSERT( thisPath!=edgePath || thisPath==kIgnoredOutput );
    LOG("X Merging contour %p (%c) with %p (%c) at ",
        thisPath, fSide==kLeft?'<':'>',
        edgePath, edge->fSide==kLeft?'<':'>');

    // Must glue the two paths together and remove the second one:
    if( fOutput != kIgnoredOutput ) {
      fOutput->AppendContour(edgePath,fSide);
      delete edgePath;
    }
    
    // Introduce the end edges of the merged path to each other:
    PGEdge *myOther = fOtherSide;
    PGEdge *itsOther = edge->fOtherSide;
    myOther->fOtherSide = itsOther;
    itsOther->fOtherSide = myOther;
    itsOther->fOutput = thisPath;
    itsOther->fSide = (PGSide)( -(int)myOther->fSide );    // Opposite side
  }

   fOutput = edge->fOutput = kODNULL;
  fOtherSide = edge->fOtherSide = kODNULL;
  
  fMatched = edge->fMatched = kODTrue;
  
  LOG(*where); LOG("\n");
}


//=============================================================================
// PGEdgeTable
//=============================================================================


void
PGEdgeTable::Add( PGEdge *e )
{
  e->fLeftBundle = e->fRightBundle = kODFalse;
  
  for( PGEdge *el=(PGEdge*)this->First(); el; el=(PGEdge*)this->After(*el) ) {
    PGSide test = el->TestPoint(e->fLowVert->AsPoint());
    if( test == kRight ) {
      e->AddBefore(el);
      return;
      
    } else if( test == kOnTop ) {
      // Overlapping an existing edge, so add to bundle.
      // Add first if positive edge, last if negative.
      if( e->fSense == kPositive ) {
        e->AddBefore(el);
        e->fRightBundle = el->fLeftBundle = kODTrue;
        
      } else {
        while( el->fRightBundle )
          el = (PGEdge*)this->After(*el);
        e->AddAfter(el);
        el->fRightBundle = e->fLeftBundle = kODTrue;
      }
      return;
    }
  }
  
  // Must be to the right of each edge in the list...
  this->AddLast(e);
}


PGEdge*
PGEdgeTable::RemoveRange( PGEdge *first, PGEdge *last )
{
  if( !first ) {
    first = (PGEdge*)this->First();
    if( !first )
      return kODNULL;            // List is empty
  }
  if( !last )
    last = (PGEdge*)this->Last();
    
  Link *before = first->GetPrevious();
  Link *after  = last->GetNext();
  
  before->SetNext( after );
  after->SetPrevious( before );
  first->SetPrevious( kODNULL );
  last->SetNext( kODNULL );
  
  return first;
}


void
PGEdgeTable::AddRange( PGEdge *first, PGEdge *last, PGEdge *before )
{
  // Assumes first...last are chained together but not part of a list.
  // (RemoveRange will leave them in this state.)
  
  if( first ) {
    PGASSERT(last);
    if( !before )
      before = (PGEdge*)&this->fSentinel;
    PGEdge *after = (PGEdge*)before->GetPrevious();
    after->SetNext( first );    first->SetPrevious( after );
    before->SetPrevious( last );  last->SetNext( before );
  }
}


void
PGEdgeTable::InsertListBefore( PGEdgeTable &list, PGEdge *before )
{
  PGEdge *first = (PGEdge*)list.First();
  if( first ) {
    PGEdge *last  = (PGEdge*)list.Last();
    list.RemoveRange(first,last);
    this->AddRange(first,last,before);
  }
}


void
PGEdgeTable::Unmatch( )
{
  for( Link *e = this->First(); e; e=this->After(*e) )
    ((PGEdge*)e)->fMatched = kODFalse;
}


PGEdge*
PGEdgeTable::FirstUnmatched( )
{
  return this->UnmatchedAfter((PGEdge*)&fSentinel);
}


PGEdge*
PGEdgeTable::LastUnmatched( )
{
  return this->UnmatchedBefore((PGEdge*)&fSentinel);
}


PGEdge*
PGEdgeTable::UnmatchedBefore( const PGEdge *e )
{
  PGASSERT(e!=kODNULL);
  for( ; ; ) {
    e = (PGEdge*)e->GetPrevious();
    if( (Link*)e==&fSentinel )
      return kODNULL;
    else if( ! e->fMatched )
      return (PGEdge*)e;
  }
}


PGEdge*
PGEdgeTable::UnmatchedAfter( const PGEdge *e )
{
  PGASSERT(e!=kODNULL);
  for( ; ; ) {
    e = (PGEdge*)e->GetNext();
    if( (Link*)e==&fSentinel )
      return kODNULL;
    else if( ! e->fMatched )
      return (PGEdge*)e;
  }
}



