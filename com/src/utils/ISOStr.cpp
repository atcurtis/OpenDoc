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
// @(#) 1.4 com/src/utils/ISOStr.cpp, odutils, od96os2, odos29646d 7/15/96 17:58:17 [ 11/15/96 15:28:43 ]
/*
	File:		ISOString.cpp

	Contains:	Routines for manipulating ISO Strings

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#include <ODPagtun.h>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#include <string.h>


ODISOStr ODISOStrFromCStr (const char* cstring)
{
	return strcpy( (char*)ODNewPtr(strlen(cstring)+1), cstring );
}

