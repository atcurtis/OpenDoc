/* @(#)Z 1.4 com/src/cm/CMAPIDbg.h, odstorage, od96os2, odos29646d 96/11/15 15:26:41 (96/10/29 09:15:55) */
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
	File:		CMAPIDbg.h

	Contains:	Container Manager Global Name Values Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<< CM_API_Debug.h >>>                          |
 |                                                                           |
 |              Container Manager Global Name Values Interfaces              |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  3/30/92                                  |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1994               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file defines a set of special debugging API routines and switches.  They are kept
 separate from the API since they may or may not be supported in a particular installation.
 
 There are three routines:
 
 	1. CMDebugging()					- to set some internal debugging "options"
	2. CMDumpTOCStructures()	- to dump in-memory TOC as a tree-like format
	3. CMDumpContainerTOC()		- to read in container TOC and display it in a table format
*/
 
#ifndef __CM_API_DEBUG__
#define __CM_API_DEBUG__

#include <stdio.h>

#ifndef __CM_API__
#include "CMAPI.h"
#endif


/*                        C A U T I O N   A N D   N O T E!															*/
/*                        --------------------------------															*/

/* In order to be able to use these routines the Container Manager API code must have		*/
/* the routines defined!  They may be suppressed as a function of the installation.		  */
/* Thus this file should NOT be supplied if the Container Manager is built with these 	*/
/* suppressed.  Even if it is, it is left up to the builder is s/he wants these routines*/
/* publically known.																																		*/


																		 CM_CFUNCTIONS

void CM_FIXEDARGS CMDebugging(CMSession sessionData, CM_USHORT tocTableSize, 
															FILE CM_PTR *debuggingFile, CMBoolean doValidations);
	/*
	This routine provides some debugging "options" to the API.  It must be called AFTER 
 	CMStartSession() but BEFORE any containers are opened.  It takes the session global data
  pointer returned by CMStartSession() and adds (actually modifies) some of the data there
  to enable debugging.
	
	The current options that can be set are:
	
	1. The size of the TOC index tables.  If 0 is passed, the default is used.  The 
		 current in-memory TOC implementation scheme utilizes index tables.  Each level of
		 table represents one digit of an object descriptor ID.  For ID=123 there are three
		 tables.  A hundereds table, a tens table, and finally the unit (lowest level)
		 table.  The i'th entry of a table points to the next lower table.  The lowest
		 table of course points to the object itself in memory.  The tocTableSize determines
		 the size of these tables.  The current defualt is 256.
	
	2. A FILE* debugging output variable. It is usually stdout, stderr, or NULL. For NULL,
		 no debugging output is produced.  Currently this thing is used to display the TOC
		 as a table when it is open for input (if CMDUMPTOC was set).
 			 
	3. The dynamic switch controlling the refNum validations (e.g., checking for NULL) and
		 various other protections.  If set to 0 the protection checks are suppressed.  If
		 nonzero they are performed, but only if the protection code is present.  On a per-
		 installation basis it may be suppressed.  In that case the switch has no effect.
*/


void CM_FIXEDARGS CMDumpTOCStructures(CMContainer container, FILE CM_PTR *f);
	/*
	This routine is used for internal debugging of the TOC data structures for the specified
	container.  It displays their current state to the (open) file associated with the
	specified file variable, f.
	*/


void CM_FIXEDARGS CMDumpContainerTOC(CMconst_CMContainer container, FILE CM_PTR *f,
													  	 			 const CM_ULONG tocOffset, const CM_ULONG tocSize);
	/*
	This routine is used for internal debugging of the TOC data as it appears in its final
	form in the container itself.  The entire TOC in the container, starting at container
	offset tocOffset for tocSize bytes, is displayed to the (open) file associated with the
	file variable f.  It is formatted, but otherwise  unchanged.  You can use this routine to
	see if the TOC was correctly written to its container.
	
	Note, the container's handler routines are used to do the positioning and reading.  It
	is assumed that the passed tocOffset and tocSize are valid (from a container label).
	*/

																	CM_END_CFUNCTIONS
#endif
