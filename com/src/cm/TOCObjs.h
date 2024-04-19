/* @(#)Z 1.4 com/src/cm/TOCObjs.h, odstorage, od96os2, odos29646d 96/11/15 15:27:32 (96/10/29 09:19:54) */
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
	File:		 TOCObjs.h  

	Contains:	Container Manager TOC Objects Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182275 Add constants EditedRecently and
									DeletedRecently to be used for merging.
		 <2>	 3/31/94	EL		Add cmPutObjectInTOC call. #1150214
		 <1>	  2/3/94	EL		first checked in
		 <4>	  2/2/94	EL		cmFreeLastDeletedObject no longer needed,
													use refNumHandling to control freeing.
		 <3>	10/29/93	EL		Add new call cmFreeLastDeletedObject so we
													can free the reference associated with
													value
		 <2>	 10/4/93	EL		Add cmGetMasterListTail call.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<<  TOCObjs.h   >>>                            |
 |                                                                           |
 |                  Container Manager TOC Objects Interfaces                 |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/20/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 All TOC object manipulation routines are contained in this file. Only manipulations down
 to the object level are done here.  The objects themselves contain pointers off to lower
 level structures which are maintained by TOCEntries.c. The split is done this way to keep
 the accessing of objects separate from the stuff represented by those objects.
 
 TOC objects are based on their ID's.  The ID's do not lend themselves well to the 
 standard binary-tree mechanism we use elsewhere so another scheme is used here.
 
 TOC objects are access through a set of index tables.  The tables correspond to "powers"
 of a chosen index table size.  For example, if the size was 100 and we had ID 1234567 we
 would have 4 indices: 01, 23, 45, 67.  Four index tables would exist each corresponding
 to the indices 00 to 99.  The first table would have its 01'th entry pointing to the next
 table.  That next table would have its 23rd entry pointing to the third table.  The 
 third table would have its 45th entry pointing to the last table.  The 67th entry in the
 last table would point to the actual object with ID 1234567.
 
 The actual number of tables is dependent on the table size and the max ID value supported
 (which is 0xFFFFFFFF).  Small ids are padded with 00 as the higher order indices.
 
 The number of tables is dependent on the table size and the sparseness of the ID's
 presented.  This scheme represents a comprimise between speed, space, and the fact that
 we want to always have the ID's in sorted order.  We get the sorted order by walking the
 index tables in the appropriate way.
 
 The support for this scheme has been generalized to allow any size index table. 
 
 Multiple TOC's are supported.  Each TOC is tied to a anonymous reference number from
 the outside caller's point of view.  In here it is a pointer to a control block that
 contains the root pointer to the TOC index tables and the control variables to access
 through the index tables mentioned above.
 
 Each TOC is independent.  Since the control variables are tied to the TOC, it is allowed
 to have different size index tables for different TOC.  It's not clear this has any
 benefit, but it's allowed.
 
 The TOC routines only know the final TOC entries down to the level of "objects".  The
 objects themselves contain fields maintained by other routines outside the scope of this
 file.  In particular the object is mainly a "head cell" for a list of entries chained
 off of it.  The  ListMgr package is used to maintain the lists by the caller by using the
 object.  Other fields are provided as well which are also maintained by the caller.  The
 only exception is the object ID number which is known to this file.
*/

#ifndef __TOCOBJECTS__
#define __TOCOBJECTS__


#include <setjmp.h>

/*------------------------------------------------------------------------------------*
 | W A R N I N G - - -> this header as well as  TOCObjs.h   and  TOCEnts.h   all have |
 |                      mutual references.  The include order is NOT arbitrary.  It's |
 |                      the only one that works!  Even with it we have to do some     |
 |                      forward struct pointer references of the form "struct foo *p" |
 *------------------------------------------------------------------------------------*/

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif

struct Container;
struct TOCObject;

																	CM_CFUNCTIONS


/* All in-memory objects have the following layout. There objects are accessed by their	*/
/* object ID. 																																					*/

struct TOCObject {										/* Layout of a TOC object:												*/
	CMObjectID			 objectID;					/*		the object's ID	 (keep first for debugging) */
	ListHdr					 propertyList;			/* 		list of object property entries							*/
	struct Container *container;				/*		ptr to "owning" container control block			*/
	struct TOCObject *nextObject;				/* 		chain to next object by increasing ID				*/
	struct TOCObject *prevObject;				/* 		chain to previous object by decreasing ID		*/
	struct TOCObject *nextTypeProperty;	/* 		chain of next type/property by increasing ID*/
	struct TOCObject *prevTypeProperty;	/* 		chain of previous type/property by decr. ID	*/
	CM_USHORT	 			 objectFlags;				/*		info flags about the object									*/
	CMRefCon 				 objectRefCon;			/*		user's object refCon												*/
	CM_ULONG			   useCount;					/*		count of nbr of times "used"								*/
	struct TOCObject *nextTouchedObject;/*		link to next touched object									*/
	ListHdr					 touchedList;				/* 		values/properties touched IN this object		*/
};
typedef struct TOCObject TOCObject, *TOCObjectPtr;

/* NOTE: there is a mutual reference here between Container and TOCObject.  They refer	*/
/* to each other!  Thus we must do the weird "struct Container" to define the container	*/
/* pointer above.  Sorry about that!																										*/

#define UndefinedObject 		0x0001U		/* objectFlags: 1 ==> object created but undefined*/
#define ObjectObject	 			0x0002U		/* 								==> object is a base object			*/
#define PropertyObject 			0x0004U		/*								==> object is a property				*/
#define TypeObject		 			0x0008U		/*					 	  	==> object is a type						*/
#define DeletedObject				0x0010U		/*						 		==> object has been deleted			*/
#define DeletedRecently			0x0200U		/*								==> object prop deleted recently*/
#define EditedRecently			0x0400U		/*								==> object edited recently			*/
#define DynamicValuesObject	0x0800U		/*								==> object "owns" dynamic values*/
#define TouchedObject				0x1000U		/*								==> object has been "touched"		*/
#define ProtectedObject			0x2000U		/*								==> object is locked/protected	*/
#define LinkedObject				0x4000U		/*								==> object linked to master lsts*/
#define UndefObjectCounted 	0x8000U		/*								==> object counted as undefined	*/

#define DefaultIndexTableSize	256			/* "recommended" index table size									*/
#define MinIndexTableSize			16			/* minimum index table size												*/
#define MaxIndexTableSize			1024		/* maximum index table size												*/

#define ALLOBJECTS			0x0000000UL		/* starting ID for cmForEachObject() for all objs	*/
#define MAXUSERID				0xFFFFFFFFUL	/* ending ID to scan objects to the highes user ID*/

/* The following is used to increment object ID counters. The counter is set to 0 if it	*/
/* wraps.  Any attempt to use the counter after that to define an object will result in	*/
/* an error.																																						*/

#define IncrementObjectID(id) (id = ((id) == MAXUSERID) ? 0x00000000UL : ++(id))


void *cmCreateTOC(CM_ULONG tableSize, const struct Container *container);
	/*
	This routine MUST be called before any other TOC routine is called. It is used to set up
	the data needed by the other TOC routines.  A "reference number" is returned to the
	caller who must pass it to ALL the other TOC routines.  Any number of TOC can be 
	created.  By passing the returned "reference number" to the other routines they will
	operate on the corresponding TOC.
 
	Note, a TOC is "owned" by a container in the sense that each TOC is associated with a
	particular container.  Container "objects" need to be able to get back at the container
	control block that is associated with the TOC containing the object.  Thus the container
	control block is passed here so that we may save it and store it into all created 
	objects.
	
	Internally, i.e., in this file, the "reference number" is actually a pointer to a control
	block containing the root pointer to the TOC index tables (set to NULL here) and the set
	of variables needed to convert an object ID value into a set of indices which are used
	to index into the index tables that lead to the actual TOC object corresponding to the
	ID.  There is also a use count.  The use count is explained in cmUseTOC().
	
	The single parameter is the index table size.  See TOC.c on exactly how this is used.
	But for purposes of description here, the tableSize determines how many index tables
	we indirect through to get at an object and the size of each table (each entry in a
	table is a pointer).  There is a trade-off between space and time here.  So that's why
	the tableSize is a parameter.
	
	This function returns the pointer if we can allocate the control block. NULL is returned
	if the allocation fails.
	*/


void *cmUseTOC(void *toc, struct Container *container);
	/*
	This allows multiple users of the same TOC for the specified container.  This can come
	about due to updating containers which want to use the TOC of their target.  They call
	this routine to register the additional use of the specified TOC.  It is returned as the
	function result but with the owning container of the TOC changed to the one specified.
	Here we register the additional use by incrementing a use count of the TOC.  When
	cmDestroyTOC() is called to remove the TOC it will decrement the use count.  Only when 
	the count goes to 0 is the TOC actually destroyed.
	
	Since the owning container of the TOC is being changed, it is assumed the caller knows
	what s/he is doing!  In particular we only do this for a container updating a target
	and after the updates are applied, we want to use the target's TOC, but associate it with
	the updating container.  This will allow new updates to be recorded in the updating
	container, but in a TOC common to both the updater and the target.
	*/


TOCObjectPtr cmCreateObject(const void *toc, const CMObjectID objectID, 
														CM_USHORT objectFlags, CMBoolean *dup);
	/*
	This routine is called to create a new object in the specified TOC for the specified ID.
	A "raw" object is created and the function returns a pointer to where it was placed in
	the TOC. If there is already an object with that ID the TOC, a pointer to it is returned
	and dup set to true. The function returns NULL if there is an allocation failure so that
	we couldn't create the object.
	
	Note that the object created is "raw" in the sense that all info in it is NULLed out.
	This is the lowest level routine whose sole goal in life is object creation.  The caller 
	is responsible for maintaining the objects.  Life is simpler this way (not to mention
	more maintainable)!
	*/


void cmUncreateObject(const void *toc, const CMObjectID id, CMRefCon refCon,
									  	void (*freeAction)(TOCObjectPtr object, CMRefCon refCon));
	/*
	This routine is the inverse (more-or-less) of cmCreateObject().  It removes an object
	with the specified ID from the specified TOC.  The space for the object is freed.  The
	lowest level index table entry pointing to the object is made NULL to indicate it is
	undefined.  However, the higher level tables leading to the lowest table are not changed.
	This doesn't hurt except to take up space if the object being deleted is the only one in
	the tables leading to it.  But it's not worth all the extra work!
	
	As discussed in cmCreateObject() we simply create a "raw" object and return.  The caller
	builds upon the object with lower level link structures. Unfortunately here, we have to 
	allow the caller to handle his or her stuff BEFORE we delete the object.  Thus a
	freeAction routine may be passed which is called prior to the freeing of the object. A
	"refCon" is also provided which the caller can use as a communication facility to convey
	additional info to the freeAction routine.
 
  Note, the freeAction may be passed as NULL, in which case only the object is deleted and
  it is assumed averything linked to it must have already been deleted.
	*/



void cmDelete1Object(const void *toc, const CMObjectID id, CMRefCon refCon,
									   void (*deleteAction)(TOCObjectPtr object, CMRefCon refCon));
	/*
	This routine is the inverse (more-or-less) of cmCreateObject().  It removes an object
	with the specified ID from the the specified TOC.  Unlike cmUncreateObject() however,
	the space for the object is NOT freed.  Rather the object is RELINKED on to a singly
	linked list of deleted objects pointed to by deletedObjects (a global).  This is done
	to prevent dangling pointers that the API user may have as object "refNum"s (we give 
	the API user these pointers as CMObject's).
	
	Since the object is removed from the TOC it will never be seen by us here using the TOC
	index tables.  Just as in cmUncreateObject() the lowest level index table entry pointing
	to the object is made NULL to indicate it is undefined in the TOC.
	
	Also as in cmUncreateObject() we allow the caller to do something with the object before
	it is deleted from the TOC and before we link it on to the deletedObjects list.  The
	calling conventions are the same and NULL may be passed to indicate no action is to be
	performed.
	
	Caution: the deletedObjects list utilizes the nextObject field of an object to link its
	objects.  That field is also a link field for active objects.  Thus the object must be
	unlinked from the active object chain BEFORE calling this routine. 
	*/
	

TOCObjectPtr cmFindObject(const void *toc, const CMObjectID id);
	/*
 	Given an object ID in a TOC, this function returns a pointer to the corrsponding object
 	or NULL if the object does not exist.
	*/


int cmForEachObject(const void *toc, CMObjectID startingID, CMObjectID endingID, 
									  CMRefCon refCon, void (*action)(TOCObjectPtr object, CMRefCon refCon));
	/*
	Do (call) the specified action for each object in the specified TOC with object ID's
	greater than or equal to the startingID and less than or equal to endingID.  The pointer
	to each TOC object is passed to the action routine along with a "refCon" which the caller
	can use as a communication facility to convey additional info to the action routine.  0
	is returned to indicate successful completion.  Use the AbortForEachObject(x) (a macro)
	in the action routine to abort the interator.  The "x" should be a positive integer which
	is returned from cmForEachObject() and it should not be 0.
	*/


#define AbortForEachObject(x) longjmp(SESSION->cmForEachObjectEnv, x)
	/*
	Use this in routines passed to cmForEachObject() to abort the interator. The value of x
	is returned from cmForEachObject(). It should be positive and nonzero, since 0 is the
	"success" return value for cmForEachObject(). It is assumed that the variable "container"
	is the current container pointer.
	*/


void cmDestroyTOC(void **toc, CMRefCon refCon,  CMBoolean objectToo,
									void (*deleteAction)(TOCObjectPtr object, CMRefCon refCon));
	/*
	This routine is called to remove wipe a TOC out of existence.  All existing objects in
	the TOC, supporting index tables, and deleted objects on the deletedObjects list are
	freed.  The "reference number" must not be used beyond this point.  The TOC pointer is
	NULLed out prior to return to prevent further use.
	
	Note, that anything "below" an object entry is NOT handled here, i.e., all stuff that 
	the object might be pointing to.  The caller MUST provide a way of deleting that stuff.
	To this end, the caller should provide a "deleteAction" routine.  It is called just
	prior to deleting each object.  The pointer to each object is passed to the deleteAction
	routine along with a "refCon" which the caller can use as a communication facility to
	convey additional info to the deleteAction routine.
	
	The objectToo flag permits you to destroy the toc without deleting the objects.
 
	The deleteAction value may be passed as NULL.  If that is the case, then (obviously) no
	call is done and the object is simply deleted.  It's not clear anyone will do such a
	thing, but it's provided anyway!
	
	Since multiple users of the same TOC are permitted (because of updating), a use count is
	maintained of the number of users of the TOC.  Additional users call cmUseTOC() to 
	increment the useCount.  It is decremented here.  Only when the count goes to 0 do is
	the TOC actually destroyed.
	*/


TOCObjectPtr cmLinkObject(const void *toc, TOCObjectPtr theObject, 
													TOCObjectPtr afterThisObject);
	/*
	This routine is (doubly) links an object (theObject) into the master object chains whose
	head and tail pointers are contained in the specified TOC.  The objects is inserted in
	the object chain following afterThisObject. If afterThisObject is NULL the object is
	appended to the end of its list(s).  The function returns the input object pointer as its
	result.
	
	There are three master chains:
	
		 (1).  A chain of property objects.
		 (2).  A chain of type objects.
		 (3).  A chain of ALL objects no matter what their type.  It may be neither a type or
					 property.
					 
	Chain (3) is always built. The decision on whether to do (1) or (2) is a function of the
	objectFlags field contained in the object itself.  So the caller must have set these
	prior to calling this routine.
	*/
	

TOCObjectPtr cmUnlinkObject(const void *toc, TOCObjectPtr theObject);
	/*
  This routine is the inverse to cmLinkObject().  It unlinks the specified object from the
  master object chains whose head and tail pointers are contained in the specified TOC.
  The function returns the input object pointer as its result.
	
	The object is always deleted from the chain of all objects.  It is also deleted from the
	property or type chain if the objectFlags field contained in the object itself indicates
	it is a property or type (see cmLinkObject() above for further details about the chains).
	*/


TOCObjectPtr cmGetMasterListHead(const void *toc, CM_USHORT objectKindFlag);
	/*
	This routine returns the head of one of the master chain lists in a TOC depending on 
	the objectKindFlag.  This flag should be one of the standard object flags; ObjectObject,
	PropertyObject, or TypeObject.  The respective chain head will be returned as the function
	result.
	
	These chain heads are used by the CMGetNextObject(), CMGetNextProperty(), and
	CMGetNextType() routines to start them off.
	*/

TOCObjectPtr cmGetMasterListTail(const void *toc, CM_USHORT objectKindFlag);
	/*
	This routine returns the tail of one of the master chain lists in a TOC depending on 
	the objectKindFlag.  This flag should be one of the standard object flags; ObjectObject,
	PropertyObject, or TypeObject.  The respective chain head will be returned as the function
	result.
	
	These chain tails are used by the CMGetPrevObject(), CMGetPrevProperty(), and
	CMGetPrevType() routines to start them off.
	*/

void cmChangeRefNumHandling(const void *toc, CMBoolean setting);
	/*
	Normally all deleted values and objects are "remembered".  A value's header is placed on
	the deleted values list attacthed to the container control block.  Deleted TOCObject's
	are placed on the deleted object list attached to the toc control block.  Both value
	headers and TOCObject's are the refNums we give to the user.  By remembering these we
	can check for attempted reuse and report an error.
	
	However, there are times we don't want to track these things because we know internally
	what we're doing (well, that's the theory).  So an override switch is provided.  Since
	both kinds of refNums are TOC entities, the override is on a TOC basis and hence the
	switch attached to the TOC control block.
	
	This routine is therefore provided to change the setting the override switch.  It is a
	boolean with true meaning that deleted refNums are to be saved on their respective lists.
	Setting the switch to false will cause the refNum to simply be freed.
	
	By calling cmKeepDeletedRefNums(), the current setting of the switch can be interrogated
	to decide what to do.
 
  Oh, in case you're curious, one case where we suppress the refNum tracking is when we are
  applying updates at open time where updating instructions are being interpreted to 
  update a target container.
	*/


CMBoolean cmKeepDeletedRefNums(const void *toc);
	/*
	As described above, this is used by all outside callers to see if a refNum is to be saved
	on its deleted list.  The only thing outside callers can possibly be interested in is
	for deleted values and whether the value headers are to be saved on the deleted values
	list.
	*/
	
void cmPutObjectInTOC(const void *toc, TOCObjectPtr object, TOCObjectPtr *replacedObject);
	/*
	This routine let put put an object (currently belonging to another TOC) and put it into
	this TOC. If an object of the same ID is already in this TOC, that old object in this
	TOC will be replaced and also returned from the routine so that the caller can deal
	with it.
	
	This routine is used to move objects from one TOC to another.
	*/		
														  CM_END_CFUNCTIONS
#endif
