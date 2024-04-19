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
// @(#) 1.11 com/src/utils/include/ODNew.h, odutils, od96os2, odos29646d 7/15/96 18:01:05 [ 11/15/96 15:29:33 ]
/*
	File:		ODNew.h

	Contains:	Inline versions of operator new and operator delete for OpenDoc

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _ODNEW_
#define _ODNEW_

#include <stddef.h>

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

//==============================================================================
// Global function declarations
//==============================================================================

void* operator new(size_t size, ODMemoryHeapID);		// from a specific heap
void* operator new(size_t);							// from the client heap
void operator delete(void*);						// return memory to pool 
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || \
    (defined(_PLATFORM_UNIX_) && defined(__IBMCPP__))
// Note:  Variants of new[] must also be overridden
void *operator new[](size_t);
void* operator new[](size_t size, ODMemoryHeapID);
void operator delete[](void*);
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) ||
       // (defined(_PLATFORM_UNIX_) && defined(__IBMCPP__))

#endif
