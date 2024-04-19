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
// @(#) 1.18 com/src/utils/IPlfmFil.cpp, odutils, od96os2, odos29646d 11/12/96 10:26:50 [ 11/15/96 15:49:44 ]
/*

	File:		PlfmFile.cpp

	Contains:	Implmentation for PlatformFile class

	Owned by:	Vincent Lo

	Copyright:	© 1993-95 by Apple Computer, Inc., all rights reserved.

*/

#include <stdio.h>
#include <string.h>
#include <wcstr.h>
#include <time.h>

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

#ifndef __ODDEBUG__
#include <ODDebug.h>
#endif

#ifndef __ERRORS__
#include <Errors.h>
#endif

#if defined(_PLATFORM_UNIX_)
#include <errno.h>
#endif // defined(_PLATFORM_UNIX_)

//==============================================================================
// Constants
//==============================================================================

const ODULong	kMaxCopyBufferSize			= 32 * 1024L;
const unsigned long kODMaxFileNameBufferSize = kODMaxFileNameSize + 1;
const unsigned long kODMaxWcsFileNameSize = kODMaxFileNameSize * sizeof(wchar_t);
const unsigned long kODMaxWcsFileNameBufferSize = kODMaxWcsFileNameSize + sizeof(wchar_t);

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

const ODOSType kBogusOSType = 0;	// Default type/creator value until it's read

//------------------------------------------------------------------------------
// PlatformFile::PlatformFile
//------------------------------------------------------------------------------
PlatformFile::PlatformFile()
{
	fCreator = kBogusOSType;
	fFileType = kBogusOSType;
	fScriptTag = 0;
	
	fFileSpec.name[0] = 0;
	fPermission = kODFileRead | kODFileWrite;
	fDataRefNum = kODNoFileRefNum;
}

//------------------------------------------------------------------------------
// PlatformFile::~PlatformFile
//------------------------------------------------------------------------------
PlatformFile::~PlatformFile()
{
}

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
//------------------------------------------------------------------------------
// PlatformFile::UniquifyAndCreate
//------------------------------------------------------------------------------
// This routine combines the UniquifyName and Create functions into one call
// because of a window resulting from a multitasking environment.  A race
// condition exists between different threads making a call to UniquifyName
// followed by a call to Create.  Different threads could conceivably have
// the same unique name returned, as long as no thread has yet created a file
// with that unique name.  The subsequent Create could fail due to a "file
// already exists" error, which lessens the value of calling UniquifyName in
// the first place.

// To make things easier on the caller, UniquifyName and Create have been
// rolled into one function.  This function iteratively calls UniquifyName and
// Create until a file with a unique name is successfully created (or until an
// error other than "file already exists" is encountered).
void PlatformFile::UniquifyAndCreate(ODOSType creator, ODOSType fileType,
                                     ODScriptCode scriptCode, short copyCount,
                                     ODBoolean forceNewName)
{
	int fileCreated = 0;
	short count = copyCount;
	ODFileSpec savedFileSpec = fFileSpec;

	do
	{
		// Create unique name (or use current one if not forcing new name)
		UniquifyName(kODNoResourceID, kODNULL, kODNoResourceID, kODNULL, count,
		             kSpecifyNewNameOnly, forceNewName);
		TRY
			Create(creator, fileType, scriptCode);
			fileCreated = 1;
		CATCH_ALL
			// If a "file already exists" error occurred, keep trying.  Otherwise,
			// rethrow the exception
#if defined(_PLATFORM_WIN32_)
			if (_exception.error != ERROR_FILE_EXISTS)
#elif defined(_PLATFORM_OS2_)
			if (_exception.error != ERROR_OPEN_FAILED || !Exists())
#elif defined(_PLATFORM_UNIX_)
			if (_exception.error != EEXIST)
#else // unimplemented platform
			#error Function not implemented on specified platform
#endif // defined(_PLATFORM_UNIX_)
				RERAISE;
			// On each iteration, the name must be reset to the original name;
			// otherwise numbers (appended from UniquifyName) will keep getting
			// appended on each iteration of this loop, instead of simply
			// incremented.
			fFileSpec = savedFileSpec;
			// UniquifyName may have had to increment more than this, but
			// at least one check will be avoided
			count++;
		ENDTRY;
		forceNewName = kODTrue;
	} while (!fileCreated);
}
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)


//------------------------------------------------------------------------------
// PlatformFile::Specify
//------------------------------------------------------------------------------
void PlatformFile::Specify(const ODFileSpec* fileSpec)
{
	// Note:  The following strncpy and null terminator assignment replaces a
	//        direct structure assignment because various callers were
	//        (incorrectly) casting char * to ODFileSpec *, and did not
	//        necessarily have sizeof(fFileSpec.name) bytes addressable.
	strncpy((char *)fFileSpec.name, (char *)fileSpec->name, sizeof(fFileSpec.name));
	fFileSpec.name[sizeof(fFileSpec.name) - 1] = '\0';
	fCreator = kBogusOSType;
	fFileType = kBogusOSType;
}

void PlatformFile::SpecifyFromFile(PlatformFile* file)
{
	const ODFileSpec &fileSpec = file->GetFileSpec();
	this->Specify(&fileSpec);
}

//------------------------------------------------------------------------------
// PlatformFile::SetAsciiname
//------------------------------------------------------------------------------
void PlatformFile::SetAsciiName(const char* asciiName)
{
	wchar_t *wcsFileName;
	size_t nameLength, copyLength;

	/******************************************************************
	Note: To copy the specified mbs (multi-byte string) string, strncpy
	      cannot be used, because the string may contain a mixture of
	      double and single byte characters, and the string must not be
	      truncated so that it ends with half of a double-byte
	      character.

	      The input string must be converted to a wcs (wide character
	      string) string and converted back to an mbs with the
	      desired max length.  This way, only complete characters
	      are copied, up to the given length.
	******************************************************************/
	wcsFileName = (wchar_t *)malloc(kODMaxWcsFileNameBufferSize);

	mbstowcs(wcsFileName, asciiName, kODMaxWcsFileNameBufferSize);
	copyLength = sizeof(fFileSpec.name) - 1;
	nameLength = wcstombs((char *)fFileSpec.name, wcsFileName,
	                     copyLength);
	if (nameLength == copyLength)
		fFileSpec.name[nameLength] = '\0';

	free(wcsFileName);
}

//------------------------------------------------------------------------------
// PlatformFile::IsLocked
//------------------------------------------------------------------------------
ODBoolean PlatformFile::IsLocked()
{
	return  kODFalse;
}

//------------------------------------------------------------------------------
// PlatformFile::Lock
//------------------------------------------------------------------------------
void PlatformFile::Lock()
{
}

//------------------------------------------------------------------------------
// PlatformFile::Unlock
//------------------------------------------------------------------------------
void PlatformFile::Unlock()
{
}

//------------------------------------------------------------------------------
// PlatformFile::GetName
//------------------------------------------------------------------------------
ODName* PlatformFile::GetName()
{
	return CreateIText(fScriptTag, 0, (char *)fFileSpec.name);
}

//------------------------------------------------------------------------------
// PlatformFile::GetAsciiName
//------------------------------------------------------------------------------
void PlatformFile::GetAsciiName(char* asciiName, ODULong maxLength)
{
	wchar_t *wcsFileName;
	size_t nameLength;

	/******************************************************************
	Note: To copy the specified mbs (multi-byte string) string, strncpy
	      cannot be used, because the string may contain a mixture of
	      double and single byte characters, and the string must not be
	      truncated so that it ends with half of a double-byte
	      character.

	      The input string must be converted to a wcs (wide character
	      string) string and converted back to an mbs with the
	      desired max length.  This way, only complete characters
	      are copied, up to the given length.
	******************************************************************/

	wcsFileName = (wchar_t *)malloc(kODMaxWcsFileNameBufferSize);

	mbstowcs(wcsFileName, (char *)fFileSpec.name, kODMaxWcsFileNameBufferSize);
	nameLength = wcstombs(asciiName, wcsFileName, maxLength);
	// Note:  The original DR4 code does this (i.e., potentially nulls out
	//        byte maxLength + 1).
	if (nameLength == maxLength)
		asciiName[maxLength] = '\0';

	free(wcsFileName);
}

//------------------------------------------------------------------------------
// PlatformFile::GetPlatformCreator
//------------------------------------------------------------------------------
ODOSType PlatformFile::GetPlatformCreator()
{
	return fCreator;
}	

//------------------------------------------------------------------------------
// PlatformFile::GetPlatformType
//------------------------------------------------------------------------------
ODOSType PlatformFile::GetPlatformType()
{
	return fFileType;
}	

//------------------------------------------------------------------------------
// PlatformFile::SetPlatformType
//------------------------------------------------------------------------------
void PlatformFile::SetPlatformType(ODOSType fileType)
{
	fFileType = fileType;
}	



//------------------------------------------------------------------------------
// PlatformFile::GetAllocationBlockSize
//------------------------------------------------------------------------------
ODULong PlatformFile::GetAllocationBlockSize()
{
	return 8192; // 8K
}

//------------------------------------------------------------------------------
// PlatformFile::CopyFrom
//------------------------------------------------------------------------------
void PlatformFile::CopyFrom( PlatformFile* src )
{
	ODBoolean srcIsOpen = (src->GetFileRefNum() != kODNoFileRefNum);
	if ( !srcIsOpen )
		src->Open();
	this->SetPermission( fsRdWrPerm );		// currently this is the default value
	if (!this->Exists())
		this->Create(src->GetPlatformCreator(), src->GetPlatformType(), 0 /*$$$$$ scriptcode?*/);

	this->Open();
			
	src->SetFilePos( fsFromStart, 0 );
	this->SetFilePos( fsFromStart, 0 );
	
	ODSLong bytesLeft = src->GetEndOfFile();

	void *hbuffer = kODNULL;
	ODSLong bufferSize = kMaxCopyBufferSize;
	if( bufferSize > bytesLeft )
		bufferSize = bytesLeft;

	if (bufferSize > 0)
	{
		hbuffer = new char [bufferSize];
		THROW_IF_NULL(hbuffer);
		void *buffer = hbuffer;

		ODSLong count;

		while( bytesLeft > 0 )
		{
			count = bytesLeft < bufferSize? bytesLeft: bufferSize ;
			src->Read( buffer, &count );
			this->Write( buffer, &count );
			bytesLeft -= count ;
		}
		delete hbuffer;
	}
	
	if ( !srcIsOpen )
		src->Close();
	this->Close();
}


//------------------------------------------------------------------------------
// PlatformFile::UniquifyName
// This function generates a unique name based on the filespec of this
// PlatformFile object.  A unique name is generated by appending the
// copyCount parameter value to the fileSpec.  The copyCount value is
// incremented until a unique name is generated.
// 
// This implementation is a simplified version of the original Apple version.
// Mac-specific parameters are ignored, and code for unused options was
// removed.  Only the copyCount and forceNewName parameters are used.
//------------------------------------------------------------------------------

const int kODMaxBaseFileNameSize = 8;

void PlatformFile::UniquifyName( ODSShort uniquifyingStringID,
			StringHandle uniquifyingString, 
			ODSShort uniquifyingNumberID,
			StringHandle uniquifyingNumberString, 
			short copyCount,
			PFUniquifyAction action, 
			ODBoolean forceNewName)
{ 

	if ((forceNewName != kODTryCurrentName) || this->Exists())
	{
		PlatformFile possibleOther;
		possibleOther.SpecifyFromFile(this);

		ODFileSpec originalFileSpec = this->GetFileSpec();
	
		char suffixString[kODMaxFileNameSize+1];
		sprintf(suffixString, "%d", copyCount);
			
		while (1)
		{
			ODFilePathname newFilePathname;
			strcpy ((char *)newFilePathname, (char *)originalFileSpec.name);
			ConcatenateSuffix (newFilePathname, suffixString);
	
			possibleOther.SetAsciiName((char *)newFilePathname);
			if (!possibleOther.Exists())
			{
				this->SetAsciiName((char *)newFilePathname);
				break;
			}

			// Increment suffix in quest to find unused name
			copyCount++;
			sprintf (suffixString, "%d", copyCount);
		}
	}
}	// UniquifyName

/**************************************************************************
 This routine is used to concatenate a suffix onto the end of a file name.
 Both the suffix and pathname are passed in by the caller.  The pathname
 is assumed to be anything from a fully-qualified pathname to just a
 file name.

 This routine must work with mbs (multi-byte string) strings, which
 complicates things quite a bit.  The basic approach is to convert the
 pathname to a wcs (wide character string) string and parse the wcs string
 into a pathname (without file name), file name (without extension), and
 extension.  Once these strings are determined, the path and extension are
 converted back to mbs strings, since no more manipulation on them will be
 necessary, except copying or concatenating them.

 The next step is to concatenate the suffix onto the file name.  This is
 done with wcs strings.  After the suffix is concatenated, the resulting
 name must be checked to make sure it does not exceed the maximum
 allowed file name length.  This must be done by converting the name to
 an mbs string and comparing its length to the maximum allowed length.  If
 truncation is necessary, the number of bytes that must be truncated is
 known, but not the characters, since mbs strings can be a mixture of
 single- and double-byte characters.  So that no more characters are
 truncated than necessary, the number of bytes to truncate is divided
 by two (rounding up), and that many characters are truncated from the
 wcs copy of the file name.  Since the name may consist of mostly
 single byte characters, more truncation may be necessary.  Thus
 the name truncation is an interative process.  The loop ends when
 no more truncation is necessary.  An exception is thrown if the name
 becomes completely truncated.

 Once the name is truncated, the length entire pathname (with the suffix
 added to the file name) is compared to the maximum allowd pathname
 length.  More truncation from the name may be required.  If necessary,
 truncation is continued in the same manner as above until the pathname
 is short enough or until an exception is thrown because the original
 file name has become completely truncated.
**************************************************************************/
void PlatformFile::ConcatenateSuffix(ODFilePathname filePathname, char *suffixString)
{
	wchar_t *wcsNamePtr, *wcsExtPtr;
	wchar_t *wcsFilePathname, *wcsFileName;
	char *pathNoName, *fileName, *fileExtension;
	long truncateLength, fileNameLengthInChars, suffixLength,
	     lengthOfAllButName;
	ODError error = 0;
	wchar_t driveSeparator = L';', extSeparator = L'.';
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
	wchar_t pathSeparator = L'\\';
#elif defined(_PLATFORM_UNIX_)
	wchar_t pathSeparator = L'/';
#else // unknown platform
	#error IPlfmFil.cpp:  This platform not supported.
#endif // defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)

	wcsFilePathname = (wchar_t *)malloc(kODMaxWcsFileNameBufferSize);
	wcsFileName = (wchar_t *)malloc(kODMaxWcsFileNameBufferSize);
	pathNoName = (char *)malloc(kODMaxFileNameBufferSize);
	fileName = (char *)malloc(kODMaxFileNameBufferSize);
	fileExtension = (char *)malloc(kODMaxFileNameBufferSize);

	suffixLength = strlen(suffixString);

	if (wcsFilePathname && wcsFileName && pathNoName && fileName && fileExtension)
	{
		// Convert the mbs pathname to wcs
		mbstowcs(wcsFilePathname, (char *)filePathname, kODMaxWcsFileNameBufferSize);

		/****************************************************************
		Find file name in wcs copy of pathname and copy to another
		buffer.  Strip the file name from the pathname.
		****************************************************************/
		wcsNamePtr = wcsrchr(wcsFilePathname, pathSeparator);
		if (!wcsNamePtr)
			wcsNamePtr = wcsrchr(wcsFilePathname, driveSeparator);
		if (wcsNamePtr)
			wcsNamePtr++;
		else
			wcsNamePtr = wcsFilePathname;

		// Make wcs copy of file name
		wcscpy(wcsFileName, wcsNamePtr);

		// Strip the file name from the wcs copy of the the pathname
		*wcsNamePtr = L'\0';
	
		/****************************************************************
		Locate file name extension in wcs copy of file name and copy to
		another buffer in mbs format.  Then strip the extension from
		the file name.
		****************************************************************/
		wcsExtPtr = wcschr(wcsFileName, extSeparator);
		if (wcsExtPtr)
		{
			wcstombs(fileExtension, wcsExtPtr, kODMaxFileNameBufferSize);
			*wcsExtPtr = L'\0';
		}
		else
			fileExtension[0] = '\0';

		/****************************************************************
		Copy pathname (without file name) into another buffer in mbs
		format. Deallocate wcs pathname buffer.
		****************************************************************/
		wcstombs(pathNoName, wcsFilePathname, kODMaxFileNameBufferSize);
		free (wcsFilePathname);
		wcsFilePathname = NULL;

		/****************************************************************
		Truncate the file name, if necessary, so that adding the suffix
		will not cause the name to exceed the maximum allowed length.
		****************************************************************/
		fileNameLengthInChars = wcslen(wcsFileName);
		do
		{
			// Create an mbs copy of the file name
			wcstombs(fileName, wcsFileName, kODMaxFileNameBufferSize);
			// Calculate the number of bytes that must be truncated
			// from the file name
			truncateLength = strlen(fileName) + suffixLength -
			                 kODMaxBaseFileNameSize;
			// Assume characters are double-byte, so divide the truncate
			// length by two, rounding up
			truncateLength++;
			truncateLength >>= 1;
			if (truncateLength > 0)
			{
				if (truncateLength > fileNameLengthInChars)
				{
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
					error = ERROR_CANNOT_MAKE;
#else // defined(_PLATFORM_UNIX_)
					error = ENAMETOOLONG;
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
					goto errexit;
				}

				// Truncate temporary copy of file name
				fileNameLengthInChars -= truncateLength;
				wcsFileName[fileNameLengthInChars] = L'\0';
			}
			else
				truncateLength = 0;
		} while (truncateLength > 0);


		/****************************************************************
		Truncate the file name some more, if necessary, so that adding
		the suffix will not cause the entire path name to exceed the
		maximum allowed length.
		****************************************************************/
		lengthOfAllButName  = strlen(pathNoName) + suffixLength +
		                      strlen(fileExtension);
		do
		{
			// Create an mbs copy of the file name
			wcstombs(fileName, wcsFileName, kODMaxFileNameBufferSize);
			// If total path name is still too long
			truncateLength =  lengthOfAllButName + strlen(fileName) - 
			                  kODMaxFileNameSize;
			// Assume characters are double-byte, so divide the truncate
			// length by two, rounding up
			truncateLength++;
			truncateLength >>= 1;
			if (truncateLength > 0)
			{
				if (truncateLength > fileNameLengthInChars)
				{
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
					error = ERROR_CANNOT_MAKE;
#else // defined(_PLATFORM_UNIX_)
					error = ENAMETOOLONG;
#endif // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
					goto errexit;
				}

				// Truncate temporary copy of file name some more
				fileNameLengthInChars -= truncateLength;
				wcsFileName[fileNameLengthInChars] = L'\0';
			}
		} while (truncateLength > 0);


		/****************************************************************
		Copy/concatenate path, filename, suffix, and file extension
		to caller's pathname buffer.
		****************************************************************/
		strcpy((char *)filePathname, pathNoName);
		strcat((char *)filePathname, fileName);
		strcat((char *)filePathname, suffixString);
		strcat((char *)filePathname, fileExtension);
	}
	else
		error = kODErrOutOfMemory;

errexit:
	
	if (wcsFilePathname)
		free (wcsFilePathname);
	if (wcsFileName)
		free (wcsFileName);
	if (pathNoName)
		free (pathNoName);
	if (fileName)
		free (fileName);
	if (fileExtension)
		free (fileExtension);

	if (error)
		THROW(error);
}
