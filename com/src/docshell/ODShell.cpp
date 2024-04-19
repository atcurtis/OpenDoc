/* @(#)Z 1.50 com/src/docshell/ODShell.cpp, odshell, od96os2, odos29646d 96/11/15 15:49:31 (96/10/29 09:24:07) */
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
// ODShell.cpp
//
//	This file provides the implementation of the OpenDocShell,
//	an abstract base class.
//------------------------------------------------------------------------------

#ifndef SOM_Module_odshell_Source
#define SOM_Module_odshell_Source
#endif
#define OpenDocShell_Class_Source

#include <DocMgr.xh>
#include <ODSessn.xh>
#include <Disptch.xh>
#include <WinStat.xh>
#include <CmdDefs.xh>
#include <tpllitr.xh>
#include <ODUtils.h>
#include <odres.h>
#include <Window.xh>
#include <ShPInLst.h>
#include <TempObj.h>
#ifdef _PLATFORM_WIN32_
#include <olemgrw.h>
#endif

#if (defined  _PLATFORM_WIN32_ || defined _PLATFORM_OS2_)
#ifndef SOM_ODHelp_xh
#include <ODHelp.xh>
#endif
#ifndef _HLPPANELS_
#include <hlppanls.h>
#endif
#endif //(_PLATFORM_WIN32_ || _PLATFORM_OS2_)

#define VARIABLE_MACROS
#include <ODShell.xih>


//------------------------------------------------------------------------------
// InitOpenDocShell
//
//	The main initialization item is to create and initialize the
//	ODSession object.  We also initialize the shell plug-in list
//	and perform some internal initialization.
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellInitOpenDocShell(OpenDocShell *somSelf,
                                                      Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellInitOpenDocShell");

    try
    {
        // Create the session
        _fSession = new ODSession;

        // Set the WindowSystemData now so that it has it for Initialization
        _fSession->SetWindowSystemData(ev, &_fWindowSystemData);

        #ifdef _PLATFORM_WIN32_
          // If this is Win32 then the session needs to give the OLEManager
          // a pointer to the shell
            _fSession->InitSession(ev, somSelf);
        #else
            _fSession->InitSession(ev);
        #endif

        // Initialize any shell plug-ins
        InitShellPlugIn();

        // Cache some useful objects
        _fDispatcher = _fSession->GetDispatcher(ev);
        _fWindowState = _fSession->GetWindowState(ev);

        // Create a list for DocumentManager objects
        _fDocumentManagerList = new somf_TPrimitiveLinkedList;

#ifdef _PLATFORM_WIN32_
	// Initialize OLE factories.  This must be the last thing done
	// as OLE will call the factories as soon as they are registered
	// and the factories will start making OD calls.
	_fSession->GetOleManager(ev)->InitFactories();
#endif

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::InitOpenDocShell");
	ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// DispatchEvent
//
//	Forward the event the the dispatcher.
//------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK OpenDocShellDispatchEvent(OpenDocShell *somSelf,
                                                        Environment *ev,
                                                        ODEventData* event)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellDispatchEvent");

    ODBoolean handled = kODFalse;

    try
    {
        handled = _fDispatcher->Dispatch(ev, event);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::DispatchEvent");
	ODSetSOMException(ev, _exception);
    }

    return handled;

}

//------------------------------------------------------------------------------
// Terminate
//
//	Perform ant necessary clean up prior to exiting
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellTerminate(OpenDocShell *somSelf,
                                               Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellTerminate");

    // Nothing identified to do yet

}

//------------------------------------------------------------------------------
// CreateDocumentManager
//
//	Instantiate and initialize a new DocumentManager object and add
//	it to our list
//------------------------------------------------------------------------------
SOM_Scope DocumentManager*  SOMLINK OpenDocShellCreateDocumentManager(
                                                       OpenDocShell *somSelf,
                                                       Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellCreateDocumentManager");

    DocumentManager     *docMgr = kODNULL;

    try
    {

        // Create a new Document Manager
        docMgr = new DocumentManager;
        if (docMgr)
        {
            // Initialize the DocumentManager
            docMgr->Initialize(ev, _fSession);

            // Add the DocumentManager to our list
            _fDocumentManagerList->somfAddLast(ev, docMgr);
        }

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::CreateDocumentManager");

        // Delete newly created document manager
        if (docMgr)
        {
           delete docMgr;
           docMgr = kODNULL;
        }
	ODSetSOMException(ev, _exception);
    }

    return docMgr;

}

//------------------------------------------------------------------------------
// UpdateActiveDocumentManager
//
//	Set the active DocumentManager object to the one which corresponds
//	to the given window
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellUpdateActiveDocumentManager(
                                                        OpenDocShell *somSelf,
                                                        Environment *ev,
							ODWindow *odWindow)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell",
				"OpenDocShellUpdateActiveDocumentManager");

    ODDraft			*draft;

    try
    {
        // Get the draft belonging to the active window
        draft = odWindow->GetDraft(ev);

        // Set the active document manager to the one
        // that holds the document belonging to the given draft
        somSelf->SetActiveDocumentManager(ev,
                               somSelf->GetDocumentManagerOfDraft(ev, draft));

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::UpdateActiveDocumentManager");
	ODSetSOMException(ev, _exception);
    }

}


//------------------------------------------------------------------------------
// GetDocumentManagerOfDraft
//
//	Find the DocumentManager object to the one which corresponds
//	to the given draft
//------------------------------------------------------------------------------
SOM_Scope DocumentManager* SOMLINK OpenDocShellGetDocumentManagerOfDraft(
                                                        OpenDocShell *somSelf,
                                                        Environment *ev,
							ODDraft *draft)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell",
				"OpenDocShellGetDocumentManagerOfDraft");

    ODDocument				*doc;
    somf_TPrimitiveLinkedListIterator	*docMgrIter;
    DocumentManager			*findDocMgr = kODNULL;

    try
    {
        // Find the document manager by iterating through the document
        // manager list to compare the document stored in each document
        // manager with the document belonging to the draft
        doc = draft->GetDocument(ev);
        docMgrIter = somSelf->CreateDocumentManagerIterator(ev);
        for (DocumentManager *docMgr =
                    (DocumentManager *)docMgrIter->somfFirst(ev);
                    docMgr != NULL;
	            docMgr = (DocumentManager *)docMgrIter->somfNext(ev))
        {
            if (docMgr->GetDocument(ev) == doc)
            {
                findDocMgr = docMgr;
                break;
            }
        }
        ODDeleteObject(docMgrIter);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::GetDocumentManagerOfDraft");
        ODDeleteObject(docMgrIter);

	ODSetSOMException(ev, _exception);
    }

    return findDocMgr;
}

//------------------------------------------------------------------------------
// GetActiveDocumentManager
//
//	Accessor method for _fActiveDocumentManager
//------------------------------------------------------------------------------
SOM_Scope DocumentManager*  SOMLINK OpenDocShellGetActiveDocumentManager(
                                                        OpenDocShell *somSelf,
                                                        Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell",
                                        "OpenDocShellGetActiveDocumentManager");

    return _fActiveDocumentManager;

}

//------------------------------------------------------------------------------
// SetActiveDocumentManager
//
//	Accessor method for _fActiveDocumentManager
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellSetActiveDocumentManager(
                                                        OpenDocShell *somSelf,
                                                        Environment *ev,
                                                        DocumentManager *docMgr)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell",
                                        "OpenDocShellSetActiveDocumentManager");

    _fActiveDocumentManager = docMgr;

}

//------------------------------------------------------------------------------
// DeleteDocumentManager
//
//	Remove the given DocumentManager object from our list and delete it
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellDeleteDocumentManager(OpenDocShell *somSelf,
                                                        Environment *ev,
							DocumentManager *docMgr)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellDeleteDocumentManager");

    try
    {
        _fDocumentManagerList->somfRemove(ev, docMgr);
        ODDeleteObject(docMgr);

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::DeleteDocumentManager");
	ODSetSOMException(ev, _exception);
    }


}

//------------------------------------------------------------------------------
// CreateDocumentManagerIterator
//
//	Create an iterator for our DocumentManager list
//------------------------------------------------------------------------------
SOM_Scope somf_TPrimitiveLinkedListIterator*  SOMLINK
				OpenDocShellCreateDocumentManagerIterator(
                                                        OpenDocShell *somSelf,
                                                        Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell",
                                "OpenDocShellCreateDocumentManagerIterator");

    somf_TPrimitiveLinkedListIterator *iterator;
    try
    {
       iterator = new somf_TPrimitiveLinkedListIterator;
       iterator->somfTPrimitiveLinkedListIteratorInit(ev, _fDocumentManagerList);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::CreateDocumentManagerIterator");
	ODSetSOMException(ev, _exception);
    }
    return iterator;

}

//------------------------------------------------------------------------------
// DeleteDocumentManagerIterator
//
//	Delete a DocumentManager iterator
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellDeleteDocumentManagerIterator(
                                OpenDocShell *somSelf, Environment *ev,
				somf_TPrimitiveLinkedListIterator *docMgrIter)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell",
                                "OpenDocShellDeleteDocumentManagerIterator");

    try
    {
          ODDeleteObject(docMgrIter)

    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::DeleteDocumentManagerIterator");
	ODSetSOMException(ev, _exception);
    }

}

//------------------------------------------------------------------------------
// GetSession
//
//	Accessor method for _fSession
//------------------------------------------------------------------------------
SOM_Scope ODSession*  SOMLINK OpenDocShellGetSession(OpenDocShell *somSelf,
                                                      Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellGetSession");

    return _fSession;

}

//------------------------------------------------------------------------------
// GetDispatcher
//
//	Accessor method for _fDispatcher
//------------------------------------------------------------------------------
SOM_Scope ODDispatcher*  SOMLINK OpenDocShellGetDispatcher(
							OpenDocShell *somSelf,
							Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellGetDispatcher");

    return _fDispatcher;

}

//------------------------------------------------------------------------------
// GetWindowState
//
//	Accessor method for _fSession
//------------------------------------------------------------------------------
SOM_Scope ODWindowState* SOMLINK OpenDocShellGetWindowState(
						OpenDocShell *somSelf,
                                                Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellGetWindowState");

    return _fWindowState;

}

//------------------------------------------------------------------------------
// SubClassResponsibility
//
//	This method is used to indicate that the calling method must be
//	overridden by a subclass
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellSubClassResponsibility(
						OpenDocShell *somSelf,
                                                Environment *ev,
						char *methodName)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell",
					"OpenDocShellSubClassResponsibility");

    WARNMSG(WARN_INDEX(AMSG_490),
            "A subclass should have overridden method %s!", methodName);

}

//------------------------------------------------------------------------------
// GetWindowSystemData
//
//	Accessor method for _fWindowSystemData
//------------------------------------------------------------------------------
SOM_Scope WindowSystemData*  SOMLINK OpenDocShellGetWindowSystemData(
						OpenDocShell *somSelf,
                                                Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellGetWindowSystemData");

    return &_fWindowSystemData;

}

//------------------------------------------------------------------------------
// SetWindowSystemData
//
//	Accessor method for _fWindowSystemData
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellSetWindowSystemData( OpenDocShell *somSelf,
                                Environment *ev,
				WindowSystemData *windowSystemData)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellSetWindowSystemData");

    memcpy(&_fWindowSystemData, windowSystemData,
					sizeof(WindowSystemData));
    if (_fSession)
        _fSession->SetWindowSystemData(ev, windowSystemData);

}

//------------------------------------------------------------------------------
// GetArgc
//
//	Accessor method for _fArgc
//------------------------------------------------------------------------------
SOM_Scope long  SOMLINK OpenDocShellGetArgc( OpenDocShell *somSelf,
                                Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellGetArgc");

    return _fArgc;

}

//------------------------------------------------------------------------------
// SetArgc
//
//	Accessor method for _fArgc
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellSetArgc( OpenDocShell *somSelf,
                                Environment *ev, long argc)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellSetArgc");

    _fArgc = argc;

}

//------------------------------------------------------------------------------
// GetArgv
//
//	Accessor method for _fArgv
//------------------------------------------------------------------------------
SOM_Scope char**  SOMLINK OpenDocShellGetArgv( OpenDocShell *somSelf,
                                Environment *ev)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellGetArgv");

    return _fArgv;

}

//------------------------------------------------------------------------------
// SetArgv
//
//	Accessor method for _fArgv
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellSetArgv( OpenDocShell *somSelf,
                                Environment *ev, char** argv)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellSetArgv");

    _fArgv = argv;

}

//------------------------------------------------------------------------------
// somUninit
//
//	Uninitializer method
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK OpenDocShellsomUninit(OpenDocShell *somSelf)
{
    OpenDocShellData *somThis = OpenDocShellGetData(somSelf);
    OpenDocShellMethodDebug("OpenDocShell","OpenDocShellsomUninit");

    try
    {
       ODDeleteObject(_fSession);
       OpenDocShell_parents_somUninit(somSelf);
    }
    catch (ODException _exception)
    {
        LOG("SOM exception occured in OpenDocShell::somUninit");
    }
}
