/* @(#)Z 1.13 com/src/bento/FSHdr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:32 (96/10/29 09:12:30) */
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
	File:		FSHdr.cpp

	Contains:	Class definition for ODFSBentoHandlers class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<17>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		<16>	 5/26/95	VL		1251403: Multithreading naming support.
		<15>	 5/11/95	EL		Use ODBlockMove instead of memcpy.
		<14>	 4/25/95	EL		1242376: Synchronize label between handler
									and container after WriteLabel.
		<13>	 4/10/95	EL		1236290: No debugger warning of file not
									properly closed for DR2.
		<12>	  4/7/95	EL		1225905: More ModDate stuff to container.
									Use FlipEnd.
		<11>	 3/24/95	EL		1225905: Move state information such as mod
									date from plfmFile to here
		<10>	 3/13/95	EL		1226127 fix bug where endOfWrite is very
									close to end of file we need a new block
									for the safety label.
		 <9>	 3/10/95	EL		1223465: allocate buffer on the heap.
									1226127 fix bug buffering code destory
									safety label at file end. Scan backward for
									label if file is corrupted. 1227122
									truncate file if space was lost in crash.
		 <8>	 2/15/95	EL		1213321: Fix buffering bug where part of
									the read in data is not the same as data in
									a dirty buffer.
		 <7>	 2/10/95	VL		1205627: Make it work for readonly file.
		 <6>	 1/31/95	EL		1213321: Buffer the Bento file I/O.
		 <5>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <4>	 8/26/94	EL		#1182308 Allows non-byte swapping
									format/extract.
		 <3>	  8/8/94	VL		1170009: Wrong byte calculation in
									ReadHandler.
		 <2>	 6/18/94	MB		Correct memory includes
		 <1>	 5/27/94	VL		first checked in

	To Do:
		1) Get rid of fSession. Make it global.
		2) Share ODFlipMove among handlers.
		3) Make sure scan for valid label code work for document edited in both
		   big and little endian platform.
*/

#ifdef _PLATFORM_MACINTOSH_
#define fsFromStart	SEEK_SET
#endif

#ifndef	_ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _FSHDR_
#include "FSHdr.h"
#endif

#ifndef _BENTOHDR_
#include "BentoHdr.h"
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _FLIPEND_
#include "FlipEnd.h"
#endif

#ifndef _PLFMFILE_
#include "PlfmFile.h"
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

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

//==============================================================================
// Constants
//==============================================================================

const ODType	kODBentoFileTypeName = "FileCtr";

#define kInvalidBuffer 1				/* an invalid value for fBufferBegin */
#define ChunkHighBitsMask	~(fChunkSize-1)	/* mask for masking off the lower bits */

//==============================================================================
// Scalar Types
//==============================================================================

//struct ContainerLabelFmt {			/* Layout of a container label:			*/
// unsigned char  magicBytes[8];		/* 8 bytes: the magic byte identifier	*/
// unsigned short flags;				/* 2	as a short						*/
// unsigned short bufSize;			/* 2	TOC buffer size / 1024			*/
// unsigned short majorVersion;		/* 2	major format version number		*/
// unsigned short minorVersion;		/* 2	minor format version number		*/
// unsigned long	tocOffset;			/* 4	offset to start of TOC			*/
// unsigned long	tocSize;			/* 4	total byte size of the TOC		*/
//};
//typedef struct ContainerLabelFmt ContainerLabelFmt;


#pragma segment FSHdr

static void ODFlipMove(ODPtr from, ODPtr to, ODULong size)
{
	char	*dest = (char *)to + size - 1;
	char	*src = (char *)from;
	
	while (size--)
		*dest-- = *src++;			
}

//==============================================================================
// ODFSBentoHandlers
//==============================================================================

ODFSBentoHandlers::ODFSBentoHandlers(CMSession session, PlatformFile* file)
{
	fFile = file;
	fSession = session;
	fBuffer = kODNULL;
	fReverseEndian = kODFalse;		/* assume same Endian-ness				*/
}

ODFSBentoHandlers::~ODFSBentoHandlers()
{
}

void ODFSBentoHandlers::Initialize()
{
	CMSetMetaHandler(fSession, kODBentoFileTypeName, (CMMetaHandler) containerMetahandler);
}

CMSession ODFSBentoHandlers::GetCMSession()
{
	return fSession;
}

CMRefCon ODFSBentoHandlers::OpenHandler(CMOpenMode mode)
{
	Environment* ev = somGetGlobalEnvironment();
	ODULong	chunkSize;

	fFile->Open();
	fFileSize = fFile->GetEndOfFile();
	fPhysicalFileSize = fFileSize;	/* must set it here because used in read label */
	fWriteLimit = fFileSize;		/* normally initialize in read label, but we may not have label */
	fLogicalPos = 0;
	fBufferBegin = kInvalidBuffer; /* buffer not loaded yet */
	fBufferDirty = kODFalse;
	fHasLabel = kODFalse;
	chunkSize = fFile->GetAllocationBlockSize();
	/* make sure it is a multiple of 2 */
	for (fChunkSize = MininumChunkSize; fChunkSize < chunkSize; fChunkSize *= 2)
		;
	try
        {
		fBuffer = (ODSByte *)ODNewPtr(fChunkSize);
        }
	catch ( ODException _exception)
        {
		/* there may be platform that returns a large chunksize to try to read 	*/
		/* the whole file into memory. If that fails, we can try to allocate   	*/
		/* with a default chunksize so that it can still run. In theory even   	*/
		/* if the default chunksize memory allocation fails, we can still catch	*/
		/* and run without buffering, but if memory is really so short at		*/
		/* open time, may as well forget about it								*/
                SetErrorCode(kODNoError);
		fChunkSize = DefaultChunkSize;
		fBuffer = (ODSByte *)ODNewPtr(fChunkSize);	
	}
	if ((strcmp(mode, "rb") == 0) || fFile->IsLocked())
		fOpenReadOnly = kODTrue;
	else {
		fOpenReadOnly = kODFalse;
		if (fFileSize > sizeof(ContainerLabelFmt)) {
			CM_UCHAR magicBytes[8];
			CMContainerFlags flags;
			CM_USHORT bufSize;
			CM_USHORT majorVersion;
			CM_USHORT minorVersion;
			ODULong tocOffset;
			ODULong tocSize;
			this->ReadLabelHandler(magicBytes, &flags, &bufSize,
					 &majorVersion, &minorVersion,
					 &tocOffset, &tocSize);
		}
	}

	return ((CMRefCon) this);
}

void ODFSBentoHandlers::CloseHandler()
{
	if (fBufferDirty)
		this->FlushBuffer();
	if (fBuffer != kODNULL) {
		ODDisposePtr(fBuffer);
		fBuffer = kODNULL;
	}
	if (fOpenReadOnly == kODFalse) {
		fFile->SetEndOfFile(fFileSize);		/* remove the reserved space */
	}
	fFile->Close();
	fHasLabel = kODFalse;
}

CMSize ODFSBentoHandlers::FlushHandler()
{
	Environment* ev = somGetGlobalEnvironment();
	int result = 0;
	
	ODVolatile(result);
	
	if (fBufferDirty)
		this->FlushBuffer();
	fFile->FlushVolume();

	return result;
}

CMSize ODFSBentoHandlers::SeekHandler(CM_LONG posOff, CMSeekMode mode)
{
	if (mode == kCMSeekEnd)
		fLogicalPos = fFileSize + posOff;
	else if (mode == kCMSeekCurrent) {
		fLogicalPos = fLogicalPos + posOff;
	}
	else
		fLogicalPos = posOff;
	
	return 0;
}

CMSize ODFSBentoHandlers::TellHandler()
{	
	return fLogicalPos;
}

CMSize ODFSBentoHandlers::ReadHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	ODULong	endOfRead, firstReadBlock, lastReadBlock;

	ODSLong amountRead = elementSize * theCount;
	endOfRead = fLogicalPos + amountRead;
	if (endOfRead > fFileSize) {
		amountRead = fFileSize - fLogicalPos;
		endOfRead = fFileSize;
	}
	firstReadBlock = fLogicalPos & ChunkHighBitsMask;
	lastReadBlock = (endOfRead-1) & ChunkHighBitsMask;
		
	if (firstReadBlock == lastReadBlock) {
		/* the content to read can be in a single buffer */
		if (firstReadBlock != fBufferBegin) /* the block is not loaded */
			this->ReloadBuffer();
		/* After we make sure the buffer is loaded, we just copy it from the buffer */
		ODBlockMove(fBuffer+fLogicalPos-fBufferBegin, buffer, amountRead);
	}
	else { /* for bigger block we read it directly bypassing the buffer */
		/* if we read directly and part of data is in dirty buffer then write it first */
		if ((fBufferDirty != kODFalse) && (firstReadBlock <= fBufferBegin) && (fBufferBegin <= lastReadBlock))
			this->FlushBuffer();
		fFile->SetFilePos(fsFromStart, fLogicalPos);
		fFile->Read((ODSByte*) buffer, &amountRead);
	}
	
	fLogicalPos = endOfRead;
	
	return ((CMSize)amountRead);
}

CMSize ODFSBentoHandlers::WriteHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	ODULong			endOfWrite, newEndOfFile, beginWriteBlock, toMove, theGap;
	ODSLong			labelSize, amountWritten;
	ODBoolean		needToWrite;
	
	if (theCount > 0) {	
	
		amountWritten = elementSize * theCount;
		endOfWrite = fLogicalPos + amountWritten;
		beginWriteBlock = fLogicalPos & ChunkHighBitsMask;
		needToWrite = kODTrue;

		if ((beginWriteBlock == ((endOfWrite-1) & ChunkHighBitsMask)) && (beginWriteBlock != fBufferBegin)) {
			/* changes are all in a single buffer but data not loaded, load it now */
			this->ReloadBuffer();
		}

		if (fBufferBegin != kInvalidBuffer) { /* we have a buffer */
			if (fLogicalPos < fBufferBegin) { /* the tail of the write may touch the buffer */
				if (endOfWrite > fBufferBegin) { /* yes, the tail touches the buffer */
					/*
					
						fLogicalPos     fBufferBegin      endOfWrite
							 |              |            	 |       |
											<-       ChunkSize      ->
											<-  toMove      ->
					*/
					toMove = endOfWrite - fBufferBegin;  /* move so many bytes into the buffer */
					if (toMove > fChunkSize)			 /* but no larger than the buffer itself */
						toMove = fChunkSize;
					/* update the part of the buffer that has been changed */
					ODBlockMove((char *)buffer+fBufferBegin-fLogicalPos, fBuffer, toMove);
					/* note that we do not need to dirty it because we still write it out */
				}
			}
			else { /* the front part of the write may touch the buffer */
				theGap = fLogicalPos - fBufferBegin;
				if (theGap < fChunkSize) { /* yes, it does touch the buffer */
					/*
					
						fBufferBegin    fLogicalPos            endOfWrite
							 |               |        |            |
							 <-       ChunkSize      ->
							 <-   theGap    ->
							                 <-   amountWritten   ->
							                 <-toMove->

					*/
					toMove = amountWritten;
					if (toMove + theGap > fChunkSize)
						toMove = fChunkSize - theGap;
					else {
						fBufferDirty = kODTrue; /* dirty it so we would write it out later */
						needToWrite = kODFalse;	/* for now, no writing is necessary */
					}
					ODBlockMove(buffer, fBuffer + theGap, toMove); /* update the buffer */
				}
			}
		}
		 
		if (endOfWrite > fFileSize) { /* we are appending */
			if (endOfWrite > fWriteLimit) {
				ODSLong	newWriteLimit;
				/* extend it to block boundry */
				newEndOfFile = ((endOfWrite + fChunkSize)) & ChunkHighBitsMask;
				if (fHasLabel != kODFalse) {
					/* must have enough room to end of file to fit in a label */
					if ((newEndOfFile - endOfWrite) < sizeof(ContainerLabelFmt))
						newEndOfFile = newEndOfFile + fChunkSize;
					newWriteLimit = newEndOfFile - sizeof(ContainerLabelFmt);
				}
				else
					newWriteLimit = newEndOfFile;
				/* We will add at least another block to file, so we need a new label at end */
				fFile->SetEndOfFile(newEndOfFile);
				fPhysicalFileSize = newEndOfFile;
				fWriteLimit = newWriteLimit;
				if (fHasLabel != kODFalse) {
					if ((fWriteLimit & ChunkHighBitsMask) == fBufferBegin) {
						/* the label at the end will overlap with the buffer, put it in the buffer */
						ODBlockMove(&fLabel, fBuffer + fChunkSize - sizeof(ContainerLabelFmt), sizeof(ContainerLabelFmt));
					}
					fFile->SetFilePos(fsFromStart, fWriteLimit);
					labelSize = sizeof(ContainerLabelFmt);
						fFile->Write((const ODSByte*)&fLabel, &labelSize);
						/* if fails, no safety label at end of end, that is not end of the world */
				}
			}
			fFileSize = endOfWrite;
		}
		if (needToWrite != kODFalse) {
			fFile->SetFilePos(fsFromStart, fLogicalPos);
			fFile->Write((const ODSByte*)buffer, &amountWritten);
		}
		fLogicalPos = endOfWrite;
	} else
		amountWritten = 0;

	return ((CMSize)amountWritten);
}

void ODFSBentoHandlers::FlushBuffer()
{
	ODSLong		theSize = fChunkSize;	

	fFile->SetFilePos(fsFromStart, fBufferBegin);
	if (fPhysicalFileSize - fBufferBegin < fChunkSize)
		theSize = fPhysicalFileSize - fBufferBegin;
	fFile->Write(fBuffer, &theSize);
	fBufferDirty = kODFalse;
}

void ODFSBentoHandlers::ReloadBuffer()
{
	ODSLong		theSize;	

	if (fBufferDirty != kODFalse)
		this->FlushBuffer();
	fBufferBegin = fLogicalPos & ChunkHighBitsMask;
	fFile->SetFilePos(fsFromStart, fBufferBegin);
	theSize = fChunkSize;
	if (fPhysicalFileSize - fBufferBegin < fChunkSize)
		theSize = fPhysicalFileSize - fBufferBegin;
	fFile->Read(fBuffer, &theSize);
}

CMEofStatus ODFSBentoHandlers::EOFHandler()
{
	return ((CMEofStatus) kODFalse);

}

CMBoolean ODFSBentoHandlers::TruncHandler(CMSize containerSize)
{
	fFileSize = containerSize;		
	return kODTrue;
}

CMSize ODFSBentoHandlers::ContainerSizeHandler()
{
	return ((CMSize) fFileSize);
}

void ODFSBentoHandlers::ReadLabelHandler(CMMagicBytes magicByteSequence,
									 CMContainerFlags *flags, CM_USHORT *bufSize,
									 CM_USHORT *majorVersion, CM_USHORT *minorVersion,
									 CMSize *tocOffset, CMSize *tocSize)
{
	ODULong		endOfTOC, tempFileEnd;
	char *magicSequence = "\xA4""CM""\xA5""Hdr""\xD7";		/* Must be 8 characters		*/
	ODBoolean 		labelMatches = fHasLabel;

	/* Seek to the end of the label at the end of the container and read it...*/
	
	tempFileEnd = fFileSize;
	while (tempFileEnd > sizeof(ContainerLabelFmt)) {
		fLogicalPos = tempFileEnd - sizeof(ContainerLabelFmt);
		/* if we really want to speed it up, we could compare it directly from the buffer 	*/
		/* but since we only need to loop through this in corrupted file and that does not	*/
		/* occur very often, and even when it happens, the current speed is acceptable	  	*/
		/* so we would leave it as it is now												*/
		if (this->ReadHandler((CMPtr)&fLabel, (CMSize)sizeof(unsigned char), 8) !=  8)
				THROW(kODErrBentoErr);
		if (memcmp(&fLabel.magicBytes, magicSequence, 8) == 0)
			labelMatches = kODTrue;
		if (labelMatches) {
			/* Return all the label info... */
			fLogicalPos = tempFileEnd - sizeof(ContainerLabelFmt);
			if (this->ReadHandler((CMPtr)&fLabel, 
								  (CMSize)sizeof(unsigned char), 
								  sizeof(ContainerLabelFmt)) !=  sizeof(ContainerLabelFmt))
					THROW(kODErrBentoErr);
			ODBlockMove(&fLabel.magicBytes, magicByteSequence, 8);
#if kCMDefaultEndian
			/* little endian machine */
			if ((fLabel.flags & kCMLittleEndianTwin) == 0) {
#else
			/* big endian machine */
			if (fLabel.flags & kCMLittleEndianTwin) {
#endif
				fReverseEndian = kODTrue;
				*flags = ODFlipShort(fLabel.flags);
				*bufSize = ODFlipShort(fLabel.bufSize);
				*majorVersion = ODFlipShort(fLabel.majorVersion);
				*minorVersion = ODFlipShort(fLabel.minorVersion);
				*tocOffset = ODFlipLong(fLabel.tocOffset);
				*tocSize = ODFlipLong(fLabel.tocSize);
			}
			else {
				fReverseEndian = kODFalse;
				*flags = (CMContainerFlags)fLabel.flags;
				*bufSize = (CM_USHORT)fLabel.bufSize;
				*majorVersion = (CM_USHORT)fLabel.majorVersion;
				*minorVersion = (CM_USHORT)fLabel.minorVersion;
				*tocOffset = (CMSize)fLabel.tocOffset;
				*tocSize = (CMSize)fLabel.tocSize;
			}
			endOfTOC = *tocOffset + *tocSize + sizeof(ContainerLabelFmt);
			if (endOfTOC == tempFileEnd) {
				/* match exactly, this is most likely to be the label */
				if (endOfTOC != fFileSize) {
					WARNMSG_DEBUG(WARN_INDEX(-1),
					"The file was corrupted! I will just do my best to read it");
					fFileSize = endOfTOC;
				}
				fHasLabel = kODTrue;
				break;
			}
			if (endOfTOC < tempFileEnd) {
				ContainerLabelFmt tempLabel;
				fLogicalPos = endOfTOC - sizeof(ContainerLabelFmt);
				if (this->ReadHandler((CMPtr)&tempLabel, 
									  (CMSize)sizeof(unsigned char), 
									  sizeof(ContainerLabelFmt)) !=  sizeof(ContainerLabelFmt))
					THROW(kODErrBentoErr);
				if (memcmp(&tempLabel, &fLabel, sizeof(ContainerLabelFmt)) == 0) {
//#if ODDebug
//					WARN("The file was not closed properly! You get away with it this time");
//#endif
					/* we passed the sanity check, so we can truncate the file and use the label */
					fFileSize = endOfTOC;
					fHasLabel = kODTrue;
					break;
				}
				/* even if we did not pass the test, if the label was at the end, still accept it */
				if (tempFileEnd == fFileSize) {
					WARNMSG_DEBUG(WARN_INDEX(0),
					     "File looks funny! Send a copy to Bento folks");
					fHasLabel = kODTrue;
					break;
				}
			}
		} /* it is a label */
		/* otherwise keep scanning backwards */
		labelMatches = kODFalse;
		tempFileEnd -= 1;
	}
	if (fHasLabel) {
		fPhysicalFileSize = fFileSize;
		fWriteLimit = fFileSize;
		if ((fOpenReadOnly == kODFalse) && (fFile->GetEndOfFile() != fFileSize)) {
			fFile->SetEndOfFile(fFileSize);		/* get back the correct size */
		}
	}
	else
		WARNMSG_DEBUG(WARN_INDEX(0),
		     "File has no valid Bento label. Cannot be recovered");
}

void ODFSBentoHandlers::WriteLabelHandler(CMMagicBytes magicByteSequence,
										CMContainerFlags flags, CM_USHORT bufSize,
										CM_USHORT majorVersion, CM_USHORT minorVersion,
										CMSize tocOffset, CMSize tocSize)
{
	ODULong				labelSize;
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

	/* Write the label to the end of the container value...																*/
	
	this->SeekHandler(0, kCMSeekEnd);
	labelSize = (unsigned long)this->WriteHandler((CMPtr)&theLabel,
											(CMSize)sizeof(unsigned char),
											(CMCount)sizeof(ContainerLabelFmt));
	
	if (labelSize != sizeof(ContainerLabelFmt))
		THROW(kODErrBentoErr);

	ODBlockMove(&theLabel, &fLabel, sizeof(ContainerLabelFmt));
	fHasLabel = kODTrue;
	fPhysicalFileSize = fFileSize;
	fWriteLimit = fFileSize;
	fFile->SetEndOfFile(fFileSize);		/* get back the correct size */
}

CMValue ODFSBentoHandlers::ReturnParentValueHandler()
{
	return kODNULL;
}

CM_UCHAR* ODFSBentoHandlers::ReturnContainerNameHandler()
{
	static Str63	name;
	
	fFile->GetAsciiName((char*)name,sizeof(name));
	return ((CM_UCHAR *) name);
}

CMType ODFSBentoHandlers::ReturnTargetTypeHandler(CMContainer container)
{
ODUnused(container);

	return kODNULL;
}

void ODFSBentoHandlers::ExtractDataHandler(CMDataBuffer buffer,
 												CMSize size, CMPrivateData data)
{
	ODBoolean	reverseEndian = fReverseEndian;
	
	if ((CM_LONG)size < 0) {	/* this means it is endian-ness netural	*/
		size = -(CM_LONG)size;
		reverseEndian = kODFalse;
	}
	
	if (reverseEndian)
		ODFlipMove(buffer, data, (size_t)size);
	else
		ODBlockMove(buffer, data, (size_t)size);
}

void ODFSBentoHandlers::FormatDataHandler(CMDataBuffer buffer,
 									CMSize size, CMPrivateData data)
{
	ODBoolean	reverseEndian = fReverseEndian;
	
	if ((CM_LONG)size < 0) {	/* this means it is endian-ness netural	*/
		size = -(CM_LONG)size;
		reverseEndian = kODFalse;
	}
	
	if (reverseEndian)
		ODFlipMove(data, buffer, (size_t)size);
	else
		ODBlockMove(data, buffer, (size_t)size);
}
