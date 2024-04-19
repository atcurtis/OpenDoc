/* @(#)Z 1.4 com/src/cm/CMAPIErr.h, odstorage, od96os2, odos29646d 96/11/15 15:26:42 (96/10/29 09:16:04) */
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
	File:		CMAPIErr.h

	Contains:	Container Manager Error Code Numbers and Their Meaning

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
 |                           <<< CM_API_Errno.h >>>                          |
 |                                                                           |
 |           Container Manager Error Code Numbers and Their Meaning          |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/06/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1992                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This header contains all the error number codes passed to the error handler.  The error
 handler function address MUST be passed to CMStartSession() which MUST be called before
 any other Container Manager routine.  It should have the following prototype:
 
 					void anErrorHandler(CMErrorNbr errorNumber, ...)

 The "..." is for possible error message string inserts which are a function of the error
 number.
 
 In the list of error code definitions below, the comment shows the meaning of the error
 code.  The "^n"s (^0, ^1, etc.) in the comments show where the inserts would go.  ^0 is
 the first insert, ^1 the second, and so on.
 
 The Container Manager API provides routines to allow an error handler to convert the
 error number and its associated insert to the messages shown in the comments.  The API
 routine CMVGetErrorString() is the primary routine which a error handler would probably
 use to convert the error code and inserts to a message string. Other routines are
 provided.  See API.h and Container Manager API documentation for further details.
*/


#ifndef __CM_API_ERRNO__
#define __CM_API_ERRNO__


												 /*-----------------------------------*
													| The Container Manager Error Codes |
													*-----------------------------------*/
													 
/* The comments for each of these error codes correspond to the intended meaning.  A ^n	*/
/* in the comment indicates a insert.  ^0 is the first insert, ^1 the second, and so on.*/

#define CM_err_GenericMessage			0	/* ^0																																									*/
#define CM_err_BadTocSize				  1	/* TOC index table size out of range (^0 <= range <= ^1)															*/
#define CM_err_NoSession					2	/* Unable to allocate session (task) global data																			*/
#define CM_err_NoHandler				  3	/* Cannot allocate space for handler for type "^0"																		*/
#define CM_err_BadWriteUse				4	/* Cannot do a CMOpenNewContainer(...^0...) with useFlags set for update-in-place			*/
#define CM_err_NoContainer			  5	/* Cannot allocate CCB space for type "^0"																						*/
#define CM_err_UndefMetaHandler	  6	/* Metahandler was not defined for type "^0"																					*/
#define CM_err_HandlerError			  7	/* Allocation problem while looking up handler for type "^0"													*/
#define CM_err_NullMetaPtr			  8	/* Null metahandler pointer for type "^0"																							*/
#define CM_err_UndefRoutine			  9	/* Handler routine(s) missing for operation type(s): ^0																*/
#define CM_err_NoTOC						 10	/* Cannot allocate TOC for container "^0"																							*/
#define CM_err_BadReadUse 			 11	/* Cannot do a CMOpenContainer(..."^0"...) with useFlags set for ^1										*/
#define CM_err_BadMagicBytes		 12	/* Invalid magic byte sequence in label for container "^0" - expected "^1", got "^2""	*/
#define CM_err_BadVersion				 13	/* Invalid version fields in label for container "^0" - expected ^1.^2, got ^3.^4"		*/
#define CM_err_BadTOCRead				 14	/* Incorrect byte length read while reading TOC for container "^0"										*/
#define CM_err_NoObjEntry				 15	/* Cannot allocate space for TOC object entry for container "^0"											*/
#define CM_err_MultDef				 	 16	/* Multiple definition for TOC object ^0 for container "^1"														*/
#define CM_err_NoPropEntry		 	 17	/* Cannot allocate space for TOC property entry for container "^0"										*/
#define CM_err_BadContinue		 	 18	/* Invalid continued value (inconsistent types) in container "^0"											*/
#define CM_err_NoValueEntry		 	 19 /* Cannot allocate space for TOC value entry for container "^0"												*/
#define CM_err_BadOffset			 	 20	/* Invalid offset or value (^0, ^1) for TOC value entry for container "^2" 						*/
#define CM_err_UndefObjects		 	 21	/* There is (are) ^0 undefined object(s) in container "^1"														*/
#define CM_err_NoStrValue			 	 22	/* Cannot allocate space for constant value in container "^0"													*/
#define CM_err_DupBaseType	 	 	 23	/* Cannot add dup base type "^0" to type "^1" in container "^2"												*/
#define CM_err_BadTOCWrite		 	 24	/* Incorrect byte length written while writing TOC for "^0"														*/
#define CM_err_NotGlobalName	 	 25 /* Have global name tied to value of wrong type in containter "^0"										*/
#define CM_err_BadGNameWrite	 	 26	/* Incorrect byte length written while writing global name "^0" in container "^1"			*/
#define CM_err_DupGlobalName	 	 27	/* Duplicate global name definition for "^0" in container "^1"												*/
#define CM_err_MultGlblNames	 	 28	/* Object ^0 alread defined -- trying to redefine it for "^1" (container "^2")				*/
#define CM_err_NoGlobalName 	 	 29	/* Cannot allocate space for global name "^0" in container "^1"												*/
#define CM_err_NoGNameLoad 		 	 30	/* Cannot allocate space for global name during load in container "^0"								*/
#define CM_err_BadGNameRead		 	 31	/* Incorrect byte length read while reading global name in container "^0"							*/
#define CM_err_NotGName				 	 32	/* Invalid global name string read in container "^0"																	*/
#define CM_err_BadType				 	 33 /* Invalid ^0 type passed to ^1 for container "^2"																		*/
#define CM_err_2Containers		 	 34	/* Objects not in same containers (containers "^0" and "^1")													*/
#define CM_err_3Containers		 	 35	/* Objects not in same containers (containers "^0", "^1", and "^2")										*/
#define CM_err_MissingMinIDSeed	 36	/* Min ID seed value missing in TOC object 1 in container "^0"												*/
#define CM_err_MissingTOCObj 		 37	/* TOC object ID 1 missing in TOC in container "^0"																		*/
#define CM_err_NotConverting 		 38	/* Cannot use CMDefineValueData except for converting container "^0"									*/
#define CM_err_BadDefineData 		 39	/* Attempt to define offset (^0) out of range in container "^1"												*/
#define CM_err_BadValue			 		 40	/* Attempt to use a deleted value	in container "^0"																		*/
#define CM_err_BadObject		 		 41	/* Attempt to use a deleted object in container "^0"																	*/
#define CM_err_BadContainer		 	 42	/* Container for ^0 ("^1") does not belong to the container being used ("^2")					*/
#define CM_err_NoValue				 	 43	/* No value defined for object ID ^0, property "^1" in container "^2"									*/
#define CM_err_HasValue				 	 44	/* Cannot set a (sub)value to an already defined value (container "^0")								*/
#define CM_err_AlreadyReleased1	 45	/* Attempting to release a value already completely released in container "^0"				*/
#define CM_err_NotReleased			 46	/* A dynamic value has not been fully released in container "^0"											*/
#define CM_err_MissingFreeTotal	 47	/* Total free space value missing in TOC object 1 in container "^0"										*/
#define CM_err_DupType				 	 48	/* Attempt to insert two values with the same type (^0) in container "^1"							*/
#define CM_err_HasNoValue		 		 49	/* No value defined for CMReadValueData in container "^0"															*/
#define CM_err_BadWrite				 	 50 /* Write error writing to container "^0"																							*/
#define CM_err_CantWriteGlbl 		 51	/* Cannot write to a global name in container "^0"																		*/
#define CM_err_Offset2Big		 		 52	/* Write/insert offset (^0) beyond end of a value in container "^1"										*/
#define CM_err_Internal1		 		 53	/* Internal error! Unknown flags setting (0x^0)																				*/
#define CM_err_MissingIDSeed 		 54	/* ID seed value missing in TOC object 1 in container "^0"														*/
#define CM_err_AmbiguousType 		 55	/* ^0 object is ambiguous in container "^1"																						*/
#define CM_err_TypeNotGlobal	 	 56 /* ^0 object is not for a global name in container "^1"																*/
#define CM_err_MissingFreeList	 57	/* Internal error - empty free list for property in container "^0"										*/
#define CM_err_NoNewValueHandler 58	/* A "new value" handler is not defined for type "^0"																	*/
#define CM_err_UndefReference		 59	/* Object ID ^0 from a reference is undefined in container "^1"												*/
#define CM_err_BadObjectKind	 	 60	/* Invalid kind of object -- expected ^0 in container "^1"														*/
#define CM_err_WriteIllegal1		 61	/* Cannot write to a container ("^0") opened for reading															*/
#define CM_err_WriteIllegal2 		 62	/* Attempt to write to a protected object in container "^0"														*/
#define CM_err_ReadIllegal	 		 63	/* Cannot read from a container ("^0") opened for writing															*/
#define CM_err_MissingSize		 	 64	/* Size value missing in TOC object 1 in container "^0"																*/
#define CM_err_BadSize				 	 65	/* Inconsistent size values between container label and TOC (container "^0")					*/
#define CM_err_Internal2	 		 	 66	/* Internal error! TOC offset to offset/size value not found in container "^0"				*/
#define CM_err_CantDelete1		 	 67	/* Attempt to delete to a protected object in container "^0"													*/
#define CM_err_CantDelete2		 	 68	/* Attempt to delete to a property with a protected value in container "^0"						*/
#define CM_err_CantDelete3		 	 69	/* Attempt to delete to a protected value in container "^0"														*/
#define CM_err_StillOpen	 			 70	/* Container "^0" is still open at session close time																	*/
#define CM_err_EmptyRead			 	 71	/* Cannot read from an empty embedded container value (container "^0")								*/
#define CM_err_NoEmbedding		 	 72	/* Cannot allocate space to save embedding status while opening container "^0"				*/
#define CM_err_BadGenNbr			 	 73	/* Invalid generation number (^0) passed to ^1 (container "^2")												*/
#define CM_err_NoRef		 				 74	/* Connot allocate space for an object reference in container "^0"										*/
#define CM_err_CantGetBase	 		 75	/* CMGetBaseValue() may only be called from a dynamic value handler in container "^0"	*/
#define CM_err_MultTypeProp		 	 76	/* Attempt to register a ^0 name (^1) in container "^2" -- already defined as a ^3		*/
#define CM_err_NotSameMode		 	 77	/* Embedded container ("^0") must be opened in same mode as its parent ("^1")					*/
#define CM_err_CantDelete4		 	 78	/* Cannot delete a value currently in use in container "^0"														*/
#define CM_err_MissingHandler		 79	/* Memory allocator and dealocator handlers must be supplied													*/
#define CM_err_NoMissingBuffer	 80	/* Unable to allocate private temporary buffer while opening type "^0"								*/
#define CM_err_MoveIllegal 			 81	/* Cannot move a value in a container ("^0") not opened for writing										*/
#define CM_err_DeleteIllegal		 82	/* Attempt to delete ^0 in a container ("^1") not opened for writing									*/
#define CM_err_BadDefineType 		 83	/* Attempt to define additional data for a immediate value in container "^0"					*/
#define CM_err_NoDynExtensions	 84	/* Cannot allocate space for TOC dynamic value entry for type "^0" in container "^1"	*/
#define CM_err_HandlerRecursed	 85	/* Attempt to use dynamic value handler for ^0() recursively in container "^1"				*/
#define CM_err_BadRealValue			 86	/* Invalid base ("real") value passed to ^0() in container "^1"												*/
#define CM_err_NoMetahandler		 87	/* A value operations metahandler is not defined for type "^0" in container "^1"			*/
#define CM_err_NotDynValue			 88	/* A "use value" metahandler did not create a dynamic value in container "^0"					*/
#define CM_err_NoGlobalTable 		 89	/* Cannot allocate globally unique name table for container "^0"											*/
#define CM_err_BadMetaSpec			 90	/* Invalid metadata format specification (%^0) for type "^1" in container "^2"				*/
#define CM_err_NoDeletesList 		 91	/* Internal error - empty "deletes" list for property in container ^0									*/
#define CM_err_NoDataPacket		 	 92	/* Cannot allocate space for data packet for type "^0" in container "^1"							*/
#define CM_err_BaseRelAttempted	 93	/* A dynamic value release handler attempted to release its base in container "^0"		*/
#define CM_err_NoDynMetahandler	 94	/* "Use value" handler for type "^0" in container "^1" MUST return a metahandler			*/
#define CM_err_MissingTotalSize  95	/* Total container size value missing in TOC object 1 in container "^0"								*/
#define CM_err_Internal3	 		 	 96	/* Internal error! TOC offset to container value not found in container "^0"					*/
#define CM_err_AmbiguousUseFlags 97	/* Ambiguous updating useFlags passed to CMOpenNewContainer(..."^0"...) - ^1					*/
#define CM_err_NoTypeHandler		 98	/* Dynamic value type handler not defined for updating in container "^0"							*/
#define CM_err_NotDynamicValue	 99	/* Dynamic value in container "^0" not created to access target for updating					*/
#define CM_err_NoMetaDataHandler 100/* A "metadata" handler is not defined for type "^0"																	*/
#define CM_err_NoDataBuffer		 	 101/* Cannot allocate space for data I/O buffer in container ^0													*/
#define CM_err_BadUpdateRead		 102/* Incorrect byte length read while reading updates in container ^0										*/
#define CM_err_BadUpdateWrite		 103/* Write error while writing updates in container ^0																	*/
#define CM_err_Internal4				 104/* Internal error! End-of-updates signal not detected in container ^0									*/
#define CM_err_Internal5				 105/* Internal error! TOC offset to updates TOC subsection not found in container ^0			*/
#define CM_err_NoNewValuesTOC	 	 106/* "New values" TOC offset/size missing in TOC object 1 in container ^0								*/
#define CM_err_wrappedIDs				 107/* Too many objects in container "^0" or next user object ID > 0xFFFFFFFF							*/
#define CM_err_NoTouchedEntry		 108/* Cannot allocate space for recording updating operation in container ^0							*/
#define CM_err_NoUpdateObject	   109/* Container ^0 updating -- cannot find a object ID ^1 to be ^2												*/
#define CM_err_NoUpdateProperty  110/* Container ^0 updating -- cannot find a property ID ^1 in object ID ^2 to be ^3			*/
#define CM_err_BadUpdateControl	 111/* Container ^0 updating -- unknown control byte read (0x^1, during ^2)								*/
#define CM_err_NoUpdateType	 		 112/* Container ^0 updating -- cannot find value (type ID ^1) in object ^2, property ^3	*/
#define CM_err_UndefUpdateObject 113/* Container ^0 updating -- cannot find ^1 (ID ^2) to use in ^3												*/
#define CM_err_UpdateBadGenNbr	 114/* Container ^0 updating -- invalid generation number (^1)														*/
#define CM_err_BadInsertData		 115/* Container ^0 updating -- bad updating info (0 segment insert encountered)					*/
#define CM_err_BadInsertOffset	 116/* Container ^0 updating -- insert offset (^1) beyond end of a value									*/
#define CM_err_CantRepImmediate	 117/* Container ^0 updating -- attempt to replace non-immediate with immediate						*/
#define CM_err_CantRepBaseType	 118/* Container ^0 updating -- attempt to replace non-base type value with a base type		*/
#define CM_err_CantReference		 119/* Value and referenced object not in the same containers (containers "^0" and "^1")	*/
#define CM_err_GloblaNameError	 120/* Allocation problem while looking up global name "^0" in container "^1"							*/
#define CM_err_FlushErr					 121/* Error detected in flushing output while closing container ^0												*/
#define CM_err_CantDelete5		 	 122/* Cannot delete an object with unreleased dynamic values in use in container "^0"		*/
#define CM_err_NoTOCBuffer			 123/* Cannot allocate space for TOC I/O buffer in container "^0"													*/
#define CM_err_BadTOCCode				 124/* Invalid TOC code read (0x^0) from TOC in container "^1"														*/
#define CM_err_TOCoutOfSync			 125/* TOC reading out-of-sync (trying to read across buffer boundary) in container "^0"	*/
#define CM_err_TOCParseErr1			 126/* TOC "parse" error - expected object ID, got 0x^0 in container "^1"									*/
#define CM_err_TOCParseErr2			 127/* TOC "parse" error - got 0x^0 at an unexpected time in container "^1"								*/
#define CM_err_Unsupported1			 128/* 8-byte offset in container "^0" are not supported in this implementation						*/
#define CM_err_CantDelete6		 	 129/* Cannot delete an object currently in use in container "^1"													*/
#define CM_err_AlreadyReleased2	 130/* Attempting to release an object already completely released in container "^0"			*/
#define CM_err_BadRefRead		 	 	 131/* Read error while reading references in container ^0																*/
#define CM_err_Internal6				 132/* Internal error! Missing reference property or value in container "^0"							*/
#define CM_err_ZeroRefKey				 133/* Attempt to use a CMReference key of 0 in container "^0"														*/
#define CM_err_NoRefShadowList	 134/* Cannot allocate space to record reference in container "^0"												*/

#endif
