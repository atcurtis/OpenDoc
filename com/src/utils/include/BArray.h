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
// @(#) 1.5 com/src/utils/include/BArray.h, odutils, od96os2, odos29646d 8/19/96 11:54:18 [ 11/15/96 15:29:32 ]
/*
	File:		BArray.h

	Contains:	ByteArray collection

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _BARRAY_
#define _BARRAY_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


/* DisposeByteArrayStruct is a macro instead of a function so it can take a different
   parameter type than DisposeByteArray(), and still set the disposed pointer field
   of the structure to null.
   DisposeByteArray() must never be called on a ByteArray structure; DisposeByteArray()
   disposes the structure which is usually a distaster.
*/

#define	DisposeByteArrayStruct(byteArray)		\
	do{											\
		if (byteArray._buffer != kODNULL) {		\
			delete [] byteArray._buffer;	\
			byteArray._buffer = kODNULL;		\
		}										\
		byteArray._length = 0;					\
		byteArray._maximum = 0;					\
	}while(0)


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

extern "C" {
	_DLLIMPORTEXPORT_ ODByteArray*	CreateByteArray(void* buffer, ODULong size);
	_DLLIMPORTEXPORT_ ODByteArray	CreateByteArrayStruct(void* buffer, ODULong size);

	_DLLIMPORTEXPORT_ ODByteArray*	CreateEmptyByteArray(ODULong maximum);
	_DLLIMPORTEXPORT_ ODByteArray	CreateEmptyByteArrayStruct(ODULong maximum);

	_DLLIMPORTEXPORT_ void UseByteArray(ODByteArray* ba, void* buffer, ODULong size);

	_DLLIMPORTEXPORT_ ODByteArray*	CopyByteArray(ODByteArray* fromBA);
	_DLLIMPORTEXPORT_ ODByteArray	CopyByteArrayStruct(ODByteArray* fromBA);

	_DLLIMPORTEXPORT_ void		DisposeByteArray(ODByteArray* array);

	_DLLIMPORTEXPORT_ ODBoolean	AreByteArraysEqual(ODByteArray* ba1, ODByteArray* ba2);
}

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif


#endif 	// _BARRAY_
