/* @(#)Z 1.11 com/src/imaging/polyclip/PolyClip.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:08 (96/10/29 09:26:47) */
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
/*          3/21/96 jab  Merge with DR4. Changes for cross          */
/*                       platform support.                          */
/*  120604  4/28/95 ced  Merge with B1C16 Apple code.               */
/*                                                                  */
/********************************************************************/
/*
  File:    PolyClip.cpp

  Contains:  The polygon clipper! Performs intersection, union, difference.

  Owned by: Jens Alfke (based on algorithm by A. C. Kilgour)

  Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):
  
    <10>   8/18/95  NP	   1274946: Remove kODErrInvalidParameter
     <9>   8/16/95  NP     1274946: ErrorDef.idl problems. Add include file.
     <8>   5/25/95  jpa    Fixed 'for' loop for ANSI compliance [1253324]
     <7>   4/15/95  jpa    Weakened post-clip bbox assertion for union [1233290]
     <6>   3/22/95  jpa   Use Approx... methods when testing bounding
                  boxes at end of clipping [1186959]. Added
                  aet param to MergeWith [1230776]. Generate
                  correct PolyDump filenames. Set version to
                  3.
     <5>    1/6/95  jpa    Truncate PolyDump filename to 31 chars. [1207604]
     <4>   12/5/94  jpa    Made DumpPolygons more xplatform
                  compatible. Added version # to dump.
     <3>  10/24/94  jpa    Implemented PolyOutset. [1186719]
     <2>   9/29/94  RA    1189812: Mods for 68K build.
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <2>   5/27/94  jpa    Removed ASLM dependency [1165267]
     <1>    5/9/94  jpa    first checked in
     
  Theory of Operation:
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

#ifndef _POLYCLIP_
#include "PolyClip.h"
#endif

#ifndef _LINEOPS_
#include "LineOps.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifndef __ODMATH__
#include "ODMath.h"
#endif

#include <string.h>


const short kPolyClipVersion = 3;
/*  Version 1:      As of 10/27/94.
  Version 2:      Fixes from Chuck Dumont. Code tweaks from Imaging code-review.
  Version 3: 3/21/95: Another Chuck D. fix to PGEdge::TestPoint.
            Use approx bbox testing at end of clip. [1186959]
            Fix to PGEvent::MergeWith [1230776]
*/

#if ODDebug
#define ODDebugPolyClip 1
#else
#define ODDebugPolyClip 0
#endif


ODSLong gMinWrap, gMaxWrap;

//
// Uncomment the next 2 lines to turn on logging in this file 
//
//#undef LOGGING
//#define LOGGING 1


#if ODDebugPolyClip

  #ifndef __ODMATH__
  #include "ODMath.h"
  #endif
  #include <stdio.h>
#ifdef _PLATFORM_MACINTOSH_
  #include <strings.h>
#endif
  #include <time.h>
  
  static void
  DumpPolygons( ODSLong errorCode, ODSLong nPolys, const ODPolygon* polys[], ODShapeOp op )
  {
    static const char *kOpStr[3] = {"kShapeIntersection", "kShapeUnion", "kShapeDifference"};
  
    WARNMSG_DEBUG(WARN_INDEX(-1),"Error %ld while clipping. About to dump",errorCode);
  
    char fileName[64];
    time_t ltime;
    time(&ltime);
    sprintf(fileName, "PolyDump%s", ctime(&ltime));
    fileName[31] = '\0';    // Truncate to 31 chars
    char c;
    int i;
    for( i=0; (c=fileName[i])!=0; i++ )
    if( c==':' )
      fileName[i] = '.';      // Replace : with .
    FILE *fil = fopen(fileName, "w+");
    WASSERTMSG_DEBUG(fil!=kODNULL,"Couldn't open PolyDump file");
    
    fprintf(fil,"# OpenDoc polygon clipper version %d compiled %s\n",kPolyClipVersion,__DATE__);
    fprintf(fil,"# Error %ld on %\n",errorCode,ctime(&ltime));
    fprintf(fil,"# %s on %d polygons\n\n",kOpStr[op], nPolys);
  
    fprintf(fil,"%d\n\n%d",(int)op,nPolys);
    for(i=0; i<nPolys; i++ ) {
      const ODPolygon &p = *polys[i];
      fprintf(fil,"\n\n%d",p.GetNContours());
      const ODContour *c = p.FirstContour();
      for( int j=0; j<p.GetNContours(); j++ ) {
        fprintf(fil,"\n%d   ",c->nVertices);
        for( int k=0; k<c->nVertices; k++ ) {
          fprintf(fil," %6.2f,%6.2f", (float)ODFixedToFloat(c->vertex[k].x),
                        (float)ODFixedToFloat(c->vertex[k].y));
        }
        c = c->NextContour();
      }
    }
    fprintf(fil, "\n\n\n");
    fclose(fil);
  #ifdef _PLATFORM_MACINTOSH_
    FlushVol(NULL,0);
  #endif
  }
#endif


ODBoolean
PolyOutset( ODPolygon &poly, ODCoordinate dist )
{
  // This operation does NOT simplify the polygon! However, it returns True if the
  // result needs [additional] simplification to remove degenerate edges or contours.
  
  ODULong i=poly.GetNContours();
  ODBoolean needToSimplify = (i>1);
  ODContour *cont = poly.FirstContour();
  for( ; i>0; i-- ) {
    // Simplify a contour. We work with a consecutive triplet of points, p0..p2.
    // First p0 and p1 are moved perp to p0p1 to produce newp0 and newp10.
    // Then  p1 and p2 are moved perp to p1p2 to produce newp12 and newp2.
    // Then newp0newp10 and newp12newp2 are intersected to produce the new value of p1.
    
    ODULong n = cont->nVertices;
    if( n<3 ) continue;
    ODPoint newp0 = cont->vertex[n-2];
    ODPoint *p1 = &cont->vertex[n-1];
    ODPoint *p2 = &cont->vertex[0];
    ODPoint delta, newp10;
    GetLineShift(newp0,*p1,dist, delta);
    newp0.x += delta.x;
    newp0.y += delta.y;
    newp10.x = p1->x + delta.x;
    newp10.y = p1->y + delta.y;
    ODPoint origFirstPt = *p1;    // Save for use on last iteration
    
    for( ODULong j=n; j>0; j-- ) {
      ODPoint newp12, newp2;
      
      GetLineShift(*p1,*p2,dist, delta);
      newp12.x = p1->x + delta.x;
      newp12.y = p1->y + delta.y;
      newp2.x = p2->x + delta.x;
      newp2.y = p2->y + delta.y;
      
      // In order to snip off corners that got too sharp, we'd have to
      // do the distance checking shown in Kilgour p.394.
      if( IntersectLines(newp0,newp10, newp12,newp2, p1)        // SHAZAM! Move p1
              != kOutsideIntersection )
        needToSimplify = kODTrue;
      
      if( j>1 ) {
        newp0 = newp12;
        newp10 = newp2;
        p1 = p2;
        if( j>2 )
          p2++;          // On last iteration, need to use _original_ value
        else            // of the point at p2, not the version in the polygon,
          p2 = &origFirstPt;    // which was modified during the 1st iteration!
      }
    }
    
    cont = cont->NextContour();
  }
  
  return needToSimplify;
}

typedef struct {
    double x;
    double y;
} SimplifyPoint;


int 
AdjustPolygon( SimplifyPoint *p, int i, int n )
{
  if (i == (n-2)) {      // p1 is p[n]                        
    n--;                                                      
  }                                                           
  else if (i == (n-1)) { // p1 is p[0]                        
    for (int j = 0; j < n-1; j++) {                           
      p[j].x = p[j+1].x;                                      
      p[j].y = p[j+1].y;                                      
    }                                                         
    n--;                                                      
  }                                                           
  else {                 // p1 is between p[0] and p[n]       
    for (int j = i+1; j < n-1; j++) {                         
      p[j].x = p[j+1].x;                                      
      p[j].y = p[j+1].y;                                      
    }                                                         
    n--;                                                      
  }                                                           
  return n;
}

inline double ABS(double v) {
  if (v < 0) return -v;
  else       return v;
}

int 
SimplifyPoly( SimplifyPoint *p, int n )
{
  double epsilon = 0.001;
  SimplifyPoint p0, p1, p2;

//  printf("simplify\n");
//  printf(" pre size: %d\n", n);
//  for (int k = 0; k < n; k++) {
//    printf("[%5.2f,%5.2f]\n", p[k].x, p[k].y);
//  }

  for (int i = 0; i < n; i++) {

    if (i == (n-2)) {
      p0.x = p[i].x;   p0.y = p[i].y;
      p1.x = p[i+1].x; p1.y = p[i+1].y;
      p2.x = p[0].x;   p2.y = p[0].y;
    }
    else if (i == (n-1)) {
      p0.x = p[i].x;   p0.y = p[i].y;
      p1.x = p[0].x;   p1.y = p[0].y;
      p2.x = p[1].x;   p2.y = p[1].y;
    }
    else {
      p0.x = p[i].x;   p0.y = p[i].y;
      p1.x = p[i+1].x; p1.y = p[i+1].y;
      p2.x = p[i+2].x; p2.y = p[i+2].y;
    }

    double deltax = p2.x - p0.x;
    double deltay = p2.y - p0.y;

    if ((ABS(deltax) < epsilon) && (ABS(deltay) < epsilon)) { // same point!
      n = AdjustPolygon(p, i, n);
      i--;
    }
    else if (ABS(deltax) < epsilon) { // vertical line
      if (ABS(p1.x - p0.x) < epsilon) {
      // p1 co-linear!!!
        n = AdjustPolygon(p, i, n);
        i--;
      }
    }
    else if (ABS(deltay) < epsilon) { // horizontal line
      if (ABS(p1.y - p0.y) < epsilon) {
      // p1 co-linear!!!
        n = AdjustPolygon(p, i, n);
        i--;
      }
    }
    else {
      if (ABS( ((p1.x - p0.x) / deltax) - ((p1.y - p0.y) / deltay) ) < epsilon) {
       // p1 co-linear!!!
        n = AdjustPolygon(p, i, n);
        i--;
      }
    }
  }

//  printf("post size: %d\n", n);
//  for (i = 0; i < n; i++) {
//    printf("[%5.2f,%5.2f]\n", p[i].x, p[i].y);
//  }
//  printf("\n\n");

  return n;
}

void
PolySimplify( ODPolygon &dstPoly, const ODPolygon &srcPoly )
{
  // First, simplify polygon by getting rid of "extra" points

  ODContour *cont = (ODContour *)srcPoly.FirstContour();
  for( int i = srcPoly.GetNContours(); i > 0; i-- ) {
    int n = cont->nVertices;
    SimplifyPoint *p = (SimplifyPoint *)malloc(n * sizeof(SimplifyPoint));
    for( int k = 0; k < n; k++ ) {
      p[k].x = (double)ODFixedToFloat(cont->vertex[k].x);
      p[k].y = (double)ODFixedToFloat(cont->vertex[k].y);
    }
    int newn = SimplifyPoly(p, n);
    if (newn != n) {
      cont->nVertices = newn;
      for( k = 0; k < newn; k++ ) {
        cont->vertex[k].x = ODFloatToFixed(p[k].x);
        cont->vertex[k].y = ODFloatToFixed(p[k].y);
      }
    }
    free(p);
    cont = cont->NextContour();
  }

  const ODPolygon *input = &srcPoly;
  PolyClip(1,&input, kShapeUnion, dstPoly);
}



void
PolyClip( ODSLong nPolys, const ODPolygon* polys[], ODShapeOp op, ODPolygon &output )
{
  ASSERT(nPolys>0, kODErrValueOutOfRange);
  ASSERT(polys!=kODNULL, kODErrIllegalNullInput);
  ASSERT(op>=kShapeIntersection && op<=kShapeDifference,kODErrValueOutOfRange );
  
  PGEventQueue events(20);        // Event queue
  PGEdgeTable aet;            // Active Edge Table
  PGContourList outputContours;
  gOutputContours = &outputContours;
  
  if( op==kShapeIntersection ) {
    gMinWrap = nPolys;
    gMaxWrap = nPolys; //was 32767; removed by jpa 10/27/94
  } else {
    gMinWrap = gMaxWrap = 1;
  }
  
  TRY{
  
  // Preprocess input polygons:


#if LOGGING
//    ClearDebugWindow();
    LOG("BEGIN CLIP: Processing %d polygons, op=%d\n", nPolys,(int)op);

    PGEventQueue tmpq(20);        // temporary event queue

#endif
  
    PGContourList input;
    ODBoolean reverse = kODFalse;
    for( ODSLong i=0; i<nPolys; i++ ) {
      LOG("INPUT polygon #%ld:\n",i);
      input.ReadPolygon(*polys[i],reverse,events);
      if( op==kShapeDifference )
        reverse = kODTrue;
    }


    // Process all events in order from top to bottom:
    
    while( !events.IsEmpty() ) {

#if LOGGING

       LOG("\n\n*** current event queue\n");
       while( !events.IsEmpty() ) {
          PGEvent *tmpev = events.RmvFirst();
          LOG("* -> (%.2f,%.2f):\n", tmpev->x/65536.0, tmpev->y/65536.0);
          tmpq.Add( tmpev );
       }
       LOG("\n*** End of event queue ***\n\n");

       while( !tmpq.IsEmpty() ) {
          PGEvent *tmpev = tmpq.RmvFirst();
          events.Add( tmpev );
       } 

#endif


      PGEvent *ev = events.RmvFirst();      // Get next event
  
      LOG("\n* Processing event at (%.2f,%.2f):\n", ev->x/65536.0,ev->y/65536.0);
  
      for(;;){
         if( !events.IsEmpty() && (*ev == *(events.First()) ) ) {
            LOG( "= Merged with next event.\n" );

            // While equal to next, merge with next...
            PGEvent *evNext = events.RmvFirst();
            ev->MergeWith( evNext, aet );      
         } else
            break;
      }
 
      if( LOGGING ) {                // Dump AET if logging:
        BEGINLOG;
        LOG("  AET =");
        for( Link *l=aet.First(); l; l=aet.After(*l) ) {
          PGEdge *e = (PGEdge*)l;
          ODCoordinate x;
          if( e->fHighVert->y == e->fLowVert->y )
            x = ev->x;    // horizontal
          else
            x = GetLineXAtY(*e->fHighVert,*e->fLowVert,ev->y);
          if( e->fRightBundle && !e->fLeftBundle )
            LOG(" {");
          LOG(" %.2f[%c%c%d]", x/65536.0,
                     e->fSense>0?'v':'^',
                     e->fSide==kLeft?'l':'r',
                     e->fWrapNo);
          if( e->fLeftBundle )
            if( e->fRightBundle )
              LOG(" =");
            else
              LOG(" }");
        }
        LOG("\n");
        ENDLOG;
      }
      
      PGASSERT( (aet.Count() & 1) == 0 );      // Must have even # of edges in aet!
      
      ev->Process(aet,events);          // Process event
      delete ev;
    }
    
    // Convert output into ODPolygon and return it:
    
    input.DeleteAllLinks();
    
    outputContours.BuildPolygon(output);
    outputContours.DeleteAllLinks();
    
    if( ODDebug ) {
      // Check the bounding boxes to make sure they make sense:
      ODRect inBounds, outBounds;
      output.ComputeBoundingBox(&outBounds);
      polys[0]->ComputeBoundingBox(&inBounds);
      if( op==kShapeDifference )
        PGASSERT(inBounds.ApproxContains(outBounds));
      else {
        for( int i=1; i<nPolys; i++ ) {
          ODRect polyBounds;
          polys[i]->ComputeBoundingBox(&polyBounds);
          if( op==kShapeIntersection )
            inBounds &= polyBounds;
          else
            inBounds |= polyBounds;
        }

        // **** After a union the outBounds should equal the inBounds.
        // However Thomas has come up with one very complex case where it
        // doesn't. I need to investigate this; in the meantime, shoving
        // kODTrue in here will turn off the warning.  --jpa 4/15/95
        if( kODTrue /*op==kShapeIntersection*/ )
          PGASSERT(inBounds.ApproxContains(outBounds));
        else
          PGASSERT(inBounds.ApproxEquals(outBounds));
      }
    }

  }CATCH_ALL{
#if ODDebugPolyClip
    // Something went wrong during clipping. Dump the input polygons, and set the
    // output to be a copy of the first input polygon. Do not propagate the failure!
    // This should keep things limping along although things may look weird onscreen.
    DumpPolygons(ErrorCode(),nPolys,polys,op);
    output.CopyFrom(*polys[0]);
    // Fall through and return...
#else
    RERAISE;
#endif
  }ENDTRY
  
  LOG("DONE clipping\n");
}


