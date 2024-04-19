/* @(#)Z 1.4 com/src/cm/BufferIO.h, odstorage, od96os2, odos29646d 96/11/15 15:26:40 (96/10/29 09:15:46) */
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
	File:		BufferIO.h

	Contains:	Container Manager Buffered Data I/O Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182308 Add new macro for format/extract
									without byte swapping.
		 <2>	  5/9/94	MB		#1162181 Changes necessary to install MMM.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<<  BufferIO.h  >>>                           |
 |                                                                           |
 |              Container Manager Buffered Data I/O Interfaces               |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  8/3/92                                   |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file defines the routines to allocate and free I/O buffers used for buffered reading
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
 these handlers, unless care is taken to properly protect the file position (seek) 
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
			
			cmNewBufferedInputData(ioBuffer, [valueRefNum], totalAmountToRead);
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
			cmNewBufferedOutputData(ioBuffer, [valueRefNum]);
			
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

#ifndef __BUFFEREDIO__
#define __BUFFEREDIO__

#include <setjmp.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif

struct Container;
struct TOCValueHdr;


																	CM_CFUNCTIONS

void *cmUseIOBuffer(struct Container *container, long maxBufferSize, jmp_buf *ioEnv);
	/*
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
	
	
void cmReleaseIOBuffer(void *ioBuffer);
	/*
	This is called to free a buffer, ioBuffer,  allocated by cmUseIOBuffer(). If the caller
	"forgets" to call this routine, then when the container is closed, cmFreeAllIOBuffers()
	will be called to make sure all the buffers are free for that container.
	*/
	

void cmFreeAllIOBuffers(struct Container *container);
	/*
  This is called at container close time to make sure all buffers allocated for that
  container are released (freed).
	*/


void cmNewBufferedOutputData(void *ioBuffer, struct TOCValueHdr *theValueHdr);
	/*
	This conditions the output buffer, ioBuffer, previously allocated by cmUseIOBuffer(), for
	initial writing of the value data for theValueHdr.  If theValueHdr is NULL, the container
	is written directly with the container's output handler.  This must be called once after
	cmUseIOBuffer() and before any other calls.
	*/
	

void cmWriteBufferedData(void *ioBuffer, CM_LONG size, CM_ULONG data4);
	/*
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


#define PUT1(data, io) 	cmWriteBufferedData(io, 1, (CM_ULONG)(data))
#define PUT2(data, io) 	cmWriteBufferedData(io, 2, (CM_ULONG)(data))
#define PUT4(data, io) 	cmWriteBufferedData(io, 4, (CM_ULONG)(data))
#define PUT4Direct(data, io) 	cmWriteBufferedData(io, -4, (CM_ULONG)(data))
	/*
	cmWriteBufferedData() is never explicitly called. Rather, these macros are provided to
	expand to cmWriteBufferedData() calls.
	*/
	
	
void cmFlushOutputBuffer(void *ioBuffer);
	/*
	This routine is called to make sure that an output buffer (associated with the ioBuffer)
	is flushed, i.e., fully written.  This completes the value data for the value specified
	to cmNewBufferedOutputData(), or simply writes the remaining partial buffer directly if
	the output handler is to be used directly (see cmNewBufferedOutputData() for further
	details).
	*/
	

void cmNewBufferedInputData(void *ioBuffer, struct TOCValueHdr *theValueHdr,
														CM_ULONG dataSize);
	/*
	This conditions an input buffer, ioBuffer, previously allocated by cmUseIOBuffer(), for
	initial reading up to dataSize bytes from the value data for theValueHdr.  If theValueHdr
	is NULL, the container will be read directly with the container's input handler.  The
	first call to cmReadBufferedData() after this call will cause cmReadBufferedData() to
	reload its buffer with "new" data.  For each new value refNum, call this routine to read
	its data.  If there is no value refNum, call it once after cmUseIOBuffer().  Either way,
	call it before doing the first cmReadBufferedData() or cmBufferedIOftell().
	*/

	
CM_ULONG cmReadBufferedData(void *ioBuffer, CM_LONG size);
	/*
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
	
	
#define GET1(io) ((CM_UCHAR)cmReadBufferedData(io,  1))
#define GET2(io) ((CM_USHORT)cmReadBufferedData(io, 2))
#define GET4(io) ((CM_ULONG)cmReadBufferedData(io,  4))
#define GET4Direct(io) ((CM_ULONG)cmReadBufferedData(io,  -4))
	/*
	cmReadBufferedData() is never explicitly called.  Rather, these macros are provided to
	expand to cmReadBufferedData() calls.
	*/
	
	
CM_ULONG cmBufferedIOftell(void *ioBuffer);
	/*
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
	the value data offset rather than a container offset.  But this routine, for completeness,
	will return the appropriate offset position the I/O wants to use.
	*/


														  CM_END_CFUNCTIONS
#endif
