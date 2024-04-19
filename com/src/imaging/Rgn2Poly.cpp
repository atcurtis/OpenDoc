/* @(#)Z 1.11 os2/src/imaging/Rgn2Poly.cpp, odimaging, od96os2, odos29646d 96/11/15 15:49:50 (96/10/24 10:30:48) */
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
  File:    Rgn2Poly.cpp   

  Contains:  Converting a platform-specific region into an ODPolygon.

  Written by:  Chuck Dumont  (tip o' the hat to Jens Alfke at Apple)

  Theory Of Operation:

    This implementation of Rgn2Poly uses the array of rectangles returned
    by GpiQueryRegionRects to directly construct an ODPolygon.  This
    algorithm makes use of the fact that all of the contours of the
    polygon are contained in the sides of the region rectangles.  Because
    of the way the rectangles are layed out in scan line order, every
    vertical side of a rectangle is also a contour of the polygon.  All
    that remains to do is determine what portions of the horizontal sides
    of the rectangles contribute to the horizontal contours of the polygon.
*/

#ifdef _PLATFORM_OS2_
#define INCL_GPI
#endif

#ifdef _PLATFORM_WIN32_
#include <wtypes.h>
#include <wingdi.h>
#endif

#ifndef _ALTPOINT_
#include "AltPoint.h"      // Use C++ savvy ODPoint and ODRect
#endif


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif
#endif   // IBM Platforms

#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifdef _PLATFORM_OS2_
#include <os2.h>      // [129073] - ced
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _RGN2PLYM_
#include "Rgn2PlyM.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODMATH_
#include "ODMath.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifdef _PLATFORM_UNIX_
#ifndef _REALSHPE_
#include <RealShpe.h>
#endif
#endif

#include <limits.h>
#include <stdio.h>



//
// The following redefine of ODDebug was necessary as a temporary solution
// to allow ASSERT, WARN, and other macros to be expanded.  Some conditions,
// such as passing a NULL shape as input, were not detected at runtime and
// the proper exceptions were not thrown.  The reason for this was that
// the macros, as defined in ODDebug.h, are dependent on ODDebug being
// equal to 1.  One of the system wide makefile defines ODDebug to 0.  The
// following should override ODDebug and set it to 1.  This code was put
// into this header file because it was common to the rest of the source
// in the polyclip directory.  This code can be removed when the macros in
// ODDebug.h are redesigned.  jab - 4/22/96.
//
#ifdef DEBUG
#undef ODDebug
#define ODDebug 1
#endif

//=============================================================================
// Functon Prototypes
//=============================================================================
#if ODImageDebug

void DumpPolygon(const ODPolygon &p);

#ifdef _PLATFORM_OS2_
void DumpRegion(HPS hps, HRGN hrgn);
#endif

#ifdef _PLATFORM_WIN32_
void DumpRegion( HRGN hrgn );
#endif

#ifdef _PLATFORM_UNIX_
void DumpRegion( ODRgnHandle rgn );
#endif

#endif  // ODImageDebug


//=============================================================================
// Types & Constants
//=============================================================================


typedef ODSLong ContourID;
const ContourID kNoContour = -1;


//=============================================================================
// RgnNode Struct
//=============================================================================


struct RgnNode :public Link {
  public:
    enum End {Head, Tail};
    enum Orientation {NorthSouth, EastWest, UnKnown};
    ODSLong  fX, fY;        // Coordinates of this vertex

          RgnNode( long x, long y );
    virtual    ~RgnNode( )  { }
    void    AddToContour( RgnNode* );
    void    MergeContours( RgnNode*, LinkedList &closed );
    ODSLong  CountVertices( );
    void    DeleteAllNodes( );
    End     WhichEnd() {return (fNextVertex ? Tail : Head); }
    Orientation GetOrientation( );

    static void BuildPolygon( LinkedList &output, ODPolygon& );

  private:
    RgnNode    *fNextVertex,    // Next vertex in output contour
               *fPrevVertex;    // Previous vertex in output contour
    RgnNode    *fOtherEnd;      // Other end of partial contour
    ContourID   fContourID;     // Sequential contour ID
};



RgnNode::RgnNode( long xx, long yy )
  :fX(xx), fY(yy),
   fPrevVertex(kODNULL),
   fNextVertex(kODNULL),
   fOtherEnd(this)
{
  static ContourID nextContourID = 0;

  this->fContourID = nextContourID++;
}



void
RgnNode::AddToContour( RgnNode *node )
{
  // Set my contour ID to be the same as node's:
  WASSERT(fContourID != node->fContourID);
  fContourID = node->fContourID;

  WASSERT(this->fNextVertex == kODNULL && this->fPrevVertex == kODNULL);
  WASSERT(!(node->fNextVertex != kODNULL && node->fPrevVertex != kODNULL));
  // Link myself in the contour before or after node:
  if( node->fNextVertex != kODNULL) {
    node->fPrevVertex = this;
    this->fNextVertex = node;
  } else {
    node->fNextVertex = this;
    this->fPrevVertex = node;
  }
  // Connect myself to the other end of the contour:
  this->fOtherEnd = node->fOtherEnd;
  this->fOtherEnd->fOtherEnd = this;
}



void
RgnNode::MergeContours( RgnNode *node, LinkedList &output )
{
  WASSERT(this->GetNext()==kODNULL);
  WASSERT(node->GetNext()==kODNULL);

  if( fContourID == node->fContourID ) {  // Closing a contour:
    if( fPrevVertex == kODNULL )
      output.AddLast(this);        // Add first link to output list
    else
      output.AddLast(node);

  } else {                // Merging two contours together:
    if( this->fPrevVertex == kODNULL ) {
      WASSERT(node->fNextVertex == kODNULL);
      node->fNextVertex = this;
      this->fPrevVertex = node;
    } else {
      WASSERT(node->fPrevVertex == kODNULL);
      WASSERT(this->fNextVertex == kODNULL);
      node->fPrevVertex = this;
      this->fNextVertex = node;
    }

    // Fix up the fContourID of this contour (actually just other end):
    this->fOtherEnd->fContourID = node->fContourID;

    // Introduce the remaining ends to each other:
    this->fOtherEnd->fOtherEnd = node->fOtherEnd;
    node->fOtherEnd->fOtherEnd = this->fOtherEnd;
  }
}



RgnNode::Orientation
RgnNode::GetOrientation()
{
  if (fPrevVertex) {
     WASSERT(fX == fPrevVertex->fX || fY == fPrevVertex->fY);
     return (fPrevVertex->fX == fX) ? NorthSouth : EastWest;
  } else if (fNextVertex) {
     WASSERT(fX == fNextVertex->fX || fY == fNextVertex->fY);
     return (fNextVertex->fX == fX) ? NorthSouth : EastWest;
  } else
     return UnKnown;
}



ODSLong
RgnNode::CountVertices( )
{
  WASSERT(fPrevVertex==kODNULL);    // Only makes sense on 1st node in contour
  ODSLong count = 0;
  for( RgnNode *n=this; n; n=n->fNextVertex )
    count++;
  return count;
}



void
RgnNode::DeleteAllNodes( )
{
  // Delete myself and all the following nodes:

  WASSERT(fPrevVertex==kODNULL);    // Only makes sense on 1st node in contour
  RgnNode *n, *next;
  for( n=this; n; n=next ) {
    next = n->fNextVertex;
    delete n;
  }
}



void
RgnNode::BuildPolygon( LinkedList &output, ODPolygon &poly ) /* static method */
{
  // Build an ODPolygon from a list of contours, deleting the contours:

  ODSLong nContours = 0;
  ODSLong *nVertices = new ODSLong[output.Count()];

  RgnNode *out, *next;
  for( out=(RgnNode*)output.First(); out; out=next ) {
    next = (RgnNode*)output.After(*out);
    ODSLong n = out->CountVertices();
    WASSERT(n>=3);
    nVertices[nContours++] = n;
  }

  if( nContours==0 ) {
    delete nVertices;
    poly.Clear();
    return;
  }

  ODContour *cont;
  ODError err = 0;
  TRY{
    poly.SetContours(nContours,nVertices);
    cont = poly.FirstContour();
  }CATCH_ALL{
    poly.Clear();
    // Don't reraise, keep going:
  }ENDTRY
  delete nVertices;

  out = (RgnNode*)output.First();
  for( ODSLong i=nContours; i>0; i-- ) {
    RgnNode *next = (RgnNode*)out->GetNext();
    out->Remove();
    if( poly.HasData() ) {
      RgnNode *v = out;
      for( ODSLong j=0; j<cont->nVertices; j++ ) {
        cont->vertex[j].Set(ODIntToFixed(v->fX),ODIntToFixed(v->fY));
        v = v->fNextVertex;
      }
      cont = cont->NextContour();
    }
    out->DeleteAllNodes();      // Delete input as we go along
    out = next;
  }

  THROW_IF_ERROR(err);
}



#ifdef _PLATFORM_OS2_
int GetRegionRects(HPS hps, HRGN hrgn, PRECTL *pprcl)
{
   RGNRECT rgnrcControl;
   PRECTL prcl;
   BOOL fResult;
   int i, nRects, nBytes;

   /*--------------------------------------------------+
   | Determine the number of rectangles in the region. |
   +--------------------------------------------------*/

   rgnrcControl.crc = 100;
   rgnrcControl.ulDirection = RECTDIR_LFRT_TOPBOT;
   rgnrcControl.ircStart = 1;
   nBytes = rgnrcControl.crc * sizeof(RECTL);
   if (!(prcl = (PRECTL)malloc(nBytes)))
      THROW(kODErrOutOfMemory);

   while (0 != (fResult = GpiQueryRegionRects(hps, hrgn, NULL,
                                 &rgnrcControl,
                                 prcl + rgnrcControl.ircStart - 1)) &&
          rgnrcControl.crcReturned == rgnrcControl.crc)
   {
      nBytes += (rgnrcControl.crc * sizeof(RECTL));
      if (!(prcl = (PRECTL)realloc(prcl, nBytes)))
         THROW(kODErrOutOfMemory);
      rgnrcControl.ircStart += rgnrcControl.crc;
   };
   nRects = rgnrcControl.ircStart + rgnrcControl.crcReturned - 1;
   /*-----------------------------------------------+
   | If there was an error then raise an exception. |
   +-----------------------------------------------*/
   if (!fResult) {
      if (prcl) free(prcl);
//
//  Not sure what error to throw here.  kODErrPMError does not exist
//  (and may not make sense) in the cross-platform OpenDoc product.
//      THROW(kODErrPMError);
      *pprcl = NULL;
      return 0;
   }
   *pprcl = prcl;
   return nRects;
}
#endif  // _PLATFORM_OS2_




#ifdef _PLATFORM_WIN32_
int GetRegionRects( HRGN hrgn, LPRGNDATA *pprcl )
{
   LPRECT prcl;
   BOOL fResult;
   int i, nRects, nBytes;

   /*--------------------------------------------------+
   | Determine the number of rectangles in the region. |
   +--------------------------------------------------*/

   LPRGNDATA lpRgn;

   // TODO: Should do some error checking here.  If dwSize = 0
   //       then an error occurred, dwSize = 1 if successful,
   //       and otherwise equal to the size, in bytes, of the
   //       required buffer.

   DWORD dwSize = GetRegionData( hrgn, 0, NULL );

   if( dwSize > 1 ) {
      if( !( lpRgn = (LPRGNDATA) malloc( dwSize ) ) )
         THROW( kODErrOutOfMemory );
      GetRegionData( hrgn, dwSize, lpRgn );

      nRects = lpRgn->rdh.nCount;
   }
   else {
         return 0;
   }
   *pprcl = lpRgn;
   return nRects;
}
#endif  // _PLATFORM_WIN32_



RgnNode*
GatherNodes(RgnNode* node, int xLimit, LinkedList& output)
{
   // Merges pairs of nodes starting with the specified node, upto and
   // including all nodes who's X value is less than xLimit.  Returns
   // the new current node.

   RgnNode *next, *newNext;

   while( node->fX < xLimit && ((RgnNode*)node->GetNext())->fX <= xLimit) {
     next = (RgnNode*)node->GetNext();
     newNext = (RgnNode*)next->GetNext();
     node->Remove();
     next->Remove();
     next->MergeContours(node, output);
     node = newNext;
   }
   return node;
}




//=============================================================================
// Rgn2Poly Routine (OS/2)
//=============================================================================
#ifdef _PLATFORM_OS2_

void
Rgn2Poly( HPS hps, ODRgnHandle rgn, ODPolygon &poly )
{
  RgnNode *node, *next, *tmpNode, *newNode;
  unsigned i=0, nRects;
  RECTL rcl, *prcl;

  nRects = GetRegionRects(hps, rgn, &prcl);
  if (!nRects) {
     poly.Clear();
     return;
  }

  LinkedList active;
  RgnNode sentinel (INT_MAX, INT_MAX);            // Always at end of list
  active.AddLast( &sentinel );

  LinkedList output;            // List of output contours (RgnNodes)

  i = 0;
  rcl = prcl[i++];
  while (i <= nRects) {

    node = (RgnNode*)active.First();

    if (node->fY < INT_MAX && node->fY != rcl.yTop)
      node = GatherNodes(node, INT_MAX, output);

    unsigned yCurrentBottom = rcl.yBottom;

    while (i <= nRects && rcl.yBottom == yCurrentBottom) {

      node = GatherNodes(node, rcl.xLeft, output);

      if (node->fX < rcl.xRight) {
         next = (RgnNode*)node->GetNext();
         if (node->fX == rcl.xLeft && node->GetOrientation() == RgnNode::NorthSouth) {
            node->fY = rcl.yBottom;
         } else {
            node->Remove();
            newNode  = new RgnNode(rcl.xLeft, rcl.yBottom);
            unsigned yTop = rcl.yTop;
            tmpNode  = new RgnNode(rcl.xLeft, yTop);
            tmpNode->AddToContour(newNode);
            tmpNode->MergeContours(node, output);
            newNode->AddBefore(next);
         }
         node = next;

         node = GatherNodes(node, rcl.xRight, output);

         next = (RgnNode*)node->GetNext();
         if (node->fX == rcl.xRight && node->GetOrientation() == RgnNode::NorthSouth) {
            node->fY = rcl.yBottom;
         } else if (node->fX <= rcl.xRight) {
            node->Remove();
            newNode  = new RgnNode(rcl.xRight, rcl.yBottom);
            unsigned yTop = rcl.yTop;
            tmpNode = new RgnNode(rcl.xRight, yTop);
            newNode->AddToContour(tmpNode);
            tmpNode->MergeContours(node, output);
            newNode->AddBefore(next);
         } else {
            newNode  = new RgnNode(rcl.xRight, rcl.yBottom);
            tmpNode  = new RgnNode(rcl.xRight, rcl.yTop);
            newNode->AddToContour(tmpNode);
            newNode->AddBefore(node);
            tmpNode->AddBefore(node);
            next = tmpNode;
         }
         node = next;

      } else {
         tmpNode = new RgnNode(rcl.xLeft, rcl.yBottom);
         tmpNode->AddBefore(node);
         newNode = new RgnNode(rcl.xLeft, rcl.yTop);
         newNode->AddToContour(tmpNode);
         tmpNode = newNode;
         newNode = new RgnNode(rcl.xRight, rcl.yTop);
         newNode->AddToContour(tmpNode);
         tmpNode = newNode;
         newNode = new RgnNode(rcl.xRight, rcl.yBottom);
         newNode->AddToContour(tmpNode);
         newNode->AddBefore(node);
      }
      rcl = prcl[i++];
      WASSERT((active.Count() & 1) == 1);  // Always have an even # of nodes + sentinal
    }
    GatherNodes(node, INT_MAX, output);
  }
  node = (RgnNode*)active.First();
  GatherNodes(node, INT_MAX, output);

  WASSERT(active.Count()==1);    // Nothing left but sentinal
  free(prcl);

  RgnNode::BuildPolygon(output,poly);

#ifdef GPIPOLYGONBUGFIXED
#if ODImageDebug
    GpiQueryRegionBox(hps, rgn, &rcl);
    ODRect polyBounds;
    poly.ComputeBoundingBox(&polyBounds);
    WASSERT(polyBounds.left==ODIntToFixed(rcl.xLeft));
    WASSERT(polyBounds.right==ODIntToFixed(rcl.xRight));
    WASSERT(polyBounds.top==ODIntToFixed(rcl.yTop));
    WASSERT(polyBounds.bottom==ODIntToFixed(rcl.yBottom));

    HRGN testRgn = poly.AsRegion(hps);
    LONG lEquality = GpiEqualRegion(hps, testRgn, rgn);
    if (lEquality != EQRGN_EQUAL) {
       printf("Error in RgnToPoly: output region not equal to input region\n");
       printf("Dumping input region\n");
       DumpRegion(hps, rgn);
       printf("Dumping output region\n");
       DumpRegion(hps, testRgn);
       printf("Dumping output polygon\n");
       DumpPolygon(poly);
    }
    WASSERT(lEquality == EQRGN_EQUAL);
    GpiDestroyRegion(hps, testRgn);

#endif // ODImageDebug
#endif // GPIPOLYGONBUGFIXED
}

#endif   // _PLATFORM_OS2_




//=============================================================================
// Rgn2Poly Routine (Windows)
//=============================================================================

#ifdef _PLATFORM_WIN32_
void
Rgn2Poly( ODRgnHandle rgn, ODPolygon &poly )
{
   RgnNode *node, *next, *tmpNode, *newNode;
   int i = 0, nRects;
   LPRGNDATA pRgnData;
   RECTL rcl, *prcl;

   nRects = GetRegionRects( rgn, &pRgnData );
   if ( !nRects ) {
      poly.Clear();
      return;
   }

   LinkedList active;
   RgnNode sentinel (INT_MAX, INT_MAX);            // Always at end of list
   active.AddLast( &sentinel );

   LinkedList output;            // List of output contours (RgnNodes)

   i = 0;
   prcl = (RECTL*)pRgnData->Buffer;
   rcl = prcl[i++];
   while ( i <= nRects ) {

      node = (RgnNode*)active.First();

      if ( node->fY < INT_MAX && node->fY != rcl.top )
         node = GatherNodes( node, INT_MAX, output );

      unsigned CurrentBottom = rcl.bottom;

      while ( i <= nRects && rcl.bottom == CurrentBottom) {

         node = GatherNodes( node, rcl.left, output );

         if ( node->fX < rcl.right ) {
            next = (RgnNode*)node->GetNext();
            if ( node->fX == rcl.left && node->GetOrientation() == RgnNode::NorthSouth ) {
               node->fY = rcl.bottom;
            } else {
               node->Remove();
               newNode  = new RgnNode( rcl.left, rcl.bottom );
               unsigned Top = rcl.top;
               tmpNode  = new RgnNode( rcl.left, Top );
               tmpNode->AddToContour( newNode );
               tmpNode->MergeContours( node, output );
               newNode->AddBefore( next );
            }
            node = next;

            node = GatherNodes( node, rcl.right, output );

            next = (RgnNode*)node->GetNext();
            if ( node->fX == rcl.right && node->GetOrientation() == RgnNode::NorthSouth ) {
               node->fY = rcl.bottom;
            }  else if ( node->fX <= rcl.right ) {
               node->Remove();
               newNode  = new RgnNode( rcl.right, rcl.bottom );
               unsigned Top = rcl.top;
               tmpNode = new RgnNode( rcl.right, Top);
               newNode->AddToContour( tmpNode );
               tmpNode->MergeContours( node, output );
               newNode->AddBefore( next );
            } else {
               newNode  = new RgnNode( rcl.right, rcl.bottom );
               tmpNode  = new RgnNode( rcl.right, rcl.top );
               newNode->AddToContour( tmpNode );
               newNode->AddBefore( node );
               tmpNode->AddBefore( node );
               next = tmpNode;
            }
            node = next;

         } else {
            tmpNode = new RgnNode( rcl.left, rcl.bottom );
            tmpNode->AddBefore( node );
            newNode = new RgnNode( rcl.left, rcl.top );
            newNode->AddToContour( tmpNode );
            tmpNode = newNode;
            newNode = new RgnNode( rcl.right, rcl.top );
            newNode->AddToContour( tmpNode );
            tmpNode = newNode;
            newNode = new RgnNode( rcl.right, rcl.bottom );
            newNode->AddToContour( tmpNode );
            newNode->AddBefore( node );
         }
         rcl = prcl[i++];
         WASSERT( (active.Count() & 1 ) == 1 );  // Always have an even # of nodes + sentinal
      }
      GatherNodes( node, INT_MAX, output );
   }
   node = (RgnNode*)active.First();
   GatherNodes( node, INT_MAX, output);

   WASSERT( active.Count() == 1 );    // Nothing left but sentinal
   free( pRgnData );

   RgnNode::BuildPolygon( output, poly );

#if ODImageDebug
    GetRgnBox( rgn, &rcl );
    ODRect polyBounds;
    poly.ComputeBoundingBox( &polyBounds );

    WASSERT( polyBounds.left == ODIntToFixed( rcl.left ) );
    WASSERT( polyBounds.right == ODIntToFixed( rcl.right ) );
    WASSERT( polyBounds.top == ODIntToFixed( rcl.top ) );
    WASSERT( polyBounds.bottom == ODIntToFixed( rcl.bottom ) );

    HRGN testRgn = poly.AsWinRegion();
    BOOL lEquality = EqualRgn( testRgn, rgn );

    if ( !lEquality ) {
       printf( "Error in RgnToPoly: output region not equal to input region\n" );
       printf( "Dumping input region\n" );
       DumpRegion( rgn );
       printf( "Dumping output region\n" );
       DumpRegion( testRgn );
       printf( "Dumping output polygon\n" );
       DumpPolygon( poly );
    }
    WASSERT( lEquality );
    DeleteObject( testRgn );

#endif // ODImageDebug
}

#endif  // _PLATFORM_WIN32_



//=============================================================================
// Rgn2Poly Routine (UNIX)
//=============================================================================

#ifdef _PLATFORM_UNIX_
void
Rgn2Poly( ODRgnHandle rgn, ODPolygon &poly )
{
   RgnNode *node, *next, *tmpNode, *newNode;
   int i = 0, nRects;
   XRectangle *pRgnData;
   XRectangle rcl, *prcl;

   nRects = RealShape::GetRegionRects( rgn, &pRgnData );
   if ( !nRects ) {
      poly.Clear();
      return;
   }

   LinkedList active;
   RgnNode sentinel (INT_MAX, INT_MAX);            // Always at end of list
   active.AddLast( &sentinel );

   LinkedList output;            // List of output contours (RgnNodes)

   i = 0;
   prcl = pRgnData;
   rcl = prcl[i++];
   while ( i <= nRects ) {

      int r_top = rcl.y;
      int r_bottom = rcl.y+rcl.height;
      int r_left = rcl.x;
      int r_right = rcl.x+rcl.width;

      node = (RgnNode*)active.First();

      if ( node->fY < INT_MAX && node->fY != r_top )
         node = GatherNodes( node, INT_MAX, output );

      unsigned CurrentBottom = r_bottom;

      while ( i <= nRects && r_bottom == CurrentBottom) {

         node = GatherNodes( node, r_left, output );

         if ( node->fX < r_right ) {
            next = (RgnNode*)node->GetNext();
            if ( node->fX == r_left && node->GetOrientation() == RgnNode::NorthSouth ) {
               node->fY = r_bottom;
            } else {
               node->Remove();
               newNode  = new RgnNode( r_left, r_bottom );
               unsigned Top = r_top;
               tmpNode  = new RgnNode( r_left, Top );
               tmpNode->AddToContour( newNode );
               tmpNode->MergeContours( node, output );
               newNode->AddBefore( next );
            }
            node = next;

            node = GatherNodes( node, r_right, output );

            next = (RgnNode*)node->GetNext();
            if ( node->fX == r_right && node->GetOrientation() == RgnNode::NorthSouth ) {
               node->fY = r_bottom;
            }  else if ( node->fX <= r_right ) {
               node->Remove();
               newNode  = new RgnNode( r_right, r_bottom );
               unsigned Top = r_top;
               tmpNode = new RgnNode( r_right, Top);
               newNode->AddToContour( tmpNode );
               tmpNode->MergeContours( node, output );
               newNode->AddBefore( next );
            } else {
               newNode  = new RgnNode( r_right, r_bottom );
               tmpNode  = new RgnNode( r_right, r_top );
               newNode->AddToContour( tmpNode );
               newNode->AddBefore( node );
               tmpNode->AddBefore( node );
               next = tmpNode;
            }
            node = next;

         } else {
            tmpNode = new RgnNode( r_left, r_bottom );
            tmpNode->AddBefore( node );
            newNode = new RgnNode( r_left, r_top );
            newNode->AddToContour( tmpNode );
            tmpNode = newNode;
            newNode = new RgnNode( r_right, r_top );
            newNode->AddToContour( tmpNode );
            tmpNode = newNode;
            newNode = new RgnNode( r_right, r_bottom );
            newNode->AddToContour( tmpNode );
            newNode->AddBefore( node );
         }
         rcl = prcl[i++];

	 r_top = rcl.y;
	 r_bottom = rcl.y+rcl.height;
	 r_left = rcl.x;
	 r_right = rcl.x+rcl.width;

         WASSERT( (active.Count() & 1 ) == 1 );  // Always have an even # of nodes + sentinal
      }
      GatherNodes( node, INT_MAX, output );
   }
   node = (RgnNode*)active.First();
   GatherNodes( node, INT_MAX, output);

   WASSERT( active.Count() == 1 );    // Nothing left but sentinal
   free( pRgnData );

   RgnNode::BuildPolygon( output, poly );

#if ODImageDebug
    GetRgnBox( rgn, &rcl );
    ODRect polyBounds;
    poly.ComputeBoundingBox( &polyBounds );

    WASSERT( polyBounds.left == ODIntToFixed( r_left ) );
    WASSERT( polyBounds.right == ODIntToFixed( r_right ) );
    WASSERT( polyBounds.top == ODIntToFixed( r_top ) );
    WASSERT( polyBounds.bottom == ODIntToFixed( r_bottom ) );

    Region testRgn = poly.AsXRegion();
    Boolean lEquality = EqualRgn( testRgn, rgn );

    if ( !lEquality ) {
       printf( "Error in RgnToPoly: output region not equal to input region\n" );
       printf( "Dumping input region\n" );
       DumpRegion( rgn );
       printf( "Dumping output region\n" );
       DumpRegion( testRgn );
       printf( "Dumping output polygon\n" );
       DumpPolygon( poly );
    }
    WASSERT( lEquality );
    DeleteObject( testRgn );

#endif // ODImageDebug
}

#endif  // _PLATFORM_UNIX_








//
// Debugging routines
//
#if ODImageDebug

#ifdef _PLATFORM_OS2_
void DumpRegion(HPS hps, HRGN hrgn)
{
  PRECTL prcl;
  int nRects = GetRegionRects(hps, hrgn, &prcl);

  for (int i = 0; i < nRects; i++ ) {
     printf(" (%3d,%3d  %3d,%3d)", prcl[i].xLeft, prcl[i].yBottom, prcl[i].xRight, prcl[i].yTop);
     if (!((i+1) % 4))
       printf("\n");
  }
  printf("\n\n");
}
#endif  // _PLATFORM_OS2_



#ifdef _PLATFORM_WIN32_
void DumpRegion( HRGN hrgn )
{
   LPRGNDATA lpRgn;
   LPRECT prcl;

   int nRects = GetRegionRects( hrgn, &lpRgn );

   prcl = (LPRECT)lpRgn->Buffer;
   for (int i = 0; i < nRects; i++ ) {
      printf(" (%3d,%3d  %3d,%3d)", prcl->left, prcl->top, prcl->right, prcl->bottom);
      if (!((i+1) % 4))
         printf("\n");
   }
   printf("\n\n");
   free( lpRgn );
}
#endif  // _PLATFORM_WIN32_


#ifdef _PLATFORM_UNIX_
void DumpRegion( ODRgnHandle rgn )
{
   XRectangle* rectangles;
   XRectangle* prect;

   int nRects = RealShape::GetRegionRects( rgn, &rectangles );

   prect = rectangles;
   for (int i = 0; i < nRects; i++ ) {
      printf(" (%3d,%3d  %3d,%3d)", prect->x, prect->y, prect->width, prect->height);
      if (!((i+1) % 4))
         printf("\n");
      prect++;
   }
   printf("\n\n");
   XFree( rectangles );
}
#endif  // _PLATFORM_UNIX_



void DumpPolygon(const ODPolygon &p)
{
  printf("\n# Contours=%d",p.GetNContours());
  const ODContour *c = p.FirstContour();
  for( int j=0; j<p.GetNContours(); j++ ) {
    printf("\n# Verticies=%d\n",c->nVertices);
    for( int k=0; k<c->nVertices; k++ ) {
      printf(" %6.2f,%6.2f ", (float)ODFixedToFloat(c->vertex[k].x),
                              (float)ODFixedToFloat(c->vertex[k].y));
      if (!((k+1) % 5))
        printf("\n");
    }
    c = c->NextContour();
  }
  printf("\n\n");
}
#endif // ODImageDebug

