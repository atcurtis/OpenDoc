/* @(#)Z 1.25 com/src/pubutils/src/ODExcept.cpp, odpubutils, od96os2, odos29646d 96/11/15 15:54:07 (96/10/29 09:28:51) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odpubutils
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
//====START_GENERATED_PROLOG======================================
//
//
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

#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_UNIX_) ) 
//  Programmers note:  The non-native (aka emulated) exception portion
//  of the ifdefs have been removed for clarity.  Therefore, the code
//  that remains should be regarded as #ifdef _NATIVE_EXCEPTIONS_.
#endif
/*
	File:		Except.cpp

	Contains:	Exception-handling utility

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef SOM_Module_OpenDoc_Errors_defined
#include "ErrorDef.xh"
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#ifndef __SOM__
#include <som.xh>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


//extern void BREAK( const char[] );

#ifdef _PLATFORM_MACINTOSH_
static ODBoolean gBreakOnThrow = kODFalse;	// Set to true to break on exceptions

#endif

const ODSize kSpareMemSize = 512;

static void* gSpareMem = kODNULL;
// gSpareMem is a block of memory we keep around to make sure that in an emergency
// we always have enough memory to allocate a SOM exception, by releasing the spare
// block if we have to.


#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODExceptions
#endif  // _PLATFORM_MACINTOSH_


#ifdef _PLATFORM_MACINTOSH_
//=====================================================================================
// Setting BreakOnThrow
//=====================================================================================


ODBoolean
BreakOnThrow( ODBoolean brk )
{
	ODBoolean oldBrk = gBreakOnThrow;
	gBreakOnThrow = brk;
	return oldBrk;
}
#endif // _PLATFORM_MACINTOSH_


//=====================================================================================
// Initializing ODException Structures
//=====================================================================================


static void
InitODException( ODException *x, ODError error, const char *message, ODSLong msgsize )
{
	x->error = error;
	if( message ) {
		memcpy(x->message,message,msgsize);
		x->message[msgsize] = '\0';
	} else
		x->message[0] = 0;
}


static ODException*
NewODException( ODError error, const char *message )
{
	SOMFree(gSpareMem);							// Get some slack
	
	ODULong size = message ?(strlen(message)+1) :1;
	if( size>256 ) size=256;
	ODException *x = (ODException*) SOMMalloc( sizeof(ODError) + size );
	
	if( x )
		InitODException(x,error,message,size);
	
	gSpareMem = SOMMalloc(kSpareMemSize);		// Tighten up again
	
	return x;
}



//=================================================================================== 
// THROW, et al
//=================================================================================== 


static void
DoThrow( ODError err, const char* msg, ODException *x = kODNULL )
{
	if( err == kODNoError ) {
		WARNMSG_DEBUG(WARN_INDEX(-1),"Do not call THROW(0)!");
		return;
	}
			
#ifdef DEBUG
        PRINT("** Exception! THROW(%d) called.\n", err);
#endif
	ODException xx;
	if (msg)  
	{
		ODSLong msglen;
		if ( strlen(msg) >= 255 )  
			msglen = 255;
		else
			msglen = strlen(msg);
		InitODException(&xx, err, msg, msglen);
	}
	else
		InitODException(&xx, err, msg, 0);
	throw(xx);

}



WIN32_DLLEXPORT void
THROW( ODError err )
{
	DoThrow(err,kODNULL);
}


WIN32_DLLEXPORT void
THROW_IF_ERROR( ODError err )
{
	if( err != 0 )
		DoThrow(err,kODNULL);
}


WIN32_DLLEXPORT void
THROW_IF_NULL( void* value )
{
	if( value == kODNULL )
		DoThrow(kODErrOutOfMemory,kODNULL);
}


#ifdef DEBUG
WIN32_DLLEXPORT void
THROW_IF_ERROR_M( ODError err, const char *msg )
{
	if( err != 0 )
		DoThrow(err,msg);
}

WIN32_DLLEXPORT void
THROW_M( ODError err, const char* msg )
{
	DoThrow(err,msg);
}

WIN32_DLLEXPORT void
THROW_IF_NULL_M( void* value, const char* msg )
{
	if( value == kODNULL )
		DoThrow(kODErrOutOfMemory,msg);
}
#endif /*DEBUG*/

// The following functions operate the same in debug mode and non-debug mode.
// The following functions are the same as the above THROW_M, THROW_IF_ERROR_M,
// and THROW_IF_NULL_M functions when debugging tools are enabled.
// When the debugging tools are not enabled, the following functions allow the
// created exceptions to contain messages while the THROW_M, THROW_IF_ERROR_M,
// and THROW_IF_NULL_M macros discard the passed in message and do not include
// the messages in the created exceptions. 
WIN32_DLLEXPORT void
THROW_IF_ERROR_MSG( ODError err, const char *msg )
{
	if( err != 0 )
		DoThrow(err,msg);
}

WIN32_DLLEXPORT void
THROW_MSG( ODError err, const char* msg )
{
	DoThrow(err,msg);
}

WIN32_DLLEXPORT void
THROW_IF_NULL_MSG( void* value, const char* msg )
{
	if( value == kODNULL )
		DoThrow(kODErrOutOfMemory,msg);
}

//=================================================================================== 
// SOM EXCEPTIONS
//===================================================================================


WIN32_DLLEXPORT void
ODSetSOMException( Environment *ev, ODError error, const char *message /*=NULL*/ )
{
	// Free the existing exception since pointer to this exception will be
	// lost if a new exception is created.  If no error was passed in, we 
	// need to clean up the old exception. 
	somExceptionFree(ev);
	ev->_major = NO_EXCEPTION;

	// If there is an error code, create a new exception.
	// If the new error code is different than in the original exception,
	// users should save have previously saved the original error code.
	if( error ) {
		ODException *x = NewODException(error,message);
			SOMFree(gSpareMem);							// Get some slack
			somSetException(ev,USER_EXCEPTION,ex_ODException,x);
			gSpareMem = SOMMalloc(kSpareMemSize);		// Tighten up
	}
}


WIN32_DLLEXPORT void
ODSetSOMException( Environment *ev, ODException &except )
{
	ODSetSOMException(ev,except.error,except.message);
}


WIN32_DLLEXPORT ODError
ODGetSOMException( Environment *ev )
{
	if( ev->_major ) {
		const char *excpName = somExceptionId(ev);
		if( strcmp(excpName,ex_ODException) == 0 ) {
			ODException *x = (ODException*)somExceptionValue(ev);
			return x->error;
		} else {
			WARNMSG_DEBUG(WARN_INDEX(-1),"Env has non-OpenDoc err: %s",excpName);
			return kODErrSOMException;
		}
	} else
		return kODNoError;
}

WIN32_DLLEXPORT void
ODSetErrorCode( Environment *ev, ODException &x, ODError err )
{
      if(err == kODNoError) {
        x.message[0] = 0;
        x.error = 0;
        if(!ev) {
          // use the global ev
          Environment *tmpEv = somGetGlobalEnvironment();
          somExceptionFree(tmpEv);
          tmpEv->_major = NO_EXCEPTION;
        }
        else {
          somExceptionFree(ev);
          ev->_major = NO_EXCEPTION;
        }
      }
      else {
        x.error = err;
      }
}

WIN32_DLLEXPORT void
CHECK_ENV( Environment *ev )
{
// The original version included the following two lines.
//			somExceptionFree(ev);
//			ev->_major = NO_EXCEPTION;
// These lines cleared the exception flag on the ev.  We elected not to do 
// this since the catch-handler may want to examine this value.  It is now
// the responsibility of the exception handler to 
// call SetErrorCode(kODNoError) to do this.   (RobC)

	if( ev->_major ) {
		const char *excpName = somExceptionId(ev);
		if( strcmp(excpName,ex_ODException) == 0 ) {
			ODException x = *(ODException*)somExceptionValue(ev);
			DoThrow(x.error, x.message);
		} else {
			WARNMSG_DEBUG(WARN_INDEX(-1),"Env has non-OpenDoc err: %s",excpName);
			DoThrow(kODErrSOMException,kODNULL);
		}
	}
}
