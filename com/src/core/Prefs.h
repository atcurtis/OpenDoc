/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odcore
 *
 *   CLASSES:   Preferences
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
/* @(#)Z 1.4 com/src/core/Prefs.h, odcore, od96os2, odos29646d 96/11/15 15:24:54 (96/10/30 20:21:51) */
/*
	File:		Prefs.h

	Contains:	Class definition for Preferences

	Owned by:	Caia Grisar 

	Copyright:	© 1993 - 95 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<12>	10/20/95	CG		#1294494 GM:Info writing size info to the
									wrong prefs file.
		<11>	 8/30/95	CG		#1255135 FB2:"Updating Part Editor
									database" dlg comes up indefinitely
									#1257569 FB3:Editor nmaps on read-only
									volumes are not being cached.
		<10>	 8/25/95	JP		1272288:Added AETE routines & cacheFile
									optimizations
		 <9>	 6/22/95	jpa		Added fFoundBadAlias [1257556]
		 <8>	 6/19/95	jpa		Added fScanningDialog and show/close
									methods for it [1258142]
		 <7>	  6/9/95	CG		#1203571 implemented  nmap caching
									mechanism.
		 <6>	 5/25/95	jpa		Store list of editor dirs [1240150]
		 <5>	 5/18/95	CG		#1241621	BB: Need to implement Purge for
									Binding subsystem objects.
		 <4>	 5/17/95	TJ		Backed out changes from previous checkin.
		 <2>	 4/24/95	eeh		1231707: fsspec param added to
									Read_nmap_Buffer and Read_nmap_Resources
		 <1>	 4/13/95	CG		first checked in
		<14>	 2/14/95	jpa		Changes to support reading Editors folders
									on all disks. [1205141]
		<13>	  2/2/95	CG		Changed dirIDs to ODSLong and vRefNums to
									ODSShort.
		<12>	 1/10/95	CG		Removed dead code.
		<11>	12/13/94	CG		#1205141:Prefs code now looks in "Editors"
									and "OpenDoc Libraries" folders for nmaps.
		<10>	11/30/94	RR		#1188078 Use FSSpec instead of ODFileSpec
		 <9>	 9/12/94	CG		#1157104: GetPartEditorNMAPs now looks in
									same search path as CFM does for Part
									Editor library files.  Temporary fix
									removed.
		 <8>	 8/25/94	CG		#1175107: Temporary fix to allow documents
									to be opened from anywhere.
		 <7>	 8/24/94	CG		#1181622: Updated ownership info.
		 <6>	 7/29/94	CG		Changed some methods to return booleans
									instead of throwing errors.
		 <5>	 7/26/94	jpa		Don't inherit from ODObject!
		 <4>	  7/1/94	CG		Renamed LocateASLMLibFiles to
									LocatePartLibFiles.
		 <3>	  7/1/94	CG		Change to LoadNMAPResource param list.
		 <2>	 6/27/94	CG		Moved IsEditorLoaded to BindngH, removed
									fEv.
		 <1>	 6/14/94	CG		first checked in
		 <1>	 5/27/94	CG		first checked in
	To Do:
	In Progress:
		
*/

#ifndef _PREFS_
#define _PREFS_

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODOBJ_
#include "ODObject.xh"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __DIALOGS__
#include <Dialogs.h>
#endif
#endif

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class Preferences;
class PrefsDirInfo;
class PrefsDirInfoList;

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class ODSession;
class PlatformFile;
class ODStorageUnitView;
class LinkedList;
class PrefsDirInfo;

//==============================================================================
// Local Function Prototypes (from NamSpacM.cpp)
//==============================================================================

//void ReadBytes(ODFileRefNum file, ODStorageUnitView* view,ODPtr bytes, ODULong* length);
//ODULong ReadSize(ODFileRefNum file, ODStorageUnitView* view);
enum {
	kNestedFolder = 0,
	kLibsFolder,
	kSysEditorsFolder,
	kRootEditorsFolder
};
typedef ODULong PrefsDirType;


//=====================================================================================
// Class Preferences
//=====================================================================================

class Preferences
{
public:
	
	Preferences();
	~Preferences();
	ODNVMethod void			InitPreferences(ODSession* session);
		 		 
	ODNVMethod ODSize		Purge(ODSize size);

	ODNVMethod ODBoolean	HaveEditorsFoldersChanged( );
	ODNVMethod ODBoolean	ScanLibsAndEditors();
	ODNVMethod void			UpdatePreferences();
	ODNVMethod void			GetAETEs( ODSLong languageCode, AEDescList* aeteList);
	ODNVMethod PlatformFile* GetPrefsFile();
	
private:
	ODNVMethod void			CreatePrefsNameSpaces();
	ODNVMethod ODSize		ClearEditorNameSpaces();
	ODNVMethod void			GetOpenDocPrefs();
	ODNVMethod void			GetOpenDoc_nmaps();
	ODNVMethod void			ScanDirectory( ODSShort fldrVRefNum, ODSLong fldrDirID,
											PlatformFile* cacheFile ); 
	ODNVMethod void			ScanDirectoryTree(ODSShort fldrVRefNum, ODSLong fldrDirID,
											  		ODULong fldrModDate, PrefsDirType);
	ODNVMethod void 	    VerifyCache(ODSShort fldrVRefNum, 
										  ODSLong fldrDirID, 
										  ODULong fldrModDate,
										  PrefsDirType type,
										  PlatformFile** cacheFile,
										  LinkedList** dirTreeList, 
										  ODBoolean* cacheIsValid);
	ODNVMethod ODBoolean 	LoadCachedDirList(PlatformFile* cacheFile,
										  ODSShort fldrVRefNum,
										  LinkedList** dirTreeList,
										  PrefsDirType type);
	ODNVMethod ODBoolean	GetCacheFile(PlatformFile** cacheFile, ODSShort fldrVRefNum, 
								 		 ODSLong fldrDirID, ODULong fldrModDate );
	ODNVMethod ODBoolean	LoadCachedRezData(PlatformFile* cacheFile, ODSShort vRefNum);
	ODNVMethod void 		TraverseDirectory( ODSShort fldrVRefNum, 
										  ODSLong fldrDirID, 
										  ODSShort depth, 
										  LinkedList* dirList );
	ODNVMethod PrefsDirInfo* FindDirInfo( LinkedList* dirList, short vol, long dir );
	ODNVMethod PrefsDirInfo* AddDirInfo( LinkedList* dirList, 
										short vol, long dir, ODULong modDate,
										ODSShort depth, PrefsDirType = kNestedFolder );
	ODNVMethod	void		AddDirList(LinkedList* dirList);
	ODNVMethod ODBoolean	Read_nmap_Resources(FSSpec* fsspec, PlatformFile* cacheFile);
	ODNVMethod void			Read_nmap_Buffer( ODPtr buffer, ODSize bufferSize,
																FSSpec* fsspec );
	ODNVMethod void			Write_nmap_Buffer( ODPtr buffer, ODSize bufferSize );
	ODNVMethod ODBoolean	LocatePrefsFile();
	ODNVMethod ODBoolean	LoadPreferences();
	ODNVMethod ODBoolean	VerifyPrefsFileVersion();
	ODNVMethod	void		UpdatePrefsFileVersion();
	
	ODNVMethod void			ShowScanningDialog( );
	ODNVMethod void			CloseScanningDialog( );

// Fields:
	ODSession* 			fSession;
	ODNameSpaceManager *fNameSpaceMgr;
	Str31				fLibsFolderName;
	Str31				fEditorsFolderName;
	PlatformFile*		fPrefsFile;
	ODULong				fPrefsModDate;
	LinkedList*			fDirList;
	ODULong				fLastSynchedAt;
	ODBoolean			fScanning;
	ODBoolean			fFoundBadAlias;
	DialogPtr			fScanningDialog;
	
	friend class PrefsDirInfo;
};

#endif // _PREFS_
