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
// @(#) 1.4 com/src/utils/include/POUtils.h, odutils, od96os2, odos29646d 7/15/96 18:01:35 [ 11/15/96 15:48:19 ]
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
/*  IBM Change History (most recent first):                         */
/*  146079 12/06/95 ced  make calling convention APIENTRY           */
/*                                                                  */
/********************************************************************/
/*
  File:    POUtils.h

  Contains:  Utilities for PersistentObject

  Owned by:  Vincent Lo

  Copyright:  © 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <4>    6/7/95  jpa    Made it compile in C. [1244001]
     <3>   5/25/95  jpa    Use #pragma import. [1241078]
     <2>   4/24/95  TJ    Changed the #include StorageU.h to
                  StorageU.xh
     <1>   4/16/95  VL    first checked in

  To Do:

  Theory of operation:
  
  At InitXXXX or InitXXXXFromStorage, the client should call the appropriate
  Init procedures in this file (InitCreationDate, InitModifcationDate, InitUserName).
  At CloneInto, the client should call the corresponding Clone procedures in
  this file (CloneCreationDate, CloneModifcationDate, CloneUserName).
  When the modification info (i.e., mod date or user), the client should call the appropriate Externalize procedures
  int this file (ExternalizeModifcationDate, ExternalizeUserName).
  
  In Progress:
    
*/

#ifndef _POUTILS_
#define _POUTILS_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifdef __cplusplus
  class ODStorageUnit;
#else
  #ifndef SOM_ODStorageUnit_h
  #include <StorageU.h>
  #endif
#endif

#ifdef _OD_IMPL_
#pragma import on
#endif

#ifdef _PLATFORM_AIX_
#ifndef APIENTRY
#define APIENTRY
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* To be called at Init time */

void APIENTRY InitDateInfo(Environment* ev, ODStorageUnit* su);
void APIENTRY InitModificationInfo(Environment* ev, ODStorageUnit* su);

/* To be called at CloneInto time */

void APIENTRY CloneDateInfo(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU);
void APIENTRY CloneModificationInfo(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU);

/* To be called whenever the content of a part is changed (i.e., the mod date needs  */
/* to be changed.                                  */

void APIENTRY UpdateDateInfo(Environment* ev, ODStorageUnit* su);
void APIENTRY UpdateModificationInfo(Environment* ev, ODStorageUnit* su);

#ifdef __cplusplus
}  // End of extern "C" {
#endif

#ifdef _OD_IMPL_
#pragma import off
#endif

#endif // _POUTILS_
