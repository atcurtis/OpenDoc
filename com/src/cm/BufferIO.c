/* @(#)Z 1.4 com/src/cm/BufferIO.c, odstorage, od96os2, odos29646d 96/11/15 15:26:39 (96/10/29 09:15:41) */
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
	File:		BufferIO.c

	Contains:	Container Manager Buffered Data I/O Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182308 BufferIO should use value's
									container and not the updating container to
									determine endian-ness.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<<  BufferIO.c  >>>                           |
 |                                                                           |
 |               Container Manager Buffered Data I/O Routines                |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  8/3/92                                   |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file contains the routines to allocate and free I/O buffers used for buffered reading
 and writing special data in "chunks" of 1, 2, and 4 byte quantities.  This is NOT a
 generalized I/O package that handles arbitrary sized data.  It is intended as an interface
 to the 1, 2, and 4 byte formatting and extraction handlers for portably and efficiently
 handling internal information written to the container.  This includes, for example,
 updating instructions and TOC entries.  
 
 The main use of these I/O routines is to buffer this internal data as value data for
 some value.  Thus in the case of updating instructions, the updating information is
 buffered as value data for an object's special "updating" property.
 
 Although the main use is to write value data for a value, the buffering routines also
 support I/O directly to and from the container's I/O handlers.  The buffering algorithms
 are basically the same.  The only difference is that in this case a value refNum is not
 supplied.  An example of such a direct handler use would be TOC I/O.
 
 Multiple buffers of different sizes can be set up simultaneously for multiple values.  The
 routines in this file take care of buffering and deblocking as necessary.  Obviously, if
 the I/O handlers are being called directly, there can only be one such buffered use of
 these handlers, unless care is taken to properly protect the container I/O position (seek) 
 pointer(s).
 
 The use of the routines in this package follow a specific protocol as follows ("|" here
 means "or" as an alternative):
 
 For input:
 
 			if (setjmp(setJmpEnv)) {
				cmReleaseIOBuffer(ioBuffer);
				ERRORx(...)
				return (failure);
			}
			
			ioBuffer = cmUseIOBuffer(container, bufferSize, &setJmpEnv);
			
			cmNewBufferedInputData(ioBuffer, valueRefNum | NULL, totalAmountToRead);
			cmReadBufferedData(ioBuffer, 1 | 2 | 4);
			           - - -
			
			cmReleaseIOBuffer(ioBuffer);
 
 For output:
  
 			if (setjmp(setJmpEnv)) {
				cmReleaseIOBuffer(ioBuffer);
				ERRORx(...)
				return (failure);
			}

			ioBuffer = cmUseIOBuffer(container, bufferSize, &setJmpEnv);
			cmNewBufferedOutputData(ioBuffer, valueRefNum | NULL);
			
			cmWriteBufferedData(ioBuffer, 1 | 2 | 4);
			           - - -
			
			cmFlushOutputBuffer(ioBuffer);
			cmReleaseIOBuffer(ioBuffer);
 
 A setjmp/longjmp environment is defined for both input and output.  The I/O routines
 use this technique rather than having to check for errors on each I/O call.  All I/O is
 beacketed between a cmUseIOBuffer() and cmReleaseIOBuffer().  cmNewBufferedInputData() is
 called for each new value to read to define that value and the total amount to read.
 cmNewBufferedOutputData() just defines the value for output.  cmReadBufferedData() is 
 used to read 1, 2, and 4 byte quantities while cmWriteBufferedData() is used to write
 those quantities.  The write also requires a cmFlushOutputBuffer() at the end to make
 sure the last buffer is written.
 
 There are a few other special calls defined here.  But the above protocal must be 
 followed to do the I/O properly.
*/

 
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
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
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __ERRORRPT__
#include "ErrorRpt.h"      
#endif
#ifndef __UTILITYROUTINES__
#include "Utility.h"        
#endif
#ifndef __BUFFEREDIO__
#include "BufferIO.h"  
#endif

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment BufferedIO
#endif


/* All I/O buffers are linked together on a chain attached to a container. The buffers	*/
/* are actually part of an "I/O control block" defined below.  This information is only	*/
/* known to this file.  All callers to the routines here only know it as an anonymous		*/
/* "void *" pointer.																																		*/

struct IOControl {												/* Layout of an I/O control block:						*/
	ListLinks		 		ioLinks;								/*    in-use buffers links (must be first)		*/
	TOCValueHdrPtr	theValueHdr;						/*		refNum whose value data to do I/O on		*/
	ContainerPtr 		container;							/*		(updating) container control block ptr	*/
	CM_LONG			 		maxBufSize;							/*		max size of the buffer									*/
	CM_LONG 		 		bufferPosition;					/*		0-rel. next byte to use in updateBuffer	*/
	CM_LONG			 		bufferReadOffset;				/*		offset to next buffer to read in value	*/
	CM_ULONG			 	dataSize;								/*		total size of data to be read						*/
	CM_ULONG			 	ioPos;									/*		current container I/O position					*/
	union {																	/*		setjmp environment for buffered I/O			*/
		jmp_buf				jmpBuf;									/*		      *** PORTABILITY PROBLEM ***				*/
		CM_CHAR				firstByte;							/*       Don't assume the layout of a jmp_buf	*/
	} bufferIOEnv;													/*			 firstByte used to memcpy to  jmp_buf	*/
	CM_CHAR 	 			dataBuffer[1];					/* 		the START of the data I/O buffer				*/
};
typedef struct IOControl IOControl, *IOControlPtr;

/* CAUTION: I/O error "recovery" here is done by using a longjmp on a setjmp environment*/
/* 					passed to cmUseIOBuffer() below.  In order to save that environment, a 			*/
/*					memcpy() is done to copy the cmUseIOBuffer() parameter to the above struct.	*/
/*					No assumptions are made about the layout of a setjmp's jmp_buf!  For 				*/
/*					example, it is never assumed to be an array.  Therefore, in order to 				*/
/* 					PORTABLY pass the jmp_buf address to a memcpy(), a union is done to the 		*/
/* 					first byte of the jmp_buf to address it.  To some "picky" compilers (in			*/
/*					particular, the Zortech MPW C/C++ compiler), addressing the first byte of a */
/* 					jmp_buf which IS an array as "&foo" is treated as an error (it expects 			*/
/*					"&foo[0]" -- I always thought this was benign).  To avoid such problems, 		*/
/* 					the above "kludge" is used. 																								*/


/*----------------------------------------*
 | cmUseIOBuffer - allocate an I/O buffer |
 *----------------------------------------*
 
 This is used to allocate an I/O buffer and its associated control information.  The
 buffer is used to buffer data for input or output using cmReadBufferedData() and
 cmWriteBufferedData() respectively.
 
 The I/O control block pointer allocated is returned as the function result as an
 anonymous "void *" pointer.  The caller should view this as a "buffer pointer" to be
 passed to all the other routines in this file.
 
 An error is reported for allocation errors.  If the error reporter returns, NULL is
 returned as the function result.
 
 The maximum size of the buffer to be allocated is passed.  A set setjmp/longjmp
 environment variable is also passed for read and write error reporting and recovery.  The
 setjmp/longjmp is used rather than have to check for errors on each I/O call of the
 buffered I/O routines.
  
 Note, if the longjmp is taken, it is the caller's responsibility (in the setjmp code) to
 report the error message.  This is done because the buffered I/O is used in a number of
 contexts and a more appopriate error message can be reported by the caller.
 
 The I/O buffer control block allocated is added to a chain whose header is in the
 updating container associated with the specified container.  The buffer should be 
 released when is't use is no longer needed by calling cmReleaseIOBuffer().
*/

void *cmUseIOBuffer(ContainerPtr container, long maxBufferSize, jmp_buf *ioEnv)
{
	IOControlPtr io;
	
	/* The actual buffer allocated is 4 greater than the size required. This is enough of */
	/* a pad (actually we use sizeof(CM_ULONG)) to allow cmWriteBufferData to always have	*/
	/* room to write the data just added.																									*/
	
	if ((io = (IOControlPtr)CMmalloc(sizeof(IOControl) + maxBufferSize +  sizeof(CM_ULONG))) == NULL) {
		ERROR1(CM_err_NoDataBuffer, CONTAINERNAME);
		return (NULL);
	}
	
	io->container			 = container;										/* remember the container						*/
	io->maxBufSize		 = maxBufferSize;								/* remember how big a buffer we got	*/
	memcpy(&io->bufferIOEnv.firstByte, ioEnv, sizeof(jmp_buf)); /* remember setjmp env.		*/
	
	io->bufferPosition = 0;														/* init buffer index for output			*/
	
	return ((void *)cmAppendListCell(&io->container->updatingContainer->activeIOBuffers, io));
}


/*--------------------------------------------------*
 | cmReleaseIOBuffer - release (free) an I/O buffer |
 *--------------------------------------------------*
 
 This is called to free a buffer, ioBuffer,  allocated by cmUseIOBuffer(). If the caller
 "forgets" to call this routine, then when the container is closed, cmFreeAllIOBuffers()
 will be called to make sure all the buffers are free for that container.
*/

void cmReleaseIOBuffer(void *ioBuffer)
{
	ContainerPtr container;
	
	if (ioBuffer != NULL) {
		container = ((IOControlPtr)ioBuffer)->container->updatingContainer;
		CMfree(cmDeleteListCell(&container->activeIOBuffers, ioBuffer));/*unlink/free buffer*/
	}
}


/*---------------------------------------------------------------------*
 | cmFreeAllIOBuffers - free all I/O buffers allocated for a container |
 *---------------------------------------------------------------------*
 
 This is called at container close time to make sure all buffers allocated for that
 container are released (freed).
*/

void cmFreeAllIOBuffers(ContainerPtr container)
{
	IOControlPtr io, next;
	
	io = (IOControlPtr)cmGetListHead(&container->updatingContainer->activeIOBuffers);

	while (io) {																	/* free all buffers still chained...		*/
		next = (IOControlPtr)cmGetNextListCell(io);
		CMfree(io);
		io = next;
	}
}


/*------------------------------------------------------------*
 | cmNewBufferedOutputData - initialize for buffered writing  |
 *------------------------------------------------------------*
 
 This conditions the output buffer, ioBuffer, previously allocated by cmUseIOBuffer(), for
 initial writing of the value data for theValueHdr.  If theValueHdr is NULL, the container
 is written directly with the container's output handler.  This must be called once after
 cmUseIOBuffer() and before any other calls.
*/

void cmNewBufferedOutputData(void *ioBuffer, TOCValueHdrPtr theValueHdr)
{
	IOControlPtr io = (IOControlPtr)ioBuffer;
		
	io->bufferPosition = 0;														/* init buffer index for output			*/
	io->theValueHdr    = theValueHdr;									/* remember refNum (if any)					*/
	io->ioPos        	 = (theValueHdr != NULL) ? 0UL 	/* remember where we are						*/
																						 : (CM_ULONG)CMftell(io->container->updatingContainer);
}


/*----------------------------------------------------------*
 | cmWriteBufferedData - write 1, 2, or 4 bytes to a buffer |
 *----------------------------------------------------------*
 
 This is called to do buffered writes of 1, 2, or 4 byte data to a container. It is "glue"
 code in the sense that it is the interface to the "format data" handler.  That handler
 actually places the bytes in the buffer.  The buffer used is one returned from a previous
 call to cmUseIOBuffer() and passed as the ioBuffer parameter.
 
 A CM_UCHAR, CM_USHORT, or CM_ULONG is passed in data4 which the handler is expected to
 format into the buffer as 1, 2, or 4 byte quantities respectively.  This is done for
 portability reasons since the architecture we're running this code on might map
 differently into standard container entities.
 
 The expected output size, 1, 2, or 4, is passed, and the value to put into the buffer in
 data4 (always passed as a CM_ULONG).  The buffer is written as value data for the value
 "refNum" passed to cmNewBufferedOutputData() (theValueHdr passed to
 cmNewBufferedOutputData()).  If there is no value data (theValudHdr is NULL), then the
 container's output handler is called directly.  Thus two levels of I/O are available; a
 "high level" through a value refNum, and a "low level" directly using the handler.
 
If the output size is negative, use its absolutely value. This is use to denote
endian-ness netural data.
	
 This routine handles writing the buffer as it fills.  If an error is detected, a longjmp
 on the setjmp environment passed to cmUseIOBuffer() is taken.  It is that setjmp code
 which is responsible for reporting the error message.  This is done because the buffered
 I/O routines can be used in a number of contexts.  The caller can report a more
 appropriate error message rather than a more generic one we would have to report from
 here.

 Note, for direct handler writes, no seeks are done here.  It is assumed that the output
 is positioned to what is to be written.  If other code does seeks, then that code should
 call cmBufferedIOftell() to reseek to the proper output position.
*/

void cmWriteBufferedData(void *ioBuffer, CM_LONG size, CM_ULONG data4)
{
	IOControlPtr 	io				= (IOControlPtr)ioBuffer;
	ContainerPtr 	container = io->container;
	CM_UCHAR		 	data1;
	CM_USHORT		 	data2;
	CMPrivateData d;
	CM_LONG				realSize = size;
	
	if (realSize < 0)															/* use the absolute value								*/
		realSize = -realSize;
	
	/* As documented in the handlers, a pointer to the data to be placed in the buffer is	*/
	/* passed, and the handler can assume that pointer is pointing to a CM_UCHAR if size 	*/
	/* is 1, CM_USHORT if size is 2, and CM_ULONG if size if 4.  So, not to be liers, we 	*/
	/* set up a pointer to exactly that.																									*/
	
	if (realSize == 1) {													/* 1 ==> pointer to CM_UCHAR						*/
		data1 = (CM_UCHAR)data4;
		d = (CMPrivateData)&data1;
	} else if (realSize == 2) {										/* 2 ==> pointer to CM_USHORT						*/
		data2 = (CM_USHORT)data4;
		d = (CMPrivateData)&data2;
	} else																				/* 4 ==> pointer to CM_ULONG						*/
		d = (CMPrivateData)&data4;
	
	/* Now we can put the data into the buffer using the handler...												*/
	
	CMformatData(container, io->dataBuffer + io->bufferPosition, size, d);
	
	/* Bump the buffer offset. The max offset imposed on the buffer is actully at least 4	*/
	/* bytes less than the true maximum buffer size allocated because we had bumped up the*/
	/* allocation by 4 bytes.  This means that the next free byte	could be passed					*/ 
	/* maxBufSize without overruning the buffer.  At that point we write the buffer as 		*/
	/* (value) data and reset the offset.  By doing this we never have to worry about not */
	/* being able to write 2 or 4 bytes to the buffer.  We also document in the handler 	*/
	/* that we always guarantee enough space in the buffer. The artifical limit does that.*/
	
	io->bufferPosition += realSize;							/* update offset to the next free byte		*/
	
	/* If the buffer needs flushing, now's the time to do it...														*/
	
	if (io->bufferPosition >= io->maxBufSize)		/* flush if full 													*/
		cmFlushOutputBuffer(ioBuffer);
}


/*----------------------------------------------*
 | cmFlushOutputBuffer - flush an output buffer |
 *----------------------------------------------*
 
 This routine is called to make sure that an output buffer (associated with the ioBuffer)
 is flushed, i.e., fully written.  This completes the value data for the value specified
 to cmNewBufferedOutputData(), or simply writes the remaining partial buffer directly if
 the output handler is to be used directly (see cmNewBufferedOutputData() for further
 details).
 
 Note, internally, this routine is also called from cmWriteBufferedData() when the buffer
 fills.
*/

void cmFlushOutputBuffer(void *ioBuffer)
{
	IOControlPtr 	io = (IOControlPtr)ioBuffer;
	ContainerPtr 	container = io->container->updatingContainer;

	if (io->bufferPosition > 0) {												/* flush if somthing to flush			*/
		if (io->theValueHdr != NULL) {										/* high or low level I/O ?				*/
			CMWriteValueData((CMValue)io->theValueHdr, (CMPtr)io->dataBuffer,
											 (CMCount)io->theValueHdr->size, (CMSize)io->bufferPosition);
			io->ioPos += io->bufferPosition;								/* update output position					*/
		} else if (CMfwrite(container, io->dataBuffer, sizeof(CM_UCHAR), io->bufferPosition) == (CMSize)io->bufferPosition) {
			io->ioPos = (CM_ULONG)CMftell(container);	/* get "true" container position	*/
			if (io->ioPos > container->physicalEOF) 				/* make sure of EOF settings...		*/
				container->physicalEOF = io->ioPos;
			SetLogicalEOF(container->physicalEOF);
		} else																						/* take longjmp if output error		*/
			longjmp(io->bufferIOEnv.jmpBuf, 1);
				
		io->bufferPosition = 0;														/* the buffer is now empty				*/
	}
}


/*----------------------------------------------------------*
 | cmNewBufferedInputData - initialize for buffered reading |
 *----------------------------------------------------------*
 
 This conditions an input buffer, ioBuffer, previously allocated by cmUseIOBuffer(), for
 initial reading up to dataSize bytes from the value data for theValueHdr.  If theValueHdr
 is NULL, the container will be read directly with the container's input handler.  The
 first call to cmReadBufferedData() after this call will cause cmReadBufferedData() to
 reload its buffer with "new" data.  For each new value refNum, call this routine to read
 its data.  If there is no value refNum, call it once after cmUseIOBuffer().  Either way,
 call it before doing the first cmReadBufferedData() or cmBufferedIOftell().
*/

void cmNewBufferedInputData(void *ioBuffer, TOCValueHdrPtr theValueHdr,
														CM_ULONG dataSize)
{
	IOControlPtr io = (IOControlPtr)ioBuffer;
	
	io->bufferPosition   =  (io->maxBufSize);					/* pretend we're at end of buffer		*/
	io->bufferReadOffset = -(io->maxBufSize);					/* 1st refNum offset to will be 0		*/		
	io->theValueHdr   	 = theValueHdr;								/* remember refNum (if any)					*/
	io->dataSize				 = dataSize;									/* and max amount to read						*/
	io->ioPos          	 = (theValueHdr != NULL) ? 0UL/* remember where we are						*/
																						 : (CM_ULONG)CMftell(io->container->updatingContainer);
}


/*----------------------------------------------------------*
 | cmReadBufferedData - read 1, 2, or 4 bytes from a buffer |
 *----------------------------------------------------------*
 
 This is called to do buffered reads of data from the container.  It is "glue" code in the
 sense that it is the interface to the "extract data" handler.  That handler actually
 copies the bytes from the buffer.  The buffer used is one returned from a previous
 call to cmUseIOBuffer() and passed as the ioBuffer parameter.
 
 The handler is expected to extract 1, 2, or 4 byte quantities from the buffer and place
 them in a CM_UCHAR, CM_USHORT, or CM_ULONG respectively.  This is done for portability
 reasons since the architecture we're running this code on might map differently into 
 standard container entities.
 
 The buffer is read as value data for the passed value "refNum" passed to the most recent
 call to cmNewBufferedInputData().  The expected input size, 1, 2, or 4, is passed, and
 the value returned as the function result (always as a CM_ULONG).

 The expected input size, 1, 2, or 4, is passed, and the value returned as the function
 result (always as a CM_ULONG).  The buffer is read as value data for value "refNum" 
 passed to cmNewBufferedInputData() (theValueHdr passed to cmNewBufferedInputData()).  If
 there is no value data (theValueHdr is NULL), then the container's input handler is
 called directly.  Thus two levels of I/O are available; a "high level" through a value
 refNum, and a "low level" directly using the handler.
 
 If the input size is negative, use its absolutely value. This is use to denote
 endian-ness netural data.
	
 This routine handles reading the buffer as all its data is extracted.  If an error is
 detected, a longjmp on the setjmp environment passed to cmUseIOBuffer() is taken.  It is
 that setjmp code which is responsible for reporting the error message.  This is done
 because the buffered I/O routines can be used in a number of contexts.  The caller can
 report a more appropriate error message rather than a more generic one we would have to
 report from here.
 
 Note, the caller MUST call cmNewBufferedInputData() prior to the first call of
 cmReadBufferedData() to cause the buffer to be loaded the first time and to get the
 value refNum.  cmNewBufferedInputData() should be used any time a new value refNum is to
 be used to read new data. 
 
 Also note, for direct handler reads, no seeks are done here.  It is assumed that the
 input is positioned to what is to be read.  If other code does seeks, then that code
 should call cmBufferedIOftell() to reseek to the proper input position.
*/

CM_ULONG cmReadBufferedData(void *ioBuffer, CM_LONG size)
{
	IOControlPtr 	io				= (IOControlPtr)ioBuffer;
	ContainerPtr 	container = io->container;
	CM_UCHAR		 	data1;
	CM_USHORT		 	data2;
	CM_ULONG		 	data4;
	CMPrivateData d;
	CMSize			 	bufSize, amountRead;
	CM_LONG 			nextPos, remaining;
	CM_LONG				realSize = size;
	
	if (realSize < 0)															/* use the absolute value								*/
		realSize = -realSize;
	
	/* Compute the buffer offset of the NEXT byte we will read AFTER the 1, 2, or 4 bytes	*/
	/* we are about to read.  Knowing this tells us whether all the bytes we need are in 	*/
	/* buffer.  If they are not the buffer must be reloaded.															*/
	
	nextPos = io->bufferPosition + realSize;				/* next byte to read after this one		*/
	
	if (nextPos > io->maxBufSize) {									/* reload if all bytes not available	*/
		io->bufferReadOffset += io->bufferPosition;		/* next value offset									*/
		remaining = io->dataSize - io->bufferReadOffset;	/* data left to read							*/
		bufSize 	= (remaining > io->maxBufSize) ? (CMSize)io->maxBufSize : (CMSize)remaining;
		
		if (io->theValueHdr != NULL)
			amountRead = CMReadValueData((CMValue)io->theValueHdr, (CMPtr)io->dataBuffer,
																	 (CMCount)io->bufferReadOffset, bufSize);
		else {
			io->ioPos += io->bufferPosition;						/* we will reread unprocessed stuff		*/
			CMfseek(container->updatingContainer, io->ioPos, kCMSeekSet);
			amountRead = CMfread(container->updatingContainer, io->dataBuffer, sizeof(CM_UCHAR), bufSize);
		}
		
		if (amountRead != bufSize || amountRead == 0) {
			io->bufferReadOffset = io->dataSize; 				/* safety															*/
			longjmp(io->bufferIOEnv.jmpBuf, 2);
		}
		
		io->ioPos += amountRead;											/* update input position							*/
		io->bufferPosition = 0;												/* reset the buffer offset						*/
		nextPos = (CM_LONG)realSize;									/* reset the next offset after that		*/
	}
	
	/* As documented in the handlers, a pointer to the data to be extracted from the 			*/
	/* buffer is passed, and the handler can assume that pointer is pointing to a CM_UCHAR*/
	/* if size is 1, CM_USHORT if size is 2, and CM_ULONG if size if 4.  It is also 			*/
	/* documented that the handler need not worry that all the bytes are available in the	*/
	/* buffer.  We took care of that problem above.																				*/

	if (realSize == 1) 															/* 1 ==> pointer to CM_UCHAR					*/
		d = (CMPrivateData)&data1;	
	else if (realSize == 2)													/* 2 ==> pointer to CM_USHORT					*/
		d = (CMPrivateData)&data2;
	else																						/* 4 ==> pointer to CM_ULONG					*/
		d = (CMPrivateData)&data4;
	
	/* Now we can extract the data from the buffer using the handler...										*/
		
	CMextractData(container, io->dataBuffer + io->bufferPosition, size, d);
	
	/* The data is always returned as the function result as a CM_ULONG.  Thus, for size 	*/
	/* 1 and 2, we must up-convert the value...																						*/
	
	if (realSize == 1)
		data4 = (CM_ULONG)data1;
	else if (realSize == 2)
		data4 = (CM_ULONG)data2;
	
	/* Set the offset to the next byte to read in the buffer and return the value...			*/
	
	io->bufferPosition = nextPos;										/* offset to next byte to read				*/
	
	return (data4);																	/* give caller back data as a CM_ULONG*/
}


/*-------------------------------------------------*
 | cmBufferedIOftell - return current I/O position |
 *-------------------------------------------------*
 
 This returns the current I/O position from the time cmNewBufferedOutputData() or 
 cmNewBufferedInputData() was called.  For I/O to value data, the position is a value
 data offset (initially always 0).  For direct handler I/O, the position is a container
 offset.  This was set at the time cmNewBufferedOutputData() or cmNewBufferedInputData()
 was called.  The position is updated as buffers fill and are written or reloaded when
 reading.
 
 This routine is needed when only when there is the possibility that other code might be
 doing "seeks" to the same container behind the buffered I/O routine's back!  Code doing
 such seeks must be able to reseek the container position according to the value returned
 here.  For value refNums, this usually is not necessary, since everything is in terms of
 the value data offset rather than a container offset. But this routine, for completeness,
 will return the appropriate offset position the I/O wants to use.
*/

CM_ULONG cmBufferedIOftell(void *ioBuffer)
{
	return (((IOControlPtr)ioBuffer)->ioPos);
}

															CM_END_CFUNCTIONS
