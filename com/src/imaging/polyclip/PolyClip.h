/* @(#)Z 1.5 com/src/imaging/polyclip/PolyClip.h, odimaging, od96os2, odos29646d 96/11/15 15:25:09 (96/10/29 09:26:51) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
 *
 *   CLASSES: none
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

/*
  File:    PolyClip.h

  Contains:  The polygon clipper! Performs intersection, union, difference.

  Written by:  Jens Alfke (based on algorithm by A. C. Kilgour)

  Copyright:  © 1994 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):
  
     <3>  10/24/94  jpa    Added PolyOutset. [1186719]
     <2>    9/9/94  jpa    Added kShapeOutset (not yet implemented...)
                  [1178690]
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <1>    5/9/94  jpa    first checked in
  
  Theory Of Operation:
    PolyClip( ) performs Boolean operations on any number of polygons.
    Pass the number of polygons, an array of pointers to them, and the
    operation you want performed:
     kShapeIntersection computes the intersection of all the polygons.
     kShapeUnion computes the union of all the polygons.
     kShapeDifference subtracts all the other polygons from the first.
    
    PolySimplify( ) "simplifies" an input polygon by removing all
    overlaps or self-intersections. F'rinstance, given a pentagram it
    would return a five-pointed star. (This actually just calls
    PolyClip with nPolys=1 and op=kShapeUnion.)
    
    PolyOutset( ) outsets the vertices of a polygon by a given distance,
    or insets them if the distance is negative. The result may need
    simplification, since parts of contours may flip over. The function
    returns True if additional simplification may be needed.
    
    ** PolyClip is _not_ guaranteed to operate properly on self-
    intersecting input polygons for operations other than union.
    If this is a problem, just simplify the inputs beforehand.
    
    ** PolyClip uses winding-rule containment. This means that input
    vertices must be ordered such that positive contours go clockwise
    and holes go counterclockwise, as viewed in a coordinate system
    where the positive Y axis extends downward (i.e. in the Macintosh
    coordinate system, but not that of PostScript or traditional
    computer graphics.)
*/


#ifndef _POLYCLIP_
#define _POLYCLIP_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


typedef enum {
  kShapeIntersection,
  kShapeUnion,
  kShapeDifference,
  
  // For use internally by ODShape: do not pass to PolyClip:
  kShapeOutset,
  kShapeNoOp = -1
} ODShapeOp; 


void    PolyClip( ODSLong nPolys, const ODPolygon* polys[], ODShapeOp op, ODPolygon &result );
void    PolySimplify( ODPolygon &dstPoly, const ODPolygon &srcPoly );
// It's okay to use the same ODPolygon for input and output.

ODBoolean  PolyOutset( ODPolygon &poly, ODCoordinate distance );  // True if needs simplifying

#endif /*_POLYCLIP_*/


