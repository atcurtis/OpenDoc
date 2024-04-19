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
// @(#) 1.12 com/src/utils/IODMath.cpp, odutils, od96os2, odos29646d 8/2/96 18:24:47 [ 11/15/96 15:47:52 ]
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
/*  129861  7/18/95 ced  Reduce rounding error for ODFixedMultiply  */
/*                       and ODFractMultiply.                       */
/********************************************************************/
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
#include <builtin.h>
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
#include <math.h>

#ifndef _ODMATH_
#include "ODMath.h"
#endif
#ifndef __ODPAGTUN__
#include <ODPagTun.h>
#endif

//=============================================================================
// Constants
//=============================================================================


#define longSize    32
#define wideSize    64

#define highBit      ((ODULong) 0x80000000)
#define ODFloatToFract(a) ((ODFract)(((double)(a)) * kODFract1))


ODFixed  ODFixedMultiply( register ODFixed a, register ODFixed b ) {
   ODWide result;
   ODWideMultiply(a, b, &result);
   ODWideShift(&result, 16);
#if !defined(_PLATFORM_UNIX_)
	if (ODWideIsLong(&result))
   	return result.lo;
	if (result.hi > 0)
		return kODFixedInfinity;
	else
		return kODFixedMinusInfinity;
#else // defined(_PLATFORM_UNIX_)
	if (ODWideIsLong(&result))
   	return (unsigned long) result;
	if (result > 0)
		return kODFixedInfinity;
	else
		return kODFixedMinusInfinity;
#endif // !defined(_PLATFORM_UNIX_)
}

ODFixed  ODFixedDivide( ODFixed a, ODFixed b ) {
   ODWide dividend;
   ODSLong remainder;
#if !defined(_PLATFORM_UNIX_)
   dividend.lo = a;
   dividend.hi = (a < 0) ? -1 : 0;
#else // defined(_PLATFORM_UNIX_)
   dividend = a;
#endif // !defined(_PLATFORM_UNIX_)
   ODWideShift(&dividend, -16);
   return ODWideDivide(&dividend, b, &remainder);
}

ODFract  ODFractMultiply( ODFract a, ODFract b ) {
   ODWide result;
   ODWideMultiply(a, b, &result);
   ODWideShift(&result, 30);
#if !defined(_PLATFORM_UNIX_)
   return result.lo;
#else // defined(_PLATFORM_UNIX_)
   return (unsigned long)result;
#endif // !defined(_PLATFORM_UNIX_)
}

ODFract  ODFractDivide( ODFract a, ODFract b ) {
   ODWide dividend;
   ODSLong remainder;
#if !defined(_PLATFORM_UNIX_)
   dividend.lo = a;
   dividend.hi = (a < 0) ? -1 : 0;
#else // defined(_PLATFORM_UNIX_)
   dividend = a;
#endif // !defined(_PLATFORM_UNIX_)
   ODWideShift(&dividend, -30);
   return ODWideDivide(&dividend, b, &remainder);
}

ODFract  ODFractSinCos( ODFixed angle, ODFract *cos ) {
   double sine, cosine;
#if defined(_PLATFORM_UNIX_)
	float dAngle;
   dAngle =  ODFixedToFloat(angle);
	cosine = ::cos(dAngle);
	sine = sin(dAngle);
#else // defined(_PLATFORM_UNIX_)
   sine = ODFloatToFract(_fsincos(ODFixedToFloat(angle), &cosine));
#endif // defined(_PLATFORM_UNIX_)
   *cos = ODFloatToFract(cosine);
   return ODFloatToFract(sine);
}

/*
 * Approximates the square root of a positive ODWide number.
 */

ODULong ODWideSquareRoot( const ODWide *src )
{
#if !defined(_PLATFORM_UNIX_)
   if (src->hi < 0) return 0;      // Only works for positive numbers
   ODULong hi = src->hi, number;
   ODULong lo = src->lo;
#else // defined(_PLATFORM_UNIX_)
   if (*src < 0) return 0;         // Only works for positive numbers
   ODULong hi = (*src >> 32), number;
   ODULong lo = (long)*src;
#endif // !defined(_PLATFORM_UNIX_)


   if (hi <= 0x00007fff)
       number = (((hi << 16) & 0xffff0000) + ((lo >> 16) & 0x0000ffff));
   else {
          if (hi <= 0x0000ffff) {
              number  = (((hi << 16) & 0xffff0000) + ((lo >> 16) & 0x0000ffff));
              number  = ((number >> 2) & 0x3fffffff);    /* 8000-FFFF */
          }
          else
              number  = hi;
   }
   number = (ODULong)(sqrt( (double) number) * 256 );
   if (hi <= 0x0000ffff) {
      if (hi > 0x00007fff)
          number = number << 1;
   }
   else
      number = number << 8;

   return number;
}
#if defined(_PLATFORM_UNIX_)
ODWide*  ODWideMultiply( ODSLong a, ODSLong b, ODWide *result )
{
	*result = (ODWide)a * (ODWide)b;
	return(result);
}

ODSLong  ODWideDivide( const ODWide *dividend,
                     ODSLong divisor, ODSLong *remainder)
{
	ODWide quotient;

	quotient = *dividend / divisor;

	if (divisor == 0 || !ODWideIsLong(&quotient))
		return(kODFixedMinusInfinity);

	*remainder = *dividend - (quotient * divisor);

	return (ODSLong) quotient;
}
#endif // defined(_PLATFORM_UNIX_)
