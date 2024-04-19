/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odmri
 *
 *   CLASSES:   ITextTest
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
// @(#) 1.18 com/src/pubutils/include/IText.h, odmri, od96os2, odos29646d 9/18/96 15:15:35 [ 11/15/96 15:29:12 ]
/*
   File:           IText.h

   Contains:       Routines for manipulating ITexts.

   Owned by:       Vincent Lo

   Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#ifndef _ITEXT_
#define _ITEXT_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif

typedef short ODScriptCode;
typedef short ODLangCode;

#ifdef _PLATFORM_AIX_
// Used in ODGetCodePageIdentifier() to represent codepages
typedef enum {
  kUnknown,    /* Unknown codepage */
  kIBM_1046,   /* IBM-1046     */
  kISO8859_6,  /* ISO8859-6    */
  kISO8859_1,  /* ISO8859-1    */
  kISO8859_5,  /* ISO8859-5    */
  kIBM_850,    /* IBM-850      */
  kIBM_eucTW,  /* IBM-eucTW    */
  kISO8859_2,  /* ISO8859-2    */
  kIBM_932,    /* IBM-932      */
  kIBM_eucJP,  /* IBM-eucJP    */
  kIBM_eucKR,  /* IBM-eucKR    */ 
  kISO8859_7,  /* ISO8859-7    */
  kIBM_856,    /* IBM-856      */ 
  kISO8859_8,  /* ISO8859-8   */
  kIBM_eucCN,  /* IBM-eucCN    */
  kISO8859_9,  /* ISO8859-9    */
  kUnicode,    /* Unicode      */
  kUTF_8       /* UTF-8        */
} ODCodePages;
#endif // _PLATFORM_AIX_


// ODTradIText: This is the format of the data stored in an IText whose
// format is kODTradMacintosh.

struct ODTradITextDataHeader {
   ODScriptCode theScriptCode;
   ODLangCode       theLangCode;
};
typedef struct ODTradITextDataHeader ODTradITextDataHeader;

struct ODTradITextData {
   ODScriptCode theScriptCode;
   ODLangCode       theLangCode;
   char             theText[1];            // Variable length array!
};
typedef struct ODTradITextData ODTradITextData;

#define kSizeOfODTradITextData  4


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

#ifdef __cplusplus
extern "C" {
#endif


_DLLIMPORTEXPORT_ ODIText*        CreateITextFromCharPtr(char* text);
_DLLIMPORTEXPORT_ ODIText*        CreateITextCString(ODScriptCode, ODLangCode, char* text);
_DLLIMPORTEXPORT_ ODIText*        CreateITextPString(ODScriptCode, ODLangCode, StringPtr text);
_DLLIMPORTEXPORT_ ODIText*        CreateITextClear(  ODScriptCode, ODLangCode, ODSize stringLength );
_DLLIMPORTEXPORT_ ODIText*        CreateITextWLen(ODScriptCode, ODLangCode, ODUByte* text,
                  ODSize textLength );
   // For text that is neither a C string or a Pascal string.

_DLLIMPORTEXPORT_ ODIText*        SetITextBufferSize( ODIText*, ODSize bufferSize, ODBoolean preserveContents );
   // Low level operation to set byte-array buffer size directly.
   // If input is NULL, will create & return a new ODIText.

_DLLIMPORTEXPORT_ ODIText*        CopyIText(ODIText* original);
   // Allocates and returns an exact copy of the IText passed in.
_DLLIMPORTEXPORT_ ODIText         CopyITextStruct(ODIText* original);

_DLLIMPORTEXPORT_ void            SetITextScriptCode(ODIText*, ODScriptCode);
_DLLIMPORTEXPORT_ ODScriptCode GetITextScriptCode(ODIText*);

_DLLIMPORTEXPORT_ void            SetITextLangCode(ODIText*, ODLangCode);
_DLLIMPORTEXPORT_ ODLangCode      GetITextLangCode(ODIText*);

_DLLIMPORTEXPORT_ ODIText*        SetITextStringLength( ODIText*, ODSize length, ODBoolean preserveText );
   // If NULL is passed in, allocates & returns a new ODIText.

_DLLIMPORTEXPORT_ ODULong         GetITextStringLength(ODIText*);
_DLLIMPORTEXPORT_ char*           GetITextPtr( ODIText* );
   // Returns ptr to raw text without allocating any memory.

_DLLIMPORTEXPORT_ void            SetITextCString(ODIText*, char* text);
_DLLIMPORTEXPORT_ void            SetITextPString(ODIText*, StringPtr text);
_DLLIMPORTEXPORT_ void            SetITextText(ODIText*, ODUByte* text, ODSize textLength);

_DLLIMPORTEXPORT_ char*           GetITextCString(ODIText*, char *cstring);
_DLLIMPORTEXPORT_ StringPtr       GetITextPString(ODIText*, Str255 pstring);
   // If a string is passed in, it copies the text there and returns a ptr to it.
   // If a NULL string is passed, it allocates a new string and returns it.


_DLLIMPORTEXPORT_ void            DisposeIText(ODIText* iText);
_DLLIMPORTEXPORT_ void            DisposeITextStruct(ODIText iText);

/*
   DisposeITextStruct is a macro instead of a function so it can take a
   different parameter type than DisposeIText(), and still set the disposed
   pointer field of the structure to null.
   DisposeIText() must never be called on a IText structure;
   DisposeIText() disposes the structure which is usually a disaster.

   See BArray.h for another example of this.
*/

#define DisposeITextStruct(iText)               \
   do{                                                                                     \
      if ((iText).text._buffer != kODNULL) {          \
         ODDisposePtr((iText).text._buffer);     \
         (iText).text._buffer = kODNULL;         \
      }                                                                               \
      (iText).text._length = 0;                                       \
      (iText).text._maximum = 0;                                      \
   }while(0)

#ifdef __cplusplus
   } // closes extern "C" block

   // Overloaded variants for convenience:
   inline ODIText* CreateIText(ODScriptCode s, ODLangCode l, char* text)
                  {return CreateITextCString(s,l,text);}
   inline ODIText* CreateIText(ODScriptCode s, ODLangCode l, StringPtr text)
                  {return CreateITextPString(s,l,text);}

   inline ODIText* CreateIText(ODScriptCode s, ODLangCode l, ODSize stringLength )
                  {return CreateITextClear(s,l,stringLength);}
   inline ODIText* CreateIText(ODSize textLength)
                  {return SetITextStringLength(kODNULL,textLength,kODFalse);}
   inline ODIText* CreateIText(ODScriptCode scriptCode, ODLangCode langCode,
                  ODUByte* text, ODSize textLength)
                  {return CreateITextWLen(scriptCode, langCode, text, textLength );}

   inline void             SetITextString(ODIText* i, char* text)
                  {SetITextCString(i,text);}
   inline void             SetITextString(ODIText* i, StringPtr text)
                  {SetITextPString(i,text);}

   inline char*    GetITextString(ODIText* i, char* text)
                  {return GetITextCString(i,text);}
   inline StringPtr GetITextString(ODIText* i, StringPtr text)
                  {return GetITextPString(i,text);}

   inline char*    GetCStringFromIText(ODIText* iText)
                  {return GetITextCString(iText,(char*)kODNULL);}
   inline StringPtr GetPStringFromIText(ODIText* iText)
                  {return GetITextPString(iText,(StringPtr)kODNULL);}
#endif /*__cplusplus*/


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif

#endif  /*_ITEXT_*/
