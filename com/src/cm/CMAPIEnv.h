/* @(#)Z 1.6 com/src/cm/CMAPIEnv.h, odstorage, od96os2, odos29646d 96/11/15 15:26:41 (96/10/29 09:15:59) */
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
	File:		CMAPIEnv.h

	Contains:	Container Manager API Environment Specific Definitions

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 1/19/95	VL		1183685: Turned on Merge Container flag.
		 <2>	 8/26/94	EL		#1182308 Make endian flag platform
									independent
		 <3>	 3/17/94	EL		Add endian-ness flag #1149979

	To Do:
*/

/* (use tabs = 2 to view this file correctly) */
/*---------------------------------------------------------------------------*
 |                                                                           |
 |                       <<< CM_API_Environment.h >>>                        |
 |                                                                           |
 |           Container Manager API Environment Specific Definitions          |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  4/27/92                                  |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1994               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file is included by CM_API.h and is intended to contain site-specific and compiler-
 specific stuff that should be defined for the particular installation which is used to
 compile the Container Manager code and the user's API calls.  People responsible for
 installing the Container Manager on specific environments are EXPECTED to change this
 file.
 
 The stuff here is assumed to be global macros utilized by the API and the Container
 Manager code itself.
 
 
											  *------------------------------*
												| NOTE TO DOS (80x86) USERS... |
												*------------------------------*
 
 The Container Manager should be compiled with the "large" memory model so that all
 routines, pointers, etc. are "far".  All external API interfaces are appropriately
 qualified this way.  Since handlers are implementation or user dependent, these files
 must be compiled with the large memory model.  The only exception is local static
 routines which usually can be qualified with "near" (except, of course, for handlers).
*/

#ifndef __CM_API_ENV__
#define __CM_API_ENV__

#ifndef MergeContainersAvailable
#define MergeContainersAvailable	1
#endif


/*--------------------------*
 | Target Machines Switches |
 *--------------------------*

 If there are machine specific definitions, define an appropriate macro switch here...
*/

#ifndef _PLATFORM_BIG_ENDIAN_
#define CM_80x86        1
#endif

#ifndef CM_80x86															/* 1 if target is x86 machine							*/
#define CM_80x86 	0
#endif

#if CM_80x86
#define kCMDefaultEndian 0x0101U
#else
#define kCMDefaultEndian 0x0000U
#endif

/*--------------------------*
 | Target Compiler Switches |
 *--------------------------*
 
 If there are compiler specific definitions, define an appropriate macro switch here...
*/

#ifndef CM_MICROSOFT
#define CM_MICROSOFT 	0								/* 80x86 machines using Microsoft DOS compilers		*/
#endif

#ifndef CM_BORLAND
#define CM_BORLAND 		0								/* 80x86 machines using Borland DOS compilers			*/
#endif

#ifndef CM_ZORTECH
#define CM_ZORTECH 		0								/* 80x86 machines using Zortech DOS compilers			*/
#endif

#ifndef CM_MPW												/* Macintosh MPW C compiler/environment						*/
#define CM_MPW 				0
#endif


/*----------*
 | #pragmas |
 *----------*
 
 Environment #pragmas should go here.  Remember this file is included to build the
 Container Manager too!
*/


/*-------------------------------*
 | Compiler Specific Definitions |
 *-------------------------------*
 
 The following are used in to modify the source to satisfy the "peculiarities" of various
 compilers used to compile the Container Manager.
*/

/* Compilers in 80x86 environments define the "size" of pointers as a function of the 	*/
/* memory modes used.  In these environments, it is assumed and required that the 			*/
/* Container Manager code is ALWAYS compiled with a "large" memory module.  The 				*/
/* following define common names to use for the keywords that control pointer sizes:		*/

#if CM_MICROSOFT
#define CM_NEAR _near									/* the "near" keyword for Microsoft C							*/
#define CM_FAR  _far									/* the "far" keyword for Microsoft C							*/
#elif CM_BORLAND
#define CM_NEAR near									/* the "near" keyword for Borland C								*/
#define CM_FAR  far										/* the "far" keyword for Borland C								*/
#elif CM_ZORTECH
#define CM_NEAR near									/* the "near" keyword for Zortech C								*/
#define CM_FAR  far										/* the "far" keyword for Zortech C								*/
#else
#define CM_NEAR												/* Non-DOS environments don't need this stuff!			*/
#define CM_FAR
#endif

/* Microsoft, Borland, and Zortech DOS C compilers support multiple linkage conventions.*/
/* In particular, C and Pascal conventions.  As it turns out, in general, the Pascal		*/
/* calling conventions are more efficient than the C conventions for these compilers.  	*/
/* But Pascal calling conventions cannot be used when there are "..." (variable number 	*/
/* of) arguments.  So the following allow for overriding the default calling 						*/
/* conventions:																																					*/

#ifndef CM_C
#if CM_MICROSOFT
#define CM_C 			_cdecl							/* the "cdecl" keyword for Microsoft C						*/
#elif CM_BORLAND
#define CM_C 			cdecl								/* the "cdecl" keyword for Borland C							*/
#elif CM_ZORTECH
#define CM_C 			cdecl								/* the "cdecl" keyword for Zortech C							*/
#else
#define CM_C 													/* no override provided for C calling conventions	*/
#endif
#endif

#ifndef CM_PASCAL
#if CM_MICROSOFT
#define CM_PASCAL _pascal							/* the "pascal" keyword for Microsoft C						*/
#elif CM_BORLAND
#define CM_PASCAL pascal							/* the "pascal" keyword for Borland C							*/
#elif CM_ZORTECH
#define CM_PASCAL pascal							/* the "pascal" keyword for Zortech C							*/
#else
#define CM_PASCAL 										/* no override provided for Pascal								*/
#endif
#endif

/* Note: Because default linkage conventions are usually a function of the options			*/
/*			 specified to the compiler, the C and Pascal macros may also be controlled from	*/
/* 			 the options.  Hence the #ifndef's.																							*/

/* Finally, the API user doesn't want know or care how the Container Manager is built.	*/
/* Nor should the user care what linkage conventions are being used. So the API headers */
/* use the following and more generic attributes of the functions and pointers to				*/
/* explicitly override whatever memory model the user is using.  These attributes fall	*/
/* into three classes:																																	*/

/*		1. Pointer sizes: CM_PTR																													*/

/*			 Since the Container Manager is built with the large memory model in 80x86			*/
/*			 environments, all pointers are "far" pointers. 																*/

/*		2. Functions that take a variable number of arguments: CM_VARARGS									*/

/*			 This is a unique class of functions since the calling conventions for these		*/
/*			 functions may be different from those with a fixed number of arguments.  For		*/
/*			 example, in Microsoft, C calling conventions must be used here while the more	*/
/*			 efficient Pascal calling conventions can be used for functions with a fixed		*/
/*			 number of arguments (which is class 3).																				*/

/*		3. Functions that take a fixed number of arguments: CM_FIXEDARGS									*/

/*			 API functions that take a fixed number of arguments are given this attribute.	*/
/*			 This allows the Container Manager builder to use different calling conventions	*/
/*			 from those used for functions that take a variable number of arguments.				*/

#define CM_PTR			 CM_FAR						/* pointer attribute															*/
#define CM_VARARGS	 CM_FAR CM_C 			/* variable nbr of arguments function attribute		*/
#define CM_FIXEDARGS CM_FAR CM_PASCAL	/* fixed nbr of arguments function attribute			*/

/* The following are all defined to allow compiling the source under C++.  These are		*/
/* used to define all API and handler interfaces as C functions rather than C++ 				*/
/* functions.  Bracket each function, or group of functions, or their prototypes with		*/
/* CM_CFUNCTION and CM_END_CFUNCTION.  Plural versions are also defined to aid 					*/
/* readability.																																					*/

#ifdef __cplusplus
#define CM_CFUNCTION 			extern "C" {
#define CM_CFUNCTIONS 		extern "C" {
#define CM_END_CFUNCTION	}
#define CM_END_CFUNCTIONS	}
#else
#define CM_CFUNCTION
#define CM_CFUNCTIONS
#define CM_END_CFUNCTION
#define CM_END_CFUNCTIONS
#endif

/* NOTE: There is a subtle C++ semantic rule involving linkage specifications.  When a	*/
/*			 function is declared as a C function, and that function takes a function 			*/
/*			 pointer as a parameter (e.g., in API calls like CMSetMetaHandler(), handler		*/
/*			 addresses, etc.), then the passed function must itself be a C function!  Thus	*/
/* 			 such functions must also be bracketed with the above definitions.  Thanks a 		*/
/*			 lot Bjarne (%^%$#@!).																													*/

#endif
