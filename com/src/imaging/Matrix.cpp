/* @(#)Z 1.8 com/src/imaging/Matrix.cpp, odimaging, od96os2, odos29646d 96/11/15 15:25:03 (96/10/29 09:25:06) */
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
/*                                                                  */
/*  IBM Change History (most recent first):                         */
/*          3/18/96 jab  Merge OS/2 Feb. 13 drop with DR4.          */
/*  117544  4/3/95  MAP  Replace definitions of true/false          */
/*  <OS2>   8/27/94 MAP  Port A8C3                                  */
/*                                                                  */
/********************************************************************/
/*
	File:		Matrix.cpp

	Contains:	Matrix math for ODTransforms.

	Owned by:	Jens Alfke
	Owned by:	Cary Clark, Michael Fairman, Robert Johnson, Keith McGreggor,
				Oliver Steele, David Van Brink, Jens Alfke
	Based on:	QuickDraw GX "matrixMath.c" and "math.c" sources

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):
	
		 <2>	 5/25/95	jpa		Renamed some routines that conflict with GX
									exports. [1241078, 1253324]
		 <1>	 6/15/94	jpa		first checked in
		 ---------------------------Moved to ODSOM project.
		 <2>	 5/10/94	jpa		Replaced a too-complex typecast to appease
									%$*# cfront.
		 <1>	  5/9/94	jpa		first checked in
	To Do:
		* I have no idea whether this code will work on little-endian CPUs.
		  It _should_, but who knows?
	In Progress:
		
*/


#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODTransform
#endif


#ifndef _MATRIX_
#include "Matrix.h"
#endif


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#ifndef _PAGETUNING_OFF_
#include <ODPagtun.h>
#endif 
#endif   // IBM Platforms

#ifndef _ODMATH_
#include "ODMath.h"
#endif

#if  defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#define FracDiv ODFractDivide
#define FracMul ODFractMultiply
#endif

//=============================================================================
// Constants & Types
//=============================================================================

// defect 117544
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#define true  kODTrue
#define false kODFalse
#endif

#define wideSize		64						/* bit sizes */
#define longSize		32
#define fractPrecision	30						/* fixed ODPoint precisions */
#define fixedPrecision	16


typedef int fastInt;


const ODMatrix kODIdentityMatrix = {{{kODFixed1, 0, 0},
                     {0, kODFixed1, 0},
                     {0,  0, kODFract1}}};


//=============================================================================
// Math Subroutines
//=============================================================================

// MyWideScale is the same as GX's WideScale.

static ODSShort
MyWideScale( const ODWide *wide )	// JPA: I wrote this from scratch
{
	ODWide temp = *wide;
#if !defined(_PLATFORM_UNIX_)
	if (temp.hi < 0)
		ODWideNegate(&temp);
	if (temp.hi)
		return ODFirstBit(temp.hi) + longSize;
	else
		return ODFirstBit(temp.lo);
#else // defined(_PLATFORM_UNIX_)
	if (temp < 0)
		temp = -temp;
	return ODWideFirstBit(temp);
#endif // !defined(_PLATFORM_UNIX_)
}


#if PLATFORM_MACINTOSH && USES68KINLINES
	/* an inline macro version of overflow checking: */
	extern "C" {
		#pragma parameter __D0 FixAddCheck(__D0, __A0)
		extern ODBoolean FixAddCheck( ODFixed a, ODFixed *b ) = {
			0xD190,		// ADD.L	D0,(A0)
			0x58C0,		// SVC		D0		0 if overflow, 0xff if no overflow
			0x5200  	// ADDQ.B	#$1,D0	1 if overflow, 0 if no overflow
		};
	}
#else
	static ODBoolean
	FixAddCheck( ODFixed a, ODFixed *b )
	{
		register ODFixed temp = *b;
		
		*b += a;
		if (*b < temp ^ a < 0)
			return true;
		else
			return false;
	}
#endif


static ODBoolean
FixMulCheck( ODFixed a, ODFixed b, ODFixed *result )
{
	*result = ODFixedMultiply(a,b);
	return (*result==kODFixedInfinity || *result==kODFixedMinusInfinity);
}


static ODBoolean
FixDivCheck( ODFixed a, ODFixed b, ODFixed *result )
{
	*result = ODFixedDivide(a,b);
	return (*result==kODFixedInfinity || *result==kODFixedMinusInfinity);
}


static ODBoolean
FixMulAddCheck(register ODFixed a, register ODFixed b, register ODFixed *c)
{
	ODWide temp;
	ODWideShift(ODWideMultiply(a, b, &temp), fixedPrecision);
#if !defined(_PLATFORM_UNIX_)
	if (*c < 0) --temp.hi;
	temp.lo += *c;
	if (temp.lo < *c) ++temp.hi;
	*c = temp.lo;
#else // defined(_PLATFORM_UNIX_)
	temp += *c;
	*c = (ODFixed)temp;
#endif // !defined(_PLATFORM_UNIX_)
	return !ODWideIsLong(&temp);
}


static ODBoolean
MulMulAddCheck( long a1, long b1, long a2, long b2, short bias, long *dest )
{
	ODWide temp1, temp2;
	ODWideShift(ODWideAdd(ODWideMultiply(a1, b1, &temp1),
				 			ODWideMultiply(a2, b2, &temp2)),
				 bias);
#if !defined(_PLATFORM_UNIX_)
	if (*dest < 0) --temp1.hi;
	if ((temp1.lo += *dest) < *dest) ++temp1.hi;
	*dest = temp1.lo;
#else // defined(_PLATFORM_UNIX_)
	temp1 += *dest;
	*dest = (long)temp1;
#endif // !defined(_PLATFORM_UNIX_)
	return !ODWideIsLong(&temp1);
}


static ODBoolean
FracDivCheck(register ODFixed dividend, register ODFixed divisor, register ODFixed *dest)
{
	ODWide temp;
#if !defined(_PLATFORM_UNIX_)
	temp.hi = dividend; temp.lo = 0;
#else // defined(_PLATFORM_UNIX_)
	temp = (ODWide)dividend << sizeof(long);
#endif // !defined(_PLATFORM_UNIX_)
	*dest = ODWideDivide(ODWideShift(&temp, longSize-fractPrecision), divisor, kODIgnoreRemainder);
	return *dest == kODFixedMinusInfinity;
}


static ODBoolean
FracMulAddCheck(register ODFixed a, register ODFixed b, register ODFixed *c)
{
	ODWide temp;
	ODWideShift(ODWideMultiply(a, b, &temp), fractPrecision);
#if !defined(_PLATFORM_UNIX_)
	if (*c < 0) --temp.hi;
	temp.lo += *c;
	if (temp.lo < *c) ++temp.hi;
	*c = temp.lo;
#else // defined(_PLATFORM_UNIX_)
	temp += *c;
	*c = (long)temp;
#endif // !defined(_PLATFORM_UNIX_)
	return !ODWideIsLong(&temp);
}


static ODBoolean
FracDivAddCheck(register ODFixed a, register ODFixed b, register ODFixed *c)
{
	ODFixed d;
	return FracDivCheck(a, b, &d) || FixAddCheck(d, c);
}


// MyVectorMultiply is the same as GX's VectorMultiply. Ditto the ...Divide variant.

static ODWide*
MyVectorMultiply(register long count, register const long *vector1, register long step1,
				register const long *vector2, register long step2, register ODWide *dot)
{
	short flags = count >= 0;
#if !defined(_PLATFORM_UNIX_)
	dot->hi = dot->lo = 0;
#else // defined(_PLATFORM_UNIX_)
	*dot = 0;
#endif // !defined(_PLATFORM_UNIX_)
	if (!flags)
		count = -count;
	while (count--)
	{	ODWide temp;
		if (flags ^= 2)
			ODWideAdd(dot, ODWideMultiply(*vector1, *vector2, &temp));
		else
			ODWideSubtract(dot, ODWideMultiply(*vector1, *vector2, &temp));
		vector1 += step1;
		vector2 += step2;
	}
	return dot;
}


static long
MyVectorMultiplyDivide(long count, const long *vector1, long step1,
								 const long *vector2, long step2, long scalar)
{
	ODWide temp;
	return ODWideDivide(MyVectorMultiply(count, vector1, step1, vector2, step2, &temp), scalar, 0);
}



//=============================================================================
// Matrix Math
//=============================================================================


//
// IBM Platforms
//
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_) // [122017]

const ODFixed kFixedEpsilon= 0x00000007;  // Amt by which fixeds can differ and be "equal"

inline ODBoolean Neq( ODFixed a, ODFixed b )
{
  return (a-b > kFixedEpsilon) || (a-b < -kFixedEpsilon);
}

ODTransformType
MxType(const ODMatrix *matrix)
{
  register ODSShort translate;

  if (matrix->m[0][2] == 0 && matrix->m[1][2] == 0 && matrix->m[2][2] <= 0)
    return kODInvalidXform;
  if (Neq(matrix->m[0][2],0) || Neq(matrix->m[1][2],0) || Neq(matrix->m[2][2],kODFract1))
    return kODPerspectiveXform;
  if (Neq(matrix->m[2][0],0) || Neq(matrix->m[2][1],0))
    translate = kODTranslateXform;
  else
    translate = kODIdentityXform;
  if (Neq(matrix->m[0][1],0) || Neq(matrix->m[1][0],0))
    return (ODTransformType)( (ODSShort)kODLinearXform + translate );
  if (Neq(matrix->m[0][0],kODFixed1) || Neq(matrix->m[1][1],kODFixed1))
    return (ODTransformType)( (ODSShort)kODScaleXform + translate );
  return (ODTransformType)translate;
}

#endif  // IBM Platforms


//
// Macintosh specific code
//
#ifdef _PLATFORM_MACINTOSH_

ODTransformType
MxType(const ODMatrix *matrix)
{
  register ODSShort translate;

  if (matrix->m[0][2] == 0 && matrix->m[1][2] == 0 && matrix->m[2][2] <= 0)
    return kODInvalidXform;
  if (matrix->m[0][2] || matrix->m[1][2] || matrix->m[2][2] != kODFract1)
    return kODPerspectiveXform;
  if (matrix->m[2][0] || matrix->m[2][1])
    translate = kODTranslateXform;
  else
    translate = kODIdentityXform;
  if (matrix->m[0][1] || matrix->m[1][0])
    return (ODTransformType)( (ODSShort)kODLinearXform + translate );
  if (matrix->m[0][0] != kODFixed1 || matrix->m[1][1] != kODFixed1)
    return (ODTransformType)( (ODSShort)kODScaleXform + translate );
  return (ODTransformType)translate;
}

#endif // _PLATFORM_MACINTOSH_

ODBoolean
MxNormalize(ODMatrix *map)
{
	unsigned long norm[3][3];
	register fastInt shift, count;
	register ODFixed *src;
	register unsigned long *dst;
	ODTransformType state = MxType(map);

	if (state == kODInvalidXform) return true;
	if (state != kODPerspectiveXform) return false;			/* map is already normalized */

	{	register unsigned long mask = 0;
		src = &map->m[0][0]; dst = &norm[0][0];
		count = 9;
		do
		{	register ODFixed temp;
			if ((temp = *src++) < 0) temp = -temp;
			mask |= *dst++ = temp;
		} while (--count);
		shift = ODFirstBit(mask) - fractPrecision;			/* find the scale of greatest element */
	}

	src = &map->m[0][0];
	{	register unsigned long sum;
		register ODFract w;
		if ( (sum = norm[0][2] + norm[1][2]) != 0 )				/* map is truly perspective */
		{	count = ODFirstBit(sum) - (fractPrecision - 1);
			if (count >= 0 ? sum > kODFract1 - kODFixed1 << count : sum << -count > kODFract1 - kODFixed1) ++count;
			if (count > shift) shift = count;
		}
		else										/* map is pathologically affine */
		{	w = map->m[2][2];					/* must be > 0 or map would be invalid */
			if (w & w - 1)							/* if it's not a power of 2, try to divide by w */
			{	count = 3;
				dst = &norm[0][0];
				do								/* check if map can be divided by w */
				{	if (*dst++ >> 1 >= w ) break;		/* is the x-coefficient too big? */
					if (*dst++ >> 1 >= w ) break;		/* is the y-coefficient too big? */
				} while (++dst, --count);
				if (count == 0)						/* no coefficients were too big */
				{	count = 3;
					do
					{	*src = FracDiv(*src, w); ++src;	/* divide the x-coefficient by w */
						*src = FracDiv(*src, w); ++src;	/* divide the y-coefficient by w */
					} while (++src, --count);
					map->m[2][2] = kODFract1;
					return false;
				}
			}
		}
	}

	/*	if map is truly perspective, w is a power of 2, or map cannot be divided by w,
		shift map so that either the scale of some element is fractPrecision or
		the L1 norm of u and v does not exceed kODFract1 - kODFixed1.  This insures that
			FractMultiply(u, x) + FractMultiply(v, y) + FractToFixed(w)
		never overflows.
	*/
	count = 9;
	if (shift > 0)
	{	register ODFixed round = 1 << shift - 1;
		do *src = *src + round >> shift;
		while (++src, --count);
	}
	else if ( (shift = -shift) != 0 )
		do *src <<= shift;
		while (++src, --count);
	return false;
}


typedef struct
{	char bits[2][2];
} bias;


#if !defined(_PLATFORM_UNIX_)
static ODBoolean
MxWideNorm(ODWide src[3][3], ODMatrix *dst, register bias *delta)
{
	ODWide norm[3][3];
	register fastInt shift;
	register long hi;
	register unsigned long lo;
	register long *srcPtr, *dstPtr;

	{	register fastInt count;

				/* Compute the absolute values of src */
		srcPtr = (long *)&src[0][0]; dstPtr = (long *)&norm[0][0];
		count = 9;
		do
		{	hi = *srcPtr++; lo = *srcPtr++;
			if (hi < 0)
				(lo = -lo)!=0 ? (hi = ~hi) : (hi = -hi);
			*dstPtr++ = hi; *dstPtr++ = lo;
		} while (--count);

				/* Find the scale of a, b, c, and d */
		if ( (hi = norm[0][0].hi | norm[0][1].hi | norm[1][0].hi | norm[1][1].hi) != 0 )
			shift = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[0][0];
		else if ( (lo = norm[0][0].lo | norm[0][1].lo | norm[1][0].lo | norm[1][1].lo) != 0 )
			shift = ODFirstBit(lo) - fractPrecision - delta->bits[0][0];
		else shift = -wideSize;

				/* Find the scale of u and v */
		if ( (hi = norm[0][2].hi | norm[1][2].hi) != 0 )
			count = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[0][1];
		else if ( (lo = norm[0][2].lo | norm[1][2].lo) != 0 )
			count = ODFirstBit(lo) - fractPrecision - delta->bits[0][1];
		else count = -wideSize;
		if (count > shift) shift = count;

				/* Find the scale of h and k */
		if ( (hi = norm[2][0].hi | norm[2][1].hi) != 0 )
			count = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[1][0];
		else if ( (lo = norm[2][0].lo | norm[2][1].lo) != 0 )
			count = ODFirstBit(lo) - fractPrecision - delta->bits[1][0];
		else count = -wideSize;
		if (count > shift) shift = count;

				/* Find the scale of w */
		if ( (hi = norm[2][2].hi) != 0 )
			count = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[1][1];
		else if ( (lo = norm[2][2].lo) != 0 )
			count = ODFirstBit(lo) - fractPrecision - delta->bits[1][1];
		else count = -wideSize;
		if (count > shift) shift = count;
	}

	{	register fastInt i, j;

				/* Execute the shift */
		srcPtr = (long *)&src[0][0]; dstPtr = &dst->m[0][0];
		for (i = 3; i--; )
			for (j = 3; j--; )
			{	ODWide temp;
				temp.hi = *srcPtr++; temp.lo = *srcPtr++;
				ODWideShift(&temp, shift + delta->bits[!i ][!j]);
				*dstPtr++ = temp.lo;
			}
	}
	return MxNormalize(dst);
}
#else // defined(_PLATFORM_UNIX_)
static ODBoolean
MxWideNorm(ODWide src[3][3], ODMatrix *dst, register bias *delta)
{
	ODWide norm[3][3];
	register fastInt shift;
	ODWide *srcPtr, *tNormPtr, temp;
	register long *dstPtr;

	{
		register fastInt count;

				/* Compute the absolute values of src */
		count = 9;
		srcPtr = &src[0][0]; tNormPtr = &norm[0][0];
		do
		{
			if (*srcPtr < 0)
				*tNormPtr++ = *srcPtr++;
			else
				*tNormPtr++ = -*srcPtr++;
		} while (--count);

				/* Find the scale of a, b, c, and d */
		temp = norm[0][0] | norm[0][1] | norm[1][0] | norm[1][1];
		if (temp)
			shift = ODFirstBit(temp) - fractPrecision - delta->bits[0][0];
		else shift = -wideSize;

				/* Find the scale of u and v */
		temp = norm[0][2] | norm[1][2];
		if (temp)
			count = ODFirstBit(temp) - fractPrecision - delta->bits[0][1];
		else count = -wideSize;
		if (count > shift) shift = count;

				/* Find the scale of h and k */
		temp = norm[2][0] | norm[2][1];
		if (temp)
			count = ODFirstBit(temp) - fractPrecision - delta->bits[1][0];
		else count = -wideSize;
		if (count > shift) shift = count;

				/* Find the scale of w */
		temp = norm[2][2];
		if (temp)
			count = ODFirstBit(temp) - fractPrecision - delta->bits[1][1];
		else count = -wideSize;
		if (count > shift) shift = count;
	}

	{
		register fastInt i, j;
		
				/* Execute the shift */
		srcPtr = &src[0][0]; dstPtr = &dst->m[0][0];
		for (i = 3; i--; )
			for (j = 3; j--; )
			{
				temp = *srcPtr++;
				ODWideShift(&temp, shift + delta->bits[!i ][!j]);
				*dstPtr++ = (unsigned long)temp;
			}
	}
	return MxNormalize(dst);
}
#endif // !defined(_PLATFORM_UNIX_)


/*	MxInverse - Computes the inverse of the given matrix, placing it in the destination (second) matrix. */
ODBoolean
MxInverse(const ODMatrix *matrix, register ODTransformType flags, ODMatrix *imatrix)
{
	register ODFixed *im = &imatrix->m[0][0];
	register const ODFixed *m0 = &matrix->m[0][0];

	switch(flags) {
		case kODIdentityXform:
			*imatrix = *matrix;				/* do nothing but copy if identity */
		break;
		case kODTranslateXform:
		{
			*im++ = *m0++;											/* a = 1.0 */
			*im++ = *m0++;											/* b = 0.0 */
			*im++ = *m0++;											/* u = 0.0 */
			*im++ = *m0++;											/* c = 0.0 */
			*im++ = *m0++;											/* d = 1.0 */
			*im++ = *m0++;											/* v = 0.0 */
			*im++ = -*m0++;											/* h = -h */
			*im++ = -*m0++;											/* k = -k */
			*im = *m0;												/* w = 1.0 */
		}
		break;
		case kODScaleXform:
		case kODScaleXform+kODTranslateXform:
		{	ODFixed temp;
			register ODFixed a, d;
			if (FixDivCheck(kODFixed1, a = *m0++, &temp)) return true; *im++ = temp;	/* a = 1/a */
			*im++ = *m0++;											/* b = 0.0 */
			*im++ = *m0++;											/* u = 0.0 */
			*im++ = *m0++;											/* c = 0.0 */
			if (FixDivCheck(kODFixed1, d = *m0++, &temp)) return true; *im++ = temp;	/* d = 1/d */
			*im++ = *m0++;											/* v = 0.0 */
			if (flags & kODTranslateXform)
			{	if (FixDivCheck(*m0++, a, &temp)) return true; *im++ = -temp;	/* h = -h/a */
				if (FixDivCheck(*m0++, d, &temp)) return true; *im++ = -temp;	/* k = -k/d */
			}
			else
			{	*im++ = *m0++;										/* h = 0.0 */
				*im++ = *m0++;										/* k = 0.0 */
			}
			*im = *m0;												/* w = 1.0 */
		}
		break;
#if !defined(_PLATFORM_UNIX_)
		case kODLinearXform:
		case kODLinearXform+kODTranslateXform:
		{	ODWide det, temp, tmp;
			register short shift;

			/* compute the determinant */
			ODWideSubtract(ODWideMultiply(matrix->m[0][0], matrix->m[1][1], &det),
					ODWideMultiply(matrix->m[1][0], matrix->m[0][1], &temp));
			if ((shift = MyWideScale(&det) - (longSize - 2)) > 0)
				ODWideShift(&det, shift);
			else
				shift = 0;

			temp.hi = matrix->m[1][1]; temp.lo = 0;
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), det.lo, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			temp.hi = -matrix->m[0][1]; temp.lo = 0;
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), det.lo, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			*im++ = 0;
			temp.hi = -matrix->m[1][0]; temp.lo = 0;
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), det.lo, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			temp.hi = matrix->m[0][0]; temp.lo = 0;
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), det.lo, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			*im++ = 0;
			if (flags & kODTranslateXform)
			{	shift -= fixedPrecision;
				ODWideSubtract(ODWideMultiply(matrix->m[1][0], matrix->m[2][1], &temp), ODWideMultiply(matrix->m[2][0], matrix->m[1][1], &tmp));
				if (shift < MyWideScale(&temp) - (wideSize - 2))
					return true;
				if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), det.lo, kODIgnoreRemainder)) == kODFixedMinusInfinity)
					return true;
				ODWideSubtract(ODWideMultiply(matrix->m[0][1], matrix->m[2][0], &temp), ODWideMultiply(matrix->m[2][1], matrix->m[0][0], &tmp));
				if (shift < MyWideScale(&temp) - (wideSize - 2))
					return true;
				if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), det.lo, kODIgnoreRemainder)) == kODFixedMinusInfinity)
					return true;
			}
			else
			{	*im++ = 0;
				*im++ = 0;
			}
			*im++ = kODFract1;
		}
		break;
		case kODPerspectiveXform:
		{	ODWide inv[3][3];
			ODWide temp;
			bias delta = { 30, 16, 16, 2 };

			ODWideSubtract(ODWideMultiply(m0[4], m0[8], &inv[0][0]), ODWideMultiply(m0[5], m0[7], &temp));
			ODWideSubtract(ODWideMultiply(m0[2], m0[7], &inv[0][1]), ODWideMultiply(m0[1], m0[8], &temp));
			ODWideSubtract(ODWideMultiply(m0[1], m0[5], &inv[0][2]), ODWideMultiply(m0[2], m0[4], &temp));
			ODWideSubtract(ODWideMultiply(m0[5], m0[6], &inv[1][0]), ODWideMultiply(m0[3], m0[8], &temp));
			ODWideSubtract(ODWideMultiply(m0[0], m0[8], &inv[1][1]), ODWideMultiply(m0[6], m0[2], &temp));
			ODWideSubtract(ODWideMultiply(m0[2], m0[3], &inv[1][2]), ODWideMultiply(m0[0], m0[5], &temp));
			ODWideSubtract(ODWideMultiply(m0[3], m0[7], &inv[2][0]), ODWideMultiply(m0[6], m0[4], &temp));
			ODWideSubtract(ODWideMultiply(m0[6], m0[1], &inv[2][1]), ODWideMultiply(m0[0], m0[7], &temp));
			ODWideSubtract(ODWideMultiply(m0[0], m0[4], &inv[2][2]), ODWideMultiply(m0[3], m0[1], &temp));

			return MxWideNorm(inv, imatrix, &delta);
		}
#else // defined(_PLATFORM_UNIX_)
		case kODLinearXform:
		case kODLinearXform+kODTranslateXform:
		{	ODWide det, temp;
			register short shift;

			/* compute the determinant */
			det = (ODWide)matrix->m[0][0] * (ODWide)matrix->m[1][1];
			temp = (ODWide)matrix->m[1][0] * (ODWide)matrix->m[0][1];
			det -= temp;
			if ((shift = MyWideScale(&det) - (longSize - 2)) > 0)
				ODWideShift(&det, shift);
			else
				shift = 0;

			temp = ((ODWide)matrix->m[1][1]) << sizeof(long);
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), (unsigned long)det, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			temp = ((ODWide)-matrix->m[0][1]) << sizeof(long);
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), (unsigned long)det, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			*im++ = 0;
			temp = ((ODWide)-matrix->m[1][0]) << sizeof(long);
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), (unsigned long)det, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			temp = ((ODWide)matrix->m[0][0]) << sizeof(long);
			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), (unsigned long)det, kODIgnoreRemainder)) == kODFixedMinusInfinity)
				return true;
			*im++ = 0;
			if (flags & kODTranslateXform)
			{	shift -= fixedPrecision;
				temp = (ODWide)matrix->m[1][0] * (ODWide)matrix->m[2][1] - (ODWide)matrix->m[2][0] * (ODWide)matrix->m[1][1];
				if (shift < MyWideScale(&temp) - (wideSize - 2))
					return true;
				if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), (unsigned long)det, kODIgnoreRemainder)) == kODFixedMinusInfinity)
					return true;
				temp  = (ODWide)matrix->m[0][1] * (ODWide)matrix->m[2][0] - (ODWide)matrix->m[2][1] * (ODWide)matrix->m[0][0];
				if (shift < MyWideScale(&temp) - (wideSize - 2))
					return true;
				if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), (unsigned long)det, kODIgnoreRemainder)) == kODFixedMinusInfinity)
					return true;
			}
			else
			{	*im++ = 0;
				*im++ = 0;
			}
			*im++ = kODFract1;
		}
		break;
		case kODPerspectiveXform:
		{	ODWide inv[3][3];
			bias delta = { 30, 16, 16, 2 };

			inv[0][0] = (ODWide)m0[4] * (ODWide)m0[8] - (ODWide)m0[5] * (ODWide)m0[7];
			inv[0][1] = (ODWide)m0[2] * (ODWide)m0[7] - (ODWide)m0[1] * (ODWide)m0[8];
			inv[0][2] = (ODWide)m0[1] * (ODWide)m0[5] - (ODWide)m0[2] * (ODWide)m0[4];
			inv[1][0] = (ODWide)m0[5] * (ODWide)m0[6] - (ODWide)m0[3] * (ODWide)m0[8];
			inv[1][1] = (ODWide)m0[0] * (ODWide)m0[8] - (ODWide)m0[6] * (ODWide)m0[2];
			inv[1][2] = (ODWide)m0[2] * (ODWide)m0[3] - (ODWide)m0[0] * (ODWide)m0[5];
			inv[2][0] = (ODWide)m0[3] * (ODWide)m0[7] - (ODWide)m0[6] * (ODWide)m0[4];
			inv[2][1] = (ODWide)m0[6] * (ODWide)m0[1] - (ODWide)m0[0] * (ODWide)m0[7];
			inv[2][2] = (ODWide)m0[0] * (ODWide)m0[4] - (ODWide)m0[3] * (ODWide)m0[1];

			return MxWideNorm(inv, imatrix, &delta);
		}
#endif // !defined(_PLATFORM_UNIX_)
		break;
		case kODUnknownXform:
		case kODInvalidXform:
			return true;
	}
	return false;
}


/*	MxMul multiples a list of vectors by the given matrix and returns the result in place.
	( x1 y1 1 ) = ( x0 y0 1 ) * matrix;	*/
ODBoolean
MxMul(const ODMatrix *matrix, ODTransformType flags, ODPoint *vector, ODSLong cnt)
{
	register const ODFixed *m0 = matrix->m[0];
	register ODFixed *vectorPtr = &vector->x;
	register ODSLong count = cnt;
	
	switch(flags) {  /* do nothing if identity matrix */
		case kODTranslateXform:
			m0 += 6;  /* advance to x translate */
			{	register ODFixed x = *m0++;  /* x translation */
				register ODFixed y = *m0;  /* y translation */
					
				while (count--)
				{	if (FixAddCheck(x, vectorPtr))
						goto MxMulError;
					++vectorPtr;
					if (FixAddCheck(y, vectorPtr))
						goto MxMulError;
					++vectorPtr;
				}
			}
		break;
		case kODScaleXform:
			{	register ODFixed xScale = *m0; 
				register ODFixed yScale = *(m0 + 4);
					
				while (count--)
				{	ODFixed temp;
				
					if (FixMulCheck(*vectorPtr, xScale, &temp))
						goto MxMulError;
					*vectorPtr++ = temp;
					if (FixMulCheck(*vectorPtr, yScale, &temp))
						goto MxMulError;
					*vectorPtr++ = temp;
				}
			}
		break;
		case kODTranslateXform + kODScaleXform:
			{	register ODFixed xScale = *m0;
				register ODFixed yScale = *(m0 + 4);
				register ODFixed x, y;
					
				m0 += 6;  /* advance to x translate */
				x = *m0++;  /* x translation */
				y = *m0;  /* y translation */
				while (count--)
				{	ODFixed tempX = *vectorPtr, tempY = *(vectorPtr + 1);
					*vectorPtr = x;
					if (FixMulAddCheck(xScale, tempX, vectorPtr))
						goto MxMulError;
					*++vectorPtr = y;
					if (FixMulAddCheck(yScale, tempY, vectorPtr))
						goto MxMulError;
					++vectorPtr;
				}
			}
		break;
		case kODLinearXform:
		case kODLinearXform + kODTranslateXform:
			{	register const ODFixed *m1;	/* will ODPoint at y translation vector */
				register ODFixed xScale = *m0;
				register ODFixed alpha = *(m0 += 1);
				register ODFixed beta = *(m0 += 2);
				register ODFixed yScale = *(m0 += 1);
				register ODFixed xTemp, yTemp;
				
				m0 += 2; /* ODPoint at x translation vector */
				m1 = m0 + 1;
				
				while (count--)
				{	xTemp = *vectorPtr, yTemp = *(vectorPtr + 1);
					*vectorPtr = *m0;
					if (MulMulAddCheck(xTemp, xScale, yTemp, beta, fixedPrecision, vectorPtr))
						goto MxMulError;
					*++vectorPtr = *m1;
					if (MulMulAddCheck(xTemp, alpha, yTemp, yScale, fixedPrecision, vectorPtr))
						goto MxMulError;
					++vectorPtr;
				}
			}
		break;
		case kODPerspectiveXform:
			{	register ODFixed *dst = vectorPtr;
				register ODFixed m, divisor;
				register ODFract u, v;
				ODFixed cpy[3];
				cpy[2] = kODFixed1;
				u = m0[2];
				v = m0[5];
				m = ODFractToFixed(m0[8]);
				while (count--)
				{	divisor = FracMul(u, vectorPtr[0]) + FracMul(v, vectorPtr[1]) + m;
					if (divisor <= 0)
						goto MxMulError;
					cpy[0] = *vectorPtr++;
					cpy[1] = *vectorPtr++;
					*dst++ = MyVectorMultiplyDivide(3, cpy, 1, m0, 3, divisor);
					*dst++ = MyVectorMultiplyDivide(3, cpy, 1, m0+1, 3, divisor);
				}
			}
		break;
		case kODUnknownXform:
		case kODInvalidXform:
		MxMulError:
			return true;
	}  /* end of switch */
	return false;
}


/*	MxMulOffset - Multiples a list of vectors by the given matrix and returns the result in place.
	( x1 y1 1 ) = ( x0-hOffset y0-vOffset 1 ) * matrix;	*/
static ODBoolean
MxMulOffset(const ODMatrix *matrix, ODTransformType flags, ODPoint *vector, ODSLong cnt, ODFixed hOffset, ODFixed vOffset)
{
	register ODFixed *vectorPtr = &vector->x;
	register ODFixed h = hOffset, v = vOffset;

	if (h || v)
	{	ODMatrix translation;
		ODBoolean error;
		translation.m[2][0] = -h;
		translation.m[2][1] = -v;
		error = MxMul(&translation, kODTranslateXform, vector, cnt);
		if( error ) return error;
	}
	return MxMul(matrix, flags, vector, cnt);
}


/* MxConcat - Postmultiplies matrix B by matrix A.
	B = B * A;
 	returns true if error occurs (either invalid matrix or overflow in calculation)
 */
ODBoolean
MxConcat(register const ODMatrix *a, register ODTransformType aFlags,
		 register ODMatrix *b, register ODTransformType bFlags)
{
	ODBoolean savedB = false;
	ODMatrix oldB;

	if (aFlags == kODUnknownXform && (aFlags = MxType(a)) == kODInvalidXform)
		return true;
	if (bFlags == kODUnknownXform && (bFlags = MxType(b)) == kODInvalidXform)
		return true;
	if (aFlags == kODIdentityXform)
		return false;
	if (bFlags == kODIdentityXform)
	{	*b = *a;
		return false;
	}
	if (bFlags != kODPerspectiveXform && aFlags < kODScaleXform)
	{	ODPoint hk = *(ODPoint *)&b->m[2][0];
		if (FixAddCheck(a->m[2][0], &b->m[2][0]) || FixAddCheck(a->m[2][1], &b->m[2][1]))
		{	*(ODPoint *)&b->m[2][0] = hk;
			goto rangeError;
		}
		return false;
	}
	if (aFlags < kODLinearXform)
	{	register fastInt i, j;
		oldB = *b;
		savedB = true;
		for (i = 3; i--; )
			for (j = 2; j--; )
			{	if (aFlags >= kODScaleXform)
					if (FixMulCheck(b->m[i][j], a->m[j][j], &b->m[i][j]))
						goto rangeError;
				if (aFlags & kODTranslateXform)
					if (i == 2) {
						if (bFlags == kODPerspectiveXform) {
							if (FracMulAddCheck(b->m[2][2], a->m[2][j], &b->m[2][j]))
								goto rangeError;
						} else if (FixAddCheck(a->m[2][j], &b->m[2][j]))
							goto rangeError;
					} else if (bFlags == kODPerspectiveXform)
						if (FracMulAddCheck(b->m[i][2], a->m[2][j], &b->m[i][j]))
							goto rangeError;
			}
		return false;
	}
	if (aFlags < kODPerspectiveXform)
	{	
		ODMatrix c;
		register fastInt i, j;
		for (i = 3; i--; )
		{	c.m[i][2] = b->m[i][2];
			for (j = 2; j--; )
			{	if (aFlags >= kODScaleXform)
				{	if (FixMulCheck(b->m[i][j], a->m[j][j], &c.m[i][j]))
						goto rangeError;
					if (aFlags >= kODLinearXform)
						if (FixMulAddCheck(b->m[i][1-j], a->m[1-j][j], &c.m[i][j]))
							goto rangeError;
				}
				else
					c.m[i][j] = b->m[i][j];
				if (aFlags & kODTranslateXform)
					if (i == 2) {
						if (bFlags == kODPerspectiveXform) {
							if (FracMulAddCheck(b->m[2][2], a->m[2][j], &c.m[2][j]))
								goto rangeError;
						} else
							if (FixAddCheck(a->m[2][j], &c.m[2][j]))
								goto rangeError;
					} 
					  else if (bFlags == kODPerspectiveXform)
						if (FracMulAddCheck(b->m[i][2], a->m[2][j], &c.m[i][j]))
							goto rangeError;
			}
		}
		*b = c;
		return false;
	}
perspectiveCase:	/* aFlags == kODPerspectiveXform */
	{	ODWide c[3][3];
		register fastInt i, j;
		bias delta = { 16, 16, 16, 16 };
		for (i = 3; i--; )
			for (j = 3; j--; )
			{	ODWide temp;
				ODWideShift(ODWideMultiply(b->m[i][2], a->m[2][j], &c[i][j]), fractPrecision - fixedPrecision);
				ODWideAdd(&c[i][j], ODWideMultiply(b->m[i][1], a->m[1][j], &temp));
				ODWideAdd(&c[i][j], ODWideMultiply(b->m[i][0], a->m[0][j], &temp));
			}
		return MxWideNorm(c, b, &delta);
	}

rangeError:
	if (savedB) *b = oldB;
	goto perspectiveCase;
}


/*										|  1   0   0	|
	currentMatrix =  currentMatrix *	|  0   1   0	|
										|  tx  ty  1	|
*/
ODBoolean
MxMove(ODMatrix *currentMatrix, ODTransformType flags, register ODFixed tx, register ODFixed ty)
{
	if (flags == kODPerspectiveXform)
	{	register ODFixed *fixPtr = &currentMatrix->m[0][0];
		register ODFract *fracPtr = &currentMatrix->m[0][2];
		register fastInt counter = 3;
		
		do
		{	register ODFract u = fracPtr[0];
		
			if (FracMulAddCheck(u, tx, &fixPtr[0]) || FracMulAddCheck(u, ty, &fixPtr[1]))
				return true;
			fracPtr += 3;
			fixPtr += 3;
		} while (--counter);
	} else if (FixAddCheck(tx, &currentMatrix->m[2][0]) || FixAddCheck(ty, &currentMatrix->m[2][1]))
		return true;
	return false;
}


ODBoolean
MxMoveTo(register ODMatrix *currentMatrix, ODTransformType flags, ODFixed x, ODFixed y)
{
	if (flags == kODPerspectiveXform)
	{	register ODFract u = currentMatrix->m[2][2];
		
		if (FracDivAddCheck(-currentMatrix->m[2][0], u, &x) ||
			FracDivAddCheck(-currentMatrix->m[2][1], u, &y))
		{
			return true;
		} 
		return MxMove(currentMatrix, kODPerspectiveXform, x, y);
	} else
	{	register ODFixed *fixPtr = &currentMatrix->m[2][0];
	
		*fixPtr++ = x;
		*fixPtr = y;
	}
	return false;
}

/*
 * scales a matrix by sx and sy.
 *
 *								| sx 0   0	|
 *    currentMx =  currentMx *	| 0   sy 0	|;
 *								| 0   0  1	|
 *
 */
ODBoolean
MxScale(ODMatrix *currentMatrix, register ODFixed sx, register ODFixed sy)
{	
	register ODFixed *m = currentMatrix->m[0];
	
	if (FixMulCheck( sx, *m, m))
		return true;
	m++;
	if (FixMulCheck( sy, *m, m))
		return true;
	m += 2;
	if (FixMulCheck( sx, *m, m))
		return true;
	m++;
	if (FixMulCheck( sy, *m, m))
		return true;
	m += 2;
	if (FixMulCheck( sx, *m, m))
		return true;
	m++;
	if (FixMulCheck( sy, *m, m))
		return true;
	return false;
}


/*										| 1    		ySkew 	0	|
	currentMatrix =  currentMatrix *	| xSkew     1    	0   |
	    								| 0    		0    	1   |	*/
ODBoolean
MxSkew(ODMatrix *currentMatrix, register ODFixed xSkew, register ODFixed ySkew)
{
	register ODFixed *matrixPtr = currentMatrix->m[0];
	register fastInt counter = 3;
	
	do
	{	register ODFixed temp = *matrixPtr;
		
		if (xSkew)
			if (FixMulAddCheck(*(matrixPtr+1), xSkew, matrixPtr))
				return true;
		matrixPtr += 1;
		if (ySkew)
			if (FixMulAddCheck(temp, ySkew, matrixPtr))
				return true;
		matrixPtr += 2;
	} while (--counter);
	return false;
}


/*									| cosine	sine  	0	|
	currentMatrix = currentMatrix *	| -sine		cosine  0	| 
									| 0			0		1	|	*/
/* note that while this would almost never fail, in extreme pathological cases, it could , for */
/* instance if the ODMatrix [0][0] and [0][1] are both > ff(23200) */
ODBoolean
MxRotate(ODMatrix *currentMatrix, ODFixed angle)
{
	ODFract cos;
	register ODFract s = ODFractSinCos(angle, &cos);
	register ODFract c = cos;
	register ODFixed *src = currentMatrix->m[0];
	register ODFixed *dst = src;
	register fastInt counter = 3;	

	do
	{	register ODFixed x = *src++;
		register ODFixed y = *src;
		
		src += 2;
		*dst = FracMul(x, c);
		if (FracMulAddCheck(y, -s, dst++))
			return true;
		*dst = FracMul(x, s);
		if (FracMulAddCheck(y, c, dst)) 
			return true;
		dst += 2;
	} while (--counter);
	return false;
}
