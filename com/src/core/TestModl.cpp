//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odcore
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
/* @(#)Z 1.5 com/src/core/TestModl.cpp, odcore, od96os2, odos29646d 96/11/15 15:49:06 (96/10/30 20:22:05) */

/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitxtm.dll: Development
 */

#ifndef SOM_Module_testmodl_Source
#define SOM_Module_testmodl_Source
#endif
#define ODTestModule_Class_Source

#include "TestModl.xih"
#include "StorageU.xh"
#include "Facet.xh"


SOM_Scope void  SOMLINK somInit(ODTestModule *somSelf)
{
    /* ODTestModuleData *somThis = ODTestModuleGetData(somSelf); */
    ODTestModuleMethodDebug("ODTestModule","somInit");

    ODTestModule_parent_ODTestModuleB_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(ODTestModule *somSelf)
{
    /* ODTestModuleData *somThis = ODTestModuleGetData(somSelf); */
    ODTestModuleMethodDebug("ODTestModule","somUninit");

    ODTestModule_parent_ODTestModuleB_somUninit(somSelf);
}

SOM_Scope void  SOMLINK Init(ODTestModule *somSelf,  Environment *ev, 
                             ODStorageUnit* partSU, ODFacet* PartFacet)
{
    /* ODTestModuleData *somThis = ODTestModuleGetData(somSelf); */
    ODTestModuleMethodDebug("ODTestModule","Init");

    ODTestModule_parent_ODTestModuleB_Init(somSelf, ev, partSU, 
                                           PartFacet);
}

SOM_Scope void  SOMLINK Run(ODTestModule *somSelf,  Environment *ev, 
                            ODStorageUnit* partSU, ODFacet* partFacet)
{
    /* ODTestModuleData *somThis = ODTestModuleGetData(somSelf); */
    ODTestModuleMethodDebug("ODTestModule","Run");

    ODTestModule_parent_ODTestModuleB_Run(somSelf, ev, partSU, 
                                          partFacet);
}

