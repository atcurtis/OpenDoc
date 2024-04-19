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
// @(#) 1.13 com/src/storage/Clipbd.cpp, oddataxfer, od96os2, odos29646d 9/20/96 16:34:56 [ 11/15/96 15:29:20 ]
/*
	File:		Clipbd.cpp

	Contains:	Implementation for ODClipboard class.

	Owned by:	Craig Carper

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<54>	 10/9/95	CC		1289794: Avoid calling GetScrap if scrap is
									in an inconsistent state.
									1288241: SetPlatformClipboard prevents
									exporting clipboard.
									1290782: Purge is not implemented.
									1286400: Preflight scrap memory
									allocations.
		<53>	 10/3/95	TJ		Changes done by RefBall Team
		<52>	 9/26/95	EL		1285709: ODClipboard does not close down
									properly if it was never accessed.
		<51>	  9/8/95	T‚		1281096 FB2:Many constants in ODTypesB
									without kOD prefix!
		<50>	 8/31/95	DM		1273863, 1275336: add ODClipboard::
									DraftSaved(), make GetOriginalDraft() honor
									standard endianess
		<49>	 8/30/95	EL		1279832: Storage THROW_IF_NULL problems.
		<48>	 8/29/95	DM		1276549: call parent Purge in ODClipboard
		<47>	 8/26/95	T‚		1274606 FB2: Patching Remarks
		<46>	 8/25/95	CC		1264154: Added ActionDone, ActionUndone,
									and ActionRedone methods.
		<45>	 8/21/95	VL		1277291: Use GetOriginalCloneKind from
									StorUtil.
		<44>	 8/16/95	NP		1274946: ErrorDef.idl problems. Add include
									file.
		<43>	 8/12/95	T‚		1276812 Need to use TempObjs and TempRefs
									for exception safety and to avoid TRY
									blocks, 1276807 Opt./Bug: use StdTypIO
									routines for portable streaming & smaller
									footprint
		<42>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<41>	 7/26/95	DM		#1270320: Memory leak fixes: dispose handle
									on return in ODClipboardImportContent
		<40>	 7/20/95	CC		1153954: ExportPlatformTypes: Don't move
									types to scrap that are not platform types.
									1260342: Clear: fail if process is in the
									background.
		<39>	  7/1/95	CC		1264197: DraftClosing must not remove
									Original Draft property.  Instead, change
									original clone kind to kODCloneCopy.
									1257374: Added ForceZeroScrap(), called
									from SetPlatformClipboard() and
									ExportClipboard() methods.
		<38>	 6/23/95	CC		1260902: ImportContent: Skip if scrap
									handle is null, and Ignore types of size
									zero.
									1257374: GetContentStorageUnit: Never issue
									debugger break on failure to import from
									the system scrap.
		<37>	 6/19/95	CC		1259079: Added local function
									IsFrontProcess.
									GetContentStorageUnit: Don't warn if
									process is in background.
		<36>	 5/26/95	VL		1251403: Multithreading naming support.
		<35>	 5/22/95	CC		1251106: CloseClipboard externalizes root
									SU to resolve promises before externalizing
									the clipboard draft.
		<34>	 5/18/95	CC		1250280: ShowPasteAsDialog throws on
									invalid null arguments.
		<33>	 5/17/95	RR		#1250135/1250137/1250143 Getters increment
									refcount
		<32>	 5/16/95	CC		1244991: ExportClipboard: Re-export
									clipboard if link spec has been removed.
									ExportClipboard: Set fOriginalDraft field.
									DiscardClipboard: Added SOM_CATCH return.
		<31>	  5/5/95	CC		1224474: ImportContent: Fixed memory leak
									due to variable theISOType
		<30>	 4/25/95	CC		1242555: PutContentOnPlatformClipboard:
									Removed obsolete $5 comment.
		<29>	 4/14/95	T‚		#1235279 BB: InfoUtil & StdTypIO functions
									should take Environment* and SU* when
									possible
		<28>	 4/11/95	CC		1231378: DraftClosing: Removed link spec if
									present.
		<27>	 3/22/95	CC		1230322: Changed parameter to
									ShowPasteAsDialog().
		<26>	 3/10/95	CC		1225050: Added DraftClosing() method.
									1227468: Clear: Set fScrapCount to ensure a
									changed clipboard is exported again.
		<25>	 2/24/95	CC		1222076: BB: Clipboard contents lost on
									error converting to Mac scrap.
									1184034:	BB: Error -108 during
									cross-document paste link.
									1186774, 1201430, 1218790:	Ignore GetScrap
									errors.
									1223020:	ODClipboard calls LoadScrap() at
									innapropriate times.
									Fixed mismatched delimiters: #include
									<ODDebug.h".
		<24>	 2/14/95	jpa		Added return statement to a SOM_CATCH
									[1215160]
		<23>	 2/10/95	CG		#1177475: Added include for StdDefs.xh.
		<22>	  2/7/95	CC		1211295: Added GetOriginalCloneKind().
									Pass isMove parameter to ShowPasteAsDialog.
									1216124: ShowPasteAsDialog aquires modal
									focus.
		<21>	  2/1/95	CC		1153802: Improvements to
									ImportStyledTextType().
									ImportContent(): Create stxt only if not
									already present.
		<20>	 1/26/95	VL		#???: Use updated Storage Unit Ref API.
		<19>	 1/25/95	CC		1153802 Conversion between 'styl' and
									'stxt' types: Added ImportStyledTextType
									and ExportStylType functions.
		<18>	 1/19/95	CC		1212419 Add parameter to
									GetMemoryContainer.
									1212833 Change to use
									kODScrapTypeODBentoContainer.
		<17>	12/20/94	VL		1195012: Make Storage calls be
									marshallable.
		<16>	 12/8/94	CC		1186774, 1201430 - Suppress errors returned
									when reading the scrap.
		<15>	 9/29/94	RA		1189812: Mods for 68K build.
		<14>	 9/23/94	VL		1155579, 1184272: Use StorUtil to
									create/get container and get its file.
		<13>	 9/19/94	CC		1187315 - somUninit() must call parent
									method last.
									1160121 - Clear() must not return an error.
		<12>	 9/15/94	CC		1186774 - Suppress GetScrap() errors.
		<11>	 8/26/94	VL		1183174: Use updated cloning APIs.
		<10>	 8/15/94	T‚		#1180922 Removed most obsolete types from
									StdTypes.idl
		 <9>	  8/3/94	CC		Removed Lock() and Unlock(); removed
									ODClipboardKey parameter from other
									methods.  (1160484)
		 <8>	  8/3/94	VL		1153123: Storage to ODStor.
		 <7>	  8/2/94	CC		#1178169 - ShowPasteAsDialog() takes
									ODFacet* instead of ODFrame*.
		 <6>	 7/31/94	CC		Change WASSERTs to WARNs.
		 <5>	 7/12/94	CC		Companion to .idl checkin -- no changes.
		 <4>	 6/28/94	VL		Used Translt.xh.
		 <3>	 6/24/94	CC		Use kODScrapTypePart constant in
									ConstDef.h.
		 <2>	 6/24/94	CC		Fleshed out ShowPasteAsDialog().
		 <1>	 6/21/94	CC		first checked in

		------------------- CC ---- Converted to SOM

		<13>	  5/9/94	MB		Changes necessary to install MMM. Bug
									#1162181.
		<12>	  4/4/94	CC		SetPlatformClipboard() &
									ExportPlatformClipboard(): Parameter
									changed from XMPTypeSet to XMPTypeList
									(1153046)
		<11>	 3/28/94	CG		1153547: Renamed XMPSessn.h to XMPSessM.h
		<10>	 3/27/94	T‚		#1153523.  Adjust name of
									kXMPPropRootPartSU to more clearly indicate
									what it is supposed to be used for.
		 <9>	 3/25/94	JA		Added missing #include (1147588)
		 <8>	 3/18/94	CC		Added counting locks to Lock() and
									Unlock(); Implementation of and calls to
									ValidateKey(). (1151853)
		 <7>	 3/16/94	CG		#1151186: Added call to InitPlatformTypeSet
									after new XMPPlatformTypeSet.
		 <6>	  3/2/94	CC		Lock() & Unlock(): Commented out erroneous
									WASSERTs. (1145487)
		 <5>	 2/28/94	VL		Changed code that may cause memory
									problems.
		 <4>	 2/22/94	VL		THROW -> THROW_IF_ERROR.
		 <3>	 2/15/94	CC		Bug #1142933 - Added key parameter to
									SetPlatformClipboard.
		 <2>	 2/15/94	CC		Bug #1142949 - Adapted to method name
									changes in XMPPlatformTypeSet.
		 <3>	  2/8/94	VL		Use new exception macros.
		 <2>	  2/4/94	VL		Session.h -> XMPSessn.h.
		 <1>	  2/4/94	VL		first checked in

		<21>	  2/4/94	VL		Moved to PPC Header and began code cleanup.
		<20>	 1/28/94	CC		Implemented change ids; replaced DebugStrs
									with WASSERTMs.
		<19>	 1/21/94	CC		Converted from XMPTimeStamp to XMPUpdateID.
									Added GetUpdateID.  GetLock() returns
									boolean result and returns key in second
									parameter.
		<18>	 1/21/94	CG		Renamed kXMPMemoryContainer to
									kXMPDefaultMemoryContainer.
		<17>	 1/18/94	CG		Added include for StorgDef.yh
		<16>	 1/18/94	CC		Use kXMPScrapTypePart; release draft
									properties on failure in NewClipboard() and
									ImportClipboard().
		<15>	 1/14/94	CC		Added type XMPClipboardKey, added key
									parameter to various routines, replaced
									GetGeneration() with GetTimeStamp, renamed
									AcquireStorageUnit() GetContentStorageUnit();
									changed from using a handle to a pointer in
									ExportPlatformTypes() and
									PutContentOnPlatformClipboard().
		<14>	 1/11/94	T‚		Init... changes
		<13>	12/21/93	VL		Changed XMPStorageUnit::GetValueSize to
									StorageUnit::GetSize.
		<12>	12/15/93	T‚		more InitObject changes, remove
									Initialize() method
		<11>	12/15/93	T‚		InitObject changes
		<10>	 12/2/93	CC		Added Lock(), Unlock(), and GetGeneration()
									methods.
		 <9>	11/10/93	CC		Doesn't externalize draft before disposing
									memory container
		 <8>	 11/1/93	VL		Used Strong reference.
		 <7>	10/29/93	RR		XMPClipboard:: -> XMPMacClipboard
		 <6>	10/27/93	CC		Added methods to transfer to/from the
									platform clipboard.
		 <5>	10/20/93	CC		Added session parameter to XMPClipboard
									constructor; added SetPlatformClipboard
									method; added code to create the clipboard
									container.
		 <3>	 4/29/93	VL		More 8.3 Name Change.
		 <2>	 4/29/93	VL		8.3 Name Change.
		 <1>	 4/27/93	VL		first checked in

	To Do:
	¥ Reuse fContainerHandle when possible after exporting it.
	¥ Add a resolvePromises parameter to ExportClipboard (but maybe we need to
		do this all the time).
	¥ Vincent will add a draft change seed.  The clipboard object can examine
		the seed for the clipboard draft to determine if the clipboard has
		been changed since it was exported (if the clipboard generation is the
		same).
	¥ XMPClipboard::Clear() should clear the system clipboard.
	
	In Progress:
		
*/

#define VARIABLE_MACROS

#define ODClipboard_Class_Source
#include <Clipbd.xih>

#ifndef SOM_ODPartHandlerRegistry_xh
#include <ODPrtReg.xih>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _CONSTDEF_
#include <ConstDef.h>
#endif
#endif

#ifndef _PLFMDEF_
#include <PlfmDef.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
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

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

//#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif
//#endif // MAC or AIX

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh 
#include <TypLsItr.xh>
#endif

#ifndef SOM_ODPlatformTypeList_xh
#include <PfTypLs.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef _LINKDLGS_
#include <LinkDlgs.h>
#endif

#ifndef SOM_RegistryManager_xh
#include <RManager.xh>
#endif 

#ifdef _PLATFORM_MACINTOSH_
#ifndef __SCRAP__
#include <Scrap.h>
#endif
#endif

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef __ERRORS__
#include <Errors.h>
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

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
#ifndef __TEXTEDIT__
#include <TextEdit.h>	// for ScrpSTElement
#endif
#endif // MAC or AIX

#ifdef _PLATFORM_OS2_
#ifndef _ORDCOLL_
#include <OrdColl.h>
#endif

#ifdef OLE_STUFF
#ifndef OLEDEFS_H
#include <OLEDefs.h>
#endif
#endif // OLE_STUFF

#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODClipboard
#endif

#include "ClipbdB.cpp"	// Platform-independent methods, if any

#if ODDebug
#define ODDebugClipboard 0
#undef _REPORT_SCRAP_ERRORS_
#else
#define ODDebugClipboard 0
#undef _REPORT_SCRAP_ERRORS_
#endif

//==============================================================================
// Constants
//==============================================================================

const short kInvalidScrapCount = -1;	// Must never match the scrap count!

// Mac Scrap state constants
const short kUninitializedScrap = -1;
const short kScrapOnDisk = 0;

#ifdef _PLATFORM_OS2_
#define kCF_OPENDOCDOCUMENT "CF_OPENDOCDOCUMENT"
#define CF_OPENDOCDOCUMENT  WinFindAtom(WinQuerySystemAtomTable(),kCF_OPENDOCDOCUMENT)
#define kCF_OPENDOCOWNERID "CF_OPENDOCOWNERID"
#define CF_OPENDOCOWNERID   WinFindAtom(WinQuerySystemAtomTable(),kCF_OPENDOCOWNERID)
/*---------------------------------------------------------------------------
                               TypeDefs
---------------------------------------------------------------------------*/
typedef struct ClipboardTypeInfo
{
   ODType               opendocType;
   ODPlatformType       platformType;
   ODULong              formatInfo;
   ODBoolean            deletable;
} CLIPBOARDTYPEINFO;

CLIPBOARDTYPEINFO clipboardTypeInfo[] =
{
   { kODKindOS2Text,       CF_TEXT,             CFI_POINTER,    kODFalse },
   { kCF_OPENDOCDOCUMENT,  CF_OPENDOCDOCUMENT,  CFI_POINTER,    kODFalse },
   { kCF_OPENDOCOWNERID,   CF_OPENDOCOWNERID,   CFI_HANDLE,     kODFalse },
   { kODKindOS2DspText,    CF_DSPTEXT,          CFI_POINTER,    kODFalse },
   { kODKindOS2Bitmap,     CF_BITMAP,           CFI_HANDLE,     kODFalse },
   { kODKindOS2DspBitmap,  CF_DSPBITMAP,        CFI_HANDLE,     kODFalse },
   { kODKindOS2Metafile,   CF_METAFILE,         CFI_HANDLE,     kODFalse },
   { kODKindOS2DspMetafile,CF_DSPMETAFILE,      CFI_HANDLE,     kODFalse },
   { kODKindOS2Palette,    CF_PALETTE,          CFI_HANDLE,     kODFalse }
};
#endif

//==============================================================================
// Function Prototypes
//==============================================================================

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
ODStatic ODULong ApplicationHeapFree();
ODStatic short GetMacScrapCount();
ODStatic ODBoolean IsFrontProcess();
ODStatic void ImportStyledTextType(ODClipboard *somSelf, Environment *ev);
ODStatic OSErr ExportStylType(ODPtr	stxtData);
#endif // MAC or AIX
ODStatic void CloseClipboard(ODClipboard *somSelf, Environment *ev, ODBoolean externalize);
ODStatic void OpenClipboard(ODClipboard *somSelf, Environment *ev);
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
ODStatic ODBoolean ScrapIsInconsistent();
ODStatic ODBoolean ScrapHasData();
ODStatic ODBoolean ScrapHasType(ODPlatformType type);
ODStatic OSErr ForceZeroScrap();
ODStatic ODBoolean ScrapMemoryAvailable(Size dataSize);
ODStatic OSErr UnloadScrapIfMemLow(Size dataSize);
ODStatic OSErr ClipboardLoadScrap();
ODStatic OSErr ClipboardPutScrap(Size dataSize, ODPlatformType	platformType, ODPtr data);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
ULONG GetScrap( ODClipboard *somSelf, Handle hData, ODPlatformType type, long * dummy );
ODStatic ODBoolean SetClipboardOwnership( ODClipboard *somSelf,
                                          Environment *ev );
ODBoolean PutPMPromise( ODClipboard *somSelf, ODPlatformType platformType );
OSErr PutScrap( ODClipboard *somSelf, ULONG dataSize,
                                ODPlatformType platformType, Ptr data);
ODStatic ODUpdateID GetPMUpdateID(HAB hab);
ODStatic ODBoolean ScrapHasData();
ODStatic void SetPMUpdateID(HAB hab, ODUpdateID id);
ULONG GetDataHandle(Environment*ev, ODStorageUnit* su);
#ifdef OLE_STUFF
void PutOleContentOnPlatformClipboard(ODStorageUnit *su);
void GetOleContentFromPlatformClipboard(ODStorageUnit *su);
#endif // OLE_STUFF
ODULong GetFormatInfo( ODClipboard *somSelf, ODPlatformType ulType  ); // [140007]
HBITMAP BitmapFromStorage(Environment* ev, ODStorageUnit* storageUnit);
HMF MetafileFromStorage(Environment* ev, ODStorageUnit* storageUnit);
#endif

//==============================================================================
// Utility Functions
//==============================================================================

//------------------------------------------------------------------------------
// SetOriginalCloneKind
//------------------------------------------------------------------------------

static void SetOriginalCloneKind(Environment* ev, ODDraft* draft, ODCloneKind cloneKind)
{
	if ( draft != kODNULL )
	{
		TempODStorageUnit draftProperties = draft->AcquireDraftProperties(ev);
		ODSetULongProp(ev, draftProperties, kODPropOriginalCloneKind, kODULong, (ODULong)cloneKind);
	}
}

//------------------------------------------------------------------------------
// GetOriginalDraft
//------------------------------------------------------------------------------
// Returns kODNULL if the original draft is unknown.  This is the case when content
// was placed in the draft without cloning.

static ODDraft* GetOriginalDraft(Environment* ev, ODDraft* draft)
{
	TempODStorageUnit	draftProperties = draft->AcquireDraftProperties(ev);
	return (ODDraft*)ODGetULongProp(ev, draftProperties, kODPropOriginalDraft, kODULong);
}

//==============================================================================
// ODClipboard
//==============================================================================

//------------------------------------------------------------------------------
// ODClipboard::somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardsomUninit(ODClipboard *somSelf)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","somUninit");
	
	Environment *ev = somGetGlobalEnvironment();
	// somSelf->DiscardClipboard(ev); 
	// It is incorrect to call methods on somSelf inside somUninit, 
	// a subclass may have already been somUninited.  See OpenDoc Building Code for details.
	// Instead the implementation of DiscardClipboard has been copied here.
	
	// BEGIN copy of DiscardClipboard implementation
	CloseClipboard(somSelf, ev, kODFalse);
	
	ODDisposeHandle(_fContainerHandle);
	_fContainerHandle = (ODHandle) kODNULL;
	// END copy of DiscardClipboard implementation
#ifdef _PLATFORM_OS2_
        delete _fTypeCollection;
#endif

	parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODClipboard::InitClipboard
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardInitClipboard(ODClipboard *somSelf, Environment *ev,
		ODSession* session)
{
    ODClipboardData *somThis = ODClipboardGetData(somSelf);
    ODClipboardMethodDebug("ODClipboard","InitClipboard");

	/* Moved from somInit. SOM itself sets fields to zero
	_fSession			= (ODSession*) kODNULL;
	_fContainerHandle	= (ODHandle) kODNULL;
	_fContainer			= (ODContainer*) kODNULL;
	_fDocument			= (ODDocument*) kODNULL;
	_fDraft				= (ODDraft*) kODNULL;
	_fSU				= (ODStorageUnit*) kODNULL;
	
	_fOriginalDraft		= (ODDraft*) kODNULL;
	_fExportedLinkSpec	= kODFalse;

	_fClonePasteCount	= 0;
	*/
	somSelf->InitObject(ev);
			
	_fSession = session;
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	// Initialize our scrap count to an invalid value so a paste will force copying
	// from the platform scrap.
	_fScrapCount			= kInvalidScrapCount;
	_fScrapCountLastChange	= kInvalidScrapCount;
#endif
	
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	_fUpdateID = kODUnknownUpdate;
	_fNextUpdateID = _fSession->UniqueUpdateID(ev);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
  _fUpdateID = kODUnknownUpdate;
  _fLastImportUpdateID = kODUnknownUpdate;
  _fNextUpdateID = _fSession->UniqueUpdateID(ev);
  _fArePromises = kODFalse;
  _fHab = WinQueryAnchorBlock( HWND_DESKTOP );
   _fTypeCollection = new OrderedCollection;

   for ( int i = 0;
         i < sizeof( clipboardTypeInfo ) / sizeof( clipboardTypeInfo[0] );
         i++ )
      {
      _fTypeCollection->AddLast( (ElementType)&clipboardTypeInfo[i] );
      }

   WinAddAtom(WinQuerySystemAtomTable(), kCF_OPENDOCDOCUMENT);
   WinAddAtom(WinQuerySystemAtomTable(), kCF_OPENDOCOWNERID);
#endif // _PLATFORM_OS2_

	_fOriginalCloneKind = kODCloneCopy;
}

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
//------------------------------------------------------------------------------
// ApplicationHeapFree
//------------------------------------------------------------------------------

ODStatic ODULong ApplicationHeapFree()
{
	ODULong free;
	THz curZone = GetZone();
	SetZone(ApplicationZone());
	free = FreeMem();
	SetZone(curZone);
	return free;
}
#endif

//------------------------------------------------------------------------------
// ODClipboard::Purge (OVERRIDE)
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODClipboardPurge(ODClipboard *somSelf, Environment *ev,
	ODSize size)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","Purge");
	
	ODSize freed = 0;
	ODVolatile(freed);
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
			
	SOM_TRY

		Size scrapSize = InfoScrap()->scrapSize;
		short scrapCount = GetMacScrapCount();
		
		if ( (scrapCount == _fScrapCountLastChange) && (scrapCount == _fScrapCount) )
		{
			ForceZeroScrap();
			// Update scrap later
			_fScrapCountLastChange = _fScrapCount = GetMacScrapCount();

#if ODDebugClipboard
			PRINT("ODClipboard::Purge: Scrap cleared\n");
#endif
		}
		else
		{
#if ODDebugClipboard
			if ( InfoScrap()->scrapState > kScrapOnDisk )
				PRINT("ODClipboard::Purge: Scrap unloaded\n");
#endif
			UnloadScrap();
		}

#if ODDebugClipboard
		PRINT("ODClipboard::Purge: Freed %ld bytes in app heap\n", freed);
#endif

		freed = parent_Purge(somSelf, ev, size); // always call parent version of Purge()

	SOM_CATCH_ALL
	
	SOM_ENDTRY
#endif

	return freed;
}

//------------------------------------------------------------------------------
// ODClipboard::NewClipboard
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardNewClipboard(ODClipboard *somSelf, Environment *ev)
{
    ODClipboardData *somThis = ODClipboardGetData(somSelf);
    ODClipboardMethodDebug("ODClipboard","NewClipboard");

#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return;
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_

	ODStorageUnit* draftProperties = (ODStorageUnit*) kODNULL;
	
	ODVolatile(draftProperties);
	ODVolatile(somSelf);
	ODVolatile(ev);

	TRY
		// Create a new clipboard in-memory container
		_fContainerHandle = ODNewHandle(0);
		_fContainer = CreateMemoryContainer(ev, _fSession, _fContainerHandle, kODBentoMemoryContainer);

		// Create the root content storage unit
		_fDocument = _fContainer->AcquireDocument(ev, kODDefaultDocument);
		_fDraft = _fDocument->AcquireBaseDraft(ev, kODDPExclusiveWrite);
		_fSU = _fDraft->CreateStorageUnit(ev);

		// Store a reference to the content storage unit in the draft properties
		draftProperties = _fDraft->AcquireDraftProperties(ev);
		ODSetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef, _fSU->GetID(ev));	
		draftProperties->Release(ev);

	CATCH_ALL
#ifdef _REPORT_SCRAP_ERRORS_
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: Cannot create clipboard - error %ld",ErrorCode());
#endif
		somSelf->DiscardClipboard(ev);
		ODReleaseObject(ev, draftProperties);
		RERAISE;
	ENDTRY
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// CloseClipboard
//------------------------------------------------------------------------------
// Close the current clipboard container, without disposing of the container handle.
// Note: CloseClipboard must not make any method calls to somSelf, since
//		 CloseClipboard is called from the somUninit method.

ODStatic void CloseClipboard(ODClipboard *somSelf, Environment *ev, ODBoolean externalize)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	
	if ( externalize && _fDraft )
	{
		// Externalize may fail with an iterator-out-of-sync error because 
		// resolving promises may add storage units to the draft's collection
		// during iteration.  In theory, the only promises should be in the
		// root storage unit, so explicitly externalize it first to resolve
		// promises before interating over all storage units.
		if ( _fSU )
			_fSU->Externalize(ev);
		_fDraft->Externalize(ev);
	}

	ODReleaseObject(ev,_fSU);
	ODReleaseObject(ev,_fDraft);
	ODReleaseObject(ev,_fDocument);
	ODReleaseObject(ev,_fContainer);
}

//------------------------------------------------------------------------------
// ODClipboard::DiscardClipboard
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardDiscardClipboard(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","DiscardClipboard");

#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return;
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_
	
	CloseClipboard(somSelf, ev, kODFalse);
	
	ODDisposeHandle(_fContainerHandle);
	_fContainerHandle = (ODHandle) kODNULL;
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODClipboard::ImportContent
//------------------------------------------------------------------------------
//
// On entry, assumes the scrap has already been checked for consistency.
// Returns an exception if the scrap can't be loaded into memory.

SOM_Scope void  SOMLINK ODClipboardImportContent(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","ImportContent");
	
	ODHandle	hData = kODNULL;
	long		scrapOffset = 0;
	
	ODVolatile(hData);
	
	SOM_TRY
	
		hData = ODNewHandle(0);

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)

		// Because we walk the scrap, it must be loaded in memory.
		THROW_IF_ERROR(ClipboardLoadScrap());
	
		PScrapStuff myScrapStuff = InfoScrap();
		Handle		hScrap = myScrapStuff->scrapHandle;

		while ( scrapOffset < myScrapStuff->scrapSize )
		{
			long		scrapTypeLength;
			ResType		theType;
			ODULong		realOffset;
	
			// There is more in the scrap
			theType = *(ResTypePtr)(((long) *hScrap) + scrapOffset);
#endif // AIX or MAC
#ifdef _PLATFORM_OS2_


                ResType theType, thePrevType = 0;
                while ( (theType = WinEnumClipbrdFmts( _fHab, thePrevType)) != 0 )
                {
                    long    scrapTypeLength;
                    ODType    theISOType;
                    ODULong  realOffset;

                    theISOType = _fSession->GetTranslation(ev)->GetISOTypeFromPlatformType(ev, theType, kODPlatformDataType );

                    if (theISOType != (ODType) kODNULL)
                    {
#endif // _PLATFORM_OS2_
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
			scrapTypeLength = GetScrap((Handle) hData, theType, (long*) &realOffset);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                        scrapTypeLength = GetScrap( somSelf, (Handle) hData, theType,
                                        (long*) &realOffset);
#endif // _PLATFORM_OS2_
			if ( scrapTypeLength < 0 )
				THROW(scrapTypeLength);

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
			if ( realOffset < scrapOffset )
			{
				// We tried to read a second occurance of the same resource type
				// on the scrap.  Since the result of GetScrap is the length
				// of the first occurance, we can't read the scrap past this point.
				// So just quit.
#ifdef _REPORT_SCRAP_ERRORS_
				PRINT("ODClipboard: ImportContent: Duplicate resource on scrap, type = \'%.4s\'\n", &theType);
#endif
				break;
			}
#endif // MAC or AIX
#if ODDebugClipboard
			PRINT("Found scrap type \'%.4s\'\n", &theType);
#endif
			if ( scrapTypeLength > 0 )
			{
#ifdef _REPORT_SCRAP_ERRORS_
				if ( theType == kODScrapTypeODBentoContainer )
				{
					// Internal error
					WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: ImportContent: Reading BentoContainer as scrap type!");
				}
#endif

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
				// Special case: If 'styl' is present, create an 'stxt' if one
				// isn't also on the scrap
				if ( theType == 'styl' )
				{
					long dummy;
					if ( GetScrap(nil, 'stxt', &dummy) == noTypeErr )
						ImportStyledTextType(somSelf, ev);
				}
				
				TempODType theISOType = kODNULL;
				theISOType = _fSession->GetTranslation(ev)->GetISOTypeFromPlatformType(ev, theType, kODPlatformDataType);
#endif // MAC or AIX		

				if ( theISOType != (ODType) kODNULL )
				{
					if ( ODSUExistsThenFocus(ev, _fSU, kODPropContents, (ODType) theISOType) )
					{
						WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: Replacing value on clipboard");
						_fSU->Remove(ev);
					}
					ODSUForceFocus(ev, _fSU, kODPropContents, (ODType) theISOType);
					ODValue pData = ODLockHandle(hData);
					StorageUnitSetValue(_fSU, ev, (ODULong) scrapTypeLength, pData);
					ODUnlockHandle(hData);
				}
			}
#ifdef _PLATFORM_OS2_
                        thePrevType = theType;
                    }

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
			// The length must be EVEN!
			if (scrapTypeLength & 1)
			{
				scrapTypeLength += 1;
			}
			scrapOffset = realOffset + scrapTypeLength;
#endif // MAC or AIX
		}

	SOM_CATCH_ALL

#ifdef _REPORT_SCRAP_ERRORS_
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: ImportContent: Failed with error %ld",ErrorCode());
#endif
		if ( ErrorCode() == memFullErr )
			SetErrorCode(kODErrOutOfMemory);

	SOM_ENDTRY

#ifdef _PLATFORM_OS2_
      _fArePromises = kODFalse;
#endif
	ODDisposeHandle(hData);
}

//------------------------------------------------------------------------------
// ODClipboard::PutContentOnPlatformClipboard
//------------------------------------------------------------------------------
//
// Copy values with corresponding platform types onto the desk scrap.
// THROWs an exception if any error occurs, which may cause the desk scrap to
// be only partially updated. The desk scrap requires copying the data in one chunk.

SOM_Scope void  SOMLINK ODClipboardPutContentOnPlatformClipboard(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","PutContentOnPlatformClipboard");
	
	ODULong			count;
	ODULong			index;
	ODPlatformType	platformType;

	SOM_TRY

		if ( (_fSU == kODNULL) && (_fContainerHandle != kODNULL) )
			OpenClipboard(somSelf, ev);
			
		if ( _fSU )
		{
                        //sesh -- [136778]
#ifdef OLE_STUFF
                        if (  _fSU->Exists(ev,kODPropContents, kODKindOlePart, 0))
                        {
                           PutOleContentOnPlatformClipboard(_fSU);
                        }
#endif // OLE_STUFF
			_fSU->Focus(ev, kODPropContents, kODPosUndefined, 0, 0, kODPosUndefined);
			count = _fSU->CountValues(ev);
			for (index = 1; index <= count; ++index)
			{
				_fSU->Focus(ev, kODPropContents, kODPosUndefined, 0, index, kODPosUndefined);
				TempODType theISOType = _fSU->GetType(ev);

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
				platformType = _fSession->GetTranslation(ev)->GetPlatformTypeFromISOType(ev, (ODType) theISOType);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                                platformType = _fSession->GetTranslation(ev)->GetPlatformTypeFromISOType(ev, theISOType );
#endif // _PLATFORM_OS2_
				if (platformType != (ODPlatformType) kODNULL)
				{

					_fSU->Focus(ev, kODPropContents, kODPosUndefined, (ODType) theISOType, 0, kODPosUndefined); // rrk 26 jul 96
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
//					_fSU->Focus(ev, kODPropContents, kODPosUndefined, (ODType) theISOType, 0, kODPosUndefined);
					Size dataSize = _fSU->GetSize(ev);

					TempODPtr data = kODNULL;
					data = ODNewPtr(dataSize, kDefaultHeapID);

					StorageUnitGetValue(_fSU, ev, dataSize, (ODValue) data);
	
					THROW_IF_ERROR(ClipboardPutScrap(dataSize, platformType, data));

					// If styled text ('stxt') was just written to the scrap, also write
					// 'stly' if a 'TEXT' representation will also be written.
					if ( platformType == 'stxt' )
						if ( _fSU->Exists(ev, kODPropContents, kODAppleTEXT, 0) )
							if ( !_fSU->Exists(ev, kODPropContents, kODApplestyl, 0) )
								THROW_IF_ERROR(ExportStylType(data));
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                                        OSErr  error;
                                        ODPtr  data = (ODPtr) kODNULL;
                                        ULONG  dataSize;

                               //         _fSU->Focus(ev, kODPropContents, kODPosUndefined, theISOType, 0,
                               //                                kODPosUndefined);
                                        if ( _fSU->IsPromiseValue( ev ))
                                        {
                               //            _fArePromises = kODTrue;
                                           PutPMPromise( somSelf, platformType );
                                           _fArePromises = kODTrue;
                               /* [146569] - commented out promise removal
                                           _fSU->Remove( ev );
                                           index--;
                               */
                                           error = noErr;
                                        }
                                        else
                                        {
                                           ULONG handle = NULLHANDLE;
                                           if(handle = GetDataHandle(ev, _fSU)) {
                                               dataSize = sizeof(handle);
                                               data = &handle;
                                           }
                                           else {
                                                dataSize = _fSU->GetSize(ev);
                                                // there may be no size, but we need to use a real size here
                                                // so that all the storage stuff works.
                                                if (dataSize == 0) dataSize = 4;
                                                data = ODNewPtrClear(dataSize, kDefaultHeapID);
                                                StorageUnitGetValue(_fSU, ev, dataSize, (ODValue) data);
                                           }
                                           error = (OSErr) PutScrap( somSelf, dataSize, platformType, (Ptr) data);
                                           if(!handle) ODDisposePtr(data);
                                        }
                                        if (error != noErr)
                                        {
                                           ODDisposePtr(theISOType);
                                           THROW(error);
                                        }
#endif // _PLATFORM_OS2_
				}
			}
		}

	SOM_CATCH_ALL

	SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODClipboard::ExportPlatformTypes
//------------------------------------------------------------------------------
//
// Export data of the argument platform types to the host clipboard.
// Since only one ISO type corresponds directly to a platform type (no translation
// is attempted), we just examine each value on the clipboard and check if
// its platform equivalent is present in the argument set.


SOM_Scope void  SOMLINK ODClipboardExportPlatformTypes(ODClipboard *somSelf, Environment *ev,
	ODPlatformTypeList* typeList)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","ExportPlatformTypes");
#ifdef _PLATFORM_OS2_	
        OSErr  error = noErr;
#endif
	ODULong	countOfValues;
	ODULong	index;
	ODULong	countToExport;
	
	SOM_TRY

		if ( (_fSU == kODNULL) && (_fContainerHandle != kODNULL) )
			OpenClipboard(somSelf, ev);
			
		if ( _fSU )
		{
			_fSU->Focus(ev, kODPropContents, kODPosUndefined, 0, 0, kODPosUndefined);
			countOfValues = _fSU->CountValues(ev);
			if (typeList != (ODPlatformTypeList*) kODNULL)
				countToExport = typeList->Count(ev);
			else
				countToExport = countOfValues;
			
			for (index = 1; (index <= countOfValues) && (countToExport > 0); ++index)
			{
				_fSU->Focus(ev, kODPropContents, kODPosUndefined, 0, index, kODPosUndefined);
				TempODType theISOType = _fSU->GetType(ev);
			
				ODPlatformType platformType = 
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
					_fSession->GetTranslation(ev)->GetPlatformTypeFromISOType(ev, (ODType) theISOType);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                                        _fSession->GetTranslation(ev)->GetPlatformTypeFromISOType(ev, theISOType );
#endif // _PLATFORM_OS2_
				if ( platformType != (ODPlatformType) 0 )
				{
					if ((typeList == (ODPlatformTypeList*) kODNULL) || (typeList->Contains(ev, platformType)))
					{
						_fSU->Focus(ev, kODPropContents, kODPosUndefined, (ODType) theISOType, 0, kODPosUndefined);
#ifdef _PLATFORM_OS2_
                                                if ( _fSU->IsPromiseValue( ev ))
                                                {
                                                  PutPMPromise( somSelf, platformType );
                                                  _fArePromises = kODTrue;
                                 /* [146569] - commented out promise removal
                                                  _fSU->Remove( ev );
                                                  index--;
                                 */
                                                  error = noErr;
                                                }
                                                else
                                                {
#endif
						ULONG dataSize = _fSU->GetSize(ev);

						TempODPtr data = kODNULL;
						data = ODNewPtr(dataSize, kDefaultHeapID);
				
						StorageUnitGetValue(_fSU, ev, dataSize, (ODValue) data);
			

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
						THROW_IF_ERROR(ClipboardPutScrap(dataSize, platformType, data));
#endif	
#ifdef _PLATFORM_OS2_
                                                error = (OSErr) PutScrap( somSelf, dataSize, platformType, /*(Ptr)*/(char*)(void*) data);
                                                if (error != noErr)
                                                {

                                                  ODDisposePtr(theISOType);
                                                  THROW(error);
                                                }
                                                }
#endif
						if (typeList != (ODPlatformTypeList*) kODNULL)
							typeList->Remove(ev, platformType);
						countToExport -= 1;
					}
				}
			}
		}

	SOM_CATCH_ALL
	
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		if ( ErrorCode() == memFullErr )
			SetErrorCode(kODErrOutOfMemory);
#endif // MAC or AIX

	SOM_ENDTRY
}

//------------------------------------------------------------------------------
// OpenClipboard
//------------------------------------------------------------------------------

ODStatic void OpenClipboard(ODClipboard *somSelf, Environment *ev)
{
    ODClipboardData *somThis = ODClipboardGetData(somSelf);

	ODStorageUnitID	suID;
	
	ODVolatile(somSelf);
	ODVolatile(ev);

	if ( _fContainerHandle )
	{
		TRY
			_fContainer = GetMemoryContainer(ev, _fSession, _fContainerHandle, kODBentoMemoryContainer);		
			_fDocument = _fContainer->AcquireDocument(ev, kODDefaultDocument);
			_fDraft = _fDocument->AcquireBaseDraft(ev, kODDPExclusiveWrite);
	
			{ TempODStorageUnit draftProperties = _fDraft->AcquireDraftProperties(ev);
			  suID = ODGetStrongSURefProp(ev, draftProperties, kODPropRootPartSU, kODStrongStorageUnitRef);
			}
	
			_fSU = _fDraft->AcquireStorageUnit(ev, suID);
		CATCH_ALL
			CloseClipboard(somSelf, ev, kODFalse);
			RERAISE;
		ENDTRY
	}
}

//------------------------------------------------------------------------------
// ODClipboard::PutClipboardOnPlatformClipboard
//------------------------------------------------------------------------------
//
// This method has the side effect of closing the clipboard container.
// Also note that by externalizing the clipboard draft, all promises are resolved.

SOM_Scope void  SOMLINK ODClipboardPutClipboardOnPlatformClipboard(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","PutClipboardOnPlatformClipboard");
	
	SOM_TRY 

	// Externalize then close the clipboard container
	CloseClipboard(somSelf, ev, kODTrue);

	if ( _fContainerHandle )
	{
		OSErr	error;
		ULONG	dataSize;
		char	flags;
	
		dataSize = ODGetHandleSize(_fContainerHandle);


#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		THROW_IF_ERROR(UnloadScrapIfMemLow(dataSize));
#endif // MAC or AIX

#ifdef OSA_INCLUDED
		flags = HGetState((Handle) _fContainerHandle);
#endif // OSA_INCLUDED
		Ptr containerPtr = (Ptr) ODLockHandle(_fContainerHandle);
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		error = (OSErr) PutScrap(dataSize, kODScrapTypeODBentoContainer, containerPtr);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                error = (OSErr) PutScrap(somSelf, dataSize, CF_OPENDOCDOCUMENT,
                                (Ptr)*((Handle)_fContainerHandle));
#endif // _PLATFORM_OS2_
#ifdef OSA_INCLUDED
		HSetState((Handle) _fContainerHandle, flags);
#endif // OSA_INCLUDED

#ifdef _REPORT_SCRAP_ERRORS_
		WASSERTM(error == noErr, "ODClipboard: PutScrap() failed");
#endif
		THROW_IF_ERROR(error);
	}
#ifdef _REPORT_SCRAP_ERRORS_
	else
	{
		// Internal error
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: PutClipboardOnPlatformClipboard: No Clipboard to export!\n");
	}
#endif

	SOM_CATCH_ALL
	
#ifdef _REPORT_SCRAP_ERRORS_
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: PutClipboardOnPlatformClipboard: Raising exception %d\n", ErrorCode());
#endif

	SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODClipboard::ImportClipboard
//------------------------------------------------------------------------------
//
// A bento memory container is always used for data interchange.
// If OpenClipboard throws, its probably because a bad resource of type
// kODScrapTypeODBentoContainer was found on the clipboard (currently error 1012,
// but it may change and there may be others).
// This routine returns:
//		kODFalse, if the platform clipboard does not contain an OpenDoc clipboard;
//		kODTrue, if an OpenDoc clipboard is successfully imported;
//		an exception if an OpenDoc clipboard could not be imported

SOM_Scope ODBoolean  SOMLINK ODClipboardImportClipboard(ODClipboard *somSelf, Environment *ev)
	{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","ImportClipboard");

        ODBoolean retVal;
#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_
	
	ODHandle	hData = kODNULL;
	ULONG		dataSize;
	long		dummy;
#ifdef _PLATFORM_OS2_
        ULONG        ulFmtInfo;
#endif
	
	WASSERTMSG_DEBUG(_fContainerHandle == (ODHandle) kODNULL, "ODClipboard: fContainerHandle not null");
	WASSERTMSG_DEBUG(_fContainer == (ODContainer*) kODNULL, "ODClipboard: fContainer not null");
	WASSERTMSG_DEBUG(_fDocument == (ODDocument*) kODNULL, "ODClipboard: fDocument not null");
	WASSERTMSG_DEBUG(_fDraft == (ODDraft*) kODNULL, "ODClipboard: fDraft not null");
	WASSERTMSG_DEBUG(_fSU == (ODStorageUnit*) kODNULL, "ODClipboard: fSU not null");
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	if ( !ScrapHasType(kODScrapTypeODBentoContainer) )
		return kODFalse;
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
   if (!WinQueryClipbrdFmtInfo( _fHab, CF_OPENDOCDOCUMENT, &ulFmtInfo ))
      return kODFalse;
#endif
	
	ODVolatile(hData);
	
	TRY
		hData = ODNewHandle(0);
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		dataSize = GetScrap((Handle) hData, kODScrapTypeODBentoContainer, &dummy);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                dataSize = GetScrap(somSelf, (Handle) hData, CF_OPENDOCDOCUMENT, &dummy);
#endif
		if ( dataSize < 0 )
		{
			if ( dataSize == memFullErr )
				THROW(kODErrOutOfMemory);
			else
				THROW(dataSize);
		}
	
		_fContainerHandle = hData;

		OpenClipboard(somSelf, ev);
	CATCH_ALL
#ifdef _REPORT_SCRAP_ERRORS_
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: Cannot import clipboard - error %ld",ErrorCode());
#endif
		_fContainerHandle = kODNULL;
		ODDisposeHandle(hData);
		RERAISE;
	ENDTRY
        retVal = kODTrue;
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          retVal = kODFalse;
        SOM_ENDTRY
#endif
	
	return retVal;
}

//------------------------------------------------------------------------------
// ODClipboard::GetUpdateID
//------------------------------------------------------------------------------

SOM_Scope ODUpdateID  SOMLINK ODClipboardGetUpdateID(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","GetUpdateID");
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	short curScrapCount = GetMacScrapCount();
	
	// If the desk scrap has not changed since we last imported it, 
	// or if the desk scrap has not changed since the last change to this clipboard,
	// return the current change id, otherwise, return the next change id.
	// Note that this routine will return the same "next" change id
	// even if the desk scrap has changed since the last call to this routine.
	if (curScrapCount == _fScrapCount)
		return _fUpdateID;
	else if (curScrapCount == _fScrapCountLastChange)
		return _fUpdateID;
	else
		return _fNextUpdateID;
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
   ODUpdateID updateID = GetPMUpdateID(_fHab);

   if(!updateID)
      return _fNextUpdateID;

   if( (!_fUpdateID) || (_fLastImportUpdateID == updateID) )
      return _fUpdateID;

   return _fNextUpdateID;
#endif
}

//------------------------------------------------------------------------------
// ODClipboard::Clear
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardClear(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","Clear");

#if ODDebugClipboard
	PRINT("ODClipboard::Clear \n");
#endif
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	if ( !IsFrontProcess() )
	{
		ODSetSOMException(ev, kODErrBackgroundClipboardClear);
		return;
	}
	
	// Unless called when the process is in the background,
	// don't return an error so this method can be called in a failure handler.
	FN_CATCH return;
#endif // MAC or AIX

#if ODDebugClipboard
	if ( (_fSU != (ODStorageUnit*) kODNULL) && (_fSU->GetRefCount(ev) != 1) )
		PRINT("ODClipboard::Clear - Storage unit refCount = %d\n", _fSU->GetRefCount(ev));
#endif
	
#ifdef _PLATFORM_OS2_
  WinOpenClipbrd(_fHab);
  WinEmptyClipbrd(_fHab);
  WinCloseClipbrd(_fHab);
  ODUpdateID updateID = GetPMUpdateID(_fHab);

  _fLastImportUpdateID = updateID;
  if(!_fLastImportUpdateID) {
           _fLastImportUpdateID = _fSession->UniqueUpdateID(ev);
           SetPMUpdateID(_fHab, _fLastImportUpdateID);
  }
#endif
	somSelf->DiscardClipboard(ev);
	
	_fUpdateID = _fNextUpdateID;
	_fNextUpdateID = _fSession->UniqueUpdateID(ev);

#if ODDebugClipboard
	PRINT("ODClipboard::Clear _fOriginalDraft = kODNULL\n");
#endif
	
	_fOriginalDraft = kODNULL;
	_fOriginalCloneKind = kODCloneCopy;
	_fClonePasteCount = 0;
	

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	// Remember to export to desk scrap
	_fScrapCountLastChange = _fScrapCount = GetMacScrapCount();
#endif // MAC or AIX
}

//------------------------------------------------------------------------------
// ODClipboard::GetContentStorageUnit
//------------------------------------------------------------------------------
//
// If importing from the host clipboard fails, this method returns a new, empty
// storage unit.  No alert is displayed to inform the user.  This method will
// retry importing until Clear() is called.  An exception is returned only if
// an empty clipboard cannot be created.
//
// If this routine is called when the process is in the background, GetScrap()
// will probably return memFullError.  Even if it did return scrap data, the
// data would not be reliable because the forground application might be keeping
// the true clipboard data in a private scrap.  So rather than returning a scrap
// error, just return an empty clipboard.

SOM_Scope ODStorageUnit*  SOMLINK ODClipboardGetContentStorageUnit(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","GetContentStorageUnit");
	
#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH
	{
#ifdef _REPORT_SCRAP_ERRORS_
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: GetContentStorageUnit: Raising exception %d\n", ErrorCode());
#endif
		return kODNULL;
	}
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_
	
	ODVolatile(somSelf);
	ODVolatile(ev);

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	short curScrapCount = GetMacScrapCount();
#endif // MAC or AIX
	
#if ODDebugClipboard
	if ( (_fSU != (ODStorageUnit*) kODNULL) && (_fSU->GetRefCount(ev) != 1) )
		PRINT("ODClipboard::GetContentStorageUnit - Storage unit refCount = %d\n", _fSU->GetRefCount(ev));
#endif

	// _fScrapCount is the platform scrap count that was last imported or in effect when the clipboard changed.
	// If "curScrapCount == _fScrapCount", the platform scrap has not changed since it was last imported.
	// _fScrapCountLastChange is the platform scrap count at the time the OpenDoc clipboard last changed.
	// If "curScrapCount == _fScrapCountLastChange", the platform scrap has not changed since the
	// OpenDoc clipboard last changed.

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	if ( (curScrapCount != _fScrapCount) && (curScrapCount != _fScrapCountLastChange) )
	{
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
        ODUpdateID updateID = GetPMUpdateID(_fHab);

        if(!updateID || updateID != _fLastImportUpdateID)
        {
                WinOpenClipbrd( _fHab );
                _fLastImportUpdateID = updateID;
#endif
		// Import from desk scrap into the OpenDoc clipboard
#if ODDebugClipboard
		PRINT("ODClipboard: GetContentStorageUnit: Updating clipboard from scrap\n");
#endif

		somSelf->DiscardClipboard(ev);

		TRY
			if ( ScrapHasData() )
			{
				if ( !somSelf->ImportClipboard(ev) )
				{
					somSelf->NewClipboard(ev);
					somSelf->ImportContent(ev);
				}
#ifdef _PLATFORM_OS2_
                                if(!_fLastImportUpdateID) {
                                       _fLastImportUpdateID = _fSession->UniqueUpdateID(ev);
                                       WinSetClipbrdData( _fHab, _fLastImportUpdateID, CF_OPENDOCOWNERID, CFI_HANDLE );
                                }
#endif
			}
			else
			{
				somSelf->NewClipboard(ev);
			}
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
			_fScrapCount = curScrapCount;
#endif // MAC or AIX
			_fUpdateID = _fNextUpdateID;
			_fNextUpdateID = _fSession->UniqueUpdateID(ev);
#ifdef _PLATFORM_OS2_
                        _fArePromises = kODFalse;
#endif // _PLATFORM_OS2_
		CATCH_ALL
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
#ifdef _REPORT_SCRAP_ERRORS_
			if ( IsFrontProcess() )
				WARN("ODClipboard: GetContentStorageUnit: Importing from desk scrap raised exception %d\n", ErrorCode());
#endif
#endif // MAC or AIX
			// Importing an OpenDoc clipboard, content, or NewClibpard failed;
			// just create a new, empty clipboard
			somSelf->DiscardClipboard(ev);
			somSelf->NewClipboard(ev);
		ENDTRY
		_fOriginalDraft = kODNULL;
		_fOriginalCloneKind = kODCloneCopy;
		_fClonePasteCount = 0;
#ifdef _PLATFORM_OS2_
                WinCloseClipbrd( _fHab );
#endif
	}
	else if ( _fContainerHandle == (ODHandle) kODNULL )
	{
		// Open a new clipboard container if a container handle doesn't exist
		// This case can only happen after an error or after Clear() is called
		somSelf->NewClipboard(ev);
		_fOriginalDraft = kODNULL;
		_fOriginalCloneKind = kODCloneCopy;
		_fClonePasteCount = 0;
	}
	else
	{
		// reopen the container handle if necessary.
		if ( _fContainer == kODNULL )
			OpenClipboard(somSelf, ev);
		if ( _fOriginalDraft == kODNULL )
			_fOriginalDraft = GetOriginalDraft(ev, _fDraft);
	}

	return _fSU;
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
	{
#ifdef _REPORT_SCRAP_ERRORS_
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: GetContentStorageUnit: Raising exception %d\n", ErrorCode());
		WARNMSG_DEBUG(WARN_INDEX(-1),"ODClipboard: Cannot create clipboard - error %ld",ErrorCode());
#endif
	}
        SOM_ENDTRY
	return kODNULL;
#endif
}

//------------------------------------------------------------------------------
// ODClipboard::ActionDone
//------------------------------------------------------------------------------

SOM_Scope ODUpdateID  SOMLINK ODClipboardActionDone(ODClipboard *somSelf, Environment *ev,
	ODCloneKind cloneKind)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","ActionDone");

	ODUpdateID update = kODUnknownUpdate;
	
	SOM_TRY

		// Note that if the clipboard is changed by one OpenDoc document,
		// and used (via Paste) in another, the sequence of undo, redo, 
		// and paste actions is separate in each document. That is, the
		// first document is unaware of the Paste in the second document.
		// This isn't semantically correct, but doesn't cause errors [cc].

		if ( (cloneKind == kODCloneCut) || (cloneKind == kODCloneCopy) )
		{
			_fOriginalCloneKind = cloneKind;
			_fClonePasteCount = 0;
		}
		else if ( cloneKind == kODClonePaste )
		{
			_fClonePasteCount += 1;
		}
		else
			THROW(kODErrIllegalClipboardCloneKind);

		update = _fUpdateID;
	
	SOM_CATCH_ALL
	
	SOM_ENDTRY
	
	return update;
}

//------------------------------------------------------------------------------
// ODClipboard::ActionUndone
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardActionUndone(ODClipboard *somSelf, Environment *ev,
	ODUpdateID update,
	ODCloneKind originalCloneKind)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","ActionUndone");

	SOM_TRY

		if ( update == _fUpdateID )
		{
			if ( originalCloneKind == kODCloneCut )
			{
				SetOriginalCloneKind(ev, _fDraft, kODCloneCopy);
			}
			else if ( originalCloneKind == kODClonePaste )
			{
				_fClonePasteCount -= 1;
				if ( (_fClonePasteCount == 0) && (_fOriginalCloneKind == kODCloneCut) )
					SetOriginalCloneKind(ev, _fDraft, kODCloneCut);
			}
			else if ( originalCloneKind != kODCloneCopy )
				THROW(kODErrIllegalClipboardCloneKind);
		}

	SOM_CATCH_ALL
	
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODClipboard::ActionRedone
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardActionRedone(ODClipboard *somSelf, Environment *ev,
	ODUpdateID update,
	ODCloneKind originalCloneKind)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","ActionRedone");

	SOM_TRY

		if ( update == _fUpdateID )
		{
			if ( originalCloneKind == kODCloneCut )
			{
				SetOriginalCloneKind(ev, _fDraft, kODCloneCut);
			}
			else if ( originalCloneKind == kODClonePaste )
			{
				_fClonePasteCount += 1;
				if ( (_fClonePasteCount == 1) )
					SetOriginalCloneKind(ev, _fDraft, kODCloneCopy);
			}
			else if ( originalCloneKind != kODCloneCopy )
				THROW(kODErrIllegalClipboardCloneKind);
		}

	SOM_CATCH_ALL
	
	SOM_ENDTRY
}

//------------------------------------------------------------------------------
// ODClipboard::SetPlatformClipboard
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODClipboardSetPlatformClipboard(ODClipboard *somSelf, Environment *ev,
	ODPlatformTypeList* typeList)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","SetPlatformClipboard");
	
	ODPlatformTypeList* requestedTypes = (ODPlatformTypeList*) kODNULL;
	ODVolatile(requestedTypes);
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	short scrapCount = GetMacScrapCount();

	if ( (scrapCount == _fScrapCountLastChange) && (scrapCount == _fScrapCount) )
	{
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
               ODUpdateID updateID = GetPMUpdateID(_fHab);

                if(!updateID || _fLastImportUpdateID != updateID) return;
#endif // _PLATFORM_OS2_
		SOM_TRY
		
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
			THROW_IF_ERROR(ForceZeroScrap());
#endif // MAC or AIX

			if ( _fSU != (ODStorageUnit*) kODNULL )
			{
#ifdef _PLATFORM_OS2_
                                WinOpenClipbrd( _fHab );
                                WinEmptyClipbrd( _fHab );
#endif // _PLATFORM_OS2_
				if ( typeList )
					requestedTypes = _fSession->GetStorageSystem(ev)->CreatePlatformTypeList(ev, typeList);

				TRY
					somSelf->ExportPlatformTypes(ev, requestedTypes);
#ifdef _PLATFORM_OS2_
                                        SetClipboardOwnership( somSelf, ev );
                                        _fLastImportUpdateID = _fUpdateID;
#endif // _PLATFORM_OS2_
				CATCH_ALL
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
					ZeroScrap();
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                                        WinEmptyClipbrd( _fHab );
#endif // _PLATFORM_OS2_
					RERAISE;
				ENDTRY
#ifdef _PLATFORM_OS2_
                                WinCloseClipbrd( _fHab );
#endif // _PLATFORM_OS2_
			}

		SOM_CATCH_ALL
		
		SOM_ENDTRY

		if ( requestedTypes )
			delete requestedTypes;

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		// Since the scrap has only been partially updated, or an error has
		// occurred, we still need to remember to transfer everything later [cc]
		_fScrapCountLastChange = _fScrapCount = GetMacScrapCount();
	}
#endif // MAC or AIX
}

//------------------------------------------------------------------------------
// ODClipboard::ExportClipboard
//------------------------------------------------------------------------------
//
// Returns a SOM exception if setting the platform clipboard fails.
// Unfortunately, until the clipboard container is closed its size isn't known.

SOM_Scope void  SOMLINK ODClipboardExportClipboard(ODClipboard *somSelf, Environment *ev)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","ExportClipboard");
	
	SOM_TRY 
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		// If this clipboard was the last change to the platform scrap ("count == _fScrapCountLastChange"),
		// and this clipboard has not already been copied to the platform scrap
		// ("count == _fScrapCount"), update the platform scrap now. [cc]
		
		short count = GetMacScrapCount();
#endif // MAC or AIX
		
#ifdef _PLATFORM_OS2_
// aml - [146749]
// we need to figure out whether we should update the PM Clipboard

                ODUpdateID updateID = GetPMUpdateID(_fHab);
                if(_fLastImportUpdateID && _fLastImportUpdateID == updateID) {
//                    we've imported the clipboard, so we must have the latest changes
                   if(_fLastImportUpdateID == _fUpdateID) // we're in sync
                     return;
                }
                else return; // we don't own clipboard contents

                WinOpenClipbrd( _fHab );
                WinEmptyClipbrd( _fHab );
#endif // _PLATFORM_OS2_
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
#if ODDebugClipboard
		PRINT("ODClipboard::ExportClipboard called\n");
		if (count == _fScrapCountLastChange)
		{
			PRINT("ExportClipboard: OpenDoc clipboard contains last change\n");
			if ( count == _fScrapCount )
			{
				PRINT("ExportClipboard: OpenDoc clipboard needs exporting");
				if ( _fSU && _fSU->Exists(ev, kODPropLinkSpec, kODNULL, 0) )
					PRINT(" (has link spec)");
				PRINT("\n");
			}
			else if ( _fExportedLinkSpec && _fSU && !_fSU->Exists(ev, kODPropLinkSpec, kODNULL, 0) )
				PRINT("ExportClipboard: OpenDoc clipboard needs exporting to remove link spec\n");
			else
				PRINT("ExportClipboard: OpenDoc clipboard already copied to platform scrap\n");
		}
#endif
#endif // MAC or AIX
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		ODBoolean needToExport = kODFalse;
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                // if we got this far in OS/2, we know we need to export the clipboard
		ODBoolean needToExport = kODTrue;
#endif // _PLATFORM_OS2_
		ODBoolean needToExportToRemoveLinkSpec = kODFalse;
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		if  (count == _fScrapCountLastChange )
		{
			if ( count == _fScrapCount )
			{
				needToExport = kODTrue;
				_fExportedLinkSpec = _fSU && _fSU->Exists(ev, kODPropLinkSpec, kODNULL, 0);
			}
			else if ( _fExportedLinkSpec )
			{
				needToExportToRemoveLinkSpec = _fSU && !_fSU->Exists(ev, kODPropLinkSpec, kODNULL, 0);
			}
		}
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
		_fExportedLinkSpec = _fSU && _fSU->Exists(ev, kODPropLinkSpec, kODNULL, 0);
                if ( _fExportedLinkSpec )
	        {
			needToExportToRemoveLinkSpec = _fSU && !_fSU->Exists(ev, kODPropLinkSpec, kODNULL, 0);
		}
#endif // _PLATFORM_OS2_
	
		// Immediately after changing the clipboard, _fOriginalDraft won't be set correctly
		// because the clipboard is not informed when a part is finished writing. [cc]
		if ( needToExport && (_fOriginalDraft == kODNULL) )
		{
			ASSERT(_fDraft != kODNULL, kODErrAssertionFailed);
			_fOriginalDraft = GetOriginalDraft(ev, _fDraft);
		        WASSERTMSG(_fOriginalDraft != kODNULL, "Last clipboard change did not clone", AMSG_861);
		}
	
		if ( needToExport || needToExportToRemoveLinkSpec )
		{
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
			// Copy the OpenDoc clipboard to the scrap
			THROW_IF_ERROR(ForceZeroScrap());
	
			// Optimistically load the scrap into memory if it was unloaded
			LoadScrap();
#endif // MAC or AIX
	
			TRY
				somSelf->PutContentOnPlatformClipboard(ev);
				somSelf->PutClipboardOnPlatformClipboard(ev);
				
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
				_fScrapCountLastChange = GetMacScrapCount();
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                                SetClipboardOwnership( somSelf, ev );
                                _fLastImportUpdateID = _fUpdateID;     // [146749]
#endif // _PLATFORM_OS2_
			CATCH_ALL
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
				// If writing the scrap fails, remove any scrap types written [cc]
				ZeroScrap();
#endif // MAC or AIX
#ifdef _REPORT_SCRAP_ERRORS_
				PRINT("ODClipboard::ExportClipboard failed; clearing scrap\n");
#endif
				RERAISE;
			ENDTRY

#if ODDebugClipboard
			PRINT("ODClipboard::ExportClipboard updated the scrap, size is %ld\n", InfoScrap()->scrapSize);
#endif
		}
	
		// Exporting was successful
		if ( needToExportToRemoveLinkSpec )
			_fExportedLinkSpec = kODFalse;

	SOM_CATCH_ALL

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		// If we fail to update the scrap for any reason, ensure fScrapCountLastChanged
		// and fScrapCount equals the Mac scrap count, so we can try again later if 
		// no other cut or copy is attempted.
		_fScrapCountLastChange = _fScrapCount = GetMacScrapCount();
#endif // MAC or AIX

#if ODDebugClipboard
		PRINT("ODClipboard::ExportClipboard failed!\n");
#endif

	SOM_ENDTRY
#ifdef _PLATFORM_OS2_
        WinCloseClipbrd( _fHab );
#endif // _PLATFORM_OS2_
	
#if ODDebugClipboard
	PRINT("ODClipboard::ExportClipboard done, fExportedLinkSpec = %d\n", _fExportedLinkSpec);
#endif

}

//------------------------------------------------------------------------------
// ODClipboard::DraftClosing
//------------------------------------------------------------------------------
//
// Forces resolution of promises, and prevents moving content across draft closings
// via the clipboard. In order to work correctly, parts MUST use BeginClone-EndClone
// when writing to the clipboard.

SOM_Scope void  SOMLINK ODClipboardDraftClosing(ODClipboard *somSelf, Environment *ev,
		ODDraft* draft)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","DraftClosing");

#if ODDebugClipboard
	PRINT("ODClipboard::DraftClosing called, draft is %x\n", draft);
#endif

#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return;
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_

	if ( draft )
	{
		// Immediately after changing the clipboard, _fOriginalDraft won't be set correctly
		// because the clipboard is not informed when a part is finished writing.
	
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		short scrapCount = GetMacScrapCount();
		ODBoolean needToExport = (scrapCount == _fScrapCountLastChange) && (scrapCount == _fScrapCount);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                ODUpdateID updateID = GetPMUpdateID(_fHab);
//                ODBoolean needToExport = (_fLastImportUpdateID != updateID) || (_fLastImportUpdateID != _fUpdateID); 
                ODBoolean needToExport = (_fLastImportUpdateID && _fLastImportUpdateID != updateID); 
#endif // _PLATFORM_OS2_                                                    
		if ( needToExport && (_fOriginalDraft == kODNULL) )
		{
			ASSERT(_fDraft != kODNULL, kODErrAssertionFailed);
			_fOriginalDraft = GetOriginalDraft(ev, _fDraft);
			WASSERTMSG(_fOriginalDraft != kODNULL, "Last clipboard change did not clone", AMSG_861);
		}
	
#if ODDebugClipboard
		PRINT("Last draft to change clipboard is %x\n", _fOriginalDraft);
#endif

		if ( _fOriginalDraft == draft )
		{
			// Last draft that wrote the clipboard is being closed.

#if ODDebugClipboard
			PRINT("ODClipboard::Draft that last wrote clipboard is closing\n", draft);
			if ( needToExport )
				PRINT("ODClipboard::Clipboard needs exporting\n");
#endif
			// If the clipboard container is open, force resolution of promises 
			// by externalizing the clipboard draft. (If there are unresolved promises, 
			// the clipboard hasn't be exported yet). Otherwise open the container.

			if ( _fContainer )
				_fDraft->Externalize(ev);
			else
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
				OpenClipboard(somSelf, ev);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                        {
				OpenClipboard(somSelf, ev);
                                somSelf->ResolvePMPromises(ev);
                        }
#endif // _PLATFORM_OS2_

			// Remove a link spec if present
			ODSURemoveProperty(ev, _fSU, kODPropLinkSpec);

			// Check to see if content was cut (not copied) to the clipboard.
			// If so, change the original clone kind to copy, so any future
			// paste won't be treated as a move.
						
			if ( GetOriginalCloneKind(ev, _fDraft) == kODCloneCut )
			{
				SetOriginalCloneKind(ev, _fDraft, kODCloneCopy);
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
				// Force exporting to platform clipboard
				_fScrapCountLastChange = _fScrapCount = GetMacScrapCount();
#endif // MAC or AIX
			}

			_fOriginalDraft = kODNULL;
		}
	}
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODClipboard::DraftSaved
//------------------------------------------------------------------------------
//

SOM_Scope void  SOMLINK ODClipboardDraftSaved(ODClipboard *somSelf, Environment *ev,
		ODDraft* draft)
{
	ODClipboardData *somThis = ODClipboardGetData(somSelf);
	ODClipboardMethodDebug("ODClipboard","DraftSaved");

#if ODDebugClipboard
	PRINT("ODClipboard::DraftSaved called, draft %08x, orig %08x, fDraft %08x\n", 
		(long) draft, (long) _fOriginalDraft, (long) _fDraft);
#endif

#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return;
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_
	
	// ODDraft* tDraft = _fDraft;
	// ODDraft* tOriginalDraft = _fOriginalDraft;
	
	if ( draft )
	{
		if ( _fOriginalDraft == kODNULL )
		{
			if (_fDraft != kODNULL)		// can be null if never accessed
				_fOriginalDraft = GetOriginalDraft(ev, _fDraft);

#if ODDebugClipboard
	PRINT("    ODClipboard::DraftSaved orig %08x\n", 
		(long) _fOriginalDraft);
#endif
		}

		if ( _fOriginalDraft == draft && _fDraft )
		{
			// Check to see if content was cut (not copied) to the clipboard.
			// If so, change the original clone kind to copy, so any future
			// paste won't be treated as a move.

#if ODDebugClipboard
			PRINT("        ODClipboard::DraftSaved try cut -> copy\n");
#endif
						
			if ( GetOriginalCloneKind(ev, _fDraft) == kODCloneCut )
			{
				SetOriginalCloneKind(ev, _fDraft, kODCloneCopy);

#if ODDebugClipboard
				PRINT("            ODClipboard::DraftSaved perform cut -> copy\n");
#endif
			}
		}
	}
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODClipboard::ShowPasteAsDialog
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODClipboardShowPasteAsDialog(ODClipboard *somSelf, Environment *ev,
		ODBoolean canPasteLink,
		ODPasteAsMergeSetting mergeSetting,
		ODFacet* facet,
		ODTypeToken viewType,
		ODPasteAsResult* result)
{
    ODClipboardData *somThis = ODClipboardGetData(somSelf);
    ODClipboardMethodDebug("ODClipboard","ShowPasteAsDialog");

	ODBoolean returnValue = kODFalse;

#ifdef _PLATFORM_MACINTOSH_
	SOM_CATCH return kODFalse;
#else
        SOM_TRY
#endif // _PLATFORM_MACINTOSH_

	THROW_IF_NULL(facet, kODErrNullFacetInput);
	THROW_IF_NULL(result, kODErrNullPasteAsResultInput);

	ODTypeToken modalFocus = _fSession->Tokenize(ev, kODModalFocus);
	ODArbitrator* arbitrator = _fSession->GetArbitrator(ev);
	TempODFrame currentOwner = arbitrator->AcquireFocusOwner(ev, modalFocus);

	if ( arbitrator->RequestFocus(ev, modalFocus, facet->GetFrame(ev)) )
	{
		ODStorageUnit* clipContentSU = somSelf->GetContentStorageUnit(ev);
	
		ODBoolean isMove = (GetOriginalCloneKind(ev, clipContentSU->GetDraft(ev)) == kODCloneCut);

		returnValue = ShowPasteAsDialog(
								canPasteLink, 
								mergeSetting,
								isMove,
								clipContentSU,
								facet, 
								viewType, 
								result);

		arbitrator->TransferFocus(ev, modalFocus, facet->GetFrame(ev), currentOwner);
	}
	else
	{
#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
		SysBeep(2);
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
                WinAlarm(HWND_DESKTOP, WA_ERROR);
#endif
	}
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL
          returnValue =  kODFalse;
        SOM_ENDTRY
	return returnValue;
#endif
}

#ifdef _PLATFORM_OS2_
/*---------------------------------------------------------------------------
        ODClipboard::DeregisterClipboardFormat
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODClipboardDeregisterClipboardFormat(
                                ODClipboard *somSelf,
                                Environment *ev,
                                ODType theISOType )
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);
   ODClipboardMethodDebug("ODClipboard","DeregisterClipboardFormat");

   OrderedCollectionIterator * iter = kODNULL;
   ODBoolean result = kODFalse;

   //ODBeginTryBlock;

#if ODDebugClipboard
   somPrintf( "ODClipboard::DeregisterClipboardFormat\n" );
#endif

   iter = _fTypeCollection->CreateIterator();
   CLIPBOARDTYPEINFO *pclipboardTypeInfo;

   try {
      for ( pclipboardTypeInfo = (CLIPBOARDTYPEINFO *)iter->First();
            iter->IsNotComplete();
            pclipboardTypeInfo = (CLIPBOARDTYPEINFO *)iter->Next())
         {
         if ( !strcmp(pclipboardTypeInfo->opendocType, theISOType ))
            {
            if ( pclipboardTypeInfo->deletable == kODTrue )
               {
               delete [] pclipboardTypeInfo->opendocType;
               _fTypeCollection->Remove( pclipboardTypeInfo );
               result = kODTrue;
               }
            break;
            }
         }

        } catch( ... ) {
#if ODDebugClipboard
      somPrintf( "ODClipboard::DeregisterClipboardFormat - SOM EXCEPTION THROWN\n" );
#endif
      result = kODFalse;
   }

   delete iter;

   //ODExitTryBlock;
   return( result );
/*
OnException:

   if (iter != kODNULL) delete iter;

#if ODDebugClipboard
   somPrintf( "ODClipboard::DeregisterClipboardFormat - OS2 EXCEPTION THROWN\n" );
#endif
   ODLogError( SEVERITY_WARNING, PMERR_INVALID_PARAMETER );
   return( result );
*/
}

/*---------------------------------------------------------------------------
        ODClipboard::DispatchHandler

        This method is unique to OS/2. It was introduced to facilitate
        delayed rendering of the Opendoc clipboard. This method is called
        from the dispatcher when an WM_RENDERFMT message is encountered.
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODClipboardDispatchHandler(
                                ODClipboard     *somSelf,
                                Environment     *ev,
                                ODEventData     *event )
{
   ODClipboardData *somThis = ODClipboardGetData( somSelf );
   ODClipboardMethodDebug( "ODClipboard","ODClipboardDispatchHandler" );

#if ODDebugClipboard
   somPrintf( "ODClipboard::DispatchHandler\n" );
#endif

   ODBoolean result = kODTrue;

   TRY
      if ( event->msg != WM_RENDERFMT )
      {
         // we shouldn't get here - only WM_RENDERFMT msgs are forwarded.
#if ODDebugClipboard
         somPrintf( "ODClipboard::DispatchHandler"
                  " - Warning: msg not WM_RENDERFMT" );
#endif
         result = kODFalse;
      }
      else
      {
/* [146749] - aml - we cannot use this any more...
         ODPart *part = _fClipboardOwner;
         if (kODNULL == part)
         {
            // we shouldn't get here - the "giving" part can't be found
#if ODDebugClipboard
            somPrintf( "ODClipboard::DispatchHandler"
                     " - Warning: _fClipboardOwner is null\n" );
#endif
         }
         else
         {
*/
// [146569] start
/*
            // borrow from the drag & drop stuff, to build a storage unit we
            // can pass along to the part...
            ODMemDragItem  *newItem = kODNULL;
            newItem = new(ODGetDefaultHeap())
                     ODMemDragItem(_fSession->GetStorageSystem( ev ), kODFalse);
            if ( kODNULL == newItem )
            {
                                        // out of memory
#if ODDebugClipboard
               somPrintf( "ODClipboard::DispatchHandler"
                        " - Error: can't alloc newItem\n" );
#endif
            }
            else
            {
               newItem->Initialize(ev);

               // set up the storage unit view for the part to fulfill
               ODStorageUnit *su = newItem->fSU;
               su->AddProperty(ev, kODPropContents);
               PSZ type = _fSession->GetTranslation(ev)->GetISOTypeFromPlatformType(ev, (ODULong)event->mp1, kODPlatformDataType );
//               PSZ type = GetISOTypeFromPlatformType( somSelf, (ODULong)event->mp1 );
               su->AddValue( ev, type );
                                        ODStorageUnitView *destSUView = su->CreateView(ev);
#if ODDebugClipboard
               somPrintf( "ODClipboard::DispatchHandler"
                        " - About to call FulfillPromise type(%s)\n", type );
#endif
               (part)->FulfillPromise( ev, destSUView );
*/
               if(!_fSU && _fContainerHandle) {
                       OpenClipboard(somSelf, ev);
               }
               else if(!_fSU) return result;
               PSZ theISOType = _fSession->GetTranslation(ev)->GetISOTypeFromPlatformType(ev, (ODULong)event->mp1, kODPlatformDataType);
               if (theISOType != (ODType) kODNULL &&
                   ODSUExistsThenFocus(ev,_fSU,kODPropContents,(ODValueType)theISOType))
               {
                 ODStorageUnitView *destSUView = _fSU->CreateView(ev);
                 if(_fSU->IsPromiseValue(ev)) {
                   ODByteArray ba;  // [146749]
                   ODPart *sourcePart;
                   _fSU->GetPromiseValue(ev, (ODValueType)theISOType,
                                        0, 0, &ba, &sourcePart );
                   sourcePart->FulfillPromise( ev, destSUView );
                 }

                 ODPlatformType ulType = (ODPlatformType)event->mp1;
                 ODULong flag = GetFormatInfo(somSelf,ulType);

                 if(flag == CFI_HANDLE)
                 {
                     ULONG handle = NULLHANDLE;

                     if(!(handle = GetDataHandle(ev, _fSU))) {  // [146749]
                       destSUView->SetOffset( ev, 0 );
                       StorageUnitViewGetValue(destSUView, ev,
                           sizeof( ULONG ), (ODValue) &handle);
                     }
                     result = WinSetClipbrdData( _fHab, (ULONG)handle,
												 ulType, CFI_HANDLE );
//                     _fArePromises = kODTrue;
#if ODDebugClipboard
                     somPrintf( "ODClipboard::DispatchHandler"
                              " - result(%d) = WinSetClipbdData( handle(%lX) )\n",
                                           result, handle );
#endif
                 }
                 else  // if(flag == CFI_POINTER)
                 {
                     ULONG dataSize = destSUView->GetSize(ev);
                     PBYTE  pbData = kODNULL;
                     APIRET rc = DosAllocSharedMem( (PPVOID)&pbData, 0L,
                                      dataSize,
                                      PAG_READ | PAG_WRITE | PAG_COMMIT |
                                      OBJ_GIVEABLE | OBJ_GETTABLE | OBJ_TILE );
                     if ( kODNULL == rc )
                        {
                        destSUView->SetOffset( ev, 0 );
                        StorageUnitViewGetValue(destSUView, ev,
                              dataSize, (ODValue) pbData);

                        WinSetClipbrdData( _fHab, (ULONG)pbData,
                                                                                                                 (ODULong)event->mp1, CFI_POINTER );
#if ODDebugClipboard
                        somPrintf( "ODClipboard::DispatchHandler"
                                 " - result(%d) = WinSetClipbdData( pointer(%p) )\n",
                                           result, pbData );
#endif
                        }
                     // the free of pbData is done for us by PM...
//                     break;
                 }

//               delete destSUView;
//               delete newItem;
               }
//            }           // newItem
//         }              // part   [146749]
      }                 // WM_RENDERFMT

        CATCH_ALL
#if ODDebugClipboard
      somPrintf( "ODClipboard::DispatchHandler - SOM EXCEPTION THROWN\n" );
#endif
   ENDTRY

   return result;      // handled
}

/*---------------------------------------------------------------------------
        ODClipboard::QueryContent
---------------------------------------------------------------------------*/
SOM_Scope void  SOMLINK ODClipboardQueryContent(
                                ODClipboard *somSelf,
                                Environment *ev,
                                ODTypeList *typeList)
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);
   ODClipboardMethodDebug("ODClipboard","QueryContent");

#if ODDebugClipboard
   somPrintf( "ODClipboard::QueryContent\n" );
#endif

   typeList->InitTypeList(ev,kODNULL);
   ODType    theISOType;
   TRY

      ODUpdateID updateID = GetPMUpdateID(_fHab);
      if(updateID && updateID == _fLastImportUpdateID && _fContainerHandle) {
         if(!_fSU )
             OpenClipboard(somSelf, ev);
         if(!ODSUExistsThenFocus(ev,_fSU,kODPropContents,kODNULL)) return;
         ODULong count = _fSU->CountValues(ev);
         for (ODULong index = 1; index <= count;index++)
         {
            _fSU->Focus(ev, kODPropContents, kODPosUndefined, 0, index,
                                kODPosUndefined);
            theISOType = _fSU->GetType(ev);
            if (theISOType != (ODType) kODNULL)
            {
               typeList->AddLast(ev,theISOType);
            }
         }
         return;
      }
      ResType theType, thePrevType = 0;
      while ( (theType = WinEnumClipbrdFmts( _fHab, thePrevType)) != 0 )
      {
         long    scrapTypeLength;
         ODULong  realOffset;

         if(theType == CF_OPENDOCDOCUMENT) typeList->AddLast(ev,kCF_OPENDOCDOCUMENT); // [145019]
         else {
           theISOType = _fSession->GetTranslation(ev)->GetISOTypeFromPlatformType(ev, theType, kODPlatformDataType );
           if (theISOType != (ODType) kODNULL)
           {
               typeList->AddLast(ev,theISOType);
           }
         }
         thePrevType = theType;
      }

	CATCH_ALL
#if ODDebugClipboard
      somPrintf( "ODClipboard::QueryContent - SOM EXCEPTION THROWN\n" );
#endif
      RERAISE;
   ENDTRY

}

/*---------------------------------------------------------------------------
        ODClipboard::CanEmbed
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean  SOMLINK ODClipboardCanEmbed(
                                ODClipboard *somSelf,
                                Environment *ev)
{
//   CHAR typeList[CCHMAXPATH];
   ODType theType;
   ODBoolean result = kODFalse;
   ODTypeList* typeList = new ODTypeList();
   somSelf->QueryContent(ev,typeList);
   if(!typeList->Count(ev)) {
      delete typeList;
      return kODFalse;
   }
//#ifdef LATER
   RegistryManager* registrationMgr;
   registrationMgr = new RegistryManager;
   ODPartHandlerRegistry* odRegistry;
   odRegistry = registrationMgr->GetODPartHandlerRegistry( ev);
   ODTypeListIterator* iter = typeList->CreateTypeListIterator(ev);
   for(theType = iter->First(ev); iter->IsNotComplete(ev); theType = iter->Next(ev))
   {
      if(strcmp((PSZ)theType, kCF_OPENDOCDOCUMENT) == 0) result = kODTrue;
      else if(theType[0]) {
         _IDL_SEQUENCE_ISOString partHandlerList = odRegistry->GetPartHandlerList( ev,theType, 0) ;
         if ( partHandlerList. _length ) {
            result = kODTrue;
         }
         for ( int i = 0; i < partHandlerList. _length; i++)
         {
              SOMFree( partHandlerList. _buffer[i]);
         }
         SOMFree( partHandlerList. _buffer);
      }
      if(result) break;
   }
   ODDeleteObject(iter);
   ODDeleteObject(typeList);
   return result;
//#else
//   return kODFalse;
//#endif // LATER
}


/*---------------------------------------------------------------------------
        ODClipboard::CanIncorporate
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean  SOMLINK ODClipboardCanIncorporate(
                                ODClipboard *somSelf,
                                Environment *ev,
                                ODType kind)
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);
   ODClipboardMethodDebug("ODClipboard","CanIncorporate");

   ULONG fmtInfo;
   ULONG fmt;
   ODBoolean result = kODFalse;;

   TRY
   ODUpdateID updateID = GetPMUpdateID(_fHab);
   if(updateID && _fLastImportUpdateID == updateID) {
        // this clipboard SU is up-to-date, check its kODPropContents
            if ( !_fSU && _fContainerHandle )
                   OpenClipboard(somSelf, ev);
            else return kODFalse;
            return(_fSU->Exists(ev, kODPropContents, kind, 0));
   }
   if (!ScrapHasData() ) return kODFalse;
   fmt = _fSession->GetTranslation(ev)->GetPlatformTypeFromISOType(ev, kind);
   if(fmt) result = WinQueryClipbrdFmtInfo(_fHab, fmt, &fmtInfo);
   else {
     // may be an OpenDoc kind
     if(WinQueryClipbrdFmtInfo(_fHab, CF_OPENDOCDOCUMENT, &fmtInfo)) {
       if(!WinOpenClipbrd(_fHab)) return kODFalse;
       somSelf->DiscardClipboard(ev);
       if(!somSelf->ImportClipboard(ev)) {
           somSelf->NewClipboard(ev);
       }
       somSelf->ImportContent(ev);
       _fLastImportUpdateID = updateID;
       if(!_fLastImportUpdateID) {
           _fLastImportUpdateID = _fSession->UniqueUpdateID(ev);
           WinSetClipbrdData( _fHab, _fLastImportUpdateID, CF_OPENDOCOWNERID, CFI_HANDLE );
       }
       _fUpdateID = _fLastImportUpdateID;
       WinCloseClipbrd(_fHab);
       if(_fSU)
         result = _fSU->Exists(ev, kODPropContents, kind, 0);
     }
   }
	CATCH_ALL
#if ODDebugClipboard
      somPrintf( "ODClipboard::CanIncorporate - SOM EXCEPTION THROWN\n" );
#endif
      RERAISE;
   ENDTRY
   return result;
}

/*---------------------------------------------------------------------------
        ODClipboard::RegisterClipboardFormat
---------------------------------------------------------------------------*/
SOM_Scope ODBoolean SOMLINK ODClipboardRegisterClipboardFormat(
                                ODClipboard *somSelf,
                                Environment *ev,
                                ODType theISOType,
                                ODPlatformType platformType,
                                ODULong formatInfo )
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);
   ODClipboardMethodDebug("ODClipboard","RegisterClipboardFormat");

   OrderedCollectionIterator * iter = kODNULL;
   ODBoolean result = kODTrue;

   //ODBeginTryBlock;

   iter = _fTypeCollection->CreateIterator();
   CLIPBOARDTYPEINFO *pclipboardTypeInfo;

   try {
      for ( pclipboardTypeInfo = (CLIPBOARDTYPEINFO *)iter->First();
            iter->IsNotComplete();
            pclipboardTypeInfo = (CLIPBOARDTYPEINFO *)iter->Next())
         {
         // if it's not unique, bail out
         if ( !strcmp(pclipboardTypeInfo->opendocType, theISOType ) ||
              pclipboardTypeInfo->platformType == platformType )
            {
            result = kODFalse;
            break;
            }
         }

      delete iter;

      if ( result == kODTrue )
         {
         pclipboardTypeInfo = new CLIPBOARDTYPEINFO;

         pclipboardTypeInfo->opendocType = new char[ strlen( theISOType ) + 1 ];
         strcpy( pclipboardTypeInfo->opendocType, theISOType );
         pclipboardTypeInfo->platformType = platformType;
         pclipboardTypeInfo->formatInfo = formatInfo;
         pclipboardTypeInfo->deletable = kODTrue;

         _fTypeCollection->AddLast(pclipboardTypeInfo);
         }
   } catch( ... ) {

#if ODDebugClipboard
      somPrintf( "ODClipboard::RegisterClipboardFormat - SOM EXCEPTION THROWN\n" );
#endif
      result = kODFalse;

   }

   //ODExitTryBlock;
   return( result );

/*
OnException:
#if ODDebugClipboard
   somPrintf( "ODClipboard::RegisterClipboardFormat - OS2 EXCEPTION THROWN\n" );
#endif
   ODLogError( SEVERITY_WARNING, PMERR_INVALID_PARAMETER );
   return( kODFalse );
*/
}

// [146749]
/*---------------------------------------------------------------------------
        ODClipboard::ResolvePMPromises
---------------------------------------------------------------------------*/
SOM_Scope void SOMLINK ODClipboardResolvePMPromises(
                                ODClipboard *somSelf,
                                Environment *ev)
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);
   ODClipboardMethodDebug("ODClipboard","ResolvePMPromises");

   ODUpdateID updateID = somSelf->GetUpdateID(ev);
   if(updateID != _fUpdateID || !_fArePromises) return;
   WinOpenClipbrd( _fHab );
   USHORT usFormat = 0;
   while ( usFormat = (USHORT) WinEnumClipbrdFmts( _fHab, usFormat ) )
        WinQueryClipbrdData( _fHab,  usFormat);
   WinCloseClipbrd( _fHab );
   _fArePromises = kODFalse;
}
#endif
//==============================================================================
// Local Functions
//==============================================================================

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
//------------------------------------------------------------------------------
// GetMacScrapCount
//------------------------------------------------------------------------------
//
// This function returns the identification stamp of the last mac scrap change,
// NOT the count of items on the scrap!

ODStatic short GetMacScrapCount()
{
	return InfoScrap()->scrapCount;
}

//------------------------------------------------------------------------------
// IsFrontProcess
//------------------------------------------------------------------------------

ODStatic ODBoolean IsFrontProcess()
{
	ProcessSerialNumber psnFront, psnMine;
	ODBoolean frontmost;

	GetFrontProcess(&psnFront);
	GetCurrentProcess(&psnMine);
	SameProcess(&psnFront, &psnMine, &frontmost);

	return frontmost;
}

//------------------------------------------------------------------------------
// ImportStyledTextType
//------------------------------------------------------------------------------

ODStatic void ImportStyledTextType(ODClipboard *somSelf, Environment *ev)
{
    ODClipboardData *somThis = ODClipboardGetData(somSelf);

	ODHandle	hndl = kODNULL;
	ODValue		value;
	long		dummy;

	ODVolatile(hndl);
	ODVolatile(somSelf);
	ODVolatile(somThis);
	ODVolatile(ev);

	SOM_CATCH
	{
		if ( ODSUExistsThenFocus(ev, _fSU, kODPropContents, kODApplestxt) )
			_fSU->Remove(ev);

		ODDisposeHandle(hndl);
		return;
	}
	
	// Make sure both 'TEXT' and 'styl' are present
	ODULong sizeText = GetScrap(nil, 'TEXT', &dummy);
	ODULong sizeStyl = GetScrap(nil, 'styl', &dummy);
	
	if ( (sizeText < 0) || (sizeStyl < 0) )
		return;

	if ( ODSUExistsThenFocus(ev, _fSU, kODPropContents, kODApplestxt) )
	{
#ifdef _REPORT_SCRAP_ERRORS_
		WARN("ODClipboard: Replacing value on clipboard");
#endif
		_fSU->Remove(ev);
	}
	
	ODSUForceFocus(ev, _fSU, kODPropContents, kODApplestxt);

	hndl = ODNewHandle(sizeStyl);
	
	sizeStyl = GetScrap((Handle) hndl, 'styl', &dummy);
	if ( sizeStyl < 0 )
		THROW(sizeStyl);

	value = (ODValue) ODLockHandle(hndl);
	StorageUnitSetValue(_fSU, ev, sizeStyl, value);
	ODUnlockHandle(hndl);
	ODDisposeHandle(hndl);
	hndl = kODNULL;

	hndl = ODNewHandle(sizeText);

	sizeText = GetScrap((Handle) hndl, 'TEXT', &dummy);
	if ( sizeText < 0 )
		THROW(sizeText);
	
	value = (ODValue) ODLockHandle(hndl);
	StorageUnitSetValue(_fSU, ev, sizeText, value);
	ODUnlockHandle(hndl);

	ODDisposeHandle(hndl);
}

//------------------------------------------------------------------------------
// ExportStylType
//------------------------------------------------------------------------------
//
// Argument is a pointer to styled text, with begins with a style record.

ODStatic OSErr ExportStylType(ODPtr	stxtData)
{
	Size stylSize = *((short *) stxtData);
	stylSize = (stylSize * sizeof(ScrpSTElement)) + sizeof(short);
	
	return ClipboardPutScrap(stylSize, 'styl', stxtData);
}

//------------------------------------------------------------------------------
// ScrapIsInconsistent
//------------------------------------------------------------------------------

ODStatic ODBoolean ScrapIsInconsistent()
{
	// Return false if the scrap in an inconsistent state and thus cannot
	// be read. [cc]

#if ODDebugClipboard
	if ( (InfoScrap()->scrapState > 0) && (InfoScrap()->scrapHandle == nil) )
		PRINT("ODClipboard: Scrap is inconsistent!\n");
#endif

	return  ((InfoScrap()->scrapState > 0) && (InfoScrap()->scrapHandle == nil));
	
}	

#endif // MAC or AIX
//------------------------------------------------------------------------------
// ScrapHasData
//------------------------------------------------------------------------------

ODStatic ODBoolean ScrapHasData()
{
	// Return true if the scrap size is greater than zero and the scrap is 
	// in a consistent state. [cc]

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
	return ((InfoScrap()->scrapSize > 0) && (!ScrapIsInconsistent()));
#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
  return (WinEnumClipbrdFmts(WinQueryAnchorBlock(HWND_DESKTOP), 0L) != 0L );
#endif // _PLATFORM_OS2_
}	

#if defined (_PLATFORM_MACINTOSH_) || defined(_PLATFORM_AIX_)
//------------------------------------------------------------------------------
// ScrapHasType
//------------------------------------------------------------------------------

ODStatic ODBoolean ScrapHasType(ODPlatformType type)
{
	long		dummy;
	Size		dataSize;
	ODBoolean	result = kODFalse;

	if ( ScrapHasData() )
	{
		dataSize = GetScrap(nil, type, &dummy);
		if ( dataSize >= 0 )
			result = kODTrue;
#ifdef _REPORT_SCRAP_ERRORS_
		else
		{
			if (dataSize != noTypeErr )
				WARN("ODClipboard: ScrapHasType: GetScrap() returned error %ld, type = %.4s", dataSize, &type);
		}
#endif
	}
	
	return result;
}

//------------------------------------------------------------------------------
// ForceZeroScrap
//------------------------------------------------------------------------------

ODStatic OSErr ForceZeroScrap()
{
	OSErr err = ZeroScrap();
	if ( err == nilHandleErr )
	{
#ifdef _REPORT_SCRAP_ERRORS_
		PRINT("ODClipboard: ZeroScrap returned error -109\n");
#endif
		// According to Dylan Ashe, if the scrap is inconsistent the best thing to
		// do is force re-initiaization of the scrap.  Use of this constant to
		// force re-initiaization will continue to work in Maxwell. [cc]
		InfoScrap()->scrapState = kUninitializedScrap;
		err = ZeroScrap();
	}
	return err;
}

//------------------------------------------------------------------------------
// ScrapMemoryAvailable
//------------------------------------------------------------------------------
//
// This function ensures than allocating dataSize bytes would still leave
// a 40K contiguous block free, as recommended by Inside Mac, Memory, 1-43. [cc]

ODStatic ODBoolean ScrapMemoryAvailable(Size dataSize)
{
	const Size kMemCushion = 40*1024;
	
	Size totalFree;
	Size contigFree;
	
	PurgeSpace(&totalFree, &contigFree);
	
#if ODDebugClipboard
	PRINT("Request %ld bytes with %ld available\n", dataSize, contigFree);
#endif

	return (dataSize + kMemCushion) < contigFree;
}

//------------------------------------------------------------------------------
// UnloadScrapIfMemLow
//------------------------------------------------------------------------------

ODStatic OSErr UnloadScrapIfMemLow(Size dataSize)
{
	OSErr unloadError = noErr;
	
	if ( InfoScrap()->scrapState != kScrapOnDisk )
	{
		if ( !ScrapMemoryAvailable(dataSize) )
		{
			unloadError = UnloadScrap();
	
#if ODDebugClipboard
			PRINT("ODClipboard: Scrap unloaded with error %d\n", unloadError);
#endif
		}
	}

	return unloadError;
}

//------------------------------------------------------------------------------
// ClipboardLoadScrap
//------------------------------------------------------------------------------

ODStatic OSErr ClipboardLoadScrap()
{
	OSErr loadError = noErr;
	
	if ( InfoScrap()->scrapState == kScrapOnDisk )
	{
		if ( ScrapMemoryAvailable(InfoScrap()->scrapSize) )
		{
			loadError = LoadScrap();
	
#if ODDebugClipboard
			 PRINT("ODClipboard: Scrap loaded with error %d\n", loadError);
#endif
		}
		else
		{
			loadError = kODErrOutOfMemory;
		}
	}

	return loadError;
}

//------------------------------------------------------------------------------
// ClipboardPutScrap
//------------------------------------------------------------------------------

ODStatic OSErr ClipboardPutScrap(Size dataSize, ODPlatformType	platformType, ODPtr data)
{
	OSErr error = UnloadScrapIfMemLow(dataSize);
	
	if ( error == noErr )
	{
		error = (OSErr) PutScrap(dataSize, platformType, data);

#ifdef _REPORT_SCRAP_ERRORS_
		WASSERTM(error == noErr, "ODClipboard: PutScrap() failed");
#endif
	}

	return error;
}

#endif // MAC or AIX
#ifdef _PLATFORM_OS2_
         
// [140007]
/*---------------------------------------------------------------------------
        GetFormatInfo
---------------------------------------------------------------------------*/
ODULong GetFormatInfo(ODClipboard* somSelf, ODPlatformType ulType  )
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);
   OrderedCollectionIterator * iter = _fTypeCollection->CreateIterator();
   CLIPBOARDTYPEINFO *pclipboardTypeInfo;

   ODULong ulFlag = 0l;
   for ( pclipboardTypeInfo = (CLIPBOARDTYPEINFO *)iter->First();
         iter->IsNotComplete();
         pclipboardTypeInfo = (CLIPBOARDTYPEINFO *)iter->Next())
   {
      if ( pclipboardTypeInfo->platformType == ulType)
      {
         ulFlag = pclipboardTypeInfo->formatInfo;
         break;
      }
   }

   delete iter;


   return( ulFlag );
}

/*---------------------------------------------------------------------------
        GetScrap
---------------------------------------------------------------------------*/
ULONG GetScrap( ODClipboard *somSelf, Handle hData, ODPlatformType type, long * dummy )
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);

   PBYTE        pbData;
   PULONG       pulData;
   ULONG        ulSize = 0;

#if ODDebugClipboard
   somPrintf( "GetScrap - type = %ld\n", type );
#endif

        pbData = (PBYTE)WinQueryClipbrdData( _fHab, type );

#if ODDebugClipboard
   somPrintf( "pbData: %p\n", pbData );
#endif
   if ( pbData )
      {
      switch ( type )
// aml - [140007] start
         {
         case CF_TEXT:
         case CF_DSPTEXT:
            ulSize = strlen( (PSZ)pbData ) + 1;

            ODSetHandleSize( (ODHandle)hData, ulSize );
            ODBlockMove( pbData, *hData, ulSize );
            break;

         case CF_METAFILE:
         case CF_DSPMETAFILE:
            ulSize = GpiQueryMetaFileLength( (HMF)pbData );
            ODSetHandleSize( (ODHandle)hData, ulSize );
            GpiQueryMetaFileBits( (HMF)pbData, 0, ulSize, (PBYTE)*hData );
            break;

         case CF_BITMAP:
         case CF_DSPBITMAP:
            {
            BITMAPINFOHEADER2 bmih;
            PBITMAPINFO2 pbmih = NULL;
            ULONG scansize;
            LONG bmisize;
            PVOID  data;
            HPS     hpsMem;
            HDC     hdcMem;
            HBITMAP hbmMem = (HBITMAP)pbData;
            SIZEL sizlPage;

            hdcMem = DevOpenDC( _fHab,OD_MEMORY,"*",0L,NULL,NULLHANDLE);
            sizlPage.cx = sizlPage.cy = 0;

            hpsMem = GpiCreatePS( _fHab, hdcMem, &sizlPage,
                           PU_PELS | GPIA_ASSOC );
            HBITMAP hbmOld = GpiSetBitmap( hpsMem, hbmMem );

            bmih.cbFix = sizeof(bmih);
            GpiQueryBitmapInfoHeader(hbmMem, &bmih);

            scansize = ((bmih.cBitCount*bmih.cx+31)/32)*bmih.cPlanes*4;
            // bitmap size calc changed; defect 22946 (Merlin OS/2 defect 155161
            //bmisize = (bmih.cbFix+(sizeof(RGB2))*
            //                     (bmih.cclrUsed ? bmih.cclrUsed :
            //                      2<<(bmih.cBitCount*bmih.cPlanes) ) );
            bmisize = bmih.cbFix;
            if ((bmih.cBitCount * bmih.cPlanes) < 24) {
               bmisize += sizeof(RGB2) * (1 << bmih.cBitCount * bmih.cPlanes);
            ulSize = bmisize+scansize*bmih.cy;
            }

            ODSetHandleSize( (ODHandle)hData, ulSize );
            pbmih = (PBITMAPINFO2)*hData;

            data = (PVOID)pbmih;
            memcpy(data, (PVOID)&bmih, bmih.cbFix);
            pbmih->cBitCount = bmih.cBitCount*bmih.cPlanes;
            pbmih->cPlanes = 1;
            pbmih->cbImage = scansize*bmih.cy;
            pbmih->ulCompression = 0;
            GpiQueryBitmapBits(hpsMem, 0, bmih.cy,
                                ((PSZ)data)+bmisize, pbmih);
            GpiSetBitmap( hpsMem, hbmOld );
//            GpiDeleteBitmap( hbmMem );

            GpiSetBitmap(hpsMem,NULLHANDLE);
            GpiAssociate( hpsMem, NULLHANDLE );
            GpiDestroyPS( hpsMem );
            DevCloseDC( hdcMem );

            break;
            }

         case CF_PALETTE:
         {
            HPAL hpal = (HPAL)pbData;
            ULONG flOptions = 0L;
            ULONG ulCount = 0L;
            PULONG aulArray = kODNULL;
            ulSize = 0L;
            ulCount = GpiQueryPaletteInfo(hpal,NULLHANDLE, flOptions, 0L, 0L, aulArray);
            if(ulCount == PAL_ERROR) break;
            ulSize = (ulCount+1)*sizeof(ULONG);
            ODSetHandleSize( (ODHandle)hData, ulSize );
            aulArray = (PULONG)*hData;
            *aulArray++ = ulCount;
            ulCount = GpiQueryPaletteInfo(hpal,NULLHANDLE, flOptions, 0L, ulCount, aulArray);
            break;
         }
// aml - [140007] end
         default:
            if ( type == CF_OPENDOCDOCUMENT )
            {
               pulData = (PULONG)pbData;
               ulSize = *pulData;

               ODSetHandleSize( (ODHandle)hData, ulSize );
               ODBlockMove( pbData + sizeof( ULONG ), *hData, ulSize );
            }
            else {
               // this is to accomodate stuff placed on the pm clipboard
               // but not defined by a standard cf_ value.
               ULONG format;
               if(!WinQueryClipbrdFmtInfo(_fHab,type,&format))
                  format = 0L;
               if(!(format & CFI_POINTER)) break;
               ULONG ulFlags;
               ulSize = -1;

               if ( 0 == DosQueryMem( (PVOID)pbData, &ulSize, &ulFlags ))
               {
                  ODSetHandleSize( (ODHandle)hData, ulSize );
                  ODBlockMove( pbData, *hData, ulSize );
               }
            }
            break;
         }
      }

   return ulSize;
}

           

/*---------------------------------------------------------------------------
        PutPMPromise
---------------------------------------------------------------------------*/
ODBoolean PutPMPromise( ODClipboard *somSelf, ODPlatformType platformType )
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);
   ODBoolean fResult = kODFalse;

#if ODDebugClipboard
   somPrintf( "PutPMPromise - platformType( %ld )\n", platformType );
#endif

/*
   switch ( platformType )
      {
      case CF_BITMAP:
      case CF_DSPBITMAP:
      case CF_METAFILE:
      case CF_DSPMETAFILE:
      case CF_PALETTE:

         fResult = WinSetClipbrdData( _fHab, (ULONG)0,
                                            platformType, CFI_HANDLE );
         break;

      default:
         fResult = WinSetClipbrdData( _fHab, (ULONG)0,
                                      platformType, CFI_POINTER );
         break;
      }
*/

      ODULong flag = GetFormatInfo(somSelf,platformType);
      if(!flag) flag = CFI_POINTER;
      fResult = WinSetClipbrdData( _fHab, (ULONG)0,
                                      platformType, flag );
      _fArePromises = kODTrue;
#if ODDebugClipboard
   somPrintf( "result (%d)\n", fResult );
#endif
   return( fResult );
}
           
/*---------------------------------------------------------------------------
        PutScrap
---------------------------------------------------------------------------*/
OSErr PutScrap( ODClipboard *somSelf, ULONG dataSize, ODPlatformType platformType, Ptr data)
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);

#if ODDebugClipboard
   somPrintf( "PutScrap - size( %ld ) - platformType( %ld )\n",
                        dataSize, platformType );
#endif

   BOOL   fResult;
   PBYTE  pbData;
   APIRET rc;
   OSErr  err = noErr;
   PULONG pulData;

   switch ( platformType )
      {
      case CF_BITMAP:
      case CF_DSPBITMAP:
      case CF_METAFILE:
      case CF_DSPMETAFILE:
      case CF_PALETTE:     // [140007]
         pulData = (PULONG)data;
         fResult = WinSetClipbrdData( _fHab, (ULONG)*pulData,
                                            platformType, CFI_HANDLE );
         break;

      default:
         ULONG  ulSize =
            ( platformType == CF_OPENDOCDOCUMENT ) ? sizeof( ULONG ) : 0L;

         rc = DosAllocSharedMem( (PPVOID)&pbData, 0L, dataSize + ulSize,
                                    PAG_READ | PAG_WRITE | PAG_COMMIT |
                                    OBJ_GIVEABLE | OBJ_GETTABLE | OBJ_TILE );
         if ( rc )
            {
            err = kODErrOutOfMemory;
            }
         else
            {
            pulData = (PULONG)pbData;
            *pulData = dataSize;      // possibly overwritten by memcpy()

            memcpy( pbData + ulSize, data, dataSize );
            fResult = WinSetClipbrdData( _fHab, (ULONG)pbData,
                                            platformType, CFI_POINTER );
            // No need to free pbData here, the clipboard mgr has already
            // taken it from us...
            }
         break;
      }

   return err;
}

/*---------------------------------------------------------------------------
        SetClipboardOwnership

        This method is unique to OS/2. It sets up the PM clipboard so
        that we can later fulfill promies.
---------------------------------------------------------------------------*/
ODStatic ODBoolean SetClipboardOwnership(
                                ODClipboard *somSelf,
                                Environment *ev )
{
   ODClipboardData *somThis = ODClipboardGetData(somSelf);

   ODBoolean result = kODTrue;

#if ODDebugClipboard
   somPrintf( "SetClipboardOwnership - _fUpdateID(%08lX)\n", _fUpdateID );
#endif

//   _fUpdateID = _fSession->UniqueUpdateID(ev);

   result = WinSetClipbrdData( _fHab, _fUpdateID,        // stamp it
                      CF_OPENDOCOWNERID, CFI_HANDLE );

#if ODDebugClipboard
   somPrintf( "result(%d) = WinSetClipbrdData( CF_OPENDOCOWNERID )\n", result );
#endif

   // We are assuming everyone is a good citizen here right...
   ODTypeToken clipboardFocus =
                    _fSession->Tokenize(ev, kODClipboardFocus);
//D137529   ODFrame *frame = _fSession->GetArbitrator( ev )->
//D137529                                AcquireFocusOwner( ev, clipboardFocus );
   TempODFrame frame = _fSession->GetArbitrator( ev )->                      //D137529
                                AcquireFocusOwner( ev, clipboardFocus );     //D137529
   ODWindow* window = kODNULL;
   HWND hwndOwner = NULLHANDLE;
   if(frame == kODNULL) { // this may get called when no one has the clipboard focus;
      window = _fSession->GetWindowState(ev)->AcquireActiveWindow(ev);
   }
   else {
      window = frame->AcquireWindow( ev );
   }
   if(window) {
      hwndOwner = window->GetPlatformWindow( ev );
      ODReleaseObject(ev,window);
   }
   else return kODFalse;
/*
   ODPart* part = kODNULL;
   ODReleaseObject(ev,_fClipboardOwner);
   if(frame)
      part = frame->AcquirePart( ev );
   _fClipboardOwner = part;
*/
   // This is who we will send the WM_RENDERFMT to...
   result = WinSetClipbrdOwner( _fHab, hwndOwner );

#if ODDebugClipboard
         somPrintf( "result(%d) = WinSetClipbrdOwner( %08lX )\n",
                              result, hwndOwner );
#endif

   return( result );
}


ODStatic void SetPMUpdateID(HAB hab, ODUpdateID id)
{

#if ODDebugClipboard
   PRINT("SetPMUpdateID called: %d.\n", id);
#endif
   WinOpenClipbrd( hab );
   WinSetClipbrdData( hab, id, CF_OPENDOCOWNERID, CFI_HANDLE );
   WinCloseClipbrd( hab );
}

/*
        GetPMUpdateID

        Return the last change ID placed on the PM clipboard.
*/
ODStatic ODUpdateID GetPMUpdateID(HAB hab)
{
   ODUpdateID updateID = 0;

   WinOpenClipbrd( hab );

   updateID = WinQueryClipbrdData( hab, CF_OPENDOCOWNERID );

   WinCloseClipbrd( hab );

#if ODDebugClipboard
   PRINT("GetPMUpdateID called: %d.\n", updateID);
#endif
   return( updateID );
}
#ifdef OLE_STUFF
//D001  @SESH
//------------------------------------------------------------------------------
//   Function:   PutOleContentOnPlatformClipboard
//   Parameters: ODStorageUnit
//-------------------------------------------------------------------------------
//
void PutOleContentOnPlatformClipboard(ODStorageUnit *su)
{

  CHAR pszLibName[] = "PROXYHND";
  CHAR szMsg[80];
  HMODULE ModuleHandle;
  APIRET rc;

  typedef APIRET (_System *pfnODExportOlePart)(ODStorageUnit *);
  pfnODExportOlePart ODExportOlePart;

  rc =  DosLoadModule(szMsg, sizeof(szMsg), pszLibName, &ModuleHandle);
  if (rc != 0)
  {

#if ODDebugClipboard
      somPrintf("Could not Load PROXYHND.DLL, return code = %ld", rc);
#endif
  }

  rc = DosQueryProcAddr(ModuleHandle, 0, "ODExportOlePart", (PFN*)&ODExportOlePart);
  if (rc != 0)
      somPrintf("DosQueryProcAddr error: rc = %ld",rc);
  else
      ODExportOlePart(su);
  return;
}  //D001

//D001 -- SESH for Ole Interoperability

//------------------------------------------------------------------------------
//   Function:   GetOleContentFromPlatformClipboard
//   Parameters: ODStorageUnit
//-------------------------------------------------------------------------------
//
void GetOleContentFromPlatformClipboard(ODStorageUnit *su)
{

  CHAR pszLibName[] = "PROXYHND";
  CHAR szMsg[80];
  HMODULE ModuleHandle;
  APIRET rc;
  Environment *ev = somGetGlobalEnvironment();

  char   FmtName[20];
  USHORT usFormat = 0;        /* Temporary used when enumerating */
  HAB    vhab = WinInitialize(0);   /* if you don't have it already */

  /*
   * Cycle through the available clipboard formats
   */

   while ( usFormat = (USHORT) WinEnumClipbrdFmts( vhab, usFormat ) )
   {
      switch ( usFormat )
      {
//         case CF_EMPTY:
         case CF_TEXT:
         case CF_DSPTEXT:
         case CF_BITMAP:
         case CF_DSPBITMAP:
         case CF_METAFILE:
         case CF_DSPMETAFILE:
              break;

         default:

         /*
          *  Get the format name from the system atom table.
          */
         if ( WinQueryAtomName( WinQuerySystemAtomTable(),
                               usFormat, FmtName, sizeof(FmtName)) )
         {
             if (stricmp(FmtName, "DataObject") == 0)
             {
                 /* we have an OLE object to import */

                 typedef APIRET (_System *pfnODImportOlePart)(ODStorageUnit *);
                 pfnODImportOlePart ODImportOlePart;

                 rc =  DosLoadModule(szMsg, sizeof(szMsg), pszLibName, &ModuleHandle);
                 if (rc != 0)
                 {
                     somPrintf("Could not Load PROXYHND.DLL, return code = %ld", rc);
                     return;
                 }

                 rc = DosQueryProcAddr(ModuleHandle, 0, "ODImportOlePart", (PFN*)&ODImportOlePart);
                 if (rc != 0)
                     somPrintf("DosQueryProcAddr error: rc = %ld",rc);
                 else
                 {
                    const ODType kODOpenDocPresNormal = "OpenDoc:Presentation:Normal"; //#D001
                    ODEditor partEditor = "Proxyhnd:Proxyhnd";

                    ODDraft* clipdraft = su->GetDraft(ev);
                    ODPart* OlePart = clipdraft->CreatePart(ev,kODKindOlePart, partEditor);
                    if(OlePart == kODNULL)
                          THROW(kODErrCannotCreatePart);

                    //SESH -- needs to be checked again
                    ODShape* newShape = new ODShape;
                    newShape->InitShape(ev);

                    ODFrame* OleFrame = clipdraft->CreateFrame(ev, (ODType)kODFrameObject, (ODFrame*)kODNULL,
                                        newShape, (ODCanvas*)kODNULL, OlePart,(ODTypeToken)"OpenDoc:ViewAs:Frame",
                                        su->GetSession(ev)->Tokenize(ev, kODOpenDocPresNormal),
                                        kODFalse, kODFalse);
                    if(OleFrame == kODNULL)
                        THROW(kODErrCannotCreateFrame);

                    ODID frameInfoData = OleFrame->GetID(ev);
                    StorageUnitSetValue(su, ev, sizeof(ODID), (ODValue)&frameInfoData);
                    su->AddProperty(ev, kODPropContentFrame);
                    su->Focus(ev, kODPropContentFrame, kODPosUndefined, kODNULL, 0, kODPosUndefined);
                    su->AddValue(ev, kODWeakStorageUnitRef);

                    su->AddProperty(ev, kODPropFrameShape);

                    ODImportOlePart(su);
                 }
                 return;
             }
         }
         break;
      }
   }
}
//D001  OleInteroperability
#endif // OLE_STUFF
#endif // _PLATFORM_OS2_

ULONG GetDataHandle(Environment*ev, ODStorageUnit* su)
{

   ODType               theISOType;
   ULONG handle = NULLHANDLE;
   theISOType = su->GetType(ev);
   if(strcmp((PSZ)theISOType, kODKindOS2Bitmap) == 0)
      handle = (ULONG)BitmapFromStorage(ev,su);
   else if(strcmp((PSZ)theISOType, kODKindOS2Metafile) == 0)
      handle = (ULONG)MetafileFromStorage(ev,su);
   return handle;
}


HBITMAP BitmapFromStorage(Environment* ev, ODStorageUnit* storageUnit)
{
         SIZEL sizlPage = {0, 0};
         HAB hab = WinQueryAnchorBlock(HWND_DESKTOP);
         HDC hdc = DevOpenDC( hab,OD_MEMORY,"*",0L,NULL,NULLHANDLE);
         HPS hps = GpiCreatePS(hab, hdc, &sizlPage, PU_PELS | GPIA_ASSOC );
         HBITMAP hBitmap = NULLHANDLE;
         if(storageUnit->Exists(ev, kODPropContents, kODKindOS2Bitmap, 0)) {
            storageUnit->Focus(ev, kODPropContents, kODPosUndefined, kODKindOS2Bitmap, 0, kODPosUndefined);
            ULONG dataSize = storageUnit->GetSize( ev );
            if(dataSize > 0) {
               PBYTE Bitmap;

               DosAllocMem ((PPVOID)&Bitmap, dataSize,
                              PAG_COMMIT | PAG_READ | PAG_WRITE);

               StorageUnitGetValue(storageUnit, ev, dataSize, (ODValue) Bitmap );

               PBITMAPINFOHEADER2  pbmih = (PBITMAPINFOHEADER2)Bitmap;
               ULONG bmisize = (pbmih->cbFix+(sizeof(RGB2))*
                                       (pbmih->cclrUsed ? pbmih->cclrUsed :
                                        2<<(pbmih->cBitCount*pbmih->cPlanes) ) );

               hBitmap = GpiCreateBitmap (hps, pbmih, CBM_INIT,
                                Bitmap + bmisize, (PBITMAPINFO2)pbmih);

               if (hBitmap == GPI_ERROR)
                  hBitmap = 0;

               DosFreeMem( Bitmap );
            }
         }
         if (hps)
         {
            GpiAssociate(hps, 0);
            GpiDestroyPS(hps);
         }
         if (hdc)
            DevCloseDC(hdc);
         return hBitmap;
}

HMF MetafileFromStorage(Environment* ev, ODStorageUnit* storageUnit)
{
       HMF hMetafile = NULLHANDLE;
       if ( storageUnit->Exists(ev, kODPropContents, kODKindOS2Metafile, 0) )
       {
         storageUnit->Focus(ev, kODPropContents, kODPosUndefined, kODKindOS2Metafile, 0, kODPosUndefined);
         ULONG dataSize = storageUnit->GetSize( ev );
         if(dataSize > 0) {
          PBYTE Metafile;
          DosAllocMem ((PPVOID)&Metafile, dataSize,
                        PAG_COMMIT | PAG_READ | PAG_WRITE);

          StorageUnitGetValue(storageUnit, ev, dataSize, (ODValue) Metafile );
          DEVOPENSTRUC dop;
          HDC hdcMeta;
          SIZEL size = {0L,0L};
          HAB hab = WinQueryAnchorBlock(HWND_DESKTOP);
          dop.pszLogAddress = (PSZ) NULL;
          dop.pszDriverName = "DISPLAY";
          hdcMeta = DevOpenDC(hab,OD_METAFILE,"*",2L,(PDEVOPENDATA)&dop, (HDC)NULLHANDLE);
          hMetafile = DevCloseDC(hdcMeta);
          GpiSetMetaFileBits(hMetafile,0L,dataSize,Metafile);
          DosFreeMem(Metafile);
         }
       }
       return hMetafile;
}
#endif // _PLATFORM_OS2_
