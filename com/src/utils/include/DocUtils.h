//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
// @(#) 1.7 com/src/utils/include/DocUtils.h, odutils, od96os2, odos29646d 7/15/96 18:00:08 [ 11/15/96 15:28:58 ]
/*
	File:		DocUtils.h

	Contains:	interface for useful high level UI oriented document utilities

	Owned by:	Tantek ‚elik

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Notes:

	These functions are a collection of utilities which are used by the OpenDoc Shell
	and other subsystems to manipulate user documents.
	
	They will also be useful to part editors which manipulate documents directly,
	e.g. if a part editor wants to open another document in its process,
	or create a new document from part/document stationery.
	
	The functions also help container part editors implement the "InsertÉ" Document
	menu item.
	
	To Do:
	
		Reorder functions hilevel first, lowlevel last.
		Move NewDocument from Shell to here.

*/

#ifndef _DOCUTILS_
#define _DOCUTILS_

// for ODFileRefNum
#ifndef _PLFMFILE_
#include "PlfmFile.h"
#endif

#ifndef SOM_ODWindow_xh
#include "Window.xh"
#endif

//==============================================================================
// Classes used by this interface
//==============================================================================

class Environment;
class ODSession;

class ODPart;

class ODStorageUnit;
class ODDraft;
class ODDocument;
class ODContainer;
class PlatformFile;

//==============================================================================
// MACROS
//==============================================================================

#define RETURN_IF_NULL(x,y)	if (!(x)) return y;
#define HAS_WRITE_ACCESS(x) ((x) >= kODDPSharedWrite)

extern "C" {

//==============================================================================
// Opening an file/container/document/draft
//==============================================================================

//------------------------------------------------------------------------------
// ODAcquireCtrDocTopDraft
//	Given a PlatformFile object, get the container, document and top draft (readonly)
//  reget the draft if you need more permissions.
//------------------------------------------------------------------------------

void ODAcquireCtrDocTopDraft(Environment* ev, ODSession* session, PlatformFile* file, 
		ODContainer** container, ODDocument** document, ODDraft** draft);

//------------------------------------------------------------------------------
// ODGetFirstOpenDocument
//	A pointer to the first document in the collection of open documents is returned.
//	If the collection is empty, then kODNULL is returned.
//	It is the caller's responsibility to Acquire the returned document it she wishes to 
//	hold onto it.
//------------------------------------------------------------------------------

ODDocument*	ODGetFirstOpenDocument(Environment* ev, ODSession* session);
				
//------------------------------------------------------------------------------
// ODGetNthOpenDocument
//	A pointer to the nth document in the collection of open documents is returned.
//	If the collection is empty, then kODNULL is returned.
//	It is the caller's responsibility to Acquire the returned document it she wishes to 
//	hold onto it.
//------------------------------------------------------------------------------

ODDocument*	ODGetNthOpenDocument(Environment* ev, ODSession* session, ODULong n);
				
//------------------------------------------------------------------------------
// ODGetOpenDocumentFromRefNum
//	Given a refnum, if it refers to a document open in this process, then a pointer to
//	the respective ODDocument object is returned, else kODNULL is returned.
//	It is the caller's responsibility to Acquire the returned document it she wishes to 
//	hold onto it.
//------------------------------------------------------------------------------

ODDocument*	ODGetOpenDocumentFromRefNum(Environment* ev, ODSession* session,
				ODFileRefNum refnum);
				
//------------------------------------------------------------------------------
// ODGetTempDraftFromOpenDocument
//	Given a pointer to an already open ODDocument which has a tempdraft with rw perms,
//	a pointer to that tempdraft is returned.  
//	If there no tempdraft is found then kODNULL is returned.
//	It is the caller's responsibility to Acquire the returned tempdraft it she wishes to 
//	hold onto it.
//------------------------------------------------------------------------------
				
ODDraft*	ODGetTempDraftFromOpenDocument(Environment* ev, ODSession* session,
				ODDocument* document);

//------------------------------------------------------------------------------
// ODTempDraftCreated
//	This method updates the appropriate internal namespaces to note that a 
//	tempdraft has been created and opened on the document passed in.
//  The caller is responsible for properly refcounting the tempdraft.
//------------------------------------------------------------------------------
				
void	ODTempDraftCreated(Environment* ev, ODSession* session, 
			ODDocument* document, ODDraft* tempDraft);

//------------------------------------------------------------------------------
// ODDocumentOpened
//	This method updates the appropriate internal namespaces to note that an ODDocument
//	has been opened with the given file refnum.  If a tempdraft was created and opened
//	with rw perms, then that should also be passed in, else pass in kODNULL.
//	This method bumps the refcount of document and its container in order to keep a valid
//	reference to it.  The caller is responsible for properly refcounting the tempdraft.
//	This function calls ODTempDraftCreated as part of its implementation;
//	no need to also call ODTempDraftCreated if you call this function.
//------------------------------------------------------------------------------
				
void		ODDocumentOpened(Environment* ev, ODSession* session,
				ODFileRefNum refnum, ODDocument* document, ODDraft* tempDraft);
				
//------------------------------------------------------------------------------
// ODDocumentClosed
//	This method updates the appropriate internal namespaces to note that an ODDocument
//	has been closed with the given file refnum.
//	This method simply removes the appropriate entries from the namespaces;
//	The document is released, as is its container.
//	It is up to the caller to make sure that the tempdraft if any
//	is released.
//------------------------------------------------------------------------------
				
void		ODDocumentClosed(Environment* ev, ODSession* session,
				ODDocument* document);

//==============================================================================
// Document Properties
//==============================================================================

//------------------------------------------------------------------------------
// ODResetDateModByInfo
//  Use this call when creating a new draft, either a new docuemnt from stationery,
//	or a new saved draft.
//  If it's a new document, then after calling SetBaseDraftFromForeignDraft on the new document,
//  you need to reset the creation date, modification date, modification by (username) info
//  since the person who created the new document may not necessarily be the same person
//  who created the stationery.
//------------------------------------------------------------------------------

void ODResetDateModByInfo(Environment* ev, ODStorageUnit* su);

//------------------------------------------------------------------------------
// ODGetDocumentFileName
//	Use this call to retrieve the name of the file of the document.
//	fileName is presumed to be preallocated array of 256 characters.
//	Maximum length of fileName filled in is 255 characters (256th reserved for
//  terminating null).
//------------------------------------------------------------------------------

void ODGetDocumentFileName(Environment* ev, ODDocument* document, char* fileName);

//==============================================================================
// Root Part/StorageUnit/Draft
//==============================================================================

//------------------------------------------------------------------------------
// ODAcquireRootPartOfDraft
//  Given a draft object, get the root part
//------------------------------------------------------------------------------


ODPart* ODAcquireRootPartOfDraft(Environment* ev, ODDraft* draft);

//------------------------------------------------------------------------------
// ODAcquireRootPartSUOfDraft
//  Given a draft object, get the root part's storageUnit without getting the part
//------------------------------------------------------------------------------

ODStorageUnit* ODAcquireRootPartSUOfDraft(Environment* ev, ODDraft* draft);


//------------------------------------------------------------------------------
// ODSetRootPartOfDraft
//  Given a draft object and the storageUnit of a part, set that part to be
//  the root part of the draft.
//  Preconditions:
//		Write permissions on the draft.
//		No current root part in the draft*.
//
// *Calling this function when there already is a root part, not represented by the
// rootPartSU passed in, will cause data loss in the document.
//------------------------------------------------------------------------------

void ODSetRootPartSUOfDraft(Environment* ev, ODDraft* draft, ODStorageUnit* rootPartSU);

//------------------------------------------------------------------------------
// ODGetDraftOfWindow
//  Given a window object, get the root part's storageUnit's draft
//------------------------------------------------------------------------------

ODDraft* ODGetDraftOfWindow(Environment* ev, ODWindow* window);


//==============================================================================
// Cursor Utils
//==============================================================================

void WatchCursor();

void ArrowCursor();

//==============================================================================
// Active/Changed Utils
//==============================================================================

ODWindow*	ODAcquireActiveWindow(Environment* ev, ODSession* session);
ODDraft*	ODGetActiveDraft(Environment* ev, ODSession* session);
ODDocument*	ODGetActiveDocument(Environment* ev, ODSession* session);

ODBoolean	ODDocumentHasChanges(Environment* ev, ODSession* session,
				ODDocument* document);
ODBoolean	ODDocumentHasWriteAccess(Environment* ev, ODSession* session,
				ODDocument* document);
ODBoolean	ODDraftHasWriteAccess(Environment* ev, ODDraft* draft);

//==============================================================================
// User level document operations
//==============================================================================

//------------------------------------------------------------------------------
// ODNewDocument
//  Given a valid container, this function turns it into a valid New OpenDoc document
//	By creating a root part with the editor and/or kind passed inCreates a new document
//	It is the caller's responsibility to make sure that the container can be written to.
//------------------------------------------------------------------------------

void ODNewDocument(Environment* ev, ODContainer* container,
							ODType		rootPartKind,
							ODEditor	rootPartEditor);

//------------------------------------------------------------------------------
// ODIsUnsavedNewDocument
//  Returns whether or not the document has been saved since it was created.
//------------------------------------------------------------------------------

ODBoolean ODIsUnsavedNewDocument(Environment* ev, ODSession* session, ODDocument* document);

//------------------------------------------------------------------------------
// ODSetIsUnsavedNewDocument
//  Sets the fact that the document has not been saved since it was created.
//------------------------------------------------------------------------------

void ODSetIsUnsavedNewDocument(Environment* ev, ODDraft* draft);

#if !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
//------------------------------------------------------------------------------
// ODOpenFileDocument
//	Opens the document specified in the passed in file.
//	If write permissions are requested (and possible), then a tempdraft is created,
//	otherwise the document is opened readonly.
//	This method calls ODDocumentOpened appropriately.
//  The "current" draft is returned, all ready to be sent into an ODOpenDraft call
//	Note that if a tempdraft was created, that is what is returned as the "current" draft.
//	The draft returned is a refcounted reference that the caller owns.
//------------------------------------------------------------------------------

ODDraft* ODOpenFileDocument(Environment* ev, ODSession* session, PlatformFile* file,
			ODDraftPermissions permissions);

#endif // !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
 
//------------------------------------------------------------------------------
// ODOpenDraft
//	Notifies all appropriate objects that this draft is opening.
//	The draft's document must have already been opened with ODOpenDocument.
//  E.g. the LinkManager, Undo, Clipboard, WindowState (opens windows on that draft) etc.
//  The draft is not acquired, nor is it added to the tempdrafts collection. 
//------------------------------------------------------------------------------

void ODOpenDraft(Environment* ev, ODSession* session, ODDraft* draft);

//------------------------------------------------------------------------------
// ODCloseDraft
//	Notifies all appropriate objects that this draft is closing.
//  E.g. the LinkManager, Undo, Clipboard, WindowState (closes windows on that draft) etc.
//  The draft is not released, nor is it removed from the tempdrafts collection. 
//------------------------------------------------------------------------------

void ODCloseDraft(Environment* ev, ODSession* session, ODDraft* draft);

//------------------------------------------------------------------------------
// ODCloseDocument
//	Closes the document and any associated windows etc.
//	Caller is responsible for saving changes, prompting the user etc. BEFORE calling.
//	Document is released.
//	It is removed from the list of open documents, and if it has a tempdraft,
//	that too is removed from the tempdrafts collection.
//	Removes any tempdraft and any changes therein which implicitly releases the tempdraft.
//	If there are no more open windows once the document is closed,
//	then kODTrue is returned.
//------------------------------------------------------------------------------

ODBoolean ODCloseDocument(Environment* ev, ODSession* session, ODDocument* document);

//------------------------------------------------------------------------------
// ODReleaseCloseWindow
//	Releases the window and
//	Closes the window and any dependent windows.
//	All child windows are dependent on the root windows of their draft.
//	All windows of a saved draft of a document are dependent on the root windows 
//	of the "current/top/temp" draft of the document.
//	If the last window of a document is closed, then ODCloseDocument is called.
//	If there are no more open windows once the window is closed,
//	then kODTrue is returned.
//	This function MUST consume the window reference passed in because:
//	1. The Windowstate hangs onto a reference to every window
//	2. The window's frame hangs onto a reference to the window
//	2. The window passed in is AT LEAST the third reference to that window
//	3. If the window is that last root window of the draft,
//	   then the window MUST be fully released by this function so that
//	   the draft, document and container can be properly released by ODCloseDocument
//	   which as noted above, is called by this function.
//------------------------------------------------------------------------------

ODBoolean ODReleaseCloseWindow(Environment* ev, ODSession* session, ODWindow* window);
 
#if !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)

//------------------------------------------------------------------------------
// ODDeleteDocument
//	Saves & closes the document and moves it to the trash.
//	If there are no more open windows once the document is closed,
//	then kODTrue is returned.
//------------------------------------------------------------------------------

ODBoolean ODDeleteDocument(Environment* ev, ODSession* session, ODDocument* document);

//------------------------------------------------------------------------------
// ODSaveDocument
//	Saves the document if there is an associated tempdraft.  Otherwise does nothing.
//------------------------------------------------------------------------------

void ODSaveDocument(Environment* ev, ODSession* session, ODDocument* document);

#endif  // !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
//------------------------------------------------------------------------------
// ODSaveACopyOfDraft
//	Saves a copy of the draft.
//	Note: destinationFile must be Specified but should not Exist.
//------------------------------------------------------------------------------

void ODSaveACopyOfDraft(Environment* ev, ODSession* session, ODDraft* draft, 
						PlatformFile* destinationFile);

#if !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
//------------------------------------------------------------------------------
// ODRevertDocument
//	Reverts the document if there is an associated tempdraft.  Otherwise does nothing.
//  Note: it is advised that you get a confirmation from the user BEFORE
//	calling this function.
//------------------------------------------------------------------------------

void ODRevertDocument(Environment* ev, ODSession* session, ODDocument* document);
#endif // !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)

//------------------------------------------------------------------------------
// ODDocumentInfo
//	Displays the document info dialog.
//------------------------------------------------------------------------------

void ODDocumentInfo(Environment* ev, ODSession* session);

//------------------------------------------------------------------------------
// ODIsTempDocument
//  Returns whether or not the document is a temp document.
//------------------------------------------------------------------------------

ODBoolean ODIsTempDocument(Environment* ev, ODSession* session, ODDocument* document);

//------------------------------------------------------------------------------
// ODSetIsTempDocument
//  Sets whether the document is a temp document or not.
//------------------------------------------------------------------------------

void ODSetIsTempDocument(Environment* ev, ODDraft* draft, ODBoolean isTemp);

}	// End of extern "C" {

#endif
// _DOCUTILS_
