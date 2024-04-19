/* @(#)Z 1.16 os2/src/utils/PlfmFile.cpp, odutils, od96os2, odos29646d 96/11/15 15:49:44 (96/10/18 18:09:57) */
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

/*
	File:		PlfmFile.cpp

	Contains:	Implmentation for PlatformFile class

	Owned by:	Vincent Lo

	Copyright:	© 1993-95 by Apple Computer, Inc., all rights reserved.

*/

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/utime.h>

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMFILE_
#include "PlfmFile.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _ITEXT_
#include "IText.h"
#endif

#ifndef __ODTYPESP__
#include <ODTypesP.h>
#endif

#ifndef __ODPAGTUN__
#include <ODPagTun.h>
#endif

#ifndef __STRING__
#include <string.h>
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef _UTILERRS_
#include <UtilErrs.h>
#endif

#include <locale.h>

#ifndef _H_ODMESSAG_MSG
#include <ODMessag.h>
#endif

//==============================================================================
// Constants
//==============================================================================

#define INFO_LEVEL_1  1

//==============================================================================
// Scalar Types
//==============================================================================

//==============================================================================
// Local Classes
//==============================================================================

//==============================================================================
// Global Variables
//==============================================================================

//==============================================================================
// Function Prototype
//==============================================================================

//==============================================================================
// PlatformFile
//==============================================================================


//------------------------------------------------------------------------------
// PlatformFile::IsEqualTo
//------------------------------------------------------------------------------
ODBoolean PlatformFile::IsEqualTo(PlatformFile* file)
{
	APIRET error;
	char *fullPathThis, *fullPathThat;
	int notFoundCount = 0;
	ODBoolean match, noMem = kODFalse;

	if (!Exists())
		notFoundCount = 1;

	if (!file->Exists())
		notFoundCount++;

	if (notFoundCount == 2)
		THROW(ERROR_FILE_NOT_FOUND);

	if (notFoundCount == 1)
		return(kODFalse);

	fullPathThis = (char *)malloc(kODMaxFileNameSize + 1);
	fullPathThat = (char *)malloc(kODMaxFileNameSize + 1);
	if (fullPathThis != NULL || fullPathThat != NULL)
	{
		error = DosQueryPathInfo((char *)fFileSpec.name, FIL_QUERYFULLNAME,
		                         fullPathThis, kODMaxFileNameSize + 1);
		if (!error)
			error = DosQueryPathInfo((char *)file->fFileSpec.name, FIL_QUERYFULLNAME,
			                         fullPathThat, kODMaxFileNameSize + 1);
		if (!error)
		{
			strlwr(fullPathThis);
			strlwr(fullPathThat);
			match = !strcmp(fullPathThis, fullPathThat);
		}
	}
	else
		noMem = kODTrue;

	if (fullPathThis)
		free(fullPathThis);
	if (fullPathThat)
		free(fullPathThat);

	if (noMem)
		THROW(kODErrOutOfMemory);

	if (error)
		THROW(error);

	return(match);
}

//------------------------------------------------------------------------------
// PlatformFile::IsDirectory
//------------------------------------------------------------------------------
ODBoolean PlatformFile::IsDirectory()
{	
	APIRET error;
	FILESTATUS3 fileInfo;

	error = DosQueryPathInfo((char *)fFileSpec.name, FIL_STANDARD,
	                         &fileInfo, sizeof(fileInfo));
	if (error)
		THROW(error);

	return (fileInfo.attrFile & FILE_DIRECTORY);
}

//------------------------------------------------------------------------------
// PlatformFile::Create
//------------------------------------------------------------------------------
void PlatformFile::Create(ODOSType creator, ODOSType fileType, ODScriptCode scriptCode)
{	
   const char kODFileEaTypeName[] = ".TYPE";
	const char kODFileEaTypeValue[] = "OpenDoc Document";

	ULONG access = 0;
	ULONG openAction;
	APIRET error;
	EAOP2 ea;
	FEA2LIST *feaList;
	FEA2 fea;
	char *feaListBuffer, *eaValuePtr, *tPtr;
	int closeCat = 0;
	USHORT eaDataType = EAT_ASCII;  // Type of data used to represent EA value
	USHORT eaDataLength;            // Length of EA value

	if (ODcat_handle == NULL)
	{
		ODInitExceptions();
		closeCat = 1;
	}

	if (ODcat_handle != CATD_ERR)
	{
		eaValuePtr = catgets(ODcat_handle, DOCSH_SET, DOCMSG_15,
		                     kODFileEaTypeValue);
	}
	else
		eaValuePtr = (char *)kODFileEaTypeValue;

   eaDataLength = strlen(eaValuePtr);

   const int  kFileFeaListSize = sizeof(FEA2LIST) + strlen(kODFileEaTypeName) +
                                 sizeof(eaDataType) + sizeof(eaDataLength) +
                                 eaDataLength;

	if (fPermission & kODFileWrite)
		access = OPEN_ACCESS_READWRITE;
	else
	{
		if (fPermission & kODFileRead)
			access = OPEN_ACCESS_READONLY;
	}

	if (fPermission & kODFileShareWrite)
		access |= OPEN_SHARE_DENYNONE;
	else
	{
		if (fPermission & kODFileShareRead)
			access |= OPEN_SHARE_DENYWRITE;
		else
			access |= OPEN_SHARE_DENYREADWRITE;
	}

	feaListBuffer = new char[kFileFeaListSize];
	// Set file type extended attribute to Sweepstakes Log
	feaList = (PFEA2LIST)feaListBuffer;

	feaList->cbList = kFileFeaListSize;
	feaList->list[0].oNextEntryOffset = 0;
	feaList->list[0].fEA = 0;
	feaList->list[0].cbName = strlen(kODFileEaTypeName);
	feaList->list[0].cbValue = eaDataLength + sizeof(eaDataType) +
	                           sizeof(eaDataLength);

	// Form the following data starting at szName:
	//   " .TYPE" <EAT_ASCII> <length of Value> <Value, no null term>
	tPtr = feaList->list[0].szName;
	strcpy(tPtr, kODFileEaTypeName);
	tPtr += strlen(kODFileEaTypeName) + 1;
	memcpy(tPtr, (const void *)&eaDataType, sizeof(eaDataType));
	tPtr += sizeof(eaDataType);
	memcpy(tPtr, (const void *)&eaDataLength, sizeof(eaDataLength));
	tPtr += sizeof(eaDataLength);
	memcpy(tPtr, eaValuePtr, feaList->list[0].cbValue);
	ea.fpGEA2List = NULL;
	ea.fpFEA2List = feaList;
	ea.oError = 0;

	if (closeCat)
		ODTermExceptions();

	error = DosOpen((char *)fFileSpec.name, &fDataRefNum, &openAction, 0L, FILE_NORMAL,
	                OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_FAIL_IF_EXISTS, access, &ea);
	if (error)
		THROW(error);

	fCreator = creator;
	fFileType = fileType;
	fScriptTag = scriptCode;

	Close();
}

//------------------------------------------------------------------------------
// PlatformFile::Exists
//------------------------------------------------------------------------------
ODBoolean PlatformFile::Exists()
{
	APIRET error;
	FILESTATUS3 fileInfo;

	error = DosQueryPathInfo((char *)fFileSpec.name, FIL_STANDARD,
	                         &fileInfo, sizeof(fileInfo));

	if (error &&
	    error != ERROR_FILE_NOT_FOUND && error != ERROR_PATH_NOT_FOUND)
		THROW(error);

	return (error == 0);
}

//------------------------------------------------------------------------------
// PlatformFile::Open
//------------------------------------------------------------------------------
void PlatformFile::Open()
{	
	if (fDataRefNum == kODNoFileRefNum)
	{
		ULONG access = 0;
		ULONG openAction;
		APIRET error;

		if (fPermission & kODFileWrite)
			access = OPEN_ACCESS_READWRITE;
		else
		{
			if (fPermission & kODFileRead)
				access = OPEN_ACCESS_READONLY;
		}

		if (fPermission & kODFileShareWrite)
			access |= OPEN_SHARE_DENYNONE;
		else
		{
			if (fPermission & kODFileShareRead)
				access |= OPEN_SHARE_DENYWRITE;
			else
				access |= OPEN_SHARE_DENYREADWRITE;
		}

		error = DosOpen((char *)fFileSpec.name, &fDataRefNum, &openAction, 0L, FILE_NORMAL,
		                OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW, access, 0L);
		if (error)
			THROW(error);

		fOpenPermission = fPermission;
	}
	else
		THROW(kODErrFileOpened);
}

//------------------------------------------------------------------------------
// PlatformFile::Close
//------------------------------------------------------------------------------
void PlatformFile::Close()
{	
	APIRET error;

	if (fDataRefNum != kODNoFileRefNum)
	{
		error = DosClose(fDataRefNum);
		if (error)
			THROW(error);
		fDataRefNum = kODNoFileRefNum;
	}
	else
		THROW(kODErrFileClosed);
}

//------------------------------------------------------------------------------
// PlatformFile::Delete
//------------------------------------------------------------------------------
void PlatformFile::Delete()
{	
	APIRET error;

	if (!IsDirectory())
	{
		error = DosDelete((char *)fFileSpec.name);
		if (error)
			THROW(error);
	}
	else
	{
		error = DosDeleteDir((char *)fFileSpec.name);
		if (error)
			THROW(error);
	}
}


//------------------------------------------------------------------------------
// PlatformFile::FlushVolume
//------------------------------------------------------------------------------
void PlatformFile::FlushVolume()
{	
	APIRET error;

	if (fDataRefNum != kODNoFileRefNum)
	{
		error = DosResetBuffer(fDataRefNum);
		if (error)
			THROW (error);
	}
}

//------------------------------------------------------------------------------
// PlatformFile::SetFilePos
//------------------------------------------------------------------------------
void PlatformFile::SetFilePos(ODSShort posMode, ODSLong posOff)
{	
	APIRET error;
	ULONG newPtr;

	error = DosSetFilePtr(fDataRefNum, posOff, posMode, &newPtr);
	if (error)
		THROW(error);
}

//------------------------------------------------------------------------------
// PlatformFile::GetFilePos
//------------------------------------------------------------------------------
ODSLong PlatformFile::GetFilePos()
{
	ULONG currOff;
	ODSLong	posOff;
	APIRET error;
	
	// Obtain the current file pointer by making a zero-byte change
	// to the current pointer, and use the "previous" offset which
	// is returned by DosSetFilePtr
	error = DosSetFilePtr(fDataRefNum, 0, FILE_CURRENT, &currOff);
	if (error)
		THROW(error);

	posOff = currOff;
	return posOff;
}

//------------------------------------------------------------------------------
// PlatformFile::Read
//------------------------------------------------------------------------------
void PlatformFile::Read(void* buffer, ODSLong* count)
{	
	ULONG bytesRead;
	APIRET error;
	ODSLong countRequested = *count;
	error = DosRead(fDataRefNum, buffer, *count, &bytesRead);
	*count = bytesRead;
	if (error)
		THROW (error);
	else
		if (countRequested != bytesRead)
			THROW (kODErrEndOfFile);
}

//------------------------------------------------------------------------------
// PlatformFile::Write
//------------------------------------------------------------------------------
void PlatformFile::Write(const void* buffer, ODSLong* count)
{	
	ULONG bytesWritten;
	ODSLong countRequested = *count;
	APIRET error;
	error = DosWrite(fDataRefNum, (void *)buffer, *count, &bytesWritten);
	*count = bytesWritten;
	if (error)
		THROW (error);
}

//------------------------------------------------------------------------------
// PlatformFile::GetEndOfFile
//------------------------------------------------------------------------------
ODSLong PlatformFile::GetEndOfFile()
{	
	ODSLong	length;
	FILESTATUS3 fileInfo;
	APIRET error;

	error = DosQueryFileInfo(fDataRefNum, INFO_LEVEL_1, &fileInfo, sizeof(fileInfo));
	if (error)
		THROW(error);

	length = fileInfo.cbFile;

	return length;
}

//------------------------------------------------------------------------------
// PlatformFile::SetEndOfFile
//------------------------------------------------------------------------------
void  PlatformFile::SetEndOfFile(ODSLong length)
{	
	unsigned long error;

	error = DosSetFileSize(fDataRefNum, length);

	if (error)
		THROW(error);
}

//------------------------------------------------------------------------------
// PlatformFile::MoveRename
//------------------------------------------------------------------------------
void PlatformFile::MoveRename( ODFileSpec* newSpec, ODBoolean isDuplicate )
{
	APIRET error;

	error = DosMove((char *)fFileSpec.name, (char *)newSpec);

	if (error == ERROR_ACCESS_DENIED && isDuplicate)
	{
		error = DosDelete((char *)fFileSpec.name);
		if (error)
			THROW(error);
		error = DosMove((char *)fFileSpec.name, (char *)newSpec);
	}

	if (error)
		THROW(error);

	strcpy ((char *)fFileSpec.name, (char *)newSpec);
}	// MoveRename()

//------------------------------------------------------------------------------
// PlatformFile::Rename
//		This routine will not yet delete a file that will prevent the
//			renaming (as Move() does above.)
//------------------------------------------------------------------------------
void PlatformFile::Rename( ODFilePathname newName )
{
	APIRET error;

	error = DosMove((char *)fFileSpec.name, (char *)newName);

	if (error)
		THROW(error);

	strcpy ((char *)fFileSpec.name, (char *)newName);
}

//------------------------------------------------------------------------------
// PlatformFile::GetFileModDate
//		This routine will set the file modification date to the input parameter
//------------------------------------------------------------------------------
ODTime PlatformFile::GetFileModDate()
{
	ODTime t;
	ODFilePermission savedPerm;

	FILESTATUS3 fileInfo;
	unsigned long error;
	BOOL closeFile = FALSE;

	// Note:  This function can be, and is, called when the file is
	//        not open.
	try
	{
		if (fDataRefNum == kODNoFileRefNum)
		{
			ODFilePermission savedPerm = fPermission;
			SetPermission(kODFileRead);
			Open();
			SetPermission(savedPerm);
			closeFile = TRUE;
		}
	}
	catch (...)
	{
		SetPermission(savedPerm);
		throw;
	}

	error = DosQueryFileInfo(fDataRefNum, INFO_LEVEL_1, &fileInfo, sizeof(fileInfo));

	try
	{
		if (closeFile)
			Close();
	}
	catch(...)
	{
		// Only rethrow the error if we haven't already experienced an error
		if (!error)
			throw;
	}

	if (!error)
		error = ConvertFromFileTime(&fileInfo.fdateLastWrite,
		                            &fileInfo.ftimeLastWrite, &t);

	if (error)
		THROW(error);

	return t;
}


//------------------------------------------------------------------------------
// PlatformFile::SetFileModDate
//		This routine will set the file modification date to the input parameter
//------------------------------------------------------------------------------
void PlatformFile::SetFileModDate(ODTime date)
{
	ODFilePermission savedPerm;
	FILESTATUS3 fileInfo;
	unsigned long error;
	BOOL closeFile = FALSE, reopenFile = FALSE;

	// Note:  This function can be, and is, called when the file is
	//        not open.  Code exists to also handle files open R/O.
	if (fDataRefNum != kODNoFileRefNum && !(fOpenPermission & kODFileWrite))
	{
		Close();
		reopenFile = TRUE;
	}

	try
	{
		if (fDataRefNum == kODNoFileRefNum)
		{
			savedPerm = fPermission;
			SetPermission(kODFileRead | kODFileWrite);
			Open();
			SetPermission(savedPerm);
			closeFile = TRUE;
		}
	}
	catch (...)
	{
		SetPermission(savedPerm);
		throw;
	}

	error = DosQueryFileInfo(fDataRefNum, INFO_LEVEL_1, &fileInfo, sizeof(fileInfo));

	if (!error)
		error = ConvertToFileTime(date, &fileInfo.fdateLastWrite,
		                          &fileInfo.ftimeLastWrite);
	if (!error)
		error = DosSetFileInfo(fDataRefNum, INFO_LEVEL_1, &fileInfo, sizeof(fileInfo));

	try
	{
		if (closeFile)
			Close();
		if (reopenFile)
			Open();
	}
	catch(...)
	{
		// Only rethrow the error if we haven't already experienced an error
		if (!error)
			throw;
	}

	if (error)
		THROW(error);
}

//------------------------------------------------------------------------------
// PlatformFile::SetFileCreationDate
//		This routine will set the file creation date to the input parameter
//------------------------------------------------------------------------------
void PlatformFile::SetFileCreationDate(ODTime date)
{
	ODFilePermission savedPerm;
	FILESTATUS3 fileInfo;
	unsigned long error;
	BOOL closeFile = FALSE, reopenFile = FALSE;

	// Note:  This function can be, and is, called when the file is
	//        not open.  Code exists to also handle files open R/O.
	if (fDataRefNum != kODNoFileRefNum && !(fOpenPermission & kODFileWrite))
	{
		Close();
		reopenFile = TRUE;
	}

	try
	{
		if (fDataRefNum == kODNoFileRefNum)
		{
			ODFilePermission savedPerm = fPermission;
			SetPermission(kODFileRead | kODFileWrite);
			Open();
			SetPermission(savedPerm);
			closeFile = TRUE;
		}
	}
	catch (...)
	{
		SetPermission(savedPerm);
		throw;
	}

	error = DosQueryFileInfo(fDataRefNum, INFO_LEVEL_1, &fileInfo, sizeof(fileInfo));

	if (!error)
		error = ConvertToFileTime(date, &fileInfo.fdateCreation,
		                          &fileInfo.ftimeCreation);
	if (!error)
		error = DosSetFileInfo(fDataRefNum, INFO_LEVEL_1, &fileInfo, sizeof(fileInfo));

	try
	{
		if (closeFile)
			Close();
		if (reopenFile)
			Open();
	}
	catch(...)
	{
		// Only rethrow the error if we haven't already experienced an error
		if (!error)
			throw;
	}

	if (error)
		THROW(error);
}
				
//------------------------------------------------------------------------------
// PlatformFile::GetFileCreationDate
//		This routine will get the file creation date and return it.
//------------------------------------------------------------------------------
ODTime PlatformFile::GetFileCreationDate()
{
	struct stat fileInfo;
	int error;

	if (fDataRefNum == kODNoFileRefNum)
		error = stat((char *)fFileSpec.name, &fileInfo);
	else
		error = fstat(fDataRefNum, &fileInfo);

	if (error < 0)
		THROW(errno);

	return fileInfo.st_ctime;
}

unsigned long  PlatformFile::ConvertFromFileTime(FDATE *fileDate, FTIME *fileTime, ODTime *odTime)
{
	struct tm tmCLib;

	tmCLib.tm_year = fileDate->year + 80;  // Normalize from 1980 base to 1900 base
	tmCLib.tm_mon = fileDate->month - 1;   // Normalize from 1 origin to 0 origin
	tmCLib.tm_mday = fileDate->day;
	tmCLib.tm_hour = fileTime->hours;
	tmCLib.tm_min = fileTime->minutes;
	tmCLib.tm_sec = fileTime->twosecs;
	tmCLib.tm_isdst = -1;

	*odTime = (ODTime) mktime(&tmCLib);
	
	return 0;
}

unsigned long PlatformFile::ConvertToFileTime(ODTime odTime, FDATE *fileDate, FTIME *fileTime)
{
	struct tm *tmCLib;
	unsigned long error = 0;

	tmCLib = localtime((time_t *)&odTime);

	if (tmCLib)
	{
		fileDate->year = tmCLib->tm_year - 80;
		fileDate->month = tmCLib->tm_mon + 1;
		fileDate->day = tmCLib->tm_mday;
		fileTime->hours = tmCLib->tm_hour;
		fileTime->minutes = tmCLib->tm_min;
		fileTime->twosecs = tmCLib->tm_sec;
	}
	else
		error = kODErrValueOutOfRange;

	return error;
}

void PlatformFile::GetFileAttributes(ODFileAttrFlags *fAttr )
{
	APIRET error;
	FILESTATUS3 fileInfo;

	error = DosQueryPathInfo((char *)fFileSpec.name, FIL_STANDARD,
	                         &fileInfo, sizeof(fileInfo));
	if (error)
		THROW(error);

	*fAttr = 0;

	if (fileInfo.attrFile & FILE_READONLY)
		*fAttr |= kODFileAttrReadOnly;
	else
	{
		ODFilePermission  savedPermission = fPermission;
		fPermission = kODFileRead | kODFileWrite;

		try
		{
			// It's possible that the filesystem is R/O (e.g. a filesystem
			// mounted R/O, or a diskette with write-protect tab set).  The
			// file itself may show up as having RW permissions, but a write
			// operation will not be allowed.  Please see defect 24848 for
			// more info.  It shouldn't be necessary, but the base OD code
			// can't handle Save operations that fail--you don't get a 2nd
			// chance and you'll lose your changes.  We detect the situation
			// a priori to avoid failed Save's.

			// Additional note:  The initial approach of getting the eof
			// and setting the eof back to the same value does not work on
			// Merlin.  They apparently optimized the code to detect the
			// no-op case and do nothing, so no error will ever result.
			// The newer approach involves reading the first byte in the
			// file and writing it back out.  If the file is a zero length
			// file, it must be truncated back to zero length after the test
			// write and before returning.  See defect 26715 for more info.
			Open();
			ODSLong eof = GetEndOfFile();
			ODSLong count;
			char buffer[1];
			buffer[0] = 'Z';

			if (eof > 0)
			{
				SetFilePos(kODSeekFromBeginning, 0);
				count = 1;
				Read(buffer, &count);
			}
			SetFilePos(kODSeekFromBeginning, 0);

			try
			{
				count = 1;
				Write(buffer, &count);
				if (eof == 0)
					SetEndOfFile(0);
			}
			catch (ODException &xcpt)
			{
				if (xcpt.error == ERROR_ACCESS_DENIED ||
				    xcpt.error == ERROR_WRITE_PROTECT)
					*fAttr |= kODFileAttrReadOnly;
				else
				{
					Close();
					throw;
				}
			}
			Close();
			fPermission = savedPermission;
		}
		catch (ODException &xcpt)
		{
			fPermission = savedPermission;
			throw;
		}
	}
}
