/* @(#)Z 1.6 com/src/imaging/polyclip/PGEdge.h, odimaging, od96os2, odos29646d 96/11/15 15:29:17 (96/10/29 09:26:24) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
 *
 *   CLASSES:   PGEdge
 *		PGEdgeTable
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
/*                                                                  */
/*  IBM Change History (most recent first):                         */
/*          3/21/96 jab     Merge with DR4.                         */
/*  <OS2>  08/22/95 aml     133968 Port DR3                         */
/*                                                                  */
/********************************************************************/
/*
  File:    PGEdge.h

  Contains:  Active-edge structure for the clipper.

  Owned by:  Jens Alfke (based on algorithm by A. C. Kilgour)

  Copyright:  © 1994 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <2>   5/25/95  jpa    List.h --> LinkList.h [1253324]
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <1>    5/9/94  jpa    first checked in

  Theory of Operation:
  In Progress:
*/

#ifndef _PGEDGE_
#define _PGEDGE_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _PGCOMMON_
#include "PGCommon.h"
#endif


class PGVertex;
class PGOutputContour;


// PGEdge represents an edge in the Active Edge Table.
class PGEdge :public Link
{
  public:
    PGEdge( const PGVertex *tail, const PGVertex *head );

    PGSide    TestPoint( const ODPoint& );  // Which side of line is pt on?
    ODBoolean  IntersectWith( const PGEdge*, ODPoint &sect );
    PGEdge*    ReplaceWithCopy( );        // Returns the copy

    void    StartPath( PGEdge *rightEdge, const ODPoint* );
    void    ExtendPath( PGEdge *lowerEdge, const ODPoint* );
    void    MergePaths( PGEdge*, const ODPoint* );

    ODSShort      fWrapNo;        // Wrap-number of my contour
    PGSense    const  fSense;          // kPositive if heading downward
    ODBoolean      fLowEvent;        // Has bottom been entered into event queue?
    ODBoolean      fMatched;        // Temp flag used in edge-matching
    ODBoolean      fLeftBundle,      // Bundled w/prev edge?
              fRightBundle;      // Bundled w/next edge?
    const PGVertex    *fHighVert,        // Top (min. y) vertex
              *fLowVert;        // Bottom (max. y) vertex
    ODPoint        fDelta;          // Dist from high to low vert. y >= 0!

    PGSide        fSide;          // Which side of output path?
    PGEdge           *fOtherSide;      // Edge on other side of output contour
    PGOutputContour    *fOutput;        // Output contour
};


class PGEdgeTable :public LinkedList
{
  public:
    void  Add( PGEdge* );            // Adds sorted

    void  InsertListBefore( PGEdgeTable&, PGEdge *before );
    void  AddRange( PGEdge *first, PGEdge *last, PGEdge *before );
    PGEdge*  RemoveRange( PGEdge *first, PGEdge *last );

    void  Unmatch( );
    PGEdge*  FirstUnmatched( );
    PGEdge*  LastUnmatched( );
    PGEdge*  UnmatchedBefore( const PGEdge* );
    PGEdge*  UnmatchedAfter( const PGEdge* );
};

#endif /*_PGEDGE_*/



