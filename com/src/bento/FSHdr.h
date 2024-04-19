/* @(#)Z 1.4 com/src/bento/FSHdr.h, odstorage, od96os2, odos29646d 96/11/15 15:29:28 (96/10/29 09:12:36) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   ODFSBentoHandlers
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
	File:		FSHdr.h

	Contains:	ODFSContainer header

	Written by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	  4/7/95	EL		1225905: More ModDate stuff to container.
		 <5>	 3/24/95	EL		1225905: Move state information such as mod
									date from plfmFile to here
		 <4>	 3/10/95	EL		1223465: allocate buffer on the heap.
		 <3>	 2/10/95	VL		1205627: Added fMode to store access mode.
		 <2>	 1/31/95	EL		1213321: Buffer the Bento file I/O.
		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

#ifndef _FSHDR_
#define _FSHDR_

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

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

//==============================================================================
// Theory of Operation
//==============================================================================

//==============================================================================
// Constants
//==============================================================================

extern const ODType	kODBentoFileTypeName; // = "FileCtr";

#define MininumChunkSize 		0x100	/* minimum size of file space allocation	*/
#define DefaultChunkSize 		0x800	/* default size of file space allocation	*/

//==============================================================================
// Scalar Types
//==============================================================================

struct ContainerLabelFmt {			/* Layout of a container label:			*/
 ODUByte magicBytes[8];				/* 8 bytes: the magic byte identifier	*/
 ODUShort flags;					/* 2	the label flag					*/
 ODUShort bufSize;					/* 2	TOC buffer size / 1024			*/
 ODUShort majorVersion;				/* 2	major format version number		*/
 ODUShort minorVersion;				/* 2	minor format version number		*/
 ODULong tocOffset;					/* 4	offset to start of TOC			*/
 ODULong tocSize;					/* 4	total byte size of the TOC		*/
};
typedef struct ContainerLabelFmt ContainerLabelFmt;

//==============================================================================
// Classes defined in this interface
//==============================================================================
class ODFSBentoHandlers;

//==============================================================================
// Classes used by this interface
//==============================================================================
class PlatformFile;

//==============================================================================
// ODFSBentoHandlers
//==============================================================================

class ODFSBentoHandlers : public ODBentoHandlers
{
public:

	ODFSBentoHandlers(CMSession session, PlatformFile* file);
	ODVMethod ~ODFSBentoHandlers();
	
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

	ODMethod	void ReloadBuffer();
	ODMethod	void FlushBuffer();

	CMSession	fSession;
	PlatformFile*	fFile;
	ODBoolean	fReverseEndian;
	ODULong		fLogicalPos;
	ODULong		fFileSize;
	ODULong		fPhysicalFileSize;
	ODULong		fWriteLimit;
	ODULong		fChunkSize;
	ODBoolean	fBufferDirty;
	ODBoolean	fOpenReadOnly;
	ODBoolean	fHasLabel;
	ODULong		fBufferBegin;
	ODSByte*	fBuffer;
	ContainerLabelFmt fLabel;
};

#endif // _FSHDR_
