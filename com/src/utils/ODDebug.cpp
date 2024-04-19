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
// @(#) 1.34 com/src/utils/ODDebug.cpp, odutils, od96os2, odos29646d 10/11/96 09:36:52 [ 11/15/96 15:28:46 ]
/*
	File:		ODDebug.cpp

	Contains:	Useful debugging macros and functions.

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#include <somobj.xh>
#include <somcls.xh>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <fstream.h>
#include <ODTypesB.xh>

#if defined(__IBMCPP__) || defined(_IBMR2)
// If the VisualAge compiler or AIX's xlC, use National Language Support
// catalogs and portable message boxes.
#include <locale.h>                     /* Locale-specific information.      */

#include <iwindow.hpp>
#include <imsgbox.hpp>

#endif

#include <ErDefMap.h>

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
#include <qprintf.h>
#endif

#if defined(_PLATFORM_WIN32_)
#include <winbase.h>
#endif

const ODSShort kOutputBufferSize = 300; // Same as qprintf buffer length on OS/2
const char *kOutputFileName = "OpenDoc.log";

static somTD_SOMOutCharRoutine	*gOld_SOMOutCharRoutine = 0;
static somTD_SOMError			*gOld_SOMError = 0;

// The output buffer is circular. We keep two indices, one for the start
// of the most recent string and one for the end (the current input position.)
static char gOutputBuffer[kOutputBufferSize];
static ODSShort gOutputStart = 0;
static ODSShort gOutputEnd   = 0;

static DebugOutputMode gOutputMode = kWriteToDebugWindow;


void BREAK( const char[] );

// I/O Streams are more portable between platforms than native File I/O
ostream* gOutputFile = NULL;

//=====================================================================================
// ODInitExceptions
//=====================================================================================


extern "C" {
	static int SOMLINK  OD_SOMOutCharRoutine( char c );
	static void SOMLINK OD_SOMError( int error, string filename, int linenum );
}


WIN32_DLLEXPORT void
ODInitExceptions( )
{
	gOld_SOMOutCharRoutine	= SOMOutCharRoutine;
	gOld_SOMError			= SOMError;
	somSetOutChar((somTD_SOMOutCharRoutine *) OD_SOMOutCharRoutine);
#ifndef _PLATFORM_WIN32_
	SOMOutCharRoutine               = OD_SOMOutCharRoutine;
#endif
	SOMError				= OD_SOMError;
//	SOM_WarnLevel			= 1; // all, from somcdev.h

#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
	if (ODcat_handle == NULL) {
	   setlocale(LC_MESSAGES, NULL);

                                      /* Open the catalog.                 */
                                      /*  (using LC_MESSAGES, not LANG via */
                                      /*   the second argument).           */
	   ODcat_handle = catopen("ODMessag.cat", 0);
	   if (ODcat_handle == CATD_ERR)
	   {
	      somPrintf("Could not open ODMessag catalog!\n");
	   }
	}
#endif
        if (gOutputMode == kWriteToFile && !gOutputFile)
        {
	      gOutputFile = ::new ofstream(&kOutputFileName[0], ios::out|ios::app);
	      if (gOutputFile->bad() ) {
	         ::delete gOutputFile;
                 // output a message?
	      }
	}
}


//=====================================================================================
// ODTermExceptions
//=====================================================================================

WIN32_DLLEXPORT void ODTermExceptions( ) {
	gOld_SOMOutCharRoutine = 0;
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
        if (ODcat_handle != NULL) {
                catclose(ODcat_handle);
                ODcat_handle = NULL;
        }
#endif
        if (gOutputMode == kWriteToFile && gOutputFile)
        {
		::delete gOutputFile;
		gOutputFile = NULL;
        }
}

//=====================================================================================
// Output routines
//=====================================================================================



static int
SOMLINK
OD_SOMOutCharRoutine( char c )
{

        // Add to buffer
        gOutputBuffer[gOutputEnd++] = c;
#if defined(_PLATFORM_AIX_) || defined(_PLATFORM_OS2_)
        if( gOutputEnd >= (kOutputBufferSize-1) ) {
#endif
#ifdef _PLATFORM_WIN32_
        if( gOutputEnd >= kOutputBufferSize ) {
#endif
                if( gOutputMode == kWriteToDebugWindow )
                {
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
			qprintf("%s",gOutputBuffer);
#endif
#if defined(_PLATFORM_AIX_)
			cout << gOutputBuffer;
#endif
		}
                if( gOutputMode == kWriteToFile && gOutputFile)
                {
			*gOutputFile << gOutputBuffer;
		}
                gOutputEnd = 0;     // "clear" buffer
        }

#if defined(_PLATFORM_AIX_) || defined(_PLATFORM_OS2_)
	// AIX and OS/2 don't give a 0x00 to mark the end of the line
	if (c == '\n') {
		gOutputBuffer[gOutputEnd++] = c = 0x00;
	}
#endif

	// SOM gives a 0x00 to mark the end of the line
        if( c == 0x00 ) {                       // Dump buffer at end of line
                if( gOutputMode == kWriteToDebugWindow )
                {
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
			qprintf("%s",gOutputBuffer);
#endif
#if defined(_PLATFORM_AIX_)
			cout << gOutputBuffer << flush;
#endif
		}
                if( gOutputMode == kWriteToFile && gOutputFile)
                {
			*gOutputFile << gOutputBuffer << flush;
		}
                gOutputEnd = 0;     // "clear" buffer

        }
	return 1;
}



WIN32_DLLEXPORT DebugOutputMode
GetOutputMode( )
{
	return gOutputMode;
}


WIN32_DLLEXPORT void
SetOutputMode( DebugOutputMode mode )
{
  	if (gOutputMode != mode)
  	{
	   if (gOutputMode == kWriteToFile && gOutputFile)
	   { 
	      ::delete gOutputFile;
	      gOutputFile = NULL;
	   }
	   else if (mode == kWriteToFile)
	   {
	      gOutputFile = ::new ofstream(&kOutputFileName[0], ios::out|ios::app);
	      if (gOutputFile->bad() ) {
	         ::delete gOutputFile;
                 // output a message?
	      }
	   }
	}
	gOutputMode = mode;
}


//=================================================================================== 
// BREAK
//=================================================================================== 

void
BREAK( const char msg[] )
{
	somPrintf("%s\n",msg);

#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the portable message boxes if using the VisualAge compiler
	// or AIX's xlC
	char* EnvVar;
#ifdef _PLATFORM_WIN32_
	// For some reason getenv doesn't work well on WIN32
	char buffer[5];
	EnvVar = &buffer[0];
	int rc = GetEnvironmentVariable("OpenDocDisplayBoxes",EnvVar,5);
	if (rc == NULL)
	   EnvVar = NULL;
#else
	EnvVar = getenv("OpenDocDisplayBoxes");
#endif
	if ((EnvVar == NULL) || (strcmp(EnvVar,"yes") == 0)) {
	   try {
		IMessageBox box(IWindow::desktopWindow());
		box.setTitle( catgets(ODcat_handle, DEBG_SET, DEBG_10, "OpenDoc Warning") );
		box.show(msg, IMessageBox::warningIcon |
				IMessageBox::moveable);
	   // Don't stop processing because IMessageBox couldn't work.
	   }
	   catch (...) {}
	}
#endif // __IBMCPP__ or xlC
	
}


//=================================================================================== 
// SOMError
//=================================================================================== 


static void
SOMLINK OD_SOMError( int error, string filename, int linenum )
{
	const char *kSeverityCode[10] =
				{"error","warning","message","error?","error?",
				 "error template","error?","error?","error?","fatal error"};
	int base = error / 10000;
	int errornumber = (error - base*10000) / 10;
	int severity = error % 10;
	
	char msg[kOutputBufferSize+3];
	sprintf(msg, "SOM %s %d-%03d-%d (%s %d): Press G...",
						kSeverityCode[severity],base,errornumber,severity,
						filename,linenum);
	
	if( severity == SOM_Fatal ) {
		somPrintf("%s\n",msg);
	} else
		BREAK(msg);				// Nonfatal error: just break

	gOld_SOMError(error,filename,linenum);
}


//=================================================================================== 
// ASSERTION-FAILED
//=================================================================================== 


WIN32_DLLEXPORT void _AssertionFailed( char *cond, const char *fileName, const int lineNbr,
		ODError error, char *msg /*=NULL*/, int catalogIndex, void* catalogHandle, int setIndex )
{
	// Initialize somPrintf and the catalog handle, if not already done.
	ODBoolean InitializedCatalog = kODFalse;
	if (gOld_SOMOutCharRoutine == 0) {
		ODInitExceptions();
		InitializedCatalog = kODTrue;
	}

#if defined(__IBMCPP__) || defined(_IBMR2)
	// Save the Catalog handle, if provided as an input
	nl_catd localCatHandle = NULL;
	if (catalogHandle != NULL)
		localCatHandle = (nl_catd) catalogHandle;
	else
		localCatHandle = ODcat_handle;

	// Save the set index, if provided as an input
	int localSetIndex = NULL;
	if (setIndex != NULL)
		localSetIndex = setIndex;
	else
		localSetIndex = AMSG_SET;
#endif

	// Toss the path and just give the file name
#ifdef _PLATFORM_UNIX_
	const char* shortFileName = strrchr(fileName,'/');
#else
	const char* shortFileName = strrchr(fileName,'\\');
#endif
	if (shortFileName == NULL)
		shortFileName = fileName;
	else
		shortFileName++;
	
	// Format the message
	char dbg[256];
	char dbg2[256];
	char* msgptr = NULL;
	char* msgptr2 = NULL;
	int   index = NULL;

	// Make sure we were given a valid ODError, and if so, convert
	// it to a catalog index.
	// (all these errors are negative numbers. Keep that
	// in mind as you read this logic.)
	if ((error <= kODMaxError) && (error >= kODMinError)) {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
		index = CatIndex[kODMaxError - error];
		msgptr = catgets(ODcat_handle, EMSG_SET, index, &CatDefaultMsg[kODMaxError - error][0]);
#else
		msgptr = (char*) &CatDefaultMsg[kODMaxError - error][0];
#endif
	} else {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
//		sprintf(&dbg2[0], catgets(ODcat_handle, DEBG_SET, DEBG_60, "Due to condition (%1$s) not being met."), cond);
		sprintf(&dbg2[0], catgets(ODcat_handle, DEBG_SET, DEBG_60, "Due to condition (%s) not being met."), cond);
#else
		sprintf(&dbg2[0], "Due to condition (%s) not being met.", cond);
#endif
		msgptr = &dbg2[0];
	}


	// Format the message
	if(error != 0) {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
//		sprintf(&dbg[0], catgets(ODcat_handle, DEBG_SET, DEBG_50, "ODException (from line %1$d in file %2$s):"), lineNbr, shortFileName);
		sprintf(&dbg[0], catgets(ODcat_handle, DEBG_SET, DEBG_50, "ODException (from line %d in file %s):"), lineNbr, shortFileName);
#else
		sprintf(&dbg[0], "ODException (from line %d in file %s):", lineNbr, shortFileName);
#endif
		strcat(&dbg[0], " ");
		strcat(&dbg[0], msgptr);
	} else {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
//		sprintf(&dbg[0], catgets(ODcat_handle, DEBG_SET, DEBG_20, "ODWarning (from line %1$d in file %2$s):"), lineNbr, shortFileName);
		sprintf(&dbg[0], catgets(ODcat_handle, DEBG_SET, DEBG_20, "ODWarning (from line %d in file %s):"), lineNbr, shortFileName);
#else
		sprintf(&dbg[0], "ODWarning (from line %d in file %s):", lineNbr, shortFileName);
#endif
		strcat(&dbg[0], " ");
		strcat(&dbg[0], msgptr);
	}

	// If -1 was used as the index, don't get the message from a catalog
	if( catalogIndex == -1) {
		strcat(&dbg[0], "\n\t");
		strcat(&dbg[0], msg);
	} else
	// Add the extra message, if a catalog index was given
	if( catalogIndex != 0 ) {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
		msgptr2 = catgets(localCatHandle, localSetIndex, catalogIndex, msg);
#else
		msgptr2 = msg;
#endif

		strcat(&dbg[0], "\n\t");
		strcat(&dbg[0], msgptr2);
	}

	// Log the Assertion
	somPrintf("%s\n",dbg);

	// If given an extra message, but not a catalog index, print it to the
	// Log too
	if ((catalogIndex == 0) && (msg))
		somPrintf("\t%s\n",msg);

#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the portable message boxes if using the VisualAge compiler
	// or AIX's xlC
	char* EnvVar;
#ifdef _PLATFORM_WIN32_
	// For some reason getenv doesn't work well on WIN32
	char buffer[5];
	EnvVar = &buffer[0];
	int rc = GetEnvironmentVariable("OpenDocDisplayBoxes",EnvVar,5);
	if (rc == NULL)
	   EnvVar = NULL;
#else
	EnvVar = getenv("OpenDocDisplayBoxes");
#endif
	if ((EnvVar == NULL) || (strcmp(EnvVar,"yes") == 0)) {
	   try {
		IMessageBox box(IWindow::desktopWindow());
		if(error != 0) {
			box.setTitle( catgets(ODcat_handle, DEBG_SET, DEBG_40, "OpenDoc Exception") );
			box.show(dbg, IMessageBox::errorIcon |
					IMessageBox::moveable);
		} else {
			box.setTitle( catgets(ODcat_handle, DEBG_SET, DEBG_10, "OpenDoc Warning") );
			box.show(dbg, IMessageBox::warningIcon |
					IMessageBox::moveable);
		}
	   // Don't stop processing because IMessageBox couldn't work.
	   }
	   catch (...) {}
	}
#endif

	// If this routine initialized the catalog, it should close it too.	
	if( InitializedCatalog )
		ODTermExceptions();

	if( error!=0 )
		THROW(error, msg ?msg :"Assertion failed");
}

	
	
//=================================================================================== 
// WARN
//=================================================================================== 


WIN32_DLLEXPORT void _Warn( void* catalogHandle, int setIndex, int catalogIndex, const char* fileName, const int lineNbr, char *fmt, ... )
{
	char msg[512];
	char msg2[256];
	msg[0] = 0x00;
	msg2[0] = 0x00;

	// Initialize somPrintf and the catalog handle, if not already done.
	ODBoolean InitializedCatalog = kODFalse;
	if (gOld_SOMOutCharRoutine == 0) {
		ODInitExceptions();
		InitializedCatalog = kODTrue;
	}

#if defined(__IBMCPP__) || defined(_IBMR2)
	// Save the Catalog handle, if provided as an input
	nl_catd localCatHandle = NULL;
	if (catalogHandle != NULL)
		localCatHandle = (nl_catd) catalogHandle;
	else
		localCatHandle = ODcat_handle;

	// Save the set index, if provided as an input
	int localSetIndex = NULL;
	if (setIndex != NULL)
		localSetIndex = setIndex;
	else
		localSetIndex = AMSG_SET;
#endif

	// Toss the path and just give the file name
#ifdef _PLATFORM_UNIX_
	const char* shortFileName = strrchr(fileName,'/');
#else
	const char* shortFileName = strrchr(fileName,'\\');
#endif
	if (shortFileName == NULL)
		shortFileName = fileName;
	else
		shortFileName++;

	va_list args;
	va_start(args,fmt);

	// format and log the message
	// If -1 was used as the index, don't get the message from a catalog
	if (catalogIndex == -1) {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
//		sprintf(&msg[0], catgets(ODcat_handle, DEBG_SET, DEBG_20, "ODWarning (from line %1$d in file %2$s):"), lineNbr, shortFileName);
		sprintf(&msg[0], catgets(ODcat_handle, DEBG_SET, DEBG_20, "ODWarning (from line %d in file %s):"), lineNbr, shortFileName);
#else
		sprintf(&msg[0], "ODWarning (from line %d in file %s):", lineNbr, shortFileName);
#endif
		strcat(&msg[0], " ");
		vsprintf(msg+strlen(msg), fmt, args);
		somPrintf("%s\n",msg);
	} else
	if (catalogIndex != 0) {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
//		sprintf(&msg[0], catgets(ODcat_handle, DEBG_SET, DEBG_20, "ODWarning (from line %1$d in file %2$s):"), lineNbr, shortFileName);
		sprintf(&msg[0], catgets(ODcat_handle, DEBG_SET, DEBG_20, "ODWarning (from line %d in file %s):"), lineNbr, shortFileName);
		strcat(&msg[0], " ");
		vsprintf(msg+strlen(msg), catgets(localCatHandle, localSetIndex, catalogIndex, fmt), args);
#else
		sprintf(&msg[0], "ODWarning (from line %d in file %s):", lineNbr, shortFileName);
		strcat(&msg[0], " ");
		vsprintf(msg+strlen(msg), fmt, args);
#endif
		somPrintf("%s\n",msg);
	} else {
#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the NLS catalogs if using the VisualAge compiler or AIX's xlC
//		sprintf(&msg[0], catgets(ODcat_handle, DEBG_SET, DEBG_30, "ODWarning: A warning occurred at line %1$d in file %2$s."), lineNbr, shortFileName);
		sprintf(&msg[0], catgets(ODcat_handle, DEBG_SET, DEBG_30, "ODWarning: A warning occurred at line %d in file %s."), lineNbr, shortFileName);
#else
		sprintf(&msg[0], "ODWarning: A warning occurred at line %d in file %s.", lineNbr, shortFileName);
#endif
		somPrintf("%s\n",msg);
		vsprintf(msg2,fmt,args);
		somPrintf("\t%s\n",msg2);
	}
	va_end(args);


#if defined(__IBMCPP__) || defined(_IBMR2)
	// Use the portable message boxes if using the VisualAge compiler
	// or AIX's xlC
	char* EnvVar;
#ifdef _PLATFORM_WIN32_
	// For some reason getenv doesn't work well on WIN32
	char buffer[5];
	EnvVar = &buffer[0];
	int rc = GetEnvironmentVariable("OpenDocDisplayBoxes",EnvVar,5);
	if (rc == NULL)
	   EnvVar = NULL;
#else
	EnvVar = getenv("OpenDocDisplayBoxes");
#endif
	if ((EnvVar == NULL) || (strcmp(EnvVar,"yes") == 0)) {
	   try {
		IMessageBox box(IWindow::desktopWindow());
		box.setTitle( catgets(ODcat_handle, DEBG_SET, DEBG_10, "OpenDoc Warning") );
		box.show(msg, IMessageBox::warningIcon |
				IMessageBox::moveable);
	   // Don't stop processing because IMessageBox couldn't work.
	   }
	   catch (...) {}
	}
#endif

	// If this routine initialized the catalog, it should close it too.
	if( InitializedCatalog )
		ODTermExceptions();
}
	

//==============================================================================
// SAFE CAST
//==============================================================================


#if ODDebug

SOMObject*
_Cast( SOMObject *obj, SOMClass *cls, long majorversion, long minorversion )
{
	if( !somIsObj(obj) )
		WARNMSG_DEBUG(WARN_INDEX(-1),"Can't cast: %p is not a SOM object",obj);
	else if( !somIsObj(cls) )
		WARNMSG_DEBUG(WARN_INDEX(-1),"Can't cast: %p is not a SOM class",cls);
	else if( !obj->somIsA(cls) )
		WARNMSG_DEBUG(WARN_INDEX(-1),"Can't cast: %p is an %s, not an %s",obj, obj->somGetClassName(), cls->somGetName());
	else {
		return obj;
	}
	THROW(kODErrAssertionFailed);
	return NULL; /* keeps compiler quiet */
}

#else /*not ODDebug*/

extern "C" void _Cast( );
void _Cast( )
{
}
	
#endif /*ODDebug*/
