/* @(#)Z 1.4 com/src/cm/TOCIO.h, odstorage, od96os2, odos29646d 96/11/15 15:27:26 (96/10/29 09:19:44) */
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
	File:		TOCIO.h

	Contains:	Container Manager TOC I/O Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182275 extra parameter in cmWriteTOC to
									truncate free space.
		 <4>	  2/3/94	EL		Bento File names are now eight chars or
													less.
		 <3>	11/23/93	EL		When updating save TOC data so that TOC
													don't need to be read twice.
		 <2>	10/21/93	EL		In cmReadTOC, allow restriction of kind of
													entry read.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                              <<< TOCIO.h >>>                              |
 |                                                                           |
 |                   Container Manager TOC I/O Interfaces                    |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 10/01/92                                  |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file defines the routines responsible for reading and writing the container TOC. 
 The routines are divided into two main groups as follows:
 
 (1). "Raw" low-level TOC I/O: all the routines to read and write TOC basic value segment
 		  entries.  These routines format the TOC with the syntatic layout described later in
			this header.

 		  cmStartTOCIO						-	initiate TOC I/O
			cmEndTOCIO							- end TOC I/O
			cmFtellTOC							- get current container TOC I/O "seek" position
			cmFlushTOCOutputBuffer	- write current TOC output buffer
			cmWrite1TOCSegment			- write one value segment
			cmRead1TOCSegment				- read one value segment
				
 (2). High-level TOC I/O control that determines how to generate or process TOC entries.
			This calles the group (1) routines.
			
			cmBuildGlobalNameTable	- add a global name to the global name table
			cmReadTOC								- read (laod) in a container TOC
			cmWriteTOC							- write a TOC to the container
			cmDoBackPatches					- back patch certain TOC entries
*/

#ifndef __TOCIO__
#define __TOCIO__

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif

																	    CM_CFUNCTIONS

#if TOC1_SUPPORT
												/*-------------------------------------*
												 | Defined "Old" Layout of a TOC Entry |
												 *-------------------------------------*
 
As defined by the original API documentation, a container TOC entry has the following
layout:
 
      0           4           8           12          16          20    22    24
		 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		 | Object ID |Property ID|  Type ID  |Valu Offset| Value Len | Gen |Flags|
		 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		       4           4           4           4           4        2     2

These are defined as "hard-coded" constants since the format is, by definition, platform
independent.  The actual formatting and I/O for a TOC entry is done by a handler because
of this.																																								*/

#define TOCentryObjectID 		 0				/* 4 bytes: the object's ID												*/
#define TOCentryPropertyID 	 4				/* 4        its property ID												*/
#define TOCentryTypeID 			 8				/* 4        its type ID														*/
#define TOCentryValueOffset 12				/* 4        offset to its value										*/
#define TOCentryValueLength	16				/* 4        size of the value											*/
#define TOCentryGeneration	20				/* 2        generation number											*/
#define TOCentryFlags				22				/* 2        flags																	*/
					 
#define TOCentrySize 				24				/* defined size of a TOC entry in its container		*/

/* The following macros allow portable (I hope) access to a TOC's entry fields.  There 	*/
/* are two sets of macros; one set for input, and one for output.  Each set consists of	*/
/* two definitions; one when using buffered I/O and the other when calling a handler		*/
/* directly.  The code is configurable (CMConfig.h) to do either.  But in some cases		*/
/* we ignore the configuration, so all definitions must be defined here.  The goal here */
/* is to define in THIS ONE PLACE all the dependencies on the actual TOC format.  If it	*/
/* changes, then theoritically all we need to do is change this stuff here.  Cross your	*/
/* fingers!																																							*/

/* The first pair of macros are for extracting the TOC fields.  In the buffered case the*/
/* fields are extracted directly out of the buffer.  In the nonbuffered case, the buffer*/
/* is supplied to the macro to extract the fields.																			*/

#define BufferedExtractTOC(ioBuffer, objectID, propertyID, typeID, value, valueLen, generation, flags) \
					{objectID	  = (CM_ULONG)GET4(ioBuffer);	\
					 propertyID = (CM_ULONG)GET4(ioBuffer);	\
					 typeID		  = (CM_ULONG)GET4(ioBuffer);	\
					 value		  = (CM_ULONG)GET4(ioBuffer);	\
					 valueLen 	= (CM_ULONG)GET4(ioBuffer);	\
					 generation = (CM_USHORT)GET2(ioBuffer);	\
					 flags			= (CM_USHORT)GET2(ioBuffer);}

#define ExtractTOC(tocBuffer, objectID, propertyID, typeID, value, valueLen, generation, flags) \
					{CMextractData(container, (char *)tocBuffer + TOCentryObjectID, 	 4, &objectID);			\
					 CMextractData(container, (char *)tocBuffer + TOCentryPropertyID,  4, &propertyID);		\
					 CMextractData(container, (char *)tocBuffer + TOCentryTypeID, 		 4, &typeID);				\
					 CMextractData(container, (char *)tocBuffer + TOCentryValueOffset, 4, &value);				\
					 CMextractData(container, (char *)tocBuffer + TOCentryValueLength, 4, &valueLen);			\
					 CMextractData(container, (char *)tocBuffer + TOCentryGeneration,  2, &generation);		\
					 CMextractData(container, (char *)tocBuffer + TOCentryFlags, 		   2, &flags);}
#endif

												/*-------------------------------------*
												 | Defined "New" Layout of a TOC Entry |
												 *-------------------------------------*

The TOC in the container has the following syntatic layout:

								                 --                        --   
								    	           |  ( PT [g] [R] v... )     |   
						  { OPT [g] [R] v... |  <                 > ... | } ...
								    	           |  ( T  [g] [R] v... )     |   
								                 --                        --   
													
where OPT = object entry (NewObject code plus 4-byte object, property, and type ID)
			PT  = property entry (NewProperty code plus 4-byte property and type ID)
			T   = type entry (newType plus 4-byte type ID)
			g   = optional generation number if different from previous generation number
		  R	  = object ID of value's references recording object (if any)
			v   = value entry (code plus offset/length or 4-byte immediate) -- code determines
						4 or 8 byte offsets and actual size of immediate
						
In other words, nothing is repeated if it doesn't change.  The 4-byte data entries are
preceded by byte codes defined as follows:																							
																																							  Bytes		*/
/*      Code											 			 Meaning														   Consumed	*/
/*======================================================================================*/
#define NewObject		 			 1U					/* object + property + type									12 		*/
#define NewProperty	 			 2U					/* property + type													 8 		*/
#define NewType			 			 3U					/* type 																		 4 		*/
#define ExplicitGen				 4U					/* generation																 4 		*/
#define Offset4Len4	 			 5U					/* 4-byte offset + 4-byte length						 8 		*/
#define ContdOffset4Len4	 6U					/* continued 4-byte offset + 4-byte length	 8 		*/
#define Offset8Len4	 			 7U					/* 8-byte offset + 4-byte length						12 		*/
#define ContdOffset8Len4	 8U					/* continued 8-byte offset + 4-byte length	12 		*/
#define Immediate0				 9U					/* immediate (length = 0)										 0 		*/
#define Immediate1				10U					/* immediate (length = 1)										 4 		*/
#define Immediate2				11U					/* immediate (length = 2)										 4 		*/
#define Immediate3				12U					/* immediate (length = 3)									 	 4 		*/
#define Immediate4				13U					/* immediate (length = 4)									 	 4 		*/
#define ContdImmediate4		14U					/* continued immediate (length = 4)					 4 		*/
#define ReferenceListID		15U					/* references recording object ID						 4		*/
#define Reserved4b				16U					/*     "     "     "    "  "  "   "    "		 4		*/
#define Reserved4c				17U					/*     "     "     "    "  "  "   "    "		 4		*/
#define Reserved8a				18U					/* reserved for future use of 8 byte entry	 8		*/
#define Reserved8b				19U					/*     "     "     "    "  "  "   "    "		 8		*/
#define Reserved8c				20U					/*     "     "     "    "  "  "   "    "		 8		*/
#define Reserved12a				21U					/* reserved for future use of 12 byte entry	12		*/
#define Reserved12b				22U					/*     "     "     "    "  "   "   "    "		12		*/
#define Reserved12c				23U					/*     "     "     "    "  "   "   "    "		12		*/
#define EndOfBufr					24U					/* end of current buffer, go to next 				 0		*/
/*======================================================================================*/

/* The following define special codes and values related to the above TOC codes:				*/

#define FirstCode		 			1U		/* lowest code used (see notes below)										*/
#define LastCode				 24U		/* highest code used (see notes below)									*/

#define NOP						 0xFFU		/* NOP or filler 																				*/
#define TOCEOF				 0x00U		/* EOF signal code only returned for TOC input					*/
																																												/*
Notes: 1. The TOCEOF code is NOT a code placed in the TOC.  It is used only reading the
					TOC to signal that all TOC bytes have been read.
			 
			 2. The codes form a dense set starting at 1 (FirstCode) and extend up to LastCode.
			 	  This is done to allow the definition arrays associated with the codes using the
					following typedef:																														*/
					
				  typedef unsigned char TOCCodeArray[LastCode+1];
			 																																									/*
			 3. One use of the codes and the code array is to define the size of the data 
			 		associated with each code.  To keep the size info in proximity with the above
					definitions (to make it easier for changing), the following macro is defined to
					init an entry size array to the proper size for each code:										*/

					#define InitCodeSizes(entrySizeArray)	entrySizeArray[NewObject       ]	=	12;	\
																								entrySizeArray[NewProperty     ]	=	 8;	\
																								entrySizeArray[NewType         ]	=	 4;	\
																								entrySizeArray[ExplicitGen     ]	=	 4;	\
																								entrySizeArray[Offset4Len4     ]	=	 8;	\
																								entrySizeArray[ContdOffset4Len4]	=	 8;	\
																								entrySizeArray[Offset8Len4     ]	=	12;	\
																								entrySizeArray[ContdOffset8Len4]	=	12;	\
																								entrySizeArray[Immediate0      ]	=	 0;	\
																								entrySizeArray[Immediate1      ]	=	 4;	\
																								entrySizeArray[Immediate2      ]	=	 4;	\
																								entrySizeArray[Immediate3      ]	=	 4;	\
																								entrySizeArray[Immediate4      ]	=	 4;	\
																								entrySizeArray[ContdImmediate4 ]	=	 4;	\
																								entrySizeArray[ReferenceListID ]	=	 4;	\
																								entrySizeArray[Reserved4b      ]	=	 4;	\
																								entrySizeArray[Reserved4c      ]	=	 4;	\
																								entrySizeArray[Reserved8a      ]	=	 8;	\
																								entrySizeArray[Reserved8b      ]	=	 8;	\
																								entrySizeArray[Reserved8c      ]	=	 8;	\
																								entrySizeArray[Reserved12a     ]	=	12;	\
																								entrySizeArray[Reserved12b     ]	=	12;	\
																								entrySizeArray[Reserved12c     ]	=	12;	\
																								entrySizeArray[EndOfBufr	     ]	=	 0
																																												/*
			 4. All data entry sizes are a multiple of 4.  Thus data values are passed around
			 		in (unsigned) long variables.  Therefore, in terms of long variables, a maximum
					of 12/4 longs would be needed to hold the largest value according to the above
					sizes.																																				*/

					#define MaxEntrySize (12/4)						/* max nbr of longs to hold entry data	*/
																																												/*
			 5. In general, ignoring immediates, a value segment takes a minimum of 8 bytes.	
			 	  Thus, when maintaining the free list, we should not add anything to that list
					if it is less than 8 plus 1 for the entry code.																*/
					
					#define MinTOCSize (1+8)							/* assumed min TOC entry size						*/

/* The following defines all the information associated with a single TOC value data 		*/
/* segment.  It is clustered as a struct to make it easier to "carry around". 					*/

struct TOCentry {											/* Place to put a TOC entry during its write:			*/
	CM_ULONG  		 objectID;						/* 		the object's ID															*/
	CM_ULONG		   propertyID;					/* 		an object's property ID											*/
	CM_ULONG		   typeID;							/* 		an object's type ID													*/
	TOCValueBytes  value;								/* 		value and length or immediate value					*/
	CM_ULONG		   generation;					/* 		generation number														*/
	CM_USHORT			 flags;								/* 		object flags																*/
};
typedef struct TOCentry TOCentry, *TOCentryPtr;

struct TOCEntryListItem {							/* So that we can put TOCEntry in a list					*/
	ListLinks				tocEntryLinks;			/* 	links to nex/prev toc entry										*/
	TOCentry				tocEntryItem;				/* 	the toc entry																	*/
	CM_ULONG		  	refsDataObjectID;		/* 	reference object id														*/
};
typedef struct TOCEntryListItem TOCEntryListItem, *TOCEntryListItemPtr;

enum TocEntryType {									/* Select what TOC entries to read:	*/
	TocEntry_All,												/*		read everything															*/
	TocEntry_Update,										/*		only the update entries 										*/
	TocEntry_NewEntry										/*		the new additions to the TOC								*/
};
typedef enum TocEntryType TocEntryType;

/* In some cases, when the TOC is written, it is necessary to backpatch certain entries	*/
/* because we don't know the information when the entry is initially written.  The 			*/
/* following defines "BackPatches" area used to remember what has to be back patched and*/
/* the corresponding container offsets that will get the back patch.  These will always	*/
/*( be offsets to the value of the value/length fields in the TOC.											*/

struct BackPatches {									/* All the info on all possible patches:					*/
	CM_USHORT			 whatWeGot;						/*		flags indicating upatched entries we got		*/
	CM_ULONG			 tocSizeEntry;				/*		offset to TOC #1 size value									*/
	CM_ULONG			 tocContEntry;				/*		offset to TOC #1 container value						*/
	CM_ULONG			 tocNewValuesTOCEntry;/*		offset to TOC #1 new values TOC value				*/
};
typedef struct BackPatches BackPatches, *BackPatchesPtr;

#define Patch_TOCsize 		 		0x0001U	/* indicates tocSizeEntry created									*/
#define Patch_TOCContSize  		0x0002U	/* indicates tocContEntry created									*/
#define Patch_TOCNewValuesTOC 0x0004U	/* indicates tocNewValuesTOCEntry created					*/

#define InitBackPatches(patches) (((BackPatchesPtr)(patches))->whatWeGot = 0x0000U)
	/*
	This macro is used to init the back-patches data prior to writing TOC(s).  The macro 
	takes a pointer to a BackPatches struct.
	*/


void *cmStartTOCIO(ContainerPtr container, CM_ULONG tocBufSize, jmp_buf *ioEnv,
									 CM_ULONG tocOffset, CM_ULONG tocSize);
	/*
	This must be called prior to any reading or writing of the TOC.  The basic standard
	protocol to be followed is as follows:
	
	 if (setjmp(env)) {
		 <caller's recovery and exit>
	 }
	 
	 CMseek(to proper TOC position in the container);
	 
	 t = cmStartTOCIO(container, container->tocBufSize, (jmp_buf *)&env, tocOffset, tocSize);
	 
							when reading                  or          when writing
							------------                              ------------
	 
			 while (cmRead1TOCSegment(t, ...)) {   |   some kind loop through the TOC
				 cmRead1TOCSegment(t, ...);          |   {
				 <process TOC entry>                 |			 <set up TOC entry>
			 }                                     |      cmWrite1TOCSegment(t, ...);
																						 |   }
																						 |   cmFlushTOCOutputBuffer(t, true);
	 cmEndTOCIO(t);
	
	The TOC to be read or written is assumed to start at the specified tocOffset. It is the
	caller's responsibility to seek to this position.  For reading a TOC, tocSize defines the
	EXACT amount of TOC to be read.  For writing, tocSize must be passed as 0. The final size
	will be returned when endTOCIO() is called to terminate TOC creation (alternatively,
	cmFlushTOCOutputBuffer() will return the final size).
	 
	cmStartTOCIO() allocates an I/O buffer and its associated control information. The buffer
	is used to format or extract TOC entries.  The TOC I/O control block pointer allocated is
	returned as the function result as an anonymous "void *" pointer.  This is passed to all
	the other low level TOC externalized buffering routines (cmWrite1TOCSegment(),
	cmread1TOCSegment(), cmFlushTOCOutputBuffer(), cmFTellTOC(), and endTOCIO()).  An error
	is reported for allocation errors.  If the error reporter returns, NULL is returned as
	the function result.
	
	The maximum size of the buffer to be allocated is passed.  This will generally be
	determined from the container label, but that is not assumed here.  The size is, however,
	always ensured to be rounded up to a multiple of 4.
	
	A set setjmp/longjmp environment variable is also passed for read and write error
	reporting and recovery.  The setjmp/longjmp is used rather than have to check for errors
	on each I/O call of the buffered I/O routines.  If the longjmp is taken, the caller can
	assume that the appropriate error has been reported and the buffer and its control block
	freed.
	 
	Note, in the case of updating, the private TOC and the non-private (updating) TOC are
	viewed as two INDEPENDENT TOCs.  There is no need to worry that the non-private (i.e.,
	second TOC) does not start on a buffer boundary with respect to the private (first) TOC.
	Each is read or written independently and viewed as distinct from one another.  They just
	happen to be adjacent to one another in the container.  The tocBufSize and tocOffset
	must be appropriately passed for each TOC.
	
	Also note, that the reason some of the low level buffering routines have been made
	external (cmWrite1TOCSegment(), cmread1TOCSegment(), cmFlushTOCOutputBuffer(), 
	cmFTellTOC(), and endTOCIO()) is to mainly allow for the debugging trace routines to read
	the TOC.  cmWrite1TOCSegment() and cmFlushTOCOutputBuffer() are external only for
	symmetry, but it should have no external callers (i.e., they're all in this file).  
	Making these routines externally visable is also why the I/O control block pointer is
	returned as an anonymous "void *".  We may have to make the routines visable outside of
	here, but no one outside has any business knowing what's in the control block!
	*/
	

CM_ULONG cmEndTOCIO(void *tocIOCtl);
	/*
	After all I/O to a TOC is completed, this routine must be called to free the TOC I/O
	buffer and control block (whose pointer is passed as the t parameter).  If the TOC was
	created (written), then it is assumed the last buffer was already flushed by calling
	flushTOCOutputBuffer() with its finalFlush parameter set to true. 
	
	The function returns the size of the TOC.  For TOC creation (writing), this will be the
	size of the TOC now written to the container.  For reading, the returned value is 
	identical to the size passed to cmStartTOCIO() (i.e., the returned value is not looked at
	for reading).
	
	Note, this routine must NOT be called if an error was reported and the longjmp taken 
	through the setjmp environment buffer passed to cmStartTOCIO()>  Part of standard error
	recovery for TOC I/O errors is effectively do a cmEndTOCIO() and then report the error.
	If the error reporter returns, the longjmp is taken.
	*/


CM_ULONG cmFtellTOC(void *tocIOCtl);
	/*
	This returns the current container I/O position.  The position is updated as the TOC I/O
	buffer fills and is written or reloaded when reading.
	
	This routine is needed when only when there is the possibility that other code might be
	doing "seeks" to the same container behind the TOC I/O routine's back!  Code doing such
	seeks must be able to reseek the container position according to the value returned here.
	*/


CM_ULONG cmFlushTOCOutputBuffer(void *tocIOCtl, CMBoolean finalFlush);
	/*
	This must be called to make sure that the TOC output buffer (allocated by cmStartTOCIO()
	and associated with tocIOCtl) is flushed, i.e., fully written.  This buffer will be the
	remaining partial buffer when finalFlush is true, and a full buffer otherwise.  The 
	buffer is padded if necessary up to the full buffer size (defined by cmStartTOCIO()) or
	to the nearest multiple of 4 when finalFlush is true.  finalFlush would, of course, only
	be true prior to calling cmEndTOCIO() to write the LAST buffer out as a "short" buffer.
	
	Padding consists of a EndOfBufr byte followed by 0 or more NOP bytes.  Padding is not
	necessary, of course, if the buffer is exactly full.
 
  The function returns the current total size of the TOC including the flushed buffer. This
  generally has no meaning until finalFlush is true, at which time the returned amount will
  be the total size of the TOC written to the container. 
	*/


CM_ULONG cmWrite1TOCSegment(void *tocIOCtl,
														CM_ULONG  objectID, 
														CM_ULONG  propertyID, 
														CM_ULONG  typeID,
														CM_ULONG  value,
														CM_ULONG  valueLen,
														CM_ULONG  generation,
														CM_USHORT flags,
														TOCObjectPtr 	refDataObject);
	/*
	All TOC value segment writing is done with this routine.  This routine interfaces the
	low-level TOC entry buffering routines (whose I/O control block is associated with
	tocIOCtl and allocated by cmStartTOCIO()) with the upper-level writers.  The upper level
	(i.e., the caller) views all value segments in terms of the fields shown as this
	routine's parameters.  Here that information is selectively passed to the lower-level
	buffering routines to generate a TOC in the container with the syntactic layout described
	above.  
	
	Since this syntax describes a TOC layout where nothing is repeated if it doesn't change,
	this routine remembers and controls what goes out and when.  Note that, at a minimum, a
	data value (v) is always generated.
 
  Note the refDataObject parameter represents the "R" in the syntax.  refDataObject is a
  pointer to a value's private reference recording object ID.  This is the value's private
  object used to hold the list of CMReference "key"/object ID associations as value data.
  If refDataObject is NULL no "R" is generated.  If not NULL we generate the ID after the
  OPT, PT, or T.  
 
	The function returns the container offset to the value entry entry data (v).  This can
	be remembered by the higher-level caller for possible back patching later.
	
	Note, no consistency checking is done here. It is assumed all error checks have been done
	by the caller, and that by the time this routine is called, it is EXPECTED that a TOC
	entry will be created.  I/O errors can, of course, occur.  They use the setjmp/longjmp
	mechanism defined by cmStartTOCIO().
	*/
	

CMBoolean cmRead1TOCSegment(void *tocIOCtl,
														TOCentry  *tocEntry, 
														CM_ULONG  *refsDataObjectID);
	/*
	All TOC value segment reading is done with this routine.  This routine interfaces the
	low-level TOC entry buffering routines (whose I/O control block is associated with
	tocIOCtl and allocated by cmStartTOCIO()) with the upper-level writers.  The upper level
	(caller) views all value segments in terms of the fields shown as this routine's
	parameters.  
	
	The container TOC has the syntactic layout described above.  Since this syntax describes
	a TOC layout where nothing is repeated if it doesn't change, this routine remembers the
	unrepeated information are repeats it back to the caller.
 
	The function always returns true until an "eof" is detected, i.e., until all the bytes, 
	whose size was specified to cmStartTOCIO(), are read.  False is returned for the "eof".
 
	Note, refsDataObjectID represents a value's private reference recording object ID.  This
	is the value's private object used to hold the list of CMReference "key"/object ID
	associations as value data.  0x00000000UL is returned in refsDataObjectID if there is no
	recording object and, of course, will be the object ID if it is.
	*/


CMBoolean cmBuildGlobalNameTable(TOCValuePtr theValue);
	/*
	 This routine creates a global name symbol table entry for each value that has a property
	 ID that indicates the value is for a global name.
	 
	 When a TOC is read in from a container we must build up our memory structures exactly as
	 they were when the container was previously written.  That means that, in addition to the
	 TOC data structures, we have to build the global name symbol table containing the global
	 name strings for those values. Thus is routine is ONLY called during read-in (and only
	 from cmAppendValue()).  At that time the switch tocFullyReadIn is false, which is a status
	 switch indicating the TOC read is not yet complete.
	 
	 A word of caution!  Remember that this routine is only called during read in of a TOC. But
	 here we must read in a global name.  That will CHANGE the current "seek" position of the
	 next read.  cmReadTOC(), which is the routine that does the TOC reading, always keeps its
	 current TOC offset (the one just read) in tocInputOffset in the container.  We use that to
	 re-seek so that cmReadTOC() is none the wiser.
	*/
	
	
CMBoolean cmReadTOC(ContainerPtr container, TocEntryType whichKind, 
										CM_ULONG tocOffset, CM_ULONG tocSize,
										ListHdr *newEntryList);
	/*
	This routine is called to read in a TOC from a container which has been opened for input
	by a call to CMOpenContainer().  The container, offset to TOC, and the size of the TOC
	are passed.  The function returns true if successfully loaded and false otherwise (this
	is a safety, since error reports should never return from the error handler).
	
	Note, the entire TOC whose size is defined by tocSize will be read.  It is an error if
	not all the bytes are read.  This means that it is expected that tocSize be the EXACT 
	amount to read from tocOffset.  This is emphasized because, for updating, two TOCs must
	be read; the private TOC, and the non-private (updating) TOC.  The private offset and
	size are defined in the container label (by definition).  For non-updating containers,
	this will reflect the entire (only) TOC.  The non-private (updating) portion is defined
	by a TOC #1 property.
	
	WhichKind controls what kind of entry will be read. This is because we need to read in the
	update entry first because they may contains deletion, then we read in the rest, otherwise the
	newly read entries may be deleted.
	
	When whickKind is TocEntry_Update, pass in an initialized but empty list so that the
	newEntry may be collected. Later when cmReadTOC is called again for the new entries,
	pass in the same newEntryList to avoid reading the TOC again fro the new entries.
	*/


CMBoolean cmWriteTOC(ContainerPtr container, void *toc, CMObjectID startingID,
									 	CMObjectID endingID, BackPatchesPtr thePatches,
									 	CM_ULONG *tocStart, CM_ULONG *tocSize,
									 	CMBoolean truncFreeSpace);
	/*
	This routine is the inverse to cmReadTOC() to write the in-memory TOC to a container. The
	TOC is written to the end of the container.  Only objects with ID's greater than or equal
	to the startingID and less than or equal to the endingID are written.  Further, only
	objects whose value headers are equal to the specified container are written.
	
	With the restriction on the container, we will only write to the TOC newly created values
	for this open session.  If we're not updating, only one container so this is not an
	issue.  But for updating, only looking at the stuff in the updating container will yield
	all new objects and all new properties for "old" objects.  We can split these two groups
	using the ID limits.
	
	Note, non-TOC updates are generated by a separate walk not done here.  See Updating.c.
	
	For updating, a container may have its own TOC and the target TOC.  Thus the TOC pointer
	is an explicit parameter.  The container offset is returned in tocStart and the size of
	the TOC in tocSize.  The function returns true to indicate success and false otherwise
	(as a safety).
	
	There are some TOC entries that must be back-patched. This is because they are a function
	of the final TOC size in the container.  We don't know that until the entire TOC is
	written.  At that time we can rewrite the entries with the proper values filled in.
	
	Because of updating, there may be multiple calls to cmWriteTOC() to write various subsets
	of the TOC (i.e., the updates).  Thus we may not know the final TOC size when we return
	from here.  The back-patching cannot be done.  Only the caller knows when all TOC writes
	are complete and the back-patching can be done.
	
	To allow this to happen, thePatches is passed as a pointer to a BackPatches struct where
	we save the original unpatched TOC info and offset for the entries of interest.  As these
	entries are encountered (and they must be sometime) during TOC writing, we save them and
	their offsets in the struct. When the caller is ready to rewrite them cmDoBackPathes() is
	called to do it.
	
	If thePatches is passed as NULL, nothing (obviously) is saved for back-patching.
	*/


CMBoolean cmDoBackPatches(ContainerPtr container, BackPatchesPtr thePatches,
											  	CM_ULONG tocStart, CM_ULONG tocSize,
													CM_ULONG newValuesTOCStart, CM_ULONG newValuesTOCSize);
	/*
	When a TOC is written (e.g., cmWriteTOC()), there are some entries that are dependent on
	the final size of the TOC.  Such entries must be back-pached with the proper info.  Only
	the callers generating the TOC in the container know when it is time to do the patching.
	They call this routine to do it when that time comes.
	
	True is returned if the back-patching is successful and false otherwise.
	
	By the time this routine is called, the original TOC entries in question and their
	offsets within the container have been recorded in a BackPatches struct whose pointer is
	passed in thePatches.  Currently only 3 such entries need back-patching:
	
	tocSizeEntry	 			 -	The TOC size entry.  This is a property of TOC #1 which
													represents the size and offset of the TOC itself.  The caller
													passes this info in the tocSize and tocStart parameters
													respectively.
											
	tocContEntry	 			 -	The TOC container entry. This is a property of TOC #1 which
													represents the entire container, from offset 0 to the end of the
													label.  No additional parameters are passed for this.  But to
													compute this, it is required that cmDoBackPathes() be the LAST
													thing called prior to writing the container label.
	 
	tocNewValuesTOCEntry - The non-private TOC offset/size entry.  An updating TOC contains
												 a TOC #1 property that defines the offset and size of all TOC
												 entries that are to be applied to this container's target.  The
												 caller passes this info in the newValuesTOCSize and
												 newValuesTOCStart parameters respectively.  For non-updating
												 containers, these are ignored.
	*/


														  CM_END_CFUNCTIONS
#endif
