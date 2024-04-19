//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   PlatformFile
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
// @(#) 1.25 com/src/utils/include/PlfmFile.h, odutils, od96os2, odos29646d 9/30/96 11:09:56 [ 11/15/96 15:29:01 ]
/*
	File:		PlatformFile.h

	Contains:	Template for PlatformFile header

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _PLFMFILE_
#define _PLFMFILE_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
#ifndef _ODTYPESP_
#include <ODTypesP.h>
#endif
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ITEXT_
#include "IText.h"		/* For ODScriptCode type */
#endif

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
#ifndef __FILES__
#include <Files.h>
#endif
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

#if defined(_PLATFORM_WIN32_)
#include <mapiwin.h>
#include <wtypes.h>
#include <winbase.h>
#endif // defined(_PLATFORM_WIN32_)

#if defined(_PLATFORM_OS2_)
#include <ODos2.h>
#endif // defined(_PLATFORM_OS2_)

#if defined(_PLATFORM_UNIX_)
#include <fcntl.h>
#include <sys/mode.h>
#include <sys/param.h>
#endif // defined(_PLATFORM_UNIX_)

//==============================================================================
// Theory of Operation
//==============================================================================

//==============================================================================
// Constants
//==============================================================================

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
const	ODSShort	kODNoFileRefNum 		= 0;
const	ODSShort	kODMaxFileNameSize		= 31;
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#if defined(_PLATFORM_WIN32_)
typedef HANDLE    ODFileRefNum;
const	ODFileRefNum kODNoFileRefNum = INVALID_HANDLE_VALUE;
const	unsigned long	kODMaxFileNameSize = MAX_PATH;
#endif // defined(_PLATFORM_WIN32_)

#if defined(_PLATFORM_OS2_)
typedef HFILE     ODFileRefNum;
const	ODFileRefNum kODNoFileRefNum = NULLHANDLE;
const	unsigned long	kODMaxFileNameSize = CCHMAXPATH;
#endif // defined(_PLATFORM_OS2_)

#if defined(_PLATFORM_UNIX_)
typedef int     ODFileRefNum;
const	ODFileRefNum kODNoFileRefNum = -1;
const	unsigned long	kODMaxFileNameSize = MAXPATHLEN;
#endif // defined(_PLATFORM_UNIX_)

#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

//==============================================================================
// Scalar Types
//==============================================================================

typedef FSSpec		ODFileSpec;
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
// Note: Keep in sync with ODTypesP.h
#define kODFileRead  0x0001
#define kODFileWrite 0x0002
#define kODFileShareRead 0x0004
#define kODFileShareWrite 0x0008

typedef unsigned long		ODFilePermission;

enum ODFilePosOptions {
	kODSeekFromBeginning = fsFromStart,
	kODSeekFromCurrent = fsFromMark,
	kODSeekFromEnd = fsFromLEOF };

typedef unsigned long ODFileAttrFlags;
#define kODFileAttrReadOnly  0x0001

#else // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
typedef short		ODFileRefNum;
typedef ODSByte		ODFilePermission;

typedef ODFileRefNum	*ODFileRefNumPtr;
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
enum PFUniquifyAction { kSpecifyNewNameOnly, kRenameInPlace };
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
// No one uses kRenameInPlace on non-Mac Platforms.  Corresponding code removed.
enum PFUniquifyAction { kSpecifyNewNameOnly };
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

const	ODBoolean	kODTryCurrentName	= kODFalse;
const	ODBoolean	kODForceNewName 	= kODTrue;

const	ODSShort	kODNoResourceID = 0;

//==============================================================================
// Classes defined in this interface
//==============================================================================

class PlatformFile;
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
class CWithActiveResources;		// private class
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)


//==============================================================================
// PlatformFile
//==============================================================================

class PlatformFile
{	
public:

	PlatformFile();
   ~PlatformFile();

// Methods you can call when the file is not open:
	// pointing the PlatformFile at a physical file	and setting default opening permissions
	// Specify() can only be called once on the object. If it is being called more than once,
	// the behavior is unpredictable. If you want to work with a different file,
	// you need to delete the current object and create a new one. 
	ODNVMethod	void		Specify(const ODFileSpec* fileSpec);
	ODNVMethod	void		SpecifyFromFile(PlatformFile* file);
	ODNVMethod	void		SetAsciiName(const char* name);
	inline		void		SetPermission(ODFilePermission filePermission);
	// create/open/delete the file
	inline		void		SetFileRefNum( ODFileRefNum );
	ODNVMethod	void		Create(ODOSType creator, ODOSType fileType, ODScriptCode scriptCode);
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
	inline		ODFilePermission GetPermission();
	ODNVMethod	void		UniquifyAndCreate(ODOSType creator, ODOSType fileType, ODScriptCode scriptCode,
	                                       short copyCount, ODBoolean forceNewName);
	ODNVMethod	void		GetFileAttributes(ODFileAttrFlags *fAttr);
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
	ODNVMethod	void		Open();
	ODNVMethod	void		Delete();

	// These are optional and simply optimize multiple resource calls
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	void		OpenResFile();
	ODNVMethod	void		CloseResFile();
	// create the resource fork (only to be used after Create).
	ODNVMethod	void		CreateResFile();
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

// Methods you can call anytime after calling Specify or SetAsciiName
	ODNVMethod	ODBoolean	IsEqualTo(PlatformFile* file);
	ODNVMethod	ODBoolean	IsLocked();
	ODNVMethod	void		Lock();
	ODNVMethod	void		Unlock();
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	ODBoolean	IsStationery();
	ODNVMethod	void		SetStationery(ODBoolean isStationery);
	ODNVMethod	void		SetCustomIcon(ODBoolean hasCustomIcon);
	ODNVMethod	ODBoolean	HasCustomIcon();
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	ODBoolean	IsDirectory();
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	void		SetCustomIconFamily(ODIconFamily icons);
	ODNVMethod	ODIconFamily	GetCustomIconFamily();
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	ODBoolean	Exists();
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	ODUShort	GetFInfoFlags();
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	// Get & set the Desktop DB comment field
	ODNVMethod	ODIText*	GetComments(ODIText* comments = kODNULL);
	ODNVMethod	void		SetComments(ODIText* comments);
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)


	ODNVMethod	void		FlushVolume();

	ODNVMethod	ODName*		GetName();
	ODNVMethod	void		GetAsciiName(char* name,ODULong maxLength);
	inline		ODULong		GetAsciiNameLength();
	ODNVMethod	ODOSType	GetPlatformCreator();					// Macintosh specific
	ODNVMethod	ODOSType	GetPlatformType();					// Macintosh specific
	ODNVMethod	void		SetPlatformType(ODOSType fileType);	// Macintosh specific
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	ODContainerType		GetContainerType();
	ODNVMethod	void		SetContainerType(ODContainerType containerType);
	ODNVMethod	ODPtr		ReadResourcePtr(ODPlatformType resType, ODSShort resID, ODULong* size); // Mac
	ODNVMethod	void		WriteResourcePtr(ODPlatformType resType, ODSShort resID, 
											const void *resPtr, ODULong size); // Mac
	ODNVMethod	void		DeleteResource(ODPlatformType resType, ODSShort resID); // Mac
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	inline		ODFileSpec&	GetFileSpec();
	ODNVMethod	void		MoveRename( ODFileSpec* newSpec, ODBoolean isDuplicate ) ;
	ODNVMethod	void		Move( ODSLong targetParID ) ;
	ODNVMethod	void		UniquifyName( ODSShort uniquifyingStringID,
											StringHandle uniquifyingString,
											ODSShort uniquifyingNumberID,
											StringHandle uniquifyingNumberString,
											short copyCount,
											PFUniquifyAction action,
											ODBoolean forceNewName);
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	void		MoveToTrash() ;
	ODNVMethod	void		Rename( Str63 name ) ;
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
	ODNVMethod	void		Rename( ODFilePathname name ) ;
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	ODTime		GetFileModDate();
	ODNVMethod	void		SetFileModDate(ODTime date);
	ODNVMethod	ODTime		GetFileCreationDate();
	ODNVMethod	void		SetFileCreationDate(ODTime date);
	ODNVMethod	ODULong 	GetAllocationBlockSize();
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODNVMethod	void		BumpFolderModDate();
	
	ODNVMethod	ODBoolean	IsInTrash( );
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

// Methods you must call while the file is open	
	ODNVMethod	void		Close();

	ODNVMethod	void		SetFilePos(ODSShort posMode, ODSLong posOff);
	ODNVMethod	ODSLong		GetFilePos();

	ODNVMethod	void		Read(void* buffer, ODSLong* count);
	ODNVMethod	void		Write(const void* buffer, ODSLong* count);

	ODNVMethod	ODSLong		GetEndOfFile();
	ODNVMethod	void		SetEndOfFile(ODSLong length);

	inline		ODFileRefNum	GetFileRefNum();

	ODNVMethod	void		CopyFrom( PlatformFile* src );
		

private:
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	/* Constants to specify the file fork GetLocalPaths should match */
	enum
	{
		kODDataFork		= 0,
		kODResourceFork	= 1
	};
	typedef short ODForkType;
	
	ODNVMethod	void		GetFileID( );
	ODNVMethod	void		UpdateSpecFromID( );
	ODNVMethod	ODBoolean	ActivateResourceFile( ODBoolean &needToCloseResFile );
	
	ODNVMethod	ODFileRefNum GetFirstLocalPath( ODForkType fork );


public:
	ODNVMethod	ODError		GetLocalPaths(
							  ODForkType fork,
							  ODULong *refNumCount,
							  ODFileRefNumPtr *refNums);

/*	Determine if local access paths to a file fork are open.
	The GetLocalPaths function looks through the FCB list to see if
	the fork of the file specified by spec and fork is open by the 
	local File Manager (you should use PBGetCatInfo and check
	ioFlAttrib if you need to see if *anyone* has the file fork open
	*anywhere*). If the result is noErr, GetLocalPaths returns a
	count and a pointer to an array of file refNums (if count is
	not zero) of local access paths to the specified file fork.

	spec		input:	An FSSpec record specifying the file.
	fork		input:	Specifies the file fork - either kDataFork
						or kResourceFork.
	refNumCount	output:	The number of local access paths found.
	refNums		output: If not NULL, a pointer to an array of
						file refNums - the number of which is
						specified by refNumCount.  The caller is
						responsible for disposing of this pointer.
*/
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
	ODNVMethod	void	ConcatenateSuffix(ODFilePathname filePathname, char *suffixString);
#if defined(_PLATFORM_WIN32_)
	ODNVMethod	DWORD	ConvertFromFileTime(LPFILETIME fileTime, ODTime *odTime);
	ODNVMethod	DWORD ConvertToFileTime(ODTime odTime, LPFILETIME fileTime);
#endif // defined(_PLATFORM_WIN32_)
#if defined(_PLATFORM_OS2_)
	ODNVMethod	unsigned long	ConvertFromFileTime(FDATE *fileDate, FTIME *fileTime,
	                                               ODTime *odTime);
	ODNVMethod	unsigned long	ConvertToFileTime(ODTime odTime,
	                                             FDATE *fileDate, FTIME *fileTime);
#endif // defined(_PLATFORM_OS2_)
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

private:

	ODFileSpec			fFileSpec;
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODSShort			fDataRefNum;
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
	ODFileRefNum	fDataRefNum;
	ODFilePermission	fOpenPermission;  // Permissions actually used for file open.
	                                    // fPermission could theoretically be changed
	                                    // by user when file is already open.
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODFilePermission	fPermission;
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODContainerType		fContainerType;
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODOSType			fCreator;
	ODOSType			fFileType;
	ODScriptCode		fScriptTag;
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODULong				fFileID;
	ODULong				fVolModDate;
	ODULong				fTimeLastChecked;
	ODFileRefNum		fResRefNum;
	ODULong				fResOpenStack;
	ODBoolean			fNeedToClose;
	
	friend class CWithActiveResources;
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)

};


//------------------------------------------------------------------------------
// Inline method bodies
//------------------------------------------------------------------------------


inline ODULong PlatformFile::GetAsciiNameLength()
{
	return fFileSpec.name[0];
}

void PlatformFile::SetPermission(ODFilePermission permission)
{
	fPermission = permission;
}

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

ODFilePermission PlatformFile::GetPermission()
{
	return fPermission;
}
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)

inline ODFileSpec& PlatformFile::GetFileSpec()
{
	return fFileSpec;
}

inline ODFileRefNum PlatformFile::GetFileRefNum()
{
	return fDataRefNum;
}

inline void PlatformFile::SetFileRefNum( ODFileRefNum refNum )
{
	fDataRefNum = refNum;
}



#endif // _PLFMFILE_
