/* @(#)Z 1.3 os2/src/storage/ilsv2as.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:23 (96/08/23 01:43:59) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: oddataxfer
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

#ifdef _PLATFORM_OS2_

#ifndef SOM_Module_ilsv2as_Source
#define SOM_Module_ilsv2as_Source
#endif
#define IODLinkServiceToAvailServer_Class_Source

#include <ISOStr.h>
#include <IODDefs.xh>
#include "ILSv2AS.xih"


/*
 * RegisterDoc is called when a new document is opened.
 */

SOM_Scope IODDocumentID  SOMLINK RegisterDoc(IODLinkServiceToAvailServer *somSelf,
                                              Environment *ev,
                                             IODFileName docPathName,
                                             IODAvailServerToLinkService* linkService)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","RegisterDoc");

    /* Return statement to be customized: */
    { IODDocumentID retVal;  return (retVal); }
}

/*
 * UnregisterDoc is called when a document is destroyed.
 */

SOM_Scope void  SOMLINK UnregisterDoc(IODLinkServiceToAvailServer *somSelf,
                                       Environment *ev, IODDocumentID docID)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","UnregisterDoc");

}

/*
 * DocOpened is called when a document already registered with the AS
 * is opened.  If the path name does not match the path name registered
 * with that document id, then an exception is returned and the document
 * is not opened.
 */

SOM_Scope ODBoolean  SOMLINK DocOpened(IODLinkServiceToAvailServer *somSelf,
                                        Environment *ev, IODDocumentID docID,
                                       IODFileName docPathName,
                                       IODAvailServerToLinkService* linkService,
                                       IODFileName* oldPathName)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","DocOpened");

    /* Return statement to be customized: */
    { ODBoolean retVal;  return (retVal); }
}

/*
 * DocClosed is called when an opened document is closed.
 */

SOM_Scope void  SOMLINK DocClosed(IODLinkServiceToAvailServer *somSelf,
                                   Environment *ev, IODDocumentID docID)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","DocClosed");

}

/*
 * DocMoved is called when a registered document's persistent storage
 * changes from oldPathName to newPathName.
 */

SOM_Scope void  SOMLINK DocMoved(IODLinkServiceToAvailServer *somSelf,
                                  Environment *ev, IODDocumentID docID,
                                 IODFileName oldPathName, IODFileName newPathName)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","DocMoved");

}

/*
 * RegisterLink is called when a new LinkSource has been
 * created in the document.  It provides a pointer to the LinkSource so
 * the AS may forward calls to it.  The AS returns a unique (within the
 * AS) ID for this LinkSource.  (LinkOpened need not be called).
 */

SOM_Scope ODLinkID  SOMLINK RegisterLink(IODLinkServiceToAvailServer *somSelf,
                                          Environment *ev, IODDocumentID docID,
                                         IODLinkTargetToLinkSource* linkSrc)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","RegisterLink");

    /* Return statement to be customized: */
    { ODLinkID retVal;  return (retVal); }
}

/*
 * LinkOpened is called when an existing LinkSource is internalized in
 * a document.  It provides a pointer to the LinkSource so
 * the AS may forward calls to it.  The AS returns the current
 * AuxiliaryState associated with this LinkSource, and whether some
 * target currently holds a lock on this LinkSource ContentSU.
 */

SOM_Scope void  SOMLINK LinkOpened(IODLinkServiceToAvailServer *somSelf,
                                    Environment *ev, ODLinkID linkID,
                                   IODLinkTargetToLinkSource* linkSrc,
                                   IODAuxiliaryState** auxState,
                                   ODBoolean* isLocked)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","LinkOpened");

}

/*
 * LinkClosed is called when a LinkSource is released in a document.
 * It provides the current AuxiliaryState for this LinkSource.
 * The AS will now process calls for this LinkSource.
 * (Note that local LinkTargets will not have a reference any longer to
 * the LinkSource object in the document.)
 */

SOM_Scope void  SOMLINK LinkClosed(IODLinkServiceToAvailServer *somSelf,
                                    Environment *ev, ODLinkID linkID,
                                   IODAuxiliaryState* auxState)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","LinkClosed");

}

/*
 * RemoveLink is called when a LinkSource is removed from a document.
 */

SOM_Scope void  SOMLINK RemoveLink(IODLinkServiceToAvailServer *somSelf,
                                    Environment *ev, ODLinkID linkID)
{
    /* IODLinkServiceToAvailServerData *somThis = IODLinkServiceToAvailServerGetData(somSelf); */
    IODLinkServiceToAvailServerMethodDebug("IODLinkServiceToAvailServer","RemoveLink");

}

#endif
