/* @(#)Z 1.4 com/src/cm/Update.h, odstorage, od96os2, odos29646d 96/11/15 15:27:39 (96/10/29 09:20:06) */
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
	File:		 Update.h 

	Contains:	Container Manager Container Updating Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 9/15/94	EL		#1182275 Add cmClosePastDataGap.
		 <2>	 8/26/94	EL		#1182275 Extra parameter in
													cmGenerateUpdate, new function
													cmPastUpdateOnTouchList.
		 <1>	  2/3/94	EL		first checked in
		 <2>	10/29/93	EL		Add extra parameter to cmDeleteTouchList
													and export cmDeleteObjeFromTouchedChain

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                             <<<  Update.h  >>>                            |
 |                                                                           |
 |             Container Manager Container Updating Interfaces               |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  6/16/92                                  |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file contains the interfaces to the "incremental update" processing routines when
 one updating container updates its target container.  The routines defined here are
 responsible for maintaining the "touched chain" and "touched lists". These are additional
 data structures layered on to the main data structures (see TOCEntries.[hc]).  See
  Update.c  for complete details.
*/


#ifndef __UPDATING__
#define __UPDATING__


#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif

struct Container;
struct TOCObject;
struct TOCProperty;
struct TOCValueHdr;
																	
																	CM_CFUNCTIONS

/* The touched list is created for every updated object. The head of this list is in 		*/
/* the touched TOCObject.  An entry is created the first time a value (header) is 			*/
/* touched.  At that time the original object, property, and type IDs are recorded.			*/

struct TouchedListEntry {										/* Layout of a touched list entry:					*/
	ListLinks								touchedListLinks;	/*		links to next/prev entry (must be 1st)*/
	struct TOCValueHdr 			*theValueHdr;			/*	 	ptr to value hdr that was touched			*/
	CMObjectID				 			objectID;					/*		value's original object ID						*/
	CMObjectID				 			propertyID;				/*		  "        "     property ID					*/
	CMObjectID				 			typeID;						/*		  "        "     type ID							*/
	unsigned short		 			touchFlags;				/*		touched list entry flags							*/
	struct TouchedListEntry *removedEntry;		/*		back ptr to "removed" entry						*/
};
typedef struct TouchedListEntry TouchedListEntry, *TouchedListEntryPtr;


/* The following touchFlags indicate what was done to the value pointed to by the				*/
/* touched list entry...																																*/

#define TouchedRemoved					0x0001			/* touchFlags: 1 ==> value moved out				*/
#define TouchedInserted					0x0002			/*							 ==> value moved in					*/
#define TouchedDeletedValue			0x0004			/*							 ==> value was deleted			*/
#define TouchedDeletedProperty	0x0008			/*							 ==> property was deleted		*/
#define TouchedEdited						0x0010			/*							 ==> editing								*/
#define TouchedImmediate				0x0020			/*						 	 ==> immediate modified			*/
#define TouchedBaseType					0x0040			/*							 ==> base type modified			*/
#define TouchedSetinfoed				0x0080			/*						 	 ==> set-infoed							*/

#define TouchedModified	(TouchedEdited 		| /* These are the basic value modifications	*/\
												 TouchedSetinfoed | 																							\
												 TouchedImmediate	| 																							\
												 TouchedBaseType)

void cmDeleteObjFromTouchedChain(ContainerPtr container,
																							TOCObjectPtr theObject,
																							TOCObjectPtr thePrevObject);
																							
void cmAddObjToTouchedChain(struct Container *container, struct TOCObject *theObject);
	/*
	The TOCObject pointed to by theObject is "touched", i.e., it is put on the updating
	container's singly linked chain of touched objects, the touchedChain.
	
	The container is passed because the touched list has to be attached to a "know fixed"
	container.  The fixed container is usually the updatingContainer.  But this needs to be
	"controlled" during open time because the updatingContainer is also used define the
	"owner" of NEW objects read in during TOC loading.  The updatingContainer then is the
	container doing the loading, NOT the original container the user opened.
 
	cmReadTOC(), who loads the TOC, calls this routine to add objects to the touched chain
	that have the special "updates" property.  Such objects have associated updating
	instructions (generated by generateValueUpdates() at close time). These must be chained
	together until the TOC is fully read in.  This is because "inserted" (i.e., move)
	updates could refer to objects not yet  read in.  The touched chain is a convenient
	place!  So we use it and this routine to build the chain.  This chain is only temporary.
	It is "fed" to cmApplyUpdates() who walks the chain to apply all the updates. The chain
	is then immediately cleared ready to receive new updates in the "normal" way.
	
	Note, the object is placed on the touched chain only once.  This routine is, however,
	called whenever we think we need to touch the object.  We worry about the multiple
	touches here.
	*/
	
	
void cmDeleteTouchedList(struct TOCObject *theObject, ContainerPtr container);
	/*
	This routine is only used when all TOC data structures are being freed.  For each object
	passed, that object's entire touched list (if any) is freed.  Since all TOC entities are
	being freed, there is no need to remove the object from the touched chain since the
	objects on the chain will be freed.
	
	Note, that the object's passed are just objects picked up during the freeing of all the
	objects.  Therefore the touched chain is not being followed in the context used here.
	*/
	
	
CMBoolean cmTouchEditedValue(struct TOCValueHdr *theValueHdr);
	/*
	This routine defines a touched list entry for a value when it is edited.  A touched list
	entry for the object "owning" the value is defined.  The parameter specifies the value
	header being edited. The function returns true if everything went ok and false otherwise.
	This can only fail because of an allocation error creating a touched list entry.
	*/
	
	
CMBoolean cmTouchImmediateValue(struct TOCValueHdr *theValueHdr);
	/*
	This routine defines a touched list entry for an immediate value when it is edited.  A
	touched list entry for the object "owning" the value is defined.  The parameter specifies
	the value header of the immediate value being edited.  The function returns true if
	everything went ok and false otherwise. This can only fail because of an allocation error
	creating a touched list entry.
	*/


CMBoolean cmTouchBaseType(struct TOCValueHdr *theValueHdr);
	/*
	This routine defines a touched list entry for an base type when it is changed.  A touched
	list entry for the object "owning" the base type (value) is defined.  The parameter
	specifies the value header of the base type value being changed.  The function returns
	true if everything went ok and false otherwise.  This can only fail because of an
	allocation error creating a touched list entry.
	*/

	
CMBoolean cmTouchSetInfoedValue(struct TOCValueHdr *theValueHdr);
	/*
	This routine defines a touched list entry for an value when its type or generation number
	is changed (i.e., it's "set-infoed").  A touched list entry for the object "owning" the
	value is defined. The parameter specifies the value header of the value being set-infoed.
	The function returns true if everything went ok and false otherwise.  This can only fail
	because of an allocation error creating a touched list entry.
 
  Warning: The call to this routine MUST be done before any changes to the type because the
 					 touch list info is in terms of the original object, property, and type.
	*/

	
CMBoolean cmTouchMovedValue(struct TOCValueHdr *theFromValueHdr,
												  	struct TOCObject *theFromObject,
														struct TOCObject *theToObject,
														CMObjectID toPropertyID);
	/*
	This routine defines touched list entries for a value move when a CMMoveValue() is done.
	Touched lists entries for the "from" and "to" (source and destination) objects involved
	in the moved value are defined.  The parameters specify the value header to be moved
	(theFromValueHdr), the source ("from") TOCObject (theFromObject), the destination ("to")
	TOCObject (theToObject), and destination property ID (toPropertyID).  The function
	returns true if everything went ok and false otherwise.  This can only fail because of an
	allocation error creating a touched list entry.
 
  Warning: The call to this routine MUST be done before the value is moved because the
 					 touch list info is in terms of the original object, property, and type.  The
					 move will obviously change the object and/or property.
	*/


CMBoolean cmTouchDeletedValue(struct TOCValueHdr *theValueHdr, struct TOCObject *theObject);
	/*
	This routine defines a touched list entry for a value when an explicit CMDeleteValue() is
	done.  A touched list entry for the object "owning" a value that is (to be) deleted is
	defined.  The parameters specify the value header to be deleted (theValueHdr) and the
	value's TOCObject (theObject).  The function returns true if everything went ok and false
	otherwise.  This can only fail because of an allocation error creating a touched list
	entry.
 
	Values are deleted individually and explicitly via CMDeleteValue(), or implicitly by
	implicit deletion of the value's property.  An implicit property deletion comes about
	when all the values of a property are explicitly deleted or moved out, or if the object
	itself is deleted.  Implicit value deletions are handled cmImplicitDeleteValueTouch() 
	specially as descriped there.
	*/


void cmImplicitDeleteValueTouch(struct TOCValueHdr *theValueHdr, struct TOCObject *theObject);
	/*
	This routine is called whenever a value is IMPLICITLY deleted because an object or a
	property is deleted.  In that case, all the values for the object, or one of the object's
	property's are deleted.  We also must delete the the corresponding touched list entry
	if there is one for those values since there isn't going to be any value there.  However,
	if a value was moved in from another object, then the "removed" entry in the original
	value is flagged as "deleted value".
	*/


CMBoolean cmTouchDeletedProperty(struct TOCProperty *theProperty,
															 struct TOCObject *theObject);
	/*
	This routine defines a touched list entry for a property when the property is explicitly
	deleted by CMDeleteObjectProperty().  A touched list entry for the object "owning" the
	property that has been deleted is defined. The parameters specify the property of the
	object to be deleted (theProperty) and the property's TOCObject (theObject). The function
	returns true if everything went ok and false otherwise.  This can only fail because of an
	allocation error creating a touched list entry.
	
	Note, this routine is slightly different from touches on values in that no value is
	involved explicitly in the property delete.  They are, however, implicitly involved to 
	the degree that all values are deleted from the property (and the value headers put on
	the deleted values list).  The process of value deletion deletes all the touched list
	entries for those values in the property's object.  Any values moved into this object
	from other objects also have a "removed" touched list entry in the original object.
	Deleting such values for the property has exactly the same effect as CMDeleteValue(),
	i.e., the "removed" entry is changed to a "deleted value" entry.  See
	cmTouchDeletedValue() for further details.
	
	Note, that values previous moved OUT of the object containing the property being deleted 
	have "removed" entries of their own in this object.  Thus, there still could be "dangling"
	"removed" entries on this object's touched list.  These must remain in the normal course
	of move handling.  However, see what happens to these things when the entire object
	is deleted!  Read cmTouchDeletedObject() for details.
	*/
	
	
void cmTouchDeletedObject(struct TOCObject *theObject);
	/*
	This routine is called whenever an object is deleted. The object is placed on the touched
	chain if it is not already there.  The object's touched chain, if any, is deleted EXCEPT
	for "removed" entries for values previously moved out of this object.  
	
	The "removed" entries are kept because there are references to these entries from 
	"inserted" entries in other object.  If a moved value is itself deleted after the object
	it came from is deleted, then the back pointer in its "inserted" entry must be kept valid
	so that the "removed" entry it points to can be changed to a "deleted value".  Although
	this "deleted" value is on a touched list of a deleted object, it won't cause harm since
	the final walks of the touched chain know to expect this.
	
	Deleted objects are moved to the deleted objects list and flagged as deleted.  This is
	done to protect against the user reusing the refNum after the delete.  Thus an object on
	the touched chain, but flagged as deleted, is enough to indicated to close-time
	processing what is going on.  The remaining touched list "goes along for the ride".
	
	This routine MUST be called AFTER all objects and properties for the object have been
	deleted.  By that time all touched list entries for values will have been processed
	(using cmImplicitDeleteValueTouch()).   The reason it must be after is that if it were
	before, we could have touched value headers pointing to touched list entries we free up
	here.  Not too cool!  cmImplicitDeleteValueTouch() would have a hard time dealing with
	that.
	*/

void cmPastUpdateOnTouchList(ContainerPtr container);
	/*
	Make sure that all the previous update are put on to the touch list so that they can be
	included in the generation in the new update
	*/
	
CMBoolean cmGenerateUpdates(struct Container *container, struct Container *targetContainer);
	/*
	This is called at close time (CMCloseContainer()) to generate all the updating
	instructions for all objects and their values touched during an updating session. True is
	returned if successful and false otherwise (and the error reporter returns).
	
	The touched objects are on the touched chain pointed to brom the container control block.
	Two passes are made over the touched chain:
	
		 Pass 1: All touched values are processed.  The touched list for each non-deleted 
						 object that is still on the touched chain (see pass 2) are processed for
						 touched values.  Touched properties are skipped.  
						 
						 A special "updates" property is generated for each object involved which 
						 contains a value whose data is all the value updates for that object.  Each
						 processed touched value's touched list entry is deleted.
						 
						 The end of pass 1 leaves only deleted property touched list entries in 
						 non-deleted objects on the touched chain and touched, but deleted, objects.
						 
		 Pass 2: All touched deleted objects and deleted properties.  This is the remaining
						 stuff on the touched chain and lists.  The data for this is attached to a
						 special TOC #1 property.  The remaining touched list entries are deleted. 
						 Thus, at the end of pass 2, all touched list space is freed up.
	*/
	
	
CMBoolean cmApplyUpdates(struct Container *container);
	/*
	This is called at open time (openTargetContainer()) to apply (execute) all the updating
	instructions for all objects and their values "touched" while loading in the non-private
	portion of an updating container's TOC.  True is returned if successful and false
	otherwise (and the error reporter returns).
	
	The touched objects are on the touched chain pointed to from the passed container control
	block.  These were objects containing a special "updates" property.  Such a property has
	a value whose value data is all the updating instructions dealing with that object.  The
	reason they were chained was so that we could get at the objects with that property and
	do it at a time when we know we have all the objects read in.  During the read there could
	be references in the instructions for as-yet unread objects.
	
	To make it easire (and more efficient) to get at the "updates" property value for the
	objects on the touched chain, such objects have had their refCon set with the refNum
	(pointer) of the updating instruction value header.  That was done at the same time the
	object was placed on the touched chain.  We'll clear it to NULL as we process those
	objects.
	
	This routine is more-or-less the symmetric counterpart to cmGenerateUpdates() above,
	which generated the instructions we're about to read here.  That generation is done in 
	two passes to generate two groups of updates; first the value updates, and then the 
	object/property deletions.  The value updates are the instructions attached to each
	object and that is what caused the touched chain to be built that we are going to process
	here.  The deletions are attached to a special TOC #1 "deletes" property.  We process
	those AFTER the value updates are processed (more about this below).
	
	Thus, rather than two passes over the touched chain that we do in generating the updates,
	we have two phases here:
	
		 Phase 1: All updating instructions are processed for each object on the touched chain.
							This will update all the values appropriately.  Since the updating
							instructions are in terms of the original "OPT addresses" of the values we
							know how to find them.  Moves can be made.
							
							Each object processed is removed from the touched chain.  Thus at the end of
							phase 1, the touched chain is empty and ready to receive REAL update touches.
						 
		 Phase 2: The "deletes" TOC #1 property is used to get the deletion updates list.  All
							object and property deletes are done at this time.  This must be done last
							in order for phase 1 to work correctly. The problem is with moves interacting
							with deletes.  If a value is moved from A to B, and then A (its original
							location) deleted, then the updates to that value would not have A to refer 
							to. We always need the ORIGINAL OPT addresses of the values.
	*/

void cmClosePastDataGap(ContainerPtr container, CM_ULONG offset, CM_ULONG size);

/*------------------------------------------------------------*
 | cmClosePastDataGap - adjust the offset in the past history |
 *------------------------------------------------------------*

	When we return free space from a embedded container to the parent container, the
	offset of the values will be changed. This affects not only the offset on the TOC.
	It also affects the offset in the update objects. So we need to be able to update
	the offset in the past history to reflect the changes in the various offset.
*/

														  CM_END_CFUNCTIONS
#endif
