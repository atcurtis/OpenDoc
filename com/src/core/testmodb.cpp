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
/* @(#)Z 1.5 com/src/core/testmodb.cpp, odcore, od96os2, odos29646d 96/11/15 15:50:00 (96/10/30 20:22:23) */

/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitxtm.dll: Development
 */

#ifndef SOM_Module_testmodb_Source
#define SOM_Module_testmodb_Source
#endif
#define ODTestModuleB_Class_Source

#include "testmodb.xih"
#include "StorageU.xh"
#include "Facet.xh"


SOM_Scope void  SOMLINK Init(ODTestModuleB *somSelf,  Environment *ev, 
                             ODStorageUnit* partSU, ODFacet* PartFacet)
{
    /* ODTestModuleBData *somThis = ODTestModuleBGetData(somSelf); */
    ODTestModuleBMethodDebug("ODTestModuleB","Init");

}

SOM_Scope void  SOMLINK Run(ODTestModuleB *somSelf,  Environment *ev, 
                            ODStorageUnit* partSU, ODFacet* partFacet)
{
    /* ODTestModuleBData *somThis = ODTestModuleBGetData(somSelf); */
    ODTestModuleBMethodDebug("ODTestModuleB","Run");

}

SOM_Scope void  SOMLINK somInit(ODTestModuleB *somSelf)
{
    /* ODTestModuleBData *somThis = ODTestModuleBGetData(somSelf); */
    ODTestModuleBMethodDebug("ODTestModuleB","somInit");

    ODTestModuleB_parent_SOMObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(ODTestModuleB *somSelf)
{
    /* ODTestModuleBData *somThis = ODTestModuleBGetData(somSelf); */
    ODTestModuleBMethodDebug("ODTestModuleB","somUninit");

    ODTestModuleB_parent_SOMObject_somUninit(somSelf);
}

