/* @(#)Z 1.4 com/src/cm/FreeSpce.c, odstorage, od96os2, odos29646d 96/11/15 15:27:05 (96/10/29 09:18:19) */
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
	File:		FreeSpce.c 

	Contains:	Container Manager Free Space Value Management Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 9/26/95	EL		1286040: Make sure space from global names
									do not go to free space if it is not on
									disk.
		 <5>	 8/16/95	EL		1238951: Bento file has wrong deleted-space
													value.
		 <4>	 12/9/94	EL		#1182275 Optionally do not maintain
													continue flag. Keep all free space, no
													matter how small and delete small segment
													only on closing.
		 <3>	 9/30/94	EL		#1182275 rename mergeCandidate to
													updateMergeCandidate.
		 <2>	 8/26/94	EL		#1182275 Handling of temporary free list.
													Correct value in spaceDeletedValue in
													merging. Avoid giving out highly fragmented
													blocks.
		 <4>	 5/10/94	EL		#1162327. ValueDoNotFree is no longer
													needed.
		 <3>	  4/6/94	EL		Do not free free space property when it is
													taken over. #1150214
		 <2>	 3/31/94	EL		Check to see if new free space is just
													behind another free segment. #1150214
		 <1>	  2/3/94	EL		first checked in
		 <5>	 1/19/94	EL		Free space just released will not be used
													immediately, but will be keep in a
													temporary list that is merge to the
													permanent free list at close time.
		 <4>	  1/6/94	EL		Add cmTakeOverFreeList call to transfer the
													free list.
		 <3>	11/16/93	EL		Correct the value of spaceDeletedValue.
		 <2>	 11/4/93	EL		Do not put deleted space in target
													container into the free list. Arrange data
													in free list in ascending order so that it
													can be merged into larger blocks.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<< FreeSpce.c  >>>                            |
 |                                                                           |
 |          Container Manager Free Space Value Management Routines           |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 4/23/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1994               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file contains routines for reusing deleted data space.  The routines record the
 deleted space and reuse it for value data writes.
 
 The free space is maintained as a list of value segment entries for a "free space"
 property belonging to the TOC ID 1 entry.  It is written to the container like other ID 1
 properties to keep track of all the deleted space.
 
 If a container is opened for reusing free space, we use the free list to reuse the space.
*/


#include <stddef.h>
#include <stdio.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API__
#include "CMAPI.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __TOCIO__
#include "TOCIO.h"
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __FREESPACE__
#include "FreeSpce.h" 
#endif
																	
																	
																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment TOCEntries
#endif

/*----------------------------------------------------------------*
 | removeEmptyFreeSpace - remove free space header if it is empty |
 *----------------------------------------------------------------*
 
 Check to see if free space is empty. If it is, completely remove that property.
*/
 
static void CM_NEAR removeEmptyFreeSpace(ContainerPtr container)
{
	TOCObjectPtr	 theTOCObject;
	TOCPropertyPtr theFreeListProperty;
	/* If there are no more free list entries, delete the value header and the "free 			*/
	/* space" property from TOC ID 1...																										*/
		
	TOCValueHdrPtr freeSpaceValueHdr = container->freeSpaceValueHdr;
	if (freeSpaceValueHdr) {
		if (cmIsEmptyList(&freeSpaceValueHdr->valueList)) {			/* if no more free list...	*/
			theFreeListProperty = freeSpaceValueHdr->theProperty;	/* ...get owning property		*/
			theTOCObject 				= theFreeListProperty->theObject;	/* ...get owning object (1)	*/
			CMfree(freeSpaceValueHdr);														/* ...clobber the value hdr	*/
			CMfree(cmDeleteListCell(&theTOCObject->propertyList, theFreeListProperty));
			container->freeSpaceValueHdr = NULL;									/* ...no more free list			*/
		}
#if CMKEEP_CONTINUE_FLAG
		else if (cmCountListCells(&freeSpaceValueHdr->valueList) == 1)
			freeSpaceValueHdr->valueFlags &= ~ValueContinued;			/* only 1, then no cont			*/
#endif
	}
}

/*------------------------------------------------*
 | deleteFreeListEntry - delete a free list entry |
 *------------------------------------------------*
 
 This routine removes the specified free list entry and frees up its space.  If this is 
 the last entry of the free list, the free list property itself (along with its value
 header, of course) are removed.
 
 Note, this routine is a low-level routine called from the other routines in this file.
 As such there are no error checks.  The caller is assumed "happy" with what its doing by
 the time it calls this routine.
*/

static void CM_NEAR deleteFreeListEntry(TOCValuePtr theValue)
{
	ContainerPtr 	 container = theValue->theValueHdr->container;
	TOCValueHdrPtr freeSpaceValueHdr = container->freeSpaceValueHdr;
#if CMKEEP_CONTINUE_FLAG
	TOCValuePtr		 prevValue;
	
	prevValue = (TOCValuePtr) cmGetPrevListCell(theValue);
	if (prevValue) {
		prevValue->flags = theValue->flags;				/* if theValue is last, now you are last 	*/
	}
#endif

	/* Delete the value entry from its list and free its space...													*/
	
	freeSpaceValueHdr->size -= theValue->value.notImm.valueLen;
	CMfree(cmDeleteListCell(&freeSpaceValueHdr->valueList, theValue));
	
	removeEmptyFreeSpace(container);						/* remove the property if it is empty			*/
}

/*----------------------------------------------------------------------------------*
 | rearrangePermFreeList - arrange items in free list in ascending order and merged |
 *----------------------------------------------------------------------------------*

	!.0d5 or earlier code writes a free list that is not in sorted order.

	This version expects free list item to be in sorted order.
	
	We do this by moving all the items in the perm free list into the temp free list.
	Then we move it back from the temp free list to the perm free list.
	Since the 2nd operation always ensure list items are in ascending order and
	adjacent items would be combined into one. We can back the list we want.
	
	However we do not want to move items originally in the temp free list to the perm
	free list. So we save the temp free list in the beginning and restore the temp free 
	list at the end.
*/
 
static void CM_NEAR rearrangePermFreeList(ContainerPtr container)
{
	ListHdr 				saveList = container->tmpList;
	TOCValuePtr		 	theValue;
	CM_ULONG				size;
	
	cmInitList(&container->tmpList);	
	theValue = (TOCValuePtr)cmGetListHead(&container->freeSpaceValueHdr->valueList);
	
	while (theValue) {															/* scan the free space list...				*/
		size = theValue->value.notImm.valueLen;
		container->spaceDeletedValue->value.imm.ulongValue -= size;
		cmAddToTmpFreeList(container, theValue->value.imm.ulongValue, size);
		theValue = (TOCValuePtr)cmGetNextListCell(theValue);
	}	
	container->freeSpaceValueHdr->size = 0;										/* we removed everything		*/
	cmFreeAllListCells(&container->freeSpaceValueHdr->valueList, SESSION);
	cmCopyTmpFreeToTOC(container);			/* move from temp tmp list back to perm free list	*/
	
	container->tmpList = saveList;
}

/*------------------------------------------------------------------------*
 | addToPermFreeList - add freed space to the permanent "free space" list |
 *------------------------------------------------------------------------*

	This routine takes some free space and put it into the permanent free space property
	for use in future.

  As part of the freeing process, we scan the free list to see if the new space can be 
	combined with an already existing entry.  It's a sort of on-the-fly garbage collector.
	
	For new containers, there is no "free space" property initially for ID 1.  It is created
	here the first time we need to free space.  We remember the value header for the "free
	space" property so that we may efficiently get at the list on all future calls.
*/
 
static void CM_NEAR addToPermFreeList(ContainerPtr container,
										 	 CM_ULONG offset, CM_ULONG size)
{
	TOCObjectPtr 	 theTOCobject;
	TOCValueHdrPtr freeSpaceValueHdr = container->freeSpaceValueHdr;
	TOCValuePtr		 theValue = NULL, nextValue;
	CM_ULONG	 		 valueStart, valueEndPlus1, prevStart;
	TOCValueBytes  valueBytes;
	void					 *toc;
	
		
	/* See if the space we're freeing can be combined with some already existing free			*/
	/* space.  If the new space is already freed (now how could that happen?) we ignore		*/
	/* the new space.  If the chunk of space we are about to free is less than the size		*/
	/* of a TOC entry, we ignore it (losing track of that space), since it would cost more*/
	/* to remember it.  However, if that space can be combined with already existing 			*/
	/* space, we do the combining.																												*/ 
	
	if (freeSpaceValueHdr != NULL) {								/* if free space list exists...				*/
		theValue = (TOCValuePtr)cmGetListHead(&freeSpaceValueHdr->valueList);
		prevStart = 0;
		
		while (theValue) {														/* scan the free space list...				*/
			valueStart 		= theValue->value.imm.ulongValue;
			valueEndPlus1 = valueStart + theValue->value.notImm.valueLen;
			if (prevStart > valueStart) {
				/* the list is not in order, must have been written by 1.0d5 or earlier software*/
				rearrangePermFreeList(container);							/* make sure it is in order first	*/
				addToPermFreeList(container, offset, size);		/* now do it again								*/
				return;
			}
			prevStart = valueStart;
			if (offset >= valueStart) { 													/* this is after this				*/
				if (offset <= valueEndPlus1) { 											/* have overlap or concat		*/
					if (offset + size > valueEndPlus1) {			/* combine new amount with this entry*/
						size = (offset + size) - valueEndPlus1;	/* reduce actual size to add in				*/
						theValue->value.notImm.valueLen += size;/* this is what we combine in					*/
						freeSpaceValueHdr->size += size;				/* echo total size in header					*/
						container->spaceDeletedValue->value.imm.ulongValue += size;
						/* we may able to join with the next value to form a single value						*/
						valueEndPlus1 += size;
						nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
						if (nextValue) 
							if (nextValue->value.imm.ulongValue <= valueEndPlus1) {
								/* we can combine with next value, absorb the next value in this one		*/
								theValue->value.notImm.valueLen = nextValue->value.imm.ulongValue +
									nextValue->value.notImm.valueLen - theValue->value.imm.ulongValue;
								/* we bump up the free space to cancel out deleteFreeListEntry					*/
								freeSpaceValueHdr->size += nextValue->value.imm.ulongValue;
								/* now everything in next value in in this one, we can delete next value*/
								deleteFreeListEntry(nextValue);
							}
					}
					return;																		/* exit since we updated "old" entry*/
				}
			} else if (offset + size >= valueStart) {			/* can combine from the lower end		*/
				theValue->value.imm.ulongValue = offset;		/* this is the low end							*/
				if (offset + size > valueEndPlus1)
					valueEndPlus1 = offset + size;
				size = valueEndPlus1 - offset - theValue->value.notImm.valueLen; /* increment */
				theValue->value.notImm.valueLen += size;/* this is what we combine in					*/
				freeSpaceValueHdr->size += size;				/* echo total size in header					*/
				container->spaceDeletedValue->value.imm.ulongValue += size;
				return;																		/* exit since we updated "old" entry	*/
			} else
				break;																			/* it should be inserted before here*/
			theValue = (TOCValuePtr)cmGetNextListCell(theValue);
		}
	} /* checking for combining */
	
#if 0	
	/* Since the cost of recording the free space in the container will be a TOC entry, we*/
	/* only record free space chucks whose size is LARGER than a TOC entry.  Since we 		*/
	/* couldn't combine it with already existing space, we simply exit and forget it.			*/

	#if TOC1_SUPPORT
	if (container->majorVersion == 1) {
		if (size <= TOCentrySize) return;
	} else 
		if (size <= MinTOCSize) return;
	#else
	if (size <= MinTOCSize) return;
	#endif
#endif
	/* If this is the first freed space in the container, create the "free space" property*/
	/* for the TOC object 1.  Otherwise, just use it. We remember the pointer to the value*/
	/* header for this property in the container.  Note, the TOC we want to deal with here*/
	/* is the container's own (private) TOC.  This will be different from the current TOC	*/
	/* when we're updating containers.																										*/
	
	if (freeSpaceValueHdr == NULL) {								/* if 1st free space for container...	*/
		toc = container->toc;													/* ...remember current TOC						*/
		container->toc = container->privateTOC;				/* use container's own (private) TOC	*/
		
		theTOCobject = cmDefineObject(container,			/* ...create "free space" property		*/
									 								CM_StdObjID_TOC, CM_StdObjID_TOC_Free,
																	CM_StdObjID_TOC_Type, NULL,
									 								container->generation, 0,
																	(ObjectObject | ProtectedObject),
									 								&freeSpaceValueHdr);
		
		container->toc = toc;													/* restore current container					*/
		if (theTOCobject == NULL) return;
		freeSpaceValueHdr->valueFlags |= ValueProtected;/* don't allow writing to this value*/
		container->freeSpaceValueHdr = freeSpaceValueHdr;
	} 

	/* At this point we want to create a new free list entry for the newly freed offset 	*/
	/* size.  The value list are standard value entries for the "free space" property of	*/
	/* TOC object ID 1.																																		*/

	container->spaceDeletedValue->value.imm.ulongValue += size;

	(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, offset, size);
	
#if CMKEEP_CONTINUE_FLAG
	if (theValue) {																			/* it should come before this			*/
		/* create a new value, and insert before theValue																		*/
		/* if we are successful, then update the value header to reflect it									*/
		if (cmInsertBeforeListCell(&freeSpaceValueHdr->valueList, 
															 cmCreateValueSegment(freeSpaceValueHdr, &valueBytes, kCMContinued), 
															 theValue)) {
			freeSpaceValueHdr->valueFlags |= ValueContinued;/* also set a more convenient flag	*/
			freeSpaceValueHdr->size += size;								/* echo total size in header					*/
		}
	} else {																						/* we should put it at very end		*/
		theValue = (TOCValuePtr)cmGetListTail(&freeSpaceValueHdr->valueList);
		if (theValue) {
			theValue->flags |= kCMContinued;								/* flag the current value as cont'd	*/
			freeSpaceValueHdr->valueFlags |= ValueContinued;/* also set a more convenient flag	*/
		}
		cmAppendValue(freeSpaceValueHdr, &valueBytes, 0);
	}
#else
	if (theValue) {																			/* it should come before this			*/
		/* create a new value, and insert before theValue																		*/
		/* if we are successful, then update the value header to reflect it									*/
		if (cmInsertBeforeListCell(&freeSpaceValueHdr->valueList, 
															 cmCreateValueSegment(freeSpaceValueHdr, &valueBytes, 0), 
															 theValue)) {
			freeSpaceValueHdr->size += size;								/* echo total size in header					*/
		}
	} else {																						/* we should put it at very end		*/
		theValue = (TOCValuePtr)cmGetListTail(&freeSpaceValueHdr->valueList);
		cmAppendValue(freeSpaceValueHdr, &valueBytes, 0);
	}
#endif
}

/*-----------------------------------------------------------------------*
 | cmAddToTmpFreeList - add freed space to the temporary "free space" list |
 *-----------------------------------------------------------------------*

	This routine takes some free space and put it into the temporary free space property
	for use in future.
	
	We want to put thing in the temporary free list for two reasons.
	
	For container opened for writing, we don't want to write into the old container
	before we close it in case it crashes before it is closed, so we put it into
	the temporary free list.
	
	For container opened for updating, we have free space that can be reused if
	it is merged. Since we do not know whether we will just close the container
	or merge it, we put it in the temporary free list and put it on the real
	free list only if we do a merge at close time.

*/
 
void cmAddToTmpFreeList(ContainerPtr container, CM_ULONG offset, CM_ULONG size)
{
  FreeSpaceItem	*toBeFree;

	toBeFree = CMmalloc(sizeof(FreeSpaceItem));			/* so we can get rid of SCpp warning*/
	if (toBeFree) {
		toBeFree->offset = offset;
		toBeFree->size = size;
		cmAppendListCell(&container->tmpList, toBeFree);
	}
}

/*-----------------------------------------------------------------------------------*
 | cmAddValueToTmpFreeList - add space in a value to the temporary "free space" list |
 *-----------------------------------------------------------------------------------*

	This routine put all the free space used by a value into the temporary free list

*/
 
void cmAddValueToTmpFreeList(TOCValueHdrPtr theValueHdr)
{
	TOCValuePtr theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);

	while (theValue) {
		/* use targetContainer->updatingContainer because during applyUpdate						*/
		/* updatingContainer point to itself, so we need to get the real updating cntr	*/
		cmAddToTmpFreeList(theValueHdr->container->targetContainer->updatingContainer,
											 theValue->value.notImm.value,
											 theValue->value.notImm.valueLen);
		theValue = (TOCValuePtr)cmGetNextListCell(theValue);
	}
}

/*----------------------------------------------------------------------*
 | cmAddToFreeList - add freed space to the permanent "free space" list |
 *----------------------------------------------------------------------*

 This routine is called whenever space for value data is to be freed.  The space is
 recorded in a free list. The free list entries are maintained as value segments belonging
 to the "free space" property of TOC object ID 1.
 
 The amount of space to be freed is specified in one of two possible ways:
 
 		1. By passing a pointer to a value segment in theValueToFree.  The offset and size
			 are extracted from the segment info.  If theValueToFree is for an immediate value,
			 we simply exit since there is no container space to actually free.  Global names,
			 as usual, are handled specially.  In particular, for no global names that have not
			 yet been written to the container yet (we keep 'em in memory unto we write the TOC
			 at container close time), then we ingore them just like immediates.  For "old"
			 global names we do account for the space.
			 
		2. If theValueToFree is passed as NULL, then an explicit size and offset may be
			 passed.  Note that a non-null theValueToFree has precedence over explicit offset
			 and size.
*/
 
void cmAddToFreeList(ContainerPtr container, TOCValuePtr theValueToFree,
		   							 CM_ULONG offset, CM_ULONG size)
{
	Boolean		mayGoToTmpList = false;

	/* We only track free space when a container is opened for writing or updating.  			*/
	/* However, even when writing there is an override switch to suppress the space				*/
	/* tracking.  This is done during applying updates during open time.  We don't want		*/
	/* to track what's going on then!																											*/
	
	if (!container->trackFreeSpace ||								/* if no tracking of free space or...	*/
		  (container->useFlags & kCMWriting) == 0)		/* ...not opened for writing/updating	*/
		return;																				/* ...exit														*/
	
	/* Get the offset and size from the value if it's specified. This is the amount being	*/
	/* freed.  If theValueToFree is NULL, then the offset and size must be explicitly 		*/
	/* specified, otherwise these parameters are ignored.  As a safety, if the value is		*/
	/* passed, but it is for an immediate value, there is no space being given up, so we	*/
	/* just exit.																																					*/
	
	if (theValueToFree != NULL) {										/* if value explicitly specified...		*/
		if ((theValueToFree->container->depth == 1) && (container->useFlags & kCMMerging))
			mayGoToTmpList = true;
		else if (theValueToFree->container != container)/* does it belong to this level?		*/
			return;																				/* no, then just return							*/
		if (theValueToFree->flags & kCMGlobalName) {	/* ...handle global names specially		*/
			if (theValueToFree->theValueHdr->valueRefCon == 0) 
				return; 																	/* not written, nothing to free				*/
			offset = theValueToFree->value.globalName.offset;	
			if (offset == 0) return; 										/* this is an extra precaution check */
			size = GetGlobalNameLength(theValueToFree->value.globalName.globalNameSymbol) + 1;
		} else if (theValueToFree->flags & kCMImmediate)
			return;																			/* ...ignore immediates...						*/
		else {																				/* ...have "normal" value							*/
			offset = theValueToFree->value.notImm.value;
			size 	 = theValueToFree->value.notImm.valueLen;
		}
	}
	
	if (size == 0) return;
	
	if (mayGoToTmpList || (container->useFlags & (kCMUpdateByAppend | kCMUpdateTarget) == 0)) {
		/* If the space is from previous container, or in the same container but we are not */
		/* doing an updating, do not reuse freespace until the container is closed.					*/
		/* So even if we crashes before the container is closed, old data is not destroyed	*/
		cmAddToTmpFreeList(container, offset, size);
	}
	else {	
		/* for updating, we do not touch the data in old container directly, so any free  	*/
		/* space comes from the new container and we can put it into the free list directly */
		addToPermFreeList(container, offset, size);
	}
}

/*------------------------------------------------------*
 | tmpListCB - add freed space to the "free space" list |
 *------------------------------------------------------*

 Action routine to take a free space and add it to the permanent free space list.
 */
 
static void CM_NEAR tmpListCB(void * theCell, CMRefCon refCon)
{
    FreeSpaceItem	*toBeFree = (FreeSpaceItem *)theCell;

    addToPermFreeList((ContainerPtr)refCon, toBeFree->offset, toBeFree->size);
}

/*---------------------------------------------------------------*
 | cmCopyTmpFreeToTOC - add freed space to the "free space" list |
 *---------------------------------------------------------------*

	This routine goes through the list of temporary free space and merge each one into
	the permanent free space. At the end the temporary free space are removed.
*/

void cmCopyTmpFreeToTOC(ContainerPtr container)
{
	cmForEachListCell(&container->tmpList, (CMRefCon)container, tmpListCB);

  cmFreeAllListCells(&container->tmpList, container->sessionData);
}

/*-----------------------------------------------------------------------*
 | cmRemoveSmallFreeSpace - remove segment too small to be worth keeping |
 *-----------------------------------------------------------------------*
 
 There is certain overhead of storing a value segment. If a free space segment is so small
 that the overhead of storing it is greater than the free space itself, then it is not
 worth to keep the free space. So we go through the free space list and remove the small
 segments.
 
 On the other hand, later we may free space that are are around these small segments.
 So if we ignore these small segments, we may cause fragmentation of the free space
 later. So we should revisit this issue to see if it is worthwhile to keep them anyway.
*/
 
void cmRemoveSmallFreeSpace(ContainerPtr container)
{
	TOCValuePtr		 theValue, nextValue;
	
	TOCValueHdrPtr freeSpaceValueHdr = container->freeSpaceValueHdr;
	if (freeSpaceValueHdr) {
		theValue = (TOCValuePtr)cmGetListHead(&freeSpaceValueHdr->valueList);
		while (theValue) {
			nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
			/* If the free space is smaller than overhead of keeping track of it, remove it 			*/
			if (theValue->value.notImm.valueLen < MinTOCSize) {
				freeSpaceValueHdr->size -= theValue->value.notImm.valueLen;
				CMfree(cmDeleteListCell(&freeSpaceValueHdr->valueList, theValue));
			}
			theValue = nextValue;
		} /* while value */
		
		removeEmptyFreeSpace(container);
	}
}


/*--------------------------------------------------------------*
 | cmGetFreeListEntry - get a free space entry from "free list" |
 *--------------------------------------------------------------*
 
 This routine returns one free list entry (if one exists).  The offset from that entry is
 returned as the function result.  The size is returned in actualSize.  If there is no 
 free space, or we can't find one big enough (see mustAllFit below), 0 is returned as the
 function result and for the actualSize.  0's are also returned if the container was not
 opened to reuse free space.
 
 The desiredSize is passed as what the caller would like as the single free list entry
 amount, i.e., a "best fit".  It is also used when we find a bigger free list entry and
 only have to reduce part of it.
 
 If mustAllFit is true, then we MUST find a single free list segment big enough or 0 will
 be returned.  This is used for data that is not allowed to be continued with multiple
 value segments (e.g., global names).
 
 The free list is built by cmAddToFreeList() as value segments belonging to a value header
 of the "free space" property for TOC object ID 1.   If all the free list entries are
 exhausted, the value header is freed along with the property.  If space is ever given up
 after this through cmAddToFreeList(), it will recreate the "free space" property, its
 value header, and the free list value entry segments.
*/

CM_ULONG cmGetFreeListEntry(ContainerPtr container, 
														CM_ULONG desiredSize, Boolean mustAllFit,
														CM_ULONG *actualSize)
{
	TOCValueHdrPtr freeSpaceValueHdr = container->freeSpaceValueHdr;
	TOCValuePtr		 theValue;
	CM_ULONG	 		 offset;
#if MinFragmentSize
	CM_ULONG	 		 minSegmentSize;
#endif

	/* Return null amount if there is no free list or not updating...											*/
	
	if (freeSpaceValueHdr == NULL || (container->useFlags & kCMReuseFreeSpace) == 0)
		return (*actualSize = 0);
	
	/* Use the next available free list entry (segment) no matter what it is unless we 		*/
	/* mustAllFit.  For mustAllFit we must scann the free list for a single entry large		*/
	/* enough to hold the entire data (desiredSize bytes).																*/
	
	/* Note as just said, when mustAllFit is false, we just use the first free list 			*/
	/* segment on the list.  A better algorithm might be to use the desired amount and 		*/
	/* find a "best fit" by scanning the free list just as in the mustAllFit case.  So 		*/
	/* which is better in time and/or space?  A scan each time or just reusing each entry */
	/* unconditionally.  The latter guarantees reuse of all available space.  But the 		*/
	/* former potentially uses less value segments.  For now I take the easy way.					*/
	
	/* Note we still need it to split larger free space entries when we don't need all		*/
	/* the space from an entry.																														*/
	
	theValue = (TOCValuePtr)cmGetListHead(&freeSpaceValueHdr->valueList);
	
#if MinFragmentSize
	if ((mustAllFit) || (desiredSize <= MinFragmentSize))
		/* If the block is small, it does not make sense to have different segments. So we	*/
		/* treat it as a single segment to avoid fragmentation.															*/
		minSegmentSize = desiredSize;
	else {
		/* Even if it is a large block, we still don't want it to fragment too much.				*/
		/* This is especially true since the block may be an embedded container. So we try	*/
		/* to limit the number of segments																									*/
#if MaxFragmentCount
		minSegmentSize = desiredSize / MaxFragmentCount;
		if (minSegmentSize < MinFragmentSize)
			minSegmentSize = MinFragmentSize;
#else
		minSegmentSize = MinFragmentSize;
#endif
	}
	
	while (theValue) {																				/* ...scan free list				*/
		if (theValue->value.notImm.valueLen >= minSegmentSize) 	/* ...for one big enough		*/
			break;
		theValue = (TOCValuePtr)cmGetNextListCell(theValue);
	}
	if (theValue == NULL) return (*actualSize = 0);					/* if none found, too bad		*/
#else
	if (mustAllFit) {																					/* if must find single seg	*/
		while (theValue) {																			/* ...scan free list				*/
			if (theValue->value.notImm.valueLen >= desiredSize) 	/* ...for one big enough		*/
				break;
			theValue = (TOCValuePtr)cmGetNextListCell(theValue);
		}
		if (theValue == NULL) return (*actualSize = 0);					/* if none found, too bad		*/
	}
#endif
	
	offset 			= theValue->value.notImm.value;								/* set to return this offset*/
	*actualSize = theValue->value.notImm.valueLen;						/* and this size (maybe!)		*/
	
	/* If the free list entry is big enough to cover all that is desired, use only the		*/
	/* required amount and retain the free list entry.  It must, however, be reduced by		*/
	/* the amount used.																																		*/
	
	/* UNRESOLVED QUESTION! Since we never allow free list entries to be created when the */
	/* space is less than a TOC entry size, should we keep the split space here when it		*/
	/* becomes less than a TOC entry size?  What does it cost?														*/
	
	if (desiredSize < *actualSize) {													/* free entry is too big		*/
		theValue->value.notImm.value    += desiredSize;					/* ...cut it down						*/
		theValue->value.notImm.valueLen -= desiredSize;
		container->spaceDeletedValue->value.imm.ulongValue -= desiredSize; /* cut total			*/
		freeSpaceValueHdr->size -= desiredSize; 								/* less free space now			*/
		*actualSize = desiredSize;															/* return what was desired	*/
		return (offset);
	}
	
	/* Now that we used an entire an free list entry, delete it from the free list and 		*/
	/* free its memory.  If there are no more free list entries, delete the value header 	*/
	/* and the "free space" property from TOC ID 1...																			*/
	
	deleteFreeListEntry(theValue);
	
	container->spaceDeletedValue->value.imm.ulongValue -= *actualSize; /* cut total				*/
	
	return (offset);																					/* return new space to use	*/
}

/*--------------------------------------------------------------------------*
 | cmTakeOverFreeList - have one container take the free space from another |
 *--------------------------------------------------------------------------*
 
 When a target container is open by a container, we want to take over the
 free space in the target container so that we can reuse the free space
 in the target container. Since the target container is open read only, we
 cannot update the free space list there. So the update container move the list
 over so that it becomes the free list of the update container.
 
 This should only be done for one level, so the update container takes the
 free list of the target container. Any free list that is in container that
 is further down may be used already and should not be used.
*/
 
void cmTakeOverFreeList(ContainerPtr toContainer, ContainerPtr container)
{
	TOCValuePtr		 theValue;
	
	TOCValueHdrPtr freeSpaceValueHdr = container->freeSpaceValueHdr;
	if (freeSpaceValueHdr) {
		theValue = (TOCValuePtr)cmGetListHead(&freeSpaceValueHdr->valueList);
		while (theValue) {
			addToPermFreeList(toContainer,
												theValue->value.notImm.value, theValue->value.notImm.valueLen);
			theValue = (TOCValuePtr)cmGetNextListCell(theValue);
		} /* value */
	}
}

/*--------------------------------------------------------------------------*
 | cmWriteData - write data to container with possible resuse of free space |
 *--------------------------------------------------------------------------*
 
 This routine is the Container Manager's low-level value data writer.  It takes a value
 header, and size bytes in the buffer and writes it to the container via the output
 handler.  The amount written is returned.  If it is not equal to the passed size, the
 caller should report an error.
 
 The reason this routine is used rather than using the CMfwrite() handler call directly is
 that this routine checks to see if the container has been opened to reuse free space.  If
 it hasn't, we degenerate into a simple CMfwrite() call.  If it has, then we use the free
 list, built by cmAddToFreeList(), to write out the data segments to reuse the free space.
 
 In all cases the data written is recorded as value segments in the value list belonging
 to the specified value header. The new segments are appended on to the END of the segment
 list.  It is up to the caller to guarantee this is where the segments are to go.  The
 continued flags are appropriately set.
 
 Note, as just mentioned, use this routine ONLY for appending data segments to a value.
 Also, use this routine ONLY if continued segments are permitted for the data.  This means
 you cannot call this routine to write global names.  They are always written as single
 segments.
 
 Even with these restrictions, for the main case, i.e., CMWriteValueData(), this is not a
 problem.  CMWriteValueData() is the primary caller.  It knows what it's doing with the
 data (lets hope so).  So it knows when to call us here.
 
 The net effect is to reuse free space whenever we can if the container was opened for
 updating, and to do straight CMfwrite() otherwise.
*/

CM_ULONG cmWriteData(TOCValueHdrPtr theValueHdr, CM_UCHAR *buffer,
										 CM_ULONG size)
{
	ContainerPtr  container = theValueHdr->container->updatingContainer;
	TOCValuePtr		prevValue;
	CM_ULONG 			chunkSize, offset, amountWritten = 0;
	TOCValueBytes valueBytes;
	Boolean				appendToPrev;

	/* Write out all the data (size bytes of it).  To save a little code space we check 	*/
	/* for updating in the loop.  If we are not updating we will attempt to do a standard	*/
	/* CMfwrite() of all the data as a single value segment.  That is also what happens		*/
	/* when we run out of free space.  Then we try to write out all the remaining data		*/
	/* as a single segment.  The code is the same, so that's why the update check is in 	*/
	/* the loop.  If we are updating, and we do have free list entries, we use them to		*/
	/* see where to write and how much.  This will reuse the free list space.							*/
	
	prevValue = (TOCValuePtr)cmGetListTail(&theValueHdr->valueList); /* last value seg		*/
	
	while (size > 0) {																	/* loop till no more data					*/
		if ((container->useFlags & kCMReuseFreeSpace)==0)	/* if not reusing free space...		*/
			chunkSize = 0;																	/* ...this will cause std write		*/
		else																							/* if updating,get some free space*/
			offset = cmGetFreeListEntry(container, size, false, &chunkSize);
		
		if (chunkSize == 0) {															/* if no free space to reuse...		*/
			offset = CMgetContainerSize(container);
			CMfseek(container, 0, kCMSeekEnd);							/* position to current eof				*/
			if (CMfwrite(container, buffer, sizeof(CM_UCHAR), size) == size) { 			/* write	*/
				amountWritten += size;												/* count total amount written			*/
				container->physicalEOF = offset + size;				/* update next free container byte*/
				SetLogicalEOF(container->physicalEOF);				/* logical EOF == physical EOF		*/
				(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, offset, size);
				cmAppendValue(theValueHdr, &valueBytes, 0);		/* append new value segment				*/
#if CMKEEP_CONTINUE_FLAG
				if (prevValue != NULL) {											/* if this is not 1st segment...	*/
					prevValue->flags |= kCMContinued;						/* ...flag last seg as cont'd			*/
					theValueHdr->valueFlags |= ValueContinued;	/* ...also echo flag in the hdr		*/
				}
#endif
			}
			return (amountWritten);													/* return amount we wrote					*/
		} /* chunkSize == 0 */
		
		/* If we get here then we are going to reuse a free space segment...								*/
		
		CMfseek(container, offset, kCMSeekSet);						/* overwrite the free space chunk	*/
		if (CMfwrite(container, buffer, sizeof(CM_UCHAR), chunkSize) != chunkSize) 
			return (amountWritten);
		
		/* if the free space is right after a previous free space segment, just extend that */

		appendToPrev = true;
		if (prevValue != NULL) {													/* if this is not 1st segment...	*/
			if (((prevValue->flags & kCMImmediate) == 0) &&
					((prevValue->value.notImm.value+prevValue->value.notImm.valueLen) == offset)) {
				prevValue->value.notImm.valueLen += chunkSize;	/* just add to previous value */
				theValueHdr->size += chunkSize;								
				appendToPrev = false;
#if CMKEEP_CONTINUE_FLAG
			} else {																				/* it is discontinous							*/
				prevValue->flags |= kCMContinued;							/* ...flag last seg as cont'd			*/
				theValueHdr->valueFlags |= ValueContinued;		/* ...also echo flag in the hdr		*/
#endif
			};
		}
		
		if (appendToPrev) {
			(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, offset, chunkSize);
			prevValue = cmAppendValue(theValueHdr, &valueBytes, 0);	/* append new value segment	*/
			if (prevValue == NULL) return (amountWritten);
		}
		
		offset += chunkSize;															/* set logical EOF								*/
		SetLogicalEOF(offset);
		
		buffer 				+= chunkSize;												/* point to next chunk to write		*/
		amountWritten += chunkSize;												/* count total amount written			*/
		size 					-= chunkSize;												/* reduce amount yet to be written*/
	} /* while */																				/* loop through all the data			*/
	
	return (amountWritten);															/* return amount we wrote					*/
}


/*----------------------------------------------------------------*
 | cmDeleteFreeSpace - delete free space beyond a specified point |
 *----------------------------------------------------------------*
 
 This routine is called to remove all free list entries that have their space beyond the
 specified offset, beyondHere.  All entries with starting offsets greater than or equal
 to beyondHere are removed.  If one spans beyondHere it will be reduced appropriately.
 
 These entries are removed so so that we may overwrite from beyondHere with a new TOC.
 Since we are reusing that space for the TOC we obviously can't keep free list entries for
 it.
*/

void cmDeleteFreeSpace(ContainerPtr container, CM_ULONG beyondHere)
{
	TOCValuePtr		theValue, nextValue;
	CM_ULONG 			offset, size, newLen;
	TOCValuePtr		spaceDeletedValue = container->spaceDeletedValue;

	if (container->freeSpaceValueHdr == NULL) return;				/* exit if no free list				*/
	
	theValue = (TOCValuePtr)cmGetListHead(&container->freeSpaceValueHdr->valueList);
	
	while (theValue) {																			/* ...scan free list					*/
		nextValue = (TOCValuePtr)cmGetNextListCell(theValue);	/* get next now for deletes		*/
		offset    = theValue->value.notImm.value;							/* offset of a free chunk			*/
		size    = theValue->value.notImm.valueLen;						/* size of a free chunk				*/
		
		if (offset >= beyondHere) {														/* if chunk entirely beyond		*/
			spaceDeletedValue->value.imm.ulongValue -= size;
			deleteFreeListEntry(theValue);											/* ...delete it								*/
		}
		else if (offset + size > beyondHere) { 								/* if partial...							*/
			newLen = beyondHere - offset;												/* ...cut size down						*/
			size -= newLen;																			/* actual size deleted				*/
			spaceDeletedValue->value.imm.ulongValue -= size;		/* decr space deleted					*/
			#if TOC1_SUPPORT
			if (container->majorVersion == 1) {									/* and if format 1 TOC...			*/
				if (newLen <= TOCentrySize) 											/* ...and too small to keep		*/
					deleteFreeListEntry(theValue);									/* ...delete it too						*/
				else {																						/* ...if still acceptable			*/
					container->freeSpaceValueHdr->size -= size;
					theValue->value.notImm.valueLen = newLen;				/* ...set its new smaller size*/
				}
			} else {																						/* if >format 1 TOC						*/
				if (newLen <= MinTOCSize) 												/* ...and still too small			*/
					deleteFreeListEntry(theValue);									/* ...delete it too						*/
				else {																						/* ...if still acceptable			*/
					container->freeSpaceValueHdr->size -= size;
					theValue->value.notImm.valueLen = newLen;				/* ...set its new smaller size*/
				}
			}
			#else
			if (newLen <= MinTOCSize) 													/* ...but if too small to keep*/
				deleteFreeListEntry(theValue);										/* ...delete it too						*/
			else {																							/* ...if still acceptable			*/
				container->freeSpaceValueHdr->size -= size;
				theValue->value.notImm.valueLen = newLen;					/* ...set its new smaller size*/
			}
			#endif
		}
		
		theValue = nextValue;																	/* loop till no more entries	*/
	} /* while */
}

														  CM_END_CFUNCTIONS
