/* @(#)Z 1.100 com/src/docshell/DocMgr.cpp, odshell, od96os2, odos29646d 96/11/15 15:49:37 (96/10/29 09:23:34) */

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
// DocMgr.cpp
//
//	This file provides the implementation of the DocumentManager class.
//	An instance of this class is created for each document which is opened
//	or created.
//
//------------------------------------------------------------------------------

#ifndef SOM_Module_docmgr_Source
#define SOM_Module_docmgr_Source
#endif
#define DocumentManager_Class_Source

#include <time.h>
#include <ODTypes.h>
#include <ODUtils.h>
#include <StorUtil.h>
#include <DocUtils.h>
#include <StdProps.xh>
#include <StdTypes.xh>
#include <StdTypIO.h>
#include <ODSessn.xh>
#include <ODStor.xh>
#include <ODCtr.xh>
#include <Document.xh>
#include <Part.xh>
#include <StorageU.xh>
#include <Undo.xh>
#include <WinIter.xh>
#include <LinkMgr.xh>
#ifdef _PLATFORM_OS2_
#include <IODDefs.xh>
#include <ILnkSrvc.xh>
#endif
#include <Except.h>
#include <Clipbd.xh>
#include <DragDrp.xh>
#include <Disptch.xh>
#include <ODPrtRei.xh>
#include "DraftRec.h"
#include <TempObj.h>
#include <EditrSet.h>
#include <ISOStr.h>
#include <BndNSUtl.h>
#include <BArray.h>
#include "ShPInLst.h"
#include "DocShell.h"

#ifdef _PLATFORM_WIN32_
#include <olewrap.h>
#include <olemgrw.h>
#endif

#define VARIABLE_MACROS
#include "DocMgr.xih"

#ifdef _PLATFORM_WIN32_
#include <objidl.h>
#endif

#define UNTITLED_FILENAME "NewDoc.od"
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#define FILE_LENGTH 256

//------------------------------------------------------------------------------
// Initialize
//
//	Initialize a newly created DocumentManager
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerInitialize(DocumentManager *somSelf, 
                                        Environment *ev, ODSession* session)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager", "DocumentManagerInitialize");

    try
    {
	// Cache references to the session and window state
        _fSession = session;
        if (_fSession)
            _fWindowState = session->GetWindowState(ev);

	// Set the draft permissions to the default
        _fDraftPermissions = kODDPExclusiveWrite;
 
        // Initialize file name and root kind name
        _fFileName[0] = kODNULL;
        _fPartKindName[0] = kODNULL;

        // Initialize to existing document
        _fUnsavedDocument = kODFalse;

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::Initialize");
	ODSetSOMException(ev, _exception);
    }
}

//------------------------------------------------------------------------------
// OpenDocumentFromFile
//
//	Opens a document that exists on disk
//------------------------------------------------------------------------------
SOM_Scope char*  SOMLINK DocumentManagerOpenDocumentFromFile(
					DocumentManager *somSelf, 
                                        Environment *ev, char *fileName,
					ODUShort actionFlag)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
					"DocumentManagerOpenDocumentFromFile");

    char* rootPartKind = kODNULL;
    PlatformFile* file = new PlatformFile;

    try
    {

        // Set the file name  -  need error handling for file existence  - RLC
        file->Specify((ODFileSpec *)fileName);

#ifdef _PLATFORM_WIN32_
	// Setup linking by registering this filename
	somSelf->GetOleWrapper(ev)->RegisterFilename(fileName, FALSE);
#endif

        // Get the document from the file
        somSelf->OpenFile(ev, file, actionFlag);

        // Install Shell Plug-in
        InstallShellPlugIn(ev, _fDraft);

#if defined(_PLATFORM_OS2_)
           _fSession->GetLinkService(ev)->
                           DraftOpened(ev, _fDraft, kODFalse, fileName);
#endif

        // Open the root part of the document
        rootPartKind = somSelf->OpenRootPart(ev);

	// Display window title
        _fWindowState->SetDefaultWindowTitles(ev, _fDraft);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::OpenDocumentFromFile");
	ODSetSOMException(ev, _exception);
    }

    // Cache the kindname and filename
    // Note: _fFileName is already set if create a new document off stationery
    if (fileName && (_fFileName[0] == kODNULL))
       strcpy(_fFileName, fileName);
    if (rootPartKind)
       strcpy(_fPartKindName, rootPartKind);
    delete file;

    return rootPartKind;

}

//------------------------------------------------------------------------------
// OpenDocumentFromMemory
//
//	Opens a document that exists in memory
//------------------------------------------------------------------------------
SOM_Scope char*  SOMLINK DocumentManagerOpenDocumentFromMemory(
					DocumentManager *somSelf, 
                                        Environment *ev,
					ODHandle handle)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerOpenDocumentFromMemory");

    char* rootPartKind = kODNULL,
          *rootPartNameStr = kODNULL;
    ODIText *rootPartName = kODNULL;

    try
    {
        // Get the container from the given memory location
        _fContainer = GetMemoryContainer(ev, _fSession, handle, 
					kODDefaultMemoryContainer);

        // Get the newly created document from the memory container
        somSelf->OpenContainer(ev, _fContainer);

        // Install Shell Plug-in
        InstallShellPlugIn(ev, _fDraft);

        // Open the root part of the document
        rootPartKind = somSelf->OpenRootPart(ev);

        // Display root part kind as the window title
        // Note: If kODPropName is not set by the part, then use the
        //       rootPartKind returned from OpenRootPart as the text
        //       in window title. 
        TempODStorageUnit rootSU = ODAcquireRootPartSUOfDraft(ev, _fDraft);
        rootPartName = ODGetITextProp(ev, rootSU, kODPropName, 
                                               kODPlatformIText, kODNULL);
        if (rootPartName)
        {
           ODULong strLength = GetITextStringLength(rootPartName);
           rootPartNameStr = new char[strLength+1];
           GetITextString(rootPartName, rootPartNameStr);
        }
        else
        {
           rootPartNameStr = new char[strlen(rootPartKind)+1];
           strcpy(rootPartNameStr, rootPartKind);
        }
        
        if (rootPartNameStr) 
        {
           _fWindowState->SetLongName(ev, rootPartNameStr);
           _fWindowState->SetDefaultWindowTitles(ev, _fDraft);
        }

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::OpenDocumentFromMemory");
        ODError error = ErrorCode();
        if (_fContainer)
           SaveAndRestoreEv1(_fContainer);
        ODSetSOMException(ev, error);
    }
     
    // Cache the kindname
    if (rootPartKind)
       strcpy(_fPartKindName, rootPartKind);

    if (rootPartNameStr)
       delete[] rootPartNameStr;

    if (rootPartName)
       DisposeIText(rootPartName);

    return rootPartKind;
}

//------------------------------------------------------------------------------
// CreateDocumentInFile
//
//	Create a document in a new file container
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCreateDocumentInFile(
					DocumentManager *somSelf, 
                                        Environment *ev, ODType kindName,
					char *filename)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerCreateDocumentInFile");

    PlatformFile *file = kODNULL;

    // Cache the kindname and filename
    strcpy(_fFileName, filename);
    strcpy(_fPartKindName, kindName);

    try
    {
        // Create a new container in a file - filename may be "uniquified"
        file = somSelf->NewFileContainer(ev, &_fContainer, _fFileName);
        file->GetAsciiName(_fFileName, FILE_LENGTH - 1);

        // Create a new document in the new container and externalize it
        somSelf->CreateDocument(ev, _fContainer, kindName);

        // Release the new container
        // Note: Need to release the container which is acquired in
        //      CreateFileContainer.
        //      The call to GetFileContainer in OpenFile will acquire
        //      the created container so the refcount for the container
        //      remains 1.
        ODReleaseObject(ev, _fContainer);

#ifdef _PLATFORM_WIN32_
	// Setup linking by registering this filename
	// char fileName[_MAX_PATH];
	// file->GetAsciiName(fileName, _MAX_PATH);
	somSelf->GetOleWrapper(ev)->RegisterFilename(_fFileName, FALSE);
#endif

        // Get the newly created document from the file
        somSelf->OpenFile(ev, file, kODNULL);

        // Install Shell Plug-in
        InstallShellPlugIn(ev, _fDraft);

        // Open the root part of the document
        somSelf->OpenRootPart(ev);

        // Sync up window titles with file name
        _fWindowState->SetDefaultWindowTitles(ev, _fDraft);

        // Mark document as unsaved document
        // Note: Should mark this as unsaved document only if working
        // with file container, not for memory container.
        _fUnsavedDocument = kODTrue;
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CreateDocumentInFile");
        ODError error = ErrorCode();
        if (_fContainer)
           SaveAndRestoreEv1(_fContainer);
        ODSetSOMException(ev, error);
    }

    if (file)
       delete file;

}

//------------------------------------------------------------------------------
// CreateDocumentInMemory
//
//	Create a document in a new memory container
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCreateDocumentInMemory(
					DocumentManager *somSelf, 
                                        Environment *ev, ODType kindName)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerCreateDocumentInMemory");

    char *rootPartNameStr = kODNULL;
    ODIText *rootPartName = kODNULL;

    // Cache the kindname
    strcpy(_fPartKindName, kindName);

    try
    {
        // Create a new memory container
        somSelf->NewMemoryContainer(ev, &_fContainer);

        // Create a new document in the new container and externalize it
        somSelf->CreateDocument(ev, _fContainer, kindName);

        // Get the newly created document from the memory container.
        //
        // Note: For memory container, the container is not acquired
        //      again in OpenMemoryContainer. So the container created
        //      in CreatedMemoryContainer is not released.
        somSelf->OpenContainer(ev, _fContainer);

        // Install Shell Plug-in
        InstallShellPlugIn(ev, _fDraft);

        // Open the root part of the document
        char *rootPartKind = somSelf->OpenRootPart(ev);

        // Display root part kind as the window title
        // Note: If kODPropName is not set by the part, then use the
        //       rootPartKind returned from OpenRootPart as the text
        //       in window title. 
        TempODStorageUnit rootSU = ODAcquireRootPartSUOfDraft(ev, _fDraft);
        rootPartName = ODGetITextProp(ev, rootSU, kODPropName, 
                                               kODPlatformIText, kODNULL);
        if (rootPartName)
        {
           ODULong strLength = GetITextStringLength(rootPartName);
           rootPartNameStr = new char[strLength+1];
           GetITextString(rootPartName, rootPartNameStr);
        }
        else
           // Note: Use rootPartKind returned from OpenRootPart not 
           //       kindName passed in because OpenRootPart may fail
           //       and open NoPart instead.
           rootPartNameStr = rootPartKind;
        
        if (rootPartNameStr) 
        {
            _fWindowState->SetLongName(ev, rootPartNameStr);
            _fWindowState->SetDefaultWindowTitles(ev, _fDraft);
        }
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CreateDocumentInMemory");
        ODError error = ErrorCode();
        if (_fContainer)
           SaveAndRestoreEv1(_fContainer);
        ODSetSOMException(ev, error);
    }

    if (rootPartNameStr)
       delete[] rootPartNameStr;

    if (rootPartName)
       DisposeIText(rootPartName);
}

//------------------------------------------------------------------------------
// CreateDocumentAsStationery
//
//	Create a document in a new file container and mark the document as
//      stationery
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCreateDocumentAsStationery(
					DocumentManager *somSelf, 
                                        Environment *ev, ODType kindName,
					char *filename)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerCreateDocumentAsStationery");

    PlatformFile *file = kODNULL;

    // Cache the kindname and filename
    strcpy(_fFileName, filename);
    strcpy(_fPartKindName, kindName);

    try
    {
        // Create a new container in a file - filename may be "uniquified"
        file = somSelf->NewFileContainer(ev, &_fContainer, _fFileName);
        file->GetAsciiName(_fFileName, FILE_LENGTH - 1);

        // Create a new document in the new container and externalize it
        somSelf->CreateDocument(ev, _fContainer, kindName);

        // Acquire new document and draft. CreateDocument method releases
        // the document and draft it acquired before returned to caller.
        _fDocument = _fContainer->AcquireDocument(ev, kODDefaultDocument);
        _fDraft = _fDocument->AcquireBaseDraft(ev, kODDPExclusiveWrite);

        // Mark the document as stationery
        somSelf->SetStationery(ev, _fDraft);

        // Write out draft
        _fDraft->Externalize(ev);

        ODReleaseObject(ev, _fDraft);
        ODReleaseObject(ev, _fDocument);
        ODReleaseObject(ev, _fContainer);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CreateDocumentAsStationery");
        ODError error = ErrorCode();
        if (file)
           delete file;
        if (_fDraft || _fDocument || _fContainer)
           SaveAndRestoreEv3(_fDraft, _fDocument, _fContainer);
        ODSetSOMException(ev, error);
    }

}

//------------------------------------------------------------------------------
// NewFileContainer
//
//	Create a new file container with a unique name
//------------------------------------------------------------------------------
SOM_Scope PlatformFile*  SOMLINK DocumentManagerNewFileContainer(
					DocumentManager *somSelf, 
                                        Environment *ev, 
					ODContainer **container,
					char *filename)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
					"DocumentManagerNewFileContainer");

    PlatformFile *file = kODNULL;

    try
    {
        // Create a uniquely named file
        file = somSelf->CreateUntitledFile(ev, filename);

	// If problem creating untitled file in current directory, create
	// untitled file in directory set in ODTMP.
	if (!file)
		file = somSelf->CreateUntitledFile(ev, _fFileName);

        // Create a new file container
        if (file)
	    (*container) = CreateFileContainer(ev, _fSession, 
						&(file->GetFileSpec()));

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::NewFileContainer");
        if (file)
           delete file;
        file = kODNULL;
	ODSetSOMException(ev, _exception);
    }

    return file;
}

//------------------------------------------------------------------------------
// NewMemoryContainer
//
//	Create a new memory container
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerNewMemoryContainer(
						DocumentManager *somSelf,
						Environment *ev,
						ODContainer** container)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
					"DocumentManagerNewMemoryContainer");
    ODHandle containerHandle;

    try
    {
        containerHandle = ODNewHandle(0);
        ASSERTMSG((containerHandle != 0), kODErrOpeningMemContainer,
		 "Error allocating document memory container",AMSG_500);

        *container = CreateMemoryContainer(ev, _fSession, containerHandle,
				kODDefaultMemoryContainer);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::NewMemoryContainer");
        if (containerHandle)
            ODDisposeHandle(containerHandle);
        *container = kODNULL;
	ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// CreateDocument
//
//	Initialize a document in a new container
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCreateDocument(DocumentManager *somSelf, 
                                                 Environment *ev, 
                                                 ODContainer* newContainer,
						 ODType partKind)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
					"DocumentManagerCreateDocument");

    try
    {
        // Get the document from the container and base draft from the document
        TempODDocument newDocument = newContainer->AcquireDocument(ev, 
						kODDefaultDocument);

	// Set the current document so that when GetDocumentManagerOfDraft
	// is called the current document is set.
	_fDocument = newDocument;

        TempODDraft newDraft = newDocument->AcquireBaseDraft(ev, 
                                                         kODDPExclusiveWrite);

        // Modify timestamp and user properties for draft
        TempODStorageUnit draftSU = newDraft->AcquireDraftProperties(ev);
        somSelf->ResetDateModByInfo(ev, draftSU);

        // Create the root part
        TempODPart rootPart = newDraft->CreatePart(ev, partKind, kODNULL);
        TempODStorageUnit su = rootPart->GetStorageUnit(ev);
	if (su)
	{
		// Need to acquire the storage unit. The storage unit
		// returned by GetStorageUnit does not increment the
		// refcount.
		su->Acquire(ev);
	        ODSetRootPartSUOfDraft(ev, newDraft, su);
	}

        // Modify timestamp and user properties for root part
        somSelf->ResetDateModByInfo(ev, su);

        // Close the document (can't FinalRelease the part since it may have a
        // refcount > 1 upon creation, e.g. some might register for idle time, 
        // etc.
        newDraft->Externalize(ev);

	// Reset the current document to null
	_fDocument = kODNULL;

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CreateDocument");
	ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// OpenFile
//
//	Get the container, document and top draft from the platform file.
//	If we've got stationery then open a copy of the file instead.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerOpenFile(DocumentManager *somSelf, 
                                                 Environment *ev, 
                                                 PlatformFile* file,
						 ODUShort actionFlag)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerOpenFile");

    PlatformFile *newFile = kODNULL;

    try
    {
        ODFileAttrFlags fileAttrs;

        // If file is readonly, set file draft permission to read only.
        file->GetFileAttributes(&fileAttrs);
        if (fileAttrs & kODFileAttrReadOnly)
           _fDraftPermissions = kODDPReadOnly;

        // Get the container, document, and top draft
        _fContainer = GetFileContainer(ev, _fSession, &(file->GetFileSpec()));
        _fDocument = _fContainer->AcquireDocument(ev, kODDefaultDocument);

	// Setting draft permission to read so that a new version won't be
	// created. 
        _fDraft = _fDocument->AcquireDraft(ev,  kODDPReadOnly, 0, 
					kODNULL, kODPosTop, kODFalse);
    }
    catch (ODException _exception)
    {
       	char  fileName[FILE_LENGTH];
       	file->GetAsciiName(fileName, FILE_LENGTH - 1); 
        
        ODError error = ErrorCode();

        if (error == kODErrDraftDoesNotExist)
	   WARNMSG(WARN_INDEX(AMSG_901), 
		"CPPOD0404 - Docshell: File %s is not a valid OpenDoc file.", fileName);
        else
	   WARNMSG(WARN_INDEX(AMSG_862), 
		"CPPOD0347 - Docshell: Error opening file %s.", fileName);

        if (_fDraft || _fDocument || _fContainer)
           SaveAndRestoreEv3(_fDraft, _fDocument, _fContainer);
        ODSetSOMException(ev, error);
	return;
    }

    try
    {
        // If file is a stationery and not a request to open the stationery, 
        // then save a copy of the stationery to a new file.
        if ((somSelf->IsStationery(ev, _fDraft)) && 
            (actionFlag != kODEditStationery))
        {
           newFile = somSelf->CopyStationery(ev);
           somSelf->OpenFile(ev, newFile, kODNULL);

       	   char  fileName[FILE_LENGTH];
           // Cache the new file name
           newFile->GetAsciiName(fileName, FILE_LENGTH - 1); 
           strcpy(_fFileName, fileName);

           delete newFile;
           newFile = kODNULL;
        }
        else
           // Create working draft
           somSelf->CreateWorkingDraft(ev);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::OpenFile");
        ODError error = ErrorCode();
        if (newFile)
           delete newFile;
        if (_fDraft || _fDocument || _fContainer)
           SaveAndRestoreEv3(_fDraft, _fDocument, _fContainer);
        ODSetSOMException(ev, error);
    }
}


//------------------------------------------------------------------------------
// CopyStationery
//
//	Create a new document file and save the top most draft of the 
//      stationery to the new document.
//------------------------------------------------------------------------------
SOM_Scope PlatformFile* SOMLINK DocumentManagerCopyStationery(
                                                 DocumentManager *somSelf, 
                                                 Environment *ev) 
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerCopyStationery");

    PlatformFile *newFile = kODNULL;

    try
    {
       ODContainer *tmpContainer;
       TempODContainer stationeryContainer = _fContainer;
       TempODDocument  stationeryDocument = _fDocument;
       TempODDraft     stationeryDraft = _fDraft;

       _fContainer = kODNULL;
       _fDocument  = kODNULL;
       _fDraft     = kODNULL;

       // Reset draft permission to the default value
       _fDraftPermissions = kODDPExclusiveWrite;

       // Create a new file container
       newFile = somSelf->NewFileContainer(ev, 
				&tmpContainer, UNTITLED_FILENAME);
       TempODContainer newContainer = tmpContainer;

       // Create a new document in the newly created file container
       TempODDocument newDoc = newContainer->AcquireDocument(ev, 
                                              kODDefaultDocument);

       // Copy the topmost draft of the stationery to the base draft
       // of the new document
       newDoc->SetBaseDraftFromForeignDraft(ev, stationeryDraft);

       // Get a reference of the base draft with write permission
       TempODDraft newDraft = newDoc->AcquireBaseDraft(ev, kODDPExclusiveWrite);
	
       // Remove stationery property
       somSelf->UnsetStationery(ev, newDraft);

       // Modify timestamp and user properties
       {
           TempODStorageUnit rootsu = ODAcquireRootPartSUOfDraft(ev, 
								newDraft);
           // Note: Need to get the root part kind before calling
           // ResetDateModByInfo
           char *partKindName = ODGetKindFromPartSU(ev,rootsu);
           if (partKindName)
              strcpy(_fPartKindName, partKindName);
           somSelf->ResetDateModByInfo(ev, rootsu); 

           TempODStorageUnit su = newDraft->AcquireDraftProperties(ev);
           somSelf->ResetDateModByInfo(ev, su); 
       }

       // Save the base draft 
       newDraft->Externalize(ev);

   }
   catch (ODException _exception)
   {
      WARNMSG(WARN_INDEX(AMSG_863), 
		"CPPOD0348 - Docshell: Error creating file from stationery.");
      if (newFile)
         delete newFile;
      newFile = kODNULL;
      ODSetSOMException(ev, _exception);
   }

   return newFile;
}

//------------------------------------------------------------------------------
// OpenContainer
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerOpenContainer(
                                                DocumentManager *somSelf,
                                                 Environment *ev,
                                                 ODContainer* container)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                        "DocumentManagerOpenContainer");

    try
    {
        // Get the document and top draft from the memory container
        _fContainer = container;
        _fDocument = _fContainer->AcquireDocument(ev, kODDefaultDocument);
        _fDraft = _fDocument->AcquireDraft(ev, _fDraftPermissions, 0, kODNULL,
                                        kODPosTop, kODFalse);

        // Create working draft
        somSelf->CreateWorkingDraft(ev);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::OpenContainer");
        ODError error = ErrorCode();
        if (_fDraft || _fDocument || _fContainer)
           SaveAndRestoreEv3(_fDraft, _fDocument, _fContainer);
        ODSetSOMException(ev, error);
    }

}


//------------------------------------------------------------------------------
// CreateWorkingDraft
//
//	Create a working draft on top of current draft
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCreateWorkingDraft(
                                           DocumentManager *somSelf,
                                           Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                 "DocumentManagerCreateWorkingDraft");

    try
    {
        // Create working draft on top of current most draft so that  
        // saving a modified draft which has a draft created on it
        // before the changes won't corrupt the draft.
        if (somSelf->HasWriteAccess(ev))
        {
           _fDraft = _fDocument->CreateDraft(ev, _fDraft, kODTrue);

	   if (_fDraft->GetPermissions(ev) != _fDraftPermissions)
              // Release the current draft and get a reference of the
              // draft again with the specified draft permission
              //
              // Note: The new working draft is set to have kODDPExclusiveWrite
	      //	permission by default.
              _fDraft = _fDocument->AcquireDraft(ev, _fDraftPermissions, 
				kODNULLID, _fDraft, kODPosSame, kODTrue);

	   // Notify link manager that this draft is opened
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
	   _fSession->GetLinkManager(ev)->DraftOpened(ev, _fDraft);
#elif defined(_PLATFORM_OS2_)
           _fSession->GetLinkService(ev)->
                           DraftOpened(ev, _fDraft, kODFalse, _fFileName);
#endif 
        }

   }
   catch (ODException _exception)
   {
        LOG("SOM exception occured in DocumentManager::CreateWorkingDraft");
	ODSetSOMException(ev, _exception);
   }

}


//------------------------------------------------------------------------------
// OpenRootPart
//
//	Acquire the root part of the document and invoke its Open method
//------------------------------------------------------------------------------
SOM_Scope char*  SOMLINK DocumentManagerOpenRootPart(DocumentManager *somSelf,
                                             Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerOpenRootPart");

    char *rootPartKind = kODNULL;

    try
    {
        // Open the root part
        _fWindowState->Internalize(ev, _fDraft);
        _fWindowState->OpenWindows(ev, _fDraft);
        if (_fWindowState->GetRootWindowCount(ev, _fDraft) == 0)
        {
            TempODPart rootPart = ODAcquireRootPartOfDraft(ev, _fDraft);
            rootPart->Open(ev, kODNULL);
        }

        // Get the root part kind to return
	{
		TempODStorageUnit su = ODAcquireRootPartSUOfDraft(ev, _fDraft);
	        rootPartKind = ODGetKindFromPartSU(ev,su);
	}

#ifdef DEBUG
	ODULong refcount = _fDraft->GetRefCount(ev);
#endif

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::OpenRootPart");
	ODSetSOMException(ev, _exception);
    }

    return rootPartKind;

}


//------------------------------------------------------------------------------
// CreateUntitledFile
//
//	Create a new platform file with a unique name
//------------------------------------------------------------------------------
SOM_Scope PlatformFile*  SOMLINK DocumentManagerCreateUntitledFile(
						DocumentManager *somSelf,
						Environment *ev,
						char *filename)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
					"DocumentManagerCreateUntitledFile");

    static retry = kODTrue;
    PlatformFile*	file = kODNULL;

    try
    {
        // Default file name...
	if (filename == kODNULL)
		filename = UNTITLED_FILENAME;

        // Create a new PlatformFile
        file = new PlatformFile;

        // Set the PlatformFile's name to the default
        file->Specify((ODFileSpec *)filename);

        // Create a uniquely named file
        file->UniquifyAndCreate(
			kODShellSignature,	// creator
			kODShellSignature,	// fileType
			0,			// scriptCode
			1,			// copyCount
			kODTryCurrentName);	// forceNewName
    }
    catch (ODException _exception)
    {
	delete file;
	file = kODNULL;
	if (!retry)
	{
		// If attempted to create file in ODTMP is tried, print out
		// a warning.
	        WARNMSG(WARN_INDEX(AMSG_858),"CPPOD0343 - Docshell: Problem creating file %s. Directory may be full or write-protected.", filename);
		ODSetSOMException(ev, _exception);
	}
	else
	{
		// Setup untitled file to directory set in ODTMP
		retry = kODFalse;
		char *tmppath = getenv("ODTMP");
		if (tmppath != NULL)
	       	{
			strcpy(_fFileName, tmppath);
			if (strncmp(tmppath+strlen(tmppath)-1, PATH_SEPARATOR,
					 1) != 0)
				strcat(_fFileName, PATH_SEPARATOR);
			strcat(_fFileName, UNTITLED_FILENAME);

			// Clear the error 
	       		SetErrorCodeEv(ev, kODNoError);
		}
		else
		{
			// If ODTMP is not set, print out a warning.
	        	WARNMSG(WARN_INDEX(AMSG_858),"CPPOD0343 - Docshell: Problem creating file %s. Directory may be full or write-protected.", filename);
			ODSetSOMException(ev, _exception);
		}
	}

    }

    return file;

}


//------------------------------------------------------------------------------
// SaveDocument
//
//	Save the document, taking care of Undo/Redo.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSaveDocument(DocumentManager *somSelf, 
                                                     Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerSaveDocument");

    try
    {
	if (_fSession)
	{
	    ODUndo *Undo = _fSession->GetUndo(ev);

	    // Clear the undo and redo stacks
	    if (Undo)
		Undo->ClearActionHistory(ev, kODDontRespectMarks);
	}

	// Save the draft
	somSelf->SaveDraft(ev);
	if (_fDraft)
	    // Note: After calling SaveToAPrevious, the draft is marked clean.
	    _fDraft->SaveToAPrevious(ev, kODNULL);

        // Mark it as a saved document
        _fUnsavedDocument = kODFalse;
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::SaveDocument");
	ODSetSOMException(ev, _exception);
    }
}


//------------------------------------------------------------------------------
// CloseDocument
//
//	Save document, close all draft windows belonging to this document,
//	remove working draft, and release objects.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCloseDocument(DocumentManager *somSelf, 
                                                      Environment *ev,
						      ODULong saveOption)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
					"DocumentManagerCloseDocument");

    ODWindowIterator *iter = kODNULL;
    ODWindow         *curWindow, *nextWindow;
    PlatformFile     *file; 

    try
    {

	// Saves the document if user chooses to save the changes to
	// the document before exit
	if (saveOption == kODCommandSaveChanges)
	    somSelf->SaveDocument(ev);

	// Close windows of all drafts belonging to this document
        //
        // Note: If the current window returned by the window iterator
        //	is closed, the window iterator will be out of sync. To
        // 	take care of this, start the iterator from the beginning.
        //	This is a performance hit if we've lots of windows at the
        //	beginning of the iterator that don't belong to this document. 
        iter = _fWindowState->CreateWindowIterator(ev);
	curWindow = iter->First(ev);
        while (iter->IsNotComplete(ev))
        {
            if (somSelf->CloseWindow(ev, curWindow->GetPlatformWindow(ev),
                                        kODFalse))
	       curWindow = iter->First(ev);
	    else
               curWindow = iter->Next(ev);
        }
	ODDeleteObject(iter);

	// Remove the working draft from document
	// Note: Changes made to the working draft should already be
	//	saved if user chooses to save changes before exit. In
	// 	this case, the working draft is empty. Otherwise, the
	//	changes in the working draft is discarded and the working
	//	draft is removed from the document.
	somSelf->RemoveChanges(ev, _fDraft);

        // Get name of document file to be deleted 
        if (_fUnsavedDocument)
           file = GetPlatformFileFromContainer(ev, _fContainer);

	// Release the document and container objects
	ODFinalReleaseObject(ev, _fDocument);
#if DEBUG
        ODULong refcount = _fContainer->GetRefCount(ev);
#endif
	ODFinalReleaseObject(ev, _fContainer);

        // Delete the unsaved document file
        if (_fUnsavedDocument)
           file->Delete();

#ifdef _PLATFORM_WIN32_
	// if there's a wrapper associated with the document manager,
	// tell it we're going away.  Do this last or other methods
	// above will cause another wrapper to be created through
	// the GetOleWrapper interface.
        //
        // SetOleWrapper now does an AddRef (when set) and Release 
        // when NULL'ed, so that the wrapper remains in existance until 
        // OpenDoc has closed this document. This will avoid additional
        // wrappers being instantiated just for closing this document.

	if (_fOleWrapper) {
	    _fOleWrapper->SetDocumentManager(NULL);
            somSelf->SetOleWrapper(ev, NULL);
	}
#endif

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CloseDocument");
        if (iter)
           ODDeleteObject(iter);
	ODSetSOMException(ev, _exception);
    }

}


//------------------------------------------------------------------------------
// CloseWindow
//
//	Close window belong to the current active document manager. For
//	non-root window, close the window. For root window containing
//	intermediate draft with readonly permission only, close the
//	draft and release the draft object. For root window containing
//	the working draft, if caller requests to confirm
//	the removal of the window, returns to caller. Otherwise
//	close the draft and release the draft object.
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK DocumentManagerCloseWindow(
						DocumentManager *somSelf, 
                                                Environment *ev,
						ODPlatformWindow platWindow,
						ODBoolean confirm)
{
//  if confirm is set to kODTrue, the actual closing of the draft
//  and window is not performed if the window containing the current
//  draft (stored in _fDraft) is closed. Instead this function returns 
//  kODTrue to the caller to inform caller that this is such case. The 
//  caller can then call CloseDocument to close windows belonging to this
//  document and release its draft objects.
//
//  if confirm is set to kODFalse, this function returns kODTrue if
//  closing of window(s) takes place. Otherwise this function returns
//  kODFalse.

    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerCloseWindow");

    ODDraft		*winDraft;
    ODBoolean		result = kODFalse;

    try
    {

	// Get the ODWindow for the platform window that will be closed
	if (platWindow && _fWindowState 
				&& (_fWindowState->IsODWindow(ev, platWindow)))
        {
	    TempODWindow window = _fWindowState->AcquireODWindow(ev,platWindow);
  	    ASSERTMSG(window != kODNULL, kODErrClosingNonODWindow, 
              "Docshell: Window to be closed does not belong to OpenDoc.", 
              AMSG_460);

	    // Get the draft for the ODWindow 
	    winDraft = window->GetDraft(ev);
            ASSERTMSG(winDraft != kODNULL, kODErrNoDraftInWindow, 
  		"Docshell: Window to be closed has no draft in it.", AMSG_470);

            // Get the document for the draft and determines
            // if the document belongs to the current 
            // active document manager.
            //
            // Note: The active document manager is set by the
            // activate event.
	    if (winDraft && (winDraft->GetDocument(ev) == _fDocument))
	    {
	        // Determine if it is the root draft that is being 
  	        // closed or some other window
	        ODBoolean isRootWindow = window->IsRootWindow(ev);
	        ODBoolean isDraftRootWindow = isRootWindow &&
		    (_fWindowState->GetRootWindowCount(ev, winDraft) == 1);
	        if (isDraftRootWindow)
	        {
	            // if current draft (_fDraft) is closed and 
                    // caller requests to confirm the close, 
                    // return to caller.
                    // Note: Caller should check the return code 
                    //       returned by CloseWindow. If return
                    //       code is kODTrue, then call CloseDocument
                    //       to close all draft and window objects
                    //       belonging to the current document and
                    //       close down the current active manager.
	            if (winDraft == _fDraft)
	            {
		       if (confirm)
		          result = kODTrue;
	            }

                    if (result == kODFalse)
                    {
  	                // Close the draft and release object
			// Note: the draft acquired by window is released
			//	when the window is closed.
	                somSelf->CloseDraft(ev, winDraft);
#ifdef DEBUG
	                ODULong refcnt = _fDraft->GetRefCount(ev);
#endif
			if (!confirm)
			   result = kODTrue;
	            }
    
                 }
	         else
	         {
	            // Other ODWindows, simply close and remove the 
                    // window object.
		    //
		    // Note: Need to actually acquire the window and not
		    //	releasing it because CloseAndRemove releases it.
		    window->Acquire(ev);
	            window->CloseAndRemove(ev);
		    if (!confirm)
			result = kODTrue;
	         }	
             }
        }

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CloseWindow");
	result = kODFalse;
	ODSetSOMException(ev, _exception);
    }

    return result;
}
			

//------------------------------------------------------------------------------
// CreateDraft
//
//	Save the current working draft and create a new working draft
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCreateDraft(DocumentManager *somSelf, 
                                                Environment *ev,
						ODULong draftNum,
						ODIText *modUser,
						ODIText *comment)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerCreateDraft");

    try
    {

        if (somSelf->HasWriteAccess(ev))
        {
            {
                ODTime savedDate;

                // Set the draft properties
                TempODStorageUnit su = _fDraft->AcquireDraftProperties(ev);

                if (modUser)
                    ODSetITextProp(ev, su, kODPropModUser, 
						kODPlatformIText, modUser);
                if (comment)
                    ODSetITextProp(ev, su, kODPropDraftComment, 
						kODPlatformIText, comment);

                time((time_t *)(&savedDate));
                ODSetTime_TProp(ev, su, kODPropDraftSavedDate, 
						kODTime_T, savedDate);
                ODSetULongProp(ev, su, kODPropDraftNumber, 
                        			kODULong, draftNum); 
            }

            // Save the document and close the current draft
            somSelf->SaveDocument(ev);

#ifdef DEBUG
	    ODULong refcnt = _fDraft->GetRefCount(ev);
#endif
	    // Note: Before calling CloseDraft, the refcount of 
	    //  current working draft is 2 (the first reference is
            //  is obtained in CreateWorkingDraft and the second reference
            //  is obtained after calling rootPart->Open in OpenRootPart).
	    //  After calling CloseWindows in CloseDraft, the refcount
	    //  of current working draft is down to 1 (the draft reference
	    //  acquired by window is released when window is closed).
	    //  With the release done in CreateDraft below, the refcount
	    //  for this draft will reduce to 0.
            somSelf->CloseDraft(ev, _fDraft);

#ifdef DEBUG
	    refcnt = _fDraft->GetRefCount(ev);
#endif

	    // Create a new working draft.
	    // Note: This create releases the refcount for the current
	    //	draft before creating a new draft. 
	    _fDraft = _fDocument->CreateDraft(ev, _fDraft, kODTrue);
		
	    if (_fDraft)
            {
                {
                    TempODStorageUnit su = _fDraft->AcquireDraftProperties(ev);

	    	    // Reset timestamp and user properties
                    somSelf->ResetDateModByInfo(ev, su); 
		    // Reset drop number to 0
               	    ODSetULongProp(ev, su, kODPropDraftNumber, 
                        			kODULong, 0L); 
                }
		
#ifdef DEBUG
	        refcnt = _fDraft->GetRefCount(ev);
#endif
	        // Open the draft
       	        // Note: The refcount for this draft will be 2
    	        //	after calling OpenDraft
	        somSelf->OpenDraft(ev, _fDraft, kODFalse);

	        somSelf->SaveDocument(ev);

#ifdef DEBUG
	        refcnt = _fDraft->GetRefCount(ev);
#endif
	    }

	}

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CreateDraft");
	ODSetSOMException(ev, _exception);
    }

    return;
}


//------------------------------------------------------------------------------
// OpenDraft
//
//	Read in draft's window and frame information and open its windows.
//	If request by caller, the reference to draft is released.
//
//	Note: Caller needs to acquire a reference to the draft before
//		calling this method
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK DocumentManagerOpenDraft(
						  DocumentManager *somSelf, 
                                                  Environment *ev,
						  ODDraft     *draft,
						  ODBoolean   toBeRelease)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerOpenDraft");

    ODBoolean result = kODTrue;

    try
    {

	ODBoolean       windowsOpen = kODFalse;

	// Notify link manager that this draft is opened
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
	_fSession->GetLinkManager(ev)->DraftOpened(ev, draft);
#elif defined(_PLATFORM_OS2_)
	_fSession->GetLinkService(ev)->
                        DraftOpened(ev, draft, kODFalse, _fFileName);
#endif

	// Read in all root frames for this draft
	_fWindowState->Internalize(ev, draft);

	// Open all windows belonging to this draft
	_fWindowState->OpenWindows(ev, draft);

	windowsOpen = (_fWindowState->GetRootWindowCount(ev, draft)>0);

	if (!windowsOpen)
	{
	    TempODPart	rootPart = ODAcquireRootPartOfDraft(ev, draft);
	    rootPart->Open(ev, kODNULL);

	    // For now, title is the same for all drafts 
	    _fWindowState->SetDefaultWindowTitles(ev, draft);
	}
	
	// Release the reference to draft if request by caller
	if (toBeRelease)
	    ODReleaseObject(ev, draft);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::OpenDraft");
        result = kODFalse;
	ODSetSOMException(ev, _exception);
    }

    return result;

}


//------------------------------------------------------------------------------
// CloseDraft
//
//	Close the draft, clear the undo/redo stack, and close all
//	windows belonging to the draft.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerCloseDraft(DocumentManager *somSelf, 
                                                   Environment *ev,
						   ODDraft *draft)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerCloseDraft");

    try
    {

	
	// Notify the link manager. If unsaved cross-document
	// links exist, the link manager breaks those links at
	// the source.
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
	ODLinkManager *LinkMgr = _fSession->GetLinkManager(ev);
	if (LinkMgr)
	    LinkMgr->DraftClosing(ev, draft);
#elif defined(_PLATFORM_OS2_)
	IODLinkService *LinkService = _fSession->GetLinkService(ev);
	if (LinkService)
	    LinkService->DraftClosing(ev, draft);
#endif

	if (draft == _fDraft)
	{
	    ODUndo *Undo = _fSession->GetUndo(ev);

	    // Clear the undo and redo stacks
	    if (Undo)
		Undo->ClearActionHistory(ev, kODDontRespectMarks);
	}
	

	// Notify Clipboard that this draft is to be closed
	ODClipboard *ClipBoard = _fSession->GetClipboard(ev);
	if (ClipBoard)
	    ClipBoard->DraftClosing(ev, draft);


	// Notify DragAndDrop object that this draft is to be closed
	ODDragAndDrop *DragDrop = _fSession->GetDragAndDrop(ev);
	if (DragDrop)
	    DragDrop->DraftClosing(ev, draft);

	// Close all windows belonging to this draft
	// Note: CloseWindows releases the draft acquired by the window
	if (_fWindowState)
	    _fWindowState->CloseWindows(ev, draft);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::CloseDraft");
	ODSetSOMException(ev, _exception);
    }

    return;
}


//------------------------------------------------------------------------------
// SaveDraft
//
//	Save the current draft, writing out its window and frame state
//	and persistent objects.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSaveDraft(DocumentManager *somSelf, 
                                                  Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerSaveDraft");

    try
    {
	// Save the window properties and frame of this draft
	if (_fWindowState)
	    _fWindowState->Externalize(ev, _fDraft);

	// Save all persistent objects and storage units belonging
	// to this draft
	if (_fDraft)
	    _fDraft->Externalize(ev);

	if (_fSession)
	{
	    // Notify link manager to update cross-document links
	    ODLinkManager *LinkMgr = _fSession-> GetLinkManager(ev);
	    if (LinkMgr)
		LinkMgr->DraftSaved(ev, _fDraft);

	    // Notify Clipboard that the draft has been saved
	    ODClipboard *ClipBoard = _fSession->GetClipboard(ev);
	    if (ClipBoard)
		ClipBoard->DraftSaved(ev, _fDraft);
	}

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::SaveDraft");
	ODSetSOMException(ev, _exception);
    }

    return;
}

//------------------------------------------------------------------------------
// DeleteDraft
//
//	Remove draft from document
//
//	Note: Caller needs to acquire a reference to the draft before
//		calling this method
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK DocumentManagerDeleteDraft(
						    DocumentManager *somSelf, 
                                                    Environment *ev,
						    DraftInfoRec *draftInfoRec)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerDeleteDraft");

    ODBoolean result = kODTrue;
    ODDraft   *fromDraft = kODNULL;

    if (!draftInfoRec)
       return kODFalse;

    // Get draft associated with the draft info record
    ODDraft *draft = draftInfoRec->GetDraft();

    if (!draft)
      return kODFalse;

    try
    {

	// Release the object which was acquired before calling 
	// DeleteDraft
	if (draft->GetRefCount(ev) > 1) ODRelease(ev, draft);

        if (draft->GetRefCount(ev) != 1)
        {
          // Check whether trying to delete open draft
	  WARNMSG(WARN_INDEX(AMSG_480), 
                                   "Docshell: trying to delete open draft.");

          result = kODFalse;
        }
        else
        {
	  if (!draft || draft == _fDraft)
	    ; // can't delete current draft, this shouldn't happen!
	  else
	  {
	    // Close the draft
	    somSelf->CloseDraft(ev, draft);

	    // Get draft above it
	    ODDraft *fromDraft = _fDocument->AcquireDraft(ev,
							kODDPReadOnly,
							kODNULL,
							draft,
							kODPosFirstAbove,
							kODFalse);

	    // If draft above is open, close and open it again.
	    // Otherwise, the window is pointing to the wrong draft.
	    ODBoolean openDraftAbove = _fWindowState->GetRootWindowCount(ev,
						fromDraft);
	    if (openDraftAbove == kODTrue)
			somSelf->CloseDraft(ev, fromDraft);

#ifdef DEBUG
	    ODULong refcount =  draft->GetRefCount(ev);
#endif
            // Check if draft above is in the draft history list. If not,
            // keep the reference, else release it.
            if (draftInfoRec->GetPrev())
               ODRelease(ev, fromDraft);

#ifdef DEBUG
	    refcount =  draft->GetRefCount(ev);
#endif
	
	    // Get the previous and next DraftInfoRec info
	    // to update the draft list
	    DraftInfoRec *prevDIR = draftInfoRec->GetPrev();
	    DraftInfoRec *nextDIR = draftInfoRec->GetNext();

	    // Get a reference to the draft. Otherwise, deleting the
	    // draft's draftInfoRec deletes its last reference.
	    draft = _fDocument->AcquireDraft(ev, kODDPReadOnly, kODNULL,
					draft, kODPosSame, kODFalse);
	    delete draftInfoRec; 
#ifdef DEBUG
	    refcount =  draft->GetRefCount(ev);
#endif

	    _fDocument->SaveToAPrevDraft(ev, fromDraft, draft);
	    _fDocument->CollapseDrafts(ev, fromDraft, draft);

            // Note: fromDraft is no longer valid
            fromDraft = kODNULL;
		
	    if (openDraftAbove == kODTrue)
	    {
#ifdef DEBUG
		ODULong refcount = draft->GetRefCount(ev);
#endif
		somSelf->OpenDraft(ev, draft, kODFalse);
			
#ifdef DEBUG
		refcount = draft->GetRefCount(ev);
#endif
	    }

	    // Readjust the draft list
	    if (prevDIR == kODNULL && nextDIR == kODNULL)
            {
		// Nothing in the draft list
		_fDraftList = kODNULL;

                // Release the draft
                draft->Release(ev);
            }
	    else if (prevDIR == kODNULL)
	    {
		// The deleted draft is head of the draft list,
		// reset head of the draft list.
		nextDIR->SetPrev(kODNULL);
		_fDraftList = nextDIR;

                // Release the draft
                draft->Release(ev);
	    }
	    else if (nextDIR == kODNULL)
	    {
		// The deleted draft is tail of the draft list
		prevDIR->SetNext(kODNULL);

		// Reset the draft object for the draft above the
		// deleted draft
		prevDIR->SetDraft(draft);
	    }
	    else
	    {
		// The deleted draft is in the middle of the draft
		// list
		prevDIR->SetNext(nextDIR);
		nextDIR->SetPrev(prevDIR);

		// Reset the draft object for the draft above the
		// deleted draft
		prevDIR->SetDraft(draft);
	    }
	  }
        }
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::DeleteDraft");
        ODError error = ErrorCode();
        if (fromDraft)
           SaveAndRestoreEv1(fromDraft);
        result = kODFalse;
        ODSetSOMException(ev, error);
    }

    return result;

}


//------------------------------------------------------------------------------
// RemoveChanges
//
//	Remove changes in the working draft if draft is dirty and remove
//	working draft from the document
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerRemoveChanges(DocumentManager *somSelf, 
                                                    Environment *ev,
						    ODDraft *draft)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
			"DocumentManagerRemoveChanges");

    try
    {

	if (_fDraft == draft)
	{
	    // For current draft, if draft's permissions provides 
	    // write access, 
	    // - remove changes made to this draft if draft is 
	    //   marked dirty
	    // - remove this draft from document
	    //   Note: The call to RemoveFromDocument releases 
	    //         the draft object too.
	    if (somSelf->HasWriteAccess(ev))
	    {
		if (_fDraft->ChangedFromPrev(ev))
		    _fDraft->RemoveChanges(ev);
		_fDraft->RemoveFromDocument(ev);
		_fDraft = kODNULL;
	    }
	    else
		// if draft's permission does not allow write
		// access, release the current draft
		ODFinalReleaseObject(ev, _fDraft);
	}


    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::RemoveChanges");
	ODSetSOMException(ev, _exception);
    }

}


//------------------------------------------------------------------------------
// Revert
//
//	Revert to the last saved version of the document
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerRevert(DocumentManager *somSelf, 
                                               Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerRevert");

    try
    {
	// Close the working draft
	somSelf->CloseDraft(ev, _fDraft);

	// Remove changes belonging to the working draft
	_fDraft->RemoveChanges(ev);

	// Open the last saved draft
	somSelf->OpenDraft(ev, _fDraft, kODFalse);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::Revert");
	ODSetSOMException(ev, _exception);
    }

    return;
}

//------------------------------------------------------------------------------
// SetSession
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSetSession(DocumentManager *somSelf, 
                                                Environment *ev,
						ODSession *session)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerSetSession");

    _fSession = session;
}


//------------------------------------------------------------------------------
// SetWindowState
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSetWindowState(DocumentManager *somSelf, 
                                                Environment *ev,
						ODWindowState *windowState)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerSetWindowState");

    _fWindowState = windowState;
}


//------------------------------------------------------------------------------
// SetDraftList
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSetDraftList(DocumentManager *somSelf, 
                                                Environment *ev,
						DraftInfoRec *draftList)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager","DocumentManagerSetDraftList");

    _fDraftList = draftList;
}


//------------------------------------------------------------------------------
// HasWriteAccess
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK DocumentManagerHasWriteAccess(
						DocumentManager *somSelf, 
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerHasWriteAccess");

    // Returns true if draft's current permission includes 
    // kODDPSharedWrite or kODDPExclusiveWrite.
    // Note: ODDraftPermissions is a enum with write permission
    //       values set at the end.
    return _fDraftPermissions >= kODDPSharedWrite;
}


//------------------------------------------------------------------------------
// GetDraft
//------------------------------------------------------------------------------
SOM_Scope ODDraft*  SOMLINK DocumentManagerGetDraft(
						DocumentManager *somSelf, 
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerGetDraft");

    return _fDraft;
}


//------------------------------------------------------------------------------
// GetDocument
//------------------------------------------------------------------------------
SOM_Scope ODDocument*  SOMLINK DocumentManagerGetDocument(
						DocumentManager *somSelf, 
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerGetDocument");

	return _fDocument;
}


//------------------------------------------------------------------------------
// GetContainer
//------------------------------------------------------------------------------
SOM_Scope ODContainer*  SOMLINK DocumentManagerGetContainer(
                                                DocumentManager *somSelf,
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                	"DocumentManagerGetContainer");

    return _fContainer;
}


//------------------------------------------------------------------------------
// GetDraftList
//------------------------------------------------------------------------------
SOM_Scope DraftInfoRec*  SOMLINK DocumentManagerGetDraftList(
						DocumentManager *somSelf, 
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerGetDraftList");

    return _fDraftList;
}


//------------------------------------------------------------------------------
// InternalizeHistory
//
//	Build the list of draft to be displayed in the draft history 
//	window 
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerInternalizeHistory(
						DocumentManager *somSelf, 
                                                Environment *ev)
{
    // The list stored in _fDraftList will have the most recent
    // draft in the front followed by the least recent drafts.

    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
				"DocumentManagerInternalizeHistory");

    DraftInfoRec  *firstInfoRec = kODNULL;
    try
    {

	TempODDraft	prevDraft = kODNULL;
        TempODDraft	bottomDraft = _fDocument->AcquireBaseDraft(ev,
							kODDPReadOnly);
	DraftInfoRec	*aDraftInfoRec, *newDIR;

        // Get the most recent created draft in the document
        prevDraft = somSelf->GetLatestCreatedDraft(ev);

	// If there are drafts created in the document, traverse through
	// the draft list to retrieve the draft info.
	//
	// Note: The most recent draft created by using the Create Draft
	// 	dialog will be the second draft below the working
	//	draft. See Create Draft for more info.
	if (prevDraft)
	{
	    for ( ; ; )
	    {
		newDIR = new DraftInfoRec;

		// Retrieve the draft info
		newDIR->InitDraftInfoRec(ev, prevDraft);
				
		// Connect the draft list
		if (firstInfoRec)
		{
		    aDraftInfoRec->SetNext(newDIR);
		    newDIR->SetPrev(aDraftInfoRec);
		    aDraftInfoRec = newDIR;
		}
		else
		{
		    aDraftInfoRec = newDIR;
		    firstInfoRec = aDraftInfoRec;
		}
		if ( prevDraft == bottomDraft )
		    break;

		prevDraft = _fDocument->AcquireDraft(ev, kODDPReadOnly,
							kODNULL,
							prevDraft,
							kODPosFirstBelow,
							kODTrue);
            }
	}

	// Set DraftList
	_fDraftList = firstInfoRec;

    }
    catch (ODException _exception)
    {
       LOG("SOM exception occured in DocumentManager::InternalizeHistory");
       if (firstInfoRec)
       {
          ODError error = ErrorCode();
          SaveEv();
          DraftInfoRec  *curDIR, *nextDIR;

          // Iterate through the list of draft to delete each newly
          // created draft record
          for (curDIR = nextDIR = firstInfoRec; 
               curDIR != kODNULL; 
               curDIR = nextDIR)
          {
              nextDIR = curDIR->GetNext();
              try
              {
                 delete curDIR;
              }
              catch (ODException _exception)
              {
                 // Clear the error
                 SetErrorCodeEv(ev, kODNoError);
              }
          }
          RestoreEv();
          ODSetSOMException(ev, error);
       }
    }

}


//------------------------------------------------------------------------------
// IsStationery
//
//	Determine whether document is a stationery document or not
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK DocumentManagerIsStationery(
						DocumentManager *somSelf, 
                                                Environment *ev,
                                                ODDraft *draft)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager", 
				"DocumentManagerIsStationery");

    ODBoolean stationeryExist = kODFalse;

    try
    {

	TempODStorageUnit su = ODAcquireRootPartSUOfDraft(ev, draft);

	if (su)
	{
	    // kODPropIsStationery is a property to tell whether
	    // this is a stationery or not.
	    // Check whether this property exists or not. If this
	    // property exists, get the value.
	    stationeryExist = (ODSUExistsThenFocus(ev, su, 
						kODPropIsStationery, 
						kODBoolean));

	    if (stationeryExist)
		StorageUnitGetValue(su, ev, sizeof(ODBoolean),
						(ODValue)&stationeryExist);

	}
		

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::IsStationery");
	ODSetSOMException(ev, _exception);
    }

    return stationeryExist;
}



//------------------------------------------------------------------------------
// SetStationery
//
//	Set document as stationery document 
//------------------------------------------------------------------------------
SOM_Scope void SOMLINK DocumentManagerSetStationery(
						DocumentManager *somSelf, 
                                                Environment *ev,
						ODDraft *draft)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager", 
				"DocumentManagerSetStationery");

    try
    {

	TempODStorageUnit su = ODAcquireRootPartSUOfDraft(ev, draft);

	// Set kODPropIsStationery property to signal that this document
	// is a stationery
	if (su)
	    ODSetBooleanProp(ev, su, kODPropIsStationery, kODBoolean, kODTrue);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::SetStationery");
	ODSetSOMException(ev, _exception);
    }
}


//------------------------------------------------------------------------------
// UnsetStationery
//
//	Remove stationery from root part
//------------------------------------------------------------------------------
SOM_Scope void SOMLINK DocumentManagerUnsetStationery(
						DocumentManager *somSelf, 
                                                Environment *ev,
						ODDraft *draft)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager", 
				"DocumentManagerUnsetStationery");

    try
    {

	TempODStorageUnit su = ODAcquireRootPartSUOfDraft(ev, draft);

	// Remove stationery property from document
	if (su)
	    ODSURemoveProperty(ev, su, kODPropIsStationery);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::UnsetStationery");
	ODSetSOMException(ev, _exception);
    }
}



//------------------------------------------------------------------------------
// GetLatestCreatedDraft
//
//	Get the most recent created draft in the document
//
// 	Note: Caller is responsible to release the draft returned by
// 		this method.
//------------------------------------------------------------------------------
SOM_Scope ODDraft* SOMLINK DocumentManagerGetLatestCreatedDraft(
						DocumentManager *somSelf, 
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager", 
				"DocumentManagerGetLatestCreatedDraft");

    ODDraft	*prevDraft = kODNULL;

    try
    {

	// If working draft provides write access, get 
	// a copy of the draft below the working draft. Otherwise,
	// get the working draft.
	if (somSelf->HasWriteAccess(ev))
	{
	    prevDraft = _fDocument->AcquireDraft(ev, kODDPReadOnly,
				kODNULL, _fDraft, kODPosFirstBelow, kODFalse);
	}
	else
	{
	    prevDraft = _fDraft;
	    prevDraft->Acquire(ev);
	}

	// Get the latest created draft from the document if there is one.
	// Otherwise, release the draft and set the latest created draft
	// to null.
	if (_fDocument->Exists(ev, kODNULLID, prevDraft, kODPosFirstBelow))
	    prevDraft = _fDocument->AcquireDraft(ev, kODDPReadOnly,
				kODNULL, prevDraft, kODPosFirstBelow, kODTrue);
	else
	    // Note: ODReleaseObject will set prevDraft to NULL
	    ODReleaseObject(ev, prevDraft);

    }
    catch (ODException _exception)
    {
       LOG("SOM exception occured in DocumentManager::GetLatestCreatedDraft");
       ODError error = ErrorCode();
       if (prevDraft)
          SaveAndRestoreEv1(prevDraft);
       ODSetSOMException(ev, error);
    }

    return (prevDraft);
	
}


//------------------------------------------------------------------------------
// SaveACopyAs
//
//	Save a copy of the top draft to a named file
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSaveACopyAs(
                                                DocumentManager *somSelf,
                                                Environment *ev,
						ODDraft *draft,
						PlatformFile *file)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                	"DocumentManagerSaveACopyAs");

    try
    {
       ODBoolean fileError = kODFalse;

       // if there is a problem creating the new file, delete existing file
       // and then create the new file again.
       try 
       {
          // Create a new file
          file->Create(kODShellSignature, kODShellSignature, 0);
       } 
       catch (ODException _exception)
       {
          // Clear the exception
          SetErrorCodeEv(ev, kODNoError);
          fileError = kODTrue;
       }

       if (fileError)
       {
          char  fileName[FILE_LENGTH];
          
          file->GetAsciiName(fileName, FILE_LENGTH - 1); 
          TempPlatformFile currentFile = GetPlatformFileFromContainer(ev, 
						_fDocument->GetContainer(ev));

          ODBoolean exists = kODFalse;
          try
          {
             exists = file->Exists();
          }
          catch (ODException _exception)
          {
             // Clear the exception. 
             SetErrorCodeEv(ev, kODNoError);
          }
 
          // If error creating the file and file does not exist, then
          // it must be an invalid path or file name. Issue a warning
          // and then return to the caller.
          if (!exists)
             WARNMSG(WARN_INDEX(AMSG_850),"CPPOD0335 - Docshell: Problem creating file %s.  File or pathname could be invalid.", fileName);
          else
          {
             // If file to where a copy is saved is the same file name as
             // the one used by the current document, issue a warning and
             // then return to the caller.
             if (file->IsEqualTo(currentFile))
                WARNMSG(WARN_INDEX(AMSG_859),"CPPOD0344 - DocMgr: Cannot save a copy of file onto current document.");
             else 
             {
                // If file exists, delete the file and then create the
                // file again.
                fileError = kODFalse;
                try
                {
                   file->Delete();
                }
                catch (ODException _exception)
                {
                   // If error deleting the file, issue a warning and then
                   // return to the caller.
                   WARNMSG(WARN_INDEX(AMSG_860),"CPPOD0345 - DocMgr: File %s is already in use. Select a new name or close the file in use by another application.", fileName);
                   // Clear the exception
                   SetErrorCodeEv(ev, kODNoError);
                   fileError = kODTrue;
                }

                if (!fileError)
                {
                   try
                   {
                      file->Create(kODShellSignature, kODShellSignature, 0);
                   } 
                   catch (ODException _exception)
                   {
                      WARNMSG(WARN_INDEX(AMSG_858),"CPPOD0343 - Docshell: Problem creating file %s. Directory may be full or write-protected.", fileName);
                      // Clear the exception
                      SetErrorCodeEv(ev, kODNoError);
                      fileError = kODTrue;
                   }
                }
             }
          }
       }

       if (!fileError)
       {
          TempODContainer newContainer = CreateFileContainer(ev, _fSession, 
						&(file->GetFileSpec()));

          // Save the top draft to the container
          somSelf->ContainerSaveACopyAs(ev, draft, newContainer); 
       }
    }

    catch (ODException _exception)
    {
        WARNMSG(WARN_INDEX(AMSG_907), "CPPOD0454 - Docshell: Error saving a copy of draft into a new document.");

        // Clear the error
	SetErrorCodeEv(ev, kODNoError);
    }

    return;
}


//------------------------------------------------------------------------------
// ContainerSaveACopyAs
//
//	Save a copy of the top draft to a named container
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerContainerSaveACopyAs(
                                                DocumentManager *somSelf,
                                                Environment *ev,
						ODDraft *draft,
						ODContainer *newContainer)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                	"DocumentManagerContainerSaveACopyAs");

    try
    {

	// if draft's permission allows write and draft is dirty,
	// save the window state and persistent object of draft. 
        if (draft && draft->GetPermissions(ev) >= kODDPSharedWrite &&
	    draft->ChangedFromPrev(ev))
	{
	    _fWindowState->Externalize(ev, draft);
	    draft->Externalize(ev);
	}

	if (draft)
	{
	    // Create a new document in the newly created container
	    TempODDocument  newDocument = newContainer->AcquireDocument(ev,
							kODDefaultDocument);

	    // Copy the draft as the base draft of the new document
	    newDocument->SetBaseDraftFromForeignDraft(ev, draft);

	    // Get a reference of the newly created base draft
	    TempODDraft newDraft = newDocument->AcquireBaseDraft(ev,
							kODDPExclusiveWrite);

	    // Modify timestamp and user properties
            TempODStorageUnit su = newDraft->AcquireDraftProperties(ev);
            somSelf->ResetDateModByInfo(ev, su);

	    // Reset drop number to 0
            ODSetULongProp(ev, su, kODPropDraftNumber, kODULong, 0L); 

            TempODStorageUnit rootsu = ODAcquireRootPartSUOfDraft(ev, newDraft);
            somSelf->ResetDateModByInfo(ev, rootsu);

	    // Save the base draft
	    newDraft->Externalize(ev);
	}

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::ContainerSaveACopyAs");
	ODSetSOMException(ev, _exception);
    }
}


//------------------------------------------------------------------------------
// ResetDateModByInfo
//
//	Modify timestamp and user properties stored in the passed-in 
//	storage unit
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerResetDateModByInfo(
                                                DocumentManager *somSelf,
                                                Environment *ev,
						ODStorageUnit *su)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                	"DocumentManagerResetDateModByInfo");

    ODIText *modBy = kODNULL;
    ODTime  modDate;

    try
    {

       // Reset creation and modification date 
       time((time_t *)(&modDate));
       ODSetTime_TProp(ev, su, kODPropCreateDate, kODTime_T, modDate);
       ODSetTime_TProp(ev, su, kODPropModDate, kODTime_T, modDate);
       _fSession->GetUserName(ev, &modBy);
       ODSetITextProp(ev, su, kODPropModUser, kODPlatformIText, modBy);
       DisposeITextStruct(*modBy);
       modBy = kODNULL;
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::ResetDateModByInfo");
        if (modBy)
           DisposeITextStruct(*modBy);
	ODSetSOMException(ev, _exception);
    }
}	

//------------------------------------------------------------------------------
// DeleteDraftList
//
//      Delete the list of draft built for draft history
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerDeleteDraftList(
                                                DocumentManager *somSelf,
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                        "DocumentManagerDeleteDraftList");

    DraftInfoRec *curDIR, *nextDIR;

    try
    {
       // Iterate through the list of draft to delete each draft record
       for (curDIR = nextDIR = _fDraftList; curDIR != kODNULL; curDIR = nextDIR)
       {
           nextDIR = curDIR->GetNext();
           delete curDIR;
       }
    }
    catch (ODException _exception)
    {
       LOG("SOM exception occured in DocMgr::DeleteDraftList");
       ODSetSOMException(ev, _exception);
    }
    _fDraftList = kODNULL;
}

//------------------------------------------------------------------------------
// GetFileName
//
//      Accessor method for _fFileName
//------------------------------------------------------------------------------
SOM_Scope char*  SOMLINK DocumentManagerGetFileName(
                                                DocumentManager *somSelf,
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                        "DocumentManagerGetFileName");

    return(_fFileName);
}

//------------------------------------------------------------------------------
// SetFileName
//
//      Accessor method for _fFileName
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSetFileName(
                                                DocumentManager *somSelf,
                                                Environment *ev,
                                                char *fileName)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                        "DocumentManagerGetFileName");

    if (fileName)
        strcpy(_fFileName, fileName);
}

//------------------------------------------------------------------------------
// GetPartKindName
//
//      Accessor method for _fPartKindName
//------------------------------------------------------------------------------
SOM_Scope char*  SOMLINK DocumentManagerGetPartKindName(
                                                DocumentManager *somSelf,
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                        "DocumentManagerGetPartKindName");

    return(_fPartKindName);
}

//------------------------------------------------------------------------------
// SetPartKindName
//
//      Accessor method for _fPartKindName
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSetPartKindName(
                                                DocumentManager *somSelf,
                                                Environment *ev,
                                                char *kindName)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                        "DocumentManagerGetPartKindName");

    if (kindName)
        strcpy(_fPartKindName, kindName);
}


//==============================================================================
//==============================================================================
//==============================================================================

//------------------------------------------------------------------------------
// The remaining methods in this class are required for OLE interoperability
// and so are only applicable on the Win32 platform
//------------------------------------------------------------------------------

#ifdef _PLATFORM_WIN32_

//------------------------------------------------------------------------------
// SetOleWrapper
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK DocumentManagerSetOleWrapper(
                                                DocumentManager *somSelf,
                                                Environment *ev,
						OleWrapper *oleWrapper)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                	"DocumentManagerSetOleWrapper");

    try
    {
       if ( oleWrapper )
           oleWrapper->AddRef();
       else   /* NULL */
           _fOleWrapper->Release();
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::SetOleWrapper");
	ODSetSOMException(ev, _exception);
    }

    _fOleWrapper = oleWrapper;
}

//------------------------------------------------------------------------------
// GetOleWrapper
//------------------------------------------------------------------------------
SOM_Scope OleWrapper*  SOMLINK DocumentManagerGetOleWrapper(
                                                DocumentManager *somSelf,
                                                Environment *ev)
{
    DocumentManagerData *somThis = DocumentManagerGetData(somSelf);
    DocumentManagerMethodDebug("DocumentManager",
                                	"DocumentManagerGetOleWrapper");

    try
    {
       // ensure there is always an ole wrapper to return
       if (!_fOleWrapper) {
   	  OleWrapper* wrapper;
	  ODOLEManager* oleManager;

	  // this hairy path is how we get a handle on the wrapper factory
	  // (creating it if necessary).  we then use the factory to create
	  // a wrapper. 
	  oleManager = _fSession->GetOleManager(ev);

	  // first make sure we have a factory appropriately registered
	  if (!oleManager->GetProcessData()->gFactory &&
	      !oleManager->RegisterOleFactory(FALSE))
		return NULL;

	  // Now create a wrapper and save it away.  Tell the wrapper
	  // about the association as well
	  ((WrapIClassFactory*)oleManager->GetProcessData()->gFactory)->CreateInstance(NULL, IID_IUnknown, (void**)&wrapper);
	  somSelf->SetOleWrapper(ev, wrapper);
	  wrapper->SetDocumentManager(somSelf);

	  // may need to set the magic WFLG_LOADEDFROMFILE bit here
	  // wrapper->fWFlg |= WFLG_LOADEDFROMFILE;
       }
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in DocumentManager::GetOleWrapper");
	ODSetSOMException(ev, _exception);
    }
    return _fOleWrapper;
}
#endif
