/* @(#)Z 1.2 com/src/include/cpp/ConstDef.h, odcore, od96os2, odos29646d 96/11/15 15:29:19 (96/07/15 18:14:45) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odcore
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
	File:		ConstDef.h

	Contains:	Global internal constants for the Macintosh implementation of
				OpenDoc

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	10/13/95	EL		1287340: Use standard ISO prefix
		 <5>	 4/26/95	TJ		Removed version Resources, they are now in
									ODVersion.r
		 <4>	 1/19/95	CC		1212833 Changed kODScrapTypePart to
									kODScrapTypeODBentoContainer.
		 <3>	 8/19/94	NP		1181622: Ownership fix.
		 <2>	 7/29/94	CG		Changed vers constants to 1.0a8.
		 <1>	 6/21/94	CC		first checked in
		------------------- CC ---- Converted to SOM
		 <7>	 4/18/94	T‚		a6
		 <6>	 3/28/94	CG		1150376: Changed version to force creation
									of new prefs files.
		 <5>	 3/23/94	CG		#1151393: Consts for use in vers resources.
		 <4>	 3/18/94	CC		Added defines for ISO string versions of
									Apple scrap types.  (1151636)
		 <3>	 3/17/94	CG		#1151392: Moved kOpenDocShellSignature to
									StdDefs.h
		 <2>	 1/18/94	CC		Added kXMPScrapTypePart.
		 <1>	 12/1/93	NP		first checked in

	To Do:
*/

#ifndef _CONSTDEF_
#define _CONSTDEF_


//==============================================================================
// Constants
//==============================================================================

// Scrap Types
#define kODScrapTypeODBentoContainer 'odcn'


// ISOString equivalents for Apple scrap types
#define		kODAppleTEXT	"+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:TEXT"
#define		kODApplestyl	"+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:styl"
#define		kODApplePICT	"+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:PICT"
#define		kODApplestxt	"+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:stxt"
#define		kODApplehfs		"+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:hfs "
#define		kODApplealis	"+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:alis"
#define		kODApplesect	"+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:sect"

#endif // _CONSTDEF_
