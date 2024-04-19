/* @(#)Z 1.12 os2/src/utils/ODMemory.cpp, odutils, od96os2, odos29646d 96/11/15 15:49:06 (96/08/23 01:44:18) */
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

/*
	File:		ODMemory.cpp

	Contains:	Procedural implementation to the Memory component

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/
/********************************
   ======================
   ODMemory Porting Notes
   ======================

   Since none of the functions in ODMemory.cpp are API's, I am removing all
   functions that are not used OpenDoc base code (i.e., not including test
   cases and test parts).  This is especially true for functionality that
   is Mac-specific.

   Apple's base OpenDoc ODMemory implementation provides the concept of
   heaps.  However, nowhere in OpenDoc is any heap but the default heap
   being used.  Therefore, in non-Apple implementations, any heap parameter
   is completely ignored and all allocation is from the "default" heap.
   As a result, the following functions have been completely removed:
   ODCreateHeap and ODDestroyHeap.  If, in a future Apple code base, these
   functions are required, they will be supported.

   ODReallocate has been completely removed.  On the Mac platform, there exists
   a memory API which returns the size of a given block.  No such interface
   exists on other platforms, short of rolling your own suballocation facility.
   This interface is necessary to support ODReallocate because, in order to
   determine if a block must be enlarged, the current size must be known.  It
   turns out that there are no references to ODReallocate in the DR4 base code.
   Therefore, the removal of ODReallocate does not impact any other code.

   ODCheckAppHeapSpace has been completely removed.  For some reason, in Apple's
   OpenDoc implementation, the approach used to allocate memory is to query the
   memory subsystem on available memory before attempting to allocate.  This
   contrasts with the usual approach of requesting memory and handling failure of
   the allocation request.  Information about available memory is typically not
   available on non-Mac platforms.  Moreover, the ODCheckAppHeapSpace approach
   would not work on multi-threaded systems, since the status can change during
   the window between the query and the actual allocation call.  ODRequireFreeSpace
   and ODHaveFreeSpace have been entirely #ifdef'ed out for the same reasons.

   ODIsBlockAnObject and ODBlockIsObject have been completely removed.  They use
   Mac-specific code not supported on other systems, and the function is not being
   used anywhere in the OpenDoc code.  The few calls to ODBlockIsObject have been
   #ifdef'ed out, since ODIsBlockAnObject is not used.

   Macintosh memory management provides two basic types of blocks. One is
   guaranteed to not move while allocated, which is what one normally
   expects from blocks.  For this type of block, the memory manager "allocate"
   function returns a pointer to a block.  However, there is a type of block which
   can be moved around by the memory manager for the purposes of compaction.  This
   type of block results in more efficient use of memory, but is slower, since
   the pointer returned from an allocation request is a "handle", i.e., a pointer to
   the current address of the block.  Thus an extra level of indirection is induced.
   The user can "lock" a block to its current address, but is only supposed to do so
   for very short intervals so as not to defeat the purpose of relocatable blocks.
   This "feature" is reflected in the ODMemory "Handle" functions.  On non-Mac
   platforms, a block returned from an ODNewHandle request is the same as an ODNewPtr
   request, except that an ODHandle is returned.  As on the Mac, an extra level of
   indirection is induced and is not recommended, especially since there is no memory
   compaction advantage.  Since blocks pointed to by ODHandle's are stationary, no
   locking or unlocking is necessary.  Since there are several calls to ODLockHandle
   and ODUnlockHandle, the functions still exist but are no-ops.  ODUnlockPtr is not
   referenced at all and has been completely removed.

********************************/

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif
#if defined(_PLATFORM_WIN32_)
#ifndef __ODPAGTUN__
#include <ODPagTun.h>
#endif
#endif // defined(_PLATFORM_WIN32_)

//========================================================================================
// Local types
//========================================================================================
// OpenDoc memory mgmt is basically a mirror of Mac's memory mgmt.  An
// ODHandle is really a pointer to a "Master Pointer", which in turn
// points to a block which may be relocated by the system.  On non-Mac
// platforms, blocks won't move around, but the structures still need
// to be in place for existing ODHandle users.  Users tend to dereference the
// ODHandle to obtain the block pointer.  The "size" field is not expected
// by ODHandle users, but is used to support certain memory mgmt. interfaces.
struct MasterPointer {
   char *ptr;           // Ptr to "relocatable" block.  Must be first in structure.
   unsigned long size;  // Size of block
};


//========================================================================================
// Constants
//========================================================================================


//========================================================================================
// Global variables
//========================================================================================


//========================================================================================
// Initialization
//========================================================================================


OSErr InitODMemory( )
{
	return 0;
}

//========================================================================================
// Function declarations for operations on pointer based blocks
//========================================================================================


//----------------------------------------------------------------------------------------
// ODGetDefaultHeap
//----------------------------------------------------------------------------------------

ODMemoryHeapID ODGetDefaultHeap()
{
	return 0;
}

//----------------------------------------------------------------------------------------
// ODNewPtr
//----------------------------------------------------------------------------------------

void *ODNewPtr(ODBlockSize blkSize, ODMemoryHeapID heapID)
{
	return new char [blkSize];
}

//----------------------------------------------------------------------------------------
// ODNewPtrClear
//----------------------------------------------------------------------------------------

void *ODNewPtrClear(ODBlockSize blkSize, ODMemoryHeapID heapID)
{

	void *block = new char [blkSize];
	memset (block, '\0', blkSize);
	return block;
}

//------------------------------------------------------------------------------
// ODDisposePtr
//------------------------------------------------------------------------------

void ODDisposePtr(void *pointer)
{
	delete[] pointer;
}

//----------------------------------------------------------------------------------------
// ODRecoverHeapID
//----------------------------------------------------------------------------------------

ODMemoryHeapID ODRecoverHeapID(const void *block)
{
	return 0;
}


//========================================================================================
// Function declarations for operations on handle based blocks
//========================================================================================

//----------------------------------------------------------------------------------------
// ODNewHandle
//----------------------------------------------------------------------------------------

ODHandle ODNewHandle(ODULong howBig)
{
	// Some callers specify a block size of zero and use the resulting ODHandle as a
	// key (after converting it to an ODByteArray).  Pass back a dummy block.  No
	// memory leak should result, because the protocol requires a call to ODDisposeHandle--
	// even with "zero-length" blocks.
	if (howBig == 0)
		howBig = 4;

	MasterPointer *h;
	h = new MasterPointer;
	if (!h)
		THROW(kODErrOutOfMemory);
	
	h->ptr = (char *)new char [howBig];
	if (!h->ptr)
	{
		delete h;
		THROW(kODErrOutOfMemory);
	}
	h->size = howBig;

	return (ODHandle)h;
}

//----------------------------------------------------------------------------------------
// ODDisposeHandle
//----------------------------------------------------------------------------------------

void ODDisposeHandle(ODHandle handle)
{
	if (!handle) return;
	delete[] ((MasterPointer *)handle)->ptr;
	delete (MasterPointer *) handle;
}

//----------------------------------------------------------------------------------------
// ODCopyHandle
//----------------------------------------------------------------------------------------

ODHandle ODCopyHandle(ODHandle handle)
{
	ODHandle newHandle;

	if (!handle) return kODNULL;
	
	if (((MasterPointer *)handle)->size != 0)
		newHandle = ODNewHandle(((MasterPointer *)handle)->size);
	else
	{
		MasterPointer *h;
		h = new MasterPointer;
		if (!h)
			THROW(kODErrOutOfMemory);
		h->size = 0;
		h->ptr = NULL;
		newHandle = (ODHandle)h;
	}
	memcpy(((MasterPointer *)newHandle)->ptr, 
	       ((MasterPointer *)handle)->ptr,
	       ((MasterPointer *)handle)->size);

	return newHandle;
}

//----------------------------------------------------------------------------------------
// ODGetHandleSize(ODHandle handle)
//----------------------------------------------------------------------------------------

ODULong ODGetHandleSize(ODHandle handle)
{
	if (!handle) return 0;
	return ((MasterPointer *)handle)->size;
}

//----------------------------------------------------------------------------------------
// ODSetHandleSize(ODHandle handle, ODULong blkSize)
//----------------------------------------------------------------------------------------

void ODSetHandleSize(ODHandle handle, ODULong blkSize)
{
	char *newPtr;

	if (!handle)
		return;

	char *oldPtr = ((MasterPointer *)handle)->ptr;
	if (blkSize)
	{
		newPtr = (char *)ODNewPtr(blkSize, 0);
		if (!newPtr)
			THROW(kODErrOutOfMemory);
	}
	else
		newPtr = NULL;

	unsigned long cpyLen = ((MasterPointer *)handle)->size;
	if (cpyLen > blkSize)
		cpyLen = blkSize;

	memcpy(newPtr, oldPtr, cpyLen);
	ODDisposePtr(oldPtr);

	((MasterPointer *)handle)->ptr = newPtr;
	((MasterPointer *)handle)->size = blkSize;
}

//----------------------------------------------------------------------------------------
// ODLockHandle(ODHandle handle)
//----------------------------------------------------------------------------------------

void* ODLockHandle(ODHandle handle)
{
	// Note:  On non-Mac systems, locking is not necessary, since blocks don't move
	//        around.  However, the block pointer needs to be returned.
	//        See notes following file header.
	return ((MasterPointer *)handle)->ptr;
}

//----------------------------------------------------------------------------------------
// ODUnlockHandle(ODHandle handle)
//----------------------------------------------------------------------------------------

void ODUnlockHandle(ODHandle handle)
{
	// Not applicable on non-Mac systems.  See notes following file header.
}


//========================================================================================
// Function declarations utility functions
//========================================================================================

//------------------------------------------------------------------------------
// ODBlockMove
//------------------------------------------------------------------------------


void ODBlockMove( const void *from, void *to, ODULong size)
{
	memcpy (to, from, size);
}
