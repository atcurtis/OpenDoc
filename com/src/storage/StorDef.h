/* @(#)Z 1.4 com/src/storage/StorDef.h, odstorage, od96os2, odos29646d 96/11/15 15:29:27 (96/07/15 18:23:31) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
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
/*
	File:		StorDef.h

	Contains:	Private Definition for Storage System

	Owned by:	Vincent Lo
	
	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):
	
		 <2>	10/13/95	EL		1287340: Use standard ISO prefix
		 <1>	 10/4/94	VL		first checked in

	To Do:
*/

#ifndef _STORDEF_
#define _STORDEF_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

const	ODValueType	kODAppleScrapTypePrefix	= "+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:";
const	ODValueType	kODAppleFileTypePrefix	= "+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:FileType:";

#ifdef _PLATFORM_WIN32_
#define kODdragDraw 					0x00000001
#define kODdragErase					0x00000002
#define kODdragMove					0x00000008
#define kODdragCopy					0x00000010
#define kODdragLink					0x00000020
#endif  // WIN32

#endif	// _STORDEF_
