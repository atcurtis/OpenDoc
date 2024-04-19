//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.12 com/src/utils/DocUtils.cpp, odutils, od96os2, odos29646d 7/15/96 17:57:45 [ 11/15/96 15:28:42 ]
/*
	File:		DocUtils.cpp

	Contains:	code for useful high level Document/Draft utilities

	Owned by:	Tantek ‚elik

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _DOCUTILS_
#include "DocUtils.h"
#endif

#ifndef SOM_ODDispatcher_xh
#include "Disptch.xh"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef SOM_ODFrame_xh
#include "Frame.xh"
#endif

#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _INFOUTIL_
#include "InfoUtil.h"
#endif
#endif

#if !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif
#endif // _PLATFORM_WIN32_

#ifndef _STORUTIL_
#include "StorUtil.h"		// for GetFileContainer, code could be inline instead. -T‚
#endif

#ifndef _REFCTCOL_
#include "RefCtCol.h"
#endif


#ifndef SOM_ODDispatcher_xh
#include <Disptch.xh>
#endif

#ifndef SOM_ODUndo_xh
#include <Undo.xh>
#endif

#ifndef SOM_ODInfo_xh
#include <Info.xh>
#endif

#ifndef SOM_ODClipboard_xh
#include <Clipbd.xh>
#endif

#ifndef SOM_ODLinkManager_xh
#include <LinkMgr.xh>
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODWindowIterator_xh
#include <WinIter.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
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

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef _TIME
#include <time.h>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif


#ifndef SOM_ODObjectNameSpace_xh
#include "ObjectNS.xh"
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODObjectIterator_xh
#include <ObjctItr.xh>
#endif


#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#if ( !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) ) \
      && !defined(_PLATFORM_AIX_)
#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __ICONS__
#include <Icons.h>
#endif

#endif // !_PLATFORM_WIN32_ && !_PLATFORM_OS2_ && !_PLATFORM_AIX_

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifdef _PLATFORM_WIN32_
#define watchCursor 4

#ifndef _WINUSER_
#include <WINUSER.H>
#endif 

#endif

//==============================================================================
// Constants
//==============================================================================

const ODISOStr		kODRefNumDocument			=	"RefNumDocument";
const ODISOStr		kODDocumentTempDraft		=	"DocumentTempDraft";

const  ODPropertyName kODPropUnsavedDocument	=	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Property:UnsavedDocument";
const  ODPropertyName kODPropTempDocument		=	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Property:TempDocument";


static ODUByte gDUHex[] = "0123456789ABCDEF";

/* Constants to specify the file fork GetLocalPaths should match */
enum
{
	kDataFork		= 0,
	kResourceFork	= 1
};

//==============================================================================
// Local functions
//==============================================================================

ODISOStr	ODCreateISOStrKeyFromULong(ODULong	uLongKey);

ODULong		ODGetODULongFromISOStrKey(ODISOStr isoStrKey);

void		SyncFileToProperties(Environment* ev, ODSession* session, ODDocument* document);
void		SyncPropertiesToFile(Environment* ev, ODSession* session, ODDocument* document);

ODFileRefNum	ODDeduceRefNum(Environment* ev, PlatformFile* file, 
					ODULong prevRefNumCount, ODSShort* prevRefNums);
					
//------------------------------------------------------------------------------
// Namespace key utilities
//------------------------------------------------------------------------------

ODISOStr	ODCreateISOStrKeyFromULong(ODULong	uLongKey)
{
	ODISOStr ret = (ODISOStr)ODNewPtrClear(sizeof(ODULong)*2 + 1);
	for (ODUByte i = 0; i<sizeof(ODULong); ++i)
	{
		ODUByte b = ((ODUByte*)(&uLongKey))[i];
		ret[i*2] = gDUHex[b / 16];
		ret[i*2 + 1] = gDUHex[b % 16];
	}
	return ret;
}


ODULong		ODGetODULongFromISOStrKey(ODISOStr isoStrKey)
{
	ODULong ret;
	for (ODUByte i = 0; i<sizeof(ODULong); ++i)
	{
		ODUByte n1 = 0, n2 = 0;
		n1 = isoStrKey[i*2];
		n2 = isoStrKey[i*2+1];
		if (n1>'9') 
			n1=n1-'A'+10;
		else 
			n1=n1-'0';
		if (n2>'9') 
			n2=n2-'A'+10;
		else 
			n2=n2-'0';
		
		((ODUByte*)(&ret))[i] = n1*16 + n2;
	}
	return ret;
}


#if ( !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) ) \
     && !defined(_PLATFORM_AIX_)
//------------------------------------------------------------------------------
// ODDeduceRefNum
//------------------------------------------------------------------------------

ODFileRefNum	ODDeduceRefNum(Environment* ev, PlatformFile* file, 
					ODULong prevRefNumCount, ODSShort* prevRefNums)
{
	ODFileRefNum	retval = 0;
	ODULong 		newRefNumCount = 0;
	ODSShort* 		newRefNums = kODNULL;
	file->GetLocalPaths(kDataFork, &newRefNumCount, &newRefNums);

	WASSERT(newRefNumCount == prevRefNumCount + 1);

	ODULong i = 0,j;
	ODBoolean found = kODFalse;
	
	while  (i<newRefNumCount)
	{
		j = 0;
		while (j<prevRefNumCount && (found = (newRefNums[i] == prevRefNums[j]))==kODFalse)
			++j;
		if (!found)
			break;
		++i;
	}
	
	WASSERT(i<newRefNumCount);
	
	retval = newRefNums[i];
	
	if (newRefNums) 
		ODDisposePtr((Ptr)newRefNums);
	
	return retval;
}



//==============================================================================
// Opening an file/container/document/draft
//==============================================================================


void ODAcquireCtrDocTopDraft(Environment* ev, ODSession* session,PlatformFile* file,
		ODContainer** container, ODDocument** document, ODDraft** draft)
{
	*container = GetFileContainer(ev, session, &(file->GetFileSpec()));
	*document = (*container)->AcquireDocument(ev,kODDefaultDocument);
	*draft = (*document)->AcquireDraft(ev,kODDPReadOnly,0,kODNULL,kODPosTop,kODFalse);
}

#endif // !_PLATFORM_WIN32_ && !_PLATFORM_OS2 && !_PLATFORM_AIX

//------------------------------------------------------------------------------
// ODGetFirstOpenDocument
//------------------------------------------------------------------------------

ODDocument*	ODGetFirstOpenDocument(Environment* ev, ODSession* session)
{
	return ODGetNthOpenDocument(ev, session, 1);
}
				
//------------------------------------------------------------------------------
// ODGetNthOpenDocument
//
// n is 1 based & returns kODNULL if there is no nth document
//------------------------------------------------------------------------------

ODDocument*	ODGetNthOpenDocument(Environment* ev, ODSession* session, ODULong n)
{
	WASSERT(session != kODNULL);
	
	if (!session)
		THROW(kODErrIllegalNullInput);
		
	ODNameSpaceManager* theNameSpaceManager = session->GetNameSpaceManager(ev);
	ODObjectNameSpace* refNumDocumentNameSpace = 
		(ODObjectNameSpace*)theNameSpaceManager->HasNameSpace( ev, 
														kODRefNumDocument);
	RETURN_IF_NULL(refNumDocumentNameSpace, kODNULL);
	
	ODObjectIterator* iter = refNumDocumentNameSpace->CreateIterator(ev);
	ODDocument* document = kODNULL;
	ODISOStr unusedKey = kODNULL;
	ODULong unusedObjectLength;
	iter->First(ev, &unusedKey, (ODObject**)&document, &unusedObjectLength);
	while (--n > 0 && document != kODNULL)
	{
		ODDisposePtr(unusedKey);
		unusedKey = kODNULL;
		document = kODNULL;
		iter->Next(ev, &unusedKey, (ODObject**)&document, &unusedObjectLength);
	}
	ODDisposePtr(unusedKey);
	ODDeleteObject(iter);
	return document;
}
				
#if !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
//------------------------------------------------------------------------------
// ODGetOpenDocumentFromRefNum
//------------------------------------------------------------------------------

ODDocument*	ODGetOpenDocumentFromRefNum(Environment* ev, ODSession* session,
				ODFileRefNum refnum)
{
	WASSERT(session != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
		
	ODNameSpaceManager* theNameSpaceManager = session->GetNameSpaceManager(ev);
	ODObjectNameSpace* refNumDocumentNameSpace = 
		(ODObjectNameSpace*)theNameSpaceManager->HasNameSpace( ev, 
														kODRefNumDocument);
	RETURN_IF_NULL(refNumDocumentNameSpace, kODNULL);
	ODDocument* document = kODNULL;
	TempODISOStr	key = ODCreateISOStrKeyFromULong(refnum);
	refNumDocumentNameSpace->GetEntry(ev, key, (ODObject**)&document);
	return document;
}
#endif // _PLATFORM_WIN32_
				
//------------------------------------------------------------------------------
// ODGetTempDraftFromOpenDocument
//------------------------------------------------------------------------------
				
ODDraft*	ODGetTempDraftFromOpenDocument(Environment* ev, ODSession* session,
				ODDocument* document)
{
	RETURN_IF_NULL(document, kODNULL);

	WASSERT(session != kODNULL);
	if (!session)
		THROW(kODErrIllegalNullInput);
		
	ODNameSpaceManager* theNameSpaceManager = session->GetNameSpaceManager(ev);

	ODObjectNameSpace* documentTempDraftNameSpace = 
		(ODObjectNameSpace*)theNameSpaceManager->HasNameSpace( ev, 
														kODDocumentTempDraft);
	RETURN_IF_NULL(documentTempDraftNameSpace, kODNULL);
	ODDraft* draft = kODNULL;
	TempODISOStr	key = ODCreateISOStrKeyFromULong((ODULong)document);
	documentTempDraftNameSpace->GetEntry(ev, key, (ODObject**)&draft);
	return draft;
}

//------------------------------------------------------------------------------
// ODTempDraftCreated
//------------------------------------------------------------------------------
				
void	ODTempDraftCreated(Environment* ev, ODSession* session, 
			ODDocument* document, ODDraft* tempDraft)
{
	WASSERT(session != kODNULL);
	WASSERT(document != kODNULL);
	WASSERT(tempDraft != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!document)
		THROW(kODErrIllegalNullDocumentInput);
	if (!tempDraft)
		THROW(kODErrIllegalNullDraftInput);
		
	ODNameSpaceManager* theNameSpaceManager = session->GetNameSpaceManager(ev);

	ODObjectNameSpace* nameSpace =	
		(ODObjectNameSpace*)theNameSpaceManager->HasNameSpace( ev, 
														kODDocumentTempDraft);
	if (!nameSpace)
	{
		nameSpace = 
			(ODObjectNameSpace*)theNameSpaceManager->CreateNameSpace(
				ev, kODDocumentTempDraft, kODNULL, 5, kODNSDataTypeODObject);
	}
	
	{
		TempODISOStr key = ODCreateISOStrKeyFromULong((ODULong)document);
		ODDraft* unusedDraft;
		if (nameSpace->GetEntry(ev, key, (ODObject**)&unusedDraft))
			nameSpace->Unregister(ev, key);
		nameSpace->Register(ev, key, tempDraft);
	}
}

//------------------------------------------------------------------------------
// ODDocumentOpened
//------------------------------------------------------------------------------
				
void		ODDocumentOpened(Environment* ev, ODSession* session,
				ODFileRefNum refnum, ODDocument* document, ODDraft* tempDraft)
{
	WASSERT(session != kODNULL);
	WASSERT(document != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!document)
		THROW(kODErrIllegalNullDocumentInput);
		
	ODNameSpaceManager* theNameSpaceManager = session->GetNameSpaceManager(ev);

	document->Acquire(ev);	// Balanced by Release in ODDocumentClosed.
	document->GetContainer(ev)->Acquire(ev); // Balanced by Release in ODDocumentClosed.

	//¥ it's not clear whether ODDocumentClosed needs to be called if this fn fails.
	//¥ Failure halfway through leaves things in an inconsistent state and it doesn't
	//¥ look like there's any way to clean up fully.

	ODObjectNameSpace* nameSpace = 
		(ODObjectNameSpace*)theNameSpaceManager->HasNameSpace( ev, 
														kODRefNumDocument);
	if (!nameSpace)
	{
		nameSpace = 
			(ODObjectNameSpace*)theNameSpaceManager->CreateNameSpace(
				ev, kODRefNumDocument, kODNULL, 5, kODNSDataTypeODObject);
	}
	
	{
		TempODISOStr key = ODCreateISOStrKeyFromULong((ODULong)refnum);
#if ODDebug
		ODDocument* unusedDocument;
		if (nameSpace->GetEntry(ev, key, (ODObject**)&unusedDocument))
			WARNMSG(WARN_INDEX(0),"refnum already in kODRefNumDocument namespace!");
#endif
		nameSpace->Register(ev, key, document);
	}
	
	if (tempDraft)
		ODTempDraftCreated(ev, session, document, tempDraft);
}
				
//------------------------------------------------------------------------------
// ODDocumentClosed
//------------------------------------------------------------------------------
				
void	ODDocumentClosed(Environment* ev, ODSession* session,
				ODDocument* document)
{
	WASSERT(session != kODNULL);
	WASSERT(document != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!document)
		THROW(kODErrIllegalNullDocumentInput);
		
	ODNameSpaceManager* theNameSpaceManager = session->GetNameSpaceManager(ev);
	ODObjectNameSpace* nameSpace;
	
	nameSpace = 
		(ODObjectNameSpace*)theNameSpaceManager->HasNameSpace( ev, 
													kODDocumentTempDraft);
	if (nameSpace)
	{
		TempODISOStr key = ODCreateISOStrKeyFromULong((ODULong)document);
		ODDraft* unusedDraft;
		if (nameSpace->GetEntry(ev, key, (ODObject**)&unusedDraft))
			nameSpace->Unregister(ev, key);
	}
	
	nameSpace = 
		(ODObjectNameSpace*)theNameSpaceManager->HasNameSpace( ev, 
														kODRefNumDocument);
	if (!nameSpace)
		return;

	ODDocument* entryDocument = kODNULL;
	ODISOStr key = kODNULL;
	ODULong unusedObjectLength;
	ODObjectIterator* iter = nameSpace->CreateIterator(ev);
	for (iter->First(ev, &key, (ODObject**)&entryDocument, &unusedObjectLength);
		 iter->IsNotComplete(ev);
		 iter->Next(ev, &key, (ODObject**)&entryDocument, &unusedObjectLength))
	{
		TempODISOStr tempKey = key; // DMc: ensure it's deleted
		if (ODObjectsAreEqual(ev, document, entryDocument))
		{
			nameSpace->Unregister(ev, key);
			ODContainer* container = document->GetContainer(ev);
			ODFinalReleaseObject(ev, document);
			ODFinalReleaseObject(ev, container);
			break;
		}
	}
	ODDeleteObject(iter);
}


//==============================================================================
// Root Part/StorageUnit
//==============================================================================

//------------------------------------------------------------------------------
// ODAcquireRootPartOfDraft
//------------------------------------------------------------------------------

ODPart* ODAcquireRootPartOfDraft(Environment* ev, ODDraft* draft)
{
	if (!draft)
		THROW(kODErrIllegalNullDraftInput);
		
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);
	return draft->
		AcquirePart(ev,ODGetStrongSURefProp(ev, su, kODPropRootPartSU, kODStrongStorageUnitRef));
}

//------------------------------------------------------------------------------
// ODAcquireRootPartSUOfDraft
//------------------------------------------------------------------------------

ODStorageUnit* ODAcquireRootPartSUOfDraft(Environment* ev, ODDraft* draft)
{
	if (!draft)
		THROW(kODErrIllegalNullDraftInput);
		
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);
	if (!su->Exists(ev, kODPropRootPartSU, kODStrongStorageUnitRef, 0))
		return kODNULL;
		
	return draft->
		AcquireStorageUnit(ev,ODGetStrongSURefProp(ev, su, kODPropRootPartSU, kODStrongStorageUnitRef));
}

//------------------------------------------------------------------------------
// ODSetRootPartSUOfDraft
//------------------------------------------------------------------------------

void ODSetRootPartSUOfDraft(Environment* ev, ODDraft* draft, ODStorageUnit* rootPartSU)
{
	if (!draft)
		THROW(kODErrIllegalNullDraftInput);
	if (!rootPartSU)
		THROW(kODErrIllegalNullStorageUnitInput);
		
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);
	ODSetStrongSURefProp(ev, su, kODPropRootPartSU, kODStrongStorageUnitRef,
						rootPartSU->GetID(ev));
}

//------------------------------------------------------------------------------
// ODGetDraftOfWindow
//  Given a window object, get the root part's storageUnit's draft
//------------------------------------------------------------------------------

ODDraft* ODGetDraftOfWindow(Environment* ev, ODWindow* window)
{
	RETURN_IF_NULL(window, kODNULL);
		
	TempODPart part = window->GetRootFrame(ev)->AcquirePart(ev);
	return part->GetStorageUnit(ev)->GetDraft(ev);
}

#if ( !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) ) \
      && !defined(_PLATFORM_AIX_)
//------------------------------------------------------------------------------
// ODResetDateModByInfo
//------------------------------------------------------------------------------

void ODResetDateModByInfo(Environment* ev, ODStorageUnit* su)
{
	WASSERT(su != kODNULL);
	if (!su)
		THROW(kODErrIllegalNullStorageUnitInput);

	WASSERT(HAS_WRITE_ACCESS(su->GetDraft(ev)->GetPermissions(ev)));
			
	ODTime dateTime;
	time((time_t *)(&dateTime));
	ODSetCreationDate(ev, su, dateTime);
	ODSetModificationDate(ev, su, dateTime);
	
	ODIText		modBy;
	su->GetSession(ev)->GetUserName(ev, &modBy);
	TRY
		ODSetModifiedBy(ev, su, &modBy);
	CATCH_ALL
		DisposeITextStruct(modBy);
		RERAISE;
	ENDTRY
	DisposeITextStruct(modBy);
	
	TempODStorageUnit	rootSU = ODAcquireRootPartSUOfDraft(ev, su->GetDraft(ev));
	if (ODObjectsAreEqual(ev, su, rootSU))
	{
		TempPlatformFile	file = GetPlatformFileFromContainer(ev, 
			su->GetDraft(ev)->GetDocument(ev)->GetContainer(ev));
		TempODName 			partname = file->GetName();
		//ODSetPOName(ev, rootPart, partname); // don't want to have to have the part soÉ
		ODSetITextProp(ev, su, kODPropName, kODMacIText, partname);
	}
}

//------------------------------------------------------------------------------
// ODGetDocumentFileName
//------------------------------------------------------------------------------

void ODGetDocumentFileName(Environment* ev, ODDocument* document, char* fileName)
{
	TempPlatformFile	file = GetPlatformFileFromContainer(ev, document->GetContainer(ev));
	file->GetAsciiName(fileName, 255);
}
#endif // !_PLATFORM_WIN32_ && !_PLATFORM_OS2_ && !_PLATFORM_AIX_

//==============================================================================
// Cursor Utils
//==============================================================================

void WatchCursor()
{
#ifdef _PLATFORM_WIN32_
	SetCursor(LoadCursor(NULL, IDC_WAIT));
#elif _PLATFORM_MACINTOSH_
	SetCursor(*GetCursor(watchCursor));
#endif // _PLATFORM_WIN32_
}

void ArrowCursor()
{
#ifdef _PLATFORM_WIN32_
	SetCursor(LoadCursor(NULL, IDC_ARROW));
#elif _PLATFORM_MACINTOSH_
	SetCursor(&(ODQDGlobals.arrow));
#endif // _PLATFORM_WIN32_
}

//==============================================================================
// Active/Changed Utils
//==============================================================================

ODWindow*	ODAcquireActiveWindow(Environment* ev, ODSession* session)
{
	WASSERT(session != kODNULL);
	if (!session)
		THROW(kODErrIllegalNullInput);
		
	return session->GetWindowState(ev)->AcquireActiveWindow(ev);
}

ODDraft*	ODGetActiveDraft(Environment* ev, ODSession* session)
{
	TempODWindow window = ODAcquireActiveWindow(ev, session);
	return ODGetDraftOfWindow(ev, window);
}

ODDocument*	ODGetActiveDocument(Environment* ev, ODSession* session)
{
	ODDraft* draft = ODGetActiveDraft(ev, session);
	RETURN_IF_NULL(draft, kODNULL);	
	
	return draft->GetDocument(ev);
}

ODBoolean	ODDocumentHasChanges(Environment* ev, ODSession* session,
				ODDocument* document)
{
	ODDraft* draft = ODGetTempDraftFromOpenDocument(ev, session, document);
	RETURN_IF_NULL(draft, kODFalse);	

	return draft->ChangedFromPrev(ev);
}

ODBoolean	ODDocumentHasWriteAccess(Environment* ev, ODSession* session,
				ODDocument* document)
{
	return ODDraftHasWriteAccess(ev, ODGetTempDraftFromOpenDocument(ev, session, document));
}


ODBoolean ODDraftHasWriteAccess(Environment* ev, ODDraft* draft)
{
	RETURN_IF_NULL(draft, kODFalse);	
	
	return HAS_WRITE_ACCESS(draft->GetPermissions(ev));
}

//==============================================================================
// User level document operations
//==============================================================================

//------------------------------------------------------------------------------
// ODNewDocument
//------------------------------------------------------------------------------

#define	kODErrEditorLibraryNotFound				-29816
// error code copied from the Shell (RlShlDef.h)

#if ( !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) ) \
      && !defined(_PLATFORM_AIX_)
void ODNewDocument(Environment* ev, ODContainer* container,
							ODType		rootPartKind,
							ODEditor	rootPartEditor)
{
	TempODDocument	document = container->AcquireDocument(ev, kODDefaultDocument);
	TempODDraft		draft = document->AcquireBaseDraft(ev, kODDPExclusiveWrite);
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);

	ODResetDateModByInfo(ev, su);	
	
  // create new root part
  	TempODPart rootPart = kODNULL;
  	
  	TRY
		rootPart = draft->CreatePart(ev, rootPartKind, rootPartEditor);
	CATCH_ALL
		if (ErrorCode()==fragLibNotFound)
			THROW(kODErrEditorLibraryNotFound);
		else
			RERAISE;
	ENDTRY
	ODStorageUnit* partSU = rootPart->GetStorageUnit(ev);
	if (partSU == kODNULL)
		THROW(kODErrEditorLibraryNotFound);
	ODSetRootPartSUOfDraft(ev, draft, partSU);

	ODSetBooleanProp(ev, su, kODPropUnsavedDocument, kODBoolean, kODTrue);

	draft->Externalize(ev);

	ODPlatformType	rootPartOSType = ODGetIconFilePlatformTypeFromPartSU(ev, partSU);

	TempPlatformFile file = GetPlatformFileFromContainer(ev, container);
	file->SetPlatformType(rootPartOSType);
}
#endif // !_PLATFORM_WIN32_ && !_PLATFORM_OS2 && !_PLATFORM_AIX

//------------------------------------------------------------------------------
// ODIsUnsavedNewDocument
//------------------------------------------------------------------------------

ODBoolean ODIsUnsavedNewDocument(Environment* ev, ODSession* session, ODDocument* document)
{
	ODDraft* draft = ODGetTempDraftFromOpenDocument(ev, session, document);
	RETURN_IF_NULL(draft, kODFalse);
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);
	if (!su->Exists(ev, kODPropUnsavedDocument, kODBoolean, 0))
		return kODFalse;
	return ODGetBooleanProp(ev, su, kODPropUnsavedDocument, kODBoolean);
}

//------------------------------------------------------------------------------
// ODSetIsUnsavedNewDocument
//------------------------------------------------------------------------------

void ODSetIsUnsavedNewDocument(Environment* ev, ODDraft* draft)
{
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);
	ODSetBooleanProp(ev, su, kODPropUnsavedDocument, kODBoolean, kODTrue);
}

//------------------------------------------------------------------------------
// ODOpenFileDocument
//------------------------------------------------------------------------------

#if ( !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) ) \
     && !defined(_PLATFORM_AIX_)

ODDraft* ODOpenFileDocument(Environment* ev, ODSession* session, PlatformFile* file,
	ODDraftPermissions permissions)
{
	WASSERT(session != kODNULL);
	WASSERT(file != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!file)
		THROW(kODErrIllegalNullInput);

	ODContainer*	container	= kODNULL;
	ODDocument*		document	= kODNULL;
	ODDraft*		currentdraft	= kODNULL;
	ODDraft*		tempdraft	= kODNULL;
	
	ODULong refNumCount, i;
	ODSShort* refNums = kODNULL;
	file->GetLocalPaths(kDataFork, &refNumCount, &refNums);
	
	for ( i=0; i<refNumCount; ++i)
		if ((document = ODGetOpenDocumentFromRefNum(ev, session, refNums[i])) != kODNULL)
		{
			currentdraft = ODGetTempDraftFromOpenDocument(ev, session, document);
			if (currentdraft)
				currentdraft->Acquire(ev);
			else
				currentdraft = document->AcquireDraft(ev,kODDPReadOnly,0,kODNULL,kODPosTop,kODFalse);
			if (refNums)
				ODDisposePtr(refNums);
			return currentdraft;
		}

	// ODAcquireCtrDocTopDraft opens the file and adds a new refnum.

	ODAcquireCtrDocTopDraft(ev, session, file, &container, &document, &currentdraft);
	TempODContainer tempCont = container;		// Ensures they're released
	TempODDocument  tempDoc  = document;
	ODFileRefNum	refnum = ODDeduceRefNum(ev, file, refNumCount, refNums);
	if (refNums)
		ODDisposePtr(refNums);
	
	if (HAS_WRITE_ACCESS(permissions) && !file->IsLocked())
	{
		currentdraft = 
			document->AcquireDraft(ev, permissions,0, currentdraft, kODPosSame, kODTrue);
		currentdraft = document->CreateDraft(ev, currentdraft, kODTrue);
		tempdraft = currentdraft;
		tempdraft->Acquire(ev); 
			// Because "The caller is responsible for properly refcounting the tempdraft."
			// From DocUtils.h documentation of ODDocumentOpened.
	}
			
	ODDocumentOpened(ev, session, refnum, document, tempdraft);
	
	return currentdraft;
}

#endif // !_PLATFORM_WIN32_ && !_PLATFORM_OS2_ && !_PLATFORM_AIX_
//------------------------------------------------------------------------------
// ODOpenDraft
//------------------------------------------------------------------------------

void ODOpenDraft(Environment* ev, ODSession* session, ODDraft* draft)
{
	WASSERT(session != kODNULL);
	WASSERT(draft != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!draft)
		THROW(kODErrIllegalNullDraftInput);

	ODBoolean	windowsOpen = kODFalse;
	ODWindowState* windowState = session->GetWindowState(ev);

	windowState->Internalize(ev,draft);
	windowState->OpenWindows(ev, draft);
	windowsOpen = (windowState->GetRootWindowCount(ev, draft)>0);

	//need this for NoPart alert 
	if (session->GetDispatcher(ev)->ShouldExit(ev))
		return;
		
	if (!windowsOpen)
	{
		TempODPart	rootPart = ODAcquireRootPartOfDraft(ev, draft);
		rootPart->Open(ev, kODNULL);
		windowState->SetDefaultWindowTitles(ev, draft);
	}
	session->GetLinkManager(ev)->DraftOpened(ev,draft);
}

//------------------------------------------------------------------------------
// ODCloseDraft
//------------------------------------------------------------------------------

void ODCloseDraft(Environment* ev, ODSession* session, ODDraft* draft)
{
	WASSERT(session != kODNULL);
	WASSERT(draft != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!draft)
		THROW(kODErrIllegalNullDraftInput);
		
	session->GetLinkManager(ev)->DraftClosing(ev, draft);
	session->GetUndo(ev)->ClearActionHistory(ev, kODDontRespectMarks);
	session->GetClipboard(ev)->DraftClosing(ev, draft);
	session->GetWindowState(ev)->CloseWindows(ev, draft);
}

//------------------------------------------------------------------------------
// ODCloseDocument
//------------------------------------------------------------------------------

ODBoolean ODCloseDocument(Environment* ev, ODSession* session, ODDocument* document)
{
	RETURN_IF_NULL(document, kODFalse);	// For the caller's convenience.

	ODDraft* tempDraft = ODGetTempDraftFromOpenDocument(ev, session, document);

	ODRefCntCollection*	draftsWithOpenWindows = new ODRefCntCollection(ev);

	ODWindowIterator* iter = session->GetWindowState(ev)->CreateWindowIterator(ev);
	ODWindow* window = kODNULL;
	while ((window = iter->Next(ev)) != kODNULL)
	{
		// close all windows whose draft's document = document
		ODDraft* draft = ODGetDraftOfWindow(ev, window);
		WASSERT(draft != kODNULL);
		if (ODObjectsAreEqual(ev, document, draft->GetDocument(ev))
			&& !draftsWithOpenWindows->Contains(draft))
			draftsWithOpenWindows->AddFirstAndAcquire(draft);
	}
	ODDeleteObject(iter);

	ODDraft* draft = kODNULL;
	while ((draft = (ODDraft*)draftsWithOpenWindows->RemoveFirst()) != kODNULL)
	{
		ODCloseDraft(ev, session, draft);
		draft->Release(ev); // to balance AddFirstAndAcquire above.
	}

	ODDeleteObject(draftsWithOpenWindows);
	if (tempDraft)
	{
		tempDraft->RemoveChanges(ev);
		WASSERT(tempDraft->GetRefCount(ev) == 1);
		tempDraft->RemoveFromDocument(ev);
	}
	ODDocumentClosed(ev, session, document);
	
	TempODWindow activeWindow = ODAcquireActiveWindow(ev, session);
	return (activeWindow == kODNULL);
}

//------------------------------------------------------------------------------
// ODReleaseCloseWindow
//------------------------------------------------------------------------------

ODBoolean ODReleaseCloseWindow(Environment* ev, ODSession* session, ODWindow* window)
{
	WASSERT(session != kODNULL);
	WASSERT(window != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!window)
		THROW(kODErrIllegalNullInput);

	WASSERT(window->GetRefCount(ev) > 1);
#if ODDebug
	ODULong windowRefCount = window->GetRefCount(ev);
	if (windowRefCount != 3)
	{
		WARNMSG(WARN_INDEX(0),"Trying to close a window with RefCount of (%d) != 3.", windowRefCount);
		WARNMSG(WARN_INDEX(0),"Éexpect problems later closing the draft, document and container.-T‚");
		WARNMSG(WARN_INDEX(0),"Solution? Make sure window is properly refcounted.");
		WARNMSG(WARN_INDEX(0),"Possible Culprit: Part::Open forgetting to release a registered window.");
	}
#endif
	
	ODBoolean wasLastRootWindow = kODFalse;
	ODDraft* draft = ODGetDraftOfWindow(ev, window);
	draft->Acquire(ev);	// necessary just in case closing all windows fully releases the draft
	ODBoolean isOnlyDraftWindow 
		= window->IsRootWindow(ev) && 
		(session->GetWindowState(ev)->GetRootWindowCount(ev, draft) == 1);
	if (isOnlyDraftWindow)
	{
		window->Release(ev);
		ODCloseDraft(ev, session, draft);

		ODDocument* document = draft->GetDocument(ev);
		if (!document->Exists(ev, kODNULLID, draft, kODPosFirstAbove))
		{
			ODReleaseObject(ev, draft); // to balance the above Acquire before calling ODCloseDocument.
			wasLastRootWindow = ODCloseDocument(ev, session, document);
		}
	}
	else
	{
		window->CloseAndRemove(ev);
	}
	ODReleaseObject(ev, draft);
	return wasLastRootWindow;
}

#if ( !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) ) \
     && !defined(_PLATFORM_AIX_)
//------------------------------------------------------------------------------
// ODDeleteDocument
//------------------------------------------------------------------------------

ODBoolean ODDeleteDocument(Environment* ev, ODSession* session, ODDocument* document)
{
	WASSERT(session != kODNULL);
	WASSERT(document != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!document)
		THROW(kODErrIllegalNullDocumentInput);
		
	TempPlatformFile	currentFile = 
		GetPlatformFileFromContainer(ev, document->GetContainer(ev));							
	ODBoolean retval = ODCloseDocument(ev, session, document);
	currentFile->MoveToTrash();
	return retval;
}

//------------------------------------------------------------------------------
// ODSaveDocument
//------------------------------------------------------------------------------

void ODSaveDocument(Environment* ev, ODSession* session, ODDocument* document)
{
	WASSERT(session != kODNULL);
	WASSERT(document != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!document)
		THROW(kODErrIllegalNullDocumentInput);

	ODDraft* tempDraft = ODGetTempDraftFromOpenDocument(ev, session, document);
	if (tempDraft)
	{
		WatchCursor();
		session->GetUndo(ev)->ClearActionHistory(ev, kODDontRespectMarks);
	
		// Fix desktop information which parallels the rootpart such as:
		// filename, comments, stationery bit, etc.
		SyncPropertiesToFile(ev, session, document);
	
		TempODStorageUnit su = tempDraft->AcquireDraftProperties(ev);
		ODSURemoveProperty(ev, su, kODPropUnsavedDocument);
		
		session->GetWindowState(ev)->Externalize(ev, tempDraft);
		tempDraft->Externalize(ev);
		session->GetLinkManager(ev)->DraftSaved(ev, tempDraft);
		session->GetClipboard(ev)->DraftSaved(ev, tempDraft);
	
		tempDraft->SaveToAPrevious(ev, kODNULL);

		ArrowCursor();
		session->GetDispatcher(ev)->InvalidateFacetUnderMouse(ev);
	}
}

//------------------------------------------------------------------------------
// ODSaveACopyOfDraft
//	Saves a copy of the draft.
//------------------------------------------------------------------------------

void ODSaveACopyOfDraft(Environment* ev, ODSession* session, ODDraft* fromDraft, 
						PlatformFile* destinationFile)
{
	WASSERT(session != kODNULL);
	WASSERT(fromDraft != kODNULL);
	WASSERT(destinationFile != kODNULL);

	if (!session)
		THROW(kODErrIllegalNullInput);

	if (!fromDraft)
		THROW(kODErrIllegalNullDraftInput);

	if (!destinationFile)
		THROW(kODErrIllegalNullInput);
		
	WatchCursor();
	if (ODDraftHasWriteAccess(ev, fromDraft) && fromDraft->ChangedFromPrev(ev))
	{
		session->GetWindowState(ev)->Externalize(ev, fromDraft);
		fromDraft->Externalize(ev);
	}
	
	TempPlatformFile currentFile = 
		GetPlatformFileFromContainer(ev, fromDraft->GetDocument(ev)->GetContainer(ev));
	TempODStorageUnit partSU = ODAcquireRootPartSUOfDraft(ev, fromDraft);
	ODPlatformType fileType = ODGetIconFilePlatformTypeFromPartSU(ev, partSU);
	destinationFile->Create(currentFile->GetPlatformCreator(),fileType,0);
	
	TempODContainer	newContainer = 
		CreateFileContainer(ev, session, &(destinationFile->GetFileSpec()));	
	TempODDocument	newDocument = newContainer->AcquireDocument(ev, kODDefaultDocument);
	newDocument->SetBaseDraftFromForeignDraft(ev, fromDraft);
	TempODDraft	newDraft = newDocument->AcquireBaseDraft(ev, kODDPExclusiveWrite);
	TempODStorageUnit su = newDraft->AcquireDraftProperties(ev);
	ODSURemoveProperty(ev, su, kODPropUnsavedDocument);
	newDraft->Externalize(ev);

	ArrowCursor();
}

//------------------------------------------------------------------------------
// ODRevertDocument
//------------------------------------------------------------------------------

void ODRevertDocument(Environment* ev, ODSession* session, ODDocument* document)
{
	WASSERT(session != kODNULL);
	WASSERT(document != kODNULL);
	if (!session)
		THROW(kODErrIllegalNullInput);
	if (!document)
		THROW(kODErrIllegalNullDocumentInput);

	ODDraft* tempDraft = ODGetTempDraftFromOpenDocument(ev, session, document);
	if (tempDraft)
	{
		WatchCursor();
		ODCloseDraft(ev, session, tempDraft);	
		tempDraft->RemoveChanges(ev);
		ODOpenDraft(ev, session, tempDraft);
		SyncFileToProperties(ev, session, document);
		ArrowCursor();
		session->GetDispatcher(ev)->InvalidateFacetUnderMouse(ev);
	}
}
#endif // !_PLATFORM_WIN32_ && !_PLATFORM_OS2_ && !_PLATFORM_AIX_

#ifdef _PLATFORM_MACINTOSH_
//------------------------------------------------------------------------------
// ODDocumentInfo
//------------------------------------------------------------------------------

void ODDocumentInfo(Environment* ev, ODSession* session)
{
	WASSERT(session != kODNULL);
	if (!session)
		THROW(kODErrIllegalNullInput);

	ArrowCursor();

	ODFacet* infoFacet = kODNULL;
	ODBoolean hasWriteAccess = kODFalse;
	{
	TempODWindow window = session->GetWindowState(ev)->AcquireFrontRootWindow(ev);
	infoFacet = window->GetRootFacet(ev);
	hasWriteAccess = ODDraftHasWriteAccess(ev, ODGetDraftOfWindow(ev, window));
	}
	session->GetInfo(ev)->ShowPartFrameInfo(ev, infoFacet, hasWriteAccess);
}
#endif


//------------------------------------------------------------------------------
// ODIsTempDocument
//------------------------------------------------------------------------------

ODBoolean ODIsTempDocument(Environment* ev, ODSession* session, ODDocument* document)
{
	ODDraft* draft = ODGetTempDraftFromOpenDocument(ev, session, document);
	RETURN_IF_NULL(draft, kODFalse);
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);
	if (!su->Exists(ev, kODPropTempDocument, kODBoolean, 0))
		return kODFalse;
	return ODGetBooleanProp(ev, su, kODPropTempDocument, kODBoolean);
}

#if ( !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) ) \
      && !defined(_PLATFORM_AIX_)
//------------------------------------------------------------------------------
// ODSetIsTempDocument
//------------------------------------------------------------------------------

void ODSetIsTempDocument(Environment* ev, ODDraft* draft, ODBoolean isTemp)
{
	if (!draft)
		return;
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);
	if (isTemp)
		ODSetBooleanProp(ev, su, kODPropTempDocument, kODBoolean, isTemp);
	else
		ODSURemoveProperty(ev, su, kODPropTempDocument);
}

//------------------------------------------------------------------------------
// SyncFileToProperties
//------------------------------------------------------------------------------


void SyncFileToProperties(Environment* ev, ODSession* session, ODDocument* document)
{
	ODDraft*			tempDraft	= ODGetTempDraftFromOpenDocument(ev, session, document);
	TempODPart			rootPart	= ODAcquireRootPartOfDraft(ev, tempDraft);
	ODIText*			foo			= kODNULL;
	ODIconFamily		icons		= kODNULL;
	TempPlatformFile	file		= GetPlatformFileFromContainer(ev, document->GetContainer(ev));
	
	// filename
	foo = ODGetPOName(ev, rootPart, kODNULL);
	if (foo)
	{
		Str255	pName;
		GetITextPString(foo, pName);
		TRY
			file->Rename(pName);
		CATCH_ALL
		ENDTRY
		DisposeIText(foo);
		ODWindowState* windowState = session->GetWindowState(ev);
		windowState->SetDefaultWindowTitles(ev, tempDraft);
	}
	
	// stationery
	file->SetStationery(ODGetSUIsStationery(ev, rootPart->GetStorageUnit(ev)));
	
	// comments
	foo = ODGetPOComments(ev, rootPart, kODNULL);
	if (foo != kODNULL)
	{
		file->SetComments(foo);
		DisposeIText(foo);
	}

	// custom icon
	icons = ODGetPOIconFamily(ev, rootPart);
	file->SetCustomIconFamily(icons);
	if (icons)
		THROW_IF_ERROR( DisposeIconSuite(icons, kODTrue) );
}


//------------------------------------------------------------------------------
// SyncPropertiesToFile
//------------------------------------------------------------------------------

void SyncPropertiesToFile(Environment* ev, ODSession* session, ODDocument* document)
{
	TempODWindow		odWindow	= session->GetWindowState(ev)->AcquireFrontRootWindow(ev);
	ODFrame*			frame		= odWindow->GetRootFrame(ev);
	ODDraft*			tempDraft	= ODGetTempDraftFromOpenDocument(ev, session, document);
	TempODPart			rootPart	= ODAcquireRootPartOfDraft(ev, tempDraft);
	ODStorageUnit*		su			= ODGetSUFromPstObj(ev, (ODPart*)rootPart);
	ODIText*			foo			= kODNULL;
	ODIconFamily		icons		= kODNULL;
	TempPlatformFile	file		= GetPlatformFileFromContainer(ev, document->GetContainer(ev));
	
	// filename
	foo = file->GetName();
	if (foo)
	{
		ODSetPOName(ev, rootPart, foo);
		DisposeIText(foo);
	}
	
	// stationery
	ODSetSUIsStationery(ev, su, file->IsStationery());

	// comments
	//
	// this gets comments and makes sure that the file matches the property
	// we just need to make sure that the property matches the file
	foo = ODGetComments(ev, frame, kODNULL);
	if (foo != kODNULL)
	{
		ODSetPOComments(ev, rootPart, foo);
		DisposeIText(foo);
	}

	// custom icon
	icons = file->GetCustomIconFamily();
	ODSetPOIconFamily(ev, rootPart, icons, kODFalse);
	if (icons)
		THROW_IF_ERROR( DisposeIconSuite(icons, kODTrue) );
}

#endif // !_PLATFORM_WIN32_ && !_PLATFORM_OS2_ && !_PLATFORM_AIX_
