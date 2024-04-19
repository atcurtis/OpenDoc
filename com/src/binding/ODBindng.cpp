/* @(#)Z 1.6 com/src/binding/ODBindng.cpp, odconfig, od96os2, odos29646d 96/11/15 15:24:43 (96/10/29 09:15:22) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odconfig
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
  File:    ODBindng.cpp

  Contains:  SOM Implementation for Binding class.

  Owned by:  Caia Grisar

  Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

    <11>  04/11/96  CBH    Ported to NT platform
    <10>   5/18/95  CG    #1241621 BB: Need to implement Purge for
                  Binding subsystem objects.
     <9>   5/17/95  VL    1170098: Added private-by-convention
                  ConstructNoPart.
     <8>    5/1/95  CG    #1241628  BB: SOM_TRY blocks needed in
                  Binding SOM methods. #1243669  XMP in
                  ODBindng.cpp ifdef
     <7>   4/26/95  CG    last check in comment should have been :
                  1241628  SOM_TRY blocks needed in Binding
                  SOM methods.
     <6>   4/26/95  CG    1211082 BB: 5$ Bugs need to be evaluated
                  and removed from Core
     <5>  12/13/94  CG    #1206024: Added function prefix to .idl
                  file.
     <4>   8/24/94  CG    #1181622: Updated file ownership.
     <3>   8/17/94  CG    #1181519: The Binding helper object was not
                  getting initialized  in InitBinding.
     <2>    7/1/94  CG    Check for somSelf->IsInitialized in
                  InitBinding.
     <1>   6/20/94  CG    first checked in
  To Do:
*/


#define ODBinding_Class_Source
#define VARIABLE_MACROS
#include <ODBindng.xih>

#ifndef _BINDNGH_
#include "BindngH.h"
#endif


#ifndef SOM_ODSession_xh
#include "ODSessn.xh"
#endif

#ifndef SOM_Module_Apple_defined
#include <NoPart.xh>
#endif

SOM_Scope void  SOMLINK ODBindingInitBinding(ODBinding *somSelf, Environment *ev,
    ODSession* session)
{
    ODBindingData *somThis = ODBindingGetData(somSelf);
    ODBindingMethodDebug("ODBinding","InitBinding");

        SOM_TRY

#ifdef _PLATFORM_MACINTOSH_
if (somSelf->IsInitialized(ev))
  return;
#endif

  somSelf->InitObject(ev);

  _fSession = session;
  _fBinding = new Binding();
  _fBinding->InitBinding(_fSession);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope ODPart*  SOMLINK ODBindingConstructNoPart(ODBinding *somSelf, Environment *ev)
{
    ODPart*  noPart;

    ODBindingData *somThis = ODBindingGetData(somSelf);
    ODBindingMethodDebug("ODBinding","ConstructNoPart");

  SOM_TRY

    noPart = new NoPart;

  SOM_CATCH_ALL
    noPart = kODNULL;
  SOM_ENDTRY

  return noPart;
}

SOM_Scope ODEditor  SOMLINK ODBindingChooseEditorForPart(ODBinding *somSelf, Environment *ev,
    ODStorageUnit* thePartSU,
    ODType newKind)
{
    ODBindingData *somThis = ODBindingGetData(somSelf);
    ODBindingMethodDebug("ODBinding","ChooseEditorForPart");

    ODEditor returnVal = kODNULL;

    SOM_TRY

    returnVal = _fBinding->ChooseEditorForPart(thePartSU, newKind);

    SOM_CATCH_ALL
    SOM_ENDTRY

    return (returnVal);

}

SOM_Scope ODContainerSuite  SOMLINK ODBindingGetContainerSuite(ODBinding *somSelf, Environment *ev,
    ODContainerType containerType)
{
    ODBindingData *somThis = ODBindingGetData(somSelf);
    ODBindingMethodDebug("ODBinding","GetContainerSuite");

    ODContainerSuite returnVal = kODNULL;

    SOM_TRY

    returnVal = _fBinding->GetContainerSuite( containerType );

    SOM_CATCH_ALL
    SOM_ENDTRY

    return (returnVal);
}

SOM_Scope void  SOMLINK ODBindingsomInit(ODBinding *somSelf)
{
    ODBindingData *somThis = ODBindingGetData(somSelf);
    ODBindingMethodDebug("ODBinding","ODBindingsomInit");

    ODBinding_parent_ODObject_somInit(somSelf);

}

SOM_Scope void  SOMLINK ODBindingsomUninit(ODBinding *somSelf)
{
    ODBindingData *somThis = ODBindingGetData(somSelf);
    ODBindingMethodDebug("ODBinding","ODBindingsomUninit");

    ODBinding_parent_ODObject_somUninit(somSelf);

}

SOM_Scope ODSize  SOMLINK ODBindingPurge(ODBinding *somSelf, Environment *ev,
    ODSize size)
{
    ODBindingData *somThis = ODBindingGetData(somSelf);
    ODBindingMethodDebug("ODBinding","ODBindingPurge");

    ODSize returnVal = 0;

    SOM_TRY

      returnVal = _fBinding->Purge(size);

    SOM_CATCH_ALL
    SOM_ENDTRY

      return(returnVal);
}
