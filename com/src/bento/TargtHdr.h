/* @(#)Z 1.4 com/src/bento/TargtHdr.h, odstorage, od96os2, odos29646d 96/11/15 15:26:38 (96/10/29 09:14:03) */
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
	File:		TargtHdr.h

	Contains:	Header for Target Handler for Bento Container Suite

	Written by:	Vincent Lo

	Copyright:	© 1993-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                     <<< TargetContainerHandlers.c >>>                     |
 |                                                                           |
 |        Container Manager Basic Target Container Handlers Interfaces       |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 5/28/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992                    |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file contains an the interfaces for of basic container I/O handler's metahandler
 used by the Container Manager when doing updating operations on a target container.  The
 "I/O" defined by this methandler is in terms of Container Manager API calls on a dynamic
 OR real value.
 
 See TargetContainerHandlers.c for complete details.
 
 Note, this header and its corresponding code are NOT to be viewed as example handlers.
 They are not internal to the Container Manager code for portability reasons.  That is 
 because these handlers, like all other handlers may have to be modified as a function of
 a particular hardware installation (for example, TOC and label I/O handlers for machines
 with different byte sizes).
*/

#ifndef _TARGTHDR_
#define _TARGTHDR_

#include "CMAPI.h"

																		CM_CFUNCTION
	
CMHandlerAddr CM_FIXEDARGS targetContainerMetahandler(CMType targetType, CMconst_CMGlobalName operationType);
	/*
	Metahandler proc for determining the addresses of the handler operation routines.  The
	API user is expected to register a predefined type, CMTargetHandlersTypeName (defined 
	in CM_API_StdObjIDs.h), with this methandler using CMSetMetaHandler().  The Container
	Manager uses this type to open the target container.  That, in turn, will use this
	metahandler due to the methandler/type association.  The methandler will thus return the
	addresses of the handler routines defined in TargetContainerHandlers.c. 
	*/

CMRefCon open_Handler(CMRefCon attributes, CMOpenMode mode);
void close_Handler(CMRefCon refCon);
CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode);
CMSize tell_Handler(CMRefCon refCon);
CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
CMSize containerSize_Handler(CMRefCon refCon);
void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
															 CMContainerFlags *flags, CM_USHORT *bufSize,
															 CM_USHORT *majorVersion, CM_USHORT *minorVersion,
															 CMSize *tocOffset, CMSize *tocSize);
CM_UCHAR *returnContainerName_Handler(CMRefCon refCon);
void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer,
 																 CMSize size, CMPrivateData data);
void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer, CMSize size, CMPrivateData data);

																	CM_END_CFUNCTION

#endif
