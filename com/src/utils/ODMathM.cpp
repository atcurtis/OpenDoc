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
// @(#) 1.8 com/src/utils/ODMathM.cpp, odutils, od96os2, odos29646d 7/15/96 17:58:51 [ 11/15/96 15:28:46 ]
/*
	File:		ODMath.cpp

	Contains:	Math routines (fixed-point and wide) for OpenDoc.

	Owned by:	Jens Alfke
	Written by:	Cary Clark, Georgiann Delaney, Michael Fairman, Dave Good,
				Robert Johnson, Keith McGreggor, Oliver Steele, David Van Brink,
				Chris Yerga

	Copyright:	© 1987 - 1995 by Apple Computer, Inc., all rights reserved.

	Notes:
		The PowerPC definitions of many of these functions are already built
		into the Mac OS, so they are compiled here only for 68k. Likewise,
		the 68k definitions of some of these functions are coded in assembly
		language in ODMathM.a, and they are compiled here only for PowerPC.
	
	To Do:
	In Progress:
		
*/


#ifndef _ODMATH_
#include "ODMath.h"
#endif

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
#ifndef __FIXMATH__
#include <FixMath.h>
#endif
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)


//=============================================================================
// Constants
//=============================================================================


#define longSize		32
#define wideSize		64

#define highBit			((ODULong) 0x80000000)


//=============================================================================
// Wide Math For Non-PowerPC
//=============================================================================


#if !defined(powerc) && !defined(__powerc)


ODFixed lastSinCosAngle = 0;	// Globals used in .a file
ODFract lastSine = 0,
		lastCosine = kODFract1;


// Wide math routines are already in the OS on Power Macs.


#if !defined(_PLATFORM_UNIX_)
ODSShort ODWideCompare(const ODWide *a, const ODWide *b)
{
	if (a->hi > b->hi) return 1;
	if (a->hi < b->hi) return -1;
	if (a->lo > b->lo) return 1;
	if (a->lo < b->lo) return -1;
	return 0;
}


ODWide *
ODWideNegate(register ODWide *dst)
{
	if (dst->lo) {
		dst->lo = -dst->lo;
		dst->hi = ~dst->hi;
	} else
		dst->hi = -dst->hi;
	return dst;
}


/* Shift a ODWide to the right (shift > 0) or left (shift < 0) */
ODWide *
ODWideShift(register ODWide *src, register ODSShort shift)
{
	register long x = src->hi, y = src->lo;
	if (shift > 0)
		if (shift >= longSize)
		{	src->hi = -(x < 0);
			src->lo = x >> shift - longSize;
			if (shift == longSize && y < 0 || x << wideSize - shift < 0)
				++src->lo || ++src->hi;
		}
		else
		{	src->hi = x >> shift;
			src->lo = x << longSize - shift | (unsigned long)y >> shift;
			if (y << longSize - shift < 0)
				++src->lo || ++src->hi;
		}
	else if ( (shift = -shift) != 0 )
		if (shift >= longSize)
		{	src->hi = y << shift - longSize;
			src->lo = 0;
		}
		else
		{	src->hi = x << shift | (unsigned long)y >> longSize - shift;
			src->lo = y << shift;
		}
	return src;
}


ODWide *
ODWideAdd(register ODWide *dst, register const ODWide *src)
{
	dst->lo += src->lo;
	if (dst->lo < src->lo)
		dst->hi += src->hi + 1;
	else
		dst->hi += src->hi;
	return dst;
}


ODWide *
ODWideSubtract(register ODWide *dst, register const ODWide *src)
{
	if (dst->lo < src->lo)
		dst->hi -= src->hi + 1;
	else
		dst->hi -= src->hi;
	dst->lo -= src->lo;
	return dst;
}
#else // defined(_PLATFORM_UNIX_)
ODSShort ODWideCompare(const ODWide *a, const ODWide *b)
{
	if (*a > *b) return 1;
	if (*a < *b) return -1;
	return 0;
}


ODWide *
ODWideNegate(register ODWide *dst)
{
	*dst = -(*dst);
	return dst;
}


/* Shift a ODWide to the right (shift > 0) or left (shift < 0) */
ODWide *
ODWideShift(register ODWide *src, register ODSShort shift)
{
	if (shift < 0)
		*src <<= -shift;
	else
		*src >>= shift;

	return src;
}


ODWide *
ODWideAdd(register ODWide *dst, register const ODWide *src)
{
	*dst += *src;
	return dst;
}


ODWide *
ODWideSubtract(register ODWide *dst, register const ODWide *src)
{
	*dst -= *src;
	return dst;
}
#endif // !defined(_PLATFORM_UNIX_)


#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

ODSShort
ODFirstBit( ODSLong x )		
{
	ODSShort bit = 0;

	if (x == 0)
		return -1;

	if (x >= 1UL << 16) {
		bit += 16;
		x >>= 16;
        }
	if (x >= 1UL << 8) {
		bit += 8; 
		x >>= 8;
        }
	if (x >= 1UL << 4) {
		bit += 4; 
		x >>= 4;
	}
	if (x >= 1UL << 2) {
		bit += 2; 
		x >>= 2;
	}
	if (x >= 1UL << 1) {
		bit += 1;
        }
	return bit;
}
#if defined(_PLATFORM_UNIX_)
ODSShort
ODWideFirstBit( ODWide x )		
{
	ODSShort fb;

	fb = ODFirstBit(*(ODSLong *)&x);
	if (fb >= 0)
		return fb + sizeof(ODSLong);
	return ODFirstBit((ODSLong)x);
}
#endif // defined(_PLATFORM_UNIX_)

#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

// ODWideMultiply and ODWideDivide are in ODMathM.a



//=============================================================================
// Wide Math  & Bit-Twiddling For PowerPC
//=============================================================================


#else /*powerc*/


ODSShort
ODFirstBit( ODSLong x )				// JPA: I wrote this from scratch
{
	ODSShort bit = 0;

	if (x == 0)
		return -1;

	if (x >= 1UL << 16)
		bit += 16, x >>= 16;
	if (x >= 1UL << 8)
		bit += 8, x >>= 8;
	if (x >= 1UL << 4)
		bit += 4, x >>= 4;
	if (x >= 1UL << 2)
		bit += 2, x >>= 2;
	if (x >= 1UL << 1)
		bit += 1;
	return bit;
}

#undef ODWideCompare
ODSShort ODWideCompare(const ODWide *a, const ODWide *b)
{
	return (ODSShort) WideCompare(a,b);
}

#undef ODWideNegate
ODWide * ODWideNegate(register ODWide *dst)
{
	return (ODWide*) WideNegate(dst);
}

#undef ODWideShift
ODWide * ODWideShift(register ODWide *src, register ODSShort shift)
{
	return (ODWide*) WideShift(src,(long)shift);
}

#undef ODWideAdd
ODWide * ODWideAdd(register ODWide *dst, register const ODWide *src)
{
	return (ODWide*) WideAdd(dst,src);
}

#undef ODWideSubtract
ODWide * ODWideSubtract(register ODWide *dst, register const ODWide *src)
{
	return (ODWide*) WideSubtract(dst,src);
}

#undef ODWideDivide
ODSLong ODWideDivide( register const ODWide *dividend,
							ODSLong divisor, ODSLong *remainder)
{
	return (ODSLong) WideDivide(dividend,divisor,remainder);
}

#undef ODWideSquareRoot
ODULong ODWideSquareRoot(register const ODWide *src)
{
	return (ODULong) WideSquareRoot(src);
}

ODFract
ODFractSinCos( ODFixed angle, ODFract *cos )
{
	// I have no idea whether FracSin and FracCos are native ... if not,
	// this will be much slower than it could be. But is this every going
	// to be a bottleneck?   [OPTIMIZATION]
	
	if( cos )
		*cos = FracCos(angle);
	return FracSin(angle);
}

#endif /*powerc*/
