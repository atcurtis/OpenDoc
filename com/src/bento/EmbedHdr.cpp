/* @(#)Z 1.6 com/src/bento/EmbedHdr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:31 (96/10/29 09:12:18) */
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
	File:		EmbedHdr.cpp

	Contains:	Class definition for ODEmbeddedHandlers class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <8>	10/24/95	jpa		1293441: Bento memory reserve & fatal
									container err.
		 <7>	 5/26/95	VL		1251403: Multithreading naming support.
		 <6>	 5/11/95	EL		1245113: fix bug of writing out garbage
									with buffering.
		 <5>	  4/7/95	EL		1213321: Use a different buffering scheme.
		 <4>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <3>	 8/26/94	EL		#1182275 Allows writing embeded value in
									large block to decrease fragmentation.
									#1182308 Allows non-byte swapping
									format/extract
		 <2>	 6/18/94	MB		Correct memory includes
		 <1>	 5/27/94	VL		first checked in

	To Do:
		1) Move ContainerLabelFmt to some common location.
		2) Use Exceptions instead of printf.
		3) Share ODFlipMove among handlers.
	In Progress:
		
*/

#ifndef	_ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _BENTOHDR_
#include "BentoHdr.h"
#endif

#ifndef _EMBEDHDR_
#include "EmbedHdr.h"
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#ifndef _INDHDR_
#include "IndHdr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _FLIPEND_
#include "FlipEnd.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ERRORDEF_
#include "ErrorDef.xh"
#endif

#ifndef __STRING__
#include "string.h"		// For strcpy and strcmp
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

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


#pragma segment EmbedHdr

static void ODFlipMove(ODPtr from, ODPtr to, ODULong size)
{
	char	*dest = (char *)to + size - 1;
	char	*src = (char *)from;
	
	while (size--)
		*dest-- = *src++;			
}

//==============================================================================
// ODEmbeddedHandlers
//==============================================================================

ODEmbeddedHandlers::ODEmbeddedHandlers(CMSession session, CMValue parentValue)
{
	fParentValue = parentValue;
	fSession = session;
}

ODEmbeddedHandlers::~ODEmbeddedHandlers()
{
}

void ODEmbeddedHandlers::Initialize()
{
	ODSByte	typeName[kBentoTypeNameSize];
	
	CMContainer	container   = CMGetValueContainer(fParentValue);
	CMSession	sessionData = CMGetSession(container);
	
	if (sessionData != fSession)
		THROW(kODErrBentoErr);

	ODSessionMustHaveCMAllocReserve(container);
	
	CMSetMetaHandler(fSession, kODEmbeddedContainerTypeName, (CMMetaHandler) containerMetahandler);
	
	CMGetContainerInfo(container, kODNULL, kODNULL, kODNULL, typeName, kODNULL);
	
	fReverseEndian = 0;						/* assume same endian-ness			*/
	fPosition	= 0;						/* container not "open" yet			*/
	fSize	= 0;							/* don't know size yet				*/
#if kLargeEmbeddedBlock
	fBufferBegin = 0;
	fBuffer = kODNULL;
#endif
	strcpy(fTypeName, typeName);			/* copy in the container typeName	*/

	ODSessionRestoreCMAllocReserve(container);
}

CMSession ODEmbeddedHandlers::GetCMSession()
{
	return fSession;
}

CMRefCon ODEmbeddedHandlers::OpenHandler(CMOpenMode mode)
{
	/* Get the size of the value.  For writing it better be 0 and for reading non-zero...	*/
	
	fSize = (ODULong) CMGetValueSize(fParentValue);
#if kLargeEmbeddedBlock
	fBufferBegin = fSize;
	fBuffer = (ODSByte *)ODNewPtr(kEmbeddedBlockSize);
#endif	
	/* Check the open mode and do appropriate checks on size. Set appropriate position...	*/
	
	if (strcmp((ODSByte*) mode, "wb+") == 0) {				/* writing...	*/
		if (fSize != 0) {									/* size must be zero	*/
			CMError(fSession, "Cannot create embedded container (type \"^0\") for a value that already has data!", fTypeName);
			return kODNULL;
		}
		fPosition = 0;									/* position to 1st free byte	*/
	} else if (strcmp((ODSByte*) mode, "rb") == 0) { 		/* reading...					*/
		if (fSize == 0) {									/* size must be non-zero		*/
			CMError(fSession, "Cannot read embedded container (type \"^0\") for a value that doesn't have data!", fTypeName);
			return kODNULL;
		}
		fPosition = 0;									/* position to 1st byte to read			*/
	} else if (strcmp((char *)mode, "rb+") == 0)  			/* converting or updating...			*/
		fPosition = fSize;								/* size can be anything(position at end)*/
	else {																				/* bad mode...													*/
		CMError(fSession, "Invalid embedded container (type \"^0\") open mode (\"^1\")!", fTypeName, (char *)mode);
			return kODNULL;
	}
	
	return ((CMRefCon) this);
}

void ODEmbeddedHandlers::CloseHandler()
{
#if kLargeEmbeddedBlock
	if (fBuffer != kODNULL) {
		(void) this->FlushHandler();
		ODDisposePtr(fBuffer);
		fBuffer = kODNULL;
	}
#endif
}

CMSize ODEmbeddedHandlers::FlushHandler()
{
	Environment* ev = somGetGlobalEnvironment();
#if kLargeEmbeddedBlock
	/* write out the data in the buffer */
	if (fBufferBegin != fSize) {
		CMWriteValueData(fParentValue, fBuffer, fBufferBegin, (CMSize) (fSize - fBufferBegin));
		fBufferBegin = fSize;
	}
#endif
	return ((CMSize) 0);
}

CMSize ODEmbeddedHandlers::SeekHandler(CM_LONG posOff, CMSeekMode mode)
{
	if (mode == kCMSeekSet)
		fPosition = (ODULong) posOff;
	else if (mode == kCMSeekEnd)
		fPosition = (ODSLong) fSize + posOff;
	else
		fPosition = (ODSLong) fPosition + posOff;
			
	return 0;
}

CMSize ODEmbeddedHandlers::TellHandler()
{	
	return fPosition;
}

CMSize ODEmbeddedHandlers::ReadHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
{
        Environment* ev = somGetGlobalEnvironment();
	ODULong	tryToRead = (ODULong) elementSize * (ODULong) theCount;

#if kLargeEmbeddedBlock
	ODULong	amountRead = 0;
	
	if (fPosition < fBufferBegin) {
		/* at least some of it on disk */
		if (tryToRead > (fBufferBegin - fPosition)) {
			/* first read from disk, later read from buffer */
			amountRead = (ODULong) CMReadValueData(fParentValue, buffer, fPosition, 
											   	   (CMSize) (fBufferBegin - fPosition));
			if (amountRead != (fBufferBegin - fPosition)) {
				/* trouble, don't continue */
				fPosition += amountRead;			/* update position by amount read				*/
				
				tryToRead = 0;		
			}
			else {
				fPosition += amountRead;			/* update position by amount read				*/
				tryToRead -= amountRead;
				buffer = (CMPtr)((char *)buffer + amountRead);
			}
		}
		else { /* it should all be from disk */
			amountRead = (ODULong) CMReadValueData(fParentValue, buffer, fPosition, 
											   	   (CMSize) tryToRead);
			fPosition += amountRead;			/* update position by amount read				*/
			
			return ((CMSize) amountRead);		
		}
	}

	if ((tryToRead) && (fPosition >= fBufferBegin)) {				/* it may be in the buffer				*/
		if (tryToRead > (fSize - fPosition))
			tryToRead = fSize - fPosition;
		ODBlockMove(fBuffer+fPosition-fBufferBegin, buffer, tryToRead);
		amountRead += tryToRead;
		fPosition += tryToRead;
	}
	
	return ((CMSize) amountRead);		
	
#else
	tryToRead = (ODULong) CMReadValueData(fParentValue, 	
											buffer,
											fPosition, 
											(CMSize) tryToRead);
	
	fPosition += tryToRead;						/* update position by amount read				*/
	
	return ((CMSize) tryToRead);		
#endif
}

CMSize ODEmbeddedHandlers::WriteHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	ODULong tryWriteAmount = (ODULong) elementSize * (ODULong) theCount;
#if kLargeEmbeddedBlock
	ODULong amountWritten = 0;
	
	/* if there is any data in the portion that is already on disk, write those out first */
	if (fPosition < fBufferBegin) {
		if ((fPosition + tryWriteAmount) > fBufferBegin) { /* it goes over to the buffer */
			amountWritten = fBufferBegin - fPosition;
			tryWriteAmount -= amountWritten;
		}
		else {
			amountWritten = tryWriteAmount;
			tryWriteAmount = 0;
		}
		CMWriteValueData(fParentValue, buffer, fPosition, amountWritten);
		fPosition += amountWritten;
		buffer = (CMPtr)((char *)buffer + amountWritten);
	}
	
	/* if there is any that should go into the buffer or extend to the buffer, move them there */
	
	while (tryWriteAmount > 0) {
		if ((fPosition + tryWriteAmount) <= (fBufferBegin + kEmbeddedBlockSize)) {
			/* we can put everything into the buffer										*/
			ODBlockMove(buffer, fBuffer + fPosition - fBufferBegin, tryWriteAmount);
			amountWritten += tryWriteAmount;
			fPosition += tryWriteAmount;
			if (fPosition > fSize)
				fSize = fPosition;
			tryWriteAmount = 0;		/* so we are done */			
		}
		else {
			/* we go beyond the end of the buffer, so we cannot put there directly */
			if (fBufferBegin < fSize) {
				/* if there are data in the buffer, flush it first, maybe there is room afterwards */
				(void) this->FlushHandler();
			}
			else {
				/* there is no data in buffer, yet it does not fit, so just write it out directly */
				CMWriteValueData(fParentValue, buffer, fPosition, (CMSize) tryWriteAmount);
				amountWritten += tryWriteAmount;
				fPosition += tryWriteAmount;
				fSize = fPosition;
				fBufferBegin = fSize;
				tryWriteAmount = 0;		/* so we are done */			
			}
		}
	}
	return amountWritten;
#else
	CMWriteValueData(fParentValue, buffer, fPosition, (CMSize) tryWriteAmount);
	fPosition += tryWriteAmount;					/* update position by amount written	*/

	if (fPosition > fSize)						/* if writing past end of value...		*/
		fSize = fPosition;						/* ...set new value size				*/
	
	return tryWriteAmount;
#endif
	
}

CMEofStatus ODEmbeddedHandlers::EOFHandler()
{
	return ((CMEofStatus) kODFalse);
}

CMBoolean ODEmbeddedHandlers::TruncHandler(CMSize containerSize)
{
        ODBoolean trunchandler = kODFalse;
	if ((ODULong)containerSize <= fSize) {
#if kLargeEmbeddedBlock
		/* only need actually to delete if end result is nothing in the buffer */
		if ((ODULong)containerSize <= fBufferBegin) {
			ODULong	amountToDelete;
			if (fSize > fBufferBegin)
				amountToDelete = fBufferBegin - (ODULong) containerSize;		/* don't delete amount in buffer */
			else
				amountToDelete = fSize - (ODULong) containerSize;			/* else delete from end */
			CMDeleteValueData(fParentValue,  (CMCount) containerSize, (CMSize) amountToDelete);
			fBufferBegin = (ODULong) containerSize;		/* so it would match fSize => empty buffer */
		}
#else
	CMDeleteValueData(fParentValue, 										
						(CMCount) containerSize,
						(CMSize) (fSize - (ODULong) containerSize));
#endif
		fSize	= (ODULong) containerSize;	
	
		trunchandler =  kODTrue;
	}
	else
		trunchandler =  kODFalse;
       return trunchandler;
}

CMSize ODEmbeddedHandlers::ContainerSizeHandler()
{
	fPosition = fSize;						/* moral equivalent of seek/tell	*/
	
	return ((CMSize) fSize);
}

void ODEmbeddedHandlers::ReadLabelHandler(CMMagicBytes magicByteSequence,
									 CMContainerFlags *flags, CM_USHORT *bufSize,
									 CM_USHORT *majorVersion, CM_USHORT *minorVersion,
									 CMSize *tocOffset, CMSize *tocSize)
{
	ODULong			labelSize;
	ContainerLabelFmt	theLabel;

	/* Seek to the end of the label at the end of the value and read it...								*/
	
	this->SeekHandler(-(ODSLong)sizeof(ContainerLabelFmt), kCMSeekEnd);
	labelSize = (ODULong) this->ReadHandler((CMPtr) &theLabel,
												(CMSize) sizeof(ODUByte),
												(CMCount) sizeof(ContainerLabelFmt));
	
	if (labelSize != sizeof(ContainerLabelFmt)) {		/* must have read it all!							*/
		CMError(fSession, "Embedded container (type \"^0\") label could not be read!", fTypeName);
		return;
	}
	
	/* Return all the label info...																												*/
	
	ODBlockMove(theLabel.magicBytes, magicByteSequence, 8);
#if kCMDefaultEndian
	/* little endian machine */
	if ((theLabel.flags & kCMLittleEndianTwin) == 0) {
#else
	/* big endian machine */
	if (theLabel.flags & kCMLittleEndianTwin) {
#endif
		fReverseEndian = kODTrue;
		*flags = ODFlipShort(theLabel.flags);
		*bufSize = ODFlipShort(theLabel.bufSize);
		*majorVersion = ODFlipShort(theLabel.majorVersion);
		*minorVersion = ODFlipShort(theLabel.minorVersion);
		*tocOffset = ODFlipLong(theLabel.tocOffset);
		*tocSize = ODFlipLong(theLabel.tocSize);
	}
	else {
		*flags = (CMContainerFlags)theLabel.flags;
		*bufSize = (CM_USHORT)theLabel.bufSize;
		*majorVersion = (CM_USHORT)theLabel.majorVersion;
		*minorVersion = (CM_USHORT)theLabel.minorVersion;
		*tocOffset = (CMSize)theLabel.tocOffset;
		*tocSize = (CMSize)theLabel.tocSize;
	}
}

void ODEmbeddedHandlers::WriteLabelHandler(CMMagicBytes magicByteSequence,
										CMContainerFlags flags, CM_USHORT bufSize,
										CM_USHORT majorVersion, CM_USHORT minorVersion,
										CMSize tocOffset, CMSize tocSize)
{
	ODULong			labelSize;
	ContainerLabelFmt	theLabel;

	/* Fill in the label buffer with the info...																					*/
	
	flags = (CMContainerFlags) ((CM_USHORT)flags & ~kCMLittleEndianTwin);	/* ignore what is passed in */
	if (fReverseEndian) {
		theLabel.flags = ODFlipShort(flags | (kCMLittleEndianTwin & ~kCMDefaultEndian));
		theLabel.bufSize = ODFlipShort(bufSize);
		theLabel.majorVersion = ODFlipShort(majorVersion); 
		theLabel.minorVersion = ODFlipShort(minorVersion);
		theLabel.tocOffset = ODFlipLong(tocOffset);
		theLabel.tocSize = ODFlipLong(tocSize);
	}
	else {
		theLabel.flags = (ODUShort)(flags | kCMDefaultEndian);
		theLabel.bufSize = (ODUShort)bufSize;
		theLabel.majorVersion = (ODUShort)majorVersion; 
		theLabel.minorVersion = (ODUShort)minorVersion;
		theLabel.tocOffset = (ODULong)tocOffset;
		theLabel.tocSize = (ODULong)tocSize;
	}
	
	ODBlockMove(magicByteSequence, theLabel.magicBytes, 8);

	/* Write the label to the end of the embedded container value...*/
	
	this->SeekHandler(0, kCMSeekEnd);
	labelSize = (ODULong) this->WriteHandler((CMPtr) &theLabel,
											(CMSize) sizeof(unsigned char),
											(CMCount) sizeof(ContainerLabelFmt));

	if (labelSize != sizeof(ContainerLabelFmt))
		THROW(kODErrBentoErr);
}

CMValue ODEmbeddedHandlers::ReturnParentValueHandler()
{
	return fParentValue;
}

CM_UCHAR* ODEmbeddedHandlers::ReturnContainerNameHandler()
{
	return ((CM_UCHAR *) fTypeName);
}

CMType ODEmbeddedHandlers::ReturnTargetTypeHandler(CMContainer container)
{
	CMType indirectType;
	
	CMSetMetaHandler(fSession,
					kODIndirectValueGlobalName,
					IndirectDynamicValueMetahandler);
	indirectType = CMRegisterType(container, kODIndirectValueGlobalName);
						
	return indirectType;
}

void ODEmbeddedHandlers::ExtractDataHandler(CMDataBuffer buffer,
 												CMSize size, CMPrivateData data)
{
	ODBoolean	reverseEndian = fReverseEndian;
	
	if ((long)size < 0) {	/* this means it is endian-ness netural	*/
		size = -(long)size;
		reverseEndian = kODFalse;
	}
	
	if (reverseEndian)
		ODFlipMove(buffer, data, (size_t)size);
	else
		ODBlockMove(buffer, data, (size_t)size);
}

void ODEmbeddedHandlers::FormatDataHandler(CMDataBuffer buffer,
 									CMSize size, CMPrivateData data)
{
	ODBoolean	reverseEndian = fReverseEndian;
	
	if ((long)size < 0) {	/* this means it is endian-ness netural	*/
		size = -(long)size;
		reverseEndian = kODFalse;
	}
	
	if (reverseEndian)
		ODFlipMove(data, buffer, (size_t)size);
	else
		ODBlockMove(data, buffer, (size_t)size);
}
