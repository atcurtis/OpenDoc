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
// @(#) 1.22 com/src/utils/include/ODDebug.h, odutils, od96os2, odos29646d 9/4/96 14:14:28 [ 11/15/96 15:29:00 ]
/*
	File:		ODDebug.h

	Contains:	Useful debugging macros and functions.

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/


#ifndef _ODDEBUG_
#define _ODDEBUG_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

#include <ODMessag.h>

// Keep the handle to OpenDoc's message catalog here, so all OpenDoc
// Runtime code has access to it.
#if defined(_IBMR2)
	// If using xlC on AIX, use the built-in message catalog functions
	#include <nl_types.h>
	extern nl_catd ODcat_handle;
#endif
#ifdef __IBMCPP__
	// If using the VisualAge compiler, use our local message catalog
	// functions
	#include <odnltyps.h>
	extern nl_catd _Import ODcat_handle;
#endif

#ifdef __cplusplus
extern "C" {
#endif


//=====================================================================================
// ODInitExceptions
//=====================================================================================

_DLLIMPORTEXPORT_ void ODInitExceptions( );

#ifndef _PLATFORM_MACINTOSH_

// This really doesn't do anything for the exceptions, it just closes
// the message catalog and the Opendoc log.  It's named the way it is
// because the original Initialization function was named "ODInitExceptions"
_DLLIMPORTEXPORT_ void ODTermExceptions( );

#endif


//=====================================================================================
// Debug Output
//=====================================================================================

enum DebugOutputMode {
	kNoOutput,
	kWriteToFile,
#ifndef _PLATFORM_MACINTOSH_
	kWriteToDebugWindow
#else
	kWriteToDebugWindow,
	kGenerateDebugStrs
#endif
};

_DLLIMPORTEXPORT_ DebugOutputMode	GetOutputMode( );
_DLLIMPORTEXPORT_ void			SetOutputMode( DebugOutputMode );


//=====================================================================================
// Warnings
//=====================================================================================

#ifdef _PLATFORM_MACINTOSH_
// WARN has the same syntax as printf but produces a SysBreakStr.
// Warnings are disabled (and generate no code) when ODDebug is off.

#define WARN	if(!ODDebug) ; else _Warn

#else

// WARNMSG prints a Warning.
// WARN_INDEX and WARN_CATALOG are used as the first parameter of WARNMSG.
// WARN_INDEX is used internally by OpenDoc, and assumes the use of the
//            ODMessage catalog, and the AMSG set.
// WARN_CATALOG can be used by anyone.  You pass it a handle to an open
//            message catalog, the set number, and the message index.
// Two examples of valid WARNMSG calls are as follows:
//   WARNMSG(WARN_INDEX(0),"Here is a message without parameters.");
//   WARNMSG(WARN_INDEX(0),"Message %d has a parameter.",2);
#define WARN_INDEX(A)	NULL, NULL, A, __FILE__, __LINE__
#define WARN_CATALOG(A,B,C)	(void*) A, B, C, __FILE__, __LINE__
#define WARNMSG		_Warn

// If you have a DEBUG define set, you can use the ???_DEBUG macros.
// They equate to real calls when DEBUG is set, and comments when
// DEBUG is not set.  This way you can have warnings and asserts that
// are only active when you have compiled with DEBUG set.
#ifdef DEBUG
	#define WARNMSG_DEBUG	_Warn
#else
	#define WARNMSG_DEBUG	//
#endif

#endif // _PLATFORM_MACINTOSH_


//=====================================================================================
// Assertions
//=====================================================================================

// These all cause a debugger break if the condition evaluates to false or 0.
// Leading "W" is a Warning: it doesn't raise an exception.
// Trailing "M" means special Message displayed instead of condition.

#ifndef __MWERKS__
	#define _FIL_ ""			/* MPW puts entire pathnames in; yuk! */
#else
	#define _FIL_ __FILE__
#endif

#ifdef _PLATFORM_MACINTOSH_
#define ASSERT( COND, ERR )	\
	if(!ODDebug || (COND)) ; else _AssertionFailed( #COND, _FIL_, ERR )
#define ASSERTM( COND, ERR, MSG )	\
	if(!ODDebug || (COND)) ; else _AssertionFailed( #COND, _FIL_, ERR, MSG )
#define WASSERT( COND )	\
	if(!ODDebug || (COND)) ; else _AssertionFailed( #COND, _FIL_, 0)
#define WASSERTM( COND, MSG )	\
	if(!ODDebug || (COND)) ; else _AssertionFailed( #COND, _FIL_, 0, MSG)
#else // not _PLATFORM_MACINTOSH_

#define ASSERT( COND, ERR )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, ERR )

#define ASSERTMSG( COND, ERR, MSG, CATALOGINDEX )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, ERR, MSG, CATALOGINDEX )

#define ASSERTCATALOG( COND, ERR, MSG, CATALOGINDEX, CATALOGHANDLE, SETINDEX ) \
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, ERR, MSG, CATALOGINDEX, (void*) CATALOGHANDLE, SETINDEX )

#define WASSERT( COND )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, 0)

#define WASSERTMSG( COND, MSG, CATALOGINDEX )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, 0, MSG, CATALOGINDEX)

#define WASSERTCATALOG( COND, MSG, CATALOGINDEX, CATALOGHANDLE, SETINDEX )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, 0, MSG, CATALOGINDEX, (void*) CATALOGHANDLE, SETINDEX )

// If you have a DEBUG define set, you can use the ???_DEBUG macros.
// They equate to real calls when DEBUG is set, and comments when
// DEBUG is not set.  This way you can have warnings and asserts that
// are only active when you have compiled with DEBUG set.
#ifdef DEBUG
	#define ASSERT_DEBUG( COND, ERR )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, ERR )
	#define ASSERTMSG_DEBUG( COND, ERR, MSG )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, ERR, MSG, -1 )
	#define WASSERT_DEBUG( COND )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, 0)
	#define WASSERTMSG_DEBUG( COND, MSG )	\
	if(COND) ; else _AssertionFailed( #COND, __FILE__, __LINE__, 0, MSG, -1)
#else
	#define ASSERT_DEBUG( COND, ERR )	//
	#define ASSERTMSG_DEBUG( COND, ERR, MSG )	//
	#define WASSERT_DEBUG( COND )	//
	#define WASSERTMSG_DEBUG( COND, MSG )	//
#endif
#endif // _PLATFORM_MACINTOSH_

// ASSERT_NOT_NULL causes a debugger break and an exception if the parameter
// is NULL. Use this in functions that take a pointer as a parameter but do
// not allow the parameter to be NULL.
// Do **NOT** use this macro to make sure memory allocation succeeded! It
// has no effect in non-debug builds. Use THROW_IF_NULL instead.

#define ASSERT_NOT_NULL(PARAM) \
	ASSERT((PARAM)!=kODNULL, kODErrIllegalNullInput)


//=====================================================================================
// Logging
//=====================================================================================

// PRINT writes to the standard output via somPrintf.

#ifdef _PLATFORM_MACINTOSH_
#define PRINT   if(!ODDebug) ; else somPrintf
#else
#define PRINT	somPrintf
#endif

// LOG is like PRINT but can easily be turned on or off on a per-file basis.
// To enable logging in a source file, you must redefine the symbol LOGGING
// as "1" in that file, otherwise LOG statements will not be compiled. Make
// sure to #undef the symbol before you re-#define it, as some compilers
// won't redefine an already-defined symbol.

// PRINT and LOG statements do not generate any code if logging is off.

#define LOGGING 0		// Redefine as 1 in source file to enable logging

#ifdef _PLATFORM_MACINTOSH_
#define LOG		if(!ODDebug || !LOGGING) ; else somPrintf
#else
#define LOG		if(!LOGGING) ; else somPrintf
#endif


//==============================================================================
// Safe Type-Casting
//==============================================================================

/*	Use CAST as a safe way to cast a SOM object from one class to another.
	For instance, if "o" is declared as an ODObject*, but your code knows
	it's an ODPart*, you can say:
			ODPart *part = CAST(o,ODPart);
	If ODDebug is turned on, this will do a runtime check and cause an assertion
	failure if o does not point to an ODPart (or subclass). Without ODDebug,
	it degenerates into a simple C-style cast that generates no code.
	
	ASSERT_IS_A is similar to CAST but is used when you just want to assert
	that the pointer points to the right kind of object.
*/

#if ODDebug
#ifdef _PLATFORM_MACINTOSH_
	#define CAST(OBJ, CLASS)	( (CLASS*)_Cast((OBJ), (somClassDataStructure*) &CLASS##ClassData,		\
														CLASS##_MajorVersion, CLASS##_MinorVersion) )
#else
	#define CAST(OBJ, CLASS)	( (CLASS*)_Cast((OBJ), CLASS##ClassData.classObject,		\
														CLASS##_MajorVersion, CLASS##_MinorVersion) )
#endif // _PLATFORM_MACINTOSH_
	#define ASSERT_IS_A(OBJ,CLASS)	( (void) CAST(OBJ,CLASS) )
#else
	#define CAST(OBJ, CLASS)	( (CLASS*) (OBJ) )
	#define ASSERT_IS_A(OBJ,CLASS)	/* */
#endif


//=====================================================================================
// Internal goop...
//=====================================================================================

#ifdef _PLATFORM_MACINTOSH_
void _Warn				( char *fmt, ... );
void _AssertionFailed	( char *cond,  char *fileName,
							ODError, char *msg = kODNULL );
#else
_DLLIMPORTEXPORT_ void _Warn( void* catalogHandle, int setIndex, int catalogIndex, const char* fileName, const int lineNbr, char *fmt, ... );
_DLLIMPORTEXPORT_ void _AssertionFailed	( char *cond,  const char *fileName, const int lineNbr,
			ODError, char *msg = kODNULL, int catalogIndex=NULL, void* catalogHandle=NULL, int setIndex=NULL );
#endif
#if ODDebug
#ifdef _PLATFORM_MACINTOSH_
SOMObject* _Cast( SOMObject *obj, somClassDataStructure *cls, long major, long minor );
#else
SOMObject* _Cast( SOMObject *obj, SOMClass *cls, long major, long minor );
#endif
#endif


#ifdef __cplusplus
}
#endif

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif


#endif /*_ODDEBUG_*/
