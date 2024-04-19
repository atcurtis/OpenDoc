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
// @(#) 1.21 com/src/pubutils/src/IText.cpp, odmri, od96os2, odos29646d 9/18/96 15:15:32 [ 11/15/96 15:29:11 ]
/*
   File:    IText.cpp

   Contains:   Routines for manipulating ITexts.

   Owned by:   Vincent Lo

   Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

   To Do:

      These could all be rewritten to use the ODByteArray functions,
      thus reducing code size.

   In Progress:

*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ODEXCEPT_
#include <ODExcept.h>
#endif

#ifndef _UTILERRS_
#include <UtilErrs.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif

#ifndef _BARRAY_
#include "BArray.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef  SOM_Module_include5cStdTypes_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#include "string.h"


#if defined(__IBMCPP__) || defined(_IBMR2)
// These are for creating an IText with default code page info
#include <nl_types.h>
#include <langinfo.h>
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#include <string.h>
#define kIBMCodePagePrefix "IBM-"
#endif

#ifdef _PLATFORM_AIX_
#include <nl_types.h>
#include <langinfo.h>
ODScriptCode ODGetCodePageIdentifier();
#endif

//==============================================================================
// IText
//==============================================================================


#if ODDebug
   #if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
   #define CHECK_FORMAT(I)               ASSERTMSG_DEBUG((I) && (I)->format==kODIBMCodePageText,  \
                                                  kODErrAssertionFailed, "Unknown IText format")

   #else // _PLATFORM_MACINTOSH_
   #define CHECK_FORMAT(I)               ASSERTMSG_DEBUG((I) && (I)->format==kODTraditionalMacText,  \
                                                  kODErrAssertionFailed, "Unknown IText format")
   #endif

#else

   #if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
   #define CHECK_FORMAT(I)               if( (I)->format==kODIBMCodePageText ) ;         \
                                            else THROW(kODErrInvalidITextFormat)

   #else // _PLATFORM_MACINTOSH_
   #define CHECK_FORMAT(I)               if( (I)->format==kODTraditionalMacText ) ;         \
                                            else THROW(kODErrInvalidITextFormat)
   #endif
#endif


//------------------------------------------------------------------------------
// NewIText  [static]   Creates an uninitialized empty IText.
//------------------------------------------------------------------------------

static ODIText* NewIText( )
{
   ODIText* result = new ODIText;

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
   result->format = kODIBMCodePageText;
#else  // _PLATFORM_MACINTOSH_
   result->format = kODTraditionalMacText;
#endif
   result->text._buffer = kODNULL;
   return result;
}


//------------------------------------------------------------------------------
// CreateITextCString
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText* CreateITextCString(ODScriptCode scriptCode, ODLangCode langCode, char* text)
{
   ODIText* result = NewIText();

   try {
      SetITextString(result, text);
      SetITextScriptCode(result, scriptCode);
      SetITextLangCode(result, langCode);
   } catch (ODException _exception) {
      DisposeIText(result);
      throw;
   }

   return result;
}


//------------------------------------------------------------------------------
// CreateITextPString
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText* CreateITextPString(ODScriptCode scriptCode, ODLangCode langCode, StringPtr text)
{
   ODIText* result = NewIText();

   try {
      SetITextString(result, text);
      SetITextScriptCode(result, scriptCode);
      SetITextLangCode(result, langCode);
   } catch (ODException _exception) {
      DisposeIText(result);
      throw;
   }

   return result;
}


//------------------------------------------------------------------------------
// CreateITextClear
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText* CreateITextClear(ODScriptCode scriptCode, ODLangCode langCode, ODSize textLen)
{
   ODIText* result = NewIText();

   try {
      SetITextBufferSize(result, textLen+sizeof(ODTradITextDataHeader), kODFalse);
      SetITextScriptCode(result, scriptCode);
      SetITextLangCode(result, langCode);
   } catch (ODException _exception) {
      DisposeIText(result);
      throw;
   }

   return result;
}


//------------------------------------------------------------------------------
// CreateIText
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText*                 CreateITextWLen(ODScriptCode scriptCode, ODLangCode langCode,
                  ODUByte* text, ODSize textLength )
{
   ODIText* result = NewIText();

   try {
      SetITextBufferSize(result,
                                         textLength + sizeof(ODTradITextDataHeader), kODFalse);
      ODBlockMove(text,
               &(((ODTradITextData*)(result->text._buffer))->theText),
               textLength);
      SetITextScriptCode(result, scriptCode);
      SetITextLangCode(result, langCode);
   } catch (ODException _exception) {
      DisposeIText(result);
      throw;
   }

   return result;
}


//------------------------------------------------------------------------------
// CreateIText from a char* with no script code or lang code. Since on Windows and
// and OS/2 there is the no seperation of the notions of script and language  it is
// possible to create an IText simply from a char*. The code page is detected from the
// and this information is kept in the script code field. - OF.
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText*        CreateITextFromCharPtr(char* text)
{

#if defined(_PLATFORM_OS2_)
   /* In order to put the code page into the script code
      we have to:

    1 - Get the script code.
    2 - Strip off the IBM- part from the returned string.
    3 - Convert the remaining string into an int.
    4 - assign the result to scriptCode

   */
   // Step 1

   char* codeset=nl_langinfo(CODESET);

   //Step 2
   char* numberPart; //The number part of the code set string.
   if (strstr(codeset, kIBMCodePagePrefix)) {
      numberPart=codeset+strlen(kIBMCodePagePrefix);
   } else {              //strstr returned null, "IBM-" is not a part of codeset.
      THROW(kODErrAssertionFailed, "Unknown code set");
   }

   //Step 3 & 4
   ODScriptCode scriptCode=atoi(numberPart);
#else
   #ifdef _PLATFORM_WIN32_
      ODScriptCode scriptCode=GetOEMCP();
   #endif
   #ifdef _PLATFORM_AIX_
      ODScriptCode scriptCode=ODGetCodePageIdentifier();
   #endif
#endif

   ODSize textLength=strlen(text);

   ODIText* result = NewIText();

   try {
      SetITextBufferSize(result,
                                         textLength + sizeof(ODTradITextDataHeader), kODFalse);
      ODBlockMove(text,
               &(((ODTradITextData*)(result->text._buffer))->theText),
               textLength);
      SetITextScriptCode(result, scriptCode);

   //There is no langCode assoicated with kODIBMCodePageText. So, set the langCode to 0
      SetITextLangCode(result, 0);
   } catch (ODException _exception) {
      DisposeIText(result);
      throw;
   }

   return result;

}
//------------------------------------------------------------------------------
// CopyIText
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText* CopyIText(ODIText* original)
{
   CHECK_FORMAT(original);

   ODIText* result = NewIText();

   try {
      unsigned long dataSize = original->text._length;
      result->format = original->format;
      result->text._buffer = (octet *)ODNewPtr(dataSize);
      result->text._maximum = dataSize;
      result->text._length  = dataSize;
      ODBlockMove(original->text._buffer, result->text._buffer, dataSize);
   } catch (ODException _exception) {
      DisposeIText(result);
      throw;
   }

   return result;
}

//------------------------------------------------------------------------------
// CopyITextStruct
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText CopyITextStruct(ODIText* original)
{
   ODIText  result;

   CHECK_FORMAT(original);

   result.format = original->format;
   result.text = CopyByteArrayStruct(&(original->text));

   return result;
}


//------------------------------------------------------------------------------
// SetITextBufferSize
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText*
SetITextBufferSize( ODIText* iText, ODSize bufferSize, ODBoolean preserveContents )
{
   // Will create new IText if input is NULL.

   if( iText && iText->text._maximum==bufferSize && iText->text._buffer ) {
      iText->text._length = bufferSize;
      return iText;                                     // No-op
   }

   ODTradITextData *buffer = (ODTradITextData*) ODNewPtr(bufferSize);
   if( !iText ) {
      try {
         iText = NewIText();
      } catch (ODException _exception) {
         ODDisposePtr(buffer);
         throw;
      }
   } else if( iText->text._buffer ) {
      if( preserveContents )
         ODBlockMove(iText->text._buffer, buffer, bufferSize);
      ODDisposePtr(iText->text._buffer);
   }
   iText->text._buffer = (octet*) buffer;
   iText->text._maximum = bufferSize;
   iText->text._length = bufferSize;
   return iText;
}


//------------------------------------------------------------------------------
// GetITextPtr
//------------------------------------------------------------------------------

WIN32_DLLEXPORT char* GetITextPtr( ODIText *iText )
{
   CHECK_FORMAT(iText);
   return ((ODTradITextData*)iText->text._buffer)->theText;
}


//------------------------------------------------------------------------------
// SetITextScriptCode
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void SetITextScriptCode(ODIText* iText, ODScriptCode scriptCode)
{
   CHECK_FORMAT(iText);
   ODTradITextData *data = (ODTradITextData*) iText->text._buffer;
   data->theScriptCode = scriptCode;
}


//------------------------------------------------------------------------------
// GetITextScriptCode
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODScriptCode GetITextScriptCode(ODIText* iText)
{
   CHECK_FORMAT(iText);
   ODTradITextData *data = (ODTradITextData*) iText->text._buffer;
   return data->theScriptCode;
}


//------------------------------------------------------------------------------
// SetITextLangCode
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void SetITextLangCode(ODIText* iText, ODLangCode langCode)
{
   CHECK_FORMAT(iText);
   ODTradITextData *data = (ODTradITextData*) iText->text._buffer;
   data->theLangCode = langCode;
}


//------------------------------------------------------------------------------
// GetITextLangCode
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODLangCode GetITextLangCode(ODIText* iText)
{
   CHECK_FORMAT(iText);
   ODTradITextData *data = (ODTradITextData*) iText->text._buffer;
   return data->theLangCode;
}


//------------------------------------------------------------------------------
// SetITextStringLength
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODIText* SetITextStringLength( ODIText* iText, ODSize length, ODBoolean preserveText )
{
   ODTradITextDataHeader data = {0,0};
   if( iText ) {
      CHECK_FORMAT(iText);
      if( iText->text._buffer )
         data = * (ODTradITextDataHeader*) iText->text._buffer;
   }
   iText = SetITextBufferSize(iText,length+sizeof(ODTradITextDataHeader),preserveText);
   if( !preserveText )
      * (ODTradITextDataHeader*) iText->text._buffer = data;
   return iText;
}


//------------------------------------------------------------------------------
// GetITextStringLength
//------------------------------------------------------------------------------

WIN32_DLLEXPORT ODULong GetITextStringLength(ODIText* iText)
{
   CHECK_FORMAT(iText);
   if( iText->text._buffer )
      return iText->text._length - sizeof(ODTradITextDataHeader);
   else
      return 0;
}


//------------------------------------------------------------------------------
// SetITextCString
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void SetITextCString(ODIText* iText, char* text)
{
   CHECK_FORMAT(iText);
   ODSize textLen = text ?strlen(text) :0;
   SetITextStringLength(iText,textLen,kODFalse);
   ODBlockMove(text, ((ODTradITextData*)iText->text._buffer)->theText, textLen);
}


//------------------------------------------------------------------------------
// SetITextPString
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void SetITextPString(ODIText* iText, StringPtr text)
{
   CHECK_FORMAT(iText);
   ODSize textLen = text ?text[0] :0;
   SetITextStringLength(iText,textLen,kODFalse);
   ODBlockMove(&text[1], ((ODTradITextData*)iText->text._buffer)->theText, textLen);
}


//------------------------------------------------------------------------------
// SetITextText
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void SetITextText(ODIText* iText, ODUByte* text, ODSize textLength)
{
   CHECK_FORMAT(iText);
   SetITextStringLength(iText, textLength, kODFalse);
   ODBlockMove(text, ((ODTradITextData*)iText->text._buffer)->theText,
            textLength);
}


//------------------------------------------------------------------------------
// GetITextCString
//------------------------------------------------------------------------------

WIN32_DLLEXPORT char* GetITextCString(ODIText* iText, char *cstring)
{
   CHECK_FORMAT(iText);
   ODSize len;
   if( iText->text._buffer )
      len = iText->text._length - sizeof(ODTradITextDataHeader);
   else
      len = 0;

   if( !cstring )
      cstring = new char[len+1];
   cstring[len] = '\0';
   if( len )
      ODBlockMove(((ODTradITextData*)iText->text._buffer)->theText,
               cstring, len);
   return cstring;
}


//------------------------------------------------------------------------------
// GetITextPString
//------------------------------------------------------------------------------

WIN32_DLLEXPORT StringPtr GetITextPString(ODIText* iText, Str255 pstring)
{
   CHECK_FORMAT(iText);
   ODSize len;
   if( iText->text._buffer )
      len = iText->text._length - sizeof(ODTradITextDataHeader);
   else
      len = 0;

   if( !pstring )
      pstring = new unsigned char[len+1];
   if( len )
      ODBlockMove(((ODTradITextData*)iText->text._buffer)->theText,
               &pstring[1], len);
   pstring[0] = len;
   return pstring;
}


//------------------------------------------------------------------------------
// DisposeIText
//------------------------------------------------------------------------------

WIN32_DLLEXPORT void DisposeIText(ODIText* iText)
{
   if( iText ) {
      CHECK_FORMAT(iText);
      DisposeITextStruct(*iText);
      delete iText;
   }
}


#ifdef _PLATFORM_AIX_
//========================================================
// This function retrieves the codeset from the system and
// then returns an identifier representing the codeset.
//========================================================
ODScriptCode ODGetCodePageIdentifier()
 {
  char* codeset=nl_langinfo(CODESET);  // get codepage from system

  // now determine which codepage we are using and
  // return the code that identifies it.

   if(!codeset)
     {
     WARNMSG(WARN_INDEX(0),"Error: codeset is NULL!,File = %s\n",__FILE__);
     return kUnknown;
     }

  if(!strcmp("IBM-1046",codeset))       
     return kIBM_1046;
  else if(!strcmp("ISO8859-6",codeset))
      return kISO8859_6;
  else if(!strcmp("ISO8859-1",codeset))
      return kISO8859_1;
  else if(!strcmp("ISO8859-5",codeset))
      return kISO8859_5;
  else if(!strcmp("IBM-850",codeset))   
      return kIBM_850;
  else if(!strcmp("IBM-eucTW",codeset)) 
      return kIBM_eucTW;
  else if(!strcmp("ISO8859-2",codeset))
      return kISO8859_2;
  else if(!strcmp("IBM-932",codeset)) 
      return kIBM_932;
  else if(!strcmp("IBM-eucJP",codeset)) 
      return kIBM_eucJP;
  else if(!strcmp("IBM-eucKR",codeset)) 
      return kIBM_eucKR;
  else if(!strcmp("ISO8859-7",codeset)) 
      return kISO8859_7;
  else if(!strcmp("IBM-856",codeset)) 
      return kIBM_856;
  else if(!strcmp("ISO8859-8",codeset)) 
      return kISO8859_8;
  else if(!strcmp("IBM-eucCN",codeset)) 
      return kIBM_eucCN;
  else if(!strcmp("ISO8859-9",codeset)) 
      return kISO8859_9;
  else if(!strcmp("Unicode",codeset)) 
      return kUnicode;
  else if(!strcmp("UTF-8",codeset)) 
      return kUTF_8;
  else
    {
      WARNMSG(WARN_INDEX(0),"Error: Unknown codepage!,File = %s\n",__FILE__);
      return kUnknown;
    }
 }
#endif // _PLATFORM_AIX_



