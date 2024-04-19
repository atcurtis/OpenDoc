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
// @(#) 1.4 com/src/utils/FlipEnd.cpp, odutils, od96os2, odos29646d 7/15/96 17:58:01 [ 11/15/96 15:28:43 ]
/*
	File:		FlipEnd.cpp

	Contains:	routines to manipulate endianness of memory

	Owned by:	David McCusker

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _FLIPEND_
#include "FlipEnd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*=========================================================================*/
/* ODUShort endianness flipping */
/*=========================================================================*/

ODUShort ODFlipShort(ODUShort n)
	/* n is a single 2-byte int; return it with flipped endianness */
{
	/* note it is unorthodox, but legal, to modify stack arguments */
	register ODUShort* p = &n;

	char c = *(char*) p;
	*(char*) p = ((char*) p)[1]; 
	((char*) p)[1] = c;

	return *p;
}

void ODFlipShortArray(register ODUShort* n, unsigned long count)
	/* n points to count 2-byte ints; flip each int's endianness */
{
	register char c;
	ODUShort* end = n + count; /* one past the end */
	
	--n;
	while (++n < end)
	{
		c = *(char*) n;
		*(char*) n = ((char*) n)[1]; 
		((char*) n)[1] = c;
	}
}

/*=========================================================================*/
/* ODULong endianness flipping */
/*=========================================================================*/

ODULong ODFlipLong(ODULong n)
	/* n is a single 4-byte int; return it with flipped endianness */
{
	/* note it is unorthodox, but legal, to modify stack arguments */
	register ODULong* p = &n;

	register char c = *(char*) p;
	*(char*) p = ((char*) p)[3]; 
	((char*) p)[3] = c; 
	
	c = ((char*) p)[1];
	((char*) p)[1]= ((char*) p)[2]; 
	((char*) p)[2] = c; 
	
	return *p;
}

void ODFlipLongArray(register ODULong* n, unsigned long count)
	/* n points to count 4-byte ints; flip each int's endianness */
{
	register char c;
	ODULong* end = n + count; /* one past the end */
	
	--n;
	while (++n < end)
	{
		c = *(char*) n;
		*(char*) n = ((char*) n)[3]; 
		((char*) n)[3] = c; 
		
		c = ((char*) n)[1];
		((char*) n)[1]= ((char*) n)[2]; 
		((char*) n)[2] = c; 
	}
}

#ifndef OD_BUG // function doesn't work, and no one uses it.
/*=========================================================================*/
/* structure endianness flipping */
/*=========================================================================*/

void ODFlipStruct(void* structure, const short* groups)
	/* Invert the endianness of the contents of memory in structure
	 * according to the layout described by groups, which should
	 * be a zero-terminated array of shorts, where each short describes
	 * the size of the next chunk of memory in structure to be processed.
	 * A negative value -x in the groups array indicates a block
	 * of endianness-neutral memory, like a string, and causes x bytes
	 * of memory to be skipped over.  A positive value x in the groups
	 * array indicates an x byte block of memory which should have its
	 * bytes flipped end for end.  Only positive values in the set
	 * { 2, 4, 8 } are handled. (Other positive values are handled like
	 * negative values: space is skipped). Example:
	 *
	 * struct Foo {
	 *     short alpha;
	 *     long  beta;
	 *     char  gamma[8];
	 *     long  delta;
	 * };
	 * const short fooGroups[] = {
	 *     2, // alpha
	 *     4, // beta
	 *    -8, // gamma 
	 *     4, // delta
	 *     0, // zero-termination
	 * };
	 */
{
	register char* p = (char*) structure;
	register char c;

	short g = *groups++;                /* next groups entry */

	while (g)
	{
		/* we expect 2 and 4 to be the most common values */
		if (g == 2)
		{
			c = *p; *p = p[1]; p[1] = c;
			p += 2;
		}
		else if (g == 4)
		{
			     /* swap bytes 0 and 3 */
			c = *p; *p = p[3]; p[3] = c;
			
			++p; /* swap bytes 1 and 2 */
			c = *p; *p = p[1]; p[1] = c;
			
			p += 3;
		}
		else if (g == 8)
		{
			     /* swap bytes 0 and 7 */
			c = *p; *p = p[7]; p[7] = c;
			
			++p; /* swap bytes 1 and 6 */
			c = *p; *p = p[5]; p[5] = c;
			
			++p; /* swap bytes 2 and 5 */
			c = *p; *p = p[3]; p[3] = c;
			
			++p; /* swap bytes 3 and 4 */
			c = *p; *p = p[1]; p[1] = c;
			
			p += 5;
		}
		else if (g < 0)
			p += -g;
		else
			p += g;
		
		g = *groups++;
	}
}
#endif // OD_BUG

#ifdef __cplusplus
}
#endif
