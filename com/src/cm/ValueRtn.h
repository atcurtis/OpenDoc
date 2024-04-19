/* @(#)Z 1.4 com/src/cm/ValueRtn.h, odstorage, od96os2, odos29646d 96/11/15 15:27:43 (96/10/29 09:20:25) */
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
	File:		ValueRtn.h

	Contains:	Container Manager Common Value Routines Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182275 Container merging.
		 <2>	  6/3/94	EL		Rename Values.h/c to ValueRtn.h/c.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                          <<<   ValueRtn.h    >>>                          |
 |                                                                           |
 |             Container Manager Common Value Routines Interfaces            |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  7/22/92                                  |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file contains the interfaces to the various routines needed for value operations,
 e.g., CMWriteValueData() CMDeleteValueData(), etc.  Some of these routines are used when
 applying updates at open time (insert and delete data, move a value header).
*/

#ifndef __VALUEROUTINES__
#define __VALUEROUTINES__


#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif

struct TOCValueHdr;
struct TOCValue;
struct TOCObject;


																	CM_CFUNCTIONS


struct TOCValue *cmGetStartingValue(struct TOCValueHdr *theValueHdr,
																	  CM_ULONG startingOffset, 
																		CM_ULONG *valueOffset);
	/*
	This returns the TOCValuePtr to the value entry which contains the stream starting 
	offset, startingOffset.  Also returned in valueOffset is the offset within the returned
	value entry which is the startingOffset'th byte.
	
	Note, NULL is returned for the value pointer if we cannot find the offset. This could 
	happen when the startingOffset is beyond the end of the value.
	
	This routine is necessary because of continued values which are represented by a list of
	TOCValue entries off of a TOCValueHdr.  Each entry represents a discontinous segment of
	the value data which is always viewed as a stream of contiguous bytes even though they 
	are not.  This routine is one of those that allows its caller to view the stream as
	contiguous.
	*/
	

CM_ULONG cmRead1ValueData(struct TOCValue *theValue, CM_UCHAR *buffer,
													CM_ULONG offset,CM_ULONG maxSize);
	/*
	This routine copies the data for the specified value (NOT a value header -- continued
	values are not worried about here), starting at the specified offset, TO the caller's
	buffer.  A maximum of maxSize characters or the size of the data, which ever is smaller,
	is copied.  The function returns the amount of data copied to the buffer.
	
	Note, this routine handles the special cases for immediate data.
	*/


CM_ULONG cmOverwrite1ValueData(struct TOCValue *theValue, CM_UCHAR *buffer,
															 CM_ULONG offset, CM_ULONG size);
	/*
	This routine copies the data for the specified value (NOT a value header -- continued
	values are not worried about here), starting at the specified offset, FROM the caller's
	buffer. A maximum of size characters are overwritten to the value.  The function returns
	the amount of data copied from the buffer to the value.
	
	Note, overwriting of global names is NOT allowed and assumed not passed to this routine.
	Immediates are, however, handled and can be overwritten up to their existing size.
	*/
	
	
CMBoolean cmConvertImmediate(struct TOCValue *theValue);
	/*
	This routine is called whenever an immediate data value must be converted to a
	non-immediate.  It takes as parameters the pointer to the immediate value and returns
	true if there are no errors.  On return the value will have been converted to a
	non-immediate and the input segment CMValue changed to container the offset to the 
	now written value.
	*/
	
	
struct TOCValue *cmInsertNewSegment(struct TOCValueHdr *theValueHdr,
																	 	struct TOCValue *insBeforeValue,
															 		 	CM_ULONG segOffset, CM_ULONG dataOffset,
															 			CMSize size);
	/*
	This routine is called by CMInsertValueData() to create a new value segment insert to be
	inserted into a previously existing value.  It is also called when applying data insert
	updates at open time (by the internal routine applyValueUpdateInstructions() in 
	 Update.c ).  The new insert data is already written to the (updating) container, and the
	data is at container offset dataOffset.  It is size bytes long.  The insert is to be at
	the specified segOffset within the value segment insBeforeValue. 
	
	The function returns a pointer to the newly created insert segment, or NULL if there is
	an error and the error reporter returns.

	Note, this routine is for non-immediate data insertions only.  CMInsertValueData() 
	handles insertions on immediates.  For updating, immediates are handled differently and
	thus can't get in here.
	*/
	
	
void cmDeleteSegmentData(struct TOCValueHdr *theValueHdr, CM_ULONG startOffset,
												 CM_ULONG endOffset);
	/*
	This routine is called by CMDeleteValueData() to delete value data  It is also called
	when applying data deletion updates at open time (by the internal routine
	applyValueUpdateInstructions() in  Update.c ).  All the data starting at offset 
	startOffset, up to and including the end offset, endOffset, are to be deleted.  If the
	start and end offsets span entire value segments, those segments are removed.
 
	Note, this routine is for non-immediate data deletions only.  CMDeleteValueData() handles
	deletions on immediates.  For updating, immediates are handled differently and thus can't
	get in here.
	*/
	
	
struct TOCValueHdr *cmMoveValueHdr(struct TOCValueHdr *theFromValueHdr, CMObject object,
																												 					 			CMProperty property);
	/*
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
	

														  CM_END_CFUNCTIONS
#endif
