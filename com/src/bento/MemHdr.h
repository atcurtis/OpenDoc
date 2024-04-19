/* @(#)Z 1.6 com/src/bento/MemHdr.h, odstorage, od96os2, odos29646d 96/11/15 15:29:29 (96/10/29 09:13:09) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   ODMemBentoHandlers
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
	File:		MemHdr.h

	Contains:	ODMemContainer header

	Written by:	Vincent Lo

	Copyright:	© 1993-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <1>	 5/27/94	VL		first checked in
	To Do:
*/

#ifndef _MEMHDR_
#define _MEMHDR_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _BENTOHDR_
#include "BentoHdr.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

// removed ifdef _PLATFORM_NT_ 22470
typedef char* Zone;
typedef Zone* THz;

//==============================================================================
// Theory of Operation
//==============================================================================

//==============================================================================
// Constants
//==============================================================================
extern const ODType kODBentoMemoryTypeName; // = "MemoryCtr";

//==============================================================================
// Scalar Types
//==============================================================================

//==============================================================================
// Classes defined in this interface
//==============================================================================
class ODMemBentoHandlers;

//==============================================================================
// Classes used by this interface
//==============================================================================

//==============================================================================
// ODMemBentoHandlers
//==============================================================================

class ODMemBentoHandlers : public ODBentoHandlers
{
public:

	ODMemBentoHandlers(CMSession session, ODHandle data);
	ODVMethod ~ODMemBentoHandlers();
	
	ODMethod void Initialize();
	ODMethod CMSession GetCMSession();
	
	ODMethod CMRefCon OpenHandler(CMOpenMode mode);
	ODMethod void CloseHandler();
	ODMethod CMSize FlushHandler();
	ODMethod CMSize SeekHandler(CM_LONG posOff, CMSeekMode mode);
	ODMethod CMSize TellHandler();
	ODMethod CMSize ReadHandler(CMPtr buffer, CMSize elementSize, CMCount theCount);
	ODMethod CMSize WriteHandler(CMPtr buffer, CMSize elementSize, CMCount theCount);
	ODMethod CMEofStatus EOFHandler();
	ODMethod CMBoolean TruncHandler(CMSize containerSize);
	ODMethod CMSize ContainerSizeHandler();
	ODMethod void ReadLabelHandler(CMMagicBytes magicByteSequence,
						CMContainerFlags *flags, CM_USHORT *bufSize,
						CM_USHORT *majorVersion, CM_USHORT *minorVersion,
						CMSize *tocOffset, CMSize *tocSize);
	ODMethod void WriteLabelHandler(CMMagicBytes magicByteSequence,
						CMContainerFlags flags, CM_USHORT bufSize,
						CM_USHORT majorVersion, CM_USHORT minorVersion,
						CMSize tocOffset, CMSize tocSize);
	ODMethod CMValue ReturnParentValueHandler();
	ODMethod CM_UCHAR* ReturnContainerNameHandler();
	ODMethod CMType ReturnTargetTypeHandler(CMContainer container);
	ODMethod void ExtractDataHandler(CMDataBuffer buffer,
							CMSize size, CMPrivateData data);
	ODMethod void FormatDataHandler(CMDataBuffer buffer,
							CMSize size,
							CMPrivateData data);

private:

				void SaveZone();
				void RestoreZone();

	CMSession	fSession;
	ODHandle   fData;
	ODBoolean  fAllocated;
	CMSize      fSize;
	ODBoolean	fSeekValid;
	ODULong    fSeekPos;
	ODSLong	fLastSeekOffset;
	ODSShort	fLastSeekMode;
	ODBoolean  fEOFReached;
	THz			fZone;
	THz			fCurZone;
};

#endif // _MEMHDR_
