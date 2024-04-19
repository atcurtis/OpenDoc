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
// @(#) 1.5 com/src/utils/include/FlipEnd.h, odutils, od96os2, odos29646d 9/20/96 12:23:30 [ 11/15/96 15:28:59 ]
/*
	File:		FlipEnd.h

	Contains:	routines to manipulate endianness of memory

	Owned by:	David McCusker

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	To Do:
		Perhaps write assembler inlines for ODFlipShort() and ODFlipLong()
		on the Mac platform.
	In Progress:
		
*/

#ifndef _FLIPEND_
#define _FLIPEND_ 1

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* _PLATFORM_BIG_ENDIAN_ should be defined for big endian platforms
 * near other platform related defines (e.g. see ODTypesM.idl)
 */

/*=========================================================================*/
/* 2 byte endianness flipping */
/*=========================================================================*/

ODUShort ODFlipShort(ODUShort n);
	/* n is a single 2-byte int; return it with flipped endianness */

void ODFlipShortArray(ODUShort* a, unsigned long count);
	/* a points to count 2-byte ints; flip each int's endianness */

/*=========================================================================*/
/* 4 byte endianness flipping */
/*=========================================================================*/

ODULong ODFlipLong(ODULong n);
	/* n is a single 4-byte int; return it with flipped endianness */

void ODFlipLongArray(ODULong* a, unsigned long count);
	/* a points to count 4-byte ints; flip each int's endianness */

#ifndef OD_BUG // function doesn't work, and no one uses it.
/*=========================================================================*/
/* structure endianness flipping */
/*=========================================================================*/

void ODFlipStruct(void* structure, const short* groups);
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
	 *     long  beta;
	 *     char  gamma[8];
	 *     long  delta;
	 *     short alpha;
	 * };
	 * const short fooGroups[] = {
	 *     4, // beta
	 *    -8, // gamma 
	 *     4, // delta
	 *     2, // alpha
	 *     0, // zero-termination
	 * };
	 */

#endif // OD_BUG

/*=========================================================================*/
/* macros for conversion to and from standard (little endian) format */
/*=========================================================================*/

#ifdef _PLATFORM_BIG_ENDIAN_
	
#define ConvertODUShortToStd(n)        ODFlipShort(n)
#define ConvertODUShortFromStd(n)      ODFlipShort(n)
	
#define ConvertODSShortToStd(n)        ((ODSShort) ODFlipShort((ODUShort) n))
#define ConvertODSShortFromStd(n)      ((ODSShort) ODFlipShort((ODUShort) n))
	
#define ConvertODULongToStd(n)         ODFlipLong(n)
#define ConvertODULongFromStd(n)       ODFlipLong(n)
		
#define ConvertODSLongToStd(n)         ((ODSLong) ODFlipLong((ODULong) n))
#define ConvertODSLongFromStd(n)       ((ODSLong) ODFlipLong((ODULong) n))

#ifndef OD_BUG // function doesn't work, and no one uses it.
#define ConvertODStructToStd(s, g)     ODFlipStruct((s),(g))
#define ConvertODStructFromStd(s, g)   ODFlipStruct((s),(g))
#endif

#define ConvertODUShortArrayToStd(a,c)    ODFlipShortArray((a),(c))
#define ConvertODUShortArrayFromStd(a,c)  ODFlipShortArray((a),(c))

#define ConvertODSShortArrayToStd(a,c)    ODFlipShortArray((ODUShort*)(a),(c))
#define ConvertODSShortArrayFromStd(a,c)  ODFlipShortArray((ODUShort*)(a),(c))

#define ConvertODULongArrayToStd(a,c)     ODFlipLongArray((a),(c))
#define ConvertODULongArrayFromStd(a,c)   ODFlipLongArray((a),(c))

#define ConvertODSLongArrayToStd(a,c)     ODFlipLongArray((ODULong*)(a),(c))
#define ConvertODSLongArrayFromStd(a,c)   ODFlipLongArray((ODULong*)(a),(c))

#else

#define ConvertODUShortToStd(n)        (n)
#define ConvertODUShortFromStd(n)      (n)
	
#define ConvertODSShortToStd(n)        (n)
#define ConvertODSShortFromStd(n)      (n)
	
#define ConvertODULongToStd(n)         (n)
#define ConvertODULongFromStd(n)       (n)
		
#define ConvertODSLongToStd(n)         (n)
#define ConvertODSLongFromStd(n)       (n)
		
#define ConvertODStructToStd(s, g)     /* do nothing */
#define ConvertODStructFromStd(s, g)   /* do nothing */

#define ConvertODUShortArrayToStd(a,c)    /* do nothing */
#define ConvertODUShortArrayFromStd(a,c)  /* do nothing */

#define ConvertODSShortArrayToStd(a,c)    /* do nothing */
#define ConvertODSShortArrayFromStd(a,c)  /* do nothing */

#define ConvertODULongArrayToStd(a,c)     /* do nothing */
#define ConvertODULongArrayFromStd(a,c)   /* do nothing */

#define ConvertODSLongArrayToStd(a,c)     /* do nothing */
#define ConvertODSLongArrayFromStd(a,c)   /* do nothing */

#endif /* _PLATFORM_BIG_ENDIAN_ */


#ifdef __cplusplus
}
#endif

#endif
/* _FLIPEND_ */
