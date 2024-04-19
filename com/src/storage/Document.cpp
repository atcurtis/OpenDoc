/* @(#)Z 1.8 com/src/storage/Document.cpp, odstorage, od96os2, odos29646d 96/11/15 15:25:49 (96/10/29 09:30:51) */
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
	File:		Document.cpp

	Contains:	Implementation of ODDocument
	
	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <7>	 10/8/95	TJ		Fixes Recomended by Refball
		 <6>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <5>	 5/26/95	VL		1251403: Multithreading naming support.
		 <4>	 9/23/94	VL		1184166: DocumentName is ODIText now.
		 <3>	  9/6/94	VL		1184154: Removed Document Properties.
		 <2>	 7/11/94	VL		Added Exists and SubClassResponsibility.
		 <1>	 6/28/94	VL		first checked in

	To Do:
*/

#define ODDocument_Class_Source
#include <Document.xih>

#include <ODDebug.h>

#ifndef _ITEXT_
#include <IText.h>
#endif

SOM_Scope ODContainer*  SOMLINK ODDocumentGetContainer(ODDocument *somSelf, Environment *ev)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentGetContainer");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODDocumentID  SOMLINK ODDocumentGetID(ODDocument *somSelf, Environment *ev)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentGetID");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return 0;
}

SOM_Scope ODDocumentName  SOMLINK ODDocumentGetName(ODDocument *somSelf, Environment *ev)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentGetName");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	
	// The following two lines are used to pacify the compiler.
	// They should never be executed.
	ODDocumentName	name;
	return name;
}

SOM_Scope void  SOMLINK ODDocumentSetName(ODDocument *somSelf, Environment *ev,
		ODDocumentName* name)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentSetName");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope ODDocument*  SOMLINK ODDocumentCollapseDrafts(ODDocument *somSelf, Environment *ev,
		ODDraft* from,
		ODDraft* to)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentCollapseDrafts");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return somSelf;
}

SOM_Scope ODDraft*  SOMLINK ODDocumentAcquireDraft(ODDocument *somSelf, Environment *ev,
		ODDraftPermissions perms,
		ODDraftID id,
		ODDraft* draft,
		ODPositionCode posCode,
		ODBoolean release)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentAcquireDraft");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODBoolean  SOMLINK ODDocumentExists(ODDocument *somSelf, Environment *ev,
		ODDraftID id,
		ODDraft* draft,
		ODPositionCode posCode)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentAcquireDraft");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODFalse;
}

SOM_Scope ODDraft*  SOMLINK ODDocumentAcquireBaseDraft(ODDocument *somSelf, Environment *ev,
		ODDraftPermissions perms)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentAcquireBaseDraft");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODDraft*  SOMLINK ODDocumentCreateDraft(ODDocument *somSelf, Environment *ev,
		ODDraft* below,
		ODBoolean releaseBelow)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentCreateDraft");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope void  SOMLINK ODDocumentSaveToAPrevDraft(ODDocument *somSelf, Environment *ev,
		ODDraft* from,
		ODDraft* to)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentSaveToAPrevDraft");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

/*
 *  private by convention
 */

SOM_Scope void  SOMLINK ODDocumentSetBaseDraftFromForeignDraft(ODDocument *somSelf, Environment *ev,
		ODDraft* draft)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentSetBaseDraftFromForeignDraft");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
}

SOM_Scope void  SOMLINK ODDocumentInitDocument(ODDocument *somSelf, Environment *ev,
		ODContainer* container,
		ODDocumentID id)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentInitDocument");

	try
        {
	  somSelf->InitRefCntObject(ev);
        }
	catch (ODException _exception)
        {
          ODSetSOMException(ev, _exception);
        }
}

SOM_Scope ODDocument*  SOMLINK ODDocumentReleaseDraft(ODDocument *somSelf, Environment *ev,
		ODDraft* draft)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentReleaseDraft");

	WARNMSG(WARN_INDEX(AMSG_690),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return somSelf;
}

SOM_Scope void  SOMLINK ODDocumentsomUninit(ODDocument *somSelf)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentsomUninit");

    ODDocument_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope void  SOMLINK ODDocumentAcquire(ODDocument *somSelf, Environment *ev)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentAcquire");

    ODDocument_parent_ODRefCntObject_Acquire(somSelf,ev);
}

SOM_Scope void  SOMLINK ODDocumentRelease(ODDocument *somSelf, Environment *ev)
{
    /* ODDocumentData *somThis = ODDocumentGetData(somSelf); */
    ODDocumentMethodDebug("ODDocument","ODDocumentRelease");

    ODDocument_parent_ODRefCntObject_Release(somSelf,ev);
}
