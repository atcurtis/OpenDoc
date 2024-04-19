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
// @(#) 1.8 com/src/utils/include/PlfmDef.h, odutils, od96os2, odos29646d 7/15/96 18:01:43 [ 11/15/96 15:29:01 ]
/*
	File:		PlfmDef.h

	Contains:	Platform specific definitions

	Written by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#ifndef _PLFMDEF_
#define _PLFMDEF_

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
#ifndef __LOWMEM__
#include <LowMem.h>
#endif
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

//==============================================================================
// Macros
//==============================================================================

#define	ODMethod	virtual	
#define ODVMethod	virtual
#define ODNVMethod
#define ODPascal	pascal
#define ODStatic	static

//------------------------------------------------------------------------------
//
// ODUnused is for parameters not used in a function. It is similar to 
// #pragma unused. As a matter of fact, ODUnused can be a pragma statement
// when it functions correctly in C++. For the time being, we have this little 
// macro to get rid of the compiler warnings. Please use this macro instead of
// commenting out the parameter name.

#define ODUnused(x)	((void) &x)

//==============================================================================
// Globals
//==============================================================================

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
#pragma lib_export on
extern QDGlobals * const gODQD;
#pragma lib_export off

#define ODQDGlobals			(*gODQD)
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)


#endif	// _PLFMDEF_
