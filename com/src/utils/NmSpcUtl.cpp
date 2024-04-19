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
// @(#) 1.8 com/src/utils/NmSpcUtl.cpp, odutils, od96os2, odos29646d 7/15/96 17:58:33 [ 11/15/96 15:28:45 ]
/*
	File:		NmSpcUtl.cpp

	Contains:	Utilities for working with NameSpaces.

	Owned by:	David McCusker

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODValueNameSpace_xh
#include <ValueNS.xh>
#endif

#ifndef SOM_ODValueIterator_xh
#include <ValueItr.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
#ifndef _MEMMGR_
#include <MemMgr.h>
#endif
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
#ifndef __ODPAGTUN__
#include <ODPagTun.h>
#endif
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif


void ValueNameSpaceRegister(ODValueNameSpace* ns, 
							Environment* ev,
							ODISOStr key,
							ODPtr buffer, 
							ODULong size)
{
	ODByteArray	ba;
	ba._buffer = (octet*) buffer;
	ba._length = size;
	ba._maximum = size;
	
	ns->Register(ev, key, &ba);
}

ODBoolean ValueNameSpaceGetEntry(ODValueNameSpace* ns, 
								 Environment* ev,
  						         ODISOStr key,
  						         ODPtr* value, 
  						         ODULong* valueLength)
{
	ODByteArray ba;

	if (ns->GetEntry(ev, key, &ba))
	{
		*value = ba._buffer;
		*valueLength = ba._length;
		return kODTrue;
	}
	else
	{
		*value = kODNULL;
		return kODFalse;
	}
}

ODBoolean ValueNameSpaceGetODName(ODValueNameSpace* ns, 
								 Environment* ev,
  						         ODISOStr key,
  						         ODName** value)
{
	ODByteArray ba;
	
	if (ns->GetEntry(ev, key, &ba))
	{
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
		ODTradITextData* theIName = (ODTradITextData*)ODNewPtrClear(ba._length, kDefaultHeapID);
		ODBlockMove(ba._buffer, theIName, ba._length);
		ODDisposePtr( ba._buffer );
		ODULong textLen = ba._length - sizeof(ODTradITextDataHeader);
		StringPtr thePString = (StringPtr)ODNewPtrClear(textLen+1);
		thePString[0] = textLen;
		ODBlockMove((ODPtr)theIName->theText, (ODPtr)&thePString[1], textLen );
		*value = CreateITextPString(theIName->theScriptCode, 
							 theIName->theLangCode, 
							 thePString);
		ODDisposePtr( theIName );
		ODDisposePtr( thePString );
#else // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
		*value = (ODName *)ba._buffer;
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
		return kODTrue;
	}
	else
	{
		*value = kODNULL;
		return kODFalse;
	}
}

void ValueIteratorFirst(ODValueIterator* iter, 
					    Environment* ev,
			            ODISOStr* key,
			            ODPtr* value, 
			            ODULong* valueLength)
{
	ODByteArray ba;
	ba._length = 0;
	
	iter->First(ev, key, &ba);
	
	if (ba._length > 0)
	{
		*value = ba._buffer;
		*valueLength = ba._length;
	}
	else
		*value = kODNULL;
}

void ValueIteratorNext(ODValueIterator* iter, 
					    Environment* ev,
			            ODISOStr* key,
			            ODPtr* value, 
			            ODULong* valueLength)
{
	ODByteArray ba;
	ba._length = 0;
	
	iter->Next(ev, key, &ba);
	
	if (ba._length > 0)
	{
		*value = ba._buffer;
		*valueLength = ba._length;
	}
	else
		*value = kODNULL;
}


ODISOStr ValueNameSpaceGetString( ODSession* sessn, ODISOStr nameSpaceName, ODISOStr key )
{
	ODByteArray ba;
	ODISOStr result = kODNULL;
	Environment* ev = somGetGlobalEnvironment();
	ODNameSpaceManager* mgr = sessn->GetNameSpaceManager(ev);
	ODValueNameSpace* ns = (ODValueNameSpace*) mgr->HasNameSpace(ev, nameSpaceName);
	if ( ns && ns->GetEntry(ev, key, &ba) && ba._length > 0 )
		result = (ODISOStr) ba._buffer;

	// The old version of this function used to allocate space for a terminating
	// null byte; but as near as I can tell, the only ways that strings become
	// values also allocate the null byte space (as they should) when the entry
	// is orginally created.
	
	return result;
}

void ValueNameSpaceWriteToFile( ODValueNameSpace* ns, PlatformFile* file )
{
	Environment* ev = somGetGlobalEnvironment();
	ODByteArray ba; 
	ba._buffer = (octet*) file; 
	ba._length = sizeof(PlatformFile); 
	ns->WriteToFile( ev, &ba );
}

void ValueNameSpaceReadFromFile( ODValueNameSpace* ns, PlatformFile* file )
{
	Environment* ev = somGetGlobalEnvironment();
	ODByteArray ba; 
	ba._buffer = (octet*) file; 
	ba._length = sizeof(PlatformFile); 
	ns->ReadFromFile( ev, &ba );
}


//------------------------------------------------------------------------------
// CreateFlatIText
//------------------------------------------------------------------------------

ODTradITextData* CreateFlatIText(ODScriptCode theScriptCode, ODLangCode theLangCode, 
												char* theText, ODUShort textLen)
{
	ODTradITextData* result = (ODTradITextData*)
			 ODNewPtrClear(textLen+sizeof(ODTradITextDataHeader), kDefaultHeapID);
	result->theScriptCode = theScriptCode;
	result->theLangCode = theLangCode;
	
	ODBlockMove((ODPtr)theText, (ODPtr)result->theText, textLen);
	
	return result;
}


