/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: oddataxfer
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
// @(#) 1.9 com/src/storage/LinkDlgs.h, oddataxfer, od96os2, odos29646d 8/22/96 15:37:13 [ 11/15/96 15:25:52 ]
/*
        File:           LinkDlgs.h

        Contains:       function declarations for Paste As and Link Info dialogs

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <11>     8/29/95        DM              1221778, 1276165: [ShowLinkUpdateAlert]
                                                                        drop count arg in ShowLinkUpdateAlert
                <10>     5/26/95        VL              1251403: Multithreading naming support.
                 <9>     3/22/95        CC              1230322: Changed parameter to
                                                                        ShowPasteAsDialog().
                 <8>      3/9/95        CC              1221471: Added changesAllowed parameter to
                                                                        ShowLinkDestinationInfo() and
                                                                        ShowLinkSourceInfo().
                 <7>      3/3/95        CC              1223861 Added ShowLinkStatusAlert().
                 <6>     2/15/95        CC              1220327 Added InitLinkDialogs().
                 <5>      2/7/95        CC               1211295: ShowPasteAsDialog() takes an
                                                                        isMove parameter.
                 <4>     9/20/94        CC              1160232 - Added ShowLinkUpdateAlert().
                 <3>     8/10/94        CC              1179943 - LinkInfo dialog functions take
                                                                        ODBaseLink and ODBaseLinkSource parameters.
                 <2>      8/2/94        CC              #1178169 - ShowPasteAsDialog() takes
                                                                        ODFacet* instead of ODFrame*.
                 <1>     6/24/94        CC              first checked in

                ------------------- CC ---- Converted to SOM

                 <8>     5/27/94        jpa             Remove ASLM dependency [1165267]
                 <7>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <6>      4/6/94        CC              XMPTypeInfo: Changed field "creationDate"
                                                                        to "creationTime" and "modificationDate" to
                                                                        "changeTime". (1154722)
                 <5>      4/5/94        CC              Removed unused declaration of class
                                                                        XMPTypeSet. (1153046)
                 <4>     3/25/94        JA              Added missing #include (1147588)
                 <3>     3/24/94        CC              ShowLinkSourceInfo() takes XMPChangeID
                                                                        parameter; XMPPasteAsResult specifies
                                                                        original kind when selected kind is a
                                                                        translation (1153068).
                 <2>     3/17/94        CC              Added ShowLinkSourceInfo(),
                                                                        ShowLinkDestinationInfo(), XMPLinkAction,
                                                                        XMPLinkResult, and XMPLinkInfo. (1151329)
                 <1>     3/14/94        CC              first checked in
                 <0>     3/14/94        CC              Declaration of ShowPasteAsDialog (1144253).

        To Do:
*/

#ifndef _LINKDLGS_
#define _LINKDLGS_



#ifdef _PLATFORM_OS2_


#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef SOM_ODStorageUnit_xh
#include "StorageU.xh"
#endif

#ifndef __TIME_H__
#include <time.h>
#endif

#ifndef _EDITRSET_
#include "EditrSet.h"
#endif

#define INCL_WIN
#include <os2.h>
// HMODULE hmodDLL;


 typedef struct _DIALOGDATA {
  ODStorageUnit * su;
  ODFacet * facet;
  ODPasteAsResult result;
} DIALOGDATA;
typedef DIALOGDATA *PDIALOGDATA;

//==============================================================================
// Classes used by this interface
//==============================================================================
extern "C" {

class ODStorageUnit;
class ODFacet;
class ODBaseLinkSource;
class ODBaseLink;

//==============================================================================
// Data Interchange functions
//==============================================================================

//-------------------------------------------------------------------------------------
// ShowPasteAsDialog
//-------------------------------------------------------------------------------------

ODBoolean ShowPasteAsDialog(
        ODBoolean        canPasteLink,
        ODPasteAsMergeSetting  mergeSetting,
        ODBoolean        isMove,
        ODStorageUnit*      contentSU,
        ODFacet*        facet,
        ODTypeToken        viewType,
        ODPasteAsResult*    result);

ODBoolean ShowMoveCopyDialog( ODMoveCopyResult*  result);
ODBoolean LoadEmbedAsComboBox(HWND hwnd, ULONG id);
void CenterWindow (HWND hwnd);
}


//Function prototypes for Dialog Procs
extern "C" {
MRESULT EXPENTRY PasteAsDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ); //@SK
MRESULT EXPENTRY MoveCopyDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ); //@SK
MRESULT EXPENTRY LinkDestInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY LinkSourceInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY LinkUpdateAlertDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
}

//os2 version
//-------------------------------------------------------------------------------------
// ShowLinkDestinationInfo
//-------------------------------------------------------------------------------------

ODBoolean ShowLinkDestinationInfo(
                                ODBaseLink*             link,
                                ODLinkInfo*             info,
                                ODBoolean               changesAllowed,
                                HWND                    hwnd,
                                ODFacet *               facet,
                                ODLinkInfoResult*       infoResult);


//-------------------------------------------------------------------------------------
// ShowLinkSourceInfo
//-------------------------------------------------------------------------------------

ODBoolean ShowLinkSourceInfo(
                                ODBaseLinkSource*       linkSource,
                                ODUpdateID              change,
                                ODBoolean               changesAllowed,
                                HWND                    hwnd,
                                ODFacet *               facet,
                                ODLinkInfoResult*       infoResult);

//------------------------------------------------------------------------------
// ShowLinkUpdateAlert
//------------------------------------------------------------------------------

ODBoolean ShowLinkUpdateAlert();

#else


#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef __TIME_H__
#include <time.h>
#endif

//==============================================================================
// Classes used by this interface
//==============================================================================

class ODStorageUnit;
class ODFacet;
class ODBaseLinkSource;
class ODBaseLink;
class ODSession;

//==============================================================================
// Data Interchange functions
//==============================================================================

//-------------------------------------------------------------------------------------
// InitLinkDialogs
//-------------------------------------------------------------------------------------

void InitLinkDialogs(ODSession* session);

//-------------------------------------------------------------------------------------
// ShowPasteAsDialog
//-------------------------------------------------------------------------------------

ODBoolean ShowPasteAsDialog(
                                ODBoolean                               canPasteLink,
                                ODPasteAsMergeSetting   mergeSetting,
                                ODBoolean                               isMove,
                                ODStorageUnit*                  contentSU,
                                ODFacet*                                facet,
                                ODTypeToken                             viewType,
#ifdef _PLATFORM_WIN32_
                                HWND                            hwnd,            // rrk 24 apr 96
#endif
                                ODPasteAsResult*                result);

//-------------------------------------------------------------------------------------
// ShowLinkSourceInfo
//-------------------------------------------------------------------------------------

ODBoolean ShowLinkSourceInfo(
                                ODBaseLinkSource*       linkSource,
                                ODUpdateID                      change,
                                ODBoolean                       changesAllowed,
#ifdef _PLATFORM_WIN32_
                                HWND                            hwnd,            // rrk 24 apr 96
                                ODFacet *                       facet,
#endif
                                ODLinkInfoResult*       infoResult);

//-------------------------------------------------------------------------------------
// ShowLinkDestinationInfo
//-------------------------------------------------------------------------------------

ODBoolean ShowLinkDestinationInfo(
                                ODBaseLink*                     link,
                                ODLinkInfo*             info,
                                ODBoolean                       changesAllowed,
#ifdef _PLATFORM_WIN32_
                                HWND                            hwnd,            // rrk 24 apr 96
                                ODFacet *                       facet,
#endif
                                ODLinkInfoResult*       infoResult);

//------------------------------------------------------------------------------
// ShowLinkUpdateAlert
//------------------------------------------------------------------------------

ODBoolean ShowLinkUpdateAlert(
#ifdef _PLATFORM_WIN32_
                                HWND                            hwnd            // rrk 24 may 96
#endif
 );

//------------------------------------------------------------------------------
// ShowLinkStatusAlert
//------------------------------------------------------------------------------

void ShowLinkStatusAlert(ODError error);

#endif


#endif // _LINKDLGS_

