/* @(#)Z 1.4 com/src/cm/CMAPI.h, odstorage, od96os2, odos29646d 96/11/15 15:26:40 (96/10/29 09:15:50) */
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
	File:		CMAPI.h

	Contains:	Container Manager API

	Owned by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1994-95 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <5>	 5/25/95	jpa		Changed last param of CMOpenNewContainer
									for ANSI/CFM68K compatibility. [1241078]
		 <4>	 3/31/95	EL		1234685: Add CMContainsValidLabel.
		 <3>	 3/24/95	EL		1209355: add CMTakeSnapShot.
		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <2>	 3/31/94	EL		Add CMMergeContainer call. #1150214
		 <8>	 3/17/94	EL		Add CMKeepObject call.
		 <7>	 12/7/93	EL		Add CMGetReferenceForObject call.
		 <6>	11/10/93	EL		Add CMCountProperties call.
		 <5>	 10/6/93	EL		Add CMGetPrevXXXX.
		 <4>	 9/27/93	VL		Added CMGetPrevObjectProperty and
													CMGetPrevValue.
		 <3>	  9/8/93	VL		Added GetObject and GetObjectID.

	To Do:
	In Progress:
		
*/

/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                             <<< CM_API.h >>>                              |
 |                                                                           |
 |                           Container Manager API                           |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/18/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1995                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This is the Container Manager API definitions.  All routines needed to communicate with
 the Container Manager are defined here.  This is the only explicit #include needed. All
 other required headers are included from here.
 
 Refer to Container Manager API documentation for details on these routines.


											  *------------------------------*
												| NOTE TO DOS (80x86) USERS... |
												*------------------------------*

 The Container Manager should be compiled with the "large" memory model so that all
 routines, pointers, etc. are "far".  All external API interfaces are appropriately
 qualified this way.  Since handlers are implementation or user dependent, these files
 must be compiled with the large memory model.  The only exception is local static
 routines which usually can be qualified with "near" (except, of course, for handlers).
 
 This file uses the function and pointer memory model attribute macros defined
 CM_API_Environment.h.  See that file for further details.
*/

#ifndef __CM_API__
#define __CM_API__

#include <stdarg.h>
#include <stdio.h>

#ifndef __CM_API_ENV__
#include "CMAPIEnv.h"													/* Environment-specific definitions				*/
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"													/* API type definitions										*/
#endif
#ifndef __CM_API_STDOBJIDS__
#include "CMAPIIDs.h"													/* Standard object ID definitions, etc.		*/
#endif
#ifndef __CM_API_ERRNO__
#include "CMAPIErr.h"												/* Error code numbers and their meaning		*/
#endif


																CM_CFUNCTIONS
																
/*---------------------------*
 | Session (task) operations |
 *---------------------------*/

CMSession CM_FIXEDARGS CMStartSession(CMMetaHandler metaHandler, CMRefCon sessionRefCon);
void CM_FIXEDARGS CMEndSession(CMSession sessionData, CMBoolean closeOpenContainers);
void CM_FIXEDARGS CMAbortSession(CMSession sessionData);
CMRefCon CM_FIXEDARGS CMGetSessionRefCon(CMContainer container);
void CM_FIXEDARGS CMSetSessionRefCon(CMContainer container, CMRefCon refCon);


/*--------------------*
 | Handler Operations |
 *--------------------*/

CMHandlerAddr CM_FIXEDARGS CMSetMetaHandler(CMconst_CMSession sessionData,
																						CMconst_CMGlobalName typeName,
													 		 							CMMetaHandler metaHandler);
CMHandlerAddr CM_FIXEDARGS CMGetMetaHandler(CMconst_CMSession sessionData,
																						CMconst_CMGlobalName typeName);
CMHandlerAddr CM_FIXEDARGS CMGetOperation(CMType targetType,
																					CMconst_CMGlobalName operationType);


/*----------------------*
 | Container Operations |
 *----------------------*/

CMContainer CM_FIXEDARGS CMOpenContainer(CMSession sessionData,
																				 CMRefCon attributes,
																				 CMconst_CMGlobalName typeName, 
																				 CMContainerUseMode useFlags);
CMContainer CM_VARARGS CMOpenNewContainer(CMSession sessionData,
																					CMRefCon attributes,
																					CMconst_CMGlobalName typeName, 
																					CMContainerUseMode useFlags,
																					CMGeneration generation,
																				/*CMContainerFlags*/ CM_ULONG containerFlags, ...);
CMContainer CM_FIXEDARGS CMVOpenNewContainer(CMSession sessionData,
																						 CMRefCon attributes,
																						 CMconst_CMGlobalName typeName, 
																						 CMContainerUseMode useFlags,
																						 CMGeneration generation,
																						 CMContainerFlags containerFlags,
																						 va_list targetTypeInitParams);
void CM_FIXEDARGS CMCloseContainer(CMconst_CMContainer container);
void CM_FIXEDARGS CMAbortContainer(CMconst_CMContainer container);
void CM_FIXEDARGS CMMergeContainer(CMconst_CMContainer container, CMValue targetValue);
void CM_FIXEDARGS CMGetContainerInfo(CMconst_CMContainer container,
																		 CMGeneration CM_PTR *generation, 
																		 CM_USHORT CM_PTR *bufSize,
																		 CMContainerFlags CM_PTR *containerFlags,
																		 CMGlobalName typeName,
																		 CMContainerModeFlags CM_PTR *openMode);
CMSession CM_FIXEDARGS CMGetSession(CMContainer container);
void CM_FIXEDARGS CMTakeSnapShot(CMconst_CMContainer container, CMBoolean includeEmbedded);


/*------------------------------*
 | Type and Property Operations |
 *------------------------------*/

CMType CM_FIXEDARGS CMRegisterType(CMContainer targetContainer, CMconst_CMGlobalName name);
CMProperty CM_FIXEDARGS CMRegisterProperty(CMContainer targetContainer,
																					 CMconst_CMGlobalName name);
CMCount CM_FIXEDARGS CMAddBaseType(CMType type, CMType baseType);
CMCount CM_FIXEDARGS CMRemoveBaseType(CMType type, CMType baseType);
CMBoolean CM_FIXEDARGS CMIsType(CMObject theObject);
CMBoolean CM_FIXEDARGS CMIsProperty(CMObject theObject);
CMType CM_FIXEDARGS CMGetNextType(CMContainer targetContainer, CMType currType);
CMType CM_FIXEDARGS CMGetPrevType(CMContainer targetContainer, CMType currType);
CMProperty CM_FIXEDARGS CMGetNextProperty(CMContainer targetContainer,
																					CMProperty currProperty);
CMProperty CM_FIXEDARGS CMGetPrevProperty(CMContainer targetContainer,
																					CMProperty currProperty);


/*-------------------*
 | Object Operations |
 *-------------------*/

CMObject CM_FIXEDARGS CMNewObject(CMContainer targetContainer);
CMObject CM_FIXEDARGS CMGetNextObject(CMContainer targetContainer, CMObject currObject);
CMObject CM_FIXEDARGS CMGetPrevObject(CMContainer targetContainer, CMObject currObject);
CMProperty CM_FIXEDARGS CMGetNextObjectProperty(CMObject theObject, CMProperty currProperty);
CMProperty CM_FIXEDARGS CMGetPrevObjectProperty(CMObject theObject, CMProperty currProperty);
CMObject CM_FIXEDARGS CMGetNextObjectWithProperty(CMContainer targetContainer,
																									CMObject currObject, CMProperty property);
CMObject CM_FIXEDARGS CMGetPrevObjectWithProperty(CMContainer targetContainer,
																									CMObject currObject, CMProperty property);
CMContainer CM_FIXEDARGS CMGetObjectContainer(CMObject theObject);
CMGlobalName CM_FIXEDARGS CMGetGlobalName(CMObject theObject);
CMCount CM_FIXEDARGS CMCountProperties(CMObject object, CMProperty property);
CMRefCon CM_FIXEDARGS CMGetObjectRefCon(CMObject theObject);
void CM_FIXEDARGS CMSetObjectRefCon(CMObject theObject, CMRefCon refCon);
void CM_FIXEDARGS CMDeleteObject(CMObject theObject);
void CM_FIXEDARGS CMDeleteObjectProperty(CMObject theObject, CMProperty theProperty);
void CM_FIXEDARGS CMReleaseObject(CMObject theObject);
void CM_FIXEDARGS CMKeepObject(CMObject theObject);
CMObject CM_FIXEDARGS CMGetObject(CMContainer targetContainer, CMObjectID objectID);
CMObjectID CM_FIXEDARGS CMGetObjectID(CMObject object);


/*------------------*
 | Value Operations |
 *------------------*/

CMCount CM_FIXEDARGS CMCountValues(CMObject object, CMProperty property, CMType type);
CMValue CM_FIXEDARGS CMUseValue(CMObject object, CMProperty property, CMType type);
CMValue CM_FIXEDARGS CMGetNextValue(CMObject object, CMProperty property, CMValue currValue);
CMValue CM_FIXEDARGS CMGetPrevValue(CMObject object, CMProperty property, CMValue currValue);
CMValue CM_VARARGS CMNewValue(CMObject object, CMProperty property, CMType type, ...);
CMValue CM_FIXEDARGS CMVNewValue(CMObject object, CMProperty property, CMType type,
													 			 va_list dataInitParams);
CMValue CM_FIXEDARGS CMGetBaseValue(CMValue value);
CMSize CM_FIXEDARGS CMGetValueSize(CMValue value);
CMSize CM_FIXEDARGS CMReadValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize);
void CM_FIXEDARGS CMWriteValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
void CM_FIXEDARGS CMDefineValueData(CMValue value, CMCount offset, CMSize size);
void CM_FIXEDARGS CMInsertValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
void CM_FIXEDARGS CMDeleteValueData(CMValue value, CMCount offset, CMSize size);
void CM_FIXEDARGS CMMoveValue(CMValue value, CMObject object, CMProperty property);
void CM_FIXEDARGS CMGetValueInfo(CMValue value, CMContainer CM_PTR *container,
																 CMObject CM_PTR *object, CMProperty CM_PTR *property,
																 CMType CM_PTR *type, CMGeneration CM_PTR *generation);
void CM_FIXEDARGS CMSetValueType(CMValue value, CMType type);
void CM_FIXEDARGS CMSetValueGeneration(CMValue value, CMGeneration generation);
CMContainer CM_FIXEDARGS CMGetValueContainer(CMValue value);
CMRefCon CM_FIXEDARGS CMGetValueRefCon(CMValue value);
void CM_FIXEDARGS CMSetValueRefCon(CMValue value, CMRefCon refCon);
void CM_FIXEDARGS CMDeleteValue(CMValue value);
void CM_FIXEDARGS CMReleaseValue(CMValue value);
CMBoolean CM_FIXEDARGS CMContainsValidLabel(CMValue value);


/*----------------------*
 | Reference Operations |
 *----------------------*/

CMReference CM_PTR * CM_FIXEDARGS CMNewReference(CMValue value,
																								 CMObject referencedObject,
																 			 			   	 CMReference CM_PTR theReferenceData);
CMReference CM_PTR * CM_FIXEDARGS CMSetReference(CMValue value,
																								 CMObject referencedObject,
																 			 			   	 CMReference CM_PTR theReferenceData);
CMObject CM_FIXEDARGS CMGetReferencedObject(CMValue value, CMReference CM_PTR theReferenceData);
CMReference CM_PTR * CM_FIXEDARGS CMGetReferenceForObject(CMValue value, 
																								 					CMObject referencedObject,
																													CMReference CM_PTR theReferenceData);
void CM_FIXEDARGS CMDeleteReference(CMValue value, CMReference CM_PTR theReferenceData);
CMCount CM_FIXEDARGS CMCountReferences(CMValue value);
CMReference CM_PTR * CM_FIXEDARGS CMGetNextReference(CMValue value,
																										 CMReference CM_PTR currReferenceData);
CMReference CM_PTR * CM_FIXEDARGS CMGetPrevReference(CMValue value,
																										 CMReference CM_PTR currReferenceData);


/*--------------------------*
 | Value Handler Operations |
 *--------------------------*/

CMCount CM_VARARGS CMScanDataPacket(CMType type, CMMetaData metaData,
																		CMDataPacket dataPacket, ...);
CMCount CM_FIXEDARGS CMVScanDataPacket(CMType type, CMMetaData metaData,
																			 CMDataPacket dataPacket,
																 			 va_list dataInitParams);


/*--------------------------*
 | Error Handler Operations |
 *--------------------------*/

char CM_PTR * CM_VARARGS CMAddMsgInserts(char CM_PTR *msgString, CMSize maxLength, ...);
char CM_PTR * CM_FIXEDARGS CMVAddMsgInserts(char CM_PTR *msgString, CMSize maxLength,
																					  va_list inserts);
CMErrorString CM_VARARGS CMGetErrorString(CMErrorString errorString, CMSize maxLength, 
															 		 				CMErrorNbr errorNumber, ...);
CMErrorString CM_FIXEDARGS CMVGetErrorString(CMErrorString errorString, CMSize maxLength, 
															  			 			 CMErrorNbr errorNumber, va_list inserts);
char CM_PTR * CM_FIXEDARGS CMReturnContainerName(CMContainer container);


/*--------------------------------*
 | Special Environment Operations |
 *--------------------------------*/

void CM_PTR * CM_FIXEDARGS CMMalloc(CMSize size, CMSession sessionData);
void CM_FIXEDARGS CMFree(CMPtr ptr, CMSession sessionData);
void CM_VARARGS CMError(CMSession sessionData, CMErrorString message, ...);
void CM_FIXEDARGS CMVError(CMSession sessionData, CMErrorString message, va_list inserts);

														CM_END_CFUNCTIONS

#endif
