/* @(#)Z 1.18 os2/src/docshell/OS2Shell.cpp, odshell, od96os2, odos29647a 96/11/18 05:18:54 (96/11/15 11:34:33) */
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

//------------------------------------------------------------------------------
// This file contains the implementation of OS/2 specific OpenDocShell and 
// ApplicationShell methods.
//------------------------------------------------------------------------------

#ifndef SOM_Module_OS2Shell_Source
#define SOM_Module_OS2Shell_Source
#endif
#define OS2Shell_Class_Source

#include <ODos2.h>

#include <AltPoint.h>
#include <ODSessn.xh>
#include <Shape.xh>
#include <PlfmFile.h>
#include <WinStat.xh>
#include <ODRes.h>
#include <TempObj.h>
#include <draftwn.h>
#include <odres.h>
#include <CmdDefs.xh>
#include <WinStat.xh>
#include <MenuBar.xh>
#include <Disptch.xh>
#include <WinIter.xh>

#define VARIABLE_MACROS
#include "os2shell.xih"

/* Shell Dialog Utility Function prototypes */
#include <ShellUtl.h>

#define OD_RESOURCE_DLL "ODRES.DLL"

//------------------------------------------------------------------------------
// CreateMenuBar
//
//	Creates the default OpenDoc menu bar and installs it in the
//	WindowState object.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OS2ShellCreateMenuBar(OS2Shell *somSelf,  
                                               Environment *ev)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellCreateMenuBar");

    try
    {
        WindowSystemData *windowSystemData = somSelf->GetWindowSystemData(ev);

	// Load the menu and accelerator table
        ODPlatformMenuBar menu = WinLoadMenu(HWND_OBJECT, 
					windowSystemData->resInstance,
					ID_BASEMENUBAR);

	ODPlatformAccelTable accelTable = WinLoadAccelTable(
						windowSystemData->hab,
						windowSystemData->resInstance,
						ID_ACCELTABLE);

        HWND popupMenu = WinLoadMenu(HWND_OBJECT,
				windowSystemData->resInstance,
				ID_BASEPOPUP);

        // Save these in the WindowSystemData structure
        windowSystemData->menu = menu;
        windowSystemData->accelTable = accelTable;
        somSelf->SetWindowSystemData(ev, windowSystemData);

        // Create MenuBar and PopupMenu objects
        ODWindowState *winState = somSelf->GetWindowState(ev);
        ODMenuBar *menuBar = winState->CreateMenuBar(ev, menu);
        ODPopup *popup = winState->CreatePopupMenu(ev, popupMenu);

        // Set the default accelerator table
        menuBar->SetAccelTable(ev, accelTable);

        // Set the base menu bar for this session
        winState->SetBaseMenuBar(ev, menuBar);

        // Set the popup for this session
        winState->SetBasePopup(ev, popup);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OS2Shell::CreateMenuBar.");
        ODSetSOMException(ev, _exception);
    }
    
}

//------------------------------------------------------------------------------
// InitWindowSystem
//
//	Performs any initialization required by the window system, in this
//	case OS2.  Since the root part is creating the "frame window" there's
//	not much to do here but initialize some data.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OS2ShellInitWindowSystem(OS2Shell *somSelf, 
                                                        Environment *ev)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellInitWindowSystem");

    HAB     hab;
    HMQ     hmq;
    HMODULE resModule;
    ULONG   rc;
    CHAR    loadError[CCHMAXPATH];

    // Set the shell global for utilities to use 
    SetShell(somSelf);

    // Initialize PM facilities for use by application
    hab = WinInitialize(0);
    if (hab != NULLHANDLE)
    {
       // Create a message queue
       hmq = WinCreateMsgQueue(hab, 0);
       if (hmq != NULLHANDLE)
       {
          // Load the resource
          rc = DosLoadModule(loadError, sizeof(loadError), "ODRES",
			&resModule);
          if (rc != 0)
             WARNMSG(WARN_INDEX(AMSG_914), 
                     "Docshell: Error loading resource file %s.", 
                     OD_RESOURCE_DLL);
          try
          {

             // Initialize the platform-specific window system data structure
             // with our application handle, message queue, and our WindowProc
             WindowSystemData *windowSystemData = somSelf->
                                            GetWindowSystemData(ev);
             windowSystemData->hab = hab;
             windowSystemData->hmq = hmq;
             windowSystemData->parentWindow = HWND_DESKTOP;
             windowSystemData->windowProc = ODDocumentWindowProc;
             windowSystemData->resInstance = resModule;
             somSelf->SetWindowSystemData(ev, windowSystemData);

          }
          catch (ODException _exception)
          {
              LOG("SOM exception occured in OS2Shell::InitWindowSystem.");

              // Free the resource
              if (resModule)
                 DosFreeModule(resModule);

              ODSetSOMException(ev, _exception);
          }
       }
    }
}

//------------------------------------------------------------------------------
// Exec
//
//	Start a new session with the given command line program to be 
//	started in the session.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OS2ShellExec(OS2Shell *somSelf, 
                                     Environment *ev,
				     char *str)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellDocumentNew");
    
    UCHAR      objBuffer[CCHMAXPATH] = {0};
    ULONG      sessID = 0;
    PID        pid = 0;
    STARTDATA  startData = {0};
    PSZ        pgmName, pgmParam = NULL;
    APIRET     rc = NO_ERROR;

    // Retrieve the program name and input parameters 
    pgmName = str;
    while (*str != ' ' && *str != '\0')
       str++;
    if (*str == ' ')
    {
       // Null terminated the program name
       *str = '\0';

       // Remove in between blanks
       while (*++str == ' ')
          ;
       pgmParam = str;
    }

    // Get environment of the current process to be inherited by the
    // new process
    PTIB ptib;
    PPIB ppib;
    DosGetInfoBlocks(&ptib, &ppib);

    // Setup data needed to start a window session
    startData.Length = sizeof(STARTDATA);
    startData.Related = SSF_RELATED_INDEPENDENT; // Start an independent session
    startData.FgBg = SSF_FGBG_FORE;              // Start session in foreground
    startData.TraceOpt = SSF_TRACEOPT_NONE;      // No trace
    startData.PgmTitle = (PSZ) NULL;             // No title
    startData.PgmName = pgmName;                 // Program name
    startData.PgmInputs = pgmParam;              // Program input
    startData.TermQ = (PBYTE) NULL;              // No termination queue
    startData.Environment = ppib->pib_pchenv;    // environment string
    startData.InheritOpt = SSF_INHERTOPT_SHELL;  // Inherit shell's environment
    startData.SessionType = SSF_TYPE_PM;         // Windowed PM session 
    startData.IconFile = (PSZ) NULL;             // No icon association 
    startData.PgmHandle = 0;                     // No program handle
    startData.PgmControl = SSF_CONTROL_VISIBLE ||   // Open the session visible
                           SSF_CONTROL_MAXIMIZE;
    startData.InitXPos = 0;  
    startData.InitYPos = 0; 
    startData.InitXSize = 0;
    startData.InitYSize = 0;
    startData.Reserved = 0;
    startData.ObjectBuffer = (char *) objBuffer;    // returns info if fails
                                                    // to start session
    startData.ObjectBuffLen = (ULONG) sizeof(objBuffer);

    // start a new session with program to be started in the new session
    rc = DosStartSession(&startData, &sessID, &pid);
    if (rc != NO_ERROR)
       WARNMSG(WARN_INDEX(AMSG_580), "Docshell: Error opening a new session.");
}

//------------------------------------------------------------------------------
// MainLoop
//
//	Run the standard event dispatching loop
//------------------------------------------------------------------------------
SOM_Scope ODSLong  SOMLINK OS2ShellMainLoop(OS2Shell *somSelf,  Environment *ev)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellMainLoop");

    try
    {
       QMSG  qmsg;
       WindowSystemData *windowSystemData = somSelf->GetWindowSystemData(ev);
       ODDispatcher *dispatcher = somSelf->GetSession(ev)->GetDispatcher(ev);

       while (!dispatcher->ShouldExit(ev))
       {
          while (WinGetMsg(windowSystemData->hab, &qmsg, 0, 0, 0))
             WinDispatchMsg(windowSystemData->hab, &qmsg);

          // Closing a document window from window list or performing 
          // a shutdown poses a WM_QUIT message
          // to the message queue right away without the chance for docshell
          // to close down gracefully. To take care of that, a WM_CLOSE message
          // is sent to the window being closed down if closing is from
          // the window list or to the root window containing the top most
          // draft for the case of shutting down. Then start the message
          // loop again until we get an exit from the dispatcher.
          //
          // Note: The exit from dispatcher is set in CloseCleanup when document
          //       is closed down from our shell, not from the window list.
          if (!dispatcher->ShouldExit(ev))
          {
             // Initialize window to where the WM_CLOSE message is posted
             ODPlatformWindow platformWindow = qmsg.hwnd;

             // Initialize to bring up dialog to confirm closing
             ODULong  confirmClose = 0;  

             // For shutdown, hwnd is set to NULL. Being the case,
             // go through window iterator to find the root window
             // containing the top most draft so as to post the 
             // WM_CLOSE message.
             if (!qmsg.hwnd)
             {
                DocumentManager *docMgr = somSelf->GetActiveDocumentManager(ev);
                ODWindowIterator *windowIter = somSelf->GetWindowState(ev)->
                                               CreateWindowIterator(ev);
                ODWindow *curWindow;

                for (curWindow = windowIter->First(ev);
                     windowIter->IsNotComplete(ev);
                     curWindow = windowIter->Next(ev))
                {
                     if (curWindow && 
                         (curWindow->GetDraft(ev) == docMgr->GetDraft(ev)) &&
                         (curWindow->IsRootWindow(ev)))
                     {
                        // Get the platform window to where WM_CLOSE message
                        // is posted
                        platformWindow = curWindow->GetPlatformWindow(ev);

                        // Close all the root windows belonging to the
                        // top most draft. 
                        if (somSelf->GetWindowState(ev)->GetRootWindowCount(ev,
                            curWindow->GetDraft(ev)) == 1)
                        {
                           // Last root window to close, set to save changes 
                           // without bringing up the dialog
                           confirmClose = kODCommandSaveChanges;
                           break;  
                        }
                        else
                           // Note: Has to use WinSendMsg to make closing 
                           //       synchronous when top most draft contains
                           //       more than 1 root window
                           WinSendMsg(WinWindowFromID(platformWindow, 
                                                      FID_CLIENT), 
                                      WM_CLOSE, 
                                      MPFROMLONG(confirmClose), 
                                      MPFROMSHORT(0));
                     }
                 }
             }
             WinPostMsg(WinWindowFromID(platformWindow, FID_CLIENT), WM_CLOSE, 
                            MPFROMLONG(confirmClose), MPFROMSHORT(0));
          }
       }
    }
    catch (ODException _exception)
    {
       LOG("SOM Exception occured in OS2Shell:MainLoop.");
       ODSetSOMException(ev, _exception);
    }

    return (0);
}

//------------------------------------------------------------------------------
// Terminate
//
//	Perform any necessary task to clean up before exit
//------------------------------------------------------------------------------
SOM_Scope void SOMLINK OS2ShellTerminate(OS2Shell *somSelf,  Environment *ev)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellTerminate");

    WindowSystemData *windowSystemData = kODNULL;
    try
    {
       WindowSystemData *windowSystemData = somSelf->GetWindowSystemData(ev);
    }
    catch (ODException _exception)
    {
       LOG("SOM Exception occured in OS2Shell:Terminate.");
       ODSetSOMException(ev, _exception);
    }

    if (windowSystemData)
    {
       // Destroy the message queue
       WinDestroyMsgQueue(windowSystemData->hmq);

       // Terminate the application
       WinTerminate(windowSystemData->hab);
    }
}

//------------------------------------------------------------------------------
// GetEventType
//
//	Returns the message number parameter from the event structure
//------------------------------------------------------------------------------
SOM_Scope ODULong  SOMLINK OS2ShellGetEventType(OS2Shell *somSelf, 
                                                       Environment *ev, 
                                                      ODEventData* event)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellGetEventType");

    return (event->msg);
}

//------------------------------------------------------------------------------
// GetEventSubType
//
//	Returns the wParam parameter from the event structure
//------------------------------------------------------------------------------
SOM_Scope ODULong  SOMLINK OS2ShellGetEventSubType(OS2Shell *somSelf, 
                                                          Environment *ev, 
                                                         ODEventData* event)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellGetEventSubType");

    return (LONGFROMMP(event->mp1));
}

//------------------------------------------------------------------------------
// GetEventWindow
//
//	Returns the window from the event structure
//------------------------------------------------------------------------------
SOM_Scope ODPlatformWindow  SOMLINK OS2ShellGetEventWindow(OS2Shell *somSelf, 
                                                    Environment *ev, 
                                                   ODEventData* event)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellGetEventWindow");
    
    ODPlatformWindow parentWindow = WinQueryWindow(event->hwnd, QW_PARENT);
    try
    {
       // Returns the parent window if it is an OpenDoc window
       if (!(parentWindow && 
           somSelf->GetWindowState(ev)->IsODWindow(ev, parentWindow)))
           parentWindow = kODNULL;
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OS2Shell:GetEventWindow.");
        parentWindow = kODNULL;
        ODSetSOMException(ev, _exception);
    }

    return parentWindow;
}

//------------------------------------------------------------------------------
// DocumentClose
//
//	This method is invoked when the Close item is selected from the
//	document menu.  We send ourselves a WM_CLOSE message so that the
//	document closing process can be handled uniformly whether or not
//	the close comes from the user or the window system.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OS2ShellDocumentClose(OS2Shell *somSelf, 
                                               Environment *ev)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellDocumentClose");

    try
    {
        // Send message for our event dispatching method to process.  It will
        // invoke the ConfirmCloseDlg utility to post confirm dialog.
        ODWindowState* winState = somSelf->GetWindowState(ev);
        TempODWindow   window   = winState->AcquireActiveWindow(ev);

	WinPostMsg(window->GetPlatformWindow(ev), WM_CLOSE,
			MPFROMLONG(kODCommandCloseDlg), (MPARAM)NULL);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OS2Shell:GetEventWindow.");
        ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// CloseCleanup - This method will do any platform dependent cleanup
//                tasks not done in other methods during the close process.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OS2ShellCloseCleanup(OS2Shell *somSelf, 
                                               Environment *ev)
{
    OS2ShellMethodDebug("OS2Shell","OS2ShellCloseCleanup");

    try
    {
       WindowSystemData *windowSystemData = somSelf->GetWindowSystemData(ev);
	
       if (windowSystemData)
       {
          // Notify dispatcher that the document shell is terminating
          somSelf->GetSession(ev)->GetDispatcher(ev)->Exit(ev);

          // Post a quit message to the message queue
          WinPostQueueMsg(windowSystemData->hmq, WM_QUIT, 0, 0);
       }

    }
    catch (ODException _exception)
    {
       LOG("SOM exception occured in CloseCleanup.");
       ODSetSOMException(ev, _exception);
    }
}
