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
// @(#) 1.13 com/src/utils/include/ODMemory.h, odutils, od96os2, odos29646d 7/15/96 18:01:00 [ 11/15/96 15:29:00 ]
/*
	File:		ODMemory.h

	Contains:	Memory calls (wrappers around MemMgr.h) that throw exceptions.

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _ODMEMORY_
#define _ODMEMORY_

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
// ***********************
// See notes in ODMemory.cpp that explain ODMemory porting rationale (e.g., why
// certain functions are #ifdef'ed out or stubbed out, etc.).
// ***********************
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#if defined(_PLATFORM_WIN32_)
#include <wtypes.h>
#include <winbase.h>
#endif // defined(_PLATFORM_WIN32_)


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

#ifdef __cplusplus
#define DEFAULT(V)	=(V)
extern "C" {
#else
#define DEFAULT(V)	/**/
#endif

//========================================================================================
// Type definitions
//========================================================================================

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
typedef struct MemHeap MemHeap;
typedef MemHeap* ODMemoryHeapID;		// An anonymous pointer type
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#if defined(_PLATFORM_WIN32_)
typedef HANDLE ODMemoryHeapID;
#endif // defined(_PLATFORM_WIN32_)

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
typedef void  *ODMemoryHeapID;
#endif // defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

typedef ODULong ODBlockSize;

//========================================================================================
// Constant definitions
//========================================================================================

#define kDefaultHeapID ((ODMemoryHeapID) 0L)


//========================================================================================
// Initialization        (MUST call this before using ODNewPtr[Clear] or ODGetDefaultHeap)
//========================================================================================

OSErr InitODMemory( );

//========================================================================================
// Operations on heaps
//========================================================================================

ODMemoryHeapID	ODGetDefaultHeap();
//ODMemoryHeapID	ODGetSystemHeap();

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
ODMemoryHeapID	ODCreateHeap(ODULong initialSize, ODULong growBy, Boolean fromSysMemory,
							const char *name);
void			ODDestroyHeap(ODMemoryHeapID heapID);
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

//========================================================================================
// Operations on pointer based blocks
//========================================================================================

// ----- Allocation and freeing

void*		ODNewPtr(ODBlockSize size, ODMemoryHeapID heapID  DEFAULT(kDefaultHeapID) );
void*		ODNewPtrClear(ODBlockSize size, ODMemoryHeapID heapID  DEFAULT(kDefaultHeapID) );
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
void*		ODReallocate(void *block, ODBlockSize newSize);
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
void 		ODDisposePtr(void *block);

// ----- Utilities

ODMemoryHeapID ODRecoverHeapID(const void *block);

//========================================================================================
// Operations on handle based blocks
//========================================================================================

// ----- Allocation and freeing

ODHandle	ODNewHandle(ODBlockSize howBig);
void		ODDisposeHandle(ODHandle handle);
ODHandle	ODCopyHandle(ODHandle handle);

// ----- Block size accessors

ODULong		ODGetHandleSize(ODHandle handle);
void		ODSetHandleSize(ODHandle handle, ODBlockSize size);

// ----- Locking and unlocking

void*		ODLockHandle(ODHandle handle);
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
void		ODUnlockPtr(void* ptr);
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
void		ODUnlockHandle(ODHandle handle);

//========================================================================================
// Utility functions
//========================================================================================

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
ODBoolean	ODHaveFreeSpace( ODSize haveTotal, ODSize haveContig DEFAULT(0),
							 ODBoolean appHeap DEFAULT(kODFalse) );
void		ODRequireFreeSpace( ODSize haveTotal, ODSize haveContig DEFAULT(0),
								ODBoolean appHeap DEFAULT(kODFalse) );
void		ODCheckAppHeapSpace();
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)


void		ODBlockMove(const void *from, void *to, ODBlockSize size);

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
void		ODBlockIsObject( void *block, ODBoolean isObject );	// Called by ODObject::somInit
ODBoolean	ODIsBlockAnObject( const void *block );
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

#ifdef _PLATFORM_MACINTOSH_
#ifndef __QUICKDRAW__
#include <QuickDraw.h>
#endif

RgnHandle	ODNewRgn( );									// Allocates rgn in temp-mem.

#ifdef __cplusplus
	inline RgnHandle ODCopyRgn( RgnHandle r )
			{return (RgnHandle)ODCopyHandle((ODHandle)r);}
#else
	#define ODCopyRgn(R)	((RgnHandle)ODCopyHandle((ODHandle)(R)))
#endif

#endif /*_PLATFORM_MACINTOSH_*/


#ifdef __cplusplus
}
#endif

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif

#endif /*_ODMEMORY_*/
