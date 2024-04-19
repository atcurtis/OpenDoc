/* @(#)Z 1.19 com/src/layout/Frame.cpp, odlayout, od96os2, odos29646d 96/11/15 15:25:24 (96/10/09 16:38:10) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odlayout
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
/*  IBM Change History (most recent first):                         */
/*   20470 03/19/96 tmw Apple DR4 and common Windows/OS2            */
/********************************************************************/
/*
   File:       Frame.cpp

   Contains:   Implementation of ODFrame

   Owned by:   Joshua Susser

   Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

       <74>     11/2/95    RR      #1298614 Check for NULL su in PrepareToSwap
       <73>     11/1/95    JBS     1289931 GM: move release calls from
                                   somUninit to Release all, use SafeRelease
       <72>    10/24/95    T‚      Removed two errant semicolons introduced
                                   with fix of #1288922 don't force
                                   DrawActiveBorder, wait for Window::Update
       <71>    10/24/95    jpa     1293441: Ignore exceptions from
                                   DisplayFrame{Closed,Removed}.
       <70>    10/18/95    JBS     1288922 don't force DrawActiveBorder, wait
                                   for Window::Update
       <69>    10/17/95    jpa     1292659: Fixed leaks if parts throw
       <68>     10/9/95    JBS     1290561 GM: ChangeLinkStatus dirties draft
                                   unnecessarily; 1289931 GM: release objects
                                   in ReleaseAll, not somUninit
       <67>     10/8/95    TJ      Fixes Recomended by Refball
       <66>     9/21/95    JBS     1284283 window refcounting in PrepareToSwap
       <65>     9/13/95    DM      1277216 GM:API return no ODPoints nor
                                   ODPolygons
       <64>     9/12/95    JBS     1283657 part->FacedAdded called twice in
                                   Frame::FacetAdded
       <63>      9/8/95    JBS     1281770 Frame::ChangeLinkStatus short
                                   circuits
       <62>      9/8/95    T‚      1281096 FB2:Many constants in ODTypesB
                                   without kOD prefix!
       <61>      9/6/95    VL      1270294: Added and use
                                   kFrameIsInRemoveFrame to avoid recursive
                                   removal. Removed CHECKVALID in GetID.
       <60>     8/31/95    JBS     1278337 error code cleanup
       <59>     8/25/95    JBS     1263078 FB: fix part editor swapping;
                                   1278052 FB: add InLimbo flag
       <58>     8/17/95    CC      1273056: ContentUpdated acquires wrong
                                   part.
       <57>     8/16/95    NP      1274946: ErrorDef.idl problems. Add include
                                   file.
       <56>     8/12/95    T‚      1276807 Opt./Bug: use StdTypIO routines for
                                   portable streaming & smaller footprint
       <55>      8/7/95    JBS     1262428 ASSERT in AcquirePart if part NULL;
                                   1265721 don't purge facets' if invalid
       <54>      8/3/95    RR      #1257260: Collapse B classes. Remove
                                   somInit methods. Don't call IsInitialized
                                   or SubclassResponsibility
       <53>     7/27/95    JBS     1231232 SetContainingFrame - remove part
                                   notification
                                   1264303 AcquireContainingFrame - don't lazy
                                   internalize if IsRoot
       <52>     7/27/95    DM      #1270320: Mem leaks: must cast when
                                   assigning from temp ref to another in
                                   EditInLink and ContentUpdated
       <51>     7/26/95    DM      #1270320: Memory leak fixes. delete
                                   typeString returns from GetType in
                                   Externalize and CloneInto
       <50>      7/3/95    RR      1242642 BB: refcounting. SetContainingFrame
                                   needed to Acquire before calling
                                   window->CloseAndRemove.
       <49>     6/29/95    DM      1242642 BB: refcounting: acquire member
                                   vars (not locals) when possible, add
                                   "Acquire" to names of local "ReadIfAny"
                                   functions, release biasTransform    in
                                   InitFrameFromStorage.
       <48>     6/28/95    RR      1242642 BB Mostly ref counting. Release
                                   window in CLose and Remove
       <47>     6/23/95    JBS     1261323 add isSubframe param to InitFrame;
                                   1238245 (done) InitFrame throws error on
                                   non-subframe recursive embed
       <46>     6/23/95    JBS     1251627 fix refcounting throughout file
       <45>     6/21/95    JBS     1238245: (not done) InitFrame check for
                                   recursive embedding; 1243657 SetWindow
                                   throws if not root frame
       <44>     6/20/95    JBS     1257315 change AcquireWindow to GetWindow
       <43>     5/31/95    JBS     1250140 fixed refcount bug in AcquirePart
       <42>     5/26/95    RR      #1251403: Multithreading naming support
       <41>     5/17/95    RR      #1250135/1250137/1250143 Getters increment
                                   refcount
       <40>     5/17/95    JBS     1245283 Undoable frame deletion - close
                                   part windows in SetContainingFrame; 1242496
                                   change SOM_CATCH to SOM_TRY; 1250140 bump
                                   refcount in AcquireContainingFrame, AcquirePart,
                                   AcquireWindow
       <39>     5/15/95    JBS     1245156 implement Purge
       <38>      5/5/95    TJ      Put "VOID" into CHECKVALID since #$&* SCpp
                                   won't take empty macro arguments.
       <37>      5/4/95    JBS     1243651 added CHECKVALID to most methods,
                                   and fixed ternalization to respect
                                   fValidState
       <36>     4/27/95    JBS     1209506 $5 comments eliminated
       <35>     4/25/95    JBS     1242496 use SOM_TRY, etc. for exception
                                   handling; 1241983 Externalize assumes
                                   containing frame is persistent; 1183059
                                   throw kODErrIllegalNullStorageUnitInput if
                                   SU is nil; 1242527  AcquireContainingFrame
                                   should not write SU; 1242546  Frame should
                                   compute IsRoot from contFrame
       <34>     4/14/95    T‚      #1235279 BB: InfoUtil & StdTypIO functions
                                   should take Environment* and SU* when
                                   possible.  Also Clone a little better to
                                   help out Save A Copy.
       <33>     4/13/95    JBS     1211972 don't mark draft dirty if frame is
                                   non-persistent;1233726 Frame::CloneInto
                                   should call part->ClonePartInfo;1238250
                                   Frame PartInfo should have multiple
                                   values;1237963 don't validate
                                   ContainingFrame property on init
       <32>      4/3/95    CC      1234631: ChangeLinkStatus: Set _fLinkStatus
                                   BEFORE notifying the part!
       <31>     3/28/95    JBS     1229656 implemented EditInLink
       <30>     3/28/95    JBS     1229656 add EditInLink()
       <29>      3/8/95    JBS     1165158 AdjustBorderShape implementation
       <28>     2/24/95    CC      Commented out redefinition of
                                   ODFrameMethodDebug.
       <27>     2/15/95    JBS     1198600 code review cleanup; 1195854 added
                                   SetDirty, dirty/write properties
                                   individually; 1189547 don't allow viewType
                                   and pres to be NULL; 1183059 throw if NULL
                                   su in InitFrameFromStorage; 1206295 don't
                                   externalize UsedShape
       <26>     1/25/95    CC      1206295: (for JBS) Temporarily add
       <25>     1/20/95    JBS     1195017, 1205669: part API changes
       <24>     11/1/94    CC      1196908 - SetContainingFrame: remove all
                                   facets displaying this frame and call
                                   ODPart::FacetRemoved();
                                   FacetRemoved(): Guard against facets not in
                                   the collection of display facets.
                                   somUninit(): Fixed memory leak by deleting
                                   fFacets collection.
       <23>     9/29/94    JBS     1188214: more coord bias impl
       <22>     9/26/94    JBS     1186332, 1188981 - nonpersistence; 1183568
                                   - keep fIsDirty flag
       <21>     9/22/94    JBS     1188214: coordinate bias implementation
       <20>      9/7/94    CC      #1185056 - Use StdTypIO methods to
                                   internalize/externalize fLinkStatus field.
       <19>      9/6/94    CC      1153777- Implement GetLinkStatus(),
                                   ChangeLinkStatus(), and ContentUpdated(),
                                   and maintain the persistent fLinkStatus
                                   field.
       <18>     8/31/94    T‚      #1183129, #1183116, #1183119, #1183111:
                                   Lots of ErrorCode cleanup.
       <17>     8/31/94    VL      1183174: Fixed CloneInto to avoid multiple
                                   cloning. Removed CloneTo which was
                                   commented out in <14>. Removed CMDraft.xh
                                   because we are not depending on it with the
                                   new cloning mechanism.
       <16>     8/30/94    T‚      #1183567 StdTypIO routine implementation.
                                   Added Get/SetProp for time_t, ODPoint,
                                   ODRect, Strong/WeakStorageUnitRefs.  Use
                                   Get/SetProp routines for
                                   Strong/WeakStorageUnitRefs.
       <15>     8/29/94    T‚      #1183567 StdTypIO routine implementation &
                                   fixup
       <14>     8/26/94    VL      1183174: Implemented CloneInto & removed
                                   CloneTo.
       <13>     8/19/94    T‚      #1180922 Need to Stop using obsolete types
                                   (kOD ID) and fixed a nasty bug where
                                   destPartId was not being initialized to 0
                                   in CloneTo which was causing a crash in
                                   Bento.
       <12>     8/18/94    jpa     Filled in imaging factories [1180387]
       <11>     8/16/94    T‚      #1180922  Remove more obsolete types from
                                   StdTypes.idl.  Localized kODPartInfo to
                                   this file.
       <10>     8/16/94    VL      1143605: Fixed parameters to CloneInto.
        <9>     8/15/94    JBS     1180387: added Imaging factory methods
        <8>     8/12/94    JBS     1179920: move fWindow from Facet to Frame
        <7>     8/11/94    VL      1180299: Use modified CreateView.
        <6>     8/10/94    JBS     1179919: coordinate system bias changes;
                                   1179920: Frame::AcquireWindow()
        <5>      8/5/94    JBS     #1179151 Part API cleanup
        <4>     7/15/94    T‚      #ifdef'd out externalization of viewtype &
                                   presentation for now to get the Externalize
                                   method to not crash.
        <3>      7/7/94    JBS     added ::ReleaseAll()
        <2>     6/30/94    jpa     Include Draft.xh, not CMDraft.h.
        <1>     6/30/94    JBS     first checked in
        <0>     6/29/94    SV      SOMverted
   In Progress:

*/

#if (defined  _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
#include <ODPagtun.h>
#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_

#define ODFrame_Class_Source

#ifndef _ALTPOINT_
#include <AltPoint.h>
#endif

#ifndef SOM_ODFrame_xih
#define VARIABLE_MACROS
#include <Frame.xih>
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef SOM_ODFrameFacetIterator_xh
#include <FrFaItr.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_ODStorageUnitCursor_xh
#include <SUCursor.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef SOM_ODCanvas_xh
#include <Canvas.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef ODWindowState_Class_Source
#include <WinStat.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODWindowIterator_xh
#include <WinIter.xh>
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _DOCUTILS_
#include <DocUtils.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _AEHSHTBL_
#include "AEHshTbl.h"
#endif
#endif //_PLATFORM_MACINTOSH_

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _BIAS_
#include "Bias.h"
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODFrame
#endif //_PLATFORM_MACINTOSH_

#ifdef _PLATFORM_WIN32_
#ifndef _OLEWRAP_H_
#include "olewrap.h"
#endif

#ifndef _OLEMGRW_H_
#include "olemgrw.h"
#endif

#ifndef SOM_OpenDocShell_xh
#include "odshell.xh"
#endif

#ifndef SOM_DocumentManager_xh
#include "docmgr.xh"
#endif
#endif


//==============================================================================
// Debugging
//==============================================================================
#if (defined  _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
#include <qprintf.h>
#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_

#ifdef _FACET_DEBUG_
#include <qprintf.h>
#endif
//#undef   ODFrameMethodDebug
//#define ODFrameMethodDebug(c,m) PRINT(">>%s::%s\n",c,m)

//==============================================================================
// Constants
//==============================================================================

   // dirty mask
   const ODULong kNotDirty                 = 0x0000UL;
   const ODULong kContainingFrameDirty     = 0x0001UL;
   const ODULong kFrameShapeDirty          = 0x0002UL;
   const ODULong kInternalTransformDirty   = 0x0004UL;
   const ODULong kPartDirty                = 0x0008UL;
   const ODULong kPartInfoDirty            = 0x0010UL;
   const ODULong kViewTypeDirty            = 0x0020UL;
   const ODULong kPresentationDirty        = 0x0040UL;
   const ODULong kFrameGroupDirty          = 0x0080UL;
   const ODULong kSequenceNumberDirty      = 0x0100UL;
   const ODULong kLinkStatusDirty          = 0x0200UL;
   const ODULong kFlagsDirty               = 0x0400UL;
   const ODULong kAllDirty                 = 0xFFFFUL;

   // valid mask
   const ODUShort kFrameIsValid    = 0x0000U;
   const ODUShort kFrameIsRemoved  = 0x0001U;
   const ODUShort kFrameIsClosed   = 0x0002U;
   const ODUShort kFrameIsInRelease    = 0x0004U;

   const ODULong kODStorageUnitRefLen = 4UL;

//==============================================================================
// Macros
//==============================================================================

#define CHECKVALID(RET) if ( _fValidState == kFrameIsValid ) ; else \
                           { ODSetSOMException(ev, kODErrInvalidFrame); return RET;}
#if (defined _PLATFORM_WIN32_ || defined _PLATFORM_OS2_)
#undef VOID                      // VOID defined in WINNT.H and OS2DEFS.H, CHECKVALID( ) needs for "void" function return
#endif //_PLATFORM_OS2_
#define VOID /**/



//==============================================================================
// ODFrame
//==============================================================================

//------------------------------------------------------------------------------
// ODFrame: GetID
//------------------------------------------------------------------------------

SOM_Scope ODID  SOMLINK ODFrameGetID(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetID");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetID()\n");
#endif
   ODID id = 0;

   if ( _fNPID != 0 )
       id = _fNPID;
   else
   {
       SOM_TRY
           id = parent_GetID(somSelf, ev);
       SOM_CATCH_ALL
           id = 0;
       SOM_ENDTRY
   }
   return id;
}

//------------------------------------------------------------------------------
// ODFrame: AcquireContainingFrame
//------------------------------------------------------------------------------

SOM_Scope ODFrame*  SOMLINK ODFrameAcquireContainingFrame(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","AcquireContainingFrame");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::AcquireContainingFrame()\n");
#endif
   CHECKVALID(kODNULL);

   ODFrame* contFrame = kODNULL;
   SOM_TRY
       ODStorageUnit* su = somSelf->GetStorageUnit(ev);
       ODID frameID = 0;

       // if there is no containing frame, and this is not a root frame,
       // and this is not a non-persistent frame, we should
       // try to get the containing frame from the storage unit
       if ( !_fContainingFrame && !_fIsRoot && su )
       {
           frameID = ODGetWeakSURefProp(ev, su, kODPropContainingFrame, kODWeakStorageUnitRef);
           if ( frameID )
               _fContainingFrame = _fDraft->AcquireFrame(ev, frameID);
       }
       contFrame = _fContainingFrame;
       // bump ref count for caller
       ODAcquireObject(ev, _fContainingFrame);
   SOM_CATCH_ALL
       contFrame = kODNULL;
   SOM_ENDTRY
   return contFrame;
}

SOM_Scope void  SOMLINK ODFrameSetContainingFrame(ODFrame *somSelf, Environment *ev,
       ODFrame* frame)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetContainingFrame");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetContainingFrame()\n");
#endif
   CHECKVALID( VOID );

   ODSession* session = kODNULL;
   ODWindowState* winState = kODNULL;
   ODWindowIterator* windows = kODNULL;    ODVolatile(windows);
   ODWindow* win = kODNULL;

   SOM_TRY
       // when setting contFrame to NULL, make sure it has no facets
       if ( (frame == kODNULL) && _fFacets && (_fFacets->Count() != 0) )
           THROW(kODErrFrameHasFacets);

       ODAcquireObject(ev, frame);
       ODReleaseObject(ev, _fContainingFrame);
       _fContainingFrame = frame;
       _fIsRoot = (frame == kODNULL);

       // close any part windows with sourceFrame of this frame or an embedded frame.
       if ( frame == kODNULL )
       {
           TempODPart tempPart = somSelf->AcquirePart(ev);
           session = tempPart->GetStorageUnit(ev)->GetSession(ev);
           winState = session->GetWindowState(ev);
           windows = winState->CreateWindowIterator(ev);
           for ( win = windows->First(ev);
                   windows->IsNotComplete(ev);
                   win = windows->Next(ev) )
           {
               TempODFrame testFrame = win->AcquireSourceFrame(ev);
               while ( testFrame != kODNULL )
               {
                   if ( testFrame == somSelf )
                   {
                       win->Acquire(ev);
                       win->CloseAndRemove(ev);
                       ODReleaseObject(ev, testFrame); // sets to kODNULL
                   }
                   else
                   {
                       ODFrame* spam = testFrame;
                       TempODFrame oldFrame = spam;
                       testFrame = kODNULL; // to avoid double Release if following fails
                       testFrame = oldFrame->AcquireContainingFrame(ev);
                   }
               }
           }
           ODDeleteObject(windows);
       }

       somSelf->SetDirty(ev, kContainingFrameDirty);
   SOM_CATCH_ALL
       ODDeleteObject(windows);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: AcquireWindow
//------------------------------------------------------------------------------

SOM_Scope ODWindow*  SOMLINK ODFrameAcquireWindow(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","AcquireWindow");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::AcquireWindow()\n");
#endif
   CHECKVALID(kODNULL);

   ODWindow* window = kODNULL; ODVolatile(window);

   SOM_TRY
       if ( _fWindow )
       {
           window = _fWindow;
           ODAcquireObject(ev, _fWindow);
       }
       else if ( !_fIsRoot )
       {
           TempODFrame containingFrame = somSelf->AcquireContainingFrame(ev);
           window = containingFrame->AcquireWindow(ev); // bumps ref count for caller
       }
   SOM_CATCH_ALL
       window = kODNULL;
   SOM_ENDTRY
   return window;
}

SOM_Scope void  SOMLINK ODFrameSetWindow(ODFrame *somSelf, Environment *ev,
       ODWindow* window)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetWindow");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetWindow()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( !_fIsRoot )
           THROW(kODErrNotRootFrame);

       ODAcquireObject(ev, window);
       ODReleaseObject(ev, _fWindow);
       _fWindow = window;
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: GetPartInfo
//------------------------------------------------------------------------------

SOM_Scope ODInfoType  SOMLINK ODFrameGetPartInfo(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetPartInfo");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetPartInfo()\n");
#endif
   CHECKVALID(kODNULL);

   if ( _fPart == kODNULL )    // part has not been internalized yet
   {
       SOM_TRY
           somSelf->AcquirePart(ev)->Release(ev);  // force internalization of partInfo
       SOM_CATCH_ALL
       SOM_ENDTRY
   }

   return _fPartInfo;
}

SOM_Scope void  SOMLINK ODFrameSetPartInfo(ODFrame *somSelf, Environment *ev,
       ODInfoType partInfo)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetPartInfo");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetPartInfo()\n");
#endif
   CHECKVALID( VOID );

   _fPartInfo = partInfo;
//#ifdef OD_BUG  //Note sent to Apple on 3/20/96 by TMW
//    SOM_TRY
//      somSelf->SetDirty(ev, kPartInfoDirty); // [112513] make frame dirty
//    SOM_CATCH_ALL
//    SOM_ENDTRY
//#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_
}

//------------------------------------------------------------------------------
// ODFrame: GetViewType
//------------------------------------------------------------------------------

SOM_Scope ODTypeToken  SOMLINK ODFrameGetViewType(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetViewType");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetViewType()\n");
#endif
   CHECKVALID(kODNULL);

   return _fViewType;
}

SOM_Scope void  SOMLINK ODFrameSetViewType(ODFrame *somSelf, Environment *ev,
       ODTypeToken viewType)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetViewType");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetViewType()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( kODNULL == viewType )
           THROW(kODErrIllegalNullTokenInput);

       if ( _fViewType != viewType )
       {
           _fViewType = viewType;

           somSelf->SetDirty(ev, kViewTypeDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODFrameChangeViewType(ODFrame *somSelf, Environment *ev,
       ODTypeToken viewType)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangeViewType");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangeViewType()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( kODNULL == viewType )
           THROW(kODErrIllegalNullTokenInput);

       if ( viewType != _fViewType )
       {
           _fViewType = viewType;

           TempODPart tempPart = somSelf->AcquirePart(ev);
           tempPart->ViewTypeChanged(ev, somSelf);

           somSelf->SetDirty(ev, kViewTypeDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: GetPresentation
//------------------------------------------------------------------------------

SOM_Scope ODTypeToken  SOMLINK ODFrameGetPresentation(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetPresentation");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetPresentation()\n");
#endif
   CHECKVALID(kODNULL);

   return _fPresentation;
}

SOM_Scope void  SOMLINK ODFrameSetPresentation(ODFrame *somSelf, Environment *ev,
       ODTypeToken presentation)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetPresentation");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetPresentation()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( kODNULL == presentation )
           THROW(kODErrIllegalNullTokenInput);

       if ( _fPresentation != presentation )
       {
           _fPresentation = presentation;

           somSelf->SetDirty(ev, kPresentationDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODFrameChangePresentation(ODFrame *somSelf, Environment *ev,
       ODTypeToken presentation)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangePresentation");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangePresentation()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( kODNULL == presentation )
           THROW(kODErrIllegalNullTokenInput);

       if ( _fPresentation != presentation )
       {
           _fPresentation = presentation;

           TempODPart tempPart = somSelf->AcquirePart(ev);
           tempPart->PresentationChanged(ev, somSelf);

           somSelf->SetDirty(ev, kPresentationDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: GetFrameGroup
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODFrameGetFrameGroup(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetFrameGroup");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetFrameGroup()\n");
#endif
   CHECKVALID(0);

   return _fFrameGroup;
}

SOM_Scope void  SOMLINK ODFrameSetFrameGroup(ODFrame *somSelf, Environment *ev,
       ODULong groupID)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetFrameGroup");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetFrameGroup()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( groupID != _fFrameGroup )
       {
           _fFrameGroup = groupID;

           somSelf->SetDirty(ev, kFrameGroupDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: GetSequenceNumber
//------------------------------------------------------------------------------

SOM_Scope ODULong  SOMLINK ODFrameGetSequenceNumber(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetSequenceNumber");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetSequenceNumber()\n");
#endif
   CHECKVALID(0);

   return _fSequenceNumber;
}

SOM_Scope void  SOMLINK ODFrameChangeSequenceNumber(ODFrame *somSelf, Environment *ev,
       ODULong sequenceNumber)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangeSequenceNumber");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangeSequenceNumber()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( sequenceNumber != _fSequenceNumber )
       {
           _fSequenceNumber = sequenceNumber;

           somSelf->SetDirty(ev, kSequenceNumberDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: IsRoot
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameIsRoot(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","IsRoot");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::IsRoot()\n");
#endif
   CHECKVALID(kODFalse);

   return _fIsRoot;
}

//------------------------------------------------------------------------------
// ODFrame: IsSubframe
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameIsSubframe(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","IsSubframe");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::IsSubframe()\n");
#endif
   CHECKVALID(kODFalse);

   return _fIsSubframe;
}

SOM_Scope void  SOMLINK ODFrameSetSubframe(ODFrame *somSelf, Environment *ev,
       ODBoolean isSubframe)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetSubframe");
#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetSubFrame()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( isSubframe != _fIsSubframe)
       {
           _fIsSubframe = isSubframe;

           somSelf->SetDirty(ev, kFlagsDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: IsOverlaid
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameIsOverlaid(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","IsOverlaid");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::IsOverlaid()\n");
#endif
   CHECKVALID(kODFalse);

   return _fIsOverlaid;
}

//------------------------------------------------------------------------------
// ODFrame: IsFrozen
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameIsFrozen(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","IsFrozen");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::IsFrozen()\n");
#endif
   CHECKVALID(kODFalse);

   return _fIsFrozen;
}

SOM_Scope void  SOMLINK ODFrameSetFrozen(ODFrame *somSelf, Environment *ev,
       ODBoolean isFrozen)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetFrozen");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetFrozen()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( isFrozen != _fIsFrozen)
       {
           _fIsFrozen = isFrozen;

           somSelf->SetDirty(ev, kFlagsDirty);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: DoesPropagateEvents
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameDoesPropagateEvents(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","DoesPropagateEvents");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::DoesPropagateEvents()\n");
#endif
   CHECKVALID(kODFalse);

   return _fDoesPropagateEvents;
}

SOM_Scope void  SOMLINK ODFrameSetPropagateEvents(ODFrame *somSelf, Environment *ev,
       ODBoolean doesPropagateEvents)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetPropagateEvents");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetPropagateEvents()\n");
#endif
   CHECKVALID( VOID );

   if ( doesPropagateEvents != _fDoesPropagateEvents)
       _fDoesPropagateEvents = doesPropagateEvents;
}

//------------------------------------------------------------------------------
// ODFrame: IsInLimbo
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameIsInLimbo(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","IsInLimbo");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::IsInLimbo()\n");
#endif
   CHECKVALID(kODFalse);

   return _fIsInLimbo;
}

SOM_Scope void  SOMLINK ODFrameSetInLimbo(ODFrame *somSelf, Environment *ev,
       ODBoolean isInLimbo)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetInLimbo");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetInLimbo()\n");
#endif
   CHECKVALID( VOID );

   if ( isInLimbo != _fIsInLimbo)
       _fIsInLimbo = isInLimbo;
}

//------------------------------------------------------------------------------
// ODFrame: AcquirePart
//------------------------------------------------------------------------------

SOM_Scope ODPart*  SOMLINK ODFrameAcquirePart(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","AcquirePart");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::AcquirePart()\n");
#endif
   CHECKVALID(kODNULL);

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);

   SOM_TRY
       if ( _fPart == kODNULL )    // part has not been internalized yet
       {
           ODStorageUnit     *su = somSelf->GetStorageUnit(ev);
           ODStorageUnitView *suView = kODNULL;

           if ( su )
           {
               _fPart = _fDraft->AcquirePart(ev,
                       ODGetStrongSURefProp(ev, su, kODPropPart, kODStrongStorageUnitRef));
               ASSERT(_fPart, kODErrAssertionFailed);

               su->Focus(ev, kODPropPartInfo, kODPosUndefined, kODNULL, 0, kODPosUndefined);
               {
                   TempODStorageUnitView suView = su->CreateView(ev);
                   _fPartInfo = (ODInfoType)(_fPart->ReadPartInfo(ev, somSelf, suView));
               }

               _fPart->DisplayFrameConnected(ev, somSelf);

               i = somSelf->CreateFacetIterator(ev);
               for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
                   _fPart->FacetAdded(ev, facet);
               ODDeleteObject(i);
           }

#ifdef _PLATFORM_WIN32_
           DocumentManager  *docMgr = NULL;
           OleWrapper       *wrapper = NULL;

           docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);
           if (docMgr) {
               wrapper = docMgr->GetOleWrapper(ev);
               if (wrapper)
                   wrapper->OnDataChange();
           }
#endif

       }
       // bump ref count for caller
       ODAcquireObject(ev, _fPart);
   SOM_CATCH_ALL
       ODSafeReleaseObject(_fPart); _fPart = kODNULL;
       ODDeleteObject(i);
   SOM_ENDTRY

   return _fPart;
}

//------------------------------------------------------------------------------
// ODFrame: ChangePart
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameChangePart(ODFrame *somSelf, Environment *ev,
       ODPart* part)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangePart");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangePart()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( !part )
           THROW(kODErrIllegalNullPartInput);
       if ( _fPart == kODNULL )
           somSelf->AcquirePart(ev)->Release(ev);  // force internalization of part
       ASSERT( _fPart, kODErrInvalidPersistentFormat);

       ODFrameFacetIterator* i = somSelf->CreateFacetIterator(ev);
       ODFacet* facet = kODNULL;

       for (facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
           _fPart->FacetRemoved(ev, facet);
       TRY
           _fPart->DisplayFrameRemoved(ev, somSelf);
       CATCH_ALL
           WARNMSG(WARN_INDEX(AMSG_870),"Part returned err %d removing frame",ErrorCode());
           // don't reraise
       ENDTRY

       ODAcquireObject(ev, part);
       ODReleaseObject(ev, _fPart);
       _fPart = part;

       _fPart->DisplayFrameAdded(ev, somSelf);
       for (facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
           _fPart->FacetAdded(ev, facet);

       delete i;

       somSelf->SetDirty(ev, kPartDirty);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: FacetAdded
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameFacetAdded(ODFrame *somSelf, Environment *ev,
       ODFacet* facet)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","FacetAdded");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::FacetAdded()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
#if (defined  _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)  // dcp # 120163 $$$$$$$$$$$
      if(facet == kODNULL)
         THROW(kODErrIllegalNullInput);


#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_
       TempODPart tempPart = somSelf->AcquirePart(ev);
       _fFacets->AddLast(facet);
       tempPart->FacetAdded(ev, facet);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: FacetRemoved
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameFacetRemoved(ODFrame *somSelf, Environment *ev,
       ODFacet* facet)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","FacetRemoved");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::FacetRemoved()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       if ( _fFacets->Contains(facet) )
       {
           _fFacets->Remove(facet);
           TempODPart tempPart = somSelf->AcquirePart(ev);
           tempPart->FacetRemoved(ev, facet);
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: CreateFacetIterator
//------------------------------------------------------------------------------

SOM_Scope ODFrameFacetIterator*  SOMLINK ODFrameCreateFacetIterator(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","CreateFacetIterator");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::CreateFacetIterator()\n");
#endif
   ODFrameFacetIterator* iter = kODNULL; ODVolatile(iter);

   CHECKVALID(kODNULL);

   SOM_TRY
       iter = new ODFrameFacetIterator;
       THROW_IF_NULL(iter);
       iter->InitFrameFacetIterator(ev, somSelf);
   SOM_CATCH_ALL
       ODDeleteObject(iter);
       iter = kODNULL;
   SOM_ENDTRY
   return iter;
}

//------------------------------------------------------------------------------
// ODFrame: CreateShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFrameCreateShape(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","CreateShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::CreateShape()\n");
#endif
   CHECKVALID(kODNULL);

    ODShape *s = kODNULL;  ODVolatile(s);

   SOM_TRY
       s = new ODShape;
       THROW_IF_NULL(s);
       s->InitShape(ev);
   SOM_CATCH_ALL
       ODDeleteObject(s);
       s = kODNULL;
   SOM_ENDTRY
    return s;
}

//------------------------------------------------------------------------------
// ODFrame: AcquireFrameShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFrameAcquireFrameShape(ODFrame *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","AcquireFrameShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::AcquireFrameShape()\n");
#endif
   CHECKVALID(kODNULL);

   ODShape* s;
   SOM_TRY
       s = BiasShapeGet(ev, _fFrameShape, biasCanvas);
   SOM_CATCH_ALL
       s = kODNULL;
   SOM_ENDTRY
   return s;
}

//------------------------------------------------------------------------------
// ODFrame: ChangeFrameShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameChangeFrameShape(ODFrame *somSelf, Environment *ev,
       ODShape* shape, ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangeFrameShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangeFrameShape()\n");
#endif
   CHECKVALID( VOID );

   ODSession* session;
   ODTypeToken selectionFocus;
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
   TempODShape tShape=kODNULL;
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_


   SOM_TRY
       if (shape)
       {
           TempODPart tempPart = somSelf->AcquirePart(ev);
           session = tempPart->GetStorageUnit(ev)->GetSession(ev);
           selectionFocus = session->Tokenize(ev, kODSelectionFocus);

           TempODFrame activeFrame = session->GetArbitrator(ev)->AcquireFocusOwner(ev, selectionFocus);
           if ( ODObjectsAreEqual(ev, somSelf, activeFrame) )
               somSelf->InvalidateActiveBorder(ev);

           ODAcquireObject(ev, shape);

#ifdef _PLATFORM_MACINTOSH_
            ODReleaseObject(ev, _fFrameShape);
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
           tShape = _fFrameShape;
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_

           _fFrameShape = BiasShapeSet(ev, shape, biasCanvas);

           tempPart->FrameShapeChanged(ev, somSelf);

#ifdef _PLATFORM_WIN32_
           DocumentManager  *docMgr = NULL;
           OleWrapper       *wrapper = NULL;

           docMgr = ODOLEManager::GetShell()->GetActiveDocumentManager(ev);
           if (docMgr) {
               wrapper = docMgr->GetOleWrapper(ev);
               if (wrapper)
                   wrapper->OnDataChange();
           }
#endif

       }
       else
           THROW(kODErrIllegalNullShapeInput); // frame's MUST have a frameShape

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
      if(tShape && (!tShape->IsSameAs(ev, shape)))
         somSelf->SetDirty(ev, kFrameShapeDirty);
#endif // _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_

#ifdef _PLATFORM_MACINTOSH_
       somSelf->SetDirty(ev, kFrameShapeDirty);
#endif
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: RequestFrameShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFrameRequestFrameShape(ODFrame *somSelf, Environment *ev,
       ODShape* shape, ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","RequestFrameShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::RequestFrameShape()\n");
#endif
   CHECKVALID(kODNULL);

   ODShape* s = kODNULL;
   SOM_TRY
       TempODFrame containingFrame = somSelf->AcquireContainingFrame(ev);
       ODSession* session;
       ODTypeToken selectionFocus;

       if ( shape == kODNULL )
       {
           THROW(kODErrIllegalNullShapeInput);
           return kODNULL;
       }

       if ( containingFrame )  // root frames can't negotiate
       {
           TempODPart containingPart = containingFrame->AcquirePart(ev);
           BiasShapeSet(ev, shape, biasCanvas);
           if (containingPart)
               shape = containingPart->RequestFrameShape(ev, somSelf, shape);

           TempODPart tempPart = somSelf->AcquirePart(ev);
           session = tempPart->GetStorageUnit(ev)->GetSession(ev);
           selectionFocus = session->Tokenize(ev, kODSelectionFocus);

           TempODFrame activeFrame = session->GetArbitrator(ev)->AcquireFocusOwner(ev, selectionFocus);
           if ( ODObjectsAreEqual(ev, somSelf, activeFrame) )
               somSelf->InvalidateActiveBorder(ev);

           // shape's refcount is already inflated by containing part
           ODReleaseObject(ev, _fFrameShape);
           _fFrameShape = shape;
       }

       somSelf->SetDirty(ev, kFrameShapeDirty);

       s = BiasShapeGet(ev, _fFrameShape, biasCanvas);
   SOM_CATCH_ALL
       s = kODNULL;
   SOM_ENDTRY
   return s;
}

//------------------------------------------------------------------------------
// ODFrame: AcquireUsedShape
//------------------------------------------------------------------------------

SOM_Scope ODShape*  SOMLINK ODFrameAcquireUsedShape(ODFrame *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","AcquireUsedShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::AcquireUsedShape()\n");
#endif
   CHECKVALID(kODNULL);

   ODShape* s;
   SOM_TRY
       if ( _fUsedShape )
           s = BiasShapeGet(ev, _fUsedShape, biasCanvas);
       else
           s = ODCopyAndRelease(ev, BiasShapeGet(ev, _fFrameShape, biasCanvas));
   SOM_CATCH_ALL
       s = kODNULL;
   SOM_ENDTRY
   return s;
}

//------------------------------------------------------------------------------
// ODFrame: ChangeUsedShape
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameChangeUsedShape(ODFrame *somSelf, Environment *ev,
       ODShape* shape, ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangeUsedShape");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangeUsedShape()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       TempODFrame containingFrame = somSelf->AcquireContainingFrame(ev);
       TempODPart containingPart = kODNULL;

       ODAcquireObject(ev, shape);
       ODReleaseObject(ev, _fUsedShape);
       _fUsedShape = BiasShapeSet(ev, shape, biasCanvas);

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_ || defined _PLATFORM_UNIX_)
       if(shape)
       {
         ODFrameFacetIterator* i = somSelf->CreateFacetIterator(ev);
         ODFacet* facet = kODNULL;

#ifdef _PLATFORM_UNIX_
         for (facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
              facet->SetFacetWidgetRegion(ev, kODTrue);
#endif

#ifdef _EVERYTHING_IS_A_WINDOW_
         for (facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
             facet->SetFacetWindowRgn(ev, kODTrue);
#endif

       ODDeleteObject(i);

       }
#endif // OS2 or WIN32 or UNIX

       if ( containingFrame )
       {   containingPart = containingFrame->AcquirePart(ev);
           if ( containingPart )
               containingPart->UsedShapeChanged(ev, somSelf);
       }


   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: CreateTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFrameCreateTransform(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","CreateTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::CreateTransform()\n");
#endif
   CHECKVALID(kODNULL);

    ODTransform *t = kODNULL;  ODVolatile(t);
   SOM_TRY
       t = new ODTransform;
       THROW_IF_NULL(t);
       t->InitTransform(ev);
   SOM_CATCH_ALL
       ODDeleteObject(t);
       t = kODNULL;
   SOM_ENDTRY
    return t;
}

//------------------------------------------------------------------------------
// ODFrame: AcquireInternalTransform
//------------------------------------------------------------------------------

SOM_Scope ODTransform*  SOMLINK ODFrameAcquireInternalTransform(ODFrame *somSelf, Environment *ev,
       ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","AcquireInternalTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::AcquireInternalTransform()\n");
#endif
   CHECKVALID(kODNULL);

    ODTransform *t;
   SOM_TRY
       t = BiasTransformGet(ev, _fInternalTransform, biasCanvas);
   SOM_CATCH_ALL
       t = kODNULL;
   SOM_ENDTRY
    return t;
}

//------------------------------------------------------------------------------
// ODFrame: ChangeInternalTransform
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameChangeInternalTransform(ODFrame *somSelf, Environment *ev,
       ODTransform* transform, ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangeInternalTransform");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangeInternalTransform()\n");
#endif
   CHECKVALID( VOID );

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);
   SOM_TRY
       if ( transform )
       {
           ODAcquireObject(ev, transform);
           ODReleaseObject(ev, _fInternalTransform);
           _fInternalTransform = BiasTransformSet(ev, transform, biasCanvas);

           i = somSelf->CreateFacetIterator(ev);
           for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
               facet->InternalTransformChanged(ev);
           delete i;
       }
       else
       {
           THROW(kODErrIllegalNullTransformInput);
       }

       somSelf->SetDirty(ev, kInternalTransformDirty);
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: GetContentExtent
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameGetContentExtent(ODFrame *somSelf, Environment *ev, ODPoint* contentExtent)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetContentExtent");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetContentExtent()\n");
#endif
   ODPoint pt(0,0);
   *contentExtent = pt;

   CHECKVALID(VOID);

   *contentExtent = _fContentExtent;
}

//------------------------------------------------------------------------------
// ODFrame: ChangeContentExtent
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameChangeContentExtent(ODFrame *somSelf, Environment *ev,
       ODPoint* contentExtent)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangeContentExtent");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangeContentExtent()\n");
#endif
   CHECKVALID( VOID );

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);
   SOM_TRY
       _fContentExtent = *contentExtent;

       // compute biasTransform of each facet's canvas
       i = somSelf->CreateFacetIterator(ev);
       for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
       {
           if ( facet->HasCanvas(ev) )
               facet->GetCanvas(ev)->ComputeBiasTransform(ev);
       }
       delete i;
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: IsDroppable
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameIsDroppable(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","IsDroppable");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::IsDroppable()\n");
#endif
   CHECKVALID(kODFalse);

   return _fIsDroppable;
}

SOM_Scope void  SOMLINK ODFrameSetDroppable(ODFrame *somSelf, Environment *ev,
       ODBoolean isDroppable)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetDroppable");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetDroppable()\n");
#endif
   CHECKVALID( VOID );

   _fIsDroppable = isDroppable;
}

//------------------------------------------------------------------------------
// ODFrame: IsDragging
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameIsDragging(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","IsDragging");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::IsDragging()\n");
#endif
   CHECKVALID(kODFalse);

   return _fIsDragging;
}

//------------------------------------------------------------------------------
// ODFrame: SetDragging
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameSetDragging(ODFrame *somSelf, Environment *ev,
       ODBoolean isDragging)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetDragging");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetDragging()\n");
#endif
   CHECKVALID( VOID );

   _fIsDragging = isDragging;
}

//------------------------------------------------------------------------------
// ODFrame: ContentUpdated
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameContentUpdated(ODFrame *somSelf, Environment *ev,
       ODUpdateID change)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ContentUpdated");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ContentUpdated()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       TempODFrame embeddedFrame = somSelf;
       TempODFrame containingFrame = somSelf->AcquireContainingFrame(ev);

       embeddedFrame->Acquire(ev); // will be released in loop below
       while ( containingFrame != kODNULL )
       {
           TempODPart containingPart = containingFrame->AcquirePart(ev);
           containingPart->EmbeddedFrameUpdated(ev, embeddedFrame, change);
           ODReleaseObject(ev, embeddedFrame);
           embeddedFrame = (ODFrame*) containingFrame; // do not bitwise copy!
           containingFrame = kODNULL; // avoid double Release in case following fails
           containingFrame = embeddedFrame->AcquireContainingFrame(ev);
       }
       ODReleaseObject(ev, embeddedFrame);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: ChangeLinkStatus
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameChangeLinkStatus(ODFrame *somSelf, Environment *ev,
       ODLinkStatus status)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ChangeLinkStatus");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ChangeLinkStatus()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       ODLinkStatus newStatus = _fLinkStatus;

       TempODFrame containingFrame = somSelf->AcquireContainingFrame(ev);
       ODLinkStatus cfStatus = containingFrame->GetLinkStatus(ev);

       if ( (status != kODNotInLink)
               && (status != kODInLinkSource)
               && (status != kODInLinkDestination) )
           THROW(kODErrInvalidLinkStatus);

       // the logic below will automatically fix the link status of an embedded
       // frame to be at least as restrictive as the status of its containing frame
       if ( cfStatus == kODNotInLink )
       {
           newStatus = status;
       }
       else if ( cfStatus == kODInLinkSource )
       {
           if ( status == kODInLinkDestination )
               newStatus = kODInLinkDestination;
           else
               newStatus = kODInLinkSource;
       }
       else if ( cfStatus == kODInLinkDestination )
       {
           newStatus = kODInLinkDestination;
       }
       else
           THROW(kODErrInvalidLinkStatus);

       if ( _fLinkStatus != newStatus )
       {
           _fLinkStatus = newStatus;
           TempODPart tempPart = somSelf->AcquirePart(ev);
           tempPart->LinkStatusChanged(ev, somSelf);
       }

       // don't mark draft dirty, just mark property dirty
       _fDirty |= kLinkStatusDirty;
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: GetLinkStatus
//------------------------------------------------------------------------------

SOM_Scope ODLinkStatus  SOMLINK ODFrameGetLinkStatus(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetLinkStatus");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetLinkStatus()\n");
#endif
   CHECKVALID(kODFalse);

   return _fLinkStatus;
}

//------------------------------------------------------------------------------
// ODFrame: EditInLink
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODFrameEditInLink(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","EditInLink");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::EditInLink()\n");
#endif
   CHECKVALID(kODFalse);

   ODBoolean success = kODFalse;   ODVolatile(success);

   SOM_TRY
       if ( _fLinkStatus == kODInLinkDestination )
       {
           TempODFrame lastFrame = somSelf;
           lastFrame->Acquire(ev); // because it will be Released below
           while ( kODTrue )
           {
               TempODFrame nextFrame = lastFrame->AcquireContainingFrame(ev);
               if ( nextFrame == kODNULL )
               {
                   TempODWindow tempWindow = lastFrame->AcquireWindow(ev);
                   nextFrame = tempWindow->AcquireSourceFrame(ev);
               }
               if ( nextFrame == kODNULL )
                   break;

               // the test below should never be true if nextFrame is lastFrame's
               // sourceFrame, therefore below nextFrame will always be the
               // containing frame of lastFrame
               if ( nextFrame->GetLinkStatus(ev) != kODInLinkDestination )
               {
                   TempODPart tempPart = nextFrame->AcquirePart(ev);
                   success = tempPart->EditInLinkAttempted(ev, lastFrame);
                   break;
               }
               else
               {
                   ODReleaseObject(ev, lastFrame);
                   lastFrame = (ODFrame*) nextFrame; // do not bitwise copy!
                   nextFrame = kODNULL; // to prevent Release on loop
               }
           }
       }
   SOM_CATCH_ALL
       success = kODFalse;
   SOM_ENDTRY
   return success;
}

//------------------------------------------------------------------------------
// ODFrame: Invalidate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameInvalidate(ODFrame *somSelf, Environment *ev,
       ODShape* invalidShape, ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","Invalidate");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::Invalidate()\n");
#endif
   CHECKVALID( VOID );

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);
   SOM_TRY
#if (defined  _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)  // $$$ dcp 05/10/95 # 120214
      if(invalidShape)
      {
         if(!invalidShape->GetRealShape(ev))
            THROW(kODErrInvalidParameter);
      }
#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_
      i = somSelf->CreateFacetIterator(ev);
      for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
          facet->Invalidate(ev, invalidShape, biasCanvas);
      delete i;
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: Validate
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameValidate(ODFrame *somSelf, Environment *ev,
       ODShape* validShape, ODCanvas* biasCanvas)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","Validate");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::Validate()\n");
#endif
   CHECKVALID( VOID );

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);
   SOM_TRY
#if (defined  _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)  // $$$ dcp 05/10/95 # 120214
      if(validShape)
      {
         if(!validShape->GetRealShape(ev))
            THROW(kODErrInvalidParameter);
      }
#endif //_PLATFORM_OS2_ || _PLATFORM_WIN32_
      i = somSelf->CreateFacetIterator(ev);
      for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
          facet->Validate(ev, validShape, biasCanvas);
      delete i;
   SOM_CATCH_ALL
      ODDeleteObject(i);
   SOM_ENDTRY
}


//------------------------------------------------------------------------------
// ODFrame: InvalidateActiveBorder
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODFrameInvalidateActiveBorder(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","InvalidateActiveBorder");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::InvalidateActiveBorder()\n");
#endif
   CHECKVALID( VOID );

   if ( _fIsRoot )     // don't draw for root frame with subframes
       return;

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);
   SOM_TRY
       if ( _fIsSubframe )
       {
           // don't draw for subframes. instead, have containingFrame draw
           TempODFrame containingFrame = somSelf->AcquireContainingFrame(ev);
           containingFrame->InvalidateActiveBorder(ev);
       }
       else
       {
           // draw active border on all my facets
           i = somSelf->CreateFacetIterator(ev);
           for ( ODFacet* facet = i->First(ev);
                   i->IsNotComplete(ev);
                   facet = i->Next(ev) )
           {
               facet->InvalidateActiveBorder(ev);
           }
           ODDeleteObject(i);
       }
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: DrawActiveBorder
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameDrawActiveBorder(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","DrawActiveBorder");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::DrawActiveBorder()\n");
#endif
   CHECKVALID( VOID );

   if ( _fIsRoot )     // don't draw for root frame with subframes
       return;

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);
   SOM_TRY
       if ( _fIsSubframe )
       {
           // don't draw for subframes. instead, have containingFrame draw
           TempODFrame containingFrame = somSelf->AcquireContainingFrame(ev);
           containingFrame->DrawActiveBorder(ev);
       }
       else
       {
           // draw active border on all my facets
           i = somSelf->CreateFacetIterator(ev);
           for ( ODFacet* facet = i->First(ev);
                   i->IsNotComplete(ev);
                   facet = i->Next(ev) )
           {
               facet->DrawActiveBorder(ev);
           }
           delete i;
       }
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameRelease(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","Release");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::Release()\n");
#endif
   // *DON'T* CHECKVALID - need to be able to release invalid frames

   SOM_TRY
       if ( (somSelf->GetRefCount(ev) == 1) &&
           (_fValidState & kFrameIsRemoved) &&
           !(_fValidState & kFrameIsInRelease) &&
           somSelf->GetStorageUnit(ev) )
       {
           // remove persistent frames previously removed which are
           // finally unreferenced
           _fValidState |= kFrameIsInRelease;
           _fDraft->RemoveFrame(ev, somSelf);
       }
       else
       {
           parent_Release(somSelf, ev);
           if (somSelf->GetRefCount(ev) == 0)
           {
               // only release frames previously closed or non-persistent
               _fDraft->ReleaseFrame(ev, somSelf);
               _fValidState &= ~kFrameIsInRelease;
           }
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: Close
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameClose(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","Close");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::Close()\n");
#endif
   CHECKVALID( VOID );

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);

   SOM_TRY
       if ( _fPart ) // if fPart not already internalized, don't force it
       {
           i = somSelf->CreateFacetIterator(ev);
           for (ODFacet* facet = i->First(ev); i->IsNotComplete(ev); facet = i->Next(ev))
               _fPart->FacetRemoved(ev, facet);
           ODDeleteObject(i);

           TRY
               _fPart->DisplayFrameClosed(ev, somSelf);
           CATCH_ALL
               WARNMSG(WARN_INDEX(AMSG_880),"Part returned err %d closing frame",ErrorCode());
               // don't reraise
           ENDTRY
           ODReleaseObject(ev,_fPart);
       }
       ODReleaseObject(ev, _fContainingFrame);
       ODReleaseObject(ev, _fWindow);
       somSelf->Release(ev);
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: Remove
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameRemove(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","Remove");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::Remove()\n");
#endif
   CHECKVALID( VOID );

   ODFrameFacetIterator* i = kODNULL; ODVolatile(i);

   SOM_TRY
       if ( _fPart == kODNULL )
           somSelf->AcquirePart(ev)->Release(ev);  // force internalization of fPart

       if ( _fFacets->Count() != 0 )
           THROW(kODErrFrameHasFacets);

       TRY
           _fPart->DisplayFrameRemoved(ev, somSelf);
       CATCH_ALL
           WARNMSG(WARN_INDEX(AMSG_870),"Part returned err %d removing frame",ErrorCode());
           // don't reraise
       ENDTRY

       ODReleaseObject(ev, _fPart);
       ODReleaseObject(ev, _fContainingFrame);
       ODReleaseObject(ev, _fWindow);

       if (somSelf->GetRefCount(ev) == 1)
       {
           _fDraft->RemoveFrame(ev, somSelf);
       }
       else
       {
           somSelf->Release(ev);
           _fValidState = kFrameIsRemoved;
       }
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODFramePurge(ODFrame *somSelf, Environment *ev,
       ODSize numBytes)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","Purge");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::Purge()\n");
#endif
   // *DON'T* CHECKVALID - still need to purge invalid frames

   ODSize freed = 0;                   ODVolatile(freed);

//#ifndef _PLATFORM_OS2_ // aml   // TMW don't know why OS/2 commented out, so we will leave it in.
   ODFrameFacetIterator* i = kODNULL;  ODVolatile(i);

   SOM_TRY
       // purge geometric objects
       if ( _fFrameShape != kODNULL )
           freed += _fFrameShape->Purge(ev, numBytes-freed);
       if ( _fUsedShape != kODNULL )
           freed += _fUsedShape->Purge(ev, numBytes-freed);
       if ( _fInternalTransform != kODNULL )
           freed += _fInternalTransform->Purge(ev, numBytes-freed);

       if ( _fValidState == kFrameIsValid )
       {
           // purge facets
           i = new ODFrameFacetIterator;
           THROW_IF_NULL(i);
           i->InitFrameFacetIterator(ev, somSelf);
           for ( ODFacet* facet = i->First(ev);
                   i->IsNotComplete(ev);
                   facet = i->Next(ev) )
           {
               freed += facet->Purge(ev, numBytes-freed);
           }
           ODDeleteObject(i);
       }
   SOM_CATCH_ALL
       ODDeleteObject(i);
   SOM_ENDTRY
//#endif //_PLATFORM_OS2_         // TMW don't know why OS/2 commented out, so we will leave it in.
   return freed;
}

//------------------------------------------------------------------------------
// WriteShapeIfAny  [static]
//------------------------------------------------------------------------------

static void
WriteShapeIfAny( Environment *ev, ODStorageUnit *su, ODShape *shape, ODPropertyName property )
{
   if (shape) {
       su->Focus(ev, property, kODPosUndefined, kODNULL, (ODValueIndex)0, kODPosUndefined);
       shape->WriteShape(ev, su);
   } else if (su->Exists(ev, property, kODNULL, 1)) {
       su->Focus(ev, property, kODPosUndefined, kODNULL, (ODValueIndex)1, kODPosUndefined);
       su->DeleteValue(ev, su->GetSize(ev));
   }
}

//------------------------------------------------------------------------------
// ODFrame: Externalize
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameExternalize(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","Externalize");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::Externalize()\n");
#endif
   // *DON'T* CHECKVALID - need to write an "I'm invalid" representation

   SOM_TRY
       ODStorageUnit* su = kODNULL;
       ODSession* session = kODNULL;
       ODISOStr typeString = kODNULL;

       su = somSelf->GetStorageUnit(ev);
       if ( su == kODNULL) return;     // don't externalize non-persistent frames

       parent_Externalize(somSelf, ev);

       session = su->GetSession(ev);

       // if frame is invalid because of removal, delete Part property to
       // indicate persistently that frame is invalid.
       if ( _fDirty && _fValidState )
       {
           ODSURemoveProperty(ev, su, kODPropPart);
           ODSURemoveProperty(ev, su, kODPropContainingFrame);
           _fDirty = kNotDirty;
           return;
       }

       if ( _fDirty & kContainingFrameDirty )
       {
           // can't write suRef to non-persistent containingFrame
           if ( _fContainingFrame && _fContainingFrame->GetStorageUnit(ev) )
               ODSetWeakSURefProp(ev, su, kODPropContainingFrame, kODWeakStorageUnitRef,
                                   _fContainingFrame->GetStorageUnit(ev)->GetID(ev));
           else
           {
               if ( ODSUExistsThenFocus(ev, su, kODPropContainingFrame, kODWeakStorageUnitRef) )
                   su->Remove(ev);
               ODSUAddPropValue(ev, su, kODPropContainingFrame, kODWeakStorageUnitRef);
           }
       }

       // geometry

       // no bias transform needed, already in standard bias
       ODSURemoveProperty(ev, su, kODPropBiasTransform);

       if ( _fDirty & kFrameShapeDirty )
       {
           su->Focus(ev, kODPropFrameShape, kODPosUndefined, kODNULL, (ODValueIndex)0, kODPosUndefined);
           _fFrameShape->WriteShape(ev, su);
       }

       if ( _fDirty & kInternalTransformDirty )
       {
           su->Focus(ev, kODPropInternalTransform, kODPosUndefined, kODNULL, (ODValueIndex)0, kODPosUndefined);
           _fInternalTransform->WriteTo(ev, su);
       }

       // part & partInfo

       if ( _fDirty & kPartDirty )
       {
           ASSERT( _fPart, kODErrInvalidFrame);
           ODSetStrongSURefProp(ev, su, kODPropPart, kODStrongStorageUnitRef, _fPart->GetID(ev));
       }

       // always try to write partInfo, part knows if it's dirty
       if ( _fPart ) // only have to write partInfo if part has been internalized
       {
           su->Focus(ev, kODPropPartInfo, kODPosUndefined, kODNULL, (ODValueIndex)0, kODPosUndefined);
           TempODStorageUnitView suView = su->CreateView(ev);
           _fPart->WritePartInfo(ev, (ODInfoType)_fPartInfo, suView);
       }

       // viewType & presentation

       if ( _fDirty & kViewTypeDirty )
       {
           if ( (_fViewType != kODNULL) && session->GetType(ev, _fViewType, &typeString) )
           {
               ODSetISOStrProp(ev, su, kODPropViewType, kODISOStr, typeString);
               delete typeString; typeString = kODNULL;
           }
       }

       if ( _fDirty & kPresentationDirty )
       {
           if ( (_fPresentation != kODNULL) && session->GetType(ev, _fPresentation, &typeString) )
           {
               ODSetISOStrProp(ev, su, kODPropPresentation, kODISOStr, typeString);
               delete typeString; typeString = kODNULL;
           }
       }

       if ( _fDirty & kFrameGroupDirty )
           ODSetULongProp(ev, su, kODPropFrameGroup, kODULong, _fFrameGroup);
       if ( _fDirty & kSequenceNumberDirty )
           ODSetULongProp(ev, su, kODPropSequenceNumber, kODULong, _fSequenceNumber);
       if ( _fDirty & kLinkStatusDirty )
           ODSetULongProp(ev, su, kODPropLinkStatus, kODULong, _fLinkStatus);

       if ( _fDirty & kFlagsDirty )
       {
           ODSetBooleanProp(ev, su, kODPropIsSubframe, kODBoolean, _fIsSubframe);
           ODSetBooleanProp(ev, su, kODPropIsOverlaid, kODBoolean, _fIsOverlaid);
           ODSetBooleanProp(ev, su, kODPropIsFrozen, kODBoolean, _fIsFrozen);
       }

       _fDirty = kNotDirty;
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: CloneInto
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameCloneInto(ODFrame *somSelf, Environment *ev,
       ODDraftKey key,
       ODStorageUnit* toSU,
       ODFrame* scopeFrame)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","CloneInto");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::CloneInto()\n");
#endif
   // *DON'T* CHECKVALID - need to write an "I'm invalid" representation

   SOM_TRY
       if ( kODNULL == toSU ) THROW(kODErrIllegalNullInput);

       ODStorageUnitID     fromSUID = somSelf->GetID(ev);
       ODDraft*            fromDraft = _fDraft;
       ODISOStr            typeString = kODNULL;

       if (toSU->Exists(ev, kODPropFrameShape, kODNULL, 0) == kODFalse)
       {
           parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);

           // if frame is invalid because of removal, don't write Part
           // Property to indicate that frame is invalid.
           if ( _fValidState )
               return;

           if (_fContainingFrame)
           {
               ODID toContainingFrameID = fromDraft->WeakClone(ev, key, _fContainingFrame->GetID(ev), 0, 0);
               ODSetWeakSURefProp(ev, toSU, kODPropContainingFrame, kODWeakStorageUnitRef,
                                   toContainingFrameID);
           }

           toSU->AddProperty(ev, kODPropFrameShape);
           _fFrameShape->WriteShape(ev, toSU);

           ODSUAddPropValue(ev, toSU, kODPropInternalTransform, kODTransform);
           _fInternalTransform->WriteTo(ev, toSU);

           // part & partInfo

           if ( _fPart == kODNULL )
               somSelf->AcquirePart(ev)->Release(ev); // force internalization of fPart

           ODID toPartID = fromDraft->Clone(ev, key, _fPart->GetID(ev), 0, (scopeFrame ? scopeFrame->GetID(ev) : 0));
           ODSetStrongSURefProp(ev, toSU, kODPropPart, kODStrongStorageUnitRef, toPartID);

           toSU->AddProperty(ev, kODPropPartInfo);
           {
               TempODStorageUnitView suView = toSU->CreateView(ev);
               _fPart->ClonePartInfo(ev, key, (ODInfoType)_fPartInfo, suView, scopeFrame);
           }


           // Window Properties
           ODStorageUnit* fromSU = somSelf->GetStorageUnit(ev);
           if (fromSU && _fIsRoot)
           {
               ODID windowPropsID;
               if ((windowPropsID = ODGetStrongSURefProp(ev, fromSU, kODPropWindowProperties, kODStrongStorageUnitRef))
                   != 0)
               {
                   ODID toWindowPropsID = fromDraft->Clone(ev, key, windowPropsID, 0, (scopeFrame ? scopeFrame->GetID(ev) : 0));
                   ODSetStrongSURefProp(ev, toSU, kODPropWindowProperties, kODStrongStorageUnitRef, toWindowPropsID);
               }
           }
           // viewType & presentation

           ODSession* session = toSU->GetSession(ev);

           ASSERT(_fViewType, kODErrInvalidFrame);
           if ( (_fViewType != kODNULL) && session->GetType(ev, _fViewType, &typeString) )
           {
               ODSetISOStrProp(ev, toSU, kODPropViewType, kODISOStr, typeString);
               delete typeString; typeString = kODNULL;
           }

           ASSERT(_fPresentation, kODErrInvalidFrame);
           if ( (_fPresentation != kODNULL) && session->GetType(ev, _fPresentation, &typeString) )
           {
               ODSetISOStrProp(ev, toSU, kODPropPresentation, kODISOStr, typeString);
               delete typeString; typeString = kODNULL;
           }

           ODSetULongProp(ev, toSU, kODPropFrameGroup, kODULong, _fFrameGroup);
           ODSetULongProp(ev, toSU, kODPropSequenceNumber, kODULong, _fSequenceNumber);
           ODSetULongProp(ev, toSU, kODPropLinkStatus, kODULong, _fLinkStatus);
           ODSetBooleanProp(ev, toSU, kODPropIsSubframe, kODBoolean, _fIsSubframe);
           ODSetBooleanProp(ev, toSU, kODPropIsOverlaid, kODBoolean, _fIsOverlaid);
           ODSetBooleanProp(ev, toSU, kODPropIsFrozen, kODBoolean, _fIsFrozen);
       }
       else
       {
           if ( _fPart == kODNULL )
               somSelf->AcquirePart(ev)->Release(ev); // force internalization of fPart
           ODID toPartID = fromDraft->Clone(ev, key, _fPart->GetID(ev), 0, (scopeFrame ? scopeFrame->GetID(ev) : 0));
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: GetFacets
//------------------------------------------------------------------------------

SOM_Scope OrderedCollection*  SOMLINK ODFrameGetFacets(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","GetFacets");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::GetFacets()\n");
#endif
   CHECKVALID(kODNULL);

   return _fFacets;
}

//------------------------------------------------------------------------------
// ODFrame: PrepareToSwap
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFramePrepareToSwap(ODFrame *somSelf, Environment *ev,
       ODPart* part)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","PrepareToSwap");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::PrepareToSwap()\n");
#endif
   CHECKVALID( VOID );

   SOM_TRY
       ODBoolean swappingMyPart = kODFalse;

       if ( _fPart )
           swappingMyPart = ODObjectsAreEqual(ev, part, _fPart);
       else
       {
           ODStorageUnit* su = somSelf->GetStorageUnit(ev);
           if ( su )
           {
               ODID myPartID = ODGetStrongSURefProp(ev, su, kODPropPart,
                                                       kODStrongStorageUnitRef);
               swappingMyPart = ( part->GetID(ev) == myPartID );
           }
       }

       if ( swappingMyPart )
       {
           if ( _fIsRoot && _fWindow )
           {
               _fWindow->Acquire(ev);  // balanced by Release in Close below
               _fWindow->Close(ev);
               _fWindow = kODNULL;
           }
           else
           {
               somSelf->Acquire(ev);  // because Close calls Release on self
               somSelf->Close(ev);
           }
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}


//------------------------------------------------------------------------------
// ODFrame: SetDirty
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameSetDirty(ODFrame *somSelf, Environment *ev,
                                       ODULong dirtyProperty)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","SetDirty");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::SetDirty()\n");
#endif
   SOM_TRY
       if ( somSelf->GetStorageUnit(ev) ) // don't set non-persistent frames dirty
       {
           _fDirty |= dirtyProperty;
           if (HAS_WRITE_ACCESS(_fDraft->GetPermissions(ev)))
           {
               _fDraft->SetChangedFromPrev(ev);
           }
       }
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: CommonInitFrame
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameCommonInitFrame(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","CommonInitFrame");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::CommonInitFrame()\n");
#endif
   SOM_TRY
       _fFacets = new OrderedCollection;
       THROW_IF_NULL(_fFacets);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: InitFrame
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODFrameInitFrame(ODFrame *somSelf, Environment *ev,

               ODStorageUnit*  storageUnit,
               ODFrame*        containingFrame,
               ODShape*        frameShape,
               ODCanvas*       biasCanvas,
               ODPart*         part,
               ODTypeToken     viewType,
               ODTypeToken     presentation,
               ODBoolean       isSubframe,
               ODBoolean       isOverlaid)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","InitFrame");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::InitFrame()\n");
#endif
   /* Moved from somInit. SOM itself sets fields to zero
   _fDraft             = kODNULL;
   _fNPID              = 0;
   _fDirty             = kNotDirty;

   _fContainingFrame   = kODNULL;
   _fWindow            = kODNULL;

   _fFrameShape        = (ODShape*) kODNULL;
   _fUsedShape         = (ODShape*) kODNULL;

   _fInternalTransform = (ODTransform*) kODNULL;

   _fFacets            = kODNULL;

   _fPart              = kODNULL;
   _fPartInfo          = (ODInfoType) kODNULL;
   _fViewType          = (ODTypeToken) 0;
   _fPresentation      = (ODTypeToken) 0;

   _fFrameGroup        = 0;
   _fSequenceNumber    = 0;

   _fLinkStatus        = kODNotInLink;
   _fIsRoot            = kODFalse;
   _fIsSubframe        = kODFalse;
   _fIsOverlaid        = kODFalse;
   _fIsFrozen          = kODFalse;
   _fIsDroppable       = kODFalse;
   _fIsDragging        = kODFalse;
   _fDoesPropagateEvents = kODFalse;
   */
   _fDirty             = kNotDirty;
   _fLinkStatus        = kODNotInLink;
   _fValidState        = kFrameIsValid;

   // *DON'T* CHECKVALID - new frames will never be invalid

   SOM_TRY
       if ( kODNULL == storageUnit )   THROW(kODErrIllegalNullStorageUnitInput);
       if ( kODNULL == frameShape  )   THROW(kODErrIllegalNullShapeInput);
       if ( kODNULL == part )          THROW(kODErrIllegalNullPartInput);
       if ( kODNULL == viewType )      THROW(kODErrIllegalNullTokenInput);
       if ( kODNULL == presentation )  THROW(kODErrIllegalNullTokenInput);

       // check for recursive embedding
       if ( (containingFrame != kODNULL) && !isSubframe )
       {
           TempODFrame tempFrame = containingFrame;
           tempFrame->Acquire(ev);   // because it will be released below
           while ( tempFrame )
           {
               TempODPart tempPart = tempFrame->AcquirePart(ev);
               if (tempPart == part)
                   THROW(kODErrIllegalRecursiveEmbedding);
               ODFrame* spam = tempFrame;
               TempODFrame oldFrame = spam;
               tempFrame = kODNULL; // to avoid double Release if following fails
               tempFrame = oldFrame->AcquireContainingFrame(ev);
           }
       }

       somSelf->InitPersistentObject(ev, storageUnit);
       _fDraft = storageUnit->GetDraft(ev);

       _fContainingFrame = containingFrame;
       ODAcquireObject(ev, _fContainingFrame);

       _fFrameShape = BiasShapeSet(ev, frameShape, biasCanvas);
       ODAcquireObject(ev, _fFrameShape);

       _fInternalTransform = somSelf->CreateTransform(ev);
       _fContentExtent     = ODPoint(0,0);

       _fViewType      = viewType;
       _fPresentation  = presentation;
       _fIsRoot        = (containingFrame == kODNULL);
       _fIsSubframe    = isSubframe;
       _fIsOverlaid    = isOverlaid;

       somSelf->CommonInitFrame(ev);

       ODSUAddPropValue(ev, storageUnit, kODPropContainingFrame, kODWeakStorageUnitRef);
       ODSUAddPropValue(ev, storageUnit, kODPropFrameShape, kODPolygon);
       ODSUAddPropValue(ev, storageUnit, kODPropInternalTransform, kODTransform);
       ODSUAddPropValue(ev, storageUnit, kODPropPart, kODStrongStorageUnitRef);
   //  ODSUAddPropValue(ev, storageUnit, kODPropPartInfo, kODPartInfo);
       ODSUAddPropValue(ev, storageUnit, kODPropViewType, kODISOStr);
       ODSUAddPropValue(ev, storageUnit, kODPropPresentation, kODISOStr);
       ODSUAddPropValue(ev, storageUnit, kODPropFrameGroup, kODULong);
       ODSUAddPropValue(ev, storageUnit, kODPropSequenceNumber, kODULong);
       ODSUAddPropValue(ev, storageUnit, kODPropLinkStatus, kODULong);
       ODSUAddPropValue(ev, storageUnit, kODPropIsSubframe, kODBoolean);
       ODSUAddPropValue(ev, storageUnit, kODPropIsOverlaid, kODBoolean);
       ODSUAddPropValue(ev, storageUnit, kODPropIsFrozen, kODBoolean);

       _fPart = part;
       ODAcquireObject(ev, _fPart);
       _fPart->DisplayFrameAdded(ev, somSelf);

       somSelf->SetDirty(ev, kAllDirty);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ReadAndAcquireShapeIfAny  [static]
//------------------------------------------------------------------------------

static ODShape* // DMc refcount - include "acquire" in function name:
ReadAndAcquireShapeIfAny( Environment *ev, ODStorageUnit *su, ODPropertyName property )
{
   ODShape* shape = kODNULL;
   if( su->Exists(ev, property, kODNULL, 0) ) {
       shape= new ODShape;
       THROW_IF_NULL(shape);
       TRY{
           shape->InitShape(ev);
           su->Focus(ev, property, kODPosUndefined, kODNULL, 0, kODPosUndefined);
           shape->ReadShape(ev, su);
       }CATCH_ALL{
           delete shape;
           RERAISE;
       }ENDTRY
   }
   return shape;
}

//------------------------------------------------------------------------------
// ReadAndAcquireTransformIfAny  [static]
//------------------------------------------------------------------------------

static ODTransform* // DMc refcount - include "acquire" in function name:
ReadAndAcquireTransformIfAny( Environment *ev, ODStorageUnit *su, ODPropertyName property )
{
   ODTransform* transform = kODNULL;
   if( su->Exists(ev, property, kODNULL, 0) ) {
       transform= new ODTransform;
       THROW_IF_NULL(transform);
       TRY{
           transform->InitTransform(ev);
           su->Focus(ev, property, kODPosUndefined, kODNULL, 0, kODPosUndefined);
           transform->ReadFrom(ev, su);
       }CATCH_ALL{
           delete transform;
           RERAISE;
       }ENDTRY
   }
   return transform;
}

//------------------------------------------------------------------------------
// ODFrame: InitFrameFromStorage
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameInitFrameFromStorage(ODFrame *somSelf, Environment *ev,
       ODStorageUnit*  storageUnit)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","InitFrameFromStorage");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::InitFrameFromStorage()\n");
#endif
   /* Moved from somInit. SOM itself sets fields to zero
   _fDraft             = kODNULL;
   _fNPID              = 0;
   _fDirty             = kNotDirty;

   _fContainingFrame   = kODNULL;
   _fWindow            = kODNULL;

   _fFrameShape        = (ODShape*) kODNULL;
   _fUsedShape         = (ODShape*) kODNULL;

   _fInternalTransform = (ODTransform*) kODNULL;

   _fFacets            = kODNULL;

   _fPart              = kODNULL;
   _fPartInfo          = (ODInfoType) kODNULL;
   _fViewType          = (ODTypeToken) 0;
   _fPresentation      = (ODTypeToken) 0;

   _fFrameGroup        = 0;
   _fSequenceNumber    = 0;

   _fLinkStatus        = kODNotInLink;
   _fIsRoot            = kODFalse;
   _fIsSubframe        = kODFalse;
   _fIsOverlaid        = kODFalse;
   _fIsFrozen          = kODFalse;
   _fIsDroppable       = kODFalse;
   _fIsDragging        = kODFalse;
   _fDoesPropagateEvents = kODFalse;
   */
   _fDirty             = kNotDirty;
   _fLinkStatus        = kODNotInLink;
   _fValidState        = kFrameIsValid;

   // *DON'T* CHECKVALID - need to read an "I'm invalid" representation

   SOM_TRY
       if (!storageUnit)
           THROW(kODErrIllegalNullStorageUnitInput);

       somSelf->InitPersistentObjectFromStorage(ev, storageUnit);
       _fDraft = storageUnit->GetDraft(ev);

   // mark this frame as invalid/removed if it was externalized without a part
   if ( !storageUnit->Exists(ev, kODPropPart, kODStrongStorageUnitRef, 0) )
   {
       _fValidState = kFrameIsRemoved;
       _fDirty = kNotDirty;
       return;
   }

       ASSERT(storageUnit->Exists(ev, kODPropFrameShape, kODPolygon, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropInternalTransform, kODTransform, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropViewType, kODISOStr, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropPresentation, kODISOStr, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropFrameGroup, kODULong, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropSequenceNumber, kODULong, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropLinkStatus, kODULong, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropIsSubframe, kODBoolean, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropIsOverlaid, kODBoolean, 0), kODErrInvalidPersistentFormat);
       ASSERT(storageUnit->Exists(ev, kODPropIsFrozen, kODBoolean, 0), kODErrInvalidPersistentFormat);

       somSelf->CommonInitFrame(ev);

       ODULong valueSize = 0;
       ODStorageUnit* su = somSelf->GetStorageUnit(ev);
       ODSession* session = su->GetSession(ev);
       ODType typeString;

       // don't internalize _fContainingFrame - be lazy and do it later

       // geometry - must use biasTransform to normalize

       TempODTransform biasTransform = // DMc refcount - make temp
           ReadAndAcquireTransformIfAny(ev, su, kODPropBiasTransform);

       _fFrameShape = ReadAndAcquireShapeIfAny(ev, su, kODPropFrameShape);
       if ( !_fFrameShape )
           THROW(kODErrInvalidPersistentFormat);
       if ( _fFrameShape && biasTransform )
           _fFrameShape->Transform(ev, biasTransform);

       _fInternalTransform = ReadAndAcquireTransformIfAny(ev, su, kODPropInternalTransform);
       if ( _fInternalTransform && biasTransform )
           _fInternalTransform->PreCompose(ev, biasTransform);

       // don't internalize _fPart or _fPartInfo

       // internalize viewType and presentation

       typeString = ODGetISOStrProp(ev, su, kODPropViewType, kODISOStr, kODNULL, &valueSize);
       if ( valueSize > 0 )
           _fViewType = session->Tokenize(ev, typeString);
       if (typeString)
           ODDisposePtr(typeString);

       typeString = ODGetISOStrProp(ev, su, kODPropPresentation, kODISOStr, kODNULL, &valueSize);
       if ( valueSize > 0 )
           _fPresentation = session->Tokenize(ev, typeString);
       if (typeString)
           ODDisposePtr(typeString);

       // _fIsRoot set if containingFrame is valid suRef
       if ( ODSUExistsThenFocus(ev, su, kODPropContainingFrame, kODWeakStorageUnitRef) )
       {
           ODStorageUnitRef value;
           StorageUnitGetValue(su, ev, kODStorageUnitRefLen, (ODValue)&value);
           _fIsRoot = !(su->IsValidStorageUnitRef(ev, value));
       }
       else
           _fIsRoot = kODTrue;


       _fFrameGroup     = ODGetULongProp(ev, su, kODPropFrameGroup, kODULong);
       _fSequenceNumber = ODGetULongProp(ev, su, kODPropSequenceNumber, kODULong);
       _fLinkStatus     = ODGetULongProp(ev, su, kODPropLinkStatus, kODULong);
       _fIsSubframe     = ODGetBooleanProp(ev, su, kODPropIsSubframe, kODBoolean);
       _fIsOverlaid     = ODGetBooleanProp(ev, su, kODPropIsOverlaid, kODBoolean);
       _fIsFrozen       = ODGetBooleanProp(ev, su, kODPropIsFrozen, kODBoolean);

       _fDirty = kNotDirty;
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: InitFrameNonPersistent
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODFrameInitFrameNonPersistent(ODFrame *somSelf, Environment *ev,
               ODDraft*    draft,
               ODID        id,
               ODFrame*    containingFrame,
               ODShape*    frameShape,
               ODCanvas*   biasCanvas,
               ODPart*     part,
               ODTypeToken viewType,
               ODTypeToken presentation,
               ODBoolean   isSubframe,
               ODBoolean   isOverlaid)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","InitFrameNonPersistent");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::InitFrameNonPersistent()\n");
#endif
   // *DON'T* CHECKVALID - new frames will never be invalid

   SOM_TRY
       if ( kODNULL == draft )         THROW(kODErrIllegalNullStorageUnitInput);
       if ( kODNULL == frameShape  )   THROW(kODErrIllegalNullShapeInput);
       if ( kODNULL == part )          THROW(kODErrIllegalNullPartInput);
       if ( kODNULL == viewType )      THROW(kODErrIllegalNullTokenInput);
       if ( kODNULL == presentation )  THROW(kODErrIllegalNullTokenInput);

       // check for recursive embedding
       if ( (containingFrame != kODNULL) && !isSubframe )
       {
           TempODFrame tempFrame = containingFrame;
           tempFrame->Acquire(ev);   // because it will be released below
           while ( tempFrame )
           {
               TempODPart tempPart = tempFrame->AcquirePart(ev);
               if (tempPart == part)
                   THROW(kODErrIllegalRecursiveEmbedding);
               ODFrame* spam = tempFrame;
               TempODFrame oldFrame = spam;
               tempFrame = kODNULL; // to avoid double Release if following fails
               tempFrame = oldFrame->AcquireContainingFrame(ev);
           }
       }

       somSelf->InitPersistentObject(ev, kODNULL);

       _fDraft = draft;
       _fNPID = id;

       _fContainingFrame = containingFrame;
       ODAcquireObject(ev, _fContainingFrame);

       _fFrameShape = BiasShapeSet(ev, frameShape, biasCanvas);
       ODAcquireObject(ev, _fFrameShape);

       _fInternalTransform = somSelf->CreateTransform(ev);
       _fContentExtent     = ODPoint(0,0);

       _fViewType      = viewType;
       _fPresentation  = presentation;
       _fIsRoot        = (containingFrame == kODNULL);
       _fIsSubframe    = isSubframe;
       _fIsOverlaid    = isOverlaid;

       somSelf->CommonInitFrame(ev);

       _fPart = part;
       ODAcquireObject(ev, _fPart);
       _fPart->DisplayFrameAdded(ev, somSelf);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: ReleaseAll
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFrameReleaseAll(ODFrame *somSelf, Environment *ev)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","ReleaseAll");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::ReleaseAll()\n");
#endif
   // *DON'T* CHECKVALID - still need to finalize invalid frames

   SOM_TRY
       ODSafeReleaseObject(_fContainingFrame);
       ODSafeReleaseObject(_fWindow);
       ODSafeReleaseObject(_fPart);

       ODSafeReleaseObject(_fFrameShape);
       ODSafeReleaseObject(_fUsedShape);
       ODSafeReleaseObject(_fInternalTransform);

       parent_ReleaseAll(somSelf,ev);
   SOM_CATCH_ALL
   SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODFrame: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODFramesomUninit(ODFrame *somSelf)
{
    ODFrameData *somThis = ODFrameGetData(somSelf);
    ODFrameMethodDebug("ODFrame","somUninit");

#ifdef _FACET_DEBUG_
    qprintf("Executing ODFrame::somUninit()\n");
#endif
   ODDeleteObject(_fFacets);
}
