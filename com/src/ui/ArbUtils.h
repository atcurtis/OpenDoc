/* @(#)Z 1.2 com/src/ui/ArbUtils.h, odui, od96os2, odos29646d 96/11/15 15:29:30 (96/07/15 18:25:54) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES:   FocusLink
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
	File:		ArbUtils.h

	Contains:	xxx put contents here xxx

	Owned by:	xxx put writers here xxx

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <1>	 9/26/94	RR		first checked in

	To Do:
	In Progress:
*/

#ifndef _ARBUTILS_
#define _ARBUTILS_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class FocusLink;

//=====================================================================================
// Classes used by this interface
//=====================================================================================



//======================================================================================
// Class FocusLink
//======================================================================================

class FocusLink : public Link {
	public:
		FocusLink(ODTypeToken focus);
		~FocusLink();
			
		ODTypeToken 		fFocus;
};

#endif // _ARBUTILS_
