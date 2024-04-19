/* @(#)Z 1.6 os2/src/storage/iavlsvr.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:01 (96/10/04 16:05:30) */
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

/*
1. Change _interrupt(3) to raise exception.
2. NOTE: If AS failed and restarted, all registered documents will be marked
*        as closed and all links are marked as UNLOADED.
*/

#ifndef SOM_Module_iavlsvr_Source
#define SOM_Module_iavlsvr_Source
#endif
#define IODAvailServer_Class_Source

#define METHOD_MACROS
#define VARIABLE_MACROS

#define INCL_WIN
#define INCL_ODAPI
#include <os2.h>

#include <Builtin.h>
#include <barray.h>
#include <storutil.h>
#include <stdtypio.h>
#include <ODMemory.h>
#include <ISOStr.h>
#include <IODDefs.xh>
#include <somd.xh>
#include <IAS2LSv.xh>
#include <link.xh>
#include <iciditr.xh>
#include <ILinkSrc.xh>
#include <StdTypes.xh>
#include <lmgrdefs.h>
#include <IODMutex.xh>
#include <ILnkSrvc.xh>
#include <CMDraft.xh>
#include <lmgrdefs.h>


/*
#ifndef _AEHSHTBL_
#include "AEHshTbl.h"
#endif
*/

#ifndef _OPENHASH_
#include "OpenHash.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#include "StorageU.xh"
#include "Draft.xh"
#include "AvsShell.h"

#include "IAvlSvr.xih"

const ODULong  kODInitialNumEntries = 8;

enum LinkSrcState
{
  UNLOADED,
  INDOC,
  INAS,
  INBOTH,
  REMOVED
};

typedef struct LinkIDNode
{
  ODLinkID    linkID;
  LinkIDNode* next;
} LinkIDNode;

typedef struct DocInfo
{
  ODISOStr                     pathName;
  LinkIDNode*                  linkIDs;
  IODAvailServerToLinkService* linkSrvc;
} DocInfo;

typedef struct LinkSrcInfo
{
  LinkSrcState               state;
  IODDocumentID              docID;
  ODStorageUnitID            auxStateSUID;
  ODBoolean                  isLocked;
  IODLinkTargetToLinkSource* asLinkSrc;
  IODLinkTargetToLinkSource* docLinkSrc;
} LinkSrcInfo;

/*
 *=============================================================================
 * CleanupDocInfo
 *
 * <1>  Delete pathName
 * <2>  Iterate through the list of linkIDs and delete each LinkIDNode entry.
 * <3>  Release the LinkService proxy.
 *=============================================================================
 */

void CleanupDocInfo(Environment* ev,
                    DocInfo* docInfo)
{
    if (ev->_major) ODSetSOMException(ev, kODNoError);
    LinkIDNode* nptr;

    //---<1>
    if (docInfo->pathName != (ODISOStr)kODNULL)
    {
      ODDisposePtr(docInfo->pathName);
    }

    //---<2>
    while (docInfo->linkIDs != (LinkIDNode*)kODNULL)
    {
      nptr = docInfo->linkIDs;
      docInfo->linkIDs = docInfo->linkIDs->next;
      nptr->next = (LinkIDNode*)kODNULL;
      delete nptr;
    }

    //---<3>
    if (docInfo->linkSrvc != (IODAvailServerToLinkService*)kODNULL)
    {
      if (((SOMDObject*)docInfo->linkSrvc)->is_proxy(ev))
      {
        SOMD_ObjectMgr->somdReleaseObject(ev, docInfo->linkSrvc);
      }
      else
      {
      }
    }
}

/*
 *=============================================================================
 * CleanupLinkSrcInfo
 *
 * <1>  Release LinkSource surrogate.
 * <2>  Release the LinkSource proxy.
 *=============================================================================
 */

void CleanupLinkSrcInfo(Environment* ev,
                        ODDraft* fSurrogateDraft,
                        LinkSrcInfo* lsInfo)
{
    if (ev->_major) ODSetSOMException(ev, kODNoError);

    //---<1>
    if (lsInfo->asLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
    {
      TRY
        ((CMDraft*)fSurrogateDraft)->
          RemoveLinkSource(ev, (ODLinkSource*)lsInfo->asLinkSrc);
      CATCH_ALL
      ENDTRY
    }

    //---<2>
    if (lsInfo->docLinkSrc != (IODLinkTargetToLinkSource*)kODNULL)
    {
      if (((SOMDObject*)lsInfo->docLinkSrc)->is_proxy(ev))
      {
         SOMD_ObjectMgr->somdReleaseObject(ev, lsInfo->docLinkSrc);
      }
      else
      {
      }
    }
}

/*
 *=============================================================================
 * ReleaseDocLinkSource
 *=============================================================================
 */

void ReleaseDocLinkSource(Environment* ev,
                          IODLinkTargetToLinkSource* linkSrc)
{
    if (ev->_major) ODSetSOMException(ev, kODNoError);

    if (linkSrc != (IODLinkTargetToLinkSource*)kODNULL)
    {
      if (((SOMDObject*)linkSrc)->is_proxy(ev))
      {
         SOMD_ObjectMgr->somdReleaseObject(ev, linkSrc);

      }
      else
      {
      }
    }
}

/*
 *=============================================================================
 * ReleaseLinkService
 *=============================================================================
 */

void ReleaseLinkService(Environment *ev,
                        IODAvailServerToLinkService* linkSrvc)
{
    if (ev->_major) ODSetSOMException(ev, kODNoError);

    if (linkSrvc != (IODAvailServerToLinkService*)kODNULL)
    {
      if (((SOMDObject*)linkSrvc)->is_proxy(ev))
      {
         SOMD_ObjectMgr->somdReleaseObject(ev, linkSrvc);
      }
      else
      {
      }
    }
}

/*
 *=============================================================================
 * SetDocumentsProp
 *
 * <1>  Focus to the given property and value.
 * <2>  Reserve space to write the total number of entries in the hashtable.
 * <3>  Iterate through each DocInfo entry of the hashtable.
 *      For each DocInfo entry, we are going to save the key, the pathName, and
 *        the list of linkIDs.
 * <4>  Write docID which is the key of the entry.
 * <5>  Get string length of pathName.
 *      Write string length of pathName.
 *      Write pathName.
 * <6>  Count the number of entries in the list of linkIDs.
 * <7>  Write the number of entries in the list of linkIDs.
 * <8>  Iterate through each entry of the list of linkIDs and write each
 *        linkID.
 * <9>  Count the number of entries in the hashtable.
 * <10> After iterating thru the hashtable, refocus to the given property and
 *      value.
 *      Write the total number of entries in the hashtable in the space
 *      reseved at <2>.
 *=============================================================================
 */

void SetDocumentsProp(Environment* ev,
                      ODStorageUnit* su,
                      ODPropertyName prop,
//                      AEHashTable &documents)
                      OpenHashTable &documents)
{
    ODULong       numOfEntries = 0;
    IODDocumentID docID;
    DocInfo*      docInfo;
    LinkIDNode*   nptr;
    ODULong       strLength;

    //---<1>
    ODSUForceFocus(ev, su, prop, kODHashTable);

    //---<2>
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfEntries);

    //---<3>
//    AEHashTableIterator iter(&documents);
//    for (iter.First(&docID, &docInfo);
//         iter.IsNotComplete();
//         iter.Next(&docID, &docInfo))

    OpenHashTableIterator iter(&documents);
    for (iter.First(&docID, &docInfo);
        iter.IsNotComplete();
        iter.Next(&docID, &docInfo))
    {
      ODULong       numOfLinkIDs = 0;
      //---<4>
      StorageUnitSetValue(su, ev, sizeof(IODDocumentID), (ODValue)&docID);

      //---<5>
      strLength = ODISOStrLength(docInfo->pathName);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su, ev, strLength+1, (ODValue)docInfo->pathName);

      //---<6>
      for (nptr = docInfo->linkIDs;
           nptr != (LinkIDNode*)kODNULL;
           nptr = nptr->next)
      {
        numOfLinkIDs++;
      }

      //---<7>
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfLinkIDs);

      //---<8>
      for (nptr = docInfo->linkIDs;
           nptr != (LinkIDNode*)kODNULL;
           nptr = nptr->next)
      {
        StorageUnitSetValue(su,
                            ev,
                            sizeof(ODLinkID),
                            (ODValue)&(nptr->linkID));
      }

      //---<9>
      numOfEntries++;
    }

    //---<10>
    su->Focus(ev, prop, kODPosSame, kODHashTable, 0, kODPosSame);
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfEntries);
}

/*
 *=============================================================================
 * GetDocumentsProp
 *
 * <1>  Focus to the given property and value.
 * <2>  Read in the total number of entries in the hashtable.
 * <3>  Loop to read in each entry of the hashtable.
 * <4>  Read in docID which will be used as the key of the entry in the
 *        hashtable.
 * <5>  Create a DocInfo entry.
 * <6>  Initilize linkIDs and linkSrvc fields of the DocInfo entry.
 * <7>  Read in the length of pathName.
 *      Allocate memory for pathName in the DocInfo entry.
 *      Read in pathName.
 * <8>  Read in the total number of entries in the list of linkIDs.
 * <9>  Loop to read in each entry of the list of linkIDs.
 * <10> Create a LinkIDNode entry.
 * <11> Read in the linkID.
 * <12> Insert the LinkIDNode entry into the list of linkIDs in DocInfo entry.
 * <13> Add the DocInfo entry to the hashtable.
 *=============================================================================
 */

void GetDocumentsProp(Environment* ev,
                      ODStorageUnit* su,
                      ODPropertyName prop,
//                      AEHashTable &documents)
                      OpenHashTable &documents)
{
    IODDocumentID docID;
    DocInfo*      docInfo;
    ODULong       i;
    ODULong       j;
    LinkIDNode*   linkIDNode;
    ODULong       numOfEntries;
    ODULong       numOfLinkIDs;
    ODULong       strLength;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, kODHashTable))
    {
      //---<2>
      StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&numOfEntries);

      //---<3>
      for (i = 0; i < numOfEntries; i++)
      {
        //---<4>
        StorageUnitGetValue(su, ev, sizeof(IODDocumentID), (ODValue)&docID);

        //---<5>
        docInfo = new DocInfo;

        //---<6>
        docInfo->linkIDs = (LinkIDNode*)kODNULL;
        docInfo->linkSrvc = (IODAvailServerToLinkService*)kODNULL;

        //---<7>
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        docInfo->pathName = (ODISOStr)ODNewPtr(strLength+1, kDefaultHeapID);
        StorageUnitGetValue(su,
                            ev,
                            strLength+1,
                            (ODValue)docInfo->pathName);

        //---<8>
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&numOfLinkIDs);

        //---<9>
        for (j = 0; j < numOfLinkIDs; j++)
        {
          //---<10>
          linkIDNode = new LinkIDNode;
          linkIDNode->next = (LinkIDNode*)kODNULL;

          //---<11>
          StorageUnitGetValue(su,
                              ev,
                              sizeof(ODLinkID),
                              (ODValue)&(linkIDNode->linkID));

          //---<12>
          if (docInfo->linkIDs == (LinkIDNode*)kODNULL)
          {
            docInfo->linkIDs = linkIDNode;
          }
          else
          {
            linkIDNode->next = docInfo->linkIDs;
            docInfo->linkIDs = linkIDNode;
          }
        }

        //---<13>
        documents.ReplaceEntry(&docID, &docInfo);
      }
    }
}

/*
 *=============================================================================
 * SetLinksProp
 *
 * <1>  Focus to the given property and value.
 * <2>  Reserve space to write the total number of entries in the hashtable.
 * <3>  Iterate through each LinkSrcInfo entry of the hashtable.
 *      For each LinkSrcInfo entry, we are going to save the key, docID and
 *        auxStateSUID.
 * <4>  Write linkID which is the key of the entry.
 * <5>  Write docID.
 * <6>  Get a weak reference from auxStateSUID.
 *      Write the weak reference.
 * <7>  Count the number of entries in the hashtable.
 * <8>  After iterating thru the hashtable, refocus to the given property and
 *      value.
 *      Write the total number of entries in the hashtable in the space
 *      reseved at <2>.
 *=============================================================================
 */

void SetLinksProp(Environment* ev,
                  ODStorageUnit* su,
                  ODPropertyName prop,
//                  AEHashTable &links)
                  OpenHashTable &links)
{
    ODULong          numOfEntries = 0;
    ODLinkID         linkID;
    LinkSrcInfo*     lsInfo;
    ODStorageUnitRef suRef;

    //---<1>
    ODSUForceFocus(ev, su, prop, kODHashTable);

    //---<2>
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfEntries);

    //---<3>
//    AEHashTableIterator iter(&links);
//    for (iter.First(&linkID, &lsInfo);
//         iter.IsNotComplete();
//         iter.Next(&linkID, &lsInfo))

    OpenHashTableIterator iter(&links);
    for (iter.First(&linkID, &lsInfo);
        iter.IsNotComplete();
        iter.Next(&linkID, &lsInfo))
    {
      if (lsInfo->state != REMOVED)
      {
        //---<4>
        StorageUnitSetValue(su, ev, sizeof(ODLinkID), (ODValue)&linkID);

        //---<5>
        StorageUnitSetValue(su,
                            ev,
                            sizeof(IODDocumentID),
                            (ODValue)&(lsInfo->docID));

        //---<6>
        su->GetWeakStorageUnitRef(ev, lsInfo->auxStateSUID, suRef);
        StorageUnitSetValue(su, ev, sizeof(ODStorageUnitRef), (ODValue)&suRef);

        //---<7>
        numOfEntries++;
      }
    }

    //---<8>
    su->Focus(ev, prop, kODPosSame, kODHashTable, 0, kODPosSame);
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfEntries);
}

/*
 *=============================================================================
 * GetLinksProp
 *
 * <1>  Focus to the given property and value.
 * <2>  Read in the total number of entries in the hashtable.
 * <3>  Loop to read in each entry of the hashtable.
 * <4>  Read in linkID which will be used as the key of the entry in the
 *        hashtable.
 * <5>  Create a LinkSrcInfo entry.
 * <6>  Initiliize the LinkSrcInfo entry as
 *        <UNLOADED, _____, _____, false, null, null>
 * <7>  Read in docID.
 * <8>  Read in the auxiliary state's SU reference.
 * <9>  Retrieve the SU ID of the auxiliary state's SU reference.
 *      The LinkSrcInfo entry now becomes
 *        <UNLOADED, docID, auxStateSUID, false, null, null>
 * <10> Add the LinkSrcInfo entry to the hashtable.
 *=============================================================================
 */

void GetLinksProp(Environment* ev,
                  ODStorageUnit* su,
                  ODPropertyName prop,
//                  AEHashTable &links)
                  OpenHashTable &links)
{
    IODDocumentID    docID;
    ODULong          i;
    ODLinkID         linkID;
    LinkSrcInfo*     lsInfo;
    ODULong          numOfEntries;
    ODStorageUnitRef suRef;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, kODHashTable))
    {
      //---<2>
      StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&numOfEntries);

      //---<3>
      for (i = 0; i < numOfEntries; i++)
      {
        //---<4>
        StorageUnitGetValue(su, ev, sizeof(ODLinkID), (ODValue)&linkID);

        //---<5>
        lsInfo = new LinkSrcInfo;

        //---<6>
        lsInfo->state = UNLOADED;
        lsInfo->isLocked = kODFalse;
        lsInfo->asLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;
        lsInfo->docLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;

        //---<7>
        StorageUnitGetValue(su,
                            ev,
                            sizeof(IODDocumentID),
                            (ODValue)&(lsInfo->docID));

        //---<8>
        StorageUnitGetValue(su,
                            ev,
                            sizeof(ODStorageUnitRef),
                            (ODValue)&suRef);

        //---<9>
        if (su->IsValidStorageUnitRef(ev, suRef))
        {
          lsInfo->auxStateSUID = su->GetIDFromStorageUnitRef(ev, suRef);
        }
        else
        {
        }

        //---<10>
        links.ReplaceEntry(&linkID, &lsInfo);
      }
    }
}

/*
 *=============================================================================
 * SetAuxStateToSU
 *
 * <1>  Write the version number.
 * <2>  Write idCounter (LinkID counter).
 * <3>  Write the remoteLinkInfos as a BArray.
 * <4>  If the new size is smaller than the old size, delete excess
 *=============================================================================
 */

void SetAuxStateToSU (Environment *ev,
                      ODStorageUnit *su,
                      IODAuxiliaryState& auxState)
{
    //---<1>
    ODSetULongProp(ev, su, kODPropVersion, kODULong, auxState.version);

    //---<2>
    ODSetULongProp(ev, su, kODPropIDCounter, kODULong, auxState.idCounter);

    //---<3>
    ODSUForceFocus(ev, su, kODPropRemoteLinkInfos, kODHashTable);
    ODULong oldsize = su->GetSize(ev);
    su->SetValue(ev, &(auxState.remoteLinkInfos));

    //---<4>
   if (oldsize - auxState.remoteLinkInfos._length > 0) {
      su->DeleteValue(ev, oldsize - auxState.remoteLinkInfos._length);
    }
}

/*
 *=============================================================================
 * GetAuxStateFromSU
 *
 * <1>  Read in version number.
 * <2>  Read in LinkID counter.
 * <3>  Read in remoteLinkInfos as a BArray.
 *=============================================================================
 */

void GetAuxStateFromSU (Environment *ev,
                        ODStorageUnit *su,
                        IODAuxiliaryState& auxState)
{
    TRY
      //---<1>
      if (ODSUExistsThenFocus(ev, su, kODPropVersion, kODULong))
      {
         StorageUnitGetValue(su,
                             ev,
                             sizeof(ODULong),
                             (ODValue)&auxState.version);
      }
      else
      {
         auxState.version = 0;
      }

      //---<2>
      if (ODSUExistsThenFocus(ev, su, kODPropIDCounter, kODULong))
      {
         StorageUnitGetValue(su,
                             ev,
                             sizeof(ODULong),
                             (ODValue)&auxState.idCounter);
      }
      else
      {
         auxState.idCounter = 0;
      }

      //---<3>
      if (ODSUExistsThenFocus(ev, su, kODPropRemoteLinkInfos, kODHashTable))
      {
        long size = su->GetSize(ev);
        auxState.remoteLinkInfos = CreateEmptyByteArrayStruct(size);
        auxState.remoteLinkInfos._length = size;
        su->GetValue(ev, size, &(auxState.remoteLinkInfos));
      }
      else
      {
        auxState.remoteLinkInfos = CreateEmptyByteArrayStruct(0);
      }
    CATCH_ALL
      RERAISE;
    ENDTRY
}

/*
 *=============================================================================
 *=============================================================================
 */

void SaveAuxState(Environment* ev,
                  AvsShell* shell,
                  ODDraft* asDraft,
                  LinkSrcInfo* lsInfo)
{
    IODAuxiliaryState* auxState = new IODAuxiliaryState;
    ((IODLinkSource*)lsInfo->asLinkSrc)->GetAuxState(ev, &auxState);
    ODStorageUnit* su = asDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
    SetAuxStateToSU(ev, su, *auxState);
    delete auxState;
//    shell->SaveDraft();
}


/*
 *=============================================================================
 *=============================================================================
 */
void SaveToAsDraft (Environment* ev,
                    AvsShell* shell,
                    ODDraft* asDraft,
                    ODULong docIDCounter,
                    ODULong linkIDCounter,
//                    AEHashTable* documents,
//                    AEHashTable* links)
                    OpenHashTable* documents,
                    OpenHashTable* links)
{
    TRY
      ODStorageUnit* su = asDraft->AcquireDraftProperties(ev);
      if (docIDCounter > 0)
      {
        ODSetULongProp(ev, su, kODPropDocIDCounter, kODULong, docIDCounter);
      }
      if (linkIDCounter > 0)
      {
        ODSetULongProp(ev, su, kODPropLinkIDCounter, kODULong, linkIDCounter);
      }
//      if (documents != (AEHashTable*)kODNULL)
      if (documents != (OpenHashTable*)kODNULL)
      {
        SetDocumentsProp(ev, su, kODPropDocuments, *documents);
      }
//      if (links != (AEHashTable*)kODNULL)
      if (links != (OpenHashTable*)kODNULL)
      {
        SetLinksProp(ev, su, kODPropLinks, *links);
      }
      su->Release(ev);
    CATCH_ALL
      RERAISE;
    ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::InitAvailServer
 *
 * InitAvailServer is called by the AS Shell to initialize the AS.
 * It supplies
 * the AS with 2 drafts.  asDraft contains the persistent storage used
 * by the AS: it contains the AS manager tables and the
 * SurrogateWriteableState for each LinkSource registered with this AS.
 * surrogateDraft is a temporary (in-memory) draft containing the
 * opened LinkSourceSurrogates and their referenced contentSUs.
 *
 * This method doesn't need to use a semaphore..
 * Unless otherwise noted, all methods need to seize a semaphore on entry,
 * and release it on exit, since we want to serialize access to the tables.
 *
 * <1>  Verify that input shell is non-null.
 * <2>  Verify that input drafts are non-null.
 * <3>  Save input shell and drafts.
 * <4>  Init fDocIDCounter and fLinkIDCounter to 0.
 * <5>  Create fMutex.
 * <6>  Create fDocuments.
 * <7>  Create fLinks.
 * <8>  Obtain properties of fAsDraft.
 * <9>  AvailServer is new.
 *      Add PropDocuments to fAsDraft's properties.
 *      Add PropLinks to fAsDraft's properties.
 *      Add PropDocIDCounter to fAsDraft's properties.
 *      Add PropLinkIDCounter to fAsDraft's properties.
 * <10> AvailServer is not new.
 *      Internalize fDocuments from fAsDraft's PropDocuments.
 *      Internalize fLinks from fAsDraft's PropLinks.
 *      Internalize fDocIDCounter from fAsDraft's PropDocIDCounter.
 *      Internalize fLinkIDCounter from fAsDraft's PropLinkIDCounter.
 * <11> Release properties of fAsDraft.
 *      Return.
 * <12>  An exception is caught!
 *      Delete fMutex.
 * <13> Cleanup and delete fDocuments.
 * <14> Cleanup and delete fLinks.
 * <15> Reset fShell, fAsDraft, fSurrogateDraft, fDocIDCounter and
 *        fLinkIDCounter.
 * <16> Release properties of fAsDraft.
 * <17> Set SOM ev to ODError.
 *      Return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK InitAvailServer(IODAvailServer *somSelf,
                        Environment *ev,
                        ODBoolean isNew,
                        AvsShell* shell,
                        ODDraft* asDraft,
                        ODDraft* surrogateDraft)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","InitAvailServer");

    IODDocumentID  docID;
    DocInfo*       docInfo;
    ODLinkID       linkID;
    LinkSrcInfo*   lsInfo;
    ODStorageUnit* su;

    if (ev->_major) ODSetSOMException(ev, kODNoError);

    //---<1>
    if (shell == (AvsShell*)kODNULL)
    {
      ODSetSOMException(ev, kODErrInvalidShell);
      return;
    }

    //---<2>
    if ((asDraft == (ODDraft*)kODNULL) ||
        (surrogateDraft == (ODDraft*)kODNULL))
    {
      ODSetSOMException(ev, kODErrInvalidDraft);
      return;
    }

    SOM_TRY

      //---<3>
      _fShell          = shell;
      _fAsDraft        = asDraft;
      _fSurrogateDraft = surrogateDraft;

      //---<4>
      _fDocIDCounter = 0;
      _fLinkIDCounter = 0;

      //---<5>
      _fMutex = new IODMutex;
      THROW_IF_NULL(_fMutex);

      //---<6>
//      _fDocuments = new AEHashTable;
//      THROW_IF_NULL(_fDocuments);
//      _fDocuments->Initialize(kODInitialNumEntries,
//                              sizeof(IODDocumentID),
//                              sizeof(DocInfo*),
//                              kODFalse);
//
      _fDocuments = new OpenHashTable;
      THROW_IF_NULL(_fDocuments);
      _fDocuments->Initialize(kODInitialNumEntries,
                              sizeof(IODDocumentID),
                              sizeof(DocInfo*),
                              kODFalse);

      //---<7>
//      _fLinks = new AEHashTable;
//      THROW_IF_NULL(_fLinks);
//      _fLinks->Initialize(kODInitialNumEntries,
//                          sizeof(ODLinkID),
//                          sizeof(LinkSrcInfo*),
//                          kODFalse);

      _fLinks = new OpenHashTable;
      THROW_IF_NULL(_fLinks);
      _fLinks->Initialize(kODInitialNumEntries,
                          sizeof(ODLinkID),
                          sizeof(LinkSrcInfo*),
                          kODFalse);
      //---<8>
      su = _fAsDraft->AcquireDraftProperties(ev);

      if (isNew)
      {
        //---<9>
        ODSUAddPropValue(ev, su, kODPropDocuments, kODHashTable);
        ODSUAddPropValue(ev, su, kODPropLinks, kODHashTable);
        ODSUAddPropValue(ev, su, kODPropDocIDCounter, kODULong);
        ODSUAddPropValue(ev, su, kODPropLinkIDCounter, kODULong);
        ODSetULongProp(ev, su, kODPropDocIDCounter, kODULong, _fDocIDCounter);
        ODSetULongProp(ev, su, kODPropLinkIDCounter, kODULong, _fLinkIDCounter);
        SetDocumentsProp(ev, su, kODPropDocuments, *_fDocuments);
        SetLinksProp(ev, su, kODPropLinks, *_fLinks);
      }
      else
      {
        //---<10>
        GetDocumentsProp(ev, su, kODPropDocuments, *_fDocuments);
        GetLinksProp(ev, su, kODPropLinks, *_fLinks);
        _fDocIDCounter = ODGetULongProp(ev, su, kODPropDocIDCounter, kODULong);
        _fLinkIDCounter = ODGetULongProp(ev, su, kODPropLinkIDCounter, kODULong);
      }

      //---<11>
      su->Release(ev);
      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      //---<12>
      if (_fMutex != kODNULL)
      {
        delete _fMutex;
        _fMutex = (IODMutex*)kODNULL;
      }

      //---<13>
//      if (_fDocuments != (AEHashTable*)kODNULL)
      if (_fDocuments != (OpenHashTable*)kODNULL)
      {
//        AEHashTableIterator iter(_fDocuments);
        OpenHashTableIterator iter(_fDocuments);
        for (iter.First(&docID, &docInfo);
             iter.IsNotComplete();
             iter.Next(&docID, &docInfo))
        {
          CleanupDocInfo(ev, docInfo);
          delete docInfo;
        }

        delete _fDocuments;
//        _fDocuments = (AEHashTable*)kODNULL;
        _fDocuments = (OpenHashTable*)kODNULL;
      }

      //---<14>
//      if (_fLinks != (*AEHashTable*)kODNULL)
      if (_fLinks != (OpenHashTable*)kODNULL)
      {
//        AEHashTableIterator iter(_fLinks);
        OpenHashTableIterator iter(_fLinks);
        for (iter.First(&linkID, &lsInfo);
             iter.IsNotComplete();
             iter.Next(&linkID, &lsInfo))
        {
          CleanupLinkSrcInfo(ev, _fSurrogateDraft, lsInfo);
          delete lsInfo;
        }

        delete _fLinks;
//        _fLinks = (AEHashTable*)kODNULL;
        _fLinks = (OpenHashTable*)kODNULL;
      }

      //---<15>
      _fShell = (AvsShell*)kODNULL;
      _fAsDraft = (ODDraft*)kODNULL;
      _fSurrogateDraft = (ODDraft*)kODNULL;
      _fDocIDCounter = 0;
      _fLinkIDCounter = 0;

      //--<16>
      if (su != (ODStorageUnit*)kODNULL)
      {
        su->Release(ev);
      }

      //---<17>
      ODSetSOMException(ev, exception);
      return;

    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::IODAvailServer

    //- Initialize all dynamic storage to NULL.
    //- This method doesn't need to use a semaphore.

 * <1> Initialize the following private variables:
 *       IODMutex      fMutex;
 *       AEHashTable*  fDocuments;
 *       AEHashTable*  fLinks;
 *       IODDocumentID fDocIDCounter;
 *       ODLinkID      fLinkIDCounter;
 *       AvsShell*      fShell;
 *       ODDraft*      fAsDraft;
 *       ODDraft*      fSurrogateDraft;
 *=============================================================================
 */

SOM_Scope void
SOMLINK somInit(IODAvailServer *somSelf)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","somInit");

    IODAvailServer_parent_IODLinkServiceToAvailServer_somInit(somSelf);
    IODAvailServer_parent_IODLinkTargetToLinkSource_somInit(somSelf);

    //---<1>
    _fMutex          = (IODMutex*)kODNULL;
//    _fDocuments      = (AEHashTable*)kODNULL;
//    _fLinks          = (AEHashTable*)kODNULL;
    _fDocuments      = (OpenHashTable*)kODNULL;
    _fLinks          = (OpenHashTable*)kODNULL;
    _fDocIDCounter   = 0;
    _fLinkIDCounter  = 0;
    _fShell          = (AvsShell*)kODNULL;
    _fAsDraft        = (ODDraft*)kODNULL;
    _fSurrogateDraft = (ODDraft*)kODNULL;
}

/*
 *=============================================================================
 * IODAvailServer::~IODAvailServer
 *
 * <1>  Delete fMutex.
 * <2>  Delete fDocuments.
 * <3>  Delete fLinks.
 * <4>  Reset fDocIDCounter and fLinkIDCounter.
 * <5>  Reset fShell, fAsDraft and fSurrogateDraft.
 *=============================================================================
 */

SOM_Scope void
SOMLINK somUninit(IODAvailServer *somSelf)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","somUninit");

    IODDocumentID docID;
    DocInfo*      docInfo;
    ODLinkID      linkID;
    LinkSrcInfo*  lsInfo;

    //---<1>
    if (_fMutex != kODNULL)
    {
      delete _fMutex;
      _fMutex = (IODMutex*)kODNULL;
    }

    //---<2>
//    if (_fDocuments != (AEHashTable*)kODNULL)
    if (_fDocuments != (OpenHashTable*)kODNULL)
    {
//      AEHashTableIterator iter(_fDocuments);
      OpenHashTableIterator iter(_fDocuments);
      for (iter.First(&docID, &docInfo);
           iter.IsNotComplete();
           iter.Next(&docID, &docInfo))
      {
        CleanupDocInfo(somGetGlobalEnvironment(), docInfo);
        delete docInfo;
      }
      delete _fDocuments;
//      _fDocuments = (AEHashTable*)kODNULL;
      _fDocuments = (OpenHashTable*)kODNULL;
    }

    //---<3>
//     if (_fLinks != (AEHashTable*)kODNULL)
     if (_fLinks != (OpenHashTable*)kODNULL)
    {
//      AEHashTableIterator iter(_fLinks);
      OpenHashTableIterator iter(_fLinks);
      for (iter.First(&linkID, &lsInfo);
           iter.IsNotComplete();
           iter.Next(&linkID, &lsInfo))
      {
        CleanupLinkSrcInfo(somGetGlobalEnvironment(), _fSurrogateDraft, lsInfo);
        delete lsInfo;
      }
       delete _fLinks;
//      _fLinks = (AEHashTable*)kODNULL;
      _fLinks = (OpenHashTable*)kODNULL;
    }

    //---<4>
    _fDocIDCounter = 0;
    _fLinkIDCounter = 0;

    //---<5>
    _fShell          = (AvsShell*)kODNULL;
    _fAsDraft        = (ODDraft*)kODNULL;
    _fSurrogateDraft = (ODDraft*)kODNULL;

    IODAvailServer_parent_IODLinkServiceToAvailServer_somUninit(somSelf);
    IODAvailServer_parent_IODLinkTargetToLinkSource_somUninit(somSelf);
}

/*
 *=============================================================================
 * IODAvailServer::RegisterDoc
 *
 * RegisterDoc is called when a new document is opened.
 *
 * <1>  Acquire fMutex before accessing fDocIDCounter and fDocuments.
 * <2>  Generate new document ID and update fDocIDCounter.
 * <3>  Create a DocInfo entry.
 * <4>  Initialize the entry as <docPathName, null, linkService>.
 * <5>  Add that entry to fDocuments.
 * <5.1> Save fDocIDCounter and fDocuments to fASDraft.
 * <6>  Release fMutex.
 *      Return the new document ID.
 * <7>  An exception is caught!
 *      Remove the newly added entry from fDocuments.
 *      Cleanup and delete that entry.
 * <8>  Release fMutex.
 * <9>  Set SOM ev to ODError.
 *      Return 0 as the document ID.
 *=============================================================================
 */

SOM_Scope IODDocumentID
SOMLINK RegisterDoc(IODAvailServer *somSelf,
                    Environment *ev,
                    IODFileName docPathName,
                    IODAvailServerToLinkService* linkService)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","RegisterDoc");

    IODDocumentID docID;
    DocInfo*      docInfo;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return 0;
    SOM_ENDTRY

    //---<2>
    docID = ++_fDocIDCounter;

    SOM_TRY
      //---<3>
      docInfo = new DocInfo;
      THROW_IF_NULL(docInfo);

      //---<4>
      docInfo->pathName = ODISOStrFromCStr(docPathName);
      docInfo->linkIDs = kODNULL;
      docInfo->linkSrvc = linkService;

      //---<5>
      _fDocuments->ReplaceEntry(&docID, &docInfo);

      //---<5.1>
      SaveToAsDraft(ev,
                    _fShell,
                    _fAsDraft,
                    _fDocIDCounter,
                    0,
                    _fDocuments,
//                    (AEHashTable*)kODNULL);
                    (OpenHashTable*)kODNULL);

      _fShell->UpdateDocList(_fDocIDCounter, docPathName, 'o');

      //---<6>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return docID;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        //---<7>
        if (docInfo != (DocInfo*)kODNULL)
        {
          _fDocuments->RemoveEntry(&docID);
          CleanupDocInfo(ev, docInfo);
          delete docInfo;
        }

        //---<8>
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      //---<9>
      ODSetSOMException(ev, exception);
      return 0;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::UnregisterDoc
 *
 * UnregisterDoc is called when a document is destroyed.
 *
 * <1>  Acquire fMutex before accessing fDocuments and fLinks.
 * <2>  Check if docID is associated with a registered document.
 *      If true retrieve and remove the DocInfo entry of docID from
 *      fDocuments.
 * <3>  Iterate through the list of linkID's in the DocInfo entry.
 * <4>  Retrieve the LinkSrcInfo entry of each linkID from fLinks.
 * <5>  Check the state of the LinkSrcInfo entry.
 * <6>  If state is UNLOADED:
 *        Remove the storage unit of the auxiliary state.
 * <7>  If state is INDOC:
 *        Remove the storage unit of the auxiliary state.
 *        Release LinkSource proxy.
 * <8>  If state is INAS:
 *        Remove the storage unit of the auxiliary state.
 *        Release LinkSource surrogate.
 * <9>  If state is INBOTH:
 *        Change state to REMOVED.
 *        Remove the storage unit of the auxiliary state.
 *        Release LinkSource proxy.
 *      NOTE: When the content SU is unlocked by the LinkTarget, the
 *            LinkSource surrogate will be released and the LinkSrcInfo
 *            entry will be deleted.
 * <10> If state of LinkSrcInfo entry is not REMOVED
 *        Remove the LinkSrcInfo entry from fLinks.
 *        Delete the LinkSrcInfo entry.
 * <11> Done interating through the list of linkID's in the DocInfo entry.
 *      Cleanup and delete the DocInfo entry.
 * <11.1> Save fDocuments and fLinks to AsDraft.
 * <12> Release fMutex.
 *      Return.
 * <13> Fail testing in <2>: docID is invalid.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK UnregisterDoc(IODAvailServer *somSelf,
                      Environment *ev,
                      IODDocumentID docID)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","UnregisterDoc");

    DocInfo*       docInfo;
    ODLinkID       linkID;
    LinkSrcInfo*   lsInfo;
    LinkIDNode*    nptr;
    ODStorageUnit* su;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    //---<2>
    if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
    {
      _fDocuments->RemoveEntry(&docID);

      //---<3>
      for (nptr = docInfo->linkIDs;
           nptr != (LinkIDNode*)kODNULL;
           nptr = nptr->next)
      {
        //---<4>
        linkID = nptr->linkID;
        if (_fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
        {
          TRY
            //---<5>
            switch (lsInfo->state)
            {
              case UNLOADED:
                //---<6>
                //Before:<UNLOADED, docID, suID, false, null, null>
                //After :<UNLOADED, docID, ____, false, null, null>
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                _fAsDraft->RemoveStorageUnit(ev, su);
                break;

              case INDOC:
                //---<7>
                //Before:<INDOC, docID, suID, false, null, docLS>
                //After :<INDOC, docID, ____, false, null, _____>
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                _fAsDraft->RemoveStorageUnit(ev, su);
                ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                break;

              case INAS:
                //---<8>
                //Before:<INAS, docID, suID, false, asLS, null>
                //After :<INAS, docID, ____, false, ____, null>
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                _fAsDraft->RemoveStorageUnit(ev, su);
                TRY
                  ((CMDraft*)_fSurrogateDraft)->
                    RemoveLinkSource(ev, (ODLinkSource*)lsInfo->asLinkSrc);
                CATCH_ALL
                ENDTRY
                break;

              case INBOTH:
                //---<9>
                //Before:<INBOTH,  docID, suID, true, asLS, docLS>
                //After :<REMOVED, docID, ____, true, asLS, _____>
                lsInfo->state = REMOVED;
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                _fAsDraft->RemoveStorageUnit(ev, su);
                ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                break;
            }
          CATCH_ALL
            // ignore exceptions caught when making calls on _fAsDraft
          ENDTRY

          //---<10>
          if (lsInfo->state != REMOVED)
          {
            _fLinks->RemoveEntry(&linkID);
            delete lsInfo;
          }
        }
      }

      TRY
        //---<11>
        CleanupDocInfo(ev, docInfo);
        delete docInfo;

        //---<11.1>
        SaveToAsDraft(ev, _fShell, _fAsDraft, 0, 0, _fDocuments, _fLinks);

        //---<12>
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;
    }
    else
    {
      //---<13>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY
      ODSetSOMException(ev, kODErrInvalidDocID);
      return;
    }
}

/*
 *=============================================================================
 * IODAvailServer::DocOpened
 *
 * DocOpened is called when a document already registered with the AS
 * is opened.  If the path name does not match the path name registered
 * with that document id, then an exception is returned and the document
 * is not opened.
 *
 * <1>  Acquire fMutex before accessing fDocuments and fLinks.
 * <2>  Check if docID is associated with a registered document.
 *      If true retrieve the DocInfo entry of docID from fDocuments.
 * <3>  Check if given path name of document matches the one given at register
 *      time.
 * <4>  Check if document is closed.
 * <5>  Normal case: document is just opened.
 *      Save given LinkService.
 *      Release fMutex.
 *      Return true.
 *XXXXX <6>  Fail testing in <4>: document is already opened.
 *XXXXX      Check if document is crashed and restarted (by comparing given
 *XXXXX      LinkService with the existing one).
 * <7>  Document is crashed and restarted.
 *      Release the proxy of old LinkService.
 *      Save given LinkService.
 * <8>  Iterate through the list of linkID's in the DocInfo entry.
 * <9>  Retrieve the LinkSrcInfo entry of each linkID from fLinks.
 * <10> Check the state of the LinkSrcInfo entry.
 * <11> If state is UNLOADED:
 *        No change.
 * <12> If state is INDOC:
 *        Change state to UNLOADED.
 *        Release LinkSource proxy.
 *        Reset docLinkSrc in LinkSrcInfo entry to null.
 * <13> If state is INAS:
 *        No change.
 * <14> If state is INBOTH:
 *        Change state to INAS.
 *        Release LinkSource proxy.
 *        Reset docLinkSrc in LinkSrcInfo entry to null.
 * <15> Done iterating through the list of linkID's in DocInfo entry.
 *      Release fMutex.
 *      Return true.
 * <16> Fail testing in <6>: document is opened more than once
 *        (Nothing has changed)
 *      Release fMutex.
 *      Return true.
 * <17> Fail testing in <3>: given path name of document does not match the
 *        one given at register time.
 *      Release fMutex.
 *      Raise exception.
 *      Return false.
 * <18> Fail testing in <2>: docID is invalid. Automatically register the
 *      document and reuse docID.
 *      Increase _fDocIDCounter if it is not already greater than or equal to
 *      docID.
 *      The logic of the codes is identical to RegisterDoc except here returns
 *      a boolean rather than the docID.
 *=============================================================================
 */

SOM_Scope ODBoolean
SOMLINK DocOpened(IODAvailServer *somSelf,
                  Environment *ev,
                  IODDocumentID docID,
                  IODFileName docPathName,
                  IODAvailServerToLinkService* linkService,
                  IODFileName* oldDocPathName)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","DocOpened");

    DocInfo*       docInfo;
    ODLinkID       linkID;
    LinkSrcInfo*   lsInfo;
    LinkIDNode*    nptr;
    ODStorageUnit* su;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return kODFalse;
    SOM_ENDTRY

    //---<2>
    if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
    {
      //---<3>
      if (ODISOStrCompare(docInfo->pathName, docPathName) == 0)
      {
        //---<4>
        if (docInfo->linkSrvc == (IODAvailServerToLinkService*)kODNULL)
        {
          //---<5>
          docInfo->linkSrvc = linkService;

          _fShell->UpdateDocList(docID, docPathName, 'o');

          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          return kODTrue;
        }
        else
        {
          //---<6>
          if (docInfo->linkSrvc != linkService)
          {
            //---<7>
            ReleaseLinkService(ev, docInfo->linkSrvc);
            docInfo->linkSrvc = linkService;

            //---<8>
            for (nptr = docInfo->linkIDs;
                 nptr != (LinkIDNode*)kODNULL;
                 nptr = nptr->next)
            {
              //---<9>
              linkID = nptr->linkID;
              if (_fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
              {
                //---<10>
                switch (lsInfo->state)
                {
                  case UNLOADED:
                    //---<11>
                    // Before:<UNLOADED, docID, suID, false, null, null>
                    // After :<UNLOADED, docID, suID, false, null, null>
                    // NO CHANGE
                    break;

                  case INDOC:
                    //---<12>
                    // Before:<INDOC,    docID, suID, false, null, docLS>
                    // After :<UNLOADED, docID, suID, false, null, null>
                    lsInfo->state = UNLOADED;
                    ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                    lsInfo->docLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;

                    _fShell->UpdateLinkList(linkID, docID, UNLOADED);

                    break;

                  case INAS:
                    //---<13>
                    // Before:<INAS, docID, suID, true/false, asLS, null>
                    // After :<INAS, docID, suID, true/false, asLS, null>
                    // NO CHANGE
                    break;

                  case INBOTH:
                    //---<14>
                    // Before:<INBOTH, docID, suID, true, asLS, docLS>
                    // After :<INAS,   docID, suID, true, asLS, null>
                    lsInfo->state = INAS;
                    ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                    lsInfo->docLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;

                    _fShell->UpdateLinkList(linkID, docID, INAS);

                    break;
                }
              }
            }
            //---<15>
            _fShell->UpdateDocList(docID, docPathName, 'o');

            TRY
              _fMutex->Release(ev);
            CATCH_ALL
            ENDTRY

            return kODTrue;
          }
          else
          {
            //---<16>
            TRY
              _fMutex->Release(ev);
            CATCH_ALL
            ENDTRY

            return kODTrue;
          }
        }
      }
      else
      {
        //---<17>
        *oldDocPathName = ODISOStrFromCStr(docInfo->pathName);

        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        ODSetSOMException(ev, kODErrInvalidDocPathName);
        return kODFalse;
      }
    }
    else
    {
      //---<18>
      if (docID > _fDocIDCounter)
      {
        _fDocIDCounter++;
      }

      SOM_TRY
        docInfo = new DocInfo;
        THROW_IF_NULL(docInfo);

        docInfo->pathName = ODISOStrFromCStr(docPathName);
        docInfo->linkIDs = kODNULL;
        docInfo->linkSrvc = linkService;

        _fDocuments->ReplaceEntry(&docID, &docInfo);

        SaveToAsDraft(ev,
                      _fShell,
                      _fAsDraft,
                      _fDocIDCounter,
                      0,
                      _fDocuments,
//                      (AEHashTable*)kODNULL);
                      (OpenHashTable*)kODNULL);

        _fShell->UpdateDocList(_fDocIDCounter, docPathName, 'o');

        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        return kODTrue;

      SOM_CATCH_ALL
        ODError exception = ODGetSOMException(ev);

        TRY
          if (docInfo != (DocInfo*)kODNULL)
          {
            _fDocuments->RemoveEntry(&docID);
            CleanupDocInfo(ev, docInfo);
            delete docInfo;
          }

          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        ODSetSOMException(ev, exception);
        return kODFalse;
      SOM_ENDTRY
    }
}

/*
 *=============================================================================
 * IODAvailServer::DocClosed
 *
 * DocClosed is called when an opened document is closed.
 *
 * [NOTE:  We assume that before this call is issued, each link that was
 * opened has been called.  Otherwise we would need to:
 * For each link associated with this document (in its fDocument entry)
 * make sure that it is not in fLoadedLinks or that its status (in
 * fLoadedLinks) is not InDoc or InBoth.
 *
 * <1>  Acquire fMutex before accessing fDocuments.
 * <2>  Check if docID is associated with a registered document.
 *      If true retrieve the DocInfo entry of docID from fDocuments.
 * <3>  Check if document is opened.
 * <4>  Document is opened.
 *      Release LinkService proxy saved in the DocInfo entry.
 *      Set linkSrvc in DocInfo entry to null.
 *      Release fMutex.
 *      Return.
 * <5>  Fail testing in <3>: document is not opened.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <6>  Fail testing in <2>: docID is invalid.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK DocClosed(IODAvailServer *somSelf,
                  Environment *ev,
                  IODDocumentID docID)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","DocClosed");

    DocInfo* docInfo;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    //---<2>
    if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
    {
      //---<3>
      if (docInfo->linkSrvc != (IODAvailServerToLinkService*)kODNULL)
      {
        //---<4>
        ReleaseLinkService(ev, docInfo->linkSrvc);
        docInfo->linkSrvc = (IODAvailServerToLinkService*)kODNULL;

        _fShell->UpdateDocList(docID, docInfo->pathName, 'c');

        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        return;
      }
      else
      {
        //---<5>
        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

//        ODSetSOMException(ev, kODErrDocNotOpened);
        return;
      }
    }
    else
    {
      //---<6>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, kODErrInvalidDocID);
      return;
    }
}

/*
 *=============================================================================
 * IODAvailServer::DocMoved
 *
 * DocMoved is called when a registered document's persistent storage
 * changes from oldPathName to newPathName.
 *
 * <1>  Acquire fMutex before accessing fDocuments.
 * <2>  Check if docID is associated with a registered document.
 *      If true retrieve the DocInfo entry of docID from fDocuments.
 * <3>  Check if given old path name of document matches the one given at
 *      register time.
 * <4>  Document path name matched.
 *      Delete old path name in the DocInfo entry.
 *      Save new path name.
 *      Save fDocuments to AsDraft.
 *      Release fMutex.
 *      Return.
 * <5>  Fail testing in <3>: old path name does not match.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <6>  Fail testing in <2>: docID is invalid.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK DocMoved(IODAvailServer *somSelf,
                 Environment *ev,
                 IODDocumentID docID,
                 IODFileName oldPathName,
                 IODFileName newPathName)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","DocMoved");

    DocInfo* docInfo;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    //---<2>
    if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
    {
      //---<3>
      if (ODISOStrCompare(docInfo->pathName, oldPathName) == 0)
      {
        SOM_TRY
          //---<4>
          ODDisposePtr(docInfo->pathName);
          docInfo->pathName = ODISOStrFromCStr(newPathName);

          SaveToAsDraft(ev,
                        _fShell,
                        _fAsDraft,
                        0,
                        0,
                        _fDocuments,
//                        (AEHashTable*)kODNULL);
                        (OpenHashTable*)kODNULL);

          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          return;

        SOM_CATCH_ALL
          ODError exception = ODGetSOMException(ev);

          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, exception);
          return;
        SOM_ENDTRY
      }
      else
      {
        //---<5>
        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        ODSetSOMException(ev, kODErrInvalidDocPathName);
        return;
      }
    }
    else
    {
      //---<6>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, kODErrInvalidDocID);
      return;
    }
}

/*
 *=============================================================================
 * IODAvailServer::RegisterLink
 *
 * RegisterLink is called when a new LinkSource has been
 * created in the document.  It provides a pointer to the LinkSource so
 * the AS may forward calls to it.  The AS returns a unique (within the
 * AS) ID for this LinkSource.  (LinkOpened need not be called).

    // 1. Check that the document is open.
    //    If not, this could be either a caller error
    //    or an indication that the AS has crashed and restarted
    //    losing the opening of the document. The LinkService
    //    has the obligation to recover from this condition.
    //    (by calling DocOpened, and asking each link source to
    //    re-issue LinkOpened, then finally re-issuing RegisterLink.)
    // 2. Create a new LinkId.
    // 3. Ask the AsDraft to create a new SU for this LinkId.
    // 4. Initialize the SU to version 0, empty Auxiliary State.
    // 5. Insert a new entry in fLinks.
    // 6. Insert a new entry in fLoadedLinks, with status inDoc.
    // 7. Optionally save the AsDraft.
    // 8. Return the LinkId.

 * <1>  Acquire fMutex before accessing fDocuments and fLinks.
 * <2>  Check if docID is associated with a registered document.
 *      If true retrieve the DocInfo entry of docID from fDocuments.
 * <3>  Check if document is opened.
 * <4>  Document is opened.
 *      Generate new linkID and update fLinkIDCounter.
 * <5>  Ask fAsDraft to create a new SU to store the auxiliary state.
 *      Add PropVersion to SU.
 *      Add PropIDCounter to SU.
 *      Add PropRemoteLinkInfos to SU.
 * <6>  Create a LinkSrcInfo entry.
 * <7>  Initialize the entry as <INDOC, docID, su->GetID, false, null, linkSrc>
 * <8>  Add that entry to fLinks.
 * <9>  Create a LinkIDNode entry.
 * <10> Initialize the entry with the linkID.
 * <11> Insert that entry to the list of link IDs in docInfo.
 * <11.1> Save fLinkIDCounter, fDocuments and fLinks to AsDraft.
 * <12> Relase fMutex.
 *      Return linkID.
 * <13> An exception is caught!
 *      Ask fAsDraft to remove the SU created earlier for the auxiliary state.
 * <14> Remove the newly added LinkSrcInfo entry from fLinks.
 *      Cleanup that entry.
 *      Delete that entry.
 * <15> Release fMutex.
 * <16> Set SOM ev to ODError.
 *      Return 0 as linkID.
 * <17> Fail testing in <3>: document is not opened.
 *      Release fMutex.
 *      Raise exception
 *      Return 0 as linkID
 * <18> Fail testing in <2>: docID is invalid.
 *      Release fMutex.
 *      Raise exception
 *      Return 0 as linkID
 *=============================================================================
 */

SOM_Scope ODLinkID
SOMLINK RegisterLink(IODAvailServer *somSelf,
                     Environment *ev,
                     IODDocumentID docID,
                     IODLinkTargetToLinkSource* linkSrc)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","RegisterLink");

    DocInfo*       docInfo;
    ODLinkID       linkID;
    LinkIDNode*    linkIDNode;
    LinkSrcInfo*   lsInfo;
    ODStorageUnit* su;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return 0;
    SOM_ENDTRY

    //---<2>
    if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
    {
      //---<3>
      if (docInfo->linkSrvc != (IODAvailServerToLinkService*)kODNULL)
      {
        //---<4>
        linkID = ++_fLinkIDCounter;

        SOM_TRY
          //---<5>
          su = _fAsDraft->CreateStorageUnit(ev);
          ODSUAddPropValue(ev, su, kODPropVersion, kODULong);
          ODSUAddPropValue(ev, su, kODPropIDCounter, kODULong);
          ODSUAddPropValue(ev, su, kODPropRemoteLinkInfos, kODHashTable);

          //---<6>
          lsInfo = new LinkSrcInfo;
          THROW_IF_NULL(lsInfo);

          //---<7>
          lsInfo->state = INDOC;
          lsInfo->docID = docID;
          lsInfo->auxStateSUID = su->GetID(ev);
          lsInfo->isLocked = kODFalse;
          lsInfo->asLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;
          lsInfo->docLinkSrc = linkSrc;

          //---<8>
          _fLinks->ReplaceEntry(&linkID, &lsInfo);

          //---<9>
          linkIDNode = new LinkIDNode;
          THROW_IF_NULL(linkIDNode);

          //---<10>
          linkIDNode->linkID = linkID;
          linkIDNode->next = (LinkIDNode*)kODNULL;

          //---<11>
          if (docInfo->linkIDs == (LinkIDNode*)kODNULL)
          {
            docInfo->linkIDs = linkIDNode;
          }
          else
          {
            linkIDNode->next = docInfo->linkIDs;
            docInfo->linkIDs = linkIDNode;
          }

          //---<11.1>
          SaveToAsDraft(ev,
                        _fShell,
                        _fAsDraft,
                        0,
                        _fLinkIDCounter,
                        _fDocuments,
                        _fLinks);

          _fShell->UpdateLinkList(linkID, docID, INDOC);

          //---<12>
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          return linkID;

        SOM_CATCH_ALL
          ODError exception = ODGetSOMException(ev);

          TRY
            //---<13>
            if (su != (ODStorageUnit*)kODNULL)
            {
              _fAsDraft->RemoveStorageUnit(ev, su);
            }

            //---<14>
            if (lsInfo != (LinkSrcInfo*)kODNULL)
            {
              _fLinks->RemoveEntry(&linkID);
              CleanupLinkSrcInfo(ev, _fSurrogateDraft, lsInfo);
              delete lsInfo;
            }

            //---<15>
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          //---<16>
          ODSetSOMException(ev, exception);
          return 0;

          // Note:
          // No need to cleanup docInfo->linkIDs because if
          //   linkIDNode = new LinkIDNode;
          // raises an exception then it will not be added to docInfo->linkIDs
        SOM_ENDTRY
      }
      else
      {
        //---<17>
        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        ODSetSOMException(ev, kODErrDocNotOpened);
        return 0;
      }
    }
    else
    {
      //---<18>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, kODErrInvalidDocID);
      return 0;
    }
}

/*
 *=============================================================================
 * IODAvailServer::LinkOpened
 *
 * LinkOpened is called when an existing LinkSource is internalized in
 * a document.  It provides a pointer to the LinkSource so
 * the AS may forward calls to it.  The AS returns the current
 * AuxiliaryState associated with this LinkSource, and whether some
 * target currently holds a lock on this LinkSource ContentSU.

    // 1. Find out what docid is associated with this linkid. (fLinks)
    //    If there is no entry in fLinks, this is either a caller
    //    error, or the AS crashed and failed to save the registration
    //    of this linkid.  Either way, return an unregistered exception.
    //    This is a major disaster,
    //    so the AS should save early and often.
    // 2. Check that the document is open (fOpenDocs).  If not, this is
    //    like step 1. of RegisterLink (see above.)  The caller
    //    can recover from this error.
    // 3. Is the linkid is in fLoadedLinks?
    //    3a. Yes, linkid is loaded. The following cases apply:
    //    3a.1 InDoc, or InBoth: This shouldn't occur.  Either
    //         somebody called LinkOpened twice, or else there's
    //         an implementation bug.
    //    3a.2 InAS and not IsLocked (IsLocked means the surrogate's ContentSU
    //                   is locked)
    //         1. Externalize the ASLinkSrc (i.e. the surrogate).
    //         2. Retrieve the Auxiliary State
    //         3. Update this entry in fLoadedLinks to be in state InDoc
    //            and save the LinkSrc ref (as DocLinkSrc)
    //         4. Release ASLinkSrc (destroying the surrogate)
    //         5. (If limitchecking: decrement count of loaded surrogates).
    //    3a.3 InAS and IsLocked:
    //         1. Externalize the ASLinkSrc
    //         2. Retrieve the Auxiliary State
    //         3. Update this entry in fLoadedLinks to be in state InBoth
    //            and save the LinkSrc ref (as DocLinkSrc)
    //    3a  ALL cases:  Move linkid to the top of the LRU list.
//???? Perhaps not necessary, as policy will be to keep all opened
//linksources in mem????
    //    3b  No, linkid is not loaded.
    //        1. Insert new entry: <InDoc, --, linkSrc, False>.
    //        2. Retrieve the Auxiliary State
    //        3. update LRU list (see above query??)
    //    4. Return AuxiliaryState and locked State

 * <1>  Acquire fMutex before accessing fDocuments and fLinks.
 * <2>  Check if linkID is associated with a registered link.
 *      If true retrieve the LinkSrcInfo entry of linkID from fLinks.
 * <3>  Check if document containing the link is registered.
 *      If true retrieve DocInfo of that document from fDocuments.
 * <4>  Check if document is opened.
 * <5>  The document is opened.
 *      Check the state of the LinkSrcInfo entry.
 * <6>  If state is UNLOADED:
 *        Change state to INDOC.
 *        Save LinkSource proxy.
 *        Assign isLocked to false.
 *        Allocate memory for auxState.
 * <6.1>  Retrieve SU of the auxiliary state from fAsDraft.
 *        Assign auxState with data read from the SU.
 *      NOTE: <6.1> is necessary because the LinkSource might have been
 *            internalized in the Avaibility Server and then released.
 * <7>  If state is INDOC:
 *        Link already opened!
 *        Release fMutex.
 *        Raise exception.
 *        Return.
 * <8>  If state is INAS and content SU is locked:
 *        Change state to INBOTH.
 *        Save LinkSource proxy.
 *        Assign isLocked to true.
 *        Allocate memory for auxState.
 * <8.1>  Assign auxState from LinkSource surrogate.
 * <8.2>  Retrieve SU of the auxiliary state from fAsDraft.
 *        Write auxState to the SU.
 *      NOTE: a. <8.2> is necessary in order to save the auxiliary state of
 *               the LinkSource surrogate in the fAsDraft because the surrogate
 *               itself was created in the fSurrogateDraft.
 *            b. LinkSource surrogate will be released when the content SU is
 *               unlocked by the LinkTarget.
 * <9>  If state is INAS and content SU is not locked:
 *        Change state to INDOC.
 *        Save LinkSource proxy.
 *        Assign isLocked to false.
 *        Allocate memory for auxState.
 * <9.1>  Assign auxState from LinkSource surrogate.
 * <9.2>  Retrieve SU of the auxiliary state from fAsDraft.
 *        Write auxState to the SU.
 * <9.3>  Release LinkSource surrogate.
 *        Reset asLinkSrc in LinkSrcInfo to null.
 * <10> If state is INBOTH:
 *        Link already opened!
 *        Release fMutex.
 *        Raise exception.
 *        Return.
 * <11> Release fMutex.
 *      Return.
 * <12> Fail testing in <4>: document is not opened.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <13> Fail testing in <3>: document is not registered
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <14> Fail testing in <2>: linkID is invalid.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkOpened(IODAvailServer *somSelf,
                   Environment *ev,
                   ODLinkID linkID,
                   IODLinkTargetToLinkSource* linkSrc,
                   IODAuxiliaryState** auxState,
                   ODBoolean* isLocked)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","LinkOpened");

    IODDocumentID  docID;
    DocInfo*       docInfo;
    LinkSrcInfo*   lsInfo;
    ODStorageUnit* su;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    //---<2>
    if (_fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
    {
      //---<3>
      docID = lsInfo->docID;
      if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
      {
        //---<4>
        if (docInfo->linkSrvc != (IODAvailServerToLinkService*)kODNULL)
        {
          SOM_TRY
            //---<5>
            switch(lsInfo->state)
            {
              case UNLOADED:
                //---<6>
                // Before:<UNLOADED, docID, suID, false, null, null>
                // After :<INDOC,    docID, suID, false, null, linkSrc>
                lsInfo->state = INDOC;
                lsInfo->docLinkSrc = linkSrc;
                *isLocked = kODFalse;
                *auxState = new IODAuxiliaryState;
                THROW_IF_NULL(*auxState);

                //---<6.1>
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                GetAuxStateFromSU(ev, su, **auxState);

                _fShell->UpdateLinkList(linkID, docID, INDOC);

                break;

              case INDOC:
                //---<7>
                TRY
                  _fMutex->Release(ev);
                CATCH_ALL
                ENDTRY

                return;

              case INAS:
                if (lsInfo->isLocked == kODTrue)
                {
                  //---<8>
                  // Before:<INAS,   docID, suID, true, asLS, null>
                  // After :<INBOTH, docID, suID, true, asLS, linkSrc>
                  lsInfo->state = INBOTH;
                  lsInfo->docLinkSrc = linkSrc;
                  *isLocked = kODTrue;
                  *auxState = new IODAuxiliaryState;
                  THROW_IF_NULL(*auxState);

                  //---<8.1>
                  ((IODLinkSource*)lsInfo->asLinkSrc)->GetAuxState(ev,
                                                                   auxState);

                  //---<8.2>
                  su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                  SetAuxStateToSU(ev, su, **auxState);

                  _fShell->UpdateLinkList(linkID, docID, INBOTH);
                }
                else
                {
                  //---<9>
                  // Before:<INAS,  docID, suID, false, asLS, null>
                  // After :<INDOC, docID, suID, false, null, linkSrc>
                  lsInfo->state = INDOC;
                  lsInfo->docLinkSrc = linkSrc;
                  *isLocked = kODFalse;
                  *auxState = new IODAuxiliaryState;
                  THROW_IF_NULL(*auxState);

                  //---<9.1>
                  ((IODLinkSource*)lsInfo->asLinkSrc)->GetAuxState(ev,
                                                                   auxState);

                  //---<9.2>
                  su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                  SetAuxStateToSU(ev, su, **auxState);

                  //---<9.3>
                  TRY
                   ((CMDraft*)_fSurrogateDraft)->
                     RemoveLinkSource(ev, (ODLinkSource*)lsInfo->asLinkSrc);
                  CATCH_ALL
                  ENDTRY
                  lsInfo->asLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;

                  _fShell->UpdateLinkList(linkID, docID, INDOC);
                }
                break;

              //---<10>
              case INBOTH:
                TRY
                  _fMutex->Release(ev);
                CATCH_ALL
                ENDTRY

                ODSetSOMException(ev, kODErrLinkAlreadyOpened);
                return;
            }

            //---<11>
            TRY
              _fMutex->Release(ev);
            CATCH_ALL
            ENDTRY

            return;

          SOM_CATCH_ALL
            ODError exception = ODGetSOMException(ev);

            TRY
              _fMutex->Release(ev);
            CATCH_ALL
            ENDTRY

            ODSetSOMException(ev, exception);
            return;
          SOM_ENDTRY
        }
        else
        {
          //---<12>
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrDocNotOpened);
          return;
        }
      }
      else
      {
        //---<13>
        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        ODSetSOMException(ev, kODErrDocNotRegistered);
        return;
      }
    }
    else
    {
      //---<14>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, kODErrInvalidLinkID);
      return;
    }
}

/*
 *=============================================================================
 * IODAvailServer::LinkClosed
 *
 * LinkClosed is called when a LinkSource is released in a document.
 * It provides the current AuxiliaryState for this LinkSource.
 * The AS will now process calls for this LinkSource.
 * (Note that local LinkTargets will not have a reference any longer to
 * the LinkSource object in the document.)

    // 1. Find the docid associated with this linkid. (fLinks)
    //    If there is no entry, in fLinks, this is a major error,
    //    as in step 1 of LinkOpened.
    // 2. Check that the document is open (fOpenDocs). The recovery
    //    is similar to that of step 1 of RegisterLink, except
    //    that we will not reissue LinkOpened on the linkID
    //    which failed.
    // 3. Is the linkId in fLoadedLinks?
    //    3a. Yes, linkId is loaded.  The following cases apply:
    //    3a. InBoth:  This shouldn't occur.  Link Sources should
    //        not call LinkClosed until the lock is released.
    //        But perhaps a buggy or malevolent target is holding
    //        the lock and the linkSource absolutely HAS TO close?
    //        We may need to require linkSources to break locks
    //        in which case entering this case is an indication
    //        that this is what happened.  Set the locked state OFF
    //        and switch to InAS, after merging the AuxiliaryState
    //        (i.e. replacing it if it is later, and ignoring it
    //        if not)
    //    3a. InDoc:  This is the normal case. Remove the fLoadedLinks
    //        entry and merge the Auxiliary state.
    //    3a. InAS: This means that the AS failed and restarted,
    //        lost the fact that the document and link opened, and now
    //        the link is being closed.  The document should have
    //        re-opened in this case, but if it didn't, we may
    //        have inconsistent auxiliary state (equal version numbers
    //        with different values.)
    //    3b. Not loaded:  This also means that the AS failed and
    //        restarted, as in 3a InAS, but now it's almost certain
    //        that you should trust the Auxiliary state maintained
    //        by the document.

 * <1>  Acquire fMutex before accessing fDocuments and fLinks.
 * <2>  Check if linkID is associated with a registered link.
 *      If true retrieve the LinkSrcInfo entry of linkID from fLinks.
 * <3>  Check if document containing the link is registered.
 *      If true retrieve the DocInfo entry of that document from fDocuments.
 * <4>  Check if that document is opened.
 * <5>  The document is opened.
 *      Check the state of the LinkSrcInfo entry.
 * <6>  If state is UNLOADED:
 *      NOTE: AS failed and restarted, but LinkService already detected this
 *            and re-issued DocOpened.
 *        If auxState is non-null
 *          Get SU of the auxililary state from fAsDraft.
 *          Write auxState to the SU.
 * <7>  If state is INDOC:
 *        Change state to UNLOADED.
 *        Release LinkSource proxy.
 *        Reset docLinkSrc in LinkSrcInfo entry to null.
 *        If auxState is non-null
 *          Get SU of the auxililary state from fAsDraft.
 *          Write auxState to the SU.
 * <8>  If state is INAS:
 *        XXXXX Should not happen!
 *      NOTE: The reason that the state of LinkSrcInfo may be INAS is that
 *            AS failed and restarted and a LinkTarget invokes a method on
 *            LinkSource before LinkService re-issue DocOpened.
 *            However once LinkService re-issue DocOpened and LinkOpened
 *            the state of LinkSrcInfo should either be INDOC or INBOTH.
 * <9>  If state is INBOTH:
 *      NOTE: A LinkTarget still holding the lock of the content SU of the
 *            LinkSource surrogate since LinkOpened was last called.
 *        Change state to INAS.
 *        Release LinkSource proxy.
 *        Reset docLinkSrc in LinkSrcInfo entry to null.
 *        If auxState is non-null
 *          Get SU of the auxililary state from fAsDraft.
 *          Write auxState to the SU.
 *          Reset the auxiliary state of LinkSource surrogate.
 * <10> Release fMutex.
 *      Return.
 * <11> Fail testing in <4>: document is not opened.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <12> Fail testing in <3>: document is not registered
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <13> Fail testing in <2>: linkID is invalid.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkClosed(IODAvailServer *somSelf,
                   Environment *ev,
                   ODLinkID linkID,
                   IODAuxiliaryState* auxState)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","LinkClosed");

    IODDocumentID  docID;
    DocInfo*       docInfo;
    LinkSrcInfo*   lsInfo;
    ODStorageUnit* su;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    //---<2>
    if (_fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
    {
      //---<3>
      docID = lsInfo->docID;
      if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
      {
        //---<4>
        if (docInfo->linkSrvc != (IODAvailServerToLinkService*)kODNULL)
        {
          SOM_TRY
            //---<5>
            switch(lsInfo->state)
            {
              case UNLOADED:
                //---<6>
                // Before:<UNLOADED, docID, suID, false, null, null>
                // After :<UNLOADED, docID, suID, false, null, null>
                if (auxState != (IODAuxiliaryState*)kODNULL)
                {
                  su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                  SetAuxStateToSU(ev, su, *auxState);
                }
                break;

              case INDOC:
                //---<7>
                // Before:<INDOC,    docID, suID, false, null, docLS>
                // After :<UNLOADED, docID, suID, false, null, null>
                lsInfo->state = UNLOADED;
                ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                lsInfo->docLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;
                if (auxState != (IODAuxiliaryState*)kODNULL)
                {
                  su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                  SetAuxStateToSU(ev, su, *auxState);
                }

                _fShell->UpdateLinkList(linkID, docID, UNLOADED);

                break;

              case INAS:
                //---<9>
                break;

              case INBOTH:
                //---<8>
                // Before:<INBOTH, docID, suID, true, asLS, docLS>
                // After :<INAS,   docID, suID, true, asLS, null>
                lsInfo->state = INAS;
                ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                lsInfo->docLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;
                if (auxState != (IODAuxiliaryState*)kODNULL)
                {
                  su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                  SetAuxStateToSU(ev, su, *auxState);
                  ((IODLinkSource*)lsInfo->asLinkSrc)->ResetAuxState(ev,
                                                                     auxState);
                }

                _fShell->UpdateLinkList(linkID, docID, INAS);

                break;
            }

            //---<10>
            TRY
              _fMutex->Release(ev);
            CATCH_ALL
            ENDTRY

            return;

          SOM_CATCH_ALL
            ODError exception = ODGetSOMException(ev);

            TRY
              _fMutex->Release(ev);
            CATCH_ALL
            ENDTRY

            ODSetSOMException(ev, exception);
            return;
          SOM_ENDTRY
        }
        else
        {
          //---<11>
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrDocNotOpened);
          return;
        }
      }
      else
      {
        //---<12>
        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        ODSetSOMException(ev, kODErrDocNotRegistered);
        return;
      }
    }
    else
    {
      //---<13>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, kODErrInvalidLinkID);
      return;
    }
}

/*
 *=============================================================================
 * IODAvailServer::RemoveLink
 *
 * RemoveLink is called when a LinkSource is removed from a document.

    // 1. Exactly like linkClosed except,
    //    a.instead of merging auxiliary state,
    //      discard entry in fLinks table, and remove SU.
    //    b.never end up in InAS state.  It's the LinkSource's
    //      responsibility to break locks (for example, to
    //      notify the lock holder.  This may be done anyway if we always
    //      notify registered targets when a remove occurs.) if necessary befor
    //      calling this method.  The AS will
    //      release the surrogate, and remove fLoadedLinks entry.

 * <1>  Acquire fMutex before accessing fDocuments and fLinks.
 * <2>  Check if linkID is associated with a registered link.
 *      If true retrieve the LinkSrcInfo entry of linkID from fLinks.
 * <3>  Check if document containing the link is registered.
 *      If true retrieve the DocInfo entry of that document from fDocuments.
 * <4>  Check if that document is opened.
 * <5>  The document is opened.
 *      Check the state of the LinkSrcInfo entry.
 * <6>  If state is UNLOADED:
 *        Get SU of the auxiliary state from fAsDraft.
 *        Ask fAsDraft to remove that SU.
 * <7>  If state is INDOC:
 *        Get SU of the auxiliary state from fAsDraft.
 *        Ask fAsDraft to remove that SU.
 *        Release LinkSource proxy.
 * <8>  If state is INAS:
 *        XXXXX Should not happen!
 *      NOTE: The reason that the state of LinkSrcInfo may be INAS is that
 *            AS failed and restarted and a LinkTarget invokes a method on
 *            LinkSource before LinkService re-issue DocOpened.
 *            However once LinkService re-issue DocOpened and LinkOpened
 *            the state of LinkSrcInfo should either be INDOC or INBOTH.
 * <9>  If state is INBOTH:
 *        Change state to REMOVED.
 *        Remove the storage unit of the auxiliary state.
 *        Release LinkSource proxy.
 *      NOTE: When the content SU is unlocked by the LinkTarget, the
 *            LinkSource surrogate will be released and the LinkSrcInfo
 *            entry will be deleted.
 * <10> Delete linkID from the list of linkIDs in the docInfo entry.
 * <10.1>
 *      If state of the LinkSrcInfo entry is not REMOVED
 *        Remove the LinkSrcInfo from fLinks.
 *        Delete the LinkSrcInfo entry.
 *      Save fDocuments and fLinks to AsDraft.
 *      Release fMutex.
 *      Return.
 * <11> Fail testing in <4>: document is not opened.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <12> Fail testing in <3>: document is not registered
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 * <13> Fail testing in <2>: linkID is invalid.
 *      Release fMutex.
 *      Raise exception.
 *      Return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK RemoveLink(IODAvailServer *somSelf,
                   Environment *ev,
                   ODLinkID linkID)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","RemoveLink");

    IODDocumentID  docID;
    DocInfo*       docInfo;
    LinkSrcInfo*   lsInfo;
    ODStorageUnit* su;

    //---<1>
    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    //---<2>
    if (_fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
    {
      //---<3>
      docID = lsInfo->docID;
      if (_fDocuments->GetValue(&docID, &docInfo) == kODTrue)
      {
        //---<4>
        if (docInfo->linkSrvc != (IODAvailServerToLinkService*)kODNULL)
        {
          TRY
            //---<5>
            switch(lsInfo->state)
            {
              case UNLOADED:
                //---<6>
                //Before:<UNLOADED, docID, suID, false, null, null>
                //After :<UNLOADED, docID, ____, false, null, null>
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                _fAsDraft->RemoveStorageUnit(ev, su);
                break;

              case INDOC:
                //---<7>
                //Before:<INDOC, docID, suID, false, null, docLS>
                //After :<INDOC, docID, ____, false, null, _____>
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                _fAsDraft->RemoveStorageUnit(ev, su);
                ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                break;

              case INAS:
                //---<9>
                break;

              case INBOTH:
                //---<8>
                //Before:<INBOTH,  docID, suID, true, asLS, docLS>
                //After :<REMOVED, docID, ____, true, asLS, _____>
                lsInfo->state = REMOVED;
                su = _fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
                _fAsDraft->RemoveStorageUnit(ev, su);
                ReleaseDocLinkSource(ev, lsInfo->docLinkSrc);
                break;
            }
          CATCH_ALL
            // ignore exceptions caught when making calls on _fAsDraft
          ENDTRY

          //---<10>
          LinkIDNode *hd, *nptr;
          hd = nptr = docInfo->linkIDs;
          while(nptr != (LinkIDNode*)kODNULL)
          {
            if (nptr->linkID = linkID)
            {
              if (hd == nptr)
              {
                docInfo->linkIDs = nptr->next;
              }
              else
              {
                hd->next = nptr->next;
              }
              nptr->next = kODNULL;
              delete nptr;
              break;
            }
            hd = nptr;
            nptr = nptr->next;
          }

          //---<10.1>
          if (lsInfo->state != REMOVED)
          {
            _fLinks->RemoveEntry(&linkID);
            delete lsInfo;
          }

          TRY
            SaveToAsDraft(ev, _fShell, _fAsDraft, 0, 0, _fDocuments, _fLinks);
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          return;
        }
        else
        {
          //---<11>
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrDocNotOpened);
          return;
        }
      }
      else
      {
        //---<12>
        TRY
          _fMutex->Release(ev);
        CATCH_ALL
        ENDTRY

        ODSetSOMException(ev, kODErrDocNotRegistered);
        return;
      }
    }
    else
    {
      //---<13>
      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, kODErrInvalidLinkID);
      return;
    }
}


/*
 *=============================================================================
// The GetDispatch does the following:
// 1. If the linkId is loaded, find its LoadedLinks entry.
//   1.1   If it's InAS, make it 'most recently used' if not already,
//          and return the ASLinkSrc.
//   1.2.  If it's InDoc or InBoth, return the DocLinkSrc
// 2. If the linkId isn't loaded,
//   2.1. If the link's document (see fLinks) is open (see OpenDocs),
//         2.1.1. ask the documents AS to open the LinkSource,
//         2.1.2. insert loadedLinks entry in state InDoc
//         2.1.3. If this fails, assume the document crashed. Close it.
//   2.2. If the link's document is closed (or was made closed by step 2.1.3)
//         2.2.1. Open up a read-only document using the document's pathname.
//         2.2.2. Locate that document's draft's LinkSource table.
//         2.2.3. Clone the LinkSource into the SurrogateDraft.
//         2.2.4. Ask the surrogate draft to create a surrogate
//                and give the new surrogate access to the auxiliary state
//                retrieved from the AsDraft. The surrogate linkSource
//                will merge the auxiliary state from the document
//                with the one retrieved from the AsDraft.  It will
//                write it into the ASDraft.
//         2.2.5. Save the ASDraft.
//         2.2.6. insert loadedLinks entry in state InAS.

 * <1>  Check if linkID is associated with a registered link.
 *      If true retrieve the LinkSrcInfo entry of linkID from fLinks.
 * <2>  Check if the state of LinkSrcInfo entry is UNLOADED.
 * <3>  Check if document containing the link is registered.
 *      If true retrieve the DocInfo entry of that document from fDocuments.
 * <4>  Allocate memory for auxState.
 *      Ask fAsDraft to get the SU of the auxiliary state.
 *      Assign auxState to the data retrieved from the SU.
 * <5>  Check if document is opened.
 * <6>  The document is opened.
 *      Release fMutex so it will not block call incoming call
 *        while making remote call.
 * <7>  Ask LinkService of the document to internalize the LinkSource and
 *        obtain its proxy.
 * <8>  Exception caught on remote call to LinkService to internalize the
 *        LinkSource, may be the document crashed.
 *      Acquire fMutex again in order to access fLinks.
 * <9> Check if linkID is still associated with a registered link.
 *     If true retrieve the LinkSrcInfo entry of linkID from fLinks.
 * <10> Ask fShell to clone LinkSource from document draft to fSurrogateDraft.
 *      Ask fSurrogateDraft to create the surrogate and save it in LinkSrcInfo
 *        entry.
 *      Change state to INAS.
 *      Cleanup and delete auxState.
 *      Return the LinkSrcInfo entry.
 * <11> Fail testing in <9>: LinkService called RemoveLink and destroy
 *        LinkSrcInfo entry while fMutex was released at <6>.
 *      Raise exception.
 * <12> No exception caught on remote call to LinkService to internalize the
 *        LinkSource.
 *      Acquire fMutex again to access fLinks.
 * <13> Check if linkID is still associated with a registered link.
 *      If true retrieve the LinkSrcInfo entry of linkID from fLinks.
 * <14> If state of LinkSrcInfo remains UNLOADED
 *        Change state to INDOC.
 *        Save the LinkSource proxy obtained in <7>.
 *        Cleanup and delete auxState.
 *        Return the LinkSrcInfo entry.
 *      NOTE: If LinkService called LinkOpen while fMutex was released at <6>
 *            the state of LinkSrcInfo becomes INDOC and it will have the
 *            LinkSource proxy already.
 *            However the state can never be INAS, INBOTH or REMOVED.
 * <15> Fail testing in <13>: LinkService called RemoveLink and destroy
 *        LinkSrcInfo entry while fMutex was released at <6>.
 *      Raise exception.

 * <16> Fail testing in <5>: document is closed.
 *      Ask fShell to clone LinkSource from document draft to fSurrogateDraft.
 *      Ask fSurrogateDraft to create the surrogate and save it in LinkSrcInfo
 *        entry.
 *      Change state to INAS.
 *      Cleanup and delete auxState.
 *      Return the LinkSrcInfo entry.
 * <17> Fail testing in <3>: document is not registered.
 *      Raise exception.
 * <18> The state of  LinkSrcInfo entry is either INDOC, INBOTH or  REMOVED.
 *      Return the LinkSrcInfo entry.
 * <19> Fail testing in <1>: invalid linkID.
 *      Raise exception.
 *=============================================================================
 */

LinkSrcInfo* GetDispatch(Environment* ev,
                         IODMutex* fMutex,
//                         AEHashTable* fDocuments,
//                         AEHashTable* fLinks,
                         OpenHashTable* fDocuments,
                         OpenHashTable* fLinks,
                         AvsShell* fShell,
                         ODDraft* fAsDraft,
                         ODDraft* fSurrogateDraft,
                         ODLinkID linkID)
{
    IODAuxiliaryState*         auxState;
    IODDocumentID              docID;
    DocInfo*                   docInfo;
    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;
    ODStorageUnit*             su;
    ODStorageUnitID            suID;

    //---<1>
    if (fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
    {
      //---<2>
      if (lsInfo->state == UNLOADED)
      {
        //---<3>
        docID = lsInfo->docID;
        if (fDocuments->GetValue(&docID, &docInfo) == kODTrue)
        {
          TRY
            //---<4>
            auxState = new IODAuxiliaryState;
            THROW_IF_NULL(auxState);

            su = fAsDraft->AcquireStorageUnit(ev, lsInfo->auxStateSUID);
            GetAuxStateFromSU(ev, su, *auxState);

          CATCH_ALL
            RERAISE;
          ENDTRY

          //---<5>
          if (docInfo->linkSrvc != (IODAvailServerToLinkService*)kODNULL)
          {
            //---<6>
            IODLinkService* linkSrvc = (IODLinkService*)docInfo->linkSrvc;

            TRY
              fMutex->Release(ev);
            CATCH_ALL
              RERAISE;
            ENDTRY

            TRY
              //---<7>
              linkSrc = (IODLinkTargetToLinkSource*)
                        linkSrvc->InternalizeLinkSource(ev, linkID, auxState);
            CATCH_ALL
              //---<8>
              TRY
                fMutex->Acquire(ev);
              CATCH_ALL
                RERAISE;
              ENDTRY

              //---<9>
              if (fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
              {
                TRY
                  //---<10>
                  suID = fShell->CloneLinkSource(linkID, docInfo->pathName);
                  lsInfo->asLinkSrc = (IODLinkTargetToLinkSource*)
                                      ((CMDraft*)fSurrogateDraft)->
                                                   GetLinkSrcForAvlSvr(
                                                     ev,
                                                     suID,
                                                     auxState);
                  lsInfo->state = INAS;

                  fShell->UpdateLinkList(linkID, docID, INAS);

                  ODDisposePtr(auxState->remoteLinkInfos._buffer);
                  delete auxState;
                  return lsInfo;

                CATCH_ALL
                  ODDisposePtr(auxState->remoteLinkInfos._buffer);
                  delete auxState;
                  RERAISE;
                ENDTRY
              }
              else
              {
                //---<11>
                ODDisposePtr(auxState->remoteLinkInfos._buffer);
                delete auxState;
                THROW(kODErrInvalidLinkID);
              }
            ENDTRY

            //---<12>
            TRY
              fMutex->Acquire(ev);
            CATCH_ALL
              RERAISE;
            ENDTRY

            //---<13>
            if (fLinks->GetValue(&linkID, &lsInfo) == kODTrue)
            {
              //---<14>
              if (lsInfo->state == UNLOADED)
              {
                lsInfo->state = INDOC;
                lsInfo->docLinkSrc = linkSrc;

                fShell->UpdateLinkList(linkID, docID, INDOC);
              }

              ODDisposePtr(auxState->remoteLinkInfos._buffer);
              delete auxState;
              return lsInfo;
            }
            else
            {
              //---<15>
              ODDisposePtr(auxState->remoteLinkInfos._buffer);
              delete auxState;
              THROW(kODErrInvalidLinkID);
            }
          }
          else
          {
            //---<16>
            TRY
              suID = fShell->CloneLinkSource(linkID, docInfo->pathName);
              lsInfo->asLinkSrc = (IODLinkTargetToLinkSource*)
                                  ((CMDraft*)fSurrogateDraft)->
                                               GetLinkSrcForAvlSvr(ev,
                                                                   suID,
                                                                   auxState);
              lsInfo->state = INAS;

              fShell->UpdateLinkList(linkID, docID, INAS);

              ODDisposePtr(auxState->remoteLinkInfos._buffer);
              delete auxState;
              return lsInfo;

            CATCH_ALL
              ODDisposePtr(auxState->remoteLinkInfos._buffer);
              delete auxState;
              RERAISE;
            ENDTRY
          }
        }
        else
        {
          //---<17>
          THROW(kODErrDocNotRegistered);
        }
      }
      else
      {
        //---<18>
        return lsInfo;
      }
    }
    else
    {
      //---<19>
      THROW(kODErrInvalidLinkID);
    }
}



// All of the following have the same form:
// 1. Grab a semaphore.
// 2. Call GetDispatch to locate the loadedLinks entry and linkSource
// 3. Call the corresponding LinkSource method (either in doc or surrogate)
// 4. If this was Lock or Unlock and not InDoc,
//    record state in loadedLinks entry.
// 5. If an InBoth entry became unlocked, release surrogate, go to InDoc.
//    (decrementing the count if limitchecking)
// 6. Release the semaphore.



/*
 *=============================================================================
 * IODAvailServer::ForwardLock
 *=============================================================================
 */

SOM_Scope ODBoolean
SOMLINK ForwardLock(IODAvailServer *somSelf,
                    Environment *ev,
                    ODLinkID linkID,
                    ODULong wait,
                    ODLinkKey* key)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","ForwardLock");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;
    ODBoolean                  rc;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return kODFalse;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case INBOTH:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          return kODFalse;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return kODFalse;
      }

      rc = linkSrc->ForwardLock(ev, linkID, wait, key);

      if ((lsInfo->state == INAS) && (rc == kODTrue))
      {
        lsInfo->isLocked = kODTrue;
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return rc;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return kODFalse;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::ForwardUnlock
 *=============================================================================
 */

SOM_Scope void
SOMLINK ForwardUnlock(IODAvailServer *somSelf,
                      Environment *ev,
                      ODLinkID linkID,
                      ODLinkKey key,
                      ODBoolean isLocal)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","ForwardUnlock");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
        case INBOTH:
        case REMOVED:
          linkSrc = lsInfo->asLinkSrc;
          break;
      }

      linkSrc->ForwardUnlock(ev, linkID, key, isLocal);

      switch(lsInfo->state)
      {
        case INAS:
          lsInfo->isLocked = kODFalse;
          break;

        case INBOTH:
          lsInfo->state = INDOC;
          lsInfo->isLocked = kODFalse;
          TRY
            ((CMDraft*)_fSurrogateDraft)->
              RemoveLinkSource(ev, (ODLinkSource*)lsInfo->asLinkSrc);
          CATCH_ALL
          ENDTRY
          lsInfo->asLinkSrc = (IODLinkTargetToLinkSource*)kODNULL;
          lsInfo->docLinkSrc->ForwardUnlock(ev, linkID, key, isLocal);
          break;

        case REMOVED:
          _fLinks->RemoveEntry(&linkID);
          TRY
            ((CMDraft*)_fSurrogateDraft)->
              RemoveLinkSource(ev, (ODLinkSource*)lsInfo->asLinkSrc);
          CATCH_ALL
          ENDTRY
          delete lsInfo;
          break;
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::ForwardGetContentStorageUnit
 *=============================================================================
 */

SOM_Scope ODStorageUnit*
SOMLINK ForwardGetContentStorageUnit(IODAvailServer *somSelf,
                                     Environment *ev,
                                     ODLinkID linkID,
                                     ODLinkKey key)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","ForwardGetContentStorageUnit");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;
    ODStorageUnit*             su;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return kODNULL;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
        case INBOTH:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return kODNULL;
      }

      su = linkSrc->ForwardGetContentStorageUnit(ev, linkID, key);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return su;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return kODNULL;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::AddLinkTarget
 *=============================================================================
 */

SOM_Scope void
SOMLINK AddLinkTarget(IODAvailServer *somSelf,
                      Environment *ev,
                      ODLinkID linkid,
                      ODULong pid,
                      ODLinkConnectionData* trgtPartDesc,
                      IODLinkTarget* linkTrgt,
                      ODStorageUnitID* sourceSUID,
                      ODLinkConnectionID* trgtID,
                      ODBoolean* isLocal,
                      ODBoolean* trackTrgt)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","AddLinkTarget");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkid);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->AddLinkTarget(ev,
                             linkid,
                             pid,
                             trgtPartDesc,
                             linkTrgt,
                             sourceSUID,
                             trgtID,
                             isLocal,
                             trackTrgt);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}
/*
 *=============================================================================
 * IODAvailServer::ForwardGetUpdateID
 *=============================================================================
 */

SOM_Scope ODUpdateID
SOMLINK ForwardGetUpdateID(IODAvailServer *somSelf,
                           Environment *ev,
                           ODLinkID linkID)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","ForwardGetUpdateID");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;
    ODUpdateID                 changeID;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return kODUnknownUpdate;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return kODUnknownUpdate;
      }

      changeID = linkSrc->ForwardGetUpdateID(ev, linkID);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return changeID;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return kODUnknownUpdate;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::ForwardGetChangeTime
 *=============================================================================
 */

SOM_Scope ODTime
SOMLINK ForwardGetChangeTime(IODAvailServer *somSelf,
                             Environment *ev,
                             ODLinkID linkID)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","ForwardGetChangeTime");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;
    ODTime                     changeTime;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return 0;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return 0;
      }

      changeTime = linkSrc->ForwardGetChangeTime(ev, linkID);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return changeTime;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return 0;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::GetSourcePartDescription
 *=============================================================================
 */

SOM_Scope void
SOMLINK GetSourcePartDescription(IODAvailServer *somSelf,
                                 Environment *ev,
                                 ODLinkID linkID,
                                 ODLinkConnectionData** srcPartDesc)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","GetSourcePartDescription");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->GetSourcePartDescription(ev, linkID, srcPartDesc);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::LinkTargetClosed
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkTargetClosed(IODAvailServer *somSelf,
                         Environment *ev,
                         ODLinkID linkID,
                         ODLinkConnectionID trgtID,
                         ODBoolean isLocal)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","LinkTargetClosed");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->LinkTargetClosed(ev, linkID, trgtID, isLocal);

      if (lsInfo->state == INAS)
      {
        SaveAuxState(ev, _fShell, _fAsDraft, lsInfo);
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::LinkTargetRemoved
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkTargetRemoved(IODAvailServer *somSelf,
                          Environment *ev,
                          ODLinkID linkID,
                          ODLinkConnectionID trgtID,
                          ODBoolean isLocal)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","LinkTargetRemoved");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->LinkTargetRemoved(ev, linkID, trgtID, isLocal);

      if (lsInfo->state == INAS)
      {
        SaveAuxState(ev, _fShell, _fAsDraft, lsInfo);
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::LinkTargetReopened
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkTargetReopened(IODAvailServer *somSelf,
                           Environment *ev,
                           ODLinkID linkID,
                           ODLinkConnectionID trgtID,
                           ODULong pid,
                           ODLinkConnectionData* trgtPartDesc,
                           IODLinkTarget* linkTrgt,
                           ODBoolean* isLocal)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","LinkTargetReopened");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->LinkTargetReopened(ev,
                                  linkID,
                                  trgtID,
                                  pid,
                                  trgtPartDesc,
                                  linkTrgt,
                                  isLocal);

      if (lsInfo->state == INAS)
      {
        SaveAuxState(ev, _fShell, _fAsDraft, lsInfo);
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::LockToRead
 *=============================================================================
 */

SOM_Scope void
SOMLINK LockToRead(IODAvailServer *somSelf,
                   Environment *ev,
                   ODLinkID linkID,
                   ODULong wait,
                   ODLinkKey* key)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","LockToRead");


}

/*
 *=============================================================================
 * IODAvailServer::RegisterTrackedLinkTarget
 *=============================================================================
 */

SOM_Scope void
SOMLINK RegisterTrackedLinkTarget(IODAvailServer *somSelf,
                                  Environment *ev,
                                  ODLinkID linkID,
                                  ODLinkConnectionID trgtID,
                                  ODBoolean isLocal)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","RegisterTrackedLinkTarget");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->RegisterTrackedLinkTarget(ev, linkID, trgtID, isLocal);

      if (lsInfo->state == INAS)
      {
        SaveAuxState(ev, _fShell, _fAsDraft, lsInfo);
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::RegisterUntrackedLinkTarget
 *=============================================================================
 */

SOM_Scope void
SOMLINK RegisterUntrackedLinkTarget(IODAvailServer *somSelf,
                                    Environment *ev,
                                    ODLinkID linkID,
                                    IODLinkTarget* linkTrgt,
                                    ODLinkConnectionData* trgtPartDesc,
                                    ODBoolean isLocal,
                                    ODLinkConnectionID* trgtID)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","RegisterUntrackedLinkTarget");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->RegisterUntrackedLinkTarget(ev,
                                           linkID,
                                           linkTrgt,
                                           trgtPartDesc,
                                           isLocal,
                                           trgtID);

      if (lsInfo->state == INAS)
      {
        SaveAuxState(ev, _fShell, _fAsDraft, lsInfo);
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::ShowSourceContentFor
 *=============================================================================
 */

SOM_Scope void
SOMLINK ShowSourceContentFor(IODAvailServer *somSelf,
                             Environment *ev,
                             ODLinkID linkID,
                             ODISOStr hostName)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","ShowSourceContentFor");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrSourceDocNotOpen);
          return;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->ShowSourceContentFor(ev, linkID, hostName);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::TargetPartDescriptionChanged
 *=============================================================================
 */

SOM_Scope void
SOMLINK TargetPartDescriptionChanged(IODAvailServer *somSelf,
                                     Environment *ev,
                                     ODLinkID linkID,
                                     ODLinkConnectionID trgtID,
                                     ODBoolean isLocal,
                                     ODLinkConnectionData* trgtPartDesc)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","TargetPartDescriptionChanged");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->TargetPartDescriptionChanged(ev,
                                            linkID,
                                            trgtID,
                                            isLocal,
                                            trgtPartDesc);

      if (lsInfo->state == INAS)
      {
        SaveAuxState(ev, _fShell, _fAsDraft, lsInfo);
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}

/*
 *=============================================================================
 * IODAvailServer::UnregisterLinkTarget
 *=============================================================================
 */

SOM_Scope void
SOMLINK UnregisterLinkTarget(IODAvailServer *somSelf,
                             Environment *ev,
                             ODLinkID linkID,
                             ODLinkConnectionID trgtID,
                             ODBoolean isLocal)
{
    IODAvailServerData *somThis = IODAvailServerGetData(somSelf);
    IODAvailServerMethodDebug("IODAvailServer","UnregisterLinkTarget");

    IODLinkTargetToLinkSource* linkSrc;
    LinkSrcInfo*               lsInfo;

    SOM_TRY
      _fMutex->Acquire(ev);
    SOM_CATCH_ALL
      return;
    SOM_ENDTRY

    SOM_TRY
      lsInfo = GetDispatch(ev,
                           _fMutex,
                           _fDocuments,
                           _fLinks,
                           _fShell,
                           _fAsDraft,
                           _fSurrogateDraft,
                           linkID);

      switch(lsInfo->state)
      {
        case INDOC:
        case INBOTH:
          linkSrc = lsInfo->docLinkSrc;
          break;

        case INAS:
          linkSrc = lsInfo->asLinkSrc;
          break;

        case REMOVED:
          TRY
            _fMutex->Release(ev);
          CATCH_ALL
          ENDTRY

          ODSetSOMException(ev, kODErrWaitingUnlock);
          return;
      }

      linkSrc->UnregisterLinkTarget(ev, linkID, trgtID, isLocal);

      if (lsInfo->state == INAS)
      {
        SaveAuxState(ev, _fShell, _fAsDraft, lsInfo);
      }

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      return;

    SOM_CATCH_ALL
      ODError exception = ODGetSOMException(ev);

      TRY
        _fMutex->Release(ev);
      CATCH_ALL
      ENDTRY

      ODSetSOMException(ev, exception);
      return;
    SOM_ENDTRY
}
#endif

