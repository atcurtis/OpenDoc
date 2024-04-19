/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odutils
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
/* @(#) 1.4 com/src/utils/include/SEUtils.h, odutils, od96os2, odos29646d 10/30/96 20:23:22 [11/15/96 15:29:02] */
/*
	File:		SEUtils.h

	Contains:	Interface to Semantic Events Utility Functions

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _SEUTILS_
#define _SEUTILS_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif
#if defined(_PLATFORM_WIN32_)
#ifndef _ODTYPESP_
#include "ODTypesP.h"
#endif
#endif // defined(_PLATFORM_WIN32_)

#ifndef SOM_ODPart_xh
#include "Part.xh"
#endif

//#ifndef _SEMTINTF_
//#include "SemtIntf.h"
//#endif

// the type should determine how many bytes are moved....

class ODAppleEvent;
class ODNameResolver;
class ODOSLToken;

#define SETBYTESOFHANDLE(h,typ,data,skipbytes)							\
	*((typ*)((*(h))+(skipbytes))) = (data)								\

#define GETBYTESOFHANDLE(h,typ,skipbytes)								\
	*((typ*)((*(h))+(skipbytes)))										\

#define SETFIRSTBYTESOFHANDLE(h,typ,data)								\
	SETBYTESOFHANDLE(h,typ,data,0)										\

#define FIRSTBYTESFROMHANDLE(h, typ)									\
		GETBYTESOFHANDLE(h,typ,0)										\

const Boolean kFromTypeIsDesc = kODTrue;
const Boolean kFromTypeIsPtr = kODFalse;
const Boolean kIsSysHandler = kODTrue;

const ODSLong kODCountProcSwapValue = -1;

#define NULL_DESCRIPTOR_DEFINITION {typeNull, NULL}

extern "C" {

ODBoolean MissingParams(const AppleEvent* message);
OSErr DecodeOrdinal(AEDesc ordData, long count, long* index,
    				Boolean* allFlag, Boolean* zeroFlag);
ODSLong GetSLongAttr(AppleEvent* ae, AEKeyword keyword);
ODSLong GetSLongAttrOD(ODAppleEvent* ae, AEKeyword keyword);

// does an in-place coercion!
void ThrowIfCantCoerce( AEDesc* data, DescType desiredType ) ;
void ThrowIfNotAbsent( OSErr err ) ;
ODSLong CountEmbeddedParts(Environment* ev, ODPart* prt);

void UpdateUserToken(Environment* ev, ODNameResolver* resolver,
						ODOSLToken* odToken, AEDesc* desc);
	// Update the user token in odToken with data in desc. desc is disposed.

// do a double-dispose of AppleEvents that are copies of what might be
// send-to-self events, since copies of these (eg. what you get when you
// call ODDescToAEDesc) cannot be disposed with a single call to AEDisposeDesc.
OSErr ODDisposeAppleEvent( AppleEvent* aevt );
}

#endif // _SEUTILS_
