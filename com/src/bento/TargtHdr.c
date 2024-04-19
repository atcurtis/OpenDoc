/* @(#)Z 1.4 com/src/bento/TargtHdr.c, odstorage, od96os2, odos29646d 96/11/15 15:26:37 (96/10/29 09:13:57) */
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
	File:		TargtHdr.c

	Contains:	Target Handler for Bento Container Suite

	Written by:	Vincent Lo, Ed Lai

	Copyright:	© 1993-1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <5>	  4/7/95	EL		Check the label rather than the passed in
									flag for endian-ness.
		 <4>	 3/10/95	EL		1226127: move scanning backwards for label
													code to FSHdr.cpp.
		 <3>	 9/29/94	RA		1189812: Mods for 68K build.
		 <2>	 8/26/94	EL		#1182308 Allows non-byte swapping
													format/extract.
		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                     <<< TargetContainerHandlers.c >>>                     |
 |                                                                           |
 |            Container Manager Basic Updating Container Handlers            |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  5/28/92                                  |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992                   |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file contains an the set of basic container handlers and its metahandler used by the
 Container Manager when doing updating operations on a target container.  The "I/O" done
 here is in terms of Container Manager API calls on a dynamic OR real value (more about
 this later).

 In order to understand how the handlers in this file are used it is necessary to 
 understand their context.  Remember, when CMOpen[New]Container() is called, a type name
 is always passed so that the Container Manager can find the metahandler/type association
 which will yield the I/O handlers that operate upon the container to be opened.  That is
 always true; there are a set of container I/O handlers associated with every open
 container.  ExampleHandlers.c is such a set.
 
 When the parameters to CMOpenNewContainer() are such as to indicate that ANOTHER (called
 the "target") container is to be updated (kCMUpdateTarget useFlags), then the target is
 opened by the Container Manager, NOT explicitly by the API user.  Note, this is only the
 case for CMOpenNewContainer() and not CMOpenContainer(), because the intent is to record
 the updating operations in a separate container without actually modifying the target. 
 Thus the updating operations are recorded, obviously in a container opened for writing
 (hence CMOpenNewContainer()), while the target is always opened for reading 
 (CMOpenContainer()).
 
 There is also a case where you can append the updates to the SAME container.  This is 
 referred to as update-by-append (CMOpenNewContainer() with kCMUpdateByAppend useFlags).
 In this case, all updates are appended to an existing container, and an additional TOC
 is layered on to the end of the container when closed (something like opening a data file
 to be made into a continer by opening it for converting -- kCMConverting useFlags).  Each
 time the container is opened and then closed for update-by-append, the new updates and a
 new TOC are appended.  Whenever such a container is opened (by any mode), all the updates
 are applied appropriately to the original container.  Conceptually, you can always look
 at it as an updating container opened for writing and the target container opened for
 reading.
 
 The target container is opened by the Container Manager using CMOpenContainer() just as
 the API user would do it.  It's done internally, so there are other "gyrations" going on.
 But the bottom line is that there still must be a metahandler/type association as usual.
 The type for the targets is always CMTargetHandlersTypeName (a macro).  The methandler
 associated with this type is the one defined in this file to yield the handlers to be
 used to read the target container.  The metahandler/type association still must be done
 as usual by the API user via CMSetMetaHandler().
 
 In summary, there are a set of standard handlers for the updating container which are
 whatever they are.  Then there is the set of handlers defined in this file which are
 used to read a target container.  The context for this file should now be clear!
 
 Now, some more details about the updating and how these handlers are used.
 
 Containers are updated by a series of instructions contained in a TOC.  The TOCs may be
 part of other containers or separate (layered) TOCs in the same container that is to be
 updated.  In either case, the Container Manager "gets at" what is to be updated (the
 target container) through a special value contained in the updating TOC.  This value
 belongs to a unique property of TOC ID 1 and is referred to here as the "pointing value"
 (because it "points" in some fashion to the target container to be updated).
 
 Because there are two cases (i.e., container to be updated is the same or a different
 container), the pointing value takes two forms:
 
 (1) For updating the SAME container - the value is a "real" value whose offset and length
 		 info (i.e., its value data) is the entire container to be updated (including its
		 label).  The value belongs to the updating container and hence a distinct laryered
		 TOC.  It is used by the target by the handlers in this file.
		 
 (2) For updating a SEPARATE container - the value is a real base value of some user
 		 defined  type that will spawn a dynamic value if a CMUseValue() or CMNewValue() is
		 done on it.  Again, the value belongs to the updating container and is used by the
		 target by the handlers in this file.

 As discussed above, the context in which the handlers in this file are used are for use
 by the target container (the one opened implicitly as a result of opening the updating
 container).  They do this by mapping the container operations on to standard value API
 calls using the pointing value (e.g., CMReadValueData()).  Thus for case (1), a real value
 for layered TOC of the same container, calls back to the API would end up calling the
 handlers for the updating container.  For case (2), a  dynamic value, the dynamic value's
 handler package would be called to do whatever needs to be done to "get at" the target
 container.
 
 In other words, the handlers for a target container, always use the value for one layer
 "up", its updater.  For multiple updaters reading from a target will work its way back
 up the chain of values doing their operatations in terms of the next value up.  This will
 continue until a top-most value is reached.  That top-most value will either be one for
 the top-most updating container or a dynamic value.  If not dynamic, the top-most value
 will use the handlers for that container (i.e., "real" handlers like those of 
 ExampleHandlers.c).  For dynamic values, it would be the dynamic value-s handler package.
  
 Unlike the other example handler packages, the ones defined here MUST be done in terms of
 an interface conventions known to the Container Manager.  Thus, this handler package is
 NOT (repeat NOT) in the "example" category as all the other handler packages are!  Its
 interface conventions are not subject to change.  However, just like the other example
 handlers, the code must be published, and for the same reasons.  Namely, different
 hardware may required modifications to the code (mainly in the TOC and label I/O
 handlers).

 In order to accomplish these goals, the metahandler defined in this file must be
 associated with the PREDEFINED type CMTargetHandlersTypeName (a macro in the header for
 this file).  This is done by the API user in the standard way using CMSetMethandler().
 
 There is no explicit routine to set up the refCon.  Here we take advantage of the
 "attributes" mechanism passed to CMOpen[New]Container().  This is where the "attributes"
 (assumed a form of refCon) is passed as-is to the open handler.  The open handler, in
 turn, returns the refCon to be passed among all the other handlers.  So all that the
 Container Manager needs to do is pass the pointing value in the "attributes".  From that
 the open handler can build the refCon.  This is one of the interface conventions assumed
 by the Container Manager for the CMTargetHandlersTypeName.
   
 Since this file is not an example, the commenting is not done in the narative form used
 in the examples and handlers not needed are not included.  Indeed, targets are only
 opened for reading, and thus no writing handlers are needed.  You should be looking at
 ExampleHandlers.c, not this file, for the documentation on how to write container
 handlers.
 
 There is one final thing to point out here.  If you compare the code in this file with
 the code for embedded handlers, ExampleEmbeddedHandlers.c, you will see they are very
 similar.  Indeed, ExampleEmbeddedHandlers.c was used as the basis for this file!  Here,
 and there, all operations are done in terms of value operations on a base value.  For
 embedded containers the value is one already in the container.  Here, for case (1) at
 least, the value is also in the same container, although in a different TOC.  Case (2)
 has no counterpart with embeddeds.  But if you associate a dynamic value with an
 embedded container the effect is the same.
 
 When you stop to think of it, for case (1) at least, it makes sense that we would be
 similar to embedded containers.  The reason we don't use that code is because of the
 reasons we mentioned above, i.e., the interface must be known by the Container Manager.
 Also we wanted to fully document an additional example of container handlers.
 
 There are some other subtle changes from ExampleEmbeddedHandlers.c.  First, we let the
 error reporter return instead of aborting execution.  The caller's return something
 "reasonable" to the Container Manager to take appropriate action.  Second, there is
 no returnParentValue_Handler() since this is not a true embedded container case. 
 Basically all that means is that the close of updating containers is handled "normally".
 Closes of embedded containers involve possible closing of all descendents in a tree of
 nested embedded containers. Finally, third, and not so subtle, as mentioned above, there
 are no handlers that do writing, since targets are always opened for reading.
*/


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "CMAPI.h"
#include "TargtHdr.h"


/*--------------------------------------------------------------------------*
 | targetContainerMetahandler - metahandler to return the handler addresses |
 *--------------------------------------------------------------------------*
 
 This is the metahandler that CMOpen[New]Container() associates with a type passed to the
 target CMOpenContainer().  Here are the requisite forward declarations to those handlers
 for the proc pointer table defined in the code below.																	*/
 
 																CM_CFUNCTIONS
																
CMHandlerAddr CM_FIXEDARGS targetContainerMetahandler(CMType targetType, CMconst_CMGlobalName operationType);

static CMRefCon open_Handler(CMRefCon attributes, CMOpenMode mode);
static void close_Handler(CMRefCon refCon);
static CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode);
static CMSize tell_Handler(CMRefCon refCon);
static CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
static CMSize containerSize_Handler(CMRefCon refCon);
static void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
															 CMContainerFlags *flags, CM_USHORT *bufSize,
															 CM_USHORT *majorVersion, CM_USHORT *minorVersion,
															 CMSize *tocOffset, CMSize *tocSize);
static CM_UCHAR *returnContainerName_Handler(CMRefCon refCon);
static void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer,
 																 CMSize size, CMPrivateData data);
static void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer, CMSize size, CMPrivateData data);

															CM_END_CFUNCTIONS
																																												/*
 See comments in CM_API_Environment.h about CM_CFUNCTIONS/CM_END_CFUNCTIONS (used only
 when compiling under C++).
 
 Note, the only action handlers we need are for input since these handlers are only used
 for reading the target container.  formatData_Handler() is generally considered as an
 output handler, but is used by the Container Manager for converting CMReference keys to
 internal hardware representation.
*/

CMHandlerAddr CM_FIXEDARGS targetContainerMetahandler(CMType targetType, CMconst_CMGlobalName operationType)
{
	static char *operationTypes[] = {CMOpenOpType,					/*  0 */ /*  Operation Types	*/
																	 CMCloseOpType,					/*  1 */
																	 CMFlushOpType,					/*  2 */
																	 CMSeekOpType,					/*  3 */
																	 CMTellOpType,					/*  4 */
																	 CMReadOpType,					/*  5 */
																	 CMWriteOpType,					/*  6 */
																	 CMEofOpType,						/*  7 */
																	 CMTruncOpType,					/*  8 */
																	 CMSizeOpType,					/*  9 */
																	 CMReadLblOpType,				/* 10 */
																	 CMWriteLblOpType,			/* 11 */
																	 CMParentOpType,				/* 12 */
																	 CMContainerOpName,			/* 13 */
																	 CMTargetTypeOpType,		/* 14 */
																	 CMExtractDataOpType,		/* 15 */
																	 CMFormatDataOpType,		/* 16 */
																	 NULL};
	char 	 **t;
	CMType ignored = targetType;
	
	/* Look up the operation type in the operationTypes table above...										*/
	
	t = operationTypes - 1;
	while (*++t) if (strcmp((char *)operationType, *t) == 0) break;

	/* Now that we got it (hopefully), return the appropriate routine address...					*/
	
	switch (t - operationTypes) {
		case  0:	return ((CMHandlerAddr)open_Handler);								/*CMOpenOpType 				*/
		case  1:	return ((CMHandlerAddr)close_Handler);							/*CMCloseOpType 			*/
		case  3:	return ((CMHandlerAddr)seek_Handler);								/*CMSeekOpType 				*/
		case  4:	return ((CMHandlerAddr)tell_Handler);								/*CMTellOpType 				*/
		case  5:	return ((CMHandlerAddr)read_Handler);								/*CMReadOpType 				*/
		case  9:	return ((CMHandlerAddr)containerSize_Handler);			/*CMSizeOpType 				*/
		case 10:	return ((CMHandlerAddr)readLabel_Handler);					/*CMReadLblOpType 		*/
		case 13:	return ((CMHandlerAddr)returnContainerName_Handler);/*CMContainerOpName		*/
		case 15:	return ((CMHandlerAddr)extractData_Handler);				/*CMExtractDataOpType	*/
		case 16:	return ((CMHandlerAddr)formatData_Handler);					/*CMFormatDataOpType	*/
		
		default:	return (NULL);
	}
}

static void ODFlipMove(CMPtr from, CMPtr to, CM_LONG size)
{
	char	*dest = (char *)to + size - 1;
	char	*src = (char *)from;
	
	while (size--)
		*dest-- = *src++;			
}

/*----------------------------------------------------------------*
 | open_Handler - open a container for input, output, or updating |
 *----------------------------------------------------------------*
 
 Called to open the container associated with the specified refCon.  The open modes passed
 are "wb+" (truncate and updating) for creating a new container, "rb" for reading an
 existing container, and "rb+" (updating) for converting a "bunch" of data in a file to
 container format.

 The open routine is unique among the handlers in that the potential interpretion of the
 first argument is different.  The attributes are passed to CMOpen[New]Container() and
 then passed to here unaltered and not looked at by the API.  It is intended to tie the
 open handler to a specific container.  Thus the attributes serves as a communication
 channel strictly to the open.  In the case of updating containers, the attributes is the
 pointing value (i.e., a CMValue).  As discussed at the start of this file it will be 
 either a real or a dynamic value. 

 What the API expects as a return value is a "refCon" to be passed unaltered to all of the
 other handler routines.  This too is a communication channel that ties the handlers to
 their open.  Using the passed pointing value, we return a refCon that has the following
 layout:																																								*/
 
 struct RefCon {																	/* The refCon layout:									*/
	 CMSession		 sessionData;											/*		ptr to current container session*/
	 CMValue  		 value;														/* 		pointing value to operate upon	*/
	 CMBoolean		 isReverseEndian;									/*    whether to use reverse endian		*/
	 unsigned long position;												/* 		current position within value		*/
	 unsigned long size;														/*		value size											*/
	 char 				 typeName[1];											/*		start of value's type name			*/
 };
 typedef struct RefCon RefCon, *RefConPtr;																							/*

 The current session data pointer, which we need to allocate the refCon is kept just as	a
 convenience in case we need it again.  At the very least it will be needed to free the
 refCon.

 Since we are simulating "I/O" with these handlers, we need to keep track of the a current
 "stream" position within the value. The position and size fields in the struct serves this
 purpose.  The typeName of the value is used as an error insert if we report an error.
*/

static CMRefCon open_Handler(CMRefCon attributes, CMOpenMode mode)
{
	CMValue			value = (CMValue)attributes;
	CMContainer	container;
	CMSession		sessionData;
	CMType			type;
	RefConPtr 	p;
	char				*typeName;
	
	/* Get the type name for the value and get current session pointer...									*/
	
	CMGetValueInfo(value, &container, NULL, NULL, &type, NULL);
	sessionData = CMGetSession(container);
	
	typeName = (char *)CMGetGlobalName(type);			/* get type name for errors 						*/
	if (typeName == NULL) {												/* no global name?											*/
		CMError(sessionData, "Unable to get type name for pointing value in container \"^0\"!", CMReturnContainerName(container));
		return (NULL);			
	}

	/* Allocate and fill in the refCon we will return to the Container Manager...					*/
	
	p = (RefConPtr)CMMalloc(sizeof(RefCon) + strlen(typeName), sessionData);
	if (p == NULL) {															/* allocation failed!										*/
		CMError(sessionData, "Allocation of \"refCon\" failed for target (type \"^0\", container \"^1\")!", typeName, CMReturnContainerName(container));
		return (NULL);
	}
	
	p->sessionData = sessionData;										/* save the current session ptr				*/
	p->value = value;																/* value we will do operations on			*/
	p->isReverseEndian = 0;													/* assume same endian-ness						*/
	p->size	 = (unsigned long)CMGetValueSize(value);/* value's size												*/
	strcpy(p->typeName, typeName);									/* copy in the global type name				*/

	/* Check the open mode and do appropriate checks on size. Set appropriate position...	*/

	if (strcmp((char *)mode, "wb+") == 0) {				/* writing...													 	*/
		if (p->size != 0) {													/* size must be zero										*/
			CMError(sessionData, "Cannot create target container for a pointing value (type \"^0\") that already has data!", p->typeName);
			CMFree(p, sessionData);
			return (NULL);
		}
		p->position = 0;														/* position to 1st free byte						*/
	} else if (strcmp((char *)mode, "rb") == 0) { /* reading...							 							*/
		if (p->size == 0) {													/* size must be non-zero								*/
			CMError(sessionData, "Cannot read target container for a pointing value (type \"^0\") that doesn't have data!", p->typeName);
			CMFree(p, sessionData);
			return (NULL);
		}
		p->position = 0;														/* position to 1st byte to read					*/
	} else if (strcmp((char *)mode, "rb+") == 0)  /* converting or updating...						*/
		p->position = p->size;											/* size can be anything(position at end)*/
	else {																				/* bad mode...													*/
		CMError(sessionData, "Invalid target container open mode (\"^0\", pointing value type \"^1\")!", (char *)mode, p->typeName);
		CMFree(p, sessionData);
		return (NULL);
	}
	
	return ((CMRefCon)p);
}


/*------------------------------------------*
 | close_Handler - close the container file |
 *------------------------------------------*
 
 Called when CMCloseContainer() is called.  The space allocated by the refCon is freed.
*/

static void close_Handler(CMRefCon refCon)
{
	RefConPtr p = (RefConPtr)refCon;
	CMSession sessionData = p->sessionData;
	
	CMFree(p, sessionData);																/* bye, bye, refcon...					*/
}


/*-------------------------------------------------------------------*
 | seek_Handler - position the I/O to the specified container offset |
 *-------------------------------------------------------------------*
 
 This allows the API random access to the target container. The current position value is
 set according to the mode and posOff.  0 is returned for success and non-zero for failure.
 For these handlers there is no failure (famous last words).
 
 The mode determines how to use posOff to set the position. It is defined as one of the
 following seek modes (defined in CM_API_Types.h which is included by CM_API.h):
 
	kCMSeekSet					Set the position to the value offset specified by posOff.  The posOff
											is the character offset from the beginning of the value data.
	
	kCMSeekCurrent			Set the stream posOff characters from the current position.  posOff
											can be postive or negative here.
											
	kCMSeekEnd					Set the position posOff characters from the current end of the value.
											posOff here is only zero or negative.  We never assume or allow
											undefined contents to be generated in a container. This makes life
											easier here since we don't have to check for this.
 
 Note, here all we do is set our position saved in the refCon.  There is nothing else to
 do.  The position is used by other handlers such as the reads and writes.
*/

static CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode)
{
	RefConPtr p = (RefConPtr)refCon;
	
	if (mode == kCMSeekSet)													/* kCMSeekSet...											*/
		p->position = (unsigned long)posOff;					/* ...just use posOff as is						*/
	else if (mode == kCMSeekEnd)										/* kCMSeekEnd...											*/
		p->position = (long)p->size + posOff;					/* ...posOff is 0 or negative here		*/
	else 																						/* kCMSeekCurrent... 									*/
		p->position = (long)p->position + posOff;			/* ...relative positioning						*/
		
	return (0);
}


/*-----------------------------------------------*
 | tell_Handler - return current stream position |
 *-----------------------------------------------*
 
 Returns the current position as characters from the start of the target container.
 The position is set by other handlers like the seek (above) and the reads and writes.
*/

static CMSize tell_Handler(CMRefCon refCon)
{
	return ((CMSize)((RefConPtr)refCon)->position);
}


/*------------------------------------------------------------------*
 | read_Handler - read information from the container into a buffer |
 *------------------------------------------------------------------*
 
 Reads bytes from the current position with the target container value into the specified
 buffer.  Up to theCount elements of the specified size are read.  The actual number of
 items read, which may be less than theCount (e.g., if the end-of-container is read) or
 even 0 is returned as the function result.
*/

static CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	RefConPtr			p = (RefConPtr)refCon;
	unsigned long amountRead;
	
	amountRead = (unsigned long)CMReadValueData(p->value, 				/* read value...				*/
															 								buffer,
															 								p->position, 
															 								(CMSize)((unsigned long)elementSize * (unsigned long)theCount));
	
	p->position += amountRead;										/* update position by amount read				*/
	amountRead  /= (unsigned long)elementSize;		/* convert back to elementSize units		*/
		
	return ((CMSize)amountRead);		
}


/*------------------------------------------------------------------*
 | containerSize_Handler - return the current size of the container |
 *------------------------------------------------------------------*
 
 Returns the 1-relative size of the value (i.e., target container).
*/

static CMSize containerSize_Handler(CMRefCon refCon)
{
	RefConPtr p = (RefConPtr)refCon;
	
	p->position = p->size;																/* moral equivalent of seek/tell*/
	
	return ((CMSize)p->size);
}


/*---------------------------------------------------------------*
 | readLabel_Handler - read in and extract the container's label |
 *---------------------------------------------------------------*
 
 Finds the label in the target container value and returns all the information it contains.
 In order to make target container values have the same format as the stream containers
 which the ExampleHandlers.c handlers maintain, we will put and look for the label at the
 end of the target container value.
 
 As defined by the API documentation, a container label has the following layout:

			+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			|      Magic Bytes      |flags|bufsz|major|minor|TOC offset |  TOC size |
			+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
									8              2     2     2     2        4           4

 For C compilers that generate code for machines where char is 1 byte, short 2, and long
 4, we may use the following struct to define the label format:													*/

 struct ContainerLabelFmt {								/* Layout of a container label:								*/
	 unsigned char  magicBytes[8];					/* 8 bytes: the magic byte identifier					*/
	 unsigned short flags;									/* 2				the label flags										*/
	 unsigned short bufSize;								/* 2				TOC buffer size / 1024						*/
	 unsigned short majorVersion;						/* 2				major format version number				*/
	 unsigned short minorVersion;						/* 2				minor format version number				*/
	 unsigned long	tocOffset;							/* 4				offset to start of TOC						*/
	 unsigned long	tocSize;								/* 4				total byte size of the TOC				*/
 };
 typedef struct ContainerLabelFmt ContainerLabelFmt, *ContainerLabelFmtPtr;							/*
 
                                    W A R N I N G !!
 
 If your C compiler and/or hardware cannot use the above struct, then this handler will
 have to be recoded accordingly.  Indeed, it is for this very reason this IS a handler. It
 shifts the portability problem outside the API.
 
 This handler looks for the label and decodes it according to the above struct.  So this
 is only for implementations with the above C size definitions.  Our own handler routines
 defined for containers are used to do the positioning and read.
 
 No editoralizing is done on the extracted information.  It is simply passed back to the
 API where the validation is done.  
*/

static void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
														 	CMContainerFlags *flags, CM_USHORT *bufSize,
														 	CM_USHORT *majorVersion, CM_USHORT *minorVersion,
										 				 	CMSize *tocOffset, CMSize *tocSize)
{
	RefConPtr 				p = (RefConPtr)refCon;
	unsigned long			labelSize;
	ContainerLabelFmt theLabel;

	/* Seek to the end of the label at the end of the value and read it...								*/
	
	seek_Handler(refCon, -(long)sizeof(ContainerLabelFmt), kCMSeekEnd);
	labelSize = (unsigned long)read_Handler(refCon, (CMPtr)&theLabel,
																					(CMSize)sizeof(unsigned char),
																					(CMCount)sizeof(ContainerLabelFmt));
	
	if (labelSize != sizeof(ContainerLabelFmt)) {		/* must have read it all!							*/
		CMError(p->sessionData, "Target container (for pointing value type \"^0\") label could not be read!", p->typeName);
		return;
	}
	
	/* Return all the label info...																												*/
	
	memcpy(magicByteSequence, theLabel.magicBytes, 8);
#if kCMDefaultEndian
	/* little endian machine */
	if ((theLabel.flags & kCMLittleEndianTwin) == 0) {
#else
	/* big endian machine */
	if (theLabel.flags & kCMLittleEndianTwin) {
#endif
		p->isReverseEndian = 1;
		ODFlipMove(&theLabel.flags, flags, sizeof(CM_USHORT));
		ODFlipMove(&theLabel.bufSize, bufSize, sizeof(CM_USHORT));
		ODFlipMove(&theLabel.majorVersion, majorVersion, sizeof(CM_USHORT));
		ODFlipMove(&theLabel.minorVersion, minorVersion, sizeof(CM_USHORT));
		ODFlipMove(&theLabel.tocOffset, tocOffset, sizeof(CMSize));
		ODFlipMove(&theLabel.tocSize, tocSize, sizeof(CMSize));
	}
	else {
		*flags = (CMContainerFlags)theLabel.flags;
		*bufSize = (CM_USHORT)theLabel.bufSize;
		*majorVersion = (CM_USHORT)theLabel.majorVersion;
		*minorVersion = (CM_USHORT)theLabel.minorVersion;
		*tocOffset = (CMSize)theLabel.tocOffset;
		*tocSize = (CMSize)theLabel.tocSize;
	}
}


/*---------------------------------------------------------------------------*
 | returnContainerName_Handler - return string that identifies the container |
 *---------------------------------------------------------------------------*
 
 Creates an appropriate string to identify the target container for error inserts.
*/

static CM_UCHAR *returnContainerName_Handler(CMRefCon refCon)
{
	return ((CM_UCHAR *)((RefConPtr)refCon)->typeName);
}


/*----------------------------------------------------------------------------*
 | extractData_Handler - extract private Container Manager data from a buffer |
 *----------------------------------------------------------------------------*
 
 This handler is used to extract "internal" Container Manager data previously written to
 the container (e.g., updating data and CMReferences).  1, 2, or 4 bytes (size 8-bit byte)
 "chunks" of data are expected to be copied from a buffer to the data.  Pointers to the
 data and the buffer are passed in.  The buffer can always be assumed large enough to
 supply all the requested data.  The pointer to the data can be assumed to point to a
 CM_UCHAR if size is 1, CM_USHORT if size is 2, and CM_ULONG is size if 4.

 The 1, 2, or 4 bytes are, of course, formatted within the CM_UCHAR, CM_USHORT, or CM_ULONG
 as a function of the architecture.  These may be a different size than what is expected
 to be written to the container.  Indeed, it is the potential difference between the
 architecture from the data layout in the  container that this handler must be provided.

 The information is stored in the container in a layout private to the Container Manager.
 For example, it is used to extract updating information and the fields of the TOC. It does
 repeated calls to this handler to extract the information it needs from a buffer
 that it loads using the read_Handler.
  
 In this example CM_UCHAR, CM_USHORT and CM_ULONG map directly into the container format
 1, 2, and 4 byte entities.  Hence extracting the data is straight-forward.
*/

static void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer,
															 	CMSize size, CMPrivateData data)
{
	RefConPtr p = (RefConPtr)refCon;
	/* Adkins -- changed from Boolean to CMBoolean */
	CMBoolean	reverseEndian = p->isReverseEndian;
		
	if ((CM_LONG)size < 0) {	/* this means it is endian-ness netural	*/
		size = -(CM_LONG)size;
		reverseEndian = 0;
	}
	
	if (reverseEndian)
		ODFlipMove(buffer, data, (size_t)size);						/* copy the buffer to the data		*/
	else
		memcpy(data, buffer, (size_t)size);									/* copy the buffer to the data		*/
}


/*--------------------------------------------------------------------------*
 | formatData_Handler - format private Container Manager data into a buffer |
 *--------------------------------------------------------------------------*

 This handler is used to format "internal" Container Manager data to be written to
 the container (e.g., updating data and CMReferences).  1, 2, or 4 bytes (size 8-bit byte)
 "chunks" of data are expected to be copied from the data to a buffer.  Pointers to the
 data and the buffer are passed in.  The buffer can always be assumed large enough to hold
 the data.  The pointer to the data can be assumed to point to a CM_UCHAR if size is 1,
 CM_USHORT if size is 2, and CM_ULONG is size if 4.
 
 The 1, 2, or 4 bytes are, of course, stored in the CM_UCHAR, CM_USHORT, or CM_ULONG as a
 function of the architecture.  These may be a different size than what is expected to be
 written to the container.  Indeed, it is the potential difference between the architecture
 from the data layout in the container that this handler must be provided.

 The information is stored in the container in a layout PRIVATE to the Container Manager.
 For example, it is used to format the fields of the TOC.  It does repeated calls to this
 handler to format the information it needs into a buffer that is eventually written using
 the write_Handler.
  
 In this example CM_UCHAR, CM_USHORT and CM_ULONG directly map into the container format
 1, 2, and 4 byte entities.  Hence the formatting is straight-forward.
*/

static void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer,
															 CMSize size, CMPrivateData data)
{
	RefConPtr p = (RefConPtr)refCon;
	/* Adkins -- changed from Boolean to CMBoolean */
	CMBoolean	reverseEndian = p->isReverseEndian;
	
	if ((CM_LONG)size < 0) {	/* this means it is endian-ness netural	*/
		size = -(CM_LONG)size;
		reverseEndian = 0;
	}
	
	if (reverseEndian)
		ODFlipMove(data, buffer, (size_t)size);						/* copy the data top the buffer		*/
	else
		memcpy(buffer, data, (size_t)size);									/* copy the data top the buffer		*/
}
