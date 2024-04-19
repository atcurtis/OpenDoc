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
// @(#) 1.4 com/src/utils/include/ODUtils.h, odutils, od96os2, odos29646d 8/19/96 11:56:00 [ 11/15/96 15:29:00 ]
/*
	File:		ODUtils.h

	Contains:	Object utility functions

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Theory Of Operation:
	
		These are useful utilities for use with objects, particularly ref-
		counted objects. Some are functions rather than macros, to reduce
		code size, and can be made inline later if necessary.
		
		ODDeleteObject deletes an object (which doesn't need to
			be a SOM object) and sets the variable pointing to it to NULL.
			If the pointer was already NULL, nothing happens.
			
		ODReleaseObject is similar, but releases the object instead of deleting
			it. Use this one for ref-counted objects.
		
		ODFinalReleaseObject is similar to ODReleaseObject, except that it is 
			meant to be used in code which is releasing the last reference
			to that object.  I.E. it does an ASSERT(refcount == 1) before calling release.
		
		ODRelease just releases an object (if it's not NULL.) Unlike ODReleaseObject
			it does not set your pointer to the object to NULL. There are a few
			occasions where you'll want to use this instead, but not many.
		
		ODFinalRelease is analogous to ODRelease; it is like ODFInalReleaseObject
			but does not reset your pointer to NULL.
			
		ODAcquireObject bumps the ref-count of an object, unless it's NULL.
		
		ODSafeReleaseObject also releases an object, but requires no Environment
			parameter and can't throw an exception. It's for use _only_ in
			somUninit methods, where no Environment is available.
		
		ODTransferReference lowers one object's refCount while incrementing
			another's. It's useful in things like setters where you want to
			forget about one object and remember another. It does the right
			thing if either pointer is NULL or if they point to the same object.
			** This function may throw an exception in the unlikely case that
				the Acquire or Release call fails.
		
		ODCopyAndRelease returns a copy of an object while releasing the object.
			This is useful when transferring control of an object to the caller,
			but the caller has permission to modify the object. It's optimized
			for the case where the ref-count of the object is 1, in which case
			it just returns the original object since no copying is needed.
			(This function works only on ODShapes and ODTransforms, which are the
			only classes that provide a Copy method.)
			** This function may throw an exception in the unlikely case that
				the GetRefCount or Release call fails.
		
		ODObjectsAreEqual returns kODTrue if two pointers point to the same object.
			In OpenDoc 1.0 this is the same thing as comparing the pointers;
			however, in the future when we use DSOM and objects may reside in
			different address spaces, it will be possible to have two different
			pointers to the same object! For future compatibility, use this function.
		
		ODAcquirePart, ODGetDraft and ODGetSession are convenience accessors.
			There's nothing magic about them, but they do save code size since they
			encapsulate several SOM calls. They also simplify the appearance of code.
	
	
		The "do...while(0)" blocks wrapped around the macros are there to prevent
		syntactic problems if a macro call is immediately followed by an "else"
		statement, to keep the "else" from binding to the "if" in the macro body.
		The "while" loop is optimized out by the compiler and has no effect on the
		flow of control or code quality.

	To Do:
	In Progress:
		
*/

#ifndef _ODUTILS_
#define _ODUTILS_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


#ifdef __cplusplus
	struct Environment;
	class ODObject;
	class ODShape;
	class ODTransform;
	class ODRefCntObject;
	class ODPart;
	class ODPersistentObject;
	class ODStorageUnit;
	class ODDraft;
	class ODSession;
	class ODWindow;
	class ODDocument;
	extern "C" {
#else
	#ifndef SOM_ODStorageUnit_h
	#include <StorageU.h>
	#endif
	#ifndef SOM_ODTransform_h
	#include <Shape.h>
	#endif
	#ifndef SOM_ODTransform_h
	#include <Trnsform.h>
	#endif
#endif

_DLLIMPORTEXPORT_ void ODRelease( Environment*, ODRefCntObject* );	// Just releases, does not set ptr to NULL

_DLLIMPORTEXPORT_ void ODFinalRelease(Environment*, ODRefCntObject* );

#define ODReleaseObject(EV,OBJ) 					\
			do{ ODRefCntObject *temp = (OBJ);		\
				(OBJ) = kODNULL;					\
				ODRelease((EV),temp); } while(0)

#define ODFinalReleaseObject(EV,OBJ) 				\
			do{ ODRefCntObject *temp = (OBJ);		\
				(OBJ) = kODNULL;					\
				ODFinalRelease((EV),temp); } while(0)

#define ODDeleteObject(OBJ) \
			if( !(OBJ) ) ; else { delete (OBJ); (OBJ)=kODNULL; }
			
_DLLIMPORTEXPORT_ void ODSafeReleaseObject( ODRefCntObject* );	// Needs no ev, throws no exceptions

_DLLIMPORTEXPORT_ void ODTransferReference( Environment*, ODRefCntObject *oldObj,
											   ODRefCntObject *newObj );

_DLLIMPORTEXPORT_ void ODAcquireObject(Environment* ev, ODRefCntObject* object);


#if ODDebug || !defined(_OD_IMPL_)
	ODBoolean ODObjectsAreEqual(Environment* ev, ODObject* a, ODObject* b);
#elif defined(__cplusplus)
	// Optimization for OD implementation since DSOM is not in the picture yet:
	inline ODBoolean ODObjectsAreEqual(Environment* ev, ODObject* a, ODObject* b)
						{return a==b;}
#else
	#define ODObjectsAreEqual(a,b)	((a)==(b))
#endif


#ifdef __cplusplus
}

// Overloaded functions can only be used in C++:

_DLLIMPORTEXPORT_ ODShape*	 ODCopyAndRelease( Environment*, ODShape* );
_DLLIMPORTEXPORT_ ODTransform* ODCopyAndRelease( Environment*, ODTransform* );

// All of these safely return NULL if the input object is NULL:

_DLLIMPORTEXPORT_ ODPart* ODAcquirePart( Environment*, ODFrame* );
_DLLIMPORTEXPORT_ ODPart* ODAcquirePart( Environment*, ODFacet* );

_DLLIMPORTEXPORT_ ODDraft* ODGetDraft( Environment*, ODStorageUnit* );
_DLLIMPORTEXPORT_ ODDraft* ODGetDraft( Environment*, ODPersistentObject* );

_DLLIMPORTEXPORT_ ODSession* ODGetSession( Environment*, ODStorageUnit* );
_DLLIMPORTEXPORT_ ODSession* ODGetSession( Environment*, ODPersistentObject* );
_DLLIMPORTEXPORT_ ODSession* ODGetSession( Environment*, ODDocument* );
_DLLIMPORTEXPORT_ ODSession* ODGetSession( Environment*, ODDraft* );
#endif


#endif //_ODUTILS_
