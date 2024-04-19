/* @(#)Z 1.5 com/src/core/InfoDefs.h, odcore, od96os2, odos29646d 96/11/15 15:24:45 (96/10/29 09:20:39) */
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
        File:           InfoDefs.h

        Contains:       constants used in both the code & resource of Info

        Owned by:       Tantek ‚elik

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <10>    10/16/95        CC              GM 1292157: Added kPartInfoEditorLabel and
                                                                        kPartInfoNoEditorText.
                 <9>     4/27/95        T‚              1223529 BB: ODSetPOName and ODSetIconFamily
                                                                        should tweak file in root part case (Did
                                                                        name piece)
                 <8>     3/28/95        T‚              #1151710 BB: Toolbox running out of app
                                                                        heap.  Document Size dialog implemented.
                 <7>     3/24/95        CG              Brought back Editor ID field.
                 <6>     3/23/95        T‚              #1151710 BB: Toolbox running out of app
                                                                        heap. (Not done).  Document Size dialog
                                                                        partially implemented.  Added constants for
                                                                        resources.
                 <5>     2/24/95        CG              1221029, 1221185: Info dialog appearance
                                                                        problems.
                 <4>     2/10/95        CG              #1187539: Renaming a document using
                                                                        Document Info handles file name conflicts.
                 <3>     9/26/94        T‚              #1188044 Part Info Checkbox behaviour
                 <2>     9/15/94        T‚              #1183571 implement Settings Extension
                 <1>     7/11/94        T‚              first checked in
                 <1>     7/11/94        T‚              first checked in
                 <1>     7/11/94        T‚              first checked in
                 <0>      7/5/94        SV              SOMverted
                 <2>     4/13/94        T‚              #1147222: Fix the View As popup in the Part
                                                                        Frame Info dialog.  Also fix the Category
                                                                        text, and the Kind & Editor popups to show
                                                                        the current kind & editor respectively.
                 <1>    11/18/93        T‚              first checked in

        To Do:
*/

#ifndef _INFODEFS_
#define _INFODEFS_

#ifndef _ODTYPES_
#include <ODTypes.h>
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

#if defined(_PLATFORM_WIN32_)
#ifndef _ODTYPESP_
#include <ODTypesP.h>
#endif
#endif // defined(_PLATFORM_WIN32_)


//==============================================================================
// Defines
//==============================================================================
#define OD_KIND_MAX     128                 /* Max length of a kind name */
#define OD_EDITOR_MAX   128                 /* Max length of an editor name */
#define OD_REGKEY_MAX   128                 /* Max length of an editor name */

#define kPartInfoDlgID          1001
#define kPartInfoKindCntl               1201
#define kPartInfoKindMenu               1201
#define kPartInfoEditorCntl             1202
#define kPartInfoEditorMenu             1202
#define kPartInfoViewAsCntl             1203
#define kPartInfoViewAsMenu             1203

#define kPartInfoVertSep                25
#define kPartInfoHorizSep                       26

/* Changeable fields in Part Info dialog */
#define kPartInfoOKBtn                          1
#define kPartInfoCancelBtn                      2
#define kPartInfoSettingsButton     3
#define kPartInfoCommentsEditFld    4
#define kPartInfoByFld                          5
#define kPartInfoModifiedFld            7
#define kPartInfoCreatedFld                     9
#define kPartInfoSizeFld                        11
#define kPartInfoSep1                       13
#define kPartInfoEditorPopup            14
#define kPartInfoKindPopup                      15
#define kPartInfoCategoryFld            16
#define kPartInfoSep2                       18
#define kPartInfoShowLinksCheckbox  22
#define kPartInfoShowLinksText          23
#define kPartInfoStationeryCheckbox     24
#define kPartInfoStationeryText         25
#define kPartInfoBundledCheckbox    26
#define kPartInfoViewAsPopup            28
#define kPartInfoNameEditFld            29
#define kPartInfoPartThumbnail      31
#define kPartInfoDefaultButtonItem      32
#define kPartInfoIDFld                          33
#define kPartInfoMemorySizeButton       35
#define kPartInfoEditorLabel            36
#define kPartInfoNoEditorText           37


// Strings used by Part Info dialogs
#define kODPartInfoSizeInBytes                  140
#define kODPartInfoSizeInK                              141
// #define kODPartInfoStrUnknownID                 142
#define kODDocumentInfoWindowName               143

/* Document Heap dialog defines */
/* resource ids in the 3000-5000 range in this file are reserved for the Doc Heap dialog */

#define kDocHeapDlgID           3001

#define kArrowBtnsCntlID                3101

#define kPICTRsrcType                   'PICT'
#define kSIZERsrcType                   'SIZE'

#define kArrowBtnsPictOffID             3201
#define kArrowBtnsPictTopOnID   3202
#define kArrowBtnsPictBotOnID   3203


#define kDocHeapOKBtn                           1
#define kDocHeapCancelBtn                       2
#define kDocHeapDefaultSizeRadBtn       3
#define kDocHeapDefaultSizeFld          4
#define kDocHeapDefaultSizeBtns         16
#define kDocHeapPreferdSizeRadBtn       6
#define kDocHeapPreferdSizeFld          7
#define kDocHeapPreferdSizeBtns         18
#define kDocHeapDocumentNameFld         10
#define kDocHeapDefaultButtonItem       14
#define kDocHeapDefaultSizeTopBtnRect 15
#define kDocHeapDefaultSizeBotBtnRect 5
#define kDocHeapPreferdSizeTopBtnRect 17
#define kDocHeapPreferdSizeBotBtnRect 8



//==============================================================================
// Constants
//==============================================================================


//==============================================================================
// Classes used by this interface
//==============================================================================

class   ODFrame;
class   ODStorageUnit;
class   ODPart;
class   ODPersistentObject;
class   ODTypeList;
class   EditorSet;
class   ODSession;
class   PlatformFile;

#endif // _INFODEFS_
