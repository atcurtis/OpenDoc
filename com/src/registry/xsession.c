/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odconfig
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
/* @(#) 1.4 com/src/registry/xsession.c, odconfig, od96os2, odos29646d 10/31/96 12:45:28 [11/15/96 15:49:47] */
/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                      <<<        XSession.c        >>>                     |
 |                                                                           |
 |                 Example Container Manager Session Handlers                |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  2/5/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992                    |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file contains a fully documented example of the set of session handlers and its
 metahandler as it would be used by the Container Manager CMStartSession() routine.

 When you call CMStartSession() you pass a pointer to a metahandler that is used to get
 the addresses of the error reporting, memory allocator and memory deallocator routines.
 Possibly others might be defined in the future.

 This file contains an example of the session metahandler and the associated handler
 routines.

 Note, this is a working example.  It is used in conjunction with the other handler
 examples in the files    XHandlrs.c     and ExampleEmbeddedHandlers.c.
*/


/*------------------------------------------------------------------------------------*
 | DOS (80x86) USERS -- USE THE "LARGE" MEMORY MODEL WHEN COMPILED FOR 80X86 MACHINES |
 *------------------------------------------------------------------------------------*

 The Container Manager is built with this same option and assumes all handler and
 metahandler interfaces are similarly built and can be accessed with "far" pointers.
*/


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "CMAPI.h"
#include "xsession.h"

#ifdef _PLATFORM_OS2_
#include <ODPagtun.h>
#endif

                                  CM_CFUNCTIONS

/* See comments in      CMAPIEnv.h      about CM_CFUNCTIONS/CM_END_CFUNCTIONS (used      */
/* only when compiling under C++).                                                      */


/*----------------------------------------------------------------------------------*
 | sessionRoutinesMetahandler - metahandler to return the session handler addresses |
 *----------------------------------------------------------------------------------*

 As mentioned above, you pass the address of a metahandler that will return the address
 of each session routine required by the Container Manager. The address of the metahandler
 is passed to CMStartSession().  It, in turn calls the specified routine to get the
 address for each session routine.  The operationType parameter defines which routine
 is requested.

 For each call a unique operation type string is passed to indicate which routine address
 is to be returned.  There are 3 session handlers needed by the API.  They should have
 the following prototypes:                                                              */

 static void CM_VARARGS error_Handler(CMErrorNbr errorNumber, ...);
 static void CM_PTR * CM_FIXEDARGS alloc_Handler(CMSize size);
 static void CM_FIXEDARGS free_Handler(CMPtr ptr);
                                                                                        /*
 The documentation for these routines is specified below as each routine is defined so we
 will not repeat it here.  Note, that we actually declare all these routines as "static"
 since they only need to be visible in this file.  The API gets at them through the
 addresses we return from here.

 In the code below, you will see a list of macro calls for the operationType strings. These
 are defined in     CMAPIIDs.h     which is included by CM_API.h.

 The API generalizes the use of metahandlers to allow specific metahandlers for specific
 type objects within a container.  This is the targetType argument.  All metahandlers have
 the same prototype, but when used by CMStartSession(), the targetType will always be NULL.
*/

CMHandlerAddr CM_FIXEDARGS regSessionRoutinesMetahandler(CMType targetType, CMconst_CMGlobalName operationType)
{
  static char *operationTypes[] = {CMErrorOpType,          /*  0 */ /*  Operation Types  */
                                   CMAllocOpType,          /*  1 */
                                   CMFreeOpType,          /*  2 */
                                   NULL};
  char    **t;
  CMType ignored = targetType;

  /* Look up the operation type in the operationTypes table above...                    */

  t = operationTypes - 1;
  while (*++t) if (strcmp((char *)operationType, *t) == 0) break;

  /* Now that we got it (hopefully), return the appropriate routine address...          */

  switch (t - operationTypes) {
    case  0:  return ((CMHandlerAddr)error_Handler);            /* CMErrorOpType         */
    case  1:  return ((CMHandlerAddr)alloc_Handler);            /* CMAllocOpType         */
    case  2:  return ((CMHandlerAddr)free_Handler);              /* CMFreeOpType         */

    default:  return (NULL);                                    /* huh?                  */
  }
}


/*-----------------------------------------*
 | error_Handler - error reporting handler |
 *-----------------------------------------*

 The error reporting handler is a required session routine whose address is asked for
 by CMStartSession().  All Container Manager errors are reported through here.

 The Container Manager API makes available some of the same routines used internally.
 Specifically, the ability to take an string that can contain inserts and "edit in" those
 inserts (CM[V]AddMsgInserts()).

   CMAPIErr.h   defines the meaning for each error number in the comments along side the
 number's definition.  A typical definition looks like the following:                                                                              */

 #define CM_err_WhatEver 1            /* Example error msg with inset ^0 and ^1          */
                                                                                        /*
 The ^0, ^1, etc. represent places for insertion strings that are also passed to the
 error handler in the "..." portion of the parameter list.  A ^0 corresponds to the first
 insert, ^1 the second, and so on.  The inserts can appear in any order and more than once.

 This is an example of an error handler that simply prints the the meaning of the error
 number with the inserts placed in the indicated spots according to the meanings defined
 in CM_API_Errno.h.

 The Container Manager API provides routines to allow an error handler to convert the
 error number and its associated insert to the messages shown in the comments. In this
 example error hander we use one of those routines, CMVGetErrorString() to do all the
 necessary work.  There is no explicit need to define the message strings and place the
 inserts.  The API provides all the routines necessary to do this work for you!
*/

static void CM_VARARGS error_Handler(CMErrorNbr errorNumber, ...)
{
  va_list inserts;
  char errorString[256];

  va_start(inserts, errorNumber);
  fprintf(stderr, "### %s\n", CMVGetErrorString(errorString, 256, errorNumber, inserts));
  va_end(inserts);

/*  exit(EXIT_FAILURE);  */                     /* D I E !                           */
}


/*---------------------------------*
 | alloc_Handler - allocate memory |
 *---------------------------------*

 The Container Manager API requires some form of memory management that can allocate memory
 and return pointers to it.  By generalizing this as a handler you are free to choose a
 memory management mechanism appropriate to your environment.

 For the purposes of this example, we map the handler directly onto the C runtime malloc().
 If you are running in a standard C runtime environment, this may prove sufficient.
*/

static void CM_PTR * CM_FIXEDARGS alloc_Handler(CMSize size)
{
  return (malloc((size_t)size));                    /* guess what this does              */
}


/*----------------------------------*
 | free_Handler - deallocate memory |
 *----------------------------------*

 The Container Manager API calls this handler when it wants to free up memory it no longer
 needs.  The memory attempting to be freed will, of course, be memory previously allocated
 by the memory allocator handler.

 There is nothing requiring you to free the memory.  If you want to chew it all up, be my
 guest!  But for this example, we'll be good guys and use the standard C free() since we
 allocated with malloc() above.
*/

static void CM_FIXEDARGS free_Handler(CMPtr ptr)
{
  free((void *)ptr);                                /* poof!                            */
}

                              CM_END_CFUNCTIONS
