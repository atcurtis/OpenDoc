/* @(#)Z 1.11 com/src/pubutils/include/Except.h, odpubutils, od96os2, odos29646d 96/11/15 15:29:12 (96/10/29 09:28:30) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odpubutils
 *
 *   CLASSES:   Destructo
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

#if ((defined (_PLATFORM_WIN32_) || defined (_PLATFORM_OS2_)) \
      || defined (_PLATFORM_AIX_) ) 
//  Programmers note:  The non-native (aka emulated) exception portion
//  of the ifdefs have been removed for clarity.  Therefore, the code
//  that remains should be regarded as #ifdef _NATIVE_EXCEPTIONS_.
#endif
/*
	File:		Except.h

	Contains:	Exception handling macros

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/


/*
	THEORY OF OPERATION
	
	This is a catch/throw based exception handling package which models
	much of the behavior of native C++ exception handling. (Its macros can
	be redefined to use native exceptions if your compiler supports them.)
	
	Reading this header is not recommended for the faint of heart! Here be
	gnarly macros, setjmp fu, and tangled #ifdefs.
	Please see the recipe document "Using Exceptions" for details presented
	in a human-readable fashion.
	
	If the symbol _NATIVE_EXCEPTIONS_ is predefined, native C++ exception
	handling will be used.
	If the symbol _ASM_XTRY_ is predefined, a special assembly-coded
	version of _xTry will be called, which makes the calling code for TRY
	shorter and slightly faster. (This feature still in progress...)
*/

#ifndef _EXCEPT_
#define _EXCEPT_

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


#ifndef _ERRORDEF_
	#ifdef __cplusplus
	#include "ErrorDef.xh"	// Clients probably need the error codes as well
	#else
	#include "ErrorDef.h"
	#endif
#endif

#ifdef __LIBRARYMANAGER__
#error "Please don't include both Except.h and LibraryManager.h"
#endif


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

#ifdef __cplusplus
extern "C" {
#endif


//=====================================================================================
// ODVolatile
//=====================================================================================

// Any variable or parameter that is modified in a TRY block and used in the
// CATCH block must be declared as volatile or it may have an incorrect value
// in the CATCH block.
// Since not all compilers support the 'volatile' keyword, use this instead;

#ifndef _NATIVE_EXCEPTIONS_
// removed. (RobC)
#else
#define ODVolatile(x)	/*no need for this with native exceptions*/
#endif

//=====================================================================================
// Exception Handling Macros (native C++ exceptions)
//=====================================================================================

#ifdef _NATIVE_EXCEPTIONS_

#define TRY													\
				try {

#define CATCH_ALL											\
				} catch(ODException _exception) {

#define RERAISE												\
				throw

#define ENDTRY												\
		        }

/* CATCH( ) will not work with native exceptions. Don't use it! */


//=====================================================================================
// Exception Handling Macros (emulated)
//=====================================================================================

#else /*not _NATIVE_EXCEPTIONS*/

// removed. (RobC)

		
#endif /*_NATIVE_EXCEPTIONS*/

//=====================================================================================
// Raising Exceptions
//=====================================================================================

// see ODExcept.h

//=====================================================================================
// SOM Exception Utilities
//=====================================================================================

// This modified TRY block should be used in SOM methods. It's just like a
// regular TRY...CATCH_ALL...ENDTRY except that the exception code will be
// stored in the Environment. Needless to say you should _not_ reraise!
// You should also not make any SOM calls after the SOM_ENDTRY, nor declare any
// Destructo objects before the SOM_TRY.

#ifdef _NATIVE_EXCEPTIONS_

#define SOM_TRY 												\
			TRY
			
#define SOM_CATCH_ALL											\
			CATCH_ALL {											\
				
#define SOM_ENDTRY												\
			} ODSetSOMException(ev,_exception);					\
			ENDTRY
			
#else /*not _NATIVE_EXCEPTIONS_*/

// removed. (RobC)

#endif /*_NATIVE_EXCEPTIONS_*/


//=====================================================================================
// Finally, the Exception Handler (ODExceptionFrame struct)
//=====================================================================================


#ifndef _NATIVE_EXCEPTIONS_

// removed. (RobC)


#endif /*_NATIVE_EXCEPTIONS_*/


#ifdef __cplusplus
}	// end of extern "C" block
#endif


//=====================================================================================
// Destructo, a C++ base class for auto-destruct objects
//=====================================================================================

#ifdef __cplusplus

class Destructo
{
#ifndef _NATIVE_EXCEPTIONS_
// removed. (RobC)
#endif /*_NATIVE_EXCEPTIONS_*/

	static void* operator new( size_t );	// Make it illegal to allocate on heap
											// In future could use alloca??
											
	private: // disallow these:
	Destructo(const Destructo& );
	void operator=(const Destructo& );
	// Bitwise assigning one destructo to another smashes
	// fPrevDestructo with potentially unpleasant effects.
};

#endif /*__cplusplus*/

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif


#endif // _EXCEPT_
