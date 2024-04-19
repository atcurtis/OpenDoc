/* @(#)Z 1.4 com/src/storage/TransltB.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:29:27 (96/10/29 09:32:04) */
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
	File:		TransltB.cpp

	Contains:	Implementation of ODBaseTranslation

	Owned by:	Vincent Lo

	Copyright:	� 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <6>	 5/26/95	VL		1251403: Multithreading naming support.
		 <5>	 3/27/95	CC		1231902: Added ODSession parameter to
									InitTranslation().
		 <4>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		 <3>	 8/16/94	VL		1180054: TypeSet should be TypeList.
		 <2>	 7/26/94	VL		Added SubClassResponsibility.
		 <1>	 6/28/94	VL		first checked in

	To Do:
*/

// No platform-independent code at this time
