/* @(#)Z 1.4 com/src/cm/CMAPIIDs.h, odstorage, od96os2, odos29646d 96/11/15 15:26:42 (96/10/29 09:16:09) */
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
	File:		CMAPIIDs.h

	Contains:	Container Manager Standard Object ID Definitions, etc.

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.

	To Do:
*/

/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                         <<< CM_API_StdObjIDs.h >>>                        |
 |                                                                           |
 |          Container Manager Standard Object ID Definitions, etc.           |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/13/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 All standard object ID's and global names are defined in the file.  Also defined are the
 standard metahandler operation type strings.
*/


#ifndef __CM_API_STDOBJIDS__
#define __CM_API_STDOBJIDS__


/*-------------------------------------------*
 | Predefined Object, Property, and Type IDs |
 *-------------------------------------------*
 
 These ID's are used by the API to define standard objects in the container and its TOC.
 You can use them where applicable to look at these objects.
*/

#define CM_StdObjID_TOC							 1		/* TOC object	ID									   (object)	*/
#define CM_StdObjID_TOC_Seed		 		 2		/* TOC object starting seed 			 (property) */
#define CM_StdObjID_TOC_MinSeed		 	 3		/* TOC object minimum seed				 (property) */
#define CM_StdObjID_TOC_Object			 4		/* TOC object											 (property)	*/
#define CM_StdObjID_TOC_Container		 5		/* TOC object	entire container	 	 (property)	*/
#define CM_StdObjID_TOC_Deleted			 6		/* TOC object amount deleted 			 (property)	*/
#define CM_StdObjID_TOC_Free				 7		/* TOC object	free space					 (property)	*/
#define CM_StdObjID_TOC_NewValuesTOC 8		/* TOC object new values TOC			 (property)	*/
#define CM_StdObjID_TOC_Target			 9		/* TOC object	target container ref.(property)	*/
#define CM_StdObjID_TOC_DeleteList	10		/* TOC object delete update list	 (property)	*/

#define CM_StdObjID_TOC_Type				19		/* TOC object property value type			 (type) */

#define CM_StdObjID_Writable				20		/* Predefined ID's below this value will be		*/
																					/* written to container TOC.  Predefined ID's	*/
																					/* above this are NOT written.								*/

#define CM_StdObjID_7BitASCII				21		/* 7-bit ASCII												 (type) */
#define CM_StdObjID_8BitASCII				22		/* 8-bit ASCII												 (type) */
#define CM_StdObjID_GlobalTypeName	23		/* Global type name								 (property) */
#define CM_StdObjID_GlobalPropName	24		/* Global property name						 (property) */

#define CM_StdObjID_DynamicValues  	25		/* Dynamic values    							 (property) */
#define CM_StdObjID_BaseTypes  			26		/* Base types 		   							 (property) */
#define CM_StdObjID_BaseTypeList  	27		/* Base type list  		 			  		 		 (type) */

#define CM_StdObjID_TargetContainer	28		/* Target container									   (type) */
#define CM_StdObjID_ValueUpdates		29		/* Value updates for object		     (property) */
#define CM_StdObjID_UpdatesData			30		/* Internal updating instruction data  (type) */

#define CM_StdObjID_ObjReferences		31		/* Referenced objects from an object(property)*/
#define CM_StdObjID_ObjRefData		 	32		/* Reference data											 (type) */

#define CM_StdObjID_32BitImmValue		40		/* 32-bit immediate data value				 (type) */
#define CM_StdObjID_16BitImmValue		41		/* 16-bit immediate data value				 (type) */
#define CM_StdObjID_8BitImmValue		42		/*  8-bit immediate data value				 (type) */

#define CM_StdObjID_MinGeneralID	 100		/* min object ID for general standard objects */


/*------------------------------------*
 | Predefined Property and Type Names |
 *------------------------------------*
 
 These define global names which can be passed to CMRegisterType() or CMRegisterProperty()
 to get the associated type or property descriptor object.  Note, some of these are used
 internally by the Container Manager and are marked as "(CM)".
*/

#define CMTOCSeedGlobalName			 		"Apple:Global_Name_TOC_Seed"				/* property			*/
#define CMTOCMinSeedGlobalName	 		"Apple:Global_Name_TOC_MinSeed"			/* property			*/
#define CMTOCObjectGlobalName		 		"Apple:Global_Name_TOC_Object"			/* property			*/
#define CMTOCContainerGlobalName 		"Apple:Global_Name_TOC_Container"		/* property			*/
#define CMTOCDeletedGlobalName	 		"Apple:Global_Name_TOC_Deleted"			/* property			*/
#define CMTOCTotalFreeGlobalName 		"Apple:Global_Name_TOC_Free"				/* property			*/
#define CMTOCNewValuesTOCGlobalName	"Apple:Global_Name_TOC_NewValuesTOC"/* property			*/
#define CMTOCTargetGlobalName 	 		"Apple:Global_Name_TOC_Target"			/* property			*/
#define CMTOCDeleteListGlobalName 	"Apple:Global_Name_TOC_DeleteList"	/* property			*/
#define CMTOCValueTypeGlobalName		"Apple:Global_Name_TOC_Type"				/* type					*/

#define CMDynamicValueGlobalName 		"Apple:Dynamic_Value"								/* property (CM)*/
#define CMBaseTypesGlobalName		 		"Apple:Type_BaseType"								/* property (CM)*/
#define CMBaseTypeListGlobalName		"Apple:Type_BaseTypeList"						/* type 		(CM)*/
#define CMTargetContainerName	 			"Apple:Target_Container"						/* type			(CM)*/
#define CMValueUpdatesGlobalName		"Apple:Type_ValueUpdates"						/* property (CM)*/
#define CMUpdatesDataGlobalName			"Apple:Type_UpdatesData"						/* type		  (CM)*/


/*-------------------------------------*
 | Predfined Editing Metahandler Types |
 *-------------------------------------*
 
 These define a type names to be associated with special metahandler packages.  Currently 
 there is only one (!), which defines the type known to the Container Manager, that must
 be associated with target container handlers whenever a target container is opened by an
 updating container.  The target container handlers must be registered as the handler
 package assoicated with this type.
*/

#define CMTargetHandlersTypeName	"Apple:Target_Handlers_TypeName"


/*---------------------------------------------------------------------*
 | Metahandler Operation Types For Container Special Global Operations |
 *---------------------------------------------------------------------*

 These are the defined strings the API passes to a metahandler (passed to CMStartSession())
 to get the handler routine addresses for special global (to all containers) operations.
*/

#define CMErrorOpType						"Apple:ReportError"
#define CMAllocOpType						"Apple:AllocMemory"
#define CMFreeOpType						"Apple:FreeMemory"


/*----------------------------------------------------------*
 | Metahandler Operation Types For Container I/O Operations |
 *----------------------------------------------------------*

 These are the defined strings the API passes to a metahandler to get the handler routine
 addresses for the I/O operations the API needs to do on a container.
*/

#define CMOpenOpType						"Apple:OpenStream"
#define CMCloseOpType						"Apple:CloseStream"
#define CMFlushOpType						"Apple:FlushStream"
#define CMSeekOpType						"Apple:SetPosStream"
#define CMTellOpType						"Apple:GetPosStream"
#define CMReadOpType						"Apple:ReadStream"
#define CMWriteOpType						"Apple:WriteStream"
#define CMEofOpType							"Apple:EofStream"
#define CMTruncOpType						"Apple:TruncateStream"
#define CMSizeOpType						"Apple:SizeStream"
#define CMReadLblOpType					"Apple:ReadLabel"
#define CMWriteLblOpType				"Apple:WriteLabel"
#define CMParentOpType					"Apple:ReturnParentValue"
#define CMContainerOpName				"Apple:ReturnContainerName"
#define CMTargetTypeOpType			"Apple:TargetContainerDynamicValueType"
#define CMExtractDataOpType			"Apple:ExtractData"
#define CMFormatDataOpType			"Apple:FormatData"


/*------------------------------------------------*
 | Metahandler Operation Types For Dynamic Values |
 *------------------------------------------------*
 
 These are the defined strings that CMUseValue() and CMNewValue() pass to a metahandler if
 the type of the value is that of a globally unique name and that name has a "use value"
 matahandler defined for it.  These are used to create dynamic values.
*/

#define CMDefineMetaDataOpType	"Apple:DefineMetaData"
#define CMNewValueOpType				"Apple:NewValue"
#define CMUseValueOpType				"Apple:UseValue"


/*----------------------------------------------------------*
 | Metahandler Operation Types For Dynamic Value Operations |
 *----------------------------------------------------------*

 These are the defined strings the API passes to a dynamic value metahandler (returned
 from a "use value" handler) to get the dynamic value handler addresses for value
 operations.
*/

#define CMGetValueSizeOpType		"Apple:CMGetValueSize"
#define CMReadValueDataOpType		"Apple:CMReadValueData"
#define CMWriteValueDataOpType	"Apple:CMWriteValueData"
#define CMInsertValueDataOpType	"Apple:CMInsertValueData"
#define CMDeleteValueDataOpType	"Apple:CMDeleteValueData"
#define CMGetValueInfoOpType		"Apple:CMGetValueInfo"
#define CMSetValueTypeOpType		"Apple:CMSetValueType"
#define CMSetValueGenOpType			"Apple:CMSetValueGeneration"
#define CMReleaseValueOpType		"Apple:CMReleaseValue"


#endif
