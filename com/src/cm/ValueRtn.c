/* @(#)Z 1.5 com/src/cm/ValueRtn.c, odstorage, od96os2, odos29646d 96/11/15 15:27:40 (96/10/29 09:20:20) */
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
	File:		   Values.c    

	Contains:	Container Manager Common Value Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <5>	 12/9/94	EL		#1182275 Optionally do not maintain
									continue flag.
		 <4>	 9/30/94	EL		#1182275 Delete space in non-update merge
													go to free list.
		 <3>	 9/22/94	EL		#1182275 Last fix put too much space on
													free list.
		 <2>	 8/26/94	EL		#1182275 Put space potentially recoverable
													by merging into the temporary free list.
		 <4>	  6/3/94	EL		Rename Values.h/c to ValueRtn.h/c.
		 <3>	 5/10/94	EL		#1162327. ValueDoNotFree is no longer
													needed.
		 <2>	  5/5/94	EL		Fix bug that some free space is not going
													to the free list.
		 <1>	  2/3/94	EL		first checked in
		 <3>	  1/6/94	EL		Check ValueDoNotFree flag before calling
													cmAddToFreeList.
		 <2>	 11/4/93	EL		Do not put deleted space in target
													container into the free list.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                          <<<   ValueRtn.c    >>>                          |
 |                                                                           |
 |                  Container Manager Common Value Routines                  |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  7/22/92                                  |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file contains various routines needed for value operations, e.g., CMWriteValueData()
 CMDeleteValueData(), etc.  Some of these routines are used when applying updates at open
 time (insert and delete data, move a value header).
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
#ifndef __VALUEROUTINES__
#include "ValueRtn.h"       
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __FREESPACE__
#include "FreeSpce.h" 
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __ERRORRPT__
#include "ErrorRpt.h"      
#endif
#ifndef __UTILITYROUTINES__
#include "Utility.h"        
#endif

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment CMValueOps
#endif


/*----------------------------------------------------------------------------------*
 | cmGetStartingValue - find value and value offset corresponding to value position |
 *----------------------------------------------------------------------------------*
 
 This returns the TOCValuePtr to the value entry which contains the stream starting 
 offset, startingOffset.  Also returned in valueOffset is the offset within the returned
 value entry which is the startingOffset'th byte.
 
 Note, NULL is returned for the value pointer if we cannot find the offset. This could 
 happen when the startingOffset is beyond the end of the value.
 
 This routine is necessary because of continued values (segments) which are represented by
 a list of TOCValue entries off of a TOCValueHdr.  Each entry represents a discontinous
 segment of the value data which is always viewed as a stream of contiguous bytes even 
 though they are not.  This routine is one of those that allows its caller to view the
 stream as contiguous.
*/

TOCValuePtr cmGetStartingValue(TOCValueHdrPtr theValueHdr, CM_ULONG startingOffset, 
															 CM_ULONG *valueOffset)
{
	TOCValuePtr		theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	CM_ULONG	prevOffset, offset;
	
	if (theValue == NULL) return (NULL);									/* saftey test 									*/
	
	/* Global names are handled separately. There is only one value entry for these. The	*/
	/* reason we split these off is because there is no length in the value. The scanning	*/
	/* logic below assumes there is always a length in the value, which there is except 	*/
	/* for this one special case.																													*/
	
	if ((theValueHdr->valueFlags & ValueGlobal) != 0) {
		offset = GetGlobalNameLength(theValue->value.globalName.globalNameSymbol) + 1;
		if (offset <= startingOffset) return (NULL);
		*valueOffset = startingOffset;
		return (theValue);
	}
	
	/* Many times the user is just appending on to the end of value. We can quickly test	*/
	/* for this and eliminate that possibility...																					*/
	
	if (startingOffset >= theValueHdr->size)							/* quick check for appending		*/
		return (NULL);
		
	/* So we don't have a global name and we're not appending.  But most values 		*/
	/* may not be continued either.  So if there is only one value there's not much 			*/
	/* question of which value segment to use!																						*/
	
	if (cmCountListCells(&theValueHdr->valueList) == 1) {
		if (theValueHdr->size <= startingOffset) return (NULL);
		*valueOffset = startingOffset;
		return (theValue);
	}
	
	/* Ok we got to scan the thing.  We potentially can limit the scan if we assume 	*/
	/* that all the continued value segments are approximately the same size.  Then we 		*/
	/* should scan the values low-to-high (left-to-right) by ascending offset if the 			*/
	/* startingOffset is less than half the total size. We should scan the values hit-to-	*/
	/* low (right-to-left) if the startingOffset is more than half way.  That's as fancy 	*/
	/* as we get.  A binary search would be nice.  But since we have a linked list the 		*/
	/* scanning wouldn't really be minimized that much.																		*/
	
	if (startingOffset >= (theValueHdr->size/2)) {				/* scan right-to-left...				*/
		prevOffset = theValueHdr->size;											/* current max offset is size		*/
		theValue = (TOCValuePtr)cmGetListTail(&theValueHdr->valueList);
		while (theValue) {																	/* scan to we find it...				*/
			prevOffset -= theValue->value.notImm.valueLen; 			/* offset to this segment			*/
			if (prevOffset <= startingOffset) break;						/* break if we found it				*/
			theValue = (TOCValuePtr)cmGetPrevListCell(theValue);/* keep scanning...						*/
		} /* while */
	} else {																							/* scan left-to-right...				*/
		offset = 0;																					/* this is current max offset		*/
		while (theValue) {																	/* scan to we find it...				*/
			prevOffset = offset;																/* save current total offset	*/
			offset += theValue->value.notImm.valueLen;					/* add size of this segment		*/
			if (offset > startingOffset) break;									/* break if we found it				*/
			theValue = (TOCValuePtr)cmGetNextListCell(theValue);/* keep scanning...						*/
		} /* while */
	}
		
	if (theValue == NULL) return (NULL);									/* offset out of range					*/
	
	*valueOffset = startingOffset - prevOffset;						/* relativize the offset				*/
	return (theValue);																		/* return value segment entry		*/
}


/*------------------------------------------------------------------------------*
 | cmRead1ValueData - copy data for a single value (segment) to caller's buffer |
 *------------------------------------------------------------------------------*
 
 This routine copies the data for the specified value (NOT a value header -- continued
 values are not worried about here), starting at the specified offset, TO the caller's
 buffer.  A maximum of maxSize characters or the size of the data, which ever is smaller,
 is copied.  The function returns the amount of data copied to the buffer.
 
 It is possible here that what we are reading is from a container TOC that was created for
 updating.  Such a TOC are a mixture of entries from the base TOC and all of its updaters.
 Each updater has its own handler package to access the value data in its respective
 container.  To get at the proper handlers for the value we must know which container 
 "owns" the value.  To that end each value segment entry (a TOCValue) has a container
 pointer of its own.  This points to the container that created that value segment and
 that's the container we use to get the proper handlers.  It is NOT necessarily to the 
 container who "owns" the entire TOC.  It will be if we're not updating, but we never
 count on that fact.
 
 Note, this routine handles the special cases for immediate data.
*/

CM_ULONG cmRead1ValueData(TOCValuePtr theValue, CM_UCHAR *buffer,
													CM_ULONG offset, CM_ULONG maxSize)
{
	ContainerPtr 	container = theValue->container;	/* use container "owning" the value		*/
	CM_ULONG amountRead, len;
	char 					*p;
	
	if (maxSize == 0) return (0);
	
	len = cmGet1ValueSize(theValue) - offset;				/* get size of the value we're reading*/
	if (len == 0) return (0);												/* if offset too far, don't read			*/
	if (len > maxSize) len = maxSize;								/* truncate if necessary to maxSize		*/
	
	if (theValue->flags & kCMGlobalName) {					/* read global names specially				*/
		p = GetGlobalName(theValue->value.globalName.globalNameSymbol);
		memcpy(buffer, p + offset, (size_t)(amountRead = len));
	} else if (theValue->flags & kCMImmediate) {		/* read immediates specially					*/
		p = (CM_CHAR *)theValue->value.imm.ucharsValue;
		memcpy(buffer, p + offset, (size_t)(amountRead = len));
	} else {																				/* non-immediate data									*/
		CMfseek(container, theValue->value.notImm.value + offset, kCMSeekSet);			
		amountRead = CMfread(container, buffer, sizeof(CM_UCHAR), len);
	}
	
	return (amountRead);														/* return amount we read							*/
}


/*---------------------------------------------------------------------*
 | cmOverwrite1ValueData - overwrite data for a single value (segment) |
 *---------------------------------------------------------------------*
 
 This routine copies the data for the specified value (NOT a value header -- continued
 values are not worried about here), starting at the specified offset, FROM the caller's
 buffer. A maximum of size characters are overwritten to the value.  The function returns
 the amount of data copied from the buffer to the value.
 
 Note, overwriting of global names is NOT allowed and ASSUMED not passed to this routine.
 Immediates are, however, handled and can be overwritten up to their existing size.
 
 The reason we don't allow writing to global names is that they are kept in a binary tree
 symbol table as a function of the names. If you could change them you would foul up the
 binary tree search.
 
 One more thing.  The offset to the next free position in the container is NOT updated
 here. Nor should it be.  We are overwriting existing, i.e., already written data.
*/

CM_ULONG cmOverwrite1ValueData(TOCValuePtr theValue, CM_UCHAR *buffer,
															 CM_ULONG offset, CM_ULONG size)
{
	ContainerPtr 	container = theValue->container->updatingContainer;
	CM_ULONG 			amountWritten, len;
	CM_UCHAR 			*p;
	
	if (size == 0) return (0);
	
	len = cmGet1ValueSize(theValue) - offset;				/* get size of the value we're writing*/
	if (len == 0) return (0);												/* if offset too far, don't write			*/
	if (len > size) len = size;											/* truncate if necessary to size			*/
	
	if (theValue->flags & kCMImmediate) {						/* overwrite immediates specially			*/
		p = theValue->value.imm.ucharsValue;
		memcpy(p + offset, buffer, (size_t)(amountWritten = len));
	} else {																				/* non-immediate data									*/
		CMfseek(container, theValue->value.notImm.value + offset, kCMSeekSet);			
		amountWritten = CMfwrite(container, buffer, sizeof(CM_UCHAR), len);
		if (amountWritten != len) {
			ERROR1(CM_err_BadWrite, CONTAINERNAME);
			return (0);
		}
	}
	
	return (amountWritten);													/* return amount actually overwritten	*/
}


/*-----------------------------------------------------------------*
 | cmConvertImmediate - convert a immediate value to non-immediate |
 *-----------------------------------------------------------------*
 
 This routine is called whenever an immediate data value must be converted to a
 non-immediate.  It takes as parameters the pointer to the immediate value and returns
 true if there are no errors.  On return the value will have been converted to a
 non-immediate and the input segment CMValue changed to container the offset to the 
 now written value.
*/

CMBoolean cmConvertImmediate(TOCValuePtr theValue)
{
	ContainerPtr   container;
	TOCValueHdrPtr theValueHdr = theValue->theValueHdr;
	CM_ULONG  valueSize, offset0, offset, actualSize;
	
	container = theValue->container->updatingContainer;	/* get container for the value...	*/
	valueSize = CMGetValueSize((CMValue)theValueHdr);		/* ...get current size of value...*/
	
	/* Reuse free space if we're allowed and there is some.  Otherwise just write to the	*/
	/* end of the container.																															*/
	
	offset0 = cmGetFreeListEntry(container, valueSize, true, &actualSize);
	if (actualSize != 0)
		CMfseek(container, offset0, kCMSeekSet);				/* reuse free space									*/
	else {
		offset0 = CMgetContainerSize(container);				/* get current size of container		*/
		CMfseek(container, 0, kCMSeekEnd);							/* write data to end of container		*/
	}
	
	if (CMfwrite(container, theValue->value.imm.ucharsValue, sizeof(CM_UCHAR), valueSize) != valueSize) {
		ERROR1(CM_err_BadWrite, CONTAINERNAME);
		return (false);
	}
	
	/* Redefine the value to be non-immediate...																					*/
	
	(void)cmSetValueBytes(container, &theValue->value, Value_NotImm, offset0, valueSize);
	theValue->flags = 0;
	theValueHdr->valueFlags = ValueDefined;
	
	offset = offset0 + valueSize;											/* update logical OR physical EOF		*/
	if (actualSize == 0)
		container->physicalEOF = offset;								/* update next free container byte	*/
	SetLogicalEOF(offset);														/* set logical EOF (may != physical)*/
	
	theValue->container = container;									/* owner is now updating container	*/
		
	return (true);
}


/*--------------------------------------------------------------------------------------*
 | cmInsertNewSegment - create and insert a new value segment to into an existing value |
 *--------------------------------------------------------------------------------------*
 
 This routine is called by CMInsertValueData() to create a new value segment insert to be
 inserted into a previously existing value.  It is also called when applying data insert
 updates at open time (by the internal routine applyValueUpdateInstructions() in 
  Update.c ).  The new insert data is already written to the (updating) container, and the
 data is at container offset dataOffset.  It is size bytes long.  The insert is to be at
 the specified segOffset within the value segment insBeforeValue. 
 
 The function returns a pointer to the newly created insert segment, or NULL if there is
 an error and the error reporter returns.
 
 Data inserts fall into three cases:
	
	 (1). Inserting into an immediate.  If the size of the immediate plus the size of the
				insert still fits in a immediate the result will remain an immediate.  Otherwise,
				the immediate is converted to a non-immediate and we have cases (2) or (3).  
				Immediates are handled differently for updating and like normal insertions
				processed separately.
				
	 (2). Inserting into the START of an existing non-immediate value segment (segOffset
	 		  offset 0).  The insert is made a segment and placed before the existing segment
				(insBeforeValue).
				
	 (3). Inserting into the MIDDLE of a non-immediate value segment.  The existing segment
				must be split into two segments and the insert segment placed between them.

 CMInsertValueData() handles case (1) and calls this routine for cases (2) and (3) after
 it determines it has these cases and after it has written the new insert data to the
 container.
 
 Updating treats immediates separately and thus, like CMInsertValueData(), only cases (2)
 and (3) are possible here.  Indeed, it is because of updating that this routine exists
 at all!   If we didn't have updating, this code could live directly in 
 CMInsertValueData().  In CMInsertValueData() the caller explicitly passes the data which
 CMInsertValueData() must write out to the container.  However, for updating, we already
 have the data written in the updating container.  Both cases thus merge here to create
 the segment for that data and insert it into the segment list in the appropriate place.
*/

TOCValuePtr cmInsertNewSegment(TOCValueHdrPtr theValueHdr, TOCValuePtr insBeforeValue,
															 CM_ULONG segOffset, CM_ULONG dataOffset,
															 CMSize size)
{
	ContainerPtr  container = theValueHdr->container->updatingContainer;
	TOCValuePtr   theLeftValue, theInsertValue;
	TOCValueBytes valueBytes;

	(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, dataOffset, size);
	theInsertValue = cmCreateValueSegment(theValueHdr, &valueBytes, 0);
	if (theInsertValue == NULL) return (NULL);				/* if insert failed, abort now			*/

	theInsertValue->logicalOffset = insBeforeValue->logicalOffset + segOffset; 

	if (segOffset > 0) {															/* split the segment to be inserted	*/
		theLeftValue = insBeforeValue;									/* use original seg for left portion*/
		(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, 					 /*right value*/
													insBeforeValue->value.notImm.value+segOffset,		 /*fix offset	*/
													insBeforeValue->value.notImm.valueLen-segOffset);/* ...& len	*/
		insBeforeValue = cmCreateValueSegment(theValueHdr,/* create right portion of insert	*/
																					&valueBytes,/* with adjusted offset length		*/
																					theLeftValue->flags);
		if (insBeforeValue == NULL) return (NULL);			/* something went wrong?						*/
		insBeforeValue->logicalOffset = theInsertValue->logicalOffset; /* adjust logical offset	*/
		insBeforeValue->container = theLeftValue->container;/* container is still the same	*/
#if CMKEEP_CONTINUE_FLAG
		theLeftValue->flags |= kCMContinued;						/* flag the left portion as cont'd	*/
		theValueHdr->valueFlags |= ValueContinued;			/* echo flags in the header					*/
#endif
		theLeftValue->value.notImm.valueLen = segOffset;/* set size of left portion					*/
		
		/* Insert right portion after the left portion. The insBeforeValue pointer will now */
		/* be pointing at the segment we want to insert the new segment before.							*/
		
		(void)cmInsertAfterListCell(&theValueHdr->valueList, insBeforeValue, theLeftValue);
	} /* splitting segment */
	
	/* Now cases (2) and (3) come back together.  We insert the new segment in front of		*/
	/* the right segment pointed to by insBeforeValue.  That will make it a continued 		*/
	/* value.																																							*/
	
	(void)cmInsertBeforeListCell(&theValueHdr->valueList, theInsertValue, insBeforeValue);
#if CMKEEP_CONTINUE_FLAG
	theInsertValue->flags = (CM_USHORT)(insBeforeValue->flags | kCMContinued); 
	theValueHdr->valueFlags |= ValueContinued;									/* echo flags in the hdr	*/
#endif
	theValueHdr->size += theInsertValue->value.notImm.valueLen; /* add to total value len	*/

	return (theInsertValue);													/* give back newly inserted segment	*/
}

/*--------------------------------------------------------------------*
 | checkForReuse - check to see if data space can be reused in future |
 *--------------------------------------------------------------------*
 
 This routine checks to see if the free space can be reused, reserved for
 future, or cannot be reused at all.
*/

static void CM_NEAR checkForReuse(ContainerPtr container, 
																	TOCValuePtr theValueToFree, 
																	CM_ULONG offset, CM_ULONG size)
{
	/* If the value is in the updating container, we call cmAddToFreeList, which nomally	*/
	/* put it on the permanent free list, unless the container is open for read/write.		*/
	if (container == theValueToFree->container)
		cmAddToFreeList(container, NULL, offset, size);
	/* If we are merging, then any value on the top level container may be merged later.	*/
	/* So we can put it in the temporary free list.																				*/
	else if ((theValueToFree->container->depth == 1) &&
					 (container->useFlags & kCMMerging) &&
					 (container->trackFreeSpace))
			cmAddToTmpFreeList(container, offset, size);
}

/*-----------------------------------------------------------------*
 | cmDeleteSegmentData - delete data represented by value segments |
 *-----------------------------------------------------------------*
 
 This routine is called by CMDeleteValueData() to delete value data  It is also called
 when applying data deletion updates at open time (by the internal routine
 applyValueUpdateInstructions() in  Update.c ).  All the data starting at offset 
 startOffset, up to and including the end offset, endOffset, are to be deleted.  If the
 start and end offsets span entire value segments, those segments are removed.
 
 Note, this routine is for non-immediate data deletions only.  CMDeleteValueData() handles
 deletions on immediates.  For updating, immediates are handled differently and thus can't
 get in here.
 
 The start and end may map into the middle of segments.  This means that deleions of 
 partial segments is possible.  Both the start and end offsets may map into a single
 segment.  In that case the segment must be split into twp segments.  The comments in the
 code explain these cases.
*/

void cmDeleteSegmentData(TOCValueHdrPtr theValueHdr, CM_ULONG startOffset,
																							 			 CM_ULONG endOffset)
{
	TOCValuePtr		 theStartValue, theEndValue, thePrevValue, theNextValue, theValue,
								 theRtValue;
#if CMKEEP_CONTINUE_FLAG
	TOCValuePtr		 thePrevValue1;
#endif
	ContainerPtr 	 container = theValueHdr->container->updatingContainer;
	CM_ULONG	 		 offset1, offset2, segMaxOffset, amountDeleted, totalDeleted;
	TOCValueBytes  valueBytes;
	
	/* Map the start and end offsets into the offsets within their respective segments...	*/
	
	theStartValue  = cmGetStartingValue(theValueHdr, startOffset, &startOffset);
	theEndValue  	 = cmGetStartingValue(theValueHdr, endOffset, 	&endOffset);
	
	/* We now have the start and end value segments and offsets within them.  Everything	*/
	/* between is to be deleted.  We will loop through the segments starting with 				*/
	/* theStartValue and up to theEndValue.  The startOffset and endOffset will be used 	*/
	/* to delete the partial first and last segment portions.  Remember all this stuff 		*/
	/* could be for a single segment or multiple segments.  All the "interior" segments		*/
	/* for a multi-segment delete are removed.  The partial segment cases take a little 	*/
	/* more work.																																					*/
	
	thePrevValue = NULL;															/* this is segment just processed		*/
	theValue 		 = theStartValue; 										/* this is the segment to process		*/
	totalDeleted = 0;																	/* sum total amount deleted					*/
	
	while (thePrevValue != theEndValue) {							/* loop through the segments...			*/
		theNextValue = (TOCValuePtr)cmGetNextListCell(theValue);/* next now for seg delete	*/
		thePrevValue = theValue;												/* remember segment we're processing*/
		
		segMaxOffset = theValue->value.notImm.valueLen - 1;			/* get segments max offset	*/
		offset1 = (theValue == theStartValue) ? startOffset : 0;/* get segment offsets...		*/
		offset2 = (theValue == theEndValue	) ? endOffset		: segMaxOffset;
		
		/* There are four cases depending on offset1 and offset2, i.e., the offsets within	*/
		/* the current segment we're dealing with each time around this loop:								*/
		
		/*	(1). The offsets specify the entire segment is to be deleted.	[///////]					*/
		/*	(2). offset1 == 0 ==> delete left portion of segment.					[////   ]					*/
		/* 	(3). offset2 == segMaxOffset == delete right portion.					[   ////]					*/
		/*	(4)  if not (1), (2), (3) delete interior portion.						[  ///  ]					*/
		
		/* The last case requires us to split the segment into two segments which means we 	*/
		/* must "manufacture" an additional segment.																				*/
		
		if (offset1 == 0 && offset2 == segMaxOffset) {	/* delete entire segment...					*/
			if (cmCountListCells(&theValueHdr->valueList) == 1)	{ /* if value going null...		*/
				checkForReuse(container, theValue,					/* add freed space to free list			*/
											theValue->value.notImm.value,		/* from the beginning of the value*/
											theValue->value.notImm.valueLen); /* and take the whole thing.		*/
				theValue->value.notImm.value 		= 0;				/* ...define remaining (only) seg		*/
				theValue->value.notImm.valueLen = 0;				/*    as a null value								*/
				theValue->flags = kCMImmediate;							/*		a null immediate value				*/
				theValueHdr->valueFlags |= ValueImmediate;	/* 		echo flag in the value hdr		*/
			} else {																			/* value not null (yet), delete seg	*/
#if CMKEEP_CONTINUE_FLAG
				if (cmGetNextListCell(theValue) == NULL) {	/* make sure of seg cont'd bit...		*/
					thePrevValue1 = (TOCValuePtr)cmGetPrevListCell(theValue);
					if (thePrevValue1 != NULL)								/* ...no longer cont'd if last seg	*/
						thePrevValue1->flags &= ~kCMContinued;	/*    is to be deleted							*/
				}
#endif
				checkForReuse(container, theValue,					/* add freed space to free list			*/
											theValue->value.notImm.value,		/* from the beginning of the value*/
											theValue->value.notImm.valueLen); /* and take the whole thing.		*/
				cmDeleteListCell(&theValueHdr->valueList, theValue);
				CMfree(theValue);														/* poof!														*/
			}
			amountDeleted = segMaxOffset + 1;							/* this is how much we're deleting	*/
		} else if (offset1 == 0) {											/* delete left portion of 1st seg...*/
			amountDeleted = offset2 + 1;									/* this is how much we're deleting	*/
			checkForReuse(container, theValue, 						/* add freed space to free list			*/
												theValue->value.notImm.value,	/* ...this is freed left part				*/
												amountDeleted);
			theValue->value.notImm.value += amountDeleted;/* bump up offset										*/
			theValue->value.notImm.valueLen  = segMaxOffset - offset2; /* adjust length				*/
			theValue->logicalOffset += amountDeleted;			/* adjust starting logical offset		*/
		} else if (offset2 == segMaxOffset) {						/* delete rt. portion of last seg...*/
			amountDeleted = segMaxOffset - offset1 + 1;		/* this is how much we're deleting	*/
			checkForReuse(container, theValue, 						/* add freed space to free list			*/
										theValue->value.notImm.value + offset1,	/* this is freed right part */
										amountDeleted);
			theValue->value.notImm.valueLen = offset1;
		} else {																				/* delete interior portion of seg...*/
			(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, 			/* rt. chunk		*/
														theValue->value.notImm.value + offset2 + 1,	/* adjust offset*/
														segMaxOffset - offset2);										/* ...and length*/
			theRtValue = cmCreateValueSegment(theValueHdr,/* create right portion 						*/
																			  &valueBytes, theValue->flags);
			if (theRtValue == NULL) return;								/* something went wrong?						*/
			theRtValue->logicalOffset = theValue->logicalOffset+offset2+1; /* fix log. offset	*/
			theRtValue->container = theValue->container;	/* rt. seg is same container as left*/ 
			(void)cmInsertAfterListCell(&theValueHdr->valueList, theRtValue, theValue);
#if CMKEEP_CONTINUE_FLAG
			theValue->flags |= kCMContinued;							/* flag the left portion as cont'd	*/
#endif
			theValue->value.notImm.valueLen = offset1;		/* set size of left portion					*/
			amountDeleted = offset2 - offset1 + 1;				/* this is how much we're deleting	*/
			checkForReuse(container, theValue, 						/* add freed space to free list			*/
										theValue->value.notImm.value + offset1,		/* this is "hole" created */
											 	amountDeleted);
		}
	
		totalDeleted += amountDeleted;									/* tally total amount we delete			*/
		theValue = theNextValue; 												/* point at next segment						*/
	}	/* while */																			/* loop...													*/

	theValueHdr->size -= totalDeleted;								/* update total size in value hdr		*/
#if CMKEEP_CONTINUE_FLAG
	if (cmCountListCells(&theValueHdr->valueList) < 2)/* make sure of cont'd bit in hdr		*/
		theValueHdr->valueFlags &= ~ValueContinued;
#endif
}


 /*---------------------------------------------------------------------*
  | cmMoveValueHdr - move a value (header) to a new position in the TOC |
  *---------------------------------------------------------------------*

 This routine is called by CMMoveValueData() to move a value (header).  It is also called
 when applying "inserted" (move) updates at open time (by the internal routine
 applyValueUpdateInstructions() in  Update.c ).   The value is physically deleted from its
 original object/property as if a CMDeleteValue() were done on it.  If the value deleted
 is the only one for the property, the property itself is deleted as in
 CMDeleteObjectProperty(). 
 
 The value is added to the "to"s object propery in a manner similar to a CMNewValue(). The
 order of the values for both the value's original object property and for the value's
 new object property may be changed.

 Note, that although the effect of a move is a combination CMDeleteValue()/CMNewValue(),
 THE INPUT REFNUM REMAINS VALID!  Its association is now with the new object property.

 The input refNum is returned as the function result.  NULL is returned if there is an 
 error and the error reporter returns.
*/

TOCValueHdrPtr cmMoveValueHdr(TOCValueHdrPtr theFromValueHdr, CMObject object, CMProperty property)
{
	TOCObjectPtr 	 theFromObject, theToObject;
	TOCPropertyPtr theFromProperty;
	TOCValueHdrPtr theToValueHdr, prev, next;
	ContainerPtr	 container = theFromValueHdr->container;

	/* We will need pointers to the the "from" object and its property...									*/
	
	theFromProperty = theFromValueHdr->theProperty;
	theFromObject 	= theFromProperty->theObject;
	
	/* For the fun of it see if the "from" and "to" represent the SAME place.  There's		*/
	/* no sense going on with this turkey if they are!																		*/
	
	if (theFromObject == (TOCObjectPtr)object)
		if (cmFindObject(container->toc, theFromProperty->propertyID) == (TOCObjectPtr)property)
			return (theFromValueHdr);													/* incredible, ain't it?				*/
		
	/* Now that we got all of that stuff out of the way we can proceed. The easiest way to	*/
	/* do the move is to create a dummy value entry in the "to" object property chain and	*/
	/* then do list manipulations on it to move the "from" value header and its value			*/
	/* chain.  We then must make sure to free the dummy we just created.  Doing it this		*/
	/* way allows us to use the mechanisms already in place without providing special 		*/
	/* case code or routines.  For example, we don't have to worry about creating the			*/
	/* property at the destination if it does not happen to previously exist.							*/
		
	/* The first thing we do is remove the "from" value header from its property list.		*/
	/* This does not free the space.  It only jumps it out of the links. 									*/
	
	cmDeleteListCell(&theFromValueHdr->theProperty->valueHdrList, theFromValueHdr);
	prev = (TOCValueHdrPtr)cmGetPrevListCell(theFromValueHdr); /* save for error recovery	*/
	next = (TOCValueHdrPtr)cmGetNextListCell(theFromValueHdr);
	cmNullListLinks(theFromValueHdr);
	
	/* Now we can create the dummy "to" value header place holder.  For lack of anything	*/
	/* better, we give it the same flags, etc. from the "from" value.  By giving it the 	*/
	/* same type, we automatically get the check for duplicate types in the "to" property.*/
	
	theToObject = cmDefineObject(container, ((TOCObjectPtr)object)->objectID,
															 ((TOCObjectPtr)property)->objectID,
															 theFromValueHdr->typeID,	  /* error if dup 							*/
															 NULL, theFromValueHdr->generation, 0, 
															 theFromObject->objectFlags,
															 &theToValueHdr);
															 
	if (theToObject == NULL) {															/* if something is wrong...		*/
		if (prev != NULL)																			/* put back original "from"		*/
			cmInsertAfterListCell(&theFromValueHdr->theProperty->valueHdrList, theFromValueHdr, prev);
		else if (next != NULL)
			cmInsertBeforeListCell(&theFromValueHdr->theProperty->valueHdrList, theFromValueHdr, next);
		else
			cmAppendListCell(&theFromValueHdr->theProperty->valueHdrList, theFromValueHdr);
		return (NULL);
	}
	
	/* Now we want to replace the value header we just created with the "from" value			*/
	/* header (which is now deleted off the "from" property list and only pointed to by		*/
	/* theFromValueHdr).  That will carry along all the value segments on the value 			*/
	/* header's chain and all the proper field settings in the value header as well.			*/
	
	/* We play some list manipulation games to move the "from" value header to the "to" 	*/
	/* list.  We know where the "to" dummy header was placed, theToValueHdr. So all we 		*/
	/* have to do is insert the "from" value header before the dummy and then delete the 	*/
	/* dummy.  We also now have to set the owning property field to point to the owning 	*/
	/* property on the new list.																													*/
	
	cmInsertBeforeListCell(&theToValueHdr->theProperty->valueHdrList, theFromValueHdr, theToValueHdr);
	theFromValueHdr->theProperty = theToValueHdr->theProperty;
	cmDeleteListCell(&theToValueHdr->theProperty->valueHdrList, theToValueHdr);
	CMfree(theToValueHdr);																		/* done with the dummy			*/
	
	/* That was easy!  Now we look at the property for "from" value header and delete it	*/
	/* if there are no more values. 																											*/
	
	if (cmIsEmptyList(&theFromProperty->valueHdrList))
		CMfree(cmDeleteListCell(&theFromObject->propertyList, theFromProperty));
		
	return (theFromValueHdr);																	/* returned move refNum			*/
}
														  
															CM_END_CFUNCTIONS
