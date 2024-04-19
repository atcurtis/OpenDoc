/* @(#)Z 1.18 com/src/docshell/rlshell.cpp, odshell, od96os2, odos29646d 96/11/15 15:52:00 (96/10/29 09:24:46) */
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


#include "rlshell.h"

int              ready2dispatch = 0;
WindowSystemData windowSystemData;

/*
 * Function prototypes
 */
#ifdef _PLATFORM_OS2_
MRESULT EXPENTRY
    ODSessionWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
#endif
#ifdef _PLATFORM_WIN32_
LRESULT CALLBACK
    ODSessionWindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

#ifdef _PLATFORM_UNIX_
void closeCallBack(Widget widget, XtPointer client_data, XtPointer call_data);
#endif

// Enable the interfaces to the rest of OpenDoc
#define SESSION_IS_READY 1


//==============================================================================
// Constants
//==============================================================================

static const char* const kNullValueType = "0";

//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------

RealShell::RealShell()
{
    // Set member variables to default values
}


RealShell::~RealShell()
{
    // Release objects
}


//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------

int RealShell::go(int argc, char *argv[])
{
    ODBoolean   initialized = kODFalse;

    // Parse the command line arguments
    this->ProcessArgs(argc, argv);

    // Initialize the OpenDoc environment
    this->Initialize();
    initialized = kODTrue;

    // Open an existing document or create a new one
#if 0
    if (fFilename)
    {
        // code to open an existing file goes here
    }
    else
#endif
    {

#if 0           // cmd line problem with VAC++
    strcpy(fRootPartKind, "SimplePart");  // replace with your part kind
#endif

        // Create a new document with the specified root part kind
        this->OpenDocumentWithPart(fRootPartKind);
    }
    // Start the main loop
    this->MainLoop();

    return 1;
}


//------------------------------------------------------------------------------
// Initialization
//------------------------------------------------------------------------------

void RealShell::Initialize()
{

    // Prevent dispatching until we're ready
    ready2dispatch = 0;

    // Initialize the windowing system
    this->InitWindowSystem();

    fEV = somGetGlobalEnvironment();
#ifdef SESSION_IS_READY
    fSession = new ODSession;
    fSession->SetWindowSystemData(fEV, &windowSystemData);
    fSession->InitSession(fEV);
    fWindowState = fSession->GetWindowState(fEV);
    fDispatcher = fSession->GetDispatcher(fEV);
#endif
    fPermissions = kODDPExclusiveWrite;
    ready2dispatch = 1;
}

void RealShell::ProcessArgs(int argc, char *argv[])
{
    int  i;

    if (argc == 3)
        for (i = 1; i < argc; i++)
        {
            if (strcmp("-f", argv[i]) == 0)
                strcpy(fFilename, argv[++i]);
            else if (strcmp("-k", argv[i]) == 0)
                strcpy(fRootPartKind, argv[++i]);
        }
}

void RealShell::OpenDocumentWithPart(ODValueType PartType)
{
    if (!strcmp(PartType,kNullValueType))
       PartType = (ODValueType)"NoPart";
    PlatformFile *pFile = this->NewDocument(PartType);
    this->OpenFile(pFile);
    this->Open();
}

PlatformFile* RealShell::NewDocument(ODValueType PartType)
{
    ODContainer *newContainer;
    PlatformFile *file = this->CreateUntitledContainer(kODNULL, &newContainer);
    this->CreateDocument(newContainer, PartType);
    // ODReleaseObject(fEV, newContainer);
    return file;
}

void RealShell::OpenFile(PlatformFile *pFile)
{
#if SESSION_IS_READY
    fContainer = GetFileContainer(fEV, fSession, &(pFile->GetFileSpec()));
    fDocument = fContainer->AcquireDocument(fEV, kODDefaultDocument);
    fDraft = fDocument->AcquireDraft(fEV, fPermissions, 0, kODNULL, kODPosTop,
                                        kODFalse);
#endif
}

void RealShell::Open()
{
#if SESSION_IS_READY
    // fDraft already set in OpenFile() ??
    //fDraft = fDocument->CreateDraft(fEV, fDraft, kODTrue);
    ODStorageUnit *su = fDraft->AcquireDraftProperties(fEV);
    ODPart* rootPart = fDraft->AcquirePart(fEV, ODGetStrongSURefProp(fEV, su,
                        kODPropRootPartSU, kODStrongStorageUnitRef));

    rootPart->Open(fEV, kODNULL);

    fWindowState->SetDefaultWindowTitles(fEV, fDraft);
#endif
}

PlatformFile* RealShell::CreateUntitledContainer(ODDocument **documentPtr,
                                                ODContainer **containerPtr)
{
#if SESSION_IS_READY
    PlatformFile *file = new PlatformFile;
    file->Specify((ODFileSpec *)"NewDoc.od");
    file->UniquifyName( kODNoResourceID,        // uniquifyingStringID
                        kODNULL,                // uniquifyingString
                        kODNoResourceID,        // uniquifyingNumberID
                        kODNULL,                // uniquifyingNumberString
                        1,                      // copyCount
                        kSpecifyNewNameOnly,    // action
                        kODTryCurrentName       // forceNewName
                        );
    file->Create(kODNULL, kODNULL, kODNULL);
    (*containerPtr) = CreateFileContainer(fEV, fSession,
                                                &(file->GetFileSpec()));
    if (documentPtr)
        (*documentPtr) = (*containerPtr)->AcquireDocument(fEV,
                                                kODDefaultDocument);
    return file;
#else
    PlatformFile *file;
    return file;
#endif
}

void RealShell::CreateDocument(ODContainer *newContainer, ODValueType partType)
{
#if SESSION_IS_READY
    ODDocument *newDocument =
        newContainer->AcquireDocument(fEV,kODDefaultDocument);
    ODDraft *newDraft = newDocument->AcquireBaseDraft(fEV, kODDPExclusiveWrite);
    ODPart *rootPart = newDraft->CreatePart(fEV, partType, kODNULL);
    ODStorageUnit *su = newDraft->AcquireDraftProperties(fEV);
    ODSetStrongSURefProp(fEV, su, kODPropRootPartSU, kODStrongStorageUnitRef,
                        rootPart->GetStorageUnit(fEV)->GetID(fEV));
    newDraft->Externalize(fEV);

    // ODReleaseObject(fEV, rootPart);
    // ODReleaseObject(fEV, su);
#endif
}

//------------------------------------------------------------------------------
// Main Event Loop
//------------------------------------------------------------------------------

#ifdef _PLATFORM_WIN32_

int RealShell::MainLoop()
{
    MSG  msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

#endif  // _PLATFORM_WIN32_

//--------------------------------------------------------------------------

#ifdef _PLATFORM_OS2_

int RealShell::MainLoop()
{
    QMSG qmsg;                            /* Message from message queue   */


    while( WinGetMsg( fhab, &qmsg, 0L, 0, 0 ) )
        WinDispatchMsg( fhab, &qmsg );

    return 0;
}

#endif  // _PLATFORM_OS2_

//--------------------------------------------------------------------------


#ifdef _PLATFORM_AIX_

void RealShell::MainLoop()
{
    XEvent Xev;

#if SESSION_IS_READY
    while ( (fDispatcher) && (!fDispatcher->ShouldExit(fEV)) )
#else
    while (1)
#endif
        {
                XtAppNextEvent(fAppContext, &Xev);
                this->DispatchEvent((ODEventData*)&Xev);
        }
}

#endif  // AIX

//--------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Platform Specific Windowing Code
//------------------------------------------------------------------------------

#ifdef _PLATFORM_WIN32_

ODBoolean RealShell::InitWindowSystem()
{
#if 0                      // TMW, SRA, and RB - Removed because now the Root Part (or Root NoPart) creates document window
    WNDCLASS wc;
    HWND     sessionWindow;

    // Define and register the session window class
    wc.style         = CS_DBLCLKS;
    wc.lpfnWndProc   = ODSessionWindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 12;
    wc.hInstance     = ApplicationInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "ODSessionWndClass";

    if (!RegisterClass(&wc))
        return kODFalse;

    // Create the window
    sessionWindow = CreateWindowEx(0L, "ODSessionWndClass", "OpenDoc",
                              WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL,
                              ApplicationInstance, NULL);

    // Display the window
    ShowWindow(sessionWindow, WindowDisplayMode);
    UpdateWindow(sessionWindow);
#endif // #if 0

    windowSystemData.instance = ApplicationInstance;
    windowSystemData.parentWindow = HWND_DESKTOP;
    windowSystemData.windowProc = ODSessionWindowProc;

    return kODTrue;
}

LRESULT CALLBACK ODSessionWindowProc(HWND window, UINT msg, WPARAM wParam,
                                        LPARAM lParam)
{
    LRESULT  result = 0;

    ODEventData event;

    event.hwnd = window;
    event.message = msg;
    event.wParam = wParam;
    event.lParam = lParam;

    if (!ready2dispatch)
      return DefWindowProc(window, msg, wParam, lParam);

    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
        {
            TempODWindow odWindow =
                        fWindowState->AcquireODWindow(fEV, window);
            if (odWindow && odWindow->GetRootFacet(fEV))
            {
                    odWindow->AdjustWindowShape(fEV);
                    result = kODTrue;
            }

            break;
        }

        case WM_PAINT:
        {

            // Create an empty region
            HRGN rgn = CreateRectRgn(0,0,0,0);

            // Get the invalid region
            GetUpdateRgn(window, rgn, FALSE);

            PAINTSTRUCT ps;
            BeginPaint(window, &ps);
            EndPaint(window, &ps);

            // If the dispatcher can't handle the message, dispatch
            // to the default windows procedure
            event.lParam = (LPARAM) rgn;
            if (result = fDispatcher->Dispatch(fEV, &event) == kODFalse)
            {
                result = DefWindowProc(window, msg, wParam, lParam);
            }

            // Delete the region
            DeleteObject(rgn);

            break;
        }

        default:
            result = fDispatcher->Dispatch(fEV, &event);
            if (!result)
              result = DefWindowProc(window, msg, wParam, lParam);
    }

    return result;
}

#endif  // _PLATFORM_WIN32_

//--------------------------------------------------------------------------

#ifdef _PLATFORM_OS2_

ODBoolean RealShell::InitWindowSystem()
{
    HWND hwndClient;
    HWND hwndFrame;
    ULONG flCreateFlags =
                  FCF_TITLEBAR
//              | FCF_MENU
                | FCF_MINMAX
                | FCF_SIZEBORDER
//              | FCF_SHELLPOSITION
//              | FCF_TASKLIST
//              | FCF_ICON
                ;

    windowSystemData.parentWindow = HWND_DESKTOP;
    windowSystemData.windowProc = ODSessionWindowProc;

    if ((fhab = WinInitialize(0)) == 0L)  /* Initialize PM     */
        { DosBeep(100, 10); }

    if ((fhmq = WinCreateMsgQueue(fhab, 0 )) == 0L)/* Create a msg queue */
        { DosBeep(100, 20); DosBeep(100, 20); }

    // temporarily do this until WindowState is ready
#if 1                      // TMW, SRA, and RB - Removed because now the Root Part (or Root NoPart) creates document window
    WinRegisterClass(                /* Register window class        */
        fhab,                        /* Anchor block handle          */
        (PSZ)"OpenDocWindow",        /* Window class name            */
        (PFNWP)ODSessionWindowProc,  /* Address of window procedure  */
        CS_SIZEREDRAW,               /* Class style                  */
        0                            /* No extra window words        */
        );

    hwndFrame = WinCreateStdWindow(
        HWND_DESKTOP,
        WS_VISIBLE,
        &flCreateFlags,
        (PSZ)"OpenDocWindow",
        (PSZ)"OpenDoc",
        0,
        NULLHANDLE,
        0,
        &hwndClient);

    if (!WinSetWindowPos(hwndFrame, HWND_TOP, 100, 100, 400, 400,
                SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW))
        { DosBeep(100, 30); DosBeep(100, 30); DosBeep(100, 30); }
#endif

    return kODTrue;
}

MRESULT EXPENTRY ODSessionWindowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  MRESULT  result = 0;

    ODEventData event;

    event.hwnd = hwnd;
    event.msg  = msg;
    event.mp1  = mp1;
    event.mp2  = mp2;

    if (!ready2dispatch)
      return WinDefWindowProc( hwnd, msg, mp1, mp2 );

  switch( msg )
  {
    case WM_DESTROY:
      /*
       * This is the place to put your termination routines
       */
      WinPostMsg( hwnd, WM_QUIT, (MPARAM)0,(MPARAM)0 );/* Cause termination*/
      break;

    case WM_SIZE:
    {
#if SESSION_IS_READY
        TempODWindow odWindow = fWindowState->AcquireODWindow(fEV, hwnd);
        if (odWindow && odWindow->GetRootFacet(fEV))
        {
                odWindow->AdjustWindowShape(fEV);
                result = (MRESULT)kODTrue;
        }
#endif
        break;
    }

    case WM_PAINT:
      /*
       * Window contents are drawn here in WM_PAINT processing.
       */
      {
      HPS    hps;                       /* Presentation Space handle    */
      RECTL  rc;                        /* Rectangle coordinates        */
      POINTL pt;                        /* String screen coordinates    */
                                        /* Create a presentation space  */
      hps = WinBeginPaint( hwnd, 0L, &rc );
      WinFillRect( hps, &rc,  /*SYSCLR_WINDOW*/ 0);
      WinEndPaint( hps );                      /* Drawing is complete   */
      break;
      }

    default:
      /*
       * Everything else comes here.  This call MUST exist
       * in your window procedure.
       */
#if SESSION_IS_READY
       result = (MRESULT)fDispatcher->Dispatch(fEV, &event);
       if (!result)
#endif
           return WinDefWindowProc( hwnd, msg, mp1, mp2 );
  }
  return result;
} /* End of ODSessionWindowProc */

#endif  // OS2

//--------------------------------------------------------------------------

#ifdef _PLATFORM_UNIX_

#define INITIAL_WIDTH 500
#define INITIAL_HEIGHT 500

ODBoolean RealShell::InitWindowSystem()
{
    Dimension menu_width, menu_height;
    Dimension mainw_margin_width, mainw_margin_height;

    // Initialize Toolkit, set AppContext
    fApplicationShell = XtVaAppInitialize(&fAppContext, "OpenDoc",
                                NULL, 0, NULL, NULL, NULL, NULL);
    fDisplay = XtDisplay(fApplicationShell);
    fScreen = XDefaultScreenOfDisplay(fDisplay);
    XtVaSetValues(fApplicationShell,
                    XtNx, 0,
                    XtNy, 0,
                    XtNwidth, 500,
                    XtNheight, 500,
                    XmNiconName, "OpenDoc",
                    XmNtitle, "OpenDoc",
                    NULL);


#if 0   // temporarily do this until WindowState is ready
    // Creation of mainWindow
    XmString title = XmStringCreateSimple("OpenDoc");
    fMainWinWidget = XtVaCreateManagedWidget( "mainWindow",
                    xmMainWindowWidgetClass,
                    fApplicationShell,
                    XmNwidth,       500,
                    XmNheight,      500,
                    XmNdialogTitle, title,
                    NULL );
    XmStringFree(title);

    // Create a dummy MenuBar
    XmString doc = XmStringCreateSimple("Document");
    Widget menubar = XmVaCreateSimpleMenuBar(fMainWinWidget, "menubar",
        XmVaCASCADEBUTTON, (XtCallbackProc)doc, 'D',
        NULL);
    XmStringFree(doc);

    // add a dummy button
    XmString close = XmStringCreateSimple("Close");
    Widget menu = XmVaCreateSimplePulldownMenu(menubar, "file_menu", 0,
        (XtCallbackProc)exit,
        XmVaPUSHBUTTON, (XtCallbackProc)close, 'C', NULL, NULL,
        NULL);
    XmStringFree(close);

    XtManageChild(menubar);
    XmMainWindowSetAreas(fMainWinWidget, menubar, NULL, NULL, NULL, NULL);

    XtVaGetValues ( fMainWinWidget,
                XmNmainWindowMarginWidth, &mainw_margin_width,
                XmNmainWindowMarginHeight, &mainw_margin_height,
                NULL);

    XtVaGetValues ( menubar,
                XmNwidth, &menu_width,
                XmNheight, &menu_height,
                NULL);

    // create a dummy MainWindow workArea
    Widget drawing_a = XtVaCreateManagedWidget("drawing_a",
        xmDrawingAreaWidgetClass, fMainWinWidget,
        XmNwidth,      480,
        XmNheight,     480,
        XmNmarginWidth, 0,
        XmNmarginHeight, 0,
        NULL);

    // set the MainWindow areas
    XmMainWindowSetAreas(fMainWinWidget, menubar, NULL, NULL, NULL, drawing_a);

    XtRealizeWidget(fApplicationShell);
#endif

#if SESSION_IS_READY
    windowSystemData.parentWindow = 0;
    windowSystemData.appContext = fAppContext;
    windowSystemData.appShell = fApplicationShell;
//    windowSystemData.mainWindow = fMainWinWidget;
    windowSystemData.display = fDisplay;
    windowSystemData.screen = fScreen;
    windowSystemData.closeCallBack = closeCallBack;
#endif

    return kODTrue;
}

void RealShell::DispatchEvent(ODEventData* event)
{
    if (!event) return;

    switch (event->type)
    {
    default:
#if SESSION_IS_READY
       fDispatcher->Dispatch(fEV, event);
#endif
        ;
    }

    XtDispatchEvent((XEvent*)event);
}

void closeCallBack(Widget widget, XtPointer client_data, XtPointer call_data)
{
    exit (0);
}


#endif  // UNIX

//--------------------------------------------------------------------------
