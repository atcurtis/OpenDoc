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
// @(#) 1.5 com/src/utils/include/NmSpcUtl.h, odutils, od96os2, odos29646d 7/15/96 18:00:43 [ 11/15/96 15:29:32 ]
/*
	File:		NmSpcUtl.h

	Contains:	Utilities for working with NameSpaces.

	Owned by:	Caia Grisar

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	

	In Progress:
		
*/


#ifndef _NMSPCUTL_
#define _NMSPCUTL_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _STORUTIL_
#include "StorUtil.h"
#endif

class ODValueNameSpace;
class ODValueIterator;

extern "C" {
	void		ValueNameSpaceRegister(ODValueNameSpace* ns, Environment* ev, ODISOStr key, ODPtr buffer, ODULong size);

	ODBoolean	ValueNameSpaceGetEntry(ODValueNameSpace* ns, Environment* ev, ODISOStr key, ODPtr* value, ODULong* valueLength);
				// value must be a pointer to where you would like to have a 
				// pointer written. If the entry exists, a copy will be
				// allocated and returned in *value, which you must eventually
				// deallocate (because you now own this space); otherwise
				// *value will be nil when there is no such entry.
	
	ODBoolean	ValueNameSpaceGetODName(ODValueNameSpace* ns, Environment* ev, ODISOStr key, ODName** value);
				// value must be a pointer to an ODName to where you would like to have a 
				// pointer written. If the entry exists, an IText will be created
				// and returned in *value, which you must eventually
				// dispose of (because you now own this space); otherwise
				// *value will be nil when there is no such entry.
	
	void		ValueIteratorFirst(ODValueIterator* nsitr, Environment* ev, ODISOStr* key, ODPtr* value, ODULong* valueLength);
				// Unless the iteration is complete, *key and *value return
				// newly allocated copies of the next key and value.  You are
				// responsible for deallocated this space when you are done
				// with it.
	
	void		ValueIteratorNext(ODValueIterator* nsitr, Environment* ev, ODISOStr* key, ODPtr* value, ODULong* valueLength);
				// Unless the iteration is complete, *key and *value return
				// newly allocated copies of the next key and value.  You are
				// responsible for deallocated this space when you are done
				// with it.

	ODISOStr	ValueNameSpaceGetString( ODSession*, ODISOStr nameSpaceName, ODISOStr key );
				// ** Returns pointer to new copy of string, or NULL.
					
	void ValueNameSpaceWriteToFile( ODValueNameSpace* ns, PlatformFile* file );
	void ValueNameSpaceReadFromFile( ODValueNameSpace* ns, PlatformFile* file );
	ODTradITextData* CreateFlatIText(ODScriptCode scriptCode, ODLangCode langCode, 
												char* theText, ODUShort textLen);
}

#endif
