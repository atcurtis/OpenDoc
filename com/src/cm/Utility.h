/* @(#)Z 1.5 com/src/cm/Utility.h, odstorage, od96os2, odos29646d 96/11/15 15:27:39 (96/10/29 09:20:16) */
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
	File:		Utility.h

	Contains:	Container Manager Miscellaneous Utility Routines Interfaces

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
 |                         <<<     Utility.h     >>>                         |
 |                                                                           |
 |        Container Manager Miscellaneous Utility Routines Interfaces        |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/02/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file containes a collection of generally usefull utility routines that don't
 logically fit anywhere else.
*/

#ifndef __UTILITYROUTINES__
#define __UTILITYROUTINES__


#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
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

																	CM_CFUNCTIONS


CM_CHAR *cmltostr(CM_LONG n, CM_SHORT width, CMBoolean hexConversion, CM_CHAR *s);
	/*
	Converts the (long) integer n to a string. IF width>0 and width > strlen(string), then
	the number will be right-justified in a string of width characters.  If width<0 and 
	abs(width) > length(string), then the number is again right-justified but padded with
	leading zeros instead of blanks.	In this case a negative number will have a minus as
	its first character.  If width is too small the entire number is returned overflowing
	the width.
	
	If hex conversion is desired, set the hexConversion parameter to true.
	
	The function returns the specified string parameter as its value. That string is assumed
	large enough to hold the number.
	
	Note, this routine is provided as a substitute for sprintf().  It is more compact,
	faster, and doesn't "drag" in a lot of I/O library routines with it.
	*/
	
	
														  CM_END_CFUNCTIONS
#endif
