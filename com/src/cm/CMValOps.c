/* @(#)Z 1.5 com/src/cm/CMValOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:56 (96/10/29 09:17:29) */
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
	File:		 CMValOps.c 

	Contains:	Container Manager Value Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>	 12/9/94	EL		#1182275 Optionally do not maintain
									continue flag.
		 <3>	 9/30/94	EL		#1182275 Cut down processing when size is 0
													in write data.
		 <2>	 8/26/94	EL		#1182275 Cut down unnecessary read before
													write.
		 <3>	  6/3/94	EL		Rename Values.h/c to ValueRtn.h/c.
		 <2>	 3/17/94	EL		update to zero length value will not show
													up. For small value compare with original
													value to avoid unnecessary write. #1147893
		 <1>	  2/3/94	EL		first checked in
		 <6>	  2/1/94	EL		Check for null target container to make
													memory use safe for multitasking.
		 <5>	  1/6/94	EL		Fix bug that freelist is not used if the
													old value has no data.
		 <4>	 10/6/93	EL		Rename CMGetAdjacentXXXX to GetAdjacentXXXX
													and static near.
		 <3>	 10/4/93	EL		Let CMGetPrevValue and CMGetNextValue share 
		 											CMGetAdjacentValue.
		 <2>	 9/27/93	VL		Added CMGetPrevValue.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<<  CMValOps.c  >>>                           |
 |                                                                           |
 |                     Container Manager Value Operations                    |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/15/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 An object's property is given a typed value by writing to that value.  Conversely, you
 get the value for an object's property by reading the value.  Before you do either of
 these operations you must get a refNum for a value.  So depending in input or output
 there are four key value operations.
 
 	1.	CMUseValue()  			returns a refNum to an existing value.
	2.	CMNewValue()				creates and returns a refNum for a new value.
	3.	CMReadValueData()		reads the value
	4.	CMWriteValueData()	writes (sets) the value
	
 These routines are all defined in this file.  Other value routines are provided for doing
 various manipulations and getting or setting certain information about the values.  Of
 all the API routines defined in the Container Manager, you may view the CMReadValueData()
 and CMWriteValueData() as two of the most important!  As it turns out they are two of the
 most complicated routines in the Container Manager.  CMWriteValueData() is one of the
 worst! (just thought I would worn you).
*/


#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

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
#ifndef __VALUEROUTINES__
#include "ValueRtn.h"       
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

/*---------------------------------------------------------------------*
 | CMCountValues - count the number of values for an object's property |
 *---------------------------------------------------------------------*
 
 A property for an object can be defined to have any number of typed values.  The types
 for all the values for a given object property are unique.  This routine is used to 
 determine the total number of values for a object's property or whether a value for a
 given type is present for that object's property.
 
 If the type is specified as NULL, the total number of values for the object's property
 is returned.  If the type is not NULL, 1 is returned if a value of that type is present
 (because there can be a maximum of one value of that type), and 0 otherwise.  If the
 property is not defined for the object, 0 is also returned.
*/

CMCount CM_FIXEDARGS CMCountValues(CMObject object, CMProperty property, CMType type)
{
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr;
	ContainerPtr 	 container;
	
	ExitIfBadObject(object, 0);												/* validate object									*/
	ExitIfBadProperty(property, 0);										/* validate property								*/
	
	container = ((TOCObjectPtr)object)->container;
	
	/* Process explicitly specified type separately...																		*/
	
	if (type != NULL) {
		ExitIfBadType(type, 0);													/* validate type										*/
		
		if (container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer ||
				container->targetContainer != ((TOCObjectPtr)type)->container->targetContainer) {
			ERROR3(CM_err_3Containers, CONTAINERNAMEx(container),
																 CONTAINERNAMEx(((TOCObjectPtr)property)->container),
																 CONTAINERNAMEx(((TOCObjectPtr)type)->container));
			return (0);
		}
		
		/* Find the TOCProperty belonging to the object with the property ID of the					*/
		/* specified property object...																											*/
	
		theProperty = cmGetObjectProperty((TOCObjectPtr)object,
																	  	((TOCObjectPtr)property)->objectID);
		if (theProperty == NULL) return (0);
		
		/* Find the value for the object's property that has the desired type...						*/
		
		theValueHdr = cmGetPropertyType(theProperty, ((TOCObjectPtr)type)->objectID);
		
		return ((CMCount)(theValueHdr != NULL));
	}
	
	/* From here on we're interested in just the total number of values...								*/
	
	if (container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)property)->container));
		return (0);
	}
	
	/* Find the TOCProperty belonging to the object with the property ID of the specified	*/
	/* property object...																																	*/

	theProperty = cmGetObjectProperty((TOCObjectPtr)object,
																	  ((TOCObjectPtr)property)->objectID);
	
	return (theProperty ? (CMCount)cmCountListCells(&theProperty->valueHdrList) : 0);
}


/*----------------------------------------------------------*
 | CMUseValue - get a refNum for an existing value (header) |
 *----------------------------------------------------------*
 
 This routine is used to get the refNum for the value of an object's property of the
 given type.  NULL is returned if the value does not exist, or if or the object does not
 contain the property.  If the type of the value corresponds to a global type name that has
 an associated "use value" handler, or if it's base types (if any) have associated "use
 value" handlers, a dynamic value will be created and returned. 
 
 Note, if the value is used as an embedded container, then that embedded container must be
 opened and read using CMOpenContainer(). The data, i.e, the embedded container for such a
 value can only be defined by using CMOpenNewContainer().  The container type name must be
 associated with a special set of handlers that define a "return parent value" handler. 
 This handler returns the parent value refNum whose data contains the embedded container.  
 
 There is no restriction on reading the data for an embedded container like any other value
 data using CMReadValueData(). However, the data for an embedded container value includes a
 TOC.  Unless a "blind" copy is being done, the TOC read this way is of not very much use!
*/

CMValue CM_FIXEDARGS CMUseValue(CMObject object, CMProperty property, CMType type)
{
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr, baseValueHdr;
	ContainerPtr	 container;
	
	ExitIfBadObject(object, NULL);										/* validate object									*/
	ExitIfBadProperty(property, NULL);								/* validate property								*/
	ExitIfBadType(type, NULL);												/* validate type										*/
	
	container = ((TOCObjectPtr)object)->container;
	
	if (container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer ||
			container->targetContainer != ((TOCObjectPtr)type)->container->targetContainer) {
		ERROR3(CM_err_3Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)property)->container),
															 CONTAINERNAMEx(((TOCObjectPtr)type)->container));
		return (NULL);
	}
	
	/* Find the TOCProperty belonging to the object with the property ID of the specified	*/
	/* property object (got that?)...																											*/
	
	theProperty = cmGetObjectProperty((TOCObjectPtr)object,
																		((TOCObjectPtr)property)->objectID);
	if (theProperty == NULL)													/* if property not in object...			*/
		return (NULL);																	/* ..tell user about it							*/
	
	/* If the resulting value is a dynamic value, just return it. If it isn't, this might */
	/* be the first use of the value.  In that case we must check to see if a dynamic 		*/
	/* value must be created. This is done by calling cmHasUseValueHandler() which checks */
	/* to see if the value's type has a registered metahandler, and that the metahandler	*/
	/* defines a "use value" handler.  If that is the case, we call cmFollowTypes() to 		*/
	/* do a depth-first processing of all the type's base types to see if layers of 			*/
	/* dynamic values must be created.  No matter what, if a dynamic value is created, we	*/
	/* return that as the result.  Otherwise, the original "real" value is returned.			*/

	/* Check to see if a dynamic value must be created.  This is done by calling 					*/
	/* cmFollowBaseTypes() to do a depth-first search starting from the given type on all	*/
	/* of that type's base types.  Dynamic values are created for each type that has a		*/
	/* "use value" handler ("new value" handlers are only required for CMNewValue()). The */
	/* resulting dynamic value is returned or the "real" value if there are no dynamic 		*/
	/* values.																																						*/
		
	theValueHdr = (TOCValueHdrPtr)cmGetPropertyType(theProperty, ((TOCObjectPtr)type)->objectID);
	
	if (theValueHdr) {																/* if we have a value header...			*/
		if (theValueHdr->dynValueData.dynValue != NULL){/* ...if dynamic value exists...		*/
			theValueHdr = theValueHdr->dynValueData.dynValue;/*...just use it, but...					*/
			++theValueHdr->useCount;											/* ...count dynamic value uses			*/
		} else {																				/* ...if no dynamic value yet...		*/
			baseValueHdr = theValueHdr;
			theValueHdr = cmFollowTypes(theValueHdr, (TOCObjectPtr)type, false, NULL);
			++baseValueHdr->useCount;											/* incr use count on base value			*/
		}
	}
	
	return ((CMValue)theValueHdr);										/* return real or dynamic value			*/
}


/*--------------------------------------------------------------------------------*
 | getAdjacentValue - use the adjacent value for a specified proprty of an object |
 *--------------------------------------------------------------------------------*

 This routine returns the refNum for the next/prev type value (according to the current value
 order) in the objects property following currValue.  If currValue is NULL, the refNum for
 the first/last value for that object's property is returned.  If  currValue is not NULL, the
 next/prev value for that object's property is returned.  NULL is returned if there are no more
 type values following/preceding currValue or the object does not contain the property.
 
 currValue is generally a refNum previously returned from this routine.  Successive calls
 to this routine will thus yield all the values for the specified property of the specified
 object as long as no other operations change the value order.
 
 Note, this routine implicitly does a CMUseValue() on the returned refNum.  Thus a dynamic
 value might be spawned.  A CMReleaseValue() is therfore required to reduce the use count
 of the refNum.
*/

static CMValue CM_NEAR getAdjacentValue(CMObject object, CMProperty property, CMValue currValue,
																				CMBoolean forward)
{
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr;
	ContainerPtr 	 container;
	CMType				 type;
	
	ExitIfBadObject(object, NULL);										/* validate object									*/
	ExitIfBadProperty(property, NULL);								/* validate property								*/

	container = ((TOCObjectPtr)object)->container;
	
	if (container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)property)->container));
		return (NULL);
	}

	/* Find the TOCProperty belonging to the object with the property ID of the specified	*/
	/* property object...																																	*/
	
	theProperty = cmGetObjectProperty((TOCObjectPtr)object,
																		((TOCObjectPtr)property)->objectID);
	if (theProperty == NULL)													/* if property not in object...			*/
		return (NULL);																	/* ..tell user about it							*/
	
	/* For the first time, return the first/last value on the property's list...					*/
	
	if (forward)
		if (currValue == NULL)														/* NULL ==> use head of list			*/
			theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		else {																						/* use next value in sequence			*/
			ExitIfBadValue(currValue, NULL);								/* validate currValue							*/
			theValueHdr = (TOCValueHdrPtr)cmGetNextListCell(currValue);
		}
	else
		if (currValue == NULL)														/* NULL ==> use tail of list			*/
			theValueHdr = (TOCValueHdrPtr)cmGetListTail(&theProperty->valueHdrList);
		else {																						/* use previous value in sequence	*/
			ExitIfBadValue(currValue, NULL);								/* validate currValue							*/
			theValueHdr = (TOCValueHdrPtr)cmGetPrevListCell(currValue);
		}
	
	/* We now have the value the caller wants.  But calling this routine is the "moral		*/
	/* equivalent" of a CMUseValue() since we are returning a value refNum.  This must be	*/
	/* like a CMUseValue() because the user will probably want to do a CMReleaseValue() 	*/
	/* and also potentially spawn a dynamic value. At the very lease the CMReleaseValue()	*/
	/* will want the use counts to be correct.  So what we must do here is do the 				*/
	/* CMUseValue() for the user.  To do that we need the type object refNum.  So...			*/
	
	if (theValueHdr != NULL) {												/* just to be safe!									*/
		type = (CMType)cmFindObject(container->toc, theValueHdr->typeID); /* type refNum		*/
		if (type == NULL)																/* if should have been found!				*/
			theValueHdr = NULL;														/* ...return NULL if it wasn't			*/
		else																						/* do the CMUseValue() for the user	*/
			theValueHdr = (TOCValueHdrPtr)CMUseValue(object, property, type);
	}
	
	return ((CMValue)theValueHdr);										/* NULL or CMUseValue() result			*/
}


/*--------------------------------------------------------------------------*
 | CMGetNextValue - use the next value for a specified proprty of an object |
 *--------------------------------------------------------------------------*

 Just call getAdjacentValue with forward.
*/

CMValue CM_FIXEDARGS CMGetNextValue(CMObject object, CMProperty property, CMValue currValue)
{
	return getAdjacentValue(object, property, currValue, true);
}

/*------------------------------------------------------------------------------*
 | CMGetPrevValue - use the previous value for a specified proprty of an object |
 *------------------------------------------------------------------------------*

 Just call getAdjacentValue with forward = false.
*/

CMValue CM_FIXEDARGS CMGetPrevValue(CMObject object, CMProperty property, CMValue currValue)
{
	return getAdjacentValue(object, property, currValue, false);
}


/*-------------------------------------------------------*
 | CMNewValue - create a refNum for a new value (header) |
 *-------------------------------------------------------*

 A new value entry is created for the specified object with the specified property and type
 and its refNum returned.  If the specified type corresponds to a global type name that has
 an associated "use value" handler, or if its base types (if any) have associated "use
 value" handlers, a dynamic value will be created and returned.
 
 An object's properties can have more than one value.  However, the all the types for the
 values belonging to a given object property must be UNIQUE.  It is an error to attempt to
 create a value for a property when there is already a value of the same type for that
 property.
 
 Note, the refNum returned from here represents a yet-to-be-set value.  The value data is
 set with CMWriteValueData(), or if it's for an embedded container, CMOpenNewContainer().
 For creating embedded containers, the container type name must be associated with a set
 of container handlers that support the "return parent value" handler.  Such a handler
 returns the parent value refNum whose data will contain the embedded container.  No value
 value data must exist for the value when the container is opened.  This value gets its
 data by using it as an embedded container instead of doing CMWriteValueData()'s to it.
 
 Note also, the value is created at an unspecified location in the sequence of values for
 the specified property.  The order of the values may change.
 
 As mentioned above, if the type, or any of its base types cause dynamic values to be
 created, the process of creating dynamic values may required data initialization
 parameters for the "new value" handler (see   DynValus.c    for a complete description of
 dynamic values and the "new value" handler).  These are suppled from the CMNewValue()
 "..." parameters.  They are "decoded" using a metadata format description returned from
 a "metadata" handler.
 
 The ORDER of the parameters is important!  Because base types are done with a depth-first
 search through the types down to their leaves, the CMNewValue() "..." parameters MUST be
 ordered for the "deepest" type first.  For example, given the following type inheritance
 heirarchy (read these as T1 inherits from T2 and T3, and so on):
 
                                      T4      T5
                                        *    *
                                         *  *
                                  T2      T3
                                    *    *
                                     *  *
                                      T1
							
 The depth-first search, starting at T1, yields the sequence: T2 T4 T5 T3 T1.  Then this
 is the order the CMNewValue() "..." parameters must be in.
 
 Implementation note: the "guts" of this routine is in CMVNewValue().  We just do what a
 user would do to use that routine.
*/

CMValue CM_VARARGS CMNewValue(CMObject object, CMProperty property, CMType type, ...)
{
	CMValue value;
	va_list dataInitParams;
	
	va_start(dataInitParams, type);										/* get ptr to the "..." parameters	*/
	value = CMVNewValue(object, property, type, dataInitParams);
	va_end(dataInitParams);
	
	return (value);
}


/*--------------------------------------------------------*
 | CMVNewValue - create a refNum for a new value (header) |
 *--------------------------------------------------------*
 
 This routine is the same as CMNewValue() above, except that the dynamic value data
 initialization (i.e., "...") parameters are given as a variable argument list as defined
 by the "stdarg" facility.
 
 This routine assumes the caller sets up and terminates the variable arg list using the
 "stdarg.h" calls as follows:
 
 			#include <stdarg.h>
			
 			callersRoutine(args, ...)
			{
				va_list dataInitParams;
				
				- - -
				
				va_start(dataInitParams, args);
				value = CMVNewValue(object, property, type, dataInitParams);
				va_end(dataInitParams);
				
				- - -
			}
			
 See CMNewValue() for further details.
*/

CMValue CM_FIXEDARGS CMVNewValue(CMObject object, CMProperty property, CMType type,
													 			 va_list dataInitParams)
{
	TOCObjectPtr 	 theObject;
	TOCValueHdrPtr theValueHdr, baseValueHdr;
	ContainerPtr 	 container;
	
	ExitIfBadObject(object, NULL);										/* validate object									*/
	ExitIfBadProperty(property, NULL);								/* validate property								*/
	ExitIfBadType(type, NULL);												/* validate type										*/
	
	container = ((TOCObjectPtr)object)->container;
	
	if (container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer ||
			container->targetContainer != ((TOCObjectPtr)type)->container->targetContainer) {
		ERROR3(CM_err_3Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)property)->container),
															 CONTAINERNAMEx(((TOCObjectPtr)type)->container));
		return (NULL);
	}

	container = container->updatingContainer;				/* use updating container from here on*/
	
	/* This will create a value header for an existing object or a new object with a 			*/
	/* value header but no value.  Other "CM..." calls will create the actual value(s).		*/
	
	theObject = cmDefineObject(container, ((TOCObjectPtr)object)->objectID,
														 ((TOCObjectPtr)property)->objectID,
														 ((TOCObjectPtr)type)->objectID,
														 NULL, container->generation, 0, 
														 ObjectObject, &theValueHdr);
	if (theObject == NULL) return (NULL);
	
	/* Check to see if a dynamic value must be created.  This is done by calling 					*/
	/* cmFollowBaseTypes() to do a depth-first search starting from the given type on all	*/
	/* of that type's base types.  Dynamic values are created for each type that has a		*/
	/* "use value" and "new value" handler.  The resulting dynamic value is returned or		*/
	/* the "real" value we created above if there are no dynamic values.									*/
	
	baseValueHdr = theValueHdr;
	theValueHdr = cmFollowTypes(theValueHdr, (TOCObjectPtr)type, true, &dataInitParams);
	
	baseValueHdr->useCount = 1;												/* set use count of "real" value		*/
	
	return ((CMValue)theValueHdr);										/* return real or dynamic value			*/
}


/*--------------------------------------------------------*
 | CMGetBaseValue - get the base value of a dynamic value |
 *--------------------------------------------------------*
 
 Returns the base value for a dynamic value and NULL if the value is not a dynamic value.
 It is expected that this routine will only be called from dynamic value handlers that
 support layered handlers.  Indeed, this is enforced!
*/

CMValue CM_FIXEDARGS CMGetBaseValue(CMValue value)
{
	TOCValueHdrPtr theValueHdr;
	ContainerPtr 	 container;

	ExitIfBadValue(value, NULL);											/* validate value										*/
	
	theValueHdr = (TOCValueHdrPtr)value;
	container 	= theValueHdr->container;

	if (container->getBaseValueOk <= 0) {							/* only callable from a handler			*/
		ERROR1(CM_err_CantGetBase, CONTAINERNAME);			
		return (NULL);
	}

	if (!IsDynamicValue(value))												/* if not dynamic value...					*/
		return (NULL);																	/* ...there is no base							*/
		
	return ((CMValue)DYNEXTENSIONS(value)->baseValue);/* return the base value						*/
}


/*--------------------------------------------*
 | CMGetValueSize - get the size of the value |
 *--------------------------------------------*
 
 The character size of the value data is returned.  The size returned is 0 if the value
 has no data.
 
 Note, the size returned is the size of the data as created by CMWriteValueData() (or
 CMOpenNewContainer() for embedded container values, but that's not applicable to this
 discussion here).  If this data represents represents some sort of compression scheme,
 the API has NO KNOWLEDGE of the actual size represented by that compressed data.
*/

CMSize CM_FIXEDARGS CMGetValueSize(CMValue value)
{
	CM_ULONG	 size = 0;
	
	ExitIfBadValue(value, 0);													/* validate value										*/
	
	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmGetValueSize, CMGetValueSizeOpType, "CMGetValueSize", 0);
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmGetValueSize);
			AllowCMGetBaseValue(((TOCValueHdrPtr)value)->container);	
			size = (CM_ULONG)CMDynGetValueSize(value);
			DisAllowCMGetBaseValue(((TOCValueHdrPtr)value)->container);	
			SignalDynHandlerAvailable(value, cmGetValueSize);
			return (size);
		}
	}
		
	return ((CMSize)((TOCValueHdrPtr)value)->size);		/* return size from the value hdr		*/
}


/*---------------------------------------------*
 | CMReadValueData - read the data for a value |
 *---------------------------------------------*
 
 The data, starting at the offset, for the value is read into the buffer.  The size of the
 data read is returned.  Up to maxSize characters will be read (can be 0). 
 
 The data is read starting at the offset, up to the end of the data, or maxSize characters,
 whichever comes first.  Offsets are relative to 0.  If the starting offset is greater than
 or equal to the current data size, no data is read and 0 returned.
 
 Normally CMReadValueData() is used on a container that was opened for input using
 CMOpenContainer().  However, it is permitted to read data already written to a container
 that has been opened with CMOpenNewContainer().
 
 It is an error to attempt to read a value which has no data, i.e., a value where only a
 CMNewValue() has been done.
*/

CMSize CM_FIXEDARGS CMReadValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize)
{
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue;
	ContainerPtr 	 container;
	CM_UCHAR			 *p;
	CM_ULONG			 len, remaining, totalRead, amountRead;

	ExitIfBadValue(value, 0);													/* validate value										*/
	
	container = ((TOCValueHdrPtr)value)->container;		/* NEVER use updatingContainer here!*/
	
	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmReadValueData, CMReadValueDataOpType, "CMReadValueData", 0);	
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmReadValueData);
			AllowCMGetBaseValue(container);
			totalRead = (CM_ULONG)CMDynReadValueData(value, buffer, offset, maxSize);
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(value, cmReadValueData);
			return (totalRead);
		}
	}
	
	theValueHdr = (TOCValueHdrPtr)value;							/* ("this") value may have changed!	*/
	
	#if 0	/* removed check to allow reading stuff written to an output container */
	if (container->useFlags & kCMWriting) {						/* make sure we're opend for reading*/
		ERROR1(CM_err_ReadIllegal, CONTAINERNAME);
		return (0);
	}
	#endif
	
	if (maxSize == 0 || buffer == NULL) return (0);
		
	/* Scan across the value list for the value header and concat data into buffer.  A		*/
	/* value list will exist for continued values.  The user views the data as one				*/
	/* contiguous chunk of stuff.  But a continued value is acutally a set of non-				*/
	/* contiguous smaller chunks that we concat into the user's buffer. The user specifies*/
	/* a starting offset with respect to his or her view of the contiguous data.  We must	*/
	/* map that offset into an offest within the proper starting continued value segment.	*/
	/* Fortunately (yea, right) we have cmGetStartingValue() to save the day!  It does 		*/
	/* the mapping (funny thing about that, isn't it?).																		*/
	
	if (cmIsEmptyList(&theValueHdr->valueList)) {			/* must have a value								*/
		ERROR1(CM_err_HasNoValue, CONTAINERNAME);
		return (0);
	}
	
	theValue = cmGetStartingValue(theValueHdr, offset, &offset); /* get start seg/offset	*/
	if (theValue == NULL) return (0);									/* offset out of range							*/

	p 				= (CM_UCHAR *)buffer;										/* p points to next byte to read to	*/
	remaining = (CM_ULONG)maxSize;										/* remaining bytes to read					*/
	totalRead = 0;																		/* how much we actually do read			*/

	while (theValue && remaining) {										/* read value (segments)...					*/
		len = cmGet1ValueSize(theValue) - offset;				/* (note, immediates work here)			*/
		if (len > remaining) len = remaining;						/* (note, so do global names)				*/
		
		if (len) {																			/* if more is wanted...							*/
			amountRead = cmRead1ValueData(theValue, p, offset, len); /* read 1 value segment	*/
			totalRead += amountRead;											/* keep track of how much we read		*/
			if (amountRead < len) break;									/* if end of total value, we're done*/
			p += len;																			/* point at next free byte in buffer*/
		}
		
		offset = 0;																				/* full segments from now on			*/
		remaining -= len;																	/* adjust what's remaining to read*/
		theValue = (TOCValuePtr)cmGetNextListCell(theValue); /* point to next value seg			*/
	}

	return (totalRead);																/* return total amount concatenated	*/
}


/*------------------------------------------*
 | CMWriteValueData - write data to a value |
 *------------------------------------------*
 
 The buffer is written to the container and defined as the data for the value.  If the
 value already has data associated with it, the buffer overwrites the "old" data starting
 at the offset character position.  size bytes are written.  size can be 0.
 
 If the current size of the value data is T (it will be 0 for a new value created by
 CMNewValue()), then the offset may be any value from 0 to T.  That is, existing data
 may be overwritten or the value extended with new data.  The value of T can be gotton
 using CMGetValueSize().  Note, no "holes" can be created.  An offset cannot be greater
 than T.
 
 Once data has been written to the container, it may be read using CMReadValueData(). Note,
 that CMReadValueData() is also used for containers opened with for input using
 CMOpenContainer().  It thus can be used for all kinds of opens. The converse is not true.
 CMWriteValueData() may only be used for a container opend for writing (or converting)
 using CMOpenNewContainer().
 
 CMWriteValueData() calls for a particular value do not have to be contiguous.  Writes 
 for other values can be done.  The API, specifically, this routine here, takes care of
 generating "continued" value data (segments) for a value.  The data is physically not
 contiguous in the container with such a case.  CMWriteValueData() hides this by allowing
 the user to view the data as contiguous.  The input offset is mapped into the proper
 starting segment and offset within that.
 
 It is an error to write to protected values.  This includes the predefined TOC objects
 (seed and offset values) and objects representing currently opened embedded containers.
 
 For creating embedded containers, CMOPenNewContainer() is used instead of 
 CMWriteValueData().  See CMNewValue() and CMOpenContainer() for further details.
*/

void CM_FIXEDARGS CMWriteValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize size)
{
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue, theNextValue;
	ContainerPtr 	 container;
	CM_UCHAR	 		 *p;
	CM_CHAR					offsetStr[15];
	CM_ULONG			 len, remaining, amountWritten, nextFree, valueSize;
	TOCValueBytes  valueBytes;
#if SizeReadBeforeWrite
	CM_CHAR				 tempBuf[SizeReadBeforeWrite];
#endif

	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
	if (buffer == NULL) return;

	theValueHdr = (TOCValueHdrPtr)value;
	container 	= theValueHdr->container->updatingContainer;
	
	if ((container->useFlags & kCMWriting) == 0) {		/* make sure opened for writing			*/
		ERROR1(CM_err_WriteIllegal1, CONTAINERNAME);
		return;
	}
	
	if ((theValueHdr->valueFlags & ValueProtected) != 0){	/* can't write if protected!		*/
		ERROR1(CM_err_WriteIllegal2, CONTAINERNAME);
		return;
	}
	
	if ((theValueHdr->valueFlags & ValueGlobal) != 0) {		/* can't write to global names	*/
		ERROR1(CM_err_CantWriteGlbl, CONTAINERNAME);
		return;
	}

	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmWriteValueData, CMWriteValueDataOpType, "CMWriteValueData", CM_NOVALUE);	
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmWriteValueData);
			AllowCMGetBaseValue(container);	
			CMDynWriteValueData(value, buffer, offset, size);
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(value, cmWriteValueData);
			return;
		}
		theValueHdr = (TOCValueHdrPtr)value;						/* ("this") value may have changed!	*/
	}

	valueSize = CMGetValueSize((CMValue)theValueHdr);	/* get current size of value				*/
	if (valueSize == 0)																/* no data, treat as empty list			*/
		cmFreeAllListCells(&theValueHdr->valueList, SESSION);

	/* If the value list is empty, create the first or only value for this value header.	*/
	/* An immediate value is created if the value size is less than or equal to the 			*/
	/* sizeof(CM_ULONG).  Otherwise we write the data to the container and set the TOC 		*/
	/* info with the offset to it.																												*/
	
	if (cmIsEmptyList(&theValueHdr->valueList)) {			/* if we have a empty value list...	*/
		if (offset > 0) {																/* ...create initial value					*/
			ERROR2(CM_err_Offset2Big, cmltostr(offset, 1, false, offsetStr), CONTAINERNAME);
			return;
		}
		
		if (size <= sizeof(CM_ULONG)) {									/* we can make value immediate...		*/
			(void)cmSetValueBytes(container, &valueBytes, Value_Imm_Chars, (CM_ULONG)buffer, size);
			cmAppendValue(theValueHdr, &valueBytes, kCMImmediate);
			cmTouchImmediateValue(theValueHdr);						/* touch for updating if necessary	*/
		} else if (size != 0) {													/* value must be written...					*/
			#if 0
			nextFree = CMgetContainerSize(container);
			CMfseek(container, 0, kCMSeekEnd);												/* position to current eof					*/
			if (CMfwrite(container, buffer, sizeof(CM_UCHAR), size) != size) {
				ERROR1(CM_err_BadWrite, CONTAINERNAME);
				return;
			}
			(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, nextFree, size);
			cmAppendValue(theValueHdr, &valueBytes, 0);
			container->physicalEOF = nextFree + size;			/* update next free container byte	*/
			SetLogicalEOF(container->physicalEOF);				/* logical EOF == physical EOF			*/
			#endif
			if ((CMSize)cmWriteData(theValueHdr, (CM_UCHAR *)buffer, (CM_ULONG)size) != size)
				ERROR1(CM_err_BadWrite, CONTAINERNAME);
			cmTouchEditedValue(theValueHdr);							/* touch for updating if necessary	*/
		}

		return;																					/* exit															*/
	} /* end of 1st value */

	if (size == 0) return;

	/* At this point we have EXISTING data (possibly already continued).  If the offset		*/
	/* says that we are NOT writing to the end of the data, then we MUST be overwriting		*/
	/* some of the existing data.  In other words, the offset must be less than the total	*/
	/* size or we have an error.  We can only overwrite or append (i.e., concat).  We			*/
	/* cannot create a "hole". In the code that follows we consume enough of the input		*/
	/* buffer to do the overwriting staring at the offset.  If we consume it all, we're		*/
	/* done.  If not, we have reached the end of the existing value and we degenerate into*/
	/* the concat case.  																																	*/
	
	if (valueSize != offset) {												/* here we must be overwriting...		*/
		if (TouchIt(container, theValueHdr->container)){/* if recording updates...					*/
#if SizeReadBeforeWrite
			/* for small write, compare with original value to skip write											*/
			if (size <= SizeReadBeforeWrite)
				if (offset+size <= theValueHdr->size)
					if (CMReadValueData(value, &tempBuf, offset, size) == size)
						if (memcmp(tempBuf, buffer, size) == 0)
							return;																	/* no change, don't need to do it	*/			
#endif

			CMDeleteValueData(value, offset, size);				/* ...do overwrites this way!				*/
			CMInsertValueData(value, buffer, offset, size);
			return;																				/* we're through										*/
		}
		
		theValue  = cmGetStartingValue(theValueHdr, offset, &offset);
		if (theValue == NULL) {													/* offset MUST be IN the value			*/
			ERROR2(CM_err_Offset2Big, cmltostr(offset, 1, false, offsetStr), CONTAINERNAME);
			return;
		}
	
		p 				= (CM_UCHAR *)buffer;									/* p points to next byte to write		*/
		remaining = (CM_ULONG)size;											/* remaining bytes to write					*/
	
		while (theValue && remaining) {									/* overwrite existing value data		*/
			theNextValue = (TOCValuePtr)cmGetNextListCell(theValue); /* get next value seg		*/
			len = cmGet1ValueSize(theValue) - offset;			/* (note, immediates work here!)		*/
			if (len > remaining) len = remaining;
			
			amountWritten = cmOverwrite1ValueData(theValue, p, offset, len); /* write 1 seg.	*/
			if (amountWritten == 0) return;
			
			p += amountWritten;
			
			offset = 0;																		/* full segments from now on				*/
			remaining -= amountWritten;										/* adjust what's remaining to write	*/
			theValue = theNextValue; 											/* point to next value seg					*/
		}
	
		if (remaining == 0) return;											/* yikes!  all of it was written!		*/
		
		size = (CMSize)remaining;												/* prepare to write rest at end			*/
		buffer = (CMPtr)p;															/* simply fall through to next case	*/
	} /* end of overwriting existing value data */

	/* At this point we want to concat the new data on to the end of the existing data.		*/
	/* We are doing this because the input offset was equal to the data size or we fell		*/
	/* through from above because there are still more bytes to write in an overwrite			*/
	/* and these bytes "stick" off the end of the current data.  Again this is a concat		*/
	/* case.  Neat isn't it?																															*/
		
	/* Well, actually no!  Unlike the code above here we must handle immediate data				*/
	/* explicitly.  It's sort of a hassle.  That's because the amount of new data to			*/
	/* concat to an immediate might mean that it can't be immediate any more. Immediate		*/
	/* data is limited in size to less than or equal to sizeof(CM_ULONG).  If we can cram	*/
	/* the new data we do it. If we can't, then we must convert the immediate to a non-		*/
	/* immediate by writing the data to the container and changing its TOC info to be			*/
	/* and offset.  We then have a non-immediate which falls through to the standard			*/
	/* concat code for non-immediates.  The way all these cases "fall" into one another		*/
	/* is the "neat" thing here.																													*/
	
	theValue = (TOCValuePtr)cmGetListTail(&theValueHdr->valueList); /* use tail of list		*/
	
	if (theValue->flags & kCMImmediate) {							/* if current value immediate...		*/	
		if (valueSize + size <= sizeof(CM_ULONG))	{			/* cram new data if we can...				*/
			if (size > 0) {																/* there must be some data to write	*/
				memcpy(theValue->value.imm.ucharsValue + valueSize, (CM_CHAR *)buffer, (size_t)size);
				theValue->value.notImm.valueLen = valueSize + size;
				theValueHdr->size += size;
				cmTouchImmediateValue(theValueHdr);					/* touch for updating if necessary	*/
			}
			return;																				/* that's all we need to do!				*/
		}
		
		if (!cmConvertImmediate(theValue))							/* convert the immediate...					*/
			return;
	} /* end of immediate */
		
	/* We are now ready to concat the new data on to the end of the existing data.  Here	*/
	/* too life is not simple.  A "concat" means here means a physical concat if the old	*/
	/* data was the last thing written to the SAME container. If it wasn't we must create */
	/* a new value entry on the valueHdr's value list to represent a continued value.			*/
	/* Note the emphasis on "SAME" container.  We could be writing updates for an "old"		*/
	/* container to be recorded in a new updating container.															*/
	
	nextFree = CMgetContainerSize(container);
	if (theValue->value.notImm.value + theValue->value.notImm.valueLen == nextFree &&
			theValue->container == container) {						/* remember, must be SAME container!*/
		if (size > 0) {																	/* there must be some data to write	*/
			CMfseek(container, 0, kCMSeekEnd);						/* make sure of container position	*/
			if (CMfwrite(container, buffer, sizeof(CM_UCHAR), size) != size) {
				ERROR1(CM_err_BadWrite, CONTAINERNAME);
				return;
			}
			container->physicalEOF = nextFree + size;			/* update next free container byte	*/
			SetLogicalEOF(container->physicalEOF);				/* logical EOF == physical EOF			*/
			theValue->value.notImm.valueLen += size;			/* update total size								*/
			theValueHdr->size += size;										/* keep size in valueHdr in sync		*/
			cmTouchEditedValue(theValueHdr);							/* touch for updating if necessary	*/
		}
		return;
	}
	
	/* Too bad!  We couldn't really do a concat.  We must create a continued value...			*/
	
	#if 0
	theValue->flags |= kCMContinued;									/* flag the current value as cont'd	*/
	theValueHdr->valueFlags |= ValueContinued;				/* also set a more convenient flag	*/
	
	CMfseek(container, 0, kCMSeekEnd);
	if (CMfwrite(container, buffer, sizeof(CM_UCHAR), size) != size) {
		ERROR1(CM_err_BadWrite, CONTAINERNAME);
		return;
	}
	container->physicalEOF = nextFree + size;					/* update next free container byte	*/
	SetLogicalEOF(container->physicalEOF);						/* logical EOF == physical EOF			*/
	
	(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, nextFree, size);
	cmAppendValue(theValueHdr, &valueBytes, 0);	
	#endif
	
	if ((CMSize)cmWriteData(theValueHdr, (CM_UCHAR *)buffer, (CM_ULONG)size) != size)
		ERROR1(CM_err_BadWrite, CONTAINERNAME);
	
	/* If we're recording updates, then define touched list entry for the write...				*/

	if (size > 0) cmTouchEditedValue(theValueHdr);		/* touch for updating if necessary	*/
}


/*------------------------------------------------------*
 | CMDefineValueData - define existing data for a value |
 *------------------------------------------------------*
 
 Existing data in the container is defined as the data for the value. The offset specifies
 a container offset from the beginning of the container.  No data is written to the
 container.  This call is used only to define values for stuff that is in a non-container
 opened to be converted to container format (kCMConverting useFlags passed to a 
 CMOpenNewContainer()).  It is an error to use this call in any other context.  The offset
 therefore, must be in the range of 0 to the N-1, where N is the size of preexisting data
 at the time the container was opened.
 
 Additional calls to CMDefineValueData() for the SAME value will define additional. i.e.,
 continued, segments when the offset produces noncontiguous data definition. If the size
 of the last (most recent) value segment is T, and the offset for that segment is such 
 that offset+T equals the offset for the additional segment, then the last segment is
 simply extended.  A new segment is not created.  This simulates the exact same sequence
 of events as done by CMWriteValueData().
*/

void CM_FIXEDARGS CMDefineValueData(CMValue value, CMCount offset, CMSize size)
{
	TOCValueHdrPtr theValueHdr;
	ContainerPtr 	 container;
	TOCValuePtr		 theValue;
	TOCValueBytes  valueBytes;
	char					 offsetStr[15];

	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
		
	theValueHdr = (TOCValueHdrPtr)value;
	container 	= theValueHdr->container;
	
	if ((container->useFlags & kCMConverting) == 0) {	/* must be converting to a container*/
		ERROR1(CM_err_NotConverting, CONTAINERNAME);
		return;
	}
	
	#if 0	/* now allow defining of additional continued value segments										*/
	if (!cmIsEmptyList(&theValueHdr->valueList)) {		/* must not have any previous data	*/
		ERROR1(CM_err_HasValue, CONTAINERNAME);
		return;
	}
	#endif
	
	/* Values defined here for a file to be converted to a container must only have 			*/
	/* offsets within the range of the original data.  maxValueOffset was the offset to		*/
	/* the last byte in the original data.  We got that when the container was opened.		*/
	
	if ((CM_ULONG)offset > container->maxValueOffset ||
			(CM_ULONG)offset + size > container->maxValueOffset) {
		ERROR2(CM_err_BadDefineData, cmltostr(offset, 1, false, offsetStr), CONTAINERNAME);
		return;
	}
	
	/* If this is the first value definition then no additional work need be done other 	*/
	/* than to create the first value segment.																						*/
	
	if (cmIsEmptyList(&theValueHdr->valueList)) {			/* define 1st data segment					*/
		(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, offset, size);
		cmAppendValue(theValueHdr, &valueBytes, 0);
		return;
	} /* end of 1st value */
	
	/* At this point we want to add an additional data definition to an existing value.		*/
	/* We must make sure the existing value wasn't created for some other purpose, i.e.		*/
	/* it can't be an immediate.																													*/
	
	theValue = (TOCValuePtr)cmGetListTail(&theValueHdr->valueList); /* use tail of list		*/
	if (theValue->flags & kCMImmediate) {										/* don't append to this!			*/
		ERROR1(CM_err_BadDefineType, CONTAINERNAME);
		return;
	}
	
	/* If the new definition is such that we can effectively do a concat, we do that 			*/
	/* instead of creating an additional value segment.																		*/
	
	if (theValue->value.notImm.value + theValue->value.notImm.valueLen == offset) {
		theValue->value.notImm.valueLen += size;				/* update total size								*/
		theValueHdr->size += size;											/* keep size in valueHdr in sync		*/
		return;
	}
	
#if CMKEEP_CONTINUE_FLAG
	/* Have a noncontiguous definition.  So here we must create an additional continued		*/
	/* value segment.																																			*/
	
	theValue->flags |= kCMContinued;									/* flag the current value as cont'd	*/
	theValueHdr->valueFlags |= ValueContinued;				/* also set a more convenient flag	*/
#endif
	
	(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, offset, size);
	cmAppendValue(theValueHdr, &valueBytes, 0);	
}


/*----------------------------------------------*
 | CMInsertValueData - insert data into a value |
 *----------------------------------------------*
 
 The buffer is inserted into the value's data at the specified offset.  size bytes are
 inserted.
 
 If the current size of the value data is T (it will be 0 for a new value created by
 CMNewValue()), then the offset may be any value from 0 to T.  That is, the insertion
 may be anywhere within the data value or the value extended with new data.  The value of
 T can be gotton using CMGetValueSize().  Note, no "holes" can be created.  An offset
 cannot be greater than T.  Also, note, that an insertion at offset T is identical to
 a CMWriteValueData() to the same place.
 
 Once data has been written to the container, it may be read using CMReadValueData(). Note,
 that CMReadValueData() is also used for containers opened with for input using
 CMOpenContainer().  It thus can be used for all kinds of opens. The converse is not true.
 CMWriteValueData() may only be used for a container opend for writing (or converting)
 using CMOpenNewContainer().
  
 It is an error to write to protected values. This includes the predefined TOC objects
 (seed and offset values) and objects representing currently opened embedded containers.
 
 For creating embedded containers, CMOPenNewContainer() is used instead of 
 CMWriteValueData().  See CMNewValue() and CMOpenContainer() for further details.
 
 From an implementation point of view, continued (i.e., segmented) values are handled by
 spliting them into one or two new segments.  The insertion is always a new segment. The
 original segment to be inserted to will split into two segments if the insertion is in
 to its middle.  It will remain intact and unsplit if we are inserting into its beginning.
 The new segment is simply inserted in front of it.  For the split case the new segment 
 must be inserted between the split pieces.
*/

void CM_FIXEDARGS CMInsertValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize size)
{
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue, insBeforeValue;
	ContainerPtr 	 container;
	CM_UCHAR			 *p, *q, *ins;
	CM_ULONG			 valueSize, insSize, segOffset, insOffset, offset1, actualSize;
	char					 offsetStr[15];

	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
	if (size == 0 || buffer == NULL) return;

	theValueHdr = (TOCValueHdrPtr)value;
	container 	= theValueHdr->container->updatingContainer;
	
	if ((container->useFlags & kCMWriting) == 0) {		/* make sure opened for writing			*/
		ERROR1(CM_err_WriteIllegal1, CONTAINERNAME);
		return;
	}
	
	if ((theValueHdr->valueFlags & ValueProtected) != 0){	/* can't write if protected!		*/
		ERROR1(CM_err_WriteIllegal2, CONTAINERNAME);
		return;
	}
	
	if ((theValueHdr->valueFlags & ValueGlobal) != 0) {		/* can't write to global names	*/
		ERROR1(CM_err_CantWriteGlbl, CONTAINERNAME);
		return;
	}

	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmInsertValueData, CMInsertValueDataOpType, "CMInsertValueData", CM_NOVALUE);	
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmInsertValueData);
			AllowCMGetBaseValue(container);	
			CMDynInsertValueData(value, buffer, offset, size);
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(value, cmInsertValueData);
			return;
		}
		theValueHdr = (TOCValueHdrPtr)value;						/* ("this") value may have changed!	*/
	}
	
	/* If the value list is empty, create the first or only value for this value header.	*/
	/* An immediate value is created if the value size is less than or equal to the 			*/
	/* sizeof(CM_ULONG). Otherwise we insert (and write) the data to the container and set*/
	/* the TOC info with the offset to it.  An insert into an empty value list is exactly	*/
	/* the same as writing to it the first time.  So to save some code space a call is 		*/
	/* made to CMWriteValueData() to do the actual work.																	*/
	
	if (cmIsEmptyList(&theValueHdr->valueList)) {			/* if we have a empty value list...	*/
		CMWriteValueData(value, buffer, offset, size);	/* write insert (offset must be 0)	*/
		return;
	} /* end of 1st value */

	/* At this point we have EXISTING data (possibly already continued).  If the offset		*/
	/* says that we are NOT writing to the end of the data, then we MUST be inserting the	*/
	/* new data.  If we are writing to the end, then the insert is exactly like a write.	*/
	/* In that case CMWriteValueData() is called to do the actual work.										*/
	
	valueSize = CMGetValueSize((CMValue)theValueHdr);	/* get current size of value				*/
	
	if (valueSize == offset) {												/* inserting to end of current value*/
		CMWriteValueData(value, buffer, offset, size);	/* append insert to end of data			*/
		return;
	} /* end of appending */
		
	/* Now we know we're actually doing an insert.  There are three cases:								*/
	
	/*	(1). Inserting into an immediate.  If the size of the immediate plus the size of	*/
	/*			 the insert still fits in a immediate the result will remain an immediate.		*/
	/*			 Otherwise, the immediate is converted to a non-immediate and we have cases		*/
	/*			 (2) or (3).																																	*/
	/*	(2). Inserting into the start of an existing segment (segment offset 0).  The 		*/
	/* 			 insert is made a segment and placed before the existing segment.							*/
	/*	(3). Inserting into the middle of a non-immediate value segment.  The existing		*/
	/*			 segment must be split into two segments and the insert segment placed between*/
	/*			 them.																																				*/
	
	/* First case (1) -- immediates...																										*/
	
	theValue = (TOCValuePtr)cmGetListTail(&theValueHdr->valueList); /* use tail of list		*/
	
	if (theValue->flags & kCMImmediate) {							/* if current value immediate...		*/	
		if (valueSize + size <= sizeof(CM_ULONG))	{			/* insert new data if we can...			*/
			insSize = (CM_ULONG)size;
			p = theValue->value.imm.ucharsValue + valueSize;
			q = p + size;
			ins = theValue->value.imm.ucharsValue + offset;
			while (--p >= ins) *--q = *p;
			q = (CM_UCHAR *)buffer;
			while (size--) *++p = *q++;
			theValue->value.notImm.valueLen = valueSize + insSize;
			theValueHdr->size += insSize;
			cmTouchImmediateValue(theValueHdr);						/* touch for updating if necessary	*/
			return;																				/* that's all we need to do!				*/
		}
		
		if (!cmConvertImmediate(theValue))							/* convert the immediate...					*/
			return;																				/* (exit if convert failure)				*/
	} /* end of immediate */

	/* At this point we have either cases (2) or (3).  Get the offset within the segment 	*/
	/* and the segment itself to determine which case we got.															*/
	
	insBeforeValue  = cmGetStartingValue(theValueHdr, offset, &segOffset);
	if (insBeforeValue == NULL) {											/* offset MUST be IN the value			*/
		ERROR2(CM_err_Offset2Big, cmltostr(offset, 1, false, offsetStr), CONTAINERNAME);
		return;
	}

	/* Write the new inserted data to the container...																		*/

	insOffset = cmGetFreeListEntry(container, (CM_ULONG)size, true, &actualSize);
	if (actualSize != 0)															/* if we got some to reuse...				*/
		CMfseek(container, insOffset, kCMSeekSet);			/* ...position to write over it			*/
	else {																						/* if couldn't find a fit...				*/
		insOffset = CMgetContainerSize(container);			/* write insert to end of container	*/
		CMfseek(container, 0, kCMSeekEnd);
	}
	
	if (CMfwrite(container, buffer, sizeof(CM_UCHAR), size) != size) {
		ERROR1(CM_err_BadWrite, CONTAINERNAME);
		return;
	}
	
	offset1 = insOffset + size;												/* update logical OR physical EOF		*/
	if (actualSize == 0)
		container->physicalEOF = offset1;								/* update next free container byte	*/
	SetLogicalEOF(offset1);														/* set logical EOF (may != physical)*/
	
	/* Create the new value segment to point to the inserted data and insert the segment	*/
	/* into the appropriate place.  For case (2) segOffset will be 0 and for (3) non-zero.*/
	/* The actual segment create and insert is done by a separate routine. This is due to	*/
	/* possible updating which uses the same routine. See comments in cmInsertNewSegment()*/
	/* (in   ValueRtn.c   ).																															*/
	
	cmInsertNewSegment(theValueHdr, insBeforeValue, segOffset, insOffset, size);
		
	/* If we're recording updates, then define touched list entry for the insert...				*/

	cmTouchEditedValue(theValueHdr);									/* touch for updating if necessary	*/
}


/*----------------------------------------------*
 | CMDeleteValueData - delete data from a value |
 *----------------------------------------------*
 
 Deletes size bytes from the value data starting at the offset.  If the offset is greater
 than the value data size, nothing is deleted.  The amount to delete may be greater than
 the current data size.  In that case, all the data starting from the offset will be
 deleted.  If ALL the data is deleted, the value is defined as null, i.e. a data length of
 0.
*/

void CM_FIXEDARGS CMDeleteValueData(CMValue value, CMCount offset, CMSize size)
{
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue;
	ContainerPtr 	 container;
	CM_UCHAR		   *p, *q;
	CM_ULONG			 valueSize, startOffset, endOffset;
	
	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
	if (size == 0) return;

	theValueHdr = (TOCValueHdrPtr)value;
	container 	= theValueHdr->container->updatingContainer;
	
	if ((container->useFlags & kCMWriting) == 0) {		/* make sure opened for writing			*/
		ERROR1(CM_err_WriteIllegal1, CONTAINERNAME);
		return;
	}
	
	if ((theValueHdr->valueFlags & ValueProtected) != 0){	/* can't write if protected!		*/
		ERROR1(CM_err_WriteIllegal2, CONTAINERNAME);
		return;
	}
	
	if ((theValueHdr->valueFlags & ValueGlobal) != 0) {		/* can't write to global names	*/
		ERROR1(CM_err_CantWriteGlbl, CONTAINERNAME);
		return;
	}

	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmDeleteValueData, CMDeleteValueDataOpType, "CMDeleteValueData", CM_NOVALUE);	
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmDeleteValueData);
			AllowCMGetBaseValue(container);	
			CMDynDeleteValueData(value, offset, size);
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(value, cmDeleteValueData);
			return;
		}
		theValueHdr = (TOCValueHdrPtr)value;						/* ("this") value may have changed!	*/
	}
	
	/* If the offset specifies that the end of the value is to be deleted, do nothing. We	*/
	/* let this through since if the user doesn't want any data there s/he's got exactly	*/
	/* what s/he thinks, i.e., no data there!  There never was, but who cares. Note, that	*/
	/* by asking this question we also know whether any data exists for the value. If the */
	/* offset is less than the size, then the size must be non-zero and hence we have some*/
	/* deleatable data.																																		*/
	
	valueSize = CMGetValueSize((CMValue)theValueHdr);	/* get current size of value				*/
	if (offset >= valueSize) return;									/* if deleting from end, just exit	*/
	
	/* Ok, we got to do some work and delete something.  Let's get the starting	*/
	/* and ending segments and the offsets within them for starters.  [If you haven't 		*/
	/* guessed, I am making this algorithm up as I go along -- this should be interesting]*/
	
	startOffset = offset;															/* initially get data offsets...		*/
	endOffset   = startOffset + size - 1;							/* end offset limited to end of data*/
	if (endOffset >= valueSize) 
		endOffset = valueSize - 1;
	
	/* Immediate values are handled separately...																					*/
	
	theValue = (TOCValuePtr)cmGetListTail(&theValueHdr->valueList);
	if (theValue->flags & kCMImmediate) {							/* if immediate...									*/
		p  = theValue->value.imm.ucharsValue;						/* ...do a simple byte delete				*/
		q  = p + startOffset;
		p += endOffset + 1;
		theValueHdr->size -= endOffset - startOffset + 1;
		theValue->value.notImm.valueLen = theValueHdr->size;
		while (++endOffset < valueSize) *q++ = *p++;	
		cmTouchImmediateValue(theValueHdr);							/* touch for updating if necessary	*/
		return;																					/* that's it for immediates					*/
	}

	/* Delete the data from the segments.  The actual deletion is done by a separate 			*/
	/* routine.  This is due to possible updating which wants to use the same routine.  	*/
	/* See comments in cmDeleteSegmentData() (in   ValueRtn.c     ).												*/
	
	cmDeleteSegmentData(theValueHdr, startOffset, endOffset);
	
	/* If we're recording updates, then define touched list entry for the data delete...	*/
	
	cmTouchEditedValue(theValueHdr);									/* touch for updating if necessary	*/
}


/*-------------------------------------------------------*
 | CMMoveValue - move a value from one object to another |
 *-------------------------------------------------------*
 
 Moves the specified value from its original object property to the specified object
 property.  The value is physically deleted from its original object/property as if a
 CMDeleteValue() were done on it.  If the value deleted is the only one for the property,
 the property itself is deleted as in CMDeleteObjectProperty(). 
 
 The value is added to the "to"s object propery in a manner similar to a CMNewValue().
 The order of the values for both the value's original object property and for the value's
 new object property may be changed.
 
 Note, that although the effect of a move is a combination CMDeleteValue()/CMNewValue(),
 THE INPUT REFNUM REMAINS VALID!  Its association is now with the new object property.
 
 This operation may be done at any time.  No data need be assoicated with the value at the
 time of the move.  Only moves WITHIN THE SAME CONTAINER are allowed.
 
 Internal note: because the refNum remains the same we don't have to worry about protected
 values here.  
*/

void CM_FIXEDARGS CMMoveValue(CMValue value, CMObject object, CMProperty property)
{
	TOCValueHdrPtr theFromValueHdr, theRealValueHdr, botLayerValueHdr;
	ContainerPtr	 container;
	CMObjectID		 typeID;

	ExitIfBadValue(value, CM_NOVALUE);										/* validate value								*/
	
	theFromValueHdr = (TOCValueHdrPtr)value;
	container = theFromValueHdr->container;
	
	ExitIfBadObject(object, CM_NOVALUE);									/* validate object							*/
	ExitIfBadProperty(property, CM_NOVALUE);							/* validate property						*/
	
	if (container->targetContainer != ((TOCObjectPtr)object)->container->targetContainer ||
			container->targetContainer != ((TOCObjectPtr)property)->container->targetContainer) {
		ERROR3(CM_err_3Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)object)->container),
															 CONTAINERNAMEx(((TOCObjectPtr)property)->container));
		return;
	}
	
	if ((container->updatingContainer->useFlags & kCMWriting) == 0) {	
		ERROR1(CM_err_MoveIllegal, CONTAINERNAME);
		return;
	}
	
	/* If the value header is a dynamic value then we must move both the dynamic value 		*/
	/* and its base ("real") value.  These are linked with the base value pointing to the	*/
	/* its dynamic value.  For layered dynamic values, each dynamic value is back linked	*/
	/* to its base with the bottom layer having the "real" value as its base.  Only the		*/
	/* bottom layer is actually on the special property chain for dynamic values 					*/
	/* associated with its "real value object.  Only the top layer should ever be passed. */
	/* We can check for this.  We also get the bottom layer and "real" value header 			*/
	/* pointers.  You will see how we use these after we get them and do the validation.	*/
	
	if (IsDynamicValue(theFromValueHdr)) {								/* if we have dynamic value...	*/
		theRealValueHdr = theFromValueHdr;									/* start with current value			*/
		do {																								/* search for base value...			*/
			if ((theRealValueHdr->valueFlags & ValueOffPropChain) == 0)
				botLayerValueHdr = theRealValueHdr;							/* remember where bot. layer is	*/
			theRealValueHdr = DYNEXTENSIONS(theRealValueHdr)->baseValue;
		} while (IsDynamicValue(theRealValueHdr));					/* loop till we find real value	*/
		
		if (theRealValueHdr->dynValueData.dynValue == NULL ||	/* do the validation...				*/
				theRealValueHdr->dynValueData.dynValue != theFromValueHdr) {
			ERROR2(CM_err_BadRealValue, "CMMoveValue", CONTAINERNAME); /* ...oops!						*/
			return;
		}
		
		/* We now have the bottom (or only) layer value header and its base ("real") value. */
		/* We can move the bottom layer to the destination by simply recursively calling 		*/
		/* ourselves.  The bottom layer is, from a data structure point of view, a value 		*/
		/* header on a property chain.  All the higher layers sort of float and will still 	*/
		/* be pointing to the moved value.  In order though to pull this "stunt", we must 	*/
		/* temporarily pretend that the dynamic value isn't a dynamic value. That way we 		*/
		/* won't get back in this dynamic value code and will simply move it with the code 	*/
		/* outside this if block.  Note the object we use here is the destination object, 	*/
		/* but the property is the one corresponding to dynamic values.											*/
		
		botLayerValueHdr->valueFlags &= ~ValueDynamic;			/* pretend it's not dynamic			*/
		typeID = botLayerValueHdr->typeID;									/* hold on to the type ID...		*/
		botLayerValueHdr->typeID = 0;												/* stop potential dup types errs*/
		CMMoveValue((CMValue)botLayerValueHdr,object,container->dynValueProperty);/* move it*/
		botLayerValueHdr->typeID = typeID;									/* put back type ID							*/
		botLayerValueHdr->valueFlags |= ValueDynamic;				/* put back the flag						*/
		
		/* Now it's the "real" base value's turn to be moved.  It does not need further			*/
		/* validating.  So all we need to do is "fall" through to the normal value case			*/
		/* below.  It will move to the destination and be placed on the desired property 		*/
		/* chain.  The dynamic value (layer) is there "waiting" for it.  Since all the 			*/
		/* refNums remain the same and so do the fields (except for the property links of 	*/
		/* course), everything is properly linked.																					*/
		
		theFromValueHdr = theRealValueHdr;									/* pretend base was passed			*/
	} /* dynamic value */
	
	/* If we're recording updates, then define touched list entry for the move. This MUST */
	/* be done before the move so that the touch list entry can be generated in terms of 	*/
	/* the original object, property, and type.  The move can change them.								*/
	
	cmTouchMovedValue(theFromValueHdr, theFromValueHdr->theProperty->theObject,
										(TOCObjectPtr)object, ((TOCObjectPtr)property)->objectID);
	
	/* Move the value (header).  The actual move is done by a separate routine.  This is 	*/
	/* due to possible updating which wants to use the same routine.  See comments in 		*/
	/* cmMoveValueHdr() (in   ValueRtn.c   ).																							*/
	
	cmMoveValueHdr(theFromValueHdr, object, property);
}


/*--------------------------------------------*
 | CMGetValueInfo - return info about a value |
 *--------------------------------------------*
 
 The specified information for the refNum associated with a value is returned. A parameter
 may be NULL indicating that info is not needed.
*/

void CM_FIXEDARGS CMGetValueInfo(CMValue value, CMContainer CM_PTR *container0,
																 CMObject CM_PTR *object, CMProperty CM_PTR *property,
																 CMType CM_PTR *type, CMGeneration CM_PTR *generation)
{
	TOCValueHdrPtr theValueHdr;
	ContainerPtr	 container;													/* this is to make error macro work	*/
	
	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
	
	container = ((TOCValueHdrPtr)value)->container;

	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmGetValueInfo, CMGetValueInfoOpType, "CMGetValueInfo", CM_NOVALUE);	
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmGetValueInfo);
			AllowCMGetBaseValue(container);	
			CMDynGetValueInfo(value, container0, object, property, type, generation);
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(value, cmGetValueInfo);
			return;
		}
	}
		
	theValueHdr = (TOCValueHdrPtr)value;							/* ("this") value may have changed!	*/
	
	if (container0) 																	/* container...											*/
		*container0 = (CMContainer)container;					
	
	if (object)																				/* object...												*/
		*object = (CMObject)theValueHdr->theProperty->theObject;
	
	if (property)																			/* property...											*/
		*property = (CMProperty)cmFindObject(container->toc, theValueHdr->theProperty->propertyID);
	
	if (type) 																				/* type...													*/
		*type = (CMType)cmFindObject(container->toc, theValueHdr->typeID);
		
	if (generation)																		/* generation...										*/
		*generation = theValueHdr->generation;
}


/*------------------------------------------*
 | CMSetValueType - set the type of a value |
 *------------------------------------------*
 
 The type ID from the type is set for the specified value.
 
 What do you know?  A short comment!
*/

void CM_FIXEDARGS CMSetValueType(CMValue value, CMType type)
{
	TOCValueHdrPtr theValueHdr, theValueHdr0;
	ContainerPtr	 container;
	CM_ULONG			 typeID;
	
	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
	ExitIfBadType(type, CM_NOVALUE);									/* validate type										*/
	
	container = ((TOCValueHdrPtr)value)->container;
	
	if (container->targetContainer != ((TOCObjectPtr)type)->container->targetContainer) {
		ERROR2(CM_err_2Containers, CONTAINERNAMEx(container),
															 CONTAINERNAMEx(((TOCObjectPtr)type)->container));
		return;
	}

	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmSetValueType, CMSetValueTypeOpType, "CMSetValueType", CM_NOVALUE);	
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmSetValueType);
			AllowCMGetBaseValue(container);	
			CMDynSetValueType(value, type);
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(value, cmSetValueType);
			return;
		}
	}
	
	theValueHdr = (TOCValueHdrPtr)value;							/* ("this") value may have changed!	*/
	
	/* Make sure the type doesn't already exist for theobject's property...								*/
	
	typeID = ((TOCObjectPtr)type)->objectID;
	
	theValueHdr0 = cmGetPropertyType(theValueHdr->theProperty, typeID);
	if (theValueHdr0 != NULL && theValueHdr != theValueHdr0) {
		ERROR2(CM_err_DupType, cmGetGlobalTypeName(container, typeID), CONTAINERNAME);
		return;
	}

	/* If we're recording updates, then define touched list entry for the set-info.  This	*/
	/* touch MUST be done before any changes to the type because the touch list info is	  */
	/* in terms of the original type.																											*/

	cmTouchSetInfoedValue(theValueHdr);								/* touch the value if necessary			*/

	/* Change the type...																																	*/
	
	theValueHdr->typeID = typeID;											/* there goes the type							*/
}


/*-------------------------------------------------------------*
 | CMSetValueGeneration - set the generation number of a value |
 *-------------------------------------------------------------*
 
 The generation for the specified value is set.  The generation number must be greater than
 or equal to 1.
 
 Normally this routine doesn't need to be used since the generation is set when a value is
 created.  The default generation number is that of the container.
*/

void CM_FIXEDARGS CMSetValueGeneration(CMValue value, CMGeneration generation)
{
	TOCValueHdrPtr theValueHdr;
	ContainerPtr	 container;
	char					 genStr[15];
	
	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
	
	container = ((TOCValueHdrPtr)value)->container;

	if (IsDynamicValue(value)) {											/* process dynamic value...					*/
		GetDynHandlerAddress(value, cmSetValueGen, CMSetValueGenOpType, "CMSetValueGeneration", CM_NOVALUE);	
		if (IsDynamicValue(value)) {
			SignalDynHandlerInUse(value, cmSetValueGen);
			AllowCMGetBaseValue(container);	
			CMDynSetValueGen(value, generation);
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(value, cmSetValueGen);
			return;
		}
	}
	
	theValueHdr = (TOCValueHdrPtr)value;							/* ("this") value may have changed!	*/

	/* If we're recording updates, then define touched list entry for the set-info.  This	*/
	/* touch MUST be done before any changes to the type because the touch list info is	  */
	/* in terms of the original type.																											*/

	cmTouchSetInfoedValue(theValueHdr);								/* touch the value if necessary			*/

	/* Change generation...																																*/
	
	if (generation < 1) {															/* validate the generation nbr			*/
		ERROR3(CM_err_BadGenNbr, cmltostr(generation, 1, false, genStr), "CMSetValueGeneration", CONTAINERNAME);
		return;
	}
			
	theValueHdr->generation = generation;							/* finally, set the generation nbr	*/
}


/*----------------------------------------------------------------------*
 | CMGetValueContainer - get the container refNum for one of its values |
 *----------------------------------------------------------------------*
 
 The refNum for the container which contains the specified value is returned.
 
 Note that CMGetValueInfo() also returns the container refNum.  But this is generally
 useful enough on its own (especially in handlers) to warrent a separate API call. 
 Further, it provides a symmetry with CMGetObjectContainer().
*/

CMContainer CM_FIXEDARGS CMGetValueContainer(CMValue value)
{
	ExitIfBadValue((TOCValueHdrPtr)value, NULL);							/* validate value						*/
	
	return ((CMContainer)((TOCValueHdrPtr)value)->container); /* return container					*/
}


/*-------------------------------------------------*
 | CMGetValueRefCon - return user's value "refCon" |
 *-------------------------------------------------*

 This routine returns the user's "refCon" (reference constant) that s/he may associate
 with any value refNum (i.e., a CMValue).  The refCon is a CM_ULONG that the user may use
 in any way.  It is not touched by the API except to init it to 0 when the value is
 initially created.
 
 Note, the refCon is NOT perserved across closed containers, i.e., it is not saved in the
 TOC.
*/

CMRefCon CM_FIXEDARGS CMGetValueRefCon(CMValue value)
{
	ExitIfBadValue((TOCValueHdrPtr)value, (CMRefCon)0);				/* validate value						*/
	
	return (((TOCValueHdrPtr)value)->valueRefCon);						/* return user's refCon			*/
}


/*--------------------------------------------------*
 | CMSetValueRefCon - set the user's value "refCon" |
 *--------------------------------------------------*
 
 This routine is used to set the user's "refCon" (reference constant) to be assoicated with
 an value.  The refCon is a CM_ULONG that the user may use in any way. It is not touched by
 the API.
 
 Note, the refCon is NOT perserved across closed containers, i.e., it is not saved in the
 TOC.
*/

void CM_FIXEDARGS CMSetValueRefCon(CMValue value, CMRefCon refCon)
{
	ExitIfBadValue((TOCValueHdrPtr)value, CM_NOVALUE);				/* validate value						*/
	
	((TOCValueHdrPtr)value)->valueRefCon = refCon;						/* set the user's refCon		*/
}


/*--------------------------------*
 | CMDeleteValue - delete a value |
 *--------------------------------*

 The value is deleted from its object property.  If the value deleted is the only one for
 the property, the property itself is deleted as in CMDeleteObjectProperty().
 
 Calling CMDeleteValue() produces an implicit call on CMReleaseValue().  Hence the passed
 value refNum's association to the value is destroyed.  There should be no further
 operations on the refNum once CMDeleteValue() is called.
 
 Note, some values are protected from deletion.  This includes the predefined TOC object
 values (seed and offset) and the currently open embedded container values.  Also, a value
 acting as a base value for a subvalue cannot be deleted.
*/

void CM_FIXEDARGS CMDeleteValue(CMValue value)
{
	TOCValueHdrPtr theValueHdr, theRealValueHdr;
	ContainerPtr	 container;

	ExitIfBadValue(value, CM_NOVALUE);										/* validate value								*/
	
	theValueHdr = (TOCValueHdrPtr)value;
	container = theValueHdr->container->updatingContainer;
	
	if ((container->useFlags & kCMWriting) == 0) {
		ERROR2(CM_err_DeleteIllegal, "value", CONTAINERNAME);
		return;
	}
		
	if ((theValueHdr->valueFlags & (ValueProtected | ValueUndeletable)) != 0)	{ 
		ERROR1(CM_err_CantDelete3, CONTAINERNAME);
		return;
	}
	
	if (theValueHdr->useCount > 1) {											/* there are other users!				*/
		ERROR1(CM_err_CantDelete4, CONTAINERNAME);					/* nice try though!							*/
		return;
	}

	/* If the value header is a dynamic value then we must delete both the dynamic value	*/
	/* and its base ("real") value.  These are linked with the base value pointing to the	*/
	/* its dynamic value. For layered dynamic values, each dynamic value is back linked		*/
	/* to its base with the bottom layer having the "real" value as its base.  Only the 	*/
	/* bottom layer is actually on the special property chain for dynamic values 					*/
	/* associated with its "real value object.  Only the top layer should ever be passed. */
	/* We can check for this.  We also remember the "real" value header pointer. You will */
	/* see how we use this after we get it and do the validation.													*/

	if (IsDynamicValue(theValueHdr)) {										/* if we have dynamic value...	*/
		theRealValueHdr = theValueHdr;											/* start with current value			*/
		do 																									/* search for real value...			*/
			theRealValueHdr = DYNEXTENSIONS(theRealValueHdr)->baseValue;
		while (IsDynamicValue(theRealValueHdr));						/* loop till we find base value	*/
			
		if (theRealValueHdr->dynValueData.dynValue == NULL ||	/* do the validation...				*/
				theRealValueHdr->dynValueData.dynValue != theValueHdr) {
			ERROR2(CM_err_BadRealValue, "CMDeleteValue", CONTAINERNAME); /* ...oops!					*/
			return;
		}
		
		/* We now got our hands on the base ("real") value and we're happy with it. By doing*/
		/* a CMReleaseValue() on the dynamic value we will cause its deletion from there.		*/
		/* It is expected that if the dynamic value is layered, the handlers will 					*/
		/* recursively call CMReleaseValue() on each of their base values down to the bottom */
		/* layer.  So by the time we return to here all the dynamic values should be gone 	*/
		/* for its base value we just found above.  So then all we have to do is delete the	*/
		/* base ("real") value.																															*/
		
		CMReleaseValue(value);															/* this should kill dynamics		*/
		theValueHdr = theRealValueHdr;											/* now set to kill the base			*/
	}	
	
	/* If we're recording updates, then define touched list entry for the value delete...	*/

	cmTouchDeletedValue(theValueHdr, theValueHdr->theProperty->theObject);
	
	/* Deleted value headers are placed on a chain and marked deleted. The value segments */
	/* are freed.  Thus any attempted reuse of the refNum will be caught. Although we NULL*/
	/* out the caller's refNum s/he could have one squirreled away some where.						*/
	
	cmMarkValueDeleted(container, theValueHdr, false);
}


/*----------------------------------------------------------------------*
 | CMReleaseValue - destroy association between an value and its refNum |
 *----------------------------------------------------------------------*
 
 The association between the refNum and the value is destroyed. There should be no further
 operations on the refNum once this routine is called.
 
 If the value is a dynamic value, then if there are no other users of the value (i.e., 
 its use count is 1), the "release" dynamic value value handler is called for the value
 and ALL of its lower layer dynamic values.  It here are other uses of the dynamic value
 (i.e., its use count is >1), we do nothing other than decrement the use count by 1.
 
 See comments in code below (you can't miss 'em) on how and why we delete all the dynamic
 value layers.  The release handlers are not expected to release their base value like
 other value handler operations.
*/

void CM_FIXEDARGS CMReleaseValue(CMValue value)
{
	TOCValueHdrPtr 		theBaseValueHdr, dynamicBaseValueHdr, theValueHdr = (TOCValueHdrPtr)value;
	ContainerPtr	 	 	container;
	DynValueHdrExtPtr extensions;
	CMHandlerAddr			handler;
	
	ExitIfBadValue(value, CM_NOVALUE);								/* validate value										*/
	
	container = theValueHdr->container;

	/* As discussed later, dynamic value release handlers must not release their base 		*/
	/* value because we do it all here.  To protect agains such "mistakes" we check for		*/
	/* the attempt.  The dynamicBaseValueHdr in the container is normally NULL.  If it 		*/
	/* is not NULL it was set when we called a release handler (later) to that handler's	*/
	/* base value.  If it attempts a CMReleaseValue(CMGetBaseValue(value)), we will 			*/
	/* intercept it here and yell.  Note, we only test for the base value of the handler.	*/
	/* This lets other releases, which a release handler might want to do, get through.		*/
	/* Since one of those releases could be for yet another dynamic value, we have to			*/
	/* protect the container's version of dynamicBaseValueHdr.  We can do this as a local	*/
	/* variable since a call back to this routine from a release handler is recursive and	*/
	/* won't hurt the up-level copy.																											*/
	
	dynamicBaseValueHdr = container->dynamicBaseValueHdr;/* save current "switch" setting */
	if (theValueHdr == dynamicBaseValueHdr) { 				/* if attempted release on base...	*/
		ERROR1(CM_err_BaseRelAttempted, CONTAINERNAME);	/* ...yell													*/
		return;
	}

	/* Releasing a value that already is released is an error...													*/
	
	if (theValueHdr->useCount == 0) {
		ERROR1(CM_err_AlreadyReleased1, CONTAINERNAME);
		return;
	}
	
	/* Decrement the use count...																													*/
	
	--theValueHdr->useCount;													/* decrement use count							*/
	
	/* If this is not a dynamic value, just exit.  With dynamic values there's just a			*/
	/* "little" bit more to do!  																													*/
	
	if (!IsDynamicValue(value)) return;															
	
	/* We have a dynamic value at this point.  If its use count has gone to 0, then it 		*/
	/* is time to call the release handler for this value and ALL of its lower layers.  	*/
	/* When each handler returns we can delete the value.																	*/

	if (theValueHdr->useCount > 0) return;						/* dynamic value still in use				*/
	
	/* The following code calls the release handler for each dynamic value in a layered		*/
	/* set. Each layer is back-linked to its base with the bottom layer having the "real" */
	/* value as its base. Only the bottom layer is actually on the special property chain */
	/* for dynamic values associated with its "real" value object.  Only the top layer 		*/
	/* should ever be passed. 																														*/
	
	/* The calling of the dynamic value handler is different here from the way all other	*/
	/* dynamic handlers are called for value operations. Here WE CALL EACH DYNAMIC VALUE	*/
	/* HANDLER FOR EACH LAYER EXPLICITLY. A release dynamic handler does NOT do a release */
	/* on its base value. The reason is that a single "use" (or "new") was done to create */
	/* a dynamic value.  The CMReleaseValue() to get here is its counterpart.  The user 	*/
	/* has no knowledge (sort of) that the type may have had base types that spawned 			*/
	/* lower dynamic value layers.																												*/
	
	/* The other reason we do all the releases from here is for safety. We cannot "trust" */
	/* the handlers to do the release of their base value.  That would leave the data			*/
	/* structures in a weird state.  By processing all the layers ourselves we can be sure*/
	/* each release handler is called and each dynamic value is properly freed. 					*/
	
	/* Finally, the following loop does the releases following each dynamic value's back	*/
	/* line.  That means we're releasing in the reverse order of creation.  That seems the*/
	/* proper order to do it.  If the handlers released their base values, the effect 		*/
	/* would be the reverse.																															*/
	
	do {																							/* loop up through all the layers...*/
		extensions 			= DYNEXTENSIONS(theValueHdr);		/* save a little code generation		*/
		theBaseValueHdr = extensions->baseValue;				/* we will be deleting theValueHdr	*/
		
		/* We must now explicitly do the basic guts of a cmGetDynHandlerAddress().  That 		*/
		/* routine can't be used because it will go after an inherited release handler if		*/
		/* there is not handler for the value.  We don't want that because we want to call 	*/
		/* each release handler explicitly from here -- only if its got one of course. 			*/
		
		/* We know this is the first and only time a full CMReleaseValue() is being done for*/
		/* this value.  We don't get into here unless the use count goes to 0.  So all we 	*/
		/* have to do to see if there is a release handler is to call the value operations	*/
		/* metahandler for this value.	That will tell us if there is a release handler for	*/
		/* this value or it wants to inherit one. If there is a release handler, we call it	*/
		/* in the normal way so that this layer can do any cleanup (e.g., close files, free */
		/* refCon memory, etc.).  If there is none, everyone is happy (at this layer at 		*/
		/* least).  In either case we merge into the delete code that follows it.						*/
		
		handler = (CMHandlerAddr)(*extensions->metaHandler)(NULL, (CMGlobalName)CMReleaseValueOpType);
		
		/* If we got a non-inherited release handler for this value, call it.  By copying		*/
		/* this value's base value into the container value, dynamicBaseValueHdr, we can		*/
		/* test on entry to this routine for an attempted release on the base value.  We		*/
		/* said above that the rules are that the handler are NOT to do that.								*/
		
		if (handler) {																	/* call release handler if we got it*/
			SignalDynHandlerInUse(theValueHdr, cmReleaseValue);
			AllowCMGetBaseValue(container);	
			container->dynamicBaseValueHdr = theBaseValueHdr;
			extensions->dynValueVector.cmReleaseValue.handler = handler;/* set for the macro	*/
			CMDynReleaseValue(theValueHdr);
			container->dynamicBaseValueHdr = dynamicBaseValueHdr;
			DisAllowCMGetBaseValue(container);	
			SignalDynHandlerAvailable(theValueHdr, cmReleaseValue);
		}
		
		/* Now we must mark the value as deleted. This means moving it to the deletedValues	*/
		/* list.  For the bottom layer we can mark the value in the normal way and remove 	*/
		/* the pointer in the "real" value to it.  For the upper layers we must explicitly 	*/
		/* move the value to the deletedValues list and mark it. We can tell which is which */
		/* because cmNewDynamicValue() flagged the upper layers (that was nice of it -- 		*/
		/* well, this is why it did it, so it wasn't really being nice).										*/
		
		/* Note, when the bottom layer is marked as deleted, the entire "dynamic values"		*/
		/* property will be deleted if this is the last (or only) dynamic value for that		*/
		/* property.  This is a good thing, because we will report an error if we ever see	*/
		/* the "dynamic values" property at continer close time.														*/
		
		/* Aslo note that freeing the last dynamic value for an object clears the dynamic 	*/
		/* value object flag, DynamicValuesObject, which is used to protect against users		*/
		/* deleting the object prematurly.																									*/
	
		if ((theValueHdr->valueFlags & ValueOffPropChain) != 0) {		/* if upper layer...		*/
			if (cmKeepDeletedRefNums(container->toc)) { 							/* ...keep refNums ?		*/
				theValueHdr->valueFlags |= ValueDeleted;								/* ...mark value deleted*/
				cmInitList(&theValueHdr->valueList);										/* ...done for safety		*/
				cmAppendListCell(&container->deletedValues,theValueHdr);/* ...move to list			*/
			} else																										/* if don't keep refNums*/
				CMfree(theValueHdr);																		/* ...free it						*/
		} else {																										/* if bottom layer...		*/
			TOCPropertyPtr theProperty = theValueHdr->theProperty;		/* ...point to dyn prop.*/
			if (cmCountListCells(&theProperty->valueHdrList) == 1)		/* ...if last dyn value	*/
				theProperty->theObject->objectFlags &= ~DynamicValuesObject;/* ...clear obj flag*/
			extensions->baseValue->dynValueData.dynValue = NULL;			/* ...fix real value		*/
			cmMarkValueDeleted(container, theValueHdr, false);				/* ...mark value deleted*/
			if (container->targetContainer)														/* if target not freed	*/
				if (container->targetContainer->nbrOfDynValues > 0)			/* ...adjust total count*/
					--container->targetContainer->nbrOfDynValues;							
		}
		
		CMfree(extensions);															/* free the extensions							*/
		
		theValueHdr = theBaseValueHdr;									/* get next layer "down"						*/
	} while (IsDynamicValue(theValueHdr));						/* loop down to real value					*/
	
	/* We have now deleted all the dynamic value layers for the spawning "real" value. 		*/
	/* At this point theValueHdr is pointing at that real value.  When it spawned the			*/
	/* dynamic value(s) we set its use count to 1.  So we "owe" one decrement here.				*/
	
	if (theValueHdr->useCount > 0)										/* (I don't trust myself!)					*/
		--theValueHdr->useCount;												/* decrement real value's use count	*/
}
														  
															CM_END_CFUNCTIONS
