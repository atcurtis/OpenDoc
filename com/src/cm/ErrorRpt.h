/* @(#)Z 1.6 com/src/cm/ErrorRpt.h, odstorage, od96os2, odos29646d 96/11/15 15:27:04 (96/10/29 09:18:11) */
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
	File:		ErrorRpt.h

	Contains:	Container Manager Error Reporting Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <2>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                         <<<    ErrorRpt.h    >>>                          |
 |                                                                           |
 |                Container Manager Error Reporting Interfaces               |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/06/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file defines the stuff the Container Manager itself needs to report errors to the
 user defined error handler.  The user never sees this stuff. The error reporting routines
 defined in     CMErrOps.c      are provided as a convenience to allow the user to
 convert error codes that we report to their corresponding messages.  The user need not
 use them however.
*/

#ifndef __ERRORRPT__
#define __ERRORRPT__

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __CM_API_ERRNO__
#include "CMAPIErr.h"			/* internally we always use    ErrorRpt.h    to get this	*/
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif


/* To make it easier and more readable for the Container Manager to call the error 			*/
/* reporter (whose pointer is in the session global data), the following macros are 		*/
/* defined.  They take 0 to 5 inserts.  They all assume that the current container 			*/
/* pointer is always contained in the variable named "container".												*/

#define ERROR(n) \
    {if(SESSION->cmReportError) \
       { \
        (*SESSION->cmReportError)((CMErrorNbr)(n)); \
       } \
    }


#define ERROR1(n,i1) \
    {if (SESSION->cmReportError) \
	 { \
	  (*SESSION->cmReportError)((CMErrorNbr)(n),(char *)(i1)); \
	 } \
    } 


#define ERROR2(n, i1, i2)  \
    {if(SESSION->cmReportError)  \
        { \
          (*SESSION->cmReportError)((CMErrorNbr)(n), (char *)(i1), (char *)(i2)); \
        }  \
    }


#define ERROR3(n, i1, i2, i3)  \
    {if(SESSION->cmReportError)  \
       { \
         (*SESSION->cmReportError)((CMErrorNbr)(n), (char *)(i1), (char *)(i2), (char *)(i3)); \
       } \
    }


#define ERROR4(n, i1, i2, i3, i4) \
    {if(SESSION->cmReportError)  \
       {  \
          (*SESSION->cmReportError)((CMErrorNbr)(n),(char *)(i1), (char *)(i2),	(char *)(i3), (char *)(i4)); \
       } \
    }


#define ERROR5(n, i1, i2, i3, i4, i5) \
   {if(SESSION->cmReportError)  \
      { \
        (*SESSION->cmReportError)((CMErrorNbr)(n), (char *)(i1), (char *)(i2), (char *)(i3), (char *)(i4), (char *)(i5)); \
      } \
   }


/* The following are the "moral equivalent" of the above, but access the error reporter	*/
/* directly through the session global data pointer. Just as the above assumed					*/
/* "container" as the container pointer, here we assume "sessionData" is the session		*/
/* data pointer.  It is used by the macro SessionError defined in the header						*/
/*      Session.h      which was included above.																				*/

#define SessionERROR(n) \
    {if(SessionError) \
       { \
         SessionError((CMErrorNbr)(n)); \
       } \
    }


#define SessionERROR1(n, i1)  \
   {if(SessionError)  \
       {  \
          SessionError((CMErrorNbr)(n), (char *)(i1)); \
       } \
   }


#define SessionERROR2(n, i1, i2) \
  {if(SessionError)  \
     {  \
        SessionError((CMErrorNbr)(n), (char *)(i1), (char *)(i2));  \
     } \
  }


#define SessionERROR3(n, i1, i2, i3) \
    {if(SessionError) \
       {  \
          SessionError((CMErrorNbr)(n), (char *)(i1), (char *)(i2),	(char *)(i3)); \
       } \
    }


#define SessionERROR4(n, i1, i2, i3, i4) \
    {if(SessionError)  \
       {  \
          SessionError((CMErrorNbr)(n),	(char *)(i1), (char *)(i2), (char *)(i3), (char *)(i4));  \
       }  \
    }



#define SessionERROR5(n, i1, i2, i3, i4, i5) \
    {if(SessionError)  \
       {  \
          SessionError((CMErrorNbr)(n), (char *)(i1), (char *)(i2), (char *)(i3), (char *)(i4), (char *)(i5));  \
       } \
    }


/* As a "safety valve" to try to protect the Container Manager, the following macro is 	*/
/* called by every routine (possibly indirectly through the validate routines described	*/
/* later).  It effectively turns a routine into a "nop" (no operation) if 							*/
/* CMStartSession() was not called.  This is all we can do.  Without the error routine 	*/
/* which is defined by CMStartSession() how do we tell the user anyway else?  Since 		*/
/* this macro can be placed in functions that return a value, the macro parameter is 		*/
/* used on the generated return statement.  The parameter must be the macro CM_NOVALUE 	*/
/* when no value is to be returned.  This gets round the preprocessor's insistance on 	*/
/* requiring an explicit macro parameter to a definition that has one!									*/

#if CMVALIDATE
#define NOPifNotInitialized(x) if (container == NULL || SESSION == NULL) return x
#else
#define NOPifNotInitialized(x)
#endif

#define CM_NOVALUE 					/* NOPifNotInitialized(CM_NOVALUE) for functs with no value */

/* Note, the above macro assumes the standard convent we use everywhere in the Container*/
/* Manager.  Namely, that the container refNum is always loaded into the variable				*/
/* "container".																																					*/

/* Caution -- the above test is NOT perfect!  If CMStartSession() is not called, the 		*/
/* odds are that CMOpen[New]Container() will most likely blow.  They test for 					*/
/* CMStartSession() by checking the global data session pointer for NULL.  We also do 	*/
/* that here as an added test.  If the user's session data pointer is indeed NULL, the 	*/
/* open routines will return NULL for the container refNum.  Then we're safe!						*/


/* The following are similar to NOPifNotInitialized(x) but are used to exit if we detect*/
/* a bad object, property, type, or value.  The "x" has the same meaning as above.			*/

#if CMVALIDATE
#define ExitIfBadObject(o, x) 	if (!cmValidateCMObject((CMObject)(o))) return x
#define ExitIfBadProperty(p, x) if (!cmValidateCMProperty((CMObject)(p))) return x
#define ExitIfBadType(t, x) 		if (!cmValidateCMType((CMObject)(t))) return x
#define ExitIfBadValue(v, x) 	  if (!cmValidateCMValue((CMValue)(v))) return x
#else
#define ExitIfBadObject(o, x)
#define ExitIfBadProperty(p, x)
#define ExitIfBadType(t, x)
#define ExitIfBadValue(v, x)
#endif

/* Note, that a NOPifNotInitialized() is implicit in these "ExitIfBad..." calls.  So 		*/
/* NOPifNotInitialized() does not need to be used if there is at least one "						*/
/* "ExitIfBad..." call.																																	*/


/* The following macros are used to get the container type name passed when the					*/
/* container was opened.  This may be used for error message inserts.  See the comments	*/
/* for CMReturnContainerName() below for additional info on this.   These macros, like 	*/
/* the ERRORn() macros always assume that the container pointer is in a variable named 	*/
/* "container".  A more general TYPENAMEx is also provided where you do supply the 			*/
/* container pointer.																																		*/

#define TYPENAME 	 TYPENAMEx(container)
#define TYPENAMEx(container) ((char *)(((ContainerPtr)container)->metaHandlerProc->typeName))


/* The following macros are the counterparts to TYPENAME and TYPENAMEx above.  But here */
/* the macros expand to calls to CMReturnContainerName() (which, by the way, is a				*/
/* handler macro defined in Handlers.h).  These are the ones that should be used for 		*/
/* error inserts.  The TYPENAME and TYPENAMEx macros are actually only used by 					*/
/* CMReturnContainerName() when no name handler is provided.														*/
	
#define CONTAINERNAME    	CONTAINERNAMEx(container)
#define CONTAINERNAMEx(container)	((char *)CMReturnContainerName((CMContainer)container))

#endif
