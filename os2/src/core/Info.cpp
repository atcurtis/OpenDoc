/* @(#)Z 1.16 os2/src/core/Info.cpp, odcore, od96os2, odos29646d 96/11/15 15:51:33 (96/10/09 17:43:53) */
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
        File:           Info.cpp

        Contains:       implementation of the Info object including ShowPartFrameInfo

        Owned by:       Tantek  elik

        Copyright:      c 1993-94 by Apple Computer, Inc., all rights reserved.


*/

#define ODInfo_Class_Source
#define VARIABLE_MACROS
#include <Info.xih>


#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODHelp_xh
#include <ODHelp.xh>
#endif

#ifndef _HLPPANELS_
#include <hlppanls.h>
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

#ifndef SOM_ODFrameFacetIterator_xh
#include <FrFaItr.xh>
#endif

#ifndef _ODTYPESP_
#include "ODTypesP.h"
#endif


#ifndef _INFODEFS_
#include "InfoDefs.h"
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _UTILDEFS_
#include <UtilDefs.h>
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

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef SOM_ODSettingsExtension_xh
#include <Settings.xh>
#endif

#ifndef SOM_Module_OpenDoc_StandardExtensions_defined
#include <StdExts.xh>
#endif

#ifndef _PASCLSTR_
#include <PasclStr.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef _BNDNSUTL_
#include <BndNSUtl.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _EDITRSET_
#include <EditrSet.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifndef SOM_ODPartWrapper_xh
#include <PartWrap.xh>
#endif

#ifndef SOM_ODNameSpace_xh
#include <namspac.xh>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef SOM_ODDispatcher_xh
#include <Disptch.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef _ORDCOLL_
#include <OrdColl.h>
#endif

#ifndef _ODRES_
#include <ODRES.H>
#endif

#ifndef _LinkSrc_
#include <LinkSrc.xh>
#endif

#ifndef _DOCMGR_
#include <DocMgr.xh>
#endif

#include <odos2.h>
#include <TIME.h>

#define INCL_WINSTDBOOK         /* KDJ - for PM notebook */
#include <ODMessag.h>           // Header file for catalog IDs

#ifndef _ODPRTREG_
#include <ODPrtReg.xh>
#endif

#ifndef _ODPRTREIG_
#include <ODPrtRei.xh>
#endif

#ifndef SOM_ODWindowIterator_xh
#include <WinIter.xh>
#endif

#include <langinfo.h>

//==============================================================================
// Constants
//==============================================================================
const ODULong kODDateLength = 29; // Length of date in property notebook

//==============================================================================
// Scalar types
//==============================================================================

//==============================================================================
// Static Globals
//==============================================================================

static ODTypeList*  ODInfo_kindList;
static ODEditor     ODInfo_editorID;
static ODEditor     ODInfo_currentEditorName;
static ODSShort     ODInfo_currentKindItem;
static ODSShort     ODInfo_currentEditorItem;

//==============================================================================
// Kind/Editor functions
//==============================================================================

void EnableApplModal(Environment *ev, ODWindowState *winState, HWND hwnd,
                     BOOL enable);

void AddTypesToMenu(ODTypeList* typeList, ODType kindID,
                    MenuHandle editorMenu, ODSession* session);

void AddEditorsToMenu(ODTypeList* kindsList, ODEditor editorID,
                      MenuHandle editorMenu, ODSession* session);


static HMODULE resInst;                           /* handle to resource DLL */
int CreatePropSheet(HWND  hpwnd, ODFacet* facet); /* load the PM notebook */
MRESULT EXPENTRY ODInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY PresPageDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY ViewPageDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY FilePageDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY SettingsPageDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
BOOL InitializeNoteBook(HWND hwnd,ODFacet* facet);
static ULONG ulPageIdPresentation = 0;
static ULONG ulPageIdBinding = 0;
static ULONG ulPageIdDocument = 0;
static HWND  hwndPresentation = NULL;
static HWND  hwndBinding      = NULL;
static HWND  hwndDocument     = NULL;
static ODSession * __session  = kODNULL;

//==============================================================================
// InitInfo
//==============================================================================

SOM_Scope void  SOMLINK ODInfoInitInfo(ODInfo *somSelf, Environment *ev,
                ODSession* session)
{
    ODInfoData *somThis = ODInfoGetData(somSelf);
    ODInfoMethodDebug("ODInfo","ODInfoInitInfo");

        _fSession = session;
        __session = session;  // Static for dialog proceedures
}

//==============================================================================
// ShowPartFrameInfo
//==============================================================================

SOM_Scope ODBoolean  SOMLINK ODInfoShowPartFrameInfo(ODInfo *somSelf, Environment *ev,
                ODFacet* facet, ODBoolean allowEditing)
{
         ODInfoData *somThis = ODInfoGetData(somSelf);
    ODInfoMethodDebug("ODInfo","ODInfoShowPartFrameInfo");



        // Need to acquire frame and storage unit before using them. Put
        // them in temp object so that when the temp object is out of
        // scope, the object get released.
        TempODFrame frame = facet->GetFrame(ev);
        frame->Acquire(ev);
        TempODStorageUnit su = frame->GetStorageUnit(ev);
        su->Acquire(ev);
        ODSession* session = su->GetSession(ev);

        resInst = session->GetWindowSystemData(ev)->resInstance; //kdj - get resource handle

        ODWindowState* WinStat = session->GetWindowState(ev);

        // Need to release the window. Use temp object to accomplish
        // the release when the temp object is out of scope.
        TempODWindow Window = WinStat->AcquireFrontRootWindow(ev);
        HWND hwnd = Window->GetPlatformWindow(ev);

        // Before creating the dialog, disable mouse and keyboard inputs
        // to all the OD windows except the window that is going to own
        // the dialog. After dismissing the dialog, enable back all the
        // OD windows except the owner window for the dialog.
        EnableApplModal(ev, WinStat, hwnd, FALSE);
        CreatePropSheet(hwnd, facet);
        EnableApplModal(ev, WinStat, hwnd, TRUE);

return 0;
}


/*----------------------------------------------------------------------*/
/* KDJ - start major portion of OS/2 port                               */
/*----------------------------------------------------------------------*/
int CreatePropSheet(HWND  hpwnd, ODFacet* facet )

   {

   WinDlgBox( HWND_DESKTOP,       /* Place anywhere on desktop          */
               hpwnd,             /* Owned by frame                     */
               ODInfoDlgProc,     /* Address of dialog procedure        */
               (HMODULE)resInst,  /* Module handle                      */
               IDD_NOTEBOOKDLG,   /* Dialog identifier in resource      */
               facet);            /* Initialization data                */

   return 0;
   }


/*----------------------------------------------------------------------*/
/* Notebook dialog procedure                                            */
/*----------------------------------------------------------------------*/
MRESULT EXPENTRY ODInfoDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  Environment *ev = somGetGlobalEnvironment();
  switch ( msg )
  {
    case WM_INITDLG:

       {
       ODFacet* facet;
       facet = (ODFacet *) MPFROMP(mp2);

       if (!InitializeNoteBook(hwndDlg,facet))
       {
          WinDismissDlg(hwndDlg, FALSE);
       }
       }
       break;

    case WM_COMMAND:                    /* Posted by pushbutton or key  */
      /*
       * PM sends a WM_COMMAND message when the user presses either
       * the Enter or Escape pushbuttons.
       */
      switch( SHORT1FROMMP( mp1 ) )     /* Extract the command value    */
      {
        case DID_OK:  /* the OK button was pressed */
         {
          /*----------------------------------------*/
          /* get the notebook page that was on top. */
          /*----------------------------------------*/
          ULONG ulCurPgID;

          ulCurPgID = (ULONG) WinSendDlgItemMsg(hwndDlg, IDC_NOTEBOOK,
                                                BKM_QUERYPAGEID, (MPARAM)0,
                                                MPFROM2SHORT(BKA_TOP,0));

          /*---------------------------------------------------------*/
          /* send notification to the correct notebook page by way   */
          /* of a user defined message.                              */
          /*---------------------------------------------------------*/
          if (ulCurPgID == ulPageIdPresentation) {

             if (hwndPresentation) {
                WinSendMsg(hwndPresentation,WM_DID_OK,NULL,NULL);
             }
          } else if (ulCurPgID == ulPageIdBinding) {
             if (hwndBinding) {
                WinSendMsg(hwndBinding,WM_DID_OK,NULL,NULL);
             }
          } else if (ulCurPgID == ulPageIdDocument) {
             if (hwndDocument) {
                WinSendMsg(hwndDocument,WM_DID_OK,NULL,NULL);
             }
          }
          WinDismissDlg( hwndDlg, TRUE );  /* Removes the dialog box    */
         }
          return (MRESULT) FALSE;

        case DID_CANCEL:         /* The Cancel pushbutton or Escape key */
          WinDismissDlg( hwndDlg, TRUE );  /* Removes the dialog box    */
          return (MRESULT) FALSE;
        default:
          break;
      }
      break;
    case WM_HELP:
       {
         ULONG   ulCurPgID;
         ODULong helpID;

         // Query NOTEBOOK for pageID
         ulCurPgID = (ULONG) WinSendDlgItemMsg(hwndDlg, IDC_NOTEBOOK,
                                               BKM_QUERYPAGEID, (MPARAM)0,
                                               MPFROM2SHORT(BKA_TOP,0));

         if (ulCurPgID == ulPageIdPresentation)   // Presentation Page
            helpID = SET_VIEW1_PANEL;
         else if (ulCurPgID == ulPageIdDocument)  // Document Page
            helpID = SET_FILE1_PANEL;
         else                                 // Make Binding Page the default
            helpID = SET_TYPE_PANEL;

         SOM_TRY
           if (__session != kODNULL)
           {
             ODHelp * helpme  = __session->GetHelp(ev);
             helpme->DisplayHelp(ev,ODHELPFILE,helpID);
           } /* endif */
         SOM_CATCH_ALL
         SOM_ENDTRY
       }
       break;
    case WM_CONTROL:
       {
       USHORT notifycode;
       PPAGESELECTNOTIFY pPageSelNotify;
       notifycode = SHORT2FROMMP(mp1);
       if (notifycode == BKN_PAGESELECTEDPENDING) {
           /*-----------------------------------------------------*/
           /* send a user defined message to the correct notebook */
           /* page when that page is about to becom the active    */
           /* page.                                               */
           /*-----------------------------------------------------*/
           pPageSelNotify = (PPAGESELECTNOTIFY) PVOIDFROMMP(mp2);
           if (pPageSelNotify->ulPageIdNew == ulPageIdPresentation) {

              if (hwndPresentation) {
                 WinSendMsg(hwndPresentation,WM_SET_ACTIVE,NULL,NULL);
              }
           } else if (pPageSelNotify->ulPageIdNew == ulPageIdBinding) {
              if (hwndBinding) {
                 WinSendMsg(hwndBinding,WM_SET_ACTIVE,NULL,NULL);
              }
           } else if (pPageSelNotify->ulPageIdNew == ulPageIdDocument) {
              if (hwndDocument) {
                 WinSendMsg(hwndDocument,WM_SET_ACTIVE,NULL,NULL);
              }
           }
       }
       }
       break;
    default:
      /*
       * Any event messages that the dialog procedure has not processed
       * come here and are processed by WinDefDlgProc.
       * This call MUST exist in your dialog procedure.
       */
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
  }
  return (MRESULT) FALSE;
}

/*--------------------------------------------------*/
/* Initialize the Notebook by inserting the         */
/* three default pages as well as any dummy pages   */
/* that have been set up.                           */
/*--------------------------------------------------*/
BOOL InitializeNoteBook(HWND hwnd,ODFacet * facet)
{

//   HWND hwndPage;

   POINTL ptl;
   USHORT usTabX, usTabY;
   HWND hwndNoteBook;
   ODSettingsExtension* settings;
   ODSheet* Sheet;
   ODULong sheet_count;

   hwndNoteBook = WinWindowFromID(hwnd,IDC_NOTEBOOK);

   ptl.x = 4;  /* average width of a character in dialog coordinates */
   ptl.y = 8;  /* average height of a character in dialog coordinates */

   WinMapDlgPoints(hwndNoteBook, &ptl, 1, TRUE); /* get pels per character */

   usTabX = ptl.x * 15;                 /* 15 characters wide */
   usTabY = (ptl.y * 2) - (ptl.y / 2);  /* 1.5 characters high */

   if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
        BKM_SETDIMENSIONS, MPFROM2SHORT(usTabX,usTabY), /* width and height pels */
        MPFROMSHORT(BKA_MAJORTAB)))
     return FALSE;



   /*
    * Insert the first page.
    */
   ulPageIdPresentation = (LONG)WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
        BKM_INSERTPAGE, NULL,
        MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | BKA_MAJOR),
        BKA_LAST));

   if ( !ulPageIdPresentation)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
        BKM_SETTABTEXT, MPFROMLONG(ulPageIdPresentation),
        MPFROMP( catgets(ODcat_handle, INFO_SET, INFO_5, "Presentation")
        )))
     return FALSE;



   hwndPresentation =
   WinLoadDlg(
      hwnd,
      hwnd,
      (PFNWP)PresPageDlgProc,
      (HMODULE)resInst,
      IDD_PRESENTATIONPAGEDLG,
      facet);

   if (!hwndPresentation)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
         BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulPageIdPresentation),
         MPFROMHWND(hwndPresentation)))
     return FALSE;


   /*
    * Insert the second page.
    */
   ulPageIdBinding = (LONG)WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
        BKM_INSERTPAGE, NULL,
        MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | BKA_MAJOR),
        BKA_LAST));

   if ( !ulPageIdBinding)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
        BKM_SETTABTEXT, MPFROMLONG(ulPageIdBinding),
        MPFROMP( catgets(ODcat_handle, INFO_SET, INFO_6, "Binding")
        )))
     return FALSE;



   hwndBinding =
   WinLoadDlg(
      hwnd,
      hwnd,
      (PFNWP)ViewPageDlgProc,
      (HMODULE)resInst,
      IDD_VIEWPAGEDLG,
      facet);

   if (!hwndBinding)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
         BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulPageIdBinding),
         MPFROMHWND(hwndBinding)))
     return FALSE;


   /*
    * Insert the third page.
    */
   ulPageIdDocument = (LONG)WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
        BKM_INSERTPAGE, NULL,
        MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | BKA_MAJOR),
        BKA_LAST));

   if ( !ulPageIdDocument)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
        BKM_SETTABTEXT, MPFROMLONG(ulPageIdDocument),
        MPFROMP( catgets(ODcat_handle, INFO_SET, INFO_7, "Document")
        )))
     return FALSE;


   hwndDocument =
   WinLoadDlg(
      hwnd,
      hwnd,
      (PFNWP)FilePageDlgProc,
      (HMODULE)resInst,
      IDD_FILEPAGE1DLG,
      facet);

   if (!hwndDocument)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
         BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulPageIdDocument),
         MPFROMHWND(hwndDocument)))
     return FALSE;


   /*
    * Insert any dummy pages
    */
   Environment *ev = somGetGlobalEnvironment();

   // Add settings page if Part has Extension.
   TempODFrame frame = facet->GetFrame(ev);

   // Need to acquire the frame before using it. Put it
   // in temp object so that the frame is released when
   // the temp object is out of scope.
   frame->Acquire(ev);

   // Need to release the part. Put it in temp object so that when
   // the temp object is out of scope, the part is released.
   TempODPart part = frame->AcquirePart(ev);

   if (part->HasExtension(ev,kODSettingsExtension))
   {

       settings = (ODSettingsExtension*)(part->AcquireExtension(ev,kODSettingsExtension));

       // get array of part's property sheet resources & sheet count
       settings->GetNotebookSheets(ev, &Sheet, &sheet_count);

       //add pages to note book and load handles into array
       SHORT i;
       ULONG ulPageToAdd;
       HWND hwndPageToAdd[10];

       for (i=0; i<10; i++)
       {
          hwndPageToAdd[i] = NULL; //initialize
       }

       for (i=0; i<sheet_count; i++) {
          ulPageToAdd = (LONG)WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
               BKM_INSERTPAGE, NULL,
               MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | BKA_MAJOR),
               BKA_LAST));

          hwndPageToAdd[i] =
          WinLoadDlg(
             hwnd,
             hwnd,
             (PFNWP)Sheet[i].dlgproc,
             (HMODULE)Sheet[i].hmod,
             (ULONG)Sheet[i].id,
             facet);

          if ( !WinSendDlgItemMsg(hwnd, IDC_NOTEBOOK,
                BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulPageToAdd),
                MPFROMHWND(hwndPageToAdd[i])))
            return FALSE;

       }


       // Release the settings
       ODReleaseObject(ev, settings);

   }


   return TRUE;
}                                       /* End of InitializeNotebook    */


/*-------------------------------------------------------------------*/
/* Dialog procedure for the presentation page.                       */
/* "View" page, Presentation                                         */
/*-------------------------------------------------------------------*/
MRESULT EXPENTRY PresPageDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   static ODFacet* facet;

   Str255  aStr255, partNameS255, partCommentsS255;
   Environment *ev = somGetGlobalEnvironment();

   ODSession* session ;
   ODBoolean isRootPart ;
   ODBoolean isRootPartOfDocument ;
   ODUShort viewval;
   HWND hKindList;
   HWND hCategory;
   HWND hEditorList;
   ODName* partName = kODNULL;

  switch ( msg )
  {
    case WM_INITDLG:
    {
       facet = (ODFacet *)MPFROMP(mp2);

       //port start
        TempODFrame frame = facet->GetFrame(ev);

        // Need to acquire the frame before using it. Put it
        // in temp object so that the frame is released when
        // the temp object is out of scope.
        frame->Acquire(ev);

        // Need to release the part. Put it in temp object so that when
        // the temp object is out of scope, the part is released.
       TempODPart part = frame->AcquirePart(ev);

       TempODWindow window = facet->GetWindow(ev);

        // Need to acquire the window and storage unit before using them.
        // Put them in temp object so that they get released.
        window->Acquire(ev);
        TempODStorageUnit su = frame->GetStorageUnit(ev);
        su->Acquire(ev);
        session = su->GetSession(ev);

        // Need to release the containing frame. Use temp object
        // to accomplish the release.
        TempODFrame containingFrame = frame->AcquireContainingFrame(ev);
        isRootPart = (containingFrame == kODNULL);
        isRootPartOfDocument = isRootPart && window->IsRootWindow(ev);


        // Set up Name
        partName = ODGetPOName(ev, part, kODNULL);
        IntlToStr(partName, (PSZ)partNameS255);
        WinSendDlgItemMsg (hwndDlg, DID_PARTNAME, EM_SETTEXTLIMIT, MPFROMSHORT(255), 0 );
        WinSetDlgItemText (hwndDlg, DID_PARTNAME, (PSZ)partNameS255);
        ODDisposePtr (partName);

        // set up viewas
        //  This need to be put into a resource so that it can be internationalized.
        WinSendDlgItemMsg (hwndDlg, DID_VIEW, LM_INSERTITEM, (MPARAM)0,
               MPFROMP(catgets(ODcat_handle, INFO_SET, INFO_1, "Frame") ));


        if (!(isRootPartOfDocument))
        {
           WinSendDlgItemMsg (hwndDlg, DID_VIEW, LM_INSERTITEM, (MPARAM)1,
               MPFROMP(catgets(ODcat_handle, INFO_SET, INFO_2, "Large Icon") ));
           WinSendDlgItemMsg (hwndDlg, DID_VIEW, LM_INSERTITEM, (MPARAM)2,
               MPFROMP(catgets(ODcat_handle, INFO_SET, INFO_3, "Small Icon") ));
           WinSendDlgItemMsg (hwndDlg, DID_VIEW, LM_INSERTITEM, (MPARAM)3,
               MPFROMP(catgets(ODcat_handle, INFO_SET, INFO_4, "Thumb Nail") ));
        }

        viewval = 0;

        WinSendDlgItemMsg (hwndDlg, DID_VIEW, LM_SELECTITEM, MPFROMSHORT(0), MPFROMSHORT(TRUE));

        // set up frozen
        WinCheckButton(hwndDlg,DID_BUNDLED, frame->IsFrozen(ev));

        if (isRootPartOfDocument)
        {
           // set up stationery from aFrame and its part
           WinCheckButton(hwndDlg,DID_STATIONARY, ODGetIsStationery(ev, frame));
           WinShowWindow(WinWindowFromID(hwndDlg, kPartInfoStationeryCheckbox), FALSE);
           WinShowWindow(WinWindowFromID(hwndDlg, kPartInfoStationeryText), FALSE);

           // set up show links from the window
           WinCheckButton(hwndDlg, DID_SHOWLINKS, window->ShouldShowLinks(ev));

        }
        else
        {
            // disable stationery button if NOT root
            WinEnableControl(hwndDlg, DID_STATIONARY, FALSE);

            // disable Show Links button if NOT root
            WinEnableControl(hwndDlg, DID_SHOWLINKS, FALSE);
        }

    }
       break;

/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the notebook page is about to come to the top as the active    */
/* page.  This corresponds to the PSN_SETACTIVE case in Windows.       */
/*---------------------------------------------------------------------*/
    case WM_SET_ACTIVE:
      // Initialize the controls.
    {

      TempODFrame frame = facet->GetFrame(ev);

      // Need to acquire frame and storage unit before using them. Put
      // them in temp object so that when the temp object is out of
      // scope, the object get released.
      frame->Acquire(ev);
      TempODStorageUnit su = frame->GetStorageUnit(ev);
      su->Acquire(ev);
      session = su->GetSession(ev);
      ODTypeToken view = frame->GetViewType(ev);

      if (view == session->Tokenize(ev, kODViewAsFrame))
              viewval = 0;
      else if (view == session->Tokenize(ev, kODViewAsLargeIcon))
              viewval = 1;
      else if (view == session->Tokenize(ev, kODViewAsSmallIcon))
              viewval = 2;
      else if (view == session->Tokenize(ev, kODViewAsThumbnail))
              viewval = 3;

      WinSendDlgItemMsg (hwndDlg, DID_VIEW, LM_SELECTITEM,
                         MPFROMSHORT(0), MPFROMSHORT(TRUE));

    }
       break;

    //the OK button was pressed and this page was on the top - execute
    //the equivalent code as for the PSN_APPLY windows version
/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the user hit the OK button and this page was on top.  This     */
/* corresponds to the PSN_SETACTIVE case in Windows.                   */
/*---------------------------------------------------------------------*/
    case WM_DID_OK:
              {
                TempODFrame frame = facet->GetFrame(ev);

                 // Need to release the part. Put it in temp object so that when
                 // the temp object is out of scope, the part is released.
                TempODPart part = frame->AcquirePart(ev);


                // Need to acquire frame, window, and su before using. Put
                // them in temp object so that they are released when
                // the temp objects are out of scope.
                frame->Acquire(ev);
                TempODWindow window = facet->GetWindow(ev);
                window->Acquire(ev);
                TempODStorageUnit su = frame->GetStorageUnit(ev);
                su->Acquire(ev);
                session = su->GetSession(ev);

                // update Frozen
                frame->SetFrozen(ev, WinQueryButtonCheckstate(hwndDlg, DID_BUNDLED));

                // update Stationery
                ODSetIsStationery(ev, frame, WinQueryButtonCheckstate(hwndDlg, DID_STATIONARY));

                // update Show Links
                window->SetShouldShowLinks(ev,WinQueryButtonCheckstate(hwndDlg, DID_SHOWLINKS));

                // Update Name
                WinQueryDlgItemText (hwndDlg, DID_PARTNAME, sizeof(partNameS255),   (PSZ)partNameS255);
                partName =StrToIntl((PSZ)partNameS255, kODNULL);
                ODSetPOName(ev, part, partName);
                ODDisposePtr (partName);

                // update ViewAs
                switch (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg, DID_VIEW, LM_QUERYSELECTION, 0, 0)) )
                {
                        case 0: // view as frame
                                frame->ChangeViewType(ev, session->Tokenize(ev, kODViewAsFrame));
                                break;
                        case 1: // view as large icon
                                frame->ChangeViewType(ev, session->Tokenize(ev, kODViewAsLargeIcon));
                                break;
                        case 2: // view as small icon
                                frame->ChangeViewType(ev, session->Tokenize(ev, kODViewAsSmallIcon));
                                break;
                        case 3: // view as thumbnail
                                frame->ChangeViewType(ev, session->Tokenize(ev, kODViewAsThumbnail));
                                break;
                        default:
                                break;
                }



              }
       break;
    case WM_COMMAND:
      if (SHORT1FROMMP( mp1 )  == DID_CANCEL)
       /* This occured as a result of the Escape key being pressed while this
        * page was active.  Want to intercept this so it doesn't go onto
        * WinDefDlgProc.  Eventually, need to pass event back up to parent to
        * handle.
        */
       break;
    default:
      /*
       * Any event messages that the dialog procedure has not processed
       * come here and are processed by WinDefDlgProc.
       * This call MUST exist in your dialog procedure.
       */
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
  }
  return (MRESULT) FALSE;
}


/*-------------------------------------------------------------------*/
/* Dialog procedure for the view page.                               */
/* Callback for "BIND" page, Binding                                 */
/*-------------------------------------------------------------------*/
MRESULT EXPENTRY ViewPageDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  static ODFacet* facet;
  Str255  aStr255, partNameS255, partCommentsS255;
  Environment *ev = somGetGlobalEnvironment();
  HWND hKindList;
  HWND hCategory;
  HWND hEditorList;
  ODType  kindID;
  ODBoolean content_exists;

  switch ( msg )
  {
    case WM_INITDLG:
      {
         facet = (ODFacet *)MPFROMP(mp2);

         TempODFrame frame = facet->GetFrame(ev);
         // Need to acquire the frame before using it. Put it
         // in temp object so that the frame is released when
         // the temp object is out of scope.
         frame->Acquire(ev);

         // Need to release the part. Put it in temp object so
         // that when the temp object is out of scope, the part
         // is released.
         TempODPart part = frame->AcquirePart(ev);

         // Need to acquire the storage unit before using it
         TempODStorageUnit su = frame->GetStorageUnit(ev);
         su->Acquire(ev);
         ODSession* session = su->GetSession(ev);

         // get editorID
         ODInfo_editorID = GetCurrentEditorForPart(part);
         kindID = ODGetKind(ev, part);

         // check for existence of the Part's storage unit
         if ( (part->GetStorageUnit(ev))->Exists(ev, kODPropContents, kODNULL, 0) )
         {
            //      set up Kind Menu
            ODInfo_kindList = new ODTypeList;
            ODInfo_kindList->InitTypeList(ev,(ODTypeList*) kODNULL);
            ContentValueTypes(part->GetStorageUnit(ev), ODInfo_kindList);

            HWND hKindList = WinWindowFromID(hwndDlg,DID_KIND);
            AddTypesToMenu(ODInfo_kindList, kindID, hKindList, session);

            //      set up Category static text
            // $$$$$ for now just put in the user string of the first category of the first kind
            // should put in user string of the first category of the CURRENT kind
            HWND hCategory = WinWindowFromID(hwndDlg,DID_CATEGORY);
            SetCategory(ODInfo_editorID, kindID, hCategory, session);

            //      set up Editor Menu
            HWND hEditorList = WinWindowFromID(hwndDlg,DID_EDITOR);
            AddEditorsToMenu(ODInfo_kindList, ODInfo_editorID, hEditorList, session);
         }
         else // list was empty, return error code saying so
         {
             WARNMSG(WARN_INDEX(AMSG_902),
               "Part's properties cannot be displayed until Document is saved.");
         }

      }
      break;
/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the notebook page is about to come to the top as the active    */
/* page.  This corresponds to the PSN_SETACTIVE case in Windows.       */
/*---------------------------------------------------------------------*/
    case WM_SET_ACTIVE:
      {
              TempODFrame frame = facet->GetFrame(ev);
              // Need to acquire the frame before using it. Put it
              // in temp object so that the frame is released when
              // the temp object is out of scope.
              frame->Acquire(ev);

              // Need to release the part. Put it in temp object so
              // that when the temp object is out of scope, the part
              // is released.
              TempODPart part = frame->AcquirePart(ev);

              // Need to acquire the storage unit before using it
              TempODStorageUnit su = frame->GetStorageUnit(ev);
              su->Acquire(ev);
              ODSession* session = su->GetSession(ev);

      }
      break;
/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the user hit the OK button and this page was on top.  This     */
/* corresponds to the PSN_SETACTIVE case in Windows.                   */
/*---------------------------------------------------------------------*/
    case WM_DID_OK:
    {
        TempODFrame frame = facet->GetFrame(ev);

        // Need to acquire the frame before using it. Put it
        // in temp object so that the frame is released when
        // the temp object is out of scope.
        frame->Acquire(ev);

        // Need to release the part. Put it in temp object so
        // that when the temp object is out of scope, the part
        // is released.
        TempODPart part = frame->AcquirePart(ev);

        // Need to acquire the storage unit before using it
        TempODStorageUnit su = frame->GetStorageUnit(ev);
        su->Acquire(ev);
        ODSession* session = su->GetSession(ev);

        // update Kind
        ODSShort kindItem = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg, DID_KIND,
                LM_QUERYSELECTION, 0, 0));
        ODType  oldKind, newKind;
        if (kindItem != ODInfo_currentKindItem)
        {
            oldKind  =  GetThisKindFromList( ODInfo_currentKindItem, ODInfo_kindList );
            newKind  =  GetThisKindFromList( kindItem, ODInfo_kindList );
        }
        else
        {
            oldKind  =  GetThisKindFromList( ODInfo_currentKindItem, ODInfo_kindList );
            newKind  =  GetThisKindFromList( ODInfo_currentKindItem, ODInfo_kindList );
        }

       // update Editor
        ODSShort editorItem = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg, DID_EDITOR,
                LM_QUERYSELECTION, 0, 0));
        if (editorItem != ODInfo_currentEditorItem)
        {

            ODInfo_editorID = ODInfo_currentEditorName;
            if ( EditorSupportsKind_Reg(ODInfo_editorID,  oldKind, session ))
            {
                    // change kind first, then editor
                    part->ChangeKind(ev, newKind);
                    ChangeEditor( part, ODInfo_editorID);

            }
            else
            {
                    // change editor first, then kind
                    ChangeEditor( part, ODInfo_editorID);
                    part->ChangeKind(ev, newKind);
            }

        }
        else
           if (kindItem != ODInfo_currentKindItem) part->ChangeKind(ev, newKind);
              //   set return value of dialog equal to TRUE
              //   WinSetWindowULong(HWND,  DWL_MSGRESULT, MPFROMSHORT(TRUE));
              return MRESULT(TRUE);
        break;
     }

      break;

    case WM_COMMAND:
      if (SHORT1FROMMP( mp1 )  == DID_CANCEL)
       /* This occured as a result of the Escape key being pressed while this
        * page was active.  Want to intercept this so it doesn't go onto
        * WinDefDlgProc.  Eventually, need to pass event back up to parent to
        * handle.
        */
       break;
    default:
      /*
       * Any event messages that the dialog procedure has not processed
       * come here and are processed by WinDefDlgProc.
       * This call MUST exist in your dialog procedure.
       */
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
  }
  return (MRESULT) FALSE;
}


/*-------------------------------------------------------------------*/
/* Dialog procedure for the file page.                               */
/* Callback for "DOCU" page, Document                                */
/*-------------------------------------------------------------------*/
MRESULT EXPENTRY FilePageDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  static ODFacet* facet;
  Str255  aStr255, partNameS255, partCommentsS255;
  ODName* partComments;
  ODName* partName;
  char theDate[kODDateLength]    = "";
  Environment *ev = somGetGlobalEnvironment();
  switch ( msg )
  {
    case WM_INITDLG:
     {
       facet = (ODFacet *)MPFROMP(mp2);
        TempODFrame frame = facet->GetFrame(ev);
        // Need to acquire the frame before using it. Put it
        // in temp object so that the frame is released when
        // the temp object is out of scope.
        frame->Acquire(ev);

        // Need to release the part. Put it in temp object so
        // that when the temp object is out of scope, the part
        // is released.
        TempODPart part = frame->AcquirePart(ev);

        // set up Comments
        partComments = kODNULL;
        partComments = ODGetPOComments(ev, part, kODNULL);
        IntlToStr(partComments, (PSZ)partCommentsS255);

        WinSendDlgItemMsg (hwndDlg, DID_COMMENTS, EM_SETTEXTLIMIT, MPFROMSHORT(255), 0 );
        WinSetDlgItemText (hwndDlg, DID_COMMENTS, (PSZ)partCommentsS255);

        ODDisposePtr (partComments);

        // set up size
        WinSetDlgItemShort(hwndDlg, DID_DATASIZE, ODGetPOSize(ev, part), FALSE);

        // Need to acquire the storage unit before using it. Put it
        // in temp object so that the storage unit is released when
        // the temp object is out of scope.
        TempODStorageUnit partSU = part->GetStorageUnit(ev);
        partSU->Acquire(ev);

        // set up created date
        char* dateFormat = nl_langinfo(D_T_FMT);
        time_t theTime = ODGetCreationDate(ev, partSU);
        if (theTime)
        {
          struct tm *timeptr = localtime((const time_t*) &theTime);
          strftime(theDate, kODDateLength, dateFormat, timeptr);
          WinSetDlgItemText (hwndDlg, DID_CRDATE, theDate);
        }

        // set up modified date
        theTime = ODGetModificationDate(ev, partSU);
        if (theTime)
        {
          struct tm *timeptr = localtime((const time_t*) &theTime);
          strftime(theDate, kODDateLength, dateFormat, timeptr);
          WinSetDlgItemText (hwndDlg, DID_MODDATE, theDate);
        }

        // set up by
        ODName* partUser = kODNULL;
        partUser = ODGetModifiedBy(ev, partSU, kODNULL);
        IntlToStr(partUser, (PSZ)aStr255);
        WinSetDlgItemText(hwndDlg, DID_MODBY, (PSZ)aStr255);
        ODDisposePtr (partUser);

        // set up Draft ID
        WinSetDlgItemShort(hwndDlg, DID_KEY, partSU->GetID(ev), FALSE);


     }
        return MRESULT(TRUE);
       break;

/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the notebook page is about to come to the top as the active    */
/* page.  This corresponds to the PSN_SETACTIVE case in Windows.       */
/*---------------------------------------------------------------------*/
    case WM_SET_ACTIVE:
      break;
/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the user hit the OK button and this page was on top.  This     */
/* corresponds to the PSN_SETACTIVE case in Windows.                   */
/*---------------------------------------------------------------------*/
    case WM_DID_OK:
    {

        // Need to acquire the frame before using it. Put it
        // in temp object so that the frame is released when
        // the temp object is out of scope.
        TempODFrame frame = facet->GetFrame(ev);
        frame->Acquire(ev);

         // Need to release the part. Put it in temp object so that when
         // the temp object is out of scope, the part is released.
        TempODPart part = frame->AcquirePart(ev);

        // Update Comments
        WinQueryDlgItemText (hwndDlg, DID_COMMENTS, sizeof(partNameS255),   (PSZ)partNameS255);
        partComments =StrToIntl((PSZ)partNameS255, kODNULL);
        ODSetPOComments(ev, part, partComments);

     }
      break;

    case WM_COMMAND:
      if (SHORT1FROMMP( mp1 )  == DID_CANCEL)
       /* This occured as a result of the Escape key being pressed while this
        * page was active.  Want to intercept this so it doesn't go onto
        * WinDefDlgProc.  Eventually, need to pass event back up to parent to
        * handle.
        */
       break;
    default:
      /*
       * Any event messages that the dialog procedure has not processed
       * come here and are processed by WinDefDlgProc.
       * This call MUST exist in your dialog procedure.
       */
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
  }
  return (MRESULT) FALSE;
}


/*-------------------------------------------------------------------*/
/* Dialog procedure for the dummy settings pages.                    */
/* Callback for "SETT" page, Settings                                */
/*-------------------------------------------------------------------*/
MRESULT EXPENTRY SettingsPageDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  static ODFacet* facet;
  switch ( msg )
  {
    case WM_INITDLG:
     {
       facet = (ODFacet *)MPFROMP(mp2);
     }
        return MRESULT(TRUE);
       break;

/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the notebook page is about to come to the top as the active    */
/* page.  This corresponds to the PSN_SETACTIVE case in Windows.       */
/*---------------------------------------------------------------------*/
    case WM_SET_ACTIVE:
      break;
/*---------------------------------------------------------------------*/
/* This is code in response to a user defined message which is sent    */
/* when the user hit the OK button and this page was on top.  This     */
/* corresponds to the PSN_SETACTIVE case in Windows.                   */
/*---------------------------------------------------------------------*/
    case WM_DID_OK:
      break;

    case WM_COMMAND:
      if (SHORT1FROMMP( mp1 )  == DID_CANCEL)
       /* This occured as a result of the Escape key being pressed while this
        * page was active.  Want to intercept this so it doesn't go onto
        * WinDefDlgProc.  Eventually, need to pass event back up to parent to
        * handle.
        */
       break;
    default:
      /*
       * Any event messages that the dialog procedure has not processed
       * come here and are processed by WinDefDlgProc.
       * This call MUST exist in your dialog procedure.
       */
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
  }
  return (MRESULT) FALSE;
}

/*----------------------------------------------------------------------*/
/* KDJ - end major portion of OS/2 port                                 */
/*----------------------------------------------------------------------*/



SOM_Scope void  SOMLINK ODInfosomInit(ODInfo *somSelf)
{
         ODInfoData *somThis = ODInfoGetData(somSelf);
         ODInfoMethodDebug("ODInfo","ODInfosomInit");

         ODInfo_parent_ODObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK ODInfosomUninit(ODInfo *somSelf)
{
         ODInfoData *somThis = ODInfoGetData(somSelf);
         ODInfoMethodDebug("ODInfo","ODInfosomUninit");

         ODInfo_parent_ODObject_somUninit(somSelf);
}



//-------------------------------------------------------------------------------------
// Editor and Kind Menu manipulation functions
//-------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// AddTypesToMenu
//------------------------------------------------------------------------------

void AddTypesToMenu(ODTypeList* typeList, ODType kindID,
                    MenuHandle kindMenu, ODSession* session)
{
  ODSShort                item = 0;
  ODSShort                i = 0;
  ISOString               displayname;
  ODPartHandlerRegistryInt* ODRegistry;
  Environment*            ev = somGetGlobalEnvironment();

        ODRegistry = session -> GetRegistry(ev);
        ODTypeListIterator* typeIter = typeList->CreateTypeListIterator(ev);

        for (ODType type = typeIter->First(ev);
                typeIter->IsNotComplete(ev);
                type = typeIter->Next(ev))
        {
           // convert kind name to appropriate display name for dialogue
           displayname = ODRegistry->GetKindDisplayNameFromKind(ev,type);

           // Copy the PartKind list to dialog box
           WinSendMsg(kindMenu, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP(displayname));

        }


        // Set the cursor position to the 1st entry.
        WinSendMsg(kindMenu, LM_SELECTITEM, 0, MPFROMSHORT(TRUE));

        // initialize currentKindItem to part kind index
        item = 0;
        for (ODType kind = typeIter->First(ev);
              typeIter->IsNotComplete(ev);
              kind = typeIter->Next(ev), ++item)
        {
              if (ODISOStrEqual(kind, kindID))
                      break;
        }

        ODInfo_currentKindItem = item;

        ODDeleteObject( typeIter );

}

//------------------------------------------------------------------------------
// AddEditorsToMenu
//------------------------------------------------------------------------------

void AddEditorsToMenu(ODTypeList* kindsList, ODEditor editorID,
                      MenuHandle editorMenu, ODSession* session)
{

    ODSShort   unique;
    ODSShort   i,j;
    ODSShort   item;
    ODSShort  list_loaded = FALSE;
    enum {STR_COUNT =  50};
    char *strarray[STR_COUNT] = {NULL};

    unsigned  long          size;
    ODPartHandlerRegistryInt* ODRegistry;
    _IDL_SEQUENCE_ISOString PartHandlerList;


        Environment*            ev = somGetGlobalEnvironment();
        ODRegistry = session -> GetRegistry(ev);

        ODTypeListIterator* kindIter = kindsList->CreateTypeListIterator(ev);
        for (ODType kind = kindIter->First(ev);
                kindIter->IsNotComplete(ev);
                kind = kindIter->Next(ev) )
        {

           // retrieve PartHandler list from Registry
           // this is a buffer that the Registry allocates and we need to dispose of.
           PartHandlerList = ODRegistry-> GetPartHandlerList( ev, kind, 0);

           //   if (GetUserEditorFromEditor(nsm, editor, &name))
           // if the list is not empty, continue
           if ( PartHandlerList. _length ) {
               list_loaded = TRUE;
           }

           // Copy the PartHandler list to dialog box
           for ( i = 0; i < PartHandlerList. _length; i++)
           {

              for ( j = 0, unique = TRUE; strarray[j]; j++)
              {
                 if (strcmp(PartHandlerList. _buffer[i],strarray[j]) == 0 )
                 {
                    // set unique to false if duplicate editor exists in string array
                    unique = FALSE;
                 }
              }
              if (unique ==  TRUE)
              {
                 // Copy the PartHandler list to dialog box if first occurrence
                 WinSendMsg(editorMenu, LM_INSERTITEM, MPFROMSHORT(LIT_END),
                    MPFROMP((ODRegistry->GetHandlerDisplayNameFromHandler(ev,PartHandlerList. _buffer[i]))));

                 // copy Part Handler name to string for duplicate check
                 strarray[j] = new char[strlen(PartHandlerList. _buffer[i]) +1];
                 strcpy ( strarray[j], PartHandlerList. _buffer[i] );
              }
           }

           // now free up the buffer we got from the Registry
           for ( i = 0; i < PartHandlerList. _length; i++)
           {
                     SOMFree( PartHandlerList. _buffer[i]);
           }
           SOMFree( PartHandlerList. _buffer);

        }



        // If list was loaded, set up list boxes.
        if ( list_loaded == TRUE )
        {
           // set editor name to current editor
           ODInfo_currentEditorName = editorID;

           for ( item = 0;  strarray[item] != NULL; item++)
           {

              // Set the cursor position to the current editor in list box
              if (strcmp(strarray[item], editorID ) == 0 )
              {
                 WinSendMsg(editorMenu, LM_SELECTITEM, MPFROMSHORT(item), MPFROMSHORT(TRUE));
                 // set editor name to current editor
                 ODInfo_currentEditorItem = item;
              }

              // free up Part Handler Strings, strarray
              delete[] strarray[item];
              strarray[item] = NULL;
           }

        }
        else // list was empty, return error code saying so
        {
            WARNMSG(WARN_INDEX(AMSG_903),
              "Couldn't load Part Handler list.");
        }

}



//------------------------------------------------------------------------------
// EnableApplModal - Simulates application modality by disabling/enabling
//                   all the OD windows except the window which owns the dialog.
//------------------------------------------------------------------------------
void EnableApplModal(Environment *ev, ODWindowState *winState, HWND hwnd,
                     BOOL enable)
{
    ODWindowIterator *iter = kODNULL;
    ODWindow         *curWindow;

    SOM_TRY
        // Disable/Enable mouse and keyboard input to all the OD windows
        // except the one which is going to own the display dialog.
        //
        // Note: The window which is going to own the display dialog is
        //       disable/enable by the system.
        iter = winState->CreateWindowIterator(ev);
        for (curWindow = iter->First(ev); iter->IsNotComplete(ev);
             curWindow = iter->Next(ev))
        {
            HWND platformWindow = curWindow->GetPlatformWindow(ev);
            if (platformWindow != hwnd)
               WinEnableWindow(platformWindow, enable);
        }
        ODDeleteObject(iter);

    SOM_CATCH_ALL
        LOG("SOM exception occured in EnableApplModal");
        ODDeleteObject(iter);

    SOM_ENDTRY
}
