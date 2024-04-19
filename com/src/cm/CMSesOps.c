/* @(#)Z 1.5 com/src/cm/CMSesOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:54 (96/10/29 09:17:05) */
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
	File:		  CMSesOps.c  

	Contains:	Container Manager Session Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 9/29/94	RA		1189812: Mods for 68K build.
		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in
		 <3>	11/22/93	EL		Add refcon for alloc and free handers.
		 <2>	 10/4/93	EL		nextContainer is not used in CMEndSession

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<<   CMSesOps.c   >>>                          |
 |                                                                           |
 |                    Container Manager Session Operations                   |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  7/27/92                                  |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1994               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 The API routines in theis file handle Container Manager session operations.  All Container
 Manager calls take place in the context of a "session".  A session must be established
 before any other Container Manager calls are performed and the session closed when all
 calls are completed.
 
 Sessions are referenced in terms of session refNum's and passed to various other API
 routines.  Basically a session represents "global information" (data) the Container
 Manager needs to communicate among its internal routines.  This is information global
 to all containers of a session.
 
 There is nothing prohibiting multiple sessions, although there is no reason for it either.
 A session "refCon" is provided to allow the user to add additional information.
*/


#include <stddef.h>
#include <stdio.h>

#ifndef __CMVERSION__
#include "CMVers.h"   
#endif
#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API__
#include "CMAPI.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __BUFFEREDIO__
#include "BufferIO.h"  
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __ERRORRPT__
#include "ErrorRpt.h"      
#endif

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment SessionOps
#endif
																																		

/*----------------------------------------------------------*
 | Copyright Apple Computer, Inc. 1991-1992 by Ira L. Ruben |
 *----------------------------------------------------------*
 
 This bit of nonsense is to "bury" the copyright notice into a number of places in the
 code.  We define the notice as a string macro so that we can call this routine.  
 The call is done to make sure it is not dead-code-stripped on systems like the Mac MPW
 linker that are smart enough to do such things.

 Note, when generating the code using Mac MPW, the call will generate the copyright in the
 code since we compile with the -b2 option in the Mac MPW developement system C compiler.
 
 The actual copyright string macro, called Copyright, is defined in the header CMVersion.h.
 Note, that we repeat the copyright string here as the name of this function just to be
 "wise guys" (it comes from the Copyright_Apple_Computer macro).
*/

static char CM_PTR * CM_NEAR Copyright_Apple_Computer(char *c)
{
	return (c = Copyright);
}


/*-----------------------------------------------------------------*
 | CMStartSession - initialize the Container Manager for a session |
 *-----------------------------------------------------------------*
 
 This call is used for all global initialization of a Container Manager session.  It MUST
 be called BEFORE ANY OTHER Container Manager routine.  If not, then every API routine will
 (try to) exit without doing anything (assuming it doesn't blow up before it can detect
 that CMStartSession() wasn't called).
 
 An anonomous non-NULL pointer is returned if initialization is successful.  NULL is 
 returned for failure unless the error reporter (discussed below) aborts execution.  
 
 The returned pointer MUST be passed to every CMOpen[New]Container() that follows.  It
 represents a pointer to private Container Manager data that is global to all open
 containers.  The intent is that this data is unique to the currently running session (or
 task).  The caller may extend this data to include his or her own special per-session
 information.  This is done with the "reference constant" (refCon) passed as the last
 parameter to this routine.  It is saved in the session data.  The refCon can be anything.
 But usually it will be a pointer to the caller's own session data.  
 
 Since the session global data is global to all containers, all containers must have
 accesses to it.  That is why it is passed to CMOpen[New]Container().  Thus all accesses
 to the session data are through container refNums (CMContainer).
 
 Routines are provided to allow the user to set or get the session refCon as a function
 of a container refNum (see CMGetSessionRefCon() and CMSetSessionRefCon()).
 
 CMStartSession() takes as its main parameter the address of a special metahandler. It is 
 similar in function to CMSetMetaHandler() except that here the metahandler is container
 independent and is used to get the address of the error reporting, memory allocator and
 memory deallocator handler routines.  ALL of these handlers MUST be defined.
 
 The metahandler is called with predified operation types for the handlers.  The associated
 handler address is returned.  If any are NULL, NULL will be returned.  If we at least get
 the error handler address, then an error will be reported prior to return.  If we don't
 get it, you're dead!
 
 All three handler routines are available to the user, usually a handler writer through
 the CMMalloc(), CMFree(), and CMError() API calls.
 
 Note that the session data is allocated with the memory allocator returned from the
 metahandler.  Also the session pointer doubles as a switch which is used by all the API
 CM... routines to see if CMStartSession() was indeed called.  If it is NULL, all the
 routines just exit without doing anything.  But there is NO guarantee that the pointer
 was initialized as NULL!  Hence most likely the user will blow up before we ever get a
 chance to get at the session pointer from a container refNum.  It's the best we can do.
 Moral -- Follow the rules and call CMStartSession().
*/

CMSession CM_FIXEDARGS CMStartSession(CMMetaHandler metaHandler, CMRefCon sessionRefCon)
{
	SessionGlobalDataPtr sessionData;
	
	/* Adkins -- C++ allows static assignment of arrays at compile time only */
	#ifdef __MWERKS__
	char								 *version = CMVersion;
	#else
	char								version[] = "CMVersion"; /* Adkins -- kludge to get this to compile 
																									tonight */
	#endif
	
	MallocProto 				 cmMalloc;
	FreeProto 					 cmFree;
	ErrorProto  				 cmReportError;
	
	/* Make the Apple lawyers happy.  The following has the effect of burying a copyright	*/
	/* notice into the code and, because we're doing a call, stopping any possible dead		*/
	/* code stripping.  																																	*/
	
	Copyright_Apple_Computer(Copyright);
	
	/* Use the provided metahandler to get the routine addresses of the error reporting,	*/
	/* malloc, and free handlers.  First the error handler...															*/
	
	if (metaHandler == NULL) return (NULL);		/* safety																		*/
	
	cmReportError = *(ErrorProto)(*metaHandler)(NULL, (CMGlobalName)CMErrorOpType);
	if (cmReportError == NULL) return (NULL);
	
	/* Now we can go after the other handlers and report errors if we don't get 'em... 		*/
	
	cmMalloc = *(MallocProto)(*metaHandler)(NULL, (CMGlobalName)CMAllocOpType);
	cmFree 	 = *(FreeProto)(*metaHandler)(NULL, (CMGlobalName)CMFreeOpType);
	
	if (cmMalloc == NULL || cmFree == NULL) { 
		(*cmReportError)(CM_err_MissingHandler);
		return (NULL);
	}
	
	/* Allocate the space for the session (task) global data area...											*/
	
	sessionData = (SessionGlobalDataPtr)(*cmMalloc)((CMSize)(sizeof(SessionGlobalData) + strlen(version)), sessionRefCon);
	if (sessionData == NULL) {
		(*cmReportError)(CM_err_NoSession);
		return (NULL);
	}
		
	/* Initialize the session global data...
																													 Prime user (client)					*/
	sessionData->cmMalloc				 	= cmMalloc;							/* Handlers.h										*/
	sessionData->cmFree					 	= cmFree;								/* Handlers.h										*/
	sessionData->cmReportError 	 	= cmReportError;				/*    ErrorRpt.h   							*/
	sessionData->cmTocTblSize		 	= DefaultIndexTableSize;/*    CMCntOps.c   							*/
	sessionData->cmDbgFile				= NULL;									/* anywhere											*/
	sessionData->metaHandlerTable = NULL;									/* Handlers.c										*/
	sessionData->refCon					  = sessionRefCon;				/*   CMHndOps.c  								*/
	sessionData->success					= true;									/* anywhere											*/
	sessionData->aborting					= false;								/*    CMCntOps.c   							*/
	cmInitList(&sessionData->openContainers);							/*    CMCntOps.c   							*/
	strcpy(sessionData->cmVersion, version);							/* actually no one!							*/
	
	#if CMVALIDATE
	sessionData->validate					= CMDEFAULT_VALIDATE;		/*    ErrorRpt.h   							*/
	#endif
	
	return ((CMSession)sessionData);					/* success																	*/
}


/*------------------------------------------------------*
 | CMEndSession - terminate a Container Manager session |
 *------------------------------------------------------*
 
 This should be called as the LAST call to the Container Manager.  If frees the space
 allocated for the session by CMStartSession() and optionally calls CMCloseContainer() on
 all remaining open containers.
 
 The sessionData specifies the session data pointer returned from CMStartSession().  If
 closeOpenContainers is passed as 0 (i.e., "false"), then an error is reported for each
 container that has NOT been explicitly closed by CMCloseContainer(). If true (non-zero) is
 specified, then the Container Manager will call CMCloseContainer() for you for each
 remaining open container.
 
 The difference between specifying closeOpenContainers as false, as opposed to true, is
 that open containers are NOT formally closed.  They remain in whatever (open) state they
 currently happen to be in when the error is reported.  This is generally true whenever
 any error is reported.  When true is passed, open containers are not treated as an error
 condition, and closed by calling the API CMCloseContainer() routine just as any API caller
 would.  This case can be viewed as a "close all" case.
 
 No further calls should be done once this routine is called.  All memory occupied by the
 containers, as well as the session itself are freed.
*/

void CM_FIXEDARGS CMEndSession(CMSession sessionData, CMBoolean closeOpenContainers)
{
	SessionGlobalDataPtr session = (SessionGlobalDataPtr)sessionData;
	ContainerPtr container;

	if (sessionData == NULL) return;							/* exit if there is no session to close	*/
	
	/* Go through the list of open containers and do what it says above...								*/
	
	container = (ContainerPtr)cmGetListHead(&session->openContainers);
	
	while (container != NULL) {										/* look at each open container...				*/
		if (closeOpenContainers)										/* if user wants US to close it...			*/
			CMCloseContainer((CMContainer)container);	/* ...we'll do it for the "lazy" user		*/
		else {																			/* if open containers aren't expected...*/
			ERROR1(CM_err_StillOpen, CONTAINERNAME);	/* ...YELL loudly!											*/
			cmFreeAllGlobalNames(&container->globalNameTable); /* if error reporter returns...*/
			cmFreeAllIOBuffers(container);										 /* ...free any I/O buffers			*/
			cmFreeTOC(container, &container->toc);						 /* ...free container memory		*/
			CMfree(cmDeleteListCell(&session->openContainers, container));
		}
		
		/* We always use the container list head because the open container queue could			*/
		/* contain embedded containers.  These "go away" implicitly by closing the embedde's*/
		/* parents.  That will remove them from the open queue.  By using the queue head we	*/
		/* always get the "next" available truly open container.														*/
		
		container = cmGetListHead(&session->openContainers);
	}
	
	/* Now that the containers are disposed of, we can free the session data.  This	*/
	/* is the methandler table and the session data itself.  After that the user is on 		*/
	/* his (or her) own!																																	*/
	
	cmFreeAllMetaHandlers(session);								/* there go the metahandler names				*/
	
	SessionFree(session);													/* and there goes the session itself		*/
}


/*----------------------------------*
 | CMAbortSession - abort a session |
 *----------------------------------*
 
 ALL containers are closed WITHOUT further writing to those containers, i.e., as if all
 containers were opened for reading even when opened for writing.  The session is then 
 closed with CMEndSession().  This is intended to be used to abort the session from
 unrecoverable errors.
 
 All memory allocated by ALL the container data structures are freed (if possible) and the
 container close handlers called to physically close the containers.  All dynamic values
 currently in use are released in an attempt to allow them to properly clean up any files
 and memory allocated by their handlers.  No further API calls should be done.
 
 Note: CMAbortSession() is basically a CMAbortContainer() for EACH currently open container
 followed by a CMEndSession().  Also, this routine WILL return to its caller.  It is up to
 the user to actually abort execution if that is required.
*/

void CM_FIXEDARGS CMAbortSession(CMSession sessionData)
{
	if (sessionData == NULL) return;											/* exit if there is no session	*/

	if (((SessionGlobalDataPtr)sessionData)->aborting)		/* ignore recursive aborts!			*/
		return;											
	
	((SessionGlobalDataPtr)sessionData)->aborting = true;	/* throw the deadly switch			*/
	
	CMEndSession(sessionData, true);											/* abort all containers 				*/
}


/*--------------------------------------------------------*
 | CMGetSessionRefCon - return the current session refCon |
 *--------------------------------------------------------*
 
 This routine can be used to get at the user's session refCon saved as part of the session
 data created by CMStartSession().  The session data is "tied" to each container created by
 CMOpen[New]Container().  Thus the refCon is accessed via a container refNum.
*/

CMRefCon CM_FIXEDARGS CMGetSessionRefCon(CMContainer container)
{
	return ((container == NULL || SESSION == NULL) ? NULL : SESSION->refCon);
}


/*-----------------------------------------------------*
 | CMSetSessionRefCon - change the session data refCon |
 *-----------------------------------------------------*
 
 This routine may be called to change the user's session refCon associated with the session
 data.  As with CMGetSessionRefCon() above, the session data is accessed through a
 container refNum.
*/

void CM_FIXEDARGS CMSetSessionRefCon(CMContainer container, CMRefCon refCon)
{
	SESSION->refCon = (container == NULL || SESSION == NULL) ? NULL : refCon;
}
														  
															CM_END_CFUNCTIONS
