/* @(#)Z 1.8 com/src/cm/CMCntOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:43 (96/10/29 09:16:18) */
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
	File:		   CMCntOps.c   

	Contains:	Container Manager Container Operations

	Owned by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<22>	10/22/95	EL		1294635: Need to unprotect value before we
									can change endian-ness of ref data.
		<21>	 9/26/95	EL		1286040: The previous error is due to put
													unsaved global name to free space, now it
													is fixed by not changing offset of global
													names if it is not on disk.
		<20>	 9/14/95	EL		1238410: Disable garbage collection of prop
													and type name
		<19>	 8/16/95	EL		1275835: Make sure the replacement TOC does
													not overwrite previous written TOC.
		<18>	 5/25/95	jpa		Changed last param of CMOpenNewContainer
													for ANSI/CFM68K compatibility. [1241078]
		<17>	 5/11/95	DM		#1214394, #1245829: stop compiler warnings
		<16>	 5/11/95	EL		1245113: When writing TOC in container
													merging, make sure it would not overlap
													both container.
		<15>	  5/2/95	EL		1238952: Do garbage collection before
													returning free space to top level
													container.
		<14>	 4/25/95	EL		1242418 Flush embedded container before
													returning space to top level container.
													1242376: Separate crash proof for embedded
													and top level container.
		<13>	 3/31/95	EL		1234685: Add CMContainsValidLabel. CMAbort
													would truncate file to original size in the
													case of open for writing.
		<12>	 3/24/95	EL		1209355: add CMTakeSnapShot.
		<11>	 3/10/95	EL		1227122: fix bug that label is written on
													wrong offset when file is truncated.
		<10>	  3/6/95	EL		1182275: Remove unused property name in the
													same container.
		 <9>	 2/27/95	EL		1222909: Garbage collection of properties
													fails with draft creation.
		 <8>	 1/31/95	EL		1182275: Take care of case where free space
													spans two containers to be merged.
		 <7>	 12/9/94	EL		#1182275 Reuse free list in merged target
													container. Garbage collect unused property
													and type object. Delete property in merged
													private TOC. Do not merge if container is
													read only. Optionally do not maintain
													continue flag.
		 <6>	 9/30/94	EL		#1182275 Fix bug where merged TOC is added
													to free list twice.
		 <5>	 9/22/94	EL		#1182275 Free space list also need
													adjustment when closing the gap that are
													return to parent.
		 <4>	 9/15/94	EL		#1182275 When merge with drafts, return
													space to parent and adjust the space in the
													past history.
		 <3>	 8/31/94	EL		#1182275 Merge leaves some space both in a
													value and the free list.
		 <2>	 8/26/94	EL		#1182275 Allow merging of two updating
													containers.
		 <6>	  5/5/94	EL		Allow only reading up to same minor
													version, but output only the minimum minor
													version required.
		 <5>	 4/13/94	EL		Allow reading of different minor version.
		 <4>	  4/6/94	EL		Free the toc in target container when it is
													merged. #1150214
		 <3>	 3/31/94	EL		Add CMMergeContainer call. #1150214
		 <2>	 3/17/94	EL		Radar 1149983 delete gargabe objects. Also
													supports truncation of file in abort
													container.
		 <1>	  2/3/94	EL		first checked in
		<10>	  2/1/94	EL		Null target container pointer when closing
													container to make memory use safe for
													multitasking.
		 <9>	 1/21/94	EL		In CMAbortContainer, get the session data
													before it is closed.
		 <8>	 1/19/94	EL		For extra safety, free space is not reused
													immediately but will be used next time the
													container is opened. Add optional automatic
													generation of generation number when
													opening new container for update by append.
		 <7>	  1/6/94	EL		Take over the free list from the target
													container so that it can be reused.
		 <6>	 12/2/93	EL		Fix close embedded container bug (see <4>)
													in another way so that embedded containers
													in target containers will be closed with
													the updating container.
		 <5>	11/23/93	EL		When updating save TOC data so that TOC
													don't need to be read twice.
		 <4>	11/10/93	EL		Fix bug that embeddedContainer cannot be
													closed. EmbeddedContainer should be added
													to container's embedded list and not the
													update container's list.
		 <3>	10/29/93	EL		Add debug code to dump out content of
													container.
		 <2>	10/21/93	EL		Update the target in two passes with new
													entries added in the second pass so that it
													would not be deleted by the delete updates.

	To Do:
		writing label for updating container in the same way as non-updating container.
	In Progress:
		
*/

#define DebugEmbedding 0
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                          <<<    CMCntOps.c    >>>                         |
 |                                                                           |
 |                  Container Manager Container Operations                   |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/02/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1995                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 Containers are created, accessed, and closed via the API routines in this file.  
 Specification of containers is in terms of container refNums.  The routines in this file
 are responsible for opening containers (and generating the associated refNums), closing
 containers, and getting some general info associated with a container.
 
 Containers may be defined as "embedded", i.e., a container within a container.  Containers
 may also be "appeneded" or descrete fo updating other containers.
 
 to do: Doug Royer thinks there is a problem with the
	if (targetContainer == NULL) {
		UndoOpen();
		return (NULL);
	}
 
*/

 
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API__
#include "CMAPI.h"
#endif
#ifndef __CM_API_DEBUG__
#include "CMAPIDbg.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
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
#ifndef __TOCIO__
#include "TOCIO.h"
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __BUFFEREDIO__
#include "BufferIO.h"  
#endif
#ifndef __UPDATING__
#include "Update.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
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

/* This is used ONLY for debugging embedded containers...																*/

#if DebugEmbedding
static short level = 0;
#endif


																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment CMContainerOps
#endif


/*--------------------------------------------------------------------*
 | validateMagicBytes - validate the magic bytes of a container label |
 *--------------------------------------------------------------------*
 
 This is used by both validateContainerLabel and CMContainsValidLabel to verify
 that there is the valid Bento magic bytes at the end. Currently CMContainsValidLabel
 only checks for the magic bytes. Later we are going to check for the version etc and
 then we will change this to validateLabel and move code from validateContainerLabel.
 However that has to wait until we handle endian-ness in CMContainsValidLabel.
	
*/

static CMBoolean CM_NEAR validateMagicBytes(ContainerPtr container, CM_UCHAR *theMagicBytes)
{
	/* Do we have the valid magic bytes ?																									*/
	
	return (memcmp(theMagicBytes, container->magicBytes, strlen(MagicByteSequence)) == 0);
}

/*-----------------------------------------------------------------*
 | validateContainerLabel - read in and validate a container label |
 *-----------------------------------------------------------------*
 
 When a container is to be opened for input (CMOpenContainer()), the container label must
 be read in order to get the info container there.  The prime piece of info, among other
 things is the TOC offset and size so we can load in the TOC.
 
 This function returns true and returns the label info if we're happy with the label.  If
 anything goes wrong (e.g., wrong magic bytes, wrong format, or whatever), false is 
 returned.
 
 Of course, the returns are somewhat academic.  That is because when we report the errors
 through the error handler, it should NOT return.  We never assume that anywhere in the
 Container Manager.  Hence we always return something for errors. It just a safety
 precaution.
*/

static CMBoolean CM_NEAR validateContainerLabel(ContainerPtr container,
														 				 						CMContainerFlags *containerFlags,
														 				 						CM_USHORT *majorVersion,
														 				 						CM_USHORT *minorVersion,
																		 						CM_ULONG *tocBufSize, CM_ULONG *tocOffset,
																		 						CM_ULONG *tocSize)
{
	CM_USHORT			majorVer, minorVer, bufSize;
	CM_UCHAR			magicBytes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	/* Tell handler to load give us the info from the label. It doesn't return (or better	*/
	/* not return if this operation fails!																								*/
	
	CMreadLabel(container, magicBytes, containerFlags, &bufSize, &majorVer, &minorVer, tocOffset, tocSize);

	/* Validate what we can from the label info. Error out anything we're unhappy with!		*/
	
	if (!validateMagicBytes(container, magicBytes)) {
		magicBytes[8] = '\0';
		ERROR3(CM_err_BadMagicBytes, CONTAINERNAME, container->magicBytes, magicBytes);
		return (false);
	}
	
	/* The major version increases with incompatible TOC format changes.  The minor is		*/
	/* reset to 0 when the major changes.  The minor is incremented for compatible format	*/
	/* changes.   However change in minor version indicates there are some data with a new*/
	/* format. Thus if the container has a minor version greater than what is supported		*/
	/* by this version of the code, we still assume we have an incompatible format.				*/
	
	/* Note, for upward compatibility the "old" format (1) is let through here since 			*/
	/* both formats are supported in this version of the code. REMOVE THIS LATER!		*/
	
	if (majorVer != MajorVersion || minorVer > MinorVersion) {
		if (majorVer > 1) {
			char i2[10], i3[10], i4[10], i5[10];
			cmltostr(MajorVersion, 1, false, i2);
			cmltostr(MinorVersion, 1, false, i3);
			cmltostr(majorVer, 1, false, i4);
			cmltostr(minorVer, 1, false, i5);
			ERROR5(CM_err_BadVersion, CONTAINERNAME, i2, i3, i4, i5);
			return (false);
		}
	}
	
	*majorVersion = majorVer;
	*minorVersion	= minorVer;
	
	/* Set the TOC buffer size to be used to read this container...												*/
	
	if (majorVer > 1) 
		*tocBufSize = ((CM_ULONG)bufSize) * kCMBufSizeUnits;

	return (true);
}


/*-----------------------------------------------------------------------------*
 | buildHandlerVector - get a handler address for a specific handler operation |
 *-----------------------------------------------------------------------------*
 
 This routine is called repeatedly for each handler operation type.  The operationType is
 passed to the metahandler to get the corresponding handler  routine address.  If there is
 one, it is passed back in the handlerAddress parameter and false (that's right, false) is
 returned which indicates success.  A true return here indicates failure.
 
 The missing parameter (carful how you read that) points to a string buffer pointer or a
 pointer to NULL.  It indicates whether the handler for the operationType is a required
 handler or optional. The NULL means it's optional.  If not NULL it means the handler is
 required.  If the metahandler returns NULL, it means there is no handler for the
 operationType.  Then depending on missing, we have a failure (true) or success (false).
 
 The reason missing is a pointer to the caller's pointer is that we accumulate missing
 operationType's in the string pointed to by missing, but we do the sting allocation here
 on the first occurrence of a missing handler.  The caller has to own the buffer.  Hence
 the extra indirection.  After the entire vector is build, the caller checks for any
 failures.  At that point the string is used.
 
 Note that there are three states for *missing; (1) 0xFFFFFFFF if there are no missing
 routines yet and the string is not yet allocated, (2) NULL if we couldn't allocate the
 string buffer, and (3) non-null (and not 0xFFFFFFFF) indicating we had a previous failure
 and we allocated the buffer.
 
 The reason we use a dynamic buffer at all is that the insert can get potentially large if
 for some reason all the operationType's we're missing!
*/

static CMBoolean CM_NEAR buildHandlerVector(const ContainerPtr container,
																						CMHandlerAddr *handlerAddress, CM_CHAR *operationType,
																						CM_CHAR **missing)
{
	CMBoolean failure;
	
	/* Call the metahandler to get the address of the handler routine for the designated	*/
	/* operationType.																																			*/
	
	*handlerAddress = (CMHandlerAddr)(*container->metaHandlerProc->metaHandler)(NULL, (CMGlobalName)operationType);
	
	/* If the metahandler returns NULL, it means that no handler routine is suppled for 	*/
	/* the operationType.  That may or may not be ok.  The way we tell is to look at 			*/
	/* missing.  If it's NULL, it's ok to not have that particular handler.  If it isn't,	*/
	/* then we concat the operationType to the string pointed to by missing.  After all		*/
	/* the handlers have been determined, the missing string will have a list of all			*/
	/* required handlers that were missing.  It can then be used as an error insert.			*/
	
	if (*handlerAddress || *missing == NULL)		/* if it's ok to not have the hander...		*/
		failure = false;													/* ...consider this a success							*/
	else if (*missing == NULL)									/* if previous allocation failure...			*/
		failure = true;														/* ...everything fails from then on!			*/
	else {																			/* if it's not ok and we need this handler*/
		if (*missing == (CM_CHAR *)0xFFFFFFFF)				/* if this is the first missing routine...*/
			if ((*missing = (CM_CHAR *)CMmalloc(1024)) == NULL){/* allocate the insert buffer		*/
				ERROR1(CM_err_NoMissingBuffer, TYPENAME);			/* huh?																*/
				return (true);												/* failure will propagate									*/
			} else
				**missing = 0;												/* string of missing routines							*/

		failure = true;														/* ...we're doomed!												*/
		if (**missing) strcat((CM_CHAR *)*missing, ", ");/* concat operationType to string			*/
		strcat((CM_CHAR *)*missing, (CM_CHAR *)operationType);
	}
	
	return (failure);														/* return success or failure (so to speak)*/
}


/*---------------------------------------------------------------------------------------*
 | registerStdTypesAndProperties - register all predefined standard types and properties |
 *---------------------------------------------------------------------------------------*
 
 This routine acts sort of like CMRegisterType() and CMRegisterProperty in that it 
 registers all predefined standard types and properties in the TOC.  This is done to allow
 the API user the ability to get at these predefined objects using the standard API "CM..."
 interfaces.  Although these objects are truely in the TOC, we never write them out to the
 container. So reregistering them is necessary every time we open a container.  It is
 openContainer() below that is this routines only caller.
 
 True is returned if everything goes ok.  False otherwise.  An error will have been
 reported if false is returned.
*/

static CMBoolean CM_NEAR registerStdTypesAndProperties(ContainerPtr container)
{
	struct PredefTbl {												/* Predefined type/property table layout:		*/
		CM_CHAR				 *globalName;							/* 		ptr to this object's global name			*/
		CM_ULONG			 objectID;								/*		TOC ID to assign to this object				*/
		CM_USHORT			 objectFlags;							/* 		the objects type/property flags				*/
	};
	typedef struct PredefTbl PredefTbl, *PredefTblPtr;
	
	/* Table of predefined types and properties...																				*/
	
	PredefTbl predefines[17] = {							/* see PredefTbl above for layout meanings	*/
		{CMTOCSeedGlobalName, 				CM_StdObjID_TOC_Seed, 				PropertyObject},	/*  1 */
		{CMTOCMinSeedGlobalName, 			CM_StdObjID_TOC_MinSeed, 			PropertyObject},	/*  2 */
		{CMTOCObjectGlobalName,				CM_StdObjID_TOC_Object,				PropertyObject},	/*  3 */
		{CMTOCContainerGlobalName,		CM_StdObjID_TOC_Container,		PropertyObject},	/*  4 */
		{CMTOCDeletedGlobalName, 			CM_StdObjID_TOC_Deleted, 			PropertyObject},	/*  5 */
		{CMTOCTotalFreeGlobalName, 		CM_StdObjID_TOC_Free, 				PropertyObject},	/*  6 */
		{CMTOCTargetGlobalName, 			CM_StdObjID_TOC_Target, 			PropertyObject},	/*  7 */
		{CMTOCNewValuesTOCGlobalName,	CM_StdObjID_TOC_NewValuesTOC,	PropertyObject},	/*  8 */
		{CMTOCDeleteListGlobalName,		CM_StdObjID_TOC_DeleteList,		PropertyObject},	/*  9 */
		{CMTOCValueTypeGlobalName,		CM_StdObjID_TOC_Type,					TypeObject		},	/* 10 */
		{CMDynamicValueGlobalName,		CM_StdObjID_DynamicValues,		PropertyObject},	/* 11 */
		{CMBaseTypesGlobalName,				CM_StdObjID_BaseTypes,				PropertyObject},	/* 12 */
		{CMBaseTypeListGlobalName,		CM_StdObjID_BaseTypeList,			TypeObject		},	/* 13 */
		{CMTargetContainerName,				CM_StdObjID_TargetContainer,	TypeObject		},	/* 14 */
		{CMValueUpdatesGlobalName,		CM_StdObjID_ValueUpdates,			PropertyObject},	/* 15 */
		{CMUpdatesDataGlobalName,			CM_StdObjID_UpdatesData,			TypeObject    },	/* 16 */
		{"",													0,														0							}	 	/* 17 */
	};
	
	/* Note, the predefines should be declared as "static" if you don't want that table		*/
	/* to be created at load time.  It is, however, not static for two reasons.  First, 	*/
	/* this routine is only called once per container setup (by openContainer()) so it is	*/
	/* not that expensive.  Second, and most important, we don't want any static global		*/
	/* data in the Container Manager!  Enough said.																				*/
	
	/* Oh, by the way.  I know you don't need the extra braces in the above init list.		*/
	/* But my MPW C compiler choked without them when I removed static. Besides, now that	*/
	/* there in, they look sort of "cute".  I think I'll keep 'em. 												*/
	/* [Don't mind me.  I in another one of those weird moods today.]											*/
	
	PredefTblPtr 	 p;
	TOCObjectPtr 	 theObject;
	CM_USHORT			 objectFlags;
	CM_ULONG			 propertyID;
	CMBoolean			 allOk = true;
	
	/* Loop through the table to register each type or property...												*/
	
	for (p = predefines; p->objectID != 0; ++p) {
		if (p->objectFlags & TypeObject) {									/* if type...										*/
			propertyID = CM_StdObjID_GlobalTypeName;					/* use std type property ID			*/
			objectFlags = (TypeObject | ProtectedObject);			/* these will be the flags			*/
		} else {																						/* if property...								*/
			propertyID = CM_StdObjID_GlobalPropName;					/* use std property property ID	*/
			objectFlags = (PropertyObject | ProtectedObject);	/* these will be the flags			*/
		}
		
		/* Register (define) the property or type...																				*/
		
		theObject = cmDefineGlobalNameObject(container, p->objectID, propertyID,
																				 CM_StdObjID_7BitASCII, 
																				 (CM_UCHAR *)p->globalName,
																				 container->generation, 0, objectFlags);
		if (allOk && theObject == NULL) allOk = false;
	} /* for */
	
	return (allOk);
}


/*---------------------------------------------------*
 | openContainer - common "open a container" routine |
 *---------------------------------------------------*
 
 This is the common open routine called by both CMOpenContainer() and CMOpenNewContainer()
 to do most of common work of creating a container control block.  For input, the 
 container is opened, the label read and TOC loaded.  For writing the container is simply
 opened.
 
 No matter what the mode, all the other container control block fields are initialized
 as appropriate.  The handler addresses are set up.  All predefined properties and types
 registered.
*/

static ContainerPtr CM_NEAR openContainer(CMRefCon attributes,
																					CMconst_CMGlobalName typeName, 
																					CMContainerUseMode useFlags,
																					CMGeneration generation,
																					CMContainerFlags containerFlags,
																					SessionGlobalDataPtr sessionData)
{
	CMBoolean				 		 failure;
	char								 *missing, *optional;
	ContainerPtr 	 			 container, previouslyOpenedContainer;
	CM_ULONG			 			 tocOffset, tocSize, valueSize;
	TOCValueHdrPtr 			 theValueHdr;
	EmbeddedContainerPtr embedded;

	/* The following macro is defined for convenience! Once we create the TOC, container, */
	/* and open it we still have additional work to do in here.  Some of those things can	*/
	/* go "wrong", resulting in error reports.  While such reports are not supposed to 		*/
	/* return, perform error recovery and return a NULL.  This can	                        */
	/* happen in a few places.  Hence the following macro is used to do the cleanup:			*/
	
	#define UndoOpen()  CMfclose(container);										/* close the container 		*/\
											cmFreeAllIOBuffers(container);					/* free I/O buffers				*/\
											cmFreeAllGlobalNames(&container->globalNameTable); /* frre globals*/\
											cmFreeTOC(container, &container->toc);	/* free TOC data					*/\
											CMfree(container);											/* free the container			*/

	/* Create a new container control block and set the session data pointer as fast as 	*/
	/* we can.  Because until we do, we can't use any of our standard macros like error		*/
	/* reporting and memory management.																										*/
	
	if ((container = (ContainerPtr)SessionMalloc(sizeof(Container))) == NULL) {
		SessionERROR(CM_err_NoContainer);
		return (NULL);
	}
	
	container->sessionData = sessionData;						/* whew!															*/
	
	container->tocFullyReadIn = false;							/* safety switch on the TOC status		*/
	container->handler.cmreturnContainerName = NULL;/* typeName for errors until defined	*/
	
	/* Generally, all accesses to the container are through refNums we return to the user.*/
	/* When such refNums are passed to the Container Manager, various validation checks		*/
	/* are done in an "attempt" to protect ourselves from using garbage.  A "deadly" bomb	*/
	/* can occur by using a garbage refNum to get at a container control block, and then 	*/
	/* using that to get at the handler addresses, and then calling one of the handlers.	*/
	/* The effect will be to jump to some arbitrary place in memory!  Such problems can 	*/
	/* be difficult to find.  So, to try to protect ourselves from this, a pointer is 		*/
	/* placed in the container control block pointing to that container control block. We	*/
	/* then validate the refNum's container pointer by checking it against this pointer.	*/
	/* If it matches we're "happy".  If not, we assume we don't have a garbage refNum.		*/
	/* This is NOT perfect.  But it doesn't cost too much to do it.  Given the "proper"		*/
	/* garbage, the pointer check will itself will blow.  But a "invalid (read) access"		*/
	/* bombout is  preferable to jumping into oblivion.																		*/
	
	container->thisContainer = container;						/* what's left to say!								*/
	
	/* Get the handler "meta-proc" pointer which we will use to build up the handler 			*/
	/* service routine vector.																														*/
	
	if ((container->metaHandlerProc = cmLookupMetaHandler((CM_UCHAR *)typeName, sessionData)) == NULL) {
		ERROR1(CM_err_UndefMetaHandler, typeName);
		CMfree(container);
		return (NULL);
	}
	
	if (!SessionSuccess) {													/* if allocation error...			*/
		ERROR1(CM_err_HandlerError, typeName);				/* ...yell														*/
		CMfree(container);
		return (NULL);
	}
	
	if (container->metaHandlerProc->metaHandler == NULL) {	/* If NULL proc ptr (?)...		*/
		ERROR1(CM_err_NullMetaPtr, typeName);					/* ...yell														*/
		CMfree(container);
		return (NULL);
	}
	
	/* Build the handler vector.  Any missing routines will result in failure with the		*/
	/* corresponding interface type names accumulated in the "missing" string which we		*/
	/* use as an error insert.	If NULL is passed as the last argument then we don't care	*/
	/* if the handler is missing.  That's because we either don't use it or don't know if	*/
	/* we will ever use it. To help in documenting these calls we will use the following 	*/
	/* macros.  Their names are self-explainatory.																				*/
	
	missing = (CM_CHAR *)0xFFFFFFFF;										/* allocate on first use							*/
	optional = NULL;
	
	#define Required						&missing
	#define RequiredForWriting	((useFlags & (kCMWriting | kCMReuseFreeSpace))!=0 ? &missing : &optional)
	#define RequiredForReading	((useFlags & (kCMWriting | kCMReuseFreeSpace))==0 ? &missing : &optional)
	#define RequiredForUpdating	((useFlags & (kCMUpdateTarget | kCMUpdateByAppend))!=0 ? &missing : &optional)
	#define Optional						&optional
	
	container->handler.cmreturnContainerName = NULL;/* optional, but make sure we know it!*/
	
	failure  = buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmfopen,			CMOpenOpType,	  		Required);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmfclose, 		CMCloseOpType,  		Required);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmfflush,	  	CMFlushOpType,  		Optional);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmfseek,			CMSeekOpType,	  		Required);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmftell,			CMTellOpType,	  		Required);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmfread,			CMReadOpType,	  		RequiredForReading);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmfwrite,	  	CMWriteOpType,  		RequiredForWriting);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmfeof,   		CMEofOpType,	  		Optional);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmftrunc,   	CMTruncOpType,	  	Optional);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmgetContainerSize, CMSizeOpType, Required);	
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmreadLabel, 	CMReadLblOpType,  	RequiredForReading);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmwriteLabel,	CMWriteLblOpType, 	RequiredForWriting);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmreturnParentValue, CMParentOpType, Optional);
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmreturnContainerName, CMContainerOpName, Optional);	
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmreturnTargetType, CMTargetTypeOpType, Optional);	
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmextractData,CMExtractDataOpType,Required);	
	failure |= buildHandlerVector(container, (CMHandlerAddr *)&container->handler.cmformatData, CMFormatDataOpType, Required);	
		
	if (failure) {
		if (missing != (CM_CHAR *)0xFFFFFFFF && missing != NULL) {
			ERROR1(CM_err_UndefRoutine, missing);
			CMfree(missing);
		}
		CMfree(container);
		return (NULL);
	}
	
	/* Set up a object TOC for this container.  There are times during updating target 		*/
	/* containers that the TOC we're using is not for the current container, but for the	*/
	/* target container.  Thus we need two TOC pointers; one that we usually use for 			*/
	/* accessing the "proper" TOC, and one that always contains the pointer to the TOC we	*/
	/* create here for this container.																										*/
	
	if ((container->privateTOC = cmCreateTOC(SESSION->cmTocTblSize, container)) == NULL) {
		ERROR1(CM_err_NoTOC, CONTAINERNAME);
		CMfree(container);
		return (NULL);
	}
	container->toc = container->privateTOC;				/* at this point both TOC ptrs are equal*/
	container->usingTargetTOC = false;						/* ...and there is no target TOC				*/

	/* Set up the global name symbol table in much the same way we did the TOC.  We need	*/
	/* two tables here too.																																*/
	
	if ((container->privateGlobals = cmCreateGlobalNameTbl(container)) == NULL) {
		ERROR1(CM_err_NoGlobalTable, CONTAINERNAME);
		CMfree(container);
		return (NULL);
	}
	container->globalNameTable = container->privateGlobals;
	container->usingTargetGlobals = false;
	
	/* Fill in the container control block and append in on to the list of open 					*/
	/* containers.  At this point, at least, some of the fields which are zeroed out will	*/
	/* be filled in shortly as a function of whether we're reading or writing. The flags,	*/
	/* and generation are initially 0 for reading (i.e., when called by										*/
	/* CMOpenContainer()), and caller specified values when writing (i.e., when called by	*/
	/* CMOpenNewContainer()).  In the read case the actual values get filled in when we		*/
	/* load in the actual "label" and TOC.																								*/
	
	memcpy(container->magicBytes, MagicByteSequence, strlen(MagicByteSequence) + 1);
	container->majorVersion					= MajorVersion;
	container->minorVersion					= MinMinorVersion;
	
	container->updatingContainer 	 	= container;
	container->targetContainer  		= container;
	container->pointingValue				= NULL;
	container->touchTheValue				= false;
	container->openingTarget				= false;
	container->updateMergeCandidate = false;
	container->depth								= 0;
	
	container->refCon								= NULL;
	container->containerFlags				= containerFlags;
	container->generation						= generation;
	container->tocBufSize						= (DefaultTOCBufSize * kCMBufSizeUnits);
	container->tocOffset						= 0;
	container->tocSize							= 0;
	container->nextStdObjectID			= MinStdObjectID;
	container->nextUserObjectID			= MinUserObjectID;
	container->useFlags							= (CM_USHORT)useFlags;
	container->physicalEOF					= 0;
	container->logicalEOF 					= 0;
	container->maxValueOffset				= 0;
	container->embeddedValue				= NULL;
	container->getBaseValueOk				= false;
	container->nbrOfDynValues				= 0;
	container->dynamicBaseValueHdr	= NULL;
	container->dynValueProperty			= NULL;
	container->spaceDeletedValue		= NULL;
	container->tocNewValuesValue		= NULL;
	container->freeSpaceValueHdr		= NULL;
	container->deletesValueHdr			= NULL;
	container->touchedChain					= NULL;
	container->ioBuffer							= NULL;
	container->tocIOCtl							= NULL;
	container->mergeInSize					= 0;
	
	container->trackFreeSpace				= true;		/* free space normally always tracked				*/
	container->deleteGarbage				= false;	/* don't delete garbage until CMKeepObject	*/	
	
	cmInitList(&container->deletedValues);
	cmInitList(&container->embeddedContainers);
	cmInitList(&container->activeIOBuffers);	
	cmInitList(&container->tmpList);	
		
	/* Determine whether we're reading or writing. If writing, there is not too much to do*/
	/* since there isn't anything yet created.  For reading, however, we must "load" in		*/
	/* the "label" info and TOC info the "label" points to.	 A "load" here is as defined	*/
	/* by the handlers that do the actual work.	 A special case exists for converting a   */
	/* "chunk" of data to the container format.  In that case we open the file for 				*/
	/* appending.																																					*/
	
	/* The modes of container (file) opening we use here need some discussion.  We have 	*/
	/* three open modes to go with the three open cases:																	*/
	
	/*		converting   For this mode the open mode is "rb+".  The intent is to open a 		*/
	/*								 container for updating, i.e., reading and writing, but preserve the*/
	/*								 current contents of the file.																			*/
	
	/*		writing			 The mode here is "wb+".  The intent is to create the container if	*/
	/*								 it doesn't already exist, set its file size to 0 (trucnate it), and*/
	/*								 to allow BOTH reading and writing (update).  The API allows reading*/
	/*								 of stuff previously written (why not?).														*/
	
	/*		reading			 The read open mode is "rb" (no plus). An existing container is to 	*/
	/* 								 be opened for input (reading) only.  It cannot be written to for		*/
	/*								 updating.																													*/
	
	/*		updating		 The read open mode is "rb+". An existing container is to be opened	*/
	/*								 for updating.  This is also used for reusing free space.						*/
	
	/* The "b" in these modes is just to indicate a binary file is intended as opposed to	*/
	/* a text file.  Some operating systems make this distinction. This is ANSI standard. */
 	
	/* The following summarizes the open modes in terms of the useFlags.  Entries marked	*/
	/* with CM_err_BadUseFlags are error conditions.  Ambiguous cases are also checked by */
	/* CMOpen[New]Container(). The kCMWriting flag is NOT set by CMOpenNewContainer() for */
	/* the updating cases.  That tells us which is open case we got here so we know to 		*/
	/* open rb+ rather than wb+. The kCMWriting then flag is or'ed in after we do the rb+ */
	/* open.																																							*/
		
	/*              useFlags     || CMOpenNewContainer() | CMOpenContainer()										*/
	/*        ===================||======================|====================						*/
	/*         kCMReading        ||         n/a          |        rb											*/
	/*         kCMWriting        ||         wb+          |        n/a											*/
	/*         kCMReuseFreeSpace ||         n/a          |        rb+											*/
	/*         kCMConverting     ||         rb+          | CM_err_BadUseFlags							*/
	/*         kCMUpdateByAppend ||         rb+          | CM_err_BadUseFlags							*/
	/*         kCMUpdateTarget   ||         wb+          | CM_err_BadUseFlags							*/
	/*        -------------------++----------------------+--------------------						*/
	
	if (useFlags & kCMConverting) {															/* if converting...				*/
		container->refCon = CMfopen(container, attributes,"rb+");	/* ...open for update...	*/
		valueSize 		= CMgetContainerSize(container);						/* next free byte 				*/
		container->originalEOF 		= valueSize;										/* next free byte 				*/
		container->physicalEOF 		= valueSize;										/* next free byte 				*/
		container->logicalEOF 		= valueSize;										/* logical EOF						*/
		container->maxValueOffset = valueSize; 										/* last byte offset + 1		*/
		container->useFlags 			= (CM_USHORT)(useFlags |= kCMWriting);
	} else if (useFlags & kCMWriting)	{													/* if just writing...			*/
		container->refCon = CMfopen(container, attributes,"wb+");	/* ...open update & trunc */
	} else {																										/* if reading...					*/
		if ((useFlags & kCMReuseFreeSpace) != 0) { 		/* if reuse free space...							*/
			container->refCon = CMfopen(container,attributes,"rb+");/* ...open for updating		*/
			container->useFlags = (CM_USHORT)(useFlags |= kCMWriting);
		} else
			container->refCon = CMfopen(container, attributes,"rb");/* ...open for reading		*/
		
		if (!validateContainerLabel(container, &container->containerFlags, 
																&container->majorVersion, &container->minorVersion, 
																&container->tocBufSize, &tocOffset, 
																&tocSize)) { 									/* ...validate label			*/
			UndoOpen();
			return (NULL);
		}
		
		valueSize 		= CMgetContainerSize(container);						/* next free byte 				*/
		container->originalEOF 		= valueSize;										/* next free byte 				*/
		container->physicalEOF 		= valueSize;										/* next free byte					*/
		container->maxValueOffset = valueSize; 										/* last byte offset + 1		*/
		container->tocOffset			= tocOffset;										/* offset to TOC					*/
		container->tocSize				= tocSize;											/* TOC total size					*/
		container->logicalEOF 		= tocOffset;										/* doesn't include TOC		*/
			
		#if CMDUMPTOC																							/* show TOC in container	*/
		if (SESSION->cmDbgFile) 																	/* ...if debugging				*/
			CMDumpContainerTOC((CMContainer)container, SESSION->cmDbgFile, tocOffset, tocSize);
		#endif
	
		if (!cmReadTOC(container, TocEntry_All, tocOffset, tocSize, NULL)) {					/* read in TOC   					*/
			UndoOpen();
			return (NULL);
		}
	}
	
	/* At this point we must determine if we're opening an embedded container. We do this	*/
	/* by calling the special handler routine, "returnParentValue".  If this handler 			*/
	/* exists, and doesn't return NULL, then we, by definition, are opening an embedded		*/
	/* container.  The non-null value will be a CMValue for the value in the parent 			*/
	/* container representing the embedded container.  If the handler doesn't exist, or		*/
	/* does but returns NULL, then we assume we don't have an embedded container.					*/
	
	if (container->handler.cmreturnParentValue != NULL)		/* if the handler exists...			*/
		theValueHdr = CMreturnParentValue(container);				/* ...get the parent CMValue		*/
	else
		theValueHdr = NULL;																	/* not embedded container				*/
		
	/* If we are indeed opening an embedded container, we maintain a list of all the 			*/
	/* containers which are embedded descendents of a parent container.  We have to 			*/
	/* allocate an EmbeddedContainer list entry and append it to the parent's list of 		*/
	/* embedded containers (NOT the container we are creating now). The list contains a 	*/
	/* pointer to the embedded container control block we are currently creating.  This 	*/
	/* allows CMCloseContainer() of a container to close all its descendents (i.e., 			*/
	/* embedded containers) first before closing itself.																	*/
	
	/* This is somewhat tricky.  At least it had me going for a while!  It's not the code	*/
	/* that adds the list entry to the parent that's tricky. It's what is implied by doing*/
	/* that.  Namely, a descendent (i.e., embedded) container is creating a "thing" in 		*/
	/* the parent's container control block (or at a least owned by the parent).  The			*/
	/* parent looks at this list to see if must recursively close embedded containers when*/
	/* CMCloseContainer() is called.  But descendents can be closed explicitly and 				*/
	/* separately BEFORE (when else?) their parents.  So, in order to be able to do a 		*/
	/* recursive depth-first search down what effectively is a tree of embedded containers*/
	/* and to satisfy independent closing, the descendent must be responsible for removing*/
	/* the list entry on the parent's list.  The parent CANNOT do it.  That's the tricky	*/
	/* bit!  There is symmetry to this though.  Here the descendent is creating the list	*/
	/* entry on the parents list.  It's only fair (!) that the descendent remove it!			*/
	
	/* Finally, because I don't trust the world, we do some validity checks on the value	*/
	/* given to use before we do any of the above!	Here's what we check:									*/
	
	/*	1. If we are reading, there must be a value.  																		*/
	
	/* 	2. If we are writing, there must be 0 values for the value header.  We are about	*/
	/* 		 to create the one and only value.  So why would there be one there?  The user 	*/
	/* 		 must do a CMNewValue(). 																												*/
	
	if (theValueHdr) {																		/* if embedding...							*/
		container->embeddedValue = theValueHdr;							/* remember parent's valueHdr		*/
		
		#if 0
		if (theValueHdr->typeID != CM_StdObjID_Embedded) {	/* type MUST be for embedding		*/
			ERROR1(CM_err_NotEmbedType, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}
		#endif
		
		valueSize = CMGetValueSize((CMValue)theValueHdr);		/* make sure size is correct		*/
		if (valueSize == 0 && (useFlags & kCMWriting) == 0){/* must be >0 for reading				*/
			ERROR1(CM_err_EmptyRead, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}
			
		if (valueSize != 0 && (useFlags & kCMWriting) != 0)	{	/* must be 0 for writing			*/
			ERROR1(CM_err_HasValue, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}

		/* If this container is open for writing so must have the parent. Similarly if we		*/
		/* are opening for reading the parent must have been too.  This is a safety check.	*/

/*		
		if (!(((useFlags & kCMWriting) != 0 && (theValueHdr->container->useFlags & kCMWriting) != 0) ||
				  ((useFlags & kCMWriting) == 0 && (theValueHdr->container->useFlags & kCMWriting) == 0))) {
			ERROR2(CM_err_NotSameMode, CONTAINERNAME, CONTAINERNAMEx(theValueHdr->container));
			UndoOpen();
			return (NULL);
		}
*/		
		if (((useFlags & kCMWriting) == 1 && (theValueHdr->container->useFlags & kCMWriting) == 0)) {
			ERROR2(CM_err_NotSameMode, CONTAINERNAME, CONTAINERNAMEx(theValueHdr->container));
			UndoOpen();
			return (NULL);
		}
		
		/* The value and its object are protected from deleting during the time it is being	*/
		/* used for an open embedded container. The protection is removed when the value is */
		/* "closed". Thus it could then be deleted.																					*/
		
		theValueHdr->valueFlags |= ValueUndeletable;				/* protect from user tampering	*/
		theValueHdr->theProperty->theObject->objectFlags |= ProtectedObject;
		
		/* Here we create an EmbeddedContainer list entry for adding to the parent's 				*/
		/* embedded container list.  The entry contains the pointer to the container we're	*/
		/* setting up.  That way the parent can know its descendents to close them before 	*/
		/* it itself gets closed. Since the descendent must delete the entry from the 			*/
		/* parent's list we record in the container we're creating the pointer to the list	*/
		/* entry.																																						*/
		
		embedded = (EmbeddedContainerPtr)CMmalloc(sizeof(EmbeddedContainer));/* alloc entry	*/
		if (embedded == NULL) {																						 	 /* oops!				*/
			ERROR1(CM_err_NoEmbedding, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}
		embedded->container = container;										/* set descendent container			*/
		cmAppendListCell(&theValueHdr->container->updatingContainer->embeddedContainers, embedded);/*add to lst*/
		container->parentListEntry = embedded;							/* remember where this entry is	*/
		#if DebugEmbedding
		fprintf(stderr, "added list entry $%.8lX to parent container $%.8lX of container $%.8lX\n", embedded, theValueHdr->container, container);
		#endif
	} else
		embedded = NULL;
		
	container->tocFullyReadIn = true;								/* assume we have a "stable" TOC now	*/
	
	/* Register all the predefined types and properties.  This is done to allow the API 	*/
	/* user the ability to get at these predefined objects using the standard API "CM..."	*/
 	/* interfaces.  Although these objects are truely in the TOC we never write them out 	*/
	/* to the container.  So reregistering them is necessary every time we open a 				*/
	/* container.																																					*/
	
	if (!registerStdTypesAndProperties(container)) {/* predefined our types and properties*/
		if (embedded) CMfree(embedded);								/* ...oops!														*/
		UndoOpen();
		return (NULL);																/* ...we we're so close too!					*/
	}	

	/* We need to remember the refNum for the property which is always used for dynamic		*/
	/* values.  We get the refNum just like the user would.  But the user should never		*/
	/* do this for this particular global name.																						*/
	
	container->dynValueProperty = CMRegisterProperty((CMContainer)container, (CMGlobalName)CMDynamicValueGlobalName);
	if (container->dynValueProperty == NULL) {
		UndoOpen();
		return (NULL);
	}
	
	/* Just as we did with the dynamic value property, we need to save the property 			*/
	/* object for base types.  This is used by CMRemoveBaseType().												*/
	
	container->baseTypesProperty = CMRegisterProperty((CMContainer)container, (CMGlobalName)CMBaseTypesGlobalName);
	if (container->baseTypesProperty == NULL) {
		UndoOpen();
		return (NULL);
	}

	/* Add the new container control block to the list of "officially" open and  active		*/
	/* containers.  The pointer to this block is what we return as the "refNum" which the */
	/* caller will pass back to us in all routines that require the current container.		*/
	
	cmAppendListCell(&SESSION->openContainers, container);
	
	/* If the newly opened container is an updating container, then "pull down" the 			*/
	/* pointer to the top-most updating container.  That is, copy down the 								*/
	/* updatingContainer pointer from the previous container control block.  The way we 	*/
	/* can tell the previous container control block is an updater is because it will have*/
	/* a switch set by the caller, openingTarget.  That switch uniquely identifies the		*/
	/* container as an updater and we reset that switch here.  For multi-layered updaters,*/
	/* all of them will end up pointing to the same top-most updater because each one			*/
	/* copies the pointer from its immediate parent.																			*/
	
	/* By propagating the same pointer into each target container, all of them will be 		*/
	/* able to get at the "top most" container we eventually return to the user as the 		*/
	/* refNum.  It's not too important for just reading.  But if we're opening a NEW 			*/
	/* container, all new updates must be placed in this container.  For multi-layered		*/
	/* updaters, there are multiple containers pointed to by the one TOC all these 				*/
	/* containers are using (see connectTargetToItsUpdater()).  We must be able to get at	*/
	/* the new updater no matter what container a TOC entry is actually pointing at.			*/
	
	/* open() is the routine that initiates the lower level opens. It does	*/
	/* a standard CMOpenContainer() to read a target which itself may be an updater that	*/
	/* again goes through openTargetContainer(), and so on.  That's how the multiple 			*/
	/* layers build up (or looking at the top-most one as the first one, "build down").  	*/
	/* It is thus one the way "down" through the layers that we're pulling the top most		*/
	/* container pointer with us.																													*/
	
	/* When openTargetContainer() calls CMOpenContainer() it defines a special type name	*/
	/* associates with a special handler package that is used to access the target through*/
	/* a value refNum.  The name is the same for all layers.  Thus to aid in debugging 		*/
	/* this stuff a "depth" counter is provided.  For each recursive calls through here 		*/
	/* (remember, we know it's recursive based on the openingTarget switch) we "pull down"*/
	/* and increment the counter.  It is used by the TOC debugging display routines. It's */
	/* better than nothing!																																*/
	
	previouslyOpenedContainer = (ContainerPtr)cmGetPrevListCell(container);
	if (previouslyOpenedContainer != NULL && previouslyOpenedContainer->openingTarget) {
		previouslyOpenedContainer->openingTarget = false;
		container->updatingContainer = previouslyOpenedContainer->updatingContainer;
		container->depth						 = (CM_USHORT)(previouslyOpenedContainer->depth + 1);
	}
	
	return (container);															/* return "CCB" ptr as the refNum			*/
}


/* The following macro is the counterpart to UndoOpen() above (and, by the way, is the	*/
/* same name). It will be used for the same purpose.  But this one is used by 					*/
/* CMOpen[New]Container() to "recover" from their error reports. This one is used after */
/* openContainer() returns and hence the container is now on the open container list. 	*/
/* So it must be deleted from there.																										*/

#undef UndoOpen
#define UndoOpen()	CMfclose(container);																									\
										cmFreeAllIOBuffers(container);																				\
										cmFreeAllGlobalNames(&container->globalNameTable); 										\
										cmFreeTOC(container, &container->toc);																\
										CMfree(cmDeleteListCell(&SESSION->openContainers, container));


/*-------------------------------------------------------------------------------------*
 | connectTargetToItsUpdater - connect a target container to the container updating it |
 *-------------------------------------------------------------------------------------*
 
 When opening appended target containers for updating or when opening a separate container
 (which may have appended containers) for updating, we must "connect" the target to its
 updater.  This routine is used for that purpose.
 
 The callers to this routine are openTargetContainer(), as it opens appended containers, or
 lower layered (i.e., older) separate targets, and from openSeparateTargetToBeUpdated()
 when connecting a new (top layer) updating container to a separate target.
 
 "Connection" here means we do the following things:
 
 		1. The TOC of the target is used and marked as belonging to this updating container.
			 Thus new updates will be or'ed into one common TOC but items are marked as to which
			 container they come from.
			 
		2. Similarly, the target's global name table is marked as belonging to this updating
			 container.  Thus new global names will be or'ed into the one common table.
			 
		3. The "target" container of the updating container is set from the target's target.
			 The lowest-level target (i.e., deepest) will point to itself (normal case when not
			 updating).  The effect is to make the target of all updating containers point to
			 the same lowest-level (original) container.
			 
			 Note, that there is also a pointer in each target pointing "up" to the top-most 
			 container (called updatingContainer).  This is not set here since this pointer must 
			 be "sent down" as each updater opens its target.  In reality it's "pulled" down
			 since a target copies it from its updating parent.  This is done by in
			 openContainer() as part of the standard opening process of create a container
			 control block.
 
 The comments in the code below fully explain these items and the "how" and "why" we're
 doing what we're doing.  There isn't much code here, but there's a lot of 
 comments explaining it!
*/

static void CM_NEAR connectTargetToItsUpdater(ContainerPtr container,
																							ContainerPtr targetContainer)
{
	/* Before we apply the updates of the current container, we must carry forward the 		*/
	/* TOC of the target.  Note, however, we can't just drop the TOC for the updating 		*/
	/* container "on the floor". It has some stuff in it (e.g., like the pointing value). */
	/* So privateTOC is used to point to the container's original TOC. A flag is set here */
	/* when we carry forward the target's TOC to indicate that there are now two TOC's for*/
	/* this container.  This will be needed by CMCloseContainer() so it knows to free 		*/
	/* both.																																							*/
	
	/* The target's TOC is carried forward as the updating container's TOC by using				*/
	/* cmUseTOC().  This keeps a use count on the TOC so that we don't release it					*/
	/* prematurely as the closes are done (remember, each updating container for mutiple	*/
	/* updaters will have a copy of this TOC pointer). In addition, the "owning" container*/
	/* of the TOC is marked as the updating container.  This is necessary since we want 	*/
	/* new TOC entries (i.e., new updates) to be in the updating container.  New TOC			*/
	/* objects always get their owning container from the TOC control block whose pointer	*/
	/* were talking about here. The container, in turn, determines which handlers we use.	*/
	/* By changing the container to the updater, we use the updater's handlers.  This is	*/
	/* where the updaters are to go.																											*/
	
	/* The following diagram illustrates the container/TOC relationships:									*/
												
	/*													"A" updates "B" updates "C"																*/
	/*												  =========================== 															*/
																																												/*
       (User's refNum)
  *-----------------------*    *-----------------------*    *-----------------------*
  |Container Control Block|--->|Container Control Block|--->|Container Control Block|
  |          "A"          |    |          "B"          |    |          "C"          |
  *-----------------------*    *-----------------------*    *-----------------------*
    |privateTOC         |toc     |privateTOC         |toc               |privateTOC
    |                   |        |                   |                  |toc
    |                   |        |                   |                  |
    |   *-----------*   |        |   *-----------*   |                  |
    +-->| TOC ["A"] |   |        +-->| TOC ["B"] |   |                  |
        |use count=1|   |            |use count=1|   |                  |
        *-----------*   |            *-----------*   |                  |
                        |                            |                  |
                        |                            |                  |   *-----------*
                        +--------------------------->+----------------->+-->| TOC ["A"] |
                                                                            |use count=3|
                                                                            *-----------*/
	
	/* In the above diagram, all three containers point to "C"s toc.  But by the time "A"	*/
	/* is returned to the user as the container refNum, "A" will "own" "C"s TOC and all		*/
	/* updates from the respective containers will be applied to "C"s TOC. Note, however, */
	/* the use count is 3 in this example. CMCloseContainer() will close these containers,*/
	/* from "C" to "A".  It will attempt to free the TOC.  But the actual freeing of the	*/
	/* toc data structures won't occur until "A" is closed.																*/
	
	/* Note also that since "A" and "B" are not pointing to "C"s TOC, we save the original*/
	/* (actual?) TOC for the container in privateTOC.  CMCloseContainer() will look at 		*/
	/* this too to make sure it is freed.																									*/
	
	/* By the way, it doesn't matter that we changed the container as far as the low-level*/
	/* TOC routines are concerned.  They only use it to get at the global session data   	*/
	/* which is the same (hopefully) no matter which container we use. But they do put it */
	/* into the object as described above.																								*/
	
	container->privateTOC = container->toc;											/* save our TOC						*/
	container->usingTargetTOC = true;														/* signal we did this			*/
	container->toc = cmUseTOC(targetContainer->toc, container); /* use target's TOC				*/
	
	/* In a manner similar to the TOC, we must use a common global name tree. This code 	*/
	/* is exactly the same as for the TOC, but for the global name symbol table.					*/
	
	container->privateGlobals = container->globalNameTable;
	container->usingTargetGlobals = true;	
	container->globalNameTable = cmUseGlobalNameTbl(targetContainer->globalNameTable, container);
	
	/* Carry forward the final ("bottom") target container pointer, i.e., the original		*/
	/* container before any updates were applied.  It is put in targetContainer.  By 			*/
	/* knowing the "top" and "bottom", CMCloseContainer() knows how many containers need 	*/
	/* closing.  In addition, since the final target is the same for all the updaters of 	*/
	/* it (since we are carrying it forward through each updater), the targetContainer 		*/
	/* is used for all container consistency checks, i.e., checks in routines that take 	*/
	/* multiple objects to verify all the objects came from the "same" container.  This		*/
	/* allows the objects to be from different containers so long as they are updaters.		*/
	
	container->targetContainer = targetContainer->targetContainer;

	/* make the minor version number of container at least that of target container				*/
	if (container->minorVersion < targetContainer->minorVersion)
		container->minorVersion = targetContainer->minorVersion;
}																															/* wasn't this fun?				*/


/*------------------------------------------------------------------------------*
 | openTargetContainer - open appended container (TOCs) and apply their updates |
 *------------------------------------------------------------------------------*
 
 This routine is called by both CMOpenContainer() and openAppendedTargetToBeUpdated()
 after it has opened a container for reading or an appended container layer that is to
 receive new updates.  This routine looks to see if there are (more) appended layers of 
 updates and opens those containers as targets to apply the updates to bring the targets
 up-to-date.
 
 This function always returns the originally opened container.  NULL is returned if any
 errors are reported and the error reporter returns.
 
 If the original container indicates that it has updates for a target container, the
 updates are applied to the target.  The target itself may have updates to apply to yet
 another target.  Thus the applying of the updates is recursive.  The end result is to
 apply all the updates and to return the initial container with a TOC that is an or'ing of
 all the targets appropriately updated.  Entries in this TOC are marked as to which
 container they came from.  All the updating containers remain open and are closed when
 CMCloseContainer() is called on the original container (which is the only one the use
 would know about -- we open all the targets).

 The caller must pass theTOCObject.  This is the object pointer to the TOC ID #1 object.
 The caller always has that for its use.  We need it here.  So we might as well get the
 caller's than having use reconstruct it here.
 
 openAppendedTargetToBeUpdated() is used only by CMOpenNewContainer() when a new layer is
 to be created (kCMUpdateByAppend useFlags).  CMOpenContainer() always calls this to see
 if the container that was opened was actually a layer that applies updates to a target.
 
 The presence of a target is detected by whether there is a "pointing" value belonging to
 a special property of TOC #1.  Each updating container, whether updating "remotely" (i.e.,
 updating a distinct container) or as an appended container of updates, contains this
 pointing value.  The separate	container case is typed so as to spawn a dynamic value that
 will access the target container.  The append case is an embedded value that defines the
 portion of itself it operates on (i.e, the target container).
	
 Here it doesn't matter if we're creating a new layer or just reading them.  If we created
 a new layer, we created the requisite pointing value from openAppendedTargetToBeUpdated()
 prior to it calling this routine.  So we can always check for the presence of the pointing
 value.  If it is there, we use it to recursively open the target container by calling
 CMOpenContainer().  The handlers we define for this are the special handler package which
 does its operations in terms of the pointing value.  It is very similar to the embedded
 container handlers package (in fact, that's where it came from).
	
 When we open the target we must apply the updates in the current container to that target.
 This will bring the resulting data structures "up to date" as when the updates were
 originally applied to the target.
	
 Since multiple open/close operations of updates can be done, we can have multiple layers
 of updates and TOCs! resulting from open-by-append opens (useFlags kCMUpdateByAppend).
 Thus we must actually check the target to see if it itself has a special pointing value.
 If it does we must follow it down, opening targets as we go (recursively calling
 CMOpenContainer() which calls back here), until we reach the "bottom", i.e., the original
 container before any updates were applied.  Then the recursion starts "unwinding".  
 
 As each layer unwinds, we then apply the updates of a container on its next lower target.
 This effectively brings the original container "up-to-date".  The applying of the updates
 is done in such a way as to produce a single TOC, but with entries tagged to the container
 doing the updating.  The updating container is always returned.  In the end the original
 container that was the "newest" update (the first one we looked at) is returned and that
 is what the user will use.
*/

static ContainerPtr CM_NEAR openTargetContainer(ContainerPtr container, 
																							 TOCObjectPtr theTOCObject)
{
	ContainerPtr 	 targetContainer, savedUpdatingContainer;
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr, targetValueHdr;
	CMProperty		 targetProperty;
	CMType				 targetType;
	ListHdr 			 newEntryList;

	cmInitList(&newEntryList);

	/* We have to use the "pointing value" to get at the target.  The target may be 			*/
	/* appended or separate as determined by the pointing value, in particular, its type.	*/
	/* We do a CMUseValue() on the pointing value's object, property, and type refNums.		*/
	/* For a separate container a dynamive value will be spawned.  This is discussed 			*/
	/* shortly.  But first, we must get the refNums.  Just to be paranoid, we check that	*/
	/* we indeed get this stuff.																													*/
	
	targetProperty = (CMProperty)cmFindObject(container->toc, CM_StdObjID_TOC_Target);
	
	theProperty 	 = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Target);
	
	if (theProperty) {
		targetValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		targetType		 = (targetValueHdr) ? (CMType)cmFindObject(container->toc, targetValueHdr->typeID)
																			: NULL;
	}
	
	if (theProperty 	 == NULL ||												/* if no target container...			*/
			targetValueHdr == NULL ||
			targetProperty == NULL || 
			targetType 		 == NULL) 	
		return (container);								 							 	/* ...return what we just opened	*/
	
	/* We have a "pointing" value.  Do the CMUseValue() on it.  If it is for a "remote"		*/
	/* container, the type will spawn a dynamic value.  This is the main reason for the		*/
	/* CMUseValue().  If it is not a real value nothing much happens, but in either case	*/
	/* we get a refNum that will be passed as the attributes to CMOpenContainer() to open	*/
	/* the target container.  Note, we have to squirrel away a copy of this value refNum  */
	/* since we're doing a CMUseValue() the user knows nothing about.  We are obligated		*/
	/* therfore to do a CMReleaseValue() on this at close time.  Indeed, if a dynamic			*/
	/* value is spawned, it is REQUIRED!																									*/
	
	theValueHdr = (TOCValueHdrPtr)CMUseValue((CMObject)theTOCObject, targetProperty, targetType);
	if (theValueHdr == NULL) {
		UndoOpen();
		return (NULL);
	}
	
	container->pointingValue = (CMValue)theValueHdr;		/* remember to release at close		*/	
	
	/* We have a target, if we may want to merge it later, we set the merge flag if				*/			
	/* this is the top container in the original file.																		*/
	
	if (container->depth == 1) 
		if (container->updatingContainer->useFlags & kCMMerging)
			if (container->updatingContainer->useFlags & (kCMUpdateByAppend | kCMUpdateTarget))
				container->updateMergeCandidate = true;
			
	/* Open the "target" using the pointing value as the "attributes".  We are using the	*/
	/* predefined container type CMTargetHandlersTypeName, which is assumed to be 				*/
	/* registered for the special handler package that does its operations in terms of 		*/
	/* the value passed in the attributes.  Because it is passed in the attributes, it 		*/
	/* will be picked up by the package's open handler because that what it expects to 		*/
	/* build its refCon. 																																	*/
	
	/* Remember that the target container can itself be an updating container with its 		*/
	/* own updates to be applied to a yet "deeper" target (leading to recursive calls to	*/
	/* this routine).  To be able to do this, ALL targets need to be able to get at the 	*/
	/* top-most container.  This is done using the updatingContainer pointer.  The 				*/
	/* openingTarget switch allows this to happen. 	It is interrogated by openContainer()	*/
	/* to propagate the top-most updater's updatingContainer value "down" as each target	*/
	/* is opened.  The top-most's updatingContainer points to itself, so all the lower		*/
	/* level target's updatingContainer will point there as well.													*/
	
	container->openingTarget = true;								/* target gets our updatingContainer	*/

	targetContainer = (ContainerPtr)CMOpenContainer((CMSession)container->sessionData, 
																									(CMRefCon)theValueHdr,
																									(CMGlobalName)CMTargetHandlersTypeName, 0);
	container->openingTarget = false;								/* set switch to its "natural" state	*/
	if (targetContainer == NULL) {
		UndoOpen();
		return (NULL);
	}
	
	/* If this is the top level container and we want to reuse free space, steal the			*/
	/* free space from level 2. Note that we only do it for the first level because				*/
	/* once the higher level container takes free space from the lower level, the free		*/
	/* space will be used and the lower level list which is not modified is obsolete			*/
	/* so we should only do this once, grab it and ignore the lower level free list				*/
	
	if (container->useFlags & kCMReuseFreeSpace)
		cmTakeOverFreeList(container, targetContainer);

	/* Fix up the updating container so that it uses the proper TOC, global name table,		*/
	/* and has its updating and target pointers correctly set.														*/
	
	connectTargetToItsUpdater(container, targetContainer);

	/* When the container went through it's "normal" open, only the TOC objects belonging	*/
	/* to this container were read, i.e., the "private" TOC.  This happened because 			*/
	/* cmReadTOC(), which is used to actually load in the TOC, is "trained" to stop when  */
	/* sees it's reading a updating TOC and the property exists that defines where non-		*/
	/* private updating TOC objects.  These are objects to be merged with the target's		*/
	/* TOC.  We inherited the target's TOC when we "connected" to it above.  Now we want 	*/
	/* do that merge.  So we have to do the additional read.  The limits of this read are	*/
	/* defined by the property as just mentioned.  We already have a pointer to this 			*/
	/* property's value, because the normal open sequence set it before calling us here.	*/
	/* But it sets it only if it actually finds the property.  There it doesn't know it's	*/
	/* an updating container. Here we do. So if the pointer is not there, it's an error! 	*/
	
	/* Note, the non-private section of the TOC is actually made up of two subsections 		*/
	/* itself, although we read them both with this single read. The first subsection are */
	/* "new property (values) for old objects". The second subsection is just new objects.*/
	
	/* One of the "new properties for old objects" will be a special "updating" property,	*/
	/* whose value points to the updating instructions on how to update all the values of	*/
	/* its associated object.  These instructions cannot be applied until the non-private */
	/* TOC is fully read in because there could be forward reference to other objects. So	*/
	/* if the special "updating" property is spotted for any object, that object is placed*/
	/* on the "touched chain" whose real use is for remembering who got touched during		*/
	/* NEW updating.  But that's after all opening is completed, so we can overload it to */
	/* use it here.  The chain is used after the TOC is read in to apply the associated 	*/
	/* updates.																																						*/
	
	/*                     --- S U B T L E   P O I N T ! ! ! ---													*/
	/*                       (and caused hours of bug hunting)														*/
	/*                                   -----                                            */
	
	/* We have to be VERY careful here about "who's on first" with respect to whose 			*/
	/* container we're talking about and from whose container we thread the touched 			*/
	/* objects that are to receive those updates (which container "owns" the touched 			*/
	/* chain).  																																					*/
	
	/* All generation of TOC information and all the updating manipulations are set up for*/
	/* "normal" processing, i.e., when a container is fully open and new updates are being*/
	/* generated. To that end, a pointer in every container, updatingContainer, is defined*/
	/* to point to the top most container, i.e., the one whose refNum we return to the 		*/
	/* user.  It acts as an "anchor point"; a place to look at no matter which container	*/
	/* pointer we happen to have.  Remember, connectTargetToItsUpdater(), just called 		*/
	/* above, connects the containers so they are all using the SAME TOC.  TOC entries 		*/
	/* thus will be "tagged" with the container that "owns" them.   The updatingContainer	*/
	/* pointer is sent "down" as each container is opened so that they immediately have 	*/
	/* the "proper" container to point to.   The problem, and subtle point, is with that  */
	/* word, "proper"!  																																	*/
	
	/* To "tag" each new TOC entry with its owning container, all entry creation use the  */
	/* updatingContainer.  In the normal case, that's what we want; the container that is */
	/* to receive all new values.  For creating a new updating container, that would be 	*/
	/* the top-most container we return to the user. 																			*/
	
	/* When recording updates, all touched objects are chained to the touched chain.  The	*/
	/* "owner" of this chain is again the updatingContainer.															*/
	
	/* Thus, updatingContainer is the key pointer controlling value tagging and object 		*/
	/* chaining.  At TOC load time the updatingContainer must be the one where new values	*/
	/* are to go and where objects needing updating are to be chained to.	 The subtle 		*/
	/* point here (finally) is that updatingContainer is pointing at the TOP MOST					*/
	/* container and, HERE, as we're unwinding the recursive opens to insert new TOC			*/
	/* entries and to chain objects needing updating, the top-most TOC is NOT what we 		*/
	/* want!																																							*/
	
	/* The container of interest here is the CURRENT container.  This is the one just			*/
	/* connected to its target (no matter how many it was previously connected to -- we		*/
	/* view it here as just one).  This is the one we're about to load non-private TOC		*/
	/* entries into and tag them with it.  And finally, this is the one which we must 		*/
	/* chain objects to, since the chain builder is called during TOC reading.  					*/
	
	/* There can be only one updatingContainer that all these guys can look at.  They get	*/
	/* it by going through their container pointer parameter.  So, in order to "fake out"	*/
	/* all these guys, we have to TEMPORARILY CHANGE updatingContainer so it points to the*/
	/* current container we're loading.  The new values will thus be tagged with who owns	*/
	/* them, and the touched chain of objects needing updating will be here as well.  		*/
	
	/* This is the only place this "stunt" happens.  During standard TOC loading of the 	*/
	/* private TOC, updatingContainer IS pointing to the container being opened to tag		*/
	/* those entries with that container.  We're essentially doing the same thing here.		*/
	/* The extra twist here is that a touched chain is also being built.  That will never	*/
	/* happen in a private TOC.																														*/
	
	/* I hope this explains what we about to do with updatingContainer.  You will never 	*/
	/* know how many hours (days?) this took to debug!  It may be obvious now.  It wasn't */
	/* then!  This was one killer bug!																										*/
			
	if ((container->useFlags & kCMWriting) == 0) {			/* if not new updating container  */
		if (container->tocNewValuesValue == NULL) {				/* ...there must be updating TOC	*/
			ERROR1(CM_err_NoNewValuesTOC, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}
		
		/* Here's we we perform the "magic" discussed above.  Temporarily change the				*/
		/* updatingContainer to point to the container to receive the new values and to			*/
		/* whom we tie the touched chain of objects needing updating.												*/
		
		savedUpdatingContainer = container->updatingContainer; /* save updatingContainer		*/
		container->updatingContainer = container;					/* CHANGE updatingContainer				*/ 
		
		/* Load in the non-private TOC of this container and build the touched chain of all	*/
		/* objects needing updating.																												*/
		
		/* since we want them for update, we are only interested in the udpate entries 			*/
		
		if (TOCNewValuesTOCSize > 0) {										/* if new value updates exist...	*/
			container->tocFullyReadIn = false;							/*    (safety status switch)			*/
			if (!cmReadTOC(container, TocEntry_Update, TOCNewValuesTOCOffset, TOCNewValuesTOCSize,
										 &newEntryList)) {
				UndoOpen();
				cmFreeAllListCells(&newEntryList, SESSION);
				return (NULL);
			}
			container->tocFullyReadIn = true;								/* reset safety switch						*/
			
		}
	
		/* Now apply the updates in this container to the target container.  The touched 		*/
		/* chain built during the reading in of the non-private TOC is walked and processed.*/
		/* There is also a separate chunk of updates that deal solely with object and 			*/
		/* property deletions.  These are pointed to by a special "deletes" property in the	*/
		/* private TOC #1.  																																*/
		
		/* Remember, that we're applying all these updates as we unwind the recursion, i.e.,*/
		/* after CMOpenContainer() called above returns.  The recursion takes us "back in 	*/
		/* time".  Unwinding moves forward in time so the updates for each layer are applied*/
		/* in the proper order.																															*/
	
		if (!cmApplyUpdates(container)) {									/* ...apply the updates (if any)	*/
			UndoOpen();
			cmFreeAllListCells(&newEntryList, SESSION);
			return (NULL);
		}

		if ((TOCNewValuesTOCSize > 0) && (!cmIsEmptyList(&newEntryList)))  {
			/* if new value updates exist...	*/
			container->tocFullyReadIn = false;							/*    (safety status switch)			*/
			if (!cmReadTOC(container, TocEntry_NewEntry, TOCNewValuesTOCOffset, TOCNewValuesTOCSize,
										 &newEntryList)) {
				UndoOpen();
				cmFreeAllListCells(&newEntryList, SESSION);
				return (NULL);
			}
			container->tocFullyReadIn = true;			/* reset safety switch	*/
			
#if CMDUMPTOC									/* show TOC in container	*/
			if (SESSION->cmDbgFile) 		/* ...if debugging			*/
				if (container->majorVersion > 1)		/* ...and not format 1 TOC	*/
					CMDumpContainerTOC((CMContainer)container, SESSION->cmDbgFile, TOCNewValuesTOCOffset, TOCNewValuesTOCSize);
#endif  /* CMDUMPTOC */
		}
	
		/* Finally, put updatingContainer back the way it should be...											*/
		
		container->updatingContainer = savedUpdatingContainer;
	}
	
	return (container);																	/* return this container to caller*/
}


/*----------------------------------------------*
 | CMOpenContainer - open a container for input |
 *----------------------------------------------*
 
 The container corresponding to the specified typeName is opened for input or for updating
 by reusing free space.  The association between the typeName and the physical container is
 through the metahandler defined for that same typeName.  The metahandler, in turn, defines
 the handlers for the container and thus knows how to get at the physical container.
 
 The useFlags must be 0 or kCMReuseFreeSpace.  0 implies that the container is to be open 
 for reading only.  No writes may be done.  If kCMReuseFreeSpace is specified, than BOTH
 reading and writing may be done to update the container.  Free space from deleted data
 will be reused and overwrites of existing data may be done to change it (subject to the
 container label flags, see below).
  
 Free space is always kept track of on a list.  It takes the form form of standard TOC
 entries for TOC ID 1, property CM_StdObjID_TOC_Free.  Only space greater than a TOC entry
 size is remembered since each free list entry cost at least a TOC entry itself.
  
 The attributes parameter is a value that may contain anything the caller wishes.  It is
 intended to tie the open handler to a specific container.  Thus the attributes serves as
 a communication channel strictly to the open. In its simplest form for a container file
 it would be a pathname.  For an embedded container, it probably would be the parent value
 (CMValue), corresponding to the embedded container.
 
 When the open handler is called to open the container, it is given the attributes so that
 the open will know what container to open or whatever (it could be a specific handler for
 a specific container -- this is a function of the metahandler/type association).
 
 Note, that special handlers must be used for embedded containers.  Such handlers must
 support the "return parent value" handler.   This handler returns the parent value refNum
 whose data contains the embedded container. The attributes and/or handlers must know it is
 an embedded container. It will define the value data for a CMValue as an embedded
 container.  Some how, usually through the attributes, the value is passed to the handlers.
 
 Any number of embedded containers can be opened (memory permitting).  Also embedded
 containers can have embedded containers which can also be opened and read.  The effect
 is that a tree of nested containers can be opened and read without restriction.
 However, when a CMCloseContainer() is done on a parent container, all of its descendents
 will also be closed.
 
 It is an error to open an embedded container for reading if it's value belongs to a 
 container open for writing.
*/

CMContainer CM_FIXEDARGS CMOpenContainer(CMSession sessionData,
																	 			 CMRefCon attributes,
																	 			 CMconst_CMGlobalName typeName, 
																	 			 CMContainerUseMode useFlags)
{
	ContainerPtr 	 container;
	TOCObjectPtr 	 theTOCObject;
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theSeedValue, theSizeValue;
	
	if (sessionData == NULL) return (NULL);					/* NOP if not initialized!						*/
	
	if (useFlags & (kCMConverting | kCMUpdateByAppend | kCMUpdateTarget)) { 
		*SessionScratchBufr = 0;
		if (useFlags & kCMConverting) strcpy((CM_CHAR *)SessionScratchBufr, "kCMConverting");
		if (useFlags & kCMUpdateByAppend) {
			if (*SessionScratchBufr) strcat((CM_CHAR *)SessionScratchBufr, ", ");
			strcat((CM_CHAR *)SessionScratchBufr, "kCMUpdateByAppend");
		}
		if (useFlags & kCMUpdateTarget) {
			if (*SessionScratchBufr) strcat((char *)SessionScratchBufr, ", ");
			strcat((CM_CHAR *)SessionScratchBufr, "kCMUpdateTarget");
		}
		SessionERROR2(CM_err_BadReadUse, typeName, SessionScratchBufr);
		return (NULL);
	}
	
	useFlags |= kCMReading;													/* throw in our "reading" flag				*/
	
	/* Open the container for input.  When we return from here the TOC will be loaded.		*/
	/* It's almost New Years (1992)...I think I'll get "loaded"!													*/
	
	/* Ok...it's not New Years any longer (in fact it's next June) -- here's some more 		*/
	/* info I just thought would be useful!  The useFlags are used by openContainer() to 	*/
	/* determine how to open the container (for pure reading, pure writing, or updating).	*/
	/* From here it will always be pure reading or updating by reusing free space.  Note 	*/
	/* that we did not set the kCMWriting useFlag for updating.  It will, however, be set */
	/* once the container is physically opened.																						*/

	if ((container = openContainer(attributes, typeName, useFlags, 0,  kCMDefaultEndian,
																 (SessionGlobalDataPtr)sessionData)) == NULL)
		return (NULL);
	
	/* Extract the starting user ID seed value, and generation from the TOC object that 	*/
	/* better be there.  We will validate the object as best we can along the way.  For 	*/
	/* updates we will create new objects starting at the seed value.  A pointer to the 	*/
	/* TOC value holding the "seed" is kept in the container control block so that we 		*/
	/* don't have to hunt it down again.  We don't need it for just reading a container,	*/
	/* but since we got the pointer we might as well make the container control block 		*/
	/* field valid anyway.				 																												*/
	
	theTOCObject = cmFindObject(container->toc, CM_StdObjID_TOC);
	if (theTOCObject == NULL) {											/* error if TOC object is missing			*/
		ERROR1(CM_err_MissingTOCObj, CONTAINERNAME);
		UndoOpen();
		return (NULL);
	}
	theTOCObject->objectFlags |= ProtectedObject;		/* this object cannot be deleted			*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Seed);
	if (theProperty) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (theValueHdr && theValueHdr->typeID == CM_StdObjID_TOC_Type)
			theSeedValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		else
			theValueHdr = NULL;
	}
	if (theProperty == NULL || theValueHdr == NULL || theSeedValue == NULL) {
		ERROR1(CM_err_MissingIDSeed, CONTAINERNAME);
		UndoOpen();
		return (NULL);
	}
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	
	container->generation				= theSeedValue->theValueHdr->generation;
	container->nextUserObjectID = theSeedValue->value.imm.ulongValue;
	container->tocIDSeedValue 	= theSeedValue;			/* save ptr to the seed value hdr			*/
	
	/* The minimum seed value is used when applying updates to a target container to 			*/
	/* suppress objects in the updating container with IDs less than the min from being 	*/
	/* applied to the target.  Such objects can occur during the opening of new updating 	*/
	/* containers before we can get at the target to know what the seed should be.				*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_MinSeed);
	if (theProperty) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (theValueHdr && theValueHdr->typeID == CM_StdObjID_TOC_Type)
			container->tocIDMinSeedValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		else
			theValueHdr = NULL;
	}
	if (theProperty == NULL || theValueHdr == NULL || container->tocIDMinSeedValue == NULL) {
		ERROR1(CM_err_MissingMinIDSeed, CONTAINERNAME);
		UndoOpen();
		return (NULL);
	}
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	
	/* The size property is used to get at the size in the TOC object.  This must be in 	*/
	/* agreement with the TOC size in the label which we already extracted to load the 		*/
	/* TOC in the first place.Note, we "lie" about the size in the TOCValueHdr for this		*/
	/* entry in that the size we place there is the TOC size and NOT the size of the 			*/
	/* value itself.  This allows CMGetValueSize() to operate without any special cases.	*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Object);
	if (theProperty) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (theValueHdr && theValueHdr->typeID == CM_StdObjID_TOC_Type)
			theSizeValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		else
			theValueHdr = NULL;
	}
	if (theProperty == NULL || theValueHdr == NULL || theSizeValue == NULL) {
		ERROR1(CM_err_MissingSize, CONTAINERNAME);
		UndoOpen();
		return (NULL);
	}
	if (theSizeValue->value.notImm.valueLen != container->tocSize) {
		ERROR1(CM_err_BadSize, CONTAINERNAME);
		UndoOpen();
		return (NULL);
	}
	
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	theValueHdr->size				 = container->tocSize;	/* "lie" about the size								*/
	container->tocObjValue	 = theSizeValue;				/* save ptr to size value for updating*/
	
	/* For updates, there is a TOC object value which represents the entire container, 		*/
	/* from first byte (always offset 0) to end of label. For in-place updates, the 			*/
	/* container size could change so this value's size must be adjusted accordingly.  		*/
	/* For appended updates we are layering additional TOCs.  The value in each TOC layer */
	/* thus includes everything up to that TOC. Also, again, still being paranoid we do	 	*/
	/* the usual validation checks.																												*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Container);
	if (theProperty) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (theValueHdr && theValueHdr->typeID == CM_StdObjID_TOC_Type)
			container->tocContainerValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		else
			theValueHdr = NULL;
	}
	if (theProperty == NULL || theValueHdr == NULL || container->tocContainerValue == NULL) {
		ERROR1(CM_err_MissingTotalSize, CONTAINERNAME);
		UndoOpen();
		return (NULL);
	}
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	
	/* For completeness we also extract the amount deleted.  It's another validation 			*/
	/* check (ok, call me paranoid -- "you're paranoid") and it makes the field in the 		*/
	/* container control block valid.	 Note, if any data is deleted during updates, we 		*/
	/* keep track of the total amount of free space.																			*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Deleted);
	if (theProperty) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (theValueHdr && theValueHdr->typeID == CM_StdObjID_TOC_Type)
			container->spaceDeletedValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		else
			theValueHdr = NULL;
	}
	if (theProperty == NULL || theValueHdr == NULL || container->spaceDeletedValue == NULL) {
		ERROR1(CM_err_MissingTotalSize, CONTAINERNAME);
		UndoOpen();
		return (NULL);
	}
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
		
	/* For updating container, the CM_StdObjID_TOC_NewValuesTOC property defines the 			*/
	/* offset and size of TOC entries that update this container's target. If this is an 	*/
	/* updating container, then when we loaded the TOC, its reading STOPPED without 			*/
	/* reading those updating TOC entries.  The reader detected the presence of the 			*/
	/* property to know to stop.  A second read will later be done after we open the 			*/
	/* target of this updating container (done at the end of this code with the 					*/
	/* openTargetContainer() call).  Then we can merge the updating TOC entries with the 	*/
	/* target.  The second read needs to know the offset and size of these updating TOC 	*/
	/* objects.  So we save this property value where it can be found.										*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_NewValuesTOC);
	if (theProperty) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (theValueHdr && theValueHdr->typeID == CM_StdObjID_TOC_Type)
			container->tocNewValuesValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		else 
			theValueHdr = NULL;
		if (theValueHdr == NULL || container->tocNewValuesValue == NULL) {
			ERROR1(CM_err_NoNewValuesTOC, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}
		theValueHdr->valueFlags |= ValueProtected;		/* don't allow writing								*/
	}

	/* The "free space" property contains a single value header with value segments 			*/
	/* defining the free space list. There may not be any "free space" property if there	*/
	/* is no free space.  If there is, we save the pointer to the value header in the			*/
	/* container to make it more efficient to maintain the free space list.  If there is	*/
	/* none, the pointer remains NULL. The cmRememberFreeDataSpace() routine will create	*/
	/* the property the first time it is called to record free space.											*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Free);
	if (theProperty) {
		container->freeSpaceValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (container->freeSpaceValueHdr == NULL) {		/* if property, we must have value		*/
			ERROR1(CM_err_MissingFreeList, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}
		container->freeSpaceValueHdr->valueFlags |= ValueProtected;	/* don't allow writing	*/
	}
	
	/* Also for updating container, there may or may not be a list of deletion updates.  	*/
	/* If there is a list, the CM_StdObjID_TOC_DeleteList property has a pointer to it.  	*/
	/* The list takes the form of ordinary value data for this property. We save a pointer*/
	/* to the value header for the value. This will be used by cmApplyUpdates() to read 	*/
	/* the deletion update instruction list to update a target container.									*/
	
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_DeleteList);
	if (theProperty) {
		container->deletesValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (container->deletesValueHdr == NULL) {			/* if property, we must have value		*/
			ERROR1(CM_err_NoDeletesList, CONTAINERNAME);
			UndoOpen();
			return (NULL);
		}
		container->deletesValueHdr->valueFlags |= ValueProtected;	/* don't allow writing		*/
	}
	
	/* If we are opening for space reuse updating, then we take the container space used	 */
	/* by the TOC and add it to the free space list.  In standard ANSI C I/O, which we 		*/
	/* assume the I/O handlers can be written in, there is no way to cut back a stream 		*/
	/* file.  So the only thing we can to is attempt to reuse */
	/* the TOC as data space.	 Note, we also include the container label in this.					*/
	/* However, if the application crash before the file is closed, we want to get back		*/	
	/* this TOC. So we add it to the temporary free list																	*/

	if ((useFlags & kCMReuseFreeSpace) != 0) {
		cmAddToTmpFreeList(container, container->tocOffset, container->tocSize + LBLsize);
		container->logicalEOF = container->tocOffset;		/* set logical EOF to TOC start			*/
	}
	
	/* At this point we have a container opened according to the useFlags.  We must now		*/
	/* check it to see if this container is an updater that contains updates to be applied*/
	/* to a target container. This is done by openTargetContainer(). See its comments for */
	/* further details.  All that we note here is that openTargetContainer() may 					*/
	/* recursively call CMOpenContainer() to handle layers of updates.  It's a bit 				*/
	/* incestrous!																																				*/
	
	return ((CMContainer)openTargetContainer(container, theTOCObject));
}


/*---------------------------------------------------------------------*
 | openSeparateTargetToBeUpdated - open target container to be updated |
 *---------------------------------------------------------------------*

 If the kCMUpdateTarget useFlag is passed to CMOpenNewContainer(), then the user wants to
 open a new container with the intent of recording updating operations (in its TOC) of
 updates applied to ANOTHER distinct (target) container.  CMOpenNewContainer() calls this
 routine to do that.

 The target container is accessed indirectly through a dynamic value whose type is gotten
 from a "return target type" handler.  That handler must have been supplied and it must
 return a type which will spawn a dynamic value.  The process of creating a dynamic value
 will generate a "real" value in the parent container.  That value can be used by future
 CMOpenContainer()'s to "get at" the target again.  To be able to find it, we put the value
 in a special property of TOC #1.  CMOpenContainer() will look for that property.
 
 All updates to the target are conceptually recorded in the parent container we will
 create.  It is "conceptual" in that the updates are actually recored as part of the the
 target's TOC data structures, but distinctively marked so we know to whom they belong.
 
 This function takes as input the parent container, newly created by CMOpenNewContainer()
 (this routines only caller), the pointer to the TOC #1 object which is where we will
 create the real and dynamic value so that CMOpenContainer() knows where to find it, and
 lastly, the CMOpenNewContainer() "..." parameters which are used to create the dynamic
 value.  The TOC #1 pointer, while not strictly necessary as a parameter (because we could
 get it ourselves here), is passed because CMOpenNewContainer() conveniently has it for its
 purposes.
 
 This function returns the refNum of the target container if successful, and NULL if an
 error is reported and the error reporter just happens to return.
*/

static ContainerPtr CM_NEAR openSeparateTargetToBeUpdated(ContainerPtr container,
																													CMObject theTOCObject,
																													va_list targetTypeInitParams)
{
	ContainerPtr	 targetContainer;
	TOCValueHdrPtr theValueHdr;
	CMProperty		 targetProperty;
	CMType				 targetType;
	
	/* Make sure we have the "return target type" handler and call it to get the target		*/
	/* type which, when used will spawn a dynamic value (at least it better!)...					*/
	
	if (container->handler.cmreturnTargetType == NULL ||	/* if not dynamic value type...	*/
			(targetType = CMreturnTargeType(container)) == NULL) {
		ERROR1(CM_err_NoTypeHandler, CONTAINERNAME);				/* ...error (it's required here)*/
		UndoOpen();
		return (NULL);
	}
	
	/* The handler did indeed return a type. Now we will attempt to create a dynamic value*/
	/* for a "real" value belonging to CMTOCTargetGlobalName property of the TOC object.	*/
	/* The "..." parameters of CMOpenNewContainer() are passed along to CMNewValue() as 	*/
	/* the initialization parameters for the dynamic value.																*/
			
	targetProperty = CMRegisterProperty((CMContainer)container, (CMGlobalName)CMTOCTargetGlobalName);
	if (targetProperty != NULL)											/* ...create a dynamic value...				*/
		theValueHdr = (TOCValueHdrPtr)CMVNewValue(theTOCObject, targetProperty, targetType,
																							targetTypeInitParams);
	else
		theValueHdr = NULL;

	if (theValueHdr == NULL) {											/* if value wasn't created...					*/								
		UndoOpen();																		/* ...we're through										*/
		return (NULL);
	}
	
	if (!IsDynamicValue(theValueHdr)) {							/* make sure value is dynamic					*/
		ERROR1(CM_err_NotDynamicValue, CONTAINERNAME);/* (thought you could slip one by?)		*/
		UndoOpen();
		return (NULL);
	}
	
	container->pointingValue = (CMValue)theValueHdr;/* remember to release value at close	*/	
	
	/* Open the target container for reading only.  The dynamic value is passed as the		*/
	/* "attributes" refCon with the predefined container type assumed to be registered 		*/
	/* with the special handler package used for the remote accessing purpose.  It will		*/
	/* expect the attributes to be this dynamic value when passed to its open handler. 		*/
	/* The open handler will use it to create its refCon and all target container I/O 		*/
	/* will be to this value.  Being dynamic, that will lead to the dynamic value's 			*/
	/* handler package which will access the target container.														*/
	
	/* The target container can itself be an updating container with its own updates to 	*/
	/* be applied to a yet "deeper" target.  To be able to do this, ALL targets need to 	*/
	/* be able to get at the top-most container (the one we're openeing here).  This is 	*/
	/* done using the updatingContainer pointer.  The openingTarget switch allows this to */
	/* happen. 	It is interrogated by openContainer() to propagate the top-most updater's */
	/* updatingContainer value "down" as each target is opened.  The top-most's 					*/
	/* updatingContainer points to itself, so all the lower level target's 								*/
	/* updatingContainer will point there as well.																				*/
		
	container->openingTarget = true;								/* target gets our updatingContainer	*/
	
	targetContainer = (ContainerPtr)CMOpenContainer((CMSession)container->sessionData, 
																									(CMRefCon)theValueHdr,
																									(CMGlobalName)CMTargetHandlersTypeName,
																									0);
	container->openingTarget = false;								/* set switch to its "natural" state	*/
	if (targetContainer == NULL) {
		UndoOpen();
		return (NULL);
	}
	
	/* Fix up the updating container so that it uses the target's TOC and that the 				*/
	/* starting ID seed is properly set.																									*/
	
	connectTargetToItsUpdater(container, targetContainer);
	
	return (container);															/* user can now start updating!				*/
}


/*-----------------------------------------------------------------------------------*
 | openAppendedTargetToBeUpdated - open targets for a new update-by-append container |
 *-----------------------------------------------------------------------------------*
 
 If we are updating-by-append (useFlags is kCMUpdateByAppend), the intent is to record
 updates in a separate TOC appended to an existing target container.  The way we do this
 is to treat the target container as an unstructured "file" that is to be opened for
 converting!  Thus CMOpenNewContainer() (this routine's only caller) opens the new
 container as if kCMConverting was specified (it actually sets that useFlag itself).
 
 Remember that you (as a garden variety user) can call CMOpenNewContainer() with the
 useFlags set to kCMConverting and do CMDefineValueData()'s to define values for a TOC
 that will be appended to the "file".  Isn't that just what we want to do?  We want to
 append a distinct TOC on to the end of the "file" which "just happens" to be a container!
	
 Once this container is opened, we have to add a "pointing" value belonging to a special
 property of TOC #1.  The value will essentially be the same as an embedded value that
 defines the target container (i.e., the thing we treated as a unstructured "file" for the
 purposes of opening it for converting) as its value data.  It will be used the same as
 embedded values in that it will be used by special container I/O handlers when we open the
 "file" as the target container.
	
 This value is the "moral equivalent" of the value that openSeparateTargetToBeUpdated()
 creates above when it is called with kCMUpdateTarget to update a separate container.
 There the value has a type which would spawn a dynamic value.  When the target is opened
 there, it uses the same handler package we will use here.  In that case the operations
 will lead to the dynamic value's handler package.  Here it leads to the "converting"
 container handler package, i.e., the container I/O handlers originally passed to
 CMOpenContainer().  [still with me?]
	
 In summary, for updating-by-append or for separate container updating, the container being
 opened will contain a value which "points" to the target container that will be updated.
 For updating-by-append we then call openTargetContainer(), the same routine that
 CMOpenContainer() and openSeparateTargetToBeUpdated() calls, to upen the target(s) pointed
 to by the pointing values, and apply the updates for layered appended containers. 
 
 Of course, the first call to openTargetContainer() will find the pointing value we create
 here.  That will then cause the target to be opened.  If it has a pointing value it will
 open its target, and so on.
 
 This function takes as input the parent container, newly created by CMOpenNewContainer()
 as a converting container and a pointer to the TOC #1 object which is where we will
 create the pointing value so that CMOpenContainer() knows where to find it.  The TOC #1
 pointer, while not strictly necessary as a parameter (because we could get it ourselves
 here), is passed because CMOpenNewContainer() conveniently has it for its own purposes.

 This function returns the refNum of the target container if successful, and NULL if an
 error is reported and the error reporter just happens to return.
*/

static ContainerPtr CM_NEAR openAppendedTargetToBeUpdated(ContainerPtr container,
																													CMObject theTOCObject)
{
	ContainerPtr	 targetContainer;
	TOCValueHdrPtr theValueHdr;
	CMProperty 		 targetProperty;
	CMType		 		 targetType;

	/* Define the special value for the special TOC #1 property that will point to the		*/
	/* target container. Since the target is the same as the one we opened for converting,*/
	/* the value data for the value is the entire container's limits, i.e., offset 0 with */
	/* a size equal to the current physical EOF.																					*/
	
	targetProperty = CMRegisterProperty((CMContainer)container, (CMGlobalName)CMTOCTargetGlobalName);
	if (targetProperty != NULL) {
		targetType = CMRegisterType((CMContainer)container, (CMGlobalName)CMTargetContainerName);
		if (targetType != NULL)
			theValueHdr = (TOCValueHdrPtr)CMNewValue((CMObject)theTOCObject, targetProperty, targetType, 1);
		else
			theValueHdr = NULL;
	}
	
	if (theValueHdr == NULL) {											/* if value wasn't created...					*/								
		UndoOpen();																		/* ...we're through										*/
		return (NULL);
	}
	
	/* Define the value data in the "converting" container...															*/
	
	container->useFlags |= kCMConverting;						/* allow this to happen!							*/
	CMDefineValueData((CMValue)theValueHdr, 0L, (CMSize)container->physicalEOF);
	container->useFlags &= ~kCMConverting;
	
	CMReleaseValue((CMValue)theValueHdr);						/* keep use count correct 						*/
	
	/* Use the value we just defined to open the target container. It itself may contain 	*/
	/* appended targets!  This leads to a recursion through CMOpenContainer().  See				*/
	/* openTargetContainer() comments for further details. 																*/
	
	targetContainer = openTargetContainer(container, (TOCObjectPtr)theTOCObject);
	if (targetContainer == NULL) {
		UndoOpen();
		return (NULL);
	}
	
	return (targetContainer);
}


/*---------------------------------------------------------*
 | prepareNewContainerForUpdating - do what the name says! |
 *---------------------------------------------------------*
 
 If a new updating container is opened (useFlags is kCMUpdateByAppend or kCMUpdateTarget),
 then by the time this routine is called all target container's will have been opened and
 updates, if any, applied to them.  Here we must do the final preparations on the newly
 created updater to make it ready for recording updates.  This consists of the following
 actions:
 
		1. Creating a new "min seed".
		
		2. Setting the touchTheValue boolean to true, also in the container control block so
			 that all further updates to old objects are processed as touched entries.  That way
			 It's been false up to this point so the "old" (i.e., previously applied) updates
			 weren't processed as new updates (not a good thing to happen).
 
 The comments in the code below fully explain these items and the "how" and "why" we're
 doing what we're doing.  Remember that we're in the context of creating a new updating
 container.  As with connectTargetToItsUpdater(), there isn't much code here, but there's
 a lot of comments explaining it!  I'm having soooo much "fun" writing all these
 comments!
*/

static void CM_NEAR prepareNewContainerForUpdating(ContainerPtr container, 
																									 CMBoolean autoGeneration)
{
	ContainerPtr immediateTarget;
	CMGeneration	nextGeneration;
	
	/* New objects in the new updating container must have IDs that continue with the next*/
	/* available after all lower level updates have been applied. The way this is done is */
	/* to inherit the starting ID seed from the immediate target.  That's the next free 	*/
	/* ID seed. 																																					*/
	
	/* The ID seed value is incremented for each new object.  It is thus not constant. 		*/
	/* But we need its initial value as a constant.  It is referred to as the "min seed".	*/
	/* Initially, for all new containers, the initial seed and the min seed are the same.	*/
	/* However, for updating containers the min seed is changed (here) to be the initial	*/
	/* next free seed (i.e., the final seed of the read-only immediate target).  At the		*/
	/* end of updating, we will essentially have the min seed and max seed range of all		*/
	/* new object IDs in the new container.																								*/
	
	/* Knowing the ID range is important (why do you think we want the min?)!		*/
	/* This is because we have a "chicken-and-egg" problem here.  In order to get at a 		*/
	/* target container we had to construct a pointing value for an object.  It has to be	*/
	/* done to get at the target. But we don't know what the next ID seed should be until */
	/* we opened the target.  Get the picture?																						*/
	
	/* The only thing we can do is use the default initial seed for the pointing value's	*/
	/* object as if it were a pristine new container, which it is.  Other objects can			*/
	/* also be created if we are creating a dynamic value.  The dynamic value's type, 		*/
	/* created by the "return target type" handler, can get as complex as it wants.  It		*/
	/* may even create base types for layered dynamic value handlers.  It's not practical	*/
	/* to hunt all these objects down to change their IDs once we know it.  That would		*/
	/* involve moving the objects to keep the TOC directory data structure correct.				*/
	
	/* Such objects are only used to get at the target. They are not involved in updating	*/
	/* the target.  We always guarantee that the min ID seed is greater than the highest 	*/
	/* private object in the updating, i.e., the current container, even if that means 		*/
	/* creating a "hole" (if the highest ID of the target is less than the highest 				*/
	/* private object ID).  We do this for each layer. IDs in the target can be less than	*/
	/* the min.  It only "counts" when the object belongs to its own container.						*/
	
	/* After explaining all this it should be pointed out that the min seed isn't really	*/
	/* used that much!  It had a bigger use when originally designed.  But it is useful 	*/
	/* whenever we have to see if an object ID belongs to the private TOC.								*/
	
	immediateTarget = (ContainerPtr)cmGetNextListCell(container);
	MinSeed = immediateTarget->nextUserObjectID;		/* ...use target's ID seed for our min*/
	if (MinSeed < container->nextUserObjectID)			/* ...if we have more IDs	that target	*/
		MinSeed = container->nextUserObjectID;				/* ...use ours (creates a hole!)			*/
	
	container->nextUserObjectID = MinSeed;					/* min is also the initial ID seed		*/
	Seed = MinSeed;
	
	/* The touchTheValue switch in the container is always initialized to false.  When		*/
	/* true, all updates to old objects are processed as new updates and as such must be	*/
	/* appropriately processed as "touched".  This is the time to set it to true, since		*/
	/* we now at the top-most container. From here on, all new updates must be remembered.*/
	
	container->touchTheValue = true;							/* set to remember all new updates			*/

	if (autoGeneration) {
		nextGeneration = immediateTarget->generation + 1;
		container->generation = nextGeneration;
		container->tocIDSeedValue->theValueHdr->generation = nextGeneration;
		container->tocIDMinSeedValue->theValueHdr->generation = nextGeneration;
		container->tocObjValue->theValueHdr->generation = nextGeneration;
		container->tocContainerValue->theValueHdr->generation = nextGeneration;
		container->spaceDeletedValue->theValueHdr->generation = nextGeneration;
		container->tocNewValuesValue->theValueHdr->generation = nextGeneration;
		((TOCValueHdrPtr)(container->pointingValue))->generation = nextGeneration;
		if (container->freeSpaceValueHdr)
			container->freeSpaceValueHdr->generation = nextGeneration;
	}
}


/*--------------------------------------------------*
 | CMOpenNewContainer - open a container for output |
 *--------------------------------------------------*
 
 The container corresponding to the specified typeName is opened for writing or for
 updating a separate container.  This is similar to opening for reading or updating done
 by CMOpenContainer() except that here a new and empty container is opened.  
 
 A minimum TOC containing the predefined registered types and properties is created along
 with the special TOC object 1 with its seed and offset properties.
 
 There are a few differences from reading!  Here the useFlags may be 0, kCMConverting,
 kCMUpdateByAppend, or kCMUpdateTarget.  In read it was just 0 or kCMReuseFreeSpace.
 
 The kCMReuseFreeSpace may also be specified in conjunction with the others just mentioned.
 Here it only means that free space will try to be reused when possible.  Unlike 
 CMOpenContainer(), you always can read and write, so it does not imply "updating" per-se.
 As will be discussed shortly, updating here is indicated by kCMUpdateByAppend or
 kCMUpdateTarget to have a special meaning.
 
 Free space is always kept track of on a list.  It takes the form form of standard TOC
 entries for TOC ID 1, property CM_StdObjID_TOC_Free.  Only space greater than a TOC entry
 size is remembered since each free list entry cost at least a TOC entry itself.
 
 With kCMConverting, the physical container is assumed to be a sequence of bytes that
 the API user wants to convert to container format.  S/he uses CMDefineValueData() to
 create values for objects in the bytes.  Other than that it is the same as a normal
 opening for writing.  All new stuff, including the TOC is written at the end of the
 existing stuff.
 
 For kCMUpdateByAppend or kCMUpdateTarget, all updates to a "target" container are recorded
 in the container being opened.  Future opens of this container, with CMOpenContainer() 
 will apply the updates to the targer to bring it "up-to-date" while it is open.
 
 If kCMUpdateByAppend is specified, then the container is opened for update-by-append.  All
 updates are appended to the existing container and an additional TOC is layered on to the
 end of the container when closed.  Each time the container is opened and then closed for
 update-by-append, the new updates and a new TOC are appended.  Whenever such a container
 is opened (by any mode), all the updates are applied appropriately to the original
 container.
 
 Using kCMUpdateTarget is similar to kCMUpdateByAppend, but the updates are recorded in a
 new container.  In this case the "target" container is accessed through a dynamic value.
 The dynamic value is created by CMOpenNewContainer() by using a type that, when used to 
 define a value, will spawn the dynamic value.  The type is defined by a "return target
 type" handler which must be part of the standard container handler package associated with
 the container typeName.  CMOpenNewContainer() actually does a CMNewValue() using the
 returned type to create the dynamic value.  As with CMNewValue(), the "..." parameters
 passed to CMOpenNewContainer(), are passed along to CMNewValue().  See CMNewValue() for
 further details on dynamic values and the "..." parameters.
  
 As with reading, the attributes have the same meaning with respect to the handlers that
 use it.
 
 For creating embedded containers, the container typeName must be associated with a set
 of container handlers that support the "return parent value" handler.  Such a handler
 returns the parent value refNum whose data will contain the embedded container.  No value
 value data must exist for the value when the container is opened.  This value gets its
 data by using it as an embedded container instead of doing CMWriteValueData()'s to it.
 
 The value belongs to the parent of the embedded container.  Just as in reading any number
 of embedded containers can be opened (memory permitting).  Also embedded containers can
 be opened for writing even deeper embedded containers!   The effect is that a tree of
 nested containers can be opened and written without restriction.  However, when a
 CMCloseContainer() is done on a parent container, all of its descendents will also be
 closed.  Just as in read.
 
 It is an error to open an embedded container for writing if it's value belongs to a 
 container open for reading.
*/

CMContainer CM_VARARGS CMOpenNewContainer(CMSession sessionData,
																					CMRefCon attributes,
																					CMconst_CMGlobalName typeName, 
																					CMContainerUseMode useFlags,
																					CMGeneration generation,
																				/*CMContainerFlags*/ CM_ULONG containerFlags, ...)
{
	/* The last param is now passed as a long, not as CMContainerFlags, which is a
	   short. This is because the ANSI standard and CFM68K do not support passing
		 a type like short, which will be promoted in size when pushed on the stack,
		 as a parameter to va_start. --jpa 5/24/95 */
		 
	va_list		  targetTypeInitParams;
	CMContainer container;
	
	va_start(targetTypeInitParams, containerFlags);
	container = CMVOpenNewContainer(sessionData, attributes, typeName, useFlags, generation, 
																	containerFlags, targetTypeInitParams);
	va_end(targetTypeInitParams);
	
	return (container);
}


/*---------------------------------------------------*
 | CMVOpenNewContainer - open a container for output |
 *---------------------------------------------------*
 
 This routine is the same as CMOpenNewContainer() above, except that the kCMUpdateTarget
 dynamic value data initialization (i.e., "...") parameters are given as a variable
 argument list as defined by the "stdarg" facility.
 
 This routine assumes the caller sets up and terminates the variable arg list using the
 "stdarg.h" calls as follows:
 
 			#include <stdarg.h>
			
 			callersRoutine(args, ...)
			{
				va_list targetTypeInitParams;
				
				- - -
				
				va_start(targetTypeInitParams, args);
				container = CMVOpenNewContainer(sessionData, attributes, typeName, useFlags,
																			  generation, containerFlags, targetTypeInitParams);
				va_end(targetTypeInitParams);
				
				- - -
			}
			
 See CMOpenNewContainer() for further details.
*/

CMContainer CM_FIXEDARGS CMVOpenNewContainer(CMSession sessionData,
																						 CMRefCon attributes,
																						 CMconst_CMGlobalName typeName, 
																						 CMContainerUseMode useFlags,
																						 CMGeneration generation,
																						 CMContainerFlags containerFlags, 
																						 va_list targetTypeInitParams)
{
	ContainerPtr 	 container;
	TOCValueBytes	 valueBytes;
	TOCValueHdrPtr theValueHdr;
	CMObject			 theTOCObject;
	CM_USHORT			 testUseFlags;
	CMBoolean			 autoGeneration = false;

	if (sessionData == NULL) return (NULL);					/* NOP if not initialized!						*/
	
	if (generation < kCMDefaultGeneration) {				/* make sure of generation number			*/
		if (generation != kCMNextGeneration) {
			SessionERROR3(CM_err_BadGenNbr, cmltostr(generation, 1, false, (char *)SessionScratchBufr), "CMOpenNewContainer", typeName);
			return (NULL);
		}
		autoGeneration = true;
		generation = kCMDefaultGeneration;					/* in case there is no target					*/
	}
	
	#if 0
	if ((useFlags & kCMReuseFreeSpace) != 0) {			/* what is s/he updating?							*/
		SessionERROR1(CM_err_BadWriteUse, typeName);	
		return (NULL);
	}
	#endif
	
	testUseFlags = (CM_USHORT)((CM_USHORT)useFlags & (kCMConverting | kCMUpdateByAppend | kCMUpdateTarget));
	if (testUseFlags != 0									&&				/* check for ambiguous flags settings	*/
			testUseFlags != kCMConverting 		&& 				
			testUseFlags != kCMUpdateByAppend &&
			testUseFlags != kCMUpdateTarget) {
		*SessionScratchBufr = 0;
		if (useFlags & kCMConverting) strcpy((CM_CHAR *)SessionScratchBufr, "kCMConverting");
		if (useFlags & kCMUpdateByAppend) {
			if (*SessionScratchBufr) strcat((CM_CHAR *)SessionScratchBufr, ", ");
			strcat((CM_CHAR *)SessionScratchBufr, "kCMUpdateByAppend");
		}
		if (useFlags & kCMUpdateTarget) {
			if (*SessionScratchBufr) strcat((CM_CHAR *)SessionScratchBufr, ", ");
			strcat((CM_CHAR *)SessionScratchBufr, "kCMUpdateTarget");
		}
		SessionERROR2(CM_err_AmbiguousUseFlags, typeName, SessionScratchBufr);
		return (NULL);
	}
	
	useFlags |= kCMWriting;													/* we're always writing here					*/
	
	/* Open the container for output or update. The presence of of the kCMWriting flag 		*/
	/* tells openContainer() was called from here as opposed to CMOpenContainer().  The		*/
	/* kCMConverting flag has presedence over kCMWriting so we know to upen an existing		*/
	/* container (or whatever) for update.	Note, that for appened container updating, we	*/
	/* turn on kCMConverting to "fake" out openContainer() into setting things up to put	*/
	/* the TOC at the end of an already existing "file", which just happens to be the			*/
	/* container we want to append the updating TOC to.																		*/

	if (useFlags & kCMUpdateByAppend)								/* if appending a "new" container...	*/
		useFlags |= kCMConverting;										/* pretend to convert "old" container */
	
	if ((container = openContainer(attributes, typeName, useFlags, generation,
																 containerFlags, (SessionGlobalDataPtr)sessionData)) == NULL)
		return (NULL);
	
	if (useFlags & kCMUpdateByAppend)								/* if we turned on kCMUpdateByAppend	*/
		useFlags = container->useFlags &= ~kCMConverting;	 /* ...turn it back off now				*/
	
	/* Define the TOC object that contains the starting object ID (MinUserObjectID for 		*/
	/* new containers) and the generation number (supplied by caller). The starting ID is	*/
	/* updated with the next free (user) ID number available to the container just prior	*/
	/* to writing the TOC out to the container.  This is picked up when we read the 			*/
	/* container to be able to continue numbering with unique ID's.	tocIDSeedValue in			*/
	/* our container control block will contain the pointer to the value for this object.	*/
	/* Note the object is flagged as "protected" to prevent its deletion (and any other		*/
	/* monky business we can think of in the future).																			*/
		
	cmSetValueBytes(container, &valueBytes, Value_Imm_Long, MinUserObjectID, sizeof(CM_ULONG));
	theTOCObject = (CMObject)cmDefineObject(container, CM_StdObjID_TOC, CM_StdObjID_TOC_Seed,
								 													CM_StdObjID_TOC_Type, &valueBytes,
																					generation, kCMImmediate,
								 													(ObjectObject | ProtectedObject), &theValueHdr);
	if (theTOCObject == NULL) {											/* huh?																*/
		UndoOpen();
		return (NULL);
	}
	container->tocIDSeedValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	
	/* The minimum seed value is always initially equal to the seed value.  It is used 		*/
	/* when applying updates to a target container. For a newly created updating container*/
	/* the min seed value is set as the same as the initial seed value of the updating		*/
	/* container and remains constant. The seed, of course, always has the next available */
	/* ID and thus is not constant.  The min is used when applying updates to a target to	*/
	/* be able to suppress objects in the updating container with IDs less than the min 	*/
	/* from being applied to the target.  Such objects can occur during the opening of 		*/
	/* new updating containers before we can get at the target to know what the seed			*/
	/* should be (a "chicken-and-egg" problem).																						*/
	
	cmSetValueBytes(container, &valueBytes, Value_Imm_Long, MinUserObjectID, sizeof(CM_ULONG));
	theTOCObject = (CMObject)cmDefineObject(container, CM_StdObjID_TOC, CM_StdObjID_TOC_MinSeed,
								 													CM_StdObjID_TOC_Type, &valueBytes,
																					generation, kCMImmediate,
								 													(ObjectObject | ProtectedObject), &theValueHdr);
	if (theTOCObject == NULL) {											/* huh?																*/
		UndoOpen();
		return (NULL);
	}
	container->tocIDMinSeedValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	
	/* To allow the user to treat the entire TOC itself as an object, we need to define a */
	/* property of the TOC object with the offset and size.  It is used as an additional 	*/
	/* check for reading containers.  More importantly we need this to allow CMUseValue() */
	/* and CMGetValueSize() to operate.  CMGetValueSize() will always return 0 while 			*/
	/* writing, but the actual size when reading.  The bad part is we have a "chicken and */
	/* egg" problem setting the value.  We can't set the value until we completed writing */
	/* the TOC.  But by that time the TOC object 1 has been written to the container. This*/
	/* means we have to back patch the size in the container.  Thus we use the pointer to */
	/* the value as a signal to remember where we write that value and when to capture the*/
	/* value offset for the back patch.	 Note, for this property the value is NOT to be 	*/
	/* immediate since we will be filling an offset.																			*/
	
	cmSetValueBytes(container, &valueBytes, Value_NotImm, 0, 0);		/* fill in on write		*/
	cmDefineObject(container, CM_StdObjID_TOC, CM_StdObjID_TOC_Object,
								 CM_StdObjID_TOC_Type, &valueBytes, generation, 0,
								 (ObjectObject | ProtectedObject), &theValueHdr);
	container->tocObjValue 	 = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	
	/* To allow us to view the entire container as a single object (from 1st byte to the	*/
	/* end of the label), we need to define a property of the TOC object with an offset 	*/
	/* equal to the first available byte (0), and a size which cannot be determined until */
	/* we write the entire TOC (so we save a pointer to the value entry as we just did 		*/
	/* above).  This value could be looked at by the user in conjunction with the total 	*/
	/* deleted space value (defined below) to get a fragmentation estimate.  However, we 	*/
	/* need it for update-by-append edits so that an editing container can "get at" the 	*/
	/* container to be edited when we have layered TOCs.  Note, that the offset for this	*/
	/* value will aways be 0.  The size changes due to initial container creation being		*/
	/* initiated here, or by updates initiated by CMOpenContainer().											*/
	
	cmSetValueBytes(container, &valueBytes, Value_NotImm, 0, 0);		/* fill in on write		*/
	cmDefineObject(container, CM_StdObjID_TOC, CM_StdObjID_TOC_Container,
								 CM_StdObjID_TOC_Type, &valueBytes, generation, 0,
								 (ObjectObject | ProtectedObject), &theValueHdr);
	container->tocContainerValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/
	
	/* When data values are deleted we keep track of the total amount of deleted space in	*/
	/* the TOC.  This will be written as one of the TOC object property values.  That			*/
	/* value is created here and now...																										*/
	
	cmSetValueBytes(container, &valueBytes, Value_Imm_Long, 0, sizeof(CM_ULONG));
	cmDefineObject(container, CM_StdObjID_TOC, CM_StdObjID_TOC_Deleted,
								 CM_StdObjID_TOC_Type, &valueBytes, generation, kCMImmediate,
								 (ObjectObject | ProtectedObject), &theValueHdr);
	container->spaceDeletedValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value	*/	

	/* The TOC for an updating container is divided into the following sections:					*/
	
	/*		1. The "private" TOC   																													*/
					
	/*			 Stuff belonging only to the updating container and thus have object ID's < 	*/
	/*       min seed.																																		*/
	
	/*  	2. The "new properties for old objects" TOC																			*/
	
	/*			 These are values with ID's < min seed too, but "belong" to the target 				*/
	/*			 container.  They we're created when the updating container was originally 		*/
	/*			 created.																																			*/
	
	/*	  3. The "new objects" TOC																												*/
	
	/*			 New objects added to the target container again when the updating container	*/
	/*  		 was originally created.																											*/
	
	/* In a new updating container, which we're working on here, we create special 				*/
	/* property of TOC #1, CM_StdObjID_TOC_NewValuesTOC, that will hold the offset and 		*/
	/* size of parts (2) and (3) of the TOC.  Then, when a TOC is opened for reading, the */
	/* TOC reader knows how to stop reading the private part (1). The remaining parts are */
	/* read in a second read done in openTargetContainer() when opening updating 					*/
	/* containers for reading.  In that case we have already got the special property  		*/
	/* with its value pointer in tocNewValuesValue in the container control block.  For 	*/
	/* writing we create it here for the future reading to find and for us to set when 		*/
	/* the TOC sections (2) and (3) are finally written at close time.										*/

	if (UPDATING(container)) {												/* create only if updating...				*/
		cmSetValueBytes(container, &valueBytes, Value_NotImm, 0, 0);	/* fill in on write		*/
		cmDefineObject(container, CM_StdObjID_TOC, CM_StdObjID_TOC_NewValuesTOC,
									 CM_StdObjID_TOC_Type, &valueBytes, container->generation, 0,
									 (ObjectObject | ProtectedObject), &theValueHdr);
		container->tocNewValuesValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		theValueHdr->valueFlags |= ValueProtected;			/* don't allow writing to this value*/
	}
	
	/* If we have a "normal" new container being opened, return the container pointer as	*/
	/* the refNum.  If we're going to use this container to update another target 				*/
	/* container, then we still have to open the target.  This is done separately by			*/
	/* openSeparateTargetToBeUpdated() for kCMUpdateTarget useFlags.  It will use the			*/
	/* CMOpenNewContainer() "..." parameters (if any) which we must pass specially.				*/
	
	/* For useFlags kCMUpdateByAppend, we call openAppendedTargetToBeUpdated() which is 	*/
	/* a continuation of the open we just did here.  It adds a "pointing" value to this		*/
	/* container and then calls openTargetContainer() just as CMOpenContainer() would to	*/
	/* apply updates to possibly appended targets.																				*/
	
	if (container->useFlags & kCMUpdateTarget) {		/* if updating separate target...			*/
		container = openSeparateTargetToBeUpdated(container, theTOCObject, targetTypeInitParams);
	} else if (container->useFlags & kCMUpdateByAppend)	/* if update-by-append...					*/
		container = openAppendedTargetToBeUpdated(container, theTOCObject);
	
	if (container && UPDATING(container))						/* do final preparations for updating	*/
		prepareNewContainerForUpdating(container, autoGeneration);
		
	return ((CMContainer)container);								/* return original or target container*/
}

/*-------------------------------------------------*
 | deleteUnprotected - delete object not protected |
 *-------------------------------------------------*
 
 Since any Bento object can be access using CMGetNextObject etc, Bento by itself has no
 concept of garbage object. Bento supports garbage collection where the application can
 declare objects to be kept. Bento would also keep all objects referenced from these
 objects. This is done recursively to form a network of objects to be kept. After that
 every other objects not protected will be deleted.
 
 This is an action routine for the object interator, cmForEachObject(). It will delete
 any object considered to be garbage by the above definition.
*/

static void deleteUnprotected(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container = (ContainerPtr)refCon;
	
	/* now we do garbage collect property and type names																	*/
	if (theObject->objectFlags & (DeletedObject | ProtectedObject | DynamicValuesObject))	
		return;							/* do not delete property or type, or object that is protected	*/																	
	/* for properties and value name, only delete those in the same container						  */
	/* one reason is that if the name is reused later, we don't need extra disk space     */
	/* another reason is that there is a bug and it may fail if we try to delete property */
	/* name in another container																												  */
	if (theObject->objectFlags & (PropertyObject | TypeObject))
		if (theObject->container != container)
			return;
	if (cmGetObjectProperty(theObject, CM_StdObjID_ObjReferences))
		return;							/* do not delete any reference object														*/
	theObject->useCount = 1;	/* to ensure we can delete it																*/
	CMDeleteObject((CMObject)theObject);
}

/*--------------------------------------------------------------------------------------*
 | deleteGarbage - Delete all unprotected objects if we want to garbage collect objects |
 *--------------------------------------------------------------------------------------*
 
	Delete all unprotected objects if we want to garbage collect objects.
	
	Since container->deleteGarbage is true only if we have protected the objects, so all
	the unprotected objects can be considered to be useless and can be deleted.
*/

static void deleteGarbage(ContainerPtr container)
{
		if (container->deleteGarbage) {												/* we don't want any garbage  */
			if (!(SESSION->aborting))
				cmForEachObject(container->toc, CM_StdObjID_MinGeneralID, MAXUSERID,
									  (void *)container, deleteUnprotected);
			container->deleteGarbage = false;										/* so we don't do it again		*/
		}
}			
/*---------------------------------------------------------------------------*
 | closeTheGap - adjust value offset when a gap in embedded value is removed |
 *---------------------------------------------------------------------------*
 
 When we take a slice of the embedded value and return the space to the embedding container,
 the offset values may be off since the offset is the logical offset from the beginning
 of the embedded value and now the distance from the beginning has been changed. 
 So we need to adjust the offsets.
 
 This is an action routine for the object interator, cmForEachObject(). It will be
 used by returnToParent. The refcon is the TOCValue of the slice to be cut off.
 So we go through all the values in the object, and if any values is located
 beyond the slice, we will reduce its offset by the length of the slice.
*/

static void closeTheGap(TOCObjectPtr theObject, CMRefCon refCon)
{
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue;
	CM_ULONG			 gapOffset = ((TOCValuePtr)refCon)->value.notImm.value;
	CM_ULONG			 gapSize = ((TOCValuePtr)refCon)->value.notImm.valueLen;

	theProperty = (TOCPropertyPtr)cmGetListHead(&(theObject)->propertyList);
	while (theProperty) {													/* go thru all properties in the object	*/
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		while (theValueHdr) {												/* go thru all values in the property		*/
			if ((theValueHdr->valueFlags & ValueImmediate) == 0) {			/* ignore immediates	*/
				theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
				while (theValue) {											/* go through all value segments				*/
					if (theValue->value.notImm.value > gapOffset)
					/* if the segment is beyond the gap, we need to adjust for closing the gap 		*/
						theValue->value.notImm.value -= gapSize;
					theValue = (TOCValuePtr)cmGetNextListCell(theValue);		/* next value segment */
				}
			}
			theValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr); /* next value header*/
		}
		theProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);		/* next property		*/
	}
}

/*-------------------------------------------------------------------*
 | returnToParent - return embedded space to the embedding container |
 *-------------------------------------------------------------------*
 
 Reusing free space in embedded container is not a easy task. First of all, only space
 on the top level container will affect the bottom line - the total disk space. So any
 resue of free space in the embedded container will only affect disk space indirectly.
 
 If the embedded container can reuse it free space efficiently, then at least the
 embbeded container will not grow so fast and would ask for less space from the
 embedding container. However, consider the case where an embedded container updates
 another embedded container. The updating embedded container cannot use the free
 space from the target embedded container. The target embedded container is open
 for reading only, and it is in a different container and so it is not usable
 for the updating container. So the free space is useless. We may use merging so
 that the two embedded container becomes one container, and if it is one container
 then we can reuse the free space. However, merging only occurs at the close time.
 Before that where most of the request for space are made the two embedding
 container are still separate and the free space cannot be used. Well, at least
 we can reuse the free space during closing time. However, a big piece of what is
 written at closing time is the TOC which need to be written in a single segment
 and at the end, so we cannot use the free space. Also a big piece of free space
 that is supposed to go free at closing time of merged container is the TOC of
 the target container. However, for crash proof file we cannot use the target TOC
 space until the new TOC has been written. There is another complication of using
 free space in merged embedded containers. Since eventually what is remained of
 the free list needed to be written to the TOC, we want the free list to be in offsets
 of the merged containers before we do the closing so that it will be written on
 the TOC correctly. On the other hand, for the free space to be reused, we want to
 keep it as offset before the merging. These contradictory requirements.
 
 So for the reuse of embedded free space, we shall use a completely different strategy.
 When the container is closed, we shall take the free space in the embedded container
 and return the free space to the embedding container and it will becomes the free
 space in the embedding container. When the embedded container ask space, it will not
 find it in the embedded container so it will be appending to the embedded container.
 Effectively it is getting the disk space from the embedding container. So in effect
 instead of each embedded container managing its own pool of free space, the free
 space is in a big pool in the embedding container where it can be used more efficiently
 be everyone, both the embedded containers and the embedding containers.
 
 This scheme is not without its drawbacks. To returns free space to the embedding
 container, it is necessary to cut slices off from the embedded values and give it
 back to the embedding containers. However since offsets in embedded containers are
 measured logicailly from the beginning of the embedded containers, slicing off space
 means that all these offset need to be readjusted. This is the purpose of the routine
 close the gap. We need to go through all the objects, properties, values to look at
 each value segment to adjust them. This is a lot of overhead. Furthermore, this means 
 after slicing off the unused space in the embedded containers, we end up with lots of 
 value segments in the embedded values. Also the present of lots of free space slice in 
 the embedding container free space space means that newly written embedded values tends 
 to have many value segments as it reuse the free space. So fragmentation is a problem. 
 
*/

static void returnToParent(ContainerPtr container, ContainerPtr targetContainer, TOCValueHdrPtr targetValue)
{
	TOCValuePtr			theValue, nextValue, freeValue;
	TOCObjectPtr	 	theTOCObject;
	TOCPropertyPtr 	theFreeListProperty;
	TOCValueHdrPtr 	freeSpaceValueHdr;
	TOCValueHdrPtr 	parentValue = container->embeddedValue;
	CM_ULONG				offset, size, prependSize = container->mergeInSize;
	CM_LONG					overFlow;

	/* can only be done if both embedded and embedding container reuse free space					*/
	if ((container->useFlags & kCMReuseFreeSpace) && (parentValue->container->useFlags & kCMReuseFreeSpace)) {
		deleteGarbage(container);			/* garbage collect so that so can return more space		*/
		cmCopyTmpFreeToTOC(container);							/* make temporary free space permanent	*/
		freeSpaceValueHdr = container->freeSpaceValueHdr;
		if (freeSpaceValueHdr) {
			/* Before we delete parent value we must flush the embedded container to make sure*/
			/* all the data got written because we are writting directly to parent						*/
			/* value without going through embedded handler																		*/
			if (container->handler.cmfflush != NULL) 	/* flush if we can 											*/
				if (CMfflush(container) != 0)						/* ...if flush handler provided					*/
					ERROR1(CM_err_FlushErr, CONTAINERNAME);/* ...report error if it occurred			*/
			theValue = (TOCValuePtr)cmGetListTail(&freeSpaceValueHdr->valueList);
			while (theValue) {												/* go through each free value segment		*/
				nextValue = (TOCValuePtr)cmGetPrevListCell(theValue);
				size = theValue->value.notImm.valueLen;
				if (size >= MinFragmentSize) {
					offset = theValue->value.notImm.value;
					if (offset >= prependSize) {					/* it belongs to the updating container	*/
						/* We use CMDeleteValueData to cut off a slice, but since we have already			*/
						/* change the offset to account for the merging, we have to change it back		*/
						CMDeleteValueData((CMValue)parentValue, offset - prependSize, size);
						container->logicalEOF -= size;
						container->physicalEOF -= size;
					}
					else {																	/* it belongs to the target container		*/
						overFlow = offset + size - prependSize; /* the free value may cover both		*/
						if (overFlow > 0) {		/* it does, so we also need to adjust update container*/ 
							CMDeleteValueData((CMValue)parentValue, 0, overFlow);
							container->logicalEOF -= overFlow;
							container->physicalEOF -= overFlow;
						}
						else
							overFlow = 0;
						/* now we do it on the target container 																		*/
						CMDeleteValueData((CMValue)targetValue, offset, size - overFlow);
						prependSize -= (size - overFlow);
					}
					container->spaceDeletedValue->value.imm.ulongValue -= size; /* decr free space	*/
					/* Now we need to adjust the offsets affected by deleting the slice							*/
					cmForEachObject(container->toc, MinUserObjectID, MAXUSERID,
													(void *)theValue, closeTheGap);
					if (targetContainer)
						cmClosePastDataGap(targetContainer, offset, size);
					/* We also need to adjust the offset of the free value beyond this one					*/
					freeValue = (TOCValuePtr)cmGetListTail(&freeSpaceValueHdr->valueList);
					while (freeValue != theValue) {
						freeValue->value.notImm.value -= size;
						freeValue = (TOCValuePtr)cmGetPrevListCell(freeValue);
					}

					CMfree(cmDeleteListCell(&container->freeSpaceValueHdr->valueList, theValue));
				}
				theValue = nextValue;
			}
			if (cmIsEmptyList(&freeSpaceValueHdr->valueList)) {
				/* Since we promote all the free space, we delete the free space property				*/
				theFreeListProperty = freeSpaceValueHdr->theProperty;	/* get owning property		*/
				theTOCObject 				= theFreeListProperty->theObject;	/* get owning object (1)	*/
				CMfree(freeSpaceValueHdr);														/* clobber the value hdr	*/
				CMfree(cmDeleteListCell(&theTOCObject->propertyList, theFreeListProperty));
				container->freeSpaceValueHdr = NULL;									/* no more free list			*/
			}
			/* We truncate the handler to inform it the container size has been modified. We	*/
			/* have already changed the size of the embedded value when we called							*/
			/* CMDeleteValueData directly. So the truncation will try to delete space beyond	*/
			/* the end and hence do nothing. This is just so that the embedded container, the	*/
			/* embedded value and the embedded container handler will be in sync regargding		*/
			/* the size of the container.																											*/
			CMftrunc(container, container->physicalEOF);
			container->mergeInSize = prependSize;
		}
	}
}

/*--------------------------------------------*
 | writeTOC - write the TOC for the container |
 *--------------------------------------------*
 
 if keepContainerOpen is true, then after writing the TOC we should update the container
 status since it will be used later. The TOC will also be put on the temporary free list
 since we will be writing a new TOC and not use this TOC when the container is closed.
*/

static void writeTOC(ContainerPtr container, CMBoolean keepContainerOpen, CMBoolean cautious)
{
	/* When the TOC is written out, the TOC needs to be continous and at the end of				*/
	/* the container, this usually means that it needs to be appended at the end.					*/
	/* However if it can be truncated and there is free space at the end, then we can			*/
	/* start at the beginning of the free space and write it there and truncate it.				*/
	/* However, there is an added complexity. There are some temporary free space that		*/
	/* we do not want to reuse until we have written the new TOC and label etc. This is		*/
	/* because if we crashes at that point, we want to at least get back the container		*/
	/* before the update, so until the new one can be written, we want to leave it				*/
	/* intact. The TOC of the container being updated is a prime example of such space		*/
	/* that cannot be reused until the new TOC is being written. Such a TOC is often			*/
	/* located at the end of the file. As a matter of fact they always occur at the end		*/
	/* of the file when the container is being closed. When it is opened for update,			*/
	/* data will be written beyond it. However if there is a lot of free space, then			*/
	/* new data will be written before that TOC and at close time that TOC may still be		*/
	/* the end. However, since we cannot write over that TOC until the new TOC has been		*/
	/* written, it appears that the new TOC must be written after the old TOC and that		*/
	/* means the file will always increase in size. Fortunately, it may be possible that	*/
	/* there may be enough free space before that TOC that can fit the new TOC. In that		*/
	/* case we can write out the new TOC in those free space, and then truncate						*/
	/* afterwards. If this is still confusing, we shall use a diagram to illustrate.			*/
	/* In the following diagrams, we shall use F to mean a unit of free space, D to mean	*/
	/* a unit of free space that can be reused after the new TOC is written, O to mean		*/
	/* space that is occupied, and T is the new TOC to be written and it will take up			*/
	/* 4 unit of space.	X will means space will be truncated afterwards.									*/
	/*																																										*/
	/* We have an exampls where there is no choice but to append the TOC at the end				*/
	/*																																										*/
	/* 	OOFFFFFFFFXXDDDDFFFDDDDDDDFF 																											*/
	/*                              TTTT 																									*/
	/*																																										*/
	/* Here is a simple example where the TOC can start at the last block of free space 	*/
	/*  FFFFFFOOFFFFFFFFFFF																																*/
	/*          TTTTXXXXXXX																																*/
	/*																																										*/
	/* Here is an example where there is a mixture of temp and perm free space but there	*/
	/* is still enough room for us to write the new TOC without distrubing old free data	*/
	/*  FFFFFFOOFFFFFFDDFFFFFFDDDD																												*/																			
	/*          TTTTXXXXXXXXXXXXXX																												*/
	/*																																										*/
	/* Of course we do not know how big the new TOC is, so we may have to write it				*/
	/* twice. If this is a performance problem we have to do a try run of going through 	*/
	/* the TOC to	determine the size first. Note when we try to write the TOC twice, if		*/
	/* the crashes occurs after the first TOC is written, the end result is that we have	*/
	/* the new content even though we are not doing it in the most space conserving				*/
	/* manner.																																						*/
	/*																																										*/
	/* The way we determine how much free space at the end so that we can attempt to			*/
	/* write TOC in free space instead of appending is first to find the last block of		*/
	/* free space, then we add in the temporary free space, and we look at the last 			*/
	/* block of free space again. From the difference, we can tell how much room we have	*/
	/* to try to write the new TOC in.																										*/

	/* Late breaking news. The above may be correct but we can be more agressive.					*/
	/* In the discussion, we write the TOC to the end, and then we try to write the TOC		*/
	/* in the free space but we avoid the temp free space because if we crash, we still		*/
	/* want the temp free space intact. However, if after we write the TOC at the end,		*/
	/* we also write a new label, then the file is safe with the new label, then we can		*/
	/* afford to write over the temp free space because they are not used in the new TOC	*/
	/* Since now we can write over temp and perm free space, the chance of finding 				*/
	/* sufficient space at the end of the file is much higher.														*/
	
	/* This requires when we write the label, the handler is aware that a new label is		*/
	/* now in effect. Since the handler is responsible for writing the label, this is not	*/
	/* a problem.																																					*/
		
	BackPatches				thePatches;
	CM_ULONG					freeOffset, freeLen, nonTOCLen; 
	CM_ULONG			 		tocStart, tocSize, containerSize, oldEOF;
	TOCValuePtr				theValue;
	CMBoolean					writeDirectToFreeSpace = false;

	InitBackPatches(&thePatches);
	if (cmWriteAllGlobalNames(container->globalNameTable)) {
		oldEOF = container->physicalEOF;
		freeOffset = 0;
		freeLen = 0;
		cmCopyTmpFreeToTOC(container);
		cmRemoveSmallFreeSpace(container);				/* but not if they are too small					*/
		nonTOCLen = container->logicalEOF + container->mergeInSize;

		if (container->handler.cmftrunc != NULL)
			if (container->freeSpaceValueHdr) {
				theValue = (TOCValuePtr) cmGetListTail(&container->freeSpaceValueHdr->valueList);
				if (theValue) {
					freeOffset = theValue->value.notImm.value;
					freeLen = theValue->value.notImm.valueLen;
					if (!cautious) {
						if (freeOffset+freeLen >= nonTOCLen) { 			/* free space is at the end 		*/
							/* However we cannot use free space from container to be merged						*/
							if (freeOffset < container->mergeInSize)
								freeOffset = container->mergeInSize;
							oldEOF = freeOffset;
							tocStart = freeOffset - container->mergeInSize;
							writeDirectToFreeSpace = true;	/* we can overwrite free space directly		*/
						}
					}
				}
			}

		if (cmWriteTOC(container, container->toc, ALLOBJECTS, MAXUSERID, &thePatches, &tocStart, &tocSize, writeDirectToFreeSpace)) {
			if (cmDoBackPatches(container, &thePatches, tocStart+container->mergeInSize, tocSize, 0, 0)) {
				CMwriteLabel(container,
										 container->magicBytes,						
										 container->containerFlags,
										 (container->tocBufSize / kCMBufSizeUnits),
										 container->majorVersion,
										 container->minorVersion,
										 tocStart+container->mergeInSize, tocSize);
				if (writeDirectToFreeSpace) {
					/* We truncate to the end of the label																				*/
					CMftrunc(container, CMftell(container));
				}
				else if (freeOffset+freeLen >= nonTOCLen) { 			/* free space is at the end  	*/
					/* now we have a new label at the end of the file, we can use temp free space	*/
					InitBackPatches(&thePatches);									/* ...prepare for patching TOC	*/
					/* However we cannot use free space from container to be merged								*/
					if (freeOffset < container->mergeInSize) {
						freeLen = freeLen - (container->mergeInSize - freeOffset);
						freeOffset = container->mergeInSize;
					}
					if (freeLen >= (tocSize+LBLsize+3)) {	/* if the toc can fit in the free space */
						oldEOF = freeOffset;
						tocStart = freeOffset - container->mergeInSize;
						if (cmWriteTOC(container, container->toc, ALLOBJECTS, MAXUSERID, &thePatches, &tocStart, &tocSize, true)) {
							container->physicalEOF = tocStart + tocSize;
							container->logicalEOF = container->physicalEOF;
							/* We truncate, that seem to imply that we are erasing our safety label		*/
							/* at the end of the file. However, if the handler does a logical					*/
							/* truncation rather than a physical truncation, then the label is still	*/
							/* there and it is safe to do it																					*/
							CMftrunc(container, container->physicalEOF);
							if (cmDoBackPatches(container, &thePatches, tocStart+container->mergeInSize, tocSize, 0, 0)) {
								CMwriteLabel(container,
														 container->magicBytes,						
														 container->containerFlags,
														 (container->tocBufSize / kCMBufSizeUnits),
														 container->majorVersion,
														 container->minorVersion,
														 tocStart+container->mergeInSize, tocSize);
							}
						}
					}
				}
			}
		}
		
		if (keepContainerOpen) {
			/* since we are not closing the container, update its status											*/
			containerSize 		= CMgetContainerSize(container);			/* next free byte 				*/
			container->originalEOF 		= containerSize;							/* next free byte 				*/
			container->physicalEOF 		= containerSize;							/* next free byte 				*/
			container->logicalEOF 		= containerSize;							/* logical EOF						*/
			container->maxValueOffset = containerSize; 							/* last byte offset + 1		*/
			cmAddToTmpFreeList(container, oldEOF, containerSize - oldEOF);
		}
	}
	
	if (container->embeddedValue == NULL)		  					/* if not embedded...							*/
		if (container->handler.cmfflush != NULL) 					/* ...flush all I/O buffers				*/
			if (CMfflush(container) != 0)										/* ...if flush handler provided		*/
				ERROR1(CM_err_FlushErr, CONTAINERNAME);				/* ...report error if it occurred	*/
}

/*----------------------------------------------*
 | CMCloseContainer - close an opened container |
 *----------------------------------------------*
 
 This routine closes the specified container and its ALL currently opened embedded
 containers (if any). For updating, all updating containers are also closed.  On return,
 the specifed container refNum and all the others corresponding to the embedded containers
 are invalid.  All memory associated with a container's data structures is freed along with
 the container control block itself.
 
 Note, since a tree of opened embedded containers can be formed, then calling 
 CMCloseContainer() for any node in the tree only closes that node and all its descendents
 (if any).  The parents are not closed.
 
 Aside from the implications for embedded containers, when a container that was opened for
 reading is closed, that basically is all that is done.
 
 For writing, the TOC and (except for embedded containers) the label are written to the 
 container and then it too is closed.
*/

void closeContainer(CMconst_CMContainer targetContainer, ContainerPtr mergeWithContainer);

void CM_FIXEDARGS CMCloseContainer(CMconst_CMContainer container)
{
	NOPifNotInitialized(CM_NOVALUE);										/* NOP if not initialized!				*/
	
	/* For updating, we may have free space which can only be reused if merging occurs.		*/
	/* If CMCloseContainer is being called then we are not doing any merging, so the free	*/
	/* space cannot be reused so we free them.																						*/
	/* If we are not doing updating, then	the free list are just free space that we do		*/
	/* not want to reuse until time to close the container, so don't free them						*/
	if (((ContainerPtr)container)->useFlags & kCMMerging) {
		if (((ContainerPtr)container)->useFlags & (kCMUpdateByAppend | kCMUpdateTarget))
			cmFreeAllListCells(&((ContainerPtr)container)->tmpList, 
													((ContainerPtr)container)->sessionData);
		((ContainerPtr)container)->useFlags &= ~kCMMerging;
	}

	closeContainer(container, NULL);
}

/*--------------------------------------------*
 | closeContainer - close an opened container |
 *--------------------------------------------*
 
 This is the real guts of close container. CMCloseContainer is just a cover routine that
 is distinguished from CMMergeContainer and just calls closeContainer.
*/

static void closeContainer(CMconst_CMContainer targetContainer, ContainerPtr mergeWithContainer)
{
	EmbeddedContainerPtr 	embedded, prevEmbedded, nextEmbedded;
	ContainerPtr				 	parentContainer, updatingContainer, container = (ContainerPtr)targetContainer;
	CM_ULONG			 			 	tocSize, pvtTOCStart, pvtTOCSize, newTOCStart, newTOCSize;
	BackPatches					 	thePatches;
	CMBoolean						 	aborting;

	TOCValuePtr						theValue;
	CM_ULONG							safeOffset, safeLen, freeOffset, freeLen, nonTOCLen; 
	
	#if DebugEmbedding
	CM_SHORT							origLevel = level;
	#endif
	
	/* If we're called from CMAbortContainer(), then the session abort switch will be set.*/
	/* In that context, we basically always pretend all containers were opened only for 	*/
	/* reading and just physically close (using the close handler) them and free all the	*/
	/* container's data structures as usual.  That's the main name of this game, free the	*/
	/* data structures. Since we will be asking the "abort" question a few times, we copy */
	/* the session switch to a local here.																								*/
	
	aborting = SESSION->aborting;

	/* If the useFlags for CMOpen[New]Container() was kCMUpdateByAppend or kCMUpdateTarget*/
	/* then we opened the container for updating a target container.  This means that, at */
	/* a minimum, there is at least two containers to be closed by this one								*/
	/* CMCloseContainer() call; the updating container, and its target. Containers opened */
	/* for update-by-append end up having their updates recorded as layers appened to the */
	/* end of the container.  That appending of the updates is done here.									*/
	
	/* Since a container can be opened for appending multiple times (with a close between	*/
	/* the opens, of course), then a single open may cause multiple containers to be open.*/
	/* Thus we can have more than two to close on the single CMCloseContainer() call.			*/
	
	/* The layered appened updates can also be opened just for reading.  Again that will 	*/
	/* lead to multiple container opens. But in this case there are no updates to record.	*/
	
	/* Finally, the container may be opened by CMOpenNewContainer() for updating with the	*/
	/* intent of updating a separate container accessed via dynamic value handlers. Again */
	/* we want to record the updates in the updating container.													  */
	
	/* Thus all operations are done to the updating container using its own handlers.	 It	*/
	/* was that container refNum the open returned and it is the one passed to us here.		*/
	/* The updating container always has a pointer to the "bottom" target of the updates.	*/
	/* Thus all containers from the input (top) container to the bottom are to be closed.	*/
	/* The closes are done in the reverse order of the opens, i.e., from bottom to top.		*/
	/* Except for possibly the updating container, no action other than freeing the 			*/
	/* container and its data structures needs to be done.																*/
	
	/* As just mentioned, there is always back pointer to the bottom container.  If we		*/
	/* are not updating, the pointer points to its own container.  Thus we can tell we 		*/
	/* have the updating case from all other cases by checking this pointer.  All the 		*/
	/* other cases are after this code.  Embedded containers are one such case.  Since we */
	/* never have "updating embedded containers" (?) we have mutually exclusive cases, so */
	/* the pointer test is sufficient.  But note, the opposite is NOT true!  An updating	*/
	/* container could have embedded containers.  Thus we must still make sure those are	*/
	/* closed when closing the updating container.																				*/
	
	if (container->targetContainer != container) {		 	/* multlayered updating open...		*/
	
		/* First lets get the embedded containers out of the way. Closing a parent embedded */
		/* container will cause closing of all its descendents.  Thus we only have to 			*/
		/* initiate the closing all of of the top-most parent embedded containers embedded 	*/
		/* within the updating container.																										*/
		
		embedded = (EmbeddedContainerPtr)cmGetListHead(&container->embeddedContainers);
		#if DebugEmbedding
		if (embedded)
		fprintf(stderr, "CMCloseContainer($%.8lX): level = %ld, embedded = $%.8lX\n", container, level, embedded);
		#endif
		
		while (embedded) {
			nextEmbedded = (EmbeddedContainerPtr)cmGetNextListCell(embedded);/* remember next	*/
			#if DebugEmbedding
			fprintf(stderr, "closing embedded container $%.8lX: level = %ld, embedded = $%.8lX\n",
																embedded->container, level, embedded);
			++level;
			#endif
			closeContainer((CMContainer)embedded->container, NULL);	/* close the descendent		*/
			#if DebugEmbedding
			--level;
			#endif
			embedded = nextEmbedded;														/* set to process the next one*/
		} /* while */
	
		#if DebugEmbedding
		level = origLevel;
		#endif
			
		deleteGarbage(container);
			
		/* If recording the updates, the close is similar to the "normal" case.  But here 	*/
		/* there are a "few twists"!  The following is the order of the stuff put out here	*/
		/* when a new updating container (i.e., one opened for writing) is closed:					*/
		
		/*			1. All global names from the private global name table.  This is "pure" 		*/
		/* 				 data.																																		*/
		/*			2. All new global names generated during this open session.  Again, 				*/
		/* 				 more pure data.																													*/
		/*			3. The updates data for all touched objects, properties, and values.  This	*/
		/* 				 is done is two separate parts; the updates for values and the updates for*/
		/*				 deleted objects and properties.  The touched objects with updated values	*/
		/*				 each get a special property with a value whose data is all the updates		*/
		/* 				 for the objects involved. Thus new TOC entries are being generated here! */
		/*				 After the values, all deletions are generated as data for a special TOC	*/
		/* 				 #1 property.																															*/
		/*			4. The "private" TOC for this container.																		*/
		/*			5. The "new properties (values) for old objects" TOC (ID's < min seed).			*/
		/*			6. All new objects (ID's >= min seed).																			*/
		/*			7. The label (last but not least -- well, maybe it is least).								*/
		
		/* Items 4, 5, and comprise the TOC.  A TOC property is defined to allow open to get*/
		/* at 5 and 6.  That also tells it what 4 is.  The non-private TOC (5 and 6) can be */
		/* written as a single walk of the TOC.  Since old objects always have IDs < min		*/
		/* seed and the min seed is less than any new objects, the IDs will come out in 		*/
		/* ascending order.  I know you don't care, but I'm telling you anyway!							*/

		
		if (!aborting && container->useFlags & kCMWriting) {/* if writing updates...				*/
			InitBackPatches(&thePatches);											/* ...prepare for patching TOC	*/
			
			if (!container->usingTargetGlobals || cmWriteAllGlobalNames(container->privateGlobals))
				if (cmWriteAllGlobalNames(container->globalNameTable))
					if (cmGenerateUpdates(container, mergeWithContainer)) {	/* updates generated here					*/
						safeOffset = 0;			/* where we can write the new TOC safely, 0 => not safe */
						/* we only use the trick if we can truncate and it is the top most container*/
						if ((container->handler.cmftrunc != NULL) && (container->embeddedValue == NULL))
							if (container->freeSpaceValueHdr) {
							/* look for the location and size of the last free block, this is not only*/
							/* free but also free to write on before new TOC is written 							*/
							theValue = (TOCValuePtr) cmGetListTail(&container->freeSpaceValueHdr->valueList);
								if (theValue) {
									safeOffset = theValue->value.notImm.value;
									safeLen = theValue->value.notImm.valueLen;
								}
							}
						cmCopyTmpFreeToTOC(container); /* now include the temporary free space			*/
						cmRemoveSmallFreeSpace(container);	/* but not if they are too small				*/
						/* Look at last free block again, new added free space should not be written*/
						/* unless new TOC has been written.																					*/
						if (safeOffset) {
							nonTOCLen = container->logicalEOF;	/* remember where container end				*/
							theValue = (TOCValuePtr) cmGetListTail(&container->freeSpaceValueHdr->valueList);
							freeOffset = theValue->value.notImm.value;
							freeLen = theValue->value.notImm.valueLen;
						}

						if (cmWriteTOC(container, container->privateTOC, ALLOBJECTS, MAXUSERID, &thePatches, &pvtTOCStart, &pvtTOCSize, false)) {
							if (cmWriteTOC(container, container->toc, MinUserObjectID, MAXUSERID, NULL, &newTOCStart, &newTOCSize, false)) {
								if (safeOffset) {				/* We may have some free space for writing TOC	*/
									/* make sure the free block goes to the end of the container					*/
									if (freeOffset+freeLen >= nonTOCLen)
										if (freeOffset <= safeOffset)	/* the last free block is a safe one 	*/
											if (safeLen >= (pvtTOCSize+newTOCSize+LBLsize+3)) { /* enough safe space for the TOCs */
												container->spaceDeletedValue->value.imm.ulongValue -= (nonTOCLen - safeOffset); /* and container*/
												InitBackPatches(&thePatches);		/* ...prepare for patching TOC	*/
												pvtTOCStart = safeOffset;			/* write from the safe free block */
												if (cmWriteTOC(container, container->privateTOC, ALLOBJECTS, 
																			 MAXUSERID, &thePatches, &pvtTOCStart, &pvtTOCSize, true)) {
													newTOCStart = pvtTOCStart+pvtTOCSize;
													(void) cmWriteTOC(container, container->toc, MinUserObjectID, MAXUSERID, 
																				 		NULL, &newTOCStart, &newTOCSize, true);
													container->physicalEOF = newTOCStart+newTOCSize;
													container->logicalEOF = container->physicalEOF;
													CMftrunc(container, container->physicalEOF);
												}
											}
								}
								tocSize = pvtTOCSize /*+ newTOCSize*/;
								if (cmDoBackPatches(container, &thePatches, 
										pvtTOCStart+container->mergeInSize, tocSize, 
										newTOCStart+container->mergeInSize, newTOCSize)) {
									CMwriteLabel(container,
															 container->magicBytes,				 
															 container->containerFlags,
															 (container->tocBufSize / kCMBufSizeUnits),
															 container->majorVersion,
															 container->minorVersion,
															 pvtTOCStart+container->mergeInSize, pvtTOCSize);
								}
								#if 0
								fprintf(stderr, "%s\n   pvtTOCStart = 0x%.4lX, pvtTOCSize = 0x%.4lX\n"
																		"   newTOCStart = 0x%.4lX, newTOCSize = 0x%.4lX\n",
																		CONTAINERNAME, pvtTOCStart, pvtTOCSize, newTOCStart, newTOCSize);
								#endif
							}
						}
					}
							
			if (container->handler.cmfflush != NULL) 				/* flush if we can 								*/
				if (CMfflush(container) != 0)									/* ...if flush handler provided		*/
					ERROR1(CM_err_FlushErr, CONTAINERNAME);			/* ...report error if it occurred	*/
		} /* writing */
				
	if (container->embeddedValue)	{											/* if embedded container...				*/
		parentContainer = container->embeddedValue->container;/* ...delete from parents list*/
		#if DebugEmbedding
		fprintf(stderr, "deleting/freeing list entry $%.8lX in parent $%.8lX: level = %ld\n", container->parentListEntry, parentContainer, level);
		#endif
		cmDeleteListCell(&parentContainer->updatingContainer->embeddedContainers, 
										 container->parentListEntry);
		CMfree(container->parentListEntry);
		container->embeddedValue->valueFlags &= ~ValueUndeletable;
		container->embeddedValue->theProperty->theObject->objectFlags &= ~ProtectedObject;
		if (!aborting) parentContainer->physicalEOF = CMgetContainerSize(parentContainer);
	}

		/* if we are aborting, then if we can truncate then we should truncate the file back*/
		/* to the original size so that it looks unchanged from the user's standpoint				*/

		if (aborting && (container->useFlags & kCMWriting))
			if (container->handler.cmftrunc != NULL) 			/* if truncation handler provided...*/
				(void) CMftrunc(container, container->originalEOF); /* then back to old size		*/
	
		/* At this point the updating container has been fully written, including its label.*/
		/* All that's left to do is physically close the container layers and free all their*/
		/* data structures.  The closes are in the reverse order of the opens, moving back 	*/
		/* "up" until we reach the top container.																						*/
		
		updatingContainer = container;										/* work back to this container		*/
		container = container->targetContainer;						/* ...starting at the target			*/
		
		cmFreeAllIOBuffers(container);									  /* free I/O buffers separately		*/
		
		for (;;) {																											 /* loop up layers	*/
			/* If we are just closing and not merging, then we may need to free the previous 	*/
			/* history. 																																			*/
			if (container->updateMergeCandidate)
				cmFreeAllListCells(&container->tmpList, container->sessionData);
			if (container->pointingValue)																	 /* release pointing*/
				CMReleaseValue(container->pointingValue);										 /*      value      */
			CMfclose(container);																					 /* close container	*/
			cmFreeAllGlobalNames(&container->globalNameTable); 	 				 	 /* free glbl names	*/
			cmFreeTOC(container, &container->toc);												 /* purge all data	*/
			if (container->usingTargetGlobals)  													 /* if 2 global tbls*/
				cmFreeAllGlobalNames(&container->privateGlobals);						 /* ...free 2nd one	*/
			if (container->usingTargetTOC) 																 /* if have 2 TOCs	*/
				cmFreeTOC(container, &container->privateTOC);								 /* ...free 2nd one	*/
			parentContainer = (ContainerPtr)cmGetPrevListCell(container);	 /* remember parent	*/
			CMfree(cmDeleteListCell(&SESSION->openContainers, container)); /* free container  */
			if (container == updatingContainer || parentContainer == NULL) /* if all closed...*/
				break;																											 /* ...we're done		*/
			container = parentContainer;																	 /* point at parent	*/
			/* This is bug fix by DKF																													*/
			/* since the target container is the first one free, container->targetContainer		*/
			/* is no longer valid, we NULL it so it won't be used in CMReleaseValue						*/
			container->targetContainer = NULL;
		}																																 /* next layer up		*/

		
		return;																						/* we're now fully closed!				*/
	} /* updating */

	/* From here on, we have a "normal" close...																					*/
	
	/* Associated with each container is a list of all containers opened as embedded 			*/
	/* within it -- the embeddedContainers list.  There may not be any.  But if there are	*/
	/* closing a parent container will close all the open container embedded within it.		*/
	/* That's what the following code is for. It goes through this container's embedded		*/
	/* container list, from tail-to-head, i.e., last in to last out, recursively calling	*/
	/* ourselves, to close our immediate decendents, which is all we know. They, in turn, */
	/* may attempt the same thing if they have embedded container.  The net result is to	*/
	/* close all the descendents from the newest to the oldest (because we're going tail 	*/
	/* to head).  Basically what we have here is a depth-first search on a tree of 				*/
	/* embedded containers.																																*/

	embedded = (EmbeddedContainerPtr)cmGetListTail(&container->embeddedContainers);	
	
	#if DebugEmbedding
	if (embedded)
		fprintf(stderr, "CMCloseContainer($%.8lX): level = %ld, embedded = $%.8lX\n", container, level, embedded);
	#endif
	
	while (embedded) {																	/* loop through the list...				*/
		prevEmbedded = (EmbeddedContainerPtr)cmGetPrevListCell(embedded);/* remember next		*/
		#if DebugEmbedding
		fprintf(stderr, "closing embedded container $%.8lX: level = %ld, embedded = $%.8lX\n",
															embedded->container, level, embedded);
		++level;
		#endif
			closeContainer((CMContainer)embedded->container, NULL);	/* close the descendent		*/
		#if DebugEmbedding
		--level;
		#endif
		embedded = prevEmbedded;													 /* set to process the next one		*/
	} /* while */

	#if DebugEmbedding
	level = origLevel;
	fprintf(stderr, "doing full close on $%.8lX: level = %ld\n", container, level);
	#endif
	
	deleteGarbage(container);

	/* At this point all embedded descendents have been closed.  We can now close this 		*/
	/* (parent) container.  You can look at the depth-first recursion mentioned above at 	*/
	/* the point where we are now at the deepest point in the tree (because if we're here,*/
	/* that's where we are -- the bottom of the tree). This container is, if we actually	*/
	/* did recurse, an embedded container and represents a value in its parent. The signal*/
	/* that tells us we're an embedded container for a parent is the field embeddedValue	*/
	/* in our container control block.																										*/
	
	/* If we opened the container for output, then we write out the global names to the 	*/
	/* end of the container. That will add the global names container offsets to the TOC.	*/
	/* Then we write the TOC itself following the global names.  Lastly, if we're not an	*/
	/* embedded container (remember the switch just mentioned above) comes the label.			*/
	
	/* Note, there is one value, which is created but its actual value is not known				*/
	/* because it is defined as TOC offset and size. The pointer to this value,						*/
	/* tocObjValue, not its value header, is kept in the container control block and is 	*/
	/* defined at open time. It points to the value for the TOC object property value. It	*/
	/* is filled in when we call cmWriteTOC() below.  Actually the correct value is back 	*/
	/* patched into the written container.																								*/
			
	if (container->useFlags & kCMWriting) {
		if (aborting) { 
			if (container->handler.cmftrunc != NULL) 			/* if truncation handler provided...*/
				(void) CMftrunc(container, container->originalEOF); /* then back to old size		*/
		}
		else
			/* write the TOC, no need to update container status since we are closing it later*/
#if (!CMTOPLEVEL_CRASH_PROOF && !CMEMBEDDED_CRASH_PROOF)
			writeTOC(container, false, false);  					/* don't need to be crush proof			*/
#endif
#if (CMTOPLEVEL_CRASH_PROOF && CMEMBEDDED_CRASH_PROOF)
			writeTOC(container, false, true);  						/* both case need to be crush proof */
#endif
#if (CMTOPLEVEL_CRASH_PROOF && !CMEMBEDDED_CRASH_PROOF)
  		/* top level container need to be crush proof 																		*/
			writeTOC(container, false, (container->embeddedValue == NULL));
#endif
#if (!CMTOPLEVEL_CRASH_PROOF && CMEMBEDDED_CRASH_PROOF)
  		/* embedded container need to be crush proof 																			*/
			writeTOC(container, false, (container->embeddedValue != NULL));
#endif
	} /* writing */
	
	/* All that's left to do is close the container.  For an embedded container we must		*/
	/* take the thing of the  parent's list of embedded containers.  This embedded 	*/
	/* container is about to cease to exist.  We don't want the parent trying to close it */
	/* again if it is not being close as part of the up-level recursion.  There's some 		*/
	/* more discussion on this near the end of openContainer() if you care to look. 			*/
	/* Basically, the descendent created the list entry.  It's only fair it remove it.  	*/
	/* You want to buy a bridge?																													*/
	
	/* Oh, this is sort of anticlimactic, but if we aren't an embedded container, then we	*/
	/* must close it.  Just thought you would like to know that. [I think my comments are	*/
	/* getting strange ... nah!]																													*/
		
	CMfclose(container);																/* ...that's it for this container*/
	
	if (container->embeddedValue)	{											/* if embedded container...				*/
		parentContainer = container->embeddedValue->container;/* ...delete from parents list*/
		#if DebugEmbedding
		fprintf(stderr, "deleting/freeing list entry $%.8lX in parent $%.8lX: level = %ld\n", container->parentListEntry, parentContainer, level);
		#endif
		cmDeleteListCell(&parentContainer->updatingContainer->embeddedContainers, container->parentListEntry);
		CMfree(container->parentListEntry);
		container->embeddedValue->valueFlags &= ~ValueUndeletable;
		container->embeddedValue->theProperty->theObject->objectFlags &= ~ProtectedObject;
		if (!aborting) parentContainer->physicalEOF = CMgetContainerSize(parentContainer);
	}
	
	/* We almost done.  Free all the memory allocated to the TOC data structures and 			*/
	/* free the container control block itself.  Ok, even here there's potential for a 		*/
	/* problem.  And this one is currently NOT solved.  And that is what if the        	*/
	/* user attempts to use the container "refCon" again.  He now has a pointer to 				*/
	/* garbage.  My initial solution was to make the container pointer a pointer to it 		*/
	/* (i.e., *container) as the CMCloseContainer() parameter.  The we could NULL it out	*/
	/* here. We could then check at each API entry point for NULL just like we do for the	*/
	/* error handler.  The problem with that is that a close of a parent container causes	*/
	/* all descended embedded containers to be closed from here.  We can't get at the 		*/
	/* user's container refNums.  So forget that scheme.  We could make the open's take		*/
	/* a *container as a parameter so we could capture all the addresses.  But we can't		*/
	/* stop the user from aliasing it.  So we loose there too.  We could just put the			*/
	/* container control blocks on a "closed" list and mark the container as closed. This	*/
	/* would work.  Instead of check for NULL we would check for the mark. But the control*/
	/* blocks are not the smallest things in the world.  So space may be a consideration	*/
	/* as to why we wouldn't use this solution.  So, for the moment, I am punting on the	*/
	/* problem.     														*/
	
	/* You might ask why I am making such a big deal of this (well, I guess you might). 	*/
	/* It is because, during testing, quite a few times, I got burnt using a refCon for a	*/
	/* closed container. It hurts when I do that (well don't do that)! Things potentially */
	/* are now even more "accident prone" with embedded containers.  Because closing a		*/
	/* parent closes ALL its descendents.  The user must keep track that this is going on	*/
	/* because there is no safety valve here. That's why I think this is such a big deal! */
	
	/* If we are just closing and not merging, then we may need to free the previous 			*/
	/* history. 																																					*/
	cmFreeAllListCells(&container->tmpList, container->sessionData);
	cmFreeAllIOBuffers(container);											/* purge ALL data structures			*/
	cmFreeAllGlobalNames(&container->globalNameTable);
	cmFreeTOC(container, &container->toc);						  
	CMfree(cmDeleteListCell(&SESSION->openContainers, container)); /* free container 			*/
}

/*----------------------------------------------------------------------------*
 | CMTakeSnapShot - write out the TOC and label without closing the container |
 *----------------------------------------------------------------------------*
 
 CMTakeSnapShot is roughly equivalent to closing a container and opening it again. The
 container is not actually closed so we don't have to go through the overhead. More
 importantly, if you close and open a container the refnum of the objects and values 
 in the containers are no longer valid. However by using CMTakeSnapShot, since the
 container is not actually closed, the refnum remains valid. 
 
 When a container is closed, the embedded container is also closed and written out.
 Since CMTakeSnapShot does not close the file, the writing out of the TOC content
 of the embedded containers is not a absolute requirement. So this is decoupled in
 the CMTakeSnapShot call. You can use the includeEmbedded parameter to do it for
 the container only or do it for the container and any embedded container.
 
 This implementation only works for non-updating container because that is all that
 is required for OpenDoc. We shall extend this to updating container later.
 
*/

void CM_FIXEDARGS CMTakeSnapShot(CMconst_CMContainer targetContainer, CMBoolean includeEmbedded)
{
	ContainerPtr 					container = (ContainerPtr)targetContainer;
	EmbeddedContainerPtr 	embedded;

	NOPifNotInitialized(CM_NOVALUE);										/* NOP if not initialized!				*/
	
	if (includeEmbedded) {							/* also do it to the embedded container						*/
		embedded = (EmbeddedContainerPtr)cmGetListHead(&container->embeddedContainers);
		while (embedded) {
			CMTakeSnapShot((CMconst_CMContainer)(embedded->container), includeEmbedded);
			embedded = (EmbeddedContainerPtr)cmGetNextListCell(embedded);
		}
	}
	
	/* current implementation is only for non-updating container 													*/
	if (container == container->targetContainer) { 
#if (!CMTOPLEVEL_CRASH_PROOF && !CMEMBEDDED_CRASH_PROOF)
			writeTOC(container, true, false);  						/* don't need to be crush proof			*/
#endif
#if (CMTOPLEVEL_CRASH_PROOF && CMEMBEDDED_CRASH_PROOF)
			writeTOC(container, true, true);  						/* both case need to be crush proof */
#endif
#if (CMTOPLEVEL_CRASH_PROOF && !CMEMBEDDED_CRASH_PROOF)
  		/* top level container need to be crush proof 																		*/
			writeTOC(container, true, (container->embeddedValue == NULL));
#endif
#if (!CMTOPLEVEL_CRASH_PROOF && CMEMBEDDED_CRASH_PROOF)
  		/* embedded container need to be crush proof 																			*/
			writeTOC(container, true, (container->embeddedValue != NULL));
#endif
	}
}

/*-------------------------------------------------------------------*
 | releaseDynamicValues - release dynamic values for a single object |
 *-------------------------------------------------------------------*
 
 This is an action routine for the object interator, cmForEachObject(). It is set by
 CMAbortContainer() to look for the dynamic value property of each object.  If the object
 has one, then all its dynamic values are released by doing standard API CMReleaseValue()
 calls for each dynamic value.  This is done to make sure dynamic values are properly
 released which may involve handlers freeing memory and closing files known only to them.
 
 This routine is used only for abort conditions, since it is invoked via 
 CMAbortContainer().  Thus the "world" is comming to an end!  For the releases to work
 properly, no other data structures must be freed prior to releasing the dynamic values.
 Of course, being an abort situitation, there are no guarantees!  Why are we aborting in
 the first place?
 
 Note, a count in the targetContainer is maintained of the total number of active dynamic
 values.  Thus this walk is not done unless that count is nonzero.
*/
 
static void releaseDynamicValues(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container = (ContainerPtr)refCon;
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr, nextValueHdr, theDynValueHdr;
	
	/* Each object is flagged as to whether it has dynamic values or not.  It it does,		*/
	/* then go after the dynamic value property and process each dynamic value on its 		*/
	/* value header chain.																																*/
	
	if ((theObject->objectFlags & DynamicValuesObject) != 0) {
		theProperty = cmGetObjectProperty(theObject, CM_StdObjID_DynamicValues);
	
		/* If the dynamic value property really is present (and it really should be if the	*/
		/* flag in the object is set -- but we safety check it), then loop through each			*/
		/* dynamic value on the property's value header list.																*/
		
		if (theProperty != NULL) {
			theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
			
			/* Loop through each dynamic value header for the dynamic value property...				*/
			
			while (theValueHdr) {
				nextValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
			
				/* Since the value headers for the dynamic value property may not correspond to */
				/* top-most dynamic value header layer (which is the "true" user refNum), then	*/
				/* go through the "real" value header to get at the top-most layer. The header 	*/
				/* on the dynamic value property value header chain is always the bottm-most and*/
				/* always has the "real" value as its base.  Hence its easy to get to the "real"*/
				/* value!																																				*/
					
				theDynValueHdr = DYNEXTENSIONS(theValueHdr)->baseValue->dynValueData.dynValue;
				
				/* Release the value as the user would.  This should free the dynamic value 		*/
				/* headers and eventually the property itself...																*/
				
				if (theDynValueHdr != NULL)
					CMReleaseValue((CMValue)theDynValueHdr);
					
				theValueHdr = nextValueHdr;
			} /* while */
		} /* theProperty */
	} /* DynamicValuesObject */
}


/*---------------------------------------------*
 | CMAbortContainer - abort use of a container |
 *---------------------------------------------*
 
 The container is closed WITHOUT further writing to the container, i.e., as if it were
 opened for reading even when opened for writing.  This is intended to be used to abort
 processing of the container from unrecoverable errors.
 
 All memory allocated by the container data structures is freed (if possible) and the
 container close handler called to physically close the container.  All dynamic values
 currently in use are released in an attempt to allow them to properly clean up any files
 and memory allocated by their handlers.  No further API calls should be done on the
 container as it will be closed upon return.
 
 Note, this routine WILL return to its caller.  It is up to the user to actually abort
 execution if that is required.

 This is implemented basically as a CMCloseContainer() but with the session abort switch
 set.  That switch causes CMCloseContainer() to avoid all writes but all the data structure
 freeing still happens as usual.  That way, all the freeing code is in that one place.
*/
 
void CM_FIXEDARGS CMAbortContainer(CMconst_CMContainer container)
{
	SessionGlobalDataPtr	sessionData;
	
	NOPifNotInitialized(CM_NOVALUE);										/* NOP if not initialized!				*/
	
	/* We get the sessionData out of the container first because the container will be		*/
	/* gone when it is closed																															*/
	
	sessionData = ((ContainerPtr)container)->sessionData;
	
	if (sessionData->aborting) return;									/* ignore recursive aborts!				*/
	sessionData->aborting = true;												/* throw the deadly switch				*/
	
	/* We are aborting, we cannot use the temporary free list we built up, so just free it*/

  cmFreeAllListCells(&((ContainerPtr)container)->tmpList, 
										 ((ContainerPtr)container)->sessionData);

	/* If there are any dynamic values currently in used, release them.  This means 			*/
	/* going through the entire TOC and "hunting" down the dynamic values since there is	*/
	/* no single thread of all of them.  To aid in this, objects that contain dynamic 		*/
	/* values are uniquely flagged, and there is a count in the targetContainer that tells*/
	/* how many dynamic values there are in use.  Thus we may not have to look at any of	*/
	/* the objects if there are no dynamic values, and then only at the ones that have		*/
	/* dynamic values if there are.  Of course, in the latter case, all objects still have*/
	/* to be visited.																																			*/
	
	if (((ContainerPtr)container)->targetContainer->nbrOfDynValues > 0)
		cmForEachObject(((ContainerPtr)container)->toc, ALLOBJECTS, MAXUSERID,
									  (void *)container, releaseDynamicValues);
	
	CMCloseContainer(container);												/* get rid of the data structures	*/
	
	sessionData->aborting = false;													/* we are no longer in abort mode	*/
}

/*------------------------------------------------------------*
 | offsetTheValues - take the values and add the prepend size |
 *------------------------------------------------------------*
 
 When we merge values, if they are embedded values, we are inseting data in front
 of the current embedded values. In the embedded container, all the offset are
 measured from the beginning of the embedded container, but now we are going to
 prepend some data in front of that. So the offset need to be changed for the values 
 in the current container. 
 
 For a value belonging to the target container, the offset will remain the same.
 However, the owning container will be changed to the current container. We shall also
 do that here.
 
*/

static void offsetTheValues(ContainerPtr container, ContainerPtr targetContainer, TOCPropertyPtr theProperty)
{
	CM_ULONG			 offset = container->mergeInSize;
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue;

	theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
	while (theValueHdr) {
		if (theValueHdr->container == targetContainer)
			theValueHdr->container = container;
		if ((theValueHdr->valueFlags & ValueImmediate) == 0) {
			/* if it is a global name and not been written to disk, leave offset as 0					*/
			if (((theValueHdr->valueFlags & ValueGlobal) == 0) || (theValueHdr->valueRefCon)) {
				theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
				while (theValue) {
					if (theValue->container == container) {
						theValue->value.notImm.value += offset;
					}
					else if (theValue->container == targetContainer) {
						theValue->container = container;
					}
					theValue = (TOCValuePtr)cmGetNextListCell(theValue);
				}
			}
		}
		theValueHdr = (TOCValueHdrPtr)cmGetNextListCell(theValueHdr);
	}
}

/*------------------------------------------------------------------------------*
 | declareAsYourOwn - grab object from target and put in update container's TOC |
 *------------------------------------------------------------------------------*
 
 This is an action routine for the object interator, cmForEachObject(). It is set by
 CMMergeContainer() to take the objects in the common TOC and put it into the updating
 container's private TOC.
 
*/
 
static void declareAsYourOwn(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container = theObject->container->updatingContainer;
	ContainerPtr 	 targetContainer = container->targetContainer;
	TOCPropertyPtr theProperty;
	TOCObjectPtr	 replacedObject;	
	if (theObject->objectID == CM_StdObjID_TOC) { /* we don't want the TOC object		 */
		cmFreeProperties(theObject, (CMRefCon)container);
		CMfree(theObject);																						/* let them go	*/
	}
	else {
		cmPutObjectInTOC(container->privateTOC, theObject, &replacedObject); /* put in my TOC */
		if (replacedObject) {	/* it is a duplicate, we don't want it													*/
			cmFreeProperties(replacedObject, (CMRefCon)container);
			CMfree(replacedObject);																			/* let them go	*/
		}
		theObject->container = container;
		/* go through all the value and valueHdr and point their container to me							*/
		theProperty = (TOCPropertyPtr)cmGetListHead(&(theObject)->propertyList);
		while (theProperty) {
			/* The merging may affect the value offset, so adjust them									*/
			offsetTheValues(container, targetContainer, theProperty);
			theProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
		}
	}
}

/*-------------------------------------------------------------------------------*
 | consolidateSegments - combine contiguous value segments into minumum size one |
 *-------------------------------------------------------------------------------*
 
 Scan through the value segments. If we have contiguous value segments that are
 smaller the minimum size and can be combined into larger one, we try to replace
 then with a single segment from the free space, and delete the small segments.
 This is how we try to prevent fragmentation from running out of hand.
 
*/
 
static void CM_NEAR consolidateSegments(TOCValueHdrPtr theValueHdr)
{
#if MinFragmentSize
	ContainerPtr		container = theValueHdr->container;
	TOCValuePtr		 	theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
	TOCValuePtr		 	nextValue;
	CM_ULONG				offset = 0;
	CM_ULONG				totalFragmentSize = 0;
	CM_ULONG				fragmentCount = 0;
	CM_ULONG				startFragmentOffset, thisSegmentSize, startWriteOffset, actualSize;
	TOCValueBytes		valueBytes;
	CM_UCHAR				tempBuf[MinFragmentSize];
	
	/* Go through all the value segments. At the end we do the block one more time				*/
	/* because we may have accumulate small segments at the end of the list that we have	*/
	/* not copy out yet.																																	*/

	while ((theValue) || (fragmentCount)) {
		if (theValue) {														/* not yet the end, look at next segment	*/
			nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
			thisSegmentSize = theValue->value.notImm.valueLen;
		}
		else	/* We are only here only if fragmentCount != 0, so we have unfinished work		*/
			thisSegmentSize = MinFragmentSize + 1;									/* force a consolidation 	*/
		if (totalFragmentSize + thisSegmentSize > MinFragmentSize) {	/* now big enough			*/
			if (fragmentCount > 1) {	/* can consoldiate only if more than 1 contig. segment	*/
				/* First get the space from the free list																				*/
				startWriteOffset = cmGetFreeListEntry(container, 
																							totalFragmentSize,
																							true, 
																							&actualSize);
				if (startWriteOffset) {		/* Free space is avaiable, read data and write there 	*/
					if (CMReadValueData((CMValue)theValueHdr, &tempBuf, 
															startFragmentOffset, totalFragmentSize) == totalFragmentSize) {
						CMfseek(container, startWriteOffset, kCMSeekSet);			/* ...position to write over it			*/
						if (CMfwrite(container, &tempBuf, sizeof(CM_UCHAR), totalFragmentSize) != totalFragmentSize) {
							ERROR1(CM_err_BadWrite, CONTAINERNAME);
							return;
						}
						/* We can remove the old smaller segments																		*/
						CMDeleteValueData((CMValue)theValueHdr, startFragmentOffset, totalFragmentSize);
						/* We put back the new consolidated segment. If we are in the middle, use		*/
						/* cmInsertNewSegment. If we are at the end we append the new value.				*/
						if (theValue) {
							cmInsertNewSegment(theValueHdr, theValue, 0, startWriteOffset, totalFragmentSize);
						}
						else {
							(void)cmSetValueBytes(container, &valueBytes, Value_NotImm, startWriteOffset, totalFragmentSize);
							cmAppendValue(theValueHdr, &valueBytes, 0);	
							cmTouchEditedValue(theValueHdr);				/* touch for updating if necessary*/
						}
					}
				}
			}
			totalFragmentSize = 0;	/* for now no more unprocessed small segments, reset size */
			fragmentCount = 0;
		}
		if (thisSegmentSize < MinFragmentSize) { 	/* We have another small segement					*/
			totalFragmentSize += thisSegmentSize;		/* Add it to the unprocessed list					*/		
			if (fragmentCount == 0)	/* the first time, then remember the offset to read data	*/
				startFragmentOffset = offset;
			fragmentCount++;
		}
		offset += thisSegmentSize;
		theValue = nextValue;
	}
#else
	return;
#endif
}

/*-----------------------------------------------------*
 | mergeValues - merge two embedded container into one |
 *-----------------------------------------------------*
 
 We have two embedded containers. So each container is a value in an embedding container.
 If we want to merge the two containers so that the two containers become a single
 container, then we meed to merge the two values too so that they become a single
 value in the embedding container.
 
 When we merge, we would no longer need the TOC of the old container, so we only take
 data from the first container up to before the TOC, and that size is given by
 prependSize. So treating targetValue as a sequence of value segments, we just take
 everything up to prependSize and prepend it before thisParentValue.
 
*/
 
static void CM_NEAR mergeValues(TOCValueHdrPtr thisParentValue, TOCValueHdrPtr targetValue,
																CM_ULONG prependSize)
{
	TOCValuePtr		 				theValue, nextValue;
	TOCValuePtr						theInsertValue, insBeforeValue;
	TOCValueBytes					valueBytes;
	
	/* merging embedded containers, then need to merge values 														*/
	theValue = (TOCValuePtr)cmGetListHead(&targetValue->valueList);
	insBeforeValue = (TOCValuePtr)cmGetListHead(&thisParentValue->valueList);
	/* we need to take all data up to prependSize and prepend it to current value					*/
	while ((theValue) && (prependSize)) {				/* loop thru target value and take it over*/
		nextValue = (TOCValuePtr)cmGetNextListCell(theValue);
		if (prependSize) {												/* we still need to grab the values 			*/
			if (prependSize >= theValue->value.notImm.valueLen) {				/* we want all of this*/
					theInsertValue = theValue;															/* just grab it 			*/
					(void)cmDeleteListCell(&targetValue->valueList, theValue); /* remove old  		*/
			}
			else {																	/* we just want the first part						*/
				/* Make a value segment that is just the first part that we want								*/
				(void)cmSetValueBytes(thisParentValue->container, &valueBytes, Value_NotImm,
															theValue->value.notImm.value, 
															prependSize);
				theInsertValue = cmCreateValueSegment(targetValue, &valueBytes, 0);
				if ((theValue->flags & kCMImmediate) == 0)
					theValue->value.notImm.value += prependSize;	/* offset to what is left over	*/
				theValue->value.notImm.valueLen -= prependSize;	/* size of the left over				*/
			}
			/* We just insert in front of thisParentValue																			*/  
			if (theInsertValue) {
				(void)cmInsertBeforeListCell(&thisParentValue->valueList, theInsertValue, insBeforeValue);
#if CMKEEP_CONTINUE_FLAG
				theInsertValue->flags = (CM_USHORT)(insBeforeValue->flags | kCMContinued); 
				thisParentValue->valueFlags |= ValueContinued;				/* echo flags in the hdr	*/
#endif
				thisParentValue->size += theInsertValue->value.notImm.valueLen; /* add to total value len	*/
				targetValue->size -= theInsertValue->value.notImm.valueLen; /* remove from total value len	*/
				prependSize -= theInsertValue->value.notImm.valueLen; /* remove from total value len	*/
			}
		}
		theValue = nextValue;
	}
	/* Anything remaining are unwanted space, we just delete it. It would go either to the*/
	/* temporary or permanent free space depending which container it belongs to					*/
	CMDeleteValueData((CMValue)targetValue, 0, targetValue->size);

	if (cmCountListCells(&thisParentValue->valueList) > MaxFragmentCount)
		consolidateSegments(thisParentValue);	/* too many, then we try to combine small ones */
}

/*----------------------------------------------------------------------------*
 | grabMostRecent - grab object from target and put in update container's TOC |
 *----------------------------------------------------------------------------*
 
 This is an action routine for the object interator, cmForEachObject(). It is set by
 doMergeUpdate to take the objects in the target container and put it into the updating
 container's private TOC.
 
*/

static void CM_NEAR grabMostRecent(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container = theObject->container->updatingContainer;
	ContainerPtr 	 targetContainer = (ContainerPtr)refCon;
	TOCPropertyPtr theProperty;
	
	/* Update instructions applies to objects in old container. If we grab an object			*/
	/* from an old container and put it into our own, then we no longer need update for		*/
	/* that object, so we remove it from the touch chain.																	*/
	if ((theObject->container == targetContainer) || (theObject->container == container)) {
		theObject->container = container;
		if (theObject->objectFlags & TouchedObject)								/*if object was touched...*/
			cmDeleteObjFromTouchedChain(container, theObject, NULL);/* no longer touched			*/	
			theObject->objectFlags |= TouchedObject;				/* .so that next call can be made	*/
			cmDeleteTouchedList(theObject, container);
	}
	
	/* go through all the value and valueHdr and point their container to me							*/
	theProperty = (TOCPropertyPtr)cmGetListHead(&(theObject)->propertyList);
	while (theProperty) {
		/* The merging may affect the value offset, so adjust them													*/
		offsetTheValues(container, targetContainer, theProperty);
		theProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
	}
}

/*-------------------------------------------------------------------------*
 | updateMyOwn - update offset of object values in container's private TOC |
 *-------------------------------------------------------------------------*
 
 This is an action routine for the object interator, cmForEachObject(). When embedded
 containers are merge, the offset in the values in the new containers need to be
 changed because we may prepend a value to it, so we go through all new objects and 
 adjust their offsets.
 
*/

static void CM_NEAR updateMyOwn(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 container = theObject->container->updatingContainer;
	TOCPropertyPtr theProperty;
	
	/* go through all the value and valueHdr and offset the values												*/
	theProperty = (TOCPropertyPtr)cmGetListHead(&(theObject)->propertyList);
	while (theProperty) {
		/* The merging may affect the value offset, so adjust them													*/
		offsetTheValues(container, NULL, theProperty);
		theProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
	}
}

/*----------------------------------------------------------------------------------------*
 | doMergeUpdate - merge the new container's update instruction to the previous container |
 *----------------------------------------------------------------------------------------*
 
 When we try to merge a new container to the previous container, if the preivous
 container is also an updating container, then we need to merge the updating instructions
 of the two container. This is different from the case where the previous conainter is
 not an updating container so we can just write a new merged container with no updating
 instructions. Since the two cases are so different we have a separate routine to do
 the merging of two updating containers.
 
*/

static void doMergeUpdate(ContainerPtr container, TOCValueHdrPtr targetValue)
{
	ContainerPtr 		targetContainer = (ContainerPtr)(cmGetNextListCell(&container->containerLinks));
	CM_ULONG				prependSize = container->mergeInSize;
	TOCValueHdrPtr	thisParentValue;
	TOCObjectPtr		theTOCObject, theTypeObject;
	TOCPropertyPtr	theProperty;
	TOCValueHdrPtr 	theValueHdr;
	TOCValuePtr		 	theValue;
	CM_ULONG				targetOffset, targetSize, nameOffset = 0, nameSize;
	CMObjectID			typeID;

	/* the min seed id of target container is now my min seed id													*/
	((ContainerPtr)container)->tocIDMinSeedValue->value.imm.ulongValue = 
									targetContainer->tocIDMinSeedValue->value.imm.ulongValue;

	theTOCObject = cmFindObject(((ContainerPtr)targetContainer)->privateTOC, CM_StdObjID_TOC);
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Target);
	if (theProperty) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
		targetOffset = theValue->value.notImm.value;
		targetSize = theValue->value.notImm.valueLen;
		typeID = theValueHdr->typeID;
		/* We no longer need the property in the private TOC																*/
		if (typeID >= MinUserObjectID) {
			theTypeObject = cmFindObject(((ContainerPtr)targetContainer)->privateTOC, typeID);
			if (theTypeObject) {
				theProperty = (TOCPropertyPtr)cmGetListHead(&theTypeObject->propertyList);
				if (theProperty) {
					theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
					theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
					if ((theValue) && (theValue->flags & kCMGlobalName)) {
						nameOffset = theValue->value.globalName.offset;	
						if (nameOffset != 0)
							nameSize = GetGlobalNameLength(theValue->value.globalName.globalNameSymbol) + 1;
					}
				}
			}
		}
	}

	/* take each object in target and put it into my own private TOC											*/
	cmForEachObject(container->toc, ALLOBJECTS, MAXUSERID,
								(CMRefCon)targetContainer, grabMostRecent);

	/* and offset value of each object in my own private TOC															*/
	cmForEachObject(container->privateTOC, ALLOBJECTS, MAXUSERID,
								(CMRefCon)NULL, updateMyOwn);

	/* if A updates B, and B update C, the end result should be A (the merge result of A 	*/
	/* & B) updates C, so we collects the information on C so that later A can point to C */ 
	
	theTOCObject = cmFindObject(container->privateTOC, CM_StdObjID_TOC);
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Target);
	if (theProperty) {
			theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
			theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
			cmAddValueToTmpFreeList(theValueHdr);
			theValue->value.notImm.value = targetOffset;
			theValue->value.notImm.valueLen = targetSize;
			if (nameOffset)
				cmAddToTmpFreeList(container, nameOffset, nameSize);
	}
	
	/* We have kept a history of the past updates so we can generate the update 					*/
	/* again in the new merged container, but since it may not be on the touch list we		*/
	/* may miss it. So here we make sure any udpates we make in container we are going to */
	/* merge with will the touch list so they shall be processed in the update generation.*/
	cmPastUpdateOnTouchList(targetContainer);
	
	if (targetValue) {
		thisParentValue = container->embeddedValue;/* remember this before we free container*/
		/* Since we are updating a target, we could not take over the free list before			*/
		/* But now that we are merging and they would be the same container, we can do it.	*/
		cmTakeOverFreeList((ContainerPtr)container, targetContainer);
	}

	cmDeleteListCell(&SESSION->openContainers, targetContainer);

	if (targetContainer->pointingValue)																/* release pointing	*/
		CMReleaseValue(targetContainer->pointingValue);									/*      	value     */
	if (targetValue == NULL)																/* free the toc to free list	*/
		cmAddToTmpFreeList((ContainerPtr)container, targetContainer->tocOffset,
											 targetContainer->physicalEOF - targetContainer->tocOffset);
	cmFreeTOC(targetContainer, &targetContainer->toc);						/* decrement use count	*/
	cmFreeTOC(targetContainer, &targetContainer->privateTOC);			/* ...free 2nd one			*/
	cmFreeAllGlobalNames(&targetContainer->privateGlobals);
	cmFreeAllGlobalNames(&targetContainer->globalNameTable);
	
	if (targetValue) {
		returnToParent((ContainerPtr)container, targetContainer, (TOCValueHdrPtr)targetValue);
		prependSize =	((ContainerPtr)container)->mergeInSize;
		((ContainerPtr)container)->useFlags &= ~kCMReuseFreeSpace;
	}
	closeContainer((CMconst_CMContainer)container, targetContainer);
	
	if (targetValue)
		mergeValues(thisParentValue, targetValue, prependSize);

	(*targetContainer->handler.cmfclose)(targetContainer->refCon);/* close the target 		*/
	container = targetContainer;
	CMfree(targetContainer); /* free container */

}

/*--------------------------------------------------------------*
 | makeRefSameEndian - merge a container to its immediate target |
 *--------------------------------------------------------------*
 
 This is an action routine for the object interator, cmForEachObject(). When we
 merge two containers with different endian-ness, we need to convert the IDs
 in the reference objects into the endian-ness of the updating container. So we
 go through all the objects and pick out the reference objects. Then we pick
 the object IDs (located at location 8n+4) and change their endian-ness.

*/
 
static void CM_NEAR makeRefSameEndian(TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 	container = theObject->container->updatingContainer;
	TOCPropertyPtr 	theProperty;
	TOCValueHdrPtr	theValueHdr;
	CM_ULONG				offset;
	CMObjectID			theData, theObjectID;
	CM_USHORT			 	orgFlags;			
	
	/* go through all the value and valueHdr and offset the values												*/
	theProperty = (TOCPropertyPtr)cmGetListHead(&(theObject)->propertyList);
	if (theProperty->propertyID == CM_StdObjID_ObjReferences) {
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
		if (theValueHdr->typeID == CM_StdObjID_ObjRefData) {
			orgFlags = theValueHdr->valueFlags;
			theValueHdr->valueFlags &= ~ValueProtected;
			/* this is a reference object																											*/
			for (offset = 4; offset < theValueHdr->size; offset += 8) {													
				/* read each object ID in the reference object																	*/
                            /* 
                            ** Convert object ID in the reference object only
                            ** if the reference object is from the target
                            ** container. If reference object is created
                            ** by the updating container, the object
                            ** ID is already in the correct format.
                            */
                            if (((TOCValueHdrPtr)theValueHdr)->container == ((ContainerPtr) refCon))
                            {
				if (CMReadValueData((CMValue)theValueHdr, (CMPtr)&theData, offset, sizeof(CMObjectID)) == sizeof(CMObjectID)) {
					/* first read out the ID using the endian-ness of the target container				*/
					CMextractData((ContainerPtr)refCon, &theData, sizeof(CMObjectID), &theObjectID);
					/* then format it using the endian-ness of the updating container							*/
					CMformatData(container, &theData, sizeof(CMObjectID), &theObjectID);
					/* and write it back 																													*/
					CMWriteValueData((CMValue)theValueHdr, (CMPtr)&theData, offset, sizeof(CMObjectID));
				}
                            }
			}
		theValueHdr->valueFlags = orgFlags;
		}
	}
}

/*--------------------------------------------------------------*
 | CMMergeContainer - merge a container to its immediate target |
 *--------------------------------------------------------------*
 
 This is only for container that was opened for update. The content of the updating
 container is merged with its immediate target container and the the two containers
 becomes one. So an application can open a container for update, and at close time
 changes its mind and close the container as if it was open for writing in the
 first place.
 
 If the target container is in another file, then it is not possible to do the merge.
 
 However if the container is an embedded value in container X, and the target
 container is another embedded value in the same container X, then it should be
 possible to merge the the two containers and the two values. Given the updating
 container, we should be able to get the parent value. However so far we have
 problem with getting the parent value of the target container. Until we can
 solve that problem we shall have the application passes in the parent value of
 the target container as the parameter targetValue. The routine would also merge
 the two embedded values into one and targetValue would become a value with 0
 length.
 
 If the container was opened updateByAppend, then pass NULL in targetValue.
 
 This call would also close the container, so either call CMCloseContainer
 or CMMergeContainer on an open container, don't do both.
 
 The current implementation only handle the case where there is a single
 updating container and a single target container. Other cases would be
 treated as a CMCloseContainer.
 
*/
 
void CM_FIXEDARGS CMMergeContainer(CMconst_CMContainer container, CMValue targetValue)
{
	TOCObjectPtr					theTOCObject;
	TOCPropertyPtr				theProperty;
	ContainerPtr					targetContainer;

	TOCValueHdrPtr				thisParentValue;
	TOCValueHdrPtr 				theValueHdr;
	TOCValuePtr		 				theValue;
	CM_ULONG							prependSize;
	TOCPropertyPtr				nextProperty;

	NOPifNotInitialized(CM_NOVALUE);										/* NOP if not initialized!				*/

	targetContainer=((ContainerPtr)container)->targetContainer;
	
	if (targetContainer == (ContainerPtr)container) 			/* nothing to merge, just close */ 
		goto cannot_merge;
		
	if (targetContainer->depth > 1) { 	/* more than 1 level then merge with most recent  */ 
		targetContainer = (ContainerPtr)(((ContainerPtr)container)->containerLinks.next);
	}

	if (((ContainerPtr)container)->useFlags & kCMUpdateTarget) { 	/* updating target */
		if (targetValue == NULL)
			goto cannot_merge;												/* without target value we cannot merge */
		if (((TOCValueHdrPtr)targetValue)->size == 0)
			goto cannot_merge;												/* empty target value, do not merge		 	*/
		thisParentValue = ((ContainerPtr)container)->embeddedValue;
		if (thisParentValue == NULL)
			goto cannot_merge;												/* without parent value we cannot merge */
		if (((TOCValueHdrPtr)targetValue)->container->updatingContainer != 
						thisParentValue->container->updatingContainer)
			goto cannot_merge;	/* if the two value not in the same container we cannot merge */
		if (thisParentValue == (TOCValueHdrPtr)targetValue)
			goto cannot_merge;												/* merge with self? forget it 					*/
		prependSize = targetContainer->tocOffset;		/* amount of data we want from target		*/
	}
	else if (((ContainerPtr)container)->useFlags & kCMUpdateByAppend) {
		prependSize = 0;																/* offset is absolute								*/
		targetValue = NULL;															/* cannot have a target value				*/			
	}
	else
		goto cannot_merge;													/* we are not updating, do not merge		*/
	
	((ContainerPtr)container)->mergeInSize = prependSize;

	if (targetContainer->updateMergeCandidate) {	/* we merge updates in old containers		*/ 
		doMergeUpdate((ContainerPtr)container, (TOCValueHdrPtr)targetValue);
		return;
	}

	/* We merge by writing a single TOC with all the objects in it												*/
	if (targetContainer != ((ContainerPtr)container)->targetContainer) {/* multiple levels*/ 
		((ContainerPtr)container)->mergeInSize = 0;
		goto cannot_merge;					/* we have updates that we cannot merge in this verison */
	}
		
	if ((targetContainer->containerFlags & kCMLittleEndian) != (((ContainerPtr)container)->containerFlags & kCMLittleEndian))
		/* We are trying to merge two container with different endian-ness									*/
		/* make sure all the object id in reference object has same endian-ness							*/
		cmForEachObject(((ContainerPtr)container)->toc, CM_StdObjID_MinGeneralID, MAXUSERID,
										(CMRefCon)targetContainer, makeRefSameEndian);

	if (((ContainerPtr)container)->pointingValue)	 {									/* release pointing	*/
		CMReleaseValue(((ContainerPtr)container)->pointingValue);				/*      value      	*/
		((ContainerPtr)container)->pointingValue = NULL;
	}
	
	/* the min seed id of target container is now my min seed id													*/
	((ContainerPtr)container)->tocIDMinSeedValue->value.imm.ulongValue = 
									targetContainer->tocIDMinSeedValue->value.imm.ulongValue;
	
	theTOCObject = cmFindObject(((ContainerPtr)container)->privateTOC, CM_StdObjID_TOC);
	/* remove all the reference to the target container																		*/
	theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_NewValuesTOC);
	if (theProperty) {
		do {
			nextProperty = (TOCPropertyPtr)cmGetNextListCell(theProperty);
			if ((targetValue == NULL) && (theProperty->propertyID == CM_StdObjID_TOC_Target)) {
				/* for update by append, prop 9 is the whole target, don't free it 							*/
				theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList);
				theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);
				theValue->value.notImm.valueLen = 0;
			}
			if (theProperty->propertyID > CM_StdObjID_TOC_Free) {	/* delete non std prop			*/
				/* We need to change the offset of the values because they will be free, and we */
				/* want it to show up in the free list as the merged offset											*/
/*				offsetTheValues((ContainerPtr)container, targetContainer, theProperty); */
				cmDeleteProperty((ContainerPtr)container, theProperty);
			}
			theProperty = nextProperty;
		} while (theProperty);
	}
	if (targetValue) {																								/* upate target		 	*/
		theProperty = cmGetObjectProperty(theTOCObject, CM_StdObjID_TOC_Free);
		if (theProperty) {
			/* normally offseting the value is done in declareAsYourOwn, however the free list*/
			/* is part of TOC object and will not be handled there, so we do it here					*/
			offsetTheValues((ContainerPtr)container, targetContainer, theProperty);
		}
		/* before merging we cannot take over the free list because they are in different 	*/
		/* container, but since we are merging they are in same container so we can do it		*/
		cmTakeOverFreeList((ContainerPtr)container, targetContainer);
	}
	
	/* take each object in target and put it into my own private TOC											*/
	cmForEachObject(((ContainerPtr)container)->toc, ALLOBJECTS, MAXUSERID,
								0, declareAsYourOwn);

	((ContainerPtr)container)->usingTargetTOC = false;
	((ContainerPtr)container)->useFlags &= ~(kCMUpdateByAppend | kCMUpdateTarget); /* just close */
	((ContainerPtr)container)->targetContainer = (ContainerPtr)container;
	((ContainerPtr)container)->toc = ((ContainerPtr)container)->privateTOC;
	targetContainer->updatingContainer = targetContainer;
	
	cmFreeAllGlobalNames(&((ContainerPtr)container)->privateGlobals);
	cmFreeAllGlobalNames(&((ContainerPtr)container)->globalNameTable); /* just to decr use count */
	cmFreeAllIOBuffers(targetContainer);								/* purge ALL data structures			*/
	if (targetValue == NULL)														/* free the toc to free list			*/
		cmAddToTmpFreeList((ContainerPtr)container, targetContainer->tocOffset, 
											 targetContainer->physicalEOF - targetContainer->tocOffset);
	cmFreeTOC(targetContainer, &targetContainer->toc);				/* decrement use count			*/
	cmDestroyTOC(&targetContainer->toc, (void *)targetContainer, false, NULL);

	(*targetContainer->handler.cmfclose)(targetContainer->refCon);/* close the target 		*/
	CMfree(cmDeleteListCell(&SESSION->openContainers, targetContainer)); /* free container*/

	if (targetValue) {
		returnToParent((ContainerPtr)container, NULL, (TOCValueHdrPtr)targetValue);
		prependSize =	((ContainerPtr)container)->mergeInSize;
		((ContainerPtr)container)->useFlags &= ~kCMReuseFreeSpace;
	}
	closeContainer(container, NULL);			/* done with merging, now close container				*/

	if (targetValue) {		/* merging embedded containers, then need to merge values 			*/
		mergeValues(thisParentValue, (TOCValueHdrPtr)targetValue, prependSize);
	}
	return;
	
cannot_merge:
	CMCloseContainer(container);					/* cannot merge, just close the container				*/
	
}

/*---------------------------------------------------------------------------*
 | CMContainsValidLabel - check to see if value may be an embedded container |
 *---------------------------------------------------------------------------*
 
 Currently we just use the normal read operation to read the label and check for the
 magic byte sequence. Improve later to use the right handler if one is available and
 check for the version number as well.
*/

CMBoolean CM_FIXEDARGS CMContainsValidLabel(CMValue value)
{
	CMBoolean			result = false;
	CMSize				valueSize;
	CM_UCHAR			magicBytes[8];
	

	ExitIfBadValue(value, result);								/* validate value										*/

	valueSize = CMGetValueSize(value);
	if (valueSize >= LBLsize)
		if (CMReadValueData(value, magicBytes, valueSize-LBLsize, 8) == 8)
			result = validateMagicBytes(((TOCValueHdrPtr)value)->container, magicBytes);
	return result;
}	

/*-------------------------------------------------*
 | CMGetContainerInfo - get info about a container |
 *-------------------------------------------------*
 
 This routine returns the info from the container corresponding to the parameters.  A
 parameter may be NULL indicating that info is not needed.
*/

void CM_FIXEDARGS CMGetContainerInfo(CMconst_CMContainer container,
																		 CMGeneration CM_PTR *generation, 
																		 CM_USHORT CM_PTR *bufSize,
																		 CMContainerFlags CM_PTR *containerFlags,
																		 CMGlobalName typeName,
																		 CMContainerModeFlags CM_PTR *openMode)
{
	NOPifNotInitialized(CM_NOVALUE);								/* NOP if not initialized!						*/
	
	if (generation)		  *generation			= (CMGeneration)((ContainerPtr)container)->generation;
	if (bufSize)				*bufSize			 	= (CM_USHORT)(((ContainerPtr)container)->tocBufSize / kCMBufSizeUnits);
	if (containerFlags)	*containerFlags = (CMContainerFlags)((ContainerPtr)container)->containerFlags;
	if (typeName)				strcpy((CM_CHAR *)typeName, (CM_CHAR *)TYPENAME);
	if (openMode)
		if (((ContainerPtr)container)->useFlags & kCMWriting)
			if (((ContainerPtr)container)->useFlags & kCMUpdating)
				*openMode = (CMContainerModeFlags)kCMUpdating;
			else
				*openMode = (CMContainerModeFlags)kCMWriting;
		else
			*openMode = (CMContainerModeFlags)kCMReading;
}


/*---------------------------------------------------------------------------------------*
 | CMGetSession - return the current session pointer for the Container Manager's Globals |
 *---------------------------------------------------------------------------------------*
 
 The session global data pointer returned from CMStartSession() is passed to most of the
 handler routines defined in this file.  This routine is provided to make it easier to
 retrieve the pointer as a a function of the container refNum.
*/

CMSession CM_FIXEDARGS CMGetSession(CMContainer container)
{
	return ((container == NULL) ? NULL : (CMSession)SESSION);
}
														  
															CM_END_CFUNCTIONS
