/* @(#)Z 1.8 com/src/storage/Draft.cpp, odstorage, od96os2, odos29646d 96/11/15 15:25:50 (96/10/29 09:30:56) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
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

/*
        File:           Draft.cpp

        Contains:       Implementation of ODDraft

        Owned by:       Vincent Lo

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <22>     9/11/95        VL              1283326: CreateFrame now takes ODObjectType
                                                                        instead of ODType
                <21>     8/25/95        JBS             1263078 FB: fix part editor swapping
                <20>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <19>     6/23/95        JBS             1261323 Draft::CreateFrame: add isSubframe
                                                                        param, remove isRoot
                <18>     6/19/95        VL              1170098: Added DeleteRealPart.
                <17>     5/26/95        VL              1251403: Multithreading naming support.
                <16>     5/22/95        CC              1242879: Added IsValidID.
                <15>     5/18/95        CC              1238898: Added destFrame parameter to
                                                                        BeginClone.
                <14>     5/17/95        VL              1170098: Added ConstructRealPart and
                                                                        ReleaseRealPart.
                <13>     1/18/95        VL              1193559: Two new methods
                                                                        (AcquirePersistentObject and
                                                                        GetPersistentObjectID).
                <12>     1/11/95        VL              1185688: Made storage more robust in terms
                                                                        of error handling. Also did some code
                                                                        review cleanup.
                <11>    12/15/94        CC              1193569 CreateLinkSpec() takes ODByteArray
                                                                        parameter.
                <10>     10/4/94        CC              1190858 - Added ReleaseLinkSource().
                 <9>     9/26/94        VL              1185419: Added SubClassResponsibility to
                                                                        methods.
                 <8>      9/6/94        VL              1184177: Removed GetName and SetName.
                 <7>     8/26/94        VL              1183174: Added parameters to Clone and
                                                                        implementation to the method.
                 <6>     8/16/94        VL              1143605: Added Clone.
                 <5>     8/15/94        VL              #???: Added frameType parameter to
                                                                        CreateFrame.
                 <4>     8/12/94        JBS             1179919: add biasCanvas param to
                                                                        Draft::CreateFrame()
                 <3>     8/11/94        CC              Added CreateLinkIterator() and
                                                                        CreateLinkSourceIterator(), implementation
                                                                        is subclass responsibility.
                 <2>      7/5/94        VL              Added SubClassResponsibility.
                 <1>     6/28/94        VL              first checked in

        To Do:
*/

#define ODDraft_Class_Source
#include <Draft.xih>

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif // _PLATFORM_WIN32_ _PLATFORM_OS2_ _PLATFORM_AIX_

SOM_Scope ODDocument*  SOMLINK ODDraftGetDocument(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftGetDocument");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODDraftID  SOMLINK ODDraftGetID(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftGetID");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODStorageUnit*  SOMLINK ODDraftAcquireDraftProperties(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquireDraftProperties");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODDraftPermissions  SOMLINK ODDraftGetPermissions(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftGetPermissions");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODStorageUnit*  SOMLINK ODDraftCreateStorageUnit(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreateStorageUnit");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODStorageUnit*  SOMLINK ODDraftAcquireStorageUnit(ODDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquireStorageUnit");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope void  SOMLINK ODDraftRemoveStorageUnit(ODDraft *somSelf, Environment *ev,
                ODStorageUnit* storageUnit)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRemoveStorageUnit");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODBoolean  SOMLINK ODDraftIsValidID(ODDraft *somSelf, Environment *ev,
                ODID id)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","IsValidID");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODFalse;
}

SOM_Scope ODDraftKey  SOMLINK ODDraftBeginClone(ODDraft *somSelf, Environment *ev,
                ODDraft* destDraft,
                ODFrame* destFrame,
                ODCloneKind kind)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftBeginClone");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return 0;
}

SOM_Scope void  SOMLINK ODDraftEndClone(ODDraft *somSelf, Environment *ev,
                ODDraftKey key)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftEndClone");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftAbortClone(ODDraft *somSelf, Environment *ev,
                ODDraftKey key)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAbortClone");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODID  SOMLINK ODDraftClone(ODDraft *somSelf, Environment *ev,
                ODDraftKey key,
                ODID fromObjectID,
                ODID toObjectID,
                ODID scope)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftClone");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
        return 0;
}

SOM_Scope ODID  SOMLINK ODDraftWeakClone(ODDraft *somSelf, Environment *ev,
                ODDraftKey key,
                ODID objectID,
                ODID toObjectID,
                ODID scope)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftWeakClone");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
        return 0;
}

SOM_Scope ODBoolean  SOMLINK ODDraftChangedFromPrev(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftChangedFromPrev");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODFalse;
}

SOM_Scope void  SOMLINK ODDraftSetChangedFromPrev(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftSetChangedFromPrev");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftRemoveFromDocument(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRemoveFromDocument");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODDraft*  SOMLINK ODDraftRemoveChanges(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRemoveChanges");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return somSelf;
}

SOM_Scope ODDraft*  SOMLINK ODDraftExternalize(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftExternalize");

        return somSelf;
}

SOM_Scope ODDraft*  SOMLINK ODDraftSaveToAPrevious(ODDraft *somSelf, Environment *ev,
                ODDraft* to)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftSaveToAPrevious");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return somSelf;
}

SOM_Scope ODFrame*  SOMLINK ODDraftCreateFrame(ODDraft *somSelf, Environment *ev,
                ODObjectType    frameType,
                ODFrame* containingFrame,
                ODShape* frameShape,
                ODCanvas* biasCanvas,
                ODPart* part,
                ODTypeToken viewType,
                ODTypeToken presentation,
                ODBoolean isSubframe,
                ODBoolean isOverlaid)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreateFrame");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODFrame*  SOMLINK ODDraftAcquireFrame(ODDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquireFrame");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope void  SOMLINK ODDraftRemoveFrame(ODDraft *somSelf, Environment *ev,
                ODFrame* frame)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRemoveFrame");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODPart*  SOMLINK ODDraftCreatePart(ODDraft *somSelf, Environment *ev,
                ODType partType,
                ODEditor optionalEditor)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreatePart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODPart*  SOMLINK ODDraftAcquirePart(ODDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquirePart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope void  SOMLINK ODDraftRemovePart(ODDraft *somSelf, Environment *ev,
                ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRemovePart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODLinkSpec*  SOMLINK ODDraftCreateLinkSpec(ODDraft *somSelf, Environment *ev,
                ODPart* part,
                ODByteArray* data)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreateLinkSpec");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODLinkSource*  SOMLINK ODDraftCreateLinkSource(ODDraft *somSelf, Environment *ev,
                ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreateLinkSource");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODLinkSource*  SOMLINK ODDraftAcquireLinkSource(ODDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquireLinkSource");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODLink*  SOMLINK ODDraftAcquireLink(ODDraft *somSelf, Environment *ev,
                ODStorageUnitID id,
                ODLinkSpec* linkSpec)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquireLink");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope void  SOMLINK ODDraftRemoveLink(ODDraft *somSelf, Environment *ev,
                ODLink* link)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRemoveLink");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODPersistentObjectID  SOMLINK ODDraftGetPersistentObjectID(ODDraft *somSelf, Environment *ev,
                ODPersistentObject* object,
                ODObjectType    objectType)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftGetPersistentObjectID");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULLID;
}

SOM_Scope ODPersistentObject*  SOMLINK ODDraftAcquirePersistentObject(ODDraft *somSelf, Environment *ev,
                ODPersistentObjectID objectID,
                ODObjectType*   objectType)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquirePersistentObject");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

/*
 *  private by convention
 */

SOM_Scope void  SOMLINK ODDraftRemoveLinkSource(ODDraft *somSelf, Environment *ev,
                ODLinkSource* link)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRemoveLinkSource");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftInitDraft(ODDraft *somSelf, Environment *ev,
                ODDocument* document,
                ODDraftID id,
                ODDraftPermissions perms)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftInitDraft");
}

SOM_Scope void  SOMLINK ODDraftReleaseFrame(ODDraft *somSelf, Environment *ev,
                ODFrame* frame)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftReleaseFrame");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftReleasePart(ODDraft *somSelf, Environment *ev,
                ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftReleasePart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftReleaseLink(ODDraft *somSelf, Environment *ev,
                ODLink* link)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftReleaseLink");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftReleaseLinkSource(ODDraft *somSelf, Environment *ev,
                ODLinkSource* linkSource)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftReleaseLinkSource");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODDraft*  SOMLINK ODDraftReleaseStorageUnit(ODDraft *somSelf, Environment *ev,
                ODStorageUnitID id)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftReleaseStorageUnit");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODLinkSourceIterator*  SOMLINK ODDraftCreateLinkSourceIterator(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreateLinkSourceIterator");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope ODLinkIterator*  SOMLINK ODDraftCreateLinkIterator(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreateLinkIterator");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope void  SOMLINK ODDraftsomUninit(ODDraft *somSelf)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftsomUninit");

    ODDraft_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope void  SOMLINK ODDraftAcquire(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftAcquire");

    ODDraft_parent_ODRefCntObject_Acquire(somSelf,ev);
}

SOM_Scope void  SOMLINK ODDraftRelease(ODDraft *somSelf, Environment *ev)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftRelease");

    ODDraft_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope ODPart*  SOMLINK ODDraftConstructRealPart(ODDraft *somSelf, Environment *ev,
                ODStorageUnit* su, ODBoolean isInitPartFromStorage,
                ODPartWrapper* partWrapper,
                ODType partType, ODEditor optionalEditor)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftConstructRealPart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

        return kODNULL;
}

SOM_Scope void  SOMLINK ODDraftReleaseRealPart(ODDraft *somSelf, Environment *ev,
        ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftReleaseRealPart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftDeleteRealPart(ODDraft *somSelf, Environment *ev,
        ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","DeleteRealPart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDraftSwapPart(ODDraft *somSelf, Environment *ev,
        ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","SwapPart");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}


#ifdef _PLATFORM_OS2_
SOM_Scope ODLinkSourceIterator*  SOMLINK ODDraftCreatePartLinkSourceIterator(ODDraft *somSelf, Environment *ev,
                ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreatePartLinkSourceIterator");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

}

SOM_Scope ODLinkIterator*  SOMLINK ODDraftCreatePartLinkIterator(ODDraft *somSelf, Environment *ev,
                ODPart* part)
{
    /* ODDraftData *somThis = ODDraftGetData(somSelf); */
    ODDraftMethodDebug("ODDraft","ODDraftCreatePartLinkIterator");

        WARNMSG(WARN_INDEX(AMSG_700),"A subclass should have overridden this method!");
        ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

}
#endif

