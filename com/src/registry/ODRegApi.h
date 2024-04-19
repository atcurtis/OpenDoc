/* @(#)Z 1.10 com/src/registry/ODRegApi.h, odconfig, od96os2, odos29646d 96/11/15 15:49:29 (96/10/29 09:29:10) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odconfig
 *
 *   CLASSES: none
 *
 *   ORIGINS: 27
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

/********************************************************************/
/*  Licensed Materials - Property of IBM                            */
/*                                                                  */
/*                                                                  */
/* Copyright (C) International Business Machines Corp., 1994.       */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/*  US Government Users Restricted Rights -                         */
/*  Use, duplication, or disclosure restricted                      */
/*  by GSA ADP Schedule Contract with IBM Corp.                     */
/*                                                                  */
/*  File:    ODRegApi.h                                             */
/*                                                                  */
/*  Contains:  OpenDoc registry functions.                          */
/*                                                                  */
/*  Change History                                                  */
/*      Took OS2 code from Feb13 drop and changed it to look like   */
/*      AIX code.  Removed APIRET and PSZ references.               */
/*                                                                  */
/********************************************************************/
#ifndef ODREGAPI_H
#define ODREGAPI_H

#include <somltype.h>

#ifdef _PLATFORM_OS2_
#ifndef __OS2DEF__
#include <os2def.h>
#endif
#endif // OS2

#include <ODTypes.h>

typedef struct 
{
    long      cBytes;
    ISOString partHandlerName;
    char*     partHandlerDisplayName;
    char*     partHandlerClassName;
    char*     partKindList;           
    char*     ole2ClassId;            
    char*     windowsIconFileName;    
    char*     dllName;    
} PartHandlerQueryInfo;

typedef struct 
{
    long      cBytes;
    ISOString partKindName;
    char*     partKindDisplayName;
    char*     filenameFilters;       
    char*     filenameTypes;       
    char*     categories;             
} PartKindQueryInfo;

/*********************************************************************************
 ** ODRegisterPartHandlerClass
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODRegisterPartHandlerClass( ISOString ClassName); 

/*********************************************************************************
 ** ODDeregisterPartHandler
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODDeregisterPartHandler( ISOString isoPartHandlerName);

/*********************************************************************************
 ** ODDeregisterPartHandlerClass
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODDeregisterPartHandlerClass( ISOString ClassName);

/*********************************************************************************
 ** ODQueryPartHandlerList
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartHandlerList( ISOString isoPartKind, 
                               ISOString Buffer, 
                               ODULong pulBufferSize);

/*********************************************************************************
 ** ODQueryPartKindList
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartKindList( ISOString isoPartHandlerName, 
                            ISOString Buffer, 
                            ODULong pulBufferSize);

/*********************************************************************************
 ** ODQueryCategoryList
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryCategoryList( ISOString isoPartHandlerName, 
                            ISOString isoPartKind, 
                            ISOString Buffer, 
                            ODULong pulBufferSize);

/*********************************************************************************
 ** ODQueryPartHandlerInfo
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartHandlerInfo( ISOString isoPartHandlerName,  
                               PartHandlerQueryInfo* partInfo);

/*********************************************************************************
 ** ODQueryPartKindInfo
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartKindInfo( ISOString isoPartHandlerName,  
                               ISOString isoPartKindName,
                               PartKindQueryInfo* partKindQueryInfo);
/*********************************************************************************
 ** ODQueryPreferredPartHandler
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandler( ISOString isoPartKindName, 
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize);

/*********************************************************************************
 ** ODSetPreferredPartHandler
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandler( ISOString isoPartKindName, 
                                  ISOString isoPartHandlerName);

/*********************************************************************************
 ** ODQueryPreferredPartHandlerForFileType
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForFileType( ISOString isoFileType, 
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize);

/*********************************************************************************
 ** ODSetPreferredPartHandlerForFileType
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForFileType( ISOString isoFileType, 
                                  ISOString isoPartHandlerName);

/*********************************************************************************
 ** ODQueryPreferredPartHandlerForFileExt
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForFileExt( ISOString isoFileExt, 
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize);

/*********************************************************************************
 ** ODSetPreferredPartHandlerForFileType
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForFileExt( ISOString isoFileExt, 
                                  ISOString isoPartHandlerName);

/*********************************************************************************
 ** ODQueryPreferredPartHandlerForCategory
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForCategory( ISOString isoCategory, 
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize);

/*********************************************************************************
 ** ODSetPreferredPartHandlerForFileType
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForCategory( ISOString isoCategory, 
                                  ISOString isoPartHandlerName);

/*********************************************************************************
 ** ODQueryKindDisplayName
 **
 **********************************************************************************/
SOMEXTERN   unsigned long  SOMLINK ODQueryKindDisplayName( ISOString partKindName,
                                                                                         ISOString Buffer,
                                                                                         ODULong pulBufferSize);

/*********************************************************************************
 ** ODQueryHandlerDisplayName
 **
 **********************************************************************************/
SOMEXTERN   unsigned long  SOMLINK ODQueryHandlerDisplayName( ISOString HandlerName,
                                                                                              ISOString Buffer,
                                                                                              ODULong pulBufferSize);
#endif // ODREGAPI_H
