/* @(#)Z 1.4 com/src/cm/CMAPITyp.h, odstorage, od96os2, odos29646d 96/11/15 15:26:43 (96/10/29 09:16:13) */
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
	File:		CMAPITyp.h

	Contains:	Container Manager API Types and Constants

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182308 Add kCMLittleEndianTwin flag.
									#1182275 Add kCMMerging to open mode
		 <4>	 1/19/94	EL		Add kCMNextGeneration for automatic
													generation of generation number.

	To Do:
*/

/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<< CM_API_Types.h >>>                          |
 |                                                                           |
 |                  Container Manager API Types and Constants                |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/18/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file contains all the type and constant definitions used by the Container Manager
 API.


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

#ifndef __CM_API_TYPES__
#define __CM_API_TYPES__

#ifndef __CM_API_ENV__
#include "CMAPIEnv.h"
#endif


/*--------------------------*
 | Low level basic types... |
 *--------------------------*/

typedef char 						CM_CHAR;									/* 1-byte objects											*/
typedef unsigned char 	CM_UCHAR;

typedef short						CM_SHORT;									/* 2-byte objects											*/
typedef unsigned short	CM_USHORT;

typedef long						CM_LONG;									/* 4-byte objects											*/
typedef unsigned long		CM_ULONG;


/*----------*
 | Types... |
 *----------*
 
 All Container Manager types are defined here with the intent to aid in enforcing compiler
 type checking.  Note that "refNums" for session data, container control blocks, values,
 and objects are uniquely typed to strictly enforce type checking of those entities. These
 types are defined as pointers derived from an "incomplete type", i.e. structs.  The
 structs are NOT defined.  In ANSI C, an "incomplete type" need not be defined.
 
 Further, for all the following types defined as pointers, a corresponding "CMconst_..." is
 defined.  These allow for function paramters of the form "pointer to constant CM type".
 Note that, in ANSI, "const CMFooPtr p" is NOT the same as "const CMFoo *p". The latter is
 what we desire, but the former defines a "consting pointer to a CMFoo" not a "pointer to a
 constant CMFoo".  Hence the additional types are required.  Hey, I didn't make the rules!
*/

typedef	struct CMSession_ 	CM_PTR *CMSession;		/* pointer to session (task) data			*/
typedef struct CMContainer_ CM_PTR *CMContainer;	/* "refNum" for containers						*/

typedef struct CMObject_ 		CM_PTR *CMObject;			/* "refNum" for objects								*/
typedef CMObject 										CMProperty;		/* "refNum" for property objects			*/
typedef CMObject 										CMType;				/* "refNum" for type objects					*/

typedef struct CMValue_ 		CM_PTR *CMValue;			/* "refNum" for values								*/

typedef CM_CHAR		CM_PTR *CMOpenMode;							/* handler open mode string pointers	*/
typedef CM_CHAR 	CM_PTR *CMGlobalName;						/* global unique name pointers				*/
typedef CM_CHAR		CM_PTR *CMErrorString;					/* error message string pointers			*/
typedef CM_CHAR		CM_PTR *CMMetaData;							/* type metadata string pointers			*/

typedef void			CM_PTR *CMRefCon;								/* reference constants ("refCon"s)		*/
typedef void 			CM_PTR *CMPtr;									/* arbitrary data pointers						*/

typedef CM_UCHAR	CM_PTR *CMMagicBytes;						/* magic byte pointers								*/
typedef CM_UCHAR	CM_PTR *CMDataPacket;						/* "new value" handler data packets		*/
typedef CM_UCHAR	CM_PTR *CMDataBuffer;						/* ptr to data buffer for handlers		*/
typedef CM_UCHAR	CM_PTR *CMPrivateData;					/* ptr to private CM data for handlers*/

typedef	const struct CMSession_ 	CM_PTR *CMconst_CMSession;		/* ptr to const types...*/
typedef const struct CMContainer_ CM_PTR *CMconst_CMContainer;
typedef const struct CMObject_ 		CM_PTR *CMconst_CMObject;
typedef CMconst_CMObject 									CMconst_CMProperty;
typedef CMconst_CMObject 									CMconst_CMType;
typedef const struct CMValue_ 		CM_PTR *CMconst_CMValue;
typedef const CM_CHAR							CM_PTR *CMconst_CMOpenMode;
typedef const CM_CHAR 						CM_PTR *CMconst_CMGlobalName;
typedef const CM_CHAR							CM_PTR *CMconst_CMErrorString;
typedef const CM_CHAR							CM_PTR *CMconst_CMMetaData;
typedef const void								CM_PTR *CMconst_CMconst_CMRefCon;
typedef const void 								CM_PTR *CMconst_CMPtr;
typedef const CM_UCHAR						CM_PTR *CMconst_CMMagicBytes;
typedef const CM_UCHAR						CM_PTR *CMconst_CMDataPacket;
typedef const CM_UCHAR						CM_PTR *CMconst_CMDataBuffer;
typedef const CM_UCHAR						CM_PTR *CMconst_CMPrivateData;

typedef CM_UCHAR	CMReference[4];									/* referenced object data pointers		*/

typedef CM_UCHAR 	CMSeekMode;											/* container "fseek()" handler modes	*/
typedef CM_UCHAR 	CMBoolean;											/* boolean values/functs. (0==>false)	*/

typedef CM_USHORT CMContainerUseMode;							/* container open use mode flags			*/
typedef CM_USHORT CMContainerFlags;								/* container label flags							*/
typedef CM_USHORT	CMContainerModeFlags;						/* container open mode flags					*/
typedef CM_USHORT CMValueFlags;										/* value flags												*/
typedef CM_USHORT CMEofStatus;										/* "feof()" handler result status			*/

typedef CM_LONG 	CMErrorNbr;											/* error handler error numbers				*/

typedef CM_ULONG 	CMGeneration;										/* container generation numbers				*/
typedef CM_ULONG 	CMSize;													/* sizes															*/
typedef CM_ULONG 	CMCount;												/* amounts or counts									*/
typedef CM_ULONG 	CMObjectID;											/* object ID's												*/

CM_CFUNCTIONS																			/* see CM_API_Environment.h about this*/
typedef void (CM_FIXEDARGS *CMHandlerAddr)();			/* handler address pointers		  			*/
typedef	CMHandlerAddr (CM_FIXEDARGS *CMMetaHandler)(CMType,							/* metahandlers	*/
																										CMconst_CMGlobalName);
CM_END_CFUNCTIONS


/*--------------*
 | Constants... |
 *--------------*/

/* The following flags are passed to CMOpenNewContainer() as CMContainerFlags.  These 	*/
/* flags are stored in the container label and define some constant attributes about the*/
/* container.	 The container flags may be retrieved by calling the CMGetContainerInfo() */
/* API routine.																																					*/

#define kCMLittleEndian		0x0001U									/* little-endian byte ordering				*/
#define kCMLittleEndianTwin	0x0101U									/* little-endian byte ordering				*/

/* The following flags are passed to CMOpen[New]Container() as CMContainerUseMode 			*/
/* useFlags.  They modify the open in the indicated ways. Note, kCMReading, kCMWriting, */
/* kCMUpdating are also returned from CMGetContainerInfo() to indicate the mode of the 	*/
/* container, i.e., it was opened for reading, writing, or updating.										*/

#define kCMReading				0x0001U									/* container was opened for reading		*/
#define	kCMWriting				0x0002U									/* container was opened for writing		*/
#define kCMReuseFreeSpace	0x0004U									/* try to reuse freed space					  */
#define kCMUpdateByAppend	0x0008U 								/* open container for update-by-append*/
#define kCMUpdateTarget		0x0010U									/* open container for updating target */
#define kCMConverting		  0x0020U 								/* open container for converting		  */
#define kCMMerging			  0x0040U 								/* container may be merged					  */

#define kCMUpdating				(kCMReuseFreeSpace | kCMUpdateByAppend | kCMUpdateTarget)

/* The following flags are options to the "seek" I/O handler.														*/

#define kCMSeekSet 				0x00 										/* "fseek()" handler mode (pos)				*/
#define kCMSeekCurrent 		0x01 										/* "fseek()" handler mode (curr+pos)	*/
#define kCMSeekEnd 				0x02 										/* "fseek()" handler mode (end+pos)		*/

/* The following are the flags that appear in the flags field of the TOC itself.  Some 	*/
/* are for internal use only and do not appear in TOC entries in the container.  The 		*/
/* comments designate which are these.																									*/

#define kCMImmediate 			0x0001U 								/* immediate value flag								*/
#define kCMContinued 			0x0002U 								/* continue value flag								*/

#define kCMDynValue 			0x4000U 								/* dynamic value flag (internal use)	*/
#define kCMGlobalName 		0x8000U 								/* global name flag		(internal use)	*/

/* The Container label contains a buffer size field which is used by the Container 			*/
/* Manager for buffered TOC I/O. This field, times 1024, yields the actual buffer size. */
/* Since this field (as it is in the label) can be returned by CMGetContainerInfo(), the*/
/* following constant is provided to get the actual buffer size in bytes.								*/

#define kCMBufSizeUnits		1024										/* bufSize*1024==actual TOC bufr size	*/

/* When we do CMOpenNewContainer updating another container, we may want to have a			*/
/* generation number equal to target container plus 1. It may not be easy since we do		*/
/* not know the generation number of the target container without opening it.						*/
/* Use kCMNextGeneration in the generation number would automatically do it for you			*/

#define	kCMNextGeneration	0x0000U

#endif
