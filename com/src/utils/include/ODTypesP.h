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
// @(#) 1.24 com/src/utils/include/ODTypesP.h, odutils, od96os2, odos29646d 7/31/96 09:57:48 [ 11/15/96 15:29:36 ]
/********************************************************************/
/*  Licensed Materials - Property of IBM                            */
/*                                                                  */
/*                                                                  */
/* Copyright (C) International Business Machines Corp., 1994.       */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/*  US Government Users Restricted Rights -                         */
/*  Use, duplication, or disclosure restricted                      */
/*  by GSA ADP Schedule Contract with IBM Corp.                     */
/*                                                                  */
/********************************************************************/

#ifndef _ODTYPESP_
#define _ODTYPESP_

#include <stdlib.h>
#include <string.h>

#ifndef _ITEXT_
#include <IText.h>
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
#ifndef _ODTYPESF_
#include <ODTypesF.h>
#endif
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

// ********* Begin Mac header Lists.h **************
   /* The apple drop was missing this file so I'll fake it. */
   typedef struct {} ListHandle;

   typedef struct {} DialogXBlahX;
   typedef DialogXBlahX * DialogPtr;
// ********* End Mac header Lists.h **************

// ********* Begin Mac header OCEStandardMail.h? **************
  typedef int SMPEditCommand;
  #define gestaltOCEToolboxAttr 2000
  #define gestaltOCETBAvailable 2001
  #define kSMPSelectAllCommand 3000
// ********* End Mac header OCEStandardMail.h? **************

//#ifndef __LIBRARYMANAGER__
//  #define __LIBRARYMANAGER__
//  class  TMemoryPool;
//#endif

// ********* Begin Mac header Types.h **************

#ifdef _PLATFORM_WIN32_
 #ifndef _WTYPES_
 #include <wtypes.h>
 #endif
 #ifndef _WINBASE_
 #include <winbase.h>
 #endif
 #ifndef _WINDEF_
 #include <windef.h>
 #endif
#endif // _PLATFORM_WIN32_
#ifdef _PLATFORM_OS2_
 #include <ODos2.h>
#endif // _PLATFORM_OS2_
#ifdef _PLATFORM_UNIX_
 #include <sys/types.h>
 #include <sys/param.h>
 #include <unistd.h>
#endif // _PLATFORM_UNIX_
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
 #define pascal
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

#ifndef SOMCorba_h
#include<somcorba.h>
#endif

#ifndef _BooleanType
   #define _BooleanType
   typedef int Boolean;
#endif

 // use OpenClass's true and false which are enums set to 1 and 0 respectively.
 // this will NOT conflict with SOM's TRUE and FALSE, which are 
 // defines set to 1 and 0 respectively.
 #include <isynonym.hpp>

#ifndef _PLATFORM_AIX_
 // for AIX, Cursor is defined in /usr/include/X11/X.h, which we got by
 // including ODTypesF.h which includes /usr/include/X11/Intrinsic.h
 typedef void* Cursor;
#endif

 #define nil 0

 typedef unsigned char Byte;
 typedef char SignedByte;
 typedef char *Ptr;
 typedef Ptr *Handle;    /*  pointer to a master pointer */

 typedef long Fixed;     /* fixed point arithmatic type */
 typedef Fixed *FixedPtr;
 typedef long Fract;
 typedef Fract *FractPtr;
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
 typedef double double_t;
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

 struct extended80 {
  short w[5];
 };
 typedef struct extended80 extended80;

 enum {v,h};
 typedef unsigned char VHSelect;

 typedef unsigned char Str63[64],Str32[33],Str31[32],Str27[28],Str15[16],**StringHandle;

 typedef const unsigned char *ConstStr255Param;
 typedef ConstStr255Param ConstStr63Param,ConstStr32Param,ConstStr31Param,
  ConstStr27Param,ConstStr15Param;

 typedef unsigned long OSType;
 typedef OSType *OSTypePtr;
 typedef unsigned long ResType;
 typedef ResType *ResTypePtr;
 typedef unsigned char Style;
 typedef short ScriptCode;

 enum {
  normal = 0,
  bold = 1,
  italic = 2,
  underline = 4,
  outline = 8,
  shadow = 0x10,
  condense = 0x20,
  extend = 0x40
 };

   #define Debugger
   #define DebugStr
   #define debugstr
   #define SysBreakStr
   #define SysBreakFunc

// ********* End Mac header Types.h **************

// ********* Begin Mac header Strings.h **************

    #define aNumber "001"

#ifndef _PLATFORM_AIX_
     // xlC has no itoa function
     inline
     void NumToString(int id, char* pString) { _itoa(id,pString,10);}
#endif

    inline
    char * p2cstr( StringPtr pString) { return (char*)pString; }

    inline
    StringPtr c2pstr( char *pString) { return (StringPtr)pString; }


// ********* End Mac header Strings.h **************


// ********* Begin Mac header AERegistry.h **************

#if 0
struct WritingCode {
    ScriptCode  theScriptCode;
    LangCode    theLangCode;
};

typedef struct WritingCode WritingCode;

struct IntlText {
    ScriptCode  theScriptCode;
    LangCode    theLangCode;
    char        theText[1];
    /* variable length data */
};

typedef struct IntlText IntlText;

#endif // 0

// ********* End Mac header AERegistry.h **************

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
// ********* Begin Mac header AppleEvents.h **************

typedef unsigned long AEKeyword;
typedef ResType DescType;
typedef AEDescList AEDesc;
typedef AEDescList AERecord;
typedef AERecord AppleEvent;

// ********* End Mac header AppleEvents.h **************

// ********* Begin Mac header Menus.h **************
struct MenuInfo {
    short menuID;
    short menuWidth;
    short menuHeight;
    Handle menuProc;
    long enableFlags;
    Str255 menuData;
};

#if defined(_PLATFORM_WIN32_)
typedef HMENU     MenuHandle;
#else // !defined(_PLATFORM_WIN32_)

#if defined(_PLATFORM_OS2_)
typedef HWND     MenuHandle;
#else // !defined(_PLATFORM_OS2_)

#if defined(_PLATFORM_AIX_)
typedef void*	MenuHandle;
#else // !defined(_PLATFORM_AIX_)
#error ODTypesP.h: MenuHandle needs to be defined for this platform.
#endif // defined(_PLATFORM_AIX_)

#endif // defined(_PLATFORM_OS2_)

#endif // defined(_PLATFORM_WIN32_)

// ********* End Mac header Menus.h **************

// ********* Begin Mac header Quickdraw.h **************

typedef ODRgnHandle  RgnHandle;

// ********* End Mac header Quickdraw.h **************

// ********* Begin Mac header Controls.h **************

typedef void *ControlHandle;

// ********* End Mac header Controls.h **************

// ********* Begin Mac header Files.h **************

#if defined(_PLATFORM_WIN32_)
typedef unsigned char ODFilePathname[MAX_PATH + 1];
#endif // defined(_PLATFORM_WIN32_)
#if defined(_PLATFORM_OS2_)
typedef unsigned char ODFilePathname[CCHMAXPATH + 1];
#endif // defined(_PLATFORM_OS2_)
#if defined(_PLATFORM_UNIX_)
typedef unsigned char ODFilePathname[MAXPATHLEN + 1];
#endif // defined(_PLATFORM_UNIX_)


struct FSSpec {
 // short vRefNum;
 // long parID;
 ODFilePathname name;
};

typedef FSSpec *FSSpecPtr;

// Note:  Keep in sync with PlfmFile.h
#define fsRdPerm      0x0001
#define fsWrPerm      0x0002
#define fsRdWrPerm    0x0003
#define fsRdWrShPerm  0x000F

#if defined(_PLATFORM_WIN32_)
#define fsFromStart SEEK_SET
#define fsFromMark  SEEK_CUR
#define fsFromLEOF  SEEK_END
#endif // defined(_PLATFORM_WIN32_)

#if defined(_PLATFORM_OS2_)
#define fsFromStart FILE_BEGIN
#define fsFromMark  FILE_CURRENT
#define fsFromLEOF  FILE_END
#endif // defined(_PLATFORM_OS2_)

#if defined(_PLATFORM_UNIX_)
#define fsFromStart SEEK_SET 
#define fsFromMark  SEEK_CUR  
#define fsFromLEOF  SEEK_END  
#endif // defined(_PLATFORM_UNIX_)

// ********* End Mac header Files.h **************
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#define smRoman  0    // default ??


// (keyboard) language codes copied from PMWINP.H
#define LG_BE         2       /* Belgium translation table */
#define LG_CF         3       /* Canadian-French translation table */
#define LG_DK         4       /* Denmark translation table */
#define LG_FR         5       /* France translation table */
#define LG_GR         6       /* Germany translation table */
#define LG_IT         7       /* Italy translation table */
#define LG_LA         8       /* Latin-American Spanish translation table */
#define LG_NL         9       /* Netherlands translation table */
#define LG_NO        10       /* Norway translation table */
#define LG_PO        11       /* Portugal translation table */
#define LG_SF        12       /* Swiss-French translation table */
#define LG_SG        13       /* Swiss-German translation table */
#define LG_SP        14       /* Spain translation table */
#define LG_SU        15       /* Finland translation table */
#define LG_SV        16       /* Sweden translation table */
#define LG_UK        17       /* United Kingdom translation table */
#define LG_US        18       /* United States translation table */
#define LG_TR        22       /* Turkish */
#define LG_CS        23       /* Czech */
#define LG_HU        25       /* Hungarian */
#define LG_YU        26       /* Yugoslav */
#define LG_PL        27       /* Polish */
#define LG_IC        28       /* Icelandic */
#define LG_BR        29       /* Brazil */
#define LG_HE        31       /* Hebrew Kbd   - National layer (Bidi) */
#define LG_AR        33       /* Arabic Kbd   - National layer (Bidi) */
#define LG_KATAKANA     0x51
#define LG_KATAR        0x52
#define LG_HIRAGANA     0x53
#define LG_HIRAR        0x54
#define langEnglish  LG_UK

// MAP - moved from ODTYPESF.H
inline Ptr NewPtr( unsigned long size)
{
  return new char[size];
}

inline void DisposPtr( Ptr ptr)
{
  delete ptr;
}

inline void* BlockMove( void* target, void* source, unsigned long size)
{
  return memmove( target, source, size);
}

inline unsigned long MemError()
{
  return 0;
}

// end of moved stuff from ODTYPESF.H


#endif
