/* @(#)Z 1.6 com/src/imaging/polyclip/PGCommon.h, odimaging, od96os2, odos29646d 96/11/15 15:29:16 (96/10/29 09:26:15) */
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
  File:    PGCommon.h

  Contains:  Common definitions for the polygon clipper.

  Written by:  Jens Alfke (based on algorithm by A. C. Kilgour)

  Copyright:  © 1994 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <2>   12/5/94  jpa    Code review fixes. [1203923]
     <1>   6/15/94  jpa    first checked in
     ---------------------------Moved to ODSOM project.
     <1>    5/9/94  jpa    first checked in
*/


#ifndef _PGCOMMON_
#define _PGCOMMON_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif


//=============================================================================
// Debugging
//=============================================================================

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


#ifdef __MWERKS__
#undef PROFILING
#define PROFILING 0          /* 1 enables Metrowerks profiling, 0 disables it*/
#endif

#if PROFILING
  #pragma profile on
  #define LOGGING 0        /* Never log while profiling*/
#else
  #undef LOGGING
  #define LOGGING 0        /* 1 enables logging, 0 disables it*/
//  #define LOGGING 1        /* 1 enables logging, 0 disables it*/
#endif

#define BEGINLOG
#define ENDLOG

#define kPGAssertionFailed 7734

#if LOGGING
#define PGASSERT(COND)  WASSERT(COND)
#else
#define PGASSERT(COND)  ASSERT(COND,kODErrAssertionFailed)
#endif


//=============================================================================
// Constants & Types
//=============================================================================


typedef enum {
  kNegative = -1,
  kZero     =  0,
  kPositive =  1
} PGSense;


typedef enum {
  kLeft  = -1,
  kOnTop  =  0,
  kRight  =  1
} PGSide;


//=============================================================================
// Globals
//=============================================================================


class PGContourList;


extern ODSLong  gMinWrap, gMaxWrap;      // Min/max wrap desired for output

extern PGContourList *gOutputContours;    // GLOBAL: Collects output contours


#endif /*_PGCOMMON*/


