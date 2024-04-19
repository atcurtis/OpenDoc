/* @(#)Z 1.4 com/src/cm/CMVHOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:56 (96/10/29 09:17:23) */
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
	File:		CMVHOps.c

	Contains:	Container Manager (Dynamic) Value Handler Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                        <<<      CMVHOps.c      >>>                        |
 |                                                                           |
 |            Container Manager (Dynamic) Value Handler Operations           |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  7/27/92                                  |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1994               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This routines in this file are used for dynamic values.  Currently one one special 
 routines is defined to aid a dynamic value's "new value" handler in getting its
 "constructor" (initialization) parameters from CMNewValue().  See CMScanDataPacket()
 for further details.
*/


#include <stddef.h>
#include <stdio.h>

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
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __ERRORRPT__
#include "ErrorRpt.h"      
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
																	

/*--------------------------------------------------------*
 | CMScanDataPacket - extract the data from a data packet |
 *--------------------------------------------------------*
 
 This routine is used by a dynamic value's "new value" handler to extract the fields of
 a data packet passed to it by the Container Manager.  The data packet represents all
 the CMNewValue() "..." parameters for the type also passed to the "new value hander".
 
 Only that portion of the CMNewValue() "..." parameters associated with the type are passed
 in the data packet.  The Container Manager determines the parameters by the placement
 of the type within its heirarchy (types may have base types) and the metadata.
 
 The Container Manager accesses the metadata through a "metadata" handler for the type to
 build the data packet.  CMScanDataPacket() inverts the the operation and allows the "new
 value" handler to extract the data back into distinct variables.  The "new value" handler
 can use its own "metadata" handler to pass to the CMScanDataPacket() routine to extract
 the data.  Each CMScanDataPacket() "..." parameter must be a pointer; extracted data read
 from the data packet are stored into the locations pointed to by the pointers.
 
 The function returns the number of data items extracted and assigned to the parameters.
 This could be 0 if metadata is passed as NULL, or if an error is reported and the error
 reporter returns.
  
 See DynamicValues.[hc] for a full description of the "metadata" handler and metadata
 format specifications.
*/

CMCount CM_VARARGS CMScanDataPacket(CMType type, CMMetaData metaData,
																		CMDataPacket dataPacket, ...)
{
	va_list 			dataInitParams;
	CM_ULONG			nbrAssigned;
	
	ExitIfBadType(type, 0);														/* validate type										*/
		
	va_start(dataInitParams, dataPacket);							/* extract data into "..." variables*/
	nbrAssigned = cmVScanDataPacketGuts((TOCObjectPtr)type, (CM_CHAR *)metaData,
																			(CM_UCHAR *)dataPacket, dataInitParams);
	va_end(dataInitParams);
	
	return ((CMCount)nbrAssigned);										/* return nbr of values assigned		*/
}


/*--------------------------------------------------------*
 | CMVScanDataPacket - extract the data from a data packet |
 *--------------------------------------------------------*
 
 This routine is the same as CMScanDataPacket() above, except that the CMNewValue()
 "..." parameters are given as a variable argument list as defined by the "stdarg"
 facility.
 
 This routine assumes the caller sets up and terminates the variable arg list using the
 "stdarg.h" calls as follows:
 
 			#include <stdarg.h>
			
 			callersRoutine(args, ...)
			{
				va_list dataInitParams;
				
				- - -
				
				va_start(dataInitParams, args);
				n = CMScanDataPacket(type, metaData, dataPacket, dataInitParams);
				va_end(dataInitParams);
				
				- - -
			}
			
 See CMScanDataPacket() for further details.
*/

CMCount CM_FIXEDARGS CMVScanDataPacket(CMType type, CMMetaData metaData,
																			 CMDataPacket dataPacket,
																 			 va_list dataInitParams)
{
	CM_ULONG	nbrAssigned;
	
	ExitIfBadType(type, 0);														/* validate type										*/
		
	nbrAssigned = cmVScanDataPacketGuts((TOCObjectPtr)type, (CM_CHAR *)metaData,
																			(CM_UCHAR *)dataPacket, dataInitParams);
	
	return ((CMCount)nbrAssigned);										/* return nbr of values assigned		*/
}
														  
															CM_END_CFUNCTIONS
