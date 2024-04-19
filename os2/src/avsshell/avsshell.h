/* @(#)Z 1.3 os2/src/avsshell/avsshell.h, oddataxfer, od96os2, odos29646d 96/11/15 15:50:01 (96/08/23 01:40:34) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: oddataxfer
 *
 *   CLASSES:   AvsShell
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

// Copyright (C) Apple Computer, Inc., 1994
//
//******************************************************************

#ifndef _AVSSHELL_
#define _AVSSHELL_

//#include <OSAEvent.h>

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifdef _PLATFORM_OS2_
#ifndef _ODTYPESP_
#include <ODTypesP.h>
#endif
#endif // _PLATFORM_OS2_

#ifndef SOM_ODDraft_xh
#include <Draft.xh>      // for ODDraftPermissions
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>    /* for ODFileSpec */
#endif

#ifndef _PLATFORM_OS2_
#ifndef __PROCESSES__
#include <Processes.h>    /* For ProcessSerialNumber */
#endif
#endif

//=============================================================================
// Theory of Operation
//=============================================================================

/*
  The Shell contains all code which interfaces a particular platform's
  user interface with the OpenDoc parts underneath.
  Each document is run in its own process.

*/

//=============================================================================
// Constants
//=============================================================================

//-----------------------------------------------------------------------------
// OSType constants
//-----------------------------------------------------------------------------

const DescType kRealShellPropAccessor     = 0x73706163;  // 'spac';
const DescType kRealShellSelfPropAccessor  = 0x7368656C;  // 'shel';
const  OSType kOpenDocLetterType     = 0x626C7472;  // 'bltr';

//=============================================================================
// Scalar Types
//=============================================================================

typedef  ODSShort  ODResNumber;

//=============================================================================
// Classes defined in this interface
//=============================================================================

class AvsShell;

//=============================================================================
// Classes used by this interface
//=============================================================================

class AVSShellSession;
class ODSession;
class ODDispatcher;
class ODWindowState;
class ODArbitrator;
class ODDraft;
class ODDocument;
class ODContainer;
class ODMenuBar;
class ODPopup;
class ShellSI;
class PlatformFile;
class StandardFileReply;
class OrderedCollection;
class ODAppleEvent;
class ODDesc;
class IODAvailServer;

//=============================================================================
// RealShell
//=============================================================================
#define SM_SETCLIENTFOCUS      (WM_USER + 0)

#define kRealShellID "appl:RealShell$class,1.0.0"
#ifdef _PLATFORM_OS2_
extern "C" {
   MRESULT EXPENTRY ODShellWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
   MRESULT EXPENTRY ODPartWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
}
#endif

class AvsShell
{
public:
   friend MRESULT ODShellWndProc(HWND, ULONG, MPARAM, MPARAM);
   friend MRESULT ODPartWndProc(HWND, ULONG, MPARAM, MPARAM);

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

  AvsShell();
  ODVMethod ~AvsShell();

//-----------------------------------------------------------------------------
// Main Entry Point
//-----------------------------------------------------------------------------

  ODVMethod void go( int argc, char *argv[] );

//-----------------------------------------------------------------------------
// Availability Server access
//-----------------------------------------------------------------------------

  // CloneLinkSource:
  // Called by availability server when a linkID needs to be opened.
  // The pathName is used to find the document.  The
  // document is opened read-only.  The linkservice is
  // asked to find the SU of the LinkSource corresponding to this linkID.
  // This LinkSource is then cloned into the surrogateDraft,
  // and the SUID of that clone is returned.
 ODMethod  ODStorageUnitID  CloneLinkSource(ODLinkID linkID,
                                             ODISOStr pathName);

 ODMethod void SetDocListHWND(HWND docListHWND);
 ODMethod void UpdateDocList(ULONG docID, char* pathName, char flag);
 ODMethod void SetLinkListHWND(HWND linkListHWND);
 ODMethod void UpdateLinkList(ULONG linkID, ULONG docID, ULONG flag);

//-----------------------------------------------------------------------------
// public for OSAEvent handlers, but private by convention
//-----------------------------------------------------------------------------
//- crs - 2/1/95 - made these public for the bento browser...
  ODMethod void SaveDraft();

protected:

//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------

  ODMethod void Initialize(char* name /* filename of doc and alias of server */);

//-----------------------------------------------------------------------------
// New
//-----------------------------------------------------------------------------

  ODMethod ODFileSpec  CreateNamedContainer(ODDocument** documentPtr,
        ODContainer** containerPtr, PlatformFile* currentFile, const char * basename);

//-----------------------------------------------------------------------------
// Open
//-----------------------------------------------------------------------------

  ODMethod void OpenFile(PlatformFile* file);

  ODMethod void Open();

  ODMethod void OpenDraft(ODDraft* draft);
  ODMethod PlatformFile*   AvsShell::OpenUntitled( const char *);

//-----------------------------------------------------------------------------
// Drafts
//-----------------------------------------------------------------------------

  ODMethod  ODBoolean  HasWriteAccess();

//-----------------------------------------------------------------------------
// private fields
//-----------------------------------------------------------------------------

  Environment*    fSOMEnvironment;
  AVSShellSession*   fSession;
  ODContainer*    fContainer;
  ODDocument*     fDocument;
  ODDraft*        fDraft;
  ODContainer*    fSurrogateContainer;
  ODDocument*     fSurrogateDocument;
  ODDraft*        fSurrogateDraft;
  ODHandle       fSurrogateContainerHandle;
  IODAvailServer* fAvailabilityServer;
  ODDraftPermissions fPermissions;
  ODPlatformWindow   fPlatformWindow; /* in streamlined version of AS */
  ODBoolean  fIsNew;
  ODBoolean fDraftIsSaved;

  HWND fDocListHWND;
  HWND fLinkListHWND;

};



#endif  // _SHELLM_
