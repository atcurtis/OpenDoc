/* @(#)Z 1.5 com/src/cm/Containr.h, odstorage, od96os2, odos29646d 96/11/15 15:27:03 (96/10/29 09:17:46) */
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
	File:		 Containr.h 

	Contains:	Container Manager Containers Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 9/30/94	EL		#1182275 rename mergeCandidate to
									updateMergeCandidate.
		 <2>	 8/26/94	EL		#1182275 Add new field mergeCandidate to
													the container struct.
		 <4>	  5/5/94	EL		Minor version one for new data format not
													supported in 2.0.
		 <3>	 3/31/94	EL		Add field mergeInSize to container.
													#1150214
		 <2>	 3/17/94	EL		Add support in container data structure for
													delete garbage and truncate back to
													original size.
		 <1>	  2/3/94	EL		first checked in
		 <2>	 1/19/94	EL		Add tmpFreeList to container.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<<  Containr.h  >>>                           |
 |                                                                           |
 |                  Container Manager Containers Interfaces                  |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/02/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This header defines the container control block and some common info about the container
 label.  Everyone includes this thing since it acts as the "global variable" data space
 tied to each individual opened or embedded container.
*/

#ifndef __CONTAINEROPS__
#define __CONTAINEROPS__

#include <setjmp.h>

/*------------------------------------------------------------------------------------*
 | W A R N I N G - - -> this header as well as  TOCObjs.h   and  TOCEnts.h   all have |
 |                      mutual references.  The include order is NOT arbitrary.  It's |
 |                      the only one that works!  Even with it we have to do some     |
 |                      forward struct pointer references of the form "struct foo *p" |
 *------------------------------------------------------------------------------------*/

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif

                                   CM_CFUNCTIONS

struct SessionGlobalData;
struct Container;
struct TOCObject;
struct TOCValueHdr;
struct TOCValue;
struct EmbeddedContainer;
struct GlobalName;


#define MagicByteSequence "\xA4""CM""\xA5""Hdr""\xD7"		/* Must be 8 characters					*/

#define MajorVersion			2															/* major format version number	*/
#define MinMinorVersion		0															/* min minor format version   	*/
#define MinorVersion			1															/* minor format version number	*/

#define MinStdObjectID		CM_StdObjID_MinGeneralID			/* min standard object id				*/
#define MinUserObjectID		0x00010000UL									/* min user object id						*/

#define IsStdObject(id)		(((id) & 0xFFFF0000UL) == 0) 	/* tests for ID type						*/
#define IsUserObject(id) 	(((id) & 0xFFFF0000UL) != 0)

/* the default and the minimum acceptable generation number is 1												*/			
#define kCMDefaultGeneration	1													/* the default generation 			*/

/* The following is used to test for updating modes that use one container to update 		*/
/* another...																																						*/

#define UPDATING(updatingContainer) (((updatingContainer)->useFlags & (kCMUpdateTarget | kCMUpdateByAppend)) != 0)

/* The following macro is used to see if touched records are to be generated...					*/

#define TouchIt(updatingContainer, container) (updatingContainer->touchTheValue 	&& \
																							 (updatingContainer) != (container) && \
																							 UPDATING(updatingContainer))


												/*-------------------------------*
												 | The "Container Control Block" |
												 *-------------------------------*/

struct Container {												/* Layout for a "Container Control Block":		*/
	ListLinks						containerLinks;			/*    in-use Containers links (must be first)	*/
	struct Container		*thisContainer;			/* 		validation check on the container ptr		*/
	
	struct SessionGlobalData *sessionData;	/* 		pointer of session (task) global data		*/
	
	struct Container		*updatingContainer;	/*		ptr to container recording new updates	*/
	CM_USHORT						depth;							/*		target container "depth"								*/
	CMBoolean						openingTarget;			/*		true ==> updater is opening target			*/
	CMBoolean						updateMergeCandidate;	/*		true ==> container may be merged			*/
	
	struct Container		*targetContainer;		/*		ptr to container being updated or used	*/
	CMValue							pointingValue;			/*		refNum of container's pointing value		*/
	
	HandlerOps	 				handler;						/* 		handler basic operations vector					*/
	MetaHandlerPtr 			metaHandlerProc;		/*		ptr to type and "metahandler" proc			*/
	CMRefCon 	 					refCon;							/* 		handler's refCon												*/
	
	unsigned char				magicBytes[10];			/*		magic byte sequence											*/
	CM_USHORT						majorVersion;				/* 		major format version										*/
	CM_USHORT						minorVersion;				/*		minor format version										*/
	
	CM_USHORT			 			containerFlags;			/*		container ("label") flags								*/
	CM_ULONG						generation;					/*		generation number												*/
	CM_ULONG						tocBufSize;					/*		actual TOC buffer size (lbl value*1024)	*/
	
	void 								*toc;								/*		ptr to TOC (this container's or target)	*/
	void 								*privateTOC;				/*		ptr to this container's private TOC			*/
	CM_ULONG						tocOffset;					/*		offset to the start of the TOC					*/
	CM_ULONG						tocSize;						/*		total size of the TOC in the container	*/
	CMBoolean						tocFullyReadIn;			/*		true ==> an "old" TOC is fully loaded		*/
	CMBoolean						usingTargetTOC;			/*		true ==> currently using target's TOC		*/
	CM_ULONG						tocInputOffset;			/*		current TOC input offset								*/
	void								*ioBuffer;					/*		current buffered TOC I/O buffer					*/
	void								*tocIOCtl;					/*		current TOC I/O control block pointer		*/
	
	struct TOCValue			*tocIDSeedValue;		/*		ptr to TOC ID seed value       (not hdr)*/
	struct TOCValue			*tocIDMinSeedValue;	/*		ptr to TOC ID min seed value   (not hdr)*/
	struct TOCValue			*tocObjValue;				/*		ptr to TOC object value        (not hdr)*/
	struct TOCValue			*tocContainerValue;	/*		ptr to TOC container value     (not hdr)*/
	struct TOCValue 		*spaceDeletedValue;	/*		ptr to TOC space deleted value (not hdr)*/
	struct TOCValue 		*tocNewValuesValue;	/*		ptr to TOC new values TOC			 (not hdr)*/
	struct TOCValueHdr	*freeSpaceValueHdr;	/*		ptr to TOC free list (this IS value hdr)*/
	struct TOCValueHdr	*deletesValueHdr;		/*		ptr to TOC deletes list (also value hdr)*/
	ListHdr							tmpList;						/*		free space available at close time			*/
		
	void								*globalNameTable;		/* 		ptr to global name tbl (or target)			*/
	void								*privateGlobals;		/* 		ptr to this container's global name tbl	*/
	CMBoolean						usingTargetGlobals;	/*		true ==> currently using target's glbls	*/
	CMBoolean						deleteGarbage;			/*		garbage collect before closing					*/

	CM_ULONG						nextStdObjectID;		/*		next available std object id number			*/
	CM_ULONG						nextUserObjectID;		/*		next available user object id number		*/
	
	CM_USHORT						useFlags;						/*		container use flags											*/
	CM_ULONG						physicalEOF;				/*		next free byte in the container					*/
	CM_ULONG						logicalEOF;					/*		offset to highest byte written + 1			*/
	CM_ULONG						maxValueOffset;			/*		max+1 allowed for value definitions			*/
	CM_ULONG						originalEOF;				/*		size before update begins								*/

	CM_ULONG						mergeInSize;				/*		the size of previous container merged in*/
		
	ListHdr 						deletedValues; 			/* 		list of all deleted values (headers)		*/
	
	ListHdr 						embeddedContainers; /* 		list of containers embedded in this one	*/
	struct TOCValueHdr	*embeddedValue;			/*		ptr to valueHdr for embedded container	*/
	struct EmbeddedContainer *parentListEntry;/*	back ptr to parent's list entry					*/
	
	void 								*tocActions;				/*		ptr to TOC walk through (private) data	*/
	
	CM_SHORT						getBaseValueOk;			/*		>0 ==> CMGetBaseValue() allowed					*/
	struct TOCValueHdr	*dynamicBaseValueHdr;/*		CMReleaseValue() dynamic value base			*/
	CM_LONG							nbrOfDynValues;			/*		count of dynamic values in container		*/
	
	CMProperty					dynValueProperty;		/*		property refNum for dynamic values			*/
	CMProperty					baseTypesProperty;	/*		property refNum for base types					*/
	
	CMBoolean						trackFreeSpace;			/*		true ==> allows tracking of freed space	*/
	CMBoolean						touchTheValue;			/*		true ==> appropriately record the touch	*/

	ListHdr 						activeIOBuffers;		/* 		list of active buffered I/O users				*/
	
	struct TOCObject		*touchedChain;			/* 		chain of objects touched during updating*/
};
typedef struct Container Container, *ContainerPtr;

/* NOTE: there is a mutual reference here between Container and TOCObject.  They refer	*/
/* to each other!  Thus we must do the weird "struct TOCObject" to define the list			*/
/* pointers above.  Sorry about that!																										*/

/* To make it easier and more self-documentating for using the "seeds" the following 		*/
/* macros are provided.  These both assume the current container pointer is the variable*/
/* "container".																																					*/

#define Seed		(container->tocIDSeedValue->value.imm.ulongValue)
#define MinSeed (container->tocIDMinSeedValue->value.imm.ulongValue)

/* Similarly, we define the following macros to get at the updating TOC limits of non-	*/
/* private objects...																																		*/

#define TOCNewValuesTOCOffset	(container->tocNewValuesValue->value.notImm.value)
#define TOCNewValuesTOCSize		(container->tocNewValuesValue->value.notImm.valueLen)

/* The following defines the layout for list entries of a list of containers directly		*/
/* embedded in another container.  A container sort of is like a "super class" with its	*/
/* embedded containers being "subclasses".  There can be any number of embedded 				*/
/* containers open simultaneously (memory permitting). Each of these, in turn, can have	*/
/* embedded containers of their own!																										*/

struct EmbeddedContainer {								/* Layout of a embedded container list entry:	*/
	ListLinks		 			 siblingsLinks;				/*    sibling Container links (must be first)	*/
	ContainerPtr			 container;						/*		ptr to the embedded container						*/
};																				/*		note, theValueHdr gives us father contnr*/
typedef struct EmbeddedContainer EmbeddedContainer, *EmbeddedContainerPtr;


/* Whenever we write to the container, we keep track of what we think is the EOF (last 	*/
/* written byte + 1). We refer to this as the "logical EOF" since it may or may not be 	*/
/* equal to the true container EOF.  For normal writing it probably is always the same.	*/
/* But for reusing free space we could be reusing free list space and the logical EOF 	*/
/* may end up less than the physical EOF.																								*/

/* The reason we want to know this is mainly because the standard ANSI C stream I/O*/
/* has no support for cutting back a file!  This is portable code and we must assume 		*/
/* that, at a minimum, the I/O handlers are using stream I/O.  We thus must take into 	*/
/* account the possible extra space that could result from space reuse when the updates */
/* use less space than what was originally there.																				*/

#define SetLogicalEOF(eof)	if ((eof) > container->logicalEOF) \
															container->logicalEOF = (eof)


												/*---------------------------------------*
												 | Defined Layout of the Container Label |
												 *---------------------------------------*

As defined by the API documentation, a container label is always placed at the END of the
container and has the following layout:

			 0                       8     10    12    14    16          20          24
			+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			|      Magic Bytes      |flags|bufsz|major|minor|TOC offset |  TOC size |
			+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
									8              2     2     2     2        4           4

These are defined as "hard-coded" constants since the format is, by definition, platform
independent. 	The actual formatting and I/O for the container label is done by a handler
because of this.																																				*/

#define LBLmagicBytes			0								/* 8 bytes: the magic byte identifier					*/
#define LBLflags					8								/* 2				the label flags										*/
#define LBLbufSize			 10								/* 2				TOC buffer size / 1024						*/
#define LBLmajorVersion	 12								/* 2				major format version number				*/
#define LBLminorVersion	 14								/* 2				minor format version number				*/
#define LBLtocOffset		 16								/* 4				offset to start of TOC						*/
#define LBLtocSize			 20								/* 4				total byte size of the TOC				*/

#define LBLsize					 24								/* defined size of the container label				*/

														 CM_END_CFUNCTIONS
#endif
