/* @(#)Z 1.11 com/src/docshell/ShPInLst.cpp, odshell, od96os2, odos29646d 96/11/15 15:52:16 (96/10/29 09:24:12) */
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
// ShPInLst.cpp
//
//	This file provides the implementation for loading and installation
//	of shell plug-in.
//
//------------------------------------------------------------------------------

#include <OrdColl.h>
#include <ODUtils.h>
#include <ODDebug.h>
#include <PlfmFile.h>
#include "ShPlugIn.h"
#include "ShPInLst.h"

#ifdef _PLATFORM_WIN32_
#include <mapiwin.h>
#include <winbase.h>
#include <wtypes.h>
#endif
#ifdef _PLATFORM_OS2_
#include <ODos2.h>
#endif
#ifdef _PLATFORM_UNIX_
#include <sys/types.h>
#include <dirent.h>
#endif

#define DLL_EXT "*.dll"
#define ODSPIDIR "ODSHELLPLUGINS" 
#define PATH_SEPARATOR "\\"

#ifdef _PLATFORM_WIN32_
#define PATH_SIZE MAX_PATH
#endif

#ifdef _PLATFORM_OS2_
#define PATH_SIZE CCHMAXPATHCOMP-1
#endif

typedef struct shellPlugInEntry 
{
#ifdef _PLATFORM_WIN32_
	HINSTANCE hModule;
#endif
#ifdef _PLATFORM_OS2_
        HMODULE hModule;
#endif
	ODShellPlugInInstallProc procAddr;
	char name[kODMaxFileNameSize+1];
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
#ifdef _PLATFORM_WIN32_
void  InitShellPlugIn()
{
        
	ODBoolean done = kODFalse;
	ODShellPlugInInstallProc procAddr;
	WIN32_FIND_DATA findFileData;
	HINSTANCE hModule = NULL;
	char path[PATH_SIZE+1], searchPath[PATH_SIZE+1], dllName[PATH_SIZE*2+1];

	InitShellPlugInList();

	// Get environment variable ODShellPlugIns which holds
	// the directory path to Shell PlugIn shared libraries
	char	*plugInPath = getenv(ODSPIDIR);
	
	// If ODShellPlugIns is set, then scan the directory
	// to look for shared library with ODShellPlugInInstall
	// function entry.
	if (plugInPath)
	{ 
		// directory path set in ODShellPlugIns environment
		// variable cannot exceed MAX_PATH.
		if (strlen(plugInPath) >= MAX_PATH) 
			return;

		path[0] = searchPath[0] = dllName[0] = '\0';

		strcpy(path, plugInPath);

		// Append path separator if directory path does not
		// end with a path separator
                if (strncmp(plugInPath+strlen(plugInPath)-1, PATH_SEPARATOR, 1)
                    != 0)
		{
			strcat(path, PATH_SEPARATOR);
		}

		// Append "*.dll" to the end of search path if 
		// MAX_PATH limit is not reached. Otherwise,
		// append "*" to the end of search path.
		strcpy(searchPath, path);
		if ((strlen(path) + strlen(DLL_EXT)) > MAX_PATH)
		{
			if ((strlen(path) + 1) > MAX_PATH)
				return;
			else
				strcat(searchPath, "*");
		}
		else
			strcat(searchPath, DLL_EXT);

		// Scan directory to look for first matched file
		HANDLE hFindFile = FindFirstFile((char *)searchPath, 
						&findFileData);

		// If error scaning the directory, log the error and return
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
//			LOG(GetLastError());
			return;
		}
		
		while (!done)
		{
			// Construct the full file name
			strcpy(dllName, path);
			strcat(dllName, findFileData.cFileName);
	
			// Load the shared library. If
			// file is not a shared library
			// then find the next file in the
			// directory.
			hModule = LoadLibraryEx(dllName, NULL,
				LOAD_WITH_ALTERED_SEARCH_PATH);
			if (hModule != NULL)
			{
				procAddr = (ODShellPlugInInstallProc) 
					GetProcAddress(hModule, 
				        "ODShellPlugInInstall");
				// Unload the library if
				// it is not a shell plugin.
				// Otherwise, add the entry
				// point to the list of shell
				// plugin install procedure.
				if (procAddr == NULL)
					FreeLibrary(hModule);
				else
				{
					shellPlugInEntry *entry = new 
						     shellPlugInEntry;
					entry->hModule = hModule;
					entry->procAddr = procAddr;
					strcpy(entry->name, 
                                                       findFileData.cFileName);
					AddShellPlugInToList(entry);
				}
			}

			// Find the next file in directory
			while (!FindNextFile(hFindFile, &findFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					done = kODTrue;
					break;
				}
			}
		} // endwhile 
	} // endif for plugInPath
}
#endif
#ifdef _PLATFORM_OS2_
void  InitShellPlugIn()
{
        
	ODBoolean done = kODFalse;
	ODShellPlugInInstallProc procAddr;
	char path[PATH_SIZE+1], searchPath[PATH_SIZE+1], dllName[PATH_SIZE*2+1],
		objectPath[PATH_SIZE+1];
	HMODULE       hModule = NULL;
	APIRET        rc;
	HDIR          hDir; 
	ULONG         cFileNames;
	PEAOP2        peaop2;
	PGEA2LIST     pgea2list;
	PFEA2LIST     pfea2list;
	PFILEFINDBUF4 pffbLocal;
	PSZ           pszFile;
	BYTE          gea2list[sizeof(GEA2LIST) + 6 /* len of ".TYPE" */];
	BYTE          findbuf[sizeof(EAOP2) + sizeof(FILEFINDBUF3) +
			CCHMAXPATHCOMP + 
			256 /* enough extra to hold FEA2LIST + */
			    /* pad characters + some */];

	InitShellPlugInList();

	// Get the directory specification for Workplace shell object
	// OD_SHELLPLUGINS
	char *plugInPath = NULL;
	HOBJECT hObject  = WinQueryObject("<OD_SHELLPLUGINS>");
	if (hObject)
	{
		WinQueryObjectPath(hObject, objectPath, sizeof(objectPath));
		plugInPath = objectPath;
	}
	
	// If OD_SHELLPLUGINS is set, then scan 
	// the folder to look for shared library with 
	// ODShellPlugInInstall function entry.
	if (plugInPath)
	{ 
		// directory path set in OD_SHELLPLUGINS object
		// cannot exceed PATH_SIZE.
		if (strlen(plugInPath) >= PATH_SIZE) 
			return;

		path[0] = searchPath[0] = dllName[0] = '\0';

		strcpy(path, plugInPath);

		// Append path separator if directory path does not
		// end with a path separator
                if (strncmp(plugInPath+strlen(plugInPath)-1, PATH_SEPARATOR, 1)
                    != 0)
		{
			strcat(path, PATH_SEPARATOR);
		}

		// Append "*.dll" to the end of search path if 
		// PATH_SIZE limit is not reached. Otherwise,
		// append "*" to the end of search path.
		strcpy(searchPath, path);
		if ((strlen(path) + strlen(DLL_EXT)) > PATH_SIZE)
		{
			if ((strlen(path) + 1) > PATH_SIZE)
				return;
			else
				strcat(searchPath, "*");
		}
		else
			strcat(searchPath, DLL_EXT);

		// Setup data to pass to DosFindFirst

		// Request 1 matching file entry at a time
		cFileNames = 1;

		// Request system to allocate a handle
		hDir = HDIR_CREATE;

		// Initialize the buffer
		memset(findbuf, 0, sizeof(findbuf));

		peaop2 = (PEAOP2)findbuf;
		pgea2list = (PGEA2LIST)&gea2list;
		pfea2list = 0;

		// create one list item in GEA list to look for files
		// with extended attribute of type
		pgea2list->cbList = sizeof(gea2list);
		pgea2list->list[0].oNextEntryOffset = 0;
		pgea2list->list[0].cbName           = 5;
		memcpy( pgea2list->list[0].szName, ".TYPE\x00", 6 );
		/* put list in a eaop2 structure */
		peaop2->fpGEA2List = pgea2list;
		
		rc = DosFindFirst(searchPath,  // file pattern to search
				&hDir,         // directory handle
				FILE_NORMAL,   // file attribute to look for
				&findbuf,      // result buffer
				sizeof(findbuf), // result buffer length
				&cFileNames,   // number of entries to find
				FIL_QUERYEASFROMLIST); // level 3 file info

		// If error finding the first matched file, return
		if (rc && rc != ERROR_BUFFER_OVERFLOW) 
			return;
		
		while (!done)
		{
			if (rc != ERROR_BUFFER_OVERFLOW)
			{
				// Get the file name from buffer
				// Note: For each matching file object found,
				//	a record is allocated to store the
				//	entry and the associated extended
				//	attributes. The record can be found
				//	following the EAOP2 data structure.

				// Go to the end of EAOP2 structure
				PFILEFINDBUF4 pffbLocal = (PFILEFINDBUF4)
						(findbuf+sizeof(EAOP2));
				// Get the pointer to the file's extended
				// attribute size
				pfea2list = (PFEA2LIST) &pffbLocal->cbList; 

				// Get to the file name by advancing
				// the pointer past the file's
				// extended attribute and the length field
				// of file name
				pszFile = (PSZ)pfea2list + pffbLocal->cbList 
						+ 1;

				// Construct the fully qualified file name
				strcpy(dllName, path);
				strcat(dllName, pszFile);

				char loadError[CCHMAXPATHCOMP] = "";
				rc = NO_ERROR;

				// Load the shared library
				rc = DosLoadModule(loadError, sizeof(loadError),
						 dllName, &hModule);
				if (rc == NO_ERROR)
				{
					procAddr = 0;
					// Get shell plugin entry address
					// Unload the library if it not a 
					// shell plugin. Otherwise, add the
					// entry point to the list of shell
					// plugin install procedure.
					rc = DosQueryProcAddr(hModule, 0L, 
						"ODShellPlugInInstall", 
						(PFN*)&procAddr);
					if (rc != NO_ERROR)
						DosFreeModule(hModule);
					else
					{
						shellPlugInEntry *entry = new 
							     shellPlugInEntry;
						entry->hModule = hModule;
						entry->procAddr = procAddr;
					        strcpy(entry->name, pszFile);
						AddShellPlugInToList(entry);
					}
				}
			} // endif

			// Find the next matching file in directory
			while ((rc = DosFindNext(hDir, &findbuf, 
				sizeof(findbuf), &cFileNames)) != NO_ERROR)
			{
				if (rc == ERROR_NO_MORE_FILES)
				{
					done = kODTrue;
					break;
				}
			}
		} // endwhile 
                DosFindClose(hDir);
	} // endif for plugInPath
}
#endif

#ifdef _PLATFORM_UNIX_

//extern "C" int (*load(char*, unsigned int, char*))();
extern "C" ODShellPlugInInstallProc load(char*, unsigned int, char*);
extern "C" int unload(ODShellPlugInInstallProc);

void InitShellPlugIn()
{
    InitShellPlugInList();

    // Get environment variable ODShellPlugIns which holds
    // the directory path to Shell PlugIn shared libraries
    char	*plugInPath = getenv(ODSPIDIR);
	
    // If ODShellPlugIns is set, then scan the directory
    // to look for shared library with ODShellPlugInInstall
    // function entry.
    if (plugInPath)
    { 
        DIR *directory = opendir(plugInPath);
        for (struct dirent *dirEntry = readdir(directory); dirEntry != NULL;
						dirEntry = readdir(directory))
        {
            ODShellPlugInInstallProc entryPoint = 
                                         load(dirEntry->d_name, 1, plugInPath);
            if (entryPoint)
            {
                // Add it to the list of shell plug-in install functions
                shellPlugInEntry *entry = new shellPlugInEntry;
                entry->procAddr = entryPoint;
		strcpy(entry->name, dirEntry->d_name);
                AddShellPlugInToList(entry);
            }
            else
            {
                // Error message
                ;
            }
        }
    }
}
#endif

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
#ifdef _PLATFORM_WIN32_
			FreeLibrary(entry->hModule);
#endif
#ifdef _PLATFORM_OS2_
                        DosFreeModule(entry->hModule);
#endif
#ifdef _PLATFORM_UNIX_
                        unload(entry->procAddr);
#endif
		}
		if (err)
		    WARNMSG(WARN_INDEX(AMSG_913), 
			"Shell plug-in %s detected an error", entry->name);
	}
	ODDeleteObject(orderColIter);
}
