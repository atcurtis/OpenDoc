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
// @(#) 1.3 com/src/utils/include/TransUtl.h, odutils, od96os2, odos29646d 7/15/96 18:02:27 [ 11/15/96 15:29:35 ]
/*
	File:		TransUtl.h

	Contains:	Translation utilities.

	Owned by:	Caia Grisar

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	To Do:
*/


#ifndef _TRANSUTL_
#define _TRANSUTL_

//==============================================================================
// Classes used by this interface
//==============================================================================
class	ODTypeList;
class	ODSession;
class	OrderedCollection;

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

extern "C" {

// GetGetDestinationKindsList is used to get a list of possible kinds to translate
// to (translateToList) given a list of kinds (kindList).  The translateFromList 
// paramenter contains a list of indexes that correspond to each entry in the 
// translateToList which indicates which index in the kindList it came from.  The 
// argument to translateFromList may be null in which case it is ignored.
void GetDestinationKindsList(
				ODTypeList*	kindList,
				ODTypeList*	translateToList,
				OrderedCollection* translateFromList,
				ODSession* session);

}	// End of extern "C" {

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif

#endif /* _TRANSUTL_ */
