/* @(#)Z 1.54 os2/src/docshell/OS2ShUtl.cpp, odshell, od96os2, odos29646d 96/11/15 15:51:38 (96/11/12 15:03:40) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odshell
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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  This file contains utility functions, primarily dialog related, used by   //
//  the Shell classes.                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "ODos2.h"

#include "DocMgr.xh"
#include "OS2Shell.xh"
#include "WinStat.xh"
#include "Window.xh"
#include "CmdDefs.xh"
#include "WinIter.xh"
#include "ODPrtReg.xh"
#include "ODPrtRei.xh"

#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif
#ifndef SOM_ODHelp_xh
#include <ODHelp.xh>
#endif
#ifndef _HLPPANELS_
#include <hlppanls.h>
#endif

#include "TempObj.h"
#include "ODTypes.h"
#include "DraftWn.h"
#include "DocShell.h"
#include "odres.h"
#include "odregapi.h"
#include <stdio.h>
#include "LinkB.xh"

OS2Shell  *shell;

// structure used for container in draft history dialog.
typedef struct _ODINFO
{
    MINIRECORDCORE	minirecordCore;
    PSZ			creatorUserName;
    USHORT		draftNumber;
    PSZ		        date;
    PSZ  		time;
    PSZ			comments;
} ODINFO, *PODINFO;

// Structure used to pass information to and from the Select Kind dialog.
struct DocInfo {
    ODULong                  textType;
    char                     selText [kODMaxLength];
    ODUShort                 selIndex;
    _IDL_SEQUENCE_ISOString  kindList;
};

// Structure used to pass information to and from the Create Draft dialog.
struct DraftInfo {
    ODULong   draftNum;
    char      name     [kODMaxLength];
    char      comment  [kODMaxLength];
    ODBoolean cancel;
};

enum      {PARTKIND_BUFFER_SIZE = 1024};

#include <ShellUtl.h>

//------------------------------------------------------------------------------
// SetShell - Sets the shell static global variable for other functions
//              in this file to use.
//------------------------------------------------------------------------------
SOMEXTERN void SOMLINK SetShell(OS2Shell *somSelf)
{
    shell = somSelf;
}
    
//------------------------------------------------------------------------------
// SelectKindDlg - invokes the Select Part Kind Dialog.
//------------------------------------------------------------------------------
SOMEXTERN ODULong SOMLINK SelectKindDlg(Environment *ev, char* selText)
{
    DocInfo docInfo;
    WindowSystemData *windowSystemData = NULL;

    try
    {
       windowSystemData = shell->GetWindowSystemData(ev);
    }
    catch (ODException _exception)
    {
       LOG("OpenDoc exception occured in SelectKindDlg.");
       throw;
    }

    // Initialize docInfo.textType to kODCommandCancel
    docInfo.textType = kODCommandCancel;
    docInfo.kindList._length = 0;

    selText[0] = '\0';

    if (windowSystemData)
       ULONG ulresult = WinDlgBox(HWND_DESKTOP, windowSystemData->parentWindow, 
				(PFNWP) SelectKindDialogFunc,
				windowSystemData->resInstance,
               		 	IDD_SELPARTKIND, (PVOID) &docInfo); 
    
    if ((docInfo.textType == kODKindName) && (docInfo.kindList._length))
    {
       strcpy (selText,docInfo.kindList._buffer[docInfo.selIndex]);

    } else if (docInfo.textType == kODFileName) 
       strcpy(selText, docInfo.selText);

    // Free the kind list that was saved in docInfo.kindList during the 
    // dialog initialization.
    for ( int i = 0; i < docInfo.kindList._length; i++)
        SOMFree( docInfo.kindList._buffer[i]);
    SOMFree( docInfo.kindList._buffer);

    return docInfo.textType;
}

//------------------------------------------------------------------------------
// CreateDraftDlg - invokes the Create Draft Dialog.
//------------------------------------------------------------------------------
SOMEXTERN void SOMLINK CreateDraftDlg(Environment *ev, ODULong draftNum)
{
    HWND hwnd = 0;

    try
    {
      DraftInfo draftInfo;
      ODWindowState* winState = shell->GetWindowState(ev);

      // Need to encapsulate this so it will be released when going out
      // of scope.
      {
        // Use TempODWindow so this object won't have to be released.
        TempODWindow Window = winState->AcquireActiveWindow(ev);
        hwnd = Window->GetPlatformWindow(ev);
      }

      // Use a staticly defined variable to pass in draft number
      draftInfo.draftNum = draftNum;
      draftInfo.cancel = kODFalse;

      // Before creating the dialog, disable mouse and keyboard inputs
      // to all the OD windows except the window that is going to own
      // the dialog. After dismissing the dialog, enable back all the
      // OD windows except the owner window for the dialog.
      EnableApplModal(ev, hwnd, FALSE);
      ULONG ulresult = WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) CreateDialogFunc,
			shell->GetWindowSystemData(ev)->resInstance,
                	IDD_CREATEDRAFTDLG, (PVOID) &draftInfo); 
      EnableApplModal(ev, hwnd, TRUE);

      // If user didn't choose cancel on Create Draft dialog,
      // call DocumentManager to create draft.
      if (!draftInfo.cancel)
          shell->GetActiveDocumentManager(ev)->CreateDraft(ev,
          	draftInfo.draftNum,
          	CreateIText(smRoman,langEnglish, draftInfo.name),
          	CreateIText(smRoman,langEnglish, draftInfo.comment));
  
    }
    catch (ODException _exception)
    {
       LOG("OpenDoc exception occured in CreateDraftDlg.");
       if (hwnd)
          RestoreApplModal(hwnd);
       throw;
    }
}

//------------------------------------------------------------------------------
// ConfirmCloseDlg - invokes a confirmation dialog when close is selected from 
//                   document menu or window menu and draft has unsaved changes.
//------------------------------------------------------------------------------
SOMEXTERN ODULong SOMLINK ConfirmCloseDlg(Environment *ev)
{

    ULONG ulResult; 
    ODULong ret = kODCommandCancel;
    HWND hwnd = 0;

    try
    {
	// Get client window handle 
	ODWindowState *winState = shell->GetWindowState(ev);
	TempODWindow window = winState->AcquireActiveWindow(ev);
	hwnd = window->GetPlatformWindow(ev);

        if (hwnd)
        {
            // Before creating the dialog, disable mouse and keyboard inputs
            // to all the OD windows except the window that is going to own
            // the dialog. After dismissing the dialog, enable back all the
            // OD windows except the owner window for the dialog.
            EnableApplModal(ev, hwnd, FALSE);
    	    ulResult = WinMessageBox(HWND_DESKTOP, hwnd, 
	    	catgets(ODcat_handle, DOCSH_SET, DOCMSG_8, 
	    	"Do you want to save your changes?"),
	    	catgets(ODcat_handle, DOCSH_SET, DOCMSG_9,"Close Confirmation"),
	    	0, MB_ICONQUESTION | MB_YESNOCANCEL);
            EnableApplModal(ev, hwnd, TRUE);

	    switch (ulResult)
	    {
		case MBID_YES:
			ret = kODCommandSaveChanges;
			break;
		case MBID_NO:
			ret = kODCommandDiscardChanges;
			break;
		case MBID_CANCEL:
		default:
			ret = kODCommandCancel;
	    }
        }
    }
    catch (ODException _exception)
    {
	LOG("OpenDoc exception occured in ConfirmCloseDlg.");
        if (hwnd)
           RestoreApplModal(hwnd);
        throw;
    }
    return ret;
}

//------------------------------------------------------------------------------
// ConfirmExitDlg - invokes an eixt confirmation dialog when close is selected 
//                   from document menu or window menu and draft has no 
//                   unsaved changes. 
//------------------------------------------------------------------------------
SOMEXTERN ODULong SOMLINK ConfirmExitDlg(Environment *ev)
{

    ULONG ulResult; 
    ODULong ret = kODCommandCancel;
    HWND hwnd = 0;

    try
    {
	// Get client window handle 
	ODWindowState *winState = shell->GetWindowState(ev);
	TempODWindow window = winState->AcquireActiveWindow(ev);
	hwnd = window->GetPlatformWindow(ev);

        if (hwnd)
        {
            // Before creating the dialog, disable mouse and keyboard inputs
            // to all the OD windows except the window that is going to own
            // the dialog. After dismissing the dialog, enable back all the
            // OD windows except the owner window for the dialog.
            EnableApplModal(ev, hwnd, FALSE);
       	    ulResult = WinMessageBox(HWND_DESKTOP, hwnd, 
	    	catgets(ODcat_handle, DOCSH_SET, DOCMSG_25, 
	    	"Do you want to close this document?"),
	    	catgets(ODcat_handle, DOCSH_SET, DOCMSG_26,"Exit Confirmation"),
	    	0, MB_ICONQUESTION | MB_YESNO);
            EnableApplModal(ev, hwnd, TRUE);

    	    switch (ulResult)
	    {
		case MBID_YES:
			ret = kODCommandDiscardChanges;
			break;
		case MBID_NO:
		default:
			ret = kODCommandCancel;
	    }
        }
    }
    catch (ODException _exception)
    {
	LOG("OpenDoc exception occured in ConfirmCloseDlg.");
        if (hwnd)
           RestoreApplModal(hwnd);
        throw;
    }
    return ret;
}
//------------------------------------------------------------------------------
// ConfirmRevertDlg - invokes a dialog confirming user's request to revert to 
//                    a previous draft.
//------------------------------------------------------------------------------
SOMEXTERN ODBoolean SOMLINK ConfirmRevertDlg(Environment *ev)
{ 
    ULONG ulResult; 
    ODBoolean ret = kODCommandCancel;
    HWND hwnd = 0;

    try
    {
	// Get client window handle 
	ODWindowState *winState = shell->GetWindowState(ev);
	TempODWindow window = winState->AcquireActiveWindow(ev);
	hwnd = window->GetPlatformWindow(ev);

        if (hwnd)
        {
            // Before creating the dialog, disable mouse and keyboard inputs
            // to all the OD windows except the window that is going to own
            // the dialog. After dismissing the dialog, enable back all the
            // OD windows except the owner window for the dialog.
            EnableApplModal(ev, hwnd, FALSE);
    	    ulResult = WinMessageBox(HWND_DESKTOP, hwnd, 
	    	catgets(ODcat_handle, DOCSH_SET, DOCMSG_10, 
	    	"Do you want to revert to the last saved version of this document?"),
	    	catgets(ODcat_handle, DOCSH_SET, DOCMSG_11, 
		"Revert Confirmation"),
	    	0, MB_ICONQUESTION | MB_OKCANCEL);
            EnableApplModal(ev, hwnd, TRUE);

	    switch (ulResult)
	    {
		case MBID_OK:
			ret = kODTrue;
			break;
		case MBID_CANCEL:
		default:
			ret = kODFalse;
	    }
        }
    }
    catch (ODException _exception)
    {
	LOG("OpenDoc exception occured in ConfirmRevertDlg.");
        if (hwnd)
           RestoreApplModal(hwnd);
        throw;
    }

    return ret;
}

//------------------------------------------------------------------------------
// ConfirmDeleteDlg - invokes a dialog confirming user's request to replace  
//                    an existing file.
//------------------------------------------------------------------------------
SOMEXTERN ODBoolean SOMLINK ConfirmDeleteDlg(Environment *ev,
				HWND parentHwnd,
				CHAR *fileName)
{ 
    ULONG ulResult; 
    ODBoolean ret = kODCommandCancel;

    if (parentHwnd)
    {
	char *msg = catgets(ODcat_handle, DOCSH_SET, DOCMSG_23, 
	    	"%s already exists. Do you want to replace it?");
        char *msgText = new char[strlen(msg) + strlen(fileName) + 1];
        sprintf(msgText, msg, fileName);

    	ulResult = WinMessageBox(HWND_DESKTOP, parentHwnd, msgText,
	    	catgets(ODcat_handle, DOCSH_SET, DOCMSG_24, "Save As"),
	    	0, MB_ICONQUESTION | MB_YESNO);
	switch (ulResult)
	{
		case MBID_YES:
			ret = kODTrue;
			break;
		case MBID_NO:
		default:
			ret = kODFalse;
	}
        delete[] msgText;
    }

    return ret;
}

//------------------------------------------------------------------------------
// AboutDlg - invokes Message Box dialog when "OpenDoc Information" is
//           selected from the Help menu.
//------------------------------------------------------------------------------
SOMEXTERN void SOMLINK AboutDlg (Environment *ev)
{
      HWND hwnd = 0;
      ODWindowState* winState = shell->GetWindowState(ev);

      // Need to encapsulate this so it will be released when going out
      // of scope.
      {
        // Use TempODWindow so this object won't have to be released.
        TempODWindow Window = winState->AcquireActiveWindow(ev);
        hwnd = Window->GetPlatformWindow(ev);
      }

      EnableApplModal(ev, hwnd, FALSE);
      WinMessageBox(HWND_DESKTOP, hwnd,
                 catgets(ODcat_handle,DOCSH_SET,DOCMSG_16,
                 "         IBM OpenDoc(R) Version 1.1\n\n(c) Copyright IBM 1996, all rights reserved.\nOpenDoc is a trademark of Apple Computer Inc.,\nregistered in the United States and other countries."),
                 catgets(ODcat_handle,DOCSH_SET,DOCMSG_2, "Information"),
                 0, MB_OK | MB_INFORMATION);
      EnableApplModal(ev, hwnd, TRUE);
}

//------------------------------------------------------------------------------
// OpenFileDlg - invokes the common Open File dialog box.
//------------------------------------------------------------------------------
SOMEXTERN void SOMLINK OpenFileDlg(HWND hwnd, char* docName)
{
    // Display the common fileopen dialog
    FILEDLG fileDlg;
    HWND hwndDlg;
    Environment *ev = somGetGlobalEnvironment();

    // Set up FILEDLG structure.
    memset(&fileDlg, 0, sizeof(FILEDLG));
    fileDlg.cbSize = sizeof(FILEDLG);
    fileDlg.fl = 
      FDS_CENTER | FDS_OPEN_DIALOG | FDS_INCLUDE_EAS | FDS_FILTERUNION; 
    fileDlg.pszIType = catgets(ODcat_handle,DOCSH_SET,DOCMSG_15,
				"OpenDoc Document");

    try
    {
       // Before creating the dialog, disable mouse and keyboard inputs
       // to all the OD windows except the window that is going to own
       // the dialog.
       EnableApplModal(ev, hwnd, FALSE);
    }
    catch (ODException _exception)
    {
       LOG("OpenDoc exception occured in OpenFileDlg.");
       throw;
    }

    do // invoke Open File dialog until user enters valid filename or cancels.
    {
      // If owner window is not enabled, enable it.
      if (!WinIsWindowEnabled(hwnd))
         WinEnableWindow(hwnd, TRUE);

      strcpy(fileDlg.szFullFile, "*.od");
      hwndDlg = WinFileDlg(HWND_DESKTOP, hwnd, &fileDlg);

      // Need to disable the owner window for the open file dialog. Otherwise,
      // if a warnmsg is displayed in IsValidFileName, the owner window
      // is enable whereas the other windows are disable.
      if (hwnd != HWND_DESKTOP)
         WinEnableWindow(hwnd, FALSE);
    }
    while (hwndDlg && (fileDlg.lReturn == DID_OK) &&	
           !(shell->IsValidFileName(ev,fileDlg.szFullFile)));

    // Enable the owner window.
    // Note: The owner window may not be an OD window so has to enable
    //       it by itself here.
    WinEnableWindow(hwnd, TRUE);

    try
    {
       // After dismissing the dialog, enable back all the OD window except the
       // owner window
       EnableApplModal(ev, hwnd, TRUE);
    }
    catch (ODException _exception)
    {
       LOG("OpenDoc exception occured in OpenFileDlg.");
       throw;
    }

    if (hwndDlg && (fileDlg.lReturn == DID_OK))
      // Filename is valid by the time we make it to this point.
      strcpy(docName, fileDlg.szFullFile);
   
}


//------------------------------------------------------------------------------
// SaveFileDlg - invokes the common  Save File dialog box.
//------------------------------------------------------------------------------
SOMEXTERN PlatformFile* SOMLINK SaveFileDlg(Environment *ev)
{

    PlatformFile        *file = new PlatformFile;
    ODBoolean		confirm;
    HWND		hwnd = 0;

    // Display the common fileopen dialog

    try
    {
	
    	FILEDLG fileDlg;
	HWND hwndDlg;
	// Get client window handle 
	ODWindowState *winState = shell->GetWindowState(ev);
	TempODWindow window = winState->AcquireActiveWindow(ev);
	hwnd = window->GetPlatformWindow(ev);

	memset(&fileDlg, 0, sizeof(FILEDLG));
	fileDlg.cbSize = sizeof(FILEDLG);
	fileDlg.fl = FDS_CENTER | FDS_INCLUDE_EAS | FDS_SAVEAS_DIALOG;
        fileDlg.pszIType = catgets(ODcat_handle,DOCSH_SET,DOCMSG_15,
				"OpenDoc Document");

        // Before creating the dialog, disable mouse and keyboard inputs
        // to all the OD windows except the window that is going to own
        // the dialog. After dismissing the dialog, enable back all the
        // OD windows except the owner window for the dialog.
        EnableApplModal(ev, hwnd, FALSE);
	do
	{
		confirm = kODTrue;
	        strcpy(fileDlg.szFullFile, "*.od");
		hwndDlg = WinFileDlg(HWND_DESKTOP, hwnd, &fileDlg);
  
		if (hwndDlg && (fileDlg.lReturn == DID_OK))
		{
                        // Check if specified file exists or not. If
			// exists, display the confirmation dialog to
                        // replace the existing file.
			file->Specify((ODFileSpec *)fileDlg.szFullFile);
			ODBoolean exists = kODFalse;
			try
			{
				exists = file->Exists();
			}
			catch (ODException _exception)
			{
				// Need to disable the owner window for the 
				// save file dialog temporarily. Otherwise, 
				// the owner window is enable whereas the other
				// windows are disable when displaying the 
				// warning message.
				WinEnableWindow(hwnd, FALSE);
				WARNMSG(WARN_INDEX(AMSG_850),"CPPOD0335 - Docshell: Problem creating file %s.  File or pathname could be invalid.", fileDlg.szFullFile);
				WinEnableWindow(hwnd, TRUE);

                                // Clear the exception.
				SetErrorCodeEv(ev, kODNoError);
				delete file;
				file = kODNULL;
			}
			if (exists)
				confirm = ConfirmDeleteDlg(ev, hwnd,
						fileDlg.szFullFile);
		}
		else
		{
			delete file;
			file = kODNULL;
		}
	}
	while (confirm == kODFalse);

        EnableApplModal(ev, hwnd, TRUE);

    }
    catch (ODException _exception)
    {
	LOG("OpenDoc exception occured in SaveFileDlg.");
	if (file)
           delete file;
        file = kODNULL;
        if (hwnd)
           RestoreApplModal(hwnd);
        throw;
    }

    return file;
}

//----------------------------------------------------------------------------
// SelectKindDialogFunc - Dialog Box Function for Select Part Kind dialog.
//----------------------------------------------------------------------------
SOMEXTERN MRESULT SOMLINK SelectKindDialogFunc( HWND hwnd, ULONG msg, 
				MPARAM mp1, MPARAM mp2 )
{
  MPARAM    docInfo;
  HWND	    listBoxHWND = WinWindowFromID(hwnd, DID_KINDLIST);

  switch (msg)
  {
      //------------------------------------------------------
      //  Initialize the select part kind dialog
      //------------------------------------------------------
      case WM_INITDLG:
      {
          // Retrieve handle to part kind
          docInfo = (MPARAM)mp2;
	  WinSetWindowULong(hwnd, QWL_USER, (ULONG) docInfo);

          Environment              *ev = somGetGlobalEnvironment();
          ISOString                displayName;
          ODSession                *session;
          ODPartHandlerRegistryInt *registry;
          _IDL_SEQUENCE_ISOString  kindList;

          try
          {
              // Get the registered part kind display names
              session = shell->GetSession(ev);
              registry = session->GetRegistry(ev);
              kindList = registry->GetPartKindList(ev, kODNULL,kODNULL);

              // Hold onto registry info to pass back.
              ((DocInfo*) docInfo)->kindList = kindList;

              // If there are part kinds registered
              if (kindList._length) 
              {
                  for ( int i = 0; i < kindList._length; i++)
                  {
                      // Get the actual display name to show in the dialog
                      displayName = registry->GetKindDisplayNameFromKind(ev,
                                                       kindList._buffer[i]);

                      // Copy the part kind display name to dialog box
                      WinSendDlgItemMsg(hwnd,DID_KINDLIST,LM_INSERTITEM,
                                     MPFROMLONG(LIT_END),MPFROMP(displayName));
                  }

                  // Set the maximum width of list box
                  WinSendDlgItemMsg(hwnd, DID_KINDLIST, LM_SETITEMWIDTH,
                                (MPARAM)PARTKIND_BUFFER_SIZE, (MPARAM) 0);

                  // Set the cursor position to the 1st entry.
                  WinSendDlgItemMsg(hwnd,DID_KINDLIST,LM_SELECTITEM,
                                    MPFROMSHORT(0), MPFROMSHORT(TRUE));

                  // Set the focus onto the list part of the dialog
                  WinSetFocus(HWND_DESKTOP,listBoxHWND);

                  // Activate the dialog
                  WinSetWindowPos(hwnd,HWND_TOP,0,0,0,0,SWP_ACTIVATE|SWP_SHOW);
    
              } else {  // list was empty, problem getting information.

                  WARNMSG(WARN_INDEX(AMSG_560),
                      "Docshell: Couldn't load Part Kind list.");
    
                  ((DocInfo*) docInfo)->textType = kODNoRootPart;
    
                  WinDismissDlg(hwnd, TRUE);
              }
          }
          catch (ODException _exception)
          {
              WinDismissDlg(hwnd, TRUE);
              ((DocInfo*) docInfo)->textType = kODNoRootPart;
              for ( int i = 0; i <  kindList._length; i++)
                 SOMFree( kindList._buffer[i]);
              SOMFree( kindList._buffer);
              throw;
          }

          break;
      }

      case WM_CLOSE:
      {
         // Retrieve handle to the docInfo structure passed in 
         docInfo = (MPARAM) WinQueryWindowULong(hwnd, QWL_USER);

         // pass info back to caller
         ((DocInfo*) docInfo)->textType = kODCommandCancel;

         WinDismissDlg(hwnd, TRUE);
         break;
      }

      case WM_SYSCOMMAND:
      {
         if (SHORT1FROMMP(mp1) == SC_CLOSE)
         {
            // Retrieve handle to the docInfo structure passed in 
            docInfo = (MPARAM) WinQueryWindowULong(hwnd, QWL_USER);

            // pass info back to caller
            ((DocInfo*) docInfo)->textType = kODCommandCancel;

            WinDismissDlg(hwnd, TRUE);
         }
         else
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         break;
      }

      case WM_CONTROL:
      {
        // retrieve handle to the docInfo structure attached to window.
        // in WM_INITDIALOG case above.
        docInfo = (MPARAM) WinQueryWindowULong(hwnd, QWL_USER);

        if (SHORT1FROMMP(mp1) == DID_KINDLIST)
        {
           switch (SHORT2FROMMP(mp1))
           {
              // user chooses an item in the list box
              case LN_ENTER:
              {
                 // Save the index of the currently selected item
                 ((DocInfo*) docInfo)->selIndex = 
                                         WinQueryLboxSelectedItem(listBoxHWND);

                 ((DocInfo*) docInfo)->textType = kODKindName;

                 WinDismissDlg(hwnd, TRUE);

	         break;
              }

              default:
                 return WinDefDlgProc( hwnd, msg, mp1, mp2);
	   }
	}
      }

      case WM_COMMAND:
      {
        // retrieve handle to the docInfo structure attached to window
        // set in WM_INITDIALOG case above.
        docInfo = (MPARAM) WinQueryWindowULong(hwnd, QWL_USER);

        switch (SHORT1FROMMP(mp1))
        {
          // User cancelled dialog
          case DID_CANCEL:
          {
            ((DocInfo*) docInfo)->textType = kODCommandCancel;
            WinDismissDlg(hwnd, TRUE);
            break;
          }

          case DID_OK:
          {
            // Save the index of the currently selected item
            ((DocInfo*) docInfo)->selIndex = 
                                         WinQueryLboxSelectedItem(listBoxHWND);

            ((DocInfo*) docInfo)->textType = kODKindName;

            WinDismissDlg(hwnd, TRUE);

            break;
          }

          case DID_OPENFILE:
          {
             // Initialized selection text to NULL so we know whether
             // anything is selected within the Open File dialog, or
             // whether cancel was chosen.
             ((DocInfo*) docInfo)->selText[0] = '\0';

             // Display the Open File dialog and return the result.
             OpenFileDlg (hwnd,((DocInfo*) docInfo)->selText);

             // If a filename was selected, end select kind dialog.
             if (((DocInfo*) docInfo)->selText[0] != '\0')
             {
               WinDismissDlg(hwnd, TRUE);
               ((DocInfo*) docInfo)->textType = kODFileName;
             }
             break;
          }

          default:
             return WinDefDlgProc( hwnd, msg, mp1, mp2);

        } // switch (LOWORD(wParam))
        break;

      } // case WM_COMMAND

      case WM_HELP:
      {
        Environment* ev = somGetGlobalEnvironment();
        try
        {
          ODSession * session = shell->GetSession(ev);
          ODHelp    * helpme  = session->GetHelp(ev);
          helpme->DisplayHelp(ev,ODHELPFILE,IDMS_SELECT_ROOT_KIND_PANEL);
        }
        catch (ODException _exception)
        {
           SetErrorCodeEv(ev, kODNoError);
        }
            
        break;
      } // end case WM_HELP

      default:
	return WinDefDlgProc(hwnd, msg, mp1, mp2);

  } // switch (message)

}

//----------------------------------------------------------------------------
// CreateDialogFunc - Dialog Box Function for Create Draft confirmation dialog
//----------------------------------------------------------------------------
SOMEXTERN MRESULT SOMLINK CreateDialogFunc( HWND hwnd, ULONG msg, MPARAM mp1, 
					MPARAM mp2 )
{
  char         DraftNumStr[kODMaxLength]; 
  MPARAM       draftInfo;

    switch (msg)
    {

      //------------------------------------------------------
      //  Initialize the create draft dialog
      //------------------------------------------------------
      case WM_INITDLG:
      {
          // Retrieve handle to the DraftInfo structure passed in 
	  draftInfo = (MPARAM)mp2;
	  WinSetWindowULong(hwnd, QWL_USER, (ULONG) draftInfo);

          // Increment the draft number
          ((DraftInfo*) draftInfo)->draftNum++;

          // Need to convert draft # to an ascii string for dialog.
          sprintf(DraftNumStr,"%d",((DraftInfo*) draftInfo)->draftNum);

          // Set limit for text entry in Creator field.
          WinSendDlgItemMsg(hwnd, DID_DRAFTNAME, EM_SETTEXTLIMIT, 
                            (MPARAM) 45, (MPARAM) 0);

          // Set the draft number in dialog message
          WinSetDlgItemText(hwnd, DID_DRAFTNUM, DraftNumStr);
          WinSetWindowPos(hwnd, NULLHANDLE, 0, 0, 0, 0, 
				SWP_ACTIVATE | SWP_SHOW);
          WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, DID_DRAFTNAME));

          break;
      }

      case WM_CLOSE:
      {
         // Retrieve handle to the DraftInfo structure passed in 
         draftInfo = (MPARAM) WinQueryWindowULong(hwnd, QWL_USER);

         // Need to pass this information back to caller.
         ((DraftInfo*) draftInfo)->cancel = kODTrue;

         WinDismissDlg(hwnd, TRUE);
         break;
      }

      case WM_SYSCOMMAND:
      {
         if (SHORT1FROMMP(mp1) == SC_CLOSE)
         {
            // Retrieve handle to the DraftInfo structure passed in 
            draftInfo = (MPARAM) WinQueryWindowULong(hwnd, QWL_USER);

            // Need to pass this information back to caller.
            ((DraftInfo*) draftInfo)->cancel = kODTrue;

            WinDismissDlg(hwnd, TRUE);
         }
         else
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         break;
      }

      case WM_COMMAND:
      {
        // retrieve handle to the DraftInfo structure attached to window
        // set in WM_INITDLG case above.
        draftInfo = (MPARAM) WinQueryWindowULong(hwnd, QWL_USER);

        switch (SHORT1FROMMP(mp1))
        {
          // User cancelled dialog
          case DID_CANCEL:
          {
              // Need to pass this information back to caller.
              ((DraftInfo*) draftInfo)->cancel = kODTrue;

              WinDismissDlg(hwnd,TRUE);
              break;
          }

          // User asked to go ahead and save a draft.
          case DID_SAVE:
          {

              // Get the text from the name field.
              WinQueryDlgItemText(hwnd, DID_DRAFTNAME, kODMaxLength,
                             ((DraftInfo*) draftInfo)->name);

              // Get the text from the comment field.
              WinQueryDlgItemText(hwnd, DID_DRAFTCOMMENT, kODMaxLength,
                             ((DraftInfo*) draftInfo)->comment);

              WinDismissDlg(hwnd,TRUE);
              break;
           }
 
           default:
              break;
 
        } // switch 
        break;

      }

      case WM_HELP:
      {
         Environment* ev = somGetGlobalEnvironment();
         try
         {
            ODSession * session = shell->GetSession(ev);
            ODHelp    * helpme  = session->GetHelp(ev);
            helpme->DisplayHelp(ev,ODHELPFILE,IDMS_DR_DRAFT_CREATEDRAFT_PANEL);
         }
         catch (ODException _exception)
         {
            SetErrorCodeEv(ev, kODNoError);
         }
         break;
      }
 
      default:
	return WinDefDlgProc(hwnd, msg, mp1, mp2);
 
    } // switch (message)
 
}

//------------------------------------------------------------------------------
// ODDocumentWindowProc - Standard window procedure to run docshell window 
//                        events
//------------------------------------------------------------------------------
SOMEXTERN MRESULT SOMLINK ODDocumentWindowProc( HWND hwnd, ULONG msg, 
						MPARAM mp1, MPARAM mp2 )
{

  Environment* ev = somGetGlobalEnvironment();
  ODBoolean handled = kODFalse;
  ODBoolean exception = kODFalse;
  MRESULT rc = MRFROMSHORT(0);

  ODEventData *event = new ODEventData;
  event->hwnd = hwnd;
  event->msg = msg;
  event->mp1 = mp1;
  event->mp2 = mp2;


  try
  {
     // Look for link update message and handle it
     if (msg == kODEvtLinkUpdate)
     {
        ODBaseLink *LinkTarget = (ODBaseLink*) PVOIDFROMMP(mp1);
        ULONG       UpdatedID = LONGFROMMP(mp2);
        if (LinkTarget)
           LinkTarget->UpdateDependents(ev, UpdatedID);
     }

     // Offer all messages to the dispatcher
     handled =  shell->DispatchEvent(ev, event);
  }
  catch (ODException _exception)
  {
     LOG("OpenDoc exception occured in ODDocumentWindowProc.");
     exception = kODTrue;
     // Clear the error
     SetErrorCodeEv(ev, kODNoError);
  }

  if (!exception)
  {
    if (handled)
       rc = event->result;
    else
       // Offer unhandled messages to the default window proc
       rc = WinDefWindowProc( hwnd, msg, mp1, mp2 );
  }

  delete event;
  return (rc);
}


//----------------------------------------------------------------------------
// DraftDialogFunc - Function that drives the Draft History dialog.
//----------------------------------------------------------------------------
SOMEXTERN MRESULT SOMLINK DraftDialogFunc(HWND hdwnd, ULONG message, 
                                 MPARAM mp1, MPARAM mp2)
{
  ODULong       result, draftNum;
  DraftInfoRec* pTraverse;
  DraftWindow*  DWThis = kODNULL;
  char*         entry;
  Environment   *ev = somGetGlobalEnvironment();
  MRESULT       retCode;
  HWND          DraftWnd;
  ODBoolean     dialogDismiss = kODFalse;

  try
  {

    switch (message)
    {
      //------------------------------------------------------
      //  Initialize the drafts listbox
      //------------------------------------------------------
      case WM_INITDLG:
      {
          // Because this function has been declared as a static (not
          // having implicit access to the "this" pointer) we passed
          // "this" in via the mp2.  Need to retrieve it and use
          // it when referring to member data.
          DWThis = (DraftWindow*) mp2;

          // save handle to this object structure passed in as mp2
          WinSetWindowULong(hdwnd, QWL_USER, LONGFROMMP(mp2));

          // Have the document manager interalize all of the draft information.
          DWThis->GetActiveDocumentManager()->InternalizeHistory(ev);
  
          // Get a list of drafts associated with currently active document.
          DWThis->SetDraftInfoList( 
                         DWThis->GetActiveDocumentManager()->GetDraftList(ev));

  
          // If there aren't any drafts, inform user and end dialog.
          if (DWThis->GetDraftInfoList() == kODNULL)
          {
              WinMessageBox(HWND_DESKTOP,DWThis->GetWindow(),
                         catgets(ODcat_handle,DOCSH_SET,DOCMSG_1,
                           "No drafts exist for this document."),
                         catgets(ODcat_handle,DOCSH_SET,DOCMSG_2,
                           "Information"), 0, MB_INFORMATION |
                         MB_OK);
              WinDismissDlg(hdwnd,0);
              dialogDismiss = kODTrue;
              delete DWThis;
              break;
          }

          // Create the container that will list draft history.
          { 
             
            // Get the id to the draft container resource.
            DraftWnd = WinWindowFromID(hdwnd, DID_DRAFTCNTR);

            // Initialize the container
            ODIText       *tmpName, *tmpCmnt;
	    char          modTime[kODDateLength], modDate[kODDateLength]; 
            char          userName[kODMaxLength], comments[kODMaxLength];
            ODULong       maxFields = 5, time;
            DraftInfoRec* pDraftInfo;

            // Set up the container to default to "details" view
            CNRINFO cnrinfo;
            cnrinfo.cb = sizeof(CNRINFO);
            cnrinfo.cFields = maxFields;
            cnrinfo.flWindowAttr = CV_DETAIL | CA_DETAILSVIEWTITLES ;
            WinSendMsg(DraftWnd, CM_SETCNRINFO, &cnrinfo,
                       MPFROMLONG( CMA_FLWINDOWATTR | CMA_CNRTITLE) );

            // Determine how many drafts exist.
            ODUShort numDrafts=0;
            for (pDraftInfo=DWThis->GetDraftInfoList();
                 pDraftInfo != kODNULL ;
                 pDraftInfo = pDraftInfo->GetNext()) numDrafts++;
          
            // allocate the MINIRECORDCORE structure
            PODINFO pStartingRecord, pRecord;
            pRecord = (PODINFO)WinSendMsg(DraftWnd, CM_ALLOCRECORD,
               MPFROMLONG( (LONG)(sizeof(ODINFO) - sizeof(MINIRECORDCORE)) ),
               MPFROMSHORT(numDrafts));
     
            // Save position of first allocated record
            pStartingRecord = pRecord;

            // put information into the recordcores.
            draftNum = 0;
            for (pDraftInfo=DWThis->GetDraftInfoList();
                 pDraftInfo != kODNULL ;
                 pDraftInfo = pDraftInfo->GetNext())
            {
                pRecord->minirecordCore.cb = sizeof(MINIRECORDCORE);
                pRecord->minirecordCore.pszIcon = "";
                pRecord->minirecordCore.hptrIcon = 
                            WinQuerySysPointer(HWND_DESKTOP,SPTR_FILE,FALSE);
      
                // Get the name of last user to modify draft
                if (tmpName = pDraftInfo->GetModUser())
                    GetITextString(tmpName, userName);

                // Get the comments 
                if (tmpCmnt = pDraftInfo->GetComment())
                    GetITextString(tmpCmnt, comments);

                // Get the date and time the draft was last modified.
                if (time = pDraftInfo->GetSavedDate())
                {
                  wchar_t wcsDate [kODDateLength] = L"";
                  wchar_t wcsTime [kODDateLength] = L"";
                  wchar_t wcsDateFormat [kODDateLength * kODWcsLength];
                  wchar_t wcsTimeFormat [kODDateLength * kODWcsLength];

                  // Convert time into readable structure
                  struct tm *timeptr = localtime((const time_t*) &time);

                  // Get correct format to localize date and time.
                  char* dateFormat = nl_langinfo(D_FMT);
                  char* timeFormat = nl_langinfo(T_FMT);

                  // Convert format string to wide char
                  mbstowcs (wcsDateFormat, dateFormat, 
                              kODDateLength * kODWcsLength);
                  mbstowcs (wcsTimeFormat, timeFormat, 
                              kODDateLength * kODWcsLength);

                  // Get date from time string using formats
                  wcsftime(wcsDate,kODDateLength*kODWcsLength,
                              wcsDateFormat,timeptr);
                  wcsftime(wcsTime,kODDateLength*kODWcsLength,
                              wcsTimeFormat,timeptr);

                  // Convert resulting date and time back to multibyte.
                  wcstombs (modDate, wcsDate, kODDateLength * kODWcsLength);
                  wcstombs (modTime, wcsTime, kODDateLength * kODWcsLength);
                }

                // Copy all detail information into container record.
                pRecord->creatorUserName = (char *)malloc(strlen(userName)+1);
                strcpy(pRecord->creatorUserName,userName);
                pRecord->time = (char *)malloc(strlen(modTime)+1);
                strcpy(pRecord->time,modTime);
                pRecord->date = (char *)malloc(strlen(modDate)+1);
                strcpy(pRecord->date,modDate);
                pRecord->draftNumber = pDraftInfo->GetDraftNum();
                pRecord->comments = (char *)malloc(strlen(comments)+1);
                strcpy(pRecord->comments,comments);
          
                // Insert the container records
                RECORDINSERT recordInsert;
                recordInsert.cb = sizeof(RECORDINSERT);
                recordInsert.pRecordParent = NULL;
                recordInsert.pRecordOrder = (PRECORDCORE)CMA_END;
                recordInsert.zOrder = (USHORT)CMA_TOP;
                recordInsert.cRecordsInsert = (USHORT)1;
                recordInsert.fInvalidateRecord = kODTrue;
                WinSendMsg(DraftWnd,CM_INSERTRECORD,
	               (PMINIRECORDCORE)pRecord,&recordInsert);
          
                pRecord = (PODINFO)pRecord->minirecordCore.preccNextRecord;

            } // endfor 

            // Initialize the FIELDINFO data for every column.
            PFIELDINFO pStartingFieldInfo, pFieldInfo;
            pFieldInfo = (PFIELDINFO) WinSendMsg(DraftWnd, 
	                                         CM_ALLOCDETAILFIELDINFO,
                                                 MPFROMLONG(maxFields), NULL);
          
            // Hold onto beginning of field info list.
            pStartingFieldInfo = pFieldInfo;
          
            // Initialize the "Creator" column.
            pFieldInfo->cb = sizeof(FIELDINFO);
            pFieldInfo->flData = CFA_HORZSEPARATOR | CFA_LEFT | 
                                 CFA_SEPARATOR | CFA_STRING;
            pFieldInfo->flTitle = CFA_CENTER | CFA_STRING;
            pFieldInfo->pTitleData = (PVOID)malloc(CCHMAXPATH);
            strcpy((char*)pFieldInfo->pTitleData,
                    catgets(ODcat_handle,DOCSH_SET,DOCMSG_17, "Creator"));
            pFieldInfo->offStruct = (ULONG) &(((ODINFO *)0)->creatorUserName);
            pFieldInfo = pFieldInfo->pNextFieldInfo;
          
            // Initialize the "Draft" column.
            pFieldInfo->cb = sizeof(FIELDINFO);
            pFieldInfo->flData = CFA_HORZSEPARATOR | CFA_CENTER | 
                                 CFA_SEPARATOR | CFA_ULONG;
            pFieldInfo->flTitle = CFA_CENTER;
            pFieldInfo->pTitleData = (PVOID)malloc(CCHMAXPATH);
            strcpy((char*)pFieldInfo->pTitleData,
                    catgets(ODcat_handle,DOCSH_SET,DOCMSG_18, "Draft"));
            pFieldInfo->offStruct = (ULONG) &(((ODINFO *)0)->draftNumber);
            pFieldInfo = pFieldInfo->pNextFieldInfo;
          
            // Initialize the "Date" column.
            pFieldInfo->cb = sizeof(FIELDINFO);
            pFieldInfo->flData = CFA_HORZSEPARATOR | CFA_CENTER | 
                                 CFA_SEPARATOR | CFA_STRING;
            pFieldInfo->flTitle = CFA_CENTER | CFA_STRING;
            pFieldInfo->pTitleData = (PVOID)malloc(CCHMAXPATH);
            strcpy((char*)pFieldInfo->pTitleData,
                   catgets(ODcat_handle,DOCSH_SET,DOCMSG_19, "Date"));
            pFieldInfo->offStruct = (ULONG) &(((ODINFO *)0)->date);
            pFieldInfo = pFieldInfo->pNextFieldInfo;
          
            // Initialize the "Time" column.
            pFieldInfo->cb = sizeof(FIELDINFO);
            pFieldInfo->flData = CFA_HORZSEPARATOR | CFA_CENTER | 
                                 CFA_SEPARATOR | CFA_STRING;
            pFieldInfo->flTitle = CFA_CENTER | CFA_STRING;
            pFieldInfo->pTitleData = (PVOID)malloc(CCHMAXPATH);
            strcpy((char*)pFieldInfo->pTitleData,
                    catgets(ODcat_handle,DOCSH_SET,DOCMSG_20, "Time"));
            pFieldInfo->offStruct = (ULONG) &(((ODINFO *)0)->time);
            pFieldInfo = pFieldInfo->pNextFieldInfo;
          
            // Initialize the "Comments" column.
            pFieldInfo->cb = sizeof(FIELDINFO);
            pFieldInfo->flData = CFA_HORZSEPARATOR | CFA_LEFT | CFA_STRING;
            pFieldInfo->flTitle = CFA_CENTER | CFA_STRING;
            pFieldInfo->pTitleData = (PVOID)malloc(CCHMAXPATH);
            strcpy((char*)pFieldInfo->pTitleData,
                    catgets(ODcat_handle,DOCSH_SET,DOCMSG_21, "Comments"));
            pFieldInfo->offStruct = (ULONG) &(((ODINFO *)0)->comments);
          
            // Insert the FIELDINFO data structures into the container
            FIELDINFOINSERT fieldInfoInsert;
            fieldInfoInsert.cb = sizeof(FIELDINFOINSERT);
            fieldInfoInsert.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST;
            fieldInfoInsert.cFieldInfoInsert = maxFields;
            fieldInfoInsert.fInvalidateFieldInfo = kODTrue;
          
            WinSendMsg(DraftWnd,CM_INSERTDETAILFIELDINFO,
                       MPFROMP(pStartingFieldInfo),
                       MPFROMP(&fieldInfoInsert) );
          
	  } // Done initializing the container

          // Disable the Delete button if document is read-only
          ODDraft* draft = shell->GetActiveDocumentManager(ev)->GetDraft(ev);
          if (draft && !(draft->GetPermissions(ev) >= kODDPSharedWrite))
             WinEnableControl(hdwnd, DID_DELETE, FALSE);

          // Make dialog visible.
          WinSetWindowPos(hdwnd, NULLHANDLE, 0,0,0,0, SWP_ACTIVATE | SWP_SHOW);
          WinSendDlgItemMsg(hdwnd, DID_DRAFTCNTR, LM_SELECTITEM,
                                            MPFROMSHORT(0), MPFROMSHORT(TRUE));
  
          break;
      }
  
      case WM_CLOSE:
      {
         WinDismissDlg(hdwnd,TRUE);
         dialogDismiss = kODTrue;

         // Free the list of history records.
         delete DWThis;
  
         break;
      }

      case WM_SYSCOMMAND:
      {
         if (SHORT1FROMMP(mp1) == SC_CLOSE)
         {
            WinDismissDlg(hdwnd, TRUE);
            dialogDismiss = kODTrue;

            // Free the list of history records.
            delete DWThis;
         }
         else
            return WinDefDlgProc(hdwnd, message, mp1, mp2);
         break;
      }

      case WM_CONTROL:
      {
          DWThis = (DraftWindow*) WinQueryWindowULong(hdwnd, QWL_USER);

          // if user selected draft from list.
          if (SHORT1FROMMP(mp1) == DID_DRAFTCNTR)
          {
              // if user double clicked on a draft listing, open it.
              if (SHORT2FROMMP(mp1) == CN_ENTER)
              {
	          PNOTIFYRECORDENTER pn = (PNOTIFYRECORDENTER) mp2;

                  draftNum = ((PODINFO) (pn->pRecord))->draftNumber;

                  // Open the draft in a new window
                  if (!(DWThis->OpenDraft(draftNum)))
                          WARNMSG(WARN_INDEX(AMSG_890),
                                  "CPPOD0402 - Unable to open draft.");
                  else
                  {
                     WinDismissDlg(hdwnd,0);
                     dialogDismiss = kODTrue;
                     delete DWThis;
                  }
              }
          }
          break;
      }

      case WM_COMMAND:
      {
          DWThis = (DraftWindow*) WinQueryWindowULong(hdwnd, QWL_USER);

          switch (SHORT1FROMMP(mp1))
          {
            //------------------------------------------------
            //  Open selected draft 
            //------------------------------------------------
            case DID_OPEN:
            {
              DraftWnd = WinWindowFromID(hdwnd, DID_DRAFTCNTR);

	      // Find selected entry in container.
	      PODINFO pTmpRecord = 
	         (PODINFO) PVOIDFROMMR(WinSendMsg(DraftWnd, 
		                                  CM_QUERYRECORDEMPHASIS,
						  (PMINIRECORDCORE) CMA_FIRST,
						  MPFROMSHORT(CRA_SELECTED)));


              if (!DWThis->OpenDraft(pTmpRecord->draftNumber))
                 WARNMSG(WARN_INDEX(AMSG_890),
                         "CPPOD0402 - Unable to open draft.");
              else
              {
                 WinDismissDlg(hdwnd,0);
                 dialogDismiss = kODTrue;
                 delete DWThis;
              }
   
              break;
            }
   
            //------------------------------------------------
            //  Delete selected draft from storage.
            //------------------------------------------------
            case DID_DELETE:
            {
              // Display confirmation dialog and let the DeleteDialogProc
	      // take the steps necessary to delete based on users response.  
              result = WinMessageBox(HWND_DESKTOP, hdwnd,
                         catgets(ODcat_handle,DOCSH_SET,DOCMSG_5,
                           "Are you sure you want to delete this draft?"),
                         catgets(ODcat_handle,DOCSH_SET,DOCMSG_6,
                           "Delete Confirmation"), 0, MB_QUERY |
                         MB_OKCANCEL);
       
              if (result == MBID_OK) 
              {
                // Get the position of the selected draft entry.
                DraftWnd = WinWindowFromID(hdwnd, DID_DRAFTCNTR);

	        // Find selected entry in container.
	        PODINFO pTmpRecord = 
	           (PODINFO) PVOIDFROMMR (WinSendMsg(DraftWnd, 
		                                  CM_QUERYRECORDEMPHASIS,
						  (PMINIRECORDCORE) CMA_FIRST,
						  MPFROMSHORT(CRA_SELECTED)));

		// Need to disable the draft history dialog. Otherwise, if
		// a warning message is displayed in DeleteDraft method of
		// document manager, then the user can go to the draft
		// history window to close the draft history dialog causing
		// docshell to trap when returning from docmgr.
		WinEnableWindow(hdwnd, FALSE);

                // Remove the draft from the dialog list and storage.
                if (DWThis->RemoveDraft(pTmpRecord->draftNumber))
                {
                   // Need to redisable all the ODwindows because if
                   // the draft above the deleted draft is opened, then
                   // the window containing the above draft is closed and
                   // opened again to point to the correct draft due to
                   // collapsing draft. By doing so, the new window opened
                   // is not disabled.
 		   EnableApplModal(ev, NULL, FALSE);

                   PMINIRECORDCORE pRecordArray = 
                        &(pTmpRecord->minirecordCore);

                   // Remove entry in the container.
	           WinSendMsg(DraftWnd, CM_REMOVERECORD, (PVOID) &pRecordArray,
			      MPFROM2SHORT(1,CMA_FREE | CMA_INVALIDATE));

                   // Refresh the container
	           //WinSendMsg(DraftWnd, CM_INVALIDATERECORD, MPFROMP(NULL),
	           //MPFROM2SHORT(0,CMA_REPOSITION));

                   // Set the cursor position to the next entry.
                }
                else
                {
                  // notify user of failure
                  WARNMSG(WARN_INDEX(AMSG_900),
                          "CPPOD0403 - Unable to delete draft.");
                }

		// Enable back the draft history dialog
		WinEnableWindow(hdwnd, TRUE);

                // Set draft history dialog to be the active window
                WinSetActiveWindow(HWND_DESKTOP, hdwnd);
  
                // Need to reaquire the list from the document manager in 
                // the event that by deleting the last element on the list, 
                // the head or list pointed to by fDraftInfoList has changed.
                DWThis->SetDraftInfoList(
                         DWThis->GetActiveDocumentManager()->GetDraftList(ev));
   
                // if the draft history info list is now empty, shutdown dialog.
                if (!DWThis->GetDraftInfoList())
                {
                   WinDismissDlg(hdwnd,0);
                   dialogDismiss = kODTrue;

                   // delete the Draft Window object.
                   delete DWThis;
                }
  
              } // result == MBID_OK

              break;

            } // case DID_DELETE
  
            //------------------------------------------------
            //  User is finished with drafts dialog.
            //------------------------------------------------
            case DID_CANCEL:  
              // Need to include this case so Escape shortcut will work
            case DID_DONE:
            { 
              WinDismissDlg(hdwnd,0);
              dialogDismiss = kODTrue;

              // Free the list of history records.
              delete DWThis;
  
              break;
            }
  
            default:
 
              break;

          } // switch (SHORT1FROMMP(mp1))

          break;

      } // case WM_COMMAND

      case WM_HELP:
      {
          ODSession * session = shell->GetSession(ev);
          ODHelp    * helpme  = session->GetHelp(ev);
          helpme->DisplayHelp(ev,ODHELPFILE, IDMS_DOC_DRAFTS_PANEL);
          break;
      }

      default:
 
          // if message isn't dealt with above, let PM take care of it.
          retCode = WinDefDlgProc(hdwnd, message, mp1, mp2);

    } // switch (message)

  }
  catch (ODException _exception)
  {
    LOG("OpenDoc exception occured in DraftDialogFunc.");

    // Free the list of history records.
    if (DWThis)
    {
       SaveEv();
       try
       {
          delete DWThis;
       }
       catch (ODException _exception)
       {
           SetErrorCodeEv(ev, kODNoError);
       }
       RestoreEv();
    }
   
    if (!dialogDismiss)
       WinDismissDlg(hdwnd,0);
    throw;
  }

  return retCode;
}

//------------------------------------------------------------------------------
// EnableApplModal - Simulates application modality by disabling/enabling
//                   all the OD windows except the window which owns the dialog.
//------------------------------------------------------------------------------
SOMEXTERN void SOMLINK EnableApplModal(Environment *ev, HWND hwnd, BOOL enable)
{
    ODWindowIterator *iter = kODNULL;
    ODWindow         *curWindow;

    try
    {
        // Disable/Enable mouse and keyboard input to all the OD windows 
        // except the one which is going to own the display dialog.
	//
	// Note: The window which is going to own the display dialog is
	//       disable/enable by the system.
	ODWindowState *windowState = shell->GetWindowState(ev);
	if (windowState)
	{
		iter = windowState->CreateWindowIterator(ev);
		for (curWindow = iter->First(ev); iter->IsNotComplete(ev); 
		     curWindow = iter->Next(ev))
	        {
	            HWND platformWindow = curWindow->GetPlatformWindow(ev);
	            if (platformWindow != hwnd)
	               WinEnableWindow(platformWindow, enable);
	        }
	        ODDeleteObject(iter);
	}

    }
    catch (ODException _exception)
    {
        LOG("OpenDoc exception occured in EnableApplModal.");
        ODDeleteObject(iter);
        throw;
    }
}

