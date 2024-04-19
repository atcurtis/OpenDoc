/* @(#)Z 1.7 com/src/imaging/Rgn2PlyM.h, odimaging, od96os2, odos29646d 96/11/15 15:29:16 (96/10/29 09:25:20) */
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
  File:    Rgn2PlyM.h

  Contains:  Converting a Mac Region into an ODPolygon.

  Written by:  Jens Alfke

  Copyright:  © 1994 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <1>   6/15/94  jpa    first checked in
     <1>   3/25/94  JA    first checked in
     <1>   1/31/94  JA    first checked in

  To Do:
*/


#ifdef _PLATFORM_MACINTOSH_ 
#pragma once
#endif    // _PLATFORM_MACINTOSH_

#ifndef _RGN2POLY_
#define _RGN2POLY_

#ifdef _PLATFORM_MACINTOSH_
#ifndef __QUICKDRAW__
#include <QuickDraw.h>
#endif
#endif


struct ODPolygon;

#ifdef _PLATFORM_OS2_ 
void Rgn2Poly( HPS hps, ODRgnHandle rgn, ODPolygon& );
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_ 
void Rgn2Poly( ODRgnHandle rgn, ODPolygon& );
#endif  // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_ 
void Rgn2Poly( ODRgnHandle rgn, ODPolygon& );
#endif  // _PLATFORM_UNIX_


#ifdef _PLATFORM_MACINTOSH_
void Rgn2Poly( RgnHandle rgn, ODPolygon& );
#endif // _PLATFORM_MACINTOSH_ 


#endif //_RGN2POLY_


