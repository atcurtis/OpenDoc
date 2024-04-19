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
// @(#) 1.3 com/src/utils/include/ODNewObj.h, odutils, od96os2, odos29646d 7/15/96 18:01:10 [ 11/15/96 15:29:33 ]
/*
	File:		ODNewObj.h

	Contains:	Abstract wrapper for instantiating objects by class-name

	Owned by:	Jens Alfke

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#ifndef _ODNEWOBJ_
#define _ODNEWOBJ_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

class SOMObject;

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

extern "C" {

	SOMObject*	ODNewObject( const char *className );
	
	ODBoolean	ODClassExists( const char *className );

} 

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif

#endif /*ODNEWOBJ*/
