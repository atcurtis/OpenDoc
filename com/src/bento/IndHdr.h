/* @(#)Z 1.4 com/src/bento/IndHdr.h, odstorage, od96os2, odos29646d 96/11/15 15:26:33 (96/10/29 09:12:53) */
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
	File:		IndHdr.h

	Contains:	Header for indirect values.

	Written by:	Vincent Lo

	Copyright:	© 1993-94 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

#ifndef _INDHDR_
#define _INDHDR_

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef __CM_API_ENVIRONMENT__
#include "CMAPIEnv.h"
#endif

//==============================================================================
// Constants
//==============================================================================

extern const CMGlobalName kODIndirectValueGlobalName ; // = "OpenDoc:IndirectValue";
extern const ODType	kODEmbeddedContainerProperty; // = "OpenDoc:EmbeddedContainer";
extern const ODType	kODEmbeddedContainerType; // = "OpenDoc:EmbeddedContainerType";
														//jpa: Can't define in .h file
//==============================================================================
// Function Prototype
//==============================================================================

	CM_CFUNCTIONS

CMHandlerAddr CM_FIXEDARGS IndirectDynamicValueMetahandler(CMType targetType,
										CMconst_CMGlobalName operationType);

	CM_END_CFUNCTIONS

#endif
