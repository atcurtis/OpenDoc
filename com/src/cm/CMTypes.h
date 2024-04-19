/* @(#)Z 1.6 com/src/cm/CMTypes.h, odstorage, od96os2, odos29646d 96/11/15 15:26:55 (96/10/29 09:17:17) */
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
	File:		CMTypes.h

	Contains:	Container Manager Private "Types.h"

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                             <<< CMTypes.h >>>                             |
 |                                                                           |
 |                    Container Manager Private "Types.h"                    |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/18/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file is the "moral equivalent" to Apple's Types.h, stripped down to the basic
 definitions needed by the Container Manager.  All internal Container Manager code files
 include this header.  Because of that, those files do not include the other environmental
 and configuration headers because we do that here for them.  Thus this file is to be
 include for all basic definitions.
*/

#ifndef __CMTYPES__
#define __CMTYPES__

#ifndef __CM_API_ENV__
#include "CMAPIEnv.h"
#endif
#ifndef __CMCONFIG__
#include "CMConfig.h"
#endif


#ifndef __DEBUG__
#define __DEBUG__ 0
#endif


/*---------------------*
 | Mac "Types.h" Stuff |
 *---------------------*
 
 These are some basic types we use internally in the Container Manager:
*/

#ifndef __TYPES__															/* allows me to test with Macintosh MPW 	*/
#ifndef _BooleanType
typedef unsigned char Boolean;								/* Booleans...														*/
#endif

#ifndef _BooleanConst
enum {false,true};
#endif

#endif


/*-----------------------*
 | Debugging Definitions |
 *-----------------------*
 
 For Mac debugging, the low level Mac debugger, "MacsBug" is used.  It's definitions are
 defined under a __DEBUG__ compile-time switch. 
*/

#if __DEBUG__
#ifndef __TYPES__

															CM_CFUNCTIONS
															
pascal void Debugger(void) = 0xA9FF; 
pascal void DebugStr(const unsigned char * aStr) = 0xABFF; 

														CM_END_CFUNCTIONS

#endif /* __TYPES__ */
#endif /* __DEBUG__ */

#endif /* __CMTYPES__ */
