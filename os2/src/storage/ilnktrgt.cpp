/* @(#)Z 1.9 os2/src/storage/ilnktrgt.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:56 (96/09/03 11:09:21) */
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


#ifdef _PLATFORM_OS2_
#include <odpagtun.h>
#endif

#ifdef _PLATFORM_OS2_
#ifndef SOM_Module_ILnkTrgt_Source
#define SOM_Module_ILnkTrgt_Source
#endif
#define IODLinkTarget_Class_Source

#define METHOD_MACROS
#define VARIABLE_MACROS

#ifdef _PLATFORM_OS2_
#ifdef _OSA_SUPPORT_
#define INCL_OSAAPI
#define INCL_OSA
#endif
#include <os2.h>
#endif

#ifndef _ODTYPESB_
#include <ODTypesB.xh>
#endif

#ifndef _ODTYPESF_
#include <ODTypesF.h>
#endif

#ifndef _ALTPOINT_
#include <AltPoint.h>
#endif

#ifndef _ODTYPESM_
#include <ODTypesM.xh>
#endif

#include <process.h>
#include <odmemory.h>

#include <ISOStr.h>
#include <IODDefs.xh>
#include <iciditr.xh>

#ifndef _STDTYPIO_
  #include <StdTypIO.h>
#endif
#ifndef _STORUTIL_
  #include <StorUtil.h>
#endif

#ifndef SOM_Module_StdPropsOpenDoc_StdDefs_defined
  #include "StdDefs.xh"
#endif

#ifndef SOM_Module_StdPropsOpenDoc_StdProps_defined
  #include "StdProps.xh"
#endif

#ifndef SOM_Module_StdTypes_OpenDoc_StdTypes_defined
  #include <StdTypes.xh>
#endif


#ifndef SOM_ODDraft_xh
  #include <Draft.xh>
#endif

#ifndef SOM_CMDocument_xh
#include <CMDoc.xh>
#endif
#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif
#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif
#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODFrame_xh
  #include <Frame.xh>
#endif

#ifndef SOM_ODLink_xh
  #include <link.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
  #include <StorageU.xh>
#endif


#ifndef SOM_IODLinkSource_xh
  #include <ILinkSrc.xh>
#endif

#ifndef SOM_IODLinkTargetToLinkSource_xh
  #include <ILTr2LSr.xh>
#endif

#ifndef SOM_ODLinkSpec_xh
  #include <LinkSpec.xh>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#include <lmgrdefs.h>

#include <somd.xh>

#include "ILnkTrgt.xih"

const ODPropertyName   kODPropCloneOfALink = "OpenDoc:Property:Link:Cloned";


void GetPartDescriptionProp (Environment *ev,
                             ODStorageUnit *su,
                             ODPropertyName prop,
                             ODValueType val,
                             ODLinkConnectionData &partDesc);

void SetPartDescriptionProp (Environment *ev,
                             ODStorageUnit *su,
                             ODPropertyName prop,
                             ODValueType val,
                             ODLinkConnectionData &partDesc);

void GetISOStrProp (Environment *ev,
                    ODStorageUnit *su,
                    ODPropertyName prop,
                    ODValueType val,
                    ODISOStr &isoStr);

void CopyPartDescription(ODLinkConnectionData &dest,
                                    ODLinkConnectionData &src);

void CleanupPartDescription (ODLinkConnectionData &partDesc);


/* ************************************************************************* */
/* The following are copied from FEB13 build in file:                        */
/* opendoc\src\opendoc\common\include\IText.h.                               */
/* Otto and Chris are working on NLS design and the following                */
/* might need to be redesigned when they are done. 5/14/96                   */
/* ************************************************************************* */
// typedef unsigned long CodePage;                      // (defined in odosacom.h)
// typedef CodePage      ODCodePage;
// ODCodePage APIENTRY GetITextCodePage(ODIText*);
// void       APIENTRY SetITextCodePage(ODIText*, ODCodePage);

/*
 *=============================================================================
 * Called by target Part or draft to retrieve the
 *   description of the source Part.
 *
 * <1>  Make sure link is not broken.
 * <2>  Invoke GetSourcePartDescription method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope void
SOMLINK GetLinkSourceConnectionData (IODLinkTarget *somSelf,
                                     Environment *ev,
                                     ODLinkConnectionData** srcConnData)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","GetLinkSourceConnectionData");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return;
    }

    //---<2>
    _fLinkSrc->GetSourcePartDescription(ev, _fLinkID, srcConnData);
}

/*
 *=============================================================================
 * <1>  Acquire fMutex before accessing fTrgtPartDesc.
 * <2>  Allocate memory for the out parameter trgtPartDesc.
 *      Copy the target part description to the out parameter trgtPartDesc.
 *      Release fMutex and return.
 * <3>  An exception is caught.
 *      Release fMutex.
 *      Cleanup trgtPartDesc.
 *      Set SOM exception and return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK GetConnectionData(IODLinkTarget *somSelf,
                          Environment *ev,
                          ODLinkConnectionData** trgtPartDesc)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","GetConnectionData");

    // Out parameter
    *trgtPartDesc = kODNULL;

    //---<1>
//    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    TRY

      //---<2>
      *trgtPartDesc = new ODLinkConnectionData;
      THROW_IF_NULL(*trgtPartDesc);

    //  CopyPartDescription(**trgtPartDesc, _fTrgtPartDesc);
        CopyPartDescription(**trgtPartDesc, _fTrgtConnData);

//      _fMutex->Release(ev);

    CATCH_ALL

      //---<3>
//      _fMutex->Release(ev);

      if (*trgtPartDesc != kODNULL)
      {
    //  CleanupPartDescription(**trgtPartDesc);
        CleanupPartDescription(**trgtPartDesc);
        delete *trgtPartDesc;
        *trgtPartDesc = kODNULL;
      }

//      SET_SOM_EV;

    ENDTRY
}
/*
 *=============================================================================
 * Called by target Part to set its commentary.
 *
 * <1>  Delete Old commentary.
 * <2>  Save new commentary.
 * <3>  Check if this is a legally tracked target, and if so:
 * <4>  If a link source exists, then:
 * <5>  Pass the new commentary to the source.
 *=============================================================================
 */
SOM_Scope void  SOMLINK SetDescription(IODLinkTarget *somSelf,
                                        Environment *ev, ODLinkDescription* Desc)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","SetDescription");

    //---<1>
    DisposeITextStruct(_fTrgtConnData.LinkDesc);

    //---<2>
    _fTrgtConnData.LinkDesc= CopyITextStruct (Desc);

    //---<3>
    if (  (_fTracked)
        ||(_fIsLocal)
        ||(_fIsRegistered)
       )
    {
    //---<4>
      if (_fLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
      {
    //---<5>
        _fLinkSrc->TargetPartDescriptionChanged(ev,
                                               _fLinkID,
                                               _fConnID,
                                               _fIsLocal,
                                               &_fTrgtConnData);
      } /* end if non-null fLinkSrc               */
    } /* end if this is a legally tracked target */

}

/*
 *=============================================================
 * Called by target Part to retrieve the description of itself.
 *
 * <1>  Acquire fMutex before accessing fSrcConnData.
 * <2>  Copy commentary.
 * <3>  Release fMutex.
 *=============================================================
 */

SOM_Scope void  SOMLINK GetDescription(IODLinkTarget *somSelf,
                                              Environment *ev,
                                             ODLinkDescription* desc)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","GetDescription");


    //---<1>
//    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<2>
    *desc = CopyITextStruct(&_fTrgtConnData.LinkDesc);

    //---<3>
//    _fMutex->Release(ev);


}

/*
 *=============================================================================
 * Returns wether or not a part is registered to receive updates
 *=============================================================================
 */


SOM_Scope ODBoolean  SOMLINK IsRegistered(IODLinkTarget *somSelf,
                                                 Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","IsRegistered");


    return (_fIsRegistered);

}

/*
 *=============================================================================
 * Called in Draft::GetLink to establish a link from the given LinkSpec.
 *
 * <1>  Invoke EstablishLink of LinkSpec and obtain values for the following
 *      variables in return.
 *
 *      ODStorageUnitID           fTrgtPartSUID;
 *      ODLinkConnectionData        fTrgtConnData;
 *      ODLinkID                  fLinkID;
 *      IODLinkTargetToLinkSource fLinkSrc;
 *      ODLinkConnectionID           fConnID;
 *      ODBoolean                 fIsLocal;
 *      ODBoolean                 fTracked;
 *      ODISOStr                  fASName;
 *=============================================================================
 */

SOM_Scope void
SOMLINK EstablishLink(IODLinkTarget *somSelf,
                      Environment *ev,
                      ODLinkSpec* linkSpec,
                      IODLinkService* trgtLinkService)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","EstablishLink");

    //---<1>
    linkSpec->EstablishLink(ev,
                            (IODLinkTarget*)somSelf,
                            _fPID,
                            &_fTrgtConnData,
                            trgtLinkService,
                            &_fTrgtPartSUID,
                            &_fLinkID,
                            &_fLinkSrc,
                            &_fConnID,
                            &_fIsLocal,
                            &_fTracked,
                            &_fASName);
}

/*
 *=============================================================================
 * Called in Draft::GetLink to initialize the LinkTarget after it is
 *   created.
 *
 * <1>  Init BaseLink.
 * <2>  create target part description.
 * <3>  Add the property kODPropAvailabilityServerName
 *      so that draft can distinguish link targets from other persistent objs.
 *=============================================================================
 */

SOM_Scope void
SOMLINK InitLinkTarget(IODLinkTarget *somSelf,
                       Environment *ev,
                       ODStorageUnit* su,
                       IODFileName pathName)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","InitLinkTarget");

    //---<1>
    _InitBaseLink(ev, su);

    //---<2>
     char            hostName[256];
     strcpy(hostName, getenv("HOSTNAME"));

     //_fTrgtConnData.HostName = ODISOStrFromCStr(hostName);
     _fTrgtConnData.HostName.format = kODIBMCodePageText;
     _fTrgtConnData.HostName.text._buffer = kODNULL;
     SetITextCString(&_fTrgtConnData.HostName, hostName);
     SetITextLangCode(&_fTrgtConnData.HostName, 0);
     SetITextScriptCode(&_fTrgtConnData.HostName, 0);

     //_fTrgtConnData.DocumentName  = ODISOStrFromCStr(pathName);
     _fTrgtConnData.DocumentName.format = kODIBMCodePageText;
     _fTrgtConnData.DocumentName.text._buffer = kODNULL;
     SetITextCString(&_fTrgtConnData.DocumentName, pathName);
     SetITextLangCode(&_fTrgtConnData.DocumentName, 0);
     SetITextScriptCode(&_fTrgtConnData.DocumentName, 0);

     //_fTrgtConnData.LinkDesc     = ODISOStrFromCStr("");
     _fTrgtConnData.LinkDesc.format = kODIBMCodePageText;
     _fTrgtConnData.LinkDesc.text._buffer = kODNULL;
     SetITextCString(&_fTrgtConnData.LinkDesc, "");
     SetITextLangCode(&_fTrgtConnData.LinkDesc, 0);
     SetITextScriptCode(&_fTrgtConnData.LinkDesc, 0);


   //---<3>
    ODSUAddPropValue(ev, su, kODPropAvailabilityServerName, kODISOStr);

}

/*
 *=============================================================================
 * Called in Draft::GetLink to initialize the LinkTarget from a
 *   pre-existing storage unit.
 *
 * <1>  Init BaseLink from the storagte.
 * <2>  Internalize the following variables from the storage:
 *        ODLinkConnectionID fConnID;
 *        ODBoolean       fIsLocal;
 *        ODLinkID        fLinkID;
 * <3>  LinkTarget was originally local:
*      <3.1> Convert SUID reference to an SUID
 *      This may fail if the LT was cloned and the LS it referred to wasn't.
 *      <3.1.A.1>If SUID exists, then obtain a pointer to the LS object
 *         <3.1.A.1.A> If you get a pointer, this is a pointer to the correct LS,
 *         but the LS-LT pair may have been cloned, so call GetPersistentLinkInfo
 *         to get the correct LinkID and the ASName
 *         <3.1.A.1.B> If you fail to get a pointer, this is a broken link.
 *         Restore the ASName from the SU and set the LinkSrc to null.
 *      <3.1.B.1> If SUID doesn't exist, this means the LT is a clone
 *         and may or may not still be local.
 *         Restore the ASName from the SU and set the LinkSrc to the
 *         availability server.  The correct LinkSrc pointer will
 *         be set in step 7.
 *
 *      Read SUID of LinkTargetToLinkSource from the SU.
 *      Convert the SUID to the reference of LinkTargetToLinkSource.
 *      Ask the linksource to tell me the linkid and ASName
 * <4>  LinkTarget was originally remote:
 *      Read Availability Server's name from the SU.
 *      Ask DSOM to find the server by the name just read.
 *      Ask the server found to create an instance of IODAvailServer.
 * <5>  Internalize the following variables from the storage:
 *        ODLinkConnectionData fTrgtPartDesc;
 *        ODStorageUnitID    fTrgtPartSUID;
 *        ODBoolean          fTracked;
 *        ODULong            fNumOfRegParts;
 * <6>  Reset the target part description hostName and docName to new values.
* <7>  If this is a Clone of a link,
 *      <7.1> Clear CloneOfALink property
 *      <7.2> If there is no SUID,
 *            <7.2.1> Call AddLinkTarget to tell LS that we have a new target.
 *            <7.2.2> If LS is now local, set isLocal, and replace
 *                    the reference to the AS with a reference to linkSrc
 *            <7.2.3> If LS is now remote, set isLocal false (leaving
 *                    linkSrc pointing to the AS).
 *=============================================================================
 */

SOM_Scope void
SOMLINK InitLinkTargetFromStorage(IODLinkTarget *somSelf,
                                  Environment *ev,
                                  ODStorageUnit* su,
                                  IODFileName pathName)

{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","InitLinkTargetFromStorage");
    ODID linkSourceSUID; // identifies the linksource if it's local
    //---<1>
    _InitBaseLinkFromStorage(ev, su);

    //---<2>
    _fConnID = ODGetULongProp(ev,su, kODPropConnectionID, kODULong);
    _fIsLocal = ODGetBooleanProp(ev,su, kODPropLinkLocality, kODBoolean);
    _fLinkID = ODGetULongProp(ev,su, kODPropLinkID, kODULong);

    if (_fIsLocal)
    {
      //---<3>
      //---<3.1.>

      linkSourceSUID = ODGetWeakSURefProp(ev,su,
                                          kODPropLinkTargetToLinkSource,
                                          kODWeakStorageUnitRef);
      if (linkSourceSUID != 0)
      {
        //---<3.1.A.1>
        TRY
          _fLinkSrc = (IODLinkTargetToLinkSource*)
                      su->GetDraft(ev)->AcquireLinkSource(ev, linkSourceSUID);
          //---<3.1.A.1.A>
          ((IODLinkSource*)_fLinkSrc)->GetPersistentLinkInfo(ev, &_fLinkID, &_fASName);
        CATCH_ALL
          //---<3.1.A.1.B>
          _fLinkSrc = kODNULL;
          GetISOStrProp(ev,
                        su,
                        kODPropAvailabilityServerName,
                        kODISOStr,
                        _fASName);

        ENDTRY
      }  /* linkSourceSUID != 0 */
    else
      {
        //---<3.1.B.1>
        GetISOStrProp(ev,
                      su,
                      kODPropAvailabilityServerName,
                      kODISOStr,
                      _fASName);
        SOMDServer *server = SOMD_ObjectMgr->somdFindServerByName(ev, _fASName);
        if (ODGetSOMException(ev) == kODNoError)
        {
          _fLinkSrc = (IODLinkTargetToLinkSource*)
                      server->somdCreateObj(ev, "IODAvailServer", "");
        }
        else
        {
          _fLinkSrc = kODNULL;
          return;
        }
      }  /* linksourceSUID == 0 */
    }    /* isLocal */
    else
    {
      //---<4>
     linkSourceSUID = 0;
     GetISOStrProp(ev,
                   su,
                   kODPropAvailabilityServerName,
                  kODISOStr,
                  _fASName);

     _GetStorageUnit(ev)->GetDraft(ev)->GetDocument(ev)->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev)->StartServer(ev, (char*)kODNULL);
     SOMDServer *server = SOMD_ObjectMgr->somdFindServerByName(ev, _fASName);
     if (ODGetSOMException(ev) == kODNoError)
     {
       _fLinkSrc = (IODLinkTargetToLinkSource*)
                   server->somdCreateObj(ev, "IODAvailServer", "");
     }
     else
     {
       _fLinkSrc = kODNULL;
       return;
     }

  }   /* !isLocal */

    //---<5>
    GetPartDescriptionProp(ev,
                           su,
                           kODPropPartDescription,
                           kODPartDescription,
                           _fTrgtConnData);

    _fTrgtPartSUID = ODGetWeakSURefProp(ev,su,
                                        kODPropTargetPart,
                                        kODWeakStorageUnitRef);

    _fTracked = ODGetBooleanProp(ev,su,
                                 kODPropLinkSourceTrackMode,
                                 kODBoolean);

    //---<6>
     char            hostName[256];
     strcpy(hostName, getenv("HOSTNAME"));

       DisposeITextStruct(_fTrgtConnData.HostName);
       _fTrgtConnData.HostName.format = kODIBMCodePageText;
       _fTrgtConnData.HostName.text._buffer = kODNULL;
       SetITextCString(&_fTrgtConnData.HostName, hostName);
       SetITextLangCode(&_fTrgtConnData.HostName, 0);
       SetITextScriptCode(&_fTrgtConnData.HostName, 0);

       DisposeITextStruct(_fTrgtConnData.DocumentName);
       _fTrgtConnData.DocumentName.format = kODIBMCodePageText;
       _fTrgtConnData.DocumentName.text._buffer = kODNULL;
       SetITextCString(&_fTrgtConnData.DocumentName, pathName);
       SetITextLangCode(&_fTrgtConnData.DocumentName, 0);
       SetITextScriptCode(&_fTrgtConnData.DocumentName, 0);


     //---<7>
     if (su->Exists(ev, kODPropCloneOfALink, kODNULL, 0))
       {
       //---<7.1>
       su->Focus(ev, kODPropCloneOfALink, 0, kODNULL, 0, 0);
       su->Remove(ev);

       //---<7.2>
       if (linkSourceSUID == 0)
         {
         ODBoolean IsNowLocal;  // is the cloned link target local to the LS?
         TRY
           //---<7.2.1>
           _fLinkSrc->AddLinkTarget(ev,
                                    _fLinkID,
                                    _fPID,
                                    &_fTrgtConnData,
                                    somSelf,
                                    &linkSourceSUID,
                                    &_fConnID,
                                    &IsNowLocal,
                                    &_fTracked);

           //---<7.2.2.>
           if (IsNowLocal) {
             _fIsLocal = kODTrue;
             if (((SOMDObject*)_fLinkSrc)->is_proxy(ev))
             {
               SOMD_ObjectMgr->somdReleaseObject(ev, _fLinkSrc);
             }

             _fLinkSrc = (IODLinkTargetToLinkSource*)
                         su->GetDraft(ev)->AcquireLinkSource(ev, linkSourceSUID);

             } else /* IsNowLocal false */
               _fIsLocal = kODFalse;
         CATCH_ALL
             if (((SOMDObject*)_fLinkSrc)->is_proxy(ev))
             {
               SOMD_ObjectMgr->somdReleaseObject(ev, _fLinkSrc);
             }
             _fLinkSrc = kODNULL;
         ENDTRY
         }  /* end if linkSourceSUID == 0 */
       }  /* end if this is a clone of a link */
}

/*
 *=============================================================================
 * Called by LinkSource to inform that source Part has broken the link.
 *
 * <1>  Release the old LinkTargetToLinkSource proxy if LinkSource is remote.
 * <2>  Set fLinkSrc to NULL so Part will get an broken link exception if
 *      a method invoked on it.
 *=============================================================================
 */

SOM_Scope void
SOMLINK RemoveLink(IODLinkTarget *somSelf,
                   Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","RemoveLink");

    //---<1>
    if (_fLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
    {
      if (!_fIsLocal)
      {
        if (((SOMDObject*)_fLinkSrc)->is_proxy(ev))
        {
          SOMD_ObjectMgr->somdReleaseObject(ev, _fLinkSrc);
        }
        SOMFree(_fASName);
        _fASName = (ODISOStr)kODNULL;
      }
    }

    //---<2>
    _fLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;
}

/*
 *=============================================================================
 * Called by LinkSource to reset the reference to it when it is closed
 *   or reopened.
 * LinkTarget gets this call only when it is registered with the
 *   LinkSource.
 *
 * <1>  Release the old LinkTargetToLinkSource proxy if LinkSource is remote.
 * <2>  Save the new LinkTargetToLinkSource proxy.
 *=============================================================================
 */
//SOM_Scope void
//SOMLINK ResetLinkSource(IODLinkTarget *somSelf,
//                        Environment *ev,
//                        IODLinkTargetToLinkSource* linkSrc)
//{
//    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
//    IODLinkTargetMethodDebug("IODLinkTarget","ResetLinkSource");
//
//    //---<1>
//    if (_fLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
//    {
//      if (!_fIsLocal)
//      {
//        SOMD_ObjectMgr->somdReleaseObject(ev, _fLinkSrc);
//      }
//    }
//
//    //---<2>
//    _fLinkSrc = linkSrc;
//}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK somInit(IODLinkTarget *somSelf)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","somInit");

    IODLinkTarget_parent_ODLink_somInit(somSelf);

    _fConnID = 0;
    _fIsLocal = kODFalse;
    _fLinkID = 0;
    _fLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;
    _fNumOfRegParts = 0;

    _fTrgtConnData.HostName.format = kODIBMCodePageText;
    _fTrgtConnData.HostName.text._buffer = kODNULL;
    SetITextCString(&_fTrgtConnData.HostName, "");
    SetITextLangCode(&_fTrgtConnData.HostName, 0);
    SetITextScriptCode(&_fTrgtConnData.HostName, 0);

    _fTrgtConnData.DocumentName.format = kODIBMCodePageText;
    _fTrgtConnData.DocumentName.text._buffer = kODNULL;
    SetITextCString(&_fTrgtConnData.DocumentName, "");
    SetITextLangCode(&_fTrgtConnData.DocumentName, 0);
    SetITextScriptCode(&_fTrgtConnData.DocumentName, 0);

    _fTrgtConnData.LinkDesc.format = kODIBMCodePageText;
    _fTrgtConnData.LinkDesc.text._buffer = kODNULL;
    SetITextCString(&_fTrgtConnData.LinkDesc, "");
    SetITextLangCode(&_fTrgtConnData.LinkDesc, 0);
    SetITextScriptCode(&_fTrgtConnData.LinkDesc, 0);
    _fTrgtPartSUID = 0;
    _fTracked = kODFalse;
    _fPID = _getpid();
    _fASName = (ODISOStr)kODNULL;
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK somUninit(IODLinkTarget *somSelf)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","somUninit");

    IODLinkTarget_parent_ODLink_somUninit(somSelf);
}

/*
 *=============================================================================
 * <1>  Invoke parent's Externalize method.
 * <2>  Externalize the following variables to the storage.
 *        ODLinkConnectionID fConnID;
 *        ODBoolean       fIsLocal;
 *        ODLinkID        fLinkID;
 *        ODISOStr        fASName;
 * <3>  LinkTarget is local:
 *      Write the weak SU reference of LinkTargetToLinkSource to the storage.
 * <4>  LinkTarget is remote: (nothing)
 * <5>  Externalize the following variables to the storage:
 *        ODLinkConnectionData fTrgtPartDesc;
 *        ODStorageUnitID    fTrgtPartSUID;
 *        ODBoolean          fTracked;
 *=============================================================================
 */

SOM_Scope void
SOMLINK Externalize(IODLinkTarget *somSelf,
                    Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","Externalize");

    ODISOStr      externalizedID = (ODISOStr)kODNULL;
    ODStorageUnit *su = _GetStorageUnit(ev);

    //---<1>
    IODLinkTarget_parent_ODLink_Externalize(somSelf, ev);

    //---<2>
    ODSetULongProp(ev, su, kODPropConnectionID, kODULong, _fConnID);
    ODSetBooleanProp(ev, su, kODPropLinkLocality, kODBoolean, _fIsLocal);
    ODSetULongProp(ev, su, kODPropLinkID, kODULong, _fLinkID);
    ODSetISOStrProp(ev, su,
                    kODPropAvailabilityServerName,
                    kODISOStr,
                    _fASName);

    if (_fIsLocal)
    {
      //---<3>
      ODSetWeakSURefProp(ev, su,
                         kODPropLinkTargetToLinkSource,
                         kODWeakStorageUnitRef,
                         ((IODLinkSource*)_fLinkSrc)->GetID(ev));
    }
    else
    {
      //---<4>
    }

    //---<5>
    SetPartDescriptionProp(ev,
                           su,
                           kODPropPartDescription,
                           kODPartDescription,
                           _fTrgtConnData);

    if (_fTrgtPartSUID)
      ODSetWeakSURefProp(ev, su,
                         kODPropTargetPart,
                         kODWeakStorageUnitRef,
                         _fTrgtPartSUID);

    ODSetBooleanProp(ev, su, kODPropLinkSourceTrackMode, kODBoolean, _fTracked);
}
/*
 *=============================================================================
 * <1>  Make sure this object is not already cloned in the toSU.
 * <2>  Invoke parent's CloneInto method.
 * <3>  Clone the following variables into toSU:
 *        ODLinkConnectionID fConnID;
 *        ODBoolean       fIsLocal;
 *        ODLinkID        fLinkID;
 *        ODISOStr        fASName;
 * <4>  LinkTarget is local:
 *      Ask the Draft to weak clone the LinkTargetToLinkSource.
 *      Clone the returned SUID into toSU.
 * <5>  LinkTarget is remote:(nothing)
 * <6>  Clone ODLinkConnectionData fTrgtPartDesc into toSU.
 * <7>  Ask the Draft to weak clone the target Part.
 *      Clone the returned SUID into toSU.
 * <8>  Clone ODBoolean fTracked into toSU.
 *=============================================================================
 */

SOM_Scope void
SOMLINK CloneInto(IODLinkTarget *somSelf,
                  Environment *ev,
                  ODDraftKey key,
                  ODStorageUnit* toSU,
                  ODFrame* scopeFrame)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","CloneInto");

    ODDraft* draft = _GetStorageUnit(ev)->GetDraft(ev);
    ODID     srcSUID;
    ODID     trgtPartSUID;

    //---<1>
    if (toSU->Exists(ev, kODPropConnectionID, kODULong, 0)) return;

    //---<2>
    IODLinkTarget_parent_ODLink_CloneInto(somSelf,
                                          ev,
                                          key,
                                          toSU,
                                          scopeFrame);

    //---<3>
    ODSetULongProp(ev, toSU, kODPropConnectionID, kODULong, _fConnID);
    ODSetBooleanProp(ev, toSU, kODPropLinkLocality, kODBoolean, _fIsLocal);
    ODSetULongProp(ev, toSU, kODPropLinkID, kODULong, _fLinkID);
    ODSetISOStrProp(ev, toSU,
                    kODPropAvailabilityServerName,
                    kODISOStr,
                    _fASName);

    if (_fIsLocal)
    {
      //---<4>
      srcSUID = draft->WeakClone(ev,
                                 key,
                                 ((IODLinkSource*)_fLinkSrc)->GetID(ev),
                                 0,
                                 (scopeFrame ? scopeFrame->GetID(ev) : 0));
      ODSetWeakSURefProp(ev, toSU,
                         kODPropLinkTargetToLinkSource,
                         kODWeakStorageUnitRef,
                         srcSUID);
    }
    else
    {
      //---<5>
      ODSetISOStrProp(ev, toSU,
                      kODPropAvailabilityServerName,
                      kODISOStr,
                      _fASName);
    }

    //---<6>
    SetPartDescriptionProp(ev,
                           toSU,
                           kODPropPartDescription,
                           kODPartDescription,
                           _fTrgtConnData);

    //---<7>
    trgtPartSUID = draft->WeakClone(ev,
                                    key,
                                    _fTrgtPartSUID,
                                    0,
                                    (scopeFrame ? scopeFrame->GetID(ev) : 0));
    ODSetWeakSURefProp(ev, toSU,
                       kODPropTargetPart,
                       kODWeakStorageUnitRef,
                       trgtPartSUID);

    //---<8>
    ODSetBooleanProp(ev, toSU,
                     kODPropLinkSourceTrackMode,
                     kODBoolean,
                     _fTracked);
}

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke ForwardGetUpdateID method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope ODUpdateID
SOMLINK GetUpdateID(IODLinkTarget *somSelf,
                    Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","GetUpdateID");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return kODUnknownUpdate;
    }

    TRY
      //---<2>
      ODUpdateID id = _fLinkSrc->ForwardGetUpdateID(ev, _fLinkID);
      return id;
    CATCH_ALL
      ODSetSOMException(ev, kODErrBrokenLink);
      return kODUnknownUpdate;
    ENDTRY  }

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke ForwardGetChangeTime method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope ODTime
SOMLINK GetChangeTime(IODLinkTarget *somSelf,
                      Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","GetChangeTime");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return (ODTime)0;
    }

    TRY
      //---<2>
      ODTime t = _fLinkSrc->ForwardGetChangeTime(ev, _fLinkID);
      return t;
    CATCH_ALL
      ODSetSOMException(ev, kODErrBrokenLink);
      return (ODTime)0;
    ENDTRY     }

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke ForwardGetContentStorageUnit method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope ODStorageUnit*
SOMLINK GetContentStorageUnit(IODLinkTarget *somSelf,
                              Environment *ev,
                              ODLinkKey key)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","GetContentStorageUnit");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return (ODStorageUnit*)kODNULL;
    }

    TRY
      //---<2>
      ODStorageUnit* su = _fLinkSrc->ForwardGetContentStorageUnit(ev,
                                                                  _fLinkID,
                                                                  key);
      return su;
    CATCH_ALL
      ODSetSOMException(ev, kODErrBrokenLink);
      return (ODStorageUnit*)kODNULL;
    ENDTRY          }

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODLinkSource*
SOMLINK GetLinkSource(IODLinkTarget *somSelf,
                      Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","GetLinkSource");

    return (ODLinkSource*)_fLinkSrc;
}

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke ForwardLock method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope ODBoolean
SOMLINK Lock(IODLinkTarget *somSelf,
             Environment *ev,
             ODULong wait,
             ODLinkKey* key)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","Lock");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return kODFalse;
    }

    TRY
      //---<2>
      ODBoolean rc = _fLinkSrc->ForwardLock(ev, _fLinkID, wait, key);
      return rc;
    CATCH_ALL
      ODSetSOMException(ev, kODErrBrokenLink);
      return kODFalse;
    ENDTRY       }

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke parent's RegisterDependent method.
 * <3>  Increase counter of registered parts.
 * <4>  Set the fIsRegistered instant variable to true.
 * <5>  If counter of registered parts change from 0 to 1 then register with
 *      LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope void
SOMLINK RegisterDependent(IODLinkTarget *somSelf,
                          Environment *ev,
                          ODPart* clientPart,
                          ODUpdateID id)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","RegisterDependent");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return;
    }

    //---<2>
    IODLinkTarget_parent_ODLink_RegisterDependent(somSelf,
                                                  ev,
                                                  clientPart,
                                                  id);

    if (ODGetSOMException(ev) == kODNoError)
    {
      //---<3>
      _fNumOfRegParts++;

      //---<4>
      _fIsRegistered = kODTrue;

      //---<5>
      if (   (_fNumOfRegParts == 1)
           &&(_fLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
         )
      {
        TRY
        if (_fTracked)
        {
          _fLinkSrc->RegisterTrackedLinkTarget(ev,
                                               _fLinkID,
                                               _fConnID,
                                               _fIsLocal);
        }// end of _ftracked
        else
        {
          _fLinkSrc->RegisterUntrackedLinkTarget(ev,
                                                 _fLinkID,
                                                 (IODLinkTarget*)somSelf,
                                                 &_fTrgtConnData,
                                                 _fIsLocal,
                                                 &_fConnID);
        }// end of !_fTracked
        CATCH_ALL
        _fNumOfRegParts = 0;
        _fIsRegistered = kODFalse;
        ODSetSOMException(ev, kODErrBrokenLink);
        return;
      ENDTRY
      }
    }
}

/*
 *=============================================================================
 * <1>  Invoke parent's Release method.
 * <2>  If LinkTarget's reference count goes down to zero inform the Draft
 *      to release the LinkTarget.
 *=============================================================================
 */

SOM_Scope void
SOMLINK Release(IODLinkTarget *somSelf,
                Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","Release");

    IODLinkTarget_parent_ODLink_Release(somSelf, ev);

    if (_GetRefCount(ev) == 0)
    {
      _GetStorageUnit(ev)->GetDraft(ev)->ReleaseLink(ev, somSelf);
    }
}

/*
 *=============================================================================
 * <1>  Invoke parent's ReleaseAll method.
 *=============================================================================
 */

SOM_Scope void
SOMLINK ReleaseAll(IODLinkTarget *somSelf,
                   Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","ReleaseAll");

    //---<1>
    IODLinkTarget_parent_ODLink_ReleaseAll(somSelf, ev);

    // <dmy> Make sure that the link target is unregistered
    // ( _fNumOfRegParts == 0 ).  Otherwise unregister it.
    if (   (_fNumOfRegParts != 0)
         &&(!_fIsLocal)
         &&(_fLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
        )
    {
      TRY
      _fLinkSrc->UnregisterLinkTarget(somGetGlobalEnvironment(), _fLinkID, _fConnID, _fIsLocal);
      CATCH_ALL
      ENDTRY
    }

    //---<2>
    if (_fLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
    {
      if (!_fIsLocal)
      {
        if (((SOMDObject*)_fLinkSrc)->is_proxy(ev))
        {
          SOMD_ObjectMgr->somdReleaseObject(ev, _fLinkSrc);
        }
      }
      else {
        TRY
        ((IODLinkSource*)_fLinkSrc)->Release(ev);
        CATCH_ALL
        ENDTRY
      }
    }
    _fLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;

    //---<3>
    _fConnID = 0;
    _fIsLocal = kODFalse;
    _fLinkID = 0;
    _fNumOfRegParts = 0;
    _fTrgtPartSUID = 0;
    _fTracked = kODFalse;
}


#ifndef _PLATFORM_OS2_
/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke ShowLinkDestinationInfo of BaseLink for now.
 *=============================================================================
 */

SOM_Scope ODBoolean
SOMLINK ShowLinkDestinationInfo(IODLinkTarget *somSelf,
                                Environment *ev,
                                ODFacet* facet,
                                ODLinkInfo* info,
                                ODBoolean changesAllowed,
                                ODLinkInfoResult* infoResult)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","ShowLinkDestinationInfo");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return kODFalse;
    }

    TRY
    //---<2>
    ODBoolean rc = IODLinkTarget_parent_ODLink_ShowLinkDestinationInfo(
                       somSelf,
                       ev,
                       facet,
                       info,
                       changesAllowed,
                       infoResult);
    return rc;
    CATCH_ALL
      ODSetSOMException(ev, kODErrBrokenLink);
      return kODFalse;
    ENDTRY
    }
#endif

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke ShowSourceContentFor method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope void
SOMLINK ShowSourceContent(IODLinkTarget *somSelf,
                          Environment *ev)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","ShowSourceContent");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return;
    }


    //---<2>
     SOM_TRY
     _fLinkSrc->ShowSourceContentFor(ev, _fLinkID, GetCStringFromIText(&_fTrgtConnData.HostName));
     SOM_CATCH_ALL
     if (ODGetSOMException(ev) != kODErrSourceDocNotOpen)
       {
        ODSetSOMException(ev, kODErrBrokenLink);
       }
     return;
     SOM_ENDTRY
}

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke ForwardUnlock method of LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope void
SOMLINK Unlock(IODLinkTarget *somSelf,
               Environment *ev,
               ODLinkKey key)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","Unlock");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return;
    }

    //---<2>
   TRY
    _fLinkSrc->ForwardUnlock(ev, _fLinkID, key, _fIsLocal);
  CATCH_ALL
    ODSetSOMException(ev, kODErrBrokenLink);
    return;
  ENDTRY
  }

/*
 *=============================================================================
 * <1>  Make sure link is not broken.
 * <2>  Invoke parent's UnregisterDependent method.
 * <3>  Decrease counter of registered parts.
 * <4>  If counter of registered parts change from 1 to 0 then do <5> and <6>
 * <5>  Set the fIsRegistered instant variable to false.
 * <6>  Unregister with LinkTargetToLinkSource.
 *=============================================================================
 */

SOM_Scope void
SOMLINK UnregisterDependent(IODLinkTarget *somSelf,
                            Environment *ev,
                            ODPart* clientPart)
{
    IODLinkTargetData *somThis = IODLinkTargetGetData(somSelf);
    IODLinkTargetMethodDebug("IODLinkTarget","UnregisterDependent");

    //---<1>
    if (_fLinkSrc == (IODLinkTargetToLinkSource*)kODNULL)
    {
      ODSetSOMException(ev, kODErrBrokenLink);
      return;
    }

    //---<2>
    IODLinkTarget_parent_ODLink_UnregisterDependent(somSelf, ev, clientPart);

    if (ODGetSOMException(ev) == kODNoError)
    {
      //---<3>
      _fNumOfRegParts--;

      //---<4>
      if (_fNumOfRegParts == 0)
      {
        //---<5>
        _fIsRegistered = kODFalse;
        //---<6>
        TRY
        _fLinkSrc->UnregisterLinkTarget(ev, _fLinkID, _fConnID, _fIsLocal);
        CATCH_ALL
        ENDTRY
        }
    }
}

#endif

