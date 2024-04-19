/* @(#)Z 1.6 os2/src/avsshell/avssessn.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:54 (96/09/30 13:45:24) */
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

//
// Copyright (C) Apple Computer, Inc., 1994
//
//
//******************************************************************


#define INCL_DOSMODULEMGR
#define INCL_WINWINDOWMGR
#define INCL_WIN
#define INCL_WINSWITCHLIST
#define INCL_WINERRORS
#include <os2.h>

#ifndef SOM_Module_shsessn_Source
#define SOM_Module_shsessn_Source
#endif
#define AVSShellSession_Class_Source
#define VARIABLE_MACROS
#ifndef ODExtension_Class_Source
#define ODExtension_Class_Source
#endif

#include <avsshell.h>
#include <dsres.h>

// Can not find Status file anymore and hwndStatusLine is not used in code.
// #ifndef SOM_ODStatusLineExtension_xih
// #include "Status.xih"
// #endif
#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif
#ifndef SOM_Module_OpenDoc_StandardExtensions_defined
#include <StdExts.xh>
#endif
#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#include "avssessn.xih"

// pick up defines
// #include "dsres.h"
//#ifndef SOM_ODSession_xih
//#include <ODSessn.xih>
//#endif

#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************
#include <unidef.h>
#include <avuni.h>
#endif

// pointer to window proc subclassed for status line support
PFNWP                    OldShellWndProc;
// subclass of window proc to support status line
extern "C" {
 MRESULT EXPENTRY NewShellWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
}

SOM_Scope void  SOMLINK AVSShellSessionSetShellPointer(AVSShellSession *somSelf,
                                                  Environment *ev,
                                                 AvsShell* shell)
{
    AVSShellSessionData *somThis = AVSShellSessionGetData(somSelf);
    AVSShellSessionMethodDebug("AVSShellSession","AVSShellSessionSetShellPointer");

    _theShell = shell;
}

SOM_Scope void  SOMLINK AVSShellSessionsomInit(AVSShellSession *somSelf)
{
    /* AVSShellSessionData *somThis = AVSShellSessionGetData(somSelf); */
    AVSShellSessionMethodDebug("AVSShellSession","AVSShellSessionsomInit");

    AVSShellSession_parent_ODSession_somInit(somSelf);
}

SOM_Scope void  SOMLINK AVSShellSessionsomUninit(AVSShellSession *somSelf)
{
    /* AVSShellSessionData *somThis = AVSShellSessionGetData(somSelf); */
    AVSShellSessionMethodDebug("AVSShellSession","AVSShellSessionsomUninit");

    AVSShellSession_parent_ODSession_somUninit(somSelf);
}

SOM_Scope void  SOMLINK AVSShellSessionInitSession(AVSShellSession *somSelf,
                                              Environment *ev)
{
    /* AVSShellSessionData *somThis = AVSShellSessionGetData(somSelf); */
    AVSShellSessionMethodDebug("AVSShellSession","AVSShellSessionInitSession");

    AVSShellSession_parent_ODSession_InitSession(somSelf, ev);
}

// No one use this.
// HWND  hwndStatusLine; // handle to Status line window Ben Garrett 02/10/95

SOM_Scope ODPlatformWindow  SOMLINK AVSShellSessionCreatePlatformWindow(AVSShellSession *somSelf,
                                                                   Environment *ev,
                                                                   ODPlatformWindowCreateOptions flags)
{
    AVSShellSessionData *somThis = AVSShellSessionGetData(somSelf);
    AVSShellSessionMethodDebug("AVSShellSession","AVSShellSessionCreatePlatformWindow");

   #if 0
       return (AVSShellSession_parent_ODSession_CreatePlatformWindow(somSelf,
                                                               ev));
   #else
       HWND hwndClient;
       char achErrorBuf[80];
       ERRORID err = 0;
       ULONG myflag = FCF_TITLEBAR|
                      FCF_SIZEBORDER|
                      FCF_SHELLPOSITION|
                      FCF_SYSMENU|
                      FCF_MINMAX|
                      FCF_TASKLIST|
                      FCF_ICON
                      ;

       err = WinGetLastError(WinQueryAnchorBlock(HWND_DESKTOP));


#ifdef _UNICODE_
//************************************
//*   set up for UNICODE             *
//*   8/28/95     jss                *
//************************************

       char      szMsg1[CCHMAXPATH];
       char      szMsg2[CCHMAXPATH];

       WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                      AV_OPENDOCSHELL, sizeof(szMsg1), szMsg1);

       WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                      AV_UNTITLEDDOC, sizeof(szMsg2), szMsg2);

       ODPlatformWindow platformWindow = WinCreateStdWindow(
                                                  HWND_DESKTOP,
                                                  WS_VISIBLE,
                                                  &myflag,
                                                  (PSZ)szMsg1,
                                                  (PSZ)szMsg2,
                                                  WS_VISIBLE,
                                                  (HMODULE)0,
                                                  0,
                                                  &hwndClient);
#else
       ODPlatformWindow platformWindow =  WinCreateStdWindow(
                                                  HWND_DESKTOP,
                                                  WS_VISIBLE,
                                                  &myflag,
                                                  (PSZ)"OpenDocShell",
                                                  (PSZ)"OpenDoc Link Manager Log",
                                                  WS_VISIBLE,
                                                  (HMODULE)0,
                                                  ROOT_DRAFTS_RESID,
                                                  &hwndClient);
#endif

       err = WinGetLastError(WinQueryAnchorBlock(HWND_DESKTOP));
       WinSetWindowPtr(hwndClient, 0, _theShell);

// *********************************************************
//  create status line, 02/07/95 Ben Garrett
// *********************************************************

  // The following controls are created to display the AS states
  // for debugging purpose only!!!!

    HWND hwndDocList = WinCreateWindow(hwndClient,
                                       WC_LISTBOX,
                                       "",
                                       WS_VISIBLE | LS_HORZSCROLL,
                                       20, 10, 370, 440,
                                       hwndClient,
                                       HWND_TOP,
                                       300,
                                       NULL,
                                       NULL);

    HWND hwndLinkList = WinCreateWindow(hwndClient,
                                        WC_LISTBOX,
                                        "",
                                        WS_VISIBLE | LS_HORZSCROLL,
                                        400, 10, 370, 440,
                                        hwndClient,
                                        HWND_TOP,
                                        301,
                                        NULL,
                                        NULL);


    HWND hwndTxt1 = WinCreateWindow(hwndClient,
                                    WC_STATIC,
                                    "Document activity log:",
                                    WS_VISIBLE | SS_TEXT,
                                    20, 450, 370, 20,
                                    hwndClient,
                                    HWND_TOP,
                                    302,
                                    NULL,
                                    NULL);

    HWND hwndTxt2 = WinCreateWindow(hwndClient,
                                    WC_STATIC,
                                    "Link source activity log:",
                                    WS_VISIBLE | SS_TEXT,
                                    400, 450, 370, 20,
                                    hwndClient,
                                    HWND_TOP,
                                    303,
                                    NULL,
                                    NULL);

    _theShell->SetDocListHWND(hwndDocList);
    _theShell->SetLinkListHWND(hwndLinkList);

// *********************************************************

       SWP swp;
       WinQueryTaskSizePos(WinQueryAnchorBlock(HWND_DESKTOP),
                           0, &swp);
       WinSetWindowPos((HWND)platformWindow, HWND_TOP,
                       swp.x, swp.y, swp.cx, swp.cy,
                       SWP_SIZE | SWP_MOVE);

       return platformWindow;
   #endif
}
