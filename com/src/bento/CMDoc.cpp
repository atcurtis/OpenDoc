/* @(#)Z 1.19 com/src/bento/CMDoc.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:08 (96/10/29 09:11:02) */
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
	File:		CMDoc.cpp

	Contains:	Implementation for CMDoc class.

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<54>	10/24/95	jpa		1293441: DM/VL: Bento memory reserve &
									fatal container err & don't throw in
									Release.
		<53>	10/20/95	VL		1293256: Changed
									kODErrBentoInvalidVersionList to
									kODErrDraftDoesNotExist.
		<52>	10/13/95	EL		1287340: Use standard ISO prefix
		<51>	 10/8/95	TJ		Fixes Recomended by Refball
		<50>	 10/3/95	TJ		Added Includes so it compiles
		<49>	 10/3/95	TJ		Changes done by RefBall Team
		<48>	 9/29/95	TJ		Made Changes for MAC SOM
		<47>	  9/8/95	VL		1282012: Replaced kODErrDraftNotExists with
									kODErrDraftDoesNotExist.
		<46>	  9/8/95	T‚		1281096 FB2:Many constants in ODTypesB
									without kOD prefix!
		<45>	  9/7/95	EL		1281410: Window for draft just above a
									deleted draft will not be automatically
									closed when that draft is deleted.
		<44>	 8/30/95	EL		1270290: Do not close and open the draft in
									SaveToPrev and Collapse draft if draft is
									not top draft and hence read only.
		<43>	 8/21/95	VL		1278330, 1278315: Error code cleanup.
		<42>	 8/18/95	NP		1274946: add kODErrInvalidPersistentFormat
		<41>	 8/16/95	NP		1274946: ErrorDef.idl problems. Add include
									file.
		<40>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		<39>	  8/8/95	EL		#1273589: version list is not collapsed
									when document is closed.
		<38>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<37>	 7/21/95	VL		1270320: Removed internal field _fName.
		<36>	 6/28/95	RR		1242642 BB Mostly ref counting. RemoveDraft
									releases draft before deleting it
		<35>	 6/20/95	VL		1259397: Release Draft after getting its id
									in GetDraftGut.
		<34>	 5/26/95	VL		1251403: Multithreading naming support.
		<33>	 5/25/95	jpa		Fixed usage of ODDebug. [1253321]
		<32>	 5/18/95	CC		1238898: Add destFrame argument to
									BeginClone call.
		<31>	 5/18/95	EL		1249941: Set container dirty flag when
									drafts changed.
		<30>	 4/25/95	VL		1210982: Removed 5$.
		<29>	  4/7/95	EL		1225905: Call SetModDate when save to prev.
		<28>	 3/31/95	EL		1234685: If top draft embedded container
									has not been saved don't write it in the
									version list. Don't externalize version
									list except before flush or close.
		<27>	 3/29/95	DM		make standard format little endian
		<26>	 3/24/95	EL		1209355: Cut down on version list
									externalization. Flush file container in
									SaveToAPrevDraft.
		<25>	 3/23/95	VL		1230357: Implemented Purge. 1228003: Added
									debug code for versionlist.
		<24>	  3/9/95	VL		1220320: Moved public error codes from
									Bento to ErrorDef.idl.
		<23>	 2/17/95	EL		1182275: Collapse version list before
									closing embedded container so we know if
									there is a new draft.
		<22>	 2/13/95	T‚		1219963: Document:AcquireDraft should not bump
									refcount if kPosSame and release==true
		<21>	 1/31/95	EL		1195846: revert file if there is no real
									change.
		<20>	 1/26/95	eeh		1214080: OpenDoc really uses kODMacIText,
									not kODIntlText.
		<19>	 1/25/95	jpa		Include StdExts.xh
		<18>	 1/11/95	VL		1185688: Made storage more robust in terms
									of error handling. Also did some code
									review cleanup.
		<17>	11/14/94	VL		1155887: Changed AcquireDraft to handle new
									posCode for top draft.
		<16>	10/19/94	VL		1155857: Added ODDebug_Drafts for debugging
									"Cannot create more than 10 drafts"
									problem.
		<15>	 9/29/94	RA		1189812: Mods for 68K build.
		<14>	 9/23/94	VL		1184272: ContainerID is now a sequence of
									octets. 1184166: DocumentName is ODIText
									now.
		<13>	  9/6/94	VL		1184154: Include StorageU.xh.
		<12>	  9/6/94	VL		1184154: Removed include file also.
		<11>	  9/6/94	VL		1184154: Removed AcquireDocumentProperties.
		<10>	  9/5/94	VL		1184871: Used Renew to remove dependency on
									default heap.
		 <9>	 8/31/94	T‚		#1183129, #1183116, #1183119, #1183111:
									Lots of ErrorCode cleanup.
		 <8>	 8/31/94	VL		No code change. Fixed comments in <7>.
		 <7>	 8/31/94	VL		1161158, 1106013: Commented out code which
									does checking on embedded container's use
									mode in AcquireDraft. This should avoid a
									crashing bug when the embedded container is
									kODNULL. Also, fixed GetName to make it
									return kODNULL if the document doesn't have a name.
		 <6>	 8/26/94	VL		1183174: Use updated cloning APIs.
		 <5>	 8/16/94	T‚		#1180922  Remove more obsolete types from
									StdTypes.idl.  Localized kODVersionList to
									this file.
		 <4>	  8/3/94	VL		1153123: Storage to ODStor.
		 <3>	 7/14/94	VL		Check return value for kODNULL before
									calling GetUseMode in AcquireDraftGut.
		 <2>	 7/11/94	VL		Added Exists.
		 <1>	  7/5/94	VL		first checked in

	To Do:
	In Progress:
		
*/

#define CMDocument_Class_Source
#define VARIABLE_MACROS
#include <CMDoc.xih>

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef _FLIPEND_
#include "FlipEnd.h"
#endif

#ifndef SOM_ODEmbeddedContainer_xh
#include <EmbedCtr.xh>
#endif

#ifndef SOM_CMDraft_xh
#include <CMDraft.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef __STRING__
#include <string.h>		// For strlen, strcpy....
#endif

#ifndef _INDHDR_
#include "IndHdr.h"			// For Embedded Property and Type names
#endif

#ifndef _CMCTR_
#include "CMCtr.xh"			// Just for getting fContainer
#endif

#ifndef _DOCPRIV_
#include "DocPriv.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifndef __TIME_H__
#include <time.h>
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
#ifndef _ODUTILS_
#include "ODUtils.h"
#endif
#endif

#ifndef _BENTOERR_
#include "BentoErr.h"
#endif

#pragma segment CMDocument

//==============================================================================
// Constants
//==============================================================================

// Private ISO Strings
const 	ODPropertyName	kODDocumentProperties	= "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:DocumentProperties";
const	ODValueType		kODVersionList 			= "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:CMVersionList";

// For debugging

#if ODDebug
// #define ODDebug_Drafts 1
// #define ODDebug_DebugRefCount	1
#endif

//==============================================================================
// Function Prototype
//==============================================================================

static CMDraft* NewCMDraft(ODMemoryHeapID heapID);
static CMObject AcquireDocumentPropertiesObject(CMContainer container);

// The following two functions need to be filled in when we are moving to a 
// real multithread system.

#define EnableInterrupt()
#define DisableInterrupt()


// For debugging

#ifdef DebugStorage

#define MyDebugStr(s) do {somPrintf(s);} while (0)
#define MyDebug1Str(f,p1) do {somPrintf(f, p1);} while (0)
#define MyDebug2Str(f,p1,p2) do {somPrintf(f, p1, p2);} while (0)
#define MyDebug3Str(f,p1,p2,p3) do {somPrintf(f, p1, p2,p3);} while (0)

#else

#define MyDebugStr(s)
#define MyDebug1Str(f,p1)
#define MyDebug2Str(f,p1,p2)
#define MyDebug3Str(f,p1,p2,p3)

#endif

#if ODDebug_Drafts

#ifndef _MEMDEBG_
#include <MemDebg.h>
#endif

static void PrintDrafts(Environment* ev, DraftList* drafts, char* string);
static void PrintHeapInfo();
#endif


//==============================================================================
// CMDocument
//==============================================================================

//------------------------------------------------------------------------------
// CMDocument: GetCMVersionList
//------------------------------------------------------------------------------

SOM_Scope CMValue  SOMLINK CMDocumentGetCMVersionList(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentGetCMVersionList");

	CMValue			versionList = kODNULL;
    try
    { 
	
	CMContainer cmContainer = _fContainer->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(cmContainer);
	
	CMProperty		versionListProp;
	CMObject		versionListObj;
	
	if ((versionListProp = CMRegisterProperty(cmContainer, kODPropVersionList)) == kODNULL)
		THROW(kODErrBentoInvalidProperty);
		
	versionListObj = CMGetNextObjectWithProperty(cmContainer, kODNULL, versionListProp);
	
	if (versionListObj != kODNULL)
		versionList = CMGetNextValue(versionListObj, versionListProp, kODNULL);

	ODSessionRestoreCMAllocReserve(cmContainer);
    }
    catch (ODException _exception)
    {
	 versionList = kODNULL;
         ODSetSOMException(ev, _exception);
    }

	return versionList;
}

//------------------------------------------------------------------------------
// CMDocument: AcquireDraftGut
//------------------------------------------------------------------------------

SOM_Scope CMDraft*  SOMLINK CMDocumentAcquireDraftGut(CMDocument *somSelf, Environment *ev,
		VersionList* versionList,
		ODDraftPermissions perms,
		ODDraftID id,
		CMDraft* draft,
		ODPositionCode posCode,
		ODBoolean release)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentAcquireDraftGut");

	CMDraft*	newDraft = kODNULL;	
        ODBoolean acquired = kODFalse;
        ODBoolean CreateNewDraft = kODFalse;

    try
    {

#if ODDebug_Drafts
	somPrintf("**** Entering AcquireDraftGut: id %d draft %x posCode %d release %d\n", id, draft, posCode, release);
	PrintDrafts(ev, _fDrafts, "**** Entering AcquireDraftGut");
	somSelf->GetVersionList(ev)->Print(">>> Entering AcquireDraftGut");
#endif

	CMDraft*				fromDraft = (CMDraft*) draft;
	ODDraftID				latestDraftID;
	ODDraftID				fromDraftID;
	ODDraftID				prevDraftID;
	ODDraftID				nextDraftID;

	if (id != 0) {
		if ((newDraft = _fDrafts->Get(id)) != kODNULL) {
			if ((perms == kODDPReadOnly) ||
				(newDraft->GetPermissions(ev) == kODDPExclusiveWrite)) {
				newDraft->Acquire(ev);
                                acquired = kODTrue;
				MyDebugStr("**** AcquireDraft: Acquire only.\n");
			}
			else
				THROW(kODErrInvalidPermissions);
		}
		else if ((newDraft = _fReleasedDrafts->Get(id)) != kODNULL) {
			if (perms == kODDPExclusiveWrite) {
				if (newDraft->GetPermissions(ev) == kODDPReadOnly) {
						latestDraftID = versionList->GetLatestDraftID();
						if (versionList->IsAbove(latestDraftID, id) != kODFalse)
							THROW(kODErrInvalidPermissions);
						MyDebugStr("**** AcquireDraft: From ReadOnly to ExclusiveWrite.\n");
				}
			}
			newDraft->Reinitialize(ev, perms);
			_fReleasedDrafts->Remove(id);
			_fDrafts->Add(id, newDraft);
			newDraft->Acquire(ev);
                        acquired = kODTrue;
			MyDebugStr("**** AcquireDraft: Back from the _fReleasedDrafts pile.\n");
		}
		else {				
			newDraft = NewCMDraft(somSelf->GetHeap(ev));
                        CreateNewDraft = kODTrue;
			newDraft->InitDraft(ev, somSelf, id, perms);
			
			_fDrafts->Add(id, newDraft);
	
			MyDebug3Str("**** id %d draft %x perms %d\n", id, newDraft, newDraft->GetPermissions(ev));
		}	
	}
	else {
		if ((posCode != kODPosTop) && (fromDraft == kODNULL)) {
			THROW(kODErrInvalidDraftID);
		}
		switch (posCode) {
			case kODPosTop:
				if (fromDraft != kODNULL)
					THROW(kODErrInvalidDraftID);
				nextDraftID = versionList->GetLatestDraftID();
				newDraft = somSelf->AcquireDraftGut(ev, versionList,
									perms,
									nextDraftID,
									kODNULL,
									kODPosUndefined,
									kODFalse);
			break;
			case kODPosSame:
				if (fromDraft->GetPermissions(ev) == perms) {
					if (release == kODFalse)
						fromDraft->Acquire(ev);
					newDraft = fromDraft;
                                        acquired = kODTrue;
				}
				else {
					if (fromDraft->GetRefCount(ev) != 1)
						THROW(kODErrRefCountNotEqualOne);
					if (release == kODFalse)
						THROW(kODErrCannotChangePermissions);
					fromDraftID = fromDraft->GetID(ev);
					fromDraft->Release(ev);
					newDraft = somSelf->AcquireDraftGut(ev, versionList,
												perms,
												fromDraftID,
												kODNULL,
												kODPosUndefined,
												kODFalse);
				}
			break;
			case kODPosFirstBelow:
			case kODPosLastBelow:
				fromDraftID = fromDraft->GetID(ev);
				prevDraftID = versionList->GetPreviousDraftID(fromDraftID);
				if (release != kODFalse)
					fromDraft->Release(ev);
				newDraft = somSelf->AcquireDraftGut(ev, versionList,
											perms,
											prevDraftID,
											kODNULL,
											kODPosUndefined,
											kODFalse);
			break;
			case kODPosFirstAbove:
			case kODPosLastAbove:
				fromDraftID = fromDraft->GetID(ev);
				nextDraftID = versionList->GetNextDraftID(fromDraftID);
				if (release != kODFalse)
					fromDraft->Release(ev);
				newDraft = somSelf->AcquireDraftGut(ev, versionList,
											perms,
											nextDraftID,
											kODNULL,
											kODPosUndefined,
											kODFalse);
				break;
			case kODPosUndefined:
			case kODPosAll:
			case kODPosFirstSib:
			case kODPosLastSib:
			case kODPosNextSib:
			case kODPosPrevSib:
			default:
				THROW(kODErrUnsupportedPosCode);
			break;
		}
	}
	
#if ODDebug_Drafts
	PrintDrafts(ev, _fDrafts, "**** Exiting AcquireDraftGut");
	somSelf->GetVersionList(ev)->Print(">>> Exiting AcquireDraftGut");
#endif

    }
    catch (ODException _exception)
    {
      ODError error = ErrorCode();
      SaveEv();
      try
      {
         if (acquired){
             newDraft->Release(ev);
	     newDraft = kODNULL;
         }
         if (CreateNewDraft)
             ODDeleteObject(newDraft);
      }
      catch(ODException _exception)
      {
          SetErrorCode(kODNoError);
      }
      RestoreEv();
      ODSetSOMException(ev, error);
      
    }
	return newDraft;
}

//------------------------------------------------------------------------------
// CMDocument: ~CMDocument
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentsomUninit(CMDocument *somSelf)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentsomUninit");

	
	Environment*	ev = somGetGlobalEnvironment();
	DraftListIterator*	iter = kODNULL;
	DraftListIterator*	releasedDraftsIter = kODNULL;
    try
    { 
	
#ifdef _PLATFORM_MACINTOSH_
                    iter  = new(somSelf->GetHeap(ev)) DraftListIterator(_fDrafts);
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
		iter = new DraftListIterator(_fDrafts);
#endif
	iter->Initialize();
	
#ifdef _PLATFORM_MACINTOSH_
                 releasedDraftsIter = new(somSelf->GetHeap(ev)) DraftListIterator(_fReleasedDrafts);
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	releasedDraftsIter = new DraftListIterator(_fReleasedDrafts);
#endif
	releasedDraftsIter->Initialize();
	
	CMDraft*			draft;

#if ODDebug_DebugRefCount
	if (somSelf->GetRefCount(ev) != 0)
		DebugStr("\pRefCount of Document is not 0 at uninit.");
	somPrintf("~CMDocument %x RefCount %d\n", somSelf, somSelf->GetRefCount(ev));
#endif

	draft = iter->Last();
	while (draft != kODNULL) {
		delete draft;
		draft = iter->Previous();
	}
		
	draft = releasedDraftsIter->Last();
	while (draft != kODNULL) {
		delete draft;
		draft = releasedDraftsIter->Previous();
	}
		
	delete iter;
	delete releasedDraftsIter;
	delete _fVersions;
	delete _fDrafts;
	delete _fReleasedDrafts;
	
    parent_somUninit(somSelf);
   }
   catch (ODException _exception)
   {
        ODDeleteObject(iter);
        ODDeleteObject(releasedDraftsIter);
        ODSetSOMException(ev, _exception);
   }

}

//------------------------------------------------------------------------------
// CMDocument: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK CMDocumentPurge(CMDocument *somSelf, Environment *ev,
		ODSize size)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentPurge");
  
 	ODSize purgedSize = 0;
	DraftListIterator* draftList = kODNULL;

    try
    { 	
 	
#ifdef _PLATFORM_MACINTOSH_
                  draftList  = new(somSelf->GetHeap(ev)) DraftListIterator(_fDrafts);
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
// 	The use of heap is not implemented in Windows platform
	         draftList = new DraftListIterator(_fDrafts);
#endif
	draftList->Initialize();
	CMDraft* draft = draftList->Last();
	while (draft != kODNULL) {
		purgedSize += draft->Purge(ev, size-purgedSize);
		draft = draftList->Previous();
	}
	delete draftList;

     }
     catch(ODException _exception)
     {
	   purgedSize = 0;
           ODDeleteObject(draftList);
           ODError error = ErrorCode();
           ODSetSOMException(ev, error);
     }
	return purgedSize;
}

//------------------------------------------------------------------------------
// CMDocument: Acquire
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentAcquire(CMDocument *somSelf, Environment *ev)
{
//  CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentAcquire");

    parent_Acquire(somSelf,ev);
}

//------------------------------------------------------------------------------
// CMDocument: Release
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentRelease(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentRelease");

    try
    {
	
#if TestFlushContainer
		if (_fContainer->GetDirtyFlag(ev) != kODFalse)
			somSelf->ExternalizeVersionList(ev, kODFalse);
#endif

		parent_Release(somSelf,ev);
		if (somSelf->GetRefCount(ev) == 0) {
			_fContainer->ReleaseDocument(ev, somSelf);
		}
		
    }
    catch(ODException _exception)
    {
		
		ODError err = ErrorCode();

		WARNMSG(WARN_INDEX(AMSG_650),"Error occurred in ODDocument::Release: %d %s", 
			err, ErrorMessage() ?ErrorMessage() :"");

		if (err == kODErrBentoErr)
			SetErrorCode(kODErrUndefined);
		else if (err != kODErrUndefined)
			SetErrorCode(kODNoError);
		
    }
}

//------------------------------------------------------------------------------
// CMDocument: AcquireContainer
//------------------------------------------------------------------------------

SOM_Scope ODContainer*  SOMLINK CMDocumentGetContainer(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentGetContainer");

	return (ODContainer*) _fContainer;
}

//------------------------------------------------------------------------------
// CMDocument: GetID
//------------------------------------------------------------------------------

SOM_Scope ODDocumentID  SOMLINK CMDocumentGetID(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentGetID");

	return _fID;
}

//------------------------------------------------------------------------------
// CMDocument: GetName
//------------------------------------------------------------------------------

SOM_Scope ODDocumentName SOMLINK CMDocumentGetName(CMDocument *somSelf, Environment *ev)
{	
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentGetName");
	
	ODDocumentName *name = kODNULL;
#if defined(_PLATFORM_WIN32) || defined(_PLATFORM_OS2) || defined(_PLATFORM_AIX)
	ODDocumentName dummy;
#endif

     try
     {
	
	CMContainer cmContainer = _fContainer->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(cmContainer);

	CMObject	cmObject = AcquireDocumentPropertiesObject(cmContainer);
	ASSERTMSG(cmObject, kODErrNoDocumentProperties, 
		"No Document Properties Object.", 0);
	
	CMProperty	cmProp = CMRegisterProperty(cmContainer, kODPropDocumentName);
	CMType		cmType = CMRegisterType(cmContainer, kODMacIText);
	CMValue		cmValue = CMUseValue(cmObject, cmProp, cmType);
	
	if (cmValue != kODNULL) {
		ODITextFormat stdFormat;
		CMReadValueData(cmValue, &stdFormat, 0, sizeof(ODITextFormat));
		ODITextFormat format = ConvertODULongFromStd(stdFormat);
		
		if( format != kODTraditionalMacText ) {
			WARNMSG(WARN_INDEX(AMSG_660),"Reading IText in unknown format %ld",format);
			THROW(kODErrInvalidPersistentFormat);
		}
		ODULong size = CMGetValueSize(cmValue) - sizeof(ODITextFormat);
		name = CreateIText(size);
		name->format = format;
		CMReadValueData(cmValue, name->text._buffer,  sizeof(ODITextFormat), name->text._length);
	} else {
		name = SetITextStringLength(kODNULL,0,kODFalse);
		somSelf->SetName(ev, name);
	}
	
	ODSessionRestoreCMAllocReserve(cmContainer);
		
     }
     catch (ODException _exception)
     {
#ifdef _PLATFORM_MACINTOSH_
		ODDocumentName dummy;
		return dummy; // will be ignored since error set
#endif
#if defined(_PLATFORM_WIN32) || defined(_PLATFORM_OS2) || defined(_PLATFORM_AIX)
		*name = dummy;
#endif
        ODSetSOMException(ev, _exception);
	
    }
	return *name;
}

//------------------------------------------------------------------------------
// CMDocument: SetName
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentSetName(CMDocument *somSelf, Environment *ev,
		ODDocumentName* name)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentSetName");

   try
   {
	
	CMContainer	cmContainer = _fContainer->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(cmContainer);

	CMObject	cmObject = AcquireDocumentPropertiesObject(cmContainer);
	ASSERTMSG(cmObject, kODErrNoDocumentProperties, 
		"No Document Properties Object.", 0);
	
	CMProperty	cmProp = CMRegisterProperty(cmContainer, kODPropDocumentName);
	CMType		cmType = CMRegisterType(cmContainer, kODMacIText);
	CMValue		cmValue = CMUseValue(cmObject, cmProp, cmType);
	if (cmValue == kODNULL) {
		cmValue = CMNewValue(cmObject, cmProp, cmType);
	}
	ODITextFormat stdFormat = ConvertODULongToStd(name->format);
	CMWriteValueData(cmValue, &stdFormat, 0, sizeof(ODITextFormat));
	CMWriteValueData(cmValue, name->text._buffer,  sizeof(ODITextFormat), name->text._length);

	ODSessionRestoreCMAllocReserve(cmContainer);
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// CMDocument: CollapseDrafts
//------------------------------------------------------------------------------

SOM_Scope ODDocument*  SOMLINK CMDocumentCollapseDrafts(CMDocument *somSelf, Environment *ev,
		ODDraft* fromDraft,
		ODDraft* toDraft)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentCollapseDrafts");

    try
    {

	CMDraft*		from = (CMDraft*) fromDraft;
	CMDraft*		to = (CMDraft*) toDraft; ODVolatile(to);
	VersionList*	versionList = kODNULL;
	ODDraftID		fromID;
	ODDraftID		toID;
	ODDraftID		baseID;
	ODBoolean 		revertIt = kODFalse;
	
	TempODDraft     tempDraft = kODNULL; // DMc - better way to release

	versionList = somSelf->TestAndGetVersionList(ev);
	ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);
	
	
	if (!from) {
//	LOG("Illegal NULL fromDraft parameter in ODDraft::CollapseDrafts method");
			THROW(kODErrIllegalNullDraftInput);
//	ODSetSOMException(ev, kODErrIllegalNullDraftInput);
//	return somSelf;
	}

	fromID = from->GetID(ev);
	baseID = versionList->GetBaseDraftID();
	
	if (to == kODNULL) {
		toID = versionList->GetPreviousDraftID(fromID);
		to = (CMDraft*) somSelf->AcquireDraft(ev, kODDPReadOnly, 
				toID, kODNULL, kODPosUndefined, kODFalse);
		// releaseToDraft = kODTrue;
		tempDraft = to; // ensure it's released
	}
	else {
		toID = to->GetID(ev);
	}
	
	if ((fromID == baseID) || (fromID == toID))
		return somSelf;
	if (versionList->IsAbove(fromID, toID) == kODFalse){
		 THROW(kODErrCannotCollapseDrafts);
//	LOG("Draft version list error in ODDraft::CollapseDrafts method");
//	ODSetSOMException(ev, kODErrCannotCollapseDrafts);
//	return somSelf;
	}
	
	// from cannot have a ref count more than 1 if it is the top draft
	// if from is not top draft, then we are not going to do anything 
	// to the draft and we need not worry about the ref count
	
	if (fromID == _fVersions->GetLatestDraftID())
	    if (from->GetRefCount(ev) > 1) // Check for outstanding draft
	    {
//		   LOG("Outstanding Draft error in ODDraft::CollapseDrafts");
		   THROW(kODErrOutstandingDraft);
 	    }
	
		
#ifdef _PLATFORM_MACINTOSH_
	DraftListIterator* draftList = 
			new(somSelf->GetHeap(ev)) DraftListIterator(_fDrafts);
#endif
#if defined(_PLATFORM_WIN32_)||defined(_PLATFORM_OS2_)||defined(_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	DraftListIterator* draftList = new DraftListIterator(_fDrafts);
#endif
	draftList->Initialize();
	CMDraft* draft = draftList->Last();
	while (draft != kODNULL) {
		ODDraftID draftID = draft->GetID(ev);
		if ((versionList->IsBelow(draftID, fromID) != kODFalse) && 
			(versionList->IsAbove(draftID, toID) != kODFalse) &&
			(versionList->GetCurrentVersion(draftID) != kODTombstonedVersion))
			THROW(kODErrOutstandingDraft);
//		   LOG("Outstanding Draft error in ODDraft::CollapseDrafts");
		else
			draft = draftList->Previous();
	}
	delete draftList;
	
	// Flush the from draft
		
	if ((from->GetPermissions(ev) == kODDPExclusiveWrite) &&
		(from->NeedExternalizing(ev) != kODFalse)) {
		 THROW(kODErrNonEmptyDraft);
//      	LOG("Nonempty Draft in ODDraft::CollapseDrafts method");
//		ODSetSOMException(ev, kODErrIllegalNullDraftInput);
//		return somSelf;
	}
	else if (from->IsChangedFromPrev(ev, versionList) != kODFalse){
		 THROW(kODErrNonEmptyDraft);
//             	LOG("Nonempty Draft in ODDraft::CollapseDrafts method");
//		ODSetSOMException(ev, kODErrIllegalNullDraftInput);
//		return somSelf;
	}
		
	if (from->IsNewDraft(ev) != kODFalse) {
		revertIt = kODTrue;
//		_fDrafts->Remove(from->GetID(ev));
		ODDraftID id = from->GetID(ev);
		from->Release(ev);
		_fReleasedDrafts->Remove(id);
		from->Abort(ev);
		delete from;
	}
	else {
	// so that the collapsed version list will be written out on close
		_fContainer->SetDirtyFlag(ev, kODTrue);

		// Release the draft
		
		from->Release(ev);
	}

	// CollapseDrafts on VersionList
		
	versionList->CollapseDrafts(fromID, toID);
		
#if !TestFlushContainer		
	// Make the change persistent

	somSelf->ExternalizeVersionList(ev, kODFalse);
#endif
		
	// If no change, then clear the dirty flag
		
	if (revertIt != kODFalse)
		_fContainer->SetDirtyFlag(ev, kODFalse);

	
	somSelf->ReleaseVersionList(ev);
	
	//if (releaseToDraft != kODFalse)
	//	to->Release(ev);
#ifdef _NO_TMP_OBJS_
	if (tempDraft != kODNULL)
		tempDraft->Release(ev);
#endif
	

   }
   catch (ODException _exception)
   {
      ODError error = ErrorCode();
      SaveEv();
      try
      {
	somSelf->ReleaseVersionList(ev);
      }
      catch (ODException _exception)
      {
          SetErrorCode(kODNoError);
      }
      RestoreEv();
      ODSetSOMException(ev, error);
   }
	return somSelf;
}

//------------------------------------------------------------------------------
// CMDocument: AcquireDraft
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMDocumentAcquireDraft(CMDocument *somSelf, Environment *ev,
		ODDraftPermissions perms,
		ODDraftID id,
		ODDraft* draft,
		ODPositionCode posCode,
		ODBoolean release)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentAcquireDraft");

	ODDraft*	newDraft;

     try
     {

#if ODDebug_Drafts
	somPrintf("&&& AcquireDraft: id %d draft %x posCode %d release %d\n", id, draft, posCode, release);
#endif

	VersionList*			versionList;

	versionList = somSelf->TestAndGetVersionList(ev);
	ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);
		
	
		newDraft = somSelf->AcquireDraftGut(ev, versionList, perms, id, (CMDraft*) draft, posCode, release);
		
		
	
	somSelf->ReleaseVersionList(ev);			

#if ODDebug_Drafts
	somPrintf("&&& Exiting AcquireDraft\n");
#endif
		
    }
    catch (ODException _exception)
    {
           newDraft = kODNULL;
      ODError error = ErrorCode();
      SaveEv();
      try
      {
	   somSelf->ReleaseVersionList(ev);
      }
      catch (ODException _exception)
      {
          SetErrorCode(kODNoError);
      }
      RestoreEv();
      ODSetSOMException(ev, error);

      
    }
	return newDraft;
}

//------------------------------------------------------------------------------
// CMDocument: Exists
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK CMDocumentExists(CMDocument *somSelf, Environment *ev,
		ODDraftID id,
		ODDraft* fromDraft,
		ODPositionCode posCode)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentExists");

	ODBoolean	exists = kODFalse;

    try
    {

	
	if (id != 0) {
		exists = _fVersions->Exists(id);
	}
	else if (fromDraft != kODNULL) {
	
		VersionList* versionList = somSelf->TestAndGetVersionList(ev);
		ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);
		
		try
                {
			ODDraftID	fromDraftID = fromDraft->GetID(ev);
	
			switch (posCode) {
				case kODPosSame:
					exists = kODTrue;
				break;
				case kODPosFirstBelow:
				case kODPosLastBelow:
					exists = versionList->PreviousDraftExists(fromDraftID);
				break;
				case kODPosFirstAbove:
				case kODPosLastAbove:
					exists = versionList->NextDraftExists(fromDraftID);
				break;
				case kODPosUndefined:
				case kODPosAll:
				case kODPosFirstSib:
				case kODPosLastSib:
				case kODPosNextSib:
				case kODPosPrevSib:
				default:
					THROW(kODErrUnsupportedPosCode);
				break;
			}
                }
		catch(ODException _exception)
                {
                   ODError  error = ErrorCode();
                   SaveEv();
                   try
                   {
			somSelf->ReleaseVersionList(ev);
                   }
                    catch(ODException _exception)
                   {
                           SetErrorCode(kODNoError);
                   }
                   RestoreEv();
                   ODSetSOMException(ev, error);
		   throw;
	        }		
	}
	else
		THROW(kODErrInsufficientInfoInParams);
	
    }
    catch (ODException _exception)
    {
	exists = kODFalse;
        ODSetSOMException(ev, _exception);
    }
	return exists;
}

//------------------------------------------------------------------------------
// CMDocument: AcquireBaseDraft
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMDocumentAcquireBaseDraft(CMDocument *somSelf, Environment *ev,
		ODDraftPermissions perms)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentAcquireBaseDraft");

	ODDraft*	baseDraft;
   try
   {

	CMContainer	cmContainer = _fContainer->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(cmContainer);

	ODDraftID	baseDraftID;
	CMProperty	versionListProp;
	CMObject	versionListObj;
	CMType		versionListType;
	CMValue		versionList;
	ODSByte	bogusData[1];
	
	if (somSelf->GetCMVersionList(ev) == kODNULL) {
		if ((versionListProp = CMRegisterProperty(cmContainer, kODPropVersionList)) == kODNULL)
			THROW(kODErrBentoInvalidProperty);
		if ((versionListType = CMRegisterType(cmContainer, kODVersionList)) == kODNULL)
			THROW(kODErrBentoInvalidType);
		if ((versionListObj = CMNewObject(cmContainer)) == kODNULL)
			THROW(kODErrBentoCannotNewObject);
		versionList = CMNewValue(versionListObj, versionListProp, versionListType);

		if (versionList != kODNULL)
			CMWriteValueData(versionList, bogusData, 0, 0);
		
#ifdef _PLATFORM_MACINTOSH_
		_fVersions = new(somSelf->GetHeap(ev)) VersionList();
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		_fVersions = new VersionList();
#endif
		_fVersions->Initialize();
		
		baseDraft = somSelf->CreateDraft(ev, kODNULL, kODFalse);
	}
	else {
		baseDraftID = _fVersions->GetBaseDraftID();
		baseDraft = somSelf->AcquireDraft(ev, perms, baseDraftID, kODNULL, kODPosUndefined, kODFalse);
	}
	ODSessionRestoreCMAllocReserve(cmContainer);

    }
    catch (ODException _exception)
    {
	  baseDraft = kODNULL;
          ODSetSOMException(ev, _exception);
    }
	return baseDraft;
}

//------------------------------------------------------------------------------
// CMDocument: CreateDraft
//------------------------------------------------------------------------------

SOM_Scope ODDraft*  SOMLINK CMDocumentCreateDraft(CMDocument *somSelf, Environment *ev,
		ODDraft* below,
		ODBoolean releaseBelow)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentCreateDraft");

	CMDraft*	newDraft;

   try
   {

#if ODDebug_Drafts
	somPrintf("### Entering CreateDraft: below %x releaseBelow %d\n", below, releaseBelow);
	PrintDrafts(ev, _fDrafts, "### Entering CreateDraft");
	somSelf->GetVersionList(ev)->Print(">>> Entering CreateDraft");
#endif
	
	ODDraftID					prevDraftID;
	VersionList*				versionList;
	
	versionList = somSelf->TestAndGetVersionList(ev);
	ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);
		
	try
        {

		prevDraftID = versionList->GetLatestDraftID();	
		if ((prevDraftID != 0) &&
			(below != kODNULL) &&
			(prevDraftID != below->GetID(ev)))
			THROW(kODErrInvalidBelowDraft);
	
		if (releaseBelow != kODFalse) {
			if (below != kODNULL) {
				below->Release(ev);
			}
		}
		else {
			if ((below != kODNULL) && (below->GetPermissions(ev) == kODDPExclusiveWrite)) {
				THROW(kODErrInvalidPermissions);
			}
		}

		newDraft = NewCMDraft(somSelf->GetHeap(ev));
		newDraft->InitDraft(ev, somSelf, kODNULL, kODDPExclusiveWrite);
	
		_fDrafts->Add(newDraft->GetID(ev), newDraft);

        }
	catch (ODException _exception)
        {
	
             ODError  error = ErrorCode();
            SaveEv();
            try
            {
		somSelf->ReleaseVersionList(ev);
            }
            catch (ODException _exception)
            {
                 SetErrorCode(kODNoError);
            }
             RestoreEv();
             ODSetSOMException(ev, error);
             throw;
		
	}
	
	somSelf->ReleaseVersionList(ev);			
	
#if ODDebug_Drafts
	PrintDrafts(ev, _fDrafts, "### Exiting CreateDraft");
	somSelf->GetVersionList(ev)->Print(">>> Exiting CreateDraft");
#endif
		
    }
    catch (ODException _exception)
    {
         newDraft = kODNULL;
         ODSetSOMException(ev, _exception);
    }
	return newDraft;
}

//------------------------------------------------------------------------------
// CMDocument: SaveToAPrevDraft
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentSaveToAPrevDraft(CMDocument *somSelf, Environment *ev,
		ODDraft* fromDraft,
		ODDraft* toDraft)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentSaveToAPrevDraft");

    try
    {

	CMDraft*		from = (CMDraft*) fromDraft;
	ODDraftID		fromID; 
	ODDraftID		toID;
	VersionList*	versionList;
	ODBoolean		notTopDraft = kODFalse;
	ODTime			now;
	DraftListIterator*	draftList = kODNULL;

	versionList = somSelf->TestAndGetVersionList(ev);
	ASSERT((versionList != kODNULL), kODErrDraftDoesNotExist);
		
	try
        {

		if (!from)
			THROW(kODErrIllegalNullDraftInput);
		fromID = from->GetID(ev);

		if (toDraft == kODNULL) {
			toID = _fVersions->GetPreviousDraftID(fromID);
		}
		else {
			toID = toDraft->GetID(ev);
		}
			
		if (fromID == toID)
			return;
			
		if ((_fVersions->Exists(fromID) == kODFalse) ||
			(_fVersions->Exists(toID) == kODFalse) ||
			(versionList->IsAbove(fromID, toID) == kODFalse))
			THROW(kODErrDraftDoesNotExist);
		
		// Check to see whether we have any outstanding ODDraft associated
		//	with this document.
		
#ifdef _PLATFORM_MACINTOSH_
                         draftList = new(somSelf->GetHeap(ev)) DraftListIterator(_fDrafts);
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
			draftList = new DraftListIterator(_fDrafts);
#endif
		draftList->Initialize();
		
		CMDraft* draft = draftList->Last();
		while (draft != kODNULL) {
			ODDraftID	id = draft->GetID(ev);
			if ((versionList->IsBelow(id, fromID) != kODFalse) && 
				(versionList->IsAbove(id, toID) != kODFalse) &&
				(versionList->GetCurrentVersion(id) != kODTombstonedVersion))
				THROW(kODErrOutstandingDraft);
			else
				draft = draftList->Previous();
		}
		delete draftList;
	
		// SaveToAPrevDraft on VersionList
		
		_fVersions->SaveToAPrevDraft(fromID, toID);
		
		if (fromID != _fVersions->GetLatestDraftID())
			notTopDraft = kODTrue;

#if !TestFlushContainer
		// Make the version list change persistent
		
		somSelf->ExternalizeVersionList(ev, kODFalse);
#endif
		
		// Write out if it is an exclusive write container
		if (notTopDraft == kODFalse)	
			from->Close(ev);

		CMContainer	cmContainer = _fContainer->GetCMContainer(ev);
		
#if TestFlushContainer

		// Make the version list change persistent
		
		somSelf->ExternalizeVersionList(ev, notTopDraft);
		
		ODSessionMustHaveCMAllocReserve(cmContainer);
		CMTakeSnapShot(cmContainer, kODFalse);
#endif
		time(&now);
		ASSERTMSG(sizeof(ODTime)==sizeof(time_t), 
			kODErrAssertionFailed, 
			"ODTime not same as time_t.", 0);

		_fContainer->SetModDate(ev, now); 

		if (notTopDraft == kODFalse) { // if we have close it before we open it again
			from->Open(ev);
			
			from->SetChangedFromPrevFlag(ev, kODFalse);
		}		
		
		// Release to draft if necessary
		
		if (toDraft != kODNULL) {
			((CMDraft*) toDraft)->Close(ev);
			((CMDraft*) toDraft)->Open(ev);
		}
		ODSessionMustHaveCMAllocReserve(cmContainer);
		
        }
	catch (ODException _exception)
        {
            ODError error = ErrorCode();
            SaveEv();
            try
            {
                ODDeleteObject(draftList);	
		somSelf->ReleaseVersionList(ev);
            }
            catch (ODException _exception)
            {
                  SetErrorCode(kODNoError);
            }
            RestoreEv();
	    ODSetSOMException(ev, error);
		throw;
		
	}
	
	somSelf->ReleaseVersionList(ev);			

    }
    catch (ODException _exception)
    {
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDocument: SetBaseDraftFromForeignDraft
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentSetBaseDraftFromForeignDraft(CMDocument *somSelf, Environment *ev,
		ODDraft* draft)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentSetBaseDraftFromForeignDraft");


     try
     {

#ifdef _NO_TMP_OBJS_
	ODDraft *baseDraft = somSelf->AcquireBaseDraft(ev, kODDPExclusiveWrite);
#else
	TempODDraft baseDraft = somSelf->AcquireBaseDraft(ev, kODDPExclusiveWrite);
#endif
	
	{
#ifdef _NO_TMP_OBJS_
		ODStorageUnit *fromDraftProperties = draft->AcquireDraftProperties(ev);
		ODStorageUnit *toDraftProperties = baseDraft->AcquireDraftProperties(ev);
#else
		TempODStorageUnit fromDraftProperties = draft->AcquireDraftProperties(ev);
		TempODStorageUnit toDraftProperties = baseDraft->AcquireDraftProperties(ev);
#endif
		
		ODDraftKey key = draft->BeginClone(ev, baseDraft, kODNULL, kODCloneAll); 
		try
                {
			draft->Clone(ev, key, fromDraftProperties->GetID(ev), toDraftProperties->GetID(ev), kODNULL);
			draft->EndClone(ev, key );
                }
		catch (ODException _exception)
                {
                     ODError error = ErrorCode();
                     SaveEv();
                     try
                     {
			draft->AbortClone(ev, key);
                     }
                     catch (ODException _exception)
                     {
                         SetErrorCode(kODNoError);
                     }
                     RestoreEv();
                     ODSetSOMException(ev, error);
		}
#ifdef _NO_TMP_OBJS_
		fromDraftProperties->Release(ev);
		toDraftProperties->Release(ev);
#endif
		
	}
	baseDraft->Externalize(ev);
#ifdef _NO_TMP_OBJS_
	baseDraft->Release(ev);
#endif

     }
     catch (ODException _exception)
     {
           ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMDocument: InitDocument
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentInitDocument(CMDocument *somSelf, Environment *ev,
		ODContainer* container,
		ODDocumentID id)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentInitDocument");
	
     try
     {
		
	/* Moved from somInit. SOM itself sets fields to zero
	_fContainer = (ODBentoContainer*) kODNULL;
	_fID = 0;
	_fDrafts = kODNULL;
	_fReleasedDrafts = kODNULL;
	
	_fVersions = kODNULL;
	_fVersionListSemaphore = 0;
	
	_fHeap = kDefaultHeapID;
	*/
	
	somSelf->InitRefCntObject(ev);
	
	_fContainer = (ODBentoContainer*) container;
	if (_fContainer == kODNULL)
		THROW(kODErrIllegalNullContainerInput);

	_fID = id;
		
#ifdef _PLATFORM_MACINTOSH_
	_fDrafts = new(somSelf->GetHeap(ev)) DraftList;
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	_fDrafts = new DraftList;
#endif
	_fDrafts->Initialize();
	
#ifdef _PLATFORM_MACINTOSH_
	_fReleasedDrafts = new(somSelf->GetHeap(ev)) DraftList;
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	_fReleasedDrafts = new DraftList;
#endif
	_fReleasedDrafts->Initialize();
	
	somSelf->InternalizeVersionList(ev);

	if (_fVersions == kODNULL) {
	
		// If we get here, that means we have a new document.
		// A new document makes a container dirty by definition. Therefore,
		// we have to set the dirty flag.
	
		_fContainer->SetDirtyFlag(ev, kODTrue);
		
		// We are not creating a VersionList yet because the user may
		// just close the document without creating any version.
	
	}
	
	_fHeap = _fContainer->GetHeap(ev);
	
     }
     catch (ODException _exception)
     {
          ODSetSOMException(ev, _exception);
     }
}

//------------------------------------------------------------------------------
// CMDocument: ReleaseDraft
//------------------------------------------------------------------------------

SOM_Scope ODDocument*  SOMLINK CMDocumentReleaseDraft(CMDocument *somSelf, Environment *ev,
		ODDraft* draftToRelease)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentReleaseDraft");

#if ODDebug_Drafts
	somPrintf("@@@ ReleaseDraft: draft %x id %d\n", draftToRelease, draftToRelease->GetID(ev));
	PrintHeapInfo();
#endif

	CMDraft*		draft = (CMDraft*) draftToRelease;
	ODDraftID		draftID = draft->GetID(ev);
	
     try
     {
	if ((draft = _fDrafts->Get(draftID)) != kODNULL) {
		_fDrafts->Remove(draftID);
		_fReleasedDrafts->Add(draftID, draft);
		if (draft->GetPermissions(ev) == kODDPExclusiveWrite) {
			if (draft->NeedExternalizing(ev) != kODFalse) {
				if (draft->ChangedFromPrev(ev) != kODFalse)
					_fContainer->SetDirtyFlag(ev, kODTrue);
				draft->Close(ev);
#if !TestFlushContainer
				somSelf->ExternalizeVersionList(ev, kODFalse);
#endif
			}
			else if (draft->IsNewDraft(ev) != kODFalse) {
				if (draft->GetID(ev) == _fVersions->GetBaseDraftID()) {
					if (draft->ChangedFromPrev(ev) != kODFalse)
						draft->RemoveChanges(ev);
					draft->Close(ev);
#if !TestFlushContainer
					somSelf->ExternalizeVersionList(ev, kODFalse);
#endif
				}
				else
					draft->Abort(ev);
			}
			else
				draft->Abort(ev);
		}
		else
			draft->Close(ev);
	}
	else
		THROW(kODErrDraftDoesNotExist);

#if ODDebug_Drafts
	somPrintf("@@@ ReleaseDraft: Done.\n");
	PrintHeapInfo();
#endif
     }
     catch (ODException _exception)
     {
          ODSetSOMException(ev, _exception);
     }
		
	return somSelf;
}

//------------------------------------------------------------------------------
// CMDocument: InternalizeVersionList
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentInternalizeVersionList(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentInternalizeVersionList");
	ODPtr			tmpBuffer;


    try
    {
	ODULong		versionListSize;
	CMValue			versionList = somSelf->GetCMVersionList(ev);
	
	if (versionList == kODNULL)
		return;

	CMContainer	cmContainer = _fContainer->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(cmContainer);

	versionListSize = CMGetValueSize(versionList);
	
	tmpBuffer = ODNewPtr(versionListSize, somSelf->GetHeap(ev));
	
	CMReadValueData(versionList, tmpBuffer, 0, versionListSize);
	
	if (_fVersions == kODNULL) {
#ifdef _PLATFORM_MACINTOSH_
		_fVersions = new(somSelf->GetHeap(ev)) VersionList;
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//		The use of heap is not implemented in Windows platform
		_fVersions = new VersionList;
#endif
		_fVersions->Initialize(tmpBuffer, versionListSize);
	}
	else {
		_fVersions->Reinitialize(tmpBuffer, versionListSize);
	}
	
	ODDisposePtr(tmpBuffer);

	ODSessionRestoreCMAllocReserve(cmContainer);
	
#if ODDebug_Drafts
	_fVersions->Print("Internalized VersionList");
#endif

    }
    catch (ODException _exception)
    {
      if(tmpBuffer)
	ODDisposePtr(tmpBuffer);
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDocument: ExternalizeVersionList
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentExternalizeVersionList(CMDocument *somSelf, Environment *ev, ODBoolean ignoreTopDraft)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","CMDocumentExternalizeVersionList");

	ODPtr			tmpBuffer = kODNULL;

    try
    {

	ODULong		tmpBufferSize;
	CMValue			versionList = somSelf->GetCMVersionList(ev);

	if (versionList == kODNULL)
		THROW(kODErrNoVersionList);

	if (_fVersions != kODNULL) {
		CMContainer	cmContainer = _fContainer->GetCMContainer(ev);
		ODSessionMustHaveCMAllocReserve(cmContainer);

#if ODDebug_Drafts
	_fVersions->Print("Externalizing VersionList");
#endif
		CMSize oldSize = CMGetValueSize(versionList);

		_fVersions->ExportTo(&tmpBuffer, &tmpBufferSize, ignoreTopDraft);
		
		CMWriteValueData(versionList, tmpBuffer, 0, tmpBufferSize);
		
		if (oldSize > tmpBufferSize)
			CMDeleteValueData(versionList, tmpBufferSize, oldSize - tmpBufferSize);
		
		ODDisposePtr(tmpBuffer);
		ODSessionRestoreCMAllocReserve(cmContainer);
		
	}

    }
    catch (ODException _exception)
    {
      if(tmpBuffer)
	 ODDisposePtr(tmpBuffer);
         ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDocument: Reopen
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentReopen(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","Reopen");
	DraftListIterator*		iter = kODNULL; 

   try
   {

#ifdef _PLATFORM_MACINTOSH_
         iter = new(somSelf->GetHeap(ev)) DraftListIterator(_fDrafts);
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
	iter = new DraftListIterator(_fDrafts);
#endif
	CMDraft*				draft;

	iter->Initialize();
	draft = iter->Last();
	while (draft != kODNULL) {
		draft->Open(ev);
		draft = iter->Previous();
	}
	delete iter;

   }
   catch (ODException _exception)
   {
        delete iter;
        ODSetSOMException(ev, _exception);
   }
}

//------------------------------------------------------------------------------
// CMDocument: GetVersionList
//------------------------------------------------------------------------------

SOM_Scope VersionList*  SOMLINK CMDocumentGetVersionList(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","GetVersionList");

	return _fVersions;
}

//------------------------------------------------------------------------------
// CMDocument: TestAndGetVersionList
//------------------------------------------------------------------------------

SOM_Scope VersionList*  SOMLINK CMDocumentTestAndGetVersionList(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","TestAndGetVersionList");


	VersionList *retVersion = kODNULL;
   try
   {

	DisableInterrupt();

	if (_fVersionListSemaphore > 0) {
//		if same thread,
			_fVersionListSemaphore++;
// 		else {
//			EnableInterrupt();
//			THROW(kODErrVersionListUnavailable);
//
	}
	else {
//		store the thread
		_fVersionListSemaphore++;
	}
			
	EnableInterrupt();

	retVersion = _fVersions;
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }

	return retVersion;
}

//------------------------------------------------------------------------------
// CMDocument: ReleaseVersionList
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK CMDocumentReleaseVersionList(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","ReleaseVersionList");


   try
   {
	DisableInterrupt();
	
	if (_fVersionListSemaphore > 0) {
//		if (same thread) {
			--_fVersionListSemaphore;
			EnableInterrupt();	
//		}
//		else {
//			EnableInterrupt();
//			THROW(kODErrVersionListUnavailable);
//		}
	}
	else {
		EnableInterrupt();	
		THROW(kODErrSemaphoreReleased);
	}

    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CMDocument: GetHeap
//------------------------------------------------------------------------------

SOM_Scope ODMemoryHeapID  SOMLINK CMDocumentGetHeap(CMDocument *somSelf, Environment *ev)
{
    CMDocumentData *somThis = CMDocumentGetData(somSelf);
    CMDocumentMethodDebug("CMDocument","GetHeap");

	return _fHeap;
}

//------------------------------------------------------------------------------
// AcquireDocumentPropertiesObject
//------------------------------------------------------------------------------

static CMObject AcquireDocumentPropertiesObject(CMContainer container)
{
	CMObject		documentPropertiesObject  = kODNULL;
	Environment* ev = somGetGlobalEnvironment();
  try
  {
	CMProperty		docPropertiesProp;
	CMType			docPropertiesType;
	CMValue			value;

	// CMContainer	cmContainer = _fContainer->GetCMContainer(ev);
	// ODSessionMustHaveCMAllocReserve(cmContainer);
	// This is a static function which is only called by functions that
	// have already called ODSessionMustHaveCMAllocReserve().

	if ((docPropertiesProp = CMRegisterProperty(container, kODDocumentProperties)) == kODNULL)
		THROW(kODErrBentoInvalidProperty);
	documentPropertiesObject = CMGetNextObjectWithProperty(container, kODNULL, docPropertiesProp);
	
	if (documentPropertiesObject == kODNULL) {

		CMContainerModeFlags	openMode;
		
		CMGetContainerInfo(container, kODNULL, kODNULL, kODNULL, kODNULL, &openMode);
		if (openMode == kCMReading)
			return kODNULL;
		
		if ((docPropertiesType = CMRegisterType(container, kODValue)) == kODNULL)
			THROW(kODErrBentoInvalidProperty);

		if ((documentPropertiesObject = CMNewObject(container)) == kODNULL)
			THROW(kODErrBentoCannotNewObject);
			
		if ((value = CMNewValue(documentPropertiesObject, docPropertiesProp, docPropertiesType)) == kODNULL)
			THROW(kODErrBentoCannotNewValue);
			
		CMWriteValueData(value, "", 0, 0);
	}
	// ODSessionRestoreCMAllocReserve(cmContainer);
    }
    catch (ODException _exception)
    {
        ODSetSOMException(ev, _exception);
    }
	
	return documentPropertiesObject;
}

//------------------------------------------------------------------------------
// NewCMDraft
//------------------------------------------------------------------------------

static CMDraft* NewCMDraft(ODMemoryHeapID heapID)
{
         Environment* ev = somGetGlobalEnvironment();
	 CMDraft*	cmDraft = kODNULL;
   try
   {
#ifdef _PLATFORM_MACINTOSH_
	SOMClass*	cmDraftClass = somNewClassReference(CMDraft);
	ODULong		size = cmDraftClass->somGetInstanceSize();
	ODPtr		buffer = ODNewPtr(size, heapID);
	CMDraft*	cmDraft = (CMDraft*) cmDraftClass->somRenew(buffer);
	somReleaseClassReference ( cmDraftClass );
#endif
#if defined(_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_) || defined (_PLATFORM_AIX_)
//	The use of heap is not implemented in Windows platform
            cmDraft  = new CMDraft;
#endif
   }
   catch (ODException _exception)
   {
        ODDeleteObject(cmDraft);
        ODError  error = ErrorCode();
        ODSetSOMException(ev, error);
   }
	
	return cmDraft;
}

#if ODDebug_Drafts

//------------------------------------------------------------------------------
// PrintDrafts
//------------------------------------------------------------------------------

static void PrintDrafts(Environment* ev, DraftList* drafts, char* string)
{
/*
	DraftListIterator*	draftList = new DraftListIterator(drafts);
	draftList->Initialize();
	for (CMDraft* draft = draftList->Last(); draftList->IsNotComplete(); draft = draftList->Previous()) {
		somPrintf("%s: draft %x %d refCount %d\n", string, draft, draft->GetID(ev), draft->GetRefCount(ev));
	}
	delete draftList;
*/
}

//------------------------------------------------------------------------------
// PrintHeapInfo
//------------------------------------------------------------------------------

static void PrintHeapInfo()
{
/*
	MemHeap*	heapID = 0;
	const char *name;
	size_t allocated;
	size_t free;
	size_t nBlocks;
	size_t nObjects;
	MMGetHeapInfo(heapID,
					&name,
					&allocated,
					&free,
					&nBlocks,
					&nObjects );
	somPrintf("Heap: allocated %d, free %d, nBlocks %d, nObjects %d\n", allocated, free, nBlocks, nObjects);
*/
}


#endif


