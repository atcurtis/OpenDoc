/* @(#)Z 1.4 com/src/bento/BentoHdr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:07 (96/10/29 09:10:42) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
//
//   CLASSES: none
//
//   ORIGINS: 82,27
//
//
//   (C) COPYRIGHT International Business Machines Corp. 1995,1996
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//   	
//   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
//   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
//   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
//   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
//   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
//   OR PERFORMANCE OF THIS SOFTWARE.
//
//====END_GENERATED_PROLOG========================================
//

/*
	File:		BentoHdr.cpp

	Contains:	Bento Handlers

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 5/26/95	VL		1251403: Multithreading naming support.
		 <1>	 5/27/94	VL		first checked in

	To Do:
*/

#ifndef _BENTOHDR_
#include "BentoHdr.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef __CM_API_STDOBJIDS_
#include "CMAPIIDs.h"
#endif

#ifndef __STRING__
#include "string.h"				// for strcmp
#endif

#pragma segment BentoHdr

//==============================================================================
// Function Prototype
//==============================================================================

 	CM_CFUNCTIONS

CMHandlerAddr CM_FIXEDARGS containerMetahandler(CMType targetType, CMconst_CMGlobalName operationType);
																
static CMRefCon open_Handler(CMRefCon attributes, CMOpenMode mode);
static void close_Handler(CMRefCon refCon);
static CMSize flush_Handler(CMRefCon refCon);
static CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode);
static CMSize tell_Handler(CMRefCon refCon);
static CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
static CMSize write_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount);
static CMEofStatus eof_Handler(CMRefCon refCon);
static CMBoolean trunc_Handler(CMRefCon refCon, CMSize containerSize);
static CMSize containerSize_Handler(CMRefCon refCon);
static void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
							CMContainerFlags *flags, CM_USHORT *bufSize,
							CM_USHORT *majorVersion, CM_USHORT *minorVersion,
							CMSize *tocOffset, CMSize *tocSize);
static void writeLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
							CMContainerFlags flags, CM_USHORT bufSize,
							CM_USHORT majorVersion, CM_USHORT minorVersion,
							CMSize tocOffset, CMSize tocSize);	
static CMValue returnParentValue_Handler(CMRefCon refCon);
static CM_UCHAR *returnContainerName_Handler(CMRefCon refCon);
static CMType returnTargetType_Handler(CMRefCon refCon, CMContainer container);	
static void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer,
								 CMSize size, CMPrivateData data);
static void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer,
								CMSize size, CMPrivateData data);
																 
	CM_END_CFUNCTIONS

//------------------------------------------------------------------------------
// containerMetahandler
//------------------------------------------------------------------------------
CMHandlerAddr CM_FIXEDARGS containerMetahandler(CMType targetType, CMconst_CMGlobalName operationType)
{
	static char *operationTypes[] = {CMOpenOpType,						/*  0 */
									 CMCloseOpType,						/*  1 */
									 CMFlushOpType,						/*  2 */
									 CMSeekOpType,						/*  3 */
									 CMTellOpType,						/*  4 */
									 CMReadOpType,						/*  5 */
									 CMWriteOpType,						/*  6 */
									 CMEofOpType,						/*  7 */
									 CMTruncOpType,						/*  8 */
									 CMSizeOpType,						/*  9 */
									 CMReadLblOpType,					/* 10 */
									 CMWriteLblOpType,					/* 11 */
									 CMParentOpType,					/* 12 */
									 CMContainerOpName,					/* 13 */
									 CMTargetTypeOpType,				/* 14 */
									 CMExtractDataOpType,				/* 15 */
									 CMFormatDataOpType,				/* 16 */
									 NULL};
	char 	 **t;
	CMType ignored = targetType;
		
	t = operationTypes - 1;
	while (*++t) if (strcmp((char *)operationType, *t) == 0) break;
	
	switch (t - operationTypes) {
		case  0:	return ((CMHandlerAddr)open_Handler);				/*CMOpenOpType 			*/
		case  1:	return ((CMHandlerAddr)close_Handler);				/*CMCloseOpType 		*/
		case  2:	return ((CMHandlerAddr)flush_Handler);				/*CMFlushOpType 		*/
		case  3:	return ((CMHandlerAddr)seek_Handler);				/*CMSeekOpType 			*/
		case  4:	return ((CMHandlerAddr)tell_Handler);				/*CMTellOpType 			*/
		case  5:	return ((CMHandlerAddr)read_Handler);				/*CMReadOpType 			*/
		case  6:	return ((CMHandlerAddr)write_Handler);				/*CMWriteOpType 		*/
		case  7:	return ((CMHandlerAddr)eof_Handler);				/*CMEofOpType 			*/
		case  8:	return ((CMHandlerAddr)trunc_Handler);				/*CMTruncOpType			*/
		case  9:	return ((CMHandlerAddr)containerSize_Handler);		/*CMSizeOpType 			*/
		case 10:	return ((CMHandlerAddr)readLabel_Handler);			/*CMReadLblOpType 		*/
		case 11:	return ((CMHandlerAddr)writeLabel_Handler);			/*CMWriteLblOpType 		*/
		case 12:	return ((CMHandlerAddr)returnParentValue_Handler);	/*CMParentOpType		*/
		case 13:	return ((CMHandlerAddr)returnContainerName_Handler);/*CMContainerOpName		*/
		case 14:	return ((CMHandlerAddr)returnTargetType_Handler);	/*CMTargetTypeOpType	*/
		case 15:	return ((CMHandlerAddr)extractData_Handler);		/*CMExtractDataOpType	*/
		case 16:	return ((CMHandlerAddr)formatData_Handler); 		/*CMFormatDataOpType	*/
		
		default:	return (NULL);
	}
}

//------------------------------------------------------------------------------
// open_Handler
//------------------------------------------------------------------------------
static CMRefCon open_Handler(CMRefCon refCon, CMOpenMode mode)
{
	return ((ODBentoHandlers*) refCon)->OpenHandler(mode);
}

//------------------------------------------------------------------------------
// close_Handler
//------------------------------------------------------------------------------
static void close_Handler(CMRefCon refCon)
{
	((ODBentoHandlers*) refCon)->CloseHandler();
}

//------------------------------------------------------------------------------
// flush_Handler
//------------------------------------------------------------------------------
static CMSize flush_Handler(CMRefCon refCon)
{
	return ((ODBentoHandlers*) refCon)->FlushHandler();
}

//------------------------------------------------------------------------------
// seek_Handler
//------------------------------------------------------------------------------
static CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode)
{
	return ((ODBentoHandlers*) refCon)->SeekHandler(posOff, mode);

}

//------------------------------------------------------------------------------
// tell_Handler
//------------------------------------------------------------------------------
static CMSize tell_Handler(CMRefCon refCon)
{
	return 	((ODBentoHandlers*) refCon)->TellHandler();
}

//------------------------------------------------------------------------------
// read_Handler
//------------------------------------------------------------------------------
static CMSize read_Handler(CMRefCon refCon,
							CMPtr buffer,
							CMSize elementSize,
							CMCount theCount)
{
	return ((ODBentoHandlers*) refCon)->ReadHandler(buffer, elementSize, theCount);
}

//------------------------------------------------------------------------------
// write_Handler
//------------------------------------------------------------------------------
static CMSize write_Handler(CMRefCon refCon,
							CMPtr buffer,
							CMSize elementSize,
							CMCount theCount)
{
	return 	((ODBentoHandlers*) refCon)->WriteHandler(buffer, elementSize, theCount);
}

//------------------------------------------------------------------------------
// eof_Handler
//------------------------------------------------------------------------------
static CMEofStatus eof_Handler(CMRefCon refCon)
{
	return 	((ODBentoHandlers*) refCon)->EOFHandler();

}

//------------------------------------------------------------------------------
// trunc_Handler
//------------------------------------------------------------------------------
static CMBoolean trunc_Handler(CMRefCon refCon, CMSize containerSize)
{
	return ((ODBentoHandlers*) refCon)->TruncHandler(containerSize);
}

//------------------------------------------------------------------------------
// containerSize_Handler
//------------------------------------------------------------------------------
static CMSize containerSize_Handler(CMRefCon refCon)
{
	return ((ODBentoHandlers*) refCon)->ContainerSizeHandler();

}

//------------------------------------------------------------------------------
// readLabel_Handler
//------------------------------------------------------------------------------
static void readLabel_Handler(CMRefCon refCon,
								CMMagicBytes magicByteSequence,
								CMContainerFlags *flags,
								CM_USHORT *bufSize,
								CM_USHORT *majorVersion,
								CM_USHORT *minorVersion,
								CMSize *tocOffset,
								CMSize *tocSize)
{
	((ODBentoHandlers*) refCon)->ReadLabelHandler(magicByteSequence, flags, bufSize,
											majorVersion, minorVersion,
											tocOffset, tocSize);
}

//------------------------------------------------------------------------------
// writeLabel_Handler
//------------------------------------------------------------------------------
static void writeLabel_Handler(CMRefCon refCon,
								CMMagicBytes magicByteSequence,
								CMContainerFlags flags,
								CM_USHORT bufSize,
								CM_USHORT majorVersion,
								CM_USHORT minorVersion,
								CMSize tocOffset,
								CMSize tocSize)
{
	((ODBentoHandlers*) refCon)->WriteLabelHandler(magicByteSequence,
											flags, bufSize,
											majorVersion, minorVersion,
											tocOffset, tocSize);
}

//------------------------------------------------------------------------------
// returnParentValue_Handler
//------------------------------------------------------------------------------
static CMValue returnParentValue_Handler(CMRefCon refCon)
{
	return ((ODBentoHandlers*) refCon)->ReturnParentValueHandler();
}

//------------------------------------------------------------------------------
// returnContainerName_Handler
//------------------------------------------------------------------------------
static CM_UCHAR *returnContainerName_Handler(CMRefCon refCon)
{
	return ((ODBentoHandlers*) refCon)->ReturnContainerNameHandler();
}

//------------------------------------------------------------------------------
// returnTargetType_Handler
//------------------------------------------------------------------------------
static CMType returnTargetType_Handler(CMRefCon refCon, CMContainer container)
{
	return ((ODBentoHandlers*) refCon)->ReturnTargetTypeHandler(container);
}

//------------------------------------------------------------------------------
// extractData_Handler
//------------------------------------------------------------------------------
static void extractData_Handler(CMRefCon refCon,
								CMDataBuffer buffer,
								CMSize size,
								CMPrivateData data)
{
	((ODBentoHandlers*) refCon)->ExtractDataHandler(buffer, size, data);
}

//------------------------------------------------------------------------------
// formatData_Handler
//------------------------------------------------------------------------------
static void formatData_Handler(CMRefCon refCon,
								CMDataBuffer buffer,
								CMSize size,
								CMPrivateData data)
{
	((ODBentoHandlers*) refCon)->FormatDataHandler(buffer, size, data);
}

//------------------------------------------------------------------------------
// ODBentoHandlers::ODBentoHandlers
//------------------------------------------------------------------------------
ODBentoHandlers::ODBentoHandlers()
{
}

//------------------------------------------------------------------------------
// ODBentoHandlers::~ODBentoHandlers
//------------------------------------------------------------------------------
ODBentoHandlers::~ODBentoHandlers()
{
}

