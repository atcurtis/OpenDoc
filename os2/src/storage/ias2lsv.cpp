/* @(#)Z 1.3 os2/src/storage/ias2lsv.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:00 (96/08/23 01:43:01) */
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


#ifndef SOM_Module_ias2lsv_Source
#define SOM_Module_ias2lsv_Source
#endif
#define IODAvailServerToLinkService_Class_Source

#include <ISOStr.h>
#include <IODDefs.xh>
#include "IAS2LSv.xih"


SOM_Scope IODLinkTargetToLinkSource*  SOMLINK InternalizeLinkSrc(IODAvailServerToLinkService *somSelf,
                                                                  Environment *ev,
                                                                 ODLinkID linkID)
{
    /* IODAvailServerToLinkServiceData *somThis = IODAvailServerToLinkServiceGetData(somSelf); */
    IODAvailServerToLinkServiceMethodDebug("IODAvailServerToLinkService","InternalizeLinkSrc");

    /* Return statement to be customized: */
    { IODLinkTargetToLinkSource* retVal;  return (retVal); }
}
#endif
