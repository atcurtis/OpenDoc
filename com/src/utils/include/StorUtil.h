/* @(#)Z 1.7 com/src/utils/include/StorUtil.h, odstorage, od96os2, odos29646d 96/11/15 15:29:34 (96/08/20 11:07:27) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
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
/*
	File:		StorUtil.h

	Contains:	Utilities for Storage.

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/



#ifndef _STORUTIL_
#define _STORUTIL_

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMEMORY_
#include <ODMemory.h>
#endif

#ifdef __PRIVATE__
#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif
#endif // __PRIVATE__

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef _PLATFORM_MACINTOSH_
#include <somltype.h>
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif

class ODContainer;
class ODSession;
class ODStorageUnitView;
class ODStorageUnit;

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif


#ifdef _PLATFORM_MACINTOSH_
extern "C" {

	void ODSUAddPropValue(Environment* ev, ODStorageUnit* su,
                               ODPropertyName prop, ODValueType val);

	void ODSUForceFocus(Environment* ev, ODStorageUnit* su,
                             ODPropertyName prop, ODValueType val);

	ODBoolean ODSUExistsThenFocus(Environment* ev, ODStorageUnit* su,
                                       ODPropertyName prop, ODValueType val);

	void ODSURemoveProperty(Environment* ev, ODStorageUnit* su,
                                               ODPropertyName prop);

#else // _PLATFORM_MACINTOSH_

SOMEXTERN _DLLIMPORTEXPORT_  void SOMLINK ODSUAddPropValue(Environment* ev, ODStorageUnit* su,
                               ODPropertyName prop, ODValueType val);

SOMEXTERN _DLLIMPORTEXPORT_  void SOMLINK ODSUForceFocus(Environment* ev, ODStorageUnit* su,
                              ODPropertyName prop, ODValueType val);

SOMEXTERN _DLLIMPORTEXPORT_  ODBoolean SOMLINK ODSUExistsThenFocus(Environment* ev, ODStorageUnit* su,
                                       ODPropertyName prop, ODValueType val);

SOMEXTERN _DLLIMPORTEXPORT_  void SOMLINK ODSURemoveProperty(Environment* ev, ODStorageUnit* su,
                                                ODPropertyName prop);
	
#endif // _PLATFORM_MACINTOSH_


#ifdef __PRIVATE__
	PlatformFile*	GetPlatformFileFromContainer(Environment* ev, ODContainer* container);
	ODFileSpec	GetODFileSpecFromContainer(Environment* ev, ODContainer* container);

	ODContainer*	CreateFileContainer(Environment* ev, ODSession* session, FSSpec* fsSpec);
	ODContainer*	GetFileContainer(Environment* ev, ODSession* session, FSSpec* fsSpec);

	ODContainer*	CreateMemoryContainer(Environment* ev,
						ODSession* session,
						ODHandle handle,
						ODContainerType containerType);

	ODContainer*	GetMemoryContainer(Environment* ev, 
						ODSession* session,
						ODHandle handle,
						ODContainerType containerType);
#endif // __PRIVATE__


#ifdef _PLATFORM_MACINTOSH_
	ODULong	  StorageUnitGetValue(ODStorageUnit* su, Environment* ev,
						ODULong size, void *buffer);

	ODULong	  StorageUnitViewGetValue(ODStorageUnitView* suv, Environment* ev,
						ODULong size, void *buffer);

	void	StorageUnitSetValue(ODStorageUnit* su, Environment* ev,
					ODULong size, const void *buffer);

	void	StorageUnitViewSetValue(ODStorageUnitView* suv, Environment* ev,
					ODULong size, const void *buffer);

	void   StorageUnitSetPromiseValue(ODStorageUnit* su, Environment* ev,
					ODValueType value, ODULong offset,
			ODULong size, const void *buffer, ODPart *sourcePart);

	ODBoolean  StorageUnitGetStylFromStyledText(ODStorageUnit* su,
						Environment* ev,
						ODULong* size,
						ODPtr* styl);
						
	ODCloneKind  GetOriginalCloneKind(Environment* ev, ODDraft* draft);

	void  RemoveDataInterchangeProperties (Environment* ev,
						ODStorageUnit* su,
						ODBoolean clonedToFileContainer);
}

#else // _PLATFORM_MACINTOSH_

SOMEXTERN _DLLIMPORTEXPORT_  ODULong  SOMLINK  StorageUnitGetValue(ODStorageUnit* su, Environment* ev,
						ODULong size, void *buffer);

SOMEXTERN _DLLIMPORTEXPORT_  ODULong  SOMLINK  StorageUnitViewGetValue(ODStorageUnitView* suv, Environment* ev,
						ODULong size, void *buffer);


SOMEXTERN _DLLIMPORTEXPORT_  void  SOMLINK  StorageUnitSetValue(ODStorageUnit* su, Environment* ev,
					ODULong size, const void *buffer);

SOMEXTERN _DLLIMPORTEXPORT_  void  SOMLINK  StorageUnitViewSetValue(ODStorageUnitView* suv, Environment* ev,
					ODULong size, const void *buffer);

SOMEXTERN _DLLIMPORTEXPORT_  void  SOMLINK  StorageUnitSetPromiseValue(ODStorageUnit* su, Environment* ev,
					ODValueType value, ODULong offset,

			ODULong size, const void *buffer, ODPart *sourcePart);

						
SOMEXTERN _DLLIMPORTEXPORT_  ODCloneKind SOMLINK  GetOriginalCloneKind(Environment* ev, ODDraft* draft);

SOMEXTERN _DLLIMPORTEXPORT_  void  SOMLINK RemoveDataInterchangeProperties (Environment* ev,
						ODStorageUnit* su,
						ODBoolean clonedToFileContainer);
#endif // _PLATFORM_MACINTOSH_


#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif
	
#endif // _STORUTIL_
