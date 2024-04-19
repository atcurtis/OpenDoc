/* @(#)Z 1.81 com/src/docshell/AppShell.cpp, odshell, od96os2, odos29646d 96/11/15 15:49:31 (96/11/05 10:03:06) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odshell
//
//   CLASSES: none
//
//   ORIGINS: 27
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

//------------------------------------------------------------------------------
// AppShell.cpp
//
//      This file implements the ApplicationShell class.  This abstract
//      class is a subclass of OpenDocShell and adds the platform-independent
//      docshell application functionality.
//------------------------------------------------------------------------------

#ifndef SOM_Module_appshell_Source
#define SOM_Module_appshell_Source
#endif
#define ApplicationShell_Class_Source

#include "DocShell.h"
#include <ODTypes.h>
#include <ODUtils.h>
#include <PlfmFile.h>
#include <Except.h>
#include <sys/stat.h>
#include <errno.h>
#include <StorUtil.h>
#include <DraftWn.h>
#include <TempObj.h>
#include <StdTypIO.h>
#include <odres.h>

#include <DocMgr.xh>
#include <Draft.xh>
#include <Info.xh>
#include <MenuBar.xh>
#include <ODSessn.xh>
#include <Disptch.xh>
#include <Window.xh>
#include <WinStat.xh>
#include <StorageU.xh>
#include <StdProps.xh>
#include <StdTypes.xh>
#include <Undo.xh>
#include <Frame.xh>

#ifndef SOM_ODHelp_xh
#include <odhelp.xh>
#endif
#ifndef _HLPPANELS_
#include <hlppanls.h>
#endif

#define VARIABLE_MACROS
#include <AppShell.xih>

#define ACTION_LABEL_LENGTH 256

// Shell Dialog Utility Function prototypes
#include "ShellUtl.h"


//------------------------------------------------------------------------------
// Go
//
//      This is where all of the action takes place: the arguments are
//      parsed, the system is initialized, and the events are processed.
//      When we return from here we're done.
//------------------------------------------------------------------------------
SOM_Scope ODSLong  SOMLINK ApplicationShellGo(ApplicationShell *somSelf,
                                Environment *ev, ODSLong argc, char **argv)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell","ApplicationShellGo");

    char		fileName[kODMaxFileNameSize];
    char		kindName[kODMaxFileNameSize];
    DocumentManager     *docMgr;
    ODSLong             result = 0;
    ODBoolean           closeDown = kODFalse;
    ODUShort            actionFlag;

    // Initialize fileName, kindName to null strings
    fileName[0] = NULL;
    kindName[0] = NULL;

    // Initialize _fExecString to default value
    strcpy(_fExecString, "docshell");

    try
    {
        // Initialize the Window System
        somSelf->InitWindowSystem(ev);

        // Initialize the OpenDoc environment
        somSelf->InitOpenDocShell(ev);

        // Parse the command line arguments.
        // NOTE: ProcessArgs must be done after InitWindowSystem and
        //       InitOpenDocShell because it relies on data set in
        //       InitWindowSystem and InitOpenDocShell.
        somSelf->ProcessArgs(ev, argc, argv, fileName, kindName, &actionFlag);

        // Create Menu Bar
        somSelf->CreateMenuBar(ev);

        // Create a DocumentManager
        docMgr = somSelf->CreateDocumentManager(ev);

        // Open a document (create it if necessary) and run it
        if (somSelf->OpenDocument(ev, docMgr, fileName, kindName, actionFlag))
            result = somSelf->MainLoop(ev); // Start the main loop
    }
    catch (ODException _exception)
    {
        result = -1;

        // Clear the exception
        SetErrorCodeEv(ev, kODNoError);
    }

    try
    {
       // Close up shop
       somSelf->Terminate(ev);
    }
    catch (ODException _exception)
    {
       // Clear the exception
        SetErrorCodeEv(ev, kODNoError);
    }

    return result;
}

//------------------------------------------------------------------------------
// ProcessArgs
//
//      Process command line arguments
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellProcessArgs(
                                ApplicationShell *somSelf, Environment *ev,
                                ODSLong argc, char **argv, char *fileName,
                                char *partKind, ODUShort *actionFlag)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellProcessArgs");

    int  	i;
    ODBoolean	implicit_dash_f = kODFalse;

    try
    {
       // Save arg 0 so we have the command to exec for New and Open
       if (argv != NULL)
          strcpy (_fExecString, argv[0]);

       // Initialize flag to kODNULL
       *actionFlag = kODNULL;

       // Save argc and argv
       somSelf->SetArgc(ev, argc);
       somSelf->SetArgv(ev, argv);

       // Allow "docshell filename" - backwards compatibility for Merlin
       // - look for argc of 2 and the second arg must not be an option
#ifdef _PLATFORM_WIN32_    // allow for "forward slash" options from OLE
       if ((argc == 2) && ((argv[1][0] != '-') || (argv[1][0] != '/')))
#else
       if ((argc == 2) && (argv[1][0] != '-'))
#endif
           implicit_dash_f = kODTrue;

       // Look through the args and set appropriate options
       for (i = 1; i < argc; i++)
       {
          if ((strcmp("-f", argv[i]) == 0) || (strcmp("-s", argv[i]) == 0)
							|| implicit_dash_f)
          {
              if (implicit_dash_f)
                  strcpy(fileName, argv[i]);
              else
              {
                  if (strcmp("-s", argv[i]) == 0)
                      // Set flag to edit stationery
                      *actionFlag = kODEditStationery;

                  // If there is another argument and it is not another option
                  if ((argc > i+1) && (*argv[i+1] != '-'))
                    strcpy(fileName, argv[++i]);
              }

              if (!(somSelf->IsValidFileName(ev, fileName)))
              {
                  WindowSystemData* wsData;

                  wsData = somSelf->GetWindowSystemData(ev);

                  // Null out filename so we know if we returned because user
                  // cancelled dialog.
                  fileName[0] = '\0';

                  // Clear the flag
                  *actionFlag = kODNULL;

                  // Display the Open File Dialog to get filename.
                  if (wsData) OpenFileDlg(wsData->parentWindow, fileName);
              }
          }
          else if (strcmp("-k", argv[i]) == 0)
          {
              if ((argc > i+1) && (*argv[i+1] != '-'))
                  strcpy(partKind, argv[++i]);
          }
          else if (strcmp("-c", argv[i]) == 0)
          {
              *actionFlag = kODCreateStationery;

              if ((argc > i+1) && (*argv[i+1] != '-'))
                  strcpy(partKind, argv[++i]);
          }
       }
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::ProcessArgs");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// OpenDocument
//
//      Open a document, creating it if necessary
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ApplicationShellOpenDocument(
                              ApplicationShell *somSelf, Environment *ev,
                              DocumentManager *docMgr, char *fileName,
                              char *partKind, ODUShort actionFlag)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellOpenDocument");

    ODBoolean   result  = kODTrue;
    ODULong     selInfo = kODKindName;
    struct stat statbuf;

    try
    {
        // If neither a filename or part kind name were given, display the
        // part kind selection dialog
        if ((fileName[0] == NULL) && (partKind[0] == NULL))
        {
            char selText[kODMaxFileNameSize];
            selInfo = SelectKindDlg(ev, selText);

            if (selInfo == kODFileName)
                strcpy(fileName, selText);
            else if (selInfo == kODKindName)
                strcpy(partKind, selText);
            else if (selInfo == kODNoRootPart)
            {
                OpenFileDlg(somSelf->GetWindowSystemData(ev)->parentWindow,
                                                                    fileName);
                if (fileName[0] == '\0')
                {
                    result = kODFalse;
                    selInfo = kODCommandCancel;
                }
            }
        }

        // If SelectKindDialog was presented and the user didn't select Cancel
        if (selInfo != kODCommandCancel)
        {
            // If a file name was specified ...
            if (fileName[0] != '\0')
                // Open the document and set partKind with the kind of
                // part in file.
                strcpy(partKind, docMgr->OpenDocumentFromFile(ev, fileName,
                                                             actionFlag));
            else
            {
                // Create a document with the given root part kind.
                // We don't care at this point if the partKind is valid.
                // Validity will be checked at a lower level.

                // Check if creating document as stationery or creating a
                // regular document
                if (actionFlag == kODCreateStationery)
                {
                    // return kODFalse so as not to enter mainloop for regular
                    // document processing
                    result = kODFalse;
                    docMgr->CreateDocumentAsStationery(ev, partKind,
                                                                 "NewDoc.od");
                }
                else
                   docMgr->CreateDocumentInFile(ev, partKind, "NewDoc.od");
            }
        }
        else // Otherwise the user must have cancelled dialog.
            result = kODFalse;

    }
    catch (ODException _exception)
    {
        result = kODFalse;
	SetErrorCodeEv(ev, kODNoError); // Clear the exception
    }

    return result;
}

//------------------------------------------------------------------------------
// DispatchEvent
//
//	Forward the event the the dispatcher but look for and act upon a
//	few events that are of interest to us.
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ApplicationShellDispatchEvent(
                                                ApplicationShell *somSelf,
                                                Environment *ev,
                                                ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellDispatchEvent");

    ODBoolean handled = kODFalse;

    try
    {
        ODDispatcher* dispatcher = somSelf->GetDispatcher(ev);

        switch (somSelf->GetEventType(ev, event))
        {
            case  kODEvtInitMenu:
            {
                // Give the parts a chance to adjust their menus before the
                //    message is sent to the dispatcher.
                somSelf->AdjustMenu(ev);
	        handled = dispatcher->Dispatch(ev, event);
                break;
            }

            case kODEvtActivate:
            {
                TempODWindow odWindow = somSelf->GetWindowState(ev)->
                        AcquireODWindow(ev, somSelf->GetEventWindow(ev, event));
                if ((somSelf->GetEventSubType(ev, event) != kODEvtInactive)
                            && odWindow)
                {
                    somSelf->UpdateActiveDocumentManager(ev, odWindow);
                    odWindow->Select(ev);
                }

                handled = dispatcher->Dispatch(ev, event);
                break;
            }

            case kODEvtCommand:
            {
                if (!(handled = somSelf->HandleMenuEvent(ev, event)))
                    handled = dispatcher->Dispatch(ev, event);
                break;
            }

            case OD_HELP:
            {
                handled = dispatcher->Dispatch(ev, event);  // Let Part override

                if (!handled)
                {
                  ODUShort context = somSelf->GetEventSubType(ev, event);
                  if (context == HLPM_ODMENU)   // If F1 on Menu Item
                  {
                    handled = somSelf->HandleHelpEvent(ev, event);  // Handle base menu
                    if (!handled) // if no help found for menu
                    {
                      ODHelp *helpMe = somSelf->GetSession(ev)->GetHelp(ev);
                      helpMe->DisplayHelp(ev, ODHELPFILE, IDMS_NO_HELP_PROVIDED_PANEL);
                    }
                  }
                  else if (context == HLPM_ODWINDOW)  // If F1 on window or part
                  {
                    ODHelp *helpMe = somSelf->GetSession(ev)->GetHelp(ev);
                    helpMe->DisplayHelp(ev, ODHELPFILE, OPENDOC_HELP_PANEL);
                  }
                }
                break;
            }

            default:
            {
                if (!(handled = ApplicationShell_parents_DispatchEvent(
                                                           somSelf,ev, event)))
                {
                    switch (somSelf->GetEventType(ev, event))
                    {
                        case kODEvtClose:
                            handled = somSelf->HandleCloseEvent(ev, event);
                            break;
                    }
                }
            }
        }
    }

    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DispatchEvent");
	ODSetSOMException(ev, _exception);
    }

    return handled;

}

//------------------------------------------------------------------------------
// CreateMenuBar
//
//      Create the default base menubar for the docshell.  A subclass must
//      provide an implementation of this platform-dependent method.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellCreateMenuBar(
                                    ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellCreateMenuBar");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::CreateMenuBar");
}

//------------------------------------------------------------------------------
// InitWindowSystem
//
//      Initialize the platform window system.  A subclass must provide an
//      implementation of this platform-dependent method.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellInitWindowSystem(
                                    ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                         "ApplicationShellInitWindowSystem");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::InitWindowSystem");
}

//------------------------------------------------------------------------------
// Exec
//
//      Spawn another docshell process.  A subclass must provide an
//      implementation of this platform-dependent method.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellExec(ApplicationShell *somSelf,
                                             Environment *ev,
                                             char* str)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell","ApplicationShellExec");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::Exec");
}

//------------------------------------------------------------------------------
// MainLoop
//
//      Provide the event processing loop.  A subclass must provide an
//      implementation of this platform-dependent method.
//------------------------------------------------------------------------------
SOM_Scope ODSLong  SOMLINK ApplicationShellMainLoop(ApplicationShell *somSelf,
                                                  Environment *ev)
{
    ///* ApplicationShellData *somThis = ApplicationShellGetData(somSelf); */
    ApplicationShellMethodDebug("ApplicationShell","ApplicationShellMainLoop");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::MainLoop");
    return 0;
}

//------------------------------------------------------------------------------
// HandleMenuEvent
//
//      Handle incoming events from menubar activity.
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ApplicationShellHandleMenuEvent(
                                                  ApplicationShell *somSelf,
                                                  Environment *ev,
                                                  ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                            "ApplicationShellHandleMenuEvent");

    ODBoolean handled = kODTrue;

    try
    {
        if (somSelf->GetEventType(ev,event) == kODEvtCommand)
        {
            switch (somSelf->GetEventSubType(ev,event))
            {
                case kODCommandNew:
                    somSelf->DocumentNew(ev);
                    break;

                case kODCommandOpenDocument:
                    somSelf->DocumentOpen(ev);
                    break;

                case kODCommandSave:
                    somSelf->DocumentSave(ev);
                    break;

                case kODCommandSaveACopy:
                    somSelf->DocumentSaveAs(ev);
                    break;

                case kODCommandRevert:
                    somSelf->DocumentRevert(ev);
                    break;

                case kODCommandDraftCreate:
                    somSelf->DocumentDraftCreate(ev);
                    break;

                case kODCommandDraftHist:
                    somSelf->DocumentDraftHistory(ev);
                    break;

                case kODCommandDocumentInfo:
                    somSelf->DocumentInfo(ev);
                    break;

                case kODCommandPageSetup:
                    somSelf->DocumentPageSetup(ev, event);
                    break;

                case kODCommandPrint:
                    somSelf->DocumentPrint(ev, event);
                    break;

                case kODCommandClose:
                    somSelf->DocumentClose(ev);
                    break;

                case kODCommandHelpIndex:
                    {
                        ODHelp *helpMe = somSelf->GetSession(ev)->GetHelp(ev);
                        helpMe->DisplayHelpIndex(ev,ODHELPFILE);
                    }
                    break;

                case kODCommandHelpGeneral:
                    {
                        ODHelp *helpMe = somSelf->GetSession(ev)->GetHelp(ev);
                        helpMe->DisplayHelp(ev,ODHELPFILE,OPENDOC_HELP_PANEL);
                    }
                    break;

                 case kODCommandHelpUsing:
                    {
                        ODHelp *helpMe = somSelf->GetSession(ev)->GetHelp(ev);
                        helpMe->DisplayHelpUsingHelp(ev,ODHELPFILE);
                    }
                    break;

                case kODCommandHelpInfo:
                    somSelf->DocumentAbout(ev);
                    break;

                case kODCommandUndo:
                    somSelf->GetSession(ev)->GetUndo(ev)->Undo(ev);
                    break;

                case kODCommandRedo:
                    somSelf->GetSession(ev)->GetUndo(ev)->Redo(ev);
                    break;

                default:
                    handled = kODFalse;
                    break;
            } // end switch
        }
        else
            handled = kODFalse;

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::HandleMenuEvent");
        ODSetSOMException(ev, _exception);
    }

    return handled;
}

//------------------------------------------------------------------------------
// HandleCloseEvent
//
//      Handle incoming events from menubar activity
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ApplicationShellHandleCloseEvent(
                                                  ApplicationShell *somSelf,
                                                  Environment *ev,
                                                  ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                            "ApplicationShellHandleCloseEvent");

    DocumentManager     *docMgr;
    ODULong             changes = kODCommandCancel;
    ODBoolean           handled = kODTrue;

    try
    {

      docMgr = somSelf->GetActiveDocumentManager(ev);

      // Find out if window requesting to be closed is the Root Window. If not
      // a root window, docMgr->CloseWindow will take care of closing it down.
      ODULong isRootWindow = docMgr->CloseWindow(ev,
                             somSelf->GetEventWindow(ev, event), kODTrue);

      // If it is a root window, we will need to do some extra work before
      // closing the window.
      if (isRootWindow)
      {
          changes = somSelf->GetEventSubType(ev,event);

          // if event not requesting to suppress close confirmation dialog
          // pop it up.
          if ((changes != kODCommandSaveChanges) &&
              (changes != kODCommandDiscardChanges))
          {
              // If draft is not dirty, then bring up the dialog to confirm
              // exiting of docshell. If draft is marked dirty, then bring
              // up the dialog to confirm save changes or not.
              ODDraft *draft = docMgr->GetDraft(ev);
              if (draft && draft->ChangedFromPrev(ev) &&
                  (draft->GetPermissions(ev) >= kODDPSharedWrite))
                 changes = ConfirmCloseDlg(ev);
              else
                 changes = ConfirmExitDlg(ev);
          }

          // if user didn't ask to cancel the close, proceed.
          if (changes != kODCommandCancel)
          {
              // Close the document, passing in type of close (whether
              // to save changes or not) chosen in Close dialog.
              docMgr->CloseDocument(ev, changes);

              // Delete the document manager
              somSelf->DeleteDocumentManager(ev, docMgr);
              somSelf->SetActiveDocumentManager(ev, (DocumentManager*) kODNULL);

              // do any required clean for close to complete.
              somSelf->CloseCleanup(ev);

              // Set handled to false, so ODDispatchEvent will close down.
              handled = kODFalse;
            }
        }
    }
    catch (ODException _exception)
    {
        // Clear the exception
        SetErrorCodeEv(ev, kODNoError);

	WARNMSG(WARN_INDEX(AMSG_570), "Docshell: Error %d returned during closing down of the document.\\nDocument may not save correctly.", ErrorCode());

        if (changes != kODCommandCancel)
        {
            try
            {
               somSelf->CloseCleanup(ev);
            }
            catch (ODException _exception)
            {
               // Clear the exception
               SetErrorCodeEv(ev, kODNoError);
            }
        }
    }

    return handled;
}

//------------------------------------------------------------------------------
// HandleHelpEvent
//
//	Handle incoming help events
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ApplicationShellHandleHelpEvent(
                                                  ApplicationShell *somSelf,
                                                  Environment *ev,
                                                  ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                            "ApplicationShellHandleHelpEvent");
    int		menuID;
    ODULong	helpID;
    ODBoolean	handled = kODTrue;
    ODHelp	*helpMe;

    #ifdef _PLATFORM_OS2_
    menuID      = SHORT1FROMMP(event->mp2);
    #endif //_PLATFORM_OS2_
    #ifdef _PLATFORM_WIN32_
    menuID      = event->lParam;
    #endif //_PLATFORM_WIN32_
    #ifdef _PLATFORM_AIX_
    menuID = 0;     // Handle Help Event is NOP on AIX, menu widget has callback
    #endif //_PLATFORM_AIX_

    try
    {
        helpMe = somSelf->GetSession(ev)->GetHelp(ev);
        helpID = helpMe->GetHelpID(ev, menuID);
        if (helpID)
            helpMe->DisplayHelp(ev, ODHELPFILE, helpID);
        else
            handled = kODFalse;
    }

    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::HandleHelpEvent");
        ODSetSOMException(ev, _exception);
    }

    return handled;
}

//------------------------------------------------------------------------------
// DocumentNew
//
//      Exec a new docshell with the same root part kind as the current
//      document.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentNew(ApplicationShell *somSelf,
                                                    Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                                "ApplicationShellDocumentNew");

    DocumentManager	*docMgr;
    char		str[kODMaxFileNameSize];

    try
    {
        docMgr = somSelf->GetActiveDocumentManager(ev);
        sprintf(str, "%s -k %s", _fExecString, docMgr->GetPartKindName(ev));
        somSelf->Exec(ev, str);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentNew");
        ODSetSOMException(ev, _exception);
    }
}


//------------------------------------------------------------------------------
// DocumentOpen
//
//      Get a document name from the user and exec a new docshell for that
//      document.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentOpen(ApplicationShell *somSelf,
                                                     Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                             "ApplicationShellDocumentOpen");

    char 		docName[kODMaxFileNameSize] = "";
    char 		str[kODMaxFileNameSize] = "";

    try
    {
        ODWindowState* WinStat = somSelf->GetWindowState(ev);

        // Use TempODWindow so this object won't have to be released.
        TempODWindow Window = WinStat->AcquireActiveWindow(ev);

        // Display the Open File Dialog to get filename.
        OpenFileDlg(Window->GetPlatformWindow(ev), docName);

        if (strcmp(docName,"\0") != 0)
        {
            // launch another copy of the shell to open the passed in file name
            sprintf(str, "%s -f \"%s\"", _fExecString, docName);
            somSelf->Exec(ev, str);
        }
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentOpen");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// DocumentSave
//
//      Save the document
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentSave(ApplicationShell *somSelf,
                                                     Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                               "ApplicationShellDocumentSave");

    try
    {
        somSelf->GetActiveDocumentManager(ev)->SaveDocument(ev);
    }
    catch (ODException _exception)
    {
        WARNMSG(WARN_INDEX(AMSG_912), "CPPOD0459 - Docshell: Error %d returned when saving document.\\nDocument may not save correctly.", ErrorCode());
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// DocumentSaveAs
//
//      Save a copy of this document in a new file specified by the user
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentSaveAs(
                                   ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                             "ApplicationShellDocumentSaveAs");

    PlatformFile *file = kODNULL;

    try
    {
       	ODWindowState *winStat = somSelf->GetWindowState(ev);
       	TempODWindow odWindow = winStat->AcquireActiveWindow(ev);

        file = SaveFileDlg(ev);
        if (file)
	{
	    ODDraft *draft = odWindow->GetDraft(ev);
            if (draft)
                somSelf->GetActiveDocumentManager(ev)->
                                                  SaveACopyAs(ev, draft, file);
	}
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentSaveAs");
        ODSetSOMException(ev, _exception);
    }

    delete file;
}

//------------------------------------------------------------------------------
// DocumentRevert
//
//      Discard all of the changes to this document since the last save
//      was performed, but confirm this with the user first.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentRevert(
                                    ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                            "ApplicationShellDocumentRevert");

    try
    {
        if (ConfirmRevertDlg(ev))
            somSelf->GetActiveDocumentManager(ev)->Revert(ev);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentRevert");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// DocumentDraftCreate
//
//      Display the Create Draft dialog
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentDraftCreate(
                                    ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                      "ApplicationShellDocumentDraftCreate");


    ODDraft*       draft = kODNULL;
    ODStorageUnit* su = kODNULL;
    ODULong        draftnum = 0;

    try
    {
        draft = somSelf->GetActiveDocumentManager(ev)->
                                                GetLatestCreatedDraft(ev);

        if (draft)
        {
            // Acquire the storage unit containing the draft properties.
            su = draft->AcquireDraftProperties(ev);

            // Retrieve draft number
            draftnum = ODGetULongProp(ev, su, kODPropDraftNumber, kODULong);

            // Release the storage unit
            ODReleaseObject(ev,su);
        }

        // Release the draft
        ODReleaseObject(ev,draft);

        CreateDraftDlg(ev,draftnum);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentDraftCreate");
        ODError error = ErrorCode();
        if (draft || su)
           SaveAndRestoreEv2(draft, su);
        ODSetSOMException(ev, error);
    }
}

//------------------------------------------------------------------------------
// DocumentDraftHistory
//
//      Display the Draft History dialog.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentDraftHistory(
                                   ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                      "ApplicationShellDocumentDraftHistory");

    ODWindowState* winStat;
    ODPlatformWindow window;
    DraftWindow* dw = kODNULL;

    try
    {
        winStat = somSelf->GetWindowState(ev);
        TempODWindow odWindow = winStat->AcquireActiveWindow(ev);
        window = odWindow->GetPlatformWindow(ev);

        dw = new DraftWindow(somSelf->GetActiveDocumentManager(ev),
                             somSelf->GetWindowSystemData(ev), window);

        dw->DraftHistory();

        // The dw object will be deleted within the draft history dialog
        // function when the dialog is brought down.
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentDraftHistory");
        ODError error = ErrorCode();
        SaveEv();
        if (dw)
            delete dw;
        RestoreEv();
        ODSetSOMException(ev, error);
    }
}

//------------------------------------------------------------------------------
// DocumentInfo
//
//      Display the Document Info dialog.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentInfo(ApplicationShell *somSelf,
                                                     Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                              "ApplicationShellDocumentInfo");

    ODWindowState* winState;
    ODSession* session;
    ODInfo* info;

    try
    {
        winState = somSelf->GetWindowState(ev);
        TempODWindow odWindow = winState->AcquireActiveWindow(ev);
        session = somSelf->GetSession(ev);
        info = session->GetInfo(ev);

        info->ShowPartFrameInfo(ev, odWindow->GetRootFacet(ev),
                somSelf->GetActiveDocumentManager(ev)->HasWriteAccess(ev));
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentInfo");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// DocumentPageSetup
//
//      Send the PageSetup menu event to the root part
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentPageSetup(
                                    ApplicationShell *somSelf, Environment *ev,
                                    ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                         "ApplicationShellDocumentPageSetup");

    try
    {
       somSelf->SendEventToRootPart(ev, event);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentPageSetup");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// DocumentPrint
//
//      Send the Print menu event to the root part
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentPrint(
                                   ApplicationShell *somSelf, Environment *ev,
                                   ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                            "ApplicationShellDocumentPrint");

    try
    {
       somSelf->SendEventToRootPart(ev, event);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::DocumentPrint");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// DocumentClose
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentClose(
                                   ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                             "ApplicationShellDocumentClose");

    //  This method gets overridden by the platform shell class because it
    //  needs to make platform dependent calls.
    somSelf->SubClassResponsibility(ev, "ApplicationShell::DocumentClose");
}

//------------------------------------------------------------------------------
// DocumentAbout
//
//      Display the OpenDoc Information dialog
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellDocumentAbout(
                                   ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                             "ApplicationShellDocumentAbout");

    // For the time being just display a message box
    AboutDlg(ev);
}

//------------------------------------------------------------------------------
// AdjustMenu
//
//      Update our menus items for the present context and then tell the
//      parts to do the same.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellAdjustMenu(
                                   ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell","AdjustMenu");

    ODWindowState* winState;
    ODDraft* draft;

    try
    {
        winState = somSelf->GetWindowState(ev);
        TempODMenuBar currentMenuBar = winState->AcquireCurrentMenuBar(ev);
        draft = somSelf->GetActiveDocumentManager(ev)->GetDraft(ev);

        if (currentMenuBar)
        {
            ODBoolean unsavedChanges = kODFalse;

            // if draft has write access, then check if draft is marked
            // dirty or not so as to enable/disable Save and Revert menu
            // items accordingly. Otherwise disable Save and Revert menu
            // items.
            if (draft && draft->GetPermissions(ev) >= kODDPSharedWrite)
                unsavedChanges = draft->ChangedFromPrev(ev);

            currentMenuBar->EnableMenuItem(ev, kODCommandDocumentMenu,
                                        kODCommandRevert, unsavedChanges);
            currentMenuBar->EnableMenuItem(ev, kODCommandDocumentMenu,
                                        kODCommandSave, unsavedChanges);
            currentMenuBar->EnableMenuItem(ev, kODCommandViewMenu,
                                        kODCommandViewAsWin, kODFalse);
            currentMenuBar->EnableMenuItem(ev, kODCommandViewMenu,
                                        kODCommandViewProperties, kODFalse);
            currentMenuBar->EnableMenuItem(ev, kODCommandViewMenu,
                                        kODCommandViewShowAs, kODFalse);

            // Some platforms automatically disable submenus on creation.
            // Therefore, we enable Drafts to ensure it can be used.
            currentMenuBar->EnableMenu(ev, kODCommandDraftsMenu, kODTrue);

            if (draft && !(draft->GetPermissions(ev) >= kODDPSharedWrite))
                currentMenuBar->EnableMenuItem(ev, kODCommandDraftsMenu,
                                        kODCommandDraftCreate, kODFalse);

            somSelf->UpdateUndoMenus(ev);

            // Let the parts have a last crack at the menus.
            winState->AdjustPartMenus(ev);
        }
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::AdjustMenu");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// UpdateUndoMenus
//
//      Enable or disable the Undo and Redo menu items as appropriate.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellUpdateUndoMenus(
                                    ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell","UpdateUndoMenus");

    ODName    *actionLabel = kODNULL;
    char      *str = kODNULL, *tmpstr = kODNULL;
    char      *accel = "";

    try
    {

        ODSession       *Session = somSelf->GetSession(ev);
        ODUndo          *undo = Session->GetUndo( ev );
        ODPart          *part;
        ODActionData    actionData;
        ODActionType    actionType;

        ODWindowState*  WinState = somSelf->GetWindowState(ev);
        TempODMenuBar   currentMenu = WinState->AcquireCurrentMenuBar( ev );

        actionLabel = CreateIText(ACTION_LABEL_LENGTH);
        actionData._buffer = kODNULL;

        // Get string for accelerator 
#if (defined  _PLATFORM_WIN32_) 
        accel = catgets(ODcat_handle,DOCSH_SET,DOCMSG_96,"\tCtrl+Z");
#elif (defined _PLATFORM_OS2_)
        accel = catgets(ODcat_handle,DOCSH_SET,DOCMSG_94,"\tAlt+Backspace");
#endif
        if (undo->PeekUndoHistory(ev, &part, &actionData, &actionType,
                                                                  actionLabel))
        {
            currentMenu->EnableMenuItem(ev, kODCommandEditMenu, kODCommandUndo,
                                                                      kODTrue);
            str = GetITextString(actionLabel, (char*)kODNULL);
            tmpstr = new char [strlen(str) + strlen(accel) + 1];
            strcpy (tmpstr, str); strcat (tmpstr, accel);
            currentMenu->SetMenuItemText(ev, kODCommandEditMenu,
                                                       kODCommandUndo, tmpstr);
            if (str)
            {
               delete[] str;
               str = kODNULL;
            }

        }  
        else
        {
            currentMenu->EnableMenuItem(ev, kODCommandEditMenu, kODCommandUndo,
                                                                kODFalse);
	    char *undoStr = catgets(ODcat_handle, DOCSH_SET, DOCMSG_43, "Undo");
            tmpstr = new char [strlen(undoStr) + strlen(accel) + 1];
            strcpy (tmpstr, undoStr); strcat (tmpstr, accel);
            currentMenu->SetMenuItemText(ev, kODCommandEditMenu,
                                                       kODCommandUndo, tmpstr);
        }
        delete [] tmpstr;
        tmpstr = kODNULL;

        // Get string for accelerator 
#if (defined  _PLATFORM_WIN32_) 
        accel=catgets(ODcat_handle,DOCSH_SET,DOCMSG_97,"\tCtrl+Y");
#elif (defined _PLATFORM_OS2_)
        accel=catgets(ODcat_handle,DOCSH_SET,DOCMSG_95,"\tShift+Alt+Backspace");
#endif
        if (undo->PeekRedoHistory(ev, &part, &actionData, &actionType,
                                                                  actionLabel))
        {
            currentMenu->EnableMenuItem(ev, kODCommandEditMenu, kODCommandRedo,
                                                                kODTrue);
            str = GetITextString(actionLabel, (char*)kODNULL);
            tmpstr = new char [strlen(str) + strlen(accel) + 1];
            strcpy (tmpstr, str); strcat (tmpstr, accel);
            currentMenu->SetMenuItemText(ev, kODCommandEditMenu,
                                                       kODCommandRedo, tmpstr);
            if (str)
            {
               delete[] str;
               str = kODNULL;
            }
        }
        else
        {
            currentMenu->EnableMenuItem(ev, kODCommandEditMenu, kODCommandRedo,
                                                                kODFalse);
	    char *redoStr = catgets(ODcat_handle, DOCSH_SET, DOCMSG_44, "Redo");
            tmpstr = new char [strlen(redoStr) + strlen(accel) + 1];
            strcpy (tmpstr, redoStr); strcat (tmpstr, accel);
            currentMenu->SetMenuItemText(ev, kODCommandEditMenu, 
                                                       kODCommandRedo, tmpstr);
        }

        delete [] tmpstr;
        tmpstr = kODNULL;
        DisposeIText( actionLabel );
        actionLabel = kODNULL;

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::UpdateUndoMenus");
        if (str)
           delete[] str;
        if (tmpstr)
           delete [] tmpstr;
        if (actionLabel)
           DisposeIText(actionLabel);

        ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// SendEventToRootPart
//
//      Send the event directly to the root part
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellSendEventToRootPart(
                                    ApplicationShell *somSelf, Environment *ev,
                                    ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                        "ApplicationShellSendEventToRootPart");

    try
    {
        TempODWindow window =
                somSelf->GetWindowState(ev)->AcquireActiveWindow(ev);
        ODFrame *rootFrame = window->GetRootFrame(ev);
        TempODPart rootPart = rootFrame->AcquirePart(ev);
        rootPart->HandleEvent(ev, event, rootFrame, kODNULL, kODNULL);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in ApplicationShell::SendEventToRootPart");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CloseCleanup
//
//      Perform cleanup tasks not done in other methods during the close
//	process.  A subclass must provide an implementation of this
//      platform-dependent method.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellCloseCleanup(
                                    ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellCloseCleanup");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::CloseCleanup");
}

//------------------------------------------------------------------------------
// SetExecString
//
//      Accessor method for _fExecString
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ApplicationShellSetExecString(
                                    ApplicationShell *somSelf, Environment *ev,
                                    char *execString)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellSetExecString");

    strcpy(_fExecString, execString);
}
//------------------------------------------------------------------------------
// GetExecString
//
//      Accessor method for _fExecString
//------------------------------------------------------------------------------
SOM_Scope char*  SOMLINK ApplicationShellGetExecString(
                                    ApplicationShell *somSelf, Environment *ev)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellGetExecString");

    return _fExecString;
}

//------------------------------------------------------------------------------
// IsValidFileName -
//
// 	This function checks whether the filename is valid or not
//
//------------------------------------------------------------------------------
SOM_Scope ODBoolean SOMLINK ApplicationShellIsValidFileName(
                                    ApplicationShell *somSelf,
                                    Environment *ev,
                                    char *fileName)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellIsValidFileName");

    struct stat       statbuf;
    WindowSystemData* wsData;


    if (fileName[0] != '\0')
    {
#ifdef _PLATFORM_UNIX_
        if (stat(fileName, &statbuf) != 0)
#else
        if (_stat(fileName, &statbuf) != 0)
#endif
        {
            // something is wrong with the file.
            if (errno == ENOENT)
                WARNMSG(WARN_INDEX(AMSG_780), "CPPOD0328 - Docshell: You specified a file that does not exist.");
            else WARNMSG(WARN_INDEX(AMSG_840), "CPPOD0334 - Docshell: Problem opening file. Error %d returned.", errno);
        }
        else // file exists so test if it is a regular file and readable
        {
            if ((statbuf.st_mode & S_IFREG) && (statbuf.st_mode & S_IREAD))
              return kODTrue;
            else
              WARNMSG(WARN_INDEX(AMSG_830), "CPPOD0333 - Docshell: File is not readable or not a regular file.");
        }
    }
    return kODFalse;
}

//------------------------------------------------------------------------------
// GetEventType
//
//	A subclass must provide a platform-dependent implementation of this
//	method which returns the event type from the event data structure.
//------------------------------------------------------------------------------
SOM_Scope ODULong  SOMLINK ApplicationShellGetEventType(
                                                     ApplicationShell *somSelf,
                                                     Environment *ev,
                                                     ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellGetEventType");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::GetEventType");
    return 0;

}

//------------------------------------------------------------------------------
// GetEventSubType
//
//	A subclass must provide a platform-dependent implementation of this
//	method which returns the primary parameter from the event data structure
//------------------------------------------------------------------------------
SOM_Scope ODULong  SOMLINK ApplicationShellGetEventSubType(
                                                     ApplicationShell *somSelf,
                                                     Environment *ev,
                                                     ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellGetEventSubType");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::GetEventSubType");
    return 0;

}

//------------------------------------------------------------------------------
// GetEventWindow
//
//	A subclass must provide a platform-dependent implementation of this
//	method which returns the window from the event data structure.
//------------------------------------------------------------------------------
SOM_Scope ODPlatformWindow  SOMLINK ApplicationShellGetEventWindow(
						ApplicationShell *somSelf,
                                                Environment *ev,
                                                ODEventData* event)
{
    ApplicationShellData *somThis = ApplicationShellGetData(somSelf);
    ApplicationShellMethodDebug("ApplicationShell",
                                "ApplicationShellGetEventWindow");

    somSelf->SubClassResponsibility(ev, "ApplicationShell::GetEventWindow");
    return 0;

}
