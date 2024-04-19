/* @(#)Z 1.3 os2/src/storage/iltr2lsr.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:53 (96/08/23 01:44:02) */
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
#ifndef SOM_Module_iltr2lsr_Source
#define SOM_Module_iltr2lsr_Source
#endif
#define IODLinkTargetToLinkSource_Class_Source

#include <IText.h>
#include <ISOStr.h>
#include <IODDefs.xh>

#include "ILTr2LSr.xih"


SOM_Scope ODUpdateID  SOMLINK ForwardGetUpdateID(IODLinkTargetToLinkSource *somSelf,
                                                  Environment *ev,
                                                 ODLinkID linkID)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","ForwardGetUpdateID");

    /* Return statement to be customized: */
    { ODUpdateID retVal;  return (retVal); }
}

SOM_Scope ODTime  SOMLINK ForwardGetChangeTime(IODLinkTargetToLinkSource *somSelf,
                                                Environment *ev,
                                               ODLinkID linkID)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","ForwardGetChangeTime");

    /* Return statement to be customized: */
    { ODTime retVal;  return (retVal); }
}

SOM_Scope ODStorageUnit*  SOMLINK ForwardGetContentStorageUnit(IODLinkTargetToLinkSource *somSelf,
                                                                Environment *ev,
                                                               ODLinkID linkID,
                                                               ODLinkKey key)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","ForwardGetContentStorageUnit");

    /* Return statement to be customized: */
    { ODStorageUnit* retVal;  return (retVal); }
}

SOM_Scope ODBoolean  SOMLINK ForwardLock(IODLinkTargetToLinkSource *somSelf,
                                          Environment *ev, ODLinkID linkID,
                                         ODULong wait, ODLinkKey* key)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","ForwardLock");

    /* Return statement to be customized: */
    { ODBoolean retVal;  return (retVal); }
}

SOM_Scope void  SOMLINK ForwardUnlock(IODLinkTargetToLinkSource *somSelf,
                                       Environment *ev, ODLinkID linkID,
                                      ODLinkKey key, ODBoolean isLocal)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","ForwardUnlock");

}

SOM_Scope void  SOMLINK GetSourcePartDescription(IODLinkTargetToLinkSource *somSelf,
                                                  Environment *ev,
                                                 ODLinkID linkID,
                                                 ODLinkConnectionData** srcPartDesc)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","GetSourcePartDescription");

}

SOM_Scope void  SOMLINK LinkTargetClosed(IODLinkTargetToLinkSource *somSelf,
                                          Environment *ev, ODLinkID linkID,
                                         ODLinkConnectionID trgtID,
                                         ODBoolean isLocal)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","LinkTargetClosed");

}

SOM_Scope void  SOMLINK LinkTargetRemoved(IODLinkTargetToLinkSource *somSelf,
                                           Environment *ev, ODLinkID linkID,
                                          ODLinkConnectionID trgtID,
                                          ODBoolean isLocal)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","LinkTargetRemoved");

}

SOM_Scope void  SOMLINK LinkTargetReopened(IODLinkTargetToLinkSource *somSelf,
                                            Environment *ev,
                                           ODLinkID linkID, ODLinkConnectionID trgtID, ODULong pid,
                                           ODLinkConnectionData* trgtPartDesc,
                                           IODLinkTarget* linkTrgt,
                                           ODBoolean* isLocal)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","LinkTargetReopened");

}

SOM_Scope void  SOMLINK LockToRead(IODLinkTargetToLinkSource *somSelf,
                                    Environment *ev, ODLinkID linkID,
                                   ODULong wait, ODLinkKey* key)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","LockToRead");

}

SOM_Scope void  SOMLINK RegisterTrackedLinkTarget(IODLinkTargetToLinkSource *somSelf,
                                                   Environment *ev,
                                                  ODLinkID linkID,
                                                  ODLinkConnectionID trgtID,
                                                  ODBoolean isLocal)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","RegisterTrackedLinkTarget");

}

SOM_Scope void  SOMLINK RegisterUntrackedLinkTarget(IODLinkTargetToLinkSource *somSelf,
                                                     Environment *ev,
                                                    ODLinkID linkID,
                                                    IODLinkTarget* linkTrgt,
                                                   ODLinkConnectionData* trgtPartDesc,
                                                    ODBoolean isLocal,
                                                    ODLinkConnectionID* trgtID)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","RegisterUntrackedLinkTarget");

}

SOM_Scope void  SOMLINK ShowSourceContentFor(IODLinkTargetToLinkSource *somSelf,
                                              Environment *ev,
                                             ODLinkID linkID,
                                             ODISOStr hostName)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","ShowSourceContentFor");

}

SOM_Scope void  SOMLINK TargetPartDescriptionChanged(IODLinkTargetToLinkSource *somSelf,
                                                      Environment *ev,
                                                     ODLinkID linkID,
                                                     ODLinkConnectionID trgtID,
                                                     ODBoolean isLocal,
                                                     ODLinkConnectionData* trgtPartDesc)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","TargetPartDescriptionChanged");

}

SOM_Scope void  SOMLINK UnregisterLinkTarget(IODLinkTargetToLinkSource *somSelf,
                                              Environment *ev,
                                             ODLinkID linkID,
                                             ODLinkConnectionID trgtID,
                                             ODBoolean isLocal)
{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource","UnregisterLinkTarget");

}

#endif
SOM_Scope void  SOMLINK AddLinkTarget(IODLinkTargetToLinkSource *somSelf,
                                              Environment *ev,
                                             ODLinkID linkID,
                                             ODULong pid,
                                             ODLinkConnectionData *trgtPartDesc,
                                             IODLinkTarget *linkTrgt,
                                             ODStorageUnitID *sourceSUID,
                                             ODLinkConnectionID *trgtID,
                                             ODBoolean *isLocal,
                                             ODBoolean *trackTrgt)

{
    /* IODLinkTargetToLinkSourceData *somThis = IODLinkTargetToLinkSourceGetData(somSelf); */
    IODLinkTargetToLinkSourceMethodDebug("IODLinkTargetToLinkSource", "AddLinkTarget");


}

