/* @(#)Z 1.4 com/src/cm/Handlers.c, odstorage, od96os2, odos29646d 96/11/15 15:27:12 (96/10/29 09:18:46) */
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
	File:		Handlers.c

	Contains:	Container Manager Handlers Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182319 Don't allocate memory for looking
									up symbol
		 <2>	  2/3/94	EL		Bento File names are now eight chars or
									less.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<< Handlers.c >>>                             |
 |                                                                           |
 |                    Container Manager Handlers Routines                    |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/18/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 The routines in this file maintain handler/type name associations.  Handlers are defined
 in terms of their type.  They may be retrieved by using that type.
*/


#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API__
#include "CMAPI.h"
#endif
#ifndef __SYMMGR__
#include "SymTbMgr.h"      
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __UTILITYROUTINES__
#include "Utility.h"        
#endif

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment Handlers
#endif


/*-----------------------------------------------------------------*
 | enterCompare - type name comparison routine for cmEnterSymbol() |
 *-----------------------------------------------------------------*
 
 This routine is "sent" to cmEnterSymbol() to do the proper comparisons for handlers.  The
 handler tree is based on their type names.  cmEnterSymbol() is a generic binary tree
 routine that requires the comparsion to be supplied by its caller to decide on what
 basis the tree is build.  Hence this routine!
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmEnterSymbol().
*/

static int enterCompare(const void *h1, const void *h2)
{
	return (strcmp((CM_CHAR *)((MetaHandlerPtr)h1)->typeName,
								 (CM_CHAR *)((MetaHandlerPtr)h2)->typeName));
}

/*-------------------------------------------------------------------*
 | lookupCompare - type name comparison routine for cmLookupSymbol() |
 *-------------------------------------------------------------------*
 
 This routine is "sent" to cmLookupSymbol() to do the proper comparisons for global names.
 The global name tree is based on the names (obviously).  cmLookupSymbol() is a generic
 binary tree routine that requires the comparsion to be supplied by its caller to decide
 on what basis the tree is build.  Hence this routine!
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmEnterSymbol().
*/

static int lookupCompare(const void *h1, const CM_UCHAR *name)
{
	return (strcmp((CM_CHAR *)((MetaHandlerPtr)h1)->typeName, (CM_CHAR *)name));
}


/*--------------------------------------------------*
 | cmDefineMetaHandler - define a new "metahandler" |
 *--------------------------------------------------*
 
 Define a new metahandler with the specifed type (a C string). The function returns a
 pointer to the new handler or, if dup is true, a pointer to a previously defined entry.
 
 If NULL is returned then there was an allocation failure and the new type could not be
 created.
 
 Note, the global data session pointer created by CMStartSession() is passed since
 methandlers are global to all containers and thus the root of the metahandler symbol
 table is kept as part of the session data.
*/

MetaHandlerPtr cmDefineMetaHandler(CMMetaHandler metaHandler, 
																 	 const CM_UCHAR *typeName,
												 		 		 	 CMBoolean *dup,
																	 SessionGlobalDataPtr sessionData)
{
	MetaHandlerPtr h, newMetaHandler;
	
	if ((newMetaHandler = (MetaHandlerPtr)SessionMalloc(sizeof(MetaHandler) + strlen((CM_CHAR *)typeName))) != NULL) {
		strcpy((CM_CHAR *)newMetaHandler->typeName, (CM_CHAR *)typeName);	/* fill in new entry		*/
		newMetaHandler->metaHandler = metaHandler;
		
		h = (MetaHandlerPtr)cmEnterSymbol(newMetaHandler, (void **)&SessionMetaHandlerTable, dup, enterCompare);
		
		if (*dup) SessionFree(newMetaHandler);											/* oops!								*/
	} else {
		h = NULL;
		*dup = false;
	}
	
	return (h);																										/* return entry ptr			*/
}


/*---------------------------------------------------------------*
 | cmLookupMetaHandler - find a previously defined "metahandler" |
 *---------------------------------------------------------------*
 
 Find a metahandler associated with the specified type.  If found, the HandlerPtr to the
 found entry is returned (which includes the handler proc and type).  If not found NULL
 is returned.
 
 Note, we allocate a temporary handler table entry here and then free it.   If the
 allocation fails, SessionSuccess, a session status switch, is returned false.  Otherwise
 SessionSuccess is true.
	
 We used to share the same compare routine for lookup and enter symbol, but that
 involves allocation of temporary memory. That is not very inefficient. So now we
 have separate compare routine for lookup and enter symbol to elimiate the temporary
 memory allocation.
*/

MetaHandlerPtr cmLookupMetaHandler(const CM_UCHAR *typeName,
																	 SessionGlobalDataPtr sessionData)
{
	sessionData->success = true;
	return (MetaHandlerPtr)(cmLookupSymbol(typeName, SessionMetaHandlerTable, lookupCompare));
}


/*-------------------------------------------------------------*
 | cmForEachMetaHandler - do some action on each "metahandler" |
 *-------------------------------------------------------------*
 
 Do (call) the specified action for each defined metahandler in the current session. The
 pointer to each metahandler entry is passed to the action routine along with a "refCon"
 which the caller can use as a communication facility to convey additional info to the
 action routine.
*/

void cmForEachMetaHandler(CMRefCon refCon, 
													void (*action)(MetaHandlerPtr aHandler, CMRefCon refCon),
													SessionGlobalDataPtr sessionData)	
{
	cmForEachSymbol(SessionMetaHandlerTable, refCon, (SymbolAction)action);
}


/*-------------------------------------------------*
 | cmFreeAllMetaHandlers - free all "metahandlers" |
 *-------------------------------------------------*
 
 This routine is called to remove the definitions of ALL previously defined metahandlers
 for the current session.
*/

void cmFreeAllMetaHandlers(SessionGlobalDataPtr sessionData)
{
	cmFreeAllSymbols((void **)&SessionMetaHandlerTable, sessionData); /* just glue code		*/
}
														  
															CM_END_CFUNCTIONS
