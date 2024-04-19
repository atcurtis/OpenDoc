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
// @(#) 1.6 com/src/utils/ODUtils.cpp, odutils, od96os2, odos29646d 8/19/96 11:49:39 [ 11/15/96 15:28:47 ]
/*
	File:		ODUtils.cpp

	Contains:	Utility functions for objects

	Owned by:	Jens Alfke

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.
		
*/

#define _OD_DONT_IMPORT_CPP_

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef SOM_Module_OpenDoc_Errors_defined
#include <ErrorDef.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#ifndef SOM_ODContainer_xh
#include <ODCtr.xh>
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef _ODEXCEPT_
#include <ODExcept.h>
#endif

#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif

#ifndef SOM_ODFacet_xh
#include <Facet.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include "StorageU.xh"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif


WIN32_DLLEXPORT void ODAcquireObject(Environment* ev, ODRefCntObject* object)
{
	if (object != kODNULL) 
	{
		object->Acquire(ev);
	}
}


WIN32_DLLEXPORT void ODRelease( Environment *ev, ODRefCntObject* obj )
{
	if( obj )
		obj->Release(ev);
}


WIN32_DLLEXPORT void ODFinalRelease( Environment *ev, ODRefCntObject* obj )
{
	if( obj ) {
#if ODDebug
		ODULong ref = obj->GetRefCount(ev);
		if( ref != 1 ) {
			WARNMSG(WARN_INDEX(0),"%s %p's refcount is %ld, not 1",
					obj->somGetClassName(),obj,ref);
		}
#endif
		obj->Release(ev);
	}
}


WIN32_DLLEXPORT void ODSafeReleaseObject( ODRefCntObject *obj )
{
	if( obj ) {
    	   try {
	    	obj->Release(somGetGlobalEnvironment());
	   } catch (ODException _exception) {
	    	// do nothing
	   }
	}
}


WIN32_DLLEXPORT void ODTransferReference( Environment *ev, ODRefCntObject *oldObj, ODRefCntObject *newObj )
{
	if( oldObj != newObj ) {
		if( newObj )
			newObj->Acquire(ev);
		if( oldObj ) {
			try {
				oldObj->Release(ev);
			} catch (ODException _exception) {
				if( newObj ) newObj->Release(ev);
				throw;
			}
		}
	}
}


WIN32_DLLEXPORT ODShape* ODCopyAndRelease( Environment *ev, ODShape *s )
{
	ASSERT(s!=kODNULL, kODErrIllegalNullInput);
	if( s->GetRefCount(ev) == 1 )
		return s;
	TempODShape tempS = s;		// Ensures that s gets released
	ODShape *copy = s->Copy(ev);
	return copy;
}

WIN32_DLLEXPORT ODTransform* ODCopyAndRelease( Environment *ev, ODTransform *t )
{
	ASSERT(t!=kODNULL, kODErrIllegalNullInput);
	if( t->GetRefCount(ev) == 1 )
		return t;
	TempODTransform tempT = t;		// Ensures that t gets released
	ODTransform *copy = t->Copy(ev);
	return copy;
}

#if ODDebug || !defined(_OD_IMPL_)
ODBoolean ODObjectsAreEqual(Environment* ev, ODObject* a, ODObject* b)
{
	return (a == b) || (a && a->IsEqualTo(ev,b));
}
#endif


WIN32_DLLEXPORT ODPart* ODAcquirePart( Environment *ev, ODFrame *f )
{
	return f ?f->AcquirePart(ev) :kODNULL;
}

WIN32_DLLEXPORT ODPart* ODAcquirePart( Environment *ev, ODFacet *f )
{
	return f ?f->GetFrame(ev)->AcquirePart(ev) :kODNULL;
}


WIN32_DLLEXPORT ODDraft* ODGetDraft( Environment *ev, ODStorageUnit *su )
{
	return su ?su->GetDraft(ev) :kODNULL;
}

WIN32_DLLEXPORT ODDraft* ODGetDraft( Environment *ev, ODPersistentObject *po )
{
	return po ?po->GetStorageUnit(ev)->GetDraft(ev) :kODNULL;
}


WIN32_DLLEXPORT ODSession* ODGetSession( Environment *ev, ODStorageUnit *su )
{
	return su ?su->GetSession(ev) :kODNULL;
}

WIN32_DLLEXPORT ODSession* ODGetSession( Environment *ev, ODPersistentObject *po )
{
	return po ?po->GetStorageUnit(ev)->GetSession(ev) :kODNULL;
}

WIN32_DLLEXPORT ODSession* ODGetSession( Environment *ev, ODDocument *d )
{
	return d ?d->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev) :kODNULL;
}

WIN32_DLLEXPORT ODSession* ODGetSession( Environment *ev, ODDraft *d )
{
	return d ?d->GetDocument(ev)->GetContainer(ev)->GetStorageSystem(ev)->GetSession(ev) :kODNULL;
}
