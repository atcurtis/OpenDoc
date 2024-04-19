/* @(#)Z 1.4 com/src/cm/Refs.h, odstorage, od96os2, odos29646d 96/11/15 15:27:14 (96/10/29 09:19:07) */
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
	File:		Refs.h

	Contains:	Container Manager Object Reference Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <2>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<<    Refs.h    >>>                           |
 |                                                                           |
 |               Container Manager Object Reference Interfaces               |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 10/25/92                                  |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file defines the routines needed by others to access object references maintained by
 the API Reference operations defined in CMReferenceOps.c.  Object references are
 "pointers" (i.e., object IDs) to other objects from a value (CMValue) that contains data
 that refers to those objects.  The data is in the form of a CMReference "key".  The
    CMRefOps.c    API routines manipulate those key/object (ID) associations.

 The associations are maintained in a list as value data for a uniquely typed value in a
 private object "tied" to the value through a pointer (refDataObject) in the value header.
*/

 
#ifndef __REFERENCES__
#define __REFERENCES__

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif

struct TOCValueHdr;
																	
																	CM_CFUNCTIONS

/* References are recorded in a value's recording object as a sequence of entries with	*/
/* the following layout...																															*/

struct ReferenceData {												/* Layout of recording objects ref data:	*/
	CMReference		key;													/* 		reference key												*/
	CM_UCHAR objectID[sizeof(CMReference)];/*		associated referenced object ID			*/
};
typedef struct ReferenceData ReferenceData;


/* The actual reference list data is maintained as value data for the recoding object.	*/
/* To increase efficiency, the data is read into memory the first time it's accessed.		*/
/* From that point on, all updates to the actual data are "shadowed" in the in-memory		*/
/* list.  The updating can't be avoided (so updating, e.g., touches, etc. work 					*/
/* correctly). But at least the searches can be made to not be I/O bound! The following */
/* describes the layout for the in-memory list.  There's a list header and the list 		*/
/* entries themselves.  It's a doubly linked list so that the ListMgr.c routines can 		*/
/* used and to make deletion easier.  						 																			*/

struct RefDataShadowEntry {										/* Reference list data shadow entries:		*/
	ListLinks 		refDataLinks;									/*		links to next/prev data(must be 1st)*/
	CM_ULONG		key;													/*		ref key (internalized CMReference)	*/
	CMObjectID 		objectID;											/*		associated object ID								*/
};
typedef struct RefDataShadowEntry RefDataShadowEntry, *RefDataShadowEntryPtr;


/* The list header is pointed to by the SAME value header field usually used to point 	*/
/* to the recording object (refDataObject).  However, a union is used to use a more 		*/
/* appropriate name (refShadowList).  We can always tell which is which because the			*/
/* refShadowList will only have meaning in the recording object's value itself which is	*/
/* uniquely typed (CM_StdObjID_ObjRefData).	  If the field is not NULL, but not that		*/
/* type, then we know we have a pointer to the recording object because in all other 		*/
/* values this field is NULL.  To aid in using these fields in a value header, the 			*/
/* following macros are provided.																												*/

#define RefDataObject(v)		(((TOCValueHdrPtr)(v))->references.refDataObject)
#define RefShadowList(v)		(((TOCValueHdrPtr)(v))->references.refShadowList)

#define HasRefDataObject(v)	(RefDataObject(v) != NULL && \
												 		 ((TOCValueHdrPtr)(v))->typeID != CM_StdObjID_ObjRefData)

#if CMSHADOW_LIST
#define HasRefShadowList(v)	(RefShadowList(v) != NULL && \
														 ((TOCValueHdrPtr)(v))->typeID == CM_StdObjID_ObjRefData)
#else
#define HasRefShadowList(v)	0
#endif


#if CMSHADOW_LIST

void cmDeleteRefDataShadowList(struct TOCValueHdr *refDataValueHdr);
	/*
	This routine is called to delete the entire shadow list pointed to from the specified
	recording object's value header.  It is used for clearing the list during error recovery
	and value (header) deletions.
	
	Note, generally the caller should have done a HasRefShadowList(refDataValueHdr) prior to
	calling this routine to make sure that the value header is indeed a recording object
	value header.
	*/

#endif

														  CM_END_CFUNCTIONS
#endif
