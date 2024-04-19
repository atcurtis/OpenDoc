/* @(#)Z 1.5 com/src/cm/TOCObjs.c, odstorage, od96os2, odos29646d 96/11/15 15:27:26 (96/10/29 09:19:49) */
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
	File:		 TOCObjs.c  

	Contains:	Container Manager TOC Objects Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182275 Matches TOCObjes.h.
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
 |                           <<<  TOCObjs.c   >>>                            |
 |                                                                           |
 |                  Container Manager TOC Objects Routines                   |
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
 off of it. The  ListMgr package is used to maintain the ListMgr by the caller by using the
 object.  Other fields are provided as well which are also maintained by the caller.  The
 only exception is the object ID number which is known to this file.
*/


#include <stddef.h>
#include <string.h>
#include <setjmp.h>
#include <limits.h>
#include <stdio.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
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
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __UPDATING__
#include "Update.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment TOCObjects
#endif


#define MaxValue ULONG_MAX							/* 0xFFFFFFFF on MC68XXX's											*/

typedef void **Indices;									/* index tables are arrays of pointers					*/

/* All objects in a TOC are linked together on a single master object list chain.  Also	*/
/* all properties and types are chained on separate lists which are subsets of the full */
/* object chain.  There are fields in an object for all three of these chain links. 		*/
/* Since types and properties are mutually exclusive objects, then, of course we need 	*/
/* only one link for either of these chains.  But there are three chain headers, and 		*/
/* these headers, defined below, are kept in the TOC control block and manipulated by		*/
/* cmLinkObject() and cmUnLinkObject().																									*/

struct ObjectListHdrs {									/* Layout of master lists object chain headers:	*/
	TOCObjectPtr objectsListHead;					/* 		list of all objects, props, and types			*/
	TOCObjectPtr objectsListTail;					/*		last object, property, or type created		*/
	
	TOCObjectPtr propertiesListHead;			/*		list of all property objects							*/
	TOCObjectPtr propertiesListTail;			/*		last property object created							*/
	
	TOCObjectPtr typesListHead;						/*		list of all type objects									*/
	TOCObjectPtr typesListTail;						/*		last type object created									*/
};
typedef struct ObjectListHdrs ObjectListHdrs;


/* The following is the "control block" that is used to access an in-memory TOC.  This	*/
/* is privately known only to this file.  Outside this is referred to via an anonymous	*/
/* "void *" pointer.																																		*/

struct TOC {														/* Layout for a TOC "control block":						*/
	Indices 		   toc;										/* 		the root of the toc												*/
	CM_SHORT 	 		 nbrOfIndices;					/* 		nbr of index table levels (table depth)		*/
	CM_ULONG  		 tableSize;							/* 		size of an index table										*/
	CMBoolean	 		 powerOfTwo;						/* 		true ==> can access index by shift/mask		*/
	CM_SHORT 	 		 indexShift;						/* 		shift value to access an index(powerOfTwo)*/
	CM_ULONG  		 indexMask;							/* 		mask value to access an index (powerOfTwo)*/
	ContainerPtr	 container;							/*		ptr to "owning" container control block		*/
	ObjectListHdrs masterLists;						/* 		"master lists" of all objs, props, types	*/
	TOCObjectPtr	 deletedObjects;				/* 		list of all deleted objects								*/
	CM_SHORT			 useCount;							/*		number of distinct users of this TOC			*/
	CMBoolean			 refNumHandling;				/*		true ==> keep deleted object/value refNums*/
};																			/* Outside caller's see this as "void *"				*/
typedef struct TOC TOC, *TOCPtr;


/* The index table accessing code assumes a big endian machine in determining whether		*/
/* shifts and ands can be used as a more efficient access than using divs and mods.  If	*/
/* there is indeed a problem in this area on other machines, set the following macro		*/
/* switch to 1.  Then the general divs and mods will always be used.										*/

#ifndef PortabilityProblems
#define PortabilityProblems 0
#endif


/*--------------------------------*
 | cmCreateTOC - create a new TOC |
 *--------------------------------*
 
 This routine MUST be called before any other TOC routine is called. It is used to set up
 the data needed by the other TOC routines.  A "reference number" is returned to the
 caller who must pass it to ALL the other TOC routines.  Any number of TOC can be created.
 By passing the returned "reference number" to the other routines they will operate on the
 corresponding TOC.
 
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
 
 This function returns the pointer if we can allocate the control block. NULL is returned
 if the allocation fails.
 
 If we weren't being so "flexible" much if the code in here wouldn't be necessary!  We
 could define all the table access variables as #define constants.  What we set up here
 in the control block are the various divide and mod values we apply to the object ID to
 extract the table indicies.
 
 We could potentially be more efficient in our index conversion by shifting and anding
 instead of dividing and moding.  But that's only appropriate if the index table size is
 a power of 2.  We could still use #defines if we took a specific power of 2.
 
 So there are a number of choices we can make: If an index table size is NOT a power of 2
 we must use divides.  If it is a power of 2 we must determine the shift and mask. So,
 given these choices, and to allow some degree in freedom of choosing an "appropriate"
 table size, this routine does the extra work.
 
 cmCreateTOC() takes as its only parameter the index table size (tableSize) and initializes
 the following data in the control block:

 nbrOfIndices	 	Number of index table levels (table depth).  Every access to an ID will
 								have to indirect through nbrOfIndices tables.  The value here is a
								function of the chosen table size.
								
 tableSize			Size of an index table (the passed tableSize parameter). It may be any
 								value, but if a power of 2 is chosen we will determine a shift and mask
								and use them instead of doing divides and mods.
								
 powerOfTwo		-	If tableSize is a power of 2 this will be true and the shift and mask set.
 
 indexShift		-	shift value to access an index (only if powerOfTwo is true).
 
 indexMask		-	mask value to access an index (only if powerOfTwo is true).
 
 An outside caller only sees the pointer to the control block as a void* reference. Thus
 none of these variables are visible outside this file.
 
 Note, there could be some portability problems with this code!  If there are, e.g., the
 target machine is not similar to the MC68XXX family, just set powerOfTwo in the TOC 
 control block to false.  None of the shift/mod stuff will be done and it should work
 anywhere!
*/

void *cmCreateTOC(CM_ULONG tableSize, const struct Container *container)
{
	CM_SHORT 			bit, bitsPerLong;
	CM_ULONG 			maxValue;
	TOCPtr				t;
	
	/* Allocate a TOC "control block" which contains the TOC root pointer and all 				*/
	/* associated access data.  The pointer to the control block will be returned as the	*/
	/* function result.																																		*/

	t = (TOCPtr)CMmalloc(sizeof(TOC));								/* a "control block" is born!				*/
	if (t == NULL) return (NULL);											/* well I thought it was born!			*/
	
	t->container 			= (ContainerPtr)container;			/* keep ptr to "owning" container		*/
	t->toc 					  = NULL;													/* 1st create will fill this in			*/
	t->deletedObjects = NULL;													/* init list of deleted TOC objects	*/
	t->useCount				= 1;														/* 1 user unless cmUseTOC() done		*/
	t->refNumHandling = true;													/* always keep deleted refNums			*/
	
	t->masterLists.objectsListHead		= t->masterLists.objectsListTail		= NULL;
	t->masterLists.propertiesListHead	= t->masterLists.propertiesListTail	= NULL;
	t->masterLists.typesListHead			= t->masterLists.typesListTail			= NULL;
	
	/* Set the size of each index table...																								*/
	
	t->tableSize = tableSize;

	/* Determine the index table depth, i.e., the number of indicies we have to go 				*/
	/* through to get at a value.																													*/
	
	t->nbrOfIndices = 1;															/* there is always at least 1 index	*/
	maxValue = MaxValue;															/* start with largest possible value*/
	while (maxValue /= tableSize) 										/* see how long it takes to reduce	*/
		++t->nbrOfIndices;															/*  the largest ID value to 0				*/
	
	/* If PortabilityProblems is set to 1 then none of the code after here will be 				*/
	/* executed and divides and mods will always be done for index table accessing.				*/
	
	#if PortabilityProblems
	t->powerOfTwo = false;														/* force divs and mods							*/
	return ((void *)t);																/* the other stuff remains garbage	*/
	#endif
	
	/* Determine the bits size of a "unsigned long".  This is done by counting the bits 	*/
	/* in MaxValue which is, in turn, defined from ULONG_MAX in the standard C header 		*/
	/* <limits.h>.  Note, it's not clear this helps with portability, but mods and divs		*/
	/* are used here instead of ands and shifts.																					*/
	
	maxValue = MaxValue;															/* this is 0xFFFFFFFF on MC68XXX's	*/
	bitsPerLong = 0;
	while (maxValue) {
		if ((maxValue % 2) != 0) ++bitsPerLong;					/* ...count them bits!							*/
		maxValue /= 2;
	}
	
	/* Finally determine whether an index table size is a power of 2.  If it is we can		*/
	/* optimize a value's access by using shifts and masks rather than divides and mods.	*/
	/* If indeed a table size is a power of 2 (i.e., there is only 1 bit in tableSize), 	*/
	/* then we set the shift and mask. These values are not used if powerOfTwo is set to 	*/
	/* false.																																							*/
	
	for (t->powerOfTwo = false, bit = 0; bit < bitsPerLong; tableSize /= 2, ++bit) {
		if ((tableSize % 2) != 0) {											/* if we got a bit...								*/
			if (t->powerOfTwo) {													/* ...but's it's not the only bit...*/
				t->powerOfTwo = false;											/* ...then we don't have power of 2 */
				break;																			/* ...too bad (no sense going on)		*/
			}
			t->powerOfTwo = true;													/* ...if it is the 1st bit we see...*/
			t->indexShift = bit;													/* ...assume power of 2, set shift	*/
		}
	}
			
	t->indexMask = t->tableSize - 1;									/* set mask...											*/
	
	return ((void *)t);																/* caller better take care of this!	*/
}


/*------------------------------------------------*
 | cmUseTOC - allow multiple uses of the same TOC |
 *------------------------------------------------*
 
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

void *cmUseTOC(void *toc, struct Container *container)
{
	++((TOCPtr)toc)->useCount;											 		/* register the additiona use			*/
	((TOCPtr)toc)->container = (ContainerPtr)container; /* set the new owning container		*/
	
	return (toc);																				/* caller can now use this TOC		*/
}


/*-----------------------------------------------------------*
 | getIndices - determine the indicies for a given object ID |
 *-----------------------------------------------------------*
 
 This is an internal routine used when we must access an object through its ID.  We must
 decompose the ID into the set of indicies we will apply to the corresponding index tables
 as we indirect through them.  This routine does that decomposition.
 
 The ID is passed in and getIndices() builds an array of indicies in the indices array
 also passed in.  It is assumed that the array is big enough.
 
 The function returns a pointer to the first index in the array.
 
 The array is built from last index to first.  Thus indicies[0] is the last index,
 indicies[1] the next to last, and so on up to indicies[n], the first index to use and
 the one we return a pointer to as the function result.  The n, of course is always equal
 to nbrOfIndices.  If the ID produces less than nbrOfIndices indicies, we pad the remainder
 with zeros.
 
 By returning the pointer to the first index, the indexing of the tables is ready to go
 starting at the root, i.e., toc.  The array pointer is simply decremented to get the next
 index.  This continues until the start of the array is reached.  The table you're in is
 the last table and points to the data.
 
 Note, this is the routine that is sensitive to whether we have index tables with a power
 of 2 size (as determined by tocInitialization() above).  Depending on powerOfTwo, we
 either do shifts and masks or divides and mods.
*/

static CM_USHORT *CM_NEAR getIndices(const TOCPtr t, CMObjectID id, register CM_USHORT *indices)
{
	CM_SHORT i = 0;
	
	/* Extract the indicies used to index into each index table.  They are extracted from	*/
	/* low to high and placed into the indices array in ascending elements...							*/
	
	if (t->powerOfTwo)																/* if we can shift and mask...			*/
		do {																						/* ...what are you waiting for?			*/
			*indices++ = (CM_USHORT)((CM_ULONG)id & t->indexMask);
			id >>= t->indexShift;
			++i;
		} while (id);
	else																							/* if we can't do shifts...					*/
		do {																						/* ...bummer!												*/
			*indices++ = (CM_USHORT)((CM_ULONG)id % t->tableSize);
			id /= t->tableSize;
			++i;
		} while (id);
	
	/* Fill in the remaining high order indices (if any) with zeros. Thus we always end   */
	/* up with nbrOfIndices indices ready to index into their respective index tables.		*/
	
	while (++i <= t->nbrOfIndices) *indices++ = 0;
	
	/* Return pointer to the high order index ready to be used by the caller...						*/
	
	return (indices - 1);															
}


/*--------------------------------------*
 | cmCreateObject - create a TOC object |
 *--------------------------------------*
 
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

TOCObjectPtr cmCreateObject(const void *toc, const CMObjectID objectID, 
														CM_USHORT objectFlags, CMBoolean *dup)
{
	register TOCPtr  t = (TOCPtr)toc;
	register Indices *t1, *t2;
	CM_ULONG 	 			 i, tableSize = t->tableSize;
	CM_USHORT   		 indices[32], *index, x;
	CM_SHORT				 n = t->nbrOfIndices;
	TOCObjectPtr	 	 p;
	ContainerPtr 		 container = t->container;	
	
	/* If this is the 1st time through here, there is no toc root pointer.  So we must		*/
	/* create the root index table to which toc will point from this time on.							*/
	
	if (t->toc == NULL) {																		/* if 1st time...							*/
		t->toc = (Indices)CMmalloc(tableSize * sizeof(void *));	/* ...create root 					*/
		if (t->toc == NULL) return (NULL);
		for (i = 0; i < tableSize; i++) t->toc[i] = NULL;			/* ...init it to all NULLs		*/
	}
	
	/* Convert the ID to indicies and indirect through the indix tables. If along the way,*/
	/* we discover "missing" tables for an index, we create those tables.  When all the		*/
	/* indicies are processed all the tables will be there to get at the object.					*/
	
	index = getIndices(t, objectID, indices);								/* 1st get the indicies				*/
	
	t1 = (Indices *)t->toc;																	/* t1 will be current table		*/
	while (n-- > 1) {																				/* use all the indicies...		*/
		t2 = (Indices *)t1[x = *index--];											/* t2 will be the "next" table*/
																													/* x is index in current table*/
		if (t2 == NULL) {																			/* if we're missing a tbl...	*/
			t2 = (Indices *)CMmalloc(tableSize * sizeof(void *)); /* ...create it							*/
			if (t2 == NULL) return (NULL);
			for (i = 0; i < tableSize; i++) t2[i] = NULL;				/* ...set all entry to NULL		*/
			t1[x] = (Indices)t2;																/* t1[x] points at new table	*/
		}
		
		t1 = t2;																							/* next table becomes current	*/
	}

	/* At this point we are now ready to use the index table corresponding to the last 		*/
	/* index.  Entries in it point to the objects themselves and not other index tables.	*/
	/* For new objects the indexed entry better be NULL or we have a dup.									*/
	
	if ((p = (TOCObjectPtr)t1[*index]) != NULL)							/* if we have a dup...				*/
		*dup = true;																					/* ...tell caller							*/
	else if ((p = (TOCObjectPtr)CMmalloc(sizeof(TOCObject))) != NULL){/* create new object*/
		*dup 								= false;													/* ...tell caller it's new		*/
		t1[*index] 					= (Indices)p;											/* ...set ptr in table				*/
		cmInitList(&p->propertyList);													/* ...set empty property list	*/
		cmInitList(&p->touchedList);													/* ...set empty touched list	*/
		p->nextTouchedObject= NULL;														/* ...not touched here				*/
		p->objectID   			= objectID;												/* ...set the objects' ID			*/
		p->container  			= container;											/* ...set "owning" container	*/
		p->nextObject 			= p->prevObject 			= NULL;			/* ...caller fills links in		*/
		p->nextTypeProperty = p->prevTypeProperty = NULL;			/* ...type/prop sub-chain too	*/
		p->objectFlags			= objectFlags;										/* ...info flags							*/
		p->objectRefCon			= (CMRefCon)NULL;									/* ...refCon									*/
		p->useCount					= 0;															/* ...use count								*/
	}
	
	return (p);																							/* ptr to new or dup object		*/
}


/*-----------------------------------------------------------------------------------*
 | cmUncreateObject - free memory for a single object and remove it from index table |
 *-----------------------------------------------------------------------------------*
 
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

void cmUncreateObject(const void *toc, const CMObjectID id, CMRefCon refCon,
									  	void (*freeAction)(TOCObjectPtr object, CMRefCon refCon))
{
	CM_USHORT 	 						indices[32];
	register CM_SHORT				n;
	register CM_USHORT 			*index;
	register Indices 				*t;
	TOCObjectPtr						object;
	ContainerPtr 						container = ((TOCPtr)toc)->container;	
	
	if (((TOCPtr)toc)->toc == NULL) return;						/* safety														*/
	
	index = getIndices((TOCPtr)toc, id, indices);			/* get the ID's indicies						*/
	
	t = (Indices *)((TOCPtr)toc)->toc;								/* indirect through index tables		*/
	n = ((TOCPtr)toc)->nbrOfIndices;							
	while (n-- > 1) {																	/* loop through nbrOfIndices-1 tbls	*/
		t = (Indices *)t[*index--];											/* apply each index									*/
		if (t == NULL) return;													/* not found if index table missing	*/
	} 
	
	object = (TOCObjectPtr)t[*index];									/* point at found object						*/
	if (freeAction) (*freeAction)(object, refCon);		/* let caller putz with it					*/
	
	CMfree(object);																		/* free the object									*/
	t[*index] = NULL;																	/* remove it from lowest index tbl	*/
}


/*-----------------------------------------------------------------------------------*
 | cmPutObjectInTOC - free memory for a single object and remove it from index table |
 *-----------------------------------------------------------------------------------*

 This routine let put put an object (currently belonging to another TOC) and put it into
 this TOC. If an object of the same ID is already in this TOC, that old object in this
 TOC will be replaced and also returned from the routine so that the caller can deal
 with it.
 
 This routine is used to move objects from one TOC to another.
*/

void cmPutObjectInTOC(const void *toc, TOCObjectPtr object, TOCObjectPtr *replacedObject)
{
	register TOCPtr  t = (TOCPtr)toc;
	register Indices *t1, *t2;
	CM_ULONG			 	 i, tableSize = t->tableSize;
	CM_USHORT   		 indices[32], *index, x;
	CM_SHORT				 n = t->nbrOfIndices;
	ContainerPtr 		 container = ((TOCPtr)toc)->container;	

	/* Convert the ID to indicies and indirect through the indix tables. If along the way,*/
	/* we discover "missing" tables for an index, we create those tables.  When all the		*/
	/* indicies are processed all the tables will be there to get at the object.					*/
	
	index = getIndices(t, object->objectID, indices);				/* 1st get the indicies				*/
	
	t1 = (Indices *)t->toc;																	/* t1 will be current table		*/
	while (n-- > 1) {																				/* use all the indicies...		*/
		t2 = (Indices *)t1[x = *index--];											/* t2 will be the "next" table*/
																													/* x is index in current table*/
		if (t2 == NULL) {																			/* if we're missing a tbl...	*/
			t2 = (Indices *)CMmalloc(tableSize * sizeof(void *)); /* ...create it							*/
			if (t2 == NULL) return;
			for (i = 0; i < tableSize; i++) t2[i] = NULL;				/* ...set all entry to NULL		*/
			t1[x] = (Indices)t2;																/* t1[x] points at new table	*/
		}
		
		t1 = t2;																							/* next table becomes current	*/
	}

	/* At this point we are now ready to use the index table corresponding to the last 		*/
	/* index.  Entries in it point to the objects themselves and not other index tables.	*/
	/* For new objects the indexed entry better be NULL or we have a dup.									*/
	
	if ((TOCObjectPtr)t1[*index])														/* if we have a dup...				*/
		*replacedObject = (TOCObjectPtr)t1[*index];		/* ...tell caller about replaced one  */
	else {
		*replacedObject = NULL;																/* ...tell caller it's new		*/
	}
	t1[*index] 					= (Indices)object;								/* ...set ptr in table				*/
}

/*-------------------------------------------------------------------------*
 | cmDelete1Object - delete a single object and remove it from index table |
 *-------------------------------------------------------------------------*
 
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

void cmDelete1Object(const void *toc, const CMObjectID id, CMRefCon refCon,
									   void (*deleteAction)(TOCObjectPtr object, CMRefCon refCon))
{
	CM_USHORT 	 						indices[32];
	register CM_SHORT				n;
	register CM_USHORT 			*index;
	register Indices 				*t;
	TOCObjectPtr						object;
	ContainerPtr						container;
	
	if (((TOCPtr)toc)->toc == NULL) return;						/* safety														*/
	
	index = getIndices((TOCPtr)toc, id, indices);			/* get the ID's indicies						*/
	
	t = (Indices *)((TOCPtr)toc)->toc;								/* indirect through index tables		*/
	n = ((TOCPtr)toc)->nbrOfIndices;
	while (n-- > 1) {																	/* loop through nbrOfIndices-1 tbls	*/
		t = (Indices *)t[*index--];											/* apply each index									*/
		if (t == NULL) return;													/* not found if index table missing	*/
	} 
	
	object = (TOCObjectPtr)t[*index];									/* point at found object						*/
	if (deleteAction) (*deleteAction)(object, refCon);/* let caller putz with it					*/
	
	if (((TOCPtr)toc)->refNumHandling) {							/* if keep deleted object refNums...*/
		object->nextObject = ((TOCPtr)toc)->deletedObjects;	/* link to deletedObjects				*/
		((TOCPtr)toc)->deletedObjects = object;					/* list rather than freeing it			*/
		object->objectFlags |= DeletedObject;						/* flag object as now deleted				*/
	} else {																					/* if freeing deleted refNums...		*/
		container = ((TOCPtr)toc)->container;						/* ...needed for CMfree()						*/
		CMfree(object);																	/* ...free the object space					*/
	}
	
	t[*index] = NULL;																	/* remove it from lowest index tbl	*/
}

/*-----------------------------------------------*
 | cmFindObject - find the specified object (ID) |
 *-----------------------------------------------*
 
 Given an object ID in a TOC, this function returns a pointer to the corrsponding object
 or NULL if the object does not exist.
*/

TOCObjectPtr cmFindObject(const void *toc, const CMObjectID id)
{
	CM_USHORT 	 						indices[32];
	register CM_SHORT				n;
	register CM_USHORT 			*index;
	register Indices 				*t;

	if (((TOCPtr)toc)->toc == NULL) return (NULL);		/* safety														*/
	
	index = getIndices((TOCPtr)toc, id, indices);			/* get the ID's indicies						*/
	
	t = (Indices *)((TOCPtr)toc)->toc;								/* indirect through index tables		*/
	n = ((TOCPtr)toc)->nbrOfIndices;								
	while (n-- > 1) {																	/* loop through nbrOfIndices-1 tbls	*/
		t = (Indices *)t[*index--];											/* apply each index									*/
		if (t == NULL) return (NULL);										/* not found if index table missing	*/
	} 
	
	return ((TOCObjectPtr)(t[*index]));								/* return object ptr or NULL				*/
}


/*---------------------------------------------------------------------------------*
 | scanIndexTable - scan all entries of an index table acting on each object entry |
 *---------------------------------------------------------------------------------*
 
 This routine is the "guts" of cmForEachObject() used to access all objects in the TOC and
 perform some action on them.  A pointer to a index table is passed in t.  The index table
 level is passed in indexLevel (i.e., a value from 1 to nbrOfIndices, with 1 corresponding
 to the first index).  If t corresponds to the deepest table, i.e., the one that contains
 object pointers, we call the action to do something with the object.
 
 The table pointer and index level are of no concern to outside callers.  That is why
 cmForEachObject() is a cover routine.  It set the initial t and indexLevel (toc and 1).
 It is also needed to start the recursion.
 
 This routine is recursive as it decends down through the tables.  It will access the
 objects in ascending order and stop on the first object that has an ID greater than the
 endingID.  The initial starting point in each index table is determined from the index
 parameter.  It points into an array of starting indicies for each index table level.
 Once used, the array element is set to 0 to use all entries in all following tables at
 that level.
 
 See cmForEachObject() for details on the refCon and action parameters.
*/

static void CM_NEAR scanIndexTable(register const TOCPtr toc, CM_USHORT *index,
																	 CMObjectID endingID, const Indices *t,
																	 const CM_SHORT indexLevel, CMRefCon refCon,
																	 void (*action)(TOCObjectPtr object, CMRefCon refCon))
{
	CM_ULONG			i;
	TOCObjectPtr 	p;
	ContainerPtr 	container;

	if (indexLevel < toc->nbrOfIndices)													/* if we have index tbl...*/
		for (i = *index, *index-- = 0; i < toc->tableSize; i++) {	/* ...look at each entry	*/
			if (t[i]) 																							/* ...if non-NULL entry...*/
				scanIndexTable(toc, index, endingID, (Indices *)t[i], (CM_SHORT)(indexLevel+1), refCon, action);
		}
	else 																												/* if last index table...	*/
		for (i = *index, *index = 0; i < toc->tableSize; i++)			/* ...do each entry				*/
			if ((p = (TOCObjectPtr)(t[i])) != NULL) 								/* ...if non-NULL entry...*/
				if (p->objectID <= endingID)													/* ...and not at max ID...*/
					(*action)(p, refCon);																/* ...process the action	*/
				else {																								/* ...if at ending ID...	*/
					container = toc->container;
					SessionSuccess = true;															/* ...return success			*/
					AbortForEachObject(1);															/* ...abort walk					*/
				}
}


/*-------------------------------------------------*
 | cmForEachObject - do some action on each object |
 *-------------------------------------------------*
 
 Do (call) the specified action for each object in the specified TOC with object ID's
 greater than or equal to the startingID and less than or equal to endingID.  The pointer
 to each TOC object is passed to the action routine along with a "refCon" which the caller
 can use as a communication facility to convey additional info to the action routine.  0
 is returned to indicate successful completion.  Use the AbortForEachObject(x) (a macro)
 in the action routine to abort the interator.  The "x" should be a positive integer which
 is returned from cmForEachObject() and it should not be 0.
*/

int cmForEachObject(const void *toc, CMObjectID startingID, CMObjectID endingID, 
									 	CMRefCon refCon, void (*action)(TOCObjectPtr object, CMRefCon refCon))
{
	CM_SHORT 			 n;
	CM_USHORT 		 *index, indices[32];
	ContainerPtr 	 container = ((TOCPtr)toc)->container;

	if (((TOCPtr)toc)->toc) {													/* scanIndexTable() does everything	*/
		if (startingID == ALLOBJECTS) {									/* if frocessing ALL objects...			*/
			n = ((TOCPtr)toc)->nbrOfIndices;							/* ...init indices array to all 0's	*/
			index = indices - 1;													/* 		the 0 index will cause all 		*/
			while (n-- > 0) *++index = 0;									/*		index table entrys to be used	*/
		} else																					/* if explicit starting ID...				*/
			index = getIndices((TOCPtr)toc, startingID, indices); /* ...init indices from ID	*/

		SessionSuccess = false;													/* assume walk fails with longjmp		*/

		if (setjmp(SESSION->cmForEachObjectEnv))				/* if longjmp taken...							*/
			return (SessionSuccess ? 0 : 1);							/* ...failure unless we hit endingID*/ 
		
		scanIndexTable((TOCPtr)toc, index, endingID, (Indices *)((TOCPtr)toc)->toc, 1, refCon, action);
	}
	
	return (0);																				/* success													*/
}


/*------------------------------------------------------------*
 | freeAllObjects - actual implementation for freeEntireTOC() |
 *------------------------------------------------------------*
 
 This routine is the "guts" of freeEntireTOC() used to access all objects in the TOC and
 free them.  A pointer to a index table is passed in t.  The index table level is passed
 in indexLevel (i.e., a value from 1 to nbrOfIndices, with 1 corresponding to the first
 index).  If t corresponds to the deepest table, i.e., the one that contains object 
 pointers, we free the object.
 
 The table pointer and index level are of no concern to outside callers.  That is why
 freeEntireTOC() is a cover routine.  It set the initial t and indexLevel (toc and 1).
 
 This routine is recursive as it descends down through the tables.  It will access the
 objects free them.  As we ascend back up the index tables themselves are freed.  By the
 time we're done, the entire TOC will cease to exist!
 
 See destroyTOC() for details on the refCon and deleteAction parameters.
*/

static void CM_NEAR freeAllObjects(register const TOCPtr toc, const Indices *t,
																	 const CM_SHORT indexLevel, CMRefCon refCon, CMBoolean objectToo,
																	 void (*deleteAction)(TOCObjectPtr object, CMRefCon refCon))
{
	CM_ULONG			i;
	TOCObjectPtr 	object;
	ContainerPtr  container = toc->container;

	if (indexLevel < toc->nbrOfIndices)								/* if we have index table...				*/
		for (i = 0; i < toc->tableSize; i++) {					/* ...look at each entry...					*/
			if (t[i]) 																		/* ...if non-NULL entry...					*/
				freeAllObjects(toc, (Indices *)t[i], (CM_SHORT)(indexLevel+1), refCon, objectToo, deleteAction);
		}
	else																							/* if last index table...						*/
		for (i = 0; i < toc->tableSize; i++)						/* ...we still look at each entry...*/
			if ((object = (TOCObjectPtr)(t[i])) != NULL) {/* ...if non-NULL entry...					*/
				if (objectToo) {															/* only if we want to free object		*/
					if (deleteAction)														/* ...and if caller wants a crack...*/
						(*deleteAction)(object, refCon);					/* ...let caller do his thing				*/
					CMfree(object);															/* ...free the object itself				*/
				}
			}
			
	CMfree(t);																				/* free the index table too					*/
}


/*-----------------------------------------------------------------------*
 | cmDestroyTOC - free all TOC objects and their associated index tables |
 *-----------------------------------------------------------------------*
 
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

void cmDestroyTOC(void **toc, CMRefCon refCon, CMBoolean objectToo,
									void (*deleteAction)(TOCObjectPtr object, CMRefCon refCon))
{
	TOCObjectPtr theObject, nextObject;
	ContainerPtr container = ((TOCPtr)*toc)->container;
	
	if (--((TOCPtr)*toc)->useCount > 0) return;				/* don't destroy if multiple users	*/
	
	if (((TOCPtr)*toc)->toc) 													/* bye, bye TOC...									*/
		freeAllObjects((TOCPtr)*toc, (Indices *)((TOCPtr)*toc)->toc, 1, refCon, objectToo, deleteAction);
	
	/* Free all objects that we removed from the TOC and placed on deletedObjects list. 	*/
	/* We assume here that such objects placed on this list have no substructures tied to	*/
	/* them.  Thus we DON'T call the action routine for each object freed here.						*/
	
	theObject = ((TOCPtr)*toc)->deletedObjects;				/* these kept around to stop user		*/
																										/* from using dangling pointers			*/
	while (theObject) {																/* go through entire list...				*/
		nextObject = theObject->nextObject;
		cmDeleteTouchedList(theObject, container);			/* free touched list (if any)				*/
		CMfree(theObject);															/* that's end of this object				*/
		theObject = nextObject;
	}

	CMfree(*toc);																			/* bye, bye TOC control block				*/
	*(TOCPtr **)toc = NULL;														/* kill caller's toc pointer				*/
}


/*-----------------------------------------------------------------------------*
 | cmLinkObject - link an object into the master object chains for a container |
 *-----------------------------------------------------------------------------*
 
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
 
 Note, the algorithm here is identical to the generic list processing routines.  But we
 don't have total freedom of where to put the link fields since we have more than one set
 of links!  So we reinvent the wheel and do the code explicitly here (three times in
 fact).
*/

TOCObjectPtr cmLinkObject(const void *toc, TOCObjectPtr theObject, 
													TOCObjectPtr afterThisObject)
{
	/* We must not link the object more that once to its master lists. We could be calling*/
	/* this routine as a result of defining additional properties and values for an 			*/
	/* existing object.  So we prevent the multiple attempts by flagging the object the		*/
	/* first time we actually do link it.  Then we just exits for additional attempts.		*/
	
	if ((theObject->objectFlags & LinkedObject) != 0) return (theObject);
		
	/* Insert in the "all objects" chain...																								*/
	
	if (afterThisObject == NULL) {														/* append to end of list...	*/
		theObject->nextObject = NULL;
		theObject->prevObject = ((TOCPtr)toc)->masterLists.objectsListTail;
		if (((TOCPtr)toc)->masterLists.objectsListTail == NULL)
			((TOCPtr)toc)->masterLists.objectsListHead = theObject;
		else
			((TOCPtr)toc)->masterLists.objectsListTail->nextObject = theObject;
		((TOCPtr)toc)->masterLists.objectsListTail = theObject;
	} else {																									/* insert after...					*/
		theObject->prevObject = afterThisObject;
		theObject->nextObject = afterThisObject->nextObject;
		if (afterThisObject->nextObject)
			afterThisObject->nextObject->prevObject = theObject;
		else
			((TOCPtr)toc)->masterLists.objectsListTail = theObject;
		afterThisObject->nextObject = theObject;
	}
	
	/* Insert in the "properties" or "type" list if it's one of those types...						*/
	
	if (theObject->objectFlags & PropertyObject) {						/* property...							*/
		if (afterThisObject == NULL) {													/* 		append...							*/
			theObject->nextTypeProperty = NULL;
			theObject->prevTypeProperty = ((TOCPtr)toc)->masterLists.propertiesListTail;
			if (((TOCPtr)toc)->masterLists.propertiesListTail == NULL)
				((TOCPtr)toc)->masterLists.propertiesListHead = theObject;
			else
				((TOCPtr)toc)->masterLists.propertiesListTail->nextTypeProperty = theObject;
			((TOCPtr)toc)->masterLists.propertiesListTail = theObject;
		} else {																								/*		insert after...				*/
			theObject->prevTypeProperty = afterThisObject;
			theObject->nextTypeProperty = afterThisObject->nextTypeProperty;
			if (afterThisObject->nextTypeProperty)
				afterThisObject->nextTypeProperty->prevTypeProperty = theObject;
			else
				((TOCPtr)toc)->masterLists.propertiesListTail = theObject;
			afterThisObject->nextTypeProperty = theObject;
		}
	} else if (theObject->objectFlags & TypeObject) {					/* type...									*/
		if (afterThisObject == NULL) {													/*		append...							*/
			theObject->nextTypeProperty = NULL;
			theObject->prevTypeProperty = ((TOCPtr)toc)->masterLists.typesListTail;
			if (((TOCPtr)toc)->masterLists.typesListTail == NULL)
				((TOCPtr)toc)->masterLists.typesListHead = theObject;
			else
				((TOCPtr)toc)->masterLists.typesListTail->nextTypeProperty = theObject;
			((TOCPtr)toc)->masterLists.typesListTail = theObject;
		} else {																								/*		insert after...				*/
			theObject->prevTypeProperty = afterThisObject;
			theObject->nextTypeProperty = afterThisObject->nextTypeProperty;
			if (afterThisObject->nextTypeProperty)
				afterThisObject->nextTypeProperty->prevTypeProperty = theObject;
			else
				((TOCPtr)toc)->masterLists.typesListTail = theObject;
			afterThisObject->nextTypeProperty = theObject;
		}
	}
	
	/* Flag the object so we never try to do this again!  See start of code above.		*/
	
	theObject->objectFlags |= LinkedObject;
	
	return (theObject);
}


/*---------------------------------------------------------------------------------*
 | cmUnlinkObject - unlink an object from the master object chains for a container |
 *---------------------------------------------------------------------------------*
 
 This routine is the inverse to cmLinkObject().  It unlinks the specified object from the
 master object chains whose head and tail pointers are contained in the specified TOC.
 The function returns the input object pointer as its result.
 
 The object is always deleted from the chain of all objects.  It is also deleted from the
 property or type chain if the objectFlags field contained in the object itself indicates
 it is a property or type (see cmLinkObject() above for further details about the chains).
*/

TOCObjectPtr cmUnlinkObject(const void *toc, TOCObjectPtr theObject)
{
	/* We must not unlink the object more that once from its master lists...							*/
	
	if ((theObject->objectFlags & LinkedObject) == 0) return (theObject);
		
	/* Delete from the "all objects" chain...																							*/

	if (theObject->prevObject)
		theObject->prevObject->nextObject = theObject->nextObject;
	else 																													/* delete head 					*/
		((TOCPtr)toc)->masterLists.objectsListHead = theObject->nextObject;
	if (theObject->nextObject)
		theObject->nextObject->prevObject = theObject->prevObject;
	else 																													/* delete tail 					*/
		((TOCPtr)toc)->masterLists.objectsListTail = theObject->prevObject;
	
	/* Delete from the "properties" or "type" list if it's one of those types...					*/
	
	if (theObject->objectFlags & PropertyObject) {
		if (theObject->prevObject)
			theObject->prevObject->nextObject = theObject->nextObject;
		else 																												/* delete head 					*/
			((TOCPtr)toc)->masterLists.propertiesListHead = theObject->nextObject;
		if (theObject->nextObject)
			theObject->nextObject->prevObject = theObject->prevObject;
		else 																												/* delete tail 					*/
			((TOCPtr)toc)->masterLists.propertiesListTail = theObject->prevObject;
	} else if (theObject->objectFlags & TypeObject) {
		if (theObject->prevObject)
			theObject->prevObject->nextObject = theObject->nextObject;
		else 																												/* delete head 					*/
			((TOCPtr)toc)->masterLists.typesListHead = theObject->nextObject;
		if (theObject->nextObject)
			theObject->nextObject->prevObject = theObject->prevObject;
		else 																												/* delete tail 					*/
			((TOCPtr)toc)->masterLists.typesListTail = theObject->prevObject;
	}
	
	/* Remove the "has been linked" flag for so we can't unlink it twice...								*/
	
	theObject->objectFlags &= ~LinkedObject;

	return (theObject);
}


/*----------------------------------------------------------------*
 | cmGetMasterListHead - get head of specified master object list |
 *----------------------------------------------------------------*
 
 This routine returns the head of one of the master chain lists in a TOC depending on 
 the objectKindFlag.  This flag should be one of the standard object flags; ObjectObject,
 PropertyObject, or TypeObject.  The respective chain head will be returned as the function
 result.
 
 These chain heads are used by the CMGetNextObject(), CMGetNextProperty(), and
 CMGetNextType() routines to start them off.
*/

TOCObjectPtr cmGetMasterListHead(const void *toc, CM_USHORT objectKindFlag)
{
	if (objectKindFlag == ObjectObject) 
		return (((TOCPtr)toc)->masterLists.objectsListHead);
	
	if (objectKindFlag == PropertyObject)
		return (((TOCPtr)toc)->masterLists.propertiesListHead);
		
	return (((TOCPtr)toc)->masterLists.typesListHead);
}

/*----------------------------------------------------------------*
 | cmGetMasterListTail - get tail of specified master object list |
 *----------------------------------------------------------------*
 
 This routine returns the tail of one of the master chain lists in a TOC depending on 
 the objectKindFlag.  This flag should be one of the standard object flags; ObjectObject,
 PropertyObject, or TypeObject.  The respective chain head will be returned as the function
 result.
 
 These chain tails are used by the CMGetPrevObject(), CMGetPrevProperty(), and
 CMGetPrevType() routines to start them off.
*/

TOCObjectPtr cmGetMasterListTail(const void *toc, CM_USHORT objectKindFlag)
{
	if (objectKindFlag == ObjectObject) 
		return (((TOCPtr)toc)->masterLists.objectsListTail);
	
	if (objectKindFlag == PropertyObject)
		return (((TOCPtr)toc)->masterLists.propertiesListTail);
		
	return (((TOCPtr)toc)->masterLists.typesListTail);
}

/*--------------------------------------------------------------------------*
 | cmChangeRefNumHandling - change status of deleted user "refNum" handling |
 *--------------------------------------------------------------------------*
 
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

void cmChangeRefNumHandling(const void *toc, CMBoolean setting)
{
	((TOCPtr)toc)->refNumHandling = setting;		/* ...set new state												*/
}


/*---------------------------------------------------------------------------------*
 | cmKeepDeletedRefNums - return current status of the handling of deleted refNums |
 *---------------------------------------------------------------------------------*
 
 As described above, this is used by all outside callers to see if a refNum is to be saved
 on its deleted list.  The only thing outside callers can possibly be interested in is
 for deleted values and whether the value headers are to be saved on the deleted values
 list.
*/

CMBoolean cmKeepDeletedRefNums(const void *toc)
{
	return (((TOCPtr)toc)->refNumHandling);			/* does this really need a comment?				*/
}
														
														  CM_END_CFUNCTIONS
