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
/* @(#) 1.3 com/src/registry/xembeded.h, odconfig, od96os2, odos29646d 10/31/96 12:45:17 [11/15/96 15:49:47] */
/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                     <<<        XEmbeded.h         >>>                     |
 |                                                                           |
 |      Example Container Manager Embedded Container Handlers Interfaces     |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  1/8/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992                    |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file represents the handler header file for the example embedded container handler
 file ExampleEmbeddedHandler.c.  That file contains complete documentation and an example
 on how to write a set of handlers and a metahandler for the Container Manager API that
 supports embedded containers.
 
 The example is a complete working set of handlers for embedded containers.  Such handlers
 do all "I/O" in terms of API calls for a value.  Thus this example is fully general to
 support all forms of embedded containers!
 
 To make this a working example, and to hide its details from the "outside world", this
 header is provided.  As documented in        XEmbeded.c         there are a few
 additional routines that have nothing to do with the API handlers per-se.  Rather they
 allow for manipulation of some of the hidden information.
 
 See        XEmbeded.c         for complete documentation on these routines.
*/

#ifndef __EXAMPLEEMBEDDEDHANDLERS__
#define __EXAMPLEEMBEDDEDHANDLERS__

#include "CMAPI.h" 

                                CM_CFUNCTIONS

/*------------------------------*
 | Routines REQUIRED by the API |
 *------------------------------*/
  
CMHandlerAddr CM_FIXEDARGS embeddedContainerMetahandler(CMType targetType, CMconst_CMGlobalName operationType);
  /*
  Metahandler proc for determining the addresses of the handler operation routines.  Pass
  the address of this routine to a CMSetMetaHandler() call.
  */


/*------------------------------------------------------------------*
 | Auxiliary routines to make the example available for general use |
 *------------------------------------------------------------------*/

CMRefCon CM_FIXEDARGS createRefConForMyEmbHandlers(const CMValue parentValue);
  /*
  Create a reference constant (a "refCon") for container handler use.  Passed as the
  "attributes" to CMOpen[New]Container() and used as the "refCon" for all handler calls.
  */
  
  
CMRefCon CM_FIXEDARGS setEmbeddedHandlersTrace(CMRefCon refCon, const char tracingState,
                                                FILE CM_PTR *tracingFile);
  /*
  Enable (tracingState != 0) or disable (tracingState == 0) handler tracing to the
  tracingFile.  This routine returns the refCon as its result.
  */

                              CM_END_CFUNCTIONS

#endif
