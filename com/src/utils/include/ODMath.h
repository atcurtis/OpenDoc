//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.9 com/src/utils/include/ODMath.h, odutils, od96os2, odos29646d 10/10/96 11:26:29 [ 11/15/96 15:29:00 ]
/*
	File:		ODMath.h

	Contains:	Math routines (fixed-point and wide) for OpenDoc.

	Owned by:	Jens Alfke

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/


#ifndef _ODMATH_
#define _ODMATH_


#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
#ifndef _ODTYPESP_
#include <ODTypesP.h>
#endif
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#if _PLATFORM_MACINTOSH_
	#ifndef __FIXMATH__
	#include <FixMath.h>
	#endif
	#ifndef __TOOLUTILS__
	#include <ToolUtils.h>
	#endif
#endif


#if _PLATFORM_MACINTOSH_
typedef wide ODWide;
#elif defined(_PLATFORM_UNIX_)
typedef long long ODWide;
#else
struct ODWide {						// 64-bit integer
	ODSLong hi;							// High order longword comes first
	ODULong lo;							// Then low-order (which has no sign bit!)
};
#endif


const ODFixed kODFixed1				= 0x00010000;
const ODFixed kODFixedHalf			= 0x00008000;
const ODFract kODFract1				= 0x40000000;
const ODFixed kODFixedInfinity		= 0x7FFFFFFF;	// Fract as well
const ODFixed kODFixedMinusInfinity	= 0x80000000;


// Some of these fns are coded in assembly on 68k, so make calling conventions C:
extern "C" {


#define ODFixedRound(a)		((ODSShort)((ODFixed)(a) + kODFixedHalf >> 16))
#define ODIntToFixed(a)		((ODFixed)(a) << 16)
#define ODFixedToFract(a)	((ODFract)(a) << 14)
#define ODFractToFixed(a)	((ODFixed)(a) + 8192L >> 14)
#if defined(OD_BUG)
#define ODFixedToFloat(a)	((double)((ODFixed)(a)) / kODFixed1)
#else // OD_BUG
#define ODFixedToFloat(a)	((ODFixed)(a) / 65536.0)
#endif // OD_BUG
#define ODFloatToFixed(a)	((ODFixed)((double_t)(a) * 65536.0))
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
#define ODFractToFloat(a)	((double)((ODFract)(a)) / kODFract1)
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)


// These fixed-point math routines return infinity (see above) on overflow.

#if _PLATFORM_MACINTOSH_
	#define ODFixedMultiply		FixMul
	#define ODFixedDivide		FixDiv
	#define ODFractMultiply		FracMul
	#define ODFractDivide		FracDiv
	
	#ifdef __cplusplus
	inline ODWide* ODWideMultiply( ODSLong a, ODSLong b, ODWide *result )
	{
		LongMul(a,b,(Int64Bit*)result);
		return result;
	}
	#endif
#else
ODFixed	ODFixedMultiply( ODFixed a, ODFixed b );
ODFixed	ODFixedDivide( ODFixed a, ODFixed b );
ODFract	ODFractMultiply( ODFract a, ODFract b );
ODFract	ODFractDivide( ODFract a, ODFract b );
ODWide*	ODWideMultiply( ODSLong a, ODSLong b, ODWide *result );
#endif

/*
#if defined(_PLATFORM_MACINTOSH_) && (defined(powerc) || defined(__powerc))
	#define ODWideCompare		WideCompare
	#define ODWideNegate		WideNegate
	#define ODWideShift			WideShift
	#define ODWideAdd			WideAdd
	#define ODWideSubtract		WideSubtract
	#define ODWideMultiply		WideMultiply
	#define ODWideDivide		WideDivide
	#define ODWideSquareRoot	WideSquareRoot
#else
*/
ODSShort ODWideCompare( const ODWide*, const ODWide* );
ODWide*	ODWideNegate( ODWide* );
ODWide*	ODWideShift( ODWide*, ODSShort bits );	// Positive is toward MSB
ODWide*	ODWideAdd( ODWide*, const ODWide* );
ODWide*	ODWideSubtract( ODWide*, const ODWide* );

ODSLong	ODWideDivide( const ODWide *dividend,
							ODSLong divisor, ODSLong *remainder);
ODULong ODWideSquareRoot( const ODWide *src );
/*
#endif
*/
#define kODIgnoreRemainder  ((ODSLong*)-1L)	// Use as remainder in ODWideDivide

#if !defined(_PLATFORM_UNIX_)
#define ODWideIsLong(w) ((w)->hi ==  0 && (long)(w)->lo >= 0 || \
						  (w)->hi == -1 && (long)(w)->lo < 0)
#else // defined(_PLATFORM_UNIX_)
const ODWide kODMaxLong = 2147483647ll;
#define ODWideIsLong(w) (-2147483647ll <= *w && *w <= 2147483647ll)
#endif // !defined(_PLATFORM_UNIX_)

ODFract	ODFractSinCos( ODFixed radians, ODFract *cos );	// returns sin

ODSShort	ODFirstBit( ODSLong );			// Returns index (0-32) of 1st bit
#if defined(_PLATFORM_UNIX_)
ODSShort	ODWideFirstBit( ODWide );			// Returns index (0-64) of 1st bit
#endif // defined(_PLATFORM_UNIX_)


}	// End of extern "C" {

#endif /*_ODMATH_*/
