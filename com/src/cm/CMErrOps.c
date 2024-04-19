/* @(#)Z 1.4 com/src/cm/CMErrOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:46 (96/10/29 09:16:36) */
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
	File:		CMErrOps.c

	Contains:	Container Manager Error Handler Support Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 8/24/95	EL		1274602, 1277989: bad container name when
									deleting object with use count > 1.
		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                        <<<     CMErrOps.c      >>>                        |
 |                                                                           |
 |            Container Manager Error Handler Support Operations             |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/06/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 All the routines in this file deal with error reporting.  They are provided as a
 convenience to allow the user (usually a handler writer) to convert error codes (defined
 in CMAPIErr.h) to their corresponding message strings.
*/


#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __CM_API__
#include "CMAPI.h"
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
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __ERRORRPT__
#include "ErrorRpt.h"      
#endif
#ifndef __UTILITYROUTINES__
#include "Utility.h"        
#endif
																	
																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment CMErrorHandlerOps
#endif


#define MaxMsgLen 1024										/* no msg should be bigger than this					*/


/*------------------------------------------*
 | my_strtoul - parse a integer in a string |
 *------------------------------------------*
 
 This routine is my private implementation of the standard ANSII library routine strtoul().
 It taks a pointer to a string which is assumed to be pointing at some digits and returns
 the resulting integer.  The pointer to the character that stopped the scan is returned
 in endptr.
 
 This routine is coded here for the following reasons:
 
 		(1).  strtoul() is a ANSI standard routine which another installation might not have
					if they don't have a full ANSI runtime library.
					
		(2).  I don't need all the generality of strtoul().  The base is always assumed as 10
					here and endptr is never NULL.
					
		(3).	There is a bug in the Mac MPW strtoul() routine!  Ah-ha, the REAL reason!  It 
					seems that MPW's strtoul() treats characters as signed.  We could have characters
					with values larger than 127 in our strings.  This causes MPW's strtoul() to
					fail in wierd ways (like when it passes such charas to isalnum()).
*/

static CM_ULONG CM_NEAR my_strtoul(CM_CHAR *s, CM_CHAR **endptr)
{
	CM_ULONG	digit, value = 0;
	
	for (;;) {														/* scan until we don't hit a digit...						*/
		switch (*s) {												/* done this way to be *completely* portable!		*/
			case '0':	digit = 0; break;				/* yuk!	 (there MUST be another way to do this)	*/
			case '1':	digit = 1; break;				/*        But it must be done WITHOUT assuming  */
			case '2':	digit = 2; break;				/*				a character set ordering dependence!	*/
			case '3':	digit = 3; break;
			case '4':	digit = 4; break;
			case '5':	digit = 5; break;
			case '6':	digit = 6; break;
			case '7':	digit = 7; break;
			case '8':	digit = 8; break;
			case '9':	digit = 9; break;
			default:	*endptr = s;						/* return ptr to delimiting non-digit						*/
								return (value);					/* return collected value												*/
		} /* switch */
		
		value = value * 10 + digit;					/* build up the (decimal) number								*/
		++s;																/* ready to look at next character							*/
	} /* for */
}


/*-------------------------------------------------------*
 | CMAddMsgInserts - add all inserts to an error message |
 *-------------------------------------------------------*

 This routine takes the (error) message string in msgString and replaces all substrings
 of the form "^n" with the inserts from the variable arg list of insert strings.  The
 function returns the edited msgString as its result.

 The string buffer must be passed in msgString.  Its max length is also specified but this
 must be a value less than or equal 1024 (not including delimiting null). The message will
 be truncated to fit in the buffer.  The string pointer is returned as the function
 result.
 
 The substring "^0" is replaced with the first insert. "^1" is replaced with the second
 insert, and so on.  It is assumed that there are enough insert strings to cover all the
 "^n"s called for (not unlike printf()).
 
 Note, the "^n"s in the message string do not have to be in ascending order.  Thus "^1"
 could occur before "^0" in the msgString.
*/

CM_CHAR CM_PTR * CM_VARARGS CMAddMsgInserts(CM_CHAR CM_PTR *msgString, CMSize maxLength, ...)
{
	va_list inserts;

	va_start(inserts, maxLength);
	CMVAddMsgInserts(msgString, maxLength, inserts);
	va_end(inserts);
	
	return (msgString);
}


/*--------------------------------------------------------*
 | CMVAddMsgInserts - add all inserts to an error message |
 *--------------------------------------------------------*

 This routine is the same as CMAddMsgInserts() above, except that the extra (insert) 
 arguments are given as a variable argument list as defined by the "stdarg" facility.

 This routine assumes the caller sets up and terminates the variable arg list using the
 "stdarg.h" calls as follows:
 
 			#include <stdarg.h>
			
 			callersRoutine(args, ...)
			{
				va_list inserts;
				
				- - -
				
				va_start(inserts, args);
				CMVAddMsgInserts(msgString, maxLength, inserts);
				va_end(inserts);
				
				- - -
			}
			
 See CMAddMsgInserts() for further details.
*/

CM_CHAR CM_PTR * CM_FIXEDARGS CMVAddMsgInserts(CM_CHAR CM_PTR *msgString, CMSize maxLength,
																					  	 va_list inserts)
{
	CM_CHAR 		 	c, *src, *dst, *p, *insert, msgString0[MaxMsgLen];
	CM_ULONG			cnt = (unsigned long)maxLength;
	CM_LONG				n;
	va_list 			inserts0 = inserts;
	
	src = strcpy(msgString0, (dst = msgString));
	
	while (((CMBoolean)(c = *src++)) && cnt > 0) {/* scan src buffer for "^n"s...						*/
		if (c != '^') {														/* if we don't hace an "^n"...						*/
			*dst++ = c;															/* ...just copy src to dst (output) buffer*/
			--cnt;																	/* ...make sure we don't exceed maxLen		*/
			continue;
		}
	
		n = my_strtoul(src, &p);								/* extract the insert number (n) from "^n"*/
		if (p == src) {														/* leave a lone "^" alone											*/
			*dst++ = c;															/* ...they are treated like normal chars	*/
			--cnt;
			continue;
		}
		
		inserts = inserts0;												/* get n'th insert (there better be one!)	*/
		while (n-- >= 0)													/* there's no way to chk that there isn't	*/
			insert = va_arg(inserts, CM_CHAR *);
	
		while (*insert && cnt > 0) {							/* copy the insert to the dst buffer... 	*/
			*dst++ = *insert++;
			--cnt;																	/* ..careful here too about going to far	*/
		}
		
		src = p;																	/* point past "^n" in the input string		*/
	} /* while */																/* keep scanning the src, copying to dst	*/
	
	*dst = '\0';																/* that's all there is to it!							*/
	return (msgString);
}


#if 0 /* orignal code not used -- it's overkill (it's an insert-in-place algorithm) */
/*--------------------------------------------------------*
 | CMVAddMsgInserts - add all inserts to an error message |
 *--------------------------------------------------------*

 This routine takes the (error) message string in msgString and replaces all substrings
 of the form "^n" with the inserts from the variable arg list of insert strings.  The
 function returns the edited msgString as its result.

 The string buffer must be passed in msgString.  Its max length is also specified but this
 must be a value less than or equal 1024 (not including delimiting null). The message will
 be truncated to fit in the buffer.  The string pointer is returned as the function
 result.
 
 The substring "^0" is replaced with the first insert. "^1" is replaced with the second
 insert, and so on.  It is assumed that there are enough insert strings to cover all the
 "^n"s called for (not unlike printf()).
 
 Note, the "^n"s in the message string do not have to be in ascending order.  Thus "^1"
 could occur before "^0" in the msgString.
*/

CM_CHAR CM_PTR * CM_FIXEDARGS CMVAddMsgInserts(CM_CHAR CM_PTR *msgString, CMSize maxLength,
																					  	 va_list inserts)
{
	CM_CHAR 	*s, *p, *q, *x, *last, *insert, c;
	CM_LONG		n;
	CM_SHORT	msglen, inslen, signalLen, cnt;
	va_list 	inserts0 = inserts;
	
	s = msgString;													/* get initial length and ptr to last char+1	*/
	while (*s++);
	msglen = --s - msgString;
	
	while (--s > msgString) {								/* scan msgString right-to-left for inserts...*/
		if (*s != '^') continue;							/* leave everything else along								*/
		
		n = my_strtoul(s+1, &x);							/* extract the insert number									*/
		if (x == s + 1) continue;							/* leave a lone "^" alone											*/
		signalLen = x - s;										/* this many chars must be replaced						*/
		
		inserts = inserts0;
		while (n-- >= 0)											/* get the n'th insert (there better be one!)	*/
			insert = va_arg(inserts, CM_CHAR *);
		inslen = strlen(insert);							/* we need its length too											*/
		
		cnt  = maxLen - msglen + signalLen;		/* accounts for deletion of the "^n" chars		*/
		
		if (inslen > signalLen) {							/* if expanding string with insert...					*/
			p    = msgString + msglen;					/* 1st position beyond end of current string	*/
			q 	 = p + inslen - signalLen;			/* q points to 1 beyond where to put last char*/
			last = msgString + maxLen;					/* 1 beyond the last char allowed in string		*/
			while (--p > s) 										/* make room for insertion...									*/ 
				if (--q < last) {									/* i.e., move chars right in insertion string	*/
					*q = *p;
				} else														/* we may have to truncate here!							*/
					cnt++;
		} else if (inslen < signalLen) {			/* if contracting string for short insert...	*/
			q = s + signalLen;									/* ptr to the char immediately following "^n"	*/
			p = s + inslen;											/* ptr to end of inset (p < q)								*/ 
			while (*p) *p++ = *q++;							/* move chars left in msg string							*/
		}
		
		p = s;
		while (cnt && *insert) {							/* insert the insert... 											*/
			cnt--;
			*p++ = *insert++;
		}
		
		msglen = maxLen - cnt;								/* update total length including insert				*/
		*(msgString + msglen) = '\0';					/* add required null at end of msg string			*/
	} /* while */
		
	return (msgString);											/* return fully inserted msg string to caller	*/
}
#endif


/*---------------------------------------------------------------------------------------*
 | CMGetErrorString - return CM (error) message string corresponding to a message number |
 *---------------------------------------------------------------------------------------*

 This routine takes a defined Container Manager (error) message number and its
 corresponding insert strings and returns a (english) string corresponding to the message
 number with the inserts filled into their proper positions. It is assumed the error
 number and inserts were the ones reported to the error handler. 
 
 The string buffer must be passed in errorString.  Its max length is also specified but
 this must be a value less than or equal 1024 (not including delimiting null).  The
 message will be truncated to fit in the buffer.  The string pointer is returned as the
 function result.
 
 This routine is provided as a convenience to easily convert the error codes and their
 corresponding inserts to a printable string.
*/

CMErrorString CM_VARARGS CMGetErrorString(CMErrorString errorString, CMSize maxLength, 
															 		 				CMErrorNbr errorNumber, ...)
{
	va_list inserts;

	va_start(inserts, errorNumber);
	CMVGetErrorString(errorString, maxLength, errorNumber, inserts);
	va_end(inserts);
	
	return (errorString);
}


/*----------------------------------------------------------------------------------------*
 | CMVGetErrorString - return CM (error) message string corresponding to a message number |
 *----------------------------------------------------------------------------------------*
 
 This routine is the same as CMGetErrorString() above, except that the extra (inserts) 
 arguments are given as a variable argument list as defined by the "stdarg" facility.
 
 This routine assumes the caller sets up and terminates the variable arg list using the
 "stdarg.h" calls as follows:
 
 			#include <stdarg.h>
			
 			callersRoutine(args, ...)
			{
				va_list inserts;
				
				- - -
				
				va_start(inserts, args);
				CMVGetErrorString(errorString, maxLength, errorNumber, inserts);
				va_end(inserts);
				
				- - -
			}
			
 See CMGetErrorString() for further details.
*/

CMErrorString CM_FIXEDARGS CMVGetErrorString(CMErrorString errorString, CMSize maxLength, 
															  			 			 CMErrorNbr errorNumber, va_list inserts)
{
	CM_CHAR 	*msgStr, msg[MaxMsgLen];
	CM_CHAR		tmpStr[15];
	CMBoolean	gotInserts = true;
	
	#define TheErrorList	switch (errorNumber) {
	#define e0(n, s) 				case n:  msgStr = (CM_CHAR *)s; gotInserts = false; break
	#define e1(n, s) 				case n:  msgStr = (CM_CHAR *)s; break
	#define EndOfList(s)		default: msgStr = (CM_CHAR *)s; gotInserts = false; break;\
												}																												 \
												strcpy((char *)msg, (char *)msgStr);
	
	/* To define a new error number and message, use the macro e1(n, s) if the string s 	*/
	/* contains inserts.  Use e0(n, s) if there are no inserts.  DON'T FORGET TO UPDATE		*/
	/* CMErrno.h.																																					*/
	
	TheErrorList
		/*----------------------------------------------------------------------------------*/
		e1(CM_err_GenericMessage, 	"^0");
		e1(CM_err_BadTocSize, 			"TOC index table size out of range (^0 <= range <= ^1)");
		e1(CM_err_NoSession, 				"Unable to allocate session (task) global data");
		e1(CM_err_NoHandler, 				"Cannot allocate space for handler for type \"^0\"");
		e1(CM_err_BadWriteUse,			"Cannot do a CMOpenNewContainer(...^0...) with useFlags set for update-in-place");
		e1(CM_err_NoContainer,			"Cannot allocate CCB space for type \"^0\"");
		e1(CM_err_UndefMetaHandler, "Metahandler was not defined for type \"^0\"");
		e1(CM_err_HandlerError,			"Allocation problem while looking up handler for type \"^0\"");
		e1(CM_err_NullMetaPtr,			"Null metahandler pointer for type \"^0\"");
		e1(CM_err_UndefRoutine, 		"Handler routine(s) missing for operation type(s): ^0");
		e1(CM_err_NoTOC,						"Cannot allocate TOC for container \"^0\"");
		e1(CM_err_BadReadUse,				"Cannot do a CMOpenContainer(...\"^0\"...) with useFlags set for ^1");
		e1(CM_err_BadMagicBytes,		"Invalid magic byte sequence in label for container \"^0\" - expected \"^1\", got \"^2\"");
		e1(CM_err_BadVersion,				"Invalid version fields in label for container \"^0\" - expected ^1.^2, got ^3.^4");
		e1(CM_err_BadTOCRead,				"Incorrect byte length read while reading TOC for container \"^0\"");
		e1(CM_err_NoObjEntry,				"Cannot allocate space for TOC object entry for container \"^0\"");
		e1(CM_err_MultDef,					"Multiple definition for TOC object ^0 for container \"^1\"");
		e1(CM_err_NoPropEntry,			"Cannot allocate space for TOC property entry for container \"^0\"");
		e1(CM_err_BadContinue,			"Invalid continued value (inconsistent types) in container \"^0\"");
		e1(CM_err_NoValueEntry,			"Cannot allocate space for TOC value entry for container \"^0\"");
		e1(CM_err_BadOffset,				"Invalid offset or value (^0, ^1) for TOC value entry for container \"^2\"");
		e1(CM_err_UndefObjects,			"There is (are) ^0 undefined object(s) in container \"^1\"");
		e1(CM_err_NoStrValue,				"Cannot allocate space for constant value in container \"^0\"");
		e1(CM_err_DupBaseType,			"Cannot add dup base type \"^0\" to type \"^1\" in container \"^2\"");
		e1(CM_err_BadTOCWrite,			"Incorrect byte length written while writing TOC for \"^0\"");
		e1(CM_err_NotGlobalName,		"Have global name tied to value of wrong type in containter \"^0\"");
		e1(CM_err_BadGNameWrite,		"Incorrect byte length written while writing global name \"^0\" in container \"^1\"");
		e1(CM_err_DupGlobalName,		"Duplicate global name definition for \"^0\" in container \"^1\"");
		e1(CM_err_MultGlblNames,		"Object ^0 alread defined -- trying to redefine it for \"^1\" (container \"^2\")");
		e1(CM_err_NoGlobalName,			"Cannot allocate space for global name \"^0\" in container \"^1\"");
		e1(CM_err_NoGNameLoad,			"Cannot allocate space for global name during load in container \"^0\"");
		e1(CM_err_BadGNameRead,			"Incorrect byte length read while reading global name in container \"^0\"");
		e1(CM_err_NotGName,					"Invalid global name string read in container \"^0\"");
		e1(CM_err_BadType,					"Invalid ^0 type passed to ^1 for container \"^2\"");
		e1(CM_err_2Containers,			"Objects not in same containers (containers \"^0\" and \"^1\")");
		e1(CM_err_3Containers,			"Objects not in same containers (containers \"^0\", \"^1\", and \"^2\")");
		e1(CM_err_MissingMinIDSeed,	"Min ID seed value missing in TOC object 1 in container \"^0\"");
		e1(CM_err_MissingTOCObj,		"TOC object ID 1 missing in TOC in container \"^0\"");
		e1(CM_err_NotConverting,		"Cannot use CMDefineValueData except for converting container \"^0\"");
		e1(CM_err_BadDefineData,		"Attempt to define offset (^0) out of range in container \"^1\"");
		e1(CM_err_BadValue,					"Attempt to use a deleted value in container \"^0\"");
		e1(CM_err_BadObject,				"Attempt to use a deleted object in container \"^0\"");
		e1(CM_err_BadContainer,			"Container for ^0 (\"^1\") does not belong to the container being used (\"^2\")");
		e1(CM_err_NoValue,					"No value defined for object ID ^0, property \"^1\" in container \"^2\"");
		e1(CM_err_HasValue,					"Cannot set a (sub)value to an already defined value (container \"^0\")");
		e1(CM_err_AlreadyReleased1,	"Attempting to release a value already completely released in container \"^0\"");
		e1(CM_err_NotReleased,			"A dynamic value has not been fully released in container \"^0\"");
		e1(CM_err_MissingFreeTotal,	"Total free space value missing in TOC object 1 in container \"^0\"");
		e1(CM_err_DupType,					"Attempt to insert two values with the same type (^0) in container \"^1\"");
		e1(CM_err_HasNoValue,				"No value defined for CMReadValueData in container \"^0\"");
		e1(CM_err_BadWrite,					"Write error writing to container \"^0\"");
		e1(CM_err_CantWriteGlbl,		"Cannot write to a global name in container \"^0\"");
		e1(CM_err_Offset2Big,				"Write/insert offset (^0) beyond end of a value in container \"^1\"");
		e1(CM_err_Internal1,				"Internal error! Unknown flags setting (0x^0)");
		e1(CM_err_MissingIDSeed,		"ID seed value missing in TOC object 1 in container \"^0\"");
		e1(CM_err_AmbiguousType,		"^0 object is ambiguous in container \"^1\"");
		e1(CM_err_TypeNotGlobal,		"^0 object is not for a global name in container \"^1\"");
		e1(CM_err_MissingFreeList,	"Internal error - empty free list for property in container \"^0\"");
		e1(CM_err_NoNewValueHandler,"A \"new value\" handler is not defined for type \"^0\"");
		e1(CM_err_UndefReference,		"Object ID ^0 from a reference is undefined in container \"^1\"");
		e1(CM_err_BadObjectKind,		"Invalid kind of object -- expected ^0 in container \"^1\"");
		e1(CM_err_WriteIllegal1,		"Cannot write to a container (\"^0\") opened for reading");
		e1(CM_err_WriteIllegal2,		"Attempt to write to a protected object in container \"^0\"");
		e1(CM_err_ReadIllegal,			"Cannot read from a container (\"^0\") opened for writing");
		e1(CM_err_MissingSize,			"Size value missing in TOC object 1 in container \"^0\"");
		e1(CM_err_BadSize,					"Inconsistent size values between container label and TOC (container \"^0\")");
		e1(CM_err_Internal2,				"Internal error! TOC offset to offset/size value not found in container \"^0\"");
		e1(CM_err_CantDelete1,			"Attempt to delete to a protected object in container \"^0\"");
		e1(CM_err_CantDelete2,			"Attempt to delete to a property with a protected value in container \"^0\"");
		e1(CM_err_CantDelete3,			"Attempt to delete to a protected value in container \"^0\"");
		e1(CM_err_StillOpen,				"Container \"^0\" is still open at session close time");
		e1(CM_err_EmptyRead,				"Cannot read from an empty embedded container value (container \"^0\")");
		e1(CM_err_NoEmbedding,			"Cannot allocate space to save embedding status while opening container \"^0\"");
		e1(CM_err_BadGenNbr,				"Invalid generation number (^0) passed to ^1 (container \"^2\")");
		e1(CM_err_NoRef,						"Connot allocate space for an object reference in container \"^0\")");
		e1(CM_err_CantGetBase,			"CMGetBaseValue() may only be called from a dynamic value handler in container \"^0\"");
		e1(CM_err_MultTypeProp,			"Attempt to register a ^0 name (^1) in container \"^2\" -- already defined as a ^3");
		e1(CM_err_NotSameMode,			"Embedded container (\"^0\") must be opened in same mode as its parent (\"^1\")");
		e1(CM_err_CantDelete4,			"Cannot delete a value currently in use in container \"^0\"");
		e1(CM_err_MissingHandler,		"Memory allocator and dealocator handlers must be supplied");
		e1(CM_err_NoMissingBuffer,	"Unable to allocate private temporary buffer while opening type \"^0\"");
		e1(CM_err_MoveIllegal,			"Cannot move a value in a container (\"^0\") not opened for writing");
		e1(CM_err_DeleteIllegal,		"Attempt to delete ^0 in a container (\"^1\") not opened for writing");
		e1(CM_err_BadDefineType,		"Attempt to define additional data for a immediate value in container \"^0\"");
		e1(CM_err_NoDynExtensions,	"Cannot allocate space for TOC dynamic value entry for type \"^0\" in container \"^1\"");
		e1(CM_err_HandlerRecursed,	"Attempt to use dynamic value handler for ^0() recursively in container \"^1\"");
		e1(CM_err_BadRealValue,			"Invalid base (\"real\") value passed to ^0() in container \"^1\"");
		e1(CM_err_NoMetahandler,		"A value operations metahandler is not defined for type \"^0\" in container \"^1\"");
		e1(CM_err_NotDynValue,			"A \"use value\" metahandler did not create a dynamic value in container \"^0\"");
		e1(CM_err_NoGlobalTable,		"Cannot allocate globally unique name table for container \"^0\"");
		e1(CM_err_BadMetaSpec,			"Invalid metadata format specification (%^0) for type \"^1\" in container \"^2\"");
		e1(CM_err_NoDeletesList,		"Internal error - empty \"deletes\" list for property in container \"^0\"");
		e1(CM_err_NoDataPacket,			"Cannot allocate space for data packet for type \"^0\" in container \"^1\"");
		e1(CM_err_BaseRelAttempted,	"A dynamic value release handler attempted to release its base in container \"^0\"");
		e1(CM_err_NoDynMetahandler,	"\"Use value\" handler for type \"^0\" in container \"^1\" MUST return a metahandler");
		e1(CM_err_MissingTotalSize,	"Total container size value missing in TOC object 1 in container \"^0\"");
		e1(CM_err_Internal3,				"Internal error! TOC offset to container value not found in container \"^0\"");
		e1(CM_err_AmbiguousUseFlags,"Ambiguous updating useFlags passed to CMOpenNewContainer(...\"^0\"...) - ^1");
		e1(CM_err_NoTypeHandler,		"Dynamic value type handler not defined for updating in container \"^0\"");
		e1(CM_err_NotDynamicValue,	"Dynamic value in container \"^0\" not created to access target for updating");
		e1(CM_err_NoMetaDataHandler,"A \"metadata\" handler is not defined for type \"^0\"");
		e1(CM_err_NoDataBuffer,			"Cannot allocate space for data I/O buffer in container \"^0\"");
		e1(CM_err_BadUpdateRead,		"Incorrect byte length read while reading updates in container \"^0\"");
		e1(CM_err_BadUpdateWrite,		"Write error while writing updates in container \"^0\"");
		e1(CM_err_Internal4,				"Internal error! End-of-updates signal not detected in container \"^0\"");
		e1(CM_err_Internal5,				"Internal error! TOC offset to updates TOC subsection not found in container \"^0\"");
		e1(CM_err_NoNewValuesTOC,		"\"New values\" TOC offset/size missing in TOC object 1 in container \"^0\"");
		e1(CM_err_wrappedIDs,				"Too many objects in container \"^0\" or next user object ID > 0xFFFFFFFF");
		e1(CM_err_NoTouchedEntry,		"Cannot allocate space for recording updating operation in container \"^0\"");
		e1(CM_err_NoUpdateObject,		"Container \"^0\" updating -- cannot find object ID ^1 to be ^2");
		e1(CM_err_NoUpdateProperty,	"Container \"^0\" updating -- cannot find a property ID ^1 in object ID ^2 to be ^3");
		e1(CM_err_BadUpdateControl,	"Container \"^0\" updating -- unknown control byte read (0x^1, during ^2)");
		e1(CM_err_NoUpdateType,			"Container \"^0\" updating -- cannot find value (type ID ^1) in object ^2, property ^3");
		e1(CM_err_UndefUpdateObject,"Container \"^0\" updating -- cannot find ^1 (ID ^2) to use in ^3");
		e1(CM_err_UpdateBadGenNbr,	"Container \"^0\" updating -- invalid generation number (^1)");
		e1(CM_err_BadInsertData,		"Container \"^0\" updating -- bad updating info (0 segment insert encountered)");
		e1(CM_err_BadInsertOffset,	"Container \"^0\" updating -- insert offset (^1) beyond end of a value");
		e1(CM_err_CantRepImmediate,	"Container \"^0\" updating -- attempt to replace non-immediate with immediate");
		e1(CM_err_CantRepBaseType,	"Container \"^0\" updating -- attempt to replace non-base type value with a base type");
		e1(CM_err_CantReference,		"Value and referenced object not in the same containers (containers \"^0\" and \"^1\")");
		e1(CM_err_GloblaNameError,	"Allocation problem while looking up global name \"^0\" in container \"^1\"");
		e1(CM_err_FlushErr,					"Error detected in flushing output while closing container \"^0\"");
		e1(CM_err_CantDelete5,			"Cannot delete an object with unreleased dynamic values in use in container \"^0\"");
		e1(CM_err_NoTOCBuffer,			"Cannot allocate space for TOC I/O buffer in container \"^0\"");
		e1(CM_err_BadTOCCode,				"Invalid TOC code read (0x^0) from TOC in container \"^1\"");
		e1(CM_err_TOCoutOfSync,			"TOC reading out-of-sync (trying to read across buffer boundary) in container \"^0\"");
		e1(CM_err_TOCParseErr1,			"TOC \"parse\" error - expected object ID, got 0x^0 in container \"^1\"");
		e1(CM_err_TOCParseErr2,			"TOC \"parse\" error - got 0x^0 at an unexpected time in container \"^1\"");
		e1(CM_err_Unsupported1,			"8-byte offset in container \"^0\" are not supported in this implementation");
		e1(CM_err_CantDelete6,			"Cannot delete an object currently in use in container \"^0\"");
		e1(CM_err_AlreadyReleased2,	"Attempting to release an object already completely released in container \"^0\"");
		e1(CM_err_BadRefRead,				"Read error while reading references in container \"^0\"");
		e1(CM_err_Internal6,				"Internal error! Missing reference property or value in container \"^0\"");
		e1(CM_err_ZeroRefKey,				"Attempt to use a CMReference key of 0 in container \"^0\"");
		e1(CM_err_NoRefShadowList,	"Cannot allocate space to record reference in container \"^0\"");
	/*----------------------------------------------------------------------------------*/
	EndOfList(strcat("Undefined error message for error #", cmltostr(errorNumber, 1, false, tmpStr)));

	if (gotInserts)
		CMVAddMsgInserts(msg, maxLength, inserts);
	
	if ((CMSize)strlen(msg) <= maxLength)
		strcpy((CM_CHAR *)errorString, (CM_CHAR *)msg);
	else {
		strncpy((CM_CHAR *)errorString, (CM_CHAR *)msg, (int)maxLength);
		*(errorString + maxLength) = '\0';
	}
		
	return (errorString);
}


/*------------------------------------------------------*
 | CMReturnContainerName - get container identification |
 *------------------------------------------------------*

 Generally the errors reported are provided with at least one insert that identifies which
 container we're talking about.  The wording of the messages defined for the Container 
 Manager assume this identification insert.  The identification takes the form of the
 container "name" which is obtained from a handler routine provided for that purpose.
 
 For convenience, the name providing handler routine is allowed to be optional, i.e., the
 user need not provide one.  This means every time we want to get the identification we
 must test to see if the handler was provided.  This could be a waste of code space, 
 particularly since we mostly use this for error reporting ("mostly" -- actually this is
 the only context it is presenting used in).
 
 To avoid the space waste, this routine is provided to do the test and call the handler if
 it exists.  If it doesn't exist the container type name (using the TYPENAMEx macro) is
 used.  If it does, we use the handler.
 
 Note, this routine is available to users (usually handler writers) so that they too can
 generalize their error reporting/message routines and word their messages with the
 container identification. 
*/

CM_CHAR CM_PTR * CM_FIXEDARGS CMReturnContainerName(CMContainer container)
{
	return (((ContainerPtr)container)->handler.cmreturnContainerName == NULL ? 
						TYPENAMEx(container) :
						CMreturnContainerName(container));
}
														  
															CM_END_CFUNCTIONS
