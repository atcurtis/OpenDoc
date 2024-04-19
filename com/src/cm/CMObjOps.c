/* @(#)Z 1.7 com/src/cm/CMObjOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:48 (96/10/29 09:16:50) */
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
	File:		 CMObjOps.c  

	Contains:	Container Manager Object Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>	 2/15/95	EL		1182275: Move CMKeepObject to CMRefOps
									because CMKeepObject depends on references.
		 <3>	 12/9/94	EL		#1182275 Now we garbage collect property
													and type object, so need to track their
													usage.
		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <2>	 3/17/94	EL		Add CMKeepObject call. #1149983
		 <1>	  2/3/94	EL		first checked in
		 <8>	 1/25/94	EL		Undo the last change.
		 <7>	 1/21/94	EL		Disable checking of useCount in
													CMDeleteObject until after alpha.
		 <6>	11/10/93	EL		Add CMCountProperties call.
		 <5>	 10/6/93	EL		Rename CMGetAdjacentXXXX to getAdjacentXXXX
													and static near.
		 <4>	 10/4/93	EL		Add CMGetPrevObject and CMGetPrevObjectWithProperty, 
		 											replace CMGetPrevObjectProperty
		 <3>	 9/27/93	VL		Added CMGetPrevObjectProperty.
		 <2>	  9/8/93	VL		Added GetObject and GetObjectID.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<<  CMObjOps.c   >>>                           |
 |                                                                           |
 |                    Container Manager Object Operations                    |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/15/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 All Container Manager API routines dealing with objects (not their properties, types, 
 or values) are handled in this file.  Properties and types are also objects, but they 
 are treated separately.
 
 An object is defined in terms of its properties.  The properties are defined in terms of
 the typed values.  Here we mainly create and access just the objects.  Internally they
 are kept distince by object ID's.  From the API user's point of view objects are 
 manipulated through refNums.
*/


#include <stddef.h>
#include <string.h>
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
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __UPDATING__
#include "Update.h"  
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
#pragma segment CMObjectOps
#endif
																	

struct GetGNameComArea {					/* CMGetGlobalName() object walking comm area layout:	*/
	TOCValuePtr 	 theValue;				/*		the first (should be only) global name value		*/
	CM_USHORT			 nbrOfGlobalNames;/*		count of nbr of global names found in object		*/
};
typedef struct GetGNameComArea GetGNameComArea, *GetGNameComAreaPtr;


/*-----------------------------------*
 | CMNewObject - create a new object |
 *-----------------------------------*
 
 A new object is created in the specified container and its refNum returned.  At this
 point the object is considered as "undefined".  It has no properties and thus no values.
*/

CMObject CM_FIXEDARGS CMNewObject(CMContainer targetContainer)
{
	TOCObjectPtr theObject;
	ContainerPtr container = (ContainerPtr)targetContainer;
	
	NOPifNotInitialized(NULL);												/* NOP if not initialized!					*/
	
	/* The object is created as "undefined".  CMNewValue() will mark it defined because 	*/
	/* at that time it will get a property and a value.																		*/
	
	theObject = cmDefineObject(container, container->nextUserObjectID, 0, 0, NULL, 0, 0, 
											 			 UndefinedObject | ObjectObject, NULL);

	if (theObject) {
		theObject->useCount = 1;												/* initial use of this object				*/
		IncrementObjectID(container->nextUserObjectID);	/* set to use next available user ID*/
	}
	
	return ((CMObject)theObject);
}


/*----------------------------------------------------------------------------------*
 | getAdjacentObject - get adjacent object in the container by increasing object ID |
 *----------------------------------------------------------------------------------*
 
 This routine returns the refNum for the next/previous object in the container 
 following/preceding currObject. If currObject is NULL, the refNum for the first/last
 object is returned.  If currObject is not NULL, the object with the next highest ID 
 next lowerest is returned.  NULL is returned if there are no more objects following/preceding
 currObject.
 
 currObject is generally a refNum previously returned from this routine.  Successive calls
 to this routine will thus yield all the objects in the container.
*/

static CMObject CM_NEAR getAdjacentObject(CMContainer targetContainer, 
																					CMObject currObject,
																					Boolean forward)
{
	TOCObjectPtr theObject;
	ContainerPtr container = (ContainerPtr)targetContainer;
	
	NOPifNotInitialized(NULL);												/* NOP if not initialized!					*/
	
	if (currObject == NULL) {													/* NULL ==> use head/tail of list				*/
		if (forward)
			theObject = (TOCObjectPtr)cmGetMasterListHead(container->toc, ObjectObject);
		else
			theObject = (TOCObjectPtr)cmGetMasterListTail(container->toc, ObjectObject);
		if (theObject) ++theObject->useCount;						/* bump use count for next object		*/
		return ((CMObject)theObject);
	}
	
	ExitIfBadObject(currObject, NULL);								/* validate currObject							*/
	
	if (container->targetContainer != ((TOCObjectPtr)currObject)->container->targetContainer) {
		ERROR3(CM_err_BadContainer, "object", CONTAINERNAMEx(((TOCObjectPtr)currObject)->container), CONTAINERNAMEx(container));
		return (NULL);
	}
	
	/* In our implementation the objects are already chained in sorted order.  So all we 	*/
	/* have to do is follow the chain (or is it a yellow brick road?).										*/
	
	if (forward)
		theObject = ((TOCObjectPtr)currObject)->nextObject;
	else
		theObject = ((TOCObjectPtr)currObject)->prevObject;
	if (theObject) ++theObject->useCount;							/* bump use count for adjacent object	*/
	
	return ((CMObject)theObject);											/* return NULL or next object refNum*/
}

/*----------------------------------------------------------------------------*
 | CMGetNextObject - get next object in the container by increasing object ID |
 *----------------------------------------------------------------------------*

 Just call getAdjacentObject with forward = true.
*/

CMObject CM_FIXEDARGS CMGetNextObject(CMContainer targetContainer, CMObject currObject)

{
	return getAdjacentObject(targetContainer, currObject, true);
}

/*--------------------------------------------------------------------------------*
 | CMGetPrevObject - get previous object in the container by increasing object ID |
 *--------------------------------------------------------------------------------*

 Just call getAdjacentObject with forward = false.
*/

CMObject CM_FIXEDARGS CMGetPrevObject(CMContainer targetContainer, CMObject currObject)

{
	return getAdjacentObject(targetContainer, currObject, false);
}

/*--------------------------------------------------------------------------------*
 | getAdjacentObjectProperty - get the adjacent property for the specified object |
 *--------------------------------------------------------------------------------*
 
 This routine returns the refNum for the next/prev property defined for the given object.  If
 currProperty is NULL, the refNum for the first/last property for the object is returned.  If 
 it is not NULL, the next/prev property refNum following currProperty is returned.
 
 currProperty is generally a refNum previously returned from this routine. Successive
 calls to this routine will thus yield all the properties for the given object.
*/

static CMProperty CM_NEAR getAdjacentObjectProperty(CMObject theObject, CMProperty currProperty, 
																										Boolean forward)
{
	TOCObjectPtr 	 theAdjacentObject;
	TOCPropertyPtr theProperty;
	ContainerPtr 	 container;
	
	ExitIfBadObject(theObject, NULL);									/* validate theObject								*/

	container = ((TOCObjectPtr)theObject)->container;
	
	/* If currProperty is NULL we return the object with the property ID taken from the		*/
	/* first/last property on theObject.																									*/
	
	if (currProperty == NULL)	{												/* NULL ==> use head of list				*/
		if (forward)
			theProperty = (TOCPropertyPtr)cmGetListHead(&((TOCObjectPtr)theObject)->propertyList);
		else
			theProperty = (TOCPropertyPtr)cmGetListTail(&((TOCObjectPtr)theObject)->propertyList);

		if (theProperty == NULL)
			return (NULL);
		else

			return ((CMProperty)cmFindObject(container->toc, theProperty->propertyID));
	}
	
	ExitIfBadProperty(currProperty, NULL);						/* validate currProperty						*/
	
	if (container->targetContainer != ((TOCObjectPtr)currProperty)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)currProperty)->container));
		return (NULL);
	}
	
	/* Here we ant to find the next/prev property for the object.  Since currProperty is 	*/
	/* not NULL, it points to an object because it's a refNum.  Remember it is not a 			*/
	/* TOCPropertyPtr.  What we have to do is use the property ID to find the property		*/
	/* (a real TOCPropertyPtr) we found the last time this routine was called.  Then we		*/
	/* simply get the next/prev on the list to get the next/prev property ID.							*/
	
	theProperty = cmGetObjectProperty((TOCObjectPtr)theObject, /* find prop ID for object	*/
																		((TOCObjectPtr)currProperty)->objectID);
	if (theProperty) {																/* if we found ID on the list...		*/
		if (forward)
			theProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty); /* ...get next			*/
		else
			theProperty = (TOCPropertyPtr)cmGetPrevListCell(theProperty); /* ...get previous	*/
		if (theProperty) {															/* if there's a next property...		*/
			theAdjacentObject = cmFindObject(container->toc, theProperty->propertyID); /* get obj	*/
			if (theAdjacentObject) ++theAdjacentObject->useCount;	/* bump its use count				*/
			return ((CMProperty)theAdjacentObject);				/* give caller the property object	*/
		}
	}
	
	return (NULL);
}

/*--------------------------------------------------------------------------*
 | CMGetNextObjectProperty - get the next property for the specified object |
 *--------------------------------------------------------------------------*
 
 Just call getAdjacentObjectProperty with forward = true.
*/

CMProperty CM_FIXEDARGS CMGetNextObjectProperty(CMObject theObject, CMProperty currProperty)
{
	return getAdjacentObjectProperty(theObject, currProperty, true);
}

/*------------------------------------------------------------------------------*
 | CMGetPrevObjectProperty - get the previous property for the specified object |
 *------------------------------------------------------------------------------*
 
 Just call getAdjacentObjectProperty with forward = false.
*/

CMProperty CM_FIXEDARGS CMGetPrevObjectProperty(CMObject theObject, CMProperty currProperty)
{
	return getAdjacentObjectProperty(theObject, currProperty, false);
}

/*-----------------------------------------------------------------------------------------*
 | getAdjacentObjectWithProperty - get the adjacent object that has the specified property |
 *-----------------------------------------------------------------------------------------*
 
 This routine returns the refNum for the next/previous object in the container that has the 
 given property.  If currObject is NULL, the search starts with the first/last object in the 
 container.  If it is not NULL the search starts with the object immediately following/preceding
 currObject.  If there is no next/previous object with the given property, NULL is returned. 
 
 currObject is generally a refNum previously returned from this routine. Successive calls
 to this routine will thus yield all the objects with the given property.
 */

static CMObject CM_NEAR getAdjacentObjectWithProperty(CMContainer targetContainer,
																									CMObject currObject, CMProperty property,
																									Boolean forward)
{
	ContainerPtr container = (ContainerPtr)targetContainer;
	TOCObjectPtr theObject = (TOCObjectPtr)currObject;
	
	NOPifNotInitialized(NULL);													/* NOP if not initialized!				*/
	
	/* If we are given a non-NULL currObject, then start up the search with the next 			*/
	/* object following that.  If currObject is NULL we start our search with the first		*/
	/* object.																																						*/
	
	if (forward) {
		if (theObject != NULL) {														/* if we have a starting object...*/
			ExitIfBadObject(theObject, NULL);									/* ...validate theObject					*/
			theObject = theObject->nextObject;								/* ...get next object							*/
			if (theObject == NULL) return (NULL);							/* return NULL if no more objects	*/
		} 
		else
			theObject = cmGetMasterListHead(container->toc, ObjectObject);/* get first object		*/
	} 
	else {
		if (theObject != NULL) {														/* if we have a starting object...*/
			ExitIfBadObject(theObject, NULL);									/* ...validate theObject					*/
			theObject = theObject->prevObject;								/* ...get prev object							*/
			if (theObject == NULL) return (NULL);							/* return NULL if no more objects	*/
		} 
		else
			theObject = cmGetMasterListTail(container->toc, ObjectObject);/* get first object		*/
	}

	ExitIfBadProperty(property, NULL);									/* ...validate property						*/
	
	if (container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)property)->container));
		return (NULL);
	}
	
	/* Starting with the first object or the next one after currObject we check each 			*/
	/* object on the object chain to see if it has a property with the property ID of the */
	/* given property object (descriptor). If it is found, the object pointer is returned.*/
	/* If not, NULL is returned. This is easy to do since we already got a routine named	*/
	/* that scans an object for a property ID.																						*/
	
	while (theObject) {
		if (cmGetObjectProperty(theObject, ((TOCObjectPtr)property)->objectID) != NULL) {
			++theObject->useCount;													/* if found, bump obj's use count	*/
			return ((CMObject)theObject);										/* give refNum back to user				*/
		}
		if (forward)
			theObject = theObject->nextObject;								/* not found, keep looking				*/
		else
			theObject = theObject->prevObject;								/* not found, keep looking				*/
	}

	return (NULL);																			/* not found 											*/
}

/*-----------------------------------------------------------------------------------*
 | CMGetNextObjectWithProperty - get the next object that has the specified property |
 *-----------------------------------------------------------------------------------*
 
 Just call getAdjacentObjectWithProperty with forward = true.
 */

CMObject CM_FIXEDARGS CMGetNextObjectWithProperty(CMContainer targetContainer,
																									CMObject currObject, CMProperty property)
{
	return getAdjacentObjectWithProperty(targetContainer, currObject, property, true);
}

/*-------------------------------------------------------------------------------------------*
 | CMGetPrevObjectWithProperty - get the previous object that has the specified property |
 *-------------------------------------------------------------------------------------------*
 
 Just call getAdjacentObjectWithProperty with forward = false.
 */

CMObject CM_FIXEDARGS CMGetPrevObjectWithProperty(CMContainer targetContainer,
																									CMObject currObject, CMProperty property)
{
	return getAdjacentObjectWithProperty(targetContainer, currObject, property, false);
}

#if 0
/*--------------------------------------------------------*
 | CMGetReferenceData - create a "reference" to an object |
 *--------------------------------------------------------*
 
 Creates a "reference" to an object (refNum), referencedObject, as data, theReferenceData,
 to be written as (part of) the value data for the specified value.  The function returns
 the input pointer to theReferenceData.  The caller should write the data to the value
 possibly along with other data.  The size of this data is determined by the size of the
 CMReference type, i.e., sizeof(CMReference).
 
 The returned data is in a form suitable for retrival by CMGetReferencedObject() to 
 convert the data back to the object refNum.
*/

CMReference CM_PTR * CM_FIXEDARGS CMGetReferenceData(CMValue value,
																										 CMObject referencedObject,
																 			 			   			 CMReference CM_PTR theReferenceData)
{
	ContainerPtr container;
	
	ExitIfBadValue(value, NULL);										/* validate value											*/
	ExitIfBadObject(referencedObject, NULL);				/* validate referencedObject					*/

	container = ((TOCValueHdrPtr)value)->container;
	
	if (container->targetContainer != ((TOCObjectPtr)referencedObject)->container->targetContainer) {
		ERROR2(CM_err_CantReference, CONTAINERNAMEx(container),
																 CONTAINERNAMEx(((TOCObjectPtr)referencedObject)->container));
		return (NULL);
	}
	
	/*                         -- "DANGER WILL ROBINSON" -- 															*/
	
	/* A CMObjectID for some hardware implementations may NOT be exactly 4 bytes (but it	*/
	/* better be at least 4 bytes).  A user's "reference" data is always 4 bytes because	*/
	/* we define such data here as an object ID.  We must take care to convert the 				*/
	/* internal, i.e., hardware, representation to that of a container representation. By	*/
	/* share "luck" (yea right, want to buy a bridge?) we just happen to have a handler 	*/
	/* that does this conversion.																													*/
	
	CMformatData(container, theReferenceData, 4, &((TOCObjectPtr)referencedObject)->objectID);
	
	return ((CMReference *)theReferenceData);				/* return the ptr to the "reference"	*/
}


/*-----------------------------------------------------------------------*
 | CMGetReferencedObject - retrieve a object (refNum) from a "reference" |
 *-----------------------------------------------------------------------*
 
 Converts an object "reference", created by CMGetReferenceData(), back to an object refNum,
 from the data supplied by the specified value.
*/

CMObject CM_FIXEDARGS CMGetReferencedObject(CMValue value, CMReference CM_PTR theReferenceData)
{
	ContainerPtr container;
	CMObjectID	 objectID;
	TOCObjectPtr theObject;
	CM_CHAR 		 idStr[15];

	ExitIfBadValue(value, NULL);															/* validate value						*/
	
	container = ((TOCValueHdrPtr)value)->container;
	
	/* Use handler to convert external representation back to internal representation. 		*/
	/* See comments in CMGetReferenceData() why we do this.																*/
	
	CMextractData(container, theReferenceData, 4, &objectID);	/* get "reference" object ID*/
	
	theObject = cmFindObject(container->toc, objectID);				/* get the "refNum"					*/
	if (theObject == NULL)																		/* huh?											*/
		ERROR2(CM_err_UndefReference, cmltostr(objectID, 1, false, idStr), CONTAINERNAME);
	
	++theObject->useCount;																		/* count this use of the obj*/
	
	return ((CMObject)theObject);															/* give back the refNum			*/
}
#endif


/*------------------------------------------------------------------------*
 | CMGetObjectContainer - get the container refNum for one of its objects |
 *------------------------------------------------------------------------*
 
 The refNum for the container which contains the specified object is returned.
*/

CMContainer CM_FIXEDARGS CMGetObjectContainer(CMObject theObject)
{
	ExitIfBadObject(theObject, NULL);								/* validate theObject									*/

	return ((CMContainer)(((TOCObjectPtr)theObject)->container));
}


/*---------------------------------------------------------*
 | checkValue - check a value to see if it's a global name |
 *---------------------------------------------------------*
 
 This is a cmWalkObject() action routine initiated by CMGetGlobalName() below to find the
 global name value in an object.  The "refCon" is a pointer to a GetGNameComArea
 communication area where we will save the value pointer.  We also count the number of
 global names seen in the object.  It shoould be 1.  But we walk the entire object just to
 be sure.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmWalkObject().
*/
 
static TOCWalkReturns checkValue(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon)
{
	GetGNameComAreaPtr g = (GetGNameComAreaPtr)refCon;
	ContainerPtr			 unused = container;
	
	if (theValue->flags & kCMGlobalName) {
		g->theValue = theValue;													/* save global name ptr							*/
		++g->nbrOfGlobalNames;													/* count it too											*/
	}
	
	return (WalkNextTOCValue);												/* continue value walk							*/
}


/*--------------------------------------------------------------------*
 | CMGetGlobalName - get the global name (string) value for an object |
 *--------------------------------------------------------------------*
 
 This routine takes an object refNum and searches its values to see if it has a global
 name (defined by CMRegisterType() or CMRegisterProperty()).  If it does, a pointer to
 the global name string is returned.  If none is found, NULL is returned.
 
 Caution: the user must treat the returned pointer as a value pointing to a read-only
          entity!  The pointer we return here is a pointer directly to the name string
					in the global name symbol table.  There is no enforcement of this (how?).
*/

CMGlobalName CM_FIXEDARGS CMGetGlobalName(CMObject theObject)
{
	TOCValuePtr		 	theValue;
	GetGNameComArea gNameComArea;
	ContainerPtr		container;

	ExitIfBadObject(theObject, NULL);								/* validate theObject									*/
	
	container = ((TOCObjectPtr)theObject)->container;

	/* Walk the entire object and look at all the values to find the value for a global		*/
	/* name.  The object can have other properties and values, but there better be only 	*/
	/* one global name.  The object is walked with cmWalkObject().  The "refCon" we pass	*/
	/* is a pointer to a communication area which will hold the count of the global name	*/
	/* values and the pointer to it.  As just mentioned, if the count comes back other 		*/
	/* than 1, we have an error.																													*/
	
	gNameComArea.theValue = NULL;										/* init value ptr to no value yet			*/
	gNameComArea.nbrOfGlobalNames = 0;							/* there are no global names yet too	*/
	
	cmWalkObject(container, (TOCObjectPtr)theObject, &gNameComArea, NULL, NULL, NULL, checkValue);
	
	/* After the walk we should have found 1 or 0 global names. If there is more than 1 	*/
	/* we have an ambiguity.  0 means return NULL.																				*/
		
	if (gNameComArea.nbrOfGlobalNames > 1) {				/* must have exactly 1 global name		*/
		ERROR2(CM_err_AmbiguousType, "CMGetGlobalName", CONTAINERNAME);
		return (NULL);
	}

	if (gNameComArea.nbrOfGlobalNames == 0)					/* no name ==> NULL return						*/
		return (NULL);
	
	theValue = gNameComArea.theValue;
	if (theValue == NULL) return (NULL);						/* safety															*/
	
	return ((CMGlobalName)GetGlobalName(theValue->value.globalName.globalNameSymbol));
}

/*-----------------------------------------------------------------*
 | CMCountProperties - count the number of properties in an object |
 *-----------------------------------------------------------------*
 
 An object can be defined to have any number of properties.  The properties for a given 
 object are unique.  This routine is used to determine the total number of properties 
 for an object or whether a property is present in that object.
 
 If the property is specified as NULL, the total number of properties in the object
 is returned.  If the property is not NULL, 1 is returned if that property is present
 (because there can be a maximum of one value of that type), and 0 otherwise.
*/

CMCount CM_FIXEDARGS CMCountProperties(CMObject object, CMProperty property)
{
	ContainerPtr 	 container;
	
	ExitIfBadObject(object, 0);												/* validate object									*/
	
	container = ((TOCObjectPtr)object)->container;
	
	/* Process explicitly specified type separately...																		*/
	
	if (property != NULL) {
		ExitIfBadProperty(property, 0);							/* validate property										*/

		if (container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer) {
			ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
																 CONTAINERNAMEx(((TOCObjectPtr)property)->container));
			return (0);
		}
		
		/* Find the TOCProperty belonging to the object with the property ID of the					*/
		/* specified property object...																											*/
	
		return ((CMCount)(cmGetObjectProperty((TOCObjectPtr)object,
																	  	((TOCObjectPtr)property)->objectID) != NULL));
	}
	
	/* From here on we're interested in just the total number of properties...						*/
	
	return ((CMCount)cmCountListCells(&((TOCObjectPtr)object)->propertyList));
}

/*---------------------------------------------------*
 | CMGetObjectRefCon - return user's object "refCon" |
 *---------------------------------------------------*
 
 This routine returns the user's "refCon" (reference constant) that s/he may associate
 with any object refNum (i.e., a CMObject).  The refCon is a CM_ULONG that the user may use
 in any way.  It is not touched by the API except to init it to 0 when the object is
 initially created.
 
 Note, the refCon is NOT perserved across closed containers, i.e., it is not saved in the
 TOC.
*/

CMRefCon CM_FIXEDARGS CMGetObjectRefCon(CMObject theObject)
{
	ExitIfBadObject(theObject, (CMRefCon)NULL);							/* validate theObject					*/
	
	return (((TOCObjectPtr)theObject)->objectRefCon);				/* return user's refCon				*/
}


/*----------------------------------------------------*
 | CMSetObjectRefCon - set the user's object "refCon" |
 *----------------------------------------------------*
 
 This routine is used to set the user's "refCon" (reference constant) to be assoicated with
 an object.  The refCon is a CM_ULONG that the user may use in any way.  It is not touched
 by the API.
 
 Note, the refCon is NOT perserved across closed containers, i.e., it is not saved in the
 TOC.
*/

void CM_FIXEDARGS CMSetObjectRefCon(CMObject theObject, CMRefCon refCon)
{
	ExitIfBadObject(theObject, CM_NOVALUE);									/* validate theObject					*/
	
	((TOCObjectPtr)theObject)->objectRefCon = refCon;				/* set the user's refCon			*/
}


/*-----------------------------------*
 | CMDeleteObject - delete an object |
 *-----------------------------------*
 
 The specified object and all its properties and values are deleted.  It is an error to
 use this object (refNum) from this point on.  Deleted objects no longer exist.  The
 CMGetNext... routines will also never return them.
 
 Note, some objects are protected from deletion.  This includes the predefined TOC
 objects (seed and offset values) and objects representing currently opened embedded
 containers. 
*/

void CM_FIXEDARGS CMDeleteObject(CMObject theObject)
{
	ContainerPtr container;
	
	ExitIfBadObject(theObject, CM_NOVALUE);							/* validate theObject							*/
	
	container = ((TOCObjectPtr)theObject)->container->updatingContainer;
	
	if ((container->useFlags & kCMWriting) == 0) {			/* make sure opened for writing		*/
		ERROR2(CM_err_DeleteIllegal, "object", CONTAINERNAME);
		return;
	}

	if (((TOCObjectPtr)theObject)->useCount > 1) {			/* there are other users!					*/
		ERROR1(CM_err_CantDelete6, CONTAINERNAME);
		return;
	}
	
	((TOCObjectPtr)theObject)->useCount = 0;						/* no longer in use (obviously)		*/
	
	/* It is an error to delete a protected object (e.g., object ID 1) or an object 			*/
	/* containing dynamic values.  But if it's not one of these, delete the object. If		*/
	/* we're recording updates, then define touched list entry for the delete...					*/
	
	if (((TOCObjectPtr)theObject)->objectFlags & ProtectedObject)
		ERROR1(CM_err_CantDelete1, CONTAINERNAME)
	else if (((TOCObjectPtr)theObject)->objectFlags & DynamicValuesObject)
		ERROR1(CM_err_CantDelete5, CONTAINERNAME)
	else {
		cmMarkObjectDeleted(container, (TOCObjectPtr)theObject);
		cmTouchDeletedObject((TOCObjectPtr)theObject);	 	/* must call AFTER marking				*/
	}
}


/*------------------------------------------------------------*
 | CMDeleteObjectProperty - delete a property of an an object |
 *------------------------------------------------------------*
 
 The property corresponding to the object ID of theProperty is deleted along with all of 
 its values.
 
 Note, some property values are protected from deletion.  This includes the predefined
 TOC object values (seed and offset) and the currently open embedded container values.
*/

void CM_FIXEDARGS CMDeleteObjectProperty(CMObject theObject, CMProperty theProperty)
{
	ContainerPtr 	 container;
	TOCValueHdrPtr theValueHdr;

	ExitIfBadObject(theObject, CM_NOVALUE);					/* validate theObject									*/
	ExitIfBadProperty(theProperty,CM_NOVALUE);			/* validate theProperty								*/
	
	container = ((TOCObjectPtr)theObject)->container;
	
	if (container->targetContainer != ((TOCObjectPtr)theProperty)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)theProperty)->container));
		return;
	}

	container = container->updatingContainer;				/* use updating container from here on*/
	
	if ((container->useFlags & kCMWriting) == 0) {
		ERROR2(CM_err_DeleteIllegal, "object's property", CONTAINERNAME);
		return;
	}
	
	/* Search through the properties for this object looking for the one that has the			*/
	/* ID as the passed property descriptor object. Note we store back into theProperty		*/
	/* (the variable) thus changing its meaning and type.	I just thought I would point 		*/
	/* that out explicitly!																																*/
	
	theProperty = (CMProperty)cmGetObjectProperty((TOCObjectPtr)theObject,
																								((TOCObjectPtr)theProperty)->objectID);
	if (theProperty == NULL) return;								/* exit if property not in object			*/
	
	/* Scan the values (headers) for this property to make sure there are no protected		*/
	/* values.  These are values which we never want deleted.  If there are any then it 	*/
	/* is an error to delete this property from the object.  Sorry about that!						*/
	
	theValueHdr = (TOCValueHdrPtr)cmGetListHead(&((TOCPropertyPtr)theProperty)->valueHdrList);
	while (theValueHdr) {
		if ((theValueHdr->valueFlags & ValueProtected) != 0){	/* can't delete if protected!	*/
			ERROR1(CM_err_CantDelete2, CONTAINERNAME);
			return;
		}
		theValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
	} /* while */

	/* If we're recording updates, then define touched list entry for the delete...				*/
	
	cmTouchDeletedProperty((TOCPropertyPtr)theProperty, ((TOCPropertyPtr)theProperty)->theObject);
		
	cmDeleteProperty(container, (TOCPropertyPtr)theProperty);
}


/*------------------------------------------------------------------------*
 | CMReleaseObject - destroy association between an object and its refNum |
 *------------------------------------------------------------------------*
 
 The association between the refNum and the object is destroyed. There should be no further
 operations with the object (refNum) once this routine is called.
*/

void CM_FIXEDARGS CMReleaseObject(CMObject theObject)
{
	ContainerPtr container;
	
	ExitIfBadObject(theObject, CM_NOVALUE);					/* validate theObject									*/
	
	if (((TOCObjectPtr)theObject)->useCount == 0) {	/* error if already fully released		*/
		container = ((TOCObjectPtr)theObject)->container;
		ERROR1(CM_err_AlreadyReleased2, CONTAINERNAME);
	} else
		--((TOCObjectPtr)theObject)->useCount;				/* decrement the object's use count		*/
}

CMObject CM_FIXEDARGS CMGetObject(CMContainer targetContainer, CMObjectID objectID)
{
	ContainerPtr container = (ContainerPtr)targetContainer;
	TOCObjectPtr	object;
	
	object = cmFindObject(container->toc, objectID);
	if (object != NULL)
		++object->useCount;
	
	return (CMObject) object;
}
	
CMObjectID CM_FIXEDARGS CMGetObjectID(CMObject object)
{
	return ((TOCObjectPtr) object)->objectID;
}
	
														  
															CM_END_CFUNCTIONS
