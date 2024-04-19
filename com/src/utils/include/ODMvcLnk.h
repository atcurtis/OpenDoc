/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odutils
 *
 *   CLASSES: none
 *
 *   ORIGINS: 27
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
// @(#) 1.3 com/src/utils/include/ODMvcLnk.h, odutils, od96os2, odos29646d 10/30/96 09:22:27 [ 11/15/96 15:55:07 ]

#ifndef _ODMVCLNK_
#define _ODMVCLNK_


#include <somltype.h>
/* DEFAULT: no defines from somltype.h means define it to NULL/nothing */
#if !defined(WIN32_DLLIMPORT)
  #define WIN32_DLLIMPORT
#endif
#if !defined(WIN32_DLLEXPORT)
  #define WIN32_DLLEXPORT
#endif

// Set the default definition for _DLLIMPORTEXPORT_
#ifdef _OD_DONT_IMPORT_CPP_
  #define _DLLIMPORTEXPORT_ WIN32_DLLEXPORT

  // You can't export constructors with the VisualAge compiler,
  // but you may need to with the Microsoft C++ compiler.
  #ifdef __IBMCPP__
    #define _DLLCTIMPORTEXPORT_
  #else
    #define _DLLCTIMPORTEXPORT_ WIN32_DLLEXPORT
  #endif
#else
  #define _DLLIMPORTEXPORT_ WIN32_DLLIMPORT

  // You can't import constructors with the VisualAge compiler,
  // but you may need to with the Microsoft C++ compiler.
  #ifdef __IBMCPP__
    #define _DLLCTIMPORTEXPORT_
  #else
    #define _DLLCTIMPORTEXPORT_ WIN32_DLLIMPORT
  #endif
#endif

// Set the default definition for _BCIMPORTEXPORT_
#ifdef _OD_DONT_IMPORT_BC_
  #define _BCIMPORTEXPORT_ WIN32_DLLEXPORT

  // You can't export constructors with the VisualAge compiler,
  // but you may need to with the Microsoft C++ compiler.
  #ifdef __IBMCPP__
    #define _BCCTIMPORTEXPORT_
  #else
    #define _BCCTIMPORTEXPORT_ WIN32_DLLEXPORT
  #endif
#else
  #define _BCIMPORTEXPORT_ WIN32_DLLIMPORT

  // You can't import constructors with the VisualAge compiler,
  // but you may need to with the Microsoft C++ compiler.
  #ifdef __IBMCPP__
    #define _BCCTIMPORTEXPORT_
  #else
    #define _BCCTIMPORTEXPORT_ WIN32_DLLIMPORT
  #endif
#endif


#endif // _ODMVCLNK_

