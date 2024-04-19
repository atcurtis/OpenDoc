/* @(#)Z 1.4 com/src/cm/CMHndOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:47 (96/10/29 09:16:43) */
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
	File:		CMHndOps.c

	Contains:	Container Manager Handler Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<<   CMHndOps.c   >>>                          |
 |                                                                           |
 |                    Container Manager Handler Operations                   |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/05/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 Containers are always access through handlers, to provide platform independence.  Handlers
 are responsible for doing all the I/O operations, including opening and closing, to
 containers.  They are also responsible for reading and writing the container label.
 
 The routines in this file are the API interfaces to allow the Container Manager to
 determine what handlers it should use for a particular container.  The Container Manager
 handler routines defined here maintain container type name/metahandler associations.
 
 A metahandler is a special required routine the API user defines which is called by the
 API to return the address of the handler I/O routines that the Container Manager needs.
 There following handlers are needed by the API.  They should have these prototypes:	
 
 void open_Handler(CMRefCon refCon, CMOpenMode mode);
 void close_Handler(CMRefCon refCon);
 CMSize flush_Handler(CMRefCon refCon);
 CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode);
 CMSize tell_Handler(CMRefCon refCon);
 CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
 CMSize write_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
 CMEofStatus eof_Handler(CMRefCon refCon);
 CMBoolean trunc_Handler(CMRefCon refCon, CMSize containerSize);
 CMSize containerSize_Handler(CMRefCon refCon);
 void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
												CMContainerFlags *flags, CM_USHORT *bufSize,
												CM_USHORT *majorVersion, USHORT *minorVersion,
												CMSize *tocOffset, CMSize *tocSize);
 void writeLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
												 CMContainerFlags flags, CM_USHORT bufSize,
												 CM_USHORT majorVersion, USHORT minorVersion,
												 CMSize tocOffset, CMSize tocSize);	
 CMValue returnParentValue_Handler(CMRefCon refCon);
 CM_UCHAR *returnContainerName_Handler(CMRefCon refCon);
 CMType returnTargetType_Handler(CMRefCon refCon, CMContainer container);
 void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer, 
 													CMSize size, CMPrivateData data);
 void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer, 
 												 CMSize size, CMPrivateData data);

 The names chosen can be, of course, anything.  They were chosen here to document what
 each routine does since the semantics for each routine follow exactly those of standard
 ANSI C.  The last two routines are for reading and writing labels.  See documentation of
 those routines for further details.  Note, we will talk about the "refCon" a little later.
 
 Not all of the above handlers are required at all times.  Some are obviously used only
 for input and others for output.  Here is a chart as to which  routines are required for
 reading, writing, and updating:
 
                                        Mode
													 Reading  |  Writing  | Updating
													 ---------+-----------+---------
								open         	X     |     X     |    X
								close    	    X     |     X     |    X
								flush    	          |           |
								seek    	  	X     |     X     |    X
								tell      		X     |     X     |    X
								read       		X     |           |    X
								write    	          |     X     |    X
								eof    		          |           |
								trunc					      |           |
								size          X     |     X     |    X
								readLabel     X     |           |    X
								writeLabel          |     X     |    X
								parent	  		1     |     1     |    
								name                |           |     
								targetType          |           |    X
								extract       X     |     X     |    X
								format        X     |     X     |    X
													 ---------+-----------+---------

 Notes: 1. The parent value handler is required ONLY for embedded container handlers.
				
 The X's indicate required for the mode.  Blanks mean optional or are not used for the
 mode.  Note, that updating generally is an or'ing of the reading and writing cases.
	
 In each handler routine a "refCon" is passed.  This is short for "reference constant". It
 is an arbitrary (void *) value that is passed to the Container Manager when a container
 is to be opened.  The Container Manager doesn't look at it. It simply passes it on each
 handler call.  The intent is that it will be used as a communication device to allow the
 handler routines to coordinate among themselves.
*/


#include <stddef.h>
#include <string.h>
#include <stdarg.h>
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
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
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

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment CMHandlerOps
#endif


struct GetOpComArea {							/* CMGetOperation() object walking comm area layout:	*/
	TOCValuePtr 	 theValue;				/*		the first (should be only) global name value		*/
	CM_USHORT			 nbrOfGlobalNames;/*		count of nbr of global names found in object		*/
};
typedef struct GetOpComArea GetOpComArea, *GetOpComAreaPtr;


/*-----------------------------------------------------------------------*
 | CMSetMetaHandler - associate a container type name with a metahandler |
 *-----------------------------------------------------------------------*
 
 This routine records the association of type names with their metahandlers. A type name
 is passed to CMOpen[New]Container() so that the Container Manager can use it to call the
 appropriate metahandler to get the actual handler addresses.  Further, a value that has
 a global type name which has an associated metahandler will produce a "dynamic" value.
 
 The function returns the address of the previous metahandler for the specified type if
 one exists.  Otherwise NULL is returned.
 
 Note, that type name/metahandler associations are global to all containers.  Therefore,
 the Container Manager neest to store its information as a function of the current session
 global data defined by CMStartSession().  Hence the sessionData pointer should always be
 the value returned by CMStartSession().
*/

CMHandlerAddr CM_FIXEDARGS CMSetMetaHandler(CMconst_CMSession sessionData,
																						CMconst_CMGlobalName typeName,
													 		 							CMMetaHandler metaHandler)
{
	Boolean 	 	 	 dup;
	CMMetaHandler  prevMetaHandler;
	MetaHandlerPtr h;
	
	if (sessionData == NULL) return (NULL);			/* NOP if not initialized!								*/
	
	/* Enter the new metahandler and its type name into the metaHandler symbol table. The	*/
	/* address of the new entry is returned.  If there is one already there, the address 	*/
	/* of that is returned, dup is set true, and nothing else done.												*/
	
	h = cmDefineMetaHandler(metaHandler, (CM_UCHAR *)typeName, &dup,
													(SessionGlobalDataPtr)sessionData);
	
	if (!h)	{																		/* if allocation failed...								*/
		SessionERROR1(CM_err_NoHandler, typeName);/* yell and do nothing else									*/
		return (NULL);														/* ...that's what I said!									*/
	}
	
	/* If there was a previous entry, update it with the new metahandler address and 			*/
	/* return the "old" address.																													*/
	
	if (dup)	{																	/* if dup entry...												*/
		prevMetaHandler = h->metaHandler;					/* ...we will return "old" metahandler		*/
		h->metaHandler = metaHandler;							/* ...just replace with new metahandler		*/
	} else
		prevMetaHandler = NULL;										/* if not dup, there is no prev. handler	*/
	
	return ((CMHandlerAddr)prevMetaHandler);
}


/*-------------------------------------------------------------------------------*
 | CMGetMetaHandler - return the metahandler address associated with a type name |
 *-------------------------------------------------------------------------------*
 
 This function searches the metaHandler symbol table for the specified typeName and returns
 the associated metahandler address.  NULL is returned if the type name does not exist.
 
 As with CMSetMetaHandler() above, CMGetMetaHandler() requires the session global data
 pointer returned by CMStartSession().
*/

CMHandlerAddr CM_FIXEDARGS CMGetMetaHandler(CMconst_CMSession sessionData,
																						CMconst_CMGlobalName typeName)
{
	MetaHandlerPtr h;
	
	if (sessionData == NULL) return (NULL);							/* NOP if not initialized!				*/

	h = cmLookupMetaHandler((CM_UCHAR *)typeName, (SessionGlobalDataPtr)sessionData);
	
	if (!((SessionGlobalDataPtr)sessionData)->success) {/* if we had allocation error...	*/
		SessionERROR1(CM_err_HandlerError, typeName);			/* ...yell												*/
		h = NULL;
	}

	return (h ? (CMHandlerAddr)h->metaHandler : NULL);	/* NULL or found metahandler			*/
}


/*---------------------------------------------------------*
 | checkValue - check a value to see if it's a global name |
 *---------------------------------------------------------*
 
 This is a cmWalkObject() action routine initiated by CMGetOperation() below to find the
 global name value in an object. The "refCon" is a pointer to a GetOpComArea communication
 area where we will save the value pointer.  We also count the number of global names
 seen in the object.  It shoould be 1.  But we walk the entire object just to be sure.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmWalkObject().
*/
 
static TOCWalkReturns checkValue(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon)
{
	GetOpComAreaPtr g = (GetOpComAreaPtr)refCon;
	ContainerPtr		unused = container;
	
	if (theValue->flags & kCMGlobalName) {
		g->theValue = theValue;													/* save global name ptr							*/
		++g->nbrOfGlobalNames;													/* count it too											*/
	}
	
	return (WalkNextTOCValue);												/* continue value walk							*/
}


/*--------------------------------------------------------------------------*
 | CMGetOperation - get handler for a specific operation on a specific type |
 *--------------------------------------------------------------------------*
 
 This routine takes a targetType which is defined as a globally unique name and uses that
 name find a metahandler. The matahandler, in turn, is used to get the handler routine
 address for the specified operationType.  The function returns the resulting address.
 
 Metahandler proc addresses are given to the Container Manager by calls to
 CMSetMetaHandler().  The global name value for the input targetType is treated as the
 typeName to find the metahandler.  As just mentioned above, the  metahandler is called
 with the operationType to get the operation address returned.
*/

CMHandlerAddr CM_FIXEDARGS CMGetOperation(CMType targetType,
																					CMconst_CMGlobalName operationType)
{	
	TOCObjectPtr 	 theObject;
	TOCValuePtr		 theValue;
	ContainerPtr 	 container;
	CM_CHAR	 			 *typeName;
	MetaHandlerPtr metaHandler;
	GetOpComArea 	 getOpComArea;
	
	ExitIfBadType(targetType, NULL);									/* validate targetType							*/

	theObject = (TOCObjectPtr)targetType;
	container = theObject->container;
		
	/* Walk the entire object and look at all the values to find the value for a global		*/
	/* name.  The object can have other properties and values, but there better be only 	*/
	/* one global name.  The object is walked with cmWalkObject().  The "refCon" we pass	*/
	/* is a pointer to a communication area which will hold the count of the global name	*/
	/* values and the pointer to it.  As just mentioned, if the count comes back other 		*/
	/* than 1, we have an error.																													*/
	
	getOpComArea.theValue = NULL;											/* init value ptr to no value yet		*/
	getOpComArea.nbrOfGlobalNames = 0;								/* there are no global names yet too*/
	
	cmWalkObject(container, theObject, &getOpComArea, NULL, NULL, NULL, checkValue);
	
	/* Get the global name for this type. The type must have only one property, one value,*/
	/* and that value must be for a global name.																					*/
		
	if (getOpComArea.nbrOfGlobalNames > 1) {					/* must have exactly 1 global name	*/
		ERROR2(CM_err_AmbiguousType, "CMGetOperation", CONTAINERNAME);
		return (NULL);
	}

	if (getOpComArea.nbrOfGlobalNames == 0) {
		ERROR2(CM_err_TypeNotGlobal, "CMGetOperation", CONTAINERNAME);
		return (NULL);
	}
	
	/* Set the typeName to point at the global name now that we're happy there is one...	*/
	
	theValue = getOpComArea.theValue;
	if (theValue == NULL) return (NULL);							/* safety														*/
	typeName = GetGlobalName(theValue->value.globalName.globalNameSymbol);
	
	/* Use the global name to look up the metahandler in the meta handler symbol table.		*/
	/* A few things can go wrong here, but if we successfully found a metahandler symbol	*/
	/* table entry we use the resulting metahandler proc pointer to get the routine				*/
	/* address for the input operationType.  That's what we return.												*/
	
	if ((metaHandler = cmLookupMetaHandler((CM_UCHAR *)typeName, SESSION)) == NULL) { 
		ERROR1(CM_err_UndefMetaHandler, typeName);
		return (NULL);
	}
	
	if (!SessionSuccess) {														/* if allocation error...						*/
		ERROR1(CM_err_HandlerError, typeName);					/* ...yell													*/
		return (NULL);
	}
	
	return ((CMHandlerAddr)(*metaHandler->metaHandler)(NULL, operationType));
}


/*---------------------------------------------------------------------------*
 | CMMalloc - handler/user interface to Container Manager's memory allocator |
 *---------------------------------------------------------------------------*
 
 This routine provides a access path for the user (usually a handler writer) to use the
 same memory management allocator defined for the current Container Manager session.  size
 bytes are allocated as defined by that handler.  
 
 The session memory allocator handler is defined by the metahandler passed to
 CMStartSession().  The sessionData is the current session refNum returned from
 CMStartSession().
*/

void CM_PTR * CM_FIXEDARGS CMMalloc(CMSize size, CMSession sessionData)
{
	return ((sessionData == NULL) ? NULL : SessionMalloc(size));
}


/*---------------------------------------------------------------------------*
 | CMFree - handler/user interface to Container Manager's memory deallocator |
 *---------------------------------------------------------------------------*
 
 This routine provides a access path for the user (usually a handler writer) to use the
 same memory management deallocator defined for the current Container Manager session.  A
 pointer (ptr) assumed to be allocated by CMMalloc() is passed to release the memory in the
 manner defined by the handler.  

 The session memory deallocator handler is defined by the metahandler passed to
 CMStartSession().  The sessionData is the current session refNum returned from
 CMStartSession().
*/

void CM_FIXEDARGS CMFree(CMPtr ptr, CMSession sessionData)
{
	if (sessionData != NULL) SessionFree(ptr);
}


/*--------------------------------------------------------------------------------*
 | CMError - handler/user interface to Container Manager's session error reporter |
 *--------------------------------------------------------------------------------*
 
 This routines provides an access path for the user (usually an handler writer) to use the
 same error reporter defined for the current Container Manager session.  The session error 
 reporting handler is defined by the metahandler passed to CMStartSession().  The
 sessionData is the current session refNum returned from CMStartSession().  The string is
 the message to be processed by the handler.  Optional string inserts can be passed to put
 into the string.  The string indicates the position of the inserts by "^0", "^1", etc.
 "^0" gets the first insert, "^1" the second, and so on.  Inserts may be repeated.
 
 Note, the maximum length of the passed message plus its inserts is limited to 256 
 characters.  Also, if the error reporting handler returns, so with this routine.
*/

void CM_VARARGS CMError(CMSession sessionData, CMErrorString message, ...)
{
	va_list inserts;
	
	va_start(inserts, message);
	CMVError(sessionData, message, inserts);
	va_end(inserts);
}


/*---------------------------------------------------------------------------------*
 | CMVError - handler/user interface to Container Manager's session error reporter |
 *---------------------------------------------------------------------------------*
 
 This routine is the same as CMError() above, except that the extra message inserts are
 given as a variable argument list as defined by the "stdarg" facility.
 
 This routine assumes the caller sets up and terminates the variable arg list using the
 "stdarg.h" calls as follows:
 
 			#include <stdarg.h>
			
 			callersRoutine(args, ...)
			{
				va_list inserts;
				
				- - -
				
				va_start(inserts, args);
				CMVError(sessionData, message, inserts);
				va_end(inserts);
				
				- - -
			}
			
 See CMError() for further details.
*/

void CM_FIXEDARGS CMVError(CMSession sessionData, CMErrorString message, va_list inserts)
{
	if (sessionData != NULL) {																		/* if session defined...*/
		strcpy((CM_CHAR *)SessionScratchBufr, message);								/* ...use private bufr	*/
		CMVAddMsgInserts((CM_CHAR *)SessionScratchBufr, 256, inserts);	/* ...add in inserts		*/
		SessionERROR1(CM_err_GenericMessage, SessionScratchBufr);		/* ...report error			*/
	}
}
														  
															CM_END_CFUNCTIONS
