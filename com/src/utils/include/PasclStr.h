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
// @(#) 1.5 com/src/utils/include/PasclStr.h, odutils, od96os2, odos29646d 7/15/96 18:01:39 [ 11/15/96 15:29:34 ]
/*
	File:		PasclStr.h

	Contains:	Pascal string manipulation routines

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _PASCLSTR_
#define _PASCLSTR_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


#ifdef _PLATFORM_MACINTOSH_
#ifdef THINK_CPLUS
	#ifndef __pascal__
	#include <pascal.h>
	#endif
#else // THINK_CPLUS
	#ifndef __STRINGS__
	#include <strings.h>
	#endif
#endif // THINK_CPLUS

#ifndef __TYPES__
#include <types.h>
#endif

#else  // not Platform MacIntosh
  #ifndef _ODTYPESP_
  #include <ODTypesP.h>
  #endif
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif


#ifdef __cplusplus
	// These routines convert the string in place.
	
	inline StringPtr CToPascalString(char* string)
	{
		return c2pstr(string);
	}
	
	inline char* PascalToCString(StringPtr string)
	{
		return p2cstr(string);
	}

	extern "C" {

#else
	#define CToPascalString	c2pstr
	#define PascalToCString p2cstr
#endif


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

void ConcatPascalStrings(Str255 destString, ConstStr255Param srcString);
	// Error checking is done to ensure that the limit of 256 chars is not
	//	exceeded.

void CopyPascalString(Str255 destString, ConstStr255Param srcString);

ODBoolean EqualPascalStrings(ConstStr255Param str1, ConstStr255Param str2);
	// Returns kODTrue if strings are equal, kODFalse otherwise. Does a CASE-
	//	INSENSITIVE compare!

void CopyISOStr2PStr(Str255 destString, const ODISOStr srcString);

StringPtr IntlToPStr(ODIText* intlText, StringPtr pstr);
	// Caller must allocate storage for pstr. No error checking is done for
	//	strings longer than 255 chars. Simply grabs the text from the theText
	//	field of ODIText.
	//	CAUTION: This function is only intended to convert the bytes of the
	//	IText to a pascal string. These bytes are not valid without the
	//	accompanying script code and language code as well.

ODIText* PStrToIntl(StringPtr pstr, ODIText** intlText);
	// Caller must dispose storage for intlText when done with it. The ODIText
	//	is created with theScriptCode = smScriptRoman and
	//	theLangCode = langEnglish.  Storage for the ODIText is allocated only if
	//  intlText == NULL; otherwise it's assumed that *intlText is a valid ptr to
	//  an ODIText big enough to hold the string.
	//	CAUTION: This function is only intended to convert the bytes of the
	//	pascl string to the bytes in an ODIText. These bytes are not valid
	//	without first setting the appropriate accompanying script code and
	//	language code in the ODIText as well.

ODHandle PStrToText(ConstStr255Param pstr);
	// Caller must dispose of the returned ODHandle.

void TextToPStr(ODHandle textHandle, Str255 destString);

ODSShort ClipStringToBytes( Str255 string, ODSShort numBytes,
		ODScriptCode scriptCode );
	// Truncate a pascal string so that it occupies the given number of bytes
	// (plus the length byte) or one less.  The one less case occurs when
	// simple truncation would clip off the second byte of a double-byte
	// character.
	// returns the number of bytes the truncated string is long, ie numBytes or
	// numBytes - 1.

#ifdef __cplusplus
}
#endif

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif


#endif // _PASCLSTR_
