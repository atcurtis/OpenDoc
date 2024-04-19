/* @(#)Z 1.15 com/src/binding/PartWrap.cpp, odconfig, od96os2, odos29646d 96/11/15 15:24:43 (96/10/28 13:37:56) */

//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odconfig
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
/*  <OS2>  11/27/95 MAP  143759    Added linking ODPart methods     */
/*  <OS2>  08/24/95 JSO  133968    - Port dr3   Apple <18-28>       */
/*  <OS2>  04/28/95 JSO  120604    - Port B1C16 Apple <14-17>       */
/*  <OS2>  04/18/95 JSO  119426    - added increment ref count      */
/*                       - so UseEditor would not cause trap        */
/*                                                                  */
/********************************************************************/
/*
  File:    PartWrap.cpp

  Contains:  Implementation of class ODPartWrapper

  Owned by:  Caia Grisar

  Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

    <29>  04/11/96 CBH    Ported to NT platform
    <28>   6/19/95  VL    1170098: Used DeleteRealPart.
    <27>    6/2/95  CC    1255477: Deligate EditInLinkAttempted to
                  fPart.
    <26>   5/26/95  DM    1251403: Multithreading naming support.
    <25>   5/25/95  jpa    Fixed usage of ODDebug. [1253321]
    <24>   5/17/95  VL    1170098: Moved creation of real part back
                  to draft.
    <23>   5/17/95  RR    #1250135/1250137/1250143 Getters increment
                  refcount
    <22>    5/1/95  CG    #1241628  BB: SOM_TRY blocks needed in
                  Binding SOM methods.
    <21>   4/30/95  T‚    1241263 BB: ODPart::Externalize should
                  initialize part name if the part name is
                  empty
    <20>   4/13/95  CG    #1231608: UseEditor should do nothing if
                  parameter is current editor.
    <19>   4/13/95  JBS    1233726 add ClonePartInfo; 1238250 Frame
                  PartInfo should have multiple values;
                  1228451 call draft->ReleasePart when
                  refcount reaches 0
    <18>    4/6/95  RR    # 1180527 Check for unrelinquished foci in
                  FrameClosed and FrameRemoved
    <17>    3/7/95  CC    1224121: CreateLink() returns an
                  ODLinkSource object.
    <16>   2/24/95  CG    1147225 BB: Changing Editors in Part Info
                  fixed.
    <15>   2/16/95  CG    #1210975 BB:  New kODErrPartInUse error.
    <14>   2/16/95  CG    #1209517 BB: Undo stack is cleared when
                  editor is changed
    <13>   1/26/95  eeh    1213354: ODActionData now an ODByteArray.
    <12>   1/22/95  NP    EmbeddedFrameSpec signature changed because
                  of change to AE types.
    <11>   1/20/95  JBS    1195017, 1205669: part API changes
    <10>   1/13/95  RR    1192147, 1201708 Removed overrides of
                  HandleEventInEmbedded, MouseEnter,
                  MouseWithin and MouseLeave. Moved
                  functionality to HandleEvent
     <9>  12/23/94  JBS    1186869: hide ODPartWrapper from public
                  API; 1192557: ODPartWrapper realPart access
     <8>  12/20/94  CG    #1201428: Added SOM_CATCH to all methods.
     <7>  12/15/94  CC    1197221 CreateLink() takes ODByteArray
                  parameter.
                  1205684 LinkUdated parameter renamed to
                  "change" from "id".
     <6>  10/27/94  CG    #1147225: Removed call to ODDeleteObject in
                  UseEditor() which was causing editor
                  swapping to crash.
     <5>   9/29/94  RA    1189812: Mods for 68K build.
     <4>   9/23/94  CC    1188476 - InitPartWrapper() does not
                  terminate ODEditor string of wrapped
                  editor.
     <3>   9/21/94  CG    #1187347: Part Editor now save the current
                  editor for part in fEditor.
     <2>   9/19/94  T‚    #1187347 PartWrapper Needs a mechanism for
                  getting the current editor & real part
     <1>    9/9/94  CG    first checked in
     <7>    9/1/94  CC    RADAR #1183005 - change AdjustBorderShape
                  param from Frame to Facet (for JBS)
     <6>   8/28/94  VL    #1182975: DragEnter & DragWithin should
                  return ODDragResult (instead of ODBoolean).
     <5>   8/26/94  T‚    #1183531 PartWrapper implementation, delete
                  _fPart when somUninit is called.
     <4>   8/25/94  T‚    #1183531 PartWrapper implementation
     <3>   8/16/94  VL    1143605: Fixed parameters to CloneInto.
     <2>   8/15/94  T‚    #1180814 Fix InitPart methods to comply
                  with new Part API
     <1>   8/13/94  T‚    first checked in

  In Progress:

*/

#define ODPartWrapper_Class_Source
#define VARIABLE_MACROS
#include <PartWrap.xih>

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef _ODNEWOBJ_
#include "ODNewObj.h"
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif

#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _INFOUTIL_
#include "InfoUtil.h"
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef SOM_Apple_NoPart_xh
#include <NoPart.xh>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef SOM_ODUndo_xh
#include <Undo.xh>
#endif

#if DEBUG
#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include "Foci.xh"
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif
#endif // DEBUG

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_ODSession_xh
#include "ODSessn.xh"
#endif

// include for ODSUExistsThenFocus definition
#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment  PartWrapper
#endif

//==============================================================================
// ODPartWrapper
//==============================================================================

SOM_Scope void  SOMLINK ODPartWrapperInitPartWrapper(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

}

SOM_Scope void  SOMLINK ODPartWrapperSetRealPart(ODPartWrapper *somSelf, Environment *ev,
    ODPart* realPart, ODEditor editor)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart = realPart;

  ODUShort length = (ODUShort)ODISOStrLength(editor);
  _fEditor = (ODEditor)ODNewPtrClear( length+1 );
  ODBlockMove(editor,  _fEditor, length+1 );

  SOM_CATCH_ALL
  SOM_ENDTRY

}

SOM_Scope void  SOMLINK ODPartWrapperUseEditor(ODPartWrapper *somSelf, Environment *ev,
    ODEditor editor)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  ODULong currentRefCount = 0;
  ODStorageUnit* su = kODNULL;
  ODDraft* draft = kODNULL;
  ODPart* newRealPart = kODNULL;
  ODULong i = 0;

  if ( !ODISOStrEqual( editor,somSelf->GetEditor(ev) ) )
  {
    if (_fPartInUse)
      ODSetSOMException(ev, kODErrPartInUse);
    else
    {
      if (_fPart)
      {
        currentRefCount = _fPart->GetRefCount(ev);
        su = _fPart->GetStorageUnit(ev);
        su->Acquire(ev);

        su->GetSession(ev)->GetUndo(ev)->ClearActionHistory(ev,
                                 kODDontRespectMarks);

        _fPart->Externalize(ev);

        for (i=currentRefCount; i>0; --i)
          _fPart->Release(ev);

        draft = su->GetDraft(ev);
        draft->ReleaseRealPart(ev, _fPart);
        newRealPart = draft->ConstructRealPart(ev, su, kODTrue, somSelf, kODNULL, editor);

        su->Release(ev);
      }
      else
      {
        newRealPart = draft->ConstructRealPart(ev, kODNULL, kODFalse, somSelf, kODNULL, editor);
      }

      somSelf->SetRealPart(ev, newRealPart, editor);

      if (_fPart)
      {
        for (i=currentRefCount; i>0; --i)
          _fPart->Acquire(ev);
      }
    }
  }

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODEditor  SOMLINK ODPartWrapperGetEditor(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  return _fEditor;
}

SOM_Scope void  SOMLINK ODPartWrappersomInit(ODPartWrapper *somSelf)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);


  _fPart = kODNULL;
    parent_somInit(somSelf);

}

SOM_Scope void  SOMLINK ODPartWrappersomUninit(ODPartWrapper *somSelf)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  Environment* ev = somGetGlobalEnvironment();

  SOM_TRY

  ODDeleteObject(_fEditor);
  _fPart->GetStorageUnit(ev)->GetDraft(ev)->DeleteRealPart(ev, _fPart);
    parent_somUninit(somSelf);

  SOM_CATCH_ALL
  SOM_ENDTRY

}

SOM_Scope void  SOMLINK ODPartWrapperFulfillPromise(ODPartWrapper *somSelf, Environment *ev,
    ODStorageUnitView* promiseSUView)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->FulfillPromise(ev,promiseSUView);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperDropCompleted(ODPartWrapper *somSelf, Environment *ev,
    ODPart* destPart,
    ODDropResult dropResult)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->DropCompleted(ev,destPart,dropResult);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODDragResult  SOMLINK ODPartWrapperDragEnter(ODPartWrapper *somSelf, Environment *ev,
    ODDragItemIterator* dragInfo,
    ODFacet* facet,
    ODPoint* where)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
    ODDragResult  dragResult = kODFalse;

  SOM_TRY

  dragResult = _fPart->DragEnter(ev,dragInfo,facet,where);

  SOM_CATCH_ALL
  SOM_ENDTRY

    return dragResult;
}

SOM_Scope ODDragResult  SOMLINK ODPartWrapperDragWithin(ODPartWrapper *somSelf, Environment *ev,
    ODDragItemIterator* dragInfo,
    ODFacet* facet,
    ODPoint* where)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
    ODDragResult  dragResult = kODFalse;

  SOM_TRY

  dragResult = _fPart->DragWithin(ev,dragInfo,facet,where);

  SOM_CATCH_ALL
  SOM_ENDTRY

    return dragResult;
}

SOM_Scope void  SOMLINK ODPartWrapperDragLeave(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* facet,
    ODPoint* where)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->DragLeave(ev,facet,where);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODDropResult  SOMLINK ODPartWrapperDrop(ODPartWrapper *somSelf, Environment *ev,
    ODDragItemIterator* dropInfo,
    ODFacet* facet,
    ODPoint* where)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
#ifdef _PLATFORM_MACINTOSH_
    ODDragResult  dragResult = kODDropFail;
#else
    ODDropResult  dragResult = kODDropFail;
#endif

  SOM_TRY

  dragResult = _fPart->Drop(ev,dropInfo,facet,where);

  SOM_CATCH_ALL
  SOM_ENDTRY

    return dragResult;

}

SOM_Scope void  SOMLINK ODPartWrapperContainingPartPropertiesUpdated(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame,
    ODStorageUnit* propertyUnit)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->ContainingPartPropertiesUpdated(ev,frame,propertyUnit);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODStorageUnit*  SOMLINK ODPartWrapperAcquireContainingPartProperties(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODStorageUnit* partProps = kODNULL;

  SOM_TRY

    partProps = _fPart->AcquireContainingPartProperties(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY

    return partProps;
}

SOM_Scope ODBoolean  SOMLINK ODPartWrapperRevealFrame(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* embeddedFrame,
    ODShape* revealShape)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODBoolean  revealFrame = kODFalse;

  SOM_TRY

    revealFrame = _fPart->RevealFrame(ev,embeddedFrame,revealShape);

  SOM_CATCH_ALL
  SOM_ENDTRY

    return revealFrame;
}

SOM_Scope void  SOMLINK ODPartWrapperEmbeddedFrameSpec(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* embeddedFrame,
    ODObjectSpec* spec)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->EmbeddedFrameSpec(ev,embeddedFrame,spec);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODEmbeddedFramesIterator*  SOMLINK ODPartWrapperCreateEmbeddedFramesIterator(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODEmbeddedFramesIterator*  itr = kODNULL;

  SOM_TRY

    itr = _fPart->CreateEmbeddedFramesIterator(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY

    return itr;
}

SOM_Scope void  SOMLINK ODPartWrapperDisplayFrameAdded(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
  ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->DisplayFrameAdded(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperDisplayFrameRemoved(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->DisplayFrameRemoved(ev,frame);

#ifdef DEBUG

    // Sanity check to make sure parts have relinquished foci.
    // It would be more general to have a private ODArbitrator
    // method to determine if any frames of a part still own
    // foci

    ODStorageUnit* su = _fPart->GetStorageUnit(ev);
    ODSession* session = su->GetSession(ev);
    ODArbitrator* arbitrator = session->GetArbitrator(ev);

    ODFrame* focusFrame = kODNULL;

    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODSelectionFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameRemoved: Frame %x of Part %x still has selection focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODMenuFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameRemoved: Frame %x of Part %x still has menu focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODKeyFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameRemoved: Frame %x of Part %x still has key focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODScrollingFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameRemoved: Frame %x of Part %x still has scrolling focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODClipboardFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameRemoved: Frame %x of Part %x still has clipboard focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);

#endif

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperDisplayFrameConnected(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->DisplayFrameConnected(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperDisplayFrameClosed(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->DisplayFrameClosed(ev,frame);

#ifdef DEBUG

    // Sanity check to make sure parts have relinquished foci.
    // It would be more general to have a private ODArbitrator
    // method to determine if any frames of a part still own
    // foci

    ODStorageUnit* su = _fPart->GetStorageUnit(ev);
    ODSession* session = su->GetSession(ev);
    ODArbitrator* arbitrator = session->GetArbitrator(ev);

    ODFrame* focusFrame = kODNULL;

    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODSelectionFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameClosed: Frame %x of Part %x still has selection focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODMenuFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameClosed: Frame %x of Part %x still has menu focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODKeyFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameClosed: Frame %x of Part %x still has key focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODScrollingFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameClosed: Frame %x of Part %x still has scrolling focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);
    focusFrame = arbitrator->AcquireFocusOwner(ev, session->Tokenize(ev, kODClipboardFocus));
    if (focusFrame && ODObjectsAreEqual(ev, frame, focusFrame))
      WARNMSG_DEBUG(WARN_INDEX(-1),"DisplayFrameClosed: Frame %x of Part %x still has clipboard focus", frame, _fPart);
    ODReleaseObject(ev, focusFrame);

#endif

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperAttachSourceFrame(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame,
    ODFrame* sourceFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->AttachSourceFrame(ev,frame,sourceFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperFrameShapeChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->FrameShapeChanged(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperViewTypeChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->ViewTypeChanged(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperPresentationChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->PresentationChanged(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperSequenceChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->SequenceChanged(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODInfoType  SOMLINK ODPartWrapperReadPartInfo(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame,
    ODStorageUnitView* storageUnitView)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODInfoType partInfo = kODNULL;

  SOM_TRY

    partInfo = _fPart->ReadPartInfo(ev,frame,storageUnitView);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return partInfo;
}

SOM_Scope void  SOMLINK ODPartWrapperWritePartInfo(ODPartWrapper *somSelf, Environment *ev,
    ODInfoType partInfo,
    ODStorageUnitView* storageUnitView)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->WritePartInfo(ev,partInfo,storageUnitView);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperClonePartInfo(ODPartWrapper *somSelf, Environment *ev,
    ODDraftKey key,
    ODInfoType partInfo,
    ODStorageUnitView* storageUnitView,
    ODFrame* scopeFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->ClonePartInfo(ev,key,partInfo,storageUnitView,scopeFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODID  SOMLINK ODPartWrapperOpen(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODID  id = 0;

  SOM_TRY

  id = _fPart->Open(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return id;
}

SOM_Scope ODFrame*  SOMLINK ODPartWrapperRequestEmbeddedFrame(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* containingFrame,
    ODFrame* baseFrame,
    ODShape* frameShape,
    ODPart* embedPart,
    ODTypeToken viewType,
    ODTypeToken presentation,
    ODBoolean isOverlaid)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODFrame*  frame = kODNULL;

  SOM_TRY

  frame = _fPart->RequestEmbeddedFrame(ev,containingFrame,baseFrame,frameShape,embedPart,viewType,presentation,isOverlaid);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return frame;
}

SOM_Scope void  SOMLINK ODPartWrapperRemoveEmbeddedFrame(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* embeddedFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->RemoveEmbeddedFrame(ev,embeddedFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODShape*  SOMLINK ODPartWrapperRequestFrameShape(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* embeddedFrame,
    ODShape* frameShape)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODShape*  shape = kODNULL;

  SOM_TRY

  shape = _fPart->RequestFrameShape(ev,embeddedFrame,frameShape);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return shape;
}

SOM_Scope void  SOMLINK ODPartWrapperUsedShapeChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* embeddedFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->UsedShapeChanged(ev,embeddedFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODShape*  SOMLINK ODPartWrapperAdjustBorderShape(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* embeddedFacet,
    ODShape* shape)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODShape*  borderShape = kODNULL;

  SOM_TRY

    borderShape = _fPart->AdjustBorderShape(ev,embeddedFacet,shape);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return borderShape;
}

SOM_Scope void  SOMLINK ODPartWrapperFacetAdded(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* facet)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->FacetAdded(ev,facet);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperFacetRemoved(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* facet)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->FacetRemoved(ev,facet);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperCanvasChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* facet)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->CanvasChanged(ev,facet);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperGeometryChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* facet,
    ODBoolean clipShapeChanged,
    ODBoolean externalTransformChanged)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->GeometryChanged(ev,facet,clipShapeChanged,externalTransformChanged);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperDraw(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* facet,
    ODShape* invalidShape)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->Draw(ev,facet,invalidShape);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperCanvasUpdated(ODPartWrapper *somSelf, Environment *ev,
    ODCanvas* canvas)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->CanvasUpdated(ev,canvas);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperHighlightChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFacet* facet)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->HighlightChanged(ev,facet);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODULong  SOMLINK ODPartWrapperGetPrintResolution(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODULong  printRes = 0;

  SOM_TRY

  printRes = _fPart->GetPrintResolution(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return printRes;
}

SOM_Scope ODLinkSource*  SOMLINK ODPartWrapperCreateLink(ODPartWrapper *somSelf, Environment *ev,
    ODByteArray* data)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODLinkSource*  link = kODNULL;

  SOM_TRY

  link = _fPart->CreateLink(ev,data);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return link;
}

SOM_Scope void  SOMLINK ODPartWrapperLinkUpdated(ODPartWrapper *somSelf, Environment *ev,
    ODLink* updatedLink,
    ODUpdateID change)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->LinkUpdated(ev,updatedLink,change);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperRevealLink(ODPartWrapper *somSelf, Environment *ev,
    ODLinkSource* linkSource)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->RevealLink(ev,linkSource);

  SOM_CATCH_ALL
  SOM_ENDTRY
}


#ifdef _PLATFORM_OS2_ // OS/2 specific linking ODPart methods

SOM_Scope void  SOMLINK ODPartWrapperBreakLink(ODPartWrapper *somSelf,
                                               Environment *ev,
                                              ODLink* link)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

#if 0 // defect 22580 - comment out until OS/2 Linking is implemented
  _fPart->BreakLink(ev,link);
#endif

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperBreakLinkSource(ODPartWrapper *somSelf,
                                                     Environment *ev,
                                                    ODLinkSource* linkSource)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

#if 0 // defect 22580 - comment out until OS/2 Linking is implemented
  _fPart->BreakLinkSource(ev, linkSource);
#endif

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperLinkBroken(ODPartWrapper *somSelf,
                                                Environment *ev,
                                               ODLink* link)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);


  SOM_TRY

#if 0 // defect 22580 - comment out until OS/2 Linking is implemented
  _fPart->LinkBroken(ev, link);
#endif

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperUpdateFromLinkSource(ODPartWrapper *somSelf,
                                                          Environment *ev,
                                                         ODLinkSource* linkSource)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

#if 0 // defect 22580 - comment out until OS/2 Linking is implemented
  _fPart->UpdateFromLinkSource(ev, linkSource);
#endif

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperShowLink(ODPartWrapper *somSelf,
                                              Environment *ev,
                                             ODLink* link)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

#if 0 // defect 22580 - comment out until OS/2 Linking is implemented
  _fPart->ShowLink(ev, link);
#endif

  SOM_CATCH_ALL
  SOM_ENDTRY
}

#endif  // OS/2 specific linking ODPart methods


SOM_Scope void  SOMLINK ODPartWrapperEmbeddedFrameUpdated(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame,
    ODUpdateID change)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->EmbeddedFrameUpdated(ev,frame,change);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODBoolean SOMLINK ODPartWrapperEditInLinkAttempted(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
    ODBoolean result = kODFalse;

  SOM_TRY

  result = _fPart->EditInLinkAttempted(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return result;
}

SOM_Scope void  SOMLINK ODPartWrapperLinkStatusChanged(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->LinkStatusChanged(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODBoolean  SOMLINK ODPartWrapperBeginRelinquishFocus(ODPartWrapper *somSelf, Environment *ev,
    ODTypeToken focus,
    ODFrame* ownerFrame,
    ODFrame* proposedFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODBoolean  relinqFocus = kODFalse;

  SOM_TRY

  relinqFocus = _fPart->BeginRelinquishFocus(ev,focus,ownerFrame,proposedFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return relinqFocus;
}

SOM_Scope void  SOMLINK ODPartWrapperCommitRelinquishFocus(ODPartWrapper *somSelf, Environment *ev,
    ODTypeToken focus,
    ODFrame* ownerFrame,
    ODFrame* proposedFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->CommitRelinquishFocus(ev,focus,ownerFrame,proposedFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperAbortRelinquishFocus(ODPartWrapper *somSelf, Environment *ev,
    ODTypeToken focus,
    ODFrame* ownerFrame,
    ODFrame* proposedFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->AbortRelinquishFocus(ev,focus,ownerFrame,proposedFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperFocusAcquired(ODPartWrapper *somSelf, Environment *ev,
    ODTypeToken focus,
    ODFrame* ownerFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->FocusAcquired(ev,focus,ownerFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperFocusLost(ODPartWrapper *somSelf, Environment *ev,
    ODTypeToken focus,
    ODFrame* ownerFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->FocusLost(ev,focus,ownerFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperInitObject(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->InitObject(ev);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

// IsInitialized not present in base code for windows 4/18/96
#ifdef _PLATFORM_MACINTOSH_
SOM_Scope ODBoolean  SOMLINK ODPartWrapperIsInitialized(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODBoolean  isInited = kODFalse;

  SOM_TRY

  isInited = _fPart->IsInitialized(ev);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return isInited;
}
#endif

SOM_Scope ODBoolean  SOMLINK ODPartWrapperHasExtension(ODPartWrapper *somSelf, Environment *ev,
    ODType extensionName)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODBoolean  hasExt = kODFalse;

  SOM_TRY

  hasExt = _fPart->HasExtension(ev,extensionName);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return hasExt;
}

SOM_Scope ODExtension*  SOMLINK ODPartWrapperAcquireExtension(ODPartWrapper *somSelf, Environment *ev,
    ODType extensionName)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODExtension*  extName = kODNULL;

  SOM_TRY

  extName = _fPart->AcquireExtension(ev,extensionName);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return extName;
}

SOM_Scope void  SOMLINK ODPartWrapperReleaseExtension(ODPartWrapper *somSelf, Environment *ev,
    ODExtension* extension)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->ReleaseExtension(ev,extension);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODSize  SOMLINK ODPartWrapperPurge(ODPartWrapper *somSelf, Environment *ev,
    ODSize size)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODSize  purgeSize = 0;

  SOM_TRY

  purgeSize = _fPart->Purge(ev,size);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return purgeSize;
}

SOM_Scope ODBoolean  SOMLINK ODPartWrapperIsEqualTo(ODPartWrapper *somSelf, Environment *ev,
    ODObject* object)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODBoolean  isEqualTo = kODFalse;

  SOM_TRY

  isEqualTo = _fPart->IsEqualTo(ev,object);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return isEqualTo;
}

SOM_Scope void  SOMLINK ODPartWrapperSubClassResponsibility(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->SubClassResponsibility(ev);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperInitRefCntObject(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->InitRefCntObject(ev);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperAcquire(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->Acquire(ev);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperRelease(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->Release(ev);
  if ( _fPart->GetRefCount(ev) == 0 ) {
    _fPart->GetStorageUnit(ev)->GetDraft(ev)->ReleasePart(ev, somSelf);
  }

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODULong  SOMLINK ODPartWrapperGetRefCount(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODULong  refCount = 0;

  SOM_TRY

    refCount = _fPart->GetRefCount(ev);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return refCount;
}

SOM_Scope void  SOMLINK ODPartWrapperInitPersistentObject(ODPartWrapper *somSelf, Environment *ev,
    ODStorageUnit* storageUnit)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->InitPersistentObject(ev,storageUnit);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperInitPersistentObjectFromStorage(ODPartWrapper *somSelf, Environment *ev,
    ODStorageUnit* storageUnit)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

   _fPart->InitPersistentObjectFromStorage(ev,storageUnit);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperReleaseAll(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->ReleaseAll(ev);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperExternalize(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->Externalize(ev);

  ODStorageUnit* su = somSelf->GetStorageUnit(ev);

  if (su) {
    //  replaced kODOS2IText with kODISOStr, 4/18/96
    if ( !ODSUExistsThenFocus(ev, su, kODPropName, kODIntlText) ||
         (su->GetSize(ev) == 0)
       )
        {
        ODName* category = ODGetCatFromPartSU(ev, ODGetSUFromPstObj(ev, somSelf),
                                  su->GetSession(ev)->GetNameSpaceManager(ev));

        if (category) ODSetPOName(ev, somSelf, category);
        }
  }

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODStorageUnit*  SOMLINK ODPartWrapperGetStorageUnit(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

    SOM_TRY

    return (_fPart->GetStorageUnit(ev));


    SOM_CATCH_ALL
    return kODNULL;
    SOM_ENDTRY

}

SOM_Scope ODID  SOMLINK ODPartWrapperGetID(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);


  SOM_TRY

    return (_fPart->GetID(ev));

  SOM_CATCH_ALL
    return 0;
  SOM_ENDTRY


}

SOM_Scope void  SOMLINK ODPartWrapperCloneInto(ODPartWrapper *somSelf, Environment *ev,
    ODDraftKey key,
    ODStorageUnit* storageUnit,
    ODFrame* initiatingFrame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->CloneInto(ev,key,storageUnit,initiatingFrame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperExternalizeKinds(ODPartWrapper *somSelf, Environment *ev,
    ODTypeList* kindset)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->ExternalizeKinds(ev,kindset);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperChangeKind(ODPartWrapper *somSelf, Environment *ev,
    ODType kind)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->ChangeKind(ev,kind);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODBoolean  SOMLINK ODPartWrapperIsRealPart(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  return kODFalse;
}

SOM_Scope ODPart*  SOMLINK ODPartWrapperGetRealPart(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODPart*  realPart = kODNULL;

  SOM_TRY

  if ( _fPartInUse )
    ODSetSOMException(ev, kODErrPartInUse);
  else
  {
    _fPartInUse = kODTrue;
    realPart = _fPart;
  }

  SOM_CATCH_ALL
  SOM_ENDTRY

  return realPart;
}

SOM_Scope void  SOMLINK ODPartWrapperReleaseRealPart(ODPartWrapper *somSelf, Environment *ev)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPartInUse = kODFalse;

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODBoolean  SOMLINK ODPartWrapperHandleEvent(ODPartWrapper *somSelf, Environment *ev,
    ODEventData* event,
    ODFrame* frame,
    ODFacet* facet,
    ODEventInfo* eventInfo)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);
  ODBoolean  eventHandled = kODFalse;

  SOM_TRY

    eventHandled = _fPart->HandleEvent(ev,event,frame,facet, eventInfo);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return eventHandled;
}

SOM_Scope void  SOMLINK ODPartWrapperAdjustMenus(ODPartWrapper *somSelf, Environment *ev,
    ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->AdjustMenus(ev,frame);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperUndoAction(ODPartWrapper *somSelf, Environment *ev,
    ODActionData* actionState)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->UndoAction(ev,actionState);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperRedoAction(ODPartWrapper *somSelf, Environment *ev,
    ODActionData* actionState)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->RedoAction(ev,actionState);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperDisposeActionState(ODPartWrapper *somSelf, Environment *ev,
    ODActionData* actionState,
    ODDoneState doneState)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->DisposeActionState(ev,actionState,doneState);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperWriteActionState(ODPartWrapper *somSelf, Environment *ev,
    ODActionData* actionState,
    ODStorageUnitView* storageUnitView)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->WriteActionState(ev,actionState,storageUnitView);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope ODActionData  SOMLINK ODPartWrapperReadActionState(ODPartWrapper *somSelf, Environment *ev,
    ODStorageUnitView* storageUnitView)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  ODActionData data;
  data._maximum = 0;
  data._length = 0;
  data._buffer = kODNULL;

  SOM_TRY

    data = _fPart->ReadActionState(ev,storageUnitView);

  SOM_CATCH_ALL
  SOM_ENDTRY

  return data;
}

SOM_Scope void  SOMLINK ODPartWrapperInitPart(ODPartWrapper *somSelf, Environment *ev,
    ODStorageUnit* storageUnit, ODPart* partWrapper)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

    _fPart->InitPart(ev,storageUnit, somSelf);

  SOM_CATCH_ALL
  SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODPartWrapperInitPartFromStorage(ODPartWrapper *somSelf, Environment *ev,
    ODStorageUnit* storageUnit, ODPart* partWrapper)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

  SOM_TRY

  _fPart->InitPartFromStorage(ev,storageUnit, somSelf);

  SOM_CATCH_ALL
  SOM_ENDTRY
}
#ifdef _PLATFORM_MACINTOSH_
SOM_Scope ODMenuBar*  SOMLINK ODPartWrapperCreateRootMenuBar(ODPartWrapper *somSelf,
                                                             Environment *ev,
                                                             ODFrame* frame)
{
    ODPartWrapperData *somThis = ODPartWrapperGetData(somSelf);

    SOM_TRY

    return _fPart->CreateRootMenuBar(ev, frame);

    SOM_CATCH_ALL
    return 0;
    SOM_ENDTRY
}
#endif
