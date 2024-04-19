/* @(#)Z 1.7 com/src/cm/Session.h, odstorage, od96os2, odos29646d 96/11/15 15:27:14 (96/10/29 09:19:12) */
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
	File:		     Session.h     

	Contains:	Container Manager Session (task) Global Data Definitions

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in
		 <2>	11/22/93	EL		Add refcon for alloc and free handers.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                        <<<      Session.h      >>>                        |
 |                                                                           |
 |         Container Manager Session (task) Global Data Definitions          |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  4/9/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1994               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 Although it is kept to a minimum, the Container Manager needs some global data!  This
 header defines that data.  It is defined in terms of a struct to group all the globals
 in one place.  The space for the struct is dynamically allocated by CMStartSession(). This
 allows the global data to be associated with a particular running session or task.  Using
 static global data is not necessary supported in some configurations of the Container
 Manager.  For example, in some DLL environments, static global data is not supported.
 
 The scheme chosen here is the most painless (unless you're me who had to change all the
 code to use this thing). The SessionGlobalData struct is known only to the Container
 Manager.  It is allocated by CMStartSession() and returned as an anonomous pointer to be
 passed to the container open routines, CMOpen[New]Container().  All routines, except 
 ones global to all containers, access the session data through a container refNum.
 
 There are some routines that are global to all containers.  Specifically, they are the
 handler operations (e.g, CMSetMetaHandler(), CMGetMetaHandler(), etc.).  All of these
 take the session data pointer explicitly as one of their parameters.
*/

#ifndef __SESSIONDATA__
#define __SESSIONDATA__

#include <stdio.h>
#include <setjmp.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif

struct MetaHandler;
																	CM_CFUNCTIONS


/*----------------------------*
 | Session Handler Prototypes |
 *----------------------------*
 
 There are curently three session handlers whose addresses are returned through the session
 metahandler passed to CMStartSession().  These handlers have the following prototypes:
*/
 
typedef void CM_PTR *(CM_FIXEDARGS *MallocProto)(CMSize size, CMRefCon sessionRefCon);
typedef void (CM_FIXEDARGS *FreeProto)(CMPtr ptr, CMRefCon sessionRefCon);
#ifdef _PLATFORM_OS2_
typedef void (CM_VARARGS * _System ErrorProto)(CMErrorNbr errorNumber, ...);
#else
typedef void (CM_VARARGS *ErrorProto)(CMErrorNbr errorNumber, ...);
#endif


/*-----------------------------------------------------*
 | SessionGlobalData - all global data is defined here |
 *-----------------------------------------------------*
 
 Note that the Container Manager version number is placed in the session global data as 
 the last item.  It is variable length so it must come at the end to allow for the 	
 proper dynamic allocation. This makes it harder to find, but it sould not be a problem.
*/

struct SessionGlobalData {							/* Session global data layout:									*/
	MallocProto cmMalloc;									/* 		handler's version of malloc()							*/
	FreeProto 	cmFree;										/* 		handler's version of free()								*/
	ErrorProto 	cmReportError; 						/* 		error reporter														*/
	
	jmp_buf cmForEachGlobalNameEnv;				/* 		AbortForEachGlobalName() setjmp env.			*/
	jmp_buf cmForEachObjectEnv;						/* 		AbortForEachObject() setjmp env.					*/
	
	CM_ULONG cmTocTblSize;						/* 		size of TOC index tables									*/
	
	FILE *cmDbgFile;											/* 		debugging trace file											*/
	
	#if CMDUMPTOC
	CM_ULONG currTOCoffset;					/* 		current input TOC offset for debugging		*/
	CMBoolean gotExplicitGen;								/*		true ==> explicit generation nbr read			*/
	#endif
	
	ListHdr openContainers;								/* 		list of open containers										*/
	struct MetaHandler *metaHandlerTable;	/* 		the root of the handler tree							*/
	
	CMRefCon refCon;											/* 		pointer to additional user supplied data	*/
	
	#if CMVALIDATE
	CMBoolean validate;											/* 		true ==> do refNum validations 						*/
	#endif
	
	unsigned char scratchBuffer[256];			/*	 	ptr to a general short-term scratch buffer*/
	CMBoolean success;											/*		special function result status						*/
	CMBoolean	aborting;											/*		in "abort" state (abnormal termination)		*/
	
	CM_CHAR cmVersion[1];										/*	 	start of CM version nbr (MUST be last) 		*/
};
typedef struct SessionGlobalData SessionGlobalData, *SessionGlobalDataPtr;


/* By convention, the variable "container" will always be used to point to the current	*/
/* container.  The following macro is used to access the session global data through 		*/
/* container:																																						*/

#define SESSION (((ContainerPtr)container)->sessionData)


/* Note, some routines must deal directly with the session data instead of accessing it */
/* through the container. By convention, the variable "sessionData" will always be used */
/* as the session pointer.  To make life a little simpler, the following macros are 		*/
/* defined to access some of the fields in the session data:														*/

#define SessionRefCon						(((SessionGlobalDataPtr)sessionData)->refCon)
#define SessionError 						(*(((SessionGlobalDataPtr)sessionData)->cmReportError))
#define SessionMalloc(size)  		(*(((SessionGlobalDataPtr)sessionData)->cmMalloc))((CMSize)(size),SessionRefCon)
#define SessionFree(ptr)  			(*(((SessionGlobalDataPtr)sessionData)->cmFree))((CMPtr)(ptr),SessionRefCon)

#define SessionMetaHandlerTable	(((SessionGlobalDataPtr)sessionData)->metaHandlerTable)


/* The validate switch in the session data is controlled by CMDebugging() if 						*/
/* CMDebugging() itself is allowed to be generated (under its CMDEBUGGING switch macro 	*/
/* controlled by the header CMTypes.h). It is preset to CMDEFAULT_VALIDATE (also defined*/
/* in CMTypes.h to keep if hidden from the user).																				*/

/* The validate switch allows for dynamic controlling of the refNum validations and also*/
/* checking for CMStartSession() being called. Thus the code checks can be toggled on or*/
/* off through CMDebugging().																														*/

/* There is also a static switch to suppress the validation code entirely. It is called */
/* CMVALIDATE, also defined in CMTypes.h.  Of course, if CMVALIDATE is 0, ALL the				*/
/* validations are suppressed and the dynamic switch has no effect.											*/

/* Remember, suppressing the validation checks removes what little protection the				*/
/* Container Manager has against careless users.  */

/* I am getting off the track.  The whole point here is to define a macro to make it		*/
/* easier to access the validate switch...																							*/

#if CMVALIDATE
#define VALIDATE (SESSION->validate)
#else
#define VALIDATE 0
#endif

/* Since this uses the SESSION macro, container is assumed to be the container pointer.	*/

/* Also, since we usually get at the container via a refNum, and assuming the validation*/
/* code is not suppressed (CMVALIDATE was 1), we must at least always validate that a		*/
/* refNum is not NULL before we use it to go after the session data pointer to see if		*/
/* all further validation checking is being dynamically suppressed.											*/


/* The following allows access to scratchBuffer as a function of the current container 	*/
/* pointer via the SESSION macro defined above.																					*/

#define ScratchBufr (SESSION->scratchBuffer)

/* The following serves the same purpose but uses the session data pointer directly. As	*/
/* above, it is assued this pointer is in the variable "sessionData".										*/

#define SessionScratchBufr	(((SessionGlobalDataPtr)sessionData)->scratchBuffer)


/* Some routines, for example, some of the lookup routines like the metahandler lookup,	*/
/* need to do "malloc"s to allocate special temporaries to do their work.	 There are 		*/
/* vary few routines like this.  "Malloc"s, of course, are subject to failure.  So we 	*/
/* need a way for these routines to report such an error status.  It is confusing and 	*/
/* inconvenient to have an extra explicit parameter for these routines since the return	*/
/* value is always used to indicate a found/not found status.  So the session global,		*/
/* "success", is provided for this purpose.  																						*/

/* The preceding is one use for the switch. It can also be used as an "separate channel"*/
/* to report other status as well.  Fo example, indicating success or failure for 			*/
/* setjmp/longjmp's, since you cannot use the returned int from a setjmp in a pure ANSI */
/* envoironment!																																				*/

/* The following macro is provided to interrogate the success/failure status.  This 		*/
/* assumes "container" is the current container pointer.	Note, routines that use this	*/
/* switch must always set it both ways as appropriate. Never assume the previous setting*/
/* of this switch.																																			*/

#define SessionSuccess (SESSION->success)

														  CM_END_CFUNCTIONS
#endif
