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
// @(#) 1.8 com/src/utils/POUtils.cpp, odutils, od96os2, odos29646d 9/26/96 14:37:46 [ 11/15/96 15:48:19 ]
/*
  File:    POUtils.cpp

  Contains:  Utilities for handling standard persistent object properties

  Owned by:  Vincent Lo

  Copyright:  © 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <6>   6/15/95  TJ    Added Includes for compiling with out PCH
     <5>   6/13/95  VL    1241352: Called su::Exteranlize in Clone to
                  force update of moddate. Also added WARN to
                  UpdateModificationInfo.
     <4>    5/1/95  T‚    1234420 StdTypIO functions should not
                  return default values
     <3>   4/25/95  EL    1242436: Clone part name and comment.
     <2>   4/24/95  TJ    Included ODSessn.xh, and ODMemory.h.
     <1>   4/16/95  VL    first checked in
  To Do:

  Theory of operation:

  At InitXXXX or InitXXXXFromStorage, the client should call the appropriate
  Init procedures in this file (InitDateInfo or InitModifcationInfo).
  At CloneInto, the client should call the corresponding Clone procedures in
  this file (CloneDateInfo or CloneModifcationInfo).
  When the persistent object wants to update its modification info, it
  should call the appropriate Update procedures (UpdateDateInfo or
  UpdateModifcationInfo).

*/

#ifndef _POUTILS_
#include <POUtils.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _TIME
#include <time.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

// Added for DR4
#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

static void InitUserName(Environment* ev, ODStorageUnit* su);
static void CloneUserName(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU);
static void UpdateUserName(Environment* ev, ODStorageUnit* su);
static void ClonePartName(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU);
static void ClonePartComment(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU);

//------------------------------------------------------------------------------
// InitDateInfo
//
// This procedure is called in the Init or InitFromStorage of the persistent object.
// It writes out the creation date and the modification date of the object.
//------------------------------------------------------------------------------

void InitDateInfo(Environment* ev, ODStorageUnit* su)
{
  ODULong creationTimeDate;
  time((time_t *)(&creationTimeDate));

  if (su->Exists(ev, kODPropCreateDate, kODTime_T, 0) == kODFalse) {
    ODSUForceFocus(ev, su, kODPropCreateDate, kODTime_T);
    ODSetTime_TProp(ev, su, kODPropCreateDate, kODTime_T, creationTimeDate);
  }
  if (su->Exists(ev, kODPropModDate, kODTime_T, 0) == kODFalse) {
    ODSUForceFocus(ev, su, kODPropModDate, kODTime_T);
    ODSetTime_TProp(ev, su, kODPropModDate, kODTime_T, creationTimeDate);
  }
}

//------------------------------------------------------------------------------
// InitModificationInfo
//
// This procedure is called in the Init or InitFromStorage of the persistent object.
// It writes out the modification info of the object.
//------------------------------------------------------------------------------

void InitModificationInfo(Environment* ev, ODStorageUnit* su)
{
  InitDateInfo(ev, su);
  InitUserName(ev, su);
}

//------------------------------------------------------------------------------
// CloneDateInfo
//
// This procedure is called in the CloneInto method of the persistent object.
// It clones creation date and the modification date.
//------------------------------------------------------------------------------

void CloneDateInfo(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU)
{
  ODULong creationTimeDate = ODGetTime_TProp(ev, fromSU, kODPropCreateDate, kODTime_T);

  if (toSU->Exists(ev, kODPropCreateDate, kODTime_T, 0) == kODFalse)
    ODSUForceFocus(ev, toSU, kODPropCreateDate, kODTime_T);
  ODSetTime_TProp(ev, toSU, kODPropCreateDate, kODTime_T, creationTimeDate);

  ODULong modificationTimeDate = ODGetTime_TProp(ev, fromSU, kODPropModDate, kODTime_T);
  if (toSU->Exists(ev, kODPropModDate, kODTime_T, 0) == kODFalse)
    ODSUForceFocus(ev, toSU, kODPropModDate, kODTime_T);

  ODSetTime_TProp(ev, toSU, kODPropModDate, kODTime_T, modificationTimeDate);
}

//------------------------------------------------------------------------------
// CloneModificationInfo
//
// This procedure is called in the CloneInto method of the persistent object.
// It clones the modification info of the object.
//------------------------------------------------------------------------------

void CloneModificationInfo(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU)
{
  fromSU->Externalize(ev);
  CloneDateInfo(ev, fromSU, toSU);
  CloneUserName(ev, fromSU, toSU);
  ClonePartName(ev, fromSU, toSU);
  ClonePartComment(ev, fromSU, toSU);
}

//------------------------------------------------------------------------------
// UpdateDateInfo
//
// This procedure is called by persistent objects when the modification date
// needs to be updated.
//------------------------------------------------------------------------------

void UpdateDateInfo(Environment* ev, ODStorageUnit* su)
{
  WARNMSG_DEBUG(WARN_INDEX(-1),"Calling UpdateDateInfo.");

  ODULong modificationTimeDate;
  time((time_t *)(&modificationTimeDate));

  ODSetTime_TProp(ev, su, kODPropModDate, kODTime_T, modificationTimeDate);
}

//------------------------------------------------------------------------------
// UpdateModificationInfo
//
// This procedure is called by parts when the modification date and user need
// to be updated.
//------------------------------------------------------------------------------

void UpdateModificationInfo(Environment* ev, ODStorageUnit* su)
{
  WARNMSG_DEBUG(WARN_INDEX(-1),"Calling UpdateModificationInfo.");
  UpdateDateInfo(ev, su);
  UpdateUserName(ev, su);
}

//------------------------------------------------------------------------------
// InitUserName
//
// This procedure is called in InitXXX or InitXXXFromStorage of ODPart.
// It creates the property and values and writes out the user name.
//------------------------------------------------------------------------------

void InitUserName(Environment* ev, ODStorageUnit* su)
{
  if (su->Exists(ev, kODPropModUser, kODMacIText, 0) == kODFalse) {
    ODSUForceFocus(ev, su, kODPropModUser, kODMacIText);
    UpdateUserName(ev, su);
  }
}

//------------------------------------------------------------------------------
// CloneUserName
//
// This procedure is called in the CloneInto method of ODPart.
// It clones the user name.
//------------------------------------------------------------------------------

void CloneUserName(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU)
{
  ODIText* userName = ODGetITextProp(ev, fromSU, kODPropModUser, kODMacIText, kODNULL);

  if (userName)
    ODSetITextProp(ev, toSU, kODPropModUser, kODMacIText, userName);

  ODDisposePtr(userName);
}

//------------------------------------------------------------------------------
// UpdateUserName
//
// This procedure is called by parts when the user property and value needs
// to be updated.
//------------------------------------------------------------------------------

void UpdateUserName(Environment* ev, ODStorageUnit* su)
{
  ODIText* userName = CreateIText((ODSize)0);
  su->GetSession(ev)->GetUserName(ev, &userName);
  ODSetITextProp(ev, su, kODPropModUser, kODMacIText, userName);
  DisposeIText(userName);
}

//------------------------------------------------------------------------------
// ClonePartName
//
// This procedure is called in the CloneInto method of ODPart.
// It clones the part name.
//------------------------------------------------------------------------------

void ClonePartName(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU)
{
  ODIText* partName = ODGetITextProp(ev, fromSU, kODPropName, kODMacIText, kODNULL);

  if (partName && GetITextStringLength(partName))
    ODSetITextProp(ev, toSU, kODPropName, kODMacIText, partName);

  ODDisposePtr(partName);
}

//------------------------------------------------------------------------------
// ClonePartComment
//
// This procedure is called in the CloneInto method of ODPart.
// It clones the part comment.
//------------------------------------------------------------------------------

void ClonePartComment(Environment* ev, ODStorageUnit* fromSU, ODStorageUnit* toSU)
{
  ODIText* partComment = ODGetITextProp(ev, fromSU, kODPropComments, kODMacIText, kODNULL);

  if (partComment && GetITextStringLength(partComment))
    ODSetITextProp(ev, toSU, kODPropComments, kODMacIText, partComment);

  ODDisposePtr(partComment);
}


