/* @(#)Z 1.18 com/src/core/Part.cpp, odcore, od96os2, odos29646d 96/11/15 15:29:12 (96/10/29 09:21:23) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odcore
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
	File:		Part.cpp

	Contains:	ODPart class implementation

	Owned by:	Joshua Susser

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<31>	10/13/95	T‚		1284461 GM: Parts that are created in a
									document should not automatically be given
									names
		<30>	 10/8/95	TJ		Fixes Recomended by Refball
		<29>	 9/18/95	CG		1285279 GM: kODPartNotWrapper should be
									kODErrPartNotWrapper.
		<28>	  9/1/95	CC		1274788: InitPartName: Initialize to
									category.
									1273646: Override Externalize to remove
									lingering data interchange properties.
		<27>	 8/12/95	T‚		1276807 Opt./Bug: use StdTypIO routines for
									portable streaming & smaller footprint
		<26>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<25>	 6/26/95	VL		1262622: Added InitPartName. This ensures
									that every part has a name property.
		<24>	  6/1/95	JBS		1242639 Remove #define DR2_OPTIONS 1
		<23>	 5/26/95	DM		1251403: Multithreading naming support.
		<22>	 4/26/95	CG		1211082 BB: 5$ Bugs need to be evaluated
									and removed from Core
		<21>	 4/16/95	VL		1230084: Use POUtils in Init,
									InitFromStorage and CloneInto.
		<20>	 4/13/95	JBS		1233726 temp DR2_OPTIONS support for parts
									which don't implement ClonePartInfo.
		<19>	  4/3/95	JBS		1234218 optional protocols now return
									errors instead of SubclassResponsibility
		<18>	 3/28/95	JBS		1229656 add EditInLinkAttempted; 1226398
									add ClonePartInfo
		<17>	  3/7/95	CC		1224121: CreateLink() returns an
									ODLinkSource object.
		<16>	 1/26/95	eeh		1213354: ODActionData now an ODByteArray.
		<15>	 1/22/95	NP		EmbeddedFrameSpec signature changed because
									of change to AE types.
		<14>	 1/20/95	JBS		1195017, 1205669: part API changes
		<13>	 1/13/95	RR		1192147, 1201708 Removed implementations of
									HandleEventInEmbedded, MouseEnter,
									MouseWithin and MouseLeave. Moved
									functionality to HandleEvent
		<12>	12/23/94	JBS		1186869: hide ODPartWrapper from public
									API; 1192557: ODPartWrapper realPart access
		<11>	12/15/94	CC		1197221 CreateLink() takes ODByteArray
									parameter.
									1205684 LinkUdated parameter renamed to
									"change" from "id".
		<10>	  9/1/94	CC		RADAR #1183005 - change AdjustBorderShape
									param from Frame to Facet (for JBS)
		 <9>	 8/26/94	VL		1183174: Removed CloneInto.
		 <8>	 8/18/94	jpa		Fixed warnings by adding return values
									[1181839]
		 <7>	 8/16/94	VL		1143605: Fixed parameters to CloneInto.
		 <6>	 8/13/94	T‚		#1180814 Added support for ODPartWrapper
									API
		 <5>	  8/9/94	VL		1179857: DragEnter and DragWithin return
									ODBoolean.
		 <4>	  8/5/94	JBS		#1179151 Part API cleanup
		 <3>	 7/12/94	VL		Added SubClassResponsibility.
		 <2>	 6/28/94	CG		Synched API with a7.
		 <1>	 6/24/94	CG		first checked in
	To Do:
*/

#define ODPart_Class_Source
#define M_ODPart_Class_Source

#ifndef SOM_ODPart_xih
#define VARIABLE_MACROS
#include <Part.xih>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _POUTILS_
#include <POUtils.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _INFOUTIL_
#include "InfoUtil.h"
#endif
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _STORAGEU_
#include <StorageU.xh>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

//==============================================================================
// Function Prototype
//==============================================================================

void InitPartName(Environment* ev, ODStorageUnit* su);

//==============================================================================
// ODPart
//==============================================================================

//------------------------------------------------------------------------------
// ODPart: somUninit
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartsomUninit(ODPart *somSelf)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","somUninit");

    ODPart_parent_ODPersistentObject_somUninit(somSelf);
}
//------------------------------------------------------------------------------
// ODPart: InitPart
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartInitPart(ODPart *somSelf, Environment *ev,
		ODStorageUnit* storageUnit, ODPart* partWrapper)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","InitPart");
	
	try {

	somSelf->InitPersistentObject(ev, storageUnit);
	InitModificationInfo(ev, storageUnit);
	InitPartName(ev, storageUnit);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODPart: InitPartFromStorage
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartInitPartFromStorage(ODPart *somSelf, Environment *ev,
		ODStorageUnit* storageUnit, ODPart* partWrapper)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","InitPartFromStorage");
	
	try {

	somSelf->InitPersistentObjectFromStorage(ev, storageUnit);

	InitModificationInfo(ev, storageUnit);
	InitPartName(ev, storageUnit);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODPart: CloneInto
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartCloneInto(ODPart *somSelf, Environment *ev,
		ODDraftKey key,
		ODStorageUnit* toSU,
		ODFrame* scopeFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","CloneInto");
	
	try {

	parent_CloneInto(somSelf, ev, key, toSU, scopeFrame);
	CloneModificationInfo(ev, somSelf->GetStorageUnit(ev), toSU);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODPart: Externalize
//------------------------------------------------------------------------------

SOM_Scope void	 SOMLINK ODPartExternalize(ODPart *somSelf, Environment *ev)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","Externalize");
	
	try {

	parent_Externalize(somSelf, ev);

	// Remove data interchange properties that may have been left
	// when this part was dragged to the Finder
	RemoveDataInterchangeProperties(ev, somSelf->GetStorageUnit(ev), kODFalse);
	
        }       
        catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODPart: DisplayFrameAdded
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartDisplayFrameAdded(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DisplayFrameAdded");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: DisplayFrameRemoved
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartDisplayFrameRemoved(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DisplayFrameRemoved");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: DisplayFrameConnected
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartDisplayFrameConnected(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DisplayFrameConnected");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: DisplayFrameClosed
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartDisplayFrameClosed(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DisplayFrameClosed");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: AttachSourceFrame
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartAttachSourceFrame(ODPart *somSelf, Environment *ev,
		ODFrame* frame,
		ODFrame* sourceFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","AttachSourceFrame");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: FrameShapeChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartFrameShapeChanged(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","FrameShapeChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: ViewTypeChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartViewTypeChanged(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ViewTypeChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: PresentationChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartPresentationChanged(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","PresentationChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: SequenceChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartSequenceChanged(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","SequenceChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: LinkStatusChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartLinkStatusChanged(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","LinkStatusChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: ContainingPartPropertiesUpdated
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartContainingPartPropertiesUpdated(ODPart *somSelf, Environment *ev,
		ODFrame* frame,
		ODStorageUnit* propertyUnit)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ContainingPartPropertiesUpdated");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: ReadPartInfo
//------------------------------------------------------------------------------

SOM_Scope ODInfoType SOMLINK ODPartReadPartInfo(ODPart *somSelf, Environment *ev,
		ODFrame* frame,
		ODStorageUnitView* storageUnitView)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ReadPartInfo");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: WritePartInfo
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartWritePartInfo(ODPart *somSelf, Environment *ev,
		ODInfoType partInfo,
		ODStorageUnitView* storageUnitView)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","WritePartInfo");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: ClonePartInfo
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartClonePartInfo(ODPart *somSelf, Environment *ev,
		ODDraftKey key,
		ODInfoType partInfo,
		ODStorageUnitView* storageUnitView,
		ODFrame* scopeFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ClonePartInfo");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: Open
//------------------------------------------------------------------------------

SOM_Scope ODID SOMLINK ODPartOpen(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","Open");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return 0;
}

//------------------------------------------------------------------------------
// ODPart: Draw
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartDraw(ODPart *somSelf, Environment *ev,
		ODFacet* facet,
		ODShape* invalidShape)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","Draw");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: FacetAdded
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartFacetAdded(ODPart *somSelf, Environment *ev,
		ODFacet* facet)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","FacetAdded");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: FacetRemoved
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartFacetRemoved(ODPart *somSelf, Environment *ev,
		ODFacet* facet)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","FacetRemoved");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: GeometryChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartGeometryChanged(ODPart *somSelf, Environment *ev,
		ODFacet* facet, ODBoolean clipShapeChanged, ODBoolean externalTransformChanged)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","GeometryChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: HighlightChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartHighlightChanged(ODPart *somSelf, Environment *ev,
		ODFacet* facet)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","HighlightChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: CanvasChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartCanvasChanged(ODPart *somSelf, Environment *ev,
		ODFacet* facet)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","CanvasChanged");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: CanvasUpdated
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartCanvasUpdated(ODPart *somSelf, Environment *ev,
		ODCanvas* canvas)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","CanvasUpdated");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: GetPrintResolution
//------------------------------------------------------------------------------

SOM_Scope ODULong SOMLINK ODPartGetPrintResolution(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","GetPrintResolution");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return 0;
}

//------------------------------------------------------------------------------
// ODPart: BeginRelinquishFocus
//------------------------------------------------------------------------------

SOM_Scope ODBoolean SOMLINK ODPartBeginRelinquishFocus(ODPart *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* ownerFrame,
		ODFrame* proposedFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","BeginRelinquishFocus");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODFalse;
}

//------------------------------------------------------------------------------
// ODPart: CommitRelinquishFocus
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartCommitRelinquishFocus(ODPart *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* ownerFrame,
		ODFrame* proposedFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","CommitRelinquishFocus");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: AbortRelinquishFocus
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartAbortRelinquishFocus(ODPart *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* ownerFrame,
		ODFrame* proposedFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","AbortRelinquishFocus");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: FocusAcquired
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartFocusAcquired(ODPart *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* ownerFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","FocusAcquired");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}


//------------------------------------------------------------------------------
// ODPart: FocusLost
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartFocusLost(ODPart *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* ownerFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","FocusLost");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}


//------------------------------------------------------------------------------
// ODPart: ExternalizeKinds
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartExternalizeKinds(ODPart *somSelf, Environment *ev,
		ODTypeList* kindset)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ExternalizeKinds");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}


//------------------------------------------------------------------------------
// ODPart: ChangeKind
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartChangeKind(ODPart *somSelf, Environment *ev,
		ODType kind)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ChangeKind");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: HandleEvent
//------------------------------------------------------------------------------

SOM_Scope ODBoolean SOMLINK ODPartHandleEvent(ODPart *somSelf, Environment *ev,
		ODEventData* event,
		ODFrame* frame,
		ODFacet* facet,
		ODEventInfo* eventInfo)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","HandleEvent");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODFalse;
}

//------------------------------------------------------------------------------
// ODPart: AdjustMenus
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartAdjustMenus(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","AdjustMenus");

	WARNMSG(WARN_INDEX(AMSG_760),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

//------------------------------------------------------------------------------
// ODPart: UndoAction
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartUndoAction(ODPart *somSelf, Environment *ev,
		ODActionData* actionState)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","UndoAction");

	ODSetSOMException(ev, kODErrDoesNotUndo);
}

//------------------------------------------------------------------------------
// ODPart: RedoAction
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartRedoAction(ODPart *somSelf, Environment *ev,
		ODActionData* actionState)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","RedoAction");

	ODSetSOMException(ev, kODErrDoesNotUndo);
}

//------------------------------------------------------------------------------
// ODPart: DisposeActionState
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartDisposeActionState(ODPart *somSelf, Environment *ev,
		ODActionData* actionState,
		ODDoneState doneState)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DisposeActionState");

	ODSetSOMException(ev, kODErrDoesNotUndo);
}

//------------------------------------------------------------------------------
// ODPart: WriteActionState
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartWriteActionState(ODPart *somSelf, Environment *ev,
		ODActionData* actionState,
		ODStorageUnitView* storageUnitView)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","WriteActionState");

	ODSetSOMException(ev, kODErrDoesNotUndo);
}

//------------------------------------------------------------------------------
// ODPart: ReadActionState
//------------------------------------------------------------------------------

SOM_Scope ODActionData SOMLINK ODPartReadActionState(ODPart *somSelf, Environment *ev,
		ODStorageUnitView* storageUnitView)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ReadActionState");

	ODSetSOMException(ev, kODErrDoesNotUndo);
	ODActionData data;
	return data;
}

//------------------------------------------------------------------------------
// ODPart: FulfillPromise
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPartFulfillPromise(ODPart *somSelf, Environment *ev,
		ODStorageUnitView* promiseSUView)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","FulfillPromise");

	ODSetSOMException(ev, kODErrNoPromises);
}

//------------------------------------------------------------------------------
// ODPart: DropCompleted
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODPartDropCompleted(ODPart *somSelf, Environment *ev,
		ODPart* destPart,
		ODDropResult dropResult)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DropCompleted");

	ODSetSOMException(ev, kODErrDoesNotDrop);
}


//------------------------------------------------------------------------------
// ODPart: DragEnter
//------------------------------------------------------------------------------

SOM_Scope ODDragResult  SOMLINK ODPartDragEnter(ODPart *somSelf, Environment *ev,
		ODDragItemIterator* dragInfo,
		ODFacet* facet,
		ODPoint* where)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DragEnter");

	ODSetSOMException(ev, kODErrDoesNotDrop);
	return kODFalse;
}


//------------------------------------------------------------------------------
// ODPart: DragWithin
//------------------------------------------------------------------------------

SOM_Scope ODDragResult  SOMLINK ODPartDragWithin(ODPart *somSelf, Environment *ev,
		ODDragItemIterator* dragInfo,
		ODFacet* facet,
		ODPoint* where)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DragWithin");

	ODSetSOMException(ev, kODErrDoesNotDrop);
	return kODFalse;
}

//------------------------------------------------------------------------------
// ODPart: DragLeave
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartDragLeave(ODPart *somSelf, Environment *ev,
		ODFacet* facet,
		ODPoint* where)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","DragLeave");

	ODSetSOMException(ev, kODErrDoesNotDrop);
}

//------------------------------------------------------------------------------
// ODPart: Drop
//------------------------------------------------------------------------------

SOM_Scope ODDropResult SOMLINK ODPartDrop(ODPart *somSelf, Environment *ev,
		ODDragItemIterator* dropInfo,
		ODFacet* facet,
		ODPoint* where)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","Drop");

	ODSetSOMException(ev, kODErrDoesNotDrop);
	return kODDropFail;
}

//------------------------------------------------------------------------------
// ODPart: CreateLink
//------------------------------------------------------------------------------

SOM_Scope ODLinkSource* SOMLINK ODPartCreateLink(ODPart *somSelf, Environment *ev,
		ODByteArray* data)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","CreateLink");

	ODSetSOMException(ev, kODErrDoesNotLink);
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: RevealLink
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartRevealLink(ODPart *somSelf, Environment *ev,
		ODLinkSource* linkSource)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","RevealLink");

	ODSetSOMException(ev, kODErrDoesNotLink);
}

//------------------------------------------------------------------------------
// ODPart: LinkUpdated
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartLinkUpdated(ODPart *somSelf, Environment *ev,
		ODLink* updatedLink,
		ODUpdateID change)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","LinkUpdated");

	ODSetSOMException(ev, kODErrDoesNotLink);
}

//------------------------------------------------------------------------------
// ODPart: EmbeddedFrameUpdated
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartEmbeddedFrameUpdated(ODPart *somSelf, Environment *ev,
		ODFrame* frame,
		ODUpdateID change)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","EmbeddedFrameUpdated");

	// The default behavior for this method is to do nothing.  The reason
	// is that a part which does not support linking may be embedded within
	// a part which publishes a link source, and then itself embed a part which
	// calls ODFrame::ContentUpdated.  This part will be notified that the
	// EmbeddedFrameUpdated, and the correct thing to do is nothing.  Only
	// parts which want to do something special in response to this notification
	// need to override this method.
}

//------------------------------------------------------------------------------
// ODPart: EditInLinkAttempted
//------------------------------------------------------------------------------

SOM_Scope ODBoolean SOMLINK ODPartEditInLinkAttempted(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","EditInLinkAttempted");

	ODSetSOMException(ev, kODErrDoesNotLink);
	return kODFalse;
}

//------------------------------------------------------------------------------
// ODPart: RequestEmbeddedFrame
//------------------------------------------------------------------------------

SOM_Scope ODFrame* SOMLINK ODPartRequestEmbeddedFrame(ODPart *somSelf, Environment *ev,
		ODFrame* containingFrame,
		ODFrame* baseFrame,
		ODShape* frameShape,
		ODPart* embedPart,
		ODTypeToken viewType,
		ODTypeToken presentation,
		ODBoolean isOverlaid)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","RequestEmbeddedFrame");

	ODSetSOMException(ev, kODErrCannotEmbed);
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: RemoveEmbeddedFrame
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartRemoveEmbeddedFrame(ODPart *somSelf, Environment *ev,
		ODFrame* embeddedFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","RemoveEmbeddedFrame");

	ODSetSOMException(ev, kODErrCannotEmbed);
}

//------------------------------------------------------------------------------
// ODPart: RequestFrameShape
//------------------------------------------------------------------------------

SOM_Scope ODShape* SOMLINK ODPartRequestFrameShape(ODPart *somSelf, Environment *ev,
		ODFrame* embeddedFrame,
		ODShape* frameShape)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","RequestFrameShape");

	ODSetSOMException(ev, kODErrCannotEmbed);
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: UsedShapeChanged
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartUsedShapeChanged(ODPart *somSelf, Environment *ev,
		ODFrame* embeddedFrame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","UsedShapeChanged");

	ODSetSOMException(ev, kODErrCannotEmbed);
}


//------------------------------------------------------------------------------
// ODPart: AdjustBorderShape
//------------------------------------------------------------------------------

SOM_Scope ODShape* SOMLINK ODPartAdjustBorderShape(ODPart *somSelf, Environment *ev,
		ODFacet* embeddedFacet,
		ODShape* shape)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","AdjustBorderShape");

	ODSetSOMException(ev, kODErrCannotEmbed);
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: AcquireContainingPartProperties
//------------------------------------------------------------------------------

SOM_Scope ODStorageUnit* SOMLINK ODPartAcquireContainingPartProperties(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","AcquireContainingPartProperties");

	ODSetSOMException(ev, kODErrCannotEmbed);
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: RevealFrame
//------------------------------------------------------------------------------

SOM_Scope ODBoolean SOMLINK ODPartRevealFrame(ODPart *somSelf, Environment *ev,
		ODFrame* embeddedFrame,
		ODShape* revealShape)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","RevealFrame");

	ODSetSOMException(ev, kODErrCannotEmbed);
	return kODFalse;
}

//------------------------------------------------------------------------------
// ODPart: EmbeddedFrameSpec
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartEmbeddedFrameSpec(ODPart *somSelf, Environment *ev,
		ODFrame* embeddedFrame,
		ODObjectSpec* spec)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","EmbeddedFrameSpec");

	ODSetSOMException(ev, kODErrCannotEmbed);
}


//------------------------------------------------------------------------------
// ODPart: CreateEmbeddedFramesIterator
//------------------------------------------------------------------------------

SOM_Scope ODEmbeddedFramesIterator* SOMLINK ODPartCreateEmbeddedFramesIterator(ODPart *somSelf, Environment *ev,
		ODFrame* frame)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","CreateEmbeddedFramesIterator");

	ODSetSOMException(ev, kODErrCannotEmbed);
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: IsRealPart
//------------------------------------------------------------------------------

SOM_Scope ODBoolean SOMLINK ODPartIsRealPart(ODPart *somSelf, Environment *ev)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","IsRealPart");

	// actuall implementation, no need to override
	return kODTrue;
}

//------------------------------------------------------------------------------
// ODPart: GetRealPart
//------------------------------------------------------------------------------

SOM_Scope ODPart* SOMLINK ODPartGetRealPart(ODPart *somSelf, Environment *ev)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","GetRealPart");

	// actuall implementation, no need to override
	ODSetSOMException(ev, kODErrPartNotWrapper);
	return kODNULL;
}

//------------------------------------------------------------------------------
// ODPart: ReleaseRealPart
//------------------------------------------------------------------------------

SOM_Scope void SOMLINK ODPartReleaseRealPart(ODPart *somSelf, Environment *ev)
{
    /* ODPartData *somThis = ODPartGetData(somSelf); */
    ODPartMethodDebug("ODPart","ReleaseRealPart");

	// actuall implementation, no need to override
	ODSetSOMException(ev, kODErrPartNotWrapper);
}

//------------------------------------------------------------------------------
// InitPartName
//------------------------------------------------------------------------------

void InitPartName(Environment* ev, ODStorageUnit* su)
{
	if ( su->Exists(ev, kODPropName, kODNULL, 0) == kODFalse )
	{
		TempODIText itext = CreateIText(0);
		ODSetITextProp(ev, su, kODPropName, kODMacIText, itext);
	}
}

//=============================================================================
// M_ODPartcls methods
//=============================================================================

SOM_Scope ISOString  SOMLINK M_ODPartclsGetODPartHandlerName(M_ODPart *somSelf,
                                                              Environment *ev)
{
    /* M_ODPartData *somThis = M_ODPartGetData(somSelf); */
     M_ODPartMethodDebug("M_ODPart","M_ODPartclsGetODPartHandlerName");

    /* Return statement to be customized: */
    { ISOString retVal;  return (retVal); }
}
SOM_Scope string  SOMLINK M_ODPartclsGetODPartHandlerDisplayName
				(M_ODPart *somSelf, Environment *ev)
{
    /* M_ODPartData *somThis = M_ODPartGetData(somSelf); */
     M_ODPartMethodDebug("M_ODPart","M_ODPartclsGetODPartHandlerDisplayName");

    /* Return statement to be customized: */
    { string retVal;  return (retVal); }
}

SOM_Scope _IDL_SEQUENCE_PartKindInfo  SOMLINK M_ODPartclsGetODPartKinds
				(M_ODPart *somSelf, Environment *ev)
{
    /* M_ODPartData *somThis = M_ODPartGetData(somSelf); */
     M_ODPartMethodDebug("M_ODPart","M_ODPartclsGetODPartKinds");

    /* Return statement to be customized: */
    { _IDL_SEQUENCE_PartKindInfo retVal;  return (retVal); }
}

SOM_Scope string SOMLINK M_ODPartclsGetOLE2ClassId(M_ODPart *somSelf,
                                                  Environment *ev)
{
	/* M_ODPartData *somThis = M_ODPartGetData(somSelf); */
        M_ODPartMethodDebug("M_ODPart","M_ODPartclsGetOLE2ClassId");

        /* Return statement to be customized:  */
        { string retVal;  return (retVal); }
}

SOM_Scope string  SOMLINK M_ODPartclsGetWindowsIconFileName(M_ODPart *somSelf,
                                                              Environment *ev)
{
    /* M_ODPartData *somThis = M_ODPartGetData(somSelf); */
    M_ODPartMethodDebug("M_ODPart","M_ODPartclsGetWindowsIconFileName");

    /* Return statement to be customized: */
    { string retVal;  return (retVal); }
}

SOM_Scope SOMObject *  SOMLINK M_ODPartsomNew(M_ODPart *somSelf)
{
    /* M_ODPartData *somThis = M_ODPartGetData(somSelf); */
    M_ODPartMethodDebug("M_ODPart","M_somNew");


    return M_ODPart_parent_SOMClass_somNew(somSelf);

}

