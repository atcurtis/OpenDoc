/* @(#)Z 1.9 com/src/pubutils/include/ODExcept.h, odpubutils, od96os2, odos29646d 96/11/15 15:52:18 (96/10/29 09:28:35) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odpubutils
 *
 *   CLASSES: none
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

// New exception model. (RobC)

#ifndef _ODEXCEPT_
#define _ODEXCEPT_

#ifdef __cplusplus           // This file is only valid in the C++ environment

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


#ifndef _ERRORDEF_
    #include "ErrorDef.xh"	// Clients probably need the error codes as well
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

// you can use the macros below instead of calling directly.
_DLLIMPORTEXPORT_ void ODSetErrorCode( Environment *, ODException &, ODError); 

extern "C" {

//=========================================================
// Exception Handling Macros (native C++ exceptions)
//=========================================================
// Programmer note.  These macros assume that the exception that
// has been caught is called: _exception.  That is, you should have
// a catch block that looks something like this:
//  catch(ODException _exception)
//     etc., etc.


#define ErrorCode()				(_exception.error)
#define ErrorMessage()			(_exception.message)
#define SetErrorMessage(MSG)	(strcpy(_exception.message,(MSG))

// this will update the global som ev exception flag
#define SetErrorCode(ERR)        (ODSetErrorCode(kODNULL, _exception, (ERR)))	

// this will update the som ev exception flag that is passed in
#define SetErrorCodeEv(EV, ERR)	 (ODSetErrorCode(EV, _exception, (ERR)))	

// we should consider removing this macro.
#define SET_SOM_EXCEPTION(EV)	ODSetSOMException(EV,_exception);


//=========================================================
// Raising Exceptions
//=========================================================

_DLLIMPORTEXPORT_ void THROW(ODError error);
_DLLIMPORTEXPORT_ void THROW_IF_ERROR(ODError error);
_DLLIMPORTEXPORT_ void THROW_IF_NULL(void* value);

// Optional message parameters (ignored in nondebug build):
#if defined(DEBUG)
_DLLIMPORTEXPORT_ void THROW_IF_ERROR_M(ODError error, const char* msg);
_DLLIMPORTEXPORT_ void THROW_M(ODError error, const char* msg);
_DLLIMPORTEXPORT_ void THROW_IF_NULL_M(void* value, const char* msg);
#else
#define THROW_IF_ERROR_M(ERR,MSG)	THROW_IF_ERROR(ERR)
#define THROW_M(ERR,MSG)			THROW(ERR)
#define THROW_IF_NULL_M(ERR,MSG)	THROW_IF_NULL(ERR)
#endif

// The following functions operate the same in debug mode and non-debug mode.
// The following functions are the same as the above THROW_M, THROW_IF_ERROR_M,
// and THROW_IF_NULL_M functions when debugging tools are enabled.
// When the debugging tools are not enabled, the following functions allow the
// created exceptions to contain messages while the THROW_M, THROW_IF_ERROR_M,
// and THROW_IF_NULL_M macros discard the passed in message and do not include
// the messages in the created exceptions. 
_DLLIMPORTEXPORT_ void THROW_IF_ERROR_MSG(ODError error, const char* msg);
_DLLIMPORTEXPORT_ void THROW_MSG(ODError error, const char* msg);
_DLLIMPORTEXPORT_ void THROW_IF_NULL_MSG(void* value, const char* msg);
}
// Overloaded C++ equivalents:
inline void THROW_IF_ERROR(ODError error, const char* msg)
		               {THROW_IF_ERROR_M(error,msg);}
inline void THROW(ODError error, const char* msg)
                               {THROW_M(error,msg);}
inline void THROW_IF_NULL(void* value, const char* msg)
                               {THROW_IF_NULL_M(value,msg);}
inline void THROW_IF_NULL(void* value, ODError error)
                               {if ( value == kODNULL ) THROW(error);}
extern "C" {

#ifdef _PLATFORM_MACINTOSH_
// Call BreakOnThrow(TRUE) to break into the debugger whenever THROW is called.
// (The call returns the previous value of the setting.)
ODBoolean BreakOnThrow( ODBoolean brk );
#endif


// ODSetSOMException stores an OD error code in the environment.
// ODGetSOMException returns the OD error code (if any) from an environment.

_DLLIMPORTEXPORT_ void	ODSetSOMException( Environment*, ODError, const char *msg =kODNULL );
} // end extern C
_DLLIMPORTEXPORT_ void	ODSetSOMException( Environment*, ODException& );

extern "C" {

_DLLIMPORTEXPORT_ ODError	ODGetSOMException( Environment *ev );

// CHECK_ENV throws an exception if the environment indicates an error.
_DLLIMPORTEXPORT_ void	CHECK_ENV( Environment* );

// SOMCHKEXCEPT is a macro that is called in a .xh file if the ev variable
// indicates an exception is set.
#define SOMCHKEXCEPT {CHECK_ENV(ev);}


} // end extern C
#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif


#endif // __cplusplus
#endif // _ODEXCEPT_
