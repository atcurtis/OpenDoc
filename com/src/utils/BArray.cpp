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
// @(#) 1.7 com/src/utils/BArray.cpp, odutils, od96os2, odos29646d 8/19/96 11:47:22 [ 11/15/96 15:28:41 ]
/*
	File:		BArray.cpp

	Contains:	ByteArray implementation

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _ODEXCEPT_
#include <ODExcept.h>
#endif

#ifndef SOM_Module_OpenDoc_Errors_defined
#include <ErrorDef.xh>
#endif

#ifdef _UNICODE_
  ODShortArray  CreateShortArrayStruct(void* buffer, ODULong size);
  ODShortArray  CreateEmptyShortArrayStruct(ODULong maximum);
  ODShortArray  CopyShortArrayStruct(ODShortArray* fromSA);
  void          DisposeShortArrayStruct(ODShortArray* byteArray);
#endif

//------------------------------------------------------------------------------
// CreateByteArray
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODByteArray* CreateByteArray(void* buffer, ODULong size)
{
	ODByteArray*	ba = (ODByteArray*) SOMMalloc( sizeof(ODByteArray) );

	if (ba != kODNULL) {

		ba->_buffer = (octet*) SOMCalloc(size, sizeof(octet) );

		if (ba->_buffer) {
			memcpy(ba->_buffer, buffer, size);
			ba->_length = size;
			ba->_maximum = size;
		}
		else
			THROW(kODErrOutOfMemory);
	}
	else
		THROW(kODErrOutOfMemory);
	
	return ba;
}

//------------------------------------------------------------------------------
// CreateByteArrayStruct
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODByteArray CreateByteArrayStruct(void* buffer, ODULong size)
{
	ODByteArray	ba = CreateEmptyByteArrayStruct(size);

	if ( size > 0 )
	{
		memcpy(ba._buffer, buffer, size);
		ba._length = size;
	}
	
	return ba;
}

//------------------------------------------------------------------------------
// CreateEmptyByteArray
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODByteArray* CreateEmptyByteArray(ODULong maximum)
{
	ODByteArray*	ba = (ODByteArray*) SOMMalloc( sizeof(ODByteArray) );
	octet*		buffer = (octet*) SOMCalloc(maximum, sizeof(octet) );


	if ((ba != kODNULL) && (buffer != kODNULL)) {
		memset (buffer, '\0', maximum);
		ba->_buffer = (octet *) buffer;
		ba->_length = 0;
		ba->_maximum = maximum;
	}
	else {
		SOMFree ( ba );
		SOMFree ( buffer );
		THROW(kODErrOutOfMemory);
	}
	return ba;
}

//------------------------------------------------------------------------------
// CreateEmptyByteArrayStruct
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODByteArray CreateEmptyByteArrayStruct(ODULong maximum)
{
	ODByteArray	ba;

	if ( maximum > 0 )
	{
		ba._buffer = (octet*) SOMCalloc(maximum, sizeof(octet) );
		if ( ba._buffer == kODNULL )
			THROW(kODErrOutOfMemory);

		memset (ba._buffer, '\0', maximum);
	}
	else
		ba._buffer = kODNULL;

	ba._length = 0;
	ba._maximum = maximum;

	return ba;
}

//------------------------------------------------------------------------------
// UseByteArray
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void UseByteArray(ODByteArray* ba, void* buffer, ODULong size)
{
	ba->_buffer = (octet *) buffer;
	ba->_length = size;
	ba->_maximum = size;
}

//------------------------------------------------------------------------------
// CopyByteArray
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODByteArray* CopyByteArray(ODByteArray* fromBA)
{
	ODByteArray*	ba = (ODByteArray*) SOMMalloc( sizeof(ODByteArray) );
	octet*		buffer = (octet*) SOMCalloc(fromBA->_maximum, sizeof(octet) );


	if ((ba != kODNULL) && (buffer != kODNULL)) {
		ba->_buffer = (octet*) buffer;
		memcpy(ba->_buffer, fromBA->_buffer, fromBA->_length);
		ba->_length = fromBA->_length;
		ba->_maximum = fromBA->_maximum;
	}
	else {
		SOMFree (ba);
		SOMFree (buffer);
		THROW(kODErrOutOfMemory);
	}
	return ba;	
}

//------------------------------------------------------------------------------
// CopyByteArrayStruct
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODByteArray CopyByteArrayStruct(ODByteArray* fromBA)
{
	ODByteArray	ba = CreateEmptyByteArrayStruct(fromBA->_maximum);

	if ( fromBA->_length > 0 )
	{
		memcpy(ba._buffer, fromBA->_buffer, fromBA->_length);
		ba._length = fromBA->_length;
	}

	return ba;	
}

//------------------------------------------------------------------------------
// DisposeByteArray
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void DisposeByteArray(ODByteArray* array)
{
	if (array->_buffer != kODNULL)
		SOMFree ( array->_buffer );
	SOMFree ( array );
}

//------------------------------------------------------------------------------
// AreByteArraysEqual
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODBoolean AreByteArraysEqual(ODByteArray* ba1, ODByteArray* ba2)
{
	return (((ba1->_length == ba2->_length) &&
			!memcmp(ba1->_buffer, ba2->_buffer, ba1->_length)) ? kODTrue : kODFalse);
}
#ifdef _UNICODE_


//------------------------------------------------------------------------------
// CreateShortArrayStruct
//------------------------------------------------------------------------------

ODShortArray  CreateShortArrayStruct(void* buffer, ODULong size)
{
        ODShortArray     ba = CreateEmptyShortArrayStruct(size);

        if ( size > 0 )
        {
                memcpy(ba._buffer, buffer, size);
                ba._length = size;
        }

        return ba;
}

//------------------------------------------------------------------------------
// CreateEmptyShortArrayStruct
//------------------------------------------------------------------------------

ODShortArray  CreateEmptyShortArrayStruct(ODULong maximum)
{
        ODShortArray     ba;

        if ( maximum > 0 )
        {
                ba._buffer = (unsigned short*) ODNewPtrClear(maximum);
                if ( ba._buffer == kODNULL )
                        THROW(kODErrOutOfMemory);
        }
        else
                ba._buffer = kODNULL;

        ba._length = 0;
        ba._maximum = maximum;

        return ba;
}

//------------------------------------------------------------------------------
// CopyShortArrayStruct
//------------------------------------------------------------------------------

ODShortArray  CopyShortArrayStruct(ODShortArray* fromBA)
{
        ODShortArray     ba = CreateEmptyShortArrayStruct(fromBA->_maximum);

        if ( fromBA->_length > 0 )
        {
                memcpy(ba._buffer, fromBA->_buffer, fromBA->_length);
                ba._length = fromBA->_length;
        }

        return ba;
}

//------------------------------------------------------------------------------
// DisposeShortArrayStruct
//------------------------------------------------------------------------------
/*
 *  [119548]
 */
void  DisposeShortArrayStruct(ODShortArray* byteArray)
{

  if (byteArray->_buffer != kODNULL)
  {
    SOMFree (byteArray->_buffer);
    byteArray->_buffer = kODNULL;
  }
  byteArray->_length = 0;
  byteArray->_maximum = 0;

}

#endif
