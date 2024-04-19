//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: oddataxfer
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
// @(#) 1.7 com/src/storage/LinkSpec.cpp, oddataxfer, od96os2, odos29646d 8/15/96 17:25:23 [ 11/15/96 15:25:53 ]
/*
        File:           LinkSpec.cpp

        Contains:       Implementation for ODLinkSpec class.

        Owned by:       Craig Carper

        Copyright:      й 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <14>     10/8/95        TJ              Fixes Recomended by Refball
                <13>     8/16/95        NP              1274946: ErrorDef.idl problems. Add include
                                                                        file.
                <12>     8/12/95        TВ              1276812 Need to use TempObjs and TempRefs
                                                                        for exception safety and to avoid TRY
                                                                        blocks, 1276807 Opt./Bug: use StdTypIO
                                                                        routines for portable streaming & smaller
                                                                        footprint
                <11>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <10>     5/26/95        VL              1251403: Multithreading naming support.
                 <9>     4/27/95        CC              1242555: GetOriginatingProcessID:  Removed
                                                                        obsolete $5 comment.
                 <8>     3/10/95        CC              1225926:  Fixed typo in FromThisDraft().
                 <7>     1/27/95        TJ              Changed ODAppleEvent to AppleEvent.
                 <6>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                 <5>    12/15/94        CC              1205709 LinkSpec.cpp - Removed AcquirePart(),
                                                                        GetData(), and GetDataSize() methods;
                                                                        changed ReadLinkSpec(), WriteLinkSpec(),
                                                                        ReadFromAppleEvent(), and
                                                                        WriteToAppleEvent().
                 <4>     12/8/94        CC              1203864 WriteToAppleEvent: changed &_fData
                                                                        to _fData, to copy the link data correctly.
                 <3>     9/15/94        CC              1186775 - Added SOM_CATCH to
                                                                        FromThisDraft(); Return som error from
                                                                        WriteToAppleEvent().
                 <2>     7/27/94        CC              Removed GetProcessLaunchDate, added
                                                                        FromThisDraft, changed GetProcessID to
                                                                        GetOriginatingProcessID.
                 <1>     6/21/94        CC              first checked in

                ------------------- CC ---- Converted to SOM

                 <2>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <1>     3/25/94        CC              first checked in

                ------------------- CC ---- Moved from Core to Storage subsystem

                 <3>     3/24/94        CC              Changed WriteToProperty() to
                                                                        WriteLinkSpec(), ReadFromProperty() to
                                                                        ReadLinkSpec() (1153049); write version in
                                                                        WriteToAppleEvent().
                 <2>     3/15/94        MB              Changes to support SCpp/ASLM builds,
                                                                        #1150864.
                 <2>      2/8/94        TВ              Throw -> THROW & some code clean up
                 <1>     1/21/94        CC              first checked in
                 <4>    12/21/93        VL              Changed XMPStorageUnit::GetValueSize to
                                                                        StorageUnit::GetSize.
                 <3>     12/8/93        CC              Added WriteToAppleEvent,
                                                                        ReadFromAppleEvent, and accessors for
                                                                        fields.
                 <2>     12/3/93        TВ              Stop including XMPError.h, it is included
                                                                        as ErrorDef.h inside Except.h
                 <1>    11/15/93        CC              first checked in
        To Do:
*/

#define VARIABLE_MACROS
#define METHOD_MACROS

#define ODLinkSpec_Class_Source
#include <LinkSpec.xih>

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include "StdProps.xh"
#endif

#ifdef _PLATFORM_OS2_

 #ifdef _OSA_SUPPORT_
  #define INCL_OSAAPI
  #define INCL_OSA
 #endif
  #include <os2.h>

  #ifndef _ISOSTR_
  #include <ISOStr.h>
  #endif

  #ifndef _IODDEFS_
  #include <IODDefs.xh>
  #endif

  #ifndef _STORUTIL_
  #include <storutil.h>
  #endif

  #include <ODPagtun.h>

  #ifndef _ODMEMORY_
  #include <ODMemory.h>
  #endif


  #include <iciditr.xh>

  #ifndef SOM_IODLinkService_xh
    #include <ILnkSrvc.xh>
  #endif

  #ifndef SOM_ODLink_xh
    #include <link.xh>
  #endif

  #ifndef SOM_IODLinkSource_xh
    #include <ILinkSrc.xh>
  #endif
  #include <ILnkTrgt.xh>

  #ifndef SOM_ODStorageUnit_xh
    #include <StorageU.xh>
  #endif

  #ifndef SOM_ODPart_xh
   #include <Part.xh>
  #endif

  #include <process.h>
  #include <somd.xh>
  #include <stdio.h>

  #ifndef SOM_Module_OpenDoc_StdTypes_defined
  #include <StdTypes.xh>
  #endif

  #ifndef SOM_ODDraft_xh
  #include <Draft.xh>
  #endif

  #ifndef _ITEXT_
  #include <IText.h>
  #endif

#endif


#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include "StdTypes.xh"
#endif

#ifndef SOM_ODDraft_xh
#include "Draft.xh"
#endif

#ifndef SOM_ODStorageUnit_xh
#include "StorageU.xh"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifdef _PLATFORM_OS2_ // CED
#include <builtin.h>
#else
#ifndef __ERRORS__
#include <Errors.h>
#endif
#endif


#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifndef _PLATFORM_OS2_
#pragma segment ODLinkSpec
#endif

//==============================================================================
// Constants
//==============================================================================
#ifdef _PLATFORM_OS2_
#define kODULongLen          4
const kODOS2 = 1;
const kODDSOMExternalizedIDSize = 1024;
#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_
const ODULong kLinkSpecVersion = 1;     // Second version

// ееее kODSourcePartKey also defined for Drag & Drop ееее
// ееее These don't belong here ееее
#define kODProcessLaunchDateKey 'pldt'
#define kODSourcePartKey                        'part'
#define kODLinkSpecDataKey                      'data'
#define kODLinkSpecDataSizeKey          'size'
#define kODLinkSpecVersionKey           'vers'
#endif // _PLATFORM_MACINTOSH

//==============================================================================
// ODLinkSpec
//==============================================================================

//------------------------------------------------------------------------------
// ODLinkSpec::somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSpecsomUninit(ODLinkSpec *somSelf)
{
    /* ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf); */
    ODLinkSpecMethodDebug("ODLinkSpec","somUninit");

    #ifdef _PLATFORM_OS2_
    ODLinkSpec_parent_ODBaseLinkSpec_somUninit(somSelf);
    #else
    parent_somUninit(somSelf);
    #endif
}

#ifdef _PLATFORM_OS2_

 SOM_Scope void  SOMLINK ODLinkSpecsomInit(ODLinkSpec *somSelf)
{

/*
-------------------------------------------------------------------------------
 1. Initialize the Base LinkSpec.
 2. Initialize constants:  version id
//  3. Go into U state.
-------------------------------------------------------------------------------
*/
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","ODLinkSpecsomInit");

    // 1.
    ODLinkSpec_parent_ODBaseLinkSpec_somInit(somSelf);

    // 2.
    _fVersion=kODOS2;

    // 3.
    _fState=ODLinkSpec_UNINIT;


}


/*
 * Called in LinkTarget::EstablishLink to establish a new link with the
 *   source part.
 * If aState is TRANSIENTLINKABLE, it internalizes fSourceLinkService and
 *   invokes its EstablishLinkFromTSpec method.
 * If aState is PERSISTENTLINKABLE, it internalizes aLinkRegistry and
 *   queries the LinkRegistry for the reference of the LinkSource to
 *   invokes its AddTarget method.
 */

SOM_Scope void
SOMLINK ODLinkSpecEstablishLink(ODLinkSpec *somSelf,
                                Environment *ev,
                                IODLinkTarget* linkTrgt,
                                ODULong pid,
                                ODLinkConnectionData* trgtPartDesc,
                                IODLinkService* trgtLinkService,
                                ODStorageUnitID* trgtPartSUID,
                                ODLinkID* linkID,
                                IODLinkTargetToLinkSource** linkSrc,
                                ODLinkConnectionID* trgtID,
                                ODBoolean* isLocal,
                                ODBoolean* trackTrgt,
                                ODISOStr* asName)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","ODLinkSpecEstablishLink");


/*
-------------------------------------------------------------------------------
0. Return targetpart SUID
1. If state is T,
   1.1. internalize fSourceLinkService,
   1.2. invoke EstablishLinkFromTSpec
   1.2. invoke RegisterLinkTarget
2. If state is P,
   2.1. internalize aLinkRegistry (Avail Server name) into LTToLS pointer,
   2.2. extract linkRegistry as asName, linkID as linkID
   2.3. invoke AddTarget on LinkSource via avail server
  2.4. If local, get linkSrc pointer from SUID;
        else use AvailServer dispatcher as linkSrc pointer

-------------------------------------------------------------------------------
*/
  #ifndef _PLATFORM_OS2_
   SOM_CATCH return;
  #else
   SOM_TRY
  #endif

  IODLinkService *sourceLinkService;

  // 0.
  *trgtPartSUID = _fTargetPartSUID;

  switch (_fState) {
    case ODLinkSpec_TRANSIENTLINKABLE:
      // 1.
      {
        ODStorageUnitID linkSrcSUID;
        // 1.1

        //sourceLinkService = (IODLinkService*)
        //                    SOMD_ObjectMgr->
        //                      somdGetObjectFromId(ev, _fSourceLinkService);

        SOMDObject* objRef = SOMD_ORBObject->
                               string_to_object(ev, _fSourceLinkService);

        sourceLinkService = (IODLinkService*)
                            SOMD_ServerObject->
                              somdSOMObjFromRef(ev, objRef);


        // 1.2.
        sourceLinkService->EstablishLinkFromTSpec(ev,
                                                  pid,
                                                  _fSourcePartSUID,
                                                  &_fData,
                                                  trgtPartDesc,
                                                  linkTrgt,
                                                  linkID,
                                                  linkSrc,
                                                  trgtID,
                                                  isLocal,
                                                  trackTrgt,
                                                  asName);

        // 1.3.
       trgtLinkService->RegisterLinkTarget(ev,
                                           _fTargetPartSUID,
                                           linkTrgt);
      }
      break;

    case ODLinkSpec_PERSISTENTLINKABLE:
      // 2.
      {
        //ODISOStr asName = _fASName; /* no use to save it anymore */
        ODStorageUnitID sourceSUID;

        //2.1.
        SOMDServer *server = SOMD_ObjectMgr->somdFindServerByName(ev, _fASName);
        if (ODGetSOMException(ev) == kODNoError)
        {
          *linkSrc = (IODLinkTargetToLinkSource*)
                      server->somdCreateObj(ev, "IODAvailServer", "");
        }
        else
        {
          *linkSrc = kODNULL;
          THROW(kODErrCorruptLinkSpecValue);
        }

        // 2.2.
        *linkID = _fLinkID;
        *asName = _fASName;

        // 2.3.
        ((*linkSrc))->AddLinkTarget(ev,
                                    *linkID,
                                    pid,
                                    trgtPartDesc,
                                    linkTrgt,
                                    &sourceSUID,
                                    trgtID,
                                    isLocal,
                                    trackTrgt);

        // 2.4.
        if (*isLocal)
          {
            *linkSrc = (IODLinkTargetToLinkSource*)linkTrgt->GetStorageUnit(ev)->GetDraft(ev)->AcquireLinkSource(ev, sourceSUID);
          }

      }
      break;
    default:
      {THROW(kODErrCorruptLinkSpecValue);}

  }

  #ifdef _PLATFORM_OS2_
    SOM_CATCH_ALL
    SOM_ENDTRY
      return;
  #endif

}


/*
 *=============================================================================
 * Called by target Part or draft to retrieve the
 *   description of the source Part.
 *
 * <1>  Make sure you are in the P state
 * <2>  Use the ASName to get access to the dispatcher
 * <3>  Invoke GetSourcePartDescription method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope void
SOMLINK ODLinkSpecGetLinkSourceConnectionData (ODLinkSpec *somSelf,
                                                        Environment *ev,
                                                        ODLinkConnectionData** srcConnData)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","GetLinkSourceConnectionData");


   //---<1>
    if (_fState != ODLinkSpec_PERSISTENTLINKABLE)
      {
      THROW(kODErrNoLinkSource);
      return;
      }

    //---<2>
     IODLinkTargetToLinkSource* myLinkSrc;
     SOMDServer *server = SOMD_ObjectMgr->somdFindServerByName(ev, _fASName);
     if (ODGetSOMException(ev) == kODNoError)
        {
          myLinkSrc = (IODLinkTargetToLinkSource*)
                      server->somdCreateObj(ev, "IODAvailServer", "");
        }
     else
        {
          THROW(kODErrBrokenLinkSource);
          return;
        }

    //---<3>
     TRY
       myLinkSrc->GetSourcePartDescription(ev, _fLinkID, srcConnData);
     CATCH_ALL
       THROW(kODErrBrokenLinkSource);
     ENDTRY
}


/*
 * Called in Draft::CreateLinkSpec to initialize the LinkSpec after it
 *   is created.
 * If caller is the draft of a source part, it must provide the
 *   linkService and source partSUID for call back to establish a link.
 * If caller is the draft of a target part, it only needs to provide
 *   the target partSUID for the LinkSpec to query the part attributes.
 * In either case the TI state will be entered.
 *
 */

SOM_Scope void
SOMLINK ODLinkSpecInitLinkSpec(ODLinkSpec *somSelf,
                               Environment *ev,
                               IODDSOMExternalizedID linkService,
                               //XXXXX ODStorageUnitID partSUID,
                               ODPart* part,
                               ODByteArray* data)
{

/*
-------------------------------------------------------------------------------
1. Error if not currently in U state.
2. Initialize the ODObject
3. Save linkService, partSUID, data, and size; Enter the TI state.
-------------------------------------------------------------------------------
*/
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","ODLinkSpecInitLinkSpec");

// 1.
    if (_fState != ODLinkSpec_UNINIT)
      {
        THROW(kODErrCorruptLinkSpecValue);
      }

// 2.
    #ifndef _PLATFORM_OS2_
     SOM_CATCH return;
    #else
     SOM_TRY
    #endif

//   Can not call IsInitialized any more. Make sure InitObject can handle
//   initialization.

//    if ( !_IsInitialized(ev) )
      _InitObject(ev);


// 3.
    _fState=ODLinkSpec_TRANSIENT_INIT;
    _fSourcePartSUID=kODNULL; /* in case TRY fails */
    _fSourceLinkService = (IODDSOMExternalizedID) kODNULL; /* in case TRY fails */
    //_fData = (ODByteArray*) kODNULL;
    _fData = CreateEmptyByteArrayStruct(0);
    TRY
      //XXXXX __set_aSourcePartSUID(ev, partSUID);
      if (part != (ODPart*)kODNULL)
        _fSourcePartSUID=part->GetID(ev);
      else
        _fSourcePartSUID=0;

      _fSourceLinkService = ODISOStrFromCStr(linkService);
      if (data != (ODByteArray*) kODNULL)
        {
          _fData = CopyByteArrayStruct(data);
        }
      somSelf->InitBaseLinkSpec(ev, part, data); //XXXXX

    CATCH_ALL
      _VoidLinkSpec(ev);
      RERAISE;
    ENDTRY

    #ifdef _PLATFORM_OS2_
     SOM_CATCH_ALL
     SOM_ENDTRY
    return;
    #endif
 }


/*
 * Called to void this LinkSpec.
 */

SOM_Scope void  SOMLINK ODLinkSpecVoidLinkSpec(ODLinkSpec *somSelf,
                                                Environment *ev)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","ODLinkSpecVoidLinkSpec");

/*
-------------------------------------------------------------------------------
1. Uninit all the storage and go to U state
-------------------------------------------------------------------------------
*/

   switch (_fState) {
     case ODLinkSpec_TRANSIENT_INIT: /* free space for Data, fSourceLinkService */
     case ODLinkSpec_TRANSIENTLINKABLE: /* same */
       if (_fSourceLinkService != (IODDSOMExternalizedID) kODNULL)
       {
         ODDisposePtr (_fSourceLinkService);
         _fSourceLinkService = (IODDSOMExternalizedID) kODNULL;
       }
       DisposeByteArrayStruct(_fData);
       _fState=ODLinkSpec_UNINIT;
       break;
     case ODLinkSpec_PERSISTENTLINKABLE: /* free ASName */
     {
       ;
       if (_fASName !=  kODNULL)
       {
         SOMFree(_fASName);
         _fASName=kODNULL ;
        }
       _fState=ODLinkSpec_UNINIT;
       break;
     }

     default:
       break;
   }
}

#endif // _PLATFORM_OS2_


#ifdef _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
// ODLinkSpec::InitLinkSpec
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSpecInitLinkSpec(ODLinkSpec *somSelf, Environment *ev,
                ODPart* part,
                ODByteArray* partData)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","InitLinkSpec");

        SOM_CATCH return;

        /* Moved from somInit. SOM itself sets fields to zero
        _fProcessLaunchDate = 0;
        */
        _fProcessID.lowLongOfPSN = kNoProcess;
        _fProcessID.highLongOfPSN = 0;

        somSelf->InitBaseLinkSpec(ev, part, partData);

        if ( part )
        {
                ProcessSerialNumber psn;
                ProcessInfoRec myProcessInfo;

                psn.lowLongOfPSN = kCurrentProcess;
                psn.highLongOfPSN = 0;
                myProcessInfo.processAppSpec = NULL;
                myProcessInfo.processName = NULL;
                myProcessInfo.processInfoLength = sizeof(myProcessInfo);

                OSErr error = GetProcessInformation(&psn, &myProcessInfo);
                ASSERT((error == 0), kODErrAssertionFailed);

                _fProcessID = myProcessInfo.processNumber;
                _fProcessLaunchDate = myProcessInfo.processLaunchDate;
        }
}
#endif // _PLATFORM_MACINTOSH_

//------------------------------------------------------------------------------
// ODLinkSpec::FromThisDraft (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODLinkSpecFromThisDraft(ODLinkSpec *somSelf, Environment *ev)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","FromThisDraft");

        #ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return kODFalse;

        ProcessSerialNumber myPSN;
        ProcessInfoRec myProcessInfo;
        Boolean isSamePSN;

        myPSN.lowLongOfPSN = kCurrentProcess;
        myPSN.highLongOfPSN = 0;
        myProcessInfo.processAppSpec = NULL;
        myProcessInfo.processName = NULL;
        myProcessInfo.processInfoLength = sizeof(myProcessInfo);
        THROW_IF_ERROR(GetProcessInformation(&myPSN, &myProcessInfo));
        THROW_IF_ERROR(SameProcess(&_fProcessID, &myProcessInfo.processNumber, &isSamePSN));

        return (isSamePSN && (myProcessInfo.processLaunchDate == _fProcessLaunchDate));
        #else
        return(kODFalse);
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSpec::GetOriginatingProcessID
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSpecGetOriginatingProcessID(ODLinkSpec *somSelf, Environment *ev,
                ProcessSerialNumber *psn)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","GetProcessLaunchDate");

        #ifdef _PLATFORM_OS2_
        _interrupt( 3 );
        #else
        ProcessInfoRec info;

        info.processAppSpec = NULL;
        info.processName = NULL;
        info.processInfoLength = sizeof(info);

        if ( (GetProcessInformation(&_fProcessID, &info) == noErr) &&
                        (info.processLaunchDate == _fProcessLaunchDate) )
        {
                *psn = _fProcessID;
        }
        else
        {
                psn->lowLongOfPSN = kNoProcess;
                psn->highLongOfPSN = 0;
                ODSetSOMException(ev, kODErrCannotGetExternalLink);
        }
        #endif
}

//------------------------------------------------------------------------------
// ODLinkSpec::WriteLinkSpec (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSpecWriteLinkSpec(ODLinkSpec *somSelf, Environment *ev,
                ODStorageUnit* su)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","WriteLinkSpec");

/*
-------------------------------------------------------------------------------
1. If state is U, reject
2. (no longer focus to a property; assume already focussed)
2. Assumes su has been focused to a property and link spec value
3. If state is P, write version, Persistent, aLinkID, aLinkRegistry
4. If state is TI or T, write version, Transient, aSourcePartSUID, fSourceLinkService, fDataSize, fData
-------------------------------------------------------------------------------
*/

#ifdef _PLATFORM_OS2_

//    ODLinkSpec_parent_ODBaseLinkSpec_WriteLinkSpec(somSelf, ev, su);
         ODULong version = _fVersion;
      ODULong subVersion;
      ODLinkID linkID;
      ODISOStr asName;
      ODStorageUnitID sourcePartSUID;

      #ifndef _PLATFORM_OS2_
         SOM_CATCH return;
      #else
         SOM_TRY
      #endif

  ODVolatile(su);


  ODSUForceFocus(ev, su, kODNULL, kODLinkSpec);


  // 1.
  if (_fState == ODLinkSpec_UNINIT)
    {
      THROW(kODErrCorruptLinkSpecValue);
    }

  // 2.

  switch (_fState) {
    case ODLinkSpec_PERSISTENTLINKABLE:
      // 3.
      TRY
        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &version);
        subVersion = ODLinkSpec_PERSISTENT_LINKSPEC_VALUE;
        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &subVersion);
        linkID = _fLinkID;
        StorageUnitSetValue(su, ev, sizeof(ODLinkID), (ODValue) &linkID);
        asName = _fASName;
        ODULong strLength = strlen(asName) + 1;/* length of string + null terminator */
        StorageUnitSetValue(su, ev, kODULongLen, (ODValue)&strLength);
        StorageUnitSetValue(su,
                            ev,
                            strLength,
                            (ODValue)asName);
      CATCH_ALL
        su->Remove(ev);    //??????
        RERAISE;
      ENDTRY

      break;
    case ODLinkSpec_TRANSIENTLINKABLE:
    case ODLinkSpec_TRANSIENT_INIT:
      // 4.
      TRY
        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &version);
        subVersion = ODLinkSpec_TRANSIENT_LINKSPEC_VALUE;
        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &subVersion);
        sourcePartSUID = _fSourcePartSUID;
        StorageUnitSetValue(su, ev, sizeof(ODStorageUnitID), (ODValue) &sourcePartSUID);
        StorageUnitSetValue(su, ev, kODDSOMExternalizedIDSize, (ODValue) _fSourceLinkService);
        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &_fData._length);
        StorageUnitSetValue(su, ev, _fData._length, (ODValue) _fData._buffer);
      CATCH_ALL
        su->Remove(ev);
        RERAISE;
      ENDTRY

    }


#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;

        ODVolatile(su);

        ODULong version = kLinkSpecVersion;
        ODULong size;

        ODSUForceFocus(ev, su, kODNULL, kODLinkSpec);
        size = su->GetSize(ev);
        su->DeleteValue(ev, size);

        TRY
                StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &version);
                StorageUnitSetValue(su, ev, sizeof(ProcessSerialNumber), (ODValue) &_fProcessID);
                StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &_fProcessLaunchDate);

                parent_WriteLinkSpec(somSelf, ev, su);
        CATCH_ALL
                su->Remove(ev);
                RERAISE;
        ENDTRY
#endif // _PLATFORM_MACINTOSH_


#ifdef _PLATFORM_OS2_
  SOM_CATCH_ALL
  SOM_ENDTRY
    return;
#endif

}

//------------------------------------------------------------------------------
// ODLinkSpec::ReadLinkSpec (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODLinkSpecReadLinkSpec(ODLinkSpec *somSelf, Environment *ev,
                ODStorageUnit* su)
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","ReadLinkSpec");

/*
-------------------------------------------------------------------------------
1. If state is U, reject
2. If state is T or P, coerce down to TI
3. Move SourcePartSUID to TargetPartSUID; kill fSourceLinkService, fData
4. (we assume su has been focused to a property and link spec value,
   and the current offset is where the base link spec was written.) nop.
5. If version isn't kODOS2, reject
6. Check sub-version:
6.1. Transient: Read aSourcePartSUID, fSourceLinkService, fData.size, fData.value, and go to T
6.2. Persistent: Read aLinkID, aLinkRegistry, and go to P
-------------------------------------------------------------------------------
*/

#ifdef _PLATFORM_OS2_

    ODULong version;

   #ifndef _PLATFORM_OS2_
    SOM_CATCH return;
   #else
    SOM_TRY
   #endif

    // Pre-focused property must contain a link spec value
    if (! su)
      THROW(kODErrIllegalNullStorageUnitInput);
    if (!ODSUExistsThenFocus(ev, su, (ODPropertyName) kODNULL, kODLinkSpec))
      THROW(kODErrNoLinkSpecValue);

    // 1.
    if (_fState == ODLinkSpec_UNINIT)
      {
        THROW(kODErrCorruptLinkSpecValue);
      }

    // 2.
    if (_fState == ODLinkSpec_TRANSIENTLINKABLE)
      {
        _fState= ODLinkSpec_TRANSIENT_INIT;
      }

    if (_fState == ODLinkSpec_PERSISTENTLINKABLE)
      {
        ODDisposePtr(_fASName);
        _fState= ODLinkSpec_TRANSIENT_INIT;
        _fSourcePartSUID=_fTargetPartSUID;
        DisposeByteArrayStruct(_fData);
        _fSourceLinkService = (IODDSOMExternalizedID) kODNULL;
       }

    // 3.
    _fTargetPartSUID=_fSourcePartSUID;
    if (_fSourceLinkService != (IODDSOMExternalizedID) kODNULL)
    {
      ODDisposePtr (_fSourceLinkService);
      _fSourceLinkService = (IODDSOMExternalizedID) kODNULL;
    }
    DisposeByteArrayStruct(_fData);

    // 4.

    // 5.
    ODULong    valueSize; //-- used to check integrity of bytestream

    TRY
      {
        valueSize = StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &version);
        if (valueSize != sizeof(ODULong)) {
          _VoidLinkSpec(ev);
          THROW(kODErrCorruptLinkSpecValue);
        }
        if (version != _fVersion)
        {
          _VoidLinkSpec(ev);
          THROW(kODErrUnknownLinkSpecVersion);
        }
      }
    CATCH_ALL
      _VoidLinkSpec(ev);
      RERAISE;
    ENDTRY

   // 6.
    TRY
      {
        ODULong subVersion;
        ODLinkID linkID;
        ODISOStr asName;
        ODStorageUnitID sourcePartSUID;
        ODULong dataSize;

        valueSize = StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &subVersion);
        if (valueSize != sizeof(ODULong))
        {
          THROW(kODErrCorruptLinkSpecValue);
        }

        switch (subVersion) {
          case ODLinkSpec_TRANSIENT_LINKSPEC_VALUE:
            // 6.1
            _fSourceLinkService = (IODDSOMExternalizedID) kODNULL;
            _fState=ODLinkSpec_TRANSIENTLINKABLE;

            valueSize = StorageUnitGetValue(su, ev, sizeof(ODStorageUnitID), (ODValue) &sourcePartSUID);
            if (valueSize != sizeof(ODStorageUnitID))
            {
              THROW(kODErrCorruptLinkSpecValue);
            }
            _fSourcePartSUID= sourcePartSUID;
            _fSourceLinkService = (IODDSOMExternalizedID) ODNewPtr(kODDSOMExternalizedIDSize, kDefaultHeapID);
            valueSize = StorageUnitGetValue(su, ev, kODDSOMExternalizedIDSize, (ODValue) _fSourceLinkService);
            if (valueSize != kODDSOMExternalizedIDSize)
            {
              THROW(kODErrCorruptLinkSpecValue);
            }

            valueSize = StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &dataSize);
            if (valueSize != sizeof(ODULong))
            {
              THROW(kODErrCorruptLinkSpecValue);
            }
            _fData = CreateEmptyByteArrayStruct(dataSize);
            if (dataSize > 0)
            {
              valueSize = StorageUnitGetValue(su, ev, dataSize, (ODValue) _fData._buffer);
              _fData._length = dataSize;
              if (valueSize != dataSize)
              {
                THROW(kODErrCorruptLinkSpecValue);
              }

            }
            break;
          case ODLinkSpec_PERSISTENT_LINKSPEC_VALUE:

            // 6.2
            _fASName=kODNULL;
            _fState=ODLinkSpec_PERSISTENTLINKABLE;
            valueSize = StorageUnitGetValue(su, ev, sizeof(ODLinkID), (ODValue) &linkID);
            if (valueSize != sizeof(ODLinkID))
            {
              THROW(kODErrCorruptLinkSpecValue);
            }
            _fLinkID=linkID;
           ODULong strLength;
            StorageUnitGetValue(su, ev, kODULongLen, (ODValue)&strLength);
            asName = (ODISOStr) SOMMalloc(strLength);
            valueSize = StorageUnitGetValue(su,
                                ev,
                                strLength,
                                (ODValue)asName);
            if (valueSize != strLength)
            {
              THROW(kODErrCorruptLinkSpecValue);
            }
            _fASName=asName;
            break;
          default:
            THROW(kODErrUnknownLinkSpecVersion);
          }
     }
    CATCH_ALL
      _VoidLinkSpec(ev);
      RERAISE;
    ENDTRY

#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_

        SOM_CATCH return;

        ODULong                         version;
        ProcessSerialNumber processID;
        ODULong                         processLaunchDate;
        ODULong                         valueSize;

        // Pre-focused property must contain a link spec value
        if (!ODSUExistsThenFocus(ev, su, (ODPropertyName) kODNULL, kODLinkSpec))
                THROW(kODErrNoLinkSpecValue);

        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &version);
        if (version != kLinkSpecVersion)
                THROW(kODErrUnknownLinkSpecVersion);

        valueSize = StorageUnitGetValue(su, ev, sizeof(ProcessSerialNumber), (ODValue) &processID);
        if ( valueSize != sizeof(ProcessSerialNumber) )
                THROW(kODErrCorruptLinkSpecValue);

        valueSize = StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &processLaunchDate);
        if ( valueSize != sizeof(ODULong) )
                THROW(kODErrCorruptLinkSpecValue);

        parent_ReadLinkSpec(somSelf, ev, su);

        _fProcessID = processID;
        _fProcessLaunchDate = processLaunchDate;

#endif // _PLATFORM_MACINTOSH

#ifdef _PLATFORM_OS2_
    SOM_CATCH_ALL
    SOM_ENDTRY
      return;
#endif
}

#ifdef _OSA_SUPPORT_
//------------------------------------------------------------------------------
// ODLinkSpec::WriteToOSAEvent
//------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
SOM_Scope void  SOMLINK ODLinkSpecWriteToAppleEvent(ODLinkSpec *somSelf, Environment *ev,
                AppleEvent* theAppleEvent)
#endif

#ifdef _PLATFORM_OS2_
SOM_Scope void  SOMLINK ODLinkSpecWriteToOSAEvent(ODLinkSpec *somSelf,
                                                   Environment *ev,
                                                  OSAEvent* theOSAEvent)
#endif
{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","WriteToAppleEvent");

#ifdef _PLATFORM_OS2_ // CED
  _interrupt(3);
#endif

#ifdef _PLATFORM_MACINTOSH_
        SOM_TRY

        OSErr error;
        ODPart* part = somSelf->GetPart(ev);
        ODByteArray partData = somSelf->GetPartData(ev);

        ODULong version = kLinkSpecVersion;
        error = AEPutParamPtr((AppleEvent*) theAppleEvent, kODLinkSpecVersionKey, typeLongInteger,
                                                &version, sizeof(version));

        if ( error == noErr )
                error = AEPutParamPtr((AppleEvent*) theAppleEvent, keyProcessSerialNumber, typeProcessSerialNumber,
                                                        &_fProcessID, sizeof(_fProcessID));

        if ( error == noErr )
                error = AEPutParamPtr((AppleEvent*) theAppleEvent, kODProcessLaunchDateKey, typeLongInteger,
                                                        &_fProcessLaunchDate, sizeof(_fProcessLaunchDate));

        if ( error == noErr )
                error = AEPutParamPtr((AppleEvent*) theAppleEvent, kODSourcePartKey, typeLongInteger,
                                                        &part, sizeof(part));

        if ( error == noErr )
                error = AEPutParamPtr((AppleEvent*) theAppleEvent, kODLinkSpecDataSizeKey, typeLongInteger,
                                                        &partData._length, sizeof(ODULong));

        if ( (error == noErr) && (partData._length > 0) )
                error = AEPutParamPtr((AppleEvent*) theAppleEvent, kODLinkSpecDataKey, typeChar /* еее need better type here ееее */,
                                                        partData._buffer, partData._length);

        DisposeByteArrayStruct(partData);

        THROW_IF_ERROR(error);

        SOM_CATCH_ALL
        SOM_ENDTRY

#endif // _PLATFORM_MACINTOSH_

}

//------------------------------------------------------------------------------
// ODLinkSpec::ReadFromAppleEvent
//------------------------------------------------------------------------------

#ifdef _PLATFORM_MACINTOSH_
SOM_Scope void  SOMLINK ODLinkSpecReadFromAppleEvent(ODLinkSpec *somSelf, Environment *ev,
                AppleEvent* theAppleEvent)
#endif

#ifdef _PLATFORM_OS2_
SOM_Scope void  SOMLINK ODLinkSpecReadFromOSAEvent(ODLinkSpec *somSelf,
                                                    Environment *ev,
                                                   OSAEvent* theOSAEvent)
#endif

{
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","ReadFromAppleEvent");

        #ifdef _PLATFORM_OS2_ // CED
        _interrupt(3);
        #endif // _PLATFORM_OS2_

        #ifdef _PLATFORM_MACINTOSH_

        SOM_CATCH return;

        /* Moved from somInit. SOM itself sets fields to zero
        _fProcessLaunchDate = 0;
        _fProcessID.lowLongOfPSN = kNoProcess;
        _fProcessID.highLongOfPSN = 0;
        */

        OSErr                           result;
        DescType                        returnedType;
        Size                            actualSize;
        ODPart*                         part;
        ODByteArray                     partData;
        ProcessSerialNumber processID;
        ODULong                         processLaunchDate;
        ODULong                         dataSize;

        result = AEGetParamPtr((AppleEvent*) theAppleEvent, keyProcessSerialNumber, typeProcessSerialNumber, &returnedType,
                                                        &processID, sizeof(processID), &actualSize);
        THROW_IF_ERROR(result);

        result = AEGetParamPtr((AppleEvent*) theAppleEvent, kODProcessLaunchDateKey, typeLongInteger, &returnedType,
                                                        &processLaunchDate, sizeof(processLaunchDate), &actualSize);
        THROW_IF_ERROR(result);

        result = AEGetParamPtr((AppleEvent*) theAppleEvent, kODSourcePartKey, typeLongInteger, &returnedType,
                                                        &part, sizeof(part), &actualSize);
        THROW_IF_ERROR(result);

        result = AEGetParamPtr((AppleEvent*) theAppleEvent, kODLinkSpecDataSizeKey, typeLongInteger, &returnedType,
                                                        &dataSize, sizeof(ODULong), &actualSize);
        THROW_IF_ERROR(result);

        partData = CreateEmptyByteArrayStruct(dataSize);

        TRY
                if ( dataSize > 0 )
                {
                        result = AEGetParamPtr((AppleEvent*) theAppleEvent, kODLinkSpecDataKey, typeChar, &returnedType,
                                                                        partData._buffer, dataSize, &actualSize);
                        THROW_IF_ERROR(result);
                        partData._length = dataSize;
                }
                somSelf->InitBaseLinkSpec(ev, part, &partData);
        CATCH_ALL
                DisposeByteArrayStruct(partData);
                RERAISE;
        ENDTRY

        DisposeByteArrayStruct(partData);

        _fProcessID = processID;
        _fProcessLaunchDate = processLaunchDate;
        #endif // _PLATFORM_MACINTOSH_
}
#endif // _OSA_SUPPORT_


SOM_Scope void
SOMLINK ODLinkSpecConvertToPersistent(ODLinkSpec *somSelf,
                                Environment *ev)
{

/*
-------------------------------------------------------------------------------
1. If state is U or TI, reject
2. If state is P, do nothing
3. If state is T,
   3.1 get Linksrvc
   3.2 get Persistent Link Info
   3.3 save the ASName and LinkID
   3.4 erase the transient Link Info
   3.5 Change the state to P
-------------------------------------------------------------------------------
*/
    ODLinkSpecData *somThis = ODLinkSpecGetData(somSelf);
    ODLinkSpecMethodDebug("ODLinkSpec","ODLinkSpecWriteLinkSpec");
#ifdef _PLATFORM_OS2_

  switch (_fState) {

    // 1.
    case ODLinkSpec_UNINIT:
     THROW(kODErrCorruptLinkSpecValue);

    case ODLinkSpec_TRANSIENT_INIT:
     THROW(kODErrCorruptLinkSpecValue);

    // 2.
    case ODLinkSpec_PERSISTENTLINKABLE:
    return;

    // 3.
    case ODLinkSpec_TRANSIENTLINKABLE:
    IODLinkService* sourceLinkService=kODNULL;

    TRY
        // 3.1
        SOMDObject* objRef = SOMD_ORBObject->
                               string_to_object(ev, _fSourceLinkService);

        sourceLinkService = (IODLinkService*)
                            SOMD_ServerObject->
                              somdSOMObjFromRef(ev, objRef);

        // 3.2
        sourceLinkService->GetPersistentLinkInfoFromTSpec(ev, _fSourcePartSUID, &_fData, &_fLinkID, &_fASName);

        // 3.3
        DisposeByteArrayStruct(_fData);
        ODDisposePtr(_fSourceLinkService);
/*
//HAVING PROBLEM HERE
         if (((SOMDObject*)sourceLinkService)->is_proxy(ev))
            {
             SOMD_ObjectMgr->somdReleaseObject(ev, sourceLinkService);
             }
*/

        // 3.4
        _fState = ODLinkSpec_PERSISTENTLINKABLE;

    CATCH_ALL
      if (sourceLinkService!=kODNULL)
       {
          if (((SOMDObject*)sourceLinkService)->is_proxy(ev))
            {
             SOMD_ObjectMgr->somdReleaseObject(ev, sourceLinkService);
             }
        }
      _VoidLinkSpec(ev);
      THROW(kODErrCorruptLinkSpecValue);
    ENDTRY
    break;
    } //end of switch
    #endif
}
