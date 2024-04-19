//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odmri
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
// @(#) 1.37 com/src/utils/StdTypIO.cpp, odmri, od96os2, odos29646d 10/30/96 10:04:21 [ 11/15/96 15:28:52 ]

/*
   File:           StdTypIO.cpp

   Contains:       functions for read/writing standard typed values from/to storage units

   Owned by:       Tantek ‚elik

   Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

   Theory of operation:

 The philosophy of StdTypIO is to give the client of the standard type IO functions
 flexibility with respect to where the data is stored while providing a standard for
 the data format used to store the types of data used in the various pieces of
 OpenDoc metadata in order to facilitate document exchange.

 The standard type IO functions are designed to be used independent of property &
 value type, and in some/most cases can even be used to manipulate data midstream
 (in the middle of values) by passing in a prefocused storageunit along with the
 offset set correctly, and passing in kODNULL for the prop and the val params.


*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ALTPOINT_
#include "AltPoint.h"
#endif

#ifndef _ALTPOLY_
#include "AltPoly.h"
#endif

#ifndef _ODEXCEPT_
#include <ODExcept.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
  #ifndef __SCRIPT__
  #include <Script.h>
  #endif
#endif

#ifndef _TEMPITER_
#include <TempIter.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef __TIME_H__
#include <time.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include "StdDefs.xh"
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include "StdTypes.xh"
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __OSUTILS__
#include <OSUtils.h>
#endif
#endif

#ifndef _FLIPEND_
#include <FlipEnd.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _UTILERRS_
#include <UtilErrs.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif


#include <errno.h>
#if defined(__IBMCPP__) || defined(_IBMR2)
// The following is for the iconv additions.
#include <iconv.h>
#include <nl_types.h>
#include <langinfo.h>
#endif

// This code is for error messages
#include <ODMessag.h>


//==============================================================================
// Constants
//==============================================================================

#define         kODBooleanLen           1
#define         kODUShortLen            2
#define         kODSShortLen            2
#define         kODULongLen                     4
#define         kODSLongLen                     4
#define         kODCoordinateLen        4
#define         kODPointLen                     (4*2)
#define         kODRectLen                      (4*4)
#define         kODStorageUnitRefLen 4

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
#define kIBMCodePagePrefix "IBM-"
#endif

#ifdef _PLATFORM_MACINTOSH_
#define useGMT 1
#else
#define useGMT 0
#endif

//==============================================================================
// Boolean
//==============================================================================

WIN32_DLLEXPORT ODBoolean       ODGetBooleanProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODBoolean value;
      StorageUnitGetValue(su, ev, sizeof(ODBoolean), (ODValue)&value);
      return (value ? kODTrue : kODFalse);
   }
   else {
      WARNMSG_DEBUG(WARN_INDEX(-1),"No Boolean Property.");
      return kODFalse;
   }
}

WIN32_DLLEXPORT void            ODSetBooleanProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODBoolean value)
{
   ODSUForceFocus(ev, su, prop, val);
   StorageUnitSetValue(su, ev, sizeof(ODBoolean), (ODValue)&value);
}

//==============================================================================
// Short
//==============================================================================

WIN32_DLLEXPORT ODUShort        ODGetUShortProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODUShort value;
      StorageUnitGetValue(su, ev, kODUShortLen, (ODValue)&value);
      return ConvertODUShortFromStd(value);
   }
   else {
      WARNMSG_DEBUG(WARN_INDEX(-1),"No UShort Property.");
      return 0;
   }
}

WIN32_DLLEXPORT void            ODSetUShortProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODUShort value)
{
   ODSUForceFocus(ev, su, prop, val);
   value = ConvertODUShortToStd(value);
   StorageUnitSetValue(su, ev, kODUShortLen, (ODValue)&value);
}


WIN32_DLLEXPORT ODSShort        ODGetSShortProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODSShort value;
      StorageUnitGetValue(su, ev, kODSShortLen, (ODValue)&value);
      return ConvertODSShortFromStd(value);
   }
   else {
      WARNMSG_DEBUG(WARN_INDEX(-1),"No SShort Property.");
      return 0;
   }
}

WIN32_DLLEXPORT void            ODSetSShortProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODSShort value)
{
   ODSUForceFocus(ev, su, prop, val);
   value = ConvertODSShortToStd(value);
   StorageUnitSetValue(su, ev, kODSShortLen, (ODValue)&value);
}


//==============================================================================
// Long
//==============================================================================

WIN32_DLLEXPORT ODULong         ODGetULongProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODULong value;
      StorageUnitGetValue(su, ev, kODULongLen, (ODValue)&value);
      return ConvertODULongFromStd(value);
   }
   else {
      //WARN("No ULong Property.");  // This is EXPECTED in many uses of ODGetULongProp, so this makes sense.
      return 0;
   }
}

WIN32_DLLEXPORT void            ODSetULongProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODULong value)
{
   ODSUForceFocus(ev, su, prop, val);
   value = ConvertODULongToStd(value);
   StorageUnitSetValue(su, ev, kODULongLen, (ODValue)&value);
}


WIN32_DLLEXPORT ODSLong         ODGetSLongProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODSLong value;
      StorageUnitGetValue(su, ev, kODSLongLen, (ODValue)&value);
      return ConvertODSLongFromStd(value);
   }
   else {
      WARNMSG_DEBUG(WARN_INDEX(-1),"No SLong Property.");
      return 0;
   }
}

WIN32_DLLEXPORT void            ODSetSLongProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODSLong value)
{
   ODSUForceFocus(ev, su, prop, val);
   value = ConvertODSLongToStd(value);
   StorageUnitSetValue(su, ev, kODSLongLen, (ODValue)&value);
}


//==============================================================================
// ISOStr
//==============================================================================

// In: The size parameter is the length of the longest ISOStr which can be written
// to the 'value' buffer.  This does not include the '\0' which is written to the buffer
// at the end.
// In other words, at most (*size)+1 bytes can be written to 'value' by ODGetISOStrProp.
// If the value parameter is NULL, then an ISOStr is allocated and for you and returned.
// Out: The size parameter is the length of the ISOStr which was read in.
// If there is no value at the passed in prop & val then kODNULL is returned.
// This is consistent with the other routines in StdTypIO which return pointers.
// The reason is that there are clients of ODGetISOStrProp which more easily handle the
// flow of control if it just returns kODNULL instead of THROWing. -Tantek
// One such client: ODGetKindFromPartSU in InfoUtil.

WIN32_DLLEXPORT ODISOStr        ODGetISOStrProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODISOStr value, ODULong* size)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODULong valueSize = su->GetSize(ev);
      if (valueSize == 0)
         valueSize++;    /* just to be absolutely safe */
      if (!value)
      {
         value = (ODISOStr)ODNewPtr(valueSize);
      }
      else
      {
         if (size!=kODNULL && valueSize > *size)
            valueSize = *size;
      }
      ODULong length = valueSize - 1;
      StorageUnitGetValue(su, ev, length, (ODValue)value);
      value[length] = '\0';
      if (size!=kODNULL)
         *size = length;
      return value;
   }
   else
   {
      if (size!=kODNULL)
         *size = 0;
      return kODNULL;
   }
}

WIN32_DLLEXPORT void            ODSetISOStrProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODISOStr value)
{
   ODSUForceFocus(ev, su, prop, val);
   ODULong oldsize = su->GetSize(ev);
   ODULong newsize = strlen(value)+1;
   StorageUnitSetValue(su, ev, newsize, (ODValue)value);
   if (oldsize > newsize)
      su->DeleteValue(ev, oldsize - newsize);
}


//==============================================================================
// TypeList
//==============================================================================
//
// Aan ODTypeList property value containing n elements begins with (n+1) offsets,
// followed by n ISO strings with their null termination.  The first n offsets
// identify the starting positions of the corresponding ISO string.  The last
// offset is always equal to the size of the value, and is immediately before
// the first character of the first ISO string (its actually redundant)
// For example, a property value representing an empty ODTypeList will be
// four bytes long and contain offset four, signifying that there are no ISO
// strings present.

WIN32_DLLEXPORT void ODGetTypeListProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODTypeList* typeList)
{
   // If an error is thrown, typeList may be partially updated.

   if ( ODSUExistsThenFocus(ev, su, prop, val) )
   {
      ODULong valueSize = su->GetSize(ev);
      if ( valueSize >= kODULongLen )
      {
         ODULong index = 0;
         ODULong offset;
         ODULong nextOffset;

         su->SetOffset(ev, 0);
         StorageUnitGetValue(su, ev, kODULongLen, (ODValue) &offset);

         while ( offset < valueSize )
         {
            su->SetOffset(ev, (index+1)*kODULongLen);
            StorageUnitGetValue(su, ev, kODULongLen, (ODValue) &nextOffset);

            su->SetOffset(ev, offset);

            TempODType kind = (ODType) ODNewPtr(nextOffset-offset);;

            StorageUnitGetValue(su, ev, nextOffset-offset, (ODValue) kind);
            typeList->AddLast(ev, kind);

            offset = nextOffset;
            ++index;
         }
      }
   }
   else
   {
      WARNMSG_DEBUG(WARN_INDEX(-1),"No TypeList Property.");
   }
}

WIN32_DLLEXPORT void ODSetTypeListProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODTypeList* typeList)
{
   // If an error is thrown, the property value will be truncated to zero length.

   ODSUForceFocus(ev, su, prop, val);
   ODULong oldsize = su->GetSize(ev);

   ODType kind = kODNULL;
   ODULong index = 0;
   ODULong offset = (typeList->Count(ev)+1)*kODULongLen;

   try {

      // Write the offset to the first Type value and placeholders for
      // the remaining offsets to be written later
      su->SetOffset(ev, 0);
      while ( su->GetSize(ev) < offset )
         StorageUnitSetValue(su, ev, kODULongLen, (ODValue) &offset);

      for( TempODTypeListIterator iter(ev,typeList); iter; iter.Next() )
      {
         kind = iter.Current();
         su->SetOffset(ev, offset);
         ODULong size = strlen(kind)+1;

         StorageUnitSetValue(su, ev, size, (ODValue) kind);
         offset += size;
         ++index;
         su->SetOffset(ev, index*kODULongLen);
         StorageUnitSetValue(su, ev, kODULongLen, (ODValue) &offset);

         ODDisposePtr(kind);
      }

   } catch (ODException _exception) {

      ODDisposePtr(kind);
      su->SetOffset(ev, 0);
      su->DeleteValue(ev, oldsize);
      throw;

   }

   if (oldsize > offset)
   {
      su->SetOffset(ev, offset);
      su->DeleteValue(ev, oldsize - offset);
   }
}

//==============================================================================
// IText
//==============================================================================


// TABLE ONLY HANDLES MACINTOSH EXTENDED CHARACTER SET FOR THE MacRoman SCRIPT,
//      THE langEngligh LANGUAGE, AND A "NORMAL" FONT (I.E. NOT THE "SYMBOL" FONT
//      OR A DINGBAT FONT). PROBABLY VALID FOR OTHER LANGUAGES AS WELL. - NP

// NOTE THAT MAC CHARACTERS 0x0000 - 0x007F ARE MAPPED THE SAME UNICODE
//      CHARACTERS. DIFFERENCES APPEAR FROM 0x0080 - 0x00FF

ODUShort MacToUniTable[256] =
{
/* 0x0000 */  0x0000,
/* 0x0001 */  0x0001,
/* 0x0002 */  0x0002,
/* 0x0003 */  0x0003,
/* 0x0004 */  0x0004,
/* 0x0005 */  0x0005,
/* 0x0006 */  0x0006,
/* 0x0007 */  0x0007,
/* 0x0008 */  0x0008,
/* 0x0009 */  0x0009,
/* 0x000A */  0x000A,
/* 0x000B */  0x000B,
/* 0x000C */  0x000C,
/* 0x000D */  0x000D,
/* 0x000E */  0x000E,
/* 0x000F */  0x000F,
/* 0x0010 */  0x0010,
/* 0x0011 */  0x0011,
/* 0x0012 */  0x0012,
/* 0x0013 */  0x0013,
/* 0x0014 */  0x0014,
/* 0x0015 */  0x0015,
/* 0x0016 */  0x0016,
/* 0x0017 */  0x0017,
/* 0x0018 */  0x0018,
/* 0x0019 */  0x0019,
/* 0x001A */  0x001A,
/* 0x001B */  0x001B,
/* 0x001C */  0x001C,
/* 0x001D */  0x001D,
/* 0x001E */  0x001E,
/* 0x001F */  0x001F,
/* 0x0020 ' ' */  ' ',
/* 0x0021 '!' */  '!',
/* 0x0022 '"' */  '"',
/* 0x0023 '#' */  '#',
/* 0x0024 '$' */  '$',
/* 0x0025 '%' */  '%',
/* 0x0026 '&' */  '&',
/* 0x0027 '\'' */  '\'',
/* 0x0028 '(' */  '(',
/* 0x0029 ')' */  ')',
/* 0x002A '*' */  '*',
/* 0x002B '+' */  '+',
/* 0x002C ',' */  ',',
/* 0x002D '-' */  '-',
/* 0x002E '.' */  '.',
/* 0x002F '/' */  '/',
/* 0x0030 '0' */  '0',
/* 0x0031 '1' */  '1',
/* 0x0032 '2' */  '2',
/* 0x0033 '3' */  '3',
/* 0x0034 '4' */  '4',
/* 0x0035 '5' */  '5',
/* 0x0036 '6' */  '6',
/* 0x0037 '7' */  '7',
/* 0x0038 '8' */  '8',
/* 0x0039 '9' */  '9',
/* 0x003A ':' */  ':',
/* 0x003B ';' */  ';',
/* 0x003C '<' */  '<',
/* 0x003D '=' */  '=',
/* 0x003E '>' */  '>',
/* 0x003F '?' */  '?',
/* 0x0040 '@' */  '@',
/* 0x0041 'A' */  'A',
/* 0x0042 'B' */  'B',
/* 0x0043 'C' */  'C',
/* 0x0044 'D' */  'D',
/* 0x0045 'E' */  'E',
/* 0x0046 'F' */  'F',
/* 0x0047 'G' */  'G',
/* 0x0048 'H' */  'H',
/* 0x0049 'I' */  'I',
/* 0x004A 'J' */  'J',
/* 0x004B 'K' */  'K',
/* 0x004C 'L' */  'L',
/* 0x004D 'M' */  'M',
/* 0x004E 'N' */  'N',
/* 0x004F 'O' */  'O',
/* 0x0050 'P' */  'P',
/* 0x0051 'Q' */  'Q',
/* 0x0052 'R' */  'R',
/* 0x0053 'S' */  'S',
/* 0x0054 'T' */  'T',
/* 0x0055 'U' */  'U',
/* 0x0056 'V' */  'V',
/* 0x0057 'W' */  'W',
/* 0x0058 'X' */  'X',
/* 0x0059 'Y' */  'Y',
/* 0x005A 'Z' */  'Z',
/* 0x005B '[' */  '[',
/* 0x005C '\\' */  '\\',
/* 0x005D ']' */  ']',
/* 0x005E '^' */  '^',
/* 0x005F '_' */  '_',
/* 0x0060 '`' */  '`',
/* 0x0061 'a' */  'a',
/* 0x0062 'b' */  'b',
/* 0x0063 'c' */  'c',
/* 0x0064 'd' */  'd',
/* 0x0065 'e' */  'e',
/* 0x0066 'f' */  'f',
/* 0x0067 'g' */  'g',
/* 0x0068 'h' */  'h',
/* 0x0069 'i' */  'i',
/* 0x006A 'j' */  'j',
/* 0x006B 'k' */  'k',
/* 0x006C 'l' */  'l',
/* 0x006D 'm' */  'm',
/* 0x006E 'n' */  'n',
/* 0x006F 'o' */  'o',
/* 0x0070 'p' */  'p',
/* 0x0071 'q' */  'q',
/* 0x0072 'r' */  'r',
/* 0x0073 's' */  's',
/* 0x0074 't' */  't',
/* 0x0075 'u' */  'u',
/* 0x0076 'v' */  'v',
/* 0x0077 'w' */  'w',
/* 0x0078 'x' */  'x',
/* 0x0079 'y' */  'y',
/* 0x007A 'z' */  'z',
/* 0x007B '{' */  '{',
/* 0x007C '|' */  '|',
/* 0x007D '}' */  '}',
/* 0x007E '~' */  '~',
/* 0x007F */  0x007F,

/* 0x0080 */  0x00C4,   // LATIN CAPITAL LETTER A DIAERESIS
/* 0x0081 */  0x00C5,   // LATIN CAPITAL LETTER A RING
/* 0x0082 */  0x00C7,   // LATIN CAPITAL LETTER C CEDILLA
/* 0x0083 */  0x00C9,   // LATIN CAPITAL LETTER E ACUTE
/* 0x0084 */  0x00D1,   // LATIN CAPITAL LETTER N TILDE
/* 0x0085 */  0x00D6,   // LATIN CAPITAL LETTER O DIAERESIS
/* 0x0086 */  0x00DC,   // LATIN CAPITAL LETTER U DIAERESIS
/* 0x0087 */  0x00E1,   // LATIN SMALL LETTER A ACUTE
/* 0x0088 */  0x00E0,   // LATIN SMALL LETTER A GRAVE
/* 0x0089 */  0x00E2,   // LATIN SMALL LETTER A CIRCUMFLEX
/* 0x008A */  0x00E4,   // LATIN SMALL LETTER A DIAERESIS
/* 0x008B */  0x00E3,   // LATIN SMALL LETTER A TILDE
/* 0x008C */  0x00E5,   // LATIN SMALL LETTER A RING ABOVE
/* 0x008D */  0x00E7,   // LATIN SMALL LETTER C CEDILLA
/* 0x008E */  0x00E9,   // LATIN SMALL LETTER E ACUTE
/* 0x008F */  0x00E8,   // LATIN SMALL LETTER E GRAVE
/* 0x0090 */  0x00EA,   // LATIN SMALL LETTER E CIRCUMFLEX
/* 0x0091 */  0x00EB,   // LATIN SMALL LETTER E DIAERESIS
/* 0x0092 */  0x00ED,   // LATIN SMALL LETTER I ACUTE
/* 0x0093 */  0x00EC,   // LATIN SMALL LETTER I GRAVE
/* 0x0094 */  0x00EE,   // LATIN SMALL LETTER I CIRCUMFLEX
/* 0x0095 */  0x00EF,   // LATIN SMALL LETTER I DIAERESIS
/* 0x0096 */  0x00F1,   // LATIN SMALL LETTER N TILDE
/* 0x0097 */  0x00F3,   // LATIN SMALL LETTER O ACUTE
/* 0x0098 */  0x00F2,   // LATIN SMALL LETTER O GRAVE
/* 0x0099 */  0x00F4,   // LATIN SMALL LETTER O CIRCUMFLEX
/* 0x009A */  0x00F6,   // LATIN SMALL LETTER O DIAERESIS
/* 0x009B */  0x00F5,   // LATIN SMALL LETTER O TILDE
/* 0x009C */  0x00FA,   // LATIN SMALL LETTER U ACUTE
/* 0x009D */  0x00F9,   // LATIN SMALL LETTER U GRAVE
/* 0x009E */  0x00FB,   // LATIN SMALL LETTER U CIRCUMFLEX
/* 0x009F */  0x00FC,   // LATIN SMALL LETTER U DIAERESIS
/* 0x00A0 */  0x2020,   // DAGGER
/* 0x00A1 */  0x00B0,   // DEGREE SIGN
/* 0x00A2 */  0x00A2,   // CENT SIGN
/* 0x00A3 */  0x00A3,   // POUND SIGN
/* 0x00A4 */  0x00A7,   // SECTION SIGN
/* 0x00A5 */  0x2022,   // BULLET
/* 0x00A6 */  0x00B6,   // PILCROW SIGN
/* 0x00A7 */  0x00DF,   // LATIN SMALL LETTER SHARP S
/* 0x00A8 */  0x00AE,   // REGISTERED SIGN
/* 0x00A9 */  0x00A9,   // COPYRIGHT SIGN
/* 0x00AA */  0x2122,   // TRADE MARK SIGN
/* 0x00AB */  0x00B4,   // ACUTE ACCENT
/* 0x00AC */  0x00A8,   // DIAERESIS
/* 0x00AD */  0x2260,   // NOT EQUAL TO
/* 0x00AE */  0x00C6,   // LATIN CAPITAL LETTER LIGATURE A E
/* 0x00AF */  0x00D8,   // LATIN CAPITAL LETTER O STROKE
/* 0x00B0 */  0x221E,   // INFINITY
/* 0x00B1 */  0x00B1,   // PLUS-MINUS SIGN
/* 0x00B2 */  0x2264,   // LESS-THAN OR EQUAL TO
/* 0x00B3 */  0x2265,   // GREATER-THAN OR EQUAL TO
/* 0x00B4 */  0x00A5,   // YEN SIGN
/* 0x00B5 */  0x00B5,   // MICRO SIGN
/* 0x00B6 */  0x2202,   // PARTIAL DIFFERENTIAL
/* 0x00B7 */  0x2211,   // N-ARY SUMMATION
/* 0x00B8 */  0x220F,   // N-ARY PRODUCT
/* 0x00B9 */  0x03C0,   // GREEK SMALL LETTER PI
/* 0x00BA */  0x222B,   // INTEGRAL
/* 0x00BB */  0x00AA,   // FEMININE ORDINAL INDICATOR
/* 0x00BC */  0x00BA,   // MASCULINE ORDINAL INDICATOR
/* 0x00BD */  0x2126,   // OHM SIGN
/* 0x00BE */  0x00E6,   // LATIN SMALL LETTER LIGATURE A E
/* 0x00BF */  0x00F8,   // LATIN SMALL LETTER O SLASH
/* 0x00C0 */  0x00BF,   // INVERTED QUESTION MARK
/* 0x00C1 */  0x00A1,   // INVERTED EXCLAMATION MARK
/* 0x00C2 */  0x00AC,   // NOT SIGN
/* 0x00C3 */  0x221A,   // SQUARE ROOT
/* 0x00C4 */  0x0192,   // LATIN SMALL LETTER F HOOK
/* 0x00C5 */  0x2248,   // ALMOST EQUAL TO
/* 0x00C6 */  0x2206,   // INCREMENT
/* 0x00C7 */  0x00AB,   // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
/* 0x00C8 */  0x00BB,   // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
/* 0x00C9 */  0x2026,   // HORIZONTAL ELLIPSIS
/* 0x00CA */  0x00A0,   // NO-BREAK SPACE
/* 0x00CB */  0x00C0,   // LATIN CAPITAL LETTER A GRAVE
/* 0x00CC */  0x00C3,   // LATIN CAPITAL LETTER A TILDE
/* 0x00CD */  0x00D5,   // LATIN CAPITAL LETTER O TILDE
/* 0x00CE */  0x0152,   // LATIN CAPITAL LETTER LIGATURE O E
/* 0x00CF */  0x0153,   // LATIN SMALL LETTER LIGATURE O E
/* 0x00D0 */  0x2013,   // EN DASH
/* 0x00D1 */  0x2014,   // EM DASH
/* 0x00D2 */  0x201C,   // LEFT DOUBLE QUOTATION MARK
/* 0x00D3 */  0x201D,   // RIGHT DOUBLE QUOTATION MARK
/* 0x00D4 */  0x2018,   // LEFT SINGLE QUOTATION MARK
/* 0x00D5 */  0x2019,   // RIGHT SINGLE QUOTATION MARK
/* 0x00D6 */  0x00F7,   // DIVISION SIGN
/* 0x00D7 */  0x25CA,   // LOZENGE
/* 0x00D8 */  0x00FF,   // LATIN SMALL LETTER Y DIAERESIS
/* 0x00D9 */  0x0178,   // LATIN CAPITAL LETTER Y DIAERESIS
/* 0x00DA */  0x2044,   // FRACTION SLASH
/* 0x00DB */  0x00A4,   // CURRENCY SIGN
/* 0x00DC */  0x2039,   // SINGLE LEFT-POINTING ANGLE QUOTE MARK
/* 0x00DD */  0x203A,   // SINGLE RIGHT-POINTING ANGLE QUOTE MARK
/* 0x00DE */  0xFB01,   // LATIN SMALL LETTER LIGATURE FI
/* 0x00DF */  0xFB02,   // LATIN SMALL LETTER LIGATURE FL
/* 0x00E0 */  0x2021,   // DOUBLE DAGGER
/* 0x00E1 */  0x00B7,   // MIDDLE DOT
/* 0x00E2 */  0x201A,   // SINGLE LOW-9 QUOTATION MARK
/* 0x00E3 */  0x201E,   // DOUBLE LOW-9 QUOTATION MARK
/* 0x00E4 */  0x2030,   // PER MILLE SIGN
/* 0x00E5 */  0x00C2,   // LATIN CAPITAL LETTER A CIRCUMFLEX
/* 0x00E6 */  0x00CA,   // LATIN CAPITAL LETTER E CIRCUMFLEX
/* 0x00E7 */  0x00C1,   // LATIN CAPITAL LETTER A ACUTE
/* 0x00E8 */  0x00CB,   // LATIN CAPITAL LETTER E DIAERESIS
/* 0x00E9 */  0x00C8,   // LATIN CAPITAL LETTER E GRAVE
/* 0x00EA */  0x00CD,   // LATIN CAPITAL LETTER I ACUTE
/* 0x00EB */  0x00CE,   // LATIN CAPITAL LETTER I CIRCUMFLEX
/* 0x00EC */  0x00CF,   // LATIN CAPITAL LETTER I DIAERESIS
/* 0x00ED */  0x00CC,   // LATIN CAPITAL LETTER I GRAVE
/* 0x00EE */  0x00D3,   // LATIN CAPITAL LETTER O ACUTE
/* 0x00EF */  0x00D4,   // LATIN CAPITAL LETTER O CIRCUMFLEX
/* 0x00F0 */  0xF8FF,   // APPLE LOGO
/* 0x00F1 */  0x00D2,   // LATIN CAPITAL LETTER O GRAVE
/* 0x00F2 */  0x00DA,   // LATIN CAPITAL LETTER U ACUTE
/* 0x00F3 */  0x00DB,   // LATIN CAPITAL LETTER U CIRCUMFLEX
/* 0x00F4 */  0x00D9,   // LATIN CAPITAL LETTER U GRAVE
/* 0x00F5 */  0x0131,   // LATIN SMALL LETTER DOTLESS I
/* 0x00F6 */  0x02C6,   // MODIFIER LETTER CIRCUMFLEX ACCENT
/* 0x00F7 */  0x02DC,   // SMALL TILDE
/* 0x00F8 */  0x00AF,   // MACRON
/* 0x00F9 */  0x02D8,   // BREVE
/* 0x00FA */  0x02D9,   // DOT ABOVE
/* 0x00FB */  0x02DA,   // RING ABOVE
/* 0x00FC */  0x00B8,   // CEDILLA
/* 0x00FD */  0x02DD,   // DOUBLE ACUTE ACCENT
/* 0x00FE */  0x02DB,   // OGONEK
/* 0x00FF */  0x02C7,   // CARON
};

//------------------------------------------------------------------------------
// TradMacTextToUnicode
//
//      Using mapping table, convert characters. Note, we make no attempt to handle
//      double-byte characters!
//
//      uniCodeBufferLength is expressed in BYTES!
//
//      Storage will be allocated for *unicodeText with ODNewPtr.
//------------------------------------------------------------------------------

void TradMacTextToUnicode(ODUByte* macText, ODULong macTextLength,
                  ODUShort** unicodeText, ODULong* uniCodeBufferLength);

void TradMacTextToUnicode(ODUByte* macText, ODULong macTextLength,
                  ODUShort** unicodeText, ODULong* uniCodeBufferLength)
{
/*

#if defined(_IBMR2) || defined(__IBMCPP__)

should be for both AIX(IBMR2) and VAC++ but AIX has some problem with
passing null strings to iconv. The proper fix is to check that a null 
string is not passed to iconv, but for now (10/30/96) I'll use iconv
for VAC++ only.

*/
#if defined(__IBMCPP__)

   ODUShort*       uniCodeTextPtr;
   *uniCodeBufferLength = macTextLength * 2;
   *unicodeText = uniCodeTextPtr = (ODUShort*)ODNewPtr(*uniCodeBufferLength);
//==========================================================================
   ODUByte* macTextPtr = macText;

//==========================================================================
// Query current codepage and build it to pass to converters
//==========================================================================


   char* fromcode=nl_langinfo(CODESET);
//==========================================================================
   const char tocode[] = "UCS-2";
   iconv_t    cd;
   errno = 0;  // reset the errno value
   int gotCd=1; // a flag to indicate if the iconv_open got a converter descriptor.

      // *** convert to unicode
      if ((iconv_t)(-1) == (cd = iconv_open(tocode, fromcode))) {

        #if defined(_PLATFORM_WIN32) || defined(_PLATFORM_AIX)
          WARNMSG(WARN_INDEX(AMSG_851),"Failed to iconv_open %s to %s.\n", fromcode, tocode);
        #endif
          gotCd=0;
          *unicodeText = NULL;   // set to null because nothing was converted
          return;
      }

    if (gotCd) {
   /* Convert characters in array from current codepage to Unicode. */
    #if defined(_PLATFORM_WIN32_)
       if ((size_t)(-1) == iconv(cd, (char**)&macTextPtr, (size_t*)&macTextLength, (char**)&uniCodeTextPtr, (size_t*)uniCodeBufferLength)) {
    #endif
    #if defined(_PLATFORM_OS2_)
       if ((size_t)(-1) == iconv(cd, (unsigned char**)&macTextPtr, (size_t*)&macTextLength, (unsigned char**)&uniCodeTextPtr, (size_t*)uniCodeBufferLength)) {
    #endif
    #if defined(_PLATFORM_UNIX_)
       if ((size_t)(-1) == iconv(cd, (const char**)&macTextPtr, (size_t*)&macTextLength, (char**)&uniCodeTextPtr, (size_t*)uniCodeBufferLength)) {
    #endif // !defined(_PLATFORM_UNIX_)
           switch(errno)
              {
              case EILSEQ:
                WARNMSG(WARN_INDEX(AMSG_852),"iconv returned - EILSEQ: An input byte does not belong to the input codeset.\n");
              case E2BIG:
                WARNMSG(WARN_INDEX(AMSG_853),"iconv returned - E2BIG: outbuf is not large enough to hold the converted value.\n");
              case EINVAL:
                WARNMSG(WARN_INDEX(AMSG_854),"iconv returned - EINVAL: Incomplete character shift sequence at the end of the input buffer.\n");
              case EBADF:
                WARNMSG(WARN_INDEX(AMSG_855),"iconv returned - EBADF: cd is not a valid conversion descriptor.\n");
              }
           WARNMSG(WARN_INDEX(AMSG_856),"Fail to convert the characters to unicode set.\n");
           *unicodeText = NULL; // set to null because nothing was converted
           return;
       }

     //===========================================================================
     // close the iconv converter
     //===========================================================================
       if (-1 == iconv_close(cd)) {
           WARNMSG(WARN_INDEX(AMSG_857),"Fail to iconv_close.\n");
           return;
       }
     } else {
        //use the table
           for (ODULong i = 0; i < macTextLength; i++)
              uniCodeTextPtr[i] = MacToUniTable[macText[i]];
     } /* endif */

#else //It's not AIX's Xlc and it's not VAC++
   #ifdef _PLATFORM_WIN32_
    ODUShort*       unicodeTextPtr;

    *uniCodeBufferLength = macTextLength * 2;
    *unicodeText = unicodeTextPtr = (ODUShort*)ODNewPtr(*uniCodeBufferLength);

    UINT codePage=GetOEMCP();

    LPCSTR source = (LPCSTR)macText;
    LPWSTR target = (LPWSTR)unicodeTextPtr;

    int rc=MultiByteToWideChar(
      codePage,         // code page
      MB_ERR_INVALID_CHARS,   // Return error if an invalid character is encountered
      source,             // address of string to map
      macTextLength,      // number of characters in string
      target,             // address of wide-character buffer
      *uniCodeBufferLength // size of buffer
      );

    if (rc==0) { //failed to convert, so use the trusty table.
       for (ODULong i = 0; i < macTextLength; i++)
          unicodeTextPtr[i] = MacToUniTable[macText[i]];
    }
   #else
   ODUShort*       uniCodeTextPtr;

   *uniCodeBufferLength = macTextLength * 2;
   *unicodeText = uniCodeTextPtr = (ODUShort*)ODNewPtr(*uniCodeBufferLength);
   for (ODULong i = 0; i < macTextLength; i++)
      uniCodeTextPtr[i] = MacToUniTable[macText[i]];
   #endif
#endif
}


//------------------------------------------------------------------------------
// UnicodeToTradMacText
//
//      Using mapping table, convert characters. Note, we make no attempt to handle
//      most unicode characters above 0x00FF. Most will be mapped to '*'!
//
//      uniCodeTextLength is expressed in BYTES!
//
//      uniCodeTextLength must be even!
//
//      Storage will be allocated for *macText with ODNewPtr.
//------------------------------------------------------------------------------

void UnicodeToTradMacText(ODUShort* unicodeText, ODULong uniCodeTextLength,
                  ODUByte** macText, ODULong* macTextLength);

 void UnicodeToTradMacText(ODUShort* unicodeText, ODULong uniCodeTextLength,
                  ODUByte** macText, ODULong* macTextLength)
{
#if defined(_IBMR2) || defined(__IBMCPP__)
   ODUByte*        macTextPtr;
   ODULong         numUnicodeChars = *macTextLength = uniCodeTextLength;
   *macText = macTextPtr = (ODUByte*)ODNewPtr(numUnicodeChars);

//==========================================================================
//       The following code uses VAC++ iconv utility to convert from
//       Unicode to the current code page running on this OS.
//==========================================================================
//==========================================================================
// Query current codepage and build it to pass to converters
//==========================================================================

   char* tocode=nl_langinfo(CODESET);
//==========================================================================
   const char fromcode[] = "UCS-2";
   iconv_t    cd;
   char* unicodeTextPtr = (char*)unicodeText;
   int gotCd=1; // a flag to indicate if the iconv_open got a converter descriptor.

   // *** open converter for code page conversion from unicode
   if ((iconv_t)(-1) == (cd = iconv_open(tocode,fromcode))) {
        #if defined(_PLATFORM_WIN32) || defined(_PLATFORM_AIX)
          WARNMSG(WARN_INDEX(AMSG_851),"Failed to iconv_open %s to %s.\n", fromcode, tocode);
        #endif
          gotCd=0;
          *macText = kODNULL; // set to null if error
          return;
   }

   /* Convert characters in array from Unicode to current codepage. */

    if (gotCd) {
       #if defined(_PLATFORM_WIN32_)
         if ((size_t)(-1) == iconv(cd, (char**)&unicodeTextPtr, (size_t*)&uniCodeTextLength, (char**)&macTextPtr, (size_t*)macTextLength)) {
       #endif
       #if defined(_PLATFORM_OS2_)
         if ((size_t)(-1) == iconv(cd, (unsigned char**)&unicodeTextPtr, (size_t*)&uniCodeTextLength, (unsigned char**)&macTextPtr, (size_t*)macTextLength)) {
       #endif
       #if defined(_PLATFORM_UNIX_)
         if ((size_t)(-1) == iconv(cd, (const char**)&unicodeTextPtr, (size_t*)&uniCodeTextLength, (char**)&macTextPtr, (size_t*)macTextLength)) {
       #endif // !defined(_PLATFORM_UNIX_)
           switch(errno)
              {
              case EILSEQ:
                WARNMSG(WARN_INDEX(AMSG_852),"iconv returned - EILSEQ: An input byte does not belong to the input codeset.\n");
              case E2BIG:
                WARNMSG(WARN_INDEX(AMSG_853),"iconv returned - E2BIG: outbuf is not large enough to hold the converted value.\n");
              case EINVAL:
                WARNMSG(WARN_INDEX(AMSG_854),"iconv returned - EINVAL: Incomplete character shift sequence at the end of the input buffer.\n");
               case EBADF:
                WARNMSG(WARN_INDEX(AMSG_855),"iconv returned - EBADF: cd is not a valid conversion descriptor.\n");
              }
           WARNMSG(WARN_INDEX(AMSG_856),"Fail to convert the characters to unicode set.\n");
           *macText = NULL; // set to null because nothing was converted
           return;
       }
       *macTextPtr = '\0'; // must put the terminating NULL on the end.
       *macTextLength = *macTextLength +1; // add one to Length for NULL

   //===========================================================================
   // close the iconv converter
   //===========================================================================
       if (-1 == iconv_close(cd)) {
           WARNMSG(WARN_INDEX(AMSG_857),"Fail to iconv_close.\n");
          return;
          }

     } else {
        //use the table
        for (ODULong i = 0; i < numUnicodeChars; i++)
        {
          // CAN MAP 0x0000 - 0x007F ONE TO ONE.
          if (unicodeText[i] <= 0x007F)
            macTextPtr[i] = unicodeText[i];
          else // LINEAR SEARCH FOR NOW! THIS CODE SHOULD BE REPLACED - NP 1234657
          {
            ODBoolean       foundChar = kODFalse;

            for (int tableIndex = 0;
                  tableIndex < sizeof(MacToUniTable);
                  tableIndex++)
            {
               if (MacToUniTable[tableIndex] == unicodeText[i])
               {
                  macTextPtr[i] = tableIndex;
                  foundChar = kODTrue;
                  break;
               }
            }
            if (!foundChar)
               macTextPtr[i] = '*'; // SUBSITUTE ASTERISK FOR UNKNOWN UNICODE
                                                     //      CHARACTER.
          }
        } /* end for */
     } /* endif */
#else // if !defined(_IBMR2) && !defined(__IBMCPP__)

   ODUByte*        macTextPtr;
   ODULong         numUnicodeChars = *macTextLength = uniCodeTextLength / 2;

   *macText = macTextPtr = (ODUByte*)ODNewPtr(numUnicodeChars);

#ifdef _PLATFORM_WIN32_

   int rc=WideCharToMultiByte(
    CP_OEMCP,                // code page
    (DWORD)0,                // performance and mapping flags
    (LPCWSTR)unicodeText,    // address of wide-character string
    (int) uniCodeTextLength, // number of characters in string
    (LPSTR)macTextPtr,       // address of buffer for new string
    (int) *macTextLength,    // size of mbs buffer
    NULL,                    // use system default for unmappable characters
    NULL);                   // don't bother to set a flag if default char. used

    if (rc==0)  { //if it fails, look it up in a table.
#endif


   for (ODULong i = 0; i < numUnicodeChars; i++)
   {
      // CAN MAP 0x0000 - 0x007F ONE TO ONE.
      if (unicodeText[i] <= 0x007F)
         macTextPtr[i] = unicodeText[i];
      else // LINEAR SEARCH FOR NOW! THIS CODE SHOULD BE REPLACED - NP 1234657
      {
         ODBoolean       foundChar = kODFalse;

         for (int tableIndex = 0;
               tableIndex < sizeof(MacToUniTable);
               tableIndex++)
         {
            if (MacToUniTable[tableIndex] == unicodeText[i])
            {
               macTextPtr[i] = tableIndex;
               foundChar = kODTrue;
               break;
            }
         }
         if (!foundChar)
            macTextPtr[i] = '*'; // SUBSITUTE ASTERISK FOR UNKNOWN UNICODE
                                                  //      CHARACTER.
      }
   }

#ifdef _PLATFORM_WIN32_
  } //end of that if (rc==0).
#endif
#endif // if defined(_IBMR2) || defined(__IBMCPP__)
}

//------------------------------------------------------------------------------
// UnicodeToIText
//
//      IText will be filled in. If iText is kODNULL, it will be created.
//------------------------------------------------------------------------------

ODIText* UnicodeToIText(ODIText* iText, ODUShort* unicodeText,
                  ODULong unicodeTextLength);
ODIText* UnicodeToIText(ODIText* iText, ODUShort* unicodeText,
                  ODULong unicodeTextLength)
{
   ODUByte* text;
   ODULong  textSize;

   UnicodeToTradMacText(unicodeText, unicodeTextLength, &text, &textSize);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
   if(!text)  // UnicodeToIText returns NULL if iconv fails
      return(0);  // return null
#endif
   try {
      if (iText)
      {
         ODScriptCode scriptValue;
#ifdef _PLATFORM_WIN32_
// For Windows use the special Microsoft API.
        scriptValue = GetOEMCP();
#endif

#ifdef _PLATFORM_MACINTOSH_
//For Mac hardcode Roman script
        scriptValue=smRoman;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
//For every one else use the standard XPG/4 API.
        char* codeset=nl_langinfo(CODESET);

//Convert the string to a numeric value
        char* numberPart; //The number part of the code set string.

           if (strstr(codeset, kIBMCodePagePrefix)) {
              numberPart=codeset+strlen(kIBMCodePagePrefix);
              scriptValue=atoi(numberPart);
           } else {
              //strstr returned null, "IBM-" is not a part of codeset.
              //use a default value.
              scriptValue=0;
           }

#endif
         SetITextScriptCode(iText, scriptValue);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
// On Intel and AIX platforms code page information is all that is needed. The langcode
// is set to 0.
         SetITextLangCode(iText, 0);
#else
         SetITextLangCode(iText, langEnglish);
#endif

         SetITextText(iText, text, textSize);
      }
      else
         iText = CreateITextFromCharPtr((char*)text);
   } catch (ODException _exception) {
      ODDisposePtr(text);
   }

   ODDisposePtr(text);

   return iText;

}


const ODULong   kODUnicodeEncodingSize = 2;
const ODULong   kODUnicodeTextLengthSize = 4;

//------------------------------------------------------------------------------
// ODGetITextProp
//
//      If iText is kODNULL, it is allocated and passed back.
//      If not, the _buffer of the text of the iText is disposed and a new _buffer
//              is allocated and filled.
//      If there is no value at the passed in prop & val then kODNULL is returned.
//      This is consistent with the other routines in StdTypIO which return pointers.
//      The reason is that there may be clients of ODGetITextProp which more easily
//      handle the flow of control if it just returns kODNULL instead of THROWing. -Tantek
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText* ODGetITextProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODIText* iText)
{

   // CHECK FOR EXISTENCE OF TRADITIONAL MAC TEXT FIRST.
   ODULong         macTextBufferSize = 0;
   ODBoolean       tradMacTextExists = ODSUExistsThenFocus(ev, su, prop, val);

   if (tradMacTextExists)
   {
      macTextBufferSize = su->GetSize(ev);
      if (macTextBufferSize == 0)     // if nothing there, treat as if doesn't exist
         tradMacTextExists = kODFalse;
      else
         macTextBufferSize -= sizeof(ODITextFormat);
         // otherwise, we need to account for the format info
      }

      if (tradMacTextExists) {
        ODITextFormat format;
        su->SetOffset(ev,0);
        StorageUnitGetValue(su, ev, sizeof(ODITextFormat),(ODValue)&format);
        if( (format != kODTraditionalMacText) &&
            (format != kODIBMCodePageText )) {
           WARNMSG_DEBUG(WARN_INDEX(-1),"Reading IText prop in unknown format %ld",format);
           // Don't throw, it's not that bad.
           // THROW(kODErrReadErr);           // Is this the best error code?
        }

#ifdef _UNICODE_
    unsigned short *macTextBuffer = (unsigned short *)ODNewPtr(macTextBufferSize);
#else
      octet *macTextBuffer = (octet *)ODNewPtr(macTextBufferSize);
#endif
      try { // HEY! GROPING OF THE ODITEXT GOING ON HERE! - NP
         StorageUnitGetValue(su, ev, macTextBufferSize,macTextBuffer);
         if( !iText ) {
            iText = new ODIText;
            iText->format = format;
         } else
            ODDisposePtr(iText->text._buffer);
      } catch (ODException _exception) {
         ODDisposePtr(macTextBuffer);
         throw;
      }
      iText->text._buffer = macTextBuffer;
      iText->text._length = macTextBufferSize;
      iText->text._maximum = macTextBufferSize;

   }
   // USE UNICODE IF TRADITIONAL MAC TEXT DOESN'T EXIST.
   else
   {
      ODBoolean       unicodeExists = ODSUExistsThenFocus(ev, su, prop,
                                                              kODIntlText);

      ODULong sizeOfValue = 0;
      ODULong sizeOfValueMinusHeader = 0;
      ODULong unicodeHeaderSize = kODUnicodeEncodingSize
                                                        + kODUnicodeTextLengthSize;
      if (unicodeExists)
      {
         sizeOfValue = su->GetSize(ev);
         if (sizeOfValue == 0)   // if nothing there, treat as if doesn't exist
            unicodeExists = kODFalse;
         else
            // unused for now.
            sizeOfValueMinusHeader = sizeOfValue - unicodeHeaderSize;
      }

      if (unicodeExists)
      {

         ODUShort        unicodeEncoding;
         ODULong         unicodeTextSize;

         su->SetOffset(ev,0);
         StorageUnitGetValue(su, ev, kODUnicodeEncodingSize,
                                            (ODValue)&unicodeEncoding);


/*    Comparing constants, doesn't make sense.
         ASSERTMSG(kODUnicodeEncodingSize == 2, kODErrAssertionFailed, "Code is broken. Assuming incorrect size passed to \"FlipEnd\" functions",0);
         ASSERTMSG(kODUnicodeTextLengthSize == 4, kODErrAssertionFailed, "Code is broken. Assuming incorrect size passed to \"FlipEnd\" functions",0);
*/
         unicodeEncoding = ConvertODUShortFromStd(unicodeEncoding);

         if( unicodeEncoding != kODISO10646_1993BaseEncoding )
         {
            THROW(kODErrReadErr);           // Is this the best error code?
         }

         StorageUnitGetValue(su, ev, kODUnicodeTextLengthSize,
                                            (ODValue)&unicodeTextSize);

         unicodeTextSize = ConvertODULongFromStd(unicodeTextSize);

         octet*  uniCodeTextBuffer;

         try {
            uniCodeTextBuffer = (octet *)ODNewPtr(unicodeTextSize);
            StorageUnitGetValue(su, ev, unicodeTextSize, uniCodeTextBuffer);

            ConvertODUShortArrayFromStd((ODUShort*)uniCodeTextBuffer,
                                                     unicodeTextSize / 2);

            iText = UnicodeToIText(iText, (ODUShort*)uniCodeTextBuffer,
                                                  unicodeTextSize);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
            if(!iText)  // UnicodeToIText returns NULL if iconv fails
                return (ODIText*) 0;
#endif
         } catch (ODException _exception) {
            ODDisposePtr(uniCodeTextBuffer);
            throw;
         }

         ODDisposePtr(uniCodeTextBuffer);
      }
      else
      {
         // clear the iText passed in if any.
         if( iText )
            SetITextString(iText,(char*)kODNULL);

         // but make sure we return kODNULL to indicate the lack of a property
         iText = kODNULL;
      }
   }   //this is the other brace to if (tradMacTextExist) {

   return iText;
}

//------------------------------------------------------------------------------
// ITextToUnicode
//
//      Storage passed back must be deallocated with ODDisposePtr
//------------------------------------------------------------------------------

void ITextToUnicode(ODIText* iText, ODUShort** unicodeText,
                  ODULong* unicodeTextLength);
void ITextToUnicode(ODIText* iText, ODUShort** unicodeText,
                  ODULong* unicodeTextLength)
{
   TradMacTextToUnicode((ODUByte*)GetITextPtr(iText),
                                         GetITextStringLength(iText), unicodeText,
                                         unicodeTextLength);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
   if(!*unicodeText)  // UnicodeToIText returns NULL if iconv fails
     return;
#endif
}

//------------------------------------------------------------------------------
// ODSetITextProp
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void    ODSetITextProp(Environment* ev,
         ODStorageUnit* su, ODPropertyName prop, ODValueType val,
         ODIText* iText)
{
   ODSUForceFocus(ev, su, prop, val);

   ODULong oldsize = su->GetSize(ev);
   ODULong newsize = sizeof(ODITextFormat) + iText->text._length;
   StorageUnitSetValue(su, ev,  sizeof(ODITextFormat),  (ODValue)&iText->format);
#ifdef _UNICODE_
  su->SetValue(ev, (ODByteArray*)&iText->text);      // text is an ODByteArray after all
#else
   su->SetValue(ev, &iText->text);                 // text is an ODByteArray after all
#endif
   if (oldsize > newsize)
      su->DeleteValue(ev, oldsize - newsize);

   // WRITE OUT kODIntlText NOW TOO.
#ifdef _PLATFORM_MAC_
//On Mac Unicode converison only works for Roman script and English language.
   if (GetITextScriptCode(iText) == smRoman
      && GetITextLangCode(iText) == langEnglish)
   {
#endif
      ODSUForceFocus(ev, su, prop, kODIntlText);
      ODUShort*       unicodeText;
      ODULong         unicodeTextLength, convertTextLength;
      const ODUShort  encoding = kODISO10646_1993BaseEncoding;
      ITextToUnicode(iText, &unicodeText, &unicodeTextLength);

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
      if(!unicodeText)  // ITextToUnicode returns NULL if iconv fails
         return;
#endif
      oldsize = su->GetSize(ev);
      newsize = kODUnicodeEncodingSize + kODUnicodeTextLengthSize
               + unicodeTextLength;

/* Comparing constants doesn't make sense
      ASSERTMSG(kODUnicodeEncodingSize == 2, kODErrAssertionFailed, "Code is broken. Assuming incorrect size passed to \"FlipEnd\" functions",0);
      ASSERTMSG(kODUnicodeTextLengthSize == 4, kODErrAssertionFailed, "Code is broken. Assuming incorrect size passed to \"FlipEnd\" functions",0);
*/
      ConvertODUShortToStd(encoding);
      StorageUnitSetValue(su, ev, kODUnicodeEncodingSize,
                                               (ODValue)&encoding);

      convertTextLength = ConvertODULongToStd(unicodeTextLength);
      StorageUnitSetValue(su, ev, kODUnicodeTextLengthSize,
                                            &convertTextLength);

      ConvertODUShortArrayToStd(unicodeText, unicodeTextLength / 2);
      StorageUnitSetValue(su, ev, unicodeTextLength,
                                            unicodeText);

      ODDisposePtr(unicodeText);
#ifdef _PLATFORM_MAC_
   }
#endif
}

//==============================================================================
// Time_T
//==============================================================================

#if useGMT

// actually most of these can be replaced by a constant that is platform specific

WIN32_DLLEXPORT static ODTime   diffBtwStdAndLocalTime()
{
   MachineLocation theLocation;
   struct tm date;
   ODTime baseDate;
   ODSLong internalGMTDelta;

   // 1970 Jan 1 in local time
   date.tm_sec = 0;
   date.tm_min = 0;
   date.tm_hour = 0;
   date.tm_mday = 1;
   date.tm_mon = 0;
   date.tm_year = 70;
   date.tm_isdst = 0;
   baseDate = (ODTime)mktime(&date);

   ReadLocation(&theLocation);

   internalGMTDelta = theLocation.u.gmtDelta & 0x00FFFFFF;

   if ((internalGMTDelta >> 23) &1)
      internalGMTDelta |= 0xFF000000;

   return (baseDate + internalGMTDelta);
}
#endif

WIN32_DLLEXPORT ODTime          ODGetTime_TProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODTime value;
      StorageUnitGetValue(su, ev, kODULongLen, (ODValue)&value);

#if useGMT
      return (ConvertODULongFromStd(value) + diffBtwStdAndLocalTime());
#else
      return ConvertODULongFromStd(value);
#endif
   }
   else {
      WARNMSG_DEBUG(WARN_INDEX(-1),"No Time_T Property.");
      return 0;
   }
}

WIN32_DLLEXPORT void            ODSetTime_TProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODTime value)
{
   ODSUForceFocus(ev, su, prop, val);

#if useGMT
   value = ConvertODULongToStd(value - diffBtwStdAndLocalTime());
#else
   value = ConvertODULongToStd(value);
#endif

   StorageUnitSetValue(su, ev, kODULongLen, (ODValue)&value);
}


//==============================================================================
// Point
//==============================================================================

//      If there is no value at the passed in prop & val then kODNULL is returned.
//      This is consistent with the other routines in StdTypIO which return pointers.
//      The reason is that there may be clients of ODGetPointProp which more easily
//      handle the flow of control if it just returns kODNULL instead of THROWing. -Tantek

WIN32_DLLEXPORT ODPoint*        ODGetPointProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODPoint* value)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      if (!value)
         value = (ODPoint*)ODNewPtrClear(kODPointLen);

      StorageUnitGetValue(su, ev, sizeof(ODPoint), (ODValue) value);

      ConvertODSLongArrayFromStd((ODSLong*) value, sizeof(ODPoint)/sizeof(ODSLong));

      return value;
   }
   else {
      return kODNULL;
   }
}

WIN32_DLLEXPORT void            ODSetPointProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODPoint* value)
{
   ODSUForceFocus(ev, su, prop, val);

   ODPoint tmpValue = *value;
   ConvertODSLongArrayToStd((ODULong*) &tmpValue, sizeof(ODPoint)/sizeof(ODSLong));

   StorageUnitSetValue(su, ev, sizeof(ODPoint), (ODValue) &tmpValue);
}

//==============================================================================
// Rect
//==============================================================================

//      If there is no value at the passed in prop & val then kODNULL is returned.
//      This is consistent with the other routines in StdTypIO which return pointers.
//      The reason is that there may be clients of ODGetRectProp which more easily
//      handle the flow of control if it just returns kODNULL instead of THROWing. -Tantek

WIN32_DLLEXPORT ODRect*         ODGetRectProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODRect* value)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      if (!value)
         value = (ODRect*)ODNewPtrClear(kODRectLen);

      StorageUnitGetValue(su, ev, sizeof(ODRect), (ODValue) value);
      ConvertODSLongArrayFromStd((ODSLong*) value, sizeof(ODRect)/sizeof(ODCoordinate));

      return value;
   }
   else {
      return kODNULL;
   }
}

WIN32_DLLEXPORT void            ODSetRectProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODRect* value)
{
   ODSUForceFocus(ev, su, prop, val);

   ODRect tmpValue = *value;
   ConvertODSLongArrayToStd((ODULong*) &tmpValue, sizeof(ODRect)/sizeof(ODCoordinate));

   StorageUnitSetValue(su, ev, sizeof(ODRect), (ODValue) &tmpValue);
}

//==============================================================================
// StorageUnit Reference
//==============================================================================

WIN32_DLLEXPORT ODID            ODGetStrongSURefProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODStorageUnitRef value;
      StorageUnitGetValue(su, ev, kODStorageUnitRefLen, (ODValue)&value);
      if ( su->IsValidStorageUnitRef(ev, value) )
         return su->GetIDFromStorageUnitRef(ev,value);
      else
         return kODNULLID;
   }
   else {
      //WARN("No StrongSURef Property.");// This is EXPECTED in many uses of ODGetStrongSURefProp, so this makes sense.
      return kODNULLID;
   }
}

WIN32_DLLEXPORT void            ODSetStrongSURefProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODID id)
{
   ODSUForceFocus(ev, su, prop, val);
   ODStorageUnitRef value;
   su->GetStrongStorageUnitRef(ev, id, value);
   StorageUnitSetValue(su, ev, kODStorageUnitRefLen, value);
}


WIN32_DLLEXPORT ODID            ODGetWeakSURefProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODStorageUnitRef value;
      StorageUnitGetValue(su, ev, kODStorageUnitRefLen, (ODValue)&value);
      ODID id = kODNULLID;
      if (su->IsValidStorageUnitRef(ev, value))
         id = su->GetIDFromStorageUnitRef(ev,value);

      return id;
   }
   else {
      //WARN("No WeakSURef Property."); // This is EXPECTED in many uses of ODGetWeakSURefProp, so this makes sense.
      return kODNULLID;
   }
}

WIN32_DLLEXPORT void            ODSetWeakSURefProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODID id)
{
   ODSUForceFocus(ev, su, prop, val);
   ODStorageUnitRef value;
   su->GetWeakStorageUnitRef(ev, id, value);
   StorageUnitSetValue(su, ev, kODStorageUnitRefLen, value);
}

//==============================================================================
// Polygon
//==============================================================================

//      If there is no value at the passed in prop & val then kODNULL is returned.
//      This is consistent with the other routines in StdTypIO which return pointers.
//      The reason is that there may be clients of ODGetPolygonProp which more easily
//      handle the flow of control if it just returns kODNULL instead of THROWing. -Tantek

WIN32_DLLEXPORT ODPolygon*      ODGetPolygonProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODPolygon* value)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      ODULong valueSize = su->GetSize(ev);
      if (value == kODNULL)
      {
         value = new ODPolygon;
      }
      // value->Realloc(valueSize); // can't because Realloc is private. Doh!
      // instead we do the following
      if (valueSize > 0)
      {
         ODPolygonData* data = (ODPolygonData*)ODNewPtr(valueSize);
         StorageUnitGetValue(su, ev, valueSize, (ODValue)data);
         ConvertODSLongArrayFromStd(data, valueSize/sizeof(ODSLong));
         value->SetData(data);
      }
      return value;
   }
   else
   {
      return kODNULL;
   }
}


WIN32_DLLEXPORT void            ODSetPolygonProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            const ODPolygon* value)
{
   ODSUForceFocus(ev, su, prop, val);
   ODULong oldsize = su->GetSize(ev);
   ODULong newsize = value->GetDataSize();
   ODPtr buf = ODNewPtr(newsize);
   ODBlockMove(value->GetData(), buf, newsize);
   ConvertODSLongArrayToStd(buf, newsize/sizeof(ODSLong));
   StorageUnitSetValue(su, ev, newsize, (ODValue) buf);
   if (oldsize > newsize)
      su->DeleteValue(ev, oldsize - newsize);
   ODDisposePtr(buf);
}

//==============================================================================
// Matrix
//==============================================================================

//      If there is no value at the passed in prop & val then kODNULL is returned.
//      This is consistent with the other routines in StdTypIO which return pointers.
//      The reason is that there may be clients of ODGetMatrixProp which more easily
//      handle the flow of control if it just returns kODNULL instead of THROWing. -Tantek

WIN32_DLLEXPORT ODMatrix*       ODGetMatrixProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODMatrix* value)
{
   if (ODSUExistsThenFocus(ev, su, prop, val))
   {
      if (!value)
         value = (ODMatrix*)ODNewPtrClear(sizeof(ODMatrix));

      StorageUnitGetValue(su, ev, sizeof(ODMatrix), (ODValue)value);
      ConvertODSLongArrayFromStd((ODSLong*) value, sizeof(ODMatrix)/sizeof(ODFixed));

      return value;
   }
   else
   {
      return kODNULL;
   }
}

WIN32_DLLEXPORT void    ODSetMatrixProp(Environment* ev,
            ODStorageUnit* su, ODPropertyName prop, ODValueType val,
            ODMatrix* value)
{
   ODSUForceFocus(ev, su, prop, val);

   ODMatrix tmpValue = *value;
   ConvertODSLongArrayToStd((ODULong*) &tmpValue, sizeof(ODMatrix)/sizeof(ODFixed));

   StorageUnitSetValue(su, ev, sizeof(ODMatrix), (ODValue) &tmpValue);
}

//==============================================================================
// IconFamily -- see StdIFmIO.cpp
//==============================================================================

