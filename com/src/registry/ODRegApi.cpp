/* @(#)Z 1.19 com/src/registry/ODRegApi.cpp, odconfig, od96os2, odos29646d 96/11/15 15:49:22 (96/10/28 13:39:30) */

//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odconfig
//
//   CLASSES: none
//
//   ORIGINS: 27
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
 *   File:      odregapi.cpp
 *
 *   Contains:  Implementation of registry api's
 *
 *   Change History (most recent first):
 *
 *     143600   11/16/95   jso  Fixed error codes
 *     141034   10/25/95   jso  Added Check for NULL several API's           
 *     139875   10/12/95   jso  Fixed potential trap in QueryPartHandlerInfo 
 *     123852   09/28/95   jso  Added Query/Set for Category preferred editor
 *     123247   09/21/95   jso  Added partCategoryDisplayName     
 *     119227   08/02/95   jso  Alloc mem for querypart\kind list 
 *     119025   07/20/95   jso  Fixed QueryPartHandlerInfo to return proper  
 *                              if partHandler not found and to zero buffer.
 *     123904   07/19/95   jso  Initialized return string QueryPartHandlerList  
 *     123473   07/14/95   jso  Added API's for Query/Set for FileTypes and Exts
 *     128470   07/07/95   jso  Added ODDebug ifdef for somLPrintf calls
 *     119036   06/06/95   jso  Fixed QueryPartHandlerInfo
 *     121007   05/31/95   jso  Added Template flag.
 *     120116   05/09/95   jso  Fixed rexx return codes.
 *
 *   To Do:
 *
 *   Copyright: (c) 1993-1994 by IBM Corp., all rights reserved.
 *
 */

#define     INCL_DOSMEMMGR
#include    <ODTypes.h>

#ifdef _PLATFORM_WIN32_
#include <qprintf.h>
#endif

#ifndef _ODTYPESF_
#include <ODTypesF.h>
#endif

#include    <RManager.xh>
#include    <ODPrtRei.xh>
#include    <PartInfi.xh>
#include    <Part.xh>

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
//#include <ODPagtun.h>
#endif

#define     INCL_RXFUNC      /* include external function  info */
#define     INCL_RXSHV
#include    <ODRegApi.h>
#include    <rmutil.h>

#ifndef     _ODDEBUG_
#include    "ODDebug.h"
#endif

#if (defined _PLATFORM_OS2_)
#include    <rexxsaa.h>
#endif

#include    <stdlib.h>
#include    <string.h>

/*********************************************************************/
/* Constants                                                         */
/*********************************************************************/
const ODULong MAX = 256;

/*********************************************************************/
/* Numeric Error Return Strings                                      */
/*********************************************************************/

#define  NO_UTIL_ERROR    "0"          /* No error whatsoever        */
#define  ERROR_NOMEM      "2"          /* Insufficient memory        */
#define  ERROR_FILEOPEN   "3"          /* Error opening text file    */

/*********************************************************************/
/* Alpha Numeric Return Strings                                      */
/*********************************************************************/

#define  ERROR_RETSTR   "ERROR:"

/*********************************************************************/
/* Numeric Return calls                                              */
/*********************************************************************/

#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */

/*********************************************************************/
/* Object pointers used to perform functions for API.                */
/*********************************************************************/

static Environment* ev;
static RegistryManager* rManager = 0;
static ODPartHandlerRegistryInt* odRegistry = 0;

/*********************************************************************/
/* Initialization routine assures object pointer are initialized.    */
/*********************************************************************/

inline void Initialize()
{
    if ( !rManager )
    {
        ev = somGetGlobalEnvironment();
        rManager = new RegistryManager;
        odRegistry = (ODPartHandlerRegistryInt*) rManager-> GetODPartHandlerRegistry( ev);
    }
}

/*****************************************************************************/
/** ODRegisterPartHandlerClass                                               */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODRegisterPartHandlerClass( ISOString ClassName)
{
   unsigned long rc = 0;
   try {
    Initialize();

    somId partsSomId = somIdFromString ((string)ClassName);
    string tmpname = SOMClassMgrObject->somLocateClassFile(partsSomId, 0,0);

/* SOMMalloc will reuse the space pointed by tmpname, so we have to
    SOMMalloc a new space ourselves to keep DLLName                          */

    string DLLName = string( SOMMalloc( strlen(tmpname)+1));
    strcpy( DLLName, tmpname);
    rc =   odRegistry->RegisterPartHandlerClass(ev, ClassName,
						   DLLName, 0, (long) 0);
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */
  return rc;
}

/*****************************************************************************/
/** ODDeregisterPartHandler                                                  */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODDeregisterPartHandler( ISOString isoPartHandlerName)
{
    Initialize();

    unsigned long rc = 
		odRegistry->DeregisterPartHandler(ev, isoPartHandlerName);
    return rc;
}

/*****************************************************************************/
/** ODDeregisterPartHandlerClass                                             */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODDeregisterPartHandlerClass( ISOString ClassName)
{
    Initialize();

    unsigned long rc = 
		odRegistry->DeregisterPartHandlerClass(ev, ClassName);
    return rc;
}

/*****************************************************************************/
/** ODQueryPartHandlerList                                                   */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartHandlerList( ISOString isoPartKind,
                               ISOString Buffer,
                               ODULong pulBufferSize)
{
    long i;
    long cBytes = 0;
    unsigned long rc = RM_NO_ERROR;
    _IDL_SEQUENCE_ISOString partHandlerList;

  try {
    // Make sure that the global pointers to the Reigstry are set.
    Initialize();

    // Retrieve PartHandler list from Registry.
    // This is a buffer that the Registry allocates and we need to dispose of.
    partHandlerList = odRegistry-> GetPartHandlerList( ev, isoPartKind, 0);
  
    // if the list is not empty, get its size
    if ( partHandlerList. _length ) {
          for ( i = 0; i < partHandlerList. _length; i++)
          {
              cBytes += strlen( partHandlerList. _buffer[i]) + 1;
          }
    }
    else { // the list was empty. return error code saying so.
        *(long *)pulBufferSize = 0;
#ifdef DEBUG
        PRINT("ODQueryPartHandlerList: partHandlerList. _length==0\n");
#endif
        rc = errPR_PART_HANDLER_NOT_FOUND;
        return rc;
    }
  
    // if a NULL buffer was passed in -or- the buffer size is too small
    if ( (!Buffer) || (cBytes  > (*(long *) pulBufferSize)) ) 
    {
	// return error code indicating buffer too small and
	// the actual buffer size required in the pulBufferSize parameter
        if (!Buffer) {
#ifdef DEBUG
            PRINT("ODQueryPartHandlerList: BUFFER_TOO_SMALL==0 Buffer == NULL\n");
#endif
        } 
        else {
#ifdef DEBUG
            PRINT("ODQueryPartHandlerList: BUFFER_TOO_SMALL==0 cBytes=%d Size=%ld\n",cBytes,*(long*)pulBufferSize);
#endif
        }
        *(long *)pulBufferSize = cBytes;
        rc = errPR_BUFFER_TOO_SMALL;
        return rc;
    }

    // There is enough space in the buffer. Copy the PartHandler list into it.
    strcpy( Buffer, partHandlerList. _buffer[0]);
  
    // separate the PartHandler by commas
    for ( i = 1; i < partHandlerList. _length; i++)
    {
        strcat( Buffer, ",");
        strcat( Buffer, partHandlerList. _buffer[i]);
    }
  
    for ( i = 0; i < partHandlerList. _length; i++)
    {
       SOMFree( partHandlerList. _buffer[i]);
    }
    SOMFree( partHandlerList. _buffer);
    
    *(long*)pulBufferSize = cBytes;
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */
  return rc;
}

/******************************************************************************/
/** ODQueryPartKindList                                                       */
/**                                                                           */
/******************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartKindList( ISOString isoPartHandlerName,
                            ISOString Buffer,
                            ODULong pulBufferSize)
{
    long i;
    long cBytes = 0;
    unsigned long rc = RM_NO_ERROR;
    _IDL_SEQUENCE_ISOString partKindList;

    // Make sure that the global pointers to the Reigstry are set.
  try {
    Initialize();

    // retrieve PartKind list from Registry
    // this is a buffer that the Registry allocates and we need to dispose of.
    partKindList = odRegistry-> GetPartKindList( ev, isoPartHandlerName, 0);
  
    // if the list is not empty, get its size
    if ( partKindList. _length ) {
          for ( i = 0; i < partKindList. _length; i++)
          {
              cBytes += strlen( partKindList. _buffer[i]) + 1; 
              // Add 1 for comma separator
          }
    }
    else { // the list was empty. return error code saying so.
        *(long *)pulBufferSize = cBytes;
        rc = errPR_PART_KIND_NOT_FOUND;
#ifdef DEBUG
        PRINT("ODQueryPartKindList: PART_KIND_NOT_FOUND==0\n");
#endif
        return rc;
    }
  
    // if a NULL buffer was passed in -or- the buffer size is too small
    if ( (!Buffer) || (cBytes  > (*(long *) pulBufferSize)) ) 
    {
	// return error code indicating buffer too small and
	// the actual buffer size required in the pulBufferSize parameter
        *(long *)pulBufferSize = cBytes;
        rc = errPR_BUFFER_TOO_SMALL;
#ifdef DEBUG
        PRINT("ODQueryPartKindList: BUFFER_TOO_SMALL==0\n");
#endif
        return rc;
    }

    // There is enough space in the buffer. Copy the PartKind list into it.
    strcpy( Buffer, partKindList. _buffer[0]);

    // separate the PartKinds by commas
    for ( i = 1; i < partKindList. _length; i++)
    {
        strcat( Buffer, ",");
        strcat( Buffer, partKindList. _buffer[i]);
    }

    // now free up the buffer we got from the Registry
    for ( i = 0; i < partKindList. _length; i++)
    {
              SOMFree( partKindList. _buffer[i]);
    }
    SOMFree( partKindList. _buffer);

    // return size of PartKind list and successful return code.
    *(long *)pulBufferSize = cBytes;
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */

    return rc;
}

/*********************************************************************************
 ** ODQueryCategoryList
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryCategoryList( ISOString isoPartHandlerName,
                            ISOString isoPartKind,
                            ISOString Buffer,
                            ODULong pulBufferSize)
{
    long i;
    long cBytes = 0;
    unsigned long rc = RM_NO_ERROR;
    _IDL_SEQUENCE_string CategoryList;

    // Make sure that the global pointers to the Reigstry are set.
  try {
    Initialize();

    // retrieve PartKind list from Registry
    // this is a buffer that the Registry allocates and we need to dispose of.
    CategoryList = odRegistry-> GetCategoryList( ev, isoPartHandlerName, isoPartKind);

    // if the list is not empty, get its size
    if ( CategoryList. _length ) {
          for ( i = 0; i < CategoryList. _length; i++)
          {
              cBytes += strlen( CategoryList. _buffer[i]) + 1;
              // Add 1 for comma separator
          }
    }
    else { // the list was empty. return error code saying so.
        *(long *)pulBufferSize = cBytes;
        rc = errPR_PART_KIND_NOT_FOUND;
#ifdef DEBUG
        PRINT("ODQueryCategoryList: PART_KIND_NOT_FOUND==0\n");
#endif
        return rc;
    }

    // if a NULL buffer was passed in -or- the buffer size is too small
    if ( (!Buffer) || (cBytes  > (*(long *) pulBufferSize)) )
    {
        // return error code indicating buffer too small and
        // the actual buffer size required in the pulBufferSize parameter
        *(long *)pulBufferSize = cBytes;
        rc = errPR_BUFFER_TOO_SMALL;
#ifdef DEBUG
        PRINT("ODQueryCategoryList: BUFFER_TOO_SMALL==0\n");
#endif
        return rc;
    }

    // There is enough space in the buffer. Copy the PartKind list into it.
    strcpy( Buffer, CategoryList. _buffer[0]);

    // separate the PartKinds by commas
    for ( i = 1; i < CategoryList. _length; i++)
    {
        strcat( Buffer, ",");
        strcat( Buffer, CategoryList. _buffer[i]);
    }

    // now free up the buffer we got from the Registry
    for ( i = 0; i < CategoryList. _length; i++)
    {
              SOMFree( CategoryList. _buffer[i]);
    }
    SOMFree( CategoryList. _buffer);

    // return size of PartKind list and successful return code.
    *(long *)pulBufferSize = cBytes;
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */

    return rc;
}

/*****************************************************************************/
/** ODQueryPartHandlerInfo                                                   */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartHandlerInfo( ISOString isoPartHandlerName,
                               PartHandlerQueryInfo* partHandlerQueryInfo)
{
    unsigned long rc = RM_NO_ERROR;
    Initialize();
    long cBytes;
    long i;

    string                     partHandlerClassName;
    ISOString                  partHandlerName;
    string                     partHandlerDisplayName;
    _IDL_SEQUENCE_PartKindInfo partKindInfo;
    string                     ole2ClassId;
    string                     windowsIconFileName;
    string                     dllName;

  try {
    ODPartHandlerInfo* partHandlerInfo = 
		odRegistry-> GetPartHandlerInfo( ev, isoPartHandlerName);

    if ( partHandlerInfo )
    {
        partHandlerName        = partHandlerInfo->GetPartHandlerName(ev);
        partHandlerDisplayName = partHandlerInfo->GetPartHandlerDisplayName(ev);
        partHandlerClassName   = partHandlerInfo->GetPartHandlerClassName(ev);
        partKindInfo           = partHandlerInfo->GetPartKindInfo(ev, 0);
        ole2ClassId            = partHandlerInfo->GetOLE2ClassId(ev);
        windowsIconFileName    = partHandlerInfo->GetWindowsIconFileName(ev);
        dllName                = partHandlerInfo->GetDLLName(ev);

        cBytes = sizeof( PartHandlerQueryInfo);
        cBytes += SSTRLEN( partHandlerClassName);
        cBytes += SSTRLEN( partHandlerName);
        cBytes += SSTRLEN( partHandlerDisplayName );

        for ( i = 0; i <  partKindInfo. _length; i++)
        {
            long len = SSTRLEN( partKindInfo. _buffer[i]. partKindName);

            if ( len)
                cBytes += (len+1);    // Add 1 for comma separator
        }

        cBytes += SSTRLEN( ole2ClassId);
        cBytes += SSTRLEN( windowsIconFileName);
        cBytes += SSTRLEN( dllName);

	if ( (partHandlerQueryInfo == NULL) ||
             (cBytes > partHandlerQueryInfo-> cBytes) )
        {
#ifdef DEBUG
            PRINT("ODQueryPartHandlerInfo: BUFFER_TOO_SMALL==0\n");
#endif
            rc = errPR_BUFFER_TOO_SMALL;
        }
        else
        {
            ISOString ptr = (ISOString)(partHandlerQueryInfo->partHandlerName); 
                  // Point to first byte after structure.

            STRUCTCAT( ptr, partHandlerQueryInfo, partHandlerName, 
				partHandlerName);
            STRUCTCAT( ptr, partHandlerQueryInfo, partHandlerDisplayName, 
				partHandlerDisplayName);
            STRUCTCAT( ptr, partHandlerQueryInfo, partHandlerClassName, 
				partHandlerClassName);

            if ( partKindInfo. _length )
            {
                STRUCTCAT( ptr, partHandlerQueryInfo, partKindList, 
				partKindInfo._buffer[0].partKindName);

                for ( i = 1; i <  partKindInfo. _length; i++)
                {
                    ptr--;
                    strcat( ptr, ",");
                    strcat( ptr, partKindInfo. _buffer[i]. partKindName );
                    ptr = ptr + strlen(ptr) + 1;
                }
            }

            STRUCTCAT( ptr, partHandlerQueryInfo, ole2ClassId         , 
				ole2ClassId);
            STRUCTCAT( ptr, partHandlerQueryInfo, windowsIconFileName , 
				windowsIconFileName);
            STRUCTCAT( ptr, partHandlerQueryInfo, dllName             , 
				dllName);
        }

        SOMFree( partHandlerClassName);
        SOMFree( partHandlerName);
        SOMFree( partHandlerDisplayName);
        for ( i = 0; i <  partKindInfo. _length; i++)
        {
            SOMFree( partKindInfo. _buffer[i]. partKindName);
            SOMFree( partKindInfo. _buffer[i]. partKindDisplayName);
            SOMFree( partKindInfo. _buffer[i]. filenameFilters);
            SOMFree( partKindInfo. _buffer[i]. filenameTypes);
            SOMFree( partKindInfo. _buffer[i]. categories);
            SOMFree( partKindInfo. _buffer[i]. categoryDisplayName);
            SOMFree( partKindInfo. _buffer[i]. objectID);
        }
        SOMFree( partKindInfo. _buffer);
        SOMFree( ole2ClassId);
        SOMFree( windowsIconFileName);
        SOMFree( dllName);
        SOMFree( partHandlerInfo);

	if (partHandlerQueryInfo) partHandlerQueryInfo-> cBytes = cBytes;
    }
    else {
#ifdef DEBUG
        PRINT("ODQueryPartHandlerInfo: PART_HANDLER_NOT_FOUND==0\n");
#endif
        rc = errPR_PART_HANDLER_NOT_FOUND;
    }
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */

  return rc;
}

/*****************************************************************************/
/** ODQueryPartKindInfo                                                      */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPartKindInfo( ISOString isoPartHandlerName,  
                                   ISOString isoPartKindName,
                                   PartKindQueryInfo* partKindQueryInfo)
{
    unsigned long rc = RM_NO_ERROR;
    Initialize();
    long cBytes = 0;
    long i;

    string                     partHandlerClassName;
    ISOString                  partHandlerName;
    string                     partHandlerDisplayName;
    _IDL_SEQUENCE_PartKindInfo partKindInfo;
    string                     ole2ClassId;
    string                     windowsIconFileName;
    PartKindInfo*              foundKind = 0;

  try {
    // Get info record for this part from registry. 
    // We must free this buffer later.
    ODPartHandlerInfo* partInfo = odRegistry-> 
		GetPartHandlerInfo( ev, isoPartHandlerName);

    // If part info was found in registry
    if ( partInfo )
    {
    	// Get part kinds that this part supports.
	// We must free this buffer ourselves before returning.
        partKindInfo = partInfo-> GetPartKindInfo( ev, 0);

	// find the part kind that matches the one passed in (isoPartKindName).
        for ( int i = 0; i <  partKindInfo. _length; i++)
        {
            if ( !strcmp( isoPartKindName, 
			  partKindInfo. _buffer[i]. partKindName) )
            {
                  foundKind = &partKindInfo. _buffer[i];
                  break;
            }
        }

	// if we found a match.
        if ( foundKind )
        {
	    // sum the size of the record (cBytes).
            cBytes = sizeof( PartKindQueryInfo);
            cBytes += SSTRLEN( foundKind-> partKindName);
            cBytes += SSTRLEN( foundKind-> partKindDisplayName);
            cBytes += SSTRLEN( foundKind-> filenameFilters);
            cBytes += SSTRLEN( foundKind-> filenameTypes);
            cBytes += SSTRLEN( foundKind-> categories);

	    // can we return the record?
    	    if ( (partKindQueryInfo == NULL) ||
                 ( cBytes > partKindQueryInfo-> cBytes ) ) 
            {
                rc = errPR_BUFFER_TOO_SMALL;
            }
            else
            {
		// Point to first byte after structure.
                ISOString ptr = (ISOString)(partKindQueryInfo->partKindName); 

		// concat strings from foundKind into partKindQueryInfo
                STRUCTCAT( ptr, partKindQueryInfo, partKindName, 
				foundKind->partKindName);
                STRUCTCAT( ptr, partKindQueryInfo, partKindDisplayName, 
				foundKind->partKindDisplayName);
                STRUCTCAT( ptr, partKindQueryInfo, filenameFilters, 
				foundKind->filenameFilters);
                STRUCTCAT( ptr, partKindQueryInfo, filenameTypes, 
				foundKind->filenameTypes);
                STRUCTCAT( ptr, partKindQueryInfo, categories, 
				foundKind->categories);
            }

	    // return the size of the record
	    if (partKindQueryInfo) partKindQueryInfo-> cBytes = cBytes;
        }
        else
        {
            rc = errPR_PART_KIND_NOT_FOUND;
        }

	// free up buffers received from the registry.
        for ( i = 0; i <  partKindInfo. _length; i++)
        {
            SOMFree( partKindInfo. _buffer[i]. partKindName);
            SOMFree( partKindInfo. _buffer[i]. partKindDisplayName);
            SOMFree( partKindInfo. _buffer[i]. filenameFilters);
            SOMFree( partKindInfo. _buffer[i]. filenameTypes);
            SOMFree( partKindInfo. _buffer[i]. categories);
            SOMFree( partKindInfo. _buffer[i]. categoryDisplayName);
            SOMFree( partKindInfo. _buffer[i]. objectID);
        }
        SOMFree( partKindInfo. _buffer);
        SOMFree( partInfo);
    }
    else
    {
#ifdef DEBUG
        PRINT("ODQueryPartKindInfo: PART_KIND_NOT_FOUND==0\n");
#endif
        rc = errPR_PART_KIND_NOT_FOUND;
    }
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */

  return rc;
}

/******************************************************************************/
/** ODQueryPreferredPartHandler                                               */
/**                                                                           */
/******************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandler( ISOString isoPartKindName,
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize)
{
    unsigned long rc = RM_NO_ERROR;
    Initialize();
    string partHandlerName;
    long cBytes = 0;

    if(isoPartHandlerName == NULL) // 141034
       return errPR_BUFFER_TOO_SMALL;

  try {
    partHandlerName = odRegistry-> GetPreferredPartHandler( ev, isoPartKindName);

    if ( partHandlerName)
    {
        cBytes = strlen( partHandlerName) + 1;

        if ( cBytes > (pulBufferSize))
        {
            rc = errPR_BUFFER_TOO_SMALL;
        }
        else
        {
            strcpy( isoPartHandlerName, partHandlerName);
        }
    }
    else
    {
#ifdef DEBUG
        PRINT("ODQueryPreferredPartHandler: PART_KIND_NOT_FOUND==0\n");
#endif
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    *(long*)pulBufferSize = cBytes;

    SOMFree( partHandlerName);
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */


    return rc;
}

/******************************************************************************/
/** ODSetPreferredPartHandler                                                 */
/**                                                                           */
/******************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandler( ISOString isoPartKindName,
                                         ISOString isoPartHandlerName)
{
    Initialize();

    return odRegistry-> SetPreferredPartHandler( ev, isoPartKindName, isoPartHandlerName);
}

/******************************************************************************/
/** ODQueryPreferredPartHandlerForFileType                                    */
/**                                                                           */
/******************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForFileType( ISOString isoFileType,
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize)
{
    unsigned long rc = RM_NO_ERROR;
    Initialize();
    string partHandlerName;
    long cBytes = 0;

    if(isoPartHandlerName == NULL) // 141034
       return errPR_BUFFER_TOO_SMALL;

  try {
    partHandlerName = odRegistry-> GetPreferredPartHandlerForFileType( ev, isoFileType);

    if ( partHandlerName)
    {
        cBytes = strlen( partHandlerName) + 1;

        if ( cBytes > (pulBufferSize))
        {
#ifdef DEBUG
            PRINT("ODQueryPreferredPartHandlerForFileType: BUFFER_TOO_SMALL==0\n");
#endif
            rc = errPR_BUFFER_TOO_SMALL;
        }
        else
        {
            strcpy( isoPartHandlerName, partHandlerName);
        }
    }
    else
    {
#ifdef DEBUG
        PRINT("ODQueryPreferredPartHandlerForFileType: PART_KIND_NOT_FOUND==0\n");
#endif
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    pulBufferSize = cBytes;

    SOMFree( partHandlerName);
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */


    return rc;
}

/*****************************************************************************/
/** ODSetPreferredPartHandlerForFileType                                     */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForFileType( ISOString isoFileType,
                                           ISOString isoPartHandlerName)
{
    Initialize();

    return odRegistry-> SetPreferredPartHandlerForFileType( ev, isoFileType, isoPartHandlerName);
}

/******************************************************************************/
/** ODQueryPreferredPartHandlerForFileExt                                     */
/**                                                                           */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForFileExt( ISOString isoFileExt,
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize)
{
    unsigned long rc = RM_NO_ERROR;
    Initialize();
    string partHandlerName;
    long cBytes = 0;

    if(isoPartHandlerName == NULL) // 141034
       return errPR_BUFFER_TOO_SMALL;

  try {
    partHandlerName = odRegistry-> GetPreferredPartHandlerForFileExt( ev, isoFileExt);

    if ( partHandlerName)
    {
        cBytes = strlen( partHandlerName) + 1;

        if ( cBytes > (pulBufferSize))
        {
            rc = errPR_BUFFER_TOO_SMALL;
        }
        else
        {
            strcpy( isoPartHandlerName, partHandlerName);
        }
    }
    else
    {
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    pulBufferSize = cBytes;

    SOMFree( partHandlerName);
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */


    return rc;
}

/******************************************************************************/
/** ODSetPreferredPartHandlerForFileExt                                       */
/**                                                                           */
/******************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForFileExt( ISOString isoFileExt,
                                           ISOString isoPartHandlerName)
{
    Initialize();

    return odRegistry-> SetPreferredPartHandlerForFileExt( ev, isoFileExt, isoPartHandlerName);
}

/******************************************************************************/
/** ODQueryPreferredPartHandlerForCategory                                    */
/**                                                                           */
/******************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForCategory( ISOString isoCategory,
                                    ISOString isoPartHandlerName,
                                    ODULong pulBufferSize)
{
    unsigned long rc = RM_NO_ERROR;
    Initialize();
    string partHandlerName;
    long cBytes = 0;

    if(isoPartHandlerName == NULL) // 141034
       return errPR_BUFFER_TOO_SMALL;

  try {
    partHandlerName = odRegistry-> GetPreferredPartHandlerForCategory( ev, isoCategory);

    if ( partHandlerName)
    {
        cBytes = strlen( partHandlerName) + 1;

        if ( cBytes > (pulBufferSize))
        {
            rc = errPR_BUFFER_TOO_SMALL;
        }
        else
        {
            strcpy( isoPartHandlerName, partHandlerName);
        }
    }
    else
    {
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    pulBufferSize = cBytes;

    SOMFree( partHandlerName);
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */

    return rc;
}

/*****************************************************************************/
/** ODSetPreferredPartHandlerForCategory                                     */
/**                                                                          */
/*****************************************************************************/
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForCategory( ISOString isoCategory,
                                           ISOString isoPartHandlerName)
{
    Initialize();

    return odRegistry-> SetPreferredPartHandlerForFileExt( ev, isoCategory, isoPartHandlerName);
}


/*********************************************************************************
 ** ODQueryKindDisplayName
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryKindDisplayName( ISOString partKindName,
                                                                                      ISOString Buffer,
                                                                                      ODULong pulBufferSize)
{
    long cBytes = 0;
    unsigned long rc = RM_NO_ERROR;
    ISOString               displayname;

  try {
    // Make sure that the global pointers to the Reigstry are set.
    Initialize();
    displayname = odRegistry->GetKindDisplayNameFromKind(ev,partKindName);
    cBytes = strlen(displayname ) + 1;
    // if a NULL buffer was passed in -or- the buffer size is too small
    if ( (!Buffer) || (cBytes  > (*(long *) pulBufferSize)) )
    {
        // return error code indicating buffer too small and
        // the actual buffer size required in the pulBufferSize parameter
        *(long *)pulBufferSize = cBytes;
        rc = errPR_BUFFER_TOO_SMALL;
#ifdef DEBUG
        PRINT("ODQueryCategoryList: BUFFER_TOO_SMALL==0\n");
#endif
        return rc;
    }

    // There is enough space in the buffer. Copy the PartKind list into it.
    strcpy( Buffer, displayname);
    SOMFree( displayname);

    // return size of PartKind list and successful return code.
    *(long *)pulBufferSize = cBytes;
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */
  return rc;
}

/*********************************************************************************
 ** ODQueryHandlerDisplayName
 **
 **********************************************************************************/
SOMEXTERN unsigned long SOMLINK ODQueryHandlerDisplayName( ISOString HandlerName,
                                                                                           ISOString Buffer,
                                                                                           ODULong pulBufferSize)
{
    long cBytes = 0;
    unsigned long rc = RM_NO_ERROR;
    _IDL_SEQUENCE_string CategoryList;
    ISOString               displayname;

  try {
    // Make sure that the global pointers to the Reigstry are set.
    Initialize();
    displayname = odRegistry->GetHandlerDisplayNameFromHandler(ev,HandlerName);
    cBytes = strlen(displayname ) + 1;
    // if a NULL buffer was passed in -or- the buffer size is too small
    if ( (!Buffer) || (cBytes  > (*(long *) pulBufferSize)) )
    {
        // return error code indicating buffer too small and
        // the actual buffer size required in the pulBufferSize parameter
        *(long *)pulBufferSize = cBytes;
        rc = errPR_BUFFER_TOO_SMALL;
#ifdef DEBUG
        PRINT("ODQueryCategoryList: BUFFER_TOO_SMALL==0\n");
#endif
        return rc;
    }

    // There is enough space in the buffer. Copy the PartKind list into it.
    strcpy( Buffer, displayname);
    SOMFree( displayname);

    // return size of PartKind list and successful return code.
    *(long *)pulBufferSize = cBytes;
  } // try
  catch (ODException _exception  ) {
     SetErrorCode(kODNoError);
  } /* end catch */
  return rc;
}
/*############################################################################*/
/*############################################################################*/
/*############################################################################*/

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_      
//   REXX SCRIPT INTERFACE
struct FuncNames
{
    PSZ cFunc;
    PSZ rxFunc;
};

static FuncNames funcTable[] =
{
   {"RxODRegisterPartHandlerClass",  "ODRegisterPartHandlerClass"  },
   {"RxODDeregisterPartHandlerClass","ODDeregisterPartHandlerClass"},
   {"RxODDeregisterPartHandler",     "ODDeregisterPartHandler"     },
   {"RxODQueryPartHandlerList",      "ODQueryPartHandlerList"      },
   {"RxODQueryPartKindList",         "ODQueryPartKindList"         },
   {"RxODQueryPartHandlerInfo",      "ODQueryPartHandlerInfo"      },
   {"RxODQueryPreferredPartHandler", "ODQueryPreferredPartHandler" },
   {"RxODSetPreferredPartHandler",   "ODSetPreferredPartHandler"   },
   {"RxODQueryPreferredPartHandlerForFileType", "ODQueryPreferredPartHandlerForFileType" },
   {"RxODSetPreferredPartHandlerForFileType",   "ODSetPreferredPartHandlerForFileType"   },
   {"RxODQueryPreferredPartHandlerForFileExt", "ODQueryPreferredPartHandlerForFileExt" },
   {"RxODSetPreferredPartHandlerForFileExt",   "ODSetPreferredPartHandlerForFileExt"   },
   {"RxODUnloadOpenDocFuncs",        "ODUnloadOpenDocFuncs"       }
};

/**********************************************************************************/
/**  WriteStringSeqToStem                                                         */
/**                                                                               */
/**********************************************************************************/
ULONG WriteStringSeqToStem( PSZ stem, _IDL_SEQUENCE_string& seq)
{
    SHVBLOCK   shvb;                     /* Request block for RxVar    */
    ULONG      stemlen;
    PSZ        stemname;
    ULONG      varlen;
    PSZ        varname;
    ULONG      valulen;
    CHAR       valubuf[MAX];
    ULONG      count;
    ULONG      rc = 0;

    stemlen = strlen( stem);
    stemname = new char[ stemlen+1];
    strcpy( stemname, stem);
    strupr( stemname);
    strtok( stemname, ".");

    varname = new char[stemlen+8];       // Leave room for . and ### following stem name.
    sprintf( varname, "%s.0", stemname);
    varlen = strlen( varname);

    sprintf( valubuf, "%d", seq. _length);  // Set stem.0 to be the count
    valulen = strlen(valubuf);

    shvb.shvnext            = NULL;
    shvb.shvname.strptr     = varname;
    shvb.shvname.strlength  = varlen;
    shvb.shvvalue.strptr    = valubuf;
    shvb.shvvalue.strlength = valulen;
    shvb.shvnamelen         = varlen;
    shvb.shvvaluelen        = valulen;
    shvb.shvcode            = RXSHV_SET;
    shvb.shvret             = 0;

    rc  =  RexxVariablePool(&shvb);

    for ( count = 0; count < seq. _length && (rc != RXSHV_BADN); count++)
    {
        valulen = SSTRLEN( seq. _buffer[count]);
        sprintf(varname, "%s.%d", stemname, count+1);
        varlen                  = strlen( varname);

        shvb.shvnext            = NULL;
        shvb.shvname.strptr     = varname;
        shvb.shvname.strlength  = varlen;
        shvb.shvvalue.strptr    = seq. _buffer[count];
        shvb.shvvalue.strlength = valulen;
        shvb.shvnamelen         = varlen;
        shvb.shvvaluelen        = valulen;
        shvb.shvcode            = RXSHV_SET;
        shvb.shvret             = 0;

        rc  =  RexxVariablePool(&shvb);
    }

    delete     varname;
    delete     stemname;

    if (rc == RXSHV_BADN)
        return INVALID_ROUTINE;    /* error on non-zero          */
    else
        return 0;

}

/**********************************************************************************/
/**  WriteStringToVar                                                             */
/**                                                                               */
/**********************************************************************************/

ULONG WriteStringToVar( PSZ varname, string str)
{
    SHVBLOCK   shvb;                     /* Request block for RxVar    */
    ULONG      rc = 0;

    shvb.shvnext            = NULL;
    shvb.shvname.strptr     = varname;
    shvb.shvname.strlength  = SSTRLEN( varname);
    shvb.shvvalue.strptr    = str;
    shvb.shvvalue.strlength = SSTRLEN( str);
    shvb.shvnamelen         = shvb.shvname.strlength;
    shvb.shvvaluelen        = shvb.shvvalue.strlength;
    shvb.shvcode            = RXSHV_SET;
    shvb.shvret             = 0;

    rc  =  RexxVariablePool(&shvb);

    if (rc == RXSHV_BADN)
        return INVALID_ROUTINE;    /* error on non-zero          */
    else
        return 0;

}

/**********************************************************************************/
/**  ODLoadOpenDocFuncs                                                           */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY ODLoadOpenDocFuncs( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;

    ret-> strlength = 0;

    if ( argc == 0)
    {
        long entries = sizeof(funcTable)/sizeof(FuncNames);

        for ( long i = 0; i < entries; i++)
        {
            ULONG rc = RexxRegisterFunctionDll( funcTable[i].rxFunc, "OPENDOC", funcTable[i].cFunc);
#ifdef DEBUG
            PRINT("loading %s, RC = %d\n", funcTable[i].rxFunc, rc);
#endif
        }
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  RxODUnloadOpenDocFuncs                                                           */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODUnloadOpenDocFuncs( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;

    ret-> strlength = 0;

    if ( argc == 0)
    {
        long entries = sizeof(funcTable)/sizeof(FuncNames);

        for ( long i = 0; i < entries; i++)
        {
            ULONG rc = RexxDeregisterFunction( funcTable[i].rxFunc);
#ifdef DEBUG
            PRINT("unloading %s, RC = %d\n", funcTable[i].rxFunc, rc);
#endif
        }
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  ODRegisterPartHandlerClass                                                   */
/**                                                                               */
/**  PARM    OPTIONAL    DESCRIPTION                                              */
/**   0        NO        partHandlerName                                          */
/**   1        YES       DLLName (defaults to be same as class name)              */
/**   2        YES       cTemplate( create template or not)                       */
/**   3        YES       reserved                                                 */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODRegisterPartHandlerClass( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;

    ret-> strlength = 0;

    if ( argc == 4)
    {
        BOOL cTemplate = (!strcmp(argv[2]. strptr, "FALSE") || !strcmp(argv[2]. strptr,
                              "false")) ? 0 : 1;
        long reserved = strtol( argv[3]. strptr, 0, 0);

        apiret = ODRegisterPartHandlerClass( argv[0]. strptr,
                                             argv[1]. strptr,
                                             cTemplate, 
                                             reserved ); 
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else if(argc == 3)
    {
        BOOL cTemplate = (!strcmp(argv[2]. strptr, "FALSE") || !strcmp(argv[2]. strptr,
                              "false")) ? 0 : 1;

        apiret = ODRegisterPartHandlerClass( argv[0]. strptr,
                                             argv[1]. strptr,
                                             cTemplate, 
                                             0 ); 
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else if(argc == 2)
    {
        apiret = ODRegisterPartHandlerClass( argv[0]. strptr,
                                             argv[1]. strptr,
                                             TRUE, 
                                             0 ); 
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else if(argc == 1)
    {
        apiret = ODRegisterPartHandlerClass( argv[0]. strptr,
                                             argv[0]. strptr,
                                             TRUE, 
                                             0 ); 
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  ODDeregisterPartHandler                                                      */
/**                                                                               */
/**  PARM    OPTIONAL    DESCRIPTION                                              */
/**   0        NO        partHandlerName                                          */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODDeregisterPartHandler( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;

    ret-> strlength = 0;

    if ( argc == 1)
    {
        apiret = ODDeregisterPartHandler( argv[0]. strptr);
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  ODDeregisterPartHandler                                                      */
/**                                                                               */
/**  PARM    OPTIONAL    DESCRIPTION                                              */
/**   0        NO        partHandlerClassName                                     */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODDeregisterPartHandlerClass( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;

    ret-> strlength = 0;

    if ( argc == 1)
    {
        apiret = ODDeregisterPartHandlerClass( argv[0]. strptr);
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}



/**********************************************************************************/
/**  ODQueryPartHandlerList                                                       */
/**                                                                               */
/**  PARM    OPTIONAL    DESCRIPTION                                              */
/**   0        NO        stemVariableName                                         */
/**   1        YES       partKindName                                             */
/**   2        YES       category                                                 */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPartHandlerList( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;
    _IDL_SEQUENCE_string partHandlerList;
    ISOString partKindName = 0;  // Initialize to null as default (if optional parm is not passed)
    string    category     = 0;  // Initialize to null as default (if optional parm is not passed)

    ret-> strlength = 0;

    switch ( argc )
    {
      case 3:
        if ( RXSTRLEN( argv[2]) )
            category = argv[2]. strptr;
      case 2:
        if ( RXSTRLEN( argv[1]) )
            partKindName = argv[1]. strptr;
      case 1:
        Initialize();
        partHandlerList = odRegistry-> GetPartHandlerList( ev, partKindName, category);

        if ( partHandlerList. _length )
        {
            rc = WriteStringSeqToStem( argv[0]. strptr, partHandlerList);

            for ( long i = 0; i < partHandlerList. _length; i++)
            {
                SOMFree( partHandlerList. _buffer[i]);
            }
            SOMFree( partHandlerList. _buffer);
        }

        if(rc == 0)
        {
        sprintf( ret-> strptr, "%d", apiret);     /* 120116 */
        ret-> strlength = strlen( ret-> strptr);  /* 120116 */
        }
        else
          rc = -1;

        break;
      default:
      rc = -1;
    }

    return rc;
}

/**********************************************************************************/
/**  ODQueryPartKindList                                                          */
/**                                                                               */
/**  PARM    OPTIONAL    DESCRIPTION                                              */
/**   0        NO        stemVariableName                                         */
/**   1        YES       partHandlerName                                          */
/**   2        YES       category                                                 */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPartKindList( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;
    _IDL_SEQUENCE_string partKindList;
    ISOString partHandlerName = 0;  // Initialize to null as default (if optional parm is not passed)
    string    category     = 0;     // Initialize to null as default (if optional parm is not passed)

    ret-> strlength = 0;

    switch ( argc )
    {
      case 3:
        if ( RXSTRLEN( argv[2]) )
          category = argv[2]. strptr;
      case 2:
        if ( RXSTRLEN( argv[1]) )
          partHandlerName = argv[1]. strptr;
      case 1:
        Initialize();
        partKindList = odRegistry-> GetPartKindList( ev, partHandlerName, category);

        if ( partKindList. _length )
        {
            rc = WriteStringSeqToStem( argv[0]. strptr, partKindList);

            for ( long i = 0; i < partKindList. _length; i++)
            {
                SOMFree( partKindList. _buffer[i]);
            }
            SOMFree( partKindList. _buffer);

        if(rc == 0)
        {
        sprintf( ret-> strptr, "%d", apiret);     /* 120116 */
        ret-> strlength = strlen( ret-> strptr);  /* 120116 */
        }
        else
          rc = -1;
        }
        break;
      default:
      rc = -1;
    }

    return rc;
}

/**********************************************************************************/
/**  ODQueryPartHandlerInfo                                                       */
/**                                                                               */
/**  PARM    OPTIONAL    DESCRIPTION                       VALUE/VARIABLE         */
/**   0        NO        partHandlerName                      VALUE               */
/**   1        YES       partHandlerDisplayName              VARIABLE             */
/**   2        YES       partHandlerClassName                VARIABLE             */
/**   3        YES       partKindNames                       VARIABLE             */
/**   4        YES       ole2ClassId                         VARIABLE             */
/**   5        YES       windowsIconFileName                 VARIABLE             */
/**   6        YES       dllName                             VARIABLE             */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPartHandlerInfo( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    long rc = 0;
    APIRET apiret = RM_NO_ERROR;
    Initialize();
    long i;

    string                     partHandlerClassName;
    ISOString                  partHandlerName;
    string                     partHandlerDisplayName;
    _IDL_SEQUENCE_PartKindInfo partKindInfo;
    string                     ole2ClassId;
    string                     windowsIconFileName;
    PSZ                        ptr;

    if ( argc > 0 )
    {

        ODPartHandlerInfo* partHandlerInfo = odRegistry-> GetPartHandlerInfo( ev, argv[0]. strptr);

        if ( partHandlerInfo )
        {
            switch ( argc )
            {
              case 7:
                if ( RXSTRLEN( argv[6]) )
                {
                    ptr = partHandlerInfo-> GetDLLName( ev);
                    if ( WriteStringToVar( argv[6]. strptr, ptr) == RXSHV_BADN)
                    {
                        rc = INVALID_ROUTINE;    /* error on non-zero          */
                    }
                    SOMFree( ptr);
                }
              case 6:
                if ( RXSTRLEN( argv[5]) )
                {
                    ptr = partHandlerInfo-> GetWindowsIconFileName( ev);
                    if ( WriteStringToVar( argv[5]. strptr, ptr) == RXSHV_BADN)
                    {
                        rc = INVALID_ROUTINE;    /* error on non-zero          */
                    }
                    SOMFree( ptr);
                }
              case 5:
                if ( RXSTRLEN( argv[4]) )
                {
                    ptr = partHandlerInfo-> GetOLE2ClassId( ev);
                    if ( WriteStringToVar( argv[4]. strptr, ptr) == RXSHV_BADN)
                    {
                        rc = INVALID_ROUTINE;    /* error on non-zero          */
                    }
                    SOMFree( ptr);
                }
              case 4:
                if ( RXSTRLEN( argv[3]) )
                {
                    long cBytes = 0;
                    partKindInfo = partHandlerInfo-> GetPartKindInfo( ev, 0);

                    for ( i = 0; i <  partKindInfo. _length; i++)
                    {
                        long len = SSTRLEN( partKindInfo. _buffer[i]. partKindName);

                        if ( len)
                            cBytes += (len+1);    // Add 1 for comma separator
                    }

                    string partKindNames = (string)SOMMalloc( cBytes);

                    if ( partKindInfo. _length )
                    {
                        strcpy( partKindNames, partKindInfo._buffer[0].partKindName);
                        SOMFree( partKindInfo. _buffer[0]. partKindName);

                        for ( i = 1; i <  partKindInfo. _length; i++)
                        {
                            strcat( ptr, ",");
                            strcat( ptr, partKindInfo. _buffer[i]. partKindName );
                            SOMFree( partKindInfo. _buffer[i]. partKindName);
                        }
                       SOMFree( partKindInfo. _buffer);
                    }

                    if ( WriteStringToVar( argv[3]. strptr, partKindNames) == RXSHV_BADN)
                    {
                        rc = INVALID_ROUTINE;    /* error on non-zero          */
                    }
                }
              case 3:
                if ( RXSTRLEN( argv[2]) )
                {
                    ptr = partHandlerInfo-> GetPartHandlerClassName( ev);
                    if ( WriteStringToVar( argv[2]. strptr, ptr) == RXSHV_BADN)
                    {
                        rc = INVALID_ROUTINE;    /* error on non-zero          */
                    }
                    SOMFree( ptr);
                }
              case 2:
                if ( RXSTRLEN( argv[1]) )
                {
                    ptr = partHandlerInfo-> GetPartHandlerDisplayName( ev);
                    if ( WriteStringToVar( argv[1]. strptr, ptr) == RXSHV_BADN)
                    {
                        rc = INVALID_ROUTINE;    /* error on non-zero          */
                    }
                    SOMFree( ptr);
                }
                break;
              default:
                rc = INVALID_ROUTINE;    /* error on non-zero          */
            }
        }
        else
        {
            apiret = PR_PART_HANDLER_NOT_FOUND;
        }
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = INVALID_ROUTINE;
    }

    return rc;
}

/**********************************************************************************/
/**  ODQueryPartKindInfo                                                          */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPartKindInfo( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = RM_NO_ERROR;
    PartKindInfo kindInfo;

    _IDL_SEQUENCE_string partKindList;
    ISOString partHandlerName = 0;
    ISOString partKindName    = 0;
    string    category        = 0;
    string    filenameFilters = 0;
    string    filenameTypes = 0;
    string    partKindDisplayName = 0;
    string    objectID = 0;
    string    categoryDisplayName = 0;

    ret-> strlength = 0;

    switch ( argc )
    {
      case 8:
        if ( RXSTRLEN( argv[7]) )
          objectID = argv[7]. strptr;
      case 7:
        if ( RXSTRLEN( argv[6]) )
          categoryDisplayName = argv[6]. strptr;
      case 6:
        if ( RXSTRLEN( argv[5]) )
          category = argv[5]. strptr;
      case 5:
        if ( RXSTRLEN( argv[4]) )
          filenameTypes = argv[4]. strptr;
      case 4:
        if ( RXSTRLEN( argv[3]) )
          filenameFilters = argv[3]. strptr;
      case 3:
        if ( RXSTRLEN( argv[2]) )
          partKindDisplayName = argv[2]. strptr;
      case 2:
        if ( RXSTRLEN( argv[1]) )
          partHandlerName = argv[1]. strptr;
      case 1:
        Initialize();
        partKindList = odRegistry-> GetPartKindList( ev, partHandlerName, category);

        if ( partKindList. _length )
        {
            rc = WriteStringSeqToStem( argv[0]. strptr, partKindList);

            for ( long i = 0; i < partKindList. _length; i++)
            {
                SOMFree( partKindList. _buffer[i]);
            }
            SOMFree( partKindList. _buffer);
        }

        if(rc == 0)
        {
        sprintf( ret-> strptr, "%d", apiret);     /* 120116 */
        ret-> strlength = strlen( ret-> strptr);  /* 120116 */
        }
        else
          rc = -1;

        break;
      default:
        rc = -1;
    }

    return rc;
}

/**********************************************************************************/
/**  ODQueryPreferredPartHandler                                                  */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPreferredPartHandler  ( PUCHAR func,
                            ULONG argc,
                            PRXSTRING argv,
                            PSZ que,
                            PRXSTRING ret)
{
    ULONG      rc     = 0;
    APIRET     apiret = RM_NO_ERROR;
    ISOString  partHandlerName;
    LONG       length = 0;
    SHVBLOCK shvb;                     /* Request block for RxVar    */

    ret-> strlength = 0;

    if ( argc == 2)
    {
        Initialize();

        partHandlerName = odRegistry-> GetPreferredPartHandler( ev, argv[0]. strptr);

        if ( partHandlerName)
        {
            if ( WriteStringToVar( argv[1]. strptr, partHandlerName) == RXSHV_BADN)
              return INVALID_ROUTINE;    /* error on non-zero          */
        }
        else
        {
            apiret = PR_PART_KIND_NOT_FOUND;
        }

        SOMFree( partHandlerName);

        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**                                                                               */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODSetPreferredPartHandler( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;

    ret-> strlength = 0;

    if ( argc == 2)
    {
        apiret = ODSetPreferredPartHandler( argv[0]. strptr, argv[1]. strptr);
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  ODQueryPreferredPartHandlerForFileType                                       */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPreferredPartHandlerForFileType  ( PUCHAR func,
                            ULONG argc,
                            PRXSTRING argv,
                            PSZ que,
                            PRXSTRING ret)
{
    ULONG      rc     = 0;
    APIRET     apiret = RM_NO_ERROR;
    ISOString  partHandlerName;
    LONG       length = 0;
    SHVBLOCK shvb;                     /* Request block for RxVar    */

    ret-> strlength = 0;

    if ( argc == 2)
    {
        Initialize();

        partHandlerName = odRegistry-> GetPreferredPartHandlerForFileType( ev, argv[0]. strptr);

        if ( partHandlerName)
        {
            if ( WriteStringToVar( argv[1]. strptr, partHandlerName) == RXSHV_BADN)
              return INVALID_ROUTINE;    /* error on non-zero          */
        }
        else
        {
            apiret = PR_PART_KIND_NOT_FOUND;
        }

        SOMFree( partHandlerName);

        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**                                                                               */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODSetPreferredPartHandlerForFileType( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;

    ret-> strlength = 0;

    if ( argc == 2)
    {
        apiret = ODSetPreferredPartHandlerForFileType( argv[0]. strptr, argv[1]. strptr);
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}
/**********************************************************************************/
/**  ODQueryPreferredPartHandlerForFileExt                                       */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPreferredPartHandlerForFileExt  ( PUCHAR func,
                            ULONG argc,
                            PRXSTRING argv,
                            PSZ que,
                            PRXSTRING ret)
{
    ULONG      rc     = 0;
    APIRET     apiret = RM_NO_ERROR;
    ISOString  partHandlerName;
    LONG       length = 0;
    SHVBLOCK shvb;                     /* Request block for RxVar    */

    ret-> strlength = 0;

    if ( argc == 2)
    {
        Initialize();

        partHandlerName = odRegistry-> GetPreferredPartHandlerForFileExt( ev, argv[0]. strptr);

        if ( partHandlerName)
        {
            if ( WriteStringToVar( argv[1]. strptr, partHandlerName) == RXSHV_BADN)
              return INVALID_ROUTINE;    /* error on non-zero          */
        }
        else
        {
            apiret = PR_PART_KIND_NOT_FOUND;
        }

        SOMFree( partHandlerName);

        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  ODSetPreferredPartHandlerForFileExt                                          */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODSetPreferredPartHandlerForFileExt( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;

    ret-> strlength = 0;

    if ( argc == 2)
    {
        apiret = ODSetPreferredPartHandlerForFileExt( argv[0]. strptr, argv[1]. strptr);
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  ODQueryPreferredPartHandlerForCategory                                       */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODQueryPreferredPartHandlerForCategory ( PUCHAR func,
                            ULONG argc,
                            PRXSTRING argv,
                            PSZ que,
                            PRXSTRING ret)
{
    ULONG      rc     = 0;
    APIRET     apiret = RM_NO_ERROR;
    ISOString  partHandlerName;
    LONG       length = 0;
    SHVBLOCK shvb;                     /* Request block for RxVar    */

    ret-> strlength = 0;

    if ( argc == 2)
    {
        Initialize();

        partHandlerName = odRegistry-> GetPreferredPartHandlerForCategory(ev, argv[0]. strptr);

        if ( partHandlerName)
        {
            if ( WriteStringToVar( argv[1]. strptr, partHandlerName) == RXSHV_BADN)
              return INVALID_ROUTINE;    /* error on non-zero          */
        }
        else
        {
            apiret = PR_PART_KIND_NOT_FOUND;
        }

        SOMFree( partHandlerName);

        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

/**********************************************************************************/
/**  ODSetPreferredPartHandlerForCategory                                         */
/**                                                                               */
/**********************************************************************************/
ULONG APIENTRY RxODSetPreferredPartHandlerForCategory( PUCHAR    func,
                            ULONG     argc,
                            PRXSTRING argv,
                            PSZ       que,
                            PRXSTRING ret)
{
    ULONG rc = 0;
    APIRET apiret = 0;

    ret-> strlength = 0;

    if ( argc == 2)
    {
        apiret = ODSetPreferredPartHandlerForCategory( argv[0]. strptr, argv[1]. strptr);
        sprintf( ret-> strptr, "%d", apiret);
        ret-> strlength = strlen( ret-> strptr);
    }
    else
    {
        rc = -1;
    }
    return rc;
}
#endif // PLATFORM OS2  REXX SCRIPT INTERFACE
#endif // defect 22796
