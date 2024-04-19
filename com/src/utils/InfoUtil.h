/* @(#)Z 1.3 com/src/utils/include/InfoUtil.h, odcore, od96os2, odos29646d 96/11/15 15:28:59 (96/07/15 18:15:52) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odcore
 *
 *   CLASSES: none
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */
/*
	File:		InfoUtil.h

	Contains:	function declarations Info getters & setters

	Owned by:	Tantek ‚elik

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/


#ifndef _INFOUTIL_
#define _INFOUTIL_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#if !defined(_PLATFORM_WIN32_)
#ifndef __CONTROLS__
#include <Controls.h>
#endif
#endif // !defined(_PLATFORM_WIN32_)

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include "NmSpcMg.xh"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#if defined(_PLATFORM_WIN32_)
#ifndef _ODTYPESP_
#include <ODTypesP.h>
#endif
#endif // defined(_PLATFORM_WIN32_)
//==============================================================================
// Theory of Operation
//==============================================================================

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

//==============================================================================
// Classes used by this interface
//==============================================================================

class	ODFrame;
class	ODStorageUnit;
class	ODPart;
class	ODPersistentObject;
class	ODTypeList;
class	EditorSet;
class	ODSession;
class	PlatformFile;

//==============================================================================
// Info functions
//==============================================================================

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

extern "C" {



//-------------------------------------------------------------------------------------
// Persistent Object Property getters and setters
// Note: 
//  all Getters expect you to pass in a buffer to be filled 
//  or kODNULL which causes it to create one for you
//  all Setters do NOT consume what you pass in, so it is safe to pass in
// 	a reference to your internal structures.  They are only accessed for the duration
// 	of the Setter API call.
// These conventions were adopted to reduce memory allocation/deallocation.
//-------------------------------------------------------------------------------------

ODULong		ODGetCreationDate(Environment* ev, 
				ODStorageUnit* su);
void		ODSetCreationDate(Environment* ev, 
				ODStorageUnit* su,
				ODTime dateTime);
				
ODULong		ODGetModificationDate(Environment* ev, 
				ODStorageUnit* su);
void		ODSetModificationDate(Environment* ev, 
				ODStorageUnit* su,
				ODTime dateTime);
				
ODIText*	ODGetModifiedBy(Environment* ev, 
				ODStorageUnit* su,ODIText* userName);
void		ODSetModifiedBy(Environment* ev, 
				ODStorageUnit* su, ODIText* userName);


ODStorageUnit* ODGetSUFromPstObj(Environment* ev, ODPersistentObject* pstobj);

ODIText*	ODGetPOComments(Environment* ev,  ODPart* part, ODIText* comments);
void		ODSetPOComments(Environment* ev, ODPart* part, ODIText* comments);

ODIconFamily	ODGetPOIconFamily(Environment* ev,
					ODPart* part);
void		ODSetPOIconFamily(Environment* ev, 
					ODPart* part, ODIconFamily iconFamily,
					ODBoolean deleteOtherPlatformIcons =kODTrue);

ODULong		ODGetPOSize(Environment* ev, 
				ODPersistentObject* pstobj);
ODID		ODGetPOID(Environment* ev, 
				ODPersistentObject* pstobj);

// Note: the next two functions are lowlevel functions for directly setting
// and getting the name property of persistent objects.  
// If you are dealing with a part, please use ODGetPartName & ODSetPartName
// in order to make sure the right thing happens with respect to 
// the document file, titles of windows etc.  -Tantek
ODIText*	ODGetPOName(Environment* ev, 
						ODPersistentObject* pstobj,ODIText* name);
void		ODSetPOName(Environment* ev, 
						ODPersistentObject* pstobj,ODIText* name);

// The next function is used when the persistent object is not internalized yet. - Vincent
void		ODSetPONameUsingSU(Environment* ev, ODStorageUnit* su, ODIText* name);

//-------------------------------------------------------------------------------------
// Part only Property getters and setters
//-------------------------------------------------------------------------------------

ODIText*	ODGetPartName(Environment* ev, 
				ODFrame* frame,ODIText* name);
		// Returns the name of the part of the frame passed in.

ODBoolean	ODSetPartName(Environment* ev, 
				ODFrame* frame, ODIText* name, DescType replaceOption);
		// Renames the part of the frame passed in.
		// Returns whether or not it succeeded.
		// The frame helps more easily determine whether
		// the part is the root part of the document or not.
		// For example, renaming the root part of the document could fail
		// if there was a file with the same name and the user canceled the
		// "Replace?" dialog.

void		ODRenamePartWindows( Environment *ev, ODSession *session, ODPart *part,
								 ODIText *oldName, ODIText *name );
		// Renames windows with 'part' as their root part. Subroutine used
		// by ODSetPartName; not usually needed for other purposes (although
		// the Shell does use it.)


ODIText*	ODGetComments(Environment* ev,  ODFrame* frame, ODIText* comments);
void		ODSetComments(Environment* ev, ODFrame* frame, ODIText* comments);

ODName*		ODGetCategory(Environment* ev, ODPart* part, ODNameSpaceManager* nsm);
ODName*		ODGetCatFromPartSU(Environment* ev, ODStorageUnit* su, ODNameSpaceManager* nsm);
ODName*		ODGetCatFromKind(Environment* ev, ODType kind, ODNameSpaceManager* nsm);

ODType		ODGetKind(Environment* ev, ODPart* part);
ODType		ODGetKindFromPartSU(Environment* ev, ODStorageUnit* su);
ODPlatformType ODGetIconFilePlatformTypeFromPartSU(Environment* ev, 
		ODStorageUnit* su);

ODIconFamily	ODGetIconFamily(Environment* ev,
					ODFrame* frame);
void		ODSetIconFamily(Environment* ev, 
					ODFrame* frame, ODIconFamily iconFamily,
					ODBoolean deleteOtherPlatformIcons =kODTrue);

ODBoolean	ODGetIsStationery(Environment* ev, 
				ODFrame* frame);
void		ODSetIsStationery(Environment* ev, 
				ODFrame* frame, ODBoolean isStationery);

ODBoolean		ODGetSUIsStationery(Environment* ev, ODStorageUnit* su);
void		ODSetSUIsStationery(Environment* ev, 
				ODStorageUnit* su, ODBoolean isStationery);

//-------------------------------------------------------------------------------------
// Frame only Property getters and setters
//-------------------------------------------------------------------------------------
// see Frame.idl


//-------------------------------------------------------------------------------------
// General Utility functions
//-------------------------------------------------------------------------------------

void SetAllWindowShowLinks(Environment* ev, ODWindowState* winState, ODBoolean showLinks);
PlatformFile*	ODGetFileIfRoot(Environment* ev, ODFrame* frame);

ODBoolean	ODAskUserReplace(Environment* ev,
							ODSession* session);
// Pass in the name in question via ParamText(^0).

//-------------------------------------------------------------------------------------
// Editor and Kind popup Menu manipulation functions
// copied from LinkDlgs.cpp (!with changes by T‚ and CG)
//-------------------------------------------------------------------------------------
// private by convention.

void AddTypesToMenu(ODTypeList* kindList, 
								MenuHandle kindMenu, 
								ODSession* session);
ODUShort EnableTypesInMenu(ODTypeList* typeList,
								MenuHandle kindMenu,
								ODEditor editor,
								ODNameSpaceManager* nsm);
ODBoolean SetupEditorMenu(ODType kind,
								EditorSet* editorList,
								MenuHandle editorMenu, 
								ControlHandle popupCtlHndl, 
								ODSession* session);
void AddEditorsToMenu(EditorSet* editorList,
								MenuHandle editorMenu, 
								ODSession* session);
void ContentValueTypes(ODStorageUnit* contentSU, ODTypeList* typeList);
ODEditor GetThisEditorFromList(ODSShort editorIndex, EditorSet* editorList);
ODType GetThisKindFromList(ODSShort kindItem, ODTypeList* kindList);
ODSShort IndexOfEditorInList(EditorSet* editorsList, ODEditor editor);
ODSShort IndexOfKindInList(ODTypeList* kindList, ODType kind);
void TranslateValueTypes(ODTypeList* kindList,
								ODTypeList* translateToList,
								OrderedCollection* translateFromList,
								ODSession* session);
void AddTranslationKindToMenu(ODType		kind, 
								MenuHandle	kindMenu,
								short		item,
								ODBoolean	insert,
								ODSession*	session);
}	// End of extern "C" {

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif

#endif // _INFOUTIL_
