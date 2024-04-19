/* @(#)Z 1.9 com/src/docshell/rlshell.h, odshell, od96os2, odos29646d 96/11/15 15:52:00 (96/10/29 09:24:50) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odshell
 *
 *   CLASSES:   RealShell
 *
 *   ORIGINS: 27
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


#ifndef _RLSHELL_
#define _RLSHELL_

#include <stdio.h>

#ifdef _PLATFORM_OS2_
#include <ODos2.h>
#endif // _PLATFORM_OS2_

#ifdef _PLATFORM_WIN32_
#include <windows.h>
extern HINSTANCE ApplicationInstance;
extern int       WindowDisplayMode;
#endif // _PLATFORM_WIN32_

#ifdef _PLATFORM_UNIX_
#include <Xm/XmAll.h>
#endif

#include <Disptch.xh>
#include <WinStat.xh>
#include <Window.xh>
// #include <MenuBar.xh>
#include <ODSessn.xh>
#include <ODStor.xh>
#include <StorageU.xh>
#include <Document.xh>
#include <ODCtr.xh>
#include <Draft.xh>
#include <Part.xh>
#include <StdTypes.xh>
#include <StdProps.xh>
#include <StdTypIO.h>
#include <StorUtil.h>
#include <ODMemory.h>
#include <ODTypesF.h>
#include <ODTypes.h>
#include <PlfmFile.h>
#include <TempObj.h>

// Classes used by this interface
class ODSession;
class ODDispatcher;
class ODWindowState;
class ODArbitrator;
class ODDraft;
class ODDocument;
class ODContainer;
class ODMenuBar;
class PlatformFile;


//==============================================================================
// RealShell
//==============================================================================

class RealShell
{

public:

//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------

  RealShell();
  ODVMethod ~RealShell();

//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------
  ODVMethod int go(int argc, char *argv[]);

protected:

//-------------------------------------------------------------------------------------
// Initialization
//-------------------------------------------------------------------------------------

  ODMethod void Initialize();
  ODMethod ODBoolean InitWindowSystem();
  ODMethod void ProcessArgs(int argc, char *argv[]);

  ODMethod void OpenDocumentWithPart(ODValueType PartType);
  ODMethod PlatformFile* NewDocument(ODValueType PartType);
  ODMethod void OpenFile(PlatformFile *pFile);
  ODMethod void Open();
  ODMethod PlatformFile* CreateUntitledContainer(ODDocument **documentPtr,
						 ODContainer **containerPtr);
  ODMethod void CreateDocument(ODContainer *container, ODValueType PartType);

//-------------------------------------------------------------------------------------
// Main Event Loop
//-------------------------------------------------------------------------------------

#ifdef _PLATFORM_UNIX_
  ODMethod void MainLoop();
  ODMethod void DispatchEvent(ODEventData* event);
#else
  ODMethod int MainLoop();
#endif

//-------------------------------------------------------------------------------------
// Private Fields
//-------------------------------------------------------------------------------------
  Environment*		fEV;
  ODSession*		fSession;
  ODWindowState*	fWindowState;
  ODDispatcher*		fDispatcher;
  ODArbitrator*		fArbitrator;
  ODContainer*		fContainer;
  ODDocument*		fDocument;
  ODDraft* 		fDraft;
  ODDraftPermissions	fPermissions;
  char			fFilename[256];
  char			fRootPartKind[256];
#ifdef  _PLATFORM_OS2_
  HAB                   fhab;
  HMQ                   fhmq; 
#endif
#ifdef  _PLATFORM_UNIX_
  XtAppContext          fAppContext;
  Widget                fApplicationShell;
  Widget                fMainWinWidget;
  Display*              fDisplay;
  Screen*               fScreen;
#endif
};

#endif  // _RLSHELL_
