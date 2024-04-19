//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odcore
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
/* @(#)Z 1.3 com/src/core/Prefs.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:54 (96/10/30 20:21:46) */
/*
	File:		Prefs.cpp

	Contains:	Implementation for Preferences class.

	Owned by:	Caia Grisar

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<58>	10/20/95	CG		#1294494 GM:Info writing size info to the
									wrong prefs file.
		<57>	 9/29/95	eeh		1287262: use ShellDef.h
		<56>	 9/11/95	CG		#1282719 FB2: Core error code mods.
		<55>	  9/8/95	CG		#1282518 FB1:Ignore "bad" aliases.
		<54>	  9/7/95	CG		#1268251 FB1:Editors scan reads unloadable
									editors.
		<53>	  9/5/95	JP		1281666: Catch NULL ptr in AddAETEs
		<52>	 8/30/95	CG		#1255135 FB2:"Updating Part Editor
									database" dialog comes up an indefinitely
									#1278294 FB2:Error code mods in prefs
									files.
									#1274119 FB3:Problems purging editor
									namespaces
									#1257569 FB3:Editor nmaps on read-only
									volumes are not being cached.
									#1273519 FB:Editors database repeatedly
									rebuilt
		<51>	 8/29/95	jpa		Rename Toolbox calls for Univ Hdrs 2.1.
									[1279173]
		<50>	 8/25/95	JP		1272288: Added AETE routines, Resource file
									& cacheFile optimizations
		<49>	 8/22/95	eeh		1276821:
									kOpenDocShellSignature->kODShellSignature
		<48>	 8/16/95	jpa		BlockMove --> ODBlockMove [1258234]
		<47>	  8/8/95	jpa		Allow Libraries/Editors folders to be
									aliases. [1241321]
		<46>	 7/31/95	CG		#1270320: fixed leak left over from fix to
									#1271082.
		<45>	 7/28/95	CG		#1271082: Changed the 3 UserString name
									spaces to be value instead of object -
									leak.
		<44>	 7/27/95	DM		#1270320: Mem leaks: use Temp strings in
									Read_nmap_Buffer
		<43>	 7/24/95	CG		#1270320: Memory leak fixes.
		<42>	 6/22/95	jpa		Notify user when illegal x-vol alias moved
									to trash. [1257556]
		<41>	 6/20/95	T‚		1240820 BB:OpenDoc reports generic error
									when it can't find libs. 1240687 BB:
									OpenDoc gives a cryptic error when there is
									no Editors folder.
		<40>	 6/19/95	jpa		Don't show dialog when reading from cache
									[1258142]
		<39>	  6/9/95	CG		#1203571 implemented  nmap caching
									mechanism implemented.
		<38>	  6/7/95	jpa		Fixed UsingLibRsrcs calls [1255369]
		<37>	 5/25/95	jpa		Rolled <33> back in: keep list of scanned
									editor dirs. Don't get stuck in loops. Only
									rescan when dir mod dates change. [1240150]
		<36>	 5/24/95	CG		#1239535 BB: Added temporary work around.
		<35>	 5/18/95	CG		#1241621	BB: Need to implement Purge for
									Binding subsystem objects.
		<34>	 5/17/95	TJ		Backed out changes from previous checkin.
		<32>	 5/11/95	DM		#1246524, #1246529:
									ValueNameSpaceGetEntry(), must pass through
									the value allocated. #1246920:
									ExtractISOString(), ExtractISOStringList(),
									no need to unregister before registering
									values.
									#1246951: ExtractISOString() memory leak
		<31>	 5/10/95	CG		#1224857	BB: AppleGuide support missing
									from OpenDoc
									#1243604	The constant kNameMappings should
									be renamed to kODNameMappings.
		<30>	 4/26/95	CG		1236615 Prefs.cpp needs to use
									CreateTypeList factory method
		<29>	 4/24/95	eeh		1231707: add and initialize
									kODFileFromEditor namespace
		<28>	  4/5/95	CG		#1216169 BB, #1221135: When Editors folder
									or OpenDoc Libraries folder is missing,
									OpenDoc quits and displays error.
		<27>	 3/24/95	CG		#1217023 API: Read and Write now use
									ODByteArray instead of Platformfile.
		<26>	  3/8/95	eeh		1220615: (for Caia) in ExtractIText, use
									key and value correctly
		<25>	 2/16/95	CG		Suppress fragLibNotFound warning on 68K
									build for now.
		<24>	 2/14/95	TJ		Fixed Copyright date.
		<23>	 2/14/95	jpa		Search all volumes for "Editors" folders &
									register libs. Plus minor tweaks & fixes.
									[1205141]
		<22>	 2/10/95	CG		#11774575: New nmap for editors handling
									platform types now being loaded.
		<21>	  2/2/95	CG		#1205177: folders registered with cfm.
		<20>	 1/25/95	jpa		Fixed no-prefs crashes [1214750]
		<19>	 1/23/95	CG		#1211961: No longer crash if prefs file
									vers resource is invalid.
		<18>	 1/10/95	CG		Removed dead code.
		<17>	12/13/94	CG		#1205141:Prefs code now looks in "Editors"
									and "OpenDoc Libraries" folders for nmaps.
		<16>	 10/7/94	CG		#1184728: If read fails in LoadPreferences
									close and delete the prefs file.
		<15>	 9/12/94	CG		#1157104: GetPartEditorNMAPs now looks in
									same search path as CFM does for Part
									Editor library files.
		<14>	 8/26/94	T‚		#1175107 Temporary fix to allow documents
									to be opened from anywhere, added : on the
									end of CW.PPC in the string constant
		<13>	 8/25/94	CG		#1175107: Temporary fix to allow documents
									to be opened from anywhere.
		<12>	 8/24/94	CG		#1181622: Updated file ownership.
		<11>	 8/22/94	jpa		Fixed Toolbox errors (such as double
									dispose) in UpdatePrefsFileVersion.
									[1182271]
		<10>	 8/17/94	CG		#1181440: Viewer, EditorKinds,
									KindCategory, and all three UserString name
									spaces changed to type ODObjectNameSpace.
		 <9>	 7/29/94	CG		NameSpace API changes, started to get rid
									of TRY blocks as flow of control, got rid
									of ASLM code.
		 <8>	 7/26/94	jpa		Don't call InitObject from InitPreferences.
		 <7>	  7/1/94	CG		Fixed for PPCC compiling.
		 <6>	  7/1/94	CG		Renamed LocateASLMLibFiles to
									LocatePartLibFiles and cleanout some old
									ASLM stuff.
		 <5>	  7/1/94	CG		Uncommented everything except for
									ASLM-based code.
		 <4>	 6/27/94	CG		Moved IsEditorLoaded to BindngH class and
									uncommented some code.
		 <3>	 6/24/94	CG		Uncommented include for UseRsrcM.h.
		 <2>	 6/15/94	CG		Added include for StdDefs.xh.
		 <1>	 6/14/94	CG		first checked in
		 
	To Do:
		
	In Progress:
		
*/

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _PREFS_
#include "Prefs.h"
#endif

#ifndef _PREFSDEF_
#include "PrefsDef.h"
#endif

// this is here only because error codes like kODErrCannotFindOpenDocLibrariesFolder,
// which should be elsewhere, are needed.
#ifndef _SHELLDEF_
#include <ShellDef.h>
#endif

#ifndef _PLFMFILE_
#include "PlfmFile.h"
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include "NmSpcMg.xh"
#endif

#ifndef SOM_ODNameSpace_xh
#include "NamSpac.xh"
#endif

#ifndef SOM_ODObjectNameSpace_xh
#include "ObjectNS.xh"
#endif

#ifndef SOM_ODValueNameSpace_xh
#include "ValueNS.xh"
#endif

#ifndef _ODSESSN_
#include "ODSessn.xh"
#endif

#ifndef SOM_ODTypeList_xh
#include "TypeList.xh"
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef __SCRIPT__
#include <Script.h>
#endif

#ifndef _USERSRCM_
#include "UseRsrcM.h"
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef _STDDEFS_
#include "StdDefs.xh"
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef __ALIASES__
#include <Aliases.h>
#endif

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#include <string.h>

#ifndef __RESOURCES__
#include <Resources.h>
#endif

#ifndef __FOLDERS__
#include <Folders.h>
#endif

#ifndef __FINDER__
#include <Finder.h>		/* For kIsInvisible */
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef _REGISTER_
#include <RegisterFileLibs.h>
#endif

#ifndef _MEMDEBG_
#include <MemDebg.h>	/* Testing only */
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef __ASREGISTRY__
#include <ASRegistry.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
#endif

#pragma segment Preferences


#undef LOGGING
#define LOGGING 0

//==============================================================================
// Constants
//==============================================================================

const ODULong		kNumExpectedParts = 10;

const ODULong		kMinResynchTime = 2 * 60;	// Only check folders every 2 seconds


/* Following is the table of namespaces to be created by Prefs. The first seven
	are also cleared whenever we re-scan the Editors folders. */

struct NameSpaceSpec {
	const char *name;
	ODNSTypeSpec type;
};

const kNPrefNameSpaces = 4;

#define kODFileFromEditor "FileFromEditor"
static const NameSpaceSpec kPrefNameSpaces[kNPrefNameSpaces] = {
		{kODSysPrefEditorKinds,		kODNSDataTypeODValue}
		,{kODSysPrefEditorCategories,kODNSDataTypeODValue}
		,{kODSysPrefContainerSuites,	kODNSDataTypeODValue}
		,{kODFileFromEditor,			kODNSDataTypeODValue}
	};

const kNEditorNameSpaces = 9;

static const NameSpaceSpec kEditorNameSpaces[kNEditorNameSpaces] = {
		{kODContainerSuite,			kODNSDataTypeODValue},	// From OD libraries (Bento)
		{kODKind,					kODNSDataTypeODObject},
		{kODEditorKinds,			kODNSDataTypeODObject},
		//{kODViewer,					kODNSDataTypeODObject},
		{kODViewer,					kODNSDataTypeODValue},
		//{kODEditorUserString,		kODNSDataTypeODObject},
		//{kODKindUserString,			kODNSDataTypeODObject},
		//{kODCategoryUserString,		kODNSDataTypeODObject},
		{kODEditorUserString,		kODNSDataTypeODValue},
		{kODKindUserString,			kODNSDataTypeODValue},
		{kODCategoryUserString,		kODNSDataTypeODValue},
		{kODKindOldMacOSType,		kODNSDataTypeODValue},
		{kODEditorHelpFile,			kODNSDataTypeODValue}
	};

const ODSShort kMaxDirDepth = 5;

#define typeAECache 'aech'

//==============================================================================
// Scalar Types
//==============================================================================

struct cfragHeader {
	ODULong	reserved1;
	ODULong	reserved2;
	ODULong	version;
	ODULong	reserved3;
	ODULong	reserved4;
	ODULong	reserved5;
	ODULong	reserved6;
	ODULong	numFragDescs;
};

typedef cfragHeader cfragHeader;

struct cfragDescHeader {
	ODULong	arch;
	ODULong	updateLevel;
	ODULong	currVersion;
	ODULong	oldDefVer;
	ODULong	appStackSize;
	ODSShort	appLibDir;
	ODSByte	fragType;
	ODSByte	fragLoc;
	ODULong	fragOffset;
	ODULong	fragLen;
	ODULong	reserved1;
	ODULong	reserved2;
	ODSShort	infoRecLen;
};

struct cfragDescriptor {
	cfragDescHeader header;
	Str255			fragName;
};

typedef cfragDescriptor cfragDescriptor;

//==============================================================================
// Global Variables
//==============================================================================

//==============================================================================
// Local Classes
//==============================================================================

class PrefsDirInfo :public Link 
{
public:
	PrefsDirType				fType;			// Type of folder
	ODSShort					fDepth;			// Nesting depth
	short						fVol;			// Volume refNum
	long						fDir;			// Directory ID
	ODULong						fModDate;		// Dir's mod date when last scanned
};


//==============================================================================
// Function Prototype
//==============================================================================

static ODBoolean GetIndVolume( short index, short *vRefNum );
static ODSLong GetFldrDirID( FSSpec theFolderSpec, ODULong *modDate =kODNULL );
static void GetDirModDateAndParent( short vol, long dir, ODULong *modDate, long *parent );
static ODSize ExtractIText(ODSize bufferPos, ODSession* session, ODValueNameSpace*, ODISOStr key);
static ODSize ExtractISOStringList(ODSize bufferPos, ODSession* session, ODObjectNameSpace*, ODISOStr key);
static ODSize ExtractOSType(ODSize bufferPos, ODSession* session, ODValueNameSpace*, ODISOStr key);
static ODSize ExtractPltfrmTypeSpac(ODSize bufferPos, ODSession* session, ODISOStr key);
static ODSize ExtractISOString(ODSize bufferPos, ODSession* session, ODValueNameSpace*, ODISOStr key);
static void FindMagicFolder( OSType folderType, ConstStr255Param folderName,
							 short *vol, long *dir, ODULong *modDate );
static ODBoolean FindVolumeEditorsFolder( short vol, ConstStr255Param, FSSpec * );
static ODSize ExtractHelpFileName(	ODSize bufferPos, 
									ODSession* session, 
									ODFileSpec* fsspec,
									ODValueNameSpace *theNameSpace, 
									ODISOStr key);
static void Cache_nmap( PlatformFile* cacheFile, ODSize bufferSize, Handle mappingPair, FSSpec* fsspec );
static void MoveToTrash( short vRefnum, long dirID, Str255 name );
static void	Read_aete_Resources( FSSpec* fsspec, AEDescList* aetes );
static void	AddAETEs(PlatformFile* cacheFile, ODSLong languageCode, AEDescList* aeteList);
static ODBoolean GetODPrefsFolder(ODSShort* prefVol, ODSLong* prefDir );
static OSErr ConstructCacheFileName(ODSShort fldrVRefNum, ODSLong fldrDirID, 
									  ODULong fldrModDate, Str255* fileName );
static void DeleteOldCacheFiles();
static ODBoolean LoadableLibrary(ODSShort resRefNum);


//==============================================================================
// Class Preferences
//==============================================================================

//------------------------------------------------------------------------------
// Preferences::Preferences
//------------------------------------------------------------------------------
Preferences::Preferences()
{
	fSession			= (ODSession*) kODNULL;
	fNameSpaceMgr		= kODNULL;
	fPrefsFile 			= (PlatformFile*)kODNULL;
	fPrefsModDate		= 0;
	fDirList			= kODNULL;
	fLastSynchedAt		= 0;
	fScanning			= kODFalse;
	fScanningDialog		= kODNULL;
}

//------------------------------------------------------------------------------
// Preferences::~Preferences
//------------------------------------------------------------------------------
Preferences::~Preferences()
{
	ODDeleteObject( fPrefsFile );
	if( fDirList ) {
		fDirList->DeleteAllLinks();
		delete fDirList;
	}
	WASSERT(fScanningDialog==kODNULL);
}

//------------------------------------------------------------------------------
// Preferences::InitPreferences
//------------------------------------------------------------------------------
void Preferences::InitPreferences(ODSession* session)
{
	if( LOGGING ) {
		SetOutputMode(kWriteToDebugWindow);
		LOG("~C");
	}

	fSession = session;
	fNameSpaceMgr = fSession->GetNameSpaceManager(somGetGlobalEnvironment());

	fDirList = new LinkedList;
	
	ODGetString(fLibsFolderName,   kODLibsFldrNameStrID);
	ODGetString(fEditorsFolderName,kODEditorsFldrNameStrID);

		// Create the ContainerSuite, Kind and Category NameSpaces
	CreatePrefsNameSpaces();

		// Initialize fPrefsFile and fPrefsfModDate
	GetOpenDocPrefs();

	// Don't read the Libraries and Editors folders yet; be lazy about this,
	// they'll be read when needed (ScanLibsAndEditors will be called.)
	
	DeleteOldCacheFiles();
}

//------------------------------------------------------------------------------
// Preferences::Purge
//------------------------------------------------------------------------------
ODSize Preferences::Purge(ODSize size)
{
ODUnused(size);
	return 0; //this->ClearEditorNameSpaces();
}

//------------------------------------------------------------------------------
// Preferences::GetOpenDoc_nmaps
//------------------------------------------------------------------------------

void Preferences::GetOpenDoc_nmaps()
{
	ODSShort libsVRefNum;
	ODSLong libsDirID;
	ODULong libsModDate;
	
		// Load any nmaps found in magic OpenDoc Libraries folder
	TRY{
		FindMagicFolder(kExtensionFolderType,fLibsFolderName,
						&libsVRefNum,&libsDirID,&libsModDate);
		this->ScanDirectoryTree(libsVRefNum, libsDirID, libsModDate, kLibsFolder);
	}CATCH_ALL{
		THROW(kODErrCannotFindOpenDocLibrariesFolder);
	}ENDTRY
}

//------------------------------------------------------------------------------
// Preferences::CreateNameSpaces
//------------------------------------------------------------------------------

void Preferences::CreatePrefsNameSpaces()
{
	// This creates the 3 namespaces loaded from Prefs and the container suites.
	
	Environment* ev = somGetGlobalEnvironment ();
	for( ODSLong i=0; i<kNPrefNameSpaces; i++ )
		fNameSpaceMgr->CreateNameSpace(ev, (ODISOStr)kPrefNameSpaces[i].name,
										   kODNULL,kNumExpectedParts,
										   kPrefNameSpaces[i].type);
}

//------------------------------------------------------------------------------
// Preferences::ClearEditorNameSpaces
//------------------------------------------------------------------------------

ODSize Preferences::ClearEditorNameSpaces()
{
	// Clears the namespaces that store the info read from the Editors folders.
	// The other three namespaces are read in from the Prefs file.

	size_t oldFreeSpace;
	MMGetFreeSpace(kODNULL,&oldFreeSpace,kODNULL);
	
	Environment* ev = somGetGlobalEnvironment ();
	for( ODSLong i=0; i<kNEditorNameSpaces; i++ ) {
		ODNameSpace *ns = fNameSpaceMgr->HasNameSpace(ev, (ODISOStr)kEditorNameSpaces[i].name);
		if( ns )
			fNameSpaceMgr->DeleteNameSpace(ev,ns);
		ns = fNameSpaceMgr->CreateNameSpace(ev, (ODISOStr)kEditorNameSpaces[i].name,
										   kODNULL,kNumExpectedParts,
										   kEditorNameSpaces[i].type);
		ns->SetSynchedToEditors(ev,kODTrue);
	}
	
	fDirList->DeleteAllLinks();
	
	size_t newFreeSpace;
	MMGetFreeSpace(kODNULL,&newFreeSpace,kODNULL);
	newFreeSpace -= oldFreeSpace;
	if( (ODSLong)newFreeSpace < 0 )
		newFreeSpace = 0;
	return newFreeSpace;
}


//------------------------------------------------------------------------------
// Preferences::ScanDirectoryTree
//------------------------------------------------------------------------------

void Preferences::ScanDirectoryTree(ODSShort fldrVRefNum, ODSLong fldrDirID,
									ODULong fldrModDate, PrefsDirType type)
{
	LinkedList* dirTreeList = new LinkedList;
	PrefsDirInfo *last = kODNULL;
	PlatformFile* cacheFile = new PlatformFile;
	
		// verify the nmap cache
	ODBoolean cacheIsValid = kODFalse;
	this->VerifyCache(fldrVRefNum, fldrDirID, fldrModDate, type,
											&cacheFile, &dirTreeList, &cacheIsValid);
	
		// if the cache is valid, load the cached data
	if ( cacheIsValid )
		cacheIsValid = this->LoadCachedRezData(cacheFile, fldrVRefNum);
		
	if ( !cacheIsValid )
	{
		this->ShowScanningDialog();				// Tell user we're updating the list
		
		 	// empty the list and start over
		if (!dirTreeList->IsEmpty())
			dirTreeList->RemoveAll();
			//	get current mod date of dir id
		ODULong currentModDate;
		long parent;
		GetDirModDateAndParent(fldrVRefNum, fldrDirID, &currentModDate, &parent);

			// add to local list of scanned directories
		PrefsDirInfo *last = this->AddDirInfo(dirTreeList, fldrVRefNum,fldrDirID,
												currentModDate,1,type);

		ODSLong	sizeOfLong = sizeof( long );
		ODULong	zero = 0;
		
		while( last ) {
			this->TraverseDirectory(last->fVol,last->fDir, last->fDepth, dirTreeList);
				// write this directory id and mod date to cache file
			if (cacheFile)
			{
				cacheFile->Write( (ODSByte*) &last->fDir, &sizeOfLong );
				cacheFile->Write( (ODSByte*) &last->fModDate , &sizeOfLong );
			}
			last = (PrefsDirInfo*) dirTreeList->After((const Link&)*last);
		}
			// write out 0 to signify end of dir list in cache file
		if (cacheFile)
			cacheFile->Write( (ODSByte*) &zero, &sizeOfLong );

			// Loop through directory list loading nmaps and adding nmap data to cache
		PrefsDirInfo *f;
		for( f=(PrefsDirInfo*)dirTreeList->First(); f; 
								f=(PrefsDirInfo*)dirTreeList->After(*f) ) 
		{
			this->ScanDirectory(f->fVol,f->fDir, cacheFile);
		}
	}
	
		// Add this local list to the end of fDirList
	this->AddDirList( dirTreeList );

	if (!dirTreeList->IsEmpty())
		dirTreeList->DeleteAllLinks();
	ODDeleteObject( dirTreeList );
	
	if (cacheFile)
	{
		cacheFile->Close();
		ODDeleteObject( cacheFile );
	}
}

//------------------------------------------------------------------------------
// Preferences::VerifyCache
//------------------------------------------------------------------------------

void Preferences::VerifyCache(ODSShort fldrVRefNum, 
								  ODSLong fldrDirID, 
								  ODULong fldrModDate,
								  PrefsDirType type,
								  PlatformFile** cacheFile,
								  LinkedList** dirTreeList, 
								  ODBoolean* cacheIsValid)
{
	*cacheIsValid = kODTrue;
	
		// attempt to locate the fPrefsFile
	if (!GetCacheFile(cacheFile, fldrVRefNum, fldrDirID, fldrModDate) || 
		!LoadCachedDirList(*cacheFile, fldrVRefNum, dirTreeList, type))
	{
			// If a cache file is invalid or cannot be found
			// attempt to delete an existing file if it exists
		TRY{
			if (*cacheFile && (*cacheFile)->Exists())
				(*cacheFile)->Delete();
		}CATCH_ALL{
			ODDeleteObject (*cacheFile);
		}ENDTRY
			
			// attempt to create a new cache file
		if (*cacheFile)
		{
			TRY{
				(*cacheFile)->Create(kODShellSignature, 
									kPreferencesFolderType, 
									smSystemScript );
									
			}CATCH_ALL{
				
				//CATCH(afpAccessDenied)
				//CATCH(wPrErr)
				//CATCH(vLckdErr)
				//CATCH(dskFulErr)

					// if the cache file cannot be written to volume, create it in prefs
					// folder
				Str255 cacheName;
				ODSShort prefsVol;
				ODSLong prefsDir;
				ODFileSpec cacheFileSpec;

				if(GetODPrefsFolder( &prefsVol, &prefsDir ))
				{
					OSErr result = ConstructCacheFileName( fldrVRefNum, 
						fldrDirID, fldrModDate, &cacheName );
					if (result == noErr)
					{
						result = FSMakeFSSpec(prefsVol, prefsDir, cacheName, &cacheFileSpec);
						if (result == fnfErr )
						{
							(*cacheFile)->Specify(&cacheFileSpec);
							
							TRY{
								(*cacheFile)->Create(kODShellSignature, 
												kPreferencesFolderType, 
												smSystemScript );
							}CATCH_ALL{
								WARN("Error creating cache file");
				if ((*cacheFile)->Exists())
					(*cacheFile)->Delete();
				ODDeleteObject (*cacheFile);
			}ENDTRY
		}
						else
							ODDeleteObject (*cacheFile);
					}
					else
						ODDeleteObject (*cacheFile);
				}
				else
					ODDeleteObject (*cacheFile);

			}ENDTRY

			if (*cacheFile)
			{
				TRY{
						// After creating file, make it invisible:
					ODFileSpec fileSpec = (*cacheFile)->GetFileSpec();
				FInfo info;
				if( HGetFInfo(fldrVRefNum,fldrDirID,fileSpec.name,&info) == noErr ) {
					info.fdFlags |= kIsInvisible;
					HSetFInfo(fldrVRefNum,fldrDirID,fileSpec.name,&info);
				}

					(*cacheFile)->Open();
						// Position mark to beginning of file
					(*cacheFile)->SetFilePos(fsFromStart, 0);
					
				}CATCH_ALL{
					if ((*cacheFile)->Exists())
						(*cacheFile)->Delete();
					ODDeleteObject (*cacheFile);
				}ENDTRY
			}
		}
			// indicate that cached data is not valid
		*cacheIsValid = kODFalse;
	}
}

//------------------------------------------------------------------------------
// Preferences::LoadCachedDirList
//------------------------------------------------------------------------------

ODBoolean Preferences::LoadCachedDirList(PlatformFile* cacheFile,
										  ODSShort fldrVRefNum,
										  LinkedList** dirTreeList,
										  PrefsDirType type)
{
	ODBoolean cacheIsValid = kODFalse;
	PrefsDirType flderType = type;
	
	if ((!cacheFile) || (!cacheFile->Exists()))
		return cacheIsValid;
	
	TRY{
		cacheFile->Open();
			// Position mark to beginning of file
		cacheFile->SetFilePos(fsFromStart, 0);
		
		for ( ODSLong fileOffset = 0; 
			  fileOffset != cacheFile->GetEndOfFile(); 
			  fileOffset = cacheFile->GetFilePos() ) 
		{
			ODULong dirID;
			ODULong modDate;
			ODSLong	sizeoflong = sizeof( long );
			
			//	read in a dir id
			cacheFile->Read( (ODSByte*)&dirID, &sizeoflong );
	
				//	if 0 is read in, end of cached dir list
			if (dirID == 0)
				break;
	
				// read in a modDate
			cacheFile->Read( (ODSByte*)&modDate, &sizeoflong );
	
				//	get current mod date of dir id
			ODULong currentModDate;
			long parent;
			GetDirModDateAndParent(fldrVRefNum, dirID, &currentModDate, &parent);
	
				//	compare it with the one read in
			if (modDate == currentModDate)
			{
						//Add this dir to linked list
					PrefsDirInfo *last = kODNULL;
					last = this->AddDirInfo(*dirTreeList, fldrVRefNum,dirID,modDate,1,flderType );
					last = (PrefsDirInfo*) (*dirTreeList)->After(*last);
					flderType = kNestedFolder;
					cacheIsValid = kODTrue;
			}
			else
			{
				cacheIsValid = kODFalse;
				break;
			}
		}

		if (!cacheIsValid)
			cacheFile->Close();
		else
		{
				// Loop through directory list registering each dir with cfm
			PrefsDirInfo *f;
			for( f=(PrefsDirInfo*)(*dirTreeList)->First(); f; 
									f=(PrefsDirInfo*)(*dirTreeList)->After(*f) ) 
			{
				FSSpec folderSpec;
				OSErr result= FSMakeFSSpec(f->fVol,f->fDir,kODNULL, &folderSpec);
				if( !result )
					result= RegisterFileLibs(&folderSpec);
#if ODDebug
				if( result == noErr)
					LOG("Registered folder (%hd,%ld)\n",f->fVol,f->fDir);
				else if( result != fragDupRegLibName && result != fragLibNotFound )
					WARN("Err %hd registering lib folder (%hd,%ld)",
							result,f->fVol,f->fDir);
#endif
			}
		}

	}CATCH_ALL{
		cacheIsValid = kODFalse;
		cacheFile->Close();
	}ENDTRY
	
	
	return cacheIsValid;
	
}

//------------------------------------------------------------------------------
// Preferences::GetCacheFile
//------------------------------------------------------------------------------

ODBoolean Preferences::GetCacheFile(PlatformFile** cacheFile, ODSShort fldrVRefNum, 
								ODSLong fldrDirID, ODULong fldrModDate )
{
	ODBoolean	cacheFileFound = kODFalse;
	FSSpec		cacheFileSpec;
	
	//Construct an FSSpec
	OSErr result = FSMakeFSSpec(fldrVRefNum, fldrDirID, kODCacheFileName, &cacheFileSpec);
	(*cacheFile)->Specify(&cacheFileSpec);

	if (result == noErr)
		cacheFileFound =  kODTrue;
	else
	{
		ODSShort		prefVol;
		ODSLong			prefDir;
		FSSpec			prefCacheFileSpec;
		
		// check prefs folder for a cache file
		if (GetODPrefsFolder( &prefVol, &prefDir ))
		{
			Str255 cacheName;
			result = ConstructCacheFileName( fldrVRefNum, 
							fldrDirID, fldrModDate, &cacheName );
			if (result == noErr)
				result = FSMakeFSSpec(prefVol, prefDir, cacheName, &prefCacheFileSpec);
			if (result == noErr )
			{
				(*cacheFile)->Specify(&prefCacheFileSpec);
				cacheFileFound =  kODTrue;
			}
		}
	}

	return cacheFileFound;
}

//------------------------------------------------------------------------------
// Preferences::LoadCachedRezData
//------------------------------------------------------------------------------

ODBoolean Preferences::LoadCachedRezData(PlatformFile* cacheFile, ODSShort vRefNum)
{
	ODBoolean 	validCacheFile = kODTrue;
	ODPtr		buffer = kODNULL;
	ODSLong		sizeofsize = sizeof( ODSize );
	ODSLong		sizeoflong = sizeof( long );
	ODSize 		sizeofname;
	ODSize		nmapSize;
	FSSpec		fsspec;
	long		parID;
	Str63		name;
		
	TRY{
		if (cacheFile)
		{
			for ( ODSLong fileOffset = cacheFile->GetFilePos(); 
				  fileOffset != cacheFile->GetEndOfFile(); 
				  fileOffset = cacheFile->GetFilePos() ) 
			{
				cacheFile->Read( (ODSByte*)&parID, &sizeoflong );
				cacheFile->Read( (ODSByte*)&sizeofname, &sizeofsize );
				cacheFile->Read( (ODSByte*)&name[1], (ODSLong*)&sizeofname );
				name[0] = (ODSByte)sizeofname;
				OSErr result = FSMakeFSSpec(vRefNum, parID, name, &fsspec);
				if( result )
				{
					validCacheFile = kODFalse;
					break;
				}

				cacheFile->Read( (ODSByte*)&nmapSize, &sizeofsize );
				buffer = ODNewPtrClear(nmapSize, kDefaultHeapID);
				cacheFile->Read( (ODSByte*) buffer, (ODSLong*)&nmapSize );

				Read_nmap_Buffer(buffer, nmapSize, &fsspec);
				
				ODDisposePtr(buffer);
			}
	
			if (cacheFile && !validCacheFile)
				cacheFile->SetEndOfFile(0);
		}
	
	}CATCH_ALL{
		cacheFile->SetEndOfFile(0);
		validCacheFile = kODFalse;
	}ENDTRY
	
	return validCacheFile;
}

//------------------------------------------------------------------------------
// Preferences::TraverseDirectory
//------------------------------------------------------------------------------

void Preferences::TraverseDirectory( ODSShort fldrVRefNum, ODSLong fldrDirID, 
										ODSShort depth, LinkedList* dirList )
{
	CInfoPBRec		pb;
	FSSpec			fileSpec;
	ODBoolean		anyEditors = kODFalse;
	ODError			result;

		//check that this is not a bogus folder
	if (fldrDirID == -1)
		return;
	
		// Loop through all the files in folder
	memset (&pb,0,sizeof(pb));
	for( pb.hFileInfo.ioFDirIndex = 1; kODTrue; pb.hFileInfo.ioFDirIndex++ )
	{
			//need to do this each time since PBGetCatInfo call returns val here
		pb.hFileInfo.ioNamePtr = (StringPtr)&fileSpec.name;
		pb.hFileInfo.ioVRefNum = fldrVRefNum;
		pb.hFileInfo.ioDirID = fldrDirID;  
		
		if( PBGetCatInfoSync( &pb ) != noErr )
			break;
		
		if ( (pb.dirInfo.ioFlAttrib & (1<<4)) ) 
		{
			// if this is a folder, append it to the list:
			this->AddDirInfo(dirList, fldrVRefNum, pb.dirInfo.ioDrDirID,
										 pb.dirInfo.ioDrMdDat,depth+1, kNestedFolder );
		}
		else 
		{
			Boolean	wasAliased = kODFalse;
			fileSpec.vRefNum = pb.hFileInfo.ioVRefNum;
			fileSpec.parID = pb.hFileInfo.ioFlParID;
			
			if ( (pb.hFileInfo.ioFlFndrInfo.fdFlags & kIsAlias) )
			{
				FSSpec	theSpec;
				Boolean	targetIsFolder = kODFalse;
				
				result = FSMakeFSSpec(fileSpec.vRefNum, fileSpec.parID,
						fileSpec.name, &theSpec);
				if (result == noErr)
				{
						//resolve alias
					result = ResolveAliasFile(&theSpec, kODTrue, &targetIsFolder, &wasAliased);
					if ((result == noErr) && targetIsFolder && (theSpec.vRefNum == fldrVRefNum))
					{
						ODULong modDate;
						long dirID = GetFldrDirID(theSpec,&modDate);
						this->AddDirInfo(dirList, theSpec.vRefNum, dirID, modDate, depth+1, kNestedFolder);
					}
				}
			}
			
		}
	}
}


//------------------------------------------------------------------------------
// Preferences::ScanDirectory
//------------------------------------------------------------------------------

void Preferences::ScanDirectory( ODSShort fldrVRefNum, ODSLong fldrDirID, 
													   PlatformFile* cacheFile )
{
	CInfoPBRec		pb;
	FSSpec			fileSpec;
	ODBoolean		anyEditors = kODFalse;
	ODError			result;
	AEDescList		aetes = {typeNull, kODNULL};
	ODSLong			n;

		//check that this is not a bogus folder
	if (fldrDirID == -1)
		return;
	
	LOG("Scanning folder (%hd,%ld) for editors...\n", fldrVRefNum,fldrDirID);

	THROW_IF_ERROR( AECreateList(kODNULL, 0, kODFalse, &aetes) );

	ODSShort saveCurRefNum = CurResFile();
	
		// Loop through all the files in folder
	memset (&pb,0,sizeof(pb));
	for( pb.hFileInfo.ioFDirIndex = 1; kODTrue; pb.hFileInfo.ioFDirIndex++ )
	{
			//need to do this each time since PBGetCatInfo call returns val here
		pb.hFileInfo.ioNamePtr = (StringPtr)&fileSpec.name;
		pb.hFileInfo.ioVRefNum = fldrVRefNum;
		pb.hFileInfo.ioDirID = fldrDirID;  
		
		if( PBGetCatInfoSync( &pb ) != noErr )
			break;
		
		Boolean	wasAliased = kODFalse;
		fileSpec.vRefNum = pb.hFileInfo.ioVRefNum;
		fileSpec.parID = pb.hFileInfo.ioFlParID;
		
		if ( (pb.hFileInfo.ioFlFndrInfo.fdFlags & kIsAlias) )
		{
			FSSpec	theSpec;
			Boolean	targetIsFolder = kODFalse;
			
			result = FSMakeFSSpec(fileSpec.vRefNum, fileSpec.parID,
					fileSpec.name, &theSpec);
			if (result == noErr)
			{
					//resolve alias
				result = ResolveAliasFile(&theSpec, kODTrue, &targetIsFolder, &wasAliased);
				if (result == noErr)
				{
					if (theSpec.vRefNum == fldrVRefNum)
					{
						if (!targetIsFolder) 
						{
							if ( pb.hFileInfo.ioFlFndrInfo.fdType == kCFragLibraryFileType ) 
							{
								// Use the original file of the alias:
								wasAliased = kODTrue;
								fileSpec.vRefNum = theSpec.vRefNum;
								fileSpec.parID = theSpec.parID;
								CopyPascalString(fileSpec.name, theSpec.name);
							}
						}
					}
					else
					{
						// Aliases to targets on other volumes not allowed!!!
						// need to alert user!
						if ( targetIsFolder || pb.hFileInfo.ioFlFndrInfo.fdType == kCFragLibraryFileType ) {
							MoveToTrash(fileSpec.vRefNum, fileSpec.parID, fileSpec.name);
							fFoundBadAlias = kODTrue;
						}
					}
				}
			}
		}
		
		if ( pb.hFileInfo.ioFlFndrInfo.fdType == kCFragLibraryFileType ) 
		{
			ODBoolean readNMaps = kODFalse;
			short thisFile = HOpenResFile(fileSpec.vRefNum, fileSpec.parID,
					fileSpec.name, fsRdPerm );
			if (thisFile != -1)
			{
				if (LoadableLibrary(thisFile))
				{
					// Neither of these routines will throw out of themselves
					readNMaps =  Read_nmap_Resources(&fileSpec, cacheFile);
					Read_aete_Resources(&fileSpec, &aetes);
				}
				CloseResFile(thisFile);
			}
				
			if( readNMaps && (!anyEditors || wasAliased) ) {
				// There are editors in this folder, so register the folder:
				FSSpec folderSpec;
				result= FSMakeFSSpec(fileSpec.vRefNum,fileSpec.parID,kODNULL, &folderSpec);
				if( !result )
					result= RegisterFileLibs(&folderSpec);
#if ODDebug
				if( result == noErr)
					LOG("Registered folder (%hd,%ld)\n",fileSpec.vRefNum,fileSpec.parID);
				else if( result != fragDupRegLibName && result != fragLibNotFound )
					WARN("Err %hd registering lib folder (%hd,%ld)",
							result,fileSpec.vRefNum,fileSpec.parID);
#endif
				if( !wasAliased )
					anyEditors = kODTrue;
			}
		}
	}

	THROW_IF_ERROR( AECountItems(&aetes, &n) );
	if (n != 0)
	{
		AEDesc	tempRec;
		THROW_IF_ERROR( AECreateList(kODNULL, 0, kODTrue, &tempRec) );
		THROW_IF_ERROR( AEPutKeyDesc(&tempRec, keyDirectObject, &aetes) );
		HLock(tempRec.dataHandle);
		cacheFile->WriteResourcePtr(typeAECache, 0, *tempRec.dataHandle, GetHandleSize(tempRec.dataHandle));
		THROW_IF_ERROR( AEDisposeDesc(&tempRec) );
	};
	THROW_IF_ERROR( AEDisposeDesc(&aetes) );
	
	UseResFile( saveCurRefNum );

	LOG("Done scanning folder (%hd,%ld)\n", fldrVRefNum,fldrDirID);
}


//------------------------------------------------------------------------------
// Preferences::FindDirInfo
//------------------------------------------------------------------------------

PrefsDirInfo*
Preferences::FindDirInfo( LinkedList* dirList, short vol, long dir )
{
	PrefsDirInfo *e;
	for( e=(PrefsDirInfo*)dirList->First(); e; e=(PrefsDirInfo*)dirList->After(*e) )
		if( e->fDir == dir && e->fVol == vol )
		 	return e;						// Already in list, don't add again
	return kODNULL;
}

//------------------------------------------------------------------------------
// Preferences::AddDirInfo
//------------------------------------------------------------------------------

PrefsDirInfo*
Preferences::AddDirInfo( LinkedList* dirList, short vol, long dir, ODULong modDate, 
								ODSShort depth, PrefsDirType type )
{
	if( depth > kMaxDirDepth )
		return kODNULL;						// Too deeply nested
	else if( this->FindDirInfo(dirList, vol,dir ) )
		return kODNULL;						// Already in list
	else {
		LOG("  Add folder (%hd,%ld) to list.\n", vol,dir);
	
		PrefsDirInfo *info = new PrefsDirInfo;
		info->fType = type;
		info->fDepth = depth;
		info->fVol = vol;
		info->fDir = dir;
		info->fModDate = modDate;
		dirList->AddLast(info);
		return info;
	}
}

//------------------------------------------------------------------------------
// Preferences::AddDirList
//------------------------------------------------------------------------------

void Preferences::AddDirList(LinkedList* dirList)
{
	PrefsDirInfo *e;
	for( e=(PrefsDirInfo*)dirList->First(); e; e=(PrefsDirInfo*)dirList->After(*e) )
		this->AddDirInfo(fDirList, e->fVol, e->fDir, e->fModDate, e->fDepth, e->fType);
}

//------------------------------------------------------------------------------
// Preferences::Read_nmap_Resources
//	Utilty routine for reading NMAP resources and creating NameSpaces out of them
//------------------------------------------------------------------------------

ODBoolean Preferences::Read_nmap_Resources( FSSpec* fsspec, PlatformFile* cacheFile )
{
	ODBoolean readNMaps = kODFalse;

	LOG("* Reading 'nmap's from file '%P' in (%hd,%ld)\n",fsspec->name,
			fsspec->vRefNum,fsspec->parID);
	TRY{
		ODSShort mappingCount = Count1Resources( kODNameMappings );
		for (short i=1; i<=mappingCount; i++) 
		{
				// Get the Resource
			Handle mappingPair = Get1IndResource(kODNameMappings, i);
			ODError result = ResError();
			
			if ( mappingPair != nil && !result )
			{
				ODLockHandle((ODHandle) mappingPair);
				
				ODSize bufferSize = ODGetHandleSize((ODHandle)mappingPair);
				Cache_nmap(cacheFile, bufferSize, mappingPair, fsspec);
				Read_nmap_Buffer( (ODPtr)*mappingPair, bufferSize, fsspec );
				readNMaps = kODTrue;
				
				ReleaseResource( mappingPair );
			}
		}
	}CATCH_ALL{
		WARN("Error %ld reading nmaps for file %#s",ErrorCode(),fsspec->name);
	}ENDTRY
	return readNMaps;
}

//------------------------------------------------------------------------------
// Preferences::Read_nmap_Buffer
//------------------------------------------------------------------------------

void Preferences::Read_nmap_Buffer(ODPtr buffer, ODSize bufferSize,
		ODFileSpec* fsspec)
{
	ODObjectNameSpace *theNameSpace;
	ODSize			bufferPos=(ODSize)buffer;
	ODSize			bufferEnd=(ODSize)buffer+bufferSize;
	ODUShort		keyCount;
	ODUShort		valueType;
	ODSize			sizeShort = sizeof(ODUShort);
	ODSize			sizeLong = sizeof(ODULong);
	Environment* 	ev = somGetGlobalEnvironment ();
	
	ODUShort		nameLen;
	ODBlockMove((ODPtr)bufferPos, (ODPtr)&nameLen, sizeShort);
	bufferPos += sizeShort;

	TRY
		TempODISOStr nameSpaceName = 
			(ODISOStr)ODNewPtrClear((ODSize)nameLen+1, kDefaultHeapID);
		ODBlockMove((ODPtr)bufferPos, (ODPtr)(ODISOStr)nameSpaceName, (ODSize)nameLen);
		bufferPos += (ODSize)nameLen;
		
		ODBoolean editorAvail = ODISOStrEqual( nameSpaceName, kODEditorUserString );
		
		/*	In general, we will get the namespace from the string read here and then pass it
			to the routine that reads the data. The exception is kODEditorPlatformKind
			which is not a real namespace, and whose entries are automatically read into
			the kODEditorKinds namespace. */
		
		theNameSpace = (ODObjectNameSpace*)(fNameSpaceMgr->HasNameSpace(ev, nameSpaceName));

		if( ! theNameSpace && !ODISOStrEqual(nameSpaceName,kODEditorPlatformKind) )
			WARN("Unknown nmap namespace '%s'", (ODISOStr) nameSpaceName);
		else {
			ODBlockMove((ODPtr)bufferPos, (ODPtr)&keyCount, sizeShort);
			bufferPos += sizeShort;
			
			for(ODUShort i=1; i<= keyCount; i++)
			{
				ODUShort keyLen;
				ODBlockMove((ODPtr)bufferPos, (ODPtr)&keyLen, sizeShort);
				bufferPos += sizeShort;
		
				TempODISOStr key = 
					(ODISOStr)ODNewPtrClear((ODSize)keyLen+1, kDefaultHeapID);
				ODBlockMove((ODPtr)bufferPos, (ODPtr)(ODISOStr)key, (ODSize)keyLen);
				bufferPos += (ODSize)keyLen;
		
				if ( editorAvail )
				{
					// put it in the namespace here....
					ODValueNameSpace* editorToFileNS = (ODValueNameSpace*)
							(fNameSpaceMgr->HasNameSpace(ev, kODFileFromEditor));
					if ( editorToFileNS )
						ValueNameSpaceRegister( editorToFileNS, ev, key,
								(ODPtr)fsspec, sizeof(*fsspec) );
				}
		
				ODBlockMove((ODPtr)bufferPos, (ODPtr)&valueType, sizeShort);
				bufferPos += sizeShort;
		
				switch( valueType )
				{
						// If the value type is international text read ODName 
					case kODIsINTLTextID:
						bufferPos = ExtractIText(bufferPos, fSession, 
												(ODValueNameSpace*)theNameSpace, key);
						break;
					
						// If the value type is an ISOString list read ODTypeList 
					case kODIsAnISOStringListID:
						bufferPos = ExtractISOStringList(bufferPos, fSession, theNameSpace, key);
						break;
						
						// If the value type is an OSType read ODOSType 
					case kODIsMacOSTypeID:
						bufferPos = ExtractOSType(bufferPos, fSession,
													(ODValueNameSpace*)theNameSpace, key);
						break;
						
						// If the value type is an ISOString read ISOString 
					case kODIsAnISOStringID:
						bufferPos = ExtractISOString(bufferPos, fSession,
													 (ODValueNameSpace*)theNameSpace, key);
						break;
					
						// If the value type is a Platform Type Spec read ODPlatformTypeSpec
					case kODIsPltfmTypeSpacID:
						bufferPos = ExtractPltfrmTypeSpac(bufferPos, fSession, key);
						break;
						
						// If the value type is a file name for an AppleGuile help file
					case kODIsHelpFileNameID:
						bufferPos = ExtractHelpFileName(bufferPos, fSession, fsspec,
													(ODValueNameSpace*)theNameSpace, key);
						break;
						
					default:
						break;
				}
			}
		}

	CATCH_ALL
		WARN("Error %ld in Read_nmap_buffer",ErrorCode());
	ENDTRY
	
	return;
}

//------------------------------------------------------------------------------
// Preferences::Write_nmap_Buffer
//------------------------------------------------------------------------------

void Preferences::Write_nmap_Buffer(ODPtr buffer, ODSize bufferSize)
{
}

//------------------------------------------------------------------------------
// Preferences::GetOpenDocPrefs
//------------------------------------------------------------------------------

void Preferences::GetOpenDocPrefs( )
{
		// attempt to locate the fPrefsFile
	if (!LocatePrefsFile() || 
		!VerifyPrefsFileVersion() || 
		!LoadPreferences())
	{
			// If any of the above fail, create the file
		if (fPrefsFile)
		{
			TRY{
				fPrefsFile->Create(kODShellSignature, 
									kPreferencesFolderType, 
									smSystemScript );
				fPrefsFile->CreateResFile();
				fPrefsFile->SetPermission(fsRdWrPerm);
				UpdatePrefsFileVersion();
			}CATCH_ALL{
				WARN("Error %ld reading prefs file: deleting it",ErrorCode());
				fPrefsFile->Delete();
				fPrefsFile = kODNULL;
			}ENDTRY
		}
	}
}

//------------------------------------------------------------------------------
// Preferences::LocatePrefsFile
//------------------------------------------------------------------------------

ODBoolean Preferences::LocatePrefsFile()
{
	ODError 		result;
	HParamBlockRec	theParamBlock;
	Str255			prefFileName;
	ODSShort		prefVol;
	ODSLong			prefDir;
	ODFileSpec		prefFileSpec;
	ODBoolean		retVal = kODFalse;

	if (!GetODPrefsFolder( &prefVol, &prefDir ))
		return kODFalse;
	
	ODGetString(prefFileName,kODPrefsFileNameStrID);

		//Construct an FSSpec
	result = FSMakeFSSpec(prefVol, prefDir, prefFileName, &prefFileSpec);
					
	if (!fPrefsFile)
	{
		fPrefsFile = new PlatformFile;
		TRY{
			fPrefsFile->Specify(&prefFileSpec);
		}CATCH_ALL{
			fPrefsFile->Delete();
			fPrefsFile = kODNULL;
			return kODFalse;
		}ENDTRY
	}

		//try to find the file	
		// if this file does not exists return false
	if (result) return kODFalse;
		
	memset (&theParamBlock,0,sizeof(HParamBlockRec));
	theParamBlock.fileParam.ioNamePtr = (StringPtr)&prefFileName;
	theParamBlock.fileParam.ioVRefNum = prefVol;
	theParamBlock.fileParam.ioFDirIndex = 0;
	theParamBlock.fileParam.ioDirID = prefDir;
	if( PBHGetFInfoSync(&theParamBlock) == noErr )
		fPrefsModDate = theParamBlock.fileParam.ioFlMdDat;
	return kODTrue;
}

//------------------------------------------------------------------------------
// Preferences::VerifyPrefsFileVersion
//------------------------------------------------------------------------------

ODBoolean Preferences::VerifyPrefsFileVersion()
{
	ODSShort		result;
	ODSShort		saveCurRefNum;
	ODBoolean		retVal=kODFalse;
	
			//Get OpenDoc 'vers' resource
	VersRecHndl odVers;
	{ CUsingLibraryResources r;
		odVers = (VersRecHndl)Get1Resource( 'vers', 1 );
		result = ResError();
		if (odVers )
			DetachResource((Handle)odVers);
	}
	
	if (odVers )
	{
			//Open the resource fork of the Prefs file
		saveCurRefNum = CurResFile();
		fPrefsFile->OpenResFile();
		
			//Attempt to read 'vers' resource from prefs file
		VersRecHndl prefVers = (VersRecHndl)Get1Resource('vers', 1);
		result = ResError();
		if (!prefVers)
		{
			TRY{
				fPrefsFile->CloseResFile();
				fPrefsFile->Delete();
			}CATCH_ALL{
			}ENDTRY
		}
		else
		{
				//Compare prefs 'vers' w/ OpenDoc 'vers'
			if( ((*prefVers)->numericVersion.majorRev == 
									  (*odVers)->numericVersion.majorRev) &&
				((*prefVers)->numericVersion.minorAndBugRev == 
				                (*odVers)->numericVersion.minorAndBugRev) &&
				((*prefVers)->numericVersion.stage == 
				  						 (*odVers)->numericVersion.stage) &&
				((*prefVers)->numericVersion.nonRelRev == 
										(*odVers)->numericVersion.nonRelRev)
			  )
			{
				retVal = kODTrue;
				fPrefsFile->CloseResFile();
			}
			else
			{
				fPrefsFile->CloseResFile();
				fPrefsFile->Delete();
			}
		}	
				
		DisposeHandle((Handle)odVers);
		UseResFile( saveCurRefNum );
	}

	return retVal;
}

//------------------------------------------------------------------------------
// Preferences::LoadPreferences
//------------------------------------------------------------------------------

ODBoolean Preferences::LoadPreferences()
{
	ODULong			nameSpaceNameLength;
	ODISOStr		nameSpaceName;
	ODValueNameSpace* 	theNameSpace;
	Environment* 	ev = somGetGlobalEnvironment ();
	

		// If there is a pref file, open it
	if (fPrefsFile)
	{
		TRY{
			fPrefsFile->Open();
			
				// Position mark to beginning of file
			fPrefsFile->SetFilePos(fsFromStart, 0);
			
			for ( ODSLong fileOffset = 0; 
				  fileOffset != fPrefsFile->GetEndOfFile(); 
				  fileOffset = fPrefsFile->GetFilePos() ) 
			{
					// Get each "mapping"
				nameSpaceNameLength = ReadSize(fPrefsFile->GetFileRefNum(), 
												   (ODStorageUnitView*)kODNULL);
				nameSpaceName = (ODISOStr)ODNewPtrClear(nameSpaceNameLength+1,
															    kDefaultHeapID);
				TRY{
					ReadBytes(fPrefsFile->GetFileRefNum(), 
									(ODStorageUnitView*)kODNULL, nameSpaceName, 
									&nameSpaceNameLength);
					theNameSpace = (ODValueNameSpace*)fNameSpaceMgr->HasNameSpace(ev,
																		nameSpaceName);
					// Create the namespace if it's unknown  --jpa
					if( !theNameSpace ) {
						WARN("Unknown namespace '%s' in prefs",nameSpaceName);
						theNameSpace = (ODValueNameSpace*)
								fNameSpaceMgr->CreateNameSpace(ev,nameSpaceName,
															kODNULL,kNumExpectedParts,
															kODNSDataTypeODValue);
					}
					ValueNameSpaceReadFromFile(theNameSpace, fPrefsFile);
						
				}CATCH_ALL{
					ODDisposePtr(nameSpaceName);
					RERAISE;
				}ENDTRY

				ODDisposePtr(nameSpaceName);
			}
	
			fPrefsFile->Close();
	
		}CATCH_ALL{
			fPrefsFile->Close();
			fPrefsFile->Delete();
			return kODFalse;
		}ENDTRY
		
		return kODTrue;
	}
	else
		return kODFalse;
}

//------------------------------------------------------------------------------
// Preferences::UpdatePreferences
//------------------------------------------------------------------------------

void Preferences::UpdatePreferences()
{
	ODValueNameSpace*	theNameSpace=kODNULL;
	ODSLong			sizeLong = sizeof(ODULong);
	Environment* 	ev = somGetGlobalEnvironment ();
	
	
		// If there is a pref file, update it
	if (fPrefsFile)
	{
			// Open the file
		fPrefsFile->Open();
		
			// Release current contents of file
		fPrefsFile->SetEndOfFile( 0 );
		
			// Position mark to beginning of file
		fPrefsFile->SetFilePos(fsFromStart, 0);

			// Get SysPrefEditorKinds nameSpace
		ODValueNameSpace* sysPrefEditorKindsNameSpace = (ODValueNameSpace*)
			fNameSpaceMgr->HasNameSpace( ev,
										 kODSysPrefEditorKinds );

		if (sysPrefEditorKindsNameSpace != kODNULL)
		{
			TempODISOStr nsName = sysPrefEditorKindsNameSpace->GetName(ev); // ensure deletion
			ODULong nsNameLength = ODISOStrLength((ODISOStr) nsName);

				// write length of name of name space to prefs
			fPrefsFile->Write( (const ODSByte*)&nsNameLength, &sizeLong );
				// write name of name space to prefs
			fPrefsFile->Write( (const ODSByte*) (ODISOStr) nsName, (ODSLong*)&nsNameLength );
				// write out entire namespace to prefs
			ValueNameSpaceWriteToFile( sysPrefEditorKindsNameSpace, fPrefsFile );
		}

			// Get SysPrefEditorCategories nameSpace
		ODValueNameSpace* sysPrefEditorCategoriesNameSpace = 
			(ODValueNameSpace*)(fNameSpaceMgr->
									HasNameSpace(ev, kODSysPrefEditorCategories ));

		if (sysPrefEditorCategoriesNameSpace != kODNULL)
		{
			TempODISOStr nsName = sysPrefEditorCategoriesNameSpace->GetName(ev); // ensure deletion
			ODULong nsNameLength = ODISOStrLength((ODISOStr) nsName);

				// write length of name of name space to prefs
			fPrefsFile->Write( (const ODSByte*) &nsNameLength, &sizeLong );
				// write name of name space to prefs
			fPrefsFile->Write( (const ODSByte*) (ODISOStr) nsName, (ODSLong*)&nsNameLength );
				// write out entire namespace to prefs
			ValueNameSpaceWriteToFile( sysPrefEditorCategoriesNameSpace, fPrefsFile );
		}

			// Get SysPrefContainerSuites nameSpace
		ODValueNameSpace* containerSuiteNameSpace = (ODValueNameSpace*)
			(fNameSpaceMgr->HasNameSpace( ev, kODSysPrefContainerSuites ));

		if (containerSuiteNameSpace != kODNULL)
		{
			TempODISOStr nsName = containerSuiteNameSpace->GetName(ev); // ensure deletion
			ODULong nsNameLength = ODISOStrLength((ODISOStr) nsName);

				// write length of name of name space to prefs
			fPrefsFile->Write( (const ODSByte*) &nsNameLength, &sizeLong );
				// write name of name space to prefs
			fPrefsFile->Write( (const ODSByte*) (ODISOStr) nsName, (ODSLong*)&nsNameLength );
				// write out entire namespace to prefs
			ValueNameSpaceWriteToFile( containerSuiteNameSpace, fPrefsFile );
		}
		
			// Close the file
		fPrefsFile->Close();
	}
}

//------------------------------------------------------------------------------
// Preferences::UpdatePrefsFileVersion
//------------------------------------------------------------------------------

void Preferences::UpdatePrefsFileVersion()
{
	OSErr			result;
	ODSShort		saveCurRefNum;

		//Get OpenDoc 'vers' resource
	ODSLong 		savedRefNum;
	BeginUsingLibraryResources(savedRefNum);

	Handle odVers = Get1Resource( 'vers', 1 );
	result = ResError();
	if (odVers) {
		DetachResource(odVers);
		result = ResError();
	} else
		if( result==noErr ) result=resNotFound;

	EndUsingLibraryResources(savedRefNum);
	
	THROW_IF_ERROR(result);
	
		//Open the resource fork of the Prefs file
	saveCurRefNum = CurResFile();
	fPrefsFile->OpenResFile();
		//Write OpenDoc 'vers' resource to prefs file
	Handle prefVers = Get1Resource( 'vers', 1 );
	if (prefVers==kODNULL) {
		result= ResError();
		if( result==noErr || result==resNotFound ) {
			AddResource(odVers, 'vers', 1, "\p");
			result= ResError();
		}
		if( !result )
			odVers = kODNULL;	// It's been adopted by fPrefsFile
	} else {
		Size odSize = GetHandleSize(odVers);
		SetHandleSize(prefVers, odSize);
		result= MemError();
		if( !result ) {
			ODBlockMove(*odVers,*prefVers,odSize);
			ChangedResource(prefVers);
			result = ResError();
		}
	}
	fPrefsFile->CloseResFile();
	UseResFile( saveCurRefNum );
	if( odVers ) DisposeHandle( odVers );
	
	THROW_IF_ERROR(result);
}

//------------------------------------------------------------------------------
// Preferences::HaveEditorsFoldersChanged
//------------------------------------------------------------------------------

ODBoolean Preferences::HaveEditorsFoldersChanged()
{
	if( fScanning )
		return kODFalse;			// Skip re-entrant scans
		
	// Don't even scan folders more often than once every kMinResynchTime ticks:
	if( (ODULong)(TickCount() - fLastSynchedAt) <= kMinResynchTime )
		return kODFalse;
		
	// First find the current Editors folder in the System folder:
	FSSpec editorsSpec;
	ODULong editorsModDate;
	TRY{
		FindMagicFolder(kSystemFolderType,fEditorsFolderName,
							&editorsSpec.vRefNum,&editorsSpec.parID,&editorsModDate);
	}CATCH_ALL{
		WARN("Error %ld reading system editors folder",ErrorCode());
		THROW(kODErrCannotFindSystemEditorsFolder);
	}ENDTRY

	// Check if any of the scanned folders have had their mod date bumped or are deleted:
	ODBoolean foundSysEditorsFolder = kODFalse;
	PrefsDirInfo *f;
	for( f=(PrefsDirInfo*)fDirList->First(); f; f=(PrefsDirInfo*)fDirList->After(*f) ) {
		ODULong modDate;
		long parent;
		GetDirModDateAndParent(f->fVol,f->fDir, &modDate,&parent);
		
		if( modDate != f->fModDate )
			return kODTrue;						// Folder's been changed
		else if( f->fType==kRootEditorsFolder ) {
			if( parent != fsRtDirID )
				return kODTrue;					// Root editors folder's been moved
		} else if( f->fType==kSysEditorsFolder ) {
			if( f->fDir != editorsSpec.parID )
				return kODTrue;					// Sys editors folder not same as current one
			foundSysEditorsFolder = kODTrue;
		}
	}
	
	// If the current sys editors folder isn't in the list, something's changed:
	if( !foundSysEditorsFolder )
		return kODTrue;
	
	// Look for each vol's Editors folder and see if it's a different folder:
	short vol;
	for( ODSShort vIndex=1; GetIndVolume(vIndex,&vol); vIndex++ ) {
		TRY{
			if( FindVolumeEditorsFolder(vol,fEditorsFolderName,&editorsSpec) ) {
				if( !this->FindDirInfo(fDirList, vol,GetFldrDirID(editorsSpec)) )
					return kODTrue;
			}
		}CATCH_ALL{
			return kODTrue;
		}ENDTRY
	}
	
	fLastSynchedAt = TickCount();				// Don't check again for another few seconds
	return kODFalse;
}

//------------------------------------------------------------------------------
// Preferences::ScanLibsAndEditors
//------------------------------------------------------------------------------

ODBoolean Preferences::ScanLibsAndEditors()
{
	/* First, check whether the Editors folders have changed. If not, do nothing.
	   Also disable this routine completely if it's being called re-entrantly, by
	   checking the fScanning flag. This is quite likely to happen because the
	   namespace manager calls me anytime anyone asks for any of the editor name-
	   spaces, which is something I do when rebuilding them. */
	   
	if( ! this->HaveEditorsFoldersChanged() )
		return kODFalse;
	
	fScanning = kODTrue;
	fFoundBadAlias = kODFalse;
	
	TRY{
		FSSpec editorsSpec;
		
		// Clear out the name spaces, we're starting from scratch:
		this->ClearEditorNameSpaces();
		
		// First search all 'nmap's in the OpenDoc Libraries folder:
		this->GetOpenDoc_nmaps();
		
		// Search the "Editors" folder in the System folder:
		TRY{
				ODULong editorsModDate;
				FindMagicFolder(kSystemFolderType,fEditorsFolderName,
									&editorsSpec.vRefNum,&editorsSpec.parID,&editorsModDate);
				this->ScanDirectoryTree(editorsSpec.vRefNum,editorsSpec.parID,
										editorsModDate,kSysEditorsFolder);
		}CATCH_ALL{
			WARN("Error %ld reading system editors folder",ErrorCode());
			THROW(kODErrCannotFindSystemEditorsFolder);
		}ENDTRY
		
		// Now for each mounted volume, find and search its "Editors" folder:
		short vol;
		for( ODSShort vIndex=1; GetIndVolume(vIndex,&vol); vIndex++ ) {
			TRY{
					if( FindVolumeEditorsFolder(vol,fEditorsFolderName,&editorsSpec) ) {
						ODULong modDate;
						ODSLong dir = GetFldrDirID(editorsSpec,&modDate);
						this->ScanDirectoryTree( vol, dir, modDate, kRootEditorsFolder );
				}
			}CATCH_ALL{
				WARN("Error %ld reading root editors folder on volume %hd",ErrorCode(),vol);
			}ENDTRY
		}
		
		fLastSynchedAt = ::TickCount();
		fScanning = kODFalse;
		this->CloseScanningDialog();
		
	}CATCH_ALL{
		fScanning = kODFalse;
		this->CloseScanningDialog();
		RERAISE;
	}ENDTRY
	
	if( fFoundBadAlias ) {
		// Tell user she can't put aliases to other volumes in an Editors folder.
		// I've avoided using a filterProc to avoid re-entrancy problems since who
		// knows who might have triggered this scan by innocently asking for a
		// namespace? --jpa
		CUsingLibraryResources r;
		::CautionAlert(kODBadAliasInEditorsAlertID,kODNULL);
	}
	
	return kODTrue;
}


void
Preferences::ShowScanningDialog( )
{
	if( !fScanningDialog ) {
		CUsingLibraryResources r;
		fScanningDialog = ::GetNewDialog(kODScanEditorsDialogID,kODNULL,(WindowPtr)-1L);
		::ShowWindow(fScanningDialog);
		::DrawDialog(fScanningDialog);
	}
}


void
Preferences::CloseScanningDialog( )
{
	if( fScanningDialog ) {
		CUsingLibraryResources r;
		::DisposeDialog(fScanningDialog);
		fScanningDialog = kODNULL;
	}
}


void
Preferences::GetAETEs( ODSLong languageCode, AEDescList* aeteList)
{
	/*	First, make sure the cache is up to date, which should be a noop. 
	 *	Then walk the Editors Folder cache files and accumulate aete resources
	 *	with the proper language code and return them in the aeteList.
	 */
	   
	FSSpec editorsSpec;
	PlatformFile* cacheFile = kODNULL;

	this->ScanLibsAndEditors();
	cacheFile = new PlatformFile;
	
	TRY
		// Search the "Editors" folder in the System folder:
		TRY{
				ODULong editorsModDate;
				FindMagicFolder(kSystemFolderType, fEditorsFolderName,
						&editorsSpec.vRefNum, &editorsSpec.parID, &editorsModDate);
				this->GetCacheFile(&cacheFile, editorsSpec.vRefNum, editorsSpec.parID, editorsModDate);
				AddAETEs(cacheFile, languageCode, aeteList);
		}CATCH_ALL{
			WARN("Error %ld reading system editors folder",ErrorCode());
			THROW(kODErrCannotFindSystemEditorsFolder);
		}ENDTRY
		
		// Now for each mounted volume, find and search its "Editors" folder:
		short vol;
		for( ODSShort vIndex=1; GetIndVolume(vIndex,&vol); vIndex++ ) {
			TRY{
				if( FindVolumeEditorsFolder(vol, fEditorsFolderName, &editorsSpec) ) {
					ODULong modDate;
					ODSLong dir = GetFldrDirID(editorsSpec, &modDate);
					this->GetCacheFile(&cacheFile, vol, dir, modDate);
					AddAETEs(cacheFile, languageCode, aeteList);
				}
			}CATCH_ALL{
				WARN("Error %ld reading root editors folder on volume %hd",ErrorCode(),vol);
			}ENDTRY
		}
	CATCH_ALL
		ODDeleteObject(cacheFile);
		RERAISE;
	ENDTRY
	ODDeleteObject(cacheFile);
}	// GetAETEs


//------------------------------------------------------------------------------
// GetPrefsFile
//------------------------------------------------------------------------------
PlatformFile* Preferences::GetPrefsFile()
{
	return fPrefsFile;
}

//------------------------------------------------------------------------------
// FindMagicFolder
//------------------------------------------------------------------------------
static void FindMagicFolder( OSType folderType, ConstStr255Param folderName,
							 short *vol, long *dir, ODULong *modDate )
{
	short	sysFldrVRefNum=0;
	long	sysFldrDirID=0;
	ODError result = noErr;
	
	CInfoPBRec		pb;
	memset (&pb,0,sizeof(pb));

	THROW_IF_ERROR( FindFolder(kOnSystemDisk, folderType, kDontCreateFolder, 
								&pb.dirInfo.ioVRefNum, &pb.dirInfo.ioDrDirID) );
	Str63 name;
	CopyPascalString(name,folderName);
	pb.dirInfo.ioNamePtr = name;
	pb.dirInfo.ioFDirIndex = 0;
	THROW_IF_ERROR( PBGetCatInfoSync( &pb ) );
	
	if( pb.hFileInfo.ioFlFndrInfo.fdFlags & kIsAlias ) {
		FSSpec spec;
		ODBoolean targetIsFolder, wasAliased;
		FSMakeFSSpec(pb.dirInfo.ioVRefNum,pb.dirInfo.ioDrParID,name, &spec);
		THROW_IF_ERROR( ResolveAliasFile(&spec, kODTrue, &targetIsFolder, &wasAliased) );
		if( !targetIsFolder )
			THROW(dirNFErr);
		// Now get dir ID of target folder:
		pb.dirInfo.ioNamePtr = spec.name;
		pb.dirInfo.ioFDirIndex = 0;
		pb.dirInfo.ioVRefNum = spec.vRefNum;
		pb.dirInfo.ioDrDirID = spec.parID;
		THROW_IF_ERROR( PBGetCatInfoSync( &pb ) );
	}
	
	*vol = pb.dirInfo.ioVRefNum;
	*dir = pb.dirInfo.ioDrDirID;
	*modDate = pb.dirInfo.ioDrMdDat;
}

//------------------------------------------------------------------------------
// FindVolumeEditorsFolder
//------------------------------------------------------------------------------
static ODBoolean
FindVolumeEditorsFolder( short vol, ConstStr255Param localizedFolderName, FSSpec *folderSpec )
{
	// Look for the magic invisible hint file at the root, which contains the folder name:
	OSErr err;
	short fil;
	ODBoolean targetIsFolder, wasAliased;
	
	if( FSMakeFSSpec(vol,fsRtDirID,kODEditorHintFileName, folderSpec) == noErr ) {
		short fil;
		Str63 name;
		err= FSpOpenDF(folderSpec,fsRdPerm,&fil);
		if( !err ) {
			Size size = 64;
			err= FSRead(fil,&size,name);
			if( err==eofErr && size==1L+name[0] )
				err= noErr;
			FSClose(fil);
		}
		if( !err )
			err= FSMakeFSSpec(vol,fsRtDirID,name, folderSpec);
		if( !err )
		{
			err = ResolveAliasFile(folderSpec, kODTrue, &targetIsFolder, &wasAliased);
			if( targetIsFolder )
				return kODTrue;
		}
	}
	
	// If there's no hint file or it lied to us, look for the current localized name:
	err = FSMakeFSSpec(vol,fsRtDirID,localizedFolderName, folderSpec);
	if( !err )
	{
		err = ResolveAliasFile(folderSpec, kODTrue, &targetIsFolder, &wasAliased);
		if( !targetIsFolder )
			return kODFalse;
	}
	
	if( err != noErr )
		return kODFalse;
	
	// We've found the folder, so create or update the hint file:
	err= HCreate(vol,fsRtDirID,kODEditorHintFileName, 'odtm','\?\?\?\?');
	if( err==noErr ) {
		// After creating file, make it invisible:
		FInfo info;
		if( HGetFInfo(vol,fsRtDirID,kODEditorHintFileName,&info) == noErr ) {
			info.fdFlags |= kIsInvisible;
			HSetFInfo(vol,fsRtDirID,kODEditorHintFileName,&info);
		}
	} else if( err!=dupFNErr ) {
		WARN("Couldn't create/update hint file, err %hd",err);
		return kODTrue;
	}
	
	// Update the file:
	err= HOpen(vol,fsRtDirID,kODEditorHintFileName, fsWrPerm, &fil);
	if( !err ) {
		long size = 1+localizedFolderName[0];
		if( !err )
			err= FSWrite(fil,&size,localizedFolderName);
		if( !err )
			err= SetEOF(fil,size);
		FSClose(fil);
		FlushVol(kODNULL,vol);
	}
	if( err )
		WARN("Err %hd writing editor hint file on vol %hd",err,vol);
	return kODTrue;
}


//------------------------------------------------------------------------------
// GetIndVolume
//------------------------------------------------------------------------------
static ODBoolean
GetIndVolume( short index, short *vRefNum )
{
	WASSERT(index>0);
	HParamBlockRec pb;
	pb.volumeParam.ioCompletion = NULL;
	pb.volumeParam.ioNamePtr = NULL;
	pb.volumeParam.ioVolIndex = index;
	
	if( PBHGetVInfoSync(&pb) != noErr )
		return kODFalse;
	*vRefNum = pb.volumeParam.ioVRefNum;
	return kODTrue;
}


//------------------------------------------------------------------------------
// GetFldrDirID
//------------------------------------------------------------------------------
static ODSLong
GetFldrDirID( FSSpec theFolderSpec, ODULong *modDate /*=NULL*/ )
{
	ODSLong dirID=-1;
	CInfoPBRec	theParamBlock;
	memset (&theParamBlock,0,sizeof(theParamBlock));
	theParamBlock.dirInfo.ioFDirIndex = 0;
	theParamBlock.dirInfo.ioNamePtr = theFolderSpec.name;
	theParamBlock.dirInfo.ioVRefNum = theFolderSpec.vRefNum;
	theParamBlock.dirInfo.ioDrDirID = theFolderSpec.parID;
	ODError result = PBGetCatInfoSync((CInfoPBPtr)&theParamBlock);
	if (result == noErr && (theParamBlock.hFileInfo.ioFlAttrib & (1<<4)) ) {
		dirID = theParamBlock.dirInfo.ioDrDirID;
		if( modDate )
			*modDate = theParamBlock.dirInfo.ioDrMdDat;
	}

	return dirID;
}


//------------------------------------------------------------------------------
// GetDirModDateAndParent
//------------------------------------------------------------------------------
static void
GetDirModDateAndParent( short vol, long dir, ODULong *modDate, long *parent )
{
	CInfoPBRec	pb;
	memset (&pb,0,sizeof(pb));
	pb.dirInfo.ioFDirIndex = -1;
	pb.dirInfo.ioNamePtr = kODNULL;
	pb.dirInfo.ioVRefNum = vol;
	pb.dirInfo.ioDrDirID = dir;
	ODError result = PBGetCatInfoSync((CInfoPBPtr)&pb);
	if( result )
		*modDate = *parent = 0;
	else {
		*modDate = pb.dirInfo.ioDrMdDat;
		*parent = pb.dirInfo.ioDrParID;
	}
}


//------------------------------------------------------------------------------
// ExtractIText
//------------------------------------------------------------------------------
static ODSize ExtractIText(ODSize bufferPos, ODSession* session, 
										   ODValueNameSpace *theNameSpace, ODISOStr key)
{
	ODTradITextData* theODName = kODNULL;
	ODUShort		theODNameLen;
	ScriptCode		theScriptCode;
	LangCode		theLangCode;
	ODSize			sizeShort = sizeof(ODUShort);
	ODSize			sizeScriptCode = sizeof(ScriptCode);
	ODSize			sizeLangCode = sizeof(LangCode);
	Environment* 	ev = somGetGlobalEnvironment ();

	ODBlockMove((ODPtr)bufferPos, (ODPtr)&theScriptCode, sizeof(ScriptCode));
	bufferPos += sizeScriptCode;
	
	ODBlockMove((ODPtr)bufferPos, (ODPtr)&theLangCode, sizeof(LangCode));
	bufferPos += sizeLangCode;
	
	ODBlockMove((ODPtr)bufferPos, (ODPtr)&theODNameLen, sizeShort);
	bufferPos += sizeShort;
	
	char* theText = (char*)ODNewPtrClear(theODNameLen+1, kDefaultHeapID);
	ODBlockMove((ODPtr)bufferPos, (ODPtr)theText, theODNameLen);
	//theODName = CreateIText(theScriptCode, theLangCode, theText );

	theODName = CreateFlatIText(theScriptCode, theLangCode, theText, theODNameLen);
	ODDisposePtr( theText );

	bufferPos += theODNameLen;
	
	TRY
		if (theNameSpace->GetType(ev) != kODNSDataTypeODValue)
			THROW(kODErrInvalidNSType);
		// no need to remove a previously registered key

			// Register the new entry with theNameSpace 
		ValueNameSpaceRegister(theNameSpace, ev, key, 
				(ODPtr)theODName,theODNameLen+sizeof(ODTradITextDataHeader));
	CATCH_ALL
	ENDTRY
	
	ODDisposePtr( theODName );

	return (ODSize)bufferPos;
}

//------------------------------------------------------------------------------
// ExtractISOStringList
//------------------------------------------------------------------------------
static ODSize ExtractISOStringList(ODSize bufferPos, ODSession* session, 
										   ODObjectNameSpace *theNameSpace, ODISOStr key)
{
	ODTypeList*		valueList = kODNULL;
	ODUShort		valueCount = 0;
	ODUShort		valueItemLen = 0;
	ODISOStr		valueItem = kODNULL;
	ODObject*		entry = kODNULL;
	ODSize			sizeShort = sizeof(ODUShort);
	Environment* 	ev = somGetGlobalEnvironment ();

	ODBlockMove((ODPtr)bufferPos, (ODPtr)&valueCount, sizeShort);
	bufferPos += sizeShort;
	
	valueList = session->GetStorageSystem(ev)->CreateTypeList(ev,(ODTypeList*)kODNULL);

	for(ODUShort j=1; j<= valueCount; j++)
	{
		ODBlockMove((ODPtr)bufferPos, (ODPtr)&valueItemLen, sizeShort);
		bufferPos += sizeShort;

		valueItem = (ODISOStr)ODNewPtrClear((ODSize)valueItemLen+1, kDefaultHeapID);
		ODBlockMove((ODPtr)bufferPos, (ODPtr)valueItem, valueItemLen);
		bufferPos += valueItemLen;
		
		valueList->AddLast( ev, (ODType)valueItem );
		ODDisposePtr((ODPtr)valueItem);
	}
	
		// Register entry with theNameSpace
	TRY
		if (theNameSpace->GetType(ev) != kODNSDataTypeODObject)
			THROW(kODErrInvalidNSType);
		
		// no need to remove a previously registered key

			// Register the new entry with theNameSpace 
		theNameSpace->Register( ev, key, (ODObject*)valueList );
	CATCH_ALL
		ODDeleteObject( valueList );
	ENDTRY
	
	return (ODSize)bufferPos;
}

//------------------------------------------------------------------------------
// ExtractOSType
//------------------------------------------------------------------------------
static ODSize ExtractOSType(ODSize bufferPos, ODSession* session, 
										   ODValueNameSpace *theNameSpace, ODISOStr key)
{
	ODULong			valueLen = 0;
	ODOSType*		theODType = kODNULL;
	Environment* 	ev = somGetGlobalEnvironment ();

	theODType = (ODOSType*)ODNewPtrClear(sizeof(ODOSType), kDefaultHeapID);
	ODBlockMove((ODPtr)bufferPos, (ODPtr)theODType, sizeof(ODOSType));
	bufferPos += sizeof(ODOSType);
	
		// Register entry with theNameSpace
	TRY
		if (theNameSpace->GetType(ev) != kODNSDataTypeODValue)
			THROW(kODErrInvalidNSType);
		// no need to remove a previously registered key

			// Register the new entry with theNameSpace 
		ValueNameSpaceRegister(theNameSpace,ev,key,(ODPtr)theODType, 
														  sizeof(ODOSType));
	CATCH_ALL
	ENDTRY
	
	ODDisposePtr( (ODPtr)theODType );

	return (ODSize)bufferPos;
}

//------------------------------------------------------------------------------
// ExtractPltfrmTypeSpac
//------------------------------------------------------------------------------
static ODSize ExtractPltfrmTypeSpac(ODSize bufferPos, ODSession* session, 
										   ODISOStr key)
{
	ODObjectNameSpace* 		edKindsNmSpc = kODNULL;
	ODObjectNameSpace* 		kindCategoryNS = kODNULL;
	ODPlatformType			thePlfType = 0;
	ODPlatformTypeSpace		thePlfTypeSpc = 0;
	ODULong					valueLen = 0;
	ODUShort				theCategoryLen = 0;
	ODType					theKind = kODNULL;
	ODISOStr				theCategory = kODNULL;
	ODObject*				entry = kODNULL;
	ODUShort				valueCount = 0;
	ODSize					sizeShort = sizeof(ODUShort);
	ODTypeList*				valueList = kODNULL;
	Environment* 			ev = somGetGlobalEnvironment ();

	ODBlockMove((ODPtr)bufferPos, (ODPtr)&valueCount, sizeShort);
	bufferPos += sizeShort;
	
	for(ODUShort j=1; j<= valueCount; j++)
	{
			// First read thePlfTypeSpc
		ODULong size = sizeof(ODPlatformTypeSpace);
		ODBlockMove((ODPtr)bufferPos, (ODPtr)&thePlfTypeSpc, size);
		bufferPos += size;
		
			// Next read thePlfType
		size = sizeof(ODPlatformType);
		ODBlockMove((ODPtr)bufferPos, (ODPtr)&thePlfType, size);
		bufferPos += size;
		
		ODTranslation* translation = session->GetTranslation(ev);
		theKind = translation->GetISOTypeFromPlatformType(ev, thePlfType, thePlfTypeSpc);
		
			// Register entry with theNameSpace
		edKindsNmSpc = (ODObjectNameSpace*)(session->GetNameSpaceManager(ev)->
											HasNameSpace(ev, kODEditorKinds));
		TRY
			if (edKindsNmSpc->GetType(ev) != kODNSDataTypeODObject)
				THROW(kODErrInvalidNSType);
				
				// if there is already a list registered add to end of it
			if (edKindsNmSpc->GetEntry(ev,key, (ODObject**)&valueList ))
				valueList->AddLast( ev, theKind );
			else
			{	// else make a new list
				valueList = session->GetStorageSystem(ev)->
							CreateTypeList(ev, (ODTypeList*)kODNULL);
				valueList->AddLast( ev, theKind );

					// Register the new entry with edKindsNmSpc 
				edKindsNmSpc->Register( ev, key, (ODObject*)valueList );
			}
		
			ODValueNameSpace *userStringNS = (ODValueNameSpace*)
					(session->GetNameSpaceManager(ev)->HasNameSpace(ev, kODKindUserString));
			bufferPos = ExtractIText(bufferPos, session, userStringNS, theKind);
			
			kindCategoryNS = (ODObjectNameSpace*)(session->GetNameSpaceManager(ev)->
												HasNameSpace(ev, kODKind));
		
			ODBlockMove((ODPtr)bufferPos, (ODPtr)&theCategoryLen, sizeShort);
			bufferPos += sizeShort;
		
			theCategory = (ODISOStr)ODNewPtrClear((ODSize)theCategoryLen+1, kDefaultHeapID);
			ODBlockMove((ODPtr)bufferPos, (ODPtr)theCategory, theCategoryLen);
			bufferPos += theCategoryLen;
		
			TRY
				if (kindCategoryNS->GetType(ev) != kODNSDataTypeODObject)
					THROW(kODErrInvalidNSType);
				// if there is already a list registered add to end of it
				if (kindCategoryNS->GetEntry(ev,theKind, (ODObject**)&valueList ))
					valueList->AddLast( ev, theCategory );
				else
				{	// else make a new list
					valueList = session->GetStorageSystem(ev)->
										 CreateTypeList(ev, (ODTypeList*)kODNULL);
					valueList->AddLast( ev, theCategory );
						// Register the new entry with kindCategoryNS 
					kindCategoryNS->Register( ev, theKind, (ODObject*)valueList );
				}
			CATCH_ALL
			ENDTRY

			ODDisposePtr( theCategory );
			
		CATCH_ALL
		ENDTRY

		ODDisposePtr( theKind );
	}	
	
	return (ODSize)bufferPos;
}

//------------------------------------------------------------------------------
// ExtractISOString
//------------------------------------------------------------------------------
static ODSize ExtractISOString(ODSize bufferPos, ODSession* session, 
										   ODValueNameSpace *theNameSpace, ODISOStr key)
{
	ODISOStr		value = kODNULL;
	ODULong		valueLen = 0;
	ODUShort	valueItemLen = 0;
	ODISOStr	valueItem = kODNULL;
	ODSize				sizeShort = sizeof(ODUShort);
	Environment* 		ev = somGetGlobalEnvironment ();

	ODBlockMove((ODPtr)bufferPos, (ODPtr)&valueItemLen, sizeShort);
	bufferPos += sizeShort;

	valueItem = (ODISOStr)ODNewPtrClear((ODSize)valueItemLen+1, kDefaultHeapID);
	ODBlockMove((ODPtr)bufferPos, (ODPtr)valueItem, valueItemLen);
	bufferPos += valueItemLen;

		// Register entry with theNameSpace
	TRY
		if (theNameSpace->GetType(ev) != kODNSDataTypeODValue)
			THROW(kODErrInvalidNSType);

		ODNameSpaceManager *theNmSpcMgr = theNameSpace->GetNameSpaceManager(ev);
		// no need to remove a previously registered key

			// Register the new entry with theNameSpace 
		ValueNameSpaceRegister( theNameSpace, ev, key, (ODPtr)valueItem, 
														   valueItemLen+1 );
	CATCH_ALL
	ENDTRY
	
	ODDisposePtr( (ODPtr)valueItem );
	
	return (ODSize)bufferPos;
}

//------------------------------------------------------------------------------
// ExtractHelpFileName
//------------------------------------------------------------------------------
static ODSize ExtractHelpFileName(	ODSize bufferPos, 
									ODSession* session, 
									ODFileSpec* libspec,
									ODValueNameSpace *theNameSpace, 
									ODISOStr editorID)
{
	ODSize			sizeShort = sizeof(ODUShort);
	Environment* 	ev = somGetGlobalEnvironment ();
	ODUShort		fileNameLen = 0;
	ODFileSpec	 	fsspec;
	ODISOStr		value = kODNULL;
	ODULong		valueLen = 0;

		// Register entry with theNameSpace
	TRY

		fsspec.vRefNum = libspec->vRefNum;
		fsspec.parID = libspec->parID;
	
		ODBlockMove((ODPtr)bufferPos, (ODPtr)&fileNameLen, sizeShort);
		bufferPos += sizeShort;
		fsspec.name[0] = fileNameLen;
		
		ODBlockMove((ODPtr)bufferPos, (ODPtr)&fsspec.name[1], fileNameLen);
		bufferPos += fileNameLen;
	
		if (theNameSpace->GetType(ev) != kODNSDataTypeODValue)
			THROW(kODErrInvalidNSType);
	
			// Register the new entry with theNameSpace 
		ValueNameSpaceRegister( theNameSpace, ev, editorID, 
												(ODPtr)&fsspec, sizeof(fsspec));
	CATCH_ALL
	ENDTRY
		
	return (ODSize)bufferPos;
}

//------------------------------------------------------------------------------
// Cache_nmap
//------------------------------------------------------------------------------
static void
Cache_nmap( PlatformFile* cacheFile, ODSize bufferSize, Handle mappingPair, FSSpec* fsspec )
{
	ODSLong	sizeofsize = sizeof( ODSize );
	ODSLong	sizeoflong = sizeof( long );
	ODSize	sizeofname = (ODSize)fsspec->name[0];

	if (cacheFile)
	{
		cacheFile->Write( (ODSByte*) &fsspec->parID, &sizeoflong );
		cacheFile->Write( (ODSByte*) &sizeofname, &sizeofsize );
		cacheFile->Write( (ODSByte*) &fsspec->name[1], (ODSLong*)&sizeofname );
		cacheFile->Write( (ODSByte*) &bufferSize, &sizeofsize );
		cacheFile->Write( (ODSByte*) *mappingPair, (ODSLong*)&bufferSize );
	}
}

//------------------------------------------------------------------------------
// MoveToTrash
//------------------------------------------------------------------------------
static void
MoveToTrash( short vRefnum, long dirID, Str255 name )
{
	short foundVRefNum;
	long  foundDirID;
	
	if(!FindFolder(vRefnum, kTrashFolderType, kDontCreateFolder, 
								&foundVRefNum, &foundDirID))
	{

		CMovePBRec paramBlock ;
		memset( &paramBlock, 0, sizeof(paramBlock) );
		
		paramBlock.ioNamePtr = name ;
		paramBlock.ioVRefNum = foundVRefNum ;
		paramBlock.ioNewDirID = foundDirID ;
		paramBlock.ioDirID = dirID ;
		
		(void) HDelete(foundVRefNum,foundDirID,name); 	// ignore error (may not exist)
		OSErr result = PBCatMoveSync( &paramBlock );
		if (result)
			(void) HDelete(vRefnum,dirID,name); 
	}
	else
		(void) HDelete(vRefnum,dirID,name); 
}


//------------------------------------------------------------------------------
// Read_aete_Resources
//------------------------------------------------------------------------------
static void
Read_aete_Resources( FSSpec* fsspec, AEDescList* aetes )
{
	LOG("* Reading 'aete's from file '%P' in (%hd,%ld)\n",fsspec->name,
			fsspec->vRefNum,fsspec->parID);
	TRY
		ODSShort aeteCount = Count1Resources( typeAETE );
		for (short i=1; i<=aeteCount; i++) 
		{
				// Get the Resource
			Handle aete = Get1IndResource(typeAETE, i);
			ODError result = ResError();
			
			if ( aete != nil && !result )
			{
				ODLockHandle((ODHandle) aete);
				
				ODSize bufferSize = ODGetHandleSize((ODHandle)aete);
				THROW_IF_ERROR( AEPutPtr(aetes, 0, typeAETE, *aete, bufferSize) );
				
				ReleaseResource( aete );
			}
		}
	CATCH_ALL
		WARN("Error %ld reading aetes for file %#s",ErrorCode(),fsspec->name);
	ENDTRY
}

struct partialAETE {
	short	version;
	short	language;
	short	script;
};

//------------------------------------------------------------------------------
// AddAETEs
//------------------------------------------------------------------------------
static void
AddAETEs(PlatformFile* cacheFile, ODSLong languageCode, AEDescList* aeteList)
{
	ODPtr		ptr;
	ODULong		size;
	ODSLong		i, n, lang;
	AEDesc		temp;
	AEDesc		list;
	DescType	type;
	
	ptr = cacheFile->ReadResourcePtr(typeAECache, 0, &size);
	if (ptr != kODNULL)
	{
		PtrToHand(ptr, &temp.dataHandle, size);
		ODDisposePtr(ptr);
		temp.descriptorType = typeAERecord;
		THROW_IF_ERROR( AEGetKeyDesc(&temp, keyDirectObject, typeAEList, &list) );
		ODDisposeHandle(temp.dataHandle);
		temp.dataHandle = kODNULL;
		THROW_IF_ERROR( AECountItems(&list, &n) );
		for (i = 1; i <= n; i++)
		{
			THROW_IF_ERROR( AEGetNthDesc(&list, i, typeAETE, &type, &temp) );
			lang = ((**(partialAETE**)temp.dataHandle).language << 8) + (**(partialAETE**)temp.dataHandle).script;
			if (lang == languageCode)
				THROW_IF_ERROR( AEPutDesc(aeteList, 0, &temp) );
			THROW_IF_ERROR( AEDisposeDesc(&temp) );
		}
		THROW_IF_ERROR( AEDisposeDesc(&list) );
	}
}


//------------------------------------------------------------------------------
// GetODPrefsFolder
//------------------------------------------------------------------------------

static ODBoolean GetODPrefsFolder(ODSShort* prefVol, ODSLong* prefDir )
{
	Str255			prefFolderName;
	ODULong 		prefModDate;
	OSErr			result;
	ODSLong			tempDir;

		//find preferences folder
	ODGetString(prefFolderName,kODPrefsFolderNameStrID);

	TRY{
		FindMagicFolder(kPreferencesFolderType,prefFolderName,
							prefVol, prefDir, &prefModDate);
	}CATCH_ALL{
		result =  FindFolder(kOnSystemDisk, kPreferencesFolderType, 
						kDontCreateFolder, prefVol, &tempDir);
		result = DirCreate(*prefVol, tempDir, prefFolderName, prefDir);
		if (result != noErr)
			return kODFalse;
	}ENDTRY
	
	return kODTrue;
}

//------------------------------------------------------------------------------
// ConstructCacheFileName
//------------------------------------------------------------------------------

static OSErr ConstructCacheFileName(ODSShort fldrVRefNum, ODSLong fldrDirID, 
										ODULong fldrModDate, Str255* fileName )
{
	OSErr theError = noErr;
	ParamBlockRec pb;
	
	pb.volumeParam.ioNamePtr = (StringPtr)fileName;
	pb.volumeParam.ioVRefNum = fldrVRefNum;
	pb.volumeParam.ioVolIndex = 0; 			/* Use vRefNum */
	theError = PBGetVInfoSync(&pb);
	if (theError == noErr)
	{
		Str255		dateString;
		IUDateString(fldrModDate, shortDate, dateString);

		ConcatPascalStrings( *fileName, "\p " );
		ConcatPascalStrings( *fileName, (ConstStr255Param)dateString );
	}
	return theError;
}

//------------------------------------------------------------------------------
// DeleteOldCacheFiles
//------------------------------------------------------------------------------

static void DeleteOldCacheFiles()
{
	ODSShort		prefVol;
	ODSLong			prefDir;

	if (GetODPrefsFolder( &prefVol, &prefDir ))
	{
		CInfoPBRec	pb;
		Str255 fileName;
		Str255 prefFileName;
		ODGetString(prefFileName,kODPrefsFileNameStrID);

			// Loop through all the files in folder
		memset (&pb,0,sizeof(pb));
		for( pb.hFileInfo.ioFDirIndex = 1; kODTrue; pb.hFileInfo.ioFDirIndex++ )
		{
				//need to do this each time since PBGetCatInfo call returns val here
			pb.hFileInfo.ioNamePtr = (StringPtr)&fileName;
			pb.hFileInfo.ioVRefNum = prefVol;
			pb.hFileInfo.ioDirID = prefDir;  
			
			if( PBGetCatInfoSync( &pb ) != noErr )
				break;
			
			if ( !(pb.dirInfo.ioFlAttrib & (1<<4)) && 
				!(pb.hFileInfo.ioFlFndrInfo.fdFlags & kIsAlias) &&
				(pb.hFileInfo.ioFlFndrInfo.fdCreator == kODShellSignature) )
			{
				if (!EqualPascalStrings(fileName, prefFileName))
				{
					ODULong		secs;
					GetDateTime(&secs);
						// if this cache file is over 30 days old, delete it
					Str255		dateString;
					ODULong aMonthAgo = secs - (30*24*60*60);
					IUDateString(aMonthAgo, shortDate, dateString);
					if (pb.hFileInfo.ioFlMdDat < aMonthAgo )
					{
						MoveToTrash( pb.hFileInfo.ioVRefNum, 
								     pb.hFileInfo.ioFlParID, fileName );
					}
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
// LoadableLibrary
//------------------------------------------------------------------------------

static ODBoolean LoadableLibrary(ODSShort resRefNum)
{
	ODBoolean	libIsLoadable = kODFalse;
	

	ODHandle cfrg = (ODHandle)Get1Resource(kCFragResourceType, kCFragResourceID);
	ODError result = ResError();

	if (cfrg) {
		DetachResource(cfrg);
		result = ResError();
	} else
		if( result==noErr ) result=resNotFound;

	if (result == noErr)
	{
		ODLockHandle( cfrg);
		
		ODSize bufferPos = (ODSize)*cfrg;
		cfragHeader theHeader;
		cfragDescriptor theDesc;
		
		ODBlockMove((ODPtr)bufferPos, (ODPtr)&theHeader, sizeof(cfragHeader));
		bufferPos += sizeof(cfragHeader);
		
		for(ODUShort i=0; i< theHeader.numFragDescs; i++)
		{
			ODBlockMove((ODPtr)bufferPos, (ODPtr)&theDesc.header, sizeof(cfragDescHeader));
			bufferPos += sizeof(cfragDescHeader);
			
			ODULong nameLen = theDesc.header.infoRecLen - sizeof(cfragDescHeader);
			ODBlockMove((ODPtr)bufferPos, (ODPtr)&theDesc.fragName, nameLen);
			bufferPos += nameLen;
			
			if (theDesc.header.arch == kCurrentCFragArch)
			{
				libIsLoadable = kODTrue;
				break;
			}
		}

		ODDisposeHandle( cfrg );
	}
	
	return libIsLoadable;
}


//eof
