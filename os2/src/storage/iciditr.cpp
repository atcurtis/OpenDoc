/* @(#)Z 1.2 os2/src/storage/iciditr.cpp, odstorage, od96os2, odos29646d 96/11/15 15:50:03 (96/08/23 01:43:09) */
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

#ifdef _PLATFORM_OS2_

#ifndef SOM_Module_iciditr_Source
#define SOM_Module_iciditr_Source
#endif
#define IODConnIDIterator_Class_Source

#define VARIABLE_MACROS

#include <Except.h>
#include <ODTypes.h>

#include <IODDefs.xh>

typedef struct CIDNode
{
    ODLinkConnectionID cid;
    CIDNode*        next;
} CIDNode;

#include "ICIDItr.xih"

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK Add(IODConnIDIterator *somSelf,
            Environment *ev,
            ODLinkConnectionID cid)
{
    IODConnIDIteratorData *somThis = IODConnIDIteratorGetData(somSelf);
    IODConnIDIteratorMethodDebug("IODConnIDIterator","Add");

    CIDNode* entry = new CIDNode;
    THROW_IF_NULL(entry);

    entry->cid = cid;
    if (_fCIDList != (CIDNode*)kODNULL)
    {
      entry->next = _fCIDList;
    }
    else
    {
      entry->next = (CIDNode*)kODNULL;
    }
    _fCIDList = entry;
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODBoolean
SOMLINK IsNotComplete(IODConnIDIterator *somSelf,
                      Environment *ev)
{
    IODConnIDIteratorData *somThis = IODConnIDIteratorGetData(somSelf);
    IODConnIDIteratorMethodDebug("IODConnIDIterator","IsNotComplete");

    if (_fCurrent != (CIDNode*)kODNULL)
    {
       return kODTrue;
    }
    else
    {
       return kODFalse;
    }
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODLinkConnectionID
SOMLINK First(IODConnIDIterator *somSelf,
              Environment *ev)
{
    IODConnIDIteratorData *somThis = IODConnIDIteratorGetData(somSelf);
    IODConnIDIteratorMethodDebug("IODConnIDIterator","First");

    if ((_fCurrent = _fCIDList) != (CIDNode*)kODNULL)
    {
      return _fCurrent->cid;
    }
    else
    {
      return kODNULL;
    }
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope ODLinkConnectionID
SOMLINK Next(IODConnIDIterator *somSelf,
             Environment *ev)
{
    IODConnIDIteratorData *somThis = IODConnIDIteratorGetData(somSelf);
    IODConnIDIteratorMethodDebug("IODConnIDIterator","Next");

    if (_fCurrent != (CIDNode*)kODNULL)
    {
      _fCurrent = _fCurrent->next;
      if (_fCurrent != (CIDNode*)kODNULL)
      {
        return _fCurrent->cid;
      }
    }
    return kODNULL;
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK somInit(IODConnIDIterator *somSelf)
{
    IODConnIDIteratorData *somThis = IODConnIDIteratorGetData(somSelf);
    IODConnIDIteratorMethodDebug("IODConnIDIterator","somInit");

    IODConnIDIterator_parent_SOMObject_somInit(somSelf);

    _fCIDList = (CIDNode*)kODNULL;
    _fCurrent = (CIDNode*)kODNULL;
}

/*
 *=============================================================================
 *=============================================================================
 */

SOM_Scope void
SOMLINK somUninit(IODConnIDIterator *somSelf)
{
    IODConnIDIteratorData *somThis = IODConnIDIteratorGetData(somSelf);
    IODConnIDIteratorMethodDebug("IODConnIDIterator","somUninit");

    while(_fCurrent = _fCIDList)
    {
      _fCIDList = _fCurrent->next;
      _fCurrent->next = (CIDNode*)kODNULL;
      delete _fCurrent;
    }

    IODConnIDIterator_parent_SOMObject_somUninit(somSelf);
}
#endif

