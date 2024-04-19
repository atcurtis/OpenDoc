/* @(#)Z 1.4 com/src/bento/SessHdr.h, odstorage, od96os2, odos29646d 96/11/15 15:26:36 (96/10/29 09:13:51) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES: none
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */

/*
	File:		SessHdr.h

	Contains:	Definition for session-related Bento Handlers.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>	10/24/95	jpa		1293441: DM: Bento memory reserve.
		 <3>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <2>	 6/15/94	RR		ODHeap -> ODMemoryHeap
		 <1>	 5/27/94	VL		first checked in

	To Do:
	In Progress:
		
*/


#ifndef _SESSHDR_
#define _SESSHDR_

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

//==============================================================================
// Classes used by this interface
//==============================================================================
class ODContainer;

//==============================================================================
// Scalar Types
//==============================================================================

typedef struct ODSessionRefCon ODSessionRefCon;
struct ODSessionRefCon {
	ODSessionRefCon() {}
	~ODSessionRefCon() {}
	ODContainer*	container;
	ODMemoryHeapID			heap;

	void*            cmAllocReserveBlock;
	CMSize           cmAllocReserveSize;
	
#ifdef ODDebugBentoSize
	ODULong          fMark;
	ODULong          fTotal;
	ODULong          fHighest;
	// ODULong          fLowest;
	ODULong          fHigh;
	ODULong          fLow;
	ODBoolean        fUp;
#endif

};

void ODSessionMustHaveCMAllocReserve(CMContainer cmContainer);
void ODSessionRestoreCMAllocReserve(CMContainer cmContainer);


//==============================================================================
// sessionRoutinesMetahandler
//==============================================================================

	CM_CFUNCTION
	
CMHandlerAddr CM_FIXEDARGS sessionRoutinesMetahandler(CMType targetType, CMconst_CMGlobalName operationType);

	// Metahandler proc for determining the addresses of the session handler operation routines.
	// Pass the address of this routine to a CMStartSession() call.

	CM_END_CFUNCTIONS


#endif	// _SESSHDR_
