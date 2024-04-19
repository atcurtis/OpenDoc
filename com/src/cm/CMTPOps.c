/* @(#)Z 1.4 com/src/cm/CMTPOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:55 (96/10/29 09:17:10) */
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
	File:		     CMTPOps.c     

	Contains:	Container Manager Type and Property Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 12/9/94	EL		#1182275 Optionally do not maintain
									continue flag.
		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in
		 <3>	 10/6/93	EL		Rename CMGetAdjacentXXXX to GetAdjacentXXXX
													and static near.
		 <2>	 10/4/93	EL		Add CMGetPrevProperty and CMGetPrevType

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                        <<<      CMTPOps.c      >>>                        |
 |                                                                           |
 |              Container Manager Type and Property Operations               |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/15/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 All types and properties must be registered with CMRegisterType() and CMRegisterProperty()
 to get their associated refNums.  The refNums, in turn, are used as "type" and "property"
 parameters to other API calls.  When reading containers CMRegisterType() and
 CMRegisterProperty() are also used to get refNums for existing types and properties in
 the container.  Obviously CMRegisterType() and CMRegisterProperty() are two key routines 
 and are defined in this file.
 
 The other routines here allow for testing refNums to see if they are types or properties
 and for accessing all the types and properties in the container.
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
#ifndef __DYNAMICVALUES__
#include "DynValus.h"     
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
#ifndef __FREESPACE__
#include "FreeSpce.h" 
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
#pragma segment CMTypePropertyOps
#endif
																	

/*----------------------------------------------*
 | CMRegisterType - register a global type name |
 *----------------------------------------------*
 
 The specified type name is registered as a type in the specified container.  A refNum
 for it is returned.  If the type name already exists, the refNum for it is returned.
 
 Note, types may be used to create dynamic values when passed to CMNewValue().  Such types
 must have a metahandler registered for them and that metahandler must have handler 
 routines for "use value", "new value", and "metadata" operation types.  See
   DynValus.c    for a full explaination of dynamic values (more than you probably ever
 want to know).
*/

CMType CM_FIXEDARGS CMRegisterType(CMContainer targetContainer, CMconst_CMGlobalName name)
{
	TOCObjectPtr 	t;
	GlobalNamePtr g;
	ContainerPtr 	container = (ContainerPtr)targetContainer;
	
	NOPifNotInitialized(NULL);												/* NOP if not initialized!					*/
	
	/* See if the type global name already exists...																			*/
	
	g = cmLookupGlobalName(container->globalNameTable, (CM_UCHAR *)name);
	
	if (!SessionSuccess) {
		ERROR2(CM_err_GloblaNameError, name, CONTAINERNAME);
		return (NULL);
	}
	
	/* If the name previously existed, cmLookupGlobalName() will return a pointer to the	*/
	/* global name entry.  Even deleted global names have entries!  But we can tell that	*/
	/* because the value back pointer in the entry is NULLed out. If we do have a already */
	/* existing entry, just return the owning object pointer after one last validation 		*/
	/* that it is indeed a type object (so I'm paranoid).																	*/
	
	if (g && g->theValue) {														/* previously existing name					*/
		t = g->theValue->theValueHdr->theProperty->theObject;
		if ((t->objectFlags & TypeObject) == 0) {
			ERROR4(CM_err_MultTypeProp, "type", name, CONTAINERNAME,
						 (t->objectFlags & PropertyObject) ? "property" : "object");
			t = NULL;
		} else
			++t->useCount;																/* bump this object's use count			*/
	} else {																					/* a new name												*/
		t = cmDefineGlobalNameObject(container, container->nextUserObjectID, CM_StdObjID_GlobalTypeName,
																 CM_StdObjID_7BitASCII, (CM_UCHAR *)name,
																 container->generation, 0, TypeObject);
		if (t) {
			t->useCount = 1;															/* initial use of this object				*/
			IncrementObjectID(container->nextUserObjectID);
		}
	}
		
	return ((CMType)t);
}


/*-----------------------------------------------*
 | CMRegisterProperty - register a property name |
 *-----------------------------------------------*
 
 The specified property name is registered as a property in the specified container.  A
 refNum or it is returned.  If the property name already exists, the refNum for it is
 returned.
 
 Note, the implementation here is basically the same as CMRegisterType() except for the
 metaData.  Properties have none.
*/

CMProperty CM_FIXEDARGS CMRegisterProperty(CMContainer targetContainer,
																					 CMconst_CMGlobalName name)
{
	TOCObjectPtr	p;
	GlobalNamePtr g;
	ContainerPtr 	container = (ContainerPtr)targetContainer;
	
	NOPifNotInitialized(NULL);												/* NOP if not initialized!					*/

	g = cmLookupGlobalName(container->globalNameTable, (CM_UCHAR *)name);
	
	if (!SessionSuccess) {
		ERROR2(CM_err_GloblaNameError, name, CONTAINERNAME);
		return (NULL);
	}
	
	if (g && g->theValue) {
		p = g->theValue->theValueHdr->theProperty->theObject;
		if ((p->objectFlags & PropertyObject) == 0) {
			ERROR4(CM_err_MultTypeProp, "type", name, CONTAINERNAME,
						 (p->objectFlags & TypeObject) ? "type" : "object");
			p = NULL;
		} else
			++p->useCount;																/* bump this object's use count			*/
	} else {
		p = cmDefineGlobalNameObject(container, container->nextUserObjectID, CM_StdObjID_GlobalPropName,
																 CM_StdObjID_7BitASCII, (CM_UCHAR *)name,
																 container->generation, 0, PropertyObject);
		if (p) {
			p->useCount = 1;															/* initial use of this object				*/
			IncrementObjectID(container->nextUserObjectID);
		}
	}
	
	return ((CMProperty)p);
}


/*-------------------------------------------*
 | CMAddBaseType - add a base type to a type |
 *-------------------------------------------*
 
 A base type (baseType) is added to the specified type.  For each call to CMAddBaseType()
 for the type a new base type is recorded.  They are recorded in the order of the calls.
 The total number of base types recorded for the type is retuned.   0 is returned if there
 is an error and the error reporter returns.
 
 It is an error to attempt to add the SAME base type more than once to the type. Base types
 may be removed from a type by using CMRemoveBaseType().
 
 The purpose of this routine is to define base types for a given type so that layered
 dynamic values can be created.  Base types simulate the form of type inheritance.
 
 See   DynValus.c    for a full description of dynamic values and how base types are used.
*/

CMCount CM_FIXEDARGS CMAddBaseType(CMType type, CMType baseType)
{
	ContainerPtr	 container;
	TOCObjectPtr 	 theObject;
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr 	 theValue;
	TOCValueBytes	 valueBytes;
	CMObjectID		 baseTypeID;
	char	 				 *typeName, *baseTypeName;
	
	ExitIfBadType(type, 0);
	ExitIfBadType(baseType, 0);
	
	container = ((TOCObjectPtr)type)->container;

	if (container->targetContainer != ((TOCObjectPtr)type)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)baseType)->container));
		return (0);
	}
	
	container = container->updatingContainer;

	if ((container->useFlags & kCMWriting) == 0) {			/* make sure opened for writing		*/
		ERROR1(CM_err_WriteIllegal1, CONTAINERNAME);
		return (0);
	}
	
	/* Scan the "base type" property value data to see if the base type ID is already 		*/
	/* there.	 It's an error if it's already there.																				*/
	
	baseTypeID  = ((TOCObjectPtr)baseType)->objectID;
	theProperty = cmGetObjectProperty((TOCObjectPtr)type, CM_StdObjID_BaseTypes);
	
	if (theProperty != NULL) {													/* if base type property exists		*/
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		
		if (theValueHdr != NULL) {												/* double check for value hdr			*/
			theValue  = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList); /* 1st base ID		*/
			
			while (theValue != NULL) {											/* look at the base type values		*/
				if ((CMObjectID)theValue->value.imm.ulongValue == baseTypeID) {	/* look for ID	*/
					typeName 		 = cmIsGlobalNameObject((TOCObjectPtr)type, CM_StdObjID_GlobalTypeName);
					baseTypeName = cmIsGlobalNameObject((TOCObjectPtr)baseType, CM_StdObjID_GlobalTypeName);
					ERROR3(CM_err_DupBaseType, baseTypeName, typeName, CONTAINERNAME);
					return (0);
				}
				
				theValue = (TOCValuePtr)cmGetNextListCell(theValue); /* look at next base type	*/
			} /* while */
			
		} /* theValueHdr */
	} /* theProperty */
	
	/* We have a new base type for the type.  If this is the first base type, create the 	*/
	/* special "base type" property with a single immediate base type ID value. Additional*/
	/* base type ID's form a continued immediate value producing the list of base types.	*/
	/* Note, this is rare instance where we allow a continued immediate value!						*/

	cmSetValueBytes(container, &valueBytes, Value_Imm_Long, (CM_ULONG)baseTypeID, sizeof(CMObjectID));
	
	if (theProperty == NULL || theValueHdr == NULL) {		/* if 1st base type for type...		*/
		theObject = cmDefineObject(container, 						/* ...create base type property		*/
															 ((TOCObjectPtr)type)->objectID,
															 CM_StdObjID_BaseTypes, CM_StdObjID_BaseTypeList,
															 &valueBytes, container->generation, kCMImmediate, 
															 TypeObject, &theValueHdr);
		if (theObject == NULL) return (0);
		theValueHdr->useCount = 1;												/* ...always count use of value		*/
	} else {																						/* if additional base types...		*/
		theValue  = (TOCValuePtr)cmGetListTail(&theValueHdr->valueList);
#if CMKEEP_CONTINUE_FLAG
		theValue->flags |= kCMContinued;									/* ...flag current value as cont'd*/
#endif
		cmAppendValue(theValueHdr, &valueBytes, kCMImmediate); /* ...add in new base type		*/	
#if CMKEEP_CONTINUE_FLAG
		theValueHdr->valueFlags |= ValueContinued;				/* ...create cont'd imm ID list		*/
#endif
		cmTouchBaseType(theValueHdr);											/* touch for updating if necessary*/
	}

	return ((CMCount)cmCountListCells(&theValueHdr->valueList)); /* ret nbr of base types	*/
}


/*---------------------------------------------------*
 | CMRemoveBaseType - remove a base type from a type |
 *---------------------------------------------------*
 
 A base type (baseType) previously added to the specifed type by CMAddBaseType() is
 removed.  If NULL is specified as the baseType, ALL base types are removed.  The number of
 base types remaining for the type is returned.
 
 Note, no error is reported if the specified base type is not present or the type has no
 base types.
*/

CMCount CM_FIXEDARGS CMRemoveBaseType(CMType type, CMType baseType)
{
	ContainerPtr	 container;
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr 	 theValue;
#if CMKEEP_CONTINUE_FLAG
	TOCValuePtr 	 thePrevValue;
#endif
	CMObjectID		 baseTypeID;
	CMCount				 n;
	
	ExitIfBadType(type, 0);
	ExitIfBadType(baseType, 0);
	
	container = ((TOCObjectPtr)type)->container;

	if (container->targetContainer != ((TOCObjectPtr)type)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)baseType)->container));
		return (0);
	}
		
	container = container->updatingContainer;

	if ((container->useFlags & kCMWriting) == 0) {			/* make sure opened for writing		*/
		ERROR1(CM_err_WriteIllegal1, CONTAINERNAME);
		return (0);
	}

	/* Scan the "base type" property value data to see if the base type ID is already 		*/
	/* there so that we know we can delete it.  We count the number of base types so that	*/
	/* we may return the new count as the function result. Note if the base type does not	*/
	/* exist, we return the current count and do nothing else.  The user didn't want the	*/
	/* the type and it's not there, so it's not an error to not find it.  In a similar 		*/
	/* vain, we remove the property if the caller asked that all base types be removed.		*/
	
	theProperty = cmGetObjectProperty((TOCObjectPtr)type, CM_StdObjID_BaseTypes);
	if (theProperty == NULL) return (0);
		
	theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
	if (theValueHdr == NULL) return (0);
	
	if (baseType == NULL) {															/* delete entire property ?				*/
		CMDeleteObjectProperty((CMObject)type, container->baseTypesProperty);
		return (0); 
	}
	
	baseTypeID = ((TOCObjectPtr)baseType)->objectID;
	
	theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList); /* 1st base ID				*/
			
	while (theValue != NULL) {														   /* look at base type values..*/
		if ((CMObjectID)theValue->value.imm.ulongValue != baseTypeID) { /*if ID is not found*/
			theValue = (TOCValuePtr)cmGetNextListCell(theValue); /* ...look at next base type	*/
			continue;																						 /* ...keep looking						*/
		}
			
#if CMKEEP_CONTINUE_FLAG
		if (cmCountListCells(&theValueHdr->valueList) > 1) { 	 /* if more than 1 basetype...*/
			if (cmGetNextListCell(theValue) == NULL) {					 /* make sure of seg contd bit*/
				thePrevValue = (TOCValuePtr)cmGetPrevListCell(theValue);
				if (thePrevValue != NULL)													 /* ...no longer cont'd if		*/
					thePrevValue->flags &= ~kCMContinued;						 /*    last sef is deleted		*/
			}
		}
#endif
			
		cmAddToFreeList(container, theValue, 0, 0);				/* add freed space to free list		*/
		cmDeleteListCell(&theValueHdr->valueList, theValue);
		CMfree(theValue);																	/* poof!													*/
		
		theValueHdr->size -= sizeof(CMObjectID);					/* update total size in value hdr	*/
#if CMKEEP_CONTINUE_FLAG
		if (cmCountListCells(&theValueHdr->valueList) < 2)/* make sure of cont'd bit in hdr	*/
			theValueHdr->valueFlags &= ~ValueContinued;
#endif

		break;																						/* we're done -- it's gone				*/
	} /* while */
	
	/* If there are no base types left, remove the property.  Also touch for updating if	*/
	/* needed.																																						*/
	
	n = (CMCount)cmCountListCells(&theValueHdr->valueList);
	
	if (n > 0)
		cmTouchBaseType(theValueHdr);											/* touch for updating if necessary*/
	else
		CMDeleteObjectProperty((CMObject)type, container->baseTypesProperty);
		
	return (n); 																				/* nbr of base types left					*/
}


/*----------------------------------------*
 | CMIsType - test if an object is a type |
 *----------------------------------------*
 
 Returns non-zero if the object is a type and 0 otherwise.
*/

CMBoolean CM_FIXEDARGS CMIsType(CMObject theObject)
{
	ExitIfBadObject(theObject, 0);										/* validate theObject								*/
	
	return ((CMBoolean)((((TOCObjectPtr)theObject)->objectFlags & TypeObject) != 0));
}


/*------------------------------------------------*
 | CMIsProperty - test if an object is a property |
 *------------------------------------------------*
 
 Returns non-zero if the object is a property and 0 otherwise.
*/

CMBoolean CM_FIXEDARGS CMIsProperty(CMObject theObject)
{
	ExitIfBadObject(theObject, 0);										/* validate theObject								*/

	return ((CMBoolean)((((TOCObjectPtr)theObject)->objectFlags & PropertyObject) != 0));
}


/*------------------------------------------------------------------------------*
 | getAdjacentType - get adjacent type in the container by increasing object ID |
 *------------------------------------------------------------------------------*

 This routine returns the refNum for the adjacent type in the container following/preceding
 currType.  If currType is NULL, the refNum for the first/last type object is returned.  If
 currType is not NULL, the type object with the next highest/lowest ID is returned.  NULL is
 returned if there are no more type objects following/[receding currType.
 
 currType is generally a refNum previously returned from this routine.  Successive calls
 to this routine will thus yield all the type objects in the container.
*/

static CMType CM_NEAR getAdjacentType(CMContainer targetContainer, CMType currType, Boolean forward)
{
	TOCObjectPtr theObject;
	ContainerPtr container = (ContainerPtr)targetContainer;
	
	NOPifNotInitialized(NULL);												/* NOP if not initialized!					*/
	
	if (currType == NULL) 														/* NULL ==> use head of list				*/
		if (forward)
			return ((CMType)cmGetMasterListHead(container->toc, TypeObject));
		else
			return ((CMType)cmGetMasterListTail(container->toc, TypeObject));

	ExitIfBadType(currType, NULL);										/* validate currType								*/
	
	#if 0
	if (((TOCObjectPtr)currType)->container != container || 	/* safety check container		*/
			(((TOCObjectPtr)currType)->objectFlags & TypeObject) == 0) {
		ERROR3(CM_err_BadType, "CMType", "CMGetNextType", CONTAINERNAME);
		return (NULL);
	}
	#endif
	
	/* In our implementation the type objects are already chained in sorted order. So all */
	/* we have to do is follow the type chain.																						*/
	
	if (forward)
		theObject = ((TOCObjectPtr)currType)->nextTypeProperty;
	else
		theObject = ((TOCObjectPtr)currType)->prevTypeProperty;
	if (theObject) ++theObject->useCount;							/* bump use count for next object		*/
	
	return ((CMType)theObject);
}

/*------------------------------------------------------------------------*
 | CMGetNextType - get next type in the container by increasing object ID |
 *------------------------------------------------------------------------*

 Just call getAdjacentType with forward = true.
*/

CMType CM_FIXEDARGS CMGetNextType(CMContainer targetContainer, CMType currType)
{
	return getAdjacentType(targetContainer, currType, true);
}
														 
/*----------------------------------------------------------------------------*
 | CMGetPrevType - get previous type in the container by decreasing object ID |
 *----------------------------------------------------------------------------*

 Just call getAdjacentType with forward = false.
*/

CMType CM_FIXEDARGS CMGetPrevType(CMContainer targetContainer, CMType currType)
{
	return getAdjacentType(targetContainer, currType, false);
}
														 
/*-------------------------------------------------------------------------------------*
 | getAdjacentProperty - get adjacent property in the container by next/prev object ID |
 *-------------------------------------------------------------------------------------*

 This routine returns the refNum for the next/prev property object in the container following
 currProperty.  If currProperty is NULL, the refNum for the first/last property object is
 returned.  If currProperty is not NULL, the property object with the next highest/lowest ID is
 returned.  NULL is returned if there are no more property objects following/preceding currProperty.
 
 currProperty is generally a refNum previously returned from this routine.  Successive
 calls to this routine will thus yield all the property objects in the container.
*/

static CMProperty CM_NEAR getAdjacentProperty(CMContainer targetContainer,
																							CMProperty currProperty,
																							Boolean forward)
{
	TOCObjectPtr theObject;
	ContainerPtr container = (ContainerPtr)targetContainer;

	NOPifNotInitialized(NULL);												/* NOP if not initialized!					*/

	if (currProperty == NULL) 												/* NULL ==> use head of list				*/
		if (forward)
			return ((CMProperty)cmGetMasterListHead(container->toc, PropertyObject));
		else
			return ((CMProperty)cmGetMasterListTail(container->toc, PropertyObject));
	
	ExitIfBadProperty(currProperty, NULL);						/* validate currProperty						*/

	#if 0
	if (((TOCObjectPtr)currProperty)->container != container || /* safety check container	*/
			(((TOCObjectPtr)currProperty)->objectFlags & PropertyObject) == 0) {
		ERROR3(CM_err_BadType, "CMProperty", "CMGetNextProperty", CONTAINERNAME);
		return (NULL);
	}
	#endif

	/* In our implementation the property objects are already chained in sorted order. 		*/
	/* So all we have to do is follow the property chain.																	*/
	
	if (forward)
		theObject = ((TOCObjectPtr)currProperty)->nextTypeProperty;
	else
		theObject = ((TOCObjectPtr)currProperty)->prevTypeProperty;

	if (theObject) ++theObject->useCount;							/* bump use count for next object		*/
	
	return ((CMProperty)theObject);
}

/*----------------------------------------------------------------------------------*
 | CMGetNextProperty - get next property in the container by next highest object ID |
 *----------------------------------------------------------------------------------*

 Just call getAdjacentProperty with forward = true.
*/

CMProperty CM_FIXEDARGS CMGetNextProperty(CMContainer targetContainer,
																					CMProperty currProperty)
{
	return getAdjacentProperty(targetContainer, currProperty, true);
}
														 
/*-------------------------------------------------------------------------------------*
 | CMGetPrevProperty - get previous property in the container by next lowest object ID |
 *-------------------------------------------------------------------------------------*

 Just call getAdjacentProperty with forward = false.
*/

CMProperty CM_FIXEDARGS CMGetPrevProperty(CMContainer targetContainer,
																					CMProperty currProperty)
{
	return getAdjacentProperty(targetContainer, currProperty, false);
}
														 
														  CM_END_CFUNCTIONS
