/* @(#)Z 1.8 com/src/imaging/polyclip/PGEvent.h, odimaging, od96os2, odos29646d 96/11/15 15:25:08 (96/10/29 09:26:34) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
 *
 *   CLASSES:   PGEvent
 *              PGEventQueue
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
/*          3/21/96 jab  Merge with DR4.                            */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    PGEvent.h

  Contains:  Geometric event structure for the clipper.

   Owned by:   Jens Alfke (based on algorithm by A. C. Kilgour)

   Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

       <3>   3/22/95 jpa      Added aet param to MergeWith [1230776]
     <2>   6/30/94  jpa    Added dummy destructor for PGEvent to
                  appease CodeWarrior compiler.
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <1>    5/9/94  jpa    first checked in
   
   In Progress:
      
*/

#ifndef _PGEVENT_
#define _PGEVENT_

#ifndef _EVENT_TEMPLATES_ 
#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif
#else
#include <stack.h>
#include <vector.h>
#endif 

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _EVENT_TEMPLATES_ 
#ifndef _PRIORTYQ_
#include "PriortyQ.h"
#endif
#endif

#ifndef _PGCOMMON_
#include "PGCommon.h"
#endif

#ifndef _PGEDGE_
#include "PGEdge.h"
#endif


class PGContour;
class PGEdge;
class PGEdgeTable;
class PGVertex;
class PGEventQueue;



#ifdef _EVENT_TEMPLATES_
class PGEvent :public ODPoint
#else
class PGEvent :public Sortable, public ODPoint
#endif   // _EVENT_TEMPLATES_ 
{
  public:
    PGEvent( );
    ~PGEvent( )    { }          // Keeps CodeWarrior happy
    PGEvent*  InitLocalMinEvent( const PGVertex* );
    PGEvent*  InitLowEvent( const PGEdge* );
    PGEvent*  InitIntersection( const PGEdge*, const PGEdge*,
                    const ODPoint &sect );

#ifndef _EVENT_TEMPLATES_
    virtual ODBoolean ComesBefore( const Sortable* ) const;    // From Sortable class
#endif

    void     MergeWith( PGEvent*, PGEdgeTable &aet );
    void    Process( PGEdgeTable&, PGEventQueue& );

  protected:
    void    ConnectEdgePaths( PGEdgeTable &upperEdges );
    void    ConnectAdjacentEdgePaths( PGEdgeTable &upperEdges, PGSense firstSense );

    PGEdge*    fFirstUpperEdge;    // 1st edge (in AET) of my upper-edge range
    PGEdge*    fLastUpperEdge;      // Last edge of upper-edge range
    PGEdgeTable  fLowerEdges;      // List of lower edges
};


#ifdef _EVENT_TEMPLATES_
     
struct less<PGEvent*> : binary_function<PGEvent*, PGEvent*, bool> {
    bool operator()(PGEvent*& ev1, PGEvent*& ev2) const {
     return (((ev1->y) != (ev2->y)) ? ((ev1->y)>(ev2->y)) : ((ev1->x)>(ev2->x)));}
}; 

// In the OS/2 port, templates were used to improve performance as 
// documented below.  In the DR4 port, we haven't implemented templates
// yet so the following ifdef chooses between the OS/2 template version
// and the DR4 version.  jab 4/12/96
 
// We use a template version of the priority queue class here so that we can
// declare the comparison operator inline, rather than using a virtual method.
// This improves the performance of the polygon clipper about 15%

class PGEventQueue : public priority_queue<vector<PGEvent*>, less<PGEvent*> >
{
  public:
    PGEventQueue( ODULong suggestedSize )
      : priority_queue<vector<PGEvent*>, less<PGEvent*> >()
      { } // Can't adjust the vector size with the exiting priority_queue constructors
    inline ODBoolean IsEmpty( ) const { return empty(); }
    inline void Add(PGEvent* T) { push(T); }
    void    AddIntersectionIfBelow( PGEdge*, PGEdge*, const PGEvent *curEv );
    PGEvent*  First( )  const  { return top(); }
    PGEvent*  RmvFirst( )      { value_type first = top();
				 pop();
				 return first; }
};

#else  // _EVENT_TEMPLATES_

class PGEventQueue :private PriorityQueue
{
  public:
    PGEventQueue( ODULong suggestedSize )
      :PriorityQueue(suggestedSize)
      { }

    PriorityQueue::IsEmpty;
    PriorityQueue::Add;
    
    void    AddIntersectionIfBelow( PGEdge*, PGEdge*, const PGEvent *curEv );
    
    PGEvent*  First( )  const  {return (PGEvent*)PriorityQueue::GetFirst();}
    PGEvent*  RmvFirst( )      {return (PGEvent*)PriorityQueue::RemoveFirst();}
};
#endif   // _EVENT_TEMPLATES_

#endif /*_PGEVENT_*/

