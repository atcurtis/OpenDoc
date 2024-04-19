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
// @(#) 1.23 com/src/storage/LinkDlgs.cpp, oddataxfer, od96os2, odos29646d 11/7/96 12:03:50 [ 11/15/96 15:25:52 ]
#ifdef _PLATFORM_OS2_


/**********************************************************************\
 *  Module Header                                                     *
 *                                                                    *
 *  File Name: LinkDlgs.cpp                                           *
 *                                                                    *
 *  Description:                                                      *
 *  This file implements the dialog's required for                    *
 *  OpenDoc Link Support for OS/2 :                                   *
 *  It contains implementation of the Paste As and Link Info dialogs  *
 *                                                                    *
 *  Written by: Saurabh Kumar                                         *
 *                                                                    *
 *                                                                    *
 *  Change History (most recent first):                               *
 *  -------                                                           *
 * <3>   7/19/95  129813   Saurabh Kumar   Disable Embed As if canEmbed False *
 * <2>   5/01/95  123004   Saurabh Kumar   Added code for ShowPasteAsDialog   *
 * <1>   3/10/95   <OS2>   Saurabh Kumar   Created for Link Dialogs   *
 *                                                                    *
 *   To Do                                                            *
 *  -------                                                           *
 *                                                                    *
\**********************************************************************/


#define INCL_WIN
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_ERROR_H
#include <os2.h>
#include <bseerr.h>
#include <odres.h>       //For PasteAs dialog's resource id's
#include <ODPagtun.h>
#include <isostr.h>

#ifndef _PLFMDEF_
#include "plfmdef.h"
#endif

#ifndef _LINKDLGS_
#include "LinkDlgs.h"
#endif

#if (defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))
#ifndef SOM_ODHelp_xh
#include <ODHelp.xh>
#endif
#ifndef _HLPPANELS_
#include <hlppanls.h>
#endif
#endif //_PLATFORM_WIN32_ || _PLATFORM_OS2_

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _LINKDEFS_
#include "LinkDefs.h"
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_RegistryManager_xh
#include <RManager.xh>
#endif

#ifndef _ODPRTREIG_
#include <ODPrtRei.xh>
#endif

#ifndef SOM_ODPartHandlerRegistry_xh
#include <ODPrtReg.xh>
#endif


#ifndef SOM_ODTranslation_xh
#include "Translt.xh"
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef SOM_ODBinding_xh
#include <ODBindng.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_Facet_xh
#include "Facet.xh"
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODPartWrapper_xh
#include <PartWrap.xh>
#endif

//#ifndef SOM_ODPart_xh
//#include <Part.xh>
//#endif

/*
#ifndef _INFOUTIL_
#include "infoutil.h"
#endif
*/

#ifndef _BNDNSUTL_
#include "bndnsutl.h"
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_Facet_xh
#include "Facet.xh"
#endif

#ifndef SOM_ODSession_xh
#include "ODSessn.xh"
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif


#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

// #define RRK_DEBUG 1
#ifndef SOM_ODBaseLinkSource_xh
#include <LinkSrcB.xh>
#endif

#ifndef SOM_ODBaseLink_xh
#include <LinkB.xh>
#endif

#include <wchar.h>
#include <DraftWn.h>
#include <langinfo.h>

ODStatic void GetBestKindAndSize(ODStorageUnit* contentSU, ODType* kind, ODULong* size);

// infoutil is no longer part of OS2 release.
void AddTypesToListBox(ODTypeList* typeList, HWND kindMenu,
                          ODSession* session, ODULong idItem);


HMODULE hmodDLL;

static ODLinkInfoResult*   gLIResult;
static ODFacet*            gFacet;
static ODSShort            gAlertResult;
static ODBaseLink*		   gLink;              // passed in to ShowLinkDestinationInfo function.
static ODUpdateID	   gUpdateID;           // passed in to ShowLinkSourceInfo function.
static ODBoolean	   gChangesAllowed;    // passed in to both Link info functions.
static ODSession*          gSession;

//==============================================================================
// Constants
//==============================================================================


//==============================================================================
// Scalar types
//==============================================================================

//==============================================================================
// Function Prototypes
//==============================================================================

//------------------------------------------------------------------------------
// ShowPasteAsDialog
//------------------------------------------------------------------------------

ODBoolean ShowPasteAsDialog(
        ODBoolean        canPasteLink,
        ODPasteAsMergeSetting  mergeSetting,
        ODBoolean        isMove,
        ODStorageUnit*      contentSU,
        ODFacet*        facet,
        ODTypeToken        viewType,
        ODPasteAsResult*    result)

{

  ULONG rc = 0;
  PDIALOGDATA pDialogData = NULL;
  CHAR LoadError[CCHMAXPATH];
  Environment* ev = somGetGlobalEnvironment();

  DosLoadModule(LoadError, sizeof(LoadError), "ODRes", &hmodDLL);

  // Allocate memory for our private DIALOGDATA structure
  // memory should be freed in the dialog proc.
  pDialogData = new DIALOGDATA;
  memset((PVOID) pDialogData, 0, sizeof(DIALOGDATA));

  //Initialize result
  if ( canPasteLink )
        canPasteLink = contentSU->Exists(ev, kODPropLinkSpec, (ODValueType) kODLinkSpec, 0);

        result->pasteLinkSetting =  canPasteLink;
        result->autoUpdateSetting = kODTrue;
        result->translateKind = (ODType) kODNULL;
        char* myString = catgets(ODcat_handle, LINKSPEC_SET, LINKSPEC_90, "No user editor");
        result->editor = myString;


    result->mergeSetting = mergeSetting;


// result->selectedView = 0;
// result->selectedKind = (ODType) kODNULL;

      // Fill in the DIALOGDATA structure
  pDialogData->su = contentSU;
  pDialogData->facet = facet;
  pDialogData->result.pasteLinkSetting = result->pasteLinkSetting;
  pDialogData->result.autoUpdateSetting = result->autoUpdateSetting;
  pDialogData->result.mergeSetting = result->mergeSetting;
  pDialogData->result.selectedView = result->selectedView;
  pDialogData->result.selectedKind = result->selectedKind;
  pDialogData->result.translateKind = result->translateKind;
  pDialogData->result.editor = result->editor;

  // Need to acquire frame and storage unit before using them. Put
  // them in temp object so that when the temp object is out of
  // scope, the object get released.
  TempODFrame frame = facet->GetFrame(ev);
  frame->Acquire(ev);
  TempODStorageUnit su = frame->GetStorageUnit(ev);
  su->Acquire(ev);
  ODSession* session = su->GetSession(ev);

  ODWindowState* WinStat = session->GetWindowState(ev);

  // Need to release the window. Use temp object to accomplish
  // the release when the temp object is out of scope.
  TempODWindow Window = WinStat->AcquireFrontRootWindow(ev);
  HWND hwnd = Window->GetPlatformWindow(ev);


  //Create a modal dialog box and display it.
  rc = WinDlgBox(HWND_DESKTOP,      //Parent is the DeskTop
//            HWND_DESKTOP,
             hwnd,
            (PFNWP)PasteAsDlgProc,  //dialog window proc
            hmodDLL,         //handle to the DLL containing Dialog resource.
            IDD_PASTEASDLG,  //dialog template ID
            (PVOID) pDialogData );           //application data

  //Update result
  result->pasteLinkSetting  = pDialogData->result.pasteLinkSetting;
  result->autoUpdateSetting = pDialogData->result.autoUpdateSetting;
  result->mergeSetting = pDialogData->result.mergeSetting;
  result->selectedView = pDialogData->result.selectedView;
  result->selectedKind = pDialogData->result.selectedKind;
  result->translateKind = pDialogData->result.translateKind;
  result->editor = pDialogData->result.editor;

#ifdef RRK_DEBUG
       PRINT("*** RRK ODPasteAsResult structure.\n");
       PRINT("\t pasteLinkSetting:\t %d.\n", result->pasteLinkSetting);
       PRINT("\t autoUpdateSetting:\t %d.\n", result->autoUpdateSetting);
       PRINT("\t mergeSetting:\t %d.\n", result->mergeSetting);
       PRINT("\t selectedView:\t %d.\n", result->selectedView);
       PRINT("\t selectedKind:\t %s.\n", result->selectedKind);
       PRINT("\t translateKind:\t %s.\n", result->translateKind);
       PRINT("\t editor:\t %s.\n", result->editor);
#endif
  return rc;
}


/*********************************************************************
 *  Name : PasteAsDlgProc
 *
 *************************************************************************/
MRESULT EXPENTRY PasteAsDlgProc( HWND hwnd, ULONG msg, MPARAM mp1,
                                  MPARAM mp2)
{
static PDIALOGDATA pDlgData = NULL;
Environment* ev = somGetGlobalEnvironment();
static ODTypeList * kindList = NULL;
static ODBoolean  autoUpdateSetting;
static ODPasteAsMergeSetting mergeSetting;
static pasteLinkSetting;
ODEditor editorID = kODNULL;
ODULong maxEditors = 0;
ODSShort  currentKindItem = 1;
ODSShort  currentEditorItem = 0;
static ODBoolean canMerge;
static ODBoolean canEmbed;
static ODBoolean flag = kODFalse;

   switch(msg)
   {
      case WM_INITDLG:
           {
            CenterWindow(hwnd);

            pDlgData = (PDIALOGDATA) mp2;

             // Set Window pointer with object pointer and instance data info
             WinSetWindowPtr(hwnd, QWL_USER, pDlgData);

            //If "Paste with Link" is NOT checked then disable
            //the controls used only for linking.

            if (!pDlgData->result.pasteLinkSetting)
               {
                WinEnableWindow(WinWindowFromID(hwnd,IDD_PASTEAUTOBTN), FALSE);
                WinEnableWindow(WinWindowFromID(hwnd,IDD_PASTEMANUALBTN), FALSE);
                WinEnableWindow(WinWindowFromID(hwnd,IDD_PASTELINKCHKBOX), FALSE);
               }
            else   //Linking is enabled
               {
                //Check the "Paste with Link" check box
                 WinCheckButton(hwnd,IDD_PASTELINKCHKBOX,TRUE);
                //Setup default settings
                 autoUpdateSetting = kODTrue;
                //Check the radio button for getting updates to Automatic.
                 WinCheckButton(hwnd,IDD_PASTEAUTOBTN,TRUE);
               

                 canMerge = (pDlgData->result.mergeSetting != kODPasteAsEmbedOnly);
                 canEmbed = (pDlgData->result.mergeSetting != kODPasteAsMergeOnly);
                 pDlgData->result.mergeSetting = (pDlgData->result.mergeSetting == kODPasteAsMerge)
                                      || (pDlgData->result.mergeSetting == kODPasteAsMergeOnly);

                 mergeSetting = pDlgData->result.mergeSetting;

                if (mergeSetting)  //Merge initially selected
                 {
                 //Check "Merge with Contents" Radio Button
                 WinCheckButton(hwnd,IDD_MERGEBTN,TRUE);

                 if(canEmbed)
                  {
                    WinEnableWindow(WinWindowFromID(hwnd,IDD_EMBEDAS), TRUE);
                    WinEnableWindow(WinWindowFromID(hwnd,IDD_EMBEDBTN), TRUE);
                  }
                 else   //canEmbed is False only when MergeOnly is selected
                  {
                    // inactivate(grey out) the "Embed as:" combo box
                    // if merge only is initially selected
                    WinEnableWindow(WinWindowFromID(hwnd,IDD_EMBEDAS), FALSE);
                    WinEnableWindow(WinWindowFromID(hwnd,IDD_EMBEDBTN), FALSE);
                  }

                 }
                else   //Embed is intially selected
                  {

                  flag = kODTrue;
                  //Check "Embed as "
                  WinCheckButton(hwnd,IDD_EMBEDBTN,TRUE);

                 if(canMerge)
                  {
                    WinEnableWindow(WinWindowFromID(hwnd,IDD_MERGEBTN), TRUE);
                  }
                 else   //canMerge is False only when EmbedOnly is selected
                  {
                    // inactivate(grey out) the "Merge Btn"
                    WinEnableWindow(WinWindowFromID(hwnd,IDD_MERGEBTN), FALSE);
                  }


                  //Load data into "Embed As" Combo Box
                  LoadEmbedAsComboBox(hwnd, IDD_EMBEDAS);

                  //Set up Paste As Kind List
                  kindList = new ODTypeList;
                  kindList->InitTypeList(ev, (ODTypeList *) kODNULL);

                  ContentValueTypes(pDlgData->su, kindList);
                  AddTypesToListBox(kindList,hwnd,pDlgData->su->GetSession(ev)
                              ,IDD_PARTKIND);
                  ODEditor preferredEditor = pDlgData->su->GetSession(ev)->GetBinding(ev)->
                                    ChooseEditorForPart(ev,pDlgData->su,kODNULL);
                  WinSetDlgItemText(hwnd, IDD_PARTEDITOR, preferredEditor);

                  pDlgData->result.editor = preferredEditor;
                  WinSendMsg( WinWindowFromID(hwnd, IDD_PARTKIND),
                             LM_SELECTITEM, MPFROMSHORT(0),
                             MPFROMSHORT( TRUE | FALSE));

                  }
               }

         }
      break;

     case WM_HELP:
           {
             #if (defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))
             ODULong ulHelpid = IDMS_EDIT_PASTEAS_PANEL;
             ODHelp * helpme  = pDlgData->su->GetSession(ev)->GetHelp(ev);
             helpme->DisplayHelp(ev,ODHELPFILE,ulHelpid);
             #endif
           }
     break;

      case WM_COMMAND:

         switch (SHORT1FROMMP (mp1) )
         {
          case DID_PASTE:
           {
           //Check whether "Paste with Link" was checked

           pDlgData=(PDIALOGDATA) WinQueryWindowPtr(hwnd, QWL_USER);

           if(! WinQueryButtonCheckstate(hwnd, IDD_PASTELINKCHKBOX))
           {
            //  Do Paste Content
            pasteLinkSetting = kODFalse;
           }

           else  //Do Paste Link
           {
            pasteLinkSetting = kODTrue;
           }
           //Get user selected information from the dialog box.
           ODType selectedKind = (ODISOStr)kODNULL;     ;
           ODTypeToken  selectedView= 0;
           SHORT sitemselected = 0;
           ULONG ultextlength = 0;
           CHAR  szBuffer[CCHMAXPATH];
           memset((PSZ) szBuffer, 0, sizeof(szBuffer));

           if (pDlgData->result.mergeSetting)  //Merge initially selected
           {

           }
           else   //Embed is selected
            {
            //Get the Part 'Kind'
            sitemselected =(ULONG) WinSendMsg (WinWindowFromID(hwnd, IDD_PARTKIND),
                               LM_QUERYSELECTION,
                               MPFROMSHORT((SHORT) LIT_FIRST),
                               (MPARAM) NULL);

             if (sitemselected != LIT_NONE)
               {
               ultextlength = (ULONG)WinSendMsg (WinWindowFromID(hwnd, IDD_PARTKIND),
                               LM_QUERYITEMTEXTLENGTH,
                               MPFROMSHORT(sitemselected),
                               (MPARAM) NULL);

              //Now , get the part kind the user has selected
                             MRESULT mr = WinSendMsg (WinWindowFromID(hwnd, IDD_PARTKIND),
                               LM_QUERYSELECTION, 0, 0);
                             ODULong kindIndex = (ODULong) mr;
                             selectedKind = GetThisKindFromList( kindIndex+1, kindList );

              }
           else
              {
              selectedKind = (ODISOStr)kODNULL;
              }
         //Determine which item of the 'Embed As' Combo Box the user selected
         selectedView   =(ULONG)  WinSendMsg (WinWindowFromID(hwnd, IDD_EMBEDAS),
                               LM_QUERYSELECTION,
                               MPFROMSHORT((SHORT) LIT_FIRST),
                               (MPARAM) NULL);

           }

         pDlgData->result.pasteLinkSetting = pasteLinkSetting;
         pDlgData->result.autoUpdateSetting = autoUpdateSetting;
         pDlgData->result.mergeSetting = mergeSetting;
         pDlgData->result.selectedView = selectedView;
         pDlgData->result.selectedKind = selectedKind;

//       pDlgData->result.translateKind = translateKind;
//       pDlgData->result.editor = editor;
         WinDismissDlg(hwnd, TRUE); //WindismissDlg is called with result value TRUE.
        }
             break;

          case DID_CANCEL:

             pDlgData=(PDIALOGDATA) WinQueryWindowPtr(hwnd, QWL_USER);
             pDlgData->result.pasteLinkSetting = kODFalse;
             WinDismissDlg(hwnd, FALSE); //WindismissDlg is called with result value FALSE.
            break;

          case DID_HELP:
           {
             #if (defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))
             ODULong ulHelpid = IDMS_EDIT_PASTEAS_PANEL;
             ODHelp * helpme  = pDlgData->su->GetSession(ev)->GetHelp(ev);
             helpme->DisplayHelp(ev,ODHELPFILE,ulHelpid);
             #endif
           }
     break;
          }  /* end switch WM_COMMAND */

         break;


     case WM_CONTROL:

        switch ( SHORT1FROMMP ( mp1 ) )
          {


                   case IDD_PASTELINKCHKBOX:
                       if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED )
                       {
                        pDlgData=(PDIALOGDATA) WinQueryWindowPtr(hwnd, QWL_USER);
                        if(! WinQueryButtonCheckstate(hwnd, IDD_PASTELINKCHKBOX))
                          {
                           //disable for linking
                           WinEnableWindow(WinWindowFromID(hwnd,IDD_PASTEAUTOBTN), FALSE);
                           WinEnableWindow(WinWindowFromID(hwnd,IDD_PASTEMANUALBTN), FALSE);
                           //Do Paste Content
                           pDlgData->result.pasteLinkSetting = kODFalse;
                          }
                        else
                        {
                           //Enable for linking
                           WinEnableWindow(WinWindowFromID(hwnd,IDD_PASTEAUTOBTN), TRUE);
                           WinEnableWindow(WinWindowFromID(hwnd,IDD_PASTEMANUALBTN), TRUE);
                           pasteLinkSetting = kODTrue;
                        }
                       }
                     break;


                   case IDD_PASTEAUTOBTN:
                       if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED )
                       {
                       autoUpdateSetting = kODTrue;
                       }
                     break;

                   case IDD_PASTEMANUALBTN:
                       if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED )
                       {
                       autoUpdateSetting = kODFalse;
                       }
                     break;

                   case IDD_MERGEBTN:
                       if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED )
                       {
                        mergeSetting = kODTrue;
                         // Inactivate(grey out) the "Embed as:" combo box
                         WinEnableWindow(WinWindowFromID(hwnd,IDD_EMBEDAS), FALSE);
                         WinEnableWindow(WinWindowFromID(hwnd,IDD_PARTKIND), FALSE);
                         WinEnableWindow(WinWindowFromID(hwnd,IDD_PARTEDITOR), FALSE);
                       }
                     break;

                   case IDD_EMBEDBTN:
                       if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED )
                       {
                         // Activate the "Embed as:" combo box
                         WinEnableWindow(WinWindowFromID(hwnd,IDD_EMBEDAS), TRUE);
                         WinEnableWindow(WinWindowFromID(hwnd,IDD_PARTKIND), TRUE);
                         WinEnableWindow(WinWindowFromID(hwnd,IDD_PARTEDITOR), TRUE);
                        mergeSetting = kODFalse;

                         if (!flag)
                           {
                           //Check "Embed as "
                           WinCheckButton(hwnd,IDD_EMBEDBTN,TRUE);
                           //Load data into "Embed As" Combo Box
                            LoadEmbedAsComboBox(hwnd, IDD_EMBEDAS);

                          //Set up Paste As Kind List
                            kindList = new ODTypeList;
                            kindList->InitTypeList(ev, (ODTypeList *) kODNULL);
                            ContentValueTypes(pDlgData->su, kindList);
                            AddTypesToListBox(kindList,hwnd,pDlgData->su->GetSession(ev)
                                             ,IDD_PARTKIND);
                            ODEditor preferredEditor = pDlgData->su->GetSession(ev)->GetBinding(ev)->
                                         ChooseEditorForPart(ev,pDlgData->su,kODNULL);
                            WinSetDlgItemText(hwnd, IDD_PARTEDITOR, preferredEditor);

                            WinSendMsg( WinWindowFromID(hwnd, IDD_PARTKIND),
                                        LM_SELECTITEM, MPFROMSHORT(0),
                                        MPFROMSHORT( TRUE | FALSE));
                       }
                       }
                     break;


           }
        return (MRESULT) TRUE;
        break;


      case WM_DESTROY:

         // Clean up memory

         if (pDlgData){
             delete pDlgData;
             pDlgData = NULL;
           }

          if(kindList)
             {
               delete kindList;
               kindList = NULL;
             }

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;


      default:
         //Pass all unprocessed messages to the default dialog proc

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;

   }  //end switch (msg)

  return (MRESULT) FALSE;
}


/*********************************************************************
 *  Name : LoadEmbedAsComboBox
 *
 *  Description : Loads the EmbedAs Combo Box. The string is added via LM_INSERTITEM
 *
 *  Parameters : hwnd - Dialog Window Handle
 *                    id - Resource ID of the combo box.
 *
 *  Returns: TRUE if list box is loaded successfully, FALSE otherwise
 *
 ****************************************************************/
ODBoolean LoadEmbedAsComboBox(HWND hwnd, ULONG id)
{
   HWND hwndcbox;
   ULONG ulCounter=0;
   CHAR pszTemp[CCHMAXPATH];

   hwndcbox = WinWindowFromID(hwnd,id);

   //All strings must be loaded from the RC file
    HAB hab = WinQueryAnchorBlock(HWND_DESKTOP);

    for (ulCounter=0; ulCounter<=3 ; ulCounter++ ) {
      if (!WinLoadString(hab, hmodDLL, (IDS_FRAME + ulCounter),
                        CCHMAXPATH, (PSZ)pszTemp))
      {
         return FALSE;
      }

    WinSendMsg (hwndcbox,
                LM_INSERTITEM,
                MPFROMSHORT(LIT_END),
                MPFROMP((PSZ)pszTemp));
    } /* endfor */

    WinSendMsg( hwndcbox,
               LM_SELECTITEM, MPFROMSHORT(0),
               MPFROMSHORT( TRUE | FALSE));
    return TRUE;
}

//------------------------------------------------------------------------------
// ShowMoveCopyDialog
//------------------------------------------------------------------------------

ODBoolean ShowMoveCopyDialog(
        ODMoveCopyResult*  result)

{

  ULONG rc = 0;
  CHAR LoadError[CCHMAXPATH];
  Environment* ev = somGetGlobalEnvironment();

  //first check if a file existst in the old path name. If not -
  //then this is definitely a move.
  HDIR dirHandle = HDIR_SYSTEM;
  ULONG searchCount = 1;
  FILEFINDBUF3 fileInfo;
  rc = DosFindFirst((PCSZ)(result->fOldDocPathName), &dirHandle, FILE_ARCHIVED, &fileInfo,
                           sizeof(fileInfo), &searchCount, FIL_STANDARD);
  if (rc == ERROR_NO_MORE_FILES)
  {
     result->FileMoved   =   kODTrue;
     result->FileCopied  =   kODFalse;
     return rc;
  }


  DosLoadModule(LoadError, sizeof(LoadError), "ODRes", &hmodDLL);

  result->FileMoved   =   kODTrue;
  result->FileCopied  =   kODFalse;

  //Create a modal dialog box and display it.
  rc = WinDlgBox(HWND_DESKTOP,               //Parent is the DeskTop
            HWND_DESKTOP,                    //displayed in DeskTop window
            (PFNWP)MoveCopyDlgProc,          //dialog window proc
            hmodDLL,                         //handle to the DLL containing Dialog resource.
            IDD_MOVECOPYDLG,                 //dialog template ID
            (PVOID) result );           //application data

  return rc;
}


/*********************************************************************
 *  Name : MoveCopyDlgProc
 *
 *************************************************************************/
MRESULT EXPENTRY MoveCopyDlgProc( HWND hwnd, ULONG msg, MPARAM mp1,
                                  MPARAM mp2)
{

RECTL rclScreen, rclDialog;
LONG sWidth,sHeight, sBLCx,sBLCy;
static ODMoveCopyResult *pDlgData = NULL;
USHORT okMoved  = MBID_ENTER;
USHORT okCopied = MBID_ENTER;
Environment* ev = somGetGlobalEnvironment();


   switch(msg)
   {
      case WM_INITDLG:
              {

            //Center the dialog window and get the screen rectange
            WinQueryWindowRect(HWND_DESKTOP, &rclScreen);

            //Get the dialog-window rectange
            WinQueryWindowRect(hwnd, &rclDialog);

            //Get the dialog - window width;
            sWidth = (LONG) (rclDialog.xRight - rclDialog.xLeft);

            //Get the dialog - window height;
            sHeight = (LONG) (rclDialog.yTop - rclDialog.yBottom);

            //Set the horizontal coordinate of the lower-left corner
            sBLCx = ((LONG) rclScreen.xRight - sWidth) /2;

            //Set the vertical   coordinate of the lower-left corner
            sBLCy = ((LONG) rclScreen.yTop - sHeight) /2;

            //Move, size and show the window

            WinSetWindowPos(hwnd,
                                HWND_TOP,
                                sBLCx, sBLCy,
                                0,0,                      //ignore size arguments
                                SWP_MOVE);


            pDlgData = (ODMoveCopyResult*) mp2;
            WinSetDlgItemText(hwnd, IDD_OLDFILENAME, pDlgData->fOldDocPathName);
            WinSetDlgItemText(hwnd, IDD_NEWFILENAME, pDlgData->fNewDocPathName);

             // Set Window pointer with object pointer and instance data info
             WinSetWindowPtr(hwnd, QWL_USER, pDlgData);


         }
      break;

      case WM_COMMAND:

         switch (SHORT1FROMMP (mp1) )
         {
          case DID_FILECOPIED:
             pDlgData=(ODMoveCopyResult *) WinQueryWindowPtr(hwnd, QWL_USER);
             pDlgData->FileCopied = kODTrue;
             pDlgData->FileMoved  = kODFalse;
             char       messageCopied[256];
             strcpy(messageCopied, "This Document will not be responsible for any sources previously owned by: \n");
             strcat(messageCopied, pDlgData->fOldDocPathName);
             okCopied = WinMessageBox(HWND_DESKTOP, hwnd, messageCopied, "Notice:",0, MB_MOVEABLE | MB_OKCANCEL);
             if (okCopied == MBID_OK)
             {
              WinDismissDlg(hwnd, TRUE); //WindismissDlg is called with result value TRUE.
             }
            break;

          case DID_FILEMOVED:
             pDlgData=(ODMoveCopyResult *) WinQueryWindowPtr(hwnd, QWL_USER);
             pDlgData->FileCopied = kODFalse;
             pDlgData->FileMoved  = kODTrue;
             char       messageMoved[256];
             strcpy(messageMoved, "This Document will now be responsible for all sources previously owned by: \n");
             strcat(messageMoved, pDlgData->fOldDocPathName);
             okMoved = WinMessageBox(HWND_DESKTOP, hwnd, messageMoved, "Notice:",0, MB_MOVEABLE | MB_OKCANCEL);
             if (okMoved == MBID_OK)
             {
               WinDismissDlg(hwnd, TRUE); //WindismissDlg is called with result value TRUE.
             }
            break;


          case DID_CANCEL:

             pDlgData=(ODMoveCopyResult *) WinQueryWindowPtr(hwnd, QWL_USER);
             WinDismissDlg(hwnd, FALSE); //WindismissDlg is called with result value FALSE.
            break;

//          case DID_MOVECOPYHELP:
//           break;

          }  /* end switch WM_COMMAND */

         break;


      case WM_DESTROY:

         // Clean up memory

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;


      default:
         //Pass all unprocessed messages to the default dialog proc

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;

     } //end switch (msg)

  return (MRESULT) FALSE;
}

//--------------------------------------------------------------------------
//
// This function will center a Dialog Window over it's parent window.
//
//--------------------------------------------------------------------------

void CenterWindow (HWND hwnd)
{
         HWND hwndParent;
         RECTL rectl;
         RECTL rectlParent;

         WinQueryWindowRect(hwnd,&rectl);

         hwndParent=WinQueryWindow(hwnd,QW_PARENT);
         WinQueryWindowRect(hwndParent,&rectlParent);

         rectl.xRight/=2;
         rectl.yTop/=2;
         rectlParent.xRight/=2;
         rectlParent.yTop/=2;

         WinSetWindowPos(hwnd,
                         (HWND)NULL,
                         (LONG)(rectlParent.xRight-rectl.xRight),
                         (LONG)(rectlParent.yTop-rectl.yTop),
                         0l,
                         0l,
                         (ULONG)SWP_MOVE );
}

//------------------------------------------------------------------------------
// AddTypesToListBox
//------------------------------------------------------------------------------


void AddTypesToListBox(ODTypeList* typeList, HWND kindMenu,
                          ODSession* session, ODULong idItem)
{
  ISOString        name;
  char * pWorkArea = kODNULL;
  ODSShort      item = 0;
  Environment*    ev = somGetGlobalEnvironment();
  ODNameSpaceManager*  nsm = session->GetNameSpaceManager(ev);
  HWND hwndListBox = WinWindowFromID(kindMenu, idItem);

  ODTypeListIterator* typeIter = typeList->CreateTypeListIterator(ev);

   ODPartHandlerRegistryInt* odRegistry;
   odRegistry = session -> GetRegistry(ev);

  for (ODType type = typeIter->First(ev);
    typeIter->IsNotComplete(ev);
    type = typeIter->Next(ev))
  {

    name =  odRegistry->GetKindDisplayNameFromKind(ev, type);
    if ( (strcmp(name, "GetKindDisplayName: No PartKind matches partKindName") != 0)
              && (strcmp(name, "GetKindDisplayName: No Parts found") != 0) )
    //if (name =  odRegistry->GetKindDisplayNameFromKind(ev, type) )
    {
      WinInsertLboxItem(hwndListBox, LIT_FIRST, name);


    }
    else
    {
      // No available editors support this type
      ODTranslation* translation = session->GetTranslation(ev);
      ODPlatformType platformType = translation->GetPlatformTypeFromISOType(ev, type);
      if ( platformType == kODNULL )
         pWorkArea = catgets(ODcat_handle, LINKSPEC_SET, LINKSPEC_80, "No user kind");
      else {
         pWorkArea = new char[80];
         switch ( (UINT)platformType )
         {
         case CF_TEXT:
                ODBlockMove("TEXT", pWorkArea, 5);
                break;
         case CF_BITMAP:
                ODBlockMove("BITMAP", pWorkArea, 7);
                break;
         case CF_DSPTEXT:
                ODBlockMove("DSPTEXT", pWorkArea, 13);
                break;
         case CF_DSPBITMAP:
                ODBlockMove("DSPBITMAP", pWorkArea, 5);
                break;
         case CF_DSPMETAFILE:
                ODBlockMove("DSPMETAFILE", pWorkArea, 4);
                break;
         case CF_METAFILE:
                ODBlockMove("METAFILE", pWorkArea, 5);
                break;
         case CF_PALETTE:
                ODBlockMove("PALETTE", pWorkArea, 8);
                break;
         default:
                if (!WinQueryAtomName(WinQuerySystemAtomTable(), platformType, pWorkArea, 80)) {
                        delete pWorkArea;
                        pWorkArea = catgets(ODcat_handle, LINKSPEC_SET, LINKSPEC_80, "No user kind");
                }
                break;
             }
      }
      if(strcmp(pWorkArea, kODDragitem) != 0 &&
         strcmp(pWorkArea, kODSelectedRMF) != 0 )
          WinInsertLboxItem(hwndListBox, LIT_FIRST, pWorkArea);
   }

  }
  ODDisposePtr(pWorkArea);
  delete typeIter;
}

//------------------------------------------------------------------------------
// ShowLinkDestinationInfoDlg
//------------------------------------------------------------------------------
ODBoolean ShowLinkDestinationInfo(
               ODBaseLink*         link,
               ODLinkInfo*         info,
               ODBoolean           ChangesAllowed,
               HWND                    hwnd,
               ODFacet*                facet,
               ODLinkInfoResult*   infoResult)

{
   CHAR LoadError[CCHMAXPATH];
   ULONG rc = 0;

   Environment *ev = somGetGlobalEnvironment();

   gLIResult = infoResult;
   gLink     = link;
   gChangesAllowed = ChangesAllowed;

   gFacet = facet;

   // Load the resource file
   DosLoadModule(LoadError, sizeof(LoadError), "ODRes", &hmodDLL);

  // Need to acquire frame and storage unit before using them. Put
  // them in temp object so that when the temp object is out of
  // scope, the object get released.
  TempODFrame frame = facet->GetFrame(ev);
  frame->Acquire(ev);
  TempODStorageUnit su = frame->GetStorageUnit(ev);
  su->Acquire(ev);
  ODSession* session = su->GetSession(ev);

  ODWindowState* WinStat = session->GetWindowState(ev);

  // Need to release the window. Use temp object to accomplish
  // the release when the temp object is out of scope.
  TempODWindow Window = WinStat->AcquireFrontRootWindow(ev);
  HWND Temphwnd = Window->GetPlatformWindow(ev);

  //Create a modal dialog box and display it.
  rc = WinDlgBox(HWND_DESKTOP,           //Parent is the DeskTop
//            HWND_DESKTOP,
            Temphwnd,
            (PFNWP)LinkDestInfoDlgProc,  //dialog window proc
            hmodDLL,                     //handle to the DLL containing Dialog resource.
            IDD_LDPAGEDLG,               //dialog template ID
            (PVOID) info);               //application data

   if (gLIResult){
       return kODTrue;
   }else
       return kODFalse;
}



/*********************************************************************
 *  Name : LinkDestInfoDlgProc
 *
 *************************************************************************/
MRESULT EXPENTRY LinkDestInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1,
                                      MPARAM mp2)
{

Environment* ev = somGetGlobalEnvironment();

   ODBoolean needsUpdate;
   static ODBoolean firstTime;
   static ODLinkInfo* info;

   static wchar_t wcsDate[kODDateLength]    = L"";
   char* mbsDate;

   switch(msg)
   {
      case WM_INITDLG:
      {

            info = (ODLinkInfo*)mp2;

            // set the Kind
            WinSetDlgItemText(hwnd, IDD_LDKINDTXT, info->kind);

            // set up created
            time_t theTime = info->creationTime;
	    if (theTime) {
	       char* dateFormat = nl_langinfo(D_T_FMT); 
	       struct tm *timeptr = localtime((const time_t*) &theTime);

	       wchar_t wcsDateFormat [kODDateLength * kODWcsLength];
	       mbstowcs (wcsDateFormat, dateFormat, kODDateLength * kODWcsLength);
	       wcsftime(wcsDate, kODDateLength*kODWcsLength, wcsDateFormat, timeptr);
	       mbsDate = (char*) malloc (kODDateLength*kODWcsLength);
	       wcstombs(mbsDate, wcsDate, kODDateLength*kODWcsLength);
               WinSetDlgItemText(hwnd, IDD_LDCREATEDTXT, mbsDate);
	       free(mbsDate);
	    }

            // set up modified
            theTime = info->changeTime;
	    if (theTime) {
	       char* dateFormat = nl_langinfo(D_T_FMT); 
	       struct tm *timeptr = localtime((const time_t*) &theTime);

	       wchar_t wcsDateFormat [kODDateLength * kODWcsLength];
	       mbstowcs (wcsDateFormat, dateFormat, kODDateLength * kODWcsLength);
	       wcsftime(wcsDate, kODDateLength*kODWcsLength, wcsDateFormat, timeptr);
	       mbsDate = (char*) malloc (kODDateLength*kODWcsLength);
	       wcstombs(mbsDate, wcsDate, kODDateLength*kODWcsLength);
               WinSetDlgItemText(hwnd, IDD_LDUPDATEDTXT, mbsDate);
	       free(mbsDate);

	    }
       
	    needsUpdate = (info->change == kODUnknownUpdate) || (info->change != gLink->GetUpdateID(ev));
			
            gLIResult->autoUpdate = info->autoUpdate;
            firstTime = kODTrue;
            WinCheckButton(hwnd,IDD_LDAUTOMATICALLYBTN,(gLIResult->autoUpdate));
            WinCheckButton(hwnd,DID_LDMANUALLYBTN,!(gLIResult->autoUpdate));
	    // Disable Update Now button if updates are automatic or link is up to date.
	    if ( info->autoUpdate || !needsUpdate ) {

                WinEnableWindow(WinWindowFromID(hwnd,DID_LDUPDATENOWBTN), FALSE);

            }else{
                WinEnableWindow(WinWindowFromID(hwnd,DID_LDUPDATENOWBTN), TRUE);
            }


            WinSetWindowPtr(hwnd, QWL_USER, info);


      }
      return FALSE;
      break;
      case WM_HELP:
            {
              #if (defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))
              ODFrame   *frame   = gFacet->GetFrame(ev);
              ODSession* session = frame->GetStorageUnit(ev)->GetSession(ev);
              ODHelp   * helpme  = session->GetHelp(ev);
              helpme->DisplayHelp(ev,ODHELPFILE,IDD_LTPAGEDLG_PANEL);
              #endif
            }
      break;

      case WM_CONTROL:
         switch (SHORT1FROMMP (mp1) )
         {
          case IDD_LDAUTOMATICALLYBTN:
              // we get this message when the dialog comes up; don't want to act on it.
              if(firstTime)
                 firstTime = kODFalse;
              else if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED ) {
                   WinCheckButton(hwnd,IDD_LDAUTOMATICALLYBTN,TRUE);
                   WinCheckButton(hwnd,DID_LDMANUALLYBTN,FALSE);
                   gLIResult->autoUpdate = kODTrue;
                   WinEnableWindow(WinWindowFromID(hwnd,DID_LDUPDATENOWBTN), FALSE);
              }
          break;

          case DID_LDMANUALLYBTN:
              if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED ) {
                   WinCheckButton(hwnd,DID_LDMANUALLYBTN,TRUE);
                   WinCheckButton(hwnd,IDD_LDAUTOMATICALLYBTN,FALSE);
                   gLIResult->autoUpdate = kODFalse;
	           needsUpdate = (info->change == kODUnknownUpdate) || (info->change != gLink->GetUpdateID(ev));
	           if (needsUpdate) {
                     WinEnableWindow(WinWindowFromID(hwnd,DID_LDUPDATENOWBTN), TRUE);
                   }
              }
          break;
         }
      break;

      case WM_COMMAND:

         switch (SHORT1FROMMP (mp1) )
         {
          case DID_OK:
            gLIResult->action = kODLinkInfoOk;
            WinDismissDlg(hwnd, TRUE);
          break;

          case DID_CANCEL:
              gLIResult = kODNULL;
              WinDismissDlg(hwnd, FALSE);
          break;

          case DID_LDBREAKLINKBTN:
              gLIResult->action = kODLinkInfoBreakLink;
              WinDismissDlg(hwnd, TRUE);
          break;

          case DID_LDUPDATENOWBTN:
              gLIResult->action = kODLinkInfoUpdateNow;
              WinDismissDlg(hwnd, TRUE);
          break;

          case IDD_LDFINDSOURCEBTN:
              gLIResult->action = kODLinkInfoFindSource;
              WinDismissDlg(hwnd, TRUE);
          break;

          case DID_HELP:
              {
                // We need to get a handle to the session object here...
                ODFrame   *frame   = gFacet->GetFrame(ev);
                ODSession* session = frame->GetStorageUnit(ev)->GetSession(ev);
                ODHelp   * helpme  = session->GetHelp(ev);
                helpme->DisplayHelp(ev,ODHELPFILE,IDD_LTPAGEDLG_PANEL);
              }
              break;

          }  /* end switch WM_COMMAND */

         break;


      case WM_DESTROY:

         // Clean up memory

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;


      default:
         //Pass all unprocessed messages to the default dialog proc

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;

     } //end switch (msg)

  return (MRESULT) FALSE;
}



//------------------------------------------------------------------------------
// ShowLinkSourceInfo
//------------------------------------------------------------------------------
ODBoolean ShowLinkSourceInfo(
               ODBaseLinkSource*   linkSource,
               ODUpdateID          change,
               ODBoolean           changesAllowed,
               HWND                hwnd,
               ODFacet*            facet,
               ODLinkInfoResult*   infoResult)

{
   CHAR LoadError[CCHMAXPATH];
   ULONG rc = 0;
   Environment *ev = somGetGlobalEnvironment();

   gLIResult = infoResult;

   gFacet = facet;

   gSession = gFacet->GetFrame(ev)->GetStorageUnit(ev)->GetSession(ev);
   gUpdateID = change;
   gChangesAllowed = changesAllowed;

   // Load the resource file
   DosLoadModule(LoadError, sizeof(LoadError), "ODRes", &hmodDLL);

  // Need to acquire frame and storage unit before using them. Put
  // them in temp object so that when the temp object is out of
  // scope, the object get released.
  TempODFrame frame = facet->GetFrame(ev);
  frame->Acquire(ev);
  TempODStorageUnit su = frame->GetStorageUnit(ev);
  su->Acquire(ev);
  ODSession* session = su->GetSession(ev);

  ODWindowState* WinStat = session->GetWindowState(ev);

  // Need to release the window. Use temp object to accomplish
  // the release when the temp object is out of scope.
  TempODWindow Window = WinStat->AcquireFrontRootWindow(ev);
  HWND Temphwnd = Window->GetPlatformWindow(ev);

   //Create a modal dialog box and display it.
   rc = WinDlgBox(HWND_DESKTOP,           //Parent is the DeskTop
//             HWND_DESKTOP,
             Temphwnd,
             (PFNWP)LinkSourceInfoDlgProc,  //dialog window proc
             hmodDLL,                     //handle to the DLL containing Dialog resource.
             IDD_LSPAGEDLG,               //dialog template ID
             (PVOID) linkSource);         //application data

   if (gLIResult){
       return kODTrue;
   }else
       return kODFalse;
}


/*********************************************************************
 *  Name : LinkSourceInfoDlgProc
 *
 *************************************************************************/
MRESULT EXPENTRY LinkSourceInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1,
                                        MPARAM mp2)
{

   Environment *ev = somGetGlobalEnvironment();
   static ODNameSpaceManager* nsm;

   static ODBaseLinkSource* linkSource;
   ODBoolean needsUpdate;
   static ODBoolean firstTime;

   static wchar_t wcsDate[kODDateLength]    = L"";
   char* mbsDate;

   switch(msg)
   {
      case WM_INITDLG:
      {

            linkSource = (ODBaseLinkSource*)mp2;

            ODLinkKey key;
            if (linkSource->Lock(ev, 0, &key))
            {
              TRY
                ODStorageUnit *contentSU = linkSource->GetContentStorageUnit(ev, key);

                // set the Kind
                ODULong kindLen;
                ODType  kind;
                GetBestKindAndSize(contentSU, &kind, &kindLen);
                if (kindLen)
                    WinSetDlgItemText(hwnd, IDD_LSKINDTXT,kind);

                // set up created

                // ODGetPOCreationDate is replaced by ODGetCreationDate; pass in SU instead
                // of PersistentObject ptr.
                // time_t theTime = ODGetPOCreationDate(ev, linkSource);
                //  time_t theTime = ODGetCreationDate(ev, contentSU);

                time_t theTime = ODGetCreationDate(ev, ODGetSUFromPstObj(ev, linkSource));
	        if (theTime) {
	           char* dateFormat = nl_langinfo(D_T_FMT); 
      	           struct tm *timeptr = localtime((const time_t*) &theTime);

	           wchar_t wcsDateFormat [kODDateLength * kODWcsLength];
	           mbstowcs (wcsDateFormat, dateFormat, kODDateLength * kODWcsLength);
	           wcsftime(wcsDate, kODDateLength*kODWcsLength, wcsDateFormat, timeptr);
	           mbsDate = (char*) malloc (kODDateLength*kODWcsLength);
	           wcstombs(mbsDate, wcsDate, kODDateLength*kODWcsLength);
                   WinSetDlgItemText(hwnd, IDD_LSCREATEDTXT, mbsDate);
	           free(mbsDate);
 	        }


                // set up modified
                theTime = linkSource->GetChangeTime(ev);
	        if (theTime) {
	           char* dateFormat = nl_langinfo(D_T_FMT); 
      	           struct tm *timeptr = localtime((const time_t*) &theTime);

	           wchar_t wcsDateFormat [kODDateLength * kODWcsLength];
	           mbstowcs (wcsDateFormat, dateFormat, kODDateLength * kODWcsLength);
	           wcsftime(wcsDate, kODDateLength*kODWcsLength, wcsDateFormat, timeptr);
	           mbsDate = (char*) malloc (kODDateLength*kODWcsLength);
	           wcstombs(mbsDate, wcsDate, kODDateLength*kODWcsLength);
                   WinSetDlgItemText(hwnd, IDD_LSUPDATEDTXT, mbsDate);
	           free(mbsDate);
 	        }

              CATCH_ALL
                    linkSource->Unlock(ev, key);
                    RERAISE;
              ENDTRY
              linkSource->Unlock(ev, key);
            }
	    needsUpdate = (gUpdateID == kODUnknownUpdate) || (gUpdateID != linkSource->GetUpdateID(ev));
			
	    gLIResult->autoUpdate = linkSource->IsAutoUpdate(ev);
            firstTime = kODTrue;
            WinCheckButton(hwnd,IDD_LSONSAVEBTN,(gLIResult->autoUpdate));
            WinCheckButton(hwnd,DID_LSMANUALLYBTN,!(gLIResult->autoUpdate));

	    if ( gLIResult->autoUpdate || !needsUpdate ) {
                WinEnableWindow(WinWindowFromID(hwnd,DID_LSUPDATENOWBTN), FALSE);
            }else{
                WinEnableWindow(WinWindowFromID(hwnd,DID_LSUPDATENOWBTN), TRUE);
            }

            WinSetWindowPtr(hwnd, QWL_USER, linkSource);

      }
      return FALSE;
      break;
      case WM_HELP:
            {
              ODFrame   *frame   = gFacet->GetFrame(ev);
              ODSession* session = frame->GetStorageUnit(ev)->GetSession(ev);
              ODHelp   * helpme  = session->GetHelp(ev);
              helpme->DisplayHelp(ev,ODHELPFILE,IDD_LSPAGEDLG_PANEL);

            }
      break;

      case WM_CONTROL:
         switch (SHORT1FROMMP (mp1) )
         {
          case IDD_LSONSAVEBTN:
              // we get this message when the dialog comes up; don't want to act on it.
              if(firstTime)
                 firstTime = kODFalse;
              else if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED ) {
                   WinCheckButton(hwnd,IDD_LSONSAVEBTN,TRUE);
                   WinCheckButton(hwnd,DID_LSMANUALLYBTN,FALSE);
                   gLIResult->autoUpdate = kODTrue;
                   WinEnableWindow(WinWindowFromID(hwnd,DID_LSUPDATENOWBTN), FALSE);
              }
          break;

          case DID_LSMANUALLYBTN:
              if ((USHORT) SHORT2FROMMP (mp1) == BN_CLICKED ) {
                   WinCheckButton(hwnd,DID_LSMANUALLYBTN,TRUE);
                   WinCheckButton(hwnd,IDD_LSONSAVEBTN,FALSE);
                   gLIResult->autoUpdate = kODFalse;
	           needsUpdate = (gUpdateID == kODUnknownUpdate) || (gUpdateID != linkSource->GetUpdateID(ev));
	           if (needsUpdate) {
                     WinEnableWindow(WinWindowFromID(hwnd,DID_LSUPDATENOWBTN), TRUE);
                   }
              }
          break;
         }
      break;

      case WM_COMMAND:

         switch (SHORT1FROMMP (mp1) )
         {
          case DID_OK:
            gLIResult->action = kODLinkInfoOk;
            WinDismissDlg(hwnd, TRUE);
          break;

          case DID_CANCEL:
              gLIResult = kODNULL;
              WinDismissDlg(hwnd, FALSE);
          break;

          case DID_LSBREAKLINKBTN:
              gLIResult->action = kODLinkInfoBreakLink;
              WinDismissDlg(hwnd, TRUE);
          break;

          case DID_LSUPDATENOWBTN:
              gLIResult->action = kODLinkInfoUpdateNow;
              WinDismissDlg(hwnd, TRUE);
          break;

          case DID_HELP:
              {
                // We need to get a handle to the session object here...
                ODFrame   *frame   = gFacet->GetFrame(ev);
                ODSession* session = frame->GetStorageUnit(ev)->GetSession(ev);
                ODHelp   * helpme  = session->GetHelp(ev);
                helpme->DisplayHelp(ev,ODHELPFILE,IDD_LSPAGEDLG_PANEL);
              }
              break;


          }  /* end switch WM_COMMAND */

         break;


      case WM_DESTROY:

         // Clean up memory

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;


      default:
         //Pass all unprocessed messages to the default dialog proc

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;

     } //end switch (msg)

  return (MRESULT) FALSE;
}


//------------------------------------------------------------------------------
// GetBestKindAndSize
#ifdef DEFECT_22109
// note:  Will want to use GetUserKindForKind here when 22109 is resolved.
#endif
//------------------------------------------------------------------------------
ODStatic void GetBestKindAndSize(ODStorageUnit* contentSU, ODType* kind, ODULong* size)
{
   Environment* ev = somGetGlobalEnvironment();

   if (contentSU->Exists(ev, kODPropContents, (ODValueType)kODNULL, 1))
   {
       contentSU->Focus(ev, kODPropContents, kODPosUndefined, (ODValueType)kODNULL, 1, kODPosUndefined);
       *kind = contentSU->GetType(ev);
       *size = contentSU->GetSize(ev);
   }
   else
   {
       *kind = (ODType) kODNULL;
       *size = 0;
   }
}


//------------------------------------------------------------------------------
// ShowLinkUpdateAlert
//------------------------------------------------------------------------------

ODBoolean ShowLinkUpdateAlert()
{

   CHAR LoadError[CCHMAXPATH];
   ULONG rc = 0;
   HSWITCH hswitch;
   SWCNTRL swcntrl;
   PTIB    ptib;
   PPIB    ppib;
   PID     pid;
   HWND    hwnd = HWND_DESKTOP;

   Environment *ev = somGetGlobalEnvironment();

   gAlertResult = kLinkUpdateStopBtn;

   // Load the resource file
   DosLoadModule(LoadError, sizeof(LoadError), "ODRes", &hmodDLL);
   
   // get the current hwnd so the dialog can be made modal.
   if(DosGetInfoBlocks(&ptib, &ppib) == 0) {
     pid = ppib->pib_ulpid;
     if ((hswitch = WinQuerySwitchHandle(0, pid) ) != NULLHANDLE)
       if (!WinQuerySwitchEntry(hswitch, &swcntrl))
	 hwnd = swcntrl.hwnd;
   }
	
   //Create a modal dialog box and display it.
   rc = WinDlgBox(HWND_DESKTOP,
             hwnd,
             (PFNWP)LinkUpdateAlertDlgProc,  //dialog window proc
             hmodDLL,                        //handle to the DLL containing Dialog resource.
             IDD_LUDLG,                      //dialog template ID
             NULL);

   return (gAlertResult == kLinkUpdateContinueBtn);
}


/*********************************************************************
 *  Name : LinkUpdateAlertDlgProc
 *
 *************************************************************************/
MRESULT EXPENTRY LinkUpdateAlertDlgProc( HWND hwnd, ULONG msg, MPARAM mp1,
                                        MPARAM mp2)
{

   Environment *ev = somGetGlobalEnvironment();
   static ODNameSpaceManager* nsm;


   switch(msg)
   {
      case WM_INITDLG:
      {

            WinSetWindowPtr(hwnd, QWL_USER, mp2);

      }
      return FALSE;
      break;

      case WM_COMMAND:

         switch (SHORT1FROMMP (mp1) )
         {
          case IDD_LUUPDATE:
            gAlertResult = kLinkUpdateContinueBtn;
            WinDismissDlg(hwnd, TRUE);
          break;
          case IDD_LUSTOP:
            gAlertResult = kLinkUpdateStopBtn;
            WinDismissDlg(hwnd, TRUE);
          break;
          }  /* end switch WM_COMMAND */

         break;


      case WM_DESTROY:

         // Clean up memory

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;


      default:
         //Pass all unprocessed messages to the default dialog proc

         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;

     } //end switch (msg)

  return (MRESULT) FALSE;
}


#endif //  _PLATFORM_OS2_

#ifdef _PLATFORM_MACINTOSH_         //Mac specific file - not used by IBM
/*
        File:           LinkDlgs.cpp

        Contains:       implementation of the Paste As and Link Info dialogs

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <58>    10/16/95        CC              GM 1292157: Moved TranslateValueTypes,
                                                                        AddTranslationKindToMenu,
                                                                        GetMenuStringForKind,
                                                                        DefaultMenuStringForKind,
                                                                        StringPtrFromStrHandle to InfoUtil.cpp.
                <57>    10/10/95        DM              1285040: GM: PasteAs into part viewed in
                                                                        window hangs (dont assume persistent frame
                                                                        in ShowPasteAsDialog())
                <56>     10/3/95        eeh             1284995: new default param to
                                                                        SetDialogTextStyle
                <55>     10/3/95        TJ              Changes done by RefBall Team
                <54>     9/12/95        DM              1280020 FB1 part trans dlg mem leaks
                <53>      9/8/95        T‚              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
                <52>      9/1/95        CC              1279449: Initialize dlg to null & use
                                                                        ODVolatile.
                                                                        ShowLinkSourceInfo,
                                                                        ShowLinkDestinationInfo: Use ODVolatile.
                <51>     8/29/95        DM              1221778, 1276165: [ShowLinkUpdateAlert]
                                                                        drop count arg in ShowLinkUpdateAlert
                <50>     8/26/95        T‚              1274606 FB2: Patching Remarks
                <49>     8/25/95        eeh             1230007: new param to DrawITextInDlogBox
                <48>     8/23/95        CC              1278378: ShowPasteAsDialog behaves
                                                                        incorrectly on switch from Embed to Merge.
                                                                        1242581: ShowPasteAsDialog should respect
                                                                        preferred editor.
                                                                        1278840: ShowPasteAsDialog should respect
                                                                        preferred kind.
                <47>     8/15/95        CC              1276520: Adapted to changes to
                                                                        ShowTranslateDialog.
                <46>     8/12/95        T‚              1276812 Need to use TempObjs and TempRefs
                                                                        for exception safety and to avoid TRY
                                                                        blocks, 1276807 Opt./Bug: use StdTypIO
                                                                        routines for portable streaming & smaller
                                                                        footprint
                <45>     7/28/95        CC              1242576: Removed obsolete Others dialog
                                                                        code.
                <44>     7/24/95        CC              1236616: Use CreateTypeList factory method.
                <43>     7/18/95        eeh             1222282: move constants to DlogUtil.h
                <42>     6/21/95        CC              1260937: BB: ODTypeListIterator must return
                                                                        copies of ODType values.
                <41>     6/16/95        CC              1258538: ShowPasteAsDialog: Release kind
                                                                        and editor menu resources and delete the
                                                                        menu; also check GetMenu result.
                                                                        1257141: TranslateValueTypes: Changed
                                                                        translateFromList parameter to
                                                                        OrderedCollection. Added
                                                                        GetIndexedElement() function.
                                                                        SetDateTimeStrings: Added
                                                                        CUsingLibraryResources.
                                                                        DefaultMenuStringForKind: Added
                                                                        CUsingLibraryResources.
                <40>      6/7/95        jpa             Fixed EndUsingLibRsrcs calls. [1255369]
                <39>      6/7/95        CC              1236299:  Wrap individual calls with using
                                                                        library resources calls.
                <38>      6/2/95        CC              1255476: ShowPasteAsDialog: Release object
                                                                        returned by ODFrame::AcquirePart().
                <37>     5/30/95        CG              #1251234 Extraneous items in Paste As kind
                                                                        popup.
                <36>     5/26/95        VL              1251403: Multithreading naming support.
                <35>     5/18/95        CG              #1249263 BB: BndNSUtl.cpp does not compile
                                                                        standalone.
                <34>      5/1/95        CG              #1240845        BB: Kinds popups need to include
                                                                        all kinds that an editor can edit.
                <33>     4/29/95        T‚              1243222 BB: New documents from stationery
                                                                        should have create/modinfo reset.
                <32>     4/27/95        CC              1243348: Moved ShowAlert into
                                                                        DlogUtils.cpp.
                                                                        ShowLinkInfo: Don't alert user on break
                                                                        link (problems with nested calls use
                                                                        library resources).
                <31>     4/25/95        CC              1242555: TranslateValueTypes: $5 entered as
                                                                        bug 1242571.
                                                                        1242555: ShowPasteAsDialog: $5 entered as
                                                                        bug 1242581.
                                                                        1242555:  OthersDlgFilterProc: Removed
                                                                        obsolete $5 comment.
                <30>     4/14/95        T‚              #1235279 BB: InfoUtil & StdTypIO functions
                                                                        should take Environment* and SU* when
                                                                        possible
                <29>     3/28/95        CC              1229640: Added LinkInfoFilterProc and
                                                                        PasteAsFilterProc.
                                                                        Added DeavtivateModalDialog routine.
                                                                        DrawKindName and DrawEditorName calls
                                                                        DrawITextInDlogBox utility.
                <28>     3/27/95        CC              1229640: Use ODTranslation::
                                                                        ShowTranslateDialog() in ShowPasteAsDialog.
                                                                        Show editor in Paste As dialog.
                <27>     3/22/95        CC              1230322: Changed parameter to
                                                                        ShowPasteAsDialog(); removed
                                                                        DefaultMergeSetting().
                <26>     3/17/95        CC              1203315: Added DefaultMergeSetting(),
                                                                        EnablePasteAsOkButton() and
                                                                        DrawDeactivePICTItem().
                                                                        Removed InitialMergeSetting() &
                                                                        DrawDeactiveDialogItem().
                                                                        Fix to PrepareToDrawGray to set the pen
                                                                        pattern to gray if no gray color is
                                                                        available.
                <25>      3/9/95        CC              1221471: Added changesAllowed parameter to
                                                                        ShowLinkDestinationInfo() and
                                                                        ShowLinkSourceInfo().
                <24>      3/9/95        CC              1220342: DrawIntlStaticText: Save and
                                                                        restore font; don't change font if dialog
                                                                        font is same script as intl text.
                                                                        defined ODDisposeRoutineDescriptor and
                                                                        ODDisposeDlg macros.
                                                                        MakeOthersList(): don't set window font and
                                                                        size; adopt font and size as determined by
                                                                        'finf' resource.
                                                                        ShowOthersDialog: removed okButtonEnabled.
                                                                        Moved DrawDefaultButtonOutline to DlogUtils
                                                                        and renamed.
                <23>      3/7/95        CC              1226083: Removed SetKindText(); added
                                                                        DrawIntlStaticText() and gKindName global.
                <22>      3/6/95        jpa             Use dialog utilities (ODGetNewDialog,
                                                                        ODDialogFilterProc) to make dlogs movable.
                                                                        [1218957]
                <21>      3/3/95        CC              1199186 Save and restore the current port
                                                                        in ShowPasteAsDialog, ShowLinkSourceInfo,
                                                                        and ShowLinkDestinationInfo.
                                                                        1223607 Removed link size from info
                                                                        dialogs;
                                                                        GetBestKindAndSize->BestContentKind.
                                                                        1224967 Time updated was incorrect.
                                                                        1223861 Added ShowLinkStatusAlert().
                <20>     2/24/95        jpa             Use ODNewRgn. [1220810]
                <19>     2/24/95        CC              1220342: GetMenuStringForKind: Return
                                                                        script code.
                                                                        1220342: AddTranslationKindToMenu: Set
                                                                        script code for menu item.
                                                                        1220339: CreateTranslationStrings: Modified
                                                                        to build StringWithScriptCodeArrayPtr.
                <18>     2/15/95        CC              Lots of changes:
                                                                        1194329 UpdateNow button in info dialogs is
                                                                        enabled only if "Manually" is selected and
                                                                        the link (or link destination) needs
                                                                        updating.
                                                                        1220318 Added LinkDialogFilter.
                                                                        1220318 ShowLinkInfo -- Use
                                                                        LinkDialogFilter in call to ModalDialog.
                                                                        1205033 LinkInfo dialogs should be static
                                                                        if draft is read-only
                                                                        1218931 Link Info Dialog changes when draft
                                                                        is read-only
                                                                        1196831 Show link size in K bytes if 1K or
                                                                        larger.
                                                                        1220327 Data Interchange dialogs need to
                                                                        use filter procs.
                                                                        1153942 Changes to Destination Link Info
                                                                        dialog
                <17>      2/7/95        CC              1211295: Added GetOriginalDraft() and
                                                                        SUClonedFromDraft().  ShowPasteAsDialog()
                                                                        takes an isMove parameter.
                                                                        1216124: ShowPasteAsDialog calls
                                                                        ODWindowState::DeactivateFrontWindow() and
                                                                        ActivateFrontWindow().
                <16>     1/25/95        jpa             New ODIText format [1212619]
                <15>     1/25/95        CC              1213198 ShowOthersDialog: Call LDispose
                                                                        BEFORE disposing of the LDEF.
                                                                        1213954 Initialize editor field of
                                                                        ODPasteAsResult to kODNoEditor.
                <14>     1/12/95        jpa             Don't use obsolete Toolbox names [1211211]
                <13>     1/10/95        CC              1153930: Added translation dialog;
                                                                        implemented new technique for dimming
                                                                        inactive items in Paste As dialog.
                <12>    11/17/94        CG              #1187128: Moved GetDestinationKindsList to
                                                                        TransUtl.cpp to be shared with Part Info
                                                                        and NoPart.
                <11>    10/31/94        CG              #1147225: Moved ContentValueTypes to
                                                                        InfoUtil.cpp.
                <10>    10/27/94        CG              #1147225: Moved AddTypesToMenu to
                                                                        InfoUtil.cpp.
                 <9>     10/7/94        CC              1160232 - Added calls to
                                                                        EndUsingLibraryResources().
                 <8>     10/4/94        CC              1190299 - Use utility fcns in InfoUtil.h
                                                                        and DlogUtil.h.
                 <7>     9/20/94        CC              1160232 - Added ShowLinkUpdateAlert()
                                                                        1153930 - Added TranslateValueTypes() and
                                                                        other mods to add translation choices to
                                                                        Paste As dialog.
                 <6>     8/10/94        CC              1179943 - LinkInfo dialog functions take
                                                                        ODBaseLink and ODBaseLinkSource parameters.
                 <5>      8/2/94        CC              #1178169 - ShowPasteAsDialog() takes
                                                                        ODFacet* instead of ODFrame*.
                 <4>     7/31/94        CC              ShowLinkDestinationInfo - fix to display
                                                                        dialog correctly.
                 <3>     7/27/94        CC              Allocate kindList (a SOM object) on the
                                                                        heap, not the stack.
                 <2>      7/5/94        CC              Added typecasts of pascal strings for PPCC
                                                                        compiler.
                 <1>     6/24/94        CC              first checked in
                 <0>     6/13/94        SV              SOMverted

                ------------------- CC ---- Converted to SOM

                <14>      6/1/94        CG              Changed to use new public binding object.
                <13>     5/11/94        T‚              #1162405 Removed explicit ASLM dependency
                                                                        for using resources (missed a couple)
                <12>     5/10/94        T‚              #1162405 Removed explicit ASLM dependency
                                                                        for using resources
                <11>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                <10>      4/6/94        CC              XMPTypeInfo: Changed field "creationDate"
                                                                        to "creationTime" and "modificationDate" to
                                                                        "changeTime". (1154722)
                 <9>      4/4/94        CC              Convert from using XMPTypeSet to
                                                                        XMPTypeList (1153046).
                 <8>      4/1/94        T‚              #1154941: Get OpenDoc source to actually
                                                                        compile for PowerPC
                 <7>     3/28/94        CG              1153547: Renamed XMPSessn.h to XMPSessM.h.
                 <6>     3/25/94        JA              Added missing #include (1147588)
                 <5>     3/25/94        eeh             bug #1153053: Changes for PPC nativity.
                 <4>     3/24/94        CC              ShowLinkSourceInfo() takes XMPUpdateID
                                                                        parameter; XMPPasteAsResult specifies
                                                                        original kind when selected kind is a
                                                                        translation (1153068).
                 <3>     3/17/94        CC              Added ShowLinkSourceInfo() and
                                                                        ShowLinkDestinationInfo(). (1151329)
                 <1>     3/14/94        CC              first checked in
                 <0>     3/14/94        CC              Create ShowPasteAsDialog (1144253).

        In Progress:

*/

#ifndef _PLFMDEF_
#include <plfmdef.h>
#endif

#ifndef _LINKDLGS_
#include <LinkDlgs.h>
#endif

#ifndef _INFOUTIL_
#include <InfoUtil.h>
#endif

#ifndef _DLOGUTIL_
#include <DlogUtil.h>
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _LINKDEFS_
#include <LinkDefs.h>
#endif

#ifndef _USERSRCM_
#include <UseRsrcM.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODBaseLink_xh
#include <LinkB.xh>
#endif

#ifndef SOM_ODBaseLinkSource_xh
#include <LinkSrcB.xh>
#endif

#ifndef SOM_ODBinding_xh
#include <ODBindng.xh>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef _BINDNGH_
#include <BindngH.h>
#endif

#ifndef _BNDNSUTL_
#include <BndNSUtl.h>
#endif

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_ODDispatcher_xh
#include <Disptch.xh>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _PASCLSTR_
#include <PasclStr.h>
#endif

#ifndef _ISOSTR_
#include <ISOStr.h>
#endif

#ifndef _DOCUTILS_
#include <DocUtils.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

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

#ifndef __RESOURCES__
#include <Resources.h>
#endif

#ifndef _TRANSUTL_
#include <TransUtl.h>
#endif

#ifndef __LISTS__
#include <Lists.h>
#endif

#ifndef __PALETTES__
#include <Palettes.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _EDITRSET_
#include <EditrSet.h>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef SOM_ODStorageSystem_xh
#include "ODStor.xh"
#endif

#pragma segment Info

#if ODDebug
#define ODDebugLinkDlgs 0
#else
#define ODDebugLinkDlgs 0
#endif

//==============================================================================
// Global variables
//==============================================================================

// Use a global to pass the current session to the LinkFilterProc
// Globals are per process, and there is only one session per process.
static ODSession* gSession = kODNULL;

static ODName* gKindName = kODNULL;
static ODName* gEditorName = kODNULL;
static ODName* gEmbeddedEditorName = kODNULL;

//==============================================================================
// Constants
//==============================================================================

const Boolean kNoDefaultButton = kODFalse;

const short kButtonFrameInset = -4;

const short kControlInactive = 255;
const short kControlActive = 0;

// Constants used in list manager calls
const Boolean kDoDraw = true;
const Boolean kNoGrow = false;
const Boolean kIncludeScrollBar = true;

// Key codes
// <eeh> moved to DlogUtil.h
// const char kEnterKey         = 0x03;
// const char kReturnKey                = 0x0D;
// const char kEscapeKey                = 0x1B;
// const char kUpArrowKey               = 0x1E;
// const char kDownArrowKey     = 0x1F;

//==============================================================================
// Scalar types
//==============================================================================
typedef char ODHandleState;

//==============================================================================
// Function declarations
//==============================================================================
ODStatic void DisposeGlobalODNameAndZeroPtr(ODName** name);

ODStatic void SetDateTimeStrings(
                                        ODULong         dateTime,
                                        Str255          dateString,
                                        Str255          timeString);

ODStatic void InitItemActive(
                                        DialogPtr       dlg,
                                        short           itemNumber,
                                        UserItemUPP     itemUPP,
                                        ODBoolean       itemActive);

ODStatic pascal void DrawDeactivePICTItem(DialogPtr theDialog, short theItem);

ODStatic ODBoolean DraftIsReadOnly(Environment* ev, ODStorageUnit* su);

ODStatic void SetControlActive(
                                        DialogPtr       dlg,
                                        short           itemNumber,
                                        ODBoolean       itemActive);

ODStatic ODType BestContentKind(ODStorageUnit* contentSU);

ODStatic pascal void DrawKindName(DialogPtr dialog, SInt16 item);
ODStatic pascal void DrawEditorName(DialogPtr dialog, SInt16 item);

ODStatic void ResetEditorPopup(ODTypeList* kindList,
                                                                ODUShort kindIndex,
                                                                ODTypeList* translateToList,
                                                                ODUShort translateToIndex,
                                                                EditorSet* editorList,
                                                                MenuHandle editorMenu,
                                                                ControlHandle popupCtlHndl,
                                                                ODSession* session);

static pascal Boolean
PasteAsFilterProc(DialogPtr dialog, EventRecord *event, short *itemHit);

ODStatic void SetPopupControlValue(ControlHandle popupControlHandle, short itemValue);

//------------------------------------------------------------------------------
// DisposeGlobalODNameAndZeroPtr
//------------------------------------------------------------------------------
ODStatic void DisposeGlobalODNameAndZeroPtr(ODName** name)
{
        if (*name)
        {
                // simple way to force deletion of old name value without knowing
                // that DisposeIText is the right way to destroy an ODName:
                TempODName tempName = *name;
                *name = kODNULL;
        }
}

//------------------------------------------------------------------------------
// InitLinkDialogs
//------------------------------------------------------------------------------
void InitLinkDialogs(ODSession* session)
{
        gSession = session;
}

//------------------------------------------------------------------------------
// DrawEditorName
//------------------------------------------------------------------------------

ODStatic pascal void DrawEditorName(DialogPtr dialog, SInt16 item)
{
        short                   itemType;
        Handle                  itemHandle;
        Rect                    itemRect;

        GetDialogItem(dialog, item, &itemType, &itemHandle, &itemRect);

        if ( gEditorName )
                DrawITextInDlogBox(gEditorName, &itemRect, dialog, kODTrue);
}

//------------------------------------------------------------------------------
// DrawEmbeddedEditorName
//------------------------------------------------------------------------------

ODStatic pascal void DrawEmbeddedEditorName(DialogPtr dialog, SInt16 item)
{
        short                   itemType;
        Handle                  itemHandle;
        Rect                    itemRect;

        GetDialogItem(dialog, item, &itemType, &itemHandle, &itemRect);

        if ( gEmbeddedEditorName )
                DrawITextInDlogBox(gEmbeddedEditorName, &itemRect, dialog, kODTrue);
}

//------------------------------------------------------------------------------
// GetSelectedType
//------------------------------------------------------------------------------
// $$$$$ Should be replaced by GetThisKindFromList in InfoUtil.h

ODStatic ODType GetSelectedType(ODTypeList* typeList, ODSShort typeIndex)
{
        Environment*            ev = somGetGlobalEnvironment();
        ODType                          type = kODNULL;
        ODTypeListIterator*     iter = typeList->CreateTypeListIterator(ev);

        for ( type = iter->First(ev);
                  (--typeIndex > 0) && iter->IsNotComplete(ev);
                  type = iter->Next(ev) )
        {
                ODDisposePtr(type);
                type = kODNULL;
        }

        delete iter;

        return type;
}

//------------------------------------------------------------------------------
// GetSelectedEditor
//------------------------------------------------------------------------------

ODStatic ODEditor GetSelectedEditor(EditorSet* editorList, ODSShort editorIndex)
{
        ODEditor                        editor;
        EditorSetIterator*      iter = editorList->CreateIterator();

        for ( editor = iter->First();
                  (--editorIndex > 0) && iter->IsNotComplete();
                  editor = iter->Next() )
        {
        }

        delete iter;

        if ( editor != kODNULL )
                editor = ODISOStrFromCStr((char *) editor);

        return editor;
}

//------------------------------------------------------------------------------
// EnableKindPopupItems
//------------------------------------------------------------------------------

ODStatic void EnableKindPopupItems(
                                        ODBoolean                       forMerge,
                                        ODTypeList*                     typeList,
                                        ODTypeList*                     translateList,
                                        ODSShort                        translationIndex,
                                        MenuHandle                      kindMenu,
                                        ODEditor                        editor,
                                        ODNameSpaceManager*     nsm)
{
        Environment* ev = somGetGlobalEnvironment();

        ODUShort item = EnableTypesInMenu(typeList, kindMenu, forMerge ? editor : kODNULL, nsm);

        // If a translation choice is present in the popup, enable it if appropriate;
        if ( translationIndex != 0 )
        {
                TempODType translationKind = GetSelectedType(translateList, translationIndex);

                if ( !forMerge || EditorSupportsKind(nsm, editor, translationKind) )
                        EnableItem(kindMenu, ++item);
                else
                        DisableItem(kindMenu, ++item);
        }

        // Enable the "Translate toÉ" item in the kind popup if translation is possible
        if ( translateList->Count(ev) > 0 )
                EnableItem(kindMenu, ++item);
        else
                DisableItem(kindMenu, ++item);
}

//------------------------------------------------------------------------------
// EnablePasteAsOkButton
//------------------------------------------------------------------------------

ODStatic void EnablePasteAsOkButton(DialogPtr dlog, MenuHandle kindMenu)
{
        Environment*    ev = somGetGlobalEnvironment();
        short                   itemType;
        Handle                  itemHandle;
        Rect                    itemRect;
        ODSShort                kindItemSelected;
        ODBoolean               shouldBeEnabled;

        GetDialogItem(dlog, kPasteAsKindPopup, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        kindItemSelected = (ODSShort) GetControlValue((ControlHandle)itemHandle);

        // Only the first 31 items of a menu may be disabled
        if ( kindItemSelected > 31 )
                shouldBeEnabled = kODTrue;
        else
                // BitTst numbers the bits hi to low
                shouldBeEnabled = BitTst(&(**kindMenu).enableFlags, 31-kindItemSelected);

        EnableOkButton(dlog, shouldBeEnabled);
}

//------------------------------------------------------------------------------
// BestMergeKindSetting
//------------------------------------------------------------------------------

ODStatic ODSShort BestMergeKindSetting(
                                        ODTypeList*                     kinds,
                                        ODType                          bestKind,
                                        ODEditor                        editor,
                                        ODNameSpaceManager*     nsm)
{
        if ( bestKind && EditorSupportsKind(nsm, editor, bestKind) )
                return IndexOfKindInList(kinds, bestKind);

        Environment*            ev = somGetGlobalEnvironment();
        ODSShort                        kindItem = 0;           // Default in case none are supported

        ODTypeListIterator*     iter = kinds->CreateTypeListIterator(ev);

        ODType                          type;
        ODSShort                        i;
        for ( type = iter->First(ev), i = 1;
                  iter->IsNotComplete(ev);
                  type = iter->Next(ev), ++i )
        {
                ODBoolean supports = EditorSupportsKind(nsm, editor, type);
                ODDisposePtr(type);
                if ( supports )
                {
                        kindItem = i;
                        break;
                }
        }

        delete iter;

        return kindItem;
}

//------------------------------------------------------------------------------
// InitialViewSelection
//------------------------------------------------------------------------------
ODStatic void InitialViewSelection(DialogPtr dlg, ODTypeToken viewType, ODSession* session)
{
        short           itemType;
        Handle          itemHandle;
        Rect            itemRect;
        ODSShort        viewItem;

        Environment* ev = somGetGlobalEnvironment();

        if (viewType == session->Tokenize(ev, kODViewAsFrame))
                viewItem = kODPasteAsFrameMenuItem;
        else if (viewType == session->Tokenize(ev, kODViewAsLargeIcon))
                viewItem = kODPasteAsLargeIconMenuItem;
        else if (viewType == session->Tokenize(ev, kODViewAsSmallIcon))
                viewItem = kODPasteAsSmallIconMenuItem;
        else if (viewType == session->Tokenize(ev, kODViewAsThumbnail))
                viewItem = kODPasteAsThumbnailMenuItem;
        else
                viewItem = kODPasteAsFrameMenuItem;

        GetDialogItem(dlg, kPasteAsEmbedPopup, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, viewItem);
}

//------------------------------------------------------------------------------
// GetViewSelected
//------------------------------------------------------------------------------
ODStatic ODTypeToken GetViewSelected(DialogPtr dlg, ODSession* session)
{
        short           itemType;
        Handle          itemHandle;
        Rect            itemRect;

        Environment* ev = somGetGlobalEnvironment();

        GetDialogItem(dlg, kPasteAsEmbedPopup, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        ODSShort viewItem = (ODSShort) GetControlValue((ControlHandle)itemHandle);
        ODType viewAs;
        switch ( viewItem )
        {
        case kODPasteAsLargeIconMenuItem:
                        viewAs = kODViewAsLargeIcon;
                        break;
        case kODPasteAsSmallIconMenuItem:
                        viewAs = kODViewAsSmallIcon;
                        break;
        case kODPasteAsThumbnailMenuItem:
                        viewAs = kODViewAsThumbnail;
                        break;
        default:
                        viewAs = kODViewAsFrame;
                        break;
        }

        return session->Tokenize(ev, viewAs);
}

//------------------------------------------------------------------------------
// HaveColorQuickdraw
//------------------------------------------------------------------------------
ODStatic ODBoolean HaveColorQuickdraw()
{
        long response;

        Gestalt(gestaltQuickdrawFeatures, &response);

        /* check if Color QuickDraw is available */
        return BitTst(&response, 31-gestaltHasColor);
}

//------------------------------------------------------------------------------
// EnableDialogItem
//------------------------------------------------------------------------------
ODStatic void EnableDialogItem(DialogPtr dlg, short itemNumber, ODBoolean enable)
{
        short           itemType;
        Handle          itemHandle;
        Rect            itemRect;

        GetDialogItem(dlg, itemNumber, &itemType, &itemHandle, &itemRect);
        if ( enable )
                itemType &= ~itemDisable;
        else
                itemType |= itemDisable;
        SetDialogItem(dlg, itemNumber, itemType, itemHandle, &itemRect);
}

//------------------------------------------------------------------------------
// InvalDialogItem
//------------------------------------------------------------------------------
ODStatic void InvalDialogItem(DialogPtr dlg, short itemNumber)
{
        short           itemType;
        Handle          itemHandle;
        Rect            itemRect;
        WindowPtr       savePort;

        GetPort(&savePort);
        SetPort(dlg);
        GetDialogItem(dlg, itemNumber, &itemType, &itemHandle, &itemRect);
        InvalRect(&itemRect);
        SetPort(savePort);
}

//------------------------------------------------------------------------------
// SetPopupControlValue
//------------------------------------------------------------------------------
ODStatic void SetPopupControlValue(ControlHandle popupControlHandle, short itemValue)
{
        // The resource file must be in the chain in order for the popup to redraw.
        CUsingLibraryResources r;
        SetControlValue(popupControlHandle, itemValue);
}

//------------------------------------------------------------------------------
// SetupControlItem
//------------------------------------------------------------------------------
ODStatic void SetupControlItem(
                                        DialogPtr       dlg,
                                        short           itemNumber,
                                        short           itemValue,
                                        ODBoolean       itemActive)
{
        short           itemType;
        Handle          itemHandle;
        Rect            itemRect;

        GetDialogItem(dlg, itemNumber, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, itemActive? itemValue : 0);
        HiliteControl((ControlHandle)itemHandle, itemActive ? kControlActive : kControlInactive);
}

//------------------------------------------------------------------------------
// InactivateEmbedAsPopup
//------------------------------------------------------------------------------

ODStatic void InactivateEmbedAsPopup(DialogPtr dlg)
// Set clipping to just the display area of the popup so the radio button
// label isn't dimmed.
{
        short           aItem;
        Handle          aHandle;
        Handle          embedPopupCntlHandle;
        Rect            aRect;
        RgnHandle       embedTextRgn = ODNewRgn();
        RgnHandle       embedPopupRgn = ODNewRgn();
        RgnHandle       clipRgn = ODNewRgn();

        GetDialogItem(dlg, kPasteAsEmbedRadioBtn, &aItem, &aHandle, &aRect);
        RectRgn(embedTextRgn,&aRect);

        GetDialogItem(dlg, kPasteAsEmbedPopup, &aItem, &embedPopupCntlHandle, &aRect);
        RectRgn(embedPopupRgn,&aRect);

        GetClip(clipRgn);
        DiffRgn(embedPopupRgn, embedTextRgn, embedPopupRgn);
        SetClip(embedPopupRgn);
        HiliteControl((ControlHandle)embedPopupCntlHandle, kControlInactive);
        SetClip(clipRgn);

        ODDisposeHandle((Handle)clipRgn);
        ODDisposeHandle((Handle)embedPopupRgn);
        ODDisposeHandle((Handle)embedTextRgn);
}

//------------------------------------------------------------------------------
// ClipOutRect
//------------------------------------------------------------------------------

ODStatic RgnHandle ClipOutRect(Rect* r)
{
        RgnHandle       saveRgn = ODNewRgn();
        RgnHandle       rectRgn = ODNewRgn();
        RgnHandle       clipRgn = ODNewRgn();

        GetClip(saveRgn);

        RectRgn(rectRgn, r);

        GetClip(clipRgn);
        DiffRgn(clipRgn, rectRgn, clipRgn);
        SetClip(clipRgn);

        ODDisposeHandle((Handle)rectRgn);
        ODDisposeHandle((Handle)clipRgn);

        return saveRgn;
}

//------------------------------------------------------------------------------
// SetAutomaticUpdateControls
//------------------------------------------------------------------------------
ODStatic void SetAutomaticUpdateControls(DialogPtr dlg, ODBoolean autoUpdateSetting)
{
        short   itemType;
        Handle  itemHandle;
        Rect    itemRect;

        GetDialogItem(dlg, kPasteAsAutomaticRadioBtn, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, autoUpdateSetting);
        GetDialogItem(dlg, kPasteAsManualRadioBtn, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, !autoUpdateSetting);
}

//------------------------------------------------------------------------------
// SetMergeEmbedControls
//------------------------------------------------------------------------------
ODStatic void SetMergeEmbedControls(DialogPtr dlg, ODBoolean mergeSetting)
{
        short   itemType;
        Handle  itemHandle;
        Rect    itemRect;

        CUsingLibraryResources r;       // Necessary to change embed popup

        GetDialogItem(dlg, kPasteAsMergeRadioBtn, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, mergeSetting);

        GetDialogItem(dlg, kPasteAsEmbedRadioBtn, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, !mergeSetting);

        if ( mergeSetting )
        {
                InactivateEmbedAsPopup(dlg);
        }
        else
        {
                GetDialogItem(dlg, kPasteAsEmbedPopup, &itemType, &itemHandle, &itemRect);
                ASSERT_CONTROL_ITEM(itemType);
                HiliteControl((ControlHandle)itemHandle, kControlActive);
        }
}

//------------------------------------------------------------------------------
// GetIndexedElement
//------------------------------------------------------------------------------

ODStatic ODULong GetIndexedElement(OrderedCollection* translateFromList, ODULong translationIndex)
{
        OrderedCollectionIterator* oci = translateFromList->CreateIterator();

        ODULong elem = 0;

        for (   elem = (ODULong) oci->First();
                        oci->IsNotComplete();
                        elem = (ODULong) oci->Next() )
        {
                if ( --translationIndex == 0 )
                        break;
        }

        ODDeleteObject(oci);

        return elem;
}

//------------------------------------------------------------------------------
// GetOriginalDraft
//------------------------------------------------------------------------------
// Returns 0 if the original draft is unknown.  This is the case when content
// was placed in the draft without cloning.  This code is dependent on properties
// created by the argument storage unit's container suite.


static ODULong GetOriginalDraft(Environment* ev, ODDraft* draft)
{
        TempODStorageUnit       draftProperties = draft->AcquireDraftProperties(ev);
        return ODGetULongProp(ev, draftProperties, kODPropOriginalDraft, kODULong);
}

//------------------------------------------------------------------------------
// SUClonedFromDraft
//------------------------------------------------------------------------------
// Returns kODTrue is the argument storage unit is the clone of a storage unit
// from the argument draft.  This code is dependent on properties created by
// the argument storage unit's container suite.

static ODBoolean SUClonedFromDraft(Environment* ev,
                        ODStorageUnit* su,
                        ODDraft* draft)
{
        return ((ODDraft*)GetOriginalDraft(ev, su->GetDraft(ev)) == draft &&
                        su->Exists(ev, kODPropOriginalID, kODULong, 0));
}

//------------------------------------------------------------------------------
// DeactivateModalDialog
//------------------------------------------------------------------------------

ODStatic void DeactivateModalDialog(ModalFilterUPP filterProc)
{
        DialogPtr dialog = (DialogPtr) FrontWindow();

        if ( dialog )
        {
                EventRecord event;

                event.what = activateEvt;
                event.message = (long) dialog;
                event.modifiers = 0;

                CallModalFilterProc(filterProc, dialog, &event, 0);
        }
}

//------------------------------------------------------------------------------
// PasteAsFilterProc
//------------------------------------------------------------------------------

static pascal Boolean
PasteAsFilterProc(DialogPtr dialog, EventRecord *event, short *itemHit)
{
        // If the event is a deactivate event, the ok button must be deactivated
        // before the surrounding outline is drawn so it is drawn dim.
        ODBoolean rslt = ODButtonKeyFilterProc(dialog,event,itemHit);

        if ( event->what == activateEvt )
        {
                ODOutlineDefaultButtonDrawProc(dialog, kPasteAsDefaultButtonItem);
        }

        return rslt;
}

//------------------------------------------------------------------------------
// ShowPasteAsDialog
//------------------------------------------------------------------------------

ODBoolean ShowPasteAsDialog(
                                ODBoolean                               canPasteLink,
                                ODPasteAsMergeSetting   mergeSetting,
                                ODBoolean                               isMove,
                                ODStorageUnit*                  contentSU,
                                ODFacet*                                facet,
                                ODTypeToken                             viewType,
                                ODPasteAsResult*                result)

{
        Environment* ev = somGetGlobalEnvironment();

        WindowPtr       savePort;                                                       ODVolatile(savePort);
        GetPort(&savePort);

        short           itemHit = kPasteAsCancelBtn;
        short           itemType;
        DialogPtr       dlg = kODNULL;                                          ODVolatile(dlg);
        Handle          itemHandle;
        Rect            itemRect;
        Rect            kindPopupRect;
        Rect            editorPopupRect;
        short           kindItemSelected;
        short           kindItemOthers;
        short           kindItemCurrent;
        ODBoolean       kindPopupHasNoTranslationKind = kODTrue;
        ODUShort        translationIndex = 0;

        ODSLong         refSaved;
        OSErr           resErr;

        ODBoolean       userChangedKind = kODFalse;

        ControlHandle kindPopupControlHandle;
        ControlHandle editorPopupControlHandle;

        ODEditor mergeEditorID = kODNULL;                               ODVolatile(mergeEditorID);
        ODEditor embedEditorID = kODNULL;                               ODVolatile(embedEditorID);
        ODEditor preferredEditorID = kODNULL;                   ODVolatile(preferredEditorID);

        EditorSet* editorList = kODNULL;                                ODVolatile(editorList);

        ODSShort bestKindIndex = 1;
        ODSShort bestMergeKind = 1;

        ODType bestKind = kODNULL;                                              ODVolatile(bestKind);
        ODTypeList*     kindList = kODNULL;                                     ODVolatile(kindList);
        ODTypeList*     translateToList = kODNULL;                      ODVolatile(translateToList);
        OrderedCollection* translateFromList = kODNULL; ODVolatile(translateFromList);

        MenuHandle kindMenu = kODNULL;                                  ODVolatile(kindMenu);
        MenuHandle editorMenu = kODNULL;                                ODVolatile(editorMenu);

        UserItemUPP DrawBoxItemUPP = kODNULL;                   ODVolatile(DrawBoxItemUPP);
        UserItemUPP DrawDeactiveItemUPP = kODNULL;              ODVolatile(DrawDeactiveItemUPP);
        UserItemUPP     DrawEditorNameUPP = kODNULL;            ODVolatile(DrawEditorNameUPP);
        UserItemUPP     DrawEmbeddedEditorNameUPP = kODNULL;ODVolatile(DrawEmbeddedEditorNameUPP);
        UserItemUPP     DrawKindNameUPP = kODNULL;                      ODVolatile(DrawKindNameUPP);

        ModalFilterUPP modalFilter = kODNULL;                   ODVolatile(modalFilter);

        if ( canPasteLink )
                canPasteLink = contentSU->Exists(ev, kODPropLinkSpec, (ODValueType) kODLinkSpec, 0);

        result->pasteLinkSetting = (!isMove) && canPasteLink;
        result->autoUpdateSetting = kODTrue;
        result->selectedKind = (ODType) kODNULL;
        result->translateKind = (ODType) kODNULL;
        result->editor = kODNoEditor;

        ODSession* session;
        {
                TempODPart part = facet->GetFrame(ev)->AcquirePart(ev);
                session = part->GetStorageUnit(ev)->GetSession(ev);
        }

        TRY
                ODBinding* binding = session->GetBinding(ev);
                ODNameSpaceManager* nsm = session->GetNameSpaceManager(ev);

                session->GetWindowState(ev)->DeactivateFrontWindows(ev);

                { TempODPart part = facet->GetFrame(ev)->AcquirePart(ev);
                  mergeEditorID = GetCurrentEditorForPart(part);
                }

                kindList = session->GetStorageSystem(ev)->CreateTypeList(ev, (ODTypeList*) kODNULL);
                ContentValueTypes(contentSU, kindList);

                // Now get all possible translations from the available kinds
                translateToList = session->GetStorageSystem(ev)->CreateTypeList(ev, (ODTypeList*) kODNULL);
                translateFromList = new OrderedCollection;
                TranslateValueTypes(kindList, translateToList, translateFromList, session);

                refSaved = BeginUsingLibraryResources();
                kindMenu = GetMenu(kPasteAsKindPopupMenu);
                resErr = ResError();
                EndUsingLibraryResources(refSaved);
                THROW_IF_NULL(kindMenu, resErr ? resErr : resNotFound);

                AddTypesToMenu(kindList, kindMenu, session);
                kindItemOthers = kindList->Count(ev) + 1;

                ODBoolean canMerge = (mergeSetting != kODPasteAsEmbedOnly);
                ODBoolean canEmbed = (mergeSetting != kODPasteAsMergeOnly);

                result->mergeSetting = (mergeSetting == kODPasteAsMerge) || (mergeSetting == kODPasteAsMergeOnly);

                {
                        CUsingLibraryResources r;
                        dlg = ODGetNewDialog(ev, kPasteAsDlgID, session, kNoDefaultButton);
                        THROW_IF_NULL(dlg);
                        SetPort(dlg);
                        SetDialogTextStyle(dlg, kPasteAsDlgID, smCurrentScript);
                }

                /* Horizontal line needs to be drawn */
                GetDialogItem(dlg, kPasteAsHorizSep, &itemHit, &itemHandle, &itemRect);
                DrawBoxItemUPP = NewUserItemProc(DrawGrayBoxItem);
                SetDialogItem(dlg, kPasteAsHorizSep, itemHit, (Handle)DrawBoxItemUPP, &itemRect);

                // Set the draw routine for the default button outline item
                GetDialogItem(dlg, kPasteAsDefaultButtonItem, &itemHit, &itemHandle, &itemRect);
                SetDialogItem(dlg, kPasteAsDefaultButtonItem, itemHit, (Handle)GetODOutlineDefaultButtonDrawProc(), &itemRect);

                // Create one UPP for all user items used to draw deactive PICTs
                DrawDeactiveItemUPP = NewUserItemProc(DrawDeactivePICTItem);

                // Initialize state of "Paste with Link" checkbox and label
                SetupControlItem(dlg, kPasteAsLinkCheckbox, result->pasteLinkSetting, canPasteLink);

                // Initialize state of "Get Updates" radio buttons and labels
                if ( result->pasteLinkSetting)
                        HideDialogItem(dlg, kPasteAsUpdateDisabledText);
                else
                        HideDialogItem(dlg, kPasteAsUpdateText);

                SetupControlItem(dlg, kPasteAsAutomaticRadioBtn, result->autoUpdateSetting, canPasteLink);
                SetupControlItem(dlg, kPasteAsManualRadioBtn, !result->autoUpdateSetting, canPasteLink);

                // set up merge picture, radio button, and label
                InitItemActive(dlg, kPasteAsMergePict, DrawDeactiveItemUPP, canMerge);
                SetupControlItem(dlg, kPasteAsMergeRadioBtn, result->mergeSetting, canMerge);

                // set up embed picture, radio button, and label
                InitItemActive(dlg, kPasteAsEmbedPict, DrawDeactiveItemUPP, canEmbed);
                SetupControlItem(dlg, kPasteAsEmbedRadioBtn, !result->mergeSetting, canEmbed);

                // set up initial "Paste as:" popup selection
                InitialViewSelection(dlg, viewType, session);

                // inactivate the "Embed as:" popup if merge is initially selected
                if ( result->mergeSetting )
                {
                        GetDialogItem(dlg, kPasteAsEmbedPopup, &itemType, &itemHandle, &itemRect);
                        ASSERT_CONTROL_ITEM(itemType);
                        HiliteControl((ControlHandle)itemHandle, kControlInactive);
                }

                // Determine the indices of the best kind and the best kind that can be merged
                bestKind = BestContentKind(contentSU);
                if ( bestKind )
                        bestKindIndex = IndexOfKindInList(kindList, bestKind);
                if ( bestKindIndex == 0 )       // In case preferred kind property is bogus
                        bestKindIndex = 1;

                bestMergeKind = BestMergeKindSetting(kindList, bestKind, mergeEditorID, nsm);

                // Suppress the preferred editor if it doesn't support the preferred kind
                preferredEditorID = ODGetISOStrProp(ev, contentSU, kODPropPreferredEditor, kODEditor, kODNULL, kODNULL);
                if ( preferredEditorID && bestKind && !EditorSupportsKind(nsm, preferredEditorID, bestKind) )
                {
                        ODDisposePtr((ODPtr) preferredEditorID);
                        preferredEditorID = kODNULL;
                }

#if ODDebugLinkDlgs
                somPrintf("ShowPasteAsDialog: mergeSetting = %u, best merge kind = %u\n", mergeSetting, (bestMergeKind != 0));
#endif

                // Set up the Kind popup items
                EnableKindPopupItems(result->mergeSetting, kindList, translateToList, 0, kindMenu, mergeEditorID, nsm);
                GetDialogItem(dlg, kPasteAsKindPopup, &itemType, (Handle*) &kindPopupControlHandle, &kindPopupRect);
                ASSERT_CONTROL_ITEM(itemType);
                SetControlValue(kindPopupControlHandle, result->mergeSetting ? bestMergeKind : bestKindIndex);

                // Determine if the root storage unit is an existing part from the same draft
                // as the pasting part

                ODDraft* partDraft;
                // Get the draft from the part, not the frame, because the frame's storage
                // unit is null if non-persistent.
                {
                        TempODPart part = facet->GetFrame(ev)->AcquirePart(ev);
                        partDraft = part->GetStorageUnit(ev)->GetDraft(ev);
                }
                ODBoolean contentIsPart = SUClonedFromDraft(ev, contentSU, partDraft);

                // Set text of static "Kind:" label
                DisposeGlobalODNameAndZeroPtr(&gKindName);
                if ( bestKind && GetUserKindFromKind(nsm, bestKind, &gKindName) )
                {
                        GetDialogItem(dlg, kPasteAsKindText, &itemType, &itemHandle, &itemRect);
                        DrawKindNameUPP = NewUserItemProc(DrawKindName);
                        SetDialogItem(dlg, kPasteAsKindText, userItem+itemDisable, (Handle) DrawKindNameUPP, &itemRect);
                }

                // inactivate the "Kind:" popup if embedding is initially selected, the operation is
                // a move, and a cloned part is in the storage unit (not intrinsic content).
                if ( (!result->mergeSetting) && isMove && contentIsPart )
                {
                        HideDialogItem(dlg, kPasteAsKindPopup);
                }
                else
                {
                        HideDialogItem(dlg, kPasteAsKindLabel);
                        HideDialogItem(dlg, kPasteAsKindText);
                }

                // Setup the static text for the merge editor
                DisposeGlobalODNameAndZeroPtr(&gEditorName);
                if ( mergeEditorID && GetUserEditorFromEditor(nsm, mergeEditorID, &gEditorName) )
                {
                        GetDialogItem(dlg, kPasteAsEditorText, &itemType, &itemHandle, &itemRect);
                        DrawEditorNameUPP = NewUserItemProc(DrawEditorName);
                        SetDialogItem(dlg, kPasteAsEditorText, userItem+itemDisable, (Handle) DrawEditorNameUPP, &itemRect);
                }

                // Setup the static text for the embedding editor
                // Since the contentSU will not be cloned during a move in the same document, this is a guess as to the
                // editor actually bound to the moved frame.
                if ( preferredEditorID )
                        embedEditorID = ODISOStrFromCStr((char *) preferredEditorID);
                else
                        embedEditorID = binding->ChooseEditorForPart(ev, contentSU, bestKind);

                DisposeGlobalODNameAndZeroPtr(&gEmbeddedEditorName);
                if ( embedEditorID && GetUserEditorFromEditor(nsm, embedEditorID, &gEmbeddedEditorName) )
                {
                        GetDialogItem(dlg, kPasteAsEmbedEditorText, &itemType, &itemHandle, &itemRect);
                        DrawEmbeddedEditorNameUPP = NewUserItemProc(DrawEmbeddedEditorName);
                        SetDialogItem(dlg, kPasteAsEmbedEditorText, userItem+itemDisable, (Handle) DrawEmbeddedEditorNameUPP, &itemRect);
                }

                // Setup the editor popup
                editorList = new EditorSet;
                editorList->InitEditorSet();

                refSaved = BeginUsingLibraryResources();
                editorMenu = GetMenu(kPasteAsEditorPopupMenu);
                resErr = ResError();
                EndUsingLibraryResources(refSaved);
                THROW_IF_NULL(editorMenu, resErr ? resErr : resNotFound);

                GetDialogItem(dlg, kPasteAsEditorPopup, &itemType, (Handle*) &editorPopupControlHandle, &editorPopupRect);
                SetupEditorMenu(bestKind, editorList, editorMenu, editorPopupControlHandle, session);

                if ( result->mergeSetting )
                {
                        HideDialogItem(dlg, kPasteAsEditorPopup);
                        HideDialogItem(dlg, kPasteAsEmbedEditorText);
                }
                else if ( isMove && contentIsPart )
                {
                        HideDialogItem(dlg, kPasteAsEditorPopup);
                        HideDialogItem(dlg, kPasteAsEditorText);
                }
                else
                {
                        HideDialogItem(dlg, kPasteAsEditorLabel);
                        HideDialogItem(dlg, kPasteAsEditorText);
                }

                modalFilter = NewModalFilterProc(PasteAsFilterProc);

                // Display the dialog
                ShowWindow(dlg);

                do {
                        // Remember the currently selected kind
                        kindItemCurrent = (ODSShort) GetControlValue(kindPopupControlHandle);

                        {       CUsingLibraryResources r;
                                ModalDialog(modalFilter, &itemHit);
                        }

                        switch (itemHit)
                        {
                        case kPasteAsLinkCheckbox:
                                        result->pasteLinkSetting = !result->pasteLinkSetting;
                                        GetDialogItem(dlg, itemHit, &itemType, &itemHandle, &itemRect);
                                        ASSERT_CONTROL_ITEM(itemType);
                                        SetControlValue((ControlHandle)itemHandle, result->pasteLinkSetting);

                                        if ( result->pasteLinkSetting )
                                        {
                                                GetDialogItem(dlg, kPasteAsUpdateDisabledText, &itemType, &itemHandle, &itemRect);
                                                itemRect.left += 16384;
                                                itemRect.right += 16384;
                                                SetDialogItem(dlg, kPasteAsUpdateDisabledText, itemType, itemHandle, &itemRect);
                                                ShowDialogItem(dlg, kPasteAsUpdateText);
                                        }
                                        else
                                        {
                                                GetDialogItem(dlg, kPasteAsUpdateText, &itemType, &itemHandle, &itemRect);
                                                itemRect.left += 16384;
                                                itemRect.right += 16384;
                                                SetDialogItem(dlg, kPasteAsUpdateText, itemType, itemHandle, &itemRect);
                                                ShowDialogItem(dlg, kPasteAsUpdateDisabledText);
                                        }

                                        SetupControlItem(dlg, kPasteAsAutomaticRadioBtn, result->autoUpdateSetting, result->pasteLinkSetting);
                                        SetupControlItem(dlg, kPasteAsManualRadioBtn, !result->autoUpdateSetting, result->pasteLinkSetting);
                                        break;

                        case kPasteAsAutomaticRadioBtn:
                                        result->autoUpdateSetting = kODTrue;
                                        SetAutomaticUpdateControls(dlg, result->autoUpdateSetting);
                                        break;

                        case kPasteAsManualRadioBtn:
                                        result->autoUpdateSetting = kODFalse;
                                        SetAutomaticUpdateControls(dlg, result->autoUpdateSetting);
                                        break;

                        case kPasteAsMergePict:
                        case kPasteAsMergeRadioBtn:
                                        if ( !result->mergeSetting )
                                        {
                                                result->mergeSetting = kODTrue;
                                                SetMergeEmbedControls(dlg, result->mergeSetting);

                                                TempODType selectedKind = kODNULL;
                                                if ( kindItemCurrent <= kindList->Count(ev) )
                                                        selectedKind = GetSelectedType(kindList, kindItemCurrent);
                                                else
                                                        selectedKind = GetSelectedType(translateToList, translationIndex);

                                                if ( !EditorSupportsKind(nsm, mergeEditorID, selectedKind) )
                                                {
                                                        SetPopupControlValue(kindPopupControlHandle, bestMergeKind);
                                                        userChangedKind = kODFalse;     // Allow embed to automatically select best kind
                                                }

                                                EnableKindPopupItems(result->mergeSetting, kindList, translateToList, translationIndex, kindMenu, mergeEditorID, nsm);

                                                if ( isMove && contentIsPart )
                                                {
                                                        HideDialogItem(dlg, kPasteAsKindLabel);
                                                        HideDialogItem(dlg, kPasteAsKindText);
                                                        ShowDialogItem(dlg, kPasteAsKindPopup);
                                                }
                                                else
                                                {
                                                        EnablePasteAsOkButton(dlg, kindMenu);
                                                }

                                                HideDialogItem(dlg, kPasteAsEditorPopup);
                                                HideDialogItem(dlg, kPasteAsEmbedEditorText);
                                                ShowDialogItem(dlg, kPasteAsEditorLabel);
                                                ShowDialogItem(dlg, kPasteAsEditorText);
                                        }
                                        break;

                        case kPasteAsEmbedPict:
                        case kPasteAsEmbedRadioBtn:
                                        if ( result->mergeSetting )
                                        {
                                                result->mergeSetting = kODFalse;
                                                SetMergeEmbedControls(dlg, result->mergeSetting);

                                                // If the user has not changed the kind setting, switch to the preferred kind
                                                kindItemSelected = (ODSShort) GetControlValue(kindPopupControlHandle);
                                                if ( !userChangedKind )
                                                {
                                                        // Make sure item is enabled before drawing it!
                                                        EnableItem(kindMenu, bestKindIndex);
                                                        SetPopupControlValue(kindPopupControlHandle, bestKindIndex);
                                                        kindItemSelected = bestKindIndex;
                                                }

                                                // When embedding is chosen, determine the preferred editor, then enable the kind menu
                                                ResetEditorPopup(kindList,
                                                                                kindItemSelected,
                                                                                translateToList,
                                                                                translationIndex,
                                                                                editorList,
                                                                                editorMenu,
                                                                                editorPopupControlHandle,
                                                                                session);

                                                // Select the preferred editor if specified
                                                ODSShort preferredEditorItem = IndexOfEditorInList(editorList, preferredEditorID);
                                                if ( preferredEditorItem != 0 )
                                                        SetPopupControlValue(editorPopupControlHandle, preferredEditorItem);

                                                if ( isMove && contentIsPart )
                                                {
                                                        HideDialogItem(dlg, kPasteAsEditorText);
                                                        ShowDialogItem(dlg, kPasteAsEmbedEditorText);
                                                }
                                                else
                                                {
                                                        HideDialogItem(dlg, kPasteAsEditorLabel);
                                                        HideDialogItem(dlg, kPasteAsEditorText);
                                                        ShowDialogItem(dlg, kPasteAsEditorPopup);
                                                }

                                                // Now enable the kind items
                                                if ( isMove && contentIsPart )
                                                {
                                                        HideDialogItem(dlg, kPasteAsKindPopup);
                                                        ShowDialogItem(dlg, kPasteAsKindLabel);
                                                        ShowDialogItem(dlg, kPasteAsKindText);
                                                }
                                                else
                                                {
                                                        ODSShort editorItem = (ODSShort) GetControlValue(editorPopupControlHandle);
                                                        ODEditor selectedEditor = GetSelectedEditor(editorList, editorItem);
                                                        EnableKindPopupItems(result->mergeSetting, kindList, translateToList, translationIndex, kindMenu, selectedEditor, nsm);
                                                        ODDisposePtr((ODPtr) selectedEditor);
                                                        EnablePasteAsOkButton(dlg, kindMenu);
                                                }
                                        }
                                        break;

                        case kPasteAsKindPopup:
                                        kindItemSelected = (ODSShort) GetControlValue(kindPopupControlHandle);
                                        if ( kindItemSelected == kindItemOthers )
                                        {
                                                ODUShort selectedIndex;
                                                ODSShort editorItem = (ODSShort) GetControlValue(editorPopupControlHandle);
                                                ODEditor initialEditorID;
                                                if ( result->mergeSetting )
                                                        initialEditorID = ODISOStrFromCStr((char *) mergeEditorID);
                                                else
                                                        initialEditorID = GetSelectedEditor(editorList, editorItem);
                                                ODEditor newEditorID = kODNULL;

                                                // Return the kind popup to its current value
                                                SetPopupControlValue(kindPopupControlHandle, kindItemCurrent);
                                                DeactivateModalDialog(modalFilter);

                                                if ( session->GetTranslation(ev)->ShowTranslateDialog(
                                                                                                                                ev,
                                                                                                                                translateToList,
                                                                                                                                translationIndex,
                                                                                                                                &selectedIndex,
                                                                                                                                initialEditorID,
                                                                                                                                !result->mergeSetting,
                                                                                                                                &newEditorID) )
                                                {
                                                        // Add translationKind to the list just above "Translate toÉ",
                                                        //  and make it the current selection.
                                                        translationIndex = selectedIndex;

                                                        { TempODType translationKind = GetSelectedType(translateToList, translationIndex);
                                                          AddTranslationKindToMenu(translationKind, kindMenu, kindItemOthers-1,
                                                                                                                kindPopupHasNoTranslationKind, session);
                                                        }

                                                        if ( kindPopupHasNoTranslationKind )
                                                        {
                                                                kindPopupHasNoTranslationKind = kODFalse;
                                                                kindItemOthers += 1;
                                                        }

                                                        SetPort(dlg);
                                                        SetControlValue(kindPopupControlHandle, kindItemOthers-1);
                                                        InvalRect(&kindPopupRect);      // Redraw the popup

                                                        if ( !result->mergeSetting )
                                                        {
                                                                ResetEditorPopup(kindList,
                                                                                                kindItemSelected,
                                                                                                translateToList,
                                                                                                translationIndex,
                                                                                                editorList,
                                                                                                editorMenu,
                                                                                                editorPopupControlHandle,
                                                                                                session);
                                                                // Select editorID in the list
                                                                ODSShort editorItem = IndexOfEditorInList(editorList, newEditorID);
                                                                if ( editorItem != 0 )
                                                                        SetControlValue(editorPopupControlHandle, editorItem);
                                                                InvalRect(&editorPopupRect);    // Redraw the popup
                                                        }
                                                        ODDisposePtr((ODPtr) initialEditorID);
                                                        ODDisposePtr((ODPtr) newEditorID);

                                                        userChangedKind = kODTrue;
                                                }
                                        }
                                        else if ( kindItemSelected != kindItemCurrent )
                                        {
                                                userChangedKind = kODTrue;
                                                if ( !result->mergeSetting )
                                                {
                                                        ResetEditorPopup(kindList,
                                                                                        kindItemSelected,
                                                                                        translateToList,
                                                                                        translationIndex,
                                                                                        editorList,
                                                                                        editorMenu,
                                                                                        editorPopupControlHandle,
                                                                                        session);
                                                        SetPort(dlg);
                                                        InvalRect(&editorPopupRect);    // Redraw the popup
                                                }
                                        }
                                        EnablePasteAsOkButton(dlg, kindMenu);
                                        break;
                        default:
                                        break;
                        }
                } while ((itemHit != kPasteAsOKBtn) && (itemHit != kPasteAsCancelBtn));

                if ( itemHit == kPasteAsOKBtn )
                {
                        // update from "View" popup
                        if ( !result->mergeSetting )
                                result->selectedView = GetViewSelected(dlg, session);

                        // update from "Kind" popup
                        if ( (!result->mergeSetting) && isMove && contentIsPart )
                        {
                                // Selected kind is the best kind
                                result->selectedKind = GetSelectedType(kindList, bestKindIndex);
                        }
                        else
                        {
                                ODSShort kindItem = (ODSShort) GetControlValue(kindPopupControlHandle);
                                ODSShort kindCount = (ODSShort) kindList->Count(ev);
                                if (kindItem <= kindCount)
                                {
                                        result->selectedKind = GetSelectedType(kindList, kindItem);
                                }
                                else
                                {
                                        result->selectedKind = GetSelectedType(translateToList, translationIndex);
                                        result->translateKind = GetSelectedType(kindList, GetIndexedElement(translateFromList, translationIndex));
                                }
                        }

                        // update from "Editor" popup
                        if ( (!result->mergeSetting) && (!isMove || !contentIsPart) )
                        {
                                ODSShort editorItem = (ODSShort) GetControlValue(editorPopupControlHandle);
                                result->editor = GetSelectedEditor(editorList, editorItem);
                        }
                }

        CATCH_ALL
                ODDisposeRoutineDescriptor(DrawBoxItemUPP);
                ODDisposeRoutineDescriptor(DrawDeactiveItemUPP);
                ODDisposeRoutineDescriptor(DrawKindNameUPP);
                ODDisposeRoutineDescriptor(DrawEditorNameUPP);
                ODDisposeRoutineDescriptor(DrawEmbeddedEditorNameUPP);
                ODDisposeRoutineDescriptor(modalFilter);

                refSaved = BeginUsingLibraryResources();
                if ( kindMenu )
                {
                        DeleteMenu(kPasteAsKindPopupMenu);
                        ReleaseResource((Handle) kindMenu);
                }
                if ( editorMenu )
                {
                        DeleteMenu(kPasteAsEditorPopupMenu);
                        ReleaseResource((Handle) editorMenu);
                }
                ODDisposeDialog(dlg);
                EndUsingLibraryResources(refSaved);

                ODDisposePtr((ODPtr) mergeEditorID);
                ODDisposePtr((ODPtr) embedEditorID);
                ODDisposePtr((ODPtr) preferredEditorID);
                ODDisposePtr((ODPtr) bestKind);

                ODDeleteObject(kindList);
                ODDeleteObject(translateToList);
                ODDeleteObject(translateFromList);
                delete editorList; editorList = kODNULL;

                DisposeGlobalODNameAndZeroPtr(&gEditorName);
                DisposeGlobalODNameAndZeroPtr(&gEmbeddedEditorName);

                SetPort(savePort);
                RERAISE;
        ENDTRY;

        DisposeRoutineDescriptor(DrawBoxItemUPP);
        DisposeRoutineDescriptor(DrawDeactiveItemUPP);
        DisposeRoutineDescriptor(DrawKindNameUPP);
        DisposeRoutineDescriptor(DrawEditorNameUPP);
        DisposeRoutineDescriptor(DrawEmbeddedEditorNameUPP);
        DisposeRoutineDescriptor(modalFilter);

        refSaved = BeginUsingLibraryResources();
        if ( kindMenu )
        {
                DeleteMenu(kPasteAsKindPopupMenu);
                ReleaseResource((Handle) kindMenu);
        }
        if ( editorMenu )
        {
                DeleteMenu(kPasteAsEditorPopupMenu);
                ReleaseResource((Handle) editorMenu);
        }
        DisposeDialog(dlg);
        EndUsingLibraryResources(refSaved);

        ODDisposePtr((ODPtr) mergeEditorID);
        ODDisposePtr((ODPtr) embedEditorID);
        ODDisposePtr((ODPtr) preferredEditorID);
        ODDisposePtr((ODPtr) bestKind);

        ODDeleteObject(kindList);
        ODDeleteObject(translateToList);
        ODDeleteObject(translateFromList);
        delete editorList; editorList = kODNULL;

        DisposeGlobalODNameAndZeroPtr(&gEditorName);
        DisposeGlobalODNameAndZeroPtr(&gEmbeddedEditorName);

        session->GetWindowState(ev)->ActivateFrontWindows(ev);

        SetPort(savePort);

        return (itemHit == kPasteAsOKBtn);
}

//------------------------------------------------------------------------------
// ResetEditorPopup
//------------------------------------------------------------------------------

ODStatic void ResetEditorPopup(ODTypeList* kindList,
                                                                ODUShort kindIndex,
                                                                ODTypeList* translateToList,
                                                                ODUShort translateToIndex,
                                                                EditorSet* editorList,
                                                                MenuHandle editorMenu,
                                                                ControlHandle popupCtlHndl,
                                                                ODSession* session)
{
        Environment* ev = somGetGlobalEnvironment();
        TempODType kind = kODNULL;

        ODSShort kindCount = (ODSShort) kindList->Count(ev);
        if (kindIndex <= kindCount)
                kind = GetSelectedType(kindList, kindIndex);
        else
                kind = GetSelectedType(translateToList, translateToIndex);

        editorList->RemoveAllEditors();
        SetupEditorMenu(kind,
                                        editorList,
                                        editorMenu,
                                        popupCtlHndl,
                                        session);
}

//------------------------------------------------------------------------------
// PrepareToDrawGray
//------------------------------------------------------------------------------
//
// Sets the foreground color to the best color midway between the forground and
// background colors; usually gray.  If no intermediate gray is available (4 or
// fewer colors available), the pen pattern is set to draw gray.

ODStatic Boolean PrepareToDrawGray(DialogPtr theDialog, short theItem, RGBColor* fgSaveColor)
{
        Boolean result = false;
        const short kColorPort = 0xC000;

        Boolean isColorPort = ((((CGrafPtr)theDialog)->portVersion & kColorPort) == kColorPort);

        if ( isColorPort )
        {
                RGBColor        fgNewColor;
                RGBColor        bgColor;

                Rect            itemRect;
                Handle          itemHandle;
                short           itemKind;

                GetBackColor(&bgColor);
                GetForeColor(fgSaveColor);
                fgNewColor = *fgSaveColor;

                GetDialogItem(theDialog, kPasteAsOKBtn, &itemKind, &itemHandle, &itemRect);

                Rect globalRect = itemRect;
                LocalToGlobal((Point *)&(globalRect.top));
                LocalToGlobal((Point *)&(globalRect.bottom));
                GDHandle targetDevice = GetMaxDevice(&globalRect);

                result = GetGray(targetDevice, &bgColor, &fgNewColor);

                if ( result )
                        RGBForeColor(&fgNewColor);
        }

        if ( !result )
        {
#ifdef THINK_CPLUS
                PenPat(ODQDGlobals.gray);
#else
                PenPat(&ODQDGlobals.gray);
#endif
        }

        return result;
}

//------------------------------------------------------------------------------
// DrawDeactivePICTItem
//------------------------------------------------------------------------------
//
// Draws a dimmed image of the preceeding dialog item, which should be
// beyond the visible area of the dialog, in this item's rectangle.

ODStatic pascal void DrawDeactivePICTItem(DialogPtr theDialog, short theItem)
{
        Rect            itemRect, otherRect;
        Handle          itemHandle, otherHandle;
        short           itemKind, otherKind;

        WindowPtr       savePort;
        PenState        savePen;

        RGBColor        fgSaveColor;
        Boolean         isColor;

        GetPort(&savePort);
        SetPort(theDialog);
        GetPenState(&savePen);
        PenNormal();

        // Get this dialog item
        GetDialogItem(theDialog, theItem, &itemKind, &itemHandle, &itemRect);

        // Get the dialog item to be drawn disabled
        GetDialogItem(theDialog, theItem-1, &otherKind, &otherHandle, &otherRect);
        if ( (otherKind & ~itemDisable) == picItem )
        {
                DrawPicture((PicHandle) otherHandle, &itemRect);
                isColor = PrepareToDrawGray(theDialog, theItem, &fgSaveColor);
                if ( isColor )
                        PenMode(addMax);
                else
                        PenMode(patBic);
                PaintRect(&itemRect);
                if ( isColor )
                        RGBForeColor(&fgSaveColor);
        }

        SetPenState(&savePen);
        SetPort(savePort);
}

//------------------------------------------------------------------------------
// InitItemActive
//------------------------------------------------------------------------------

ODStatic void InitItemActive(
                                        DialogPtr       dlg,
                                        short           itemNumber,
                                        UserItemUPP     itemUPP,
                                        ODBoolean       itemActive)
{
        Rect            itemRect, otherRect;
        Handle          itemHandle, otherHandle;
        short           itemKind, otherKind;

        // Get the rectangle from the actual item
        GetDialogItem(dlg, itemNumber, &otherKind, &otherHandle, &otherRect);

        // Establish the draw routine and rectangle for the user item
        GetDialogItem(dlg, itemNumber+1, &itemKind, &itemHandle, &itemRect);
        SetDialogItem(dlg, itemNumber+1, itemKind, (Handle)itemUPP, &otherRect);

        if ( itemActive )
                HideDialogItem(dlg, itemNumber+1);
        else
                HideDialogItem(dlg, itemNumber);
}

//------------------------------------------------------------------------------
// SetControlActive
//------------------------------------------------------------------------------
ODStatic void SetControlActive(
                                        DialogPtr       dlg,
                                        short           itemNumber,
                                        ODBoolean       itemActive)
{
        short           itemType;
        Handle          itemHandle;
        Rect            itemRect;

        GetDialogItem(dlg, itemNumber, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        HiliteControl((ControlHandle)itemHandle, itemActive ? kControlActive : kControlInactive);
}

//------------------------------------------------------------------------------
// BestContentKind
//------------------------------------------------------------------------------
ODStatic ODType BestContentKind(ODStorageUnit* contentSU)
{
        Environment* ev = somGetGlobalEnvironment();

        ODType bestKind = ODGetISOStrProp(ev, contentSU, kODPropPreferredKind, kODISOStr, kODNULL, kODNULL);

        if ( bestKind == kODNULL )
        {
                if (contentSU->Exists(ev, kODPropContents, (ODValueType)kODNULL, 1))
                {
                        contentSU->Focus(ev, kODPropContents, kODPosUndefined, (ODValueType)kODNULL, 1, kODPosUndefined);
                        bestKind = contentSU->GetType(ev);
                }
        }

        return bestKind;
}

//------------------------------------------------------------------------------
// SetLinkUpdateUpdateControls
//------------------------------------------------------------------------------
ODStatic void SetLinkUpdateUpdateControls(DialogPtr dlg, ODBoolean autoUpdateSetting)
{
        short   itemType;
        Handle  itemHandle;
        Rect    itemRect;

        GetDialogItem(dlg, kLinkInfoOnSaveRadioBtn, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, autoUpdateSetting);
        GetDialogItem(dlg, kLinkInfoManualRadioBtn, &itemType, &itemHandle, &itemRect);
        ASSERT_CONTROL_ITEM(itemType);
        SetControlValue((ControlHandle)itemHandle, !autoUpdateSetting);
}

//------------------------------------------------------------------------------
// LinkInfoFilterProc
//------------------------------------------------------------------------------

static pascal Boolean
LinkInfoFilterProc(DialogPtr dialog, EventRecord *event, short *itemHit)
{
        // If the event is a deactivate event, the ok button must be deactivated
        // before the surrounding outline is drawn so it is drawn dim.
        ODBoolean rslt = ODButtonKeyFilterProc(dialog,event,itemHit);

        if ( event->what == activateEvt )
        {
                ODOutlineDefaultButtonDrawProc(dialog, kLinkInfoDefaultButtonItem);
        }

        return rslt;
}

//------------------------------------------------------------------------------
// ShowLinkInfo
//------------------------------------------------------------------------------

ODStatic ODBoolean ShowLinkInfo(
                                                DialogPtr                       dlg,
                                                ConstStr255Param        creationDate,
                                                ConstStr255Param        creationTime,
                                                ConstStr255Param        modificationDate,
                                                ConstStr255Param        modificationTime,
                                                ODBoolean                       needsUpdate,
                                                ODLinkInfoResult*       infoResult
                                                )
{
        short   itemHit;

        ParamText(creationDate, creationTime, modificationDate, modificationTime);

        ShowWindow(dlg);

        ModalFilterUPP modalFilter = NewModalFilterProc(LinkInfoFilterProc);

        do {
                ODSLong refSaved = BeginUsingLibraryResources();
                ModalDialog(modalFilter, &itemHit);
                EndUsingLibraryResources(refSaved);

                switch (itemHit)
                {
                case kLinkInfoManualRadioBtn :
                        if ( infoResult->autoUpdate )
                        {
                                infoResult->autoUpdate = kODFalse;
                                SetLinkUpdateUpdateControls(dlg, infoResult->autoUpdate);
                                SetControlActive(dlg, kLinkInfoUpdateBtn, needsUpdate);
                        }
                        break;

                case kLinkInfoOnSaveRadioBtn :
                        if ( !infoResult->autoUpdate )
                        {
                                infoResult->autoUpdate = kODTrue;
                                SetLinkUpdateUpdateControls(dlg, infoResult->autoUpdate);
                                SetControlActive(dlg, kLinkInfoUpdateBtn, kODFalse);
                        }
                        break;
                }
        } while ((itemHit != kLinkInfoOKBtn) && (itemHit != kLinkInfoCancelBtn) &&
                         (itemHit != kLinkInfoUpdateBtn) && (itemHit != kLinkInfoBreakLinkBtn) &&
                         (itemHit != kLinkInfoFindSrcBtn));

        switch (itemHit) {
        case kLinkInfoOKBtn:
                        infoResult->action = kODLinkInfoOk;
                        break;
        case kLinkInfoUpdateBtn:
                        infoResult->action = kODLinkInfoUpdateNow;
                        break;
        case kLinkInfoBreakLinkBtn:
                        infoResult->action = kODLinkInfoBreakLink;
                        break;
        case kLinkInfoFindSrcBtn:
                        infoResult->action = kODLinkInfoFindSource;
                        break;
        default:
                        infoResult->action = kODLinkInfoCancel;
                        break;
        }

        ODDisposeRoutineDescriptor(modalFilter);

        return (itemHit != kLinkInfoCancelBtn);
}

//------------------------------------------------------------------------------
// DrawKindName
//------------------------------------------------------------------------------

pascal void DrawKindName(DialogPtr dialog, SInt16 item)
{
        short   itemType;
        Handle  itemHandle;
        Rect    itemRect;

        GetDialogItem(dialog, item, &itemType, &itemHandle, &itemRect);

        DrawITextInDlogBox(gKindName, &itemRect, dialog, kODTrue);
}

//------------------------------------------------------------------------------
// ShowLinkSourceInfo
//------------------------------------------------------------------------------

ODBoolean ShowLinkSourceInfo(
                                ODBaseLinkSource*       linkSource,
                                ODUpdateID                      change,
                                ODBoolean                       changesAllowed,
                                ODLinkInfoResult*       infoResult)
{
        Environment* ev = somGetGlobalEnvironment();

        WindowPtr       savePort;                                               ODVolatile(savePort);
        GetPort(&savePort);
        ODSLong         refSaved;

        DialogPtr       dlg = kODNULL;                                  ODVolatile(dlg);
        UserItemUPP DrawBoxItemUPP = kODNULL;           ODVolatile(DrawBoxItemUPP);
        UserItemUPP DrawKindNameUPP = kODNULL;          ODVolatile(DrawKindNameUPP);
        short           itemType;
        Handle          itemHandle;
        Rect            itemRect;
        Str255          creationDate, modificationDate;
        Str255          creationTime, modificationTime;
        ODType          kind;
        ODBoolean       needsUpdate;
        ODBoolean       result;

        TRY
                ODSession* session = linkSource->GetStorageUnit(ev)->GetSession(ev);
                ODNameSpaceManager* nsm = session->GetNameSpaceManager(ev);

                {
                        CUsingLibraryResources r;
                        dlg = ODGetNewDialog(ev, kLinkSrcInfoDlgID, session, kNoDefaultButton);
                        THROW_IF_NULL(dlg);
                }

                SetPort(dlg);

                {       CUsingLibraryResources r;
                        SetDialogTextStyle(dlg, kLinkSrcInfoDlgID, smCurrentScript);
                }

                // Set the draw routine for the default button outline item
                GetDialogItem(dlg, kLinkInfoDefaultButtonItem, &itemType, &itemHandle, &itemRect);
                SetDialogItem(dlg, kLinkInfoDefaultButtonItem, itemType, (Handle)GetODOutlineDefaultButtonDrawProc(), &itemRect);

                // Vertical and horizontal lines need to be drawn
                GetDialogItem(dlg, kLinkInfoSeparator, &itemType, &itemHandle, &itemRect);
                DrawBoxItemUPP = NewUserItemProc(DrawGrayBoxItem);
                SetDialogItem(dlg, kLinkInfoSeparator, itemType, (Handle)DrawBoxItemUPP, &itemRect);

                ODLinkKey key;
                if (linkSource->Lock(ev, 0, &key))
                {
                        TRY
                                ODStorageUnit*  contentSU = linkSource->GetContentStorageUnit(ev, key);

                                // Set up kind
                                kind = BestContentKind(contentSU);
                                DisposeGlobalODNameAndZeroPtr(&gKindName);
                                if ( kind && GetUserKindFromKind(nsm, kind, &gKindName) )
                                {
                                        GetDialogItem(dlg, kLinkInfoKind, &itemType, &itemHandle, &itemRect);
                                        DrawKindNameUPP = NewUserItemProc(DrawKindName);
                                        SetDialogItem(dlg, kLinkInfoKind, userItem+itemDisable, (Handle) DrawKindNameUPP, &itemRect);
                                }
                                ODDisposePtr((ODPtr) kind);

                                // Set up creation date
                                SetDateTimeStrings(ODGetCreationDate(ev, ODGetSUFromPstObj(ev, linkSource)),
                                                                        creationDate, creationTime);

                                // Set up modified date
                                SetDateTimeStrings(linkSource->GetChangeTime(ev), modificationDate, modificationTime);
                        CATCH_ALL
                                linkSource->Unlock(ev, key);
                                RERAISE;
                        ENDTRY

                        linkSource->Unlock(ev, key);
                }

                // Set up Send Updates
                SetLinkUpdateUpdateControls(dlg, infoResult->autoUpdate);

                needsUpdate = (change == kODUnknownUpdate) || (change != linkSource->GetUpdateID(ev));

                // Disable Update Now button if updates are automatic or link is up to date.
                if ( infoResult->autoUpdate || !needsUpdate )
                {
                        GetDialogItem(dlg, kLinkInfoUpdateBtn, &itemType, &itemHandle, &itemRect);
                        ASSERT_CONTROL_ITEM(itemType);
                        HiliteControl((ControlHandle)itemHandle, kControlInactive);
                }

                // Disable some items if changes are not allowed or draft is read only
                if ( !changesAllowed || DraftIsReadOnly(ev, linkSource->GetStorageUnit(ev)) )
                {
                        SetControlActive(dlg, kLinkInfoOnSaveRadioBtn, kODFalse);
                        SetControlActive(dlg, kLinkInfoManualRadioBtn, kODFalse);
                        SetControlActive(dlg, kLinkInfoBreakLinkBtn, kODFalse);
                        SetControlActive(dlg, kLinkInfoUpdateBtn, kODFalse);
                }

                result = ShowLinkInfo(dlg, creationDate, creationTime, modificationDate,
                                                                modificationTime, needsUpdate, infoResult);

        CATCH_ALL
                ODDisposeRoutineDescriptor(DrawBoxItemUPP);
                ODDisposeRoutineDescriptor(DrawKindNameUPP);

                refSaved = BeginUsingLibraryResources();
                ODDisposeDialog(dlg);
                EndUsingLibraryResources(refSaved);

                SetPort(savePort);
                RERAISE;
        ENDTRY

        ODDisposeRoutineDescriptor(DrawBoxItemUPP);
        ODDisposeRoutineDescriptor(DrawKindNameUPP);

        refSaved = BeginUsingLibraryResources();
        ODDisposeDialog(dlg);
        EndUsingLibraryResources(refSaved);

        SetPort(savePort);

        return result;
}

//------------------------------------------------------------------------------
// SetDateTimeStrings
//------------------------------------------------------------------------------

ODStatic void SetDateTimeStrings(
                                        ODULong         dateTime,
                                        Str255          dateString,
                                        Str255          timeString)
{
        if ( dateTime == 0 )
        {
                CUsingLibraryResources r;

                StringHandle str = GetString(kODLinkInfoStrUnknownID);
                if ( str != kODNULL )
                {
                        ODLockHandle((ODHandle) str);
                        ODBlockMove((ODPtr) *str, (ODPtr) dateString, (ODULong) (*str[0])+1);
                        ODUnlockHandle((ODHandle) str);
                        ReleaseResource((Handle) str);
                }
                else
                {
                        dateString[0] = (char) 0;
                }
                timeString[0] = (char) 0;
        }
        else
        {
                DateString(dateTime, abbrevDate, dateString, kODNULL);
                TimeString(dateTime, kODFalse, timeString, kODNULL);
        }
}

//------------------------------------------------------------------------------
// DraftIsReadOnly
//------------------------------------------------------------------------------
ODStatic ODBoolean DraftIsReadOnly(Environment* ev, ODStorageUnit* su)
{
        return (!HAS_WRITE_ACCESS(su->GetDraft(ev)->GetPermissions(ev)));
}

//------------------------------------------------------------------------------
// ShowLinkDestinationInfo
//------------------------------------------------------------------------------

ODBoolean ShowLinkDestinationInfo(
                                ODBaseLink*                     link,
                                ODLinkInfo*             info,
                                ODBoolean                       changesAllowed,
                                ODLinkInfoResult*       infoResult)
{
        Environment* ev = somGetGlobalEnvironment();

        WindowPtr       savePort;                                               ODVolatile(savePort);
        GetPort(&savePort);
        ODSLong         refSaved;

        short           itemType;
        DialogPtr       dlg = kODNULL;                                  ODVolatile(dlg);
        UserItemUPP DrawBoxItemUPP = kODNULL;           ODVolatile(DrawBoxItemUPP);
        UserItemUPP DrawKindNameUPP = kODNULL;          ODVolatile(DrawKindNameUPP);
        Handle          itemHandle;
        Rect            itemRect;
        Str255          creationDate, modificationDate;
        Str255          creationTime, modificationTime;
        ODBoolean       needsUpdate;
        ODBoolean       result;

        TRY
                ODSession* session = link->GetStorageUnit(ev)->GetSession(ev);
                ODNameSpaceManager* nsm = session->GetNameSpaceManager(ev);

                {
                        CUsingLibraryResources r;
                        dlg = ODGetNewDialog(ev, kLinkDstInfoDlgID, session, kNoDefaultButton);
                        THROW_IF_NULL(dlg);
                }

                SetPort(dlg);

                {       CUsingLibraryResources r;
                        SetDialogTextStyle(dlg, kLinkDstInfoDlgID, smCurrentScript);
                }

                // Set the draw routine for the default button outline item
                GetDialogItem(dlg, kLinkInfoDefaultButtonItem, &itemType, &itemHandle, &itemRect);
                SetDialogItem(dlg, kLinkInfoDefaultButtonItem, itemType, (Handle)GetODOutlineDefaultButtonDrawProc(), &itemRect);

                // Vertical and horizontal lines need to be drawn
                GetDialogItem(dlg, kLinkInfoSeparator, &itemType, &itemHandle, &itemRect);
                DrawBoxItemUPP = NewUserItemProc(DrawGrayBoxItem);
                SetDialogItem(dlg, kLinkInfoSeparator, itemType, (Handle) DrawBoxItemUPP, &itemRect);

                // Set up kind
                DisposeGlobalODNameAndZeroPtr(&gKindName);
                if ( info->kind && GetUserKindFromKind(nsm, info->kind, &gKindName) )
                {
                        GetDialogItem(dlg, kLinkInfoKind, &itemType, &itemHandle, &itemRect);
                        DrawKindNameUPP = NewUserItemProc(DrawKindName);
                        SetDialogItem(dlg, kLinkInfoKind, userItem+itemDisable, (Handle) DrawKindNameUPP, &itemRect);
                }

                // Set up creation date
                SetDateTimeStrings(info->creationTime, creationDate, creationTime);

                // Set up modified date
                SetDateTimeStrings(info->changeTime, modificationDate, modificationTime);

                // Set up Send Updates
                SetLinkUpdateUpdateControls(dlg, info->autoUpdate);

                needsUpdate = (info->change == kODUnknownUpdate) || (info->change != link->GetUpdateID(ev));

                // Disable Update Now button if updates are automatic or destination is up to date.
                if ( info->autoUpdate || !needsUpdate )
                {
                        SetControlActive(dlg, kLinkInfoUpdateBtn, kODFalse);
                }

                // Disable some items if changes are not allowed or draft is read only
                if ( !changesAllowed || DraftIsReadOnly(ev, link->GetStorageUnit(ev)) )
                {
                        SetControlActive(dlg, kLinkInfoOnSaveRadioBtn, kODFalse);
                        SetControlActive(dlg, kLinkInfoManualRadioBtn, kODFalse);
                        SetControlActive(dlg, kLinkInfoBreakLinkBtn, kODFalse);
                        SetControlActive(dlg, kLinkInfoUpdateBtn, kODFalse);
                }

                result = ShowLinkInfo(dlg, creationDate, creationTime, modificationDate,
                                                                modificationTime, needsUpdate, infoResult);

        CATCH_ALL
                ODDisposeRoutineDescriptor(DrawBoxItemUPP);
                ODDisposeRoutineDescriptor(DrawKindNameUPP);

                refSaved = BeginUsingLibraryResources();
                ODDisposeDialog(dlg);
                EndUsingLibraryResources(refSaved);

                SetPort(savePort);
                RERAISE;
        ENDTRY

        ODDisposeRoutineDescriptor(DrawBoxItemUPP);
        ODDisposeRoutineDescriptor(DrawKindNameUPP);

        refSaved = BeginUsingLibraryResources();
        ODDisposeDialog(dlg);
        EndUsingLibraryResources(refSaved);

        SetPort(savePort);

        return result;
}

//------------------------------------------------------------------------------
// ShowLinkUpdateAlert
//------------------------------------------------------------------------------

ODBoolean ShowLinkUpdateAlert( /*ODUShort count*/ )
{
        Environment* ev = somGetGlobalEnvironment();

        // ODUnused(count);

        gSession->GetWindowState(ev)->DeactivateFrontWindows(ev);
        ODSShort alertResult = ShowAlert(ev, kLinkUpdateAlertID, GetODButtonKeyFilterProc(), gSession);
        gSession->GetWindowState(ev)->ActivateFrontWindows(ev);

        ResetAlertStage();

        return (alertResult == kLinkUpdateContinueBtn);
}

//------------------------------------------------------------------------------
// ShowLinkStatusAlert
//------------------------------------------------------------------------------

void ShowLinkStatusAlert(ODError error)
{
        Environment* ev = somGetGlobalEnvironment();

        ODSShort alertID;
        Str63 numberString;

        NumToString(error, numberString);
        ParamText(numberString, 0, 0, 0);

        if ( error == kODErrCannotFindLinkSourceEdition )
                alertID = kLinkEditionAlertID;
        else
                alertID = kLinkStatusAlertID;

        gSession->GetWindowState(ev)->DeactivateFrontWindows(ev);
        ShowAlert(ev, alertID, GetODButtonKeyFilterProc(), gSession);
        gSession->GetWindowState(ev)->ActivateFrontWindows(ev);

        ResetAlertStage();
}
#endif // _PLATFORM_MACINTOSH_
