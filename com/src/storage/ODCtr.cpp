/* @(#)Z 1.8 com/src/storage/ODCtr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:03 (96/10/29 09:31:20) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
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

/*
	File:		ODCtr.cpp

	Contains:	Implementation of ODCtr

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 10/8/95	TJ		Fixes Recomended by Refball
		 <5>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <4>	 5/26/95	VL		1251403: Multithreading naming support.
		 <3>	 9/23/94	VL		1184166: ContainerName is ODIText now.
									1184272: ContainerID is now a sequence of
									octets.
		 <2>	  7/7/94	VL		Added SubClassResponsibility.
		 <1>	 6/28/94	VL		first checked in

	To Do:
*/

#define ODContainer_Class_Source
#include <ODCtr.xih>

#ifndef _ITEXT_
#include <IText.h>
#endif

#include <ODDebug.h>

#ifndef _BARRAY_
#include <BArray.h>
#endif

SOM_Scope ODStorageSystem*  SOMLINK ODContainerGetStorageSystem(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerGetStorageSystem");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODContainerID  SOMLINK ODContainerGetID(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerGetID");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	// The following lines are used to pacify the compiler.
	// They should never be executed.
	ODByteArray	ba;
	return ba;
}

SOM_Scope ODContainerName  SOMLINK ODContainerGetName(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerGetName");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

	// The following lines are used to pacify the compiler.
	// They should never be executed.
	ODContainerName	name;
	return name;
}

SOM_Scope void  SOMLINK ODContainerSetName(ODContainer *somSelf, Environment *ev,
		ODContainerName* name)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerSetName");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

/*
 *  private by convention
 */

SOM_Scope ODDocument*  SOMLINK ODContainerAcquireDocument(ODContainer *somSelf, Environment *ev,
		ODDocumentID id)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerAcquireDocument");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope void  SOMLINK ODContainerInitContainer(ODContainer *somSelf, Environment *ev,
		ODStorageSystem* system,
		ODContainerID* id)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerInitContainer");

	try
        {
           somSelf->InitRefCntObject(ev);
        }
	catch (ODException _exception)
        {
           ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODContainer*  SOMLINK ODContainerCreate(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerCreate");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");

	return somSelf;
}

SOM_Scope ODContainer*  SOMLINK ODContainerOpen(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerOpen");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return somSelf;
}

SOM_Scope ODContainer*  SOMLINK ODContainerClose(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerClose");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return somSelf;
}

SOM_Scope ODContainer*  SOMLINK ODContainerReleaseDocument(ODContainer *somSelf, Environment *ev,
		ODDocument* document)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerReleaseDocument");

	WARNMSG(WARN_INDEX(AMSG_630),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return somSelf;
}

SOM_Scope void  SOMLINK ODContainersomUninit(ODContainer *somSelf)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainersomUninit");

    ODContainer_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope void  SOMLINK ODContainerAcquire(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerAcquire");

    ODContainer_parent_ODRefCntObject_Acquire(somSelf,ev);
}

SOM_Scope void  SOMLINK ODContainerRelease(ODContainer *somSelf, Environment *ev)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerRelease");

    ODContainer_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope ODSize  SOMLINK ODContainerPurge(ODContainer *somSelf, Environment *ev,
		ODSize size)
{
    /* ODContainerData *somThis = ODContainerGetData(somSelf); */
    ODContainerMethodDebug("ODContainer","ODContainerPurge");

    return (ODContainer_parent_ODRefCntObject_Purge(somSelf,ev,size));
}
