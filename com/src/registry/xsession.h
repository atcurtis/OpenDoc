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
/* @(#) 1.3 com/src/registry/xsession.h, odconfig, od96os2, odos29646d 10/31/96 12:45:31 [11/15/96 15:49:47] */
/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                      <<<        XSession.h        >>>                     |
 |                                                                           |
 |           Example Container Manager Special Handlers Interfaces           |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  2/5/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992                    |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file represents the handler header file for the example handler file 
 ExampleSessionHandlers.c.  That file contains complete documentation and an example on how
 to write a set of required session handlers and a metahandler for the Container Manager
 API.
 
 The only visible routine is the metahandler you need to pass to CMStartSession().  See
        XSession.c        for complete documentation.
*/


#ifndef __EXAMPLESESSIONHANDLERS__
#define __EXAMPLESESSIONHANDLERS__

#include "CMAPI.h" 

                                  CM_CFUNCTION
  
CMHandlerAddr CM_FIXEDARGS regSessionRoutinesMetahandler(CMType targetType, CMconst_CMGlobalName operationType);
  /*
  Metahandler proc for determining the addresses of the session handler operation routines.
  Pass the address of this routine to a CMStartSession() call.
  */

                                CM_END_CFUNCTIONS

#endif
