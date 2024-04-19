/* @(#)Z 1.6 com/src/imaging/polyclip/PGPoly.h, odimaging, od96os2, odos29646d 96/11/15 15:29:17 (96/10/29 09:26:42) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
 *
 *   CLASSES:   PGContourList
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
  File:    PGPoly.h

  Contains:  Polygon structs for the clipper.

  Owned by:  Jens Alfke (based on algorithm by A. C. Kilgour)

  Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <4>   5/25/95  jpa    List.h --> LinkList.h [1253324]
     <3>   9/29/94  RA    1189812: Mods for 68K build.
     <2>    9/9/94  jpa    Changed _Log(ODPoint) to somPrintf(ODPoint)
                  due to redefinition of LOG macro.
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <1>    5/9/94  jpa    first checked in

  In Progress:
*/

#ifndef _PGPOLY_
#define _PGPOLY_

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PGCOMMON_
#include "PGCommon.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
struct ODPolygon;
struct ODContour;
#endif

struct PGEvent;
struct PGEventQueue;
struct PGEdge;

class PGContourList;


PGSense Compare( const ODPoint*, const ODPoint* );

void somPrintf( const ODPoint& );      // Overload of somPrintf, used only when logging


// A PGVertex is a point stored in a linked list (but not a LinkedList) owned by a PGContour.

struct PGVertex :public ODPoint
{
  public:
    PGVertex( const ODPoint*, PGVertex *prev );

    PGVertex*  GetPrevious( )  const  {return fPrevious;}
    PGVertex*  GetNext( )  const    {return fNext;}

    void    Reverse( );
    void    JamNext( PGVertex* );

    inline ODPoint& AsPoint( )        {return *(ODPoint*)this;}
    inline const ODPoint& AsPoint( ) const  {return *(const ODPoint*)this;}

//  private:
    PGVertex*  fPrevious;
    PGVertex*  fNext;
};


// A PGContour is a contour of a polygon. It's a linked list of ODVertexes, but it's
// not a descendent of LinkedList since it doesn't use a 'sentinel'. The vertices
// form a pure circular list with no interruptions.
// PGContours are themselves strung together into lists, so they inherit from Link.

struct PGContour :public Link
{
  public:
    PGContour( LinkedList *onList );
    virtual ~PGContour( );

    PGContour*  InitContour( const ODContour*, ODBoolean reverse );
    void    AddAllEvents( PGEventQueue& );

    PGVertex*  First( )      {return fFirst;}
    PGVertex*  Last( )        {return fLast;}
    ODSLong  CountVertices( )  {return fNVertices;}

  protected:
    PGVertex*  fFirst;        // First vertex in list
    PGVertex*  fLast;        // Last vertex (not linked to 1st in PGOutputContours!)
    ODSLong  fNVertices;      // Number of vertices in list
};


struct PGOutputContour :public PGContour
{
  public:
    PGOutputContour( const PGEdge *left, const PGEdge *right );

    void    AddVertex( PGSide, const ODPoint* );
    void    Close( );
    void    AppendContour( PGOutputContour *cont, PGSide toSide );

    ODSShort  fWrapNo;      // Wrap number of this contour
    PGSense    fSense;        // kPositive means clockwise

  // The following are not used by the clipper, but by Rgn2PlyM.cpp.
  // Both take QuickDraw coordinates:
    PGOutputContour( short x, short y );
    void    AddVertex( PGSide, short x, short y );
};


// kIgnoredOutput is a special output-contour-pointer used to indicate that
// there is an output contour but it is not needed (due to its wrap number.)
#define kIgnoredOutput ((PGOutputContour*)0xFFFFFFFF)


class PGContourList :public LinkedList
{
  public:
    void    ReadPolygon( const ODPolygon&, ODBoolean reverse, PGEventQueue& );
    void    BuildPolygon( ODPolygon& );
};


#endif /*_PGPOLY*/


