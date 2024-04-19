//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.3 com/src/utils/include/ISOStr.h, odutils, od96os2, odos29646d 7/15/96 18:00:28 [ 11/15/96 15:28:59 ]
/*
	File:		ISOStr.h

	Contains:	Routines to manipulate ODISOStr

	Owned by:	Steve Smith, Vincent Lo.

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _ISOSTR_
#define _ISOSTR_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define ODISOStrCopy(D,S)		strcpy(D,S)
#define ODISOStrNCopy(D,S,L)	strncpy(D,S,L)
#define ODISOStrConcat(D,S)		strcat(D,S)
#define ODISOStrNConcat(D,S,L)	strncat(D,S,L)
#define ODISOStrCompare(A,B)	strcmp(A,B)
#define ODISOStrNCompare(A,B,L)	strncmp(A,B,L)
#define ODISOStrEqual(A,B)		(strcmp(A,B)==0)
#define ODISOStrLength(S)		strlen(S)

ODISOStr	ODISOStrFromCStr(const char* cstring);

#ifdef __cplusplus
}
#endif

#endif /*_ISOSTR_*/
