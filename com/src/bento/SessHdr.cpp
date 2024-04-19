/* @(#)Z 1.9 com/src/bento/SessHdr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:36 (96/10/29 09:13:44) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
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
	File:		SessHdr.cpp

	Contains:	Implementation of sessionRoutinesMetahandler and its
						associated handlers.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	10/24/95	jpa		1293441: DM: Bento memory reserve.
		 <5>	10/20/95	VL		1293256: THROW kODBentoErr for all Bento
									errors.
		 <4>	 5/26/95	VL		1251403: Multithreading naming support.
		 <3>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <2>	 6/15/94	RR		ODHeap -> ODMemoryHeap
		 <1>	 5/27/94	VL		first checked in

	To Do:
	In Progress:
		
*/

//==============================================================================
// Theory of Operation
//==============================================================================

/*

	This file contains a fully documented example of the set of session handlers and its
	metahandler as it would be used by the Container Manager CMStartSession() routine.
 
	When you call CMStartSession() you pass a pointer to a metahandler that is used to get
	the addresses of the error reporting, memory allocator and memory deallocator routines.
	Possibly others might be defined in the future.
	
*/

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _MEMMGR_
#include "MemMgr.h"
#endif
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#pragma segment SessHdr

//==============================================================================
// Function Prototype
//==============================================================================

	CM_CFUNCTIONS
	
 CMHandlerAddr CM_FIXEDARGS sessionRoutinesMetahandler(CMType targetType, CMconst_CMGlobalName operationType);
																
#ifdef _PLATFORM_OS2_
 static void CM_VARARGS _System error_Handler(CMErrorNbr errorNumber, ...);
#else
 static void CM_VARARGS error_Handler(CMErrorNbr errorNumber, ...);
#endif
 static void CM_PTR * CM_FIXEDARGS alloc_Handler(CMSize size, CMRefCon sessionRefCon);
 static void CM_FIXEDARGS free_Handler(CMPtr ptr, CMRefCon sessionRefCon);
																 
	CM_END_CFUNCTIONS

//------------------------------------------------------------------------------
// sessionRoutinesMetahandler
//------------------------------------------------------------------------------
CMHandlerAddr CM_FIXEDARGS sessionRoutinesMetahandler( CMType targetType,
													   CMconst_CMGlobalName operationType )
{
	static char *operationTypes[] = {CMErrorOpType,					/*  0 */ /*  Operation Types	*/
									 CMAllocOpType,					/*  1 */
									 CMFreeOpType,					/*  2 */
									 NULL};
	char 	 **t;
	CMType ignored = targetType;
	
	/* Look up the operation type in the operationTypes table above...								*/
	
	t = operationTypes - 1;
	while (*++t) if (strcmp((char *)operationType, *t) == 0) break;

	/* Now that we got it (hopefully), return the appropriate routine address...					*/
	
	switch (t - operationTypes) {
		case  0:	return ((CMHandlerAddr)error_Handler);						/* CMErrorOpType 				*/
		case  1:	return ((CMHandlerAddr)alloc_Handler);						/* CMAllocOpType 				*/
		case  2:	return ((CMHandlerAddr)free_Handler);						/* CMFreeOpType 				*/
		
		default:	return (NULL);												/* huh?							*/
	}
}

//------------------------------------------------------------------------------
// error_Handler
//------------------------------------------------------------------------------
#ifdef _PLATFORM_OS2_
static void CM_VARARGS _System error_Handler(CMErrorNbr errorNumber, ...)
#else
static void CM_VARARGS error_Handler(CMErrorNbr errorNumber, ...)
#endif
{
	va_list inserts;
	char errorString[256];

	va_start(inserts, errorNumber);
	CMVGetErrorString(errorString, 256, errorNumber, inserts);
	va_end(inserts);

	THROW_MSG(kODErrBentoErr, errorString);
}

//------------------------------------------------------------------------------
// ODSession_Trace
//------------------------------------------------------------------------------

#ifdef ODDebugBentoSize
static void ODSession_Trace(ODSessionRefCon* session, long size)
{
	ODBoolean reverse = (session->fUp && size < 0) || (!session->fUp && size > 0);
	ODULong oldTotal = session->fTotal;
	
	if ( size < 0 ) // freeing
	{
		if (reverse) // previous total was a local high
			session->fHigh = session->fTotal;

		session->fUp = kODFalse;
		long amount = -size;
		if (session->fTotal < amount)
			session->fTotal = amount;
		else
			session->fTotal -= amount;
			
	}
	else //allocating
	{
		if (reverse) // previous total was a local low
			session->fLow = session->fTotal;

		session->fUp = kODTrue;
		session->fTotal += size;
	}
	
	if (session->fTotal > session->fHighest)
		session->fHighest = session->fTotal;
	
	long diff = ((long) session->fTotal - (long) session->fMark);
	ODBoolean mark = (diff > 2048 || diff < -2048);
	if (mark)
		session->fMark = session->fTotal;
		
	if (reverse || mark)
	{
		if ( session->fUp )
			somPrintf("¥alloc¥(%ld) [%lu] : %lu UP FROM %lu\n", 
				size,  session->fHighest, session->fTotal, session->fLow);
		else
			somPrintf("¥free¥(%ld) [%lu] : %lu down from %lu\n", 
				size,  session->fHighest, session->fTotal, session->fHigh);		
	}
	else
	{
		if ( session->fUp )
			somPrintf("/ %ld \\\n", size);
		else 
			somPrintf("\\ %ld /\n", size);
	}
}
#endif

//------------------------------------------------------------------------------
// alloc_Handler
//------------------------------------------------------------------------------

static void CM_PTR * CM_FIXEDARGS alloc_Handler(CMSize size, CMRefCon sessionRefCon)
{
	ODMemoryHeapID		heap = kDefaultHeapID;
	ODSessionRefCon* sessRc = (ODSessionRefCon*) sessionRefCon;
	
	if (sessionRefCon != kODNULL)
		heap = sessRc->heap;

#ifdef _PLATFORM_MACINTOSH_
	void* block = MMAllocate(size);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
	void* block = ODNewPtr(size, heap);
#endif
	if ( !block )
	{
		if ( sessRc->cmAllocReserveBlock )
		{
			void* reserve = sessRc->cmAllocReserveBlock;
			sessRc->cmAllocReserveBlock = kODNULL;
			
#ifdef _PLATFORM_MACINTOSH
			MMFree(reserve);
			block = MMAllocate(size);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
			ODDisposePtr(reserve);
			block = ODNewPtr(size, heap);
#endif
		}
	}
	if ( !block )
		THROW( kODErrOutOfMemory ); // should be kFatalError

#ifdef ODDebugBentoSize
	long blockSize = (long) MMBlockSize(block) + 8;
		
	ODSession_Trace(sessRc, (long) blockSize); 

	return block;
#else

	// return ODNewPtr(size, heap);
	return block;
#endif
		
}

//------------------------------------------------------------------------------
// free_Handler
//------------------------------------------------------------------------------


static void CM_FIXEDARGS free_Handler(CMPtr ptr, CMRefCon sessionRefCon)
{

#ifdef ODDebugBentoSize
	if ( ptr )
	{
		long blockSize = (long) MMBlockSize(ptr) + 8;
		ODSession_Trace((ODSessionRefCon*) sessionRefCon,  - blockSize ); 
	}
#else
ODUnused(sessionRefCon);
#endif

	ODDisposePtr(ptr);
}

//------------------------------------------------------------------------------
// reserve
//------------------------------------------------------------------------------


// void*            cmAllocReserveBlock;
// CMSize           cmAllocReserveSize;

void ODSessionMustHaveCMAllocReserve(CMContainer cmContainer)
{
	if (cmContainer)
	{
		ODSessionRefCon* src = (ODSessionRefCon*) CMGetSessionRefCon(cmContainer);
		if ( src && !src->cmAllocReserveBlock )
			src->cmAllocReserveBlock = ODNewPtr(src->cmAllocReserveSize, src->heap);
	}
}

void ODSessionRestoreCMAllocReserve(CMContainer cmContainer)
{
	if (cmContainer)
	{
		ODSessionRefCon* src = (ODSessionRefCon*) CMGetSessionRefCon(cmContainer);
		if ( src && !src->cmAllocReserveBlock )
#ifdef _PLATFORM_MACINTOSH_
			src->cmAllocReserveBlock = MMAllocate( src->cmAllocReserveSize );
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
			src->cmAllocReserveBlock = ODNewPtr(src->cmAllocReserveSize, kODNULL);
#endif
	}
}

