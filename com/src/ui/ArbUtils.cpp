/* @(#)Z 1.3 com/src/ui/ArbUtils.cpp, odui, od96os2, odos29646d 96/11/15 15:29:30 (96/07/15 18:25:50) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odui
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
/*
	File:		ArbUtils.cpp

	Contains:	xxx put contents here xxx

	Owned by:	xxx put writers here xxx

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 5/26/95	RR		#1251403: Multithreading naming support
		 <1>	 9/26/94	RR		first checked in

	To Do:
*/

#ifndef _ARBUTILS_
#include "ArbUtils.h"
#endif

//======================================================================================
// Class FocusLink
//======================================================================================

FocusLink::FocusLink(ODTypeToken focus)
{
	fFocus = focus;
}
		
FocusLink::~FocusLink() 
{
}
			
