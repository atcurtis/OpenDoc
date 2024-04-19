/* @(#)Z 1.11 os2/src/storage/ilnksrvc.cpp, oddataxfer, od96os2, odos29647a 96/11/18 05:18:55 (96/11/15 14:38:50) */
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


#ifndef SOM_Module_ilnksrvc_Source
#define SOM_Module_ilnksrvc_Source
#endif
#define IODLinkService_Class_Source

#define METHOD_MACROS
#define VARIABLE_MACROS

#define INCL_DOSERRORS
#define INCL_ERROR_H
#include <bseerr.h>

//lsdlgs includes
#define INCL_WIN
#define INCL_DOS
#include <os2.h>
#include <odres.h>       //For Link Servic dialog's resource id's
#include <ODPagtun.h>
#define INCL_DOSMODULEMGR
#include <bsedos.h>
//#include <pmwin.h>

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _PLFMDEF_
#include "plfmdef.h"
#endif

#ifndef _LINKDLGS_
#include "Linkdlgs.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _LINKDEFS_
#include "LinkDefs.h"
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_Facet_xh
#include "Facet.xh"
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

//#ifndef _INFOUTIL_
//#include "infoutil.h"
//#endif


#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif
//end lsdlgs includes
#include <ISOStr.h>
#include <stdtypes.xh>
#include <IODDefs.xh>
#include <Except.h>
#include <AvsShell.h>
#include <barray.h>
#include <IAvlSvr.xh>
#include <cmdraft.xh>
#include <iciditr.xh>
#ifdef WHATISTHIS
#include <lsdlgs.h>
#endif // WHATISTHIS

#include <io.h>

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


#ifndef _OPENHASH_
#include "OpenHash.h"
#endif

#ifndef _STDTYPIO_
  #include <StdTypIO.h>
#endif

#ifndef _STORUTIL_
  #include <storutil.h>
#endif

#ifndef SOM_ODStorageUnit_xh
  #include <StorageU.xh>
#endif

#ifndef SOM_ODDraft_xh
  #include <Draft.xh>
#endif

#ifndef SOM_ODPart_xh
  #include <part.xh>
#endif

#ifndef SOM_IODLinkSource_xh
  #include <ILinkSrc.xh>
#endif

#ifndef SOM_IODLinkTargetToLinkSource_xh
  #include <ILTr2LSr.xh>
#endif

#include <ilnktrgt.xh>

#include <Builtin.h>
#include <somd.xh>
#include <servmgr.xh>
#include "ILnkSrvc.xih"

#ifndef SOM_IODMutex_xh
  #include <IODMutex.xh>
#endif

#include <cmlksitr.xh>
#include <cmlkitr.xh>

#include <lmgrdefs.h>

//==============================================================================
// Constants
//==============================================================================
#define kODLinkIDLen         4
#define kODULongLen          4
#define kODStorageUnitRefLen 4
const ODULong kODInitialNumEntries = 8;
#define FILE_LENGTH 256


typedef struct
{
    ODLinkID           linkID;
    ODStorageUnitRef   linkSrcSURef;
    IODLinkSource*     linkSrc;
    ODBoolean          isOpened;
    ODBoolean          isSaved;
    ODStorageUnitRef   srcPartSURef;
} LinkSrcInfo;
//# Structure of an entry in the fLinkSrcInfos hash table where the key is
//#   the link ID

typedef struct
{
  ODID               trgtID;
  IODLinkTarget*     linkTrgt;
  ODStorageUnitRef   linkTrgtSURef;
  ODStorageUnitRef   trgtPartSURef;
} LinkTrgtInfo;
//# Structure of an entry in the fLinkTrgtInfos hash table where the key is
//#   the link target

char* GetPathName (char* docName)
{
    char   drive[3] = "::";
    char*  hostName;
    char*  pathName = (char*)kODNULL;
    APIRET rc = NO_ERROR;
    char*  tmp;
    ULONG  ulOrdinal = 0;

    if (strstr(docName, "LOCALHOST") == docName)
    {
      pathName = (char*)SOMMalloc(strlen(docName));
      strcpy(pathName, docName+10);
      return pathName;
    }

    if (hostName = getenv("HOSTNAME"))
    {
      if (strstr(docName, strupr(hostName)) == docName)
      {
        pathName = (char*)SOMMalloc(strlen(docName));
        strcpy(pathName, docName+strlen(hostName)+1);
        return pathName;
      }
    }

    for (char c = 'C'; c <= 'Z'; c++)
    {
      drive[0] = c;

      {
        BYTE        fsqBuffer[sizeof(FSQBUFFER2) + (3 * CCHMAXPATH)] = {0};
        PFSQBUFFER2 pfsqBuffer = (PFSQBUFFER2)fsqBuffer;
        ULONG       cbBuffer = sizeof(fsqBuffer);
        PBYTE       prgFSAData = NULL;
        PBYTE       pszFSDName = NULL;

        rc = DosQueryFSAttach(drive,
                              ulOrdinal,
                              FSAIL_QUERYNAME,
                              pfsqBuffer,
                              &cbBuffer);

        if (rc == NO_ERROR)
        {
          pszFSDName = (BYTE*)(pfsqBuffer->szName + pfsqBuffer->cbName + 1);
          prgFSAData = strupr(pszFSDName + pfsqBuffer->cbFSDName + 1);

          // Only consider NFS remote file
          if ((pfsqBuffer->iType == FSAT_REMOTEDRV) &&
              (strcmp(pszFSDName, "NFS") == 0))
          {
            if (strstr(docName, prgFSAData) == docName)
            {
              pathName = (char*)malloc(strlen(docName) + 3);
              pathName[0] = c;
              pathName[1] = ':';
              pathName[2] = '\0';
              strcat(pathName, docName+strlen(prgFSAData));
              return pathName;
            }
          }
        }
      }
    }

    return pathName;
}

/*
 *=============================================================================
 * Local function for cleaning up LinkService Properties.
 *
 * <1> Clean Up instance variables.
 * <2> Release the local _fAvlServer object(deletes the proxy, but not the
 *                                           remote AvlServer...)
 *=============================================================================
 */

SOM_Scope void
SOMLINK CleanupLinkServiceProperties(IODLinkService *somSelf, Environment *ev)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","CleanUpLinkServicProperties");
  //---<1>
  ODLinkID linkID;
  LinkSrcInfo *linkSrcInfo = kODNULL;
  if (_fLinkSrcInfos != (OpenHashTable*)kODNULL)
  {
   OpenHashTableIterator iter(_fLinkSrcInfos);
   for (iter.First(&linkID, &linkSrcInfo);
        iter.IsNotComplete();
        iter.Next(&linkID, &linkSrcInfo))
    {
     (linkSrcInfo->linkSrc)->Release(ev);
      delete linkSrcInfo;
    }

    delete _fLinkSrcInfos;
    _fLinkSrcInfos = (OpenHashTable*)kODNULL;
   }
   ODLinkConnectionID    trgtID;
   LinkTrgtInfo *linkTrgtInfo = kODNULL;
   if (_fLinkTrgtInfos != (OpenHashTable*)kODNULL)
   {
    OpenHashTableIterator iter(_fLinkTrgtInfos);
    for (iter.First(&trgtID, &linkTrgtInfo);
         iter.IsNotComplete();
         iter.Next(&linkID, &linkTrgtInfo))
     {
       delete linkTrgtInfo;
     }

     delete _fLinkTrgtInfos;
     _fLinkTrgtInfos = (OpenHashTable*)kODNULL;
    }
   SOMFree(_fDocPathName);
   _fDocPathName  = kODNULL;
   SOMFree(_fASName);
   _fASName       = kODNULL;
   _fDraftOpened  = kODFalse;
   _fDraft = kODNULL;
   _fDraftOpened = kODFalse;
   _fDocumentID   = kODNULL;
   //---<2>
   if (_fAvlSvr != (IODAvailServer*)kODNULL)
   {
     if (((SOMDObject*)_fAvlSvr)->is_proxy(ev))
     {
       SOMD_ObjectMgr->somdReleaseObject(ev, (SOMDObject*)_fAvlSvr);
     }
     else
     {
       _interrupt(3);
     }
   }
   _fAvlSvr       = kODNULL;
}
/*
 *=============================================================================
 * Local function for contacting the availability server
 *
 * <1>  If the availability server already contacted, then nothing to do.
 * <2>  Get a pointer to the Availability Server from its name using
 *      somdFindServerByName. Store it in the _fAvlSvr field.
 * <3>  If this is a moved document, call DocMoved
 * <4A> If this is a new document (one with no ASName property, or DocumentID 0)
 *     <4.A.1.> Call Register Document on the Availability server; get back the
 *              document id.
 * <4B> If this is an old document
 *     <4.B.1.> Call the Availability Server DocOpened.
 *     <4.B.2.> If DocOpened failed - check return code and if it is Invalid
 *          doc name - then this is an original of a copy/move.
 *          Bring up a dialogue to select the dominant document.
 *     <4.B.2.1.> If move, call DocMoved, and
 *                Call the Availability Server DocOpened again.
 *                and it should work; Raise an exception if it doesn't.
 *     <4.B.2.2.> If copy, call the availability server Register,
 *                and empty out the table of source links
 *=============================================================================
*/

void ContactAvailabilityServer (Environment *ev,
                                IODLinkService *somSelf,
                                IODLinkServiceData *somThis,
                                ODMoveCopyResult *mcResult /* non-null if move-copy dialog has taken place */
                                )
{
    SOMDServer* server;
    CHAR        LoadError[CCHMAXPATH];
    CHAR        pszTemp[CCHMAXPATH];
    CHAR        pszTemp2[CCHMAXPATH];
    HAB         hab;
    HMODULE     hmodDLL;

    HPOINTER waitptr, arrowptr;
    waitptr  = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE);
    arrowptr = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE);
    WinSetPointer(HWND_DESKTOP,waitptr);

// #ifdef OS2LinkChanges
    _fDraft->
      GetDocument(ev)->
        GetContainer(ev)->
          GetStorageSystem(ev)->
            GetSession(ev)->
              StartServer(ev, (char*)kODNULL);
// #endif // OS2LinkChanges

    // 1.
    if (_fAvlSvr != kODNULL) return;

    SOMDServerMgr* servMgr;
    servMgr = new SOMDServerMgr;
    servMgr->somdStartServer(ev, _fASName);

    if (ev->_major != NO_EXCEPTION) {
       DosLoadModule(LoadError,sizeof(LoadError),"ODRes",&hmodDLL);
       WinLoadString(hab,hmodDLL,IDS_NOSERVER,CCHMAXPATH,pszTemp);
       WinLoadString(hab,hmodDLL,IDS_ERROR,CCHMAXPATH,pszTemp2);
       WinMessageBox(HWND_DESKTOP,
                     HWND_DESKTOP,
                     pszTemp,
                     pszTemp2,
                     0L,
                     MB_OK | MB_ERROR);
    }

    // 2.
    server = SOMD_ObjectMgr->somdFindServerByName(ev, _fASName);

    if(server == NULL)                   //@SK [124619]
    {                                              //@SK [124619]
      somPrintf("\n Error: Cannot find Availability Server.\n");//@SK [124619]
      WinSetPointer(HWND_DESKTOP,arrowptr);
      DosExit(EXIT_PROCESS, 1);                      //@SK [124619]
    }

    if(server)
    {
      _fAvlSvr = (IODAvailServer*)(void*)server->
                                           somdCreateObj(ev,
                                                         "IODAvailServer",
                                                         "");
    }

    if (ODGetSOMException(ev) != kODNoError)
    {
      _fAvlSvr = kODNULL;
      THROW(kODErrInvalidAvlSvr);
    }

    // 3.
    if ((mcResult != kODNULL) && (mcResult->FileMoved))
    {
      TRY
        somSelf->DraftMoved(ev,
                            _fDraft,
                            mcResult->fOldDocPathName,
                            mcResult->fNewDocPathName);
      CATCH_ALL
        ODMoveCopyResult mcDialog;
        mcDialog.fNewDocPathName = _fDocPathName;
        mcDialog.fOldDocPathName = mcResult->fOldDocPathName;
        do
        {
          ShowMoveCopyDialog(&mcDialog);
          if (mcDialog.FileCopied)
          {
            _fDocumentID = _fAvlSvr->
                             RegisterDoc(ev,
                                         _fDocPathName,
                                         (IODAvailServerToLinkService*)somSelf);
            THROW_IF_NULL((void *)_fDocumentID);
            delete _fLinkSrcInfos;
            _fLinkSrcInfos = new OpenHashTable;
            _fLinkSrcInfos->Initialize(kODInitialNumEntries,
                                       sizeof(ODStorageUnitID),
                                       sizeof(LinkSrcInfo*),
                                       kODFalse);
            delete _fLinkTrgtInfos;
            _fLinkTrgtInfos = new OpenHashTable;
            _fLinkTrgtInfos->Initialize(kODInitialNumEntries,
                                        sizeof(ODStorageUnitID),
                                        sizeof(LinkTrgtInfo*),
                                        kODFalse);
            _fIsNew = kODTrue;
            return;
          }
        } while (mcDialog.FileMoved);
      ENDTRY
    }

    // 4.
    if (_fIsNew || _fDocumentID == 0)
    {
      //---<4.A.1.>
      _fDocumentID = _fAvlSvr->
                       RegisterDoc(ev,
                                   _fDocPathName,
                                   (IODAvailServerToLinkService*)somSelf);
      THROW_IF_NULL((void *)_fDocumentID);
    }
    else
    {
      ODISOStr oldDocPathName;
      ODBoolean success;
    SOM_TRY
      //---<4.B.1.>
      success = _fAvlSvr->
                             DocOpened(ev,
                                       _fDocumentID,
                                       _fDocPathName,
                                       (IODAvailServerToLinkService*)somSelf,
                                       &oldDocPathName);
    SOM_CATCH_ALL
      //---<4.B.2.>
      if (!success)
      {
        if (ODGetSOMException(ev) == kODErrInvalidDocPathName)
        {
          ODSetSOMException(ev, kODNoError);
          ODMoveCopyResult mcDialog;
          mcDialog.fNewDocPathName = _fDocPathName;
          mcDialog.fOldDocPathName = oldDocPathName;
          ShowMoveCopyDialog(&mcDialog);
          if (mcDialog.FileMoved == kODTrue)
          {
          TRY
            //---<4.B.2.1.>
            somSelf->DraftMoved(ev,
                                _fDraft,
                                mcDialog.fOldDocPathName,
                                mcDialog.fNewDocPathName);
            if (oldDocPathName != kODNULL)
            {
              SOMFree(oldDocPathName);
              oldDocPathName = kODNULL;
            }
            success = _fAvlSvr->
                        DocOpened(ev,
                                  _fDocumentID,
                                  _fDocPathName,
                                  (IODAvailServerToLinkService*)somSelf,
                                   &oldDocPathName);
            if (oldDocPathName != kODNULL)
            {
              SOMFree(oldDocPathName);
              oldDocPathName = kODNULL;
            }
          CATCH_ALL
            if(!success)
            {
              THROW(kODErrDocOpenedFailed);
            }
          ENDTRY
          } /* if moved */
          else
          {
            //---<4.B.2.2.>
            _fDocumentID = _fAvlSvr->
                             RegisterDoc(ev,
                                         _fDocPathName,
                                         (IODAvailServerToLinkService*)somSelf);
            THROW_IF_NULL((void *)_fDocumentID);
            delete _fLinkSrcInfos;
            _fLinkSrcInfos = new OpenHashTable;
            _fLinkSrcInfos->Initialize(kODInitialNumEntries,
                                       sizeof(ODStorageUnitID),
                                       sizeof(LinkSrcInfo*),
                                       kODFalse);
            delete _fLinkTrgtInfos;
            _fLinkTrgtInfos = new OpenHashTable;
            _fLinkTrgtInfos->Initialize(kODInitialNumEntries,
                                        sizeof(ODStorageUnitID),
                                        sizeof(LinkTrgtInfo*),
                                        kODFalse);
            _fIsNew = kODTrue;
          }/* else */
        } /* if invalidDocId */
      } /* if not success */
    SOM_ENDTRY
      if  (oldDocPathName != kODNULL)
      {
        SOMFree(oldDocPathName);
      }
    } /* if not IsNew */

    WinSetPointer(HWND_DESKTOP,arrowptr);
}

/*
 *=============================================================================
 * Local function for writing an ODISOStr to a storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Save old size of the focused value in oldSize.
 * <3>  Write the length of the string, followed by the string itself.
 *       Increase newSize by the size of the length counter + lenght bytes.
 * <4>  Delete unused space left from previous write.
 *=============================================================================
*/

void SetISOStrProp (Environment *ev,
                             ODStorageUnit *su,
                             ODPropertyName prop,
                             ODISOStr myISOStr)
{
    ODULong                      newSize;
    ODULong                      oldSize;
    ODULong                      strLength;/* length of string written to SU */
                                           /* is string length + its null    */
    //---<1>
    ODSUForceFocus(ev, su, prop, kODISOStr);

    //---<2>
    oldSize = su->GetSize(ev);

    //---<3>
    strLength = strlen(myISOStr) + 1;
    StorageUnitSetValue(su, ev, kODULongLen, (ODValue)&strLength);
    StorageUnitSetValue(su,
                        ev,
                        strLength,
                        (ODValue)myISOStr);
    newSize = (kODULongLen + strLength);

    //---<4>
    if (oldSize > newSize)
    {
      su->DeleteValue(ev, oldSize-newSize);
    }

} /* end SetISOStrProp */

/*
 *=============================================================================
 * Local function for reading an ODISOStr from a storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Read in the string.
 *=============================================================================
*/

void GetISOStrProp (Environment *ev,
                    ODStorageUnit *su,
                    ODPropertyName prop,
                    ODISOStr *myISOStr)
{
    ODULong                strLength;/* length of string written to SU */
                                     /* is string length + its null    */
    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, kODISOStr))
    {
        //---<2>
        StorageUnitGetValue(su, ev, kODULongLen, (ODValue)&strLength);
        *myISOStr = (ODISOStr)SOMMalloc(strLength);
        StorageUnitGetValue(su,
                            ev,
                            strLength,
                            (ODValue)*myISOStr);
   }
} /* end GetISOStrProp */

/*
 *=============================================================================
 * Local function for writing an ODULong to a storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Write the ULong to storage unit.
 *=============================================================================
*/

void SetULongProp (Environment *ev,
                   ODStorageUnit *su,
                   ODPropertyName prop,
                   ODULong  &myULong)
{
    //---<1>
    ODSUForceFocus(ev, su, prop, kODULong);

    //---<2>
    StorageUnitSetValue(su, ev, kODULongLen, (ODValue)&myULong);

} /* end SetISOStrProp */

/*
 *=============================================================================
 * Local function for reading an ODULong from a storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Read in the ULong.
 *=============================================================================
*/

void GetULongProp (Environment *ev,
                    ODStorageUnit *su,
                    ODPropertyName prop,
                    ODULong &myULong)
{
    ODULong                strLength;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, kODULong))
    {
        //---<2>
        StorageUnitGetValue(su, ev, kODULongLen, (ODValue)&myULong);
   }
} /* end GetULongProp */

/*
 *=============================================================================
 * Local function for writing a hashtable of LinkSrcInfo to the
 * storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Save old size of the focused value in oldSize.
 * <3>  Reserve space to write the number of elements in the hashtable.
 *      Initialize newSize to the size of ulong.
 * <4>  Iterate thru each element of the hash table.
 * <5>  Write the LinkID.
 *      Increase newSize by the size of IODLinkID.
 * <6>  Write the weak SU reference to the linksource storage unit.
 *      Increase newSize by the size of kODStorageUnitRef.
 * <7>  Write the weak SU reference to the source part storage unit.
*      If the source part from the last SetSourcePart is null,
 *      write a zero.
 *      Increase newSize by the size of kODStorageUnitRef.
 * <8>  Count the number of elements in the hashtable.
 * <9>  Delete unused space left from previous write.
 * <10> After iterating through the hashtable, refocus to the given property and
 *      value.
 *      Write the total number of elements in the hashtable in the space
 *      reseved at <3>.
 *=============================================================================
*/

void SetLinkSrcInfosProp (Environment *ev,
                             ODStorageUnit *su,
                             ODPropertyName prop,
                             ODValueType val,
                             OpenHashTable &LinkSrcInfos)
{
    ODLinkID                     key;
    LinkSrcInfo*                 linkSrcInfo;
    ODULong                      numOfElmnts = 0;
    ODULong                      newSize;
    ODULong                      oldSize;
    ODULong                      strLength;

    //---<1>
    ODSUForceFocus(ev, su, prop, val);

    //---<2>
    oldSize = su->GetSize(ev);

    //---<3>
    StorageUnitSetValue(su, ev, kODULongLen, (ODValue)&numOfElmnts);
    newSize = kODULongLen;

    //---<4>
    OpenHashTableIterator infos(&LinkSrcInfos);
    for (infos.First(&key, &linkSrcInfo);
         infos.IsNotComplete();
         infos.Next(&key, &linkSrcInfo))
    {
      //---<5>
      StorageUnitSetValue(su,
                          ev,
                          kODLinkIDLen,
                          (ODValue)&(linkSrcInfo->linkID));

      newSize += kODLinkIDLen;

      //---<6>
      StorageUnitSetValue(su,
                          ev,
                          kODStorageUnitRefLen,
                          (ODValue)&(linkSrcInfo->linkSrcSURef));

      newSize += kODStorageUnitRefLen;

      //---<7>
      ODStorageUnit *partSU =  (linkSrcInfo->linkSrc) -> AcquireSourcePart(ev);
      if ( partSU == kODNULL)
        {
           ODStorageUnitRef zero;
           memset(zero, 0, kODStorageUnitRefLen);
           StorageUnitSetValue(su,
                               ev,
                               kODStorageUnitRefLen,
                               (ODValue)kODNULL);
         } else {
           StorageUnitSetValue(su,
                               ev,
                               kODStorageUnitRefLen,
                               (ODValue)&(linkSrcInfo->srcPartSURef));
           partSU -> Release(ev);
         }


      newSize += kODStorageUnitRefLen;

      //---<8>
      numOfElmnts++;
    }

    //---<9>
    if (oldSize > newSize)
    {
      su->DeleteValue(ev, oldSize-newSize);
    }

    //---<10>
    su->Focus(ev, prop, kODPosUndefined, val, 0, kODPosUndefined);
    StorageUnitSetValue(su, ev, kODULongLen, (ODValue)&numOfElmnts);

} /* end SetLinkSrcInfosProp */

/*
 *=============================================================================
 * Local function for reading a hashtable of LinkSrcInfo from a
 * storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Read the total number of elements in the hashtable into numOfElemnts.
 * <3>  Loop to read in each element of the hashtable.
 * <4>  Create a new LinkSrcInfo.
 * <5>  Read in the link storage unit ref field of LinkSrcInfo.
 * <6>  Read in the LinkSource's SU reference.
 * <7>  Read in the source parts' SU reference.
 * <8>  Add linkInfo to the hashtable.
 *=============================================================================
*/

void GetLinkSrcInfosProp (Environment *ev,
                             ODStorageUnit *su,
                             ODPropertyName prop,
                             ODValueType val,
                             OpenHashTable &LinkSrcInfos)
{
    ODULong                i;
    ODLinkID               key;
    LinkSrcInfo*           linkSrcInfo;
    ODULong                numOfElmnts;
    ODID                   linkSUID;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, val))
    {
      //---<2>
      StorageUnitGetValue(su, ev, kODULongLen, (ODValue)&numOfElmnts);

      //---<3>
      for (i = 0; i < numOfElmnts; i++)
      {
        //---<4>
        linkSrcInfo = new LinkSrcInfo;

        //---<5>
        StorageUnitGetValue(su,
                            ev,
                            kODLinkIDLen,
                            (ODValue)&(linkSrcInfo->linkID));
        //---<6>
        StorageUnitGetValue(su,
                            ev,
                            kODStorageUnitRefLen,
                            (ODValue)&linkSrcInfo->linkSrcSURef);
        //---<7>
        StorageUnitGetValue(su,
                            ev,
                            kODStorageUnitRefLen,
                            (ODValue)&(linkSrcInfo->srcPartSURef));

        linkSrcInfo->linkSrc  = kODNULL;
        linkSrcInfo->isOpened = kODFalse;
        linkSrcInfo->isSaved  = kODTrue;
        //---<8>
        LinkSrcInfos.ReplaceEntry(&(linkSrcInfo->linkID), &linkSrcInfo);
      }
    }
} /* end GetLinkSrcInfosProp */

/*
 *=============================================================================
 * Local function for writing a hashtable of LinkTrgtInfo to the
 * storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Reserve space to write the number of elements in the hashtable.
 * <3>  Iterate thru each element of the hash table.
 * <4>  Write the trgtID field of linkTrgtInfo.
 * <5>  Write the weak SU reference to the linktarget storage unit.
 *      Increase newSize by the size of kODStorageUnitRef.
 * <6>  Write the weak SU reference to the source part storage unit.
 *      Increase newSize by the size of kODStorageUnitRef.
 * <7>  Count the number of elements in the hashtable.
 * <8>  After iterating thru the hashtable, refocus to the given property and
 *      value.
 *      Write the total number of elements in the hashtable in the space
 *      reseved at <2>.
 *=============================================================================
*/

void SetLinkTrgtInfosProp (Environment *ev,
                       ODStorageUnit *su,
                       ODPropertyName prop,
                       ODValueType val,
                       OpenHashTable &linkTrgtInfos)
{
    ODLinkConnectionID  key;
    LinkTrgtInfo*    linkTrgtInfo;
    ODULong          numOfElmnts = 0;
    ODULong          newSize;
    ODULong          oldSize;
    ODISOStr         remoteTrgtID;
    ODULong          strLength;
    ODID             trgtSUID;
    ODStorageUnitRef trgtSURef;

    //---<1>
    ODSUForceFocus(ev, su, prop, val);

    //---<2>
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfElmnts);

    //---<3>
    OpenHashTableIterator infos(&linkTrgtInfos);
    for (infos.First(&key, &linkTrgtInfo);
         infos.IsNotComplete();
         infos.Next(&key, &linkTrgtInfo))
    {
      //---<4>
      StorageUnitSetValue(su,
                          ev,
                          sizeof(ODLinkConnectionID),
                          (ODValue)&(linkTrgtInfo->trgtID));

      //---<5>
      StorageUnitSetValue(su,
                          ev,
                          kODStorageUnitRefLen,
                          (ODValue)&(linkTrgtInfo->linkTrgtSURef));

      newSize += kODStorageUnitRefLen;
      //---<6>
      StorageUnitSetValue(su,
                          ev,
                          kODStorageUnitRefLen,
                          (ODValue)&(linkTrgtInfo->trgtPartSURef));

      newSize += kODStorageUnitRefLen;
      //---<7>
      numOfElmnts++;
    }

    //---<8>
    su->Focus(ev, prop, kODPosSame, val, 0, kODPosSame);
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfElmnts);
}

/*
 *=============================================================================
 * Local function for reading a hashtable of LinkTrgtInfo from a
 * storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Read the total number of elements in the hashtable into numOfElemnts.
 * <3>  Loop to read in each element of the hashtable.
 * <4>  Create a new linkTrgtInfo.
 * <5>  Read in the trgtID field of linkTrgtInfo.
 * <6>  Read in the LinkTarget's SU reference.
 * <7>  Read in the source parts' SU reference.
 * <8>  Add linkTrgtInfo to the hashtable.
 *=============================================================================
*/

void GetLinkTrgtInfosProp (Environment *ev,
                       ODStorageUnit *su,
                       ODPropertyName prop,
                       ODValueType val,
                       OpenHashTable &linkTrgtInfos)
{
    ODULong          i;
    LinkTrgtInfo*    linkTrgtInfo;
    ODULong          numOfElmnts;
    ODISOStr         remoteTrgtID;
    ODULong          strLength;
    ODID             trgtSUID;
    ODStorageUnitRef trgtSURef;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, val))
    {
      //---<2>
      StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&numOfElmnts);

      //---<3>
      for (i = 0; i < numOfElmnts; i++)
      {
        //---<4>
        linkTrgtInfo = new LinkTrgtInfo;

        //---<5>
        StorageUnitGetValue(su,
                            ev,
                            sizeof(ODLinkConnectionID),
                            (ODValue)&(linkTrgtInfo->trgtID));

        //---<6>
        StorageUnitGetValue(su,
                            ev,
                            kODStorageUnitRefLen,
                            (ODValue)&linkTrgtInfo->linkTrgtSURef);
        //---<7>
        StorageUnitGetValue(su,
                            ev,
                            kODStorageUnitRefLen,
                            (ODValue)&(linkTrgtInfo->trgtPartSURef));

        //---<9>
        linkTrgtInfos.ReplaceEntry(&(linkTrgtInfo->trgtID), &linkTrgtInfo);
      }
    }
}

/*
 *=============================================================================
 * Local function for browsing an externalized LinkSrcInfos hashtable
 * in search of an entry matching a given LinkID. If found, the
 * corresponding storage unit reference is returned.
 * <1>  Focus to the property and value.
 * <2>  Read the total number of elements in the hashtable into numOfElemnts.
 * <3>  Loop to read linkid in each element of the hashtable.
 * <4>  Compare the linkid you read with the given linkid.
 * <5>  If the match - read in the corresponding link storage unit ref.
 * <6>  If they don't match - step to the next linkid entry.
 * <7>  Convert the storage unit ref you found to a storage unit id.
 * <8>  Return the storage unit id you found(or null if you did not find
 *      a matching LinkID).
 *=============================================================================
*/

ODStorageUnitID FindLinkSrcSURefbyLinkID (Environment      *ev,
                                        ODStorageUnit    *su,
                                        ODLinkID         linkid)
{
    ODULong                i;
    ODLinkID               key;
    LinkSrcInfo*           linkSrcInfo;
    ODULong                numOfElmnts;
    ODID                   linkSUID;
    ODLinkID               tempLinkID;
    ODULong                offset;
    ODStorageUnitRef       linkSrcSURef;
    ODStorageUnitID        suID = kODNULL;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, kODPropLinkSourceInfos, kODHashTable))
    {
      offset = su->GetOffset(ev);
      //---<2>
      StorageUnitGetValue(su, ev, kODULongLen, (ODValue)&numOfElmnts);
      offset += kODULongLen;

      //---<3>
      for (i = 0; i < numOfElmnts; i++)
      {

        StorageUnitGetValue(su,
                            ev,
                            kODLinkIDLen,
                            (ODValue)&tempLinkID);

       //---<4>
       if(tempLinkID == linkid)
       {
         //---<5>
         StorageUnitGetValue(su,
                             ev,
                             kODStorageUnitRefLen,
                             (ODValue)&linkSrcSURef);
         //---<3>
         if (su->IsValidStorageUnitRef(ev, linkSrcSURef))
         {
          suID = su->GetIDFromStorageUnitRef(ev, linkSrcSURef);
         }
         if (suID == (ODStorageUnitID)kODNULL)
         {
          ODSetSOMException(ev, kODErrInvalidLinkID);
         }
         break;
       }
       else
       {
         //---<6>
         offset += kODLinkIDLen;
         offset += kODStorageUnitRefLen;
         su->SetOffset(ev, offset);
       }
      } /* for loop over all elements */
    } /* if ODSUExistsThenFocus */
    //---<7>
    return (suID);
} /* end GetLinkSrcInfosProp */

/*
*=============================================================================
* DraftOpened
* Called by the Docshell after new writeable draft of
* a read-write document is opened (internalized)
  // Reads persistent information about an existing document
  // and calls "Open" on the AS; else calls "Register" on
  // the AS.
  // Initializes table of link sources for this draft
  // gets hold of the drafts _fDraftProperties storage unit
  // Internalizes the fLinkSrcInfos table from the draftproperties.
  // Internalizes the fLinkTrgtInfos table from the draftproperties.
  // (In this implementation, only one table per LinkService supported
* <0> Validate input variables.
* <1> Get the draft properties.
* <2> Initialize _fDraft and _fDocPathName instance variables
*     from input variable and set the _fExistsUnsavedLink to false.
* <3> Initialize empty LinkSrcInfos table.
* <4> Initialize empty LinkTrgtInfos table.
* <5> Remember whether document is new (or alternatively,
*     is old but has no ASName property, which could happen
*     if this is a saved template, in which case we treat it as new.)
*     This will be used later when
*     deferred opening or registration takes place.
*     If this is a new draft (or an existing draft with no suDraftProperties):
* <6> Get the Availability server name by query on environment variables
*     "HOSTNAME" and "SUFFIXNAME" and set the _fASName instance variable.
* <7> Leave _fAvlSvr null.  The acquisition of the availability server
*     will happen when later if a request to open or register a link is made
* <8>  This is not a new draft:
* <9>  Get the Availability Server name from the draft properties.
* <10> fAvlSvr remains null, as in <6> above.
* <11> Read the Document ID from draft properties.
* ** notice **: in the next version, we will compare new path name
*               to old path name and if they differ and if isnew is
*               false, then we know that if the old file is still there
*               we have detected a "copy" - otherwise, a "move"
* <12> Read the old Document pathname from draft properties.
* <14> Compare the old and new pathnames to detect a "move or "copy".
*      If they differ, bring up a dialogue to ask whether move or copy.
*      <14A.> If move, contact availability server to do DocMoved
*      <14B.> If copy, mark this as a new document but don't contact
*             the availability server.
* <16> If this is not a document copy - internalize the fLinkSrcInfos table
*      from the draftproperties.
* <17> If this is not a new document - internalize the fLinkTrgtInfos table
*      from the draftproperties.
* <18> An exception is caught - clean up and return.
*=============================================================================
*/
SOM_Scope void
SOMLINK DraftOpened(IODLinkService *somSelf,
                    Environment *ev,
                    ODDraft* draft,
                    ODBoolean isNew,
                    IODFileName pathName)
{

  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","DraftOpened");
  ODMoveCopyResult      mcResult;
  mcResult.FileMoved  = kODFalse;
  mcResult.FileCopied = kODFalse;
  SOMDServer* server;

    if (ev->_major) ODSetSOMException(ev, kODNoError);

    // Sets up link service's data if the draft is
    // writeable only. 27463

    if (!(draft->GetPermissions(ev) > kODDPSharedWrite)) {
      return;
    }

SOM_TRY
  //---<0>
  ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);
  ASSERT(pathName != (IODFileName)kODNULL, kODErrInvalidDocPathName);
  //---<1>
  ODStorageUnit* suDraftProperties = draft->AcquireDraftProperties(ev);
  ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
  //---<2>


  char pathbuffer[FILE_LENGTH];
  
  if (pathName)
  {
    APIRET rc = 
      DosQueryPathInfo(pathName, FIL_QUERYFULLNAME, pathbuffer, FILE_LENGTH);

    if (rc == NO_ERROR)
      pathName = pathbuffer; 
    else 
    LOG("Error %d converting %s to full pathname.\n", rc, pathName);
 
    pathName = strupr(pathName);

  }

        
  _fDraft = draft;
  _fDocPathName = ODISOStrFromCStr(pathName);
  _fExistsUnsavedLink = kODFalse;
  //---<3>
  _fLinkSrcInfos = new OpenHashTable;
  _fLinkSrcInfos->Initialize(kODInitialNumEntries,
                  sizeof(ODStorageUnitID),
                  sizeof(LinkSrcInfo*),
                  kODFalse);
  //---<4>
  _fLinkTrgtInfos = new OpenHashTable;
  _fLinkTrgtInfos->Initialize(kODInitialNumEntries,
                  sizeof(ODStorageUnitID),
                  sizeof(LinkTrgtInfo*),
                  kODFalse);

  //---<5>
  if (isNew || !suDraftProperties->Exists(ev, kODPropASName, kODISOStr, 0))
  {
    char            hostName[256];
    char            asName[256];

    _fIsNew = kODTrue;

    //---<6>
    strcpy(asName, getenv("HOSTNAME"));
    strcat(asName, "_");
    char *asSuffixName = getenv("AVLSVRNAME");
    if(asSuffixName != (char*)kODNULL)
    {
      strcat(asName, asSuffixName);
    }
    else
    {
      strcat(asName, "AS");
    }
    _fASName = ODISOStrFromCStr(asName);
    //XXXXX If the Availability Server is not started,
    //XXXXX   server->somdCreateObject(ev, "IODAvailServer", "")
    //XXXXX in ContactAvailabilityeServer always returns a NO_RESPONSE
    //XXXXX exception even though the Availability Server is fired up by that
    //XXXXX method call.
    //XXXXX This seems to be a DSOM bug!!!!!
//   SOMDServerMgr* servMgr;
//   servMgr = new SOMDServerMgr;
//   servMgr->somdStartServer(ev, _fASName);

}
  //---<7>
  else /* not a new draft */
  {
   _fIsNew = kODFalse;

    //---<8>
    GetISOStrProp(ev,
                  suDraftProperties,
                  kODPropASName,
                  &_fASName);

    //---<9>
    GetULongProp(ev,
                 suDraftProperties,
                 kODPropDocumentID,
                 _fDocumentID);

    //---<10>
    ODISOStr oldDocPathName;
    GetISOStrProp(ev,
                  suDraftProperties,
                  kODPropDocumentPathName,
                  &oldDocPathName);

    //---<11>
    if (!(ODISOStrEqual(oldDocPathName, _fDocPathName )))
    {
      char* pathName = GetPathName(oldDocPathName);

      // pathname is null: it is from a mounted dir which is no longer valid
      //                   so treat current file as a copy
      // pathname is a valid filename: treat current file as a copy
      if ((pathName == (char*)kODNULL) || (_access(pathName, 00) == 0))
      {
        SOMFree(pathName);
        _fIsNew = kODTrue;
      }
      else
      {
        // pathname does not exist: treat current file has been moved
        SOMFree(pathName);
        mcResult.FileMoved = kODTrue;
        mcResult.fNewDocPathName = _fDocPathName;
        mcResult.fOldDocPathName = oldDocPathName;
        ContactAvailabilityServer(ev, somSelf, somThis, &mcResult);
      }
    } /* end old pathname not equal new path name */
    SOMFree(oldDocPathName);

    //---<16>
    if (!_fIsNew)
    {
      GetLinkSrcInfosProp(ev,
                             suDraftProperties,
                             kODPropLinkSourceInfos,
                             kODHashTable,
                             *_fLinkSrcInfos);
    } /* if this is an original document, not a copy */
  } /* end if not a new draft */
  //---<17>
  if (!isNew)
  {
    GetLinkTrgtInfosProp(ev,
                           suDraftProperties,
                           kODPropLinkTargetInfos,
                           kODHashTable,
                           *_fLinkTrgtInfos);
  }
  _fDraftOpened = kODTrue;

 //---<18>
SOM_CATCH_ALL
   somSelf->CleanupLinkServiceProperties(ev);
SOM_ENDTRY
}/* end DraftOpened */

/*
*=============================================================================
* GetLSInDraft
      // Called by the AS Shell when a target requests a LinkSrc whose
      // draft is closed and its surrogate has not yet been constructed.
      // The AS Shell passes in a read-only draft and a linkid
      // and needs to get back the storage unit id for this linkid
      // in order to clone it.
      // The LinkService will focus the draft properties on the
      // LinkSrcInfos property, lookup the storage unit reference corresponding
      // to the linkid and map it to a storage unit id.
      // It will then clean up the temporary table and return a pointer
      // to the storage unit id.
* <0> Validate input variables.
* <1> Get the draft properties.
* <2> Browse the LinkSrcInfo hash table in the draft properties to get
*     the storage unit id corresponding to the given linkid.
* <3> Convert the storage unit ref to a storage unit id.
* <3> return the suID.
*=============================================================================
*/
SOM_Scope ODStorageUnitID
SOMLINK GetLinkSrcInDraft(IODLinkService *somSelf,
                     Environment *ev,
                     ODDraft* draft,
                     ODLinkID linkid)
{

  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","GetLSInDraft");

    if (ev->_major) ODSetSOMException(ev, kODNoError);

SOM_TRY
  //---<0>
  ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);
  ASSERT(linkid != (ODLinkID)kODNULL, kODErrInvalidLinkID);
  //---<1>
  ODStorageUnit* suDraftProperties = draft->AcquireDraftProperties(ev);
  ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
  //---<2>
  ODStorageUnitID suID =  FindLinkSrcSURefbyLinkID (ev,
                                                  suDraftProperties,
                                                  linkid);

  if (ODGetSOMException(ev) != kODNoError)
  {
    THROW(kODErrInvalidLinkID);
  }
  //---<3>
  return suID;
SOM_CATCH_ALL
  return 0;
SOM_ENDTRY
}/* end GetLinkSrcInDraft */

/*
*=============================================================================
* DraftMoved
* Called by the Docshell when a draft is moved.
*<0> Validate input variables.
*<1> Call the Availability Server's DocMoved, passing it both
*    the new and the old document pathnames.
*<2> Update instance variable to new pathname and save it to storage.
*=============================================================================
*/
SOM_Scope void  SOMLINK DraftMoved(IODLinkService *somSelf, Environment *ev,
                ODDraft* draft,
                IODFileName oldPathName,
                IODFileName newPathName)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","DraftMoved");

    if (ev->_major) ODSetSOMException(ev, kODNoError);

SOM_TRY
  //---<0>
  ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);
  ASSERT(oldPathName != (IODFileName)kODNULL, kODErrInvalidDocPathName);
  ASSERT(newPathName != (IODFileName)kODNULL, kODErrInvalidDocPathName);
  //---<1>
  _fAvlSvr->DocMoved(ev,
                     _fDocumentID,
                     oldPathName,
                     newPathName);
  if (ODGetSOMException(ev) != kODNoError)
  {
    return;
  }
  //---<2>
  delete _fDocPathName;
  _fDocPathName = ODISOStrFromCStr(newPathName);
  ODStorageUnit* suDraftProperties = draft->AcquireDraftProperties(ev);
  ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
  SetISOStrProp(ev,
                suDraftProperties,
                kODPropDocumentPathName,
                _fDocPathName);
SOM_CATCH_ALL
  return;
SOM_ENDTRY
} /* end DraftMoved */
/*
*=============================================================================
* DraftClosing
* Called by the Docshell when a draft opened with DraftOpened
* is closed.
* Notifies the AS that each LinkSource is closed.
* <0> Validate input variables.
* <1> For each LinkID entry in the LinkSrcInfos table:
* <2> If the link is unsaved call the AS to remove it.
* <3>   If the link is opened:
* <4>   Get its Auxilary State.
* <5>   Call the Availability Server: this link is closed
* <6>   Clean up the entry in the LinkSrcInfos table.
* <7> Delete and nullify the LinkSrcInfos which has been emptied out.
*     (This is done now to save iterating the hash table at cleanup )
* <7.1> If availability server contacted, ask AvailServer to close document.
* <8> Delete and nullify the LinkTrgtInfos which has been emptied out.
*     (This is done now to save iterating the hash table at cleanup )
* <9> Clean up the LinkService Properties
*     instance variables.
****NOTICE**** find out what to do if LinkSrc is locked.
*=============================================================================
*/
SOM_Scope void
SOMLINK DraftClosing(IODLinkService *somSelf,
                     Environment *ev,
                     ODDraft* draft)
{
    IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
    IODLinkServiceMethodDebug("IODLinkService","DraftClosing");

    if (ev->_major) ODSetSOMException(ev, kODNoError);

SOM_TRY
    //---<0>
    ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);

    // Clean up link service's data if the draft is
    // writeable only. 27463

    if (!(draft->GetPermissions(ev) > kODDPSharedWrite)) {
      return;
    }

    // If a draft from the draft history is deleted, DraftClosing is
    // called. However, we should not do any cleanups as the
    // current active draft (_fDraft) is still valid. So the
    // check below will protect us from unnecessary cleanups.      
    // 27463 

    if (draft !=_fDraft) 
       return;

    if (!_fLinkSrcInfos) return;                      // 25006
    OpenHashTableIterator iter(_fLinkSrcInfos);
    ODLinkID linkID;
    LinkSrcInfo *linkSrcInfo = kODNULL;
    //---<1>
    for (iter.First(&linkID, &linkSrcInfo);
         iter.IsNotComplete();
         iter.Next(&linkID, &linkSrcInfo))
    {
      //---<2>
      // If there are links, then _fAvlSvr is non-null
      if (!linkSrcInfo->isSaved)
      {
      TRY
        _fAvlSvr-> RemoveLink(ev,
                              linkID);
      CATCH_ALL
      ENDTRY
      }
      //---<3>
      else if (linkSrcInfo->isOpened)
      {
        //---<4>
        IODAuxiliaryState *auxState = new IODAuxiliaryState;
        (linkSrcInfo->linkSrc)->GetAuxState(ev, &auxState);

        //---<5>
      TRY
        _fAvlSvr-> LinkClosed(ev,
                              linkID,
                              auxState);
      CATCH_ALL
      ENDTRY
        ODDisposePtr(auxState->remoteLinkInfos._buffer);
        delete auxState;


         //--<6>
         (linkSrcInfo->linkSrc)->Release(ev); /* once for linkservice */
         (linkSrcInfo->linkSrc)->Release(ev); /* once more for avs */
      }
    delete linkSrcInfo;
    }
    //---<7>
    delete _fLinkSrcInfos;
    _fLinkSrcInfos = (OpenHashTable*)kODNULL;

    //---<7.1>
    if (_fAvlSvr != kODNULL) {
      _fAvlSvr->DocClosed(ev, _fDocumentID);
     }

    //---<8>
    ODLinkConnectionID id;
    LinkTrgtInfo*   linkTrgtInfo;
    OpenHashTableIterator trgtIter(_fLinkTrgtInfos);
    for (trgtIter.First(&id, &linkTrgtInfo);
         trgtIter.IsNotComplete();
         trgtIter.Next(&id, &linkTrgtInfo))
    {
      delete linkTrgtInfo;
    }
    delete _fLinkTrgtInfos;
    _fLinkTrgtInfos = kODNULL;

    //---<9>
    somSelf->CleanupLinkServiceProperties(ev);
SOM_CATCH_ALL
  return;
SOM_ENDTRY
} /* end DraftClosing */


/*
*=============================================================================
* DraftSaving:
      // Called by the Docshell/Draft BEFORE a draft is saved.
      // The LinkService will externalize its properties as
      // well as the LinkSrcInfos hash table into the draft properties.
      // The LinkService will go over LinkSrcInfos hash table
      // and set the saved bit on each entry to true (in order
      // to keep track of which linksources have never been saved).
* <-1> Ignore if given draft is not opened.
* <0> Validate input variables.
* <1> Get hold of the draft properties storage unit to externalize the
*     Link Service properties.
* <3> Externalize the Document Path Name property.
* <4> If the AS was contacted, externalize the Availability Server Name property
*     and the DocumentID property, else set DocumentID to 0.
* <5> Externalize the Link Source Table.
* <6> Externalize the Link Target Table.
* <7> Iterate over the Link Source Table and set the isSaved field
*     in each entry to true.
*=============================================================================
*/
SOM_Scope void
SOMLINK DraftSaving(IODLinkService *somSelf,
                   Environment *ev,
                   ODDraft* draft)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","DraftSaving");

    if (ev->_major) ODSetSOMException(ev, kODNoError);

SOM_TRY
  //---<-1>
  if (draft != _fDraft) return;

  //---<0>
  ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);
  //---<1>
  ODStorageUnit* suDraftProperties = draft->AcquireDraftProperties(ev);
  ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
  //---<3>
  SetISOStrProp(ev,
                suDraftProperties,
                kODPropDocumentPathName,
                _fDocPathName);
  //---<4>
  if (_fAvlSvr != kODNULL) {
    SetISOStrProp(ev,
                suDraftProperties,
                kODPropASName,
                _fASName);
    SetULongProp(ev,
                 suDraftProperties,
                 kODPropDocumentID,
                 _fDocumentID);
    }
  else {
    ODULong Tmp = 0;
    SetULongProp(ev,
                 suDraftProperties,
                 kODPropDocumentID,
                 Tmp);
    }


  //---<5>
  SetLinkSrcInfosProp(ev,
                      suDraftProperties,
                      kODPropLinkSourceInfos,
                      kODHashTable,
                      *_fLinkSrcInfos);
  //---<6>
  SetLinkTrgtInfosProp(ev,
                      suDraftProperties,
                      kODPropLinkTargetInfos,
                      kODHashTable,
                      *_fLinkTrgtInfos);
  //---<7>
  OpenHashTableIterator iter(_fLinkSrcInfos);
  ODLinkID linkID;
  LinkSrcInfo *linkSrcInfo = kODNULL;
  for (iter.First(&linkID, &linkSrcInfo);
       iter.IsNotComplete();
       iter.Next(&linkID, &linkSrcInfo))
  {
    linkSrcInfo->isSaved = kODTrue;
  }
SOM_CATCH_ALL
  return;
SOM_ENDTRY
} /* end DraftSaving */

/*
*=============================================================================
* UnsavedExportedLinks:
      // Allows LinkSource to discover whether there are cross-document
      // links which would be lost if the user fails to save now.
* <0> Validate input variables.
* <1> If the _fExistsUnsavedLink instance data fields is set to true,
*     it means there is one or more unsaved links - return true.
* <2> If the _fExistsUnsavedLink instance data fields is set to false,
*     return false.
*=============================================================================
*/
SOM_Scope ODBoolean
SOMLINK UnsavedExportedLinks(IODLinkService *somSelf,
                             Environment *ev,
                             ODDraft* draft)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","UnsavedExportedLinks");

    if (ev->_major) ODSetSOMException(ev, kODNoError);

SOM_TRY
  //---<0>
  ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);
  //---<1>
  if (_fExistsUnsavedLink)
  {
    return kODTrue;
  }
  //---<2>
  else
  {
   return kODFalse;
  }
SOM_CATCH_ALL
  return kODFalse;
SOM_ENDTRY
}


/*
*=============================================================================
* RegisterLinkSource:
     // Called by draft when a new link source is being
     // created.  The draft must have previously issued DraftOpened.
     // The LinkService will remember the linkSource and
     // suid; it will register the linkSource with the AS.
* <0> Validate input variables.
* <1> Get hold of the Draft Properties storage unit.
* <2> Create a LinkSrcInfo entry, and set its linkSrc pointer field from input.
* <2bis> If availability server not contacted yet, contact it now.
* <3> Register the Link Source with the Availability Server and get back
*     a LinkID. Set the LinkID field of the LinkSrcInfo entry to this LinkID.
* <4> Set the isOpened field in the entry to true.
* <5> Set the ExistsUnsavedLink field property to true.
* <6> Get a Storage Unit Reference to the Link Source's suid which was passed
*     in as a parameter, and set the entry's suref field to it.
* <7> Get a Storage Unit Reference to the storage unit of the part which has
*     been passed in as a parameter, and set the entry's partSURef field to it.
* <8> Add the entry to the LinkSrcInfos HashTable.
* <8bis> Copy the ASName
* <9> If linkSrc registration failed<tested in 3> - null linkid was returned.
*     Release linkSrcInfo
*     Raise exception
*     Return 0 as linkID
*=============================================================================
*/
SOM_Scope ODLinkID
SOMLINK RegisterLinkSource(IODLinkService *somSelf,
                           Environment *ev,
                           ODDraft* draft,
                           ODStorageUnitID suid,
                           ODLinkSource* linkSource,
                           ODPart* part,
                           ODISOStr* ASName)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","RegisterLinkSource");

  if (ev->_major) ODSetSOMException(ev, kODNoError);
  LinkSrcInfo* linkSrcInfo = kODNULL;

SOM_TRY
  //---<0>
  ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);
  ASSERT(suid != (ODStorageUnitID)kODNULL, kODErrInvalidStorageUnit);
  ASSERT(linkSource != (ODLinkSource*)kODNULL, kODErrInvalidLinkSource);
  //---<1>
  ODStorageUnit* suDraftProperties = draft->AcquireDraftProperties(ev);
  ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
  //---<2>
  linkSrcInfo = new LinkSrcInfo;
  linkSrcInfo->linkSrc     = (IODLinkSource*)linkSource;
  linkSrcInfo->linkSrc->Acquire(ev);
  linkSrcInfo->linkID      = kODNULL;

  //--- <2bis>
  ContactAvailabilityServer(ev, somSelf, somThis, kODNULL);

    //---<3>
    linkSrcInfo->linkID      = _fAvlSvr->RegisterLink(ev,
                                                      _fDocumentID,
                                                      (IODLinkTargetToLinkSource*)linkSource);

    THROW_IF_NULL((void *)(linkSrcInfo->linkID));
    linkSource->Acquire(ev); /* record that avs retains a ptr to linkSource */
    //---<4>
    linkSrcInfo->isOpened    = kODTrue;
    //---<5>
    linkSrcInfo->isSaved  = kODFalse;
    _fExistsUnsavedLink = kODTrue;
    //---<6>

    ODSUForceFocus(ev, suDraftProperties,kODPropLinkSourceInfos, kODHashTable);
    suDraftProperties->GetWeakStorageUnitRef(ev,
                                             suid,
                                             linkSrcInfo->linkSrcSURef);
    //---<7>
    if (part != (ODPart*)kODNULL)
    {
     ODStorageUnit*  partSU   = part->GetStorageUnit(ev);
     ODStorageUnitID srcPartSUID = partSU->GetID(ev);
     ODSUForceFocus(ev, suDraftProperties,kODPropLinkSourceInfos, kODHashTable);
     suDraftProperties->GetWeakStorageUnitRef(ev,
                                             srcPartSUID,
                                             linkSrcInfo->srcPartSURef);
    }
    else
    {
//      linkSrcInfo->srcPartSURef = (ODStorageUnitRef)kODNULL;
    }
    //---<8>
    _fLinkSrcInfos->ReplaceEntry(&(linkSrcInfo->linkID), &linkSrcInfo);

    //---<8bis>
    *ASName = ODISOStrFromCStr(_fASName);
    return(linkSrcInfo->linkID);
SOM_CATCH_ALL
    //---<9>
    delete linkSrcInfo;
    return 0;
SOM_ENDTRY
}

/*
*=============================================================================
*  OpenLinkSource:
     // Called by the draft when an existing link source is being
     // internalized and the draft is read/write.  The draft must
     // have previously issued DraftOpened.
     // The LinkService will retrieve auxState and isLocked
     // by calling the Availability server's OpenLinkSource;
     //it will remember the location of the linkSource.
 * <0> Validate input variables.
 * <1bis> Contact the availability server if haven't yet done so.
 * <1> Get the LinkSrcInfos Table entry corresponding to the LinkID which
 *     is passed in as a parameter.If not found, or
 *     if the linkid corresponds to a different SU, then we
 *     assume the SU passed to OpenLinkSource is a copy, and do
 *     a register, returning the new linkid. Proceed if not a copy.
 * <1'> If the sourcepart SU ref is 0, then this is a "deleted"
 *     link source (i.e. it was externalized after the user did
 *     a SetSourcePart to null).  Raise an exception.
 * <2> Call the Availability Server's LinkOpened method passing in the
 *     LinkID and LinkSource pointer.
 *     We get back the auxiliary state and the isLocked info.
 * <3> If LinkOpened returned a LinkAlreadyOpen exception - return
 * <4> If LinkOpened returned any other exception - throw a LinkOpenedFailed
 *     exception.
 * <5> Set the entry's LinkSrc field to the linkSource passed in.
 * <6> Set the entry's isOpened field to true.
 * <7> Return a null ODLinkID, and the current ASName
 * <8> If a LinkOpened failure is caught<tested in 4> -
 *     Remove the linkSrcInfo entry
 *     Raise exception and return
*=============================================================================
*/
SOM_Scope ODLinkID
SOMLINK OpenLinkSource(IODLinkService *somSelf,
                       Environment *ev,
                       ODDraft* draft,
                       ODStorageUnitID suid, /* for checking only */
                       ODLinkSource* linkSource,
                       ODLinkID linkid,
                       IODAuxiliaryState** auxState,
                       ODBoolean* isLocked,
                       ODISOStr* ASName)
{

  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","OpenLinkSource");

    if (ev->_major) ODSetSOMException(ev, kODNoError);
  LinkSrcInfo* linkSrcInfo;

SOM_TRY
  //---<0>
  ASSERT(draft != (ODDraft*)kODNULL, kODErrInvalidDraft);
  ASSERT(linkSource != (ODLinkSource*)kODNULL, kODErrInvalidLinkSource);
  ASSERT(linkid != (ODLinkID)kODNULL, kODErrInvalidLinkID);

  ODBoolean copy; /* true if this linksource is a copy */

  //---<1bis>
  ContactAvailabilityServer(ev, somSelf, somThis, kODNULL);

  //---<1>
  if (_fLinkSrcInfos->GetValue(&linkid, &linkSrcInfo) != kODTrue)
  {
     copy = kODTrue;
  } else {
    ODStorageUnit* suDraftProperties = _fDraft->AcquireDraftProperties(ev);
    ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
    ODSUForceFocus(ev, suDraftProperties, kODPropLinkSourceInfos, kODHashTable);
    if (suDraftProperties->IsValidStorageUnitRef(ev, linkSrcInfo->linkSrcSURef))
      {
        copy = (suid != suDraftProperties->GetIDFromStorageUnitRef(ev, linkSrcInfo->linkSrcSURef));
      } else {
        copy = kODTrue;
      };
  }
  if (copy) {
    *isLocked = kODFalse;
    *auxState = kODNULL;
    return somSelf->RegisterLinkSource(ev, draft, suid, linkSource,kODNULL, ASName);
    }
SOM_CATCH_ALL
  return 0;
SOM_ENDTRY

  //---<1'>
  if ((linkSrcInfo->srcPartSURef[0] == 0) &&
      (linkSrcInfo->srcPartSURef[1] == 0) &&
      (linkSrcInfo->srcPartSURef[2] == 0) &&
      (linkSrcInfo->srcPartSURef[3] == 0))
    {
      ODSetSOMException(ev,  kODErrInvalidStorageUnit);
      return 0;
    }

  //---<2>
SOM_TRY
  _fAvlSvr->LinkOpened(ev,
                       linkid,
                       (IODLinkTargetToLinkSource*)linkSource,
                       auxState,
                       isLocked);
SOM_CATCH_ALL
  if (ODGetSOMException(ev) == kODErrInvalidLinkID)
  {
    _fLinkSrcInfos->RemoveEntry(&linkid);
    return 0;
  }
  //---<4>
  ODSetSOMException(ev, kODErrLinkOpenedFailed);
  return 0;
SOM_ENDTRY
   //---<5>
  linkSrcInfo->linkSrc     = (IODLinkSource*)linkSource;
  linkSrcInfo->linkSrc->Acquire(ev); /* avs retains a ptr to ls */
  linkSrcInfo->linkSrc->Acquire(ev); /* linksrvc retains a ptr to ls */
  //---<6>
  linkSrcInfo->isOpened    = kODTrue;
  //---<7>
  *ASName = ODISOStrFromCStr(_fASName);
  return(0);
}

/*
*=============================================================================
* InternalizeLinkSource:
     // Called by the Availability Server to internalize a Link
     // Source frome storage.
     // If the LS is already opened - will return a pointer to it.
     // Otherwise, queries the LinkSrcInfos Hashtable for this
     // Link Source's storage unit reference using the LinkId as key,
     // Gets the storage unit ID from the storage unit reference,
     // Calls draft to get back a linksource pointer, updates the
     // table and returns the pointer to the Link Source.
* <0> Validate input variables.
* <1> Get the LinkSrcInfos Table entry corresponding to the LinkID which
*     is passed in as a parameter.
* <2> If the LinkSrc is already opened - return a pointer to it.
* <3> The LinkSrc is not yet opened:
* <4> Get the storage unit ID from the storage unit reference in the
*     LinkSrcInfo entry.
* <5> Call the draft to get the pointer to the LinkSource corresponding
*     to the storage unit id. Save it in the LinkSrcInfo entry.
* <6> Set the isOpened field in the LinkSrcInfo entry to true.
* <7> Increment refcount and Return a pointer to the LinkSrc.
*=============================================================================
*/
SOM_Scope ODLinkSource*
SOMLINK InternalizeLinkSource(IODLinkService *somSelf,
                              Environment *ev,
                              ODLinkID linkid,
                              IODAuxiliaryState* auxState)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","InternalizeLinkSource");

    if (ev->_major) ODSetSOMException(ev, kODNoError);

SOM_TRY
  //---<0>
  ASSERT(linkid != (ODLinkID)kODNULL, kODErrInvalidLinkID);
  ASSERT(auxState != (IODAuxiliaryState*)kODNULL, kODErrInvalidAuxState);
  LinkSrcInfo *linkSrcInfo;
  //---<1>
  if (_fLinkSrcInfos->GetValue(&linkid, &linkSrcInfo) != kODTrue)
  {
   ODSetSOMException(ev, kODErrLinkIDNotInDoc);
   return (ODLinkSource*)kODNULL ;
  }
  //---<2>
  if (linkSrcInfo->isOpened)
  {
    return(linkSrcInfo->linkSrc);
  }
  //---<3>
  else /* LinkSrcInfo entry not in opened state */
  {
  SOM_TRY
    //---<4>
    ODStorageUnit* suDraftProperties = _fDraft->AcquireDraftProperties(ev);
    ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
      ODStorageUnitID suID;
      if (suDraftProperties->IsValidStorageUnitRef(ev, linkSrcInfo->linkSrcSURef))
      {
       suID = suDraftProperties->GetIDFromStorageUnitRef(ev, linkSrcInfo->linkSrcSURef);
      }
      else
      {
        THROW(kODErrInvalidLinkSource);
      }
      //---<5>
      linkSrcInfo->linkSrc = (IODLinkSource*)
                             ((CMDraft*)_fDraft)->GetLinkSrcForAvlSvr(ev,
                                                           suID,
                                                           auxState);
      ASSERT(linkSrcInfo->linkSrc != (ODLinkSource*)kODNULL, kODErrInvalidLinkSource);

      linkSrcInfo->linkSrc->Acquire(ev);


      //---<6>
      linkSrcInfo->isOpened = kODTrue;
      //---<7>
      linkSrcInfo->linkSrc->Acquire(ev);
      return(linkSrcInfo->linkSrc);

  SOM_CATCH_ALL
      //---<7>
      _fLinkSrcInfos->RemoveEntry(&linkid);
      return kODNULL;
  SOM_ENDTRY
  } /* end if LinkSrcInfo entry not in opened state */
SOM_CATCH_ALL
  return kODNULL;
SOM_ENDTRY
} /* end InternalizeLinkSource */

/*
*=============================================================================
      // EstablishLinkFromTSpec:
      // Called in LinkSpec::EstablishLink to establish a new link
      // to the Part with sourcePartSUID.
* <0> Validate input variables.
* <1> Call the draft to get a pointer to the part from the part's suid.
* <2> Call CreateLink on the part. It will return a Link Source
* <3> Get the linkid and ASName of that link source
* <4> Add the established Link from the Link Spec to the LinkSource's table.
* <5> Set the LinkSrc pointer return value to the Link Target.
*=============================================================================
*/
SOM_Scope void
SOMLINK EstablishLinkFromTSpec(IODLinkService *somSelf,
                               Environment *ev,
                               ODULong pid,
                               ODStorageUnitID srcPartSUID,
                               ODByteArray* data,
                               ODLinkConnectionData* trgtPartDesc,
                               IODLinkTarget* linkTrgt,
                               ODLinkID* linkid,
                               IODLinkTargetToLinkSource** linkSrc,
                               ODLinkConnectionID* trgtID,
                               ODBoolean* isLocal,
                               ODBoolean* trackTrgt,
                               ODISOStr*  ASName)
{
    IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
    IODLinkServiceMethodDebug("IODLinkService","EstablishLinkFromTSpec");

    ODStorageUnitID sourceSUID; /* not needed because linksrc is returned */
    if (ev->_major) ODSetSOMException(ev, kODNoError);

SOM_TRY
  //---<0>
    ASSERT(srcPartSUID != (ODStorageUnitID)kODNULL, kODErrInvalidStorageUnit);
    ASSERT(_fDraftOpened == kODTrue, kODErrDocNotOpened);

  //---<1>
    ODPart *part = _fDraft->AcquirePart(ev, srcPartSUID);

  //---<2>
    IODLinkSource *linkSource = (IODLinkSource*)part->CreateLink(ev, data);

  //---<3>
    ASSERT(linkSource != (ODLinkSource*)kODNULL, kODErrInvalidLinkSource);

    linkSource->GetPersistentLinkInfo(ev, linkid, ASName);

  //---<4>
    linkSource->AddLinkTarget(ev,
                              *linkid,
                              pid,
                              trgtPartDesc,
                              linkTrgt,
                              &sourceSUID,
                              trgtID,
                              isLocal,
                              trackTrgt);

  //---<5>
    if (*isLocal)
    {
      linkSource->Acquire(ev);
      *linkSrc = (IODLinkTargetToLinkSource*)linkSource;
    }
    else
    {
      *linkSrc = (IODLinkTargetToLinkSource*)_fAvlSvr;
    }
SOM_CATCH_ALL
  return;
SOM_ENDTRY
}

/*
*=============================================================================
      // RegisterLinkTarget:
      // Called in LinkSpec::EstablishLink to register a new target
* <1> create a linktrgtinfo entry for the established Link.
* <2> Add the linktrgtinfo to the LinkTarget's table.
*=============================================================================
*/
SOM_Scope void  SOMLINK RegisterLinkTarget(IODLinkService *somSelf, Environment *ev,
                ODStorageUnitID trgtPartSUID,
                IODLinkTarget* linkTrgt)
{
    IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
    IODLinkServiceMethodDebug("IODLinkService","RegisterLinkTarget");

SOM_TRY
   //---<1>
   LinkTrgtInfo* linkTrgtInfo = new LinkTrgtInfo;
   THROW_IF_NULL((void *)linkTrgtInfo);
   linkTrgtInfo->trgtID         = ++_fTargetIDCounter;
   linkTrgtInfo->linkTrgt       = linkTrgt;
   ODStorageUnit* linkTrgtSU    = linkTrgt->GetStorageUnit(ev);
   ODStorageUnitID linkTrgtSUID = linkTrgtSU->GetID(ev);
   ODStorageUnit* suDraftProperties = _fDraft->AcquireDraftProperties(ev);
   ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);
   if (linkTrgtSUID != (ODStorageUnitID)kODNULL)
   {
     ODSUForceFocus(ev, suDraftProperties,kODPropLinkTargetInfos, kODHashTable);
     suDraftProperties->GetWeakStorageUnitRef(ev,
                                              linkTrgtSUID,
                                              linkTrgtInfo->linkTrgtSURef);
   }
   else
   {
//      linkTrgtInfo->linkTrgtSURef = (ODStorageUnitRef)kODNULL;
   }
   if (trgtPartSUID != (ODStorageUnitID)kODNULL)
   {
    ODSUForceFocus(ev, suDraftProperties,kODPropLinkTargetInfos, kODHashTable);
    suDraftProperties->GetWeakStorageUnitRef(ev,
                                             trgtPartSUID,
                                             linkTrgtInfo->trgtPartSURef);
   }
   else
   {
//      linkTrgtInfo->srcPartSURef = (ODStorageUnitRef)kODNULL;
   }
   //---<2>
   _fLinkTrgtInfos->ReplaceEntry(&(linkTrgtInfo->trgtID), &linkTrgtInfo);
SOM_CATCH_ALL
  return;
SOM_ENDTRY
}

/*
 *=============================================================================
 * Called by Draft to obtain the list of LinkSources belonging to a
 *   given part
 *
 * <1>  Get the Storage Unit Reference for the part.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve all linkSource SUID's from fLinkInfos which have
 *      a part storage unit ref similar to given part,
 *      and add them to linkSourceIter.
 *      Release fMutex and return.
 * <4>  An exception is caught.
 *      Release fMutex.
 *      Cleanup linkSrcIter.
 *      Set SOM exception and return.
 *=============================================================================
 */
SOM_Scope void  SOMLINK GetLinkSourcesForPart(IODLinkService *somSelf, Environment *ev,
                ODPart* part,
                CMLinkSourceIterator* linkSourceIter)
{
    IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
    IODLinkServiceMethodDebug("IODLinkService","EstablishLinkFromTSpec");

SOM_TRY
    ODStorageUnit* suDraftProperties = _fDraft->AcquireDraftProperties(ev);
    ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);

    //---<1>
    ODStorageUnit*  partSU   = part->GetStorageUnit(ev);
    ODStorageUnitID partSUID = partSU->GetID(ev);

     //---<2>
    _fMutex->Acquire(ev);

      //---<3>
      ODLinkID linkID;
      ODStorageUnitID  linkSrcSUID;
      ODStorageUnitID  thisPartSUID;
      LinkSrcInfo *linkSrcInfo = kODNULL;
      ODSUForceFocus(ev, suDraftProperties,kODPropLinkSourceInfos, kODHashTable);
      if (_fLinkSrcInfos != (OpenHashTable*)kODNULL)
      {
       OpenHashTableIterator iter(_fLinkSrcInfos);
       for (iter.First(&linkID, &linkSrcInfo);
            iter.IsNotComplete();
            iter.Next(&linkID, &linkSrcInfo))
        {
         if (suDraftProperties->IsValidStorageUnitRef(ev, linkSrcInfo->srcPartSURef))
         {
          thisPartSUID = suDraftProperties->GetIDFromStorageUnitRef(ev, linkSrcInfo->srcPartSURef);
         }
         else
         {
           thisPartSUID = (ODStorageUnitID)kODNULL;
         }
         if (thisPartSUID == partSUID)
         {
           if (suDraftProperties->IsValidStorageUnitRef(ev, linkSrcInfo->linkSrcSURef))
           {
            linkSrcSUID = suDraftProperties->GetIDFromStorageUnitRef(ev, linkSrcInfo->linkSrcSURef);
           }
           else
           {
             THROW(kODErrInvalidLinkSource);
           }
// #ifdef OS2LinkChanges
          linkSourceIter->Add(ev, linkSrcSUID);
// #endif // OS2LinkChanges
         }
        }
      }
      _fMutex->Release(ev);

SOM_CATCH_ALL

      ODError exception = ODGetSOMException(ev);

      //---<4>
    TRY
      _fMutex->Release(ev);
    CATCH_ALL
    ENDTRY


      if (linkSourceIter != kODNULL)
      {
        delete linkSourceIter;
      }

      ODSetSOMException(ev, exception);
SOM_ENDTRY
}

/*
 *=============================================================================
 * Called by Draft to obtain the list of LinkTargets belonging to a
 *   given part
 *
 * <1>  Get the Storage Unit Reference for the part.
 * <2>  Acquire fMutex before accessing fLinkTargetInfos.
 * <3>  Retrieve all linkTarget SUID's from fLinkTargetInfos which have
 *      a part storage unit ref similar to given part,
 *      and add them to linkTargetIter.
 *      Release fMutex and return.
 * <4>  An exception is caught.
 *      Release fMutex.
 *      Cleanup linkTargetIter.
 *      Set SOM exception and return.
 *=============================================================================
 */
SOM_Scope void  SOMLINK GetLinkTargetsForPart(IODLinkService *somSelf, Environment *ev,
                ODPart* part,
                CMLinkIterator* linkTargetIter)
{
    IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
    IODLinkServiceMethodDebug("IODLinkService","GetLinkTargetsForPart");

SOM_TRY
    ODStorageUnit* suDraftProperties = _fDraft->AcquireDraftProperties(ev);
    ASSERT(suDraftProperties != (ODStorageUnit*)kODNULL, kODErrInvalidStorageUnit);

    //---<1>
    ODStorageUnit*  partSU   = part->GetStorageUnit(ev);
    ODStorageUnitID partSUID = partSU->GetID(ev);

     //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

      //---<3>
      ODID linkID;
      ODStorageUnitID  linkTrgtSUID;
      ODStorageUnitID  thisPartSUID;
      LinkTrgtInfo *linkTrgtInfo = kODNULL;
      ODSUForceFocus(ev, suDraftProperties,kODPropLinkTargetInfos, kODHashTable);
      if (_fLinkTrgtInfos != (OpenHashTable*)kODNULL)
      {
       OpenHashTableIterator iter(_fLinkTrgtInfos);
       for (iter.First(&linkID, &linkTrgtInfo);
            iter.IsNotComplete();
            iter.Next(&linkID, &linkTrgtInfo))
        {
         if (suDraftProperties->IsValidStorageUnitRef(ev, linkTrgtInfo->trgtPartSURef))
         {
          thisPartSUID = suDraftProperties->GetIDFromStorageUnitRef(ev, linkTrgtInfo->trgtPartSURef);
         }
         else
         {
           thisPartSUID = (ODStorageUnitID)kODNULL;
         }
         if (thisPartSUID == partSUID)
         {
           if (suDraftProperties->IsValidStorageUnitRef(ev, linkTrgtInfo->linkTrgtSURef))
           {
            linkTrgtSUID = suDraftProperties->GetIDFromStorageUnitRef(ev, linkTrgtInfo->linkTrgtSURef);
           }
           else
           {
             THROW(kODErrInvalidLink);
           }
// #ifdef OS2LinkChanges
          linkTargetIter->Add(ev, linkTrgtSUID);
// #endif // OS2LinkChanges
         }
        }
      }
      _fMutex->Release(ev);

SOM_CATCH_ALL

      ODError exception = ODGetSOMException(ev);

      //---<4>
    TRY
      _fMutex->Release(ev);
    CATCH_ALL
    ENDTRY


      if (linkTargetIter != kODNULL)
      {
        delete linkTargetIter;
      }

      ODSetSOMException(ev, exception);
SOM_ENDTRY
}

/*
 *=============================================================================
 * Called by Linkspec to obtain the Persistent Link Infos
 *
 * <1>  Use the draft to Acquire Part
 * <2>  Call CreateLink using the Data to get a pointer to the Linksrc
 * <3>  Call GetPersistentLinkInfo to get the LinkId and ASName
 * <4>  Release the Linksrc pointer
 *=============================================================================
 */
SOM_Scope void  SOMLINK GetPersistentLinkInfoFromTSpec(IODLinkService *somSelf, Environment *ev,
                ODStorageUnitID   partSUID,
                ODByteArray  *partData,
                ODLinkID     *linkID,
                ODISOStr     *ASName)
{
    IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
    IODLinkServiceMethodDebug("IODLinkService","GetPersistentLinkInfoFromTspec");

SOM_TRY
    // 1.
    ODPart* myPart = _fDraft->AcquirePart(ev, partSUID);

    // 2.
    IODLinkSource* myLinkSrc = (IODLinkSource*) myPart->CreateLink(ev,partData);
    ASSERT(myLinkSrc != (ODLinkSource*)kODNULL, kODErrInvalidLinkSource);

    // 3.
    myLinkSrc->GetPersistentLinkInfo(ev, linkID,  ASName);

    // 4.
    myLinkSrc->Release(ev);
SOM_CATCH_ALL
  return;
SOM_ENDTRY
}

/*
*=============================================================================
      //somInit
*=============================================================================
*/
SOM_Scope void
SOMLINK somInit(IODLinkService *somSelf)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","InitLinkService");
  _fDraftOpened  = kODFalse; /* true if a docshell called DraftOpened*/
  _fDraft        = kODNULL;  /* initialized if DraftOpened   */
  _fDocumentID   = 0;        /* initialized if DraftOpened   */
  _fAvlSvr       = kODNULL;  /* initialized if DraftOpened   */
  _fLinkSrcInfos = kODNULL;  /* a table of LinkSrcInfos
                                           keyed by LinkId   */
  _fLinkTrgtInfos = kODNULL; /* a table of LinkTrgtInfos
                                           keyed by LinkId   */
  _fDocPathName  = kODNULL;  /* name from which doc was read */
  _fASName       = kODNULL;  /* name of availability server  */
  _fExistsUnsavedLink = kODFalse;  /* true if there is one or more */
  _fMutex = new IODMutex;
  THROW_IF_NULL((void*)_fMutex);
}

/*
*=============================================================================
      //somUnInit
*=============================================================================
*/
SOM_Scope void
SOMLINK somUninit(IODLinkService *somSelf)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","UnInitLinkService");
  somSelf->CleanupLinkServiceProperties(somGetGlobalEnvironment());
  delete _fMutex;
}

SOM_Scope IODFileName
SOMLINK GetDocName(IODLinkService *somSelf,
                    Environment *ev)
{
  IODLinkServiceData *somThis = IODLinkServiceGetData(somSelf);
  IODLinkServiceMethodDebug("IODLinkService","GetDocName");
  return (IODFileName)_fDocPathName;
}
#endif

