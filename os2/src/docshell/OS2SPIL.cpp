/* @(#)Z 1.3 os2/src/docshell/OS2SPIL.cpp, odshell, od96os2, odos29646d 96/11/15 15:51:38 (96/08/23 01:41:34) */
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
// OS2SPIL.cpp
//
//	This file provides the implementation for loading and installation
//	of shell plug-in.
//
//------------------------------------------------------------------------------

#ifdef _PLATFORM_WIN32_
#include <mapiwin.h>
#include <winbase.h>
#include <wtypes.h>
#endif

#include <ordcoll.h>
#include <ODUtils.h>
#include "ShPlugIn.h"
#include "ShPInLst.h"

#define DLL_EXT "*.dll"
#define ODSPIDIR "ODSHELLPLUGINSDIR" 
#define PATH_SEPARATOR "\\"

typedef struct shellPlugInEntry 
{
#ifdef _PLATFORM_WIN32_
	HINSTANCE hModule;
#endif
	ODShellPlugInInstallProc procAddr;
} shellPlugInEntry;
	
static OrderedCollection *fShellPlugInList;

//------------------------------------------------------------------------------
// InitShellPlugInList
//
//	Instantiates the shell plug-in list
//------------------------------------------------------------------------------
void InitShellPlugInList()
{
	fShellPlugInList = new OrderedCollection;
}

//------------------------------------------------------------------------------
// AddShellPlugInToList
//	
//	Add the entry address of the shell plug-in library to
//	the shell plug-in list
//------------------------------------------------------------------------------
void AddShellPlugInToList(shellPlugInEntry *entry)
{
	fShellPlugInList->AddLast(entry);
}


//------------------------------------------------------------------------------
// InitShellPlugIn
//
//	This method initializes the shell plug-in list and 
//      scans the directory specified in the environment
//	variable ODShellPlugIns for shell plug-in shared libraries. 
//	For each shell plug-in located in the directory, add its
//	installation method (ODShellPluginInstall) entry address to 
//	the shell plug-in list that is to be executed later on when the
//	document's current draft is opened.
//
//	The loading of shell plug-in library and the invocation of
//	its installation method has to be separated to take account
//	of the support of multiple document enviornment. The loading
//	of the shell plug-in is performed once whereas the installation
//	method is performed on the current draft of each document when the
//      document is opened.
//------------------------------------------------------------------------------
void  InitShellPlugIn()
{
	ODBoolean done = kODFalse;
	ODShellPlugInInstallProc procAddr;
}


//------------------------------------------------------------------------------
// InstallShellPlugIn
//
//	Iterates through the shell plug-in list. For each shell plug-in
//	library in the list, calls its ODShellPluginInstall function.
//------------------------------------------------------------------------------
void  InstallShellPlugIn(Environment *ev, ODDraft *draft)
{
	OSErr err;
	shellPlugInEntry         *entry;
	ODShellPlugInInstallProc installProc;
	ODShellPlugInActionCodes action;
	OrderedCollectionIterator *orderColIter = fShellPlugInList->
							CreateIterator();

	// For each shell plug-in in the iterator, execute the install 
	// procedure
	for (entry = (shellPlugInEntry *) orderColIter->First();
		orderColIter->IsNotComplete();
		entry = (shellPlugInEntry *) orderColIter->Next())
	{
		action = kODShellPlugInNoAction;
		installProc = entry->procAddr;
		err = (*installProc) (ev, draft, &action);
		// if error returned or action is set to close connection
		// remove entry from list and unload the shared library
		if (err || (!err && (action & kODShellPlugInCloseConnection)))
		{
			orderColIter->RemoveCurrent();
			// FreeLibrary(entry->hModule);
		}
	}
	ODDeleteObject(orderColIter);
}
