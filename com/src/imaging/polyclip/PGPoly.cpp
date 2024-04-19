/* @(#)Z 1.6 com/src/imaging/polyclip/PGPoly.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:08 (96/10/29 09:26:38) */
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

/*
  File:    PGPoly.cpp

  Contains:  Polygon structs for the clipper.

  Owned by:  Jens Alfke (based on algorithm by A. C. Kilgour)

  Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <4>   5/25/95  jpa    Only define ff if not already defined.
                  [1241078, 1253324]
     <3>   12/5/94  jpa    Support right handed coords, e.g. OS/2.
                  [1203950]
     <2>   9/29/94  RA    1189812: Mods for 68K build.
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <1>    5/9/94  jpa    first checked in
  To Do:
    Exception handling!!
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

#ifndef _PGPOLY_
#include "PGPoly.h"
#endif

#ifndef _PGEDGE_
#include "PGEdge.h"
#endif

#ifndef _PGEVENT_
#include "PGEvent.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif


//
// Uncomment the next 2 lines to turn on logging in this file.
//
//#undef LOGGING
//#define LOGGING 1

//
// Does the graphics system use right handed (origin at bottom left) coordinates?
//
#ifdef _PLATFORM_OS2_
  #define RIGHT_HANDED_COORDS 1
#else
  #define RIGHT_HANDED_COORDS 0
#endif


#ifndef ff
#define ff(INT)    ( (ODFixed)(INT) <<16 )
#endif


PGContourList *gOutputContours;


PGSense
Compare( const ODPoint *p1, const ODPoint *p2 )
{
  if( p1->y > p2->y )
    return kPositive;            // p1 is below p2
  else if( p1->y < p2->y )
    return kNegative;            // p1 is above p2
  else if( p1->x > p2->x )
    return kPositive;            // p1 is directly right of p2
  else if( p1->x < p2->x )
    return kNegative;            // p1 is directly left of p2
  else
    return kZero;              // p1 is coincident with p2.
}


#ifdef _PLATFORM_MACINTOSH_
#if LOGGING
void
somPrintf( const ODPoint &pt )
{
  LOG("(%.2f,%.2f)", pt.x/65536.0,pt.y/65536.0);
}
#endif
#else
void
somPrintf( const ODPoint &pt )
{
  LOG("(%.2f,%.2f)", pt.x/65536.0,pt.y/65536.0);
}
#endif      // _PLATFORM_MACINTOSH_



//=============================================================================
// PGVertex
//=============================================================================


PGVertex::PGVertex( const ODPoint *pt, PGVertex *prev )
  :ODPoint(*pt)
{
  if( prev ) {
    fPrevious = prev;
    fNext = prev->fNext;
    fPrevious->fNext = this;
    fNext->fPrevious = this;
  } else
    fPrevious = fNext = this;
}


void
PGVertex::Reverse( )
{
  PGVertex *temp = fNext;
  fNext = fPrevious;
  fPrevious = temp;
}


void
PGVertex::JamNext( PGVertex *next )
{
  fNext = next;
  next->fPrevious = this;
}


//=============================================================================
// PGContour
//=============================================================================


PGContour::PGContour( LinkedList *onList )
  :Link(),
   fFirst(kODNULL),
   fLast(kODNULL),
   fNVertices(0)
{
  if( onList )
    onList->AddLast(this);
}


PGContour::~PGContour( )
{
  this->Remove();
  
  // Delete all vertices. Remember that PGContours are circular,
  // while PGOutputContours are not.
  PGVertex *v, *next;
  for( v=fFirst; v; v=next ) {
    next = v->GetNext();
    if( next==fFirst )
      next=kODNULL;
    delete v;
  }
}


PGContour*
PGContour::InitContour( const ODContour *cont, ODBoolean reverse )
{
  fNVertices = cont->nVertices;
  const ODPoint *prev = &cont->vertex[fNVertices-1];
  const ODPoint *p = &cont->vertex[0];
  if( reverse ) {
    const ODPoint *temp = prev; prev=p; p=temp;
  }

  BEGINLOG;
  for( ODSLong n=fNVertices; n>0; n-- ) {
    LOG(*p);
    if( *p!=*prev ) {
      PGVertex *v = new PGVertex(p,fLast);
      if( !fFirst )
        fFirst = v;
      fLast = v;
      prev = p;
    } else
      fNVertices--;
    if( reverse )
      p--;
    else
      p++;
  }
  LOG("\n"); ENDLOG;
  
  if( fNVertices >= 3 )
    return this;
  else {
    delete this;      // Not a proper contour, so bail out!
    return kODNULL;
  }
}


void
PGContour::AddAllEvents( PGEventQueue &q )
{
  PGVertex *prev = fLast;
  PGVertex *v = fFirst;
  PGSense lastDir = Compare(prev,prev->GetPrevious());
    
  for( ODSLong i=fNVertices; i>0; i--, prev=v, v=v->GetNext() ) {
    PGSense dir = Compare(v,prev);
    if( lastDir<0 && dir>0 ) {          // Ha! Local minimum
      PGEvent *ev = (new PGEvent)->InitLocalMinEvent(prev);
      q.Add(ev);
    }
    lastDir = dir;
  }
}


//=============================================================================
// PGOutputContour
//=============================================================================


PGOutputContour::PGOutputContour( const PGEdge* /*left*/, const PGEdge *right )
  :PGContour(gOutputContours),
   fWrapNo(right->fWrapNo),
   fSense(right->fSense)
{
  // Clockwise contours have fSense>0, counterclockwise have fSense<0.
}


PGOutputContour::PGOutputContour( short x, short y )
  :PGContour(kODNULL),
   fWrapNo(1),
    fSense(kPositive)
{
  // Used by Rgn2Poly() but not by the clipper.
  
  ODPoint pt(ff(x),ff(y));
  this->AddVertex(kRight,&pt);
}


void
PGOutputContour::AddVertex( PGSide side, const ODPoint *pt )
{
  // Negative-sense contours link their vertices from right to left!

  PGVertex *v = new PGVertex(pt,fLast);
  if( side!=fSense || !fFirst )
    fFirst = v;
  if( side==fSense || !fLast )
    fLast = v;
  fNVertices++;
}


void
PGOutputContour::AddVertex( PGSide side, short x, short y )
{
  // Used by Rgn2Poly() but not by the clipper.
  
  ODPoint pt( ff(x), ff(y) );
  this->AddVertex(side,&pt);
}


void
PGOutputContour::Close( )
{
  // Don't need to do anything (?)
}


void
PGOutputContour::AppendContour( PGOutputContour *cont, PGSide toSide )
{
  fNVertices += cont->fNVertices;
  
  PGVertex *a, *b;
  if( toSide==fSense ) {
    a = fLast;
    b = cont->fFirst;
    fLast = cont->fLast;
  } else {
    a = cont->fLast;
    b = fFirst;
    fFirst = cont->fFirst;
  }
  a->JamNext(b);
  
  fLast->JamNext(fFirst);
  
  cont->fFirst = cont->fLast = kODNULL;
}


//=============================================================================
// PGContourList
//=============================================================================


void
PGContourList::ReadPolygon( const ODPolygon &poly, ODBoolean reverse, PGEventQueue &q )
{
#if RIGHT_HANDED_COORDS
  reverse = !reverse;
#endif

  ODSLong n = poly.GetNContours();
  const ODContour *c;
  for( c=poly.FirstContour(); n>0; n--,c=c->NextContour() ) {
    LOG("    ");
    PGContour *cont = (new PGContour(this))->InitContour(c,reverse);
    if( cont )
      cont->AddAllEvents(q);
  }
}


void
PGContourList::BuildPolygon( ODPolygon &poly )
{
  poly.Clear();
  
  ODSLong nContours = 0;
  ODSLong *nVertices = new ODSLong[this->Count()];
  
  PGOutputContour *pgcont;
  for( pgcont=(PGOutputContour*)this->First(); pgcont; pgcont=(PGOutputContour*)this->After(*pgcont) ) {
    ODSLong n = pgcont->CountVertices();
    if( n>=3 )
      nVertices[nContours++] = n;
  }
  
  LOG("$ Building output polygon with %d contours...\n", nContours);
  
  if( nContours==0 ) {
    delete nVertices;
    return;
  }
  
  TRY{
    poly.SetContours(nContours,nVertices);
  }CATCH_ALL{
    delete nVertices;
    RERAISE;
  }ENDTRY
  delete nVertices;
  
  BEGINLOG;
  ODContour *cont = poly.FirstContour();
  pgcont = (PGOutputContour*)this->First();
  for( ODSLong i=nContours; i>0; i-- ) {
    while( pgcont->CountVertices()<3 )
      pgcont = (PGOutputContour*)pgcont->GetNext();      // Skip unwanted contours
    
#if RIGHT_HANDED_COORDS
      PGVertex *vert = pgcont->Last();
#else
      PGVertex *vert = pgcont->First();
#endif
    for( ODSLong j=0; j<cont->nVertices; j++ ) {
      LOG(*vert); LOG(" ");
      cont->vertex[j] =*vert;
#if RIGHT_HANDED_COORDS
      vert = vert->GetPrevious();
#else
      vert = vert->GetNext();
#endif
    }
    LOG("\n");
    pgcont = (PGOutputContour*)pgcont->GetNext();
    cont = cont->NextContour();
  }
  ENDLOG;
}

