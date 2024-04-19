/* @(#)Z 1.6 com/src/cm/TOCEnts.c, odstorage, od96os2, odos29646d 96/11/15 15:27:16 (96/10/29 09:19:26) */
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
	File:		 TOCEnts.c  

	Contains:	Container Manager TOC Entry Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1993 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 7/28/95	EL		#1272079: Value should not use the continue
									flag until being written to TOC.
		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <3>	  6/3/94	EL		Rename Values.h/c to ValueRtn.h/c.
		 <2>	 3/31/94	EL		Export cmFreeProperties. #1150214
		 <1>	  2/3/94	EL		first checked in
		 <4>	  2/2/94	EL		Fix memory leak by freeing property and
													value associated with a reference.
		 <3>	10/29/93	EL		Add deleteAssocatedRef call which would
													delete a reference associated with a value
													without leaving it on the delete list
		 <2>	10/21/93	EL		ValueHdr should not point to deleted
													property
	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<<  TOCEnts.c   >>>                           |
 |                                                                           |
 |                    Container Manager TOC Entry Routines                   |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/02/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1992                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file contains the manipulation routines to handle TOC objects below the object
 level.  The objects and the means to get at them are maintained in TOCObjects.c.  The
 split is done this way to keep the accessing of objects separate from the stuff 
 represented by those objects.
 
 The following diagram shows the pointer relationships among the various data structures
 descending from an object.  The data structurs leading to the object itself are handled
 independently by the  TOCObjs.c   routines.  They are of no concern here as the structures
 described here are of no concern to  TOCObjs.c  !
 
 Arrows of the form "<---->" represent double links of forward/backward link lists. Arrows
 of the form "--->" are pointers in the indicated direction.  A "+" in an arrow just means
 it's turning or intersecting since there is no other means to notate it given the
 character set used!
 
 A name above a box corresponds to the actual typedef struct name.  A "*name*" just
 signifies what the structure is and destinguishes it from other similar structures in the
 diagram.  Names in brackets are fields in the structures.  The fields representing the
 list links and headers are not shown.
 
 
                        +<-----+<--------------------+<--------------------+
                        |      |                     |                     |
      TOCObject         |      | TOCProperty         |                     |
 *------------------*<--+     *--------------*      *--------------*      *--------------*
 |     *Object*     |<------->|  *Property1* |<---->|  *Property2* |<---->|  *Property3* |
 |   [objectID]     | +>+---->| [propertyID] |      | [propertyID] |      | [propertyID] |
 |   [container]    | | |     *--------------*      *--------------*<-+   *--------------*<-+
 |   [nextObject]   | | |                                             |                     |
 |[nextTypeProperty]| | |                                             |                     |
 |  [objectFlags]   | | |                                             +->...                +->...
 *------------------* | |
                      | |       TOCValueHdr          TOCValue
                      | |    *--------------*      *----------*
                      | +--->|  *ValueHdr1* |<---->| *Value1* |
 The nextObject       +<-----|   [typeID]   |      | [flags]  |
 links ALL objects.   |      | [container]  |      | [value]  |
                      |      | [valueFlags] |      *----------*
                      |      | [generation] |           |
                      | +--->|    [size]    |           |
 The nextTypeProperty | |    *--------------*<----------+
 is used for two      | |
 separate chains;     | |
 one that links all   | |
 property objects,    | |    *--------------*      *-----------*      *-----------*
 and one that links   | +--->|  *ValueHdr2* |<---->| *Value21* |<---->| *Value22* |
 all type objects.    +<-----|   [typeID]   |      |  [flags]  |      |  [flags]  |
                      |      | [container]  |      |  [value]  |      |  [value]  |
                      |      | [valueFlags] |      *-----------*      *-----------*
                      |      | [generation] |            |                  |
                      | +--->|    [size]    |            |                  |
                      | |    *--------------*<-----------+<-----------------+   
                      | |                                     (continued)
                      | |
                      | |
                      | |
                      | |    *--------------*      *-----------*    Note, all value
                      | +--->|  *ValueHdr3* |<---->|  *Value3* |    entries have a ptr
                      +<-----|   [typeID]   |      |  [flags]  |    to their "owning"
                             | [container]  |      |  [value]  |    ValueHdr to be able
                             | [valueFlags] |      *-----------*    to get at the TypeID
                             | [generation] |            |          and container.
                             |    [size]    |            |
                             *--------------*<-----------+
 
 
 The above diagram shows a single object with three properties. Properties are chained off
 the object. Only the layout for the first property is shown.  Propert1 has three values.
 Values are chained off a value header and the value headers are chained off the properties.
 Value2 represents a continued value. They are chained together, and each value entry
 (continued or not) has a back pointer to its value header.  Similarly, the value headers
 have back pointers to their property and the properties back to their object.  This
 allows us to get at the owning container and list headers.
 
 Not shown in this example diagram is the handling of global names. They are contained in
 a binary tree symbol table.  Each entry in that symbol table contains the symbol itself
 and a back pointer to its corresponding value entry for which the global name represents
 the value.  The value, in turn, points to its global name symbol table entry.  This
 global name layout allows us to keep the names in memory for mapping of global names
 to objects or vice versa.  By keeping the global names on a binary tree we can
 efficiently walk the tree to write the value out to the container. This is more efficient
 than the alternative, which is walking the entire TOC structure looking only for the
 global name value pointers to just write them out.
 
 Note, the API permits deletion of objecs and values.  This is implemented by two separate
 deletion lists.  There is a list of all deleted TOCObject's and another list of all
 deleted TOCValueHdr's.  Pointers to these are returned to the API user as "refNums". Thus
 these are the only entities that we must never free in an open container.  All the other
 stuff, including TOCValue's off the TOCValueHdr's, can be freed when a delete is done.
 The reason we keep this stuff around at all is to protect ourselves against the silly
 user passing a "refNum" to a deleted object back to us!  The "refNum"s remain valid
 pointers.  When a TOCObject or TOCValueHdr is put on its list, it is flagged as deleted.
 Wenever a API caller passes in a "refNum" we check it to make sure it's still valid and
 yell if it isn't.  It's a lot of machinery for this.  But do you have a better idea?
 
 
            ---> Help for those looking at this file for the firtst time <---
 
 The place to start looking is at the four routines cmAppendValue(), defineValue(),
 defineProperty(), and cmDefineObject().  They are the main routines that implement the
 above structures. Understand them and the rest should fall out.  That's how this file was
 developed and all other stuff "spun out" from there.
*/


#include <stddef.h>
#include <string.h>
#include <setjmp.h>
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
#ifndef __VALUEROUTINES__
#include "ValueRtn.h"       
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __UPDATING__
#include "Update.h"  
#endif
#ifndef __DYNAMICVALUES__
#include "DynValus.h"     
#endif
#ifndef __BUFFEREDIO__
#include "BufferIO.h"  
#endif
#ifndef __REFERENCES__
#include "Refs.h"      
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
#pragma segment TOCEntries
#endif


#ifndef DEBUG_TOCENTRIES
#define DEBUG_TOCENTRIES 0
#endif

#define dbgFile stdout					/* trace display output file														*/

/* The following is used in this file by cmWalkThroughEntireTOC(), etc. to hold the 		*/
/* function pointers, caller's refCon, and setjmp/longjmp escape vector.  See that 			*/
/* routine for further details.																													*/

struct TOCActions {							/* cmWalkThroughEntireTOC() communication area layout:	*/
		jmp_buf walkThroughEnv;			/* 		MUST be 1st for AbortWalkThroughEntireTOC() macro */
		CMRefCon refCon;						/* 		caller's refCon																	 	*/
		TOCWalkReturns (*objectAction)(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon);
		TOCWalkReturns (*propertyAction)(ContainerPtr container, TOCPropertyPtr theProperty, CMRefCon refCon);
		TOCWalkReturns (*valueHdrAction)(ContainerPtr container, TOCValueHdrPtr theValueHdr, CMRefCon refCon);
		TOCWalkReturns (*valueAction)(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon);
};
typedef struct TOCActions TOCActions, *TOCActionsPtr;

static void deleteProperties(TOCObjectPtr theObject, CMRefCon refCon);

/*---------------------------------------------------------------------------*
 | deleteAssociatedRef - remove the reference associated with this reference |
 *---------------------------------------------------------------------------*
 
 This routine checks to see if there is any reference assoicated with a value, if there
 is one, we also delete the reference.
 
 Since a reference is delete as a side effect, if the delete reference remains in the
 delete list and generate a update delete, then when the update is play back, the
 reference is deleted twice, once as the side effect and once as directed by the updating
 instruction. So we want to make sure the reference is completely gone so it would not
 be used to generate a delete update later.
*/

static void CM_NEAR deleteAssociatedRef(ContainerPtr container, TOCValueHdrPtr theValueHdr)
{
	TOCObjectPtr	theObject;
	void 					*toc;
	CMBoolean				orgSetting;
	
	if (HasRefDataObject(theValueHdr))	{												/* delete refs object...	*/
		theObject = RefDataObject(theValueHdr);
		toc = container->toc;
		theObject->objectFlags &= ~ProtectedObject;
		cmUnlinkObject(toc, theObject);														/* must do this first			*/
		orgSetting = cmKeepDeletedRefNums(toc);
		/* since the reference is deleted as a side effect, we don't want it to show up in 	*/
		/* any delete list or touch list, so we going to free it directly, and not put it 	*/
		/* on the delete or touch list. We do this by setting RefNumHandling to false.			*/
		cmChangeRefNumHandling(toc, false);												/* allows freeing					*/
		cmDelete1Object(toc, theObject->objectID, (void *)container, deleteProperties);
		cmChangeRefNumHandling(toc, orgSetting);									/* restore original	value */
	}
	
	#if CMSHADOW_LIST
	if (HasRefShadowList(theValueHdr))													/* delete refs shadow list*/
		cmDeleteRefDataShadowList(theValueHdr);
	#endif
}

/*--------------------------------------------------------------*
 | cmMarkValueDeleted - remove a value for an object's property |
 *--------------------------------------------------------------*
 
 This routine takes a pointer to a value header in the specified container and deletes it.
 If the value was the only one for its property, the property is also deleted.
 
 Note, properties and values actually have their memory freed up.  The value headers are
 placed on the deletedValues list to keep them around, but out of the TOC.  This is to
 prevent dangling references from API user "refNum"s, because it is value header pointers
 that we give to the user as those "refNum"s.  Such value headers are marked as "deleted"
 to prevent future use by the user.
 
 The moving of the deleted value headers to the deletedValues list can be suppressed by 
 passing the deleteHdr parameter as true.  Its memory is simply freed along with the rest.
 This is used currently in only one place for error recovery when dynamic value setup
 fails.  There we want to free all the dynamic value layers (which are headers), and
 finally the base "real" value itself.  It is that final value that is passed to here to
 get rid of.
*/

void cmMarkValueDeleted(ContainerPtr container, TOCValueHdrPtr theValueHdr, 
												CMBoolean deleteHdr)
{
	TOCValuePtr		 nextValue, theValue;
	TOCPropertyPtr theProperty;
	
	/* Free all values for the value header...																						*/
	
	theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	while (theValue) {
		nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
		cmAddToFreeList(container, theValue, 0, 0);						/* add space to free list			*/
		if (theValue->flags & kCMGlobalName)									/* mark global name as deleted*/
			if (theValue->value.globalName.globalNameSymbol) 
				MarkGlobalNameDeleted(theValue->value.globalName.globalNameSymbol);
		CMfree(theValue);
		theValue = nextValue;
	} /* value */
	
	/* Mark the value header as deleted and put it on the separate chain of deleted 			*/
	/* values.  Also keep track of total space deleted.																		*/
	
	#if 0 /* now done in cmAddToFreeList() */
	container->spaceDeletedValue->value.imm.ulongValue += theValueHdr->size;
	#endif
	
	theProperty = theValueHdr->theProperty;											/* prepare to remove hdr	*/
	cmDeleteListCell(&theProperty->valueHdrList, theValueHdr);	/* del. valueHdr off list	*/
	
	deleteAssociatedRef(container, theValueHdr);								/* delete any reference		*/
	
	if (deleteHdr || !cmKeepDeletedRefNums(container->toc)) {		/* free the refNum?				*/	
		CMfree(theValueHdr);																			/* this caller is serious!*/
		theValueHdr = NULL;																				/* so we know it's gone 	*/
	} else {																											/* this is normal case...	*/
		cmInitList(&theValueHdr->valueList);											/* no more values					*/
		theValueHdr->valueFlags |= ValueDeleted;									/* mark hdr as deleted		*/
		cmAppendListCell(&container->deletedValues, theValueHdr);	/* move to deletedValues 	*/
	}
	
	/* If the value is the only one for a property, the property itself is deleted...			*/
	
	if (cmIsEmptyList(&theProperty->valueHdrList)) {
		CMfree(cmDeleteListCell(&theProperty->theObject->propertyList, theProperty));
		if (theValueHdr)
			theValueHdr->theProperty = NULL;												/* so we won't access it 	*/
	}
}


/*-----------------------------------------------------------------------*
 | cmDeleteProperty - remove a property and all its values for an object |
 *-----------------------------------------------------------------------*
 
 This routine takes a pointer to a property in the specified container and deletes it and
 all its values.
 
 Note, properties and values actually have their memory freed up.  The value headers are
 placed on the deletedValues list to keep them around, but out of the TOC.  This is to
 prevent dangling references from API user "refNum"s, because it is value header pointers
 that we give to the user as those "refNum"s.  Such value headers are marked as "deleted"
 to prevent future use by the user.
 
 This routine differs from deleteProperties() below by the fact that this is an external
 routine for deleting one property for an object. It is used as an isolated and explicit 
 delete of a property.  deleteProperties(), on the other hand, is for deleting all the
 properties of an object when we know we're deleting the object itself.  It is the action
 routine for the isolated delete of an object.
*/

void cmDeleteProperty(ContainerPtr container, TOCPropertyPtr theProperty)
{
	TOCValueHdrPtr theValueHdr, nextValueHdr;
	TOCValuePtr		 theValue, nextValue;
	TOCObjectPtr 	 theObject;
	
	/* Mark all the values for the property as deleted...																	*/
	
	theObject 	= theProperty->theObject;
	theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
	
	while (theValueHdr) {
		nextValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
		
		theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		while (theValue) {
			nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
			cmAddToFreeList(container, theValue, 0, 0);					/* add space to free list			*/
			if (theValue->flags & kCMGlobalName)								/* mark global name as deleted*/
				if (theValue->value.globalName.globalNameSymbol) 
					MarkGlobalNameDeleted(theValue->value.globalName.globalNameSymbol);
			CMfree(theValue);
			theValue = nextValue;
		} /* value */
		
		#if 0 /* now done in cmAddToFreeList() */
		container->spaceDeletedValue->value.imm.ulongValue += theValueHdr->size;
		#endif
		
		deleteAssociatedRef(container, theValueHdr);							/* delete any reference		*/
		
		if (cmKeepDeletedRefNums(container->toc)) { 							/* keep refNums ?				 	*/
			cmInitList(&theValueHdr->valueList);										/* no more values					*/
			theValueHdr->valueFlags |= ValueDeleted;								/* mark valueHdr deleted	*/
			cmAppendListCell(&container->deletedValues,theValueHdr);/* move to deletedValues 	*/
			cmImplicitDeleteValueTouch(theValueHdr, theObject);			/* check touched updates	*/
		} else																										/* don't keep refNums			*/
			CMfree(theValueHdr);
		
		theValueHdr = nextValueHdr;														/* around and around we go...	*/
	} /* valueHdr */
	
	/* Free the property itself since it now has no values...															*/
	
	CMfree(cmDeleteListCell(&theObject->propertyList, theProperty));
}


/*---------------------------------------------------------------------------------*
 | deleteProperties - delete ALL the properties and and their values for an object |
 *---------------------------------------------------------------------------------*
 
 This routine is an action routine for cmDelete1Object() initiated by cmMarkObjectDeleted()
 below to free up all the memory for values and properties for an object.  The value
 headers are marked as deleted and move to the deletedValues list.  This makes sure there
 are no dangling value "refCon"s (which are value header pointers) laying around.  Such
 value headers are flagged as deleted to prevent their use (assuming the checks are put
 in, of course).
 
 Note, this routine differs from cmDeleteProperty() in that here we delete ALL the
 properties for an object.  Not just a single property.  Also, unlike cmDeleteProperty(),
 we know we are being called because an object is being deleted.  We thus do no try to
 delete the object because all its properties are deleted as cmDeleteProperty() does.
 
 Also note, this "static" is intentionally left to default memory model under DOS since it
 is passed as a function pointer to cmDelete1Object().
*/

static void deleteProperties(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container = (ContainerPtr)refCon;
	TOCPropertyPtr theProperty, nextProperty;
	TOCValueHdrPtr theValueHdr, nextValueHdr;
	TOCValuePtr		 theValue, nextValue;
	
	/* Free all the properties for this object...																					*/
	
	theProperty = (TOCPropertyPtr)cmGetListHead(&theObject->propertyList);
	while (theProperty) {
		nextProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
	
		/* Mark all the value headers for the property as deleted and place them on a 			*/
		/* separate list.  The values themselves are freed.																	*/
		
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		while (theValueHdr) {
			nextValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
			
			theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
			while (theValue) {
				nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
				cmAddToFreeList(container, theValue, 0, 0);				/* add space to free list			*/
				if (theValue->flags & kCMGlobalName)							/* mark global name as deleted*/
					if (theValue->value.globalName.globalNameSymbol) 
						MarkGlobalNameDeleted(theValue->value.globalName.globalNameSymbol);
				CMfree(theValue);
				theValue = nextValue;
			} /* value */
			
			/*CMfree(theValueHdr);*/														/* no longer freed						*/
			
			#if 0 /* now done in cmAddToFreeList() */
			container->spaceDeletedValue->value.imm.ulongValue += theValueHdr->size;
			#endif
			
			deleteAssociatedRef(container, theValueHdr);							/* delete any reference	*/
			
			if (cmKeepDeletedRefNums(container->toc)) {								/* keep refNums ?				*/
				cmInitList(&theValueHdr->valueList);										/* no more values				*/
				theValueHdr->valueFlags |= ValueDeleted;								/* mark valueHdr deleted*/
				cmAppendListCell(&container->deletedValues,theValueHdr);/* move to deletedValues*/
				cmImplicitDeleteValueTouch(theValueHdr, theObject);			/* check touched updates*/
			} else																										/* don't keep refNums		*/
				CMfree(theValueHdr);
				
			theValueHdr = nextValueHdr;													/* around and around we go...	*/
		} /* valueHdr */
		
		CMfree(theProperty);																	/* property not needed				*/	
		theProperty = nextProperty;
	} /* property */
	
	cmInitList(&theObject->propertyList);										/* no properties for object		*/
}


/*-------------------------------------------------*
 | cmMarkObjectDeleted - mark an object as deleted |
 *-------------------------------------------------*
 
 This routine is called to remove an entire object from the TOC for the specified
 container.  All its associated structures (properties and values) are made unavailable
 from the TOC.  The space for the object is NOT freed. The object is placed on a list of
 deleted objects and marked as deleted.  By doing it this way, any outstanding "refNum"s
 we gave the API user (which are pointers to this stuff) will still be "valid" (sort of
 -- at least not looking at garbage).
 
 Moving the object structure takes all its substructures (properties, value headers, and
 values with it).  But the API user may also have references to value headers!  So we have
 to mark these as deleted as well (we can still, however, free the memory for the value
 lists attached to the value headers and the properties which contain the value headers).
 
 The object deletion (and actual marking) is handled by cmDelete1Object().  It takes an
 action routine to walk an object's properties.  We pass it deleteProperties() to do that.
 It is there that we delete all the properties and values and move the value headers to
 there deletedValues list.
*/

void cmMarkObjectDeleted(ContainerPtr container, TOCObjectPtr theObject)
{
	/* Here we use cmDelete1Object() which removes an object by object id.  It takes a		*/
	/* function pointer (markValuesDeleted here) to delete the stuff attached to the 			*/
	/* object prior to deleting the object itself. Note, we must also remember to unlink	*/
	/* the object from the master chains.  We do this first since cmDelete1Object() will 	*/
	/* use the same link fields to relink the object to the list of deleted objects.			*/
	
	cmUnlinkObject(container->toc, theObject);							/* must do this first					*/
	cmDelete1Object(container->toc, theObject->objectID, (void *)container, deleteProperties);
}


/*-------------------------------------------------------------------------------*
 | cmFreeProperties - free ALL the properties and and their values for an object |
 *-------------------------------------------------------------------------------*
 
 This routine is called to unconditionally free all the properties for an
 objects and the values and other data structures associated with those properties.  It is
 done when an TOC is deleted, i.e, for cmFreeTOC().
 
 Note, here we can just free the properties and associated data structures.  We don't have
 to respect the fact that they are on a list whose header belongs to the the object.  That
 is because we know we are going to delete the object too.
 
 This routine differs from deleteProperties(), and cmDeleteProperty() in that we just free
 the data.  There is no list maintenance and we do not have to worry about moving deleted
 objects or value headers to their delete lists.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmDestroyTOC().
*/

void cmFreeProperties(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container = (ContainerPtr)refCon;
	TOCPropertyPtr theProperty, nextProperty, theDynProperty = NULL;
	TOCValueHdrPtr theValueHdr, nextValueHdr;
	TOCValuePtr		 theValue, nextValue;
	
	/* Free this object's touched list (if any)...																				*/
	
	cmDeleteTouchedList(theObject, container);
	
	/* Free all the properties for this object...																					*/
	
	theProperty = (TOCPropertyPtr)cmGetListHead(&theObject->propertyList);
	while (theProperty) {
		nextProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
		
		/* Free all the values and their headers for a property. If a "real" value is for a */
		/* dynamic value, the associated dynamic value layers are also freed.  This should 	*/
		/* end up freeing ALL the dynamic values for this object, because we visit all the 	*/
		/* real values in the object.  This will leave the dynamic value property with no		*/
		/* dynamic values on it.  But we have to be careful to not free the dynamic value		*/
		/* property until we have looked at all the values for the object.  Thus we remember*/
		/* where the property is and delete it last.																				*/
		
		if (theProperty->propertyID == CM_StdObjID_DynamicValues) { /* skip dyn value prop.	*/
			theDynProperty = theProperty;															/* but remember it			*/
			theProperty 	 = nextProperty;
			continue;
		}
		
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		
		if (theValueHdr->dynValueData.dynValue != NULL)					/* free dynamic values...		*/
			cmDeleteAllDynamicValueLayers(theValueHdr->dynValueData.dynValue, false);
		
		while (theValueHdr) {
			nextValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
			
			theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
			while (theValue) {
				nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
				
				#if 0																								/* table already deleted!		*/
				if (theValue->flags & kCMGlobalName)								
					if (theValue->value.globalName.globalNameSymbol) 
						MarkGlobalNameDeleted(theValue->value.globalName.globalNameSymbol);
				#endif
				
				CMfree(theValue);
				theValue = nextValue;
			} /* value */

			#if CMSHADOW_LIST
			if (HasRefShadowList(theValueHdr))										/* delete refs shadow list	*/
				cmDeleteRefDataShadowList(theValueHdr);
			#endif
			
			CMfree(theValueHdr);
			theValueHdr = nextValueHdr;
		} /* valueHdr */
		
		CMfree(theProperty);
		theProperty = nextProperty;
	} /* property */
	
	/* If there was a dynamic value property, then by this time all of its dynamic values	*/
	/* have been deleted.  All that's left is to remove the property from the object. But	*/
	/* given that we're expected to MAKE SURE all TOC memory is released we're going to be*/
	/* paranoid here and formally attempt to free any value headers and their value				*/
	/* segments just as we did above.  There shouldn't be any, but lets just be sure.			*/
	
	if (theDynProperty != NULL) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theDynProperty->valueHdrList);
		while (theValueHdr) {
			nextValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
			theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
			while (theValue) {
				nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
				CMfree(theValue);
				theValue = nextValue;
			} /* value */
			CMfree(theValueHdr);
			theValueHdr = nextValueHdr;
		} /* valueHdr */
	} /* theDynProperty */
}


/*----------------------------------------------*
 | cmFreeTOC - free all the structures in a TOC |
 *----------------------------------------------*
 
 This routine is called to free all the structures in a TOC. For updating, a container may
 have its own TOC and the target TOC.  Thus the TOC pointer is an explicit parameter. It
 is passed as a pointer to the toc pointer because freeing the TOC will NULL out the
 caller's pointer.
 
 All memory for objects, properties, and types is freed.  On return the toc pointer in the
 container is NULL as well as all the master link head/tail pointers.
*/

void cmFreeTOC(ContainerPtr container, void **toc)
{
	TOCValueHdrPtr theValueHdr, nextValueHdr;
	TOCValuePtr		 theValue, nextValue;
	
	/* Free the main data structures...																										*/
	
	cmDestroyTOC(toc, (void *)container, true, cmFreeProperties);
	
	/* Free all the value headers on the deletedValues list. This list results in deletes	*/
	/* of objects, properties, or value headers themselves.  Objects are similarly put		*/
	/* on a separate list of deleted objects.  This list is freed by cmDestroyTOC().			*/
	
	theValueHdr = (TOCValueHdrPtr)cmGetListHead(&container->deletedValues);
	while (theValueHdr) {
		nextValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
		theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		while (theValue) {
			nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
			CMfree(theValue);
			theValue = nextValue;
		} /* value */
		CMfree(theValueHdr);
		theValueHdr = nextValueHdr;
	} /* valueHdr */
	
	cmInitList(&container->deletedValues);
}


/*-------------------------------*
 | cmSetValueBytes - set a value |
 *-------------------------------*
 
 This routine is used to just set the value (theValueBytes) with the specified data. The
 data is placed in theValueBytes as a function of the specified type, dataType.  There
 are 7 types:
	
 (1). Value_NotImm				This is the "normal" case for a non-immediate value data (a
	                        container offset) and data length.  The value/valueLen (i.e.,
													notImm) variant of a TOCValueBytes is used.
													
 (2). Value_Imm_Chars    	All of these use the corresponding imm variant of the ptr to the
 (3). Value_Imm_Long     	TOCValueBytes struct passed here.  The data is placed in the
 (4). Value_Imm_Short    	field according to type and, for the fun of it, the notImm
 (5). Value_Imm_Byte		  valueLen field set accordingly.  Note signed and unsigned are
 (6). Value_Imm_Ptr      	treated alike.
													
 (7). Value_GlobalName		This is a special case for global name strings. The data is
													assumed to be a pointer to a global name string.  A global name
													symbol table entry is created (using cmDefineGlobalName()) with
													the string.  The pointer to the symbol table entry is set as
													the value data. Note, a global name symbol table entry also
													has a back pointer to its "owning" TOCValue. THE CALLER MUST
													SET THIS FIELD!  Here we only deal with the value bytes.
													
 The function returns the input theValueBytes pointer as its result.  NULL is returned
 for Value_GlobalName if the allocation fails and an error will have been reported.
*/

TOCValueBytesPtr cmSetValueBytes(const ContainerPtr container,
																 TOCValueBytesPtr theValueBytes,
															 	 ConstValueType dataType, CM_ULONG data,
															 	 CM_ULONG dataLength)
{
	GlobalNamePtr g;
	CMBoolean			dup;
	
	switch (dataType) {				/* set 'em...																								*/
		case Value_NotImm:			theValueBytes->notImm.value		 = (CM_ULONG)data;
														theValueBytes->notImm.valueLen = (CM_ULONG)dataLength;
														break;
		
		case Value_Imm_Chars:		if (dataLength > sizeof(CM_ULONG)) dataLength = sizeof(CM_ULONG);
														theValueBytes->imm.ulongValue  = 0;			/* zero extra bytes */
														memcpy((CM_CHAR *)theValueBytes->imm.ucharsValue, (CM_CHAR *)data, (size_t)dataLength);
														theValueBytes->notImm.valueLen = dataLength;
														break;
		
		case Value_Imm_Long:		theValueBytes->imm.ulongValue  = (CM_ULONG)data;
														theValueBytes->notImm.valueLen = (CM_ULONG)sizeof(CM_ULONG);
														break;
		
		case Value_Imm_Short:		theValueBytes->imm.ulongValue  = 0;			/* zero extra bytes */
														theValueBytes->imm.ushortValue = (CM_USHORT)data;
														theValueBytes->notImm.valueLen = (CM_ULONG)sizeof(CM_USHORT);
														break;
		
		case Value_Imm_Byte:		theValueBytes->imm.ulongValue  = 0;			/* zero extra bytes */
														theValueBytes->imm.ubyteValue  = (CM_UCHAR)data;
														theValueBytes->notImm.valueLen = (CM_ULONG)sizeof(CM_UCHAR);
														break;
														
		case Value_GlobalName:	g = cmCreateGlobalName(container->globalNameTable, (CM_UCHAR *)data, &dup);
														if (g == NULL) {
															ERROR2(CM_err_NoGlobalName, data, CONTAINERNAME);
															theValueBytes = NULL;
														} else if (dup) {
															ERROR2(CM_err_DupGlobalName, data, CONTAINERNAME);
															theValueBytes = NULL;
														} else {
															theValueBytes->globalName.globalNameSymbol = g;
															theValueBytes->globalName.offset 					 = 0;
														}
														break;
	} /* switch */
	
	return (theValueBytes);
}


/*------------------------------------------------------------------------*
 | cmSetTOCValueHdrFlags - echo a value's flags in its value header flags |
 *------------------------------------------------------------------------*
 
 This routine is used to echo the settings of a value's flags in the value header which
 "owns" the value (TOCValue's are on a list whose header is in a TOCValueHdr).  This is
 done because a CMValue "refNum" that an API user is given and in turn given back to us
 is a pointer to a TOCValueHdr.  It is sometimes more convenient therefore to check the
 kind of value we have by looking at the header then "going out" to the value. In all but
 continued values there is only one TOCValue entry on the valueList anyway.  So echoing
 is more efficient than always going after the tail or head (they're the same) of a
 valueList just to see the flags and the kind of value.	
 
 The function updates the valueFlags field in the passed TOCValueHdr according to the
 value flags also passed.  The updated valueFlags are also returned as the function
 result.
*/

CM_USHORT cmSetTOCValueHdrFlags(TOCValueHdrPtr theValueHdr, const CM_USHORT flags)
{
	CM_USHORT			 valueFlags = (CM_USHORT)(theValueHdr->valueFlags & ValueCstFlags);
	ContainerPtr	 container;
	TOCValuePtr		 the1stValue;
	CM_CHAR				 flagsStr[15];
	
	/* The only non-obvious thing here is that we must take special precautions for 			*/
	/* continued values.  Since the last continued value flags does NOT say kCMContinued,	*/
	/* we check the first value of the given value header which must say it is continued.	*/
	/* From that we can reliably set the value header flag to say its value list is 			*/
	/* continued.																																					*/
	
	if (flags & kCMGlobalName)												/* global names											*/
		valueFlags |= (ValueGlobal | ValueDefined);
	else if (flags & kCMImmediate)										/* immediates												*/
		if (flags & kCMContinued) 
			valueFlags |= (ValueImmediate | ValueContinued);
		else
			valueFlags |= (ValueImmediate | ValueDefined);
	else if (flags & kCMContinued) 										/* continued												*/
		valueFlags |= ValueContinued;
	else if (flags & kCMDynValue)
		valueFlags |= ValueDynamic;
	else if (flags == 0) 															/* garden-variety value (...maybe)	*/
		if ((the1stValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList)) != NULL &&
				(the1stValue->flags & kCMContinued) != 0)
			valueFlags |= (ValueContinued | ValueDefined);
		else
			valueFlags |= ValueDefined;
	else {																						/* huh?															*/
		container = theValueHdr->container;
		ERROR1(CM_err_Internal1, cmltostr(flags, -4, true, flagsStr));
		valueFlags = theValueHdr->valueFlags;
	}
	
	return (theValueHdr->valueFlags = valueFlags);
}


/*---------------------------------------------------*
 | cmCreateValueSegment - create a new value segment |
 *---------------------------------------------------*
 
 This routine is called to create a new value segment and to fill in its fields with the
 passed parameters.  A pointer to the newly created value segment is returned.  NULL is
 returned if there is an allocation error.
 
 Segments are created generally created for continued values and appended on to the value
 chain whose header is container in a value header.  However, due to such things like
 value inserts a value segment may not necessarily be appended to the end of its chain.
 Thus this routine, which brings a value segment into existence, is a separate routine.
 
 Note, only the segment is created here.  It is up to the caller to chain it to its value
 header and echo the flags in that header.
*/

TOCValuePtr cmCreateValueSegment(TOCValueHdrPtr theValueHdr, TOCValueBytesPtr value,
														 	 	 const CM_USHORT flags)
{
	ContainerPtr container = theValueHdr->container;
	TOCValuePtr  theValue;

	if ((theValue = (TOCValuePtr)CMmalloc(sizeof(TOCValue))) == NULL) { /* create seg...	*/
		ERROR1(CM_err_NoValueEntry, CONTAINERNAME);
		return (NULL);
	}
	
	theValue->theValueHdr = theValueHdr;								/* fill in the fields...					*/
#if CMKEEP_CONTINUE_FLAG
	theValue->flags				= flags;
#else
	theValue->flags				= flags & ~kCMContinued;
#endif
	theValue->value				= *value;
	cmNullListLinks(theValue);
	
	theValue->container = container->updatingContainer;	/* who "owns" this new value			*/
	theValue->logicalOffset = 0;												/* filled in elsewhere						*/
	
	return (theValue);
}


/*----------------------------------------------------------*
 | cmAppendValue - append a value segment to a value header |
 *----------------------------------------------------------*
 
 This routine takes a pointer to a value definition (the actual bytes) and its flags
 and creates a TOCValue struct which is appended to the specified value header.  The
 function returns the value pointer as its result (i.e. pointer to the newly created
 value).  NULL is returned and an error reported if the allocation of the TOCValue fails.
 
 This routine is used to append actual values to an existing value header.  It is allowed
 to call cmDefineObject() with a NULL value bytes pointer.  In that case an object is 
 created with a value header but no value sublist off of that header.  This routine can
 be called to build that sublist.
 
 Internally, cmDefineObject() itself will result in a call to here to add the first value
 if one is so passed to it.
*/
															 
TOCValuePtr cmAppendValue(TOCValueHdrPtr theValueHdr, TOCValueBytesPtr value,
													const CM_USHORT flags)
{
	ContainerPtr container = theValueHdr->container;
	TOCValuePtr  theValue;
	CMObjectID	 propertyID;
	CM_CHAR		 offsetStr[15], lenStr[15];
	
	/* Check to see that the offset in the value is in range of the container. We only do	*/
	/* while we're loading a container (i.e., the container is not considered in a valid	*/
	/* state yet.																																					*/
	
	if (!container->tocFullyReadIn && (flags & kCMImmediate) == 0 && 
			(value->notImm.value > container->maxValueOffset 					||
			 value->notImm.value + value->notImm.valueLen > container->maxValueOffset)) {
		ERROR3(CM_err_BadOffset, cmltostr(value->notImm.value, 1, false, offsetStr), 
														 cmltostr(value->notImm.valueLen, 1, false, lenStr), CONTAINERNAME);
		return (NULL);
	}
	
	/* Create the value segment, fill in its fields, and add it to its value hdr list...	*/

	theValue = cmCreateValueSegment(theValueHdr, value, flags);
	if (theValue == NULL) return (NULL);
	cmAppendListCell(&theValueHdr->valueList, theValue);
	
	/* By saving the current total value size in the value segment we get the logical 		*/
	/* offset of this segment.  For only a single segment this will always be 0.  But for	*/
	/* continued values, each additional segment will then get its starting logical 			*/
	/* offset.  By remembering the original offset of each segment we will be able to			*/
	/* "do the math" to determine inserts and deletes of this data should be edited (we		*/
	/* don't care about that here).																												*/
	
	theValue->logicalOffset = theValueHdr->size;				/* set segment's logical offset		*/	
	
	/* Echo the flags and sum total size in the value header for this value entry...			*/
	
	(void)cmSetTOCValueHdrFlags(theValueHdr, flags);		/* echo flags in the value hdr		*/
	
	if ((theValueHdr->valueFlags & ValueGlobal) != 0)		/* global name										*/
		theValueHdr->size += GetGlobalNameLength(theValue->value.globalName.globalNameSymbol) + 1;
	else if (theValue->flags & kCMImmediate)
		theValueHdr->size += theValue->value.notImm.valueLen;	/* immediate byte, word, long */
	else																						
		theValueHdr->size += theValue->value.notImm.valueLen;	/* non-immediate data					*/
	
	/* If we're currently reading in the TOC (indicated by tocFullyReadIn == false), then	*/
	/* we must set the logical offset of each value segment.  This is the user's "view" of*/
	/* the record.  We use it only for updating to determine insert/delete operations for	*/
	/* creating update information.																												*/
	
	/* Also during this time, we must build the global name table for values that 				*/
	/* correspond to global names.  This is an in-memory table of the global names to make*/
	/* looking those things up more efficient.																						*/
	
	if (!container->tocFullyReadIn)	{										/* set logicalSize while loading	*/
		theValueHdr->logicalSize = theValueHdr->size;			/*    (only used for updating)		*/
		
		propertyID = theValueHdr->theProperty->propertyID;/* see if we have a global name...*/
		if (propertyID == CM_StdObjID_GlobalTypeName ||
				propertyID == CM_StdObjID_GlobalPropName)
			if (!cmBuildGlobalNameTable(theValue)) {				/* ...build (up) global name table*/
				CMfree(theValue);
				return (NULL);
			}
	}
	
	return (theValue);																	/* give caller back the value ptr	*/
}


/*--------------------------------------------*
 | defineValue - define a value for an object |
 *--------------------------------------------*
 
 This routine is used only by defineProperty() to create a value header and value for a
 property (theProperty) associated with an object.  A value is chained to its value
 header and the value headers to their property.  
 
 The value's typeID, value, generation and flags are given. The function returns a pointer
 to the value's value header if the value was successfully created and NULL if it wasn't.
 An error is reported if NULL is returned.  Note, while error calls are not supposed to
 return we assume here they due just to be safe!
 
 The flags indicate the kind of value we have.  Continued values are special, in that any
 continued value after the first for the current value header (the most recent one
 created) do not cause a new value header to be created for the property.  The returned
 value header is not linked into its property.  The caller, i.e., defineProperty() is
 responsible for that.  It has to know whether a continued value caused a new value header
 to be created.  Thus the newValueHdr switch is set true if it was and false if it is 
 just another continued value.
 
 Note, the pointer to the value bytes may be NULL.  This indicates that only the value 
 header for a yet to be defined value is to be created.
*/

static TOCValueHdrPtr CM_NEAR defineValue(ContainerPtr container,
																					TOCPropertyPtr theProperty,
																					const CM_ULONG typeID,
																					TOCValueBytesPtr value,
																					const CM_ULONG generation,
																					const CM_USHORT flags,
																					CMBoolean *newValueHdr)
{
	TOCValueHdrPtr theValueHdr;
	
	/* If the last value for the property is for a continued value, then the new value is	*/
	/* also considered continued (whether the flags say so or not -- the last continued		*/
	/* value doesn't have its contined bit set).  Continued values must all have the same	*/
	/* type ID.  Note, that since the last value of a set of continued set of values does	*/
	/* not have the continued flag, but all of them cause the continued flag in the value	*/
	/* header to be set, we need to know if the previous continued value was the last one.*/
	/* The ValueDefined bit in the header is used to do this (it's why it was created). If*/
	/* the last value was continued but is fully defined (because the last value was the	*/
	/* end of the continuation -- its flags were 0), then we must have a NEW incoming 		*/
	/* value.	Having said all this, it should be noted that all this continue stuff is 		*/
	/* used ONLY when reading in a TOC from the container.  At all other times continued	*/
	/* values are "manufactured" directly by CMWriteValueData().													*/
	
	/* If we don't have a continued value, we create a new value header for the property.	*/
	/* In all cases (continued and not continued) we, of course, create a new value entry.*/
	/* The only difference is weather the value header is a new one or a previous one.		*/
	
	if ((theValueHdr = (TOCValueHdrPtr)cmGetListTail(&theProperty->valueHdrList)) != NULL &&
			(theValueHdr->valueFlags & ValueContinued) != 0 &&
			(theValueHdr->valueFlags & ValueDefined) == 0){	/* most recent value is cont'd...	*/
		if (theValueHdr->typeID != typeID) {							/* ...types must agree						*/
			ERROR1(CM_err_BadContinue, CONTAINERNAME);
			return (NULL);
		}
		*newValueHdr = false;															/* ...use most recent value hdr		*/
	} else {																						/* create a new value header...		*/
		if (container->tocFullyReadIn)										/* if valid TOC, check for dups...*/
			if (cmGetPropertyType(theProperty, typeID) != NULL) {
				ERROR2(CM_err_DupType, cmGetGlobalTypeName(container, typeID), CONTAINERNAME);
				return (NULL);
			}
			
		if ((theValueHdr = (TOCValueHdrPtr)CMmalloc(sizeof(TOCValueHdr))) == NULL) {
			ERROR1(CM_err_NoValueEntry, CONTAINERNAME);
			return (NULL);
		}

		*newValueHdr = true;	
		cmNullListLinks(theValueHdr);											/* init value hdr fields...				*/
		cmInitList(&theValueHdr->valueList);							/* ...hdr is linked by caller			*/
		theValueHdr->valueFlags    = 0;										/* ...no flags yet								*/
		theValueHdr->typeID 	 	   = typeID;							/* ...use passed type ID					*/
		theValueHdr->container 	   = container;						/* ...ptr to owning container			*/
		theValueHdr->theProperty   = theProperty;					/* ...ptr to owning property 			*/
		theValueHdr->size				   = 0;										/* ...total value size initially 0*/
		theValueHdr->logicalSize	 = 0;										/* ...same for logicalSize				*/
		theValueHdr->generation	   = generation;					/* ...generation number						*/
		theValueHdr->useCount		   = 0;										/* ...no uses yet (well, not here)*/
		theValueHdr->valueRefCon   = NULL;								/* ...no refCon value yet					*/
		RefDataObject(theValueHdr) = NULL;								/* ...no value so no references		*/
		theValueHdr->touch 				 = NULL;								/* ...not interested in touch here*/
		DYNEXTENSIONS(theValueHdr) = NULL;								/* ...no dynamic value extensions	*/
																											
		/* Note: Dynamic value extensions are ONLY set by cmNewDynamicValue().							*/
		/*       References value extensions are ONLY set by ?().														*/
	}
	
	/* Now we can create a new value entry.  To make it easire to check for the various		*/
	/* attributes of values we set the valueFlags in the value header according to the		*/
	/* value flags themselves.  We only do it now if there currently is no value (see			*/
	/* below for more about that). If there is a value, we will be calling cmAppendValue()*/
	/* which does the same thing.  There is no sense doing it twice.											*/
	
	/* We also allow the pointer to the value bytes to be NULL.  This is defined as the 	*/
	/* creation of a object with a valueHdr but with NO VALUE.  This occurs when the API	*/
	/* caller needs only a "refNum" to a value.  We use value header pointers as such			*/
	/* "refNum"s.  Other API calls are done which specify the same object with a real 		*/
	/* value to be attached to the object. Values are then attached to the valueHdr, BUT	*/
	/* NOT FROM HERE!  This routine is responsible only for creating objects WITH values.	*/
	/* If the value creation is suppressed that is not this routines problem!  Take it		*/
	/* somewhere else!																																		*/
	
	if (value == NULL)																	/* append new value if we have one*/
		(void)cmSetTOCValueHdrFlags(theValueHdr, flags);
	else if (cmAppendValue(theValueHdr, value, flags) == NULL)
		return (NULL);
	
	return (theValueHdr);																/* give caller the value hdr ptr	*/
}


/*----------------------------------------------------------------*
 | defineProperty - define a property and its value for an object |
 *----------------------------------------------------------------*
 
 This routine is used only by cmDefineObject() to create a property and its associated
 values for an object.  The properties are chained to the object, values to value headers,
 and the value headers to the properties.
 
 The value's typeID, value, generation and flags are given. The function returns a pointer
 to the property if the property was successfully created and NULL if it wasn't.  An error
 is reported if NULL is returned.  Note, while error calls are not supposed to return we
 assume here they due just to be safe!
 
 A new property for the object is only created if it has a different property id.  The
 value is chained to the property (a previous one or the new one for a new id). The
 returned property pointer is not linked to the object.  The caller, i.e., cmDefineObject()
 is responsible for that. It has to know whether a new property was created.  Thus the
 newProperty switch is set true if it was and false if it is just a previously used
 property for the object.
 
 Note, the value associated with the property is appended to the the end of the property's
 value list.  The caller has the option of getting the pointer to the value header 
 (theValueHdr) when theValueHdr is not passed as NULL.
*/

static TOCPropertyPtr CM_NEAR defineProperty(ContainerPtr container,
																						 TOCObjectPtr theObject,
																						 const CM_ULONG propertyID,
																						 const CM_ULONG typeID,
																						 TOCValueBytesPtr value,
																						 const CM_ULONG generation,
																						 const CM_USHORT flags, CMBoolean *newProperty,
																						 TOCValueHdrPtr *theValueHdr)
{
	TOCPropertyPtr theProperty;
	CMBoolean 			 newValueHdr;
	
	/* Scan all the properties for the object to see if we got a new one...								*/
	
	theProperty = cmGetObjectProperty(theObject, propertyID);
	*newProperty = (CMBoolean)(theProperty == NULL);
	
	/* If we got a new property create it and init its fields...													*/
	
	if (*newProperty) {															/* new property...										*/
		if ((theProperty = (TOCPropertyPtr)CMmalloc(sizeof(TOCProperty))) == NULL) {
			ERROR1(CM_err_NoPropEntry, CONTAINERNAME);
			return (NULL);
		}
		cmNullListLinks(theProperty);
		cmInitList(&theProperty->valueHdrList);
		theProperty->propertyID = propertyID;
		theProperty->theObject  = theObject;					/* note, ptr to owning object					*/
	}
	
	/* To make it easier to read this mess we define the value and its header separately!	*/
	
	*theValueHdr = defineValue(container, theProperty, typeID, value, generation, flags, &newValueHdr);
	
	if (*theValueHdr == NULL) {
		if (*newProperty) CMfree(theProperty);
		return (NULL);
	}
	
	/* If we don't have a continued value, or even if we do, but its the first value of		*/
	/* the continuation, add it to property's value list. 																*/
	
	if (newValueHdr) 																/* if new value hdr...								*/
		cmAppendListCell(&theProperty->valueHdrList, *theValueHdr); /* ...add to end of list*/
		
	return (theProperty);														/* give caller the property ptr				*/
}


/*------------------------------------------------------------*
 | cmDefineObject - define a object with a property and value |
 *------------------------------------------------------------*
 
 This routine is called to define a new TOC entry for an object.  We may have either a
 new object (id) or a preexisting one for which a new property and value are to be 
 defined.  All the fields for a TOC entry are passed.  The objectFlags indicate the
 type of the object (more about this later).
 
 Note, the value associated with the property is appended to the the end of the
 property's value list.  The caller has the option of getting the pointer to the value
 header (theValueHdr) when theValueHdr is not passed as NULL.
 
 The function returns a pointer to the object if it was successfully created and NULL if
 it wasn't.  An error is reported if NULL is returned.  Note, while error calls are not
 supposed to return we assume here they due just to be safe!
 
 The objectFlags determine how we treat the object and all the object fields (the other
 parameters).  There are four possible objectFlags:
 
 (1). UndefinedObject			Set if the object is to be created, but we don't yet know what
 													its TOC entries are.  Basically a null object (or placeholder)
													is created.  It is an incomplete object in that there are no
													properties or types chained to the object (yet).  This flag may
													be used in combination with the others if we know that the 
													object ID corresponds to a property, type, or neither.
													
 (2). ObjectObject				This flags is used when we don't know the type of the object but
 													we know a property and type for it.  If the object already exists
													and it's undefined (UndefinedObject) it is now considered as
													defined.  If it was an undefined property or type, it now becomes
													a defined property or type.  Of course, duplicate definitions
													are an error.
 
 (3). PropertyObject			This is similar to ObjectObject, but here we know the object is
 													for a property.  It has to either not exist previously, or was
													previously flagged as UndefinedObject and PropertyObject.
 
 (4). TypeObject					Same as PropertyObject, but for type objects.

 This routine is the "main control" of Container Manager object creation.  It is used
 for "normal" object creation and while loading in a TOC from a preexisting container
 (i.e., from cmReadTOC()). After cmReadTOC(), or if we are writing and never do a
 cmReadTOC() we have the "normal" case.  "We control the horizontal. Do not adjust your
 TV set". We always know what we are creating (yeah, I got a bridge to sell you too). 
 Thus, in that case, we can link the objects to their master lists. 

 For cmReadTOC() however, we are creating objects as we see thier id's in each TOC entry as
 we read them.  We can have forward references to undefined and yet to be fully
 created objects and backward references to existing objects.  We can also have multiple
 properties and values for the same object.  Hence the flags and all they imply.  We also
 do not link the objects.  That is delayed until the TOC is completely read in.  At that
 point the entire TOC is walked sequentially and the links built.  We can also do some
 additional validation to make sure that everything is defined after the read in.
*/

TOCObjectPtr cmDefineObject(ContainerPtr container, CM_ULONG objectID,
									   				const CM_ULONG propertyID, const CM_ULONG typeID, 
										 				TOCValueBytesPtr value, const CM_ULONG generation,
														const CM_USHORT flags, const CM_USHORT objectFlags,
														TOCValueHdrPtr *theValueHdr)
{
	TOCObjectPtr 	 theObject;
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr dummyValueHdr;
	CMBoolean			 dup, newProperty;
	CM_CHAR				 idStr[15];
	
	/* If the object ID's wrap, the objectID value will go to 0x00000000.  We set it to		*/
	/* this when we bump the ID counter and detect we will wrap if we should ever attempt	*/
	/* to use another ID.  But we may not.  So the error is not reported when the wrap is	*/
	/* detected.  The counter is only set to 0x00000000.  That ID will work its way into	*/
	/* here to define another object.  Now we're attempting to use the ID.  So now is the	*/
	/* time to report the error.																													*/
	
	if (objectID == 0x00000000UL) {
		ERROR1(CM_err_wrappedIDs, CONTAINERNAME);
		return (NULL);
	}
	
	/* Create the basic object.  If it already exists dup will be true  If it doesn't, it	*/
	/* will be created.. Its fields will be initialized and the objectFlags set.					*/
	
	theObject = cmCreateObject(container->toc, (CMObjectID)objectID, objectFlags, &dup);

	if (theObject == NULL) {
		ERROR1(CM_err_NoObjEntry, CONTAINERNAME);
		return (NULL);
	}
	
	/* If the object is to be an undefined place holder we ignore all the TOC field				*/
	/* parameters.  But we must do a set of consistency checks before we will accept it.	*/
	/* Here are the conditions which make an undefined object acceptable:									*/
	
	/*		(1). The object is new (not a dup).  This is the first time we have seen it.		*/
	
	/*		(2). The object has been seen before and it is still undefined.  Thus we are 		*/
	/*				 only trying to create the object because it was referenced more than once 	*/
	/* 				 as we were loading a TOC.																									*/
	
	/*		(3). The object has been seen before and IS defined.  Its type is the same as		*/
	/*				 what we are trying make a place holder for.  This occurs when the caller 	*/
	/* 				 thinks the id may be undefined but knows it's a property or type.  If we		*/
	/*				 already defined it it better be the same (i.e., property or type).					*/
	
	/*		(4). The object has been seen before and IS defined.  But it was flagged as an 	*/
	/*				 ObjectObject because we didn't know whether it was a property or type. The	*/
	/*				 caller still thinks it may be undefined, but knows it's a property or type.*/
	/*				 We thus CHANGE the ObjectObject flag to property or type now that we know	*/
	/* 				 what it realy is. The end result is that a earlier defined but unknown kind*/
	/* 				 of object (hence ObjectObject) now has a know kind, i.e., it's a property	*/
	/*				 or type.  We have resolve a forward reference to an object.  Note that if	*/
	/*			   the forward reference was to a known kind (property or type) and the knew	*/
	/*				 kind is different, we have an error. It is an attempt to multiply define 	*/
	/*				 the object as both a property and a type.																	*/
	
	/* In summary, whew...																																*/
	
	if (objectFlags & UndefinedObject) {						/* do what it says above!							*/
		if (!dup) 																		return (theObject);			/*    (1) 		*/
		if (theObject->objectFlags & UndefinedObject) return (theObject);			/*    (2)			*/
		if (theObject->objectFlags & objectFlags) 		return (theObject);			/*    (3)			*/
		if (theObject->objectFlags & (PropertyObject | TypeObject)) {					/*    (4)			*/
			ERROR2(CM_err_MultDef, cmltostr(objectID, 1, false, idStr), CONTAINERNAME);
			return (NULL);
		}
		theObject->objectFlags = (CM_USHORT)(objectFlags & ~UndefinedObject);
		return (theObject);														/* give caller back the object				*/
	}
	
	/* At this point we are trying to fully create an object.  We know what it is because	*/
	/* the objectFlags didn't say UndefinedObject.  If the object already exists, but it  */
	/* is still undefined, we now mark it defined. 													 							*/
	
	theObject->objectFlags &= ~UndefinedObject;
	
	/* If we get this far we are going to create a new property for an object or use an		*/
	/* existing one if one for the same property ID already exists.  Either way the value	*/
	/* will be added to the chain off the property.  The value will be appened to the end	*/
	/* of the chain. Here it goes...																											*/
	
	if (theValueHdr == NULL) theValueHdr = &dummyValueHdr; /* supply dummy if necessary		*/
		
	theProperty = defineProperty(container, theObject, propertyID, typeID, value, generation,
															 flags, &newProperty, theValueHdr);
	if (theProperty == NULL) return (NULL);
	
	/* If we indeed did create a new property add it to the object's property chain...		*/
	
	if (newProperty) cmAppendListCell(&theObject->propertyList, theProperty);
	
	/* Finally, if we are working with a "valid" TOC (remember "we control the 						*/
	/* horizontal") then link the object to the master lists.															*/
	
	if (container->tocFullyReadIn) cmLinkObject(container->toc, theObject, NULL);
	
	return (theObject);															/* give object and valueHdr to caller	*/
}


/*-----------------------------------------------------------------------------*
 | cmGetObjectProperty - find the property with the specified ID for an object |
 *-----------------------------------------------------------------------------*
 
 This routine takes a pointer to an object and scans its properties for the specified ID.
 The pointer to the property on the object's property chain is returned if found.  NULL is
 returned if not found.
*/

TOCPropertyPtr cmGetObjectProperty(TOCObjectPtr theObject, CM_ULONG propertyID)
{
	TOCPropertyPtr theProperty;
		
	theProperty = (TOCPropertyPtr)cmGetListHead(&theObject->propertyList);
	
	while (theProperty) {																	/* scan each property on list		*/
		if (theProperty->propertyID == propertyID) break;		/* exit as soon as we find ID		*/
		theProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
	}
	
	return (theProperty);																	/* return property ptr or NULL	*/
}


/*---------------------------------------------------------------------------------------*
 | cmGetPropertyType - find the value (header) with the specified type ID for a property |
 *---------------------------------------------------------------------------------------*

 This routine takes a pointer to a object's property and scans its value headers for one
 containing the specified type ID.  The pointer to the value header is returned if found.
 NULL is returned if not found.
*/

TOCValueHdrPtr cmGetPropertyType(TOCPropertyPtr theProperty, CM_ULONG typeID)
{
	TOCValueHdrPtr theValueHdr;
		
	theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
	
	while (theValueHdr) {																	/* scan each value hdr on list	*/
		if (theValueHdr->typeID == typeID) break;						/* exit as soon as we find ID		*/
		theValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
	}
	
	return (theValueHdr);																	/* return value hdr ptr or NULL	*/
}


/*----------------------------------------------------------------------------*
 | cmGet1ValueSize - return the size of the data for a single value (segment) |
 *----------------------------------------------------------------------------*
 
 This routine takes a pointer to a value (NOT a value header -- continued values are not
 worried about here) and returns the size represented by that value.  For non-immediate
 values we simply return the length field from the value.  Immediate value sizes are
 dependent on their type.
*/

CM_ULONG cmGet1ValueSize(TOCValuePtr theValue)
{
	CM_ULONG size;
	
	if (theValue->flags & kCMGlobalName)
		size = GetGlobalNameLength(theValue->value.globalName.globalNameSymbol) + 1;
	else if (theValue->flags & kCMImmediate)
		size = theValue->value.notImm.valueLen;				/* immediate byte, word, or long data	*/
	else																						
		size = theValue->value.notImm.valueLen;				/* non-immediate data									*/
		
	return (size);
}


#if CMVALIDATE
/*-------------------------------------------------------------------------------*
 | cmValidateCMValue - validate a CMValue "refNum" passed to the API by the user |
 *-------------------------------------------------------------------------------*
 
 CMValue "refNum"s in the API are implemented in the Container Manager as pointers to 
 TOCValueHdr structs.  This routine is used to filter "bad" value "refNum"s passed to the
 higher level API interface  ("CM...") routines.  False is returned if the value is
 considered as "bad" and true otherwise.
 
 As currently implemented we return false but do NOT report an error for NULL pointers.
 That is because we return NULL from everthing we error out of earlier.  The NULL test is
 just protection from letting such pointer from ever being used.
 
 Another check we do without reporting an error is on the container pointer.  The refNums
 have a pointer to their container.  The container has a special pointer pointing to 
 itself.  We check agains this pointer.  If we think we don't have a valid container
 pointer then we assume we have a bad refNum and return false.  No error can be reported
 because, wiut a container pointer, there is no way to do it!
 
 We DO report an error for attempting to use a deleted value.  Deleted values are NOT
 freed.  Rather they (actually their value headers) are put on a list of deleted values
 and flagged as deleted just for the purpose of testing here.  If we had freed them, then
 we would be getting pointers to "garbage". By keeping them around we have valid pointers
 and we can do the test. Yuk!
*/

CMBoolean cmValidateCMValue(CMValue value)
{
	ContainerPtr container;
	
	if (value == NULL) return (false);								/* false if bad value								*/
	
	container = ((TOCValueHdrPtr)value)->container;		/* get container from refNum				*/
	
	NOPifNotInitialized(false);												/* false if not initialized (NULL)	*/

	if (container != container->thisContainer)				/* check against the check ptr			*/
		return (false);
	
	container = container->updatingContainer;					/* use updating container						*/
	
	if (!VALIDATE) return (true);											/* dynamically suppressed checking	*/
	
	/* Make sure value has not been deleted...																						*/
	
	if ((((TOCValueHdrPtr)value)->valueFlags & ValueDeleted) != 0) {
		ERROR1(CM_err_BadValue, CONTAINERNAME);
		return (false);
	}
	
	return (true);																		/* ok, it's good -- maybe!					*/
}
#endif


#if CMVALIDATE
/*---------------------------------------------------------------------------------*
 | cmValidateCMObject - validate a CMObject "refNum" passed to the API by the user |
 *---------------------------------------------------------------------------------*

 CMObject (and CMProperty, and CMType) "refNum"s in the API are implemented in the
 Container Manager as pointers to TOCObject structs.  This routine is used to filter "bad"
 object "refNum"s  passed to the higher level API interface  ("CM...") routines.  False is
 returned if the object is considered as "bad" and true otherwise.
 
 As currently implemented we return false but do NOT report an error for NULL pointers.
 That is because we return NULL from everthing we error out of earlier.  The NULL test is
 just protection from letting such pointer from ever being used.
 
 Another check we do without reporting an error is on the container pointer.  The refNums
 have a pointer to their container.  The container has a special pointer pointing to 
 itself.  We check agains this pointer.  If we think we don't have a valid container
 pointer then we assume we have a bad refNum and return false.  No error can be reported
 because, wiut a container pointer, there is no way to do it!
 
 We DO report an error for attempting to use a deleted object.  Deleted objects are NOT
 freed.  Rather they are put on a list of deleted objects and flagged as deleted just for
 the purpose of testing here.  If we had freed them, then we would be getting pointers to
 "garbage".  By keeping them around we have valid pointers and we can do the test.
 
	For types and properties we also validate that such "refNum"s are indeed types or
	properties.  The desired type is passed in the objectFlags.  This test can always be
	forced to pass by passing 0xFFFFU.  For types and properties, TypeObject or 
	PropertyObject should be passed as appropriate.
*/

CMBoolean cmValidateCMObjects(CMObject object, CM_USHORT objectFlags)
{
	ContainerPtr container;
	
	if (object == NULL) return (false);								/* false if bad value								*/
	
	container = ((TOCObjectPtr)object)->container;		/* get container from refNum				*/
	
	NOPifNotInitialized(false);												/* false if not initialized (NULL)	*/
	
	if (container != container->thisContainer)				/* check against the check ptr			*/
		return (false);

	container = container->updatingContainer;					/* use updating container						*/
	
	if (!VALIDATE) return (true);											/* dynamically suppressed checking	*/
	
	/* Note, we assume that if an object is not NULL, it does indeed point to a container	*/
	/* object.  We must make that assumption because it's the only way we can get the 		*/
	/* container pointer.  We return NULLs from everwhere when something goes wrong, so 	*/
	/* the probability that a non-NULL is an object is pretty high.  Besides, if the user	*/
	/* follos the rules, the second we report an error, the error report should abort			*/
	/* execution!																																					*/
	
	/* Make sure object has not been deleted...																						*/

	if ((((TOCObjectPtr)object)->objectFlags & DeletedObject) != 0) {
		ERROR1(CM_err_BadObject, CONTAINERNAME);
		return (false);
	}

	/* Make sure the kind of object is what is desired...																	*/

	if ((((TOCObjectPtr)object)->objectFlags & objectFlags) == 0) {
		ERROR2(CM_err_BadObjectKind, (objectFlags & TypeObject) ? "type" : "property", CONTAINERNAME);
		return (false);
	}
	
	return (true);																		/* ok, it's good -- maybe!					*/
}
#endif


/*----------------------------------------------------------*
 | walkObject - walk all the structures for a single object |
 *----------------------------------------------------------*
 
 This routine serves two purposes. First it is an action routine for the object interator,
 cmForEachObject(). It is set by cmWalkThroughEntireTOC() to walk all the structures for a
 single object.  Thus the net result from cmWalkThroughEntireTOC()'s point of view is that
 each structure in the the TOC is visited.  Each object (by ascending order of object id),
 each property for each object, and each value (header and value) for each property.
 
 The second purpose of this routine is to act as the "guts" of cmWalkObject().  There are
 possible situations where we only want to walk a single object.  Since that's what this
 routine does, cmWalkObject() calls it explicitly instead of repeatedly from 
 cmForEachObject().  The result is the same.  One object is walk per call.
 
 At each point in the data structures linked to an object we call a action routine
 specific to that structure.  There is a object action routine (objectAction), a property
 action routine (propertyAction) and a value action routine (valueAction).  See
 cmWalkThroughEntireTOC() or cmWalkObject() for further details on these routines.  Note
 that if a routine pointer is NULL, no action is taken for that structure.  There are also
 conventions for skipping the walking of parts of the data structures.  Again, see
 cmWalkThroughEntireTOC() for details.
 
 The refcon in this context is the container pointer.  It, in turn, contains a pointer
 back to a structure defined locally in cmWalkThroughEntireTOC() or cmWalkObject()
 containing all the action routine pointers and the original caller's refCon.  See
 cmWalkThroughEntireTOC() or cmWalkObject() for details.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmForEachObject().
*/

static void walkObject(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container  = (ContainerPtr)refCon;
	TOCActionsPtr  tocActions = (TOCActionsPtr)container->tocActions;
	TOCPropertyPtr theProperty, nextProperty;
	TOCValueHdrPtr theValueHdr, nextValueHdr;
	TOCValuePtr		 theValue, nextValue;
	TOCWalkReturns action;
	
	/* From this point on, the refCon to the action routines will be whatever the caller	*/
	/* supplied to cmWalkThroughEntireTOC(). We overload our used of "refCon" since				*/
	/* there's  no sense creating another variable.																				*/
	
	refCon = tocActions->refCon;
	
	/* Call the action routine to handle object structures...															*/
	
	if (tocActions->objectAction) 
		if ((*tocActions->objectAction)(container, theObject, refCon) == WalkNextTOCObject)
			return;
	
	/* If there are no property, valueHdr, or value actions, there is nothing more to do..*/
	
	if (tocActions->propertyAction == NULL &&
			tocActions->valueHdrAction == NULL &&
		  tocActions->valueAction == NULL) 
		return;
	
	/* Walk through the property list for this object, calling its action routine...			*/
	
	theProperty = (TOCPropertyPtr)cmGetListHead(&theObject->propertyList);
	while (theProperty) {
		nextProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
		
		if (tocActions->propertyAction) {
			action = (*tocActions->propertyAction)(container, theProperty, refCon);
			if (action == WalkNextTOCProperty) goto nextproperty;
			if (action == WalkNextTOCObject) 	 return;
		}
		
		/* Walk through the value header list for each property, calling the action routine	*/
		/* for them.  Note the value header pointer is passed since a single value is				*/
		/* considered as all the value entries chained off a single value header.  That			*/
		/* only comes about from continued values.																					*/
		
		if (tocActions->valueHdrAction || tocActions->valueAction) {
			theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
			while (theValueHdr) {
				nextValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
				
				if (tocActions->valueHdrAction) {
					action = (*tocActions->valueHdrAction)(container, theValueHdr, refCon);
					if (action == WalkNextTOCValueHdr) goto nextvaluehdr;
					if (action == WalkNextTOCProperty) goto nextproperty;
					if (action == WalkNextTOCObject)   return;
				}
				
				if (tocActions->valueAction) {
					theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
					while (theValue) {
						nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
						action = (*tocActions->valueAction)(container, theValue, refCon);
						theValue = nextValue;
						if (action == WalkNextTOCValueHdr) goto nextvaluehdr;
						if (action == WalkNextTOCProperty) goto nextproperty;
						if (action == WalkNextTOCObject)   return;
					} /* value */
				}
				
				nextvaluehdr:
				theValueHdr = nextValueHdr;
			} /* valueHdr */
		}
		
		nextproperty:		
		theProperty = nextProperty;
	} /* property */
}
					

/*-------------------------------------------------------------------*
 | cmWalkThroughEntireTOC - apply actions to each structure in a TOC |
 *-------------------------------------------------------------------*
 
 This routine is called to walk the entire specified TOC by ascending object ID, starting 
 with object ID's greater than or equal to the startingID and less than or equal to
 endingID, and to call an action routine appropriate for each kind of structure: object,
 property, value header, and value.  
 
 Pointers to the four action routines are passed with headers as shown in the definition. 
 If a pointer is passed as NULL, no action will be performed for the corresponding
 structure.  Each function has return codes (type TOCWalkReturns) that direct the walking
 through the TOC.	The action routines should return the codes shown in the following
 table:
			
					  Walk...  NextTOCObject | NextTOCProperty | NextTOCValueHdr | NextTOCValue
		===============|===============|=================|=================|==============
		objectAction   |  next object  |  next property* |                 |
		propertyAction |  next object  |  next property  | next value hdr* |
		valueHdrAction |  next object  |  next property  | next value hdr  | next segment*
		valueAction    |  next object  |  next property  | next value hdr  | next segment*
		---------------+---------------+-----------------+-----------------+--------------

 The entries marked with "*" are the returns that should be used in the normal case to
 fully walk each data structure of the TOC.  Those not marked with "*" are for special
 cases to abort the walk to go to the next indicated data structure.  Walks are sequential
 through the TOC by ascending object ID.  Thus skipping a particular walk of one structure
 only makes sense if the skip is possible.  That is the reason not all returns are allowed
 in all action routines.  In reality, only the non-starred returns are checked on return
 from an action routine. So the default is the starred return (i.e., keep walking normally)
 if a non-starred is not returned.
 
 A "refCon" is also passed which the caller can use as a communication facility to convey
 additional info to the action routines.  
 
 This function returns 0 to indicate successful completion, and non-zero otherwise.  Thus
 if AbortWalkThroughEntireTOC(x) is used to abort processing, the x should be a positive
 non-zero integer.
 
 Note, if the property, value header, and value action routines are supplied as NULL,
 then cmWalkThroughEntireTOC() is functionally equivalent to cmForEachObject() which just
 applies a single action routine to the objects.  Thus if only the objects are to be
 walked, it is recommended that cmForEachObject() be used instead of
 cmWalkThroughEntireTOC().
 
 Note also, this routine uses cmForEachObject() to walk the TOC objects with walkObject()
 defined above as the action routine.  It handles the walking of the structures "below"
 the object level. We use the refCon parameter to cmForEachObject() to communicate to
 walkObject() all the action routine pointers and the caller's refCon.  In particular,
 the refCon is a pointer to the container.  In the container is a pointer BACK to a local
 data structure we set up here with all the info.
 
 We can get away with this stunt because this routine is always up level in the stack and
 its locals are valid while cmForEachObject() is doing its thing.
 
 The additional trick we pull here is to set up a setjmp/longjmp environment vector for
 the AbortWalkThroughEntireTOC() macro.  That macro is a longjmp back to here with a
 value we return as the function result.  Since the data structure is local here, the only
 way the macro can get at the environment vector is through the pointer in the container.
 I don't want the local data structure visable outside this file.  So the only place the
 vector can be placed is at the START of the data structure.  Win a few, loose a few!
*/

int cmWalkThroughEntireTOC(ContainerPtr container, void *toc, 
													 CMObjectID startingID, CMObjectID endingID,
													 CMRefCon refCon,
													 TOCWalkReturns (*objectAction)(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon),
													 TOCWalkReturns (*propertyAction)(ContainerPtr container, TOCPropertyPtr theProperty, CMRefCon refCon),
													 TOCWalkReturns (*valueHdrAction)(ContainerPtr container, TOCValueHdrPtr theValueHdr, CMRefCon refCon),
													 TOCWalkReturns (*valueAction)(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon))
{	
	TOCActions tocActions;
	
	tocActions.objectAction		= objectAction;					/* save the action routine ptrs			*/
	tocActions.propertyAction	= propertyAction;
	tocActions.valueHdrAction = valueHdrAction;
	tocActions.valueAction		= valueAction;
	
	tocActions.refCon					= refCon;								/* save caller's refCon							*/
	
	if (setjmp(tocActions.walkThroughEnv))						/* if longjmp taken...							*/
		return (1);																			/* ...report the bad news						*/
	
	container->tocActions = (void *)&tocActions;			/* set ptr to our info in container	*/

	return (cmForEachObject(toc, startingID, endingID, (void *)container, walkObject));
}


/*-----------------------------------------------------------------------*
 | cmWalkObject - apply actions to each structure of a single TOC object |
 *-----------------------------------------------------------------------*
 
 This routine is called to walk a single object (theObject) and call an action routine
 appropriate for each kind of structure: object, property, value header, and value. 
 Pointers to the four action routines are passed with headers as shown in the definition.
 If a pointer is passed as NULL, no action will be performed for the corresponding
 structure.  The returns are the same as cmWalkThroughEntireTOC().  See it for further
 details.
 
 A "refCon" is also passed which the caller can use as a communication facility
 to convey additional info to the action routines.  0 is returned to indicate successfull
 completion.  Use the AbortWalkObject(x) (a macro) in an action routine to abort the
 walk.  The "x" is a integer which is returned from cmWalkObject() so it should not be 0.
 
 Implementation note: This routine is basically "glue" code for outside callers to 
 walkObject().  walkObject() is defined as an action routine for cmForEachObject() which is
 called by cmWalkThroughEntireTOC() above.  The walkObject() routine wals a single object
 and applies the action routines at each point in an objects data structure.  This is
 exactly what is needed here so that why we use.  There's no sence reinventing the wheel!
 
 Of course, since walkObject() doesn't know we are calling it from here, and things it's an
 action routine for cmForEachObject(), which was called by cmWalkThroughEntireTOC(), then
 we must observe ALL the conventions set up by cmWalkThroughEntireTOC().  See comments
 for cmWalkThroughEntireTOC() for these conventsions (the refCon, aborting, etc.).
 
 The only thing different in the code below is that we explicitly call walkObject() instead
 of cmForEachObject() and a more apporpriate "abort" macro is provided.  The macro is
 called AbortWalkObject(). But expands to identically to what AbortWalkThroughEntireTOC()
 expands to.
*/

int cmWalkObject(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon,
								 TOCWalkReturns (*objectAction)(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon),
								 TOCWalkReturns (*propertyAction)(ContainerPtr container, TOCPropertyPtr theProperty, CMRefCon refCon),
								 TOCWalkReturns (*valueHdrAction)(ContainerPtr container, TOCValueHdrPtr theValueHdr, CMRefCon refCon),
								 TOCWalkReturns (*valueAction)(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon))
{
	TOCActions tocActions;
	
	tocActions.objectAction		= objectAction;					/* save the action routine ptrs			*/
	tocActions.propertyAction	= propertyAction;
	tocActions.valueHdrAction = valueHdrAction;
	tocActions.valueAction		= valueAction;
	
	tocActions.refCon					= refCon;								/* save caller's refCon							*/
	
	if (setjmp(tocActions.walkThroughEnv))						/* if longjmp taken...							*/
		return (1);																			/* ...report the bad news						*/
		
	container->tocActions = (void *)&tocActions;			/* set ptr to our info in container	*/

	walkObject(theObject, (void *)container);					/* walk single object	(not a beast)	*/
	
	return (0);																				/* what do you know, we made it!		*/
}
														 
														  CM_END_CFUNCTIONS
