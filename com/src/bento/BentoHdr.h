/* @(#)Z 1.4 com/src/bento/BentoHdr.h, odstorage, od96os2, odos29646d 96/11/15 15:26:07 (96/10/29 09:10:48) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   ODBentoHandlers
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
	File:		ODBentoHandlers.h

	Contains:	Header for ODBentoHandlers

	Written by:	Vincent Lo

	Copyright:	© 1993-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

#ifndef _BENTOHDR_
#define _BENTOHDR_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

//==============================================================================
// Classes defined in this interface
//==============================================================================
class ODBentoHandlers;

//==============================================================================
// containerMetahandler
//==============================================================================

CM_CFUNCTIONS

CMHandlerAddr CM_FIXEDARGS containerMetahandler(CMType targetType,
												CMconst_CMGlobalName operationType);

CM_END_CFUNCTIONS

//==============================================================================
// ODFSContainer
//==============================================================================

class ODBentoHandlers
{
public:
	
	ODBentoHandlers();
	ODVMethod ~ODBentoHandlers();
	
	ODVMethod void Initialize()
		= 0;
		
	ODVMethod CMSession GetCMSession()
		= 0;
	
	ODVMethod CMRefCon OpenHandler(CMOpenMode mode)
		= 0;
		
	ODVMethod void CloseHandler()
		= 0;
		
	ODVMethod CMSize FlushHandler()
		= 0;
		
	ODVMethod CMSize SeekHandler(CM_LONG posOff, CMSeekMode mode)
		= 0;
		
	ODVMethod CMSize TellHandler()
		= 0;
		
	ODVMethod CMSize ReadHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
		= 0;
		
	ODVMethod CMSize WriteHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
		= 0;
		
	ODVMethod CMEofStatus EOFHandler()
		= 0;
		
	ODVMethod CMBoolean TruncHandler(CMSize containerSize)
		= 0;
		
	ODVMethod CMSize ContainerSizeHandler()
		= 0;
		
	ODVMethod void ReadLabelHandler(CMMagicBytes magicByteSequence,
						CMContainerFlags *flags, CM_USHORT *bufSize,
						CM_USHORT *majorVersion, CM_USHORT *minorVersion,
						CMSize *tocOffset, CMSize *tocSize)
		= 0;
		
	ODVMethod void WriteLabelHandler(CMMagicBytes magicByteSequence,
						CMContainerFlags flags, CM_USHORT bufSize,
						CM_USHORT majorVersion, CM_USHORT minorVersion,
						CMSize tocOffset, CMSize tocSize)
		= 0;
		
	ODVMethod CMValue ReturnParentValueHandler()
		= 0;
		
	ODVMethod CM_UCHAR* ReturnContainerNameHandler()
		= 0;
		
	ODVMethod CMType ReturnTargetTypeHandler(CMContainer container)
		= 0;
		
	ODVMethod void ExtractDataHandler(CMDataBuffer buffer,
							CMSize size, CMPrivateData data)
		= 0;
		
	ODVMethod void FormatDataHandler(CMDataBuffer buffer,
							CMSize size,
							CMPrivateData data)
		= 0;
};

#endif	// _BENTOHDR_
