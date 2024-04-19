/* @(#)Z 1.7 com/src/bento/IndHdr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:33 (96/10/29 09:12:47) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
//
//   CLASSES: none
//
//   ORIGINS: 82,27
//
//
//   (C) COPYRIGHT International Business Machines Corp. 1995,1996
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//   	
//   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
//   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
//   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
//   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
//   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
//   OR PERFORMANCE OF THIS SOFTWARE.
//
//====END_GENERATED_PROLOG========================================
//

/*
	File:		IndHdr.cpp

	Contains:	Indirect Handler for Bento Container Suite

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>	10/22/95	EL		1294623: Indirect value contains endian-
									specific name length.
		 <6>	10/13/95	EL		1287340: Use standard ISO prefix
		 <5>	  8/8/95	jpa		Fixed DebugStrs [1265584]
		 <4>	 5/26/95	VL		1251403: Multithreading naming support.
		 <3>	  7/5/94	CC		Type cast pascal strings.
		 <2>	 6/18/94	MB		Correct memory includes, still problems
		 <1>	 5/27/94	VL		first checked in

	To Do:
	In Progress:
		
*/

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#ifndef _INDHDR_
#include "IndHdr.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _FLIPEND_
#include "FlipEnd.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef som_xh
#include "som.xh"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#ifndef _FILECTR_
#include "FileCtr.xh"
#endif

//==============================================================================
// Constants
//==============================================================================

const CMGlobalName kODIndirectValueGlobalName = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:IndirectValue";
const ODType	kODEmbeddedContainerProperty = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:EmbeddedContainer";
const ODType	kODEmbeddedContainerType = "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:EmbeddedContainer";

//==============================================================================
// Scalar Types
//==============================================================================
typedef struct {
	CMValue		targetValue;
	CMSession	sessionData;
	ODType		objectName;
} IndirectValueRefCon;
typedef IndirectValueRefCon *IndirectValueRefConPtr;

//------------------------------------------------------------------------------
// For IndirectValueMetahandler
//------------------------------------------------------------------------------
	CM_CFUNCTIONS

 static CMHandlerAddr IndirectValueMetahandler(CMType targetType, const CMGlobalName operationType);
													
 static CMSize getValueSize_Handler(CMValue value);
 static CMSize readValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize);
 static void writeValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
 static void insertValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
 static void deleteValueData_Handler(CMValue value, CMCount offset, CMSize size);
 static void getValueInfo_Handler(CMValue value, CMContainer *container, CMObject *object,
																	CMProperty *property, CMType *type, 
																	CMGeneration *generation);
 static void setValueType_Handler(CMValue value, CMType type);
 static void setValueGeneration_Handler(CMValue value, CMGeneration generation);
 static void releaseValue_Handler(CMValue value);
 																 
//------------------------------------------------------------------------------
// For IndirectDynamicValueMetahandler
//------------------------------------------------------------------------------

 static CMMetaData metaData_Handler(CMType type);
 static CMBoolean newValue_Handler(CMValue dynamicBaseValue, 
 									CMType type,
 									CMDataPacket dataPacket);
 static CMBoolean useValue_Handler(CMValue dynamicBaseValue,
 									CMType type,
 									CMMetaHandler *metahandler,
									CMRefCon *refCon);
	CM_END_CFUNCTIONS


//==============================================================================
// IndirectDynamicValueMetahandler
//==============================================================================
 																																												
CMHandlerAddr CM_FIXEDARGS IndirectDynamicValueMetahandler(CMType targetType, CMconst_CMGlobalName operationType)
{
	CMType ignored = targetType;
	
	if (strcmp((ODSByte*) operationType, CMDefineMetaDataOpType)==0)
		return ((CMHandlerAddr) metaData_Handler);
	else if (strcmp((ODSByte*) operationType, CMNewValueOpType) == 0)
		return ((CMHandlerAddr) newValue_Handler);
	else if (strcmp((ODSByte*) operationType, CMUseValueOpType) == 0)
		return ((CMHandlerAddr) useValue_Handler);
	else																											
		return (kODNULL);
}

//------------------------------------------------------------------------------
// metaData_Handler
//------------------------------------------------------------------------------

static CMMetaData metaData_Handler(CMType type)
{
	CMType unused = type;
	
	return ((CMMetaData)"%p(=remoteObject)");	
}

//------------------------------------------------------------------------------
// newValue_Handler
//------------------------------------------------------------------------------

static CMBoolean newValue_Handler(CMValue dynamicBaseValue, CMType type, CMDataPacket dataPacket)
{
	CMContainer	container   = CMGetObjectContainer(type);
	CMSession	sessionData = CMGetSession(container);
	CMCount 	i;	
	ODType		objectName;
	ODULong		objectNameSize, stdNameSize;
		
	i = CMScanDataPacket(type, metaData_Handler(type), dataPacket, &objectName);
	
	if (i != 1) {
		CMError(sessionData,
				"Unable to get all of CMNewValue()'s parameter in container \"^0\"",
				CMReturnContainerName(container));
		return (kODFalse);
	}
	
	if (objectName == kODNULL) {
		objectNameSize = 0;
	}
	else {
		objectNameSize = strlen(objectName) + 1;
	}
	stdNameSize = ConvertODULongToStd(objectNameSize);
	CMWriteValueData(dynamicBaseValue, &stdNameSize, 0, sizeof(ODULong));
	if (objectName == kODNULL)
		CMWriteValueData(dynamicBaseValue, "", sizeof(ODULong), 1);
	else
		CMWriteValueData(dynamicBaseValue, objectName, sizeof(ODULong), objectNameSize);
	
	return (kODTrue);
}


//------------------------------------------------------------------------------
// useValue_Handler
//------------------------------------------------------------------------------

static CMBoolean useValue_Handler(CMValue dynamicBaseValue,
								CMType type,
								CMMetaHandler *metahandler,
								CMRefCon *refCon)
{
	CMContainer  			container	  = CMGetObjectContainer(type);
	CMSession				sessionData = CMGetSession(container);
	IndirectValueRefConPtr	myRefCon;
	CMProperty				targetProperty,
							embeddedContainerProperty;
	CMType					embeddedContainerType;
	CMObject				targetObject;
	CMValue					targetValue;
	ODULong				objectNameSize;
	ODFileContainer*		parentContainer;
	ODSessionRefCon*		sessionRefCon;

	sessionRefCon = (ODSessionRefCon*) CMGetSessionRefCon(container);
	if (sessionRefCon == kODNULL)
		WARNMSG(WARN_INDEX(AMSG_360),"No Session.");

	objectNameSize = CMGetValueSize(dynamicBaseValue);
	if (objectNameSize < sizeof(ODULong)) {
		CMError(sessionData, 
			"Incorrect byte length read while reading indirect value data in container \"^0\"", 
			CMReturnContainerName(container));
		return (kODFalse);
	}
	else
		objectNameSize -= sizeof(ODULong);
	
	/* Allocate the refCon that we will pass among the handlers.  Since we are doing a		*/
	/* dynamic allocation here we will use the "malloc" handler defined for the container.*/
	
	myRefCon = (IndirectValueRefConPtr) ODNewPtr(sizeof(IndirectValueRefCon), sessionRefCon->heap);
	
	myRefCon->objectName = (ODType) ODNewPtr(objectNameSize, sessionRefCon->heap);
	if (CMReadValueData(dynamicBaseValue, myRefCon->objectName, sizeof(ODULong), objectNameSize) != objectNameSize) {
		CMError(sessionData, 
			"Incorrect byte length read while reading indirect value data in container \"^0\"", 
			CMReturnContainerName(container));
		return (kODFalse);
	}
	
	myRefCon->sessionData = sessionData;								/* save the current session ptr		*/
		
	parentContainer = (ODFileContainer*) sessionRefCon->container;
	if (parentContainer == kODNULL)
		WARNMSG(WARN_INDEX(AMSG_370),"No File Container.");
		
	Environment* ev = somGetGlobalEnvironment();
	targetProperty = CMRegisterProperty(parentContainer->GetCMContainer(ev), myRefCon->objectName);
	embeddedContainerProperty = CMRegisterProperty(parentContainer->GetCMContainer(ev), kODEmbeddedContainerProperty);
	embeddedContainerType = CMRegisterType(parentContainer->GetCMContainer(ev), kODEmbeddedContainerType);
	targetObject = CMGetNextObjectWithProperty(parentContainer->GetCMContainer(ev), kODNULL, targetProperty);
	targetValue = CMUseValue(targetObject, embeddedContainerProperty, embeddedContainerType);
	
	if (targetValue != kODNULL)
		myRefCon->targetValue = targetValue;
	else
		WARNMSG(WARN_INDEX(AMSG_380),"No target value.");
		
	*metahandler = (CMMetaHandler) IndirectValueMetahandler;			/* return metahandler			*/
	*refCon	= (CMRefCon) myRefCon;										/* ...and refCon				*/
		
	return (kODTrue);
}

//==============================================================================
// IndirectValueMetahandler
//==============================================================================

static CMHandlerAddr IndirectValueMetahandler(CMType targetType, const CMGlobalName operationType)
{
	static char *operationTypes[] = {CMGetValueSizeOpType,			/* 0 */ /* Operation Types	*/
										CMReadValueDataOpType,		/* 1 */
									 	CMWriteValueDataOpType,		/* 2 */
										CMInsertValueDataOpType,	/* 3 */
										CMDeleteValueDataOpType,	/* 4 */
										CMGetValueInfoOpType,		/* 5 */
										CMSetValueTypeOpType,		/* 6 */
										CMSetValueGenOpType,		/* 7 */
										CMReleaseValueOpType,		/* 8 */
										NULL};
	char 	 **t;
	CMType ignored = targetType;
	
	/* Look up the operation type in the operationTypes table above...	*/
	
	t = operationTypes - 1;
	while (*++t) if (strcmp((char *)operationType, *t) == 0) break;

	/* Now that we got it (hopefully), return the appropriate routine address...*/
	
	switch (t - operationTypes) {
		case  0:	return ((CMHandlerAddr)getValueSize_Handler);		/* CMGetValueSizeOpType 	*/
		case  1:	return ((CMHandlerAddr)readValueData_Handler);		/* CMReadValueDataOpType 	*/
		case  2:	return ((CMHandlerAddr)writeValueData_Handler);		/* CMWriteValueDataOpType */
		case  3:	return ((CMHandlerAddr)insertValueData_Handler);	/* CMInsertValueDataOpType*/
		case  4:	return ((CMHandlerAddr)deleteValueData_Handler);	/* CMDeleteValueDataOpType*/
		case  5:	return (NULL);/* use inherited handler or API */	/* CMGetValueInfoOpType 	*/
		case  6:	return (NULL);/* use inherited handler or API */	/* CMSetValueTypeOpType 	*/
		case  7:	return (NULL);/* use inherited handler or API */	/* CMSetValueGenOpType 		*/
		case  8:	return ((CMHandlerAddr)releaseValue_Handler);		/* CMReleaseValueOpType 	*/
		
		default:	return (NULL);
	}
}

//------------------------------------------------------------------------------
// getValueSize_Handler
//------------------------------------------------------------------------------
static CMSize getValueSize_Handler(CMValue value)
{
	IndirectValueRefConPtr	myRefCon = (IndirectValueRefConPtr) CMGetValueRefCon(value);

	return (CMGetValueSize(myRefCon->targetValue));
}


//------------------------------------------------------------------------------
// readValueData_Handler
//------------------------------------------------------------------------------
static CMSize readValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize)
{
	IndirectValueRefConPtr	myRefCon = (IndirectValueRefConPtr) CMGetValueRefCon(value);
	
	return CMReadValueData(myRefCon->targetValue, buffer, offset, maxSize);
}


//------------------------------------------------------------------------------
// writeValueData_Handler
//------------------------------------------------------------------------------
static void writeValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize size)
{
	IndirectValueRefConPtr	myRefCon = (IndirectValueRefConPtr) CMGetValueRefCon(value);
	
	CMWriteValueData(myRefCon->targetValue, buffer, offset, size);
}


//------------------------------------------------------------------------------
// insertValueData_Handler
//------------------------------------------------------------------------------
static void insertValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize size)
{
	IndirectValueRefConPtr refCon = (IndirectValueRefConPtr)CMGetValueRefCon(value);
	CMCount				unused1 = offset;
	CMSize				unused2 = size;
	CMPtr				unused3 = buffer;

	CMError(refCon->sessionData,
				"Insertions into an embedded container \"^0\" are not supported",
				CMReturnContainerName(CMGetValueContainer(value)));
}


//------------------------------------------------------------------------------
// deleteValueData_Handler
//------------------------------------------------------------------------------
static void deleteValueData_Handler(CMValue value, CMCount offset, CMSize size)
{
	IndirectValueRefConPtr refCon = (IndirectValueRefConPtr)CMGetValueRefCon(value);
	CMCount				unused1 = offset;
	CMSize				unused2 = size;

	CMError(refCon->sessionData,
			"Deletions of data in an embedded container \"^0\" are not supported",
			CMReturnContainerName(CMGetValueContainer(value)));
}


//------------------------------------------------------------------------------
// getValueInfo_Handler
//------------------------------------------------------------------------------
static void getValueInfo_Handler(CMValue value, CMContainer *container, CMObject *object,
									CMProperty *property, CMType *type,
									CMGeneration *generation)
{	
	CMGetValueInfo(CMGetBaseValue(value), container, object, property, type, generation);
}


//------------------------------------------------------------------------------
// setValueType_Handler
//------------------------------------------------------------------------------
static void setValueType_Handler(CMValue value, CMType type)
{	
	CMSetValueType(CMGetBaseValue(value), type);
}


//------------------------------------------------------------------------------
// setValueGeneration_Handler
//------------------------------------------------------------------------------
static void setValueGeneration_Handler(CMValue value, CMGeneration generation)
{
	CMSetValueGeneration(CMGetBaseValue(value), generation);
}


//------------------------------------------------------------------------------
// releaseValue_Handler
//------------------------------------------------------------------------------
static void releaseValue_Handler(CMValue value)
{
	IndirectValueRefConPtr myRefCon = (IndirectValueRefConPtr)CMGetValueRefCon(value);
	CMSession			sessionData = myRefCon->sessionData;
	
	CMReleaseValue(myRefCon->targetValue);
	if (myRefCon != kODNULL) {
		if (myRefCon->objectName != kODNULL)
			ODDisposePtr(myRefCon->objectName);
		ODDisposePtr(myRefCon);
	}
}
