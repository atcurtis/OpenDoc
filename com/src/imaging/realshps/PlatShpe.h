/* @(#)Z 1.12 os2/src/imaging/realshps/PlatShpe.h, odimaging, od96os2, odos29646d 96/11/15 15:49:04 (96/10/04 11:53:44) */
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

/********************************************************************/
/*                                                                  */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/********************************************************************/

#ifndef _PLATSHAPE_
#define _PLATSHAPE_


// The following is really ugly.  This was merged from the os/2 code
// base of 02/13/96.  These redefines are there to limit the amount
// of porting by redefining methods, variables and constants from the
// Mac definition to the OS/2 definition.  jab 04/12/96 

#ifdef _PLATFORM_OS2_

#undef kODQuickDraw
#define kODQuickDraw kODPM

#define AsQDPoint AsPOINTL
#define AsQDRect AsRECTL
#define AsQDPolygon AsPOLYGON
#define GetQDOffset GetOffset
#define IsQDOffset IsIntegerOffset
#define h x
#define v y

#endif  // _PLATFORM_OS2_


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
typedef HRGN RgnHandle;
#endif
#if defined(_PLATFORM_UNIX_)
typedef Region RgnHandle;
#endif

// Helper functions for Mac system calls



#ifdef _PLATFORM_OS2_

inline HPS GetPresSpace( ) {
   return WinGetScreenPS( HWND_DESKTOP );
}

inline void ReleasePresSpace( HPS hps ) {
   WinReleasePS(hps);
}

#endif  // _PLATFORM_OS2_

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
inline void RectRgn( HRGN  hrgn, PRECTL prcl ) {

#ifdef _PLATFORM_OS2_
   HPS hps = GetPresSpace();
   RECTL rcl = *prcl;
   GpiSetRegion( hps, hrgn, 1, &rcl );
   ReleasePresSpace( hps );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   hrgn = CreateRectRgn( prcl->left, prcl->top, 
                           prcl->right, prcl->bottom );
#endif  // _PLATFORM_WIN32_

}
#endif

#if defined(_PLATFORM_UNIX_)
inline void RectRgn( ODRgnHandle hrgn, Rect prcl ) {
   XUnionRectWithRegion(&prcl, hrgn, hrgn);
}
#endif

inline ODRgnHandle NewRgn() {

#ifdef _PLATFORM_OS2_
   HPS hps = GetPresSpace();
   ODRgnHandle rgn = GpiCreateRegion( hps, 0, 0 );
   ReleasePresSpace( hps );
   return rgn;
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   ODRgnHandle rgn = CreateRectRgn( 0, 0, 0, 0 );
   return rgn;
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
   ODRgnHandle rgn = XCreateRegion();
   return rgn;
#endif  // _PLATFORM_UNIX_

}



inline ODRgnHandle ODNewRgn() { return NewRgn(); }



#ifdef _PLATFORM_OS2_     

// SetRectRgn is defined as part of the standard Windows GDI library.
// It's signature is:
//    BOOL SetRectRgn( HRGN hrgn, int left, int top, int right, int bottom)
//
inline VOID SetRectRgn( ODRgnHandle rgn, int top, int left, int bottom, int right ) {
   RECTL rcl;
   rcl.xLeft = left;
   rcl.xRight = right;
   rcl.yTop = top;
   rcl.yBottom = bottom;
   HPS hps = GetPresSpace();
   GpiSetRegion( hps, rgn, 1, &rcl );
   ReleasePresSpace( hps );
   
}


// EqualRgn is defined as part of the standard Windows GDI library.
// It's signature is:
//    BOOL EqualRgn( HRGN hrgn1, HRGN hrgn2)
//
inline BOOL EqualRgn( ODRgnHandle rgn1, ODRgnHandle rgn2 ) {
   LONG lEquality;
   HPS hps = GetPresSpace();
   lEquality = GpiEqualRegion( hps, rgn1, rgn2 );
   ReleasePresSpace( hps );
   return ( lEquality == EQRGN_EQUAL );
}

#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_UNIX_     

inline void SetRectRgn( ODRgnHandle rgn, int top, int left, int bottom, int right ) {
   XRectangle rect;
   rect.x = left;
   rect.y = bottom;
   rect.width = right - left;
   rect.height = top - bottom;
   XUnionRectWithRegion(&rect, rgn, rgn);
}

inline ODBoolean EqualRgn( ODRgnHandle rgn1, ODRgnHandle rgn2 ) {
   return ( XEqualRegion(rgn1, rgn2) );
}

#endif  // _PLATFORM_UNIX_


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
inline BOOL EmptyRgn( ODRgnHandle rgn ) {

#ifdef _PLATFORM_OS2_     
   RECTL rcl;
   HPS hps = GetPresSpace();
   LONG lComplexity = GpiQueryRegionBox( hps, rgn, &rcl );
   ReleasePresSpace(hps);
   return ( lComplexity == RGN_NULL || lComplexity == RGN_ERROR );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   RECT rcl;
   LONG lComplexity = GetRgnBox( rgn, &rcl );
   return ( lComplexity == NULLREGION );
#endif  // _PLATFORM_WIN32_

}
#endif

#if defined(_PLATFORM_UNIX_)
inline ODBoolean EmptyRgn( ODRgnHandle rgn ) {
	return XEmptyRegion(rgn);
}
#endif



#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
inline BOOL PtInRgn( Point pt, ODRgnHandle rgn ) {

#ifdef _PLATFORM_OS2_     
   HPS hps = GetPresSpace();
   LONG lInside = GpiPtInRegion( hps, rgn, &pt );
   ReleasePresSpace( hps );
   return( lInside == PRGN_INSIDE ); 
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   return( PtInRegion( rgn, pt.x, pt.y ) );
#endif  // _PLATFORM_WIN32_

}
#endif

#if defined(_PLATFORM_UNIX_)
inline ODBoolean PtInRgn( Point pt, ODRgnHandle rgn ) {
	return XPointInRegion(rgn, pt.x, pt.y);
}
#endif


inline void DisposeRgn( ODRgnHandle rgn ) {

#ifdef _PLATFORM_OS2_     
   HPS hps = GetPresSpace();
   GpiDestroyRegion( hps, rgn );
   ReleasePresSpace( hps );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   BOOL success = DeleteObject( rgn );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
   XDestroyRegion(rgn);
#endif  // _PLATFORM_UNIX_

}



#ifdef _PLATFORM_OS2_

// OffsetRgn is defined as part of the Windows GDI library.
// It's signature is:
//   int OffsetRgn( HRGN hrgn, int x, int y )
//
inline void OffsetRgn( ODRgnHandle rgn, int x, int y ) {
   POINTL ptl = {x, y};
   HPS hps = GetPresSpace();
   GpiOffsetRegion( hps, rgn, &ptl );
   ReleasePresSpace( hps );

}

#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_UNIX_

// OffsetRgn is defined as part of the Windows GDI library.
// It's signature is:
//   int OffsetRgn( HRGN hrgn, int x, int y )
//
inline void OffsetRgn( ODRgnHandle rgn, int x, int y ) {
   XOffsetRegion(rgn, x, y);
}

#endif  // _PLATFORM_UNIX_





#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
inline BOOL SectRect( Rect* r1, Rect* r2, Rect* sect ) {

#ifdef _PLATFORM_OS2_     
   return WinIntersectRect( WinQueryAnchorBlock(HWND_DESKTOP), sect, r1, r2 );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   return IntersectRect( sect, r1, r2 );
#endif  // _PLATFORM_WIN32_

}
#endif

#if defined(_PLATFORM_UNIX_)

#define max(a,b) (a)>(b) ? (a) : (b)
#define min(a,b) (a)<(b) ? (a) : (b)

inline ODBoolean SectRect( Rect* r1, Rect* r2, Rect* sect ) {
   int left1 = r1->x;
   int bottom1 = r1->y;
   int right1 = r1->x + r1->width;
   int top1 = r1->y + r1->height;
   int left2 = r2->x;
   int bottom2 = r2->y;
   int right2 = r2->x + r2->width;
   int top2 = r2->y + r2->height;
   int leftS = max(left1, left2);
   int bottomS = max(bottom1, bottom2);
   int rightS = min(right1, right2);
   int topS = min(top1, top2);
   
   if ((leftS < rightS) && (bottomS < topS)) {
     sect->x = leftS;
     sect->y = bottomS;
     sect->width = rightS - leftS;
     sect->height = topS - bottomS;
     return kODTrue;
	 }
   else return kODFalse;
}

#endif





inline void DiffRgn( ODRgnHandle srcRgn1, ODRgnHandle srcRgn2, ODRgnHandle dstRgn ) {

#ifdef _PLATFORM_OS2_     
   HPS hps = GetPresSpace();
   GpiCombineRegion( hps, dstRgn, srcRgn1, srcRgn2, CRGN_DIFF );
   ReleasePresSpace( hps );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   CombineRgn( dstRgn, srcRgn1, srcRgn2, RGN_DIFF );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
   XSubtractRegion(srcRgn1, srcRgn2, dstRgn);
#endif  // _PLATFORM_UNIX_

}





inline void UnionRgn( ODRgnHandle srcRgn1, ODRgnHandle srcRgn2, ODRgnHandle dstRgn ) {

#ifdef _PLATFORM_OS2_     
   HPS hps = GetPresSpace();
   GpiCombineRegion( hps, dstRgn, srcRgn1, srcRgn2, CRGN_OR );
   ReleasePresSpace( hps );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   CombineRgn( dstRgn, srcRgn1, srcRgn2, RGN_OR );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
   XUnionRegion(srcRgn1, srcRgn2, dstRgn);
#endif  // _PLATFORM_UNIX_

}




inline void SectRgn( ODRgnHandle srcRgn1, ODRgnHandle srcRgn2, ODRgnHandle dstRgn ) {

#ifdef _PLATFORM_OS2_     
   HPS hps = GetPresSpace();
   GpiCombineRegion( hps, dstRgn, srcRgn1, srcRgn2, CRGN_AND );
   ReleasePresSpace( hps );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   CombineRgn( dstRgn, srcRgn1, srcRgn2, RGN_AND );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
   XIntersectRegion(srcRgn1, srcRgn2, dstRgn);
#endif  // _PLATFORM_UNIX_

}


inline ODRgnHandle ODCopyRgnHandle( ODRgnHandle rgn ) {

#ifdef _PLATFORM_OS2_     
   HPS hps = GetPresSpace();
   HRGN hrgn = GpiCreateRegion(hps, 0, 0);
   GpiCombineRegion(hps, hrgn, (RgnHandle)rgn, 0, CRGN_COPY);
   ReleasePresSpace(hps);
   return (ODRgnHandle)hrgn;
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
   HRGN hrgn = CreateRectRgn( 0, 0, 0, 0 );
   CombineRgn( hrgn, rgn, 0, RGN_COPY );
   return (ODRgnHandle)hrgn;
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
   Region newRgn = XCreateRegion();
   XUnionRegion((Region)rgn, newRgn, newRgn);
   return (ODRgnHandle)newRgn;
#endif // _PLATFORM_UNIX_
}

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

#endif // _PLATSHAPE_



