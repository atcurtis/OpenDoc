/* @(#)Z 1.17 os2/src/storage/DragDrp.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:51:59 (96/11/06 16:56:02) */
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

/*---------------------------------------------------------------------------
   Name:        DragDrp.cpp

   Description: Contains the implementation of the OS/2 Opendoc Drag&Drop
                Object.

   Notes:
                - source part can place data or promise data into drag su
                - source part can affect dragitem structure placed in
                  drag su at getcontentstorageunit time.
   see below    - target part can create view and call getdatafromdragmanager
   see below    - the rmf on getdatafromdragmanager will be used to focus
                  the drag su. data will be written to this value.

   Assumptions:

   To Do:

   Design discoveries:

	The OS/2 Merlin OpenDoc team designed ODDragAndDrop with the
	primary goal, I'm guessing, of allowing OpenDoc parts to 'fully
	participate' in the community of happy OS/2 desktop applications.

	Apple DR4 ODDragAndDrop was designed to allow parts to initiate
	drag and drop operations and to receive dragged and dropped
	content.  When the user indicates that he wishes to drag selected
	content, parts get one or more storage units from their session's
	ODDragAndDrop object and store in them the user's selected
	content.  The parts then request a StartDrag, and the
	ODDragAndDrop object uses platform APIs to drag the content around
	the user desktop.  As the user gesture enters and moves within a
	part's facet(s), the part receives DragEnter and DragWithin
	messages from the ODDragAndDrop object; the part can respond to
	these calls to communicate its willingness to accept the dragged
	content to the ODDragAndDrop object, and to indicate to the user
	that same information via whatever feedback is appropriate.  Parts
	accomplish these tasks by examining the drag source's storage
	units made available to them via a drag item iterator passed in
	the DragEnter/DragWithin calls.  Finally, the ODDragAndDrop object
	sends a Drop message to the part's facet under the cursor when the
	user completes the drag gesture.  The list of the source part's
	storage units is once again passed to the destination part via a
	drag item iterator, and the destination part may embed,
	incorporate or refuse the dropped content as it chooses.

	The Merlin design departs from this model in at least one key
	area: parts receiving DragEnter, DragWithin and Drop messages are
	not passed an iterator to the list of the source part's storage
	units; parts instead receive an iterator to a list of storage
	units that contain OS/2 DRAGITEM structures.  Parts are required
	to call the OS/2-only OpenDoc APIs 'CanEmbed' or 'CanIncorporate'
	during DragEnter/DragWithin to determine their ability to accept
	the dragged content, and parts are required to call the OS/2-only
	OpenDoc API GetDataFromDragManager during the Drop method to
	obtain the "true" storage unit(s) containing the content that the
	source part intended to transfer in the drag and drop operation.

	I am guessing that this design change was done to allow parts to
	manipulate the DRAGITEM and DRAGINFO structures that are used in
	the OS/2 Direct Manipulation (Drag and Drop) API; it appears that
	access to OS/2 DRAGITEM/DRAGINFO structures was given to OS/2
	drag-capable source parts, allowing them to insert their own OS/2
	direct manipulation information, so that the information would
	subsequently be available to destination parts via methods
	DragEnter, DragWithin, and Drop.  Unfortunately, this results in
	extra work for part writers, it dramatically reduces the
	portability of OpenDoc parts, and I think that in many cases the
	same functionality could be achieved via existing OpenDoc APIs.

	Additional responsibilities of OS/2 OpenDoc parts beyond the
	usual DR4 design:

	1)	In their DragEnter/DragWithin methods, parts must call
		CanEmbed and/or CanIncorporate to determine if they can accept
		the dragged content.
	2)	In their Drop methods, parts must, for each drag item made
		available by the passed drag item iterator:
		a) focus on the kODDragItem value
		b) create a storage unit view
		c) call GetDataFromDragManager to obtain the 'translated'
		   storage units that contain the original data placed in
		   the storage units by the source part

	Changes from previous OS/2 OpenDoc implementations:

	1)	Parts should not return MRESULTs in response to DragEnter,
		DragWithin or Drop, but should instead return regular OpenDoc
		boolean ODDragResults or ODDropResults.


   Format considerations:	formatted for tab width of 4


---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                            Include Files
---------------------------------------------------------------------------*/
#define INCL_WINMESSAGEMGR
#define INCL_WINSTDFILE
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#define INCL_DOSMODULEMGR
#define INCL_DOSRESOURCES
#define INCL_WININPUT
#define INCL_WINSTDDRAG
#define INCL_WINFRAMEMGR
#define INCL_WINWINDOWMGR
#define INCL_WINPOINTERS
#define INCL_WINACCELERATORS
#define INCL_WINSYS
// #define INCL_OSAAPI
// #define INCL_OSAEVENTS

#ifdef _UNICODE_
#define INCL_WIN
#endif

#include <os2.h>

#ifndef _ALTPOINT_
#include "AltPoint.h"				// Use C++ savvy XMPPoint and XMPRect
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>				//  the OpenDoc kitchen sink
#endif

#define ODDragAndDrop_Class_Source
#define VARIABLE_MACROS
#include <DragDrp.xih>

#ifndef SOM_ODDragItemIterator_xh
#include <DgItmIt.xh>
#endif

#ifndef _DRAGPRIV_
#include <DragPriv.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>   // [140007]
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODNew_
#include "ODNew.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef _STORDEF_
#include "StorDef.h"
#endif

#ifndef SOM_CMStorageUnit_xh
#include <CMSU.xh>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef _PASCLSTR_
#include <PasclStr.h>
#endif

#ifndef _LINKDLGS_
#include "LinkDlgs.h"
#endif

#ifndef __MEMORY__
#include <Memory.h>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODValueNameSpace_xh
#include <ValueNS.xh>
#endif

#ifndef _STORUTIL_
#include "StorUtil.h"
#endif

#include <string.h>

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#if ODDebug
#ifndef __strstream_h			// for StackCrawl stuff
#include <strstrea.h>
#endif
#include "stkcrawl.h"
#endif

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

//  [140007] Part registration includes
#include    <RManager.xh>
#include    <ODPrtReg.xh>

//==============================================================================
// Constants
//==============================================================================

#if ODDebug
// #define ODDebug_DragAndDrop 1
#endif

// NASTY HACK:
// Because of the way that the OS/2 implementation dorks the storage units that
// are dragged around by the ODDragAndDrop object (see 'Design discoveries' at
// top of file for complete discussion), we need a way to communicate the
// original clone kind of a drag and drop data transfer operation to the target
// of a drop when the target is in a different process than the source.  In my
// quest to find the nastiest hack possible, I have decided to change the case
// of the hstrTargetName when the source specifies that the clone operation is
// a copy.  This will allow ODDragAndDrop objects in non-local target processes
// to quickly determine if the source's original clone kind was a copy or not.
// [For more boring discussion of the implications of not properly interpreting
// the clone kind, see the comments in dmDrop that start with the line:
//     'check to see whether the source wants to allow only copy']
// 
// So, careful now - don't change the OPENDOC_FILENAME_DEFAULT unless you know
// what is going on.  When the OS/2 implementation is brought in line with DR4,
// this nasty hack won't be necessary.
#define OPENDOC_FILENAME_DEFAULT		"OPENDOC.OD"
#define OPENDOC_FILENAME_FOR_CLONECOPY	"OpenDoc.OD"

//==============================================================================
// Scalar Types
//==============================================================================

typedef struct
{
	ODPart            *sourcePart;
	ODStorageUnitView *destSUView;
} ODPromiseDesc;

/*---------------------------------------------------------------------------
                               Typedefs
---------------------------------------------------------------------------*/

// Structures to aid in accessing the EA information lists.
// (taken from shfsys.h)

// Ascii subtype used to help read the ASCII type in an MVMT list.
typedef struct eastascii
{
	USHORT        wEASType;
	USHORT        cbValue;
	char          bValue[1];
} EASTASCII;

// ASCII EA type structure.
typedef struct eatascii
{
	USHORT        cbValue;
	char          bValue[1];
} EATASCII;

// MVMT EA type structure.
typedef struct eatmvmt
{
	USHORT        wReserved;
	USHORT        cItem;
} EATMVMT;

// Union of EA type structures.
typedef union eatypes
{
	EATASCII      stEATAscii;
	EATMVMT       stEATMvmt;
} EATYPES;

// Value output structure used to help read EA values.
typedef struct ea2value
{
	USHORT        wEAType;
	EATYPES       unEATypes;
} FEA2VALUE;

/*---------------------------------------------------------------------------
                              Prototypes
---------------------------------------------------------------------------*/
USHORT SetEAFromName(PSZ pszFilePath,
					 PSZ pszEAName,
					 PSZ pszEAValue);

USHORT GetEAFromName(PSZ pszFilePath,
					 PSZ pszEAName,
					 USHORT *pwEAType,
					 PSZ    pszEAValue);

BOOL GetFileName(PSZ szFullPath);

BOOL GetEAFromPFEA(PFEA2 pFEA,
				   USHORT *pwEAType,
				   PSZ pszEAValue);

ODBoolean WriteDragItemtoFile(ODDragAndDrop *somSelf,
							  Environment *ev,
							  ODPtr theItem,
							  ODUByte *pathName,
							  ODUByte *fileName,
							  PSZ type);
// [129057]
void GiveMemoryIfNecessary(HWND hwndTarget,
						   HWND hwndSource,
						   PBYTE pbData);

PBYTE AllocSharedMem(PDRAGTRANSFER pDragTransfer,
					 ULONG dataSize);
// [140007] :
void ParseRMF(PSZ pszRMF,
			  LinkedList *fRMFList);

PSZ GetNextRMF(PSZ buf);

void AddXProductRMF(PSZ buf,
					LinkedList* fRMFList);

PSZ GetToken(PSZ start,
			 PSZ buf);

void AddRMF(PSZ buf,
			LinkedList* fRMFList);

ODBoolean SupportedRMF(PSZ theRMF);

PSZ GetKindForFormat(PSZ fmt);

PSZ GetFormatFromRMF(PSZ buf);

ODBoolean GetRMFForKind(PSZ kind,
						PSZ pszRMF);

ODBoolean QueryRMF(PSZ pszRMF,
				   PSZ mech,
				   PSZ fmt);

ODBoolean KindInTypeList(PSZ kind,
						 PSZ typeList);
// end [140007]

// [145338]
//---------------------------------------------------------------------------
// VK_SHIFT  VK_CTRL  VK_ALT  VK_CAPSLOCK  VK_NUMLOCK  VK_SCRLLOCK VK_ALTGRAF
//---------------------------------------------------------------------------
BOOL key_shift(SHORT vk_key)
{
	SHORT key_state = WinGetKeyState(HWND_DESKTOP, vk_key);
	return (BOOL)(key_state & 0x8000);
}
// end [145338]

/*---------------------------------------------------------------------------
                            ODDragAndDrop
---------------------------------------------------------------------------*/

#if ODDebug
// turn this on/off via the debugger to enable/disable logging
static BOOL fDebugLogging = kODTrue;
// this helps reduce logging of DM_DRAGOVER messages
static int counter = 0;
#endif
#ifdef SOMDEBUG
// this will turn on the MethodDebug statements in the DLL
int SOM_TraceLevel = 1;
#endif

/*---------------------------------------------------------------------------
        ODDragAndDrop::ShowPasteAsDialog
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean  SOMLINK ODDragAndDropShowPasteAsDialog(ODDragAndDrop *somSelf,
															Environment   *ev,
															ODBoolean canPasteLink,
															ODPasteAsMergeSetting mergeSetting,
															ODFacet* facet,
															ODTypeToken viewType,
															ODStorageUnit * contentSU,
															ODPasteAsResult* result)

{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropShowPasteAsDialog");

#if ODDebug
	if (fDebugLogging)
		somPrintf("in ShowPasteAsDialog\n");
#endif

	THROW_IF_NULL(facet);
	THROW_IF_NULL(contentSU);
	THROW_IF_NULL(result);

	ODBoolean returnValue = ShowPasteAsDialog(canPasteLink,
											  mergeSetting,
											  (somSelf->GetDropResult(ev) == kODDropMove),
											  contentSU,
											  facet,
											  viewType,
											  result);

	return returnValue;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::InitDragAndDrop
---------------------------------------------------------------------------*/
SOM_Scope void  SOMLINK ODDragAndDropInitDragAndDrop(ODDragAndDrop *somSelf,
													 Environment   *ev,
													 ODSession     *session)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropInitDragAndDrop");

	if (session == kODNULL)                           //D127323
		THROW(kODErrIllegalNullInput);                  //D127323

//TDF	if (somSelf->IsInitialized(ev))
//TDF		return;

	_fSession = session;

	_fWindowState      = kODNULL;		//  moved from somInit
	_fStorageSystem    = kODNULL;		//  moved from somInit
	_fDragItemList     = kODNULL;		//  moved from somInit
	_fDragGarbageList  = kODNULL;		//  moved from somInit
	_fListFromHandler  = kODFalse;		//  moved from somInit
	_fDragItemIterator = kODNULL;		//  moved from somInit
	_fAttributes       = 0;				//  moved from somInit
	_fDragReference    = 0;				//  moved from somInit
	_fDragOperation    = 0;				//  moved from somInit
	_fDragItem         = 0;				//  moved from somInit
	_fPrevFacet        = kODNULL;		//  moved from somInit
	_fSourceFrame      = kODNULL;		//  moved from somInit
	_fSourcePart       = kODNULL;		//  moved from somInit
	_fDestFacet        = kODNULL;		//  moved from somInit
	_fDragInform       = kODNULL;		//  moved from somInit
	_fNeverDrop        = kODFalse;		//  moved from somInit

	somSelf->InitObject(ev);

	TRY
		_fWindowState      = _fSession->GetWindowState(ev);
		_fStorageSystem    = _fSession->GetStorageSystem(ev);
      	_fDragItemList     = new LinkedList;
      	_fDragGarbageList  = new LinkedList;

	CATCH_ALL
		_fWindowState   = kODNULL;
		_fStorageSystem = kODNULL;
		if (_fDragItemList)
		{
			delete _fDragItemList;
			_fDragItemList = kODNULL;
		}
		if (_fDragGarbageList)
		{
			delete _fDragGarbageList;
			_fDragGarbageList = kODNULL;
		}
		RERAISE;
	ENDTRY
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::GetDragAttributes
---------------------------------------------------------------------------*/
SOM_Scope ODULong  SOMLINK ODDragAndDropGetDragAttributes(ODDragAndDrop *somSelf,
														  Environment   *ev)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragAttributes");

	return _fAttributes;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::GetDragOperation
---------------------------------------------------------------------------*/
SOM_Scope ODUShort SOMLINK ODDragAndDropGetDragOperation(ODDragAndDrop *somSelf,
														 Environment   *ev)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragOperation");

	return _fDragOperation;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::GetDragReference
---------------------------------------------------------------------------*/
SOM_Scope ODPlatformDragReference SOMLINK ODDragAndDropGetDragReference(
														ODDragAndDrop *somSelf,
														Environment   *ev)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop", "ODDragAndDropGetDragReference");

	return _fDragReference;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::FindTargetFacet

NOTE: this method is not currently called in this OS/2 implementation!
      Probably, when the OS/2 drag and drop implementation is redesigned,
      this method should be used in a similar fashion to the way it is
      used by the NT implementation, and the actual code of this method
      should follow the NT implementation's code closely, as well (the
      NT implementation of this method is actually very close to DR4).
---------------------------------------------------------------------------*/
SOM_Scope ODFacet * SOMLINK ODDragAndDropFindTargetFacet(ODDragAndDrop    *somSelf,
														 Environment      *ev,
														 ODPlatformWindow theWindow,
														 PPOINTL          mouse,
														 ODPoint          *local)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropFindTargetFacet");

	ODFacet  *targetFacet = kODNULL;
	POINTL   ptl;

	SOM_TRY
		ODFacet  *curFacet;
		{
			TempODWindow targetWindow = _fWindowState->AcquireODWindow(ev, theWindow);
			if (!targetWindow)
				return kODNULL;
		
			//  ... so how come the NT port doesn't need to map the window point here?
			ptl = *mouse;
			WinMapWindowPoints(HWND_DESKTOP,
							   WinWindowFromID(theWindow, FID_CLIENT), 
							   &ptl, 
							   1);
			local->x = MAKEFIXED(ptl.x,0);
			local->y = MAKEFIXED(ptl.y,0);
		
			curFacet = targetWindow->GetFacetUnderPoint(ev, local);
		}
		
		while (curFacet && !targetFacet)
		{
			ODFrame* curFrame = curFacet->GetFrame(ev);
			if (!curFrame->IsDragging(ev) &&
				curFrame->IsDroppable(ev) &&
				!curFrame->IsFrozen(ev))
			  targetFacet = curFacet;

			curFacet = curFacet->GetContainingFacet(ev);
		}

	SOM_CATCH_ALL
	SOM_ENDTRY

	return targetFacet;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::GetPromiseFromDragManager
---------------------------------------------------------------------------*/
SOM_Scope void  SOMLINK ODDragAndDropGetPromiseFromDragManager(
											ODDragAndDrop     *somSelf,
											Environment       *ev,
											ODPart            *sourcePart,
											ODStorageUnitView *destSUView)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetPromiseFromDragManager");

	// 127321 - check for null input
	if (destSUView == kODNULL || sourcePart == kODNULL)
	{
#if ODDebug
	  if (fDebugLogging)
		somPrintf("GetPromiseFromDragManager: Bad parameter\n");
#endif
	  return;
	}

	ODValueType type = destSUView->GetType(ev);

	(sourcePart)->FulfillPromise(ev, destSUView);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::GetDataFromDragManager
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropGetDataFromDragManager(
										ODDragAndDrop     *somSelf,
										Environment       *ev,
										ODStorageUnitView *theSUView,
										ODStorageUnit     **renderedSU)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDataFromDragManager");

	ODMemDragItem		*newItem = kODNULL;
	ODDragLink			*newLink = kODNULL;
	DRAGITEM			DragItem;
	PDRAGITEM			pDragItem;
	PDRAGTRANSFER		pDragTransfer;
	CHAR				szName[CCHMAXPATH];
	BOOL				fFile = FALSE;
	PDRAGINFO			pDragInfo = (PDRAGINFO)_fDragReference;
	ODBoolean			fSuccess = kODTrue;
	ODULong				size;
// [140007] :
	CHAR szSelectedKind[CCHMAXPATH];
	CHAR szSelectedRMF[CCHMAXPATH];
	PSZ pszSelectedKind = kODNULL;

	// whoa - this method is no good until a drop has occurred:
	if (_fDestFacet == kODNULL)
	{
		ASSERT(_fDestFacet != kODNULL, kODErrDropFacetNotKnown);
#if ODDebug
		if (fDebugLogging)
			somPrintf("GDFDM: _fDestFacet = NULL (GDFDM called too early?)\n");
#endif
		return kODFalse;
	}

#if ODDebug
	if (fDebugLogging)
		somPrintf("GDFDM: theSUView = %08p, renderedSU = %08p\n",
				  theSUView, renderedSU);
#endif

	// see if we can embed the dragged content
	ODStorageUnit* su = theSUView->GetStorageUnit(ev);
	if (!su->Exists(ev, kODPropContents, (ODValueType)kODSelectedRMF, 0))
		if (!somSelf->CanEmbed(ev, su))
			return kODFalse;
#if ODDebug
	if (fDebugLogging)
		somPrintf("GDFDM: content embeddable\n");
#endif

	// focus and get value of selected RMF
	su->Focus(ev,
			  kODPropContents,
			  kODPosUndefined,
			  (ODValueType)kODSelectedRMF,
			  0,
			  kODPosUndefined);
	size = su->GetSize(ev);
	if (!size)
		return kODFalse;
	StorageUnitGetValue(su, ev, size, szSelectedRMF);

	// focus and get value of selected kind
	if (su->Exists(ev, kODPropContents,kODSelectedKind,0))
	{
		su->Focus(ev,
				  kODPropContents,
				  kODPosUndefined,
				  (ODValueType)kODSelectedKind,
				  0,
				  kODPosUndefined);
		size = su->GetSize(ev);
		if (size)
		{
			StorageUnitGetValue(su, ev, size, szSelectedKind);
			pszSelectedKind = szSelectedKind;
		}
	}
#if ODDebug
	if (fDebugLogging)
		somPrintf("GDFDM: RMF = \"%s\", kind = \"%s\"\n",
				  szSelectedRMF, pszSelectedKind);
#endif
// [140007] end

	// allocate the storage unit we will return to caller...
	newItem = new(ODGetDefaultHeap()) ODMemDragItem(_fStorageSystem, kODFalse);
	ASSERT(newItem != kODNULL, kODErrCannotAllocateDragItem);
	newItem->Initialize(ev);
	newLink = new ODDragLink(newItem, kODTrue);
	if (!newLink)
	{
    	delete newItem;
		ASSERT(newLink != kODNULL, kODErrCannotAllocateDragItem);
		fSuccess = kODFalse;
	}
	else
	{
		// grab the dragitem from the user
    	StorageUnitViewGetValue(theSUView,
								ev,
								sizeof(DRAGITEM),
								&DragItem);
		theSUView->SetOffset(ev, sizeof(DRAGITEM));
		StorageUnitViewGetValue(theSUView,
								ev,
								sizeof(PDRAGITEM),
								&pDragItem);

		// bring over whatever changes the user has made (trust)
		memcpy(pDragItem, &DragItem, sizeof(DRAGITEM));
#if ODDebug
		if (fDebugLogging)
			somPrintf("GDFDM: user's drag item: %08p\n", pDragItem);
#endif

		//clean this up - just refer to pDragItem from now on
// [140007]:
		// if we don't handle the RMF, let the part do the rendering
		if (!SupportedRMF((PSZ)szSelectedRMF))
		{
        	newItem->fSU->AddProperty(ev, kODPropContents)->AddValue(ev,(ODValueType)szSelectedRMF);
			newItem->fSU->AddValue(ev,(ODValueType)kODDragitem);
			StorageUnitSetValue(newItem->fSU, ev,sizeof(DRAGITEM), pDragItem);
			_fDragGarbageList->AddLast(newLink);
			*renderedSU = newItem->fSU;
#if ODDebug
			if (fDebugLogging)
				somPrintf("GDFDM: unsupported RMF; returning SU (%08p) to part to render\n",
						  *renderedSU);
#endif
			return kODTrue;
		}

		// if the selected RMF is an DRM_OS2FILE then we want
		// to fill in the name info
		if (!strncmp((PCSZ)szSelectedRMF, "<DRM_OS2FILE", 12))
			fFile = TRUE;
// [140007] end

		// start the drag/drop conversation by sending the DM_RENDER;
		// initialize drag transfer structure
		HWND hwndClient = WinWindowFromID(_fDestFacet->GetWindow(ev)->GetPlatformWindow(ev),
										  FID_CLIENT);
		pDragTransfer = DrgAllocDragtransfer(1);
		pDragTransfer->cb = sizeof(DRAGTRANSFER);
		pDragTransfer->hwndClient = hwndClient;
		pDragTransfer->pditem = pDragItem;
		DrgPushDraginfo(pDragInfo, pDragInfo->hwndSource);
		pDragTransfer->hstrSelectedRMF = DrgAddStrHandle((PCSZ)szSelectedRMF);

		// set hstrRenderToName appropriately:
		// if the selected RMF is a DRM_OS2FILE ...
		if (fFile)
		{
			szName[0] = 0;
			CHAR tbuf[CCHMAXPATH];
			DrgQueryStrName(DragItem.hstrContainerName, CCHMAXPATH, szName);
			if (!szName[0])
				WinQueryActiveDesktopPathname(szName,sizeof(szName));
			if (szName[strlen(szName)-1] != '\\')
				strcat (szName, "\\");
			tbuf[0] = 0;
			DrgQueryStrName(DragItem.hstrSourceName, CCHMAXPATH, tbuf);
			if (!tbuf[0])
				DrgQueryStrName(DragItem.hstrTargetName, CCHMAXPATH, tbuf);
			if (!tbuf[0])
				sprintf(tbuf,"%d.TMP",rand());
			strcat(szName,tbuf);
			pDragTransfer->hstrRenderToName = DrgAddStrHandle(szName);
#if ODDebug
			if (fDebugLogging)
				somPrintf("GDFDM: name is %s\n", szName);
#endif
		}
		else
			pDragTransfer->hstrRenderToName = 0;

		pDragTransfer->ulTargetInfo = (ULONG)pDragInfo;			// save the drag info pointer
		pDragTransfer->usOperation = pDragInfo->usOperation;
		pDragTransfer->fsReply = 0;

		// Send DM_RENDERPREPARE here if fsReply & DC_PREPARE
		if (pDragItem->fsControl & DC_PREPARE)
			if (!DrgSendTransferMsg(pDragInfo->hwndSource,
								   DM_RENDERPREPARE,
								   (MPARAM)pDragTransfer,
								   (MPARAM)NULL))
				fSuccess = kODFalse;
#if ODDebug
		if (fDebugLogging)
		{
			somPrintf("GDFDM: sending xfer - hwndClient: %08p\n", hwndClient);
			somPrintf("GDFDM: sending xfer - DragItem.hwndItem: %08p\n", DragItem.hwndItem);
			somPrintf("GDFDM: pdragitem: %08p\n", pDragItem);
		}
#endif

		// send transfer message:
		_fRenderCompleteReceived = kODFalse;
		MRESULT mr = DrgSendTransferMsg(pDragItem->hwndItem,
										DM_RENDER,
										(MPARAM)pDragTransfer,
										(MPARAM)NULL);

		if (mr && fFile)
		{     // [140007]
			// see if the file exists; if it does, don't wait for DM_RENDERCOMPLETE
			FILESTATUS3 fInfo;
			if (DosQueryPathInfo(szName, FIL_STANDARD, &fInfo, sizeof(fInfo)) == NO_ERROR)
				mr = kODFalse;
		}
		if (mr)
		{
			// The source will render the data later on DM_RENDERCOMPLETE;
			// here is where the Merlin guys spin on the message queue
			// to accommodate the OpenDoc single thread design
#if ODDebug
			if (fDebugLogging)
				somPrintf("GDFDM: Waiting for DM_RENDERCOMPLETE\n");
#endif
			QMSG qmsg;
//			HAB hab = WinQueryAnchorBlock(_fDestFacet->GetWindow(ev)->GetPlatformWindow(ev));

			//TDF: I bet this should get reset every time ...
			//     otherwise, how do we know it has been initialized?
			//     maybe it gets set in dmRenderComplete every time ...
			_fRenderCompleteFlag = 0;

			while(!_fRenderCompleteReceived)
			{
				if (WinPeekMsg(0L, &qmsg, 0L, 0, 0, PM_NOREMOVE))
				{
					if (!WinGetMsg(0L, &qmsg, 0L, 0, 0))
						break;
					if (qmsg.msg == DM_RENDERCOMPLETE)
					{
						_fRenderCompleteFlag = DMFL_RENDEROK;
						break;
					}
					WinDispatchMsg(0L,&qmsg);
				}
			}

			if (_fRenderCompleteFlag != DMFL_RENDEROK)
			{
				fSuccess = kODFalse;
#if ODDebug
				if (fDebugLogging)
					somPrintf("GDFDM: flag != DMFL_RENDEROK\n");
#endif
			}
			else
			{
#if ODDebug
				if (fDebugLogging)
					somPrintf("GDFDM: flag == DMFL_RENDEROK\n");
#endif
			}
		}

#if ODDebug
		if (fDebugLogging)
			somPrintf("GDFDM: fSuccess: %X\n", fSuccess);
#endif
		// if render was successful, receive the data:
		if (fSuccess)
		{
#if ODDebug
			if (fDebugLogging)
				somPrintf("GDFDM: about to do receive\n");
#endif
			if (somSelf->dmDoReceive(ev, 
									 (PSZ)szSelectedRMF,
									 pDragTransfer->usOperation,
									 pDragTransfer->hstrRenderToName,
									 pDragItem->hstrType,
									 pszSelectedKind,
									 (ODPtr)newItem))
			{
#if ODDebug
				if (fDebugLogging)
					somPrintf("GDFDM: receive ok\n");
#endif
// [146814]
				if (!newItem->fSU->Exists(ev, kODPropMouseDownOffset, kODPoint, 0))
				{
					ODPoint offset;
					offset.x = MAKEFIXED(-pDragItem->cxOffset,0);
					offset.y = MAKEFIXED(-pDragItem->cyOffset,0);
					newItem->fSU->AddProperty(ev, kODPropMouseDownOffset)->AddValue(ev,(ODValueType)kODPoint);
					StorageUnitSetValue(newItem->fSU, ev,sizeof(Point), (ODValue)&offset);
				}
// [146814] end
			}
			else
			{
#if ODDebug
				if (fDebugLogging)
					somPrintf("GDFDM: receive did not work\n");
#endif
				fSuccess = kODFalse;
			}
		}

		// end the rendering conversation
		DrgSendTransferMsg(pDragItem->hwndItem,
						   DM_ENDCONVERSATION,
						   (MPARAM)pDragItem->ulItemID,
						   (fSuccess == kODTrue)
						   		? (MPARAM)DMFL_TARGETSUCCESSFUL
						   		: (MPARAM)DMFL_TARGETFAIL);

		DrgFreeDragtransfer(pDragTransfer);
	}

	// clean up
	if (fSuccess == kODTrue)
	{
    	_fDragGarbageList->AddLast(newLink);
		*renderedSU = newItem->fSU;
	}
	else
	{
    	if (newItem) delete newItem;
	}

#if ODDebug
	if (fDebugLogging)
		somPrintf("GDFDM: returning to caller\n");
#endif
	return(fSuccess);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::somUninit
---------------------------------------------------------------------------*/
SOM_Scope void  SOMLINK ODDragAndDropsomUninit(ODDragAndDrop *somSelf)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropsomUninit");

//   DosCloseEventSem(_fHEVRenderComplete);

	if (_fDragItemList)
	{
    	_fDragItemList->DeleteAllLinks();
		delete _fDragItemList;
	}
	if (_fDragGarbageList)
	{
    	_fDragGarbageList->DeleteAllLinks();
		delete _fDragGarbageList;
	}

	if (_fDragItemIterator)
    	delete _fDragItemIterator;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::Clear
---------------------------------------------------------------------------*/
SOM_Scope void  SOMLINK ODDragAndDropClear(ODDragAndDrop *somSelf,
										   Environment   *ev)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropClear");

	_fPrevFacet = kODNULL; // [pfe] 123942

#if ODDebug
	if (fDebugLogging)
		somPrintf("Clear: _fDragItemIterator = %08p, _fDragItemList = %08p, _fDragGarbageList = %08p\n",
				  _fDragItemIterator, _fDragItemList, _fDragGarbageList);

	if (getenv("STACKCRAWL_CLEAR") != NULL)
	{
		StackCrawl stackCrawl;
		strstream stack;
		stackCrawl.DumpStack(0, 20, stack);
		somPrintf("\n%s", stack.str());
		stack.rdbuf()->freeze(0);
	}
#endif

	SOM_TRY
		if (_fDragItemIterator)
		{
			delete _fDragItemIterator;
			_fDragItemIterator = kODNULL;
		}
		_fDragItemList->DeleteAllLinks();

		if (_fDragGarbageList)
			_fDragGarbageList->DeleteAllLinks();

	SOM_CATCH_ALL
	SOM_ENDTRY

	_fulItemID = 0;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::GetContentStorageUnit
---------------------------------------------------------------------------*/
SOM_Scope ODStorageUnit*  SOMLINK ODDragAndDropGetContentStorageUnit(
													ODDragAndDrop *somSelf,
													Environment   *ev)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetContentStorageUnit");

	ODStorageUnit* contentSU = kODNULL;

	SOM_TRY
		ODMemDragItem        *newItem;
		ODDragLink           *newLink;
		DRAGITEM             DragItem;
		
		newItem = new(ODGetDefaultHeap()) ODMemDragItem(_fStorageSystem, kODFalse);
		ASSERT(newItem != kODNULL, kODErrCannotAllocateDragItem);
		newItem->Initialize(ev);
		newLink = new ODDragLink(newItem, kODTrue);
		if (!newLink)
		{
			delete newItem;
			ASSERT(newLink != kODNULL, kODErrCannotAllocateDragItem);
		}
		_fDragItemList->AddLast(newLink);
		contentSU = newItem->fSU;

		memset(&DragItem, 0, sizeof(DRAGITEM));

		// Can't set this field here... it will be set in StartDrag()
		// DragItem.hwndItem = hwndSource;

		DragItem.ulItemID = _fulItemID++;
		DragItem.hstrType = DrgAddStrHandle(DRT_UNKNOWN);
		DragItem.hstrRMF =

		  // 126007 - don't show DRM_PRINT
		  DrgAddStrHandle("(DRM_SHAREDMEM,DRM_OS2FILE,DRM_DISCARD)x(DRF_OPENDOCDOCUMENT)");
//		  DrgAddStrHandle("(DRM_SHAREDMEM,DRM_OS2FILE,DRM_DISCARD,DRM_PRINT)x(DRF_OPENDOCDOCUMENT)");

		// DragItem.hstrContainerName = DrgAddStrHandle (""); // source rendering
		DragItem.hstrSourceName = DrgAddStrHandle(OPENDOC_FILENAME_DEFAULT);
		DragItem.hstrTargetName = DrgAddStrHandle(OPENDOC_FILENAME_DEFAULT);
		DragItem.cxOffset = 0;
		DragItem.cyOffset = 0;
		DragItem.fsControl = 0;
		DragItem.fsSupportedOps = DO_COPYABLE | DO_MOVEABLE;

		contentSU->AddProperty(ev, kODPropContents);
		contentSU->AddValue(ev, (ODValueType)kODDragitem);
		StorageUnitSetValue(contentSU, 
							ev,
							(ODULong) sizeof(DRAGITEM),
							&DragItem);
	SOM_CATCH_ALL
	SOM_ENDTRY

#if ODDebug
	if (fDebugLogging)
		somPrintf("GCSU: returning SU %08p\n", contentSU);
#endif
	return contentSU;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::StartDrag
---------------------------------------------------------------------------*/
SOM_Scope ODDropResult  SOMLINK ODDragAndDropStartDrag(ODDragAndDrop *somSelf,
													   Environment *ev,
													   ODFrame* srcFrame,
													   ODType imageType,
													   ODByteArray* image,
													   ODPart** destPart,
													   ODByteArray* refCon)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropStartDrag");

#if ODDebug
	// reset DM_DRAGOVER counter
	counter = 0;

	// avoid reentrancy during testing:
	static int drag_in_progress = FALSE;
	if (drag_in_progress)
	{
		somPrintf("StartDrag: reentrancy flag set at entry!\n");
		return (kODDropFail);
	}
	else
		drag_in_progress = TRUE;
#endif

	// check for bad input:
	if ((srcFrame == kODNULL) ||					// srcFrame must be != kODNULL
		(destPart == kODNULL) ||					// destPart must be != kODNULL
		((strncmp(imageType, kODOS2DragImage, strlen(kODOS2DragImage)) != 0)
		 && (imageType != kODNULL)))				// imageType must be == kODOS2DragImage or == kODNULL
													// (to support legacy OS/2 OpenDoc applications,
													// imageType can be == kODNULL; this tolerance should
													// go away in a future release!)
#if ODDebug
	{
		if (fDebugLogging)
			somPrintf("StartDrag: returning kODDropFail - parameter validation failed\n");
		drag_in_progress = FALSE;
#endif
		return(kODDropFail);
#if ODDebug
	}
#endif

	ASSERT(image != kODNULL, kODErrIllegalNullInput);
	ASSERT(refCon != kODNULL, kODErrIllegalNullInput);

	LinkedListIterator *theIter = kODNULL;
	HWND             hwndDrop   = kODNULL;       // Handle of drop window
	PDRAGINFO        pDragInfo;
	DRAGITEM         DragItem;
	PDRAGITEM        pDragItem;
	CHAR kindList[CCHMAXPATH];    // [140007]
	kindList[0] = 0;
	_fDropResult                = kODDropFail;
	_fAttributes                = 0;
	_fHasLeftSourceFrame        = kODFalse;
	_fSourceFrame               = srcFrame;
	_fDestFacet                 = kODNULL;
	_fPrevFacet                 = kODNULL;
	_fNeverDrop                 = kODFalse;

	_fSourcePart                = srcFrame->AcquirePart(ev);

	ODWindow * window           = srcFrame->AcquireWindow(ev);

	HWND hwndSource = WinWindowFromID(window->GetPlatformWindow(ev), FID_CLIENT);
#if ODDebug
	if (fDebugLogging)
		somPrintf("StartDrag: hwndSource = %08p\n", hwndSource);
#endif		

	ODReleaseObject(ev, window);

	TRY

		theIter            = new LinkedListIterator(_fDragItemList);
		
		_fDragItemIterator = new ODDragItemIterator();
		_fDragItemIterator->InitDragItemIterator(ev, theIter);
		
		ULONG cDitem = 0, index, ulTmp;
		ODDragLink *theDragLink;
		ODMemDragItem *theDragItem;
		ODStorageUnit *theSU;
		
		// Determine how many storage units we have been given
		// (In reality - there should only be one storage unit on an
		//  Opendoc initiated drag.)
		for (theDragLink = (ODDragLink*) theIter->First();
			 theDragLink != kODNULL;
			 theDragLink = (ODDragLink*) theIter->Next())
			cDitem++;              // bump count
		
		if (0 == cDitem)
		{
#if ODDebug
			if (fDebugLogging)
				somPrintf("StartDrag: nothing selected\n");
			drag_in_progress = FALSE;
#endif		
			delete theIter;
			return(_fDropResult);
		}

#if ODDebug
		if (fDebugLogging)
			somPrintf("StartDrag: count of su's: %ld\n", cDitem);
#endif

		// alloc the pm drag info block
		pDragInfo = DrgAllocDraginfo(cDitem);

		// store it in our instance data
		_fDragInform = pDragInfo;

		// retrieve our dragitems from each storage unit, and set into
		// the draginfo struct.
		for (theDragLink = (ODDragLink*) theIter->First(), index = 0;
			 theDragLink != kODNULL;
			 theDragLink = (ODDragLink*) theIter->Next(), index++)
		{
			theSU = theDragLink->fItem->fSU;

			if (theSU->Exists(ev, kODPropContents, (ODValueType)kODDragitem, 0))
			{
				theSU->Focus(ev,
							 kODPropContents,
							 kODPosUndefined,
							 (ODValueType)kODDragitem,
							 0,
							 kODPosUndefined);
				StorageUnitGetValue(theSU,
									ev,
									sizeof(DRAGITEM),
									&DragItem);
				// This is the one field we couldn't set in GetContentStorageUnit
				DragItem.hwndItem = hwndSource;

//TDF Warning!  NASTY HACK!  See notes in Constants section at top of file:
				// here's the nasty hack: if the cloneKind is Copy, change
				// the case of the TargetName string handle on the dragitem:
				ODCloneKind cloneKind = GetOriginalCloneKind(ev, theSU->GetDraft(ev));
				if (cloneKind == kODCloneCopy)
					DragItem.hstrTargetName = DrgAddStrHandle(OPENDOC_FILENAME_FOR_CLONECOPY);
//TDF:  end of NASTY HACK!
#if ODDebug
				if (fDebugLogging)
					somPrintf("StartDrag: storing DrgItem.hwndItem: %08p \n", hwndSource);
#endif
// [140007] :
				kindList[0] = 0;
				for (ODValueIndex valueIndex = 1;
					 theSU->Exists(ev, kODPropContents,(ODValueType)kODNULL,valueIndex);
					 valueIndex++)
				{
					theSU->Focus(ev,
								 kODPropContents,
								 kODPosSame,
								 (ODValueType)kODNULL,
                                 valueIndex,
								 kODPosSame);
					PSZ kind = theSU->GetType(ev);

					if (!strcmp(kind, kODDragitem)		||
						!strcmp(kind, kODDragOperation)	||
						!strcmp(kind, kODSelectedKind)	||
						!strcmp(kind, kODSelectedRMF)	||
						!strcmp(kind, kODFileType)		||
						!strcmp(kind, kODFileTypeEA))
					  continue;

					if (strlen(kindList))
					  strcat(kindList,",");

					strcat(kindList,kind);
				}
				if (strlen(kindList))
#if ODDebug
				{
					if (fDebugLogging)
						somPrintf("StartDrag: kindList is \"%s\"\n", kindList);
#endif
					DragItem.hstrType = DrgAddStrHandle(kindList);
#if ODDebug
				}
#endif

				theSU->Focus(ev,
							 kODPropContents,
							 kODPosUndefined,
							 (ODValueType)kODDragitem,
							 0,
							 kODPosUndefined);
// [140007] end
				DrgSetDragitem(pDragInfo, &DragItem, sizeof(DRAGITEM), index);

				theSU->SetOffset(ev, 0);
				StorageUnitSetValue(theSU,
									ev,
									(ODULong) sizeof(DRAGITEM),
									&DragItem);
				ulTmp = (ULONG)DrgQueryDragitemPtr(pDragInfo, index);
				theSU->SetOffset(ev, sizeof(DRAGITEM));
				StorageUnitSetValue(theSU,
									ev,
									(ODULong) sizeof(PDRAGITEM),
									&ulTmp);
            }
#if ODDebug
			else
			{
				// panic
				if (fDebugLogging)
					somPrintf("StartDrag: panic - storage unit value type kODDragitem doesn't exist\n");
            }
#endif
		}
		hwndDrop = DrgDrag(hwndSource,
						   pDragInfo,
						   (PDRAGIMAGE)image->_buffer,
						   1,
						   VK_ENDDRAG,
						   0);

#if ODDebug
		if (fDebugLogging)
			somPrintf("StartDrag: back from DrgDrag: hwndDrop == %08p\n", hwndDrop);
#endif

		// 146002 - store draginfo in our instance data again, as it can be
		// changed (zeroed) for interprocess drag and drops
		_fDragInform = pDragInfo;                 // 146002

		// Later - when we post ourselves a WM_PROCESSDROP message to get
		// off the PM win input thread, we will need to move this stuff
		// to the place where the render actually takes place and just
		// fall thru here....

		if (!hwndDrop)
		{
			DrgFreeDraginfo((PDRAGINFO)_fDragInform);
			_fDragInform = 0;
		}

		if (hwndDrop && _fDragInform)
		{
			switch (((PDRAGINFO)_fDragInform)->usOperation)
			{
				case DO_MOVE:
#if ODDebug
					if (fDebugLogging)
						somPrintf("StartDrag: A move operation (0x%04X) was requested by the drag target.\n",
								  ((PDRAGINFO)_fDragInform)->usOperation);
#endif
					_fDropResult = kODDropMove;
					break;
				
				case DO_LINK:			// [145338]
#if ODDebug
					if (fDebugLogging)
						somPrintf("StartDrag: A link operation (0x%04X) was requested by the drag target.\n",
								  ((PDRAGINFO)_fDragInform)->usOperation);
#endif
					_fDropResult = kODDropLink;
					break;
				
				default:
#if ODDebug
					if (fDebugLogging)
						somPrintf("StartDrag: A non- move, link, copy or create operation (0x%04X) was\n"
								  "           requested by the drag target - treat as a copy\n",
								  ((PDRAGINFO)_fDragInform)->usOperation);
								  
#endif
				case DO_COPY:
				case DO_CREATE:
#if ODDebug
					if (fDebugLogging)
						somPrintf("StartDrag: A copy operation (0x%04X) was requested by the drag target.\n",
								  ((PDRAGINFO)_fDragInform)->usOperation);
#endif
					_fDropResult = kODDropCopy;
					break;
			}
		}
		else
			_fDropResult = kODDropFail;

		// need to check pid of drop window against this pid,
		// set destpart to -1 if not the same, otherwise caller thinks
		// it was a nop.
		PID pid;
		TID tid;
		PTIB pptib;
		PPIB pppib;

		WinQueryWindowProcess(hwndDrop, &pid, &tid);
		DosGetInfoBlocks(&pptib, &pppib);

		//  if the pids don't match, then another process
		//  handled the drop - so I guess this code is here
		//  just to bump the reference count?  It's from DR4 ...
		if ((pid == pppib->pib_ulpid) && (_fDestFacet))
			*destPart = _fDestFacet->GetFrame(ev)->AcquirePart(ev);
		else
			*destPart = kODNULL;

#ifdef ASYNCHRONOUS_DRAGGING_SUPPORTED
// This call to DropCompleted has been #ifdef'd out because it can
// lead to refcount problems on parts.  Parts written for OS/2 don't
// expect to have both Drop and DropCompleted called, so until OS/2
// asynchronous dragging has been properly designed and implemented,
// we should not call DropCompleted on OS/2 OpenDoc parts.
#if ODDebug
		if (fDebugLogging)
			somPrintf("StartDrag: calling DropCompleted on source part...\n");
#endif
		_fSourcePart->DropCompleted(ev, *destPart, _fDropResult);
#endif // ASYNCHRONOUS_DRAGGING_SUPPORTED

		if (_fDragInform)
		{
			DrgFreeDraginfo((PDRAGINFO)_fDragInform);
			_fDragInform = kODNULL;
		}

		_fDragItem      = kODNULL;
		_fDragReference = kODNULL;
		_fDragOperation = kODNULL;
		_fSourceFrame   = kODNULL;
		ODReleaseObject(ev, _fSourcePart);
		_fDestFacet     = kODNULL;

		if (theIter)
		{
        	delete theIter;
			theIter = kODNULL;
        }

		somSelf->Clear(ev);

	CATCH_ALL

		if (theIter)
        	delete theIter;

	RERAISE;

	ENDTRY

#if ODDebug
	if (fDebugLogging)
		somPrintf("StartDrag: returning %d\n", _fDropResult);
	drag_in_progress = FALSE;
#endif		
	return _fDropResult;
}

SOM_Scope LinkedList*  SOMLINK ODDragAndDropGetDragItemList(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragItemList");

  return _fDragItemList;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDragItemList(ODDragAndDrop *somSelf, Environment *ev,
    LinkedList* list)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDragItemList");

  _fDragItemList = list;
}

SOM_Scope ODBoolean  SOMLINK ODDragAndDropGetListFromHandler(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetListFromHandler");

    return _fListFromHandler;
}

SOM_Scope void  SOMLINK ODDragAndDropSetListFromHandler(ODDragAndDrop *somSelf, Environment *ev,
    ODBoolean listFromHandler)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetListFromHandler");

  _fListFromHandler = listFromHandler;
}

SOM_Scope ODFrame*  SOMLINK ODDragAndDropGetSourceFrame(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetSourceFrame");

  return _fSourceFrame;
}

SOM_Scope void  SOMLINK ODDragAndDropSetSourceFrame(ODDragAndDrop *somSelf, Environment *ev,
    ODFrame* sourceFrame)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetSourceFrame");

  _fSourceFrame = sourceFrame;
}

SOM_Scope ODULong  SOMLINK ODDragAndDropGetAttributes(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetAttributes");

  return _fAttributes;
}

SOM_Scope void  SOMLINK ODDragAndDropSetAttributes(ODDragAndDrop *somSelf, Environment *ev,
    ODULong attr)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetAttributes");

  _fAttributes = attr;
}

SOM_Scope ODPart*  SOMLINK ODDragAndDropGetSourcePart(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetSourcePart");

  return _fSourcePart;
}

SOM_Scope void  SOMLINK ODDragAndDropSetSourcePart(ODDragAndDrop *somSelf, Environment *ev,
    ODPart* sourcePart)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetSourcePart");

  _fSourcePart = sourcePart;
}

SOM_Scope ODDragItemIterator*  SOMLINK ODDragAndDropGetDragItemIterator(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDragItemIterator");

  return _fDragItemIterator;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDragItemIterator(ODDragAndDrop *somSelf, Environment *ev,
    ODDragItemIterator* iter)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDragItemIterator");

  _fDragItemIterator = iter;
}

SOM_Scope ODSession*  SOMLINK ODDragAndDropGetSession(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetSession");

  return _fSession;
}

SOM_Scope ODStorageSystem*  SOMLINK ODDragAndDropGetStorageSystem(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetStorageSystem");

  return _fStorageSystem;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDragReference(ODDragAndDrop *somSelf, Environment *ev,
    ODPlatformDragReference dragReference)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDragReference");

  _fDragReference = dragReference;
}

SOM_Scope ODFacet*  SOMLINK ODDragAndDropGetPrevFacet(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetPrevFacet");

  return _fPrevFacet;
}

SOM_Scope void  SOMLINK ODDragAndDropSetPrevFacet(ODDragAndDrop *somSelf, Environment *ev,
    ODFacet* prevFacet)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetPrevFacet");

  _fPrevFacet = prevFacet;
}

SOM_Scope ODFacet*  SOMLINK ODDragAndDropGetDestFacet(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDestFacet");

  return _fDestFacet;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDestFacet(ODDragAndDrop *somSelf, Environment *ev,
    ODFacet* destFacet)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDestFacet");

  _fDestFacet = destFacet;
}

SOM_Scope ODDropResult  SOMLINK ODDragAndDropGetDropResult(ODDragAndDrop *somSelf, Environment *ev)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropGetDropResult");

  return _fDropResult;
}

SOM_Scope void  SOMLINK ODDragAndDropSetDropResult(ODDragAndDrop *somSelf, Environment *ev,
    ODDropResult dropResult)
{
    ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
    ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropSetDropResult");

  _fDropResult = dropResult;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::DispatchHandler
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropDispatchHandler(ODDragAndDrop *somSelf,
														 Environment   *ev,
														 ODEventData   *evt,
														 ODFacet* targetfacet)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropDispatchHandler");

	ODBoolean fHandled = FALSE;

	switch (evt->msg)
	{
    	case DM_DISCARDOBJECT:
		  fHandled = somSelf->dmDiscardObject(ev, evt);
		  break;

		case DM_DRAGERROR:
		  fHandled = somSelf->dmDragError(ev, evt);
		  break;

		case DM_DRAGLEAVE:
		  fHandled = somSelf->dmDragLeave(ev, evt);
		  break;

		case DM_DRAGOVER:
		  fHandled = somSelf->dmDragOver(ev, evt, targetfacet);   // [138899] - ced
		  break;

		case DM_DRAGOVERNOTIFY:
		  fHandled = somSelf->dmDragOverNotify(ev, evt);
		  break;

		case DM_DROP:
		  fHandled = somSelf->dmDrop(ev, evt);
		  break;

		case DM_DROPHELP:
		  fHandled = somSelf->dmDropHelp(ev, evt);
		  break;

		case DM_ENDCONVERSATION:
		  fHandled = somSelf->dmEndConversation(ev, evt);
		  break;

		case DM_PRINTOBJECT:
		  fHandled = somSelf->dmPrintObject(ev, evt);
		  break;

		case DM_RENDER:
		  fHandled = somSelf->dmRender(ev, evt);
		  break;

		case DM_RENDERCOMPLETE:
		  fHandled = somSelf->dmRenderComplete(ev, evt);
		  break;

		case DM_RENDERPREPARE:
		  fHandled = somSelf->dmRenderPrepare(ev, evt);
		  break;

		default:
		  break;
	}
   return(fHandled ? TRUE : FALSE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDiscardObject
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmDiscardObject(
                                  ODDragAndDrop *somSelf,
                                  Environment   *ev,
                                  ODEventData   *evt)
{
   ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
   ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDiscardObject");

   evt->result = (MRESULT)DRR_ABORT;
   return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDoReceive
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean  SOMLINK ODDragAndDropdmDoReceive(ODDragAndDrop *somSelf,
													  Environment *ev,
													  ODPtr    pszSelectedRMF,
													  ODUShort usOperation,
													  ODULong  hstrRenderToName,
													  ODULong  hstrType,
													  ODPtr    pszSelectedKind,
													  ODPtr    dragItem)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDoReceive");

#if ODDebug
	if (fDebugLogging)
		somPrintf("in doReceive\n");
#endif

	PBYTE pData;
	ODBoolean fSuccess = FALSE;
	CHAR  szFile[CCHMAXPATH];
	CHAR  szSubject[CCHMAXPATH];
	ULONG rc;
	HFILE  hFile;
	ULONG  ulAction, ulBytes;
	FILESTATUS3 FileInfoBuf;
	PBYTE pbData;
	PULONG pulData;
	ULONG  dataSize;
	ODMemDragItem *theDragItem = (ODMemDragItem *)dragItem;

	if (!strcmp((PSZ)pszSelectedRMF, "<DRM_SHAREDMEM,DRF_OPENDOCDOCUMENT>"))
	{
		pbData = (PBYTE) hstrRenderToName;
#if ODDebug
		if (fDebugLogging)
			somPrintf("format == pointer, address = %08p\n", pbData);
#endif

		// no need to issue DosGetSharedMem here - it was given by source
		pulData = (PULONG)pbData;
		dataSize = *pulData;

		theDragItem->Close(ev);    // [142637] - ced

		theDragItem->fContainerHandle = ODNewHandle(dataSize);

		memcpy(*((ODHandle)theDragItem->fContainerHandle),
			   pbData + sizeof(ULONG),
			   dataSize);

		DosFreeMem(pbData);

		fSuccess = kODTrue;

		TRY
			theDragItem->Open(ev);
		CATCH_ALL
			fSuccess = kODFalse;
		ENDTRY
	}
	else
		if (!strcmp((PSZ)pszSelectedRMF, "<DRM_OS2FILE,DRF_OPENDOCDOCUMENT>"))
		{
			szFile[0] = 0;
			DrgQueryStrName (hstrRenderToName, CCHMAXPATH, szFile);
#if ODDebug
			if (fDebugLogging)
				somPrintf("format = file: (%s)\n", szFile);
#endif

			rc = DosOpen(szFile,
						 &hFile,
						 &ulAction,
						 0,
						 FILE_NORMAL,
						 OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
						 OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE,
						 0);
			if (rc)
			{
#if ODDebug
				if (fDebugLogging)
					somPrintf("open failed: %d - (%s)\n", rc, szFile);
#endif
				fSuccess = FALSE;
			}
			else
			{
				DosQueryFileInfo(hFile, 1, &FileInfoBuf, sizeof(FileInfoBuf));

				theDragItem->Close(ev);           // [142637] - ced

				theDragItem->fContainerHandle = ODNewHandle(FileInfoBuf.cbFile);

				rc = DosRead(hFile,
							 *((ODHandle)theDragItem->fContainerHandle),
							 FileInfoBuf.cbFile,
							 &ulBytes);
#if ODDebug
				if (fDebugLogging)
					somPrintf("read: rc = %ld - bytes = %ld\n", rc, FileInfoBuf.cbFile);
#endif
				DosClose(hFile);

				fSuccess = kODTrue;
				TRY
					theDragItem->Open(ev);
				CATCH_ALL
					fSuccess = kODFalse;
				ENDTRY
			}
		}
		else
			if (!strncmp((PSZ)pszSelectedRMF, "<DRM_OS2FILE",12)) // [140007]
			{
				szFile[0] = 0;
				DrgQueryStrName (hstrRenderToName, CCHMAXPATH, szFile);
#if ODDebug
				if (fDebugLogging)
					somPrintf("unknown format = file: (%s)\n", szFile);
#endif

				theDragItem->fSU->AddProperty(ev, kODPropContents);
				if (pszSelectedKind)
				{
					theDragItem->fSU->AddValue(ev, (ODValueType)pszSelectedKind);
				}
				theDragItem->fSU->AddValue(ev, (ODValueType)kODFileType);
				StorageUnitSetValue(theDragItem->fSU,
									ev,
									(ODULong)strlen(szFile) + 1,
									szFile);

				szSubject[0] = 0;
				if (!GetEAFromName(szFile, ".TYPE", NULL, szSubject))
					DrgQueryStrName (hstrType, CCHMAXPATH, szSubject);
				if (szSubject[0])
				{
					theDragItem->fSU->AddValue(ev, (ODValueType)kODFileTypeEA);
					StorageUnitSetValue(theDragItem->fSU,
										ev,
										(ODULong)sizeof(szSubject),
										szSubject);
				}
				theDragItem->fSU->AddValue(ev, kODSelectedRMF);
				StorageUnitSetValue(theDragItem->fSU,
									ev,
									(ODULong)strlen((PSZ)pszSelectedRMF) + 1,
									pszSelectedRMF);
				theDragItem->fSU->AddValue(ev, (ODValueType)kODDragOperation);
				StorageUnitSetValue(theDragItem->fSU,
									ev,
									(ODULong)sizeof(USHORT),
									&usOperation);

				fSuccess = TRUE;
			}
			else
				if (!strncmp((PSZ)pszSelectedRMF, "<DRM_SHAREDMEM",14)) // [140007]
				{
					// This must be a 'part-specific' format. just add a
					// value for it.
					// NOTE: I am assuming this is an in memory transfer...
#if ODDebug
					if (fDebugLogging)
						somPrintf("special format = (%s)\n", pszSelectedRMF);
#endif

					theDragItem->fSU->AddProperty(ev, kODPropContents);
					theDragItem->fSU->AddValue(ev, (ODValueType)pszSelectedRMF);

					pbData = (PBYTE) hstrRenderToName;
#if ODDebug
					if (fDebugLogging)
						somPrintf("address = %08p\n", pbData);
#endif

					// no need to issue DosGetSharedMem here -
					// it was given by source
					pulData = (PULONG)pbData;
					dataSize = *pulData;

					StorageUnitSetValue(theDragItem->fSU,
										ev,
										(ODULong)dataSize,
										pbData + sizeof(ULONG));

					fSuccess = TRUE;
				}

	return fSuccess;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDrop
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmDrop(ODDragAndDrop *somSelf,
												Environment   *ev,
												ODEventData   *evt)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDrop");

	ODPoint      localMouse;
	POINTL       mouse;
	PDRAGINFO    pDragInfo = (PDRAGINFO)evt->mp1;
	ODFacet      *targetFacet;
	ODDropResult dropResult = kODDropFail;

#if ODDebug
	if (fDebugLogging)
		somPrintf("dmDrop (entry): hwndTarget = %08p, usOperation = 0x%08X,"
				  " _fAttributes = 0x%08X\n",
				  evt->hwnd, pDragInfo->usOperation, _fAttributes);
#endif

	mouse.x = pDragInfo->xDrop;
	mouse.y = pDragInfo->yDrop;
// [138899] begin - ced
	WinMapWindowPoints(HWND_DESKTOP, evt->hwnd, &mouse, 1);
	localMouse.x = MAKEFIXED(mouse.x,0);
	localMouse.y = MAKEFIXED(mouse.y,0);

//TDF:  hmm ... investigate re-enabling this code:
//
//	targetFacet = somSelf->FindTargetFacet(ev,
//							   WinQueryWindow(evt->hwnd, QW_PARENT),
//							   &mouse, &localMouse);
//	
//	_fDestFacet = targetFacet;
//	_fLocalMouse = localMouse;
	targetFacet = _fPrevFacet;
// [138899] end - ced

#ifdef	PORTABILITY_HACK
// OK - read long, boring 'design change' comment in header for
// the rationale behind this nasty hack:

	LinkedListIterator 			*theIter		= kODNULL;
	ODDragLink		        	*theDragLink;
	ODStorageUnit				*dragitemSU;
	ODStorageUnit				*renderedSU;
	int							count			= 0;

	//  Scan through all items in the drag and convert each one
	//  via GetDataFromDragManager
	theIter            = new LinkedListIterator(_fDragItemList);
	_fDragItemIterator = new ODDragItemIterator();
	_fDragItemIterator->InitDragItemIterator(ev, theIter);
	for (theDragLink = (ODDragLink*)theIter->First();
		 theDragLink != kODNULL;
		 theDragLink = (ODDragLink*)theIter->Next())
	{
		count++;
#if ODDebug
		if (fDebugLogging)
			somPrintf("dmDrop: drag link %d: %08p\n", count, theDragLink);
#endif

		//  alias the current storage unit
		dragitemSU = theDragLink->fItem->fSU;
#if ODDebug
		if (fDebugLogging)
			somPrintf("dmDrop: dragitemSU %d: %08p\n", count, dragitemSU);
#endif

		// now: focus on the DRAGITEM
		dragitemSU->Focus(ev,
						  kODPropContents,
						  kODPosUndefined,
						  (ODValueType)kODDragitem,
						  0,
						  kODPosUndefined);
		
		// create a view on the dragitem storage unit
		ODStorageUnitView *unitView = dragitemSU->CreateView(ev);
		
		// now get the real data (will be added to _fDragGarbageList)
		if (!somSelf->GetDataFromDragManager(ev, unitView, &renderedSU))
		{
#if ODDebug
			if (fDebugLogging)
				somPrintf("dmDrop: GDFDM returned FALSE\n");
#endif
			continue;
		}
	}
	delete theIter;

	// now, create iterator over _fDragGarbageList to let part
	// have that iterator instead of _fDragItemIterator:
	ODDragItemIterator *DragGarbageIterator;
	theIter = new LinkedListIterator(_fDragGarbageList);
	DragGarbageIterator = new ODDragItemIterator();
	DragGarbageIterator->InitDragItemIterator(ev, theIter);
#endif // PORTABILITY_HACK

#ifdef MERLIN_CODE
	// Note: this really needs to be cleaned up to mimic what Apple is doing.

	//TDF:	my interpretation of this code is as follows:
	//
	//  If the dragged content has been dropped in a draft other than the
	//  source draft, then the current state of the modifier keys completely
	//  determines the type of drop (link, move or copy).  If the dragged
	//  content has been dropped in the source draft, then the type of drop
	//  is determined by the DragInfo operation field as was received by
	//  the DM_DROP message, ignoring the current state of the modifier keys
	//  (note, however, that the modifier key states previously determined
	//  the operation that was passed to DM_DROP)

	//  first, clear any move, copy, link and paste as attributes that may be
	//  left over from when they were set during DM_DRAGOVERs ... now that we
	//  have been dropped on, we will redetermine these attributes since the
	//  drag operation is in its final state:
	_fAttributes &= ~(kODDropIsMove | kODDropIsCopy | kODDropIsLink | kODDropIsPasteAs);

	// Are we going across Drafts?
	if ((_fSourcePart == kODNULL) ||
		(_fPrevFacet->GetFrame(ev)->GetStorageUnit(ev)->GetDraft(ev) !=
		 _fSourcePart->GetStorageUnit(ev)->GetDraft(ev)))
	{
		if (key_shift(VK_SHIFT))
		{
			if (key_shift(VK_CTRL))
				_fAttributes |= kODDropIsLink;
			else
				_fAttributes |= kODDropIsMove;
		}
		else
			_fAttributes |= kODDropIsCopy;
	}
	else
	{
		switch (pDragInfo->usOperation)
		{
			case DO_LINK:
			  _fAttributes |= kODDropIsLink;
			  break;

			case DO_MOVE:
			  _fAttributes |= kODDropIsMove;
			  break;

			case DO_COPY:
			default:
			  _fAttributes |= kODDropIsCopy;
			  break;
		}
	}

	if (key_shift(VK_ALT))
	{
		_fAttributes |= kODDropIsPasteAs;
	}
#endif //MERLIN_CODE

	//  grab the target part to allow us to determine whether or not
	//  the target of the drop is the same as the source of the drop
	//  (use TempODPart to complement call to AcquirePart)
	TempODPart targetPart = targetFacet->GetFrame(ev)->AcquirePart(ev);

	//  determine the initial state of _fDropResult:
	
	//  start by assuming the _fDropResult will be kODDropMove
	somSelf->SetDropResult(ev, kODDropMove);
	
	//  if we are going across drafts, make kODDropCopy the default
	if ((_fSourcePart == kODNULL) ||
		(targetFacet->GetFrame(ev)->GetStorageUnit(ev)->GetDraft(ev) !=
		 _fSourcePart->GetStorageUnit(ev)->GetDraft(ev)))
		somSelf->SetDropResult(ev, kODDropCopy);

	//  set attributes for inspection by target:

	//  start by clearing all attributes
	_fAttributes = 0;

	//  now set the real 'Drop' attributes:
	if (targetFacet->GetFrame(ev) == _fSourceFrame)
		_fAttributes = kODDropIsInSourcePart | kODDropIsInSourceFrame;
	else
		if (targetPart == _fSourcePart)
			_fAttributes |= kODDropIsInSourcePart;

	//  interpret modifier keys
	if (key_shift(VK_ALT))							// ALT key overrides other modifiers
	  _fAttributes |= kODDropIsPasteAs;
	  //  do I also need to set drop result to Move here?
	else
	  if (key_shift(VK_CTRL))						// CTRL key indicates Copy or ...
	  {
		somSelf->SetDropResult(ev, kODDropCopy);
		if (key_shift(VK_SHIFT))					// with SHIFT key indicates Link
		  _fAttributes |= kODDropIsLink;
		else
		  _fAttributes |= kODDropIsCopy;
	  }
	  else
		if (key_shift(VK_SHIFT))					// SHIFT key alone indicates Move
		{
		  _fAttributes |= kODDropIsMove;
		  somSelf->SetDropResult(ev, kODDropMove);
		}
		else
		  //  no modifier keys pressed - use the initial state
		  //  of _fDropResult to set the initial drop attribute:
		  if (somSelf->GetDropResult(ev) == kODDropCopy)
			_fAttributes |= kODDropIsCopy;
		  else
			_fAttributes |= kODDropIsMove;

	//  check to see whether the source wants to allow only copy
	//
	//  NOTE: Apple DR4 placed this block of code immediately after
	//  the interpretation of modifier keys, which means that any
	//  attempts by the user to override a default copy operation
	//  with modifier keys will be ignored.  Although this seems
	//  overly restrictive, we have uncovered at least one situation
	//  where this logic appears to be neccessary: dragging content
	//  that is participating in a link from one part to another
	//  part in the same draft.  In this situation, the destination
	//  part must not be allowed to modify the drag from a copy to a
	//  move because that would allow Draft code to 'reparent' the
	//  content into the destination, rather than enforcing the
	//  desired copy.  This reparenting is designed to be an
	//  optimization by the draft code, but it unfortunately opens a
	//  hole in Apple's design - if content is reparented, then both
	//  the source and the destination parts 'own' the content
	//  simultaneously until the source cleans itself up after the
	//  drop.
	//  
	//  This becomes a problem when content participating in a link
	//  is dragged from a part that supports linking to a part that
	//  does not support linking: if the user is allowed to modify
	//  the drop from a copy to a move, the non-linking destination
	//  part will receive the content and update the content's link
	//  status information to indicate the content is no longer in a
	//  link-capable environment.  However, because of the
	//  reparenting that occurs as discussed above, the source still
	//  'owns' (points to) that same dropped content.  When the Drop
	//  method invoked on the destination returns and control comes
	//  back to the source via the return from the source's
	//  StartDrag call, the source will attempt to clean up its
	//  links, but it will be unable to determine if it should
	//  update any link status that concerns the content that was
	//  involved in the drag and drop because the destination will
	//  have ALREADY cleared the relevant information from the
	//  content.  The source then disregards content that should be
	//  marked as 'no longer involved in a link', and when a
	//  corresponding link is updated, the source will trap.
#ifdef OS2_IMPLEMENTATION_FOLLOWS_DR4
	if (somSelf->GetDropResult(ev) != kODDropCopy)
	{
		ODDragItemIterator* dragItemIter = somSelf->GetDragItemIterator(ev);
		if (dragItemIter)
		{
			ODStorageUnit* su = dragItemIter->First(ev);
			ODDraft* draft = su->GetDraft(ev);
			ODCloneKind cloneKind = GetOriginalCloneKind(ev, draft);
			if (cloneKind == kODCloneCopy)
			{
				somSelf->SetDropResult(ev, kODDropCopy);
				_fAttributes &= ~kODDropIsMove;
				_fAttributes |= kODDropIsCopy;
			}
		}
	}
#else
	//TDF Warning!  NASTY HACK!  See notes in Constants section at top of file:
	if (somSelf->GetDropResult(ev) != kODDropCopy)
		if (DrgAccessDraginfo(pDragInfo))
		{
			if (DrgQueryDragitemCount(pDragInfo) > 0)
			{
				PDRAGITEM pDragItem = DrgQueryDragitemPtr(pDragInfo, 0);
				if (pDragItem != NULL)
				{
					int sizeTargetName = DrgQueryStrNameLen(pDragItem->hstrTargetName) + 1;
					char *szTargetName = (char *)malloc(sizeTargetName);
					DrgQueryStrName(pDragItem->hstrTargetName, sizeTargetName, szTargetName);
					if (strncmp(szTargetName,
								OPENDOC_FILENAME_FOR_CLONECOPY,
								strlen(OPENDOC_FILENAME_FOR_CLONECOPY)) == 0)
					{
						somSelf->SetDropResult(ev, kODDropCopy);
						_fAttributes &= ~kODDropIsMove;
						_fAttributes |= kODDropIsCopy;
					}
					free(szTargetName);
				}
			}
			//DrgFreeDraginfo(pDragInfo);		//don't free drag info yet!
		}
#endif // OS2_IMPLEMENTATION_FOLLOWS_DR4

	//  cache the destination facet and call Drop on the part
	_fDestFacet = targetFacet;
	_fPrevFacet = NULL;

#if ODDebug
	if (fDebugLogging)
		somPrintf("dmDrop: about to call part's drop routine;\n"
				  "        drop result initially %d, _fAttributes = 0x%08X, targetFacet = %08p\n",
				  _fDropResult, _fAttributes, targetFacet);
#endif

#ifdef	PORTABILITY_HACK
#if ODDebug
	if (fDebugLogging)
		somPrintf("dmDrop:  passing iterator over _fDragGarbageList to part\n");
#endif // ODDebug
	// give part the iterator over the drag items set up by GetDataFromDragManager:
	dropResult  = targetFacet->Drop(ev, &localMouse, DragGarbageIterator, kODNULL);
	if (DragGarbageIterator)
		delete DragGarbageIterator;
	if (theIter)
		delete theIter;
#else
	dropResult  = targetFacet->Drop(ev, &localMouse, _fDragItemIterator, kODNULL);
#endif // PORTABILITY_HACK

#if ODDebug
	if (fDebugLogging)
		somPrintf("dmDrop: back from part's drop routine - dropResult = %d\n",
				  dropResult);
#endif

    //  update our _fDropResult - it will remain the same
    //  value as determined before the target's Drop method
    //  was called unless the target returned kODDropFail,
    //  or if the target turned a move into a copy:
	if ((dropResult == kODDropFail) ||
		((somSelf->GetDropResult(ev) == kODDropMove) &&
		 (dropResult == kODDropCopy)))
	  somSelf->SetDropResult(ev, dropResult);

	//  Set the drop operation type properly.
	//  This will return user's intentions to PM for
	//  subsequent interpretation by source's StartDrag
	switch (somSelf->GetDropResult(ev))
	{
		case kODDropMove:
		  pDragInfo->usOperation = DO_MOVE;
		  break;

		case kODDropLink:
		  pDragInfo->usOperation = DO_LINK;
		  break;

		case kODDropCopy:
		  if (_fAttributes & kODDropIsLink)
			pDragInfo->usOperation = DO_LINK;
		  else
			pDragInfo->usOperation = DO_COPY;
		  break;

		//	How do I communicate back to the source the
		//	kODDropIsPasteAs attribute?  Does the source
		//	even care or need to know?

		case kODDropFail:
		default:
			pDragInfo->usOperation = DO_UNKNOWN;
			break;
	}
	evt->result = (MRESULT)TRUE;

	// this really marks the end of the drop.
	somSelf->Clear(ev);
	_fDragReference = kODNULL; // [146749]

#if ODDebug
	if (fDebugLogging)
		somPrintf("dmDrop (exit): drop result set to %d, usOperation == 0x%08X\n",
				  _fDropResult, pDragInfo->usOperation);
#endif

	return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDragError
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmDragError(
                                  ODDragAndDrop *somSelf,
                                  Environment   *ev,
                                  ODEventData   *evt)
{
   ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
   ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragError");

   evt->result = (MRESULT)FALSE;
   return(FALSE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDragLeave
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmDragLeave(ODDragAndDrop *somSelf,
													 Environment   *ev,
													 ODEventData   *evt)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragLeave");

#if ODDebug
	if (getenv("STACKCRAWL_DL") != NULL)
	{
		StackCrawl stackCrawl;
		strstream stack;
		stackCrawl.DumpStack(0, 20, stack);
		somPrintf("\n%s", stack.str());
		stack.rdbuf()->freeze(0);
	}
#endif

	ODPoint localMouse;

// [138899] begin - ced
	POINTL mouse;
	PDRAGINFO pDragInfo = (PDRAGINFO)evt->mp1;

	mouse.x = pDragInfo->xDrop;
	mouse.y = pDragInfo->yDrop;

	WinMapWindowPoints(HWND_DESKTOP, evt->hwnd, &mouse, 1);
	localMouse.x = MAKEFIXED(mouse.x,0);
	localMouse.y = MAKEFIXED(mouse.y,0);
// [138899] end - ced

#if ODDebug
	if (fDebugLogging)
		somPrintf("dmDragLeave: hwnd = %08p, fPrevFacet = %08p\n",
				  evt->hwnd, _fPrevFacet);
#endif

	_fAttributes = 0;
	if (_fPrevFacet)
    {
		_fPrevFacet->DragLeave(ev, &localMouse, kODNULL);
		_fPrevFacet = kODNULL;
	}

	//  any given DragLeave will mean that the drag is
	//  currently leaving the source frame or must have
	//  already left the source frame on a previous
	//  DragLeave, so it is always appropriate to set
	//  this flag:
	_fHasLeftSourceFrame = kODTrue;

   // [123546] First attempt at this fix failed. We may be receiving this
   // DM_DRAGLEAVE on our way out of the docshell, towards the workplace.
   // In that case, we don't want to call Clear() as it would destroy our
   // drag information. _fSourcePart will be set in StartDrag() if we are
   // the initiator...
	if (!_fSourcePart)
    	somSelf->Clear(ev);                // 123546

	_fDragReference = kODNULL;              // [146749]

	evt->result = (MRESULT)FALSE;
#if ODDebug
	if (fDebugLogging)
		somPrintf("dmDragLeave: returning TRUE, _fSourcePart == %08p\n", _fSourcePart);
#endif
	return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDragOver
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmDragOver(ODDragAndDrop *somSelf,
													Environment   *ev,
													ODEventData   *evt,
													ODFacet* targetFacet)		// [138899] - ced
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragOver");

#if ODDebug
#define DEFAULT_SKIP 10
	int skip;
	counter++;
	if (getenv("SKIP_COUNT") != NULL)
		skip = strtoul(getenv("SKIP_COUNT"), NULL, 0);
	else
		skip = DEFAULT_SKIP;

	if (fDebugLogging)
	{
		if (counter % skip == 0)
		{
			somPrintf("dmDragOver: at entry - evt->hwnd = %08p, targetFacet = %08p\n",
					  evt->hwnd, targetFacet);
			somPrintf("dmDragOver: at entry - _fPrevFacet = %08p, _fDragItemIterator = %08p\n",
					  _fPrevFacet, _fDragItemIterator);
		}
		if (counter == 1 && (getenv("STACKCRAWL_DO") != NULL))
		{
			StackCrawl stackCrawl;
			strstream stack;
			stackCrawl.DumpStack(0, 20, stack);
			somPrintf("\n%s", stack.str());
			stack.rdbuf()->freeze(0);
		}
	}
#endif
//	ODFacet      *targetFacet;   [138899] - ced
//TDF	MRESULT      dragResult;
	ODDragResult dragResult = kODFalse;
	POINTL       mouse;
	ODPoint      localMouse;
	PDRAGINFO    pDInfo = (PDRAGINFO)(evt->mp1);
//	LinkedListIterator *theIter = kODNULL;

	DrgAccessDraginfo(pDInfo);

	_fDragReference = (ODPlatformDragReference)pDInfo;
	_fDragOperation = pDInfo->usOperation;
#if ODDebug
	if (fDebugLogging && (counter % skip == 0))
		somPrintf("dmDragOver:  _fDragReference = %08p, _fDragOperation = %d (0x%08X)\n",
				  _fDragReference, _fDragOperation, _fDragOperation);
#endif

	//  I assume this if block is here to rebuild drag items in the
	//  cross-process drag scenario ...
	if (_fDragItemIterator == kODNULL)
	{
#if ODDebug
		if (fDebugLogging)
			somPrintf("dmDragOver:  entered hwnd %08p, rebuilding drag item iterator\n",
					  evt->hwnd);
#endif
		somSelf->Clear(ev);

		ULONG cItems = DrgQueryDragitemCount(pDInfo);
#if ODDebug
		if (fDebugLogging)
			somPrintf("dmDragOver:  DrgQueryDragitemCount = %ld\n", cItems);
#endif

		for (ULONG index = 0; index < cItems; index++)
		{
			// create intermediate storage unit that will be
			// passed to GetDataFromDragManager:
			ODStorageUnit *unit = somSelf->GetContentStorageUnit(ev);
			DRAGITEM DItem;
			ULONG ulTmp;

			DrgQueryDragitem(pDInfo, sizeof(DRAGITEM), &DItem, index);

			unit->AddProperty(ev, kODPropContents);
			unit->AddValue(ev, (ODValueType)kODDragitem);
			StorageUnitSetValue(unit, ev, (ODULong)sizeof(DRAGITEM), &DItem);
			ulTmp = (ULONG)DrgQueryDragitemPtr(pDInfo, index);
			unit->SetOffset(ev, sizeof(DRAGITEM));
			StorageUnitSetValue(unit, ev, (ODULong)sizeof(PDRAGITEM), &ulTmp);
#if ODDebug
			if (fDebugLogging)
				somPrintf("dmDragOver:  storing pDragItem = %08p\n", ulTmp);
#endif
		}

		LinkedListIterator *theIter = kODNULL;
		theIter            = new LinkedListIterator(_fDragItemList);

		_fDragItemIterator = new ODDragItemIterator();
		_fDragItemIterator->InitDragItemIterator(ev, theIter);
#if ODDebug
		if (fDebugLogging)
			somPrintf("dmDragOver:  new _fDragItemIterator = %08p\n", _fDragItemIterator);
#endif
	}

	mouse.x = SHORT1FROMMP(evt->mp2);
	mouse.y = SHORT2FROMMP(evt->mp2);

// [138899] begin - ced

	WinMapWindowPoints(HWND_DESKTOP, evt->hwnd, &mouse, 1);
	localMouse.x = MAKEFIXED(mouse.x,0);
	localMouse.y = MAKEFIXED(mouse.y,0);

// targetFacet = somSelf->FindTargetFacet(ev,
//                           WinQueryWindow(evt->hwnd, QW_PARENT),
//                           &mouse, &localMouse);

// [138899] end - ced

	//  since all drag attributes are determinable in every drag within,
	//  let's clear all flags and set them according to the results of
	//  the FindTargetFacet call (well, in the current OS/2 implementation,
	//  we aren't using FindTargetFacet, so we just check to see if
	//  the target facet we were passed is non-null ...)
	_fAttributes = 0;
	if (targetFacet)
	{
		TempODPart tempPart = targetFacet->GetFrame(ev)->AcquirePart(ev);
		if ((targetFacet->GetFrame(ev) == _fSourceFrame) && (_fHasLeftSourceFrame == kODFalse))
			_fAttributes |= (kODDragIsInSourcePart | kODDragIsInSourceFrame);
		else
		{
			_fHasLeftSourceFrame = kODTrue;
			if (tempPart == _fSourcePart)
				_fAttributes |= kODDragIsInSourcePart;
		}

		//  send appropriate drag messages along
		if (_fPrevFacet == targetFacet)
		{
			if (!_fNeverDrop)
				dragResult = targetFacet->DragWithin(ev, 
													 &localMouse,
													 _fDragItemIterator,
													 kODNULL);
		}
		else
		{
			if (_fPrevFacet)
				_fPrevFacet->DragLeave(ev, &localMouse, kODNULL);

			dragResult = targetFacet->DragEnter(ev,
												&localMouse,
												_fDragItemIterator,
												kODNULL);
		}

#if ODDebug
		if (fDebugLogging && (counter % skip == 0))
			somPrintf("dmDragOver:  dragResult = %d, _fAttributes = 0x%08X\n", dragResult, _fAttributes);
#endif
		//  if the facet cannot accept the drag ...
		if (!dragResult)
		{
			//  would there be a better operation to indicate here than DO_UNKNOWN?
			evt->result = MRFROM2SHORT(DOR_NODROP, DO_UNKNOWN);
			_fNeverDrop = kODTrue;
#if ODDebug
			if (fDebugLogging)
				somPrintf("dmDragOver:  _fNeverDrop set to kODTrue, evt->result = 0x%08X\n", evt->result);
#endif
		}
		else
		{
			//  so what happens here?  do I need to check modifier keys like NT does?
			//  or check to see if I am still in the source part?
#if ODDebug
			if (getenv("DO_COPY") != NULL)
				evt->result = MRFROM2SHORT(DOR_DROP, DO_COPY);
			else
#endif
				evt->result = MRFROM2SHORT(DOR_DROP, DO_MOVE);
			_fNeverDrop = kODFalse;
#if ODDebug
			if (fDebugLogging && (counter % skip == 0))
				somPrintf("dmDragOver:  _fNeverDrop set to kODFalse, evt->result = 0x%08X\n", evt->result);
#endif
		}
	}
	else	// !targetFacet
	{
		//  We weren't over any facet; therefore, we
		//  must have left our source frame and part ...
		_fHasLeftSourceFrame = kODTrue;
		_fAttributes &= ~(kODDragIsInSourceFrame | kODDragIsInSourcePart);

		if (_fPrevFacet)
			_fPrevFacet->DragLeave(ev, &localMouse, kODNULL);
	}

	_fPrevFacet = targetFacet;

	return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDragOverNotify
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmDragOverNotify(
                                  ODDragAndDrop *somSelf,
                                  Environment   *ev,
                                  ODEventData   *evt)
{
   ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
   ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmDragOverNotify");

#if ODDebug
        if (fDebugLogging)
                somPrintf("drag over notify\n");
#endif

   evt->result = (MRESULT)FALSE;
   return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmEndConversation
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmEndConversation(ODDragAndDrop *somSelf,
														   Environment   *ev,
														   ODEventData   *evt)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropDragdmEndConversation");

	unsigned long flags = (unsigned long)evt->mp2;

#if ODDebug
	if (fDebugLogging)
		somPrintf("EndConversation\n");
#endif

	if (_fDragInform && (flags & DMFL_TARGETFAIL))
    {
#if ODDebug
		if (fDebugLogging)
			somPrintf("TARGETFAIL\n");
#endif

		DrgFreeDraginfo((PDRAGINFO)_fDragInform);
		_fDragInform = kODNULL;
	}

	evt->result = (MRESULT)FALSE;
	return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmDropHelp
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmDropHelp(
                                  ODDragAndDrop *somSelf,
                                  Environment   *ev,
                                  ODEventData   *evt)
{
   ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
   ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropDragdmDropHelp");

#if ODDebug
        if (fDebugLogging)
                somPrintf("DropHelp\n");
#endif

   evt->result = (MRESULT)FALSE;
//TDF   return(TRUE);
   return(FALSE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmPrintObject
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmPrintObject(
                                  ODDragAndDrop *somSelf,
                                  Environment   *ev,
                                  ODEventData   *evt)
{
   ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
   ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmPrintObject");

#if ODDebug
        if (fDebugLogging)
                somPrintf("PrintObject\n");
#endif

   evt->result = (MRESULT)DRR_ABORT;
   return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmRender
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmRender(ODDragAndDrop *somSelf,
												  Environment   *ev,
												  ODEventData   *evt)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmRender");

	HFILE hFile;
	ULONG ulAction;
	CHAR szFile[CCHMAXPATH];
	CHAR szSubject[CCHMAXPATH];
	USHORT usOperation;
	BOOL fSuccess = FALSE;
	PBYTE pData;
	ULONG cbSize;
	ODDragLink         *theDragLink;
	ODMemDragItem      *theDragItem;
	ULONG dataSize, rc, ulBytes;
	PBYTE pbData, pbLocalData;
	PULONG pulData;

#if ODDebug
	if (fDebugLogging)
		somPrintf("dmRender: entry\n");
#endif

	//------------------------------------------------------------------------
	// Tell the workplace there will not be a RENDERCOMPLETE to follow.
	//------------------------------------------------------------------------
	evt->result = (MRESULT)kODFalse;

	//------------------------------------------------------------------------
	// First make sure we are not being lied to
	//------------------------------------------------------------------------
	PDRAGTRANSFER pDragTransfer = (PDRAGTRANSFER)evt->mp1;
	if (pDragTransfer == kODNULL)
	{
#if ODDebug
		if (fDebugLogging)
			somPrintf("dmRender: pDragTransfer is null in dmRender\n");
#endif
		return(kODTrue);
	}

	//------------------------------------------------------------------------
	// Use the ulItemID to determine the requested storage unit
	//------------------------------------------------------------------------
	PDRAGITEM pDragItem = pDragTransfer->pditem;
	ULONG ulItemID      = pDragItem->ulItemID;
	
	LinkedListIterator *theIter = kODNULL;
	theIter            = new LinkedListIterator(_fDragItemList);
	
	theDragItem = (ODMemDragItem *)kODNULL;
	for (theDragLink = (ODDragLink*) theIter->First();
         theDragLink != kODNULL;
         theDragLink = (ODDragLink*) theIter->Next())
	{
		if (ulItemID-- == 0)
			theDragItem = (ODMemDragItem *)theDragLink->fItem;
	}
	delete theIter;

	if (theDragItem == kODNULL)
	{
#if ODDebug
		if (fDebugLogging)
			somPrintf("dmRender: theDragItem not assigned in dmRender\n");
#endif
		pDragTransfer->fsReply |= DMFL_RENDERFAIL;
		return(kODTrue);
	}

	//------------------------------------------------------------------------
	// Let's see how we are being asked to render...
	//------------------------------------------------------------------------
	CHAR  szSelectedRMF[CCHMAXPATH];
	DrgQueryStrName (pDragTransfer->hstrSelectedRMF, CCHMAXPATH, szSelectedRMF);
#if ODDebug
	if (fDebugLogging)
		somPrintf("dmRender: RMF = (%s)\n", szSelectedRMF);
#endif

	// This is a temporary hack. Currently the workplace always asks for
	// <OS2_FILE,UNKNOWN>, even if we had shown opendocdocdocument.
	if (!strcmp(szSelectedRMF, "<DRM_OS2FILE,DRF_UNKNOWN>"))
	{
    	CHAR szTmp[CCHMAXPATH];
		DrgQueryStrName(pDragItem->hstrRMF, CCHMAXPATH, szTmp);
		if (strstr(szTmp, "DRF_OPENDOCDOCUMENT"))
			strcpy(szSelectedRMF, "<DRM_OS2FILE,DRF_OPENDOCDOCUMENT>");
	}

	//------------------------------------------------------------------------
	// Test 1: Check if its an Opendoc Document to disk
	//------------------------------------------------------------------------
	if (!strcmp(szSelectedRMF, "<DRM_OS2FILE,DRF_OPENDOCDOCUMENT>"))
	{
		DrgQueryStrName(pDragTransfer->hstrRenderToName,
						CCHMAXPATH, szFile);

#if ODDebug
		if (fDebugLogging)
			somPrintf("dmRender: szFile: (%s)\n", szFile);
#endif
		if (szFile[0] != '\x0' && strlen(szFile))
		{
// [140007] :
			CHAR kindList[CCHMAXPATH];
			kindList[0] = 0;
			DrgQueryStrName(pDragItem->hstrType, CCHMAXPATH, kindList);
			theDragItem->fSU->ResolveAllPromises(ev); // [146343]
			WriteDragItemtoFile(somSelf,
								ev,
								(ODPtr)theDragItem,
								0,
								(ODUByte *)szFile,
								kindList);
// end [140007]
#if ODDebug
			if (fDebugLogging)
				somPrintf("dmRender: back from writedragtofile\n");
#endif

			pDragTransfer->fsReply |= (DMFL_NATIVERENDER | DMFL_RENDEROK);
		}
		else
		{
#if ODDebug
			if (fDebugLogging)
				somPrintf("dmRender: no file specified\n");
#endif
			pDragTransfer->fsReply |= DMFL_RENDERFAIL;
		}
	}
	//------------------------------------------------------------------------
	// Test 2: Check for Opendoc doc in shared memory....
	//------------------------------------------------------------------------
	else
		if (!strcmp(szSelectedRMF, "<DRM_SHAREDMEM,DRF_OPENDOCDOCUMENT>"))
		{
			theDragItem->fSU->ResolveAllPromises(ev);   // [146373]
			theDragItem->Close(ev);

			dataSize = ODGetHandleSize(theDragItem->fContainerHandle);
			ODLockHandle(theDragItem->fContainerHandle);
			pbLocalData = (PBYTE)*((ODHandle)theDragItem->fContainerHandle);

			if (pbData = AllocSharedMem(pDragTransfer, dataSize))
			{
				memcpy(((PBYTE)pbData + sizeof(ULONG)), pbLocalData, dataSize);

				GiveMemoryIfNecessary(pDragTransfer->hwndClient,
									  evt->hwnd,
									  pbData);
			}

//TDF:  Ja, you bet ...
//TDF:			// maybe try reopen here instead...
//TDF: 			ODDisposeHandle(theDragItem->fContainerHandle);
//TDF: 			theDragItem->fContainerHandle = (ODHandle) kODNULL;
			ODUnlockHandle(theDragItem->fContainerHandle);
			theDragItem->Open(ev);
		}
		//------------------------------------------------------------------------
		// Test 3: Next check if the RMF is present as a value.
		//------------------------------------------------------------------------
		else
		{
			// [146373] do RMF-to-kind mapping
			PSZ theKind = szSelectedRMF;
			if (!strncmp(szSelectedRMF, "<DRM_OS2FILE", 11))
			{
				PSZ theFmt = GetFormatFromRMF(szSelectedRMF);
				if (theFmt && strcmp(theFmt,"DRF_UNKNOWN"))
				{
					theKind = GetKindForFormat(theFmt);
					if (!theKind)
						theKind = szSelectedRMF;
				}
			}
			// [146373] end
			if (theDragItem->fSU->Exists(ev,
										 kODPropContents,
										 (ODValueType)theKind,
										 0))
			{
				theDragItem->fSU->Focus(ev,
										kODPropContents,
										kODPosUndefined,
										(ODValueType)theKind,
										0,
										kODPosUndefined);

         //------------------------------------------------------------------
         // Test 4: If its not a promise, just render it
         //------------------------------------------------------------------
				if (! theDragItem->fSU->IsPromiseValue(ev))
				{
		 //------------------------------------------------------------------
		 // Test 5: Check for shared memory
         //------------------------------------------------------------------
					if (!strncmp(szSelectedRMF, "<DRM_SHAREDMEM", 14))
					{
						dataSize = theDragItem->fSU->GetSize(ev);

						if (pbData = AllocSharedMem(pDragTransfer, dataSize))
						{
							StorageUnitGetValue(theDragItem->fSU,
												ev,
												dataSize,
												((PBYTE)pbData + sizeof(ULONG)));

							GiveMemoryIfNecessary(pDragTransfer->hwndClient,
												  evt->hwnd,
												  pbData);
						}

						ODDisposeHandle(theDragItem->fContainerHandle);
						theDragItem->fContainerHandle = (ODHandle) kODNULL;
					}
        //------------------------------------------------------------------
        // Test 6: Check for file
        //------------------------------------------------------------------
					else
						if (!strncmp(szSelectedRMF, "<DRM_OS2FILE", 11))
						{
							DrgQueryStrName(pDragTransfer->hstrRenderToName,
											CCHMAXPATH,
											szFile);

							// we'll write the file to disk

							dataSize = theDragItem->fSU->GetSize(ev);
							rc = DosAllocMem((PPVOID)&pbData,
											 dataSize,
											 PAG_READ | PAG_WRITE | PAG_COMMIT);
							if (rc)
							{
								pDragTransfer->fsReply |= DMFL_RENDERFAIL;
#if ODDebug
								if (fDebugLogging)
									somPrintf("dmRender: Out of memory.\n");
#endif
							}
							else
							{
								StorageUnitGetValue(theDragItem->fSU,
													ev,
													dataSize,
													pbData);

								rc = DosOpen(szFile,
											 &hFile,
											 &ulAction,
											 0,
											 FILE_NORMAL,
											 OPEN_ACTION_CREATE_IF_NEW |
											  OPEN_ACTION_REPLACE_IF_EXISTS,
											 OPEN_SHARE_DENYNONE |
											  OPEN_ACCESS_READWRITE,
											 0);
								if (rc)
								{
									pDragTransfer->fsReply |= DMFL_RENDERFAIL;
#if ODDebug
									if (fDebugLogging)
										somPrintf("dmRender: Cannot open file (%s).\n", szFile);
#endif
								}
								else
								{
									DosWrite(hFile, (PVOID)pbData, dataSize, &ulBytes);
									DosClose(hFile);
									pDragTransfer->fsReply |= (DMFL_NATIVERENDER | DMFL_RENDEROK);
								}
								DosFreeMem(pbData);
							}
						}
						else // error
						{
#if ODDebug
							if (fDebugLogging)
								somPrintf("dmRender: Don't know how to handle format.\n");
#endif
						}
				}
         //------------------------------------------------------------------
         // It's a promise - check for shared mem vs disk
         //------------------------------------------------------------------
				else
				{
            //------------------------------------------------------------------
            // Test 7: Check for shared memory
            //------------------------------------------------------------------
					if (!strncmp(szSelectedRMF, "<DRM_SHAREDMEM", 14))
					{
						ODByteArray ba;  // [146373]
						ODPart *sourcePart;
						theDragItem->fSU->GetPromiseValue(ev,
														  (ODValueType)theKind,
														  0,
														  0,
														  &ba,
														  &sourcePart);

						// Note: this differs from below (promise to file) in that
						// both we and the promiser are allocating memory for this
						// transfer.
						if (sourcePart)    // [146373]
						{
							ODStorageUnitView *destSUView = theDragItem->fSU->CreateView(ev);
							sourcePart->FulfillPromise(ev, destSUView);
							dataSize = destSUView->GetSize(ev);

//							dataSize = ba->_length;

							if (pbData = AllocSharedMem(pDragTransfer, dataSize))
							{
								StorageUnitViewGetValue(destSUView,
														ev,
														dataSize,
														(ODValue)pbData);
//								memcpy(((PBYTE)pbData + sizeof(ULONG)),
//									   ba->_buffer,
//									   dataSize);

								GiveMemoryIfNecessary(pDragTransfer->hwndClient,
													  evt->hwnd,
													  pbData);
							}
						}   // [146373] end
						ODDisposeHandle(theDragItem->fContainerHandle);
						theDragItem->fContainerHandle = (ODHandle) kODNULL;

//						delete ba;
					}
            //------------------------------------------------------------------
            // Test 8: Check for file
            //------------------------------------------------------------------
					else
						if (!strncmp(szSelectedRMF, "<DRM_OS2FILE", 11))
						{
							DrgQueryStrName(pDragTransfer->hstrRenderToName,
											CCHMAXPATH,
											szFile);

							// let the promiser write the file to disk, tell him
							theDragItem->fSU->AddProperty(ev, kODPropContents);
							theDragItem->fSU->AddValue(ev, (ODValueType)kODFileType);
							StorageUnitSetValue(theDragItem->fSU, ev, (ODULong) strlen(szFile) + 1, szFile);

							if (GetEAFromName(szFile, ".TYPE", NULL, szSubject))
							{
								theDragItem->fSU->AddValue(ev, (ODValueType)kODFileTypeEA);
								StorageUnitSetValue(theDragItem->fSU, ev, (ODULong) sizeof(szSubject), szSubject);
							}

							theDragItem->fSU->AddValue(ev, (ODValueType)kODDragOperation);
							StorageUnitSetValue(theDragItem->fSU,
												ev,
												(ODULong) sizeof(USHORT),
												&usOperation);

							ODByteArray ba;      // [146373]
							ODPart *sourcePart;
							theDragItem->fSU->GetPromiseValue(ev,
															  (ODValueType)theKind,
															  0,
															  0,
															  &ba,
															  &sourcePart);
							if (sourcePart)   // [146373]
							{
								ODStorageUnitView *destSUView = theDragItem->fSU->CreateView(ev);
								sourcePart->FulfillPromise(ev, destSUView);
							}
//							delete ba;
							// [146373] end
							ODDisposeHandle(theDragItem->fContainerHandle);
							theDragItem->fContainerHandle = (ODHandle) kODNULL;
						}
						else // error
						{
#if ODDebug
							if (fDebugLogging)
								somPrintf("dmRender: Don't know how to handle format.\n");
#endif
						}
				} // promise
			}
		}
	return(kODTrue);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmRenderComplete

        Because of the fact that GetDataFromDragManager is a synchronous
        thing and that when we fall out from it we return immediately to
        the part, we need to use a pm semaphore to wait for the source
        to render.
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmRenderComplete(
										ODDragAndDrop *somSelf,
										Environment   *ev,
										ODEventData   *evt)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmComplete");

//   ULONG  ulWaitingTime;
	PDRAGTRANSFER pDragTransfer  = (PDRAGTRANSFER)evt->mp1;

   // need to get this flag back to doReceive...
	_fRenderCompleteFlag         = SHORT1FROMMP(evt->mp2);

#if ODDebug
	if (fDebugLogging)
		somPrintf("dmRenderComplete: pDragTransfer = %08p, _fRenderCompleteFlag = 0x%08X\n",
				  pDragTransfer, _fRenderCompleteFlag);
#endif

// [140007]
//   DosResetEventSem(_fHEVRenderComplete, &ulWaitingTime);
//   DosPostEventSem(_fHEVRenderComplete);
	_fRenderCompleteReceived = kODTrue;

	evt->result = (MRESULT)kODTrue;
	return(TRUE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::dmRenderPrepare
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODDragAndDropdmRenderPrepare(
                                  ODDragAndDrop *somSelf,
                                  Environment   *ev,
                                  ODEventData   *evt)
{
   ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
   ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropdmRenderPrepare");

#if ODDebug
        if (fDebugLogging)
                somPrintf("RenderPrepare\n");
#endif

   evt->result = (MRESULT)FALSE;
   return(FALSE);
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::CanEmbed
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean  SOMLINK ODDragAndDropCanEmbed(ODDragAndDrop *somSelf,
												   Environment   *ev,
												   ODStorageUnit* dropSU)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropCanEmbed");

	DRAGITEM DragItem;
	CHAR szRMF[CCHMAXPATH];
	CHAR selectedRMF[CCHMAXPATH];
	CHAR selectedKind[CCHMAXPATH];
	CHAR szType[CCHMAXPATH];
	ODBoolean result = kODTrue;
	selectedRMF[0] = 0;
	selectedKind[0] = 0;

	if (!dropSU->Exists(ev, kODPropContents, (ODValueType)kODDragitem, 0))
		return kODFalse;
	dropSU->Focus(ev,
				  kODPropContents,
				  kODPosUndefined,
				  (ODValueType)kODDragitem,
				  0,
				  kODPosUndefined);
	StorageUnitGetValue(dropSU, ev,sizeof(DRAGITEM), &DragItem);
	if (DrgVerifyRMF(&DragItem, "DRM_SHAREDMEM", "DRF_OPENDOCDOCUMENT"))
		strcpy(selectedRMF, "<DRM_SHAREDMEM,DRF_OPENDOCDOCUMENT>");
	else
		if (DrgVerifyRMF(&DragItem, "DRM_OS2FILE", "DRF_OPENDOCDOCUMENT"))
        	strcpy(selectedRMF, "<DRM_OS2FILE,DRF_OPENDOCDOCUMENT>");
		else
		{
        	szRMF[0] = 0;
			DrgQueryStrName(DragItem.hstrRMF, CCHMAXPATH, szRMF);
			DrgQueryStrName(DragItem.hstrType, CCHMAXPATH, szType);

			if (!szRMF[0])
				return kODFalse;
			LinkedList* fRMFList     = new LinkedList;
			ParseRMF(szRMF,fRMFList);
			LinkedListIterator *theIter = new LinkedListIterator(fRMFList);
			if (!theIter)
			{
				delete fRMFList;
				return kODFalse;
			}
			RegistryManager* registrationMgr;
			registrationMgr = new RegistryManager;
			ODPartHandlerRegistry* odRegistry;
			odRegistry = registrationMgr->GetODPartHandlerRegistry(ev);
			PSZ theRMF;
			PSZ theFmt;
			PSZ theKind;
			result = kODFalse;
			ODDragRMF* item;
			for (item = (ODDragRMF *)theIter->First();
				  item != kODNULL;
				  item = (ODDragRMF *)theIter->Next())
			{
				theRMF = item->fRMF;
				theKind = theRMF;
				theFmt = GetFormatFromRMF(theRMF);

				if (theFmt)
				{
					if (strcmp(theFmt,"DRF_UNKNOWN"))
					{
						theKind = GetKindForFormat(theFmt);
						if (!theKind)
							theKind = theRMF;
					}
					else
						theKind = kODNULL;
				}
				_IDL_SEQUENCE_ISOString partHandlerList = 
				  (theKind)
				  	? odRegistry->GetPartHandlerList(ev, theKind, 0)
				  	: odRegistry->GetPartHandlerListForFileTypes(ev, szType);
				if (partHandlerList. _length)
				{
					result = kODTrue;
					strcpy(selectedRMF, theRMF);
					if (theKind)
						strcpy(selectedKind,theKind);
				}
				for (int i = 0; i < partHandlerList. _length; i++)
                	SOMFree(partHandlerList. _buffer[i]);

				SOMFree(partHandlerList. _buffer);
				if (result)
					break;
			}
			delete theIter;
			delete fRMFList;
		}

	if (result)
	{
		if (dropSU->Exists(ev, kODPropContents, (ODValueType)kODSelectedRMF, 0))
		{
        	dropSU->Focus(ev,
						  kODPropContents,
						  kODPosUndefined,
						  (ODValueType)kODSelectedRMF,
						  0,
						  kODPosUndefined);
			dropSU->Remove(ev);
		}
		dropSU->AddProperty(ev,kODPropContents)->AddValue(ev, kODSelectedRMF);
		StorageUnitSetValue(dropSU, ev, (ODULong) strlen(selectedRMF) + 1, selectedRMF);
		if (dropSU->Exists(ev, kODPropContents, (ODValueType)kODSelectedKind, 0))
		{
			dropSU->Focus(ev,
						  kODPropContents,
						  kODPosUndefined,
						  (ODValueType)kODSelectedKind,
						  0,
						  kODPosUndefined);
			dropSU->Remove(ev);
		}
		if (selectedKind[0])
		{
			dropSU->AddProperty(ev,kODPropContents)->AddValue(ev, kODSelectedKind);
			StorageUnitSetValue(dropSU, ev, (ODULong) strlen(selectedKind) + 1, selectedKind);
		}
	}
	return result;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::CanIncorporate
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean  SOMLINK ODDragAndDropCanIncorporate(ODDragAndDrop *somSelf,
														 Environment   *ev,
														 ODStorageUnit* dropSU,
														 ODType kind)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropCanIncorporate");

	CHAR szRMF[CCHMAXPATH];
	CHAR szKindRMF[CCHMAXPATH];
	CHAR szMech[CCHMAXPATH];
	CHAR szFmt[CCHMAXPATH];
	CHAR szKind[CCHMAXPATH];
	CHAR szType[CCHMAXPATH];
	CHAR szName[CCHMAXPATH];
	DRAGITEM DragItem;

	// OpenDoc data, check kind list from hstrType
	if (!dropSU->Exists(ev, kODPropContents, (ODValueType)kODDragitem, 0))
		return kODFalse;
	dropSU->Focus(ev,
				  kODPropContents,
				  kODPosUndefined,
				  (ODValueType)kODDragitem,
				  0,
				  kODPosUndefined);
	StorageUnitGetValue(dropSU, ev,sizeof(DRAGITEM), &DragItem);
	if (DrgVerifyRMF(&DragItem,NULL,"DRF_OPENDOCDOCUMENT"))
	{
		if (DrgVerifyRMF(&DragItem, "DRM_SHAREDMEM", "DRF_OPENDOCDOCUMENT"))
		{
			strcpy(szKindRMF, "<DRM_SHAREDMEM,DRF_OPENDOCDOCUMENT>");
            DrgQueryStrName(DragItem.hstrType, CCHMAXPATH, szType);
		}
		else
			if (DrgVerifyRMF(&DragItem, "DRM_OS2FILE", "DRF_OPENDOCDOCUMENT"))
			{
				strcpy(szKindRMF, "<DRM_OS2FILE,DRF_OPENDOCDOCUMENT>");
				szName[0] = 0;
				DrgQueryStrName(DragItem.hstrContainerName, CCHMAXPATH, szName);
				if (!szName[0])
					return kODFalse;
				if (szName[strlen(szName)-1] != '\\')
					strcat (szName, "\\");
				CHAR tbuf[CCHMAXPATH];
				tbuf[0] = 0;
				DrgQueryStrName (DragItem.hstrSourceName, CCHMAXPATH, tbuf);
				if (!tbuf[0])
					DrgQueryStrName (DragItem.hstrTargetName, CCHMAXPATH, tbuf);
				if (!tbuf[0])
					return kODFalse;
				strcat(szName,tbuf);
				if (!GetEAFromName(szName, "OD.KIND", NULL, szType))
					return kODFalse;
			}
	  	if (!KindInTypeList((PSZ)kind,szType))
			return kODFalse;
	}
	else
	{
		if (!GetRMFForKind((PSZ)kind,szKindRMF))
			strcpy(szKindRMF,(PSZ)kind);
		if (!QueryRMF(szKindRMF,szMech,szFmt))
			return kODFalse;
		if (!DrgVerifyRMF(&DragItem,szMech,szFmt))
			return kODFalse;
	}
	if (dropSU->Exists(ev, kODPropContents, (ODValueType)kODSelectedRMF, 0))
	{
		dropSU->Focus(ev,
					  kODPropContents,
					  kODPosUndefined,
					  (ODValueType)kODSelectedRMF,
					  0,
					  kODPosUndefined);
		dropSU->Remove(ev);
	}
	dropSU->AddProperty(ev,kODPropContents)->AddValue(ev, kODSelectedRMF);
	StorageUnitSetValue(dropSU, ev, (ODULong) strlen(szKindRMF) + 1, szKindRMF);
	if (dropSU->Exists(ev, kODPropContents, (ODValueType)kODSelectedKind, 0))
	{
		dropSU->Focus(ev,
					  kODPropContents,
					  kODPosUndefined,
					  (ODValueType)kODSelectedKind,
					  0,
					  kODPosUndefined);
		dropSU->Remove(ev);
	}
	dropSU->AddProperty(ev,kODPropContents)->AddValue(ev, kODSelectedKind);
	StorageUnitSetValue(dropSU, ev, (ODULong) strlen((PSZ)kind) + 1, (PSZ)kind);
	return kODTrue;
}

/*---------------------------------------------------------------------------
        ODDragAndDrop::DraftClosing
---------------------------------------------------------------------------*/
SOM_Scope void  SOMLINK ODDragAndDropDraftClosing(ODDragAndDrop *somSelf, 
												  Environment *ev, 
												  ODDraft* draft)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);
	ODDragAndDropMethodDebug("ODDragAndDrop","ODDragAndDropDraftClosing");

#if ODDebug
	if (fDebugLogging)
		somPrintf("Entering ODDragAndDropDraftClosing:\n");
#endif

	SOM_TRY

		//  currently, we don't care what comes across in the draft pointer ...
		// if (draft) {
	
		//  clear our drag items
		somSelf->Clear(ev);

	SOM_CATCH_ALL
	SOM_ENDTRY

		return;
}

/*---------------------------------------------------------------------------
                           Local Functions
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
        GetEAFromName

        Extract a specific EA from a given filename
---------------------------------------------------------------------------*/
USHORT  GetEAFromName(PSZ    pszFilePath,
                      PSZ    pszEAName,
                      USHORT *pwEAType,
                      PSZ    pszEAValue)
{
   EAOP2           stEAOPBuf;
   GEA2LIST        *pstGEA2List;
   FEA2LIST        *pstFEA2List;
   USHORT          cbMem;
   BOOL            bSuccess = TRUE;

   // To set up GEA2LIST structure allocate memory for the structure
   // plus the size of the string to get.

   cbMem = (USHORT) (sizeof (GEA2LIST) + strlen(pszEAName));
   pstGEA2List = (GEA2LIST *) malloc(cbMem);
   if (!pstGEA2List)
      return FALSE;

   // Next set the values of cbList, oNextEntryOffset, cbName, and copy
   // the string to find to szName.

   pstGEA2List->cbList = cbMem;
   pstGEA2List->list->oNextEntryOffset = 0;
   pstGEA2List->list->cbName = (BYTE) strlen(pszEAName);
   strcpy(pstGEA2List->list->szName, pszEAName);

   // Now set up the FEA2LIST structure.

   pstFEA2List = (FEA2LIST *) malloc(0x10000);
   if (!pstFEA2List)
      {
      free((CHAR *) pstGEA2List);
      return FALSE;
      }

   pstFEA2List->cbList = 0xFFFF;

   // Now set up the EAOP2 structure.

   stEAOPBuf.fpGEA2List = pstGEA2List;
   stEAOPBuf.fpFEA2List = pstFEA2List;
   stEAOPBuf.oError     = 0;

   // Now make the call to DosQueryPathInfo

   DosError(FERR_DISABLEHARDERR);

   if (DosQueryPathInfo(pszFilePath, 3L, &stEAOPBuf, sizeof (stEAOPBuf))
      || !pstFEA2List->list[0].cbValue)
      {
      DosError(FERR_ENABLEHARDERR);
      free((CHAR *) pstGEA2List);
      free((CHAR *) pstFEA2List);
      return (FALSE);
      }
   DosError(FERR_ENABLEHARDERR);

   // Use our common routine to parse the EA string.

   if (pstFEA2List->cbList > sizeof(ULONG))
      {
      bSuccess = GetEAFromPFEA(pstFEA2List->list,pwEAType,pszEAValue);
      }
   else
      {
      bSuccess = FALSE;
      }

   // Make sure allocated memory is freed.

   free((CHAR *) pstGEA2List);
   free((CHAR *) pstFEA2List);

   // Return success flag.

   return (USHORT)bSuccess;
}

/*---------------------------------------------------------------------------
        GetEAFromPFEA

        Extract string ea data from EAT_ASCII or EAT_MVMT feas...
---------------------------------------------------------------------------*/
BOOL GetEAFromPFEA(PFEA2  pFEA,
                    USHORT *pwEAType,
                    PSZ    pszEAValue)
{
   FEA2            *pstFEA2;
   FEA2VALUE       *pstFEA2Value;
   EASTASCII       *pstEASTAscii;
   char            *pszNextEAValue;
   USHORT          i;
   BOOL            bSuccess = TRUE;

   // Get the location and length of the EA value string.

   pstFEA2      = pFEA;
   pstFEA2Value = (FEA2VALUE *) (pstFEA2->szName + pstFEA2->cbName + 1);

   // If the user passed in a buffer for the EA type, return it.

   if (pwEAType)
     *pwEAType = pstFEA2Value->wEAType;

   // If the user passed in a buffer for the EA value, attempt to return it.

   if (pszEAValue)
      {
      // Check to see if this is an EAT_ASCII or an EAT_MVMT string
      // (types we can handle)

      switch (pstFEA2Value->wEAType)
         {
         // Handle EAT_ASCII EA type
         case EAT_ASCII:
            memcpy(pszEAValue,
                   pstFEA2Value->unEATypes.stEATAscii.bValue,
                   pstFEA2Value->unEATypes.stEATAscii.cbValue);
            pszEAValue[pstFEA2Value->unEATypes.stEATAscii.cbValue] = '\0';
            break;

         // Handle EAT_MVMT EA type
         case EAT_MVMT:
            pstEASTAscii = (EASTASCII *) (((char *) &(pstFEA2Value->unEATypes.stEATMvmt.cItem)) + 2);
            pszNextEAValue = pszEAValue;
            for (i = 0; i < pstFEA2Value->unEATypes.stEATMvmt.cItem; ++i)
               {
               if (pstEASTAscii->wEASType == EAT_ASCII)
                  {
                  memcpy(pszNextEAValue,
                         pstEASTAscii->bValue,
                         pstEASTAscii->cbValue);
                  pszNextEAValue[pstEASTAscii->cbValue] = ',';
                  pszNextEAValue += pstEASTAscii->cbValue + 1;
                  pstEASTAscii = (EASTASCII *) (pstEASTAscii->bValue + pstEASTAscii->cbValue);
                  }
               }
            // null terminate the EA string
            if (pszNextEAValue > pszEAValue)    // EA string in buffer
               {
               *(pszNextEAValue - 1) = '\0';
               }
            else                        // no EA string
               {
               *pszEAValue = '\0';
               bSuccess = FALSE;
               }
            break;

         // Handle default case (Unknown EA types)
         default:
            bSuccess = FALSE;
            break;
         }
      }

   // Return success flag.

   return (bSuccess);
}

/*---------------------------------------------------------------------------
        GetFileName
---------------------------------------------------------------------------*/
BOOL GetFileName(PSZ szFullPath)
{
   FILEDLG fileDialog;

   memset(&fileDialog, '\0', sizeof(FILEDLG));

   fileDialog.cbSize = sizeof(FILEDLG);
   fileDialog.fl = FDS_HELPBUTTON |
                     FDS_CENTER | FDS_SAVEAS_DIALOG | FDS_ENABLEFILELB;
   fileDialog.ulUser = 0;
   fileDialog.lReturn = 0;
   fileDialog.lSRC = 0;
   fileDialog.pszIType = NULL;
   fileDialog.papszITypeList = NULL;
   fileDialog.pszIDrive = NULL;
   fileDialog.papszIDriveList = NULL;
   fileDialog.hMod = (HMODULE)0;
   strcpy(fileDialog.szFullFile, szFullPath);
   fileDialog.papszFQFilename = NULL;
   fileDialog.ulFQFCount = 0;
   fileDialog.x = 0;
   fileDialog.y = 0;
   fileDialog.sEAType = 0;

   // Get the file

   if (!WinFileDlg(HWND_DESKTOP, HWND_DESKTOP, (PFILEDLG)&fileDialog))
      return FALSE;

   if (fileDialog.lReturn != DID_OK)
      return FALSE;

   // Copy file name and path returned into buffers

   strcpy(szFullPath, fileDialog.szFullFile);

   return TRUE;
}

/*---------------------------------------------------------------------------
        SetEAFromName
---------------------------------------------------------------------------*/
USHORT SetEAFromName(PSZ pszFilePath, PSZ pszEAName, PSZ pszEAValue)
{
   USHORT       cbValue;
   USHORT       cbMem;
   USHORT       rc;
   PFEA2LIST    pfea2l;
   PSZ          ptr, ptrEnd;
   PBYTE        ptrValue, ptrCurrent;
   EAOP2        eaop;

   ptr = pszEAValue;
   cbValue = 0;
   while (ptr && *ptr)
      {
      ptrEnd = strchr(ptr, ',');
      if (!ptrEnd)
         {
         ptrEnd = &ptr[strlen(ptr)];
         }
      cbValue += ((USHORT)(ptrEnd - ptr)) + 2 * sizeof(USHORT);
      ptr = ptrEnd;
      if (ptr && *ptr == ',')
         {
         ptr++;
         }
      }
   if (cbValue)
      {
      cbValue += 3 * sizeof(USHORT);
      }
   cbMem = (USHORT)sizeof(FEA2LIST) + strlen(pszEAName) + (USHORT)cbValue;
   pfea2l = (PFEA2LIST) malloc((ULONG)cbMem);
   if (!pfea2l)
      {
      return FALSE;
      }
   memset((PCH)pfea2l,0,cbMem);
   pfea2l->cbList = cbMem;
   pfea2l->list[0].cbName = (BYTE)strlen(pszEAName);
   pfea2l->list[0].cbValue = cbValue;
   strcpy(pfea2l->list[0].szName, pszEAName);
   if (cbValue)
      {
      ptr = pszEAValue;
      ptrValue = (PBYTE) &pfea2l->list[0].szName[ strlen(pszEAName) + 1 ];
      ptrCurrent = (PBYTE)(&(((PUSHORT)ptrValue)[3]));
         ((PUSHORT)ptrValue)[0] = EAT_MVMT;
      while (ptr && *ptr)
         {
         ptrEnd = strchr(ptr, ',');
         if (!ptrEnd)
            {
            ptrEnd = &ptr[strlen(ptr)];
            }
         ((PUSHORT)ptrCurrent)[0] = EAT_ASCII;
         ((PUSHORT)ptrCurrent)[1] = (USHORT)(ptrEnd - ptr);
         memcpy(&ptrCurrent[4], ptr, ((PUSHORT)ptrCurrent)[1]);
         ptrCurrent += ((PUSHORT)ptrCurrent)[1] + 4;
         ptr = ptrEnd;
         if (ptr && *ptr == ',')
            {
            ptr++;
            }
         ((PUSHORT)ptrValue)[2] += 1;
         }
      }

   eaop.fpGEA2List = NULL;
   eaop.fpFEA2List = pfea2l;
   eaop.oError     = 0;

   rc = (USHORT)DosSetPathInfo(pszFilePath, 2L, &eaop, sizeof(eaop), 0L);

   free((CHAR *)pfea2l);

   return (USHORT)(rc);
}

/*---------------------------------------------------------------------------
        WriteDragItemtoFile
---------------------------------------------------------------------------*/
ODBoolean WriteDragItemtoFile(ODDragAndDrop *somSelf,
							  Environment *ev,
							  ODPtr    theItem,
							  ODUByte  *pathName,
							  ODUByte  *fileName,
							  PSZ      type)
{
	ODDragAndDropData *somThis = ODDragAndDropGetData(somSelf);

	CHAR szFileName[CCHMAXPATH];
	APIRET rc;
	HFILE hFile;
	ULONG ulAction, ulBytes;
	ULONG dataSize;
	PBYTE pbData;
	ODBoolean fSuccess = kODTrue;

	ODMemDragItem *theDragItem = (ODMemDragItem *)theItem;

	if (fileName && fileName[0] != '\0')
	{
    	szFileName[0] = '\0';
		if (pathName != 0)
		{
			strcpy(szFileName, (char*)pathName);
			strcat(szFileName, "\\");
		}
		strcat(szFileName, (char*)fileName);
	}
	else
	{
    	szFileName[0] = '\0';
		if (pathName != 0)
		{
			strcpy(szFileName, (char*)pathName);
			strcat(szFileName, "\\");
		}
		if (!(fSuccess = GetFileName(szFileName)))
			return(fSuccess);
	}

	theDragItem->Close(ev);

	dataSize = ODGetHandleSize(theDragItem->fContainerHandle);
#if ODDebug
	if (fDebugLogging)
		somPrintf("WriteDragItemtoFile: size = %ld\n", dataSize);
#endif
	ODLockHandle(theDragItem->fContainerHandle);
	pbData = (PBYTE)*((ODHandle)theDragItem->fContainerHandle);

	rc = DosOpen(szFileName,
				 &hFile,
				 &ulAction,
				 0,
				 FILE_NORMAL,
                 OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,
                 OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE,
				 0);
	if (rc)
	{
#if ODDebug
		if (fDebugLogging)
			somPrintf("WriteDragItemtoFile: open failed: %d\n", rc);
#endif
		fSuccess = kODFalse;
	}
	else
	{
		DosWrite(hFile, (PVOID)pbData, dataSize, &ulBytes);
		DosClose(hFile);
		SetEAFromName(szFileName, ".TYPE", "OpenDoc Document");
		if (type && type[0])										// [140007]
			SetEAFromName(szFileName, "OD.KIND", type);
	}

//	ODDisposeHandle(theDragItem->fContainerHandle);
//	theDragItem->fContainerHandle = (ODHandle)kODNULL;

	ODUnlockHandle(theDragItem->fContainerHandle);		//TDF: commented in Merlin

	theDragItem->Open(ev);								//TDF: commented in Merlin

   return(fSuccess);
}

// [129057]
void GiveMemoryIfNecessary(HWND hwndTarget, HWND hwndSource, PBYTE pbData)
{
   PID myPid, pid;
   TID myTid, tid;

   WinQueryWindowProcess(hwndTarget, &pid, &tid);
   WinQueryWindowProcess(hwndSource, &myPid, &myTid);

   // if this is intra-process, no need to give memory
   if (myPid != pid)
      {
      DosGiveSharedMem(pbData, pid, PAG_WRITE | PAG_READ);
      // I think this is unneeded ???
      DosFreeMem(pbData);
      }
}

PBYTE AllocSharedMem(PDRAGTRANSFER pDragTransfer, ULONG dataSize)
{
   APIRET       rc;
   PBYTE        pbData;
   PULONG       pulData;

   rc = DosAllocSharedMem((PPVOID)&pbData, 0, dataSize + sizeof(ULONG),
                     PAG_READ | PAG_WRITE | PAG_COMMIT |
                     OBJ_GIVEABLE | OBJ_GETTABLE | OBJ_TILE);

   if (rc)
      {
      pbData = 0;
      pDragTransfer->fsReply |= DMFL_RENDERFAIL;
#if ODDebug
                if (fDebugLogging)
                        somPrintf("AllocSharedMem: Out of memory.\n");
#endif
      }
   else
      {
      pulData = (PULONG)pbData;
      *pulData = dataSize;
      pDragTransfer->fsReply |= (DMFL_NATIVERENDER | DMFL_RENDEROK);
      pDragTransfer->hstrRenderToName = (ULONG)pbData;
      }

   return(pbData);
}

void AddRMF(PSZ buf, LinkedList* fRMFList)
{
         if (strstr(buf,"DRF_OPENDOCDOCUMENT")) return;
         ODDragRMF* newRMF = new ODDragRMF(buf);
         fRMFList->AddLast(newRMF);
}

PSZ GetToken(PSZ start, PSZ buf)
{
   PSZ p = start;
   PSZ q = buf;
   while(*p) {
      if (*p == ',' || *p == ')') {
        break;
      }
      *q++ = *p;
      p++;
   }
   if (*p == ',') p++;
   if (q > buf) {
      *q = 0;
      return p;
   }
   else return kODNULL;
}

void AddXProductRMF(PSZ buf, LinkedList* fRMFList)
{
   CHAR mech[CCHMAXPATH],fmt[CCHMAXPATH], szRMF[CCHMAXPATH];
   PSZ p, q, r;
   p = buf;
   if (*p != '(') return;
   p++;
   q = strchr(p, '(');
   if (!q) return;
   q++;
   while(q = GetToken(q,fmt)) {
         r = p;
         while(r = GetToken(r,mech)) {
            sprintf(szRMF,"<%s,%s>",mech,fmt);
            AddRMF(szRMF,fRMFList);
         }
   }
}

PSZ GetNextRMF(PSZ buf)
{
   PSZ p = buf;
   PSZ q, pszNextRMF;

   if (*p == '<') {
      // Find the end of the RMF pair
      if (!(q = strchr(p,'>'))) return kODNULL;
   }
   else if (*p == '(') {
      // Find the end of the RMF cross product
      // A cross product has the form (mechanism,...) X (format,...)
      if (!(q = strchr(p,')'))) return kODNULL;
      if (!(q = strchr(q,'('))) return kODNULL;
      if (!(q = strchr(q,')'))) return kODNULL;
   }
   else {
      // The string must start with '<' or '('
      return kODNULL;
   }

   // Set q to the character just after the first RMF.
   // Set pszNextRMF to point to the next RMF or to the end of the string.
   pszNextRMF = ++q;
   while(*pszNextRMF)
   {
      if ((*pszNextRMF == '<') || (*pszNextRMF == '(')) break;
      pszNextRMF++;
   }

   // Insert a NULL character to terminate the first RMF string
   *q = 0;

   return pszNextRMF;
}

void ParseRMF(PSZ pszRMF, LinkedList *fRMFList)
{
   CHAR tbuf[CCHMAXPATH];
   strcpy(tbuf,pszRMF);
   PSZ p;
   while(p = GetNextRMF(tbuf)) {
      if (tbuf[0] == '<') {
         AddRMF(tbuf,fRMFList);
      }
      else AddXProductRMF(tbuf,fRMFList);
      strcpy(tbuf,p);
   }
}

ODBoolean SupportedRMF(PSZ theRMF)
{
   if (strstr(theRMF, "DRF_OPENDOCDOCUMENT")) return kODTrue;
   if (strstr(theRMF, "DRM_OS2FILE")) return kODTrue;
   if (strstr(theRMF, "DRM_SHAREDMEM")) return kODTrue;
   return kODFalse;
}

PSZ GetFormatFromRMF(PSZ buf)
{
static CHAR tbuf[CCHMAXPATH];
PSZ p;
         if (strncmp(buf,"<DRM_OS2FILE",12)) return kODNULL;
         PSZ q = strchr(buf,',');
         if (q) {
            q++;
            strcpy(tbuf,q);
            if (q = strchr(tbuf,'>')) {
               *q = 0;
               return tbuf;
            }
         }
         return kODNULL;
}

ODBoolean QueryRMF(PSZ pszRMF, PSZ mech, PSZ fmt)
{
   CHAR tbuf[CCHMAXPATH];
   if (*pszRMF != '<') return kODFalse;
   strcpy(tbuf,pszRMF);
   PSZ p = tbuf+1;
   PSZ q = strchr(p,',');
   if (!q) return kODFalse;
   *q++ = 0;
   strcpy(mech,p);
   p = strchr(q,'>');
   if (!p) return kODFalse;
   *p = 0;
   strcpy(fmt,q);
   return kODTrue;
}

ODBoolean KindInTypeList(PSZ kind, PSZ typeList)
{
	PSZ p,q;
	p = typeList;

	USHORT len = strlen(kind);
	while(p && *p)
	{
		q = strchr(p,',');
		if (!q)
			q = p + strlen(p);
		if (len == q-p && !strncmp(kind,p,len))
			return kODTrue;
		if (*q == ',')
			q++;
		p = q;
	}
	return kODFalse;
}

/*---------------------------------------------------------------------------
                             End of File
---------------------------------------------------------------------------*/
