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
// @(#) 1.9 com/src/utils/ODNewObj.cpp, odutils, od96os2, odos29646d 7/16/96 17:27:14 [ 11/15/96 15:28:47 ]
/*
	File:		ODNewObj.cpp

	Contains:	Abstract wrapper for instantiating objects by class-name

	Owned by:	Jens Alfke

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.
		
*/

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef _ODNEWOBJ_
#include "ODNewObj.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _PASCLSTR_
#include <PasclStr.h>
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifndef __SOM__
#include <som.xh>
#endif

#ifndef SOM_SOMClassMgr_xh
#include <somcm.xh>
#endif


#ifdef _PLATFORM_MACINTOSH_

	#ifndef __STRINGS__
	#include <strings.h>
	#endif
	
	#include <CodeFragments.h>
	
	#define LOAD_UNDER_ASSUMED_NAME   1
	/*	SOM requires that the library name (in the 'cfrg') be of the form "Module::ClassName".
		This obviously won't match the library's SYM file name. This causes problems for source
		level debuggers; they can't associate the two and you end up not being able to debug
		the library.
		To work around this, we give the library two names (two 'cfrg' entries), the second of
		which is just the classname. Then if the SYM file name matches the classname, we can first
		load the library by the classname, which will signal the debugger that it can link the
		library up with the similarly-named SYM file.
		Got that?? Off we go!!
	*/
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif
#if defined(_PLATFORM_WIN32_)
#ifndef __ODPAGTUN__
#include <ODPagTun.h>
#endif
#endif // defined(_PLATFORM_WIN32_)


#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
const ODSize kMinAppFreeSpace =		48 * 1024;
const ODSize kMinAppContigSpace =	 8 * 1024;

static somTD_SOMError			*gOld_SOMError;


extern "C" {
	static void
	Temp_GetClass_SOMError( int error, corbastring filename, int linenum );
}


static void
Temp_GetClass_SOMError( int error, corbastring filename, int linenum )
{
	int severity = error % 10;
	if( severity == SOM_Fatal ) {
		WARNMSG(WARN_INDEX(0),"Fatal SOM err %d caught during GetClass; throwing...",error);
		THROW(error);
	} else if( gOld_SOMError )
		gOld_SOMError(error,filename,linenum);
}
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)


static SOMClass*
GetClass( const char *className )
{
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	ODRequireFreeSpace(kMinAppFreeSpace,kMinAppContigSpace,kODTrue);
	
	char libname[64];

#if LOAD_UNDER_ASSUMED_NAME
	// First grab everything after the last colon in className:
	const char *lastcolon = strrchr(className,':');
	if (lastcolon == nil) {
		if( strcmp(className,"ODFileContainer")==0 ||
			strcmp(className,"ODMemContainer")==0 ||
			strcmp(className,"ODEmbeddedContainer")==0 )
				strcpy(libname, "OpenDoc Bento");
		else
				strcpy(libname, className);
	}
	else {
		strncpy(libname,lastcolon+1,sizeof(libname)-1);
		libname[sizeof(libname)-1] = '\0';
	}
#else
	strcpy(libname,className);
#endif /*LOAD_UNDER_ASSUMED_NAME*/
	
	c2pstr(libname);

	CFragConnectionID connID;
	Ptr mainAddr;
	Str255 errName;
	
	// First see if the library is already loaded:
	OSErr err = GetSharedLibrary((StringPtr)libname, kCurrentCFragArch,
								kFindCFrag, &connID,&mainAddr,errName);
								
	if( err == fragLibNotFound || err==fragLibConnErr ) {
		// Nope, need to load it:
		err = GetSharedLibrary((StringPtr)libname, kCurrentCFragArch,
									kLoadCFrag, &connID,&mainAddr,errName);
		if( err==noErr && !ODHaveFreeSpace(kMinAppFreeSpace,
										   kMinAppContigSpace,kODTrue) ) {
			CloseConnection(&connID);
			err = fragNoMem;
#if ODDebug
			CopyPascalString(errName,"\pOD free space too low");
#endif
		}
	}

	if( err ) {
		p2cstr(errName);
		WARNMSG(WARN_INDEX(0),"Can't load lib '%s'; error %hd, '%s'",
				p2cstr((StringPtr)libname),err,errName);
		THROW(err,(char*)errName);
	}
	
	/* Now try to load the class. Install a temporary error handler that
		converts a fatal SOM error into a THROW, in case SOM runs out of
		memory. */
	SOMClass *c;
	gOld_SOMError = SOMError;
	SOMError = &Temp_GetClass_SOMError;
	TRY{
		long majorVersion = 0;
		long minorVersion = 0;
		somId id = somIdFromString((corbastring)className);
		c = somGetDynamicClassReference(id,majorVersion,minorVersion, kODNULL);
		SOMFree(id);
	}CATCH_ALL{
		SOMError = gOld_SOMError;
		RERAISE;
	}ENDTRY
	SOMError = gOld_SOMError;
		
	
	if( !c )
		THROW(kODErrCantLoadSOMClass);
	return c;
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
	long majorVersion = 0;
	long minorVersion = 0;
	SOMClass* partClass = kODNULL;
	char *copy = new char[ strlen(className) + 1];
	strcpy( copy, className );
	char * somClassName = strtok(copy, ":");
	char * moduleName = strtok(NULL, ":");
	somId classId = somIdFromString(somClassName);
	partClass = 
		SOMClassMgrObject->somFindClsInFile(classId, majorVersion,
		                                    minorVersion, moduleName);
	SOMFree(classId);
	delete[] copy;
	
	return partClass;
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
}


SOMObject*
ODNewObject( const char *className )
{
	SOMClass *cls = GetClass(className);
	THROW_IF_NULL (cls);  // would have thrown anyway...
	SOMObject* obj = cls->somNew();
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	somReleaseClassReference(cls); // must release class reference
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	THROW_IF_NULL (obj); // similar to new ODObject, must throw if nil
	return obj;
}


ODBoolean
ODClassExists( const char *className )
{
	/* This function will now propagate errors if the library _does_ exist
	   but just couldn't be loaded due to e.g. insufficient memory or missing
	   imports. This allows for better error display to the user since otherwise
	   the error would be eaten and replaced with a simple Boolean return value. */
	
	ODBoolean result = kODTrue;
	TRY
		SOMClass *cls = GetClass(className);
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
		somReleaseClassReference ( cls );
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
	CATCH_ALL
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
		if( ErrorCode()==kODErrCantLoadSOMClass || ErrorCode()==fragLibNotFound )
#else // defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_UNIX_)
		if( ErrorCode()==kODErrCantLoadSOMClass )
#endif // !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_UNIX_)
			result = kODFalse;
		else
			RERAISE;
	ENDTRY
	return result;
}
