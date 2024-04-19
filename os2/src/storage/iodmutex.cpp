/* @(#)Z 1.4 os2/src/storage/iodmutex.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:23 (96/08/23 01:44:06) */
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
#include <odpagtun.h>
#endif

#ifdef _PLATFORM_OS2_
#ifndef SOM_Module_iodmutex_Source
#define SOM_Module_iodmutex_Source
#endif
#define IODMutex_Class_Source
#define VARIABLE_MACROS
#define METHOD_MACROS

#define INCL_DOSSEMAPHORES
#define INCL_DOSERRORS
#include <itrace.hpp>
#include <os2.h>
#include <itext.h>

#ifndef _ODTYPESM_
#include <ODTypesM.xh>
#endif

#ifndef _EXCEPT_
  #include <Except.h>
#endif

#ifndef SOM_Module_IODDefs_IOpenDoc_Defs_defined
  #include <IODDefs.xh>
#endif

#include "IODMutex.xih"

SOM_Scope void
SOMLINK Acquire(IODMutex *somSelf,
                Environment *ev)
{
    IODMutexData *somThis = IODMutexGetData(somSelf);
    IODMutexMethodDebug("IODMutex","acquire");

    if (ev->_major) ODSetSOMException(ev, kODNoError);    
    APIRET rc = NO_ERROR;
    if (_fMutex != kODNULL)
    {
        rc = DosRequestMutexSem(_fMutex, _fTimeOut);
    }
    else {
      ODSetSOMException(ev, kODErrRequestMutexInvalidHandle);
      return;
    };

    switch (rc)
    {
      case NO_ERROR:
        return;

      case ERROR_TIMEOUT:
        ODSetSOMException(ev, kODErrRequestMutexTimeOut);
        return;

      case ERROR_INTERRUPT:
        ODSetSOMException(ev, kODErrRequestMutexInterrupt);
        return;

      case ERROR_INVALID_HANDLE:
        ODSetSOMException(ev, kODErrRequestMutexInvalidHandle);
        return;

      case ERROR_TOO_MANY_SEM_REQUESTS:
        ODSetSOMException(ev, kODErrRequestMutexTooManyRequests);
        return;

      case ERROR_SEM_OWNER_DIED:
        ODSetSOMException(ev, kODErrRequestMutexOwnerDied);
        return;
    }
}

SOM_Scope void
SOMLINK Release(IODMutex *somSelf,
                Environment *ev)
{
    IODMutexData *somThis = IODMutexGetData(somSelf);
    IODMutexMethodDebug("IODMutex","release");

    if (ev->_major) ODSetSOMException(ev, kODNoError);
    APIRET rc = DosReleaseMutexSem(_fMutex);

    switch (rc)
    {
      case NO_ERROR:
        return;

      case ERROR_INVALID_HANDLE:
        ODSetSOMException(ev, kODErrReleaseMutexInvalidHandle);
        return;

      case ERROR_NOT_OWNER:
        ODSetSOMException(ev, kODErrReleaseMutexNotOwner);
        return;
    }
}

SOM_Scope void
SOMLINK somInit(IODMutex *somSelf)
{
    IODMutexData *somThis = IODMutexGetData(somSelf);
    IODMutexMethodDebug("IODMutex","somInit");

    IODMutex_parent_SOMObject_somInit(somSelf);

    _fMutex = kODNULL;
      APIRET rc = DosCreateMutexSem((PSZ)NULL, &_fMutex, (ULONG)0, FALSE);

    _fTimeOut = 60000;
}

SOM_Scope void
SOMLINK somUninit(IODMutex *somSelf)
{
    IODMutexData *somThis = IODMutexGetData(somSelf);
    IODMutexMethodDebug("IODMutex","somUninit");

    IODMutex_parent_SOMObject_somUninit(somSelf);

    if (_fMutex != kODNULL)
    {
      APIRET rc = DosCloseMutexSem(_fMutex);
      _fMutex = kODNULL;
    }
}

#endif
