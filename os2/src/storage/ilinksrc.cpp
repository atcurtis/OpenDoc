/* @(#)Z 1.7 os2/src/storage/ilinksrc.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:22 (96/10/01 12:46:24) */
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
#ifndef SOM_Module_ILinkSrc_Source
#define SOM_Module_ILinkSrc_Source
#endif
#define IODLinkSource_Class_Source

#define METHOD_MACROS
#define VARIABLE_MACROS
#include <Builtin.h>
#include <barray.h>
#include <process.h>
#include <limits.h>

#ifndef _ITEXT_
#include <IText.h>
#endif

#include <ISOStr.h>
#include <IODDefs.xh>
#include <ICIDItr.xh>
#define INCL_DOSSEMAPHORES
#define INCL_ODAPI
#include <os2.h>
#include <bsedos.h>

#ifndef _OPENHASH_
  #include <OpenHash.h>
#endif

#ifndef _EXCEPT_
  #include <Except.h>
#endif

#ifndef _STDTYPIO_
  #include <StdTypIO.h>
#endif

#ifndef _STORUTIL_
  #include <storutil.h>
#endif

#ifdef _PLATFORM_OS2_
#include <ODPagtun.h>
#endif

#ifndef SOM_Module_StdTypes_OpenDoc_StdTypes_defined
  #include <StdTypes.xh>
#endif


#ifndef SOM_ODDraft_xh
  #include <Draft.xh>
#endif

#ifndef SOM_ODFrame_xh
  #include <Frame.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
  #include <StorageU.xh>
#endif

#ifndef SOM_IODLinkTarget_xh
  #include <ILnkTrgt.xh>
#endif

#ifndef SOM_IODMutex_xh
  #include <IODMutex.xh>
#endif

#ifndef SOM_ODLink_xh
  #include <link.xh>
#endif

#ifndef SOM_ODLinkspec_xh
  #include <linkspec.xh>
#endif

#include <lmgrdefs.h>

#include <somd.xh>

#include "ILinkSrc.xih"

const ODULong kODInitialNumEntries = 8;
const kODOS2 = 1;

#define SET_SOM_EV \
  ODSetSOMException(ev, ErrorCode(), ErrorMessage())

//Structure of an entry in the fLinkInfos hash table where the key is
//   the connection ID
typedef struct LinkInfo {
  ODLinkConnectionData trgtPartDesc;
  ODLinkConnectionID    trgtID;
  IODLinkTarget*     linkTrgt;
  ODBoolean          isAlive;
  ODBoolean          isRegistered;
} LinkInfo;


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
 * Local function to free a lock wait element
 *
 * <1> destroy mutex
 * <2> close event
 * <3> free structure
 *
 *=============================================================================
*/
void FreeLWE  (LockWaitElement* LWE)
{
  delete LWE->mutex;
  APIRET rc = DosCloseEventSem(LWE->event);
  delete LWE;
}

/*
 *=============================================================================
 * Local function for getting a ISO string from the storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Get string length by getting the size of the focused value.
 * <3>  Allocate memory for the out parameter isoStr.
 * <4>  Read the value of isoStr from the storage unit.
 * <5>  Set string terminator.
 *=============================================================================
*/

void GetISOStrProp (Environment *ev,
                    ODStorageUnit *su,
                    ODPropertyName prop,
                    ODValueType val,
                    ODISOStr &isoStr)
{
    ODULong strLength;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, val))
    {
      //---<2>
      strLength = su->GetSize(ev);

      //---<3>
      isoStr = (ODISOStr)ODNewPtr(strLength+1, kDefaultHeapID);

      //---<4>
      StorageUnitGetValue(su, ev, strLength, (ODValue)isoStr);

      //---<5>
      isoStr[strLength] = '\0';
    }
}

/*
 *=============================================================================
 * Local function for locking the content SU of this LinkSource:
 *
 * <1> Grab the mutex
 * <2> Try to get the lock.
 *    <2a> succeeds (was unlocked and call to parent succeeded)
 *         <2a1> marked locked
 *         <2a2> release mutex
 *         <2a3> return success
 *    <2b> fails hopelessly (was unlocked but call to parent failed
 *                          or was broken)
 *         <2b1> release mutex
 *         <2b2> return failure
 *    <2c> fails hopefully (was locked)
 *         <2c1> create a lock wait element:
 *               new mutex;
 *               new event;
 *               not broken
 *               not timedout
 *         <2c2> queue it at the end of the FIFO
 *         <2c3> release the mutex
 *         <2c4> wait for the event
 *         <2c5> grab the mutex in the LWE
 *         <2c6> Determine whether you succeeded:
 *               <2c6a> broken -- release mutex, free lock wait element
 *                      and return failure
 *                      Don't touch object --- it might have been freed
 *               <2c6b> !broken&&!timeoutreturn -- release mutex,
 *                      free lock wait element, and return key, failure if !=0
 *               <2c6c> !broken&&timeoutreturn -- set timedout,
 *                      release mutex, and return failure
 *                      but don't free LWE because UNLOCK will free it.
 *
 *=============================================================================
*/
ODBoolean WaitForLock  (IODLinkSource *somSelf,
                        Environment *ev,
                        ODULong wait,
                        ODLinkKey* key,
                        LockState* lockstate,
                        IODMutex* mutex,
                        LockWaitElement** first,
                        LockWaitElement** last)
{
ODBoolean success;
LockWaitElement* myLWE;

// 1.
   mutex->Acquire(ev);

// 2.
   if (*lockstate == IODLinkSource_UNLOCKED)
     {
       // 2a or 2b.
       success = kODTrue;
       *key = 1;
       if (success)
         {
           *lockstate = IODLinkSource_LOCKED;
         }
       mutex->Release(ev);
       return (success);
     }
   else if (*lockstate == IODLinkSource_BROKEN) {
       // 2b.
       mutex->Release(ev);
       *key = 0;
       return (kODFalse);
     }
   else {
     // 2c.1
     myLWE = new LockWaitElement;
     myLWE->mutex = new IODMutex;
     APIRET rc = DosCreateEventSem( (PSZ)kODNULL,
                                    &myLWE->event,
                                    0,
                                    FALSE);
     if (rc != 0) {
       delete myLWE->mutex;
       delete myLWE;
       *key = 0;
       return (kODFalse);
       }

     ODULong usedwait = wait;
     #if 0
     usedwait = ULONG_MAX;
     #endif

     if (usedwait == ULONG_MAX) {
       usedwait = -1;
     }

     myLWE->timedout = kODFalse;
     myLWE->broken = kODFalse;

     // 2c.2.
     myLWE->next = kODNULL;
     if (*last == kODNULL) {
       *first = myLWE;
       *last = myLWE;
     }
     else {
       (*last)->next = myLWE;
     }

     // 2c.3
     mutex->Release(ev);

     // 2c.4
     APIRET rc2 = DosWaitEventSem(myLWE->event, usedwait);

     // 2c.5
     myLWE->mutex->Acquire(ev);

     // 2c.6
     if (myLWE->broken) {
       // 2c.6.a.
       myLWE->mutex->Release(ev);
       *key = 0;
       FreeLWE(myLWE);
       return (kODFalse);
     }
     else if (rc2 == 0) {
       // 2c.6.b.
       myLWE->mutex->Release(ev);
       *key = myLWE->key;
       FreeLWE(myLWE);
       return ((ODBoolean) (*key > 0));
     }
     else {
       // 2c.6.c.
       myLWE->timedout = kODTrue;
       myLWE->mutex->Release(ev);
       *key = 0;
       return (kODFalse);
     }
   }
}

/*
 *=============================================================================
 * Local function for writing a hashtable of LinkInfo to the
 * storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Reserve space to write the number of elements in the hashtable.
 * <3>  Iterate thru each element of the hash table.
 * <4>  Write the trgtID, isAlive and isRegistered fields of linkInfo.
 * <5>  The hashtable is for local linkInfo:
 *      Retreive the SUID of the LinkTarget.
 *      Get a weak SU reference from the SUID.
 *      Write the weak SU reference.
 * <6>  The hashtable is for remote linkInfo.
 *      Ask SOM to convert the LinkTarget proxy reference to a SOM externalized
 *      ID.
 *      Get the string length of the SOM externalized ID.
 *      Write the string length.
 *      Write the SOM externalized ID.
 *      Free the ID.
 *      XXXXX Use SOMFree or ORDFree to free the ID ?????
 * <7>  Write the trgtPartDesc field of linkInfo.
 *      The trgtPartDesc is a struct consists of three fields: hostName,
 *      docName and linkdesc, each of which is an ODIText string.
 * <8>  Count the number of elements in the hashtable.
 * <9>  After iterating thru the hashtable, refocus to the given property and
 *      value.
 *      Write the total number of elements in the hashtable in the space
 *      reseved at <2>.
 *=============================================================================
*/

void SetLinkInfosProp (Environment *ev,
                       ODStorageUnit *su,
                       ODPropertyName prop,
                       ODValueType val,
                       OpenHashTable &linkInfos)
{
    ODLinkConnectionID  key;
    LinkInfo*        linkInfo;
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
    OpenHashTableIterator infos(&linkInfos);
    for (infos.First(&key, &linkInfo);
         infos.IsNotComplete();
         infos.Next(&key, &linkInfo))
    {
      //---<4>
      StorageUnitSetValue(su,
                          ev,
                          sizeof(ODLinkConnectionID),
                          (ODValue)&(linkInfo->trgtID));
      StorageUnitSetValue(su,
                          ev,
                          sizeof(ODBoolean),
                          (ODValue)&(linkInfo->isAlive));
      StorageUnitSetValue(su,
                          ev,
                          sizeof(ODBoolean),
                          (ODValue)&(linkInfo->isRegistered));

      if (ODISOStrEqual(prop, kODPropLocalLinkInfos))
      {
        //---<5>
        trgtSUID = linkInfo->linkTrgt->GetStorageUnit(ev)->GetID(ev);
        su->GetWeakStorageUnitRef(ev, trgtSUID, trgtSURef);
        StorageUnitSetValue(su,
                            ev,
                            sizeof(ODStorageUnitRef),
                            (ODValue)&trgtSURef);
      }
      else
      {
        //---<6>
        remoteTrgtID = SOMD_ObjectMgr->
                         somdGetIdFromObject(ev, linkInfo->linkTrgt);
        strLength = ODISOStrLength(remoteTrgtID);

        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        StorageUnitSetValue(su, ev, strLength+1, (ODValue)remoteTrgtID);
        SOMFree(remoteTrgtID);
      }

      //---<7>
      {
      char* istring; /* string part of itext */
//      ODCodePage scode; /* script code part of itext */
      ODScriptCode scode;
      ODLangCode lcode; /* language code partof itext */

      istring = GetCStringFromIText(&linkInfo->trgtPartDesc.HostName);
//      scode = GetITextCodePage(&linkInfo->trgtPartDesc.HostName);
      scode = GetITextScriptCode(&linkInfo->trgtPartDesc.HostName);
      lcode = GetITextLangCode(&linkInfo->trgtPartDesc.HostName);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

      istring = GetCStringFromIText(&linkInfo->trgtPartDesc.DocumentName);
//      scode = GetITextCodePage(&linkInfo->trgtPartDesc.DocumentName);
      scode = GetITextScriptCode(&linkInfo->trgtPartDesc.DocumentName);
      lcode = GetITextLangCode(&linkInfo->trgtPartDesc.DocumentName);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

      istring = GetCStringFromIText(&linkInfo->trgtPartDesc.LinkDesc);
//      scode = GetITextCodePage(&linkInfo->trgtPartDesc.LinkDesc);
      scode = GetITextScriptCode(&linkInfo->trgtPartDesc.LinkDesc);
      lcode = GetITextLangCode(&linkInfo->trgtPartDesc.LinkDesc);
      strLength = strlen(istring);
//     StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

      }
      //---<8>
      numOfElmnts++;
    }

    //---<9>
    su->Focus(ev, prop, kODPosSame, val, 0, kODPosSame);
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfElmnts);
}

/*
 *=============================================================================
 * Local function for reading a hashtable of LinkInfo from a
 * storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Read the total number of elements in the hashtable into numOfElemnts.
 * <3>  Loop to read in each element of the hashtable.
 * <4>  Create a new linkInfo.
 * <5>  Read in the trgtID, isAlive and isRegistered sub fields of linkInfo.
 * <6>  The hashtable is for local linkInfo.
 *      Read in the LinkTarget's SU reference.
 *      Retrieve the SU ID from the SU reference.
 *      Ask the Draft for an object reference of the LinkTarget from the SU ID.
 * <7>  The hashtable is for remote linkInfo.
 *      Read in the string length of the SOM externalized ID.
 *      Allocate memory for remoteTrgtID as read buffer.
 *      Read in the SOM externalized ID.
 *      Ask SOM to convert the ID to a LinkTarget Proxy reference.
 *      Free the SOM externalized ID.
 * <8>  Read in the trgtPartDesc field of linkInfo.
 * <9>  Add linkInfo to the hashtable.
 *=============================================================================
*/

void GetLinkInfosProp (Environment *ev,
                       ODStorageUnit *su,
                       ODPropertyName prop,
                       ODValueType val,
                       OpenHashTable &linkInfos)
{
    ODULong          i;
    LinkInfo*        linkInfo;
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
        linkInfo = new LinkInfo;

        //---<5>
        StorageUnitGetValue(su,
                            ev,
                            sizeof(ODLinkConnectionID),
                            (ODValue)&(linkInfo->trgtID));
        StorageUnitGetValue(su,
                            ev,
                            sizeof(ODBoolean),
                            (ODValue)&(linkInfo->isAlive));
        StorageUnitGetValue(su,
                            ev,
                            sizeof(ODBoolean),
                            (ODValue)&(linkInfo->isRegistered));

        if (ODISOStrEqual(prop, kODPropLocalLinkInfos))
        {
          //---<6>
          StorageUnitGetValue(su,
                              ev,
                              sizeof(ODStorageUnitRef),
                              (ODValue)&trgtSURef);
          if (su->IsValidStorageUnitRef(ev, trgtSURef))
          {
            trgtSUID = su->GetIDFromStorageUnitRef(ev, trgtSURef);
            if (trgtSUID != 0)
            {
              linkInfo->linkTrgt = (IODLinkTarget*)
                                   su->GetDraft(ev)->
                                         AcquireLink(ev,
                                                     trgtSUID,
                                                     (ODLinkSpec*)kODNULL);
            }
            else
            {
              //XXXXX Should raise exception here
            continue;
            }
          }
          else
          {
            //XXXXX Should raise exception here
            continue;
          }
        }
        else
        {
          //---<7>
          StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
          remoteTrgtID = (ODISOStr)SOMMalloc(strLength+1);

          StorageUnitGetValue(su, ev, strLength+1, (ODValue)remoteTrgtID);
          linkInfo->linkTrgt = (IODLinkTarget*)
                               SOMD_ObjectMgr->
                                 somdGetObjectFromId(ev, remoteTrgtID);
          SOMFree(remoteTrgtID);
        }

        //---<8>
        {
        char* istring; /* string part of itext */
//        ODCodePage scode; /* script code part of itext */
        ODScriptCode scode; /* script code part of itext */
        ODLangCode lcode; /* language code part of itext */

        StorageUnitGetValue(su, ev, sizeof(ODScriptCode), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODLangCode), (ODValue)&lcode);
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        istring = new char[strLength+1];
        istring[strLength] = '\0';
        StorageUnitGetValue(su,
                            ev,
                            strLength+1,
                            (ODValue)istring);
        linkInfo->trgtPartDesc.HostName.format = kODIBMCodePageText;
        linkInfo->trgtPartDesc.HostName.text._buffer = kODNULL;
        SetITextCString(&linkInfo->trgtPartDesc.HostName, istring);
        SetITextLangCode(&linkInfo->trgtPartDesc.HostName, lcode);
//        SetITextCodePage(&linkInfo->trgtPartDesc.HostName, scode);
        SetITextScriptCode(&linkInfo->trgtPartDesc.HostName, scode);
        delete istring;

//        StorageUnitGetValue(su, ev, sizeof(ODCodePage), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODScriptCode), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODLangCode), (ODValue)&lcode);
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        istring = new char[strLength+1];
        istring[strLength] = '\0';
        StorageUnitGetValue(su,
                            ev,
                            strLength+1,
                            (ODValue)istring);
        linkInfo->trgtPartDesc.DocumentName.format = kODIBMCodePageText;
        linkInfo->trgtPartDesc.DocumentName.text._buffer = kODNULL;
        SetITextCString(&linkInfo->trgtPartDesc.DocumentName, istring);
        SetITextLangCode(&linkInfo->trgtPartDesc.DocumentName, lcode);
//        SetITextCodePage(&linkInfo->trgtPartDesc.DocumentName, scode);
        SetITextScriptCode(&linkInfo->trgtPartDesc.DocumentName, scode);
        delete istring;

//        StorageUnitGetValue(su, ev, sizeof(ODCodePage), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODScriptCode), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODLangCode), (ODValue)&lcode);
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        istring = new char[strLength+1];
        istring[strLength] = '\0';
        StorageUnitGetValue(su,
                            ev,
                            strLength+1,
                            (ODValue)istring);
        linkInfo->trgtPartDesc.LinkDesc.format = kODIBMCodePageText;
        linkInfo->trgtPartDesc.LinkDesc.text._buffer = kODNULL;
        SetITextCString(&linkInfo->trgtPartDesc.LinkDesc, istring);
        SetITextLangCode(&linkInfo->trgtPartDesc.LinkDesc, lcode);
//       SetITextCodePage(&linkInfo->trgtPartDesc.LinkDesc, scode);
        SetITextScriptCode(&linkInfo->trgtPartDesc.LinkDesc, scode);
        delete istring;

        }
        //---<9>
        linkInfos.ReplaceEntry(&(linkInfo->trgtID), &linkInfo);
      }
    }
}

/*
 *=============================================================================
 * Local function for cloning a hashtable of LinkInfo to another
 * storage unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Reserve space to write the number of elements in the hashtable.
 * <3>  Iterate thru each element of the hash table.
 * <4>  Write the trgtID, isAlive and isRegistered fields of linkInfo.
 * <5>  The hashtable is for local linkInfo:
 *      Ask the Draft to weak clone the LinkTarget and obtain a cloned SUID.
 *      Get a weak SU reference from the cloned SUID.
 *      Write the weak SU reference.
 * <6>  The hashtable is for remote linkInfo:
 *      Ask SOM to convert the LinkTarget proxy reference to a SOM externalized
 *      ID.
 *      Get the string length of the SOM externalized ID.
 *      Write the string length.
 *      Write the SOM externalized ID.
 *      Free the ID.
 *      XXXXX Use SOMFree or ORDFree to free the ID ?????
 * <7>  Write the trgtPartDesc field of linkInfo.
 *      The trgtPartDesc is a struct consists of three fields: hostName,
 *      docName and linkdesc, each of which is an ODIText string.
 * <8>  Count the number of elements in the hashtable.
 * <9>  After iterating thru the hashtable, refocus to the given property and
 *      value.
 *      Write the total number of elements in the hashtable to the space
 *      reseved at <2>.
 *=============================================================================
*/

void CloneLinkInfosProp (Environment *ev,
                         ODStorageUnit *su,
                         ODPropertyName prop,
                         ODValueType val,
                         ODDraft* draft,
                         ODDraftKey draftKey,
                         ODFrame *scopeFrame,
                         OpenHashTable &linkInfos)
{
    ODLinkConnectionID  key;
    LinkInfo*        linkInfo;
    ODULong          numOfElmnts = 0;
    ODISOStr         remoteTrgtID;
    ODULong          strLength;
    ODID             trgtSUID;
    ODStorageUnitRef trgtSURef;

    //---<1>
    ODSUForceFocus(ev, su, prop, val);

    //---<2>
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfElmnts);

    //---<3>
    OpenHashTableIterator infos(&linkInfos);
    for (infos.First(&key, &linkInfo);
         infos.IsNotComplete();
         infos.Next(&key, &linkInfo))
    {
      //---<4>
      StorageUnitSetValue(su,
                          ev,
                          sizeof(ODLinkConnectionID),
                          (ODValue)&(linkInfo->trgtID));
      StorageUnitSetValue(su,
                          ev,
                          sizeof(ODBoolean),
                          (ODValue)&(linkInfo->isAlive));
      StorageUnitSetValue(su,
                          ev,
                          sizeof(ODBoolean),
                          (ODValue)&(linkInfo->isRegistered));

      if (ODISOStrEqual(prop, kODPropLocalLinkInfos))
      {
        //---<5>
        trgtSUID = draft->WeakClone(ev,
                                    draftKey,
                                    linkInfo->linkTrgt->GetID(ev),
                                    0,
                                    (scopeFrame ? scopeFrame->GetID(ev) : 0));
        su->GetWeakStorageUnitRef(ev, trgtSUID, trgtSURef);
        StorageUnitSetValue(su,
                            ev,
                            sizeof(ODStorageUnitRef),
                            (ODValue)&trgtSURef);
      }
      else
      {
        //---<6>
        remoteTrgtID = SOMD_ObjectMgr->
                         somdGetIdFromObject(ev, linkInfo->linkTrgt);
        strLength = ODISOStrLength(remoteTrgtID);

        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        StorageUnitSetValue(su, ev, strLength+1, (ODValue)remoteTrgtID);
        SOMFree(remoteTrgtID);
      }

      //---<7>
      {
      char* istring; /* string part of itext */
//      ODCodePage scode; /* script code part of itext */
      ODScriptCode scode; /* script code part of itext */
      ODLangCode lcode; /* language code partof itext */

      istring = GetCStringFromIText(&linkInfo->trgtPartDesc.HostName);
//      scode = GetITextCodePage(&linkInfo->trgtPartDesc.HostName);
      scode = GetITextScriptCode(&linkInfo->trgtPartDesc.HostName);
      lcode = GetITextLangCode(&linkInfo->trgtPartDesc.HostName);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

      istring = GetCStringFromIText(&linkInfo->trgtPartDesc.DocumentName);
//      scode = GetITextCodePage(&linkInfo->trgtPartDesc.DocumentName);
      scode = GetITextScriptCode(&linkInfo->trgtPartDesc.DocumentName);
      lcode = GetITextLangCode(&linkInfo->trgtPartDesc.DocumentName);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

      istring = GetCStringFromIText(&linkInfo->trgtPartDesc.LinkDesc);
//      scode = GetITextCodePage(&linkInfo->trgtPartDesc.LinkDesc);
      scode = GetITextScriptCode(&linkInfo->trgtPartDesc.LinkDesc);
      lcode = GetITextLangCode(&linkInfo->trgtPartDesc.LinkDesc);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

     }

      //---<8>
      numOfElmnts++;
    }

    //---<9>
    su->Focus(ev, prop, kODPosSame, val, 0, kODPosSame);
    StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&numOfElmnts);
}

/*
 *=============================================================================
 * Local function for writing a ODLinkConnectionData structure to the storage
 * unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Write the hostName, docName and LinkDesc fields of the structure.
 *      These three fields are all ODIText types, therefore their string
 *      length, langcode, and scriptcode must be written before the string.
 *=============================================================================
 */

void SetPartDescriptionProp (Environment *ev,
                             ODStorageUnit *su,
                             ODPropertyName prop,
                             ODValueType val,
                             ODLinkConnectionData &partDesc)
{
    ODULong strLength;
      char* istring; /* string part of itext */
//      ODCodePage scode; /* script code part of itext */
      ODScriptCode scode; /* script code part of itext */
      ODLangCode lcode; /* language code part of itext */

    //---<1>
    ODSUForceFocus(ev, su, prop, val);

    //---<2>
      istring = GetCStringFromIText(&partDesc.HostName);
//     scode = GetITextCodePage(&partDesc.HostName);
      scode = GetITextScriptCode(&partDesc.HostName);
      lcode = GetITextLangCode(&partDesc.HostName);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

      istring = GetCStringFromIText(&partDesc.DocumentName);
//      scode = GetITextCodePage(&partDesc.DocumentName);
      scode = GetITextScriptCode(&partDesc.DocumentName);
      lcode = GetITextLangCode(&partDesc.DocumentName);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;

      istring = GetCStringFromIText(&partDesc.LinkDesc);
//      scode = GetITextCodePage(&partDesc.LinkDesc);
      scode = GetITextScriptCode(&partDesc.LinkDesc);
      lcode = GetITextLangCode(&partDesc.LinkDesc);
      strLength = strlen(istring);
//      StorageUnitSetValue(su, ev, sizeof(ODCodePage), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODScriptCode), (ODValue) &scode);
      StorageUnitSetValue(su, ev, sizeof(ODLangCode), (ODValue) &lcode);
      StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
      StorageUnitSetValue(su,
                          ev,
                          strLength+1,
                          (ODValue)istring);
      delete istring;


}

/*
 *=============================================================================
 * Local function for reading a ODLinkConnectionData structure from the storage
 * unit.
 *
 * <1>  Focus to the given property and value.
 * <2>  Read in the hostName, docName and comm fields of the structure.
 *      These three fields are all ODItext types, therefore their string
 *      length must be read first in order to read the string itself.
 *=============================================================================
 */

void GetPartDescriptionProp (Environment *ev,
                             ODStorageUnit *su,
                             ODPropertyName prop,
                             ODValueType val,
                             ODLinkConnectionData &partDesc)
{
    ODULong strLength;

    //---<1>
    if (ODSUExistsThenFocus(ev, su, prop, val))
    {
      //---<2>
        char* istring; /* string part of itext */
//        ODCodePage scode; /* script code part of itext */
        ODScriptCode scode; /* script code part of itext */
        ODLangCode lcode; /* language code part of itext */

        StorageUnitGetValue(su, ev, sizeof(ODScriptCode), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODLangCode), (ODValue)&lcode);
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        istring = new char[strLength+1];
        istring[strLength] = '\0';
        StorageUnitGetValue(su,
                            ev,
                            strLength+1,
                            (ODValue)istring);
        partDesc.HostName.format = kODIBMCodePageText;
        partDesc.HostName.text._buffer = kODNULL;
        SetITextCString(&partDesc.HostName, istring);
        SetITextLangCode(&partDesc.HostName, lcode);
        SetITextScriptCode(&partDesc.HostName, scode);
        delete istring;

        StorageUnitGetValue(su, ev, sizeof(ODScriptCode), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODLangCode), (ODValue)&lcode);
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        istring = new char[strLength+1];
        istring[strLength] = '\0';
        StorageUnitGetValue(su,
                            ev,
                            strLength+1,
                            (ODValue)istring);
        partDesc.DocumentName.format = kODIBMCodePageText;
        partDesc.DocumentName.text._buffer = kODNULL;
        SetITextCString(&partDesc.DocumentName, istring);
        SetITextLangCode(&partDesc.DocumentName, lcode);
        SetITextScriptCode(&partDesc.DocumentName, scode);
        delete istring;

        StorageUnitGetValue(su, ev, sizeof(ODScriptCode), (ODValue)&scode);
        StorageUnitGetValue(su, ev, sizeof(ODLangCode), (ODValue)&lcode);
        StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue)&strLength);
        istring = new char[strLength+1];
        istring[strLength] = '\0';
        StorageUnitGetValue(su,
                            ev,
                            strLength+1,
                            (ODValue)istring);
        partDesc.LinkDesc.format = kODIBMCodePageText;
        partDesc.LinkDesc.text._buffer = kODNULL;
        SetITextCString(&partDesc.LinkDesc, istring);
        SetITextLangCode(&partDesc.LinkDesc, lcode);
        SetITextScriptCode(&partDesc.LinkDesc, scode);
        delete istring;

    }
}

/*
 *=============================================================================
 * Local function for copying ODLinkConnectionData structure
 *
 * <1>  Allocate memory for the hostName field and copy from the source.
 * <2>  Allocate memory for the docName field and copy from the source.
 * <3>  Allocate memory for the linkdesc field and copy from the source.
 *=============================================================================
 */

void CopyPartDescription(ODLinkConnectionData &dest,
                         ODLinkConnectionData &src)
{
    //---<1>
    dest.HostName = CopyITextStruct(&src.HostName);

    //---<2>
    dest.DocumentName = CopyITextStruct(&src.DocumentName);

    //---<3>
    dest.LinkDesc = CopyITextStruct(&src.LinkDesc);
}

/*
 *=============================================================================
 * Local function for cleaning up ODLinkConnectionData structure
 *
 * <1>  Free hostName, docName and comm field of partDesc.
 *=============================================================================
 */

void CleanupPartDescription (ODLinkConnectionData &partDesc)
{
    int i;

    //---<1>
    DisposeITextStruct(partDesc.LinkDesc);
    DisposeITextStruct(partDesc.DocumentName);
    DisposeITextStruct(partDesc.HostName);
}

/*
 *=============================================================================
 * Called by source Part or source Draft
 *   to retrieve the description of the
 *   given LinkTarget.
 *
 * <1>  Make sure current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 * <4>  Allocate memory for the out parameter trgtPartDesc.
 *      If the link target is local, it has the same hostname and docname
 *      as the link source.
 *      Otherwise copy the trgtPartDesc field of linkInfo into the out
 *      parameter trgtPartDesc.
 *      Release fMutex and return.
 * <5>  An exception is caught.
 *      Release fMutex.
 *      Cleanup out trgtPartDesc.
 *      Set SOM exception and return.
 *=============================================================================
 */
SOM_Scope void  SOMLINK GetLinkConnectionData(IODLinkSource *somSelf,
                                               Environment *ev,
                                              ODLinkConnectionID connID,
                                              ODLinkConnectionData** ConnData)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","DescribeLinkTarget");

    LinkInfo *linkInfo = kODNULL;
    ODBoolean isLocal = kODFalse;

    // Out parameter
    *ConnData = kODNULL;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
    if (_fLocalLinkInfos->GetValue(&connID, &linkInfo) != kODTrue)
    {
      if (_fRemoteLinkInfos->GetValue(&connID, &linkInfo) != kODTrue)
      {
        ODSetSOMException(ev, kODErrInvalidTargetID);
        _fMutex->Release(ev);
        return;
      }
    }
    else
    {
      isLocal = kODTrue;
    }

    TRY

      //---<4>
      *ConnData = new ODLinkConnectionData;
      THROW_IF_NULL(*ConnData);

      if (isLocal)
      {
        (**ConnData).HostName = CopyITextStruct(&_fSrcConnData.HostName);
        (**ConnData).DocumentName = CopyITextStruct(&_fSrcConnData.DocumentName);
        (**ConnData).LinkDesc = CopyITextStruct(&linkInfo->trgtPartDesc.LinkDesc);
      }
      else
      {
        CopyPartDescription(**ConnData, linkInfo->trgtPartDesc);
      }

      _fMutex->Release(ev);

    CATCH_ALL

      //---<5>
      _fMutex->Release(ev);

      if (*ConnData != kODNULL)
      {
        CleanupPartDescription(**ConnData);
        delete *ConnData;
        *ConnData = kODNULL;
      }

      SET_SOM_EV;

    ENDTRY
}

/*
 *=============================================================================
 * Called by source Part to obtain the list of LinkTarget
 *   connection ID's in the LinkSource.
 *
 * <1>  Make sure current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Allocate memory for trgtIDs.
 * <4>  Retrieve all connection ID's from fLinkInfos and add them to trgtIDs.
 *      Release fMutex and return.
 * <5>  An exception is caught.
 *      Release fMutex.
 *      Cleanup trgtIDs.
 *      Set SOM exception and return.
 *=============================================================================
 */
SOM_Scope void  SOMLINK GetLinkConnections(IODLinkSource *somSelf,
                                            Environment *ev,
                                           _IDL_SEQUENCE_unsigned_long* connIDs)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","GetLinkTargets");

    ODLinkConnectionID id;
    LinkInfo*       linkInfo = kODNULL;


    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    TRY

      //---<3>
      (connIDs) -> _maximum = _fLocalLinkInfos->Count() + _fRemoteLinkInfos->Count();
      (connIDs) -> _length = 0;
      (connIDs) -> _buffer = (ODLinkConnectionID*) SOMMalloc(sizeof(ODLinkConnectionID) * (connIDs) -> _maximum);

      //---<4>
      OpenHashTableIterator lInfos(_fLocalLinkInfos);
      for (lInfos.First(&id, &linkInfo);
           lInfos.IsNotComplete();
           lInfos.Next(&id, &linkInfo))
      {
        (connIDs) -> _buffer[((connIDs)->_length)++] = id;
      }

      OpenHashTableIterator rInfos(_fRemoteLinkInfos);
      for (rInfos.First(&id, &linkInfo);
           rInfos.IsNotComplete();
           rInfos.Next(&id, &linkInfo))
      {
        (connIDs) -> _buffer[((connIDs)->_length)++] = id;
      }

      _fMutex->Release(ev);

    CATCH_ALL

      //---<5>
      _fMutex->Release(ev);

    SET_SOM_EV;

    ENDTRY
}

/*
 *=============================================================================
 * Called by Part after creation to initialize the mode of tracking
 *   LinkTargets.
 * By default LinkSource only tracks local or registered LinkTargets.
 *
 * <1>  Make sure current state is not CONNECTED.
 *      Once it is connected fTrackTrgts can not be changed.
 * <2>  Save the call back mode.
 *=============================================================================
 */

SOM_Scope void
SOMLINK InitCallBackMode(IODLinkSource *somSelf,
                         Environment *ev,
                         ODBoolean trackTrgts)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","InitCallBackMode");

    //---<1>
    if (_fState == IODLinkSource_CONNECTED)
    {
      ODSetSOMException(ev, kODErrLinkSourceConnected);
      return;
    }

    //---<2>
    _fTrackTrgts = trackTrgts;
}

/*
 *=============================================================================
 * Called by source Part to break the link with the given LinkTarget.
 *
 * <1>  Make sure current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 *      Remove linkInfo from fLinkInfos if found.
 * <4>  Release fMutex.
 * <5>  Inform LinkTarget that its link is broken (oneway message).
 * <6>  Delete linkInfo.
 *=============================================================================
 */

SOM_Scope void
SOMLINK BreakLinkConnection(IODLinkSource *somSelf,
                         Environment *ev,
                         ODLinkConnectionID connID)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","RemoveLinkTarget");

    ODBoolean isLocal = kODTrue;
    LinkInfo *linkInfo = kODNULL;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
    if (_fLocalLinkInfos->GetValue(&connID, &linkInfo) != kODTrue)
    {
      if (_fRemoteLinkInfos->GetValue(&connID, &linkInfo) != kODTrue)
      {
        ODSetSOMException(ev, kODErrInvalidTargetID);
        _fMutex->Release(ev);
        return;
      }
      else
      {
        _fRemoteLinkInfos->RemoveEntry(&connID);
        isLocal = kODFalse;
      }
    }
    else _fLocalLinkInfos->RemoveEntry(&connID);

    //----<4>
    _fMutex->Release(ev);

    //---<5>
    if (isLocal)
    {
      linkInfo->linkTrgt->Release(ev);
    }
    linkInfo->linkTrgt->RemoveLink(ev);

    //---<6>
    CleanupPartDescription(linkInfo->trgtPartDesc);
    delete linkInfo;
}

/*
 *=============================================================================
 * Called by source Part to get its commentary.
 *
 * <1>  Make sure current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fSrcConnData.
 * <3>  Copy commentary.
 * <4>  Release fMutex.
 *=============================================================================
 */
SOM_Scope void  SOMLINK GetDescription(IODLinkSource *somSelf,
                                        Environment *ev,
                                        ODLinkDescription* desc)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","SetSourcePartCommentary");


    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>

    //---<4>
    *desc = CopyITextStruct(&_fSrcConnData.LinkDesc);

    //---<5>
    _fMutex->Release(ev);
}

/*
 *=============================================================================
 * Called by source Part to set its commentary.
 *
 * <1>  Make sure current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fSrcConnData.
 * <3>  Copy commentary.
 * <4>  Save new commentary.
 * <5>  Release fMutex.
 *=============================================================================
 */

SOM_Scope void
SOMLINK SetDescription(IODLinkSource *somSelf,
                       Environment *ev,
                       ODLinkDescription* desc)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","SetSourcePartCommentary");

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
       DisposeITextStruct(_fSrcConnData.LinkDesc);

    //---<4>
    _fSrcConnData.LinkDesc = CopyITextStruct(desc);

    //---<5>
    _fMutex->Release(ev);
}

/*
 *=============================================================================
 * called:
 * (1) when a persistent linkspec is made from a linksource
 * (2) when a linktarget is copied together with its
 * linksource and the linktarget needs to get the
 * new linkinfo for the copied linksource
 *
 * (1) return the instance variables _fLinkID and _fASName
 *=============================================================================
 */

SOM_Scope void
SOMLINK GetPersistentLinkInfo(IODLinkSource *somSelf,
                              Environment *ev,
                             ODLinkID *linkID,
                              ODISOStr *ASName)

{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","GetPersistentLinkInfo");

    *linkID = _fLinkID;
    *ASName = ODISOStrFromCStr(_fASName);
}

/*
 *=============================================================================
 * Called in Draft::CreateLinkSource to initialize the LinkSource after
 *    it is created.
 *
 * <1>  Make sure current state is EMPTY.
 * <2>  Create fMutex.
 * <3>  Create and initialize the hashtables to store the local and remote
 *      link infos.
 * <4>  Save the input hostName and docName into fSrcConnData.
 *      Set the comm field of fSrcConnData to NULL for now.
 * <5>  Init BaseLinkSource.
 * <6>  Set the current state to INITED.
 *      Save the flinkID as 0 -- will be set at Complete.
 *      Set fIDCounter to 0.
 *      Set fTrackTrgts to false.
 *      Set fLockState to UNLOCKED.
 * <7>  An exception is caught.
 *      Delete fMutex.
 *      Cleanup fSrcConnData.
 *      Delete fLocalInfos and fRemoteInfos.
 *      Set SOM exception and return.
 * <8>  Add properties and values in storage unit to externalize the following
 *      variables:
 *        ODLinkID           fLinkID;
 *        OpenHashTable*       fLocalLinkInfos;
 *        ODLinkConnectionData fSrcConnData;
 *        State              fState;
 *        ODBoolean          fTrackTrgts;
 *        ODULong            fVersion;
 *        ODLinkConnectionID    fIDCounter;
 *        OpenHashTable*       fRemoteLinkInfos;
 * <9>  Externalize so that complete will find the data
 *=============================================================================
 */

SOM_Scope void
SOMLINK InitILinkSource(IODLinkSource *somSelf,
                        Environment *ev,
                        ODStorageUnit* su,
                        ODPart* srcPart,
                        ODISOStr hostName,
                        ODISOStr docName)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","InitILinkSource");

    //---<1>
    if (_fState != IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceAlreadyInited);
      return;
    }

    TRY

      //---<2>
      _fMutex = new IODMutex;
      THROW_IF_NULL(_fMutex);

      //---<3>
      _fLocalLinkInfos = new OpenHashTable;
      THROW_IF_NULL(_fLocalLinkInfos);
      _fLocalLinkInfos->Initialize(kODInitialNumEntries,
                                   sizeof(ODLinkConnectionID),
                                   sizeof(LinkInfo*),
                                   kODFalse);

      _fRemoteLinkInfos = new OpenHashTable;
      THROW_IF_NULL(_fRemoteLinkInfos);
      _fRemoteLinkInfos->Initialize(kODInitialNumEntries,
                                    sizeof(ODLinkConnectionID),
                                    sizeof(LinkInfo*),
                                    kODFalse);

      //---<4>
      _fSrcConnData.HostName.format = kODIBMCodePageText;
      _fSrcConnData.HostName.text._buffer = kODNULL;
      SetITextCString(&_fSrcConnData.HostName, (char*)hostName);
      SetITextLangCode(&_fSrcConnData.HostName, 0);
      SetITextScriptCode(&_fSrcConnData.HostName, 0);

      _fSrcConnData.DocumentName.format = kODIBMCodePageText;
      _fSrcConnData.DocumentName.text._buffer = kODNULL;
      SetITextCString(&_fSrcConnData.DocumentName, (char*)docName);
      SetITextLangCode(&_fSrcConnData.DocumentName, 0);
      SetITextScriptCode(&_fSrcConnData.DocumentName, 0);

      _fSrcConnData.LinkDesc.format = kODIBMCodePageText;
      _fSrcConnData.LinkDesc.text._buffer = kODNULL;
      SetITextCString(&_fSrcConnData.LinkDesc, (char*)"");
      SetITextLangCode(&_fSrcConnData.LinkDesc, 0);
      SetITextScriptCode(&_fSrcConnData.LinkDesc, 0);
      //---<5>
      _InitBaseLinkSource(ev, su, srcPart);

      //---<6>
      _fState = IODLinkSource_INITED;
      _fLinkID = 0; /* linkID will not be known until Complete */
      _fIDCounter = 0;
      _fTrackTrgts = kODFalse;
      _fLockState = IODLinkSource_UNLOCKED;

    CATCH_ALL

      //---<7>
      if (_fMutex != kODNULL)
      {
        delete _fMutex;
        _fMutex = (IODMutex*)kODNULL;
      }

      CleanupPartDescription(_fSrcConnData);

      if (_fLocalLinkInfos != kODNULL)
      {
        delete _fLocalLinkInfos;
        _fLocalLinkInfos = kODNULL;
      }

      if (_fRemoteLinkInfos != kODNULL)
      {
        delete _fRemoteLinkInfos;
        _fRemoteLinkInfos = kODNULL;
      }

      SET_SOM_EV;

    ENDTRY

    //---<8>
    ODSUAddPropValue(ev, su, kODPropLinkID, kODULong);
    ODSUAddPropValue(ev, su, kODPropLocalLinkInfos, kODHashTable);
    ODSUAddPropValue(ev, su, kODPropPartDescription, kODPartDescription);
    ODSUAddPropValue(ev, su, kODPropLinkSourceState, kODULong);
    ODSUAddPropValue(ev, su, kODPropLinkSourceTrackMode, kODBoolean);

    ODSUAddPropValue(ev, su, kODPropVersion, kODULong);
    ODSUAddPropValue(ev, su, kODPropIDCounter, kODULong);
    ODSUAddPropValue(ev, su, kODPropRemoteLinkInfos, kODHashTable);

    //---<9>
    somSelf->Externalize(ev);

}

/*
 *=============================================================================
 * Called in Draft::CreateLinkSource to initialize the LinkSource from
 *    a pre-existing storage unit.
 *
 * <1>  Make sure current state is EMPTY.
* <2>  ... moved to Complete
 * <3>  Init BaseLinkSource from storage.
 * <4>  Create a new fMutex.
 * <5>  Internalize the following variable from the given SU:
 *        ODLinkID           fLinkID;
 * <6)  Return fLinkID
 * XXXXX Should catch exception in the future
 *=============================================================================
 */

SOM_Scope ODLinkID
SOMLINK InitILinkSourceFromStorage(IODLinkSource *somSelf,
                                   Environment *ev,
                                   ODStorageUnit* su)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","InitILinkSourceFromStorage");

    //---<1>
    if (_fState != IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceAlreadyInited);
      return 0;
    }

    //---<2>
    _InitBaseLinkSourceFromStorage(ev, su);

    //---<3>
   // ..  moved to Complete

    //---<4>
    _fMutex = new IODMutex;

    //---<5>
    _fLinkID = ODGetULongProp(ev,su, kODPropLinkID, kODULong);

    _fLocalLinkInfos = new OpenHashTable;
    _fLocalLinkInfos->Initialize(kODInitialNumEntries,
                                 sizeof(ODLinkConnectionID),
                                 sizeof(LinkInfo*),
                                 kODFalse);

    _fRemoteLinkInfos = new OpenHashTable;
    _fRemoteLinkInfos->Initialize(kODInitialNumEntries,
                                  sizeof(ODLinkConnectionID),
                                  sizeof(LinkInfo*),
                                  kODFalse);

    return _fLinkID;
}

/*
 *=============================================================================
 * called after InitILinkSource or InitILinkSourceFromStorage
 * linkIDIFNew will always be supplied (nonzero) for a brand new link source
 * linkIDIfNew will be supplied for a previously stored link source
 *     only if the draft determines that this link source is
 *     a copy in which case a new linkid will be supplied
 *
 * <1> If linkIDIFNew is nonzero, replace the linkID
 * <2>  Save asName
 * <3>  If auxState is non-null retrieve the following properties
 *        PropVersion, PropIDCounter and PropRemoteLinkInfos
 *      from auxState and write them to the given SU and to memory.
 * <4>  Save isLocked.
 * <5>  Internalize the following variables from the given SU:
 *        OpenHashTable*       fLocalLinkInfos;
 *        ODLinkConnectionData fSrcConnData;
 *        State              fState;
 *        ODBoolean          fTrackTrgts;
 *=============================================================================
 */

SOM_Scope void
SOMLINK CompleteInitILinkSource(IODLinkSource *somSelf,
                                   Environment *ev,
                                   ODStorageUnit* su,
                                   ODLinkID linkIDIfNew,
                                   IODAuxiliaryState* auxState,
                                   ODBoolean isLocked,
                                   ODISOStr ASName)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","CompleteInitILinkSource");

/* eventually add a state and check that not fully init until complete called */

    // 1.
    if (linkIDIfNew != (ODLinkID)0)
      {
        _fLinkID = linkIDIfNew;
      }

    // 2.
     _fASName = ODISOStrFromCStr(ASName);

    // 3.
    // eventually we should merge auxstates
    if (auxState != (IODAuxiliaryState*)kODNULL)
    {
      ODSetULongProp(ev,su, kODPropVersion, kODULong, auxState->version);
      ODSetULongProp(ev,su, kODPropIDCounter, kODULong, auxState->idCounter);

      ODSUForceFocus(ev, su, kODPropRemoteLinkInfos, kODHashTable);
      su->SetValue(ev, &(auxState->remoteLinkInfos));
      _fVersion = ODGetULongProp(ev,su, kODPropVersion, kODULong);

      _fIDCounter = ODGetULongProp(ev,su, kODPropIDCounter, kODULong);
      GetLinkInfosProp(ev,
                       su,
                       kODPropRemoteLinkInfos,
                       kODHashTable,
                       *_fRemoteLinkInfos);
    }


    // 4.
    if (isLocked) _fLockState = IODLinkSource_LOCKEDINAS;

    // 5.
    GetLinkInfosProp(ev,
                     su,
                     kODPropLocalLinkInfos,
                     kODHashTable,
                     *_fLocalLinkInfos);

    GetPartDescriptionProp(ev,
                           su,
                           kODPropPartDescription,
                           kODPartDescription,
                           _fSrcConnData);

    _fState = ODGetULongProp(ev,su, kODPropLinkSourceState, kODULong);

    _fTrackTrgts = ODGetBooleanProp(ev,su,
                                    kODPropLinkSourceTrackMode,
                                    kODBoolean);


}

/*
 *=============================================================================
 * <1>  Retrieve the SU of the LinkSource.
 * <2>  Retrieve the following properties
 *        PropVersion, PropIDCounter and PropRemoteLinkInfos
 *      from the LinkSource and write them into its SU.
 * <3>  Retrieve the following properties
 *        PropVersion, PropIDCounter and PropRemoteLinkInfos
 *      from the SU of the LinkSource and save them in auxState.
 *=============================================================================
 */

SOM_Scope void
SOMLINK GetAuxState(IODLinkSource *somSelf,
                    Environment *ev,
                    IODAuxiliaryState** auxState)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","GetAuxState");

    //---<1>
    ODStorageUnit *su = _GetStorageUnit(ev);

    //---<2>
    ODSetULongProp(ev,su, kODPropVersion, kODULong, _fVersion);
    ODSetULongProp(ev,su, kODPropIDCounter, kODULong, _fIDCounter);
    SetLinkInfosProp(ev,
                     su,
                     kODPropRemoteLinkInfos,
                     kODHashTable,
                     *_fRemoteLinkInfos);

    //---<3>
    (*auxState)->version = ODGetULongProp(ev,su, kODPropVersion, kODULong);
    (*auxState)->idCounter = ODGetULongProp(ev,su, kODPropIDCounter, kODULong);
    if (ODSUExistsThenFocus(ev, su, kODPropRemoteLinkInfos, kODHashTable))
    {
      long size = su->GetSize(ev);
      (*auxState)->remoteLinkInfos = CreateEmptyByteArrayStruct(size);
      (*auxState)->remoteLinkInfos._length = size;
      su->GetValue(ev,size, &((*auxState)->remoteLinkInfos));
    }
   else{
      (*auxState)->remoteLinkInfos = CreateEmptyByteArrayStruct(0);
    }
}

/*
 *=============================================================================
 * <1>  Retrieve the SU of the LinkSource.
 * <2>  If auxState is non-null retrieve the following properties
 *        PropVersion, PropIDCounter and PropRemoteLinkInfos
 *      from auxState and write them into the SU of the LinkSource.
 * <3>  Delete old fRemoteLinkInfos hashtable.
 * <4>  Reinternalize
 *        IODULong        fVersion;
 *        ODLinkConnectionID fIDCounter;
 *        OpenHashTable*    fRemoteLinkInfos;
 *=============================================================================
 */

SOM_Scope void
SOMLINK ResetAuxState(IODLinkSource *somSelf,
                      Environment *ev,
                      IODAuxiliaryState* auxState)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ResetAuxState");

    //---<1>
    ODStorageUnit *su = _GetStorageUnit(ev);

    if (auxState != (IODAuxiliaryState*)kODNULL)
    {
      //---<2>
      ODSetULongProp(ev,su, kODPropVersion, kODULong, auxState->version);
      ODSetULongProp(ev,su, kODPropIDCounter, kODULong, auxState->idCounter);

      ODSUForceFocus(ev, su, kODPropRemoteLinkInfos, kODHashTable);
      su->SetValue(ev, &(auxState->remoteLinkInfos));

      //--<3>
      ODLinkConnectionID id;
      LinkInfo*       linkInfo;
      OpenHashTableIterator rInfos(_fRemoteLinkInfos);
      for (rInfos.First(&id, &linkInfo);
           rInfos.IsNotComplete();
           rInfos.Next(&id, &linkInfo))
      {
        CleanupPartDescription(linkInfo->trgtPartDesc);
        if (linkInfo->linkTrgt != kODNULL)
        {
          if (((SOMDObject*)(linkInfo->linkTrgt))->is_proxy(ev))
          {
            SOMD_ObjectMgr->somdReleaseObject(ev, linkInfo->linkTrgt);
          }
        }
        delete linkInfo;
      }
      delete _fRemoteLinkInfos;
      _fRemoteLinkInfos = kODNULL;

      //---<4>
      _fVersion = ODGetULongProp(ev,su, kODPropVersion, kODULong);
      _fIDCounter = ODGetULongProp(ev,su, kODPropIDCounter, kODULong);

      _fRemoteLinkInfos = new OpenHashTable;
      _fRemoteLinkInfos->Initialize(kODInitialNumEntries,
                                    sizeof(ODLinkConnectionID),
                                    sizeof(LinkInfo*),
                                    kODFalse);
      GetLinkInfosProp(ev,
                       su,
                       kODPropRemoteLinkInfos,
                       kODHashTable,
                       *_fRemoteLinkInfos);
    }
}


    // eventually this method will be moved to ODLinkSource and
    // will become public.
    // This writes out a linkspec giving rights to link to this linksource

/*
 * eventually this method will be moved to ODLinkSource and
 * will become public.
 * This writes out a linkspec giving rights to link to this linksource
 */

SOM_Scope void
SOMLINK WritePersistentLinkSpec(IODLinkSource *somSelf,
                                   Environment *ev,
                                  ODStorageUnit* su)

// Write version, Persistent, LinkID, ASName

{
    const kODULongLen = 4 ;
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","WritePersistentLinkSpec");
    ODULong version = kODOS2;
      ODULong subVersion;
      ODLinkID linkID;
      ODISOStr asName;
      ODStorageUnitID sourcePartSUID;
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

}


/*
 *=============================================================================
 *=============================================================================
 */





SOM_Scope void
SOMLINK somInit(IODLinkSource *somSelf)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","somInit");

    IODLinkSource_parent_ODLinkSource_somInit(somSelf);
    IODLinkSource_parent_IODLinkTargetToLinkSource_somInit(somSelf);

    _fMutex = (IODMutex*)kODNULL;
    _fIDCounter = 0;
    _fLinkID = 0;
    _fLocalLinkInfos = (OpenHashTable*)kODNULL;
    _fRemoteLinkInfos = (OpenHashTable*)kODNULL;
    _fSrcConnData.LinkDesc.text._buffer = kODNULL;
    _fSrcConnData.DocumentName.text._buffer = kODNULL;
    _fSrcConnData.HostName.text._buffer = kODNULL;
    _fState = IODLinkSource_EMPTY;
    _fTrackTrgts = kODFalse;
    _fLockState = IODLinkSource_UNLOCKED;
    _fPID = _getpid();
    _fVersion = 0;
    #ifdef _PLATFORM_OS2_
      _fFirstWE = (LockWaitElement*) kODNULL;
      _fLastWE = (LockWaitElement*) kODNULL;
   #endif/* _PLATFORM_OS2_ */

}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK somUninit(IODLinkSource *somSelf)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","somUninit");

    IODLinkSource_parent_ODLinkSource_somUninit(somSelf);
    IODLinkSource_parent_IODLinkTargetToLinkSource_somUninit(somSelf);
}

/*
 *=============================================================================
 * Called in LinkService::EstablishLinkFromTSpec (which is called
 *   in LinkSpec::EstablishLink) or
 *   PersistentLinkSpec::EstablishLink to add a new LinkTarget to the
 *   LinkSource.
 *
 * <1>  Make sure current state is note EMPTY.
 * <2>  Determine the locality of the LinkTarget
 * <3>  For case where LinkTargets are being tracked or the LinkTarget is
 *      local.
 * <4>  Acquire fMutex before accessing fLinkInfos.
 * <5>  Allocate memory for a new linkInfo.
 * <6>  Fill in each field of linkInfo.
 * <7>  Add linkInfo to fLinkInfos.
 * <8>  Fill in out parameters.
 *      Set current state to CONNECTED.
 *      Release fMutex and return.
 * <9>  An exception is caught.
 *      Release fMutex.
 *      Cleanup linkInfo.
 *      Set SOM exception and return.
 * <10> For case where LinkTargets are not tracked and the LinkTarget is
 *      remote just fill in out parameters.
 *=============================================================================
 */

SOM_Scope void
SOMLINK AddLinkTarget(IODLinkSource *somSelf,
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
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","AddLinkTarget");
    LinkInfo *linkInfo = kODNULL;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    //*isLocal = ODISOStrEqual(_fSrcPartDesc.docName, trgtPartDesc->docName);
    if ((_fPID == pid)
        &&
        (strcmp(GetCStringFromIText(&_fSrcConnData.HostName),
                GetCStringFromIText(&trgtPartDesc->HostName))==0))
    {

      *isLocal = kODTrue;
    }
    else
    {
      *isLocal = kODFalse;
    }

    //---<3>
    if (_fTrackTrgts || *isLocal) {
      TRY

        //---<4>
        _fMutex->Acquire(ev);
        if (ODGetSOMException(ev) != kODNoError)
        {
          return;
        }

        //---<5>
        linkInfo = new LinkInfo;
        THROW_IF_NULL(linkInfo);

        //---<6>
        CopyPartDescription(linkInfo->trgtPartDesc, *trgtPartDesc);

        linkInfo->trgtID = ++_fIDCounter;
        linkInfo->linkTrgt = linkTrgt;
        linkInfo->isAlive = kODTrue;
        linkInfo->isRegistered = kODFalse;

        //---<7>
        if (*isLocal)
        {
          linkTrgt->Acquire(ev);
          _fLocalLinkInfos->ReplaceEntry(&(linkInfo->trgtID), &linkInfo);
        }
        else
        {
          _fRemoteLinkInfos->ReplaceEntry(&(linkInfo->trgtID), &linkInfo);
        }

        //---<8>
        *trgtID = linkInfo->trgtID;
        *trackTrgt = kODTrue;
       *sourceSUID = somSelf->GetID(ev);
        _fState = IODLinkSource_CONNECTED;
        _fMutex->Release(ev);

      CATCH_ALL

        //---<9>
        _fMutex->Release(ev);

        if (linkInfo != kODNULL)
        {
          CleanupPartDescription(linkInfo->trgtPartDesc);
          delete linkInfo;
        }

        SET_SOM_EV;

      ENDTRY
    }
    else
    {
      //---<10>
      *trgtID = 0;
      *trackTrgt = kODFalse;

      _fState = IODLinkSource_CONNECTED;
    }

}

/*
 *=============================================================================
 * <1>  Make sure this object is not already cloned in the toSU.
 * <2>  Invoke parent's CloneInto method.
 * <3>  Clone the following variables into the toSU:
 *        ODLinkConnectionID    fIDCounter;
 *        ODLinkID           fLinkID;
 *        OpenHashTable        *fLocalLinkInfos;
 *        OpenHashTable        *fRemoteLinkInfos;
 *        ODLinkConnectionData fSrcConnData;
 *        State              fState;
 *        ODBoolean          fTrackTrgts;
 *=============================================================================
 */

SOM_Scope void
SOMLINK CloneInto(IODLinkSource *somSelf,
                  Environment *ev,
                  ODDraftKey draftKey,
                  ODStorageUnit* toSU,
                  ODFrame* scopeFrame)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","CloneInto");

    ODDraft *draft = _GetStorageUnit(ev)->GetDraft(ev);

    //---<1>
    if (toSU->Exists(ev, kODPropIDCounter, kODULong, 0)) return;

    //---<2>
    IODLinkSource_parent_ODLinkSource_CloneInto(somSelf,
                                                ev,
                                                draftKey,
                                                toSU,
                                                scopeFrame);

    //---<3>
    ODSetULongProp(ev,toSU, kODPropIDCounter, kODULong, _fIDCounter);

    ODSetULongProp(ev,toSU, kODPropLinkID, kODULong, _fLinkID);

    CloneLinkInfosProp(ev,
                       toSU,
                       kODPropLocalLinkInfos,
                       kODHashTable,
                       draft,
                       draftKey,
                       scopeFrame,
                       *_fLocalLinkInfos);

    CloneLinkInfosProp(ev,
                       toSU,
                       kODPropRemoteLinkInfos,
                       kODHashTable,
                       draft,
                       draftKey,
                       scopeFrame,
                       *_fRemoteLinkInfos);

    SetPartDescriptionProp(ev,
                           toSU,
                           kODPropPartDescription,
                           kODPartDescription,
                           _fSrcConnData);

    ODSetULongProp(ev,toSU, kODPropLinkSourceState, kODULong, _fState);

    ODSetBooleanProp(ev,toSU,
                     kODPropLinkSourceTrackMode,
                     kODBoolean,
                     _fTrackTrgts);
}

/*
 *=============================================================================
 * <1>  Invoke parent's Externalize method.
 * <2>  Externalize the following variables to the storage unit:
 *        ODLinkConnectionID    fIDCounter;
 *        ODLinkID           fLinkID;
 *        OpenHashTable        *fLocalLinkInfos;
 *        OpenHashTable        *fRemoteLinkInfos;
 *        ODLinkConnectionData fSrcConnData;
 *        State              fState;
 *        ODBoolean          fTrackTrgts;
 *=============================================================================
 */

SOM_Scope void
SOMLINK Externalize(IODLinkSource *somSelf,
                    Environment *ev)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","Externalize");

    ODStorageUnit *su = _GetStorageUnit(ev);

    //---<1>
    IODLinkSource_parent_ODLinkSource_Externalize(somSelf, ev);

    //---<2>
    ODSetULongProp(ev,su, kODPropIDCounter, kODULong, _fIDCounter);

    ODSetULongProp(ev,su, kODPropLinkID, kODULong, _fLinkID);

    SetLinkInfosProp(ev,
                     su,
                     kODPropLocalLinkInfos,
                     kODHashTable,
                     *_fLocalLinkInfos);

    SetLinkInfosProp(ev,
                     su,
                     kODPropRemoteLinkInfos,
                     kODHashTable,
                     *_fRemoteLinkInfos);

    SetPartDescriptionProp(ev,
                           su,
                           kODPropPartDescription,
                           kODPartDescription,
                           _fSrcConnData);

    ODSetULongProp(ev,su, kODPropLinkSourceState, kODULong, _fState);

    ODSetBooleanProp(ev,su,
                     kODPropLinkSourceTrackMode,
                     kODBoolean,
                     _fTrackTrgts);
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODUpdateID
SOMLINK ForwardGetUpdateID(IODLinkSource *somSelf,
                           Environment *ev,
                           ODLinkID linkID)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ForwardGetUpdateID");

    return _GetUpdateID(ev);
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODTime
SOMLINK ForwardGetChangeTime(IODLinkSource *somSelf,
                             Environment *ev,
                             ODLinkID linkID)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ForwardGetChangeTime");

    return _GetChangeTime(ev);
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODStorageUnit*
SOMLINK ForwardGetContentStorageUnit(IODLinkSource *somSelf,
                                     Environment *ev,
                                     ODLinkID linkID,
                                     ODLinkKey key)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ForwardGetContentStorageUnit");

    return _GetContentStorageUnit(ev, key);
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODBoolean
SOMLINK ForwardLock(IODLinkSource *somSelf,
                    Environment *ev,
                    ODLinkID linkID,
                    ODULong wait,
                    ODLinkKey* key)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ForwardLock");

#ifdef _PLATFORM_OS2_
    return (WaitForLock(somSelf,
                        ev,
                        wait,
                        key,
                        &_fLockState,
                        _fMutex,
                        &_fFirstWE,
                        &_fLastWE));
#else
    if (_fLockState == IODLinkSource_UNLOCKED)
    {
      ODBoolean success =  _Lock(ev, wait, key);
      if(success)
      {
       _fLockState = IODLinkSource_LOCKED;
      }
      return (success);
    }
    else
    {
      return kODFalse;
    }
#endif
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODBoolean
SOMLINK Lock(IODLinkSource *somSelf,
                    Environment *ev,
                    ODULong wait,
                    ODLinkKey* key)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ForwardLock");

#ifdef _PLATFORM_OS2_
    return (WaitForLock(somSelf,
                        ev,
                        wait,
                        key,
                        &_fLockState,
                        _fMutex,
                        &_fFirstWE,
                        &_fLastWE));
#else
  return (IODLinkSource_parent_ODLinkSource_Lock(somSelf, ev, wait, key));
#endif
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK ForwardUnlock(IODLinkSource *somSelf,
                      Environment *ev,
                      ODLinkID linkID,
                      ODLinkKey key,
                      ODBoolean isLocal)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ForwardUnlock");

    switch (_fLockState)
    {
      case IODLinkSource_UNLOCKED:
        ODSetSOMException(ev, kODErrInvalidLinkKey);
        return;

      case IODLinkSource_LOCKED:
        _Unlock(ev, key);  /* on OS2 this will set fLockState */
       #ifndef _PLATFORM_OS2_
        _fLockState = IODLinkSource_UNLOCKED;
        #endif
        return;

      case IODLinkSource_LOCKEDINAS:
        if (isLocal)
        {
          ODSetSOMException(ev, kODErrInvalidLinkKey);
          return;
        }
        _fLockState = IODLinkSource_UNLOCKED;
        return;
    }
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODLink*
SOMLINK GetLink(IODLinkSource *somSelf,
                Environment *ev)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","GetLink");

    //---<1>
    return kODNULL;
}

/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fSrcConnData.
 * <3>  Allocate memory for the out parameter srcPartDesc.
 *      Copy the source part description to the out parameter srcPartDesc.
 *      Release fMutex and return.
 * <4>  An exception is caught.
 *      Release fMutex.
 *      Cleanup srcPartDesc.
 *      Set SOM exception and return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK GetSourcePartDescription(IODLinkSource *somSelf,
                                 Environment *ev,
                                 ODLinkID linkID,
                                 ODLinkConnectionData** srcPartDesc)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","GetSourcePartDescription");

    // Out parameter
    *srcPartDesc = kODNULL;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    TRY

      //---<3>
      *srcPartDesc = new ODLinkConnectionData;
      THROW_IF_NULL(*srcPartDesc);

      CopyPartDescription(**srcPartDesc, _fSrcConnData);
      _fMutex->Release(ev);

    CATCH_ALL

      //---<4>
      _fMutex->Release(ev);

      if (*srcPartDesc != kODNULL)
      {
        CleanupPartDescription(**srcPartDesc);
        delete *srcPartDesc;
        *srcPartDesc = kODNULL;
      }

      SET_SOM_EV;

    ENDTRY
}


/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fSrcConnData.
 * <3>  Allocate memory for the out parameter srcPartDesc.
 *      Copy the source part description to the out parameter srcPartDesc.
 *      Release fMutex and return.
 * <4>  An exception is caught.
 *      Release fMutex.
 *      Cleanup srcPartDesc.
 *      Set SOM exception and return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK GetConnectionData(IODLinkSource *somSelf,
                     Environment *ev,
                     ODLinkConnectionData** srcConnData)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","GetSourcePartDescription");

    // Out parameter
    *srcConnData = kODNULL;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    TRY

      //---<3>
      *srcConnData = new ODLinkConnectionData;
      THROW_IF_NULL(*srcConnData);

      CopyPartDescription(**srcConnData, _fSrcConnData);
      _fMutex->Release(ev);

    CATCH_ALL

      //---<4>
      _fMutex->Release(ev);

      if (*srcConnData != kODNULL)
      {
        CleanupPartDescription(**srcConnData);
        delete *srcConnData;
        *srcConnData = kODNULL;
      }

      SET_SOM_EV;

    ENDTRY
}

/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 * <4>  Set the isAlive field of linkInfo to false.
 *      Release fMutex.
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkTargetClosed(IODLinkSource *somSelf,
                         Environment *ev,
                         ODLinkID linkID,
                         ODLinkConnectionID trgtID,
                         ODBoolean isLocal)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","LinkTargetClosed");

    LinkInfo *linkInfo = kODNULL;
    OpenHashTable *linkInfos = isLocal ? _fLocalLinkInfos : _fRemoteLinkInfos;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
    if (linkInfos->GetValue(&trgtID, &linkInfo) != kODTrue)
    {
      ODSetSOMException(ev, kODErrInvalidTargetID);
      _fMutex->Release(ev);
      return;
    }

    //---<4>
    linkInfo->isAlive = kODFalse;
    _fMutex->Release(ev);
}

/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 * <4>  Remove linkInfo from fLinkInfos.
 *      Release fMutex.
 * <5>  Delete linkInfo.
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkTargetRemoved(IODLinkSource *somSelf,
                          Environment *ev,
                          ODLinkID linkID,
                          ODLinkConnectionID trgtID,
                          ODBoolean isLocal)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","LinkTargetRemoved");

    LinkInfo *linkInfo = kODNULL;
    OpenHashTable *linkInfos = isLocal ? _fLocalLinkInfos : _fRemoteLinkInfos;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
    if (linkInfos->GetValue(&trgtID, &linkInfo) != kODTrue)
    {
      ODSetSOMException(ev, kODErrInvalidTargetID);
      _fMutex->Release(ev);
      return;
    }

    //---<4>
    linkInfos->RemoveEntry(&trgtID);

    //---<5>
    CleanupPartDescription(linkInfo->trgtPartDesc);
    delete linkInfo;
}

/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Determine the new locality of the LinkTarget.
 * <3>  Acquire fMutex before accessing fLinkInfos.
 * <4>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 * <5>  Delete old target part description.
 * <6>  Fill in linkInfo with new information.
 * <7>  If locality of LinkTarget has been changed then move linkInfo to
 *      the corresponding hashtable.
 * <8>  Release fMutex and return.
 * <9>  An exception is caught.
 *      Release fMutex.
 *      Cleanup linkInfo->trgtPartDesc.
 *      Set SOM exception and return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK LinkTargetReopened(IODLinkSource *somSelf,
                           Environment *ev,
                           ODLinkID linkID,
                           ODLinkConnectionID trgtID,
                           ODULong pid,
                           ODLinkConnectionData* trgtPartDesc,
                           IODLinkTarget* linkTrgt,
                           ODBoolean* isLocal)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","LinkTargetReopened");

    LinkInfo *linkInfo = kODNULL;
    OpenHashTable *linkInfos = (*isLocal) ? _fLocalLinkInfos : _fRemoteLinkInfos;
    ODBoolean wasLocal = *isLocal;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    //*isLocal = ODISOStrEqual(_fSrcPartDesc.docName, trgtPartDesc->docName);
    *isLocal = (_fPID == pid) ? kODTrue : kODFalse;

    //---<3>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<4>
    if (linkInfos->GetValue(&trgtID, &linkInfo) != kODTrue)
    {
      ODSetSOMException(ev, kODErrInvalidTargetID);
      _fMutex->Release(ev);
      return;
    }

    TRY

      //---<5>
      CleanupPartDescription(linkInfo->trgtPartDesc);

      //---<6>
      CopyPartDescription(linkInfo->trgtPartDesc, *trgtPartDesc);

      linkInfo->linkTrgt = linkTrgt;
      linkInfo->isAlive = kODTrue;

      //---<7>
      if (wasLocal != *isLocal)
      {
        linkInfos->RemoveEntry(&trgtID);
        if (*isLocal)
        {
          _fLocalLinkInfos->ReplaceEntry(&trgtID, &linkInfo);
        }
        else
        {
          _fRemoteLinkInfos->ReplaceEntry(&trgtID, &linkInfo);
        }
      }

      //---<8>
      _fMutex->Release(ev);

    CATCH_ALL

      //---<9>
      _fMutex->Release(ev);

      CleanupPartDescription(linkInfo->trgtPartDesc);

      SET_SOM_EV;

    ENDTRY
}

/*
 *=============================================================================
 * No op for now
 *=============================================================================
 */

SOM_Scope void
SOMLINK LockToRead(IODLinkSource *somSelf,
                   Environment *ev,
                   ODLinkID linkID,
                   ODULong wait,
                   ODLinkKey* key)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","LockToRead");

    IODLinkSource_parent_IODLinkTargetToLinkSource_LockToRead(somSelf,
                                                              ev,
                                                              linkID,
                                                              wait,
                                                              key);
}

/*
 *=============================================================================
 * <1>  Invoke parent's Release method.
 * <2>  If LinkSource's reference count goes down to zero inform the Draft
 *      to release the LinkSource.
 *=============================================================================
 */

SOM_Scope void
SOMLINK Release(IODLinkSource *somSelf,
                Environment *ev)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","Release");

    //---<1>
    IODLinkSource_parent_ODLinkSource_Release(somSelf, ev);

    //---<2>
    if (_GetRefCount(ev) == 0)
    {
      _GetStorageUnit(ev)->GetDraft(ev)->ReleaseLinkSource(ev, somSelf);
    }
}

/*
 *=============================================================================
 * <1>  Invoke parent's ReleaseAll method.
 * <2>  Delete fMutex.
 * <3>  Reset fIDCounter to zero.
 * <4>  Reset fLinkID to zero.
 * <5>  Delete fLocalLinkInfos and fRemoteLinkInfos.
 * <6>  Free each fields of fSrcConnData.
 * <7>  Reset current state to EMPTY.
 * <8>  Reset fTrackTrgts to false.
 *=============================================================================
 */

SOM_Scope void
SOMLINK ReleaseAll(IODLinkSource *somSelf,
                   Environment *ev)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ReleaseAll");

    ODLinkConnectionID id;
    LinkInfo *linkInfo = kODNULL;

    //---<1>
    IODLinkSource_parent_ODLinkSource_ReleaseAll(somSelf, ev);

    //---<2>
    delete _fMutex;

    //---<3>
    _fIDCounter = 0;

    //---<4>
    _fLinkID = 0;

    //---<5>
    if (_fLocalLinkInfos!=kODNULL)
     {
       OpenHashTableIterator lInfos(_fLocalLinkInfos);
       for (lInfos.First(&id, &linkInfo);
            lInfos.IsNotComplete();
            lInfos.Next(&id, &linkInfo))
         {
          CleanupPartDescription(linkInfo->trgtPartDesc);
          linkInfo->linkTrgt->Release(ev);
          delete linkInfo;
          }

       delete _fLocalLinkInfos;
       _fLocalLinkInfos = kODNULL;
     } //_fLocalLinkInfos != kODNULL

    if (_fRemoteLinkInfos!=kODNULL)
     {
       OpenHashTableIterator rInfos(_fRemoteLinkInfos);
       for (rInfos.First(&id, &linkInfo);
            rInfos.IsNotComplete();
            rInfos.Next(&id, &linkInfo))
         {
          CleanupPartDescription(linkInfo->trgtPartDesc);
          delete linkInfo;
         }

       delete _fRemoteLinkInfos;
       _fRemoteLinkInfos = kODNULL;
     } //_fRemoteLinkInfos != kODNULL

    //---<6>
    CleanupPartDescription(_fSrcConnData);

    //---<7>
    _fState = IODLinkSource_EMPTY;

    //---<8>
    _fTrackTrgts = kODFalse;

}

/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 * <4>  Set the isRegistered field of linkInfo to true.
 *      Release fMutex.
 *=============================================================================
 */

SOM_Scope void
SOMLINK RegisterTrackedLinkTarget(IODLinkSource *somSelf,
                                  Environment *ev,
                                  ODLinkID linkID,
                                  ODLinkConnectionID trgtID,
                                  ODBoolean isLocal)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","RegisterTrackedLinkTarget");

    LinkInfo *linkInfo = kODNULL;
    OpenHashTable *linkInfos = isLocal ? _fLocalLinkInfos : _fRemoteLinkInfos;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
    if (linkInfos->GetValue(&trgtID, &linkInfo) != kODTrue)
    {
      ODSetSOMException(ev, kODErrInvalidTargetID);
      _fMutex->Release(ev);
      return;
    }

    //---<4>
    linkInfo->isRegistered = kODTrue;
    _fMutex->Release(ev);
}

/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Allocate a new linkInfo.
 * <4>  Fill in each field of linkInfo.
 * <5>  Add linkInfo to fLinkInfos.
 *      Release fMutex.
 *      Return the connection ID to caller.
 * <6>  An exception is caught.
 *      Release fMutex.
 *      Cleanup linkInfo.
 *      Set SOM exception and return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK RegisterUntrackedLinkTarget(IODLinkSource *somSelf,
                                    Environment *ev,
                                    ODLinkID linkID,
                                    IODLinkTarget* linkTrgt,
                                    ODLinkConnectionData* trgtPartDesc,
                                    ODBoolean isLocal,
                                    ODLinkConnectionID* trgtID)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","RegisterUntrackedLinkTarget");

    LinkInfo *linkInfo = kODNULL;
    OpenHashTable *linkInfos = isLocal ? _fLocalLinkInfos : _fRemoteLinkInfos;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    TRY

      //---<3>
      linkInfo = new LinkInfo;
      THROW_IF_NULL(linkInfo);

      //---<4>
      if (isLocal)
      {
        linkTrgt->Acquire(ev);
      }
      linkInfo->trgtID = ++_fIDCounter;
      linkInfo->linkTrgt = linkTrgt;
      linkInfo->isAlive = kODTrue;
      linkInfo->isRegistered = kODTrue;
      CopyPartDescription(linkInfo->trgtPartDesc, *trgtPartDesc);

      //---<5>
      linkInfos->ReplaceEntry(&_fIDCounter, &linkInfo);
      _fMutex->Release(ev);
      *trgtID = linkInfo->trgtID;
      return;

    CATCH_ALL

      //---<6>
      _fMutex->Release(ev);
      delete linkInfo;

      SET_SOM_EV;
      return;

    ENDTRY
}

/*
 *=============================================================================
 * XXXXX For now:
 * <1>  Invoke parent's SetAutoUpdate method.
 *=============================================================================
 */

SOM_Scope void
SOMLINK SetAutoUpdate(IODLinkSource *somSelf,
                      Environment *ev,
                      ODBoolean automatic)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","SetAutoUpdate");

    //---<1>
    IODLinkSource_parent_ODLinkSource_SetAutoUpdate(somSelf, ev, automatic);
}


/*
 *=============================================================================
 * <1>  Invoke parent's ShowSourceContent method if caller is from the same
 *      host.
 *=============================================================================
 */

SOM_Scope void
SOMLINK ShowSourceContentFor(IODLinkSource *somSelf,
                             Environment *ev,
                             ODLinkID linkID,
                             string hostName)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","ShowSourceContentFor");

    //---<1>
    if (!strcmp(GetCStringFromIText(&_fSrcConnData.HostName), hostName))
      {
      _ShowSourceContent(ev);
      }
    else
      {
       ODSetSOMException(ev, kODErrSourceDocRemote);
      }
}
/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 * <4>  Delete old target part description.
 * <5>  Save new target part description.
 *      Release fMutex and return.
 * <6>  An exception is caught.
 *      Release fMutex.
 *      Cleanup linkInfo->trgPartDesc.
 *      Set SOM exception and return.
 *=============================================================================
 */

SOM_Scope void
SOMLINK TargetPartDescriptionChanged(IODLinkSource *somSelf,
                                     Environment *ev,
                                     ODLinkID linkID,
                                     ODLinkConnectionID trgtID,
                                     ODBoolean isLocal,
                                     ODLinkConnectionData* trgtPartDesc)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","TargetPartDescriptionChanged");

    LinkInfo *linkInfo = kODNULL;
    OpenHashTable *linkInfos = isLocal ? _fLocalLinkInfos : _fRemoteLinkInfos;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
    if (linkInfos->GetValue(&trgtID, &linkInfo) != kODTrue)
    {
      ODSetSOMException(ev, kODErrInvalidTargetID);
      _fMutex->Release(ev);
      return;
    }

    //---<4>
    CleanupPartDescription(linkInfo->trgtPartDesc);

    TRY

      //---<5>
      CopyPartDescription(linkInfo->trgtPartDesc, *trgtPartDesc);
      _fMutex->Release(ev);

    CATCH_ALL

      //---<6>
      _fMutex->Release(ev);

      CleanupPartDescription(linkInfo->trgtPartDesc);

      SET_SOM_EV;

    ENDTRY
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK Unlock(IODLinkSource *somSelf,
               Environment *ev,
               ODLinkKey key)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","Unlock");

    ODUpdateID      changeIDold = _GetUpdateID(ev);
    ODLinkConnectionID id;

#if 0           // dr3
    ODBoolean       isChanged = _IsChanged(ev, key);
#endif

    LinkInfo*       linkInfo = kODNULL;
    ODBoolean       isChanged ;

    IODLinkSource_parent_ODLinkSource_Unlock(somSelf, ev, key);

    ODUpdateID      changeIDnew = _GetUpdateID(ev);

/*
     if ( changeID !=kODUnknownUpdate)
           isChanged = kODTrue;
     else
           isChanged = kODFalse;
 */

     if ( changeIDold != changeIDnew)
           isChanged = kODTrue;
     else
        {
           if (_GetUpdateContinue(ev)) {
             isChanged = kODTrue;
           } else {
             isChanged = kODFalse;
           } /* endif */
        }

    if (ev->_major) {
      ODSetSOMException(ev, kODNoError);
    }

#ifdef _PLATFORM_OS2_
    if (key != 1) {
      ODSetSOMException(ev, kODErrInvalidLinkKey);
      return;
      }
#endif /* _PLATFORM_OS2_ */

    _fMutex->Acquire(ev);

    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

   #ifdef _PLATFORM_OS2_
    // The following actions are taken to support waiting for unlock
    // 1. Dequeue and free any timed out LockWaitElements
    // 2. Dequeue first non-timed-out LockWaitElement, if any.
    // 3. If it exists,
    //    3.1. try to lock it
    //    3.2. if lock fails, save key as 0, wakeup and repeat 1.
    //    3.3. if lock succeeds, save key, wakeup leaving state LOCKED
    // 4. If it doesn't exist, leave state UNLOCKED

    // 1.
    _fLockState = IODLinkSource_UNLOCKED;
    for (LockWaitElement* LWE = _fFirstWE; LWE != kODNULL; LWE = _fFirstWE) {
      if (_fFirstWE == _fLastWE) {
        _fFirstWE = (LockWaitElement*) kODNULL;
        _fLastWE = (LockWaitElement*) kODNULL;
      } else {
        _fFirstWE = LWE->next;
      }

      if (LWE->timedout) {
        FreeLWE(LWE);
      } else {
        APIRET rc;
        // 3.1.
        ODBoolean success =
                  IODLinkSource_parent_ODLinkSource_Lock(somSelf,
                                                         ev,
                                                         0,
                                                         &LWE->key);
        if (success) {
          // 3.2.
          _fLockState = IODLinkSource_LOCKED;
          rc = DosPostEventSem(LWE->event);
          break;
        } else {
          // 3.3.
          LWE->key = 0;
          rc = DosPostEventSem(LWE->event);
        }
      }
    }
    // 4. (nop: we either left by successful WAIT or by end of list)
    #endif

    if (isChanged == kODTrue)
    {
      OpenHashTableIterator lInfos(_fLocalLinkInfos);
      for (lInfos.First(&id, &linkInfo);
           lInfos.IsNotComplete();
           lInfos.Next(&id, &linkInfo))
      {
        if (linkInfo->isRegistered)
        {
          linkInfo->linkTrgt->UpdateDependents(ev, changeIDold);
        }
      }

      OpenHashTableIterator rInfos(_fRemoteLinkInfos);
      for (rInfos.First(&id, &linkInfo);
           rInfos.IsNotComplete();
           rInfos.Next(&id, &linkInfo))
      {
        if (linkInfo->isRegistered)
        {
          SOM_TRY
            linkInfo->linkTrgt->UpdateDependents(ev, changeIDold);
          SOM_CATCH_ALL
            if (!_fTrackTrgts)
            {
              //XXXXX Should delete linkInfo->linkTrgt proxy
              _fRemoteLinkInfos->RemoveEntry(&id);
              delete linkInfo;
            }
            else
            {
              linkInfo->isRegistered = kODFalse;
              linkInfo->isAlive = kODFalse;
              //XXXXX Should delete linkInfo->linkTrgt proxy
            }
            ODSetSOMException(ev, kODNoError);
          SOM_ENDTRY;
        }
      }
    }
    _fMutex->Release(ev);
}

/*
 *=============================================================================
 * <1>  Make sure the current state is not EMPTY.
 * <2>  Acquire fMutex before accessing fLinkInfos.
 * <3>  Retrieve linkInfo associated with trgtID from fLinkInfos.
 * <4>  If caller is a untracked remote LinkTarget then remove its entry from
 *      fLinkInfos and delete linkInfo.
 * <5>  Otherwise set the isRegistered field of linkInfo to false.
 * <6>  Release fMutex.
 *=============================================================================
 */

SOM_Scope void
SOMLINK UnregisterLinkTarget(IODLinkSource *somSelf,
                             Environment *ev,
                             ODLinkID linkID,
                             ODLinkConnectionID trgtID,
                             ODBoolean isLocal)
{
    IODLinkSourceData *somThis = IODLinkSourceGetData(somSelf);
    IODLinkSourceMethodDebug("IODLinkSource","UnregisterLinkTarget");

    LinkInfo *linkInfo = kODNULL;
    OpenHashTable *linkInfos = isLocal ? _fLocalLinkInfos : _fRemoteLinkInfos;

    //---<1>
    if (_fState == IODLinkSource_EMPTY)
    {
      ODSetSOMException(ev, kODErrLinkSourceIsEmpty);
      return;
    }

    //---<2>
    _fMutex->Acquire(ev);
    if (ODGetSOMException(ev) != kODNoError)
    {
      return;
    }

    //---<3>
    if (linkInfos->GetValue(&trgtID, &linkInfo) != kODTrue)
    {
      ODSetSOMException(ev, kODErrInvalidTargetID);
      _fMutex->Release(ev);
      return;
    }

    if (!_fTrackTrgts && !isLocal)
    {
      //---<4>
      linkInfos->RemoveEntry(&trgtID);

      // No need to delete linkInfo->trgtPartDesc since it's NULL
      delete linkInfo;
    }
    else
    {
      //---<5>
      linkInfo->isRegistered = kODFalse;
    }

    //---<6>
     _fMutex->Release(ev);
    }

#endif
