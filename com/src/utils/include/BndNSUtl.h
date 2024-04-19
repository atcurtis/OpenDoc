/* @(#) 1.8 com/src/utils/include/BndNSUtl.h, odconfig, od96os2, odos29646d 10/30/96 18:06:39 [11/15/96 15:28:57] */
/*====START_GENERATED_PROLOG====================================== */
/*
 *   COMPONENT_NAME: odconfig
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
/*====END_GENERATED_PROLOG======================================== */

/*
        File:           BndNSUtl.h

        Contains:       Binding utilities for EditorSetup.

        Owned by:       Caia Grisar

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/


#ifndef _BNDNSUTL_
#define _BNDNSUTL_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _PLFMFILE_
#include "PlfmFile.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __CONTROLS__
#include <Controls.h>
#endif
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include "NmSpcMg.xh"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#if (defined _PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_ )
#ifndef _ODTYPESP_
#include <ODTypesP.h>
#endif


#ifndef _ODTYPESB_
#include <ODTypesB.xh>
#endif


#ifndef _ODTYPESM_
#include <ODTypesM.xh>
#endif


#ifndef _SOMCORBA_
#include <somcorba.h>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#if defined (_PLATFORM_UNIX_)
#define EXPENTRY
typedef char *PSZ;
#endif

#endif // defined(_PLATFORM_WIN32_ || _PLATFORM_OS2_ || _PLATFORM_UNIX_)



//==============================================================================
// Classes used by this interface
//==============================================================================
class   ODTypeList;
class   EditorSet;
class   ODNameSpaceManager;
class   ODStorageUnitView;
class   ODPart;
class   ODStorageUnit;
class   ODPersistentObject;



//==============================================================================
// functions
//==============================================================================


extern "C" {

ODEditor SetSysPrefEditorForKind(ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType kind, ODEditor editor);
        // Sets the system-wide preferred editor for the argument data kind.
        // This routine will overwrite any preference set by the user.

ODEditor SetSysPrefEditorForCategory(ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType category,ODEditor editor);
        // Sets the system-wide preferred editor for the argument data category.
        // This routine will overwrite any preference set by the user.

ODEditor GetSysPrefEditorForKind(ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType kind);
        // Returns the system-wide preferred editor for the argument data kind.
        // A null value is returned if no preference has been specified.
        // This routine checks to ensure that the preferred editor actually exists;
        // if it does not, the preference is canceled and null is returned.

ODEditor GetSysPrefEditorForCategory(ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType category);
        // Returns the system-wide preferred editor for the argument data category.
        // A null value is returned if no preference has been specified.

ODTypeList* GetCategoriesOfKind(ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType kind);
        // Returns the categories of the argument data kind, in the content of the
        // argument storage unit.  The client must dispose of the result.

ODBoolean GetUserKindFromKind( ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType kind, ODName** name );
ODBoolean GetUserEditorFromEditor( ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType editor, ODName** name );
ODBoolean GetUserCatFromCat( ODNameSpaceManager* theNameSpaceManager,
                                                                        ODType category, ODName** name );

ODOSType ODGetODOSTypeForKind(ODNameSpaceManager* theNmSpcMgr, ODType kind);

ODBoolean EditorSupportsKind( ODNameSpaceManager* theNameSpaceManager,
                              ODEditor editor, ODType kind );
                // Returns true if the argument editor supports the argument data type
                // without translation.

ODBoolean EditorForKind( ODEditor editor, ODType kind,
                      ODSession* session);
                // Returns true if the argument editor supports the argument data type
                // without translation.

ODBoolean GetAllEditorsForCategory( ODNameSpaceManager* theNameSpaceManager,
                                                                ODType category, EditorSet* theEditorSet );
                // Returns the set of Editors supporting the argument data category without translation.

ODBoolean GetAllEditorsForKind(ODNameSpaceManager* theNameSpaceManager,
                                                                ODType kind, EditorSet* theEditorSet);
                // Returns the set of Editors supporting the argument data kind without translation.

void GetAllKindsForEditor(ODNameSpaceManager* theNmSpcMgr,
                                                                ODTypeList*     kindList, ODEditor editorID);

ODULong ReadSize(ODFileRefNum file, ODStorageUnitView* view);

void ReadBytes(ODFileRefNum file, ODStorageUnitView* view,
                                                ODPtr bytes, ODULong* length);

EditorSet* ODGetEditorSetFromResFile(ODSShort resRefNum);

                // Returns the set of Editors IDs (if any) found in this file

EditorSet* ODGetEditorSetFromNMAP(ODHandle nmap );



//------------------------------------------------------------------------------
// phase1 - add following definitions from the defunct InfoUtil.h file
// ODSetPOName        ODGetCategory      ODGetKindFromPartSU
// ODGetCreationDate  ContentValueTypes  GetThisKindFromList
// ODGetCatFromKind   ODGetSUFromPstObj  ODGetCatFromPartSU
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// Persistent Object Property getters and setters
//    all Getters expect you to pass in a buffer to be filled
//    or kODNULL which causes it to create one for you.
//    all Setters do NOT consume what you pass in, so it is safe to pass in
//    a reference to your internal structures.  They are only accessed for
//    the duration of the Setter API call.
//-------------------------------------------------------------------------------------

ODULong  ODGetCreationDate(Environment* ev, ODStorageUnit* su);

ODIText* ODGetPOName(Environment* ev, ODPersistentObject* pstobj, ODIText* name);

void     ODSetPOName(Environment* ev, ODPersistentObject* pstobj, ODIText* name);

ODStorageUnit* ODGetSUFromPstObj(Environment* ev, ODPersistentObject* pstobj);

ODIText* ODGetPOComments(Environment* ev,  ODPart* part, ODIText* comments);

void     ODSetPOComments(Environment* ev, ODPart* part, ODIText* comments);

ODULong  ODGetPOSize(Environment* ev, ODPersistentObject* pstobj);

//-------------------------------------------------------------------------------------
// Part only Property getters and setters
//-------------------------------------------------------------------------------------
ODName*   ODGetCategory(Environment* ev, ODPart* part, ODNameSpaceManager* nsm);

ODType    ODGetKindFromPartSU(Environment* ev, ODStorageUnit* su);

ODName*   ODGetCatFromPartSU(Environment* ev, ODStorageUnit* su, ODNameSpaceManager* nsm);

ODName*   ODGetCatFromKind(Environment* ev, ODType kind, ODNameSpaceManager* nsm);

ODEditor EXPENTRY  GetCurrentEditorForPart(ODPart* part );

void     SetCategory(ODEditor editorID, ODType  kindID,
                 HWND  categoryItem, ODSession* session);

void     ChangeEditor(ODPart* part, ODEditor editorID);

ODULong  ODGetModificationDate(Environment* ev, ODStorageUnit* su);

void     ODSetModificationDate(Environment* ev, ODStorageUnit* su, ODTime dateTime);

ODType   ODGetKind(Environment* ev, ODPart* part);

ODIText* ODGetModifiedBy(Environment* ev, ODStorageUnit* su,ODIText* userName);

//------------------------------------------------------------------------------------
// Editor and Kind popup Menu manipulation functions copied from LinkDlgs.cpp
//------------------------------------------------------------------------------------
void ContentValueTypes(ODStorageUnit* contentSU, ODTypeList* typeList);


ODType GetThisKindFromList(ODSShort kindItem, ODTypeList* kindList);

ODBoolean EditorSupportsKind_Reg( ODEditor editor, ODType kind,
                      ODSession* session);

//-------------------------------------------------------------------------------------
// General Utility functions
//-------------------------------------------------------------------------------------

PSZ IntlToStr(ODIText* intlText, PSZ pstr);

ODIText* StrToIntl(PSZ pstr, ODIText** intlText);

ODBoolean ODGetIsStationery(Environment* ev, ODFrame* frame);

void      ODSetIsStationery(Environment* ev, ODFrame* frame, ODBoolean isStationery);



}       // End of extern "C" {

#endif // _BNDNSUTL_
