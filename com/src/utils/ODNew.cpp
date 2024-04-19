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
// @(#) 1.12 com/src/utils/ODNew.cpp, odutils, od96os2, odos29646d 7/15/96 17:58:56 [ 11/15/96 15:28:47 ]
/*
	File:		ODNew.cpp

	Contains:	Implementation of a new operator for OpenDoc

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
#ifndef __TYPES__
#include <Types.h>
#endif
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
#ifndef _EXCEPT_
#include "Except.h"
#endif
#ifndef __ODPAGTUN__
#include <ODPagTun.h>
#endif
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)

#pragma segment ODNew

//==============================================================================
// Global new and delete operators
//==============================================================================

//------------------------------------------------------------------------------
// operator new
//------------------------------------------------------------------------------

void* operator new(size_t size)
{
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	return ODNewPtr(size, kDefaultHeapID);
#else // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	if (!size)
		return NULL;
	void *blockPtr = SOMMalloc(size);
	if (!blockPtr)
		THROW(kODErrOutOfMemory);
	return(blockPtr);
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
}

//------------------------------------------------------------------------------
// operator new
//------------------------------------------------------------------------------

void* operator new(size_t size, ODMemoryHeapID heap)
{
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	return ODNewPtr(size, heap);
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
	void *blockPtr = SOMMalloc(size);
	if (!blockPtr)
		THROW(kODErrOutOfMemory);
	return(blockPtr);
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
}

//------------------------------------------------------------------------------
// operator delete
//------------------------------------------------------------------------------

void operator delete(void* object)
{
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODDisposePtr(object);
#else // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	if (object)
		SOMFree(object);
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
}


#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || \
    (defined(_PLATFORM_UNIX_) && defined(__IBMCPP__))
void * operator new[](size_t size)
{
  return ::operator new(size);
}

void* operator new[](size_t size, ODMemoryHeapID heap)
{
  return ::operator new(size, heap);
}

void operator delete[](void *object)
{
  ::operator delete(object);
}
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) ||
       // (defined(_PLATFORM_UNIX_) && defined(__IBMCPP__)
