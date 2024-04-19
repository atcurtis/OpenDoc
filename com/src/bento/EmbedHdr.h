/* @(#)Z 1.4 com/src/bento/EmbedHdr.h, odstorage, od96os2, odos29646d 96/11/15 15:29:28 (96/10/29 09:12:24) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   ODEmbeddedHandlers
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
	File:		EmbedHdr.h

	Contains:	ODEmbbeddedHandlers header

	Written by:	Vincent Lo

	Copyright:	© 1993-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 8/16/95	EL		1242376: Buffer embedded handler so there
									is less fragmentation on top level
									container.
		 <5>	  5/2/95	EL		1245113: Turn off buffering because it
									currently cause problem in this bug.
		 <4>	 4/25/95	EL		1213321: turn on embedded container
									buffering.
		 <3>	  4/7/95	EL		1213321: New buffering scheme but leave it
									inactivated until after DR2.
		 <2>	 8/26/94	EL		#1182275 Allows writing embeded value in
									large block to decrease fragmentation.
		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

#ifndef _EMBEDHDR_
#define _EMBEDHDR_

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

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#define kLargeEmbeddedBlock 1

//==============================================================================
// Theory of Operation
//==============================================================================

//==============================================================================
// Constants
//==============================================================================
const	kBentoTypeNameSize = 512;
const	kEmbeddedBlockSize = 2048;
extern const ODType	kODEmbeddedContainerTypeName; // = "EmbedHdr";

//==============================================================================
// Scalar Types
//==============================================================================

//==============================================================================
// Classes defined in this interface
//==============================================================================
class ODEmbeddedHandlers;

//==============================================================================
// Classes used by this interface
//==============================================================================

//==============================================================================
// ODEmbeddedHandlers
//==============================================================================

class ODEmbeddedHandlers : public ODBentoHandlers
{
public:

	ODEmbeddedHandlers(CMSession session, CMValue parentValue);
	ODVMethod ~ODEmbeddedHandlers();
	
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

	CMSession	fSession;
	CMValue		fParentValue;
	CMSize		fPosition;
	CMSize		fSize;
#if kLargeEmbeddedBlock
	CMSize		fBufferBegin;
	ODSByte*	fBuffer;
#endif
	ODSByte	fTypeName[kBentoTypeNameSize];
	ODBoolean	fReverseEndian;
};

#endif // _EMBEDHDR_
