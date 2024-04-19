/* @(#)Z 1.4 com/src/bento/SUClone.h, odstorage, od96os2, odos29646d 96/11/15 15:29:29 (96/10/29 09:13:30) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   SUCloneHelper
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
	File:		SUClone.h

	Contains:	SUCloneHelper header

	Written by:	Vincent Lo

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <1>	 1/26/95	VL		first checked in
	To Do:
	
*/

#ifndef _SUCLONE_
#define _SUCLONE_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

//==============================================================================
// Classes defined in this interface
//==============================================================================
class SUCloneHelper;

//==============================================================================
// Classes used by this interface
//==============================================================================
class ScopeIDList;

//==============================================================================
// SUCloneHelper
//==============================================================================
class SUCloneHelper
{
public:

	SUCloneHelper();
	
	~SUCloneHelper();
	
	ODBoolean ShouldClone(ODDraftKey key, ODID id);

private:

	ScopeIDList*	fScopeIDList;
	ODDraftKey		fKey;
	ODMemoryHeapID	fHeap;
	
	ODMemoryHeapID	GetHeap() {return fHeap;}
};

#endif	// _SUCLONE_
