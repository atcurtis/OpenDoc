/* @(#)Z 1.4 com/src/storage/ClipbdB.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:29:26 (96/10/29 09:30:36) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: oddataxfer
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
	File:		ClipbdB.cpp

	Contains:	Implementation for ODBaseClipboard class.

	Owned by:	Craig Carper

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <5>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <4>	 5/26/95	VL		1251403: Multithreading naming support.
		 <3>	 3/10/95	CC		1225050: Added DraftClosing() method.
		 <2>	  8/3/94	CC		Removed Lock() and Unlock(); removed
									ODClipboardKey parameter from other
									methods; added SubclassResponsibility()
									calls.  (1160484)
		 <1>	 6/23/94	CC		first checked in
	To Do:
*/

// No platform-independent code at this time
