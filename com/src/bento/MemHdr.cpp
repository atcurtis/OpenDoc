/* @(#)Z 1.10 com/src/bento/MemHdr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:34 (96/10/29 09:13:04) */
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
	File:		MemHdr.cpp

	Contains:	Class definition for ODMemBentoHandlers class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 5/26/95	VL		1251403: Multithreading naming support.
		 <5>	 5/16/95	EL		1249948: Implemenation truncation of memory
									container.
		 <4>	 12/9/94	EL		#1182308 Allows non-byte swapping
									format/extract.
		 <3>	11/14/94	VL		1188257: Use Bento errors in BenotDef.h.
		 <2>	 6/20/94	CC		ODMemoryHeap* changed to ODMemoryHeapID.
		 <1>	 5/27/94	VL		first checked in
	To Do:
*/

#ifndef	_ODTYPES_
#include "ODTypes.h"
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
#ifndef	_ODTYPEPS_
#include "ODTypesP.h"
#endif
#endif

#ifndef _MEMHDR_
#include "MemHdr.h"
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

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _ERRORDEF_
#include "ErrorDef.xh"
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
THz	GetZone() { return kODNULL; }
void	SetZone(THz aZone) { aZone = kODNULL; }
THz	HandleZone(Handle fHandle) { return kODNULL; }
#endif 

//==============================================================================
// Constants
//==============================================================================

const ODType kODBentoMemoryTypeName = "MemoryCtr";

//==============================================================================
// Scalar Types
//==============================================================================

struct ContainerLabelFmt {			/* Layout of a container label:			*/
 unsigned char  magicBytes[8];		/* 8 bytes: the magic byte identifier	*/
 unsigned short flags;				/* 2	the label flag					*/
 unsigned short bufSize;			/* 2	TOC buffer size / 1024			*/
 unsigned short majorVersion;		/* 2	major format version number		*/
 unsigned short minorVersion;		/* 2	minor format version number		*/
 unsigned long	tocOffset;			/* 4	offset to start of TOC			*/
 unsigned long	tocSize;			/* 4	total byte size of the TOC		*/
};
typedef struct ContainerLabelFmt ContainerLabelFmt;


#pragma segment MemHdr

//==============================================================================
// ODMemBentoHandlers
//==============================================================================

ODMemBentoHandlers::ODMemBentoHandlers(CMSession session, ODHandle data)
{
	fSession        = session;	
	fData           = data;
	fAllocated      = kODFalse;
	fSize           = 0;
	fSeekValid      = kODFalse;
	fSeekPos        = 0;
	fLastSeekOffset = 0;
	fLastSeekMode   = 0;
	fEOFReached     = kODFalse;
	fZone			= kODNULL;
	fCurZone		= kODNULL;
}

ODMemBentoHandlers::~ODMemBentoHandlers()
{
}

void ODMemBentoHandlers::Initialize()
{
	CMSetMetaHandler(fSession, kODBentoMemoryTypeName, (CMMetaHandler)containerMetahandler);
	
	if (fData)
	{
		fZone = HandleZone((Handle) fData);
		THROW_IF_ERROR(MemError());
		
		this->SaveZone();
		
		fSize = ODGetHandleSize(fData);
		OSErr err = MemError();
		
		this->RestoreZone();
		
		THROW_IF_ERROR(err);
       
	}	
}

CMSession ODMemBentoHandlers::GetCMSession()
{
	return fSession;
}

CMRefCon ODMemBentoHandlers::OpenHandler(CMOpenMode mode)
{
ODUnused(mode);

	if (!fData)
	{
		fData = ODNewHandle(0);
		THROW_IF_ERROR(MemError());
		
		fZone = GetZone();
		THROW_IF_ERROR(MemError());
		
		fAllocated = kODTrue;
		fSize = 0;
	}
	return ((CMRefCon)this);
}

void ODMemBentoHandlers::CloseHandler()
{
	if (fAllocated)
	{
		OSErr	err;
		
		this->SaveZone();
		ODDisposeHandle(fData);
		err = MemError();
		fAllocated = kODFalse;
		this->RestoreZone();
		THROW_IF_ERROR(err);
	}
}

CMSize ODMemBentoHandlers::FlushHandler()
{
	return((CMSize)0);
}

CMSize ODMemBentoHandlers::SeekHandler(CM_LONG posOff, CMSeekMode mode)
{
	int result = 0;
	
	/* if nothing changed since last seek there's no need for another seek	*/
	if ((fSeekValid) && (fLastSeekMode == mode) && (fLastSeekOffset == (long)posOff))
		return (result);

	switch(mode)
	{
		case kCMSeekSet:
			if (posOff > fSize) 
			{
				fSeekPos = fSize;
				result = -1;
			}
			else
				fSeekPos = posOff;
			break;

		case kCMSeekEnd:
			if (posOff > 0) 
			{
				fSeekPos = fSize;
				result = -1;
			}
			else
			{
				fSeekPos = fSize + posOff;
				if (fSeekPos < 0)
					result = -1;
			}
			break;

		default:
			if (fSeekPos + posOff > fSize)
			{
				fSeekPos = fSize;
				result = -1;
			}
			else
				fSeekPos = fSeekPos + posOff;
			break;
	}
	
	fSeekValid = kODTrue;				/* indicate seek has been done			*/
	fLastSeekMode = mode;				/* remember the mode that we just used	*/
	fLastSeekOffset = (long)posOff;		/* and also the specified offset		*/
	
	return ((CMSize)result);
}

CMSize ODMemBentoHandlers::TellHandler()
{	
	return fSeekPos;
}

CMSize ODMemBentoHandlers::ReadHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	size_t amountRead = 0;
	
	amountRead = (size_t) (elementSize * theCount);
	if (fSeekPos + amountRead > fSize)
	{
		amountRead = (size_t) (fSize - fSeekPos);
		fEOFReached = kODTrue;
	}
	else
		fEOFReached = kODFalse;
	ODBlockMove(((char *)(*((Handle) fData))) + fSeekPos, buffer, amountRead);
	
	fSeekPos += amountRead;
	fSeekValid = 0;					/* stream pointer has now changed	*/
	
	return ((CMSize)amountRead);
}

CMSize ODMemBentoHandlers::WriteHandler(CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	size_t amountWritten;
	OSErr  err = noErr;
	
	if (theCount > 0) 
	{
		this->SaveZone();

		amountWritten = (size_t) (elementSize * theCount);

		if (fSeekPos + amountWritten > fSize) 
		{
			ODSetHandleSize(fData, fSeekPos + amountWritten);
			err = MemError();
			if (err == noErr) {
				fSize = ODGetHandleSize(fData);
				err = MemError();
			}
		}
		if (err == noErr) {
			ODBlockMove(buffer, ((char *)(*((Handle) fData))) + fSeekPos, amountWritten);
			fSeekPos += amountWritten;
			fSeekValid = 0;				/* stream pointer now changed			*/
		}
		
		this->RestoreZone();
		
		THROW_IF_ERROR(err);
	}
	else
		amountWritten = 0;
		
	fEOFReached = kODFalse;
	
	return ((CMSize)amountWritten);
}

CMEofStatus ODMemBentoHandlers::EOFHandler()
{
	return ((CMEofStatus)fEOFReached);
}

CMBoolean ODMemBentoHandlers::TruncHandler(CMSize containerSize)
{

	CMBoolean result = kODFalse;

	if (containerSize <= fSize) {
		if (containerSize != fSize) {
			ODSetHandleSize(fData, containerSize);
			fSize = containerSize;
		}
		result = kODTrue;
	}
	fSeekValid = 0;
	return result;
}

CMSize ODMemBentoHandlers::ContainerSizeHandler()
{
	return ((CMSize)fSize);
}

void ODMemBentoHandlers::ReadLabelHandler(CMMagicBytes magicByteSequence,
									 CMContainerFlags *flags, CM_USHORT *bufSize,
									 CM_USHORT *majorVersion, CM_USHORT *minorVersion,
									 CMSize *tocOffset, CMSize *tocSize)
{ 
	unsigned long		labelSize;
	ContainerLabelFmt	theLabel;

	/* Seek to the end of the label at the end of the container and read it...*/
	
	this->SeekHandler(-(long)sizeof(ContainerLabelFmt), kCMSeekEnd);
	labelSize = (unsigned long)this->ReadHandler((CMPtr)&theLabel,
											(CMSize)sizeof(unsigned char),
											(CMCount)sizeof(ContainerLabelFmt));
	
	fSeekValid = 0;	
	
	if (labelSize != sizeof(ContainerLabelFmt))
		THROW(kODErrBentoErr);
	
	/* Return all the label info... */
	
	ODBlockMove(theLabel.magicBytes, magicByteSequence, 8);
	*flags = (CMContainerFlags)theLabel.flags;
	*bufSize = (CM_USHORT)theLabel.bufSize;
	*majorVersion = (CM_USHORT)theLabel.majorVersion;
	*minorVersion = (CM_USHORT)theLabel.minorVersion;
	*tocOffset = (CMSize)theLabel.tocOffset;
	*tocSize = (CMSize)theLabel.tocSize;
}

void ODMemBentoHandlers::WriteLabelHandler(CMMagicBytes magicByteSequence,
										CMContainerFlags flags, CM_USHORT bufSize,
										CM_USHORT majorVersion, CM_USHORT minorVersion,
										CMSize tocOffset, CMSize tocSize)
{
	unsigned long		labelSize;
	ContainerLabelFmt	theLabel;
		
	/* Fill in the label buffer with the info...																					*/
	theLabel.flags        = (unsigned short)flags;
	theLabel.bufSize      = (unsigned short)bufSize;
	theLabel.majorVersion = (unsigned short)majorVersion; 
	theLabel.minorVersion = (unsigned short)minorVersion;
	theLabel.tocOffset 	  = (unsigned long)tocOffset;
	theLabel.tocSize      = (unsigned long)tocSize;
	
	ODBlockMove(magicByteSequence, theLabel.magicBytes, 8);

	/* Write the label to the end of the container value... */
	
	this->SeekHandler(0, kCMSeekEnd);
	labelSize = (unsigned long)this->WriteHandler((CMPtr)&theLabel,
											(CMSize)sizeof(unsigned char),
											(CMCount)sizeof(ContainerLabelFmt));
	fSeekValid = kODFalse;

	if (labelSize != sizeof(ContainerLabelFmt))
		THROW(kODErrBentoErr);
}

CMValue ODMemBentoHandlers::ReturnParentValueHandler()
{
	return kODNULL;
}

CM_UCHAR* ODMemBentoHandlers::ReturnContainerNameHandler()
{
	return kODNULL;
}

CMType ODMemBentoHandlers::ReturnTargetTypeHandler(CMContainer container)
{
ODUnused(container);

	return kODNULL;
}

void ODMemBentoHandlers::ExtractDataHandler(CMDataBuffer buffer,
 												CMSize size, CMPrivateData data)
{
	if ((ODSLong)size < 0)			/* this means it is endian-ness netural	*/
		size = -(ODSLong)size;
	ODBlockMove(buffer, data, (size_t)size);
}

void ODMemBentoHandlers::FormatDataHandler(CMDataBuffer buffer,
 									CMSize size, CMPrivateData data)
{
	if ((ODSLong)size < 0)			/* this means it is endian-ness netural	*/
		size = -(ODSLong)size;

	ODBlockMove(data, buffer, (size_t)size);
}


void ODMemBentoHandlers::SaveZone()
{
	fCurZone = GetZone();
	SetZone(fZone);
}

void ODMemBentoHandlers::RestoreZone()
{
	if (fCurZone != kODNULL) {
		SetZone(fCurZone);
		fCurZone = kODNULL;
	}
#ifdef _PLATFORM_MACINTOSH_
	else
		THROW(kODErrInvalidZone);
#endif
}
