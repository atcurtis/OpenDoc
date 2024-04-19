/* @(#)Z 1.6 com/src/cm/CMConfig.h, odstorage, od96os2, odos29646d 96/11/15 15:26:45 (96/10/29 09:16:25) */
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
	File:		CMConfig.h

	Contains:	Container Manager Configuration Controls

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1994-95 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>	 10/9/95	EL		1290781: flag to allow ignoring non-fatal
									error.
		 <6>	 4/25/95	EL		1242376: Separate crash proof for embedded
													and top level container.
		 <5>	 3/24/95	EL		1226127: add CMCRASH_PROOF.
		 <4>	 12/9/94	EL		#1182275 Add CMKEEP_CONTINUE_FLAG
		 <3>	 9/15/94	EL		#1182275 More comment on MinFragmentSize.
		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <3>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <2>	  2/3/94	EL		Bento File names are now eight chars or
									less.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<< CMConfig.h >>>                             |
 |                                                                           |
 |                 Container Manager Configuration Controls                  |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  8/4/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1995               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This header is used to defined various "configuration" attributes that control optional
 or alternative versions of various aspects of the Container Manager code.  Some constants
 are also defined here which may be a function of the environment in which the Container
 Manager is run.  Such constants are not in CMAPIEnv.h.h because these also may
 control the way code is generated and this header is not a published header as is the
 case for CMAPIEnv.h.h.
*/


#ifndef __CMCONFIG__
#define __CMCONFIG__


/*---------------------------------------------------------------*
 | For Lotus (support for "original format 1 TOC layout)...      |
 *---------------------------------------------------------------*
 
 The following macros are used to define call "old" code for support of format 1 TOC
 manipulations.  They are sufficiently "grotesque" to make them stand out.  That is because
 the support of the "old" TOC format is only there TEMPORARILY for Lotus and initial
 format size comparisons with the "new" and more compact TOC format.  To make it easier to
 find and remove the bracketed code, we make these macros stand out.
*/

#ifndef TOC1_SUPPORT
#define TOC1_SUPPORT 0					/* Must be explicitly requested as compiler option!			*/
#endif

#if TOC1_SUPPORT								/* function call TOC format 1 alternative with result		*/
#define USE_TOC_FORMAT_1_ALTERNATIVE(format1Proc, params)	\
	if (((ContainerPtr)container)->majorVersion == 1)				\
		return format1Proc##params
#else
#define USE_TOC_FORMAT_1_ALTERNATIVE(format1Proc, params)	
#endif

#if TOC1_SUPPORT								/* function call TOC format 1 alternative with no result*/
#define USE_TOC_FORMAT_1_ALTERNATIVE1(format1Proc, params)	\
	if (((ContainerPtr)container)->majorVersion == 1)	{				\
		format1Proc##params;																		\
		return;																									\
	}
#else
#define USE_TOC_FORMAT_1_ALTERNATIVE1(format1Proc, params)	
#endif


/*--------------------*
 | Buffering Controls |
 *--------------------*
 
 Some routines buffer their I/O.  The following define the buffer sizes used.  Where 
 indicated, some sizes may be defined as 0 to disable the explicit buffering.  This would
 usually be done if the the I/O is through handlers which do their own buffering so there
 would be n need to do additional buffering by the handler callers.
 
 Unless indicated, these buffers are only allocated during the time they are used. These 
 uses are generally at mutually exclusive times and thus the total space defined is not
 the total space used.
 
 The buffers being defined here are used as the buffer size parameter to  BufferIO.c 's
 cmUseIOBuffer() which allocate the buffers.  The buffering routines will take any
 (reasonable) size.  It does not have to be a multiple of something.  Although that 
 probably would not hurt!
*/
 

/* The reading and writing the TOC is buffered.  The following defines the buffer size	*/
/* used...																																							*/

#if TOC1_SUPPORT
#ifndef TOCInputBufSize
#define TOCInputBufSize (0)					/* This defines the size of the TOC input buffer. It*/
#endif															/* should be a "moderate" size (maybe about 50*24)	*/
																		/* chosen as a comprimize to wanting to read large 	*/
																		/* chunks of TOC and not wanting to read too far 		*/
																		/* into a "split" TOC which has a public and private*/
																		/*  section when the TOC is used for updating a 		*/
																		/* target container.																*/
																				
																		/* THIS CAN BE 0.  No explict TOC buffering will be */
																		/* done. It is then assumed that either no buffering*/
																		/* is wanted (why?), or that the handlers do the 		*/
																		/* buffering.  																			*/
																		
																		/* If the handlers are doing standard stream I/O, 	*/
																		/* then presumably a setvbuf() can be done to allow	*/
																		/* the stream I/O to do the buffering so none would	*/
																		/* be needed to be defined here.										*/
#endif

#ifndef UpdateBufSize
#define UpdateBufSize 		512				/* Max size of the updates buffer, i.e., the buffer	*/
#endif															/* used to write updating instructions into as value*/
																		/* data.  This can NEVER be 0.  Update instructions	*/
																		/* are always buffered because the handlers are not	*/
																		/* explicitly used as is the case for TOC I/O.			*/
																		
																		/* A "moderate" size should be chosen for this since*/
																		/* the buffer is reused for each object that has		*/
																		/* updates associated with it.  It come down to 		*/
																		/* guessing how much updating will be done to the		*/
																		/* values belonging to an object.										*/

#define DefaultTOCBufSize	1					/* Default TOC output buffer size in units of 1024	*/
																		/* bytes. A value of "n" means n*1024 bytes is used	*/
																		/* as the TOC output buffer size.  The TOC is 			*/
																		/* formatted in "blocks" of n*1024 and will never 	*/
																		/* span (cross) a block boundary.  Padding is added */
																		/* if necessary.  The n here is placed in the 			*/
																		/* container label and is limited to a maximum value*/
																		/* of 65535 (0xFFFF).																*/
																		
																		/* For reading, whatever "n" is found in the label	*/
																		/* is used to determine the input TOC buffer size.	*/
																		/* Thus, there can be no default for the input 			*/
																		/* buffer size.																			*/

#define RefsBufSize				(64*8)		/* Size of the input buffer used to search for 			*/
																		/* a value's references. See "Reference Shadow List */
																		/* Controls" for a brief description of references.	*/
																		/* The value data is formatted as 8-byte CMReference*/
																		/* key/object ID pairs (4 bytes each).  Almost any	*/
																		/* reasonable size will do here depending on your		*/
																		/* estimate on how heavily references are used.			*/

/*----------------------*
 | Performance Controls |
 *----------------------*

	Some parameters can be used to control the trade off between performance and disk
	space usage. For example if we allows a value to be separated into many value
	segments to use up every possible disk space, we save disk space but the resulting
	performance would suffer because of the fragmentation. So you want to adjust it
	to suit your own need.
 */

/* Very often, the same value is being written back. If we know it is unchanged we can	*/
/* avoid the write. We can do this by reading the data back and comparing with the new	*/
/* data. However, this reading and comparison introduces overhead. So we only want to		*/
/* do it if the data size is small. kSizeReadBeforeWrite is the data size limit that we	*/
/* would attempt a read before write.																										*/

#ifndef SizeReadBeforeWrite
#define SizeReadBeforeWrite		32
#endif														

/* When we are getting space from the free space, we try not to break it up into very		*/
/* small segment. MinFragmentSize is the smallest block we try to get unless we are			*/
/* asking for less ourselves.																														*/
#ifndef MinFragmentSize
#define MinFragmentSize				128
#endif														

/* For large blocks, we try not to have more than MaxFragmentCount segments.						*/
/* In reality, we end up with more segments than this. The reason is that if we do not	*/
/* get all the space we want, we would ask again. However the second time we ask we 		*/
/* are asking for the remainder, which is smaller. So if kMaxFragmentCount is 4 and we	*/
/* initially ask for 4K, we may not end up with 4 1K segment. Rather we may get back		*/
/* blocks of 1K, 0.75K, 0.56K, 0.42K, 0.32K etc.																				*/

/* We also use this constant to trigger when we need to try to consolidate segments			*/
/* Of course this does not means that we can cut it down to this number. 								*/
#ifndef MaxFragmentCount
#define MaxFragmentCount			4
#endif														

/*--------------------*
 | Debugging Controls |
 *--------------------*
 
 The following two switches control whether the indicated debugging code should be 
 generated. The first controls the generation of CMDebugging() while the second controls
 the generation of CMDumpTOCStructures() and CMDumpContainerTOC().  These two routines have
 the following actions:
 
 CMDebugging()					- to set some internal debugging "options"
 CMDumpTOCStructures()	- to dump in-memory TOC as a tree-like format
 CMDumpContainerTOC()		- to read in container TOC and display it in a table format
*/

#ifndef CMDEBUGGING
#define CMDEBUGGING 0			/* 1 ==> define CMDebugging()																	*/
#endif

#ifndef CMDUMPTOC
#define CMDUMPTOC 0				/* 1 ==> define CMDumpTOCStructures() and CMDumpContainerTOC()*/
#endif


/*------------------------------------*
 | Validation and Protection Controls |
 *------------------------------------*

 The following two switches control the validation checking code in the Container Manager.
 Validations include checking that CMStartSession() was called and for NULL refNums. There
 is a dynamic switch controlled by CMDebugging() (the switch is in the session global data)
 and a static (compile time) switch to suppress the validation checks entirely.
 
 Remember, suppressing the validation checks removes what little protection the Container
 Manager has against careless users.
*/

#ifndef CMVALIDATE
#define CMVALIDATE 1						/* 1 ==> generate validation code; 0 ==> don't generate */
#endif

#ifndef CMDEFAULT_VALIDATE
#define CMDEFAULT_VALIDATE 1		/* default (initial) dynamic validation switch setting	*/
#endif


/*--------------------------------*
 | Reference Shadow List Controls |
 *--------------------------------*

 Object references are "pointers" (i.e., object IDs) to other objects from a value 
 (CMValue) that contains data that refers to those objects.  The data is in the form of a
 CMReference "key" and associated object ID.  The routines in    CMRefOps.c    manipulate
 those key/object (ID) associations.  The associations are maintained in a list as value
 data for a uniquely typed value in a private object "tied" to the value through a pointer
 in the value header.

 To increase efficiency, the data is read into memory the first time it's accessed.  From
 that point on, all updates to the actual data are "shadowed" in the in-memory list.  The
 updating can't be avoided (so updating, e.g., touches, etc. work correctly).  But at least
 the searches can be made to not be I/O bound!  This shadow list support is OPTIONAL and
 controlled by the macro variable defined below.  If the variable is defined as 0, no
 shadow list support is provided.  All search operations for a reference will take place
 directly on the value data.  This has the benefit of not requiring additional memory 
 space.  It also could potentially be more efficient depending on the supporting I/O
 handlers.  These are the reasons the shadow list support is optional.
*/

#ifndef CMSHADOW_LIST
#define CMSHADOW_LIST	0					/* 1 ==> support reference shadow list									*/
#endif

/*-----------------------*
 | Miscelaneous Controls |
 *-----------------------*

*/

/* If there are multiple value segments, there is a continous flag in all the values
	except the last one as well as value header. A lot of work is needed to maintain
	the flags to make sure that they are correct as segments are being added and
	deleted. The flags are used at close time when the TOC is being written out.
	If we do not use the flags until close time, we may skip the maintenance of
	the flags. At close time we can reconstruct the flag by looking at the value
	list.
*/

#ifndef CMKEEP_CONTINUE_FLAG
#define CMKEEP_CONTINUE_FLAG	0			/* 1 ==> maintain the continue flag in values				*/
#endif

/* If there are multiple value segments, there is a continous flag in all the values
	except the last one as well as value header. A lot of work is needed to maintain
	the flags to make sure that they are correct as segments are being added and
	deleted. The flags are used at close time when the TOC is being written out.
	If we do not use the flags until close time, we may skip the maintenance of
	the flags. At close time we can reconstruct the flag by looking at the value
	list.
*/
#ifndef CMIGNORE_NONFATAL
#define CMIGNORE_NONFATAL	1					/* 1 ==> ignore non-fatal errors				*/
#endif

/* If an error is non-fatal, this flag allows the program to ignore the error and
	continue instead of calling the error handling routine.
*/

#ifndef CMTOPLEVEL_CRASH_PROOF
#define CMTOPLEVEL_CRASH_PROOF	1				/* 1 ==> make sure don't write over old content				*/
#endif

#ifndef CMEMBEDDED_CRASH_PROOF
#define CMEMBEDDED_CRASH_PROOF	0				/* 1 ==> make sure don't write over old content				*/
#endif


#endif
