/* @(#)Z 1.3 com/src/utils/InfoUtil.cpp, odcore, od96os2, odos29646d 96/11/15 15:28:44 (96/07/15 18:15:47) */
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
	File:		InfoUtil.cpp

	Contains:	implementation of Info getters & setters

	Owned by:	Tantek ‚elik

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

#if !defined(_PLATFORM_WIN32_)
#ifndef _USERSRCM_
#include "UseRsrcM.h"
#endif
#endif // !defined(_PLATFORM_WIN32_)

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
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

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _DLOGUTIL_
#include <DlogUtil.h>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODBinding_xh
#include <ODBindng.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#if !defined(_PLATFORM_WIN32_)
#ifdef __SC__
#ifndef __PACKAGES__
#include <Packages.h>
#endif
#else
#ifndef __TEXTUTILS__
#include <TextUtils.h>
#endif
#endif

#ifndef __STRING__
#include <String.h>
#endif

#ifndef __GESTALTEQU__
#include <GestaltEqu.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
#endif

#ifndef __CONTROLS__
#include <Controls.h>
#endif

#ifndef __ICONS__
#include <Icons.h>
#endif

#ifndef __FINDER__
#include <Finder.h>
#endif
#endif // !defined(_PLATFORM_WIN32_)

#ifndef _INFOUTIL_
#include "InfoUtil.h"
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef _EDITRSET_
#include "EditrSet.h"
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef SOM_ODWindowIterator_xh
#include <WinIter.xh>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODValueNameSpace_xh
#include <ValueNS.xh>
#endif

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#ifndef _UTILDEFS_
#include "UtilDefs.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#pragma segment Info

//==============================================================================
// Constants
//==============================================================================

const short kControlInactive = 255;
const short kControlActive = 0;
static const char* const 		kODIconFamilyBaseString = kODIconFamily ":";
#if !defined(_PLATFORM_WIN32_)
static const ResType			kIconType[6] = {'ICN#','icl4','icl8','ics#','ics4','ics8'};
#endif // !defined(_PLATFORM_WIN32_)
static const short				kIconSize[6] = {256,   512,   1024,  64,    128,   256   };

#if !defined(_PLATFORM_WIN32_)
const ODPlatformType kODFileType_hfs = 0x68667320;	// 'hfs '
#else // defined(_PLATFORM_WIN32_)
// Talk to Leigh Williamson about the definition of ODPlatformType
const ODPlatformType kODFileType_hfs = (ODPlatformType) 0x68667320;	// 'hfs '
#endif // !defined(_PLATFORM_WIN32_)

//==============================================================================
// Static Function declarations
//==============================================================================

ODTime	ODGetDateProperty(Environment* ev, 
			ODStorageUnit* su,
			ODPropertyName prop);

void	ODSetDateProperty(Environment* ev, 
			ODStorageUnit* su,
			ODPropertyName prop,
			ODTime dateTime);

ODStatic ODBoolean 		ODUserRenameFile(Environment* ev,
								ODSession* session,
								PlatformFile*	usersFile, 
								ODIText* name,
								DescType replaceOption);
// If PlatformFile is made a shared utility, then the above function should probably be
// integrated into the PlatformFile::Rename method.

ODStatic ODIconFamily	GetGenericIcon( short iconID );

ODStatic OSErr 			DetachIconProc( ResType, Handle *theIcon, void */*yourDataPtr*/ );

ODStatic ODBoolean		CommentsDontMatch(ODIText* dtComments, ODIText* propComments);

//==============================================================================
// Info functions
//==============================================================================


ODStorageUnit* ODGetSUFromPstObj(Environment* ev, ODPersistentObject* pstobj)
{
	return pstobj->GetStorageUnit(ev);
}

//-------------------------------------------------------------------------------------
// Part & Frame Property getters and setters
//-------------------------------------------------------------------------------------

ODIText*
ODGetPOComments(Environment* ev, ODPart *part, ODIText* comments)
{
	ODStorageUnit*	su = ODGetSUFromPstObj(ev, part);
	ODIText*		propComments;

	propComments = ODGetITextProp(ev, su, kODPropComments, kODMacIText, comments);
	return propComments;
}

void	ODSetPOComments(Environment* ev, ODPart *part, ODIText* comments)
{
	ODStorageUnit*	su = ODGetSUFromPstObj(ev, part);
	
	ODSetITextProp(ev, su, kODPropComments, kODMacIText, comments);
}


ODIText*
ODGetComments(Environment* ev, ODFrame* frame, ODIText* comments)
{
	TempODPart		part = frame->AcquirePart(ev);
	ODIText*		propComments;
	ODIText*		dtComments;

#if !defined(_PLATFORM_WIN32_)
	propComments = ODGetPOComments(ev, part, comments);
	if (comments && !propComments)
		propComments = comments;
	TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
	if (usersFile)
	{
		dtComments = usersFile->GetComments();
		if (dtComments && GetITextStringLength(dtComments) == 0)
		{
			// since there are no desktop comments
			// make the desktop comments match the property
			usersFile->SetComments(propComments);
		}
		else if (CommentsDontMatch(dtComments, propComments) && dtComments)
		{
			// if there are desktop comments and the property comments don't match
			// then the desktop comments take precedence, so return the desktop comments
			DisposeIText(propComments);
			propComments = dtComments;
			dtComments = kODNULL;
		}
		DisposeIText(dtComments);
	}
#endif // !defined(_PLATFORM_WIN32_)
	return propComments;
}

void	ODSetComments(Environment* ev, ODFrame* frame, ODIText* comments)
{
#if !defined(_PLATFORM_WIN32_)
	TempODPart		part = frame->AcquirePart(ev);
	ODSetPOComments(ev, part, comments);
	TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
	if (usersFile)
		usersFile->SetComments(comments);
#endif // !defined(_PLATFORM_WIN32_)
}


ODULong	ODGetPOSize(Environment* ev, ODPersistentObject* pstobj)
{
	ODStorageUnit* su = ODGetSUFromPstObj(ev, pstobj);
	su->Focus(ev, kODNULL, kODPosAll, kODNULL,0, kODPosAll);
	return su->GetSize(ev);
}

ODID ODGetPOID(Environment* ev, ODPersistentObject* pstobj)
{
	ODStorageUnit* su = ODGetSUFromPstObj(ev, pstobj);
	su->Focus(ev, kODNULL, kODPosAll, kODNULL,0, kODPosAll);
	return su->GetID(ev);
}

ODTime	ODGetDateProperty(Environment* ev, 
			ODStorageUnit* su,
			ODPropertyName prop)
{	return ODGetTime_TProp(ev, su, prop, kODTime_T);}

void	ODSetDateProperty(Environment* ev, 
			ODStorageUnit* su,
			ODPropertyName prop,
			ODTime dateTime)
{	ODSetTime_TProp(ev, su, prop, kODTime_T, dateTime);}


ODULong	ODGetCreationDate(Environment* ev, 
				ODStorageUnit* su)
{	return ODGetDateProperty(ev, su, kODPropCreateDate);}

void	ODSetCreationDate(Environment* ev, 
				ODStorageUnit* su,
				ODTime dateTime)
{	ODSetDateProperty(ev, su, kODPropCreateDate, dateTime);}


ODULong	ODGetModificationDate(Environment* ev, 
				ODStorageUnit* su)
{	return ODGetDateProperty(ev, su, kODPropModDate);}

void	ODSetModificationDate(Environment* ev, 
				ODStorageUnit* su,
				ODTime dateTime)
{	ODSetDateProperty(ev, su, kODPropModDate, dateTime);}


ODIText*	ODGetModifiedBy(Environment* ev, 
				ODStorageUnit* su, ODIText* userName)
{
	return ODGetITextProp(ev, su, kODPropModUser,
			kODMacIText, userName);
}

void		ODSetModifiedBy(Environment* ev, 
				ODStorageUnit* su, ODIText* userName)
{
	ODSetITextProp(ev, su, kODPropModUser,
			kODMacIText, userName);
}


//-------------------------------------------------------------------------------------
// Part only Property getters and setters
//-------------------------------------------------------------------------------------
ODIText*	ODGetPartName(Environment* ev, 
				ODFrame* frame, ODIText* name)
{
	TempODPart part = frame->AcquirePart(ev);
	TempODIText partName = ODGetPOName(ev, part, name);
	
	TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
	if (usersFile)
	{
		TempODIText fileName = usersFile->GetName();
		if ( CommentsDontMatch( fileName, partName ) )
			return CopyIText( fileName );
	}
	return CopyIText( partName );
}

ODBoolean	ODSetPartName(Environment* ev, 
				ODFrame* frame, ODIText* name, DescType replaceOption)
		// Renames the part of the frame passed in.
		// Returns whether or not it succeeded.
		// The frame helps more easily determine whether
		// the part is the root part of the document or not.
		// For example, renaming the root part of the document could fail
		// if there was a file with the same name and the user canceled the
		// "Replace?" dialog.
{
	ASSERT(frame != kODNULL, kODErrIllegalNullInput);
	
	TempODPart part = frame->AcquirePart(ev);
	TempODWindow window = frame->AcquireWindow(ev);
				
	ODFrame* 	contFrame = frame->AcquireContainingFrame(ev);
	ODBoolean 	isRootPart = (contFrame == kODNULL);
	ODReleaseObject(ev, contFrame);
	
	ODBoolean	isRootPartOfDocument = isRootPart && window->IsRootWindow(ev);
	ODBoolean	succeeded = kODTrue;
	
#if !defined(_PLATFORM_WIN32_)
	TRY
		ODName* 		oldName = ODGetPartName(ev, frame, kODNULL); 
		ODStorageUnit*	partSU	= ODGetSUFromPstObj(ev, part);
		ODSession*		session = partSU->GetSession(ev);

		if (isRootPartOfDocument)
		{
			ODDraft*		draft = partSU->GetDraft(ev);
			PlatformFile*	usersFile = 
				GetPlatformFileFromContainer(ev, draft->GetDocument(ev)->GetContainer(ev));

			if (ODUserRenameFile(ev, session, usersFile, name, replaceOption))
			{
				ODWindowState* windowState = session->GetWindowState(ev);
				windowState->SetDefaultWindowTitles(ev, draft);
				ODSetPOName(ev, part, name);
				ODRenamePartWindows(ev,session,part,oldName,name);
				if (isRootPart)
				{
					Str255 pName;
					GetITextPString(name, pName);
					SetWTitle(window->GetPlatformWindow(ev), pName);
				}
			}
			else
				succeeded = kODFalse;
			ODDeleteObject(usersFile);
		}
		else
		{
			ODSetPOName(ev, part,name);
			ODRenamePartWindows(ev,session,part,oldName,name);
		}
		
		DisposeIText(oldName);

	CATCH_ALL
		WARN("ODSetPartName failed, error %ld",ErrorCode());
		succeeded = kODFalse;
	ENDTRY
	
#endif // !defined(_PLATFORM_WIN32_)
	return succeeded;
}

void
ODRenamePartWindows( Environment *ev, ODSession *session,
					 ODPart *part, ODIText *oldName, ODIText *name )
{
	// go through all windows which have this part as their root, and
	// have the current name of the part as a prefix
	// rename their prefix to be the new name
	Str255	pOldName;
	Str255	pNewName;
	Str255	pWinTitle;
	ODUByte pOldNameLen;
	ODUByte pNewNameLen;
	
	GetITextPString(oldName, pOldName);
	pOldNameLen = pOldName[0];
	GetITextPString(name, pNewName);
	pNewNameLen = pNewName[0];
	
#if !defined(_PLATFORM_WIN32_)
	ODWindowIterator* iter = session->GetWindowState(ev)->CreateWindowIterator(ev);
	for (ODWindow* window = iter->First(ev); 
		 iter->IsNotComplete(ev);
		 window = iter->Next(ev))
	{
		TempODPart winPart = window->GetRootFrame(ev)->AcquirePart(ev);
		if (ODObjectsAreEqual(ev, part, winPart))
		{
			// Check to see if name of window has pOldName as prefix
			ODPlatformWindow pwindow = window->GetPlatformWindow(ev);
			GetWTitle(pwindow, pWinTitle);
			if (!strncmp((char*)&pWinTitle[1], (char*)&pOldName[1], pOldNameLen) &&
					!EqualPascalStrings(pWinTitle, pNewName))
			{
				ODUByte suffixLen = pWinTitle[0] - pOldNameLen;
				if (suffixLen + pNewNameLen > 255) 
					suffixLen = 255 - pNewNameLen;
				strncpy((char*)&pNewName[pNewNameLen+1], 
						(char*)&pWinTitle[pOldNameLen+1],
						suffixLen);
				pNewName[0] = pNewNameLen+suffixLen;
				SetWTitle(pwindow, pNewName);
			}
		}
	}
	ODDeleteObject(iter);
#endif // !defined(_PLATFORM_WIN32_)
}


ODIText*	ODGetPOName(Environment* ev, ODPersistentObject* pstobj,ODIText* name)
{
	return ODGetITextProp(ev, ODGetSUFromPstObj(ev, pstobj), kODPropName, kODMacIText,name);
}

void		ODSetPOName(Environment* ev, ODPersistentObject* pstobj,ODIText* name)
{
	ODSetITextProp(ev, ODGetSUFromPstObj(ev, pstobj), kODPropName, kODMacIText, name);
}

void		ODSetPONameUsingSU(Environment* ev, ODStorageUnit* su, ODIText* name)
{
	ODSetITextProp(ev, su, kODPropName, kODMacIText, name);
}

ODName*	ODGetCategory(Environment* ev, ODPart* part, ODNameSpaceManager* nsm)
{
	return ODGetCatFromPartSU(ev, ODGetSUFromPstObj(ev, part), nsm);
}

ODName*	ODGetCatFromPartSU(Environment* ev, ODStorageUnit* su, ODNameSpaceManager* nsm)
{
	ODType kind = ODGetKindFromPartSU(ev, su);
	ODName*	categoryName = ODGetCatFromKind(ev, kind, nsm);
	ODDisposePtr( kind );

	return categoryName;
}

ODName*	ODGetCatFromKind(Environment* ev, ODType kind, ODNameSpaceManager* nsm)
{
	StringHandle strHandle = 0;
	ODTypeList* categoryList = GetCategoriesOfKind(nsm, kind);
	ODTypeListIterator* catIter = kODNULL;
	ODType category = kODNULL;
	ODName*	categoryName = kODNULL;
	

#if !defined(_PLATFORM_WIN32_)
	if (categoryList)
	{
		catIter = categoryList->CreateTypeListIterator(ev);
		category = catIter->First(ev);
		ODDeleteObject(catIter);
	}
	
	if (category == kODNULL || !GetUserCatFromCat(nsm, category, &categoryName)) {
		CUsingLibraryResources r;
		categoryName = GetODIText(kODPartInfoStrUnknownID);
	}
	//else // make a disposable copy // NO NEED TO! GetUserCatFromCat returns a copy!
	//	categoryName = CopyIText(categoryName);
		
	ODDisposePtr(category);

#endif // !defined(_PLATFORM_WIN32_)
	return categoryName;
}

ODType	ODGetKindFromPartSU(Environment* ev, ODStorageUnit* su)
{
	ODULong unused;
	ODType kind = ODGetISOStrProp(ev, su, kODPropPreferredKind, kODISOStr, kODNULL, &unused);
	if ( kind == kODNULL )
		if ( su->Exists(ev, kODPropContents, (ODValueType) kODNULL,1) )
		{
			su->Focus(ev, kODPropContents, kODPosSame, (ODValueType) kODNULL,1, kODPosSame);
			kind = su->GetType(ev);

			ODTranslation* translation = su->GetSession(ev)->GetTranslation(ev);
			TempODType hfsType = 
				translation->GetISOTypeFromPlatformType(ev, kODFileType_hfs, kODPlatformDataType);

			if ( ODISOStrEqual(kind, hfsType) )
			{
				ODDisposePtr(kind);
				kind = kODNULL;
				if ( su->Exists(ev, kODPropContents, (ODValueType) kODNULL,2) )
				{
					su->Focus(ev, kODPropContents, kODPosSame, (ODValueType) kODNULL,2, kODPosSame);
					kind = su->GetType(ev);
				}
			}
		}
	return kind;
}

ODType	ODGetKind(Environment* ev, ODPart* part)
{
	return ODGetKindFromPartSU(ev, ODGetSUFromPstObj(ev, part));
}

ODPlatformType ODGetIconFilePlatformTypeFromPartSU(Environment* ev, ODStorageUnit* su)
{
	ODSession* session = su->GetSession(ev);
	
	ODType partKind = ODGetKindFromPartSU(ev, su);
	ODPlatformType partOSType = session->GetTranslation(ev)->
							GetPlatformTypeFromISOType(ev, partKind);
#if !defined(_PLATFORM_WIN32_)
	if (partOSType != kODNULL)
	{
		((ODUByte*)(&partOSType))[0] = kODPlatformKindFileChar1;
	}
	else
	{
		ODBoolean	partOSTypeFound = kODFalse;
		ODValueNameSpace*	osTypeNameSpace = (ODValueNameSpace*)
			(session->GetNameSpaceManager(ev)->HasNameSpace(ev, kODKindOldMacOSType));
	
		if (osTypeNameSpace)
		{
			ODPlatformType* typePtr;
			ODULong		unusedLength;
			partOSTypeFound = ValueNameSpaceGetEntry(osTypeNameSpace, 
														ev, partKind, 
														(ODPtr*) &typePtr, 
														&unusedLength);
			if (partOSTypeFound)
			{
				partOSType = *typePtr;
				ODDeleteObject( typePtr );
			}
		}
		if (!partOSTypeFound)
			partOSType = kODShellSignature;
	}
	
#endif // !defined(_PLATFORM_WIN32_)
	ODDisposePtr(partKind);

	return partOSType;
}


//--------------------------------------------------------------------
// ODGetPOIconFamily
//--------------------------------------------------------------------

ODIconFamily
ODGetPOIconFamily(Environment* ev, ODPart* part)
{
#if !defined(_PLATFORM_WIN32_)
	//// FIRST, get the Mac icon value if one exists:
	ODStorageUnit*	su = ODGetSUFromPstObj(ev, part);
	ODIconFamily icons = ODGetIconFamilyProp(ev, su, 
								kODPropCustomIcon, kODIconFamily, kAllIconsMask);
	if (icons )
		return icons;
		
	//// OTHERWISE, look for other platform icon values:
	if (ODSUExistsThenFocus(ev,su, kODPropCustomIcon, kODNULL))
	{
		ODULong numValues = su->CountValues(ev);
		for( ODULong i = 1; i <= numValues; i++ )
		{
			su->Focus(ev, kODNULL, kODPosSame, kODNULL, 0, kODPosNextSib);	// Next value
			TempODValueType type = su->GetType(ev);
			if( strncmp(type,kODIconFamilyBaseString,strlen(kODIconFamilyBaseString)) == 0 )
			{
				// This value is an icon family for some platform. Use the b/w icons
				// because we don't know what color table the color ones may use.
				icons = ODGetIconFamilyProp(ev, su, kODPropCustomIcon, type, kBWIconsMask);
				if( icons )
					return icons;
			}
		}
		return kODNULL;
	}
	return icons;
#else // defined(_PLATFORM_WIN32_)
	return NULL;
#endif // !defined(_PLATFORM_WIN32_)
}



//--------------------------------------------------------------------
// ODSetPOIconFamily
//--------------------------------------------------------------------

void	ODSetPOIconFamily(Environment* ev, ODPart* part, 
			ODIconFamily icons, ODBoolean deleteOtherPlatformIcons)
{
#if !defined(_PLATFORM_WIN32_)
	ODSetIconFamilyProp(ev, ODGetSUFromPstObj(ev, part), 
								kODPropCustomIcon, kODIconFamily,
								icons, deleteOtherPlatformIcons);
#endif // !defined(_PLATFORM_WIN32_)
}

//--------------------------------------------------------------------
// ODGetIconFamily
//--------------------------------------------------------------------

ODIconFamily
ODGetIconFamily(Environment* ev, ODFrame* frame)
{
	TempODPart		part = frame->AcquirePart(ev);
	ODStorageUnit*	su = ODGetSUFromPstObj(ev, (ODPart*)part);
	ODIconFamily	icons = kODNULL;
	
#if !defined(_PLATFORM_WIN32_)
	// FIRST we check for the custom icon resources
	TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
	if (usersFile)
		icons = usersFile->GetCustomIconFamily();
	if (icons)
		return icons;
	
	//// THEN get the property icon value if one exists:
	icons = ODGetPOIconFamily(ev, part);
	if (icons)
		return icons;

	//// THEN get the editor's icon from the Mac desktop database:
	ODPlatformType fileType = ODGetIconFilePlatformTypeFromPartSU(ev, su);
	ODPlatformType fileCreator = kODShellSignature;
	
	// Query the desktop database:
	DTPBRec pb;
	pb.ioNamePtr = kODNULL;
	pb.ioVRefNum = -1; //spec.vRefNum;
	THROW_IF_ERROR( PBDTGetPath( &pb ) );		// Sets pb.ioDTRefNum
	
	THROW_IF_ERROR( NewIconSuite(&icons) );
	
	ODBoolean hasIcons = kODFalse;
	TRY{
		char iconBuffer[kLarge8BitIconSize];
		for( pb.ioIconType=1; pb.ioIconType<=6; pb.ioIconType++ ) {
			pb.ioTagInfo = 0;
			pb.ioDTBuffer = (Ptr)&iconBuffer;
			pb.ioDTReqCount = sizeof(iconBuffer);
			pb.ioFileCreator = fileCreator; //info.fdCreator;
			pb.ioFileType = fileType; //info.fdType;
			OSErr err= PBDTGetIconSync(&pb);
			
			if( err == noErr ) {
				Handle hIcon;
				WASSERT(pb.ioDTActCount==kIconSize[pb.ioIconType-1]);
				THROW_IF_ERROR( PtrToHand(iconBuffer,&hIcon, kIconSize[pb.ioIconType-1]) );
				ASSERT(hIcon!=NULL, kODErrAssertionFailed);
				ASSERT(*hIcon!=NULL, kODErrAssertionFailed);
				err= AddIconToSuite(hIcon,icons, kIconType[pb.ioIconType-1] );
				if( err ) {
					DisposeHandle(hIcon);
					THROW_IF_ERROR(err);
				}
				hasIcons = kODTrue;
			} else if( err != afpItemNotFound )
				THROW_IF_ERROR(err);
		}
	
	}CATCH_ALL{
		DisposeIconSuite(icons, kODTrue);
		RERAISE;
	}ENDTRY
	
	// FINALLY use a generic icon:
	if( !hasIcons ) {
		DisposeIconSuite(icons, kODTrue);

		short id;
				if( ODGetIsStationery(ev, frame) )
					id = genericStationeryIconResource;
				else
					id = genericDocumentIconResource;
				icons= GetGenericIcon(id);
			}
#endif // !defined(_PLATFORM_WIN32_)
	return icons;
}


//--------------------------------------------------------------------
// ODSetIconFamily
//--------------------------------------------------------------------

void	ODSetIconFamily(Environment* ev, 
				ODFrame* frame, ODIconFamily icons, ODBoolean deleteOtherPlatformIcons)
{
#if !defined(_PLATFORM_WIN32_)
	TempODPart 		part = frame->AcquirePart(ev);
	
	ODSetPOIconFamily(ev, part, icons, deleteOtherPlatformIcons);
	
	TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
	if (usersFile)
		usersFile->SetCustomIconFamily(icons);
#endif // !defined(_PLATFORM_WIN32_)
}

//--------------------------------------------------------------------
// ODGetFileIfRoot
//--------------------------------------------------------------------
PlatformFile*	ODGetFileIfRoot(Environment* ev, ODFrame* frame)
{
	PlatformFile*	usersFile = kODNULL;
	ODBoolean		isRootPartOfDocument;

	ASSERT(frame != kODNULL, kODErrIllegalNullInput);

	ODFrame* 	contFrame = frame->AcquireContainingFrame(ev);
	ODBoolean 	isRootPart = (contFrame == kODNULL);
	ODReleaseObject(ev, contFrame);

	{ TempODWindow window = frame->AcquireWindow(ev);
	  isRootPartOfDocument = isRootPart && window->IsRootWindow(ev);
	}
	
	if (isRootPartOfDocument)
	{
		ODDraft* draft;
		{ TempODPart part = frame->AcquirePart(ev);
		  draft = ODGetSUFromPstObj(ev, part)->GetDraft(ev);
		}
		usersFile = GetPlatformFileFromContainer(ev, 
								draft->GetDocument(ev)->GetContainer(ev));
	}
	return usersFile;
}


//--------------------------------------------------------------------
// DetachIconProc
//--------------------------------------------------------------------

static OSErr DetachIconProc( ResType, Handle *theIcon, void */*yourDataPtr*/ )
{
#if !defined(_PLATFORM_WIN32_)
	if( *theIcon ) {
		DetachResource(*theIcon);
		return ResError();
	} else
#endif // !defined(_PLATFORM_WIN32_)
		return noErr;
}

//--------------------------------------------------------------------
// GetGenericIcon
//--------------------------------------------------------------------

ODStatic ODIconFamily GetGenericIcon( short iconID )
{
	ODIconFamily icon;
#if !defined(_PLATFORM_WIN32_)
	THROW_IF_ERROR( GetIconSuite( &icon, iconID, svAllAvailableData ) );
	IconActionUPP detachProc = NewIconActionProc(&DetachIconProc);
	(void) ForEachIconDo(icon, svAllAvailableData, detachProc, kODNULL);
	DisposeRoutineDescriptor(detachProc);
#endif // !defined(_PLATFORM_WIN32_)
	return icon;
}


//--------------------------------------------------------------------
// ODGetIsStationery
//--------------------------------------------------------------------

ODBoolean	ODGetIsStationery(Environment* ev, 
				ODFrame* frame)
{
	TempODPart		part = frame->AcquirePart(ev);
	ODBoolean		isStationery;
	
	TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
	if (usersFile)
		isStationery = usersFile->IsStationery();
	else
		isStationery = ODGetSUIsStationery(ev, ODGetSUFromPstObj(ev, part));
	return isStationery;
}

//--------------------------------------------------------------------
// ODSetIsStationery
//--------------------------------------------------------------------

void		ODSetIsStationery(Environment* ev, 
				ODFrame* frame, ODBoolean isStationery)
{
	{ TempODPart part = frame->AcquirePart(ev);
	  ODSetBooleanProp( ev, ODGetSUFromPstObj(ev, part),
							kODPropIsStationery, kODBoolean, isStationery);
	}
	TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
	if (usersFile)
		usersFile->SetStationery(isStationery);
}

//--------------------------------------------------------------------
// ODSetSUIsStationery
//--------------------------------------------------------------------

void		ODSetSUIsStationery(Environment* ev, 
				ODStorageUnit* su, ODBoolean isStationery)
{
	ODSetBooleanProp( ev, su, kODPropIsStationery, kODBoolean, isStationery);
}

//--------------------------------------------------------------------
// ODGetSUIsStationery
//--------------------------------------------------------------------

ODBoolean		ODGetSUIsStationery(Environment* ev, 
				ODStorageUnit* su)
{
	ODBoolean isStationery;
	
	if (su->Exists(ev, kODPropIsStationery, kODBoolean, 0))
		isStationery = ODGetBooleanProp( ev, su, kODPropIsStationery, kODBoolean);
	else
		isStationery = kODFalse;
		
	return isStationery;
}

//-------------------------------------------------------------------------------------
// Editor and Kind Menu manipulation functions
//-------------------------------------------------------------------------------------
// copied from LinkDlgs.cpp (with changes by T‚)
//------------------------------------------------------------------------------
// AddTypesToMenu
//------------------------------------------------------------------------------

void AddTypesToMenu(ODTypeList* typeList, MenuHandle kindMenu, ODSession* session)
{
	ODName*				name;
	ODSShort			item = 0;
#if !defined(_PLATFORM_WIN32_)
	Environment*		ev = somGetGlobalEnvironment();
	ODNameSpaceManager*	nsm = session->GetNameSpaceManager(ev);
	
	ODTypeListIterator* typeIter = typeList->CreateTypeListIterator(ev);
	
	EditorSet*			editorList = new EditorSet;
	editorList->InitEditorSet();

	for (ODType type = typeIter->First(ev);
		typeIter->IsNotComplete(ev);
		type = typeIter->Next(ev))
	{
		if ( GetUserKindFromKind(nsm, type, &name) )
		{
			Str255 itemString;
			IntlToPStr(name, itemString);
			InsertMenuItem(kindMenu, itemString, item++);
			
			ScriptCode itemScript = GetITextScriptCode(name);
			SetPopupItemScript(kindMenu, item, itemScript);
			
			DisposeIText( name );
		}
		else
		{
			// there's no name for this type
			Str255 pKindStr;
			ODTranslation* translation = session->GetTranslation(ev);
			ODPlatformType platformType = 
				translation->GetPlatformTypeFromISOType(ev, type);
			if ( platformType == kODNULL )
			{
				// Special menu item
				CUsingLibraryResources r;
				GetIndString(pKindStr, kODInfoUtilStrsID, kODStrIndNoKinds); 
			}
			else
			{
				ODBlockMove(&platformType, &(pKindStr[1]), (ODULong) sizeof(ODPlatformType));
				pKindStr[0] = (unsigned char) sizeof(ODPlatformType);
			}
			// Allow metacharacters in item text
			InsertMenuItem(kindMenu, "\p ", item++);
			SetMenuItemText(kindMenu, item, pKindStr);
		}
	
		if (!GetAllEditorsForKind(nsm, type, editorList))
			DisableItem(kindMenu, item);
		editorList->RemoveAllEditors();

		ODDisposePtr(type);
	}
	ODDeleteObject( editorList );
	ODDeleteObject( typeIter );
#endif // !defined(_PLATFORM_WIN32_)
}

//------------------------------------------------------------------------------
// EnableTypesInMenu
//------------------------------------------------------------------------------

ODUShort EnableTypesInMenu(
					ODTypeList*			typeList,
					MenuHandle			kindMenu,
					ODEditor			editor,
					ODNameSpaceManager*	nsm)
{
	Environment* ev = somGetGlobalEnvironment();
	ODUShort itemStart = 0;

#if !defined(_PLATFORM_WIN32_)
	if ( kindMenu && typeList && (typeList->Count(ev) > 0) )
	{
		ODTypeListIterator* typeIter = typeList->CreateTypeListIterator(ev);
		
		for (ODType type = typeIter->First(ev);
			typeIter->IsNotComplete(ev);
			type = typeIter->Next(ev))
		{
			if ( editor == kODNULL )
			{
				// Editor is not fixed; Aassume kind can be embedded even if there is
				// no editor installed for it
				EnableItem(kindMenu, ++itemStart);
			}
			else
			{
				if ( EditorSupportsKind(nsm, editor, type) )
					EnableItem(kindMenu, ++itemStart);
				else
					DisableItem(kindMenu, ++itemStart);
			}
			ODDisposePtr(type);
		}
	
		ODDeleteObject( typeIter );
	}
	
#endif // !defined(_PLATFORM_WIN32_)
	return itemStart;
}

//------------------------------------------------------------------------------
// DeleteAllMenuItems
//------------------------------------------------------------------------------
ODStatic void DeleteAllMenuItems(MenuHandle theMenu)
{
#if !defined(_PLATFORM_WIN32_)
	ODUShort i;

	for (i = CountMItems(theMenu);  i > 0;  --i)
	{
		DeleteMenuItem(theMenu, i);
	}
#endif // !defined(_PLATFORM_WIN32_)
}

//------------------------------------------------------------------------------
// SetupEditorMenu
//------------------------------------------------------------------------------
ODBoolean SetupEditorMenu(ODType kind,
							EditorSet* editorList,
							MenuHandle editorMenu, 
							ControlHandle popupCtlHndl, 
							ODSession* session)
{
	ODBoolean anyEditors = (editorList->GetEditorCount() > 0);
	
#if !defined(_PLATFORM_WIN32_)
	Environment* ev = somGetGlobalEnvironment();
	ODNameSpaceManager* nsm = session->GetNameSpaceManager(ev);

	// And remove all items from the editors menu
	DeleteAllMenuItems(editorMenu);
	
	// Get all editors supporting that kind; Preferred editor must be first
	if ( kind && !anyEditors)
	{
		ODEditor prefEditor = GetSysPrefEditorForKind(nsm, kind);
		if ( prefEditor )
			editorList->AddEditor(prefEditor);
		ODDisposePtr((ODPtr) prefEditor);
		anyEditors = GetAllEditorsForKind(nsm, kind, editorList);
	}

	if ( anyEditors )
	{
		AddEditorsToMenu(editorList, editorMenu, session);
		if ( popupCtlHndl )
		{
			SetControlMaximum(popupCtlHndl, editorList->GetEditorCount());
			HiliteControl(popupCtlHndl, kControlActive);
		}
	}
	else
	{
		// The editor popup should be inactive and contain one blank item
		AppendMenu(editorMenu, "\p");
		if ( popupCtlHndl )
		{
			SetControlMaximum(popupCtlHndl, 1);
			HiliteControl(popupCtlHndl, kControlInactive);
		}
	}

	if ( popupCtlHndl )
	{
		SetControlMinimum(popupCtlHndl, 1);
		SetControlValue(popupCtlHndl, 1);
	}
	
#endif // !defined(_PLATFORM_WIN32_)
	return anyEditors;
}

//------------------------------------------------------------------------------
// AddEditorsToMenu
//------------------------------------------------------------------------------

void AddEditorsToMenu(EditorSet* editorList,
								MenuHandle editorMenu, 
								ODSession* session)
{
	ODName*		name;
#if !defined(_PLATFORM_WIN32_)
	Str255 		pEditorString;
	ODSShort		item = 0;
	Environment* 		ev = somGetGlobalEnvironment();
	ODNameSpaceManager*	nsm = session->GetNameSpaceManager(ev);
	
	EditorSetIterator* editorIter = editorList->CreateIterator();
	
	for (ODType editor = editorIter->First();
		editorIter->IsNotComplete();
		editor = editorIter->Next())
	{
		if (GetUserEditorFromEditor(nsm, editor, &name))
		{
			TempODName tempName = name; // DMc: ensure it's deleted
			IntlToPStr(name, pEditorString);
			InsertMenuItem(editorMenu, pEditorString, item);
			EnableItem(editorMenu, ++item);
			// comments for SetPopupItemScript say don't call unless the script
			// needs setting, but there's no way to test that here.  Sounds like
			// an optimization to me....
			SetPopupItemScript( editorMenu, item, GetITextScriptCode(name) );
		}
		else
		{
			// Special menu item in italics
			CUsingLibraryResources r;
			GetIndString(pEditorString, kODInfoUtilStrsID, kODStrIndNoEditors); 

			InsertMenuItem(editorMenu, pEditorString, item);
			DisableItem(editorMenu, ++item);
		}
	}

	ODDeleteObject( editorIter );
#endif // !defined(_PLATFORM_WIN32_)
}

//------------------------------------------------------------------------------
// GetThisEditorFromList
//------------------------------------------------------------------------------
ODEditor GetThisEditorFromList(ODSShort editorIndex, EditorSet* editorList)
{
	ODEditor result = kODNULL;
	
	if ( editorIndex > 0 )
	{
		ODEditor editor;
		EditorSetIterator* iter = editorList->CreateIterator();
		
		for ( editor = iter->First(); 
			  (--editorIndex > 0) && iter->IsNotComplete(); 
			  editor = iter->Next() )
		{
		}
	
		delete iter;
		
		if ( editor != kODNULL )
			result = ODISOStrFromCStr((char *) editor);
	}

	return result;
}

//------------------------------------------------------------------------------
// IndexOfEditorInList
//------------------------------------------------------------------------------
ODSShort IndexOfEditorInList(EditorSet* editorList, ODEditor editor)
{
	ODSShort retVal = 0;
	
	if ( editorList->GetEditorCount() )
	{
		ODSShort i = 1;
		EditorSetIterator* editorIter = editorList->CreateIterator();
		
		for (ODEditor anEditor = editorIter->First();
			editorIter->IsNotComplete();
			anEditor = editorIter->Next())
		{
			if ( (anEditor == editor) || ODISOStrEqual(anEditor, editor) )
			{
				retVal = i;
				break;
			}
			i++;
		}

		ODDeleteObject( editorIter );
	}
	
	return retVal;
}

//------------------------------------------------------------------------------
// IndexOfKindInList
//------------------------------------------------------------------------------
ODSShort IndexOfKindInList(ODTypeList* kindList, ODType kind)
{
	ODSShort retVal = 0;
	
	Environment* ev = somGetGlobalEnvironment();

	if ( kindList->Count(ev) )
	{
		ODSShort i = 1;
		ODTypeListIterator* kindIter = kindList->CreateTypeListIterator(ev);
		
		TRY
		
			for (ODType type = kindIter->First(ev);
				kindIter->IsNotComplete(ev);
				type = kindIter->Next(ev), i++)
			{
				if ( ODISOStrEqual(type, kind) )
					retVal = i;
				ODDisposePtr(type);
				if ( retVal != 0 )
					break;
			}

		CATCH_ALL
			ODDeleteObject(kindIter);
			RERAISE;
		ENDTRY
		
		ODDeleteObject(kindIter);
	}
	
	return retVal;
}

//------------------------------------------------------------------------------
// GetThisKindFromList
//------------------------------------------------------------------------------
ODType GetThisKindFromList(ODSShort kindItem, ODTypeList* kindList)
{
	Environment* ev = somGetGlobalEnvironment();
	ODType type = kODNULL;
	ODTypeListIterator* kindIter = kindList->CreateTypeListIterator(ev);

	for ( type = kindIter->First(ev); 
		  (--kindItem > 0) && kindIter->IsNotComplete(ev); 
		  type = kindIter->Next(ev) )
	{
		ODDisposePtr(type);
		type = kODNULL;
	}

	ODDeleteObject(kindIter);

	return type;
}


//------------------------------------------------------------------------------
// ContentValueTypes
//------------------------------------------------------------------------------
void ContentValueTypes(ODStorageUnit* contentSU, ODTypeList* typeList)
{
	TempODType hfsType = kODNULL;

	TRY
		ODULong		count;
		ODULong		index;
		
		Environment* ev = somGetGlobalEnvironment();

		ODTranslation* translation = contentSU->GetSession(ev)->GetTranslation(ev);
		hfsType = translation->GetISOTypeFromPlatformType(ev, kODFileType_hfs, kODPlatformDataType);

		contentSU->Focus(ev, kODPropContents, kODPosUndefined, 0, 0, kODPosUndefined);
		count = contentSU->CountValues(ev);
		for (index = 1; index <= count; ++index)
		{
			contentSU->Focus(ev, kODPropContents, kODPosUndefined, 0, index, kODPosUndefined);
			TempODType type = contentSU->GetType(ev);
			if ( !ODISOStrEqual(type, hfsType) )
				typeList->AddLast(ev, type);
		}
	CATCH_ALL
	ENDTRY
}

//------------------------------------------------------------------------------
// ODUserRenameFile
//------------------------------------------------------------------------------


ODStatic ODBoolean ODUserRenameFile(Environment* ev,
					ODSession* session, 
					PlatformFile*	usersFile, 
					ODIText* name,
					DescType replaceOption)
{
	ODBoolean	fileRenamed = kODFalse;
#if !defined(_PLATFORM_WIN32_)
	
	// name to aStr255
	Str255 aStr255;
	
	GetITextPString(name, aStr255);

	TRY
		usersFile->Rename(aStr255);
		fileRenamed = kODTrue;
	CATCH_ALL
		if (ErrorCode() == dupFNErr)
		{
			switch (replaceOption) {
			case kAEAsk:
				ParamText(aStr255, "\p","\p","\p");
				fileRenamed = ODAskUserReplace(ev, session);
				break;
			case kAEYes:
				fileRenamed = kODTrue; break;
			case kAENo:
				fileRenamed = kODFalse; break;
			}
			
			if (fileRenamed)
			{
				ODFileSpec fsspec = usersFile->GetFileSpec();
				ODError result = HDelete(fsspec.vRefNum,fsspec.parID,aStr255);
				if ( result!=wPrErr && result!=fLckdErr && 
									result!=fBsyErr && result!=afpAccessDenied )
				{
					TRY
						usersFile->Rename(aStr255);
					CATCH_ALL
						WARN("File renaming failed, err %ld",ErrorCode());
						fileRenamed = kODFalse;
					ENDTRY
				}
				else
				{
					fileRenamed = kODFalse;
					if (replaceOption == kAEAsk)
						switch(result)
						{
							case wPrErr:
							case fLckdErr:
							case fBsyErr:
							{
								CUsingLibraryResources res;
								StopAlert(kFilelockedAlrtID, GetODDialogFilter());
								break;
							}
							default:
								SysBeep(2);
						}
					else
						SysBeep(2);
				}
			}
		}
		else 
			RERAISE;
	ENDTRY
#endif // !defined(_PLATFORM_WIN32_)
	return( fileRenamed );
}

//------------------------------------------------------------------------------
// ODAskUserReplace
//------------------------------------------------------------------------------

ODBoolean	ODAskUserReplace(Environment* ev,
							ODSession* session)
// Pass in the name in question via ParamText(^0).
{  
#if !defined(_PLATFORM_WIN32_)
	CUsingLibraryResources r;
	ODBoolean	doReplace	= kODFalse;
	short      	itemHit 	= 0;
	DialogPtr 	dlg 		= ODGetNewDialog(ev, kNameConflictDlgID, 
									session, kODFalse );
	if (dlg)
	{
		Handle		scratchHandle = kODNULL;
		Rect		scratchRect;

		SetPort(dlg);
		
			// Set the draw routine for the default button outline item
		GetDialogItem(dlg, kNameConflictDefaultButtonItem, &itemHit, &scratchHandle, &scratchRect);
		SetDialogItem(dlg, kNameConflictDefaultButtonItem, itemHit, 
				 (Handle)GetODOutlineDefaultButtonDrawProc(), &scratchRect);
				
		ShowWindow(dlg);

		do {
			ModalDialog(GetODDialogFilter(), &itemHit);
			switch(itemHit)
			{
				case kNameConflictReplaceBtn:
					doReplace = kODTrue;
					break;
				case kNameConflictCancelBtn:
					break;
			}
		} while ((itemHit != kNameConflictReplaceBtn) && 
										(itemHit != kNameConflictCancelBtn));
				
		ODDisposeDialog(dlg);
	}
	return doReplace;
#else // defined(_PLATFORM_WIN32_)
	return kODFalse;
#endif // !defined(_PLATFORM_WIN32_)
}

//------------------------------------------------------------------------------
//	CommentsDontMatch
//
//	This compares 2 ODIText strings to see if they match and returns TRUE if
//	they do not match.  It take into account the fact that the comments field
//	of the desktop database clips its string to 200 characters.
//
//	Returns false if the strings are identical but the script/language are
//	not.  This is because these values in dtComments have been set using
//	system defaults while those in propComments were actually saved, and
//	should therefore take precedence. <eeh>
//
//	propComments can be NULL, dtComments cannot
//------------------------------------------------------------------------------

ODBoolean
CommentsDontMatch(ODIText* dtComments, ODIText* propComments)
{
	ODBoolean	result;
#if !defined(_PLATFORM_WIN32_)
	ODULong		dtLen, propLen;
	Str255		dtStr, propStr;
	
	if (dtComments != kODNULL)
		dtLen = GetITextStringLength(dtComments);
	else
		dtLen = 0;
	if (propComments == kODNULL)
		return (dtLen != 0);
	propLen = GetITextStringLength(propComments);
	if (dtLen == 0)
		return (propLen != 0);
	
	// if either string is length 0 (or NULL) we won't get here
	WASSERT(dtComments && propComments);
	result = kODTrue;

	// because the Finder does not store script and language with the
	// comments or file names, we ignore these in comparing strings.  That
	// is, if the strings are identical byte-for-byte, we consider them
	// to be equal.  That way, the string *we* stored will be used, as will
	// the script and language information we stored with it.

	ODBoolean lengthsMatch =
			(dtLen == propLen) || (dtLen == 200 && propLen >= 200);
	if ( lengthsMatch )
	{
		GetITextPString(dtComments, (StringPtr) &dtStr);
		GetITextPString(propComments, (StringPtr) &propStr);
		propStr[0] = dtStr[0];			// fix the 200 case
		result = !EqualString( dtStr, propStr, kODTrue, kODTrue );
	}
#endif // !defined(_PLATFORM_WIN32_)
	return result;

}


void SetAllWindowShowLinks(Environment* ev, ODWindowState* winState, ODBoolean showLinks)
{
	ODWindowIterator* iter = kODNULL; ODVolatile(iter);
			
	TRY

		iter = winState->CreateWindowIterator(ev);
	
		for (ODWindow* window = iter->First(ev); iter->IsNotComplete(ev);
				window = iter->Next(ev))
		{
			if (window)
			{
				window->SetShouldShowLinks(ev, showLinks);
				ODFrame* root = window->GetRootFrame(ev);
				root->Invalidate(ev, kODNULL, kODNULL);
			}
		}
		ODDeleteObject(iter);

	
	CATCH_ALL

		ODDeleteObject(iter);
		RERAISE;

	ENDTRY
}

//------------------------------------------------------------------------------
// TranslateValueTypes
//------------------------------------------------------------------------------
void TranslateValueTypes(
				ODTypeList*	kindList,
				ODTypeList*	translateToList,
				OrderedCollection* translateFromList,
				ODSession*	session)
{
	Environment* ev = somGetGlobalEnvironment();

	ODTranslation* translation = session->GetTranslation(ev);

	ODTypeListIterator* kindIter = kindList->CreateTypeListIterator(ev);
	
	ODUShort kindIndex = 0;

	for (ODType kind = kindIter->First(ev);
		kindIter->IsNotComplete(ev);
		kind = kindIter->Next(ev))
	{
		++kindIndex;

		TRY
			ODTypeList* toList = translation->GetTranslationOf(ev, kind);
			
			ODTypeListIterator* toIter = toList->CreateTypeListIterator(ev);
			
			for (ODType toKind = toIter->First(ev);
				toIter->IsNotComplete(ev);
				toKind = toIter->Next(ev))
			{
				TRY
				
					if ( (kindList->Contains(ev, toKind) == kODFalse)
						&&
						 (translateToList->Contains(ev, toKind) == kODFalse)
					   )
					{
						translateToList->AddLast(ev, toKind);
						translateFromList->AddLast((void*) kindIndex);
					}
				
				CATCH_ALL
				ENDTRY
				ODDisposePtr(toKind);
			}
			delete toIter;
			delete toList;

		CATCH_ALL
		ENDTRY

		ODDisposePtr(kind);
	}
	delete kindIter;
}

//------------------------------------------------------------------------------
// StringPtrFromStrHandle
//------------------------------------------------------------------------------

ODStatic StringPtr StringPtrFromStrHandle(StringHandle strHandle)
{
	StringPtr result = kODNULL;

#if !defined(_PLATFORM_WIN32_)
	if ( strHandle )
	{
		ODULong size = GetHandleSize((Handle) strHandle);
		StringPtr strPtr = (StringPtr) ODLockHandle((ODHandle) strHandle);
		result = (StringPtr) ODNewPtr(size);
		ODBlockMove(strPtr, result, size);
		ODUnlockHandle((ODHandle) strHandle);
	}

#endif // !defined(_PLATFORM_WIN32_)
	return result;
}

//------------------------------------------------------------------------------
// DefaultMenuStringForKind
//------------------------------------------------------------------------------

ODStatic StringPtr DefaultMenuStringForKind()
{
#if !defined(_PLATFORM_WIN32_)
	CUsingLibraryResources r;

	StringPtr result = kODNULL;

	StringHandle strHandle = GetString(kODPartInfoStrUnknownID);
	if ( strHandle )
	{
		result = StringPtrFromStrHandle(strHandle);
		ReleaseResource((Handle) strHandle);
	}
	else
	{
		strHandle = NewString("\p");
		result = StringPtrFromStrHandle(strHandle);
		ODDisposeHandle((ODHandle) strHandle);
	}
	return result;
#else // defined(_PLATFORM_WIN32_)
	return kODNULL;
#endif // !defined(_PLATFORM_WIN32_)
}

//------------------------------------------------------------------------------
// GetMenuStringForKind
//------------------------------------------------------------------------------

ODStatic void GetMenuStringForKind(
	ODSession*		session,
	ODType			kind,
	StringPtr*		itemString,
	ODScriptCode*	itemScript)
{
#if !defined(_PLATFORM_WIN32_)
	ODName*				name;
	Environment*		ev = somGetGlobalEnvironment();
	ODNameSpaceManager*	nsm = session->GetNameSpaceManager(ev);

	if ( GetUserKindFromKind(nsm, kind, &name) )
	{
		TempODName tempName = name; // DMc: ensure it's deleted
		*itemString = (StringPtr) ODNewPtr(GetITextStringLength(name)+1);
		IntlToPStr(name, *itemString);
		*itemScript = GetITextScriptCode(name);
	}
	else
	{
		// No available editors support this kind
		ODTranslation* translation = session->GetTranslation(ev);
		ODPlatformType platformType = translation->GetPlatformTypeFromISOType(ev, kind);
		if ( platformType != kODNULL )
		{
			*itemString = (StringPtr) ODNewPtr(sizeof(ODPlatformType) + 1);
			ODBlockMove(&platformType, &((*itemString)[1]), (ODULong) sizeof(ODPlatformType));
			(*itemString)[0] = (unsigned char) sizeof(ODPlatformType);
		}
		else
		{
			*itemString = DefaultMenuStringForKind();
		}
		*itemScript = smSystemScript;
	}
#endif // !defined(_PLATFORM_WIN32_)
}

//------------------------------------------------------------------------------
// AddTranslationKindToMenu
//------------------------------------------------------------------------------

void AddTranslationKindToMenu(
		ODType		kind, 
		MenuHandle	kindMenu,
		short		item,
		ODBoolean	insert,
		ODSession*	session)
{
#if !defined(_PLATFORM_WIN32_)
	StringPtr itemString;
	ODScriptCode itemScript;
	
	GetMenuStringForKind(session, kind, &itemString, &itemScript);
	TempODString tempItemString = (char*) itemString; // DMc: ensure it's deleted

	if ( insert )
	{
		InsertMenuItem(kindMenu, itemString, item);
		item += 1;
	}
	else
		SetMenuItemText(kindMenu, item, itemString);
	
	SetPopupItemScript(kindMenu, item, itemScript);
#endif // !defined(_PLATFORM_WIN32_)
}
