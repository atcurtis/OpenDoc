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
// @(#) 1.4 com/src/utils/TransUtl.cpp, odutils, od96os2, odos29646d 7/15/96 17:59:46 [ 11/15/96 15:28:57 ]
/*
	File:		TransUtl.cp

	Contains:	Translation utilities.

	Owned by:	Caia Grisar

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
*/

#ifndef _TRANSUTL_
#include <TransUtl.h>
#endif

#ifndef _ODEXCEPT_
#include <ODExcept.h>
#endif

#ifndef _TYPELIST_
#include "TypeList.xh"
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _TEMPITER_
#include <TempIter.h>
#endif

//------------------------------------------------------------------------------
// GetDestinationKindsList
//------------------------------------------------------------------------------
void GetDestinationKindsList(
		ODTypeList*	kindList,
		ODTypeList*	translateToList,
		OrderedCollection* translateFromList,
		ODSession* session)
{
	Environment* ev = somGetGlobalEnvironment();

	ODType kind = kODNULL;
	ODTranslation* translation = session->GetTranslation(ev);

	ODULong kindIndex = 0;

	for( TempODTypeListIterator kindIter(ev,kindList); kindIter; kindIter.Next() )
	{
		ODType kind = kindIter.Current();
		++kindIndex;

		TempODTypeList toList = translation->GetTranslationOf(ev, kind);
		
		for( TempODTypeListIterator toIter(ev,toList); toIter; toIter.Next() )
		{
			ODType toKind = toIter.Current();

			if ( (kindList->Contains(ev, toKind) == kODFalse)
				&&
				 (translateToList->Contains(ev, toKind) == kODFalse)
			   )
			{
				translateToList->AddLast(ev, toKind);
				if ( translateFromList )
					translateFromList->AddLast((void*) kindIndex);
			}
		}
	}
}


