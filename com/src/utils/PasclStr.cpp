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
// @(#) 1.7 com/src/utils/PasclStr.cpp, odutils, od96os2, odos29646d 7/15/96 17:59:21 [ 11/15/96 15:28:51 ]
/*
	File:		PasclStr.cpp

	Contains:	Pascal string manipulation routines

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.
*/

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#include <ODPagtun.h>
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __MEMORY__
#include <Memory.h>
#endif

#ifndef __STRING__
#include <string.h>
#endif
#endif

#ifndef __CTYPE__
#include <ctype.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
  #ifndef __SCRIPT__
  #include <Script.h>
  #endif
#endif

#ifdef __SC__
	#ifndef __LANGUAGE__
	#include <Language.h>
	#endif
#endif // __SC__

#pragma segment ODPascalString

//==============================================================================
// Functions
//==============================================================================

void CopyPascalString(Str255 destString, ConstStr255Param srcString)
{
	ODBlockMove((ODPtr)srcString, (ODPtr)destString, srcString[0] + 1L);
}

void CopyISOStr2PStr(Str255 destString, const ODISOStr srcString)
{
	ODSize len = ODISOStrLength(srcString);
	ODBlockMove((ODPtr)srcString, &(destString[1]), (ODULong)len);
	destString[0] = (unsigned char) len;
}

void ConcatPascalStrings(Str255 destString, ConstStr255Param srcString)
{
	ODSShort maxSrcLength = 255 - destString[0];
	if ( srcString[0] > maxSrcLength )
	{
#ifdef _PLATFORM_MACINTOSH_
		Str255 tmpStr;
		ODBlockMove( srcString, tmpStr, srcString[0] + 1 );
		maxSrcLength = ClipStringToBytes( (StringPtr)srcString,
				maxSrcLength, smCurrentScript );
#endif
	}
	else
		maxSrcLength = srcString[0];

    ODBlockMove((ODPtr)(srcString + 1),
					(ODPtr)(destString + destString[0] + 1), maxSrcLength);
    destString[0] += maxSrcLength;
}

ODBoolean EqualPascalStrings(ConstStr255Param str1, ConstStr255Param str2)
{
	if (str1[0] != str2[0])
		return kODFalse;
	for (int i = 1; i <= str1[0]; i++)
	{
		if (tolower(str1[i]) != tolower(str2[i]))
			return kODFalse;
	}
	return kODTrue;
}

StringPtr IntlToPStr(ODIText* intlText, StringPtr pstr)
{
	return GetITextString(intlText,pstr);
}

ODIText* PStrToIntl(StringPtr pstr, ODIText** intlText)
{
	ODIText *result;
	if( intlText && *intlText ) {
		result = *intlText;
		SetITextString(result,pstr);
		SetITextScriptCode(result, smRoman);
		SetITextLangCode(result, langEnglish);
	} else {
		result = CreateIText(smRoman,langEnglish,pstr);
		if( intlText )
			*intlText = result;
	}
	return result;
}

//------------------------------------------------------------------------------
// PStrToText
//------------------------------------------------------------------------------

ODHandle PStrToText(ConstStr255Param pstr)
{
	ODHandle textHandle = kODNULL;
	
	if ( pstr )
	{
		textHandle = ODNewHandle(pstr[0]);
		if ( textHandle )
		{
			ODPtr textPtr = ODLockHandle(textHandle);
			ODBlockMove((ODPtr)&(pstr[1]), textPtr, pstr[0]);
			ODUnlockHandle(textHandle);
		}
	}

	return textHandle;
}

//------------------------------------------------------------------------------
// TextToPStr
//------------------------------------------------------------------------------

void TextToPStr(ODHandle textHandle, Str255 destString)
{
	if ( textHandle )
	{
#ifdef _PLATFORM_MACINTOSH_
		ODUShort textSize = GetHandleSize((Handle) textHandle);
#else
		ODUShort textSize = ODGetHandleSize(textHandle);
#endif
		if (textSize > 255)
			textSize = 255;
#ifdef _PLATFORM_MACINTOSH_
		ODBlockMove(*textHandle, (ODPtr) &destString[1], textSize);
#else
                ODBlockMove((ODPtr)*((Handle)textHandle), (ODPtr) &destString[1], textSize);
#endif
		destString[0] = textSize;
	}
	else
	{
		destString[0] = 0;
	}
}

//------------------------------------------------------------------------------
// ClipStringToBytes
//------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
ODSShort ClipStringToBytes( Str255 string, ODSShort numBytes,
		ODScriptCode scriptCode )
{
	// point at the proposed last character of the string.  Ask what it is.
	// If the answer is SecondByte, we're done.  If it's SingleByte, we're
	// also done.  If it's FirstByte, we back up one and then we're done.
	// Note that this use of CharacterByteType may cause it to erroneously
	// identify a secondByte as a singleByte, but that that's ok in this case
	// as both are treated the same.

	Ptr lastByte = (Ptr)&string[numBytes];
	short cbResult = CharacterByteType( lastByte, 0, scriptCode );
	if ( cbResult == smFirstByte )
		--numBytes;

	string[0] = numBytes;
	return numBytes;
}
#endif

